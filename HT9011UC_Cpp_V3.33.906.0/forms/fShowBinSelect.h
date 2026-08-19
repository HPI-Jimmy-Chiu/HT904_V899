// =============================================================================
//  forms/fShowBinSelect.h  --  non-VCL facade for golden's TfShowBinSelect
//                               (cShowBinSelect.h)
//
//  AI(W906-FW3-ShowBinSelect-WA) 20260818: new file, FW-3 queue item 2.
//  Golden ref: HT9011UC_Code_V3.33.906.0_20260618/cShowBinSelect.h (524
//  lines) + cShowBinSelect.cpp (3,016 lines, cp950/Big5 -- decoded with
//  `python3 -c "open(path,'rb').read().decode('cp950')"`, 0 U+FFFD, this
//  wave).
//
//  AI(W906-FW-SBWC) 20260819: FW-3 Wave C -- lands ShowBinSel_ARTNor/
//  ShowBinSel_ARTRT/TimerAutoCleanCountTimer/FormShow real bodies (WAVE B
//  QUEUE items). Re-decoded golden this wave with the same cp950 command,
//  0 U+FFFD, per-line-range (`text.split('\n')`, 1-based golden line ==
//  `lines[N-1]`) -- see WAVE C SCOPE below for exact spans. STYLE NOTE: the
//  prior FW-SBWB2 wave translated Chinese attribution comments into English
//  prose and reflowed whitespace; DEVLOG 20260819 flagged this as a
//  deviation from cp950-verbatim policy ("風格警示：本波代理偏離逐字翻譯"). This
//  wave follows FW-BinSel-WB's precedent instead: original Chinese
//  attribution comments (`//kevin 20180705 有BIN TRAY set true` etc) are
//  kept BYTE-VERBATIM (only `__fastcall` removed), not translated/reflowed.
//
//  ROLE
//  ----
//  TfShowBinSelect is golden's Unloader Bin-Select / Category-Info dialog:
//  per-Auto/Fix/Magazine bin assignment display, the ART Category grids, the
//  UPH history grid, and the physical bin-display-hardware refresh loop.
//  Its `ShowCategoryBin()`/`UPH_StringGrid`/`labArmDiff`/`labSiteDiff`/
//  `labLowYield`/`labTotalYield`/`labTotalYieldTotal`/`lblSpeciallYield`/
//  `lblSpeciallYieldTotal`/`IntervalByTotal`/`iLowYieldBinSelectContactCount`
//  are 14 of fYieldMonitoring's 39 GATE touches (forms/fYieldMonitoring.h
//  GATE REGISTER (Y2)) -- unblocking THOSE is this wave's primary purpose.
//
//  WAVE A / WAVE B SPLIT -- WHY THIS FILE DOES NOT COVER THE WHOLE GOLDEN CLASS
//  --------------------------------------------------------------------------
//  Golden's class carries ~300 individually-named widget members (7 parallel
//  32-slot label arrays for Auto/Fix/Magazine bin display alone, plus every
//  ART-FT/ART-RT/AI-CCD variant) and several methods are irreducibly blocked
//  by an OPAQUE hardware pointer (`database.h:300 TMyBinDispCtrl *BinDisCtrl;
//  // opaque; NULL until UI wave wires InstallColorBinDisplay` -- confirmed
//  this wave, `grep -n BinDisCtrl database.h`). Per the FW campaign's own
//  "大表單切塊" (slice big forms) policy (fw-wave-loop skill S6), this wave
//  (WAVE A) translates the subset that (a) satisfies the fYieldMonitoring
//  GATE REGISTER and (b) is reachable without the opaque hardware pointer or
//  the ~250-widget ctor-population surface; the remainder is WAVE B,
//  explicitly queued below, NOT silently dropped.
//
//  WAVE A SCOPE (golden line spans, this file's methods)
//  --------------------------------------------------------------------------
//    TfShowBinSelect()      golden :44-161   DEVIATION -- bootstrap ctor,
//                            see CTOR NOTE below (NOT a verbatim translation
//                            of golden's ctor body)
//    FormDestroy            golden :163-176  ACTIVE
//    FormClose              golden :868-873  ACTIVE
//    InitShowBinDigital     golden :874-878  ACTIVE
//    SetLabelVisible        golden :1425-1434 ACTIVE
//    SetAutoVisible         golden :1436-1477 ACTIVE
//    ShowInitialString      golden :1634-1700 ACTIVE
//    ShowCategoryBin        golden :1701-2052 ACTIVE, 3 GATEs (fCounterClear)
//    UPH_StringGridDblClick golden :2054-2099 ACTIVE, 1 GATE (Application->MessageBoxA)
//    CaculateUPH            golden :2279-2333 ACTIVE, 1 GATE (fMain->StatusBar1)
//    edSLT01Change          golden :2334-2337 ACTIVE, 1 GATE (fMain->StatusBar1)
//    btnSetInpputCntClick   golden :2339-2352 ACTIVE
//    RefreshAiCnt           golden :2354-2364 ACTIVE, 1 GATE (fFixAICCD->UnloadAICntNG[])
//    ed_AutoCleanCountClick golden :2216-2223 ACTIVE, 2 GATEs (fSecurity, fCleaning)
//    labAuto1Click          golden :2225-2242 ACTIVE, 1 GATE (fBinSel->chkShow0Xbin)
//    btReturnClick          golden :2244-2251 ACTIVE, 1 GATE (PageControl1Change -- Wave B)
//    btnCleanResetClick     golden :2253-2260 ACTIVE, 2 GATEs (fSecurity, fCleaning)
//    btnClearCountClick     golden :2262-2277 ACTIVE, 2 GATEs (fSecurity, fCounterClear)
//    DelDot (free function) golden :178-205  ACTIVE (pure)
//
//  WAVE B SCOPE (AI(W906-FW-SBWB2) 20260819 -- golden line spans)
//  --------------------------------------------------------------------------
//    ShowBinSel             golden :388-756  ACTIVE, 3 GATEs -- (B9)
//                             fSortCT->myCountPanel, (B10)/(B11)
//                             ShowBinSel_ARTNor/ShowBinSel_ARTRT DISSOLVED
//                             by WAVE C (see below); 1 GOLDEN BUG (B12).
//
//  WAVE C SCOPE (this wave, AI(W906-FW-SBWC) 20260819 -- golden line spans)
//  --------------------------------------------------------------------------
//    ShowBinSel_ARTNor      golden :2365-2580 ACTIVE, 0 GATEs. GATE (B10)
//                             DISSOLVED -- widget surface (tsARTNormalBin,
//                             palARTNor, MyBinSelARTFT[] retyped
//                             TfShowBinSelectLabel*) all real now.
//    ShowBinSel_ARTRT       golden :2581-2799 ACTIVE, 0 GATEs. GATE (B11)
//                             DISSOLVED, same shape as ShowBinSel_ARTNor
//                             (tsARTRTBin, palARTRT, MyBinSelARTRT[]).
//    TimerAutoCleanCountTimer golden :2168-2214 ACTIVE, 0 GATEs (10 new
//                             widgets: rg_FixBinBox, ed_FixBinBoxAlarmCount,
//                             LabErrorBinNowCount, PageControl1_ART,
//                             Tab_ARTSkipICCount, AutocleanlifeTime,
//                             btnCleanReset, BulkBox, labScheduleNAME,
//                             labInQty). The golden :2207-2213 `CC_JCET`
//                             block is commented out IN GOLDEN ITSELF (dead,
//                             not a translation gap) -- kept as a comment.
//    FormShow               golden :758-866 ACTIVE, 1 GATE (B18) -- see
//                             GATE REGISTER. `labAuto1/2/3`/`labFix1/2/3`
//                             (golden :779-784, CC_Greatek branch) are NOT
//                             new facade members -- see CTOR NOTE below,
//                             which already proved these ALIAS
//                             MyBinSelLab[eAuto1..eAuto3]/[eFix1..eFix3]
//                             (golden ctor :56-63/:124, `tempMyBinSelLab[]=
//                             {labAuto1,labAuto2,labAuto3,...,labFix1,
//                             labFix2,labFix3,...}` assigned verbatim into
//                             `MyBinSelLab[i]`) -- translated as
//                             `MyBinSelLab[eAuto1]->Caption=...` etc, same
//                             object identity as golden, zero new members.
//                             `PageControl1->ActivePage` (golden :800) is
//                             new: PageControl1 retyped
//                             TfShowBinSelectPageControl* (adds ->ActivePage,
//                             a TTabSheet*; vclcompat::TPageControl only
//                             carries ->ActivePageIndex).
//
//  WAVE B QUEUE (explicit, NOT translated -- golden line spans; ShowBinSel_
//  ARTNor/ShowBinSel_ARTRT/TimerAutoCleanCountTimer/FormShow MOVED to WAVE C
//  above, closed)
//  --------------------------------------------------------------------------
//    PageControl1Change       golden :1479-1633 (page-layout geometry, many
//                              widgets) -- DECLARED here as a documented
//                              no-op GAP so btReturnClick's call site has
//                              something to bind to (see GATE REGISTER below)
//    ChangeBinDispStatus      golden :208-386   BLOCKED: >90% of its body is
//                              `HSys.BinDisCtrl->...` (opaque, always NULL)
//    ShowBinDigital           golden :880-996   physical 7-segment display
//                              driver (SW[] switch coupling + fiosetview)
//    DoShowBinDigital         golden :998-1423  BLOCKED: dominant body is
//                              `HSys.BinDisCtrl->...`
//  SAFETY-QUEUED (per project policy -- machine-mode-switching actions,
//  queued for user sign-off, NOT translated as a "widget surface" deferral):
//    btnAutoCleanClick             golden :2101-2166 (starts a REAL Auto
//                                   Clean sequence -- bRunAutoClean=true,
//                                   motor-position preconditions)
//    btnAutoDeviceEjectionRemoveClick golden :2852-2893 (physical tray
//                                   ejection: InitNewTray/bEject/bNeedEject)
//    btnAutoDeviceEjectionClick    golden :2896-3016 (switches LastSet.iTester
//                                   to OFF_LINE, rewrites Bin-config files,
//                                   purges devices to AUTO1)
//    EdLoadCountClick              golden :2800-2803 (LOW risk on its own --
//                                   pops a virtual keyboard -- but queued
//                                   alongside its sibling clicks rather than
//                                   cherry-picked, per this wave's brief
//                                   scope)
//    btnICMisPlacementClick        golden :2805-2843 (manually injects a real
//                                   JAM alarm -- ShowErrorMessage("JAM11/12/
//                                   1310", ...))
//    sbCopyRecipeClick             golden :2845-2850 (fMain->RunBatchCopyRecipe,
//                                   a setup-file mutation entry point)
//
//  CTOR NOTE -- DEVIATION: bootstrap ctor, not a verbatim golden translation
//  --------------------------------------------------------------------------
//  Golden's ctor's ENTIRE job (cShowBinSelect.cpp :47-133) is copying ~190
//  individually-named `.dfm` widget pointers (laAuto1, laFix1, ... 33 of
//  each) into 7 parallel arrays (MyBinSel/MyBinSelLab/MyBinSelARTFT/
//  MyBinSelARTFTLab/MyBinSelARTRT/MyBinSelARTRTLab/grpBinDisp/EditAi) plus a
//  handful of scalar inits (ColorRed/Green/Orange, tsUPH, sBinCode_ATK[],
//  ShowInitialString(), an SPIL_FOR_QLE branch, btReturn->Align).  VERIFIED
//  this wave (`grep -n "<name>" <utf8-decoded-cpp>` for a sample of the ~190
//  names): apart from `labAuto1/2/3`/`labFix1/2/3` (read again by FormShow,
//  golden :779-784, CC_Greatek customer code only -- translated by
//  AI(W906-FW-SBWC) 20260819 as `MyBinSelLab[eAuto1..eAuto3/eFix1..eFix3]`,
//  see WAVE C SCOPE above; NOT new facade members) and each
//  name's own `<name>Click` event-handler declaration, NONE of the ~190
//  individually-named widgets is ever read anywhere outside the ctor -- only
//  the 7 ARRAYS are. This facade therefore constructs the 7 arrays directly
//  (one heap object per slot, in a loop) rather than declaring ~190 facade
//  members whose only reader would be this same ctor. This is a BEHAVIOUR-
//  PRESERVING end-state (each `MyBinSel[i]` etc. IS a live, independently
//  mutable object, exactly as golden's ctor leaves it), not a scope
//  reduction -- see forms/fObserver.h/forms/fYieldMonitoring.h's own "NO
//  CONSTRUCTOR THIS WAVE" precedent for the general shape of this class of
//  decision; here a (non-verbatim) constructor is written specifically
//  because SetLabelVisible/SetAutoVisible/ShowCategoryBin (all THIS wave's
//  scope) actively read those arrays, so NSDMI defaults alone (arrays of
//  null pointers) would crash the first `MyBinSel[i]->Caption=...`.
//
//  Two genuinely PORTED ctor statements: ShowInitialString() is called
//  (translated this wave, see below) and tsUPH is populated with 20 blank
//  entries (verbatim). The SPIL_FOR_QLE `palAutoDeviceEjection` branch and
//  `btReturn->Align=alBottom` are cosmetic-only (see GATE REGISTER) and are
//  each represented as a documented no-op rather than silently dropped.
//
//  STATIC-INIT SAFETY (for the eventual main-loop homecoming, NOT decided
//  this wave -- global NOT defined here, see banner tail)
//  --------------------------------------------------------------------------
//  This ctor calls ShowInitialString(), which calls ShowCategoryBin(), which
//  reads Prod/LastSet/CosFunction/IniConfig/ArmData[]/TestSocket (all
//  plain-old extern globals with static storage duration and no dynamic
//  initializer of their own) and fContact->fShow (atester_shims.h's
//  TfContactShim -- also a plain global, no config/file I/O in ITS ctor
//  either). Unlike cObserver's ctor (which reads INI files directly), this
//  ctor touches NO file/registry I/O and NO other class's runtime state --
//  so, UNLIKE cObserver, an `INIFileGeneral!=0`-style guard is likely NOT
//  needed. This is flagged for the main loop to VERIFY (not asserted as
//  proven-safe by this wave) before defining a live global from this class,
//  per this wave's task brief.
//
//  STALE as of 20260819 (FW-SBWB2/FW-SBWC, not fixed here since it predates
//  this wave and the correction belongs beside the code it describes): the
//  global WAS defined by FW-SBWB2 (`cShowBinSelect.cpp:125, TfShowBinSelect
//  *fShowBinSelect = new TfShowBinSelect();`) with its own SIOF guard
//  (`INIFileGeneral!=0`, see that file's banner ahead of the ctor) -- this
//  section's "NOT decided this wave" framing is historical record of WAVE A's
//  own reasoning, not the current state.
//
//  GATE REGISTER
//  --------------------------------------------------------------------------
//  (B1) ShowCategoryBin's three `fCounterClear->ClearCount(ctBinCount)`
//       calls (golden :1984/:2000/:2041) -- fCounterClear has NO facade
//       anywhere in the tree (`grep -rn "fCounterClear" --include=*.h .` --
//       only ONE hit, Automation/SCK_ART_Remainder.h:71, itself a comment
//       recording the same gap: "fCounterClear->ClearCount(), the last
//       needing an untranslated cCounterClear.cpp form"). Every OTHER
//       statement in each of the three surrounding `if` blocks (the Low-
//       Yield-alarm threshold checks, DoLowYieldAlarm/InitialAutoCleanAllTask
//       calls, fYieldMonitoring->ClearYieldCount()) is REAL and ACTIVE --
//       only the counter-clear call itself is gated, same "gate the missing
//       call, not the surrounding logic" idiom as forms/fContactCT.h's
//       GATE (C1)/fObserver.h's GATE (2).
//  (B2) UPH_StringGridDblClick's `Application->MessageBoxA(...)==IDOK`
//       guard (golden :2063) -- same absent-TApplication gap as forms/
//       fContactCT.h GATE (C2)/database.cpp's GA1-B6 precedent. Fail-closed:
//       the destructive row-delete loop does not run without a real confirm
//       dialog.
//  (B3) CaculateUPH's / edSLT01Change's `fMain->StatusBar1->Panels->
//       Items[N]->Text=...` (golden :2315/:2319/:2336) -- forms/fMain.h has
//       NO `StatusBar1` member (`grep -n "StatusBar1" forms/fMain.h` -- 0
//       hits, 20260818). The UPH NUMBER ITSELF (iNetUPH/iGrossUPH/
//       iRecordEventLogUPH -- all real, already-ported globals) is computed
//       and stored regardless; only the on-screen status-bar text write is
//       gated. edSLT01Change's `strcpy(LastSet.szJamClearData[0], ...)`
//       READS the same missing StatusBar1 as its RHS, so that one statement
//       is gated as a whole (no independently computable value, same
//       posture as forms/fYieldMonitoring.h GATE (Y3)).
//  (B4) RefreshAiCnt's `fFixAICCD->UnloadAICntNG[i]` (golden :2359) --
//       forms/fFixAICCD.h exists but carries no `UnloadAICntNG` member
//       (`grep -n "UnloadAICntNG" forms/fFixAICCD.h` -- 0 hits, 20260818).
//       Safe default: blank text (same as the existing `tNotUse` else-arm),
//       i.e. "no AI-CCD count available" rather than fabricating a number.
//  (B5) ed_AutoCleanCountClick / btnCleanResetClick's `fSecurity->
//       Insufficient(43)` / `Insufficient(97)` (golden :2218/:2255) --
//       same fSecurity absence as forms/fContactCT.h GATE (C3); same
//       fail-closed substitute (forced `false`, i.e. permission NOT
//       verified) -- both methods' bodies become unreachable until a real
//       fSecurity facade lands. `fCleaning->btnResetCleanCountClick(Owner)`
//       (golden :2220/:2257) is ALSO gated (forms/fCleaning.h has no such
//       member, `grep -n "btnResetCleanCountClick" forms/fCleaning.h` -- 0
//       hits) -- moot while the Insufficient() gate is fail-closed, recorded
//       so it is not missed when (B5)'s outer gate is lifted.
//  (B6) btnClearCountClick's `fSecurity->Insufficient(108)==false` (golden
//       :2266) -- same gap/substitute as (B5); `ShowMyMessageBox_YES_NO`
//       (golden :2270) is a SEPARATE gap (Automation/AGV_E84.h:58's own
//       comment already records it: "ShowMyMessageBox_YES_NO (mymessbox.h,
//       W7-UI modal dialog, untranslated)") -- fail-closed substitute: `ret`
//       forced to 2 ("No"/cancel), so `fCounterClear->ClearCount(
//       ctIndexCount)` (golden :2276, ALSO independently gated -- see (B1))
//       never runs either way.
//  (B7) labAuto1Click's `fBinSel->chkShow0Xbin->Checked` (golden :2237) --
//       fBinSel has NO facade anywhere (`grep -rn "fBinSel" --include=*.h .`
//       -- only comment-only hits in forms/fMain.h, 20260818). Safe default:
//       `false` (an unchecked checkbox is Delphi's own real-VCL default, and
//       is also this tree's established "default false" convention for
//       boolean widget state -- see forms/FormWidgets.h's own DEFAULT-VALUE
//       RULE banner), so the `else` arm (`Width=331`) is taken.
//       STALE as of 20260819 (FW-SBWB2, not fixed here since labAuto1Click
//       itself is Wave A/out of this wave's scope): fBinSel is now REAL
//       (forms/fBinSel.h, `class TfBinSel` with a real `chkShow0Xbin`
//       member) -- ShowBinSel() (this wave, below) uses it unguarded. This
//       (B7) gate on labAuto1Click could be dissolved by a future pass; not
//       done here to keep this wave's diff to its own target.
//  (B8) btReturnClick's `PageControl1Change(this)` call (golden :2250) --
//       PageControl1Change itself is WAVE B (golden :1479-1633, ~154 lines,
//       many more widgets). Declared here with a documented no-op body
//       (cShowBinSelect.cpp) so the call site compiles and links; every
//       OTHER statement in btReturnClick (the two Left/Top assignments) is
//       real and ACTIVE.
//  (B9) ShowBinSel's `fSortCT->myCountPanel[i].pnlYield->Font->Color` /
//       `.pnlCount->Font->Color` (golden :640/:642 grey arm, :648/:650
//       colour arm) -- fSortCT itself is real (forms/fSortCT.h), but that
//       facade declares no `myCountPanel` member (`grep -n "myCountPanel"
//       forms/fSortCT.h` -- 0 hits, 20260819; golden's own member is
//       `_MyCountPanel myCountPanel[eTrayCount]`, cSortCT.h:335). Gated
//       exactly those 2 lines per arm (4 total); the other 3 statements in
//       each arm (UnLoadPanel[i]->Color, UnLoadLabel[i]->Font->Color,
//       MyBinSel[i]->Font->Color) are real and ACTIVE -- same "gate the
//       missing call, not the surrounding logic" idiom as (B1)/(B3).
//  (B10) ShowBinSel's `ShowBinSel_ARTNor()` call (golden :752) --
//        ShowBinSel_ARTNor's real body is golden :2365-2580 (~216 lines,
//        guarded by `IniConfig.bSPILFunction && bCanRunSCKART`, both default
//        false), explicitly left WAVE B QUEUE (see above) rather than
//        translated this wave -- declared with a documented no-op body
//        (cShowBinSelect.cpp) so ShowBinSel's call site compiles/links, same
//        posture as (B8)/PageControl1Change.
//  (B11) ShowBinSel's `ShowBinSel_ARTRT()` call (golden :753) -- same
//        reasoning/posture as (B10), real body golden :2581-2799.
//  (B12) GOLDEN BUG, ShowBinSel (golden :541-545): `bUnloadHasBin[Data]=true`
//        (golden :543, kevin 20180705) is immediately overwritten to `false`
//        two lines later (golden :545, kevin 20220906) with NO intervening
//        read of the variable -- for the "Data>0, non-BulkBox error-bin
//        reroute" arm, `bUnloadHasBin[Data]` therefore ALWAYS ends up
//        `false` and golden's own `=true` statement is dead code. Kept
//        verbatim (translation-fidelity policy: "看到 golden 裡不合理的邏輯：
//        照翻，並在 //AI 註解寫下它為什麼看起來錯") -- NOT "fixed" to `=true`,
//        since that would change observable behaviour (a real downstream
//        reader of bUnloadHasBin[] would then see this tray as "has a bin"
//        after an error-bin reroute) without user sign-off. Translated at
//        its own line in cShowBinSelect.cpp with a matching comment. A
//        SECOND, smaller golden oddity in the same function (golden
//        :602-609, Ifor 20231122 Magazine-Link inner loop) is documented
//        in-line at its own site rather than separately numbered here: a
//        pathological all-true `bMagazineLink[]` chain down to index 0 would
//        walk `bMagazineLink[i-j]` to a negative subscript -- unreachable in
//        practice (index 0/eAuto1 is never written true by any port'd
//        writer), kept verbatim.
//  (B13) GOLDEN ODDITY (not a gate -- dead branch in golden itself), lives
//        inline at ShowBinSel's own Pos()-check site in cShowBinSelect.cpp
//        (integration 20260819). ShowBinSel_ARTNor/ShowBinSel_ARTRT (this
//        wave, FW-SBWC) each carry the IDENTICAL 87-char-needle Pos() check
//        (golden :2543/:2762) -- same dead branch, same reasoning, cross-
//        referenced at each site rather than re-derived.
//  (B18) FormShow's CC_GIGAS/SPIL_FOR_QLE inner Top-stacking calculation
//        (golden :855-858: `palAutoDeviceEjection->Top=btnASM->Top+
//        btnASM->Height+10;` / `...=gbAutoCleanCount->Top+gbAutoCleanCount->
//        Height+10;`) -- `btnASM`/`gbAutoCleanCount` are NOT on this facade
//        (`grep -n "btnASM\|gbAutoCleanCount" forms/fShowBinSelect.h` -- 0
//        hits before this wave, 20260819) and neither vclcompat::TSpeedButton
//        nor vclcompat::TGroupBox carries `->Top`/`->Height` (pure cosmetic
//        pixel-stacking geometry, never read back by anything this tree
//        tests) -- gated rather than growing two more facade-only geometry
//        subclasses for a calculation with zero behavioural effect. The
//        OUTER `palAutoDeviceEjection->Visible=true/false` toggle (golden
//        :848-849/:863-864) and the `if(palAutoDeviceEjection && ...)` guard
//        are REAL/ACTIVE -- only the inner `->Top=` assignment is gated, same
//        "gate the missing call, not the surrounding logic" idiom as (B1)/
//        (B3)/(B9).
//
//  DESIGN NOTE -- facade-only widget wrapper shapes
//  --------------------------------------------------------------------------
//  TfShowBinSelectGrid : public vclcompat::TStringGrid
//    Adds a `Selection` sub-object with one field, `.Top` (int), for
//    UPH_StringGrid->Selection.Top (UPH_StringGridDblClick) -- vclcompat::
//    TStringGrid deliberately carries no selection-state surface (its own
//    SCOPE note). Offline default 0 (no cell selected), same "cosmetic/
//    interaction-state, offline inert" posture as every other facade-only
//    extension in this tree.
//  TfShowBinSelectPanel : public vclcompat::TPanel
//    Adds `Top` (int) for palAutoDeviceEjection->Top (ctor's SPIL_FOR_QLE
//    branch) -- vclcompat::TPanel carries no geometry. AI(W906-FW-SBWC)
//    20260819, WAVE C ADD: also adds `Left`/`Width` (both int, default 0) --
//    reused (not a new subclass) for `palUnloader->Width` (FormShow, CC_
//    Greatek) and `PLoadInput->Top`/`->Left` (FormShow, bG15LoadInputCount).
//  TfShowBinSelectPageControl : public vclcompat::TPageControl -- AI(W906-
//    FW-SBWC) 20260819, WAVE C ADD.
//    Adds `ActivePage` (a `TTabSheet*`, default nullptr) for
//    `PageControl1->ActivePage=tsTestBin;` (FormShow, golden :800) --
//    vclcompat::TPageControl carries only `->ActivePageIndex` (int), no
//    tab-sheet pointer surface. PageControl1 retyped from plain
//    `TPageControl*` to this subclass; safe for every existing reader (the
//    only prior touch anywhere in this file is the ctor's implicit default-
//    construction, `grep -n "PageControl1" cShowBinSelect.cpp` before this
//    wave -- 0 hits besides the member declaration).
//  TfShowBinSelectTimer
//    golden `TTimer *TimerAutoCleanCount;` -- ONLY `->Enabled` is ever
//    written (FormDestroy, golden :167) among this wave's scope; no
//    vclcompat TTimer stand-in exists anywhere in the tree (`grep -rn
//    "class TTimer" vclcompat/` -- 0 hits, 20260818), so a minimal
//    facade-only stand-in is declared here (same "PART 2 -- DEFERRED
//    STAND-INS" posture as forms/FormWidgets.h's own bespoke types for
//    golden classes vclcompat/Controls.h does not carry).
//  TfShowBinSelectLabel : public vclcompat::TLabel -- AI(W906-FW-SBWB2)
//    20260819, WAVE B ADD.
//    Adds a `TFont *Font` member. vclcompat::TLabel (vclcompat/Controls.h)
//    deliberately carries no Font surface at all (that header's own SCOPE
//    note: "Only ... LabelPtr->Caption ... is touched" by every prior
//    consumer) -- but ShowBinSel (golden :638-650) is the first golden
//    method this tree translates that reads/writes `->Font->Color` on a
//    TLabel-shaped widget (`UnLoadLabel[i]->Font->Color=...` /
//    `MyBinSel[i]->Font->Color=...`). Verified this wave (`grep -n "class
//    TLabel" vclcompat/Controls.h`, 20260819) that adding a `Font` field to
//    the real vclcompat::TLabel is out of this wave's write boundary (only
//    cShowBinSelect.cpp/forms/fShowBinSelect.h/tests/
//    test_showbinselect_core.cpp are writable) -- same "facade-only
//    subclass, not an upstream vclcompat change" shape as
//    TfShowBinSelectGrid/TfShowBinSelectPanel above. `MyBinSel[e3TrayCount]`
//    (Wave A, previously `TLabel*`) and the new `UnLoadLabel[e3TrayCount]`
//    are BOTH retyped to `TfShowBinSelectLabel*` -- safe for every existing
//    Wave A reader (SetAutoVisible/SetLabelVisible/ShowCategoryBin/tests)
//    since TfShowBinSelectLabel IS-A TLabel and every one of their reads is
//    through the TLabel base (->Caption/->Visible), never through a `TLabel*`
//    variable that would need the reverse (derived-to-base is implicit,
//    base-to-derived is not, and nothing does the latter). Font->Color
//    defaults 0 (TFont's own ctor), i.e. black, matching real VCL's TLabel
//    default font colour (clWindowText, which is black on the tree's own
//    stock colour scheme).
//
//  HYDRATION (dfm IR: tools/dfm2rc/ir_out/cShowBinSelect.dfm.ir.json, this
//  wave)
//  --------------------------------------------------------------------------
//    StrGrdCategory/StrGrdCategoryART/StrGrdCategoryContCT/StrARTSkipICCount:
//      ColCount=3, RowCount=16 (dfm design-time default -- ShowInitialString/
//      ShowCategoryBin overwrite both at runtime, exactly as golden does).
//    UPH_StringGrid: ColCount=4, RowCount=14 (dfm design-time default;
//      CORRECTED by AI(W906-FW-SBWC) 20260819 -- the `IniConfig.bVTESTFunction`
//      ColCount=7 override actually lives in ShowBinSel (golden :397-400,
//      landed FW-SBWB2), NOT FormShow as this note previously claimed;
//      FormShow itself never touches UPH_StringGrid. Both ShowBinSel and
//      FormShow are ACTIVE as of this wave, so the override IS reachable
//      (FormShow calls ShowBinSel() -- golden :798/:2365-2799 region).
//    tsARTNormalBin/tsARTRTBin/palARTNor/palARTRT/PageControl1_ART/
//      Tab_ARTSkipICCount/tsFxiAI/tsSECS_Category/tsTestBin/Tab_UPH/tsASE/
//      tsCategoryInfoContCT/BulkBox/rg_FixBinBox/ed_FixBinBoxAlarmCount/
//      LabErrorBinNowCount/AutocleanlifeTime/btnCleanReset/labScheduleNAME/
//      labInQty/tsUnloadMap/btnClearCount/palUnloader/btnAutoClean/
//      labJamrate/Jamrate/Panel1/pnlSpeciallYield/PLoadInput/gbTriggerAlm
//      (AI(W906-FW-SBWC) 20260819, WAVE C ADD): no per-slot dfm geometry/
//      Caption/Color hydrated, same "cosmetic pre-population for golden's
//      own real VCL render only" posture as UnLoadPanel[]/UnLoadLabel[]
//      below -- every one of ShowBinSel_ARTNor/ShowBinSel_ARTRT/
//      TimerAutoCleanCountTimer/FormShow's writes to these widgets is an
//      unconditional runtime OVERWRITE (->TabVisible=/->Caption=/->Color=)
//      before this wave's tests ever read any of them back.
//    palAutoDeviceEjection: Top=473 (dfm) -- overwritten to 230 by the ctor's
//      SPIL_FOR_QLE branch when that flag is set, exactly as golden does.
//    grpBinDisp[]/MyBinSel[]/... arrays: no per-slot dfm geometry hydrated
//      (golden's OWN ctor never reads geometry off these either -- only
//      ->Caption/->Visible/->Font->Color, all zero/false/black-default via
//      each element's own default constructor -- MyBinSel[]'s ->Font is a
//      real `TFont*` as of AI(W906-FW-SBWB2) 20260819, see DESIGN NOTE
//      TfShowBinSelectLabel below; TFont::Color defaults 0, i.e. black,
//      matching this note's original claim).
//    UnLoadPanel[]/UnLoadLabel[]/pnlEmpty/pnlColor (AI(W906-FW-SBWB2)
//      20260819, WAVE B ADD): same "no per-slot geometry" posture --
//      ShowBinSel (this wave) never reads ->Left/->Top/->Width/->Height off
//      any of the 33 UnLoadPanel[]/UnLoadLabel[] slots, only ->Caption/
//      ->Color/->Font->Color, and it unconditionally OVERWRITES ->Color for
//      every slot with `Prod.iTrayType[i]!=tNotUse` before this wave's tests
//      ever read it back (the `.dfm` design-time Caption/Color, e.g.
//      pnlAuto1's `Caption="Auto1"`/`Color=clGreen` per
//      tools/dfm2rc/ir_out/cShowBinSelect.dfm.ir.json, is therefore cosmetic
//      pre-population for golden's OWN real VCL render only -- not hydrated
//      here). The slots ShowBinSel SKIPS (`continue` on
//      `Prod.iTrayType[i]==tNotUse`) are exactly the slots Wave A's
//      SetAutoVisible/SetLabelVisible already hide via
//      `grpBinDisp[i]->Visible=false`, so the un-hydrated cosmetic default
//      is never user-visible either.
// =============================================================================
#ifndef FORMS_FSHOWBINSELECT_H
#define FORMS_FSHOWBINSELECT_H

#include "vclcompat/vcl_compat.h"    // AnsiString, TObject, TStringList
#include "vclcompat/Controls.h"      // TLabel, TGroupBox, TPanel, TButton, TEdit, TLabeledEdit, TPageControl
#include "vclcompat/StringGrid.h"    // TStringGrid
#include "MachineType.h"             // e3TrayCount, eTrayCount

using vclcompat::TObject;
using vclcompat::TStringGrid;
using vclcompat::TLabel;
using vclcompat::TGroupBox;
using vclcompat::TPanel;
using vclcompat::TButton;
using vclcompat::TEdit;
using vclcompat::TLabeledEdit;
using vclcompat::TPageControl;
using vclcompat::TStringList;
using vclcompat::TFont;   // AI(W906-FW-SBWB2) 20260819: TfShowBinSelectLabel's Font member
using vclcompat::TSpeedButton;   // AI(W906-FW-SBWC) 20260819: btnCleanReset/btnAutoClean
using vclcompat::TTabSheet;      // AI(W906-FW-SBWC) 20260819: tsARTNormalBin/PageControl1_ART/etc
using vclcompat::TRadioGroup;    // AI(W906-FW-SBWC) 20260819: rg_FixBinBox

// -- facade-only widget extensions (see DESIGN NOTE above) -------------------
class TfShowBinSelectGrid : public vclcompat::TStringGrid
{
public:
    struct { int Top; } Selection;

    explicit TfShowBinSelectGrid(int initialColCount = 4, int initialRowCount = 14)
        : vclcompat::TStringGrid(initialColCount, initialRowCount)
    {
        Selection.Top = 0;
    }
};

class TfShowBinSelectPanel : public vclcompat::TPanel
{
public:
    int Top;
    int Left;    // AI(W906-FW-SBWC) 20260819: PLoadInput->Left (FormShow)
    int Width;   // AI(W906-FW-SBWC) 20260819: palUnloader->Width (FormShow, CC_Greatek)
    TfShowBinSelectPanel() : Top(0), Left(0), Width(0) {}
};

// golden TPageControl->ActivePage (a TTabSheet*) -- vclcompat::TPageControl only
// carries ->ActivePageIndex (int), see DESIGN NOTE. AI(W906-FW-SBWC) 20260819:
// FormShow (golden :800) is this tree's first reader of ->ActivePage itself.
class TfShowBinSelectPageControl : public vclcompat::TPageControl
{
public:
    TTabSheet *ActivePage = nullptr;
};

// golden TTimer -- see DESIGN NOTE above.
class TfShowBinSelectTimer
{
public:
    bool Enabled;
    TfShowBinSelectTimer() : Enabled(false) {}
};

// vclcompat::TLabel + Font -- see DESIGN NOTE (TfShowBinSelectLabel) above.
class TfShowBinSelectLabel : public vclcompat::TLabel
{
public:
    TFont *Font = new TFont();
};

// =============================================================================
//  TfShowBinSelect -- non-VCL facade (golden cShowBinSelect.h), WAVE A subset
// =============================================================================
class TfShowBinSelect
{
public:
    // -- Category / UPH grids (golden __published) ---------------------------
    TStringGrid *StrGrdCategory       = new TStringGrid(3, 16);
    TStringGrid *StrGrdCategoryART    = new TStringGrid(3, 16);
    TStringGrid *StrGrdCategoryContCT = new TStringGrid(3, 16);
    TStringGrid *StrARTSkipICCount    = new TStringGrid(3, 16);
    TfShowBinSelectGrid *UPH_StringGrid = new TfShowBinSelectGrid(4, 14);

    // -- unloader bin-select label arrays (golden ctor, see CTOR NOTE) -------
    // Sized e3TrayCount (33): 6 Auto + 12 Fix + 1 BulkBox + 14 Magazine.
    // MyBinSel[] retyped TfShowBinSelectLabel* by AI(W906-FW-SBWB2) 20260819
    // (Wave A had it as plain TLabel*) -- see DESIGN NOTE (TfShowBinSelectLabel)
    // above for why (ShowBinSel's `MyBinSel[i]->Font->Color=...`).
    TfShowBinSelectLabel *MyBinSel[e3TrayCount];
    TLabel    *MyBinSelLab[e3TrayCount];
    // MyBinSelARTFT[]/MyBinSelARTRT[] retyped TfShowBinSelectLabel* by
    // AI(W906-FW-SBWC) 20260819 (Wave A/B had them as plain TLabel*) -- same
    // ->Font->Color reason as MyBinSel[] above (ShowBinSel_ARTNor/ARTRT,
    // golden :2548/:2552/:2767/:2771).
    TfShowBinSelectLabel *MyBinSelARTFT[e3TrayCount];
    TLabel    *MyBinSelARTFTLab[e3TrayCount];
    TfShowBinSelectLabel *MyBinSelARTRT[e3TrayCount];
    TLabel    *MyBinSelARTRTLab[e3TrayCount];
    TGroupBox *grpBinDisp[e3TrayCount];

    // Sam 20210609: Fix AOI software upgrade -- golden sizes this eTrayCount
    // (cShowBinSelect.h:519), one slot per unloader-tray-type destination
    // (bigger than e3TrayCount's 33 -- includes eBulkBox distinctly from the
    // Auto/Fix/Magazine set), NOT e3TrayCount; kept exactly as golden sizes
    // it.
    TLabeledEdit *EditAi[eTrayCount];

    // AI(W906-FW-SBWB) 20260818: the 8 yield-monitor labels the FW-YEnable
    // wave found MISSING (header prose claimed them, class body had none --
    // Y2 went 4/14 because of exactly this). All 8 are `TLabel*` in golden
    // (cShowBinSelect.h:64/:66/:68/:70/:87/:94/:98/:99); landed now so
    // uYieldMonitoring.cpp's 10 re-gated (Y2) sites can dissolve.
    TLabel *labArmDiff             = new TLabel();
    TLabel *labSiteDiff            = new TLabel();
    TLabel *labLowYield            = new TLabel();
    TLabel *labTotalYield          = new TLabel();
    TLabel *labTotalYieldTotal     = new TLabel();
    TLabel *lblSpeciallYield       = new TLabel();
    TLabel *lblSpeciallYieldTotal  = new TLabel();
    TLabel *IntervalByTotal        = new TLabel();

    // -- other widgets this wave's methods touch ------------------------------
    TGroupBox *gbBinBox   = new TGroupBox();
    TPanel    *pnlMag123  = new TPanel();
    TPanel    *pnlFix789  = new TPanel();
    TPanel    *pnlAuto456 = new TPanel();
    TGroupBox *gbAuto6    = new TGroupBox();
    TfShowBinSelectPanel *palAutoDeviceEjection = new TfShowBinSelectPanel();
    TButton   *btReturn   = new TButton();
    TEdit     *EdLoadCount     = new TEdit();
    TEdit     *ed_AutoCleanCount = new TEdit();
    TfShowBinSelectTimer *TimerAutoCleanCount = new TfShowBinSelectTimer();
    // PageControl1 retyped TfShowBinSelectPageControl* by AI(W906-FW-SBWC)
    // 20260819 for ->ActivePage (FormShow, golden :800) -- see DESIGN NOTE.
    TfShowBinSelectPageControl *PageControl1 = new TfShowBinSelectPageControl();

    // AI(W906-FW-SBWC) 20260819, WAVE C ADD: ShowBinSel_ARTNor/ShowBinSel_ARTRT
    // widget surface (golden :2365-2580 / :2581-2799) -- both real bodies land
    // this wave, see GATE REGISTER (B10)/(B11) DISSOLVED note below.
    TTabSheet *tsARTNormalBin = new TTabSheet();
    TTabSheet *tsARTRTBin     = new TTabSheet();
    TPanel    *palARTNor      = new TPanel();
    TPanel    *palARTRT       = new TPanel();

    // AI(W906-FW-SBWC) 20260819, WAVE C ADD: TimerAutoCleanCountTimer widget
    // surface (golden :2168-2214, real body lands this wave).
    TRadioGroup  *rg_FixBinBox           = new TRadioGroup();
    TEdit        *ed_FixBinBoxAlarmCount = new TEdit();
    TLabel       *LabErrorBinNowCount    = new TLabel();
    TTabSheet    *PageControl1_ART       = new TTabSheet();   // also read by FormShow
    TTabSheet    *Tab_ARTSkipICCount     = new TTabSheet();
    TLabel       *AutocleanlifeTime      = new TLabel();
    TSpeedButton *btnCleanReset          = new TSpeedButton();   // also written by FormShow
    TPanel       *BulkBox                = new TPanel();
    TLabel       *labScheduleNAME        = new TLabel();
    TLabel       *labInQty               = new TLabel();

    // AI(W906-FW-SBWC) 20260819, WAVE C ADD: FormShow widget surface (golden
    // :758-866, real body lands this wave).
    TTabSheet    *tsUnloadMap          = new TTabSheet();
    TButton      *btnClearCount        = new TButton();
    TfShowBinSelectPanel *palUnloader  = new TfShowBinSelectPanel();
    TTabSheet    *tsFxiAI              = new TTabSheet();
    TTabSheet    *tsSECS_Category      = new TTabSheet();
    TTabSheet    *tsTestBin            = new TTabSheet();
    TTabSheet    *Tab_UPH              = new TTabSheet();
    TSpeedButton *btnAutoClean         = new TSpeedButton();
    TTabSheet    *tsASE                = new TTabSheet();
    TLabel       *labJamrate           = new TLabel();
    TLabel       *Jamrate              = new TLabel();
    TPanel       *Panel1               = new TPanel();
    TPanel       *pnlSpeciallYield     = new TPanel();
    TfShowBinSelectPanel *PLoadInput   = new TfShowBinSelectPanel();
    TTabSheet    *tsCategoryInfoContCT = new TTabSheet();
    TGroupBox    *gbTriggerAlm         = new TGroupBox();

    // AI(W906-FW-SBWB2) 20260819, WAVE B ADD: golden's ShowBinSel (golden
    // :394-404) builds two LOCAL arrays out of 33 individually-named `.dfm`
    // widgets each (pnlAuto1..6/pnlFix1..12/pnlBinBox/pnlMag1..14 ->
    // UnLoadPanel[]; lblAuto1..6/lblFix1..12/lblBinBox/lblMag1..14 ->
    // UnLoadLabel[]). Absence-claim grep this wave (`grep -n "\bpnlAuto1\b"
    // cShowBinSelect.cpp` / `grep -n "\blblAuto1\b" cShowBinSelect.cpp`,
    // 20260819): each of the 33 names appears in exactly ONE other place in
    // the whole golden class -- the still-BLOCKED Wave C ChangeBinDispStatus
    // (golden :210-215)'s own, separate local array -- so, same "array
    // member, not ~33 individually named pointers" shape as MyBinSel/
    // MyBinSelLab/etc above (see CTOR NOTE), these are declared as member
    // arrays and populated once in the ctor rather than as 33 named
    // pointers. Sized e3TrayCount (33), same convention as MyBinSel[] etc.
    // UnLoadLabel[] is TfShowBinSelectLabel* (not plain TLabel*) for the
    // same `->Font->Color` reason as MyBinSel[] above.
    TPanel *UnLoadPanel[e3TrayCount];
    TfShowBinSelectLabel *UnLoadLabel[e3TrayCount];

    // golden pnlEmpty/pnlColor (cShowBinSelect.h:362/364) -- read/written
    // directly by ShowBinSel (golden :727-736), not via an array.
    TPanel *pnlEmpty = new TPanel();
    TPanel *pnlColor = new TPanel();

    // -- data (golden cShowBinSelect.h:492-517) -------------------------------
    bool bShow;
    bool bUpdateBinDigital;
    int  iShowBinDigitalTask;
    int  ColorRed, ColorGreen, ColorOrange;
    TStringList *tsUPH;

    // -- form-level geometry (golden TForm->Left/Top/Width/Height) -----------
    // COSMETIC LAYOUT ONLY, offline inert -- nothing in this wave's scope (or
    // its tests) reads these back; declared as plain data (not a widget
    // wrapper) because nothing else in the tree needs a TfShowBinSelect* to
    // BE-A TControl. Same posture as forms/fContactCT.h's identical NOTE on
    // the golden TForm's own Height.
    int Left, Top, Width, Height;

    TfShowBinSelect();   // DEVIATION bootstrap ctor -- see CTOR NOTE above

    void FormDestroy(TObject *Sender);
    void FormClose(TObject *Sender);
    void InitShowBinDigital();
    void SetLabelVisible(int iTag, bool bVisible);
    void SetAutoVisible();
    void ShowInitialString();
    void ShowCategoryBin();
    void UPH_StringGridDblClick(TObject *Sender);
    void CaculateUPH();
    void edSLT01Change(TObject *Sender);
    void btnSetInpputCntClick(TObject *Sender);
    void RefreshAiCnt();
    void ed_AutoCleanCountClick(TObject *Sender);
    void labAuto1Click(TObject *Sender);
    void btReturnClick(TObject *Sender);
    void btnCleanResetClick(TObject *Sender);
    void btnClearCountClick(TObject *Sender);

    int iLowYieldBinSelectContactCount;   // KaiChen 20181115: BinSelect Yield control uses Contact Count

    // AI(W906-FW-SBWB2) 20260819, WAVE B primary target: ACTIVE, 3 GATEs
    // ((B9)/(B10)/(B11)), 1 GOLDEN BUG (B12) -- see GATE REGISTER above.
    void ShowBinSel();

    // GATE (B10)/(B11) DISSOLVED by AI(W906-FW-SBWC) 20260819 -- real bodies
    // land this wave (golden :2365-2580 / :2581-2799), see WAVE C SCOPE above.
    void ShowBinSel_ARTNor();
    void ShowBinSel_ARTRT();

    // GATE (B8): documented no-op this wave -- real body is WAVE B (golden
    // :1479-1633). Declared so btReturnClick's call site compiles/links.
    void PageControl1Change(TObject *Sender);

    // AI(W906-FW-SBWC) 20260819, WAVE C primary targets: ACTIVE.
    // TimerAutoCleanCountTimer -- golden :2168-2214, FULL, 0 GATEs.
    void TimerAutoCleanCountTimer(TObject *Sender);
    // FormShow -- golden :758-866, ACTIVE, 1 GATE (B18) -- see GATE REGISTER.
    void FormShow(TObject *Sender);
};

// AI(W906-FW3-ShowBinSelect-WA) 20260818: global NOT defined here -- main-loop
// homecoming (see CTOR NOTE / STATIC-INIT SAFETY above; same posture as
// forms/fContactCT.h / forms/fYieldMonitoring.h before their own swap
// waves).
extern TfShowBinSelect *fShowBinSelect;   // golden cShowBinSelect.h:522

#endif // FORMS_FSHOWBINSELECT_H
