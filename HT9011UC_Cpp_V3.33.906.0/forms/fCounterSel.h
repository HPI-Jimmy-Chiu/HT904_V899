// =============================================================================
//  forms/fCounterSel.h  --  non-VCL facade for golden's TfCounterSel
//                            (cCounterSel.h)
//
//  AI(W906-FW3-DTL1) 20260828: new file, FW wave FW3-DTL1 (facade wave 1 of
//  cCounterSel -- read-only direction, no write path).
//  GOLDEN SOURCE: HT9011UC_Code_V3.33.906.0_20260618/cCounterSel.h (62 lines)
//  + cCounterSel.cpp (150 lines), read with `io.open(p, encoding='cp950')`,
//  written out UTF-8 / bare-LF / 0 U+FFFD.
//
//  ROLE
//  ----
//  TfCounterSel is golden's "which counters does fMain show" dialog: 11
//  on/off switches (UPH, index time, contact height, cycle time, loading
//  count, contact count, test category, scanner, temperature, bin assign,
//  category-by-arm) that map 1:1 onto `IniConfig.bShow*` / `iShowCateByArm`
//  and are persisted into `D:\HT9045\config\config.ini` by
//  `ProcessLastSetIni_Visible()`.  It also owns the "restore default window
//  positions" button, which rewrites `config\FormPos.def`.
//
//  `python tools/census/wave_preflight.py cCounterSel.cpp` (20260828, re-run
//  at closeout): 5 bodies = 5 `TfCounterSel::` members + 0 file-scope,
//  123 span lines, 0 brace-swallow short-fall, 0 overload collapse.
//
//  THIS WAVE, against the 5-member denominator:
//     2 ACTIVE           ctor / spbExitClick
//     3 GATED-WITH-BODY  FormShow / FormClose / CheckFormIni, 113 golden span
//                        lines carried as `#if 0` transcript in
//                        forms/fCounterSel.cpp
//  BY LINES: the 2 ACTIVE bodies are 10 of 123 golden span lines (8.1%).
//  ⚠ This is a THIN wave by design, not by omission: this form's whole job is
//  to persist settings, so all three substantial bodies land on the
//  read-only-direction rule.  Quote both numbers or neither.
//
//  ============================================================================
//  *** THE GLOBALS `TfCounterSel` / `fCounterSel` ARE FREE -- THIS WAVE
//  CLAIMS THEM ***
//  Measured 20260828 with `python tools/census/wave_preflight.py
//  cCounterSel.cpp` (section [3] SHIM OCCUPATION, 1195 port files scanned,
//  build_*/vendor/third_party excluded): (a) class name `clear`, (b) function
//  name in another class `clear`, (c) global name `clear`, (d) TU-local seam
//  `clear`.  Cross-checked the same day with
//  `rg -c -g '!build_*' -g '*.h' -g '*.cpp' "\bTfCounterSel\b"` -- 0 files.
//
//  CONDITION-TWO CHECK (does a port of these bodies ALREADY exist, possibly
//  as a RENAMED free function outside the class?).  Searched 20260828, whole
//  port tree, build_* excluded:
//    * `rg "\b(CounterSel_|fCounterSel_)\w+"`                       0 hits
//    * `CheckFormIni`                                               0 files
//    * `FormPos.def` (the literal CheckFormIni writes)              0 files
//    * `bShowLoaderCT` / `bShowContactCT` / `iShowCateByArm`        2 files
//      each -- both are Config.h (the declaration) and cprod.cpp (the
//      `ProcessLastSetIni_Visible` persistence loop).  Neither is a port of a
//      `TfCounterSel::` body: cprod.cpp holds the SHARED persistence helper
//      this form CALLS, which is a different function with a different name
//      and already had its own port long before this wave.
//    * `ProcessLastSetIni_Visible`                                  2 files
//      (cprod.h:3271 decl, cprod.cpp:2824 def) -- the helper, not a body.
//  Conclusion: nothing to re-use, nothing re-translated twice.
//
//  ============================================================================
//  PRE-COMMITTED #if 0 CALL SITES ELSEWHERE IN THE TREE (1 name, 2 sites)
//  ============================================================================
//    fShow    Command.cpp:10247, Command.cpp:15035
//             -- matches this facade's `bool fShow;`.
//  ⚠ These two lines sit one line BELOW the pair (Command.cpp:10246 / :15034)
//  that carries `fLd_ULd->fShow` AND `fTowerLight->fShow` on a single line.
//  Same long "is ANY form open" OR chain, and the same chain spells
//  `fCCLink->bShow` with a **b**.  Do not normalise any of them.
//  Both sites stay `#if 0` after this wave.
//
//  ============================================================================
//  GATE REGISTER (3)
//  ============================================================================
//  (C-1) FormShow(TObject*)  golden :29-54 (26L) -- HIDDEN WRITE.
//        Line :31 `ProcessLastSetIni_Visible(bReadFile);` is the FIRST
//        statement and is unconditional.  That helper is REAL and REACHABLE
//        (cprod.cpp:2824; cprod.cpp's `add_library` block is
//        **ht9045_globals**, which ht9045_forms links) -- so this is NOT a
//        link-boundary gate, it is the hidden-write trap:
//          cprod.cpp:2824 calls `ReadWriteIni(sPath,"Visible",...,bRead)`
//          11 times -> common.cpp:1599 with `bIsRead==true` calls
//          `CheckAndReadIniData(...)` -> common.cpp:623-624
//              `if (!INIFile->ValueExists(Group, Name))
//                   INIFile->WriteBool(Group, Name, Value);   // seed`
//        i.e. a *read* call SEEDS `AuthPath+"config.ini"`
//        (= `D:\HT9045\config\config.ini`, a shared runtime-parameter file)
//        whenever a key is absent.  This is the SAME function-pair trap
//        forms/fHotPlate.h GATE (G-1) documents: `CheckAndReadIniData`
//        (common.cpp:613) writes back, the similarly-named pure-read
//        `ReadIniData` does not.
//        The other 25 lines are pure widget reflection off `IniConfig.bShow*`
//        (Config.cpp, ht9045_globals -- all reachable).  NOT split: dropping
//        :31 would silently change which values the form displays, which is
//        an invented behaviour, not a translation (忠實優先).
//  (C-2) FormClose()         golden :56-109 (54L) -- FOUR INDEPENDENT GATES.
//        (a) WRITES DISK, hidden-write side.  :106
//            `ProcessLastSetIni_Visible(bWriteFile);` -- with `bIsRead==false`
//            common.cpp:1609 calls `WriteIniData(FileName,Group,Name,tValue)`
//            UNCONDITIONALLY, 11 times, into `config\config.ini`.
//        (b) WRITES DISK, explicit side.  :91 calls this file's own
//            `CheckFormIni(AuthPath, "FormPos.def")` (GATE C-3, which
//            MyForceDirectories + writes 24 keys) and :92-103 then call
//            `ReplaceIniData(szDir, ...)` TWELVE times (common.cpp:1298).
//            Both helpers are REACHABLE (common.cpp, `ht9045_core`) -- again
//            a safety gate, not a link gate.
//        (c) MISSING MEMBERS on OTHER facades.  Verified 20260828 by reading
//            the port headers, not by grepping the class name:
//              :73 `fMain->StatusBar1->Panels->Items[0]->Text=""`
//                  -- `fMain` IS real and live (forms/fMain.cpp:404,
//                     ht9045_forms, same target as this facade), but
//                     forms/fMain.h declares NO `StatusBar1`
//                     (`rg "StatusBar1" forms/fMain.h` -> 0 hits; the same
//                     probe DID find `BackupSetupFile` at forms/fMain.h:260,
//                     so the probe itself is sound).  INDEPENDENTLY
//                     CORROBORATED: ainarm9045.cpp:7385-7402 already carries
//                     its own gate for the identical expression, quoting
//                     "forms/fMain.h has no StatusBar1 member".
//              :81-82 `fMain->lbArm0Torque->Caption=""` -- same, 0 hits in
//                     forms/fMain.h.  ⚠ TRAP: `rg "\blbArm0Torque\b"`
//                     tree-wide DOES hit atester.cpp:5563
//                     (`TPanel lbArm0Torque, lbArm1Torque;`) -- that is a
//                     TU-LOCAL stand-in struct inside atester.cpp
//                     (archive-extraction shape (d)/(e)), NOT a member of the
//                     real fMain facade, and it satisfies nothing for
//                     ht9045_forms.
//                  ⚠ golden writes this line TWICE, identically.  That is
//                     golden's own copy/paste (the second was almost
//                     certainly meant to be `lbArm1Torque`).  Transcribed
//                     verbatim below; NOT "fixed" -- 改行為要留給使用者決定.
//              :74-75 `fShowMessage->lblIndexCycleTime->Visible` /
//                     `->lblTestTime->Visible` -- `TfShowMessage` is real
//                     (forms/fShowMessage.cpp, ht9045_forms) but declares
//                     neither member (0 hits).  ⚠ TRAP: both names DO appear
//                     tree-wide, in tools/dfm2rc/layout_out/
//                     uShowMessage_layout.gen.cpp:13-14 -- that is generated
//                     .dfm DATA (a row in a widget table), not a C++ member,
//                     and nothing in this port loads a layout.
//        (d) NOT a blocker, recorded so a future wave does not re-derive it:
//            :77 `fShowBinSelect->Tab_UPH->TabVisible` DOES resolve --
//            forms/fShowBinSelect.h:966 `TTabSheet *Tab_UPH`, extern at
//            :1133, same `ht9045_forms` target.
//  (C-3) CheckFormIni(AnsiString,AnsiString)  golden :111-143 (33L)
//        -- WRITES DISK.  `MyForceDirectories(szDir)` (common.cpp:1888)
//        CREATES DIRECTORIES, then, when `!FileExists(szDir)`, 24 x
//        `WriteIniData(szDir, "<form>", "X"/"Y"/"defaultX"/"defaultY", n)`
//        (common.cpp:958) seed `config\FormPos.def` with the factory window
//        layout.  Both helpers are reachable (`ht9045_core` /
//        `vclcompat` FileExists) -- pure safety gate.  Note the golden body's
//        `return szDir;` is what FormClose feeds to ReplaceIniData, so
//        (C-2)(b) depends on this one.
//
//  ============================================================================
//  ACTIVE (2) -- reachability evidence
//  ============================================================================
//   TfCounterSel()          golden :23-27 -- `NeedRef=false;` only, own field,
//        no global deref, no call.  SIOF-safe (docs/KNOWLEDGE.md "static-init
//        ctor 不可碰 NULL 全域").
//   spbExitClick(TObject*)  golden :145-149 -- `spbExit->Down=false;` (own
//        vclcompat::TSpeedButton, `Down` at vclcompat/Controls.h:487) +
//        `Close();` (DEVIATION D-3, port-only no-op), making this a FULL
//        translation rather than a partial one.
//
//  ============================================================================
//  DEVIATIONS (port-only; each with why)
//  ============================================================================
//  (D-1) `__fastcall` dropped from every signature -- tree-wide convention.
//  (D-2) `TfCounterSel` has NO base class (golden: `: public TForm`).
//      Matches forms/fCleaning.h / forms/fHotPlate.h (D-2) /
//      forms/fPrecaution.h (D-4).
//  (D-3) `Close()` is a PORT-ONLY no-op (`{}`) -- golden's own inherited
//      `TForm::Close`, which D-2 removes.  Needed by ACTIVE spbExitClick.
//  (D-4) `FormClose(TObject*, TCloseAction&)` -> `void FormClose();`.
//      `TCloseAction` has no port anywhere in this tree; golden's body reads
//      NEITHER parameter.  Established precedent forms/fMesSystem.h D-5 /
//      forms/fHotPlate.h (:281) / forms/fPrecaution.h (D-5).
//  (D-5) Golden's 27 other `__published` widgets (GroupBox17, gbLoadingCount,
//      gbContactCount, gbTestCategory, gbScanner, gbTemperature, gbBinAssign,
//      gbStatus, labLoadingCount/labContactCount/labTestCategory/labScanner/
//      labTemperature/labBinAssign/labStatus, and the 11 rb*/cb* controls
//      + rgTestCategory) are NOT declared -- no ACTIVE body and no
//      pre-committed site touches them (minimal-declaration idiom, matches
//      forms/fHotPlate.h's own FIELD LIST practice).  Their names are all
//      inside the `#if 0` transcripts, which never compile.
//
//  ============================================================================
//  FIELD LIST (1 widget + 2 bools)
//  ============================================================================
//    NeedRef (bool)                      -- golden :56, set by ACTIVE ctor
//    fShow   (bool)                      -- golden :57, PRE-COMMITTED (2 sites)
//    spbExit (TSpeedButton*)             -- golden :48, ACTIVE spbExitClick
//  NOT declared: see DEVIATION (D-5).
//
//  ============================================================================
//  ZERO-WRITER FIELDS
//  ============================================================================
//  `NeedRef` is written by the ACTIVE ctor (false) and by the GATED FormShow
//  (false, :50) / FormClose (true, :107); `fShow` is written ONLY by the two
//  GATED bodies.  CONSEQUENCE while (C-1)/(C-2) stay gated: `fShow` is
//  permanently `false` and `NeedRef` permanently `false`.  For `fShow` that
//  is observable at the 2 pre-committed Command.cpp sites the moment THEY are
//  un-gated -- the "is any form open" chain would answer "this form is never
//  open".  That is a consequence of the link/safety boundary, not a bug in
//  this facade; flagged per task rule.  `NeedRef` has no reader anywhere in
//  the port today (`rg "\bNeedRef\b"` outside this pair -- 0 hits, 20260828).
//
//  ============================================================================
//  DISCLOSURE (task rule 8 -- what was NOT read to completion / NOT verified)
//  ============================================================================
//  - `ProcessLastSetIni_Visible`'s body was read in FULL (11 lines,
//    cprod.cpp:2824-2836) and `ReadWriteIni(bool)` / `CheckAndReadIniData
//    (bool)` were read in full, because the whole (C-1) gate rests on them.
//    The OTHER five `ReadWriteIni` overloads (AnsiString/TDateTime/int/double/
//    unsigned long, common.cpp:1619/:1638/:1662/:1687/:1714) were NOT read --
//    `ProcessLastSetIni_Visible` uses the bool overload for 10 of its 11 keys
//    and the int overload for `iShowCateByArm`; the int overload was NOT
//    opened, so the claim "the read path seeds the file" is proven for the
//    bool keys and ASSUMED (by symmetry of the wrapper) for the int one.
//    Immaterial to the gate -- one proven seeding line is enough.
//  - `ReplaceIniData` (common.cpp:1298) and `MyForceDirectories`
//    (common.cpp:1888) were NOT read in full; only their names, signatures
//    and owning `add_library` were established.  Both are gated regardless.
//  - golden `cCounterSel.dfm` was NOT opened.  The GATED FormShow writes
//    `Top=20; Left=250;`, so no design-time geometry is needed by any ACTIVE
//    body -- but a future wave un-gating (C-1) should note this facade
//    declares no `Left`/`Top` at all yet.
// =============================================================================
#ifndef FORMS_FCOUNTERSEL_H
#define FORMS_FCOUNTERSEL_H

#include "forms/FormWidgets.h"   // AnsiString / TObject / TSpeedButton (vclcompat)

// ===========================================================================
//  TfCounterSel -- non-VCL facade (golden cCounterSel.h:12-58,
//  `class TfCounterSel : public TForm`; NO base class here, DEVIATION D-2).
// ===========================================================================
class TfCounterSel
{
public:
    TfCounterSel();

    // -- golden public: User declarations ------------------------------------
    bool NeedRef = false;                              // golden :56, written by the ACTIVE ctor
    bool fShow   = false;                              // golden :57, PRE-COMMITTED (Command.cpp:10247, Command.cpp:15035)

    // -- golden __published widget touched by an ACTIVE body -----------------
    TSpeedButton *spbExit = new TSpeedButton();         // golden :48

    // -- ACTIVE this wave (defined in forms/fCounterSel.cpp) ------------------
    void spbExitClick(TObject *Sender);                 // golden :145-149  ACTIVE
    void Close() {}                                     // DEVIATION D-3, port-only no-op

    // -- GATED this wave -- declared here, golden body translated in
    //    forms/fCounterSel.cpp inside `#if 0`.  Calling one is a LINK ERROR
    //    by design (see GATE REGISTER above). ---------------------------------
    void FormShow(TObject *Sender);                     // golden :29-54    GATE (C-1)
    void FormClose();                                   // golden :56-109   GATE (C-2), DEVIATION D-4
    AnsiString CheckFormIni(AnsiString szDir, AnsiString str);  // golden :111-143  GATE (C-3)
};

// AI(W906-FW3-DTL1) 20260828: TfCounterSel/fCounterSel were FREE tree-wide
// (see banner's measured provenance) -- declared here so the 2 pre-committed
// `#if 0` sites (Command.cpp:10247, Command.cpp:15035) have something real to
// bind to once Command.cpp's OWN gate is separately lifted (NOT done by this
// wave).
extern TfCounterSel *fCounterSel;

#endif // FORMS_FCOUNTERSEL_H
