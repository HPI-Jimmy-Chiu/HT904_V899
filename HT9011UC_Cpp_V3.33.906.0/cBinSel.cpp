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
    // AI(W906-FW-BinSel-WB) 20260819: `FileName` is genuinely consumed below
    // by `SaveFunctionData(tag, FileName)` now that WAVE B lands its real
    // body -- the WAVE A `(void)FileName;` placeholder cast is removed (no
    // longer accurate: it is not "only" feeding gated sessions any more).

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

    SaveFunctionData(tag, FileName);   // FW-BinSel-WB: real body (writes gated inside)

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

        ReadFunctionData(tag, szDir);   // FW-BinSel-WB: real body (see its own banner)

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

        mtTrayNameSetColor(tag);   // FW-BinSel-WB: real body landed, entire body gated (see its own banner)
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
    SetPrimeButton();   // FW-BinSel-WB: real body landed, entire body gated (see its own banner)
    LotSummary.SetIsRTBin();
    // GATE (write-policy): ATKRecipeInfo->SaveFile(); -- writes a
    // work-file-compare file (golden comment "為了加快ini讀取速度"/"要新增
    // 工作檔比對用的檔案"); treated as a write per WRITE-PATH GATE TABLE
    // regardless of ATKRecipeInfo's own landed status.
}

//---------------------------------------------------------------------------
//  ReadFunctionData -- golden :4807-5667. FW-BinSel-WB (this wave). ACTIVE:
//  every CheckAndReadIniData/CheckSectionExist call (reads are POLICY-ALLOWED,
//  see forms/fBinSel.h WRITE-PATH GATE TABLE) plus the full old/new-format
//  BinSelect[tag]/MyBinPanel[tag] in-memory post-processing golden's own body
//  does after the per-tag read cascade (golden :5356-5636 -- NOT mechanical
//  reads, real validation logic, kept ACTIVE). GATE: the `MyBinPanel[tag]->
//  ed*->Text=...` widget-mirror block (golden :5637-5661, see inline GATE
//  below) and, transitively, `SaveFunctionData`'s own writes (called from
//  within this method too, golden :5508/:5664 -- see that function's own
//  banner). DEVIATION: signature gains an `AnsiString szDir` parameter, the
//  same substitution WAVE A already used throughout `ReadFile`/`SaveOther` --
//  golden relies on `FormSysTools` (a `TFormSysTools*` ini-file cache object,
//  systools.h/.cpp, `OpenFormData(szDir)`'d by the CALLER just before this
//  method runs, golden :1277) staying implicitly bound to the caller's file;
//  `TFormSysTools` has no port anywhere in this tree (`grep -rln
//  "TFormSysTools\|OpenFormData" --include=*.h --include=*.cpp .` -- only
//  dfm2rc layout-generator artifacts under tools/, no real class, 20260819),
//  so `szDir` (the SAME value `ReadFile` already computes right before
//  calling this, unchanged) is threaded through explicitly instead, matching
//  every other `FormSysTools->CheckAndReadIniData(...)` substitution already
//  established in this file.
//---------------------------------------------------------------------------
void TfBinSel::ReadFunctionData(int tag, AnsiString szDir)
{
    bool bHasNewSetupData=false;
    AnsiString GroupName, GroupStr;
    int iData, iT3, iT6;
    int iNewBinFormat=0;                                                        //kevin 20160928 判斷 Bin format 是否有轉過記錄

    if(CUSTOMER_CODE!=CC_AMKOR_China)                                           //JerryYang 20180207 (Steven) Amkor上海要求使用舊的讀檔方式
    {
        if(tag==eBinRT)
        {
            //Steven 20160623 : 當IniConfig.bFTBin2RTBin==true && 讀檔時, RT Bin要跟FT Bin一樣
            if((IniConfig.bA02BinModelPrime && IniConfig.bFTBin2RTBin==true && (iBinModelPrime==0 || CosFunction.bDisableRTBinSet)) ||
               (IniConfig.bA02BinModelPrime==false && (IniConfig.bFTBin2RTBin==true || CosFunction.bDisableRTBinSet)))
            {
                GroupName="Bin Func FT";
                if(CheckSectionExist(szDir, GroupName))
                {
                    iNewBinFormat=CheckAndReadIniData(szDir, GroupName, "NewBinFormat",77);//kevin 2016028 使用新的bin format
                    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                         //kevin 2016028 add 使用新的bin format
                    {
                        if(iNewBinFormat!=0 &&iNewBinFormat!=77)                //kevin 20161108
                           bHasNewSetupData=true;
                    }
                    else
                    {
                        bHasNewSetupData=true;
                    }

                    if(bHasNewSetupData)                                        //kevin 20161108
                    {
                        sBinTraySetT3Pos        [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "3617 BinTraySetting", AnsiString(""));
                        sBinType                [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "3656 BinType", AnsiString(""));
                        sBinDoubleContact       [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "3636 BinDoubleContac", AnsiString(""));
                        sBinConsFail            [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "3676 BinConsFail", AnsiString(""));
                        sBinEnableFail          [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10641 BinEnableFail", AnsiString(""));
                        sBinFailPercent         [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10642 BinFailPercent", AnsiString(""));
                        sBinFailIgnore          [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10643 BinFailIgnore", AnsiString(""));
                        sBinCountEnable         [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10644 BinCountEnable", AnsiString(""));
                        sBinCountIgnore         [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10646 BinCountIgnore", AnsiString(""));
                        sBinCountNumber         [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10645 BinCountNumber", AnsiString(""));
                        sSpecialBinByArm        [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10647 SpecialBinByArm", AnsiString(""));
                        sSpecialBinCountByArm   [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10648 SpecialBinCountByArm", AnsiString(""));
                        sSpecialBinBySocket     [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10649 SpecialBinBySocket", AnsiString(""));
                        sSpecialBinCountBySocket[tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10650 SpecialBinCountBySocket", AnsiString(""));
                        sLowYield               [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10680 LowYield", AnsiString(""));
                        sArmYield               [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10681 ArmYield", AnsiString(""));
                        sSiteYield              [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10682 SiteYield", AnsiString(""));
                        sBySiteClean            [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10683 BySiteClean", AnsiString(""));
                        sByBinClean             [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10684 ByBinClean", AnsiString(""));
                        //JerryYang 20170712 (Steven) by site by bin compare percent
                        //==>
                        sSpecBinBySiteCompareEnable       [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10685 SpecBinBySiteCompareEnable", AnsiString(""));
                        sSpecBinBySiteCompareIgnore       [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10686 SpecBinBySiteCompareIgnore", AnsiString(""));
                        sSpecBinBySiteComparePercent      [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10687 SpecBinBySiteComparePercent",  AnsiString(""));
                        sSpecBinByArmPerSiteCompareEnable [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10688 SpecBinByArmPerSiteCompareEnable", AnsiString(""));
                        sSpecBinByArmPerSiteCompareIgnore [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10689 SpecBinByArmPerSiteCompareIgnore", AnsiString(""));
                        sSpecBinByArmPerSiteComparePercent[tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10690 SpecBinByArmPerSiteComparePercent",  AnsiString(""));
                        //<==
                        //JerryYang 20170712 (Steven) by site by bin compare percent

                        sBinTrayLinked  [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "BinTrayLinked(10698)",  AnsiString("0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"));    //Ifor 20240730 add : BinTrayLinked
                        sBinLinked      [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "BinLinked(10699)",  AnsiString("0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;"));    //Ifor 20240730 add : BinLinked
                        sMagazineSetup  [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "MagazineSetup(10700)", AnsiString("0"));

                        sAOIBinTraySetting[tag]->CommaText=CheckAndReadIniData(szDir, GroupName,"AOIBinTraySetting", AnsiString(""));//Eastsun 20260316 : AOI 要分bin
                    }
                }
            }
            else
            {
                GroupName="Bin Func RT";
                if(CheckSectionExist(szDir, GroupName))
                {
                    iNewBinFormat=CheckAndReadIniData(szDir, GroupName, "NewBinFormat", 77);//kevin 2016028 使用新的bin format
                    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                         //kevin 2016028 add 使用新的bin format
                    {
                        if(iNewBinFormat!=77)                                   //kevin 20160928
                            bHasNewSetupData=true;
                    }
                    else
                    {
                        bHasNewSetupData=true;
                    }

                    if(bHasNewSetupData)                                        //kevin 20161108
                    {
                        sBinTraySetT3Pos        [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "3717 BinTraySetting", AnsiString(""));
                        sBinDoubleContact       [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "3718 BinDoubleContac", AnsiString(""));
                        sBinType                [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "3719 BinType", AnsiString(""));
                        sBinConsFail            [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "3720 BinConsFail", AnsiString(""));
                        sBinEnableFail          [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10741 BinEnableFail", AnsiString(""));
                        sBinFailPercent         [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10742 BinFailPercent", AnsiString(""));
                        sBinFailIgnore          [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10743 BinFailIgnore", AnsiString(""));
                        sBinCountEnable         [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10744 BinCountEnable", AnsiString(""));
                        sBinCountNumber         [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10745 BinCountNumber", AnsiString(""));
                        sBinCountIgnore         [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10746 BinCountIgnore", AnsiString(""));
                        sSpecialBinByArm        [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10747 SpecialBinByArm", AnsiString(""));
                        sSpecialBinCountByArm   [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10748 SpecialBinCountByArm", AnsiString(""));
                        sSpecialBinBySocket     [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10749 SpecialBinBySocket", AnsiString(""));
                        sSpecialBinCountBySocket[tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10750 SpecialBinCountBySocket", AnsiString(""));
                        sLowYield               [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10780 LowYield", AnsiString(""));
                        sArmYield               [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10781 ArmYield", AnsiString(""));
                        sSiteYield              [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10782 SiteYield", AnsiString(""));
                        sBySiteClean            [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10783 BySiteClean", AnsiString(""));
                        sByBinClean             [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10784 ByBinClean", AnsiString(""));
                        //JerryYang 20170712 (Steven) by site by bin compare percent
                        //==>
                        sSpecBinBySiteCompareEnable       [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10785 SpecBinBySiteCompareEnable", AnsiString(""));
                        sSpecBinBySiteCompareIgnore       [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10786 SpecBinBySiteCompareIgnore", AnsiString(""));
                        sSpecBinBySiteComparePercent      [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10787 SpecBinBySiteComparePercent",  AnsiString(""));
                        sSpecBinByArmPerSiteCompareEnable [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10788 SpecBinByArmPerSiteCompareEnable", AnsiString(""));
                        sSpecBinByArmPerSiteCompareIgnore [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10789 SpecBinByArmPerSiteCompareIgnore", AnsiString(""));
                        sSpecBinByArmPerSiteComparePercent[tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10790 SpecBinByArmPerSiteComparePercent",  AnsiString(""));
                        //<==
                        //JerryYang 20170712 (Steven) by site by bin compare percent

                        sBinTrayLinked  [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "BinTrayLinked(10798)",  AnsiString("0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"));    //Ifor 20240730 add : BinTrayLinked
                        sBinLinked      [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "BinLinked(10799)",  AnsiString("0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;"));    //Ifor 20240730 add : BinLinked
                        sMagazineSetup  [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "MagazineSetup(10800)", AnsiString("0"));

                        sAOIBinTraySetting[tag]->CommaText=CheckAndReadIniData(szDir, GroupName,"AOIBinTraySetting", AnsiString(""));//Eastsun 20260316 : AOI 要分bin
                    }
                }
            }
        }
        else if(tag==eBinFT)
        {
            GroupName="Bin Func FT";
            if(CheckSectionExist(szDir, GroupName))
            {
                iNewBinFormat=CheckAndReadIniData(szDir, GroupName, "NewBinFormat",77);//kevin 2016028 使用新的bin format
                if(CUSTOMER_CODE==CC_ASE_KaohSiung)                             //kevin 2016028 add 使用新的bin format
                {
                    if(iNewBinFormat!=77)                                       //kevin 20160928
                        bHasNewSetupData=true;
                }
                else
                {
                    bHasNewSetupData=true;
                }

                if(bHasNewSetupData)                                            //kevin 20161108
                {
                    sBinTraySetT3Pos        [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "3617 BinTraySetting", AnsiString(""));
                    sBinType                [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "3656 BinType", AnsiString(""));
                    sBinDoubleContact       [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "3636 BinDoubleContac", AnsiString(""));
                    sBinConsFail            [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "3676 BinConsFail", AnsiString(""));
                    sBinEnableFail          [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10641 BinEnableFail", AnsiString(""));
                    sBinFailPercent         [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10642 BinFailPercent", AnsiString(""));
                    sBinFailIgnore          [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10643 BinFailIgnore", AnsiString(""));
                    sBinCountEnable         [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10644 BinCountEnable", AnsiString(""));
                    sBinCountIgnore         [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10646 BinCountIgnore", AnsiString(""));
                    sBinCountNumber         [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10645 BinCountNumber", AnsiString(""));
                    sSpecialBinByArm        [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10647 SpecialBinByArm", AnsiString(""));
                    sSpecialBinCountByArm   [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10648 SpecialBinCountByArm", AnsiString(""));
                    sSpecialBinBySocket     [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10649 SpecialBinBySocket", AnsiString(""));
                    sSpecialBinCountBySocket[tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10650 SpecialBinCountBySocket", AnsiString(""));
                    sLowYield               [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10680 LowYield", AnsiString(""));
                    sArmYield               [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10681 ArmYield", AnsiString(""));
                    sSiteYield              [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10682 SiteYield", AnsiString(""));
                    sBySiteClean            [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10683 BySiteClean", AnsiString(""));
                    sByBinClean             [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10684 ByBinClean", AnsiString(""));
                    //JerryYang 20170712 (Steven) by site by bin compare percent
                    //==>
                    sSpecBinBySiteCompareEnable       [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10685 SpecBinBySiteCompareEnable", AnsiString(""));
                    sSpecBinBySiteCompareIgnore       [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10686 SpecBinBySiteCompareIgnore", AnsiString(""));
                    sSpecBinBySiteComparePercent      [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10687 SpecBinBySiteComparePercent",  AnsiString(""));
                    sSpecBinByArmPerSiteCompareEnable [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10688 SpecBinByArmPerSiteCompareEnable", AnsiString(""));
                    sSpecBinByArmPerSiteCompareIgnore [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10689 SpecBinByArmPerSiteCompareIgnore", AnsiString(""));
                    sSpecBinByArmPerSiteComparePercent[tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10690 SpecBinByArmPerSiteComparePercent", AnsiString(""));
                    //<==
                    //JerryYang 20170712 (Steven) by site by bin compare percent

                    sBinTrayLinked  [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "BinTrayLinked(10698)",  AnsiString("0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"));    //Ifor 20240730 add : BinTrayLinked
                    sBinLinked      [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "BinLinked(10699)",  AnsiString("0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;"));    //Ifor 20240730 add : BinLinked
                    sMagazineSetup  [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "MagazineSetup(10700)", AnsiString("0"));

                    sAOIBinTraySetting[tag]->CommaText=CheckAndReadIniData(szDir, GroupName,"AOIBinTraySetting", AnsiString(""));//Eastsun 20260316 : AOI 要分bin
                }
            }
        }
        else if(tag==eBinOffLine)
        {
            GroupName="Bin Func OffLine";
            if(CheckSectionExist(szDir, GroupName))
            {
                iNewBinFormat=CheckAndReadIniData(szDir, GroupName, "NewBinFormat",77);//kevin 2016028 使用新的bin format
                if(CUSTOMER_CODE==CC_ASE_KaohSiung)                             //kevin 2016028 add 使用新的bin format
                {
                    if(iNewBinFormat!=77)                                       //kevin 20160928
                        bHasNewSetupData=true;
                }
                else
                {
                    bHasNewSetupData=true;
                }

                if(bHasNewSetupData)                                            //kevin 20161108
                {
                    sBinTraySetT3Pos        [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "3722 BinTraySetting", AnsiString(""));
                    sBinDoubleContact       [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "3723 BinDoubleContac", AnsiString(""));
                    sBinType                [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "3724 BinType", AnsiString(""));
                    sBinConsFail            [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "3725 BinConsFail", AnsiString(""));
                    sBinEnableFail          [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10941 BinEnableFail", AnsiString(""));
                    sBinFailPercent         [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10942 BinFailPercent", AnsiString(""));
                    sBinFailIgnore          [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10943 BinFailIgnore", AnsiString(""));
                    sBinCountEnable         [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10944 BinCountEnable", AnsiString(""));
                    sBinCountNumber         [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10945 BinCountNumber", AnsiString(""));
                    sBinCountIgnore         [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10946 BinCountIgnore", AnsiString(""));
                    sSpecialBinByArm        [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10947 SpecialBinByArm", AnsiString(""));
                    sSpecialBinCountByArm   [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10948 SpecialBinCountByArm", AnsiString(""));
                    sSpecialBinBySocket     [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10949 SpecialBinBySocket", AnsiString(""));
                    sSpecialBinCountBySocket[tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10950 SpecialBinCountBySocket", AnsiString(""));
                    sLowYield               [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10980 LowYield", AnsiString(""));
                    sArmYield               [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10981 ArmYield", AnsiString(""));
                    sSiteYield              [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10982 SiteYield", AnsiString(""));
                    sBySiteClean            [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10983 BySiteClean", AnsiString(""));
                    sByBinClean             [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10984 ByBinClean", AnsiString(""));
                    //JerryYang 20170712 (Steven) by site by bin compare percent
                    //==>
                    sSpecBinBySiteCompareEnable       [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10985 SpecBinBySiteCompareEnable", AnsiString(""));
                    sSpecBinBySiteCompareIgnore       [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10986 SpecBinBySiteCompareIgnore", AnsiString(""));
                    sSpecBinBySiteComparePercent      [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10987 SpecBinBySiteComparePercent",  AnsiString(""));
                    sSpecBinByArmPerSiteCompareEnable [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10988 SpecBinByArmPerSiteCompareEnable", AnsiString(""));
                    sSpecBinByArmPerSiteCompareIgnore [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10989 SpecBinByArmPerSiteCompareIgnore", AnsiString(""));
                    sSpecBinByArmPerSiteComparePercent[tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "10990 SpecBinByArmPerSiteComparePercent",  AnsiString(""));
                    //<==
                    //JerryYang 20170712 (Steven) by site by bin compare percent

                    sBinTrayLinked  [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "BinTrayLinked(10998)",  AnsiString("0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"));    //Ifor 20240730 add : BinTrayLinked
                    sBinLinked      [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "BinLinked(10999)",  AnsiString("0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;"));    //Ifor 20240730 add : BinLinked
                    sMagazineSetup  [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "MagazineSetup(11000)", AnsiString("0"));

                    sAOIBinTraySetting[tag]->CommaText=CheckAndReadIniData(szDir, GroupName,"AOIBinTraySetting", AnsiString(""));//Eastsun 20260316 : AOI 要分bin
                }
            }
        }
        else if(tag==eBinFT_ART)                                                //Steven 20161123 : eBinRT_ART --> eBinFT_ART
        {
            GroupName="Bin Func ART_FT";                                        //wei 20161124 修改eBinFT_ART存成Bin Func ART_RT問題
            if(CheckSectionExist(szDir, GroupName))
            {
                iNewBinFormat=CheckAndReadIniData(szDir, GroupName, "NewBinFormat",77);//kevin 2016028 使用新的bin format
                if(CUSTOMER_CODE==CC_ASE_KaohSiung)                             //kevin 2016028 add 使用新的bin format
                {
                    if(iNewBinFormat!=77)                                       //kevin 20160928
                        bHasNewSetupData=true;
                }
                else
                {
                    bHasNewSetupData=true;
                }

                if(bHasNewSetupData)                                            //kevin 20161108
                {
                    sBinTraySetT3Pos        [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "3801 BinTraySetting", AnsiString(""));
                    sBinDoubleContact       [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "3802 BinDoubleContac", AnsiString(""));
                    sBinType                [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "3803 BinType", AnsiString(""));
                    sBinConsFail            [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "3804 BinConsFail", AnsiString(""));
                    sBinEnableFail          [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11041 BinEnableFail", AnsiString(""));
                    sBinFailPercent         [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11042 BinFailPercent", AnsiString(""));
                    sBinFailIgnore          [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11043 BinFailIgnore", AnsiString(""));
                    sBinCountEnable         [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11044 BinCountEnable", AnsiString(""));
                    sBinCountNumber         [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11045 BinCountNumber", AnsiString(""));
                    sBinCountIgnore         [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11046 BinCountIgnore", AnsiString(""));
                    sSpecialBinByArm        [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11047 SpecialBinByArm", AnsiString(""));
                    sSpecialBinCountByArm   [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11048 SpecialBinCountByArm", AnsiString(""));
                    sSpecialBinBySocket     [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11049 SpecialBinBySocket", AnsiString(""));
                    sSpecialBinCountBySocket[tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11050 SpecialBinCountBySocket", AnsiString(""));
                    sLowYield               [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11080 LowYield", AnsiString(""));
                    sArmYield               [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11081 ArmYield", AnsiString(""));
                    sSiteYield              [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11082 SiteYield", AnsiString(""));
                    sBySiteClean            [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11083 BySiteClean", AnsiString(""));
                    sByBinClean             [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11084 ByBinClean", AnsiString(""));
                    //JerryYang 20170712 (Steven) by site by bin compare percent
                    //==>
                    sSpecBinBySiteCompareEnable       [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11085 SpecBinBySiteCompareEnable", AnsiString(""));
                    sSpecBinBySiteCompareIgnore       [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11086 SpecBinBySiteCompareIgnore", AnsiString(""));
                    sSpecBinBySiteComparePercent      [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11087 SpecBinBySiteComparePercent",  AnsiString(""));
                    sSpecBinByArmPerSiteCompareEnable [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11088 SpecBinByArmPerSiteCompareEnable", AnsiString(""));
                    sSpecBinByArmPerSiteCompareIgnore [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11089 SpecBinByArmPerSiteCompareIgnore", AnsiString(""));
                    sSpecBinByArmPerSiteComparePercent[tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11090 SpecBinByArmPerSiteComparePercent",  AnsiString(""));
                    //<==
                    //JerryYang 20170712 (Steven) by site by bin compare percent

                    sBinTrayLinked  [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "BinTrayLinked(11098)",  AnsiString("0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"));    //Ifor 20240730 add : BinTrayLinked
                    sBinLinked      [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "BinLinked(11099)",  AnsiString("0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;"));    //Ifor 20240730 add : BinLinked
                    sMagazineSetup  [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "MagazineSetup(11100)", AnsiString("0"));

                    sAOIBinTraySetting[tag]->CommaText=CheckAndReadIniData(szDir, GroupName,"AOIBinTraySetting", AnsiString(""));//Eastsun 20260316 : AOI 要分bin
                }
            }
            else
            {
                GroupName="Bin Func ART_RT";
                if(CheckSectionExist(szDir, GroupName))
                {
                    iNewBinFormat=CheckAndReadIniData(szDir, GroupName, "NewBinFormat",77);//kevin 2016028 使用新的bin format
                    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                         //kevin 2016028 add 使用新的bin format
                    {
                        if(iNewBinFormat!=77)                                   //kevin 20160928
                            bHasNewSetupData=true;
                    }
                    else
                    {
                        bHasNewSetupData=true;
                    }

                    if(bHasNewSetupData)                                        //kevin 20161108
                    {
                        sBinTraySetT3Pos        [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "3901 BinTraySetting", AnsiString(""));
                        sBinDoubleContact       [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "3902 BinDoubleContac", AnsiString(""));
                        sBinType                [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "3903 BinType", AnsiString(""));
                        sBinConsFail            [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "3904 BinConsFail", AnsiString(""));
                        sBinEnableFail          [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11141 BinEnableFail", AnsiString(""));
                        sBinFailPercent         [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11142 BinFailPercent", AnsiString(""));
                        sBinFailIgnore          [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11143 BinFailIgnore", AnsiString(""));
                        sBinCountEnable         [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11144 BinCountEnable", AnsiString(""));
                        sBinCountNumber         [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11145 BinCountNumber", AnsiString(""));
                        sBinCountIgnore         [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11146 BinCountIgnore", AnsiString(""));
                        sSpecialBinByArm        [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11147 SpecialBinByArm", AnsiString(""));
                        sSpecialBinCountByArm   [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11148 SpecialBinCountByArm", AnsiString(""));
                        sSpecialBinBySocket     [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11149 SpecialBinBySocket", AnsiString(""));
                        sSpecialBinCountBySocket[tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11150 SpecialBinCountBySocket", AnsiString(""));
                        sLowYield               [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11180 LowYield", AnsiString(""));
                        sArmYield               [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11181 ArmYield", AnsiString(""));
                        sSiteYield              [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11182 SiteYield", AnsiString(""));
                        sBySiteClean            [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11183 BySiteClean", AnsiString(""));     //Ifor 20170316 (wei) BinRT_ART BySiteClean 11083 --> 11183
                        sByBinClean             [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11184 ByBinClean", AnsiString(""));      //Ifor 20170316 (wei) BinRT_ART ByBinClean  11084 --> 11184
                        //JerryYang 20170712 (Steven) by site by bin compare percent
                        //==>
                        sSpecBinBySiteCompareEnable       [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11185 SpecBinBySiteCompareEnable", AnsiString(""));
                        sSpecBinBySiteCompareIgnore       [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11186 SpecBinBySiteCompareIgnore", AnsiString(""));
                        sSpecBinBySiteComparePercent      [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11187 SpecBinBySiteComparePercent",  AnsiString(""));
                        sSpecBinByArmPerSiteCompareEnable [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11188 SpecBinByArmPerSiteCompareEnable", AnsiString(""));
                        sSpecBinByArmPerSiteCompareIgnore [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11189 SpecBinByArmPerSiteCompareIgnore", AnsiString(""));
                        sSpecBinByArmPerSiteComparePercent[tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11190 SpecBinByArmPerSiteComparePercent",  AnsiString(""));
                        //<==
                        //JerryYang 20170712 (Steven) by site by bin compare percent

                        sBinTrayLinked  [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "BinTrayLinked(11198)",  AnsiString("0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"));    //Ifor 20240730 add : BinTrayLinked
                        sBinLinked      [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "BinLinked(11199)",  AnsiString("0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;"));    //Ifor 20240730 add : BinLinked
                        sMagazineSetup  [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "MagazineSetup(11200)", AnsiString("0"));

                        sAOIBinTraySetting[tag]->CommaText=CheckAndReadIniData(szDir, GroupName,"AOIBinTraySetting", AnsiString(""));//Eastsun 20260316 : AOI 要分bin
                    }
                }
            }
        }
        else if(tag==eBinRT_ART)                                                //Steven 20161123 : eBinFT_ART --> eBinRT_ART
        {
            GroupName="Bin Func ART_RT";                                        //wei 20161124 修改eBinRT_ART存成Bin Func ART_FT問題
            if(CheckSectionExist(szDir, GroupName))
            {
                iNewBinFormat=CheckAndReadIniData(szDir, GroupName, "NewBinFormat", 77);//kevin 2016028 使用新的bin format
                if(CUSTOMER_CODE==CC_ASE_KaohSiung)                             //kevin 2016028 add 使用新的bin format
                {
                    if(iNewBinFormat!=77)                                       //kevin 20160928
                        bHasNewSetupData=true;
                }
                else
                {
                    bHasNewSetupData=true;
                }

                if(bHasNewSetupData)                                            //kevin 20161108
                {
                    sBinTraySetT3Pos        [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "3901 BinTraySetting", AnsiString(""));
                    sBinDoubleContact       [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "3902 BinDoubleContac", AnsiString(""));
                    sBinType                [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "3903 BinType", AnsiString(""));
                    sBinConsFail            [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "3904 BinConsFail", AnsiString(""));
                    sBinEnableFail          [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11141 BinEnableFail", AnsiString(""));
                    sBinFailPercent         [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11142 BinFailPercent", AnsiString(""));
                    sBinFailIgnore          [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11143 BinFailIgnore", AnsiString(""));
                    sBinCountEnable         [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11144 BinCountEnable", AnsiString(""));
                    sBinCountNumber         [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11145 BinCountNumber", AnsiString(""));
                    sBinCountIgnore         [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11146 BinCountIgnore", AnsiString(""));
                    sSpecialBinByArm        [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11147 SpecialBinByArm", AnsiString(""));
                    sSpecialBinCountByArm   [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11148 SpecialBinCountByArm", AnsiString(""));
                    sSpecialBinBySocket     [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11149 SpecialBinBySocket", AnsiString(""));
                    sSpecialBinCountBySocket[tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11150 SpecialBinCountBySocket", AnsiString(""));
                    sLowYield               [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11180 LowYield", AnsiString(""));
                    sArmYield               [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11181 ArmYield", AnsiString(""));
                    sSiteYield              [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11182 SiteYield", AnsiString(""));
                    sBySiteClean            [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11183 BySiteClean", AnsiString(""));    //wei 20161124 11083 --> 11183
                    sByBinClean             [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11184 ByBinClean", AnsiString(""));     //wei 20161124 11084 --> 11184
                    //JerryYang 20170712 (Steven) by site by bin compare percent
                    //==>
                    sSpecBinBySiteCompareEnable       [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11185 SpecBinBySiteCompareEnable", AnsiString(""));
                    sSpecBinBySiteCompareIgnore       [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11186 SpecBinBySiteCompareIgnore", AnsiString(""));
                    sSpecBinBySiteComparePercent      [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11187 SpecBinBySiteComparePercent",  AnsiString(""));
                    sSpecBinByArmPerSiteCompareEnable [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11188 SpecBinByArmPerSiteCompareEnable", AnsiString(""));
                    sSpecBinByArmPerSiteCompareIgnore [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11189 SpecBinByArmPerSiteCompareIgnore", AnsiString(""));
                    sSpecBinByArmPerSiteComparePercent[tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11190 SpecBinByArmPerSiteComparePercent",  AnsiString(""));
                    //<==
                    //JerryYang 20170712 (Steven) by site by bin compare percent

                    sBinTrayLinked  [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "BinTrayLinked(11198)",  AnsiString("0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"));    //Ifor 20240730 add : BinTrayLinked
                    sBinLinked      [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "BinLinked(11199)",  AnsiString("0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;"));    //Ifor 20240730 add : BinLinked
                    sMagazineSetup  [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "MagazineSetup(11200)", AnsiString("0"));

                    sAOIBinTraySetting[tag]->CommaText=CheckAndReadIniData(szDir, GroupName,"AOIBinTraySetting", AnsiString(""));//Eastsun 20260316 : AOI 要分bin
                }
            }
            else
            {
                GroupName="Bin Func ART_FT";
                if(CheckSectionExist(szDir, GroupName))
                {
                    iNewBinFormat=CheckAndReadIniData(szDir, GroupName, "NewBinFormat",77);//kevin 2016028 使用新的bin format
                    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                         //kevin 2016028 add 使用新的bin format
                    {
                        if(iNewBinFormat!=77)                                   //kevin 20160928
                            bHasNewSetupData=true;
                    }
                    else
                    {
                        bHasNewSetupData=true;
                    }

                    if(bHasNewSetupData)                                        //kevin 20161108
                    {
                        sBinTraySetT3Pos        [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "3801 BinTraySetting", AnsiString(""));
                        sBinDoubleContact       [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "3802 BinDoubleContac", AnsiString(""));
                        sBinType                [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "3803 BinType", AnsiString(""));
                        sBinConsFail            [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "3804 BinConsFail", AnsiString(""));
                        sBinEnableFail          [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11041 BinEnableFail", AnsiString(""));
                        sBinFailPercent         [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11042 BinFailPercent", AnsiString(""));
                        sBinFailIgnore          [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11043 BinFailIgnore", AnsiString(""));
                        sBinCountEnable         [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11044 BinCountEnable", AnsiString(""));
                        sBinCountNumber         [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11045 BinCountNumber", AnsiString(""));
                        sBinCountIgnore         [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11046 BinCountIgnore", AnsiString(""));
                        sSpecialBinByArm        [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11047 SpecialBinByArm", AnsiString(""));
                        sSpecialBinCountByArm   [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11048 SpecialBinCountByArm", AnsiString(""));
                        sSpecialBinBySocket     [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11049 SpecialBinBySocket", AnsiString(""));
                        sSpecialBinCountBySocket[tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11050 SpecialBinCountBySocket", AnsiString(""));
                        sLowYield               [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11080 LowYield", AnsiString(""));
                        sArmYield               [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11081 ArmYield", AnsiString(""));
                        sSiteYield              [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11082 SiteYield", AnsiString(""));
                        sBySiteClean            [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11083 BySiteClean", AnsiString(""));
                        sByBinClean             [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11084 ByBinClean", AnsiString(""));

                        sBinTrayLinked          [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "BinTrayLinked(11098)",  AnsiString("0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"));    //Ifor 20240730 add : BinTrayLinked
                        sBinLinked              [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "BinLinked(11099)",  AnsiString("0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;"));    //Ifor 20240730 add : BinLinked
                        sMagazineSetup          [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "MagazineSetup(11100)", AnsiString("0"));

                        sAOIBinTraySetting      [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "AOIBinTraySetting", AnsiString(""));//Eastsun 20260316 : AOI 要分bin
                    }
                }
            }
        }
        else if(tag==eBinFT_MRT)                                                //Steven 20161123 : eBinRT_ART --> eBinFT_ART
        {                                                                       //Ifor 20170316 (wei) add KYEC MRT Mode
            GroupName="Bin Func MRT_FT";                                        //wei 20161124 修改eBinFT_ART存成Bin Func ART_RT問題
            if(CheckSectionExist(szDir, GroupName))
            {
                iNewBinFormat=CheckAndReadIniData(szDir, GroupName, "NewBinFormat", 77);//kevin 2016028 使用新的bin format
                if(CUSTOMER_CODE==CC_ASE_KaohSiung)                             //kevin 2016028 add 使用新的bin format
                {
                    if(iNewBinFormat!=77)                                       //kevin 20160928
                        bHasNewSetupData=true;
                }
                else
                {
                    bHasNewSetupData=true;
                }

                if(bHasNewSetupData)                                            //kevin 20161108
                {
                    sBinTraySetT3Pos        [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "4001 BinTraySetting", AnsiString(""));
                    sBinDoubleContact       [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "4002 BinDoubleContac", AnsiString(""));
                    sBinType                [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "4003 BinType", AnsiString(""));
                    sBinConsFail            [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "4004 BinConsFail", AnsiString(""));
                    sBinEnableFail          [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11241 BinEnableFail", AnsiString(""));
                    sBinFailPercent         [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11242 BinFailPercent", AnsiString(""));
                    sBinFailIgnore          [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11243 BinFailIgnore", AnsiString(""));
                    sBinCountEnable         [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11244 BinCountEnable", AnsiString(""));
                    sBinCountNumber         [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11245 BinCountNumber", AnsiString(""));
                    sBinCountIgnore         [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11246 BinCountIgnore", AnsiString(""));
                    sSpecialBinByArm        [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11247 SpecialBinByArm", AnsiString(""));
                    sSpecialBinCountByArm   [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11248 SpecialBinCountByArm", AnsiString(""));
                    sSpecialBinBySocket     [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11249 SpecialBinBySocket", AnsiString(""));
                    sSpecialBinCountBySocket[tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11250 SpecialBinCountBySocket", AnsiString(""));
                    sLowYield               [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11280 LowYield", AnsiString(""));
                    sArmYield               [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11281 ArmYield", AnsiString(""));
                    sSiteYield              [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11282 SiteYield", AnsiString(""));
                    sBySiteClean            [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11283 BySiteClean", AnsiString(""));
                    sByBinClean             [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11284 ByBinClean", AnsiString(""));

                    sBinTrayLinked          [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "BinTrayLinked(11298)",  AnsiString("0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"));    //Ifor 20240730 add : BinTrayLinked
                    sBinLinked              [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "BinLinked(11299)",  AnsiString("0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;"));    //Ifor 20240730 add : BinLinked
                    sMagazineSetup          [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "MagazineSetup(11300)", AnsiString("0"));

                    // (B14) GOLDEN BUG (golden cBinSel.cpp :5301): every other tag's
                    // AOIBinTraySetting key is the bare literal "AOIBinTraySetting" -- only
                    // FT_MRT concatenates GroupName onto it ("Bin Func MRT_FTAOIBinTraySetting"),
                    // so this tag's AOI-bin setting silently reads/writes a DIFFERENT ini key
                    // than every sibling tag. Preserved verbatim (not "fixed" -- see project
                    // translation policy).
                    sAOIBinTraySetting      [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, GroupName + "AOIBinTraySetting", AnsiString(""));//Eastsun 20260316 : AOI 要分bin
                }
            }
        }
        else if(tag==eBinRT_MRT)                                                //Steven 20161123 : eBinFT_ART --> eBinRT_ART
        {
            GroupName="Bin Func MRT_RT";                                        //wei 20161124 修改eBinRT_ART存成Bin Func ART_FT問題
            if(CheckSectionExist(szDir, GroupName))
            {
                iNewBinFormat=CheckAndReadIniData(szDir, GroupName, "NewBinFormat",77);//kevin 2016028 使用新的bin format
                if(CUSTOMER_CODE==CC_ASE_KaohSiung)                             //kevin 2016028 add 使用新的bin format
                {
                    if(iNewBinFormat!=77)                                       //kevin 20160928
                        bHasNewSetupData=true;
                }
                else
                {
                    bHasNewSetupData=true;
                }

                if(bHasNewSetupData)                                            //kevin 20161108
                {
                    sBinTraySetT3Pos        [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "4101 BinTraySetting", AnsiString(""));
                    sBinDoubleContact       [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "4102 BinDoubleContac", AnsiString(""));
                    sBinType                [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "4103 BinType", AnsiString(""));
                    sBinConsFail            [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "4104 BinConsFail", AnsiString(""));
                    sBinEnableFail          [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11341 BinEnableFail", AnsiString(""));
                    sBinFailPercent         [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11342 BinFailPercent", AnsiString(""));
                    sBinFailIgnore          [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11343 BinFailIgnore", AnsiString(""));
                    sBinCountEnable         [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11344 BinCountEnable", AnsiString(""));
                    sBinCountNumber         [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11345 BinCountNumber", AnsiString(""));
                    sBinCountIgnore         [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11346 BinCountIgnore", AnsiString(""));
                    sSpecialBinByArm        [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11347 SpecialBinByArm", AnsiString(""));
                    sSpecialBinCountByArm   [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11348 SpecialBinCountByArm", AnsiString(""));
                    sSpecialBinBySocket     [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11349 SpecialBinBySocket", AnsiString(""));
                    sSpecialBinCountBySocket[tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11350 SpecialBinCountBySocket", AnsiString(""));
                    sLowYield               [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11380 LowYield", AnsiString(""));
                    sArmYield               [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11381 ArmYield", AnsiString(""));
                    sSiteYield              [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11382 SiteYield", AnsiString(""));
                    sBySiteClean            [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11383 BySiteClean", AnsiString(""));
                    sByBinClean             [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "11384 ByBinClean", AnsiString(""));

                    sBinTrayLinked          [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "BinTrayLinked(11398)",  AnsiString("0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"));    //Ifor 20240730 add : BinTrayLinked
                    sBinLinked              [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "BinLinked(11399)",  AnsiString("0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;0;"));    //Ifor 20240730 add : BinLinked
                    sMagazineSetup          [tag]->CommaText=CheckAndReadIniData(szDir, GroupName, "MagazineSetup(11400)", AnsiString("0"));

                    // (B15) GOLDEN BUG (golden cBinSel.cpp :5347): same shape as (B14) above,
                    // but with a leading space ("Bin Func MRT_RT AOIBinTraySetting") -- a
                    // second, independently-inconsistent copy-paste variant, not the same key
                    // as (B14)'s FT_MRT variant either. Preserved verbatim.
                    sAOIBinTraySetting      [tag]->CommaText=CheckAndReadIniData(szDir, GroupName,GroupName + " AOIBinTraySetting", AnsiString(""));//Eastsun 20260316 : AOI 要分bin
                }
            }
        }
    }
//    QQ2=MyQQTimer.LatchCycleTime()/1000.0;
//    Memo1->Lines->Add(" ReadFunctionDataB"+AnsiString(tag)+": "+AnsiString(QQ2)+" ="+AnsiString(QQ2-QQ1));
//    QQ1=QQ2;

    if(bHasNewSetupData==false)                                                 //這裡是為了跟舊版的相容, 所以新的功能不需要加在這邊
    {
        for(int i=0; i<iTestBinCount; i++)
        {                                                                       //QQQ
            GroupStr.sprintf("Category%d", i);                                  //kevin 20140317

            //Scan-------------
            BinSelect[tag].bScanner[i]=0;
            MyBinPanel[tag]->bScan[i]=BinSelect[tag].bScanner[i];

            if(IniConfig.bA30SetupTeachFunction && tag==OffT && LastSet.bNeedSetupTeach && (i>=1 && i<=iFixRight))  //JerryYang 20180921 Setup Teach功能
            {
                BinSelect[tag].iCatDataT3Pos[i]=CheckAndReadIniData(szDir, GroupStr, "Bin", i);      //Bin 1..6 to Auto 123 Fix 123
            }
            else
            {
                BinSelect[tag].iCatDataT3Pos[i]=CheckAndReadIniData(szDir, GroupStr, "Bin", (tag==OffT)?e3PosFix2:e3PosNoUse);      //Steven 20150209 : ePosFix1 --> ePosFix2 for HT9046LS + Rotate
            }

            if(BinSelect[tag].iCatDataT3Pos[i]<e3PosNoUse)
                BinSelect[tag].iCatDataT3Pos[i]= e3PosNoUse;

            if(USE_ROTATE==eCynRot && BinSelect[tag].iCatDataT3Pos[i]-1==iRotate_Out_Tray6) //kevin 20130812  氣缸版   //Steven 20121001 : 旋轉Kit
                BinSelect[tag].iCatDataT3Pos[i]= e3PosNoUse;

            if(AUTO3_IS_MAGAZINE==1 && BinSelect[tag].iCatDataT3Pos[i]==e3PosAuto3)         //JerryYang 20221207 : Magazine not use AUTO3
                BinSelect[tag].iCatDataT3Pos[i]=e3PosNoUse;

            if(CosFunction.bLoaderTrayToAuto1 && TrayForm.LoaderToEmptyColor[iRunStartMode]==2 && BinSelect[tag].iCatDataT3Pos[i]==e3PosAuto1)
                BinSelect[tag].iCatDataT3Pos[i]=e3PosNoUse;

            if(AUTO3_IS_MAGAZINE==1 && TestIF_File.iMagFixTrayType==1 && BinSelect[tag].iCatDataT3Pos[i]>=e3PosFix1 && BinSelect[tag].iCatDataT3Pos[i]<=e3PosFix6)  //JerryYang 20221215 : Magazine把fix區當buffer區功能
                BinSelect[tag].iCatDataT3Pos[i]=e3PosNoUse;

            if(TrayForm.iFixTrayMode==false && BinSelect[tag].iCatDataT3Pos[i]>e3PosFix3)   //Fix滿盤
                BinSelect[tag].iCatDataT3Pos[i]=e3PosNoUse;

            if(CosFunction.bUseTrayUpDownSet)                                   //wei 20160224 TSMC FIX UPDOWN
            {
                if(TrayForm.iFixTrayMode && BinSelect[tag].iCatDataT3Pos[i]==e3PosFix4 && TrayForm.bTrayUpDownSet[e3PosFix4-e3PosFix4]==false)
                    BinSelect[tag].iCatDataT3Pos[i]=e3PosNoUse;
                else if(TrayForm.iFixTrayMode && BinSelect[tag].iCatDataT3Pos[i]==e3PosFix5 && TrayForm.bTrayUpDownSet[e3PosFix5-e3PosFix4]==false)
                    BinSelect[tag].iCatDataT3Pos[i]=e3PosNoUse;
                else if(TrayForm.iFixTrayMode && BinSelect[tag].iCatDataT3Pos[i]==e3PosFix6 && TrayForm.bTrayUpDownSet[e3PosFix6-e3PosFix4]==false)
                    BinSelect[tag].iCatDataT3Pos[i]=e3PosNoUse;
            }

            if(TestIF_File.bEnableQASampling &&                                 //Steven 20190326 : QA Sampling
               BinSelect[tag].iCatDataT3Pos[i]==TestIF_File.iQASamplingT3Pos)
            {
                BinSelect[tag].iCatDataT3Pos[i]=e3PosNoUse;
            }

            MyBinPanel[tag]->BackT6PosTray[i][eBinNotUse+BinSelect[tag].iCatDataT3Pos[i]]=1;

            //Double Contact------
            if(Prod.bD22SupportMultiDoubleContact==false)                       //Steven 20170920 : 重新整理Double Contact選項
               //IniConfig.bD22VerifyMode==false)                               //Sam 20231117 : 整合到 QA 模式      //Sam 20221012 : 新增 VerifyMode 功能
            {
                BinSelect[tag].iDBContact[i]    =0;
            }
            else
            {
                BinSelect[tag].iDBContact[i]    =CheckAndReadIniData(szDir, GroupStr, "Contact", 0);
            }
            MyBinPanel[tag]->i2Contact[i]       =BinSelect[tag].iDBContact[i];

            //Con. Fail--------
            BinSelect[tag].bConsFail[i]         =CheckAndReadIniData(szDir, GroupStr, "Cons.Fail", 0);
            MyBinPanel[tag]->bConFail[i]        =BinSelect[tag].bConsFail[i];

            //Persent Enable------                                              //Steven 20140529 Start: Fail Persent & Count同時存在
            BinSelect[tag].bFailure[i]          =CheckAndReadIniData(szDir, GroupStr, "Fail Percent", 0);
            MyBinPanel[tag]->bPersentEnable[i]  =BinSelect[tag].bFailure[i];

            //Persent Ignore------
            BinSelect[tag].iPersentIgnore[i]    =CheckAndReadIniData(szDir, GroupStr, "Fail Percent Ignore", 0);
            MyBinPanel[tag]->iPersentIgnore[i]  =BinSelect[tag].iPersentIgnore[i];

            //Persent Number------
            BinSelect[tag].dFailureLimit[i]     =CheckAndReadIniData(szDir, GroupStr, "Fail Percent Limit", 0.0);
            MyBinPanel[tag]->dPersentNumber[i]  =BinSelect[tag].dFailureLimit[i];

            //Count Enable------
            BinSelect[tag].bFailCountEnable[i]  =CheckAndReadIniData(szDir, GroupStr, "Fail Count", 0);
            MyBinPanel[tag]->bCountEnable[i]    =BinSelect[tag].bFailCountEnable[i];

            //Count Ignore------
            BinSelect[tag].iFailCountIgnore[i]  =CheckAndReadIniData(szDir, GroupStr, "Fail Count Ignore", 0);
            MyBinPanel[tag]->iCountIgnore[i]    =BinSelect[tag].iFailCountIgnore[i];

            //Count Number------
            BinSelect[tag].iFailCountLimit[i]   =CheckAndReadIniData(szDir, GroupStr, "Fail Count Limit", 0.0);
            MyBinPanel[tag]->iCountNumber[i]    =BinSelect[tag].iFailCountLimit[i];

            BinSelect[tag].bSpecialBinByArm[i]          =CheckAndReadIniData(szDir, GroupStr, "Special Bin By Arm", false);
            MyBinPanel[tag]->bSpecialBinByArm[i]        =BinSelect[tag].bSpecialBinByArm[i];

            BinSelect[tag].iSpecialBinCountByArm[i]     =CheckAndReadIniData(szDir, GroupStr, "Special Bin Count By Arm", 0);
            MyBinPanel[tag]->iSpecialBinCountByArm[i]   =BinSelect[tag].iSpecialBinCountByArm[i];

            BinSelect[tag].bSpecialBinBySocket[i]       =CheckAndReadIniData(szDir, GroupStr, "Special Bin By Socket", false);
            MyBinPanel[tag]->bSpecialBinBySocket[i]     =BinSelect[tag].bSpecialBinBySocket[i];

            BinSelect[tag].iSpecialBinCountBySocket[i]  =CheckAndReadIniData(szDir, GroupStr, "Special Bin Count By Socket", 0.0);
            MyBinPanel[tag]->iSpecialBinCountBySocket[i]=BinSelect[tag].iSpecialBinCountBySocket[i];

            BinSelect[tag].bLowYield[i]     =CheckAndReadIniData(szDir, GroupStr, "By Bin Low Yield", false);                    //Steven 20140828 : By Bin Yield Monitor
            MyBinPanel[tag]->bLowYield[i]   =BinSelect[tag].bLowYield[i];

            BinSelect[tag].bArmYield[i]     =CheckAndReadIniData(szDir, GroupStr, "By Bin and Site Compare Arm Yield", false);   //Steven 20140828 : By Bin Arm Yield Monitor
            MyBinPanel[tag]->bArmYield[i]   =BinSelect[tag].bArmYield[i];

            BinSelect[tag].bSiteYield[i]    =CheckAndReadIniData(szDir, GroupStr, "By Bin Compare Site Yield", false);           //Steven 20140828 : By Bin Site Yield Monitor
            MyBinPanel[tag]->bSiteYield[i]  =BinSelect[tag].bSiteYield[i];
        }

        for(int i=0; i<iTestBinCount; i++)
        {
            if(i<sBinDoubleContact[tag]->Count)
                sBinDoubleContact       [tag]->Strings[i]=BinSelect[tag].iDBContact[i];
            if(i<sBinConsFail[tag]->Count)
                sBinConsFail            [tag]->Strings[i]=(BinSelect[tag].bConsFail[i])?"1":"0";
            if(i<sBinEnableFail[tag]->Count)
                sBinEnableFail          [tag]->Strings[i]=(BinSelect[tag].bFailure[i]  )?"1":"0";
            if(i<sBinFailPercent[tag]->Count)
                sBinFailPercent         [tag]->Strings[i]=AnsiString(BinSelect[tag].dFailureLimit[i]);
            if(i<sBinFailIgnore[tag]->Count)
                sBinFailIgnore          [tag]->Strings[i]=AnsiString(BinSelect[tag].iPersentIgnore[i]);
            if(i<sBinCountEnable[tag]->Count)
                sBinCountEnable         [tag]->Strings[i]=(BinSelect[tag].bFailCountEnable[i])?"1":"0";
            if(i<sBinCountIgnore[tag]->Count)
                sBinCountIgnore         [tag]->Strings[i]=AnsiString(BinSelect[tag].iFailCountIgnore[i]);
            if(i<sBinCountNumber[tag]->Count)
                sBinCountNumber         [tag]->Strings[i]=AnsiString(BinSelect[tag].iFailCountLimit[i]);
            if(i<sSpecialBinByArm[tag]->Count)
                sSpecialBinByArm        [tag]->Strings[i]=(BinSelect[tag].bSpecialBinByArm[i]?"1":"0");
            if(i<sSpecialBinCountByArm[tag]->Count)
                sSpecialBinCountByArm   [tag]->Strings[i]=AnsiString(BinSelect[tag].iSpecialBinCountByArm[i]);
            if(i<sSpecialBinBySocket[tag]->Count)
                sSpecialBinBySocket     [tag]->Strings[i]=(BinSelect[tag].bSpecialBinBySocket[i]?"1":"0");
            if(i<sSpecialBinCountBySocket[tag]->Count)
                sSpecialBinCountBySocket[tag]->Strings[i]=AnsiString(BinSelect[tag].iSpecialBinCountBySocket[i]);
            if(i<sLowYield[tag]->Count)                                         //Steven 20140828 : By Bin Yield Monitor
                sLowYield               [tag]->Strings[i]=(BinSelect[tag].bLowYield[i]?"1":"0");
            if(i<sArmYield[tag]->Count)                                         //Steven 20140828 : By Bin Arm Yield Monitor
                sArmYield               [tag]->Strings[i]=(BinSelect[tag].bArmYield[i]?"1":"0");
            if(i<sSiteYield[tag]->Count)                                        //Steven 20140828 : By Bin Site Yield Monitor
                sSiteYield              [tag]->Strings[i]=(BinSelect[tag].bSiteYield[i]?"1":"0");
            if(i<sBinTraySetT3Pos[tag]->Count)
                sBinTraySetT3Pos        [tag]->Strings[i]=AnsiString(BinSelect[tag].iCatDataT3Pos[i]);
        }
        SaveFunctionData(tag, szDir);
    }
    else
    {
        for(int i=0; i<iTestBinCount; i++)
        {
            if(IniConfig.bD22SupportMultiDoubleContact==false &&                //Steven 20170920 : 重新整理Double Contact選項
               IniConfig.bD22VerifyMode==false)                                 //Sam 20221012 : 新增 VerifyMode 功能
            {
                sBinDoubleContact[tag]->Clear();
            }

            BinSelect[tag].iDBContact[i]                =(i<sBinDoubleContact[tag]->Count       )?atoi(AnsiString(sBinDoubleContact        [tag]->Strings[i]).c_str()):false;
            BinSelect[tag].bConsFail[i]                 =(i<sBinConsFail[tag]->Count            )?atoi(AnsiString(sBinConsFail             [tag]->Strings[i]).c_str()):false;
            BinSelect[tag].bFailure[i]                  =(i<sBinEnableFail[tag]->Count          )?atoi(AnsiString(sBinEnableFail           [tag]->Strings[i]).c_str()):false;
            BinSelect[tag].dFailureLimit[i]             =(i<sBinFailPercent[tag]->Count         )?atof(AnsiString(sBinFailPercent          [tag]->Strings[i]).c_str()):0.0;         //JerryYang 20160613 修正By Bin Yield要能設定到小數點
            BinSelect[tag].iPersentIgnore[i]            =(i<sBinFailIgnore[tag]->Count          )?atoi(AnsiString(sBinFailIgnore           [tag]->Strings[i]).c_str()):0;
            BinSelect[tag].bFailCountEnable[i]          =(i<sBinCountEnable[tag]->Count         )?atoi(AnsiString(sBinCountEnable          [tag]->Strings[i]).c_str()):false;
            BinSelect[tag].iFailCountIgnore[i]          =(i<sBinCountIgnore[tag]->Count         )?atoi(AnsiString(sBinCountIgnore          [tag]->Strings[i]).c_str()):0;
            BinSelect[tag].iFailCountLimit[i]           =(i<sBinCountNumber[tag]->Count         )?atoi(AnsiString(sBinCountNumber          [tag]->Strings[i]).c_str()):0;
            BinSelect[tag].bSpecialBinByArm[i]          =(i<sSpecialBinByArm[tag]->Count        )?atoi(AnsiString(sSpecialBinByArm         [tag]->Strings[i]).c_str()):false;
            BinSelect[tag].iSpecialBinCountByArm[i]     =(i<sSpecialBinCountByArm[tag]->Count   )?atoi(AnsiString(sSpecialBinCountByArm    [tag]->Strings[i]).c_str()):0;
            BinSelect[tag].bSpecialBinBySocket[i]       =(i<sSpecialBinBySocket[tag]->Count     )?atoi(AnsiString(sSpecialBinBySocket      [tag]->Strings[i]).c_str()):false;
            BinSelect[tag].iSpecialBinCountBySocket[i]  =(i<sSpecialBinCountBySocket[tag]->Count)?atoi(AnsiString(sSpecialBinCountBySocket [tag]->Strings[i]).c_str()):0;
            BinSelect[tag].bLowYield[i]                 =(i<sLowYield[tag]->Count               )?atoi(AnsiString(sLowYield                [tag]->Strings[i]).c_str()):false;
            BinSelect[tag].bArmYield[i]                 =(i<sArmYield[tag]->Count               )?atoi(AnsiString(sArmYield                [tag]->Strings[i]).c_str()):false;
            BinSelect[tag].bSiteYield[i]                =(i<sSiteYield[tag]->Count              )?atoi(AnsiString(sSiteYield               [tag]->Strings[i]).c_str()):false;
            BinSelect[tag].iAutoCleanByBin[i]           =(i<sBySiteClean[tag]->Count            )?atoi(AnsiString(sBySiteClean             [tag]->Strings[i]).c_str()):0;
            BinSelect[tag].iAutoCleanBySite[i]          =(i<sByBinClean[tag]->Count             )?atoi(AnsiString(sByBinClean              [tag]->Strings[i]).c_str()):0;
            BinSelect[tag].iCatDataT3Pos[i]             =(i<sBinTraySetT3Pos[tag]->Count        )?atoi(AnsiString(sBinTraySetT3Pos         [tag]->Strings[i]).c_str()):e3PosNoUse;
            BinSelect[tag].bBinTrayLinked[i]            =(i<sBinTrayLinked[tag]->Count          )?atoi(AnsiString(sBinTrayLinked           [tag]->Strings[i]).c_str()):false;
            BinSelect[tag].iAOICategData[i]             =(i<sAOIBinTraySetting[tag]->Count      )?atoi(AnsiString(sAOIBinTraySetting       [tag]->Strings[i]).c_str()):ePosNoUse;   //Eastsun 20260316 : AOI 分BIN

            if(CosFunction.bBySiteByBinPercentCompare==true &&                  //JerryYang 20170712 (Steven) by site by bin compare percent
               CosFunction.bByBinAlarmFromYieldForm==false)                     //jou 20180113 (Steven) : By Site By Bin Percent Compare From Yield form
            {
                BinSelect[tag].bSpecBinBySiteCompareEnable[i]       =(i<sSpecBinBySiteCompareEnable[tag]->Count       )?atoi(AnsiString(sSpecBinBySiteCompareEnable[tag]->Strings[i]).c_str()):false;
                BinSelect[tag].iSpecBinBySiteCompareIgnore[i]       =(i<sSpecBinBySiteCompareIgnore[tag]->Count       )?atoi(AnsiString(sSpecBinBySiteCompareIgnore[tag]->Strings[i]).c_str()):0;
                BinSelect[tag].dSpecBinBySiteComparePercent[i]      =(i<sSpecBinBySiteComparePercent[tag]->Count      )?atof(AnsiString(sSpecBinBySiteComparePercent[tag]->Strings[i]).c_str()):0.0;
                BinSelect[tag].bSpecBinByArmPerSiteCompareEnable[i] =(i<sSpecBinByArmPerSiteCompareEnable[tag]->Count )?atoi(AnsiString(sSpecBinByArmPerSiteCompareEnable[tag]->Strings[i]).c_str()):false;
                BinSelect[tag].iSpecBinByArmPerSiteCompareIgnore[i] =(i<sSpecBinByArmPerSiteCompareIgnore[tag]->Count )?atoi(AnsiString(sSpecBinByArmPerSiteCompareIgnore[tag]->Strings[i]).c_str()):0;
                BinSelect[tag].dSpecBinByArmPerSiteComparePercent[i]=(i<sSpecBinByArmPerSiteComparePercent[tag]->Count)?atof(AnsiString(sSpecBinByArmPerSiteComparePercent[tag]->Strings[i]).c_str()):0.0;
            }

            if(i<sBinTraySetT3Pos[tag]->Count)
            {
                iData=atoi(AnsiString(sBinTraySetT3Pos[tag]->Strings[i]).c_str());

                if(iData<e3PosNoUse)
                    BinSelect[tag].iCatDataT3Pos[i]=e3PosNoUse;

                iT3=BinSelect[tag].iCatDataT3Pos[i]-1;
                if(iT3>=e3Auto1)
                {
                    iT6=iTo6Unload[iT3];
                    if(Prod.iTrayType[iT6]==tNotUse)
                        iData=e3PosNoUse;

                    if(CosFunction.bLoaderTrayToAuto1 && TrayForm.LoaderToEmptyColor[iRunStartMode]==2)
                    {
                        if(BinSelect[tag].iCatDataT3Pos[i]==e3PosAuto1)
                            iData=e3PosNoUse;
                    }

                    if(AUTO3_IS_MAGAZINE==1)
                    {
                        if(BinSelect[tag].iCatDataT3Pos[i]==e3PosAuto3)         //JerryYang 20221207 : Magazine not use AUTO3
                            iData=e3PosNoUse;

                        if(TestIF_File.iMagFixTrayType==1 && iT6>=iFixMin && iT6<=iFixRightHalf)
                            iData=e3PosNoUse;
                    }
                }
                else
                {
                    iData=e3PosNoUse;
                }

                BinSelect[tag].iCatDataT3Pos[i]=iData;
                sBinTraySetT3Pos[tag]->Strings[i]=AnsiString(iData);
            }
            else
            {
                BinSelect[tag].iCatDataT3Pos[i]=e3PosNoUse;
            }
        }

        for(int i=0; i<iTestBinCount; i++)
        {
            if(TestIF_File.bEnableQASampling &&                                 //Steven 20190326 : QA Sampling
               BinSelect[tag].iCatDataT3Pos[i]==TestIF_File.iQASamplingT3Pos)
            {
                BinSelect[tag].iCatDataT3Pos[i]=e3PosNoUse;
            }

            int iUnloader=0;
            iUnloader=iTo6PosUnload[BinSelect[tag].iCatDataT3Pos[i]];           //JerryYang 20230915 : test     //JerryYang 20230926

            MyBinPanel[tag]->BackT6PosTray[i][eBinNotUse+iUnloader]=1;
            MyBinPanel[tag]->i2Contact[i]               =BinSelect[tag].iDBContact[i];
            MyBinPanel[tag]->bConFail[i]                =BinSelect[tag].bConsFail[i];
            MyBinPanel[tag]->bPersentEnable[i]          =BinSelect[tag].bFailure[i];
            MyBinPanel[tag]->iPersentIgnore[i]          =BinSelect[tag].iPersentIgnore[i];
            MyBinPanel[tag]->dPersentNumber[i]          =BinSelect[tag].dFailureLimit[i];
            MyBinPanel[tag]->bCountEnable[i]            =BinSelect[tag].bFailCountEnable[i];
            MyBinPanel[tag]->iCountIgnore[i]            =BinSelect[tag].iFailCountIgnore[i];
            MyBinPanel[tag]->iCountNumber[i]            =BinSelect[tag].iFailCountLimit[i];
            MyBinPanel[tag]->bSpecialBinByArm[i]        =BinSelect[tag].bSpecialBinByArm[i];
            MyBinPanel[tag]->iSpecialBinCountByArm[i]   =BinSelect[tag].iSpecialBinCountByArm[i];
            MyBinPanel[tag]->bSpecialBinBySocket[i]     =BinSelect[tag].bSpecialBinBySocket[i];
            MyBinPanel[tag]->iSpecialBinCountBySocket[i]=BinSelect[tag].iSpecialBinCountBySocket[i];
            MyBinPanel[tag]->bLowYield[i]               =BinSelect[tag].bLowYield[i];
            MyBinPanel[tag]->bArmYield[i]               =BinSelect[tag].bArmYield[i];
            MyBinPanel[tag]->bSiteYield[i]              =BinSelect[tag].bSiteYield[i];
            MyBinPanel[tag]->iAutoCleanByBin[i]         =BinSelect[tag].iAutoCleanByBin[i];             //Steven 20160308 : By Bin count do auto clean
            MyBinPanel[tag]->iAutoCleanBySite[i]        =BinSelect[tag].iAutoCleanBySite[i];            //Steven 20160308 : By Bin pre site count do auto clean

            if(CosFunction.bBySiteByBinPercentCompare==true &&                  //JerryYang 20170712 (Steven) by site by bin compare percent
               CosFunction.bByBinAlarmFromYieldForm==false)                     //jou 20180113 (Steven) : By Site By Bin Percent Compare From Yield form
            {
                MyBinPanel[tag]->bSpecBinBySiteCompareEnable[i]       =BinSelect[tag].bSpecBinBySiteCompareEnable[i];
                MyBinPanel[tag]->iSpecBinBySiteCompareIgnore[i]       =BinSelect[tag].iSpecBinBySiteCompareIgnore[i];
                MyBinPanel[tag]->dSpecBinBySiteComparePercent[i]      =BinSelect[tag].dSpecBinBySiteComparePercent[i];
                MyBinPanel[tag]->bSpecBinByArmPerSiteCompareEnable[i] =BinSelect[tag].bSpecBinByArmPerSiteCompareEnable[i];
                MyBinPanel[tag]->iSpecBinByArmPerSiteCompareIgnore[i] =BinSelect[tag].iSpecBinByArmPerSiteCompareIgnore[i];
                MyBinPanel[tag]->dSpecBinByArmPerSiteComparePercent[i]=BinSelect[tag].dSpecBinByArmPerSiteComparePercent[i];
            }
        }

        // AI(W906-FW-BinSel-WB) 20260819: GATE -- MyBinPanel[tag]->ed* (TEdit*,
        // golden TMyBinPanel widget half, cBinSel.h public block) has no
        // TMyBinPanelData counterpart (`grep -n "edBinSetT3Pos\|edBinSettingConFail\|edSpecBinByArmPerSiteComparePercent\|edBinSetT6Link" forms/fBinSel.h`
        // -- 0 hits, 20260819; TMyBinPanelData is the data-only stand-in declared
        // by WAVE A's CTOR NOTE, no `TEdit *ed*` members). These 22 assignments
        // are pure widget-Text mirrors of the sXxx[tag]->CommaText this method
        // already populated above (no further state derives from them anywhere
        // in this wave's ACTIVE call graph) -- gated as a block rather than
        // fabricating 22 facade members with no reader.
#if 0
        MyBinPanel[tag]->edBinSetT3Pos->Text                =sBinTraySetT3Pos[tag]->CommaText;
        MyBinPanel[tag]->edBinSetting2Contact->Text         =sBinDoubleContact[tag]->CommaText;
        MyBinPanel[tag]->edBinSettingConFail->Text          =sBinConsFail[tag]->CommaText;
        MyBinPanel[tag]->edBinSettingFailPersentEnable->Text=sBinEnableFail[tag]->CommaText;
        MyBinPanel[tag]->edBinSettingFailPersentIgnore->Text=sBinFailPercent[tag]->CommaText;
        MyBinPanel[tag]->edBinSettingFailPersentNumber->Text=sBinFailIgnore[tag]->CommaText;
        MyBinPanel[tag]->edBinSettingFailCountEnable->Text  =sBinCountEnable[tag]->CommaText;
        MyBinPanel[tag]->edBinSettingFailCountIgnore->Text  =sBinCountIgnore[tag]->CommaText;
        MyBinPanel[tag]->edBinSettingFailCountNumber->Text  =sBinCountNumber[tag]->CommaText;
        MyBinPanel[tag]->edSpecialBinByArm->Text            =sSpecialBinByArm[tag]->CommaText;
        MyBinPanel[tag]->edSpecialBinCountByArm->Text       =sSpecialBinCountByArm[tag]->CommaText;
        MyBinPanel[tag]->edSpecialBinBySocket->Text         =sSpecialBinBySocket[tag]->CommaText;
        MyBinPanel[tag]->edSpecialBinCountBySocket->Text    =sSpecialBinCountBySocket[tag]->CommaText;
        MyBinPanel[tag]->edLowYield->Text                   =sLowYield[tag]->CommaText;
        MyBinPanel[tag]->edArmYield->Text                   =sArmYield[tag]->CommaText;
        MyBinPanel[tag]->edSiteYield->Text                  =sSiteYield[tag]->CommaText;
        MyBinPanel[tag]->edBySiteClean->Text                =sBySiteClean[tag]->CommaText;
        MyBinPanel[tag]->edByBinClean->Text                 =sByBinClean[tag]->CommaText;

        MyBinPanel[tag]->edSpecBinBySiteCompareEnable->Text         =sSpecBinBySiteCompareEnable[tag]->CommaText;
        MyBinPanel[tag]->edSpecBinBySiteCompareIgnore->Text         =sSpecBinBySiteCompareIgnore[tag]->CommaText;
        MyBinPanel[tag]->edSpecBinBySiteComparePercent->Text        =sSpecBinBySiteComparePercent[tag]->CommaText;
        MyBinPanel[tag]->edSpecBinByArmPerSiteCompareEnable->Text   =sSpecBinByArmPerSiteCompareEnable[tag]->CommaText;
        MyBinPanel[tag]->edSpecBinByArmPerSiteCompareIgnore->Text   =sSpecBinByArmPerSiteCompareIgnore[tag]->CommaText;
        MyBinPanel[tag]->edSpecBinByArmPerSiteComparePercent->Text  =sSpecBinByArmPerSiteComparePercent[tag]->CommaText;
        MyBinPanel[tag]->edBinSetT6Link->Text                       =sBinTrayLinked[tag]->CommaText;
#endif

        SaveFunctionData(tag, szDir);                                                  //wei 20161124 修改存檔錯誤的問題
    }
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  SaveFunctionData -- golden :5668-6009. FW-BinSel-WB (this wave). GATE:
//  100% of this method's real effect is `FormSysTools->WriteIniData(...)` --
//  every site becomes `WriteIniData(FileName,...)` verbatim and is `#if 0`-
//  gated per WRITE-PATH GATE TABLE (project policy: writes to shared
//  recipe-config are gated regardless of transcription effort). ACTIVE: the
//  `TransferBinTrayStrToName(tag)` calls interleaved between gated write runs
//  (golden :5679/:5722/:5766/:5809/:5852/:5895/:5927/:5970 -- already-real,
//  in-memory-only, no file I/O) and the full if/else-if tag-selection
//  structure (本體結構照翻 -- kept so a future wave can un-gate write-by-
//  write without re-deriving which GroupName/key belongs to which tag).
//  DEVIATION: signature gains an `AnsiString FileName` parameter -- same
//  `FormSysTools->OpenFormData(FileName)`-elision substitution as
//  ReadFunctionData above (golden :2542, `SaveOther` opens `FileName` right
//  before calling this); `ReadFunctionData`'s own two internal calls
//  (golden :5508/:5664) now pass its own `szDir` as this `FileName` --
//  same file, same FormSysTools object in golden, so this is the same
//  parameter under two names, not two different files.
//---------------------------------------------------------------------------
void TfBinSel::SaveFunctionData(int tag, AnsiString FileName)
{
    AnsiString GroupName;
    (void)FileName;   // only feeds the gated WriteIniData calls below in this wave
    (void)GroupName;  // ditto -- read only inside the #if 0 blocks below
    if(tag==eBinRT)
    {
        if((IniConfig.bA02BinModelPrime==true  && IniConfig.bFTBin2RTBin==true && (iBinModelPrime==0 || CosFunction.bDisableRTBinSet)) ||        //Steven 20160623 : 當IniConfig.bFTBin2RTBin==true && 讀檔時, RT Bin要跟FT Bin一樣
           (IniConfig.bA02BinModelPrime==false && (IniConfig.bFTBin2RTBin==true || CosFunction.bDisableRTBinSet)))
        {
            GroupName="Bin Func FT";
#if 0
            // GATE (write-policy)
            WriteIniData(FileName, GroupName, "NewBinFormat",               1);   //kevin 20160928 使用新的bin format
            WriteIniData(FileName, GroupName, "3617 BinTraySetting",        sBinTraySetT3Pos        [tag]->CommaText);
#endif
            TransferBinTrayStrToName(tag);
#if 0
            // GATE (write-policy)
            WriteIniData(FileName, GroupName, "3617 BinTraySetName",        sBinTraySetT3PosName    [tag]->CommaText);
            WriteIniData(FileName, GroupName, "3636 BinDoubleContac",       sBinDoubleContact       [tag]->CommaText);
            WriteIniData(FileName, GroupName, "3656 BinType",               sBinType                [tag]->CommaText);
            WriteIniData(FileName, GroupName, "3676 BinConsFail",           sBinConsFail            [tag]->CommaText);
            WriteIniData(FileName, GroupName, "10641 BinEnableFail",        sBinEnableFail          [tag]->CommaText);
            WriteIniData(FileName, GroupName, "10642 BinFailPercent",       sBinFailPercent         [tag]->CommaText);
            WriteIniData(FileName, GroupName, "10643 BinFailIgnore",        sBinFailIgnore          [tag]->CommaText);
            WriteIniData(FileName, GroupName, "10644 BinCountEnable",       sBinCountEnable         [tag]->CommaText);
            WriteIniData(FileName, GroupName, "10646 BinCountIgnore",       sBinCountIgnore         [tag]->CommaText);
            WriteIniData(FileName, GroupName, "10645 BinCountNumber",       sBinCountNumber         [tag]->CommaText);
            WriteIniData(FileName, GroupName, "10647 SpecialBinByArm",      sSpecialBinByArm        [tag]->CommaText);
            WriteIniData(FileName, GroupName, "10648 SpecialBinCountByArm", sSpecialBinCountByArm   [tag]->CommaText);
            WriteIniData(FileName, GroupName, "10649 SpecialBinBySocket",   sSpecialBinBySocket     [tag]->CommaText);
            WriteIniData(FileName, GroupName, "10650 SpecialBinCountBySocket", sSpecialBinCountBySocket[tag]->CommaText);
            WriteIniData(FileName, GroupName, "10680 LowYield",             sLowYield               [tag]->CommaText);
            WriteIniData(FileName, GroupName, "10681 ArmYield",             sArmYield               [tag]->CommaText);
            WriteIniData(FileName, GroupName, "10682 SiteYield",            sSiteYield              [tag]->CommaText);
            WriteIniData(FileName, GroupName, "10683 BySiteClean",          sBySiteClean            [tag]->CommaText);
            WriteIniData(FileName, GroupName, "10684 ByBinClean",           sByBinClean             [tag]->CommaText);
#endif

            if(CosFunction.bBySiteByBinPercentCompare==true &&                  //JerryYang 20170712 (Steven) by site by bin compare percent
               CosFunction.bByBinAlarmFromYieldForm==false)                     //jou 20180113 (Steven) : By Site By Bin Percent Compare From Yield form
            {
#if 0
                // GATE (write-policy)
                WriteIniData(FileName, GroupName, "10685 SpecBinBySiteCompareEnable",        sSpecBinBySiteCompareEnable         [tag]->CommaText);
                WriteIniData(FileName, GroupName, "10686 SpecBinBySiteCompareIgnore",        sSpecBinBySiteCompareIgnore         [tag]->CommaText);
                WriteIniData(FileName, GroupName, "10687 SpecBinBySiteComparePercent",       sSpecBinBySiteComparePercent        [tag]->CommaText);
                WriteIniData(FileName, GroupName, "10688 SpecBinByArmPerSiteCompareEnable",  sSpecBinByArmPerSiteCompareEnable   [tag]->CommaText);
                WriteIniData(FileName, GroupName, "10689 SpecBinByArmPerSiteCompareIgnore",  sSpecBinByArmPerSiteCompareIgnore   [tag]->CommaText);
                WriteIniData(FileName, GroupName, "10690 SpecBinByArmPerSiteComparePercent", sSpecBinByArmPerSiteComparePercent  [tag]->CommaText);
#endif
            }

#if 0
            // GATE (write-policy)
            WriteIniData(FileName, GroupName, "BinTrayLinked(10698)", sBinTrayLinked        [tag]->CommaText);
            WriteIniData(FileName, GroupName, "BinLinked(10699)",     sBinLinked            [tag]->CommaText);
            WriteIniData(FileName, GroupName, "MagazineSetup(10700)", sMagazineSetup        [tag]->CommaText);

            WriteIniData(FileName, GroupName,"AOIBinTraySetting", sAOIBinTraySetting    [tag]->CommaText);  //Eastsun 20260316 : AOI 要分bin
#endif
        }
        else
        {
            GroupName="Bin Func RT";
#if 0
            // GATE (write-policy)
            WriteIniData(FileName, GroupName, "NewBinFormat",               1);   //kevin 20160928 使用新的bin format
            WriteIniData(FileName, GroupName, "3717 BinTraySetting",        sBinTraySetT3Pos        [tag]->CommaText);
#endif
            TransferBinTrayStrToName(tag);
#if 0
            // GATE (write-policy)
            WriteIniData(FileName, GroupName, "3717 BinTraySetName",        sBinTraySetT3PosName    [tag]->CommaText);
            WriteIniData(FileName, GroupName, "3718 BinDoubleContac",       sBinDoubleContact       [tag]->CommaText);
            WriteIniData(FileName, GroupName, "3719 BinType",               sBinType                [tag]->CommaText);
            WriteIniData(FileName, GroupName, "3720 BinConsFail",           sBinConsFail            [tag]->CommaText);
            WriteIniData(FileName, GroupName, "10741 BinEnableFail",        sBinEnableFail          [tag]->CommaText);
            WriteIniData(FileName, GroupName, "10742 BinFailPercent",       sBinFailPercent         [tag]->CommaText);
            WriteIniData(FileName, GroupName, "10743 BinFailIgnore",        sBinFailIgnore          [tag]->CommaText);
            WriteIniData(FileName, GroupName, "10744 BinCountEnable",       sBinCountEnable         [tag]->CommaText);
            WriteIniData(FileName, GroupName, "10745 BinCountNumber",       sBinCountNumber         [tag]->CommaText);
            WriteIniData(FileName, GroupName, "10746 BinCountIgnore",       sBinCountIgnore         [tag]->CommaText);
            WriteIniData(FileName, GroupName, "10747 SpecialBinByArm",      sSpecialBinByArm        [tag]->CommaText);
            WriteIniData(FileName, GroupName, "10748 SpecialBinCountByArm", sSpecialBinCountByArm   [tag]->CommaText);
            WriteIniData(FileName, GroupName, "10749 SpecialBinBySocket",   sSpecialBinBySocket     [tag]->CommaText);
            WriteIniData(FileName, GroupName, "10750 SpecialBinCountBySocket", sSpecialBinCountBySocket[tag]->CommaText);
            WriteIniData(FileName, GroupName, "10780 LowYield",             sLowYield               [tag]->CommaText);
            WriteIniData(FileName, GroupName, "10781 ArmYield",             sArmYield               [tag]->CommaText);
            WriteIniData(FileName, GroupName, "10782 SiteYield",            sSiteYield              [tag]->CommaText);
            WriteIniData(FileName, GroupName, "10783 BySiteClean",          sBySiteClean            [tag]->CommaText);
            WriteIniData(FileName, GroupName, "10784 ByBinClean",           sByBinClean             [tag]->CommaText);
#endif

            if(CosFunction.bBySiteByBinPercentCompare==true &&                  //JerryYang 20170712 (Steven) by site by bin compare percent
               CosFunction.bByBinAlarmFromYieldForm==false)                     //jou 20180113 (Steven) : By Site By Bin Percent Compare From Yield form
            {
#if 0
                // GATE (write-policy)
                WriteIniData(FileName, GroupName, "10785 SpecBinBySiteCompareEnable",        sSpecBinBySiteCompareEnable         [tag]->CommaText);
                WriteIniData(FileName, GroupName, "10786 SpecBinBySiteCompareIgnore",        sSpecBinBySiteCompareIgnore         [tag]->CommaText);
                WriteIniData(FileName, GroupName, "10787 SpecBinBySiteComparePercent",       sSpecBinBySiteComparePercent        [tag]->CommaText);
                WriteIniData(FileName, GroupName, "10788 SpecBinByArmPerSiteCompareEnable",  sSpecBinByArmPerSiteCompareEnable   [tag]->CommaText);
                WriteIniData(FileName, GroupName, "10789 SpecBinByArmPerSiteCompareIgnore",  sSpecBinByArmPerSiteCompareIgnore   [tag]->CommaText);
                WriteIniData(FileName, GroupName, "10790 SpecBinByArmPerSiteComparePercent", sSpecBinByArmPerSiteComparePercent  [tag]->CommaText);
#endif
            }

#if 0
            // GATE (write-policy)
            WriteIniData(FileName, GroupName, "BinTrayLinked(10798)", sBinTrayLinked        [tag]->CommaText);
            WriteIniData(FileName, GroupName, "BinLinked(10799)",     sBinLinked            [tag]->CommaText);
            WriteIniData(FileName, GroupName, "MagazineSetup(10800)", sMagazineSetup        [tag]->CommaText);

            WriteIniData(FileName, GroupName, "AOIBinTraySetting", sAOIBinTraySetting        [tag]->CommaText);  //Eastsun 20260316 : AOI 要分bin
#endif
        }
    }
    else if(tag==eBinFT)
    {
        GroupName="Bin Func FT";
#if 0
        // GATE (write-policy)
        WriteIniData(FileName, GroupName, "NewBinFormat",               1); //kevin 20160928 使用新的bin format
        WriteIniData(FileName, GroupName, "3617 BinTraySetting",        sBinTraySetT3Pos        [tag]->CommaText);
#endif
        TransferBinTrayStrToName(tag);
#if 0
        // GATE (write-policy)
        WriteIniData(FileName, GroupName, "3617 BinTraySetName",        sBinTraySetT3PosName    [tag]->CommaText);
        WriteIniData(FileName, GroupName, "3636 BinDoubleContac",       sBinDoubleContact       [tag]->CommaText);
        WriteIniData(FileName, GroupName, "3656 BinType",               sBinType                [tag]->CommaText);
        WriteIniData(FileName, GroupName, "3676 BinConsFail",           sBinConsFail            [tag]->CommaText);
        WriteIniData(FileName, GroupName, "10641 BinEnableFail",        sBinEnableFail          [tag]->CommaText);
        WriteIniData(FileName, GroupName, "10642 BinFailPercent",       sBinFailPercent         [tag]->CommaText);
        WriteIniData(FileName, GroupName, "10643 BinFailIgnore",        sBinFailIgnore          [tag]->CommaText);
        WriteIniData(FileName, GroupName, "10644 BinCountEnable",       sBinCountEnable         [tag]->CommaText);
        WriteIniData(FileName, GroupName, "10645 BinCountNumber",       sBinCountNumber         [tag]->CommaText);
        WriteIniData(FileName, GroupName, "10646 BinCountIgnore",       sBinCountIgnore         [tag]->CommaText);
        WriteIniData(FileName, GroupName, "10647 SpecialBinByArm",      sSpecialBinByArm        [tag]->CommaText);
        WriteIniData(FileName, GroupName, "10648 SpecialBinCountByArm", sSpecialBinCountByArm   [tag]->CommaText);
        WriteIniData(FileName, GroupName, "10649 SpecialBinBySocket",   sSpecialBinBySocket     [tag]->CommaText);
        WriteIniData(FileName, GroupName, "10650 SpecialBinCountBySocket", sSpecialBinCountBySocket[tag]->CommaText);
        WriteIniData(FileName, GroupName, "10680 LowYield",             sLowYield               [tag]->CommaText);
        WriteIniData(FileName, GroupName, "10681 ArmYield",             sArmYield               [tag]->CommaText);
        WriteIniData(FileName, GroupName, "10682 SiteYield",            sSiteYield              [tag]->CommaText);
        WriteIniData(FileName, GroupName, "10683 BySiteClean",          sBySiteClean            [tag]->CommaText);
        WriteIniData(FileName, GroupName, "10684 ByBinClean",           sByBinClean             [tag]->CommaText);
#endif

        if(CosFunction.bBySiteByBinPercentCompare==true &&                      //JerryYang 20170712 (Steven) by site by bin compare percent
           CosFunction.bByBinAlarmFromYieldForm==false)                         //jou 20180113 (Steven) : By Site By Bin Percent Compare From Yield form
        {
#if 0
            // GATE (write-policy)
            WriteIniData(FileName, GroupName, "10685 SpecBinBySiteCompareEnable",        sSpecBinBySiteCompareEnable         [tag]->CommaText);
            WriteIniData(FileName, GroupName, "10686 SpecBinBySiteCompareIgnore",        sSpecBinBySiteCompareIgnore         [tag]->CommaText);
            WriteIniData(FileName, GroupName, "10687 SpecBinBySiteComparePercent",       sSpecBinBySiteComparePercent        [tag]->CommaText);
            WriteIniData(FileName, GroupName, "10688 SpecBinByArmPerSiteCompareEnable",  sSpecBinByArmPerSiteCompareEnable   [tag]->CommaText);
            WriteIniData(FileName, GroupName, "10689 SpecBinByArmPerSiteCompareIgnore",  sSpecBinByArmPerSiteCompareIgnore   [tag]->CommaText);
            WriteIniData(FileName, GroupName, "10690 SpecBinByArmPerSiteComparePercent", sSpecBinByArmPerSiteComparePercent  [tag]->CommaText);
#endif
        }

#if 0
        // GATE (write-policy)
        WriteIniData(FileName, GroupName, "BinTrayLinked(10698)", sBinTrayLinked        [tag]->CommaText);
        WriteIniData(FileName, GroupName, "BinLinked(10699)",     sBinLinked            [tag]->CommaText);
        WriteIniData(FileName, GroupName, "MagazineSetup(10700)", sMagazineSetup        [tag]->CommaText);

        WriteIniData(FileName, GroupName,"AOIBinTraySetting", sAOIBinTraySetting    [tag]->CommaText);  //Eastsun 20260316 : AOI 要分bin
#endif
    }
    else if(tag==eBinOffLine)
    {
        GroupName="Bin Func OffLine";
#if 0
        // GATE (write-policy)
        WriteIniData(FileName, GroupName, "NewBinFormat",               1); //kevin 20160928 使用新的bin format
        WriteIniData(FileName, GroupName, "3722 BinTraySetting",        sBinTraySetT3Pos        [tag]->CommaText);
#endif
        TransferBinTrayStrToName(tag);
#if 0
        // GATE (write-policy)
        WriteIniData(FileName, GroupName, "3722 BinTraySetName",        sBinTraySetT3PosName    [tag]->CommaText);
        WriteIniData(FileName, GroupName, "3723 BinDoubleContac",       sBinDoubleContact       [tag]->CommaText);
        WriteIniData(FileName, GroupName, "3724 BinType",               sBinType                [tag]->CommaText);
        WriteIniData(FileName, GroupName, "3725 BinConsFail",           sBinConsFail            [tag]->CommaText);
        WriteIniData(FileName, GroupName, "10941 BinEnableFail",        sBinEnableFail          [tag]->CommaText);
        WriteIniData(FileName, GroupName, "10942 BinFailPercent",       sBinFailPercent         [tag]->CommaText);
        WriteIniData(FileName, GroupName, "10943 BinFailIgnore",        sBinFailIgnore          [tag]->CommaText);
        WriteIniData(FileName, GroupName, "10944 BinCountEnable",       sBinCountEnable         [tag]->CommaText);
        WriteIniData(FileName, GroupName, "10945 BinCountNumber",       sBinCountNumber         [tag]->CommaText);
        WriteIniData(FileName, GroupName, "10946 BinCountIgnore",       sBinCountIgnore         [tag]->CommaText);
        WriteIniData(FileName, GroupName, "10947 SpecialBinByArm",      sSpecialBinByArm        [tag]->CommaText);
        WriteIniData(FileName, GroupName, "10948 SpecialBinCountByArm", sSpecialBinCountByArm   [tag]->CommaText);
        WriteIniData(FileName, GroupName, "10949 SpecialBinBySocket",   sSpecialBinBySocket     [tag]->CommaText);
        WriteIniData(FileName, GroupName, "10950 SpecialBinCountBySocket", sSpecialBinCountBySocket[tag]->CommaText);
        WriteIniData(FileName, GroupName, "10980 LowYield",             sLowYield               [tag]->CommaText);
        WriteIniData(FileName, GroupName, "10981 ArmYield",             sArmYield               [tag]->CommaText);
        WriteIniData(FileName, GroupName, "10982 SiteYield",            sSiteYield              [tag]->CommaText);
        WriteIniData(FileName, GroupName, "10983 BySiteClean",          sBySiteClean            [tag]->CommaText);
        WriteIniData(FileName, GroupName, "10984 ByBinClean",           sByBinClean             [tag]->CommaText);
#endif

        if(CosFunction.bBySiteByBinPercentCompare==true &&                      //JerryYang 20170712 (Steven) by site by bin compare percent
           CosFunction.bByBinAlarmFromYieldForm==false)                         //jou 20180113 (Steven) : By Site By Bin Percent Compare From Yield form
        {
#if 0
            // GATE (write-policy)
            WriteIniData(FileName, GroupName, "10985 SpecBinBySiteCompareEnable",        sSpecBinBySiteCompareEnable         [tag]->CommaText);
            WriteIniData(FileName, GroupName, "10986 SpecBinBySiteCompareIgnore",        sSpecBinBySiteCompareIgnore         [tag]->CommaText);
            WriteIniData(FileName, GroupName, "10987 SpecBinBySiteComparePercent",       sSpecBinBySiteComparePercent        [tag]->CommaText);
            WriteIniData(FileName, GroupName, "10988 SpecBinByArmPerSiteCompareEnable",  sSpecBinByArmPerSiteCompareEnable   [tag]->CommaText);
            WriteIniData(FileName, GroupName, "10989 SpecBinByArmPerSiteCompareIgnore",  sSpecBinByArmPerSiteCompareIgnore   [tag]->CommaText);
            WriteIniData(FileName, GroupName, "10990 SpecBinByArmPerSiteComparePercent", sSpecBinByArmPerSiteComparePercent  [tag]->CommaText);
#endif
        }

#if 0
        // GATE (write-policy)
        WriteIniData(FileName, GroupName, "BinTrayLinked(10998)", sBinTrayLinked        [tag]->CommaText);
        WriteIniData(FileName, GroupName, "BinLinked(10999)",     sBinLinked            [tag]->CommaText);
        WriteIniData(FileName, GroupName, "MagazineSetup(11000)", sMagazineSetup        [tag]->CommaText);

        WriteIniData(FileName, GroupName,"AOIBinTraySetting", sAOIBinTraySetting    [tag]->CommaText);  //Eastsun 20260316 : AOI 要分bin
#endif
    }
    else if(tag==eBinFT_ART)
    {
        GroupName="Bin Func ART_FT";                                            //wei 20161124 修改eBinFT_ART存成Bin Func ART_RT問題
#if 0
        // GATE (write-policy)
        WriteIniData(FileName, GroupName, "NewBinFormat",               1); //kevin 20160928 使用新的bin format
        WriteIniData(FileName, GroupName, "3801 BinTraySetting",        sBinTraySetT3Pos        [tag]->CommaText);
#endif
        TransferBinTrayStrToName(tag);
#if 0
        // GATE (write-policy)
        WriteIniData(FileName, GroupName, "3801 BinTraySetName",        sBinTraySetT3PosName    [tag]->CommaText);
        WriteIniData(FileName, GroupName, "3802 BinDoubleContac",       sBinDoubleContact       [tag]->CommaText);
        WriteIniData(FileName, GroupName, "3803 BinType",               sBinType                [tag]->CommaText);
        WriteIniData(FileName, GroupName, "3804 BinConsFail",           sBinConsFail            [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11041 BinEnableFail",        sBinEnableFail          [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11042 BinFailPercent",       sBinFailPercent         [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11043 BinFailIgnore",        sBinFailIgnore          [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11044 BinCountEnable",       sBinCountEnable         [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11045 BinCountNumber",       sBinCountNumber         [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11046 BinCountIgnore",       sBinCountIgnore         [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11047 SpecialBinByArm",      sSpecialBinByArm        [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11048 SpecialBinCountByArm", sSpecialBinCountByArm   [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11049 SpecialBinBySocket",   sSpecialBinBySocket     [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11050 SpecialBinCountBySocket", sSpecialBinCountBySocket[tag]->CommaText);
        WriteIniData(FileName, GroupName, "11080 LowYield",             sLowYield               [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11081 ArmYield",             sArmYield               [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11082 SiteYield",            sSiteYield              [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11083 BySiteClean",          sBySiteClean            [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11084 ByBinClean",           sByBinClean             [tag]->CommaText);
#endif

        if(CosFunction.bBySiteByBinPercentCompare==true &&                      //JerryYang 20170712 (Steven) by site by bin compare percent
           CosFunction.bByBinAlarmFromYieldForm==false)                         //jou 20180113 (Steven) : By Site By Bin Percent Compare From Yield form
        {
#if 0
            // GATE (write-policy)
            WriteIniData(FileName, GroupName, "11085 SpecBinBySiteCompareEnable",        sSpecBinBySiteCompareEnable         [tag]->CommaText);
            WriteIniData(FileName, GroupName, "11086 SpecBinBySiteCompareIgnore",        sSpecBinBySiteCompareIgnore         [tag]->CommaText);
            WriteIniData(FileName, GroupName, "11087 SpecBinBySiteComparePercent",       sSpecBinBySiteComparePercent        [tag]->CommaText);
            WriteIniData(FileName, GroupName, "11088 SpecBinByArmPerSiteCompareEnable",  sSpecBinByArmPerSiteCompareEnable   [tag]->CommaText);
            WriteIniData(FileName, GroupName, "11089 SpecBinByArmPerSiteCompareIgnore",  sSpecBinByArmPerSiteCompareIgnore   [tag]->CommaText);
            WriteIniData(FileName, GroupName, "11090 SpecBinByArmPerSiteComparePercent", sSpecBinByArmPerSiteComparePercent  [tag]->CommaText);
#endif
        }

#if 0
        // GATE (write-policy)
        WriteIniData(FileName, GroupName, "BinTrayLinked(11098)", sBinTrayLinked        [tag]->CommaText);
        WriteIniData(FileName, GroupName, "BinLinked(11099)",     sBinLinked            [tag]->CommaText);
        WriteIniData(FileName, GroupName, "MagazineSetup(11100)", sMagazineSetup        [tag]->CommaText);

        WriteIniData(FileName, GroupName,"AOIBinTraySetting", sAOIBinTraySetting    [tag]->CommaText);  //Eastsun 20260316 : AOI 要分bin
#endif
    }
    else if(tag==eBinRT_ART)
    {
        GroupName="Bin Func ART_RT";                                            //wei 20161124 修改eBinRT_ART存成Bin Func ART_FT問題
#if 0
        // GATE (write-policy)
        WriteIniData(FileName, GroupName, "NewBinFormat",               1); //kevin 20160928 使用新的bin format
        WriteIniData(FileName, GroupName, "3901 BinTraySetting",        sBinTraySetT3Pos        [tag]->CommaText);
#endif
        TransferBinTrayStrToName(tag);
#if 0
        // GATE (write-policy)
        WriteIniData(FileName, GroupName, "3901 BinTraySetName",        sBinTraySetT3PosName    [tag]->CommaText);
        WriteIniData(FileName, GroupName, "3902 BinDoubleContac",       sBinDoubleContact       [tag]->CommaText);
        WriteIniData(FileName, GroupName, "3903 BinType",               sBinType                [tag]->CommaText);
        WriteIniData(FileName, GroupName, "3904 BinConsFail",           sBinConsFail            [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11141 BinEnableFail",        sBinEnableFail          [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11142 BinFailPercent",       sBinFailPercent         [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11143 BinFailIgnore",        sBinFailIgnore          [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11144 BinCountEnable",       sBinCountEnable         [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11145 BinCountNumber",       sBinCountNumber         [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11146 BinCountIgnore",       sBinCountIgnore         [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11147 SpecialBinByArm",      sSpecialBinByArm        [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11148 SpecialBinCountByArm", sSpecialBinCountByArm   [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11149 SpecialBinBySocket",   sSpecialBinBySocket     [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11150 SpecialBinCountBySocket", sSpecialBinCountBySocket[tag]->CommaText);
        WriteIniData(FileName, GroupName, "11180 LowYield",             sLowYield               [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11181 ArmYield",             sArmYield               [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11182 SiteYield",            sSiteYield              [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11183 BySiteClean",          sBySiteClean            [tag]->CommaText);   //wei 20161124 11083 --> 11183
        WriteIniData(FileName, GroupName, "11184 ByBinClean",           sByBinClean             [tag]->CommaText);   //wei 20161124 11084 --> 11184

        WriteIniData(FileName, GroupName, "BinTrayLinked(11198)", sBinTrayLinked        [tag]->CommaText);
        WriteIniData(FileName, GroupName, "BinLinked(11199)",     sBinLinked            [tag]->CommaText);
        WriteIniData(FileName, GroupName, "MagazineSetup(11200)", sMagazineSetup        [tag]->CommaText);

        WriteIniData(FileName, GroupName,"AOIBinTraySetting", sAOIBinTraySetting    [tag]->CommaText);  //Eastsun 20260316 : AOI 要分bin
#endif
    }
    else if(tag==eBinFT_MRT)
    {
        GroupName="Bin Func MRT_FT";                                            //Ifor 20170316 (wei) add KYEC MRT Mode
#if 0
        // GATE (write-policy)
        WriteIniData(FileName, GroupName, "NewBinFormat",               1);
        WriteIniData(FileName, GroupName, "4001 BinTraySetting",        sBinTraySetT3Pos        [tag]->CommaText);
#endif
        TransferBinTrayStrToName(tag);
#if 0
        // GATE (write-policy)
        WriteIniData(FileName, GroupName, "4001 BinTraySetName",        sBinTraySetT3PosName    [tag]->CommaText);
        WriteIniData(FileName, GroupName, "4002 BinDoubleContac",       sBinDoubleContact       [tag]->CommaText);
        WriteIniData(FileName, GroupName, "4003 BinType",               sBinType                [tag]->CommaText);
        WriteIniData(FileName, GroupName, "4004 BinConsFail",           sBinConsFail            [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11241 BinEnableFail",        sBinEnableFail          [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11242 BinFailPercent",       sBinFailPercent         [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11243 BinFailIgnore",        sBinFailIgnore          [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11244 BinCountEnable",       sBinCountEnable         [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11245 BinCountNumber",       sBinCountNumber         [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11246 BinCountIgnore",       sBinCountIgnore         [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11247 SpecialBinByArm",      sSpecialBinByArm        [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11248 SpecialBinCountByArm", sSpecialBinCountByArm   [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11249 SpecialBinBySocket",   sSpecialBinBySocket     [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11250 SpecialBinCountBySocket", sSpecialBinCountBySocket[tag]->CommaText);
        WriteIniData(FileName, GroupName, "11280 LowYield",             sLowYield               [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11281 ArmYield",             sArmYield               [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11282 SiteYield",            sSiteYield              [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11283 BySiteClean",          sBySiteClean            [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11284 ByBinClean",           sByBinClean             [tag]->CommaText);
#endif

        if(CosFunction.bBySiteByBinPercentCompare==true &&                      //JerryYang 20170712 (Steven) by site by bin compare percent
           CosFunction.bByBinAlarmFromYieldForm==false)                         //jou 20180113 (Steven) : By Site By Bin Percent Compare From Yield form
        {
#if 0
            // GATE (write-policy)
            WriteIniData(FileName, GroupName, "11285 SpecBinBySiteCompareEnable",        sSpecBinBySiteCompareEnable         [tag]->CommaText);
            WriteIniData(FileName, GroupName, "11286 SpecBinBySiteCompareIgnore",        sSpecBinBySiteCompareIgnore         [tag]->CommaText);
            WriteIniData(FileName, GroupName, "11287 SpecBinBySiteComparePercent",       sSpecBinBySiteComparePercent        [tag]->CommaText);
            WriteIniData(FileName, GroupName, "11288 SpecBinByArmPerSiteCompareEnable",  sSpecBinByArmPerSiteCompareEnable   [tag]->CommaText);
            WriteIniData(FileName, GroupName, "11289 SpecBinByArmPerSiteCompareIgnore",  sSpecBinByArmPerSiteCompareIgnore   [tag]->CommaText);
            WriteIniData(FileName, GroupName, "11290 SpecBinByArmPerSiteComparePercent", sSpecBinByArmPerSiteComparePercent  [tag]->CommaText);
#endif
        }

#if 0
        // GATE (write-policy)
        WriteIniData(FileName, GroupName, "BinTrayLinked(11298)", sBinTrayLinked        [tag]->CommaText);
        WriteIniData(FileName, GroupName, "BinLinked(11299)",     sBinLinked            [tag]->CommaText);
        WriteIniData(FileName, GroupName, "MagazineSetup(11300)", sMagazineSetup        [tag]->CommaText);

        WriteIniData(FileName, GroupName,"AOIBinTraySetting", sAOIBinTraySetting    [tag]->CommaText);  //Eastsun 20260316 : AOI 要分bin
#endif
    }
    else if(tag==eBinRT_MRT)
    {
        GroupName="Bin Func MRT_RT";
#if 0
        // GATE (write-policy)
        WriteIniData(FileName, GroupName, "NewBinFormat",               1);
        WriteIniData(FileName, GroupName, "4101 BinTraySetting",        sBinTraySetT3Pos        [tag]->CommaText);
#endif
        TransferBinTrayStrToName(tag);
#if 0
        // GATE (write-policy)
        WriteIniData(FileName, GroupName, "4101 BinTraySetName",        sBinTraySetT3PosName    [tag]->CommaText);
        WriteIniData(FileName, GroupName, "4102 BinDoubleContac",       sBinDoubleContact       [tag]->CommaText);
        WriteIniData(FileName, GroupName, "4103 BinType",               sBinType                [tag]->CommaText);
        WriteIniData(FileName, GroupName, "4104 BinConsFail",           sBinConsFail            [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11341 BinEnableFail",        sBinEnableFail          [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11342 BinFailPercent",       sBinFailPercent         [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11343 BinFailIgnore",        sBinFailIgnore          [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11344 BinCountEnable",       sBinCountEnable         [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11345 BinCountNumber",       sBinCountNumber         [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11346 BinCountIgnore",       sBinCountIgnore         [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11347 SpecialBinByArm",      sSpecialBinByArm        [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11348 SpecialBinCountByArm", sSpecialBinCountByArm   [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11349 SpecialBinBySocket",   sSpecialBinBySocket     [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11350 SpecialBinCountBySocket", sSpecialBinCountBySocket[tag]->CommaText);
        WriteIniData(FileName, GroupName, "11380 LowYield",             sLowYield               [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11381 ArmYield",             sArmYield               [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11382 SiteYield",            sSiteYield              [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11383 BySiteClean",          sBySiteClean            [tag]->CommaText);
        WriteIniData(FileName, GroupName, "11384 ByBinClean",           sByBinClean             [tag]->CommaText);
#endif

        if(CosFunction.bBySiteByBinPercentCompare==true &&                      //JerryYang 20170712 (Steven) by site by bin compare percent
           CosFunction.bByBinAlarmFromYieldForm==false)                         //jou 20180113 (Steven) : By Site By Bin Percent Compare From Yield form
        {
#if 0
            // GATE (write-policy)
            WriteIniData(FileName, GroupName, "11385 SpecBinBySiteCompareEnable",        sSpecBinBySiteCompareEnable         [tag]->CommaText);
            WriteIniData(FileName, GroupName, "11386 SpecBinBySiteCompareIgnore",        sSpecBinBySiteCompareIgnore         [tag]->CommaText);
            WriteIniData(FileName, GroupName, "11387 SpecBinBySiteComparePercent",       sSpecBinBySiteComparePercent        [tag]->CommaText);
            WriteIniData(FileName, GroupName, "11388 SpecBinByArmPerSiteCompareEnable",  sSpecBinByArmPerSiteCompareEnable   [tag]->CommaText);
            WriteIniData(FileName, GroupName, "11389 SpecBinByArmPerSiteCompareIgnore",  sSpecBinByArmPerSiteCompareIgnore   [tag]->CommaText);
            WriteIniData(FileName, GroupName, "11390 SpecBinByArmPerSiteComparePercent", sSpecBinByArmPerSiteComparePercent  [tag]->CommaText);
#endif
        }

#if 0
        // GATE (write-policy)
        WriteIniData(FileName, GroupName, "BinTrayLinked(11398)", sBinTrayLinked        [tag]->CommaText);
        WriteIniData(FileName, GroupName, "BinLinked(11399)",     sBinLinked            [tag]->CommaText);
        WriteIniData(FileName, GroupName, "MagazineSetup(11400)", sMagazineSetup        [tag]->CommaText);

        WriteIniData(FileName, GroupName,"AOIBinTraySetting", sAOIBinTraySetting    [tag]->CommaText);  //Eastsun 20260316 : AOI 要分bin
#endif
    }
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  SetPrimeButton -- golden :6010-6042 (33 lines). FW-BinSel-WB (this wave).
//  GATE: entire body -- "純 widget" per this wave's own brief, confirmed by
//  grep: `spbNormal`/`spbPrime`/`tsRetest` are absent from TfBinSel's facade
//  (`grep -n "spbNormal\|spbPrime\|tsRetest" forms/fBinSel.h` -- only
//  comment hits, 20260819) and `fMain->SetNormalOrPrime()` is absent
//  (`grep -n "SetNormalOrPrime" forms/fMain.h` -- 0 hits, 20260819).
//  `cbTestMode->Enabled` (the one statement whose target DOES exist on the
//  facade) is left gated alongside its sibling widgets rather than partially
//  wired: toggling Enabled without the paired spbNormal/spbPrime
//  Visible/Down state would show an incomplete, possibly-misleading UI
//  state, not golden's real behaviour.
//---------------------------------------------------------------------------
void TfBinSel::SetPrimeButton()
{
#if 0
    if(IniConfig.bFTBin2RTBin && IniConfig.bA02BinModelPrime)                   //ChungHung 20120912 add Bin Setting like Epson
    {
        spbNormal->Visible=true;
        spbPrime->Visible=true;

        if(iBinModelPrime==1)
        {
            tsRetest->Enabled=true;
            spbPrime->Down=true;
            cbTestMode->Enabled=true;
        }
        else
        {
            tsRetest->Enabled=false;
            spbNormal->Down=true;
            cbTestMode->Enabled=false;
        }
    }
    else
    {
        if(IniConfig.bFTBin2RTBin && IniConfig.bA02BinModelPrime==false)        //jou 2014-08-26 修正 BIN select FT/RT 顯示異常
        {
            tsRetest->Enabled=false;
            cbTestMode->Enabled=false;
        }

        spbNormal->Visible=false;
        spbPrime->Visible=false;
    }
    fMain->SetNormalOrPrime();                                                  //Steven 20160817 : 必須在讀完Bin別設定之後
#endif
}

//---------------------------------------------------------------------------
//  mtTrayNameSetColor -- golden :4033-4140 (108 lines). FW-BinSel-WB (this
//  wave). GATE: entire body.
//
//  CORRECTED ABSENCE CLAIM (re-verify-before-relying-on rule, see
//  KNOWLEDGE.md "wave-agents-stale-absence-claims"): WAVE A's own STUB
//  COLLISION SCAN asserted `grep -rn "class TTMyTray256" vclcompat/` -- 0
//  hits (20260819) and concluded "no vclcompat stand-in exists anywhere in
//  the tree". That literal-string grep was correct but MISLEADING -- W7-C1
//  (20260728, BEFORE WAVE A's own banner date) already added
//  `vclcompat::Tray256Core`/`vclcompat::TrayCore` (vclcompat/TrayCore.h/.cpp,
//  linked into ht9045_sm per CMakeLists.txt:256) as this tree's RENAMED
//  stand-in for golden's TTMyTray256/TTMyTray (vclcompat/Controls.h:79's own
//  cross-reference: "TTMyTray / TTMyTray256 -> vclcompat/TrayCore.*
//  (W7-C1)"). Re-verified this wave: `grep -rln "Tray256Core" --include=*.h
//  --include=*.cpp .` -- 3 hits, 20260819 (TrayCore.h/.cpp +
//  test_traycore.cpp only) -- the TYPE exists, but no form anywhere in this
//  tree yet declares a `Tray256Core`/`TrayCore` MEMBER.
//
//  WHY STILL GATED (not wired up this wave): wiring this method alone would
//  need THREE additions this wave does not make -- (1) `TMyBinPanelData::
//  mtTrayName`/`mtTrayItem` (`Tray256Core`, per-tag widgets), (2) `TfBinSel::
//  mtTrayName`/`mtTrayItem` (`TrayCore`, golden cBinSel.h:73-74 -- a
//  SEPARATE, class-level pair; see (B16) below), AND (3) the ~90-line
//  cell-text population golden's OWN `TMyBinPanel::TMyBinPanel` ctor does for
//  these widgets (golden cBinSel.cpp :420-509, e.g. `mtTrayName->
//  SetCellNumber(eItemLink,eBinNo,"Link")`) -- which WAVE A's own CTOR NOTE
//  explicitly deferred ("widget wiring dropped"). Wiring only THIS method's
//  body without also landing (3) would leave eItemART/eItemCateR/eItemPass's
//  cell TEXT blank forever (set by the ctor loop, not by this method, for
//  the per-tag `MyBinPanel[tag]->mtTrayName` widget) while COLOUR renders
//  fine -- a half-populated grid that LOOKS wired but silently shows wrong
//  content is worse than a documented gate. (1)+(2)+(3) together are a
//  larger, separate unit of work than this wave's 4-method scope; flagged
//  here with exact golden line spans so a future wave does not have to
//  rediscover this.
//
//  (B16) GOLDEN QUIRK: the two calls at golden :4094/:4106
//  (`mtTrayName->SetCellNumber(eItemART,j,"Retest")` / `mtTrayName->
//  SetCellNumber(eItemCateR,j,"CateR")`) omit the `MyBinPanel[tag]->` prefix
//  every sibling call in this SAME method has. Confirmed against golden
//  cBinSel.h:73-74: `TfBinSel` has its OWN `TTMyTray *mtTrayName`/
//  `*mtTrayItem` (a DIFFERENT, class-level, tag-invariant pair -- NOT
//  `MyBinPanel[tag]`'s per-tag one) -- so these two calls write the
//  (tag-invariant) label text "Retest"/"CateR" onto that shared widget
//  instead of the per-tag one that every other line in this method targets.
//  Preserved verbatim (not "fixed" -- see project translation policy); a
//  separate finding from the ReadFunctionData (B14)/(B15) AOI-key bugs above.
//---------------------------------------------------------------------------
void TfBinSel::mtTrayNameSetColor(int tag)
{
    (void)tag;   // only feeds the entirely-gated body below in this wave
#if 0
    int iT6, iT6Pos;
    for(int j=eBinSetting; j<eBinSetTotal; j++)
    {
        if(j>=MyBinPanel[tag]->mtTrayName->YItem)
            continue;

        iT6   =j-eBinSetting;
        iT6Pos=j-eBinNotUse;

        if(Prod.iTrayType[iT6]==tNotUse)
        {
            MyBinPanel[tag]->mtTrayItem->SetCellNumber    (eItemError,  j, "");
            MyBinPanel[tag]->mtTrayItem->SetCellColorIndex(eItemError,  j, eCLBtnFace);
            MyBinPanel[tag]->mtTrayName->SetCellNumber    (eItemPass,   j, "");
            MyBinPanel[tag]->mtTrayName->SetCellColorIndex(eItemPass,   j, eCLBtnFace);
            MyBinPanel[tag]->mtTrayName->SetCellNumber    (eItemART,    j, "");
            MyBinPanel[tag]->mtTrayName->SetCellColorIndex(eItemART,    j, eCLBtnFace);
            MyBinPanel[tag]->mtTrayName->SetCellNumber    (eItemCateR,  j, "");
            MyBinPanel[tag]->mtTrayName->SetCellColorIndex(eItemCateR,  j, eCLBtnFace);
            MyBinPanel[tag]->mtTrayName->SetCellNumber    (eItemLink,   j, "");
            MyBinPanel[tag]->mtTrayName->SetCellColorIndex(eItemLink,   j, eCLBtnFace);
        }
        else
        {
            //Error----------------
            if(MyBinPanel[tag]->iErrorT6==iT6)                                      //Error     //JerryYang 20230926
            {
                MyBinPanel[tag]->mtTrayName->SetCellNumber    (eItemError, j, "Error");
                MyBinPanel[tag]->mtTrayName->SetCellColorIndex(eItemError, j, eCLRed);
            }
            else
            {
                MyBinPanel[tag]->mtTrayName->SetCellColorIndex(eItemError, j, eCLWhite);
            }

            //Pass/Fail------------
            if(TestIF_File.bEnableQASampling &&                                     //Steven 20190326 : QA Sampling
               iT6Pos==TestIF_File.iQASamplingT3Pos)
            {
                MyBinPanel[tag]->mtTrayName->SetCellNumber    (eItemPass, j, "QA");
                MyBinPanel[tag]->mtTrayName->SetCellColorIndex(eItemPass, j, eCLBlue);
            }
            else if(MyBinPanel[tag]->iT6IsFail[iT6]>0)                              //0=Pass / 1=Fail
            {
                MyBinPanel[tag]->mtTrayName->SetCellNumber    (eItemPass, j, "Failed");
                MyBinPanel[tag]->mtTrayName->SetCellColorIndex(eItemPass, j, MyBinPanel[tag]->iT6IsFail[j-eBinSetting]+1);  //Steven 20240702 : fixed for bin color
            }
            else
            {
                MyBinPanel[tag]->mtTrayName->SetCellNumber    (eItemPass, j, "Pass");
                MyBinPanel[tag]->mtTrayName->SetCellColorIndex(eItemPass, j, eCLGreen);
            }

            //ART------------------
            if(USE_AUTO_RETEST==eartInstall ||                                  //ChungHung 20140317 add Auto Retest
               IniConfig.bA38_SLT_Summary)                                      //JerryYang 20220923 : add for SLT lot summary
            {
                if(iT6<=iAutoRight)
                {
                    mtTrayName->SetCellNumber(eItemART, j, "Retest");
                    if(UNLOADER_ART[iT6]==eartInstall &&                        //Steven 20161221 (wei) : for SCK only Auto 2 has ART
                       MyBinPanel[tag]->bT6ART[iT6])
                    {
                        MyBinPanel[tag]->mtTrayName->SetCellColorIndex(eItemART, j, eCLRed);
                    }
                    else
                    {
                        MyBinPanel[tag]->mtTrayName->SetCellColorIndex(eItemART, j, eCLWhite);
                    }
                }

                mtTrayName->SetCellNumber(eItemCateR, j, "CateR");
                if(CosFunction.bUseSCKART &&
                   (TestIF_File.iSCKART_SortMode==1 ||                          //Steven 20161221 (wei) : Cate R for ART
                    IniConfig.bA38_SLT_Summary) &&                              //JerryYang 20220923 : add for SLT lot summary
                    MyBinPanel[tag]->bT6CateR[iT6])
                {
                    MyBinPanel[tag]->mtTrayName->SetCellColorIndex(eItemCateR, j, eCLRed);
                }
                else
                {
                    MyBinPanel[tag]->mtTrayName->SetCellColorIndex(eItemCateR, j, eCLWhite);
                }
            }
            else
            {
                MyBinPanel[tag]->mtTrayName->SetCellColorIndex(eItemART,   j, eCLWhite);
                MyBinPanel[tag]->mtTrayName->SetCellColorIndex(eItemCateR, j, eCLWhite);
            }

            //Link-----------------
            if(MyBinPanel[tag]->bT6Link[iT6] && bCanLinkT6[iT6]==true)
            {
                MyBinPanel[tag]->mtTrayName->SetCellNumber    (eItemLink, j, "Linked");
                MyBinPanel[tag]->mtTrayName->SetCellColorIndex(eItemLink, j, eCLGreen);
            }
            else
            {
                MyBinPanel[tag]->mtTrayName->SetCellNumber    (eItemLink, j, "");
                MyBinPanel[tag]->mtTrayName->SetCellColorIndex(eItemLink, j, eCLWhite);
            }
        }
    }

    // GATE: InitDataToEdit(tag) -- not translated anywhere this wave
    // (`grep -rn "InitDataToEdit" --include=*.h --include=*.cpp .` -- only
    // comment hits, this file's own banner above and forms/fBinSel.h's WAVE
    // B QUEUE, 20260819; golden :4142-4784, 643 lines, its own Wave-B-queue
    // item, same Edit-widget-population shape as this method).
    InitDataToEdit(tag);
#endif
}
