# -*- coding: utf-8 -*-
r"""
emit_rc.py -- W7-B1b: `.rc` + `_ids.h` generator, third stage of the
.dfm-to-MFC pipeline.

AI(W906-W7-B1b) 20260728: new file (plan doc:
HT9011UC_Cpp_V3.33.906.0/docs/W7_UI_ARCHITECTURE_PLAN.md, S6
"W7-B1b -- .rc + _ids.h 產生器 + 閘 G5/G6"). Consumes a W7-B1a `.dfm.ir.json`
IR file and emits, per form: a self-contained ASCII `.rc` (MFC dialog
resource script, structurally faithful per D8/D12; geometry is a
non-authoritative placeholder per D7) and a `_ids.h` control-ID header
(D11 join key). A companion `<Form>.rcmeta.json` sidecar records exactly
what was emitted (dialog/control id -> expected class/caption/synthesized
flag), generated fresh from the IR on every run, for check_g6.py's
independent post-compilation diff.

===========================================================================
DESIGN DECISIONS THIS FILE MAKES (read before modifying)
===========================================================================

1. REAL HWND NESTING, NO FLATTENING (D8). A "dialog owner" is either the
   form ROOT or any SUBDLG-kind node (TPanel-with-kids / TGroupBox /
   TScrollBox / TTabSheet / TRadioGroup / TframeProdInfo). Each dialog
   owner gets its own `DIALOGEX` block. A TABHOST-kind node (TPageControl)
   is *not* a dialog owner -- it is a real, ordinary CONTROL entry in its
   *own* parent's template (a SysTabControl32 leaf-like control); its
   SUBDLG children (TTabSheet) are still separate dialog owners, reached
   by walking through the TABHOST rather than stopping at it.

   A dialog owner's CONTROL list is built from *every LEAF/TABHOST node in
   its subtree, at any depth, stopping only at a nested SUBDLG boundary*:
     - LEAF / TABHOST children -> one CONTROL line each (classmap.py).
     - SUBDLG children -> *no* line at all (D8: "絕不在父 template 裡替
       容器放 placeholder Static" -- that exact mistake is the corpus's
       own recorded self-authored bug, DUP_ID_IN_RES, cited in the plan).
       They are simply enqueued as a new dialog owner to be emitted
       separately; their parent/position lives in the B1c layout table.
     - NONVISUAL children -> excluded entirely (no .rc trace whatsoever,
       matching B1a's G1 census gate's one documented asymmetry).
   AI(W906-W7-B1-review) 20260728: the "at any depth" part was a bug fix,
   not original design -- an earlier version of this file only looked at a
   dialog owner's *direct* children, so a LEAF node that itself had further
   .dfm children (a real corpus shape: TChart is LEAF per classmap.py, yet
   still a genuine windowed control that can parent further controls
   placed on its surface -- `cObserver.dfm:1919` ChartYield containing
   SpeedButton1/edYieldMax/edYieldMin, `DynamicTemp.dfm:43` Chart1
   containing edMax/edMin) had those grandchildren silently vanish from
   the .rc entirely: not emitted as a CONTROL anywhere, and not queued as a
   new dialog owner either (LEAF never becomes one). See
   `_enqueue_nested_owners` / `direct_leaf_entries` below for the fix, and
   gates.py's `gate_rc_census` for the new gate this failure mode is now
   checked against (proven to fail against the old code, per
   docs/W7_UI_ARCHITECTURE_PLAN.md S6-B1d's "prove the gate is load-bearing"
   standard, before this fix was applied).

2. GEOMETRY IS A LITERAL, UNCONVERTED PIXEL COPY (D7). The plan measured
   px->DLU->px round-tripping as pixel-exact for only 17.2-17.4% of
   controls, and made the `_layout.gen.cpp` table (B1c, a sibling wave)
   the sole geometry authority. Rather than manufacture an approximate
   DLU conversion formula that could be mistaken for meaningful, this
   generator copies the .dfm's literal integer Left/Top/Width/Height
   straight into the DIALOGEX/CONTROL x/y/cx/cy fields verbatim. The text
   format of a .rc treats those as plain integers -- rc.exe does not
   itself perform any unit conversion at compile time (verified: a
   several-hundred-pixel CONTROL compiles and round-trips through .res
   with the exact same integers, see check_g6.py) -- so this is both the
   simplest-to-audit choice and, as a side effect, gives every geometry
   field an *exact* round-trip through G6, not merely a class/caption one.
   AI(W906-W7-B1-review) 20260728: this geometry-round-trips-through-G6
   claim was written before check_g6.py's comparison loop (or this file's
   own .rcmeta.json sidecar) actually carried x/y/cx/cy at all -- i.e. it
   was false when first written (MEDIUM-1 finding). Now genuinely true:
   build_meta() below records each control's compiled-target (x,y,cx,cy)
   in the sidecar, and check_g6.py's check_one_form() diffs it against the
   real compiled DLGITEMTEMPLATEEX fields (res_parse.py's DlgItem already
   decodes these) -- see check_g6.py's own docstring for the verified
   wording.

3. STRING ESCAPING (S9-R4, independently re-verified against a live
   rc.exe before writing a single form -- see the session's scratch smoke
   tests): every text field is wrapped `L"..."` (the L *outside* the
   quotes -- writing it as the first character *inside* the string, e.g.
   `"L\x4E2D..."`, is a real mistake this generator's own author made and
   caught on the first compile: it silently produces `LN2De87` instead of
   the CJK text). Given a decoded IR string, three substitutions are
   applied in this order: literal `\` -> `\\`, literal `"` -> `""`
   (doubling, the classic RC/Pascal quote-escape -- `\"` is NOT accepted
   by rc.exe, it errors with RC2104), then every remaining character
   outside printable ASCII (0x20-0x7E) is rendered `\xHHHH` (4 hex
   digits, matching the plan's own worked example). All three rules were
   compiled and round-tripped through a real rc.exe + read back with
   res_parse.py before being trusted here.

4. SYNTHESIS -- exactly THREE rules, and they are the only three
   `gates.py::_ALLOWED_SYNTH_RULES` permits. Corpus-wide emitted counts
   (AI(W906-W7-C-followup) 20260728: counted directly from all 133
   `.rcmeta.json` sidecars, replacing an earlier version of this docstring
   that named a fourth rule `SYNTH_TABSHEET_GEOM` which does not exist
   anywhere in this code and is emitted 0 times, while omitting
   `SYNTH_LABELEDEDIT_LABEL` which is emitted 198 times):
     (a) `SYNTH_RADIOGROUP_ITEM` -- 2,012 entries. TRadioGroup -> N
         RADIOBUTTON entries from Items.Strings (first gets WS_GROUP per
         Win32 convention), ids `IDC_<FORM>_<GROUP>_ITEM<n>`. Without
         this the generated .rc would omit ~336 TRadioGroup dialogs'
         entire interactive content.
     (b) `SYNTH_LABELEDEDIT_LABEL` -- 198 entries. TLabeledEdit is one
         VCL control but two Win32 windows, so its caption becomes its
         own Static entry.
     (c) `SYNTH_CONTAINER_FRAME` -- 1,584 entries. TGroupBox/TRadioGroup
         drawing their own BS_GROUPBOX frame inside their own dialog at
         local (0,0,w,h). Purely decorative and arguably not an "invented
         control" at all (it is a mechanical consequence of D8 giving
         every container a real HWND that would otherwise have no visible
         boundary), but it is tagged anyway, for transparency.
   NOT a synthesis rule: TTabSheet sizing. A TTabSheet is SUBDLG-kind, so
   it is a dialog OWNER and its size lives in its own DIALOGEX header --
   no CONTROL entry is invented for it, hence no tag.
   Every synthesized entry carries `synthesized: true` plus its `rule`
   string in the sidecar, and `gates.py::gate_synth_predicted` asserts the
   synthesized set is *exactly* what these three rules predict, rejecting
   both extra entries and any unrecognised rule tag.

5. ONE FONT PER DIALOG (D7 -- DIALOGEX permits exactly one FONT
   statement, the corpus has 9,933 per-control fonts). Each dialog owner
   uses *its own node's* Font.Name/Font.Height/Font.Charset properties if
   present (VCL only stores an explicit override in the .dfm; true
   ParentFont inheritance-chain resolution is out of scope for this
   placeholder and is called out below), else falls back to the classic
   BCB6 default (8pt "MS Sans Serif", DEFAULT_CHARSET). Point size is
   derived from the VCL negative Font.Height via the standard
   `round(-Height * 72 / 96)` formula (96 dpi assumed, matching the
   corpus's own default -11 -> 8pt convention).

6. COLOR IS OUT OF SCOPE HERE. Native Win32 dialog resources have no
   declarative per-control foreground/background field in the CONTROL
   statement (colors are only ever applied at runtime via WM_CTLCOLOR /
   owner-draw, S7 item 3) -- so Color/Font.Color/TrueColor/FalseColor etc
   are read by B1c into the layout table, never referenced here.

7. DO-NOT-EDIT banners + idempotence. Every generated file states its
   source golden .dfm path and GENERATOR_VERSION; the generator is a pure
   function of the IR (no timestamps, no filesystem-order-dependent
   iteration), so re-running it produces byte-identical output -- the
   property W7-B1d's G7 idempotence gate checks for.
"""
from __future__ import print_function

import os
import re
import sys
import json
import math
import collections

import classmap as CM

GENERATOR_VERSION = 'emit_rc.py W7-B1b v1.0'

DS_SETFONT = 0x40  # local re-derivation for our own point-size math only;
                    # the *textual* DS_SETFONT token in generated .rc text
                    # resolves via <windows.h>, this constant is not textual.

# ---------------------------------------------------------------------------
# Identifier helpers
# ---------------------------------------------------------------------------

_IDENT_BAD = re.compile(r'[^A-Za-z0-9_]')


def sanitize_ident(s):
    s = _IDENT_BAD.sub('_', s)
    if not s or s[0].isdigit():
        s = '_' + s
    return s.upper()


def formstem(source_dfm):
    """.dfm relpath -> FORMSTEM per D11 ('main.dfm' -> 'MAIN',
    'cConfiguration.dfm' -> 'CCONFIGURATION')."""
    base = os.path.basename(source_dfm)
    stem = base[:-4] if base.lower().endswith('.dfm') else base
    return sanitize_ident(stem)


def containerpath_suffix(root_path, node_path):
    if node_path == root_path:
        return ''
    rel = node_path[len(root_path) + 1:]  # strip 'root.' prefix
    parts = rel.split('.')
    return '_' + '_'.join(sanitize_ident(p) for p in parts)


# ---------------------------------------------------------------------------
# RC wide-string escaping (S9-R4; see module docstring point 3 for the
# empirical verification this encodes).
# ---------------------------------------------------------------------------

def rc_wstr(text):
    if text is None:
        text = ''
    out = []
    for ch in text:
        cp = ord(ch)
        if ch == '\\':
            out.append('\\\\')
        elif ch == '"':
            out.append('""')
        elif 0x20 <= cp <= 0x7E:
            out.append(ch)
        else:
            out.append('\\x%04X' % cp)
    return u'L"%s"' % u''.join(out)


# ---------------------------------------------------------------------------
# ID allocation (D11: one control-id namespace per whole form tree
# including all child dialogs, starting 0x0400; one dialog-template-id
# namespace per form, starting 0x2000; each independently sequential).
# ---------------------------------------------------------------------------

class IdAllocator(object):
    def __init__(self, formstem_):
        self.formstem = formstem_
        self._next_idc = 0x0400
        self._next_idd = 0x2000
        self._used_symbols = {}   # symbol -> value (dedup safety net)

    def _register(self, symbol, value):
        if symbol in self._used_symbols:
            raise ValueError(
                'IdAllocator: duplicate symbol %r (existing=%d, new=%d) -- '
                'this should be impossible given D11 leaf-name uniqueness; '
                'a synthesized-name suffix must be colliding with a real '
                'control name' % (symbol, self._used_symbols[symbol], value))
        self._used_symbols[symbol] = value

    def new_idc(self, control_name_upper):
        symbol = 'IDC_%s_%s' % (self.formstem, control_name_upper)
        value = self._next_idc
        self._next_idc += 1
        self._register(symbol, value)
        return symbol, value

    def new_idd(self, root_path, node_path):
        suffix = containerpath_suffix(root_path, node_path)
        symbol = 'IDD_%s%s' % (self.formstem, suffix)
        value = self._next_idd
        self._next_idd += 1
        self._register(symbol, value)
        return symbol, value


# ---------------------------------------------------------------------------
# Font
# ---------------------------------------------------------------------------

DEFAULT_FONT_NAME = 'MS Sans Serif'
DEFAULT_FONT_HEIGHT = -11
DEFAULT_CHARSET = 1  # DEFAULT_CHARSET


def node_font(node):
    props = node['properties']
    name_pv = props.get('Font.Name')
    height_pv = props.get('Font.Height')
    name = name_pv['value'] if name_pv and name_pv['type'] == 'STR' else DEFAULT_FONT_NAME
    height = height_pv['value'] if height_pv and isinstance(height_pv['value'], int) else DEFAULT_FONT_HEIGHT
    pointsize = int(round(abs(height) * 72.0 / 96.0)) if height else 8
    if pointsize <= 0:
        pointsize = 8
    return pointsize, name


# ---------------------------------------------------------------------------
# Root/container size
# ---------------------------------------------------------------------------

def scalar(props, key, default=None):
    pv = props.get(key)
    if pv is None:
        return default
    return pv['value']


def root_size(root_node):
    props = root_node['properties']
    w = scalar(props, 'ClientWidth')
    h = scalar(props, 'ClientHeight')
    if not isinstance(w, int):
        geom = root_node.get('geometry') or {}
        w = geom.get('width') or 400
    if not isinstance(h, int):
        geom = root_node.get('geometry') or {}
        h = geom.get('height') or 300
    return w, h


TABSTRIP_MARGIN_PX = 22  # approximate SysTabControl32 tab-strip height at
                          # default font size; placeholder only (D7) -- the
                          # real per-form value is a B1c/rendering concern.


def tabsheet_geometry(tabsheet_node, by_path):
    parent = by_path[tabsheet_node['parent_path']]
    pgeom = parent.get('geometry') or {}
    w = pgeom.get('width') or 400
    h = pgeom.get('height') or 300
    return 0, 0, w, max(h - TABSTRIP_MARGIN_PX, 20)


# ---------------------------------------------------------------------------
# Caption/text extraction
# ---------------------------------------------------------------------------

def node_caption(node):
    props = node['properties']
    for key in ('Text', 'Caption'):
        pv = props.get(key)
        if pv is not None and pv['type'] == 'STR':
            return pv['value']
    return None


# ---------------------------------------------------------------------------
# Dialog-owner tree walk (see module docstring point 1)
# ---------------------------------------------------------------------------

def _enqueue_nested_owners(node, by_path, queue):
    """Find every further SUBDLG dialog owner nested anywhere underneath
    `node` (which is itself already an owner, or on the path down to one)
    and append it to `queue`.

    AI(W906-W7-B1-review) 20260728: fixed a real, silent control-dropping
    bug found by independent review. The old version of this walk only
    ever recursed into a child when *that child's own* kind was SUBDLG (to
    enqueue it) or TABHOST (to look one level further for its TTabSheet/
    SUBDLG pages) -- so a LEAF child that itself had further .dfm children
    was never visited at all, in either function. Most LEAF classes never
    have children (a TEdit doesn't parent anything), but a few real VCL
    classes are LEAF (per classmap.py -- no clean SUBDLG/TABHOST mapping)
    while still being genuine HWND-owning windowed controls that CAN and
    DO parent further controls placed on their surface -- confirmed golden
    case: TChart (windowed; `cObserver.dfm:1919` 'object ChartYield:
    TChart' contains :1959 'object SpeedButton1: TSpeedButton', :1973
    'object edYieldMax: TEdit', :1982 'object edYieldMin: TEdit', all
    three event-wired; `DynamicTemp.dfm:43` Chart1 similarly contains
    edMax/edMin). Those 5 controls were being silently dropped from every
    generated .rc/_ids.h/.rcmeta.json with no gate ever noticing (see
    gates.py's new `gate_rc_census`, added specifically to catch this
    class of bug and verified to FAIL against the old walk before this
    fix, per docs/W7_UI_ARCHITECTURE_PLAN.md S6-B1d's "every gate must be
    proven load-bearing" standard).

    The fix generalizes the walk to descend through ANY child regardless
    of its own kind, stopping only at a SUBDLG child (which becomes its
    own separate dialog owner -- its subtree is that owner's job, resumed
    when it reaches the front of `queue`) -- this naturally subsumes the
    old TABHOST special case too (a TABHOST's own children are always its
    TTabSheet/SUBDLG pages, so recursing into it finds them the same way)."""
    for cp in node['child_paths']:
        child = by_path.get(cp)
        if child is None:
            continue
        if child['kind'] == 'SUBDLG':
            queue.append(cp)
            continue  # its own children are that owner's own turn, not ours
        # AI(W906-W7-B1-followup) 20260728: stop at NONVISUAL too, so this walk
        # is genuinely symmetric with direct_leaf_entries() below (which
        # already refuses to descend into a NONVISUAL child, and whose
        # docstring claims the two walks match). Before this, the owner walk
        # DID recurse through NONVISUAL subtrees: had any NONVISUAL node ever
        # acquired a SUBDLG child, this walk would have emitted a DIALOGEX for
        # a node the control walk never references, and gate_rc_census would
        # NOT have caught it (it counts an owner_path occurrence as satisfying
        # the node). Behaviourally inert on today's corpus -- verified by
        # walking all 133 IR files: zero NONVISUAL nodes have any child at all
        # -- so the generated output is byte-identical, which the G7
        # fidelity/idempotency gates confirm against the canonical trees.
        if child['kind'] == 'NONVISUAL':
            continue
        _enqueue_nested_owners(child, by_path, queue)


def collect_dialog_owner_paths(by_path, root_path):
    owners = []
    queue = collections.deque([root_path])
    seen = set()
    while queue:
        path = queue.popleft()
        if path in seen:
            continue
        seen.add(path)
        owners.append(path)
        node = by_path[path]
        _enqueue_nested_owners(node, by_path, queue)
    return owners


def direct_leaf_entries(owner_node, by_path):
    """(child_node) list, in .dfm declaration order, for every LEAF/TABHOST
    descendant of this dialog owner that is not itself inside a nested
    SUBDLG (a SUBDLG child is a separate dialog owner with its own,
    separately-collected CONTROL list -- rule 1). NONVISUAL children are
    excluded per rule 1 (G1's documented asymmetry), and their subtrees are
    not descended into (no known corpus case of a NONVISUAL node owning a
    visual grandchild; TMenuItem/etc. children of e.g. TPopupMenu are
    themselves NONVISUAL all the way down).

    AI(W906-W7-B1-review) 20260728: generalized to recurse through a LEAF
    child's own children (previously only this owner's *direct* children
    were considered) -- see `_enqueue_nested_owners`'s docstring above for
    the golden bug (cObserver.dfm ChartYield / DynamicTemp.dfm Chart1) this
    fixes. A TABHOST child's own children are always its TTabSheet/SUBDLG
    pages, so recursing into one here is a structural no-op in practice
    (the recursive call immediately hits the SUBDLG `continue` for each of
    them) -- kept anyway for symmetry with the owner-discovery walk rather
    than special-cased away."""
    out = []
    for cp in owner_node['child_paths']:
        child = by_path.get(cp)
        if child is None:
            continue
        k = child['kind']
        if k == 'SUBDLG':
            continue
        if k == 'NONVISUAL':
            continue
        if k in ('LEAF', 'TABHOST'):
            out.append(child)
        out.extend(direct_leaf_entries(child, by_path))
    return out


# ---------------------------------------------------------------------------
# Per-control-entry construction (records both the .rc CONTROL line and the
# sidecar metadata in one place, so they can never drift apart).
# ---------------------------------------------------------------------------

ControlEntry = collections.namedtuple(
    'ControlEntry',
    ['symbol', 'value', 'win32_class', 'styles', 'x', 'y', 'cx', 'cy',
     'caption', 'synthesized', 'rule', 'source_path', 'fallback_class'])


def make_leaf_entry(alloc, child):
    spec, is_fallback = CM.lookup(child['class'])
    name_upper = sanitize_ident(child['name'])
    symbol, value = alloc.new_idc(name_upper)
    geom = child.get('geometry') or {'left': 0, 'top': 0, 'width': 0, 'height': 0}
    caption = node_caption(child)
    entries = [ControlEntry(symbol, value, spec.win32_class, spec.styles,
                             geom.get('left') or 0, geom.get('top') or 0,
                             geom.get('width') or 0, geom.get('height') or 0,
                             caption, False, None, child['path'], is_fallback)]
    if spec.synthesizes_label:
        props = child['properties']
        label_cap_pv = props.get('EditLabel.Caption')
        label_cap = label_cap_pv['value'] if label_cap_pv else None
        lw = scalar(props, 'EditLabel.Width', 60)
        lh = scalar(props, 'EditLabel.Height', geom.get('height') or 14)
        spacing = scalar(props, 'LabelSpacing', 3)
        pos = scalar(props, 'LabelPosition', 'lpLeft')
        ex, ey, ew, eh = (geom.get('left') or 0, geom.get('top') or 0,
                          geom.get('width') or 0, geom.get('height') or 0)
        if pos == 'lpAbove':
            lx, ly = ex, ey - lh - spacing
        elif pos == 'lpBelow':
            lx, ly = ex, ey + eh + spacing
        elif pos == 'lpRight':
            lx, ly = ex + ew + spacing, ey
        else:  # lpLeft, and unknown positions default here (placeholder only)
            lx, ly = ex - lw - spacing, ey
        label_symbol, label_value = alloc.new_idc(name_upper + '_LABEL')
        entries.insert(0, ControlEntry(label_symbol, label_value, 'Static',
                                        ['SS_LEFTNOWORDWRAP'], lx, ly, lw, lh,
                                        label_cap, True, 'SYNTH_LABELEDEDIT_LABEL',
                                        child['path'], False))
    return entries


def make_container_frame_and_synth(alloc, owner_node, by_path):
    """For TGroupBox/TRadioGroup dialog owners: the self-drawn frame, and
    (TRadioGroup only) the synthesized radio buttons. Returns a list of
    ControlEntry destined for the owner's *own* CONTROL list (these are
    logically "children of the container drawn inside its own dialog",
    distinct from direct_leaf_entries which only sees real .dfm children)."""
    out = []
    cls = owner_node['class']
    props = owner_node['properties']
    geom = owner_node.get('geometry') or {'width': 0, 'height': 0}
    w = geom.get('width') or 0
    h = geom.get('height') or 0
    name_upper = sanitize_ident(owner_node['name'])

    if cls in CM.CONTAINER_FRAME_CLASSES:
        caption = node_caption(owner_node)
        symbol, value = alloc.new_idc(name_upper + '_FRAME')
        out.append(ControlEntry(symbol, value, 'Button', ['BS_GROUPBOX'],
                                 0, 0, w, h, caption, True,
                                 'SYNTH_CONTAINER_FRAME', owner_node['path'], False))

    if cls in CM.CONTAINER_RADIO_SYNTH_CLASSES:
        items_pv = props.get('Items.Strings')
        items = items_pv['value'] if items_pv and items_pv['type'] == 'LIST' else []
        cols = scalar(props, 'Columns', 1) or 1
        n = len(items)
        if n:
            rows = int(math.ceil(n / float(cols)))
            item_w = max((w - 16) // cols, 40)
            item_h = 14
            row_h = max((h - 20) // max(rows, 1), 16)
            for i, label in enumerate(items):
                col = i % cols
                row = i // cols
                ix = 8 + col * item_w
                iy = 14 + row * row_h
                styles = ['BS_AUTORADIOBUTTON']
                if i == 0:
                    styles.append('WS_GROUP')
                sym, val = alloc.new_idc(name_upper + '_ITEM%d' % i)
                cap = label if isinstance(label, str) else str(label)
                out.append(ControlEntry(sym, val, 'Button', styles, ix, iy,
                                         item_w, item_h, cap, True,
                                         'SYNTH_RADIOGROUP_ITEM', owner_node['path'], False))
    return out


# ---------------------------------------------------------------------------
# Full-form emission
# ---------------------------------------------------------------------------

BASE_TABSTOP_CATEGORIES = frozenset([CM.EDIT, CM.BUTTON, CM.COMBO, CM.LISTBOX,
                                      CM.SCROLLBAR, CM.COMMCTRL, CM.CUSTOM])


def category_of(win32_class, spec_lookup_class):
    spec, _ = CM.lookup(spec_lookup_class)
    return spec.category


class FormEmission(object):
    def __init__(self, formstem_, ir):
        self.formstem = formstem_
        self.ir = ir
        self.warnings = []
        self.dialogs = []           # list of dict: {idd_symbol, idd_value, x,y,cx,cy, font, is_root, controls:[...]}
        self.fallback_classes_used = set()


def emit_form_ir(ir):
    """Build a FormEmission (in-memory) from a parsed IR dict. Pure
    function of `ir` -- no filesystem, no randomness, no dict-ordering
    dependence beyond Python's stable insertion order (idempotence, D9)."""
    nodes = ir['nodes']
    by_path = {n['path']: n for n in nodes}
    root_path = nodes[0]['path']
    root_node = by_path[root_path]
    fstem = formstem(ir['source_dfm'])
    alloc = IdAllocator(fstem)
    fe = FormEmission(fstem, ir)

    owner_paths = collect_dialog_owner_paths(by_path, root_path)

    for owner_path in owner_paths:
        owner = by_path[owner_path]
        is_root = (owner_path == root_path)
        idd_symbol, idd_value = alloc.new_idd(root_path, owner_path)

        controls = []
        # (a) self-drawn frame + (b) synthesized radio items, for
        # TGroupBox/TRadioGroup dialog owners.
        controls.extend(make_container_frame_and_synth(alloc, owner, by_path))
        # real (or TLabeledEdit-synthesized-label) .dfm children.
        for child in direct_leaf_entries(owner, by_path):
            entries = make_leaf_entry(alloc, child)
            for e in entries:
                if e.fallback_class:
                    fe.fallback_classes_used.add(child['class'])
                    fe.warnings.append(
                        'class %r (node %s) has no curated classmap entry; '
                        'auto-fallback to custom registered class %r used'
                        % (child['class'], child['path'], e.win32_class))
            controls.extend(entries)

        if is_root:
            w, h = root_size(owner)
            x, y = 0, 0
            style_tokens = ['DS_SETFONT', 'WS_POPUP', 'WS_CAPTION', 'WS_SYSMENU', 'WS_CLIPCHILDREN']
            exstyle_tokens = ['WS_EX_CONTROLPARENT']
            caption = node_caption(owner) or fstem
        elif owner['class'] == 'TTabSheet':
            x, y, w, h = tabsheet_geometry(owner, by_path)
            style_tokens = ['DS_SETFONT', 'DS_CONTROL', 'WS_CHILD', 'WS_VISIBLE', 'WS_CLIPCHILDREN']
            exstyle_tokens = ['WS_EX_CONTROLPARENT']
            caption = None
        else:
            geom = owner.get('geometry') or {'left': 0, 'top': 0, 'width': 0, 'height': 0}
            x, y = 0, 0  # a child dialog's own template is in its own local
                          # coordinate space; its position within its
                          # *parent* is a B1c layout-table concern, not
                          # something the .rc DIALOGEX statement carries.
            w, h = geom.get('width') or 0, geom.get('height') or 0
            style_tokens = ['DS_SETFONT', 'DS_CONTROL', 'WS_CHILD', 'WS_VISIBLE', 'WS_CLIPCHILDREN']
            exstyle_tokens = ['WS_EX_CONTROLPARENT']
            caption = None

        pointsize, facename = node_font(owner)

        fe.dialogs.append({
            'idd_symbol': idd_symbol, 'idd_value': idd_value,
            'is_root': is_root, 'owner_path': owner_path, 'owner_class': owner['class'],
            'x': x, 'y': y, 'cx': w, 'cy': h,
            'style_tokens': style_tokens, 'exstyle_tokens': exstyle_tokens,
            'caption': caption, 'font_pointsize': pointsize, 'font_facename': facename,
            'controls': controls,
        })

    return fe


# ---------------------------------------------------------------------------
# Text emission
# ---------------------------------------------------------------------------

BANNER_TEMPLATE = u"""// ============================================================================
// DO NOT EDIT -- machine-generated by {gen} on {src}
// AI(W906-W7-B1b) 20260728: generated .rc/_ids.h from the .dfm IR (see
// docs/W7_UI_ARCHITECTURE_PLAN.md S6 W7-B1b). Hand edits will be
// overwritten and are rejected by the idempotence gate (G7).
// Source golden: {src}
// ============================================================================
"""


def render_rc(fe, ids_header_name):
    lines = []
    lines.append(BANNER_TEMPLATE.format(gen=GENERATOR_VERSION, src=fe.ir['source_dfm']))
    lines.append(u'#include <windows.h>')
    lines.append(u'#ifndef DS_CONTROL')
    lines.append(u'#define DS_CONTROL 0x0400L  // afxres.h is out of reach at this wave (see plan D8/G10 note);')
    lines.append(u'                            // MinGW windres pre-defines this via winuser.h, MSVC rc.exe does not')
    lines.append(u'#endif')
    lines.append(u'#ifndef SS_REALSIZECONTROL')
    lines.append(u'#define SS_REALSIZECONTROL 0x00000040L  // MinGW winuser.h lacks this (verified: absent from')
    lines.append(u'                                        // C:/MinGW/include/winuser.h) while the Windows SDK')
    lines.append(u'                                        // winuser.h rc.exe uses has it -- without this guard')
    lines.append(u'                                        // windres chokes with a bare "syntax error" on every')
    lines.append(u'                                        // TImage/TEBW8Image control (S9-R2: windres is the')
    lines.append(u'                                        // secondary runner, not authoritative, but G5 requires')
    lines.append(u'                                        // both to pass when both are present)')
    lines.append(u'#endif')
    lines.append(u'#include "%s"' % ids_header_name)
    lines.append(u'')
    for d in fe.dialogs:
        lines.append(u'%s DIALOGEX %d, %d, %d, %d' % (d['idd_symbol'], d['x'], d['y'], d['cx'], d['cy']))
        lines.append(u'STYLE ' + u' | '.join(d['style_tokens']))
        lines.append(u'EXSTYLE ' + u' | '.join(d['exstyle_tokens']))
        if d['caption'] is not None:
            lines.append(u'CAPTION %s' % rc_wstr(d['caption']))
        lines.append(u'FONT %d, %s' % (d['font_pointsize'], rc_wstr(d['font_facename'])))
        lines.append(u'BEGIN')
        for c in d['controls']:
            style_str = u' | '.join(c.styles) if c.styles else u'0'
            cls_str = u'"%s"' % c.win32_class
            cap_str = rc_wstr(c.caption)
            lines.append(u'    CONTROL %s, %s, %s, %s, %d, %d, %d, %d' % (
                cap_str, c.symbol, cls_str, style_str, c.x, c.y, c.cx, c.cy))
        lines.append(u'END')
        lines.append(u'')
    return u'\n'.join(lines) + u'\n'


def render_ids_h(fe):
    lines = []
    lines.append(BANNER_TEMPLATE.format(gen=GENERATOR_VERSION, src=fe.ir['source_dfm']))
    guard = 'HT9045_UI_GEN_%s_IDS_H' % fe.formstem
    lines.append(u'#ifndef %s' % guard)
    lines.append(u'#define %s' % guard)
    lines.append(u'')
    lines.append(u'// Dialog template IDs')
    for d in fe.dialogs:
        lines.append(u'#define %s %d' % (d['idd_symbol'], d['idd_value']))
    lines.append(u'')
    lines.append(u'// Control IDs (D11 join key: mechanically derived from the')
    lines.append(u'// .dfm leaf control name -- IDC_<FORMSTEM>_<CONTROLNAME>)')
    for d in fe.dialogs:
        for c in d['controls']:
            tag = u'  // synthesized: %s' % c.rule if c.synthesized else u''
            lines.append(u'#define %s %d%s' % (c.symbol, c.value, tag))
    lines.append(u'')
    lines.append(u'#endif // %s' % guard)
    return u'\n'.join(lines) + u'\n'


def build_meta(fe):
    dialogs_meta = []
    for d in fe.dialogs:
        controls_meta = []
        for c in d['controls']:
            controls_meta.append({
                'symbol': c.symbol, 'value': c.value, 'win32_class': c.win32_class,
                'caption': c.caption, 'synthesized': c.synthesized, 'rule': c.rule,
                'source_path': c.source_path, 'fallback_class': c.fallback_class,
                # AI(W906-W7-B1-review) 20260728: geometry added to the
                # sidecar (MEDIUM-1 fix) -- this module's own docstring
                # point 2 and check_g6.py's docstring both already claimed
                # a compiled (x,y,cx,cy) round-trip through G6, but neither
                # the sidecar nor check_g6.py's comparison loop actually
                # carried/checked geometry until now (the claim was false).
                'x': c.x, 'y': c.y, 'cx': c.cx, 'cy': c.cy,
            })
        dialogs_meta.append({
            'idd_symbol': d['idd_symbol'], 'idd_value': d['idd_value'],
            'is_root': d['is_root'], 'owner_path': d['owner_path'],
            'owner_class': d['owner_class'], 'controls': controls_meta,
        })
    return {
        'schema_version': 1,
        'generator': GENERATOR_VERSION,
        'source_dfm': fe.ir['source_dfm'],
        'formstem': fe.formstem,
        'dialog_count': len(fe.dialogs),
        'control_count': sum(len(d['controls']) for d in fe.dialogs),
        'fallback_classes_used': sorted(fe.fallback_classes_used),
        'warnings': fe.warnings,
        'dialogs': dialogs_meta,
    }


# ---------------------------------------------------------------------------
# CLI: single-file smoke test
# ---------------------------------------------------------------------------

def emit_from_ir_file(ir_path, out_rc_path, out_ids_path, out_meta_path):
    ir = json.load(open(ir_path, 'r', encoding='utf-8'))
    fe = emit_form_ir(ir)
    rc_text = render_rc(fe, os.path.basename(out_ids_path))
    ids_text = render_ids_h(fe)
    meta = build_meta(fe)
    with open(out_rc_path, 'w', encoding='ascii', errors='backslashreplace', newline='\n') as f:
        f.write(rc_text)
    with open(out_ids_path, 'w', encoding='ascii', errors='backslashreplace', newline='\n') as f:
        f.write(ids_text)
    with open(out_meta_path, 'w', encoding='utf-8', newline='\n') as f:
        json.dump(meta, f, ensure_ascii=False, indent=2)
        # AI(W906-Gate4) 20260802: json.dump leaves no trailing newline, which put every generated .json in violation of plan S12 gate 4; emit one so a fresh regen is gate-clean at birth (the checked-in corpora were swept to match in the same commit, keeping gate G7 byte-identical).
        f.write('\n')
    return fe, meta


def main(argv):
    if len(argv) != 4:
        print('usage: emit_rc.py <form.dfm.ir.json> <out.rc> <out_ids.h>', file=sys.stderr)
        return 2
    ir_path, out_rc, out_ids = argv[1], argv[2], argv[3]
    out_meta = out_rc[:-3] + '.rcmeta.json' if out_rc.endswith('.rc') else out_rc + '.rcmeta.json'
    fe, meta = emit_from_ir_file(ir_path, out_rc, out_ids, out_meta)
    print('dialogs=%d controls=%d fallback_classes=%s warnings=%d' % (
        meta['dialog_count'], meta['control_count'], meta['fallback_classes_used'], len(meta['warnings'])))
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))
