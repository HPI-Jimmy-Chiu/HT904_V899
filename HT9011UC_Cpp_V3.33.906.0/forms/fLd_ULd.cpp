// =============================================================================
//  forms/fLd_ULd.cpp  --  definitions for the fLd_ULd facade
//
//  AI(W906-FW3-DTL1) 20260828: new file, FW wave FW3-DTL1.
//  GOLDEN SOURCE: HT9011UC_Code_V3.33.906.0_20260618/cLd_ULd.cpp (262 lines)
//  + cLd_ULd.h (102 lines), read with `io.open(p, encoding='cp950')`.
//  SPAN: 16 golden `TfLd_ULd::` member bodies, 222 span lines
//  (tools/census/wave_preflight.py, 20260828).
//
//  THIS WAVE, against the 16-member denominator:
//     7 ACTIVE           41 of 222 golden span lines (18.5%)
//     1 ACTIVE-PARTIAL   the ctor -- golden :25 live, :26-84 GATE (L-1)
//     7 GATED-WITH-BODY  110 golden span lines carried as `#if 0` transcript
//     1 OMITTED ENTIRELY FormShortCut -- `TWMKey` has no port, GATE (L-10)
//
//  See forms/fLd_ULd.h for the full GATE REGISTER (L-1)..(L-8)/(L-10), the
//  ACTIVE evidence, DEVIATIONS D-1..D-7, the FIELD LIST (including the ~50
//  widgets deliberately NOT declared) and the ZERO-WRITER note about `fShow`
//  and `LastFileName`.
//
//  ⚠ EVERY `#if 0` BLOCK BELOW HAS NEVER BEEN COMPILED.  The text is golden's
//  own (the only edits are dropping `__fastcall` and the unused FormClose
//  parameters), so it is a faithful TRANSCRIPT -- not verified code.  Most of
//  the identifiers it names (elUdUld, Ld_UldDelayTime, ECDouble/ECBool/
//  ECInteger, Image1/Image2, grpLoader, palKnocker, cbP13.., edP40_*, rbTemp,
//  spbSave, fSecurity, fBuilder, SECS_EVENT, ...) are NOT members of this
//  facade and/or have no reachable definition; un-gating requires supplying
//  them first.
// =============================================================================
#include "forms/fLd_ULd.h"
#include "common.h"             // OnlyNumberAndDotInPut (common.cpp, ht9045_core)
#include "cmydef.h"             // N_INTEGER (extern const int, cmydef.cpp, ht9045_globals)
#include "forms/fQwertyKey.h"   // fQwertyKey->ShowQwertyKey (forms/fQwertyKey.cpp, ht9045_forms)

// AI(W906-FW3-DTL1) 20260828: TfLd_ULd/fLd_ULd were FREE tree-wide -- same
// idiom as forms/fPrecaution.cpp:14 / forms/fHotPlate.cpp:24 /
// forms/fTesterTCP.cpp:49.  The ACTIVE part of golden's ctor is ONE own-field
// assignment, so this static-init `new` touches no global -- no SIOF risk.
// Golden's remaining 20 ctor statements dereference the NULL global
// `elUdUld` and are gated on that ground as well as on the ht9045_sm link
// boundary; see GATE (L-1)(a)/(b) and docs/KNOWLEDGE.md "static-init ctor
// 不可碰 NULL 全域" (the fLaserSensor incident that rule comes from turned 88
// of 134 ctest binaries into SEGFAULTs).
TfLd_ULd *fLd_ULd = new TfLd_ULd();

// ---------------------------------------------------------------------------
//  golden :22-85.  ACTIVE-PARTIAL: golden :25 is live below; golden :26-84
//  (three locals + 20 `elUdUld->Add(...)` registrations) is GATE (L-1) and
//  appears as transcript further down this file.
// ---------------------------------------------------------------------------
TfLd_ULd::TfLd_ULd()
{
    LastFileName="";
    // GATE (L-1) -- golden :26-84 (the elUdUld registration block), transcript
    // below.  Two independent gates: ht9045_sm link boundary + NULL global
    // dereference during static initialisation.
}

// ---------------------------------------------------------------------------
void TfLd_ULd::SetDefaultPos()   // golden :131-135, Steven 20240215 : 調整預設位置
{
    Left=150;
    Top =10;
}

// ---------------------------------------------------------------------------
void TfLd_ULd::edtLD_TrayArrivalDelyKeyPress(TObject *Sender, char &Key)   // golden :164-168
{
    (void)Sender;
    if(OnlyNumberAndDotInPut(Key)==false)
        Key=0;   // golden `Key=NULL;` on a char -- faithful as 0
}

// ---------------------------------------------------------------------------
void TfLd_ULd::sbtExitClick(TObject *Sender)   // golden :218-222
{
    (void)Sender;
    sbtExit->Down=false;
    Close();   // DEVIATION D-4 -- port-only no-op
}

// ---------------------------------------------------------------------------
//  golden :224-237.  ACTIVE.
//  ⚠ Golden resets only EIGHT of this form's nine loader/unloader edits --
//  `edtLD_BeforeDownDelay` (golden cLd_ULd.h:76) and `edtULD_TrayArrivalWait`
//  (:47) are NOT touched by "Default Value".  Verbatim, not "fixed"
//  (改行為要留給使用者決定).
// ---------------------------------------------------------------------------
void TfLd_ULd::btnDefaultValueClick(TObject *Sender)
{
    (void)Sender;
    //Loader
    edtLD_TrayArrivalDely->Text     =0.2;                                       //tray Arrival Lock (Sec)
    edtLD_FixTrayDely->Text         =0.5;                                       //Lock-Preparation is completed(Sec)    //jou 2014-05-29 load/unload tray lock wait define 0.2 -> 0.5 sec
    edtLD_MiddLockDelay->Text       =2;                                         //Lifter down to the middle-separator closed(Sec)
    edtLD_LiftDownDelay->Text       =1;                                         //Lifter down-Tray exist/non-exit check(Sec)

    //Unloader
    edtULD_TrayArrivalDelay->Text   =0.2;                                       //tray Arrival Lock (Sec)
    edtULD_FixTrayDely->Text        =0.5;                                       //Lock-Preparation is completed(Sec)    //jou 2014-05-29 load/unload tray lock wait define 0.2 -> 0.5 sec
    edtULD_TrayBackDelay->Text      =1;                                         //Lifter down to the middle-separator closed(Sec)  //JerryYang 20200408 有翻盤風險改最短1sec
    edtULD_LiftDownDelay->Text      =0.2;                                       //Lifter down-Tray exist/non-exit check(Sec)
}

// ---------------------------------------------------------------------------
//Eastsun 20260525 P40 splice (from KYEC AutoUP) - loader跛動馬達
void TfLd_ULd::edP40_1Click(TObject *Sender)   // golden :248-251
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 1, 10);  //JerryYang 20191001 loader跛動馬達
}

// ---------------------------------------------------------------------------
//  ⚠ golden really does bind edP40_2 to a DblClick handler while its two
//  neighbours use Click (golden cLd_ULd.h:84-86).  Verbatim.
void TfLd_ULd::edP40_2DblClick(TObject *Sender)   // golden :253-256
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 1, 20);  //JerryYang 20191001 loader跛動馬達
}

// ---------------------------------------------------------------------------
void TfLd_ULd::edP40_3Click(TObject *Sender)   // golden :258-261
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 1, 15);  //JerryYang 20191001 loader跛動馬達
}

// ===========================================================================
//  GATE REGISTER -- translated golden bodies, deliberately NOT COMPILED.
//  See forms/fLd_ULd.h for the per-entry reasoning.
// ===========================================================================

#if 0 // GATE (L-1) -- golden :26-84, the ctor's elUdUld registration block.
      // (a) LINK BOUNDARY: `elUdUld` is defined in Public/HTEditList.cpp:197,
      //     add_library(ht9045_sm); ht9045_forms links only vclcompat +
      //     ht9045_globals + ht9045_core.  `HTEditList::Add` is out-of-line.
      // (b) STATIC-INIT SAFETY: `elUdUld` is a NULL global before main(), and
      //     this facade is constructed by a file-scope `new`.  All 20 lines
      //     would be NULL dereferences during static initialisation.
{
    bool bShow=true; //bNoShow=false;
    bool bEnable=true;//, bDisable=false;
    bool bReadFromFile=true;//, bFixedValue=false;

    elUdUld->Add(edtLD_TrayArrivalDely,  &Ld_UldDelayTime.LD_TrayArrivalDely,           ECDouble,  "Loader Wait Time",    "Arrived",            bShow, bEnable, bReadFromFile, 0.2,      true,  10.0,      0.2);
    elUdUld->Add(edtLD_FixTrayDely,      &Ld_UldDelayTime.LD_FixTrayDely,               ECDouble,  "Loader Wait Time",    "Lock",               bShow, bEnable, bReadFromFile, 0.5,      true,  10.0,      0.2);
    elUdUld->Add(edtLD_MiddLockDelay,    &Ld_UldDelayTime.LD_StackMiddLockDelay,        ECDouble,  "Loader Wait Time",    "Close",              bShow, bEnable, bReadFromFile, 2.0,      true,  10.0,      0.2);
    elUdUld->Add(edtLD_BeforeDownDelay,  &Ld_UldDelayTime.LD_BeforeDownDelay,           ECDouble,  "Loader Wait Time",    "Down",               bShow, bEnable, bReadFromFile, 0.0,      true,  10.0,      0.0);    //Steven 20240215 : 下降前Delay
    elUdUld->Add(edtLD_LiftDownDelay,    &Ld_UldDelayTime.LD_LiftDownDelay,             ECDouble,  "Loader Wait Time",    "Check",              bShow, bEnable, bReadFromFile, 1.0,      true,  10.0,      0.2);

    elUdUld->Add(edtULD_TrayArrivalDelay,&Ld_UldDelayTime.ULD_TrayArrivalDelay,         ECDouble,  "UnLoader Wait Time",  "Unlock",             bShow, bEnable, bReadFromFile, 0.2,      true,  10.0,      0.2);
    elUdUld->Add(edtULD_FixTrayDely,     &Ld_UldDelayTime.ULD_FixTrayDely,              ECDouble,  "UnLoader Wait Time",  "Lock",               bShow, bEnable, bReadFromFile, 0.5,      true,  10.0,      0.2);
    if(TRAY_ARM_MODE==eUnderCoveyor)                                                    //Ztex 2024.03.25 Add Reduce tray exit time
        elUdUld->Add(edtULD_TrayBackDelay, &Ld_UldDelayTime.ULD_TrayBackDelay,          ECDouble,  "UnLoader Wait Time",  "Arrived",            bShow, bEnable, bReadFromFile, 1.0,      true,  10.0,      0.1);
    else
        elUdUld->Add(edtULD_TrayBackDelay, &Ld_UldDelayTime.ULD_TrayBackDelay,          ECDouble,  "UnLoader Wait Time",  "Arrived",            bShow, bEnable, bReadFromFile, 1.0,      true,  10.0,      1.0);    //JerryYang 20200408 有翻盤風險改最短1sec
    elUdUld->Add(edtULD_LiftDownDelay,   &Ld_UldDelayTime.ULD_LiftDownDelay,            ECDouble,  "UnLoader Wait Time",  "Check",              bShow, bEnable, bReadFromFile, 0.2,      true,  10.0,      0.2);
    elUdUld->Add(edtULD_TrayArrivalWait, &Ld_UldDelayTime.ULD_TrayArrivalWaitDelay,     ECDouble,  "UnLoader Wait Time",  "TrayArrivedWait",    bShow, bEnable, bReadFromFile, 0.3,      true,  10.0,      0.2);    //kevin 20190326 add auto 退tray 等待時間

    if(CosFunction.bKnockerSetBySetupFile==true && USE_LOADER_VIBRATION!=0)     //JerryYang 20191001 loader震動馬達
    {
        elUdUld->Add(cbF40,              &Ld_UldDelayTime.LD_EnableVibrate,             ECBool,    "Loader Wait Time",    "EnableVibrate",      bShow, bEnable, bReadFromFile, 0);
        elUdUld->Add(edP40_1,            &Ld_UldDelayTime.LD_VibrateLoopCnt,            ECInteger, "Loader Wait Time",    "VibrateLoopCount",   bShow, bEnable, bReadFromFile, 1,        true,  1,         15);
        elUdUld->Add(edP40_2,            &Ld_UldDelayTime.LD_VibrateOnDelay,            ECDouble,  "Loader Wait Time",    "VibrateOn",          bShow, bEnable, bReadFromFile, 10.0,     true,  1.0,       20.0);
        elUdUld->Add(edP40_3,            &Ld_UldDelayTime.LD_VibrateOffDelay,           ECDouble,  "Loader Wait Time",    "VibrateOff",         bShow, bEnable, bReadFromFile, 10.0,     true,  1.0,       15.0);
    }

    if(CosFunction.bKnockerSetBySetupFile==true && ShuttleVibration_INSTALL!=0) //JerryYang 20171006 (wei) Shuttle 震動馬達
    {
        elUdUld->Add(cbF23,              &IniConfig.bF23ShuttleVibration,                   ECBool,    "Tray",    "bF23ShuttleVibration",       bShow, bEnable, bReadFromFile, 0);
        if(CUSTOMER_CODE==CC_TERAPOWER)                                         //Sam 20210520 : TPW 修改震動時間 Range
            elUdUld->Add(edF23,          &IniConfig.iF23ShuttleVibrationTime,           ECInteger, "Shuttle", "iF23ShuttleVibrationTime",       bShow, bEnable, bReadFromFile, 20,       true,  1,         100);
        else
            elUdUld->Add(edF23,          &IniConfig.iF23ShuttleVibrationTime,           ECInteger, "Shuttle", "iF23ShuttleVibrationTime",       bShow, bEnable, bReadFromFile, 20,       true,  20,         100);
    }

    if(CosFunction.bKnockerSetBySetupFile==true)                                //Steven 20160329 : 敲擊汽缸參數調整可搭配工作檔處理
    {
        elUdUld->Add(cbP13,         &IniConfig.bP13EnableAutoTrayEdgePushCylinderLoop,  ECBool,     "Tray",     "bEnableAutoTrayEdgePushCylinderLoop",  bShow, bEnable, bReadFromFile, LastSet.bEnableAutoTrayEdgePushCylinderLoop?"1":"0");
        elUdUld->Add(edP13_1,       &IniConfig.iP13EdgePushCylinderLoopDelay,           ECInteger,  "Tray",     "iP13EdgePushCylinderLoopDelay",        bShow, bEnable, bReadFromFile, LastSet.iEdgePushCylinderLoopDelay,          true,  2,   300);
        elUdUld->Add(edP13_2,       &IniConfig.iP13EdgePushCylinderOnDelay,             ECInteger,  "Tray",     "iP13EdgePushCylinderOnDelay",          bShow, bEnable, bReadFromFile, 10,                                          true,  0,   50);

        elUdUld->Add(cbP14,         &IniConfig.bP14EnableAutoTrayRecevieDelayCount,     ECBool,     "Tray",     "bEnableAutoTrayRecevieDelayCount",     bShow, bEnable, bReadFromFile, LastSet.bEnableAutoTrayRecevieDelayCount?"1":"0");
        elUdUld->Add(edP14_1,       &IniConfig.iP14AutoTrayRecevieDelayCount,           ECInteger,  "Tray",     "iP14AutoTrayRecevieDelayCount",        bShow, bEnable, bReadFromFile, LastSet.iAutoTrayRecevieDelayCount,          true,  1,   1000);
        elUdUld->Add(edP14_2,       &IniConfig.iP14AutoTrayRecevieLoopDelayTime,        ECInteger,  "Tray",     "iP14AutoTrayRecevieLoopDelayTime",     bShow, bEnable, bReadFromFile, LastSet.iAutoTrayRecevieLoopDelayTime,       true,  2,   3000);

        elUdUld->Add(cbP16,         &IniConfig.bP16EnableHotplateEdgePushCylinderLoop,  ECBool,     "Hotplate", "bEnableHotplateEdgePushCylinderLoop",  bShow, bEnable, bReadFromFile, 0);

        if(TRAY_VIBRATION==VibrationMotor)                                      //JerryYang 20170531 (wei) 敲擊方式改成可以分別設定on off時間
        {
            elUdUld->Add(edP16_1,   &IniConfig.iP16HotplateEdgePushCylinderLoopDelay,   ECInteger, "Tray",      "iHotplateEdgePushCylinderLoopDelay",   bShow, bEnable, bReadFromFile, 10,                                          true,  200,   600);
            elUdUld->Add(edP16_2,   &IniConfig.iP16HotplateEdgePushCylinderOnDelay,     ECInteger, "Tray",      "iHotplateEdgePushCylinderOnDelay",     bShow, bEnable, bReadFromFile, 10,                                          true,  0,   50);
        }
        else
        {
            elUdUld->Add(edP16_1,   &IniConfig.iP16HotplateEdgePushCylinderLoopDelay,   ECInteger, "Tray",      "iHotplateEdgePushCylinderLoopDelay",   bShow, bEnable, bReadFromFile, 10,                                          true,  2,   600);
            elUdUld->Add(edP16_2,   &IniConfig.iP16HotplateEdgePushCylinderOnDelay,     ECInteger, "Tray",      "iHotplateEdgePushCylinderOnDelay",     bShow, bEnable, bReadFromFile, 10,                                          true,  2,   50);
        }
    }
}
#endif // GATE (L-1)

#if 0 // GATE (L-2) FormShow -- golden :87-129.  TRANSITIVE on TWO gated
      // callees in its first four lines: LoadImage() (:90, L-4, missing
      // TImage/TPicture) and ReadFile() (:92, L-5, elUdUld link boundary).
      // Everything after those two IS reachable today -- see the header.
void TfLd_ULd::FormShow(TObject *Sender)
{
    AnsiString S;
    LoadImage();
    LastFileName=GetLastOpenFN();
    ReadFile();
    S.sprintf("Loader/Unloader Condition  ''%s''  ",LastFileName);
    Caption=S;
    SetDefaultPos();                                                            //Steven 20240215 : 調整預設位置

    grpLoader->Enabled      =fSecurity->Insufficient(115, false);               //jou 981207 權限控制
    grpUnloader->Enabled    =fSecurity->Insufficient(115, false);
    btnDefaultValue->Enabled=fSecurity->Insufficient(115, false);
    tsKnocker->TabVisible   =(CosFunction.bKnockerSetBySetupFile);              //Steven 20160329 : 敲擊汽缸參數調整可搭配工作檔處理
    palKnocker->Enabled     =fSecurity->Insufficient(116, false);

    if(CosFunction.bKnockerSetBySetupFile==true && TRAY_VIBRATION==VibrationMotor)
    {
        labP13->Caption="[P13-1] Off delay time                   (Unit : 0.1 Sec)";
        labP16->Caption="[P16-1] Off delay time                   (Unit : 0.1 Sec)";
    }

    if(CosFunction.bKnockerSetBySetupFile==false || USE_LOADER_VIBRATION==0)    //JerryYang 20191001 loader震動馬達
    {
        cbF40->Visible=false;
        edP40_1->Visible=false;
        edP40_2->Visible=false;
        edP40_3->Visible=false;
        labF40_1->Visible=false;
        labF40_2->Visible=false;
        labF40_3->Visible=false;
    }

    if(ShuttleVibration_INSTALL==0)                                             //JerryYang 20171006 (wei) Shuttle 震動馬達
    {
        cbF23->Visible=false;
        edF23->Visible=false;                                                   //JerryYang 20171205 (Steven) shuttle震動馬達功能
    }

    fShow=true;
    //這一行請保持在最下面!!-----------------
//    myLog.Do_Log(Sender, asUser, asLogPath);                                  //Steven 20100629
}
#endif // GATE (L-2)

#if 0 // GATE (L-4) LoadImage -- golden :137-141.  MISSING TYPE: Image1/Image2
      // are golden `TImage*` and `->Picture->LoadFromFile(...)` needs
      // TImage/TPicture, neither of which has any port in vclcompat.  Same
      // established gap as forms/fHotPlate.h GATE (G-2).
void TfLd_ULd::LoadImage()
{
    Image1->Picture->LoadFromFile(BmpPath+"LoaderCondition.bmp");
    Image2->Picture->LoadFromFile(BmpPath+"UnloaderCondition.bmp");
}
#endif // GATE (L-4)

#if 0 // GATE (L-5) ReadFile -- golden :143-154.  LINK BOUNDARY: elUdUld's
      // ReadEditTextFromFile/InitialDataToEdit are out-of-line members of
      // HTEditList (Public/HTEditList.cpp, add_library(ht9045_sm)).
      // GetLastOpenFN/DataPath around them ARE reachable (common.cpp,
      // ht9045_core) -- the two registry calls are the whole blocker.
void TfLd_ULd::ReadFile()
{
    AnsiString szDir="";
    LastFileName=GetLastOpenFN();
    szDir.sprintf("%s%s\\", DataPath, LastFileName);

    if(elUdUld!=NULL)
    {
        elUdUld->ReadEditTextFromFile(szDir, "UdUld.Data");
        elUdUld->InitialDataToEdit();
    }
}
#endif // GATE (L-5)

#if 0 // GATE (L-6) DoIniDataToForm -- golden :156-162.  LINK BOUNDARY, same
      // elUdUld reason.  ⚠ The NULL guard is GOLDEN'S OWN, so the static-init
      // half of (L-1)(b) is already handled here; what still blocks this one
      // is purely the ht9045_sm link edge on HTEditList::InitialDataToEdit.
      // PRE-COMMITTED name (csystem.cpp:29302).
void TfLd_ULd::DoIniDataToForm()
{
    if(elUdUld!=NULL)
    {
        elUdUld->InitialDataToEdit();
    }
}
#endif // GATE (L-6)

#if 0 // GATE (L-3) FormClose -- golden :170-177.  TRANSITIVE via ReadFile()
      // (:172, L-5), its FIRST statement.  `rbTemp->SetFocus();` would itself
      // be fine (vclcompat::TControl::SetFocus is a real offline no-op) and
      // `fShow=false;` is an own field.
      // DEVIATION D-3: golden's (TObject*, TCloseAction&) parameters dropped.
void TfLd_ULd::FormClose()
{
    ReadFile();
    rbTemp->SetFocus();                                                         //Steven 20240215 : 避免Enable false的時候跳例外
    fShow=false;
    //這一行請保持在最下面!!-----------------
//    myLog.Do_Log(Sender, asUser, asLogPath);                                  //Steven 20100629
}
#endif // GATE (L-3)

#if 0 // GATE (L-7) spbSaveClick -- golden :179-202.  THREE INDEPENDENT GATES:
      // (a) TRANSITIVE/WRITES DISK -- SaveSetupFile(...) at :191 (L-8).
      // (b) LINK BOUNDARY -- EventReport (SECSGEM/SecsEventReport.cpp:15,
      //     add_library(ht9045_secsgem); the dependency runs the other way,
      //     ht9045_secsgem links ht9045_forms).
      // (c) MISSING SYMBOL -- fBuilder (no declaration anywhere in this port),
      //AI(W906-FW3-BTQ1) 20260828: SUPERSEDED -- FW3-BTQ1 新增 forms/fBuilder.{h,cpp}，fBuilder 現在有宣告。
      //    （此處仍在 #ifdef ASE_KaohSiung 內，而編譯器探針實測該巨集未定義。）
      //     inside #ifdef ASE_KaohSiung.
      //
      // ⚠ THE ONLY NON-MECHANICAL EDIT IN ANY TRANSCRIPT IN THIS WAVE is on
      // golden :189 below: its trailing `//Steven 20210716 : Add \\` ends the
      // line with a backslash, which the C++ lexer treats as a line splice --
      // so the comment swallows the following (blank) line, and g++ -Wall
      // reports -Wcomment EVEN INSIDE `#if 0` (the lexer runs regardless of
      // conditional compilation; a `#pragma GCC diagnostic ignored` does NOT
      // suppress it -- measured this wave).  The comment DELIMITER is changed
      // from `//` to `/* */`; the comment TEXT is byte-identical to golden's,
      // and the code line is untouched.  Recorded here so nobody later
      // mistakes it for a transcription error, and so nobody "tidies" the
      // text itself.
void TfLd_ULd::spbSaveClick(TObject *Sender)
{
    if(IniConfig.bA02DisableSaveParsWhenSwitchToOp==true &&                     //RogerYang 20260305 張寧說時間到都要登出，且不可被更改任何設定(A01_2)
        AccessLevel==0)
    {
        ShowMyMessage("[A01_2]目前已切換到Operator權限，\r\n請重新登入再做設定!");
        Close();
    }

    AnsiString szDir="";
    szDir.sprintf("%s%s\\", DataPath, LastFileName);                            /*Steven 20210716 : Add \\ */   // <- delimiter changed, see note at the top of this block

    SaveSetupFile(szDir, LastFileName);                                         //kevin 20150105  Start 另存 jobfile
    #ifdef ASE_KaohSiung
       fBuilder->bSaveAsJobFile(LastFileName, "JOBFILE");
    #endif
    ReadFile();
    spbSave->Down=false;
    if(IniConfig.bEnable_SECS_GEM==true)
    {
        bHasSaveSet=true;                                                       //Ifor 20151204 新增判斷機台有無修改設定檔
        EventReport(SECS_EVENT.SaveRecipe);
    }
}
#endif // GATE (L-7)

#if 0 // GATE (L-8) SaveSetupFile -- golden :206-216.  TWO INDEPENDENT GATES:
      // (a) WRITES DISK -- MyForceDirectories CREATES DIRECTORIES
      //     (common.cpp:1888) and elUdUld->SaveEditTextToFile serialises the
      //     recipe's loader/unloader timing block to the file UdUld.Data
      //     under <DataPath>/<recipe>/.
      // (b) LINK BOUNDARY -- the same elUdUld / ht9045_sm edge as (L-1)(a).
      //     ⚠ fMain->BackupSetupFile() (:215) is NOT the blocker: it IS
      //     reachable (forms/fMain.h:260, forms/fMain.cpp, ht9045_forms).
      // PRE-COMMITTED name and signature (csystem.cpp:29303).
//------------------------------------------------------------
//kevin 20140105 儲存 檔案另存 jobfile
//------------------------------------------------------------
void TfLd_ULd::SaveSetupFile(AnsiString szDir, AnsiString S)
{
    MyForceDirectories(szDir);

    if(elUdUld!=NULL)
    {
        elUdUld->SaveEditTextToFile(szDir, "UdUld.Data");
    }

    fMain->BackupSetupFile();                                                   //Ifor 20170620 (wei) add Auto BackUp Setup File & Last Data
}
#endif // GATE (L-8)

// -- GATE (L-10) FormShortCut -- golden :239-245, OMITTED ENTIRELY (not even
//    declared in forms/fLd_ULd.h).  `TWMKey &Msg` has zero port anywhere in
//    this tree, so even a `#if 0` transcript would name a type that does not
//    exist.  Golden's whole body is
//        if(Msg.CharCode==VK_F5) SetDefaultPos();
//    -- i.e. the only thing it does is call this wave's own ACTIVE
//    SetDefaultPos(); nothing of substance is lost, only the key binding.
//    Same established precedent as forms/fHotPlate.h GATE (G-12) /
//    forms/fContact.h GATE (X-27).
