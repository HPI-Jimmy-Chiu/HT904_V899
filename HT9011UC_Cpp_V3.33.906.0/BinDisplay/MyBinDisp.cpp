// =============================================================================
//  BinDisplay/MyBinDisp.cpp  --  TMyBinDispCtrl data-layer method bodies.
//
//  Golden ref: HT9011UC_Code_V3.33.906.0_20260618\BinDisplay\MyBinDisp.cpp
//              (3169 lines, cp950 -- 0 U+FFFD on decode, verified this wave).
//  Recon this wave is built from: docs/RECON_BinDisCtrl.md (2026-08-20).
//  Translator: AI(W906-BinDisp-WA) 20260820
//
//  See BinDisplay/MyBinDisp.h for the FULL wave-scope banner (WAVE SCOPE /
//  NOTE A/B / GATE REGISTER (1)-(3) / RECON CORRECTION / DEVIATION (a)-(c) /
//  VCL adaptations). This file's own banner covers only what is specific to
//  the .cpp: golden line ranges per translated method, the include-set
//  rationale, and two additional adaptations the header banner does not
//  already cover.
//
//  ============================ GOLDEN LINE MAP ==============================
//    ctor                          golden :39-129   (91 lines)
//    dtor                          golden :133-143  (11 lines)
//    SetComParity/UnitHasInstall/CloseUnit/OpenUnit/SetDelayTime/
//      GetDelayTime/GetTotalInstalledUnit/GetColorNow/GetBinNow/GerErrNow
//                                  golden :145-154  (10 one-liners)
//    GetRunStatus                  golden :156-170
//    SetComPort/SetComPort2        golden :172-180
//    ProcessStopStart              golden :182-195
//    Timer1Timer                   golden :284-604  GATE (1) -- stub only, see .h
//    CommBinReceiveData            golden :199-250
//    CommBinReceiveData2           golden :252-269
//    InstalledUnit                 golden :273-280
//    ShowCommLog                   golden :701-720  (NOTE A in .h -- required transitive dep)
//    WriteTargetBin                golden :606-638
//    WriteTargetCount              golden :640-646
//    StartFlash/IsAnyFlashing      golden :3078-3096
//    ProcessFlash                  golden :3100-3126
//    ClearAutoChangingWarn         golden :3129-3149
//    FlashPro                      golden :3154-3168
//
//  =========================== INCLUDE-SET NOTES ==============================
//  Golden includes: "MyBinDisp.h", "mymessbox.h", "cmydef.h", "cprod.h",
//  "TextProcess.h", "database.h" (golden :1-10). This TU differs by:
//    * "mymessbox.h" does not exist anywhere in this ported tree (verified:
//      `Glob mymessbox.h` under the Cpp root -- 0 hits). ShowMyMessage (the
//      only symbol from it this file's in-scope code calls, WriteTargetCount)
//      already has an established substitute declaration in
//      "canary_support.h" (that header's own banner item 3: "ShowMyMessage --
//      golden mymessbox.h:58 ... matches the database.cpp local forward-decl").
//    * "TextProcess.h" is DROPPED: verified (grep over this file's in-scope
//      translated bodies) that nothing this wave translates calls anything
//      from it -- its only golden consumers here (MyDeCodeASCII,
//      T_HEX2ASCII_Mac, A_Create_LCR, command_TFT_Input/Font) live in the
//      GATE (2) hardware-protocol family this wave does not translate.
//    * "Public/MyStringList.h" is ADDED (golden gets the complete
//      TMyStringList type transitively through ITS OWN cmydef.h, which in
//      this ported tree deliberately dropped that include -- see V906
//      cmydef.h's own banner: "AI(W0-TAIL) 20260626: de-VCL cmydef.h. Dropped
//      ... MyStringList.h ..."). Matches handlerlog.cpp's identical, already-
//      established fix for the identical gap.
//    * `MyDBIProcess(AnsiString,AnsiString)` (dtor's catch block, golden :141)
//      is forward-declared LOCALLY rather than pulled in via a header, for a
//      real, tree-wide reason: TWO overloads of this name exist --
//      aHotPlateSubstrate.h:933 `MyDBIProcess(AnsiString,AnsiString)` (2-arg,
//      REAL no-op body, aHotPlateSubstrate.cpp:1099 -- what golden's 2-arg
//      call here needs) and cMyDB.h:81
//      `MyDBIProcess(AnsiString,AnsiString,AnsiString="")` (3-arg, 3rd
//      defaulted -- ALSO callable with exactly 2 arguments, so if BOTH were
//      visible in one TU, golden's 2-arg call would be an AMBIGUOUS overload
//      resolution error). This exact landmine is already documented and
//      solved the same way by Automation/automation.cpp:72,
//      CCLink/MyCCLink.cpp, KYECFTP/FTPClient_*.cpp and SECSGEM/
//      uHGemClass.cpp: redeclare the 2-arg extern locally, do NOT
//      #include aHotPlateSubstrate.h wholesale (that header also declares an
//      unrelated global `class TList` that collides with vclcompat::TList if
//      both are visible unqualified in one TU -- see KNOWLEDGE.md). Reused
//      verbatim here.
//
//  ====================== TWO FURTHER ADAPTATIONS (not in the .h banner) =====
//  (d) `ZeroMemory(buf, sizeof(buf))` (golden :206,258) -> `memset(buf, 0,
//      sizeof(buf))`. Mechanical Win32-macro-to-libc translation (ZeroMemory
//      is `#define ZeroMemory(p,n) memset(p,0,n)` in real <windows.h>); this
//      tree does not pull raw Win32 headers into translated units that don't
//      already need them (matches cBinSel.cpp's own precedent of not
//      reaching for <windows.h> just for this macro).
//  (e) `ShowCommLog(BinDispCom2Buffer, -1, "")` (golden :248) passes a
//      `char*` where the callee expects `unsigned char*` -- BCB6 accepts
//      this narrowing implicitly; standard C++ does not. An explicit
//      `(unsigned char*)` cast is added at the one in-scope call site. Same
//      class of adaptation as (d): mechanical, not a behaviour change (the
//      bytes are identical either way).
//
//  GOLDEN DEFECT (kept, not fixed -- flagged per this tree's translation
//  policy): WriteTargetBin's bounds check (golden :608) is
//  `if(Index>MAX_BIN_UNIT) return;`, one-off from the correct
//  `>=MAX_BIN_UNIT` -- MAX_BIN_UNIT itself is one-past-the-last valid index
//  (`iSetBin[MAX_BIN_UNIT][...]`/`bSetBin[MAX_BIN_UNIT]` etc. are all sized
//  exactly MAX_BIN_UNIT), so `Index==MAX_BIN_UNIT` slips past this guard and
//  reaches `iSetBin[Index][...]`/`bSetBin[Index]=true;` one element past the
//  end of those arrays. Translated verbatim (`>`, not `>=`); see the call
//  site comment below.
// =============================================================================
#include "MyBinDisp.h"

#include "canary_support.h"         // ShowMyMessage (golden mymessbox.h substitute -- see banner)
#include "Public/MyStringList.h"    // complete TMyStringList (golden gets this transitively; see banner)
#include "cmydef.h"                 // NUMBER_PANEL_TYPE/MAGAZINE_BIN_DISP_TYPE/iRunStartMode/bAutoChangingWarn[]/iAutoColorBak[]
#include "cprod.h"                  // TrayForm (SYSTEM_TRAY_FORM)/CosFunction (via cprod.h's own #include "CosFunction.h")
#include "database.h"                                                          //Eastsun 20260513 KYEC: HSys.BinDisCtrl

#include <cstring>                  // memset/memcpy/strncpy

// See banner's MyDBIProcess note: redeclare the 2-arg overload locally rather
// than #include aHotPlateSubstrate.h wholesale (TList collision) or cMyDB.h
// (ambiguous-overload landmine with this exact 2-arg call).
extern void MyDBIProcess(AnsiString S1, AnsiString S2);

//------------------------------------------------------------------------------
// AI(W906-BinDisp-WA) 20260820: NO golden counterpart. See MyBinDisp.h's
// DEVIATION (b) for the full field-by-field rationale (BCB6
// TObject::NewInstance zero-fills the raw instance before any ctor body
// runs; golden's own ctor below never assigns these fields, so this port
// must zero them explicitly or leave them at C++'s indeterminate default).
//------------------------------------------------------------------------------
void TMyBinDispCtrl::ZeroInitVclFields()
{
    Addr=0;
    MagAddr=0;
    for(int i=0; i<MAX_BIN_UNIT; i++)
    {
        bSetColor[i]=false;
        bGetStatus[i]=false;
        iCountTFT[i]=0;
    }
    bFlashCom=false;
    memset(SendBuffer, 0, sizeof(SendBuffer));
    BinDispRecv2=false;
    memset(BinDispCom2Buffer, 0, sizeof(BinDispCom2Buffer));
    memset(BinDispCom2Buffer2, 0, sizeof(BinDispCom2Buffer2));
    memset(Com2ReceiveByte, 0, sizeof(Com2ReceiveByte));
    Receivelen=0;
    iOnceTask=0;
    iOnceTFTTask=0;
    iCycleTask=0;
    iCycleTFTTask=0;
    iStartSetBinTask=0;
    iStartSetColorTask=0;
    iStartGetStatusTask=0;
    CommBin=NULL;
    CommBin2=NULL;
}
//------------------------------------------------------------------------------
// 建構子   golden MyBinDisp.cpp:39-129
//------------------------------------------------------------------------------
TMyBinDispCtrl::TMyBinDispCtrl()
{
    ZeroInitVclFields();    // NOT a golden line -- see banner above / .h DEVIATION (b)

    for(int i=0; i<MAX_BIN_UNIT; i++)
    {
        bHasUnitArray[i]=false;
        bSetBin[i]=false;
        for(int j=0; j<TEST_MAX_BIN; j++)                                       //kevin 20140326     MAX_BIN_UNIT //for(j=0; j<MAX_BIN_UNIT; j++)
            iSetBin[i][j]=-1;
        iSetColor[i]=1;
        Alias[i]="";
        bSliding[i]=true;
        bSliding_Cnt[i]=true;
        iVersion[i]=0;
        iBinNow[i]=0;                                                           // 顯示器目前的Bin
        iColorNow[i]=1;                                                         // 顯示器目前的的Color
        bHasError[i]=false;                                                     // 顯示器是否有出錯
        iSetCount[i]=0;                                                         //Sam 20240604 : 新增 BinDisplay TFT
    }

    //==> Eastsun 20260513 KYEC: AutoChanging flash array init (v1.0)
    for(int k=0; k<MAX_BIN_UNIT; k++)
    {
        bFlash[k]         = false;
        iFlashColorA[k]   = 1;
        iFlashColorB[k]   = 3;
        iFlashPeriodMs[k] = 600;
        bFlashShowA[k]    = true;
    }
    //<== Eastsun 20260513

    slBinDispLog=new TMyStringList("D:\\HT9045_Log\\BinDisplayLog",             //Steven 20210910 : Bin顯示器的log
                                   "BinDisplayLog",
                                   "Date, Time, Action, Message");
    iSetBin[0][0]=111;                                                          //Loader
    iSetBin[1][0]=104;                                                          //Empty Tray
    iSetBin[2][0]=102;                                                          //Color Tray
    iSetColor[0]=3;                                                             //橘色
    iSetColor[1]=3;
    iSetColor[2]=3;

    bStopProcess=false;
    bStartSetColor=false;
    bStartSetBin=false;
    bStartOnce=false;                                                           //Sam 20240604 : 新增 BinDisplay TFT
    bStartCycle=false;
    bHasUnit=false;
    BinDispRecv=false;
    ComPort=4;
    ComParity=None;

    dDelaySec=5.0;                                                              //Sam 20240604 : 顯示器輪巡時間改為 double
    InitialOK=false;

    Timer1=new ht9045_bindisp::TTimer(nullptr);
    // golden: `Timer1->OnTimer=Timer1Timer;` (a BCB6 closure member-method
    // assignment). This tree's established idiom for that exact idiom is a
    // capturing lambda (PowerSavingMode.cpp:519 / uRENESAS_Server.cpp:186)
    // since ht9045_bindisp::TTimer::OnTimer is a std::function, not a
    // __closure. Timer1Timer itself is GATE (1) (see .h) -- this wiring
    // compiles and stores the callback, but nothing in this build's event
    // loop ever ticks it (no message pump), so it is faithfully inert.
    Timer1->OnTimer=[this](TObject *Sender){ Timer1Timer(Sender); };
    Timer1->Interval=200;                                                       //kevin 20140327  30 --> 200
    iOldTimerInterval=Timer1->Interval;                                         //Eastsun 20260514
    Timer1->Enabled=true;
    iTotalInstalledUnit=-1;
    iBinDispCtrlTask=1;
    bFirstInit=true;                                                            //Steven 20110407
    iRusStatus=eBDP_Initial;

    sUnitName[eBinDispLoader]="Loader";                                         //Sam 20240604 : 新增 BinDisplay TFT
    sUnitName[eBinDispEmpty]="Empty";
    sUnitName[eBinDispColor]="Color";
    sUnitName[eBinDispAuto1]="Auto1";
    sUnitName[eBinDispAuto2]="Auto2";
    sUnitName[eBinDispAuto3]="Auto3";
    sUnitName[eBinDispFix1]="Fix1";
    sUnitName[eBinDispFix2]="Fix2";
    sUnitName[eBinDispFix3]="Fix3";
    sUnitName[eBinDispFix4]="Fix4";
    sUnitName[eBinDispFix5]="Fix5";
    sUnitName[eBinDispFix6]="Fix6";
    sUnitName[eBinDispBulkBox]="Box";
    sUnitName[eBinDispMag1]="Magazine01";
    sUnitName[eBinDispMag2]="Magazine02";
    sUnitName[eBinDispMag3]="Magazine03";
    sUnitName[eBinDispMag4]="Magazine04";
    sUnitName[eBinDispMag5]="Magazine05";
    sUnitName[eBinDispMag6]="Magazine06";
    sUnitName[eBinDispMag7]="Magazine07";
    sUnitName[eBinDispMag8]="Magazine08";
    sUnitName[eBinDispMag9]="Magazine09";
    sUnitName[eBinDispMag10]="Magazine10";
    sUnitName[eBinDispMag11]="Magazine11";
    sUnitName[eBinDispMag12]="Magazine12";
    sUnitName[eBinDispMag13]="Magazine13";
    sUnitName[eBinDispMag14]="Magazine14";
}
//------------------------------------------------------------------------------
// 解構子   golden MyBinDisp.cpp:133-143
//------------------------------------------------------------------------------
TMyBinDispCtrl::~TMyBinDispCtrl()
{
    try
    {
        Timer1->Enabled=false;
    }
    catch(...)
    {
        MyDBIProcess("Exception", "~TMyBinDispCtrl");
    }
}
//------------------------------------------------------------------------------
// golden MyBinDisp.cpp:145-154 (10 one-liners)
//------------------------------------------------------------------------------
void  TMyBinDispCtrl::SetComParity(Spcomm::TParity Parity)  {ComParity=Parity;}          // 設定顯示器群組是利用那一個Parity
bool  TMyBinDispCtrl::UnitHasInstall(int Index)     {return bHasUnitArray[Index];}       // 確認該顯示器使是否有啟用
void  TMyBinDispCtrl::CloseUnit(int Index)          {bHasUnitArray[Index]=false;}        // 關閉該顯示器
void  TMyBinDispCtrl::OpenUnit(int Index)           {bHasUnitArray[Index]=true;}         // 開啟該顯示器
void  TMyBinDispCtrl::SetDelayTime(double Sec)      {dDelaySec=Sec;}                     // 設定多久輪替一次 //Sam 20240604 : 顯示器輪巡時間改為 double
double   TMyBinDispCtrl::GetDelayTime()             {return dDelaySec;}                  // 設定多久輪替一次
int   TMyBinDispCtrl::GetTotalInstalledUnit()       {return iTotalInstalledUnit+1;}      // 取得總共安裝了幾個顯示器
int   TMyBinDispCtrl::GetColorNow(int Index)        {return iColorNow[Index];}           // 顯示器目前的顏色
int   TMyBinDispCtrl::GetBinNow(int Index)          {return iBinNow[Index];}             // 顯示器目前的字元
bool  TMyBinDispCtrl::GerErrNow(int Index)          {return bHasError[Index];}           // 顯示器是否有出錯
//---------------------------------------------------------------------------
// golden MyBinDisp.cpp:156-170
//---------------------------------------------------------------------------
AnsiString TMyBinDispCtrl::GetRunStatus()                                       // 回傳目前在執行的動作
{
    AnsiString Message="";
    switch(iRusStatus)
    {
        case eBDP_Initial:      Message="Initialing...";    break;
        case eBDP_GetStatus:    Message="Get status...";    break;
        case eBDP_ColorSet:     Message="Color Setting.";   break;
        case eBDP_BinSet:       Message="Bin Setting.";     break;
        case eBDP_BinRun:       Message="Bin Running.";     break;
        case eBDP_DispErr:      Message="Display Error!!";  break;
    }

    return Message;
}
//---------------------------------------------------------------------------
// golden MyBinDisp.cpp:172-180
//---------------------------------------------------------------------------
void  TMyBinDispCtrl::SetComPort(AnsiString port)                               // 設定顯示器群組是利用那一個COM PORT
{
    ComPort=port;
}
//---------------------------------------------------------------------------
void  TMyBinDispCtrl::SetComPort2(AnsiString port)                              // 設定顯示器群組是利用那一個COM PORT
{
    ComPort2=port;
}
//---------------------------------------------------------------------------
// golden MyBinDisp.cpp:182-195
//---------------------------------------------------------------------------
void TMyBinDispCtrl::ProcessStopStart(bool Value)
{
    bStopProcess=Value;
    if(bFirstInit==true)
    {
        InitialTask();                                                          //Sam 20240604 : 新增 BinDisplay TFT -- reaches TMyBinDispOffline's no-op override
        bFirstInit=false;
    }
    else
    {
        if(bStopProcess==true)                                                  //Steven 20110216
            iBinDispCtrlTask=50;
    }
}
//------------------------------------------------------------------------------
// GATE (1) -- see BinDisplay/MyBinDisp.h banner. Golden body is
// MyBinDisp.cpp:284-604 (~320 lines), the hardware-bring-up dispatcher.
// Intentionally an empty stub in this wave; declared/wired (ctor's
// Timer1->OnTimer assignment) so a future "UI/hardware wave" can fill this
// in without touching the ctor again.
//------------------------------------------------------------------------------
void TMyBinDispCtrl::Timer1Timer(TObject * /*Sender*/)
{
}
//------------------------------------------------------------------------------
// COM PORT 接收資料function   golden MyBinDisp.cpp:199-250
//------------------------------------------------------------------------------
void TMyBinDispCtrl::CommBinReceiveData(TObject * /*Sender*/,
      void *Buffer, Spcomm::Word BufferLength)
{
    if(BufferLength>=1024)                                                      //Steven 20141110 : 避免記憶體破壞
        return;
    AnsiString asLog22="";

    memset(BinDispCom2Buffer, 0, sizeof(BinDispCom2Buffer));                    // golden: ZeroMemory(...) -- see banner (d)
    if(NUMBER_PANEL_TYPE==4)                                                    //Sam 20240604 : 新增 BinDisplay TFT
    {
        sReadBuffer="";
        memcpy(BinDispCom2Buffer, Buffer, BufferLength);
        BinDispCom2Buffer[BufferLength]='\x0';
        for(int i=0; i<BufferLength; i++)
        {
            asLog22.sprintf("%02X", BinDispCom2Buffer[i]);
            sReadBuffer+= asLog22;
        }

        if(MAGAZINE_BIN_DISP_TYPE==eTFT)
        {
            sReadBufferMag="";
            for(int i=0; i<BufferLength; i++)
            {
                asLog22.sprintf("%02X", BinDispCom2Buffer[i]);
                sReadBufferMag+= asLog22;
            }
        }
    }
    else
    {
        strncpy(BinDispCom2Buffer, (char*)Buffer, BufferLength);
        BinDispCom2Buffer[BufferLength]='\x0';

        sReadBuffer.sprintf("%s", AnsiString(BinDispCom2Buffer));
        if(MAGAZINE_BIN_DISP_TYPE==eTFT)
        {
            sReadBufferMag="";
            for(int i=0; i<BufferLength; i++)
            {
                asLog22.sprintf("%02X", BinDispCom2Buffer[i]);
                sReadBufferMag+= asLog22;
            }
        }
    }

    if(sReadBuffer=="")                                                         //kevin 20170710 (wei) add bin display
        return;

    ShowCommLog((unsigned char*)BinDispCom2Buffer, -1, "");                     //Sam 20240604 : 新增 BinDisplay TFT -- see banner (e), char*->unsigned char* cast
    BinDispRecv=true;
}
//------------------------------------------------------------------------------
// golden MyBinDisp.cpp:252-269
//------------------------------------------------------------------------------
void TMyBinDispCtrl::CommBinReceiveData2(TObject * /*Sender*/,
      void *Buffer, Spcomm::Word BufferLength)
{
    if(BufferLength>=1024)                                                      //Steven 20141110 : 避免記憶體破壞
        return;

    memset(BinDispCom2Buffer2, 0, sizeof(BinDispCom2Buffer2));                  // golden: ZeroMemory(...) -- see banner (d)
    strncpy(BinDispCom2Buffer2, (char*)Buffer, BufferLength);
    BinDispCom2Buffer2[BufferLength]='\x0';

    sReadBuffer2.sprintf("%s", AnsiString(BinDispCom2Buffer2));
    if(sReadBuffer2=="")                                                        //kevin 20170710 (wei) add bin display
        return;

    if(IniConfig.bC14SaveBinDisplayLog)                                         //Steven 20220309 : BinDisplay Log
        slBinDispLog->AddTextWithDateTime(AnsiString("Recv, ")+sReadBuffer2);
    BinDispRecv2=true;
}
//------------------------------------------------------------------------------
// 設定address=Index 是有存在的   golden MyBinDisp.cpp:273-280
//------------------------------------------------------------------------------
void TMyBinDispCtrl::InstalledUnit(int Index)
{
    bHasUnitArray[Index]=true;
    bHasUnit=true;
    bSetBin[Index]=true;
    if(iTotalInstalledUnit<Index)
        iTotalInstalledUnit=Index;
}
//------------------------------------------------------------------------------
// golden MyBinDisp.cpp:701-720 (NOTE A in .h -- required transitive dep of
// CommBinReceiveData, not independently in the recon's own line-item list)
//------------------------------------------------------------------------------
void TMyBinDispCtrl::ShowCommLog(unsigned char ucLog[], int Address, AnsiString sFun) //Sam 20240604 : 新增 BinDisplay TFT
{
    AnsiString sLog="",asLog="", asLog22="";

    for(int i=0; i<20; i++)
    {
        asLog22.sprintf("[%02X]", ucLog[i]);                                    //記錄16進制
        asLog=asLog+" "+asLog22;
    }

    if(Address==-1)                                                             //Recv
        sLog.sprintf("Recv, %s", asLog);
    else if(Address>=0 && Address<eBinDispTotal)
        sLog.sprintf("Send, %s, %s %s", asLog, sUnitName[Address], sFun);
    else
        sLog.sprintf("Send, %s, %s %s", asLog, "Undefined", sFun);

    if(IniConfig.bC14SaveBinDisplayLog)                                         //Steven 20220309 : BinDisplay Log
        slBinDispLog->AddTextWithDateTime(sLog);
}
//------------------------------------------------------------------------------
// golden MyBinDisp.cpp:606-638
//------------------------------------------------------------------------------
void TMyBinDispCtrl::WriteTargetBin(int Index, int *bin, int color)
{
    if(Index>MAX_BIN_UNIT)                                                      //jou 2011-08-15 start : Addr破壞記憶體  //GOLDEN DEFECT: off-by-one, should be >=MAX_BIN_UNIT (MAX_BIN_UNIT is one-past-the-last valid index into iSetBin[]/bSetBin[]); kept verbatim, see file banner.
        return;
    if(Index<0)
        return;

    //bin = -1      : 不顯示，所以填入123
    //bin = 0~99    : 顯示數字0~99
    //bin = 100~125 : 顯示字母A~Z

    // bin輸入值對照表
    //  A   B   C   D   E   F   G   H   I   J   K   L   M   N   O   P   Q   R   S   T   U   V   W   X   Y   Z
    //  100 101 102 103 104 105 106 107 108 109 110 111 112 113 114 115 116 117 118 119 120 121 122 123 124 125

    iSetColor[Index]=color;
    for(int i=0; i<TEST_MAX_BIN; i++)                                           //kevin 20140326
        iSetBin[Index][i]=bin[i];

    if(CosFunction.bLoaderTrayToAuto1 && TrayForm.LoaderToEmptyColor[iRunStartMode]==2 && Index==eBinDispAuto1)
        iSetBin[Index][0]=104;

    if(CUSTOMER_CODE==CC_MAXIM_THAILAND &&
       TrayForm.LoaderToEmptyColor[iRunStartMode]==2)                           //Ifor 20260225 add:客戶要求功能有開啟才顯示"R"
        iSetBin[1][0]=117;

    iSetBin[0][1]=-1;                                                           //Loader
    bStartSetBin=true;
    bStartSetColor=true;
    bStartOnce=true;                                                            //Sam 20240604 : 新增 BinDisplay TFT
    bStartCycle=true;
    bSetBin[Index]=true;
}
//---------------------------------------------------------------------------
// golden MyBinDisp.cpp:640-646
//---------------------------------------------------------------------------
void TMyBinDispCtrl::WriteTargetCount(int Index, int iCount)                    //Sam 20240604 : 新增 BinDisplay TFT
{
    if(Index>=0 && Index<MAX_BIN_UNIT)
        iSetCount[Index]=iCount;
    else
        ShowMyMessage("WriteTargetCount error");
}
//==> Eastsun 20260513 KYEC: AutoChanging flash centralised implementation (v2.0 minimal)
//---------------------------------------------------------------------------
// StartFlash: pin this BinDisp to flash between colorA / colorB   golden :3078-3088
//---------------------------------------------------------------------------
void TMyBinDispCtrl::StartFlash(int Index, int colorA, int colorB, int periodMs)
{
    if(Index<0 || Index>=MAX_BIN_UNIT) return;
    if(periodMs < 50) periodMs = 50;
    bFlash[Index]         = true;
    iFlashColorA[Index]   = colorA;
    iFlashColorB[Index]   = colorB;
    iFlashPeriodMs[Index] = periodMs;
    bFlashShowA[Index]    = true;
    bFlashCom=true;
}

// IsAnyFlashing: any BinDisp currently flashing?   golden :3090-3096
//---------------------------------------------------------------------------
bool TMyBinDispCtrl::IsAnyFlashing(void)
{
    for(int k=0; k<MAX_BIN_UNIT; k++) if(bFlash[k]) return true;
    return false;
}
//---------------------------------------------------------------------------
// ProcessFlash: called by Timer1Timer (GATE (1) -- so this is never actually
// invoked in this wave; translated verbatim regardless since it is a pure,
// self-contained data-layer state machine, per recon S2).   golden :3100-3126
//---------------------------------------------------------------------------
void TMyBinDispCtrl::ProcessFlash(void)                                         //Eastsun 20260513 : 閃爍功能
{
    if(!IniConfig.bP66AutoChangingFlashWarn) return;
    if(!IsAnyFlashing()) return;
    for(int k=0; k<MAX_BIN_UNIT; k++)
    {
        if(!bFlash[k])
        {
            bSetColor[k] = false;
            continue;
        }

        bSetColor[k] = true;
        if(bFlashCom)
        {
            bFlashShowA[k] = !bFlashShowA[k];
            iFlashColorA[k]++;
            if(iFlashColorA[k]>3)
                iFlashColorA[k]=1;
            iSetColor[k] = iFlashColorA[k];
            bFlashCom=false;
        }
    }
}
//------------------------------------------------------------------------------
//==> Eastsun 20260513 KYEC 防工傷 Auto1/Auto2 換盤閃爍警示 helper   golden :3129-3149
void TMyBinDispCtrl::ClearAutoChangingWarn(int AutoTarget)
{
    if(!IniConfig.bP66AutoChangingFlashWarn) return;
    //Eastsun 20260513 KYEC: 放寬到 Auto1/Auto2/Auto3         f
    if(AutoTarget < 0 || AutoTarget > 2) return;
    if(!bAutoChangingWarn[AutoTarget]) return;

    bAutoChangingWarn[AutoTarget] = false;

    for(int k=0; k<MAX_BIN_UNIT; k++)
    {
        if(bFlash[k])
        {
            iSetColor[k] = iAutoColorBak[AutoTarget];
            bFlash[k]=false;
        }
    }
    iStartSetColorTask=1;
    iBinDispCtrlTask=200;
}
//<== Eastsun 20260513
//--------------------------------------------------------------------------
    //==> Eastsun 20260513 KYEC: AutoChange-tray flashing warn entry (v1.0 central scanner)
    //Eastsun 20260513 KYEC: central scanner in BinDispCtrl, 250ms toggle, color-only (no bin re-send)
// golden :3154-3168
void TMyBinDispCtrl::FlashPro(int AutoTarget)
{
    bool bFlashThis = (AutoTarget==0 || AutoTarget==1);
    if(AutoTarget==2 && MAGAZINE_BIN_DISP_TYPE==eMagBinUninstall) bFlashThis = true;
    if(IniConfig.bP66AutoChangingFlashWarn && bFlashThis && !bAutoChangingWarn[AutoTarget])
    {
        int iBdIdx;
        if(AutoTarget==0)      iBdIdx = eBinDispAuto1;
        else if(AutoTarget==1) iBdIdx = eBinDispAuto2;
        else                   iBdIdx = eBinDispAuto3;
        // GOLDEN ODDITY (.h DEVIATION (a)): reaches through the global
        // singleton HSys.BinDisCtrl instead of `this`, even though this IS
        // `this` on the live production singleton. Kept verbatim, not
        // "fixed" to this->GetColorNow(...)/this->StartFlash(...).
        iAutoColorBak[AutoTarget] = HSys.BinDisCtrl->GetColorNow(iBdIdx);
        bAutoChangingWarn[AutoTarget] = true;
        HSys.BinDisCtrl->StartFlash(iBdIdx, 1 /*紅*/, 4 /*黑*/, 250);
    }
}
//<== Eastsun 20260513
