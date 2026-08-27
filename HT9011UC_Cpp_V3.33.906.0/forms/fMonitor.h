// =============================================================================
//  forms/fMonitor.h  --  non-VCL facade for golden's TfMonitor / fMonitor
//                         (CCD/video monitor connection + relay-command form)
//
//  AI(W906-FW3-OWM1) 20260828: new file. Wave FW3-OWM1 (golden
//  Monitor/MonitorInterface.cpp, HT9011UC_Code_V3.33.906.0_20260618,
//  cp950-decoded with `python -c "open(p,encoding='cp950').read()"`,
//  0 U+FFFD). Denominator (tools/census/wave_preflight.py, 20260828): 23
//  bodies = 23 TfMonitor:: members + 0 file-scope, 391 span lines.
//
//  WHY THIS FILE MATTERS BEYOND ITS OWN LOGIC: TfMonitor/fMonitor were BOTH
//  completely absent from the port before this wave (class name + global
//  name both "clear" per wave_preflight's shim-occupation scan, 20260828 --
//  the 36 `(b)` hits it found are ALL unrelated OTHER classes' FormShow/
//  FormClose/FormDestroy overloads, not an occupation). 3 predetermined
//  names are referenced by #if 0 call sites in csystem.cpp: `OpenMonitorVedio`/
//  `StopMonitorVedio` (multiple sites) and `iStatus`. All 3 are declared
//  below with matching name+type; un-gating those call sites is a separate,
//  safety-relevant decision left to whoever owns csystem.cpp.
//
//  WHY THIS FILE IS THIN: golden's whole point is a live TCP video-relay link
//  (MVCtrl, a MonitorTCPIP*) that opens/polls/commands a CCD IPC over the
//  network and starts/stops per-channel video streaming. MonitorTCPIP.cpp is
//  ALREADY translated (PT-W2) but lives in ht9045_sm (CMakeLists.txt:2334,
//  "reaches MyDBIProcess, so sm rather than comms"); ht9045_forms links only
//  vclcompat+ht9045_globals+ht9045_core (CMakeLists.txt:859) -- NOT one of
//  the 4 verified ht9045_forms->ht9045_sm exceptions either (RecordProcess/
//  ShowMyMessage/ShowErrorMessage/MyDBIProcess -- MonitorTCPIP is a
//  different class, not on that list). This wave leaves MVCtrl permanently
//  NULL (ctor PORT SPLIT below), so every method that calls through it AND
//  every method whose entire purpose IS to command the video relay
//  (OpenMonitorVedio/StopMonitorVedio/GetMonitorVedioState/GetMonitorHDSpec/
//  MonitorTimerTimer/sb*Connect*/sbSendCommandClick) is excluded -- either
//  fully undeclared, or (for the 2 predetermined names) declared with a
//  fully-gated body. What survives is UI-geometry/ini-read/own-widget
//  bookkeeping that never touches MVCtrl, plus the 4 on-screen-keyboard
//  MouseDown delegates.
//
//  WAVE SCOPE (23 bodies; golden line numbers)
//  --------------------------------------------------------------------------
//    TfMonitor ctor                golden :23-45   PORT SPLIT (see .cpp)
//    FormShow                       golden :47-55   ACTIVE verbatim (calls
//                                                    LoadTCPIPParament, itself
//                                                    1-line-gated -- GATE M-1)
//    FormDestroy                    golden :57-72   ACTIVE, 2 segments gated
//                                                    (GATE M-2, GATE M-3)
//    FormClose                      golden :74-77   ACTIVE verbatim
//    LoadTCPIPParament               golden :79-96   ACTIVE, 1 line gated
//                                                    (GATE M-1)
//    SaveTCPIPParament               golden :98-121  EXCLUDED (write path)
//    sbMVUpdateClick                  golden :123-126 EXCLUDED (wraps
//                                                    SaveTCPIPParament)
//    sbMVConnectClick                 golden :128-131 EXCLUDED (comm action)
//    sbMVDisconnectClick              golden :133-136 EXCLUDED (comm action)
//    sbMVExitClick                    golden :138-141 ACTIVE verbatim
//    MonitorTimerTimer                 golden :143-327 EXCLUDED (comm+video,
//                                                    185 lines, see below)
//    sbSendCommandClick                golden :329-332 EXCLUDED (comm action)
//    sbMVLogClearClick                  golden :334-337 ACTIVE verbatim
//    sbMonitor1OpenClick                 golden :339-348 EXCLUDED (wraps
//                                                    Open/StopMonitorVedio)
//    OpenMonitorVedio(int)                golden :350-363 DECLARED, body
//                                                    fully gated (GATE M-4 --
//                                                    predetermined name)
//    StopMonitorVedio(int)                golden :365-378 DECLARED, body
//                                                    fully gated (GATE M-4 --
//                                                    predetermined name)
//    GetMonitorVedioState(int)             golden :380-390 EXCLUDED (comm
//                                                    action, not predetermined)
//    GetMonitorHDSpec()                     golden :392-405 EXCLUDED (comm
//                                                    action, not predetermined)
//    sbMonitorGetHDSpaceClick                golden :407-410 EXCLUDED (wraps
//                                                    GetMonitorHDSpec)
//    edAfterHandlerTrayFeedMonitor1ClosedVideoWaitTimeMouseDown
//                                              golden :412-417 ACTIVE verbatim
//    edWhenHDFullPromptMouseDown                golden :419-423 ACTIVE verbatim
//    edLowHDSpaceMouseDown                       golden :425-429 ACTIVE verbatim
//    edMVPortMouseDown                            golden :431-435 ACTIVE verbatim
//  Total: 11 ACTIVE (3 with an internal gated segment) + 2 DECLARED-but-
//  fully-gated (predetermined names, 0 live lines) + 10 EXCLUDED ENTIRELY = 23/23.
//
//  GATE REGISTER
//  --------------------------------------------------------------------------
//  (M-1) ctor golden :30-33 / LoadTCPIPParament golden :94 -- the ctor's
//      `MVCtrl=new MonitorTCPIP(new TCPClient(MonitorClientSocket));
//      MVCtrl->InitialMemo(MonitorMemo);` is gated OUTRIGHT (MonitorTCPIP's
//      ctor lives in ht9045_sm, not reachable, AND static-init ctors may only
//      `new` their own widget stand-ins -- docs/KNOWLEDGE.md "static-init
//      ctor 不可碰 NULL 全域" / the SIOF trap). The ctor's call to
//      `LoadTCPIPParament()` (golden :32) is ALSO gated -- not because
//      LoadTCPIPParament itself is unsafe (it is translated ACTIVE below),
//      but because eagerly reading a real file
//      (D:\HT9045\system\MVData.ini) as a side effect of every process that
//      links ht9045_forms constructing its static `fMonitor` global, BEFORE
//      main(), is exactly the disk-I/O-at-static-init hazard that rule
//      exists to prevent. LoadTCPIPParament() IS still called live from
//      FormShow (golden :54) -- that is a real UI event, not a hidden
//      global-constructor side effect, and is the intended, safe place for
//      this read to happen once anything wires FormShow. LoadTCPIPParament's
//      OWN trailing line (golden :94, `MVCtrl->InitialSocket(sADDRESS,
//      iPORT);`) is separately gated -- cross-archive call, MVCtrl is
//      permanently NULL this wave.
//  (M-2) FormDestroy golden :62/:64 -- `MVCtrl->Disconnect(); ... delete
//      MVCtrl;` gated as one segment: Disconnect() is a MonitorTCPIP::
//      method (cross-archive, MVCtrl NULL); `delete MVCtrl` on a
//      forward-declared (incomplete) type is additionally ill-formed to
//      compile at all. Rest of the try-block (MonitorTimer->Enabled=false;
//      sComData->Clear(); delete sComData;) stays ACTIVE, and the catch
//      block's `MyDBIProcess("Exception","TfMonitor::FormDestroy");` stays
//      ACTIVE (one of the 4 verified ht9045_forms->ht9045_sm exceptions --
//      CMakeLists.txt:725-731, "forms/fLotInfo.cpp -- already in this same
//      source list -- calls them 4/14/4/2 times... because the final
//      executables link both archives").
//  (M-3) FormDestroy golden :71 -- `LogSoftwareOffTime("TfMonitor,
//      FormDestroy");` gated: its body is acarry_shims.cpp:255 (ht9045_sm,
//      CMakeLists.txt:1978/2523) and it is NOT one of the 4 verified
//      exceptions ("其他 sm／io／db 符號一律退出" -- this wave's own task
//      brief).
//  (M-4) OpenMonitorVedio(int)/StopMonitorVedio(int) -- BOTH predetermined
//      names (csystem.cpp #if 0 call sites) AND both fall squarely in the
//      explicit safety-exclusion category "對外通訊：...影像串流" (they send
//      the literal "SR0%d\r\n"/"SS0%d\r\n" relay commands that start/stop a
//      channel's live video). The two golden guard-checks
//      (`iStatus[index]`/`bCommandReady`/`sComData->Count`) read only
//      already-live, self-owned fields and WOULD compile+run clean by
//      themselves, but have zero observable effect once the send they guard
//      is removed -- gating them too rather than leaving a do-nothing
//      skeleton that reads as functional. Declared (name+signature only) so
//      a future un-gate of csystem.cpp's call sites has something
//      type-correct to bind to.
//
//  DEVIATIONS (port-only adaptations, zero intended behaviour change):
//    (D-1) `__fastcall` + `TComponent* Owner` ctor glue dropped; class is
//        not TForm-derived (established convention, every translated form
//        facade in this tree).
//    (D-2) FormClose: golden `TObject *Sender, TCloseAction &Action` both
//        dropped -- neither read in golden's own body (`bShow=false;` only),
//        and TCloseAction has zero port anywhere in this tree (established
//        precedent, forms/fGroundMan.cpp:257 etc.).
//    (D-3) `TTimer` has no vclcompat port (confirmed: `grep -rn "class
//        TTimer" vclcompat/` -- 0 hits, 20260828, matching the existing
//        TU-local-shim precedent at forms/fGroundMan.h:456 /
//        ATC/ATCInterface.h:189 / BinDisplay/MyBinDisp.h:231 /
//        Automation/uRENESAS_Server.h:168). Added as a minimal facade-local,
//        header-guarded stand-in below (guard name UNIQUE to this file, so
//        it cannot collide with any other form's own TTimer shim UNLESS
//        both headers are ever included in the same translation unit --
//        neither is today). Only `->Enabled` is touched anywhere in this
//        wave's translated scope (ctor, FormDestroy).
//    (D-4) golden private: `bShow` collapsed to public -- established
//        convention (forms/fTemp_Set.h:566-569 citation chain, D-6 in
//        several sibling facades this same wave).
//    (D-5) `Close()` -- golden inherited TForm::Close(), called by
//        sbMVExitClick (golden :140). Added as the same permanent offline
//        no-op every other non-TForm facade in this tree already carries
//        (forms/fContact.h:1603, forms/fMotorTest.h:977,
//        forms/fPassword.h:337, forms/fSmartDiagnostic.h precedent).
//
//  EXCLUDED WIDGETS (golden __published members with NO translated body
//  dereferencing them by name -- forms/fDynamicTemp.h "only what's
//  dereferenced" rule): pnMVConnectTitle, lbMVConnectState, sbMVConnect,
//  sbMVDisconnect, tsMVMain, tsMVSettingPage, pnMVSetupPage, lbMVAddress,
//  lbMVPort, MonitorClientSocket, pnMVButtonTitle, sbMVUpdate, sbMVExit,
//  pnMVMain, edCommandData, sbSendCommand, sbMVLogClear, pnMVState,
//  sbMonitor1Open/2Open/3Open/1Stop/2Stop/3Stop, MyLed1, Memo1,
//  sbMonitorGetHDSpace, tsMonitorSpecific, lbWhenHDFullPrompt -- every one of
//  them is touched ONLY inside an EXCLUDED body above.
//
//  DEPENDENCY AUDIT (this wave, 20260828) -- all grepped before translating:
//    MonitorTCPIP                REAL type, ht9045_sm (Monitor/
//                               MonitorTCPIP.cpp, CMakeLists.txt:2334) --
//                               forward-declared ONLY below (pointer field,
//                               no method call, no `new` -- does not create
//                               a link edge).
//    MyDBIProcess(S1,S2)          REAL  aHotPlateSubstrate.h:933, body
//                               aHotPlateSubstrate.cpp -- ht9045_sm, but ONE
//                               OF THE 4 VERIFIED FORMS->SM EXCEPTIONS
//                               (CMakeLists.txt:725-731). Used in FormDestroy.
//    fQwertyKey / ShowQwertyKey    REAL  forms/fQwertyKey.h:406/:370,
//                               compiled into ht9045_forms
//                               (CMakeLists.txt:699) -- same archive.
//    N_INTEGER                     REAL  cmydef.h:289 (extern const int),
//                               body cmydef.cpp, ht9045_globals
//    N_PORT                         REAL  cmydef.h:296 (extern const int),
//                               body cmydef.cpp, ht9045_globals
//    TEdit/TCheckBox/TMemo/TPageControl/TObject
//                                    REAL  vclcompat/Controls.h /
//                               vclcompat/vcl_compat.h (both global)
//    TIniFile                        REAL  vclcompat/IniFiles.h -- used by
//                               LoadTCPIPParament (Read* only, never Write*).
//
//  CMAKE STATUS -- NOT WIRED by this wave (CMakeLists.txt is outside the
//  write boundary). Suggested landing: forms/fMonitor.cpp -> the
//  ht9045_forms source list (CMakeLists.txt:654 block) -- diet-clean AS
//  TRANSLATED: the only ht9045_sm symbol touched by any COMPILED (non-#if-0)
//  line is MyDBIProcess, which already rides the verified forms->sm
//  exception forms/fLotInfo.cpp established.
// =============================================================================
#ifndef FORMS_FMONITOR_H
#define FORMS_FMONITOR_H

#include "vclcompat/vcl_compat.h"    // AnsiString/TObject/TStringList at global scope
#include "vclcompat/Controls.h"      // TEdit/TCheckBox/TMemo/TPageControl (global)
#include "vclcompat/IniFiles.h"      // TIniFile -- LoadTCPIPParament

using vclcompat::TIniFile;

// Forward-declared ONLY -- see DEPENDENCY AUDIT above. No method of
// MonitorTCPIP is ever called from this file, and it is never `new`'d here
// (golden's `new MonitorTCPIP(...)` is GATE M-1), so the pointer-only field
// below does not need (and must not pull in) the full class definition / its
// ht9045_sm link edge.
class MonitorTCPIP;

// ---------------------------------------------------------------------------
//  TTimer -- facade-local, header-guarded stand-in (see DEVIATION D-3).
//  Only `->Enabled` is touched anywhere in this wave's translated scope.
// ---------------------------------------------------------------------------
#ifndef HT9045_FMONITOR_TTIMER_SHIM
#define HT9045_FMONITOR_TTIMER_SHIM
class TTimer : public vclcompat::TObject
{
public:
    bool Enabled;
    TTimer() : Enabled(false) {}
    virtual ~TTimer() {}
};
#endif

// ===========================================================================
//  TfMonitor -- non-VCL facade (golden Monitor/MonitorInterface.h:19-77,
//  TfMonitor : public TForm)
// ===========================================================================
class TfMonitor
{
public:
    TfMonitor();   // golden ctor :23-45, PORT SPLIT -- see .cpp (GATE M-1)

    // -- golden __published (h:21-63) -- only what a translated body
    // dereferences by name is declared (see EXCLUDED WIDGETS above).
    TPageControl *MVPageControl = new TPageControl();
    TTimer       *MonitorTimer  = new TTimer();
    TEdit        *edMVAddress   = new TEdit();
    TEdit        *edMVPort      = new TEdit();
    TCheckBox    *cbWhenHDFullAlarm = new TCheckBox();
    TEdit        *edLowHDSpace      = new TEdit();
    TEdit        *edWhenHDFullPrompt = new TEdit();
    TCheckBox    *cbAfterHandlerTrayFeedMonitor1ClosedVideo = new TCheckBox();
    TEdit        *edAfterHandlerTrayFeedMonitor1ClosedVideoWaitTime = new TEdit();
    TMemo        *MonitorMemo = new TMemo();

    // -- golden private: User declarations (h:64) ----------------------------
    bool bShow = false;   // (D-4) collapsed to public

    // -- golden public: User declarations (h:66-79) ---------------------------
    // GATE M-1: permanently NULL this wave -- see header banner.
    MonitorTCPIP *MVCtrl = 0;

    bool       bCommandReady = false;
    AnsiString sADDRESS;
    int        iPORT = 0;
    TStringList *sComData = 0;   // `new`'d in the ctor's live prefix, see .cpp
    int        iHDSpace = 0;

    // GATE-REGISTER predetermined name (csystem.cpp, multiple #if 0 sites).
    int iStatus[4] = {0, 0, 0, 0};

    int iDisconnectCount = 0;

    // Form-level plain-int fields (golden TForm::Width/Height/Left/Top) --
    // same D-7-class deviation as forms/fWinway.h/forms/fQwertyKey.h: this
    // facade does not derive TForm.
    int Width  = 0;
    int Height = 0;
    int Left   = 0;
    int Top    = 0;

    void Close() {}   // (D-5) golden TForm::Close() -- offline no-op

    void FormShow();       // golden :47-55, ACTIVE verbatim
    void FormDestroy();    // golden :57-72, ACTIVE (GATE M-2, GATE M-3)
    void FormClose();      // golden :74-77, ACTIVE verbatim (D-2)
    void LoadTCPIPParament();   // golden :79-96, ACTIVE (GATE M-1)
    void sbMVExitClick();       // golden :138-141, ACTIVE verbatim
    void sbMVLogClearClick();   // golden :334-337, ACTIVE verbatim

    void OpenMonitorVedio(int index);   // golden :350-363 (GATE M-4)
    void StopMonitorVedio(int index);   // golden :365-378 (GATE M-4)

    void edAfterHandlerTrayFeedMonitor1ClosedVideoWaitTimeMouseDown(TObject *Sender);   // golden :412-417
    void edWhenHDFullPromptMouseDown(TObject *Sender);                                  // golden :419-423
    void edLowHDSpaceMouseDown(TObject *Sender);                                        // golden :425-429
    void edMVPortMouseDown(TObject *Sender);                                            // golden :431-435
};

// AI(W906-FW3-OWM1) 20260828: golden Monitor/MonitorInterface.h:79 `extern
// PACKAGE TfMonitor *fMonitor;` -- PACKAGE glue dropped. Class name and
// global name were BOTH free tree-wide (wave_preflight, 20260828), so this
// facade declares the global -- same idiom as forms/fCleaning.cpp:56 /
// forms/fMesSystem.cpp:597 / forms/fSmartDiagnostic.cpp:37.
extern TfMonitor *fMonitor;

#endif // FORMS_FMONITOR_H
