// Automation/SCK_ART_Remainder.h
// Standard C++ translation of BCB6 Automation/SCK_ART.h/.cpp (V3.33.906.0_20260618) -- REMAINDER
// wave, continuing the sibling W5-SckArt-Translate 8-function extract (Automation/SCK_ART.h/.cpp,
// same directory) with the NEXT contiguous slice of golden TfSCKART methods.
//
// Golden source: HT9011UC_Code_V3.33.906.0_20260618/Automation/SCK_ART.cpp (4392 lines, cp950) and
//                HT9011UC_Code_V3.33.906.0_20260618/Automation/SCK_ART.h  (370 lines).
//
// AI(W5-Final-SckArtRemainder) 20260711: new file. This wave translates golden lines 183-1646
// (SetSetupFilePath through the end of SaveTestSummary's dispatcher body) MINUS the pure-VCL
// event-handler / lifecycle functions in that range (see "SKIPPED -- PURE VCL" list below), which
// carry no calc-core to extract. Golden lines 1647-4358 (the four giant SaveTestSummary* report-writer
// bodies + DoARTLotStart + the remaining click handlers) are OUT OF SCOPE this wave -- see
// "GOLDEN LINE BOUNDARY" below for the precise hand-off split, matching the project's established
// BarCode_Shuttle2_Scan clean-partial-handoff precedent (see docs/DEVLOG.md 2026-07-11 BarCode entry).
//
// ---------------------------------------------------------------------------------------------
// GOLDEN LINE BOUNDARY (what THIS file covers vs. what remains)
// ---------------------------------------------------------------------------------------------
//   DONE  (this file):
//     TfSCKART::SetSetupFilePath()              -> SckArtRem_SetSetupFilePath   (golden :183-188)
//     TfSCKART::AccessFile(bool,int)            -> SckArtRem_AccessFile        (golden :190-444)
//     TfSCKART::SetGPIBVersion(AnsiString)      -> SckArtRem_SetGPIBVersion    (golden :446-475)
//     TfSCKART::UpdateCount()                   -> SckArtRem_UpdateCount      (golden :477-591)
//     TfSCKART::AddAlarmCode(AnsiString,AnsiString,int) -> SckArtRem_AddAlarmCode (golden :593-624)
//     TfSCKART::ClearLotInfo()                  -> SckArtRem_ClearLotInfo    (golden :837-923)
//     TfSCKART::AddOutputJamCnt(int,int,int,int)-> SckArtRem_AddOutputJamCnt (golden :1361-1389)
//     TfSCKART::SaveTestSummary(int)            -> SckArtRem_SaveTestSummary (golden :1619-1645,
//                                                    dispatcher body only -- see gate #7 below)
//
//   SKIPPED -- PURE VCL (no calc-core; widget lifecycle/event glue only, verified by direct golden
//   read, not silently dropped):
//     FormShow/FormClose/FormDestroy (:122-181), SettingPanelOnOff (:674-721),
//     btnApplyCountClick (:723-798 -- DOES contain real LastSet count-correction arithmetic, but
//       every input is an operator-typed VCL Panel->Caption value with no non-UI representation;
//       flagged as a JUDGMENT CALL below, not translated), palInputJamCntClick (:800-806),
//     btnExitClick (:808-820), edlRTTryCntClick/edARTPassRateClick/edLotCountClick/
//       edARTAutoSkipClick (:822-835, :925-928 -- all just ShowQwertyKey(Sender,...) VCL glue),
//     btnDeleteLotInfoClick (:1391-1401 -- thin wrapper: 4 LastSet/member resets + ClearLotInfo() +
//       fCounterClear->ClearCount(), the last needing an untranslated cCounterClear.cpp form),
//     TimerSCKARTFlowTimer (:1403-1519 -- 100% TALed widget color/blink refresh, zero state written),
//     btnApplyQtyClick/btnApplySettingClick/btnApplyLotInfoClick (:1521-1606 -- each reads ~15 VCL
//       Edit/CheckBox/ComboBox values the operator just typed, then calls AccessFile(false,-1); the
//       "read VCL, assign into TestIF_File" glue itself has no non-UI calc-core to extract),
//     edtVersionMouseUp/edSPBinYieldClick (:1608-1617).
//
//   REMAINING (next wave, golden :1647-4358; DoChkInputCntAlarm :4359-4391 already done by the
//   sibling SCK_ART.h/.cpp): SaveTestSummarySECS(:1647-2044)/SaveMultiLotTestSummary(:2045-2804)/
//   SaveTestSummaryTSV(:2805-3128)/SaveSummaryTrayFeed(:3129-3401)/Save2DSortingSummary(:3402-4062)
//   (five large report-file-writer bodies, ~2500 golden lines combined -- gated no-op stand-ins in
//   THIS file at their SaveTestSummary dispatch call sites, see gate #7), srvrscktTSVClientRead/
//   TimerTSVTimer(:4063-4102, VCL socket/timer events), FTP_Upload(:4103-4180, TNMFTP VCL component),
//   edtMRejectCntClick/edtAlmAutoCloseSiteClick(:4181-4190, VCL), DoARTLotStart(:4191-4256, needs the
//   3 LastSet GPIB-lot-state fields NOT touched by this wave -- bWaitStartLotAutoRetestGPIB/
//   bEndLotAutoRetestGPIB/bFirstTestAutoRetestGPIB, see gate #4 note), ledShowFTCTDataClick/
//   palInputCountMouseDown/btnFTCTResetClick/edtLdCntLimNClick(:4257-4358, VCL).
//
// ---------------------------------------------------------------------------------------------
// WHY A SEPARATE SckArtRemainderState, NOT THE SIBLING FILE'S SckArtState (read before wiring in)
// ---------------------------------------------------------------------------------------------
// Per hand-off instruction this is a NEW SIBLING file -- Automation/SCK_ART.h/.cpp (the already-
// translated 8-function extract) is NOT to be edited. That sibling file already defines its own
// `SckArtState` (a partial mirror of golden TfSCKART's data members) and free functions operating on
// it. This remainder's 8 functions need a LARGER, PARTIALLY-OVERLAPPING subset of golden TfSCKART's
// data members (e.g. both files need `dCurrYield` and `iFTRTCount`/`iNeedRT`/`iInputCount`/
// `iManualRejectCnt`/`iTesterType`/`sLOTSTATUS`/`iCurrentStatus`). Rather than take a dependency on
// the sibling's internal struct layout (which the hand-off explicitly says not to touch/couple to),
// this file defines its OWN independent `SckArtRemainderState` with its own copies of the overlapping
// fields. This is a KNOWN, EXPLICIT judgment call, consistent with the precedent the sibling file
// ITSELF already established for the LastSet-field duplication (its gate #3): "when both this file's
// fields and csystem.cpp's seam are retired in favour of the real LastSet, make sure only ONE copy
// survives" -- the exact same reconciliation debt now applies three ways (SckArtState /
// SckArtRemainderState / csystem.cpp's W7C2_TfSCKARTSeam) for `dCurrYield`, `iFTRTCount`, `iNeedRT`,
// `iInputCount`, `iManualRejectCnt`, `iTesterType`, `sLOTSTATUS`, `iCurrentStatus`. FLAGGED for human
// review at the eventual FormsFacade/TfSCKART unification point (see sibling file's own header for
// the fold-in recipe).
//
// ---------------------------------------------------------------------------------------------
// GATED DEPENDENCIES (mirrors the sibling file's local-TU-macro convention; every gate re-cited at
// its call site in the .cpp)
// ---------------------------------------------------------------------------------------------
//   #1  GetRecipePath()/GetRecipeFileName(AnsiString)/MyForceDirectories(AnsiString) -- golden
//       common.cpp:2039/2044 (bodies exist in golden) but common.h:287-300 gates them `#if 0 //
//       TODO(wave-path)/TODO(wave-file)` in THIS translated tree -- declared (common.h) but NOT
//       DEFINED anywhere linkable yet. TU-local stand-ins that skip the per-recipe subfolder
//       (GetLastOpenFN(), itself gated) and persist directly under the real `DataPath` global
//       (common.cpp:87, NOT gated) instead of `DataPath+GetLastOpenFN()+"\"`. JUDGMENT CALL: this
//       differs from golden when multiple named recipes coexist (golden nests one Tester.Data per
//       recipe folder; this stand-in shares a single file across all recipes) -- flagged for human
//       review once GetLastOpenFN/GetRecipePath/GetRecipeFileName/MyForceDirectories are for-real
//       translated (see MIGRATION_ROADMAP.md's "wave-path"/"wave-file" DEFERRED entries).
//   #2  FTestIF->ReadTestIFFile() -- golden cTesterIF.h (class TFTestIF, a whole untranslated VCL
//       form). No-op stand-in (re-reading the tester-interface file has no bearing on this wave's
//       ART lot-tracking state).
//   #3  [RESOLVED -- AI(W5-Final-Integrate) 20260711] fMain->SetStartModeData() /
//       fMain->LoadTestModePicture() / fMain->BackupSetupFile() -- golden main.h (TfMain methods).
//       INTEGRATE added all 3 as offline no-op methods to FormsFacade.h/.cpp's TfMain; the local
//       W5SCKARTREM_FMAIN_* macros in the .cpp now forward to the real facade calls.
//   #4  13 LastSet fields absent from canary_support.h's LAST_GENERAL_SET shim, all VERIFIED present
//       at these exact golden LastSet.h lines: bBreakSCKART(:420), iSCKART_RTUnitCount(:410),
//       lSCKARTBinCT[256](:507), iHdPass(:20), iHdFail(:21), iHdRejectATPass(:22),
//       iHdRejectATFail(:23), iHdRejectBT(:24), iTesterMatch(:25), iTesterUnmatch(:26),
//       iTestIgnore(:27), iHdPickUp(:28). TU-local zero-initialized stand-ins (matches the shim's
//       own all-fields-default-0/false posture). CROSS-UNIT NOTE: the sibling SCK_ART.cpp ALREADY
//       carries its OWN separate copies of iSCKART_RTUnitCount and lSCKARTBinCT[256] (same golden
//       fields, its own gate #3) -- when folding these into the real LastSet, retire ALL THREE
//       copies (sibling SCK_ART.cpp / this file / csystem.cpp's W7C2_TfSCKARTSeam) so exactly one
//       survives. NOT touched by this wave's translated functions but STILL genuinely needed by the
//       full SCK_ART.cpp remainder (per the hand-off's own field list) and hence still owed to the
//       integrate agent: bWaitStartLotAutoRetestGPIB(:400), bEndLotAutoRetestGPIB(:402),
//       bFirstTestAutoRetestGPIB(:405), bUseTestSocketEE[2][4][8](:392) -- these 4 are consumed by
//       DoARTLotStart/DoAutoSocketOff (DoAutoSocketOff already gated by the sibling file's own
//       gate #3 for bUseTestSocketEE), both out of THIS wave's translated-function set.
//   #5  LotSummary (golden cSocket.h `class TLotSummary`, extern global `LotSummary`) -- cSocket.h/
//       .cpp is not translated at all yet (same untranslated family as the sibling file's gate #4
//       ArmData/GetPCA). ClearLotInfo only touches `LotSummary.ClearAllData()` (golden body:
//       cSocket.cpp:754-763, zeroes iCountCategory/iTotalCategory/iLastTotalCategory/iLoadTotal +
//       3 file-scope E1/E2/E3 error counters, ALL currently untranslated/unconsumed elsewhere in
//       this tree) and `LotSummary.iByLotLoadCount[5]` (a plain array, zeroed element-by-element in
//       golden, no method call). TU-local minimal stand-in struct carrying exactly these two
//       members; ClearAllData() is a documented no-op (harmless: the fields it would zero are not
//       read by any other translated code yet).
//   #6  fTesterTCP->ProcessOSPrint() -- golden Automation/TesterTCP.h/.cpp (Interface/TesterTCP is
//       still DEFERRED per MIGRATION_ROADMAP.md's W5 "Automation 剩餘" list). No-op stand-in; only
//       reached when TestIF_File.iTestType==TCP_IP_MODE && iSaveData!=0.
//   #7  Save2DSortingSummary/SaveTestSummarySECS/SaveTestSummaryTSV/SaveSummaryTrayFeed -- the four
//       giant report-writer bodies this wave intentionally defers (golden :1647-4062, see GOLDEN
//       LINE BOUNDARY above). SaveTestSummary's dispatcher (translated for-real in this file) calls
//       TU-local no-op stand-ins for all four; the dispatch LOGIC (which of the four golden would
//       have picked, per CosFunction/TestIF_File/LastSet flags) is preserved verbatim and is directly
//       testable even though the callees are stubs.
//   #8  WriteLastDataFile() / CustomerFunctionSelect() / RunInfo.AddAlarm() -- DISCOVERED LATE, only
//       by an actual real-link smoke test (not just -fsyntax-only) run for this hand-off: cprod.h
//       DECLARES all three (WriteLastDataFile:3237, CustomerFunctionSelect:3280, RUN_INFO::AddAlarm:
//       2755) and cprod.cpp contains their TEXT (1944-2043 / 3686-3831 / 984-1028) verbatim, but
//       cprod.cpp wraps its ENTIRE function-body region (line 184 through 4036 -- effectively the
//       whole file after the global-variable-definitions head) in one blanket `#if 0 // TODO(W6)`.
//       None of these three are linkable symbols in this translated tree yet. TU-local no-op stand-
//       ins (`RUN_INFO RunInfo` the GLOBAL INSTANCE is real/active, cprod.cpp:64, outside the gate --
//       only its `.AddAlarm()` method body is gated). See translate report for why the extra real-
//       link step (beyond -fsyntax-only) mattered here.
// ---------------------------------------------------------------------------------------------

#ifndef AUTOMATION_SCK_ART_REMAINDER_CORE_H
#define AUTOMATION_SCK_ART_REMAINDER_CORE_H

#include "vclcompat/vcl_compat.h"   // AnsiString

// ---------------------------------------------------------------------------
// SckArtRemainderState -- this file's own-field subset of golden TfSCKART (Automation/SCK_ART.h)
// that the 8 functions below read/write. Field names/types verbatim from golden; see the file-head
// "WHY A SEPARATE STATE STRUCT" note above for why this does NOT reuse the sibling file's
// `SckArtState` (some fields, e.g. dCurrYield/iFTRTCount/iNeedRT, are INTENTIONAL duplicates -- flagged
// there for eventual reconciliation).
// ---------------------------------------------------------------------------
struct SckArtRemainderState
{
    // -- setup/lot core (golden SCK_ART.h:236-251, :288-289, :285, :261, :349, :351) --
    AnsiString sSetupFilePath;     // golden :240 (computed by SckArtRem_SetSetupFilePath)
    AnsiString sLOTSTATUS;         // golden :236
    AnsiString sLotID;             // golden :237
    AnsiString sProcessCode;       // golden :238
    AnsiString sLotStartTime;      // golden :241
    int    iCurrentStatus;         // golden :251
    int    iTesterType;            // golden :249  (0:Flex, 1:93K)
    int    iInputCount;            // golden :245
    int    iLotCount;              // golden :246
    int    iFTRTCount;             // golden :248
    int    iManualRejectCnt;       // golden :250
    int    iInputJamCnt;           // golden :288
    int    iOutputJamCnt;          // golden :289
    int    iNeedRT;                // golden :285
    double dCurrYield;             // golden :261
    int    iManualStart;           // golden :349
    bool   bFirstFullSkip;         // golden :351

    // -- SPIL/customer lot-info fields (golden SCK_ART.h:307-323) --
    AnsiString sInfo_Customer, sInfo_InnerLotID, sInfo_CustLotID, sInfo_CustDevGup,
               sInfo_DeviceName, sInfo_Stage, sInfo_Step, sInfo_ReportCnt,
               sInfo_ProgramName, sInfo_TestBinNo, sInfo_TesterID, sInfo_HandlerID,
               sInfo_Temperauture, sInfo_CurrQty, sInfo_OperatorID, sInfo_BinSet;
    int iInfo_MultiLotCnt;         // golden :323

    // -- multi-lot (5-slot) array mirrors (golden SCK_ART.h:325-340) --
    AnsiString sInfoArr_Customer[5], sInfoArr_InnerLotID[5], sInfoArr_CustLotID[5],
               sInfoArr_CustDevGup[5], sInfoArr_DeviceName[5], sInfoArr_Stage[5],
               sInfoArr_Step[5], sInfoArr_ReportCnt[5], sInfoArr_ProgramName[5],
               sInfoArr_TestBinNo[5], sInfoArr_TesterID[5], sInfoArr_HandlerID[5],
               sInfoArr_Temperauture[5], sInfoArr_CurrQty[5], sInfoArr_OperatorID[5],
               sInfoArr_BinSet[5];

    // -- bundle-list fields (golden SCK_ART.h:342-344) --
    AnsiString sBundleList;
    int iBundleInCnt;
    int iBundleOutCnt;

    // Golden ctor (SCK_ART.cpp:36-120) explicitly zero/NONE-inits the subset this struct mirrors;
    // fields with no explicit golden ctor line get BCB6's implicit TForm-member zero-init (0/""/false)
    // -- same convention the sibling file's SckArtState ctor documents.
    SckArtRemainderState();
};

// ---------------------------------------------------------------------------
// SckArtRem_SetSetupFilePath -- golden TfSCKART::SetSetupFilePath() (SCK_ART.cpp:183-188). See gate
// #1 for the GetRecipePath/GetRecipeFileName/MyForceDirectories stand-ins this uses.
// ---------------------------------------------------------------------------
void SckArtRem_SetSetupFilePath(SckArtRemainderState &st);

// ---------------------------------------------------------------------------
// SckArtRem_AccessFile -- golden TfSCKART::AccessFile(bool,int=-1) (SCK_ART.cpp:190-444).
//   Extract-calc-core: golden's widget-refresh block (:335-403, every line a `palXxx->Caption=`/
//   `edXxx->Text=`/`cbXxx->Checked=`/`cbXxx->ItemIndex=` VCL write with NO state read back by other
//   logic) is INTENTIONALLY NOT reproduced -- see gate list above for the handful of calls still
//   inside that overall function that ARE preserved in spirit: the bCanRunSCKART logic and the
//   UpdateCount() call are for-real; WriteLastDataFile()/CustomerFunctionSelect() are gated no-ops
//   (gate #8 -- both are text-present-but-link-absent in cprod.cpp today, discovered via this wave's
//   own real-link smoke test, NOT a deliberate skip).
// ---------------------------------------------------------------------------
void SckArtRem_AccessFile(SckArtRemainderState &st, bool bRead, int iAccess = -1);

// SckArtRem_SetGPIBVersion -- golden TfSCKART::SetGPIBVersion(AnsiString) (SCK_ART.cpp:446-475).
void SckArtRem_SetGPIBVersion(SckArtRemainderState &st, AnsiString Str);

// ---------------------------------------------------------------------------
// SckArtRem_UpdateCount -- golden TfSCKART::UpdateCount() (SCK_ART.cpp:477-591).
//   Extract-calc-core: skips every `palXxx->Caption=`/`->Visible=` VCL write (the ENTIRE function
//   body except the yield/LastSet computations below is such a write). Preserves: the
//   iUnloadCnt/iTotalUnloadCount tallies (local), the 4 LastSet.iHd* writes (bRENESAS_EnableFTCT
//   branch), and st.dCurrYield (consumed by the sibling file's CheckNeedRT -- see the "WHY A
//   SEPARATE STATE STRUCT" cross-file reconciliation note above).
// ---------------------------------------------------------------------------
void SckArtRem_UpdateCount(SckArtRemainderState &st);

// ---------------------------------------------------------------------------
// SckArtRem_AddAlarmCode -- golden TfSCKART::AddAlarmCode(AnsiString,AnsiString,int) (SCK_ART.cpp:
//   593-624). Extract-calc-core: the memoAlarmList Add/SaveToFile block (a VCL TMemo alarm-history
//   JOURNAL, loaded back only by golden FormShow (:100) for on-screen display -- never consumed by
//   other logic) is intentionally not reproduced. The one state-affecting tail that golden runs
//   unconditionally -- RunInfo.AddAlarm(Code,Message) when iDuplicate==0 && Code contains "JAM"
//   (golden :619-623) -- is a GATED no-op here (gate #8: RUN_INFO::AddAlarm's body is
//   TODO(W6)-gated in cprod.cpp, not actually linkable yet), not a deliberate extract-calc-core skip.
// ---------------------------------------------------------------------------
void SckArtRem_AddAlarmCode(AnsiString Code, AnsiString Message, int iDuplicate);

// ---------------------------------------------------------------------------
// SckArtRem_ClearLotInfo -- golden TfSCKART::ClearLotInfo() (SCK_ART.cpp:837-923). See gate #5 for
// the LotSummary stand-in this uses, and gate #4 for the LastSet.iHd*/iTester*/iTestIgnore fields.
//   `fSCKART->SetLotStatus(iLOTSTATUS_NONE)` (golden :853) becomes an inline
//   `st.sLOTSTATUS="NONE"; st.iCurrentStatus=0;` (iLOTSTATUS_NONE==0, matching golden's own default:
//   branch value -- see the sibling file's SetLotStatus for the identical mapping) rather than a
//   second SetLotStatus implementation; this file does not duplicate the sibling's SetLotStatus
//   switch/case (only its `default:` outcome is reachable here since iLOTSTATUS_NONE is a fixed 0).
//   `fSCKART->AccessFile(false)` (golden :921) is exposed via the bDoAccessFile out-param instead of
//   being called internally -- lets the caller decide whether/how to invoke SckArtRem_AccessFile
//   (which needs the SAME SckArtRemainderState instance; calling it recursively from inside
//   ClearLotInfo would be a value-semantics trap given st is passed by reference).
// ---------------------------------------------------------------------------
void SckArtRem_ClearLotInfo(SckArtRemainderState &st, bool *outNeedAccessFileWrite = 0);

// ---------------------------------------------------------------------------
// SckArtRem_AddOutputJamCnt -- golden TfSCKART::AddOutputJamCnt(int,int,int,int=0) (SCK_ART.cpp:
//   1361-1389). row/col index into OutArmSuck.iBinData[][]; iBinOnCarryKit defaults to 0 (golden
//   SCK_ART.h:302 default arg) meaning "look up the bin from OutArmSuck instead".
// ---------------------------------------------------------------------------
void SckArtRem_AddOutputJamCnt(SckArtRemainderState &st, int row, int col, int ret, int iBinOnCarryKit = 0);

// ---------------------------------------------------------------------------
// SckArtRem_SaveTestSummary -- golden TfSCKART::SaveTestSummary(int=0) (SCK_ART.cpp:1619-1645).
//   Dispatcher logic translated verbatim; the 4 callees are gated no-op stand-ins (gate #7) --
//   deferred to next wave. iSaveData default 0 matches golden SCK_ART.h:296.
// ---------------------------------------------------------------------------
void SckArtRem_SaveTestSummary(int iSaveData = 0);

#endif // AUTOMATION_SCK_ART_REMAINDER_CORE_H
