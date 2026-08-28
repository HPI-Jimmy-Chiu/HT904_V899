// =============================================================================
//  forms/fLd_ULd.h  --  non-VCL facade for golden's TfLd_ULd (cLd_ULd.h)
//
//  AI(W906-FW3-DTL1) 20260828: new file, FW wave FW3-DTL1 (facade wave 1 of
//  cLd_ULd -- read-only direction, no write path).
//  GOLDEN SOURCE: HT9011UC_Code_V3.33.906.0_20260618/cLd_ULd.h (102 lines)
//  + cLd_ULd.cpp (262 lines), read with `io.open(p, encoding='cp950')`,
//  written out UTF-8 / bare-LF / 0 U+FFFD.
//
//  ROLE
//  ----
//  TfLd_ULd is golden's "Loader / Unloader Condition" setup dialog: nine
//  wait-time values for the loader and unloader lifters (tray-arrival, lock,
//  middle-separator close, before-down, down-check, tray-back, arrival-wait)
//  plus the knocker / vibration-motor page (P13 / P14 / P16 / F23 / F40).
//  Every one of those edits is bound to a backing variable through the SHARED
//  `elUdUld` HTEditList registry, and persisted to the file `UdUld.Data`
//  under `<DataPath>/<recipe>/`.
//
//  `python tools/census/wave_preflight.py cLd_ULd.cpp` (20260828, re-run at
//  closeout): 16 bodies = 16 `TfLd_ULd::` members + 0 file-scope, 222 span
//  lines, 0 brace-swallow short-fall, 0 overload collapse.
//
//  THIS WAVE, against the 16-member denominator:
//     7 ACTIVE           SetDefaultPos / edtLD_TrayArrivalDelyKeyPress /
//                        sbtExitClick / btnDefaultValueClick / edP40_1Click /
//                        edP40_2DblClick / edP40_3Click
//     1 ACTIVE-PARTIAL   the ctor -- golden :25 (`LastFileName="";`) is live,
//                        golden :26-84 (the 20 `elUdUld->Add(...)`
//                        registrations) is GATE (L-1)
//     7 GATED-WITH-BODY  FormShow / LoadImage / ReadFile / DoIniDataToForm /
//                        FormClose / spbSaveClick / SaveSetupFile, 110 golden
//                        span lines carried as `#if 0` transcript
//     1 OMITTED ENTIRELY FormShortCut -- `TWMKey` has no port, GATE (L-10)
//  BY LINES: the 7 fully-ACTIVE bodies' golden spans total 41 of 222 (18.5%);
//  the ctor contributes 1 live statement out of a 64-line span on top of
//  that.  Quote both numbers or neither.
//
//  ============================================================================
//  *** THE GLOBALS `TfLd_ULd` / `fLd_ULd` ARE FREE -- THIS WAVE CLAIMS THEM ***
//  Measured 20260828 with `python tools/census/wave_preflight.py cLd_ULd.cpp`
//  (section [3] SHIM OCCUPATION, 1195 port files scanned, build_*/vendor/
//  third_party excluded): (a) class name `clear`, (b) function name in
//  another class `clear`, (c) global name `clear`, (d) TU-local seam `clear`.
//
//  CONDITION-TWO CHECK (does a port of these bodies ALREADY exist, possibly
//  as a RENAMED free function outside the class?).  Searched 20260828, whole
//  port tree, build_* excluded:
//    * `rg "\b(Ld_ULd_|LdULd_|fLd_ULd_)\w+"`                       0 hits
//    * `SetDefaultPos`                                             0 files
//    * `LoaderCondition.bmp` (the literal LoadImage loads)          0 files
//    * `elUdUld`                                                    3 files --
//      Public/HTEditList.cpp:197 (the global's DEFINITION),
//      Public/HTEditList.h:248 (its extern) and cConfiguration.cpp:4475-4476
//      + :4970.  ⚠ READ THIS BEFORE CONCLUDING: cConfiguration.cpp:4475-4476
//      are TWO `elUdUld->Add(edP16_1/edP16_2, &IniConfig.iP16Hotplate...)`
//      lines that look almost exactly like golden cLd_ULd.cpp:76-77/:81-82 --
//      but they belong to `TfConfiguration`'s OWN registration pass, not to a
//      relocated `TfLd_ULd::TfLd_ULd`.  Golden has the same near-duplicate
//      pair in cConfiguration.cpp itself, so this is golden's own redundancy
//      and NOT an already-existing port of this ctor.  The other 18
//      registrations of golden's ctor have no counterpart anywhere.
//    * `UdUld.Data`                                                 2 files --
//      Automation/auto9045.cpp:2441-2583 and SECSGEM/uHGemHT9045.cpp:1217-1693.
//      Both COPY/BACKUP the file (recipe download / recipe switch); neither
//      reads or writes its CONTENT, and neither is a `TfLd_ULd::` body.
//    * `Ld_UldDelayTime`                                           22 files --
//      the shared struct this form edits; consumed all over the loader /
//      unloader state machines.  None is a port of a body here.
//  Conclusion: nothing to re-use, nothing re-translated twice.
//
//  ============================================================================
//  PRE-COMMITTED #if 0 CALL SITES ELSEWHERE IN THE TREE (3 names, 4 sites)
//  ============================================================================
//    DoIniDataToForm()  csystem.cpp:29302  -- matches this wave's GATED
//                                             `void DoIniDataToForm();`
//    SaveSetupFile(...) csystem.cpp:29303  -- matches this wave's GATED
//                                             `void SaveSetupFile(AnsiString,
//                                             AnsiString);`
//    fShow              Command.cpp:10246, Command.cpp:15034
//                                          -- matches `bool fShow;`
//  ⚠ Command.cpp:10246 and :15034 are each ONE line naming BOTH
//  `fLd_ULd->fShow` AND `fTowerLight->fShow` (a long "is ANY form open" OR
//  chain).  Not a duplicate report -- checked against the source.
//  ⚠ The SAME chain spells `fCCLink->bShow` with a **b**.  Do not normalise.
//  All 4 sites stay `#if 0` after this wave.
//
//  ============================================================================
//  GATE REGISTER (8: 7 with a transcript + 1 omitted entirely)
//  ============================================================================
//  (L-1) the ctor's golden :26-84 (59 of its 64 span lines) -- TWO
//        INDEPENDENT GATES.
//        (a) LINK BOUNDARY.  All 20 statements are `elUdUld->Add(...)`.
//            `elUdUld` is `extern HTEditList *elUdUld`
//            (Public/HTEditList.h:248) and its DEFINING TU is
//            Public/HTEditList.cpp (`elUdUld` at :197), whose enclosing
//            `add_library` block is **ht9045_sm** (located this wave by
//            walking the enclosing add_library() with a script; the TARGET
//            NAME is quoted rather than a line number, because line numbers
//            move whenever a sibling wave adds a file).  `ht9045_forms` links
//            only `vclcompat` + `ht9045_globals` + `ht9045_core` -- ht9045_sm
//            is unreachable from here, and `HTEditList::Add` is an
//            out-of-line member, so this is a real link edge, not a header
//            dependency.
//        (b) STATIC-INIT SAFETY.  This facade is created by a file-scope
//            `new` (`TfLd_ULd *fLd_ULd = new TfLd_ULd();`), so its ctor runs
//            BEFORE `main()`.  `elUdUld` is a NULL global pointer at that
//            point (Public/HTEditList.cpp:197 has no initialiser), so even if
//            (a) were solved every one of these 20 lines would be a NULL
//            dereference during static initialisation.  This is the exact
//            failure docs/KNOWLEDGE.md records for `fLaserSensor` (88 of 134
//            ctest binaries turned into SEGFAULTs) and for `TfMonitor`'s
//            `LoadTCPIPParament()`.  Golden is safe because VCL constructs
//            its forms inside `Application->CreateForm`, i.e. after
//            everything is initialised.
//        The ctor's remaining line, golden :25 `LastFileName="";`, is an own
//        field and IS live (ACTIVE-PARTIAL -- same shape as
//        forms/fTesterTCP.cpp's ctor).  Golden :26-28's three locals
//        (`bShow`/`bEnable`/`bReadFromFile`) exist only to feed the gated
//        Add() calls and go with them.
//  (L-2) FormShow(TObject*)   golden :87-129 (43L) -- TRANSITIVE, on TWO
//        gated callees, both unconditional and both in the first four lines:
//        `LoadImage();` (:90, GATE L-4) and `ReadFile();` (:92, GATE L-5).
//        Recorded for a future wave: everything AFTER those two is reachable
//        today -- `GetLastOpenFN()` (common.cpp:1317, `ht9045_core`),
//        `fSecurity->Insufficient(115,false)` (forms/fSecurity.h:561,
//        `ht9045_forms`), `CosFunction.bKnockerSetBySetupFile`
//        (CosFunction.cpp, `ht9045_globals`), `TRAY_VIBRATION` /
//        `VibrationMotor` / `USE_LOADER_VIBRATION` /
//        `ShuttleVibration_INSTALL` (cmydef.cpp, same), `SetDefaultPos()`
//        (this wave's own ACTIVE body).  So (L-4)+(L-5) are the whole
//        blocker.
//  (L-3) FormClose()          golden :170-177 (8L) -- TRANSITIVE via
//        `ReadFile();` (:172, GATE L-5), its FIRST statement.  `rbTemp->
//        SetFocus();` (:173) would itself be fine -- `SetFocus()` is a real
//        offline no-op on vclcompat::TControl -- and `fShow=false;` (:174) is
//        an own field.  Same "a method that calls a gated method is itself
//        gated" transitive rule as every prior facade wave.
//  (L-4) LoadImage() (private) golden :137-141 (5L) -- MISSING TYPE.
//        `Image1`/`Image2` are golden `TImage*` and the body is
//        `->Picture->LoadFromFile(BmpPath+"...bmp")`.  vclcompat has no
//        `TImage` and no `TPicture` (0 hits in vclcompat/, 20260828) -- same
//        established gap as forms/fHotPlate.h GATE (G-2) and
//        forms/fContact.h's own TImage/TPicture note.  Declared (private,
//        matching golden visibility), body transcribed, not compiled.
//  (L-5) ReadFile()           golden :143-154 (12L) -- LINK BOUNDARY, same
//        `elUdUld` reason as (L-1)(a): `elUdUld->ReadEditTextFromFile(szDir,
//        "UdUld.Data")` and `->InitialDataToEdit()` (:151-152).  The
//        surrounding lines (`GetLastOpenFN()`, `szDir.sprintf("%s%s\\",
//        DataPath, ...)`) are reachable -- the two Add-registry calls are the
//        whole blocker.  NOT split: an `elUdUld==NULL`-guarded stub that
//        silently loads nothing would be an invented behaviour.
//  (L-6) DoIniDataToForm()    golden :156-162 (7L) -- LINK BOUNDARY, same
//        `elUdUld` reason.  The ENTIRE body is
//        `if(elUdUld!=NULL){ elUdUld->InitialDataToEdit(); }`.
//        ⚠ Note the NULL guard is golden's OWN -- so the null-pointer half of
//        (L-1)(b) is already handled here by golden; what still blocks this
//        one is purely (L-1)(a), the ht9045_sm link edge on
//        `HTEditList::InitialDataToEdit`.  PRE-COMMITTED name
//        (csystem.cpp:29302).
//  (L-7) spbSaveClick(TObject*) golden :179-202 (24L) -- THREE INDEPENDENT
//        GATES.
//        (a) TRANSITIVE / WRITES DISK: `SaveSetupFile(szDir, LastFileName);`
//            (:191, GATE L-8) unconditionally.
//        (b) LINK BOUNDARY: `EventReport(SECS_EVENT.SaveRecipe);` (:200) --
//            `EventReport` is real (SECSGEM/SecsEventReport.cpp:15) but its
//            `add_library` block is **ht9045_secsgem**, which ht9045_forms
//            does not link (the dependency runs the OTHER way:
//            ht9045_secsgem links ht9045_forms).
//            ⚠ ABSENCE-CLAIM PRECISION: `EventReport` also exists as a
//            MEMBER, `THGem::EventReport(unsigned,unsigned)`
//            (SECSGEM/uHGemEquipment.cpp:7580) -- a different arity, a
//            different class, and the same unreachable target.  Neither
//            spelling helps ht9045_forms.
//        (c) MISSING SYMBOL, inside `#ifdef ASE_KaohSiung`: `fBuilder->
//            bSaveAsJobFile(...)` (:193) -- `fBuilder` has NO declaration
//            anywhere in this port (20260828: the only `.h` hit is a comment,
//            forms/fHotPlate.h:231, which is that file's OWN identical note;
//            Command.cpp:10248/:15036 do name `fBuilder->fShow`, but inside
//            their own `#if 0`).  Dead in a standard build, recorded for
//            completeness.
//        `IniConfig.bA02DisableSaveParsWhenSwitchToOp` / `AccessLevel` /
//        `ShowMyMessage` / `bHasSaveSet` at the top of the body are all
//        reachable -- they are not what blocks it.
//  (L-8) SaveSetupFile(AnsiString,AnsiString) golden :206-216 (11L)
//        -- TWO INDEPENDENT GATES.
//        (a) WRITES DISK.  `MyForceDirectories(szDir)` CREATES DIRECTORIES
//            (common.cpp:1888) and `elUdUld->SaveEditTextToFile(szDir,
//            "UdUld.Data")` serialises the recipe's loader/unloader timing
//            block.  The name itself says "save" -- read in full before
//            deciding, per the task rule; the read confirms it.
//        (b) LINK BOUNDARY: the same `elUdUld` edge as (L-1)(a), PLUS
//            `fMain->BackupSetupFile();` (:215) which -- unusually -- IS
//            reachable (forms/fMain.h:260 declares it, forms/fMain.cpp is
//            `ht9045_forms`).  Recorded so a future wave does not mistake
//            fMain for the blocker.
//        PRE-COMMITTED name and signature (csystem.cpp:29303).
//  (L-9) -- (intentionally unused; kept so the (L-10) label below matches the
//        numbering used in this wave's report.)
//  (L-10) FormShortCut(TWMKey&, bool&) golden :239-245 (7L) -- OMITTED
//        ENTIRELY, not even declared.  `TWMKey` has ZERO port anywhere in
//        this tree (`rg "TWMKey" -g '*.h'` 20260828 -> only comment lines in
//        forms/fContact.h:489/:758/:774/:1309, which are that file's OWN
//        identical gate).  Same established precedent as forms/fHotPlate.h
//        GATE (G-12) / forms/fContact.h GATE (X-27) / forms/fSetup.h /
//        forms/fTemp_Set.h.  A declaration would have to invent the type,
//        which is outside this wave's write boundary.  Golden's body is
//        `if(Msg.CharCode==VK_F5) SetDefaultPos();` -- i.e. the ONLY thing it
//        does is call this wave's own ACTIVE `SetDefaultPos()`, so nothing of
//        substance is lost, only the key binding.
//
//  ============================================================================
//  ACTIVE (8, one of them PARTIAL) -- reachability evidence
//  ============================================================================
//   TfLd_ULd()               golden :22-85, ACTIVE-PARTIAL -- only :25
//        `LastFileName="";` (own field).  See GATE (L-1) for the rest.
//        SIOF-safe as delivered: the live remainder touches no global.
//   SetDefaultPos()          golden :131-135 -- `Left=150; Top=10;`, own
//        fields (DEVIATION D-2 makes them plain ints).  Note golden declares
//        this one WITHOUT `__fastcall` (cLd_ULd.h:96), unlike its siblings.
//   edtLD_TrayArrivalDelyKeyPress(TObject*, char&)  golden :164-168 --
//        `OnlyNumberAndDotInPut` (common.cpp:1436, `ht9045_core`, which
//        ht9045_forms links -- see CMakeLists' own AI(W906-FW-SETUP-C2) note
//        on that edge).
//   sbtExitClick(TObject*)   golden :218-222 -- `sbtExit->Down=false;` (own
//        vclcompat::TSpeedButton) + `Close();` (DEVIATION D-4).
//   btnDefaultValueClick(TObject*) golden :224-237 (14L) -- eight
//        `edt*->Text = <number>;` assignments, nothing else.  vclcompat's
//        AnsiString carries `operator=(double)` and `operator=(int)`
//        (vclcompat/AnsiString.h:97/:96), which is exactly golden's BCB6
//        semantics, so `->Text=0.2;` formats rather than truncates.
//        ⚠ This body sets only EIGHT of the nine loader/unloader edits --
//        golden never resets `edtULD_TrayArrivalWait` or
//        `edtLD_BeforeDownDelay`.  Verbatim, not "fixed".
//   edP40_1Click / edP40_2DblClick / edP40_3Click (TObject*)
//        golden :248-251 / :253-256 / :258-261 -- `fQwertyKey->
//        ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 1, <10|20|15>)`.
//        `fQwertyKey` is forms/fQwertyKey.h:406 (forms/fQwertyKey.cpp,
//        `ht9045_forms` -- same target); `ShowQwertyKey`'s ported signature
//        (forms/fQwertyKey.h:370) takes `TControl*` and matches golden's
//        6-argument form; `N_INTEGER` is `extern const int` (cmydef.h:289).
//        Same call shape as the already-landed forms/fHotPlate.cpp:109.
//        ⚠ Golden's own oddity, kept: `edP40_2` is wired to a **DblClick**
//        handler while its two neighbours use **Click** (golden
//        cLd_ULd.h:84-86).
//
//  ============================================================================
//  DEVIATIONS (port-only; each with why)
//  ============================================================================
//  (D-1) `__fastcall` dropped from every signature -- tree-wide convention.
//  (D-2) `TfLd_ULd` has NO base class (golden: `: public TForm`).  Matches
//      forms/fCleaning.h / forms/fHotPlate.h (D-2) / forms/fPrecaution.h
//      (D-4).  `Left`/`Top`/`Caption` therefore become plain members; only
//      `Left`/`Top` are declared (Caption is written solely by the GATED
//      FormShow).  Same shape as forms/fTrayAssignment.h:678-679.
//  (D-3) `FormClose(TObject*, TCloseAction&)` -> `void FormClose();`.
//      `TCloseAction` has no port; golden's body reads neither parameter
//      (its Do_Log line is already `//`-commented out in golden itself,
//      :176).  Precedent forms/fMesSystem.h D-5 / forms/fHotPlate.h (:281).
//  (D-4) `Close()` is a PORT-ONLY no-op (`{}`) -- golden's own inherited
//      `TForm::Close`, which D-2 removes.  Needed by ACTIVE sbtExitClick and
//      by the GATED spbSaveClick transcript.
//  (D-5) `FormShortCut` OMITTED ENTIRELY -- see GATE (L-10).
//  (D-6) `LoadImage` kept `private` (matching golden cLd_ULd.h:89) even
//      though GATED -- declared-not-defined either way, visibility is free.
//  (D-7) Golden's other 50-odd `__published` widgets (Panel1/2/3,
//      PageControl1, TabSheet1, tsKnocker, grpLoader, grpUnloader, Image1/2,
//      Label*, lblUnit, palKnocker, labP13/P14/P16/F40*, cbP13/P14/P16/F23/
//      F40, edP13_*/edP14_*/edP16_*/edF23/edP40_*, rbTemp, spbSave,
//      btnDefaultValue) are NOT declared -- no ACTIVE body and no
//      pre-committed site touches them (minimal-declaration idiom, matches
//      forms/fHotPlate.h's own FIELD LIST practice).  Their names appear only
//      inside the `#if 0` transcripts, which never compile.
//
//  ============================================================================
//  FIELD LIST (9 widgets + 1 bool + 1 AnsiString + 2 ints)
//  ============================================================================
//    fShow (bool)                              -- golden :97, PRE-COMMITTED
//    LastFileName (AnsiString, private)        -- golden :88, ACTIVE ctor
//    Left / Top (int)                          -- SetDefaultPos writes 150/10
//    sbtExit (TSpeedButton*)                   -- golden :19, sbtExitClick
//    edtLD_TrayArrivalDely / edtLD_FixTrayDely / edtLD_MiddLockDelay /
//      edtLD_LiftDownDelay / edtULD_TrayArrivalDelay / edtULD_FixTrayDely /
//      edtULD_TrayBackDelay / edtULD_LiftDownDelay (TEdit*)
//                                              -- btnDefaultValueClick
//  NOT declared: see DEVIATION (D-7).
//
//  ============================================================================
//  ZERO-WRITER FIELDS / VALUE PROVENANCE
//  ============================================================================
//  `fShow` is written ONLY by the GATED FormShow (:126 true) and the GATED
//  FormClose (:174 false), so it is permanently `false` while (L-2)/(L-3)
//  stand.  That is observable at the 2 pre-committed Command.cpp sites the
//  moment THEY are un-gated: the "is any form open" chain would answer "this
//  form is never open".  Consequence of the link boundary, not a bug here.
//  `LastFileName` is set to "" by the ACTIVE ctor and re-set from
//  `GetLastOpenFN()` only inside the GATED FormShow/ReadFile, so it stays ""
//  -- which is exactly why the GATED SaveSetupFile must not be un-gated
//  casually: with an empty recipe name its `szDir` would collapse to
//  `DataPath` itself.
//  The eight declared `TEdit*`s are written by ACTIVE btnDefaultValueClick
//  and read by nobody in this port (golden's reader is `elUdUld`, GATE L-1).
//  `Left`/`Top` are recorded values only -- vclcompat::TControl's own note is
//  explicit that Left/Top只存值, nothing moves.
//
//  ============================================================================
//  DISCLOSURE (task rule 8 -- what was NOT read to completion / NOT verified)
//  ============================================================================
//  - `HTEditList`'s own API (Add/ReadEditTextFromFile/InitialDataToEdit/
//    SaveEditTextToFile) was NOT read.  Only the owning `add_library`
//    (`ht9045_sm`) and the global's definition site
//    (Public/HTEditList.cpp:197) were established -- which is all (L-1)(a) /
//    (L-5) / (L-6) / (L-8)(b) rest on.  A future ht9045_sm-reaching wave must
//    re-verify that the 20 golden `Add(...)` overloads actually exist with
//    those argument shapes before un-gating the ctor.
//  - The 20 backing variables golden's ctor registers
//    (`Ld_UldDelayTime.*`, `IniConfig.bP13*`/`iP14*`/`iP16*`/
//    `bF23ShuttleVibration`, `LastSet.bEnableAutoTray*`) were NOT
//    individually confirmed to exist in the port headers -- moot while
//    (L-1) stands, but they are the second thing to check when it is lifted.
//  - `fSecurity->Insufficient(115,false)` / `(116,false)` were confirmed to
//    EXIST (forms/fSecurity.h:561) but their permission-table provenance was
//    NOT traced; forms/fSecurity.h's own banner warns that the table
//    `Insufficient()` indexes is populated in a ctor whose CTOR SAFETY note
//    should be read first.  Moot -- FormShow is gated on (L-4)/(L-5).
//  - golden `cLd_ULd.dfm` was NOT opened; no design-time geometry or Tag is
//    supplied to any declared field.  `SetDefaultPos` writes Left/Top itself,
//    so no ACTIVE body depends on it.
// =============================================================================
#ifndef FORMS_FLD_ULD_H
#define FORMS_FLD_ULD_H

#include "forms/FormWidgets.h"   // AnsiString / TObject / TEdit / TSpeedButton (vclcompat)

// ===========================================================================
//  TfLd_ULd -- non-VCL facade (golden cLd_ULd.h:14-98,
//  `class TfLd_ULd : public TForm`; NO base class here, DEVIATION D-2).
// ===========================================================================
class TfLd_ULd
{
public:
    TfLd_ULd();

    // -- golden public: User declarations ------------------------------------
    bool fShow = false;                                   // golden :97, PRE-COMMITTED (Command.cpp:10246, Command.cpp:15034)

    // -- DEVIATION D-2: golden inherited these from TForm ---------------------
    int Left = 0;                                         // SetDefaultPos golden :133 writes 150
    int Top  = 0;                                         // SetDefaultPos golden :134 writes 10

    // -- golden __published widgets touched by an ACTIVE body -----------------
    TSpeedButton *sbtExit = new TSpeedButton();            // golden :19

    TEdit *edtLD_TrayArrivalDely   = new TEdit();          // golden :32
    TEdit *edtLD_FixTrayDely       = new TEdit();          // golden :33
    TEdit *edtLD_MiddLockDelay     = new TEdit();          // golden :34
    TEdit *edtLD_LiftDownDelay     = new TEdit();          // golden :35
    TEdit *edtULD_TrayArrivalDelay = new TEdit();          // golden :43
    TEdit *edtULD_FixTrayDely      = new TEdit();          // golden :44
    TEdit *edtULD_TrayBackDelay    = new TEdit();          // golden :45
    TEdit *edtULD_LiftDownDelay    = new TEdit();          // golden :46

    // -- ACTIVE this wave (defined in forms/fLd_ULd.cpp) ----------------------
    void SetDefaultPos();                                  // golden :131-135  ACTIVE (golden has no __fastcall here either)
    void edtLD_TrayArrivalDelyKeyPress(TObject *Sender, char &Key);  // golden :164-168  ACTIVE
    void sbtExitClick(TObject *Sender);                    // golden :218-222  ACTIVE
    void btnDefaultValueClick(TObject *Sender);            // golden :224-237  ACTIVE
    void edP40_1Click(TObject *Sender);                    // golden :248-251  ACTIVE, JerryYang 20191001 loader跛動馬達
    void edP40_2DblClick(TObject *Sender);                 // golden :253-256  ACTIVE, ditto (golden really is DblClick here)
    void edP40_3Click(TObject *Sender);                    // golden :258-261  ACTIVE, ditto
    void Close() {}                                        // DEVIATION D-4, port-only no-op

    // -- GATED this wave -- declared here, golden body translated in
    //    forms/fLd_ULd.cpp inside `#if 0`.  Calling one is a LINK ERROR by
    //    design (see GATE REGISTER above). -----------------------------------
    void FormShow(TObject *Sender);                        // golden :87-129   GATE (L-2)
    void FormClose();                                      // golden :170-177  GATE (L-3), DEVIATION D-3
    void ReadFile();                                       // golden :143-154  GATE (L-5)
    void DoIniDataToForm();                                // golden :156-162  GATE (L-6), PRE-COMMITTED (csystem.cpp:29302)
    void spbSaveClick(TObject *Sender);                    // golden :179-202  GATE (L-7)
    void SaveSetupFile(AnsiString szDir, AnsiString S);    // golden :206-216  GATE (L-8), PRE-COMMITTED (csystem.cpp:29303)

private:
    AnsiString LastFileName;                               // golden :88, written by the ACTIVE ctor
    void LoadImage();                                      // golden :137-141  GATE (L-4), private (matches golden)

    // FormShortCut(TWMKey&, bool&) -- OMITTED ENTIRELY, see GATE (L-10) /
    // DEVIATION D-5.
};

// AI(W906-FW3-DTL1) 20260828: TfLd_ULd/fLd_ULd were FREE tree-wide (see
// banner's measured provenance) -- declared here so the 4 pre-committed
// `#if 0` sites (csystem.cpp:29302, csystem.cpp:29303, Command.cpp:10246,
// Command.cpp:15034) have something real to bind to once each owning file's
// OWN gate is separately lifted (NOT done by this wave).
extern TfLd_ULd *fLd_ULd;

#endif // FORMS_FLD_ULD_H
