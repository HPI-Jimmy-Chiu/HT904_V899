// =============================================================================
//  forms/fTesterIF.cpp  --  definitions for the FTestIF facade
//
//  AI(W906-FW3-TIF1) 20260828: new file, FW wave FW3-TIF1.
//  GOLDEN SOURCE: HT9011UC_Code_V3.33.906.0_20260618/cTesterIF.cpp (1569
//  lines) + cTesterIF.h (318 lines), read with
//  `io.open(p, encoding='cp950')`.
//  SPAN: 38 golden `TFTestIF::` member bodies, 1488 span lines
//  (tools/census/wave_preflight.py, 20260828).
//  THIS WAVE: 21 of 38 members ACTIVE (55.3%, the ctor PARTIAL) + 16 GATED
//  (bodies translated below inside `#if 0`) + 1 OMITTED (FormShortCut).
//  BY LINES: 115 of 1488 golden span lines are LIVE (7.7%); 1338 (89.9%) are
//  carried as `#if 0` transcript; 8 (0.5%) omitted. Quote member count and
//  line count together -- alone, either one misleads.
//
//  ⚠ `TFTestIF`/`FTestIF` (this form) is NOT `TestIF`/`TestIF_File`
//  (cprod.h:2576-2577, plain SYSTEM_TEST_IF data globals in ht9045_globals).
//  See the NAME COLLISION block at the top of forms/fTesterIF.h.
//
//  See forms/fTesterIF.h for the full GATE REGISTER (F-1)..(F-18) with the
//  per-symbol target attributions, the ACTIVE reachability evidence,
//  DEVIATIONS D-1..D-7, the FIELD LIST and the ZERO-WRITER value-provenance
//  warning (rgInterfaceType->ItemIndex==0 means TTL_MODE, an actively matched
//  branch -- it is not a neutral "unset").
//
//  ⚠ EVERY `#if 0` BLOCK BELOW HAS NEVER BEEN COMPILED. The text is golden's
//  own, sliced programmatically out of the cp950 decode (the ONLY edit is
//  dropping `__fastcall`), so it is a faithful TRANSCRIPT -- not verified
//  code. Most identifiers it names (fMain->CheckCanChangeRealDummy, fSecurity,
//  fDIOFrom, fRPDefault, fSpeed, SW[], Barcode_Reader, CheckTTLBoardBitMode,
//  Application, TWinControl, and ~200 undeclared widgets) are unreachable
//  and/or unported here; un-gating requires supplying them first.
// =============================================================================
#include "forms/fTesterIF.h"
#include "forms/fQwertyKey.h"   // fQwertyKey->ShowQwertyKey (real, forms/fQwertyKey.cpp, ht9045_forms)
#include "forms/fTesterTCP.h"   // fTesterTCP->Show() (this wave's sibling facade, same target)
#include "cprod.h"              // TestIF.bAntiSignal (cprod.cpp, ht9045_globals) + MachineType.h CC_* defines
#include "cmydef.h"             // N_DOUBLE / N_INTEGER / N_PORT / CUSTOMER_CODE (cmydef.cpp, ht9045_globals)
#include "Config.h"             // IniConfig.bVTESTFunction (Config.cpp, ht9045_globals)
#include "CosFunction.h"        // CosFunction.bHiSiliconFunction (CosFunction.cpp, ht9045_globals)
#include "common.h"             // OnlyNumberInPut / OnlyNumberAndDotInPut (common.cpp, ht9045_core)

#include <cstdlib>              // atof / atoi (golden :1537/:1542/:1547)

// AI(W906-FW3-TIF1) 20260828: TFTestIF/FTestIF were FREE tree-wide -- same
// idiom as forms/fPrecaution.cpp:14 / forms/fHotPlate.cpp:24. The ACTIVE part
// of golden's ctor (:43-44) writes only two of this object's own ints, so this
// static-init `new` touches no global -- no SIOF risk (docs/KNOWLEDGE.md
// "static-init ctor 不可碰 NULL 全域"). Golden's remaining ctor statements DO
// call a directory-scanning method and read CUSTOMER_CODE, and are gated --
// see (F-1).
TFTestIF *FTestIF = new TFTestIF();

// ---------------------------------------------------------------------------
//  golden :39-67.  ACTIVE-PARTIAL: golden :43-44 are live below; golden :42
//  (InitcbDIOType) and :46-66 (the CC_ASE_KaohSiung / CC_Greatek blocks) are
//  GATE (F-1) and appear as transcript further down this file.
// ---------------------------------------------------------------------------
TFTestIF::TFTestIF()
{
    // GATE (F-1)a -- golden :42 `InitcbDIOType(false);`, transcript below.
    ioldTestType=-1;                                                            //0:TTL_MODE ; 1: GPIB_MODE ; 2: RS232_MODE     //Isaac 20200903 :TTL RS232通訊
    ioldDIOType=-1;
    // GATE (F-1)b -- golden :46-66 (CC_ASE_KaohSiung GPIB type renaming and
    // the CC_Greatek `->Hint` writes), transcript below.
}

// ---------------------------------------------------------------------------
void TFTestIF::edMaxTestTimeKeyPress(TObject *Sender, char &Key)   // golden :1126-1131  ACTIVE
{
    if(OnlyNumberAndDotInPut(Key)==false)
        Key=0;                                                                  // DEVIATION D-2: golden `Key=NULL;` on a char
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 0, true, 0.0, 5000.0); //kevin 20200804 add min time
}

// ---------------------------------------------------------------------------
void TFTestIF::Edit1KeyPress(TObject *Sender, char &Key)   // golden :1133-1137  ACTIVE
{
    (void)Sender;
    if(OnlyNumberInPut(Key)==false)
        Key=0;                                                                  // DEVIATION D-2: golden `Key=NULL;` on a char
}

// ---------------------------------------------------------------------------
void TFTestIF::edMaxTestTimeMouseDown(TObject *Sender, TMouseButton Button,
                                      TShiftState Shift, int X, int Y)   // golden :1185-1189  ACTIVE
{
    (void)Button; (void)Shift; (void)X; (void)Y;
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 0, true, 0.0, 5000.0); //kevin 20200804 add min time
}

// ---------------------------------------------------------------------------
void TFTestIF::Edit1MouseDown(TObject *Sender, TMouseButton Button,
                              TShiftState Shift, int X, int Y)   // golden :1191-1195  ACTIVE
{
    (void)Button; (void)Shift; (void)X; (void)Y;
    fQwertyKey->ShowQwertyKey((TEdit*) Sender, N_INTEGER, 0, true, 100000, 1);
}

// ---------------------------------------------------------------------------
//  golden :1289-1292  ACTIVE.  ⚠ `TestIF` here is the SYSTEM_TEST_IF DATA
//  global (cprod.h:2576, body cprod.cpp = ht9045_globals) -- NOT this form.
//  See the NAME COLLISION block in forms/fTesterIF.h.
void TFTestIF::AntiSignalCBoxClick(TObject *Sender)
{
    (void)Sender;
    TestIF.bAntiSignal = AntiSignalCBox->Checked;
}

// ---------------------------------------------------------------------------
void TFTestIF::cbRs232TypeChange(TObject *Sender)   // golden :1294-1300  ACTIVE
{
    (void)Sender;
    if(cbRs232Type->ItemIndex==0)
        gbRs232BinCount->Visible=false;
    else
        gbRs232BinCount->Visible=true;
}

// ---------------------------------------------------------------------------
void TFTestIF::edMaxBinCountMouseDown(TObject *Sender, TMouseButton Button,
                                      TShiftState Shift, int X, int Y)   // golden :1368-1372  ACTIVE
{
    (void)Button; (void)Shift; (void)X; (void)Y;
    fQwertyKey->ShowQwertyKey((TEdit*) Sender, N_INTEGER, 0, true, 15, 255);    //Steven 20121116 : RS232回傳的最大Bin數  //kevin 20140317 for 256 bin
}

// ---------------------------------------------------------------------------
void TFTestIF::edTempOffsetCountMouseDown(TObject *Sender, TMouseButton Button,
                                          TShiftState Shift, int X, int Y)   // golden :1374-1378  ACTIVE
{
    (void)Button; (void)Shift; (void)X; (void)Y;
    fQwertyKey->ShowQwertyKey((TEdit*) Sender, N_INTEGER, 0, true, 1, 50);
}

// ---------------------------------------------------------------------------
void TFTestIF::edInitialMaxTestClick(TObject *Sender)   // golden :1380-1403  ACTIVE
{
    if(CUSTOMER_CODE==CC_ASE_M)
    {
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 60, 9999);
    }
    else if(CUSTOMER_CODE==CC_AMKOR_Korea ||
            CUSTOMER_CODE==CC_SCK)
    {
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0);
    }
    else if(CUSTOMER_CODE==CC_GIGAS)
    {
        fQwertyKey->ShowQwertyKey((TEdit*) Sender, N_DOUBLE, 2, true, 0, 50000);//Jimmychiu 20260302 : Ben要求最大測試時間限制50000
    }
    else if(IniConfig.bVTESTFunction==true)                                     //RogerYang 20260513 : Add for VTEST
    {
        fQwertyKey->ShowQwertyKey((TEdit*) Sender, N_DOUBLE, 2, true, 0, 36000);
    }
    else
    {
        fQwertyKey->ShowQwertyKey((TEdit*) Sender, N_DOUBLE, 2, true, 0, 15000);//Richard 20230322 : limit修改  //JerryYang 20231016 : 9999->15000
    }
}

// ---------------------------------------------------------------------------
void TFTestIF::edtInitialDelay_1Click(TObject *Sender)   // golden :1405-1408  ACTIVE
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 0.0, 3000.0);
}

// ---------------------------------------------------------------------------
void TFTestIF::edtInitStartDelayClick(TObject *Sender)   // golden :1410-1416  ACTIVE
{
    if(CosFunction.bHiSiliconFunction && CUSTOMER_CODE==CC_ASE_KaohSiung)       //kevin 20191227 add  Initial Start Delay Time 需卡下限, 最小30秒.  //JerryYang 20220330 : 只有海思才卡
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 30.0, 3000.0);
    else
        fQwertyKey->ShowQwertyKey((TEdit*) Sender, N_DOUBLE, 2, false);
}

// ---------------------------------------------------------------------------
//  golden :1516-1519  ACTIVE.  NOTE the effect is currently nil: `Show()` on
//  this wave's sibling forms/fTesterTCP facade is a port-only no-op (that
//  file's DEVIATION D-2). The body is live; the behaviour is not.
void TFTestIF::btTesterTCPShowClick(TObject *Sender)
{
    (void)Sender;
    fTesterTCP->Show();
}

// ---------------------------------------------------------------------------
void TFTestIF::edTCPIP_PortClick(TObject *Sender)   // golden :1530-1533  ACTIVE
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_PORT);
}

// ---------------------------------------------------------------------------
void TFTestIF::edPowerSwitchDelayClick(TObject *Sender)   // golden :1535-1538  ACTIVE
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 0, atof(edMaxTestTime->Text.c_str()));
}

// ---------------------------------------------------------------------------
void TFTestIF::edMaxBIOSWaitTimeClick(TObject *Sender)   // golden :1540-1543  ACTIVE
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 0, ((atoi(edMaxTestTime->Text.c_str())-2)>0)?(atof(edMaxTestTime->Text.c_str())-2):0);
}

// ---------------------------------------------------------------------------
void TFTestIF::edMinTestTimeClick(TObject *Sender)   // golden :1545-1548  ACTIVE
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 0, atof(edSLTMaxTestTime->Text.c_str())-1);
}

// ---------------------------------------------------------------------------
void TFTestIF::edtAutoOnecycleHomStartTimeClick(TObject *Sender)   // golden :1550-1553  ACTIVE
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 10000, 10);
}

// ---------------------------------------------------------------------------
void TFTestIF::edGPIBAddressClick(TObject *Sender)   // golden :1555-1558  ACTIVE
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 64, 1);
}

// ---------------------------------------------------------------------------
void TFTestIF::edtInitialDecCount1Click(TObject *Sender)   // golden :1560-1563  ACTIVE
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 0, 3000);
}

// ---------------------------------------------------------------------------
void TFTestIF::edPurgeAirClick(TObject *Sender)   // golden :1565-1568  ACTIVE
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 0, 10000);
}

// ===========================================================================
//  GATE REGISTER -- translated golden bodies, deliberately NOT COMPILED.
// ===========================================================================

#if 0 // GATE (F-D7) golden cTesterIF.cpp:37 -- the file-scope `static bool
      // bflag`. Its ONLY users are (F-8) rgInterfaceTypeClick and (F-10)
      // FormClose, both gated, so keeping it live would be a
      // defined-but-unused static under this target's -Wall -Wextra.
      // DEVIATION D-7.
static bool bflag=false;
#endif // GATE (F-D7)

#if 0 // GATE (F-1)a -- golden :42, the ctor's `InitcbDIOType(false);`.
      // [T] on (F-2) AND a static-init hazard: (F-2) can reach
      // Application->Terminate(), which from a static-init ctor would kill the
      // process before main().
{
    InitcbDIOType(false);                                                       //Steven 20101008 : 連按三下會不見
}
#endif // GATE (F-1)a

#if 0 // GATE (F-1)b -- golden :46-66, the ctor's two CUSTOMER_CODE blocks.
      // [M] `->Hint` does not exist on vclcompat::TControl (Controls.h:213-280)
      // and `cbGPIBType->Refresh()` has no port; plus reading CUSTOMER_CODE
      // (ht9045_globals) from a static-init ctor is forbidden by
      // docs/KNOWLEDGE.md after the fLaserSensor incident.
{
    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                         //kevin 20210924 GPIB 顯示type 命名
    {
        cbGPIBType->Clear();
        cbGPIBType->Items->Add("Advan type1");
        cbGPIBType->Items->Add("256 Bin");
        cbGPIBType->Items->Add("16 Bin");
        cbGPIBType->Items->Add("32 Bin");
        cbGPIBType->Items->Add("SPEA Type");
        cbGPIBType->Items->Add("16 Bin GS");
        cbGPIBType->Items->Add("32 Bin GS");
        cbGPIBType->Items->Add("Advan T6577");
        cbGPIBType->Items->Add("Qrovo Protocol");
        cbGPIBType->Refresh();
    }

    if(CUSTOMER_CODE==CC_Greatek)                                               //Sam 20201209 : Default Recipe ChangeLog
    {
        edInitialMaxTest->Hint  ="Tester_Initial Max Time";
        edMaxTestTime->Hint     ="Tester_Wait Max Time";
        edStartDelayTime->Hint  ="Tester_Start Delay Time";
    }
}
#endif // GATE (F-1)b

#if 0 // GATE (F-2) InitcbDIOType -- golden :69-106. [M] `Application->
      // MessageBox` / `Application->Terminate` (:97-98): `Application` has NO
      // port anywhere in this tree (20260828 scan).
void TFTestIF::InitcbDIOType(bool bAlarm)                            //Steven 20101008 : 連按三下會不見
{
    cbDIOType->Clear();
    WIN32_FIND_DATA filedata;                                                   // Structure for file data
    HANDLE filehandle;                                                          // Handle for searching
    AnsiString szFileName;
    filehandle=FindFirstFile((DIOCFGPath + "*.ini").c_str(), &filedata);        //Steven 20100706 Start: 改用讀取資料夾的方式

    if(filehandle!=INVALID_HANDLE_VALUE)
    {
        do
        {
            if((filedata.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)!=0 ||
                strcmp(filedata.cFileName, ".")==0 ||                           /* 不處理隱藏檔及 . 跟 .. */
                strcmp(filedata.cFileName, "..")==0)
                continue;

            if(ExtractFileExt(filedata.cFileName).LowerCase()==".ini")          // 若找到的檔案的副檔名是 .ini
            {
                szFileName=ChangeFileExt(ExtractFileName(filedata.cFileName), "");  // 取出檔名，其實就是把副檔名設成""
                cbDIOType->Items->Add(szFileName);                              // 將檔名加到 cbDIOType
            }
        } while(FindNextFile(filehandle, &filedata));
    }
    else
    {
        if(bAlarm==false)
        {
            Application->MessageBox("DIO data has been lossed, please check!!", "DIO Data loss", MB_OK|MB_TOPMOST);
            Application->Terminate();
        }
        else
        {
            ShowMyMessage("DIO data has been lossed, please check!!", "DIO資料遺失，請檢查！！");
        }
    }
    FindClose(filehandle);
}
#endif // GATE (F-2)

#if 0 // GATE (F-3) FormShow -- golden :108-334 (227L). Five independent
      // reasons: [T] on (F-2):117 and (F-5):118; [L] fSecurity (cSecurity.cpp
      // = ht9045_sm:2168) at :141/:316; [L] SW[SwPurgeAir] (myswitch.cpp =
      // ht9045_io:1240) at :325; [L] CheckTTLBoardBitMode (csystem.cpp =
      // ht9045_sm:2131) at :148; [M] fMain->CheckCanChangeRealDummy (:130),
      // fSCKART->sInfo_Stage (:266), PageControl1->ActivePage (:264/:268/:272).
void TFTestIF::FormShow(TObject *Sender)
{
    AnsiString S;
    PageControl1->Enabled=true;                                                 //Steven 20220523 : 避免SetFocus失敗
    palTime->Enabled=true;
    gbRs232BinCount->Visible=false;
    PageControl1->ActivePageIndex=0;                                            //Jimmychiu 20221205 直接SetFocus會異常
    rbTemp->SetFocus();

    InitcbDIOType(TestIF_File.iTestType==TTL_MODE);                             //Steven 20101008 : 連按三下會不見   //Steven 20210723 : 修正不是TTL模式會Alarm
    ReadTestIFFile();
    DoIniDataToForm();
    S.sprintf("Tester Interface  ''%s''  ",GetLastOpenFN());
    Caption=S;

    ShowTTLState();
    AntiSignalCBox->Visible = LastSet.bUseNewTTLBoard;                          //Eliot 2008_02_01 start
    AntiSignalCBox->Checked = TestIF.bAntiSignal;                               //Eliot 2008_02_01 end

    Left=100;                                                                   //Steven 20091103
    Top=10;                                                                     //Steven 20091103

    if(fMain->CheckCanChangeRealDummy()==false)                                 //jou 981125 start : 防止測試中被改變
    {
        grpIFType->Enabled=false;
    }
    else if(CosFunction.bLotStartLockCriticalPara && RunInfo.bLotStart)         //JerryYang 20220311 : ATP鎖定Critical parameter
    {
        if(bAuthCriticalPara[21])
            grpIFType->Enabled=false;
    }
    else
    {
        grpIFType->Enabled=fSecurity->Insufficient(19,false);                   //half
    }

    if(rgInterfaceType->ItemIndex==0)                                           //Isaac 20210309 :TTL RS232兩塊板子，TTL mode
    {
        if((TTL_CARD_TYPE==2 || TTL_CARD_TYPE==3))                              //Isaac 20210309 :TTL RS232兩塊板子
        {
            CheckTTLBoardBitMode();
        }
        else
        {
            if(TestIF.iTestMode> _8Site2X4 &&                                   //jou 2010-01-06 start : 4 Site 以上不支援 TTL
               CosFunction.bTTLCanUse8Site==false)                              //Alick 20161011 (Steven) : TTL支援8Site
            {
                ShowMyMessage("TTL only support less then 4 site, please select GPIB.", "4 Site 以上不支援 TTL，請選擇GPIB");
            }
        }
    }

    if(IniConfig.bSPILFunction==true)                                           //JerryYang 20220927 : SPIL lot info
    {
        tsRT->TabVisible=true;
        tsEQC->TabVisible=true;
    }
    else
    {
        tsRT->TabVisible=false;
        tsEQC->TabVisible=false;
    }

//    if(CUSTOMER_CODE!=CC_AMD_M)                                               //Ifor 20190927 : add AMD 強制開啟AMD Function
//    {
//        chkAMDFunction->Visible=(CosFunction.bAMDFunction==true);             //Ifor 20191025 : AMD 開放2D Barcode資料格式選項切換
//    }
//    chkAMDFunction->Visible=(CosFunction.bAMDFunction==true);                 //JerryYang 20200422 2DID format選項改用下拉選單

    if(CosFunction.bFTRTDiffInitStartDelayTime)                                 //JerryYang 20220330 : FT RT可設定不同的initial delay time
    {
        rgTime1->Caption="FT";
    }
    else
    {
        rgTime1->Caption="Time1";
    }

    fShow=true;
#ifdef SOFT_SIMULTE
    cbForEgistec->Visible=(CosFunction.bArm2ForFingerPrintTest);                //Steven 20140922 : Arm2當作指紋測試
#else
    cbForEgistec->Visible=false;
#endif

    cbRs232TypeChange(this);                                                    //Steven 20100722
    PageControl1->ActivePageIndex=0;
    gbInitialDelay->Visible=(LastSet.iTemperature==Tempture_Hot ||              //Steven 20171219 : 只有加熱模式要跑Initial Delay
                             LastSet.iTemperature==Tempture_AmbientHot ||       //kevin 20180811 (Steven) : add 恆溫控制
                             CUSTOMER_CODE==CC_KYEC_LEE);                       //Ifor 20180109 (Steven) : add KYEC 常高溫都要跑Initial Delay

    if(CUSTOMER_CODE==CC_SCC ||                                                 //ChungHung 20130621 add SCK RMS
       CUSTOMER_CODE==CC_SCK)                                                   //Steven 20101221
    {
        if(IniConfig.bEnableRms && AccessLevel<=iDefEngineerLevel)              //jou 2014-06-19 Security Have 5 Level 1->iDefEngineerLevel
        {
            PageControl1->Enabled=false;                                        //kevin 20180307 change
            rgInterfaceType->Enabled=false;
            tsDio->Enabled=false;
            tsGpib->Enabled=false;
            tsRs232->Enabled=false;
        }
        else
        {
            PageControl1->Enabled=true;                                         //kevin 20180307 change
            rgInterfaceType->Enabled=true;
            tsDio->Enabled=true;
            tsGpib->Enabled=true;
            tsRs232->Enabled=true;
        }
    }
    else if(CUSTOMER_CODE==CC_ASE_CL)                                           //JerryYang 20170301 (wei) ASE_CL權限控制
    {
        palTime->Enabled=(AccessLevel>=LevelSet.AccessLevel[91]);
        palStartDelay->Enabled=(AccessLevel>=LevelSet.AccessLevel[131]);        //JerryYang 20170301 Start Delay權限設定
    }
    else
    {
        //grpTestTimeSetting->Enabled=(AccessLevel>=LevelSet.AccessLevel[91]);  //Steven 20140114 : 測試時間設定的權限
        PageControl1->Enabled=(AccessLevel>=LevelSet.AccessLevel[91]);          //kevin 20180307 change
    }

    Label49             ->Visible=IniConfig.bInitialStartDelayCount;            //jou 2012-11-30 高溫動作下希望增加顆數記數,在前幾顆下壓到Socket後,都要等待Delay time
    edtInitStartDelay   ->Visible=IniConfig.bInitialStartDelayCount;            //jou 2012-11-30 高溫動作下希望增加顆數記數,在前幾顆下壓到Socket後,都要等待Delay time
    Label1              ->Visible=IniConfig.bInitialStartDelayCount;            //jou 2012-11-30 高溫動作下希望增加顆數記數,在前幾顆下壓到Socket後,都要等待Delay time
    Label2              ->Visible=IniConfig.bInitialStartDelayCount;            //jou 2012-11-30 高溫動作下希望增加顆數記數,在前幾顆下壓到Socket後,都要等待Delay time
    edtInitStartDelayCT ->Visible=IniConfig.bInitialStartDelayCount;            //jou 2012-11-30 高溫動作下希望增加顆數記數,在前幾顆下壓到Socket後,都要等待Delay time
    Label52             ->Visible=IniConfig.bInitialStartDelayCount;            //jou 2012-11-30 高溫動作下希望增加顆數記數,在前幾顆下壓到Socket後,都要等待Delay time

    grpTestingStopTime->Visible=IniConfig.bEnableTestingNeedStopAllMotor && IniConfig.bI24TestingNeedStopAllMotor;   //jou 2013-09-25 Testing Need Stop All Motor

    Label8              ->Visible=CosFunction.bEnableAfterTestedDelay;          //ChungHung 20140730 add for ATK function after tested delay time
    edtAfterTestedDelay ->Visible=CosFunction.bEnableAfterTestedDelay;          //ChungHung 20140730 add for ATK function after tested delay time
    Label12             ->Visible=CosFunction.bEnableAfterTestedDelay;          //ChungHung 20140730 add for ATK function after tested delay time

    if(CUSTOMER_CODE==CC_KYEC_LEE || CUSTOMER_CODE==CC_KYEC_XILINX)             //20140320 wei   KYEC 低於權限顯示不能修改
    {
        if(AccessLevel<LevelSet.AccessLevel[19])
        {
            grpIFType->Enabled=false;
            //grpTestTimeSetting->Enabled=false;
            PageControl1->Enabled=false;                                        //kevin 20180307 change

            grpTestingStopTime->Enabled=false;
        }
        else
        {
            grpIFType->Enabled=true;
            //grpTestTimeSetting->Enabled=true;
            PageControl1->Enabled=true;                                         //kevin 20180307 change
            grpTestingStopTime->Enabled=true;
        }
    }

    if(IniConfig.bSPILFunction==true && iRunStartMode==RT)                      //JerryYang 20220330 : FT RT可設定不同的initial delay time   //Steven 20221129 : Add SPIL
    {
        PageControl1->ActivePage=tsRT;
    }
    else if(IniConfig.bSPILFunction==true && fSCKART->sInfo_Stage.Pos("QC")==1) //Steven 20221129 : Add SPIL
    {
        PageControl1->ActivePage=tsEQC;
    }
    else
    {
        PageControl1->ActivePage=rgTime1;
    }

    rgTime2->TabVisible=(CUSTOMER_CODE==CC_ASE_KaohSiung || CUSTOMER_CODE==CC_AMD_M);             //kevin 20180307 add Steven 20170215 (wei): 其他客戶不顯示
    rgPurgeAir->TabVisible=(CUSTOMER_CODE==CC_ASE_KaohSiung ||                  //kevin 20180928 add
                            IniConfig.bSPILFunction);                           //JerryYang 20181115 (Steven) : SPIL要求吹氣功能

    gbInitialDelay->Enabled=(AccessLevel>=LevelSet.AccessLevel[123]);           //jou 2016-05-12 新增 "Use initial start delay in socket" 權限管控

    if(CUSTOMER_CODE==CC_TSMC_TAINAN)                                           //wei 20151228 No FullSite delay
        pnlNoFullsite->Visible=true;
    else
        pnlNoFullsite->Visible=false;

    if(CosFunction.bAfterInitialDelayUseOtherArm==true)                         //Jerryyang 20180607 (wei) : Initail delay後不直接測試,換用另外一支arm下去測,用意是避免device溫度被socket帶走後直接測試容易fail
        cbUseOtherArmToTestAfterInitialDelay->Visible=true;
    else
        cbUseOtherArmToTestAfterInitialDelay->Visible=false;

    if(IniConfig.bI39SpiroxTesterLotEnd)                                        //JerryYang 20170706 (Steven) JCET通知測試機lot end功能改為by工作檔
        cbSpiroxTesterLotEnd->Visible=true;
    else
        cbSpiroxTesterLotEnd->Visible=false;
//    gbInitialDelay->Visible=(CUSTOMER_CODE!=CC_AMKOR_China);                  //jou 2014-08-25 安靠要求不使用 Use initial start delay in socket     //Steven 20150416 : 客戶說要打開

    //Steven 20190313 : Initial Start Delay use different setting in FT and RT
    //==>
    lblDelayTime->Left=(IniConfig.bI13InitStartDelayHasFTandRT)?356:320;
    lblRT->Visible=(IniConfig.bI13InitStartDelayHasFTandRT);
    lblFT->Visible=(IniConfig.bI13InitStartDelayHasFTandRT);
    edtInitialDelay_1_RT->Visible=(IniConfig.bI13InitStartDelayHasFTandRT);
    edtInitialDelay_2_RT->Visible=(IniConfig.bI13InitStartDelayHasFTandRT);
    edtInitialDelay_3_RT->Visible=(IniConfig.bI13InitStartDelayHasFTandRT);
    edtInitialDelay_4_RT->Visible=(IniConfig.bI13InitStartDelayHasFTandRT);
    edtInitialDelay_5_RT->Visible=(IniConfig.bI13InitStartDelayHasFTandRT);
    edtInitialDelay_6_RT->Visible=(IniConfig.bI13InitStartDelayHasFTandRT);
    edtInitialDelay_7_RT->Visible=(IniConfig.bI13InitStartDelayHasFTandRT);
    edtInitialDelay_8_RT->Visible=(IniConfig.bI13InitStartDelayHasFTandRT);
    edtInitialDelay_9_RT->Visible=(IniConfig.bI13InitStartDelayHasFTandRT);
    edtInitialDelay_10_RT->Visible=(IniConfig.bI13InitStartDelayHasFTandRT);
    //<==
    //Steven 20190313 : Initial Start Delay use different setting in FT and RT
    if(CUSTOMER_CODE==CC_Greatek)
    {
        grpIFType->Enabled=fSecurity->Insufficient(27, false);                  //JimmyChiu 20211228 : InterFace Type
    }
    else if(CUSTOMER_CODE==CC_GIGAS)                                            //Isaac 20210821 : 全智要求定時onecycle回home,start
    {
        cbAutoOnecycleHomStart->Visible=true;
        edtAutoOnecycleHomStartTime->Visible=true;
        labcAutoOnecycleHomStart->Visible=true;
    }

    if(SW[SwPurgeAir].Enable==false)
    {
        rgPurgeAir->TabVisible=false;
    }

    ioldTestType=TestIF_File.iTestType;
    ioldDIOType=TestIF_File.iDioMode;
    //這一行請保持在最下面!!-----------------
//    myLog.Do_Log(Sender, asUser, asLogPath);                                  //Steven 20100629
}
#endif // GATE (F-3)

#if 0 // GATE (F-4) SaveSetupFile -- golden :336-492 (157L). [W] WRITES THE
      // RECIPE: MyForceDirectories(:338), 60+ WriteIniData, AddSpace(:491).
      // PRE-COMMITTED at csystem.cpp:29301 and uPAT_Function.cpp:2069.
void TFTestIF::SaveSetupFile(AnsiString szDir, AnsiString S)         //kevin 20150105
{
    MyForceDirectories(szDir);
    szDir+="\\Tester.Data";

    WriteIniData(szDir, "Mode",     "Tester Type",                  rgInterfaceType->ItemIndex);

    WriteIniData(szDir, "Time",     "Initial MAX Time",             FormatFloat("0.00", edInitialMaxTest->Text.ToDouble()));         //jou 2011-11-09 增加initial max time set
    WriteIniData(szDir, "Time",     "MAX Time",                     FormatFloat("0.00", edMaxTestTime->Text.ToDouble()));
    WriteIniData(szDir, "Time",     "Dummy Time",                   FormatFloat("0.00", edDummyTestTime->Text.ToDouble()));
    WriteIniData(szDir, "Time",     "Stary Delay",                  FormatFloat("0.00", edStartDelayTime->Text.ToDouble()));

    WriteIniData(szDir, "Time",     "Initial Stary Delay",          FormatFloat("0.00", edtInitStartDelay->Text.ToDouble()));       //jou 2012-11-30 高溫動作下希望增加顆數記數,在前幾顆下壓到Socket後,都要等待Delay time
    WriteIniData(szDir, "Time",     "Initial Stary Delay CT",       edtInitStartDelayCT->Text);                                     //jou 2012-11-30 高溫動作下希望增加顆數記數,在前幾顆下壓到Socket後,都要等待Delay time

    WriteIniData(szDir, "Time",      "RT Initial MAX Time",         edInitialMaxTest_RT->Text);  //JerryYang 20220330 : FT RT可設定不同的initial delay time
    WriteIniData(szDir, "Time",      "RT MAX Time",                 edMaxTestTime_RT->Text);
    WriteIniData(szDir, "Time",      "RT Dummy Time",               edDummyTestTime_RT->Text);
    WriteIniData(szDir, "Time",      "RT Stary Delay",              edStartDelayTime_RT->Text);
    WriteIniData(szDir, "Time",      "RT Initial Stary Delay",      edtInitStartDelay_RT->Text);
    WriteIniData(szDir, "Time",      "RT Initial Stary Delay CT",   edtInitStartDelayCT_RT->Text);

    WriteIniData(szDir, "Time",      "EQC Initial MAX Time",        edInitialMaxTest_EQC->Text);
    WriteIniData(szDir, "Time",      "EQC MAX Time",                edMaxTestTime_EQC->Text);
    WriteIniData(szDir, "Time",      "EQC Dummy Time",              edDummyTestTime_EQC->Text);
    WriteIniData(szDir, "Time",      "EQC Stary Delay",             edStartDelayTime_EQC->Text);
    WriteIniData(szDir, "Time",      "EQC Initial Stary Delay",     edtInitStartDelay_EQC->Text);
    WriteIniData(szDir, "Time",      "EQC Initial Stary Delay CT",  edtInitStartDelayCT_EQC->Text);

    WriteIniData(szDir, "Time",     "Init Wait Time",               FormatFloat("0.00", edtInitWaitTime->Text.ToDouble()));         //jou 2013-09-25 Testing Need Stop All Motor
    WriteIniData(szDir, "Time",     "Testing Wait Time",            FormatFloat("0.00", edtTestingWaitTime->Text.ToDouble()));      //jou 2013-09-25 Testing Need Stop All Moto

    //ChungHung 20140730 add for ATK function after tested delay time
    WriteIniData(szDir, "Time",     "After Tested Delay",           FormatFloat("0.00", edtAfterTestedDelay->Text.ToDouble()));

    WriteIniData(szDir, "DIO",      "Type",         cbDIOType->ItemIndex);
    WriteIniData(szDir, "DIO",      "TypeName",     cbDIOType->Text);
    WriteIniData(szDir, "DIO",      "Anti Signal",  AntiSignalCBox->Checked);
    //Steven 20161208 : ART need to set to 32binGS for ATK
    //==>
    if(USE_AUTO_RETEST==eartInstall       &&
       CosFunction.bUseSCKART==true       &&
       IniConfig.bA10_AutoReTest==true    &&
       IniConfig.bA10TestModeForART==true)
    {
        //用Config的,而且不複寫原本工作檔
    }
    else
    {
        WriteIniData(szDir, "GP-IB",     "Type",         cbGPIBType->ItemIndex);
    }
    //<==
    //Steven 20161208 : ART need to set to 32binGS for ATK

    WriteIniData(szDir, "GP-IB",     "Address",      edGPIBAddress->Text);
    WriteIniData(szDir, "GP-IB",     "2DIDFormat", rg2DID_Format->ItemIndex);
//    WriteIniData(szDir, "GP-IB",     "Intel 2DID",   cbIntel2D->Checked);     //JerryYang 20200422 2DID format選項改用下拉選單

    WriteIniData(szDir, "RS-232C",   "Type",         cbRs232Type->ItemIndex);
    WriteIniData(szDir, "RS-232C",   "Bin Count",    edMaxBinCount->Text);
//    WriteIniData(szDir, "RS-232C",   "Baud Rate",    rgBaudRate->ItemIndex);
    WriteIniData(szDir, "RS-232C",   "BaudRate",     cbbBaudRate->Text);        //Steven 20211224 : baudrate改用數字
    WriteIniData(szDir, "RS-232C",   "Bit Length",   rgBitLength->ItemIndex);
    WriteIniData(szDir, "RS-232C",   "Stop Bit",     rgStopBit->ItemIndex);
    WriteIniData(szDir, "RS-232C",   "Parity",       rgParity->ItemIndex);

//    WriteIniData(szDir, "RS-232C",   "NeedVSOT",     cb_NeedSendVSOT->Checked);
    if(CosFunction.bArm2ForFingerPrintTest)                                     //Steven 20140922 : Arm2當作指紋測試
        WriteIniData(szDir, "RS-232C",   "ForEgisTecTest",   cbForEgistec->Checked);
    else
        WriteIniData(szDir, "RS-232C",   "ForEgisTecTest",   false);

    //ChungHung 20140425 add for TSMC Device
    WriteIniData(szDir, "InitialMode",       "bEveryFirstDeviceUseInitialDelay",            cbEveryFirstDeviceUseInitialDelay->Checked);

    if(CosFunction.bAfterInitialDelayUseOtherArm)                               //Jerryyang 20180607 (wei) : Initail delay後不直接測試,換用另外一支arm下去測,用意是避免device溫度被socket帶走後直接測試容易fail
        WriteIniData(szDir, "InitialMode",       "bUseOtherArmToTestAfterInitialDelay",         cbUseOtherArmToTestAfterInitialDelay->Checked);
    else
        WriteIniData(szDir, "InitialMode",       "bUseOtherArmToTestAfterInitialDelay",         false);

    WriteIniData(szDir, "InitialMode",       "bAfterShowAlarmMessageUseInitialDelay",       cbAfterShowAlarmMessageUseInitialDelay->Checked);
    WriteIniData(szDir, "InitialMode",       "bWhenHappenTestedTimeBelowUseInitialDelay",   cbWhenHappenTestedTimeBelowUseInitialDelay->Checked);
    WriteIniData(szDir, "InitialMode",       "iEveryFirstDeviceUseInitialDelay",            edBelowSec->Text);
    WriteIniData(szDir, "InitialMode",       "iInitialDelay",                               FormatFloat("0.00", edtInitialDelay_1->Text.ToDouble()));    //ChungHung 20141210 add for SCK want to every event have delay
    WriteIniData(szDir, "InitialMode",       "iInitialDelay_2",                             FormatFloat("0.00", edtInitialDelay_2->Text.ToDouble()));    //ChungHung 20141210 add for SCK want to every event have delay
    WriteIniData(szDir, "InitialMode",       "iInitialDelay_3",                             FormatFloat("0.00", edtInitialDelay_3->Text.ToDouble()));    //ChungHung 20141210 add for SCK want to every event have delay
    WriteIniData(szDir, "InitialMode",       "iInitialDelay_4",                             FormatFloat("0.00", edtInitialDelay_4->Text.ToDouble()));    //ChungHung 20141210 add for SCK want to every event have delay
    WriteIniData(szDir, "InitialMode",       "iInitialDelay_5",                             FormatFloat("0.00", edtInitialDelay_5->Text.ToDouble()));    //ChungHung 20141210 add for SCK want to every event have delay
    WriteIniData(szDir, "InitialMode",       "iInitialDelay_6",                             FormatFloat("0.00", edtInitialDelay_6->Text.ToDouble()));    //ChungHung 20150526 add for ATK want to even stop over will use initial delay

    WriteIniData(szDir, "InitialMode",       "bWhenPressStopOverUseInitialDelay",           cbWhenPressStopOverUseInitialDelay->Checked);   //ChungHung 20150526 add for ATK want to even stop over will use initial delay
    WriteIniData(szDir, "InitialMode",       "iWhenPressStopOver",    edOverSec->Text);                                                     //ChungHung 20150526 add for ATK want to even stop over will use initial delay

    WriteIniData(szDir, "InitialMode",       "bWhenNoFullSiteUseInitialDelay",              cbWhenNoFullSiteUseInitialDelay->Checked);      //wei 20151228 No FullSite delay
    WriteIniData(szDir, "InitialMode",       "iInitialDelay_7",                             edtInitialDelay_7->Text);                       //wei 20151228 No FullSite delay

    //ChungHung 20141017 add for SCK Add for TSMC Device
    WriteIniData(szDir, "InitialMode",       "bAfterAutoCleanFunctionUseInitialDelay",      cbAfterAutoCleanFunctionUseInitialDelay->Checked);
    //ChungHung 20141210 add for SCK want to after Open HeatDoor have delay time
    WriteIniData(szDir, "InitialMode",       "bAfterOpenHeatDoorUseInitialDelay",           cbAfterOpenHeatDoorUseInitialDelay->Checked);

    WriteIniData(szDir, "InitialMode",       "bTestFinishToNextTestOver",                   cbTestFinishToNextTestOver->Checked);                                       //kevin 20160310 測試完成到下一次完成超過所設時間須啟動
    WriteIniData(szDir, "InitialMode",       "iTestFinishToNextTestOver",                   FormatFloat("0.00", iTestFinishToNextTestOver->Text.ToDouble()));           //kevin 20160310 測試完成到下一次完成超過所設時間須啟動
    WriteIniData(szDir, "InitialMode",       "iInitialDelay_8",                             FormatFloat("0.00", edtInitialDelay_8->Text.ToDouble()));                   //kevin 20160310 測試完成到下一次完成超過所設時間須啟動

    WriteIniData(szDir, "InitialMode",       "bOTDUnlockDelay",                             cbOTDUnlockDelay->Checked);                                                 //Steven 20160818 : OTD打開Delay
    WriteIniData(szDir, "InitialMode",       "iInitialDelay_9",                             FormatFloat("0.00", edtInitialDelay_9->Text.ToDouble()));                   //Steven 20160818 : OTD打開Delay
    WriteIniData(szDir, "InitialMode",       "bTestStartToNextTestStart",                   cbTestStartToNextTestStart->Checked);                                       //kevin 20181031 : SOT 超過時間Delay
    WriteIniData(szDir, "InitialMode",       "dTeststartToNextTestStart",                   FormatFloat("0.00", iTeststartToNextTestStart->Text.ToDouble()));           //kevin 20181031 : SOT 超過時間Delay
    WriteIniData(szDir, "InitialMode",       "dInitialDelay_10",                            FormatFloat("0.00", edtInitialDelay_10->Text.ToDouble()));                  //kevin 20181031 : SOT 超過時間Delay

    WriteIniData(szDir, "InitialMode",       "iEnStartDelayCount",                          iStartDelayCount->ItemIndex );                                              //kevin 20180307  使用幾次 延遲遞增減 開始執行送訊號delay
    WriteIniData(szDir, "InitialMode",       "dInitialStartDelayDec1",                      FormatFloat("0.00", edtInitialDec1->Text.ToDouble()));                      //kevin 20180307 每次等待時間
    WriteIniData(szDir, "InitialMode",       "iStartDelayCount1",                           edtInitialDecCount1->Text);                                                 //kevin 20180307 第幾個 Count開始執行送訊號delay
    WriteIniData(szDir, "InitialMode",       "dInitialStartDelayDec2",                      FormatFloat("0.00", edtInitialDec2->Text.ToDouble()));                      //kevin 20180307 每次等待時間
    WriteIniData(szDir, "InitialMode",       "iStartDelayCount2",                           edtInitialDecCount2->Text);                                                 //kevin 20180307 第幾個 Count開始執行送訊號delay
    WriteIniData(szDir, "InitialMode",       "dInitialStartDelayDec3",                      FormatFloat("0.00", edtInitialDec3->Text.ToDouble()));                      //kevin 20180307 每次等待時間
    WriteIniData(szDir, "InitialMode",       "iStartDelayCount3",                           edtInitialDecCount3->Text);                                                 //kevin 20180307 第幾個 Count開始執行送訊號delay
    WriteIniData(szDir, "InitialMode",       "dInitialStartDelayDec4",                      FormatFloat("0.00", edtInitialDec4->Text.ToDouble()));                      //kevin 20180307 每次等待時間
    WriteIniData(szDir, "InitialMode",       "iStartDelayCount4",                           edtInitialDecCount4->Text);                                                 //kevin 20180307 第幾個 Count開始執行送訊號delay

    if(IniConfig.bI39SpiroxTesterLotEnd)                                        //JerryYang 20170706 (Steven) JCET通知測試機lot end功能改為by工作檔
        WriteIniData(szDir, "InitialMode",   "bSpiroxTesterLotEnd",                         cbSpiroxTesterLotEnd->Checked);
    else
        WriteIniData(szDir, "InitialMode",   "bSpiroxTesterLotEnd",                         false);

    WriteIniData(szDir, "InitialMode",       "bPurgeAir",                                   cbPurgeAir->Checked);                           //kevin 20180928 Contrarct time blower air loadboard
    WriteIniData(szDir, "InitialMode",       "iPurgeAir",                                   edPurgeAir->Text);                              //kevin 20180928 Contrarct time blower air loadboard

    //Steven 20190313 : Initial Start Delay use different setting in FT and RT
    //==>
    WriteIniData(szDir, "InitialMode",       "dInitialDelay_1_RT",              FormatFloat("0.00", edtInitialDelay_1_RT->Text.ToDouble()));
    WriteIniData(szDir, "InitialMode",       "dInitialDelay_2_RT",              FormatFloat("0.00", edtInitialDelay_2_RT->Text.ToDouble()));
    WriteIniData(szDir, "InitialMode",       "dInitialDelay_3_RT",              FormatFloat("0.00", edtInitialDelay_3_RT->Text.ToDouble()));
    WriteIniData(szDir, "InitialMode",       "dInitialDelay_4_RT",              FormatFloat("0.00", edtInitialDelay_4_RT->Text.ToDouble()));
    WriteIniData(szDir, "InitialMode",       "dInitialDelay_5_RT",              FormatFloat("0.00", edtInitialDelay_5_RT->Text.ToDouble()));
    WriteIniData(szDir, "InitialMode",       "dInitialDelay_6_RT",              FormatFloat("0.00", edtInitialDelay_6_RT->Text.ToDouble()));
    WriteIniData(szDir, "InitialMode",       "dInitialDelay_7_RT",              FormatFloat("0.00", edtInitialDelay_7_RT->Text.ToDouble()));
    WriteIniData(szDir, "InitialMode",       "dInitialDelay_8_RT",              FormatFloat("0.00", edtInitialDelay_8_RT->Text.ToDouble()));
    WriteIniData(szDir, "InitialMode",       "dInitialDelay_9_RT",              FormatFloat("0.00", edtInitialDelay_9_RT->Text.ToDouble()));
    WriteIniData(szDir, "InitialMode",       "dInitialDelay_10_RT",             FormatFloat("0.00", edtInitialDelay_10_RT->Text.ToDouble()));
    //<==
    //Steven 20190313 : Initial Start Delay use different setting in FT and RT

    WriteIniData(szDir, "Tester TCPIP",       "Address",                                    edTCPIP_Address->Text);                         //wei 20211027 open short TCP/IP
    WriteIniData(szDir, "Tester TCPIP",       "Port",                                       edTCPIP_Port->Text);                            //wei 20211027 open short TCP/IP
    WriteIniData(szDir, "InitialMode",        "bTTLUseASEJPMode",                           cbASEJPMode->Checked); //Frank 20220408 Add TTL ASE_JP Mode
    if(CUSTOMER_CODE==CC_GIGAS)                                                 //Isaac 20210821 : 全智要求定時onecycle回home,start
    {
        WriteIniData(szDir, "Time",    "bAutoOnecycleHomStart", cbAutoOnecycleHomStart->Checked);       //Isaac 20210821 : 全智要求定時onecycle回home,start
        WriteIniData(szDir, "Time",    "iAutoOnecycleHomStartTime", edtAutoOnecycleHomStartTime->Text); //Isaac 20210821 : 全智要求定時onecycle回home,start
    }

    WriteIniData(szDir, "Time",      "bUseSocketHeating",    cbUseSocketHeating->Checked);//Ztex 2024.09.07 Add Use Socket Heating
    WriteIniData(szDir, "Time",      "iUseSocketHeating",    edt_UseSocketHeating->Text); //Ztex 2024.09.07 Add Use Socket Heating
    AddSpace(szDir);
}
#endif // GATE (F-4)

#if 0 // GATE (F-6) CheckRs232StandardIni -- golden :496-558 (63L). [W] writes
      // four keys into a SECOND product's config file
      // (D:\RS232Standard\System\Setup.ini) at :512/:524/:536/:550, and [M]
      // fMain->CloseGpibProgram (:556). golden's own //==> / //<== comment
      // sandwich (:494-495, :559-560) is kept.
//Sam 20181219 : Handler 與 Rs232Standard 設定同步
//==>
void TFTestIF::CheckRs232StandardIni()
{
    int iTemp1, iTemp2;
    AnsiString szDir;
    bool bTemp;

    bTemp=false;
    szDir="D:\\RS232Standard\\System\\Setup.ini";

    //BaudRate
    iTemp1=ReadIniData(szDir, "COMPort", "BaudRate", 9600);                     //4800,7200,9600,14400,19200      //Steven 20211224 : baudrate改用數字
    iTemp2=TestIF_File.Rs232_Data.Baud_Rate;

    if(iTemp1!=iTemp2)
    {
        bTemp=true;
        WriteIniData(szDir, "COMPort", "BaudRate", iTemp2);
    }

    //ByteSize
    iTemp1=ReadIniData(szDir, "COMPort", "ByteSize", 7);                        //0:5,1:6,2:7,3:8
    if(TestIF_File.Rs232_Data.Bit_Length==0)                                    //0:7,1:8
        iTemp2=2;
    else
        iTemp2=3;
    if(iTemp1!=iTemp2)
    {
        bTemp=true;
        WriteIniData(szDir, "COMPort", "ByteSize", iTemp2);
    }

    //StopBits
    iTemp1=ReadIniData(szDir, "COMPort", "StopBits", 1);                        //0:1,1:1.5,2:2
    if(TestIF_File.Rs232_Data.Stop_Bit==0)                                      //0:1,1:2
        iTemp2=0;
    else
        iTemp2=2;
    if(iTemp1!=iTemp2)
    {
        bTemp=true;
        WriteIniData(szDir, "COMPort", "StopBits", iTemp2);
    }

    //Parity
    iTemp1=ReadIniData(szDir, "COMPort", "Parity"  , 2);                        //0:None,1:Odd,2:Even,3:Mark,4:Space
    if(TestIF_File.Rs232_Data.Parity==0)                                        //0:Even,1:Odd,2:None
        iTemp2=2;
    else if(TestIF_File.Rs232_Data.Parity==1)
        iTemp2=1;
    else
        iTemp2=0;
    if(iTemp1!=iTemp2)
    {
        bTemp=true;
        WriteIniData(szDir,"COMPort", "Parity", iTemp2);
    }

    bIsResetRs232Standard=bTemp;
    if(bIsResetRs232Standard)                                                   //Steven 20220323 : 針對RS232測試的參數被改變時, 要通知小程式一起改
    {
        fMain->CloseGpibProgram(__FUNC__);
    }
}
//<==
//Sam 20181219 : Handler 與 Rs232Standard 設定同步
#endif // GATE (F-6)

#if 0 // GATE (F-5) ReadTestIFFile -- golden :562-977 (416L, the file's largest
      // body). [W] HIDDEN WRITE x3 despite the name: WriteIniData at :576,
      // :824 and :892, plus ATKRecipeInfo->SaveFile() at :973. Also [L]
      // fShowBinSelect (cShowBinSelect.cpp = ht9045_sm:2162) at :931, [T] on
      // (F-6) at :947, [M] fTesterTCP->TimerTCPIPConnect/TimerProcessTCPData/
      // ClientSocket_TCPIP (:584-615, deliberately not members of this wave's
      // forms/fTesterTCP facade) and fLotInfo->labTCPIPSimulate (:586/:616).
void TFTestIF::ReadTestIFFile()
{
    int iTemp;
    AnsiString S="";
    S=GetLastOpenFN();
    AnsiString szDir="";//, szDir2;
    szDir=DataPath+S;
    szDir+="\\Tester.Data";
//    szDir2.sprintf("%s%s\\Temperature.Data", DataPath, S);

    TestIF_File.iTestType                =ReadIniData(szDir, "Mode", "Tester Type",     0);

    if(TestIF_File.iTestType>rgInterfaceType->Items->Count-1)
    {
        WriteIniData(szDir, "Mode", "Tester Type",       1);
        TestIF_File.iTestType            =GPIB_MODE;
    }

    if(TestIF_File.iTestType==TCP_IP_MODE)                                      //wei 20211027 open short TCP/IP
    {
        if(LastSet.iTester==OFF_LINE)                                           //wei 20211027 open short TCP/IP
        {
            fTesterTCP->TimerTCPIPConnect->Enabled=false;
            fTesterTCP->TimerProcessTCPData->Enabled=false;
            fLotInfo->labTCPIPSimulate->Visible=true;
        }
        else
        {
            fTesterTCP->TimerTCPIPConnect->Enabled=true;
            fTesterTCP->TimerProcessTCPData->Enabled=true;
        }

        if(IniConfig.bSECS_GEM_OneCycle && IniConfig.bN07_EnableSecsLotCheck)   //Steven 20230710 : OS測試機的工作檔也要上傳
        {
            if(DirectoryExists(IniConfig.sN07_6OSRecipePath)==false)
            {
                S.sprintf("Net Drive [N07-6] %s does not exists. Please make the folder first.", IniConfig.sN07_6OSRecipePath);
                ShowMyMessage(S, "請設定OS測試機的網路硬碟");
            }
        }
        else if(IniConfig.bN06_CopyTesterFile)                                  //Steven 20250327 : OS測試機的工作檔也要上傳
        {
            if(DirectoryExists(IniConfig.asN06_TesterPath)==false)
            {
                S.sprintf("Net Drive [I55] %s does not exists. Please make the folder first.", IniConfig.asN06_TesterPath);
                ShowMyMessage(S, "請設定OS測試機的網路硬碟");
            }
        }
    }
    else
    {
        fTesterTCP->TimerTCPIPConnect->Enabled=false;
        fTesterTCP->TimerProcessTCPData->Enabled=false;
        fTesterTCP->ClientSocket_TCPIP->Close();
        fLotInfo->labTCPIPSimulate->Visible=false;
    }
    fLotInfo->SettsChipAdvVisible();                                            //Steven 20250515 : 整合Open Short測試報表

    TestIF_File.iMaxTime                 =ReadIniData(szDir, "Time", "MAX Time",      0.0);
    TestIF_File.iInitialMaxTime          =ReadIniData(szDir, "Time", "Initial MAX Time", TestIF_File.iMaxTime);    //jou 2011-11-09 增加initial max time set
    TestIF_File.iDummyTime               =ReadIniData(szDir, "Time", "Dummy Time",    0.0);
    TestIF_File.dStartDelayTime          =ReadIniData(szDir, "Time", "Stary Delay",   0.0);

    TestIF_File.dInitStartDelayTime      =ReadIniData(szDir, "Time", "Initial Stary Delay",   0.0); //jou 2012-11-30 高溫動作下希望增加顆數記數,在前幾顆下壓到Socket後,都要等待Delay time
    TestIF_File.iInitStartDelayTimeCT    =ReadIniData(szDir, "Time", "Initial Stary Delay CT",  0); //jou 2012-11-30 高溫動作下希望增加顆數記數,在前幾顆下壓到Socket後,都要等待Delay time
    if(CosFunction.bFTRTDiffInitStartDelayTime)                                 //JerryYang 20220330 : FT RT可設定不同的initial delay time
    {
        TestIF_File.iFTMaxTime                 =ReadIniData(szDir, "Time", "MAX Time",                        TestIF_File.iMaxTime              );
        TestIF_File.iFTInitialMaxTime          =ReadIniData(szDir, "Time", "Initial MAX Time",                TestIF_File.iInitialMaxTime       );
        TestIF_File.iFTDummyTime               =ReadIniData(szDir, "Time", "Dummy Time",                      TestIF_File.iDummyTime            );
        TestIF_File.dFTStartDelayTime          =ReadIniData(szDir, "Time", "Stary Delay",                     TestIF_File.dStartDelayTime       );
        TestIF_File.dFTInitStartDelayTime      =ReadIniData(szDir, "Time", "Initial Stary Delay",             TestIF_File.dInitStartDelayTime   );
        TestIF_File.iFTInitStartDelayTimeCT    =ReadIniData(szDir, "Time", "Initial Stary Delay CT",          TestIF_File.iInitStartDelayTimeCT );

        TestIF_File.iRTMaxTime                 =ReadIniData(szDir, "Time", "RT MAX Time",               TestIF_File.iMaxTime              );
        TestIF_File.iRTInitialMaxTime          =ReadIniData(szDir, "Time", "RT Initial MAX Time",       TestIF_File.iInitialMaxTime       );    //jou 2011-11-09 增加initial max time set
        TestIF_File.iRTDummyTime               =ReadIniData(szDir, "Time", "RT Dummy Time",             TestIF_File.iDummyTime            );
        TestIF_File.dRTStartDelayTime          =ReadIniData(szDir, "Time", "RT Stary Delay",            TestIF_File.dStartDelayTime       );
        TestIF_File.dRTInitStartDelayTime      =ReadIniData(szDir, "Time", "RT Initial Stary Delay",    TestIF_File.dInitStartDelayTime   ); //jou 2012-11-30 高溫動作下希望增加顆數記數,在前幾顆下壓到Socket後,都要等待Delay time
        TestIF_File.iRTInitStartDelayTimeCT    =ReadIniData(szDir, "Time", "RT Initial Stary Delay CT", TestIF_File.iInitStartDelayTimeCT ); //jou 2012-11-30 高溫動作下希望增加顆數記數,在前幾顆下壓到Socket後,都要等待Delay time

        TestIF_File.iEQCMaxTime                 =ReadIniData(szDir, "Time", "EQC MAX Time",                 TestIF_File.iMaxTime              );
        TestIF_File.iEQCInitialMaxTime          =ReadIniData(szDir, "Time", "EQC Initial MAX Time",         TestIF_File.iInitialMaxTime       );
        TestIF_File.iEQCDummyTime               =ReadIniData(szDir, "Time", "EQC Dummy Time",               TestIF_File.iDummyTime            );
        TestIF_File.dEQCStartDelayTime          =ReadIniData(szDir, "Time", "EQC Stary Delay",              TestIF_File.dStartDelayTime       );
        TestIF_File.dEQCInitStartDelayTime      =ReadIniData(szDir, "Time", "EQC Initial Stary Delay",      TestIF_File.dInitStartDelayTime   );
        TestIF_File.iEQCInitStartDelayTimeCT    =ReadIniData(szDir, "Time", "EQC Initial Stary Delay CT",   TestIF_File.iInitStartDelayTimeCT );
        if(iRunStartMode==RT)
        {
            TestIF_File.iMaxTime                   =TestIF_File.iRTMaxTime;
            TestIF_File.iInitialMaxTime            =TestIF_File.iRTInitialMaxTime;
            TestIF_File.iDummyTime                 =TestIF_File.iRTDummyTime;
            TestIF_File.dStartDelayTime            =TestIF_File.dRTStartDelayTime;
            TestIF_File.dInitStartDelayTime        =TestIF_File.dRTInitStartDelayTime;
            TestIF_File.iInitStartDelayTimeCT      =TestIF_File.iRTInitStartDelayTimeCT;
        }
        else if(fSCKART->sInfo_Stage.Pos("QC")==1)
        {
            TestIF_File.iMaxTime                   =TestIF_File.iEQCMaxTime;
            TestIF_File.iInitialMaxTime            =TestIF_File.iEQCInitialMaxTime;
            TestIF_File.iDummyTime                 =TestIF_File.iEQCDummyTime;
            TestIF_File.dStartDelayTime            =TestIF_File.dEQCStartDelayTime;
            TestIF_File.dInitStartDelayTime        =TestIF_File.dEQCInitStartDelayTime;
            TestIF_File.iInitStartDelayTimeCT      =TestIF_File.iEQCInitStartDelayTimeCT;
        }
        else
        {
            TestIF_File.iMaxTime                         =TestIF_File.iFTMaxTime;
            TestIF_File.iInitialMaxTime                  =TestIF_File.iFTInitialMaxTime;
            TestIF_File.iDummyTime                       =TestIF_File.iFTDummyTime;
            TestIF_File.dStartDelayTime                  =TestIF_File.dFTStartDelayTime;
            TestIF_File.dInitStartDelayTime              =TestIF_File.dFTInitStartDelayTime;
            TestIF_File.iInitStartDelayTimeCT            =TestIF_File.iFTInitStartDelayTimeCT;
        }
    }

    TestIF_File.dInitWaitTime            =ReadIniData(szDir, "Time", "Init Wait Time",   1.0);  //jou 2013-09-25 Testing Need Stop All Motor
    TestIF_File.dTestingWaitTime         =ReadIniData(szDir, "Time", "Testing Wait Time",3.0);  //jou 2013-09-25 Testing Need Stop All Motor

    //ChungHung 20140730 add for ATK function after tested delay time
    TestIF_File.dAfterTestedDelay        =ReadIniData(szDir, "Time", "After Tested Delay", 0.0);

    TestIF_File.iDioMode                 =ReadIniData(szDir, "DIO", "Type",             0);

    if(TestIF_File.iDioMode<0)
        TestIF_File.iDioMode=0;

    //ChungHung 20140425 add for TSMC Device
    TestIF_File.bEveryFirstDeviceUseInitialDelay            =ReadIniData(szDir, "InitialMode",       "bEveryFirstDeviceUseInitialDelay",          false);

    if(CosFunction.bAfterInitialDelayUseOtherArm)                               //Jerryyang 20180607 (wei) : Initail delay後不直接測試,換用另外一支arm下去測,用意是避免device溫度被socket帶走後直接測試容易fail
        TestIF_File.bUseOtherArmToTestAfterInitialDelay     =ReadIniData(szDir, "InitialMode",       "bUseOtherArmToTestAfterInitialDelay",       false);
    else
        TestIF_File.bUseOtherArmToTestAfterInitialDelay     =false;

    TestIF_File.bAfterShowAlarmMessageUseInitialDelay       =ReadIniData(szDir, "InitialMode",       "bAfterShowAlarmMessageUseInitialDelay",     false);
    TestIF_File.bWhenHappenTestedTimeBelowUseInitialDelay   =ReadIniData(szDir, "InitialMode",       "bWhenHappenTestedTimeBelowUseInitialDelay", false);
    TestIF_File.iEveryFirstDeviceUseInitialDelay            =ReadIniData(szDir, "InitialMode",       "iEveryFirstDeviceUseInitialDelay",          1.00);

    #ifdef SOFT_SIMULTE                                                         //Steven 20241108 : 軟體模擬有做五秒就好
    TestIF_File.iInitialDelay                               =5;
    TestIF_File.iInitialDelay_2                             =5;
    TestIF_File.iInitialDelay_3                             =5;
    TestIF_File.iInitialDelay_4                             =5;
    TestIF_File.iInitialDelay_5                             =5;
    TestIF_File.iInitialDelay_6                             =5;
    TestIF_File.iInitialDelay_7                             =5;
    TestIF_File.iInitialDelay_8                             =5;
    TestIF_File.iInitialDelay_9                             =5;
    TestIF_File.dInitialDelay_10                            =5;
    #else
    TestIF_File.iInitialDelay                               =ReadIniData(szDir, "InitialMode",       "iInitialDelay",            1.00);     //ChungHung 20141210 add for SCK want to every event have delay
    TestIF_File.iInitialDelay_2                             =ReadIniData(szDir, "InitialMode",       "iInitialDelay_2",          1.00);     //ChungHung 20141210 add for SCK want to every event have delay
    TestIF_File.iInitialDelay_3                             =ReadIniData(szDir, "InitialMode",       "iInitialDelay_3",          1.00);     //ChungHung 20141210 add for SCK want to every event have delay
    TestIF_File.iInitialDelay_4                             =ReadIniData(szDir, "InitialMode",       "iInitialDelay_4",          1.00);     //ChungHung 20141210 add for SCK want to every event have delay
    TestIF_File.iInitialDelay_5                             =ReadIniData(szDir, "InitialMode",       "iInitialDelay_5",          1.00);     //ChungHung 20141210 add for SCK want to every event have delay
    TestIF_File.iInitialDelay_6                             =ReadIniData(szDir, "InitialMode",       "iInitialDelay_6",          1.00);     //ChungHung 20150526 add for ATK want to even stop over will use initial delay
    TestIF_File.iInitialDelay_7                             =ReadIniData(szDir, "InitialMode",       "iInitialDelay_7",          1.00);     //wei 20151228 No FullSite delay
    TestIF_File.iInitialDelay_8                             =ReadIniData(szDir, "InitialMode",       "iInitialDelay_8",          1.00);     //kevin 20160310 測試完成到下一次完成超過所設時間須啟動
    TestIF_File.iInitialDelay_9                             =ReadIniData(szDir, "InitialMode",       "iInitialDelay_9",          1.00);     //Steven 20160818 : OTD打開Delay
    TestIF_File.dInitialDelay_10                            =ReadIniData(szDir, "InitialMode",       "dInitialDelay_10",         0.0);      //kevin 20181031 (Steven) : add SOT start SRQ41 send next SRQ 41 wait time
    #endif

    if(CosFunction.bHiSiliconFunction && CUSTOMER_CODE==CC_ASE_KaohSiung)       //kevin 20191227 add  Initial Start Delay Time 需卡下限, 最小30秒.
    {
        if(TestIF_File.dInitStartDelayTime<30)                                  //kevin 20200110
        {
            TestIF_File.dInitStartDelayTime=30;
            TestIF_File.iInitialDelay=30;
        }
    }

    TestIF_File.bOTDUnlockDelay                             =ReadIniData(szDir, "InitialMode",       "bOTDUnlockDelay",                             false);     //Steven 20160818 : OTD打開Delay
    TestIF_File.bWhenPressStopOverUseInitialDelay           =ReadIniData(szDir, "InitialMode",       "bWhenPressStopOverUseInitialDelay",           false);     //ChungHung 20150526 add for ATK want to even stop over will use initial delay
    TestIF_File.iWhenPressStopOver                          =ReadIniData(szDir, "InitialMode",       "iWhenPressStopOver",                          1.00);      //ChungHung 20150526 add for ATK want to even stop over will use initial delay
    TestIF_File.bWhenNoFullSiteUseInitialDelay              =ReadIniData(szDir, "InitialMode",       "bWhenNoFullSiteUseInitialDelay",              false);     //wei 20151228 No FullSite delay
    TestIF_File.bAfterAutoCleanFunctionUseInitialDelay      =ReadIniData(szDir, "InitialMode",       "bAfterAutoCleanFunctionUseInitialDelay",      false);
    TestIF_File.bTestFinishToNextTestOver                   =ReadIniData(szDir, "InitialMode",       "bTestFinishToNextTestOver",                   false);     //kevin 20160310 測試完成到下一次完成超過所設時間須啟動
    TestIF_File.iTestFinishToNextTestOver                   =ReadIniData(szDir, "InitialMode",       "iTestFinishToNextTestOver",                   1.00);      //kevin 20160310 測試完成到下一次完成超過所設時間須啟動
    TestIF_File.bAfterOpenHeatDoorUseInitialDelay           =ReadIniData(szDir, "InitialMode",       "bAfterOpenHeatDoorUseInitialDelay",           false);     //ChungHung 20141210 add for SCK want to after Open HeatDoor have delay time
    TestIF_File.bTestStartToNextTestStart                   =ReadIniData(szDir, "InitialMode",       "bTestStartToNextTestStart",                   false);     //kevin 20181031 (Steven) : add SOT start SRQ41 send next SRQ 41
    TestIF_File.dTeststartToNextTestStart                   =ReadIniData(szDir, "InitialMode",       "dTeststartToNextTestStart",                   1.00);      //kevin 20181031 (Steven) : add SOT start SRQ41 send next SRQ 41  over time

    //Steven 20190313 : Initial Start Delay use different setting in FT and RT
    //==>
    TestIF_File.dInitialDelay_1_RT                          =ReadIniData(szDir, "InitialMode",       "dInitialDelay_1_RT",              0.0);
    TestIF_File.dInitialDelay_2_RT                          =ReadIniData(szDir, "InitialMode",       "dInitialDelay_2_RT",              0.0);
    TestIF_File.dInitialDelay_3_RT                          =ReadIniData(szDir, "InitialMode",       "dInitialDelay_3_RT",              0.0);
    TestIF_File.dInitialDelay_4_RT                          =ReadIniData(szDir, "InitialMode",       "dInitialDelay_4_RT",              0.0);
    TestIF_File.dInitialDelay_5_RT                          =ReadIniData(szDir, "InitialMode",       "dInitialDelay_5_RT",              0.0);
    TestIF_File.dInitialDelay_6_RT                          =ReadIniData(szDir, "InitialMode",       "dInitialDelay_6_RT",              0.0);
    TestIF_File.dInitialDelay_7_RT                          =ReadIniData(szDir, "InitialMode",       "dInitialDelay_7_RT",              0.0);
    TestIF_File.dInitialDelay_8_RT                          =ReadIniData(szDir, "InitialMode",       "dInitialDelay_8_RT",              0.0);
    TestIF_File.dInitialDelay_9_RT                          =ReadIniData(szDir, "InitialMode",       "dInitialDelay_9_RT",              0.0);
    TestIF_File.dInitialDelay_10_RT                         =ReadIniData(szDir, "InitialMode",       "dInitialDelay_10_RT",             0.0);
    //<==
    //Steven 20190313 : Initial Start Delay use different setting in FT and RT

    TestIF_File.iEnStartDelayCount                          =ReadIniData(szDir, "InitialMode",       "iEnStartDelayCount",          0);             //kevin 20180307  使用幾次 延遲遞增減 開始執行送訊號delay
    TestIF_File.dInitialStartDelayDec[0]                    =ReadIniData(szDir, "InitialMode",       "dInitialStartDelayDec1",   0.00);             //kevin 20180307 每次等待時間
    TestIF_File.iStartDelayCount[0]                         =ReadIniData(szDir, "InitialMode",       "iStartDelayCount1",           0);             //kevin 20180307 第幾個 Count開始執行送訊號delay
    TestIF_File.dInitialStartDelayDec[1]                    =ReadIniData(szDir, "InitialMode",       "dInitialStartDelayDec2",   0.00);             //kevin 20180307 每次等待時間
    TestIF_File.iStartDelayCount[1]                         =ReadIniData(szDir, "InitialMode",       "iStartDelayCount2",           0);             //kevin 20180307 第幾個 Count開始執行送訊號delay
    TestIF_File.dInitialStartDelayDec[2]                    =ReadIniData(szDir, "InitialMode",       "dInitialStartDelayDec3",   0.00);             //kevin 20180307 每次等待時間
    TestIF_File.iStartDelayCount[2]                         =ReadIniData(szDir, "InitialMode",       "iStartDelayCount3",           0);             //kevin 20180307 第幾個 Count開始執行送訊號delay
    TestIF_File.dInitialStartDelayDec[3]                    =ReadIniData(szDir, "InitialMode",       "dInitialStartDelayDec4",   0.00);             //kevin 20180307 每次等待時間
    TestIF_File.iStartDelayCount[3]                         =ReadIniData(szDir, "InitialMode",       "iStartDelayCount4",           0);             //kevin 20180307 第幾個 Count開始執行送訊號delay

    if(IniConfig.bI39SpiroxTesterLotEnd==true)                                  //JerryYang 20170706 (Steven) JCET通知測試機lot end功能改為by工作檔
        TestIF_File.bSpiroxTesterLotEnd                     =ReadIniData(szDir, "InitialMode",       "bSpiroxTesterLotEnd",         false);
    else
        TestIF_File.bSpiroxTesterLotEnd=false;

    TestIF_File.bPurgeAirAfterContract                      =ReadIniData(szDir, "InitialMode",       "bPurgeAir",   false);                             //kevin 20180928 Contrarct time blower air loadboard
    TestIF_File.iPurgeAirContract                           =ReadIniData(szDir, "InitialMode",       "iPurgeAir",   0);   //kevin 20180928 Contrarct time blower air loadboard

    TestIF_File.asTester_Address                            =ReadIniData(szDir, "Tester TCPIP",      "Address",                  AnsiString("172.16.8.150"));           //wei 20211027 open short TCP/IP
    TestIF_File.iTester_Port                                =ReadIniData(szDir, "Tester TCPIP",      "Port",                     6000);                                 //wei 20211027 open short TCP/IP

    TestIF_File.bTTLUseASEJPMode                            =ReadIniData(szDir, "InitialMode",       "bTTLUseASEJPMode",          false);   //Frank 20220408 Add TTL ASE_JP Mode

    fTesterTCP->edTCPIP_Address->Text                       =TestIF_File.asTester_Address;          //wei 20211027 open short TCP/IP
    fTesterTCP->edTCPIP_Port   ->Text                       =TestIF_File.iTester_Port;              //wei 20211027 open short TCP/IP

    if(CUSTOMER_CODE==CC_GIGAS)                                                 //Isaac 20210821 : 全智要求定時onecycle回home,start
    {
        TestIF_File.bAutoOnecycleHomStart                       =ReadIniData(szDir, "Time",              "bAutoOnecycleHomStart",   false);     //Isaac 20210821 : 全智要求定時onecycle回home,start
        TestIF_File.iAutoOnecycleHomStartTime                   =ReadIniData(szDir, "Time",              "iAutoOnecycleHomStartTime",   0);     //Isaac 20210821 : 全智要求定時onecycle回home,start
    }
    else
    {
        TestIF_File.bAutoOnecycleHomStart=false;
        TestIF_File.iAutoOnecycleHomStartTime=0;
    }
    //Steven 20101008 : Start
    if(CheckIniData(szDir, "DIO", "TypeName")==false)
    {
        WIN32_FIND_DATA filedata1;                                              // Structure for file data
        HANDLE filehandle1;                                                     // Handle for searching
        AnsiString szFileName;
        filehandle1=FindFirstFile((DIOCFGPath + "*.ini").c_str(), &filedata1);
        TStringList *MyList=new TStringList();
        if(filehandle1!=INVALID_HANDLE_VALUE)
        {
            do
            {
                /* 不處理隱藏檔及 . 跟 .. */
                if((filedata1.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)!=0 ||
                    strcmp(filedata1.cFileName, ".")==0 ||
                    strcmp(filedata1.cFileName, "..")==0)
                    continue;

                if(ExtractFileExt(filedata1.cFileName).LowerCase()==".ini")     // 若找到的檔案的副檔名是 .ini
                {
                    szFileName=ChangeFileExt(ExtractFileName(filedata1.cFileName), "");  // 取出檔名，其實就是把副檔名設成""
                    MyList->Add(szFileName);
                }
            } while(FindNextFile(filehandle1, &filedata1));
            FindClose(filehandle1);
        }

        TestIF_File.sDioName=MyList->Strings[TestIF_File.iDioMode];
        WriteIniData(szDir, "DIO",       "TypeName",     TestIF_File.sDioName);
        MyList->Clear();                                                        //Ifor 20170603 (wei) TStringList 刪除前先 Clean
        delete MyList;
    }
    else
    {
        TestIF_File.sDioName             =ReadIniData(szDir, "DIO", "TypeName", AnsiString(""));
    }
    //Steven 20101008 : End

    TestIF_File.bAntiSignal              =ReadIniData(szDir, "DIO", "Anti Signal",      0);
    TestIF_File.iGpibMode                =ReadIniData(szDir, "GP-IB", "Type",           0);
    //Steven 20161208 : ART need to set to 32binGS for ATK
    //==>
    if(USE_AUTO_RETEST==eartInstall       &&
       CosFunction.bUseSCKART==true       &&
       IniConfig.bA10_AutoReTest==true    &&
       IniConfig.bA10TestModeForART==true)
    {
        TestIF_File.iGpibMode=IniConfig.iA10TestModeForART;
    }
    //<==
    //Steven 20161208 : ART need to set to 32binGS for ATK

    //fSCKART->SetGPIBVersion(fSCKART->edtVersion->Text);                       //Steven 20230811 : 針對GPIB發送Version命令, 統一處理          //Jimmychiu 20231205 : Mark SetGPIBVersion function

    TestIF_File.iGpibAddress             =ReadIniData(szDir, "GP-IB", "Address",        0);
    //Ifor 20191025 : AMD 開放2D Barcode資料格式選項切換
//    if(CUSTOMER_CODE==CC_AMD_M) //Ifor 20190927 : add AMD 強制開啟AMD Function
//    {
//        TestIF_File.bAMDFunction=true;
//    }
//    else
//    {
//        TestIF_File.bAMDFunction         =ReadIniData(szDir, "GP-IB", "AMD Function",   false);
//    }

    if(CUSTOMER_CODE==CC_AMD_M)                                                 //Ifor 20191025 : AMD 預設開啟AMD Function
    {
        TestIF_File.bAMDFunction        =ReadIniData(szDir, "GP-IB", "AMD Function",   true);
    }
    else
    {
        TestIF_File.bAMDFunction        =ReadIniData(szDir, "GP-IB", "AMD Function",   false);
    }

    if(CUSTOMER_CODE==CC_AMD_M)                                                 //JerryYang 20200422 2DID format選項改用下拉選單
    {
        TestIF_File.i2DIDFormat         =ReadIniData(szDir, "GP-IB", "2DIDFormat",   1);
    }
    else
    {
        TestIF_File.i2DIDFormat         =ReadIniData(szDir, "GP-IB", "2DIDFormat",   int(TestIF_File.bAMDFunction));
    }

    TestIF_File.iRs232Mode              =(eRs232Mode)ReadIniData(szDir, "RS-232C", "Type",         0);

    TestIF_File.iRs232MaxBinCount       =ReadIniData(szDir, "RS-232C", "Bin Count",    32);    //Steven 20121116 : RS232回傳的最大Bin數

    if(CheckIniData(szDir, "RS-232C",   "BaudRate")==false)
    {
        iTemp                           =ReadIniData(szDir, "RS-232C", "Baud Rate",    0);
        if(iTemp==0)//0:19200,1:9600,2:4800
            TestIF_File.Rs232_Data.Baud_Rate=19200;
        else if(iTemp==1)
            TestIF_File.Rs232_Data.Baud_Rate=9600;
        else
            TestIF_File.Rs232_Data.Baud_Rate=4800;
        WriteIniData(szDir, "RS-232C",   "BaudRate", TestIF_File.Rs232_Data.Baud_Rate);
    }
    else
    {
        TestIF_File.Rs232_Data.Baud_Rate =ReadIniData(szDir, "RS-232C", "BaudRate",     0);
    }
    TestIF_File.Rs232_Data.Bit_Length    =ReadIniData(szDir, "RS-232C", "Bit Length",   0);
    TestIF_File.Rs232_Data.Stop_Bit      =ReadIniData(szDir, "RS-232C", "Stop Bit",     0);
    TestIF_File.Rs232_Data.Parity        =ReadIniData(szDir, "RS-232C", "Parity",       0);

//    TestIF_File.bRS232NeedSendVSOT       =ReadIniData(szDir, "RS-232C", "NeedVSOT",     0);
    if(CosFunction.bArm2ForFingerPrintTest)                                     //Steven 20140922 : Arm2當作指紋測試
        TestIF_File.bForEgisTecTest      =ReadIniData(szDir, "RS-232C", "ForEgisTecTest",   false);
    else
        TestIF_File.bForEgisTecTest      =false;

    if(TestIF_File.iTestType==RS232_MODE &&
       TestIF_File.iRs232Mode==eRs23232Bin)                                     //Steven 20121112 : RS232支援32Bin b8080 --> bSupport32Bin
        iTestBinCount=TestIF_File.iRs232MaxBinCount;
    //else
    //    iTestBinCount=15;
    else if(TestIF_File.iTestType==GPIB_MODE &&
            TestIF_File.iGpibMode==InterfaceType_256Bin)                        //kevin 20140308 add 256Bin
        iTestBinCount=255;                                                      //Steven 20200602 : 256 --> 255
    else if(TestIF_File.iTestType==GPIB_MODE &&
            (TestIF_File.iGpibMode==InterfaceType_16Bin ||                      //Steven 20140805 ADD 16Bin and 32Bin
             TestIF_File.iGpibMode==InterfaceType_16BinGS))                     //Steven 20161122 (jou) : Add 16 bin GS and 32 bin GS
        iTestBinCount=17;
    else if(TestIF_File.iTestType==GPIB_MODE &&
            (TestIF_File.iGpibMode==InterfaceType_32Bin ||
             TestIF_File.iGpibMode==InterfaceType_32BinGS))                     //Steven 20161122 (jou) : Add 16 bin GS and 32 bin GS
        iTestBinCount=33;
    else
        iTestBinCount=16;

//    if(iTestBinCount>256)  //kevin 20140308
//        iTestBinCount=255;

    if(fShowBinSelect!=NULL)
        fShowBinSelect->ShowInitialString();

    if(IniConfig.bD58UseArm1PickPlaceArm2Test)
    {
        if(TestIF_File.bForEgisTecTest==true)                                   //Steven 20140922 : Arm2當作指紋測試
        {
            fMain->SendMSG_CMD(MSG_CMD_2ArmTestMode);
        }
//        else                                                                  //JerryYang 20240111 : mark掉
//        {
//            fMain->SendMSG_CMD(MSG_CMD_1ArmTestMode);
//        }
    }

    if(TestIF_File.iTestType==RS232_MODE)                                       //Sam 20181219 : Handler 與 Rs232Standard 設定同步
    {
        CheckRs232StandardIni();
    }

    if(Temperature.bBoostFuncttion)                                             //Steven 20180817 : Boost Function
    {
        TestIF_File.bEveryFirstDeviceUseInitialDelay=false;
        TestIF_File.bUseOtherArmToTestAfterInitialDelay=false;
        TestIF_File.bAfterShowAlarmMessageUseInitialDelay=false;
        TestIF_File.bWhenHappenTestedTimeBelowUseInitialDelay=false;
        TestIF_File.bAfterAutoCleanFunctionUseInitialDelay=false;
        TestIF_File.bAfterOpenHeatDoorUseInitialDelay=false;
        TestIF_File.bWhenPressStopOverUseInitialDelay=false;
        TestIF_File.bWhenNoFullSiteUseInitialDelay=false;
        TestIF_File.bTestFinishToNextTestOver=false;
        TestIF_File.bOTDUnlockDelay=false;
    }

    if(TestIF_File.iShuttleMode==1)                                             //JerryYang 20240710 : 單ARM模式不支援換ARM預熱
    {
        TestIF_File.bUseOtherArmToTestAfterInitialDelay=false;
    }

    if(TestIF_File.bTTLUseASEJPMode)                                            //Frank 20220408 Add TTL ASE_JP Mode
    {
        TTLRS232VerCheck=11040801;
    }
    ATKRecipeInfo->SaveFile();                                                  //Steven 20170901 (wei) : For ATK要新增工作檔比對用的檔案
    TestIF_File.bUseSocketHeating    =ReadIniData(szDir, "Time", "bUseSocketHeating",   TestIF_File.bUseSocketHeating );//Ztex 2024.09.07 Add Use Socket Heating
    TestIF_File.iUseSocketHeating    =ReadIniData(szDir, "Time", "iUseSocketHeating",   0 );                            //Ztex 2024.09.07 Add Use Socket Heating
    DoIniDataToForm();
}
#endif // GATE (F-5)

#if 0 // GATE (F-7) DoIniDataToForm -- golden :979-1124 (146L). [T] ONLY, on
      // (F-8) at :1123 (`rgInterfaceTypeClick(this);`, the last statement).
      // Everything above that line is a pure TestIF_File -> own-widget mirror
      // and would be safe -- this is the wave's single biggest deferred win.
      // PRE-COMMITTED at csystem.cpp:29300. Un-gating needs ~70 more widget
      // fields declared first (see forms/fTesterIF.h FIELD LIST).
void TFTestIF::DoIniDataToForm()
{
    rgInterfaceType->ItemIndex      =TestIF_File.iTestType;

    if(CosFunction.bFTRTDiffInitStartDelayTime)                                                     //JerryYang 20220330 : FT RT可設定不同的initial delay time
    {
        edInitialMaxTest->Text          =FormatFloat("0.00",TestIF_File.iFTInitialMaxTime);         //jou 2011-11-09 增加initial max time set
        edMaxTestTime->Text             =FormatFloat("0.00",TestIF_File.iFTMaxTime);
        edDummyTestTime->Text           =FormatFloat("0.00",TestIF_File.iFTDummyTime);
        edStartDelayTime->Text          =FormatFloat("0.00",TestIF_File.dFTStartDelayTime);
        edtInitStartDelay->Text         =FormatFloat("0.00",TestIF_File.dFTInitStartDelayTime);     //jou 2012-11-30 高溫動作下希望增加顆數記數,在前幾顆下壓到Socket後,都要等待Delay time
        edtInitStartDelayCT->Text       =TestIF_File.iFTInitStartDelayTimeCT;                       //jou 2012-11-30 高溫動作下希望增加顆數記數,在前幾顆下壓到Socket後,都要等待Delay time
    }
    else
    {
        edInitialMaxTest->Text          =FormatFloat("0.00",TestIF_File.iInitialMaxTime);           //jou 2011-11-09 增加initial max time set
        edMaxTestTime->Text             =FormatFloat("0.00",TestIF_File.iMaxTime);
        edDummyTestTime->Text           =FormatFloat("0.00",TestIF_File.iDummyTime);
        edStartDelayTime->Text          =FormatFloat("0.00",TestIF_File.dStartDelayTime);

        edtInitStartDelay->Text         =FormatFloat("0.00",TestIF_File.dInitStartDelayTime);       //jou 2012-11-30 高溫動作下希望增加顆數記數,在前幾顆下壓到Socket後,都要等待Delay time
        edtInitStartDelayCT->Text       =TestIF_File.iInitStartDelayTimeCT;                         //jou 2012-11-30 高溫動作下希望增加顆數記數,在前幾顆下壓到Socket後,都要等待Delay time
    }

    edInitialMaxTest_RT->Text          =FormatFloat("0.00",TestIF_File.iRTInitialMaxTime);          //jou 2011-11-09 增加initial max time set
    edMaxTestTime_RT->Text             =FormatFloat("0.00",TestIF_File.iRTMaxTime);
    edDummyTestTime_RT->Text           =FormatFloat("0.00",TestIF_File.iRTDummyTime);
    edStartDelayTime_RT->Text          =FormatFloat("0.00",TestIF_File.dRTStartDelayTime);
    edtInitStartDelay_RT->Text         =FormatFloat("0.00",TestIF_File.dRTInitStartDelayTime);      //jou 2012-11-30 高溫動作下希望增加顆數記數,在前幾顆下壓到Socket後,都要等待Delay time
    edtInitStartDelayCT_RT->Text       =TestIF_File.iRTInitStartDelayTimeCT;                        //jou 2012-11-30 高溫動作下希望增加顆數記數,在前幾顆下壓到Socket後,都要等待Delay time

    edInitialMaxTest_EQC->Text          =FormatFloat("0.00",TestIF_File.iEQCInitialMaxTime);        //jou 2011-11-09 增加initial max time set
    edMaxTestTime_EQC->Text             =FormatFloat("0.00",TestIF_File.iEQCMaxTime);
    edDummyTestTime_EQC->Text           =FormatFloat("0.00",TestIF_File.iEQCDummyTime);
    edStartDelayTime_EQC->Text          =FormatFloat("0.00",TestIF_File.dEQCStartDelayTime);
    edtInitStartDelay_EQC->Text         =FormatFloat("0.00",TestIF_File.dEQCInitStartDelayTime);    //jou 2012-11-30 高溫動作下希望增加顆數記數,在前幾顆下壓到Socket後,都要等待Delay time
    edtInitStartDelayCT_EQC->Text       =TestIF_File.iEQCInitStartDelayTimeCT;                      //jou 2012-11-30 高溫動作下希望增加顆數記數,在前幾顆下壓到Socket後,都要等待Delay time

    edtInitWaitTime->Text           =FormatFloat("0.00", TestIF_File.dInitWaitTime);                //jou 2013-09-25 Testing Need Stop All Motor
    edtTestingWaitTime->Text        =FormatFloat("0.00", TestIF_File.dTestingWaitTime);             //jou 2013-09-25 Testing Need Stop All Motor

    //ChungHung 20140425 add for TSMC Device
    cbEveryFirstDeviceUseInitialDelay->Checked          =TestIF_File.bEveryFirstDeviceUseInitialDelay;
    cbUseOtherArmToTestAfterInitialDelay->Checked       =TestIF_File.bUseOtherArmToTestAfterInitialDelay;               //Jerryyang 20180607 (wei) : Initail delay後不直接測試,換用另外一支arm下去測,用意是避免device溫度被socket帶走後直接測試容易fail
    cbAfterShowAlarmMessageUseInitialDelay->Checked     =TestIF_File.bAfterShowAlarmMessageUseInitialDelay;
    cbWhenHappenTestedTimeBelowUseInitialDelay->Checked =TestIF_File.bWhenHappenTestedTimeBelowUseInitialDelay;
    cbAfterAutoCleanFunctionUseInitialDelay->Checked    =TestIF_File.bAfterAutoCleanFunctionUseInitialDelay;            //ChungHung 20141017 add for SCK Add for TSMC Device
    cbAfterOpenHeatDoorUseInitialDelay->Checked         =TestIF_File.bAfterOpenHeatDoorUseInitialDelay;                 //ChungHung 20141210 add for SCK want to after Open HeatDoor have delay time
    edBelowSec->Text                                    =FormatFloat("0.00", TestIF_File.iEveryFirstDeviceUseInitialDelay);
    edtInitialDelay_1->Text                             =FormatFloat("0.00", TestIF_File.iInitialDelay);                //ChungHung 20141210 add for SCK want to every event have delay
    edtInitialDelay_2->Text                             =FormatFloat("0.00", TestIF_File.iInitialDelay_2);              //ChungHung 20141210 add for SCK want to every event have delay
    edtInitialDelay_3->Text                             =FormatFloat("0.00", TestIF_File.iInitialDelay_3);              //ChungHung 20141210 add for SCK want to every event have delay
    edtInitialDelay_4->Text                             =FormatFloat("0.00", TestIF_File.iInitialDelay_4);              //ChungHung 20141210 add for SCK want to every event have delay
    edtInitialDelay_5->Text                             =FormatFloat("0.00", TestIF_File.iInitialDelay_5);              //ChungHung 20141210 add for SCK want to every event have delay
    edtAfterTestedDelay->Text                           =FormatFloat("0.00", TestIF_File.dAfterTestedDelay);            //ChungHung 20140730 add for ATK function after tested delay time

    cbWhenPressStopOverUseInitialDelay->Checked         =TestIF_File.bWhenPressStopOverUseInitialDelay;                 //ChungHung 20150526 add for ATK want to even stop over will use initial delay
    edOverSec->Text                                     =FormatFloat("0.00", TestIF_File.iWhenPressStopOver);           //ChungHung 20150526 add for ATK want to even stop over will use initial delay
    edtInitialDelay_6->Text                             =FormatFloat("0.00", TestIF_File.iInitialDelay_6);              //ChungHung 20150526 add for ATK want to even stop over will use initial delay

    cbWhenNoFullSiteUseInitialDelay->Checked            =TestIF_File.bWhenNoFullSiteUseInitialDelay;                    //wei 20151228 No FullSite delay
    edtInitialDelay_7->Text                             =FormatFloat("0.00", TestIF_File.iInitialDelay_7);              //wei 20151228 No FullSite delay

    cbTestFinishToNextTestOver->Checked                 =TestIF_File.bTestFinishToNextTestOver;                         //kevin 20160310 測試完成到下一次完成超過所設時間須啟動
    iTestFinishToNextTestOver->Text                     =FormatFloat("0.00", TestIF_File.iTestFinishToNextTestOver);    //kevin 20160310
    edtInitialDelay_8->Text                             =FormatFloat("0.00", TestIF_File.iInitialDelay_8);              //kevin 20160310

    cbOTDUnlockDelay->Checked                           =TestIF_File.bOTDUnlockDelay;                                   //Steven 20160818 : OTD打開Delay
    edtInitialDelay_9->Text                             =FormatFloat("0.00", TestIF_File.iInitialDelay_9);              //Steven 20160818 : OTD打開Delay
    cbTestStartToNextTestStart->Checked                 =TestIF_File.bTestStartToNextTestStart;                         //kevin 20160310 測試完成到下一次完成超過所設時間須啟動
    iTeststartToNextTestStart->Text                     =FormatFloat("0.00", TestIF_File.dTeststartToNextTestStart);    //kevin 20160310
    edtInitialDelay_10->Text                            =FormatFloat("0.00", TestIF_File.dInitialDelay_10);             //kevin 20181031 : SOT 超過時間Delay

    //Steven 20190313 : Initial Start Delay use different setting in FT and RT
    //==>
    edtInitialDelay_1_RT->Text                          =FormatFloat("0.00", TestIF_File.dInitialDelay_1_RT);
    edtInitialDelay_2_RT->Text                          =FormatFloat("0.00", TestIF_File.dInitialDelay_2_RT);
    edtInitialDelay_3_RT->Text                          =FormatFloat("0.00", TestIF_File.dInitialDelay_3_RT);
    edtInitialDelay_4_RT->Text                          =FormatFloat("0.00", TestIF_File.dInitialDelay_4_RT);
    edtInitialDelay_5_RT->Text                          =FormatFloat("0.00", TestIF_File.dInitialDelay_5_RT);
    edtInitialDelay_6_RT->Text                          =FormatFloat("0.00", TestIF_File.dInitialDelay_6_RT);
    edtInitialDelay_7_RT->Text                          =FormatFloat("0.00", TestIF_File.dInitialDelay_7_RT);
    edtInitialDelay_8_RT->Text                          =FormatFloat("0.00", TestIF_File.dInitialDelay_8_RT);
    edtInitialDelay_9_RT->Text                          =FormatFloat("0.00", TestIF_File.dInitialDelay_9_RT);
    edtInitialDelay_10_RT->Text                         =FormatFloat("0.00", TestIF_File.dInitialDelay_10_RT);
    //<==
    //Steven 20190313 : Initial Start Delay use different setting in FT and RT

    edTCPIP_Address->Text                               =TestIF_File.asTester_Address;                                  //wei 20211027 open short TCP/IP
    edTCPIP_Port->Text                                  =TestIF_File.iTester_Port;                                      //wei 20211027 open short TCP/IP
    //Steven 20101008 Start
    cbASEJPMode->Checked                                =TestIF_File.bTTLUseASEJPMode;                                  //Frank 20220408 Add TTL ASE_JP Mode
    if(TestIF_File.sDioName!="")
    {
        for(int i=0; i<cbDIOType->Items->Count; i++)
        {
            if(TestIF_File.sDioName==cbDIOType->Items->Strings[i])
            {
                cbDIOType->ItemIndex=i;
                break;
            }
        }
    }
    else
    {
        if(TestIF_File.iDioMode<0)  //Steven 20110105
            TestIF_File.iDioMode=0;
        cbDIOType->ItemIndex=TestIF_File.iDioMode;
    }
    //Steven 20101008 End

    cbGPIBType->ItemIndex           =TestIF_File.iGpibMode;
    edGPIBAddress->Text             =TestIF_File.iGpibAddress;
    rg2DID_Format->ItemIndex        =TestIF_File.i2DIDFormat;                   //JerryYang 20200422 2DID format選項改用下拉選單

    cbRs232Type->ItemIndex          =TestIF_File.iRs232Mode;
    edMaxBinCount->Text             =TestIF_File.iRs232MaxBinCount;             //Steven 20121116 : RS232回傳的最大Bin數
//    rgBaudRate->ItemIndex           =TestIF_File.Rs232_Data.Baud_Rate;
    cbbBaudRate->Text               =TestIF_File.Rs232_Data.Baud_Rate;          //Steven 20211224 : baudrate改用數字
    rgBitLength->ItemIndex          =TestIF_File.Rs232_Data.Bit_Length;
    rgStopBit->ItemIndex            =TestIF_File.Rs232_Data.Stop_Bit;
    rgParity->ItemIndex             =TestIF_File.Rs232_Data.Parity;
//    cb_NeedSendVSOT->Checked        =(bool)TestIF_File.bRS232NeedSendVSOT;

    cbForEgistec->Checked           =TestIF_File.bForEgisTecTest;                               //Steven 20140922 : Arm2當作指紋測試
    cbSpiroxTesterLotEnd->Checked   =TestIF_File.bSpiroxTesterLotEnd;                           //JerryYang 20170706 (Steven) JCET通知測試機lot end功能改為by工作檔

    iStartDelayCount->ItemIndex     =TestIF_File.iEnStartDelayCount;                            //kevin 20180307  使用幾次 延遲遞增減 開始執行送訊號delay
    edtInitialDec1->Text            =FormatFloat("0.00",TestIF_File.dInitialStartDelayDec[0]);  //kevin 20180307  使用幾次 延遲遞增減 開始執行送訊號delay時間
    edtInitialDecCount1->Text       =TestIF_File.iStartDelayCount[0];                           //kevin 20180307  使用幾次 延遲遞增減 開始執行送訊號delay時間 次數
    edtInitialDec2->Text            =FormatFloat("0.00",TestIF_File.dInitialStartDelayDec[1]);  //kevin 20180307  使用幾次 延遲遞增減 開始執行送訊號delay時間
    edtInitialDecCount2->Text       =TestIF_File.iStartDelayCount[1];                           //kevin 20180307  使用幾次 延遲遞增減 開始執行送訊號delay時間 次數
    edtInitialDec3->Text            =FormatFloat("0.00",TestIF_File.dInitialStartDelayDec[2]);  //kevin 20180307  使用幾次 延遲遞增減 開始執行送訊號delay時間
    edtInitialDecCount3->Text       =TestIF_File.iStartDelayCount[2];                           //kevin 20180307  使用幾次 延遲遞增減 開始執行送訊號delay時間 次數
    edtInitialDec4->Text            =FormatFloat("0.00",TestIF_File.dInitialStartDelayDec[3]);  //kevin 20180307  使用幾次 延遲遞增減 開始執行送訊號delay時間
    edtInitialDecCount4->Text       =TestIF_File.iStartDelayCount[3];                           //kevin 20180307  使用幾次 延遲遞增減 開始執行送訊號delay時間 次數

    cbPurgeAir->Checked             =TestIF_File.bPurgeAirAfterContract;                        //kevin 20180928 Contrarct time blower air loadboard
    edPurgeAir->Text                =TestIF_File.iPurgeAirContract;                              //kevin 20180928 Contrarct time blower air loadboard
    cbAutoOnecycleHomStart->Checked =TestIF_File.bAutoOnecycleHomStart;                         //Isaac 20210821 : 全智要求定時onecycle回home,start
    edtAutoOnecycleHomStartTime->Text=TestIF_File.iAutoOnecycleHomStartTime;                    //Isaac 20210821 : 全智要求定時onecycle回home,start

    cbUseSocketHeating->Checked     =TestIF_File.bUseSocketHeating;             //Ztex 2024.09.07 Add Use Socket Heating
    edt_UseSocketHeating->Text      =TestIF_File.iUseSocketHeating;             //Ztex 2024.09.07 Add Use Socket Heating
    rgInterfaceTypeClick(this);
}
#endif // GATE (F-7)

#if 0 // GATE (F-8) rgInterfaceTypeClick -- golden :1139-1172. [L]
      // Barcode_Reader(bcTesterIF) at :1142 -- body BarcodeReader.cpp, target
      // ht9045_sm (CMakeLists.txt:2215). [T] on (F-9) at :1171. Uses the
      // file-scope `bflag` carried in GATE (F-D7).
void TFTestIF::rgInterfaceTypeClick(TObject *Sender)
{
    if(rgInterfaceType->ItemIndex!=TestIF_File.iTestType &&
       Barcode_Reader(bcTesterIF)==0)                                           // 20140103 wei KYEC Barcode Reader
    {
        rgInterfaceType->ItemIndex=TestIF_File.iTestType;
        return;
    }

    if(bflag==false && rgInterfaceType->ItemIndex!=TestIF_File.iTestType && CosFunction.bTTLCanUse8Site==true)
    {
        rgInterfaceType->ItemIndex=TestIF_File.iTestType;
        bflag=true;
    }

    if(rgInterfaceType->ItemIndex==0)                                           //Isaac 20210309 :TTL RS232兩塊板子，TTL mode
    {
        if((TTL_CARD_TYPE==2 || TTL_CARD_TYPE==3))                              //Isaac 20210309 :TTL RS232兩塊板子
        {
//            CheckTTLBoardBitMode();                                           //Mark 換工作檔會有問題
        }
        else
        {
            if(TestIF.iTestMode>=_8Site2X4 &&                                   //jou 2010-01-06 start : 4 Site 以上不支援 TTL
               fShow==true &&
               CosFunction.bTTLCanUse8Site==false)                              //Alick 20161011 (Steven) : TTL支援8Site
            {
                ShowMyMessage("TTL only support less then 4 site, please select GPIB or RS-232.", "4 Site 以上不支援 TTL，請選擇GPIB 或 RS-232");
            }
        }
    }

    ShowPageControl2(rgInterfaceType->ItemIndex);
}
#endif // GATE (F-8)

#if 0 // GATE (F-9) ShowPageControl2 -- golden :1174-1183. [M]
      // `PageControl2->ActivePage=tsTemp[Index];` (:1180) --
      // vclcompat::TPageControl (Controls.h:497) has ActivePageIndex but no
      // ActivePage, and adding it is a shared-file edit this wave may not make.
void TFTestIF::ShowPageControl2(int Index)
{
    TTabSheet *tsTemp[]={tsDio, tsGpib, tsRs232, tsTCPIP};                      //Steven 20230221 : 修正TCP/IP Tester設定顯示問題
    for(int i=0; i<4; i++)
        tsTemp[i]->TabVisible=false;
    tsTemp[Index]->TabVisible=true;
    PageControl2->ActivePage=tsTemp[Index];
    PageControl2->ActivePageIndex=Index;
    cbRs232TypeChange(this);
}
#endif // GATE (F-9)

#if 0 // GATE (F-10) FormClose -- golden :1197-1217. [T] on (F-5) at :1199;
      // [M] fMain->oldLastiTestMode (:1206) and fMain->CloseGpibProgram
      // (:1212); [L] CheckTTLBoardBitMode (:1211, ht9045_sm).
      // DEVIATION D-4: golden's `(TObject*, TCloseAction&)` becomes `()`
      // because TCloseAction has no port; the transcript keeps golden's
      // signature so the text stays literal.
void TFTestIF::FormClose(TObject *Sender, TCloseAction &Action)
{
    ReadTestIFFile();
    fShow=false;
    bflag=false;

    if(ioldTestType!=TestIF_File.iTestType ||
       ioldDIOType!=cbDIOType->ItemIndex)                                       //Isaac 20200903 :TTL RS232通訊
    {
        fMain->oldLastiTestMode=-1;
    }

    if(TestIF_File.iTestType==TTL_MODE)                                         //Isaac 20210309 :TTL RS232兩塊板子
    {
        CheckTTLBoardBitMode();
        fMain->CloseGpibProgram(__FUNC__);                                      //20210920 Isaac : 兩塊板子必定帶站號
    }

    //這一行請保持在最下面!!-----------------
//    myLog.Do_Log(Sender, asUser, asLogPath);                                  //Steven 20100629
}
#endif // GATE (F-10)

#if 0 // GATE (F-11) ShowTTLState -- golden :1219-1279 (golden's own
      // `//jou 980331` marker at :1219 kept). [M] fDIOFrom->rgStartLogic/
      // rgStartChannel/cbSignalType/rgBinLogic/rgBinBitLength/rgBinDataType
      // (:1226-1259): no TfDIOFrom class and no fDIOFrom global exist in this
      // tree, and DIOInterFaceCFG.{h,cpp} do not exist here at all
      //AI(W906-FW3-BTQ1) 20260828: PARTLY SUPERSEDED -- FW3-BTQ1 新增了 forms/fDIOFrom.{h,cpp}，
      //    所以「no TfDIOFrom class and no fDIOFrom global」已不成立；
      //    但「DIOInterFaceCFG.{h,cpp} do not exist here」**字面上仍為真**
      //    （本 port 的檔名是 forms/fDIOFrom.*，不是 golden 的檔名）。
      //    ⚠ 解閘前先查值從哪來：本 port 沒有載入 .dfm layout，
      //    那六個 ->Items->Strings[i] 讀到的會是空清單。
      // (direct stat, 20260828). [L] CheckTTLBoardBitMode (:1265, ht9045_sm).
//jou 980331
void TFTestIF::ShowTTLState()
{
    lstTTL->Clear();
    AnsiString sString;
    char cStr[128]="";
    lstTTL->Items->Add("[Start Signal]");
    lstTTL->Items->Add("Logic                      : "+fDIOFrom->rgStartLogic->Items->Strings[TTLCfg.iSTLogicMode]);
    lstTTL->Items->Add("Chanel                    : "+fDIOFrom->rgStartChannel->Items->Strings[TTLCfg.iStartType]);
    if(CosFunction.bTTLUseUSec)                                                 //Steven 20180808 (wei) : TTL的時間單位改成microsecond
        sprintf(cStr,        "Pulse Width           : %d μs.\n", TTLCfg.iSTPluseWidth);
    else
        sprintf(cStr,        "Pulse Width           : %d msec.\n", TTLCfg.iSTPluseWidth);
    lstTTL->Items->Add(cStr);

    lstTTL->Items->Add("");

    lstTTL->Items->Add("[DUT Signal]");
    if(TTLCfg.iDutType<0)                                                       //Steven 20100105
        TTLCfg.iDutType=0;
    lstTTL->Items->Add("Type                       : "+fDIOFrom->cbSignalType->Items->Strings[TTLCfg.iDutType]);
    if(CosFunction.bTTLUseUSec)                                                 //Steven 20180808 (wei) : TTL的時間單位改成microsecond
        sprintf(cStr,        "Before ON             : %d μs.", TTLCfg.iDutBfOnTime);
    else
        sprintf(cStr,        "Before ON             : %d msec.", TTLCfg.iDutBfOnTime);
    lstTTL->Items->Add(cStr);
    if(CosFunction.bTTLUseUSec)                                                 //Steven 20180808 (wei) : TTL的時間單位改成microsecond
        sprintf(cStr,        "After OFF              : %d μs.", TTLCfg.iDutAfOffTime);
    else
        sprintf(cStr,        "After OFF              : %d msec.", TTLCfg.iDutAfOffTime);
    lstTTL->Items->Add(cStr);

    lstTTL->Items->Add("");

    lstTTL->Items->Add("[END Signal]");
    lstTTL->Items->Add("Logic                      : "+fDIOFrom->rgBinLogic->Items->Strings[TTLCfg.iCateLogicMode]);

    lstTTL->Items->Add("");

    lstTTL->Items->Add("Channel & Bit Length  : "+fDIOFrom->rgBinBitLength->Items->Strings[TTLCfg.iCateBitLength]);
    lstTTL->Items->Add("Data Type                    : "+fDIOFrom->rgBinDataType->Items->Strings[TTLCfg.iCateDataType]);

    if(rgInterfaceType->ItemIndex==0)                                           //Steven 20100812 : 要在TTL模式下才檢查     //Isaac 20210309 :TTL RS232兩塊板子，TTL mode
    {                                                                           //jou 2010-01-06 start : TTL 8bit & 10bit 不能測試超過 2個Site
        if(TTL_CARD_TYPE==2 || TTL_CARD_TYPE==3)                                //Isaac 20210309 :TTL RS232兩塊板子
        {
            CheckTTLBoardBitMode();
        }
        else
        {
            if(TTLCfg.iCateBitLength==_8Bit    || TTLCfg.iCateBitLength==_10Bit ||
               TTLCfg.iCateBitLength==_10BitPE || TTLCfg.iCateBitLength==_10BitPO )
            {
                if(TestIF.iTestMode >= QualSite1X4)
                {
                    ShowMyMessage("TTL 8bit & 10bit only support less than 2 site!", "TTL 2 Site 以上不支援 8bit & 10bit 模式");
                }
            }
        }
    }
}
#endif // GATE (F-11)

#if 0 // GATE (F-12) cbDIOTypeChange -- golden :1281-1287. [M] fDIOFrom->
      // GetDIOFileName() / ->LoadData(S) (same absence as (F-11)); [T] on
      // (F-11) at :1286.
void TFTestIF::cbDIOTypeChange(TObject *Sender)
{
    AnsiString S;
    S=fDIOFrom->GetDIOFileName();                                               //Steven 20180626 (wei) : TTL設定存到工作檔裡面
    fDIOFrom->LoadData(S);
    ShowTTLState();
}
#endif // GATE (F-12)

#if 0 // GATE (F-13) spbSaveClick -- golden :1302-1356 (55L). [W] on (F-4)
      // SaveSetupFile (:1316) and (F-5) ReadTestIFFile (:1326). [L] fSpeed
      // (cSpeed.cpp = ht9045_sm:2231) :1337-1342, fYieldMonitoring
      // (uYieldMonitoring.cpp = ht9045_sm:2154) :1348, SetWorkParameter
      // (cinitial.cpp = ht9045_sm:1696) :1355, EventReport (SECSGEM =
      // ht9045_secsgem:1379) :1324. [M] fRPDefault->RP_* (:1331-1333, :1351).
void TFTestIF::spbSaveClick(TObject *Sender)
{
    if(IniConfig.bA02DisableSaveParsWhenSwitchToOp==true &&                     //RogerYang 20260305 張寧說時間到都要登出，且不可被更改任何設定(A01_2)
        AccessLevel==0)
    {
        ShowMyMessage("[A01_2]目前已切換到Operator權限，\r\n請重新登入再做設定!");
        Close();
    }

    AnsiString S="";
    S=GetLastOpenFN();
    AnsiString szDir="";
    szDir=DataPath+S;

    SaveSetupFile(szDir, S);                                                    //kevin 20150105  Start 另存 jobfile
    #ifdef ASE_KaohSiung
       fBuilder->bSaveAsJobFile(S,"JOBFILE");
    #endif

    if(IniConfig.bEnable_SECS_GEM==true)
    {
        bHasSaveSet=true;                                                       //Ifor 20151204 新增判斷機台有無修改設定檔
        EventReport(SECS_EVENT.SaveRecipe);
    }
    ReadTestIFFile();
    spbSave->Down=false;

    if(CosFunction.bRecipeParameterDefault)                                     //Sam 20201209 : Default Recipe ChangeLog
    {
        fRPDefault->RP_Name->Clear()    ;
        fRPDefault->RP_Default->Clear() ;
        fRPDefault->RP_Value->Clear()   ;

        if(CosFunction.bRecipeParameterDefaultLog)
        {
            fSpeed->SearchRecipeParameter(fSpeed->tsAllSpeed,-1);               //Isaac 20171227 (Steven) : AutoSpeed RPD一鍵復原
            fSpeed->SearchRecipeParameter(fSpeed->tsIndex,0);
            fSpeed->SearchRecipeParameter(fSpeed->tsInArm,1);
            fSpeed->SearchRecipeParameter(fSpeed->tsOutArm,2);
            fSpeed->SearchRecipeParameter(fSpeed->tsTrayArm,3);
            fSpeed->SearchRecipeParameter(fSpeed->tsShuttle,4);
            fCleaning->SearchRecipeParameter(fCleaning,0);
        }
        SearchRecipeParameter(FTestIF,0);
        if(CosFunction.bRecipeParameterDefaultLog)
        {
            fYieldMonitoring->SearchRecipeParameter(fYieldMonitoring,0);
        }

        fRPDefault->CompareRPDefaultAndValue(fRPDefault->RP_Name, fRPDefault->RP_Default, fRPDefault->RP_Value);
    }

    fMain->BackupSetupFile();                                                   //Ifor 20170620 (wei) add Auto BackUp Setup File & Last Data
    SetWorkParameter();                                                         //Steven 20120130 : 存檔後要重新load參數    //Isaac 20210309 :TTL RS232兩塊板子，要更新TTL的資料
}
#endif // GATE (F-13)

#if 0 // GATE (F-14) sbtExitClick -- golden :1358-1366. ONE-LINE GATE: [L]
      // CheckTTLBoardBitMode() at :1362 (csystem.cpp:30645, ht9045_sm:2131).
      // Everything else in the body (rgInterfaceType->ItemIndex,
      // sbtExit->Down=false, Close()) is already reachable here -- so an
      // ht9045_sm-reaching wave can un-gate this one almost for free.
void TFTestIF::sbtExitClick(TObject *Sender)
{
    if(rgInterfaceType->ItemIndex==0)                                           //TTL
    {
        CheckTTLBoardBitMode();
    }
    sbtExit->Down=false;
    Close();
}
#endif // GATE (F-14)

#if 0 // GATE (F-15) DoSetRPDefault -- golden :1418-1442. [W] WriteIniData
      // into D:\HT9045\IniData\RPDefault.ini (:1439). [M] the TWinControl
      // traversal idiom: ControlCount / Controls[] / Hint have no port and
      // there is no TWinControl type (DEVIATION D-6 adapts the declaration to
      // TControl*; the transcript keeps golden's TWinControl).
void TFTestIF::DoSetRPDefault(TWinControl *PCtrl, int iSection)
{
    AnsiString szDir="D:\\HT9045\\IniData\\RPDefault.ini", aSection="";
    AnsiString aHintName;
    AnsiString sRet;

    if(iSection==0)
        aSection="Tester";

    for(int iP=0; iP<PCtrl->ControlCount; iP++)
    {
        TControl *P=PCtrl->Controls[iP];
        if(dynamic_cast<TWinControl *>(P)!=NULL)
        {
            DoSetRPDefault((TWinControl *)P, iSection);                         // 找該物件附屬的物件(遞迴)
        }

        TEdit *PEdit=dynamic_cast<TEdit*>(P);
        if(PEdit!=NULL && PEdit->Hint!="")
        {
            aHintName=PEdit->Hint;
            WriteIniData(szDir, aSection, aHintName, PEdit->Text);
        }
    }
}
#endif // GATE (F-15)

#if 0 // GATE (F-16) DoReplyDefaultToForm -- golden :1444-1473. [M] ONLY --
      // the same TWinControl traversal as (F-15). It does NOT write. This is
      // the one gated body in this file whose gate is purely a missing type:
      // a future TWinControl traversal shim un-gates it with no safety
      // argument needed.
void TFTestIF::DoReplyDefaultToForm(TWinControl *PCtrl, int iSection)
{
    AnsiString aHintName;
    AnsiString szDir="D:\\HT9045\\IniData\\RPDefault.ini";
    AnsiString aSection="";

    if(iSection==0)
        aSection="Tester";

    AnsiString Stringtemp="";
    for(int iP=0; iP<PCtrl->ControlCount; iP++)
    {
        TControl *P=PCtrl->Controls[iP];
        if(dynamic_cast<TWinControl *>(P)!=NULL)
        {
            DoReplyDefaultToForm((TWinControl *)P, iSection);                   // 找該物件附屬的物件(遞迴)
        }

        TEdit *PEdit=dynamic_cast<TEdit*>(P);
        if(PEdit!=NULL)
        {
            aHintName=PEdit->Hint;
            if(CheckIniData(szDir, aSection, aHintName))
            {
                Stringtemp =ReadIniData(szDir, aSection, aHintName, Stringtemp);
                PEdit->Text=Stringtemp;
            }
        }
    }
}
#endif // GATE (F-16)

#if 0 // GATE (F-17) SearchRecipeParameter -- golden :1475-1514 (40L). [W]
      // `CheckAndReadIniData` (:1499) WRITES THE DEFAULT BACK when the key is
      // missing (common.cpp:1567-1569) -- it is NOT the pure-read ReadIniData
      // (common.cpp:692). [M] same TWinControl traversal as (F-15), plus
      // fRPDefault->RP_Name/RP_Value/RP_Default.
void TFTestIF::SearchRecipeParameter(TWinControl *PCtrl, int iSection)
{
    AnsiString aHintName;
    AnsiString szDir="D:\\HT9045\\IniData\\RPDefault.ini";
    AnsiString aSection="";
    if(iSection==0)
        aSection="Tester";
    AnsiString Stringtemp="";
    AnsiString asDefaultValue="";

    for(int iP=0; iP<PCtrl->ControlCount; iP++)
    {
        TControl *P = PCtrl->Controls[iP];
        if(dynamic_cast<TWinControl *>(P)!=NULL)
        {
            SearchRecipeParameter((TWinControl *)P, iSection);                  // 找該物件附屬的物件(遞迴)
        }

        TEdit *PEdit=dynamic_cast<TEdit*>(P);
        if(PEdit!=NULL)
        {
            aHintName=PEdit->Hint;
            if(CheckIniData(szDir, aSection, aHintName))
            {
                asDefaultValue=CheckAndReadIniData(szDir, aSection, aHintName, Stringtemp);
                if(CosFunction.bRecipeParameterDefaultLog &&
                   asDefaultValue=="")                                          //Sam 20201209 : Default Recipe ChangeLog
                {
                                                                                //Default 數值被手動刪除掉的話，就不要監控了
                }
                else
                {
                    fRPDefault->RP_Name->Add(aHintName);
                    fRPDefault->RP_Value->Add(PEdit->Text);
                    fRPDefault->RP_Default->Add(asDefaultValue);
                }
            }
        }
    }
}
#endif // GATE (F-17)

#if 0 // GATE (F-18) FormShortCut -- golden :1521-1528. OMITTED FROM THE CLASS
      // ENTIRELY (not declared in forms/fTesterIF.h): its signature needs
      // `TWMKey`, which has ZERO port in this tree (20260828 scan; the only
      // hits are banner prose recording the same absence in cBinSel.cpp:4196,
      // cSetUp.cpp:250, forms/fContact.h:489/:758/:774). The transcript is
      // kept here so the text is not lost.
void TFTestIF::FormShortCut(TWMKey &Msg, bool &Handled)
{
    if(Msg.CharCode==VK_F5)                                                     //Sam 20220725 : 介面新增鍵盤 F5 將視窗恢復初始位置
    {
        Left=100;
        Top=10;
    }
}
#endif // GATE (F-18)
