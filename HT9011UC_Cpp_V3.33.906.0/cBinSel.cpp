// =============================================================================
//  cBinSel.cpp  --  FW-3 表單波 (FW-BinSel-WA): TfBinSel, WAVE A subset
//
//  Translation wave: FW-BinSel-WA
//  Translator: AI(W906-FW-BinSel-WA) 20260819
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/cBinSel.cpp (6,652 lines)
//  + cBinSel.h (285 lines), cp950/Big5. Decoded this wave with
//  `python3 -c "open(path,'rb').read().decode('cp950')"` -- 0 U+FFFD over
//  both files (measured before any line below was written).
//
//  See forms/fBinSel.h for the full WAVE A / WAVE B split, GATE REGISTER,
//  WRITE-PATH GATE TABLE, CTOR NOTE, DESIGN NOTE and STUB COLLISION SCAN --
//  not duplicated here to avoid the two files drifting apart.
//
//  ABSENCE-CLAIM TIMESTAMPS (commands + when run, this wave, before writing
//  the citing code below -- re-run at hand-off per project policy)
//  --------------------------------------------------------------------------
//    fQAMode           : `grep -rn "\bfQAMode\b" --include=*.h .` -- 0 hits
//                         (20260819).
//    AutoForm[]        : `grep -rn "AutoForm\[" --include=*.h .` -- 0 hits
//                         (20260819).
//    fMain->SetOpenBin : `grep -n "SetOpenBin" forms/fMain.h` -- 0 hits
//                         (20260819).
//    fShowBinSelect->ShowBinSel : forms/fShowBinSelect.h's OWN WAVE B QUEUE
//                         (that file's banner, golden :388-757) -- not this
//                         wave's gap, cited from there.
//    class TfBinSel / fBinSel : `grep -rn "class TfBinSel" --include=*.h .`
//                         and `grep -rn "\bfBinSel\b" --include=*.cpp
//                         --include=*.h .` -- every hit a comment or a
//                         pre-existing `#if 0` GATE awaiting this file (see
//                         forms/fBinSel.h STUB COLLISION SCAN for the full
//                         line-by-line list), 20260819.
// =============================================================================
#include "forms/fBinSel.h"

#include "MachineType.h"        // eBinType enum, CC_* customer codes, TEST_MAX_BIN, e3*/e6* position enums
#include "cmydef.h"              // AccessLevel/bCancelErrorBin/bSpecificBin[]/iBinModelPrime/iBinBoxAtFix/
                                   // bAutoReTest_ART/bPurgeOutAllDevice/bGPIBChangeAlarm/bSetByDLL/
                                   // bTCPIPChangeAlarm/bMOFileControlBinChangeAlarm/bSetBINCOUNT/bOLPSetCategroy/
                                   // s3TrayName[]/s6TrayName[]/iTo3Unload[]/iTo6Unload[]/iTo3PosUnload[]
#include "cprod.h"                // Prod/TestIF/TestIF_File/BinSelect[]/TrayForm/tAOISetup
#include "LastSet.h"               // LastSet
#include "Config.h"                 // IniConfig
#include "CosFunction.h"             // CosFunction
#include "cSocket.h"                  // LotSummary
#include "cinitial.h"                  // SetWorkParameter()
#include "common.h"                    // ReadIniData/WriteIniData/CheckAndReadIniData/CheckSectionExist/
                                         // GetRecipePath/GetRecipeFileName/GetLastOpenFN/DataPath/
                                         // MyForceDirectories/AddSpace
#include "canary_support.h"              // ShowMyMessage/RecordProcess
#include "ProductionInfo/FileInfo.h"      // FileInfo::PathCombin
#include "forms/fShowBinSelect.h"           // fShowBinSelect

#include <cstdlib>   // atoi

// S19 (see cSecurity.cpp's own identical note): `cMyDB.h` and
// `canary_support.h` both declare `RecordProcess`/`MyDBIProcessNew` with
// CONFLICTING default arguments -- including both in one TU errors either
// order. Targeted forward declaration instead of `#include "cMyDB.h"`,
// signature copied verbatim from cMyDB.h:81 (real body lives in
// SECSGEM/uHGemEquipment.cpp today, per that header's own HOMECOMING
// NOTICE).
void __fastcall MyDBIProcess(AnsiString asTable, AnsiString S1, AnsiString S2="");

//---------------------------------------------------------------------------
// golden TU-local declarations (cBinSel.cpp :47-121) -- reproduced here
// verbatim, same file-scope, same names, so every unqualified reference in
// the ported methods below resolves exactly as golden's own does.
//---------------------------------------------------------------------------
static const int iBinCount = TEST_MAX_BIN;                                     // 使用的Bin數量 (golden :121)

enum eBinSettingItems                                                          // golden :53-86 (TU-local)
{   eBinNo          =0,
    eScanning       =1,
    eDoubleContact  =2,
    eConsFail       =3,
    ePersentEnable  =4,
    ePersentIgnore  =5,
    ePersentNumber  =6,
    eCountEnable    =7,
    eCountIgnore    =8,
    eCountNumber    =9,

    eSpecialBinByArm            =10,
    eSpecialBinCountByArm       =11,
    eSpecialBinBySocket         =12,
    eSpecialBinCountBySocket    =13,

    eLowYield       =14,
    eArmYield       =15,
    eSiteYield      =16,
    eAutoCleanByBin =17,
    eAutoCleanBySite=18,

    eSpecBinBySiteCompareEnable      =19,
    eSpecBinBySiteCompareIgnore      =20,
    eSpecBinBySiteComparePercent     =21,
    eSpecBinByArmPerSiteCompareEnable=22,
    eSpecBinByArmPerSiteCompareIgnore=23,
    eSpecBinByArmPerSiteComparePercent=24,

    eBinNotUse      ,
    eBinSetting     ,
    eBinSetTotal    =(eBinSetting+eTrayCount)
};

bool bCanLinkT6[eTrayCount];                                                    // golden :52 (TU-local)
e6TrayName iLinkTrayMin =eFix2;                                                 // golden :49
e6TrayName iLinkTrayMax =eFix3;                                                 // golden :50

const AnsiString sTrayName[eTrayCount]={"Auto1", "Auto2", "Auto3",              // golden cBinSel.cpp :88-92
    "Fix1", "Fix2", "Fix3", "Fix4", "Fix5", "Fix6", "BulkBox",
    "Magazine1", "Magazine2","Magazine3", "Magazine4", "Magazine5", "Magazine6", "Magazine7",
    "Magazine8", "Magazine9","Magazine10", "Magazine11", "Magazine12", "Magazine13", "Magazine14"
};

//---------------------------------------------------------------------------
// AI(W906-FW-BinSel-WA) 20260819: SIOF homecoming -- real instance (see
// forms/fBinSel.h STATIC-INIT SAFETY -- this ctor's own body was read in
// full against golden before this global was defined; no cross-TU / file-I/O
// dependency found, so no `INIFileGeneral!=0` guard is used).
//---------------------------------------------------------------------------
TfBinSel *fBinSel = new TfBinSel();

//---------------------------------------------------------------------------
//  TfBinSel::TfBinSel -- DEVIATION bootstrap ctor. See forms/fBinSel.h CTOR
//  NOTE for why this is not a verbatim translation of golden's ctor body
//  (golden :970-1117).
//---------------------------------------------------------------------------
TfBinSel::TfBinSel()
{
    for (int i = 0; i < eBinTypeTotal; i++)                                    // golden :984-1076
    {
        sBinDoubleContact[i]=new TStringList();
        sBinConsFail[i]     =new TStringList();
        sBinEnableFail[i]   =new TStringList();
        sBinFailPercent[i]  =new TStringList();

        sBinFailIgnore[i]   =new TStringList();
        sBinCountEnable[i]  =new TStringList();
        sBinCountIgnore[i]  =new TStringList();
        sBinCountNumber[i]  =new TStringList();

        sSpecialBinByArm[i]         =new TStringList();
        sSpecialBinCountByArm[i]    =new TStringList();
        sSpecialBinBySocket[i]      =new TStringList();
        sSpecialBinCountBySocket[i] =new TStringList();

        sLowYield[i]   =new TStringList();
        sArmYield[i]   =new TStringList();
        sSiteYield[i]  =new TStringList();

        sBinTraySetT3PosName[i]=new TStringList();
        sBinTraySetT3Pos[i] =new TStringList();
        sBinType[i]         =new TStringList();
        sT3TrayType[i]      =new TStringList();
        sT6Retest[i]        =new TStringList();
        sBySiteClean[i]     =new TStringList();
        sByBinClean[i]      =new TStringList();
        sT3CateR[i]         =new TStringList();

        sSpecBinBySiteCompareEnable[i]       =new TStringList();
        sSpecBinBySiteCompareIgnore[i]       =new TStringList();
        sSpecBinBySiteComparePercent[i]      =new TStringList();
        sSpecBinByArmPerSiteCompareEnable[i] =new TStringList();
        sSpecBinByArmPerSiteCompareIgnore[i] =new TStringList();
        sSpecBinByArmPerSiteComparePercent[i]=new TStringList();
        sBinTrayLinked[i]   =new TStringList();
        sBinLinked[i]       =new TStringList();
        sMagazineSetup[i]   =new TStringList();

        sAOIBinTraySetting[i]   =new TStringList();
        sAOIBinTraySetting[i]   ->Add("0");

        for (int j = 0; j < iTestBinCount; j++)
        {
            sBinDoubleContact[i]->Add("0");
            sBinConsFail[i]     ->Add("0");
            sBinEnableFail[i]   ->Add("0");
            sBinFailPercent[i]  ->Add("0");

            sBinFailIgnore[i]   ->Add("0");
            sBinCountEnable[i]  ->Add("0");
            sBinCountIgnore[i]  ->Add("0");
            sBinCountNumber[i]  ->Add("0");

            sSpecialBinByArm[i]         ->Add("0");
            sSpecialBinCountByArm[i]    ->Add("0");
            sSpecialBinBySocket[i]      ->Add("0");
            sSpecialBinCountBySocket[i] ->Add("0");

            sLowYield[i]   ->Add("0");
            sArmYield[i]   ->Add("0");
            sSiteYield[i]  ->Add("0");

            sBinTraySetT3PosName[i] ->Add("NotUse");
            sBinTraySetT3Pos[i]     ->Add("0");
            sBinType[i]             ->Add("0");
            sBySiteClean[i]         ->Add("0");
            sByBinClean[i]          ->Add("0");

            sSpecBinBySiteCompareEnable[i]->Add("0");
            sSpecBinBySiteCompareIgnore[i]->Add("0");
            sSpecBinBySiteComparePercent[i]->Add("0");
            sSpecBinByArmPerSiteCompareEnable[i]->Add("0");
            sSpecBinByArmPerSiteCompareIgnore[i]->Add("0");
            sSpecBinByArmPerSiteComparePercent[i]->Add("0");

            sBinTrayLinked[i]   ->Add("0");
            sBinLinked[i]       ->Add("0");
            sMagazineSetup[i]   ->Add("0");
        }

        for (int j = e3Auto1; j < eTrayCount; j++)
        {
            sT3TrayType[i]->Add("0");
            sT3CateR[i]->Add("0");
        }

        for (int j = eAuto1; j <= iAutoRight; j++)
        {
            sT6Retest[i]->Add("0");
        }
    }

    // golden :1098-1102/:1104-1115 -- MyBinPanel population. Data-only this
    // wave (see forms/fBinSel.h CTOR NOTE); `->mtBinSelect->Width/XItem`,
    // `InitmtBinSelectData()` and the KYEC `->Panel->Color` tint are all
    // widget-half, no reader in this wave's ACTIVE methods -- dropped.
    for (int tag = 0; tag < eBinTypeTotal; tag++)
        MyBinPanel[tag] = new TMyBinPanelData();

    if (CosFunction.bDisableRTBinSet)                                          // golden :1078-1079
        cbTestMode->Items->Delete(1);

    if (CosFunction.bOffLineBin || USE_AUTO_RETEST==eartInstall)                // golden :1081-1084
        cbTestMode->Items->Add("Off-Line");

    if (CosFunction.bUseSCKART==false && USE_AUTO_RETEST==eartInstall)         // golden :1086-1090
    {
        cbTestMode->Items->Add("ART Re-Test");
        cbTestMode->Items->Add("ART Normal");
    }

    if (CosFunction.bUseMRTMode==true)                                         // golden :1092-1096
    {
        cbTestMode->Items->Add("MRT Re-Test");
        cbTestMode->Items->Add("MRT Normal");
    }

    // AI(W906-FW-BinSel-WA) 20260819: hydration, not a golden line -- see
    // forms/fBinSel.h HYDRATION. `iTestBinCount` isn't visible to the
    // header's own default member initializer (cmydef.h/cprod.h not
    // included there), so the placeholder `(32,2)` construction is
    // corrected here, where it is in scope.
    sgSpecificBin->ColCount = iTestBinCount;
}

//---------------------------------------------------------------------------
//  FormDestroy -- golden :1995-2088
//---------------------------------------------------------------------------
void TfBinSel::FormDestroy(TObject * /*Sender*/)
{
    try
    {
        for (int i = eBinTypeTotal-1; i >= 0; i--)
            delete MyBinPanel[i];

        for (int i = eBinTypeTotal-1; i >= 0; i--)
        {
            if (sBinDoubleContact[i]!=NULL) delete sBinDoubleContact[i];
            if (sBinConsFail[i]!=NULL) delete sBinConsFail[i];
            if (sBinEnableFail[i]!=NULL) delete sBinEnableFail[i];
            if (sBinFailPercent[i]!=NULL) delete sBinFailPercent[i];

            if (sBinFailIgnore[i]!=NULL) delete sBinFailIgnore[i];
            if (sBinCountEnable[i]!=NULL) delete sBinCountEnable[i];
            if (sBinCountIgnore[i]!=NULL) delete sBinCountIgnore[i];
            if (sBinCountNumber[i]!=NULL) delete sBinCountNumber[i];

            if (sSpecialBinByArm[i]!=NULL) delete sSpecialBinByArm[i];
            if (sSpecialBinCountByArm[i]!=NULL) delete sSpecialBinCountByArm[i];
            if (sSpecialBinBySocket[i]!=NULL) delete sSpecialBinBySocket[i];
            if (sSpecialBinCountBySocket[i]!=NULL) delete sSpecialBinCountBySocket[i];

            if (sLowYield[i]!=NULL) delete sLowYield[i];
            if (sArmYield[i]!=NULL) delete sArmYield[i];
            if (sSiteYield[i]!=NULL) delete sSiteYield[i];

            if (sBinTraySetT3PosName[i]!=NULL) delete sBinTraySetT3PosName[i];
            if (sBinTraySetT3Pos[i]!=NULL) delete sBinTraySetT3Pos[i];
            if (sBinType[i]!=NULL) delete sBinType[i];
            if (sT3TrayType[i]!=NULL) delete sT3TrayType[i];
            if (sT6Retest[i]!=NULL) delete sT6Retest[i];
            if (sBySiteClean[i]!=NULL) delete sBySiteClean[i];
            if (sByBinClean[i]!=NULL) delete sByBinClean[i];
            if (sT3CateR[i]!=NULL) delete sT3CateR[i];

            if (sSpecBinBySiteCompareEnable[i]!=NULL) delete sSpecBinBySiteCompareEnable[i];
            if (sSpecBinBySiteCompareIgnore[i]!=NULL) delete sSpecBinBySiteCompareIgnore[i];
            if (sSpecBinBySiteComparePercent[i]!=NULL) delete sSpecBinBySiteComparePercent[i];
            if (sSpecBinByArmPerSiteCompareEnable[i]!=NULL) delete sSpecBinByArmPerSiteCompareEnable[i];
            if (sSpecBinByArmPerSiteCompareIgnore[i]!=NULL) delete sSpecBinByArmPerSiteCompareIgnore[i];
            if (sSpecBinByArmPerSiteComparePercent[i]!=NULL) delete sSpecBinByArmPerSiteComparePercent[i];

            if (sBinTrayLinked[i]!=NULL) delete sBinTrayLinked[i];
            if (sBinLinked[i]!=NULL) delete sBinLinked[i];
            if (sMagazineSetup[i]!=NULL) delete sMagazineSetup[i];

            if (sAOIBinTraySetting[i]!=NULL) delete sAOIBinTraySetting[i];
        }
    }
    catch (...)
    {
        MyDBIProcess("Exception", "TfBinSel::FormDestroy", "");
    }
    LogSoftwareOffTime("TfBinSel, FormDestroy");
}

//---------------------------------------------------------------------------
//  FormClose -- golden :2099-2107
//---------------------------------------------------------------------------
void TfBinSel::FormClose(TObject * /*Sender*/)
{
    bShow=false;
    ReadFile(false, false, "");
    fShowBinSelect->InitShowBinDigital();
}

//---------------------------------------------------------------------------
//  ReadParam -- golden :2090-2097 (pure read)
//---------------------------------------------------------------------------
void TfBinSel::ReadParam()
{
    AnsiString szDir=GetRecipeFileName("Tester.Data");
    bUseContinueFail=ReadIniData(szDir, "Alarm", "SocketEnable", 0);
    if (bUseContinueFail!=true)
        bUseContinueFail=ReadIniData(szDir, "Alarm", "HeadEnable", 0);
    bUseFailRate=ReadIniData(szDir, "Alarm", "RateEnable", 0);
}

//---------------------------------------------------------------------------
//  ChangeActivePageIndex -- golden :2194-2212. See forms/fBinSel.h DESIGN
//  NOTE for the ActivePageIndex integer convention (0..6, matching golden's
//  own SaveOther literal comparisons).
//---------------------------------------------------------------------------
void TfBinSel::ChangeActivePageIndex()
{
    if (iTestRunMode==eBinFT)
        PageControl1->ActivePageIndex=0;        // tsNormal
    else if (iTestRunMode==eBinRT)
        PageControl1->ActivePageIndex=1;        // tsRetest
    else if (iTestRunMode==eBinOffLine)
        PageControl1->ActivePageIndex=2;        // tsOffline
    else if (iTestRunMode==eBinFT_ART)
        PageControl1->ActivePageIndex=3;        // tsArtFT
    else if (iTestRunMode==eBinRT_ART)
        PageControl1->ActivePageIndex=4;        // tsArtRT
    else if (iTestRunMode==eBinFT_MRT)
        PageControl1->ActivePageIndex=5;        // tsMrtFT
    else if (iTestRunMode==eBinRT_MRT)
        PageControl1->ActivePageIndex=6;        // tsMrtRT
    else
        ShowMyMessage("ChangeActivePageIndex error", "ChangeActivePageIndex 錯誤");
}

//---------------------------------------------------------------------------
//  CheckFix2Tray -- golden :4785-4805 (pure, MyBinPanelData)
//---------------------------------------------------------------------------
bool TfBinSel::CheckFix2Tray()
{
    if (tAOISetup.bEnabledAOI)
    {
        for (int tag = 0; tag < 3; tag++)
        {
            for (int i = 0; i < iTestBinCount; i++)
            {
                for (int j = eBinNotUse; j < eBinSetTotal; j++)
                {
                    if (MyBinPanel[tag]->BackT6PosTray[i][j])
                    {
                        if ((j-eBinNotUse)==ePosFix2)
                            return true;
                    }
                }
            }
        }
    }
    return false;
}

//---------------------------------------------------------------------------
//  CheckOSBin -- golden :6254-6266 (pure)
//---------------------------------------------------------------------------
bool TfBinSel::CheckOSBin()
{
    bool IsOK=true;
    if (CosFunction.bAutoSiteMappingSetOpenBIN==true)
    {
        if (Prod.iOpenBin>=0)
        {
            if (BinSelect[iTestRunMode].iCatDataT3Pos[Prod.iOpenBin]==0)
                IsOK=false;
        }
    }
    return IsOK;
}

//---------------------------------------------------------------------------
//  SetOSBin -- golden :6268-6284
//---------------------------------------------------------------------------
bool TfBinSel::SetOSBin(int iBin)
{
    bool IsOK=true;
    if (iBin>=-1 && iBin<16)
    {
        cbbAutoSiteMap->ItemIndex=iBin+1;
        ReadWriteMRTMode(1);
        ReadWriteMRTMode(0);
        if (CheckOSBin()==false)
            IsOK=false;
    }
    else
    {
        IsOK=false;
    }
    return IsOK;
}

//---------------------------------------------------------------------------
//  ReadWriteMRTMode -- golden :6133-6185. GATE (G4): fMain->SetOpenBin()
//  absent -- see forms/fBinSel.h GATE REGISTER. Write half (iMode==1) gated
//  per WRITE-PATH GATE TABLE.
//---------------------------------------------------------------------------
void TfBinSel::ReadWriteMRTMode(int iMode)
{
    AnsiString S=GetLastOpenFN();
    AnsiString szDir="";
    szDir.sprintf("%s%s\\HandlerCondition.Data", DataPath.c_str(), S.c_str());

    if (CosFunction.bUseMRTMode==true)
    {
        if (iMode==0)
        {
            TestIF_File.bEnableMRTMode=ReadIniData(szDir, "Configuration", "Enable MRT Mode", false);
            cbUseMRTMode->Checked     =TestIF_File.bEnableMRTMode;
        }
        else
        {
#if 0
            // GATE (write-policy): WriteIniData(szDir,"Configuration","Enable MRT Mode",...)
            WriteIniData(szDir, "Configuration", "Enable MRT Mode",(cbUseMRTMode->Checked)?1:0);
#endif
            TestIF_File.bEnableMRTMode=cbUseMRTMode->Checked;
        }
    }
    else
    {
        TestIF_File.bEnableMRTMode=false;

        if (CosFunction.bAutoSiteMappingSetOpenBIN==true)
        {
            if (iMode==0)
            {
                cbbAutoSiteMap->ItemIndex=ReadIniData(szDir, "Configuration", "ASM Open Bin", 0);
                TestIF_File.iOpenBin=cbbAutoSiteMap->ItemIndex-1;
                Prod.iOpenBin=TestIF_File.iOpenBin;
                // GATE (G4): fMain->SetOpenBin(); -- forms/fMain.h has no such
                // member (`grep -n "SetOpenBin" forms/fMain.h` -- 0 hits,
                // 20260819). Prod.iOpenBin above (the real, independently
                // observable state) is set regardless.
            }
            else
            {
#if 0
                // GATE (write-policy)
                WriteIniData(szDir, "Configuration", "ASM Open Bin", cbbAutoSiteMap->ItemIndex);
#endif
            }

            if (CosFunction.bAutoSiteMappingSetPassBIN==true)
            {
                if (iMode==0)
                {
                    cbbASMPassBin->ItemIndex=ReadIniData(szDir, "Configuration", "ASM Pass Bin", 2);
                    TestIF_File.iASMPassBin=cbbASMPassBin->ItemIndex-1;
                }
                else
                {
#if 0
                    // GATE (write-policy)
                    WriteIniData(szDir, "Configuration", "ASM Pass Bin", cbbASMPassBin->ItemIndex);
#endif
                }
            }
        }
    }
}

//---------------------------------------------------------------------------
//  ReadWriteSpecialFunction -- golden :6187-6252. Write half (bReadWrite==
//  true) gated per WRITE-PATH GATE TABLE.
//---------------------------------------------------------------------------
void TfBinSel::ReadWriteSpecialFunction(bool bReadWrite)
{
    if (IniConfig.bSPILFunction==true)
    {
        AnsiString S=GetLastOpenFN();
        AnsiString szDir="";
        szDir.sprintf("%s%s\\HandlerCondition.Data", DataPath.c_str(), S.c_str());

        if (CosFunction.bOutShtLoseICSetErrUntilOneCycle)
        {
            if (bReadWrite==false)
            {
                TestIF_File.bOutShtLoseICSetErrUntilOneCycle=ReadIniData(szDir, "Configuration", "bOutShtLoseICSetErrUntilOneCycle", false);
                cbOutShtLoseICSetErrUntilOneCycle->Checked  =TestIF_File.bOutShtLoseICSetErrUntilOneCycle;
            }
            else
            {
#if 0
                // GATE (write-policy)
                WriteIniData(szDir, "Configuration", "bOutShtLoseICSetErrUntilOneCycle", cbOutShtLoseICSetErrUntilOneCycle->Checked);
#endif
                TestIF_File.bOutShtLoseICSetErrUntilOneCycle=cbOutShtLoseICSetErrUntilOneCycle->Checked;
            }
        }
        else
        {
            TestIF_File.bOutShtLoseICSetErrUntilOneCycle=false;
        }

        if (CosFunction.bIndexDropICSetErrUntilOneCycle)
        {
            if (bReadWrite==false)
            {
                TestIF_File.bIndexDropICSetErrUntilOneCycle=ReadIniData(szDir, "Configuration", "bIndexDropICSetErrUntilOneCycle", false);
                cbIndexDropErrSetErrUntilOneCycle->Checked =TestIF_File.bIndexDropICSetErrUntilOneCycle;
            }
            else
            {
#if 0
                // GATE (write-policy)
                WriteIniData(szDir, "Configuration", "bIndexDropICSetErrUntilOneCycle", cbIndexDropErrSetErrUntilOneCycle->Checked);
#endif
                TestIF_File.bIndexDropICSetErrUntilOneCycle=cbIndexDropErrSetErrUntilOneCycle->Checked;
            }
        }
        else
        {
            TestIF_File.bIndexDropICSetErrUntilOneCycle=false;
        }
    }
    else
    {
        TestIF_File.bOutShtLoseICSetErrUntilOneCycle  =CosFunction.bOutShtLoseICSetErrUntilOneCycle  ? true : false;
        TestIF_File.bIndexDropICSetErrUntilOneCycle   =CosFunction.bIndexDropICSetErrUntilOneCycle   ? true : false;
    }
}

//---------------------------------------------------------------------------
//  ReadPrimeDara -- golden :6044-6057 (pure read + Wave-B stub call)
//---------------------------------------------------------------------------
void TfBinSel::ReadPrimeDara()
{
    AnsiString S=GetLastOpenFN();
    AnsiString szDir;

    szDir.sprintf("%s%s\\Binasgn.Data", DataPath.c_str(), S.c_str());
    iBinModelPrime=ReadIniData(szDir, "BinModel", "bPrime", 0);
    SetPrimeButton();

    if (CosFunction.bShow0Xbin)
    {
        chkShow0Xbin->Checked=ReadIniData(szDir, "Other", "bShow0XBin", false);
    }
}

//---------------------------------------------------------------------------
//  WritePrimeDara -- golden :6059-6068. Write gated per WRITE-PATH GATE
//  TABLE; `iBinModelPrime=Mode;` (real, in-memory) stays ACTIVE.
//---------------------------------------------------------------------------
void TfBinSel::WritePrimeDara(int Mode)
{
    AnsiString S=GetLastOpenFN();
    AnsiString szDir;

    szDir.sprintf("%s%s\\Binasgn.Data", DataPath.c_str(), S.c_str());
#if 0
    // GATE (write-policy)
    WriteIniData(szDir, "BinModel", "bPrime", Mode);
#endif
    iBinModelPrime=Mode;
    SetPrimeButton();
}

//---------------------------------------------------------------------------
//  ARTBinCheck -- golden :6381-6401 (pure, MyBinPanelData)
//---------------------------------------------------------------------------
void TfBinSel::ARTBinCheck(int tag)
{
    if (USE_AUTO_RETEST==eartInstall && IniConfig.bA10_AutoReTest &&
        (LastSet.iRunStartMode==rsmInitial_ART || LastSet.iRunStartMode==rsmContinuStart_ART || LastSet.iRunStartMode==rsmContinuRetest_ART))
    {
        for (int i = 0; i < 3; i++)
        {
            if (MyBinPanel[tag]->iT6IsFail[i]==0)
            {
                MyBinPanel[tag]->bT6ART[i]=0;
            }
        }

        if ((MyBinPanel[tag]->iT6IsFail[0]==0 && MyBinPanel[tag]->iT6IsFail[1]==0 && MyBinPanel[tag]->iT6IsFail[2]==0) ||
            (MyBinPanel[tag]->bT6ART[0]==0 && MyBinPanel[tag]->bT6ART[1]==0 && MyBinPanel[tag]->bT6ART[2]==0))
        {
            MyBinPanel[tag]->iT6IsFail[1]=1;
            MyBinPanel[tag]->bT6ART[1]=1;
        }
    }
}

//---------------------------------------------------------------------------
//  TransferBinTrayStrToName -- golden :6404-6423 (pure TStringList)
//---------------------------------------------------------------------------
void TfBinSel::TransferBinTrayStrToName(int iTag)
{
    int iT3;
    TStringList *sT3List;
    sT3List = new TStringList();
    sT3List->CommaText=sBinTraySetT3Pos[iTag]->CommaText;
    for (int i = 0; i < sT3List->Count; i++)
    {
        iT3=atoi(AnsiString(sT3List->Strings[i]).c_str());
        if (i<sBinTraySetT3PosName[iTag]->Count)
        {
            if (i>=iTestBinCount || iT3==0)
                sBinTraySetT3PosName[iTag]->Strings[i]="NotUse";
            else
                sBinTraySetT3PosName[iTag]->Strings[i]=s3TrayName[iT3-1];
        }
    }
    sT3List->Clear();
    delete sT3List;
}

//---------------------------------------------------------------------------
//  SetConFail -- golden :2274-2335. One WriteIniData gated per WRITE-PATH
//  GATE TABLE; MyForceDirectories also gated (directory-mutation prerequisite
//  of the gated write).
//---------------------------------------------------------------------------
void TfBinSel::SetConFail(int iBinasgnMode)
{
    int tag=0, iTray;
    AnsiString GroupStr;
    AnsiString szDir=GetRecipePath();
#if 0
    // GATE (write-policy): directory-mutation prerequisite of the gated write below
    MyForceDirectories(szDir);
#endif

    if (iBinasgnMode==eBinFT)
    {
        szDir=GetRecipeFileName("Binasgn.Data");
        tag=eBinFT;
    }
    else if (iBinasgnMode==eBinRT)
    {
        szDir=GetRecipeFileName("BinasgnOff.Data");
        tag=eBinRT;
    }
    else if (iBinasgnMode==eBinOffLine)
    {
        szDir=GetRecipeFileName("BinasgnOff-Line.Data");
        tag=eBinOffLine;
    }
    else if (iBinasgnMode==eBinFT_ART)
    {
        szDir=GetRecipeFileName("Binasgn_ART.Data");
        tag=eBinFT_ART;
    }
    else if (iBinasgnMode==eBinRT_ART)
    {
        szDir=GetRecipeFileName("BinasgnOff_ART.Data");
        tag=eBinRT_ART;
    }
    else if (iBinasgnMode==eBinFT_MRT)
    {
        szDir=GetRecipeFileName("Binasgn_MRT.Data");
        tag=eBinFT_MRT;
    }
    else if (iBinasgnMode==eBinRT_MRT)
    {
        szDir=GetRecipeFileName("Binasgn_MRT_RT.Data");
        tag=eBinRT_MRT;
    }
    (void)tag;   // only feeds the gated WriteIniData below in this wave

    bool bT3FailTray[e3TrayCount];
    for (int i = 0; i < e3TrayCount; i++)
    {
        bT3FailTray[i]=CheckAndReadIniData(szDir, s3TrayName[i], "Pass/Fail", 0);
    }

    for (int i = 0; i < iTestBinCount; i++)
    {
        GroupStr.sprintf("Category%d", i);
        iTray=CheckAndReadIniData(szDir, GroupStr, "Bin", (tag==eBinOffLine)?e3PosFix2:e3PosNoUse);
        if (iTray!=e3PosNoUse && bT3FailTray[iTray-1]==true)
        {
#if 0
            // GATE (write-policy)
            WriteIniData(szDir, GroupStr, "Cons.Fail", 1);
#endif
        }
    }
}

//---------------------------------------------------------------------------
//  Save -- golden :6296-6361. Every WriteIniData gated per WRITE-PATH GATE
//  TABLE; GATE (G5) fShowBinSelect->ShowBinSel() (Wave B in that file).
//---------------------------------------------------------------------------
void TfBinSel::Save(int iECIDCode, int iFileCode)
{
    int tag=iFileCode;

    ReadPrimeDara();
    AnsiString SavePath[]={"\\BinasgnOff.Data",
                           "\\Binasgn.Data",
                           "\\BinasgnOff_ART.Data",
                           "\\Binasgn_ART.Data",
                           "\\Binasgn_MRT_RT.Data",
                           "\\Binasgn_MRT.Data"};

    if (IniConfig.bA02BinModelPrime)
    {
        if (IniConfig.bFTBin2RTBin==true && (iBinModelPrime==0 || CosFunction.bDisableRTBinSet))
            SavePath[0]="\\Binasgn.Data";
        else
            SavePath[0]="\\BinasgnOff.Data";
    }
    else
    {
        if (IniConfig.bFTBin2RTBin==true || CosFunction.bDisableRTBinSet)
            SavePath[0]="\\Binasgn.Data";
        else
            SavePath[0]="\\BinasgnOff.Data";
    }

    AnsiString szDir=GetRecipePath()+SavePath[tag];
    (void)szDir;   // only feeds the gated writes below in this wave

    switch (iECIDCode)
    {
        case 3616:
#if 0
            // GATE (write-policy)
            WriteIniData(szDir, "I/F Error", "Bin", BinSelect[eBinFT].IfErrorT3);
            WriteIniData(szDir, "I/F Error", "Tray", s3TrayName[BinSelect[eBinFT].IfErrorT3]);
#endif
            ReadFile(false, false, "");
            // GATE (G5): fShowBinSelect->ShowBinSel(); -- forms/fShowBinSelect.h's
            // own WAVE B QUEUE (golden :388-757), not yet landed there.
            break;
        case 3617:
#if 0
            // GATE (write-policy)
            WriteIniData(szDir, "Bin Func FT", "3617 BinTraySetting", sBinTraySetT3Pos[eBinFT]->CommaText);
#endif
            TransferBinTrayStrToName(eBinFT);
#if 0
            // GATE (write-policy)
            WriteIniData(szDir, "Bin Func FT", "3617 BinTraySetName", sBinTraySetT3PosName[eBinFT]->CommaText);
#endif
            break;
        case 3677:
#if 0
            // GATE (write-policy)
            for (int i = 0; i < eTrayCount; i++)
                WriteIniData(szDir, s3TrayName[i], "Pass/Fail", sT3TrayType[eBinFT]->Strings[i]);
#endif
            break;
    }
}

//---------------------------------------------------------------------------
//  spbSaveClick -- golden :2214-2272. GATE (G6): fMain->SetStartModeData()/
//  BackupSetupFile() and the fLotInfo yield-monitor branch (WRITE-PATH GATE
//  TABLE + unverified fLotInfo surface). #ifdef ASE_KaohSiung block kept as
//  real conditional compilation (macro undefined in this CMake build, same
//  as golden's own BCB6-build-flag intent).
//---------------------------------------------------------------------------
void TfBinSel::spbSaveClick(TObject * /*Sender*/)
{
    if (IniConfig.bA02DisableSaveParsWhenSwitchToOp==true && AccessLevel==0)
    {
        ShowMyMessage("[A01_2]目前已切換到Operator權限，\r\n請重新登入再做設定!");
        // GATE (cosmetic): golden calls Close() here (VCL TForm::Close, no
        // non-VCL TfBinSel equivalent exists) -- the message above is the
        // observable effect this wave preserves.
    }

    if (CheckFix2Tray())
    {
        ShowMyMessage("己開啟AOI功能，不能使用Fix2 Tray");
        return;
    }

    Label1->Caption="Saving Data..";

    AnsiString szDir=GetRecipePath();
    SaveOther(szDir);

    Label1->Caption="Refresh data..";
    ReadWriteMRTMode(1);
    ReadWriteSpecialFunction(true);
    ReadFile(false, false, "");

    if (CheckOSBin()==false)
    {
        ShowMyMessage("SetOSBin Category Error", IntToStr(Prod.iOpenBin));
    }

    if (bSaveBin==false)
    {
        bSaveBin=true;
        RecordProcess("Save Bin");
    }

    if (IniConfig.bEnable_SECS_GEM==true)
    {
        bHasSaveSet=true;
    }
    fShowBinSelect->InitShowBinDigital();
    palSpecificBin->Visible=false;
    Label1->Caption="Save..OK";
    spbSave->Down=false;

    // GATE (G6): if(CosFunction.bUseMRTMode==true) fMain->SetStartModeData();
    //            fMain->BackupSetupFile();
    //            if(IniConfig.bSIGURDFunction && fLotInfo->pgLotinfo->
    //               ActivePage==fLotInfo->tsYieldMonitior)
    //                fLotInfo->RefreshYieldMonitor();
}

//---------------------------------------------------------------------------
//  SaveOther -- golden :2339-2753 ("kevin 20150105 另存一個 jobfile"). Every
//  FormSysTools->WriteIniData / OpenFormData+write-session pair below is
//  gated per WRITE-PATH GATE TABLE; the tag-selection chain, the two
//  pre-save VALIDATION checks (bP28Auto1OnlyBin1 / bUsePassBinOnlyCanSetOneBin
//  -- reads + ShowMyMessage + return, no write), ARTBinCheck, and every
//  in-memory sXxx[]/MyBinPanel[]-> update stay ACTIVE.
//---------------------------------------------------------------------------
void TfBinSel::SaveOther(AnsiString szDir)
{
    int tag=0;
    AnsiString FileName="";
#if 0
    // GATE (write-policy): directory-mutation prerequisite of the gated writes below
    MyForceDirectories(szDir);
#endif

    if (PageControl1->ActivePageIndex==1)
    {
        tag=eBinRT;
        if (IniConfig.bA02BinModelPrime)
        {
            if (IniConfig.bFTBin2RTBin==true && (iBinModelPrime==0 || CosFunction.bDisableRTBinSet))
                FileName=FileInfo().PathCombin(szDir, "Binasgn.Data");
            else
                FileName=FileInfo().PathCombin(szDir, "BinasgnOff.Data");
        }
        else
        {
            if (IniConfig.bFTBin2RTBin==true || CosFunction.bDisableRTBinSet)
                FileName=FileInfo().PathCombin(szDir, "Binasgn.Data");
            else
                FileName=FileInfo().PathCombin(szDir, "BinasgnOff.Data");
        }
    }
    else if (PageControl1->ActivePageIndex==2)
    {
        FileName=FileInfo().PathCombin(szDir, "BinasgnOff-Line.Data");
        tag=eBinOffLine;
    }
    else if (PageControl1->ActivePageIndex==3)
    {
        FileName=FileInfo().PathCombin(szDir, "Binasgn_ART.Data");
        tag=eBinFT_ART;
    }
    else if (PageControl1->ActivePageIndex==4)
    {
        FileName=FileInfo().PathCombin(szDir, "BinasgnOff_ART.Data");
        tag=eBinRT_ART;
    }
    else if (PageControl1->ActivePageIndex==5)
    {
        FileName=FileInfo().PathCombin(szDir, "Binasgn_MRT.Data");
        tag=eBinFT_MRT;
    }
    else if (PageControl1->ActivePageIndex==6)
    {
        FileName=FileInfo().PathCombin(szDir, "Binasgn_MRT_RT.Data");
        tag=eBinRT_MRT;
    }
    else
    {
        FileName=FileInfo().PathCombin(szDir, "Binasgn.Data");
        tag=eBinFT;
    }
    (void)FileName;   // only feeds the gated OpenFormData/WriteIniData sessions below

    if (IniConfig.bP28Auto1OnlyBin1==true)
    {
        int iErrCode=0;
        TStringList *sT3List;
        sT3List = new TStringList();
        sT3List->CommaText=sBinTraySetT3Pos[tag]->CommaText;
        if (CUSTOMER_CODE==CC_UNISEM_M && LastSet.iTester==OFF_LINE)
        {
        }
        else
        {
            for (int i = 0; i < iTestBinCount; i++)
            {
                if (i==1)
                {
                    if (sT3List->Strings[i]!=AnsiString("1"))
                    {
                        iErrCode=1;
                        break;
                    }
                }
                else if (sT3List->Strings[i]==AnsiString("1"))
                {
                    iErrCode=2;
                    break;
                }
            }
        }
        sT3List->Clear();
        delete sT3List;
        if (iErrCode==1)
        {
            ShowMyMessage("Bin1未被設定在Auto1 !! 請確認 !!\n Bin1 is not set to Auto1 !! Please Check !!");
            return;
        }
        else if (iErrCode==2)
        {
            ShowMyMessage("只有Bin 1被允許設為 Auto1 !! 請確認 !!\n Only Bin 1 is allowed to set to Auto 1!!! Please check !!");
            return;
        }
    }

    if (CosFunction.bUsePassBinOnlyCanSetOneBin==true)
    {
        int iBinCt=0;
        AnsiString str1, str2;
        TStringList *sT3List;
        sT3List = new TStringList();
        sT3List->CommaText=sBinTraySetT3Pos[tag]->CommaText;
        for (int i = 0; i < iAutoRight; i++)
        {
            if (MyBinPanel[tag]->iT6IsFail[i]==0)
            {
                iBinCt=0;
                for (int j = 0; j < iTestBinCount; j++)
                {
                    if (sT3List->Strings[j]==AnsiString(iTo3Unload[i]+1))
                    {
                        iBinCt++;
                        if (iBinCt>1)
                        {
                            str1.sprintf("%s Pass Bin Only Can Set One Bin.", s6TrayName[i].c_str());
                            str2.sprintf("%s Pass Bin 僅可設定一個Bin", s6TrayName[i].c_str());
                            ShowMyMessage(str1, str2);
                            sT3List->Clear();
                            delete sT3List;
                            return;
                        }
                    }
                }
            }
        }
        sT3List->Clear();
        delete sT3List;
    }

    if (CUSTOMER_CODE==CC_KYEC_LEE && (tag==eBinFT_ART || tag==eBinOffLine))
    {
        ARTBinCheck(tag);
    }

#if 0
    // GATE (write-policy): bI32CanCelErrorBin / bI33ErrorBinBox "I/F Error"
    // WriteIniData session (golden :2488-2509)
    if (IniConfig.bI32CanCelErrorBin)
    {
        WriteIniData(FileInfo().PathCombin(szDir,"BinasgnOff.Data"), "I/F Error", "CancelError", CancelErrorBin->Checked);
    }
    else if (IniConfig.bI33ErrorBinBox)
    {
        AnsiString FileName2=FileInfo().PathCombin(szDir, "BinasgnOff.Data");
        WriteIniData(FileName2, "I/F Error", "Bin",             iTo3PosUnload[MyBinPanel[tag]->iErrorT6]);
        WriteIniData(FileName2, "I/F Error", "Tray",            s6TrayName[MyBinPanel[tag]->iErrorT6]);
        WriteIniData(FileName2, "I/F Error", "ErrorBinBox",     rg_FixBinBox->ItemIndex);
        if (rg_FixBinBox->ItemIndex==1)
        {
            if (ed_FixBinBoxAlarmCount->Text.ToIntDef(0)<=2)
                ed_FixBinBoxAlarmCount->Text="2";
        }
        WriteIniData(FileName2, "I/F Error", "Alarm Count",     ed_FixBinBoxAlarmCount->Text);
    }
#endif

#if 0
    // GATE (write-policy): bAllSiteSameFailBinShowAlarm session (golden :2511-2540)
    if (CosFunction.bAllSiteSameFailBinShowAlarm)
    {
        AnsiString sBuffer1="", sBuffer="";
        for (int i = 0; i < iTestBinCount; i++)
        {
            sBuffer.sprintf("%d,", sgSpecificBin->Cells[i][1]=="V");
            sBuffer1+=sBuffer;
        }
        WriteIniData(FileInfo().PathCombin(szDir,"BinasgnOff.Data"), "AllSiteSameBin", "bSpecificFailBin", sBuffer1);

        if (CUSTOMER_CODE==CC_ASE_KaohSiung)
        {
            AnsiString FileName2=FileInfo().PathCombin(szDir, "BinasgnOff.Data");
            WriteIniData(FileName2, "I/F Error", "Bin",             iTo3PosUnload[MyBinPanel[tag]->iErrorT6]);
            WriteIniData(FileName2, "I/F Error", "Tray",            s6TrayName[MyBinPanel[tag]->iErrorT6]);
            WriteIniData(FileName2, "I/F Error", "ErrorBinBox",     rg_FixBinBox->ItemIndex);
            if (rg_FixBinBox->ItemIndex==1)
            {
                if (ed_FixBinBoxAlarmCount->Text.ToIntDef(0)<=2)
                    ed_FixBinBoxAlarmCount->Text="2";
            }
            WriteIniData(FileName2, "I/F Error", "Alarm Count",     ed_FixBinBoxAlarmCount->Text);
        }
    }
#endif

    if (bGPIBChangeAlarm)                                                      // golden :2543-2551 -- in-memory only
    {
        for (int i = 0; i < iTestBinCount; i++)
        {
            sBinConsFail[tag]->Strings[i]   =(BinSelect[tag].bConsFail[i])?"1":"0";
            sBinEnableFail[tag]->Strings[i] =(BinSelect[tag].bFailure[i])?"1":"0";
            sBinFailPercent[tag]->Strings[i]=BinSelect[tag].dFailureLimit[i];
        }
    }

    if (bTCPIPChangeAlarm)                                                     // golden :2553-2559 -- in-memory only
    {
        for (int i = 0; i < iTestBinCount; i++)
            sBinDoubleContact[tag]->Strings[i]=BinSelect[tag].iDBContact[i];
    }

    if (bSetBINCOUNT)                                                          // golden :2561-2567 -- in-memory only
    {
        for (int i = 0; i < iTestBinCount; i++)
            sBinFailIgnore[tag]->Strings[i]=BinSelect[tag].iPersentIgnore[i];
    }

    if (CUSTOMER_CODE==CC_Greatek)                                             // golden :2569-2579 -- in-memory only
    {
        if (bMOFileControlBinChangeAlarm)
        {
            for (int i = 0; i < iTestBinCount; i++)
            {
                sSpecialBinBySocket[tag]->Strings[i]     =(BinSelect[tag].bSpecialBinBySocket[i])?"1":"0";
                sSpecialBinCountBySocket[tag]->Strings[i]=IntToStr(BinSelect[tag].iSpecialBinCountBySocket[i]);
            }
        }
    }

    if (bOLPSetCategroy)                                                       // golden :2581-2589 -- in-memory only
    {
        MyBinPanel[tag]->iErrorT6=iTo6Unload[BinSelect[iTestRunMode].IfErrorT3];
        for (int i = 0; i < iTestBinCount; i++)
            sBinTraySetT3Pos[tag]->Strings[i]=BinSelect[tag].iCatDataT3Pos[i];

        for (int i = 0; i < eTrayCount; i++)
            MyBinPanel[tag]->iT6IsFail[i]=Prod.iIsFailT6[i];
    }

    SaveFunctionData(tag);   // WAVE B stub -- see forms/fBinSel.h WAVE B QUEUE

    if (bSetByDLL==true)                                                       // golden :2593-2596 -- in-memory only
    {
        MyBinPanel[tag]->iErrorT6=iTo6Unload[BinSelect[iTestRunMode].IfErrorT3];
    }

#if 0
    // GATE (write-policy): "I/F Error" Bin/Tray/Contact/Cons.Fail/Fail Percent/
    // Scan/Fail Count WriteIniData block (golden :2598-2622)
    if (CUSTOMER_CODE==CC_TFME_CHINA)
    {
        if (MyBinPanel[tag]->iErrorT6==2)
            WriteIniData(FileName, "I/F Error", "Bin", iTo3Unload[6]);
        else if (MyBinPanel[tag]->iErrorT6==11)
            WriteIniData(FileName, "I/F Error", "Bin", iTo3Unload[18]);
        else
            WriteIniData(FileName, "I/F Error", "Bin", iTo3Unload[MyBinPanel[tag]->iErrorT6+1]);
    }
    else
    {
        WriteIniData(FileName, "I/F Error", "Bin", iTo3Unload[MyBinPanel[tag]->iErrorT6]);
    }
    WriteIniData(FileName, "I/F Error", "Tray",         s6TrayName[MyBinPanel[tag]->iErrorT6]);
    WriteIniData(FileName, "I/F Error", "Contact",      0);
    WriteIniData(FileName, "I/F Error", "Cons.Fail",    0);
    WriteIniData(FileName, "I/F Error", "Fail Percent", 0);
    WriteIniData(FileName, "I/F Error", "Scan",         0);
    WriteIniData(FileName, "I/F Error", "Fail Count",   0);
#endif

#if 0
    // GATE (write-policy): per-Category WriteIniData loop (golden :2624-2690)
    // -- 100% WriteIniData sink; every local (`iResult`) it computes is only
    // ever consumed as the very next WriteIniData argument, no
    // independently-observable side effect survives gating this loop whole.
    AnsiString GroupStr;
    int iResult;
    for (int i = 0; i < iTestBinCount; i++)
    {
        GroupStr.sprintf("Category%d", i);
        for (int j = eBinNotUse; j < eBinSetTotal; j++)
        {
            if (MyBinPanel[tag]->BackT6PosTray[i][j])
            {
                WriteIniData(FileName, GroupStr, "Bin", iTo3PosUnload[j-eBinNotUse]);
                if (j==eBinNotUse)
                    WriteIniData(FileName, GroupStr, "Tray", "NotUse");
                else
                {
                    if (j==32)
                        WriteIniData(FileName, GroupStr, "Tray", s3TrayName[3]);
                    else
                        WriteIniData(FileName, GroupStr, "Tray", s3TrayName[iTo3PosUnload[j-eBinNotUse-1]]);
                }
                break;
            }
        }

        iResult=MyBinPanel[tag]->i2Contact[i];
        if (Prod.bD22SupportMultiDoubleContact==false)
            iResult=0;
        WriteIniData(FileName, GroupStr, "Contact", iResult);

        if (bGPIBChangeAlarm)
        {
            MyBinPanel[tag]->bConFail[i]=BinSelect[iTestRunMode].bFailure[i];
            MyBinPanel[tag]->bPersentEnable[i]=BinSelect[iTestRunMode].bFailure[i];
            MyBinPanel[tag]->dPersentNumber[i]=Prod.dFailureLimit[i];
        }

        iResult=(MyBinPanel[tag]->bConFail[i])?1:0;
        WriteIniData(FileName, GroupStr, "Cons.Fail", iResult);

        iResult=(MyBinPanel[tag]->bPersentEnable[i])?1:0;
        WriteIniData(FileName, GroupStr, "Fail Percent",        iResult);
        WriteIniData(FileName, GroupStr, "Fail Percent Ignore", MyBinPanel[tag]->iPersentIgnore[i]);
        WriteIniData(FileName, GroupStr, "Fail Percent Limit",  MyBinPanel[tag]->dPersentNumber[i]);

        iResult=(MyBinPanel[tag]->bCountEnable[i])?1:0;
        WriteIniData(FileName, GroupStr, "Fail Count",        iResult);
        WriteIniData(FileName, GroupStr, "Fail Count Ignore", MyBinPanel[tag]->iCountIgnore[i]);
        WriteIniData(FileName, GroupStr, "Fail Count Limit",  MyBinPanel[tag]->iCountNumber[i]);

        iResult=(MyBinPanel[tag]->bSpecialBinByArm[i])?1:0;
        WriteIniData(FileName, GroupStr, "Special Bin By Arm",        iResult);
        WriteIniData(FileName, GroupStr, "Special Bin Count By Arm", MyBinPanel[tag]->iSpecialBinCountByArm[i]);
        iResult=(MyBinPanel[tag]->bSpecialBinBySocket[i])?1:0;
        WriteIniData(FileName, GroupStr, "Special Bin By Socket",        iResult);
        WriteIniData(FileName, GroupStr, "Special Bin Count By Socket",  MyBinPanel[tag]->iSpecialBinCountBySocket[i]);

        WriteIniData(FileName, GroupStr, "By Bin Low Yield",                  MyBinPanel[tag]->bLowYield[i]);
        WriteIniData(FileName, GroupStr, "By Bin and Site Compare Arm Yield", MyBinPanel[tag]->bArmYield[i]);
        WriteIniData(FileName, GroupStr, "By Bin Compare Site Yield",         MyBinPanel[tag]->bSiteYield[i]);

        iResult=(MyBinPanel[tag]->bScan[i])?1:0;
        WriteIniData(FileName, GroupStr, "Scan", iResult);
    }
#endif

    if (bGPIBChangeAlarm || bSetByDLL)                                         // golden :2692-2697 -- in-memory only
    {
        for (int i = 0; i < eTrayCount; i++)
            MyBinPanel[tag]->iT6IsFail[i]=BinSelect[iTestRunMode].iStackDefFailCate[iTo3Unload[i]];
    }

#if 0
    // GATE (write-policy): per-tray Pass/Fail/Link/AutoRetest/Cate R
    // WriteIniData block (golden :2699-2739)
    for (int i = 0; i < eTrayCount; i++)
    {
        WriteIniData(FileName, s6TrayName[i], "Pass/Fail", MyBinPanel[tag]->iT6IsFail[i]);
        WriteIniData(FileName, s6TrayName[i], "Link",      MyBinPanel[tag]->bT6Link[i]);
    }

    if (USE_AUTO_RETEST==eartInstall || IniConfig.bA38_SLT_Summary)
    {
        for (int i = eAuto1; i <= iAutoRight; i++)
        {
            if (UNLOADER_ART[i]==eartInstall)
                WriteIniData(FileName, s6TrayName[i], "AutoRetest", MyBinPanel[tag]->bT6ART[i]);
            else
                WriteIniData(FileName, s6TrayName[i], "AutoRetest", false);
        }

        if ((USE_AUTO_RETEST==eartInstall && CosFunction.bUseSCKART && TestIF_File.iSCKART_SortMode==1 &&
             (tag==eBinFT || tag==eBinOffLine)) || IniConfig.bA38_SLT_Summary)
        {
            for (int i = 0; i < eTrayCount; i++)
                WriteIniData(FileName, s6TrayName[i], "Cate R", MyBinPanel[tag]->bT6CateR[i]);
        }
        else
        {
            for (int i = 0; i < eTrayCount; i++)
                WriteIniData(FileName, s6TrayName[i], "Cate R", false);
        }
    }
    else
    {
        for (int i = eAuto1; i <= iAutoRight; i++)
            WriteIniData(FileName, s6TrayName[i], "AutoRetest", false);
    }
#endif

#if 0
    // GATE (write-policy): "BinModel"/"bPrime" + "Other"/"bShow0XBin" session
    // (golden :2742-2750)
    AnsiString FileName2=FileInfo().PathCombin(szDir, "Binasgn.Data");
    WriteIniData(FileName2, "BinModel", "bPrime", iBinModelPrime);
    if (CosFunction.bShow0Xbin)
        WriteIniData(FileName2, "Other", "bShow0XBin", chkShow0Xbin->Checked);
#endif

    AddSpace(szDir);   // common.h documented no-op stub, not config content mutation
}

//---------------------------------------------------------------------------
//  ReadFile -- golden :1119-1596. The MUST-HAVE method. See forms/fBinSel.h
//  GATE REGISTER (G1)/(G2)/(G3) and WRITE-PATH GATE TABLE for every gate
//  below.
//---------------------------------------------------------------------------
void TfBinSel::ReadFile(bool bDelOffline, bool bChangeNeme, AnsiString /*sFileName*/)
{
    AnsiString szDir="";

    // GATE (G1): fQAMode->ReadFile(); -- `grep -rn "\bfQAMode\b"
    // --include=*.h .` -- 0 hits (20260819).
    ReadPrimeDara();
    AnsiString SavePath[]={"\\BinasgnOff.Data", "\\Binasgn.Data", "\\BinasgnOff-Line.Data", "\\BinasgnOff_ART.Data", "\\Binasgn_ART.Data", "\\Binasgn_MRT_RT.Data", "\\Binasgn_MRT.Data"};

    if (IniConfig.bA02BinModelPrime)
    {
        if (IniConfig.bFTBin2RTBin==true && (iBinModelPrime==0 || CosFunction.bDisableRTBinSet))
            SavePath[0]="\\Binasgn.Data";
        else
            SavePath[0]="\\BinasgnOff.Data";
    }
    else
    {
        if (IniConfig.bFTBin2RTBin==true || CosFunction.bDisableRTBinSet)
            SavePath[0]="\\Binasgn.Data";
        else
            SavePath[0]="\\BinasgnOff.Data";
    }

    for (int i = 0; i < eTrayCount; i++)
    {
        if (i==eFix1 || i==eAuto1)
            bCanLinkT6[i]=false;
        else if (IniConfig.bAutoTrayLink==false && i<=iAutoRight)
            bCanLinkT6[i]=false;
        else if (AUTO3_IS_MAGAZINE==1 && (i==iMagAtAuto))
            bCanLinkT6[i]=false;
        else if (AUTO_EMPTY_COLOR!=4 && i==eAuto6)
            bCanLinkT6[i]=false;
        else if (i==eBulkBox)
            bCanLinkT6[i]=false;
        else if (Prod.iTrayType[i]==tNotUse)
            bCanLinkT6[i]=false;
        else
            bCanLinkT6[i]=true;
    }

    if (bDelOffline==true)
    {
        if (bPurgeOutAllDevice)
        {
        }
        else
        {
#if 0
            // GATE (write-policy): destructive delete
            szDir=GetRecipeFileName(SavePath[2]);
            DeleteFile(szDir.c_str());
#endif
        }
    }

    if (IniConfig.bI32CanCelErrorBin && bChangeNeme==false)
    {
        szDir=GetRecipeFileName("BinasgnOff.Data");
        bCancelErrorBin=CheckAndReadIniData(szDir, "I/F Error", "CancelError", false);
    }
    else
    {
        bCancelErrorBin=false;
        if (IniConfig.bI33ErrorBinBox && bChangeNeme==false)
        {
            szDir=GetRecipeFileName("BinasgnOff.Data");
            iHWFix_BinBox=CheckAndReadIniData(szDir, "I/F Error", "ErrorBinBox", 0);
            iErrorBinBoxAlarm=CheckAndReadIniData(szDir, "I/F Error", "Alarm Count", 0);
            if (iHWFix_BinBox==1 && iErrorBinBoxAlarm<=2)
                iErrorBinBoxAlarm=2;
        }
        else
        {
            iHWFix_BinBox=0;
        }
    }

    if (CosFunction.bAllSiteSameFailBinShowAlarm)
    {
        AnsiString SBuffer1="", SBuffer="";
        szDir=GetRecipeFileName(SavePath[0]);
        SBuffer1=CheckAndReadIniData(szDir, "AllSiteSameBin", "bSpecificFailBin", AnsiString("5"));
        if (SBuffer1=="5")
        {
            SBuffer1="";
            for (int i = 0; i < iTestBinCount; i++)
            {
                bSpecificBin[i]=CheckAndReadIniData(szDir, "AllSiteSameBin", "bSpecificFailBin"+IntToStr(i), false);
                SBuffer.sprintf("%d,", bSpecificBin[i]);
                SBuffer1+=SBuffer;
            }
#if 0
            // GATE (write-policy)
            WriteIniData(szDir, "AllSiteSameBin", "bSpecificFailBin", SBuffer1);
#endif
            SBuffer1 = CheckAndReadIniData(szDir, "AllSiteSameBin", "bSpecificFailBin", AnsiString("0"));
        }

        for (int i = 0; i < iTestBinCount; i++)
            bSpecificBin[i]=(SBuffer1.SubString(i*2+1, 1)=="1");
    }

    for (int tag = 0; tag < eBinTypeTotal; tag++)
    {
        if (CosFunction.bUseMRTMode==false)
        {
            if (tag==eBinRT_MRT || tag==eBinFT_MRT)
                continue;
        }

        if (USE_AUTO_RETEST==0)
        {
            if (tag==eBinRT_ART || tag==eBinFT_ART)
                continue;
        }
        else if (USE_AUTO_RETEST==1 && CosFunction.bUseSCKART)
        {
            if (tag==eBinRT_ART || tag==eBinFT_ART)
                continue;
        }

        szDir=GetRecipeFileName(SavePath[tag]);

        if (bChangeNeme)
        {
            if (tag!=1)
                continue;
        }

        if (CUSTOMER_CODE==CC_TFME_CHINA)
        {
            BinSelect[tag].IfErrorT3Define=CheckAndReadIniData(szDir, "I/F Error", "BinDefine", 0);
            BinSelect[tag].IfErrorT3=CheckAndReadIniData(szDir, "I/F Error", "Bin", e3Fix2);
            if (BinSelect[tag].IfErrorT3Define==0)
            {
#if 0
                // GATE (write-policy)
                WriteIniData(szDir, "I/F Error", "Bin", BinSelect[tag].IfErrorT3+1);
                WriteIniData(szDir, "I/F Error", "BinDefine", 1);
#endif
            }
            else
            {
                BinSelect[tag].IfErrorT3=BinSelect[tag].IfErrorT3-1;
            }
        }
        else
        {
            BinSelect[tag].IfErrorT3=CheckAndReadIniData(szDir, "I/F Error", "Bin", e3Fix2);
        }

        if (bCancelErrorBin || iHWFix_BinBox==1)
        {
            BinSelect[tag].IfErrorT3=iTo3Unload[eBulkBox];
        }
        else if (IniConfig.bBinBox)
        {
            // GATE (G2): `AutoForm[iBinBoxAtFix]->iTrayType==3` -- `grep -rn
            // "AutoForm\[" --include=*.h .` -- 0 hits (20260819). Fail-closed
            // to the `else` arm below (golden's own fallback for this
            // condition being false).
            if (BinSelect[tag].IfErrorT3<0)
            {
                BinSelect[tag].IfErrorT3=e3Fix2;
            }

            if (CosFunction.bLoaderTrayToAuto1 && TrayForm.LoaderToEmptyColor[iRunStartMode]==2)
            {
                if (iTo6Unload[BinSelect[tag].IfErrorT3]==eAuto1)
                    BinSelect[tag].IfErrorT3=iTo3Unload[eMag14];
            }

            if (AUTO3_IS_MAGAZINE==1)
            {
                if (iTo6Unload[BinSelect[tag].IfErrorT3]==eAuto3)
                    BinSelect[tag].IfErrorT3=iTo3Unload[eMag14];

                if (TestIF_File.iMagFixTrayType==1 &&
                    iTo6Unload[BinSelect[tag].IfErrorT3]>=iFixMin &&
                    iTo6Unload[BinSelect[tag].IfErrorT3]<=iFixRightHalf)
                {
                    BinSelect[tag].IfErrorT3=iTo3Unload[eMag14];
                }
            }
            else
            {
                if (TrayForm.iFixTrayMode==false &&
                    iTo6Unload[BinSelect[tag].IfErrorT3]>=iFixMax &&
                    iTo6Unload[BinSelect[tag].IfErrorT3]<=iFixRightHalf)
                {
                    BinSelect[tag].IfErrorT3=iTo3Unload[iFixMax];
                }
            }

            if (TestIF_File.bEnableQASampling &&
                iTo6Unload[BinSelect[tag].IfErrorT3]==iTo6Unload[TestIF_File.iQASamplingT3Pos])
            {
                if (iTo6Unload[TestIF_File.iQASamplingT3Pos]==iFixMax)
                    BinSelect[tag].IfErrorT3=TestIF_File.iQASamplingT3Pos-1;
                else if (iTo6Unload[TestIF_File.iQASamplingT3Pos]==iFixRightHalf)
                    BinSelect[tag].IfErrorT3=TestIF_File.iQASamplingT3Pos-1;
                else
                    BinSelect[tag].IfErrorT3=TestIF_File.iQASamplingT3Pos+1;
            }

            if (USE_ROTATE==eCynRot)
            {
                if (iTo6Unload[BinSelect[tag].IfErrorT3]==iRotate_Out_Tray6)
                    BinSelect[tag].IfErrorT3=iTo3Unload[eFix2];
            }

            if (MachineTypeChoice==Type_HT9045 &&
                FIX3_FULL_PLACE==Fix3K_UseCylinder &&
                USE_ROTATE==eMotRot &&
                iTo6Unload[BinSelect[tag].IfErrorT3]==e3Fix3)
            {
                BinSelect[tag].IfErrorT3=iTo3Unload[eFix2];
            }
        }
        else
        {
            if (BinSelect[tag].IfErrorT3<0)
            {
                BinSelect[tag].IfErrorT3=e3Fix2;
            }

            if (CosFunction.bLoaderTrayToAuto1 && TrayForm.LoaderToEmptyColor[iRunStartMode]==2)
            {
                if (iTo6Unload[BinSelect[tag].IfErrorT3]==eAuto1)
                    BinSelect[tag].IfErrorT3=iTo3Unload[eMag14];
            }

            if (AUTO3_IS_MAGAZINE==1)
            {
                if (iTo6Unload[BinSelect[tag].IfErrorT3]==eAuto3)
                    BinSelect[tag].IfErrorT3=iTo3Unload[eMag14];

                if (TestIF_File.iMagFixTrayType==1 &&
                    iTo6Unload[BinSelect[tag].IfErrorT3]>=iFixMin &&
                    iTo6Unload[BinSelect[tag].IfErrorT3]<=iFixRightHalf)
                {
                    BinSelect[tag].IfErrorT3=iTo3Unload[eMag14];
                }
            }
            else
            {
                if (TrayForm.iFixTrayMode==false &&
                    iTo6Unload[BinSelect[tag].IfErrorT3]>=iFixMax &&
                    iTo6Unload[BinSelect[tag].IfErrorT3]<=iFixRightHalf)
                {
                    BinSelect[tag].IfErrorT3=iTo3Unload[iFixMax];
                }
            }

            if (TestIF_File.bEnableQASampling &&
                iTo6Unload[BinSelect[tag].IfErrorT3]==iTo6Unload[TestIF_File.iQASamplingT3Pos])
            {
                if (iTo6Unload[TestIF_File.iQASamplingT3Pos]==iFixMax)
                    BinSelect[tag].IfErrorT3=TestIF_File.iQASamplingT3Pos-1;
                else if (iTo6Unload[TestIF_File.iQASamplingT3Pos]==iFixRightHalf)
                    BinSelect[tag].IfErrorT3=TestIF_File.iQASamplingT3Pos-1;
                else
                    BinSelect[tag].IfErrorT3=TestIF_File.iQASamplingT3Pos+1;
            }

            if (USE_ROTATE==eCynRot)
            {
                if (iTo6Unload[BinSelect[tag].IfErrorT3]==iRotate_Out_Tray6)
                    BinSelect[tag].IfErrorT3=iTo3Unload[eFix2];
            }

            if (MachineTypeChoice==Type_HT9045 &&
                FIX3_FULL_PLACE==Fix3K_UseCylinder &&
                USE_ROTATE==eMotRot &&
                iTo6Unload[BinSelect[tag].IfErrorT3]==e3Fix3)
            {
                BinSelect[tag].IfErrorT3=iTo3Unload[eFix2];
            }
        }

        MyBinPanel[tag]->iErrorT6=iTo6Unload[BinSelect[tag].IfErrorT3];

        for (int i = 0; i < iTestBinCount; i++)
        {
            for (int j = eBinNotUse; j < eBinSetTotal; j++)
                MyBinPanel[tag]->BackT6PosTray[i][j]=0;
        }

        ReadFunctionData(tag);   // WAVE B stub -- see forms/fBinSel.h WAVE B QUEUE

        for (int i = 0; i < eTrayCount; i++)
        {
            MyBinPanel[tag]->iT6IsFail[i]=CheckAndReadIniData(szDir, s6TrayName[i], "Pass/Fail", 0);
            if (MyBinPanel[tag]->iT6IsFail[i]==0 && MyBinPanel[tag]->iErrorT6==i)
                MyBinPanel[tag]->iT6IsFail[i]=1;

            BinSelect[tag].iStackDefFailCate[iTo3Unload[i]]=MyBinPanel[tag]->iT6IsFail[i];

            if (IniConfig.bAutoTrayLink==true)
            {
                if ((i>=eAuto2 && i<=iAutoRight) || (AUTO3_IS_MAGAZINE==1 && i>=iMagMin && i<=iMagMax))
                {
                    MyBinPanel[tag]->bT6Link[i]=CheckAndReadIniData(szDir, s6TrayName[i], "Link", 0);
                    BinSelect[tag].bAutoLink[iTo3Unload[i]]=MyBinPanel[tag]->bT6Link[i];
                }
            }

            if (i>=eFix2 && i<=iFixRight)
            {
                MyBinPanel[tag]->bT6Link[i]=CheckAndReadIniData(szDir, s6TrayName[i], "Link", 0);
                BinSelect[tag].bAutoLink[iTo3Unload[i]]=MyBinPanel[tag]->bT6Link[i];
            }

            if (AUTO3_IS_MAGAZINE==1)
            {
                if (i>=eMag1 && i<=eMag14)
                {
                    iLinkTrayMax=eMag14;
                    BinSelect[tag].bMagazineLink[i-eMag1]=CheckAndReadIniData(szDir, s6TrayName[i], "Link", 0);
                    MyBinPanel[tag]->bT6Link[i]=BinSelect[tag].bMagazineLink[i-eMag1];
                }
            }

            if (CosFunction.bUseSCKART)
            {
                if (i<=iAutoRight)
                {
                    if (USE_AUTO_RETEST==eartInstall && (tag==eBinFT || tag==eBinOffLine))
                    {
                        MyBinPanel[tag]->bT6ART[i]=CheckAndReadIniData(szDir, s6TrayName[i], "AutoRetest", false);
                        if (MyBinPanel[tag]->bT6ART[i])
                            BinSelect[tag].iARTTrack=i;
                    }
                    else
                    {
                        MyBinPanel[tag]->bT6ART[i]=false;
                    }
                }
                else
                {
                    MyBinPanel[tag]->bT6ART[i]=false;
                }

                if ((USE_AUTO_RETEST==eartInstall && TestIF_File.iSCKART_SortMode==1 && (tag==eBinFT || tag==eBinOffLine)) ||
                    IniConfig.bA38_SLT_Summary)
                {
                    MyBinPanel[tag]->bT6CateR[i]=CheckAndReadIniData(szDir, s6TrayName[i], "Cate R", false);
                }
                else
                {
                    MyBinPanel[tag]->bT6CateR[i]=false;
                }
                BinSelect[tag].bAutoRetest[iTo3Unload[i]]=MyBinPanel[tag]->bT6ART[i];
                BinSelect[tag].bCateR[iTo3Unload[i]]=MyBinPanel[tag]->bT6CateR[i];
            }
            else
            {
                if (i<=iAutoRight)
                {
                    if (USE_AUTO_RETEST==eartInstall && (IniConfig.bA10_AutoReTest || bAutoReTest_ART) && tag==eBinFT_ART)
                    {
                        MyBinPanel[tag]->bT6ART[i]=CheckAndReadIniData(szDir, s6TrayName[i], "AutoRetest", false);

                        if (CUSTOMER_CODE==CC_TSMC_TAINAN)
                        {
                            if (BinSelect[tag].bAutoRetest[iTo3Unload[i]]==true && MyBinPanel[tag]->iT6IsFail[i]==0)
                                MyBinPanel[tag]->bT6ART[i]=false;
                            MyBinPanel[tag]->iT6IsFail[eAuto3]=1;
                            BinSelect[tag].bAutoRetest[e3Auto3]=true;
                            MyBinPanel[tag]->bT6ART[eAuto3]=true;
                        }
                        else if (CUSTOMER_CODE!=CC_ASE_KaohSiung && CUSTOMER_CODE!=CC_KYEC_LEE)
                        {
                            MyBinPanel[tag]->iT6IsFail[eAuto2]=1;
                            BinSelect[tag].bAutoRetest[e3Auto2]=true;
                            MyBinPanel[tag]->bT6ART[eAuto2]=true;
                        }
                    }
                    else
                    {
                        MyBinPanel[tag]->bT6ART[i]=false;
                    }
                }
                else
                {
                    MyBinPanel[tag]->bT6ART[i]=false;
                }
                BinSelect[tag].bAutoRetest[iTo3Unload[i]]=MyBinPanel[tag]->bT6ART[i];
            }
        }

        if (CUSTOMER_CODE==CC_KYEC_LEE && (tag==eBinFT_ART || tag==eBinOffLine))
        {
            ARTBinCheck(tag);
        }

        mtTrayNameSetColor(tag);   // WAVE B stub -- see forms/fBinSel.h WAVE B QUEUE
    }

    ReadWriteMRTMode(0);
    ReadWriteSpecialFunction(false);

    for (int tag = 0; tag < eBinTypeTotal; tag++)
        BinSelect[tag].iStackDefFailCate[BinSelect[tag].IfErrorT3]=1;

    CancelErrorBin->Checked = bCancelErrorBin;
    rg_FixBinBox->ItemIndex = iHWFix_BinBox;
    ed_FixBinBoxAlarmCount->Text=iErrorBinBoxAlarm;

    SetWorkParameter();

    for (int i = 0; i < eBinTypeTotal; i++)
    {
        sT3TrayType[i]->Clear();
        sT3CateR[i]->Clear();
        sT6Retest[i]->Clear();

        for (int j = e3Auto1; j < eTrayCount; j++)
        {
            sT3TrayType[i]->Add("0");
            sT3CateR[i]->Add("0");
        }

        for (int j = eAuto1; j <= iAutoRight; j++)
            sT6Retest[i]->Add("0");
    }

    for (int tag = 0; tag < eBinTypeTotal; tag++)
    {
        for (int i = 0; i < iTestBinCount; i++)
        {
            if (sBinType[tag]->Count<=i)
                sBinType[tag]->Add(AnsiString(Prod.bIsPassBin[i]?"0":"1"));
            else
                sBinType[tag]->Strings[i]=AnsiString(Prod.bIsPassBin[i]?"0":"1");
        }

        for (int i = 0; i < eTrayCount; i++)
        {
            if (iTo3Unload[i]>=0 && iTo3Unload[i]<e3TrayCount)
            {
                sT3TrayType[tag]->Strings[iTo3Unload[i]]=BinSelect[tag].iStackDefFailCate[iTo3Unload[i]];
                sT3CateR[tag]->Strings[iTo3Unload[i]]=(BinSelect[tag].bCateR[iTo3Unload[i]])?"1":"0";
            }
        }

        for (int i = eAuto1; i <= iAutoRight; i++)
        {
            if (iTo3Unload[i]>=0 && iTo3Unload[i]<e3TrayCount)
                sT6Retest[tag]->Strings[i]=(BinSelect[tag].bAutoRetest[iTo3Unload[i]])?"1":"0";
        }
    }

    if (CUSTOMER_CODE==CC_KYEC_LEE)
    {
        if (IniConfig.bA73UserOverrideFTRTBin)
        {
            IniConfig.bFTBin2RTBin       = false;
            CosFunction.bDisableRTBinSet = false;
        }
        else
        {
            IniConfig.bFTBin2RTBin       = true;
            CosFunction.bDisableRTBinSet = true;
        }
    }
    SetPrimeButton();   // WAVE B stub -- see forms/fBinSel.h WAVE B QUEUE
    LotSummary.SetIsRTBin();
    // GATE (write-policy): ATKRecipeInfo->SaveFile(); -- writes a
    // work-file-compare file (golden comment "為了加快ini讀取速度"/"要新增
    // 工作檔比對用的檔案"); treated as a write per WRITE-PATH GATE TABLE
    // regardless of ATKRecipeInfo's own landed status.
}

//---------------------------------------------------------------------------
//  WAVE B -- declared, documented no-op body this wave. See forms/fBinSel.h
//  WAVE B QUEUE for the full reasoning (transcription volume, not risk).
//---------------------------------------------------------------------------
void TfBinSel::ReadFunctionData(int /*tag*/)
{
    // WAVE B: real body is golden :4807-5667 (861 lines) -- 100% mechanical
    // `sXxx[tag]->CommaText=CheckAndReadIniData(GroupName,"<ecid> <key>","")`
    // population across all 7 tags. Every one of those 27*7 arrays stays at
    // this wave's ctor-time "0"-placeholder content until this lands.
}

void TfBinSel::SaveFunctionData(int /*tag*/)
{
    // WAVE B: real body is golden :5668-6009 (342 lines) -- 100% WriteIniData
    // sink, gated per WRITE-PATH GATE TABLE regardless; a verbatim `#if 0`
    // skeleton would add zero behaviour over this comment, so the skeleton
    // itself is queued here rather than transcribed dead.
}

void TfBinSel::SetPrimeButton()
{
    // WAVE B: real body is golden :6010-6042 (33 lines) -- pure widget-
    // visibility toggling (spbNormal/spbPrime/tsRetest/cbTestMode->Enabled)
    // plus `fMain->SetNormalOrPrime()` (unverified this wave). No ACTIVE
    // method in this wave reads anything this would set.
}

void TfBinSel::mtTrayNameSetColor(int /*tag*/)
{
    // WAVE B: real body is golden :4033-4140 (108 lines) -- grid-cell/label
    // colour rendering on `MyBinPanel[tag]->mtTrayName` (a `TTMyTray256*`,
    // no vclcompat stand-in exists anywhere in the tree) and calls
    // `InitDataToEdit(tag)` (golden :4142-4784, 643 more lines, same
    // Edit-widget-population shape). Both are pure display/UI surface --
    // no ACTIVE method in this wave reads their output.
}
