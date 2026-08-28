// =============================================================================
//  forms/fQAMode.cpp  --  definitions for the fQAMode facade
//
//  AI(W906-FW3-BTQ1) 20260828: new file, FW wave FW3-BTQ1 (3 of 5 facades).
//  GOLDEN SOURCE: HT9011UC_Code_V3.33.906.0_20260618/QAMode.cpp (336 lines)
//  + QAMode.h (64 lines), read with `io.open(p, encoding='cp950')`.
//  SPAN: 13 golden bodies = 12 `TfQAMode::` members + 1 file-scope
//  (`QABackupStatus`), 302 span lines (tools/census/wave_preflight.py,
//  20260828).
//
//  THIS WAVE, against the 12-MEMBER denominator:
//     5 ACTIVE           ctor / btnOkClick / edQAModeMouseDown / DoFormToData /
//                        edtQASampleCntMouseDown
//     1 ACTIVE-PARTIAL   FormClose -- golden :172 live, :173 GATE (Q-1)
//     6 GATED-WITH-BODY  DoIniDataToForm / btnApplyClick / FormShow /
//                        ReadFile / Image1Click / ShowTrayDirectIMG
//     + 1 GATED file-scope body, QABackupStatus (51L)
//  BY LINES: 40 of 302 golden span lines live (13.2%).
//
//  See forms/fQAMode.h for the full GATE REGISTER (Q-1)..(Q-6) + (Q-F), the
//  ACTIVE evidence, DEVIATIONS D-1..D-7, the FIELD LIST (including the 21
//  widgets deliberately NOT declared) and the ZERO-WRITER warning about
//  calling DoFormToData before (Q-1) is supplied.
//
//  ⚠ THE ONE GATE WORTH READING TWICE is (Q-4) ReadFile: a body whose NAME
//  says Read, which CREATES DIRECTORIES (MyForceDirectories, golden :184) and
//  SEEDS ini keys through CheckAndReadIniData (common.cpp:623-624 WriteBool).
//  Function names containing "Read" do not guarantee read-only.
//
//  ⚠ EVERY `#if 0` BLOCK BELOW HAS NEVER BEEN COMPILED.  The text is golden's
//  own (the only edits are dropping `__fastcall` and the unused FormClose /
//  MouseDown parameters), so it is a faithful TRANSCRIPT -- not verified code.
//  Many of the identifiers it names (fLotInfo->edQAMode, Image1, Picture,
//  gbLoaderDirection, pnlQASample, labQAModeBin, tsQASampling, Prod,
//  s6TrayName, s3TrayName, iFixRight, LevelSet, TrayForm, BmpPath,
//  CheckKeyExist, IntUnionBit, bTestSiteUse, fMain->ShowTestHeadComp,
//  fBinSel, ...) are NOT members of this facade and/or have no reachable
//  definition; un-gating requires supplying them first.
//
//  BACKSLASH-COMMENT SCAN (the -Wcomment line-splice trap): QAMode.cpp was
//  scanned 20260828 for a `//` comment whose line ends in a backslash.  ZERO
//  hits -- so no comment delimiter was changed anywhere in this file.
// =============================================================================
#include "forms/fQAMode.h"
#include "forms/fQwertyKey.h"    // fQwertyKey->ShowQwertyKey (forms/fQwertyKey.cpp, ht9045_forms)
#include "cprod.h"               // TestIF_File (cprod.h:2577, cprod.cpp, ht9045_globals)
#include "cmydef.h"              // N_INTEGER (extern const int, cmydef.cpp, ht9045_globals)
#include "MachineType.h"         // CheckRange (MachineType.h:1525) -- a TEMPLATE, header-only, no link edge

#include <cstdlib>               // atoi (golden :58, :70)

// AI(W906-FW3-BTQ1) 20260828: TfQAMode/fQAMode were FREE tree-wide -- same
// idiom as forms/fCounterSel.cpp:38 / forms/fLd_ULd.cpp:43.  Golden's ctor
// body (:22-25) is EMPTY, so this static-init `new` touches nothing at all --
// no SIOF risk whatsoever (docs/KNOWLEDGE.md "static-init ctor 不可碰 NULL
// 全域"; the fLaserSensor incident that rule comes from turned 88 of 134 ctest
// binaries into SEGFAULTs).  The eleven widget field initialisers construct
// leaf vclcompat controls and call nothing translated.
TfQAMode *fQAMode = new TfQAMode();

TfQAMode::TfQAMode()   // golden :22-25 -- golden's body is empty
{
}

// ---------------------------------------------------------------------------
void TfQAMode::btnOkClick(TObject *Sender)   // golden :27-30
{
    (void)Sender;
    Close();   // DEVIATION D-3 -- port-only no-op
}

// ---------------------------------------------------------------------------
//  golden :32-36.  ACTIVE.  DEVIATION D-6: golden's unused TMouseButton /
//  TShiftState / int X / int Y parameters are dropped.
//  ⚠ golden really does pass min=10000, max=5 -- INVERTED relative to the
//  `CheckRange(value, 5, 10000)` its own DoFormToData uses at golden :58.
//  Transcribed verbatim, NOT "fixed" (改行為要留給使用者決定).
// ---------------------------------------------------------------------------
void TfQAMode::edQAModeMouseDown(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 10000, 5);
}

// ---------------------------------------------------------------------------
//  golden :56-71.  ACTIVE.
//  ⚠ THIS WRITES A GLOBAL CONFIG STRUCT IN MEMORY (TestIF_File) and that is
//  deliberate: it touches no file, sends no packet and moves no hardware, so
//  it does not meet this wave's gating judgement.  Its disk-committing
//  partner, btnApplyClick, IS gated -- see (Q-2).
//  ⚠ BUT SEE THE HEADER'S ZERO-WRITER NOTE BEFORE WIRING A CALLER: every
//  widget read here is written only by GATED (Q-1), so today this copies
//  vclcompat's defaults (""/0/false) into TestIF_File rather than the recipe's
//  values.  Nothing calls it in this port.
// ---------------------------------------------------------------------------
void TfQAMode::DoFormToData()
{
    TestIF_File.iQAModeCount        =CheckRange(atoi(edQAMode->Text.c_str()), 5, 10000);    //Steven 20111026
    TestIF_File.iQAModeRunType      =rgQARunMode->ItemIndex;                                //Steven 20120612 : QA做完後的動作
    TestIF_File.iQAModeBin          =cbQAModeBin->ItemIndex;                                //Steven 20141023 : QA做完後的Bin
    TestIF_File.bQAModeAfterTrayEnd =cbTrayEndDoQAModeAgain->Checked;                       //Steven 20151125 : QA做完後的TrayEnd要重做QA
    TestIF_File.bQAD22DoubleContact =cbQAD22Double->Checked;                                //Sam 20231117 : 整合到 QA 模式
    TestIF_File.iQAD22DoubleContactCount=cbbQAD22Double->ItemIndex;                         //Sam 20231117 : 整合到 QA 模式
    TestIF_File.bQATrayEndCloseYield100Site=cbTrayEndCloseYield100Site->Checked;            //Sam 20231117 : 整合到 QA 模式

    TestIF_File.bEnableQASampling   =cbTempOffset->Checked;                     //Steven 20190326 : QA Sampling
    TestIF_File.iQASamplingBin      =cbbQASampleBin->ItemIndex;
//    TestIF_File.iQASamplingT3Pos     =cbbQASampleTray->ItemIndex+1;           //因為0是no-use
    TestIF_File.sQASamplingTray     =cbbQASampleTray->Text;
    TestIF_File.iQASamplingCnt      =atoi(edtQASampleCnt->Text.c_str());
}

// ---------------------------------------------------------------------------
//  golden :279-283.  ACTIVE.  DEVIATION D-6 as above.
//  ⚠ golden passes min=10000, max=0 here -- same inversion as
//  edQAModeMouseDown.  Verbatim.
// ---------------------------------------------------------------------------
void TfQAMode::edtQASampleCntMouseDown(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 10000, 0);
}

// ---------------------------------------------------------------------------
//  golden :170-176.  ACTIVE-PARTIAL: golden :172 is live below; golden :173
//  `DoIniDataToForm();` is GATE (Q-1) (fLotInfo has no edQAMode member).
//  Keeping :172 live is what gives `fShow` a real writer; the dropped line is
//  itself gated, so the two un-gate together.  Line-level gate, following the
//  forms/fTowerLight.h (T-2) / forms/fHandlerSys.h (H22-1) precedent.
//  DEVIATION D-4: golden's (TObject*, TCloseAction&) parameters dropped.
// ---------------------------------------------------------------------------
void TfQAMode::FormClose()
{
    fShow=false;
    // GATE (Q-1) -- golden :173 `DoIniDataToForm();`
    //JerryYang 20250411 : 離開頁面要刷新一次, 避免誤存檔
    //這一行請保持在最下面!!-----------------
//    myLog.Do_Log(Sender, asUser, asLogPath);                                  //Steven 20100629
}

// ===========================================================================
//  GATE REGISTER -- translated golden bodies, deliberately NOT COMPILED.
//  See forms/fQAMode.h for the per-entry reasoning.
// ===========================================================================

#if 0 // GATE (Q-1) DoIniDataToForm -- golden :38-54.  MISSING MEMBER ON
      // ANOTHER FACADE: golden :40 `fLotInfo->edQAMode->Text`.  fLotInfo IS
      // real and live (forms/fLotInfo.cpp, SAME ht9045_forms target) but
      // forms/fLotInfo.h declares NO edQAMode -- `rg "edQAMode"
      // forms/fLotInfo.h` (20260828) returns 3 hits, all three PROSE
      // (:276, :365, :773).  The other 13 lines are pure TestIF_File
      // reflection and would have been reachable.  NOT SPLIT: dropping :40
      // silently stops the MAIN screen's QA counter from tracking this form.
void TfQAMode::DoIniDataToForm()
{
    fLotInfo->edQAMode->Text         =TestIF_File.iQAModeCount;                 //JerryYang 20190701 SPIL要求主畫面可切換EQC mode
    edQAMode->Text                  =TestIF_File.iQAModeCount;                  //Steven 20111026
    rgQARunMode->ItemIndex          =TestIF_File.iQAModeRunType;                //Steven 20120612 : QA做完後的動作
    cbQAModeBin->ItemIndex          =TestIF_File.iQAModeBin;                    //Steven 20141023 : QA做完後的Bin
    cbTrayEndDoQAModeAgain->Checked =TestIF_File.bQAModeAfterTrayEnd;           //Steven 20151125 : QA做完後的TrayEnd要重做QA
    cbQAD22Double->Checked          =TestIF_File.bQAD22DoubleContact;           //Sam 20231117 : 整合到 QA 模式
    cbbQAD22Double->ItemIndex       =TestIF_File.iQAD22DoubleContactCount;      //Sam 20231117 : 整合到 QA 模式
    cbTrayEndCloseYield100Site->Checked=TestIF_File.bQATrayEndCloseYield100Site;//Sam 20231117 : 整合到 QA 模式

    cbTempOffset->Checked           =TestIF_File.bEnableQASampling;             //Steven 20190326 : QA Sampling
    cbbQASampleBin->ItemIndex       =TestIF_File.iQASamplingBin;
//    cbbQASampleTray->ItemIndex      =TestIF_File.iQASamplingT3Pos-1;          //因為0是no-use
    cbbQASampleTray->Text           =TestIF_File.sQASamplingTray;
    edtQASampleCnt->Text            =TestIF_File.iQASamplingCnt;
}
#endif // GATE (Q-1)

#if 0 // GATE (Q-2) btnApplyClick -- golden :73-110.  WRITES DISK: 13 x
      // WriteIniData into <DataPath>\<recipe>\Tester.Data (:91-104), preceded
      // by MyForceDirectories(szDir) at :88 which CREATES DIRECTORIES.  Both
      // helpers are REACHABLE (common.cpp, ht9045_core) -- pure read-only-
      // direction safety gate, not a link gate.  [T] on (Q-4) at :106 and
      // (Q-1) at :107.  fBinSel has no facade in this tree.
void TfQAMode::btnApplyClick(TObject *Sender)
{
    if(IniConfig.bA02DisableSaveParsWhenSwitchToOp==true &&                     //RogerYang 20260305 張寧說時間到都要登出，且不可被更改任何設定(A01_2)
        AccessLevel==0)
    {
        ShowMyMessage("[A01_2]目前已切換到Operator權限，\r\n請重新登入再做設定!");
        Close();
    }

    DoFormToData();

    AnsiString S="";
    S=GetLastOpenFN();
    AnsiString szDir="";
    szDir.sprintf("%s%s", DataPath, S);
    MyForceDirectories(szDir);
    szDir+="\\Tester.Data";

    WriteIniData(szDir, "QA Mode", "Count",                 int(TestIF_File.iQAModeCount));     //Steven 20111026
    WriteIniData(szDir, "QA Mode", "Run Type",              TestIF_File.iQAModeRunType);        //Steven 20120612 : QA做完後的動作
    WriteIniData(szDir, "QA Mode", "Tray Direct",           TestIF_File.iQATrayDirect);
    WriteIniData(szDir, "QA Mode", "Untest Bin",            TestIF_File.iQAModeBin);            //Steven 20141023 : QA做完後的Bin
    WriteIniData(szDir, "QA Mode", "bQAModeAfterTrayEnd",   TestIF_File.bQAModeAfterTrayEnd);   //Steven 20151125 : QA做完後的TrayEnd要重做QA
    WriteIniData(szDir, "QA Mode", "bQAD22DoubleContact",   TestIF_File.bQAD22DoubleContact);   //Sam 20231117 : 整合到 QA 模式
    WriteIniData(szDir, "QA Mode", "iQAD22DoubleContactCount", TestIF_File.iQAD22DoubleContactCount);       //Sam 20231117 : 整合到 QA 模式
    WriteIniData(szDir, "QA Mode", "bQATrayEndCloseYield100Site",   TestIF_File.bQATrayEndCloseYield100Site);   //Sam 20231117 : 整合到 QA 模式

    WriteIniData(szDir, "QA Sampling", "Enable",    TestIF_File.bEnableQASampling);             //Steven 20190326 : QA Sampling
    WriteIniData(szDir, "QA Sampling", "Bin",       TestIF_File.iQASamplingBin);
//    WriteIniData(szDir, "QA Sampling", "Tray",      TestIF_File.iQASamplingTray);
    WriteIniData(szDir, "QA Sampling", "Tray Name", TestIF_File.sQASamplingTray);               //Steven 20230930 : 改用Tray名稱存檔, 避免異常
    WriteIniData(szDir, "QA Sampling", "Count",     TestIF_File.iQASamplingCnt);
    fMain->BackupSetupFile();                                                   //Ifor 20170620 (wei) add Auto BackUp Setup File & Last Data
    ReadFile();                                                                 //JerryYang 20190701 SPIL要求主畫面可切換EQC mode
    DoIniDataToForm();

    fBinSel->ReadFile(false, false, "");                                                        //Steven 20190326 : QA Sampling
}
#endif // GATE (Q-2)

#if 0 // GATE (Q-3) FormShow -- golden :112-168.  TRANSITIVE HIDDEN WRITE:
      // :128 calls ReadFile (GATE Q-4, creates dirs AND seeds ini keys), :129
      // calls (Q-1), :130 calls (Q-6).  Also reads Prod.iTrayType[] /
      // s6TrayName[] / iFixRight / LevelSet.AccessLevel[85] and writes
      // Width/Height, none of which this facade declares (DEVIATION D-5).
void TfQAMode::FormShow(TObject *Sender)
{
    cbQAModeBin->Clear();                                                       //Steven 20150122 : Fixed for QA Mode
    for(int i=0; i<iTestBinCount; i++)
        cbQAModeBin->Items->Add(i);

    cbbQASampleTray->Clear();
    for(int i=0; i<iFixRight; i++)
    {
        if(Prod.iTrayType[i]==tTrayAuto)
            cbbQASampleTray->Items->Add(s6TrayName[i]);
    }

    Left=(1280-Width)/2;
    Top =(1024-Height)/2;

    ReadFile();
    DoIniDataToForm();
    ShowTrayDirectIMG();
    cbQAModeBin->Enabled=(AccessLevel<LevelSet.AccessLevel[85])?false:true;
    if(CosFunction.bQAModeUseUnloadCnt)                                         //JerryYang 20221004 : Maxim版本QA mode
    {
        rgQARunMode->Enabled=false;
    }
    else
    {
        rgQARunMode->Enabled=(AccessLevel<LevelSet.AccessLevel[85])?false:true;
    }
    gbLoaderDirection->Enabled=(AccessLevel<LevelSet.AccessLevel[85])?false:true;
    edQAMode->Enabled=(AccessLevel<LevelSet.AccessLevel[98])?false:true;
    pnlQASample->Enabled=(AccessLevel<LevelSet.AccessLevel[85])?false:true;     //Steven 20190326 : QA Sampling

    if(CUSTOMER_CODE==CC_SCS)
        rgQARunMode->Enabled=false;                                             // wei  20130103

    if(CUSTOMER_CODE==CC_SIGURD_PeiXing)                                        //Sam 20240122 : 矽格北興俊堯要求鎖死
    {
        edQAMode->Enabled=false;
        rgQARunMode->Enabled=false;
        cbQAModeBin->Enabled=false;
        cbTrayEndDoQAModeAgain->Enabled=false;
    }

    if(IniConfig.bMaximFunction==true ||                                        //JerryYang 20221004 : Maxim版本QA mode
       CUSTOMER_CODE==CC_AnalogDevice_Phil)
    {
        labQAModeBin->Visible=false;
        cbQAModeBin->Visible=false;
        cbTrayEndDoQAModeAgain->Visible=false;
    }

    fShow=true;
    tsQASampling->TabVisible=(CUSTOMER_CODE==CC_AMKOR_Korea &&
                              TestIF_File.bSCKART_EnableART);                   //Steven 20190326 : QA Sampling
    //這一行請保持在最下面!!-----------------
//    myLog.Do_Log(Sender, asUser, asLogPath);                                  //Steven 20100629
}
#endif // GATE (Q-3)

#if 0 // GATE (Q-4) ReadFile -- golden :178-232.  ⚠ HIDDEN WRITE.  The NAME
      // says Read; the body writes twice over:
      //   (a) MyForceDirectories(szDir) at :184 CREATES DIRECTORIES under
      //       DataPath before anything is read at all;
      //   (b) the eight CheckAndReadIniData(...) reads at :202-231 go to
      //       common.cpp:613, whose :623-624 are
      //           if (!INIFile->ValueExists(Group, Name))
      //                INIFile->WriteBool(Group, Name, Value);   // seed
      //       i.e. a *read* SEEDS the file with the default when a key is
      //       absent.  The ReadIniData(...) calls at :187/:194/:197/:199 are
      //       the pure-read sibling and do NOT do this -- same function-pair
      //       trap as forms/fHotPlate.h (G-1) / forms/fCounterSel.h (C-1).
void TfQAMode::ReadFile()
{
    AnsiString S="";
    S=GetLastOpenFN();
    AnsiString szDir="";
    szDir.sprintf("%s%s", DataPath, S);
    MyForceDirectories(szDir);
    szDir+="\\Tester.Data";

    TestIF_File.iQAModeCount    =CheckRange(ReadIniData(szDir, "QA Mode", "Count", 200), 5, 10000);  //Steven 20111026

    if(CUSTOMER_CODE==CC_SCS || CosFunction.bQAModeUseUnloadCnt)                //JerryYang 20221004 : Maxim版本QA mode
        TestIF_File.iQAModeRunType=1;                                           // wei  20130103
    else if(CUSTOMER_CODE==CC_SIGURD_PeiXing)                                   //Sam 20240122 : 矽格北興俊堯要求鎖死
        TestIF_File.iQAModeRunType=3;
    else
        TestIF_File.iQAModeRunType=ReadIniData(szDir, "QA Mode", "Run Type", 0);//Steven 20120612 : QA做完後的動作

    if(CUSTOMER_CODE==CC_AMKOR_Korea)                                           //ChungHung 20141204 add for ATK
        TestIF_File.iQAModeBin=ReadIniData(szDir, "QA Mode", "Untest Bin", 1);
    else
        TestIF_File.iQAModeBin=ReadIniData(szDir, "QA Mode", "Untest Bin", iTestBinCount-1);                    //Steven 20141023 : QA做完後的Bin
    TestIF_File.iQAModeBin=CheckRange(TestIF_File.iQAModeBin, 0, iTestBinCount-1);                              //Steven 20150122 : Fixed for QA Mode

    TestIF_File.iQATrayDirect      =CheckAndReadIniData(szDir, "QA Mode", "Tray Direct", TrayForm.Loader.Direction);
    TestIF_File.bQAModeAfterTrayEnd=CheckAndReadIniData(szDir, "QA Mode", "bQAModeAfterTrayEnd", false);        //Steven 20151125 : QA做完後的TrayEnd要重做QA
    TestIF_File.bQAD22DoubleContact=CheckAndReadIniData(szDir, "QA Mode", "bQAD22DoubleContact", false);        //Sam 20231117 : 整合到 QA 模式
    TestIF_File.iQAD22DoubleContactCount=CheckAndReadIniData(szDir, "QA Mode", "iQAD22DoubleContactCount", 1);              //Sam 20231117 : 整合到 QA 模式
    TestIF_File.bQATrayEndCloseYield100Site=CheckAndReadIniData(szDir, "QA Mode", "bQATrayEndCloseYield100Site", false);        //Sam 20231117 : 整合到 QA 模式

    TestIF_File.bEnableQASampling  =CheckAndReadIniData(szDir, "QA Sampling", "Enable",    false);             //Steven 20190326 : QA Sampling
    TestIF_File.iQASamplingBin     =CheckAndReadIniData(szDir, "QA Sampling", "Bin",       1);

    if(CheckKeyExist(szDir, "QA Sampling", "Tray Name")==false)
    {
        TestIF_File.iQASamplingT3Pos=CheckAndReadIniData(szDir, "QA Sampling", "Tray",      3);

        int iTray=TestIF_File.iQASamplingT3Pos-1;
        if(iTray<0)
            TestIF_File.sQASamplingTray=s3TrayName[0];
        else
            TestIF_File.sQASamplingTray=s3TrayName[iTray];
    }
    else
    {
        TestIF_File.sQASamplingTray=CheckAndReadIniData(szDir, "QA Sampling", "Tray Name", AnsiString("Fix1"));
        TestIF_File.iQASamplingT3Pos=0;                                         //因為0是no-use
        for(int i3=0; i3<e3TrayCount; i3++)
        {
            if(s3TrayName[i3]==TestIF_File.sQASamplingTray)
                TestIF_File.iQASamplingT3Pos=i3+1;
        }
    }
    TestIF_File.iQASamplingCnt     =CheckAndReadIniData(szDir, "QA Sampling", "Count",     100);
}
#endif // GATE (Q-4)

#if 0 // GATE (Q-5) Image1Click -- golden :234-253.  NO TIMAGE PORT:
      // `Image1->Picture->LoadFromFile(sDir)`; neither TImage nor TPicture
      // exists in vclcompat (0 hits, 20260828).  It also MUTATES
      // TestIF_File.iQATrayDirect at :242 as a side effect of a repaint, so
      // the mutation cannot be split out and kept on its own.
      // (MyDBIProcess at :250 IS reachable -- aHotPlateSubstrate.cpp:1099 --
      // so it is NOT a blocker.)
void TfQAMode::Image1Click(TObject *Sender)
{
    AnsiString sDir;

    int dir=TestIF_File.iQATrayDirect;
        dir++;
    if(dir>=8)
        dir=0;
    TestIF_File.iQATrayDirect=dir;
    sDir.sprintf("%stype%d.bmp", BmpPath, dir);
    try
    {
        Image1->Picture->LoadFromFile(sDir);
    }
    catch(...)
    {
        MyDBIProcess("Exception", "Image1Click");
        ShowMyMessage("Path :D:\\HT9045\\IMG\\BMP\\ be delete");
    }
}
#endif // GATE (Q-5)

#if 0 // GATE (Q-6) ShowTrayDirectIMG -- golden :255-277.  Same TImage absence
      // as (Q-5); also clamps TestIF_File.iQATrayDirect in place (:264-265).
      // ⚠ GOLDEN BUG RECORDED, NOT FIXED: the clamp tests `>8` while
      // Image1Click's wrap (:240) rolls at `>=8`, so 8 survives the clamp and
      // would load type8.bmp.  Verbatim.
      // ⚠ NAME COLLISION, resolved: forms/fTrayAssignment.{h,cpp} also has a
      // `ShowTrayDirectIMG` -- that is TfTrayAssignment's own member from
      // golden cTrayAssignment.cpp:1127-1148, a different class in a different
      // target (ht9045_sm).  Not a port of this body.
void TfQAMode::ShowTrayDirectIMG()
{
    AnsiString sDir;
    if(CosFunction.bQAmodeSupplyTrayDir==false)
    {
        gbLoaderDirection->Visible=false;
    }
    else
    {
        if(TestIF_File.iQATrayDirect<0 || TestIF_File.iQATrayDirect>8)
            TestIF_File.iQATrayDirect=0;
        sDir.sprintf("%stype%d.bmp", BmpPath, TestIF_File.iQATrayDirect);
        try
        {
            Image1->Picture->LoadFromFile(sDir);
        }
        catch(...)
        {
            MyDBIProcess("Exception", "TfQAMode::ShowTrayDirectIMG");
            ShowMyMessage("''Path :D:\\HT9045\\IMG\\BMP\\'' be delete");
        }
    }
}
#endif // GATE (Q-6)

#if 0 // GATE (Q-F) QABackupStatus -- golden :285-335 (51L), FILE-SCOPE.
      // TWO INDEPENDENT GATES:
      // (a) WRITES DISK -- 2 x WriteIniData into AuthPath+"config.ini"
      //     (:304-305, the SHARED runtime-parameter file behind
      //     docs/KNOWLEDGE.md's "Gerneral.ini 20260817被整檔重寫" incident)
      //     and 2 x into <recipe>\Tester.Data (:332-333).
      // (b) MODE SWITCH -- :319-320 overwrite the live SiteMap arrays
      //     bTestSiteUse[][][] and LastSet.bUseTestSocket[][][], i.e. they
      //     change which test sites the machine considers enabled.  That is a
      //     machine-state change, not merely a file write.
      // Secondary, NOT relied on: `IntUnionBit` and `fMain->ShowTestHeadComp`
      // were not resolved to a port (see the header's DISCLOSURE).
void QABackupStatus(bool bBackup)                                               //Sam 20231117 : 整合到 QA 模式
{
    IntUnionBit iuTestSiteUse[2], iubUseTestSocket[2];
    AnsiString sPath=AuthPath+"config.ini";
    if(bBackup)
    {
        //備份 SiteMap On/Off 狀態
        for(int Z=0;Z<2;Z++)
        {
            iuTestSiteUse[Z].SetInt(0);
            iubUseTestSocket[Z].SetInt(0);
            for(int Y=0;Y<MAX_SOCKET_ROW;Y++)
            {
                for(int X=0;X<MAX_SOCKET_COL;X++)
                {
                    iuTestSiteUse[Z].SetBit(Y*MAX_SOCKET_COL+X,bTestSiteUse[Z][Y][X]);
                    iubUseTestSocket[Z].SetBit(Y*MAX_SOCKET_COL+X,LastSet.bUseTestSocket[Z][Y][X]);
                }
            }
            WriteIniData(sPath, "Index", "QAMode_BackupTestSiteUse"+IntToStr(Z),    (unsigned long)iuTestSiteUse[Z].UnsignInt);
            WriteIniData(sPath, "Index", "QAMode_BackupUseTestSocket"+IntToStr(Z),  (unsigned long)iubUseTestSocket[Z].UnsignInt);
        }
    }
    else
    {
        //還原 SiteMap On/Off 狀態
        for(int Z=0;Z<2;Z++)
        {
            iuTestSiteUse[Z].SetInt(ReadIniData(sPath, "Index", "QAMode_BackupTestSiteUse"+IntToStr(Z),0));
            iubUseTestSocket[Z].SetInt(ReadIniData(sPath, "Index", "QAMode_BackupUseTestSocket"+IntToStr(Z),0));
            for(int Y=0;Y<MAX_SOCKET_ROW;Y++)
            {
                for(int X=0;X<MAX_SOCKET_COL;X++)
                {
                    bTestSiteUse[Z][Y][X]=iuTestSiteUse[Z].Bit(Y*MAX_SOCKET_COL+X);
                    LastSet.bUseTestSocket[Z][Y][X]=iubUseTestSocket[Z].Bit(Y*MAX_SOCKET_COL+X);
                }
            }
        }
        fMain->ShowTestHeadComp(false);

        AnsiString S="";
        S=GetLastOpenFN();
        AnsiString szDir="";
        szDir.sprintf("%s%s", DataPath, S);
        MyForceDirectories(szDir);
        szDir+="\\Tester.Data";
        WriteIniData(szDir, "QA Mode", "bQAD22DoubleContact", false);           //Sam 20240122 : 北興俊堯要求恢復初始設定
        WriteIniData(szDir, "QA Mode", "bQATrayEndCloseYield100Site", true);    //Sam 20231117 : 整合到 QA 模式
    }
}
#endif // GATE (Q-F)
