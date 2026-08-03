// MainCalcCore.cpp
// Standard C++ translation of 17 pure calc-core functions from BCB6 main.cpp
// (V3.33.906.0_20260618) -- CALC CORE ONLY.
// Source of truth: HT9011UC_Code_V3.33.906.0_20260618/main.cpp
//
// AI(ht9045-v899) 20260722: new file. See MainCalcCore.h for full scope rationale
// (extract-calc-core, same convention as cContact.h/.cpp; the rest of main.cpp/.h/.dfm
// is OUT OF SCOPE this wave).
//
// AI(ht9045-v899) 20260723: batch 2 appended below (5 more functions) -- see MainCalcCore.h's
// updated banner for the full rationale.
//
// AI(W906-maincalccore) 20260727: batch 3 appended below (3 more functions:
// ComputeSMCDLLVersionMismatchCode / ComputeATPDLLVersionMismatch / ComputeJamRateRecordStrings)
// -- see MainCalcCore.h's updated banner for the full rationale and OUT-OF-SCOPE list.
//
// AI(W906-maincalccore) 20260728: batch 4 appended below (1 more function:
// ComputeSetESDTriTempCommand) -- see MainCalcCore.h's updated banner for the TriTemp_Ch/
// ESD_Temperature header archaeology and the 2 rejected candidates (Tri_Temp_Set_Site,
// Tri_Temp_ChangeATCSiteUse -- both write shared extern globals and/or VCL, not pure).
//
// AI(W906-calccore) 20260804: batch 5 appended below (2 more functions:
// ComputeCheckAutoOnlySetOneBin / ComputeCheckAuto1OnlyBin1) -- the two remaining cheap
// pure-calc Check* functions from golden main.cpp. Each keeps its pure bool decision here and
// hands the ShowMyMessage() dialog TEXT back through OUT-param AnsiString(s), the same split
// ComputeCheckSiteMapState already uses; the dialog call belongs to a future ht9045_sm wrapper.
// CheckSLKSensor is deliberately still NOT here -- it reads the global Cylinder[] hardware
// array (ht9045_sm substrate). See MainCalcCore.h's corrected banner for both points.
//
// Toolchain: MinGW g++ 6.3+, C++14 or later.

#include "MainCalcCore.h"

// ---------------------------------------------------------------------------
// ComputeShtModeFlag
//   BCB6 source: main.cpp:34423-34442 (TfMain::GetShtModeFlag)
// ---------------------------------------------------------------------------
int ComputeShtModeFlag(int iShuttleMode, int iShuttle_Sel)
{
    // BCB6 :34425
    int iFlag = 0;

    // BCB6 :34426-34440
    if (iShuttleMode == 1)
    {
        if (iShuttle_Sel == 1)
        {
            iFlag = 1;
        }
        else
        {
            iFlag = 0;
        }
    }
    else
    {
        iFlag = 2;
    }
    return iFlag;
}

// ---------------------------------------------------------------------------
// ComputeCanChangeRealDummy
//   BCB6 source: main.cpp:12374-12380 (TfMain::CheckCanChangeRealDummy)
// ---------------------------------------------------------------------------
bool ComputeCanChangeRealDummy(bool bPlate1HasIC,
                                bool bPlate2HasIC,
                                bool bShuttleHasIC,
                                bool bIndexHasIC,
                                bool bInArmSuckHasIC,
                                bool bOutArmSuckHasIC)
{
    // BCB6 :12376  kevin 20120501  Autoclean
    if (bPlate1HasIC || bPlate2HasIC) return false;
    // BCB6 :12377
    if (bShuttleHasIC || bIndexHasIC) return false;
    // BCB6 :12378
    if (bInArmSuckHasIC || bOutArmSuckHasIC) return false;
    return true;
}

// ---------------------------------------------------------------------------
// ComputeSiteMapIsStander
//   BCB6 source: main.cpp:23526-23543 (TfMain::CheckSiteMapIsStander)
// ---------------------------------------------------------------------------
bool ComputeSiteMapIsStander(int iShtRow,
                              int iShtCol,
                              const int iSiteMap[][MAX_SOCKET_COL])
{
    // BCB6 :23528-23529
    int count = 1;
    int iRowCount = iShtRow;

    // BCB6 :23531-23541
    for (int iR = 0; iR < iShtRow; iR++)
    {
        count = iR + 1;   // 每一個層列的第一個數值 [0][0]=1 [1][0]=2 [2][0]=3 [3][0]=4
        for (int iC = 0; iC < iShtCol; iC++)
        {
            if (iSiteMap[iR][iC] != count &&
                iSiteMap[iR][iC] != 0)                 // Ifor 20251002 add: 避免site誤判
                return false;
            count = count + iRowCount;
        }
    }
    return true;
}

// ---------------------------------------------------------------------------
// ComputeSiteMapPriority
//   BCB6 source: main.cpp:31187-31208 (TfMain::CheckSiteMapPriority)
//
//   NOTE: iMax is computed (running max over iAutoSiteMap) exactly as golden does, but --
//   same as golden -- it is never read again after the loop.  See header comment: this is
//   preserved dead tracking, not "cleaned up", per this wave's faithful-translation mandate.
// ---------------------------------------------------------------------------
bool ComputeSiteMapPriority(const int iAutoSiteMap[][MAX_SOCKET_COL],
                             int iSiteTotal)
{
    // BCB6 :31189-31191  kevin 20180302 32->33
    int iDouble[33];
    int iMax = 0;
    for (int k = 0; k < 33; k++) iDouble[k] = 0;   // BCB6 ZeroMemory(iDouble, sizeof(iDouble))

    // BCB6 :31192-31200
    for (int i = 0; i < MAX_SOCKET_ROW; i++)
    {
        for (int j = 0; j < MAX_SOCKET_COL; j++)
        {
            if (iMax <= iAutoSiteMap[i][j])
                iMax = iAutoSiteMap[i][j];
            iDouble[iAutoSiteMap[i][j]]++;
        }
    }

    // BCB6 :31202-31206
    for (int i = 1; i <= iSiteTotal; i++)
    {
        if (iDouble[i] >= 2)
            return false;
    }
    return true;
}

// ---------------------------------------------------------------------------
// ComputeCanChangeSite
//   BCB6 source: main.cpp:14350-14393 (TfMain::CanChangeSite)
// ---------------------------------------------------------------------------
bool ComputeCanChangeSite(bool bNoIncludeHotplate,
                           bool bCanAutoCloseSite,
                           bool bI28_OnOffSiteOnTheFly,
                           bool bInArmSuckHasIC,
                           bool bInputShuttleHasIC,
                           bool bIndexHasIC,
                           bool bPlate1HasIC,
                           bool bPlate2HasIC,
                           bool bPickFromLoader,
                           int  iPickFromLoadStageTask)
{
    // BCB6 :14352  Steven 20200420 : 獨立Auto Site Off
    if (bCanAutoCloseSite)
    {
        // BCB6 :14354  kevin 20211103 add I28
        if (bI28_OnOffSiteOnTheFly == false)
        {
            if (bInArmSuckHasIC ||
                bInputShuttleHasIC ||
                bIndexHasIC)
                return false;
        }
    }
    else
    {
        // BCB6 :14364  kevin 20211103 add I28
        if (bI28_OnOffSiteOnTheFly == false)
        {
            // BCB6 :14366  ChungHung 20120713 add OneCycle Can Enable Site
            if (bNoIncludeHotplate)
            {
                if (bInArmSuckHasIC ||
                    bInputShuttleHasIC ||
                    bIndexHasIC)
                    return false;
            }
            else
            {
                if (bPlate1HasIC ||
                    bPlate2HasIC ||
                    bInArmSuckHasIC ||
                    bInputShuttleHasIC ||
                    bIndexHasIC)
                    return false;
            }
        }
        else
        {
            if (bPickFromLoader == true ||
                iPickFromLoadStageTask != 1)
            {
                return false;
            }
        }
    }
    return true;
}

// ---------------------------------------------------------------------------
// ComputeCanChangeToSocket
//   BCB6 source: main.cpp:30156-30163 (TfMain::CanChangeToSocket)
// ---------------------------------------------------------------------------
bool ComputeCanChangeToSocket(bool bSystemStart,
                               bool bCanAutoCloseSite,
                               bool bI28_OnOffSiteOnTheFly,
                               bool bInArmSuckHasIC,
                               bool bInputShuttleHasIC,
                               bool bIndexHasIC,
                               bool bPlate1HasIC,
                               bool bPlate2HasIC,
                               bool bPickFromLoader,
                               int  iPickFromLoadStageTask)
{
    // BCB6 :30158
    if (bSystemStart)
        return false;
    // BCB6 :30160  CanChangeSite() called with golden default bNoIncludeHotplate==false
    if (ComputeCanChangeSite(false,
                             bCanAutoCloseSite,
                             bI28_OnOffSiteOnTheFly,
                             bInArmSuckHasIC,
                             bInputShuttleHasIC,
                             bIndexHasIC,
                             bPlate1HasIC,
                             bPlate2HasIC,
                             bPickFromLoader,
                             iPickFromLoadStageTask) == false)
        return false;
    return true;
}

// ---------------------------------------------------------------------------
// ComputeATCAmbientTemperCheck
//   BCB6 source: main.cpp:31942-31962 (TfMain::ATCAmbientTemperCheck, wei 20151014)
// ---------------------------------------------------------------------------
bool ComputeATCAmbientTemperCheck(double dATCAmbientTemperature,
                                   int iATC_SYSTEM,
                                   int iATC_MODE_TYPE)
{
    // BCB6 :31949-31951 ATC_TYPE_33/35/61 -- #define ints, ATC/ATC_Handler_Side.h:24,25,30
    // (33/35/61). Inlined locally, NOT pulled in via that header (see MainCalcCore.h banner).
    const int ATC_TYPE_33 = 33;
    const int ATC_TYPE_35 = 35;
    const int ATC_TYPE_61 = 61;

    // BCB6 :31944-31946  golden's own `==0` exact-double-equality "unconfigured" sentinel
    // check, preserved verbatim (not "fixed" to an epsilon compare).
    if (dATCAmbientTemperature == 0 ||
        dATCAmbientTemperature < 25 ||
        dATCAmbientTemperature > 30)
    {
        // BCB6 :31948-31951  eNewATCSystem from MachineType.h eATCType enum (==6)
        if (iATC_SYSTEM == eNewATCSystem &&
            (iATC_MODE_TYPE == ATC_TYPE_33 ||
             iATC_MODE_TYPE == ATC_TYPE_35 ||
             iATC_MODE_TYPE == ATC_TYPE_61))
        {
            // BCB6 :31953-31954
            if (dATCAmbientTemperature < -5)
                return false;
        }
        else
        {
            // BCB6 :31958
            return false;
        }
    }
    return true;
}

// ---------------------------------------------------------------------------
// ComputeCheckAllMOTHome
//   BCB6 source: main.cpp:15112-15125 (TfMain::Check_AllMOT_Home)
// ---------------------------------------------------------------------------
bool ComputeCheckAllMOTHome(const bool bAtHomeLed[])
{
    // BCB6 :15114  TOTAL_MOTOR -- #define int, cmydef.h:33 (==164).
    // BCB6 :15119  MTrayX/MInArmY/MOutArmY -- extern const int, cmydef.cpp:2361/2331/2351
    // (==30/1/20). All 4 inlined locally (NOT #included from cmydef.h -- see MainCalcCore.h
    // banner: avoids the ht9045_globals link surface this TU deliberately stays clear of).
    const int TOTAL_MOTOR = 164;
    const int MTrayX  = 30;
    const int MInArmY = 1;
    const int MOutArmY = 20;

    // BCB6 :15114-15123  golden's MOT[i].ScanMotorStatus() hardware poll + Led[iHomeLed]
    // read is pre-resolved by the caller into bAtHomeLed[i] (see header comment).
    for (int i = 0; i < TOTAL_MOTOR; i++)
    {
        if (bAtHomeLed[i] == false)                                            //不在home
        {
            if (i == MTrayX || i == MInArmY || i == MOutArmY)
                continue;
            return false;
        }
    }
    return true;
}

// ---------------------------------------------------------------------------
// ComputeCheckSiteMapState
//   BCB6 source: main.cpp:31148-31172 (TfMain::CheckSiteMapState(), NO-ARG overload only)
//
//   Returns the raw bHasErr decision (see header comment for the polarity note vs golden's
//   own inverted final return). ErrPart is an OUT param, appended to only (not cleared).
// ---------------------------------------------------------------------------
bool ComputeCheckSiteMapState(const int iSiteMap[][MAX_SOCKET_COL],
                               const int iDutOnOff0[][MAX_SOCKET_COL],
                               const AnsiString IndexSuckName[][MAX_SOCKET_COL],
                               AnsiString &ErrPart)
{
    // BCB6 :31150
    bool bHasErr = false;

    // BCB6 :31152-31163
    for (int i = 0; i < MAX_SOCKET_ROW; i++)
    {
        for (int j = 0; j < MAX_SOCKET_COL; j++)                               //Eliot 2009_12_25 4->8
        {
            if (iSiteMap[i][j] >= 1 &&
                iDutOnOff0[i][j] == 0)                                         //kevin 20150115 AutoSiteMap關閉的 site
            {
                ErrPart += IndexSuckName[i][j];
                bHasErr = true;
            }
        }
    }
    return bHasErr;
}

// ---------------------------------------------------------------------------
// ComputeCheckOLPErrorHasErr
//   PARTIAL extract of TfMain::CheckOLPError() -- BCB6 source: main.cpp:33986-34012
//   (Sam 20230921). Pure ">=3" reduction only; the 3 ShowMyMessagePWD() dialogs are OUT OF
//   SCOPE (see header comment).
// ---------------------------------------------------------------------------
bool ComputeCheckOLPErrorHasErr(const int OLPSetBinErr[10])
{
    // BCB6 :33988
    bool bHasErr = false;
    // BCB6 :33989-34010  (dialog branches at i==0/i==1/i==2 omitted -- out of scope)
    for (int i = 0; i < 10; i++)
    {
        if (OLPSetBinErr[i] >= 3)
        {
            bHasErr = true;
        }
    }
    return bHasErr;
}

// ---------------------------------------------------------------------------
// ComputeCheckARTSetupFile
//   BCB6 source: main.cpp:32211-32238 (TfMain::CheckARTSetupFile, wei 20160115)
// ---------------------------------------------------------------------------
bool ComputeCheckARTSetupFile(int iUSE_AUTO_RETEST,
                               bool bA10_AutoReTest,
                               const bool bAutoRetest[],
                               const int iCatDataT3Pos[],
                               int iTestBinCount,
                               const AnsiString &aFileName_ART)
{
    // BCB6 :32215  USE_AUTO_RETEST (cmydef.h:2972, extern int) vs eartInstall (MachineType.h
    // eAutoRetest enum, ==1); IniConfig.bA10_AutoReTest resolved to explicit bool param.
    if (iUSE_AUTO_RETEST == eartInstall && bA10_AutoReTest)
    {
        // BCB6 :32217  eAuto3 -- MachineType.h e6TrayName enum (==2); header-only, no inlining
        // needed (unlike TOTAL_MOTOR/ATC_TYPE_* above -- see header comment).
        for (int i = 0; i <= eAuto3; i++)
        {
            // BCB6 :32219
            for (int j = 0; j < iTestBinCount; j++)
            {
                // BCB6 :32221-32222  BinSelect[FT_ART] pre-indexed by caller (see header)
                if (bAutoRetest[i] == true &&
                    iCatDataT3Pos[j] == i)
                {
                    // BCB6 :32224-32231  7 Pos() checks, Ifor 20170424/20200914/20190711
                    // (wei/KYEC customer-code requests) -- reproduced in golden's exact order.
                    if (aFileName_ART.Pos("_ART") != 0 ||                      //Ifor 20170424 (wei) KYEC customer-code request
                        aFileName_ART.Pos("9203") == 1 ||
                        aFileName_ART.Pos("5611") == 1 ||
                        aFileName_ART.Pos("9287") == 1 ||
                        aFileName_ART.Pos("3971") == 1 ||
                        aFileName_ART.Pos("9606") == 1 ||
                        aFileName_ART.Pos("KL")   == 1 ||                      //Ifor 20200914 Fix: HISI => KL
                        aFileName_ART.Pos("9378") == 1)                        //Ifor 20190711 : KYEC customer code 9378
                        return true;
                }
            }
        }
    }
    return false;
}

// ---------------------------------------------------------------------------
// ComputeSMCDLLVersionMismatchCode
//   BCB6 source: main.cpp:303-318 (decision tail of free function DoCheckSMCDLLVersion())
// ---------------------------------------------------------------------------
int ComputeSMCDLLVersionMismatchCode(const AnsiString &strSMCVersion0,
                                      const AnsiString &strSMCVersion1)
{
    // BCB6 :303-318  CSMCDLLRevision -- MachineType.h:111 #define string literal, already
    // visible via this file's own MachineType.h include (see header comment).
    if (strSMCVersion0 != CSMCDLLRevision && strSMCVersion1 != CSMCDLLRevision)
    {
        return 3;
    }
    else if (strSMCVersion0 != CSMCDLLRevision)
    {
        return 1;
    }
    else if (strSMCVersion1 != CSMCDLLRevision)
    {
        return 2;
    }
    else
    {
        return 0;
    }
}

// ---------------------------------------------------------------------------
// ComputeATPDLLVersionMismatch
//   BCB6 source: main.cpp:348-355 (decision tail of free function DoCheckATPDLLVersion())
// ---------------------------------------------------------------------------
int ComputeATPDLLVersionMismatch(const AnsiString &sATPDLLVersion)
{
    // BCB6 :348-355  ATPDLLVersion -- MachineType.h:113 #define string literal, already
    // visible via this file's own MachineType.h include (see header comment).
    if (sATPDLLVersion != ATPDLLVersion)
    {
        return 1;
    }
    else
    {
        return 0;                                                             //OK
    }
}

// ---------------------------------------------------------------------------
// ComputeJamRateRecordStrings
//   PARTIAL extract of TfMain::RecordJamRateByTime() -- BCB6 source: main.cpp:32047-32067.
//   See header comment for the full OUT-OF-SCOPE list (clock-gate/counter-resets/RecordProcess)
//   and the two preserved golden quirks (fixed "MTBF 1/" literal; MTBF-labelled-but-actually-a-
//   rate formula).
// ---------------------------------------------------------------------------
void ComputeJamRateRecordStrings(int iRecordJamRateByTime_JamCount,
                                  int iRecordJamRateByTime_LoaderCount,
                                  int iRecordJamRateIntervalTime,
                                  AnsiString &sMTBFRecord,
                                  AnsiString &sJamRateRecord)
{
    // BCB6 :32047-32049
    sMTBFRecord = "";
    double fH;
    int iH;

    // BCB6 :32051-32064  GOLDEN QUIRK (see header): the else branch's "MTBF 1/" is a fixed
    // literal digit, NOT iRecordJamRateByTime_JamCount -- preserved verbatim, not "fixed".
    if (iRecordJamRateByTime_JamCount == 0)
    {
        // BCB6 :32053-32056
        fH = iRecordJamRateIntervalTime / 60.;
        iH = (int)(fH * 1000);
        fH = double(iH) / 1000.;
        sMTBFRecord = AnsiString("  MTBF 0/") + AnsiString(fH) + AnsiString("hr");
    }
    else
    {
        // BCB6 :32060-32063  GOLDEN QUIRK (see header): jams-per-hour RATE, not an actual
        // mean-time-between-failures (hours-per-jam) -- preserved verbatim, not "corrected".
        fH = iRecordJamRateByTime_JamCount / (iRecordJamRateIntervalTime / 60.);
        iH = (int)(fH * 1000);
        fH = double(iH) / 1000.;
        sMTBFRecord = AnsiString("  MTBF 1/") + AnsiString(fH) + AnsiString("hr");
    }

    // BCB6 :32066-32067
    sJamRateRecord = "";
    sJamRateRecord = AnsiString("[Jam Rate Record] ") +
                      AnsiString(iRecordJamRateByTime_JamCount) + "/" +
                      AnsiString(iRecordJamRateByTime_LoaderCount) + sMTBFRecord;
}

// ---------------------------------------------------------------------------
// ComputeSetESDTriTempCommand
//   BCB6 source: main.cpp:34528-34550 (TfMain::SET_ESD_Tri_Temp, Ztex 2023.04.19)
// ---------------------------------------------------------------------------
int ComputeSetESDTriTempCommand(int iUSE_NOVX3360,
                                 int iTriTempMachine,
                                 double fWorkTemperBase,
                                 int iTemperature)
{
    // BCB6 :34530  Tempture_Hot==1 / Tempture_AmbientHot==3 -- `const int` globals,
    // cmydef.cpp:2972-2973 (NOT header-only -- see MainCalcCore.h banner). Inlined locally.
    const int Tempture_Hot = 1;
    const int Tempture_AmbientHot = 3;

    // BCB6 :34534/34537/34539/34543/34547  ESD_COMMAND enum members, Interface/InterfaceSYS.h
    // (Ambient=82, Hot=83, Cold=84, SuperHot=88). Inlined locally (see MainCalcCore.h banner).
    const int ESD_TemperatureAmbient  = 82;
    const int ESD_TemperatureHot      = 83;
    const int ESD_TemperatureCold     = 84;
    const int ESD_TemperatureSuperHot = 88;

    // BCB6 :34530  outer guard -- USE_NOVX3360 compared as `==true` (raw-int idiom, see header)
    if (iUSE_NOVX3360 == true && iTriTempMachine == 1)
    {
        // BCB6 :34532-34540
        if (iTemperature == Tempture_Hot)
        {
            if (fWorkTemperBase >= 40 && fWorkTemperBase <= 130)
                return ESD_TemperatureHot;
            else if (fWorkTemperBase > 130)
                return ESD_TemperatureSuperHot;
            else if (fWorkTemperBase < 10)
                return ESD_TemperatureCold;
            // BCB6 GOLDEN QUIRK: fWorkTemperBase in [10,40) matches none of the 3 branches
            // above -- golden calls SendCommand_ESD() zero times for this sample. Preserved
            // verbatim: return the "no command" sentinel, not a guessed nearest command.
        }
        // BCB6 :34541-34548  AmbientHot branch AND the catch-all else both send the SAME
        // ESD_TemperatureAmbient command -- preserved verbatim (not collapsed away as
        // "redundant", golden itself keeps them as two separate branches).
        else if (iTemperature == Tempture_AmbientHot)
        {
            return ESD_TemperatureAmbient;
        }
        else
        {
            return ESD_TemperatureAmbient;
        }
    }
    return kNoESDTriTempCommand;
}

// ---------------------------------------------------------------------------
// ComputeCheckAutoOnlySetOneBin
//   BCB6 source: main.cpp:32523-32553 (TfMain::CheckAutoOnlySetOneBin, V3.27R.560
//   Ifor 20171213 (Steven))
//
//   Returns golden's RAW decision (true == violation found), no polarity inversion. Msg1/Msg2
//   are OUT params carrying the two arguments golden hands to ShowMyMessage(str1, str2) at
//   main.cpp:32544; that dialog call is OUT OF SCOPE (see header). Both are assigned ONLY on
//   the return-true path, matching golden's control flow.
// ---------------------------------------------------------------------------
bool ComputeCheckAutoOnlySetOneBin(bool bUsePassBinOnlyCanSetOneBin,
                                    const int iIsPassT6[eTrayCount],
                                    const int iT6CatData[],
                                    int iTestBinCount,
                                    const AnsiString s6TrayName[eTrayCount],
                                    AnsiString &Msg1,
                                    AnsiString &Msg2)
{
    // BCB6 :32525  golden's own dead initialiser -- always overwritten at :32534 before any
    // read. Kept verbatim, NOT cleaned up (faithfulness mandate).
    int iBinCount = 0;

    // BCB6 :32527  golden declares two fresh locals `AnsiString str1, str2;` here; they are
    // extracted as the Msg1/Msg2 OUT params instead (see header contract).

    // BCB6 :32528  CosFunction.bUsePassBinOnlyCanSetOneBin (CosFunction.h:144) as explicit param
    if (bUsePassBinOnlyCanSetOneBin == true)
    {
        // BCB6 :32530  eTrayCount -- MachineType.h:1104, last enumerator of e6TrayName (==33,
        // eMag14==32 at MachineType.h:1103); header-only, read directly (see header comment).
        for (int i = 0; i < eTrayCount; i++)
        {
            // BCB6 :32532  Prod.iIsPassT6[i] (cprod.h:1068) as explicit param
            if (iIsPassT6[i] == 1)                                             //Steven 20240105 : Prod.bIsPass --> Prod.iIsFailT6
            {
                // BCB6 :32534  re-init once per Pass-marked TRAY, not per bin
                iBinCount = 0;
                // BCB6 :32535  iTestBinCount (cmydef.h:3396) bounds the scan, NOT TEST_MAX_BIN
                for (int j = 0; j < iTestBinCount; j++)
                {
                    // BCB6 :32537  Prod.iT6CatData[j] (cprod.h:512, "Auto1 = 0" 0-based)
                    if (iT6CatData[j] == i)
                    {
                        iBinCount++;
                        // BCB6 :32540  violation fires on the SECOND matching bin, not the first
                        if (iBinCount > 1)
                        {
                            // BCB6 :32542-32543  the raw-AnsiString-to-%s BCB6 idiom is
                            // preserved verbatim (NO .c_str() added): vclcompat's
                            // AnsiString::sprintf is a variadic template whose private conv()
                            // overload turns an AnsiString arg into const char*
                            // (vclcompat/AnsiString.h:141-145 and :177; see that file's own
                            // banner at :33-37, which calls out this exact idiom).
                            Msg1.sprintf("%s Pass Bin Only Can Set One Bin.", s6TrayName[i]);
                            Msg2.sprintf("%s Pass Bin 僅可設定一個Bin", s6TrayName[i]);
                            // BCB6 :32544  ShowMyMessage(str1, str2) -- OUT OF SCOPE (header)
                            // BCB6 :32545
                            return true;
                        }
                    }
                }
            }
        }
    }
    // BCB6 :32552
    return false;
}

// ---------------------------------------------------------------------------
// ComputeCheckAuto1OnlyBin1
//   BCB6 source: main.cpp:32502-32521 (TfMain::CheckAuto1OnlyBin1, Alick 20160802 add for
//   SCC Auto1 Only Bin1 Function)
//
//   Returns golden's RAW decision (true == violation found), no polarity inversion. Msg is the
//   single S1 argument of golden's one-argument ShowMyMessage calls (main.cpp:32511/32516;
//   mymessbox.h:58 defaults S2 to ""), assigned ONLY on a return-true path. The dialog call
//   itself is OUT OF SCOPE (see header).
//
//   The EARLY-RETURN ORDERING below is LOAD-BEARING and preserved bit-for-bit -- see the header
//   comment for the full analysis of the asymmetric `i==1` / `i!=1` conjuncts.
// ---------------------------------------------------------------------------
bool ComputeCheckAuto1OnlyBin1(int iCUSTOMER_CODE,
                                int iTester,
                                const int iT6PosCate[],
                                int iTestBinCount,
                                AnsiString &Msg)
{
    // BCB6 :32504  OFF_LINE -- `#define OFF_LINE  0`, cmydef.h:85, a header this TU does NOT
    // include (see MainCalcCore.h banner: avoids the ht9045_globals link surface). Inlined
    // locally, same treatment as ATC_TYPE_33/35/61 / TOTAL_MOTOR / Tempture_Hot above.
    const int OFF_LINE = 0;

    // BCB6 :32504-32505  Ifor 20171213 (Steven) : add UNISEM_M 要求Offline 不卡 Auto1 Only Bin1 功能
    // CC_UNISEM_M -- MachineType.h:358 (#define ==981), header-only, read directly. This is a
    // BYPASS returning FALSE ("no violation") BEFORE the loop is ever entered -- it is NOT a
    // violation report, and its position ahead of the loop is part of the preserved ordering.
    if (iCUSTOMER_CODE == CC_UNISEM_M && iTester == OFF_LINE)
        return false;

    // BCB6 :32507  iTestBinCount (cmydef.h:3396) bounds the scan, NOT TEST_MAX_BIN
    for (int i = 0; i < iTestBinCount; i++)
    {
        // BCB6 :32509  the `&& i == 1` conjunct is GOLDEN'S: this branch can fire on exactly
        // ONE iteration (i==1), while the branch below fires for ANY i!=1. Because both return
        // immediately, i==0 is tested against the SECOND branch before i==1 is ever reached.
        // Preserved verbatim -- ePosAuto1 is MachineType.h:1146 (eBinPositionName, ==1), and
        // iT6PosCate (cprod.h:513) is the 1-BASED "Auto1 = 1" array, not iT6CatData's 0-based one.
        if (iT6PosCate[i] != ePosAuto1 && i == 1)
        {
            // BCB6 :32511  ShowMyMessage(<this literal>) -- call OUT OF SCOPE, text kept
            Msg = "Bin1未被設定在Auto1 !! 請確認 !!\n Bin1 is not set to Auto1 !! Please Check !!";
            // BCB6 :32512
            return true;
        }
        // BCB6 :32514
        else if (iT6PosCate[i] == ePosAuto1 && i != 1)
        {
            // BCB6 :32516  ShowMyMessage(<this literal>) -- call OUT OF SCOPE, text kept
            Msg = "只有Bin 1被允許設為 Auto1 !! 請確認 !!\n Only Bin 1 is allowed to set to Auto 1!!! Please check !!";
            // BCB6 :32517
            return true;
        }
    }
    // BCB6 :32520
    return false;
}
