// ===========================================================================
//  tools/dfm2rc/layout_out/DfmLayoutTypes.h
//  W7-B1c: shared schema for the per-form *_layout.gen.{h,cpp} data tables.
//
//  AI(W906-W7-B1c) 20260728: new file (docs/W7_UI_ARCHITECTURE_PLAN.md S6
//  W7-B1c, gate G9). This header is hand-authored (unlike the per-form
//  *_layout.gen.h/.cpp, which ARE generated -- see their own DO-NOT-EDIT
//  banners) because it is schema, not data: every generated per-form header
//  includes this one instead of repeating the struct definitions, so that a
//  single translation unit can #include two or more forms' generated
//  headers together (e.g. this wave's own round-trip test) without an ODR
//  violation from N per-form headers each redeclaring "struct
//  DfmControlLayout" under N different include guards.
//
//  SCOPE / DESIGN NOTES (read before consuming from a later wave):
//
//  1. NO NUMERIC RESOURCE ID HERE, ON PURPOSE. Per D11 the join key across
//     facade / .rc / layout-table is the .dfm leaf control name, and the
//     plan's own dependency graph runs W7-B1b (the .rc + numeric
//     IDC_.../IDD_... generator) and W7-B1c (this table) in PARALLEL, both
//     depending only on W7-B1a's IR -- neither may assume the other's
//     output exists yet. `id_symbol` below is the STRING form of the D11
//     rule computed independently from the same IR B1b consumes -- but it
//     is KIND-AWARE, not one uniform pattern (AI(W906-W7-B1-review)
//     20260728: MEDIUM-2 fix -- an earlier version of emit_layout.py's
//     compute_id_symbol() applied `IDC_<FORMSTEM>_<NAME>` to every non-root
//     node regardless of kind, which was wrong for 3,840 of the corpus's
//     22,627 IR nodes):
//       - LEAF / TABHOST rows: `IDC_<FORMSTEM>_<NAME>` (all upper-case) --
//         the control-id #define W7-B1b's direct_leaf_entries emits.
//       - SUBDLG rows (D8 dialog owners: TGroupBox/TScrollBox/TTabSheet/
//         TRadioGroup/TframeProdInfo, and any TPanel with children): the
//         DIALOG-TEMPLATE symbol `IDD_<FORMSTEM><suffix>`, built by the
//         identical mechanical containerpath_suffix() rule W7-B1b's
//         IdAllocator.new_idd uses (imported directly from emit_rc.py, not
//         re-derived, so the two cannot drift apart) -- never an IDC_
//         symbol, because a SUBDLG node never gets a CONTROL entry in its
//         parent (see emit_rc.py's own docstring point 1).
//       - ROOT and NONVISUAL rows: `""`. ROOT's own IDD_<FORMSTEM> lives in
//         W7-B1b's dialog-template namespace under the ROOT node's own
//         dialog-owner entry, a different key this table does not
//         duplicate; NONVISUAL gets no .rc trace whatsoever (G1's own
//         documented asymmetry), so there is truthfully no #define to name.
//     `id_symbol` is guaranteed to be the *name* of the #define W7-B1b will
//     emit for this node (when non-empty), but this table deliberately does
//     not know or guess that macro's *numeric* value. This is no longer
//     merely a promise in a comment: gates.py's `gate_id_symbol_join`
//     (wired into run_b1d.py's `fidelity`/`idempotent` modes) actually
//     performs the join against each form's own .rcmeta.json sidecar and
//     asserts the strings match, for every row, every run.
//  2. host_dialog_path (D8): every TGroupBox/TScrollBox/TTabSheet/
//     TRadioGroup/TframeProdInfo, and every TPanel that itself has DFM
//     children, becomes its own child DIALOGEX (D8: "not flattened"). A
//     control's `parent_path` is its literal .dfm nesting parent (verbatim,
//     same as the IR); `host_dialog_path` is the answer to a different
//     question -- "which DIALOGEX template will actually contain this
//     control's HWND, i.e. which dialog's client-area coordinate space do
//     `left`/`top` below live in" -- found by walking the .dfm parent chain
//     upward (skipping intermediate LEAF/TABHOST nodes; TPageControl itself
//     hosts no DFM children as MoveWindow'd siblings, its TTabSheet
//     children are each their own child dialog) until a ROOT or SUBDLG
//     ancestor is found. For a node that is ITSELF a ROOT or SUBDLG,
//     host_dialog_path is its own nearest ROOT/SUBDLG *ancestor* (i.e. the
//     dialog it is a *child control of*, in its role as one more control
//     inside that parent template) -- not itself. The ROOT node's own
//     host_dialog_path is "" (a top-level window is not hosted inside
//     another template).
//  3. VCL property-absence defaults are NOT fabricated here. `align_mode`/
//     `anchors_mask` are "" when the .dfm did not write the property --
//     callers must apply VCL's own defaults themselves (Align: alNone;
//     Anchors: akLeft+akTop; Visible: True; TabOrder: declaration order)
//     rather than have this table silently inject a guessed value that
//     looks like real design-time data.
//  4. Colour fields carry BOTH the resolved 32-bit value (has_*_color
//     true) AND the original token (*_color_token, "" if the source was
//     already a plain integer). See colorconsts.py's own docstring for why
//     a symbolic system-colour reference (e.g. clBtnFace) cannot be fully
//     resolved ahead of paint time and must not be silently baked in as
//     today's theme colour. The resolved value is a genuine signed 32-bit
//     int (colorconsts.to_i32): system-colour references wrap to negative,
//     matching Delphi's real `TColor = type Integer` in-memory
//     representation -- this is also mechanically required, not just
//     faithful, because a C++ aggregate initializer is list-initialization
//     and the standard treats a too-big-for-`int` literal there as
//     ill-formed (verified: GCC hard-errors on it, see this wave's own
//     compile-smoke gate history).
//  5. custom_props (D12): a control's component-specific properties
//     (Alias / LEDStyle / Interval / Value / TrueColor / FalseColor /
//     TrueFontColor / FalseFontColor / XItem / YItem / DirectStyle /
//     DirectWidth / EdgeWidth / LineWidth) are carried as a generic
//     key/value bag rather than one struct field per possible property,
//     scoped to the 7 custom-control classes that this corpus's .dfm files
//     actually declare these properties on (TALed/TMyLed/TMyLedLane/
//     TBtnPanel/TBtnPanelLane/TTMyTray/TTMyTray256 -- verified empirically,
//     see emit_layout.py CUSTOM_PROP_KEYS/CUSTOM_PROP_CLASSES). FLAGGED
//     DISCREPANCY against the plan brief's prose, not silently resolved:
//     the brief's own text ("InPort/InBit/InIP/InRing/InType/OutPort/
//     OutBit/OutIP/OutRing/OutType/ISABase IO metadata") describes
//     properties that a full-corpus scan of all 133 IR files' custom-
//     control nodes shows are NEVER present as .dfm design-time properties
//     in this corpus -- they are runtime-only (`->InPort = ...` etc.
//     assignments living in golden .cpp code, which is exactly what the
//     plan's own per-family call-count histogram in S6-W7-C ("執行期 API")
//     is counting, not .dfm property counts). There is therefore nothing
//     for a .dfm-derived table to carry for those fields; TrueColor/
//     FalseColor/TrueFontColor/FalseFontColor/Alias/LEDStyle/XItem/YItem/
//     Interval/Value/DirectStyle/DirectWidth/EdgeWidth/LineWidth ARE real
//     .dfm properties and are fully carried.
// ===========================================================================
#ifndef HT9045_DFM2RC_LAYOUT_TYPES_H
#define HT9045_DFM2RC_LAYOUT_TYPES_H

namespace ht9045_layout {

struct DfmCustomProp {
    const char* key;    // e.g. "Alias", "LEDStyle", "XItem", "TrueColor"
    const char* value;  // raw decoded value, formatted as text (ints as
                        // decimal, strings verbatim/UTF-8, colour tokens
                        // left as their symbolic name -- NOT pre-resolved,
                        // so a caller wanting the resolved TColor value for
                        // e.g. "TrueColor" should prefer the dedicated
                        // true_color/true_color_token fields below instead
                        // of re-parsing this bag).
};

struct DfmControlLayout {
    // --- identity / tree shape -----------------------------------------
    const char* dfm_path;          // dotted path from IR, globally unique
    const char* leaf_name;         // .dfm leaf name (D11 join key, unique
                                    // within this form, case-insensitively
                                    // too -- verified across all 133 forms)
    const char* id_symbol;         // "IDC_<FORMSTEM>_<NAME>" (D11), "" for
                                    // the ROOT node (see note 1 above)
    const char* vcl_class;         // e.g. "TEdit", "TMyLedLane"
    const char* parent_path;       // literal .dfm nesting parent, "" for ROOT
    const char* host_dialog_path;  // D8 dialog-template host (note 2), ""
                                    // for the ROOT node
    int sibling_index;             // 0-based declaration order among siblings
    int depth;                     // 0 = ROOT
    const char* kind;               // "ROOT"|"LEAF"|"SUBDLG"|"TABHOST"|"NONVISUAL"
    bool is_graphic_control;        // TGraphicControl descendant (no HWND
                                    // in stock VCL) -- see dfm_parse.py's
                                    // GRAPHIC_CONTROL_CLASSES for the exact
                                    // set and the plan-vs-measured discrepancy
                                    // note it documents

    // --- D7 geometry authority: verbatim .dfm pixel integers ------------
    bool has_left;   int left;
    bool has_top;    int top;
    bool has_width;  int width;
    bool has_height; int height;

    // --- tab order / visibility / auto-layout ----------------------------
    bool has_tab_order; int tab_order;
    int  visible;        // -1 = property absent (VCL default True),
                          //  0 = False, 1 = True
    const char* align_mode;    // raw token e.g. "alClient", "" = absent
    const char* anchors_mask;  // comma-joined raw tokens e.g.
                               // "akLeft,akTop,akRight,akBottom", "" = absent

    // --- per-control font (9,933 specs corpus-wide; DIALOGEX allows only
    //     ONE font for the whole template, D7 -- OnInitDialog applies this
    //     per-control via CreateFontIndirect+SetFont) ----------------------
    bool has_font;
    const char* font_face;        // decoded font name (UTF-8; corpus fonts
                                   // include CJK names e.g. u8"新細明體")
    int font_height_px;           // raw signed Font.Height verbatim -- same
                                   // sign convention as LOGFONT.lfHeight,
                                   // pass straight through, do not "fix" the
                                   // sign (see file banner note 3's sibling
                                   // rule: never fabricate/normalise)
    const char* font_style_bits;  // comma-joined SET tokens e.g. "fsBold",
                                   // "" = none/empty set
    bool has_font_color;
    int font_color;                // resolved TColor (colorconsts.py), a
                                    // real signed 32-bit value (see
                                    // colorconsts.to_i32 -- system-colour
                                    // references are genuinely negative,
                                    // matching Delphi's actual in-memory
                                    // `TColor = type Integer`), only
                                    // meaningful when has_font_color is true
                                    // AND font_color_token is "" or a KNOWN
                                    // token (see next field)
    const char* font_color_token;  // "" if the source was a plain integer;
                                    // otherwise the raw "clXxx" identifier,
                                    // ALWAYS populated even when unresolved
                                    // (font_color is then 0 and must not be
                                    // used -- check has_font_color)

    // --- plain Color property (4,705 corpus-wide) ------------------------
    bool has_color;
    int color;
    const char* color_token;

    // --- D12 custom-control component props (see note 5) -----------------
    // TrueColor/FalseColor/TrueFontColor/FalseFontColor get the SAME
    // has/resolved/token colour-resolution treatment as color/font_color
    // above (not just raw text in the generic bag below) because they are
    // genuinely colours a later OnCtlColor/owner-draw pass needs resolved
    // the same way -- this is the direct, literal answer to D12's "True*
    // Color must survive losslessly" ask. They are also still present in
    // custom_props for uniform/generic lookup by key.
    bool has_true_color;        int true_color;        const char* true_color_token;
    bool has_false_color;       int false_color;       const char* false_color_token;
    bool has_true_font_color;   int true_font_color;   const char* true_font_color_token;
    bool has_false_font_color;  int false_font_color;  const char* false_font_color_token;

    int custom_prop_count;
    const DfmCustomProp* custom_props;  // pointer into this form's flat
                                        // k<FormStem>CustomProps[] array,
                                        // nullptr/0 when custom_prop_count==0
};

} // namespace ht9045_layout

#endif // HT9045_DFM2RC_LAYOUT_TYPES_H
