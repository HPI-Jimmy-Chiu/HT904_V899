//---------------------------------------------------------------------------
//  SECSGEM/uHGemEquipment.cpp -- THGem, first real slice
//
//  Translation wave: W906-uHGemEquipment
//  Translator: AI(W906-uHGemEquipment) 20260716
//  Golden source: HT9011UC_Code_V3.33.906.0_20260618/SECSGEM/uHGemEquipment.cpp
//  (9353 lines total; this file translates a bounded subset -- see
//  uHGemEquipment.h's file-head note for the full scope statement and the
//  list of what is deliberately NOT here yet).
//
//  This TU deliberately does NOT include golden's own include list
//  (MachineDefine.h/cMydef.h/database.h/myQwertyKeyBoard.h/common.h/
//  uHGemHT9045.h/SgdToXLS.h) -- none of those are needed by this wave's
//  StringGrid-only method family; pulling them in would drag in the entire
//  untranslated Handler state-machine graph for no reason (same
//  extract-calc-core discipline the project has used since Public/HTMD5).
//
//  AI(W906-uHGemEquipment-BucketB) 20260717: the above claim NO LONGER holds
//  in full -- DoUpdateStatus/InitialHGem/SaveSystemDefault (this wave)
//  genuinely need CUSTOMER_CODE/CC_KYEC_LEE/CC_SIGURD_ChungXing/
//  CC_MAXIM_THAILAND/CosFunction/bSECSGEMbyPass/bSECSGEMConnectionFail
//  (cmydef.h) and ReadWriteIni/ReadIniData/WriteIniData (common.h). Verified
//  common.h stays lightweight (its own file-head note: "PARTIAL translation
//  ... ini-helper family only", `#include`s just vclcompat + MachineType.h --
//  NOT the rest of golden's common.h), so this does not reintroduce the
//  "entire untranslated Handler state-machine graph" this note originally
//  warned against; cmydef.h itself is a bigger pull (it includes MachineType.h/
//  myTimer.h/cprod.h/cpublic.h) but is the SAME single include automation.cpp
//  already uses for the identical CUSTOMER_CODE/CC_*/CosFunction need -- see
//  root CMakeLists.txt's ht9045_secsgem target for the matching new
//  ht9045_core/ht9045_globals link deps this requires (same shape already
//  established by the KYECFTP/FTPClient_Transfer.cpp precedent).
//---------------------------------------------------------------------------
#include "SECSGEM/uHGemEquipment.h"

#include <cstdio>
#include <cstdlib>   // atoi
#include <cstring>   // strcpy/strncpy
#include <cstddef>   // NULL

// AI(W906-uHGemEquipment-ConnLifecycle) 20260717: GetSocketErrorMsg (below)
// is a thin forward to the already-real GetErrorMsg (golden
// uHGemEquipment.cpp:6842-6845) -- its one implementation lives in
// Public/WinSocketErrorCode.cpp (ht9045_public library; wired in this wave's
// CMakeLists.txt change, see root CMakeLists.txt's ht9045_secsgem target).
#include "Public/WinSocketErrorCode.h"   // GetErrorMsg(TObject*, int) -> AnsiString

// AI(W906-uHGemEquipment-BucketB) 20260717: see this file's own top-of-file
// note (above) for why these 2 includes are a deliberate, narrow widening of
// this TU's dependency surface (DoUpdateStatus/InitialHGem/SaveSystemDefault
// only).
#include "cmydef.h"    // CUSTOMER_CODE / CC_KYEC_LEE / CC_SIGURD_ChungXing / CC_MAXIM_THAILAND / CosFunction / bSECSGEMbyPass / bSECSGEMConnectionFail / InitialOK (cmydef.h:220)
#include "common.h"    // ReadWriteIni / ReadIniData / WriteIniData

// AI(W906-uHGemEquipment-BucketC) 20260717: D3 -- HSys.MyGem seam. `database.h`
// is lightweight (vclcompat + map/vector + myTimer.h only -- confirmed by
// reading it) and supplies `extern SYSTEM_MODULAR HSys;` (its `MyGem` member
// is `HTGem *`, NULL until a future SystemModularInitial wiring wave -- see
// database.h:235-239). `SECSGEM/uHGemClass.h` supplies the COMPLETE HTGem
// type (only forward-declared by database.h) for the S1F13/S9F7/S9F9 calls
// below -- it does NOT include uHGemEquipment.h back (forward-declares
// `class THGem;` only, see its own file-head note), so no circular include.
// `Config.h` supplies `extern HT9045_CONFIG IniConfig;` (Config.h:1499) --
// same include precedent as aoutarm9045_1x2_2.cpp:67 and others.
#include "database.h"            // HSys / SYSTEM_MODULAR / HSys.MyGem
#include "SECSGEM/uHGemClass.h"  // HTGem (complete type, for HSys.MyGem->S1F13.../S9F7.../S9F9... calls)
#include "Config.h"              // IniConfig.bEnable_SECS_GEM (Timer1Timer)
#include "SECSGEM/SecsEventReport.h"   // EventReport(unsigned Ceid) -- Sim-first free function, see DoUpdateStatus

// ---------------------------------------------------------------------------
//  MyDBIProcess -- process-wide diagnostics/log sink (no-op in this
//  translation; see aHotPlateSubstrate.h:576 / aHotPlateSubstrate.cpp:703 for
//  the one real definition already established elsewhere in this tree).
//  Declared locally (matching the SAME established pattern already used by
//  CCLink/MyCCLink.cpp and KYECFTP/FTPClient_EventHandlers.cpp) rather than
//  `#include "aHotPlateSubstrate.h"` -- that header pulls in a large,
//  unrelated hardware-shim surface (TMySucker/TMyKitSuck/TInLaserCheck/...)
//  this StringGrid-only unit has no other need for.
// ---------------------------------------------------------------------------
extern void MyDBIProcess(AnsiString S1, AnsiString S2);

// ---------------------------------------------------------------------------
//  IncludeTrailingPathDelimiter -- golden calls this exact SysUtils name
//  (uHGemEquipment.cpp:6225, inside ReadAlamData). vclcompat/SysUtils.h only
//  exposes IncludeTrailingBackslash (semantically identical: appends the
//  platform path separator iff not already present) -- SysUtils.h/.cpp are
//  NOT in this front's allowed file list, so this is a thin LOCAL wrapper
//  (file-scope, not exported) rather than an addition to that shared shim.
// ---------------------------------------------------------------------------
static AnsiString IncludeTrailingPathDelimiter(const AnsiString &path)
{
    return IncludeTrailingBackslash(path);
}

//===========================================================================
//  GemTimer  (golden uHGemEquipment.cpp:250-309)
//===========================================================================
//---------------------------------------------------------------------------
// V 1.0
//  設定計時器 (gloss: "arm the timer") -- iTime is in units of 0.1s
//---------------------------------------------------------------------------
void GemTimer::TimerSet(int iTime)
{
    iTimeLen = iTime * 100;
}
//---------------------------------------------------------------------------
void GemTimer::TimerSetSecAndOn(double iTime)
{
    // AI(W906-uHGemEquipment) 20260716: golden is `iTimeLen=iTime*1000;`, an
    // implicit double->int narrowing (iTimeLen is `int`). static_cast makes
    // the existing golden truncation explicit (silences -Wconversion);
    // behavior/value is unchanged.
    iTimeLen = static_cast<int>(iTime * 1000);
    ulStartTicks = ::GetTickCount();   // golden: MyTickCount() -- see header note
}
//---------------------------------------------------------------------------
void GemTimer::TimerSetMSAndOn(int iTime)
{
    iTimeLen = iTime;
    ulStartTicks = ::GetTickCount();   // golden: MyTickCount() -- see header note
}
//---------------------------------------------------------------------------
// V 1.0
//  開始計時器 (gloss: "(re)start the timer")
//---------------------------------------------------------------------------
void GemTimer::TimerOn()
{
    ulStartTicks = ::GetTickCount();   // golden: MyTickCount() -- see header note
}
//---------------------------------------------------------------------------
// V 1.0
//  讀取計時器是否時間到 (gloss: "has the armed interval elapsed?")
//---------------------------------------------------------------------------
bool GemTimer::TimerOff()
{
    if (ulStartTicks == 0)
        return false;

    if (iTimeLen <= 0)
    {   // jou 2012-01-04 gloss: "guards against a mis-entered negative/zero
        // duration hanging the caller forever" (golden uHGemEquipment.cpp:284)
        return true;
    }
    DWORD ulLimited = ulStartTicks + iTimeLen;
    DWORD ulNowTicks = ::GetTickCount();
    DWORD ulNowTicksOver = 0;
    if (ulLimited < ulStartTicks)                                          // DWORD wraparound
    {
        ulLimited = 0xFFFFFFFF - ulStartTicks + iTimeLen;
        ulNowTicksOver = 0xFFFFFFFF - ulStartTicks + ulNowTicks;

        if (ulNowTicksOver < ulStartTicks &&                                // if the "now" tick also wrapped
            ulNowTicksOver > ulLimited)                                     // and has passed the limit
        {
            return true;
        }
    }
    else
    {
        if (ulNowTicks >= (ulStartTicks + iTimeLen))
        {
            return true;
        }
    }
    return false;
}

//===========================================================================
//  THGem -- construction / destruction
//===========================================================================
// Grid sizes match golden's .dfm design-time RowCount/ColCount exactly (see
// uHGemEquipment.h's member-declaration comments for the per-grid citation).
// TStringGrid's ctor signature is (initialColCount, initialRowCount).
// AI(W906-fire-verify) 20260716: fire-wave fidelity review flagged (low) that
// allocating all 4 grids directly in the member-initializer list had no
// exception safety net -- if e.g. the 3rd `new` threw (std::bad_alloc), the
// first 2 already-constructed grids would leak (the partially-constructed
// THGem's destructor never runs on a constructor-body exception). golden's
// THGem is DFM/VCL-instantiated so this ctor/dtor pair is new code introduced
// by this wave, not a fidelity concern; fixed here by NULL-initializing the
// members and allocating in the body with an explicit try/catch that cleans
// up whichever grids DID succeed before rethrowing (delete on a NULL pointer
// is a safe no-op, so this is correct regardless of which `new` failed).
// AI(W906-uHGemEquipment-ConnLifecycle) 20260717: ctor EXTENDED for this
// wave's new members. golden's own ctor (uHGemEquipment.cpp:444-674) never
// explicitly `new`s clientGem/srvGem -- real BCB6 __published components are
// created by VCL's Owner/.dfm streaming mechanism BEFORE the ctor body even
// runs. This translation has no such streaming step, so clientGem/srvGem are
// allocated + wired here explicitly, matching the SAME established idiom
// Automation/automation.cpp's own translated ctor already uses for its
// TServerSocket/TClientSocket pair (OLPServer/OLPClient) -- design-time
// Address/Port values copied from golden's own .dfm (uHGemEquipment.dfm:
// 542-554 clientGem, :562-573 srvGem). Every bool/int explicitly initialized
// below matches golden's own ctor-body assignment for that same field
// (uHGemEquipment.cpp:461-530); iEstablishCommunicationsTryCount/iTimeFormat
// are the two exceptions -- golden's own ctor never assigns them at all (set
// later by SetEstablishCommunicationsTryCount/SetTimeFormat, both callable
// but SetTimeFormat is out of this wave's scope) -- zero-initialized here
// defensively (same "flagged deviation from golden's raw uninitialized
// state" precedent already established by GemTimer's own ctor, see this
// file's GemTimer note).
THGem::THGem()
    : strGrdAlarmOld(NULL),
      strGrdCEID(NULL),
      stdGridReportID(NULL),
      strGrdAlarm(NULL),
      GemSystemPath(""),
      clientGem(NULL),
      srvGem(NULL),
      bConnect(false),               // golden ctor :523
      bOnLine(false),                // golden ctor :530
      bOnLineLocal(false),           // golden ctor :527
      bAutoConnect(false),           // golden ctor :606
      bStartConnect(false),          // golden ctor :525
      bStartOnLine(false),           // golden ctor :529
      bTCPIP_Error(false),           // golden ctor :608
      bServoSocketConnect(false),    // golden ctor :461
      bReceiveMultiConnect(false),   // golden ctor :654
      bUseClientSocket(false),       // golden ctor :468
      bOpenCommuncation(false),      // golden ctor :609
      bCloseCommuncation(false),     // golden ctor :610
      bS1F2_OnLineData(false),       // golden ctor :539
      countConnect(0),               // golden ctor :478
      iConnectTryCount(0),           // golden ctor :479
      iEstablishCommunicationsTryCount(0),  // golden never inits this (see note above)
      iOpenCommuncationTask(1),      // golden ctor :477
      iStartConnectTask(1),          // golden ctor :524
      iStartOnLineTask(1),           // golden ctor :528
      iTimeFormat(0),                // golden never inits this (see note above)
      WaitShowString(NULL),
      LogDataString(NULL),
      // ---- Bucket C (W906-uHGemEquipment-BucketC 20260717) ------------------
      // WireCodec/SType are NOT listed here: WireCodec has its own default
      // ctor (runs automatically in declaration order); SType's fields are
      // populated for real by InitSTypeStruct() in the ctor body below
      // (golden ctor :603), matching golden exactly (SType is never touched
      // any other way in golden's own ctor either).
      StringOutColor(clBlack),                          // golden ctor :502
      bWaitSelectRsp(false),                             // golden ctor :532
      bWaitDeSelectRsp(false),                           // golden ctor :533
      bWaitEstablishCommunicationsResponse(false),       // golden ctor :535
      bWaitEstablishCommunicationsResponseError(false),  // golden ctor :536
      bReceiveEstablishCommunicationsRequest(false),     // golden ctor :614
      bSeprate(false),                                   // golden ctor :541
      bDataFormatOK(false),               // golden never inits this (see header note)
      bFirstEntry(true),                                 // golden ctor :450
      bFirstBlock(true),                                 // golden ctor :472
      // AI(W906-SysModWire) 20260720: ProcessReceiceData tail members --
      // golden ctor :503 (MoveCheckCallBack=NULL) and :462-466 (bReceiveS7F6..
      // bReceiveS101F8=false). bReceiveS110F2 is the one flagged deviation
      // (golden's own ctor NEVER initializes it -- see header member comment).
      MoveCheckCallBack(NULL),                            // golden ctor :503
      bReceiveS7F6(false),                                // golden ctor :462
      bReceiveS101F5(false),                              // golden ctor :463
      bReceiveS101F6(false),                              // golden ctor :464
      bReceiveS101F7(false),                              // golden ctor :465
      bReceiveS101F8(false),                              // golden ctor :466
      bReceiveS110F2(false),               // golden never inits this -- flagged deviation, see header note
      iFileCount(0),                                     // golden ctor :672
      Timer1Task(1),                                      // golden ctor :494
      Timer1ct(0),                                        // golden ctor :495
      iOldSecProcessSFNoResponse(0),       // golden never inits this (see header note)
      iAutoConnectSec(0),                                // golden ctor :485
      iAutoConnectDelay(2),                              // golden ctor :486
      RemoteSystemByte(0),                 // golden never inits this (see header note)
      Alias(""),                                         // golden default AnsiString ""
      Caption(""),                                       // stand-in default ""
      EthernetBuffer(NULL),               // allocated in ctor body below (golden ctor :474-475)
      iEthernetBufferLen(0),               // set to 10240 in ctor body below
      SFCodeResponseList(NULL),
      TimeLeft(NULL),
      pLockOnSocketRecvice(NULL),
      csSFCodeResponse(NULL),
      RecvMemoryBuffer(NULL),
      ProcBuffer(NULL),
      TempProcBuffer(NULL),
      // ---- Bucket B widget stand-ins (W906-uHGemEquipment-BucketB) --------
      // __published widgets (allocated in the ctor body below, matching
      // clientGem/srvGem's own established idiom -- NULL here purely as the
      // pre-allocation placeholder):
      EnableOrDisablePtr(NULL),        // golden ctor :616 (externally assigned, NOT allocated by THGem)
      OnLineOrOffLine(NULL),
      RemoteOrLocal(NULL),
      rgRole(NULL),
      edtIP(NULL),
      edtPort(NULL),
      edDeviceID(NULL),
      edtT3TimeOut(NULL),
      edtT5TimeOut(NULL),
      edtT6TimeOut(NULL),
      edtT7TimeOut(NULL),
      edtT8TimeOut(NULL),
      GemCheckBoxAcceptHostOnlineRequest(NULL),
      GemCheckBoxShowBinary(NULL),
      GemCheckBoxShowHeadInformation(NULL),
      GemCheckBoxUseExtendedAlarm(NULL),
      chkAnnotatedEventReport(NULL),
      chkMoreMessageAbortProcess(NULL),
      ckAddDefaultReport(NULL),
      cbECChaneEventReport(NULL),
      ComboBox1(NULL),
      // externally-assigned pointers (golden ctor :611-613/616, and golden's
      // own real-VCL-risk category for the 5 TSpeedButton*s/DB/TerminalMemoPtr
      // -- see each THGemXxx struct's own header comment): stay NULL, never
      // allocated by THGem itself.
      SECSConnectionState(NULL),       // golden ctor :611
      GEMCommunicatingState(NULL),     // golden ctor :612
      GemPanelControlState(NULL),      // golden ctor :613
      BtnEnableComm(NULL),
      GemBtnOfflineRequest(NULL),
      GemBtnOnlineRequest(NULL),
      GemBtnOnlineRemote(NULL),
      GemBtnOnlineLocal(NULL),
      DB(NULL),                        // golden ctor :537
      TerminalMemoPtr(NULL),
      DefaultAddress(""),              // golden ctor :451
      DefaultPort(""),                 // golden ctor :452
      DefaultDeviceID(""),             // golden ctor :453
      T3TimeOut(30),                   // golden ctor :454
      T5TimeOut(30),                   // golden ctor :455
      T6TimeOut(30),                   // golden ctor :456
      T7TimeOut(30),                   // golden ctor :457
      T8TimeOut(30),                   // golden ctor :458
      GemSystemIniPath(""),
      bShow(false),                    // golden ctor :617
      // DoUpdateStatus's own supporting state -- see this file's header note
      // on why all 6 are defensively zero/explicitly-initialized here even
      // though golden's own ctor never touches most of them (real BCB6/VCL
      // zero-inits every field for free; C++ does not).
      ctUpdateStatus(0),
      bClientSocketActive(false),
      iServoConnectCT(-1),             // golden ctor :492 (explicitly -1)
      bOldConnect(false),
      flag1UpdateStatus(true),         // golden ctor :489 (explicitly true)
      flag2UpdateStatus(true),         // golden ctor :490 (explicitly true)
      flag3UpdateStatus(true),         // golden ctor :491 (explicitly true)
      bConnectUpdateStatus(true),      // golden ctor :493 (explicitly true)
      OldSUpdateStatus(""),
      GemControlState(0),
      GemControlPreState(0),
      SECSCommunicationMode(0),        // golden ctor :500 (explicitly 0)
      // AI(W906-SvEcDataItem) 20260720: FormCreate's "system SV" targets --
      // golden's own ctor never touches any of these (see header's own note
      // on this member group); zero/empty-initialized here defensively,
      // matching this file's established precedent.
      GemLinkState(0),                 // golden ctor :499 (explicitly 0)
      lCPUFreq(0),
      Disk_C_TotalSpaceMB(0),
      Disk_D_TotalSpaceMB(0),
      Disk_C_TotalFreeSpaceMB(0),
      Disk_D_TotalFreeSpaceMB(0),
      ulMemoryLoad(0),
      ulTotalPhys(0),
      ulAvailPhys(0),
      iMinTimeFormat(0),               // golden ctor :650 (explicitly 0)
      iMaxTimeFormat(3),               // golden ctor :651 (explicitly 3)
      iTimeFormatDefault(0),           // golden never inits this either -- flagged deviation, see header note
      SV_70_UNT1_ReceipeStruct(0),
      SV_71_ASCII_FilenameExtened(""),
      GemSpoolCountActual(0)
{
    // AI(W906-SvEcDataItem) 20260720: szManID/szGetCPUType/GemSpoolStartTime
    // are fixed char[256] buffers (not in the member-init list above --
    // arrays cannot be initialized with a scalar in a ctor init list); golden
    // never initializes them either (see header note) -- zeroed explicitly
    // here, same defensive posture as every other member in this group.
    szManID[0] = 0;
    szGetCPUType[0] = 0;
    GemSpoolStartTime[0] = 0;

    try
    {
        strGrdAlarmOld  = new TStringGrid(5, 1);
        strGrdCEID      = new TStringGrid(258, 1025);
        stdGridReportID = new TStringGrid(1026, 257);
        strGrdAlarm     = new TStringGrid(12, 5);

        // AI(W906-uHGemEquipment-BucketC) 20260717: D4 -- ALIASED into
        // WireCodec's own WaitShowString/LogDataString (WireCodec, a
        // by-value member, is already fully constructed by this point --
        // member construction always precedes the ctor body -- so its own
        // ctor has already allocated these two TStringLists). THGem no
        // longer allocates its own separate pair (golden had exactly ONE
        // WaitShowString/LogDataString; before this wave the port had two,
        // so codec-side StringOut() calls -- SendLocalDataFrom, ShowSML,
        // ... -- never reached ProcessShow/SaveSECSGEMTextToLog). See
        // uHGemEquipment.h's own member-comment for the matching dtor-side
        // ownership note (THGem does NOT delete these two -- WireCodec's own
        // dtor, which runs AFTER ~THGem's body, does).
        WaitShowString = WireCodec.WaitShowString;
        LogDataString  = WireCodec.LogDataString;

        // AI(W906-uHGemEquipment-BucketC) 20260717: D2 -- install the real
        // send hook on THGem's own WireCodec (see SecsWireCodec.h's own
        // SendLocalDataHook comment + SendLocalDataFrom's own comment below
        // for the full design). Recursion-safe: SendLocalDataFrom never
        // calls wc.SendLocalData() -- it IS the sender the hook forwards to.
        WireCodec.SendLocalDataHook = [this](SecsWireCodec &wc) { SendLocalDataFrom(wc); };

        InitSTypeStruct();   // golden ctor :603
        // AI(W906-uHGemEquipment-BucketC) 20260717: golden ctor :602 also
        // calls `InitHType();` here -- NOT reproduced: HType (the shared
        // SECS-II format-byte table) is already seeded process-wide by a
        // file-static initializer object in SecsWireCodec.cpp (:57-88,
        // verified) that runs during static initialization, BEFORE main()
        // even starts -- independent of any THGem/SecsWireCodec instance
        // being constructed. A second InitHType() call would be redundant,
        // not incorrect (same literal values either way).

        // ---- clientGem (active/client role) -- golden .dfm:542-554 --------
        clientGem = new TClientSocket(NULL);
        clientGem->Address = "192.168.1.3";   // golden .dfm:544
        clientGem->Port = 5100;               // golden .dfm:546
        // OnConnecting = clientGemConnecting (golden .dfm:547): vclcompat's
        // TClientSocket has no OnConnecting event slot (documented API gap --
        // see Automation/automation.cpp's own identical ADAPTATION note for
        // its OLPClient/OLPClientConnecting). clientGemConnecting is still
        // translated below as a real, directly-callable method; it is simply
        // not auto-fired by this shim.
        clientGem->OnConnect = [this](TObject *Sender, TCustomWinSocket *Socket)
            { clientGemConnect(Sender, Socket); };
        clientGem->OnDisconnect = [this](TObject *Sender, TCustomWinSocket *Socket)
            { clientGemDisconnect(Sender, Socket); };
        clientGem->OnError = [this](TObject *Sender, TCustomWinSocket *Socket,
                                     TErrorEvent ErrorEvent, int &ErrorCode)
            { clientGemError(Sender, Socket, ErrorEvent, ErrorCode); };
        // AI(W906-uHGemEquipment-BucketC) 20260717: OnRead = clientGemRead
        // (golden .dfm:550) -- NOW WIRED. clientGemRead was out of scope for
        // the ConnLifecycle wave (needed TMemoryStream/TFixedCriticalSection
        // shims, not yet designed then); both exist now (§7/§8). Lambda
        // forwarding matches this ctor's own established event-wiring style.
        clientGem->OnRead = [this](TObject *Sender, TCustomWinSocket *Socket)
            { clientGemRead(Sender, Socket); };

        // ---- srvGem (passive/server role) -- golden .dfm:562-573 ----------
        srvGem = new TServerSocket(NULL);
        srvGem->Port = 6000;   // golden .dfm:564
        srvGem->OnClientConnect = [this](TObject *Sender, TCustomWinSocket *Socket)
            { srvGemClientConnect(Sender, Socket); };
        srvGem->OnClientDisconnect = [this](TObject *Sender, TCustomWinSocket *Socket)
            { srvGemClientDisconnect(Sender, Socket); };
        srvGem->OnClientError = [this](TObject *Sender, TCustomWinSocket *Socket,
                                        TErrorEvent ErrorEvent, int &ErrorCode)
            { srvGemClientError(Sender, Socket, ErrorEvent, ErrorCode); };
        // AI(W906-uHGemEquipment-BucketC) 20260717: OnClientRead =
        // clientGemRead (golden .dfm:569) -- NOW WIRED, same shared-handler
        // pattern the socket shims were explicitly built for (ServerSocket.h
        // :47-73's own "shared handler wiring" note; clientGemRead's real
        // signature matches BOTH TSocketNotifyEvent slots).
        srvGem->OnClientRead = [this](TObject *Sender, TCustomWinSocket *Socket)
            { clientGemRead(Sender, Socket); };

        // ---- Bucket B __published widget stand-ins (W906-uHGemEquipment-
        // BucketB) -- allocated here for the SAME reason clientGem/srvGem are
        // above: real BCB6 __published components are VCL/.dfm-streamed
        // BEFORE the ctor body runs, so a normally-constructed THGem never
        // sees these as NULL; this translation has no streaming step, so they
        // are `new`'d explicitly. No golden .dfm design-time property values
        // are replicated here (unlike clientGem/srvGem's Address/Port) --
        // InitialHGem (below) is the sole real initializer for every one of
        // these widgets' actual content in this wave's scope, so a ctor-time
        // .dfm-literal value would be immediately overwritten and is
        // therefore inert; each stand-in's own default-constructed value is
        // used instead.
        EnableOrDisablePtr = NULL;   // externally assigned, see its own member comment -- deliberately NOT allocated here
        OnLineOrOffLine    = new THGemRadioGroup();
        RemoteOrLocal      = new THGemRadioGroup();
        rgRole             = new THGemRadioGroup();
        // golden ctor :653 `rgRole->OnClick=ManualCreatergRoleClick;` -- NOT
        // reproduced: THGemRadioGroup has no OnClick callback slot (this
        // minimal stand-in models data only, not VCL event dispatch) --
        // ManualCreatergRoleClick (below) is still a real, directly-callable
        // method; it is simply not auto-fired when ItemIndex changes. Same
        // documented, acceptable gap as clientGem's own OnConnecting note
        // above (this wave's own brief explicitly anticipated and accepted
        // this exact gap for rgRole->OnClick).

        edtIP           = new THGemEdit();
        edtPort         = new THGemEdit();
        edDeviceID      = new THGemEdit();
        edtT3TimeOut    = new THGemEdit();
        edtT5TimeOut    = new THGemEdit();
        edtT6TimeOut    = new THGemEdit();
        edtT7TimeOut    = new THGemEdit();
        edtT8TimeOut    = new THGemEdit();

        GemCheckBoxAcceptHostOnlineRequest = new THGemCheckBox();
        GemCheckBoxShowBinary              = new THGemCheckBox();
        GemCheckBoxShowHeadInformation     = new THGemCheckBox();
        GemCheckBoxUseExtendedAlarm        = new THGemCheckBox();
        chkAnnotatedEventReport            = new THGemCheckBox();
        chkMoreMessageAbortProcess         = new THGemCheckBox();
        ckAddDefaultReport                 = new THGemCheckBox();
        cbECChaneEventReport               = new THGemCheckBox();

        ComboBox1 = new THGemComboBox();

        // ---- Bucket C (W906-uHGemEquipment-BucketC 20260717) --------------
        iEthernetBufferLen = 10240;
        EthernetBuffer = new unsigned char[iEthernetBufferLen];   // golden ctor :474-475

        SFCodeResponseList  = new THGemListBox();       // golden .h:138 (__published)
        TimeLeft            = new TStringList();        // golden ctor :584
        pLockOnSocketRecvice = new TFixedCriticalSection();   // golden ctor :668 (16.10.05.00 Roy Add)
        csSFCodeResponse     = new TCriticalSection();        // golden ctor :673 (20221111 Joseph (Jason))
        RecvMemoryBuffer     = new TMemoryStream();      // golden ctor :669
        ProcBuffer           = new TMemoryStream();      // golden ctor :670
        TempProcBuffer       = new TMemoryStream();      // golden ctor :671
    }
    catch (...)
    {
        // Same delete ORDER as ~THGem() below (clientGem/srvGem first) for
        // consistency/defense-in-depth -- see that destructor's own comment
        // for why the order is load-bearing there.
        delete clientGem;
        delete srvGem;
        delete OnLineOrOffLine;
        delete RemoteOrLocal;
        delete rgRole;
        delete edtIP;
        delete edtPort;
        delete edDeviceID;
        delete edtT3TimeOut;
        delete edtT5TimeOut;
        delete edtT6TimeOut;
        delete edtT7TimeOut;
        delete edtT8TimeOut;
        delete GemCheckBoxAcceptHostOnlineRequest;
        delete GemCheckBoxShowBinary;
        delete GemCheckBoxShowHeadInformation;
        delete GemCheckBoxUseExtendedAlarm;
        delete chkAnnotatedEventReport;
        delete chkMoreMessageAbortProcess;
        delete ckAddDefaultReport;
        delete cbECChaneEventReport;
        delete ComboBox1;
        delete strGrdAlarmOld;
        delete strGrdCEID;
        delete stdGridReportID;
        delete strGrdAlarm;
        // AI(W906-uHGemEquipment-BucketC) 20260717: WaitShowString/
        // LogDataString are NO LONGER deleted here (D4 -- they are ALIASED
        // to WireCodec's own instances, not separately owned; see the
        // aliasing comment in the try block above). WireCodec itself is a
        // fully-constructed member subobject by the time any exception here
        // could fire, so its own destructor (invoked automatically during
        // stack unwinding of this constructor) deletes them for real --
        // deleting them here too would be a double-free.
        delete SFCodeResponseList;
        delete TimeLeft;
        delete pLockOnSocketRecvice;
        delete csSFCodeResponse;
        delete RecvMemoryBuffer;
        delete ProcBuffer;
        delete TempProcBuffer;
        delete[] EthernetBuffer;
        throw;
    }
}

THGem::~THGem()
{
    // AI(W906-uHGemEquipment-ConnLifecycle) 20260717: ORDER IS LOAD-BEARING --
    // clientGem/srvGem MUST be deleted FIRST, before WaitShowString/
    // LogDataString. Root-caused via this wave's own test [11]
    // (test_doopencommuncation_open_guard): ~TServerSocket() (via DoClose_())
    // synchronously fires OnClientDisconnect for any still-open connection --
    // wired in this ctor to srvGemClientDisconnect, which calls GetTimeInfo()/
    // StringOut(...), which dereferences WaitShowString/LogDataString. With
    // the original delete order (StringLists first, sockets last), that was a
    // use-after-free -- manifested as a "pure virtual method called" abort
    // when a THGem with an active Sim-accepted srvGem connection went out of
    // scope. Deleting clientGem/srvGem BEFORE the StringLists ensures any
    // synchronous teardown-time callback into THGem's own methods still sees
    // live WaitShowString/LogDataString/other members.
    //
    // AI(W906-uHGemEquipment-BucketC) 20260717: this ordering concern now has
    // a SECOND, independent safety net -- WaitShowString/LogDataString are
    // ALIASED to WireCodec's own TStringLists (D4) and are no longer
    // `delete`d anywhere in THIS destructor's body at all; WireCodec (a
    // by-value member) is only destroyed automatically AFTER this entire
    // destructor body finishes running (standard C++ member-destruction
    // timing), so WaitShowString/LogDataString stay valid for the WHOLE body
    // regardless of the delete order below. The order is kept as-is anyway
    // (harmless, and still correct for the ORIGINAL reason cited above).
    delete clientGem;
    delete srvGem;
    // AI(W906-uHGemEquipment-BucketB) 20260717: the 20 __published widget
    // stand-ins THGem itself allocated (see the ctor) are deleted here too --
    // same ownership rule as clientGem/srvGem above. The remaining widget
    // pointers (EnableOrDisablePtr/SECSConnectionState/GEMCommunicatingState/
    // GemPanelControlState/the 5 TSpeedButton*s/DB/TerminalMemoPtr) are
    // EXTERNALLY assigned (never allocated by THGem, see each member's own
    // header comment) -- deliberately NOT deleted here, matching golden
    // semantics (THGem never owned them in the first place).
    delete OnLineOrOffLine;
    delete RemoteOrLocal;
    delete rgRole;
    delete edtIP;
    delete edtPort;
    delete edDeviceID;
    delete edtT3TimeOut;
    delete edtT5TimeOut;
    delete edtT6TimeOut;
    delete edtT7TimeOut;
    delete edtT8TimeOut;
    delete GemCheckBoxAcceptHostOnlineRequest;
    delete GemCheckBoxShowBinary;
    delete GemCheckBoxShowHeadInformation;
    delete GemCheckBoxUseExtendedAlarm;
    delete chkAnnotatedEventReport;
    delete chkMoreMessageAbortProcess;
    delete ckAddDefaultReport;
    delete cbECChaneEventReport;
    delete ComboBox1;
    delete strGrdAlarmOld;
    delete strGrdCEID;
    delete stdGridReportID;
    delete strGrdAlarm;
    // AI(W906-uHGemEquipment-BucketC) 20260717: WaitShowString/LogDataString
    // are NO LONGER deleted here (D4 -- ALIASED to WireCodec's own instances,
    // see the ctor's own aliasing comment). WireCodec's own destructor (runs
    // AFTER this destructor's body, per C++ member-destruction order --
    // members are destroyed in REVERSE declaration order, and WireCodec is
    // declared before every pointer member here) deletes them for real.
    //
    // The 7 Bucket-C-owned pointers below ARE deleted here -- a DEVIATION
    // from golden, flagged once: golden's own ~THGem() (uHGemEquipment.cpp:
    // 676-688) deletes ONLY pLockOnSocketRecvice (+MulitBuffer, an unrelated
    // member outside this wave's scope) and LEAKS SFCodeResponseList/
    // TimeLeft/csSFCodeResponse/RecvMemoryBuffer/ProcBuffer/TempProcBuffer/
    // EthernetBuffer outright (real BCB6 processes exit and the OS reclaims
    // the memory regardless -- a THGem is a top-level VCL form, never
    // destroyed except at process exit, in golden's real deployment). This
    // port's THGem CAN be destroyed mid-process (test fixtures construct and
    // destroy many), so this wave follows the port's own established
    // "delete what you own" dtor convention instead of reproducing golden's
    // leak -- matching the exact same already-accepted deviation class as
    // this destructor's own pre-existing Bucket-B widget cleanup above.
    delete SFCodeResponseList;
    delete TimeLeft;
    delete pLockOnSocketRecvice;    // golden DOES delete this one (:681, 16.10.05.00 Roy Add)
    delete csSFCodeResponse;
    delete RecvMemoryBuffer;
    delete ProcBuffer;
    delete TempProcBuffer;
    delete[] EthernetBuffer;
}

//===========================================================================
//  Alarm-grid siblings (golden uHGemEquipment.cpp:6211-6274)
//===========================================================================
void THGem::SetAlamData(int iRowCount, AnsiString ALID, AnsiString Class, AnsiString ALTX, AnsiString Position)
{
    // golden writes ALTX (Alarm Text) into no cell here -- this is a genuine
    // golden quirk, preserved verbatim: the ALTX parameter is accepted but
    // never used inside SetAlamData's own body (uHGemEquipment.cpp:6211-6217
    // only ever touches cols 7/8/9/10; col 6, "Alarm Text", is populated by
    // other out-of-scope code elsewhere, not by this method). Do NOT "fix"
    // this by writing ALTX somewhere golden doesn't.
    (void)ALTX;
    strGrdAlarm->Cells[7][iRowCount] = 1;
    strGrdAlarm->Cells[8][iRowCount] = ALID;
    strGrdAlarm->Cells[9][iRowCount] = Class;
    strGrdAlarm->Cells[10][iRowCount] = Position;
}
//------------------------------------------------------------------------------
void THGem::ReadAlamData()
{
    // AI(W906-uHGemEquipment) 20260716: GOLDEN QUIRK, preserved verbatim --
    // ReadAlamData loads "AlarmData.def" into `strGrdAlarmOld`, NOT the live
    // `strGrdAlarm` grid that ReportAlarm (out of scope) reads from and that
    // WriteAlamData (below) saves FROM. strGrdAlarmOld appears to serve as a
    // "previous configuration" snapshot for some out-of-scope comparison
    // logic elsewhere in the real THGem -- NOT a bug to "fix" by redirecting
    // this to strGrdAlarm (golden uHGemEquipment.cpp:6219-6260).
    TStringList *memoPtr;
    AnsiString Filename;

    memoPtr = new TStringList;
    Filename = IncludeTrailingPathDelimiter(GemSystemPath) + AnsiString("AlarmData.def");

    if (FileExists(Filename) == false)
    {
        delete memoPtr;
        return;
    }

    memoPtr->LoadFromFile(Filename);

    // Ifor 20170523 (wei) gloss: "guards against a genuinely empty
    // AlarmData.def crashing SECS/GEM startup" -- trims trailing blank lines.
    try
    {
        while (1)
        {
            if (memoPtr->Count == 0)
                break;

            if (memoPtr->Strings[memoPtr->Count - 1] == "")
                memoPtr->Delete(memoPtr->Count - 1);
            else
                break;
        }
        strGrdAlarmOld->RowCount = memoPtr->Count;
        PasteStringGridAsTabFormat(strGrdAlarmOld, memoPtr);
    }
    catch (...)
    {
        MyDBIProcess("Exception", "THGem::ReadAlamData");
        memoPtr->Clear();
        delete memoPtr;
        // AI(W906-uHGemEquipment) 20260716: golden falls through to the
        // (identical) RowCount=/PasteStringGridAsTabFormat pair below EVEN
        // on the exception path, using `memoPtr` AFTER it was just deleted
        // above (uHGemEquipment.cpp:6253-6257) -- a genuine golden
        // use-after-free bug. Preserved: see the duplicated block below,
        // which is reached unconditionally (both normal and catch paths),
        // exactly matching golden's control flow. Do NOT dereference the
        // already-deleted memoPtr a second time here; the duplicate below is
        // executed after `return`-free fallthrough, same as golden.
    }
    // AI(W906-uHGemEquipment) 20260716: GOLDEN BUG, preserved verbatim
    // (uHGemEquipment.cpp:6256-6257): this exact RowCount=/Paste pair is
    // ALSO the last two statements inside the `try` block above, so on the
    // non-exception path it runs twice (harmless: idempotent). On the
    // exception path, golden's `memoPtr` was already `delete`d in the catch
    // block just above -- this is a real use-after-free in golden. We
    // preserve golden's CONTROL FLOW (the statements are textually
    // duplicated, unconditionally reached) but do not attempt to fix or
    // paper over the dangling-pointer read; a build with a hardening
    // allocator (e.g. under ASan) would legitimately trip here on the
    // exception path, exactly as it would against the original BCB6 binary.
    strGrdAlarmOld->RowCount = memoPtr->Count;
    PasteStringGridAsTabFormat(strGrdAlarmOld, memoPtr);
    memoPtr->Clear();
    delete memoPtr;
}
//------------------------------------------------------------------------------
void THGem::WriteAlamData()
{
    TStringList *memoPtr;
    AnsiString Filename;

    memoPtr = new TStringList;
    CopyStringGridAsTabFormat(strGrdAlarm, memoPtr);

    Filename = IncludeTrailingPathDelimiter(GemSystemPath) + AnsiString("AlarmData.def");
    memoPtr->SaveToFile(Filename);
    memoPtr->Clear();
    delete memoPtr;
}

//===========================================================================
//  SV/EC DataItem family (W906-SvEcDataItem 20260720)
//  (golden uHGemEquipment.cpp:2472-3336, 7623-7688)
//
//  PORT NOTE (applies to every method below that walks SvEcReg's SV_*/EC_*
//  lists): golden's THGem holds SV_ID/SV_TYPE/.../EC_OldValue as ITS OWN
//  direct members; this port holds the identical bookkeeping one level down,
//  inside the by-value `SvEcReg` member (SecsSvEcRegistration, see the
//  header's own embed note) -- so every golden `SV_ID->...`/`EC_ID->...`
//  etc. becomes `SvEcReg.SV_ID->...`/`SvEcReg.EC_ID->...` here (purely
//  mechanical -- SvEcReg IS THGem's own SV/EC bookkeeping, not a different
//  object). `->Strings[i].c_str()` becomes `->GetString(i).c_str()`
//  (vclcompat's StringsProxy has no `.c_str()`, see SecsSvEcRegistration.cpp
//  's own file-head note -- same accommodation already established there and
//  in CheckECValue, uHGemClass.cpp). `DataItemOut`/`SendInvalidDataMessage
//  ToHost` are wire-codec primitives -> `WireCodec.` (THGem's own embedded
//  codec, D1 precedent, matching CheckSFFormatOnlyHead's own established
//  idiom above). golden `_atoi64` -> `strtoll`/`strtoull` (MinGW <cstdlib>
//  has no `_atoi64`; same portability substitution SecsSvEcRegistration.cpp
//  already made, identical rationale).
//
//  GATE NOTE (design doc D2): golden's VCL-widget-cast sub-branch (IsVCL==1
//  -> dynamic_cast onto TPanel/TCustomEdit/TComboBox/TLabel/TCheckBox/
//  TRadioGroup/TStringList, none of which exist in vclcompat) is GATED
//  (`#if 0`) in DataItemOutSV/DataItemOutEC below, following the EXACT same
//  gate+conservative-fallback idiom SecsSvEcRegistration.cpp's own
//  GetECDataValue already established for the identical golden branch
//  (SecsSvEcRegistration.cpp:620-701) -- see that method for the precedent.
//  Every SV/EC THGem::FormCreate registers this wave is raw-ptr (VCL_NAME/
//  EC_VCL_NAME=="0"), so this gate is never reached by any in-scope caller;
//  the IsVCL==2 (AnsiString*-backed) sibling branch stays REAL.
//===========================================================================
//---------------------------------------------------------------------------
// V 1.0 (golden uHGemEquipment.cpp:2472-2761)
// SV data output
//---------------------------------------------------------------------------
bool THGem::DataItemOutSV(AnsiString SVID)
{
    int i, Len;
    unsigned char Type;
    AnsiString SVName;
    AnsiString SVUnit;
    void *P;
    AnsiString S, IsVCL, VCLStr, *SS;

    char  VCL_ASCII[10000];
    char  VCL_BINARY;
    char  VCL_INT1;
    short VCL_INT2;
    int   VCL_INT4;
    long long  VCL_INT8;                        // golden __int64
    unsigned char  VCL_UINT1,VCL_BOOLEAN;
    unsigned short VCL_UINT2;
    unsigned int   VCL_UINT4;
    unsigned long long  VCL_UINT8;               // golden unsigned __int64

    float VCL_FT4;
    double VCL_FT8;
    bool ret=true;

    i=SvEcReg.SV_ID->IndexOf(SVID);
    if(i>=0)
    {
        Type    =(unsigned char)atoi(SvEcReg.SV_TYPE->GetString(i).c_str());
        SVName  =SvEcReg.SV_NAME->GetString(i);
        SVUnit  =SvEcReg.SV_UNIT->GetString(i);
        IsVCL   =SvEcReg.VCL_NAME->GetString(i);
        P       =SvEcReg.SV_Ptr->Items[i];
        Len     =atoi(SvEcReg.SV_LEN->GetString(i).c_str());

        if(IsVCL==1 || IsVCL==2)
        {
            if(IsVCL==1)
            {
#if 0
                // AI(W906-SvEcDataItem) 20260720: GATED -- golden
                // uHGemEquipment.cpp:2513-2585 (dynamic_cast cluster). See
                // this method group's own file-head GATE NOTE.
                TObject *VclP=(TObject *)P;

                TPanel      *PanelPtr;
                TCustomEdit *EditPtr;
                TComboBox   *ComboBoxPtr;
                TLabel      *LabelPtr;
                TCheckBox   *CheckBoxPtr;
                TRadioGroup *RadioGroupPtr;
                TStringList *StringListPtr;

                PanelPtr     =dynamic_cast<TPanel       *>(VclP);
                EditPtr      =dynamic_cast<TCustomEdit  *>(VclP);
                ComboBoxPtr  =dynamic_cast<TComboBox    *>(VclP);
                LabelPtr     =dynamic_cast<TLabel       *>(VclP);
                CheckBoxPtr  =dynamic_cast<TCheckBox    *>(VclP);
                RadioGroupPtr=dynamic_cast<TRadioGroup  *>(VclP);
                StringListPtr=dynamic_cast<TStringList  *>(VclP);

                if(StringListPtr!=NULL)
                    VCLStr=StringListPtr->CommaText;
                else if(PanelPtr!=NULL)
                    VCLStr=PanelPtr->Caption;
                else if(EditPtr!=NULL)
                    VCLStr=EditPtr->Text;
                else if(ComboBoxPtr!=NULL)
                {
                    if(Type==HType.ASCII_TYPE)
                        VCLStr=ComboBoxPtr->Text;
                    else
                        VCLStr=ComboBoxPtr->ItemIndex;
                }
                else if(RadioGroupPtr!=NULL)
                {
                    if(RadioGroupPtr->ItemIndex>=0)
                    {
                        if(Type==HType.ASCII_TYPE)
                            VCLStr=RadioGroupPtr->Items->Strings[RadioGroupPtr->ItemIndex];
                        else
                            VCLStr=RadioGroupPtr->ItemIndex;
                    }
                    else
                    {
                        if(Type==HType.ASCII_TYPE)
                            VCLStr="";
                        else
                            VCLStr="-1";
                    }
                }
                else if(LabelPtr!=NULL)
                    VCLStr=LabelPtr->Caption;
                else if(CheckBoxPtr!=NULL)
                {
                    Type=HType.BOOLEAN_TYPE;
                    if(CheckBoxPtr->Checked)
                        VCLStr="1";
                    else
                        VCLStr="0";
                }
                else
                    VCLStr="";
#else
                // Unreachable from this unit's scope (see GATE NOTE above).
                // Conservative default matches golden's own final `else`
                // tail (nothing dynamic_casts successfully -> "").
                VCLStr="";
#endif
            }
            else
            {
                SS=(AnsiString *)P;
                VCLStr=*SS;
            }

            if(Type==HType.ASCII_TYPE)
            {
                strcpy(VCL_ASCII, VCLStr.c_str());
                P=VCL_ASCII;
            }
            else if(Type==HType.BINARY_TYPE)
            {
                VCL_BINARY=(char)atoi(VCLStr.c_str());
                P=&VCL_BINARY;
            }
            else if(Type==HType.BOOLEAN_TYPE)
            {
                VCL_BOOLEAN=(unsigned char)atoi(VCLStr.c_str());
                P=&VCL_BOOLEAN;
            }
            else if(Type==HType.INT_1_TYPE)
            {
                VCL_INT1=(char)atoi(VCLStr.c_str());
                P=&VCL_INT1;
            }
            else if(Type==HType.INT_2_TYPE)
            {
                VCL_INT2=(short)atoi(VCLStr.c_str());
                P=&VCL_INT2;
            }
            else if(Type==HType.INT_4_TYPE)
            {
                VCL_INT4=atoi(VCLStr.c_str());
                P=&VCL_INT4;
            }
            else if(Type==HType.INT_8_TYPE)                                    //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
            {
                VCL_INT8=strtoll(VCLStr.c_str(), NULL, 10);   // golden: _atoi64 (MinGW substitution)
                P=&VCL_INT8;
            }
            else if(Type==HType.UINT_1_TYPE)
            {
                VCL_UINT1=(unsigned char)atoi(VCLStr.c_str());
                P=&VCL_UINT1;
            }
            else if(Type==HType.UINT_2_TYPE)
            {
                VCL_UINT2=(unsigned short)atoi(VCLStr.c_str());
                P=&VCL_UINT2;
            }
            else if(Type==HType.UINT_4_TYPE)
            {
                VCL_UINT4=(unsigned)atoi(VCLStr.c_str());
                P=&VCL_UINT4;
            }
            else if(Type==HType.UINT_8_TYPE)                                    //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
            {
                VCL_UINT8=strtoull(VCLStr.c_str(), NULL, 10);   // golden: _atoi64 (MinGW substitution)
                P=&VCL_UINT8;
            }
            else if(Type==HType.FT_4_TYPE)
            {
                VCL_FT4=(float)atof(VCLStr.c_str());
                P=&VCL_FT4;
            }
            else if(Type==HType.FT_8_TYPE)
            {
                VCL_FT8=atof(VCLStr.c_str());
                P=&VCL_FT8;
            }
        }
        else
        {
            P=SvEcReg.SV_Ptr->Items[i];
        }

        if(Type==HType.LIST_TYPE)
        {
            int *Ptr;
            Ptr=(int *)P;
            WireCodec.DataItemOut(Len, HType.LIST_TYPE, NULL);
            for(i=0; i<Len; i++)
            {
                ret=DataItemOutSV(Ptr[i]);
                if(ret==false)
                    return false;
            }
        }

        if(Type==HType.ASCII_TYPE)
        {
            char *Ptr;
            Ptr=(char *)P;
            WireCodec.DataItemOut((int)strlen(Ptr), Type, Ptr);
        }
        else if(Type==HType.BINARY_TYPE)
        {
            char *Ptr;
            Ptr=(char *)P;
            WireCodec.DataItemOut(Len, Type, Ptr);
        }
        else if(Type==HType.BOOLEAN_TYPE)
        {
            unsigned char *Ptr;
            Ptr=(unsigned char *)P;
            WireCodec.DataItemOut(Len, Type, Ptr);
        }
        else if(Type==HType.INT_1_TYPE)
        {
            char *Ptr;
            Ptr=(char *)P;
            WireCodec.DataItemOut(Len, Type, Ptr);
        }
        else if(Type==HType.INT_2_TYPE)
        {
            short *Ptr;
            Ptr=(short *)P;
            WireCodec.DataItemOut(Len, Type, Ptr);
        }
        else if(Type==HType.INT_4_TYPE)
        {
            int *Ptr;
            Ptr=(int *)P;
            WireCodec.DataItemOut(Len, Type, Ptr);
        }
        else if(Type==HType.INT_8_TYPE)                                        //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
        {
            long long *Ptr;                                                    // golden __int64
            Ptr=(long long *)P;
            WireCodec.DataItemOut(Len, Type, Ptr);
        }
        else if(Type==HType.UINT_1_TYPE)
        {
            unsigned char *Ptr;
            Ptr=(unsigned char *)P;
            WireCodec.DataItemOut(Len, Type, Ptr);
        }
        else if(Type==HType.UINT_2_TYPE)
        {
            unsigned short *Ptr;
            Ptr=(unsigned short *)P;
            WireCodec.DataItemOut(Len, Type, Ptr);
        }
        else if(Type==HType.UINT_4_TYPE)
        {
            unsigned int *Ptr;
            Ptr=(unsigned int *)P;
            WireCodec.DataItemOut(Len, Type, Ptr);
        }
        else if(Type==HType.UINT_8_TYPE)                                       //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
        {
            unsigned long long *Ptr;                                          // golden unsigned __int64
            Ptr=(unsigned long long *)P;
            WireCodec.DataItemOut(Len, Type, Ptr);
        }
        else if(Type==HType.FT_4_TYPE)
        {
            float *Ptr;
            Ptr=(float *)P;
            WireCodec.DataItemOut(Len, Type, Ptr);
        }
        else if(Type==HType.FT_8_TYPE)
        {
            double *Ptr;
            Ptr=(double *)P;
            WireCodec.DataItemOut(Len, Type, Ptr);
        }
        return true;
    }

    S="SVID:"+SVID+" not define ";
    WireCodec.SendInvalidDataMessageToHost(S);                                  // 2013_09_06 steven
    DB->Lines->Add(S);
    return false;
}
// ---------------------------------------------------------------------------
// V 1.0 (golden uHGemEquipment.cpp:2765-2801)
// ---------------------------------------------------------------------------
bool THGem::DataItemOutSVNameList(AnsiString SVID)
{
    unsigned int j;
    AnsiString SVName;
    AnsiString SVUnit;
    AnsiString S;

    int iSV=SvEcReg.SV_ID->IndexOf(SVID);
    if(iSV>=0)
    {
        SVName=SvEcReg.SV_NAME->GetString(iSV);
        SVUnit=SvEcReg.SV_UNIT->GetString(iSV);

        WireCodec.DataItemOut(3, HType.LIST_TYPE, NULL);

        j=(unsigned int)atoi(SVID.c_str());
        S=j;

        if(SVID=="0" || S==SVID)
        {
            WireCodec.DataItemOut(1, HType.UINT_4_TYPE, &j);
        }
        else
        {
            WireCodec.DataItemOut(HType.ASCII_TYPE, SVID);
        }

        WireCodec.DataItemOut(HType.ASCII_TYPE, SVName);
        WireCodec.DataItemOut(HType.ASCII_TYPE, SVUnit);
        return true;
    }

    S="SVID:"+SVID+" not define ";
    WireCodec.SendInvalidDataMessageToHost(S);                                  //2013_09_06  steven
    DB->Lines->Add(S);
    return false;
}
// ---------------------------------------------------------------------------
// golden uHGemEquipment.cpp:2803-2840 -- Steven 20140911 : Add S103F11
// ---------------------------------------------------------------------------
bool THGem::DataItemOutSVNameListWithValue(AnsiString SVID)
{
    unsigned int j;
    AnsiString SVName;
    AnsiString SVUnit;
    AnsiString S;

    int iSV=SvEcReg.SV_ID->IndexOf(SVID);
    if(iSV>=0)
    {
        SVName=SvEcReg.SV_NAME->GetString(iSV);
        SVUnit=SvEcReg.SV_UNIT->GetString(iSV);

        WireCodec.DataItemOut(4, HType.LIST_TYPE, NULL);

        j=(unsigned int)atoi(SVID.c_str());
        S=j;

        if(SVID=="0" || S==SVID)
        {
            WireCodec.DataItemOut(1, HType.UINT_4_TYPE, &j);
        }
        else
        {
            WireCodec.DataItemOut(HType.ASCII_TYPE, SVID);
        }

        WireCodec.DataItemOut(HType.ASCII_TYPE, SVName);
        WireCodec.DataItemOut(HType.ASCII_TYPE, SVUnit);
        DataItemOutSV(SVID);
        return true;
    }

    S="SVID:"+SVID+" not define ";
    WireCodec.SendInvalidDataMessageToHost(S);                                  //2013_09_06  steven
    DB->Lines->Add(S);
    return false;
}
// ---------------------------------------------------------------------------
// V 1.0 (golden uHGemEquipment.cpp:2845-3112)
// EC output
// ---------------------------------------------------------------------------
void THGem::DataItemOutEC(AnsiString ECID)
{
    int i;
    unsigned char Type;
    AnsiString ECName;
    AnsiString ECUnit;
    void *P;
    AnsiString S, IsVCL, VCLStr, *SS;

    char  VCL_ASCII[10000];
    char  VCL_BINARY;
    char  VCL_INT1;
    short VCL_INT2;
    int   VCL_INT4;
    long long  VCL_INT8;                        // golden __int64
    unsigned char  VCL_UINT1,VCL_BOOLEAN;
    unsigned short VCL_UINT2;
    unsigned int   VCL_UINT4;
    unsigned long long  VCL_UINT8;               // golden unsigned __int64

    float VCL_FT4;
    double VCL_FT8;

    i=SvEcReg.EC_ID->IndexOf(ECID);
    if(i>=0)
    {
        IsVCL   =SvEcReg.EC_VCL_NAME->GetString(i);
        Type    =(unsigned char)atoi(SvEcReg.EC_TYPE->GetString(i).c_str());
        ECName  =SvEcReg.EC_NAME->GetString(i);
        ECUnit  =SvEcReg.EC_UNIT->GetString(i);
        P       =SvEcReg.EC_Ptr->Items[i];

        if(IsVCL==1 || IsVCL==2)
        {
            if(IsVCL==1)
            {
#if 0
                // AI(W906-SvEcDataItem) 20260720: GATED -- golden
                // uHGemEquipment.cpp:2885-2956 (dynamic_cast cluster). See
                // this method group's own file-head GATE NOTE.
                Type    =(unsigned char)atoi(SvEcReg.EC_TYPE->GetString(i).c_str());
                ECName  =SvEcReg.EC_NAME->GetString(i);
                ECUnit  =SvEcReg.EC_UNIT->GetString(i);
                TObject *VclP=(TObject *)P;

                TPanel      *PanelPtr;
                TCustomEdit *EditPtr;
                TComboBox   *ComboBoxPtr;
                TLabel      *LabelPtr;
                TCheckBox   *CheckBoxPtr;
                TRadioGroup *RadioGroupPtr;
                TStringList *StringListPtr;

                PanelPtr     =dynamic_cast<TPanel       *>(VclP);
                EditPtr      =dynamic_cast<TCustomEdit  *>(VclP);
                ComboBoxPtr  =dynamic_cast<TComboBox    *>(VclP);
                LabelPtr     =dynamic_cast<TLabel       *>(VclP);
                CheckBoxPtr  =dynamic_cast<TCheckBox    *>(VclP);
                RadioGroupPtr=dynamic_cast<TRadioGroup  *>(VclP);
                StringListPtr=dynamic_cast<TStringList  *>(VclP);

                if(StringListPtr!=NULL)
                    VCLStr=StringListPtr->CommaText;
                else if(PanelPtr!=NULL)
                    VCLStr=PanelPtr->Caption;
                else if(EditPtr!=NULL)
                    VCLStr=EditPtr->Text;
                else if(ComboBoxPtr!=NULL)
                {
                    if(Type==HType.ASCII_TYPE)
                        VCLStr=ComboBoxPtr->Text;
                    else
                        VCLStr=ComboBoxPtr->ItemIndex;
                }
                else if(RadioGroupPtr!=NULL)
                {
                    if(RadioGroupPtr->ItemIndex>=0)
                    {
                        if(Type==HType.ASCII_TYPE)
                            VCLStr=RadioGroupPtr->Items->Strings[RadioGroupPtr->ItemIndex];
                        else
                            VCLStr=RadioGroupPtr->ItemIndex;
                    }
                    else
                    {
                        if(Type==HType.ASCII_TYPE)
                            VCLStr="";
                        else
                            VCLStr="-1";
                    }
                }
                else if(LabelPtr!=NULL)
                    VCLStr=LabelPtr->Caption;
                else if(CheckBoxPtr!=NULL)
                {
                    Type=HType.BOOLEAN_TYPE;
                    if(CheckBoxPtr->Checked)
                        VCLStr="1";
                    else
                        VCLStr="0";
                }
                else
                    VCLStr="";
#else
                // Unreachable from this unit's scope (see GATE NOTE above).
                VCLStr="";
#endif
            }
            else
            {
                SS=(AnsiString *)P;
                VCLStr=*SS;
            }

            if(Type==HType.ASCII_TYPE)
            {
                strcpy(VCL_ASCII, VCLStr.c_str());
                P=VCL_ASCII;
            }
            else if(Type==HType.BINARY_TYPE)
            {
                VCL_BINARY=(char)atoi(VCLStr.c_str());
                P=&VCL_BINARY;
            }
            else if(Type==HType.BOOLEAN_TYPE)
            {
                VCL_BOOLEAN=(unsigned char)atoi(VCLStr.c_str());
                P=&VCL_BOOLEAN;
            }
            else if(Type==HType.INT_1_TYPE)
            {
                VCL_INT1=(char)atoi(VCLStr.c_str());
                P=&VCL_INT1;
            }
            else if(Type==HType.INT_2_TYPE)
            {
                VCL_INT2=(short)atoi(VCLStr.c_str());
                P=&VCL_INT2;
            }
            else if(Type==HType.INT_4_TYPE)
            {
                VCL_INT4=atoi(VCLStr.c_str());
                P=&VCL_INT4;
            }
            else if(Type==HType.INT_8_TYPE)                                    //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
            {
                VCL_INT8=strtoll(VCLStr.c_str(), NULL, 10);   // golden: _atoi64 (MinGW substitution)
                P=&VCL_INT8;
            }
            else if(Type==HType.UINT_1_TYPE)
            {
                VCL_UINT1=(unsigned char)atoi(VCLStr.c_str());
                P=&VCL_UINT1;
            }
            else if(Type==HType.UINT_2_TYPE)
            {
                VCL_UINT2=(unsigned short)atoi(VCLStr.c_str());
                P=&VCL_UINT2;
            }
            else if(Type==HType.UINT_4_TYPE)
            {
                VCL_UINT4=(unsigned)atoi(VCLStr.c_str());
                P=&VCL_UINT4;
            }
            else if(Type==HType.UINT_8_TYPE)                                    //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
            {
                VCL_UINT8=strtoull(VCLStr.c_str(), NULL, 10);   // golden: _atoi64 (MinGW substitution)
                P=&VCL_UINT8;
            }
            else if(Type==HType.FT_4_TYPE)                                     // Lee
            {
                VCL_FT4=(float)atof(VCLStr.c_str());
                P=&VCL_FT4;
            }
            else if(Type==HType.FT_8_TYPE)
            {
                VCL_FT8=atof(VCLStr.c_str());
                P=&VCL_FT8;
            }
        }

        if(Type==HType.ASCII_TYPE)
        {
            char *Ptr;
            Ptr=(char *)P;
            WireCodec.DataItemOut((int)strlen(Ptr), Type, Ptr);
        }
        else if(Type==HType.BINARY_TYPE)
        {
            char *Ptr;
            Ptr=(char *)P;
            WireCodec.DataItemOut(1, Type, Ptr);
        }
        else if(Type==HType.BOOLEAN_TYPE)
        {
            unsigned char *Ptr;
            Ptr=(unsigned char *)P;
            WireCodec.DataItemOut(1, Type, Ptr);
        }
        else if(Type==HType.INT_1_TYPE)
        {
            char *Ptr;
            Ptr=(char *)P;
            WireCodec.DataItemOut(1, Type, Ptr);
        }
        else if(Type==HType.INT_2_TYPE)
        {
            short *Ptr;
            Ptr=(short *)P;
            WireCodec.DataItemOut(1, Type, Ptr);
        }
        else if(Type==HType.INT_4_TYPE)
        {
            int *Ptr;
            Ptr=(int *)P;
            WireCodec.DataItemOut(1, Type, Ptr);
        }
        else if(Type==HType.INT_8_TYPE)                                        //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
        {
            long long *Ptr;                                                    // golden __int64
            Ptr=(long long *)P;
            WireCodec.DataItemOut(1, Type, Ptr);
        }
        else if(Type==HType.UINT_1_TYPE)
        {
            unsigned char *Ptr;
            Ptr=(unsigned char *)P;
            WireCodec.DataItemOut(1, Type, Ptr);
        }
        else if(Type==HType.UINT_2_TYPE)
        {
            unsigned short *Ptr;
            Ptr=(unsigned short *)P;
            WireCodec.DataItemOut(1, Type, Ptr);
        }
        else if(Type==HType.UINT_4_TYPE)
        {
            unsigned int *Ptr;
            Ptr=(unsigned int *)P;
            WireCodec.DataItemOut(1, Type, Ptr);
        }
        else if(Type==HType.UINT_8_TYPE)                                       //Steven 20140911 : 修正INT_8_TYPE & UINT_8_TYPE
        {
            unsigned long long *Ptr;                                          // golden unsigned __int64
            Ptr=(unsigned long long *)P;
            WireCodec.DataItemOut(1, Type, Ptr);
        }
        else if(Type==HType.FT_4_TYPE)
        {
            float *Ptr;
            Ptr=(float *)P;
            WireCodec.DataItemOut(1, Type, Ptr);
        }
        else if(Type==HType.FT_8_TYPE)
        {
            double *Ptr;
            Ptr=(double *)P;
            WireCodec.DataItemOut(1, Type, Ptr);
        }
        return;
    }
    WireCodec.DataItemOut(0, HType.LIST_TYPE, NULL);
}
//------------------------------------------------------------------------------
// golden uHGemEquipment.cpp:3114-3122
//------------------------------------------------------------------------------
bool THGem::IsValidSVID(AnsiString SVID)
{
    int i=SvEcReg.SV_ID->IndexOf(SVID);
    if(i>=0)
    {
        return true;
    }
    return false;
}
//==============================================================================
// V 1.0 (golden uHGemEquipment.cpp:3127-3332)
// for EC Name List
//==============================================================================
void THGem::DataItemOutECNameList(AnsiString ECID)
{
    int i, j;
    unsigned char Type;
    AnsiString ECName;
    AnsiString ECUnit;
    char MaxForAscii[256], MinForAscii[256], DefaultForAscii[256];              // 2013/05/08 lee
    void *PMax,*PMin,*PDefault;
    AnsiString S;
    AnsiString PMax_Value,PMin_Value,PDefault_Value;

    i=SvEcReg.EC_ID->IndexOf(ECID);
    if(i>=0)
    {
        Type  =(unsigned char)atoi(SvEcReg.EC_TYPE->GetString(i).c_str());
        ECName=SvEcReg.EC_NAME->GetString(i);
        ECUnit=SvEcReg.EC_UNIT->GetString(i);

        PMax=SvEcReg.EC_Ptr_Max->Items[i];
        PMin=SvEcReg.EC_Ptr_Min->Items[i];
        PDefault=SvEcReg.EC_Ptr_Default->Items[i];

        PMax_Value      =SvEcReg.EC_Ptr_Max_Value->GetString(i);
        PMin_Value      =SvEcReg.EC_Ptr_Min_Value->GetString(i);
        PDefault_Value  =SvEcReg.EC_Ptr_Default_Value->GetString(i);

        if(PMax==NULL || PMin==NULL || PDefault==NULL)
        {
            if(Type==HType.ASCII_TYPE )                                         // 2013/05/08 lee
            {
                strcpy(MaxForAscii, PMax_Value.c_str());
                strcpy(MinForAscii, PMin_Value.c_str());
                strcpy(DefaultForAscii, PDefault_Value.c_str());
                PMax=(void *)&MaxForAscii;
                PMin=(void *)&MinForAscii;
                PDefault=(void *)&DefaultForAscii;
            }
            else if(Type==HType.BINARY_TYPE)                                    // 2013/05/08 lee
            {
                unsigned char Max,Min,Default;
                Max=(unsigned char)atoi(PMax_Value.c_str());
                Min=(unsigned char)atoi(PMin_Value.c_str());
                Default=(unsigned char)atoi(PDefault_Value.c_str());
                PMax=(void *)&Max;
                PMin=(void *)&Min;
                PDefault=(void *)&Default;
            }
            else if(Type==HType.BOOLEAN_TYPE)
            {
                bool Max,Min,Default;
                Max=atoi(PMax_Value.c_str())!=0;
                Min=atoi(PMin_Value.c_str())!=0;
                Default=atoi(PDefault_Value.c_str())!=0;
                PMax=(void *)&Max;
                PMin=(void *)&Min;
                PDefault=(void *)&Default;
            }
            else if(Type==HType.UINT_1_TYPE ||
                    Type==HType.UINT_2_TYPE ||
                    Type==HType.UINT_4_TYPE )
            {
                unsigned int Max,Min,Default;
                Max=(unsigned)atoi(PMax_Value.c_str());
                Min=(unsigned)atoi(PMin_Value.c_str());
                Default=(unsigned)atoi(PDefault_Value.c_str());
                PMax=(void *)&Max;
                PMin=(void *)&Min;
                PDefault=(void *)&Default;
            }
            else if(Type==HType.UINT_8_TYPE)
            {
                unsigned long long Max,Min,Default;                            // golden unsigned __int64
                Max=(unsigned long long)atoi(PMax_Value.c_str());
                Min=(unsigned long long)atoi(PMin_Value.c_str());
                Default=(unsigned long long)atoi(PDefault_Value.c_str());
                PMax=(void *)&Max;
                PMin=(void *)&Min;
                PDefault=(void *)&Default;
            }
            else if(Type==HType.INT_1_TYPE ||
                    Type==HType.INT_2_TYPE ||
                    Type==HType.INT_4_TYPE )
            {
                int Max,Min,Default;
                Max=atoi(PMax_Value.c_str());
                Min=atoi(PMin_Value.c_str());
                Default=atoi(PDefault_Value.c_str());
                PMax=(void *)&Max;
                PMin=(void *)&Min;
                PDefault=(void *)&Default;
            }
            else if(Type==HType.INT_8_TYPE)
            {
                long long Max,Min,Default;                                     // golden __int64
                Max=atoi(PMax_Value.c_str());
                Min=atoi(PMin_Value.c_str());
                Default=atoi(PDefault_Value.c_str());
                PMax=(void *)&Max;
                PMin=(void *)&Min;
                PDefault=(void *)&Default;
            }
            else if(Type==HType.FT_4_TYPE)
            {
                float Max,Min,Default;
                Max=(float)atoi(PMax_Value.c_str());
                Min=(float)atoi(PMin_Value.c_str());
                Default=(float)atoi(PDefault_Value.c_str());
                PMax=(void *)&Max;
                PMin=(void *)&Min;
                PDefault=(void *)&Default;
            }
            else if(Type==HType.FT_8_TYPE)
            {
                double Max,Min,Default;
                Max=atoi(PMax_Value.c_str());
                Min=atoi(PMin_Value.c_str());
                Default=atoi(PDefault_Value.c_str());
                PMax=(void *)&Max;
                PMin=(void *)&Min;
                PDefault=(void *)&Default;
            }
        }
        //2013/05/08 lee start

        // 1. L,6
        //    1. <ECID1>
        //    2. <ECNAME1>
        //    3. <ECMIN1>
        //    4. <ECMAX1>
        //    5. <ECDEF1>
        //    6. <UNITS1>

        //2013/05/08 lee end
        WireCodec.DataItemOut(6, HType.LIST_TYPE, NULL);

        j=atoi(ECID.c_str());
        S=j;
        if(ECID=="0" || S==ECID)
        {
            WireCodec.DataItemOut(1, HType.UINT_4_TYPE, &j);
        }
        else
        {
            WireCodec.DataItemOut(HType.ASCII_TYPE, ECID);
        }

        WireCodec.DataItemOut(HType.ASCII_TYPE, ECName);

        if(PMin==NULL)
        {
            // AI(W906-SvEcDataItem) 20260720: golden `DataItemOut(0, Type, "")`
            // -- a string literal cannot bind to WireCodec::DataItemOut's
            // `void *P` param in standard C++ (dropping const on a
            // `const char*` is not an implicit conversion). Substituted with
            // NULL: SecsWireCodec::DataItemOut's own len==0 path never
            // dereferences P (verified directly in that method's body -- the
            // `for(i=0;i<len;i++)` loop bound by len==0 never executes), so
            // this is byte-for-byte behaviorally identical to golden, not a
            // behavior change -- same class of forced type-system
            // accommodation as SecsSvEcRegistration.h's own documented
            // `const char*` JUDGMENT CALL.
            WireCodec.DataItemOut(0, Type, NULL);
        }
        else
        {
            if(Type==HType.ASCII_TYPE)
                WireCodec.DataItemOut(Type, MinForAscii);                      // 2013/05/08 lee
            else
                WireCodec.DataItemOut(1, Type, PMin);
        }

        if(PMax==NULL)
        {
            WireCodec.DataItemOut(0, Type, NULL);                              // see NULL-for-"" note above
        }
        else
        {
            if(Type==HType.ASCII_TYPE)
                WireCodec.DataItemOut(Type, MaxForAscii);                      // 2013/05/08 lee
            else
                WireCodec.DataItemOut(1, Type, PMax);
        }

        if(PDefault==NULL)
        {
            WireCodec.DataItemOut(0, Type, NULL);                              // see NULL-for-"" note above
        }
        else
        {
            if(Type==HType.ASCII_TYPE)                                         // 2013/05/08 lee
                WireCodec.DataItemOut(Type, DefaultForAscii);
            else
                WireCodec.DataItemOut(1, Type, PDefault);
        }

        WireCodec.DataItemOut(HType.ASCII_TYPE, ECUnit);
        return;
    }
    WireCodec.DataItemOut(6, HType.LIST_TYPE, NULL);
    j=atoi(ECID.c_str());
    S=j;
    if(ECID=="0" || S==ECID)
    {
        WireCodec.DataItemOut(1, HType.UINT_4_TYPE, &j);
    }
    else
    {
        WireCodec.DataItemOut(HType.ASCII_TYPE, ECID);
    }

    WireCodec.DataItemOut(0, HType.ASCII_TYPE, NULL);                          // see NULL-for-"" note above
    WireCodec.DataItemOut(0, HType.ASCII_TYPE, NULL);
    WireCodec.DataItemOut(0, HType.ASCII_TYPE, NULL);
    WireCodec.DataItemOut(0, HType.ASCII_TYPE, NULL);
    WireCodec.DataItemOut(0, HType.ASCII_TYPE, NULL);
}
//------------------------------------------------------------------------------
// golden uHGemEquipment.cpp:7623-7632 (__fastcall dropped, project convention)
//------------------------------------------------------------------------------
void THGem::SendRepoerID(unsigned iReportID)
{
    unsigned ct;
    unsigned iSVIDData[1024];
    ct=GetReportIDContent(iReportID, iSVIDData, 2);
    WireCodec.DataItemOut(1, HType.UINT_4_TYPE, &iReportID);
    WireCodec.DataItemOut((int)ct, HType.LIST_TYPE, NULL);
    for(unsigned i=0; i<ct; i++)
        DataItemOutSV(iSVIDData[i]);
}
//------------------------------------------------------------------------------
// golden uHGemEquipment.cpp:7648-7662 (__fastcall dropped, project convention)
//------------------------------------------------------------------------------
void THGem::SendAnnotatedRepoerID(unsigned iReportID)
{
    unsigned ct, svid;
    unsigned iSVIDData[10240];
    ct=GetReportIDContent(iReportID, iSVIDData, 2);
    WireCodec.DataItemOut(1, HType.UINT_4_TYPE, &iReportID);
    WireCodec.DataItemOut((int)ct, HType.LIST_TYPE, NULL);
    for(unsigned i=0; i<ct; i++)
    {
        WireCodec.DataItemOut(2, HType.LIST_TYPE, NULL);
        svid=iSVIDData[i];
        WireCodec.DataItemOut(1, HType.UINT_4_TYPE, &svid);
        DataItemOutSV(iSVIDData[i]);
    }
}
//------------------------------------------------------------------------------
// golden uHGemEquipment.cpp:7664-7675 (__fastcall dropped, project convention)
//------------------------------------------------------------------------------
void THGem::SendCeid(unsigned iCeid)
{
    unsigned ct;
    unsigned iReportIDData[10240];
    ct=GetCEIDContent(iCeid, iReportIDData, 2);
    WireCodec.DataItemOut((int)ct, HType.LIST_TYPE, NULL);
    for(unsigned i=0; i<ct; i++)
    {
        WireCodec.DataItemOut(2, HType.LIST_TYPE, NULL);
        SendRepoerID(iReportIDData[i]);
    }
}
//------------------------------------------------------------------------------
// golden uHGemEquipment.cpp:7677-7688 (__fastcall dropped, project convention)
//------------------------------------------------------------------------------
void THGem::SendAnnotatedCeid(unsigned iCeid)
{
    unsigned ct;
    unsigned iReportIDData[10240];
    ct=GetCEIDContent(iCeid, iReportIDData, 2);
    WireCodec.DataItemOut((int)ct, HType.LIST_TYPE, NULL);
    for(unsigned i=0; i<ct; i++)
    {
        WireCodec.DataItemOut(2, HType.LIST_TYPE, NULL);
        SendAnnotatedRepoerID(iReportIDData[i]);
    }
}
//-------------------------------------------------------------------------------
// V 1.0 (golden uHGemEquipment.cpp:6165-6207)
// FormCreate -- registers the "system SV/EC" family (clock/link-state/
// comm-mode/CPU/disk/memory/model/software-rev/spool-count/spool-start-time/
// time-format EC/receipe-struct/receipe-extend). The bulk of golden's SV/EC
// registration (uHGemHT9045_SV.cpp/_EC.cpp, ~877+~1740 calls) is OUT OF
// SCOPE (see SecsSvEcRegistration.h's own "INTEGRATE-AGENT WIRING POINT"
// note) -- unaffected by this method.
//-------------------------------------------------------------------------------
void THGem::FormCreate(TObject *Sender)
{
    (void)Sender;
    //2013/09/16 lee start
    AnsiString S;
    lCPUFreq=GetCPUFreq();
    S=GetManID();
    strcpy(szManID, S.c_str());
    S=GetCPUType();
    strcpy(szGetCPUType, S.c_str());
//    2013/09/16 lee end

    SvEcReg.SetSVDataPointer(3, HType.ASCII_TYPE,   "GemClock",                         "",         &GemClock,                      "Equipment 目前時鐘");
    SvEcReg.SetSVDataPointer(4, HType.UINT_1_TYPE,  "GemControlState",                  "",         &GemControlState,               "Equipment 目前連線狀態    1: OffLine               , 2:OnLine Local                               , 3:OnLine Remote");
    SvEcReg.SetSVDataPointer(5, HType.UINT_1_TYPE,  "GemLinkState",                     "",         &GemLinkState,                  "目前連接狀態    0:Disabled              , 1:Enabled/Not Communicating       , 2: Communicating");
    SvEcReg.SetSVDataPointer(6, HType.INT_1_TYPE,   "SECSCommunicationMode",            "",         &SECSCommunicationMode,         "0:HSMS Mode        , 1:SECS Mode (Set By AP)");

    SvEcReg.SetSVDataPointer(9, HType.UINT_1_TYPE,  "PreviousGemControlState",                  "", &GemControlPreState,            "Equipment 目前連線狀態    1: OffLine               , 2:OnLine Local                               , 3:OnLine Remote");
    //2013/09/16 lee start
    SvEcReg.SetSVDataPointer(10, HType.INT_4_TYPE,  "CPU Frequence",                    "",         &lCPUFreq,                      "CPU operate MHZ");
    SvEcReg.SetSVDataPointer(11, HType.ASCII_TYPE,  "CPU  Manufacturer",                "",         szManID,                        "");
    SvEcReg.SetSVDataPointer(12, HType.ASCII_TYPE,  "CPU  Type",                        "",         szGetCPUType,                   "");
    //2013/09/16 lee end

    //2013/09/30 lee start
    SvEcReg.SetSVDataPointer(13, HType.INT_4_TYPE,  "Total Space Of Disk C",            "MB",       &Disk_C_TotalSpaceMB,           "Total Space Of Disk C");
    SvEcReg.SetSVDataPointer(14, HType.INT_4_TYPE,  "Total Space Of Disk D",            "MB",       &Disk_D_TotalSpaceMB,           "Total Space Of Disk D");
    SvEcReg.SetSVDataPointer(15, HType.INT_4_TYPE,  "Total FreeSpace Of Disk C",        "MB",       &Disk_C_TotalFreeSpaceMB,       "Total FreeSpace Of Disk C");
    SvEcReg.SetSVDataPointer(16, HType.INT_4_TYPE,  "Total FreeSpace Of Disk D",        "MB",       &Disk_D_TotalFreeSpaceMB,       "Total FreeSpace Of Disk D");
    SvEcReg.SetSVDataPointer(17, HType.UINT_4_TYPE, "Memory Load Percent",              "%",        &ulMemoryLoad,                  "Memory Load Percent");
    SvEcReg.SetSVDataPointer(18, HType.UINT_4_TYPE, "Memory Total Physic",              "%",        &ulTotalPhys,                   "Memory Total Physic");
    SvEcReg.SetSVDataPointer(19, HType.UINT_4_TYPE, "Memory Avail Physic",              "%",        &ulAvailPhys,                   "Memory Avail Physic");
    //2013/09/30 lee end

    SvEcReg.SetSVDataPointer(24, HType.ASCII_TYPE,  "GemMDLN",                          "",         &GemMDLN,                       "機台型號");
    SvEcReg.SetSVDataPointer(25, HType.ASCII_TYPE,  "GemSOFTREV",                       "",         &GemSOFTREV,                    "軟體版本");
    SvEcReg.SetSVDataPointer(54, HType.UINT_4_TYPE, "GemSpoolCountActual",              "record",   &GemSpoolCountActual,           "Spool enable or disable");
    SvEcReg.SetSVDataPointer(57, HType.ASCII_TYPE,  "GemSpoolStartTime",                "",         GemSpoolStartTime,              "Spool start time");

    SvEcReg.SetECDataPointer(68, HType.UINT_1_TYPE, "Time Format",                      "",         &iTimeFormat, iMinTimeFormat, iMaxTimeFormat, iTimeFormatDefault, "");

    SvEcReg.SetSVDataPointer(70, HType.UINT_1_TYPE, "Receipe Struct",                   "",         &SV_70_UNT1_ReceipeStruct,      "用來定義目前工作檔的型態 0:單一File 的結構 1:多檔結構如 .BLD ,.OFF 等組合 2:以目錄當工作檔");
    SvEcReg.SetSVDataPointer(71, HType.ASCII_TYPE,  "Receipe Extend",                   "",         &SV_71_ASCII_FilenameExtened,   "Receipe extend filename");
}

//===========================================================================
//  CEID / Report StringGrid-backed "database" family
//  (golden uHGemEquipment.cpp:7361-7967, 8615-8665)
//===========================================================================
//------------------------------------------------------------------------------
// mode=1 是Handler ,=0 是 Host 定義的 (gloss: "Mode 1 = defined by the
// Handler itself, Mode 0 = defined by the Host" -- golden uHGemEquipment.cpp:7361)
void THGem::SetCEIDContent(unsigned iCeid, AnsiString CeidAlias, unsigned iReportCount, unsigned *iReportIDData, int Mode)
{
    // AI(W906-fire-verify) 20260716: `pos` is zero-initialized here; golden
    // leaves `int pos;` uninitialized (uHGemEquipment.cpp:7363). Behaviorally
    // inert either way -- every read of `pos` below is guarded by
    // `bSearchOK`, which is only ever true after `pos` was itself just
    // assigned a few lines above -- but flagged per fire-wave fidelity review
    // as a literal (if harmless) deviation from verbatim translation.
    int pos = 0;
    bool bSearchOK = false;
    unsigned int iBackID[SECS_EVENT.TotalEvent], ct = 0;
    AnsiString S;

    for (int y = 1; y < strGrdCEID->RowCount; y++)
    {
        if (strGrdCEID->Cells[0][y] == iCeid)
        {
            bSearchOK = true;
            pos = y;
            break;
        }
    }

    if (bSearchOK == false)
    {
        for (int y = 1; y < strGrdCEID->RowCount; y++)
        {
            if (strGrdCEID->Cells[0][y] == "")
            {
                strGrdCEID->Cells[0][y] = iCeid;
                strGrdCEID->Cells[1][y] = Mode;
                strGrdCEID->Cells[2][y] = CeidAlias;

                for (int i = 3; i < strGrdCEID->ColCount; i++)
                    strGrdCEID->Cells[i][y] = "";
                for (unsigned int x = 3; x < (iReportCount + 3); x++)
                    strGrdCEID->Cells[x][y] = iReportIDData[x - 3];
                break;
            }
        }
    }
    else
    {
        strGrdCEID->Cells[2][pos] = CeidAlias;
        for (int i = 3; i < strGrdCEID->ColCount; i++)
        {
            S = strGrdCEID->Cells[i][pos];
            if (S == "")
                continue;
            if (GetReportIDType(S) == (1 - Mode))
            {
                iBackID[ct] = atoi(S.c_str());
                ct++;
            }
        }
        for (int i = 3; i < strGrdCEID->ColCount; i++)
            strGrdCEID->Cells[i][pos] = "";
        for (unsigned int x = 0; x < iReportCount; x++)
            strGrdCEID->Cells[x + 3][pos] = iReportIDData[x];
        for (unsigned int x = 0; x < ct; x++)
            strGrdCEID->Cells[x + iReportCount + 3][pos] = iBackID[x];
    }
    SaveEventReportData();
}
//------------------------------------------------------------------------------
// mode=1 是Handler ,=0 是 Host 定義的
void THGem::SetCEIDContent(unsigned iCeid, unsigned iReportCount, unsigned *iReportIDData, int Mode)
{
    SetCEIDContent(iCeid, "", iReportCount, iReportIDData, Mode);
}
//------------------------------------------------------------------------------
unsigned THGem::GetCEIDContent(unsigned iCeid, unsigned *iReportIDData, int Mode)
{
    (void)Mode;   // golden also never reads Mode inside this method's body
    int ct = 0;
    for (int y = 1; y < strGrdCEID->RowCount; y++)
    {
        if (strGrdCEID->Cells[0][y] == iCeid)
        {
            // AI(W906-uHGemEquipment) 20260716: GOLDEN BUG, preserved
            // verbatim (uHGemEquipment.cpp:7432): loop bound is
            // `strGrdCEID->RowCount`, not `->ColCount` as every other sibling
            // in this family uses (e.g. SetCEIDContent's own equivalent loop
            // just above uses `->ColCount`). strGrdCEID is 1025 rows x 258
            // cols, so RowCount(1025) > ColCount(258) here -- harmless in
            // practice ONLY because the loop always `break`s on the first ""
            // cell, which (per SetCEIDContent's own writer logic) is never
            // later than column 257. If a CEID row were ever fully packed
            // with report IDs across all 255 usable slots, this would walk
            // past the real column count and hit vclcompat::TStringGrid's
            // out-of-range check (std::out_of_range) -- matching real VCL's
            // own ERangeError in that scenario. NOT fixed here.
            for (int x = 3; x < strGrdCEID->RowCount; x++)
            {
                if (strGrdCEID->Cells[x][y] == "")
                    break;
                iReportIDData[ct] = static_cast<unsigned>(atoi(strGrdCEID->Cells[x][y].c_str()));
                ct++;
            }
            return static_cast<unsigned>(ct);
        }
    }
    return 0;
}
//------------------------------------------------------------------------------
// mode=1 是Handler ,=0 是 Host 定義的
bool THGem::SetReportIDContent(unsigned iCeid, unsigned iReportCount, unsigned *iReportIDData, int Mode)
{
    // AI(W906-fire-verify) 20260716: same zero-init-vs-golden-uninitialized
    // `pos` deviation as SetCEIDContent above (golden uHGemEquipment.cpp:7447)
    // -- behaviorally inert, see that function's comment for the full note.
    int pos = 0;
    bool bSearchOK = false;

    if ((int)(iReportCount + 2) > stdGridReportID->ColCount)
        return false;

    for (int y = 1; y < stdGridReportID->RowCount; y++)
    {
        if (stdGridReportID->Cells[0][y] == iCeid)
        {
            bSearchOK = true;
            pos = y;
            break;
        }
    }

    if (bSearchOK == false)
    {
        for (int y = 1; y < stdGridReportID->RowCount; y++)
        {
            if (stdGridReportID->Cells[0][y] == "")
            {
                stdGridReportID->Cells[0][y] = iCeid;
                stdGridReportID->Cells[1][y] = Mode;
                for (int x = 2; x < stdGridReportID->ColCount; x++)
                    stdGridReportID->Cells[x][y] = "";
                for (unsigned int i = 2; i < (iReportCount + 2); i++)
                    stdGridReportID->Cells[i][y] = iReportIDData[i - 2];
                bSearchOK = true;
                break;
            }
        }
    }
    else
    {
        for (int x = 2; x < stdGridReportID->ColCount; x++)
            stdGridReportID->Cells[x][pos] = "";
        for (unsigned int i = 2; i < (iReportCount + 2); i++)
            stdGridReportID->Cells[i][pos] = iReportIDData[i - 2];
    }
    SaveEventReportData();
    return bSearchOK;
}
//------------------------------------------------------------------------------
// 2013/11/20 lee
bool THGem::DeleteReportID(unsigned iCeid, int Type)
{
    // AI(W906-fire-verify) 20260716: same zero-init-vs-golden-uninitialized
    // `pos` deviation as SetCEIDContent above (golden uHGemEquipment.cpp:7493)
    // -- behaviorally inert, see that function's comment for the full note.
    int pos = 0, iY;
    bool bSearchOK = false;

    for (int y = 1; y < stdGridReportID->RowCount; y++)
    {
        if (stdGridReportID->Cells[0][y] == iCeid &&
            stdGridReportID->Cells[1][y] == Type)
        {
            pos = y;
            bSearchOK = true;
            break;
        }
    }

    if (bSearchOK == true)
    {
        for (int y = pos; y < (stdGridReportID->RowCount - 1); y++)
        {
            for (int x = 0; x < stdGridReportID->ColCount; x++)
                stdGridReportID->Cells[x][y] = stdGridReportID->Cells[x][y + 1];
        }

        iY = stdGridReportID->RowCount - 1;
        for (int x = 0; x < stdGridReportID->ColCount; x++)
            stdGridReportID->Cells[x][iY] = "";
        return true;
    }
    else
    {
        return false;
    }
}
//------------------------------------------------------------------------------
bool THGem::DeleteReportIDOfCeid(unsigned iCeid)
{
    int ct;
    AnsiString S[1024];

    for (int y = 1; y < strGrdCEID->RowCount; y++)
    {
        for (int x = 3; x < strGrdCEID->ColCount; x++)
        {
            if (strGrdCEID->Cells[x][y] == iCeid)
                strGrdCEID->Cells[x][y] = "";
        }
        ct = 0;
        for (int x = 3; x < strGrdCEID->ColCount; x++)
        {
            if (strGrdCEID->Cells[x][y] != "")
            {
                S[ct] = strGrdCEID->Cells[x][y];
                ct++;
            }
        }

        for (int x = 3; x < strGrdCEID->ColCount; x++)
            strGrdCEID->Cells[x][y] = "";

        for (int i = 0; i < ct; i++)
            strGrdCEID->Cells[3 + i][y] = S[i];
    }
    return true;
}
//------------------------------------------------------------------------------
// 2013/11/20 lee
bool THGem::DeleteAllHostDefineReportID()
{
    unsigned iReportID;
    bool bSearchOK;
    do
    {
        bSearchOK = false;
        for (int y = 1; y < stdGridReportID->RowCount; y++)
        {
            if (stdGridReportID->Cells[1][y] == "0")
            {
                iReportID = static_cast<unsigned>(atoi(stdGridReportID->Cells[0][y].c_str()));
                DeleteReportID(iReportID, 0);
                DeleteReportIDOfCeid(iReportID);
                bSearchOK = true;
                break;
            }
        }
    } while (bSearchOK == true);

    return true;
}
//------------------------------------------------------------------------------
unsigned THGem::GetReportIDContent(unsigned iCeid, unsigned *iReportIDData, int Mode)
{
    (void)Mode;   // golden also never reads Mode inside this method's body
    int ct = 0;
    for (int y = 1; y < stdGridReportID->RowCount; y++)
    {
        if (stdGridReportID->Cells[0][y] == iCeid)
        {
            // AI(W906-uHGemEquipment) 20260716: GOLDEN BUG, preserved
            // verbatim (uHGemEquipment.cpp:7587) -- same class of bug as
            // GetCEIDContent above, mirrored the OTHER direction: loop bound
            // is `stdGridReportID->RowCount` (257) where `->ColCount` (1026)
            // is clearly intended. Here the practical effect is the
            // OPPOSITE of GetCEIDContent's: this grid's RowCount(257) is
            // SMALLER than its ColCount(1026), so the loop can terminate
            // EARLY (at x==257) even though up to 1024 usable SVID slots
            // exist per report -- silently truncating a Report with more
            // than ~255 SVIDs to its first ~255. NOT fixed here.
            for (int x = 2; x < stdGridReportID->RowCount; x++)
            {
                if (stdGridReportID->Cells[x][y] == "")
                    break;
                iReportIDData[ct] = static_cast<unsigned>(atoi(stdGridReportID->Cells[x][y].c_str()));
                ct++;
            }
            return static_cast<unsigned>(ct);
        }
    }
    return 0;
}
//------------------------------------------------------------------------------
bool THGem::IsEnableEvent(unsigned iDataID, unsigned iCeid)
{
    (void)iDataID;   // golden signature keeps iDataID but never reads it in this method's body
    for (int y = 1; y < strGrdCEID->RowCount; y++)
    {
        if (strGrdCEID->Cells[0][y] == iCeid)
        {
            if (strGrdCEID->Cells[1][y] == "1")
                return true;
        }
    }
    return false;
}
//------------------------------------------------------------------------------
bool THGem::CheckCeidExist(AnsiString Ceid)
{
    for (int y = 1; y < strGrdCEID->RowCount; y++)
    {
        if (strGrdCEID->Cells[0][y] == Ceid)
            return true;
    }
    return false;
}
//------------------------------------------------------------------------------
int THGem::GetReportIDType(AnsiString ReportID)
{
    for (int y = 1; y < stdGridReportID->RowCount; y++)
    {
        if (stdGridReportID->Cells[0][y] == ReportID)
        {
            if (stdGridReportID->Cells[1][y] == "0")
                return 0;
            else if (stdGridReportID->Cells[1][y] == "1")
                return 1;
            else
                return 2;
        }
    }
    return 3;
}
//------------------------------------------------------------------------------
bool THGem::CheckCeidAlreadyDefine(AnsiString Ceid)
{
    AnsiString ReportID;
    for (int y = 1; y < strGrdCEID->RowCount; y++)
    {
        if (strGrdCEID->Cells[0][y] == Ceid)
        {
            for (int x = 3; x < strGrdCEID->ColCount; x++)   // 2014/01/01 lee -- add S1F23
            {
                ReportID = strGrdCEID->Cells[x][y];
                if (ReportID == "")
                    continue;
                if (GetReportIDType(ReportID) == 0)
                    return true;
            }
        }
    }
    return false;
}
//------------------------------------------------------------------------------
bool THGem::CheckReportIDExist(AnsiString ReportID)
{
    for (int y = 1; y < stdGridReportID->RowCount; y++)
    {
        if (stdGridReportID->Cells[0][y] == ReportID)
            return true;
    }
    return false;
}
//------------------------------------------------------------------------------
void THGem::AddCeidReportID(AnsiString Ceid, AnsiString ReportID)
{
    for (int y = 1; y < strGrdCEID->RowCount; y++)
    {
        if (strGrdCEID->Cells[0][y] == Ceid)
        {
            for (int x = 3; x < strGrdCEID->ColCount; x++)   // 2014/01/01 lee
            {
                // golden: `strGrdCEID->Cells[x][y]=="" || strGrdCEID->Cells[x][y]==NULL`
                // -- the `==NULL` half is dead/redundant here (a grid cell is
                // always a real AnsiString value, never a null pointer;
                // AnsiString==NULL resolves via the const-char*-overload,
                // which treats a null pointer as ""), preserved verbatim for
                // source fidelity rather than simplified away.
                if (strGrdCEID->Cells[x][y] == "" || strGrdCEID->Cells[x][y] == NULL)
                {
                    strGrdCEID->Cells[x][y] = ReportID;
                    return;
                }
            }
        }
    }
}
//------------------------------------------------------------------------------
void THGem::DeleteAllHostReportIDOfCeid(unsigned ReportID)
{
    bool bSearch = true;
    TStringList *P;
    P = new TStringList;
    for (int y = 1; y < strGrdCEID->RowCount; y++)
    {
        bSearch = false;
        for (int x = 3; x < strGrdCEID->ColCount; x++)
        {
            if (strGrdCEID->Cells[x][y] == ReportID)
            {
                strGrdCEID->Cells[x][y] = "";
                bSearch = true;
            }
        }

        if (bSearch)
        {
            P->Clear();
            for (int x = 3; x < strGrdCEID->ColCount; x++)
            {
                if (strGrdCEID->Cells[x][y] != "")
                    P->Add(strGrdCEID->Cells[x][y]);
            }

            for (int x = 3; x < strGrdCEID->ColCount; x++)
                strGrdCEID->Cells[x][y] = "";

            for (int i = 0; i < P->Count; i++)
                strGrdCEID->Cells[3 + i][y] = P->Strings[i];
        }
    }
    delete P;
}
//------------------------------------------------------------------------------
void THGem::DeleteHostReportIDOfAssignCeid(AnsiString Ceid)
{
    AnsiString ReportID;
    for (int y = 1; y < strGrdCEID->RowCount; y++)
    {
        if (strGrdCEID->Cells[0][y] == Ceid)
        {
            for (int x = 3; x < strGrdCEID->ColCount; x++)
            {
                // golden reads ReportID here (uHGemEquipment.cpp:7945) then
                // immediately clears the cell without otherwise using the
                // local -- preserved verbatim (dead read, not removed).
                ReportID = strGrdCEID->Cells[x][y];
                (void)ReportID;
                strGrdCEID->Cells[x][y] = "";
            }
        }
    }
}
//------------------------------------------------------------------------------
void THGem::DeleteAllHostDefineCeid()
{
    for (int y = 1; y < strGrdCEID->RowCount; y++)
    {
        if (strGrdCEID->Cells[0][y] != "")
            DeleteHostReportIDOfAssignCeid(strGrdCEID->Cells[0][y]);
    }
}
//------------------------------------------------------------------------------
bool THGem::CheckReportAlreadyDefine(AnsiString ReportID)
{
    for (int y = 1; y < stdGridReportID->RowCount; y++)
        if (stdGridReportID->Cells[0][y] == ReportID)
            return true;
    return false;
}
//------------------------------------------------------------------------------
// golden uHGemEquipment.cpp:8034-8039 -- GATED STUB.
// Real body is:
//     InitLocalHead(2, 38, 0);
//     DataItemOut(1, HType.BINARY_TYPE, &ErrCode);
//     SendLocalData();
// i.e. sends an S2F38 (Enable/Disable Event Report Acknowledge) over the
// wire. InitLocalHead/DataItemOut/SendLocalData are THGem's SML wire-codec
// forwarding methods, explicitly deferred to a follow-on wave (this wave's
// scope is the StringGrid CEID/Report/Alarm family only -- see this file's
// own header note). No-op: does NOT send anything over clientGem/srvGem.
// Exists purely so EnableDisableEventReport's own (in-scope) control flow,
// which unconditionally calls this at its tail, remains translatable without
// silently dropping the call site.
void THGem::EnableDisableEventReportAcknowledgeError(unsigned char ErrCode)
{
    (void)ErrCode;
}
//------------------------------------------------------------------------------
void THGem::EnableDisableEventReport(bool CEED, int slen, unsigned *CEID)
{
    if (slen == 0)   // all
    {
        for (int y = 1; y < strGrdCEID->RowCount; y++)
        {
            if (strGrdCEID->Cells[0][y] != "")
            {
                if (CEED == true)
                    strGrdCEID->Cells[1][y] = "1";
                else
                    strGrdCEID->Cells[1][y] = "0";
            }
        }
    }
    else
    {
        for (int i = 0; i < slen; i++)
        {
            for (int y = 1; y < strGrdCEID->RowCount; y++)
            {
                if (strGrdCEID->Cells[0][y] == CEID[i])
                {
                    if (CEED == true)
                        strGrdCEID->Cells[1][y] = "1";
                    else
                        strGrdCEID->Cells[1][y] = "0";
                }
            }
        }
    }
    SaveEventReportData();
    EnableDisableEventReportAcknowledgeError(0x00);   // GATED, see stub above
}

//===========================================================================
//  Generic StringGrid <-> TStringList tab-format (de)serialization
//  (golden uHGemEquipment.cpp:8059-8195)
//===========================================================================
bool THGem::CheckCEIDExist(AnsiString sID)   // upper-case-CEID sibling (golden :8051)
{
    for (int y = 1; y < strGrdCEID->RowCount; y++)
        if (strGrdCEID->Cells[0][y] == sID)
            return true;
    return false;
}
//------------------------------------------------------------------------------
void THGem::CopyStringGridAsTabFormat(TStringGrid *strGrd, TStringList *memoPtr)
{
    int endx;
    AnsiString S;

    memoPtr->Clear();
    for (int y = 0; y < strGrd->RowCount; y++)
    {
        S = "";
        endx = 0;
        for (int x = (strGrd->ColCount - 1); x >= 0; x--)
        {
            if (strGrd->Cells[x][y] != "")
            {
                endx = x;
                break;
            }
        }
        // NOTE: even a fully-empty row (endx stays 0) still emits ONE
        // tab-terminated (empty) field below -- golden quirk, preserved
        // verbatim (confirmed against the real on-disk
        // D:\HT9045\SECS\SECS\SYSTEM\EventReport_CEID.def, whose 1024 unused
        // data rows are literally single-tab lines, not truly-empty lines).
        for (int x = 0; x <= endx; x++)
        {
            S += strGrd->Cells[x][y];
            S += "\t";
        }
        memoPtr->Add(S);
    }
}
//------------------------------------------------------------------------------
void THGem::PasteStringGridAsTabFormat(TStringGrid *strGrd, TStringList *memoPtr)
{
    int x, y, StartX, StartY;
    char str[8192], str2[256];
    bool flag;

    for (y = 0; y < strGrd->RowCount; y++)
        for (x = 0; x < strGrd->ColCount; x++)
            strGrd->Cells[x][y] = "";

    for (y = 0; y < memoPtr->Count; y++)
    {
        if (y >= strGrd->RowCount)
            break;
        // AI(W906-uHGemEquipment) 20260716: golden is `memoPtr->Strings[y].c_str()`;
        // vclcompat::TStringList's Strings[] proxy has no .c_str() (same
        // accommodation already established at mycylin.cpp's Strings[i]->
        // GetString(i) sites) -- GetString(y) returns the real AnsiString.
        strncpy(str, memoPtr->GetString(y).c_str(), sizeof(str));
        str[sizeof(str) - 1] = '\0';   // AI(W906-uHGemEquipment) 20260716: golden's bare
                                        // strncpy has no explicit terminator guard; added
                                        // defensively for a line >=8192 bytes (never hit by
                                        // this wave's real/test data, which are all far
                                        // shorter) -- does not change behavior for any
                                        // in-range input, only avoids reading past `str`'s
                                        // end in SplitStrByTabOnly on a pathological input.
        x = 0;
        do
        {
            flag = SplitStrByTabOnly(str, str2, 256);
            if (flag == false)
                break;
            StartX = x;
            StartY = y;

            strGrd->Cells[StartX][StartY] = str2;
            x++;
            if (x >= strGrd->ColCount)
                break;
        } while (1);
    }
}
//------------------------------------------------------------------------------
void THGem::ReadEventReportData()
{
    // AI(W906-uHGemEquipment) 20260716: golden hardcodes this absolute path
    // (uHGemEquipment.cpp:8202/8212) -- NOT derived from GemSystemPath,
    // unlike ReadAlamData/WriteAlamData above. Preserved verbatim: the real
    // production files exist at exactly this path on the reference dev
    // machine (D:\HT9045\SECS\SECS\SYSTEM\EventReport_CEID.def /
    // EventReport_ReportID.def) and this method only ever READS them
    // (LoadFromFile) -- never writes -- so calling it is safe/non-destructive.
    // SaveEventReportData below WRITES to this same path family and must
    // never be exercised against the real path (see that method's own note).
    TStringList *memoPtr;
    AnsiString Filename;

    memoPtr = new TStringList;
    Filename = "D:\\HT9045\\SECS\\SECS\\SYSTEM\\EventReport_CEID.def";
    if (FileExists(Filename) == false)
    {
        delete memoPtr;
        return;
    }

    memoPtr->LoadFromFile(Filename);
    PasteStringGridAsTabFormat(strGrdCEID, memoPtr);

    Filename = "D:\\HT9045\\SECS\\SECS\\SYSTEM\\EventReport_ReportID.def";
    if (FileExists(Filename) == false)
    {
        delete memoPtr;
        return;
    }

    memoPtr->LoadFromFile(Filename);
    PasteStringGridAsTabFormat(stdGridReportID, memoPtr);

    delete memoPtr;
}
//------------------------------------------------------------------------------
void THGem::SaveEventReportData()
{
    // AI(W906-uHGemEquipment) 20260716: DANGEROUS IF EXECUTED AS-IS ON THIS
    // DEV MACHINE -- golden hardcodes the SAME absolute path family that
    // ReadEventReportData reads from (D-colon HT9045 SECS SECS SYSTEM
    // EventReport_CEID.def / EventReport_ReportID.def, backslash-separated),
    // and those files
    // genuinely exist as PRODUCTION data at that path here. This method
    // WRITES (SaveToFile) to that literal path -- it must NEVER be called
    // by this wave's own test harness (would silently overwrite real
    // production SECS/GEM config, which is outside this migration's write
    // boundary: D:\HT9045 root, not the D:\HT9045\HT9011UC_Cpp_V3.33.906.0
    // working tree). Translated faithfully (hardcoded path preserved,
    // matching golden uHGemEquipment.cpp:8650-8665 exactly) but verified by
    // code inspection/citation only -- see tests/test_uHGemEquipment.cpp's
    // own header note for how CopyStringGridAsTabFormat (the part of this
    // method's logic that IS actually exercised) is tested instead.
    TStringList *memoPtr;
    AnsiString Filename;

    memoPtr = new TStringList;
    CopyStringGridAsTabFormat(strGrdCEID, memoPtr);
    Filename = "D:\\HT9045\\SECS\\SECS\\SYSTEM\\EventReport_CEID.def";
    memoPtr->SaveToFile(Filename);

    CopyStringGridAsTabFormat(stdGridReportID, memoPtr);
    Filename = "D:\\HT9045\\SECS\\SECS\\SYSTEM\\EventReport_ReportID.def";
    memoPtr->SaveToFile(Filename);

    delete memoPtr;
}

//---------------------------------------------------------------------------
//  SplitStrByTabOnly (golden uHGemEquipment.cpp:8086-8161)
//  File-scope helper, NOT a THGem method (matches golden exactly -- see
//  uHGemEquipment.h's own note on why this is declared there for testability
//  even though golden's header never declares it).
//
//  Parses ONE tab-delimited field out of `str` (in place, shifting the
//  consumed portion off the front) into `dest`, tolerating a leading tab,
//  CR, and NUL/CR/LF terminators.
//
//  BEHAVIOR (verified by hand-trace, e.g. against "a\t\tb" -> "a", "", "b"):
//  if `str` STARTS with a tab, that represents an EMPTY field at the current
//  position (either the very first field of a line beginning with a tab, or
//  -- more commonly -- the trailing tab an earlier call already left behind
//  after consuming the previous field, re-entering this SAME top branch on
//  the next call when two delimiters are adjacent). This branch writes
//  `dest=""` (the empty field), shifts `str` left by one to drop the
//  consumed tab, and returns true. The non-tab (`else`) branch below extracts
//  one field into `dest`, then -- as its OWN last step -- performs the same
//  kind of one-tab left-shift on whatever follows, so a normal (non-empty)
//  field is fully consumed, INCLUDING its trailing delimiter, within a
//  single call (the next call's `str` starts clean, not with a leftover
//  tab) -- confirmed by hand-trace against "hello\tworld".
//---------------------------------------------------------------------------
bool SplitStrByTabOnly(char *str, char *dest, int Max)
{
    char Buffer[10240];
    int ct1 = 0, ct2 = 0;

    if (str[ct1] == '\t')
    {
        strcpy(dest, "");
        for (;;)
        {
            Buffer[ct1] = str[ct1 + 1];
            if (Buffer[ct1] == 0)
            {
                strcpy(str, Buffer);
                return true;
            }
            ct1++;
        }
    }
    else
    {
        while (1)   // find first character
        {
            if (str[ct1] == '\x0' || str[ct1] == '\r' || str[ct1] == '\n')
                return false;
            if ((str[ct1] != '\t' && str[ct1] != '\r'))
                break;
            ct1++;
        }
        while (1)
        {
            dest[ct2] = str[ct1];
            ct2++;
            ct1++;
            dest[ct2] = '\x0';
            if ((ct2 + 1) >= Max)
                break;

            if (str[ct1] != '\t' && str[ct1] != '\0' && str[ct1] != '\r')
            {
            }
            else
            {
                break;
            }
        }
        ct2 = 0;
        while (1)
        {
            Buffer[ct2] = str[ct1];
            if (str[ct1] == '\x0')
                break;
            ct1++;
            ct2++;
            Buffer[ct2] = '\x0';
            if (ct2 >= 10240)
                break;
        }
        strcpy(str, Buffer);
        ct1 = 0;
        if (str[0] == '\t')
        {
            for (;;)
            {
                Buffer[ct1] = str[ct1 + 1];
                if (Buffer[ct1] == 0)
                {
                    strcpy(str, Buffer);
                    return true;
                }
                ct1++;
            }
        }
    }
    return true;
}

//===========================================================================
//  TCP/IP connection lifecycle (W906-uHGemEquipment-ConnLifecycle wave)
//  (golden uHGemEquipment.cpp:315-348, 392-439, 2100-2142, 3382-3490,
//   3604-3645, 4988-5008, 5146-5159, 5548-5644, 6812-6910)
//===========================================================================

namespace {

// ---------------------------------------------------------------------------
// LogClientSocketExceptionError_ -- stand-in for golden's DEFERRED
// LogClientSocketExceptionError(Sender, "THGem") (Public/WinSocketErrorCode.h
// explicitly documents this as DEFERRED -- couples to VCL
// TClientSocket->Name/Address/Port + MyDBIProcess). Golden's one call site in
// THIS file is srvGemClientError's catch(...) branch (uHGemEquipment.cpp:6870).
//
// Per this project's established per-TU handling of the identical gap (grepped
// the tree, as directed): Automation/automation.cpp:67-76 routes to a real log
// call (RecordProcess) rather than silently dropping the diagnostic;
// Interface/TesterTCP_Socket.cpp:104-119 instead makes it a true no-op (its own
// call sites are inside an already-fully-handled catch). This TU follows the
// automation.cpp spirit (log something real, don't just drop it) but does NOT
// reach for RecordProcess: RecordProcess's only current definition
// (common.cpp) lives in the ht9045_core library, which uHGemEquipment.cpp does
// not link and, per this very file's own top-of-file note, deliberately avoids
// pulling in (extract-calc-core discipline -- MachineDefine.h/cMydef.h/
// database.h/etc. are intentionally NOT included here either). Instead this
// routes to SaveSECSGEMErrToLog (translated below, golden :422-439) -- a real,
// already in-scope logging sink for exactly this situation -- with a
// synthesized message, per this wave's own brief which explicitly offered that
// as an acceptable alternative.
// ---------------------------------------------------------------------------
void LogClientSocketExceptionError_(THGem *inst, TObject *Sender, const AnsiString &Context)
{
    (void)Sender;
    inst->SaveSECSGEMErrToLog("ClientSocketException: " + Context);
}

// ---------------------------------------------------------------------------
// Gated_MyForceDirectories -- golden common.h:262, gated `#if 0` inside
// common.h (not in this front's allowed file list). Same TU-local
// re-implementation TECHNIQUE already established by Interface/TesterTCP.cpp's
// own Gated_MyForceDirectories (each gated dependency in this project is
// intentionally reproduced per-TU, not shared via a header -- see
// docs/KNOWLEDGE.md) -- but SIMPLIFIED to match THIS file's one real call
// site's actual shape (SaveSECSGEMErrToLog's `asPath`, below): always a bare,
// guaranteed-non-empty directory path, never a path with a trailing filename
// component -- the SAME simplification Automation/automation.cpp's own
// W906Auto_MyForceDirectories already made for the identical reason (see that
// file's own note). Diagnostic branch routes to MyDBIProcess (already
// extern-declared above in this TU, established by the pre-existing
// ReadAlamData) rather than RecordProcess/ShowMyMessage (canary_support.h) --
// same minimal-dependency reasoning as LogClientSocketExceptionError_ above.
// ---------------------------------------------------------------------------
void Gated_MyForceDirectories(AnsiString Directory)
{
    if (Directory == "")
    {
        MyDBIProcess("Directory value is NULL!", "SaveSECSGEMErrToLog");
        return;
    }
    if (DirectoryExists(Directory) == false)
        ForceDirectories(Directory);
}

// AI(W906-uHGemEquipment-BucketB) 20260717: AMENDMENT to
// LogClientSocketExceptionError_'s own comment above -- this Bucket B wave
// DOES now link ht9045_core (see root CMakeLists.txt), so "which
// uHGemEquipment.cpp does not link" is no longer accurate for ht9045_core
// itself. The underlying reasoning still holds, though: RecordProcess's real
// definitions live in ht9045_sm (aHotPlateSubstrate.cpp/acatchtray_shims.cpp/
// acarry_shims.cpp/canary_support.cpp -- confirmed per KYECFTP/
// FTPClient_Transfer.cpp's own citation of the same functions), NOT
// ht9045_core -- linking ht9045_core (for common.h's ReadWriteIni/
// ReadIniData/WriteIniData, this wave's own real need) does not pull
// RecordProcess in, so routing to SaveSECSGEMErrToLog instead of
// RecordProcess is still the right call, for the still-valid reason.

// ---------------------------------------------------------------------------
// Gated_ShowMessage -- golden Dialogs.hpp `void ShowMessage(const AnsiString&)`,
// the REAL VCL modal dialog (distinct from this project's own ShowMyMessage
// wrapper in canary_support.h/mymessbox.h -- no stand-in for the real
// ShowMessage global exists anywhere in the tree today). GATED per the
// project's own established convention -- EXACT same idiom, same reasoning,
// as SecsSvEcRegistration.cpp's own `Gated_ShowMessage` (see that file for
// the precedent this mirrors). Golden's one call site in THIS file is
// ManualCreatergRoleClick (uHGemEquipment.cpp:6919) -- a "need to restart the
// program" advisory dialog after the operator flips rgRole; a no-op here
// just means that advisory is silently skipped (the REAL state change,
// bUseClientSocket/edtIP->Enabled, still happens exactly as golden does).
// ---------------------------------------------------------------------------
void Gated_ShowMessage(const AnsiString & /*S*/)
{
    // TODO(W7-UI): wire to a real modal ShowMessage dialog.
}

// ---------------------------------------------------------------------------
// HTimer -- AI(W906-uHGemEquipment-BucketC) 20260717: UPGRADED to a REAL
// elapsed-time timer, replacing the always-fires stub that used to live here
// (see the Bucket-B FLAGGED LIMITATION this note replaces, and the identical
// flag repeated at DoUpdateStatus's own KYEC branch below and at
// Timer1Timer's SECSGEM_DoSeparateWait use, both now UPDATED to say so).
//
// This is a faithful TU-local port of the canonical golden HTimer
// (D:\HT9045\elec\Component\htimer.h/.cpp -- the component-library HTimer
// golden's own `#include "HTimer.h"` resolves to; the 906 snapshot tree does
// not carry the file itself, only the reference). Ported methods: ctor
// (calls Clear()), Clear(), Set(int, 0.1s units), SetSec(double, seconds),
// SetMS(int, milliseconds), On(), SetSecAndOn(double), Off(). Deliberately
// SKIPPED (unused by this TU's two consumers, SECSGEM_DoSeparate/
// SECSGEM_DoSeparateWait below -- both only ever call SetSecAndOn/Off):
// SetTimer(int,int)/Pause()/ReStart()/ResetMSAndOn(int), and the global
// HTimerList/ThMutex(HMutex) process-wide registry + ClearAllTimer/
// PauseAllTimer/ReStartAllTimer free functions (that registry exists so a
// running program can mass-pause/clear every live HTimer at once -- nothing
// in this TU's scope needs that; the two consumers below are polled directly).
//
// Off()'s semantics are ported EXACTLY (htimer.cpp:129-173), including its
// documented oddities, verbatim: Paused is always false here (Pause() isn't
// ported, so it can never become true) but the check is kept for textual
// fidelity; ulStartTicks==0 (never armed) -> false; iTimeLen<=0 (armed with
// a zero/negative duration) -> true unconditionally ("jou 2012-01-04": guards
// against a mis-entered duration hanging the caller forever); otherwise
// expired iff now >= start+len (+pauseLen, always 0 here since Pause/ReStart
// are not ported) -- WITH the DWORD-wraparound branch preserved verbatim
// (including its own asymmetric `ulNowTicksOver < ulStartTicks` re-check,
// copied as-is, not simplified); expiry sets InUsed=false and the method
// KEEPS returning true on every subsequent re-poll (InUsed is otherwise
// unread anywhere in the real htimer.cpp golden -- it exists for the
// registry's own bookkeeping, which this port does not carry).
//
// TU-LOCAL BY DESIGN (anonymous namespace): atester_shims.h:292 keeps its
// OWN separate always-fires `struct HTimer` stub, unchanged -- promoting
// THAT one to a real elapsed-time timer would alter gated atester state
// machines that are genuinely out of this wave's scope. ODR is not at risk:
// an anonymous namespace gives this TU's `HTimer` INTERNAL linkage, a
// distinct type per translation unit -- the exact same pattern
// acatchtray.cpp's own `typedef TQPF_Timer HTimer` already relies on
// elsewhere in this tree. FLAG (risk, not a defect): a future wave should
// promote a single real vclcompat/HTimer shim and migrate the (now three)
// TU-local variants deliberately, rather than this being decided ad hoc,
// per-TU, forever.
// ---------------------------------------------------------------------------
struct HTimer
{
    DWORD ulStartTicks;
    DWORD ulPauseTicks;
    int   iTimeLen;
    int   iPauseLen;
    bool  Paused;
    bool  InUsed;
    int   iTimerID;

    HTimer() { Clear(); }

    void Clear()
    {
        ulStartTicks = 0;
        ulPauseTicks = 0;
        iTimeLen = 0;
        InUsed = false;
        Paused = false;
        iPauseLen = 0;
    }

    void Set(int iTime) { iTimeLen = iTime * 100; }              // htimer.cpp:67-70 (0.1s units)
    void SetSec(double iTime) { iTimeLen = static_cast<int>(iTime * 1000.0); }   // htimer.cpp:72-75
    void SetMS(int iTime) { iTimeLen = iTime; }                  // htimer.cpp:113-116

    void On() { ulStartTicks = ::GetTickCount(); InUsed = true; }   // htimer.cpp:120-125

    void SetSecAndOn(double iTime)   // htimer.cpp:77-82
    {
        Clear();
        SetSec(iTime);
        On();
    }

    bool Off()   // htimer.cpp:129-173, verbatim (incl. the DWORD-wraparound branch)
    {
        if (Paused)
            return false;

        if (ulStartTicks == 0)
            return false;

        if (iTimeLen <= 0)
        {
            // jou 2012-01-04: guards against a mis-entered zero/negative
            // duration hanging the caller forever.
            return true;
        }

        DWORD ulLimited = ulStartTicks + iTimeLen + iPauseLen;
        DWORD ulNowTicks = ::GetTickCount();
        DWORD ulNowTicksOver = 0;
        if (ulLimited < ulStartTicks)   // DWORD wraparound
        {
            ulLimited = 0xFFFFFFFF - ulStartTicks + iTimeLen + iPauseLen;
            ulNowTicksOver = 0xFFFFFFFF - ulStartTicks + ulNowTicks;

            if (ulNowTicksOver < ulStartTicks &&
                ulNowTicksOver > ulLimited)
            {
                InUsed = false;
                return true;
            }
        }
        else
        {
            if (ulNowTicks >= (ulStartTicks + iTimeLen))
            {
                InUsed = false;
                return true;
            }
        }
        return false;
    }
};

// ---------------------------------------------------------------------------
// ScopedAcquire -- AI(W906-uHGemEquipment-BucketC) 20260717: D8, TU-local RAII
// stand-in for golden's `try { cs->Acquire(); ... } __finally { cs->Release(); }`
// pattern (__finally is a BCB6-only extension, not standard C++). One struct
// covers BOTH TCriticalSection* and TFixedCriticalSection* (the latter
// derives from the former) call sites -- e.g.
// clientGemRead/DoProcessSFNoResponse/CheckSFCodeResponse/SendLocalDataFrom.
// Every use site below keeps the golden Acquire()/Release() lines as
// comments alongside the golden citation, per this project's convention.
// ---------------------------------------------------------------------------
struct ScopedAcquire
{
    TCriticalSection *cs;
    explicit ScopedAcquire(TCriticalSection *c) : cs(c) { cs->Acquire(); }
    ~ScopedAcquire() { cs->Release(); }
    ScopedAcquire(const ScopedAcquire&) = delete;
    ScopedAcquire& operator=(const ScopedAcquire&) = delete;
};

} // anonymous namespace

// ---------------------------------------------------------------------------
// MyDBIProcess (3-arg overload) -- AI(W906-FastcallFix) 20260720: moved OUT of
// the anonymous namespace above (was here as of W906-uHGemEquipment-BucketC
// 20260717) and given __fastcall, so this becomes the tree's ONE externally
// linkable, fastcall-decorated definition of this overload -- matching
// golden's role for it (cMyDB.cpp:788-789, declared cMyDB.h:20; not yet
// translated in this port, so this stays the interim home until that wave
// lands). Both `database.cpp:75` and `uHGemClass.cpp:310` forward-declare
// this exact signature `extern`+`__fastcall` and call it (LoadIoData/
// LoadMotData's catch blocks; ~HTGem), so THIS TU must supply the definition
// they link against. The prior anonymous-namespace body compiled and ran
// fine for calls made FROM this file, but had internal linkage and no
// __fastcall -- a differently-mangled symbol than the fastcall-decorated one
// those two TUs reference -- leaving them with an unresolved external at
// link time (2026-07-20 audit, AUDIT_fastcall_tree.md finding 1). Forwarding
// behavior is unchanged: still routes to the file-scope 2-arg no-op
// (aHotPlateSubstrate.cpp:703) for the actual side effect; S3 remains
// call-shape-fidelity only, preserving SendLocalDataFrom's golden 3-arg call
// sites VERBATIM (this file's own `MyDBIProcess("Exception",
// "THGem::SendLocalData...", SFCode)` calls, below). No default argument on
// S3 here -- this TU's OTHER (2-arg) calls to MyDBIProcess must keep
// resolving to the file-scope 2-arg extern above (line 80) unambiguously;
// giving this overload a default would make it viable for those same 2-arg
// call sites too and turn every one of them into an ambiguous-call compile
// error. (Golden's own default lives only on the database.cpp/uHGemClass.cpp
// forward declarations, matching where cMyDB.h:20 puts it too -- unaffected
// by this TU's own choice here.)
// ---------------------------------------------------------------------------
void __fastcall MyDBIProcess(AnsiString S1, AnsiString S2, AnsiString S3)
{
    // AI(W906-FastcallFix) 20260720: now that this function lives at file
    // scope instead of inside the anonymous namespace above, the RECURSION
    // risk the previous `::`-qualification comment warned about no longer
    // applies in the same way: a bare unqualified `MyDBIProcess(S1, S2)` here
    // would ALSO resolve correctly on its own, because overload resolution at
    // file scope only ever finds the 2-arg extern (line 80, no default) as
    // VIABLE for a 2-argument call -- this function's own 3-arg overload has
    // no default for S3 (see the reasoning above) and so is never a
    // candidate when only 2 arguments are supplied; there is no sibling
    // 3-arg overload at this same scope to be mistaken for it either, unlike
    // the anonymous namespace's self-contained overload set before the move.
    // The `::` is kept anyway, now purely as defensive documentation of
    // intent: it makes it unmistakable -- even to a future edit that adds a
    // default to this S3 parameter -- that this call must always reach the
    // file-scope 2-arg sink and never (accidentally, via a later change)
    // itself.
    ::MyDBIProcess(S1, S2);
    (void)S3;
}

// AI(W906-uHGemEquipment-BucketB) 20260717: golden uHGemEquipment.cpp:24/4746
// -- both are genuine FILE-SCOPE globals in golden itself (NOT THGem
// members), shared by DoUpdateStatus and Timer1Timer. This wave's own brief
// suggested making these THGem members instead; verified against golden and
// kept as plain globals here, matching golden exactly.
bool bSECSGEM_DoSeparate = false;   // golden uHGemEquipment.cpp:24
HTimer SECSGEM_DoSeparate;          // golden uHGemEquipment.cpp:4746

// AI(W906-uHGemEquipment-BucketC) 20260717: golden uHGemEquipment.cpp:5175
// `HTimer SECSGEM_DoSeparateWait;` -- another genuine file-scope global
// (immediately above golden's own Timer1Timer definition), Ifor 20180913
// (Steven)'s own comment gloss: "add a 5-second wait after sending the KYEC
// forced-disconnect SECS GEM command before allowing the upper system to
// reconnect." Declared here, next to its sibling above, per this file's own
// established convention for these two globals.
HTimer SECSGEM_DoSeparateWait;      // golden uHGemEquipment.cpp:5175

// AI(W906-uHGemEquipment-BucketB) 20260717: golden's own global singleton
// pointer (`extern PACKAGE THGem *HGem;`, uHGemEquipment.h's tail
// declaration) -- see this header's own comment (near its `extern THGem
// *HGem;`) for the ProcessShow/DoUpdateStatus call sites that dereference
// this instead of an implicit `this->`. NULL by default (C++ static-storage
// zero-init); a caller/test must set `HGem = &instance;` before exercising
// either of those two call sites.
THGem *HGem = NULL;

//---------------------------------------------------------------------------
// V 1.0
// 設定 Time String Format and get pc information (golden uHGemEquipment.cpp:315-348)
//---------------------------------------------------------------------------
void THGem::GetTimeInfo()
{
    TDateTime dtPresent;

    dtPresent = Now();
    DecodeDate(dtPresent, SystemYear, SystemMonth, SystemDate);
    DecodeTime(dtPresent, SystemHour, SystemMin, SystemSec, SystemMSec);
    TimeString.sprintf("%04d-%02d-%02d %02d:%02d:%02d.%03d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, SystemMSec);

    if (iTimeFormat == 1)                                                       // 16 byte
        GemClock.sprintf("%04d%02d%02d%02d%02d%02d%02d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, SystemMSec / 10);
    else if (iTimeFormat == 2)                                                  // 14 byte
        GemClock.sprintf("%04d%02d%02d%02d%02d%02d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
    else if (iTimeFormat == 3)                                                  // 19 byte
        GemClock.sprintf("%04d-%02d-%02dT%02d:%02d:%02d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
    else
        GemClock.sprintf("%02d%02d%02d%02d%02d%02d", SystemYear % 100, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);

    // AI(W906-uHGemEquipment-ConnLifecycle) 20260717: GATED STUB -- golden's
    // tail (uHGemEquipment.cpp:332-347) polls disk-free-space / global-memory-
    // status every 5-10 minutes via GetDiskFreeSpaceMB(...)/
    // GetGlobalMemoryStatusKB(...). Neither function exists anywhere in this
    // codebase yet (grepped), and the LastGetDiskInfoMin/LastGetMemoryStatus/
    // Disk_C_TotalSpaceMB/.../ulMemoryLoad/... members that tail reads/writes
    // are not part of THIS wave's scope either. Same gated-stub precedent as
    // EnableDisableEventReportAcknowledgeError above (this file's other GATED
    // STUB): omitted outright (no member additions, no calls) rather than
    // half-modeled -- exists purely so this comment documents WHY the tail is
    // missing, so a future wave doesn't mistake the omission for an oversight.
}

//---------------------------------------------------------------------------
// V 1.0
// 將 Data 從設計指定的 TMemo 內做 show 出 (golden uHGemEquipment.cpp:392-396)
//---------------------------------------------------------------------------
void __fastcall THGem::StringOut(AnsiString S)
{
    WaitShowString->Add(S);
    LogDataString->Add(S);
}
//---------------------------------------------------------------------------
// V 1.0
// 將 Data 從設計指定的 TMemo 內以 Binary show 出 (golden uHGemEquipment.cpp:401-404)
//---------------------------------------------------------------------------
void __fastcall THGem::StringBinaryOut(AnsiString S)
{
    // AI(W906-uHGemEquipment-ConnLifecycle) 20260717: golden's ENTIRE body is
    // a commented-out `//LogDataString->Add(S);` -- a true no-op in golden
    // itself, not a translation gap. Preserved verbatim.
    (void)S;
}
//---------------------------------------------------------------------------
// V 1.0
// 儲存 SECS LOG (golden uHGemEquipment.cpp:422-439)
//---------------------------------------------------------------------------
void __fastcall THGem::SaveSECSGEMErrToLog(AnsiString asSaveStr)
{
    TDateTime tdSaveTime = Now();
    AnsiString asPath, asFN;
    FILE *P;

    // AI(W906-uHGemEquipment-ConnLifecycle) 20260717: golden's
    // `DateSeparator='_';` (a global BCB6 SysUtils setting) is OMITTED --
    // vclcompat has no such global, and hand-tracing golden's own 3
    // FormatString calls below confirms it is behaviorally INERT for this
    // method anyway: none of "yyyy" / "mm_dd" / "hh" contain the '/' token
    // DateSeparator would ever substitute into. Nothing to port, not a gap.
    //
    // Golden calls `tdSaveTime.FormatString(fmt)` (BCB6 TDateTime member-call
    // syntax) -- vclcompat::TDateTime has no such member (see
    // vclcompat/TDateTime.h). Adapted to the free-function
    // FormatDateTime(fmt, dt) call style per this wave's own brief -- same
    // token grammar, same output, syntax-only adaptation.
    asPath.sprintf("D:\\SECS_GEM_LOGS\\%s\\%s", FormatDateTime("yyyy", tdSaveTime), FormatDateTime("mm_dd", tdSaveTime));
    // golden: MyForceDirectories(asPath);  -- TU-local stand-in, see above.
    Gated_MyForceDirectories(asPath);
    asFN.sprintf("%s\\SECSGEM_ErrLog_%s.txt", asPath, FormatDateTime("hh", tdSaveTime));

    P = fopen(asFN.c_str(), "a+");
    if (P != NULL)
    {
        fputs(asSaveStr.c_str(), P);
        fputs("\n", P);
        fclose(P);
    }
}

//---------------------------------------------------------------------------
// V1.0 (golden uHGemEquipment.cpp:2100-2104)
//---------------------------------------------------------------------------
void __fastcall THGem::clientGemConnect(TObject *Sender, TCustomWinSocket *Socket)
{
    (void)Sender;
    (void)Socket;
    StringOut("Connect");
}
//---------------------------------------------------------------------------
// V1.0 (golden uHGemEquipment.cpp:2108-2116)
//---------------------------------------------------------------------------
void __fastcall THGem::clientGemDisconnect(TObject *Sender, TCustomWinSocket *Socket)
{
    (void)Sender;
    (void)Socket;
    GetTimeInfo();
    StringOut("disconnect  " + TimeString);   // JerryYang 20190411 : 斷線時要記錄時間
    if (bConnect == true)
        bAutoConnect = true;
    bConnect = false;
}
//---------------------------------------------------------------------------
// V1.0 (golden uHGemEquipment.cpp:2120-2134)
//---------------------------------------------------------------------------
void __fastcall THGem::clientGemError(TObject *Sender, TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
    (void)Sender;
    (void)Socket;
    (void)ErrorEvent;
    clientGem->Active = false;
    ErrorCode = 0;
    bTCPIP_Error = true;
    // AI(W906-uHGemEquipment-ConnLifecycle) 20260717: golden's own
    // `try{clientGem->Close();}catch(...){LogClientSocketExceptionError(...);}`
    // is ITSELF commented out in golden (uHGemEquipment.cpp:2126-2133) -- dead
    // code in golden, preserved as dead (not translated to a live call, and
    // NOT "resurrected").
}
//---------------------------------------------------------------------------
// V1.0 (golden uHGemEquipment.cpp:2138-2142)
//---------------------------------------------------------------------------
void __fastcall THGem::clientGemConnecting(TObject *Sender, TCustomWinSocket *Socket)
{
    (void)Sender;
    (void)Socket;
    StringOut("connecting");
}

//=============================================================================
// =            對 Host 的要求做 Polling 處理所用到的程式區塊                  =
//=============================================================================
//------------------------------------------------------------------------------
// V 1.0
// 對 Socket 做 Enable 處理 (golden uHGemEquipment.cpp:3382-3490)
//------------------------------------------------------------------------------
bool THGem::DoOpenCommuncation()
{
    int &Task = iOpenCommuncationTask;

    if (bUseClientSocket == true)
    {
        switch (Task)
        {
            case 1:
                if (bOpenCommuncation == true)
                    Task = 100;
                break;
            case 100:
                if (clientGem->Address == "" || clientGem->Port == 0)
                    return false;

                // 16.04.01.01s Roy Change
                try
                {
                    clientGem->Active = true;
                }
                catch (...)
                {
                    MyDBIProcess("Exception", "THGem::DoOpenCommuncation");
                }
                // 16.04.01.01e

                if (clientGem->Active == true)
                {
                    Task = 200;
                }
                else
                {
                    DelayOpenCommuncation.TimerSetSecAndOn(60);
                    Task = 150;
                }
                break;
            case 150:
                if (DelayOpenCommuncation.TimerOff())
                    Task = 100;
                break;
            case 200:
                if (bTCPIP_Error == true)
                {
                    bTCPIP_Error = false;
                    Task = 100;
                }
                else
                {
                    Task = 1;
                    return true;
                }
                break;
        }
        return false;
    }
    else
    {
        switch (Task)
        {
            case 1:
                if (bOpenCommuncation == true)
                {
                    // AI(W906-uHGemEquipment-ConnLifecycle) 20260717: golden's
                    // own `//srvGem->Close();` right here is commented out in
                    // golden itself ("斷線重連的秘密 2013/07/18 lee -- 把 Servo
                    // Socket close 再 open 看看 -- 結果會當掉!!!!!!!!!!!", i.e.
                    // "the secret of reconnect-after-disconnect ... closing
                    // Servo Socket then reopening it -- turns out it WILL
                    // HANG!!!"). Preserved as dead code, NOT resurrected -- see
                    // srvGemClientDisconnect's own comment below for the fuller
                    // quote and why this matters there too.
                    DelayOpenCommuncation.TimerSetSecAndOn(0.5);
                    Task = 100;
                }
                break;
            case 100:
                if (DelayOpenCommuncation.TimerOff() == false)
                    break;

                if (srvGem->Port == 0)
                    return false;

                // AI(W906-uHGemEquipment-ConnLifecycle) 20260717:
                // SAFETY-CRITICAL, preserved verbatim (golden :3458-3459,
                // "Eliot 2012_1108") -- Open() is called ONLY when NOT already
                // Active. NEVER blind-close-then-reopen srvGem here (or
                // anywhere else) -- see srvGemClientDisconnect's own comment
                // for the golden warning this guards against.
                if (srvGem->Active == false)
                    srvGem->Open();

                if (srvGem->Active == true)
                {
                    Task = 200;
                }
                else
                {
                    DelayOpenCommuncation.TimerSetSecAndOn(2);
                    Task = 150;
                }
                break;
            case 150:
                if (DelayOpenCommuncation.TimerOff())
                    Task = 100;
                break;
            case 200:
                if (bTCPIP_Error == true)
                {
                    bTCPIP_Error = false;
                    Task = 100;
                }
                else if (srvGem->Socket->ActiveConnections != 0)
                {
                    Task = 1;
                    return true;
                }
                break;
        }
        return false;
    }
}
//------------------------------------------------------------------------------
// V 1.0
// 與 remote 建立連線後的 Online[online local or remote] or offline 的要求送發
// (golden uHGemEquipment.cpp:3604-3621)
//------------------------------------------------------------------------------
void THGem::OnlineLocalOrRemote()
{
    bS1F2_OnLineData = false;

    // AI(W906-uHGemEquipment-ConnLifecycle) 20260717: below this point,
    // golden's own if/else is ENTIRELY DEAD -- both the true and false
    // branches contain ONLY a commented-out EventReport(...) call, annotated
    // by golden's own author ("JerryYang 20230204: SECS/GEM GControl State
    // 轉換的處理，這邊 Event report" -- roughly, "SECS/GEM GControl-State-
    // transition handling; the Event Report used to fire here"). This is a
    // genuine golden design choice (EventReport deliberately disabled here),
    // not a translation gap -- translated as a genuinely inert branch, NOT
    // "restored" to call EventReport (which is, besides, out of this wave's
    // own scope).
    if (bOnLineLocal == true)
    {
        // golden: //EventReport(1, 92);  -- dead in golden itself.
    }
    else
    {
        // golden: //EventReport(1, 93);  -- dead in golden itself.
    }
}
//------------------------------------------------------------------------------
// V 1.0
// 與 remote 建立連線後的 Online[online local or remote] or offline 的要求送發
// (golden uHGemEquipment.cpp:3626-3645)
//------------------------------------------------------------------------------
bool THGem::DoOnLine()
{
    int &Task = iStartOnLineTask;
    switch (Task)
    {
        case 1:
            Task = 200;
            break;
        case 100:
            if (bS1F2_OnLineData)
                Task = 200;
            break;
        case 200:
            OnlineLocalOrRemote();
            bOnLine = true;
            bStartOnLine = false;
            return true;
    }
    return false;
}

//2013/11/20  lee start
//------------------------------------------------------------------------------
// (golden uHGemEquipment.cpp:4988-5008)
//------------------------------------------------------------------------------
void __fastcall THGem::ClearDefaultEvenReport()
{
    unsigned iReportID;
    bool bSearchOK;
    do
    {
        bSearchOK = false;
        for (int y = 1; y < stdGridReportID->RowCount; y++)
        {
            if (stdGridReportID->Cells[1][y] == "1")
            {
                iReportID = static_cast<unsigned>(atoi(stdGridReportID->Cells[0][y].c_str()));
                DeleteReportID(iReportID, 1);
                DeleteReportIDOfCeid(iReportID);
                bSearchOK = true;
                break;
            }
        }
    } while (bSearchOK == true);
}
//2013/11/20  lee end

//=============================================================================
// =   V 1.0                                                                   =
// =   對 Host 的要求做 Polling 處理  (golden uHGemEquipment.cpp:5146-5159)     =
// =                                                                           =
//=============================================================================
bool THGem::CheckSocketActiveFalse()
{
    if (bUseClientSocket == true)
    {
        if (clientGem->Active == false)
            return true;
    }
    else
    {
        if (srvGem->Active == false)
            return true;
    }
    return false;
}

//==============================================================================
// V 1.0
// for Application call for disable TCP/IP port (golden uHGemEquipment.cpp:5548-5552)
//==============================================================================
void THGem::CloseCommuncation()
{
    bCloseCommuncation = true;
    clientGem->Active = false;
}
//==============================================================================
// V 1.0
// for Application call for connect with remote (golden uHGemEquipment.cpp:5557-5563)
//==============================================================================
void THGem::Connect()
{
    if (bConnect == true)
        return;
    bStartConnect = true;
    bAutoConnect = true;
}
//==============================================================================
// V 1.0 (golden uHGemEquipment.cpp:5567-5572)
//==============================================================================
void THGem::DisConnect()
{
    bConnect = false;
    bStartConnect = false;
    bAutoConnect = false;
}
//==============================================================================
// V 1.0 (golden uHGemEquipment.cpp:5576-5579)
//==============================================================================
bool THGem::IsConnect()
{
    return bConnect;
}
//==============================================================================
// V 1.0 (golden uHGemEquipment.cpp:5583-5586)
//==============================================================================
void THGem::SetEstablishCommunicationsTryCount(int ct)
{
    iEstablishCommunicationsTryCount = ct;
}
//==============================================================================
// V 1.0
// Mode=true   OnLineLocal
// Mode=false  OnLineRemote (golden uHGemEquipment.cpp:5592-5598)
//==============================================================================
void THGem::OnLine(bool Mode)
{
    bOnLineLocal = Mode;
    iStartOnLineTask = 1;
    bStartOnLine = true;
    bOnLine = false;
}
//==============================================================================
// V 1.0 (golden uHGemEquipment.cpp:5602-5606)
//==============================================================================
void THGem::OnLineLocal()
{
    bOnLineLocal = true;
    OnlineLocalOrRemote();
}
//==============================================================================
// V 1.0 (golden uHGemEquipment.cpp:5610-5614)
//==============================================================================
void THGem::OnLineRemote()
{
    bOnLineLocal = false;
    OnlineLocalOrRemote();
}
//==============================================================================
// V 1.1
// 2013/04/02 Lee (golden uHGemEquipment.cpp:5619-5624)
//==============================================================================
void THGem::OffLine()
{
    bOnLine = false;
    bStartOnLine = false;
    // golden: //EventReport(1, 91);  -- dead in golden itself (same
    // "JerryYang 20230204" disabling as OnlineLocalOrRemote above), not translated.
}
//==============================================================================
// V 1.0 (golden uHGemEquipment.cpp:5628-5631)
//==============================================================================
bool THGem::IsOnLine()
{
    return bOnLine;
}
//==============================================================================
// V 1.0 (golden uHGemEquipment.cpp:5635-5638)
//==============================================================================
bool THGem::GetOnLineMode()
{
    return bOnLineLocal;
}
//==============================================================================
// V 1.0 (golden uHGemEquipment.cpp:5642-5644)
//==============================================================================
void THGem::SetCanAcceptHostOnLineRequest(bool flag)
{
    // AI(W906-uHGemEquipment-ConnLifecycle) 20260717: golden body is
    // LITERALLY empty -- not a translation gap, preserved verbatim.
    (void)flag;
}

//---------------------------------------------------------------------------
// 2013/05/27
// V1.1 (golden uHGemEquipment.cpp:6812-6837)
//---------------------------------------------------------------------------
void __fastcall THGem::srvGemClientConnect(TObject *Sender, TCustomWinSocket *Socket)
{
    (void)Sender;
    AnsiString S;   // golden also declares S0,S1,S2,S3 here -- unused in
                    // golden's own body (dead declarations), not translated.
    for (int i = 0; i < srvGem->Socket->ActiveConnections; i++)
    {
        S = "Connect " + AnsiString(i) + ":" + srvGem->Socket->Connections[i]->LocalAddress;
        StringOut(S);
    }

    S = "Local Port:" + AnsiString(Socket->LocalPort);
    StringOut(S);
    S = "Local Address:" + AnsiString(Socket->LocalAddress);
    StringOut(S);
    StringOut("==================");
    StringOut("connect");
    bServoSocketConnect = true;
    if (srvGem->Socket->ActiveConnections > 1)
    {
        bReceiveMultiConnect = true;
        // AI(W906-uHGemEquipment-BucketB) 20260717: golden's
        // `TerminalMemoPtr!=NULL` branch here (uHGemEquipment.cpp:6832-6835)
        // was OUT OF SCOPE for the earlier ConnLifecycle wave (TerminalMemoPtr
        // was not yet a member); wired up for real now that THGemMemo/
        // TerminalMemoPtr exist (see uHGemEquipment.h). Message text extracted
        // from golden's raw bytes via `.decode('cp950')` (golden's own on-disk
        // encoding) to avoid this toolchain's Big5-mojibake risk on a direct
        // copy/retype -- decodes to "有2台以上EAP連接Handler,請確認" (2013/09/30 lee).
        if (TerminalMemoPtr != NULL)
        {
            TerminalMemoPtr->Lines->Add("有2台以上EAP連接Handler,請確認");
        }
    }
}
//---------------------------------------------------------------------------
// 2013/05/27
// V1.1 (golden uHGemEquipment.cpp:6842-6845)
//---------------------------------------------------------------------------
AnsiString __fastcall THGem::GetSocketErrorMsg(TObject *Sender, int iErrCode)
{
    return GetErrorMsg(Sender, iErrCode);
}
//---------------------------------------------------------------------------
// 2013/05/27
// V1.1 (golden uHGemEquipment.cpp:6850-6874)
//---------------------------------------------------------------------------
void __fastcall THGem::srvGemClientError(TObject *Sender, TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
    (void)Socket;
    (void)ErrorEvent;
    // 20130308 Daver add
    AnsiString S;
    StringOut("---------------------------------------------------");
    GetTimeInfo();
    S = GetSocketErrorMsg(Sender, ErrorCode) + "  " + TimeString;
    StringOut(S);
    SaveSECSGEMErrToLog(S);
    // ================

    bServoSocketConnect = false;   // Eliot 2012_1105
    try
    {
        srvGem->Close();
    }
    catch (...)
    {
        // Steven 20231113 : 記錄斷線例外 -- stand-in for golden's DEFERRED
        // LogClientSocketExceptionError, see the file-scope note above.
        LogClientSocketExceptionError_(this, Sender, "THGem");
    }
    ErrorCode = 0;
    bTCPIP_Error = true;
}
//---------------------------------------------------------------------------
// 2013/05/27
// V1.1 (golden uHGemEquipment.cpp:6897-6910)
//---------------------------------------------------------------------------
void __fastcall THGem::srvGemClientDisconnect(TObject *Sender, TCustomWinSocket *Socket)
{
    (void)Sender;
    (void)Socket;
    bServoSocketConnect = false;
    GetTimeInfo();
    StringOut("disconnect  " + TimeString);   // JerryYang 20190411 : 斷線時要記錄時間

    // AI(W906-uHGemEquipment-ConnLifecycle) 20260717: SAFETY-CRITICAL,
    // preserved verbatim -- golden's own `srvGem->Close();` line right here is
    // COMMENTED OUT in golden itself, under a comment block titled (Big5,
    // paraphrased) "the secret of reconnect-after-disconnect" -- "2013/07/18
    // lee: closing Servo Socket and reopening it from *inside this very
    // disconnect handler* -- turns out it WILL HANG the application." DO NOT
    // uncomment/add a srvGem->Close() call here. DoOpenCommuncation's own
    // passive-role state machine (case 100 above) is the ONLY place that
    // re-opens srvGem, and only when `!srvGem->Active` (see that function's
    // own SAFETY-CRITICAL citation). Golden (uHGemEquipment.cpp:6904-6909):
    //     //srvGem->Close();
    //     Timer1Task=1;                // 要重新answer
    //     iOpenCommuncationTask=1;     // 要重新answer
    // Timer1Task is a Timer1Timer-owned state variable; Timer1Timer itself is
    // OUT OF THIS WAVE'S SCOPE (needs SecsWireCodec embedded first -- see this
    // header's own "Do NOT translate" list) -- that reset is DEFERRED to
    // whichever future wave translates Timer1Timer. Flagged loudly here so
    // that translator does not miss it. Only the in-scope half is done below.
    iOpenCommuncationTask = 1;
}

//===========================================================================
//  Widget-persisted state + status refresh (W906-uHGemEquipment-BucketB wave)
//  (golden uHGemEquipment.cpp:409-417, 4747-4985, 5011-5173, 6887-6950)
//===========================================================================
//---------------------------------------------------------------------------
// V 1.0
// 將 Data 從設計指定的 TMemo 內做 show 出（含色彩）(golden uHGemEquipment.cpp:409-417)
//---------------------------------------------------------------------------
void __fastcall THGem::StringOut(AnsiString S, TColor C)
{
    // AI(W906-uHGemEquipment-BucketB) 20260717: GOLDEN QUIRK, preserved
    // verbatim -- the `C` (color) parameter is accepted but never actually
    // used anywhere in golden's own body (uHGemEquipment.cpp:409-417 only
    // ever touches DB->Lines/LogDataString/DB->SelStart; no DB->Font->Color
    // or similar). Not "fixed" by applying C to DB somewhere golden doesn't.
    (void)C;
    if (DB != NULL)
    {
        DB->Lines->Add(S);
        LogDataString->Add(S);
        DB->SelStart = DB->Lines->Count - 1;
    }
}

//---------------------------------------------------------------------------
// V 1.0 (golden uHGemEquipment.cpp:5011-5099)
//---------------------------------------------------------------------------
void __fastcall THGem::InitialHGem()
{
    bool bRead = true;
    bool bCompare = true;
    AnsiString sPath = GemSystemIniPath;

    if (EnableOrDisablePtr != NULL)
    {
        EnableOrDisablePtr->ItemIndex = ReadWriteIni(sPath, "GEM", "EnableOrDisable", 0, 0, bRead, bCompare, 0, EnableOrDisablePtr->Items.Count);
    }

    if (OnLineOrOffLine != NULL)
    {
        OnLineOrOffLine->ItemIndex = ReadWriteIni(sPath, "GEM", "OnLineOrOffLine", 0, 0, bRead, bCompare, 0, OnLineOrOffLine->Items.Count);
        if (CUSTOMER_CODE == CC_KYEC_LEE)
            OnLineOrOffLine->ItemIndex = 0;   // Eastsun 20260526 #026-1.73 Ifor 20210527 add: KYEC SECSGEM 強制Online Remote
    }

    if (RemoteOrLocal != NULL)
    {
        RemoteOrLocal->ItemIndex = ReadWriteIni(sPath, "GEM", "RemoteOrLocal", 0, 0, bRead, bCompare, 0, RemoteOrLocal->Items.Count);
        if (CUSTOMER_CODE == CC_KYEC_LEE)
            RemoteOrLocal->ItemIndex = 0;     // Eastsun 20260526 #026-1.73 Ifor 20210527 add: KYEC SECSGEM 強制Online Remote
    }

    rgRole->ItemIndex = ReadWriteIni(sPath, "GEM", "ActiveOrPassive", 0, 0, bRead, bCompare, 0, rgRole->Items.Count);
    bUseClientSocket = (rgRole->ItemIndex == 1);

    edtIP->Text = ReadIniData(sPath, "GEM", "Address", DefaultAddress);
    if (CUSTOMER_CODE == CC_KYEC_LEE)             // wei 20160309 SecsGem Port 強制6000
        edtPort->Text = 6000;
    else
        edtPort->Text = ReadIniData(sPath, "GEM", "Port", DefaultPort);

    edDeviceID->Text = ReadIniData(sPath, "GEM", "DeviceID", DefaultDeviceID);
    edtT3TimeOut->Text = ReadIniData(sPath, "GEM", "T3", T3TimeOut);   // pig 2014.07.28 KYEC_SECS start
    edtT5TimeOut->Text = ReadIniData(sPath, "GEM", "T5", T5TimeOut);
    edtT6TimeOut->Text = ReadIniData(sPath, "GEM", "T6", T6TimeOut);
    edtT7TimeOut->Text = ReadIniData(sPath, "GEM", "T7", T7TimeOut);
    edtT8TimeOut->Text = ReadIniData(sPath, "GEM", "T8", T8TimeOut);

    T3TimeOut = atoi(edtT3TimeOut->Text.c_str());
    T5TimeOut = atoi(edtT5TimeOut->Text.c_str());
    T6TimeOut = atoi(edtT6TimeOut->Text.c_str());
    T7TimeOut = atoi(edtT7TimeOut->Text.c_str());
    T8TimeOut = atoi(edtT8TimeOut->Text.c_str());

    clientGem->Address = edtIP->Text;
    clientGem->Port = 0;
    srvGem->Port = 0;
    // AI(W906-uHGemEquipment-BucketB) 20260717: golden's own next line here
    // is `Local.DeviceID=atoi(edDeviceID->Text.c_str());` -- OMITTED. `Local`
    // (HSMS_Head_Struct) is not yet a THGem member this wave (it belongs to
    // the wire-codec family SecsWireCodec.h already parallels -- see that
    // header's own note); this exact write has ZERO in-scope reader (no
    // function this wave translates ever reads Local.DeviceID back), so
    // omitting it is behavior-neutral for everything this wave actually
    // exercises/tests. Flagged here rather than silently dropped.
    if (bUseClientSocket == true)
    {
        clientGem->Port = atoi(edtPort->Text.c_str());
        edtIP->Enabled = true;
    }
    else
    {
        srvGem->Port = atoi(edtPort->Text.c_str());
        edtIP->Enabled = false;
    }

    GemCheckBoxAcceptHostOnlineRequest->Checked = ReadIniData(sPath, "GEM", "AcceptHostOnlineRequest", true);
    GemCheckBoxShowBinary->Checked = false;   // Steven 20211109 : SECS不再全部顯示binary code
    GemCheckBoxShowHeadInformation->Checked = ReadIniData(sPath, "GEM", "ShowHeadInformation", false);
    GemCheckBoxUseExtendedAlarm->Checked = ReadIniData(sPath, "GEM", "UseExtendedAlarm", false);
    chkAnnotatedEventReport->Checked = ReadIniData(sPath, "GEM", "AnnotatedEventReport", false);
    chkMoreMessageAbortProcess->Checked = ReadIniData(sPath, "GEM", "CheckMoreData", false);
    if (CUSTOMER_CODE == CC_MAXIM_THAILAND)       // Ifor 20251018 add: Analog 該客戶要求AddDefaultReport需可設定
    {
        ckAddDefaultReport->Checked = ReadIniData(sPath, "GEM", "AddDefaultReport", true);   // 2013/11/20  lee
    }
    else
    {
        ckAddDefaultReport->Checked = true;   // 2014/01/01  lee
    }

    if (CosFunction.bECChangeEventReportCanOnOff)   // JerryYang 20200520 客戶提出DoReportECDataChangeCheck函式會影響UPH,改成功能選項
    {
        cbECChaneEventReport->Checked = ReadIniData(sPath, "GEM", "ECChangeEventReport", false);
    }
    else
    {
        cbECChaneEventReport->Checked = true;
    }

    ReadEventReportData();

    if (ckAddDefaultReport->Checked == false)
        ClearDefaultEvenReport();
}

//---------------------------------------------------------------------------
// V 1.0 (golden uHGemEquipment.cpp:5101-5140)
//---------------------------------------------------------------------------
void __fastcall THGem::SaveSystemDefault()
{
    AnsiString sPath = GemSystemIniPath;
    if (EnableOrDisablePtr != NULL)
    {
        WriteIniData(sPath, "GEM", "EnableOrDisable", EnableOrDisablePtr->ItemIndex);
    }

    if (OnLineOrOffLine != NULL)
    {
        if (CUSTOMER_CODE == CC_KYEC_LEE)
            OnLineOrOffLine->ItemIndex = 0;   // Eastsun 20260526 #026-1.73 Ifor 20210527 add: KYEC SECSGEM 強制Online Remote
        WriteIniData(sPath, "GEM", "OnLineOrOffLine", OnLineOrOffLine->ItemIndex);
    }

    if (RemoteOrLocal != NULL)
    {
        if (CUSTOMER_CODE == CC_KYEC_LEE)
            RemoteOrLocal->ItemIndex = 0;     // Eastsun 20260526 #026-1.73 Ifor 20210527 add: KYEC SECSGEM 強制Online Remote
        WriteIniData(sPath, "GEM", "RemoteOrLocal", RemoteOrLocal->ItemIndex);
    }

    WriteIniData(sPath, "GEM", "ActiveOrPassive", rgRole->ItemIndex);
    WriteIniData(sPath, "GEM", "Address", edtIP->Text);
    WriteIniData(sPath, "GEM", "Port", edtPort->Text);
    WriteIniData(sPath, "GEM", "DeviceID", edDeviceID->Text);

    WriteIniData(sPath, "GEM", "T3", edtT3TimeOut->Text);   // pig 2014.07.28 KYEC_SECS start
    WriteIniData(sPath, "GEM", "T5", edtT5TimeOut->Text);
    WriteIniData(sPath, "GEM", "T6", edtT6TimeOut->Text);
    WriteIniData(sPath, "GEM", "T7", edtT7TimeOut->Text);
    WriteIniData(sPath, "GEM", "T8", edtT8TimeOut->Text);

    WriteIniData(sPath, "GEM", "AcceptHostOnlineRequest", GemCheckBoxAcceptHostOnlineRequest->Checked);
    WriteIniData(sPath, "GEM", "ShowBinaryData", GemCheckBoxShowBinary->Checked);
    WriteIniData(sPath, "GEM", "ShowHeadInformation", GemCheckBoxShowHeadInformation->Checked);
    WriteIniData(sPath, "GEM", "UseExtendedAlarm", GemCheckBoxUseExtendedAlarm->Checked);
    WriteIniData(sPath, "GEM", "AnnotatedEventReport", chkAnnotatedEventReport->Checked);
    WriteIniData(sPath, "GEM", "CheckMoreData", chkMoreMessageAbortProcess->Checked);
    WriteIniData(sPath, "GEM", "AddDefaultReport", ckAddDefaultReport->Checked);          // 2013/11/20  lee
    WriteIniData(sPath, "GEM", "ECChangeEventReport", cbECChaneEventReport->Checked);     // JerryYang 20200520 客戶提出DoReportECDataChangeCheck函式會影響UPH,改成功能選項
}

//---------------------------------------------------------------------------
// V 1.0 (golden uHGemEquipment.cpp:6510-6514)
//---------------------------------------------------------------------------
// AI(W906-SysModWire) 20260720: PORT -- verbatim golden body (2 assignments).
// "Softwarse" is golden's own misspelling (should be "Software") -- preserved
// verbatim in the golden-derived signature per this project's faithful-
// translation rule; not "corrected".
void __fastcall THGem::SetMachineTypeAndSoftwarseVer(AnsiString Mdln, AnsiString SoftVer)
{
    GemMDLN = Mdln;
    GemSOFTREV = SoftVer;
}

//===========================================================================
//  Bucket C (W906-uHGemEquipment-BucketC 20260717): socket receive pump / T3
//  timeout / HSMS control-message handshake / Timer1Timer master state
//  machine. See uHGemEquipment.h's own file-head addendum for the full scope
//  statement.
//===========================================================================
//---------------------------------------------------------------------------
// V 1.0 (golden uHGemEquipment.cpp:376-387)
// 對 Message Head 的 SType or PType 為主 0 為 system byte,保留給 connect
// command 使用
//---------------------------------------------------------------------------
void THGem::InitSTypeStruct()
{
    SType.Data_Message = 0;
    SType.Select_req   = 1;
    SType.Select_rsp   = 2;
    SType.Deselect_req = 3;
    SType.Deselect_rsp = 4;
    SType.Linktest_req = 5;
    SType.Linktest_rsp = 6;
    SType.Reject_req   = 7;
    SType.Separate_req = 9;
}
//------------------------------------------------------------------------------
// V 1.0 (golden uHGemEquipment.cpp:3496-3509)
// 與 remote 建立連線的要求傳送 -- Select.req
//------------------------------------------------------------------------------
void THGem::DoSelect()
{
    WireCodec.Local.MessageID_S = 0;
    WireCodec.Local.MessageID_F = 0;
    WireCodec.Local.W_Bit = 0;
    WireCodec.Local.PType = 0;
    WireCodec.Local.SType = SType.Select_req;
    WireCodec.EquipmentSystemByte++;
    WireCodec.Local.SystemByte = WireCodec.EquipmentSystemByte;
    WireCodec.LocalLength = 0;
    WireCodec.CreateLocalHead();
    StringOut("[Send]    Select.req");
    SendLocalData();
}
//---------------------------------------------------------------------------
// AI(W906-uHGemEquipment-BucketC) 20260717: DoSeparate is now REAL --
// REPLACES the Bucket-B gated no-op stub that used to sit here (see
// uHGemEquipment.h's own updated comment on the declaration). Real body
// (golden uHGemEquipment.cpp:3518-3531) sends the HSMS Separate.req message.
//---------------------------------------------------------------------------
void THGem::DoSeparate()
{
    WireCodec.Local.MessageID_S = 0;
    WireCodec.Local.MessageID_F = 0;
    WireCodec.Local.W_Bit = 0;
    WireCodec.Local.PType = 0;
    WireCodec.Local.SType = SType.Separate_req;
    WireCodec.EquipmentSystemByte++;
    WireCodec.Local.SystemByte = WireCodec.EquipmentSystemByte;
    WireCodec.LocalLength = 0;
    WireCodec.CreateLocalHead();
    StringOut("[Send]    Separate_req");
    SendLocalData();
}
//------------------------------------------------------------------------------
// V 1.0 (golden uHGemEquipment.cpp:3536-3599)
// 與 remote 建立連線的要求傳送
//------------------------------------------------------------------------------
int THGem::DoConnect()
{
    int &Task = iStartConnectTask;

    switch (Task)
    {
        case 1:
            bWaitSelectRsp = false;
            bTCPIP_Error = false;
            DoSelect();
            ConnectDelay.TimerSetSecAndOn(3);
            Task = 100;
            break;
        case 100:
            if (bWaitSelectRsp == true)
            {
                iConnectTryCount = iEstablishCommunicationsTryCount;
                bWaitEstablishCommunicationsResponse = false;
                bWaitEstablishCommunicationsResponseError = false;
                // AI(W906-uHGemEquipment-BucketC) 20260717: D3 null-guard --
                // golden calls this unguarded (uHGemEquipment.cpp:3555);
                // HSys.MyGem is NULL until SystemModularInitial wires it
                // (database.h -- tests do so directly; the production
                // SYSTEM_MODULAR ctor call site stays gated). Precedent:
                // cprod.cpp:2199/2398/3035 already null-guard HSys.MyGem in
                // this port.
                // AI(W906-SysModWire) 20260720: S1F13_EstablishCommunications
                // Request() is REAL as of this wave (uHGemClass.cpp) -- the
                // full DoConnect<->S1F13<->S1F14<->Process_S1F14 handshake now
                // goes end-to-end when wired (see test [W3]).
                if (HSys.MyGem != NULL)
                    HSys.MyGem->S1F13_EstablishCommunicationsRequest();
                Task = 200;
            }
            else if (ConnectDelay.TimerOff())
            {
                Task = 1;
                return 2;
            }
            break;
        case 200:
            if (bWaitEstablishCommunicationsResponse == true)
            {
                if (bWaitEstablishCommunicationsResponseError == true)
                {
                    if (iEstablishCommunicationsTryCount != 0)
                    {
                        if (iConnectTryCount > iEstablishCommunicationsTryCount)
                            iConnectTryCount = iEstablishCommunicationsTryCount;
                        iConnectTryCount--;
                        if (iConnectTryCount <= 0)
                            return 2;
                    }
                    countConnect = 0;
                    Task = 500;
                    break;
                }
                bConnect = true;
                Task = 1;
                return 1;
            }
            break;
        case 500:
            countConnect++;
            if (countConnect > 100)
                Task = 600;
            break;
        case 600:
            bWaitEstablishCommunicationsResponse = false;
            bWaitEstablishCommunicationsResponseError = false;
            // D3 null-guard (golden :3594, retry) -- see case 100's own note.
            if (HSys.MyGem != NULL)
                HSys.MyGem->S1F13_EstablishCommunicationsRequest();
            Task = 200;   // Ifor 20260420: Fix deadlock - was Task=400 (no such case), should wait for S1F14 response
            break;
    }
    return 0;
}
//------------------------------------------------------------------------------
// V 1.0 (golden uHGemEquipment.cpp:4604-4694)
// 對 S,F check respone
//------------------------------------------------------------------------------
void __fastcall THGem::DoProcessSFNoResponse()
{
    int iT;
    Word &iOldSec = iOldSecProcessSFNoResponse;
    bool bClear = false;
    AnsiString S;

    if (iOldSec != SystemSec)
    {
        iOldSec = SystemSec;
        do
        {
            bClear = false;
            if (TimeLeft->Count != SFCodeResponseList->Items->Count)
            {
                // Joseph 20221111 (Jason) 新增 TCriticalSection S
                //==>
                {
                    ScopedAcquire lk(csSFCodeResponse);   // golden :4621/:4629 try/__finally
                    SFCodeResponseList->Clear();
                    TimeLeft->Clear();
                }
                //<==
                // Joseph 20221111 (Jason) 新增 TCriticalSection E
            }
            for (int i = 0; i < TimeLeft->Count; i++)
            {
                S = TimeLeft->Strings[i];
                iT = atoi(S.c_str());
                iT--;
                if (iT <= 0)
                {
                    S = SFCodeResponseList->Items->Strings[i];
                    S += "    T3   time out";
                    StringOut(S);
                    // Ifor 20260402: S9F9 Transaction Timer Timeout
                    // D3 null-guard (golden :4647) -- see DoConnect's own note.
                    if (HSys.MyGem != NULL)
                        HSys.MyGem->S9F9_TransactionTimerTimeout(S);

                    // Joseph 20221111 (Jason) 新增 TCriticalSection S
                    //==>
                    {
                        ScopedAcquire lk(csSFCodeResponse);   // golden :4651/:4659 try/__finally
                        SFCodeResponseList->Items->Delete(i);
                        TimeLeft->Delete(i);
                    }
                    //<==
                    // Joseph 20221111 (Jason) 新增 TCriticalSection E

                    bClear = true;
                    break;
                }
                else
                {
                    // Joseph 20221111 (Jason) 新增 TCriticalSection S
                    //==>
                    {
                        ScopedAcquire lk(csSFCodeResponse);   // golden :4674/:4681 try/__finally
                        TimeLeft->Strings[i] = iT;
                    }
                    //<==
                    // Joseph 20221111 (Jason) 新增 TCriticalSection E
                }
            }

            if (bClear == false)
                break;
        } while (1);
    }
}
//------------------------------------------------------------------------------
// V 1.0 (golden uHGemEquipment.cpp:4699-4741)
// 對 remote 連線後資料回應[需要被用不同 S,F code 分辨]
//------------------------------------------------------------------------------
void THGem::DoLocalAllProcessLoop()
{
//    DoReportECDataChangeCheck();  //Steven 20200807 : mark for出貨Sleep的時候去檢查EC change report

    if (bAutoConnect == false)
        iAutoConnectDelay = 2;

    if (bConnect == false && bAutoConnect)
    {
        if (SystemSec % 3 == 0)
        {
            if (iAutoConnectSec != SystemSec)
            {
                if (iAutoConnectDelay > 0)
                    iAutoConnectDelay--;
                if (iAutoConnectDelay < 0)
                    iAutoConnectDelay = 1;
                if (iAutoConnectDelay == 0)
                {
                    iAutoConnectSec = SystemSec;
                    iStartConnectTask = 1;
                    bStartConnect = true;
                }
            }
        }
    }

    if (bConnect == true)
        bAutoConnect = false;
    DoSpool();
    for (int i = 0; i < 10; i++)
        DoTraceDataResponse(i);
    if (bSeprate == true)
    {
        // AI(W906-uHGemEquipment-BucketC) 20260717: GOLDEN BUG, preserved
        // verbatim (uHGemEquipment.cpp:4733-4736) -- BOTH branches of this
        // if/else assign bSeprate=false; the bWaitEstablishCommunicationsResponse
        // check has no observable effect either way. NOT fixed here.
        if (bWaitEstablishCommunicationsResponse == false)
            bSeprate = false;
        else
            bSeprate = false;
    }
    DoUploadFileToHost();   // need debug
    DoProcessSFNoResponse();
    DoDownLoadRemoteFile();
}
//---------------------------------------------------------------------------
// AI(W906-uHGemEquipment-BucketC) 20260717: GATED STUB (golden
// uHGemEquipment.cpp:4079-4189). Real body manages the spool-file retry
// queue (WriteToSpoolFile/DoSpoolSendLocalData/GemSpoolPath/bSpoolActive/
// bBeginTransferSpool/GemSpoolCountActual/...), none of which is part of
// this wave's scope. No-op: DoLocalAllProcessLoop's own control flow
// (which unconditionally calls this) stays translatable without silently
// dropping the call.
//---------------------------------------------------------------------------
void THGem::DoSpool()
{
}
//---------------------------------------------------------------------------
// AI(W906-uHGemEquipment-BucketC) 20260717: GATED STUB (golden
// uHGemEquipment.cpp:4190-4589, called once per index 0..9 from
// DoLocalAllProcessLoop's own `for` loop). Real body manages the S6F1 Trace
// Data Send retry state machine (TraceData[]/TraceDataResponseTask[]/
// bTraceData[]/TraceDataResponseDelay[]/iTRID[]/...), none of which is part
// of this wave's scope.
//---------------------------------------------------------------------------
void THGem::DoTraceDataResponse(int TR)
{
    (void)TR;
}
//---------------------------------------------------------------------------
// AI(W906-uHGemEquipment-BucketC) 20260717: GATED STUB (golden
// uHGemEquipment.cpp:4591-4589... actually :4591 is the call site inside
// DoLocalAllProcessLoop; the real DoUploadFileToHost body is further down,
// dispatching to DoUploadFileToHost_ForSingleFile/_ForMultiFile/
// _ForDirectoryFile -- an FTP/file-transfer surface entirely out of this
// wave's scope).
//---------------------------------------------------------------------------
void THGem::DoUploadFileToHost()
{
}
//---------------------------------------------------------------------------
// AI(W906-uHGemEquipment-BucketC) 20260717: GATED STUB (golden
// uHGemEquipment.cpp:6746, called from Timer1Timer case 410's own `else`
// branch). Real body manages the remote-recipe-download retry state machine
// (DelayDownLoadRemoteFile/iDownLoadRemoteFileTask/iRetryCTDownLoadRemoteFile/
// ...), an FTP/file-transfer surface entirely out of this wave's scope.
//---------------------------------------------------------------------------
void THGem::DoDownLoadRemoteFile()
{
}
//---------------------------------------------------------------------------
// 2013/06/29  V1.1  Lee (golden uHGemEquipment.cpp:8679-8693)
// Format-only-head check for S,F handlers that expect a head-only (no data
// item) message: when chkMoreMessageAbortProcess is unchecked, this is a
// no-op pass (golden quirk -- the "more message abort" feature is opt-in);
// when checked, consumes ONE zero-length item and requires it to be exactly
// L[0], else reports "S,F data format error" via SendInvalidDataMessageToHost
// and returns false.
//---------------------------------------------------------------------------
// AI(W906-SysModWire) 20260720: PORT -- StringOut/chkMoreMessageAbortProcess
// are THGem's own members (implicit this->, unchanged); GetDataItemLenAndTypeAndDelete
// and SendInvalidDataMessageToHost are SecsWireCodec methods (D1/D2 precedent)
// so both route through `WireCodec.` here.
bool THGem::CheckSFFormatOnlyHead(AnsiString ErrStr)
{
    int ret, len;
    unsigned char Type;

    if (chkMoreMessageAbortProcess->Checked == false)
        return true;

    ret = WireCodec.GetDataItemLenAndTypeAndDelete(len, Type);
    if (ret != 1)
        return true;
    StringOut(ErrStr);
    WireCodec.SendInvalidDataMessageToHost(ErrStr);
    return false;
}
//---------------------------------------------------------------------------
// 2013/05/27  V1.1 (golden uHGemEquipment.cpp:8707-8723)
// control message SelectRequest Response
//---------------------------------------------------------------------------
void THGem::SelectRsp()
{
    unsigned short back;
    WireCodec.Local.MessageID_S = 0;
    WireCodec.Local.MessageID_F = 0;
    WireCodec.Local.W_Bit = 0;
    WireCodec.Local.PType = 0;
    WireCodec.Local.SType = SType.Select_rsp;
    WireCodec.Local.SystemByte = RemoteSystemByte;
    back = WireCodec.Local.DeviceID;
    WireCodec.Local.DeviceID = WireCodec.Remote.DeviceID;
    WireCodec.LocalLength = 0;
    WireCodec.CreateLocalHead();
    StringOut("[Send]    Select.rsp");
    SendLocalData();
    WireCodec.Local.DeviceID = back;
}
//---------------------------------------------------------------------------
// 2013/05/27  V1.1 (golden uHGemEquipment.cpp:8729-8745)
// control message DeSelectRequest Response
//---------------------------------------------------------------------------
void THGem::DeselectRsp()
{
    unsigned short back;
    WireCodec.Local.MessageID_S = 0;
    WireCodec.Local.MessageID_F = 0;
    WireCodec.Local.W_Bit = 0;
    WireCodec.Local.PType = 0;
    WireCodec.Local.SType = SType.Deselect_rsp;
    WireCodec.Local.SystemByte = RemoteSystemByte;
    back = WireCodec.Local.DeviceID;
    WireCodec.Local.DeviceID = WireCodec.Remote.DeviceID;
    WireCodec.LocalLength = 0;
    WireCodec.CreateLocalHead();
    StringOut("[Send]    Deselect.rsp");
    SendLocalData();
    WireCodec.Local.DeviceID = back;
}
//---------------------------------------------------------------------------
// 2013/05/27  V1.1 (golden uHGemEquipment.cpp:8751-8765)
// control message LinkTest Response
//---------------------------------------------------------------------------
void THGem::LinktestRsp()
{
    WireCodec.Local.MessageID_S = 0;
    WireCodec.Local.MessageID_F = 0;
    WireCodec.Local.W_Bit = 0;
    WireCodec.Local.PType = 0;
    WireCodec.Local.SType = SType.Linktest_rsp;
    WireCodec.Local.SystemByte = RemoteSystemByte;
    WireCodec.LocalLength = 0;
    WireCodec.CreateLocalHead();
    StringOut("[Send]    Linktest.rsp");
    WireCodec.LocalBuffer[4] = 0xff;
    WireCodec.LocalBuffer[5] = 0xff;
    SendLocalData();
}
//==============================================================================
// 2013/05/27  V1.1 (golden uHGemEquipment.cpp:8772-8989)
// Main handling of Stream and Function code dispatch.
//
// AI(W906-uHGemEquipment-BucketC) 20260717: SPLIT -- the HSMS control-message
// head below (Select/Deselect/Linktest/Separate) is REAL.
// AI(W906-SysModWire) 20260720: the S,F data-message dispatch tail (the final
// `else` block) is NOW REAL TOO -- see the null-guard comment at its start
// for the wiring rationale and the two-codec-instance self-heal it performs.
//==============================================================================
void THGem::ProcessReceiceData()
{
    AnsiString S;

    if (WireCodec.Remote.SType == SType.Select_req)
    {
        RemoteSystemByte = WireCodec.Remote.SystemByte;
        StringOut("[Receive] Select.req");
        SelectRsp();
    }
    else if (WireCodec.Remote.SType == SType.Select_rsp)
    {
        StringOut("[Receive] Select.rsp");
        bWaitSelectRsp = true;
    }
    else if (WireCodec.Remote.SType == SType.Deselect_req)
    {
        RemoteSystemByte = WireCodec.Remote.SystemByte;
        StringOut("[Receive] Deselect.req");
        DeselectRsp();
    }
    else if (WireCodec.Remote.SType == SType.Deselect_rsp)
    {
        StringOut("[Receive] Deselect.rsp");
        bWaitDeSelectRsp = true;
    }
    else if (WireCodec.Remote.SType == SType.Linktest_req)
    {
        RemoteSystemByte = WireCodec.Remote.SystemByte;
        StringOut("[Receive] Linktest.req");
        LinktestRsp();
    }
    else if (WireCodec.Remote.SType == SType.Separate_req)
    {
        RemoteSystemByte = WireCodec.Remote.SystemByte;
        StringOut("[Receive] Separate.req");
        if (bConnect == true)
            bAutoConnect = true;
        bConnect = false;
    }
    else
    {
        // AI(W906-SysModWire) 20260720: PORT null-guard -- golden dereferences
        // HSys.MyGem unguarded (golden :8817 etc.); in this port MyGem is
        // NULL until SystemModularInitial is invoked (tests) -- precedent:
        // cprod.cpp:2199 and this file's own DoConnect/DoProcessSFNoResponse
        // D3 sites. When NULL the ENTIRE tail (including CheckSFCodeResponse
        // and the flag latches) is skipped -- this deviation exists ONLY in
        // the unwired offline state, unreachable in golden production (where
        // MyGem is never NULL once AddSV/AddEC have run).
        if (HSys.MyGem != NULL)
        {
            // AI(W906-SysModWire) 20260720: PORT-ONLY re-bind -- golden has
            // ONE codec (inside THGem); the port's HTGem carries its own
            // by-value codec for standalone tests. Re-point the dispatch
            // target at THIS engine's live decode buffer before every
            // dispatch (self-heals golden's own NULL-at-static-init
            // construction order -- see design brief §1.1 -- and mirrors
            // golden's own HGemPtr=HGem re-point in AddSV, future
            // uHGemHT9045_SV.cpp:61).
            HSys.MyGem->ActiveWire = &WireCodec;

            if (bDataFormatOK == false)
            {
                // Ifor 20260421: S9F7 - Use proper S9F7_IllegalData instead of SendInvalidDataMessageToHost
                HSys.MyGem->S9F7_IllegalData("Data Format Error");
                return;
            }
            CheckSFCodeResponse();

            // Ifor 20260421: S9F1 - Re-enable Device ID check (reject transaction only, keep connection alive)
            if (WireCodec.Remote.W_Bit == 1 && WireCodec.Remote.DeviceID != WireCodec.Local.DeviceID && WireCodec.Remote.DeviceID != 0xFFFF)
            {
                AnsiString sDevIDErr;
                sDevIDErr.sprintf("Unrecognized Device ID: Remote=%d, Local=%d", WireCodec.Remote.DeviceID, WireCodec.Local.DeviceID);
                HSys.MyGem->S9F1_UnrecognizedDeviceID(sDevIDErr);
                return;
            }

            if (WireCodec.Remote.MessageID_S == 1 && WireCodec.Remote.MessageID_F == 1)          // [S1F1] Are you There
            {
                HSys.MyGem->S1F2_OnLineData();                                                   // [S1F2] On Line Data
            }
            else if (WireCodec.Remote.MessageID_S == 1 && WireCodec.Remote.MessageID_F == 2)      // [S1F2] On Line Data
            {
                StringOut("[On Line Data]");
                bS1F2_OnLineData = true;
            }
            else if (WireCodec.Remote.MessageID_S == 1 && WireCodec.Remote.MessageID_F == 13)     // [S1F13] Connect Request
            {
                HSys.MyGem->S1F14_ConnectRequestAcknowledge();                                    // [S1F14] Connect Request Acknowledge
            }
            else if (WireCodec.Remote.MessageID_S == 1 && WireCodec.Remote.MessageID_F == 17)     // [S1F17] Request ON-LINE
            {
                HSys.MyGem->S1F18_ONLINEAcknowledge();                                            // [S1F18] ON-LINE Acknowledge
            }
            else if (WireCodec.Remote.MessageID_S == 2 && WireCodec.Remote.MessageID_F == 15)     // [S2F15] New Equipment Constant Send
            {
                HSys.MyGem->S2F16_NewEquipmentConstantSendAcknowledge();                          // [S2F16] New Equipment Constant Send Acknowledge
            }
            else
            {
                if ((CUSTOMER_CODE == CC_TFME_CHINA && GemControlState > 1) || CUSTOMER_CODE != CC_TFME_CHINA)   // JerryYang 20200527: no other S,F code accepted except Online state
                {
                    if (MoveCheckCallBack != NULL)
                        if (MoveCheckCallBack() == 1)                                             // whether this S,F Code should be handled by the Application instead
                            return;

                    if (WireCodec.Remote.MessageID_S == 1 && WireCodec.Remote.MessageID_F == 3)         // [S1F3] Selected Status Request
                        HSys.MyGem->S1F4_SelectedStatusReply();                                         // [S1F4] Selected Status Reply
                    else if (WireCodec.Remote.MessageID_S == 1 && WireCodec.Remote.MessageID_F == 11)   // [S1F11] Status Variable Namelist Request
                        HSys.MyGem->S1F12_StatusVariableNamelistReply();                                // [S1F12] Status Variable Namelist Reply
                    else if (WireCodec.Remote.MessageID_S == 1 && WireCodec.Remote.MessageID_F == 14)   // [S1F14] Connect Request Acknowledge
                        HSys.MyGem->Process_S1F14_ConnectRequestAcknowledge();
                    else if (WireCodec.Remote.MessageID_S == 1 && WireCodec.Remote.MessageID_F == 15)   // [S1F15] Request OFF-LINE
                        HSys.MyGem->S1F16_OFFLINEAcknowledge();                                         // [S1F16] OFF-LINE Acknowledge
                    else if (WireCodec.Remote.MessageID_S == 1 && WireCodec.Remote.MessageID_F == 23)   // [S1F23] Collection Event Namelist Request   //2014/01/01  lee
                        HSys.MyGem->S1F24_CollectionEventNamelist();                                    // [S1F24] Collection Event Namelist           //2014/01/01  lee
                    else if (WireCodec.Remote.MessageID_S == 2 && WireCodec.Remote.MessageID_F == 13)   // [S2F13] Equipment Constant Request
                        HSys.MyGem->S2F14_EquipmentConstanData();                                       // [S2F14] Equipment Constant Data
                    else if (WireCodec.Remote.MessageID_S == 2 && WireCodec.Remote.MessageID_F == 17)   // [S2F17] Date and Time Request
                        HSys.MyGem->S2F18_DateandTimeData();                                            // [S2F18] Date and Time Data
                    else if (WireCodec.Remote.MessageID_S == 2 && WireCodec.Remote.MessageID_F == 23)   // [S2F23] Trace Initial Send
                        HSys.MyGem->S2F24_TraceInitializeAcknowledge();                                 // [S2F24] Trace Initial Acknowledge
                    else if (WireCodec.Remote.MessageID_S == 2 && WireCodec.Remote.MessageID_F == 25)   // [S2F25] Diagnostic Loopback Request
                        HSys.MyGem->S2F26_DiagnosticLoopbackData();                                     // [S2F26] Diagnostic Loopback Data
                    else if (WireCodec.Remote.MessageID_S == 2 && WireCodec.Remote.MessageID_F == 29)   // [S2F29] Equipment Constant Namelist Request
                        HSys.MyGem->S2F30_EquipmentConstantNamelistReply();                             // [S2F30] Equipment Constant Namelist Reply
                    else if (WireCodec.Remote.MessageID_S == 2 && WireCodec.Remote.MessageID_F == 31)   // [S2F31] Date and Time Send
                        HSys.MyGem->S2F32_DateAndTimeAcknowledge();                                     // [S2F32] Date and Time Acknowledge
                    else if (WireCodec.Remote.MessageID_S == 2 && WireCodec.Remote.MessageID_F == 33)   // [S2F33] Define Report
                        HSys.MyGem->S2F34_DefineReportAcknowledge();                                    // [S2F34] Define Report Acknowledge
                    else if (WireCodec.Remote.MessageID_S == 2 && WireCodec.Remote.MessageID_F == 35)   // [S2F35] Link Event Report
                        HSys.MyGem->S2F36_LinkEventReportAcknowledge();                                 // [S2F36] Link Event Report Acknowledge
                    else if (WireCodec.Remote.MessageID_S == 2 && WireCodec.Remote.MessageID_F == 37)   // [S2F37] Enable/Disable Event Report
                        HSys.MyGem->S2F38_EnableDisableEventReportAcknowledge();                        // [S2F38] Enable/Disable Event Report Acknowledge
                    else if (WireCodec.Remote.MessageID_S == 2 && WireCodec.Remote.MessageID_F == 43)   // [S2F37] Enable/Disable Event Report (golden's own mis-cited comment, preserved)
                        HSys.MyGem->S2F44_ResetSpoolingAcknowledge();                                   // [S2F43] Enable/Disable Event Report Acknowledge (golden's own mis-cited comment, preserved)
                    else if (WireCodec.Remote.MessageID_S == 5 && WireCodec.Remote.MessageID_F == 3)    // [S5F3] Enable/Disable Alarm Send
                        HSys.MyGem->S5F4_EnableDisableAlarmAcknowledge();                               // [S5F4] Enable/Disable Alarm Acknowledge
                    else if (WireCodec.Remote.MessageID_S == 5 && WireCodec.Remote.MessageID_F == 5)    // [S5F5] List Alarm Request
                        HSys.MyGem->S5F6_ListAlarmData();                                               // [S5F6] List Alarm Data
                    else if (WireCodec.Remote.MessageID_S == 5 && WireCodec.Remote.MessageID_F == 7)    // [S5F7] List Enable Alarm Request
                        HSys.MyGem->S5F8_ListEnableAlarmAcknowledge();                                  // [S5F8] List Enable Alarm Acknowledge
                    else if (WireCodec.Remote.MessageID_S == 6 && WireCodec.Remote.MessageID_F == 15)   // [S6F15] Event Report Request
                        HSys.MyGem->S6F16_EventReportData();                                            // [S6F16] Event Report Data
                    else if (WireCodec.Remote.MessageID_S == 6 && WireCodec.Remote.MessageID_F == 17)   // [S6F17] Annotated Event Report Request
                        HSys.MyGem->S6F18_AnnotatedEventReportData();                                   // [S6F18] Annotated Event Report Data
                    else if (WireCodec.Remote.MessageID_S == 6 && WireCodec.Remote.MessageID_F == 19)   // [S6F19] Individual Report Request
                        HSys.MyGem->S6F20_IndividualReportData();                                       // [S6F20] Individual Report Data
                    else if (WireCodec.Remote.MessageID_S == 6 && WireCodec.Remote.MessageID_F == 23)   // [S6F23] Request Spooled Data
                        HSys.MyGem->S6F24_RequestSpooledDataAcknowledgementSend();                      // [S6F24] Request Spooled Data Acknowledgement Send
                    else if (WireCodec.Remote.MessageID_S == 7 && WireCodec.Remote.MessageID_F == 1)    // [S7F1] Process Program Load Inquire
                        HSys.MyGem->S7F2_ProcessProgramLoadGrant();                                     // [S7F2] Process Program Load Grant
                    else if (WireCodec.Remote.MessageID_S == 7 && WireCodec.Remote.MessageID_F == 3)    // [S7F1] Process Program Load Inquire (golden's own mis-cited comment, preserved)
                        HSys.MyGem->S7F4_ProcessProgramAcknowledge();                                   // [S7F2] Process Program Load Grant (golden's own mis-cited comment, preserved)
                    else if (WireCodec.Remote.MessageID_S == 7 && WireCodec.Remote.MessageID_F == 5)    // [S7F5] Process Program Request
                        HSys.MyGem->S7F6_ProcessProgramData();                                          // [S7F6] Process Program Data
                    else if (WireCodec.Remote.MessageID_S == 7 && WireCodec.Remote.MessageID_F == 6)    // [S7F6] Process Program Data
                        bReceiveS7F6 = true;
                    else if (WireCodec.Remote.MessageID_S == 7 && WireCodec.Remote.MessageID_F == 17)   // [S7F17] Delete Process Program Send
                        HSys.MyGem->S7F18_DeleteProcessProgramAcknowledge();                            // [S7F18] Delete Process Program Acknowledge
                    else if (WireCodec.Remote.MessageID_S == 7 && WireCodec.Remote.MessageID_F == 19)   // [S7F19] Current EPPID Request
                        HSys.MyGem->S7F20_CurrentEPPDData();                                            // [S7F20] Current EPPID Data
                    else if (WireCodec.Remote.MessageID_S == 7 && WireCodec.Remote.MessageID_F == 20)   // [S7F20] Current EPPID Data
                        HSys.MyGem->Process_S7F20_CurrentEPPIDData();
                    else if (WireCodec.Remote.MessageID_S == 10 && WireCodec.Remote.MessageID_F == 3)   // [S10F3] Terminal Display Single
                        HSys.MyGem->S10F4_TerminalDisplaySingleAcknowledge();                           // [S10F4] Terminal Display Single Acknowledge
                    else if (WireCodec.Remote.MessageID_S == 10 && WireCodec.Remote.MessageID_F == 5)   // [S10F5] Terminal Display Multi-block
                        HSys.MyGem->S10F6_TerminalDisplayMultiBlockAcknowledge();                       // [S10F6] Terminal Display Multi-block Acknowledge
                    // AI(W906-SysModWire) 20260720: GOLDEN QUIRK preserved verbatim -- golden
                    // :8922 dereferences the GLOBAL `HGem->` pointer here instead of the
                    // implicit `this->` every other branch in this chain uses (the file's
                    // only branch that does so). A caller/test must set `HGem = &instance;`
                    // before exercising this branch (existing project convention, e.g.
                    // ProcessShow); HGem==NULL here derefs NULL, same as golden's own
                    // production precondition -- not guarded, by design.
                    else if (HGem->WireCodec.Remote.MessageID_S == 14 && HGem->WireCodec.Remote.MessageID_F == 3)   // [S14F3] receive 2DID & bin code
                        HSys.MyGem->S14F4_Get2DID_BinCode();
                    else if (WireCodec.Remote.MessageID_S == 100 && WireCodec.Remote.MessageID_F == 3)
                        HSys.MyGem->S100F4_ReportAllAlarm();
                    else if (WireCodec.Remote.MessageID_S == 101 && WireCodec.Remote.MessageID_F == 1)
                        HSys.MyGem->S101F2_CurrentEPPDData();
                    else if (WireCodec.Remote.MessageID_S == 101 && WireCodec.Remote.MessageID_F == 3)
                        HSys.MyGem->S101F4_CurrentEPPDData();
                    else if (WireCodec.Remote.MessageID_S == 101 && WireCodec.Remote.MessageID_F == 5)
                        HSys.MyGem->S101F6();                                                           // download HOST RECIPE
                    else if (WireCodec.Remote.MessageID_S == 101 && WireCodec.Remote.MessageID_F == 6)
                        bReceiveS101F6 = true;
                    else if (WireCodec.Remote.MessageID_S == 101 && WireCodec.Remote.MessageID_F == 7)
                        HSys.MyGem->S101F8();
                    else if (WireCodec.Remote.MessageID_S == 101 && WireCodec.Remote.MessageID_F == 8)
                        bReceiveS101F8 = true;
                    else if (WireCodec.Remote.MessageID_S == 103 && WireCodec.Remote.MessageID_F == 11)   // [S103F11] Status Variable Namelist Request with Value  //Steven 20140911 : Add S103F1
                        HSys.MyGem->S103F12_StatusVariableNamelistReply();                                // [S103F12] Status Variable Namelist Reply with Value
                    //pig 2014.04.01 ASEM SECS start
                    else if (WireCodec.Remote.MessageID_S == 110 && WireCodec.Remote.MessageID_F == 2)
                        bReceiveS110F2 = true;
                    //pig 2014.07.04 ASEM SECS GEM start
                    else if (WireCodec.Remote.MessageID_S == 110 && WireCodec.Remote.MessageID_F == 6)
                        HSys.MyGem->S110F6_ListCustomerName();
                    else if (WireCodec.Remote.MessageID_S == 110 && WireCodec.Remote.MessageID_F == 8)
                        HSys.MyGem->S110F8_ListReceipeInformation();
                    else if (WireCodec.Remote.MessageID_S == 120 && WireCodec.Remote.MessageID_F == 2)
                        HSys.MyGem->S120F2_ListReceipeSetupFile();
                    //pig 2014.07.04 ASEM SECS GEM end
                    //pig 2014.08.27 start
                    else if (WireCodec.Remote.MessageID_S == 125 && WireCodec.Remote.MessageID_F == 1)   // [S125F1] Enable/Disable EC Data Send
                        HSys.MyGem->S125F2_EnableDisableECDataAcknowledge();                             // [S125F2] Enable/Disable EC Data Acknowledge
                    //pig 2014.08.27 end
                    else if (WireCodec.Remote.MessageID_S == 125 && WireCodec.Remote.MessageID_F == 3)   // [S125F3] Level Setting Change Request
                        HSys.MyGem->S125F4_LevelSettingChangeAcknowledge();                              // [S125F4] Level Setting Change Acknowledge  //Steven 20150605 : S125F3 LevelSettingChangeRequest
                    else
                    {
                        // LocalAcknowledge(Remote.MessageID_S,Remote.MessageID_F+1,0);
                        // if(MoveCheckCallBack!=NULL)
                        //     MoveCheckCallBack();
                        // Ifor 20260402: S9F3/S9F5 - send for all customers (was only CC_ASE_KaohSiung_K3)
                        // S9F3: Unrecognized Stream, S9F5: Unrecognized Function (stream known but function unknown)
                        if (WireCodec.Remote.MessageID_F % 2 == 0)
                        {
                            // Secondary message reply - silently accept
                            // (S5F2, S6F12, S6F14, S10F2, S101F2/4/6/8, etc.)
                        }
                        else if (WireCodec.Remote.MessageID_F == 0)
                        {
                            // SxF0 = Abort Transaction (SEMI E5)
                            // Do not respond with S9F5; just log and ignore
                        }
                        else
                        {
                            int iS = WireCodec.Remote.MessageID_S;
                            bool bKnownStream = (iS == 1 || iS == 2 || iS == 5 || iS == 6 || iS == 7 ||
                                                 iS == 10 || iS == 14 || iS == 100 || iS == 101 ||
                                                 iS == 103 || iS == 110 || iS == 120 || iS == 125);
                            if (bKnownStream)
                                HSys.MyGem->S9F5_UnrecognizedFunctionType("S,F Function Not Define");
                            else
                                HSys.MyGem->S9F3_Unrecognized_Stream_Function_Type("S,F Stream Not Define");
                        }
                    }
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
// 2013/05/27  V1.1  Lee (golden uHGemEquipment.cpp:7020-7058)
// 檢查目前的 SF code 是否為 response code ,例如目前送 S1,F13 則是否為 S1,F14
// 若是則把此 item delete ,否則一段時間沒有得到return code 要發出 T3 error
//---------------------------------------------------------------------------
void __fastcall THGem::CheckSFCodeResponse()
{
    int Index;
    AnsiString S;

    S = AnsiString(WireCodec.Remote.MessageID_S) + " " + AnsiString(WireCodec.Remote.MessageID_F) + " " + AnsiString(WireCodec.Remote.SystemByte);

    // Joseph 20221111 (Jason) 新增 TCriticalSection S
    //==>
    {
        ScopedAcquire lk(csSFCodeResponse);   // golden :7029/:7047 try/__finally
        if (TimeLeft->Count != SFCodeResponseList->Items->Count)
        {
            SFCodeResponseList->Clear();
            TimeLeft->Clear();
        }
        else
        {
            Index = SFCodeResponseList->Items->IndexOf(S);
            if (Index != -1)
            {
                SFCodeResponseList->Items->Delete(Index);
                TimeLeft->Delete(Index);
            }
        }
    }
    // golden's own commented-out pre-critical-section duplicate (:7052-7057)
    // is dead code in golden itself -- not translated.
    //<==
    // Joseph 20221111 (Jason) 新增 TCriticalSection E
}
//---------------------------------------------------------------------------
// 2013/05/27  V1.1  Daver (golden uHGemEquipment.cpp:7068-7091)
// 把目前接收到的資料以 Text 方式做出存檔
// 存檔格式為  \\程式目錄\\logs\\SECS_GEM\\目前的年_月_日\\時.txt
//---------------------------------------------------------------------------
void __fastcall THGem::SaveSECSGEMTextToLog()
{
    TDateTime tdSaveTime = Now();
    AnsiString asPath, asFN;
    FILE *P;
    int i;

    // AI(W906-uHGemEquipment-BucketC) 20260717: DateSeparator='_' omitted --
    // same inert reasoning as the already-committed SaveSECSGEMErrToLog (see
    // that method's own comment) -- FormatDateTime's "yyyy"/"mm_dd"/"hh"
    // tokens never contain the '/' DateSeparator would substitute into.
    asPath.sprintf("D:\\SECS_GEM_LOGS\\%s\\%s", FormatDateTime("yyyy", tdSaveTime), FormatDateTime("mm_dd", tdSaveTime));
    Gated_MyForceDirectories(asPath);
    asFN.sprintf("%s\\SECSGEM_TextLog_%s.txt", asPath, FormatDateTime("hh", tdSaveTime));

    P = fopen(asFN.c_str(), "a+");
    if (P != NULL)
    {
        for (i = 0; i < LogDataString->Count; i++)
        {
            // AI(W906-uHGemEquipment-BucketC) 20260717: golden is
            // `LogDataString->Strings[i].c_str()`; vclcompat::TStringList's
            // Strings[] proxy has no .c_str() (same accommodation already
            // established at PasteStringGridAsTabFormat's own GetString(y)
            // site, above) -- GetString(i) returns the real AnsiString.
            fputs(LogDataString->GetString(i).c_str(), P);
            fputs("\n", P);
        }
        fclose(P);
        LogDataString->Clear();
    }
}
//---------------------------------------------------------------------------
// V1.0 (golden uHGemEquipment.cpp:1350-1373)
// 依設定是否將要送出的資料 以 Binary 格式將全部資料印出
//---------------------------------------------------------------------------
void THGem::ShowLocalBufferBinaryData(SecsWireCodec &wc)
{
    if (GemCheckBoxShowBinary->Checked == false)
        return;

    AnsiString S = "";
    AnsiString str;
    StringOut("");

    for (int i = 0; i < 4; i++)
    {
        str.sprintf("%02X,", wc.LocalBuffer[i]);
        S += AnsiString(str);
    }
    StringBinaryOut(S);
    S = "";
    for (int i = 4; i < 14; i++)
    {
        str.sprintf("%02X,", wc.LocalBuffer[i]);
        S += AnsiString(str);
    }
    StringBinaryOut(S);
    wc.ShowSMLBinary(wc.LocalBuffer.data(), static_cast<int>(wc.LocalLength_4));
}
// AI(W906-uHGemEquipment-BucketC) 20260717: ADDITIVE overload -- golden's
// zero-arg signature (uHGemEquipment.h:264) forwards to THGem's own WireCodec
// (see uHGemEquipment.h's own note on why the (SecsWireCodec&) overload
// exists: so SendLocalDataFrom can trace the INVOKING codec's buffer).
void THGem::ShowLocalBufferBinaryData()
{
    ShowLocalBufferBinaryData(WireCodec);
}
//---------------------------------------------------------------------------
// V1.0 (golden uHGemEquipment.cpp:1378-1405)
// 依設定是否將要送出的資料 (local) 的 Head 結構資訊印出
//---------------------------------------------------------------------------
void THGem::ShowLocalHeadInfo(SecsWireCodec &wc)
{
    AnsiString S;
    if (GemCheckBoxShowHeadInformation->Checked == true)
    {
        StringOut("");
        S.sprintf("HSMS_Head.Length=%d", wc.LocalLength);
        StringOut(S);

        S.sprintf("HSMS_Head.DeviceID=%d", wc.Local.DeviceID);
        StringOut(S);

        S.sprintf("HSMS_MessageID=S%d F%d", wc.Local.MessageID_S & 0x7f, wc.Local.MessageID_F);
        StringOut(S);
        S.sprintf("WBit=%d", wc.Local.W_Bit);
        StringOut(S);

        S.sprintf("PType=%d", wc.Local.PType);
        StringOut(S);

        S.sprintf("SType=%d", wc.Local.SType);
        StringOut(S);

        S.sprintf("Remote.SystemByte=%d", wc.Local.SystemByte);
        StringOut(S);
        StringOut("");
    }
}
// AI(W906-uHGemEquipment-BucketC) 20260717: ADDITIVE overload -- see
// ShowLocalBufferBinaryData()'s own comment just above for the rationale.
void THGem::ShowLocalHeadInfo()
{
    ShowLocalHeadInfo(WireCodec);
}
//---------------------------------------------------------------------------
// V 1.0 (golden uHGemEquipment.cpp:1985-2107)
// 1.將 Data 透過 TCP/IP 傳送
// 2.show 出送出 stream 的資訊
// 3.應要求將 Head struct 印出
// 3.應要求將 Stram data 以 Hex 印出
//
// AI(W906-uHGemEquipment-BucketC) 20260717: D2 -- parameterized over WHICH
// SecsWireCodec instance to send (`wc`), the REAL golden body translated
// verbatim with this substitution: `bReceiveData`/`LocalBuffer`/
// `LocalLength_4`/`Local` -> `wc.*` (LocalBuffer[i] -> wc.LocalBuffer[i],
// a std::vector<unsigned char>); `ShowSFDescription`/`ShowSML` -> `wc.*`;
// `ShowLocalBufferBinaryData()`/`ShowLocalHeadInfo()` -> the
// `(SecsWireCodec&)` overloads just above, passing `wc`; everything else
// (`bUseClientSocket`, `clientGem`, `srvGem`, `bServoSocketConnect`,
// `bTCPIP_Error`, `StringOut`, `GetTimeInfo`, `TimeString`, `StringOutColor`,
// `csSFCodeResponse`, `SFCodeResponseList`, `TimeLeft`, `T3TimeOut`) stays
// `this->` (implicit) -- all genuinely THGem state, not codec state (see
// this file's routing-table note in the header). `SendBuf(LocalBuffer,
// LocalLength_4)` -> `SendBuf(wc.LocalBuffer.data(), (int)wc.LocalLength_4)`.
// Called by both `SendLocalData()` below (THGem's own WireCodec) and, once
// a future wiring wave installs the identical `SendLocalDataHook` on
// `HSys.MyGem->WireCodec`, HTGem's own S9F7_IllegalData/
// S9F9_TransactionTimerTimeout/LocalAcknowledge composers (uHGemClass.cpp:
// 773-790) -- this is how ONE real send implementation serves both live
// codec instances in this port. Recursion-safe: this method never calls
// `wc.SendLocalData()` -- it IS the sender the hook forwards to.
//---------------------------------------------------------------------------
void THGem::SendLocalDataFrom(SecsWireCodec &wc)
{
    AnsiString S, SFCode;
    wc.bReceiveData = false;

    int SCode = wc.Local.MessageID_S & 0x7f;
    int FCode = wc.Local.MessageID_F;
    StringOutColor = clBlack;

    try   // 20130204 Daver add
    {
        if (bUseClientSocket == true)
        {
            clientGem->Socket->SendBuf(wc.LocalBuffer.data(), static_cast<int>(wc.LocalLength_4));
        }
        else
        {
            if (srvGem->Active == true && bServoSocketConnect == true)
            {
                if (srvGem->Socket->ActiveConnections > 0)
                {
                    if (srvGem->Socket->Connections[0]->Connected)
                    {
                        if (srvGem->Socket->ActiveConnections == 1)
                        {
                            srvGem->Socket->Connections[0]->SendBuf(wc.LocalBuffer.data(), static_cast<int>(wc.LocalLength_4));
                        }
                    }
                }
                else
                {
                    bServoSocketConnect = false;
                    srvGem->Close();
                    srvGem->Open();
                }
            }
            else
            {
                return;
            }
        }
    }
    catch (...)   // Daver 20130308 add
    {
        StringOut("--------------- Exception Start------------------------------------");
        StringOut("clientGem->Socket->SendBuf Fail!!");
        GetTimeInfo();
        S = AnsiString("[Send]    Send Fail!");
        StringOut(S);
        SFCode = wc.ShowSFDescription(static_cast<unsigned char>(wc.LocalBuffer[6] & 0x7f), wc.LocalBuffer[7]);
        MyDBIProcess("Exception", "THGem::SendLocalData - clientGem->Socket->SendBuf Fail!!", SFCode);
        wc.ShowSML(wc.LocalBuffer.data(), static_cast<int>(wc.LocalLength_4));
        bServoSocketConnect = false;
        srvGem->Close();
        srvGem->Open();
        bTCPIP_Error = true;
        StringOut("--------------- Exception End------------------------------------");
        return;
    }

    try
    {
        StringOut("---------------------------------------------------");
        GetTimeInfo();
        S = AnsiString("[Send]    ") + TimeString;
        StringOut(S);

        SFCode = wc.ShowSFDescription(static_cast<unsigned char>(wc.LocalBuffer[6] & 0x7f), wc.LocalBuffer[7]);
        ShowLocalBufferBinaryData(wc);
        ShowLocalHeadInfo(wc);
        wc.ShowSML(wc.LocalBuffer.data(), static_cast<int>(wc.LocalLength_4));
        if ((FCode % 2) == 1 && wc.Local.W_Bit == 1)
        {
            S = AnsiString(SCode) + " " + AnsiString(FCode + 1) + " " + AnsiString(wc.Local.SystemByte);
            // Joseph 20221111 (Jason) 新增 TCriticalSection S
            {
                ScopedAcquire lk(csSFCodeResponse);   // golden :2059/:2067 try/__finally
                SFCodeResponseList->Items->Add(S);
                TimeLeft->Add(T3TimeOut * 10);   // pig 2014.07.28 KYEC_SECS
            }
            // Joseph 20221111 (Jason) 新增 TCriticalSection E
        }
    }
    catch (...)   // Daver 20130308 add
    {
        StringOut("--------------- Exception Start------------------------------------");
        GetTimeInfo();
        S = AnsiString("[Send]    Send Fail!");
        StringOut(S);
        SFCode = wc.ShowSFDescription(static_cast<unsigned char>(wc.LocalBuffer[6] & 0x7f), wc.LocalBuffer[7]);
        MyDBIProcess("Exception", "THGem::SendLocalData", SFCode);
        wc.ShowSML(wc.LocalBuffer.data(), static_cast<int>(wc.LocalLength_4));
        bServoSocketConnect = false;
        srvGem->Close();
        srvGem->Open();
        bTCPIP_Error = true;
        StringOut("--------------- Exception End------------------------------------");
        return;
    }
    //==========================================================================
}
// golden public signature (.h:318) -- forwards to SendLocalDataFrom over
// THGem's own embedded WireCodec.
void THGem::SendLocalData()
{
    SendLocalDataFrom(WireCodec);
}
// golden uHGemEquipment.cpp:8997-9006, `void reverae_array(...)` -- a bare
// file-scope helper (byte-array reversal), NOT a THGem method. DEAD in
// golden itself (zero callers anywhere in the 9353-line file -- grepped);
// not translated.
//---------------------------------------------------------------------------
// (golden uHGemEquipment.cpp:9008-9028)
//---------------------------------------------------------------------------
void __fastcall THGem::clientGemRead(TObject *Sender, TCustomWinSocket *Socket)
{
    (void)Sender;
    ScopedAcquire lk(pLockOnSocketRecvice);   // golden :9010/:9026 try/__finally -- 16.10.05.00 Roy Add (Debug from kirin)
    unsigned char *EthernetBuffer;   // shadows THGem::EthernetBuffer -- golden quirk, see that member's own header comment
    int iBufferLenght = 0;
    iBufferLenght = Socket->ReceiveLength();
    if (iBufferLenght > 0)   // 13.09.30.01 klutter
    {
        EthernetBuffer = new unsigned char[iBufferLenght];   // checked
        Socket->ReceiveBuf(EthernetBuffer, iBufferLenght);
        RecvMemoryBuffer->WriteBuffer(EthernetBuffer, iBufferLenght);
        // AI(W906-uHGemEquipment-BucketC) 20260717: GOLDEN BUG, preserved
        // verbatim -- this `new[]` block is NEVER deleted (golden's own
        // comment here, 16.09.14.00 Roy Add, roughly "do NOT delete here!!"
        // -- RecvMemoryBuffer takes its own COPY via WriteBuffer, but the
        // ORIGINAL allocation above leaks on every call). NOT fixed here.
        //delete []EthernetBuffer;
    }
}
//------------------------------------------------------------------------------
// (golden uHGemEquipment.cpp:9030-9207)
//------------------------------------------------------------------------------
void __fastcall THGem::ProcessSocketReceiveData()
{
    AnsiString S = "", SFCode;
    char str[64];
    int i, ret;
    unsigned char *EthernetBuffer;   // shadows THGem::EthernetBuffer -- golden quirk, see that member's own header comment (SAME shadow as clientGemRead's own local above)
    unsigned char *TempEthernetBuffer = NULL;
    unsigned char *ProcessBuffer;
    int Value, iBufferLenght = 0;
    int TempLen = 0;
    unsigned char *p;
    static bool bInProcess = false;   // AI(W906-uHGemEquipment-BucketC) 20260717: GOLDEN QUIRK, preserved -- function-static -> single-instance assumption; a second live THGem would share this guard. NOT fixed here.
    int iProcCount = 0;
    AnsiString asFileName = "";

    if (bInProcess)
        return;

    bInProcess = true;

    if (srvGem->Socket->ActiveConnections > 1)   // 2013/09/30 lee
    {
        bInProcess = false;
        return;   // 2013/09/30 lee
    }

    if (RecvMemoryBuffer->Size == 0 && ProcBuffer->Size == 0)
    {
        bInProcess = false;
        return;
    }

    // AI(W906-uHGemEquipment-BucketC) 20260717: GOLDEN RACE, preserved
    // verbatim -- RecvMemoryBuffer is read/cleared below WITHOUT taking
    // pLockOnSocketRecvice, racing clientGemRead's own LOCKED write to the
    // same stream. NOT fixed here (matches golden exactly; harmless in this
    // port's single-threaded Sim tests, a real concern under Real-mode
    // sockets -- see this wave's own risk note).
    if (RecvMemoryBuffer->Size > 0)
    {
        TempLen = RecvMemoryBuffer->Size;
        TempEthernetBuffer = new unsigned char[TempLen];
        RecvMemoryBuffer->Position = 0;
        RecvMemoryBuffer->ReadBuffer(TempEthernetBuffer, TempLen);
        RecvMemoryBuffer->Clear();

        ProcBuffer->Seek(0, soFromEnd);
        ProcBuffer->WriteBuffer(TempEthernetBuffer, TempLen);

        iFileCount++;
        if (iFileCount == 0x7FFFFFFF)   // 16.11.04.01 s kirin
        {
            iFileCount = 0;
        }   // 16.11.04.01 e kirin
    }

    iBufferLenght = ProcBuffer->Size;
    EthernetBuffer = new unsigned char[iBufferLenght];   // test
    ProcBuffer->Seek(0, soFromBeginning);
    ProcBuffer->ReadBuffer(EthernetBuffer, iBufferLenght);

    do
    {
        WireCodec.SReceiveData->Clear();   // 16.11.04.01 kirin
        WireCodec.bReceiveData = true;     // 16.11.04.01 kirin
        {
            if ((iBufferLenght) < 14)   // 已收長度小於14 (因為 Size byte=4 ,Head Byte=10 至少要 14 byte) 2013/04/11 Lee
            {
                if (iBufferLenght != 0)
                {
                    ProcBuffer->Clear();   // test
                    StringOut(AnsiString("Err : Socket Buffer Length less than 14 byte "));
                    bInProcess = false;
                    return;
                }
                else
                {
                    ProcBuffer->Clear();   // test
                    bInProcess = false;
                    return;
                }
            }
            p = (unsigned char *)&Value;
            for (i = 0; i < 4; i++)
                p[3 - i] = EthernetBuffer[i + iProcCount];   // Value=收到這筆傳送的總長度

            if (Value > 104857600)   // 若傳送的 size 封包長度大於100MB則程式是不正常
            {
                ProcBuffer->Clear();   // test
                StringOut(AnsiString("Err : Size Length over 100M byte "));
                bInProcess = false;
                return;
            }
            else if ((iBufferLenght - iProcCount) >= (Value + 4))   // 至少有一筆資料進來了
            {
                // AI(W906-uHGemEquipment-BucketC) 20260717: GOLDEN LEAK (a),
                // preserved verbatim -- `ProcessBuffer` is `new`'d fresh on
                // every message and never `delete[]`'d anywhere in this
                // function. NOT fixed here.
                ProcessBuffer = new unsigned char[Value + 4];
                memcpy(ProcessBuffer, &EthernetBuffer[iProcCount], Value + 4);
                iProcCount += (Value + 4);
            }
            else if ((iBufferLenght - iProcCount) < (Value + 4))   // 若 Buffer 內資料不夠要傳的長度,表示會再切割(每個封包長度不一定) 16.11.02.01 kirin test
            {
                TempProcBuffer->Position = 0;
                TempProcBuffer->WriteBuffer(&EthernetBuffer[iProcCount], iBufferLenght - iProcCount);
                ProcBuffer->Clear();
                TempProcBuffer->Position = 0;
                ProcBuffer->LoadFromStream(TempProcBuffer);
                TempProcBuffer->Clear();
                bFirstBlock = false;
                delete[] EthernetBuffer;   // 13.09.25.01 klutter
                EthernetBuffer = NULL;     // test
                bInProcess = false;
                return;
            }
        }

        StringOut("===================================================");
        GetTimeInfo();
        S = AnsiString("[Receive] ") + TimeString;
        StringOut(S);

        SFCode = WireCodec.ShowSFDescription(static_cast<unsigned char>(ProcessBuffer[6] & 0x7f), ProcessBuffer[7]);   // Steven 20180815

        bDataFormatOK = true;
        if (GemCheckBoxShowBinary->Checked == true)   // Steven 20211109 : SECS不再全部顯示binary code
        {
            StringBinaryOut("");
            S = "";
            for (i = 0; i < 4; i++)   // 長度4碼
            {
                sprintf(str, "%02X,", ProcessBuffer[i]);
                S += AnsiString(str);
            }
            StringBinaryOut(S);
            S = "";
            for (i = 4; i < 14; i++)   // SF Code 10碼
            {
                sprintf(str, "%02X,", ProcessBuffer[i]);
                S += AnsiString(str);
            }
            StringBinaryOut(S);
        }
        // Ifor 20260402: Always validate SML format (S9F7 detection must not depend on checkbox)
        ret = WireCodec.ShowSMLBinary(ProcessBuffer, Value + 4);
        if (ret == -2)
            bDataFormatOK = false;
        WireCodec.ProcessRemoteHead(ProcessBuffer);
        WireCodec.SReceiveData->Clear();

        if ((WireCodec.Remote.MessageID_S == 101 && WireCodec.Remote.MessageID_F == 5) ||   // pig 2014.04.01 ASEM SECS
            (WireCodec.Remote.MessageID_S == 101 && WireCodec.Remote.MessageID_F == 11))
            WireCodec.bDisableBinaryShow = true;

        WireCodec.ShowSML(ProcessBuffer, Value + 4);

        WireCodec.bDisableBinaryShow = false;
        // Ifor 20260421: S9F3/S9F5 - Wrap ProcessReceiceData in try-catch to prevent communication thread crash
        try
        {
            ProcessReceiceData();
        }
        catch (...)
        {
            AnsiString sExErr;
            sExErr.sprintf("Exception in ProcessReceiceData S%dF%d", WireCodec.Remote.MessageID_S, WireCodec.Remote.MessageID_F);
            StringOut(sExErr);
            // Unhandled exception: report S9F7 to Host and keep connection alive.
            // D3 null-guard (golden :9190) -- see DoConnect's own note.
            if (HSys.MyGem != NULL)
                HSys.MyGem->S9F7_IllegalData(sExErr);
        }
        SaveSECSGEMTextToLog();

        if ((iBufferLenght - iProcCount) == 0)
        {
            if (bFirstBlock == true)
            {
                // AI(W906-uHGemEquipment-BucketC) 20260717: GOLDEN LEAK (b),
                // preserved verbatim -- `EthernetBuffer` is set NULL here
                // WITHOUT a preceding `delete[]` (the :9132 delete above only
                // covers the partial-frame path). NOT fixed here.
                EthernetBuffer = NULL;   // test
            }
            ProcBuffer->Clear();   // test
            WireCodec.bReceiveData = false;
            bFirstBlock = true;
            bInProcess = false;
            return;
        }
    } while (1);
}

//---------------------------------------------------------------------------
// V 1.0
// update application screen online,offline,connect status and so on
// (golden uHGemEquipment.cpp:4747-4985)
//---------------------------------------------------------------------------
void THGem::DoUpdateStatus()
{
    int &ct = ctUpdateStatus;

    AnsiString S;
    static int iControlState = 0;
    static bool bHasDisconnection = false;         // Ifor 20180913 (Steven) : add KYEC SECS GEM Connection狀態由OffLine離開時，等待30秒送出斷線命令後等待上層系統重新連線
    static unsigned char OldGemControlState = 1;   // JerryYang 20230204 : SECS/GEM GControl State轉換的處理，這邊Event report

    ct++;
    if (ct < 10)
        return;
    ct = 0;
    SECSCommunicationMode = static_cast<char>(ComboBox1->ItemIndex);

    if (SECSConnectionState != NULL)
    {
        if (bUseClientSocket == true)
        {
            if (flag1UpdateStatus || bClientSocketActive != clientGem->Active)
            {
                bClientSocketActive = clientGem->Active;
                if (bClientSocketActive == true)
                {
                    SECSConnectionState->Caption = "SECS GEM Connection";
                    SECSConnectionState->Color = clLime;
                    bConnect = false;
                }
                else
                {
                    SECSConnectionState->Caption = "SECS GEM Disconnection";
                    SECSConnectionState->Color = clRed;
                }
            }
            flag1UpdateStatus = false;
        }
        else
        {
            if (iServoConnectCT != srvGem->Socket->ActiveConnections)
            {
                iServoConnectCT = srvGem->Socket->ActiveConnections;
                if (iServoConnectCT == 0)
                {
                    SECSConnectionState->Caption = "SECS GEM Disconnection";
                    SECSConnectionState->Color = clRed;
                    bConnect = false;
                }
                else
                {
                    SECSConnectionState->Caption = "SECS GEM Connection";
                    SECSConnectionState->Color = clLime;
                }
            }
        }
        // Ifor 20180913 (Steven) : add KYEC SECS GEM Connection狀態由OffLine離開時，等待30秒送出斷線命令後等待上層系統重新連線
        //==>
        if (CUSTOMER_CODE == CC_KYEC_LEE &&
            SECSConnectionState->Caption == "SECS GEM Connection" &&
            GEMCommunicatingState->Caption == "1:OffLine")
        {
            if (bHasDisconnection == true && bSECSGEM_DoSeparate == false)
            {
                bHasDisconnection = false;
                SECSGEM_DoSeparate.SetSecAndOn(30);
            }

            // AI(W906-uHGemEquipment-BucketC) 20260717: FIXED -- `SECSGEM_DoSeparate`
            // is now the REAL, TU-local elapsed-time HTimer (D6; see its own
            // definition/comment above, ported verbatim from
            // D:\HT9045\elec\Component\htimer.cpp). Off() genuinely tracks
            // GetTickCount() now, so the 30-second duration armed by
            // SetSecAndOn(30) above is honored for real -- this branch no
            // longer fires on the same poll it was armed (the prior Bucket-B
            // FLAGGED LIMITATION, an always-fires stub, is resolved by this
            // wave). test_do_update_status (tests/test_uHGemEquipment.cpp)
            // still only covers this STRUCTURALLY within a synchronous test
            // (it cannot literally sleep 30s) -- see test [22]'s own updated
            // comment.
            if (SECSGEM_DoSeparate.Off())
            {
                DoSeparate();
                try
                {
                    HGem->srvGem->Close();
                    HGem->clientGem->Close();
                }
                catch (...)
                {
                    MyDBIProcess("Exception", "THGem::DoUpdateStatus");
                }
                bSECSGEM_DoSeparate = true;
                bHasDisconnection = true;
            }
        }
        else
        {
            bHasDisconnection = true;
        }
        //<==
    }

    if (GEMCommunicatingState != NULL)
    {
        if (bOldConnect != bConnect || flag2UpdateStatus)
        {
            bOldConnect = bConnect;
            if (bConnect == true)
            {
                if (CUSTOMER_CODE == CC_KYEC_LEE ||             // Ifor 20170531 (wei) KYEC 楊教授要求顯示連線狀態以利人員判斷
                    CUSTOMER_CODE == CC_SIGURD_ChungXing)        // Sam 20250606 : 矽格中興也要變更SECSGEM狀態顯示
                {
                    GEMCommunicatingState->Caption = "4:OnLine";
                }
                else
                {
                    GEMCommunicatingState->Caption = "4:Enable";
                }
                GEMCommunicatingState->Color = clLime;
                bSECSGEMbyPass = false;             // wei 20150817 SECSGEM 斷線後補入密碼
                bSECSGEMConnectionFail = false;      // wei 20150817 SECSGEM 斷線
            }
            else
            {
                if (CUSTOMER_CODE == CC_KYEC_LEE ||
                    CUSTOMER_CODE == CC_SIGURD_ChungXing)
                {
                    GEMCommunicatingState->Caption = "1:OffLine";
                }
                else
                {
                    GEMCommunicatingState->Caption = "1:Disable";
                }
                GEMCommunicatingState->Color = clRed;
                bSECSGEMConnectionFail = true;       // wei 20150817 SECSGEM 斷線
                bHasDisconnection = true;
            }
        }
        flag2UpdateStatus = false;
    }

    if (GemPanelControlState != NULL)
    {
        if (bOnLine)
        {
            if (bOnLineLocal)
                S = "On Line Local";
            else
                S = "On Line Remote";
        }
        else
        {
            S = "Off Line";
        }

        if (flag3UpdateStatus || S != OldSUpdateStatus)
        {
            OldSUpdateStatus = S;
            flag3UpdateStatus = false;
            GemPanelControlState->Caption = S;
            if (S == "On Line Local")
                GemPanelControlState->Color = clLime;
            else if (S == "On Line Remote")
                GemPanelControlState->Color = clYellow;
            else
                GemPanelControlState->Color = clRed;
        }
    }

    if (IsOnLine())
    {
        GemBtnOfflineRequest->Enabled = true;
        GemBtnOnlineRequest->Enabled = false;
    }
    else
    {
        GemBtnOfflineRequest->Enabled = false;
        GemBtnOnlineRequest->Enabled = true;
    }

    if (GetOnLineMode())
    {
        GemBtnOnlineRemote->Enabled = true;
        GemBtnOnlineLocal->Enabled = false;
    }
    else
    {
        GemBtnOnlineRemote->Enabled = false;
        GemBtnOnlineLocal->Enabled = true;
    }
    // Ifor 20170803 (wei) Mark 避免斷電的系統延遲造成UPH下降 -- golden's own
    // DB->Lines trim-to-3000-lines block here (uHGemEquipment.cpp:4928-4942)
    // is ITSELF commented out in golden (dead code in golden, not a
    // translation gap) -- not translated, matching golden exactly.

    if (bConnectUpdateStatus != IsConnect())
    {
        bConnectUpdateStatus = IsConnect();
        BtnEnableComm->Enabled = !IsConnect();
    }

    if (bOnLine == false)
    {
        GemControlState = 1;
    }
    else
    {
        if (bOnLineLocal == true)
            GemControlState = 2;
        else
            GemControlState = 3;
    }

    if (iControlState != GemControlState)
    {
        GemControlPreState = static_cast<unsigned char>(iControlState);
        iControlState = GemControlState;
        EventReport(141);   // Ifor 20221018 add: GEM Control State Change Report
    }

    if (OldGemControlState != GemControlState)   // JerryYang 20230204 : SECS/GEM GControl State轉換的處理，這邊Event report
    {
        OldGemControlState = GemControlState;
        if (GemControlState == 1)
        {
            EventReport(91);   // Offline
        }
        else if (GemControlState == 2)
        {
            EventReport(92);   // Online local
        }
        else if (GemControlState == 3)
        {
            EventReport(93);   // Online remote
        }
    }
}

//---------------------------------------------------------------------------
// V 1.0
// 顯示接收或傳送的資料 (golden uHGemEquipment.cpp:5165-5173)
//---------------------------------------------------------------------------
void __fastcall THGem::ProcessShow()
{
    // AI(W906-uHGemEquipment-BucketB) 20260717: golden's own body
    // dereferences the GLOBAL `HGem->` pointer here, NOT an implicit
    // `this->` (uHGemEquipment.cpp:5167-5171) -- preserved verbatim (see
    // uHGemEquipment.h's own `extern THGem *HGem;` comment for why); a
    // caller/test must set `HGem = &instance;` first.
    if (HGem->WaitShowString->Count != 0)
    {
        HGem->DB->Clear();
        HGem->DB->Lines->Assign(HGem->WaitShowString);
        HGem->WaitShowString->Clear();
    }
}

//---------------------------------------------------------------------------
// (golden uHGemEquipment.cpp:5176-5527) -- the master SECS/GEM connection
// state machine. Translated verbatim, switch arm by arm, against golden
// 5252-5525; the 5 near-identical bReceiveMultiConnect blocks (golden
// :5270/:5321/:5412/:5447/:5504) are kept as 5 separate copies, NOT factored
// into a shared helper (per this wave's own risk note -- a refactor here
// would be an unrequested behavior-preserving-in-theory change to code this
// migration's whole discipline is to leave alone). The Big5 message string
// ("有2台以上EAP連接Handler,請確認", golden's own 13.09.30.01 klutter comment)
// is the SAME UTF-8 string already committed at srvGemClientConnect's own
// TerminalMemoPtr->Lines->Add call above -- reused verbatim here rather than
// re-decoded, for zero mojibake risk.
//---------------------------------------------------------------------------
void __fastcall THGem::Timer1Timer(TObject *Sender)
{
    (void)Sender;
    static bool bSendDoSeparate = false;
    static bool bSetDoSeparateWait = true;   // Ifor 20180913 (Steven) : add 設定等待時間
    static bool bWaitHTimer = false;          // Ifor 20180913 (Steven) : add 等待等待時間
    static bool bTimerRunning = false;
    if (InitialOK == false || bTimerRunning == true)
    {
        return;
    }
    bTimerRunning = true;

    if (IniConfig.bEnable_SECS_GEM == true)   // Ifor 20170425 add 打開 SECS GEM Function 需先下斷線命令
    {
        bSendDoSeparate = false;
    }
    else
    {
        if (srvGem->Socket->ActiveConnections != 0)
        {
            if (bSendDoSeparate == false)
            {
                bSendDoSeparate = true;
                DoSeparate();
                try
                {
                    HGem->srvGem->Close();
                    HGem->clientGem->Close();
                }
                catch (...)
                {
                    MyDBIProcess("Exception", "THGem::Timer1Timer");
                }
                bSECSGEM_DoSeparate = true;
                bSetDoSeparateWait = true;
            }
        }
        bTimerRunning = false;
        return;
    }

    if (bSetDoSeparateWait && bSECSGEM_DoSeparate)   // Ifor 20180913 (Steven) : add SECS GEM 斷線等待期間避免造成無法連線的問題
    {
        bSetDoSeparateWait = false;
        SECSGEM_DoSeparateWait.SetSecAndOn(5);
        bWaitHTimer = true;
    }

    if (bWaitHTimer)
    {
        // AI(W906-uHGemEquipment-BucketC) 20260717: this 5-second window is
        // now REAL (D6 -- SECSGEM_DoSeparateWait is the real elapsed-time
        // HTimer, not the old always-fires stub); Off() genuinely waits.
        if (SECSGEM_DoSeparateWait.Off())
        {
            bSetDoSeparateWait = true;
            bSECSGEM_DoSeparate = false;
            bWaitHTimer = false;
        }
        else
        {
            bTimerRunning = false;
            return;
        }
    }

    int &Task = Timer1Task;
    int &ct = Timer1ct;
    int ret;
    if (bFirstEntry == true)
    {
        InitialHGem();
        bFirstEntry = false;
    }
    ProcessShow();
    ProcessSocketReceiveData();
    GetTimeInfo();
    DoUpdateStatus();
    Caption = Alias;
    switch (Task)
    {
        case 1:
            DelayForServoError.TimerSet(100);
            DelayForServoError.TimerOn();
            Task = 10;
            break;
        case 10:
            if (DoOpenCommuncation())
            {
                if (EnableOrDisablePtr != NULL)
                {
                    if (EnableOrDisablePtr->ItemIndex == 0)
                        Connect();
                }
                Task = 100;
            }

            if (bReceiveMultiConnect == true && DelayForServoError.TimerOff())
            {
                try
                {
                    if (bUseClientSocket == false)
                        srvGem->Close();
                }
                catch (...)
                {
                    MyDBIProcess("Exception", "THGem::Timer1Timer");
                }
                bReceiveMultiConnect = false;
                iOpenCommuncationTask = 1;
                Task = 1;
                StringOut("有2台以上EAP連接Handler,請確認");   // 13.09.30.01 klutter
            }
            break;
        case 100:
            DelayForServoError.TimerSet(100);
            DelayForServoError.TimerOn();
            Task = 110;
            break;
        case 110:
            if (CheckSocketActiveFalse())
            {
                iOpenCommuncationTask = 1;
                Task = 1;
                break;
            }

            if (bUseClientSocket && (bStartConnect == true || bAutoConnect == true))
            {
                bStartConnect = false;
                iStartConnectTask = 1;
                Task = 200;
            }
            else if (bReceiveEstablishCommunicationsRequest == true)
            {
                bReceiveEstablishCommunicationsRequest = false;
                bConnect = true;
                if (bOnLine)
                {
                    iStartOnLineTask = 1;
                    Task = 300;
                }
                else
                {
                    Task = 250;
                }
            }

            if (bReceiveMultiConnect == true && DelayForServoError.TimerOff())
            {
                try
                {
                    if (bUseClientSocket == false)
                        srvGem->Close();
                }
                catch (...)
                {
                    MyDBIProcess("Exception", "THGem::Timer1Timer");
                }
                bReceiveMultiConnect = false;
                iOpenCommuncationTask = 1;
                Task = 1;
                StringOut("有2台以上EAP連接Handler,請確認");   // 13.09.30.01 klutter
            }
            break;
        case 200:
            if (CheckSocketActiveFalse())
            {
                Task = 1;
                break;
            }

            if (bReceiveEstablishCommunicationsRequest == true)
            {
                bReceiveEstablishCommunicationsRequest = false;
                bConnect = true;
                if (bOnLine)
                {
                    iStartOnLineTask = 1;
                    Task = 300;
                }
                else
                {
                    Task = 250;
                }
                break;
            }

            ret = DoConnect();
            if (ret == 1)
            {
                if (OnLineOrOffLine != NULL)
                {
                    if (OnLineOrOffLine->ItemIndex == 0)
                        OnLine(0);
                    else
                        OffLine();
                }

                if (bOnLine)
                {
                    iStartOnLineTask = 1;
                    Task = 300;
                }
                else
                {
                    Task = 250;
                }
            }
            else if (ret == 2)
                Task = 100;
            break;
        case 250:
            DelayForServoError.TimerSet(100);
            DelayForServoError.TimerOn();
            Task = 260;
            break;
        case 260:
            if (CheckSocketActiveFalse())
            {
                Task = 1;
                break;
            }
            else if (bConnect == false)
            {
                Task = 100;
                break;
            }

            if (bOnLine)
            {
                iStartOnLineTask = 1;
                Task = 300;
            }
            else if (bStartOnLine == true)
            {
                Task = 300;
            }

            if (bReceiveMultiConnect == true && DelayForServoError.TimerOff())
            {
                try
                {
                    if (bUseClientSocket == false)
                        srvGem->Close();
                }
                catch (...)
                {
                    MyDBIProcess("Exception", "THGem::Timer1Timer");
                }
                bReceiveMultiConnect = false;
                iOpenCommuncationTask = 1;
                Task = 1;
                StringOut("有2台以上EAP連接Handler,請確認");   // 13.09.30.01 klutter
            }
            break;
        case 300:
            DelayForServoError.TimerSet(100);
            DelayForServoError.TimerOn();
            Task = 310;
            break;
        case 310:
            bReceiveEstablishCommunicationsRequest = false;
            if (CheckSocketActiveFalse())
            {
                Task = 1;
                break;
            }

            if (DoOnLine() == true)
            {
                Task = 400;
            }

            if (bReceiveMultiConnect == true && DelayForServoError.TimerOff())
            {
                try
                {
                    if (bUseClientSocket == false)
                        srvGem->Close();
                }
                catch (...)
                {
                    MyDBIProcess("Exception", "THGem::Timer1Timer");
                }
                bReceiveMultiConnect = false;
                iOpenCommuncationTask = 1;
                Task = 1;
                StringOut("有2台以上EAP連接Handler,請確認");   // 13.09.30.01 klutter
            }
            break;
        case 350:
            // AI(W906-uHGemEquipment-BucketC) 20260717: golden itself never
            // assigns Task=350 anywhere (grepped) -- this arm appears to be
            // dead/unreachable code in golden. Translated verbatim anyway,
            // per this wave's own "translate the switch arm by arm" risk
            // note -- NOT removed, NOT "fixed".
            if (RemoteOrLocal->ItemIndex == 0)
                OnLineRemote();
            else
                OnLineLocal();
            Task = 360;
            ct = 0;
            break;
        case 360:
            ct++;
            if (ct > 30)
                Task = 400;
            break;
        case 400:
            DelayForServoError.TimerSet(100);
            DelayForServoError.TimerOn();
            Task = 410;
            break;
        case 410:
            bReceiveEstablishCommunicationsRequest = false;
            if (CheckSocketActiveFalse())
            {
                Task = 1;
                break;
            }

            if (bCloseCommuncation == true)
            {
                bCloseCommuncation = false;
                Task = 1;
            }
            else if (bConnect == false)
            {
                Task = 100;
            }
            else if (bOnLine == false)
            {
                iStartOnLineTask = 1;
                Task = 250;
            }
            else if (bReceiveMultiConnect == true && DelayForServoError.TimerOff())
            {
                try
                {
                    if (bUseClientSocket == false)
                        srvGem->Close();
                }
                catch (...)
                {
                    MyDBIProcess("Exception", "THGem::Timer1Timer");
                }
                bReceiveMultiConnect = false;
                iOpenCommuncationTask = 1;
                Task = 1;
                StringOut("有2台以上EAP連接Handler,請確認");   // 13.09.30.01 klutter
            }
            else
            {
                DoLocalAllProcessLoop();
            }
            break;
    }
    bTimerRunning = false;
}

//---------------------------------------------------------------------------
// 2013/05/27
// V1.1 (golden uHGemEquipment.cpp:6887-6892)
//---------------------------------------------------------------------------
void THGem::FormClose()
{
    // AI(W906-uHGemEquipment-BucketB) 20260717: golden's own signature is
    // `void __fastcall THGem::FormClose(TObject *Sender, TCloseAction
    // &Action)` -- both parameters are UNUSED in golden's own body (only
    // SaveSystemDefault()/SaveEventReportData()/bShow=false; run here) and no
    // TCloseAction stand-in exists anywhere in this tree; dropped rather than
    // inventing a new type for 2 always-unused parameters -- a documented,
    // acceptable signature gap (same class of deviation this wave's own
    // brief already accepted for ManualCreatergRoleClick's OnClick wiring).
    SaveSystemDefault();
    SaveEventReportData();
    bShow = false;
}

//---------------------------------------------------------------------------
// 2013/05/27
// V1.1 (golden uHGemEquipment.cpp:6915-6931)
//---------------------------------------------------------------------------
void __fastcall THGem::ManualCreatergRoleClick(TObject *Sender)
{
    (void)Sender;
    // AI(W906-uHGemEquipment-BucketB) 20260717: golden's own ctor assigns
    // `rgRole->OnClick=ManualCreatergRoleClick;` dynamically (golden ctor
    // :653, NOT via the .dfm's static event table) -- NOT reproduced here:
    // THGemRadioGroup (this wave's minimal stand-in) has no OnClick callback
    // slot/observer mechanism, so this method is a real, directly-callable
    // method but is never auto-fired by an ItemIndex change. A documented,
    // acceptable gap (per this wave's own brief) -- call it explicitly after
    // changing rgRole->ItemIndex to get golden's real-world behavior.
    if (bShow == true)
    {
        Gated_ShowMessage("Need Restart Program After Modify !!!");

        if (rgRole->ItemIndex == 0)
            bUseClientSocket = false;
        else
            bUseClientSocket = true;

        if (bUseClientSocket == true)
            edtIP->Enabled = true;
        else
            edtIP->Enabled = false;
    }
}

//---------------------------------------------------------------------------
// 2013/05/27
// V1.1 (golden uHGemEquipment.cpp:6936-6950)
//---------------------------------------------------------------------------
void __fastcall THGem::FormShow(TObject *Sender)
{
    (void)Sender;
    // AI(W906-uHGemEquipment-BucketB) 20260717: golden's own
    // `Left=(1280-Width)/2; Top=(1024-Height)/2;` (window-centering) and
    // `PageControl1->ActivePageIndex=0;` (tab reset) are OMITTED -- THGem is
    // not modeled as a real window/form in this port (no Left/Top/Width/
    // Height/PageControl1 members exist), so translating these would be
    // inert busywork with nothing to observably affect. The 2 genuine
    // non-cosmetic pieces below ARE translated: bShow (read by
    // ManualCreatergRoleClick's own guard) and the KYEC-specific
    // OnLineOrOffLine/RemoteOrLocal lock (real customer-specific UI-lock
    // logic, not mere cosmetics).
    bShow = true;

    cbECChaneEventReport->Visible = CosFunction.bECChangeEventReportCanOnOff;   // JerryYang 20200520 客戶提出DoReportECDataChangeCheck函式會影響UPH,改成功能選項

    //==> Eastsun 20260526 #026-1.78 Ifor 20210527 add: KYEC SECSGEM 強制Online Remote, UI鎖定
    if (CUSTOMER_CODE == CC_KYEC_LEE)
    {
        OnLineOrOffLine->Enabled = false;
        RemoteOrLocal->Enabled = false;
    }
    //<== Eastsun 20260526 #026-1.78
}
