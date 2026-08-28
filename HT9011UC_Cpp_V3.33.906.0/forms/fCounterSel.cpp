// =============================================================================
//  forms/fCounterSel.cpp  --  definitions for the fCounterSel facade
//
//  AI(W906-FW3-DTL1) 20260828: new file, FW wave FW3-DTL1.
//  GOLDEN SOURCE: HT9011UC_Code_V3.33.906.0_20260618/cCounterSel.cpp
//  (150 lines) + cCounterSel.h (62 lines), read with
//  `io.open(p, encoding='cp950')`.
//  SPAN: 5 golden `TfCounterSel::` member bodies, 123 span lines
//  (tools/census/wave_preflight.py, 20260828).
//
//  THIS WAVE, against the 5-member denominator:
//     2 ACTIVE           ctor / spbExitClick  -- 10 of 123 golden span lines
//     3 GATED-WITH-BODY  FormShow (26L) / FormClose (54L) / CheckFormIni
//                        (33L), 113 golden span lines carried as `#if 0`
//                        transcript below
//
//  See forms/fCounterSel.h for the full GATE REGISTER (C-1)/(C-2)/(C-3), the
//  ACTIVE evidence, DEVIATIONS D-1..D-5, the FIELD LIST (including the 27
//  widgets deliberately NOT declared) and the ZERO-WRITER note about `fShow`.
//
//  ⚠ EVERY `#if 0` BLOCK BELOW HAS NEVER BEEN COMPILED.  The text is golden's
//  own (the only edit is dropping `__fastcall` / the unused FormClose
//  parameters), so it is a faithful TRANSCRIPT -- not verified code.  Most of
//  the identifiers it names (cbUPH, cbIndexTime, rbLoadingCount_On,
//  rgTestCategory, fMain->StatusBar1, fShowMessage->lblTestTime, ...) are NOT
//  members of this facade and/or have no port; un-gating requires supplying
//  them first.
// =============================================================================
#include "forms/fCounterSel.h"

// AI(W906-FW3-DTL1) 20260828: TfCounterSel/fCounterSel were FREE tree-wide --
// same idiom as forms/fPrecaution.cpp:14 / forms/fHotPlate.cpp:24 /
// forms/fTesterTCP.cpp:49.  Golden's ctor body (:23-27) is a single own-field
// assignment, so this static-init `new` touches no global -- no SIOF risk
// (docs/KNOWLEDGE.md "static-init ctor 不可碰 NULL 全域"; the fLaserSensor
// incident that rule comes from turned 88 of 134 ctest binaries into
// SEGFAULTs).
TfCounterSel *fCounterSel = new TfCounterSel();

TfCounterSel::TfCounterSel()   // golden :23-27
{
    NeedRef=false;
}

// ---------------------------------------------------------------------------
void TfCounterSel::spbExitClick(TObject *Sender)   // golden :145-149
{
    (void)Sender;
    spbExit->Down=false;
    Close();   // DEVIATION D-3 -- port-only no-op
}

// ===========================================================================
//  GATE REGISTER -- translated golden bodies, deliberately NOT COMPILED.
//  See forms/fCounterSel.h for the per-entry reasoning.
// ===========================================================================

#if 0 // GATE (C-1) FormShow -- golden :29-54.  HIDDEN WRITE: golden :31
      // `ProcessLastSetIni_Visible(bReadFile)` (cprod.cpp:2824, REACHABLE --
      // ht9045_globals) funnels 11 keys through ReadWriteIni(bool)
      // (common.cpp:1599) -> CheckAndReadIniData (common.cpp:613), whose
      // :623-624 SEED `D:\HT9045\config\config.ini` with the default whenever
      // a key is absent.  Same function-pair trap as forms/fHotPlate.h (G-1).
      // The other 25 lines are pure IniConfig reflection and would have been
      // reachable.  NOT split -- dropping :31 changes what the form shows.
void TfCounterSel::FormShow(TObject *Sender)
{
    ProcessLastSetIni_Visible(bReadFile);
    cbUPH->Checked              = IniConfig.bShowUPH;
    cbIndexTime->Checked        = IniConfig.bShowIndexTime;
    cbContactHeight->Checked    = IniConfig.bShowContactHeight;   //Steven 20100827
    cbCycleTime->Checked        = IniConfig.bShowTimeInfo;        //Steven 20100827
    rbLoadingCount_On->Checked  = IniConfig.bShowLoaderCT;
    rbLoadingCount_Off->Checked =!IniConfig.bShowLoaderCT;
    rbContactCount_On->Checked  = IniConfig.bShowContactCT;
    rbContactCount_Off->Checked =!IniConfig.bShowContactCT;
    rbTestCategory_On->Checked  = IniConfig.bShowTestCate;
    rbTestCategory_Off->Checked =!IniConfig.bShowTestCate;
    rgTestCategory->ItemIndex   = IniConfig.iShowCateByArm;
    rbScanner_On->Checked       = IniConfig.bShowScanCate;
    rbScanner_Off->Checked      =!IniConfig.bShowScanCate;
    rbTemperature_On->Checked   = IniConfig.bShowTemper;
    rbTemperature_Off->Checked  =!IniConfig.bShowTemper;
    rbBinAssign_On->Checked     = IniConfig.bShowBinCT;
    rbBinAssign_Off->Checked    =!IniConfig.bShowBinCT;

    NeedRef=false;
    Top=20;
    Left=250;
    fShow=true;
}
#endif // GATE (C-1)

#if 0 // GATE (C-2) FormClose -- golden :56-109.  FOUR INDEPENDENT GATES:
      // (a) :106 ProcessLastSetIni_Visible(bWriteFile) -> 11 x WriteIniData
      //     into config\config.ini, unconditional.
      // (b) :91-103 CheckFormIni(...) (GATE C-3, creates dirs + seeds 24 keys
      //     into config\FormPos.def) then 12 x ReplaceIniData.
      // (c) :73/:81-82 fMain->StatusBar1 / ->lbArm0Torque and :74-75
      //     fShowMessage->lblIndexCycleTime / ->lblTestTime -- none of those
      //     four members exists on the real facades (see the header for the
      //     two near-miss grep traps).
      // (d) :77 fShowBinSelect->Tab_UPH DOES resolve -- not a blocker.
      // ⚠ :81-82 are IDENTICAL lines in golden.  Verbatim, not "fixed".
      // DEVIATION D-4: golden's (TObject*, TCloseAction&) parameters dropped.
void TfCounterSel::FormClose()
{
    IniConfig.bShowUPH          =cbUPH->Checked;
    IniConfig.bShowIndexTime    =cbIndexTime->Checked;
    IniConfig.bShowTimeInfo     =cbCycleTime->Checked;
    IniConfig.bShowContactHeight=cbContactHeight->Checked;
    IniConfig.bShowLoaderCT     =rbLoadingCount_On->Checked;
    IniConfig.bShowContactCT    =rbContactCount_On->Checked;
    IniConfig.bShowScanCate     =rbScanner_On->Checked;
    IniConfig.bShowTemper       =rbTemperature_On->Checked;
    IniConfig.bShowTestCate     =rbTestCategory_On->Checked;
    IniConfig.bShowBinCT        =rbBinAssign_On->Checked;
    IniConfig.iShowCateByArm    =rgTestCategory->ItemIndex;

    //Steven 20100827 Start
    if(IniConfig.bShowIndexTime==false)
        fMain->StatusBar1->Panels->Items[0]->Text="";
    fShowMessage->lblIndexCycleTime->Visible=IniConfig.bShowTimeInfo;
    fShowMessage->lblTestTime->Visible=IniConfig.bShowTimeInfo;

    fShowBinSelect->Tab_UPH->TabVisible=IniConfig.bShowUPH;

    if(IniConfig.bShowContactHeight==false)
    {
        fMain->lbArm0Torque->Caption="";
        fMain->lbArm0Torque->Caption="";
    }
    //Steven 20100827 End

    //jou 980721 start : Return Default Value
    if(cbDefaultValue->Checked)
    {
        //Steven 20090811 : Use INI file to define form position
        cbDefaultValue->Checked=false;
        AnsiString szDir=fCounterSel->CheckFormIni(AuthPath, "FormPos.def");
        ReplaceIniData(szDir, "fMain",            "X", "defaultX");
        ReplaceIniData(szDir, "fMain",            "Y", "defaultY");
        ReplaceIniData(szDir, "fSortCT",          "X", "defaultX");
        ReplaceIniData(szDir, "fSortCT",          "Y", "defaultY");
        ReplaceIniData(szDir, "fTestCategory",    "X", "defaultX");
        ReplaceIniData(szDir, "fTestCategory",    "Y", "defaultY");
        ReplaceIniData(szDir, "fShowBinSelect",   "X", "defaultX");
        ReplaceIniData(szDir, "fShowBinSelect",   "Y", "defaultY");
        ReplaceIniData(szDir, "fContactCT",       "X", "defaultX");
        ReplaceIniData(szDir, "fContactCT",       "Y", "defaultY");
        ReplaceIniData(szDir, "fLotInfo",         "X", "defaultX");
        ReplaceIniData(szDir, "fLotInfo",         "Y", "defaultY");
    }
    //jou 980721 end
    ProcessLastSetIni_Visible(bWriteFile);
    NeedRef=true;
    fShow=false;
}
#endif // GATE (C-2)

#if 0 // GATE (C-3) CheckFormIni -- golden :111-143.  WRITES DISK:
      // MyForceDirectories(szDir) CREATES DIRECTORIES (common.cpp:1888) and,
      // when the file is absent, 24 x WriteIniData(...) (common.cpp:958) seed
      // config\FormPos.def with the factory window layout.  Both helpers are
      // REACHABLE from ht9045_forms (common.cpp is ht9045_core) -- this is a
      // pure read-only-direction safety gate, not a link-boundary gate.
AnsiString TfCounterSel::CheckFormIni(AnsiString szDir, AnsiString str)
{
    MyForceDirectories(szDir);
    szDir+=str;
    if(!FileExists(szDir))
    {
        WriteIniData(szDir, "fMain",            "X",   0);
        WriteIniData(szDir, "fMain",            "Y",   0);
        WriteIniData(szDir, "fSortCT",          "X", 756);
        WriteIniData(szDir, "fSortCT",          "Y",   0);
        WriteIniData(szDir, "fTestCategory",    "X", 756);
        WriteIniData(szDir, "fTestCategory",    "Y", 284);
        WriteIniData(szDir, "fShowBinSelect",   "X", 756);
        WriteIniData(szDir, "fShowBinSelect",   "Y", 476);
        WriteIniData(szDir, "fContactCT",       "X", 463);
        WriteIniData(szDir, "fContactCT",       "Y", 554);
        WriteIniData(szDir, "fLotInfo",         "X", 201);
        WriteIniData(szDir, "fLotInfo",         "Y", 554);
        WriteIniData(szDir, "fMain",            "defaultX",   0);
        WriteIniData(szDir, "fMain",            "defaultY",   0);
        WriteIniData(szDir, "fSortCT",          "defaultX", 756);
        WriteIniData(szDir, "fSortCT",          "defaultY",   0);
        WriteIniData(szDir, "fTestCategory",    "defaultX", 756);
        WriteIniData(szDir, "fTestCategory",    "defaultY", 284);
        WriteIniData(szDir, "fShowBinSelect",   "defaultX", 756);
        WriteIniData(szDir, "fShowBinSelect",   "defaultY", 476);
        WriteIniData(szDir, "fContactCT",       "defaultX", 463);
        WriteIniData(szDir, "fContactCT",       "defaultY", 554);
        WriteIniData(szDir, "fLotInfo",         "defaultX", 201);
        WriteIniData(szDir, "fLotInfo",         "defaultY", 554);
    }
    return szDir;
}
#endif // GATE (C-3)
