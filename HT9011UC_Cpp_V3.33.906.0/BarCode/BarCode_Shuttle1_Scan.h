// =============================================================================
//  BarCode/BarCode_Shuttle1_Scan.h  --  Shuttle-1 side small scan/trigger/
//                                       float-check functions of the golden
//                                       TfBarCode VCL form (BCB6)
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/BarCode/BarCode_Sh1.cpp
//                   (5533-line golden file -- this unit is a SMALL SUBSET):
//     TfBarCode::InitialBarcodeScanInShuttle1(bool)  golden :32-80
//     TfBarCode::DoBarcodeTriggerInShuttle_1()        golden :2455-3024
//     TfBarCode::DoBarcodeScanInShuttle_1(bool)       golden :3026-4098
//     TfBarCode::DoBarcodeScanOutShuttle_1()          golden :4100-4216
//     TfBarCode::InitialShuttleFloatCheck1()          golden :4218-4222
//     TfBarCode::DoShuttleFloatCheck_1()              golden :4224-4696
//  Plus one function whose golden home is actually BarCode.cpp (NOT
//  BarCode_Sh1.cpp -- a scoping discrepancy vs. the task brief; golden wins,
//  see translate report item 8):
//     TfBarCode::InitialBarcodeScanOutShuttle1()      golden BarCode.cpp:606-609
//
//  Translation wave: W5-BarCode (Shuttle-1 side), 2026-07-11.
//  Translator: AI(W5-BarCode-Shuttle1-Translate) 20260711.
//
//  NOT translated here (sibling units in this same parallel batch / deferred):
//    * DoBarcodeCCDInShuttle_1(bool)  golden BarCode_Sh1.cpp:83-2453 (~2371
//      lines) -- a SIBLING unit's assignment this wave.  Forward-declared
//      below (exact signature) and called nowhere in this file (none of our
//      7 functions actually invoke it -- verified by grep; it is an
//      independent, mutually-exclusive alternate entry point that happens to
//      alias the SAME `iInitialBarcodeInShuttle1Task` cursor -- see the
//      "SHARED golden TfBarCode instance data" block below).
//    * TfBarCode::DoBarcodeCCDInShuttle_2 / Sh2-side functions -- BarCode_Sh2.cpp,
//      not assigned to anyone in this wave (deferred).
//    * TfBarCode::InitialSFCAutoTune1/DoSFCAutoTune_1/Do2DIDCheckSh1 -- golden
//      BarCode_Sh1.cpp:4698-5533, NOT one of the 7 assigned methods (not
//      touched at all by this unit; deferred).
//    * BarCode.cpp core helpers this unit's functions call into but does NOT
//      own/translate (Barcode_StartScan_In, Barcode_1StartScan, TurnOffInspection,
//      InitSht2DCodeComPort, CleanBarcodeError, DoBarcodeCount,
//      AddCCDCommunicationLog, SendCCDCommand, fBarCode->DoCheckSHT2DIDStatus,
//      fBarCode->RunCheckBarcodeByServerData) -- each is GATED at its call
//      site in the .cpp with a conservative offline default (see file head of
//      the .cpp for the full gate map).  SetSFCCheckStepCount() is the lone
//      EXCEPTION: it is a trivial 2-line pure calc (golden BarCode.cpp:3128)
//      needed for correct loop bounds, so this unit supplies its own
//      identically-named-avoiding faithful mirror (BarcodeSh1_SetSFCCheckStepCount)
//      rather than gating it.
//
//  Dependencies already translated elsewhere (real, NOT gated):
//    cmydef.h / cprod.h / cpublic.h / MachineType.h -- Prod, Offset, TestIF_File,
//      IniConfig, CosFunction, K_RETRY/K_SKIP, NULL_IC/HAS_NULL_IC,
//      BAR_CODE_INSTALL, IndexSuckName, iBarcodeReject, bBarcodePassword,
//      asBarcodeLotNumber, iNeedBarcodeCount/iBarcodeErrorCount/iBarcodeAutoRetry/
//      bBarcodeFirstAutoRetry/bCheckCodeError/bCheckLotError/bBarcodeAutoSkip/
//      bBarcodeAutoSkipError (all genuine GLOBALS despite living inside the
//      golden TfBarCode.h member list textually -- verified against golden
//      cmydef.h, same names/sizes), MInShuttle1/MOutShuttle1,
//      asBarCodeErrorSend/asBarCodeErrorCheckSum, DUMMY.
//    aHotPlateSubstrate.h -- TMyKitSuck (InArmSuck/FLCarryKit/FRCarryKit),
//      Motor/mymotor.h MOT[]/TMyMotor (transitively).
//    acarry.h -- DoInOutARM_SHT_MoveSafe(int) (real body, W6.5).
//    acarry_shims.h -- fLtcSensor->ClearLtcSensor(int).
//    atester_shims.h -- fContact->IsRun2DCheck(), IsNNMode()/NN_1Row/NN_2Row.
//    canary_support.h -- ShowMyMessage, ShowErrorMessage, K_RETRY/K_SKIP,
//      LastSet.iRealDummy.
//    common.h -- asBarCodeLogPath / asBarCodeLot (real data; the
//      WriteDataToFile/MyForceDirectories FUNCTION bodies are themselves
//      `#if 0`-gated in common.cpp -- TODO(wave-file) -- so THIS unit gates
//      its own calls to them too, even though the declarations are visible).
//
//  Big5: golden read via cp950-aware iconv; zero U+FFFD verified in the
//  translated files below.
// =============================================================================
#ifndef BarCode_Shuttle1_ScanH
#define BarCode_Shuttle1_ScanH

#include "vclcompat/vcl_compat.h"
#include "cmydef.h"                 // -> cprod.h/cpublic.h/MachineType.h transitively; ALSO now the
                                    // canonical home of iSH1_1BarcodePosition/iSH1_2BarcodePosition
                                    // (cmydef.h:3846-3847 / cmydef.cpp:4119-4120 -- confirmed present
                                    // on disk at translate time; NOT redeclared/redefined here, see
                                    // "ODR RESOLVED" block below)
#include "aHotPlateSubstrate.h"     // TMyKitSuck InArmSuck/FLCarryKit/FRCarryKit, MOT[] transitively
#include "acarry.h"                 // DoInOutARM_SHT_MoveSafe (real)
#include "acarry_shims.h"           // fLtcSensor (real)
#include "atester_shims.h"          // fContact (real), IsNNMode() (real)
#include "canary_support.h"         // ShowMyMessage/ShowErrorMessage/LastSet (real)
#include "common.h"                 // asBarCodeLogPath/asBarCodeLot data (real)
#include "BarCode_Shuttle2_Scan.h"  // canonical owner of the shared TfBarCode state below (ODR RESOLVED)

#include <map>

// ---------------------------------------------------------------------------
//  BAR_CODE_COUNT -- golden BarCode.h:30 `#define BAR_CODE_COUNT 8`.  Not yet
//  in any translated shared header; guarded so a sibling BarCode unit that
//  ALSO defines this macro in its own header does not collide (macros are
//  preprocessor-only -- re-#define with an identical value is harmless, but
//  the guard keeps -Wpedantic clean).
// ---------------------------------------------------------------------------
#ifndef BAR_CODE_COUNT
#define BAR_CODE_COUNT 8
#endif

// ---------------------------------------------------------------------------
//  iBarCode1_1 / iBarCode1_2 -- golden BarCode.cpp:217-218 (TfBarCode::FormCreate
//  fixed constants, never reassigned).  Shuttle-1's two logical CCD-row
//  indices used to index the BAR_CODE_COUNT-sized arrays below.  Compile-time
//  constants (not extern data) -- safe to redeclare identically in a sibling
//  TU without an ODR/link conflict.
// ---------------------------------------------------------------------------
#ifndef HT9045_BARCODE_SH1_ROWCONST_ADDED
#define HT9045_BARCODE_SH1_ROWCONST_ADDED
enum { iBarCode1_1 = 0, iBarCode1_2 = 1 };
#endif

// ============================================================================
//  SHARED golden TfBarCode instance data this unit's functions read/write
//  across calls (golden: non-static class-member arrays/scalars, NOT the
//  function-local `static` counters, which stay function-local in the .cpp
//  exactly as golden has them).
//
//  *** ODR CONCRETELY RESOLVED (2026-07-11) ***
//  During this translation, the sibling `BarCode_Shuttle2_Scan` unit's
//  deliverable was found ALREADY on disk (BarCode/BarCode_Shuttle2_Scan.h/.cpp
//  + tests/test_barcode_shuttle2_scan.cpp), and its header explicitly claims
//  canonical ownership of the extern definitions for `iShtDupilcateFailCnt`,
//  `map2DList`/`map2DIter`, `bBarcodeStartDelay`, `bBarcodeDataSaveReady`,
//  `BarcodeExposureDelay`, `BarcodePosDelay`, `BarcodeDelay`, `bCCDcheckShuttle`
//  (verified identical golden types/sizes) -- these are genuinely the SAME
//  golden TfBarCode class members the DoBarcodeCCDInShuttle_1/_2 siblings and
//  both Sh1/Sh2 Trigger/Scan paths all read/write.  This unit therefore does
//  NOT redefine them (would be a hard multiple-definition link error against
//  BarCode_Shuttle2_Scan.cpp) -- it `#include`s BarCode_Shuttle2_Scan.h above
//  and reuses THEIR extern declarations verbatim.
//  Separately, `iSH1_1BarcodePosition`/`iSH1_2BarcodePosition` were ALSO found
//  already promoted to cmydef.h/cmydef.cpp (extern decl + definition) by the
//  time this unit finished -- likely an integrate-agent step reconciling the
//  same shared-state concern this file originally flagged speculatively.
//  This unit therefore does NOT redefine those two either (cmydef.h, already
//  #included above, supplies them).
//  Only the genuinely Shuttle-1-ONLY state (never referenced by
//  BarCode_Shuttle2_Scan or cmydef.h) is declared+defined by THIS unit below.
// ============================================================================
extern int iInitialBarcodeInShuttle1Task;   // golden BarCode.h:786
extern int iInitialBarcodeOutShuttle1Task;  // golden BarCode.h:788
extern int iShuttleFloatCheck1Task;         // golden BarCode.h:882

// iShtDupilcateFailCnt / map2DList / map2DIter / bBarcodeStartDelay /
// bBarcodeDataSaveReady / BarcodeExposureDelay / BarcodePosDelay /
// BarcodeDelay / bCCDcheckShuttle -- ALL supplied by BarCode_Shuttle2_Scan.h
// (#included above).  iSH1_1BarcodePosition/iSH1_2BarcodePosition -- supplied
// by cmydef.h (#included above).  Do NOT redeclare/redefine any of these here.

extern bool bCCDBarcodeWaitReply    [BAR_CODE_COUNT];   // golden BarCode.h:924 -- Shuttle-1-only (Sh2 unit does not touch)
extern bool bHasPin1Error           [BAR_CODE_COUNT];   // golden BarCode.h:741 -- Shuttle-1-only
extern bool bImageSaveflag          [BAR_CODE_COUNT];   // golden BarCode.h:777 -- Shuttle-1-only
extern TQPF_Timer BarcodeWaitReplyTimeOut[BAR_CODE_COUNT]; // golden BarCode.h:876 -- Shuttle-1-only

// -- Shuttle Float Check (SFC) instance data -- golden BarCode.h:882-912.
//    Only touched by DoShuttleFloatCheck_1/InitialShuttleFloatCheck1 within
//    THIS unit; DoShuttleFloatCheck_2 is explicitly NOT part of the sibling
//    Sh2 unit's delivered scope (see its header's "REMAINING / HANDED OFF"),
//    so no conflict today -- still flagged for whoever completes Sh2's float
//    check later.
extern int  iSFCTotalMoveStep;              // golden BarCode.h:886
extern TQPF_Timer SFCStartDelay   [2];       // golden BarCode.h:896 (BAR_CODE_COUNT decl, [2] actually used: iSht in {0,1})
extern TQPF_Timer SFCExposureDelay[2];       // golden BarCode.h
extern TQPF_Timer SFCResultDelay  [2];       // golden BarCode.h:898
extern int  iSFCCurrentStep [BAR_CODE_COUNT];   // golden BarCode.h:903
extern bool bSFCStepError   [BAR_CODE_COUNT];   // golden BarCode.h:904
extern bool bSFCExposureOK  [BAR_CODE_COUNT];   // golden BarCode.h
extern bool bSFCGetResultOK [BAR_CODE_COUNT];   // golden BarCode.h
extern bool bSFCGetSE9      [BAR_CODE_COUNT];   // golden BarCode.h
extern int  iSFCCheckResult [BAR_CODE_COUNT][16]; // golden BarCode.h:912

// ============================================================================
//  Sibling-unit real function (golden BarCode_Sh1.cpp:83, ~2371 lines) --
//  forward-declared per the task's explicit instruction, NOT defined here.
//  None of this unit's 7 functions actually call it (verified by grep); the
//  declaration is provided only so a future TU that DOES call it (or the
//  integrate agent wiring the shim) can see the exact signature.  Do NOT
//  define this symbol in this file/unit -- the sibling unit owns the body.
// ============================================================================
bool DoBarcodeCCDInShuttle_1(bool bVerify = false);   // golden BarCode.h:807

// ============================================================================
//  This unit's 7 assigned free functions (exact signatures per
//  aHotPlateSubstrate.h:636-651 -- the TfBarCode_Shim delegation target list).
// ============================================================================
void InitialBarcodeScanInShuttle1(bool bClear2DID = true);   // golden BarCode_Sh1.cpp:32
void InitialBarcodeScanOutShuttle1();                        // golden BarCode.cpp:606 (NOT BarCode_Sh1.cpp -- see file head)
bool DoBarcodeScanInShuttle_1(bool bErrorSkip = false);      // golden BarCode_Sh1.cpp:3026
bool DoBarcodeScanOutShuttle_1();                            // golden BarCode_Sh1.cpp:4100
bool DoBarcodeTriggerInShuttle_1();                          // golden BarCode_Sh1.cpp:2455
void InitialShuttleFloatCheck1();                            // golden BarCode_Sh1.cpp:4218
bool DoShuttleFloatCheck_1();                                // golden BarCode_Sh1.cpp:4224

#endif // BarCode_Shuttle1_ScanH
