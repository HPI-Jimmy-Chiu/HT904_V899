// =============================================================================
//  forms/fPrecaution.h  --  non-VCL facade for golden's TfPrecaution
//                            (Precaution.h)
//
//  AI(W906-FW3-HSP1) 20260828: new file, FW wave FW3-HSP1 (facade wave 1 of
//  Precaution -- read-only direction, no write path). Golden source of
//  truth: HT9011UC_Code_V3.33.906.0_20260618/Precaution.h (52 lines) +
//  Precaution.cpp (93 lines). Decoded this wave with
//  `io.open(p, encoding='cp950')`, written out UTF-8, 0 U+FFFD.
//
//  ROLE
//  ----
//  TfPrecaution is golden's read-only "Precaution Record" summary popup: it
//  mirrors 9 fields (document no / note / approved manager / watchmakers /
//  start-end time / close type / close name / spec no) plus a memo, all
//  SOURCED from `fObserver` (the maintenance-log entry form), and self-closes
//  via a polling timer once any mirrored field goes blank.
//
//  `python tools/census/wave_preflight.py Precaution.cpp` (20260828, re-run
//  at closeout, see GATE REGISTER note below): 6 bodies = 6 `TfPrecaution::`
//  members + 0 file-scope, 73 span lines, 0 brace-swallow short-fall, 0
//  overload collapse. THIS WAVE: 3 of 6 ACTIVE (50.0%) + 3 GATED (declared,
//  not defined). BY LINES: the 3 ACTIVE bodies' golden spans total 21 of 73
//  lines (28.8%) -- tm_CheckEditEmptyTimer (30L) is the largest ACTIVE
//  delivery; DoIniDataToForm (16L, GATE) + FormShow (12L, GATE) together
//  are close to this wave's ACTIVE total by line count but blocked on a
//  single shared link-boundary dependency (see (P-1)/(P-2) below).
//
//  ============================================================================
//  *** THE GLOBALS `TfPrecaution` / `fPrecaution` ARE FREE -- THIS WAVE
//  CLAIMS THEM *** (verified 20260828 via `tools/census/wave_candidates.py`
//  and a direct `grep -rn "TfPrecaution\b\|fPrecaution\b"` sweep, build*/
//  .git/.svn/.pti_frames/web excluded): 0 hits for `class TfPrecaution` or a
//  live `fPrecaution` global anywhere outside this file/forms/fPrecaution.cpp.
//  `fPrecaution` DOES already appear as a NAME inside `cObserver.cpp`'s own
//  `#if 0` GATE (B-FPRE-1)/(B-FPRE-2) blocks (cObserver.cpp:5628-5629,5686 --
//  see PRE-COMMITTED SITES below) -- that file's own banner explicitly
//  records "NOTHING of that form is ported ... 0 hits (20260825)", which
//  this wave now supplies the real symbol for (un-gating those 2 blocks is
//  cObserver.cpp's OWN separate, deliberate edit -- out of this wave's write
//  boundary). `.h` declares `extern TfPrecaution *fPrecaution;`, `.cpp`
//  defines `TfPrecaution *fPrecaution = new TfPrecaution();` -- same idiom
//  as forms/fCleaning.cpp:45.
//
//  ============================================================================
//  PRE-COMMITTED #if 0 SITES ELSEWHERE IN THE TREE (2, both inside
//  cObserver.cpp's OWN gates, confirmed this wave)
//  ============================================================================
//    Close()   cObserver.cpp:5628 (GATE B-FPRE-1), :5686 (GATE B-FPRE-2) --
//              both call with ZERO arguments, matching this wave's own
//              `void Close();` no-op (DEVIATION D-2).
//    Show()    cObserver.cpp:5629 (GATE B-FPRE-1) -- zero arguments,
//              matching this wave's own `void Show();` no-op (DEVIATION D-3).
//  Both cObserver.cpp gates are STILL `#if 0` after this wave -- declaring
//  these members does NOT un-gate them.
//
//  ============================================================================
//  GATE REGISTER (3)
//  ============================================================================
//  (P-1) FormShow            golden :32-43 (12L) -- LINK BOUNDARY. Line :40,
//        `MemoHandlerPrecautionRecord->Text=fObserver->
//        MemoHandlerPrecautionRecord->Text;`, unconditionally derefs
//        `fObserver`. `fObserver` is REAL (a class named `TfObserver` exists
//        in this port) but its DEFINING TU is `cObserver.cpp`, whose
//        `add_library` block is `ht9045_sm` (CMakeLists.txt:2062, confirmed
//        this wave by locating the enclosing `add_library` line with
//        `awk`). This facade's expected landing (`ht9045_forms`, same layer
//        as forms/fCleaning.cpp) does not link `ht9045_sm`
//        (CMakeLists.txt:769-776) -- same boundary shape as forms/
//        fShowBinSet.h GATE (H-1)'s `fShowBinSelect`. `Width=585;
//        Height=510; Left=...; Top=...;` (golden :34-37, own geometry) and
//        `DoIniDataToForm();` (golden :39, P-2 below) precede the
//        disqualifying line but the transitive rule (DoIniDataToForm is
//        ITSELF gated on the SAME `fObserver` boundary, see P-2) already
//        gates the whole body independently of the direct :40 deref.
//  (P-2) DoIniDataToForm      golden :76-91 (16L) -- LINK BOUNDARY, same
//        `fObserver` reason as (P-1). EVERY line but the first
//        (`if(SystemInitialOK==false) return;`, own-global guard) and the
//        last (`MemoHandlerPrecautionRecord->Lines->Clear();`, own widget)
//        reads `fObserver->ed*`/`->pn*`/`->cob*` -- 9 of 9 mirrored fields
//        unconditionally depend on `fObserver`, so no partial split is
//        possible without inventing which fields to drop (task rule: 忠實
//        優先, 不要寫退化版本).
//  (P-3) FormClose            golden :19-25 -- TRANSITIVE via
//        `DoIniDataToForm();` (golden :24, P-2) unconditionally, as its
//        final statement. `fShow=false;` and
//        `tm_CheckEditEmpty->Enabled=false;` (golden :22-23) are own-
//        field/own-widget only and would otherwise have been safe, but the
//        transitive rule still applies (same idiom as forms/fHotPlate.h
//        GATE (G-4)).
//
//  ============================================================================
//  ACTIVE (3) -- reachability evidence
//  ============================================================================
//   TfPrecaution()           golden :14-17 -- empty body past
//        `: TForm(Owner)`, fields-only, no global deref. SIOF-safe (docs/
//        KNOWLEDGE.md "static-init ctor 不可碰 NULL 全域").
//   FormCreate(TObject*)     golden :27-30 -- `fShow=false;` only, own
//        field.
//   tm_CheckEditEmptyTimer(TObject*) golden :45-74 (30L) -- reads 8 own
//        `TPanel*->Caption` fields via `AnsiString::IsEmpty()`
//        (vclcompat/AnsiString.cpp, `vclcompat`) and own
//        `MemoHandlerPrecautionRecord->Lines->Count` (vclcompat TStringList,
//        same target), then calls `Close()` (DEVIATION D-2, own no-op) if
//        any is empty. Zero external-class dependency -- does NOT touch
//        `fObserver` at all (unlike FormShow/DoIniDataToForm), so it is
//        NOT gated by (P-1)/(P-2)'s boundary.
//
//  ============================================================================
//  DEVIATIONS (port-only; each with why)
//  ============================================================================
//  (D-1) `__fastcall` dropped from every signature -- tree-wide convention.
//  (D-2) `Close()` is a PORT-ONLY no-op (`{}`) -- golden's own `TForm::Close`
//      (this facade has no TForm base, D-4 below). Needed by ACTIVE
//      `tm_CheckEditEmptyTimer` AND matches the 2 pre-committed
//      cObserver.cpp call sites.
//  (D-3) `Show()` is a PORT-ONLY no-op (`{}`) -- golden's own `TForm::Show`.
//      No ACTIVE body this wave calls it; declared+defined solely because
//      it is a pre-committed cObserver.cpp call site (same "declare the
//      exact name/signature a waiting #if 0 site expects" rule as every
//      other facade's GATE REGISTER convention, applied here to a fully
//      free no-op rather than a GATED prototype since defining it costs
//      nothing and needs no unavailable symbol).
//  (D-4) `TfPrecaution` has NO base class (matches forms/fCleaning.h /
//      forms/fMesSystem.h D-4).
//  (D-5) `FormClose(TObject*, TCloseAction&)` -> `void FormClose();` (GATED,
//      so signature is free either way) -- `TCloseAction` has no port,
//      matches established precedent (forms/fMesSystem.h D-5).
//
//  ============================================================================
//  FIELD LIST (9 widgets + 1 bool; only what an ACTIVE body touches --
//  minimal-declaration idiom, matches forms/fMesSystem.h's own practice.
//  GATED bodies are declared-not-defined prototypes and therefore need no
//  field of their own pre-populated.)
//  ============================================================================
//    fShow (bool)
//    pnPrecautionRecordDocumentNo/pnNoteContents/pnApprovedManager/
//      pnPrecautionStartTime/pnCloseType/pnWatchmakers/pnCloseName/
//      pnPrecautionEndTime (TPanel*)             -- tm_CheckEditEmptyTimer
//    MemoHandlerPrecautionRecord (TMemo*)          -- tm_CheckEditEmptyTimer
//  NOT declared (touched only by a GATED body): pnPrecautionsRecord/
//  pnMaintenanceRecord/pnMaintenanceRecordTitle/pnPrecautionRecordDocumentNoTitle/
//  Panel91-96/pnPRSpecification/tm_CheckEditEmpty (TTimer*).
//
//  ============================================================================
//  ZERO-WRITER FIELDS -- does not apply
//  ============================================================================
//  All 9 declared widget fields are written ONLY by the GATED
//  DoIniDataToForm (P-2) and read by the ACTIVE tm_CheckEditEmptyTimer --
//  so every one of them stays at its `new`-default (Caption="",
//  Text="") for as long as (P-2) stays gated, meaning
//  tm_CheckEditEmptyTimer's `bEmpty[]` checks are ALL true and it always
//  calls `Close()` the instant it is exercised. This is an OBSERVABLE
//  behaviour consequence of the link boundary, not a bug in
//  tm_CheckEditEmptyTimer itself -- flagged per task rule 7. Classified
//  "不外流": `Close()` only flips this facade's OWN `fShow`, nothing
//  downstream consumes it this wave.
//
//  ============================================================================
//  DISCLOSURE (task rule 6)
//  ============================================================================
//  - `fObserver`'s OWN member shapes (`edPrecautionRecordDocumentNo`/
//    `edNoteContents`/etc, all `TEdit*`) were read off Precaution.cpp's own
//    call sites, not independently re-verified against a live `TfObserver`
//    facade member list -- moot while (P-1)/(P-2) gate on the link boundary
//    alone (an `ht9045_sm`-reaching wave would need to re-check field names
//    match, not just that the class is reachable).
// =============================================================================
#ifndef FORMS_FPRECAUTION_H
#define FORMS_FPRECAUTION_H

#include "forms/FormWidgets.h"   // AnsiString / TPanel / TMemo

// ===========================================================================
//  TfPrecaution -- non-VCL facade (golden Precaution.h:12-47, `class
//  TfPrecaution : public TForm`; NO base class here, see DEVIATION D-4).
// ===========================================================================
class TfPrecaution
{
public:
    TfPrecaution();

    // -- golden public: User declarations -----------------------------------
    bool fShow = false;                                     // golden :46

    // -- golden __published widgets touched by ACTIVE tm_CheckEditEmptyTimer -
    TPanel *pnPrecautionRecordDocumentNo = new TPanel();      // golden :28
    TPanel *pnNoteContents               = new TPanel();      // golden :29
    TPanel *pnApprovedManager            = new TPanel();      // golden :30
    TPanel *pnPrecautionStartTime        = new TPanel();      // golden :25
    TPanel *pnCloseType                  = new TPanel();      // golden :33
    TPanel *pnWatchmakers                = new TPanel();      // golden :31
    TPanel *pnCloseName                  = new TPanel();      // golden :35
    TPanel *pnPrecautionEndTime          = new TPanel();      // golden :27
    TMemo  *MemoHandlerPrecautionRecord  = new TMemo();        // golden :18

    // -- ACTIVE this wave (defined in forms/fPrecaution.cpp) -----------------
    void FormCreate(TObject *Sender);                          // golden :27-30  ACTIVE
    void tm_CheckEditEmptyTimer(TObject *Sender);              // golden :45-74  ACTIVE
    void Close() {}                                             // DEVIATION D-2, port-only no-op, pre-committed (cObserver.cpp:5628,5686)
    void Show()  {}                                             // DEVIATION D-3, port-only no-op, pre-committed (cObserver.cpp:5629)

    // -- GATED this wave -- declared, NOT defined. Calling one of these is a
    //    LINK ERROR by design (see GATE REGISTER above). ---------------------
    void FormShow(TObject *Sender);                            // golden :32-43   GATE (P-1)
    void DoIniDataToForm();                                    // golden :76-91   GATE (P-2)
    void FormClose();                                          // golden :19-25   GATE (P-3), DEVIATION D-5 no-param
};

// AI(W906-FW3-HSP1) 20260828: TfPrecaution/fPrecaution globals were FREE
// tree-wide (see banner) -- declared here per this wave's task brief so the
// 2 pre-committed cObserver.cpp #if 0 sites have something real to bind to
// once that file's OWN gates are separately lifted (not done by this wave).
extern TfPrecaution *fPrecaution;

#endif // FORMS_FPRECAUTION_H
