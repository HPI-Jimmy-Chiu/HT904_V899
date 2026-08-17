// ===========================================================================
//  ui/layout/CustomCtrlAttach.cpp -- attach MFC shells to template-created
//  HT9045.* windows and push the D12 custom props
//  (docs/DESIGN_GA4_UI_ENGINES.md D-GA4-8; contract: CustomCtrlAttach.h).
//
//  AI(W906-GA4) 20260804: NEW FILE, implements the header exactly.
//
//  WHY THIS PASS EXISTS. RegisterCustomClasses.cpp registers every HT9045.*
//  window class with lpfnWndProc = DefWindowProc ("MFC subclasses later" --
//  its own banner), so the dialog template creates the 295 fMain custom
//  windows (ALed 222 / MyLed 6 / BtnPanel 11 / MyTray 56, D-GA4-8) as inert
//  holes. This pass is the promised "later": new shell -> SubclassWindow ->
//  rt.AddShell (FormRuntime owns; Destroy() Unsubclasses + deletes), then
//  push the layout row's design-time properties into the live core.
//
//  SHELL MAP. Exactly the 7 classes of emit_layout.py's CUSTOM_PROP_CLASSES
//  (:52-56) -- the empirically-verified full set of .dfm custom-prop
//  carriers -- keyed by the layout row's vcl_class string:
//      TALed / TMyLed / TMyLedLane  -> CLedCtrl       (one shell, three
//          classes: the two subclasses add only IO metadata, zero drawing --
//          CLedCtrl.h:42-47)
//      TBtnPanel / TBtnPanelLane    -> CBtnPanelCtrl
//      TTMyTray / TTMyTray256       -> CTrayCtrl
//  Every OTHER class is deliberately NOT attached: placeholder classes
//  (TStringGrid/TShape/TChart/TImage/...) stay visible inert holes during
//  bring-up per D-GA4-8 and the RegisterCustomClasses.cpp banner, and
//  standard VCL classes map to native Win32 controls that need no shell.
//  TTMyTray256 is mapped for completeness but is unreachable from any .dfm:
//  0 instances corpus-wide (RegisterCustomClasses.h:41-43 -- it exists only
//  as dynamic `new` sites in golden), so the fact that CTrayCtrl's INTERNAL
//  core is the 100-cell TrayCore (a .dfm-born MyTray256 would need
//  AttachCore256 with a caller-owned Tray256Core) is recorded, not solved.
//
//  COLOUR PUSH IS RAW, NOT RESOLVED. The W7-C1 cores typedef their own
//  signed `TColor` int (LedCore.h:54 / BtnPanelCore.h:36 / TrayCore.h:57)
//  and store system references verbatim (clBtnFace = 0x8000000F stays
//  negative); resolution to a COLORREF happens at paint time in the W7-C2
//  renderer (RenderCommon.h ResolveColor) -- exactly the schema-note-4 rule
//  CtlColorEngine::ResolveTColor implements for non-custom controls. So the
//  dedicated has_*/value colour fields are pushed AS-IS into the core
//  setters, matching the canonical usage in headless_ctrl_probe.cpp:151-152
//  (btn.Core().SetTrueColor(clRed) -- a raw TColor, no GetSysColor). Only
//  has_* == true fields are pushed; an unresolved token (has_* false, token
//  non-empty) is skipped per D-GA4-7's rule, leaving the core's own ctor
//  state -- which is golden's own behaviour for an absent .dfm property.
//
//  BAG DISCIPLINE. row.custom_props is a SLICE into the form-wide flat
//  k<FormStem>CustomProps[] array bounded by custom_prop_count
//  (DfmLayoutTypes.h:209-211) -- iteration is strictly [0, count), never
//  beyond. Keys can only come from emit_layout.py CUSTOM_PROP_KEYS (:57-61).
//  The 4 colour keys also ride in the bag "for uniform/generic lookup"
//  (schema note 5) but the schema's own DfmCustomProp comment directs
//  consumers to the dedicated resolved fields instead -- so the bag loop
//  SKIPS them (they were already pushed, resolved, from the dedicated
//  fields; re-parsing "clRed" here would duplicate colorconsts.py badly).
//
//  BAG ORDER vs GOLDEN STREAMING ORDER. The generator emits bag entries in
//  CUSTOM_PROP_KEYS order, not .dfm textual order, and the dedicated-first
//  colour push reorders further. That is safe for FINAL state: every core
//  setter involved is last-write-wins on its own field, and the only
//  cross-field effects (LedCore::SetValue/SetBlink driving ColorTemp and
//  the timer; BtnPanel SetTrueColor/SetFalseColor re-deriving Color from
//  bDown) depend on the VALUES present after all pushes, not on push order.
//  Verified against LedCore.h:95-106 / BtnPanelCore.h:83-98 /
//  TrayCore.cpp (every geometry setter re-runs CaculateTrayParameter
//  itself, e.g. SetXItem at TrayCore.cpp:73-85, so no ordering hole there
//  either).
//
//  PUSH AFTER SUBCLASS, BY DESIGN. SubclassWindow runs PreSubclassWindow
//  first: CLedCtrl's arms the blink timer machinery (CLedCtrl.cpp:115-120)
//  and CTrayCtrl's replays golden CreateWnd -- ApplyExtents (window-extent
//  SetExtents) then ClearCell (CTrayCtrl.cpp:65-73, HTray.cpp:62-68). Cell
//  GRID props (XItem/YItem/...) are pushed after that and recalc geometry
//  inside the core, so the ClearCell-wipes-cells ordering only matters for
//  cell DATA, which this pass never writes (no cell data exists in a .dfm).
//
//  LED Value/Blink/Interval go through the SHELL forwarders (CLedCtrl.h:67-
//  70) because those setters have timer side effects the shell must observe
//  (SyncTimer); direct Core() mutations (colours, LEDStyle, tray grid,
//  btnpanel colours) are followed by one SyncFromCore(), the documented
//  contract for driving Core() directly (CLedCtrl.h:37-40).
//
//  CAPTION: CBtnPanelCtrl::DrawInto pulls its text via GetWindowText at
//  paint time (CBtnPanelCtrl.cpp:83-84) and the dialog template already
//  carries the .dfm Caption as window text -- so the attach pass syncs NO
//  caption, per the header's "caption already in the template".
//
//  STATS HONESTY (probe gate D-GA4-9 item 5): leds/btnpanels/trays count
//  only successfully attached shells; every shell-backed row that could NOT
//  be attached (no HWND, SubclassWindow refused, non-LEAF kind anomaly)
//  increments `missing` and leaves a BootLog line -- never silently skipped.
// ===========================================================================
#include "CustomCtrlAttach.h"

#include "CLedCtrl.h"
#include "CBtnPanelCtrl.h"
#include "CTrayCtrl.h"
#include "Public/cBootLog.h"     // WriteBootLog -- same facility HT9045App.cpp uses

#include <stdlib.h>              // atoi / strtol for the bag's decimal ints
#include <string.h>              // strcmp
#include <vector>

namespace ht9045_ui {

namespace {

// ---------------------------------------------------------------------------
//  vcl_class -> shell family. Strings verbatim from emit_layout.py
//  CUSTOM_PROP_CLASSES (:52-56); re-verified against the live table (grep of
//  main_layout.gen.cpp's distinct vcl_class values, 2026-08-04: TALed 222,
//  TMyLed 6, TBtnPanel 11, TTMyTray 56 -- the exact D-GA4-8 census;
//  TMyLedLane / TBtnPanelLane / TTMyTray256 have 0 fMain rows but exist on
//  other forms / dynamically, and this engine serves all 133 forms).
// ---------------------------------------------------------------------------
enum ShellFamily { FAM_NONE, FAM_LED, FAM_BTNPANEL, FAM_TRAY };

struct ShellClassMapEntry { const char* vcl_class; ShellFamily fam; };

const ShellClassMapEntry kShellClassMap[] = {
    { "TALed",         FAM_LED      },
    { "TMyLed",        FAM_LED      },
    { "TMyLedLane",    FAM_LED      },
    { "TBtnPanel",     FAM_BTNPANEL },
    { "TBtnPanelLane", FAM_BTNPANEL },
    { "TTMyTray",      FAM_TRAY     },
    { "TTMyTray256",   FAM_TRAY     },
};

ShellFamily FamilyOf(const char* vclClass)
{
    if (vclClass == NULL)
        return FAM_NONE;
    for (int i = 0; i < (int)(sizeof(kShellClassMap) / sizeof(kShellClassMap[0])); ++i)
        if (::strcmp(kShellClassMap[i].vcl_class, vclClass) == 0)
            return kShellClassMap[i].fam;
    return FAM_NONE;
}

// ---------------------------------------------------------------------------
//  Bag-value decoders. The generator's _prop_scalar_text renders IR scalars
//  as: bools -> "True"/"False" (Python bool repr matches .dfm's tokens),
//  ints -> decimal, enum identifiers -> verbatim ("LEDSqLarge", "csLeftTop").
// ---------------------------------------------------------------------------
bool BoolFromToken(const char* v, bool& out)
{
    if (::strcmp(v, "True")  == 0) { out = true;  return true; }
    if (::strcmp(v, "False") == 0) { out = false; return true; }
    return false;
}

//AI(W906-GA4) 20260804: token -> enum of the SAME name, both sides quoting
// golden aled.pas:23's declared order (LedCore.h:66-69).
bool LedStyleFromToken(const char* v, vclcompat::TLEDStyle& out)
{
    if (::strcmp(v, "LEDSmall")      == 0) { out = vclcompat::LEDSmall;      return true; }
    if (::strcmp(v, "LEDLarge")      == 0) { out = vclcompat::LEDLarge;      return true; }
    if (::strcmp(v, "LEDSqSmall")    == 0) { out = vclcompat::LEDSqSmall;    return true; }
    if (::strcmp(v, "LEDSqLarge")    == 0) { out = vclcompat::LEDSqLarge;    return true; }
    if (::strcmp(v, "LEDVertical")   == 0) { out = vclcompat::LEDVertical;   return true; }
    if (::strcmp(v, "LEDHorizontal") == 0) { out = vclcompat::LEDHorizontal; return true; }
    return false;
}

//AI(W906-GA4) 20260804: token -> TTrayDirectStyle, golden HTray.h:14's
// declared order (TrayCore.h:75). fMain carries no DirectStyle rows (bag
// census 2026-08-04) but other forms may -- the engine is form-agnostic.
bool TrayDirectFromToken(const char* v, vclcompat::TTrayDirectStyle& out)
{
    if (::strcmp(v, "csNull")        == 0) { out = vclcompat::csNull;        return true; }
    if (::strcmp(v, "csLeftTop")     == 0) { out = vclcompat::csLeftTop;     return true; }
    if (::strcmp(v, "csLeftBottom")  == 0) { out = vclcompat::csLeftBottom;  return true; }
    if (::strcmp(v, "csRightTop")    == 0) { out = vclcompat::csRightTop;    return true; }
    if (::strcmp(v, "csRightBottom") == 0) { out = vclcompat::csRightBottom; return true; }
    return false;
}

bool IsDedicatedColorKey(const char* key)
{
    // emit_layout.py CUSTOM_COLOR_KEYS (:65) -- the 4 keys that ride the bag
    // AND own dedicated resolved fields; the bag copies are skipped (banner).
    return ::strcmp(key, "TrueColor")      == 0
        || ::strcmp(key, "FalseColor")     == 0
        || ::strcmp(key, "TrueFontColor")  == 0
        || ::strcmp(key, "FalseFontColor") == 0;
}

//AI(W906-GA4) 20260804: BootLog once per distinct key text (task rule) --
// keys point into the generated table's static storage but are compared by
// content so the same key on two forms' tables still logs once per pass.
void LogUnknownKeyOnce(std::vector<const char*>& logged, const char* key, const char* dfmPath)
{
    for (size_t i = 0; i < logged.size(); ++i)
        if (::strcmp(logged[i], key) == 0)
            return;
    logged.push_back(key);
    WriteBootLog(AnsiString("AttachCustomControls UNHANDLED custom-prop key: ") + key,
                 AnsiString("first seen on ") + dfmPath);
}

// ---------------------------------------------------------------------------
//  Per-family prop pushes. Dedicated colour fields first (resolved by the
//  generator, pushed raw -- file banner), then the generic bag in its stored
//  order, skipping the 4 colour keys. Order is final-state-safe (banner).
// ---------------------------------------------------------------------------
void PushLedProps(const ht9045_layout::DfmControlLayout& row, CLedCtrl* pShell,
                  std::vector<const char*>& loggedKeys)
{
    if (row.has_true_color)
        pShell->Core().SetTrueColor((vclcompat::TColor)row.true_color);
    if (row.has_false_color)
        pShell->Core().SetFalseColor((vclcompat::TColor)row.false_color);
    if (row.has_true_font_color || row.has_false_font_color)
    {
        //AI(W906-GA4) 20260804: aled.pas publishes NO font colours
        // (LedCore.h:94-106 setter census), so a .dfm cannot legally carry
        // them on an LED row -- if one ever appears it is a generator
        // anomaly, surfaced rather than silently dropped.
        WriteBootLog("AttachCustomControls LED row carries font colours but LedCore has no such setters",
                     row.dfm_path);
    }

    for (int k = 0; k < row.custom_prop_count; ++k)
    {
        const ht9045_layout::DfmCustomProp& pr = row.custom_props[k];
        if (IsDedicatedColorKey(pr.key))
            continue;                               // pushed above, resolved
        if (::strcmp(pr.key, "LEDStyle") == 0)
        {
            vclcompat::TLEDStyle eStyle;
            if (LedStyleFromToken(pr.value, eStyle))
                pShell->Core().SetLEDStyle(eStyle);
            else
                WriteBootLog(AnsiString("AttachCustomControls unparseable LEDStyle '") + pr.value + "'",
                             row.dfm_path);
        }
        else if (::strcmp(pr.key, "Value") == 0 || ::strcmp(pr.key, "Blink") == 0)
        {
            //AI(W906-GA4) 20260804: "Blink" is handled per the task contract
            // but is unreachable today -- it is absent from emit_layout.py
            // CUSTOM_PROP_KEYS (:57-61, the empirical corpus-wide .dfm key
            // set), so no generated bag can carry it. Kept because it costs
            // two strcmps and removes a trap if the key set ever grows.
            bool b;
            if (!BoolFromToken(pr.value, b))
            {
                WriteBootLog(AnsiString("AttachCustomControls unparseable bool '") + pr.value + "'",
                             row.dfm_path);
            }
            else if (pr.key[0] == 'V')
                pShell->SetValue(b);                // shell forwarder: timer side effect
            else
                pShell->SetBlink(b);                // ditto
        }
        else if (::strcmp(pr.key, "Interval") == 0)
        {
            pShell->SetInterval(::strtol(pr.value, NULL, 10));  // shell forwarder
        }
        else
        {
            // e.g. "Alias": a real CUSTOM_PROP_KEYS member, but CLedCtrl's
            // Core() is the base LedCore, which has no SetAlias (that lives
            // on MyLedCore/MyLedLaneCore, which the facade wave will own --
            // CLedCtrl.h:42-47 AttachCore note). Logged, not lost silently.
            LogUnknownKeyOnce(loggedKeys, pr.key, row.dfm_path);
        }
    }
    pShell->SyncFromCore();     // contract for direct Core() mutation, CLedCtrl.h:73
}

void PushBtnPanelProps(const ht9045_layout::DfmControlLayout& row, CBtnPanelCtrl* pShell,
                       std::vector<const char*>& loggedKeys)
{
    // All four dedicated colours exist on BtnPanelCoreBase (BtnPanelCore.h:
    // 87-90). Pushing FalseColor also refreshes the visible Color while the
    // panel is up (golden SetFalseColor, butPa1.cpp:60-65) -- the correct
    // design-time appearance.
    if (row.has_true_color)
        pShell->Core().SetTrueColor((vclcompat::TColor)row.true_color);
    if (row.has_false_color)
        pShell->Core().SetFalseColor((vclcompat::TColor)row.false_color);
    if (row.has_true_font_color)
        pShell->Core().SetTrueFontColor((vclcompat::TColor)row.true_font_color);
    if (row.has_false_font_color)
        pShell->Core().SetFalseFontColor((vclcompat::TColor)row.false_font_color);

    for (int k = 0; k < row.custom_prop_count; ++k)
    {
        const ht9045_layout::DfmCustomProp& pr = row.custom_props[k];
        if (IsDedicatedColorKey(pr.key))
            continue;                               // pushed above, resolved
        // BtnPanel takes colours only (task ruling; caption needs no sync --
        // file banner). Anything else ("Alias": Core() is the base class,
        // no SetAlias surface) is logged for the facade wave.
        LogUnknownKeyOnce(loggedKeys, pr.key, row.dfm_path);
    }
    pShell->SyncFromCore();
}

void PushTrayProps(const ht9045_layout::DfmControlLayout& row, CTrayCtrl* pShell,
                   std::vector<const char*>& loggedKeys)
{
    if (row.has_true_color || row.has_false_color ||
        row.has_true_font_color || row.has_false_font_color)
    {
        //AI(W906-GA4) 20260804: golden TTMyTray publishes no True*/False*
        // colours (TrayCore.h:121-137 setter census) -- same anomaly guard
        // as the LED font colours above.
        WriteBootLog("AttachCustomControls Tray row carries True*/False* colours but TrayCore has no such setters",
                     row.dfm_path);
    }

    vclcompat::TrayCore& core = pShell->Core();     // internal 100-cell core:
                                                    // no .dfm row is a Tray256
                                                    // (file banner), so Is256()
                                                    // is false here by construction.
    for (int k = 0; k < row.custom_prop_count; ++k)
    {
        const ht9045_layout::DfmCustomProp& pr = row.custom_props[k];
        if (IsDedicatedColorKey(pr.key))
            continue;
        if      (::strcmp(pr.key, "XItem")       == 0) core.SetXItem(::atoi(pr.value));
        else if (::strcmp(pr.key, "YItem")       == 0) core.SetYItem(::atoi(pr.value));
        else if (::strcmp(pr.key, "DirectWidth") == 0) core.SetDirectWidth(::atoi(pr.value));
        else if (::strcmp(pr.key, "EdgeWidth")   == 0) core.SetEdgeWidth(::atoi(pr.value));
        else if (::strcmp(pr.key, "LineWidth")   == 0) core.SetLineWidth(::atoi(pr.value));
        else if (::strcmp(pr.key, "DirectStyle") == 0)
        {
            vclcompat::TTrayDirectStyle eDir;
            if (TrayDirectFromToken(pr.value, eDir))
                core.SetTrayDirect(eDir);
            else
                WriteBootLog(AnsiString("AttachCustomControls unparseable DirectStyle '") + pr.value + "'",
                             row.dfm_path);
        }
        else
        {
            LogUnknownKeyOnce(loggedKeys, pr.key, row.dfm_path);
        }
    }
    pShell->SyncFromCore();
}

} // anonymous namespace

// ---------------------------------------------------------------------------
//  The pass. One walk over the form's layout rows; shell-backed rows get a
//  shell, everything else is untouched. Failure modes all land in `missing`
//  with a BootLog line (file banner: stats honesty).
// ---------------------------------------------------------------------------
AttachStats AttachCustomControls(FormRuntime& rt)
{
    AttachStats st = { 0, 0, 0, 0 };
    std::vector<const char*> loggedKeys;    // once-per-key log memory (per pass)

    const ht9045_layout::DfmControlLayout* pRows = rt.Rows();
    const int nRows = rt.RowCount();
    for (int i = 0; i < nRows; ++i)
    {
        const ht9045_layout::DfmControlLayout& row = pRows[i];
        const ShellFamily fam = FamilyOf(row.vcl_class);
        if (fam == FAM_NONE)
            continue;                       // not shell-backed (placeholders stay inert holes)

        if (::strcmp(row.kind, "LEAF") != 0)
        {
            //AI(W906-GA4) 20260804: every custom-control row in the corpus
            // is LEAF (classmap only promotes the LITERAL class "TPanel"
            // with children -- not TPanel descendants -- to SUBDLG, schema
            // note 2); a non-LEAF here would make HwndByPath return a
            // DIALOG's HWND, and subclassing a dialog with a control shell
            // corrupts it. Counted as missing: the row truthfully got no shell.
            WriteBootLog(AnsiString("AttachCustomControls shell-backed row has non-LEAF kind ") + row.kind,
                         row.dfm_path);
            ++st.missing;
            continue;
        }

        HWND h = rt.HwndByPath(row.dfm_path);
        if (h == NULL)
        {
            WriteBootLog("AttachCustomControls MISSING HWND for shell-backed row", row.dfm_path);
            ++st.missing;
            continue;
        }

        //AI(W906-GA4) 20260804: SubclassWindow (not Attach) so the MFC proc
        // replaces the registered DefWindowProc -- PreSubclassWindow runs
        // inside it, which is why every prop push sits AFTER this call
        // (file banner). On refusal (alien-thread HWND / already-attached
        // CWnd) the shell never reached the window: delete it here -- it is
        // NOT handed to rt.AddShell, whose Destroy() would Unsubclass a
        // window this shell never subclassed.
        switch (fam)
        {
        case FAM_LED:
        {
            CLedCtrl* pShell = new CLedCtrl();
            if (!pShell->SubclassWindow(h))
            {
                WriteBootLog("AttachCustomControls SubclassWindow FAILED (LED)", row.dfm_path);
                delete pShell;
                ++st.missing;
                break;
            }
            rt.AddShell(pShell);            // FormRuntime owns from here on
            ++st.leds;
            PushLedProps(row, pShell, loggedKeys);
            break;
        }
        case FAM_BTNPANEL:
        {
            CBtnPanelCtrl* pShell = new CBtnPanelCtrl();
            if (!pShell->SubclassWindow(h))
            {
                WriteBootLog("AttachCustomControls SubclassWindow FAILED (BtnPanel)", row.dfm_path);
                delete pShell;
                ++st.missing;
                break;
            }
            rt.AddShell(pShell);
            ++st.btnpanels;
            PushBtnPanelProps(row, pShell, loggedKeys);
            break;
        }
        case FAM_TRAY:
        {
            CTrayCtrl* pShell = new CTrayCtrl();
            if (!pShell->SubclassWindow(h))
            {
                WriteBootLog("AttachCustomControls SubclassWindow FAILED (Tray)", row.dfm_path);
                delete pShell;
                ++st.missing;
                break;
            }
            rt.AddShell(pShell);
            ++st.trays;
            PushTrayProps(row, pShell, loggedKeys);
            break;
        }
        default:
            break;                          // unreachable: FAM_NONE handled above
        }
    }
    return st;
}

} // namespace ht9045_ui
