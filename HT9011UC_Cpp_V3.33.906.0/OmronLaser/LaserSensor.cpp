// =============================================================================
//  OmronLaser/LaserSensor.cpp  --  Omron laser distance-sensor debug/
//                                   calibration form + serial protocol engine.
//
//  Faithful translation of golden OmronLaser/LaserSensor.cpp (1910 lines,
//  BCB6, Big5/cp950).
//  Translator: AI(W906-PT-W3-laser) 20260807
//  Translation wave: W906-PT-W3 ("laser" group), file 1 of 3.
//
//  ROLE: see LaserSensor.h's banner for the full picture.  In short: this is
//  the ONLY place in the laser subsystem that actually talks to the Omron
//  serial controllers (CommLaser1/2/InArm/OutArm->WriteCommData) and parses
//  their replies (ProcessReceiveData/ProcessReceiveData0101).
//  LaserSensorInArm.cpp / LaserSensorShuttle.cpp merely set a task cursor +
//  bool flag here and poll it; the two Timer callbacks (Timer1Timer /
//  TimerInArmTimer) are the state machines that notice the flag, send the
//  request, and clear the flag once ProcessReceiveData0101 has written the
//  measured value into palLaserValue_InArm->Caption / MyPanel[][]->Caption.
//
//  DEVIATION FROM THE forms/ FACADE CONVENTION, STATED EXPLICITLY: the other
//  17 forms already in this tree (fMain, fLotInfo, ...) only translate the
//  MEMBERS other files dereference; the form's OWN internal business logic
//  (button click bodies, its own Timer callbacks) is out of PT-campaign scope
//  and stays an offline no-op, because in golden that logic drives on-screen
//  widgets nothing here can observe. TfLaserSensor is different: its Timer
//  callbacks are not screen-painting side effects, they are the load-bearing
//  half of the InArm/Shuttle laser floating-check subsystem this wave exists
//  to complete (a) they run REGARDLESS of whether anyone can see the panel
//  Caption they update, and (b) LaserSensorInArm.cpp/LaserSensorShuttle.cpp
//  cannot do their own job without them. Skipping them the way fMain.cpp
//  skips main.cpp's 34,973 lines would compile-and-link cleanly but silently
//  leave InArmLaserInit/CheckInArmFloating/ShtLaserInit/CheckShtFloating
//  spinning forever (bInArm never clears because nothing ever calls
//  WriteCommData). So THIS form's body is translated in full (WAVE SCOPE
//  table below), unlike the established convention for every other form in
//  this tree -- a deliberate, reported, single-file exception, not a house
//  style change.
//
//  Integration note for whoever wires this into ht9045_sm's main loop: golden
//  Timer1Timer/TimerInArmTimer were VCL TTimer callbacks (fired by the
//  message pump on a fixed interval, matching the "poll and dispatch" idiom
//  uHeaterThread.cpp already uses for its own TThread). Nothing here spins up
//  a thread or registers with a real timer -- these are just two ordinary
//  public methods now; something in the eventual main loop needs to call
//  fLaserSensor->Timer1Timer(0) and fLaserSensor->TimerInArmTimer(0)
//  periodically (golden's own `.dfm` Timer1/TimerInArm interval), exactly the
//  way it already must call every other ported *Timer/Do* state-machine
//  pump. That wiring is main-loop integration work, explicitly out of this
//  wave's "new mirrored files only" mandate (see the task's hard rule 7).
//
//  WAVE SCOPE -- ACTIVE (all 42 golden functions this file defines, whole
//  bodies, verbatim numeric/string semantics) vs GATED-OUT-WHOLE (7 golden
//  mouse-down handlers with no portable signature -- see LaserSensor.h banner
//  for the citation; they are simply absent, not stubbed):
//    TfLaserSensor::TfLaserSensor (ctor)     golden :32-197
//    FormShow                                golden :199-212
//    FormClose                               golden :214-219 (params trimmed)
//    WriteInfoToMemo                         golden :221-244
//    ProcessReceiveData                      golden :246-353
//    ProcessReceiveData0101                  golden :355-409
//    CommLaser1ReceiveData                   golden :411-471
//    btSetToZeroClick                        golden :478-483
//    CommLaser2ReceiveData                   golden :485-545
//    cbStartPoolingClick                     golden :547-555
//    btGetValueSht1_2RowClick                golden :557-561
//    btGetValueSht1_1RowClick                golden :563-567
//    btGetValueSht2_2RowClick                golden :569-573
//    btGetValueSht2_1RowClick                golden :575-579
//    btConnectClick                          golden :582-687
//    CommLaserInArmReceiveData                golden :689-754
//    btGetValueInArmClick                    golden :756-760
//    btSetToZeroSh2Click                     golden :762-765
//    btSetToZeroSh1Click                     golden :767-770
//    btSetToZeroInArmClick                   golden :772-775
//    Timer1Timer                             golden :777-1075
//    InitLaserEdtList                        golden :1077-1107
//    sbUpdateClick                           golden :1109-1127
//    SaveShuttleLaserValue                   golden :1129-1153
//    SaveInArmLaserValue                     golden :1155-1174
//    ReadLaserFile                           golden :1176-1216
//    ReadFile                                golden :1218-1246
//    DoIniDataToForm                         golden :1248-1281
//    sbtExitClick                            golden :1289-1293
//    btSaveLogClick                          golden :1295-1312
//    WriteToMemo                             golden :1314-1320
//    TimerInArmTimer                         golden :1322-1578
//    FormDestroy                             golden :1580-1585
//    btSetPlate2GoldenToZeroClick             golden :1587-1598
//    btSetPlate1GoldenToZeroClick             golden :1600-1611
//    btGetInShuttleGoldenClick                golden :1673-1680
//    btGetOutShuttleGoldenClick               golden :1683-1690
//    DoGetLaserValue                         golden :1693-1773
//    DoGetOutLaserValue                      golden :1775-1855
//    CommLaserOutArmReceiveData               golden :1857-1891
//    btGetShValueClick                       golden :1893-1899
//    btGetOutShValueClick                    golden :1901-1907
//  GATED-OUT-WHOLE (no body anywhere in this file -- see LaserSensor.h banner
//  GATE section for the precedent and reasoning): edICThicknessMouseDown
//  (golden :1283-1287), mtPlate2_GoldenMouseDown (:1613-1621),
//  mtPlate1_GoldenMouseDown (:1623-1631), edDeviceXOffsetMouseDown
//  (:1633-1640), edDeviceYOffsetMouseDown (:1643-1650),
//  edOutDeviceXOffsetMouseDown (:1653-1660), edOutDeviceYOffsetMouseDown
//  (:1663-1670).
//
//  GATE REGISTER (call-level, #if 0/#else -- these are inside otherwise-ACTIVE
//  bodies, distinct from the 7 whole-function omissions above):
//   (1) golden ctor :143-144: `if(USE_LASER_DISTANCE) btConnect->Click();`.
//       ACTIVE verbatim -- btConnect is a TLaserClickButton (see .h), so this
//       call is real and opens the serial ports at construction time exactly
//       like golden. Not a gate; listed here only so the reader does not go
//       looking for one.
//   (2) golden TDateTime::TimeString() (WriteInfoToMemo golden :228,235,242)
//       -- BCB6 TDateTime has a TimeString() INSTANCE METHOD; this port's
//       vclcompat::TDateTime (vclcompat/TDateTime.h) does not carry it (grep
//       confirms zero `TimeString` hits anywhere in vclcompat). The free
//       function `vclcompat::TimeToStr(TDateTime)` (same header) produces the
//       identical "hh:nn:ss" text. Not gated (#if 0) because this is a pure
//       spelling substitution with an existing 1:1 equivalent, not a missing
//       capability -- recorded here for anyone grepping for `TimeString` and
//       wondering where it went.
//   (3) golden ProcessReceiveData0101 :366 `cbLD40L->Checked` -- real field,
//       real read, no gate.
//  No golden call in this file's 42 ACTIVE bodies references a symbol with
//  NO compiled body anywhere in this port tree (verified this wave, grepped):
//  every external symbol used (Spcomm::TComm, vclcompat::TrayCore,
//  Public/HTEditList's elLaser, EJ1N/TextProcess's MyDeCodeASCII/SetBCC/
//  ConvertByte/GetCOMPortStatus, common.h's DataPath/GetLastOpenFN/
//  MyForceDirectories/WriteIniData/CheckAndReadIniData, canary_support.h's
//  ShowMyMessage, aHotPlateSubstrate.h's MyDBIProcess, cmydef.h's
//  LogSoftwareOffTime/InitialOK/SystemStart/System*, database.h's HSys,
//  cprod.h's TestIF_File/HotPlateForm, forms/fMain.h's BackupSetupFile) is
//  already a real, previously-landed translation.
//
//  VCL/Borland conversions: `#pragma hdrstop`/`#pragma package(smart_init)`/
//  `#pragma link "..."`/`#pragma resource "*.dfm"` all dropped (golden
//  :3,21-25). `__fastcall` dropped from every definition. `byte*` -> `unsigned
//  char*` (no `byte` typedef exists anywhere in this port -- grepped -- so
//  golden's BCB6 `Windows.hpp`-supplied alias is spelled out directly rather
//  than inventing a new one). `Pointer`/`WORD` -> `void*`/`Spcomm::Word`
//  (matches vclcompat/Comm.h's own OnReceiveData signature). SOFT_SIMULTE is
//  NOT defined in this tree (MachineType.h:48 keeps it commented out, same as
//  every other translated unit's own note to this effect) -- every
//  `#ifdef SOFT_SIMULTE` arm below is reproduced verbatim but inert; the
//  `#else`-equivalent (i.e. the code that runs when the macro is undefined,
//  which for these bare `#ifdef ... #endif` blocks with no `#else` means
//  "the enclosed lines are skipped entirely") is what actually compiles.
//
//  Big5: every Chinese comment decoded via cp950 and preserved as UTF-8.
//  Final gate: ZERO U+FFFD.
// =============================================================================
// BCB6 ORIGINAL include block (mirrored as a comment for provenance):
//   #include "MachineDefine.h" ; #pragma hdrstop
//   #include "LaserSensor.h" / "LaserSensorShuttle.h" / "LaserSensorInArm.h" /
//     "main.h" / "HTEditList.h" / "database.h" / "mymessbox.h" / "Common.h" /
//     "cmydef.h" / "cprod.h" / "myQwertyKeyBoard.h" / "MyMotor.h"
//   ; #pragma package(smart_init) ; #pragma link "SPComm" ; #pragma link
//     "ALed" ; #pragma link "HTray" ; #pragma resource "*.dfm"
//
//   NOT re-included here, and why:
//     "main.h" -- golden's god-form header; this file's ACTIVE bodies touch
//       exactly one symbol from it, fMain->BackupSetupFile() (sbUpdateClick),
//       reached instead via forms/fMain.h (the tree's own facade for main.h).
//     "mymessbox.h" -- golden ShowMyMessage's declared home; this tree's
//       compiled body lives behind canary_support.h instead (see GATE
//       REGISTER preface above).
//     "myQwertyKeyBoard.h" -- only referenced by the 7 gated-out-whole mouse
//       handlers (LaserSensor.h banner); nothing else in this file uses it.
//     "MyMotor.h" -- golden's spelling of this tree's real path
//       "Motor/mymotor.h" (see below; same rename TriTemp.cpp's own banner
//       already documents for the identical golden include).
//     "HTEditList.h" -- this tree's real path is "Public/HTEditList.h".
// =============================================================================
#include "MachineDefine.h"

#include "OmronLaser/LaserSensor.h"
#include "OmronLaser/LaserSensorShuttle.h"
#include "OmronLaser/LaserSensorInArm.h"
#include "forms/fMain.h"             // fMain->BackupSetupFile() (sbUpdateClick, golden :1126)
#include "Public/HTEditList.h"       // elLaser (HTEditList*) -- golden "HTEditList.h"
#include "database.h"                // HSys.asLASER_COM[4]
#include "canary_support.h"          // ShowMyMessage -- golden "mymessbox.h"
#include "common.h"                  // DataPath, GetLastOpenFN, MyForceDirectories, WriteIniData, CheckAndReadIniData
#include "cmydef.h"                  // LogSoftwareOffTime, InitialOK, SystemStart, System{Year,Month,Date,Hour,Min,Sec}, USE_LASER_DISTANCE
#include "cprod.h"                   // TestIF_File, HotPlateForm
#include "EJ1N/TextProcess.h"        // MyDeCodeASCII, SetBCC, ConvertByte, GetCOMPortStatus
#include "Motor/mymotor.h"           // golden "MyMotor.h" -- this tree's real path (see TriTemp.cpp precedent); MOT[]/TMyMotor for DoGetLaserValue/DoGetOutLaserValue
#include "myTimer.h"                 // TQPF_Timer (also reachable via cmydef.h/cprod.h; included directly per this tree's per-file-states-its-own-deps convention)

// MyDBIProcess -- golden home aHotPlateSubstrate.h:924 (`extern void
// MyDBIProcess(AnsiString S1, AnsiString S2);`). NOT #include-d whole: that
// header also carries its OWN narrower `class TList`/`class uPlateInfo`/
// `class TInLaserCheck`/`CheckInArmFloating(bool=false)` shims (documented
// as pre-existing, deliberately narrower placeholders by Public/HTEditList.h's
// own banner "INTEGRATION HAZARDS (A)/(B)" and by LaserSensorInArm.h's real
// TInLaserCheck/CheckInArmFloating above) which REDEFINE the very symbols
// this TU already gets the real versions of via HTEditList.h and
// LaserSensorInArm.h -- a hard multiple-definition error if both headers are
// included in the same TU. Forward-declaring just the one function needed
// here matches golden's OWN idiom for exactly this situation: golden
// LaserSensor.cpp:581 does the identical thing for GetCOMPortStatus
// (`extern bool GetCOMPortStatus(AnsiString Com);` inline, rather than
// pulling in that symbol's whole declaring header).
extern void MyDBIProcess(AnsiString S1, AnsiString S2);

//---------------------------------------------------------------------------
TfLaserSensor *fLaserSensor = new TfLaserSensor();

// golden ctor (:148 etc) uses BCB6 Graphics.hpp's clAqua (RGB(0,255,255) ->
// TColor 0x00BBGGRR = 0x00FFFF00). vclcompat/TrayCore.h's local TColor shim
// (HT9045_W7C1_TCOLOR_SHIM) does not carry clAqua -- only the subset its own
// consumers needed. Per that shim's own banner precedent ("the older single-
// purpose shims... declare their own copy"), this TU declares its own, in
// the same namespace so `vclcompat::clAqua` resolves alongside vclcompat::
// clWhite/clYellow/clRed used right beside it below.
namespace vclcompat { const TColor clAqua = 0x00FFFF00; }

#define STX       2
#define ETX       3
AnsiString asLaserValue[2];                                                     //Eastsun 20260525 laser 整合
//---------------------------------------------------------------------------
TfLaserSensor::TfLaserSensor()
{
    sResponseCode[0][rcl0000]="0000"; sResponseCode[1][rcl0000]="Response Code OK.";
    sResponseCode[0][rcl1001]="1001"; sResponseCode[1][rcl1001]="命令過長";
    sResponseCode[0][rcl1002]="1002"; sResponseCode[1][rcl1002]="命令過短";
    sResponseCode[0][rcl1003]="1003"; sResponseCode[1][rcl1003]="數量不符, 數據數量與元素數量不符";
    sResponseCode[0][rcl1101]="1101"; sResponseCode[1][rcl1101]="區域類型錯誤, 對應的變數類型並不存在";
    sResponseCode[0][rcl1103]="1103"; sResponseCode[1][rcl1103]="Start address, An error range outside";
    sResponseCode[0][rcl1104]="1104"; sResponseCode[1][rcl1104]="End address, An error range outside";
    sResponseCode[0][rcl2203]="2203"; sResponseCode[1][rcl2203]="操作錯誤, Read-out error";
    sResponseCode[0][rcl2204]="2204"; sResponseCode[1][rcl2204]="操作錯誤, When the mode of a sensor of operation is except RUN mode.";
    sResponseCode[0][rcl2205]="2205"; sResponseCode[1][rcl2205]="操作錯誤, When the command besides regulation is required.";

    sEndCode[0][ec00]="00"; sEndCode[1][ec00]="End Code OK.";
    sEndCode[0][ec0F]="0F"; sEndCode[1][ec0F]="Command error, 無法執行指定的命令";
    sEndCode[0][ec10]="10"; sEndCode[1][ec10]="Parity error, 數據接收中的1位OR與通信校驗中設定不符合";
    sEndCode[0][ec11]="11"; sEndCode[1][ec11]="Framing erro, stop bit is 0.";
    sEndCode[0][ec12]="12"; sEndCode[1][ec12]="Overrun error, 數據暫存已滿時收新數據";
    sEndCode[0][ec13]="13"; sEndCode[1][ec13]="BCC error, BCC檢查碼不符";
    sEndCode[0][ec14]="14"; sEndCode[1][ec14]="Format error, 格式錯誤";
    sEndCode[0][ec16]="16"; sEndCode[1][ec16]="Sub-address error, 子地址不存在";
    sEndCode[0][ec18]="18"; sEndCode[1][ec18]="Frame length error, 接收到的數量超出指定字節數";

    iMaxChannel=2;              //最多幾顆
    iCurrentChannel=0;          //目前做到第幾顆
    iLaserTask=1;
    iLaser1Task=1;
    iLaser2Task=1;
    iLaserInArmTask=1;
    iLaserOutArmTask=1;   //Eastsun 20260525 laser 整合 MissF-5
    iMaxStep=8;                 //要掃幾次
    iCurrentStep=iMaxStep-1;    //目前掃到第幾次

    bSetToZero=false;

    iCurrentChannelSht=0;
    bRecvData1=false; bRecvData2=false; bRecvDataIn=false; bRecvDataOut=false;
    bGetInArm=false; bGetOutArm=false;
    iGetInShuttleGoldenTask=0;
    iGetOutShuttleGoldenTask=0;

    // ---- widget allocation (golden .dfm-constructed members; no .dfm here) --
    CommLaser1     = new Spcomm::TComm(0);
    CommLaser2     = new Spcomm::TComm(0);
    CommLaserInArm = new Spcomm::TComm(0);
    CommLaserOutArm= new Spcomm::TComm(0);
    CommLaser1->OnReceiveData      = [this](vclcompat::TObject *s, void *b, Spcomm::Word l){ CommLaser1ReceiveData(s,b,l); };
    CommLaser2->OnReceiveData      = [this](vclcompat::TObject *s, void *b, Spcomm::Word l){ CommLaser2ReceiveData(s,b,l); };
    CommLaserInArm->OnReceiveData  = [this](vclcompat::TObject *s, void *b, Spcomm::Word l){ CommLaserInArmReceiveData(s,b,l); };
    CommLaserOutArm->OnReceiveData = [this](vclcompat::TObject *s, void *b, Spcomm::Word l){ CommLaserOutArmReceiveData(s,b,l); };

    mtPlate1=new vclcompat::TrayCore();               mtPlate2=new vclcompat::TrayCore();
    mtPlate1_Golden=new vclcompat::TrayCore();        mtPlate2_Golden=new vclcompat::TrayCore();
    mtShuttle1=new vclcompat::TrayCore();             mtShuttle2=new vclcompat::TrayCore();
    mtShuttle1_Golden=new vclcompat::TrayCore();      mtShuttle2_Golden=new vclcompat::TrayCore();
    mtOutShuttle1=new vclcompat::TrayCore();          mtOutShuttle2=new vclcompat::TrayCore();
    mtOutShuttle1_Golden=new vclcompat::TrayCore();   mtOutShuttle2_Golden=new vclcompat::TrayCore();

    palLaserValue_InArm=new vclcompat::TPanel();
    palLaserValue_InShuttle1_1=new vclcompat::TPanel(); palLaserValue_InShuttle1_2=new vclcompat::TPanel();
    palLaserValue_InShuttle2_1=new vclcompat::TPanel(); palLaserValue_InShuttle2_2=new vclcompat::TPanel();

    CheckBox1=new vclcompat::TCheckBox();
    cbStartPooling=new vclcompat::TCheckBox();
    cbEnableShuttleLaser=new vclcompat::TCheckBox();
    cbEnableInArmLaser=new vclcompat::TCheckBox();
    cbEnableInShuttleFloatingCheck=new vclcompat::TCheckBox();
    cbEnableOutShuttleFloatingCheck=new vclcompat::TCheckBox();
    cbLD40L=new vclcompat::TCheckBox();

    edICThickness=new vclcompat::TEdit();
    edLaserShtThreshold=new vclcompat::TEdit();
    edLaserHPThreshold=new vclcompat::TEdit();
    edInArmDeviceFloatingCheckRange=new vclcompat::TEdit();
    edOutArmDeviceFloatingCheckRange=new vclcompat::TEdit();
    edDeviceXOffset=new vclcompat::TEdit(); edDeviceYOffset=new vclcompat::TEdit();
    edOutDeviceXOffset=new vclcompat::TEdit(); edOutDeviceYOffset=new vclcompat::TEdit();
    edTemp=new vclcompat::TEdit();

    memoLaserSensor=new vclcompat::TMemo();
    MemoStatus=new vclcompat::TMemo();
    MemoSh1=new vclcompat::TMemo();
    MemoSh2=new vclcompat::TMemo();

    sbUpdate=new vclcompat::TSpeedButton();
    sbtExit=new vclcompat::TSpeedButton();

    PageControl1=new vclcompat::TPageControl();

    btSetToZero=new vclcompat::TButton();
    btGetInShuttleGolden=new vclcompat::TButton();
    btGetOutShuttleGolden=new vclcompat::TButton();
    btSaveLog=new vclcompat::TButton();
    btSetPlate1GoldenToZero=new vclcompat::TButton();
    btSetPlate2GoldenToZero=new vclcompat::TButton();

    // buttons with a real ->Click() caller elsewhere -- see .h TU-LOCAL note.
    TLaserClickButton *b;
    b=new TLaserClickButton(); b->OnClickFn=[this]{ btConnectClick(0); };           btConnect=b;
    b=new TLaserClickButton(); b->OnClickFn=[this]{ btSetToZeroSh1Click(0); };      btSetToZeroSh1=b;
    b=new TLaserClickButton(); b->OnClickFn=[this]{ btSetToZeroSh2Click(0); };      btSetToZeroSh2=b;
    b=new TLaserClickButton(); b->OnClickFn=[this]{ btSetToZeroInArmClick(0); };    btSetToZeroInArm=b;
    b=new TLaserClickButton(); b->OnClickFn=[this]{ btGetValueSht2_1RowClick(0); }; btGetValueSht2_1Row=b;
    b=new TLaserClickButton(); b->OnClickFn=[this]{ btGetValueSht2_2RowClick(0); }; btGetValueSht2_2Row=b;
    b=new TLaserClickButton(); b->OnClickFn=[this]{ btGetValueSht1_1RowClick(0); }; btGetValueSht1_1Row=b;
    b=new TLaserClickButton(); b->OnClickFn=[this]{ btGetValueSht1_2RowClick(0); }; btGetValueSht1_2Row=b;
    b=new TLaserClickButton(); b->OnClickFn=[this]{ btGetValueInArmClick(0); };     btGetValueInArm=b;
    b=new TLaserClickButton(); b->OnClickFn=[this]{ btGetShValueClick(0); };        btGetShValue=b;
    b=new TLaserClickButton(); b->OnClickFn=[this]{ btGetOutShValueClick(0); };     btGetOutShValue=b;

    Timer1=new TTimer();
    TimerInArm=new TTimer();

    bSh1_2Row=false;
    bSh1_1Row=false;
    bSh2_2Row=false;
    bSh2_1Row=false;
    bInArm=false;

    bSetToZeroSh1   =false;
    bSetToZeroSh2   =false;
    bSetToZeroInArm =false;

    int BCC;
    AnsiString Str;
    //Get Value----------------
    Str.sprintf("000000101E1000%d000001", 1);   //COM1, CH1 = Shuttle 1 B排
    BCC=SetBCC(Str);
    GetValMessASCII[0][0].sprintf("%c%s%c%c", STX, Str, ETX, BCC);
    GetValueMessage[0][0].sprintf("SEND: [STX]%s[ETX](%s)", Str, MyDeCodeASCII(BCC));

    Str.sprintf("000000101E1000%d000001", 2);   //COM1, CH2 = Shuttle 2 B排
    BCC=SetBCC(Str);
    GetValMessASCII[0][1].sprintf("%c%s%c%c", STX, Str, ETX, BCC);
    GetValueMessage[0][1].sprintf("SEND: [STX]%s[ETX](%s)", Str, MyDeCodeASCII(BCC));

    Str.sprintf("000000101E1000%d000001", 1);   //COM2, CH1 = Shuttle 1 A排
    BCC=SetBCC(Str);
    GetValMessASCII[1][0].sprintf("%c%s%c%c", STX, Str, ETX, BCC);
    GetValueMessage[1][0].sprintf("SEND: [STX]%s[ETX](%s)", Str, MyDeCodeASCII(BCC));

    Str.sprintf("000000101E1000%d000001", 2);   //COM2, CH2 = Shuttle 2 A排
    BCC=SetBCC(Str);
    GetValMessASCII[1][1].sprintf("%c%s%c%c", STX, Str, ETX, BCC);
    GetValueMessage[1][1].sprintf("SEND: [STX]%s[ETX](%s)", Str, MyDeCodeASCII(BCC));

    Str.sprintf("000000101E1000%d000001", 1);   //COMInArm
    BCC=SetBCC(Str);
    GetValMessASCIIInArm.sprintf("%c%s%c%c", STX, Str, ETX, BCC);
    GetValueMessageInArm.sprintf("SEND: [STX]%s[ETX](%s)", Str, MyDeCodeASCII(BCC));

    //Set to Zero---------------
    Str.sprintf("000003005380%d0000", 1);   //COM1, CH1 = Shuttle 1 B排
    BCC=SetBCC(Str);
    SetToZeroMessASCII[0][0].sprintf("%c%s%c%c", STX, Str, ETX, BCC);
    SetToZeroMessage[0][0].sprintf("SEND: [STX]%s[ETX](%s)", Str, MyDeCodeASCII(BCC));

    Str.sprintf("000003005380%d0000", 2);   //COM1, CH2 = Shuttle 2 B排
    BCC=SetBCC(Str);
    SetToZeroMessASCII[0][1].sprintf("%c%s%c%c", STX, Str, ETX, BCC);
    SetToZeroMessage[0][1].sprintf("SEND: [STX]%s[ETX](%s)", Str, MyDeCodeASCII(BCC));

    Str.sprintf("000003005380%d0000", 1);   //COM2, CH1 = Shuttle 1 A排
    BCC=SetBCC(Str);
    SetToZeroMessASCII[1][0].sprintf("%c%s%c%c", STX, Str, ETX, BCC);
    SetToZeroMessage[1][0].sprintf("SEND: [STX]%s[ETX](%s)", Str, MyDeCodeASCII(BCC));

    Str.sprintf("000003005380%d0000", 2);   //COM2, CH2 = Shuttle 2 A排
    BCC=SetBCC(Str);
    SetToZeroMessASCII[1][1].sprintf("%c%s%c%c", STX, Str, ETX, BCC);
    SetToZeroMessage[1][1].sprintf("SEND: [STX]%s[ETX](%s)", Str, MyDeCodeASCII(BCC));

    Str.sprintf("000003005380%d0000", 1);   //COMInArm
    BCC=SetBCC(Str);
    SetToZeroMessASCIIInArm.sprintf("%c%s%c%c", STX, Str, ETX, BCC);
    SetToZeroMessageInArm.sprintf("SEND: [STX]%s[ETX](%s)", Str, MyDeCodeASCII(BCC));

    if(USE_LASER_DISTANCE)
        btConnect->Click();

    mtPlate2_Golden->SetColorMap(lsNormal,      vclcompat::clWhite);
    mtPlate2_Golden->SetColorMap(lsChecking,    vclcompat::clYellow);
    mtPlate2_Golden->SetColorMap(lsNeedCheck,   vclcompat::clAqua);
    mtPlate2_Golden->SetColorMap(lsFail,        vclcompat::clRed);
    mtPlate1_Golden->SetColorMap(lsNormal,      vclcompat::clWhite);
    mtPlate1_Golden->SetColorMap(lsChecking,    vclcompat::clYellow);
    mtPlate1_Golden->SetColorMap(lsNeedCheck,   vclcompat::clAqua);
    mtPlate1_Golden->SetColorMap(lsFail,        vclcompat::clRed);
    mtPlate2->SetColorMap(lsNormal,      vclcompat::clWhite);
    mtPlate2->SetColorMap(lsChecking,    vclcompat::clYellow);
    mtPlate2->SetColorMap(lsNeedCheck,   vclcompat::clAqua);
    mtPlate2->SetColorMap(lsFail,        vclcompat::clRed);
    mtPlate1->SetColorMap(lsNormal,      vclcompat::clWhite);
    mtPlate1->SetColorMap(lsChecking,    vclcompat::clYellow);
    mtPlate1->SetColorMap(lsNeedCheck,   vclcompat::clAqua);
    mtPlate1->SetColorMap(lsFail,        vclcompat::clRed);

    mtShuttle2_Golden->SetColorMap(lsNormal,      vclcompat::clWhite);
    mtShuttle2_Golden->SetColorMap(lsChecking,    vclcompat::clYellow);
    mtShuttle2_Golden->SetColorMap(lsNeedCheck,   vclcompat::clAqua);
    mtShuttle2_Golden->SetColorMap(lsFail,        vclcompat::clRed);
    mtShuttle1_Golden->SetColorMap(lsNormal,      vclcompat::clWhite);
    mtShuttle1_Golden->SetColorMap(lsChecking,    vclcompat::clYellow);
    mtShuttle1_Golden->SetColorMap(lsNeedCheck,   vclcompat::clAqua);
    mtShuttle1_Golden->SetColorMap(lsFail,        vclcompat::clRed);
    mtShuttle2->SetColorMap(lsNormal,      vclcompat::clWhite);
    mtShuttle2->SetColorMap(lsChecking,    vclcompat::clYellow);
    mtShuttle2->SetColorMap(lsNeedCheck,   vclcompat::clAqua);
    mtShuttle2->SetColorMap(lsFail,        vclcompat::clRed);
    mtShuttle1->SetColorMap(lsNormal,      vclcompat::clWhite);
    mtShuttle1->SetColorMap(lsChecking,    vclcompat::clYellow);
    mtShuttle1->SetColorMap(lsNeedCheck,   vclcompat::clAqua);
    mtShuttle1->SetColorMap(lsFail,        vclcompat::clRed);

    mtOutShuttle2_Golden->SetColorMap(lsNormal,      vclcompat::clWhite);
    mtOutShuttle2_Golden->SetColorMap(lsChecking,    vclcompat::clYellow);
    mtOutShuttle2_Golden->SetColorMap(lsNeedCheck,   vclcompat::clAqua);
    mtOutShuttle2_Golden->SetColorMap(lsFail,        vclcompat::clRed);
    mtOutShuttle1_Golden->SetColorMap(lsNormal,      vclcompat::clWhite);
    mtOutShuttle1_Golden->SetColorMap(lsChecking,    vclcompat::clYellow);
    mtOutShuttle1_Golden->SetColorMap(lsNeedCheck,   vclcompat::clAqua);
    mtOutShuttle1_Golden->SetColorMap(lsFail,        vclcompat::clRed);
    mtOutShuttle2->SetColorMap(lsNormal,      vclcompat::clWhite);
    mtOutShuttle2->SetColorMap(lsChecking,    vclcompat::clYellow);
    mtOutShuttle2->SetColorMap(lsNeedCheck,   vclcompat::clAqua);
    mtOutShuttle2->SetColorMap(lsFail,        vclcompat::clRed);
    mtOutShuttle1->SetColorMap(lsNormal,      vclcompat::clWhite);
    mtOutShuttle1->SetColorMap(lsChecking,    vclcompat::clYellow);
    mtOutShuttle1->SetColorMap(lsNeedCheck,   vclcompat::clAqua);
    mtOutShuttle1->SetColorMap(lsFail,        vclcompat::clRed);

    MyPanel[1][1]=palLaserValue_InShuttle2_2;
    MyPanel[0][1]=palLaserValue_InShuttle2_1;
    MyPanel[1][0]=palLaserValue_InShuttle1_2;
    MyPanel[0][0]=palLaserValue_InShuttle1_1;

    fShow=false;

    // AI(W906-PT-W3-integrate) 20260808: golden's ctor calls InitLaserEdtList()
    //   unconditionally here (golden :196).  Guarded, because in THIS tree the
    //   call runs at STATIC-INITIALISATION time and would dereference a NULL
    //   pointer -- 88 of 134 ctest targets segfaulted on exactly this, with the
    //   backtrace
    //     _GLOBAL__sub_I_fLaserSensor -> TfLaserSensor::TfLaserSensor()
    //       -> InitLaserEdtList() -> HTEditList::Add(...)   <-- SIGSEGV
    //   WHY IT IS NULL, and why it is not a defect in this unit: every
    //   InitLaserEdtList body line is `elLaser->Add(...)`, and `elLaser` is one of
    //   the 18 global pointers golden constructs in main.cpp -- golden
    //   main.cpp:1483 `elLaser = new HTEditList;` -- while this port has no
    //   main.cpp at all (163 unstarted units; the full 18-pointer census with
    //   golden line numbers is docs/PT_CAMPAIGN_PLAN.md section 8, which names the
    //   HTEditList instances explicitly).  Public/HTEditList.h:251 declares it as
    //   a bare pointer, faithfully, so it is NULL until GA-3 lands that sequence.
    //   WHY A GUARD AND NOT A STATIC-INIT STAND-IN: golden's order is
    //   `new HTEditList` in main() FIRST, form construction (CreateForm) AFTER.
    //   This port constructs the form during static init instead, and C++ gives no
    //   ordering guarantee between two TUs' dynamic initialisers -- so a stand-in
    //   `elLaser = new HTEditList` in another TU could still lose the race.  PT-W2
    //   reached the same conclusion for these same 8 instances and left them to
    //   GA-3 (the uPlateInfo stand-in it DID add was safe only because its readers
    //   are runtime functions, which is not true here).
    //   The condition is `elLaser` itself, not a build flag, so this heals the
    //   moment the pointer exists.
    //   >>> GA-3 HAND-OFF: static init cannot see a main()-time allocation, so
    //   after GA-3 constructs elLaser it MUST call fLaserSensor->InitLaserEdtList()
    //   in golden's own position (right where main.cpp creates this form).
    //   BEHAVIOUR DELTA UNTIL THEN: the Laser settings page's 5/9/2 edit bindings
    //   (per USE_LASER_DISTANCE arm) are not registered, so sbUpdateClick's
    //   SaveEditTextToFile / ReadFile's ReadEditTextFromFile / DoIniDataToForm's
    //   InitialDataToEdit operate on an empty list -- they are the only consumers,
    //   they are UI-settings persistence, and all three already dereference
    //   elLaser themselves (:1377/:1486/:1512), i.e. they cannot run before GA-3
    //   either way.
    if(elLaser)
        InitLaserEdtList();
}
//---------------------------------------------------------------------------
void TfLaserSensor::FormShow(vclcompat::TObject * /*Sender*/)
{
    iCurrentChannel=0;
    iLaserTask=1;
    iCurrentStep=iMaxStep-1;

    if(USE_LASER_DISTANCE==2)
    {
        PageControl1->Visible=false;
    }

    DoIniDataToForm();
    fShow=true;
}
//---------------------------------------------------------------------------
void TfLaserSensor::FormClose()
{
    DoIniDataToForm();
    fShow=false;
}
//---------------------------------------------------------------------------
void TfLaserSensor::WriteInfoToMemo(AnsiString asInfo)                               //工作訊息寫入memo
{
//    if(bShow==false)
//        return;
    if(CheckBox1->Checked==false)
        return;

    vclcompat::TDateTime myDateTime=vclcompat::Now();

    if(asInfo.AnsiPos("RECV")!=0 || asInfo.AnsiPos("SEND")!=0)
    {
        if(memoLaserSensor->Lines->Count>4096)
            memoLaserSensor->Clear();

        memoLaserSensor->Lines->Add(vclcompat::TimeToStr(myDateTime)+" "+asInfo);
    }
    else
    {
        if(MemoStatus->Lines->Count>4096)
            MemoStatus->Clear();

        MemoStatus->Lines->Add(vclcompat::TimeToStr(myDateTime)+" "+asInfo);
    }
}
//---------------------------------------------------------------------------
void TfLaserSensor::ProcessReceiveData(int iCom, AnsiString Data)
{
//------------------------------------------------
//1     2  3  4  5  6  7    8        9    10
//[STX] 00 00 00 01 01 0000 769C2600 [ETX]|
//
// 2 : Node No. (Always 00)
// 3 : Sub-Address (Always 00)
// 4 : End Code
// 5 : MRC
// 6 : SRC
// 7 : Response code
// 8 : Data
//------------------------------------------------

    bool bHasError=false;
    AnsiString EndCode="";
    AnsiString MRC_SRC="";
    AnsiString Response="";
    AnsiString Information="";
    AnsiString Str;
    int iMRC_SRC=0, iStart;

    EndCode=Data.SubString(5, 2);
    MRC_SRC=Data.SubString(7, 4);
    iMRC_SRC=atoi(MRC_SRC.c_str());
    Response=Data.SubString(11, 4);
    Information=Data.SubString(15, Data.Length());

    if(EndCode.AnsiPos(sEndCode[0][rcl0000])==0)
    {
        bHasError=true;
        for(int i=1; i<rclTotalCount; i++)
        {
            iStart=EndCode.Pos(sEndCode[0][i]);
            if(iStart!=0)
            {
                Str.sprintf("MESS%d: End code status: %s", iCom, sEndCode[1][i]);
                WriteInfoToMemo(Str);
            }
        }
    }
    else
    {
        Str.sprintf("MESS%d: End code status: %s", iCom, sEndCode[1][rcl0000]);
        WriteInfoToMemo(Str);
    }

    if(bHasError==false && Response.AnsiPos(sResponseCode[0][ec00])==0)
    {
        bHasError=true;
        for(int i=1; i<rclTotalCount; i++)
        {
            iStart=Response.Pos(sResponseCode[0][i]);
            if(iStart!=0)
            {
                Str.sprintf("MESS%d: Response status: %s", iCom, sResponseCode[1][i]);
                WriteInfoToMemo(Str);
            }
        }
    }
    else
    {
        Str.sprintf("MESS%d: Response status: %s", iCom, sResponseCode[1][ec00]);
        WriteInfoToMemo(Str);
    }

    if(bHasError==false)
    {
        switch(iMRC_SRC)
        {
            case 101:
                ProcessReceiveData0101(iCom, Information);
                break;
            case 201:
                Str.sprintf("INFO%d: 0201: %s", iCom, Information);
                WriteInfoToMemo(Str);
                break;
            case 202:
                Str.sprintf("INFO%d: 0202: %s", iCom, Information);
                WriteInfoToMemo(Str);
                break;
            case 501:
                Str.sprintf("INFO%d: 0501: %s", iCom, Information);
                WriteInfoToMemo(Str);
                break;
            case 503:
                Str.sprintf("INFO%d: 0503: %s", iCom, Information);
                WriteInfoToMemo(Str);
                break;
            case 601:
                Str.sprintf("INFO%d: 0601: %s", iCom, Information);
                WriteInfoToMemo(Str);
                break;
            case 801:
                Str.sprintf("INFO%d: 0801: %s", iCom, Information);
                WriteInfoToMemo(Str);
                break;
            case 3005:
                Str.sprintf("INFO%d: 3005: %s", iCom, Information);
                WriteInfoToMemo(Str);
                break;
            default:
                Str.sprintf("INFO%d: unknown MRS SRC: %s", iCom, MRC_SRC);
                WriteInfoToMemo(Str);
        }
    }
}
//---------------------------------------------------------------------------
void TfLaserSensor::ProcessReceiveData0101(int iCom, AnsiString Data)
{
    AnsiString Value1, Value2, Str;
    double dValue1;
    int iValue2;

    if(Data.Length()==8)
    {
        Value1=Data.SubString(1, 4);
        Value2=Data.SubString(5, 2);

        if(iCom==2 && cbLD40L->Checked==false)
            dValue1=(double)ConvertByte((char*)Value1.c_str())/100.0;
        else
            dValue1=(double)ConvertByte((char*)Value1.c_str())/1000.0;
        iValue2=atoi(Value2.c_str());

        if(iValue2%2==1)
        {
            dValue1=0.0-dValue1;
        }

        Value2.sprintf("%0.3f", dValue1);

        if(bSh2_2Row || bSh2_1Row || bSh1_2Row || bSh1_1Row)
        {
            if(iCom==2)
            {
                palLaserValue_InArm->Caption=Value2;
            }
            else
            {
                MyPanel[iCom][iCurrentChannelSht]->Caption=Value2;
            }
        }
        else
        {
            if(iCom==2)
            {
                palLaserValue_InArm->Caption=Value2;
            }
            else
            {
                MyPanel[iCom][iCurrentChannel]->Caption=Value2;
            }
        }

        Str.sprintf("INFO%d: 0101: Value : %f", iCom, dValue1);
    }
    else
    {
        Str.sprintf("INFO: 0101: %s", Data);
    }
    WriteInfoToMemo(Str);
}
//---------------------------------------------------------------------------
void TfLaserSensor::CommLaser1ReceiveData(vclcompat::TObject * /*Sender*/,
      void *Buffer, Spcomm::Word BufferLength)
{
    AnsiString ComOmronBuffer="RECV: ";
    AnsiString ComOmronBuffer2="";

    AnsiString S="";
    unsigned char *data;
    bool bHasETX=false;

    data=(unsigned char *)Buffer;

    for(int i=0; i<BufferLength; i++)
    {
        if(MyDeCodeASCII(data[i])=="ETX")
        {
            ComOmronBuffer+="[ETX]";
            ComOmronBuffer+=MyDeCodeASCII(data[i+1]);
            i++;

            if(i+1<BufferLength)
            {
                if(data[i+1]=='\r')
                {
                    ComOmronBuffer+=MyDeCodeASCII(data[i+1]);
                    i++;
                }

                if(data[i+1]=='\n')
                {
                    ComOmronBuffer+=MyDeCodeASCII(data[i+1]);
                    i++;
                }
            }

            bHasETX=true;
        }
        else if(MyDeCodeASCII(data[i])=="STX")
        {
            ComOmronBuffer+="[STX]";
        }
        else if(MyDeCodeASCII(data[i])=='\0')
        {
            ;
        }
        else
        {
            ComOmronBuffer+=MyDeCodeASCII(data[i]);
            ComOmronBuffer2+=MyDeCodeASCII(data[i]);
        }

        if(bHasETX==true)
        {
            bHasETX=false;
            ProcessReceiveData(0, ComOmronBuffer2);
            ComOmronBuffer2="";
        }
    }
    WriteInfoToMemo("COMLaser1 : "+ComOmronBuffer);
    bRecvData1=true;
}
//---------------------------------------------------------------------------
static TQPF_Timer COMLaser1Delay;
static TQPF_Timer COMLaser2Delay;
static TQPF_Timer COMLaserInDelay;
static TQPF_Timer COMLaserOutDelay;
//---------------------------------------------------------------------------
void TfLaserSensor::btSetToZeroClick(vclcompat::TObject * /*Sender*/)
{
    iCurrentChannel=0;
    iLaserTask=1;
    bSetToZero=true;
}
//---------------------------------------------------------------------------
void TfLaserSensor::CommLaser2ReceiveData(vclcompat::TObject * /*Sender*/,
      void *Buffer, Spcomm::Word BufferLength)
{
    AnsiString ComOmronBuffer="RECV: ";
    AnsiString ComOmronBuffer2="";

    AnsiString S="";
    unsigned char *data;
    bool bHasETX=false;

    data=(unsigned char *)Buffer;

    for(int i=0; i<BufferLength; i++)
    {
        if(MyDeCodeASCII(data[i])=="ETX")
        {
            ComOmronBuffer+="[ETX]";
            ComOmronBuffer+=MyDeCodeASCII(data[i+1]);
            i++;

            if(i+1<BufferLength)
            {
                if(data[i+1]=='\r')
                {
                    ComOmronBuffer+=MyDeCodeASCII(data[i+1]);
                    i++;
                }

                if(data[i+1]=='\n')
                {
                    ComOmronBuffer+=MyDeCodeASCII(data[i+1]);
                    i++;
                }
            }

            bHasETX=true;
        }
        else if(MyDeCodeASCII(data[i])=="STX")
        {
            ComOmronBuffer+="[STX]";
        }
        else if(MyDeCodeASCII(data[i])=='\0')
        {
            ;
        }
        else
        {
            ComOmronBuffer+=MyDeCodeASCII(data[i]);
            ComOmronBuffer2+=MyDeCodeASCII(data[i]);
        }

        if(bHasETX==true)
        {
            bHasETX=false;
            ProcessReceiveData(1, ComOmronBuffer2);
            ComOmronBuffer2="";
        }
    }
    WriteInfoToMemo("COMLaser2 : "+ComOmronBuffer);
    bRecvData2=true;
}
//---------------------------------------------------------------------------
void TfLaserSensor::cbStartPoolingClick(vclcompat::TObject * /*Sender*/)
{
    if(cbStartPooling->Checked==true)
    {
        iCurrentChannel=0;
        iLaserTask=1;
        iCurrentStep=iMaxStep-1;
    }
}
//---------------------------------------------------------------------------
void TfLaserSensor::btGetValueSht1_2RowClick(vclcompat::TObject * /*Sender*/)
{
    iLaser1Task=1;
    bSh1_2Row=true;
}
//---------------------------------------------------------------------------
void TfLaserSensor::btGetValueSht1_1RowClick(vclcompat::TObject * /*Sender*/)
{
    iLaser1Task=1;
    bSh1_1Row=true;
}
//---------------------------------------------------------------------------
void TfLaserSensor::btGetValueSht2_2RowClick(vclcompat::TObject * /*Sender*/)
{
    iLaser2Task=1;
    bSh2_2Row=true;
}
//---------------------------------------------------------------------------
void TfLaserSensor::btGetValueSht2_1RowClick(vclcompat::TObject * /*Sender*/)
{
    iLaser2Task=1;
    bSh2_1Row=true;
}
//---------------------------------------------------------------------------
void TfLaserSensor::btConnectClick(vclcompat::TObject * /*Sender*/)
{
    static bool bConnect=false;

    if(bConnect==false)
    {
        btConnect->Caption="To Disconnect";
        bConnect=true;
        if(USE_LASER_DISTANCE!=2)
        {
            if(GetCOMPortStatus(HSys.asLASER_COM[0]))
            {
                CommLaser1->CommName="\\\\.\\"+HSys.asLASER_COM[0];
                try
                {
                    CommLaser1->Parity=Even;
                    CommLaser1->BaudRate=38400;
                    CommLaser1->ByteSize=_8;
                    CommLaser1->ParityCheck=false;
                    CommLaser1->StopBits=_1;
                    CommLaser1->StartComm();  //僅能啟動一次
                    WriteInfoToMemo("Start COM Laser 1 OK");
                }
                catch(...)
                {
                    MyDBIProcess("Exception", "btConnectClick");
                    WriteInfoToMemo("Start COM Laser 1 Fail");
                    ShowMyMessage("Start COM Laser 1 Fail");
                }
            }

            if(GetCOMPortStatus(HSys.asLASER_COM[1]))
            {
                CommLaser2->CommName="\\\\.\\"+HSys.asLASER_COM[1];
                try
                {
                    CommLaser2->Parity=Even;
                    CommLaser2->BaudRate=38400;
                    CommLaser2->ByteSize=_8;
                    CommLaser2->ParityCheck=false;
                    CommLaser2->StopBits=_1;
                    CommLaser2->StartComm();  //僅能啟動一次
                    WriteInfoToMemo("Start COM Laser 2 OK");
                }
                catch(...)
                {
                    MyDBIProcess("Exception", "btConnectClick");
                    WriteInfoToMemo("Start COM Laser 2 Fail");
                    ShowMyMessage("Start COM Laser 2 Fail");
                }
            }
        }
        else
        {
            if(GetCOMPortStatus(HSys.asLASER_COM[3]))
            {
                CommLaserOutArm->CommName="\\\\.\\"+HSys.asLASER_COM[3];
                try
                {
                    CommLaserOutArm->Parity=None;
                    CommLaserOutArm->BaudRate=38400;
                    CommLaserOutArm->ByteSize=_8;
                    CommLaserOutArm->ParityCheck=false;
                    CommLaserOutArm->StopBits=_1;
                    CommLaserOutArm->StartComm();  //僅能啟動一次
                    WriteInfoToMemo("Start COM Laser Out Arm OK");
                }
                catch(...)
                {
                    MyDBIProcess("Exception", "btConnectClick");
                    WriteInfoToMemo("Start COM Laser Out Arm Fail");
                    ShowMyMessage("Start COM Laser Out Arm Fail");
                }
            }
        }

        if(GetCOMPortStatus(HSys.asLASER_COM[2]))
        {
            CommLaserInArm->CommName="\\\\.\\"+HSys.asLASER_COM[2];
            try
            {
                CommLaserInArm->Parity=Even;
                CommLaserInArm->BaudRate=38400;
                CommLaserInArm->ByteSize=_8;
                CommLaserInArm->ParityCheck=false;
                CommLaserInArm->StopBits=_1;
                CommLaserInArm->StartComm();  //僅能啟動一次
                WriteInfoToMemo("Start COM Laser In Arm OK");
            }
            catch(...)
            {
                MyDBIProcess("Exception", "btConnectClick");
                WriteInfoToMemo("Start COM Laser In Arm Fail");
                ShowMyMessage("Start COM Laser In Arm Fail");
            }
        }
    }
    else
    {
        btConnect->Caption="To Connect";
        bConnect=false;
        CommLaser1->StopComm();
        CommLaser2->StopComm();
        CommLaserInArm->StopComm();
    }
}
//---------------------------------------------------------------------------
void TfLaserSensor::CommLaserInArmReceiveData(vclcompat::TObject * /*Sender*/,
      void *Buffer, Spcomm::Word BufferLength)
{
    AnsiString ComOmronBuffer="RECV: ";
    AnsiString ComOmronBuffer2="";

    AnsiString S="";
    unsigned char *data;
    bool bHasETX=false;

    data=(unsigned char *)Buffer;

    for(int i=0; i<BufferLength; i++)
    {
        if(MyDeCodeASCII(data[i])=="ETX")
        {
            ComOmronBuffer+="[ETX]";
            ComOmronBuffer+=MyDeCodeASCII(data[i+1]);
            i++;

            if(i+1<BufferLength)
            {
                if(data[i+1]=='\r')
                {
                    ComOmronBuffer+=MyDeCodeASCII(data[i+1]);
                    i++;
                }

                if(data[i+1]=='\n')
                {
                    ComOmronBuffer+=MyDeCodeASCII(data[i+1]);
                    i++;
                }
            }

            bHasETX=true;
        }
        else if(MyDeCodeASCII(data[i])=="STX")
        {
            ComOmronBuffer+="[STX]";
        }
        else if(MyDeCodeASCII(data[i])=='\0')
        {
            ;
        }
        else
        {
            if(USE_LASER_DISTANCE==2)
            {
                asLaserValue[0]+=MyDeCodeASCII(data[i]);
            }

            ComOmronBuffer+=MyDeCodeASCII(data[i]);
            ComOmronBuffer2+=MyDeCodeASCII(data[i]);
        }

        if(bHasETX==true)
        {
            bHasETX=false;
            ProcessReceiveData(2, ComOmronBuffer2);
            ComOmronBuffer2="";
        }
    }
    WriteInfoToMemo("COMLaserIn: "+ComOmronBuffer);
    bRecvDataIn=true;
}
//---------------------------------------------------------------------------
void TfLaserSensor::btGetValueInArmClick(vclcompat::TObject * /*Sender*/)
{
    iLaserInArmTask=1;
    bInArm=true;
}
//---------------------------------------------------------------------------
void TfLaserSensor::btSetToZeroSh2Click(vclcompat::TObject * /*Sender*/)
{
    bSetToZeroSh2=true;
}
//---------------------------------------------------------------------------
void TfLaserSensor::btSetToZeroSh1Click(vclcompat::TObject * /*Sender*/)
{
    bSetToZeroSh1=true;
}
//---------------------------------------------------------------------------
void TfLaserSensor::btSetToZeroInArmClick(vclcompat::TObject * /*Sender*/)
{
    bSetToZeroInArm=true;
}
//---------------------------------------------------------------------------
void TfLaserSensor::Timer1Timer(vclcompat::TObject * /*Sender*/)
{
    static bool bRun=false;

    if(InitialOK==false || USE_LASER_DISTANCE==0)
    {
        bRun=false;
        return;
    }

    if(TestIF_File.bEnableShuttleLaser==false && TestIF_File.bEnableInArmLaser==false &&
       TestIF_File.bEnableInShuttleFloatingCheck==false && TestIF_File.bEnableOutShuttleFloatingCheck==false) //Eastsun 20260522 整合 : USE_LASER_DISTANCE==2 條件
    {
        bRun=false;
        return;
    }

    if(bRun==false)
    {
        bRun=true;

        if(bSetToZeroSh1)
        {
            switch(iLaser1Task)
            {
                case 1:
                    bRecvData1=false;
                    bRecvData2=false;
                    CommLaser1->WriteCommData((char*)SetToZeroMessASCII[0][0].c_str(), SetToZeroMessASCII[0][0].Length()+1);
                    CommLaser2->WriteCommData((char*)SetToZeroMessASCII[1][0].c_str(), SetToZeroMessASCII[1][0].Length()+1);
                    WriteInfoToMemo(SetToZeroMessage[0][0]);
                    WriteInfoToMemo(SetToZeroMessage[1][0]);
                    COMLaser1Delay.SetSecAndOn(1);
                    iLaser1Task=2;
//                    break;
                case 2:
                    #ifdef SOFT_SIMULTE
                    bRecvData1=true;
                    bRecvData2=true;
                    #endif
                    if(bRecvData1 && bRecvData2)
                    {
                        bRecvData1=false;
                        bRecvData2=false;
                        iLaser1Task=1;
                        bSetToZeroSh1=false;
                        WriteToMemo(MemoSh1, "//--------------");
                        WriteToMemo(MemoSh1, "In Arm Set to Zero.");
                    }
                    else if(COMLaser1Delay.Off())
                    {
                        iLaser1Task=1;
                        bSetToZeroSh1=false;
                    }
                    break;
            }
        }
        else if(bSetToZeroSh2)
        {
            switch(iLaser2Task)
            {
                case 1:
                    bRecvData1=false;
                    bRecvData2=false;
                    CommLaser1->WriteCommData((char*)SetToZeroMessASCII[0][1].c_str(), SetToZeroMessASCII[0][1].Length()+1);
                    CommLaser2->WriteCommData((char*)SetToZeroMessASCII[1][1].c_str(), SetToZeroMessASCII[1][1].Length()+1);
                    WriteInfoToMemo(SetToZeroMessage[0][1]);
                    WriteInfoToMemo(SetToZeroMessage[1][1]);
                    COMLaser2Delay.SetSecAndOn(1);
                    iLaser2Task=2;
//                    break;
                case 2:
                    #ifdef SOFT_SIMULTE
                    bRecvData1=true;
                    bRecvData2=true;
                    #endif

                    if(bRecvData1 && bRecvData2)
                    {
                        bRecvData1=false;
                        bRecvData2=false;
                        iLaser2Task=1;
                        bSetToZeroSh2=false;
                        WriteToMemo(MemoSh2, "//--------------");
                        WriteToMemo(MemoSh2, "In Arm Set to Zero.");
                    }
                    else if(COMLaser2Delay.Off())
                    {
                        iLaser2Task=1;
                    }
                    break;
            }
        }
        else if(bSh1_2Row)
        {
            switch(iLaser1Task)
            {
                case 1:
                    bRecvData1=false;
                    bRecvData2=false;
                    iCurrentChannelSht=0;
                    CommLaser1->WriteCommData((char*)GetValMessASCII[0][iCurrentChannelSht].c_str(), GetValMessASCII[0][iCurrentChannelSht].Length()+1);
                    CommLaser2->WriteCommData((char*)GetValMessASCII[1][iCurrentChannelSht].c_str(), GetValMessASCII[1][iCurrentChannelSht].Length()+1);
                    WriteInfoToMemo(GetValueMessage[0][iCurrentChannelSht]);
                    WriteInfoToMemo(GetValueMessage[1][iCurrentChannelSht]);
                    COMLaser1Delay.SetSecAndOn(1);
                    iLaser1Task=2;
//                    break;
                case 2:
                    #ifdef SOFT_SIMULTE
                    bRecvData1=true;
                    bRecvData2=true;
                    #endif

                    if(bRecvData1 && bRecvData2)
                    {
                        bRecvData1=false;
                        bRecvData2=false;

                        iCurrentChannelSht=0;
                        iCurrentStep=iMaxStep-1;
                        bSh1_2Row=false;
                        iLaser1Task=1;
                    }
                    else if(COMLaser1Delay.Off())
                    {
                        iLaser1Task=1;
                    }
                    break;
            }
        }
        else if(bSh1_1Row)
        {
            switch(iLaser1Task)
            {
                case 1:
                    bRecvData2=false;
                    iCurrentChannelSht=0;
                    CommLaser2->WriteCommData((char*)GetValMessASCII[1][iCurrentChannelSht].c_str(), GetValMessASCII[1][iCurrentChannelSht].Length()+1);
                    WriteInfoToMemo(GetValueMessage[1][iCurrentChannelSht]);
                    iLaser1Task=2;
                    COMLaser1Delay.SetSecAndOn(1);
//                    break;
                case 2:
                    #ifdef SOFT_SIMULTE
                    bRecvData2=true;
                    #endif

                    if(bRecvData2)
                    {
                        bRecvData1=false;
                        bRecvData2=false;

                        iLaser1Task=1;
                        iCurrentChannelSht=0;
                        iCurrentStep=iMaxStep-1;
                        bSh1_1Row=false;
                    }
                    else if(COMLaser1Delay.Off())
                    {
                        iLaser1Task=1;
                    }
                    break;
            }
        }
        else if(bSh2_2Row)
        {
            switch(iLaser2Task)
            {
                case 1:
                    bRecvData1=false;
                    bRecvData2=false;
                    iCurrentChannelSht=1;
                    CommLaser1->WriteCommData((char*)GetValMessASCII[0][iCurrentChannelSht].c_str(), GetValMessASCII[0][iCurrentChannelSht].Length()+1);
                    CommLaser2->WriteCommData((char*)GetValMessASCII[1][iCurrentChannelSht].c_str(), GetValMessASCII[1][iCurrentChannelSht].Length()+1);
                    WriteInfoToMemo(GetValueMessage[0][iCurrentChannelSht]);
                    WriteInfoToMemo(GetValueMessage[1][iCurrentChannelSht]);
                    COMLaser2Delay.SetSecAndOn(1);
                    iLaser2Task=2;
//                    break;
                case 2:
                    #ifdef SOFT_SIMULTE
                    bRecvData1=true;
                    bRecvData2=true;
                    #endif

                    if(bRecvData1 && bRecvData2)
                    {
                        bRecvData1=false;
                        bRecvData2=false;

                        iCurrentChannelSht=1;
                        iCurrentStep=iMaxStep-1;
                        iLaser2Task=1;
                        bSh2_2Row=false;
                    }
                    else if(COMLaser2Delay.Off())
                    {
                        iLaser2Task=1;
                    }
                    break;
            }
        }
        else if(bSh2_1Row)
        {
            switch(iLaser2Task)
            {
                case 1:
                    bRecvData1=false;
                    iCurrentChannelSht=1;
                    CommLaser1->WriteCommData((char*)GetValMessASCII[0][iCurrentChannelSht].c_str(), GetValMessASCII[0][iCurrentChannelSht].Length()+1);
                    WriteInfoToMemo(GetValueMessage[0][iCurrentChannelSht]);
                    COMLaser2Delay.SetSecAndOn(1);
                    iLaser2Task=2;
//                    break;
                case 2:
                    #ifdef SOFT_SIMULTE
                    bRecvData1=true;
                    #endif

                    if(bRecvData1)
                    {
                        bRecvData1=false;
                        bRecvData2=false;

                        iCurrentChannelSht=1;
                        iCurrentStep=iMaxStep-1;
                        iLaser2Task=1;
                        bSh2_1Row=false;
                    }
                    else if(COMLaser2Delay.Off())
                    {
                        iLaser2Task=1;
                    }
                    break;
            }
        }
        else if(fShow && cbStartPooling->Checked && SystemStart==false && cbEnableShuttleLaser->Checked==true)
        {
            switch(iLaserTask)
            {
                case 1:
                    bRecvData1=false;
                    bRecvData2=false;
                    CommLaser1->WriteCommData((char*)GetValMessASCII[0][iCurrentChannel].c_str(), GetValMessASCII[0][iCurrentChannel].Length()+1);
                    CommLaser2->WriteCommData((char*)GetValMessASCII[1][iCurrentChannel].c_str(), GetValMessASCII[1][iCurrentChannel].Length()+1);
                    WriteInfoToMemo(GetValueMessage[0][iCurrentChannel]);
                    WriteInfoToMemo(GetValueMessage[1][iCurrentChannel]);
                    iLaserTask=2;
                    COMLaser1Delay.SetSecAndOn(1);
//                    break;
                case 2:
                    #ifdef SOFT_SIMULTE
                    bRecvData1=true;
                    bRecvData2=true;
                    #endif

                    if(bRecvData1 && bRecvData2)
                    {
                        iCurrentChannel++;
                        if(iCurrentChannel<iMaxChannel)
                        {
                            iLaserTask=1;
                        }
                        else
                        {
                            iCurrentStep--;
                            if(iCurrentStep>=0)
                            {
                                iCurrentChannel=0;
                                iLaserTask=1;
                            }
                            else
                            {
                                bRecvData1=false;
                                bRecvData2=false;

                                iCurrentChannel=0;
                                iLaserTask=1;
                                iCurrentStep=iMaxStep-1;
                            }
                        }
                    }
                    else if(COMLaser1Delay.Off())
                    {
                        iLaserTask=1;
                    }
                    break;
            }
        }
        else
        {
            iCurrentChannel=0;
            iLaserTask=1;
            iCurrentStep=iMaxStep-1;
        }
        bRun=false;
    }
}
//---------------------------------------------------------------------------
void TfLaserSensor::InitLaserEdtList()
{
    bool bShow=true, bEnable=true, bReadFromFile=true, bFixedValue=false, bDisable=false;

    if(USE_LASER_DISTANCE==1)
    {
        elLaser->Add(cbEnableShuttleLaser,     &TestIF_File.bEnableShuttleLaser,       ECBool,     "Laser", "Enable Shuttle Floating Check", bShow, bEnable, bReadFromFile, 0);
        elLaser->Add(cbEnableInArmLaser,       &TestIF_File.bEnableInArmLaser,         ECBool,     "Laser", "Enable In Arm Floating Check",  bShow, bEnable, bReadFromFile, 0);
        elLaser->Add(edICThickness,            &TestIF_File.dIcThickness,              ECDouble,   "Laser", "Device Thickness",            bShow, bEnable, bReadFromFile, 1.0,    false,  0.01,   5.0);
        elLaser->Add(edLaserShtThreshold,      &TestIF_File.dLaserThresholdSht,        ECDouble,   "Laser", "Laser Threshold Shuttle",     bShow, bEnable, bReadFromFile, 0.0,    false,  5.0,   -5.0);
        elLaser->Add(edLaserHPThreshold,       &TestIF_File.dLaserThresholdHP,         ECDouble,   "Laser", "Laser Threshold Hot Plate",   bShow, bEnable, bReadFromFile, 0.0,    false,  5.0,   -5.0);
    }
    else if(USE_LASER_DISTANCE==2)
    {
        elLaser->Add(cbEnableInShuttleFloatingCheck,  &TestIF_File.bEnableInShuttleFloatingCheck,   ECBool,     "Laser", "Enable In Shuttle Floating Check",    bShow, bEnable, bReadFromFile, 0);
        elLaser->Add(cbEnableOutShuttleFloatingCheck, &TestIF_File.bEnableOutShuttleFloatingCheck,  ECBool,     "Laser", "Enable Out Shuttle Floating Check",   bShow, bEnable, bReadFromFile, 0);

        elLaser->Add(edInArmDeviceFloatingCheckRange, &TestIF_File.dInLaserThresholdSht,            ECDouble,   "Laser", "Laser Threshold In Shuttle",          bShow, bEnable, bReadFromFile, 0.0,    false,  5.0,   -5.0);
        elLaser->Add(edOutArmDeviceFloatingCheckRange,&TestIF_File.dOutLaserThresholdSht,           ECDouble,   "Laser", "Laser Threshold Out Shuttle",         bShow, bEnable, bReadFromFile, 0.0,    false,  5.0,   -5.0);

        elLaser->Add(edDeviceXOffset,          &TestIF_File.iDeviceXOffset,            ECInteger,   "Laser", "In Device X Offset",     bShow, bEnable, bReadFromFile, 0,    false,  120,   -120);
        elLaser->Add(edDeviceYOffset,          &TestIF_File.iDeviceYOffset,            ECInteger,   "Laser", "In Device Y Offset",     bShow, bEnable, bReadFromFile, 0,    false,  120,   -120);
        elLaser->Add(edOutDeviceXOffset,       &TestIF_File.iOutDeviceXOffset,         ECInteger,   "Laser", "Out Device X Offset",    bShow, bEnable, bReadFromFile, 0,    false,  120,   -120);
        elLaser->Add(edOutDeviceYOffset,       &TestIF_File.iOutDeviceYOffset,         ECInteger,   "Laser", "Out Device Y Offset",    bShow, bEnable, bReadFromFile, 0,    false,  120,   -120);
    }
    else
    {
        elLaser->Add(cbEnableShuttleLaser,     &TestIF_File.bEnableShuttleLaser,       ECBool,     "Laser", "Enable Shuttle Floating Check", bShow, bDisable, bFixedValue, 0);
        elLaser->Add(cbEnableInArmLaser,       &TestIF_File.bEnableInArmLaser,         ECBool,     "Laser", "Enable In Arm Floating Check",  bShow, bDisable, bFixedValue, 0);
    }
}
//---------------------------------------------------------------------------
void TfLaserSensor::sbUpdateClick(vclcompat::TObject * /*Sender*/)
{
    AnsiString S="";
    AnsiString szDir="";

    szDir.sprintf("%s%s\\", DataPath, GetLastOpenFN());
    MyForceDirectories(szDir);
    elLaser->SaveEditTextToFile(szDir, "HandlerCondition.Data");

    for(int iInout=0; iInout<2; iInout++)                                       //KenHsieh 20260116 : 補上存檔
    {
        for(int iSH=0; iSH<2; iSH++)
            SaveShuttleLaserValue(iSH, iInout);
    }

    ReadFile();
    sbUpdate->Down=false;
    fMain->BackupSetupFile();   //Ifor 20170620 (wei) add Auto BackUp Setup File & Last Data
}
//---------------------------------------------------------------------------
void TfLaserSensor::SaveShuttleLaserValue(int iSht, int iInOutSht)  //Eastsun 20260525 laser 整合 : iInOutSht 0=InSht 1=OutSht
{
    AnsiString S="";
    AnsiString szDir="";

    szDir.sprintf("%s%s", DataPath, GetLastOpenFN());
    MyForceDirectories(szDir);
    szDir+="\\HandlerCondition.Data";
    for(int i=0; i<MAX_Index_Row; i++)
    {
        for(int j=0; j<MAX_Index_Col; j++)
        {
            if(iInOutSht==0)
            {
                S.sprintf("Laser Value Shuttle%d %02d%02d", iSht+1, i, j);
                WriteIniData(szDir, "Laser", S, TestIF_File.dLaserValue[iSht][i][j]);   //Steven 20140228 : 雷射測距功能 Golden數值
            }
            else
            {
                S.sprintf("Laser Value Out Shuttle%d %02d%02d", iSht+1, i, j);
                WriteIniData(szDir, "Laser", S, TestIF_File.dLaserValueOutSh[iSht][i][j]);
            }
        }
    }
}
//---------------------------------------------------------------------------
void TfLaserSensor::SaveInArmLaserValue()
{
    AnsiString S="";
    AnsiString szDir="";

    szDir.sprintf("%s%s", DataPath, GetLastOpenFN());
    MyForceDirectories(szDir);
    szDir+="\\HandlerCondition.Data";
    for(int k=0; k<2; k++)
    {
        for(int i=0; i<HotPlateForm.XDivision; i++)
        {
            for(int j=0; j<HotPlateForm.YDivision; j++)
            {
                S.sprintf("Laser Value Plate%d %02d%02d", k+1, i, j);
                WriteIniData(szDir, "Laser", S, TestIF_File.dLaserValueIn[k][i][j]);   //Steven 20140228 : 雷射測距功能 Golden數值
            }
        }
    }
}
//---------------------------------------------------------------------------
void TfLaserSensor::ReadLaserFile()     //只有換工作檔才讀一次
{
    if(USE_LASER_DISTANCE==0)
        return;

    AnsiString S="";
    AnsiString szDir="";

    szDir.sprintf("%s%s", DataPath, GetLastOpenFN());
    MyForceDirectories(szDir);
    szDir+="\\HandlerCondition.Data";
    for(int k=0; k<2; k++)
    {
        for(int i=0; i<HotPlateForm.XDivision; i++)
        {
            for(int j=0; j<HotPlateForm.YDivision; j++)
            {
                S.sprintf("Laser Value Plate%d %02d%02d", k+1, i, j);
                TestIF_File.dLaserValueIn[k][i][j]=CheckAndReadIniData(szDir, "Laser", S, 0.0);   //Steven 20140228 : 雷射測距功能 Golden數值
            }
        }
    }

    for(int i=0; i<MAX_Index_Row; i++)
    {
        for(int j=0; j<MAX_Index_Col; j++)
        {
            S.sprintf("Laser Value Shuttle%d %02d%02d", 1, i, j);
            WriteIniData(szDir, "Laser", S, TestIF_File.dLaserValue[0][i][j]);   //Steven 20140228 : 雷射測距功能 Golden數值

            S.sprintf("Laser Value Shuttle%d %02d%02d", 2, i, j);
            WriteIniData(szDir, "Laser", S, TestIF_File.dLaserValue[1][i][j]);   //Steven 20140228 : 雷射測距功能 Golden數值

            S.sprintf("Laser Value Out Shuttle%d %02d%02d", 1, i, j);
            WriteIniData(szDir, "Laser", S, TestIF_File.dLaserValueOutSh[0][i][j]);   //Steven 20140228 : 雷射測距功能 Golden數值

            S.sprintf("Laser Value Out Shuttle%d %02d%02d", 2, i, j);
            WriteIniData(szDir, "Laser", S, TestIF_File.dLaserValueOutSh[1][i][j]);   //Steven 20140228 : 雷射測距功能 Golden數值
        }
    }
}
//---------------------------------------------------------------------------
void TfLaserSensor::ReadFile()
{
    AnsiString S="";
    AnsiString szDir="";

    szDir.sprintf("%s%s\\", DataPath, GetLastOpenFN());
    MyForceDirectories(szDir);
    elLaser->ReadEditTextFromFile(szDir, "HandlerCondition.Data");
    szDir+="HandlerCondition.Data";

    if(USE_LASER_DISTANCE==0)
        return;

    for(int i=0; i<MAX_Index_Row; i++)
    {
        for(int j=0; j<MAX_Index_Col; j++)
        {
            S.sprintf("Laser Value Shuttle1 %02d%02d", i, j);
            TestIF_File.dLaserValue[0][i][j]=CheckAndReadIniData(szDir, "Laser", S, 0.0);   //Steven 20140228 : 雷射測距功能 Golden數值
            S.sprintf("Laser Value Shuttle2 %02d%02d", i, j);
            TestIF_File.dLaserValue[1][i][j]=CheckAndReadIniData(szDir, "Laser", S, 0.0);   //Steven 20140228 : 雷射測距功能 Golden數值

            S.sprintf("Laser Value Out Shuttle1 %02d%02d", i, j);
            TestIF_File.dLaserValueOutSh[0][i][j]=CheckAndReadIniData(szDir, "Laser", S, 0.0);   //Steven 20140228 : 雷射測距功能 Golden數值
            S.sprintf("Laser Value Out Shuttle2 %02d%02d", i, j);
            TestIF_File.dLaserValueOutSh[1][i][j]=CheckAndReadIniData(szDir, "Laser", S, 0.0);   //Steven 20140228 : 雷射測距功能 Golden數值
        }
    }
}
//---------------------------------------------------------------------------
void TfLaserSensor::DoIniDataToForm()
{
    ReadFile();
    elLaser->InitialDataToEdit();

    for(int iX=0; iX<mtPlate1_Golden->FXItem; iX++)
    {
        for(int iY=0; iY<mtPlate1_Golden->FYItem; iY++)
        {
            mtPlate1_Golden->SetCellNumber(iX, iY, TestIF_File.dLaserValueIn[1][iX][iY]);
            mtPlate2_Golden->SetCellNumber(iX, iY, TestIF_File.dLaserValueIn[0][iX][iY]);
        }
    }

    for(int iX=0; iX<mtShuttle1_Golden->FXItem; iX++)
    {
        for(int iY=0; iY<mtShuttle1_Golden->FYItem; iY++)
        {
            if(USE_LASER_DISTANCE==2)
            {
                mtShuttle1_Golden->SetCellNumber(iX, iY, TestIF_File.dLaserValue[0][iY][iX]);
                mtShuttle2_Golden->SetCellNumber(iX, iY, TestIF_File.dLaserValue[1][iY][iX]);

                mtOutShuttle1_Golden->SetCellNumber(iX, iY, TestIF_File.dLaserValueOutSh[0][iY][iX]);
                mtOutShuttle2_Golden->SetCellNumber(iX, iY, TestIF_File.dLaserValueOutSh[1][iY][iX]);
            }
            else
            {
                mtShuttle1_Golden->SetCellNumber(iX, iY, TestIF_File.dLaserValue[0][iX][iY]);
                mtShuttle2_Golden->SetCellNumber(iX, iY, TestIF_File.dLaserValue[1][iX][iY]);
            }
        }
    }
}
//---------------------------------------------------------------------------
void TfLaserSensor::sbtExitClick(vclcompat::TObject * /*Sender*/)
{
    // golden :1291 `Close();` (VCL TForm::Close, dismisses the debug/
    // calibration screen window) dropped: this class has no TForm base and
    // no window to close (matches the tree's own forms/ facade convention --
    // e.g. forms/fMain.h carries no Close() either). DoIniDataToForm() is
    // golden's OnClose handler (FormClose above), not reached by this
    // dropped call in this build; nothing in the laser subsystem depends on
    // it (it is a manual "hide the debug panel" affordance only).
    sbtExit->Down=false;
}
//---------------------------------------------------------------------------
void TfLaserSensor::btSaveLogClick(vclcompat::TObject * /*Sender*/)
{
    AnsiString sFileName;
    sFileName.sprintf("%s\\%04d%02d\\", asLaserLogPath, SystemYear, SystemMonth);
    MyForceDirectories(sFileName);

    sFileName.sprintf("%s\\%04d%02d\\%04d%02d%02d%02d%02d%02d_SHT1.logs", asLaserLogPath, SystemYear, SystemMonth, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
    MemoSh1->Lines->SaveToFile(sFileName);
    MemoSh1->Clear();
    sFileName.sprintf("%s\\%04d%02d\\%04d%02d%02d%02d%02d%02d_SHT2.logs", asLaserLogPath, SystemYear, SystemMonth, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
    MemoSh2->Lines->SaveToFile(sFileName);
    MemoSh2->Clear();

    // golden mtPlate1/2->SaveCellTextToFile(sFileName) (HTray.cpp:461-478) --
    // vclcompat::TrayCore deliberately split the I/O out of the core (its own
    // banner: "a core has no file system concern... the SHELL is responsible
    // for the actual file write"), returning the same "\r\n"-joined text
    // instead. This is that shell: rebuild a TStringList from the joined text
    // and SaveToFile it, matching golden's TStringList::SaveToFile byte-for-
    // byte (same idiom TMemo->Lines->SaveToFile already uses two lines above).
    sFileName.sprintf("%s\\%04d%02d\\%04d%02d%02d%02d%02d%02d_HP1.logs", asLaserLogPath, SystemYear, SystemMonth, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
    { TStringList tl; tl.SetText(mtPlate1->SaveCellTextToString()); tl.SaveToFile(sFileName); }
    sFileName.sprintf("%s\\%04d%02d\\%04d%02d%02d%02d%02d%02d_HP2.logs", asLaserLogPath, SystemYear, SystemMonth, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
    { TStringList tl; tl.SetText(mtPlate2->SaveCellTextToString()); tl.SaveToFile(sFileName); }
}
//---------------------------------------------------------------------------
void TfLaserSensor::WriteToMemo(vclcompat::TMemo *Memo, AnsiString Str)
{
    if(Memo->Lines->Count>4096)
        Memo->Clear();

    Memo->Lines->Add(Str);
}
//---------------------------------------------------------------------------
void TfLaserSensor::TimerInArmTimer(vclcompat::TObject * /*Sender*/)
{
    static bool bRun=false;
    static int iCount=0;

    if(InitialOK==false || USE_LASER_DISTANCE==0)
    {
        bRun=false;
        return;
    }

    if(TestIF_File.bEnableShuttleLaser==false && TestIF_File.bEnableInArmLaser==false &&
      TestIF_File.bEnableInShuttleFloatingCheck==false && TestIF_File.bEnableOutShuttleFloatingCheck==false)
    {
        bRun=false;
        return;
    }

    if(bRun==false)
    {
        bRun=true;

        if(mtPlate2_Golden->FXItem!=HotPlateForm.XDivision)
            mtPlate2_Golden->SetXItem(HotPlateForm.XDivision);
        if(mtPlate2_Golden->FYItem!=HotPlateForm.YDivision)
            mtPlate2_Golden->SetYItem(HotPlateForm.YDivision);
        if(mtPlate1_Golden->FXItem!=HotPlateForm.XDivision)
            mtPlate1_Golden->SetXItem(HotPlateForm.XDivision);
        if(mtPlate1_Golden->FYItem!=HotPlateForm.YDivision)
            mtPlate1_Golden->SetYItem(HotPlateForm.YDivision);

        if(mtPlate2->FXItem!=HotPlateForm.XDivision)
            mtPlate2->SetXItem(HotPlateForm.XDivision);
        if(mtPlate2->FYItem!=HotPlateForm.YDivision)
            mtPlate2->SetYItem(HotPlateForm.YDivision);
        if(mtPlate1->FXItem!=HotPlateForm.XDivision)
            mtPlate1->SetXItem(HotPlateForm.XDivision);
        if(mtPlate1->FYItem!=HotPlateForm.YDivision)
            mtPlate1->SetYItem(HotPlateForm.YDivision);

        if(USE_LASER_DISTANCE==2)
        {
            if(TestIF_File.bEnableInShuttleFloatingCheck && bGetInArm)
            {
                switch(iLaserInArmTask)
                {
                    case 1:
                        iCount=0;
                        iLaserInArmTask=2;
                        break;
                    case 2:
                        GetInArmValue.sprintf("SR,01,519\r\n");
                        WriteInfoToMemo(GetInArmValue);
                        CommLaserInArm->WriteCommData((char*)GetInArmValue.c_str(), GetInArmValue.Length()+1);
                        COMLaserInDelay.SetSecAndOn(3);
                        iLaserInArmTask=3;
    //                    break;
                    case 3:
                        #ifdef SOFT_SIMULTE
                        bRecvDataIn=true;
                        #endif
                        if(bRecvDataIn)
                        {
                            bRecvDataIn=false;
                            iLaserInArmTask=1;
                            bGetInArm=false;
                        }
                        else if(COMLaserInDelay.Off())
                        {
                            iCount++;
                            if(iCount>=5)
                            {
                                iCount=0;
//                                SW[SwLaserPower].On();
                                COMLaserInDelay.SetSecAndOn(1);
                                iLaserInArmTask=4;
                            }
                            else
                            {
                                iLaserInArmTask=2;
                            }
                        }
                        break;
                    case 4:
                        if(COMLaserInDelay.Off())
                        {
//                            SW[SwLaserPower].Off();
                            iLaserInArmTask=5;
                        }
                        break;
                    case 5:
                        btConnectClick(0);
                        COMLaserInDelay.SetSecAndOn(1);
                        iLaserInArmTask=6;
                        break;
                    case 6:
                        if(COMLaserInDelay.Off())
                        {
                            iLaserInArmTask=2;
                        }
                        break;
                }
            }
            else if(TestIF_File.bEnableOutShuttleFloatingCheck && bGetOutArm)
            {
                switch(iLaserOutArmTask)
                {
                    case 1:
                        iCount=0;
                        iLaserOutArmTask=2;
                        break;
                    case 2:
                        GetOutArmValue.sprintf("SR,01,519\r\n");
                        WriteInfoToMemo(GetOutArmValue);
                        CommLaserOutArm->WriteCommData((char*)GetOutArmValue.c_str(), GetOutArmValue.Length()+1);
                        COMLaserOutDelay.SetSecAndOn(3);
                        iLaserOutArmTask=3;
    //                    break;
                    case 3:
                        #ifdef SOFT_SIMULTE
                        bRecvDataOut=true;
                        #endif
                        if(bRecvDataOut)
                        {
                            bRecvDataOut=false;
                            iLaserOutArmTask=1;
                            bGetOutArm=false;
                        }
                        else if(COMLaserOutDelay.Off())
                        {
                            iCount++;
                            if(iCount>=5)
                            {
                                iCount=0;
//                                SW[SwLaserPower].On();
                                COMLaserOutDelay.SetSecAndOn(1);
                                iLaserOutArmTask=4;
                            }
                            else
                            {
                                iLaserOutArmTask=2;
                            }
                        }
                        break;
                    case 4:
                        if(COMLaserOutDelay.Off())
                        {
//                            SW[SwLaserPower].Off();
                            iLaserOutArmTask=5;
                        }
                        break;
                    case 5:
                        btConnectClick(0);
                        COMLaserOutDelay.SetSecAndOn(1);
                        iLaserOutArmTask=6;
                        break;
                    case 6:
                        if(COMLaserOutDelay.Off())
                        {
                            iLaserOutArmTask=2;
                        }
                        break;
                }
            }
        }
        else if(bSetToZeroInArm)
        {
            switch(iLaserInArmTask)
            {
                case 1:
                    bRecvDataIn=false;
                    CommLaserInArm->WriteCommData((char*)SetToZeroMessASCIIInArm.c_str(), SetToZeroMessASCIIInArm.Length()+1);
                    WriteInfoToMemo(SetToZeroMessageInArm);
                    COMLaserInDelay.SetSecAndOn(1);
                    iLaserInArmTask=2;
//                    break;
                case 2:
                    #ifdef SOFT_SIMULTE
                    bRecvDataIn=true;
                    #endif
                    if(bRecvDataIn)
                    {
                        bRecvDataIn=false;
                        iLaserInArmTask=1;
                        bSetToZeroInArm=false;
                    }
                    else if(COMLaserInDelay.Off())
                    {
                        iLaserInArmTask=1;
                    }
                    break;
            }
        }
        else if(bInArm)
        {
            switch(iLaserInArmTask)
            {
                case 1:
                    bRecvDataIn=false;
                    CommLaserInArm->WriteCommData((char*)GetValMessASCIIInArm.c_str(), GetValMessASCIIInArm.Length()+1);
                    WriteInfoToMemo(GetValueMessageInArm);
                    COMLaserInDelay.SetSecAndOn(1);
                    iLaserInArmTask=2;
//                    break;
                case 2:
                    #ifdef SOFT_SIMULTE
                    bRecvDataIn=true;
                    #endif

                    if(bRecvDataIn)
                    {
                        bRecvDataIn=false;
                        iLaserInArmTask=1;
                        bInArm=false;
                    }
                    else if(COMLaserInDelay.Off())
                    {
                        iLaserInArmTask=1;
                    }
                    break;
            }
        }
        else if(fShow && cbStartPooling->Checked && SystemStart==false && cbEnableInArmLaser->Checked==true)
        {
            switch(iLaserInArmTask)
            {
                case 1:
                    bRecvDataIn=false;
                    CommLaserInArm->WriteCommData((char*)GetValMessASCIIInArm.c_str(), GetValMessASCIIInArm.Length()+1);
                    WriteInfoToMemo(GetValueMessageInArm);
                    iLaserInArmTask=2;
                    COMLaserInDelay.SetSecAndOn(1);
//                    break;
                case 2:
                    #ifdef SOFT_SIMULTE
                    bRecvDataIn=true;
                    #endif

                    if(bRecvDataIn)
                    {
                        iLaserInArmTask=1;
                    }
                    else if(COMLaserInDelay.Off())
                    {
                        iLaserInArmTask=1;
                    }
                    break;
            }
        }
        else
        {
            iLaserInArmTask=1;
        }

        bRun=false;
    }
}
//---------------------------------------------------------------------------
void TfLaserSensor::FormDestroy(vclcompat::TObject * /*Sender*/)
{
    Timer1->Enabled=false;
    TimerInArm->Enabled=false;
    LogSoftwareOffTime("TfLaserSensor, FormDestroy"); //Steven 20210526 : 紀錄軟體執行時間
}
//---------------------------------------------------------------------------
void TfLaserSensor::btSetPlate2GoldenToZeroClick(vclcompat::TObject * /*Sender*/)
{
    for(int iX=0; iX<mtPlate2_Golden->FXItem; iX++)
    {
        for(int iY=0; iY<mtPlate1_Golden->FYItem; iY++)
        {
            TestIF_File.dLaserValueIn[0][iX][iY]=0.0;
            mtPlate2_Golden->SetCellNumber(iX, iY, 0.0);
        }
    }
}
//---------------------------------------------------------------------------
void TfLaserSensor::btSetPlate1GoldenToZeroClick(vclcompat::TObject * /*Sender*/)
{
    for(int iX=0; iX<mtPlate1_Golden->FXItem; iX++)
    {
        for(int iY=0; iY<mtPlate1_Golden->FYItem; iY++)
        {
            TestIF_File.dLaserValueIn[1][iX][iY]=0.0;
            mtPlate1_Golden->SetCellNumber(iX, iY, 0.0);
        }
    }
}
//---------------------------------------------------------------------------
void TfLaserSensor::btGetInShuttleGoldenClick(vclcompat::TObject * /*Sender*/)
{
    //==> Eastsun 20260525 laser 整合 : 從 a-side 補實作
    iGetInShuttleGoldenTask=1;
    iGetOutShuttleGoldenTask=0;
    btGetInShuttleGolden->Enabled=false;
    //<== Eastsun 20260525 laser 整合
}
//---------------------------------------------------------------------------
void TfLaserSensor::btGetOutShuttleGoldenClick(vclcompat::TObject * /*Sender*/)
{
    //==> Eastsun 20260525 laser 整合 : 從 a-side 補實作
    iGetOutShuttleGoldenTask=1;
    iGetInShuttleGoldenTask=0;
    btGetOutShuttleGolden->Enabled=false;
    //<== Eastsun 20260525 laser 整合
}
//-------------------------------------------------------------
//==> Eastsun 20260525 laser 整合
void TfLaserSensor::DoGetLaserValue()
{
    int &Task=iGetInShuttleGoldenTask;

    static int iSh;

    switch(Task)
    {
        case 1:
             if(TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==0) //Arm1
                iSh=1;
             else if(TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==1) //Arm2
                iSh=2;
             else
                iSh=3;

             Task=100;
             break;
        case 100:
            if(iSh==3)
            {
                Task=500;
            }
            else if(iSh==2)
            {
                Task=1500;
            }
            else if(iSh==1)
            {
                Task=500;
            }
            break;
        case 500:
            if(MOT[MInShuttle1].IsCanMove())
            {
                Task=600;
            }
            break;
        case 600:
            if(MOT[MInShuttle1].MotorMove(Prod.InSHT[0].iLeft))
            {
                Task=700;
                UseInArmCheckShtFloating(0, true, true);
            }
            break;
        case 700:
            if(UseInArmCheckShtFloating(0, false, true))
            {
                if(iSh==3)
                {
                    Task=1500;
                }
                else
                {
                    btGetInShuttleGolden->Enabled=true;
                    Task=3000;
                }
            }
            break;
        case 1500:
            if(MOT[MInShuttle2].IsCanMove())
            {
                Task=1600;
            }
            break;
        case 1600:
            if(MOT[MInShuttle2].MotorMove(Prod.InSHT[1].iLeft))
            {
                Task=1700;
                UseInArmCheckShtFloating(1, true, true);
            }
            break;
        case 1700:
            if(UseInArmCheckShtFloating(1, false, true))
            {
                btGetInShuttleGolden->Enabled=true;
                Task=3000;
            }
            break;
    }
}
//-------------------------------------------------------------
void TfLaserSensor::DoGetOutLaserValue()
{
    int &Task=iGetOutShuttleGoldenTask;

    static int iSh;

    switch(Task)
    {
        case 1:
             if(TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==0) //Arm1
                iSh=1;
             else if(TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==1) //Arm2
                iSh=2;
             else
                iSh=3;

             Task=100;
             break;
        case 100:
            if(iSh==3)
            {
                Task=500;
            }
            else if(iSh==2)
            {
                Task=1500;
            }
            else if(iSh==1)
            {
                Task=500;
            }
            break;
        case 500:
            if(MOT[MInShuttle1].IsCanMove())
            {
                Task=600;
            }
            break;
        case 600:
            if(MOT[MInShuttle1].MotorMove(Prod.InSHT[0].iRight))
            {
                Task=700;
                UseOutArmCheckShtFloating(0, true, true);
            }
            break;
        case 700:
            if(UseOutArmCheckShtFloating(0, false, true))
            {
                if(iSh==3)
                {
                    Task=1500;
                }
                else
                {
                    btGetOutShuttleGolden->Enabled=true;
                    Task=3000;
                }
            }
            break;
        case 1500:
            if(MOT[MInShuttle2].IsCanMove())
            {
                Task=1600;
            }
            break;
        case 1600:
            if(MOT[MInShuttle2].MotorMove(Prod.InSHT[1].iRight))
            {
                Task=1700;
                UseOutArmCheckShtFloating(1, true, true);
            }
            break;
        case 1700:
            if(UseOutArmCheckShtFloating(1, false, true))
            {
                btGetOutShuttleGolden->Enabled=true;
                Task=3000;
            }
            break;
    }
}
//---------------------------------------------------------------------------
void TfLaserSensor::CommLaserOutArmReceiveData(vclcompat::TObject * /*Sender*/,
      void *Buffer, Spcomm::Word BufferLength)
{
    //==> Eastsun 20260525 laser 整合 : 從 a-side 補實作 (OutArm Laser COM 接收)
    AnsiString ComOmronBuffer="RECV: ";
    AnsiString ComOmronBuffer2="";

    AnsiString S="";
    unsigned char *data;
    bool bHasETX=false;

    data=(unsigned char *)Buffer;

    for(int i=0; i<BufferLength; i++)
    {
        if(USE_LASER_DISTANCE==2)
        {
            asLaserValue[1]+=MyDeCodeASCII(data[i]);
        }

        ComOmronBuffer+=MyDeCodeASCII(data[i]);
        ComOmronBuffer2+=MyDeCodeASCII(data[i]);
    }

    if(bHasETX==true)
    {
        bHasETX=false;
        ProcessReceiveData(2, ComOmronBuffer2);
        ComOmronBuffer2="";
    }

    WriteInfoToMemo("COMLaserOut: "+ComOmronBuffer);
    bRecvDataOut=true;
    //<== Eastsun 20260525 laser 整合
}
//---------------------------------------------------------------------------
void TfLaserSensor::btGetShValueClick(vclcompat::TObject * /*Sender*/)
{
    //==> Eastsun 20260525 laser 整合 : 從 a-side 補實作
    iLaserInArmTask=1;
    bGetInArm=true;
    //<== Eastsun 20260525 laser 整合
}
//---------------------------------------------------------------------------
void TfLaserSensor::btGetOutShValueClick(vclcompat::TObject * /*Sender*/)
{
    //==> Eastsun 20260525 laser 整合 : 從 a-side 補實作
    iLaserOutArmTask=1;
    bGetOutArm=true;
    //<== Eastsun 20260525 laser 整合
}
//---------------------------------------------------------------------------
