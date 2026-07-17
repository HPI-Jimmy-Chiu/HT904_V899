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
#include "cmydef.h"    // CUSTOMER_CODE / CC_KYEC_LEE / CC_SIGURD_ChungXing / CC_MAXIM_THAILAND / CosFunction / bSECSGEMbyPass / bSECSGEMConnectionFail
#include "common.h"    // ReadWriteIni / ReadIniData / WriteIniData
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
      SECSCommunicationMode(0)         // golden ctor :500 (explicitly 0)
{
    try
    {
        strGrdAlarmOld  = new TStringGrid(5, 1);
        strGrdCEID      = new TStringGrid(258, 1025);
        stdGridReportID = new TStringGrid(1026, 257);
        strGrdAlarm     = new TStringGrid(12, 5);

        WaitShowString = new TStringList();   // golden ctor :661-662
        LogDataString  = new TStringList();   // golden ctor :618,624

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
        // OnRead = clientGemRead (golden .dfm:550): clientGemRead is OUT OF
        // SCOPE this wave (needs TMemoryStream/TFixedCriticalSection shims --
        // see this header's own "Do NOT translate" note) -- NOT wired.

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
        // OnClientRead = clientGemRead (golden .dfm:569): same OUT-OF-SCOPE
        // reason as clientGem->OnRead above -- NOT wired.

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
        delete WaitShowString;
        delete LogDataString;
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
    delete WaitShowString;
    delete LogDataString;
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
// HTimer -- TU-local stand-in for golden cpublic.h's forward-declared
// HTimer, matching atester_shims.h's own already-established minimal
// stand-in (`struct HTimer { bool Off(){return true;} void
// SetSecAndOn(double){} };`) -- duplicated here (anonymous-namespace-scoped,
// so zero ODR/collision risk with atester_shims.h's own copy) rather than
// `#include "atester_shims.h"`, which would drag in that file's entire
// unrelated atester/iosetview/rs232/TCOM2 shim surface for a 2-line type.
//
// AI(W906-uHGemEquipment-BucketB) 20260717 -- FLAGGED LIMITATION (see
// DoUpdateStatus's own KYEC branch below for the concrete call site): this
// stand-in's Off() ALWAYS returns true, unconditionally, regardless of what
// duration SetSecAndOn(...) was asked to wait -- it is an ALWAYS-FIRES stub,
// not a real elapsed-time timer (unlike GemTimer above, which really does
// track GetTickCount()). Golden's KYEC-specific 30-second forced-disconnect
// wait (`SECSGEM_DoSeparate.SetSecAndOn(30)` then polling
// `SECSGEM_DoSeparate.Off()`) will therefore fire on the SAME poll it was
// armed on in this translated build -- the real 30-second wait this
// customer's workaround depends on does NOT actually wait 30 seconds yet.
// This is a pre-existing shim limitation (HTimer has never been designed for
// real elapsed-time behavior anywhere in this tree), not something silently
// fixed or hidden here -- a future wave needs to design a real HTimer (real
// GetTickCount()-based elapsed-time semantics, like GemTimer already has)
// before KYEC's disconnect-dance is trustworthy end-to-end. Flagged loudly
// here, at the KYEC branch itself (below), and in this wave's own final
// report.
// ---------------------------------------------------------------------------
struct HTimer
{
    bool Off() { return true; }
    void SetSecAndOn(double) {}
};

} // anonymous namespace

// AI(W906-uHGemEquipment-BucketB) 20260717: golden uHGemEquipment.cpp:24/4746
// -- both are genuine FILE-SCOPE globals in golden itself (NOT THGem
// members), shared by DoUpdateStatus (in THIS wave's scope, below) and
// Timer1Timer (still out of scope -- needs SecsWireCodec embedded first; see
// this header's own "Do NOT translate" list). This wave's own brief
// suggested making these THGem members instead; verified against golden and
// kept as plain globals here, matching golden exactly -- Timer1Timer's own
// future wave will need to read/write the SAME globals, and a header change
// now would only have to be undone/reconciled then for no fidelity benefit.
bool bSECSGEM_DoSeparate = false;   // golden uHGemEquipment.cpp:24
HTimer SECSGEM_DoSeparate;          // golden uHGemEquipment.cpp:4746

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
// golden uHGemEquipment.h:248 `void DoSeparate();` -- GATED NO-OP STUB.
// Real body sends the HSMS Separate.req message over the wire (SML/
// wire-codec family, out of THIS wave's scope -- see this header's own
// "Do NOT translate" list). Exists purely so DoUpdateStatus's own KYEC
// branch (below), which unconditionally calls this, stays translatable
// without silently dropping the call. Does NOT send anything over
// clientGem/srvGem.
//---------------------------------------------------------------------------
void THGem::DoSeparate()
{
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

            // AI(W906-uHGemEquipment-BucketB) 20260717: FLAGGED LIMITATION --
            // `SECSGEM_DoSeparate` is the TU-local HTimer stand-in (see its
            // own definition/comment above), whose Off() ALWAYS returns true
            // immediately, regardless of the 30-second duration just armed by
            // SetSecAndOn(30) on the line above. In THIS translated build,
            // the branch below fires on the very SAME poll it was armed --
            // KYEC's real 30-second forced-disconnect wait does NOT actually
            // wait 30 seconds yet. This is a pre-existing shim limitation
            // (HTimer has never been designed for real elapsed-time behavior
            // anywhere in this tree), not silently fixed or hidden -- a
            // future wave needs to design a real HTimer before this
            // customer's flow is trustworthy end-to-end. See this wave's own
            // final report for the same flag.
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
