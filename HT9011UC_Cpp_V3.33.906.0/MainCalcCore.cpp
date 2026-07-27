// MainCalcCore.cpp
// Standard C++ translation of 11 pure calc-core functions from BCB6 main.cpp
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
