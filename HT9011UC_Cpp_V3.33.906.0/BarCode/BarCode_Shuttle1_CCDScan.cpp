// =============================================================================
//  BarCode/BarCode_Shuttle1_CCDScan.cpp  --  In-Shuttle-1 2D/CCD barcode scan SM
//
//  Faithful translation of golden BarCode_Sh1.cpp:82-2453
//  (TfBarCode::DoBarcodeCCDInShuttle_1(bool bVerify=false)), the largest single
//  unit in the W5-BarCode wave (~2372 golden lines).
//  Translator: AI(W5-BarCode-Shuttle1CCDScan-Translate) 20260711
//
//  ROLE: pumped from acarry.cpp's Do_Auto_SHT1 case 2400 (and 2500) while
//  In-Shuttle-1 sits at the InArm hand-off station.  Drives the Shuttle-1
//  in/out motor (MOT[MInShuttle1]) step-by-step across every site column,
//  triggers the two row CCD cameras (GetMovePos position + SendSE0 exposure +
//  SendCCDCommand "GV/GG" read-back) to read each die's 2D/DataMatrix code,
//  decodes the result into FLCarryKit.cDeviceInf[row][col], then runs the
//  duplicate-code / cross-lot-duplicate / whitelist / all-site-fail checks
//  before releasing the shuttle onward (Task=1, return true).
//
//  SCOPE / GATING MAP (golden line citations; conservative-default in every
//  gate so the SM still makes forward progress offline):
//
//   FAITHFUL / ACTIVE (translated for real, no gate):
//     * The full Task cursor switch (case 1/100/500/1000/1120/1121/1150/1160/
//       1170/1180/1200/1300/3100/3200/3300/3400/3460/3470/3500/3600/4000/4050/
//       4100/4150/4200/4300/5000/5050/5100/5150/5160/5200/6000) -- every
//       transition, retry counter, and arithmetic formula preserved verbatim.
//     * Motor moves (MOT[MInShuttle1].MotorMove), GetMovePos (golden
//       BarCode.cpp:9016 -- pure Prod.iInSHBarCodeDetectPos*/iInSHBarMulti2D*
//       table lookup, no VCL/HAL, faithfully re-implemented here as
//       CCD1_GetMovePos), FLCarryKit state reads/writes (Item/cDeviceInf/
//       iBinData/cSBin/SetItemData/HasRealIC), map2DList / map2DShuttleList
//       duplicate-code bookkeeping (pure std::map, in-memory only -- see gate
//       note below for the disk-persisted list2DByLot mirror), IndexSuckName
//       error-part strings, ShowErrorMessage/ShowMyMessage alarms,
//       DoInOutARM_SHT_MoveSafe interlock, SendHTTPRequest reuse (already a
//       real symbol via acarry_shims.cpp), fMain->Reset.
//
//   GATED (conservative offline default; TODO tag cites exact golden home):
//     1. ClientSocket-backed CCD comms -- SendCCDCommand/SendSE0/ClearBuffer
//        (golden BarCode.cpp:8929/9??? -- TfBarCode::ClearBuffer/SendCCDCommand/
//        SendSE0 ultimately write to ClientSocket_Shuttle1_A/B).  No vclcompat
//        TClientSocket shim exists yet (same class of gap as fFTPClient).
//        Offline: no-op send.  Consequence (documented, intentional): the
//        exposure-ack flags (bCCDBarcodeExposureOK/bCCDBarcodeGetResultOK/
//        bGetSE9) never flip true from a real hardware ack, so whenever this
//        SM is actually driven past the case-100 fast-path (bVerify==true, or
//        FLCarryKit really has an IC) it will cycle through its own built-in
//        retry/timeout ladder and eventually alarm (WAR0462/0463/0464) exactly
//        as golden would for a genuinely disconnected/non-responding CCD --
//        this is a faithful degraded-hardware behaviour, not a new bug.
//     2. AddCCDCommunicationLog (golden BarCode.cpp:5903) -- routed to the
//        already-real RecordProcess() so the log trail stays observable
//        instead of being a silent no-op.
//     3. mtBarcodeInSh grid (golden BarCode.h:145, TTMyTray widget) and
//        fNote->t2DCode grid (golden note.h, another un-translated VCL form)
//        -- cosmetic on-screen cell writes only; gated to no-op.
//     4. fLotInfo widgets (edCustomerLotId->Text / cbRunMode->Text /
//        mtBarcodeInShLotInfo grid -- golden uLotInfo.h, un-translated VCL
//        form) -- gated; case 3500's CustomerLotId compare and case 4200's
//        cbRunMode=="CORR" read use documented conservative stand-ins.
//     5. TfBarCode's own list2DByLot / list2DWhitleTemp / list2DWhitleResult
//        (TStringList members, golden BarCode.h:836/841/842) and
//        bSHT_2DIDIsInsideList/DoCheckSHT2DIDStatus/DoBarcodeCount/
//        RunCheckBarcodeByServerData (golden BarCode.h methods NOT among the
//        17 TfBarCode_Shim methods this wave translates) -- these are the
//        REST of BarCode.cpp/BarCode.h that no unit in this wave owns.
//        Gated with safe defaults (no whitelist violation / no all-site-fail
//        / no yield alarm).  map2DList itself (the in-memory duplicate-code
//        map) IS kept real -- only its disk-mirrored TStringList sibling and
//        the whitelist TStringLists are gated.
//     6. fMesSystem->IsMatchServerData(...) + fBarCode->RunCheckBarcodeByServerData()
//        (golden BarCode_Sh1.cpp:815/882 -- Jimmychiu 20230925 "read 2did in
//        json file") -- explicitly the "Mes/json-2DID call" class of gate
//        called out in this wave's instructions.  Offline: condition is false
//        (no MES mismatch raised).
//     7. WriteDataToFile-backed whitelist/lot-duplicate-file logging (golden
//        common.h -- itself already gated `#if 0 TODO(wave-file)` in this
//        tree) -- any call site needing it is gated alongside its owning
//        TStringList (see #5).
//     8. TMyProductionRecord::AddTestResultRecord / Add2DIDRecord /
//        AddErrorRecordNoSave -- DECLARED (Public/MyProductionRecord.h) but
//        have NO compiled body anywhere in the tree (forward-stub header);
//        calling them would be an undefined-reference LINK error.  Gated via
//        local no-op macros, following the EXACT established precedent in
//        atester_32Site.cpp (W5_32S_ADD_TEST_RESULT_RECORD).
//     9. ShowMyMessageBox_YES_SKIP (golden mymessbox.h, modal Yes/No dialog,
//        not translated) -- offline default: "not Yes" (Skip), so the
//        all-site-2DID-fail retry loop (case 6000) does not spin forever
//        waiting for an operator answer.
//    10. cbBarCodeSimulate->Checked (golden TCheckBox) / GetSimuBarCodeName /
//        Socket2DReceiveList (golden BarCode.h:835, TStringList[8] simulate
//        injection) -- gated; offline stand-in is a plain `static bool`
//        defaulting false (no simulate), matching production behaviour.
//    11. CCDBarcode_SortingForMulti2D (golden BarCode.h:830) -- no-op; per the
//        analysis in this unit's translate report, the success branch that
//        calls it is only reachable once a real CCD ack exists (gate #1), so
//        this is inert in the offline build regardless.
//
//  SHARED-STATE CAVEAT (flag for the integrate agent): every array/map/timer
//  below marked "golden BarCode.h:NNN (TfBarCode member)" is, in the real
//  form, SHARED across ALL TfBarCode methods (this function plus
//  InitialBarcodeScanInShuttle1 / DoBarcodeScanInShuttle_1 /
//  DoBarcodeTriggerInShuttle_1 / DoBarcodeScanOutShuttle_1 / ... -- other
//  units in this SAME parallel wave).  This unit materialises its OWN
//  `static` (internal-linkage, translation-unit-local) copies so it compiles
//  and links standalone with zero ODR risk against sibling units that may
//  declare identically-named copies in THEIR OWN .cpp files.  The state is
//  therefore NOT actually shared with sibling BarCode methods yet -- a later
//  consolidation pass (mirroring how aHotPlateSubstrate.h centralises
//  in/out-arm substrate state) should hoist these into one shared BarCode
//  substrate header once all/most sibling methods have landed.  See this
//  unit's translate report item 6 for the full list.
//
//  Big5: this file introduces no new Chinese string literals (all Chinese
//  golden comments are glossed in English + golden line citations per the
//  W5-BarCode cp950 rule).  Zero U+FFFD verified before hand-off.
// =============================================================================
#include "BarCode_Shuttle1_CCDScan.h"

// NOTE: bare (root-relative) includes, matching this project's established
// convention for other subdirectory translation units (e.g. CCLink/
// MyCCLinkSensor_predicates.cpp, Automation/AGV_predicates.cpp,
// Automation/HANA_ART.cpp) -- ht9045_sm's CMake target adds the repo root as
// a PUBLIC include directory, so a bare `"aHotPlateSubstrate.h"` resolves
// correctly regardless of this file's own subdirectory.
#include "aHotPlateSubstrate.h"   // TMyKitSuck / InArmSuck / FLCarryKit / TMyProductionRecord / TQPF_Timer
#include "Motor/mymotor.h"        // MOT[] motor array
#include "cprod.h"                // Prod / TestIF_File / CosFunction / IniConfig / MachineType.h / <map>
#include "cmydef.h"               // K_RETRY/K_SKIP/K_RESET/TEST_PASS/MInShuttle1/MMSystem/IndexSuckName/
                                  // HAS_NULL_IC/NULL_IC/HAS_IC/HAS_HOT_IC/OFF_LINE/BAR_CODE_INSTALL/
                                  // SPIL_FOR_QLE/asBarCodeErrorSend/asBarCodeErrorCheckSum/iBarcodeReject/
                                  // iContactMode/bStartAutoAdjustLight/bBarcodeNeedAutoAdjust
#include "canary_support.h"       // ShowErrorMessage / ShowMyMessage / RecordProcess / LastSet
#include "atester_shims.h"        // fContact (TfContactShim::IsRun2DCheck) / IsNNMode()
#include "acarry_shims.h"         // fLtcSensor (TfLtcSensor::ClearLtcSensor)
#include "acarry.h"               // DoInOutARM_SHT_MoveSafe
#include "FormsFacade.h"          // fMain (Reset)

#include <cstdlib>                   // atoi
#include <cstring>                   // memset (ZeroMemory stand-in, see below)

// ---------------------------------------------------------------------------
//  local constants this unit needs that live in golden headers NOT owned by
//  ANY unit in this wave (the rest of BarCode.h / cContact.h).  Values copied
//  verbatim from golden so comparisons stay bit-identical; `static`/anonymous
//  scope so there is zero possibility of a link-time collision with a sibling
//  TU (enums/consts carry no external symbol regardless, but kept internal
//  for clarity).
// ---------------------------------------------------------------------------
namespace
{
    // golden BarCode/BarCode.h:30
    const int kBarCodeCount = 8;

    // golden BarCode.cpp:217-219 (TfBarCode ctor) -- fixed row-slot assignment;
    // Shuttle-1 always uses slots 0/1 unless TestIF_File.iSelectUseCCDSh1 swaps
    // them (handled inline, matching golden, not baked into these constants).
    const int kBarCode1_1 = 0;
    const int kBarCode1_2 = 1;

    // golden BarCode/BarCode.h:84-90 (enum eMulti2DType)
    enum eMulti2DType
    {
        e1x2In1CCD = 0,
        e2x1In1CCD = 1,
        e2x1In2CCD = 2,
        e2x2In1CCD = 3,
        e2x2In2CCD = 4,
        eMulti2DTypeTotal
    };

    // golden cContact.cpp:74-77 (extern const int, real body lives in the
    // un-translated cContact.cpp/.h; values copied verbatim so the
    // iContactMode comparisons below stay bit-identical).
    const int kContactNormal         = 0;  // CONTACT_NORMAL
    const int kContactAutoGetHeight  = 1;  // CONTACT_AUTO_GET_HEIGHT
    const int kContactTest           = 3;  // CONTACT_TEST
}

// ---------------------------------------------------------------------------
//  Shared TfBarCode instance-state substitutes (see file banner "SHARED-STATE
//  CAVEAT").  `static` = internal linkage, this TU only.
// ---------------------------------------------------------------------------
namespace
{
    // golden BarCode.h:855/856 -- confirm move-position matches capture position
    int  iCurrentStep[kBarCodeCount];
    bool bStepError[kBarCodeCount];

    // golden BarCode.h:742/744/748 -- per-row exposure/decode/SE9-ack flags
    bool bCCDBarcodeExposureOK[kBarCodeCount];
    bool bCCDBarcodeGetResultOK[kBarCodeCount];
    bool bGetSE9[kBarCodeCount];

    // golden BarCode.h:928 (Eastsun 20260527 AAL) -- per-side auto-adjust-light flag
    bool bCCDBarcodeAutoAdjustLight[kBarCodeCount];

    // golden BarCode.h:869-871/878 -- per-shuttle/per-row timers
    TQPF_Timer BarcodePosDelay[2];
    TQPF_Timer BarcodeExposureDelay[2];
    TQPF_Timer BarcodeAutoAdjustLightTimeOut[2];
    TQPF_Timer BarcodeDelay[kBarCodeCount];

    // golden BarCode.h:720 -- vision-software protocol version.  Default 2
    // (the modern "GV,0/GG,0" protocol branch -- Steven/wei 20170119) matches
    // the common current-generation CCD board configuration.
    double dVisionVer = 2;

    // golden BarCode.h:852/850-851 -- in-memory duplicate-code bookkeeping.
    // map2DList/iShtDupilcateFailCnt are ALSO touched by the sibling
    // InitialBarcodeScanInShuttle1 unit (see file banner caveat).
    std::map<AnsiString, AnsiString> map2DList;
    std::map<AnsiString, AnsiString>::iterator map2DIter;
    std::map<AnsiString, AnsiString> map2DShuttleList[kBarCodeCount];
    std::map<AnsiString, AnsiString>::iterator mapShtIter[kBarCodeCount];
    int iShtDupilcateFailCnt[2];

    // NOTE: iBarcodeDuplicate/iNeedBarcodeCount/iBarcodeErrorCount/
    // iBarcodePassCount (golden BarCode.h per-row duplicate/pass/error/need
    // counters) are REAL, already-translated globals (cmydef.h:4062-4066,
    // `extern int ...[4]`) -- reused directly below, not re-declared here.

    // golden BarCode.h:965 -- 2DID white-list feature enable flag (gated
    // block reads it only for documentation; the block itself is inert).
    bool bNeedCheckWhitleList = false;

    // -- gated widget/cross-module stand-ins (see banner gates #3/#4/#10) ---
    // golden BarCode.h:539 (cbBarCodeSimulate TCheckBox->Checked)
    bool g_cbBarCodeSimulateChecked = false;
    // golden uLotInfo.h (fLotInfo->cbRunMode TComboBox->Text=="CORR")
    bool g_LotInfoRunModeIsCorr = false;
}

// ---------------------------------------------------------------------------
//  Gate #1: ClientSocket-backed CCD comms.  No vclcompat TClientSocket shim
//  exists yet.  See banner gate #1 for the documented behavioural
//  consequence (SM degrades to its own timeout/retry/alarm ladder).
// ---------------------------------------------------------------------------
static void CCD1_ClearBuffer(int /*iShuttle*/, AnsiString /*Log*/)
{
    // TODO(W5-BarCode-Shuttle1CCD-socket) golden BarCode.cpp:8929
    // (TfBarCode::ClearBuffer) -- sends "SE0,9,.."/"E9,0,.." over
    // ClientSocket_Shuttle1_A/B.  Offline: no-op (no real CCD to clear).
}
static void CCD1_SendCCDCommand(int /*index*/, AnsiString /*Msg*/, AnsiString /*Msg2*/ = "")
{
    // TODO(W5-BarCode-Shuttle1CCD-socket) golden BarCode.cpp
    // (TfBarCode::SendCCDCommand) -- sends a raw command string over
    // ClientSocket_Shuttle1_A/B.  Offline: no-op.
}
static void CCD1_SendSE0(int /*iShuttle*/, int /*iHasIC*/, int /*iNowMoveStep*/,
                         int /*iNowMoveStepMulti2D*/, AnsiString /*Log*/)
{
    // TODO(W5-BarCode-Shuttle1CCD-socket) golden BarCode.cpp
    // (TfBarCode::SendSE0) -- sends the per-site "SE0,<hasIC>" exposure-enable
    // command over ClientSocket_Shuttle1_A/B.  Offline: no-op (so
    // bCCDBarcodeExposureOK/bGetSE9 never flip true from a real ack -- see
    // banner gate #1).
}
static void CCD1_AddCCDCommunicationLog(int /*index*/, AnsiString Str)
{
    // TODO(W5-BarCode-Shuttle1CCD-socket) golden BarCode.cpp:5903
    // (TfBarCode::AddCCDCommunicationLog) -- real body appends to the
    // on-screen CCD comm-log memo + a per-index log file.  Routed to the
    // already-real RecordProcess() so the trail stays observable (banner
    // gate #2), not a silent no-op.
    RecordProcess(Str);
}
static void CCD1_CCDBarcode_SortingForMulti2D(int /*iSht*/)
{
    // TODO(W5-BarCode-Shuttle1CCD-socket) golden BarCode.h:830
    // (TfBarCode::CCDBarcode_SortingForMulti2D) -- merges multi-2D partial
    // reads into FLCarryKit.cDeviceInf.  Inert offline (banner gate #11).
}

// ---------------------------------------------------------------------------
//  Gate #3: cosmetic grid widgets (mtBarcodeInSh / fNote->t2DCode).
// ---------------------------------------------------------------------------
static void CCD1_GridSetCell(int /*iRow*/, int /*iCol*/, AnsiString /*Text*/)
{
    // TODO(W5-BarCode-Shuttle1CCD-widget) golden BarCode.h:145 (mtBarcodeInSh,
    // TTMyTray custom-grid widget, VCL, W7).  Offline no-op; control flow
    // never depends on grid contents.
}
static void CCD1_NoteShowDupGrid()
{
    // TODO(W5-BarCode-Shuttle1CCD-widget) golden note.h (TfNote::t2DCode
    // debug 2DID display grid, VCL, W7).  Offline no-op.
}

// ---------------------------------------------------------------------------
//  Gate #9: modal Yes/No dialog (all-site-2DID-fail retry prompt).
// ---------------------------------------------------------------------------
static int CCD1_ShowMyMessageBox_YES_SKIP(AnsiString /*Msg*/, AnsiString /*Caption*/)
{
    // TODO(W5-BarCode-Shuttle1CCD-widget) golden mymessbox.h
    // (ShowMyMessageBox_YES_SKIP) -- modal Yes/No VCL dialog, not translated.
    // Offline conservative default: "not Yes" (Skip) so case 6000's retry
    // loop cannot spin forever waiting on an operator answer.
    return 0;   // golden: 1=Yes; anything else=Skip
}

// ---------------------------------------------------------------------------
//  Gate #5: rest-of-BarCode.h methods this wave does not own.
// ---------------------------------------------------------------------------
static bool CCD1_DoCheckSHT2DIDStatus(int /*iSht*/)
{
    // TODO(W5-BarCode-rest) golden BarCode.h (TfBarCode::DoCheckSHT2DIDStatus)
    // -- "all sites 2DID failed" detector; body lives in the un-owned rest of
    // BarCode.cpp.  Offline: false (no all-site-fail), the safe/common branch.
    return false;
}
static bool CCD1_DoBarcodeCount()
{
    // TODO(W5-BarCode-rest) golden BarCode.cpp:5841 (TfBarCode::DoBarcodeCount,
    // "Barcode Rate計算" Yield-alarm feature) -- body lives in the un-owned
    // rest of BarCode.cpp.  Offline: false (no yield alarm raised).
    return false;
}
static bool CCD1_RunCheckBarcodeByServerData()
{
    // TODO(W5-BarCode-Mes) golden BarCode_Sh1.cpp:815/882
    // (fBarCode->RunCheckBarcodeByServerData() && fMesSystem->IsMatchServerData(...),
    // Jimmychiu 20230925 "read 2did in json file") -- the Mes/json-2DID call
    // class of gate.  Offline: false (no MES mismatch raised).
    return false;
}

// ---------------------------------------------------------------------------
//  Gate #8: TMyProductionRecord methods declared (Public/MyProductionRecord.h)
//  but with NO compiled body anywhere in the tree -- calling them is an
//  undefined-reference LINK error.  Local no-op macros, exact precedent:
//  atester_32Site.cpp's W5_32S_ADD_TEST_RESULT_RECORD.
// ---------------------------------------------------------------------------
#define CCD1_ADD_TEST_RESULT_RECORD(kit, r, c, bin, sbin, tag) do{}while(0)
#define CCD1_ADD_TEST_RESULT_RECORD2(kit, r, c, bin, tag)      do{}while(0)
#define CCD1_ADD_2DID_RECORD(kit, r, c, code)                  do{}while(0)
#define CCD1_ADD_ERROR_RECORD_NOSAVE(kit, r, c, msg)           do{}while(0)

// ---------------------------------------------------------------------------
//  GetMovePos -- golden BarCode.cpp:9016 (TfBarCode::GetMovePos).  PURE
//  Prod-table lookup keyed by InArmSuck.iShtCol; no VCL/HAL touch at all, so
//  faithfully re-implemented here in full (NOT gated).
// ---------------------------------------------------------------------------
static int CCD1_GetMovePos(int iSht, int iNowMoveStep, int iNowMoveStepMulti2DX, bool bAlarm = true)
{
    int pos = 0;
    if (TestIF_File.bEnableMulti2D &&
        (TestIF_File.iMulti2DType == e1x2In1CCD ||
         TestIF_File.iMulti2DType == e2x2In1CCD ||
         TestIF_File.iMulti2DType == e2x2In2CCD))                              //Steven 20240612 : for 對角2D
    {
        if (InArmSuck.iShtCol == 1)
            pos = Prod.iInSHBarMulti2DDetectPos1x1[iSht][iNowMoveStepMulti2DX];
        else if (InArmSuck.iShtCol == 2)
            pos = Prod.iInSHBarMulti2DDetectPos1x2[iSht][iNowMoveStep][iNowMoveStepMulti2DX];
        else if (InArmSuck.iShtCol == 3)                                        //ChungHung 20140115 add for 2x3_6
            pos = Prod.iInSHBarMulti2DDetectPos2x3[iSht][iNowMoveStep][iNowMoveStepMulti2DX];
        else if (InArmSuck.iShtCol == 4)
            pos = Prod.iInSHBarMulti2DDetectPos1x4[iSht][iNowMoveStep][iNowMoveStepMulti2DX];
        else if (InArmSuck.iShtCol == 5)                                        //Steven 20221027 : Add for 2x5
            pos = Prod.iInSHBarMulti2DDetectPos2x5[iSht][iNowMoveStep][iNowMoveStepMulti2DX];
        else if (InArmSuck.iShtCol == 6)                                        //Steven 20170111 : fixed for 2x6
            pos = Prod.iInSHBarMulti2DDetectPos2x6[iSht][iNowMoveStep][iNowMoveStepMulti2DX];
        else if (InArmSuck.iShtCol == 8)
            pos = Prod.iInSHBarMulti2DDetectPos2x8[iSht][iNowMoveStep][iNowMoveStepMulti2DX];
        else
        {
            if (bAlarm)
                ShowMyMessage("The mode is not support!!", "Please save state record and provide to HonPrec software engineer", "TfBarCode::GetMovePos");
        }
    }
    else
    {
        if (InArmSuck.iShtCol == 1)
            pos = Prod.iInSHBarCodeDetectPos1x1[iSht];
        else if (InArmSuck.iShtCol == 2)
            pos = Prod.iInSHBarCodeDetectPos1x2[iSht][iNowMoveStep];
        else if (InArmSuck.iShtCol == 3)                                        //ChungHung 20140115 add for 2x3_6
            pos = Prod.iInSHBarCodeDetectPos2x3[iSht][iNowMoveStep];
        else if (InArmSuck.iShtCol == 4)
            pos = Prod.iInSHBarCodeDetectPos1x4[iSht][iNowMoveStep];
        else if (InArmSuck.iShtCol == 5)                                        //Steven 20221027 : Add for 2x5
            pos = Prod.iInSHBarCodeDetectPos2x5[iSht][iNowMoveStep];
        else if (InArmSuck.iShtCol == 6)                                        //Steven 20170111 : fixed for 2x6
            pos = Prod.iInSHBarCodeDetectPos2x6[iSht][iNowMoveStep];
        else if (InArmSuck.iShtCol == 8)
            pos = Prod.iInSHBarCodeDetectPos2x8[iSht][iNowMoveStep];
        else
        {
            if (bAlarm)
                ShowMyMessage("The mode is not support!!", "Please save state record and provide to HonPrec software engineer", "TfBarCode::GetMovePos");
        }
    }
    return pos;
}

// =============================================================================
//  BarCode_DoBarcodeCCDInShuttle_1  --  golden BarCode_Sh1.cpp:83-2453
// =============================================================================
bool BarCode_DoBarcodeCCDInShuttle_1(bool bVerify)
{
    static int iNowMoveStep = 0, iNowCheckStep = 0;
    static int iNowMoveStepMulti2D = 0, iNowMoveStepMulti2DX = 0, iNowMoveStepMulti2DY = 0;
    static int iRetryCount = 0, iStepCount = 0;
    static int iClearBuffRetry = 0;                                            //Steven 20241108 : flag分開處理 (separate the flag)
    static int iRetryCountExpTimo = 0;                                         //Steven 20160425 : exposure timeout retry
    static int iHttpErrRetryCT = 0;
    static int i2DIDRetryCTSH1 = 0;                                            //Isaac 20200206 : 2DID All site fail recheck twice and alarm
    static bool bHasError = false;
    static bool bLotCheckError = false;                                        //Alick 20170313 (wei) add
    static bool bShtHasDupError = false;
    static bool bLotHasDupError = false;
    static bool bNeesScanCode[4][8] = {};
    static bool bHasErr[4][8] = {};
    static bool bAlarm = false;
    static AnsiString ErrPart;
    static AnsiString sErrorPart = "", sSimuCode[4];

    // golden Task binds to instance member iInitialBarcodeInShuttle1Task; this
    // unit owns only DoBarcodeCCDInShuttle_1, so the cursor is a plain
    // function-local static (persists across calls, matches golden semantics
    // for this function; the sibling init method that also touches the real
    // member is out of this unit's scope -- see file banner caveat).
    static int Task = 1;

    int pos = 0, ret = 0;
    int iSht = 0;
    int MSht = MInShuttle1;
    int iBarCodeRowA = kBarCode1_1;
    int iBarCodeRowB = kBarCode1_2;
    // NOTE: golden also declares `iPos`/`iIndex` here -- both are only used
    // inside the gated case-3500 (Lot-check) / case-4200 (whitelist) bodies
    // (banner gates #4/#5), so they are omitted here to avoid unused-variable
    // warnings; re-add if those gates are ever lifted.
    AnsiString sCCDCommand;
    AnsiString Log, sLog;
    AnsiString Str, Str1, Str2, Str3;

    if (InArmSuck.iShtRow == 1 &&                                              //Steven 20240227 : Shuttle使用一排的 (single-row shuttle)
        BAR_CODE_INSTALL != ebcUseOCR)                                         //Ifor 20220322 add 自制OCR不使用 (not used with in-house OCR)
    {
        if (TestIF_File.iSelectUseCCDSh1 == 0)
        {
            iBarCodeRowA = kBarCode1_1;
            iBarCodeRowB = kBarCode1_2;
        }
        else
        {
            iBarCodeRowA = kBarCode1_2;
            iBarCodeRowB = kBarCode1_1;
        }
    }

    switch (Task)
    {
        case 1:                                                                //jou 20170509 (Steven) : 2D alway retry
            iRetryCount = 0;
            iClearBuffRetry = 0;
            i2DIDRetryCTSH1 = 0;                                               //Isaac 20200206
            Task = 100;
            // fall through
        case 100:
            if (bVerify == false &&
                FLCarryKit.HasRealIC() == false)
                return true;

            iNowMoveStep = InArmSuck.iShtCol - 1;                              //遞減方式檢查 (decrement-style scan)

            if (TestIF_File.bEnableMulti2D)                                    //Steven 20200810 : 一個IC使用多個2DID
            {
                iNowMoveStepMulti2D  = TestIF_File.iMulti2DCount - 1;
                iNowMoveStepMulti2DY = TestIF_File.iMulti2DYItem - 1;
                iNowMoveStepMulti2DX = TestIF_File.iMulti2DXItem - 1;
            }
            else
            {
                iNowMoveStepMulti2D  = 0;
                iNowMoveStepMulti2DX = 0;
                iNowMoveStepMulti2DY = 0;
            }
            iCurrentStep[iBarCodeRowA] = iNowMoveStep;                         //Steven 20160503 : 確認移動位置與拍照位置相同
            iCurrentStep[iBarCodeRowB] = iNowMoveStep;
            bStepError[iBarCodeRowA] = false;
            bStepError[iBarCodeRowB] = false;
            bCCDBarcodeExposureOK[iBarCodeRowA] = true;
            bCCDBarcodeExposureOK[iBarCodeRowB] = true;
            bCCDBarcodeGetResultOK[iBarCodeRowA] = true;
            bCCDBarcodeGetResultOK[iBarCodeRowB] = true;
            iRetryCountExpTimo = 0;

            for (int i = 0; i < FLCarryKit.iMaxCol; i++)                       //清空畫面 (clear the on-screen grid -- gated)
            {
                CCD1_GridSetCell(iBarCodeRowA, i + 1, "");
                CCD1_GridSetCell(iBarCodeRowB, i + 1, "");

                bNeesScanCode[0][i] = false;
                bNeesScanCode[1][i] = false;
            }

            FLCarryKit.cDeviceInf[0][iNowMoveStep] = "";
            FLCarryKit.cDeviceInf[1][iNowMoveStep] = "";
            bHasError = false;
            iStepCount = 0;

            if (dVisionVer >= 2)                                               //wei 20170119 (Steven) 版本號大於2
            {
                sSimuCode[iBarCodeRowA] = "GV,0,1,11111111,";
                sSimuCode[iBarCodeRowB] = "GV,0,1,11111111,";
            }
            else
            {
                sSimuCode[iBarCodeRowA] = "GV,1,11111111,";
                sSimuCode[iBarCodeRowB] = "GV,1,11111111,";
            }

            bGetSE9[iBarCodeRowA] = false;
            bGetSE9[iBarCodeRowB] = false;
            CCD1_AddCCDCommunicationLog(iBarCodeRowA, "<<Cycle, Cycle start.------------------");
            CCD1_ClearBuffer(iBarCodeRowA, "Clear buffer 1");
            CCD1_ClearBuffer(iBarCodeRowB, "Clear buffer 1");
            BarcodePosDelay[iSht].SetSecAndOn(5);
            Task = 500;
            // fall through
        case 500:
            if (bGetSE9[iBarCodeRowA] && bGetSE9[iBarCodeRowB])
            {
                Task = 1000;
                iClearBuffRetry = 0;
            }
            else if (g_cbBarCodeSimulateChecked)
            {
                Task = 1000;
                iClearBuffRetry = 0;
            }
            else if (BarcodePosDelay[iSht].Off())
            {
                if (iClearBuffRetry >= 3)
                {
                    ShowErrorMessage("WAR0462", K_RETRY, MSht, false, "Clear buffer");
                    Task = 1;
                }
                else                                                           //Steven 20240821 : Retry一次
                {
                    bGetSE9[iBarCodeRowA] = false;
                    bGetSE9[iBarCodeRowB] = false;
                    iClearBuffRetry++;
                    CCD1_ClearBuffer(iBarCodeRowA, "Clear buffer 500-" + AnsiString(iClearBuffRetry));
                    CCD1_ClearBuffer(iBarCodeRowB, "Clear buffer 500-" + AnsiString(iClearBuffRetry));
                    BarcodePosDelay[iSht].SetSecAndOn(5);
                }
            }
            break;
        case 1000:
            if (InArmSuck.iShtRow == 2)
            {
                if (bVerify == false &&
                    (FLCarryKit.Item[0][iNowMoveStep] == HAS_NULL_IC || FLCarryKit.Item[0][iNowMoveStep] == NULL_IC) &&
                    (FLCarryKit.Item[1][iNowMoveStep] == HAS_NULL_IC || FLCarryKit.Item[1][iNowMoveStep] == NULL_IC))
                {
                    BarcodePosDelay[iSht].SetMSAndOn(1);                       //Steven 20160307 : Go 1120 need reset delay
                    Task = 1120;
                    break;
                }
            }
            else
            {
                if (bVerify == false &&
                    (FLCarryKit.Item[0][iNowMoveStep] == HAS_NULL_IC || FLCarryKit.Item[0][iNowMoveStep] == NULL_IC))
                {
                    BarcodePosDelay[iSht].SetMSAndOn(1);
                    Task = 1120;
                    break;
                }
            }

            pos = CCD1_GetMovePos(iSht, iNowMoveStep, iNowMoveStepMulti2DX);
            pos += (int)Offset.iSHLeft2D[iSht];

            if (MOT[MSht].MotorMove(pos))
            {
                if (TestIF_File.bEnableMulti2D)
                    Log.sprintf("Action, Shuttle %d move to step %d-%d (%d, %d)", iSht + 1, iNowMoveStep, iNowMoveStepMulti2D, iNowMoveStepMulti2DY, iNowMoveStepMulti2DX);
                else
                    Log.sprintf("Action, Shuttle %d move to step %d", iSht + 1, iNowMoveStep);
                CCD1_AddCCDCommunicationLog(iBarCodeRowA, Log);
                if (iContactMode == kContactNormal)
                {
                    if (iStepCount == 0)
                        BarcodePosDelay[iSht].SetMSAndOn(TestIF_File.iBarCodePos1Delay);
                    else
                        BarcodePosDelay[iSht].SetMSAndOn(TestIF_File.iBarCodePosDelay);
                }
                else
                {
                    BarcodePosDelay[iSht].SetMSAndOn(100);
                }
                iStepCount++;
                Task = 1120;
            }
            break;
        case 1120:
            if (BarcodePosDelay[iSht].Off())                                   //到位Delay (arrival delay)
            {
                //==> Eastsun 20260527 整合#028.AAL.P11a Sh1 case 1120 AAL wrap-head
                for (int i = 0; i < 2; i++)
                {
                    if (CosFunction.bUseBarcodeAutoAdjustLight == true && TestIF_File.bUseBarcodeAutoAdjustLight == true && bStartAutoAdjustLight == true)
                    {
                        if (bBarcodeNeedAutoAdjust[i] == true && (FLCarryKit.Item[i][iNowMoveStep] == HAS_IC || FLCarryKit.Item[i][iNowMoveStep] == HAS_HOT_IC))
                        {
                            sCCDCommand = "SE0,2";
                            bCCDBarcodeAutoAdjustLight[i] = true;
                            CCD1_SendCCDCommand(iBarCodeRowA + i, "AutoAdjustLight 1120: ", sCCDCommand);
                        }
                        else
                        {
                            bCCDBarcodeAutoAdjustLight[i] = false;
                        }
                    }
                    else
                    {
                        bCCDBarcodeAutoAdjustLight[i] = false;
                    }
                }

                if (bCCDBarcodeAutoAdjustLight[0] == true || bCCDBarcodeAutoAdjustLight[1] == true)
                {
                    BarcodeAutoAdjustLightTimeOut[iSht].SetSecAndOn(TestIF_File.iAutoAdjustLightTimeOut);
                    Task = 1121;
                }
                else
                {
                    //<== Eastsun 20260527 整合#028.AAL.P11a
                    bCCDBarcodeExposureOK[iBarCodeRowA] = false;
                    if (InArmSuck.iShtRow == 2)
                        bCCDBarcodeExposureOK[iBarCodeRowB] = false;
                    else if (TestIF_File.bEnableMulti2D &&                      //Steven 20240612 : for 對角2D
                             (TestIF_File.iMulti2DType == e2x1In2CCD ||
                              TestIF_File.iMulti2DType == e2x2In2CCD))
                        bCCDBarcodeExposureOK[iBarCodeRowB] = false;
                    else
                        bCCDBarcodeExposureOK[iBarCodeRowB] = true;

                    sCCDCommand = "SE0,";

                    if (InArmSuck.iShtRow == 2)
                    {
                        if (bVerify == false &&
                            (FLCarryKit.Item[0][iNowMoveStep] == HAS_NULL_IC ||
                             FLCarryKit.Item[0][iNowMoveStep] == NULL_IC))
                        {
                            CCD1_SendSE0(iBarCodeRowA, false, iNowMoveStep, iNowMoveStepMulti2D, "exposure 1120: ");
                            bNeesScanCode[0][iNowMoveStep] = false;
                        }
                        else
                        {
                            CCD1_SendSE0(iBarCodeRowA, true, iNowMoveStep, iNowMoveStepMulti2D, "exposure 1120: ");
                            bNeesScanCode[0][iNowMoveStep] = true;
                        }

                        if (bVerify == false &&
                            (FLCarryKit.Item[1][iNowMoveStep] == HAS_NULL_IC ||
                             FLCarryKit.Item[1][iNowMoveStep] == NULL_IC))
                        {
                            CCD1_SendSE0(iBarCodeRowB, false, iNowMoveStep, iNowMoveStepMulti2D, "exposure 1120: ");
                            bNeesScanCode[1][iNowMoveStep] = false;
                        }
                        else
                        {
                            CCD1_SendSE0(iBarCodeRowB, true, iNowMoveStep, iNowMoveStepMulti2D, "exposure 1120: ");
                            bNeesScanCode[1][iNowMoveStep] = true;
                        }
                    }
                    else if (TestIF_File.bEnableMulti2D &&                      //Steven 20240612 : for 對角2D
                             (TestIF_File.iMulti2DType == e2x1In2CCD ||
                              TestIF_File.iMulti2DType == e2x2In2CCD))
                    {
                        if (bVerify == false &&
                            (FLCarryKit.Item[0][iNowMoveStep] == HAS_NULL_IC ||
                             FLCarryKit.Item[0][iNowMoveStep] == NULL_IC))
                        {
                            CCD1_SendSE0(iBarCodeRowA, false, iNowMoveStep, iNowMoveStepMulti2D, "exposure 1120: ");
                            CCD1_SendSE0(iBarCodeRowB, false, iNowMoveStep, iNowMoveStepMulti2D, "exposure 1120: ");
                            bNeesScanCode[0][iNowMoveStep] = false;
                        }
                        else
                        {
                            if (TestIF_File.iMulti2DMap[0][iNowMoveStepMulti2D] > 0)
                                CCD1_SendSE0(iBarCodeRowA, true, iNowMoveStep, iNowMoveStepMulti2D, "exposure 1120: ");
                            else
                                CCD1_SendSE0(iBarCodeRowA, false, iNowMoveStep, iNowMoveStepMulti2D, "exposure 1120: ");

                            if (TestIF_File.iMulti2DMap[1][iNowMoveStepMulti2D] > 0)
                                CCD1_SendSE0(iBarCodeRowB, true, iNowMoveStep, iNowMoveStepMulti2D, "exposure 1120: ");
                            else
                                CCD1_SendSE0(iBarCodeRowB, false, iNowMoveStep, iNowMoveStepMulti2D, "exposure 1120: ");
                            bNeesScanCode[0][iNowMoveStep] = true;
                        }
                    }
                    else
                    {
                        if (bVerify == false &&
                            (FLCarryKit.Item[0][iNowMoveStep] == HAS_NULL_IC ||
                             FLCarryKit.Item[0][iNowMoveStep] == NULL_IC))
                        {
                            CCD1_SendSE0(iBarCodeRowA, false, iNowMoveStep, iNowMoveStepMulti2D, "exposure 1120: ");
                            bNeesScanCode[0][iNowMoveStep] = false;
                        }
                        else
                        {
                            CCD1_SendSE0(iBarCodeRowA, true, iNowMoveStep, iNowMoveStepMulti2D, "exposure 1120: ");
                            bNeesScanCode[0][iNowMoveStep] = true;
                        }
                    }

                    Task = 1150;
                    iRetryCountExpTimo = 0;
                    BarcodeExposureDelay[iSht].SetMSAndOn(TestIF_File.i2DTriggerTime);
                }
            }
            break;
        //==> Eastsun 20260527 整合#028.AAL.P12 Sh1 case 1121 AAL exposure wait
        case 1121:
            if (bCCDBarcodeAutoAdjustLight[0] == false && bCCDBarcodeAutoAdjustLight[1] == false)
            {
                if (bBarcodeNeedAutoAdjust[0] == false && bBarcodeNeedAutoAdjust[1] == false &&
                    bBarcodeNeedAutoAdjust[2] == false && bBarcodeNeedAutoAdjust[3] == false)
                {
                    bStartAutoAdjustLight = false;
                }
                Task = 1120;
            }

            if (BarcodeAutoAdjustLightTimeOut[iSht].Off())
            {
                ret = ShowErrorMessage("WAR16199", K_RETRY | K_SKIP, MSht);
                if (ret == K_SKIP)
                {
                    bStartAutoAdjustLight = false;
                    for (int i = 0; i < 4; i++)
                        bBarcodeNeedAutoAdjust[i] = false;
                }
                Task = 1120;
            }
            break;
        //<== Eastsun 20260527 整合#028.AAL.P12
        case 1150:
            if (bStepError[iBarCodeRowA] == true ||
                bStepError[iBarCodeRowB] == true)                              //Steven 20160503 : 確認移動位置與拍照位置相同
            {
                ErrPart = "";
                if (IsNNMode() == NN_2Row)                                      //Steven 20220315 : for 2did nn mode alarm
                {
                    if (bStepError[iBarCodeRowA])
                        ErrPart += IndexSuckName[2][iNowMoveStep];
                    if (bStepError[iBarCodeRowB])
                        ErrPart += IndexSuckName[3][iNowMoveStep];
                }
                else if (IsNNMode() == NN_1Row)
                {
                    if (bStepError[iBarCodeRowA] || bStepError[iBarCodeRowB])
                        ErrPart += IndexSuckName[1][iNowMoveStep];
                }
                else if (TestIF_File.bEnableMulti2D &&                          //Steven 20240612 : for 對角2D
                         (TestIF_File.iMulti2DType == e2x1In2CCD ||
                          TestIF_File.iMulti2DType == e2x2In2CCD))
                {
                    if (bStepError[iBarCodeRowA] || bStepError[iBarCodeRowB])
                        ErrPart += IndexSuckName[0][iNowMoveStep];
                }
                else
                {
                    if (bStepError[iBarCodeRowA])
                        ErrPart += IndexSuckName[0][iNowMoveStep];
                    if (bStepError[iBarCodeRowB])
                        ErrPart += IndexSuckName[1][iNowMoveStep];
                }

                Task = 1180;
            }
            else if (bCCDBarcodeExposureOK[iBarCodeRowA] == true &&
                     bCCDBarcodeExposureOK[iBarCodeRowB] == true)               //拍照完成 (exposure complete)
            {
                if (InArmSuck.iShtRow == 2)
                {
                    if (bVerify == true ||
                        (FLCarryKit.Item[0][iNowMoveStep] != HAS_NULL_IC &&
                         FLCarryKit.Item[0][iNowMoveStep] != NULL_IC))
                        CCD1_GridSetCell(iBarCodeRowA, iNowMoveStep + 1, "Waiting result..");

                    if (bVerify == true ||
                        (FLCarryKit.Item[1][iNowMoveStep] != HAS_NULL_IC &&
                         FLCarryKit.Item[1][iNowMoveStep] != NULL_IC))
                        CCD1_GridSetCell(iBarCodeRowB, iNowMoveStep + 1, "Waiting result..");
                }
                else if (TestIF_File.bEnableMulti2D &&
                         (TestIF_File.iMulti2DType == e2x1In2CCD ||
                          TestIF_File.iMulti2DType == e2x2In2CCD))
                {
                    if (bVerify == true ||
                        (FLCarryKit.Item[0][iNowMoveStep] != HAS_NULL_IC &&
                         FLCarryKit.Item[0][iNowMoveStep] != NULL_IC))
                    {
                        CCD1_GridSetCell(iBarCodeRowA, iNowMoveStep + 1, "Waiting result..");
                        CCD1_GridSetCell(iBarCodeRowB, iNowMoveStep + 1, "Waiting result..");
                    }
                }
                else
                {
                    if (bVerify == true ||
                        (FLCarryKit.Item[0][iNowMoveStep] != HAS_NULL_IC &&
                         FLCarryKit.Item[0][iNowMoveStep] != NULL_IC))
                        CCD1_GridSetCell(iBarCodeRowA, iNowMoveStep + 1, "Waiting result..");
                }
                Task = 1200;
            }
            else if (g_cbBarCodeSimulateChecked)
            {
                // golden: simulate branch appends via GetSimuBarCodeName() +
                // Socket2DReceiveList[]->Add(...); neither is translated (both
                // are dead in production -- cbBarCodeSimulate is a bench-test
                // aid).  Offline this branch is unreachable (flag defaults
                // false) -- see banner gate #10.
                Task = 1200;
            }
            else if (BarcodeExposureDelay[iSht].Off())                         //拍照time out
            {
                if (bCCDBarcodeExposureOK[iBarCodeRowA] == false)
                {
                    CCD1_GridSetCell(iBarCodeRowA, iNowMoveStep + 1, "Exposure time out");
                    Log.sprintf("Action 1150, Exposure time out. Retry : %d", iRetryCountExpTimo);
                    CCD1_AddCCDCommunicationLog(iBarCodeRowA, Log);
                }
                else
                {
                    if (bVerify == true ||
                        (FLCarryKit.Item[0][iNowMoveStep] != HAS_NULL_IC &&
                         FLCarryKit.Item[0][iNowMoveStep] != NULL_IC))
                        CCD1_GridSetCell(iBarCodeRowA, iNowMoveStep + 1, "Waiting result..");
                }

                if (InArmSuck.iShtRow == 2)
                {
                    if (bCCDBarcodeExposureOK[iBarCodeRowB] == false)
                    {
                        CCD1_GridSetCell(iBarCodeRowB, iNowMoveStep + 1, "Exposure time out");
                        Log.sprintf("Action 1150, Exposure time out. Retry : %d", iRetryCountExpTimo);
                        CCD1_AddCCDCommunicationLog(iBarCodeRowB, Log);
                    }
                    else
                    {
                        if (bVerify == true ||
                            (FLCarryKit.Item[1][iNowMoveStep] != HAS_NULL_IC &&
                             FLCarryKit.Item[1][iNowMoveStep] != NULL_IC))
                            CCD1_GridSetCell(iBarCodeRowB, iNowMoveStep + 1, "Waiting result..");
                    }
                }
                else if (TestIF_File.bEnableMulti2D &&
                         (TestIF_File.iMulti2DType == e2x1In2CCD ||
                          TestIF_File.iMulti2DType == e2x2In2CCD))
                {
                    if (bCCDBarcodeExposureOK[iBarCodeRowB] == false)
                    {
                        Log.sprintf("Action 1150, Exposure time out. Retry : %d", iRetryCountExpTimo);
                        CCD1_AddCCDCommunicationLog(iBarCodeRowB, Log);
                    }
                }
                iRetryCountExpTimo++;
                Task = 1160;
            }
            break;
        case 1160:
            if (iRetryCountExpTimo < 3)
            {
                Task = 1170;
            }
            else
            {
                ret = ShowErrorMessage("WAR0464", K_RETRY | K_SKIP, MSht);      //2D function CCD 1 or 2 Exposure time out!
                if (ret == K_SKIP)
                    Task = 1200;
                else
                    Task = 1170;
                iRetryCountExpTimo = 0;
            }
            break;
        case 1170:
            if (bCCDBarcodeExposureOK[iBarCodeRowA] == false)
            {
                if (bVerify == false &&
                    (FLCarryKit.Item[0][iNowMoveStep] == HAS_NULL_IC ||
                     FLCarryKit.Item[0][iNowMoveStep] == NULL_IC))
                {
                    bNeesScanCode[0][iNowMoveStep] = false;
                    CCD1_SendSE0(iBarCodeRowA, false, iNowMoveStep, iNowMoveStepMulti2D, "exposure 1170: ");
                }
                else
                {
                    bNeesScanCode[0][iNowMoveStep] = true;
                    CCD1_SendSE0(iBarCodeRowA, true, iNowMoveStep, iNowMoveStepMulti2D, "exposure 1170: ");
                }
            }

            if (InArmSuck.iShtRow == 2)
            {
                if (bCCDBarcodeExposureOK[iBarCodeRowB] == false)
                {
                    if (bVerify == false &&
                        (FLCarryKit.Item[1][iNowMoveStep] == HAS_NULL_IC ||
                         FLCarryKit.Item[1][iNowMoveStep] == NULL_IC))
                    {
                        bNeesScanCode[1][iNowMoveStep] = false;
                        CCD1_SendSE0(iBarCodeRowB, false, iNowMoveStep, iNowMoveStepMulti2D, "exposure 1170: ");
                    }
                    else
                    {
                        bNeesScanCode[1][iNowMoveStep] = true;
                        CCD1_SendSE0(iBarCodeRowB, true, iNowMoveStep, iNowMoveStepMulti2D, "exposure 1170: ");
                    }
                }
            }
            else if (TestIF_File.bEnableMulti2D &&
                     (TestIF_File.iMulti2DType == e2x1In2CCD ||
                      TestIF_File.iMulti2DType == e2x2In2CCD))
            {
                if (bCCDBarcodeExposureOK[iBarCodeRowB] == false)
                {
                    if (bVerify == false &&
                        (FLCarryKit.Item[0][iNowMoveStep] == HAS_NULL_IC ||
                         FLCarryKit.Item[0][iNowMoveStep] == NULL_IC))
                    {
                        bNeesScanCode[0][iNowMoveStep] = false;
                        CCD1_SendSE0(iBarCodeRowB, false, iNowMoveStep, iNowMoveStepMulti2D, "exposure 1170: ");
                    }
                    else
                    {
                        bNeesScanCode[0][iNowMoveStep] = true;
                        CCD1_SendSE0(iBarCodeRowB, true, iNowMoveStep, iNowMoveStepMulti2D, "exposure 1170: ");
                    }
                }
            }
            Task = 1150;
            BarcodeExposureDelay[iSht].SetMSAndOn(TestIF_File.i2DTriggerTime);
            break;
        case 1180:
            Log.sprintf("Alarm, In shuttle %d exposure position error!", iSht + 1);
            CCD1_AddCCDCommunicationLog(iBarCodeRowA, Log);
            ShowErrorMessage("WAR0469", K_RETRY, MSht, false, ErrPart);         //In shuttle 1 exposure position error!
            Task = 1;
            break;
        case 1200:
            if (TestIF_File.bEnableMulti2D)
            {
                iNowMoveStepMulti2D--;
                iNowMoveStepMulti2DY--;
                if (iNowMoveStepMulti2DY < 0)
                {
                    iNowMoveStepMulti2DY = TestIF_File.iMulti2DYItem - 1;
                    iNowMoveStepMulti2DX--;
                    if (iNowMoveStepMulti2DX < 0)
                    {
                        iNowMoveStepMulti2DX = TestIF_File.iMulti2DXItem - 1;
                        iNowMoveStepMulti2D  = TestIF_File.iMulti2DCount - 1;
                        iNowMoveStep--;
                    }
                }
            }
            else
            {
                iNowMoveStep--;
            }
            iCurrentStep[iBarCodeRowA] = iNowMoveStep;
            iCurrentStep[iBarCodeRowB] = iNowMoveStep;

            if (iNowMoveStep >= 0 && iNowMoveStep < InArmSuck.iShtCol)
                Task = 1000;
            else
                Task = 1300;
            break;
        case 1300:
            if (dVisionVer >= 2)                                               //wei 20170119 (Steven) 版本號大於2
            {
                //==> Eastsun 20260527 整合#BCIR.P11 toggle GG/GV
                if (TestIF_File.bBarCodeInspReport)                            //Sam 20240426 : Add BarCoder Inspection Report
                    sCCDCommand = "GG,0";
                else
                    sCCDCommand = "GV,0";
                //<== Eastsun 20260527 整合#BCIR.P11
            }
            else
            {
                sCCDCommand = "GV0";                                           //Ifor 20151228 取得 CCD 讀取資料
            }

            bCCDBarcodeGetResultOK[iBarCodeRowA] = false;
            CCD1_SendCCDCommand(iBarCodeRowA, "get result 1300: ", sCCDCommand);

            // golden: fLotInfo->mtBarcodeInShLotInfo->SetCellNumber(0,1/2,"")
            // when TestIF_File.i2DIDFormat==eAMD -- cosmetic grid clear on
            // another un-translated VCL form (uLotInfo).  Gated: no-op.

            if (InArmSuck.iShtRow == 2)
            {
                bCCDBarcodeGetResultOK[iBarCodeRowB] = false;
                CCD1_SendCCDCommand(iBarCodeRowB, "get result 1300: ", sCCDCommand);
            }
            else if (TestIF_File.bEnableMulti2D &&
                     (TestIF_File.iMulti2DType == e2x1In2CCD ||
                      TestIF_File.iMulti2DType == e2x2In2CCD))
            {
                bCCDBarcodeGetResultOK[iBarCodeRowB] = false;
                CCD1_SendCCDCommand(iBarCodeRowB, "get result 1300: ", sCCDCommand);
            }
            else
            {
                bCCDBarcodeGetResultOK[iBarCodeRowB] = true;
            }

            // golden: cbBarCodeSimulate->Checked -> Socket2DReceiveList[]->Add()
            // simulate-injection.  Offline: g_cbBarCodeSimulateChecked defaults
            // false, so this branch is inert -- see banner gate #10.

            bHasError = false;
            Task = 1350;
            BarcodeDelay[iSht].SetMSAndOn(TestIF_File.iBarCodeDelay);          //讀取Time Out用的
            break;
        case 1350:
            if (bCCDBarcodeGetResultOK[iBarCodeRowA] == true &&
                bCCDBarcodeGetResultOK[iBarCodeRowB] == true)                  //讀取完成
            {
                CCD1_CCDBarcode_SortingForMulti2D(0);
                sErrorPart = "";
                for (int i = 0; i < InArmSuck.iShtCol; i++)                    //清空畫面
                {
                    if (bNeesScanCode[0][i] == true &&
                        (FLCarryKit.Item[0][i] != HAS_NULL_IC &&
                         FLCarryKit.Item[0][i] != NULL_IC) &&
                        (FLCarryKit.cDeviceInf[0][i].AnsiPos(asBarCodeErrorSend) != 0 ||
                         FLCarryKit.cDeviceInf[0][i] == ""))                   //wei 20160318 Barcode Error依客戶設定
                    {
                        bHasError = true;
                        if (IsNNMode() == NN_2Row)
                            sErrorPart += IndexSuckName[2][i];
                        else if (IsNNMode() == NN_1Row)
                            sErrorPart += IndexSuckName[1][i];
                        else
                            sErrorPart += IndexSuckName[0][i];

                        if (CUSTOMER_CODE == CC_ASE_CL)                        //KaiChen 20191121 中壢日月光 2D Check Sum
                            sErrorPart += "(Err)";
                    }
                    else if (bNeesScanCode[0][i] == true &&
                             (FLCarryKit.Item[0][i] != HAS_NULL_IC &&
                              FLCarryKit.Item[0][i] != NULL_IC) &&
                             (FLCarryKit.cDeviceInf[0][i].AnsiPos(asBarCodeErrorCheckSum) != 0 ||
                              FLCarryKit.cDeviceInf[0][i] == ""))
                    {
                        bHasError = true;
                        if (IsNNMode() == NN_2Row)
                            sErrorPart += IndexSuckName[2][i];
                        else if (IsNNMode() == NN_1Row)
                            sErrorPart += IndexSuckName[1][i];
                        else
                            sErrorPart += IndexSuckName[0][i];

                        if (CUSTOMER_CODE == CC_ASE_CL)
                            sErrorPart += "(Sum)";
                    }
                    else if (bNeesScanCode[0][i] == true &&
                             (FLCarryKit.Item[0][i] != HAS_NULL_IC &&
                              FLCarryKit.Item[0][i] != NULL_IC) &&
                             CCD1_RunCheckBarcodeByServerData())               //Jimmychiu 20230925 : read 2did in json file (gated -> always false)
                    {
                        bHasError = true;
                        if (IsNNMode() == NN_2Row)
                            sErrorPart += IndexSuckName[2][i];
                        else if (IsNNMode() == NN_1Row)
                            sErrorPart += IndexSuckName[1][i];
                        else
                            sErrorPart += IndexSuckName[0][i];
                    }
                    else
                    {
                        bNeesScanCode[0][i] = false;
                    }

                    if (bNeesScanCode[1][i] == true &&
                        (FLCarryKit.Item[1][i] != HAS_NULL_IC &&
                         FLCarryKit.Item[1][i] != NULL_IC) &&
                        (FLCarryKit.cDeviceInf[1][i].AnsiPos(asBarCodeErrorSend) != 0 ||
                         FLCarryKit.cDeviceInf[1][i] == ""))
                    {
                        bHasError = true;
                        if (IsNNMode() == NN_2Row)
                            sErrorPart += IndexSuckName[3][i];
                        else
                            sErrorPart += IndexSuckName[1][i];

                        if (CUSTOMER_CODE == CC_ASE_CL)
                            sErrorPart += "(Err)";
                    }
                    else if (bNeesScanCode[1][i] == true &&
                             (FLCarryKit.Item[1][i] != HAS_NULL_IC &&
                              FLCarryKit.Item[1][i] != NULL_IC) &&
                             (FLCarryKit.cDeviceInf[1][i].AnsiPos(asBarCodeErrorCheckSum) ||
                              FLCarryKit.cDeviceInf[1][i] == ""))
                    {
                        bHasError = true;
                        if (IsNNMode() == NN_2Row)
                            sErrorPart += IndexSuckName[3][i];
                        else
                            sErrorPart += IndexSuckName[1][i];

                        if (CUSTOMER_CODE == CC_ASE_CL)
                            sErrorPart += "(Sum)";
                    }
                    else if (bNeesScanCode[1][i] == true &&
                             (FLCarryKit.Item[1][i] != HAS_NULL_IC &&
                              FLCarryKit.Item[1][i] != NULL_IC) &&
                             CCD1_RunCheckBarcodeByServerData())
                    {
                        bHasError = true;
                        if (IsNNMode() == NN_2Row)
                            sErrorPart += IndexSuckName[3][i];
                        else
                            sErrorPart += IndexSuckName[1][i];
                    }
                    else
                    {
                        bNeesScanCode[1][i] = false;
                    }
                }

                Task = 2000;
            }
            else if (BarcodeDelay[iSht].Off())                                //讀取超時 (read timeout)
            {
                sErrorPart = "";
                if (bCCDBarcodeGetResultOK[iBarCodeRowA] == false)
                {
                    bCCDBarcodeGetResultOK[iBarCodeRowA] = true;
                    for (int i = 0; i < FLCarryKit.iMaxCol; i++)
                    {
                        if (bNeesScanCode[0][i] == true)
                            CCD1_GridSetCell(iBarCodeRowA, i + 1, "Decode time Out");
                    }
                    CCD1_AddCCDCommunicationLog(iBarCodeRowA, "Action 1350, Decode time out.");
                    bHasError = true;
                }

                if (InArmSuck.iShtRow == 2)
                {
                    if (bCCDBarcodeGetResultOK[iBarCodeRowB] == false)
                    {
                        bCCDBarcodeGetResultOK[iBarCodeRowB] = true;
                        for (int i = 0; i < FLCarryKit.iMaxCol; i++)
                        {
                            if (bNeesScanCode[1][i] == true)
                                CCD1_GridSetCell(iBarCodeRowB, i + 1, "Decode time Out");
                        }
                        CCD1_AddCCDCommunicationLog(iBarCodeRowB, "Action 1350, Decode time out.");
                        bHasError = true;
                    }
                }
                else if (TestIF_File.bEnableMulti2D &&
                         (TestIF_File.iMulti2DType == e2x1In2CCD ||
                          TestIF_File.iMulti2DType == e2x2In2CCD))
                {
                    if (bCCDBarcodeGetResultOK[iBarCodeRowB] == false)
                    {
                        bCCDBarcodeGetResultOK[iBarCodeRowB] = true;
                        for (int i = 0; i < FLCarryKit.iMaxCol; i++)
                        {
                            if (bNeesScanCode[0][i] == true)
                                CCD1_GridSetCell(iBarCodeRowB, i + 1, "Decode time Out");
                        }
                        CCD1_AddCCDCommunicationLog(iBarCodeRowB, "Action 1350, Decode time out.");
                        bHasError = true;
                    }
                }
                Task = 2000;
            }
            break;
        case 2000:
            if (bHasError == true)
            {
                if (iRetryCount < TestIF_File.iBarcodeRetryCount)              //Steven 20160421 : 修正判斷方式
                {
                    Task = 3000;
                }
                else
                {
                    if (TestIF_File.bNoCodeDeviceAutoSkip &&
                        fContact->IsRun2DCheck() == false)                     //JerryYang 20250220 : 2DID硬體順序檢查功能
                    {
                        CCD1_AddCCDCommunicationLog(iBarCodeRowA, ">>Cycle 2000, Decode end with auto skip.---------------\r\n");
                        if (TestIF_File.bCheckCodeByShuttle == false &&
                            TestIF_File.bCheckCodeByLot == false)
                        {
                            Task = 5000;
                        }
                        else
                        {
                            if (TestIF_File.bCheckLotHaveCode)                 //Steven 20170707 (wei) : Fixed 2DID for Korea
                                Task = 3500;
                            else
                                Task = 4000;
                        }

                        if (CUSTOMER_CODE == CC_ASE_KaohSiung)                 //KEVIN 20211005 ADD CONTINUE FAIL
                        {
                            for (int i = 0; i < InArmSuck.iShtCol; i++)        //JerryYang 20200616 累加fail alarm
                            {
                                if (bNeesScanCode[0][i] == true &&
                                    (FLCarryKit.Item[0][i] != HAS_NULL_IC &&
                                     FLCarryKit.Item[0][i] != NULL_IC) &&
                                    (FLCarryKit.cDeviceInf[0][i].AnsiPos(asBarCodeErrorSend) != 0 ||
                                     FLCarryKit.cDeviceInf[0][i] == ""))
                                {
                                    iBarcodeReject++;
                                }

                                if (bNeesScanCode[1][i] == true &&
                                    (FLCarryKit.Item[1][i] != HAS_NULL_IC &&
                                     FLCarryKit.Item[1][i] != NULL_IC) &&
                                    (FLCarryKit.cDeviceInf[1][i].AnsiPos(asBarCodeErrorSend) != 0 ||
                                     FLCarryKit.cDeviceInf[1][i] == ""))
                                {
                                    iBarcodeReject++;
                                }
                            }

                            if (TestIF_File.bEnableConsecutiveFailure &&
                                iBarcodeReject >= TestIF_File.iConsecutiveFailure)  //wei 20160823  Consecutive Failure
                            {
                                ShowErrorMessage("WAR0475", K_SKIP, MInShuttle1, false);
                                iBarcodeReject = 0;
                            }
                        }
                    }
                    else
                    {
                        Task = 2500;
                    }
                }
                bHasError = false;
            }
            else
            {
                CCD1_AddCCDCommunicationLog(iBarCodeRowA, ">>Cycle 2000, Decode end.---------------\r\n");
                if (TestIF_File.bCheckCodeByShuttle == false &&
                    TestIF_File.bCheckCodeByLot == false)
                {
                    Task = 5000;
                }
                else
                {
                    if (TestIF_File.bCheckLotHaveCode)
                        Task = 3500;
                    else
                        Task = 4000;
                }
            }
            break;
        case 2500:
            if (MOT[MSht].MotorMove(Prod.InSHT[iSht].iLeft + 1000))
            {
                Log.sprintf("Action 2500, Shuttle %d move to left", iSht + 1);
                CCD1_AddCCDCommunicationLog(iBarCodeRowA, Log);
                fLtcSensor->ClearLtcSensor(iSht);                              //Sam 20221101 : Latch 清除都要確認是否清清乾淨
                Task = 2600;
            }
            break;
        case 2600:
            Log.sprintf("Alarm 2600, In Shuttle %d Barcode Error", iSht + 1);
            CCD1_AddCCDCommunicationLog(iBarCodeRowA, Log);
            if (TestIF_File.bCheckCodeByLot &&                                 //Alick 20170124 add
                iRetryCount <= TestIF_File.iBarcodeRetryCount)                 //Alick 20170315 (wei) modify
            {
                bHasError = false;
                iRetryCount++;
                Task = 3000;                                                  //jou 20170509 (Steven) : 2D alway retry
            }
            else
            {
                if (fContact->IsRun2DCheck() == true)                         //JerryYang 20250220 : 2DID硬體順序檢查功能
                    ret = ShowErrorMessage("JAM0495", K_RETRY, MSht, false, sErrorPart);
                else
                    ret = ShowErrorMessage("JAM0460", K_RETRY | K_SKIP, MSht, false, sErrorPart);

                iRetryCount = 0;
                bHasError = false;
                if (ret == K_RETRY)
                {
                    Task = 3000;
                }
                else
                {
                    CCD1_AddCCDCommunicationLog(iBarCodeRowA, ">>Cycle 2600, Decode end with barcode error.---------------\r\n");

                    if (TestIF_File.bCheckCodeByShuttle == false &&
                        TestIF_File.bCheckCodeByLot == false)
                    {
                        Task = 5000;
                    }
                    else
                    {
                        if (TestIF_File.bCheckLotHaveCode)
                            Task = 3500;
                        else
                            Task = 4000;
                    }
                }
            }
            break;
        case 3000:                                                            //例外處理 (exception handling)
            if (TestIF_File.bEnableMulti2D)
            {
                iNowCheckStep = InArmSuck.iShtCol - 1;
                iNowMoveStepMulti2D  = TestIF_File.iMulti2DCount;
                iNowMoveStepMulti2DY = TestIF_File.iMulti2DYItem;              //遞減方式檢查, 故意+1
                iNowMoveStepMulti2DX = TestIF_File.iMulti2DXItem - 1;
            }
            else
            {
                iNowCheckStep = InArmSuck.iShtCol;                            //遞減方式檢查, 故意+1
                iNowMoveStepMulti2D  = 0;
                iNowMoveStepMulti2DX = 0;
                iNowMoveStepMulti2DY = 0;
            }

            iCurrentStep[iBarCodeRowA] = iNowCheckStep;
            iCurrentStep[iBarCodeRowB] = iNowCheckStep;

            for (int i = 0; i < FLCarryKit.iMaxCol; i++)                      //全部重新掃描 (rescan everything)
            {
                if (bVerify == true ||
                    (FLCarryKit.Item[0][i] != HAS_NULL_IC &&
                     FLCarryKit.Item[0][i] != NULL_IC))
                {
                    CCD1_GridSetCell(iBarCodeRowA, i + 1, "");
                    bNeesScanCode[0][i] = true;
                }
                else
                {
                    bNeesScanCode[0][i] = false;
                }

                if (bVerify == true ||
                    (FLCarryKit.Item[1][i] != HAS_NULL_IC &&
                     FLCarryKit.Item[1][i] != NULL_IC))
                {
                    CCD1_GridSetCell(iBarCodeRowB, i + 1, "");
                    bNeesScanCode[1][i] = true;
                }
                else
                {
                    bNeesScanCode[1][i] = false;
                }
            }
            Task = 3100;
            // fall through
        case 3100:
            if (TestIF_File.bEnableMulti2D)
            {
                iNowMoveStepMulti2D--;
                iNowMoveStepMulti2DY--;
                if (iNowMoveStepMulti2DY < 0)
                {
                    iNowMoveStepMulti2DY = TestIF_File.iMulti2DYItem - 1;
                    iNowMoveStepMulti2DX--;
                    if (iNowMoveStepMulti2DX < 0)
                    {
                        iNowMoveStepMulti2DX = TestIF_File.iMulti2DXItem - 1;
                        iNowMoveStepMulti2D  = TestIF_File.iMulti2DCount - 1;
                        iNowCheckStep--;
                    }
                }
            }
            else
            {
                iNowCheckStep--;
            }

            if (iNowCheckStep >= 0)                                           //KenHsieh 20250610 : 避免因Delay 造成exposure position error
            {
                iCurrentStep[iBarCodeRowA] = iNowCheckStep;
                iCurrentStep[iBarCodeRowB] = iNowCheckStep;
            }

            if (iNowCheckStep >= 0 && iNowCheckStep < InArmSuck.iShtCol)
            {
                Task = 3200;
            }
            else
            {
                iRetryCount++;
                Task = 1300;
            }
            break;
        case 3200:                                                            //移動馬達 (move motor)
            pos = CCD1_GetMovePos(iSht, iNowCheckStep, iNowMoveStepMulti2DX);
            pos += (int)Offset.iSHLeft2D[iSht];

            if (MOT[MSht].MotorMove(pos))
            {
                Log.sprintf("Action 3200, Shuttle %d move to step %d", iSht, iNowCheckStep);
                CCD1_AddCCDCommunicationLog(iBarCodeRowA, Log);
                BarcodePosDelay[iSht].SetMSAndOn(TestIF_File.iBarCodePosDelay); //KenHsieh 20250610 : 改為Start delay
                Task = 3300;
            }
            break;
        case 3300:
            if (BarcodePosDelay[iSht].Off())
            {
                bCCDBarcodeExposureOK[iBarCodeRowA] = false;
                if (InArmSuck.iShtRow == 2)
                    bCCDBarcodeExposureOK[iBarCodeRowB] = false;
                else if (TestIF_File.bEnableMulti2D &&
                         (TestIF_File.iMulti2DType == e2x1In2CCD ||
                          TestIF_File.iMulti2DType == e2x2In2CCD))
                    bCCDBarcodeExposureOK[iBarCodeRowB] = false;
                else
                    bCCDBarcodeExposureOK[iBarCodeRowB] = true;

                if (InArmSuck.iShtRow == 2)
                {
                    if (bNeesScanCode[0][iNowCheckStep] == false)
                        CCD1_SendSE0(iBarCodeRowA, false, iNowCheckStep, iNowMoveStepMulti2DX, "exposure 3300: ");
                    else
                    {
                        CCD1_SendSE0(iBarCodeRowA, true, iNowCheckStep, iNowMoveStepMulti2DX, "exposure 3300: ");
                        FLCarryKit.cDeviceInf[0][iNowCheckStep] = "";
                    }

                    if (bNeesScanCode[1][iNowCheckStep] == false)
                        CCD1_SendSE0(iBarCodeRowB, false, iNowCheckStep, iNowMoveStepMulti2DX, "exposure 3300: ");
                    else
                    {
                        CCD1_SendSE0(iBarCodeRowB, true, iNowCheckStep, iNowMoveStepMulti2DX, "exposure 3300: ");
                        FLCarryKit.cDeviceInf[1][iNowCheckStep] = "";
                    }
                }
                else if (TestIF_File.bEnableMulti2D &&
                         (TestIF_File.iMulti2DType == e2x1In2CCD ||
                          TestIF_File.iMulti2DType == e2x2In2CCD))
                {
                    if (bNeesScanCode[0][iNowCheckStep] == false)
                    {
                        CCD1_SendSE0(iBarCodeRowA, false, iNowCheckStep, iNowMoveStepMulti2DX, "exposure 3300: ");
                        CCD1_SendSE0(iBarCodeRowB, false, iNowCheckStep, iNowMoveStepMulti2DX, "exposure 3300: ");
                    }
                    else
                    {
                        if (TestIF_File.iMulti2DMap[0][iNowMoveStepMulti2D] > 0)
                            CCD1_SendSE0(iBarCodeRowA, true, iNowCheckStep, iNowMoveStepMulti2DX, "exposure 3300: ");
                        else
                            CCD1_SendSE0(iBarCodeRowA, false, iNowCheckStep, iNowMoveStepMulti2DX, "exposure 3300: ");

                        if (TestIF_File.iMulti2DMap[1][iNowMoveStepMulti2D] > 0)
                            CCD1_SendSE0(iBarCodeRowB, true, iNowCheckStep, iNowMoveStepMulti2DX, "exposure 3300: ");
                        else
                            CCD1_SendSE0(iBarCodeRowB, false, iNowCheckStep, iNowMoveStepMulti2DX, "exposure 3300: ");
                        FLCarryKit.cDeviceInf[0][iNowCheckStep] = "";
                    }
                }
                else
                {
                    if (bNeesScanCode[0][iNowCheckStep] == false)
                        CCD1_SendSE0(iBarCodeRowA, false, iNowCheckStep, iNowMoveStepMulti2DX, "exposure 3300: ");
                    else
                    {
                        CCD1_SendSE0(iBarCodeRowA, true, iNowCheckStep, iNowMoveStepMulti2DX, "exposure 3300: ");
                        FLCarryKit.cDeviceInf[0][iNowCheckStep] = "";
                    }
                }
                iRetryCountExpTimo = 0;
                BarcodeExposureDelay[iSht].SetMSAndOn(TestIF_File.i2DTriggerTime);
                Task = 3400;
            }
            break;
        case 3400:                                                            //拍照讀取2D Code
            if (bStepError[iBarCodeRowA] == true ||
                bStepError[iBarCodeRowB] == true)
            {
                ErrPart = "";
                if (IsNNMode() == NN_2Row)
                {
                    if (bStepError[iBarCodeRowA])
                        ErrPart += IndexSuckName[2][iNowCheckStep];            //Frank 20171011 (Steven) iNowMoveStep-->iNowCheckStep
                    if (bStepError[iBarCodeRowB])
                        ErrPart += IndexSuckName[3][iNowCheckStep];
                }
                else if (IsNNMode() == NN_1Row)
                {
                    if (bStepError[iBarCodeRowA] || bStepError[iBarCodeRowB])
                        ErrPart += IndexSuckName[1][iNowCheckStep];
                }
                else if (TestIF_File.bEnableMulti2D &&
                         (TestIF_File.iMulti2DType == e2x1In2CCD ||
                          TestIF_File.iMulti2DType == e2x2In2CCD))
                {
                    if (bStepError[iBarCodeRowA] || bStepError[iBarCodeRowB])
                        ErrPart += IndexSuckName[0][iNowCheckStep];
                }
                else
                {
                    if (bStepError[iBarCodeRowA])
                        ErrPart += IndexSuckName[0][iNowCheckStep];
                    if (bStepError[iBarCodeRowB])
                        ErrPart += IndexSuckName[1][iNowCheckStep];
                }

                Task = 1180;
            }
            else if (bCCDBarcodeExposureOK[iBarCodeRowA] == true &&
                     bCCDBarcodeExposureOK[iBarCodeRowB] == true)
            {
                if (bNeesScanCode[0][iNowCheckStep] == true)
                    CCD1_GridSetCell(iBarCodeRowA, iNowCheckStep + 1, "Waiting result..");

                if (InArmSuck.iShtRow == 2)
                {
                    if (bNeesScanCode[1][iNowCheckStep] == true)
                        CCD1_GridSetCell(iBarCodeRowB, iNowCheckStep + 1, "Waiting result..");
                }
                else if (TestIF_File.bEnableMulti2D &&
                         (TestIF_File.iMulti2DType == e2x1In2CCD ||
                          TestIF_File.iMulti2DType == e2x2In2CCD))
                {
                    if (bNeesScanCode[0][iNowCheckStep] == true)
                        CCD1_GridSetCell(iBarCodeRowB, iNowCheckStep + 1, "Waiting result..");
                }
                Task = 3100;
            }
            else if (BarcodeExposureDelay[iSht].Off())                        //拍照time out
            {
                if (bCCDBarcodeExposureOK[iBarCodeRowA] == false)
                {
                    CCD1_GridSetCell(iBarCodeRowA, iNowCheckStep + 1, "Exposure time out");
                    Log.sprintf("Action 3400, Exposure time out. Retry : %d", iRetryCountExpTimo);
                    CCD1_AddCCDCommunicationLog(iBarCodeRowA, Log);
                }
                else
                {
                    if (bNeesScanCode[0][iNowCheckStep] == true)
                        CCD1_GridSetCell(iBarCodeRowA, iNowCheckStep + 1, "Waiting result..");
                }

                if (InArmSuck.iShtRow == 2)
                {
                    if (bCCDBarcodeExposureOK[iBarCodeRowB] == false)
                    {
                        CCD1_GridSetCell(iBarCodeRowB, iNowCheckStep + 1, "Exposure time out");
                        Log.sprintf("Action 3400, Exposure time out. Retry : %d", iRetryCountExpTimo);
                        CCD1_AddCCDCommunicationLog(iBarCodeRowB, Log);
                    }
                    else
                    {
                        if (bNeesScanCode[1][iNowCheckStep] == true)
                            CCD1_GridSetCell(iBarCodeRowB, iNowCheckStep + 1, "Waiting result..");
                    }
                }
                else if (TestIF_File.bEnableMulti2D &&
                         (TestIF_File.iMulti2DType == e2x1In2CCD ||
                          TestIF_File.iMulti2DType == e2x2In2CCD))
                {
                    if (bCCDBarcodeExposureOK[iBarCodeRowB] == false)
                    {
                        Log.sprintf("Action 3400, Exposure time out. Retry : %d", iRetryCountExpTimo);
                        CCD1_AddCCDCommunicationLog(iBarCodeRowB, Log);
                    }
                    else
                    {
                        if (bNeesScanCode[0][iNowCheckStep] == true)
                            CCD1_GridSetCell(iBarCodeRowB, iNowCheckStep + 1, "Waiting result..");
                    }
                }

                iRetryCountExpTimo++;
                Task = 3460;
            }
            break;
        case 3460:
            if (iRetryCountExpTimo < 3)
            {
                Task = 3470;
            }
            else
            {
                ret = ShowErrorMessage("WAR0463", K_RETRY | K_SKIP, MSht);
                if (ret == K_SKIP)
                    Task = 3100;
                else
                    Task = 3470;
                iRetryCountExpTimo = 0;
            }
            break;
        case 3470:
            if (InArmSuck.iShtRow == 2)
            {
                if (bCCDBarcodeExposureOK[iBarCodeRowA] == false)
                {
                    bNeesScanCode[0][iNowCheckStep] = true;                    //KenHsieh 20260114
                    CCD1_SendSE0(iBarCodeRowA, true, iNowCheckStep, iNowMoveStepMulti2DX, "exposure 3470: ");
                }

                if (bCCDBarcodeExposureOK[iBarCodeRowB] == false)
                {
                    CCD1_SendSE0(iBarCodeRowB, true, iNowCheckStep, iNowMoveStepMulti2DX, "exposure 3470: ");
                    bNeesScanCode[1][iNowCheckStep] = true;
                }
            }
            else if (TestIF_File.bEnableMulti2D &&
                     (TestIF_File.iMulti2DType == e2x1In2CCD ||
                      TestIF_File.iMulti2DType == e2x2In2CCD))
            {
                if (bCCDBarcodeExposureOK[iBarCodeRowA] == false)
                {
                    bNeesScanCode[0][iNowCheckStep] = true;
                    if (TestIF_File.iMulti2DMap[0][iNowMoveStepMulti2D] > 0)
                        CCD1_SendSE0(iBarCodeRowA, true, iNowCheckStep, iNowMoveStepMulti2DX, "exposure 3470: ");
                    else
                        CCD1_SendSE0(iBarCodeRowA, false, iNowCheckStep, iNowMoveStepMulti2DX, "exposure 3470: ");
                }

                if (bCCDBarcodeExposureOK[iBarCodeRowB] == false)
                {
                    bNeesScanCode[0][iNowCheckStep] = true;
                    if (TestIF_File.iMulti2DMap[1][iNowMoveStepMulti2D] > 0)
                        CCD1_SendSE0(iBarCodeRowB, true, iNowCheckStep, iNowMoveStepMulti2DX, "exposure 3470: ");
                    else
                        CCD1_SendSE0(iBarCodeRowB, false, iNowCheckStep, iNowMoveStepMulti2DX, "exposure 3470: ");
                }
            }
            else
            {
                if (bCCDBarcodeExposureOK[iBarCodeRowA] == false)
                {
                    bNeesScanCode[0][iNowCheckStep] = true;
                    CCD1_SendSE0(iBarCodeRowA, true, iNowCheckStep, iNowMoveStepMulti2DX, "exposure 3470: ");
                }
            }

            Task = 3400;
            BarcodeExposureDelay[iSht].SetMSAndOn(TestIF_File.i2DTriggerTime);
            break;
        case 3500:
            // TODO(W5-BarCode-Shuttle1CCD-widget) golden BarCode_Sh1.cpp:1448-1611
            // (Alick 20170321 "Check Lot Have Code") -- the customer-Lot vs
            // decoded-code compare needs fLotInfo->edCustomerLotId->Text
            // (widget) and, on the non-CosFunction.b2DCodeCheckByCoustomerLot
            // path, fBarCode->list2DByLot (TStringList) + FILE* fopen on
            // asBarCodeDownLot -- none translated this wave (banner gate #4/
            // #5).  Conservative default: no lot-check error, matching the
            // "feature disabled" behaviour customers who leave
            // TestIF_File.bCheckLotHaveCode off already see.
            if (TestIF_File.bCheckLotHaveCode)                                //Alick 20170321 (wei) Check Lot Have Code
            {
                bLotCheckError = false;
                Task = 4000;
            }
            else
            {
                Task = 4000;
            }
            break;
        case 3600:
            if (MOT[MSht].MotorMove(Prod.InSHT[iSht].iLeft))                   //Steven 20160503 : 先移出來才Alarm
            {
                CCD1_NoteShowDupGrid();                                       //golden fNote->t2DCode grid -- gated no-op

                fLtcSensor->ClearLtcSensor(iSht);
                Log.sprintf("WAR0471, In shuttle %d check have Lot barcode error!\r\n", iSht + 1);
                CCD1_AddCCDCommunicationLog(iBarCodeRowA, Log);
                if (bLotCheckError == true)
                    ret = ShowErrorMessage("WAR0471", K_RETRY, MInShuttle1, false);   //Alick 20170202 remove skip

                if (ret == K_RETRY)
                    Task = 1;
            }
            break;
        //Alick 20170321 (wei) Check Lot Have Code
        case 4000:                                                            //Steven 20160428 : 檢查蝦頭的2D重複碼 (in-shuttle duplicate-code check)
            if (TestIF_File.bCheckCodeByShuttle ||
                TestIF_File.bCheckCodeByLot)                                   //Steven 20171128 (Wei)
            {
                map2DShuttleList[iBarCodeRowA].clear();
                map2DShuttleList[iBarCodeRowB].clear();
                memset(bHasErr, 0, sizeof(bHasErr));
                bShtHasDupError = false;
                ErrPart = "";
                for (int i = 0; i < InArmSuck.iShtCol; i++)
                {
                    if (FLCarryKit.Item[0][i] != HAS_NULL_IC &&
                        FLCarryKit.Item[0][i] != NULL_IC &&
                        FLCarryKit.cDeviceInf[0][i] != asBarCodeErrorSend &&
                        FLCarryKit.cDeviceInf[0][i] != "" &&
                        FLCarryKit.cDeviceInf[0][i] != asBarCodeErrorCheckSum)  //KaiHuang 20200826 : Add CheckSum
                    {
                        mapShtIter[iBarCodeRowA] = map2DShuttleList[iBarCodeRowA].find(FLCarryKit.cDeviceInf[0][i]);
                        if (mapShtIter[iBarCodeRowA] == map2DShuttleList[iBarCodeRowA].end())
                        {
                            map2DShuttleList[iBarCodeRowA][FLCarryKit.cDeviceInf[0][i]] = i;
                        }
                        else                                                   //有重複的就不用加進去了
                        {
                            iBarcodeDuplicate[iBarCodeRowA]++;
                            bShtHasDupError = true;
                            if (IsNNMode() == NN_2Row)
                                ErrPart += IndexSuckName[2][i];
                            else if (IsNNMode() == NN_1Row)
                                ErrPart += IndexSuckName[1][i];
                            else
                                ErrPart += IndexSuckName[0][i];
                            bHasErr[0][i] = true;

                            ret = atoi(map2DShuttleList[iBarCodeRowA][FLCarryKit.cDeviceInf[0][i]].c_str());  //Steven 20160503 : 重複的Code要設定為Error
                            if (IsNNMode() == NN_2Row)
                                ErrPart += IndexSuckName[2][ret];
                            else if (IsNNMode() == NN_1Row)
                                ErrPart += IndexSuckName[1][ret];
                            else
                                ErrPart += IndexSuckName[0][ret];
                            bHasErr[0][ret] = true;
                        }
                    }

                    if (InArmSuck.iShtRow == 2)
                    {
                        if (FLCarryKit.Item[1][i] != HAS_NULL_IC &&
                            FLCarryKit.Item[1][i] != NULL_IC &&
                            FLCarryKit.cDeviceInf[1][i] != asBarCodeErrorSend &&
                            FLCarryKit.cDeviceInf[1][i] != "" &&
                            FLCarryKit.cDeviceInf[1][i] != asBarCodeErrorCheckSum)
                        {
                            mapShtIter[iBarCodeRowB] = map2DShuttleList[iBarCodeRowB].find(FLCarryKit.cDeviceInf[1][i]);
                            if (mapShtIter[iBarCodeRowB] == map2DShuttleList[iBarCodeRowB].end())
                            {
                                map2DShuttleList[iBarCodeRowB][FLCarryKit.cDeviceInf[1][i]] = i;
                            }
                            else
                            {
                                iBarcodeDuplicate[iBarCodeRowB]++;
                                bShtHasDupError = true;
                                if (IsNNMode() == NN_2Row)
                                    ErrPart += IndexSuckName[3][i];
                                else
                                    ErrPart += IndexSuckName[1][i];
                                bHasErr[1][i] = true;

                                ret = atoi(map2DShuttleList[iBarCodeRowB][FLCarryKit.cDeviceInf[1][i]].c_str());
                                if (IsNNMode() == NN_2Row)
                                    ErrPart += IndexSuckName[3][ret];
                                else
                                    ErrPart += IndexSuckName[1][ret];
                                bHasErr[1][ret] = true;
                            }
                        }
                    }
                }

                if (bShtHasDupError)
                    Task = 4050;
                else
                    Task = 4100;
            }
            else
            {
                Task = 4100;
            }
            break;
        case 4050:
            if (MOT[MSht].MotorMove(Prod.InSHT[iSht].iLeft))                   //Steven 20160503 : 先移出來才Alarm
            {
                CCD1_NoteShowDupGrid();                                       //golden fNote->t2DCode grid -- gated no-op

                fLtcSensor->ClearLtcSensor(iSht);
                Log.sprintf("Alarm 4050, In shuttle %d check have duplicate barcode error!\r\n", iSht + 1);
                CCD1_AddCCDCommunicationLog(iBarCodeRowA, Log);
                if (TestIF_File.iShtDuplicateRetryCnt <= 0 ||                  //Steven 20190520 : 修正等於0時會變成無窮迴圈
                    TestIF_File.iShtDuplicateRetryCnt >= iShtDupilcateFailCnt[iSht])  //Steven 20160823 : 蝦頭重複碼要可以自動Retry
                {
                    iShtDupilcateFailCnt[iSht] = 0;
                    ret = ShowErrorMessage("WAR0465", K_RETRY | K_SKIP, MSht, false, ErrPart);
                }
                else
                {
                    iShtDupilcateFailCnt[iSht]++;
                    ret = K_RETRY;
                }

                if (ret == K_SKIP)
                {
                    for (int i = 0; i < InArmSuck.iShtRow; i++)
                    {
                        for (int j = 0; j < InArmSuck.iShtCol; j++)
                        {
                            if (SPIL_FOR_QLE == 1 &&
                                Prod.bF18InshuttleDetect == true)              //JerryYang 20241009
                            {
                                if (InArmSuck.iShtRow == 2)
                                {
                                    if (bHasErr[0][j] == true || bHasErr[1][j] == true)
                                    {
                                        FLCarryKit.SetItemData(i, j, HAS_NULL_IC);
                                        CCD1_ADD_TEST_RESULT_RECORD2(FLCarryKit, i, j, iTestBinCount, "2DID_Duplicate");
                                        FLCarryKit.cDeviceInf[i][j] = asBarCodeErrorSend;
                                    }
                                }
                                else
                                {
                                    if (bHasErr[i][j] == true)
                                    {
                                        FLCarryKit.SetItemData(i, j, HAS_NULL_IC);
                                        CCD1_ADD_TEST_RESULT_RECORD2(FLCarryKit, i, j, iTestBinCount, "2DID_Duplicate");
                                        FLCarryKit.cDeviceInf[i][j] = asBarCodeErrorSend;
                                    }
                                }
                            }
                            else
                            {
                                if (bHasErr[i][j] == true)
                                {
                                    FLCarryKit.cDeviceInf[i][j] = asBarCodeErrorSend;
                                    CCD1_ADD_ERROR_RECORD_NOSAVE(FLCarryKit, i, j, "Duplicate 2DID error");  //JerryYang 20230322
                                }
                            }
                        }
                    }

                    if (SPIL_FOR_QLE == 1 && Prod.bF18InshuttleDetect == true)
                    {
                        if (InArmSuck.iShtRow == 2)
                            ShowMyMessage("請確認已將Shuttle1 Duplicate的Device同一行的都取出!!");
                        else
                            ShowMyMessage("請確認已將Shuttle1 Duplicate的device取出!!");
                    }

                    Task = 4100;
                }
                else
                {
                    Task = 1;
                }
            }
            break;
        case 4100:
            iShtDupilcateFailCnt[iSht] = 0;
            if (/*fContact->fShow*/ false &&                                   // gated: fContact->fShow (golden cContact.h)
                                                                                // not exposed on TfContactShim; ASE_CL
                                                                                // debug-display bypass defaults off.
                CUSTOMER_CODE == CC_ASE_CL &&
                (iContactMode == kContactAutoGetHeight || iContactMode == kContactTest))
            {
                Task = 4200;
            }
            else if (TestIF_File.bCheckCodeByLot &&
                     fContact->IsRun2DCheck() == false)                       //JerryYang 20250220
            {
                bLotHasDupError = false;
                memset(bHasErr, 0, sizeof(bHasErr));
                ErrPart = "";
                for (int i = 0; i < InArmSuck.iShtCol; i++)
                {
                    if (FLCarryKit.Item[0][i] != HAS_NULL_IC &&
                        FLCarryKit.Item[0][i] != NULL_IC &&
                        FLCarryKit.cDeviceInf[0][i] != asBarCodeErrorSend &&
                        FLCarryKit.cDeviceInf[0][i] != "" &&
                        FLCarryKit.cDeviceInf[0][i] != asBarCodeErrorCheckSum)
                    {
                        map2DIter = map2DList.find(FLCarryKit.cDeviceInf[0][i]);
                        if (map2DList.size() != 0 && map2DIter != map2DList.end())
                        {
                            iBarcodeDuplicate[iBarCodeRowA]++;
                            bLotHasDupError = true;
                            if (IsNNMode() == NN_2Row)
                                ErrPart += IndexSuckName[2][i];
                            else if (IsNNMode() == NN_1Row)
                                ErrPart += IndexSuckName[1][i];
                            else
                                ErrPart += IndexSuckName[0][i];
                            bHasErr[0][i] = true;
                        }
                    }

                    if (InArmSuck.iShtRow == 2)
                    {
                        if (FLCarryKit.Item[1][i] != HAS_NULL_IC &&
                            FLCarryKit.Item[1][i] != NULL_IC &&
                            FLCarryKit.cDeviceInf[1][i] != asBarCodeErrorSend &&
                            FLCarryKit.cDeviceInf[1][i] != "" &&
                            FLCarryKit.cDeviceInf[1][i] != asBarCodeErrorCheckSum)
                        {
                            map2DIter = map2DList.find(FLCarryKit.cDeviceInf[1][i]);
                            if (map2DList.size() != 0 && map2DIter != map2DList.end())
                            {
                                iBarcodeDuplicate[iBarCodeRowB]++;
                                bLotHasDupError = true;
                                if (IsNNMode() == NN_2Row)
                                    ErrPart += IndexSuckName[3][i];
                                else
                                    ErrPart += IndexSuckName[1][i];
                                bHasErr[1][i] = true;
                            }
                        }
                    }
                }

                if (bLotHasDupError)
                    Task = 4150;
                else
                    Task = 4200;
            }
            else
            {
                Task = 4200;
            }
            break;
        case 4150:
            if (MOT[MSht].MotorMove(Prod.InSHT[iSht].iLeft))
            {
                CCD1_NoteShowDupGrid();                                       //golden fNote->t2DCode grid -- gated no-op

                fLtcSensor->ClearLtcSensor(iSht);
                Log.sprintf("Alarm 4150, In shuttle %d check have duplicate barcode in lot error!\r\n", iSht + 1);
                CCD1_AddCCDCommunicationLog(iBarCodeRowA, Log);
                ret = ShowErrorMessage("WAR0467", K_RETRY | K_SKIP, MSht, false, ErrPart);
                if (ret == K_SKIP)
                {
                    for (int i = 0; i < InArmSuck.iShtRow; i++)
                    {
                        for (int j = 0; j < InArmSuck.iShtCol; j++)
                        {
                            if (SPIL_FOR_QLE == 1 &&
                                Prod.bF18InshuttleDetect == true)
                            {
                                if (InArmSuck.iShtRow == 2)
                                {
                                    if (bHasErr[0][j] == true || bHasErr[1][j] == true)
                                    {
                                        FLCarryKit.SetItemData(i, j, HAS_NULL_IC);
                                        CCD1_ADD_TEST_RESULT_RECORD2(FLCarryKit, i, j, iTestBinCount, "2DID_Duplicate");
                                    }
                                }
                                else
                                {
                                    if (bHasErr[i][j] == true)
                                    {
                                        FLCarryKit.SetItemData(i, j, HAS_NULL_IC);
                                        CCD1_ADD_TEST_RESULT_RECORD2(FLCarryKit, i, j, iTestBinCount, "2DID_Duplicate");
                                    }
                                }
                            }
                            else
                            {
                                if (bHasErr[i][j] == true)
                                    FLCarryKit.cDeviceInf[i][j] = asBarCodeErrorSend;
                            }
                        }
                    }

                    if (SPIL_FOR_QLE == 1 && Prod.bF18InshuttleDetect == true)
                    {
                        if (InArmSuck.iShtRow == 2)
                            ShowMyMessage("請確認已將Shuttle1 2DID重複的Device同一行的都取出!!");
                        else
                            ShowMyMessage("請確認已將Shuttle1 2DID重複的Device取出!!");
                    }
                    Task = 4200;
                }
                else
                {
                    Task = 1;
                }
            }
            break;
        case 4200:
            if (/*fContact->fShow*/ false &&                                  // gated (same as case 4100)
                CUSTOMER_CODE == CC_ASE_CL &&
                (iContactMode == kContactAutoGetHeight || iContactMode == kContactTest))
            {
                // golden: empty branch (deliberately skips the duplicate-file
                // write below for this ASE_CL contact-height-test scenario).
            }
            else if (TestIF_File.bCheckCodeByLot &&                           //jou 20191008
                     CosFunction.bBarcodeDuplicateFileByOutArm == false &&
                     fContact->IsRun2DCheck() == false)
            {
                // golden also does list2DByLot->Clear()/Add() + WriteDataToFile
                // (disk mirror of the in-memory map, banner gate #5/#7) -- the
                // in-memory map2DList population itself has no such
                // dependency and is kept real:
                for (int i = 0; i < InArmSuck.iShtCol; i++)
                {
                    if (FLCarryKit.Item[0][i] != HAS_NULL_IC &&
                        FLCarryKit.Item[0][i] != NULL_IC &&
                        FLCarryKit.cDeviceInf[0][i] != asBarCodeErrorSend &&
                        FLCarryKit.cDeviceInf[0][i] != "" &&
                        FLCarryKit.cDeviceInf[0][i] != asBarCodeErrorCheckSum)
                    {
                        map2DList[FLCarryKit.cDeviceInf[0][i]] = i;
                    }

                    if (InArmSuck.iShtRow == 2)
                    {
                        if (FLCarryKit.Item[1][i] != HAS_NULL_IC &&
                            FLCarryKit.Item[1][i] != NULL_IC &&
                            FLCarryKit.cDeviceInf[1][i] != asBarCodeErrorSend &&
                            FLCarryKit.cDeviceInf[1][i] != "" &&
                            FLCarryKit.cDeviceInf[1][i] != asBarCodeErrorCheckSum)
                        {
                            map2DList[FLCarryKit.cDeviceInf[1][i]] = i;
                        }
                    }
                }
            }

            // TODO(W5-BarCode-Shuttle1CCD-widget/rest) golden BarCode_Sh1.cpp:2053-2160
            // (JerryYang 20241104 "2DID白名單功能") -- whitelist compare needs
            // fBarCode->bSHT_2DIDIsInsideList / list2DWhitleTemp /
            // list2DWhitleResult (TfBarCode's own members, not among this
            // wave's 17 shim methods) + WriteDataToFile (already gated) +
            // fLotInfo->cbRunMode->Text (widget).  Gated: bAlarm stays false
            // (no whitelist violation raised) -- banner gate #5.

            if (TestIF_File.bEnableBarCode == true &&
                TestIF_File.b2DIDAllowList == true &&
                g_LotInfoRunModeIsCorr == false &&
                bAlarm == true)
            {
                Task = 4300;
            }
            else
            {
                Task = 5000;
            }
            break;
        case 4300:
            if (MOT[MSht].MotorMove(Prod.InSHT[iSht].iLeft))
            {
                bAlarm = false;
                CCD1_NoteShowDupGrid();                                       //golden fNote->t2DCode grid -- gated no-op

                fLtcSensor->ClearLtcSensor(iSht);
                Log.sprintf("Alarm 4300, In shuttle %d check 2DID is not in white list!\r\n", iSht + 1);
                CCD1_AddCCDCommunicationLog(iBarCodeRowA, Log);
                ret = ShowErrorMessage("WAR0485", K_SKIP, MSht, false, ErrPart);

                if (ret == K_SKIP)
                {
                    for (int i = 0; i < InArmSuck.iShtRow; i++)
                    {
                        for (int j = 0; j < InArmSuck.iShtCol; j++)
                        {
                            if (SPIL_FOR_QLE == 1 &&
                                Prod.bF18InshuttleDetect == true)
                            {
                                if (InArmSuck.iShtRow == 2 && Prod.bF18InshuttleDetect)
                                {
                                    if (bHasErr[0][j] == true || bHasErr[1][j] == true)
                                    {
                                        FLCarryKit.SetItemData(i, j, HAS_NULL_IC);
                                        CCD1_ADD_TEST_RESULT_RECORD2(FLCarryKit, i, j, iTestBinCount, "2DIDNotInAllowList");
                                    }
                                }
                                else
                                {
                                    if (bHasErr[i][j] == true)
                                    {
                                        FLCarryKit.SetItemData(i, j, HAS_NULL_IC);
                                        CCD1_ADD_TEST_RESULT_RECORD2(FLCarryKit, i, j, iTestBinCount, "2DIDNotInAllowList");
                                    }
                                }
                            }
                            else
                            {
                                if (bHasErr[i][j] == true)
                                {
                                    FLCarryKit.cDeviceInf[i][j] = asBarCodeErrorSend;
                                    CCD1_ADD_ERROR_RECORD_NOSAVE(FLCarryKit, i, j, "2DIDNotInAllowList 2DID error");
                                }
                            }
                        }
                    }

                    if (SPIL_FOR_QLE == 1 && Prod.bF18InshuttleDetect == true)
                    {
                        if (InArmSuck.iShtRow == 2 && Prod.bF18InshuttleDetect)
                            ShowMyMessage("請確認已Shuttle1將不在白名單的Device同一行的都取出!!");
                        else
                            ShowMyMessage("請確認已Shuttle1將不在白名單的Device取出!!");
                    }

                    Task = 5000;
                }
            }
            break;
        case 5000:
            if (CCD1_DoCheckSHT2DIDStatus(0) == true)                        //Steven 20200909 -- gated, always false
            {
                if (CosFunction.bUse2DIDAllSiteFailSetToErrBin == true ||
                    TestIF_File.iEnableAllSite2DIDErr == 2)
                {
                    for (int i = 0; i < InArmSuck.iShtCol; i++)
                    {
                        if (FLCarryKit.Item[0][i] != HAS_NULL_IC &&
                            FLCarryKit.Item[0][i] != NULL_IC)
                        {
                            if (FLCarryKit.cDeviceInf[0][i] == asBarCodeErrorSend ||
                                FLCarryKit.cDeviceInf[0][i] == "" ||
                                FLCarryKit.cDeviceInf[0][i] == asBarCodeErrorCheckSum)
                            {
                                FLCarryKit.SetItemData(0, i, TEST_PASS + iTestBinCount);
                                FLCarryKit.iBinData[0][i] = iTestBinCount;
                                CCD1_ADD_TEST_RESULT_RECORD(FLCarryKit, 0, i, iTestBinCount, FLCarryKit.cSBin[0][i], "NonTestToRBin");
                            }
                        }

                        if (InArmSuck.iShtRow == 2)
                        {
                            if (FLCarryKit.Item[1][i] != HAS_NULL_IC &&
                                FLCarryKit.Item[1][i] != NULL_IC)
                            {
                                if (FLCarryKit.cDeviceInf[1][i] == asBarCodeErrorSend ||
                                    FLCarryKit.cDeviceInf[1][i] == "" ||           //Steven 20160512 : 2D && --> ||
                                    FLCarryKit.cDeviceInf[1][i] == asBarCodeErrorCheckSum)
                                {
                                    FLCarryKit.SetItemData(1, i, TEST_PASS + iTestBinCount);
                                    FLCarryKit.iBinData[1][i] = iTestBinCount;
                                    CCD1_ADD_TEST_RESULT_RECORD(FLCarryKit, 1, i, iTestBinCount, FLCarryKit.cSBin[1][i], "NonTestToRBin");
                                }
                            }
                        }
                    }
                }
                else
                {
                    Task = 6000;                                              //例外，要重掃 (exception, rescan)
                    break;
                }
            }
            Task = 5050;
            // fall through
        case 5050:
            for (int i = 0; i < InArmSuck.iShtCol; i++)
            {
                if (FLCarryKit.Item[0][i] != HAS_NULL_IC &&
                    FLCarryKit.Item[0][i] != NULL_IC)
                {
                    iNeedBarcodeCount[iBarCodeRowA]++;
                    if (FLCarryKit.cDeviceInf[0][i] == asBarCodeErrorSend ||
                        FLCarryKit.cDeviceInf[0][i] == "" ||
                        FLCarryKit.cDeviceInf[0][i] == asBarCodeErrorCheckSum)
                    {
                        iBarcodeErrorCount[iBarCodeRowA]++;
                    }
                    else
                    {
                        iBarcodePassCount[iBarCodeRowA]++;
                    }
                    CCD1_ADD_2DID_RECORD(FLCarryKit, 0, i, FLCarryKit.cDeviceInf[0][i]);
                }

                if (InArmSuck.iShtRow == 2)
                {
                    if (FLCarryKit.Item[1][i] != HAS_NULL_IC &&                //RogerYang 20181219
                        FLCarryKit.Item[1][i] != NULL_IC)
                    {
                        iNeedBarcodeCount[iBarCodeRowB]++;
                        if (FLCarryKit.cDeviceInf[1][i] == asBarCodeErrorSend ||
                            FLCarryKit.cDeviceInf[1][i] == "" ||
                            FLCarryKit.cDeviceInf[1][i] == asBarCodeErrorCheckSum)
                        {
                            iBarcodeErrorCount[iBarCodeRowB]++;
                        }
                        else
                        {
                            iBarcodePassCount[iBarCodeRowB]++;
                        }
                    }
                    CCD1_ADD_2DID_RECORD(FLCarryKit, 1, i, FLCarryKit.cDeviceInf[1][i]);
                }
            }

            for (int i = 0; i < InArmSuck.iShtCol; i++)                       //JerryYang 20200616 累加fail alarm
            {
                if (bNeesScanCode[0][i] == true &&
                    (FLCarryKit.Item[0][i] != HAS_NULL_IC &&
                     FLCarryKit.Item[0][i] != NULL_IC) &&
                    (FLCarryKit.cDeviceInf[0][i] == asBarCodeErrorSend ||
                     FLCarryKit.cDeviceInf[0][i] == ""))
                {
                    iBarcodeReject++;
                }

                if (bNeesScanCode[1][i] == true &&
                    (FLCarryKit.Item[1][i] != HAS_NULL_IC &&
                     FLCarryKit.Item[1][i] != NULL_IC) &&
                    (FLCarryKit.cDeviceInf[1][i] == asBarCodeErrorSend ||
                     FLCarryKit.cDeviceInf[1][i] == ""))
                {
                    iBarcodeReject++;
                }
            }

            if (TestIF_File.bEnableConsecutiveFailure &&
                iBarcodeReject >= TestIF_File.iConsecutiveFailure)
            {
                ShowErrorMessage("WAR0475", K_SKIP, MInShuttle1, false);
                iBarcodeReject = 0;
            }

            if (CCD1_DoBarcodeCount())                                       //Steven 20171222 (Wei) : Yield Alarm -- gated, always false
            {
                Task = 5200;
                break;
            }
            else
            {
                Task = 5100;
            }
            // fall through
        case 5100:
            #ifndef SOFT_SIMULTE
            ret = SendHTTPRequest(0);                                       //Steven 20200909 : 修改Murata XML上傳資料
            #else
            ret = 1;
            #endif
            if (ret == 1)
            {
                Task = 1;
                i2DIDRetryCTSH1 = 0;
                iHttpErrRetryCT = 0;
                return true;
            }
            else
            {
                BarcodePosDelay[iSht].SetSecAndOn(30);
                iHttpErrRetryCT++;
                Str.sprintf("Shuttle 1 SendHTTPRequest fail retry count: %d", iHttpErrRetryCT);
                RecordProcess(Str);
                if (iHttpErrRetryCT > 3)
                    Task = 5160;
                else
                    Task = 5150;
            }
            break;
        case 5150:
            if (BarcodePosDelay[iSht].Off())
                Task = 5100;
            break;
        case 5160:
            ret = ShowErrorMessage("WAR16320", K_RETRY | K_RESET, MMSystem, false, "DoBarcodeCCDInShuttle_1");
            iHttpErrRetryCT = 0;
            if (ret == K_RETRY)
            {
                Task = 5100;
            }
            else
            {
                fMain->Reset("DoBarcodeCCDInShuttle_1");
                i2DIDRetryCTSH1 = 0;
                Task = 1;
                return true;
            }
            break;
        case 5200:
            ShowErrorMessage("WAR0483", 0, MMSystem, false, /*s2DIDYield*/ AnsiString(""));  // golden s2DIDYield string (Yield-alarm gate, always empty here)
            Task = 5100;
            break;
        case 6000:                                                            //Isaac 20200206 : 2DID All site fail recheck twice and alarm
            if (IniConfig.bF21InOutArmZMotorPrivate)
            {
                if (DoInOutARM_SHT_MoveSafe(0))                              //kevin 20161005 SHUTTLE 1 移動安全保護
                    return false;
            }

            MOT[MInShuttle1].fCanMoveR = true;
            if (MOT[MInShuttle1].MotorMove(Prod.InSHT[0].iLeft))
            {
                i2DIDRetryCTSH1++;
                if (i2DIDRetryCTSH1 > 1)
                {
                    i2DIDRetryCTSH1 = 0;
                    ret = CCD1_ShowMyMessageBox_YES_SKIP("All sites 2DID Error On Shuttle 1!! Please check!", "Retry Again?");
                    if (ret == 1)                                            //Yes
                        Task = 100;
                    else
                        Task = 5050;
                }
            }
            break;
    }
    return false;
}
