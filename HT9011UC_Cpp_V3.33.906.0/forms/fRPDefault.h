// =============================================================================
//  forms/fRPDefault.h  --  non-VCL facade for golden's TfRPDefault
//                          (RPDefault.h)
//
//  AI(W906-FW3-BTQ1) 20260828: new file, FW wave FW3-BTQ1 (5 of 5 facades).
//  GOLDEN SOURCE: HT9011UC_Code_V3.33.906.0_20260618/RPDefault.h (70 lines)
//  + RPDefault.cpp (416 lines), read with `io.open(p, encoding='cp950')`
//  (0 U+FFFD), written out UTF-8 / bare-LF.
//
//  ROLE
//  ----
//  TfRPDefault is golden's "Recipe Parameter Default" auditor.  Three parallel
//  TStringLists (RP_Name / RP_Default / RP_Value) are filled by OTHER forms'
//  `SearchRecipeParameter` walks, and `CompareRPDefaultAndValue` then paints
//  every differing parameter red on the fRecipeDefault viewer, writes a
//  change-log .txt, and shows or hides that viewer accordingly.  The other
//  buttons push defaults into, or pull defaults back out of, five other forms.
//
//  ⚠ THIS IS THE SECOND FACADE forms/fTesterIF.cpp WAS WRITTEN AGAINST.  Four
//  of its members are RESERVED NAMES already spelled at seven `#if 0` sites in
//  forms/fTesterIF.cpp; the three list types were read off those call sites.
//
//  `python tools/census/wave_preflight.py RPDefault.cpp` (20260828, re-run at
//  closeout): 11 bodies = 11 `TfRPDefault::` members + 0 file-scope, 386 span
//  lines, 0 brace-swallow short-fall, 0 overload collapse.
//
//  THIS WAVE, against the 11-member denominator:
//     4 ACTIVE           30 of 386 golden span lines (7.8%)
//     7 GATED-WITH-BODY  356 golden span lines carried as `#if 0` transcript
//  ⚠ THIS IS THE THINNEST FACADE OF THE FIVE, and by lines it is dominated by
//  ONE body: CompareRPDefaultAndValue is 228 golden lines = 59.1% of the file
//  on its own, and it is gated twice over.  Quote both numbers or neither.
//
//  ============================================================================
//  *** THE NAMES `TfRPDefault` / `fRPDefault` ARE FREE -- THIS WAVE CLAIMS
//  THEM ***
//  ============================================================================
//  Measured 20260828 with `python tools/census/wave_preflight.py RPDefault.cpp`
//  (section [3], 1203 port files scanned): (a) class name `clear`, (c) global
//  name `clear`, (d) TU-local seam `clear`.  (b) reported 5 same-NAME members
//  of OTHER classes (FormShow/FormDestroy/spbSaveClick on TfSecurity /
//  TfSpeed / TFTestIF) -- different classes, not an occupation.
//  Cross-checked with `rg -c -g '!build_*' -g '*.h' -g '*.cpp' "\bTfRPDefault\b"
//  .`: the only hits are the generated tools/dfm2rc/layout_out/
//  RPDefault_layout.gen.cpp and its _b1d_regen twin, where it is a STRING
//  LITERAL in a .dfm widget table -- and `rg -n "layout_out" CMakeLists.txt`
//  returns 0, i.e. neither generated file is in any add_library.
//  Existing `fRPDefault` mentions are prose or `#if 0`: cSpeed.cpp:68,
//  uYieldMonitoring.cpp:1, forms/fHandlerSys.h:91, forms/fSpeed.h:1,
//  forms/fTesterIF.h:2 (all comments), forms/fTesterIF.cpp:10 (the gated call
//  sites).  Nothing starts compiling because of this wave.
//
//  ============================================================================
//  CONDITION-TWO CHECK (does a port of these bodies ALREADY exist, possibly as
//  a RENAMED FREE FUNCTION outside the class?)
//  ============================================================================
//  Searched 20260828, whole port tree, build_* excluded.
//  ⚠ MEASUREMENT CONVENTION: every count below is AS OF THE PRE-WRITE SCAN and
//  EXCLUDES this wave's own forms/fRPDefault.{h,cpp}.  Re-running the same
//  command afterwards returns higher numbers purely because these banners
//  mention the names in prose -- a comment is not a port.  Concretely,
//  `SetReciepeParameterDefault` now returns 2 files, and BOTH are the prose
//  lines in this pair.
//    * `CompareRPDefaultAndValue`     1 file -- forms/fTesterIF.cpp:1600, the
//      `#if 0` call site.  0 definitions.
//    * `SetReciepeParameterDefault`   0 files -- see the GOLDEN-SIDE FINDING
//      below; there is nothing to port.
//    * `ShowMonitoredParameter`       0 files
//    * `InitialComponentHead`         0 files
//    * `bReceipeDefaultDiffShow`      0 files
//    * `RECEIPE_DEFAULT` / `eRPVisibleCnt`   0 files each (golden's own
//      struct + enum, RPDefault.h:14-30 -- no port, and none needed: both are
//      used only inside GATE (R-6)).
//    * `fRecipeDefault`               2 files -- ONLY the generated
//      tools/dfm2rc/layout_out/uRecipeDefault_layout.gen.cpp and its twin,
//      i.e. .dfm DATA in a file no add_library compiles.  The COLLABORATOR
//      FORM HAS NO PORT AT ALL.  That is what gates (R-2) and half of (R-6).
//    * `RPLogRecord`                  2 files -- cmydef.{h,cpp}, i.e. a real
//      reachable global (ht9045_globals), NOT a port of any body here.
//      Recorded so a future wave does not mistake it for a blocker.
//    * `asDefaultRecipeChangeLogPath` 2 files -- common.{h,cpp}, likewise a
//      real reachable global (ht9045_core).  Also not a blocker; the log write
//      is gated on SAFETY, not on reach.
//  Renaming shapes probed: `rg "\b(RPDefault_|RecipeDefault_|RPD)\w+"` -> 0
//  hits.  `rg "CompareRecipeDefault|RecipeParamCompare"` -> 0 hits.
//  Conclusion: nothing to re-use, nothing re-translated twice.
//
//  ⚠ GOLDEN-SIDE FINDING, recorded because it changes the denominator's
//  meaning: golden RPDefault.h:59 declares
//  `void __fastcall SetReciepeParameterDefault(TWinControl *PCtrl);` and there
//  is NO DEFINITION OF IT ANYWHERE IN THE GOLDEN TREE -- a whole-tree grep of
//  HT9011UC_Code_V3.33.906.0_20260618 (20260828) returns exactly ONE hit, that
//  declaration.  It is a dangling declaration in golden itself, which is why
//  wave_preflight counts 11 bodies while the golden header lists 12 methods.
//  It is deliberately NOT declared here: there is no body to translate, and
//  `TWinControl` has no port in this tree either (0 hits in vclcompat/*.h).
//
//  ============================================================================
//  PRE-COMMITTED `#if 0` CALL SITES ELSEWHERE (4 names, 7 sites)
//  ============================================================================
//  All four are RESERVED NAMES; the types below were read off the call sites,
//  not chosen.  Every site is in forms/fTesterIF.cpp, committed one wave before
//  this one:
//    RP_Name       :1580  `fRPDefault->RP_Name->Clear();`
//                  :1730  `fRPDefault->RP_Name->Add(aHintName);`
//                  :1600  (as CompareRPDefaultAndValue's 1st argument)
//    RP_Value      :1582  `->RP_Value->Clear();`
//                  :1731  `->RP_Value->Add(PEdit->Text);`
//                  :1600  (3rd argument)
//    RP_Default    :1581  `->RP_Default->Clear();`
//                  :1732  `->RP_Default->Add(asDefaultValue);`
//                  :1600  (2nd argument)
//        -> all three need `Clear()` and `Add(AnsiString)`, and must be
//           acceptable as the parameters of CompareRPDefaultAndValue.  Golden
//           types them `TStringList *` (RPDefault.h:62-64); vclcompat's
//           TStringList has both methods.  Golden's types used unchanged.
//    CompareRPDefaultAndValue
//                  :1600  `fRPDefault->CompareRPDefaultAndValue(
//                             fRPDefault->RP_Name, fRPDefault->RP_Default,
//                             fRPDefault->RP_Value);`
//        -> exactly (TStringList*, TStringList*, TStringList*) and void.
//           ⚠ NOTE THE ARGUMENT ORDER: Name, DEFAULT, Value -- which matches
//           golden's parameter list `(aName, aDefault, aValue)`
//           (RPDefault.cpp:153) but NOT the visual order of the Clear() calls
//           two dozen lines above it (Name, Default, Value at :1580-1582 --
//           same order, so they agree).  Declared below in golden's order.
//  All seven sites stay `#if 0` after this wave.
//
//  ============================================================================
//  GATE REGISTER (7)
//  ============================================================================
//  (R-1) FormDestroy(TObject*)  golden :28-41 (14L) -- LINK BOUNDARY.
//        `LogSoftwareOffTime(AnsiString)` (golden :40) is declared cmydef.h
//        :5032 but its ONLY definition in this tree is acarry_shims.cpp:255
//        (a `{}` no-op) and that file is in **ht9045_sm**, which ht9045_forms
//        does not link.  It is NOT one of the four sanctioned forms->sm
//        exceptions (RecordProcess, ShowMyMessage, ShowErrorMessage,
//        MyDBIProcess).  ⚠ MyDBIProcess at :38 IS reachable (2-arg body
//        aHotPlateSubstrate.cpp:1099) and the three `delete`s are fine -- this
//        gate is ONE LINE deep and opens for free from an sm-reaching target.
//        Same shape as forms/fTestCategory.h GATE (T-8)(a), same wave.
//  (R-2) spbDefaulteViewShowClick(TObject*)  golden :43-46 (4L) -- MISSING
//        COLLABORATOR.  `fRecipeDefault->Show();` and fRecipeDefault has NO
//        port anywhere (see CONDITION-TWO: the only hits are .dfm data in
//        uncompiled generated files).  The whole 4-line body is that one call.
//  (R-3) spbSetRPDefaultClick(TObject*)  golden :53-78 (26L) -- LINK BOUNDARY,
//        several times over.  `fSecurity->Insufficient(168, false)` at :55 --
//        cSecurity.cpp is **ht9045_sm**.  `fSpeed` -- cSpeed.cpp is
//        **ht9045_sm** (and forms/fSpeed.cpp is in NO add_library at all).
//        `fCleaning->DoSetRPDefault`, `FTestIF->DoSetRPDefault`,
//        `fYieldMonitoring->DoSetRPDefault` -- none of those methods exists on
//        any facade here.
//  (R-4) spbReplyRPDefaultClick(TObject*)  golden :80-116 (37L) -- LINK
//        BOUNDARY as (R-3), PLUS it drives four other forms' SAVE paths
//        (`fSpeed->spbSaveClick`, `fCleaning->sbCleanSaveClick`,
//        `FTestIF->spbSaveClick`, `fYieldMonitoring->btnApplyClick`), i.e. it
//        REWRITES FOUR RECIPE FILES.  Safety and reach, independently.
//  (R-5) spbSaveClick(TObject*)  golden :125-151 (27L) -- LINK BOUNDARY as
//        (R-3) plus `fMain->BackupSetupFile()` at :150, which copies the whole
//        recipe.  ⚠ It is NOT gated for its RP_Name/RP_Default/RP_Value
//        Clear()s (:131-133) -- those three ARE this facade's own fields and
//        would work today; it is the eight `SearchRecipeParameter` walks and
//        the backup that gate it.
//  (R-6) CompareRPDefaultAndValue(TStringList*,TStringList*,TStringList*)
//        golden :153-380 (228L = 59.1% of the file) -- RESERVED NAME, so it is
//        DECLARED.  THREE INDEPENDENT GATES:
//        (a) MISSING COLLABORATOR -- it names 68 distinct `fRecipeDefault->`
//            members (8 TTabSheet* at :173-180 and 60 TLabel*/TPanel* pairs at
//            :186-258) and fRecipeDefault has no port (see R-2).
//        (b) WRITES DISK -- `MyForceDirectories(asDefaultRecipeChangeLogPath)`
//            at :354 CREATES A DIRECTORY, then
//            `slDefaultRecipeChangeLog->SaveToFile(aPath)` at :359 and again
//            at :363 writes a change-log .txt (the second one into
//            `IniConfig.asN14_20_ChangeLogPath`, i.e. a CUSTOMER-CONFIGURED
//            path that may be a network share).
//        (c) `fPassword->edUserName->Text` at :275 -- forms/fPassword.cpp
//            exists in ht9045_forms, but `edUserName` was NOT confirmed to be
//            a member of it (see DISCLOSURE).  Listed third because (a) and
//            (b) already decide it.
//        ⚠ GOLDEN BUG PRESERVED VERBATIM, worth flagging because it makes the
//        body's own result meaningless: :268 declares a LOCAL
//        `bool bReceipeDefaultDiffShow=false;` that SHADOWS the class member
//        of the same name (RPDefault.h:61).  Every write at :307 lands on the
//        local, and the member is never updated -- so the class field this
//        facade declares has NO WRITER even in golden.  Transcribed as-is
//        (改行為要留給使用者決定).
//        ⚠ A SECOND GOLDEN ODDITY, recorded not fixed: `static AnsiString
//        asOldValue[256]` at :161 is indexed by `i` over `aName->Count`, which
//        nothing bounds to 256.
//  (R-7) ShowMonitoredParameter()  golden :396-415 (20L) -- WRITES A SHARED
//        RUNTIME FILE.  Five `WriteDataToFile(Path, s, ...)` calls into the
//        HARD-CODED literal `"D:\\HT9045\\CurrentSetupData.txt"` (golden
//        :398), the first with the truncate flag `true`.  That file is on the
//        project's shared-runtime-parameter list (AGENTS.md / CLAUDE.md:
//        `setup.inf`, `CurrentSetupData.txt`, `system/`, `config/`, `CFG/` are
//        read-only by default).  ⚠ A HARD-CODED path cannot be redirected by
//        `--dry` -- the same class of hazard as the ReadLastDataFile /
//        WriteLastDataFile literals the wave brief names.  Everything it needs
//        is otherwise reachable (WriteDataToFile, GetLastOpenFN, TestIF_File,
//        LastSet), so this is a PURE SAFETY GATE.
//
//  ============================================================================
//  ACTIVE (4) -- reachability evidence
//  ============================================================================
//   TfRPDefault()             golden :20-26.  Three `new TStringList` into this
//        object's own fields.  vclcompat::TStringList is a leaf container that
//        calls nothing translated and dereferences no global, so this
//        static-init `new` is SIOF-safe (docs/KNOWLEDGE.md "static-init ctor
//        不可碰 NULL 全域"; the fLaserSensor incident that rule comes from
//        turned 88 of 134 ctest binaries into SEGFAULTs).  ⚠ This is the only
//        ctor in this wave that allocates, which is why it is called out.
//   spbExitClick(TObject*)    golden :48-51.  `Close();` (DEVIATION D-3).
//   InitialComponentHead()    golden :118-123.  Three
//        `strngrdReciepeParameterDefault->Cells[n][0]=...` writes.
//        vclcompat::TStringGrid provides `Cells[col][row]` as an l-value
//        (vclcompat/StringGrid.h:182-196) and defaults to 5x5, so columns 0..2
//        of row 0 are in range without any resize.
//   FormShow(TObject*)        golden :382-394.  `AccessLevel` (cmydef.h:3503)
//        vs `iDefHonPrecLevel` (cmydef.h:3588) -- both `extern int` in
//        cmydef.cpp, ht9045_globals -- driving `spbSetRPDefault->Enabled`,
//        then Left/Top (DEVIATION D-5).  A FULL translation: every line live.
//
//  ============================================================================
//  DEVIATIONS (port-only; each with why)
//  ============================================================================
//  (D-1) `__fastcall` dropped from every signature -- tree-wide convention.
//  (D-2) `TfRPDefault` has NO base class (golden: `: public TForm`).  Matches
//      forms/fCounterSel.h (D-2) / forms/fBuilder.h (D-2).
//  (D-3) `Close()` is a PORT-ONLY no-op (`{}`) -- needed by ACTIVE
//      spbExitClick (and named inside gated bodies).
//  (D-4) `SetReciepeParameterDefault(TWinControl*)` is NOT declared -- golden
//      declares it (RPDefault.h:59) but never defines it anywhere in its own
//      tree, and TWinControl has no port here.  See the GOLDEN-SIDE FINDING.
//  (D-5) `int Left = 0; int Top = 0;` declared on the facade, value-only, no
//      layout.  Written by ACTIVE FormShow (75 / 75).  Exactly the idiom at
//      forms/fLd_ULd.h:348-349 and vclcompat/Controls.h:271-272 ("只存值").
//  (D-6) golden's other 11 `__published` widgets (gbTestingStopTime, pnl1,
//      pnl2, label1, label2, pnlRPDefaultFalseColor, pnlRPDefaultTrueColor,
//      spbReplyRPDefault, spbDefaulteViewShow, spbSave, spbExit) are NOT
//      declared: no ACTIVE body and no pre-committed site touches them
//      (minimal-declaration idiom -- forms/fSetup.h:20-25).  Their names
//      appear only inside `#if 0`.
//  (D-7) golden's `RECEIPE_DEFAULT` struct and `eRPVisible*` enum
//      (RPDefault.h:14-30) are NOT ported.  Both are used only inside GATE
//      (R-6), and RECEIPE_DEFAULT's two members are `TLabel*`/`TPanel*` on
//      the un-ported fRecipeDefault.
//
//  ============================================================================
//  FIELD LIST (1 widget-grid + 1 speed-button + 3 lists + 1 bool + 2 ints)
//  ============================================================================
//    RP_Name    (TStringList*)   golden RPDefault.h:62, PRE-COMMITTED (3 sites)
//    RP_Default (TStringList*)   golden RPDefault.h:63, PRE-COMMITTED (3 sites)
//    RP_Value   (TStringList*)   golden RPDefault.h:64, PRE-COMMITTED (3 sites)
//    bReceipeDefaultDiffShow (bool)   golden RPDefault.h:61 -- see ZERO-WRITER
//    Left / Top (int)            DEVIATION D-5, ACTIVE FormShow
//    strngrdReciepeParameterDefault (TStringGrid*)  golden RPDefault.h:44
//    spbSetRPDefault                (TSpeedButton*) golden RPDefault.h:40
//  NOT declared: see DEVIATIONS (D-4) / (D-6) / (D-7).
//
//  ============================================================================
//  ZERO-WRITER FIELDS (task rule: state the consequence)
//  ============================================================================
//  * `bReceipeDefaultDiffShow` has NO WRITER -- and NOT because of this port.
//    GOLDEN's own CompareRPDefaultAndValue shadows it with a local of the same
//    name at :268, so the MEMBER is never assigned in golden either.  It has
//    no reader anywhere in this port.  Declared for surface parity with
//    golden; recorded here so nobody "fixes" the shadow without a decision
//    from the user.
//  * `RP_Name` / `RP_Default` / `RP_Value` are ALLOCATED by the ACTIVE ctor
//    and never filled: their producers are forms/fTesterIF.cpp's GATE (F-17)
//    (`->Add(...)` at :1730-1732) and this file's GATE (R-5), and their only
//    consumer is GATE (R-6).  CONSEQUENCE: all three are permanently empty
//    (Count==0), which is self-consistent -- an empty compare finds no
//    difference.  They are real, non-null objects, so the seven pre-committed
//    sites will bind and run without crashing the moment they are un-gated.
//  * `strngrdReciepeParameterDefault` is written ONLY by ACTIVE
//    InitialComponentHead (three header cells) and by GATED (R-6) (the data
//    rows).  CONSEQUENCE: header row only, no data.  Nothing reads it here.
//
//  ============================================================================
//  DISCLOSURE (task rule 8 -- what was NOT read to completion / NOT verified)
//  ============================================================================
//  - `fPassword->edUserName` (golden :275) was NOT checked against
//    forms/fPassword.h.  It is listed as gate (R-6)(c) precisely because it is
//    UNVERIFIED and is not load-bearing -- (a) and (b) already decide that
//    body.  A future wave must confirm it before un-gating.
//  - The 68 `fRecipeDefault->` member names in (R-6) were counted from
//    golden's two initialiser blocks (:173-180 and :186-258) but NOT
//    individually cross-checked against anything, because the collaborator
//    form has no port at all.
//  - `fSecurity->Insufficient` (golden :55, :84) was resolved only as far as
//    "cSecurity.cpp is in ht9045_sm"; the method's own declaration was not
//    read.  Immaterial to the archive-edge conclusion.
//  - `IniConfig.bN14_20_DefaultRecipeChangeLog` /
//    `IniConfig.asN14_20_ChangeLogPath` / `IniConfig.SocketHandlerID`
//    (golden :355-362) were NOT confirmed against Config.h.  All inside
//    GATE (R-6).
//  - `TStringList::SaveToFile` was NOT confirmed to exist on
//    vclcompat::TStringList.  It is inside GATE (R-6)(b), which is a SAFETY
//    gate -- so whether the method exists does not change the decision, but a
//    future wave must not assume it does.
//  - golden `RPDefault.dfm` was NOT opened.  ACTIVE FormShow writes Left/Top
//    itself, so no design-time geometry is needed -- but note
//    `strngrdReciepeParameterDefault` starts at vclcompat's 5x5 default, not
//    at golden's .dfm dimensions.
// =============================================================================
#ifndef FORMS_FRPDEFAULT_H
#define FORMS_FRPDEFAULT_H

#include "forms/FormWidgets.h"   // AnsiString / TObject / TSpeedButton /
                                 // TStringList / TStringGrid (vclcompat)

// ===========================================================================
//  TfRPDefault -- non-VCL facade (golden RPDefault.h:32-66,
//  `class TfRPDefault : public TForm`; NO base class here, DEVIATION D-2).
// ===========================================================================
class TfRPDefault
{
public:
    TfRPDefault();

    // -- golden public: User declarations ------------------------------------
    bool bReceipeDefaultDiffShow = false;               // golden RPDefault.h:61 -- ⚠ NO WRITER, in golden either (see ZERO-WRITER)
    TStringList *RP_Name    = 0;                        // golden RPDefault.h:62, PRE-COMMITTED (forms/fTesterIF.cpp:1580, :1600, :1730)
    TStringList *RP_Default = 0;                        // golden RPDefault.h:63, PRE-COMMITTED (forms/fTesterIF.cpp:1581, :1600, :1732)
    TStringList *RP_Value   = 0;                        // golden RPDefault.h:64, PRE-COMMITTED (forms/fTesterIF.cpp:1582, :1600, :1731)

    // -- DEVIATION D-5: golden's inherited TControl geometry, value-only -----
    int Left = 0;                                       // ACTIVE FormShow writes 75 (golden :392)
    int Top  = 0;                                       // ACTIVE FormShow writes 75 (golden :393)

    // -- golden __published widgets touched by an ACTIVE body ----------------
    TStringGrid  *strngrdReciepeParameterDefault = new TStringGrid();  // golden RPDefault.h:44
    TSpeedButton *spbSetRPDefault                = new TSpeedButton(); // golden RPDefault.h:40

    // -- ACTIVE this wave (defined in forms/fRPDefault.cpp) ------------------
    void spbExitClick(TObject *Sender);                 // golden :48-51    ACTIVE
    void InitialComponentHead();                        // golden :118-123  ACTIVE
    void FormShow(TObject *Sender);                     // golden :382-394  ACTIVE (FULL)
    void Close() {}                                     // DEVIATION D-3, port-only no-op

    // -- GATED this wave -- declared here, golden body translated in
    //    forms/fRPDefault.cpp inside `#if 0`.  Calling one is a LINK ERROR by
    //    design (see GATE REGISTER above). -------------------------------------
    void FormDestroy(TObject *Sender);                  // golden :28-41    GATE (R-1)
    void spbDefaulteViewShowClick(TObject *Sender);     // golden :43-46    GATE (R-2)
    void spbSetRPDefaultClick(TObject *Sender);         // golden :53-78    GATE (R-3)
    void spbReplyRPDefaultClick(TObject *Sender);       // golden :80-116   GATE (R-4)
    void spbSaveClick(TObject *Sender);                 // golden :125-151  GATE (R-5)
    void CompareRPDefaultAndValue(TStringList *aName, TStringList *aDefault, TStringList *aValue);
                                                        // golden :153-380  GATE (R-6)  RESERVED NAME
    void ShowMonitoredParameter();                      // golden :396-415  GATE (R-7)  ⚠ WRITES CurrentSetupData.txt
};

// AI(W906-FW3-BTQ1) 20260828: TfRPDefault/fRPDefault were FREE tree-wide (see
// the banner's measured provenance) -- declared here so the 7 pre-committed
// `#if 0` sites in forms/fTesterIF.cpp (:1580, :1581, :1582, :1600, :1730,
// :1731, :1732) have something real to bind to once THEIR own gates are
// separately lifted (NOT done by this wave).
extern TfRPDefault *fRPDefault;

#endif // FORMS_FRPDEFAULT_H
