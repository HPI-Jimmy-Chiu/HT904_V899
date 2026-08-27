// =============================================================================
//  forms/fShowBinSet.h  --  non-VCL facade for golden's TfShowBinSet
//                            (cShowBinSet.h)
//
//  AI(W906-FW3-HSP1) 20260828: new file, FW wave FW3-HSP1 (facade wave 1 of
//  cShowBinSet -- read-only direction, no write path). Golden source of
//  truth: HT9011UC_Code_V3.33.906.0_20260618/cShowBinSet.h (160 lines) +
//  cShowBinSet.cpp (234 lines). Decoded this wave with
//  `io.open(p, encoding='cp950')`, written out UTF-8, 0 U+FFFD.
//
//  ROLE
//  ----
//  TfShowBinSet is golden's start-permission confirmation dialog: two tabs
//  (Bin Setting Check / Site Mapping Check), the latter showing a live
//  channel grid mirrored off `TestIF_File.iSiteMap[][]`. It exists purely to
//  make an operator confirm bin/site settings before Initial Start for
//  specific customer codes (CC_AMKOR_Philippines / CC_Microchip_Phil) --
//  every one of its own call sites lives inside golden csystem.cpp's
//  `CheckContinusStartIsReady`, itself gated `#if 0` in this port (GATE
//  G4-4, csystem.cpp:11188) because its OWN terminal statement calls
//  `SetShuttleMode`, declared but never defined.
//
//  `python tools/census/wave_preflight.py cShowBinSet.cpp` (20260828,
//  re-run at closeout, see GATE REGISTER note below): 7 bodies = 6
//  `TfShowBinSet::` members + 1 file-scope (`ShowSiteMapping_YES_NO`), 187
//  span lines, 0 brace-swallow short-fall, 0 overload collapse. THIS WAVE:
//  4 of 6 members ACTIVE (66.7%) + the 1 file-scope body ALSO ACTIVE (5/7
//  bodies overall) + 2 members GATED (declared, not defined). BY LINES: the
//  5 ACTIVE bodies' golden spans total 83 of 187 lines (44.4%) -- the ctor
//  (62L) dominates; FormShow (97L, GATE) alone is larger than this whole
//  wave's ACTIVE total.
//
//  ============================================================================
//  ⚠ NEAR-NAME HAZARD, CHECKED BEFORE ANY OTHER WORK THIS WAVE
//  ============================================================================
//  `TfShowBinSelect` / `fShowBinSelect` (cShowBinSelect.h/.cpp, ALREADY
//  translated -- forms/fShowBinSelect.h + cShowBinSelect.cpp) is a
//  DIFFERENT golden class from `TfShowBinSet` / `fShowBinSet` (THIS file).
//  Verified this wave: `grep -rn "TfShowBinSet\b"` outside this file+.cpp
//  across the whole port tree (build*/.git/.svn/.pti_frames/web excluded) --
//  only 2 hits, both `tools/dfm2rc/**/layout_out/cShowBinSet_layout.gen.cpp`
//  (a generated dfm2rc IR artefact, not a facade). Zero real collisions.
//  `TfShowBinSelect`/`fShowBinSelect` ARE deliberately referenced BY this
//  file (FormShow, GATED -- see GATE (H-1) below) -- that reference is
//  intentional and spelled out in full each time, never abbreviated.
//
//  ============================================================================
//  *** THE GLOBALS `TfShowBinSet` / `fShowBinSet` ARE FREE -- THIS WAVE
//  CLAIMS THEM *** (verified 20260828 via `tools/census/wave_candidates.py`
//  and the near-name-hazard grep above): 0 real hits outside this
//  file/forms/fShowBinSet.cpp. `.h` declares `extern TfShowBinSet
//  *fShowBinSet;`, `.cpp` defines `TfShowBinSet *fShowBinSet = new
//  TfShowBinSet();` -- same idiom as forms/fCleaning.cpp:45.
//
//  ============================================================================
//  FIVE `#if 0`-GATED CALL SITES ELSEWHERE IN THE TREE -- NAMES/TYPES ARE
//  PRE-COMMITTED, NOT THIS WAVE'S CHOICE (all inside csystem.cpp's GATE
//  G4-4 block, csystem.cpp:11188-11239, confirmed this wave)
//  ============================================================================
//    Caption               csystem.cpp:11200,11219  -- matches `AnsiString Caption;`
//    ShowModal             csystem.cpp:11214         -- matches ACTIVE `void ShowModal();`
//    fShow                 (golden header field, no external #if0 site found
//                            for THIS class specifically beyond the class's
//                            own methods -- listed by the task brief, kept
//                            regardless as golden's own public member)
//    tsBinSettingCheck     csystem.cpp:11202,11221   -- matches `TTabSheet *tsBinSettingCheck;`
//    tsSiteMappingCheck    csystem.cpp:11201,11220   -- matches `TTabSheet *tsSiteMappingCheck;`
//  These 5 sites are STILL `#if 0` after this wave (whole block gated on
//  `SetShuttleMode`, unrelated to this file) -- declaring the matching
//  members does NOT un-gate them.
//
//  ============================================================================
//  ⚠ A SIXTH SITE THE PREFLIGHT TOOL DID NOT SURFACE (found by hand,
//  20260828) -- `wave_preflight.py`'s CALL SITES section pattern-matches
//  `fShowBinSet->member` only; it does NOT catch calls to the file-scope
//  free function `ShowSiteMapping_YES_NO()`. `grep -n
//  "ShowSiteMapping_YES_NO" csystem.cpp` finds a REAL call site,
//  csystem.cpp:11222 (`ret=ShowSiteMapping_YES_NO();`), also inside the
//  SAME GATE G4-4 `#if 0` block. Declared+DEFINED here (see ACTIVE list --
//  its own dependencies are fully self-contained, see below), matching the
//  same "reuse the exact pre-committed name/signature" rule as the 5 sites
//  above even though the tooling did not flag it. Flagged here so a future
//  wave trusts this file's own grep, not the tool's member-only pattern, for
//  free-function call sites.
//  ============================================================================
//
//  ============================================================================
//  GATE REGISTER (2)
//  ============================================================================
//  (H-1) FormShow          golden :103-199 (97L) -- LINK BOUNDARY. Its FIRST
//        loop (golden :105-112) unconditionally derefs `fShowBinSelect->
//        MyBinSel[i]`/`->MyBinSelLab[i]` (`->Caption`/`->Visible`/`->Font->
//        Color`). `fShowBinSelect` is REAL (forms/fShowBinSelect.h) but its
//        DEFINING TU is `cShowBinSelect.cpp`, whose `add_library` block is
//        `ht9045_sm` (CMakeLists.txt:2076, confirmed this wave by locating
//        the enclosing `add_library` line with `awk`) -- despite living
//        next to `forms/fShowBinSelect.h` by NAME, the .cpp is NOT in
//        `ht9045_forms`. This facade's expected landing (`ht9045_forms`,
//        same layer as forms/fCleaning.cpp) does not link `ht9045_sm`
//        (CMakeLists.txt:769-776), so `fShowBinSelect` is unreachable at
//        link time from here -- same boundary shape as forms/fMesSystem.h
//        GATE (L-1)/(L-2)'s `TestSocket`. NOT split: every OTHER symbol the
//        rest of the function touches (`SiteData[]`/`TestIF_File.iSiteMap`/
//        `IniConfig.bDualSiteSupply4CH`/`CosFunction.bUse32ChanelSiteMap`/
//        `CUSTOMER_CODE`/`CC_ASE_KaohSiung`/`TrayForm.iFixTrayMode`/
//        `bAlarmBuzzer` -- ALL individually verified this wave as
//        `ht9045_globals`/header-only, i.e. otherwise reachable) is
//        irrelevant while the FIRST loop's `fShowBinSelect` deref already
//        gates the whole body -- same "one disqualifying dependency gates
//        the whole function" idiom as forms/fHotPlate.h GATE (G-1). Own
//        widgets (`TestSiteCH_Display[][]`/`TestLabCol_Display[]`/
//        `TestLabRow_Display[]`/`MyBinSel[]`/`MyBinSelLab[]`) this wave's
//        ctor already wires up correctly (index-compatible, see CTOR NOTE
//        below) so a future `ht9045_sm`-reaching wave can lift this gate
//        without re-deriving the array-index question.
//  (H-2) Timer1Timer        golden :226-232 (7L) -- LINK BOUNDARY. Its only
//        non-trivial statement, `DoSystemMessage();` (:231), is REAL
//        (`ckernel.cpp:3069`) but `ckernel.cpp`'s `add_library` block is
//        ALSO `ht9045_sm` (CMakeLists.txt:2273, confirmed via `awk`).
//        `if(fShow==false) return;` (golden :228-229) is own-field-only and
//        would otherwise have been safe, but the transitive rule applies to
//        the function as a whole (the guard exists ONLY to protect the
//        gated call, so translating the guard alone with no body to guard
//        would be a behaviour-altering partial translation).
//
//  ============================================================================
//  ACTIVE (5) -- reachability evidence
//  ============================================================================
//   TfShowBinSet()          golden :40-101 (62L) -- see CTOR NOTE below
//        (DEVIATION, not a verbatim widget-alias translation).
//   FormClose()             golden :201-205 -- `fShow=false;` only, own field.
//        D-2-style no-param signature (`TCloseAction` has no port, same
//        established precedent as forms/fMesSystem.h D-5).
//   Panel3Click(TObject*)   golden :207-211 -- `iReturnValue=1; Close();` --
//        `iReturnValue` is this wave's OWN new file-scope global (see FIELD
//        LIST); `Close()` is a port-only no-op (DEVIATION D-3, same idiom
//        as forms/fQwertyKey.h:366 / BarcodeReader.h:108).
//   Panel4Click(TObject*)   golden :213-217 -- same shape, `iReturnValue=2`.
//   ShowSiteMapping_YES_NO() (file-scope) golden :219-224 -- `iReturnValue=0;
//        fShowBinSet->ShowModal(); return iReturnValue;`. `fShowBinSet` is
//        THIS wave's own global (self-reference, always non-NULL once
//        static-init completes); `ShowModal()` is a port-only no-op
//        (DEVIATION D-4). Fully self-contained -- no external dependency.
//
//  ============================================================================
//  CTOR NOTE -- DEVIATION: array-population loop, not ~110 individually-
//  named widget aliases
//  ============================================================================
//  Golden's ctor's entire job (cShowBinSet.cpp :40-101) is copying ~110
//  individually-named `.dfm` widget pointers (`palAa..palDh` [32],
//  `labColA..H`/`labRowA..D` [12], `laAuto1..6/laFix1..12/laBinBox/
//  laMag1..14` [33], `labAuto1..6/labFix1..12/labBinBox/labMag1..14` [33])
//  into 5 arrays (`TestSiteCH_Display[][]`/`TestLabCol_Display[]`/
//  `TestLabRow_Display[]`/`MyBinSel[]`/`MyBinSelLab[]`) plus `fShow=false;`.
//  VERIFIED this wave (`grep -n "<name>"` against the cp950-decoded golden
//  .cpp for a sample of the ~110 names): NONE of them is read anywhere
//  outside the ctor -- only the 5 ARRAYS are (by the now-GATED FormShow).
//  Same reasoning, same precedent, as forms/fShowBinSelect.h's own CTOR
//  NOTE (`grep -n "CTOR NOTE" forms/fShowBinSelect.h`) for the SAME golden
//  design pattern in the sibling class -- this facade therefore constructs
//  the arrays directly (one heap object per slot, in a loop) rather than
//  declaring ~110 facade members whose only reader would be this same ctor.
//  BEHAVIOUR DELTA (disclosed, not hidden): golden aliases EXISTING `.dfm`
//  widgets (which may carry non-default design-time Caption/Visible/Color),
//  this port `new`s fresh, blank ones -- so `MyBinSel[i]`/`MyBinSelLab[i]`
//  start at TLabel's own ctor defaults (Caption="", Visible=false) rather
//  than whatever `.dfm` values `laAuto1` etc carried, until FormShow (GATE
//  H-1) overwrites them from `fShowBinSelect`. Same "cosmetic
//  pre-population for golden's own real VCL render only, not hydrated
//  here" posture as forms/fShowBinSelect.h's own HYDRATION section.
//  INDEX COMPATIBILITY (verified, the one correctness risk this deviation
//  could have introduced): golden's OWN `tempMyBinSel[]`/`tempMyBinSelLab[]`
//  literal arrays (cShowBinSet.cpp :78-94) list `laAuto1..laAuto6,
//  laFix1..laFix12, laBinBox, laMag1..laMag14` CONSECUTIVELY -- i.e.
//  `e6TrayName` ordering (MachineType.h:1070-1105, `eAuto1=0..eAuto6=5,
//  eFix1=6..eFix12=17,eBulkBox=18,eMag1=19..eMag14=32`), even though the
//  array's declared BOUND is the numerically-identical-but-differently-
//  ORDERED `e3TrayCount` in golden's OWN cShowBinSelect.h:497 (`e3TrayName`
//  splits Auto1-3/Fix1-9/BulkBox/Mag1-14/Auto4-6/Fix7-12). Checked THIS
//  wave whether the ALREADY-TRANSLATED `fShowBinSelect->MyBinSel[]` (which
//  FormShow, GATE H-1, would read once un-gated) uses the SAME `e6TrayName`
//  ordering: `cShowBinSelect.cpp:347` (`SetAutoVisible`, ACTIVE per forms/
//  fShowBinSelect.h) loops `for(int i=eAuto1;i<eTrayCount;i++)` -- i.e. the
//  WHOLE port facade normalises to `e6TrayName`/`eTrayCount` indices
//  throughout, matching golden's own ctor ordering exactly. So this wave's
//  `eTrayCount`-sized, `e6TrayName`-ordered `MyBinSel[]`/`MyBinSelLab[]` ARE
//  index-compatible with `fShowBinSelect->MyBinSel[]`/`->MyBinSelLab[]` for
//  the day GATE (H-1) opens -- not a re-derivation left for that wave.
//
//  ============================================================================
//  DEVIATIONS (port-only; each with why)
//  ============================================================================
//  (D-1) `__fastcall` dropped from every signature -- tree-wide convention.
//  (D-2) `FormClose()` takes NO parameters -- `TCloseAction` has no port,
//      same established precedent as forms/fMesSystem.h D-5.
//  (D-3) `Close()` is a PORT-ONLY no-op (`{}`) -- golden's own `TForm::Close`
//      (this facade has no TForm base, D-6 below). Needed by ACTIVE
//      Panel3Click/Panel4Click.
//  (D-4) `ShowModal()` is a PORT-ONLY no-op (`{}`) -- golden's own
//      `TForm::ShowModal` (instant-return, no real dialog). Needed by
//      ACTIVE `ShowSiteMapping_YES_NO` AND matches the pre-committed
//      csystem.cpp:11214 call site.
//  (D-5) `TestSiteCH_Display[MAX_SOCKET_ROW][MAX_SOCKET_COL]` /
//      `TestLabCol_Display[MAX_SOCKET_COL]` / `TestLabRow_Display
//      [MAX_SOCKET_ROW]` / `iReturnValue` are golden FILE-SCOPE globals
//      (cShowBinSet.cpp :18-20,38 -- NOT declared in golden's own header
//      either), so this port declares them at file scope in
//      forms/fShowBinSet.cpp too, not in this header -- faithful to
//      golden's own linkage/visibility choice (0 hits anywhere in this
//      port tree before this wave, verified via Grep).
//  (D-6) `TfShowBinSet` has NO base class (matches forms/fCleaning.h /
//      forms/fMesSystem.h D-4).
//  (D-7) `MyBinSel[]`/`MyBinSelLab[]` kept plain `TLabel*` (matching
//      golden's OWN cShowBinSet.h:152-153 declared type, `TLabel
//      *MyBinSel[eTrayCount];` -- NOT retyped to `TfShowBinSelectLabel*`
//      (which forms/fShowBinSelect.h uses for ITS OWN `MyBinSel[]` to carry
//      `->Font->Color`). Not needed here: the only body that would read
//      `->Font->Color` off THIS class's `MyBinSel[]` is FormShow, GATED
//      (H-1) -- adding a `Font` member now would be speculative surface no
//      ACTIVE body this wave exercises.
//
//  ============================================================================
//  ZERO-WRITER FIELDS
//  ============================================================================
//  `MyBinSel[]`/`MyBinSelLab[]`/`TestSiteCH_Display[][]`/
//  `TestLabCol_Display[]`/`TestLabRow_Display[]` are WRITTEN by the ACTIVE
//  ctor but READ by nothing ACTIVE this wave (their only reader, FormShow,
//  is GATE H-1). Consequence-free for now (no ACTIVE body consumes them);
//  this does NOT flow outward (nothing outside this class observes these
//  arrays either) -- classified "不外流" (does not leak to another
//  subsystem), same posture as forms/fMesSystem.h's `map2DIDFromServer`
//  zero-writer note but mirrored (here it's zero-READER, not zero-writer --
//  flagged under the same task-rule-7 obligation regardless).
//
//  ============================================================================
//  DISCLOSURE (task rule 6)
//  ============================================================================
//  - FormShow's OWN remaining ~80 lines past the disqualifying first loop
//    (SiteData[]/TestIF_File.iSiteMap/IniConfig.bDualSiteSupply4CH/
//    CosFunction.bUse32ChanelSiteMap/CUSTOMER_CODE/CC_ASE_KaohSiung/
//    TrayForm.iFixTrayMode/bAlarmBuzzer) WERE individually traced to their
//    `add_library` block this wave (all `ht9045_globals` or header-only) --
//    but this was for gate-classification completeness only; a future wave
//    lifting GATE (H-1) should re-verify rather than trust this note.
// =============================================================================
#ifndef FORMS_FSHOWBINSET_H
#define FORMS_FSHOWBINSET_H

#include "forms/FormWidgets.h"    // AnsiString / TLabel / TTabSheet
#include "MachineType.h"          // eTrayCount (array bound) -- header-only, same precedent as forms/fLotInfo.h/forms/fMesSystem.h's own direct include

// ===========================================================================
//  TfShowBinSet -- non-VCL facade (golden cShowBinSet.h:14-154, `class
//  TfShowBinSet : public TForm`; NO base class here, see DEVIATION D-6).
// ===========================================================================
class TfShowBinSet
{
public:
    TfShowBinSet();

    // -- golden public: User declarations -----------------------------------
    bool fShow = false;                                     // golden :151
    TLabel *MyBinSel[eTrayCount];                            // golden :152 -- DEVIATION D-7 (plain TLabel*)
    TLabel *MyBinSelLab[eTrayCount];                          // golden :153

    // -- golden __published widgets touched by a pre-committed #if 0 site --
    AnsiString Caption;                                       // golden TForm::Caption, pre-committed (csystem.cpp:11200,11219)
    TTabSheet *tsBinSettingCheck  = new TTabSheet();          // golden :19, pre-committed (csystem.cpp:11202,11221)
    TTabSheet *tsSiteMappingCheck = new TTabSheet();          // golden :25, pre-committed (csystem.cpp:11201,11220)

    // -- ACTIVE this wave (defined in forms/fShowBinSet.cpp) -----------------
    void FormClose();                                         // golden :201-205  ACTIVE, DEVIATION D-2
    void Panel3Click(TObject *Sender);                        // golden :207-211  ACTIVE
    void Panel4Click(TObject *Sender);                        // golden :213-217  ACTIVE
    void Close()     {}                                       // DEVIATION D-3, port-only no-op
    void ShowModal() {}                                       // DEVIATION D-4, port-only no-op, pre-committed (csystem.cpp:11214)

    // -- GATED this wave -- declared, NOT defined. Calling one of these is a
    //    LINK ERROR by design (see GATE REGISTER above). ---------------------
    void FormShow(TObject *Sender);                            // golden :103-199  GATE (H-1)
    void Timer1Timer(TObject *Sender);                         // golden :226-232  GATE (H-2)
};

// AI(W906-FW3-HSP1) 20260828: TfShowBinSet/fShowBinSet globals were FREE
// tree-wide (see banner) -- declared here per this wave's task brief so the
// pre-committed #if 0 call sites above have something real to bind to once
// csystem.cpp's own GATE G4-4 is separately un-gated (not done by this wave).
extern TfShowBinSet *fShowBinSet;

// golden cShowBinSet.h:157 -- file-scope free function, ACTIVE this wave
// (see ACTIVE list above). Matches the pre-committed csystem.cpp:11222 call
// site found by hand (see banner note on the preflight tool's blind spot).
int ShowSiteMapping_YES_NO();

#endif // FORMS_FSHOWBINSET_H
