// =============================================================================
//  BarCode/BarCode_8CCD_Glue.h  --  8-CCD ClientSocket connection-lifecycle glue
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/BarCode/BarCode.h
//      13 __published TClientSocket* members:
//          ClientSocket_Shuttle1_A / _B          golden BarCode.h:127-128
//          ClientSocket_Shuttle2_A / _B          golden BarCode.h:129-130
//          ClientSocket_BarcodeChangeFile         golden BarCode.h:134
//          ClientSocket_Bottom_1 .. _8            golden BarCode.h:300-307
//      2 __published TTimer* members:
//          TimerBotton8CCDConnect                 golden BarCode.h:308
//          TimerBottom8CCDInitial                 golden BarCode.h:309
//  HT9011UC_Code_V3.33.906.0_20260618/BarCode/BarCode.cpp
//      TfBarCode::SendCCDCommand(int,AnsiString,AnsiString)   golden :5305-5526 (222 lines)
//      TfBarCode::TimerBotton8CCDConnectTimer(TObject*)       golden :9192-9536 (345 lines)
//      TfBarCode::TimerBottom8CCDInitialTimer(TObject*)       golden :9539-9937 (399 lines)
//  (exact bounds re-verified by grep against golden -- the two Timer functions
//   each close one line earlier than the hand-off prompt's rough estimate,
//   9538/9939; see translate report item 8.)
//
//  Translation wave: W5-BarCode (BarCode front), 8-CCD connection-lifecycle glue.
//  Translator: AI(W5-Final-BarCode8CCDGlue) 20260711
//
//  SCOPE
//  -----
//  This unit is THE FIRST BarCode translation unit to materialize the 13
//  `TClientSocket*` members as REAL `vclcompat/ClientSocket.h` objects (every
//  prior BarCode unit gated ClientSocket_* access with its own no-op stand-in
//  -- see "PRE-EXISTING GATED STAND-INS" below).  It owns:
//    (1) the 13 real `Scktcomp::TClientSocket*` globals (constructed once at
//        static-init time, Sim mode by default -- see vclcompat/ClientSocket.h),
//    (2) the REAL `SendCCDCommand()` body (dispatches by `index` to whichever
//        of the 13 sockets, writing `Msg2+"\r\n"` -- or bare `Msg2` for the OCR
//        install -- via `Socket->SendText()`),
//    (3) the REAL `TimerBotton8CCDConnectTimer()` body (per-Bottom-socket
//        reconnect-on-address/port-mismatch + connect-after-N-ticks state
//        machine, x8 near-identical unrolled blocks, faithfully copy-pasted
//        from golden -- including its ONE genuine copy-paste bug, see below),
//    (4) the REAL `TimerBottom8CCDInitialTimer()` body (two near-identical
//        cursor state machines -- `Task`/`iBottomConntectionOkTask1` drives
//        Bottom_1..4, `Task2`/`iBottomConntectionOkTask2` drives Bottom_5..8 --
//        each doing Get-Version -> Clear-buffer -> Get-File-Name -> Change-File
//        -> [Shuttle-Float-Check variant of the same 4 steps] -> idle at 3000).
//
//  NOT in scope (other BarCode units, this same W5 wave):
//    ClientSocket_Shuttle1_AConnect/Disconnect/Error/Read (golden BarCode.cpp
//      :3136-3388) -- the SHARED event-handler set golden's own BarCode.dfm
//      wires onto ALL 12 of the non-BarcodeChangeFile sockets (Shuttle1_A/B,
//      Shuttle2_A/B, Bottom_1..8), discriminated at runtime by `((TClientSocket*)
//      Sender)->Tag` (0..3 for the Shuttle quartet, 0..7 for the Bottom octet --
//      a genuine Tag-value OVERLAP across the two families, harmless in golden
//      because the two hardware variants are mutually exclusive -- verified via
//      BarCode.dfm, see translate report item 5).  NOT translated here --
//      0 references to `Ptr->Tag`/`(TClientSocket*)Sender` in this file.
//      grep-verified: not yet translated ANYWHERE in this tree.
//    ClientSocket_BarcodeChangeFileConnect/Disconnect/Read (golden :6151-6183)
//      -- the ChangeFile socket's OWN dedicated (non-shared) handler set.
//    Change2DSetupFile() (golden :5527-...) -- calls
//      `iBottomConntectionOkTask1=1; iBottomConntectionOkTask2=1;` (golden
//      :5531-5532) to force-restart the Init state machine on a recipe change;
//      NOT translated here.  A future unit translating it MUST reuse this
//      unit's `iBottomConntectionOkTask1`/`iBottomConntectionOkTask2` externs
//      (declared below), not redefine them.
//    TimerDownCCDConnectTimer / iConntectionOkTask (golden :2900-3135ish, the
//      SHUTTLE-side connect/init timers) -- a separate pair of timers for the
//      4-socket Shuttle family; NOT part of this unit's assignment (13-socket/
//      2-timer scope is explicitly the 8-CCD Bottom family's connect+init
//      pair, per the hand-off).
//
//  PRE-EXISTING GATED STAND-INS this unit does NOT retarget (see translate
//  report -- integrate agent decision, not made here):
//    BarCode_Bottom2DID.cpp        (anonymous-namespace) SendCCDCommand(...)
//    BarCode_Bottom2DID.cpp        (anonymous-namespace) AddCCDCommunicationLog(...)
//    BarCode_Shuttle1_CCDScan.cpp  static CCD1_SendCCDCommand(...)
//    BarCode_Shuttle1_Scan.cpp     BarSh1_SendCCDCommand(...)  (external linkage,
//                                   undeclared in its own .h -- unreachable
//                                   from any other TU today)
//    BarCode_Shuttle2_CCDScan.cpp  (anonymous-namespace) Gated_SendCCDCommand(...)
//    BarCode_Shuttle2_Scan.cpp     (anonymous-namespace) Gated_SendCCDCommand(...)
//    BarCode_Shuttle2_ScanRemainder2.cpp (anonymous-namespace) Gated_SendCCDCommand(...)
//    BarCode_Bottom2DID8CCD.cpp    static Bottom2DID8CCD_SendCCDCommand(...)
//      (this one's real-socket branch is itself still `#if 0`-gated even
//      though it references the REAL ClientSocket_Bottom_* names -- it predates
//      this unit and was written against golden call shapes, not against a
//      real header; it will now compile-if-ungated against THIS unit's real
//      globals, but ungating it is this unit's sibling's call, not mine.)
//    (mirror list for AddCCDCommunicationLog: CCD1_/BarSh1_/Gated_(x3)/
//     Bottom2DID8CCD_ prefixed local stand-ins, same files as above.)
//
//  SYMBOLS REUSED (NOT redefined) -- canonical owners already on disk:
//    BAR_CODE_COUNT, iBarCode1_1/iBarCode1_2 (enum, ==0/==1), iBarCode2_1/
//      iBarCode2_2 (extern const int, ==2/==3), bGetSE9[BAR_CODE_COUNT] --
//      BarCode_Bottom2DID.h (#included below; that header itself transitively
//      #includes BarCode_Shuttle1_Scan.h + BarCode_Shuttle2_Scan.h, which is
//      how the next two lines are also satisfied via the SAME single include):
//    bSFCGetSE9[BAR_CODE_COUNT], iSFCTotalMoveStep -- BarCode_Shuttle1_Scan.h
//      (canonical owner; genuinely SHARED with DoShuttleFloatCheck_1/_2 -- this
//      unit's Init-timer Shuttle-Float-Check branch (Task/Task2 cases
//      2100-2350) manipulates the SAME physical "how many CCD positions to
//      scan" state those functions consume, so reusing rather than forking is
//      the faithful choice; a forked copy would risk a real state desync).
//    InArmSuck.iShtCol -- aHotPlateSubstrate.h (TMyKitSuck, transitively).
//    TestIF_File.{bEnableBarCode,bEnableShtFloatChk,bEnableBottom2D,
//      bBarCodeMultiRecipe,bSFCUse2Photo} -- cprod.h (transitively).
//    BAR_CODE_INSTALL, BOTTOM_2DID, BOTTOM_2DID_CCD, InitialOK -- cmydef.h.
//    ebctUseCCDMode/ebcUseOCR/ebctEtherNetCCD -- MachineType.h.
//    RecordProcess -- canary_support.h (Sim log sink; used by this unit's own
//      Glue8CCD_AddCCDCommunicationLog stand-in, see .cpp).
//
//  NOT reused (this unit owns the canonical definition instead) -- see
//  "NEW SINGLE-OWNER GLOBALS" below.
//
//  Big5: golden read via cp950-aware iconv; zero U+FFFD verified in this file.
// =============================================================================
#ifndef BarCode_8CCD_GlueH
#define BarCode_8CCD_GlueH

#include "vclcompat/vcl_compat.h"
#include "vclcompat/ClientSocket.h"
#include "myTimer.h"
#include "BarCode_Bottom2DID.h"   // BAR_CODE_COUNT, iBarCode1_1/iBarCode1_2 (enum),
                                  // iBarCode2_1/iBarCode2_2 (extern const), bGetSE9[];
                                  // transitively (via its own includes):
                                  // bSFCGetSE9[]/iSFCTotalMoveStep (BarCode_Shuttle1_Scan.h),
                                  // InArmSuck/TestIF_File/cmydef.h universe.

// ---------------------------------------------------------------------------
//  Per-CCD-position constants (golden ctor BarCode.cpp:208-215: iBottom_1=0 ..
//  iBottom_8=7).  Reuses the EXACT SAME guard macro name as
//  BarCode_Bottom2DID8CCD.h's own copy (HT9045_BARCODE_IBOTTOM_CONSTS) so the
//  two headers can coexist in the same TU without a redefinition error --
//  whichever is #included first wins; both spell the enum identically, so
//  there is no behavioural difference either way.  Plain compile-time enum,
//  no ODR risk.
// ---------------------------------------------------------------------------
#ifndef HT9045_BARCODE_IBOTTOM_CONSTS
#define HT9045_BARCODE_IBOTTOM_CONSTS
enum
{
    iBottom_1 = 0, iBottom_2 = 1, iBottom_3 = 2, iBottom_4 = 3,
    iBottom_5 = 4, iBottom_6 = 5, iBottom_7 = 6, iBottom_8 = 7
};
#endif

// ---------------------------------------------------------------------------
//  The 13 real TClientSocket instances (golden BarCode.h:127-130,134,300-307).
//  Constructed once at static-init time (see .cpp), Sim mode by default (see
//  vclcompat/ClientSocket.h's Sim/Real policy note) -- exactly like every
//  other already-translated singleton pointer in this tree (`fAGV = new
//  TfAGV();` in FormsFacade.cpp is the precedent this mirrors).
//  `Tag` is seeded to match golden BarCode.dfm's per-instance value (0..3 for
//  the Shuttle quartet, 0..7 for the Bottom octet) even though nothing in
//  THIS unit reads Tag back -- it is set for the benefit of the FUTURE unit
//  that translates the shared Connect/Disconnect/Error/Read handler set
//  (out of this unit's scope, see banner above), which discriminates by it.
// ---------------------------------------------------------------------------
extern Scktcomp::TClientSocket* ClientSocket_Shuttle1_A;         // Tag=0
extern Scktcomp::TClientSocket* ClientSocket_Shuttle1_B;         // Tag=1
extern Scktcomp::TClientSocket* ClientSocket_Shuttle2_A;         // Tag=2
extern Scktcomp::TClientSocket* ClientSocket_Shuttle2_B;         // Tag=3
extern Scktcomp::TClientSocket* ClientSocket_BarcodeChangeFile;  // Tag=0 (golden .dfm: no explicit Tag line -> default 0)
extern Scktcomp::TClientSocket* ClientSocket_Bottom_1;           // Tag=0
extern Scktcomp::TClientSocket* ClientSocket_Bottom_2;           // Tag=1
extern Scktcomp::TClientSocket* ClientSocket_Bottom_3;           // Tag=2
extern Scktcomp::TClientSocket* ClientSocket_Bottom_4;           // Tag=3
extern Scktcomp::TClientSocket* ClientSocket_Bottom_5;           // Tag=4
extern Scktcomp::TClientSocket* ClientSocket_Bottom_6;           // Tag=5
extern Scktcomp::TClientSocket* ClientSocket_Bottom_7;           // Tag=6
extern Scktcomp::TClientSocket* ClientSocket_Bottom_8;           // Tag=7

// ---------------------------------------------------------------------------
//  NEW SINGLE-OWNER GLOBALS -- golden TfBarCode-member/file-scope state
//  touched by SendCCDCommand/the two Timer functions that is NOT yet declared
//  ANYWHERE else in this translated tree (grep-verified tree-wide before
//  adding each one).  This unit owns the canonical definition (see .cpp);
//  ANY future unit needing the same golden field MUST #include this header
//  and reuse these externs, not redeclare local copies.
//
//  The hand-off prompt anticipated 6 of these (bSFCGetVR,
//  iBottomConntectionOkTask1/2, bEnableCCDChannelConnect[8], tChangeSetupFile,
//  bGetGF, bGetLF); reading the full golden bodies myself turned up 5 more
//  that TimerBottom8CCDInitialTimer's Shuttle-Float-Check branch and
//  SendCCDCommand's OCR-recipe-change branch also need and that are equally
//  undeclared tree-wide: bSFCGetGF, bSFCGetLF, sSFCCurrentGF, bChangeSetupFile,
//  OCRChangeFileTimeOut.  Actual golden code wins over the estimate (see
//  translate report item 8).
// ---------------------------------------------------------------------------
extern bool        bSFCGetVR;                          // golden BarCode.h:902
extern int         iBottomConntectionOkTask1;           // golden BarCode.cpp:43 (file-scope, NOT a class member)
extern int         iBottomConntectionOkTask2;           // golden BarCode.cpp:44 (file-scope, NOT a class member)
extern bool        bEnableCCDChannelConnect[BAR_CODE_COUNT]; // golden BarCode.h:737
                                                         // (genuinely shared with the Shuttle
                                                         // family too -- iBarCode1_1..iBarCode2_2
                                                         // index the SAME array, see banner item 5)
extern TQPF_Timer  tChangeSetupFile;                    // golden BarCode.cpp:48 (file-scope)
extern bool        bGetGF;                              // golden BarCode.h:751
extern bool        bGetLF;                              // golden BarCode.h:752
extern bool        bSFCGetGF;                           // golden BarCode.h:900
extern bool        bSFCGetLF;                           // golden BarCode.h:901
extern AnsiString  sSFCCurrentGF;                       // golden BarCode.h:899
extern bool        bChangeSetupFile;                    // golden BarCode.cpp:50 (file-scope;
                                                         // ALSO read by the not-yet-translated
                                                         // ClientSocket_Shuttle1_ARead, golden
                                                         // :3301 -- future unit must reuse this
                                                         // extern, not redefine it)
extern TQPF_Timer  OCRChangeFileTimeOut;                // golden BarCode.cpp:51 (file-scope;
                                                         // golden declares it `HTimer` -- see
                                                         // translate report judgment call --
                                                         // translated as TQPF_Timer, identical
                                                         // interface, same substitution already
                                                         // used for the adjacent `tChangeSetupFile`)

// ---------------------------------------------------------------------------
//  This unit's 3 functions.
// ---------------------------------------------------------------------------
void SendCCDCommand(int index, AnsiString Msg, AnsiString Msg2);   // golden BarCode.cpp:5305
// AI(W5-Final-BarCode8CCDGlue) 20260711: golden signatures carry an unused
// `TObject *Sender` parameter (standard VCL TTimer::OnTimer shape); grep-
// verified 0 references to `Sender` in either body -- dropped for a cleaner
// free-function signature, matching this project's established convention of
// not carrying unused VCL event-plumbing parameters into translated free
// functions (e.g. the AGV_E84/BarCode_Sh* precedents already do the same for
// their own Timer-shaped handlers).
void TimerBotton8CCDConnectTimer();                                // golden BarCode.cpp:9192
void TimerBottom8CCDInitialTimer();                                // golden BarCode.cpp:9539

#endif // BarCode_8CCD_GlueH
