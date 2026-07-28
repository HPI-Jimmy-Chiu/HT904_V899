# -*- coding: utf-8 -*-
"""
dfm_parse.py -- W7-B1a: .dfm text-format parser -> lossless intermediate
representation (IR).

AI(W906-W7-B1a) 20260728: new file, first stage of the .dfm-to-MFC pipeline
(plan doc: HT9011UC_Cpp_V3.33.906.0/docs/W7_UI_ARCHITECTURE_PLAN.md, S4-V4,
S6 W7-B1a). Parses all golden .dfm forms into a per-form JSON IR that
downstream waves (W7-B1b .rc/_ids.h generator, W7-B1c layout/event/colour
table generator) consume instead of re-parsing .dfm text themselves.

DESIGN NOTES (read before modifying):

1. Grammar coverage. This corpus's .dfm dialect (BCB6, all 133 files TEXT
   format, verified zero TPF0 binary forms) uses:
     - `object|inline|inherited [Name:] Class [ [index] ]` headers, `end`
       terminators, arbitrary nesting (measured max depth 14).
     - Dotted sub-properties: `Font.Charset = ...`.
     - Six property value shapes, dispatched on the first non-blank
       character of the value (which may start on the *next* line after
       `Key =` when the key line's remainder is empty -- this single
       quirk is what breaks most naive parsers; see cConfiguration.dfm:726
       and 134 other sites in this corpus):
         '{' -> BLOB   (hex byte stream, e.g. Glyph.Data / Icon.Data)
         '(' -> LIST   (parenthesised sequence: ColWidths, Items.Strings)
         '<' -> COLLECTION (`item ... end` blocks, e.g. StatusBar Panels,
                            TTable FieldDefs -- only 7 instances in this
                            corpus, all flat property bags, no nested
                            objects observed)
         '[' -> SET    (`[fsBold, fsItalic]`, possibly empty `[]`)
         "'" or '#' -> STR (quoted text concatenated with #NNN Unicode
                            code-point escapes and '+' line-continuation;
                            #NNN is a *Unicode code point*, not a raw
                            byte -- verified against ARMS.dfm:427
                            `'@'#26032#32048#26126#39636` = "@新細
                            明體" (a CJK font name) -- so char
                            codes are applied *after* cp950 decoding of
                            the surrounding text, never before)
         anything else -> SCALAR (numbers, enum identifiers, event
                            handler names -- kept as the raw token)

2. String/list continuation via trailing '+'. A property value may be
   split across physical lines with a lone trailing '+' signalling
   Pascal-expression string concatenation onto the next line (see
   cBinSel.dfm:413-414). The naive approach of blindly gluing raw text
   together (stripping only the '+') produces a spurious doubled single
   quote at the join point (a closing quote immediately followed by the
   next fragment's opening quote) that a naive "'' is an escaped quote"
   scanner would misinterpret as escaping into the following fragment.
   This implementation avoids the whole class of bug by decoding each
   physical line's quote/#NNN tokens to real characters *before*
   concatenating across lines -- so join points are plain Python string
   concatenation of already-decoded text, never raw '+quote' splicing.
   (The corpus's one true doubled-quote escape convention, `''` for a
   literal apostrophe *within* a single physical line, is still handled
   in decode_string_expr -- it just never has to reason about line
   joins.)

3. Blobs are carried losslessly as the verbatim uppercase hex text plus
   a byte length and sha1 (so downstream extraction/dedup -- 554 blobs,
   3.70 MB, many duplicate Glyph.Data across forms -- can content-hash
   without re-reading golden). We do not decode bitmap/icon pixel data
   here; that is out of scope for B1a.

4. Encoding: golden is Big5. We decode whole-file bytes with cp950
   (superset of Big5, handles a few extra symbols) and fall back to
   big5, then latin-1 as a last resort so the parser never raises on
   encoding alone (an encoding fallback is recorded in the IR so callers
   can see it happened).

5. This module deliberately does NOT reuse any of gates.py's independent
   census/geometry scanner -- see gates.py's own docstring for why.
"""
from __future__ import print_function

import os
import re
import sys
import json
import hashlib
import collections

# ---------------------------------------------------------------------------
# Encoding
# ---------------------------------------------------------------------------

def read_text(path):
    """Return (text, encoding_used) for a golden .dfm file (Big5 source)."""
    raw = open(path, 'rb').read()
    for enc in ('cp950', 'big5'):
        try:
            return raw.decode(enc), enc
        except Exception:
            pass
    return raw.decode('latin-1'), 'latin-1(fallback)'


# ---------------------------------------------------------------------------
# Low-level helpers shared by top-level property parsing and by collection
# item parsing.
# ---------------------------------------------------------------------------

RE_OBJ = re.compile(
    r'^(object|inline|inherited)\s+(?:([A-Za-z_]\w*)\s*:\s*)?([A-Za-z_]\w*)\s*(?:\[(\d+)\])?\s*$')
RE_PROP = re.compile(r'^([A-Za-z_][\w.]*)\s*=\s*(.*)$')
RE_INT = re.compile(r'^-?\d+$')


class DfmParseError(Exception):
    pass


def strip_strings(s):
    """Remove quoted 'literals' so bracket/paren counting is safe against
    strings that themselves contain brackets/parens."""
    out = []
    i = 0
    n = len(s)
    while i < n:
        if s[i] == "'":
            i += 1
            while i < n:
                if s[i] == "'":
                    if i + 1 < n and s[i + 1] == "'":
                        i += 2
                        continue
                    i += 1
                    break
                i += 1
        else:
            out.append(s[i])
            i += 1
    return ''.join(out)


def decode_string_expr(text):
    """Decode a flat (already line-joined) sequence of 'literal' and #NNN
    tokens into the real Unicode string they represent. #NNN is a Unicode
    code point (see module docstring point 1), '' inside a 'literal' is an
    escaped literal apostrophe."""
    out = []
    i = 0
    n = len(text)
    while i < n:
        while i < n and text[i] in ' \t\r\n':
            i += 1
        if i >= n:
            break
        c = text[i]
        if c == "'":
            i += 1
            buf = []
            while i < n:
                if text[i] == "'":
                    if i + 1 < n and text[i + 1] == "'":
                        buf.append("'")
                        i += 2
                        continue
                    i += 1
                    break
                else:
                    buf.append(text[i])
                    i += 1
            out.append(''.join(buf))
        elif c == '#':
            i += 1
            start = i
            while i < n and text[i].isdigit():
                i += 1
            if i == start:
                raise DfmParseError('bad #NNN char code at offset %d in %r' % (i, text[:80]))
            out.append(_unichr(int(text[start:i])))
        else:
            raise DfmParseError('unexpected char %r at offset %d in string expr %r' % (c, i, text[:80]))
    return ''.join(out)


try:
    _unichr = unichr  # type: ignore  # python2
except NameError:
    _unichr = chr  # python3


def _coerce_scalar(tok):
    if RE_INT.match(tok):
        return int(tok)
    return tok


# ---------------------------------------------------------------------------
# Property value readers. Each takes (lines, i, first_val) where `i` is the
# index (0-based) of the line already known to contain `first_val` (the
# trimmed text after '=' -- or the trimmed next line, if the '=' line's
# remainder was empty), and returns (kind, value, next_i) where next_i is
# the index of the first not-yet-consumed line.
# ---------------------------------------------------------------------------

def read_string_value(lines, i, first_val):
    parts = [first_val]
    while parts[-1].rstrip().endswith('+') and i + 1 < len(lines):
        i += 1
        parts[-1] = parts[-1].rstrip()[:-1].rstrip()
        parts.append(lines[i].strip())
    i += 1
    flat = '\n'.join(parts)
    return 'STR', decode_string_expr(flat), i


def read_blob_value(lines, i, first_val):
    parts = [first_val.lstrip('{')]
    while not parts[-1].rstrip().endswith('}') and i + 1 < len(lines):
        i += 1
        parts.append(lines[i].strip())
    hexraw = ''.join(p.rstrip('}') for p in parts)
    hexraw = re.sub(r'\s+', '', hexraw)
    i += 1
    byte_len = len(hexraw) // 2
    sha1 = hashlib.sha1(hexraw.encode('ascii')).hexdigest() if hexraw else hashlib.sha1(b'').hexdigest()
    return 'BLOB', {'hex': hexraw, 'byte_len': byte_len, 'sha1': sha1}, i


def read_set_value(lines, i, first_val):
    buf = first_val
    while strip_strings(buf).count('[') > strip_strings(buf).count(']') and i + 1 < len(lines):
        i += 1
        buf += ' ' + lines[i].strip()
    i += 1
    inner = buf.strip()
    if not (inner.startswith('[') and inner.endswith(']')):
        raise DfmParseError('malformed set literal: %r' % (inner,))
    inner = inner[1:-1].strip()
    flags = [] if not inner else [x.strip() for x in inner.split(',') if x.strip()]
    return 'SET', flags, i


def _parse_entry_sequence(text):
    """Parse the inside of a '(...)' list body into a python list of
    entries (int / str / raw-token-string)."""
    entries = []
    pos = 0
    n = len(text)
    while True:
        while pos < n and text[pos] in ' \t\r\n':
            pos += 1
        if pos >= n:
            break
        c = text[pos]
        if c == "'" or c == '#':
            # Decode each quote/#NNN token to real characters *incrementally*
            # and join the decoded fragments -- do NOT slice the raw
            # [start:pos) span and hand it to decode_string_expr, because a
            # '+' line-continuation between two tokens (e.g.
            # CCLink/MyCCLinkSensor.dfm:4186-4187, a Lines.Strings list
            # entry that itself continues across lines) would then appear
            # verbatim inside the span and decode_string_expr rejects any
            # character that isn't a quote/#/whitespace. Mirrors
            # read_string_value's line-level approach, just token-level.
            decoded_parts = []
            while True:
                if text[pos] == "'":
                    pos += 1
                    buf = []
                    while pos < n:
                        if text[pos] == "'":
                            if pos + 1 < n and text[pos + 1] == "'":
                                buf.append("'")
                                pos += 2
                                continue
                            pos += 1
                            break
                        else:
                            buf.append(text[pos])
                            pos += 1
                    decoded_parts.append(''.join(buf))
                elif text[pos] == '#':
                    pos += 1
                    cstart = pos
                    while pos < n and text[pos].isdigit():
                        pos += 1
                    if pos == cstart:
                        raise DfmParseError('bad #NNN char code at offset %d in list entry' % pos)
                    decoded_parts.append(_unichr(int(text[cstart:pos])))
                else:
                    break
                # look for a continuation '+' (string entry spanning more
                # than one token group, e.g. across a list-line wrap)
                p2 = pos
                while p2 < n and text[p2] in ' \t':
                    p2 += 1
                if p2 < n and text[p2] == '+':
                    p2 += 1
                    while p2 < n and text[p2] in ' \t\r\n':
                        p2 += 1
                    pos = p2
                    if pos < n and (text[pos] == "'" or text[pos] == '#'):
                        continue
                    else:
                        break
                else:
                    break
            entries.append(''.join(decoded_parts))
        else:
            start = pos
            while pos < n and text[pos] not in ' \t\r\n':
                pos += 1
            entries.append(_coerce_scalar(text[start:pos]))
    return entries


def read_list_value(lines, i, first_val):
    raw_lines = [first_val]
    bal = strip_strings(first_val).count('(') - strip_strings(first_val).count(')')
    while bal > 0 and i + 1 < len(lines):
        i += 1
        t = lines[i].strip()
        raw_lines.append(t)
        bal += strip_strings(t).count('(') - strip_strings(t).count(')')
    i += 1
    blob = '\n'.join(raw_lines)
    first_paren = blob.index('(')
    last_paren = blob.rindex(')')
    inner = blob[first_paren + 1:last_paren]
    entries = _parse_entry_sequence(inner)
    return 'LIST', entries, i


def read_collection_value(lines, i, first_val):
    stripped = first_val.strip()
    if stripped == '<>':
        return 'COLLECTION', [], i + 1
    # `first_val` is expected to be exactly '<' (verified: all 7 collection
    # instances in this corpus open with `Key = <` alone on the line).
    if stripped != '<':
        raise DfmParseError('unsupported inline collection-open form: %r' % (first_val,))
    i += 1
    items = []
    cur = None
    n = len(lines)
    while i < n:
        s = lines[i].strip()
        if s == 'item':
            cur = {'order': [], 'properties': {}}
            i += 1
            continue
        if s == 'end' or s == 'end>':
            if cur is not None:
                items.append(cur)
                cur = None
            i += 1
            if s == 'end>':
                break
            continue
        if s == '>':
            i += 1
            break
        m = RE_PROP.match(s)
        if m and cur is not None:
            key, val = m.group(1), m.group(2).strip()
            if val == '':
                i += 1
                if i >= n:
                    break
                val = lines[i].strip()
            kind, value, i = dispatch_value(lines, i, val)
            if key not in cur['properties']:
                cur['order'].append(key)
            cur['properties'][key] = {'type': kind, 'value': value}
            continue
        # unexpected line inside a collection item; skip defensively
        i += 1
    return 'COLLECTION', items, i


def dispatch_value(lines, i, val):
    if val.startswith('{'):
        return read_blob_value(lines, i, val)
    if val.startswith('('):
        return read_list_value(lines, i, val)
    if val.startswith('<'):
        return read_collection_value(lines, i, val)
    if val.startswith('['):
        return read_set_value(lines, i, val)
    if val[:1] in ("'", '#'):
        return read_string_value(lines, i, val)
    return 'SCALAR', _coerce_scalar(val), i + 1


# ---------------------------------------------------------------------------
# Node tree
# ---------------------------------------------------------------------------

class Node(object):
    __slots__ = ('objkind', 'name', 'cls', 'index', 'depth', 'line', 'parent',
                 'path', 'sibling_index', 'order', 'properties', 'children')

    def __init__(self, objkind, name, cls, index, depth, line, parent, path, sibling_index):
        self.objkind = objkind          # 'object' | 'inline' | 'inherited'
        self.name = name
        self.cls = cls
        self.index = index              # int or None (the optional [N])
        self.depth = depth
        self.line = line                # 1-based source line of the header
        self.parent = parent            # Node or None
        self.path = path                # dotted path from root
        self.sibling_index = sibling_index
        self.order = []                 # property declaration order
        self.properties = {}            # key -> {'type':..., 'value':...}
        self.children = []

    def set_prop(self, key, kind, value):
        if key not in self.properties:
            self.order.append(key)
        self.properties[key] = {'type': kind, 'value': value}


class DfmForm(object):
    """Parse result for one .dfm file."""

    def __init__(self, source_path, root, errors, encoding_used):
        self.source_path = source_path
        self.root = root
        self.errors = errors
        self.encoding_used = encoding_used

    def flatten(self):
        """Pre-order (== source declaration order) flat list of all nodes,
        including the root."""
        out = []

        def walk(nd):
            out.append(nd)
            for k in nd.children:
                walk(k)
        if self.root is not None:
            walk(self.root)
        return out


def parse_dfm_text(lines, source_label='<mem>'):
    """Core recursive-descent parse of an already-decoded, line-split .dfm
    body. Returns (root Node or None, errors list)."""
    n = len(lines)
    i = 0
    root = None
    stack = []          # list of Node, innermost last
    sib_counters = {}    # id(parent Node) or None(for root) -> next sibling idx
    errors = []

    def cur():
        return stack[-1] if stack else None

    while i < n:
        raw = lines[i]
        s = raw.strip()
        if not s:
            i += 1
            continue
        m = RE_OBJ.match(s)
        if m:
            objkind, name, cls, idxs = m.groups()
            idx = int(idxs) if idxs is not None else None
            parent = cur()
            depth = len(stack)
            if name is None:
                name = '_anon_L%d' % (i + 1)
            key = id(parent) if parent is not None else None
            sib = sib_counters.get(key, 0)
            sib_counters[key] = sib + 1
            path = (parent.path + '.' + name) if parent is not None else name
            nd = Node(objkind, name, cls, idx, depth, i + 1, parent, path, sib)
            if parent is None:
                if root is None:
                    root = nd
                else:
                    errors.append(('MULTIROOT', i + 1))
            else:
                parent.children.append(nd)
            stack.append(nd)
            i += 1
            continue
        if s == 'end':
            if stack:
                stack.pop()
            else:
                errors.append(('UNBAL_END', i + 1))
            i += 1
            continue
        m = RE_PROP.match(s)
        if m:
            key, val = m.group(1), m.group(2).strip()
            nd = cur()
            if nd is None:
                errors.append(('PROP_NO_OBJ', i + 1))
                i += 1
                continue
            if val == '':
                i += 1
                if i >= n:
                    break
                val = lines[i].strip()
            try:
                kind, value, i = dispatch_value(lines, i, val)
            except DfmParseError as e:
                errors.append(('VALUE_ERROR@%d' % (i + 1), str(e)))
                i += 1
                continue
            nd.set_prop(key, kind, value)
            continue
        errors.append(('UNPARSED@%d' % (i + 1), s[:120]))
        i += 1
    if stack:
        errors.append(('UNCLOSED', len(stack)))
    return root, errors


def parse_dfm_file(path):
    text, enc = read_text(path)
    lines = [l.rstrip('\r') for l in text.split('\n')]
    root, errors = parse_dfm_text(lines, source_label=path)
    return DfmForm(path, root, errors, enc)


# ---------------------------------------------------------------------------
# Classification (D8/D12/plan-S6-B1a "kind"/"is_graphic_control" fields)
# ---------------------------------------------------------------------------

# Containers that become their own child DIALOGEX per D8 (TPanel only
# counts if it actually has children -- a leaf TPanel is just a visual
# rectangle, classified LEAF).
SUBDLG_CLASSES = frozenset([
    'TGroupBox', 'TScrollBox', 'TTabSheet', 'TRadioGroup', 'TframeProdInfo',
])
SUBDLG_IF_HAS_KIDS = frozenset(['TPanel'])
TABHOST_CLASSES = frozenset(['TPageControl'])

# Explicit NONVISUAL class list from plan S6-B1b (used as a documented,
# named set; cross-checked against the "no Width/Height property present"
# heuristic by gates.py -- see gates.py G1).
NONVISUAL_CLASSES = frozenset([
    'TTimer', 'TClientSocket', 'TComm', 'TServerSocket', 'TOpenDialog',
    'TSaveDialog', 'TSavePictureDialog', 'TPopupMenu', 'TMenuItem',
    'TActionList', 'TAction', 'TTable', 'TQuery', 'TDataSource',
    'TXMLDocument', 'TIdHTTP', 'THTTPRIO', 'TNMUDP', 'TNMFTP',
    'TLineSeries', 'TFastLineSeries', 'TBarSeries',
])

# TGraphicControl descendants: no HWND in stock VCL. Plan S9-R3 cites
# "6,760 個實例...TLabel/TSpeedButton/TShape/TImage/TBevel + 自製 LED".
# Measured independently (see report): TLabel+TBevel+TSpeedButton+TShape+
# TImage alone already sum to 6,760 in this corpus (see final report --
# this is a documented, flagged discrepancy against the plan's prose,
# not silently resolved: adding the LED family, which genuinely also
# derives from TGraphicControl per aled.pas / S4-V3, pushes the total
# to 8,600+, not 6,760). We classify is_graphic_control per the
# technically-correct VCL class hierarchy (includes LED family); callers
# that need the plan's literal 6,760 figure should filter to
# GRAPHIC_CONTROL_CLASSES_PLAN_6760 instead.
GRAPHIC_CONTROL_CLASSES = frozenset([
    'TLabel', 'TBevel', 'TSpeedButton', 'TShape', 'TImage',
    'TALed', 'TMyLed', 'TMyLedLane',
])
GRAPHIC_CONTROL_CLASSES_PLAN_6760 = frozenset([
    'TLabel', 'TBevel', 'TSpeedButton', 'TShape', 'TImage',
])


def classify_kind(node):
    if node.parent is None:
        return 'ROOT'
    has_geom = ('Width' in node.properties) or ('Height' in node.properties)
    if node.cls in TABHOST_CLASSES:
        return 'TABHOST'
    if node.cls in SUBDLG_CLASSES:
        return 'SUBDLG'
    if node.cls in SUBDLG_IF_HAS_KIDS and node.children:
        return 'SUBDLG'
    if node.cls in NONVISUAL_CLASSES or not has_geom:
        return 'NONVISUAL'
    return 'LEAF'


def is_graphic_control(node):
    return node.cls in GRAPHIC_CONTROL_CLASSES


# ---------------------------------------------------------------------------
# IR (JSON) emission
# ---------------------------------------------------------------------------

GENERATOR_VERSION = 'dfm_parse.py W7-B1a v1.0'


def _prop_to_json(pv):
    kind = pv['type']
    value = pv['value']
    if kind == 'COLLECTION':
        return {'type': kind, 'value': [
            {'order': it['order'],
             'properties': dict((k, _prop_to_json(v)) for k, v in it['properties'].items())}
            for it in value
        ]}
    return {'type': kind, 'value': value}


def node_to_json(node):
    geom = None
    left = node.properties.get('Left')
    top = node.properties.get('Top')
    width = node.properties.get('Width')
    height = node.properties.get('Height')
    if left or top or width or height:
        def val(pv):
            return pv['value'] if pv is not None else None
        geom = {
            'left': val(left), 'top': val(top),
            'width': val(width), 'height': val(height),
        }
    taborder_pv = node.properties.get('TabOrder')
    taborder = taborder_pv['value'] if taborder_pv is not None else None
    visible_pv = node.properties.get('Visible')
    visible = visible_pv['value'] if visible_pv is not None else None
    align_pv = node.properties.get('Align')
    align = align_pv['value'] if align_pv is not None else None
    anchors_pv = node.properties.get('Anchors')
    anchors = anchors_pv['value'] if anchors_pv is not None else None

    events = {}
    properties = {}
    for key in node.order:
        pv = node.properties[key]
        properties[key] = _prop_to_json(pv)
        if key.startswith('On') and pv['type'] == 'SCALAR' and isinstance(pv['value'], str):
            events[key] = pv['value']

    return {
        'path': node.path,
        'name': node.name,
        'class': node.cls,
        'objkind': node.objkind,
        'index': node.index,
        'parent_path': node.parent.path if node.parent is not None else None,
        'sibling_index': node.sibling_index,
        'depth': node.depth,
        'line': node.line,
        'kind': classify_kind(node),
        'is_graphic_control': is_graphic_control(node),
        'synthesized': False,
        'geometry': geom,
        'tab_order': taborder,
        'visible': visible,
        'align': align,
        'anchors': anchors,
        'events': events,
        'properties': properties,
        'child_paths': [c.path for c in node.children],
    }


def form_to_ir(form, golden_relpath):
    nodes = form.flatten()
    root = form.root
    ir = {
        'schema_version': 1,
        'generator': GENERATOR_VERSION,
        'source_dfm': golden_relpath.replace('\\', '/'),
        'encoding_used': form.encoding_used,
        'parse_errors': [{'code': c, 'detail': d} for (c, d) in form.errors],
        'form_name': root.name if root else None,
        'form_class': root.cls if root else None,
        'total_nodes_incl_root': len(nodes),
        'control_node_count': max(len(nodes) - 1, 0),
        'nodes': [node_to_json(nd) for nd in nodes],
    }
    return ir


# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------

def _find_golden_root():
    here = os.path.dirname(os.path.abspath(__file__))
    # tools/dfm2rc -> ... -> HT9011UC_Cpp_V3.33.906.0 -> HT9045 -> golden dir
    ht9045 = os.path.abspath(os.path.join(here, '..', '..', '..'))
    cand = os.path.join(ht9045, 'HT9011UC_Code_V3.33.906.0_20260618')
    return cand


def iter_golden_dfm_files(golden_root):
    files = []
    for dp, dn, fn in os.walk(golden_root):
        if '.svn' in dp.split(os.sep):
            continue
        for f in fn:
            if f.lower().endswith('.dfm'):
                files.append(os.path.join(dp, f))
    files.sort()
    return files


def main(argv):
    golden_root = _find_golden_root()
    if len(argv) > 1:
        target = argv[1]
        path = target if os.path.isabs(target) else os.path.join(golden_root, target)
        form = parse_dfm_file(path)
        relpath = os.path.relpath(path, golden_root)
        ir = form_to_ir(form, relpath)
        print(json.dumps(ir, ensure_ascii=False, indent=2))
        if form.errors:
            print('ERRORS:', form.errors, file=sys.stderr)
            return 1
        return 0
    # No args: quick corpus-wide sanity summary (full IR emission is
    # run_b1a.py's job).
    files = iter_golden_dfm_files(golden_root)
    total_nodes = 0
    total_errs = 0
    for p in files:
        form = parse_dfm_file(p)
        n = form.flatten()
        total_nodes += max(len(n) - 1, 0)
        if form.errors:
            total_errs += 1
            print('ERR', os.path.relpath(p, golden_root), form.errors[:4])
    print('files=%d control_nodes=%d files_with_errors=%d' % (len(files), total_nodes, total_errs))
    return 0 if total_errs == 0 else 1


if __name__ == '__main__':
    sys.exit(main(sys.argv))
