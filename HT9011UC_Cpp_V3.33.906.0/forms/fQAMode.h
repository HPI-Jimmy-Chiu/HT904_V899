// =============================================================================
//  forms/fQAMode.h  --  non-VCL facade for golden's TfQAMode (QAMode.h)
//
//  AI(W906-FW3-BTQ1) 20260828: new file, FW wave FW3-BTQ1 (3 of 5 facades).
//  GOLDEN SOURCE: HT9011UC_Code_V3.33.906.0_20260618/QAMode.h (64 lines)
//  + QAMode.cpp (336 lines), read with `io.open(p, encoding='cp950')`
//  (0 U+FFFD), written out UTF-8 / bare-LF.
//
//  ROLE
//  ----
//  TfQAMode is golden's QA-mode / QA-sampling recipe page: eleven widgets that
//  round-trip through `TestIF_File.iQAMode*` / `.*QASampling*` and are
//  persisted into `<DataPath>\<recipe>\Tester.Data`.  Its file-scope
//  `QABackupStatus(bool)` is a separate concern -- it saves and restores the
//  SiteMap on/off bitmap across a QA run, through `config\config.ini`.
//
//  `python tools/census/wave_preflight.py QAMode.cpp` (20260828, re-run at
//  closeout): 13 bodies = 12 `TfQAMode::` members + 1 file-scope
//  (`QABackupStatus`), 302 span lines, 0 brace-swallow short-fall, 0 overload
//  collapse.
//
//  THIS WAVE, against the 12-MEMBER denominator (the file-scope body is
//  counted separately, as pre-flight instructs):
//     5 ACTIVE           ctor / btnOkClick / edQAModeMouseDown / DoFormToData /
//                        edtQASampleCntMouseDown
//     1 ACTIVE-PARTIAL   FormClose -- golden :172 live, :173 gated
//     6 GATED-WITH-BODY  DoIniDataToForm / btnApplyClick / FormShow /
//                        ReadFile / Image1Click / ShowTrayDirectIMG
//     + 1 GATED file-scope body, QABackupStatus
//  BY LINES: 40 of 302 golden span lines are live (13.2%).  Quote both numbers
//  or neither.
//
//  ============================================================================
//  *** THE NAMES `TfQAMode` / `fQAMode` ARE FREE -- THIS WAVE CLAIMS THEM ***
//  ============================================================================
//  Measured 20260828 with `python tools/census/wave_preflight.py QAMode.cpp`
//  (section [3], 1203 port files scanned): (a) class name `clear`, (c) global
//  name `clear`, (d) TU-local seam `clear`.  (b) reported 14 same-NAME members
//  of OTHER classes (FormShow/FormClose/ReadFile/DoIniDataToForm/
//  edQAModeMouseDown on TATCInterfaceForm / TfBinSel / TfConfiguration /
//  TfSecurity / TfTemperFrom / TfLotInfo / TfTemp_Set) -- different classes,
//  not an occupation.  Cross-checked with `rg -c -g '!build_*' -g '*.h'
//  -g '*.cpp' "\bTfQAMode\b" .`: the only hits are the generated
//  tools/dfm2rc/layout_out/QAMode_layout.gen.cpp and its _b1d_regen twin,
//  where it is a STRING LITERAL in a .dfm widget table -- and
//  `rg -n "layout_out" CMakeLists.txt` returns 0, i.e. neither file is in any
//  add_library.
//
//  ⚠ THIS WAVE FALSIFIES TWO OF THE TREE'S OWN COMMENTS.  Recorded here, NOT
//  edited (this wave adds files only):
//    * cBinSel.cpp:18 and :1414 both quote
//      `grep -rn "\bfQAMode\b" --include=*.h .` -- 0 hits.
//    * forms/fBinSel.h:358 (its GATE (G1)) repeats the same claim.
//  Both sit above `#if 0` blocks that stay `#if 0`; nothing starts compiling.
//  ⚠ AND NOTE THE DIRECTION OF THAT GATE: cBinSel's (G1) is
//  `fQAMode->ReadFile();`, and `ReadFile` is GATE (Q-4) here -- a HIDDEN-WRITE
//  gate.  Un-gating cBinSel's site alone would NOT be enough; it would also
//  need this facade's (Q-4) opened, which is a safety decision, not a
//  reachability one.
//
//  ============================================================================
//  CONDITION-TWO CHECK (does a port of these bodies ALREADY exist, possibly as
//  a RENAMED FREE FUNCTION outside the class?)
//  ============================================================================
//  Searched 20260828, whole port tree, build_* excluded.
//  ⚠ MEASUREMENT CONVENTION: every count below is AS OF THE PRE-WRITE SCAN and
//  EXCLUDES this wave's own forms/fQAMode.{h,cpp}.  Re-running afterwards
//  returns higher numbers purely because these banners mention the names in
//  prose -- a comment is not a port.  Concretely, `QABackupStatus` and
//  `ShowTrayDirectIMG` both gain 2 files, all four of them this pair.
//    * `QABackupStatus`            0 files -- and pre-flight's section [4b]
//      (file-scope call sites) reports LIVE 0 / GATED 0, so nothing in this
//      port calls it under any name.
//    * `QAMode_BackupTestSiteUse` / `QAMode_BackupUseTestSocket`
//      (the two ini keys QABackupStatus writes)          0 files
//    * `ShowTrayDirectIMG`  ⚠ NAME-SIMILARITY HAZARD, resolved: 2 files, both
//      forms/fTrayAssignment.{h,cpp} -- and that is
//      `TfTrayAssignment::ShowTrayDirectIMG` (declared forms/
//      fTrayAssignment.h:696, defined forms/fTrayAssignment.cpp:1207, golden
//      cTrayAssignment.cpp:1127-1148).  A DIFFERENT class's member from a
//      DIFFERENT golden file that happens to share the name because both draw
//      the same tray-direction bitmap.  Not a port of TfQAMode's.
//      (Its owning file is also in ht9045_sm, so it is not even reachable
//      from here -- recorded for completeness, not as the deciding fact.)
//    * `iQAModeCount` / `bQAD22DoubleContact` / `iQASamplingCnt`
//      -- many files, ALL of them readers of the `TestIF_File` struct members
//      (cprod.h:721-724, :1916-1923).  The struct is the shared data this form
//      edits; it is not a port of any body here.
//  Renaming shapes probed: `rg "\b(QAMode_|QA_)\w+"` -> only the two ini-key
//  literals above (0 hits).  `rg "DoQAModeToForm|QAModeToData"` -> 0 hits.
//  Conclusion: nothing to re-use, nothing re-translated twice.
//
//  ============================================================================
//  PRE-COMMITTED `#if 0` CALL SITES ELSEWHERE (1 name, 2 sites)
//  ============================================================================
//    fShow   Command.cpp:10247, Command.cpp:15035 -- spelled `fQAMode->fShow`
//            inside the long "is ANY form open" OR chain.
//            -> matches this facade's `bool fShow;`.
//  ⚠ Both lines carry `fCleaning->fShow` and `fCounterSel->fShow` on the SAME
//  physical line, and the same chain spells `fCCLink->bShow` with a **b**.
//  Do not normalise any of them.  Both sites stay `#if 0` after this wave.
//
//  ============================================================================
//  GATE REGISTER (6 members + 1 file-scope)
//  ============================================================================
//  (Q-1) DoIniDataToForm()  golden :38-54 (17L) -- MISSING MEMBER ON ANOTHER
//        FACADE.  golden :40 is `fLotInfo->edQAMode->Text = TestIF_File
//        .iQAModeCount;`.  `fLotInfo` IS real and live (forms/fLotInfo.cpp,
//        ht9045_forms -- the SAME target as this facade), but forms/fLotInfo.h
//        declares NO `edQAMode` member: `rg "edQAMode" forms/fLotInfo.h`
//        (20260828) returns 3 hits and all three are PROSE
//        (:276 / :365 name the gated `edQAModeMouseDown` body, :773 quotes a
//        golden `edQAMode->Left=136;` line inside a comment).  The other 13
//        lines are pure widget reflection off TestIF_File and would have been
//        reachable.
//        NOT SPLIT: dropping :40 would silently stop the MAIN screen's QA
//        counter from tracking this form -- an invented behaviour, not a
//        translation (忠實優先).
//  (Q-2) btnApplyClick(TObject*)  golden :73-110 (38L) -- WRITES DISK.
//        Thirteen `WriteIniData(szDir, "QA Mode"/"QA Sampling", ...)` calls
//        (:91-104) into `<DataPath>\<recipe>\Tester.Data`, preceded by
//        `MyForceDirectories(szDir)` at :88 which CREATES DIRECTORIES.  Both
//        helpers are REACHABLE (common.cpp, ht9045_core) -- this is a pure
//        read-only-direction safety gate, not a link gate.  Also [T] on GATE
//        (Q-4) ReadFile at :106 and (Q-1) at :107, plus
//        `fMain->BackupSetupFile()` (forms/fMain.h:260 -- that one DOES exist)
//        and `fBinSel->ReadFile(...)` (no fBinSel facade in this tree).
//  (Q-3) FormShow(TObject*)  golden :112-168 (57L) -- TRANSITIVE HIDDEN WRITE
//        + missing members.  :128 calls ReadFile (GATE Q-4, which creates
//        directories AND seeds ini keys), :129 calls (Q-1), :130 calls (Q-6).
//        Additionally it reads `Prod.iTrayType[]` / `s6TrayName[]` /
//        `iFixRight` / `LevelSet.AccessLevel[85]` and writes `Width`/`Height`,
//        none of which this facade declares (D-5).
//  (Q-4) ReadFile()  golden :178-232 (55L) -- HIDDEN WRITE, and the most
//        important gate in this file to understand.  Its NAME says Read.  It
//        writes twice over:
//          (a) `MyForceDirectories(szDir)` at :184 CREATES DIRECTORIES under
//              DataPath before anything is read at all;
//          (b) eight of its reads go through `CheckAndReadIniData(...)`
//              (:202-231), which is common.cpp:613 -- and common.cpp:623-624
//              are
//                  `if (!INIFile->ValueExists(Group, Name))
//                       INIFile->WriteBool(Group, Name, Value);   // seed`
//              i.e. a *read* SEEDS the file with the default whenever a key is
//              absent.  This is the same function-pair trap forms/fHotPlate.h
//              GATE (G-1) and forms/fCounterSel.h GATE (C-1) already document:
//              `CheckAndReadIniData` writes back, the similarly-named pure
//              `ReadIniData` (used at :187/:194/:197/:199) does not.
//        ⚠ FUNCTION NAMES CONTAINING "Read" DO NOT GUARANTEE READ-ONLY.
//  (Q-5) Image1Click(TObject*)  golden :234-253 (20L) -- NO TIMAGE PORT.
//        `Image1->Picture->LoadFromFile(sDir)` -- neither TImage nor TPicture
//        exists in vclcompat (`rg "class TImage|class TPicture"
//        vclcompat/*.h` -> 0 hits, 20260828).  It also MUTATES
//        `TestIF_File.iQATrayDirect` (:242) as a side effect of a repaint,
//        which is why the mutation cannot simply be split out and kept.
//        (MyDBIProcess at :250 IS reachable -- aHotPlateSubstrate.cpp:1099 --
//        so it is NOT a blocker.  Recorded so a future wave does not
//        re-derive it.)
//  (Q-6) ShowTrayDirectIMG()  golden :255-277 (23L) -- same TImage absence as
//        (Q-5).  It also clamps `TestIF_File.iQATrayDirect` in place (:264-265).
//        ⚠ GOLDEN BUG RECORDED, NOT FIXED: the clamp tests
//        `iQATrayDirect<0 || iQATrayDirect>8`, but Image1Click's own wrap
//        (:240) rolls at `>=8`, so 8 is reachable in the clamp's eyes and
//        would load `type8.bmp`.  Verbatim (改行為要留給使用者決定).
//  (Q-F) QABackupStatus(bool)  golden :285-335 (51L), FILE-SCOPE -- WRITES
//        DISK.  Four `WriteIniData(...)` calls: two into
//        `AuthPath+"config.ini"` (:304-305, the SHARED runtime-parameter file
//        that docs/KNOWLEDGE.md's "Gerneral.ini 20260817被整檔重寫" incident
//        is about) and two into `<recipe>\Tester.Data` (:332-333).  It also
//        overwrites the live SiteMap arrays `bTestSiteUse[][][]` and
//        `LastSet.bUseTestSocket[][][]` (:319-320) -- i.e. it changes which
//        test sites the machine considers enabled, which is a MODE SWITCH, not
//        just a file write.  Two independent reasons to gate.
//        Secondary blockers: `IntUnionBit` and `fMain->ShowTestHeadComp` were
//        NOT resolved to a port (see DISCLOSURE).
//
//  ============================================================================
//  ACTIVE (5, plus the PARTIAL FormClose) -- reachability evidence
//  ============================================================================
//   TfQAMode()               golden :22-25.  Golden's body is EMPTY.  Nothing
//        to gate, nothing to get wrong; SIOF-trivially safe.
//   btnOkClick(TObject*)     golden :27-30.  `Close();` (DEVIATION D-3).
//   edQAModeMouseDown        golden :32-36.  `fQwertyKey->ShowQwertyKey(
//        (TEdit*)Sender, N_INTEGER, 0, true, 10000, 5)` -- fQwertyKey is
//        forms/fQwertyKey.cpp, SAME ht9045_forms target; the 6-argument
//        spelling matches forms/fQwertyKey.h:370 exactly.  N_INTEGER is
//        `extern const int` cmydef.h:289 -> cmydef.cpp, ht9045_globals.
//        ⚠ golden really does pass min=10000, max=5 -- i.e. INVERTED relative
//        to the CheckRange(value, 5, 10000) its own DoFormToData uses at :58.
//        Transcribed verbatim, NOT "fixed" (改行為要留給使用者決定).
//   DoFormToData()           golden :56-71.  Eleven widget->TestIF_File
//        assignments plus `CheckRange` (a TEMPLATE, MachineType.h:1525,
//        header-only -- no link edge at all) and `atoi`.  TestIF_File is
//        cprod.h:2577 -> cprod.cpp, ht9045_globals.
//        ⚠ THIS WRITES A GLOBAL CONFIG STRUCT IN MEMORY, and that is
//        deliberate: it touches no file, sends no packet and moves no
//        hardware, so it does not meet this wave's gating judgement.  Its
//        disk-committing partner, btnApplyClick, IS gated (Q-2).
//   edtQASampleCntMouseDown  golden :279-283.  Same shape as
//        edQAModeMouseDown; here golden passes min=10000, max=0.
//   FormClose()              golden :170-176, ACTIVE-PARTIAL.  Live = :172
//        `fShow=false;`.  GATED = :173 `DoIniDataToForm();`, which is (Q-1).
//        A LINE-LEVEL gate, following the forms/fTowerLight.h (T-2) and
//        forms/fHandlerSys.h (H22-1) precedent: keeping :172 live is what
//        gives `fShow` a real writer, and the dropped line is itself gated
//        so the two un-gate together.
//
//  ============================================================================
//  DEVIATIONS (port-only; each with why)
//  ============================================================================
//  (D-1) `__fastcall` dropped from every signature -- tree-wide convention.
//      (Golden's DoIniDataToForm / DoFormToData / ReadFile were already plain,
//      QAMode.h:55-57.)
//  (D-2) `TfQAMode` has NO base class (golden: `: public TForm`).  Matches
//      forms/fCounterSel.h (D-2) / forms/fBuilder.h (D-2).
//  (D-3) `Close()` is a PORT-ONLY no-op (`{}`) -- needed by ACTIVE btnOkClick.
//  (D-4) `FormClose(TObject*, TCloseAction&)` -> `void FormClose();`.
//      TCloseAction has no port and golden's body reads neither parameter.
//  (D-5) golden's other 21 `__published` widgets (btnApply, pgcQAMode,
//      tsQAMode, tsQASampling, gbQAMode, Label34, Label35, labQAModeBin,
//      gbLoaderDirection, Image1, pnlQASample, lblQASampleBin,
//      lblQASampleTray, Label2 ...) are NOT declared, and neither are
//      `Width`/`Height`: no ACTIVE body and no pre-committed site touches any
//      of them (minimal-declaration idiom -- forms/fSetup.h:20-25's explicit
//      refusal to grow a facade past measured need).  Their names appear only
//      inside `#if 0`.
//  (D-6) `edQAModeMouseDown(TObject*, TMouseButton, TShiftState, int, int)`
//      and `edtQASampleCntMouseDown(...)` -> single `(TObject *Sender)`.
//      TMouseButton and TShiftState have no port; golden's bodies read only
//      `Sender`.  Same reduction as forms/fBuilder.h (D-6).
//  (D-7) `QABackupStatus(bool)` IS declared here even though its body is gated
//      and it has ZERO call sites in this port (pre-flight [4b]: LIVE 0,
//      GATED 0).  Golden declares it in QAMode.h:63, so the facade's surface
//      matches golden's; an uncalled declaration costs nothing at link time.
//
//  ============================================================================
//  FIELD LIST (11 widgets + 1 bool)
//  ============================================================================
//    fShow (bool)                          golden QAMode.h:58, PRE-COMMITTED (2 sites)
//    edQAMode                   (TEdit*)       golden QAMode.h:25
//    rgQARunMode                (TRadioGroup*) golden QAMode.h:26
//    cbQAModeBin                (TComboBox*)   golden QAMode.h:27
//    cbTrayEndDoQAModeAgain     (TCheckBox*)   golden QAMode.h:30
//    cbTempOffset               (TCheckBox*)   golden QAMode.h:32
//    cbbQASampleBin             (TComboBox*)   golden QAMode.h:33
//    cbbQASampleTray            (TComboBox*)   golden QAMode.h:34
//    edtQASampleCnt             (TEdit*)       golden QAMode.h:38
//    cbTrayEndCloseYield100Site (TCheckBox*)   golden QAMode.h:39
//    cbQAD22Double              (TCheckBox*)   golden QAMode.h:40
//    cbbQAD22Double             (TComboBox*)   golden QAMode.h:41
//  All eleven are needed by ACTIVE DoFormToData.  NOT declared: see (D-5).
//
//  ============================================================================
//  ZERO-WRITER FIELDS (task rule: state the consequence)
//  ============================================================================
//  * `fShow` is written `false` by ACTIVE-PARTIAL FormClose and `true` ONLY by
//    GATED FormShow (:163).  CONSEQUENCE while (Q-3) stays gated: `fShow` is
//    permanently false, so the moment Command.cpp:10247 / :15035 are un-gated
//    the "is any form open" chain answers "this form is never open".  That is
//    a consequence of the safety boundary, not a defect here.
//  * Every widget in the FIELD LIST is WRITTEN only by GATED (Q-1)
//    DoIniDataToForm and READ by ACTIVE DoFormToData.  CONSEQUENCE: calling
//    DoFormToData today copies vclcompat's DEFAULTS (Text=="", ItemIndex==0,
//    Checked==false -- see vclcompat/Controls.h's DEFAULT-VALUE RULE) into
//    TestIF_File, i.e. it would ZERO the QA settings rather than preserve
//    them.  Nothing calls it in this port (`rg "DoFormToData"` outside this
//    pair -> other classes' members only), but this is exactly the shape
//    docs/KNOWLEDGE.md's "解gate前先查「值從哪來」" warns about: a future
//    wave must supply (Q-1) BEFORE wiring any caller to DoFormToData.
//
//  ============================================================================
//  DISCLOSURE (task rule 8 -- what was NOT read to completion / NOT verified)
//  ============================================================================
//  - `CheckAndReadIniData`'s seeding lines (common.cpp:613, :623-624) were NOT
//    re-read for this wave.  The (Q-4) gate quotes forms/fCounterSel.h's
//    (C-1), which DID read them in full one wave earlier, and the wave brief
//    states the same fact independently.  Two agreeing sources, zero fresh
//    reads -- stated so it is not mistaken for first-hand verification.
//  - `IntUnionBit` (golden :287, used by QABackupStatus) was NOT searched for
//    a port.  `fMain->ShowTestHeadComp` (golden :324) was NOT checked against
//    forms/fMain.h.  Both are inside GATE (Q-F), which is gated on safety
//    regardless of reachability.
//  - `Prod` / `s6TrayName[]` / `s3TrayName[]` / `iFixRight` / `e3TrayCount` /
//    `tTrayAuto` / `LevelSet` / `TrayForm.Loader.Direction` / `BmpPath` /
//    `CheckKeyExist` (all inside (Q-3)/(Q-4)/(Q-6)) were NOT resolved to
//    owning targets.  If a future wave opens (Q-3) or (Q-4) it must do that
//    work; this wave did not, because the safety gate decides those two
//    anyway.
//  - golden `QAMode.dfm` was NOT opened.  No ACTIVE body needs design-time
//    geometry, but note that every widget here starts at vclcompat's defaults,
//    NOT golden's .dfm values -- see the ZERO-WRITER consequence above.
// =============================================================================
#ifndef FORMS_FQAMODE_H
#define FORMS_FQAMODE_H

#include "forms/FormWidgets.h"   // AnsiString / TObject / TEdit / TComboBox /
                                 // TCheckBox / TRadioGroup (vclcompat)

// ===========================================================================
//  TfQAMode -- non-VCL facade (golden QAMode.h:13-59,
//  `class TfQAMode : public TForm`; NO base class here, DEVIATION D-2).
// ===========================================================================
class TfQAMode
{
public:
    TfQAMode();

    // -- golden public: User declarations ------------------------------------
    bool fShow = false;                                 // golden QAMode.h:58, PRE-COMMITTED (Command.cpp:10247, Command.cpp:15035)

    // -- golden __published widgets touched by an ACTIVE body ----------------
    TEdit       *edQAMode                   = new TEdit();        // golden QAMode.h:25
    TRadioGroup *rgQARunMode                = new TRadioGroup();  // golden QAMode.h:26
    TComboBox   *cbQAModeBin                = new TComboBox();    // golden QAMode.h:27
    TCheckBox   *cbTrayEndDoQAModeAgain     = new TCheckBox();    // golden QAMode.h:30
    TCheckBox   *cbTempOffset               = new TCheckBox();    // golden QAMode.h:32
    TComboBox   *cbbQASampleBin             = new TComboBox();    // golden QAMode.h:33
    TComboBox   *cbbQASampleTray            = new TComboBox();    // golden QAMode.h:34
    TEdit       *edtQASampleCnt             = new TEdit();        // golden QAMode.h:38
    TCheckBox   *cbTrayEndCloseYield100Site = new TCheckBox();    // golden QAMode.h:39
    TCheckBox   *cbQAD22Double              = new TCheckBox();    // golden QAMode.h:40
    TComboBox   *cbbQAD22Double             = new TComboBox();    // golden QAMode.h:41

    // -- ACTIVE this wave (defined in forms/fQAMode.cpp) ---------------------
    void btnOkClick(TObject *Sender);                   // golden :27-30    ACTIVE
    void edQAModeMouseDown(TObject *Sender);            // golden :32-36    ACTIVE, DEVIATION D-6
    void DoFormToData();                                // golden :56-71    ACTIVE
    void edtQASampleCntMouseDown(TObject *Sender);      // golden :279-283  ACTIVE, DEVIATION D-6
    void FormClose();                                   // golden :170-176  ACTIVE-PARTIAL (:173 -> GATE Q-1), DEVIATION D-4
    void Close() {}                                     // DEVIATION D-3, port-only no-op

    // -- GATED this wave -- declared here, golden body translated in
    //    forms/fQAMode.cpp inside `#if 0`.  Calling one is a LINK ERROR by
    //    design (see GATE REGISTER above). -------------------------------------
    void DoIniDataToForm();                             // golden :38-54    GATE (Q-1)
    void btnApplyClick(TObject *Sender);                // golden :73-110   GATE (Q-2)
    void FormShow(TObject *Sender);                     // golden :112-168  GATE (Q-3)
    void ReadFile();                                    // golden :178-232  GATE (Q-4)  ⚠ HIDDEN WRITE
    void Image1Click(TObject *Sender);                  // golden :234-253  GATE (Q-5)
    void ShowTrayDirectIMG();                           // golden :255-277  GATE (Q-6)
};

// AI(W906-FW3-BTQ1) 20260828: TfQAMode/fQAMode were FREE tree-wide (see the
// banner's measured provenance) -- declared here so the 2 pre-committed
// `#if 0` sites (Command.cpp:10247, Command.cpp:15035) and cBinSel.cpp:1414's
// own gate have something real to bind to once THEIR gates are separately
// lifted (NOT done by this wave).
extern TfQAMode *fQAMode;

// AI(W906-FW3-BTQ1) 20260828: golden QAMode.h:63 declares this file-scope
// helper alongside the class.  DEVIATION D-7 -- declared here for surface
// parity even though its BODY is GATE (Q-F) and no call site exists anywhere
// in this port (wave_preflight [4b]: LIVE 0, GATED 0, 20260828).
void QABackupStatus(bool bBackup);   //Sam 20231117 : 整合到 QA 模式

#endif // FORMS_FQAMODE_H
