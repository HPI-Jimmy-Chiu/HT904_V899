# -*- coding: utf-8 -*-
"""
emit_layout.py -- W7-B1c: per-form *_layout.gen.{h,cpp} + *_events.gen.json
emitter, consuming W7-B1a's IR JSON (tools/dfm2rc/ir_out/**/*.dfm.ir.json).

AI(W906-W7-B1c) 20260728: new file (docs/W7_UI_ARCHITECTURE_PLAN.md S6
W7-B1c, D7 -- this table is the pixel-geometry authority, D12 -- custom-
control props ride here not in the .rc). Runs entirely off B1a's IR; does
NOT read golden .dfm text directly and does NOT depend on W7-B1b's output
(both run in parallel off the same IR, per the plan's dependency graph).

OUTPUT LOCATION -- same deliberate deviation B1a already flagged for its own
IR, applied consistently here: generated files go under this wave's own
authorized tools/dfm2rc/layout_out/<relpath>/ tree (mirroring golden's
relative path underneath that root), not directly under PORTED at golden's
path, because many golden directories that own .dfm files do not exist yet
as directories in PORTED (see run_b1a.py's own docstring for the same
reasoning, cross-checked against §8's file-ownership rules -- this keeps
100% of this wave's output inside the single directory the brief
authorizes). W7-B1d (the integrator) can relocate/re-root these once a
directory-layout decision for the wider PORTED tree is made.

See tools/dfm2rc/layout_out/DfmLayoutTypes.h for the emitted schema and the
design notes behind each field (host_dialog_path, id_symbol non-numeric-ness,
colour has/value/token triples, custom_props scope) -- not re-duplicated
here.
"""
from __future__ import print_function

import os
import re
import json
import collections

import colorconsts
# AI(W906-W7-B1-review) 20260728: sharing PURE, mechanical functions with
# emit_rc.py (sanitize_ident / containerpath_suffix) -- not its generated
# OUTPUT, which this module still never reads (see file banner: B1b/B1c run
# in parallel off the same IR, neither may assume the other's *artifacts*
# exist yet). Reusing the identical identifier-construction rule as source
# code is what actually keeps compute_id_symbol() below from drifting out of
# sync with the #define W7-B1b emits -- see MEDIUM-2 fix note there.
from emit_rc import sanitize_ident, containerpath_suffix

GENERATOR_VERSION = 'emit_layout.py W7-B1c v1.0'

# D12 custom-control component props: the .dfm-declared property set,
# verified empirically against all 133 IR files' custom-control nodes (see
# DfmLayoutTypes.h note 5 for the discrepancy this resolves against the
# plan brief's prose -- InPort/OutPort/ISABase/etc. are runtime-only,
# never .dfm properties, in this corpus).
CUSTOM_PROP_CLASSES = frozenset([
    'TALed', 'TMyLed', 'TMyLedLane',
    'TBtnPanel', 'TBtnPanelLane',
    'TTMyTray', 'TTMyTray256',
])
CUSTOM_PROP_KEYS = (
    'Alias', 'LEDStyle', 'Interval', 'Value',
    'TrueColor', 'FalseColor', 'TrueFontColor', 'FalseFontColor',
    'XItem', 'YItem', 'DirectStyle', 'DirectWidth', 'EdgeWidth', 'LineWidth',
)
# The 4 colour-shaped custom props get dedicated has/value/token fields on
# DfmControlLayout in addition to appearing in the generic bag (see
# DfmLayoutTypes.h's note on this).
CUSTOM_COLOR_KEYS = ('TrueColor', 'FalseColor', 'TrueFontColor', 'FalseFontColor')

HOST_DIALOG_KINDS = frozenset(['ROOT', 'SUBDLG'])

RE_FORMSTEM_OK = re.compile(r'^[A-Za-z_][A-Za-z0-9_]*$')


class EmitError(Exception):
    pass


def form_stem_of(source_dfm):
    stem = os.path.splitext(os.path.basename(source_dfm))[0]
    if not RE_FORMSTEM_OK.match(stem):
        raise EmitError('form stem %r is not a valid C identifier (source_dfm=%r)' % (stem, source_dfm))
    return stem


def _prop_scalar_text(pv):
    """Render a property value (already-decoded IR scalar/string) as plain
    text for the generic custom_props bag. Only STR/SCALAR shapes are
    expected for the CUSTOM_PROP_KEYS set (verified empirically -- see file
    banner); anything else is rendered defensively via repr() rather than
    raising, and is reported by the caller as an anomaly."""
    v = pv['value']
    if isinstance(v, bool):
        return 'True' if v else 'False'
    if isinstance(v, int):
        return str(v)
    if isinstance(v, str):
        return v
    return repr(v)


def _join_set(values):
    if not values:
        return ''
    return ','.join(str(v) for v in values)


def build_path_index(ir):
    return dict((nd['path'], nd) for nd in ir['nodes'])


def compute_host_dialog_path(node, index):
    """D8: nearest ROOT/SUBDLG ancestor (excluding the node itself), walking
    the literal .dfm parent chain. '' for the ROOT node itself."""
    if node['parent_path'] is None:
        return ''
    cur = index.get(node['parent_path'])
    while cur is not None:
        if cur['kind'] in HOST_DIALOG_KINDS:
            return cur['path']
        if cur['parent_path'] is None:
            return cur['path']  # cur is itself ROOT but kind lookup above should have caught it
        cur = index.get(cur['parent_path'])
    return ''


def compute_id_symbol(form_stem, node, root_path):
    """D11 join-key string, made kind-aware.

    AI(W906-W7-B1-review) 20260728: MEDIUM-2 fix. The previous version
    applied the single rule 'IDC_<FORMSTEM>_<NAME>' to every non-root node
    regardless of `kind` -- correct for LEAF/TABHOST (the only kinds that
    actually get an IDC_ control-id #define from W7-B1b, see emit_rc.py's
    direct_leaf_entries/make_leaf_entry), but WRONG for the other two kinds
    that can reach here:
      - SUBDLG nodes are dialog OWNERS in W7-B1b (D8: "not flattened") --
        they get an IDD_ dialog-template symbol from
        emit_rc.IdAllocator.new_idd, built via containerpath_suffix(),
        never an IDC_ one. Mechanically reused here (imported directly, not
        re-derived) so the two can never drift apart.
      - NONVISUAL nodes get NO .rc trace at all (G1's own documented
        asymmetry) -- there is no #define to name, so '' is the only
        truthful answer, matching W7-B1b having emitted nothing.
    This was verified wrong for 3,835 of 22,627 IR nodes across the corpus
    (every SUBDLG/NONVISUAL row previously carried a fabricated-looking
    IDC_ symbol that no #define anywhere actually defines) -- see
    gates.py's new `gate_id_symbol_join`, which asserts this function's
    output against W7-B1b's own .rcmeta.json sidecar rather than leaving
    the claim as an unchecked comment.

    '' for the ROOT node itself (own IDD_<FORMSTEM> lives in W7-B1b's
    dialog-template ID namespace under a different key -- this table does
    not duplicate it, see DfmLayoutTypes.h note 1)."""
    kind = node['kind']
    if kind in ('ROOT', 'NONVISUAL'):
        return ''
    if kind == 'SUBDLG':
        suffix = containerpath_suffix(root_path, node['path'])
        return 'IDD_%s%s' % (form_stem.upper(), suffix)
    return 'IDC_%s_%s' % (form_stem.upper(), sanitize_ident(node['name']))


def build_custom_props(node):
    """Returns (list_of_(key,value_text), color_fields dict) for the 7
    custom-control classes; ([], {}) for every other class."""
    if node['class'] not in CUSTOM_PROP_CLASSES:
        return [], {}
    props = node['properties']
    entries = []
    color_fields = {}
    for key in CUSTOM_PROP_KEYS:
        pv = props.get(key)
        if pv is None:
            continue
        entries.append((key, _prop_scalar_text(pv)))
        if key in CUSTOM_COLOR_KEYS:
            has, resolved, token = colorconsts.resolve_color(pv['value'])
            color_fields[key] = (has, resolved, token)
    return entries, color_fields


def collect_layout_rows(ir):
    """Pure-data pass building one dict per node with everything
    emit_header/emit_source need, independent of C++ string formatting."""
    form_stem = form_stem_of(ir['source_dfm'])
    index = build_path_index(ir)
    root_path = ir['nodes'][0]['path']  # same convention as emit_rc.py's own
                                         # root_path = nodes[0]['path']
    rows = []
    anomalies = []
    for node in ir['nodes']:
        geom = node['geometry'] or {}

        def geo_field(k):
            v = geom.get(k)
            return (v is not None), (v if v is not None else 0)

        has_left, left = geo_field('left')
        has_top, top = geo_field('top')
        has_width, width = geo_field('width')
        has_height, height = geo_field('height')

        props = node['properties']

        tab_pv = props.get('TabOrder')
        has_tab_order = tab_pv is not None
        tab_order = tab_pv['value'] if has_tab_order else 0

        visible_pv = props.get('Visible')
        if visible_pv is None:
            visible = -1
        else:
            visible = 1 if visible_pv['value'] == 'True' else 0

        align_mode = node['align'] if node['align'] else ''
        anchors_mask = _join_set(node['anchors'])

        font_face = ''
        font_height_px = 0
        font_style_bits = ''
        has_font = False
        has_font_color = False
        font_color = 0
        font_color_token = ''
        fn_pv = props.get('Font.Name')
        fh_pv = props.get('Font.Height')
        fs_pv = props.get('Font.Style')
        fc_pv = props.get('Font.Color')
        if fn_pv is not None or fh_pv is not None or fs_pv is not None or fc_pv is not None:
            has_font = True
            if fn_pv is not None:
                font_face = fn_pv['value']
            if fh_pv is not None:
                font_height_px = fh_pv['value']
            if fs_pv is not None:
                font_style_bits = _join_set(fs_pv['value'])
            if fc_pv is not None:
                has_font_color, font_color, font_color_token = colorconsts.resolve_color(fc_pv['value'])
                if not has_font_color and font_color_token:
                    anomalies.append(('UNRESOLVED_COLOR', node['path'], 'Font.Color', font_color_token))

        color_pv = props.get('Color')
        has_color = color_pv is not None
        color = 0
        color_token = ''
        if has_color:
            has_color, color, color_token = colorconsts.resolve_color(color_pv['value'])
            if not has_color and color_token:
                anomalies.append(('UNRESOLVED_COLOR', node['path'], 'Color', color_token))

        custom_entries, custom_color_fields = build_custom_props(node)

        def cf(key):
            if key in custom_color_fields:
                has, resolved, token = custom_color_fields[key]
                if not has and token:
                    anomalies.append(('UNRESOLVED_COLOR', node['path'], key, token))
                return has, resolved, token
            return False, 0, ''

        has_true_color, true_color, true_color_token = cf('TrueColor')
        has_false_color, false_color, false_color_token = cf('FalseColor')
        has_true_font_color, true_font_color, true_font_color_token = cf('TrueFontColor')
        has_false_font_color, false_font_color, false_font_color_token = cf('FalseFontColor')

        rows.append({
            'dfm_path': node['path'],
            'leaf_name': node['name'],
            'id_symbol': compute_id_symbol(form_stem, node, root_path),
            'vcl_class': node['class'],
            'parent_path': node['parent_path'] or '',
            'host_dialog_path': compute_host_dialog_path(node, index),
            'sibling_index': node['sibling_index'],
            'depth': node['depth'],
            'kind': node['kind'],
            'is_graphic_control': node['is_graphic_control'],
            'has_left': has_left, 'left': left,
            'has_top': has_top, 'top': top,
            'has_width': has_width, 'width': width,
            'has_height': has_height, 'height': height,
            'has_tab_order': has_tab_order, 'tab_order': tab_order,
            'visible': visible,
            'align_mode': align_mode,
            'anchors_mask': anchors_mask,
            'has_font': has_font,
            'font_face': font_face,
            'font_height_px': font_height_px,
            'font_style_bits': font_style_bits,
            'has_font_color': has_font_color, 'font_color': font_color, 'font_color_token': font_color_token,
            'has_color': has_color, 'color': color, 'color_token': color_token,
            'has_true_color': has_true_color, 'true_color': true_color, 'true_color_token': true_color_token,
            'has_false_color': has_false_color, 'false_color': false_color, 'false_color_token': false_color_token,
            'has_true_font_color': has_true_font_color, 'true_font_color': true_font_color,
            'true_font_color_token': true_font_color_token,
            'has_false_font_color': has_false_font_color, 'false_font_color': false_font_color,
            'false_font_color_token': false_font_color_token,
            'custom_props': custom_entries,
        })
    return form_stem, rows, anomalies


def collect_events(ir):
    """(control_path, leaf_name, event_name, handler_name) tuples, one per
    IR node.events entry (root included -- forms have OnCreate/OnShow/... of
    their own)."""
    out = []
    for node in ir['nodes']:
        for ev_name, handler in sorted(node['events'].items()):
            out.append({
                'control_path': node['path'],
                'leaf_name': node['name'],
                'vcl_class': node['class'],
                'event_name': ev_name,
                'handler_name': handler,
            })
    return out


# ---------------------------------------------------------------------------
# C++ literal formatting
# ---------------------------------------------------------------------------

def c_str(s):
    """Format a python unicode string as a C++ UTF-8 string literal. Golden
    non-ASCII text (CJK font names etc.) is emitted as real UTF-8 bytes in
    the (UTF-8, no-BOM) source file -- see file banner / plan §5 rule 6 --
    not re-encoded to \\xHH escapes, so it stays genuinely readable/greppable
    and matches how W7-B1a's own IR JSON already carries this same text."""
    out = ['"']
    for ch in s:
        if ch == '\\':
            out.append('\\\\')
        elif ch == '"':
            out.append('\\"')
        elif ch == '\n':
            out.append('\\n')
        elif ch == '\r':
            out.append('\\r')
        elif ch == '\t':
            out.append('\\t')
        elif ord(ch) < 0x20:
            out.append('\\x%02x' % ord(ch))
        else:
            out.append(ch)
    out.append('"')
    return ''.join(out)


def c_bool(b):
    return 'true' if b else 'false'


# ---------------------------------------------------------------------------
# Emission
# ---------------------------------------------------------------------------

HEADER_TEMPLATE = """\
// ===========================================================================
//  %(relpath_h)s
//  DO NOT EDIT -- GENERATED FILE (W7-B1c layout/colour/font/custom-control
//  data table). Regenerate: python tools/dfm2rc/run_b1c.py
//  Generator:   %(generator)s
//  Source .dfm: %(source_dfm)s (golden, cp950-decoded)
//
//  AI(W906-W7-B1c) 20260728: generated layout table for %(form_stem)s -- see
//  docs/W7_UI_ARCHITECTURE_PLAN.md S6 W7-B1c (D7: pixel-geometry authority;
//  D12: custom-control props ride here, not in the .rc) and
//  tools/dfm2rc/layout_out/DfmLayoutTypes.h for the schema + field contracts.
//  A hand-edit here will be silently overwritten by the next regeneration
//  and is exactly what gate G7 (idempotence) is designed to catch.
// ===========================================================================
#ifndef HT9045_GEN_LAYOUT_%(form_stem_upper)s_H
#define HT9045_GEN_LAYOUT_%(form_stem_upper)s_H

#include "%(include_path)s"

namespace ht9045_layout {

extern const DfmControlLayout k%(form_stem)sLayout[];
extern const int k%(form_stem)sLayoutCount;

} // namespace ht9045_layout

#endif // HT9045_GEN_LAYOUT_%(form_stem_upper)s_H
"""


def relative_include_for(depth_from_layout_out_root):
    """The generated header lives under layout_out/<relpath-dirs>/, so the
    include of the shared schema header (which lives directly under
    layout_out/) needs one '../' per directory level of <relpath-dirs>."""
    if depth_from_layout_out_root <= 0:
        return 'DfmLayoutTypes.h'
    return '../' * depth_from_layout_out_root + 'DfmLayoutTypes.h'


def emit_header(form_stem, ir, rel_dir_depth):
    return HEADER_TEMPLATE % {
        'relpath_h': (ir['source_dfm'].rsplit('.', 1)[0] + '_layout.gen.h'),
        'generator': GENERATOR_VERSION,
        'source_dfm': ir['source_dfm'],
        'form_stem': form_stem,
        'form_stem_upper': form_stem.upper(),
        'include_path': relative_include_for(rel_dir_depth),
    }


def _row_initializer(row, custom_props_ptr_expr):
    fields = [
        c_str(row['dfm_path']),
        c_str(row['leaf_name']),
        c_str(row['id_symbol']),
        c_str(row['vcl_class']),
        c_str(row['parent_path']),
        c_str(row['host_dialog_path']),
        str(row['sibling_index']),
        str(row['depth']),
        c_str(row['kind']),
        c_bool(row['is_graphic_control']),
        c_bool(row['has_left']), str(row['left']),
        c_bool(row['has_top']), str(row['top']),
        c_bool(row['has_width']), str(row['width']),
        c_bool(row['has_height']), str(row['height']),
        c_bool(row['has_tab_order']), str(row['tab_order']),
        str(row['visible']),
        c_str(row['align_mode']),
        c_str(row['anchors_mask']),
        c_bool(row['has_font']),
        c_str(row['font_face']),
        str(row['font_height_px']),
        c_str(row['font_style_bits']),
        c_bool(row['has_font_color']), str(row['font_color']), c_str(row['font_color_token']),
        c_bool(row['has_color']), str(row['color']), c_str(row['color_token']),
        c_bool(row['has_true_color']), str(row['true_color']), c_str(row['true_color_token']),
        c_bool(row['has_false_color']), str(row['false_color']), c_str(row['false_color_token']),
        c_bool(row['has_true_font_color']), str(row['true_font_color']), c_str(row['true_font_color_token']),
        c_bool(row['has_false_font_color']), str(row['false_font_color']), c_str(row['false_font_color_token']),
        str(len(row['custom_props'])),
        custom_props_ptr_expr,
    ]
    return '{ ' + ', '.join(fields) + ' }'


SOURCE_HEADER_TEMPLATE = """\
// ===========================================================================
//  %(relpath_cpp)s
//  DO NOT EDIT -- GENERATED FILE. See the matching _layout.gen.h banner.
// ===========================================================================
#include "%(own_header)s"

namespace ht9045_layout {

"""


def emit_source(form_stem, ir, rows, own_header_name):
    out = [SOURCE_HEADER_TEMPLATE % {
        'relpath_cpp': (ir['source_dfm'].rsplit('.', 1)[0] + '_layout.gen.cpp'),
        'own_header': own_header_name,
    }]

    # Flatten all rows' custom_props into one array with per-row offsets so
    # each DfmControlLayout.custom_props can point into a slice of it
    # (avoids one small named array per node).
    flat_props = []
    offsets = []
    for row in rows:
        offsets.append(len(flat_props))
        flat_props.extend(row['custom_props'])

    if flat_props:
        out.append('const DfmCustomProp k%sCustomProps[] = {\n' % form_stem)
        for key, val in flat_props:
            out.append('    { %s, %s },\n' % (c_str(key), c_str(val)))
        out.append('};\n\n')
    else:
        out.append('// (no custom-control properties in this form)\n\n')

    out.append('const DfmControlLayout k%sLayout[] = {\n' % form_stem)
    for row, off in zip(rows, offsets):
        n = len(row['custom_props'])
        ptr_expr = ('&k%sCustomProps[%d]' % (form_stem, off)) if n else 'nullptr'
        out.append('    ' + _row_initializer(row, ptr_expr) + ',\n')
    out.append('};\n\n')
    out.append('const int k%sLayoutCount = %d;\n\n' % (form_stem, len(rows)))
    out.append('} // namespace ht9045_layout\n')
    return ''.join(out)


def emit_events_json(form_stem, ir, events):
    return {
        'schema_version': 1,
        'generator': GENERATOR_VERSION,
        'source_dfm': ir['source_dfm'],
        'form_name': ir['form_name'],
        'event_count': len(events),
        'events': events,
    }


def emit_form(ir_path, ir, out_root):
    """Emit the 3 artefacts for one form's IR. Returns (form_stem, rows,
    anomalies, out_paths dict) for the caller's gate bookkeeping."""
    form_stem, rows, anomalies = collect_layout_rows(ir)
    events = collect_events(ir)

    rel_no_ext = ir['source_dfm'].rsplit('.', 1)[0]  # drop .dfm
    rel_dir = os.path.dirname(rel_no_ext)
    rel_dir_depth = 0 if not rel_dir else len(rel_dir.replace('\\', '/').split('/'))

    out_dir = os.path.join(out_root, *rel_dir.split('/')) if rel_dir else out_root
    if not os.path.isdir(out_dir):
        os.makedirs(out_dir)

    h_name = form_stem + '_layout.gen.h'
    cpp_name = form_stem + '_layout.gen.cpp'
    json_name = form_stem + '_events.gen.json'

    h_path = os.path.join(out_dir, h_name)
    cpp_path = os.path.join(out_dir, cpp_name)
    json_path = os.path.join(out_dir, json_name)

    with open(h_path, 'w', encoding='utf-8', newline='\n') as fh:
        fh.write(emit_header(form_stem, ir, rel_dir_depth))
    with open(cpp_path, 'w', encoding='utf-8', newline='\n') as fh:
        fh.write(emit_source(form_stem, ir, rows, h_name))
    with open(json_path, 'w', encoding='utf-8', newline='\n') as fh:
        json.dump(emit_events_json(form_stem, ir, events), fh, ensure_ascii=False, indent=1, sort_keys=True)

    return form_stem, rows, events, anomalies, {'h': h_path, 'cpp': cpp_path, 'json': json_path}
