// =============================================================================
//  forms/fBinSel.h  --  non-VCL facade for golden's TfBinSel (cBinSel.h)
//
//  AI(W906-FW-BinSel-WA) 20260819: new file, FW-3 表單波 (FW-BinSel-WA).
//  Golden ref: HT9011UC_Code_V3.33.906.0_20260618/cBinSel.h (285 lines) +
//  cBinSel.cpp (6,652 lines, cp950/Big5 -- decoded this wave with
//  `python3 -c "open(path,'rb').read().decode('cp950')"`, 0 U+FFFD measured
//  before any line below was written).
//
//  AI(W906-FW-BinSel-WB) 20260819: FW-3 表單波 WAVE B -- lands real bodies for
//  the 4 methods WAVE A queued as documented no-op stubs (ReadFunctionData,
//  SaveFunctionData, SetPrimeButton, mtTrayNameSetColor). See the updated
//  WAVE A/WAVE B SPLIT and GATE REGISTER below; per-line golden-bug/GATE
//  citations live next to each method in cBinSel.cpp (not duplicated here,
//  same "avoid the two files drifting apart" policy as WAVE A's own banner).
//
//  ROLE
//  ----
//  TfBinSel is golden's Bin-Select setup dialog: per-bin-type (RT/FT/OffLine/
//  ART-RT/ART-FT/MRT-RT/MRT-FT, 7 "tags") category/bin/tray assignment,
//  Cons.Fail/Fail-Percent/Fail-Count/Special-Bin alarm thresholds, and the
//  I/F-Error-bin bookkeeping consumed by `Command.cpp`'s SetTrayBinByDLL family
//  (21+ `#if 0` GATE call sites, see GATE REGISTER) and `SECSGEM/
//  uHGemHT9045_EC.cpp`'s 27 EC data-pointer registrations (GATE g7-g27, all
//  citing "fBinSel absent").
//
//  WAVE A / WAVE B SPLIT (golden line spans, this file's methods)
//  -----------------------------------------------------------------------
//    TfBinSel()              golden :970-1117   DEVIATION -- bootstrap ctor,
//                             see CTOR NOTE below (data-array population only,
//                             widget wiring dropped, see below)
//    FormDestroy             golden :1995-2088  ACTIVE
//    FormClose               golden :2099-2107  ACTIVE
//    ReadParam               golden :2090-2097  ACTIVE (pure read)
//    ChangeActivePageIndex   golden :2194-2212  ACTIVE, own PageControl1
//                             index model (see DESIGN NOTE)
//    spbSaveClick            golden :2214-2272  ACTIVE, 2 GATEs (fMain writes,
//                             fLotInfo branch)
//    SetConFail              golden :2274-2335  ACTIVE, 1 GATE (write)
//    SaveOther               golden :2339-2753  ACTIVE, many GATEs (writes --
//                             see WRITE-PATH GATE TABLE)
//    CheckFix2Tray           golden :4785-4805  ACTIVE (pure)
//    ReadWriteMRTMode        golden :6133-6185  ACTIVE, 2 GATEs (write half +
//                             fMain->SetOpenBin() absence)
//    ReadWriteSpecialFunction golden :6187-6252 ACTIVE, 1 GATE (write half)
//    CheckOSBin              golden :6254-6266  ACTIVE (pure)
//    SetOSBin                golden :6268-6284  ACTIVE
//    ReadPrimeDara           golden :6044-6057  ACTIVE (pure read + 1 stub call)
//    WritePrimeDara          golden :6059-6068  ACTIVE, 1 GATE (write)
//    Save                    golden :6296-6361  ACTIVE, many GATEs (writes +
//                             fShowBinSelect->ShowBinSel() Wave B elsewhere)
//    ARTBinCheck             golden :6381-6401  ACTIVE (pure, MyBinPanelData)
//    TransferBinTrayStrToName golden :6404-6423 ACTIVE (pure TStringList)
//    ReadFile                golden :1119-1596  ACTIVE, several GATEs (see
//                             GATE REGISTER) -- the MUST-HAVE method
//    ReadFunctionData(tag,szDir)  golden :4807-5667  WAVE B (this wave).
//                             ACTIVE: all CheckAndReadIniData/CheckSectionExist
//                             reads + the full old/new-format BinSelect[tag]/
//                             MyBinPanel[tag] post-processing (golden
//                             :5356-5636 -- real validation logic, not
//                             mechanical reads). GATE: `MyBinPanel[tag]->
//                             ed*->Text=...` widget-mirror block (22 sites,
//                             see GATE REGISTER G7). DEVIATION: gains an
//                             `AnsiString szDir` parameter (see cBinSel.cpp
//                             banner) -- ReadFile's call site updated to
//                             match.
//    SaveFunctionData(tag,FileName) golden :5668-6009  WAVE B (this wave).
//                             GATE: 100% WriteIniData writes, per
//                             WRITE-PATH GATE TABLE. ACTIVE:
//                             TransferBinTrayStrToName(tag) calls + the
//                             if/else-if tag-selection structure (kept so a
//                             future wave can un-gate write-by-write).
//                             DEVIATION: gains an `AnsiString FileName`
//                             parameter, same substitution as
//                             ReadFunctionData above -- SaveOther's call site
//                             updated to match, and ReadFunctionData's own 2
//                             internal calls pass their `szDir` as this
//                             `FileName` (same file, same golden
//                             FormSysTools object).
//    SetPrimeButton()        golden :6010-6042  WAVE B (this wave). GATE:
//                             entire body -- "純 widget", spbNormal/
//                             spbPrime/tsRetest/fMain->SetNormalOrPrime()
//                             all absent from the facade (see GATE REGISTER
//                             G8).
//    mtTrayNameSetColor(tag) golden :4033-4140  WAVE B (this wave). GATE:
//                             entire body -- see GATE REGISTER G9 (the
//                             corrected Tray256Core absence claim + the
//                             coupled-ctor-population reason it stays gated
//                             this wave) and (B16) in cBinSel.cpp (golden
//                             quirk: 2 of its calls target TfBinSel's OWN
//                             mtTrayName/mtTrayItem, not MyBinPanel[tag]'s).
//
//  WAVE B QUEUE -- CLOSED this wave (FW-BinSel-WB landed all 4 items WAVE A
//  queued below). Kept verbatim as a historical record of WAVE A's own
//  reasoning for QUEUING rather than dropping these 4 (still accurate as
//  the reasoning that applied BEFORE this wave; the SPLIT table above is the
//  current, authoritative status).
//  -----------------------------------------------------------------------
//  (declared with a documented no-op/stub body, NOT silently
//  dropped -- golden line spans)
//  -----------------------------------------------------------------------
//    ReadFunctionData(tag)   golden :4807-5667 (861 lines). 100% mechanical
//                             `sXxx[tag]->CommaText=FormSysTools->
//                             CheckAndReadIniData(GroupName,"<ecid> <key>",
//                             "")` population, repeated near-verbatim for all
//                             7 tags with only the GroupName/ECID-key strings
//                             varying per tag. Every one of these reads is
//                             POLICY-ALLOWED (this project's write-path gate
//                             only restricts writes; see WRITE-PATH GATE
//                             TABLE) -- this is deferred for TRANSCRIPTION
//                             VOLUME, not risk or a blocking dependency
//                             (`CheckAndReadIniData(FileName,Group,Name,
//                             Default)`, common.h:221-226, is real and already
//                             used by every other method in this file).
//                             Declared here so ReadFile's call site compiles;
//                             body is a documented no-op this wave (the 27
//                             TStringList arrays this would populate stay at
//                             the ctor's own "0"-placeholder initial state,
//                             which is EXACTLY golden's own pre-ReadFunctionData
//                             state -- not a new deviation).
//    SaveFunctionData(tag)   golden :5668-6009 (342 lines). 100% WriteIniData
//                             sink -- every statement's only effect is a
//                             recipe-file write, all of which this wave's
//                             write-path policy gates regardless of transcription
//                             effort. Declared as a documented no-op stub for
//                             the same reason as ReadFunctionData; a verbatim
//                             (fully `#if 0`-wrapped) skeleton would produce
//                             zero additional behaviour over this one-comment
//                             stub, so the 342-line skeleton itself is queued
//                             for Wave B rather than transcribed dead.
//    SetPrimeButton()        golden :6010-6042 (33 lines). Pure widget-
//                             visibility toggling (spbNormal/spbPrime/
//                             tsRetest/cbTestMode->Enabled) + `fMain->
//                             SetNormalOrPrime()` (unverified this wave). Same
//                             "純 UI 大宗" posture as fShowBinSelect's own
//                             WAVE B QUEUE. Declared as a no-op stub so
//                             ReadPrimeDara/WritePrimeDara's call sites
//                             compile.
//    mtTrayNameSetColor(tag) golden :4033-4140 (108 lines). Grid-cell/label
//                             colour rendering on `MyBinPanel[tag]->
//                             mtTrayName` (a `TTMyTray256*` -- no vclcompat
//                             stand-in exists anywhere in the tree, `grep -rn
//                             "class TTMyTray256" vclcompat/` -- 0 hits,
//                             20260819) and calls `InitDataToEdit(tag)`
//                             (golden :4142-4784, 643 more lines, same Edit-
//                             widget-population shape). Declared as a no-op
//                             stub so ReadFile's call site compiles.
//
//  NOT DECLARED AT ALL THIS WAVE (pure UI/grid-interaction surface; no stub
//  needed because nothing in this wave's ACTIVE call graph reaches them --
//  see fw-wave-loop skill S6 "大表單切塊" precedent, same posture as
//  fShowBinSelect.h's WAVE B QUEUE / cShowBinSelect.cpp's ChangeBinDispStatus)
//  -----------------------------------------------------------------------
//    FormShow (:1663-1994, 332 lines), cbTestModeChange (:2109-2192),
//    ShowChangeBinMessage (:1598-1662), InitDataToEdit (:4142-4784, 643
//    lines), the 27 `Sete*` grid-edit predicates (:3280-3833, e.g.
//    SeteDoubleContact/SeteConsFail/.../SeteSpecBinByArmPerSiteComparePercent),
//    every mouse handler (mtTrayNameMouseDown/mtBinSelectMouseDown/
//    mtBinSelectMouseMove/mtBinSelectMouseUp/mtTrayItemMouseUp/
//    sgSpecificBinMouseDown/ed_FixBinBoxAlarmCountMouseDown), SetBinTray/
//    ShowBinTray/Change/mtTrayNameSetColor's own callee InitDataToEdit,
//    sbtExitClick/spbNormalClick/spbPrimeClick/CancelErrorBinClick/
//    rg_FixBinBoxClick/btnSettingSpecificBinClick/palSpecificBinClick/
//    btnSetAll2NotUseClick/spbAOIBinClick/AOI_SplitToInt, FormShortCut.
//    `bool bCheckTrayCanUse(int)` -- declared in golden's `.h` public surface
//    but NO `TfBinSel::bCheckTrayCanUse` definition exists anywhere in golden
//    cBinSel.cpp (`grep -n "TfBinSel::bCheckTrayCanUse"
//    cBinSel_utf8.cpp` -- 0 hits, 20260819, against the full cp950-decoded
//    text) and no consumer calls it anywhere in this tree either -- NOT
//    declared here (would be a fabricated stub for a golden member that
//    itself has no body to be faithful to).
//
//  CTOR NOTE -- DEVIATION: bootstrap ctor, not a verbatim golden translation
//  -----------------------------------------------------------------------
//  Golden's ctor (cBinSel.cpp :970-1117) does two things: (a) builds
//  `vector<TMyBinPanel*> MyBinPanel` -- 7 elements, each a `TComponent`-
//  derived helper that OWNS ~850 lines worth of hand-placed VCL widgets
//  (`TTMyTray256 *mtBinSelect/mtTrayItem/mtTrayName/mtBinSelectBy`,
//  `TScrollBox`, `TScrollBar`, 30-odd `TEdit*`, all wired with pixel geometry
//  in `TMyBinPanel`'s own ctor, cBinSel.cpp :227-916) alongside a flat data
//  block (BackT6PosTray[][]/iErrorT6/iT6IsFail[]/bT6Link[]/bScan[]/
//  i2Contact[]/bConFail[]/.../dSpecBinByArmPerSiteComparePercent[], cBinSel.cpp
//  :172-207); (b) populates the 27 `TStringList*[eBinTypeTotal]` members (this
//  file's own public surface) with "0"-filled placeholders. This wave
//  translates (b) verbatim and (a)'s DATA HALF ONLY -- as `TMyBinPanelData`
//  below, a Wave-A-invented (non-golden-named) plain struct carrying exactly
//  the fields golden's own ReadFile/SaveOther/ARTBinCheck/CheckFix2Tray/
//  TransferBinTrayStrToName bodies read or write, with NO widget members.
//  `MyBinPanel[eBinTypeTotal]` stays a POINTER ARRAY (not a `vector`, matching
//  every other array-of-7 member in this class) so every `MyBinPanel[tag]->
//  field` expression already present in golden's own source text ports
//  UNCHANGED (verbatim arrow-syntax, only the pointee TYPE changed from
//  golden's widget-carrying `TMyBinPanel` to this wave's data-only
//  `TMyBinPanelData`) -- this is the SAME "keep the golden expression,
//  substitute the type" idiom as every other vclcompat replacement in this
//  tree (see KNOWLEDGE.md's "S" numbering), tagged S18 (S1-S17 already used by
//  earlier waves per those waves' own headers; this file does not renumber
//  them). This is a BEHAVIOUR-PRESERVING end-state for every ACTIVE method in
//  this wave -- none of them ever reads a `MyBinPanel[tag]->` WIDGET field
//  (confirmed by reading every one of this wave's method bodies against
//  golden before writing the port below) -- not a scope reduction of THIS
//  wave's own methods, though it DOES mean `MyBinPanel[tag]->mtTrayName/
//  mtBinSelect/mtTrayItem/Panel/sbBinSetScroll/...` (the widget half) has no
//  home yet; `mtTrayNameSetColor`/`InitDataToEdit`/the mouse handlers that DO
//  read those fields are exactly the methods this wave declares Wave B or
//  omits entirely (see split table above).
//
//  `cbTestMode->Items->Delete(1)`/`->Add(...)` (golden :1078-1096, populating
//  the test-mode combo per CosFunction flags) IS translated verbatim (cheap,
//  self-contained, needs only the already-real `vclcompat::TComboBox`).
//  `Panel->Visible=false; bMouseDown=false;` (golden :981-982) are dropped:
//  `Panel` is `TfBinSel`'s OWN top-level TForm surface (cosmetic, offline
//  inert, no reader anywhere in this wave's methods) and `bMouseDown` is only
//  read/written by the Wave-B mouse handlers (not declared this wave).
//  `MyBinPanel[tag]->Panel->Color=...` (golden :1104-1115, KYEC-only cosmetic
//  tab-background tint) is dropped for the same "widget-half, no reader"
//  reason.
//
//  STATIC-INIT SAFETY (SIOF homecoming -- real instance, guarded ctor body)
//  -----------------------------------------------------------------------
//  `fBinSel` is defined as a REAL instance (`TfBinSel *fBinSel = new
//  TfBinSel();`, cBinSel.cpp) per this wave's brief ("SIOF homecoming"), NOT
//  a null pointer awaiting a later `new` the way golden's own `TfBinSel
//  *fBinSel;` (cBinSel.cpp :47, uninitialised until WinMain constructs the
//  real VCL form) does it. The ctor's OWN body, read line-by-line against
//  golden above, touches ONLY: (1) the 27 `new TStringList()` + `->Add("0")`
//  loops -- no file I/O, no other TU's global; (2) `MyBinPanel[i]=new
//  TMyBinPanelData();` -- a locally-defined aggregate, zero-initialised by
//  its own in-class member initialisers, no cross-TU reference; (3)
//  `cbTestMode->Items->Delete/Add`, reading `CosFunction.bDisableRTBinSet` /
//  `CosFunction.bOffLineBin` / `USE_AUTO_RETEST` / `CosFunction.bUseSCKART` /
//  `CosFunction.bUseMRTMode` -- these ARE plain-old extern globals with
//  static storage duration and no dynamic initialiser of their own (same
//  category as cShowBinSelect.cpp's own ctor-safety argument for
//  `Prod`/`LastSet`/`CosFunction`/`IniConfig`), so reading them before their
//  OWN static initialisation phase finishes reads zero-initialised memory
//  (all-false/0), not undefined behaviour -- `cbTestMode->Items` merely ends
//  up with fewer/no extra combo entries than golden's own post-config-load
//  ctor would produce, which is cosmetic (Wave-B territory, `cbTestMode` has
//  no ACTIVE reader this wave) and self-heals the moment any later,
//  config-load-order-independent code path re-populates it. NO
//  `INIFileGeneral!=0` guard is therefore needed (verified this wave by
//  reading the full ctor body above, unlike cObserver.cpp's ctor which reads
//  INI files directly and DOES need the guard) -- flagged here, per project
//  convention, for the main loop to re-verify before/при this wave's
//  integration, not asserted as unconditionally proven.
//
//  DESIGN NOTE -- PageControl1 ActivePageIndex convention (Wave-A-invented)
//  -----------------------------------------------------------------------
//  Golden's `ChangeActivePageIndex()` assigns `PageControl1->ActivePage=
//  tsNormal/tsRetest/tsOffline/tsArtFT/tsArtRT/tsMrtFT/tsMrtRT` (TTabSheet*
//  assignment); golden's `SaveOther()` separately reads `PageControl1->
//  ActivePageIndex==0..6` as bare integer literals matching the SAME tab
//  order (verified against golden's own text: `==1`->RT, `==2`->OffLine,
//  `==3`->ArtFT, `==4`->ArtRT, `==5`->MrtFT, `==6`->MrtRT, else->Normal/FT).
//  `vclcompat::TPageControl` (vclcompat/Controls.h:434) carries only a plain
//  `int ActivePageIndex` (no `TTabSheet*`/`ActivePage` surface) -- this wave
//  therefore has `ChangeActivePageIndex()` assign the SAME integer literals
//  SaveOther already reads (0=Normal/FT, 1=Retest/RT, 2=Offline/OffT,
//  3=ArtFT/FT_ART, 4=ArtRT/RT_ART, 5=MrtFT/FT_MRT, 6=MrtRT/RT_MRT), which is
//  the identical mapping golden's OWN two methods already agree on -- not an
//  invented convention, a DIRECT reading of golden's literal SaveOther
//  comparisons back onto ChangeActivePageIndex's TTabSheet* assignments.
//
//  WRITE-PATH GATE TABLE (project policy: shared config under system\/
//  config\/CFG\/IniData\ -- READS may stay ACTIVE, WRITES/deletes are
//  `#if 0`-gated regardless of whether the underlying free function exists)
//  -----------------------------------------------------------------------
//  `CheckAndReadIniData`/`ReadIniData`/`CheckSectionExist` (common.h:221-226/
//  235-238/272, all REAL, already used by every other landed form in this
//  tree) are READS -- kept ACTIVE throughout this wave's methods.
//  `WriteIniData`/`WriteIniDataNoLog` (common.h:247-257, also REAL) and
//  `DeleteFile` are WRITES -- every call site is `#if 0`-gated below,
//  file-by-file citation in cBinSel.cpp next to each gate (not duplicated
//  here to avoid the two files drifting apart). `ATKRecipeInfo->SaveFile()`
//  (cprod.h:3159, real method, golden comment "為了加快ini讀取速度"/"要新增
//  工作檔比對用的檔案") and `fMain->BackupSetupFile()` (unverified this wave
//  whether real or still gated on fMain's own side) are BOTH treated as
//  writes and gated from THIS file's call sites regardless of their own
//  landed status, per this wave's brief ("cBinSel 的 Save/spbSaveClick/
//  ReadFile 家族極可能讀寫 bin/tray 設定檔... 寫入一律 #if 0 gate").
//  `MyForceDirectories` (common.h:341, real, 2-arg call shape) is a
//  filesystem mutation (directory creation) under the same shared recipe
//  path prefix -- gated too (conservative reading of "寫入一律 gate";  it is
//  a prerequisite step for an already-gated write, so gating it alongside is
//  a no-op-preserving choice, not a functional deviation).
//  `RecordProcess`/`AddSpace` are treated as ACTIVE (audit-trail / documented
//  common.h no-op respectively, NOT recipe-config content mutation).
//
//  GATE REGISTER (individual absence-claim citations; grep commands + dates
//  are this wave's, 20260819, re-run at hand-off per project policy)
//  -----------------------------------------------------------------------
//  (G1) ReadFile's `fQAMode->ReadFile();` (golden :1124) -- `grep -rn
//       "\\bfQAMode\\b" --include=*.h .` -- 0 hits anywhere in this tree.
//  (G2) ReadFile's `AutoForm[iBinBoxAtFix]->iTrayType==3` branch (golden
//       :1309) -- `grep -rn "AutoForm\\[" --include=*.h .` -- 0 hits; the
//       `else` arm (golden :1313 onward) is ACTIVE and is what golden itself
//       falls through to whenever this condition is false, so gating this
//       ONE condition to always-false is a fail-closed substitute, not a
//       skipped branch.
//  (G3) ReadFile's/SaveOther's `DeleteFile(...)` and every `FormSysTools->
//       WriteIniData`/`->OpenFormData`+write-session pair -- see WRITE-PATH
//       GATE TABLE above (project policy, not an absence).
//  (G4) `ReadWriteMRTMode`'s `fMain->SetOpenBin();` (golden :6164) -- `grep
//       -n "SetOpenBin" forms/fMain.h` -- 0 hits (20260819); `Prod.iOpenBin=
//       TestIF_File.iOpenBin;` immediately above stays ACTIVE (real,
//       independently observable in-memory state).
//  (G5) `Save`'s `case 3616: ... fShowBinSelect->ShowBinSel();` (golden
//       :6347) -- `ShowBinSel` is forms/fShowBinSelect.h's OWN documented
//       WAVE B QUEUE item (that file's banner, golden :388-757, ~250-widget
//       surface) -- gated here citing THAT file's own gap, not a new one.
//  (G6) `spbSaveClick`'s `fMain->SetStartModeData()`/`fMain->
//       BackupSetupFile()` (golden :2264/:2265) and the whole
//       `IniConfig.bSIGURDFunction && fLotInfo->pgLotinfo->ActivePage==...`
//       branch (golden :2267-2271) -- treated as writes per WRITE-PATH GATE
//       TABLE (fMain calls) plus, for the fLotInfo branch specifically,
//       unverified member surface (`pgLotinfo`/`tsYieldMonitior`/
//       `RefreshYieldMonitor` -- not checked against forms/fLotInfo.h this
//       wave) -- gated together as one block rather than risk a wrong guess
//       at fLotInfo's shape.
//  (G7) FW-BinSel-WB (this wave). `ReadFunctionData`'s `MyBinPanel[tag]->
//       ed*->Text=...` block (golden :5637-5661, 22 sites) -- `grep -n
//       "edBinSetT3Pos\\|edBinSettingConFail\\|edSpecBinByArmPerSiteComparePercent\\|
//       edBinSetT6Link" forms/fBinSel.h` -- 0 hits (20260819); `TMyBinPanelData`
//       is WAVE A's data-only stand-in (CTOR NOTE), no `TEdit *ed*` members.
//  (G8) FW-BinSel-WB (this wave). `SetPrimeButton`'s ENTIRE body --
//       `spbNormal`/`spbPrime`/`tsRetest` (`grep -n "spbNormal\\|spbPrime\\|
//       tsRetest" forms/fBinSel.h` -- only comment hits, 20260819) and
//       `fMain->SetNormalOrPrime()` (`grep -n "SetNormalOrPrime"
//       forms/fMain.h` -- 0 hits, 20260819) are all absent from the facade.
//  (G9) FW-BinSel-WB (this wave). `mtTrayNameSetColor`'s ENTIRE body --
//       `MyBinPanel[tag]->mtTrayName`/`mtTrayItem` (needs `TMyBinPanelData`
//       members of type `vclcompat::Tray256Core`, not yet added) and
//       `TfBinSel`'s OWN `mtTrayName`/`mtTrayItem` (golden cBinSel.h:73-74,
//       type `vclcompat::TrayCore`, not yet added either) are both absent
//       from the facade, plus `InitDataToEdit(tag)` (`grep -rn
//       "InitDataToEdit" --include=*.h --include=*.cpp .` -- only comment
//       hits, 20260819). CORRECTED ABSENCE CLAIM: `vclcompat::Tray256Core`/
//       `vclcompat::TrayCore` (vclcompat/TrayCore.h/.cpp, linked into
//       ht9045_sm per CMakeLists.txt:256) DO exist as of W7-C1 (20260728,
//       BEFORE this file's own WAVE A banner date) -- WAVE A's STUB
//       COLLISION SCAN below searched the literal string `"class
//       TTMyTray256"` (golden's own type name) and correctly got 0 hits, but
//       missed that vclcompat/Controls.h:79 already documents the RENAME
//       ("TTMyTray / TTMyTray256 -> vclcompat/TrayCore.* (W7-C1)"). The TYPE
//       exists; the MEMBER declarations + the ~90-line ctor-side cell-text
//       population golden's own `TMyBinPanel::TMyBinPanel` ctor does for
//       them (golden cBinSel.cpp :420-509) do not, and landing just this
//       method without that ctor-side population would half-populate the
//       grid (colour right, text stale) -- see cBinSel.cpp's own banner on
//       this method for the full reasoning. Flagged precisely for whichever
//       wave lands `TMyBinPanel`'s widget half.
//
//  STUB COLLISION SCAN (`grep -rn "TfBinSel\\b" --include=*.cpp
//  --include=*.h .` / `grep -rn "\\bfBinSel\\b" --include=*.cpp
//  --include=*.h .`, both run 20260819 before this file existed) -- every
//  hit is either a comment recording the SAME gap this wave closes
//  (Command.cpp:49/:136/:2637/:2642/:8485-8490/:8548-8552/:10489-10490/
//  :10522, cShowBinSelect.cpp:935/:950 GATE (B7), SECSGEM/
//  uHGemHT9045_EC.cpp:159-170 HOMECOMING NOTICE banner, forms/fShowBinSelect.h
//  :54/:201 GATE (B7)) or a genuinely GATED (`#if 0`) call site awaiting this
//  file to exist (Command.cpp:2649/:2652/:2696/:2697/:2704/:2713/:2714/:8492/
//  :8500/:8540/:8564/:8566/:8573/:8577/:8582/:8591/:8600/:8604/:8609/:8616,
//  :10208/:14924 `fBinSel->bShow` -- NOT itself inside an #if 0, see NOTE
//  below --, :12295/:12297/:12300/:12301/:12677/:12678/:13963,
//  SECSGEM/uHGemHT9045_EC.cpp GATE g7-g27 (27 sites, all `#if 0`),
//  SECSGEM/uHGemHT9045.cpp:3603/:3606/:3609, csystem.cpp:11525,
//  PowerSavingMode.cpp:822 -- inside its OWN pre-existing `#if 0` GATE (4),
//  unrelated to fBinSel's own absence). ZERO pre-existing DEFINITION of
//  `class TfBinSel` or a `TfBinSel fBinSel`/`TfBinSel *fBinSel=...` object
//  anywhere in the tree before this file. NOTE: `Command.cpp:10208`/`:14924`
//  read `fBinSel->bShow` OUTSIDE any `#if 0` in the surrounding few lines
//  visible at those exact line numbers as of 20260819 -- re-verify at
//  integration time whether that TU currently compiles at all without this
//  header (it may be gated by an enclosing block not visible at that
//  offset); this wave does not modify Command.cpp so does not resolve that
//  question either way.
//
//  HYDRATION -- no `.dfm` IR consulted this wave (`tools/dfm2rc/ir_out/
//  cBinSel.dfm.ir.json` not generated); the ONE grid this wave's code
//  touches, `sgSpecificBin` (SaveOther's GATED `sgSpecificBin->
//  Cells[i][1]=="V"` read, golden :2518 -- see WRITE-PATH GATE TABLE, the
//  surrounding WriteIniData session is `#if 0`'d whole), is declared here
//  with a placeholder `(32, 2)` (default member initializers cannot see
//  `iTestBinCount`, which lives in cmydef.h/cprod.h -- not included by this
//  header to keep its footprint small) and the ctor (cBinSel.cpp) overwrites
//  `ColCount` to the real `iTestBinCount` at construction time, matching
//  every other bin-indexed grid's runtime-hydration convention already
//  established by e.g. forms/fShowBinSelect.h's `ShowInitialString()`. 2
//  rows (header + data row, matching every other bin-indexed grid's OWN
//  "+1 header row" convention) -- a REASONED default, not a measured `.dfm`
//  value; flagged for correction if a future wave parses the real `.dfm`.
// =============================================================================
#ifndef FORMS_FBINSEL_H
#define FORMS_FBINSEL_H

#include "vclcompat/vcl_compat.h"    // AnsiString, TObject, TStringList
#include "vclcompat/Controls.h"      // TLabel, TPanel, TCheckBox, TComboBox, TRadioGroup, TEdit, TSpeedButton, TPageControl
#include "vclcompat/StringGrid.h"    // TStringGrid
#include "MachineType.h"             // eBinType/eBinTypeTotal, e3TrayCount, eTrayCount

using vclcompat::TObject;
using vclcompat::TStringList;
using vclcompat::TLabel;
using vclcompat::TPanel;
using vclcompat::TCheckBox;
using vclcompat::TComboBox;
using vclcompat::TRadioGroup;
using vclcompat::TEdit;
using vclcompat::TSpeedButton;
using vclcompat::TPageControl;
using vclcompat::TStringGrid;

// =============================================================================
//  TMyBinPanelData -- Wave-A-invented, data-only stand-in for golden's
//  widget-carrying `TMyBinPanel` (cBinSel.cpp :123-213). See CTOR NOTE above
//  for why the widget half is not ported this wave. Field names/sizes copied
//  VERBATIM from golden's own declaration so every `MyBinPanel[tag]->field`
//  expression in the ported methods below reads identically to golden's own
//  source text.
// =============================================================================
struct TMyBinPanelData
{
    // golden `int BackT6PosTray[iBinCount][iBinCount];` -- iBinCount==
    // TEST_MAX_BIN in golden (cBinSel.cpp :121, TU-local `const int
    // iBinCount=TEST_MAX_BIN;`), reproduced the same way in cBinSel.cpp.
    int BackT6PosTray[TEST_MAX_BIN][TEST_MAX_BIN];
    int    iErrorT6 = 0;
    int    iT6IsFail[eTrayCount] = {};
    bool   bT6Link[eTrayCount] = {};
    bool   bScan[TEST_MAX_BIN] = {};
    int    i2Contact[TEST_MAX_BIN] = {};
    bool   bConFail[TEST_MAX_BIN] = {};

    bool   bPersentEnable[TEST_MAX_BIN] = {};
    int    iPersentIgnore[TEST_MAX_BIN] = {};
    double dPersentNumber[TEST_MAX_BIN] = {};
    bool   bCountEnable[TEST_MAX_BIN] = {};
    int    iCountIgnore[TEST_MAX_BIN] = {};
    int    iCountNumber[TEST_MAX_BIN] = {};

    bool   bT6ART[eTrayCount] = {};
    bool   bT6CateR[eTrayCount] = {};

    bool         bSpecialBinByArm[TEST_MAX_BIN] = {};
    unsigned int iSpecialBinCountByArm[TEST_MAX_BIN] = {};
    bool         bSpecialBinBySocket[TEST_MAX_BIN] = {};
    unsigned int iSpecialBinCountBySocket[TEST_MAX_BIN] = {};

    bool bLowYield[TEST_MAX_BIN] = {};
    bool bArmYield[TEST_MAX_BIN] = {};
    bool bSiteYield[TEST_MAX_BIN] = {};

    int iAutoCleanByBin[TEST_MAX_BIN] = {};
    int iAutoCleanBySite[TEST_MAX_BIN] = {};

    bool   bSpecBinBySiteCompareEnable[TEST_MAX_BIN] = {};
    int    iSpecBinBySiteCompareIgnore[TEST_MAX_BIN] = {};
    double dSpecBinBySiteComparePercent[TEST_MAX_BIN] = {};
    bool   bSpecBinByArmPerSiteCompareEnable[TEST_MAX_BIN] = {};
    int    iSpecBinByArmPerSiteCompareIgnore[TEST_MAX_BIN] = {};
    double dSpecBinByArmPerSiteComparePercent[TEST_MAX_BIN] = {};

    bool bCancelErrorBin = false;

    TMyBinPanelData()
    {
        for (int i = 0; i < TEST_MAX_BIN; i++)
            for (int j = 0; j < TEST_MAX_BIN; j++)
                BackT6PosTray[i][j] = 0;
    }
};

// =============================================================================
//  TfBinSel -- non-VCL facade (golden cBinSel.h), WAVE A subset
// =============================================================================
class TfBinSel
{
public:
    // -- widgets this wave's ACTIVE methods touch (golden __published) ------
    TLabel      *Label1                  = new TLabel();
    TSpeedButton *spbSave                = new TSpeedButton();
    TPanel      *palSpecificBin          = new TPanel();
    TCheckBox   *CancelErrorBin          = new TCheckBox();
    TCheckBox   *chkShow0Xbin            = new TCheckBox();
    TRadioGroup *rg_FixBinBox            = new TRadioGroup();
    TEdit       *ed_FixBinBoxAlarmCount  = new TEdit();
    TStringGrid *sgSpecificBin           = new TStringGrid(/*cols*/32, /*rows*/2);   // ctor overwrites ColCount to iTestBinCount -- see HYDRATION
    TCheckBox   *cbUseMRTMode            = new TCheckBox();
    TComboBox   *cbbAutoSiteMap          = new TComboBox();
    TComboBox   *cbbASMPassBin           = new TComboBox();
    TCheckBox   *cbOutShtLoseICSetErrUntilOneCycle = new TCheckBox();
    TCheckBox   *cbIndexDropErrSetErrUntilOneCycle = new TCheckBox();
    TComboBox   *cbTestMode              = new TComboBox();
    TPageControl *PageControl1           = new TPageControl();   // see DESIGN NOTE (ActivePageIndex convention)

    // -- data (golden cBinSel.h public surface) ------------------------------
    bool bShow = false;

    // -- data (golden cBinSel.h PRIVATE surface, needed by ACTIVE methods) --
    // Kept public here (no golden-private/public distinction enforced by any
    // consumer in this tree, same posture as every other facade class in
    // FormsFacade.h) -- ReadParam/spbSaveClick are the only readers/writers
    // this wave.
    bool bUseContinueFail = false;
    bool bUseFailRate     = false;
    bool bSaveBin         = false;

    TStringList *sBinDoubleContact[eBinTypeTotal];                              // Steven 20140510 : Secs Gem
    TStringList *sBinConsFail[eBinTypeTotal];
    TStringList *sBinEnableFail[eBinTypeTotal];
    TStringList *sBinFailPercent[eBinTypeTotal];

    TStringList *sBinFailIgnore[eBinTypeTotal];                                 // Steven 20140529
    TStringList *sBinCountEnable[eBinTypeTotal];
    TStringList *sBinCountIgnore[eBinTypeTotal];
    TStringList *sBinCountNumber[eBinTypeTotal];

    TStringList *sSpecialBinByArm[eBinTypeTotal];                               // ChungHung 20140730
    TStringList *sSpecialBinCountByArm[eBinTypeTotal];
    TStringList *sSpecialBinBySocket[eBinTypeTotal];
    TStringList *sSpecialBinCountBySocket[eBinTypeTotal];

    TStringList *sLowYield[eBinTypeTotal];                                      // Steven 20140828
    TStringList *sArmYield[eBinTypeTotal];
    TStringList *sSiteYield[eBinTypeTotal];

    TStringList *sBinTraySetT3Pos[eBinTypeTotal];                               // Command.cpp SetTrayBinByDLL -- 21+ GATE consumer
    TStringList *sBinTraySetT3PosName[eBinTypeTotal];                           // Steven 20241205
    TStringList *sBinType[eBinTypeTotal];

    TStringList *sBySiteClean[eBinTypeTotal];                                   // Steven 20160308
    TStringList *sByBinClean[eBinTypeTotal];
    TStringList *sT3TrayType[eBinTypeTotal];
    TStringList *sT6Retest[eBinTypeTotal];
    TStringList *sT3CateR[eBinTypeTotal];

    TStringList *sSpecBinBySiteCompareEnable[eBinTypeTotal];                    // JerryYang 20170712
    TStringList *sSpecBinBySiteCompareIgnore[eBinTypeTotal];
    TStringList *sSpecBinBySiteComparePercent[eBinTypeTotal];
    TStringList *sSpecBinByArmPerSiteCompareEnable[eBinTypeTotal];
    TStringList *sSpecBinByArmPerSiteCompareIgnore[eBinTypeTotal];
    TStringList *sSpecBinByArmPerSiteComparePercent[eBinTypeTotal];

    TStringList *sAOIBinTraySetting[eBinTypeTotal];                             // Eastsun 20260316

    TStringList *sBinTrayLinked[eBinTypeTotal];                                 // Ifor 20240730
    TStringList *sBinLinked[eBinTypeTotal];
    TStringList *sMagazineSetup[eBinTypeTotal];

    // -- Wave-A data-only stand-in for golden's `vector<TMyBinPanel*>` -------
    TMyBinPanelData *MyBinPanel[eBinTypeTotal];

    TfBinSel();   // DEVIATION bootstrap ctor -- see CTOR NOTE above

    void FormDestroy(TObject *Sender);
    void FormClose(TObject *Sender);
    void ReadParam();
    void ReadFile(bool bDelOffline, bool bChangeNeme, AnsiString sFileName);
    void ChangeActivePageIndex();
    void spbSaveClick(TObject *Sender);
    void SetConFail(int iBinasgnMode);
    void SaveOther(AnsiString szDir);
    bool CheckFix2Tray();
    void ReadWriteMRTMode(int iMode);
    void ReadWriteSpecialFunction(bool bReadWrite);
    bool CheckOSBin();
    bool SetOSBin(int iBin);
    void ReadPrimeDara();
    void WritePrimeDara(int Mode);
    void Save(int iECIDCode, int iFileCode);
    void ARTBinCheck(int tag);
    void TransferBinTrayStrToName(int iTag);

    // -- FW-BinSel-WB (this wave): real bodies landed in cBinSel.cpp -- see
    // WAVE A/WAVE B SPLIT above and GATE REGISTER G7-G9 for what's gated
    // inside each. `szDir`/`FileName` are DEVIATION parameters (golden's own
    // signature has neither -- see cBinSel.cpp banners on both methods).
    void ReadFunctionData(int tag, AnsiString szDir);
    void SaveFunctionData(int tag, AnsiString FileName);
    void SetPrimeButton();
    void mtTrayNameSetColor(int tag);
};

// AI(W906-FW-BinSel-WA) 20260819: SIOF homecoming -- real instance (see
// STATIC-INIT SAFETY above).
extern TfBinSel *fBinSel;                              // golden cBinSel.h:278
extern const AnsiString sTrayName[eTrayCount];         // golden cBinSel.h:279

#endif // FORMS_FBINSEL_H
