# -*- coding: utf-8 -*-
"""
classmap.py -- W7-B1b: VCL class -> Win32 dialog-resource CONTROL mapping.

AI(W906-W7-B1b) 20260728: new file, second stage of the .dfm-to-MFC pipeline
(plan doc: HT9011UC_Cpp_V3.33.906.0/docs/W7_UI_ARCHITECTURE_PLAN.md, S6
"W7-B1b -- .rc + _ids.h 產生器 + 閘 G5/G6"). Pure data: the class-map table
from the plan's own S6-B1b section, expressed as Python structures so
emit_rc.py (the generator) and check_g6.py (the independent verifier) can
both consume the same declared ground truth without hand-copying it twice.

MECHANICAL CHOICE (documented, not implicit): every single CONTROL in every
generated .rc uses the generic RC `CONTROL text, id, class, style, x, y, cx,
cy` statement form -- never the shorthand statements (LTEXT/PUSHBUTTON/
CHECKBOX/EDITTEXT/COMBOBOX/...). The shorthand forms bake in fixed style
bits and have quirky/inconsistent parameter orders across statement kinds;
CONTROL is the single uniform statement that is valid for every stock class
*and* for arbitrary registered custom class strings (this is exactly what
D12 already requires for the 7 self-made components), so using it
everywhere keeps the emitter one code path instead of ~10.

STYLE BITS: kept as symbolic string tokens (e.g. 'SS_LEFTNOWORDWRAP') that
emit_rc.py joins with '|' verbatim into the .rc text; they resolve via
<windows.h>'s SS_*/ES_*/BS_*/CBS_*/LBS_* macros (see A0/vcl_compat lineage
note in emit_rc.py's own docstring for why <windows.h> is included, and why
DS_CONTROL is hand-#defined instead of pulling afxres.h -- MFC's resource
header is explicitly out of reach at this wave, see plan S6-B1b DIALOGEX
rule + G10 note).

COMMON CONTROLS: per plan S6-B1b + S9-R2 (independently reproduced --
scratch probe verified `windows.h`'s `WC_TABCONTROL`-style macros expand to
wide-string literals that `windres` swallows silently but `rc.exe` rejects
with RC2104), every common-control class string here is a *literal string*
("SysTabControl32", "msctls_trackbar32", ...), never a `WC_*`/`*_CLASS`
macro.

CUSTOM/NO-CLEAN-MAPPING CLASSES: emitted as `CONTROL "", id,
"HT9045.<Name>", ...` per D12. Two of these (TEasyMain/TECameraPicolo) are
third-party frame-grabber SDK singletons (Euresys) that plan S9-R5 says go
straight into the SKIP report rather than being treated as translatable UI
-- they are still emitted as ordinary CONTROL entries here (so the .dfm
census/geometry gates never see a dropped node), just flagged via
SKIP_VENDOR_CLASSES so downstream tooling can report them separately.
"""
from __future__ import print_function

# ---------------------------------------------------------------------------
# Categories (informational; not consumed for control-flow, only for
# grouping/reporting)
# ---------------------------------------------------------------------------
STATIC = 'STATIC'
EDIT = 'EDIT'
BUTTON = 'BUTTON'
COMBO = 'COMBO'
LISTBOX = 'LISTBOX'
SCROLLBAR = 'SCROLLBAR'
COMMCTRL = 'COMMCTRL'
CUSTOM = 'CUSTOM'


class ClassSpec(object):
    __slots__ = ('category', 'win32_class', 'styles', 'synthesizes_label')

    def __init__(self, category, win32_class, styles, synthesizes_label=False):
        self.category = category
        self.win32_class = win32_class          # literal string for the CONTROL class field
        self.styles = list(styles)               # list of symbolic WS_*/BS_*/... tokens
        self.synthesizes_label = synthesizes_label


def _custom(name):
    return ClassSpec(CUSTOM, 'HT9045.%s' % name, [])


# ---------------------------------------------------------------------------
# LEAF / TABHOST class -> ClassSpec. (SUBDLG-kind classes are NOT looked up
# here for their own CONTROL entry -- per D8 they never get one in the
# *parent* template; see CONTAINER_FRAME_CLASSES / CONTAINER_RADIO_SYNTH
# below for what DOES get emitted *inside their own* child DIALOGEX.)
# ---------------------------------------------------------------------------
CLASS_MAP = {
    # ---- Static family ----
    'TLabel':      ClassSpec(STATIC, 'Static', ['SS_LEFTNOWORDWRAP', 'WS_GROUP']),
    'TBevel':      ClassSpec(STATIC, 'Static', ['SS_ETCHEDFRAME']),
    'TSplitter':   ClassSpec(STATIC, 'Static', ['SS_ETCHEDFRAME']),
    'TImage':      ClassSpec(STATIC, 'Static', ['SS_BITMAP', 'SS_REALSIZECONTROL']),
    'TEBW8Image':  ClassSpec(STATIC, 'Static', ['SS_BITMAP', 'SS_REALSIZECONTROL']),
    # a childless TPanel is classified LEAF by dfm_parse.classify_kind (only
    # a TPanel *with children* becomes SUBDLG) -- it is a plain decorative
    # rectangle in golden, approximated here as a borderless static; real
    # bevel/color fidelity is explicitly out of scope for the .rc (D7/R10).
    'TPanel':      ClassSpec(STATIC, 'Static', []),

    # ---- Edit family ----
    'TEdit':          ClassSpec(EDIT, 'Edit', ['ES_AUTOHSCROLL', 'WS_BORDER']),
    'TLabeledEdit':   ClassSpec(EDIT, 'Edit', ['ES_AUTOHSCROLL', 'WS_BORDER'], synthesizes_label=True),
    'TMemo':          ClassSpec(EDIT, 'Edit', ['ES_MULTILINE', 'ES_AUTOVSCROLL', 'WS_VSCROLL', 'WS_BORDER']),
    'TMyMemo':        ClassSpec(EDIT, 'Edit', ['ES_MULTILINE', 'ES_AUTOVSCROLL', 'WS_VSCROLL', 'WS_BORDER']),
    'TRichEdit':      ClassSpec(EDIT, 'RICHEDIT50W', ['ES_MULTILINE', 'WS_BORDER']),

    # ---- Button family ----
    'TButton':       ClassSpec(BUTTON, 'Button', ['BS_PUSHBUTTON']),
    'TBitBtn':       ClassSpec(BUTTON, 'Button', ['BS_PUSHBUTTON', 'BS_BITMAP']),
    'TSpeedButton':  ClassSpec(BUTTON, 'Button', ['BS_OWNERDRAW']),
    'TCheckBox':     ClassSpec(BUTTON, 'Button', ['BS_AUTOCHECKBOX']),
    'TRadioButton':  ClassSpec(BUTTON, 'Button', ['BS_AUTORADIOBUTTON']),

    # ---- List family ----
    'TComboBox':          ClassSpec(COMBO, 'ComboBox', ['CBS_DROPDOWNLIST', 'WS_VSCROLL']),
    'TDriveComboBox':     ClassSpec(COMBO, 'ComboBox', ['CBS_DROPDOWNLIST', 'WS_VSCROLL']),
    'TListBox':           ClassSpec(LISTBOX, 'ListBox', ['LBS_NOTIFY', 'WS_VSCROLL', 'WS_BORDER']),
    'TFileListBox':       ClassSpec(LISTBOX, 'ListBox', ['LBS_NOTIFY', 'WS_VSCROLL', 'WS_BORDER']),
    'TDirectoryListBox':  ClassSpec(LISTBOX, 'ListBox', ['LBS_NOTIFY', 'WS_VSCROLL', 'WS_BORDER']),
    'TCheckListBox':      ClassSpec(LISTBOX, 'ListBox', ['LBS_OWNERDRAWFIXED', 'LBS_HASSTRINGS']),

    # ---- Common controls (literal class-name strings only, see S9-R2) ----
    'TPageControl':     ClassSpec(COMMCTRL, 'SysTabControl32', []),
    'TTrackBar':        ClassSpec(COMMCTRL, 'msctls_trackbar32', []),
    'TUpDown':          ClassSpec(COMMCTRL, 'msctls_updown32', []),
    'TProgressBar':     ClassSpec(COMMCTRL, 'msctls_progress32', []),
    'TDateTimePicker':  ClassSpec(COMMCTRL, 'SysDateTimePick32', []),
    'TMonthCalendar':   ClassSpec(COMMCTRL, 'SysMonthCal32', []),
    'TTreeView':        ClassSpec(COMMCTRL, 'SysTreeView32', []),
    'TStatusBar':       ClassSpec(COMMCTRL, 'msctls_statusbar32', []),
    'TScrollBar':       ClassSpec(SCROLLBAR, 'ScrollBar', []),

    # ---- 7 self-made VCL components (D5/D12): registered custom classes,
    # component-specific props (Alias/LEDStyle/XItem/YItem/True*/False*)
    # ride the B1c layout table, not the .rc. ----
    'TALed':          _custom('ALed'),
    'TMyLed':         _custom('MyLed'),
    'TMyLedLane':     _custom('MyLedLane'),
    'TBtnPanel':      _custom('BtnPanel'),
    'TBtnPanelLane':  _custom('BtnPanelLane'),
    'TTMyTray':       _custom('MyTray'),
    'TTMyTray256':    _custom('MyTray256'),   # 0 .dfm instances (dynamic-only, S4-V12) but kept for completeness

    # ---- 9-class "no clean mapping" set (S9-R5) ----
    'TStringGrid':      _custom('StringGrid'),  # NOT CListCtrl/LVS_REPORT -- see S9-R5, targets the existing headless vclcompat::TStringGrid shim
    'TDBGrid':          _custom('StringGrid'),  # same shim target as TStringGrid (1 instance)
    'TShape':           _custom('Shape'),
    'TChart':           _custom('Chart'),
    'TCppWebBrowser':   _custom('CppWebBrowser'),
    'TMediaPlayer':     _custom('MediaPlayer'),
    'TEBW8ROI':         _custom('EBW8ROI'),
    'TEasyMain':        _custom('EasyMain'),        # vendor SDK singleton -- SKIP-listed, see SKIP_VENDOR_CLASSES
    'TECameraPicolo':   _custom('ECameraPicolo'),   # vendor SDK singleton -- SKIP-listed, see SKIP_VENDOR_CLASSES

    # ---- Source-not-available decorative control (S6 SKIP-AND-RECORD) ----
    'TAPHeadLabel':   _custom('APHeadLabel'),
}

# Third-party SDK singleton classes explicitly called out by plan S9-R5 as
# "not a translatable UI surface" -- reported into docs/W7-UI-SKIPPED.md by
# the driver, not silently dropped from the .rc (they still get a CONTROL
# entry above so census/geometry gates stay whole).
SKIP_VENDOR_CLASSES = frozenset(['TEasyMain', 'TECameraPicolo'])

# Approximate/not-a-faithful-port control (source doesn't exist -- S4-V12 /
# S6 SKIP-AND-RECORD). Also reported, not silently dropped.
SKIP_NO_SOURCE_CLASSES = frozenset(['TAPHeadLabel'])

# ---------------------------------------------------------------------------
# SUBDLG-only decoration rules (emit_rc.py applies these when *emitting a
# dialog owner's own CONTROL list*, i.e. these are never looked up via
# CLASS_MAP because a SUBDLG-kind node never gets a CONTROL entry in its
# *parent*; these two sets say what it gets inside *its own* child dialog).
# ---------------------------------------------------------------------------

# TGroupBox / TRadioGroup draw their own frame+caption inside their own
# child dialog template at local (0,0,w,h) -- plan S6-B1b DIALOGEX rule
# ("TGroupBox 另在自己的 template 內 (0,0,w,h) 放一個 GROUPBOX 畫框").
CONTAINER_FRAME_CLASSES = frozenset(['TGroupBox', 'TRadioGroup'])

# TRadioGroup additionally synthesizes N RADIOBUTTON entries from
# Items.Strings -- this is synthesis rule (1) of the exactly-two rules
# plan S6-B1d permits (802 expected instances across the corpus); B1b
# must implement it because otherwise ~336 generated dialogs would compile
# with zero interactive content, which is not "structurally faithful" (D7).
CONTAINER_RADIO_SYNTH_CLASSES = frozenset(['TRadioGroup'])

# TTabSheet carries no Left/Top/Width/Height in the .dfm at all (it is
# sized to its TPageControl parent's display rect at runtime) -- synthesis
# rule (2) of the plan's two permitted rules. Geometry is synthesized from
# the parent TABHOST node's own (Width,Height) per emit_rc.py.
GEOMETRY_SYNTH_CLASSES = frozenset(['TTabSheet'])

# Predefined Win32 dialog control class atoms (winuser.h) -- rc.exe folds
# the *literal string* class names below into these ordinals in the
# compiled .res (empirically verified, see docs/W7-UI-SKIPPED.md smoke
# test log / scratch probe: CONTROL "Static", ... round-trips as ORD 0x82).
# check_g6.py needs this table to recognise that an ORD atom in the binary
# is the *expected* encoding of "Static"/"Edit"/etc, not a mismatch.
PREDEFINED_CLASS_ORDINALS = {
    0x0080: 'Button',
    0x0081: 'Edit',
    0x0082: 'Static',
    0x0083: 'ListBox',
    0x0084: 'ScrollBar',
    0x0085: 'ComboBox',
}
PREDEFINED_CLASS_NAME_TO_ORD = dict((v, k) for k, v in PREDEFINED_CLASS_ORDINALS.items())


def lookup(cls_name):
    """Return a ClassSpec for a LEAF/TABHOST VCL class, falling back to an
    auto-derived custom registered class for anything not in the curated
    table (should never trigger against this corpus -- all 63 LEAF/TABHOST
    classes measured in the IR are covered above -- but a generator must
    never crash on an unrecognised class, so this is a documented,
    logged-by-caller safety net, not silent success)."""
    spec = CLASS_MAP.get(cls_name)
    if spec is not None:
        return spec, False
    return _custom(cls_name.lstrip('T') or cls_name), True
