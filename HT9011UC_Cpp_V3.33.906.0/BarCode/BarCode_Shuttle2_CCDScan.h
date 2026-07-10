// =============================================================================
//  BarCode/BarCode_Shuttle2_CCDScan.h  --  TfBarCode::DoBarcodeCCDInShuttle_2
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/BarCode/BarCode_Sh2.cpp
//                   lines 84-2473 (~2390 lines), function
//                   `bool TfBarCode::DoBarcodeCCDInShuttle_2(bool bVerify=false)`
//                   //Steven 20160106 : 改用CCD拍完就跑的方式
//                   ("switched to a shoot-then-move-on CCD capture scheme").
//  Golden decl home: BarCode/BarCode.h:808 (TfBarCode class, public method).
//  Translation wave: W5-BarCode (BarCode front, recon'd 2026-07-11, parallel
//                    translate batch -- see MIGRATION_ROADMAP DEFERRED table
//                    entry "BarCode/{BarCode,BarCode_Sh1,BarCode_Sh2,BarcodeXML}.cpp").
//  Translator: AI(W5-BarCode-Shuttle2CCDScan) 20260711
//
//  SCOPE: this unit owns exactly ONE function -- the "CCD-snapshot" (camera,
//  not serial-reader) in-shuttle-2 2DID capture state machine.  It is one of
//  17 stubbed TfBarCode_Shim methods (aHotPlateSubstrate.h:623-664); this file
//  supplies the real body as a free function (`BarCode_DoBarcodeCCDInShuttle_2`).
//  See aHotPlateSubstrate.h:645 for the exact stub signature this delegates to.
//
//  Sibling unit in this SAME wave (different golden file, no file collision --
//  see BarCode_Shuttle2_Scan.h banner, which explicitly marks golden
//  BarCode_Sh2.cpp:84-2473 -- i.e. THIS function -- "OUT OF SCOPE for this
//  whole file / separate dedicated unit"):
//    BarCode/BarCode_Shuttle2_Scan.{h,cpp}  -- DoBarcodeTriggerInShuttle_2 /
//    DoBarcodeScanOutShuttle_2 / Initial* (the mutually-exclusive SERIAL-reader
//    scan path, selected instead of THIS CCD-snapshot path by BAR_CODE_INSTALL
//    config).  That header ALREADY owns the canonical single definition of
//    several golden TfBarCode-member globals this function also needs
//    (confirmed by grep both ways) -- REUSED here via #include, NOT
//    redeclared:
//      iBarCode2_1 / iBarCode2_2             (golden BarCode.cpp:219-220, ==2/3)
//      iInitialBarcodeInShuttle2Task         (golden BarCode.h:787 -- THIS
//                                              function's own `Task` cursor,
//                                              aliased via `int &Task=...`)
//      iShtDupilcateFailCnt[2]               (golden BarCode.h:812)
//      BarcodePosDelay[2] / BarcodeExposureDelay[2] / BarcodeDelay[BAR_CODE_COUNT]
//                                             (golden BarCode.h:869-871)
//      map2DList / map2DIter                 (golden BarCode.h:852-853)
//
//  NEW shared TfBarCode-member state THIS unit is first to need (not present in
//  BarCode_Shuttle2_Scan.h) -- CANONICAL single definition provided in the .cpp,
//  declared extern below.  IMPORTANT for the integrate agent + the sibling
//  DoBarcodeCCDInShuttle_1 unit (different golden file BarCode_Sh1.cpp, but the
//  SAME golden TfBarCode class -- confirmed by grep that BarCode_Sh1.cpp's
//  DoBarcodeCCDInShuttle_1 references every one of these same field names,
//  just indexed via iBarCode1_1/iBarCode1_2 instead of iBarCode2_1/iBarCode2_2):
//      bStepError / iCurrentStep / bCCDBarcodeExposureOK / bCCDBarcodeGetResultOK /
//      bGetSE9 / bCCDBarcodeAutoAdjustLight        [BAR_CODE_COUNT]-wide
//      map2DShuttleList / mapShtIter                [BAR_CODE_COUNT]-wide maps
//      cMilti2DBuffer                               [BAR_CODE_COUNT][32]
//      dVisionVer / s2DIDYield / bNeedCheckWhitleList / iBarCodeNo[4][8]
//      Socket2DReceiveList[BAR_CODE_COUNT] / list2DByLot / list2DWhitle /
//      list2DWhitleTemp / list2DWhitleResult         (TStringList*)
//      BarcodeAutoAdjustLightTimeOut[2]
//  If the DoBarcodeCCDInShuttle_1 sibling unit ALSO defines these (plausible --
//  it needs identical names), the integrate agent MUST deduplicate to ONE
//  definition (delete one side's storage, keep both sides' `extern` decls) --
//  see translate report item 6 for the full list.
//
//  Gated (golden lives in the still-untranslated shared BarCode.cpp core
//  (~11966 lines, MIGRATION_ROADMAP DEFERRED), a raw TClientSocket_* CCD
//  socket, a not-yet-translated VCL grid/edit widget, or the not-yet-
//  translated MES/json bridge) -- every gate is an anonymous-namespace
//  (TU-local, zero ODR risk) function in the .cpp; see there for golden
//  file:line + conservative-default rationale for each:
//    SendCCDCommand (real ClientSocket_Shuttle2_A/B I/O), AddCCDCommunicationLog
//    (memoCCDCommLog widget + per-hour log file -- WriteDataToFile itself is
//    ALSO gated, common.h:295 "TODO(wave-file)"), mtBarcodeInSh (TTMyTray grid
//    widget), fNote->t2DCode (TfNote VCL form, duplicate-code display grid),
//    cbBarCodeSimulate->Checked (TCheckBox), ed_BarCodeNo->Text (TEdit),
//    fLotInfo->edCustomerLotId->Text (TEdit -- NOT yet exposed on FormsFacade's
//    TfLotInfo; see report), fMesSystem->IsMatchServerData (MES/json bridge,
//    not yet translated), ShowMyMessageBox_YES_SKIP (golden mymessbox.h:54,
//    not yet translated -- same gate the Shuttle2_Scan sibling independently
//    added; both are anonymous-namespace so no collision).
//
//  Faithfully translated in full (verified cheap/pure, no HW/UI coupling):
//    ClearBuffer / SendSE0 (pure command-string formatting, delegate to the
//      gated SendCCDCommand for the one real I/O line each)
//    GetMovePos (pure Prod.iInSH[Multi2D]BarCodeDetectPos*[...] table lookup)
//    DoBarcodeCount (per-channel Yield-alarm rate calc over already-real
//      iNeedBarcodeCount/iBarcodePassCount/iBarcodeErrorCount/iBarcodeDuplicate/
//      iBarcodeAutoRetry (cmydef.h) + ChangeToFloatNonPcnt (MachineType.h);
//      fLotInfo->sgBarcode grid-cell writes gated, calc/return kept real)
//    DoCheckSHT2DIDStatus (pure FLCarryKit/BLCarryKit.Item/cDeviceInf scan;
//      the golden `#ifdef SOFT_SIMULTE cb2DIDAllfailSimulate->Checked` debug
//      hook is copied VERBATIM -- inert here since SOFT_SIMULTE is never
//      #defined in this tree, so the compiler strips it before the widget
//      pointer would need to exist)
//    bSHT_2DIDIsInsideList / the case-3500 by-Lot 2DID check (golden
//      TStringList::Find, which requires Sorted=true + is not yet in
//      vclcompat -- substituted with the already-available, semantically
//      -equivalent TStringList::IndexOf; documented per call site)
//    GetSimuBarCodeName (pure numbering/format-string generator; the ONE
//      ed_BarCodeNo->Text widget read/write is gated to a fixed "" prefix)
//    CCDBarcode_SortingForMulti2D -- NOT reached this wave (its only two call
//      sites in this function are both statically dead: case 1350's Multi2D
//      branch is the golden `if(TestIF_File.bEnableMulti2D)` arm, which is
//      mutually exclusive with the CCD-snapshot GetResult flags this function
//      itself sets only on the non-Multi2D SendCCDCommand path -- but golden
//      calls it unconditionally at the top of the Multi2D success branch
//      regardless, so a conservative no-op stand-in is supplied rather than
//      omitting the call).
//
//  Already-available real dependencies reused (NOT gated):
//    InArmSuck/BLCarryKit (aHotPlateSubstrate.h, TMyKitSuck)
//    Prod/Offset/TestIF_File (cprod.h)  IniConfig (Config.h)  CosFunction.h
//    MOT[]/TTrayMotor::MotorMove/fCanMoveR (Motor/mymotor.h)
//    ShowMyMessage/ShowErrorMessage/K_RETRY/K_SKIP/K_RESET/OFF_LINE/LastSet
//      (canary_support.h)
//    fContact->fShow / IsRun2DCheck() (atester_shims.h, TfContactShim)
//    fLtcSensor->ClearLtcSensor() (acarry_shims.h)
//    fMain->Reset(AnsiString) (FormsFacade.h)
//    fLotInfo->cbRunMode->Text (FormsFacade.h, TfLotInfoRunMode)
//    DoInOutARM_SHT_MoveSafe (acarry.h/.cpp -- real translated W6.5 body)
//    SendHTTPRequest (acarry_shims.h -- REUSED, see report for a documented
//      return-value-convention discrepancy between this call site's strict
//      `ret==1` success check and the acarry.cpp call sites' `ret==-1` fail
//      check; kept AS-IS to avoid an ODR duplicate-definition, not "fixed")
//    IndexSuckName[4][8] / asBarCodeErrorSend / asBarCodeErrorCheckSum /
//      asBarcodeLotNumber / sWhiteListLotID / sWhiteListProcess /
//      iNeedBarcodeCount / iBarcodePassCount / iBarcodeErrorCount /
//      iBarcodeDuplicate / iBarcodeAutoRetry / iBarcodeReject /
//      bStartAutoAdjustLight / bBarcodeNeedAutoAdjust[4] (cmydef.h)
//    asBarCodeLot / asBarCodeDownLot / as2DWhiteListLog / FileExists (common.h /
//      vclcompat/SysUtils.h)
//    CONTACT_NORMAL / CONTACT_AUTO_GET_HEIGHT / CONTACT_TEST (golden
//      cContact.cpp:74-77 `const int`, values 0/1/3 -- reproduced here as
//      internal-linkage `const int`, so no ODR risk even though cContact.h/.cpp
//      themselves remain untranslated)
//
//  GOLDEN QUIRK PRESERVED VERBATIM (do not "fix"):
//    case 4300's SPIL_FOR_QLE cleanup loop tests
//    `if(InArmSuck.iShtRow==2 && Prod.bF18InshuttleDetect)` for the ShowMyMessage
//    text branch even though the OUTER guard already required
//    `Prod.bF18InshuttleDetect==true` -- the inner re-check of the same flag is
//    redundant in golden; reproduced exactly (KaiHuang/JerryYang authored, not
//    a translation artifact).
//    case 8271-equivalent (bSHT_2DIDIsInsideList) tests `Str==...` (the RAW,
//    not-yet-\r\n-trimmed string) for the error-code short-circuit, while the
//    list-membership checks below use `Str1` (the trimmed string) -- this
//    asymmetry is in golden verbatim, preserved.
//
//  Big5: every Chinese comment decoded via cp950, preserved as UTF-8.  Final
//  gate: ZERO U+FFFD (see translate report for the verification command).
// =============================================================================
#ifndef BarCode_Shuttle2_CCDScanH
#define BarCode_Shuttle2_CCDScanH

#include "vclcompat/vcl_compat.h"   // AnsiString
#include "vclcompat/TStringList.h"  // TStringList
#include "myTimer.h"                // TQPF_Timer
#include <map>

#ifndef BAR_CODE_COUNT
#define BAR_CODE_COUNT 8            // golden BarCode.h:30 (KaiChen 20200513 : 4-->8)
#endif
#ifndef _MAX_SUCK_COL_ITEM
#define _MAX_SUCK_COL_ITEM 8        // golden MyKitSuck.h (also in aHotPlateSubstrate.h)
#endif

// Reuse the sibling Shuttle2-scan unit's canonical shared-state definitions
// (iBarCode2_1/2, iInitialBarcodeInShuttle2Task, iShtDupilcateFailCnt,
// BarcodePosDelay/BarcodeExposureDelay/BarcodeDelay, map2DList/map2DIter) --
// see banner above.  Do NOT redeclare these names in this file.
#include "BarCode_Shuttle2_Scan.h"

// INTEGRATE FIX (20260711): reuse the Bottom2DID unit's canonical shared-state
// definitions too (bStepError/iCurrentStep/bCCDBarcodeExposureOK/
// bCCDBarcodeGetResultOK/bGetSE9/s2DIDYield/map2DShuttleList/list2DByLot --
// see the extern block below).  BOTH this unit and BarCode_Bottom2DID.cpp
// independently defined these same golden TfBarCode-member globals at file
// scope (a real multiple-definition link error, flagged by both this unit's
// AND the Bottom2DID8CCD unit's translate reports).  BarCode_Bottom2DID.cpp
// is the canonical owner (BarCode_Bottom2DID8CCD.h already depends on it as
// such); this file now consumes its externs instead of redefining them.
#include "BarCode_Bottom2DID.h"

// ---------------------------------------------------------------------------
//  golden cContact.cpp:74-77 (TfContact-adjacent free consts, `extern const
//  int` in cContact.h:20-23).  cContact.h/.cpp themselves are untranslated
//  (22761-line VCL form, W7).  `const int` at namespace scope has INTERNAL
//  linkage by default in C++, so redeclaring the same three values here is
//  safe even if another parallel unit in this batch does the same.
// ---------------------------------------------------------------------------
const int CONTACT_NORMAL          = 0;   // golden cContact.cpp:74
const int CONTACT_AUTO_GET_HEIGHT = 1;   // golden cContact.cpp:75
const int CONTACT_TEST            = 3;   // golden cContact.cpp:77

// ---------------------------------------------------------------------------
//  golden BarCode.h:83-91 `enum eMulti2DType`.  Only e2x1In2CCD/e2x2In2CCD are
//  read by this function; the full enum is reproduced for golden-value
//  fidelity/citation.  Unscoped enum at file scope -- safe to also appear in
//  a sibling TU's own header (enumerators carry no external-linkage storage).
// ---------------------------------------------------------------------------
enum eMulti2DType
{
    e1x2In1CCD = 0,
    e2x1In1CCD = 1,
    e2x1In2CCD = 2,
    e2x2In1CCD = 3,
    e2x2In2CCD = 4,
    eMulti2DTypeTotal
};

// ---------------------------------------------------------------------------
//  NEW shared TfBarCode-member state this unit is first to need (see banner).
//  Golden field types/homes noted per line.  Canonical single definition in
//  BarCode_Shuttle2_CCDScan.cpp.
// ---------------------------------------------------------------------------
// bStepError / iCurrentStep / bCCDBarcodeExposureOK / bCCDBarcodeGetResultOK /
// bGetSE9 / s2DIDYield / map2DShuttleList / list2DByLot -- CANONICALLY OWNED by
// BarCode_Bottom2DID.cpp (see INTEGRATE FIX include above); do NOT redeclare
// storage for them here, just reuse those externs.
extern bool bCCDBarcodeAutoAdjustLight[BAR_CODE_COUNT];  // golden BarCode.h:928 (Eastsun 20260527 AAL)

extern std::map<AnsiString, AnsiString>::iterator      mapShtIter[BAR_CODE_COUNT];        // golden BarCode.h:851

extern AnsiString cMilti2DBuffer[BAR_CODE_COUNT][_MAX_SUCK_COL_ITEM * 4]; // golden BarCode.h:947

extern double     dVisionVer;            // golden BarCode.h:720 (private CCD firmware-version flag)
extern bool        bNeedCheckWhitleList;  // golden BarCode.h:965
extern int         iBarCodeNo[4][8];      // golden BarCode.h:736

extern TStringList *Socket2DReceiveList[BAR_CODE_COUNT]; // golden BarCode.h:835
extern TStringList *list2DWhitle;         // golden BarCode.h:840
extern TStringList *list2DWhitleTemp;     // golden BarCode.h:842
extern TStringList *list2DWhitleResult;   // golden BarCode.h:841

extern TQPF_Timer BarcodeAutoAdjustLightTimeOut[2]; // golden BarCode.h:878 (Eastsun 20260527 AAL)

// ---------------------------------------------------------------------------
//  The one delegating entry point (aHotPlateSubstrate.h:645 TfBarCode_Shim
//  stub signature, verbatim default arg).
// ---------------------------------------------------------------------------
bool BarCode_DoBarcodeCCDInShuttle_2(bool bVerify = false);

#endif // BarCode_Shuttle2_CCDScanH
