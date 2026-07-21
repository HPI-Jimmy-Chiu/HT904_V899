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
//     TfSCKART::DoARTLotStart(AnsiString,AnsiString,int) -> SckArtRem_DoARTLotStart (golden :4191-4256)
//                                                    [AI(W906-DoARTLotStart) 20260721 -- see that
//                                                    function's own doc comment below for the 2 new
//                                                    gates (#9/#10) and the 2 new struct fields it needed]
//     TfSCKART::SaveTestSummarySECS(int)        -> SckArtRem_SaveTestSummarySECS (golden :1647-2044)
//                                                    [AI(W906-SaveTestSummarySECS) 20260721 -- see that
//                                                    function's own doc comment below for gates #11/#12,
//                                                    the 3 new struct fields, and gate #5's stub extension]
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
//   REMAINING (next wave, golden :1647-4358 minus DoARTLotStart :4191-4256 [DONE, see
//   AI(W906-DoARTLotStart) above] and SaveTestSummarySECS :1647-2044 [DONE, see
//   AI(W906-SaveTestSummarySECS) above]; DoChkInputCntAlarm :4359-4391 already done by the
//   sibling SCK_ART.h/.cpp): SaveMultiLotTestSummary(:2045-2804)/SaveTestSummaryTSV(:2805-3128)/
//   SaveSummaryTrayFeed(:3129-3401)/Save2DSortingSummary(:3402-4062)
//   (four large report-file-writer bodies, ~2200 golden lines combined -- gated no-op stand-ins in
//   THIS file at their SaveTestSummary dispatch call sites, see gate #7), srvrscktTSVClientRead/
//   TimerTSVTimer(:4063-4102, VCL socket/timer events -- TimerTSV's own ->Enabled arm/disarm is now
//   gate #12, see below), FTP_Upload(:4103-4180, TNMFTP VCL component),
//   edtMRejectCntClick/edtAlmAutoCloseSiteClick(:4181-4190, VCL), ledShowFTCTDataClick/
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
//       [UPDATE -- AI(W906-DoARTLotStart) 20260721]: re-verified against the CURRENT canary_support.h
//       (not just this file's own history) before translating DoARTLotStart below -- 3 of the 4 "still
//       owed" fields above are NO LONGER stand-ins: a separate, later integrate pass (also dated
//       20260711, canary_support.h:139-146, "AI(W5-Final-Integrate)") already added
//       bWaitStartLotAutoRetestGPIB/bEndLotAutoRetestGPIB/bFirstTestAutoRetestGPIB (plus
//       bUseTestSocketEE) as REAL fields on the real LastSet global, anticipating this exact wave.
//       SckArtRem_DoARTLotStart therefore reads/writes LastSet.bWaitStartLotAutoRetestGPIB/
//       bEndLotAutoRetestGPIB/bFirstTestAutoRetestGPIB DIRECTLY -- no new TU-local stand-in added for
//       these 3. bUseTestSocketEE remains unconsumed by this wave (DoAutoSocketOff is still out of
//       scope).
//   #5  LotSummary (golden cSocket.h `class TLotSummary`, extern global `LotSummary`) -- cSocket.h/
//       .cpp is not translated at all yet (same untranslated family as the sibling file's gate #4
//       ArmData/GetPCA). ClearLotInfo only touches `LotSummary.ClearAllData()` (golden body:
//       cSocket.cpp:754-763, zeroes iCountCategory/iTotalCategory/iLastTotalCategory/iLoadTotal +
//       3 file-scope E1/E2/E3 error counters, ALL currently untranslated/unconsumed elsewhere in
//       this tree) and `LotSummary.iByLotLoadCount[5]` (a plain array, zeroed element-by-element in
//       golden, no method call). TU-local minimal stand-in struct carrying exactly these two
//       members; ClearAllData() is a documented no-op (harmless: the fields it would zero are not
//       read by any other translated code yet).
//       [UPDATE -- AI(W906-SaveTestSummarySECS) 20260721]: SaveTestSummarySECS is this stub's FIRST
//       real READER (`LotSummary.iCountCategory[][]`/`.iTotalCategory[]`, golden :1803/:1828/:1833/
//       :1914/:1927/:1936/:1940) -- extended in place with exactly those 2 fields (dimensions taken
//       from golden cSocket.h's real `class TLotSummary`: `int iCountCategory[MAX_SOCKET_ROW*
//       MAX_SOCKET_COL][TEST_MAX_BIN]`, `int iTotalCategory[TEST_MAX_BIN]`), plus a NOW-REAL
//       ClearAllData() that zeroes those 2 fields (still not iLastTotalCategory/iLoadTotal/E1/E2/E3 --
//       same "no consumer yet" reasoning as the original note above). Because ClearLotInfo's own
//       `W5SCKARTREM_LOTSUMMARY_CLEARALLDATA()` call (golden :875) uses the SAME macro, this is a
//       fix-forward side effect for that existing call site too (it now actually zeroes something,
//       where before it was a pure no-op) -- not a scope violation, since gate #5 is a file-wide gate,
//       not owned by any one function. ALSO: the underlying storage lost its `static` (internal
//       linkage) -- unlike gate #4's LastSet stand-ins (see the test file's own LIMITATION 3), gate #5's
//       stub is now `extern`-linked and its struct type moved to THIS header, specifically so this
//       wave's own test can SEED iCountCategory/iTotalCategory and observe real Site/Bin/Yield report
//       content and the iUnloadCount-gated branches (an unseedable static would make that coverage
//       vacuous). `iByLotLoadCount[5]` (ClearLotInfo's own field) is unaffected in behavior, just now
//       reachable by name from outside this TU too.
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
//   #9  [NEW -- AI(W906-DoARTLotStart) 20260721] fMain->SetLotState(int) -- golden main.h (TfMain
//       method), called at SCK_ART.cpp golden :4249. csystem.cpp:2320 already carries an EXACT macro
//       precedent for this identical golden call pattern -- `W7C2_FMAIN_SETLOTSTATE(n)`, itself a pure
//       no-op (`do { (void)(n); } while(0)`) -- but it is #define'd LOCAL to csystem.cpp (not in any
//       header), so it is NOT reachable from this TU without an unwanted new dependency on that file's
//       internals. TU-local no-op stand-in defined here instead, matching (a) the SAME no-op behavior
//       as csystem.cpp's W7C2_FMAIN_SETLOTSTATE, and (b) the SAME "TU-local stand-in macro" idiom
//       already used by gates #1/#2/#6/#7/#8 above -- gate #3's alternative idiom (FormsFacade-
//       forwarding) is reserved for methods a PRIOR integrate wave already added for real to
//       FormsFacade.h's TfMain/TfSCKART; SetLotState is not one of them, and adding it now would touch
//       FormsFacade.h, outside this wave's file-boundary (Automation/SCK_ART_Remainder.{h,cpp} + test
//       only). FLAGGED for a future integrate: if/when TfMain::SetLotState lands for real, retarget
//       BOTH this macro and csystem.cpp's W7C2_FMAIN_SETLOTSTATE onto it.
//   #10 [NEW -- AI(W906-DoARTLotStart) 20260721] fMain->tESDError->Add(AnsiString) -- golden
//       `TStringList *tESDError;` (main.h:1394, an ESD-error-code queue), called at SCK_ART.cpp golden
//       :4251-4252. Verified by grep: ZERO hits anywhere in this translated tree (no prior gate, no
//       FormsFacade member, unlike gate #9's SetLotState which at least had the csystem.cpp macro
//       precedent) -- this is a genuinely brand-new gap, not just a reachability problem. TU-local
//       no-op stand-in, same idiom as gate #9.
//   #11 [NEW -- AI(W906-SaveTestSummarySECS) 20260721] ShellExecute(NULL,NULL,strFileName.c_str(),
//       NULL,NULL,SW_SHOW) -- golden SCK_ART.cpp:2027, opens the just-written lot-summary .txt via its
//       OS file association (a VISIBLE operator-convenience pop-open; nothing downstream reads back
//       success/failure). VCL ShellExecute has no vclcompat/portable-C++ equivalent in this tree --
//       Interface/TesterTCP.cpp's AI(W906-D-Interface-TesterTCP) comment already flagged the identical
//       gap and chose a REAL `system("start \"\" /B ...")` launch there because ITS 2 ShellExecute call
//       sites (7z.exe recipe zip/unzip) are functionally load-bearing. THIS call is pure UI convenience
//       with zero downstream state -- the same category this file's own header already puts every
//       widget-refresh block in ("no non-UI representation -- intentionally NOT reproduced"). TU-local
//       no-op stand-in instead (matching gate #6's identical judgment call), but captured into an
//       OBSERVABLE global (`W5SckArtRem_LastShellExecuteOpenPath`, declared below) rather than a bare
//       macro, so this wave's own test can assert the golden "strFileName can be empty" quirk (see the
//       call site's own inline comment) actually reaches this stand-in UNCONDITIONALLY, exactly as
//       golden's real ShellExecute would receive it -- including the empty-string case.
//   #12 [NEW -- AI(W906-SaveTestSummarySECS) 20260721] TimerTSV->Enabled=true -- golden SCK_ART.cpp:2038,
//       a VCL TTimer arm/disarm (golden SCK_ART.h:183 `TTimer *TimerTSV;`). Its OnTimer handler
//       (TimerTSVTimer, golden :4074-4102) is explicitly still on the REMAINING list above (VCL
//       socket/timer events, deferred) -- arming a timer whose handler does not exist yet in this tree
//       has no observable effect, so this is a TU-local no-op stand-in (same idiom as gate #9's
//       fMain->SetLotState).
// ---------------------------------------------------------------------------------------------

#ifndef AUTOMATION_SCK_ART_REMAINDER_CORE_H
#define AUTOMATION_SCK_ART_REMAINDER_CORE_H

#include "vclcompat/vcl_compat.h"   // AnsiString
// AI(W906-SaveTestSummarySECS) 20260721: 2 new includes.
//   MachineType.h -- MAX_SOCKET_ROW/MAX_SOCKET_COL/TEST_MAX_BIN, needed here (not just the .cpp) now
//   that gate #5's LotSummary stub struct (below) is declared in this header, not TU-local anymore.
//   myTimer.h -- TQPF_Timer, needed for SckArtRemainderState's new TSVdelay field below (golden
//   SCK_ART.h:303 `TQPF_Timer TSVdelay;`) -- REAL, already-translated substrate (myTimer.h/.cpp), not
//   a stand-in; both headers are self-contained (own include guards) and already transitively pulled
//   in by this file's sibling .cpp via cmydef.h, so this is a no-op for anything that already compiles
//   this file's .cpp, and only newly needed for OTHER TUs (e.g. the test file) that include ONLY this
//   header.
#include "MachineType.h"
#include "myTimer.h"

// ---------------------------------------------------------------------------
// W5SckArtRem_LotSummaryStub -- gate #5 TU-local stand-in for golden cSocket.h's `class TLotSummary`
// (extern global `LotSummary`). Originally added by an earlier wave (ClearLotInfo) as a `static`
// (internal-linkage) 1-field struct; AI(W906-SaveTestSummarySECS) 20260721 extends it IN PLACE with
// the 2 additional fields SaveTestSummarySECS reads (dimensions verbatim from golden cSocket.h's real
// `class TLotSummary`) and moves it here (external linkage now) so THIS wave's test can seed content --
// see gate #5's full doc comment above for the complete rationale.
// ---------------------------------------------------------------------------
struct W5SckArtRem_LotSummaryStub
{
    int iByLotLoadCount[5];                                            // golden TLotSummary::iByLotLoadCount[5] (ClearLotInfo's own field, unchanged)
    int iCountCategory[MAX_SOCKET_ROW*MAX_SOCKET_COL][TEST_MAX_BIN];    // golden TLotSummary::iCountCategory[Row*Col][Category]
    int iTotalCategory[TEST_MAX_BIN];                                  // golden TLotSummary::iTotalCategory[Category]
    void ClearAllData();                                               // golden cSocket.cpp:754-763 (PARTIAL -- see gate #5's [UPDATE] note above)
};
extern W5SckArtRem_LotSummaryStub W5SckArtRem_LotSummary;

// ---------------------------------------------------------------------------
// W5SckArtRem_LastShellExecuteOpenPath -- gate #11's test-observable capture (see gate #11's doc
// comment above). NOT a real launch -- see SckArtRem_SaveTestSummarySECS's own call-site comment for
// the golden "strFileName can be empty" quirk this exists to make assertable.
// ---------------------------------------------------------------------------
extern AnsiString W5SckArtRem_LastShellExecuteOpenPath;

// ---------------------------------------------------------------------------
// SckArtRemainderState -- this file's own-field subset of golden TfSCKART (Automation/SCK_ART.h)
// that the 9 functions below read/write. Field names/types verbatim from golden; see the file-head
// "WHY A SEPARATE STATE STRUCT" note above for why this does NOT reuse the sibling file's
// `SckArtState` (some fields, e.g. dCurrYield/iFTRTCount/iNeedRT, are INTENTIONAL duplicates -- flagged
// there for eventual reconciliation).
// ---------------------------------------------------------------------------
struct SckArtRemainderState
{
    // -- setup/lot core (golden SCK_ART.h:236-251, :263-264, :288-289, :285, :261, :349, :351) --
    AnsiString sSetupFilePath;     // golden :240 (computed by SckArtRem_SetSetupFilePath)
    AnsiString sLOTSTATUS;         // golden :236
    AnsiString sLotID;             // golden :237
    AnsiString sProcessCode;       // golden :238
    AnsiString sLotStartTime;      // golden :241
    // AI(W906-SaveTestSummarySECS) 20260721: 1 more golden TfSCKART member (SCK_ART.h:242) this file's
    // prior 9 functions never touched -- write-only until now (SaveTestSummarySECS both writes it,
    // golden :1672, and reads it back later in the SAME function, golden :1747/:1792/:1965).
    AnsiString sLotEndTime;        // golden :242
    int    iCurrentStatus;         // golden :251
    // AI(W906-DoARTLotStart) 20260721: 2 more golden TfSCKART members SckArtRem_DoARTLotStart writes
    // (golden :4213-4214) that this file's prior 8 functions never touched. Yet ANOTHER instance of the
    // "WHY A SEPARATE STATE STRUCT" duplicate-field reconciliation debt documented above: iCurrent93KARTStep
    // ALSO has independent copies on the sibling file's SckArtState (Automation/SCK_ART.h:107) AND on
    // FormsFacade's real TfSCKART (FormsFacade.h -- the one AMR.cpp/HANA_ART.cpp actually read via
    // `fSCKART->iCurrent93KARTStep`); iCurrentFlexARTStep has none yet elsewhere in this tree. Since
    // DoARTLotStart only WRITES both fields (never reads them back within its own body), this 3rd/4th
    // copy is write-only and self-contained -- harmless until a future unification wave.
    int    iCurrent93KARTStep;     // golden SCK_ART.h:263
    int    iCurrentFlexARTStep;    // golden SCK_ART.h:264
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

    // -- AI(W906-SaveTestSummarySECS) 20260721: 3 more golden TfSCKART members SaveTestSummarySECS
    // writes (golden SCK_ART.h:286-287/:303) that this file's prior functions never touched. Same
    // "write-only until a future wave" posture as DoARTLotStart's iCurrent93KARTStep/iCurrentFlexARTStep
    // above -- nothing in THIS wave's translated code reads bShowTSVMsg/sTSVMsg/TSVdelay back; they are
    // only meaningful once the deferred TimerTSVTimer handler (golden :4074-4102, see gate #12) lands.
    bool       bShowTSVMsg;        // golden :286
    AnsiString sTSVMsg;            // golden :287
    TQPF_Timer TSVdelay;           // golden :303 (REAL substrate, myTimer.h -- not a stand-in)

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
//   Dispatcher logic translated verbatim; 3 of the 4 callees remain gated no-op stand-ins (gate #7,
//   deferred to next wave). AI(W906-SaveTestSummarySECS) 20260721: added the `st` parameter (golden
//   dispatcher body itself touches no TfSCKART member, but its SECS branch now calls the real
//   SckArtRem_SaveTestSummarySECS below, which needs one) -- this function had NO other caller in this
//   tree yet (verified by grep; only this header's own declaration + its PART-8 test), so widening the
//   signature is not a breaking change. iSaveData default 0 matches golden SCK_ART.h:296.
// ---------------------------------------------------------------------------
void SckArtRem_SaveTestSummary(SckArtRemainderState &st, int iSaveData = 0);

// ---------------------------------------------------------------------------
// SckArtRem_SaveTestSummarySECS -- golden TfSCKART::SaveTestSummarySECS(int) (SCK_ART.cpp:1647-2044).
//   AI(W906-SaveTestSummarySECS) 20260721.
//
//   Dependency verification (each checked directly against the CURRENT target tree, not assumed):
//     * fLotInfo->edtSysLotID->Text -- REAL, FormsFacade.h TfLotInfoEdit (golden uLotInfo.h).
//     * MyForceDirectories(AnsiString,AnsiString="") -- REAL as of TODAY's separate
//       AI(W906-CommonWaveFile) 20260721 pass (common.h:321/common.cpp:1648) -- this file's OWN gate #1
//       stand-in (W5SckArtRem_MyForceDirectories) is now STALE for new code (common.h no longer #if 0
//       gates the real body) but is left untouched here since SckArtRem_SetSetupFilePath still uses it
//       and retargeting that is out of this wave's scope. This NEW function calls the REAL
//       MyForceDirectories directly, matching golden's own 2-arg call shape verbatim.
//     * ChangeToPercentage<T>(T,T) -- REAL template, MachineType.h:1585.
//     * FileExists/DirectoryExists/CopyFile/StringReplace/TReplaceFlags -- REAL, vclcompat (brought into
//       the global namespace by vcl_compat.h's `using` block).
//     * TestSocket (TMyKitSuck, .iShtRow/.iShtCol) -- REAL, aHotPlateSubstrate.h:361.
//     * Prod (PROD_INFO_ST, .iTrayType[]/.iT6PosCate[]/.iIfErrorT6/.bIsPassBin[]/.iT6CatData[]) -- REAL,
//       cprod.h:1138.
//     * IniConfig (HT9045_CONFIG, .bSPILFunction/.bA38_SLT_Summary/.bN17UploadLotSummary/
//       .asN17LotSummaryPath/.dN09_SearchTime) -- REAL, Config.h:1499.
//     * CosFunction.bUseTSVFunction -- REAL, CosFunction.h:440.
//     * RunInfo.LotStartTime -- REAL, cprod.h:2732/:2818.
//     * LastSet.iSCKARTInputCT, s6ShortTrayName[], asSummaryPath, eTrayCount, iTestBinCount,
//       SystemYear/Month/Date/Hour/Min/Sec, GetTimeInfo(), RecordProcess(), ShowMyMessage(), bWaitTSV --
//       all REAL, already used elsewhere in this same file (canary_support.h/cmydef.h/cpublic.h/
//       MachineType.h, all transitively included already via cmydef.h/cprod.h/common.h).
//     * TSVdelay/TimerTSV/bShowTSVMsg/sTSVMsg -- RECON DISCREPANCY: the hand-off assumed these were
//       "already real substrate elsewhere in the tree". Verified by grep: only `bWaitTSV` (cmydef.cpp)
//       was actually real. `TQPF_Timer` itself (the TYPE golden's `TSVdelay` uses) IS real (myTimer.h,
//       already linked into ht9045_sm), but golden's `TSVdelay`/`bShowTSVMsg`/`sTSVMsg` are themselves
//       TfSCKART MEMBERS (golden SCK_ART.h:286/287/303), not globals -- added as 3 new
//       SckArtRemainderState fields above (write-only this wave, see that struct's own comment).
//       `TimerTSV` (a VCL TTimer*) has no substrate at all -- new gate #12 (TU-local no-op).
//     * LotSummary.iCountCategory[][]/.iTotalCategory[] -- gate #5's existing stub, extended in place
//       (see gate #5's [UPDATE] note above for the exact dimensions and the ClearAllData()
//       fix-forward).
//     * ShellExecute(NULL,NULL,strFileName.c_str(),NULL,NULL,SW_SHOW) -- new gate #11 (TU-local no-op,
//       observable capture only -- see that gate's own doc comment).
//     * ZERO EXPOSURE to ServerSocket/srvrscktTSV, FTP_Upload/TfFTP, fConfiguration, FormHS -- confirmed
//       by direct golden read: none of these 4 appear anywhere in golden :1647-2044. The recon hand-off's
//       "zero exposure" claim HOLDS for this function.
//
//   Golden bug preserved VERBATIM (see the function body's own inline comment at the translation of
//   golden :2025-2027): `strFileName` is only ASSIGNED inside the `if(IniConfig.bN17UploadLotSummary)`
//   block (golden :1979/:2002), but `ShellExecute(...,strFileName.c_str()...)` at :2027 fires on a
//   SEPARATE condition (`bA38_SLT_Summary && FileName!="" && iUnloadCount>0`) -- when N17 upload is off,
//   `strFileName` is still its AnsiString default-init `""` there, so this is a real, latent no-op-open
//   (empty lpFile), not a crash. NOT "fixed" to skip when empty -- see gate #11.
//
//   Golden bug preserved VERBATIM #2: the `IniConfig.bSPILFunction==true` branch's own inner
//   `if(IniConfig.bA38_SLT_Summary) {...} else {...}` (golden :1682-1691) builds the EXACT SAME
//   `FileName.sprintf(...)` call in both arms (byte-for-byte identical argument list) -- a dead branch
//   that differentiates nothing. Preserved verbatim, not collapsed.
//
//   Golden dead local declarations DROPPED (extract-calc-core, same convention as every other function
//   in this file): `map<AnsiString,AnsiString> mapIPList`/`mapIPIter`, `IP`, `tmps1`, `Data`, plain
//   `PathName` (only `PathName2` is ever used), `str2`/`str3` (lowercase -- NOT the heavily-used
//   `Str2`/`Str3`) -- verified by direct golden grep: none of these 7 identifiers are read or written
//   anywhere in golden :1647-2044 beyond their own declaration line.
// ---------------------------------------------------------------------------
void SckArtRem_SaveTestSummarySECS(SckArtRemainderState &st, int iSaveData);

// ---------------------------------------------------------------------------
// SckArtRem_DoARTLotStart -- golden TfSCKART::DoARTLotStart(AnsiString,AnsiString,int) (SCK_ART.cpp:
//   4191-4256). AI(W906-DoARTLotStart) 20260721.
//
//   Dependency verification (each checked directly against the CURRENT target tree, not assumed):
//     * HasICUnderMachine() -- REAL, csystem.h/csystem_predicates.cpp:204. Both that file and this one
//       compile directly into the SAME CMake target (ht9045_sm, see top-level CMakeLists.txt) -- there
//       is no separate "Automation" target and therefore no circular-link risk to check; a plain
//       `#include "csystem.h"` (already used by several other ht9045_sm sources, e.g. acatchtray.cpp,
//       for this exact predicate) is all this file needed to add.
//     * bReadLotInfoFromART / bQAModeFlag -- REAL GLOBALS (cmydef.h/cmydef.cpp), NOT TfSCKART members:
//       verified against golden Automation/SCK_ART.h, which declares NEITHER name (only
//       iCurrent93KARTStep/iCurrentFlexARTStep at :263-264) -- golden's own unqualified use inside
//       TfSCKART::DoARTLotStart already refers to the cmydef.h globals, same as here. No SckArtRemainderState
//       field needed for either (this DIFFERS from the recon hand-off's assumption that they might need
//       adding as struct fields).
//     * SetRunStartMode(int) -- REAL, aHotPlateSubstrate.h:549/.cpp:695 (body is itself a no-op, but the
//       SYMBOL is real/linkable -- not a new gate this file owns). rsmInitial_ART is a real MachineType.h
//       enum value (:611).
//     * RecordProcess(AnsiString,AnsiString="") -- REAL, canary_support.h/.cpp (already used elsewhere
//       in this file's own AddAlarmCode gate #8 comment and widely across ht9045_sm).
//     * fLotInfo->cbProcess->Text / ->SetLotID(AnsiString,bool) / ->SetLotStart(AnsiString) -- REAL,
//       already-callable members of FormsFacade.h's TfLotInfo (offline no-ops for the 2 methods; Text
//       is a plain AnsiString field on the reused TfLotInfoRunMode shape) -- confirms the recon claim.
//     * fMain->SetLotState(int) / fMain->tESDError->Add(AnsiString) -- recon's hand-off flagged
//       SetLotState as "the ONE genuinely new gate needed"; VERIFICATION FOUND TWO, not one --
//       tESDError has no FormsFacade member, no gate, and no golden-macro precedent at all (unlike
//       SetLotState, which at least has csystem.cpp:2320's W7C2_FMAIN_SETLOTSTATE to model). See gates
//       #9/#10 above for both; both are pure TU-local no-op macros (chosen over adding new methods to
//       FormsFacade.h's TfMain, which would step outside this wave's file-boundary).
//     * LastSet.bWaitStartLotAutoRetestGPIB / bEndLotAutoRetestGPIB / bFirstTestAutoRetestGPIB -- this
//       file's OWN gate #4 said these were "still owed" TU-local stand-ins; VERIFICATION FOUND they are
//       now REAL fields on the real LastSet global (canary_support.h:144-146, added by a separate,
//       later "AI(W5-Final-Integrate) 20260711" pass in anticipation of this exact wave). Used directly
//       here, no new stand-in added -- see the [UPDATE] note on gate #4 above.
//
//   Golden quirk preserved VERBATIM (see the function body's own inline comment at the translation of
//   golden :4201): `if(sLotID==_sLotID && (sLotID!="" && _sLotID!=" "))` compares the FIRST half's
//   sLotID against "" (empty string) but the SECOND half's _sLotID against " " (one literal space) --
//   an asymmetry that reads like a BCB6-era typo but is golden's real, shipped behavior. NOT
//   "corrected" to != "" here.
//
//   `fSCKART->SetLotStatus(iLOTSTATUS_W)` (golden :4216) is inlined as `st.sLOTSTATUS="LOTSTATUS_W";
//   st.iCurrentStatus=1;` (iLOTSTATUS_W==1, golden ctor SCK_ART.cpp:44 -- itself a TfSCKART MEMBER
//   golden initializes once and never reassigns, i.e. a de-facto constant, same convention
//   SckArtRem_ClearLotInfo's own comment already documents for iLOTSTATUS_NONE==0), NOT by calling a
//   second SetLotStatus implementation -- same "no 2nd SetLotStatus" precedent as ClearLotInfo above.
//
//   `ClearLotInfo()` (golden :4215) is called via SckArtRem_ClearLotInfo(st,&bNeedAccessFileWrite);
//   per THAT function's own out-param contract, its internal `fSCKART->AccessFile(false)` (golden :921,
//   iAccess defaults to -1 -- ALL groups) is then invoked here explicitly via
//   SckArtRem_AccessFile(st,false) BEFORE the SetLotStatus(iLOTSTATUS_W) inline above runs -- matching
//   golden's real execution order (ClearLotInfo fully returns, INCLUDING its own tail AccessFile(false)
//   call, before DoARTLotStart's next statement runs). One net effect worth flagging: because that
//   AccessFile(false) call happens BEFORE SetLotStatus(iLOTSTATUS_W) overwrites st.sLOTSTATUS back to
//   "LOTSTATUS_W" in memory, and the LATER explicit `AccessFile(false,1)` call (iAccess=1) does NOT
//   persist the LOTSTATUS/iCurrentStatus group (that is gated by iAccess==-1||0||10, see
//   SckArtRem_AccessFile's own dispatch), the freshly-set "LOTSTATUS_W" value is never actually written
//   to the ini file by this function -- a real, faithfully-reproduced golden quirk, not a translation bug.
// ---------------------------------------------------------------------------
void SckArtRem_DoARTLotStart(SckArtRemainderState &st, AnsiString _sLotID, AnsiString _sProcess, int _iLotCount);

#endif // AUTOMATION_SCK_ART_REMAINDER_CORE_H
