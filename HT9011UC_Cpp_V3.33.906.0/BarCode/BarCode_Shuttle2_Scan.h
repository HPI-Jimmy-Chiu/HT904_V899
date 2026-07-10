// =============================================================================
//  BarCode/BarCode_Shuttle2_Scan.h  --  Shuttle-2 side small scan/trigger/
//                                        float-check functions (TfBarCode)
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/BarCode/BarCode_Sh2.cpp
//                   (5664 lines total; this unit translates a SMALL SUBSET --
//                   see per-function golden line ranges below) and
//                   .../BarCode/BarCode.cpp:606-616 (InitialBarcodeScanOutShuttle1/2,
//                   which golden puts in the shared BarCode.cpp file, not Sh2.cpp).
//  Golden decl home: BarCode/BarCode.h:799-891 (TfBarCode class, public methods).
//  Translation wave: W5-BarCode (BarCode front, recon'd 2026-07-11, not in the
//                    original W0-W7 plan -- see MIGRATION_ROADMAP DEFERRED table).
//  Translator: AI(W5-BarCode-Shuttle2Scan) 20260711
//
//  SCOPE (delivered this unit -- 5 of 7 originally assigned methods; see
//  "REMAINING / HANDED OFF" below for the other 2):
//    BarCode_Sh2_InitialBarcodeScanInShuttle2(bool bClear2DID=true)
//        golden BarCode_Sh2.cpp:31-79     (49 lines)
//    BarCode_Sh2_InitialBarcodeScanOutShuttle2()
//        golden BarCode.cpp:612-616       (5 lines -- NOTE: golden puts this
//        one sibling in BarCode.cpp, not BarCode_Sh2.cpp; verified by grep,
//        see report to integrate agent)
//    BarCode_Sh2_DoBarcodeTriggerInShuttle_2()
//        golden BarCode_Sh2.cpp:2474-3038 (~566 lines)
//    BarCode_Sh2_DoBarcodeScanOutShuttle_2()
//        golden BarCode_Sh2.cpp:4262-4399 (~138 lines)
//    BarCode_Sh2_InitialShuttleFloatCheck2()
//        golden BarCode_Sh2.cpp:4400-4405 (6 lines)
//
//  REMAINING / HANDED OFF (NOT translated this unit -- see translate report for
//  precise reasoning; TfBarCode_Shim keeps its existing offline-safe stub body
//  for these two, i.e. behaviour is UNCHANGED from before this unit ran):
//    TfBarCode::DoBarcodeScanInShuttle_2(bool bErrorSkip=false)
//        golden BarCode_Sh2.cpp:3040-4261 (~1222 lines) -- read through :3697
//        (~54%); remaining :3698-4261 (~563 lines) unread/untranslated.
//    TfBarCode::DoShuttleFloatCheck_2()
//        golden BarCode_Sh2.cpp:4406-4879 (~474 lines) -- read through :4645
//        (~57%); remaining :4646-4879 (~234 lines) unread/untranslated.
//  Both turned out to be deeply-coupled CCD-protocol state machines (far denser
//  than the "small helper" characterization) touching many more not-yet-homed
//  cross-module symbols (fNote->t2DCode, SendCCDCommand, SetSFCCheckStepCount,
//  CleanBarcodeError, iBarcodeAutoRetry[], bBarcodeAutoSkip[], etc.) on top of
//  everything already gated here.  Rather than push through hastily (past this
//  project's own documented ~1500-2000-line "stop and hand off cleanly"
//  threshold) they are left as a precise, clean hand-off for a follow-up unit.
//
//  Explicitly OUT OF SCOPE for this whole file (sibling units / other waves):
//    TfBarCode::DoBarcodeCCDInShuttle_2(bool bVerify=false)
//        golden BarCode_Sh2.cpp:84-2473 (~2390 lines) -- separate dedicated
//        unit in this same wave.  NOT called by anything in this file (verified
//        by grep -- Sh2.cpp itself never calls it; only acarry.cpp calls it,
//        as an alternate/mutually-exclusive code path to the Trigger-based scan
//        this file implements, selected by BAR_CODE_INSTALL config).  No
//        forward declaration needed here.
//
//  GOLDEN QUIRK PRESERVED VERBATIM (Shuttle-1/Shuttle-2 asymmetry, per this
//  project's own DEVLOG precedent that such pairs are sometimes NOT symmetric):
//    DoBarcodeTriggerInShuttle_2's case 3400 (golden :2910/:2930) writes to
//    iSH1_1BarcodePosition/iSH1_2BarcodePosition (the SHUTTLE-1 position
//    globals!) instead of iSH2_1BarcodePosition/iSH2_2BarcodePosition, even
//    though the surrounding case 1120 (golden :2615/:2629) and every other use
//    in this same file correctly uses the SH2 globals.  This looks like a
//    copy-paste-from-Shuttle-1 bug in the golden source.  NOT "fixed" here --
//    reproduced exactly, with an inline comment at the spot.
//
//  SHARED TfBarCode-MEMBER STATE (IMPORTANT for the integrate agent + any
//  sibling/future BarCode unit):
//  In golden, iBarCode2_1/iBarCode2_2, iInitialBarcodeInShuttle2Task,
//  bBarcodeStartDelay[]/bBarcodeDataSaveReady[], iShtDupilcateFailCnt[],
//  BarcodePosDelay[]/BarcodeExposureDelay[]/BarcodeDelay[], and map2DList/
//  map2DIter are all members of the SINGLE TfBarCode class instance (fBarCode),
//  genuinely shared across DoBarcodeTriggerInShuttle_2/DoBarcodeScanInShuttle_2
//  (this file's mutually-exclusive serial-CCD path) AND DoBarcodeCCDInShuttle_2
//  (the sibling unit's mutually-exclusive CCD-snapshot path) -- confirmed by
//  grep: DoBarcodeCCDInShuttle_2's body (golden :84-2473) references ALL of
//  these same names.  This unit therefore OWNS the single canonical extern
//  definition of each (in the .cpp), declared extern below.  ANY other
//  translation unit that needs the same golden member (the DoBarcodeCCDInShuttle_2
//  sibling unit, or a future BarCode.cpp-core wave) MUST #include this header
//  and reference these externs -- NOT redeclare/redefine local copies (would be
//  a silent state-desync at best, an ODR/duplicate-definition link error at
//  worst).  Flagged prominently in the translate report (item 6) for the
//  integrate agent to reconcile if a sibling unit already independently defined
//  any of these same names.
//
//  iBarCode2_1==2 / iBarCode2_2==3 verbatim from golden BarCode.cpp:219-220
//  (TfBarCode::FormCreate) -- that constructor never runs in this offline
//  shim world, so the fixed values are reproduced directly as compile-time
//  constants (internal linkage, so no ODR risk even if multiple TUs include
//  this header).
//
//  Gated (golden lives in the still-untranslated shared BarCode.cpp core, or a
//  VCL widget/form not yet translated) -- see BarCode_Shuttle2_Scan.cpp's
//  Gated_* wrapper functions for the exact per-symbol golden references and
//  conservative-default rationale:
//    Barcode_StartScan_In, Barcode_1StartScan, TurnOffInspection,
//    AddCCDCommunicationLog, DoCheckSHT2DIDStatus, DoBarcodeCount,
//    InitSht2DCodeComPort, ShowMyMessageBox_YES_SKIP  (all BarCode.cpp core)
//    mtBarcodeInSh / fLotInfo->mtBarcodeInShLotInfo    (VCL grid widgets, W7)
//
//  Already-available real dependencies reused (NOT gated):
//    InArmSuck/BLCarryKit/BRCarryKit (aHotPlateSubstrate.h, TMyKitSuck)
//    Prod/Offset/TestIF_File (cprod.h)  IniConfig (Config.h)
//    MOT[]/TTrayMotor::MotorMove (Motor/mymotor.h)
//    ShowMyMessage/ShowErrorMessage/K_RETRY/K_SKIP/LastSet (canary_support.h,
//      cmydef.h)
//    fContact->IsRun2DCheck() (atester_shims.h, TfContactShim)
//    DoInOutARM_SHT_MoveSafe (acarry.h/.cpp -- real translated W6.5 body)
// =============================================================================
#ifndef BarCode_Shuttle2_ScanH
#define BarCode_Shuttle2_ScanH

#include "vclcompat/vcl_compat.h"   // AnsiString
#include "myTimer.h"                // TQPF_Timer
#include <map>

#ifndef BAR_CODE_COUNT
#define BAR_CODE_COUNT 8            // golden BarCode.h:30 (KaiChen 20200513 : 4-->8)
#endif

// ---------------------------------------------------------------------------
//  Shared TfBarCode-member state this unit owns the canonical definition of
//  (see banner above).  Golden field types/homes noted per line.
// ---------------------------------------------------------------------------
extern const int iBarCode2_1;                     // golden BarCode.cpp:219 (FormCreate) == 2
extern const int iBarCode2_2;                     // golden BarCode.cpp:220 (FormCreate) == 3

extern int  iInitialBarcodeInShuttle2Task;        // golden BarCode.h:787
extern int  iInitialBarcodeOutShuttle2Task;       // golden BarCode.h:789
extern int  iShuttleFloatCheck2Task;               // golden BarCode.h:883

extern bool bBarcodeStartDelay   [BAR_CODE_COUNT]; // golden BarCode.h:738
extern bool bBarcodeDataSaveReady[BAR_CODE_COUNT]; // golden BarCode.h:740
extern int  iShtDupilcateFailCnt [2];              // golden BarCode.h:812
extern bool bCCDcheckShuttle     [2];              // golden BarCode.h (wei 20161102 IC置偏檢查)

extern TQPF_Timer BarcodePosDelay      [2];             // golden BarCode.h:871
extern TQPF_Timer BarcodeExposureDelay [2];             // golden BarCode.h:870
extern TQPF_Timer BarcodeDelay         [BAR_CODE_COUNT];// golden BarCode.h:869

extern std::map<AnsiString, AnsiString> map2DList;                    // golden BarCode.h:852
extern std::map<AnsiString, AnsiString>::iterator map2DIter;          // golden BarCode.h:853

// ---------------------------------------------------------------------------
//  Free functions -- real bodies for 5 of the 7 assigned TfBarCode_Shim
//  methods (see SCOPE above).  Names distinct from the golden TfBarCode
//  members (BarCode_Sh2_ prefix) so call sites/grep stay unambiguous about
//  which is being invoked, mirroring the AGV_/UseCanBusOrEtherCAT precedent.
// ---------------------------------------------------------------------------
void BarCode_Sh2_InitialBarcodeScanInShuttle2(bool bClear2DID = true);   // golden BarCode_Sh2.cpp:31
void BarCode_Sh2_InitialBarcodeScanOutShuttle2();                       // golden BarCode.cpp:612
bool BarCode_Sh2_DoBarcodeTriggerInShuttle_2();                         // golden BarCode_Sh2.cpp:2474
bool BarCode_Sh2_DoBarcodeScanOutShuttle_2();                           // golden BarCode_Sh2.cpp:4262
void BarCode_Sh2_InitialShuttleFloatCheck2();                           // golden BarCode_Sh2.cpp:4400

#endif // BarCode_Shuttle2_ScanH
