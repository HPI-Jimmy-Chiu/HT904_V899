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
//     TfSCKART::Save2DSortingSummary(int)       -> SckArtRem_Save2DSortingSummary (golden :3402-4061)
//                                                    [AI(W906-Save2DSortingSummary) 20260723 -- see that
//                                                    function's own doc comment below for gate #13, the
//                                                    gate #5 bIsRTBin extension, and the 6 new
//                                                    FormsFacade TfLotInfo / 1 new atester_shims
//                                                    TfObserverShim members it needed]
//     TfSCKART::SaveTestSummaryTSV(int)         -> SckArtRem_SaveTestSummaryTSV (golden :2805-3128)
//                                                    [AI(W906-SaveTestSummaryTSV) 20260728 -- see that
//                                                    function's own doc comment below for gates #14/#15,
//                                                    and the 4 new FormsFacade TfLotInfo members it needed]
//     TfSCKART::SaveSummaryTrayFeed()           -> SckArtRem_SaveSummaryTrayFeed (golden :3129-3401)
//                                                    [AI(W906-SaveSummaryTrayFeed) 20260728 -- see that
//                                                    function's own doc comment below for gates #16/#17
//                                                    and the gate #8 extension]
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
//   AI(W906-DoARTLotStart) above], SaveTestSummarySECS :1647-2044 [DONE, see
//   AI(W906-SaveTestSummarySECS) above], Save2DSortingSummary :3402-4061 [DONE, see
//   AI(W906-Save2DSortingSummary) above], and SaveTestSummaryTSV :2805-3128 / SaveSummaryTrayFeed
//   :3129-3401 [BOTH DONE, see AI(W906-SaveTestSummaryTSV)/AI(W906-SaveSummaryTrayFeed) above];
//   DoChkInputCntAlarm :4359-4391 already done by the sibling SCK_ART.h/.cpp):
//   SaveMultiLotTestSummary(:2045-2804)
//   -- DELIBERATELY DEFERRED, NOT a mechanical oversight: its ONLY golden caller is
//   csystem.cpp:10862's DoTrayFeedProcess(), which is ITSELF not yet ported in this tree (verified by
//   grep: csystem.cpp has no DoTrayFeedProcess symbol today) -- translating this ~760-line function now
//   would have zero integration-test value (nothing in the translated tree can ever reach it) and would
//   need its own large dependency-verification pass. Left exactly as currently gated (still a no-op
//   stand-in at its own SaveTestSummary-dispatcher-adjacent call site, if any -- in fact golden's
//   SaveTestSummary dispatcher never calls it directly either; SaveMultiLotTestSummary is reached only
//   via the separate DoTrayFeedProcess path, so THIS file's SckArtRem_SaveTestSummary dispatcher has no
//   call site for it at all, gated or otherwise -- nothing to wire until DoTrayFeedProcess lands),
//   srvrscktTSVClientRead/
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
//       [UPDATE 2 -- AI(W906-Save2DSortingSummary) 20260723]: extended AGAIN with `bool
//       bIsRTBin[TEST_MAX_BIN]` (golden cSocket.h:123) -- Save2DSortingSummary is its first reader
//       (golden SCK_ART.cpp:3511, `if(iNeedRT!=0 && LotSummary.bIsRTBin[iBin]==true)`). Golden's own
//       ClearAllData() never zeroes bIsRTBin either (verified, cSocket.cpp:754-763) so this stub's
//       ClearAllData() is NOT extended for it, matching golden. Nothing writes it yet -> defaults
//       all-false (harmless, see the struct's own comment in the header body below).
//       [UPDATE 3 -- AI(W906-SaveTestSummaryTSV) 20260728]: extended a 3rd time with `int iLoadTotal`
//       (golden cSocket.h:127) -- SaveTestSummaryTSV is its first reader (golden SCK_ART.cpp:2949,
//       `CUSTOMER_CODE==CC_ASE_CL` branch). UNLIKE bIsRTBin, golden's OWN ClearAllData() DOES zero
//       iLoadTotal (cSocket.cpp:759) -- so this stub's ClearAllData() is extended to match.
//   #6  fTesterTCP->ProcessOSPrint() -- golden Automation/TesterTCP.h/.cpp (Interface/TesterTCP is
//       still DEFERRED per MIGRATION_ROADMAP.md's W5 "Automation 剩餘" list). No-op stand-in; only
//       reached when TestIF_File.iTestType==TCP_IP_MODE && iSaveData!=0.
//   #7  SaveTestSummarySECS/SaveTestSummaryTSV/SaveSummaryTrayFeed/Save2DSortingSummary -- originally
//       the four giant report-writer bodies this wave intentionally deferred (golden :1647-4062, see
//       GOLDEN LINE BOUNDARY above). SaveTestSummary's dispatcher (translated for-real in this file)
//       calls TU-local no-op stand-ins for all four; the dispatch LOGIC (which of the four golden would
//       have picked, per CosFunction/TestIF_File/LastSet flags) is preserved verbatim and is directly
//       testable even though (originally) all 4 callees were stubs.
//       [UPDATE -- AI(W906-SaveTestSummarySECS) 20260721]: SaveTestSummarySECS graduated to real; its
//       macro stand-in retired.
//       [UPDATE 2 -- AI(W906-Save2DSortingSummary) 20260723]: Save2DSortingSummary ALSO graduated to
//       real (see that function's own doc comment below); its macro stand-in
//       (W5SCKARTREM_SAVE2DSORTINGSUMMARY) is likewise retired, and the dispatcher's 2D-sort branch now
//       calls SckArtRem_Save2DSortingSummary directly.
//       [UPDATE 3 -- AI(W906-SaveTestSummaryTSV/SaveSummaryTrayFeed) 20260728]: the last 2 -- both
//       SaveTestSummaryTSV and SaveSummaryTrayFeed -- ALSO graduated to real this wave (see each
//       function's own doc comment below). Both macro stand-ins (W5SCKARTREM_SAVETESTSUMMARYTSV /
//       W5SCKARTREM_SAVESUMMARYTRAYFEED) are retired, and SckArtRem_SaveTestSummary's dispatcher now
//       calls SckArtRem_SaveTestSummaryTSV / SckArtRem_SaveSummaryTrayFeed directly at all 3 former
//       macro call sites. Gate #7 is now FULLY RETIRED -- all 4 of the original giant report-writer
//       bodies are real. (SaveMultiLotTestSummary, golden :2045-2804, is separately and deliberately
//       DEFERRED -- see the file-head "GOLDEN LINE BOUNDARY" comment's own note for why it is NOT under
//       this gate at all: golden's SaveTestSummary dispatcher never calls it; its only caller is the
//       not-yet-ported DoTrayFeedProcess.)
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
//       [UPDATE -- AI(W906-SaveSummaryTrayFeed) 20260728]: SAME umbrella, a 4th cprod.h-declared/
//       cprod.cpp-gated-body member found -- `RUN_INFO::SaveJamRateByLot(bool=true)` (cprod.h:2706
//       declares it, cprod.cpp:1205 has its TEXT, but that line is inside the SAME :184-4036 blanket
//       gate as the original 3). Called unconditionally at golden SCK_ART.cpp:3333
//       (`RunInfo.SaveJamRateByLot();`). TU-local no-op stand-in, same idiom as the other 3 under this
//       gate.
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
//   #13 [NEW -- AI(W906-Save2DSortingSummary) 20260723] FTP_Upload(PathName2, IniConfig.sN09_5_Path,
//       FileName) -- golden SCK_ART.cpp:3942, the ONE call site inside Save2DSortingSummary's own body
//       (golden :3402-4061) that reaches `TfSCKART::FTP_Upload(AnsiString,AnsiString,AnsiString)`
//       (golden SCK_ART.cpp:4103-4179 / SCK_ART.h:304). Verified by grep: FTP_Upload has 4 call sites
//       total in golden SCK_ART.cpp (:2684, :3054, :3069, :3942) -- the other 3 (:2684/:3054/:3069) are
//       all inside SaveTestSummaryTSV (golden :2805-3128) -- a DIFFERENT function, translated for real
//       by a LATER wave (AI(W906-SaveTestSummaryTSV) 20260728), which REUSES this SAME gate #13
//       stand-in verbatim (see that function's own doc comment) rather than adding a new gate --
//       exactly the "reuse, don't duplicate" precedent Save2DSortingSummary's own ShellExecute call
//       already set with gate #11. FTP_Upload's own body wraps
//       `TfFTP` (golden ProductionInfo/TfFTP.h) -- confirmed NOT translated anywhere in this tree
//       (grepped HT9011UC_Cpp_V3.33.906.0: the only 2 other TfFTP mentions are Automation/HANA_ART.cpp's
//       own already-gated SendTrayMapToFTP, "not translated (W5 KYECFTP-adjacent scope)", and this same
//       file's own file-head citation of that gate -- no TfFTP class exists anywhere as a linkable
//       symbol). TU-local no-op stand-in, matching gate #6/#7's "whole untranslated dependency" idiom --
//       but, same as gate #11's ShellExecute stand-in, captures its 3 AnsiString arguments into
//       observable globals (W5SckArtRem_LastFTPUpload_Sources/_Target/_FileName, declared below) so
//       this wave's own test can assert the gate was actually REACHED (not just that dispatch didn't
//       crash), distinguishing "FTP_Upload was called with the right args" from "the whole branch was
//       skipped because some precondition was false".
//   #14 [NEW -- AI(W906-SaveTestSummaryTSV) 20260728] fConfiguration->mmoN04_IP->Lines->Count /
//       ->Strings[i] -- golden cConfiguration.h:768 (`TMemo *mmoN04_IP;`), read at SCK_ART.cpp
//       :2903-2905 to build a "this Handler's own IP" auto-pick set for the TSV header. TfConfiguration
//       (cConfiguration.h/.cpp) is a whole untranslated VCL form -- same "whole untranslated
//       dependency" category as gate #6 (fTesterTCP)/gate #13 (TfFTP). Verified by grep: `fConfiguration`
//       has NO existing definition anywhere in this translated tree (only 2 mentions, both inside
//       cprod.cpp's OWN :184-4036 blanket-gated dead zone) -- safe to introduce fresh here with no name
//       collision. TU-local minimal stand-in: a tiny 2-level struct (`fConfiguration->mmoN04_IP->Lines`)
//       whose `Lines` member is a REAL vclcompat `TStringList*` (the SAME already-real substrate this
//       file uses pervasively for sList/sIPList/etc.), not a bespoke read-only vector shape -- so a test
//       can seed it via the ordinary `fConfiguration->mmoN04_IP->Lines->Add(...)` API and the golden
//       `->Lines->Count`/`->Lines->Strings[i]` reads work completely unmodified. Read-only surface (this
//       wave's translated code never writes through it). FLAGGED for eventual reconciliation: if/when
//       cConfiguration.cpp is for-real translated, retarget/retire this stand-in same as every other
//       "whole future module" stub in this project (LotSummary/gate #5, TastCategory/gate #16 below).
//   #15 [NEW -- AI(W906-SaveTestSummaryTSV) 20260728] srvrscktTSV (TServerSocket*) -- golden SCK_ART.h
//       :182 (`TServerSocket *srvrscktTSV;`), a TfSCKART-OWNED member (ctor'd/destroyed alongside the
//       rest of TfSCKART, same "own copy of a TfSCKART member" posture already established for `slExe`
//       -- see SckArtRem_SaveSummaryTrayFeed's own doc comment below), touched at SCK_ART.cpp :3044-3047
//       (`->Active`/`->Close()`/`->Port=`/`->Open()`). UNLIKE every gate above, this is NOT a no-op
//       stand-in -- vclcompat/ServerSocket.h already provides a REAL, already-linked-in `TServerSocket`
//       (used for real by SECSGEM/uHGemEquipment.cpp's `srvGem`), whose default construction mode is
//       SIM (`bSimRequested(true)`, ServerSocket.cpp:93) -- `Open()` in Sim mode "succeeds synchronously
//       and deterministically" with NO real `socket()`/`bind()`/`listen()` call (ServerSocket.cpp:354-357)
//       -- i.e. exactly the "offline-inert" behavior this wave's hand-off asked for, achieved by REUSING
//       real substrate instead of inventing a fake one. `srvrscktTSV` is a fresh own-copy
//       `new TServerSocket(NULL)` (matching `uHGemEquipment.cpp:588`'s own construction shape verbatim),
//       declared/defined in THIS file exactly like gate #14's `fConfiguration` (verified: no existing
//       `srvrscktTSV` symbol anywhere in this tree).
//   #16 [NEW -- AI(W906-SaveSummaryTrayFeed) 20260728] TastCategory (TEST_CATEGORY, golden cSocket.h
//       :144-174, `extern TEST_CATEGORY TastCategory;` at cSocket.h:177) -- cSocket.cpp is a whole
//       future ~1300-line module (same untranslated family as gate #5's LotSummary, from the SAME
//       header). `.UpdataCount(bool)` (golden cSocket.cpp:1029-1255) itself depends on `ArmDataLot`
//       (a `TArm*[3]`, golden cSocket.h:181) -- yet ANOTHER whole untranslated class hierarchy -- so a
//       for-real UpdataCount is out of reach until cSocket.cpp lands. Verified by grep: `TastCategory`
//       has NO ungated definition anywhere in this tree today -- every existing mention is textually
//       excluded, either inside cprod.cpp's OWN :184-4036 blanket gate (cprod.cpp:1244/1258/1262/1265)
//       or inside atester_ProcessCount.cpp's OWN dedicated `#if 0 // TODO(cSocket-module)` gates
//       (:644-649, :827-1001) -- so introducing a real, linkable `TastCategory` global here has NO name
//       collision with any code that actually compiles today. TU-local minimal stand-in (own struct
//       type, global instance named `TastCategory` verbatim matching golden) exposing ONLY the 10
//       fields/1 method SckArtRem_SaveSummaryTrayFeed actually reads (iTotalSocket/iPassSocket/
//       iFailSocket/iRejectCount/iUnloadCnt[eTrayCount]/iBySiteTotal/iBySitePass/iBySiteFail
//       [MAX_SOCKET_ROW*MAX_SOCKET_COL]/iBySiteCate[MAX_SOCKET_ROW*MAX_SOCKET_COL][TEST_MAX_BIN]/
//       iTotalCategory[TEST_MAX_BIN], all POD-zero-initialized, test-seedable directly) --
//       `UpdataCount(bool)` is a no-op (does NOT recompute from ArmDataLot; the test seeds the 10
//       fields directly instead, since ArmDataLot/TArm are out of reach). FLAGGED for eventual
//       reconciliation: when cSocket.cpp is for-real translated, this stand-in AND
//       atester_ProcessCount.cpp's own (currently-dormant, still-gated) `TastCategory.iCountCategory/
//       iCountSocketTotal/iCountHeadTotal` references must be reconciled onto the SAME real
//       `TEST_CATEGORY` definition -- same 3-way-duplication caution already flagged for LotSummary/
//       gate #5 and the SckArtState/SckArtRemainderState LastSet fields.
//   #17 [NEW -- AI(W906-SaveSummaryTrayFeed) 20260728] FormHS->UpDataToServerByFTP(AnsiString,
//       AnsiString,AnsiString,bool=false) -- golden HS_Function.h:93 (`TFormHS *FormHS;` declared
//       HS_Function.h:148), a whole untranslated VCL form (HS_Function.h/.cpp, thousands of lines).
//       Called twice at SCK_ART.cpp :3342/:3346 (`"SLT_Report"`/`"EventLog"` uploads), both as bare
//       discarded-return statements (verified by grep: neither call site captures the `int` result).
//       Verified: NO `FormHS`/`TFormHS` symbol exists anywhere in this translated tree (only inside
//       cprod.cpp's own gated dead zone, cprod.cpp:1100/1310) -- safe to introduce fresh. TU-local
//       no-op stand-in (void return, matching gate #6/#13's "whole untranslated dependency" idiom),
//       but -- same as gate #11/#13's ShellExecute/FTP_Upload stand-ins -- captures its 3 AnsiString
//       arguments into observable globals (`W5SckArtRem_LastFormHSUpload_Dir/_FileName/_Type`, declared
//       below) so this wave's own test can assert each branch was actually reached.
//       ALSO under this gate: golden's OWN `slEventLog->sLotFileName` (SCK_ART.cpp:3344/:3346/:3361)
//       -- golden type is `TMyStringList*` (cmydef.h, the SAME name as the REAL global `extern
//       TMyStringList *slEventLog;` already declared in cmydef.h:117). That real global is NOT usable
//       here: `TMyStringList` is only forward-declared in this tree (cmydef.h:15, `class
//       TMyStringList;`) -- an INCOMPLETE type -- so `slEventLog->sLotFileName` would be a hard compile
//       error (member access on an incomplete type), and the pointer itself is permanently null in this
//       tree besides (nothing can ever `new` an incomplete type to give it a real object). Matches the
//       EXACT gap Automation/HANA_ART.cpp's own AddNewTrayHead already hit and gated wholesale for the
//       same reason (see that file's own comment). Rather than gate this whole function out, a
//       TU-local stand-in under a DELIBERATELY DIFFERENT name (`W5SckArtRem_slEventLog`, NOT
//       `slEventLog` -- reusing the real name would either redeclare-with-different-type at the same
//       scope, a hard error, or silently shadow the real global in a way that would misleadingly look
//       load-bearing) exposes just the one field this function reads: `AnsiString sLotFileName;`
//       (default `""`, harmless: `FileExists("")` is false, so the golden `if(FileExists(slEventLog->
//       sLotFileName))` guard simply stays closed until a test seeds it).
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
// AI(W906-SaveTestSummaryTSV) 20260728: new include, needed for gate #15's `srvrscktTSV` (TServerSocket*)
// declaration below -- REAL substrate (vclcompat/ServerSocket.h), already linked into ht9045_sm via
// SECSGEM/uHGemEquipment.cpp's own `srvGem` member; self-contained (own include guard + transitively
// pulls in ClientSocket.h itself).
#include "vclcompat/ServerSocket.h"

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
    // AI(W906-Save2DSortingSummary) 20260723: gate #5 extended AGAIN, same "first real reader" pattern
    // as the [UPDATE] note above -- Save2DSortingSummary is the first (and so far only) reader of
    // `LotSummary.bIsRTBin[iBin]` (golden cSocket.h:123 `bool bIsRTBin[TEST_MAX_BIN];`, read at golden
    // SCK_ART.cpp :3511/:2847/:3847 -- only the first of those 3 sites is inside THIS wave's function).
    // Verified via golden grep: golden's OWN ClearAllData() (cSocket.cpp:754-763) never touches
    // bIsRTBin either (only iCountCategory/iTotalCategory/iLastTotalCategory/iLoadTotal are zeroed
    // there) -- so, matching golden exactly, this stub's ClearAllData() body below is NOT extended to
    // zero this new field. Nothing in this tree WRITES bIsRTBin yet (it would normally be set via
    // AccessFile/config plumbing not yet translated), so it defaults to all-false (POD zero-init) --
    // harmless: the `iNeedRT!=0 && bIsRTBin[iBin]==true` guard this new field gates is simply never
    // true until a future wave adds a real writer.
    bool bIsRTBin[TEST_MAX_BIN];                                       // golden TLotSummary::bIsRTBin[Category] (cSocket.h:123)
    // AI(W906-SaveTestSummaryTSV) 20260728: gate #5 extended a 3rd time -- SaveTestSummaryTSV is the
    // first reader of `LotSummary.iLoadTotal` (golden cSocket.h:127 `int iLoadTotal;`, read at golden
    // SCK_ART.cpp :2949 inside the `CUSTOMER_CODE==CC_ASE_CL` branch). UNLIKE bIsRTBin above, golden's
    // OWN ClearAllData() DOES zero this field (cSocket.cpp:759, `LotSummary.iLoadTotal=0;`) -- so this
    // stub's ClearAllData() body below IS extended to match (see the .cpp definition).
    int iLoadTotal;                                                    // golden TLotSummary::iLoadTotal (cSocket.h:127)
    void ClearAllData();                                               // golden cSocket.cpp:754-763 (PARTIAL -- see gate #5's [UPDATE] notes above)
};
extern W5SckArtRem_LotSummaryStub W5SckArtRem_LotSummary;

// ---------------------------------------------------------------------------
// W5SckArtRem_LastShellExecuteOpenPath -- gate #11's test-observable capture (see gate #11's doc
// comment above). NOT a real launch -- see SckArtRem_SaveTestSummarySECS's own call-site comment for
// the golden "strFileName can be empty" quirk this exists to make assertable.
// ---------------------------------------------------------------------------
extern AnsiString W5SckArtRem_LastShellExecuteOpenPath;

// ---------------------------------------------------------------------------
// W5SckArtRem_LastFTPUpload_Sources/_Target/_FileName -- gate #13's test-observable capture (see gate
// #13's doc comment above). NOT a real FTP upload -- the 3 arguments golden's own FTP_Upload
// (sSourcesFilePath, sTargetFilePath, sULFileName) was called with, verbatim, so the test can prove the
// call site was reached (not merely that dispatch didn't crash).
// ---------------------------------------------------------------------------
extern AnsiString W5SckArtRem_LastFTPUpload_Sources;
extern AnsiString W5SckArtRem_LastFTPUpload_Target;
extern AnsiString W5SckArtRem_LastFTPUpload_FileName;

// ---------------------------------------------------------------------------
// Gate #14 -- fConfiguration (TU-local minimal stand-in for golden cConfiguration.h's whole
// TfConfiguration form). ONLY the ->mmoN04_IP->Lines surface SckArtRem_SaveTestSummaryTSV reads; Lines
// is a REAL vclcompat TStringList* (test-seedable via ->Add(...), same real substrate used pervasively
// elsewhere in this file) not a bespoke read-only shape. See gate #14's full doc comment above.
// ---------------------------------------------------------------------------
struct W5SckArtRem_ConfigMemoLines
{
    TStringList *Lines;                      // golden TMemo* mmoN04_IP -> Lines (TStrings*), REAL TStringList
    W5SckArtRem_ConfigMemoLines();
};
struct W5SckArtRem_ConfigStub
{
    W5SckArtRem_ConfigMemoLines *mmoN04_IP;  // golden cConfiguration.h:768 (TMemo* mmoN04_IP)
    W5SckArtRem_ConfigStub();
};
extern W5SckArtRem_ConfigStub *fConfiguration;

// ---------------------------------------------------------------------------
// Gate #15 -- srvrscktTSV: a fresh, own-copy REAL vclcompat TServerSocket* (TfSCKART's own member,
// golden SCK_ART.h:182). Defaults to SIM mode (offline-safe, no real socket() call) -- see gate #15's
// full doc comment above.
// ---------------------------------------------------------------------------
extern TServerSocket *srvrscktTSV;

// ---------------------------------------------------------------------------
// Gate #16 -- TastCategory (TU-local minimal stand-in for golden cSocket.h's `class TEST_CATEGORY`,
// extern global `TastCategory`). ONLY the 10 fields SckArtRem_SaveSummaryTrayFeed reads; `UpdataCount`
// is a no-op (ArmDataLot/TArm, its real golden dependency, is a whole separate untranslated hierarchy).
// See gate #16's full doc comment above for the "no name collision with today's gated code" rationale.
// ---------------------------------------------------------------------------
struct W5SckArtRem_TastCategoryStub
{
    int iTotalSocket;                                             // golden TEST_CATEGORY::iTotalSocket
    int iPassSocket;                                              // golden TEST_CATEGORY::iPassSocket
    int iFailSocket;                                               // golden TEST_CATEGORY::iFailSocket
    int iRejectCount;                                              // golden TEST_CATEGORY::iRejectCount
    int iUnloadCnt[eTrayCount];                                    // golden TEST_CATEGORY::iUnloadCnt[eTrayCount]
    int iBySiteTotal[MAX_SOCKET_ROW*MAX_SOCKET_COL];               // golden TEST_CATEGORY::iBySiteTotal[SiteMap]
    int iBySitePass[MAX_SOCKET_ROW*MAX_SOCKET_COL];                // golden TEST_CATEGORY::iBySitePass[SiteMap]
    int iBySiteFail[MAX_SOCKET_ROW*MAX_SOCKET_COL];                // golden TEST_CATEGORY::iBySiteFail[SiteMap]
    int iBySiteCate[MAX_SOCKET_ROW*MAX_SOCKET_COL][TEST_MAX_BIN];  // golden TEST_CATEGORY::iBySiteCate[SiteMap][Category]
    int iTotalCategory[TEST_MAX_BIN];                              // golden TEST_CATEGORY::iTotalCategory[Category]
    void UpdataCount(bool bCheckYield=false);                      // golden cSocket.cpp:1029-1255 -- NO-OP (see gate #16)
};
extern W5SckArtRem_TastCategoryStub TastCategory;

// ---------------------------------------------------------------------------
// Gate #17 -- FormHS (TU-local minimal stand-in for golden HS_Function.h's whole TFormHS form) and
// W5SckArtRem_slEventLog (TU-local stand-in for the `slEventLog->sLotFileName` read, deliberately NOT
// named `slEventLog` -- see gate #17's full doc comment above for why that real cmydef.h global cannot
// be used directly). Both no-op/read-only; FormHS's upload calls are captured into observable globals
// so a test can assert each branch was reached.
// ---------------------------------------------------------------------------
struct W5SckArtRem_FormHSStub
{
    void UpDataToServerByFTP(AnsiString asDirPath, AnsiString sFileName, AnsiString asFileType, bool bDailyReport=false);
};
extern W5SckArtRem_FormHSStub *FormHS;
extern AnsiString W5SckArtRem_LastFormHSUpload_Dir;
extern AnsiString W5SckArtRem_LastFormHSUpload_FileName;
extern AnsiString W5SckArtRem_LastFormHSUpload_Type;

struct W5SckArtRem_SlEventLogStub
{
    AnsiString sLotFileName;   // golden TMyStringList's own extra member (cmydef.h; only ->sLotFileName read here)
};
extern W5SckArtRem_SlEventLogStub W5SckArtRem_slEventLog;

// ---------------------------------------------------------------------------
// SckArtRemainderState -- this file's own-field subset of golden TfSCKART (Automation/SCK_ART.h)
// that the 11 functions below read/write (AI(W906-SaveTestSummaryTSV/SaveSummaryTrayFeed) 20260728:
// neither of the 2 newest functions needed any NEW field here -- both were verified, function-body-wide,
// to touch only fields already present: SaveTestSummaryTSV via `fSCKART->sLotID`/`fSCKART->iNeedRT`,
// SaveSummaryTrayFeed via no bare TfSCKART member at all besides its own `slExe`, which is modeled as a
// function-local TStringList* instead of a struct field -- see that function's own doc comment for why).
// Field names/types verbatim from golden; see the file-head
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
//   Dispatcher logic translated verbatim; 2 of the 4 callees remain gated no-op stand-ins (gate #7,
//   deferred to next wave). AI(W906-SaveTestSummarySECS) 20260721: added the `st` parameter (golden
//   dispatcher body itself touches no TfSCKART member, but its SECS branch now calls the real
//   SckArtRem_SaveTestSummarySECS below, which needs one) -- this function had NO other caller in this
//   tree yet (verified by grep; only this header's own declaration + its PART-8 test), so widening the
//   signature is not a breaking change. iSaveData default 0 matches golden SCK_ART.h:296.
//   AI(W906-Save2DSortingSummary) 20260723: the 2D-sort branch now ALSO calls the real
//   SckArtRem_Save2DSortingSummary (below) instead of the retired W5SCKARTREM_SAVE2DSORTINGSUMMARY macro.
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

// ---------------------------------------------------------------------------
// SckArtRem_Save2DSortingSummary -- golden TfSCKART::Save2DSortingSummary(int) (SCK_ART.cpp:3402-4061).
//   AI(W906-Save2DSortingSummary) 20260723.
//
//   Dependency verification (each checked directly against the CURRENT target tree, not assumed):
//     * fLotInfo->edtSysLotID/edtCusLotID/edtCusDevGrp/edtCusStep/edtDevice/edtSysOperatorID/
//       mmo2DLotInfo (all ->Text only) -- edtSysLotID was already REAL; the other 6 are NEW this wave
//       (FormsFacade.h's TfLotInfo, reusing the existing TfLotInfoEdit {AnsiString Text;} shape).
//       mmo2DLotInfo is a TMemo* in golden but only ->Text is ever read here (golden :3628
//       `sList->Text=fLotInfo->mmo2DLotInfo->Text;`) -- same minimal-shape reasoning as every other
//       TfLotInfoEdit reuse in this tree (edlRTTryCnt, edCleaningCount, ...).
//     * fLotInfo->cbRunMode->Text -- REAL, already on TfLotInfo (golden uLotInfo.h:307).
//     * fObserver->labFactory->Caption -- NEW this wave (atester_shims.h's TfObserverShim), reusing the
//       existing TfObserverLabel {AnsiString Caption;} shape (same as labModel/labPowerOnTime/...).
//     * MachineTypeChoice / Type_HT9045 / Type_HT9045_12Site / Type_HT9046 / Type_HT9046_LS -- REAL,
//       cmydef.h/cmydef.cpp (MachineTypeChoice) + MachineType.h (the 4 eMachineType enumerators),
//       already #include'd transitively via this file's existing MachineDefine.h/MachineType.h/
//       cmydef.h includes.
//     * iE1Count/iE2Count/iE3Count -- REAL globals, cmydef.h:5427-5429/cmydef.cpp:5432-5434 (a 4th
//       sibling, iENotDefinedCount, also exists but is NOT read here -- golden :3928 computes the
//       "994: Not defined" line inline as `LotSummary.iTotalCategory[iTestBinCount]-iE1Count-iE2Count-
//       iE3Count`, never touching iENotDefinedCount itself).
//     * LastSet.SendCT[4] / LastSet.iSCKARTInputCT -- REAL, canary_support.h:73 / already used by
//       SaveTestSummarySECS above.
//     * RunInfo.iUnloadCount / RunInfo.LotStartTime / RunInfo.LotEndTime -- REAL, cprod.h:2721/2732/2818
//       (iUnloadCount confirmed via direct grep this wave; LotStartTime/LotEndTime already used by
//       SaveTestSummarySECS above).
//     * TestIF_File.bSCKART_EnableART -- REAL, cprod.h:2237 (confirmed via direct grep this wave).
//     * IniConfig.bA37LotStartLotEnd/.bA66_2D_Sort/.bN09_LotCountAutoFunc/.sN09_HandlerFolder/
//       .iN09_4_UploadMethod/.sN09_5_Path/.bN23UseLotInfoFile/.bN25FolderWithoutYYMM -- all REAL,
//       Config.h (confirmed via direct grep this wave); .bSPILFunction/.bA38_SLT_Summary/
//       .bN17UploadLotSummary/.asN17LotSummaryPath/.dN09_SearchTime/.SocketHandlerID already used by
//       SaveTestSummarySECS above.
//     * CosFunction.bSortingBy2DList/.bART_SECSGEM_93K/.bUseTSVFunction -- all REAL, CosFunction.h
//       (confirmed via direct grep this wave; the latter two already used elsewhere in this file).
//     * bReadLotInfoFromART -- REAL GLOBAL (cmydef.cpp:5744), NOT a TfSCKART member -- same "verified
//       against golden SCK_ART.h, which declares no such member" reasoning SckArtRem_DoARTLotStart's own
//       comment already established for this identical global.
//     * MainVersion -- REAL, cmydef.h:4193/cmydef.cpp:4367 (`AnsiString MainVersion="";`). The
//       `#ifdef HiSilicon` / `#else` split around it (golden :3554-3557,:3615-3619) is preserved
//       verbatim; HiSilicon itself is `//#define`-commented-out tree-wide (MachineType.h:52), so only
//       the `#else` (MainVersion) arm actually compiles on this tree, matching golden's own default
//       posture on a non-HiSilicon build.
//     * asHandlerVersion/asBackup2DSortListPath/asBackup2DSummaryPath/asSummaryPath/eTrayCount/
//       iTestBinCount/s6ShortTrayName[]/tNotUse/MAX_SOCKET_ROW/MAX_SOCKET_COL/TEST_MAX_BIN/
//       SystemYear.../GetTimeInfo()/RecordProcess()/ShowMyMessage()/bWaitTSV/ChangeToPercentage<T>/
//       MyForceDirectories(AnsiString,AnsiString)/FileExists/DirectoryExists/CopyFile/DeleteFile/
//       StringReplace/TReplaceFlags -- all REAL, already used by SaveTestSummarySECS above (same file,
//       same includes) or common.h/vclcompat.
//     * Prod (.iTrayType[]/.iT6PosCate[]/.iIfErrorT6/.bIsPassBin[]/.iT6CatData[]) / TestSocket
//       (.iShtRow/.iShtCol) -- REAL, already used by SaveTestSummarySECS above.
//     * LotSummary.iCountCategory[][]/.iTotalCategory[] -- gate #5's existing (extended) stub.
//       LotSummary.bIsRTBin[] -- gate #5 extended AGAIN this wave (see gate #5's [UPDATE 2] note and the
//       struct's own comment above) -- this function is bIsRTBin's first (and, per golden grep, only
//       in-scope) reader.
//     * FTP_Upload(PathName2,IniConfig.sN09_5_Path,FileName) -- new gate #13 (TU-local no-op,
//       observable capture -- see that gate's own doc comment for the "only 1 of 4 golden call sites is
//       in-scope" verification).
//     * ShellExecute(NULL,NULL,strFileName.c_str(),NULL,NULL,SW_SHOW) -- REUSES the existing gate #11
//       stand-in (W5SCKARTREM_SHELLEXECUTE_OPEN) verbatim -- this is the SAME golden idiom
//       (SaveTestSummarySECS already established gate #11 for its own, structurally identical,
//       ShellExecute call), not a new gate.
//     * TimerTSV->Enabled=true -- REUSES the existing gate #12 stand-in (W5SCKARTREM_TIMERTSV_ENABLE).
//     * ZERO EXPOSURE to ServerSocket/srvrscktTSV/fConfiguration/FormHS -- confirmed by direct golden
//       read of :3402-4061: none of these appear anywhere in this function's body.
//
//   Golden dead local declarations DROPPED (extract-calc-core, same convention as every other function
//   in this file, INCLUDING the specific `sTestBinNo` local this wave's hand-off named): `AnsiString
//   sTestBinNo=StringReplace(fSCKART->sInfo_TestBinNo,":","-",...)` (golden :3405 -- declared once,
//   never read again anywhere in :3402-4061, unlike SaveTestSummarySECS's OWN `sTestBinNo`, which IS
//   used in that function's FileName.sprintf(); THIS function's FileName-building never references
//   sInfo_TestBinNo at all), `map<AnsiString,AnsiString> mapIPList`/`mapIPIter`, `IP`, `tmps1`, `Data`,
//   lowercase `str2` (NOT the heavily-used `Str2`) -- verified by direct golden grep across the WHOLE
//   golden SCK_ART.cpp file (not just this function): every one of these 6 identifiers appears ONLY at
//   its own declaration line (:3404/:3408-3410) within :3402-4061, with the sole other mention being a
//   dead, already-commented-out `//    mapIPList.clear();` at :3965. UNLIKE SaveTestSummarySECS's own
//   analogous local-declaration list, `PathName` (plain, no "2" suffix) and lowercase `str3` are BOTH
//   genuinely used in THIS function (PathName: golden :3435/:3446/:3457/:3946 -- the
//   `IniConfig.iN09_4_UploadMethod!=0` fallback branch saves to `PathName+FileName` instead of
//   `PathName2+FileName`; str3: golden :4045/:4048/:4055/:4058, the final 2D-sort/SPIL-summary backup
//   copy block) -- so, unlike that sibling function, both are KEPT here, not dropped.
//
//   Golden bugs preserved VERBATIM (none "fixed" -- each cited at its own translation site too):
//     1. (golden :3472-3480 and, separately, :3492-3500) TWO occurrences of the same dead-code shape:
//        an if/else-if chain sets `sInfo_ProgramName` based on `MachineTypeChoice`
//        (HT9045/HT9046/HT9046LS), immediately followed by an UNCONDITIONAL
//        `sInfo_ProgramName="HT9046LS";` that overwrites whatever the chain just computed -- the entire
//        chain is dead code. Preserved verbatim, twice, not collapsed into a single assignment.
//     2. (golden :3636) `Str.sprintf("LOT_ID:%s", fLotInfo->edtSysLotID);` passes the TEdit* POINTER
//        itself to a `%s` conversion (missing `->Text`) -- a real golden typo/bug. In THIS translated
//        tree's vclcompat, `AnsiString::sprintf` forwards to real `vsnprintf` (vclcompat/AnsiString.cpp:
//        165-178), so this compiles (any POD pointer is a legal varargs argument) and at runtime reads
//        raw bytes STARTING AT the `TfLotInfoEdit` object's own address as if they were a NUL-terminated
//        C string -- undefined behavior, matching the SAME undefined behavior golden's real BCB6 build
//        would hit passing a `TEdit*` to `%s`. Preserved verbatim (not "fixed" to `->Text`) -- but this
//        wave's own test (PART 11) deliberately does NOT exercise this exact branch (it steers via
//        bReadLotInfoFromART==true instead, see PART 11's own comment), so as to not make an automated
//        test's outcome depend on unspecified/undefined behavior, even though ordinary (non-ASan)
//        execution of this exact line is expected to be harmless in practice (the pointer's own 8 raw
//        bytes almost always contain a NUL well before running off any mapped page, on a 64-bit build).
//     3. (golden :3656, reusing :3646's `if(fSCKART->sLotStartTime!="")` a SECOND time for the
//        SUMMARY_END_TIME block) and (golden :3662, whose ELSE-branch format string is literally
//        `"SUMMARY_START_TIME:%s%s%s%s%s%s"` again -- built from `RunInfo.LotEndTime`, not
//        `RunInfo.LotStartTime`, but mislabeled "START" not "END") -- BOTH preserved verbatim. Contrast
//        with the sibling SaveTestSummarySECS (golden :1788-1792/:1965's near-neighbor, see that
//        function's own translation above), which unconditionally writes `SUMMARY_END_TIME:%s` with NO
//        re-check and NO mislabel -- proof this is a genuine, function-specific golden quirk, not a
//        transcription error introduced by this translation.
//     4. (golden :4025-4028, this wave's hand-off-flagged quirk) `ShellExecute(...,strFileName...)`
//        fires on `IniConfig.bA38_SLT_Summary && FileName!="" && iUnloadCount>0` -- a condition
//        completely independent of whichever branch (if any) of the `bN17UploadLotSummary` block just
//        above actually ASSIGNED `strFileName`. When that block is skipped (or takes a path that never
//        reaches an assignment), `strFileName` is still its `AnsiString` default `""` here -- same
//        latent "opens with an empty path" quirk SaveTestSummarySECS's own gate #11 citation already
//        documents for its structurally identical call.
// ---------------------------------------------------------------------------
void SckArtRem_Save2DSortingSummary(SckArtRemainderState &st, int iSaveData);

// ---------------------------------------------------------------------------
// SckArtRem_SaveTestSummaryTSV -- golden TfSCKART::SaveTestSummaryTSV(int) (SCK_ART.cpp:2805-3128).
//   AI(W906-SaveTestSummaryTSV) 20260728.
//
//   Dependency verification (each checked directly against the CURRENT target tree, not assumed):
//     * fSCKART->sLotID / fSCKART->iNeedRT -- the ONLY 2 bare TfSCKART members this function reads
//       (verified by a whole-body grep for every golden TfSCKART member name); both already present on
//       SckArtRemainderState (`st.sLotID`/`st.iNeedRT`) -- no new struct field needed.
//     * fLotInfo->edtASECL_LotID/edInsertion/edFlowID -- NEW this wave (FormsFacade.h's TfLotInfo,
//       reusing the existing TfLotInfoEdit {AnsiString Text;} shape, same idiom as every other TEdit*
//       reuse in this tree).
//     * IniConfig.sN09_HandlerFolder/sN09_7_SkipIP/iN09_TSV_Port/iN09_4_UploadMethod/sN09_5_Path/
//       dN09_SearchTime/bSPILFunction -- all REAL, Config.h (confirmed via direct grep this wave;
//       bSPILFunction/dN09_SearchTime/iN09_4_UploadMethod/sN09_5_Path already used by
//       SaveTestSummarySECS above).
//     * CosFunction.bUseTSVFunction -- REAL, already used elsewhere in this file.
//     * CUSTOMER_CODE/CC_ASE_CL, USE_TRAY_MAPPING, TestIF_File.bEnableTrayID2, LastSet.iTester/ON_LINE,
//       asTravelingLogPath -- all REAL (confirmed via direct grep this wave; CC_ASE_CL MachineType.h:305,
//       USE_TRAY_MAPPING cmydef.h:3006, LastSet.iTester canary_support.h:60, asTravelingLogPath
//       common.h:100).
//     * TestSocket.iShtRow/iShtCol, Prod.iT6CatData/bIsPassBin, eTrayCount/iTestBinCount/MAX_SOCKET_ROW/
//       MAX_SOCKET_COL/SystemYear.../ChangeToPercentage/MyForceDirectories/FileExists -- all REAL,
//       already used by SaveTestSummarySECS/Save2DSortingSummary above.
//     * LotSummary.iCountCategory[][]/.iTotalCategory[]/.bIsRTBin[]/.iLoadTotal/.ClearAllData() -- gate
//       #5's existing (extended) stub -- this function both reads AND writes iCountCategory/
//       iTotalCategory (the `iNeedRT!=0 && bIsRTBin[iBin]==true` reset loop, golden :2843-2853), reads
//       `.iLoadTotal` (gate #5's 3rd extension, THIS wave -- golden :2949, `CUSTOMER_CODE==CC_ASE_CL`
//       branch), and calls ClearAllData() UNCONDITIONALLY at the end (golden :3126 -- see "Golden
//       quirk" below, this DIFFERS from the sibling functions' `if(iSaveData==1)`-gated call).
//     * fObserver->memoLotSummary->Lines=sList -- gate already established (SaveTestSummarySECS).
//     * FTP_Upload -- REUSES gate #13's existing stand-in verbatim (see gate #13's own [UPDATE] note
//       above for why this is a reuse, not a new gate).
//     * fConfiguration->mmoN04_IP->Lines->Count/->Strings[i] -- NEW gate #14 (TU-local stand-in wrapping
//       a REAL TStringList* for Lines).
//     * srvrscktTSV (->Active/->Close()/->Port=/->Open()) -- NEW gate #15 (REAL vclcompat TServerSocket,
//       default SIM mode -- not a no-op, genuinely offline-safe REAL substrate).
//     * TSVdelay/bShowTSVMsg/sTSVMsg/TimerTSV -- already-present SckArtRemainderState fields / gate #12
//       (established by SaveTestSummarySECS).
//
//   Golden quirk preserved VERBATIM #1 (golden :2887-2928): `Str` is set to `"Handler ID: %s"` (golden
//   :2891) and immediately `sList->Add(Str)`'d (golden :2892). THEN, only `if(CosFunction.
//   bUseTSVFunction)`, golden OVERWRITES `Str` with a freshly computed `"Handler IP: %s"` (or
//   `"Handler IP: "` if no IP was auto-picked) -- but the trailing `sList->Add(Str)` right after that
//   whole if-block (golden :2928) is UNCONDITIONAL. So when `bUseTSVFunction==false`, `Str` still holds
//   the STALE `"Handler ID: ..."` value from :2891, and the exact same line gets added to `sList` a
//   SECOND time (a real, faithfully-reproduced duplicate-line golden bug), instead of the intended
//   "Handler IP:" line ever appearing. Preserved verbatim, not "fixed" to always compute a real
//   "Handler IP:" line.
//
//   Golden quirk preserved VERBATIM #2 (golden :3056-3074): the local `iUnloadCount` is declared `=0`
//   at the top of this function and is NEVER incremented anywhere else in its body (verified by a
//   whole-body grep -- unlike the structurally similar SaveTestSummarySECS/Save2DSortingSummary, where
//   it accumulates from `LotSummary.iCountCategory[][]`) -- so the `IniConfig.bSPILFunction==true`
//   branch's `ChangeToPercentage(LotSummary.iTotalCategory[iBin], iUnloadCount)` call (golden :3024)
//   always divides by a hardcoded 0. `ChangeToPercentage` is div-by-zero-safe (see MachineType.h:1585),
//   so this is a silent "always reads 0.00%"-shaped quirk, not a crash. Preserved verbatim.
//
//   Golden quirk preserved VERBATIM #3 (golden :3049-3078, a genuine ASYMMETRY between two near-
//   identical save blocks, NOT a transcription slip introduced by this translation -- re-verified
//   directly against golden twice): the FIRST `if(fSCKART->iNeedRT==0){ if(iN09_4_UploadMethod==0){...}
//   else{ sList->SaveToFile(IniConfig.sN09_HandlerFolder+"\\"+FileName); } }` block's non-FTP else-arm
//   saves to `sN09_HandlerFolder+"\\"+FileName`. The SECOND, near-identical block just below it (nested
//   inside `if(FileExists(PathName2+FileName)==false)`) has the SAME shape but its non-FTP else-arm
//   instead saves to `PathName+FileName` (note: plain `PathName`, the TSV-branch-only variable computed
//   at golden :2823/:2827 -- NOT `sN09_HandlerFolder` again). Preserved verbatim, not unified.
//
//   Golden dead local declarations DROPPED (extract-calc-core, same convention as every other function
//   in this file): `tmps1`, `Data=""` -- verified by whole-body grep: both identifiers appear ONLY at
//   their own declaration line (golden :2807/:2812), never read or written again anywhere in
//   golden :2805-3128. UNLIKE SaveTestSummarySECS/Save2DSortingSummary's own dead-locals list, `IP`/
//   `mapIPList`/`mapIPIter` are all GENUINELY USED here (the "Handler IP:" auto-pick block, golden
//   :2894-2925) and are NOT dropped.
// ---------------------------------------------------------------------------
void SckArtRem_SaveTestSummaryTSV(SckArtRemainderState &st, int iSaveData);

// ---------------------------------------------------------------------------
// SckArtRem_SaveSummaryTrayFeed -- golden TfSCKART::SaveSummaryTrayFeed() (SCK_ART.cpp:3129-3401).
//   AI(W906-SaveSummaryTrayFeed) 20260728.
//
//   Dependency verification (each checked directly against the CURRENT target tree, not assumed):
//     * `slExe` -- the ONLY bare TfSCKART member this function reads/writes (verified by a whole-body
//       grep for every golden TfSCKART member name). Golden's `TfSCKART::slExe` (SCK_ART.h:244) is a
//       persistent class member (ctor :39 / dtor :171-172), but its ONLY reader/writer anywhere in the
//       entire golden codebase is THIS one function (verified by grep across all of golden SCK_ART.cpp),
//       and every code path that touches it both fully populates it (Add) and fully clears it (Clear())
//       before the function returns -- no cross-call state ever survives. Modeled as a FUNCTION-LOCAL
//       `TStringList*` (same local-heap-list idiom this file already uses for sList/sListTotal/
//       sListPass/sListFail in the sibling functions above) instead of a new persistent
//       SckArtRemainderState field -- avoids an unnecessary owning-pointer lifetime dependency on the
//       state struct for a value that never survives past one call. A DELIBERATE, documented judgment
//       call, not an oversight.
//     * TastCategory (.UpdataCount()/.iTotalSocket/.iPassSocket/.iFailSocket/.iRejectCount/
//       .iUnloadCnt[]/.iBySiteTotal[]/.iBySitePass[]/.iBySiteFail[]/.iBySiteCate[][]/.iTotalCategory[])
//       -- NEW gate #16 (TU-local stand-in; golden's real UpdataCount depends on ArmDataLot/TArm, a
//       whole separate untranslated class hierarchy).
//     * RunInfo.LotStartTime/LotEndTime/LotNo/iUnloadCount/SaveJamRateByLot() -- LotStartTime/LotEndTime
//       already REAL (used by SaveTestSummarySECS above); LotNo confirmed REAL (cprod.cpp:172, the
//       UNGATED RUN_INFO::InitialData()); SaveJamRateByLot() is gate #8's 4th member (NEW this wave --
//       see gate #8's [UPDATE] note above).
//     * fLotInfo->lbledtCustomer (NEW this wave, FormsFacade.h) / edtSysOperatorID (already REAL, used
//       by Save2DSortingSummary above) -- both only ->Text read.
//     * fMain->cbSetupFileName->Text -- REAL, already on TfMain (golden main.h:875).
//     * IniConfig.SocketHandlerID/bVTESTFunction/bN10_UploadSummaryToFTP/iN10UploadMethod/
//       sN10UploadDrivePath -- all REAL, Config.h (confirmed via direct grep this wave;
//       SocketHandlerID already used by Save2DSortingSummary above).
//     * TestSocket.iShtRow/iShtCol/iMaxRow/iMaxCol -- iShtRow/iShtCol already REAL (used elsewhere in
//       this file); iMaxRow/iMaxCol confirmed REAL, aHotPlateSubstrate.h:180/205 (TMyKitSuck).
//     * TestIF.iSiteMap[][] -- REAL, cprod.h:2576 (`extern SYSTEM_TEST_IF TestIF;`, ungated global
//       instance at cprod.cpp:30) -- a DIFFERENT global from `TestIF_File` (also real, used elsewhere).
//     * Prod.iTrayType[]/iT6PosCate[]/iIfErrorT6/bIsPassBin[]/iT6CatData[] -- REAL, already used by
//       SaveTestSummarySECS/Save2DSortingSummary above.
//     * s6TrayName[eTrayCount] -- REAL, cmydef.h:138 (a DIFFERENT global from `s6ShortTrayName[]`,
//       already used elsewhere in this file for the sibling functions' own unloader-count blocks).
//     * eTrayCount/iTestBinCount/ChangeToPercentage/MyForceDirectories/FileExists/DirectoryExists/
//       ExtractFilePath/ExtractFileName/CopyFile/RecordProcess/ShowMyMessage/GetTimeInfo/SystemYear... --
//       all REAL, already used by sibling functions above (ExtractFilePath/ExtractFileName confirmed
//       REAL via vclcompat/SysUtils.h, newly needed BY NAME in this function but already transitively
//       available).
//     * IncludeTrailingPathDelimiter -- REAL underlying substrate (`IncludeTrailingBackslash`,
//       vclcompat/SysUtils.h), but the BCB6-spelling SYNONYM itself is, per this project's established
//       convention (cpublic.cpp:136, Interface/TesterTCP.cpp:98, SECSGEM/uHGemClass.cpp:698 all do the
//       SAME thing independently), only ever provided as a small per-TU `static inline` forwarding
//       function -- NOT centralized in vclcompat itself yet. This file adds its OWN copy, same as every
//       other TU that has needed it so far.
//     * MyDBIProcess(AnsiString,AnsiString) -- REAL, aHotPlateSubstrate.h:701/.cpp:772 (a no-op),
//       already `#include`'d transitively via this file's existing aHotPlateSubstrate.h include.
//     * ExecZipCommand(AnsiString,AnsiString) -- REAL, cpublic.h:47/.cpp:771 (confirmed via direct grep
//       this wave; NOT previously used by this file, but already linkable tree-wide).
//     * FormHS->UpDataToServerByFTP(...) / slEventLog->sLotFileName -- NEW gate #17 (TU-local stand-ins;
//       see gate #17's full doc comment above for why the real `slEventLog` global cannot be reused
//       directly).
//
//   Golden dead local declarations: NONE dropped -- every local this function actually declares
//   (iSiteCh, iTotalCh, iSiteTotalCt, iSitePassCt, iSiteFailCt, sPassYield, sFailYield, asFileName,
//   asFolderName, sTemp, str, s, sTotal, sTempT, sTempP, sTempF, sP, sF, redtSummary, sListTotal,
//   sListPass, sListFail) is genuinely read after being written (verified by direct golden read of
//   the full :3129-3401 body, not a mechanical dead-local scan this time -- this function has no
//   unused-declaration pattern like its 2 siblings above).
// ---------------------------------------------------------------------------
void SckArtRem_SaveSummaryTrayFeed(SckArtRemainderState &st);

#endif // AUTOMATION_SCK_ART_REMAINDER_CORE_H
