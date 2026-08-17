# -*- coding: utf-8 -*-
"""
emit_uimap.py -- GA-4: per-form *_uimap.gen.{h,cpp} join-table emitter
(docs/DESIGN_GA4_UI_ENGINES.md D-GA4-1; schema = ui/layout/UiFormMap.h).

AI(W906-GA4) 20260804: new file. Reads the two pipeline-sanctioned join
sources -- rc_out/<form>.rcmeta.json (W7-B1b's sidecar: dialog/control
symbol<->numeric-id truth) and ir_out/<form>.dfm.ir.json (W7-B1a's IR: the
properties the layout table truthfully lacks) -- and bakes the string->number
join into compilable data tables, because a runtime engine cannot resolve an
id_symbol STRING to a #define (UiFormMap.h banner; DfmLayoutTypes.h note 1).

===========================================================================
DESIGN DECISIONS THIS FILE MAKES (read before modifying)
===========================================================================

1. SYNTHESIZED CONTROLS GET AN INVENTED source_path -- '<real_path>#<tag>'.
   The rcmeta sidecar records the SOURCE NODE's real .dfm path for
   synthesized rows (emit_rc.py's make_container_frame_and_synth writes
   owner_node['path'] for SYNTH_CONTAINER_FRAME and every
   SYNTH_RADIOGROUP_ITEM; make_leaf_entry writes the TLabeledEdit's own
   path for SYNTH_LABELEDEDIT_LABEL) -- measured on main.rcmeta.json this
   duplicates 3 real paths across 26 synthesized rows (e.g. rgHP appears 3x:
   1 frame + 2 items). Emitting those verbatim would (a) collide inside
   FormRuntime's path-keyed control index (std::map: silent duplicate-key
   drop), and (b) make RowByPath(source_path) FIND the container's own
   SUBDLG layout row, corrupting the D-GA4-4 "no layout row" selection that
   UiFormMap.h's banner says the miss is FOR. The schema banner therefore
   demands "generator-invented source_path, which by construction never
   collides with a real .dfm path and simply finds no layout row" -- this
   generator is that generator, and the invention rule is:
     SYNTH_CONTAINER_FRAME   -> <real_path>#frame
     SYNTH_RADIOGROUP_ITEM   -> <real_path>#item<N>   (N parsed from the
                                 symbol's mechanical _ITEM<N> suffix, the
                                 same integer emit_rc.py minted it with)
     SYNTH_LABELEDEDIT_LABEL -> <real_path>#label
   '#' cannot occur in a real path (.dfm component names are Pascal
   identifiers, paths are dot-joined -- gated below, not assumed). The
   non-synthesized twin row (e.g. the TLabeledEdit's own Edit entry) keeps
   the real path and is the one that joins the layout table.

2. CAPTIONS / ACTIVEPAGE / TABWIDTH / ROOT CLIENT SIZE COME FROM THE IR.
   The rcmeta dialog rows do not carry the owner's Caption (build_meta
   omits it), and the layout table carries none of the four either (see
   UiFormMap.h banner's gap list). Caption is taken from the IR node whose
   path == owner_path, '' when absent; a non-STR Caption raises rather
   than guessing (none exist in this corpus's owner nodes -- 50 STR / 41
   absent on main). ActivePage is a component NAME in the .dfm ('tsMain');
   it is resolved to the full dotted path by matching the TPageControl's
   own children's names, and an unresolvable name is a hard error, not ''.
   Root client size: .dfm ROOT nodes store ClientWidth/ClientHeight, not
   the Width/Height quartet (same reality emit_rc.py's root_size() handles);
   fallback to geometry width/height only when Client* is absent, then 0
   with a loud warning (main: ClientWidth/ClientHeight present, 1272x763).

3. STRING EMISSION == emit_layout.c_str, IMPORTED NOT COPIED. Same
   precedent as emit_layout.py importing sanitize_ident from emit_rc.py:
   sharing the PURE helper is what keeps the two emitters' UTF-8 string
   literals from drifting apart (real UTF-8 bytes in the file, not \\xHH
   re-encoding -- the one CJK caption on main, gbSimulate's Big5-origin
   text, stays readable/greppable exactly like the layout table's CJK
   font faces).

4. IDEMPOTENT, PURE FUNCTION OF ITS INPUTS. No timestamps, no
   filesystem-order iteration (rcmeta dialogs[] order and IR nodes[] order
   are both stable JSON arrays); re-running produces byte-identical output
   (G7 discipline). Generated files carry the DO-NOT-EDIT banner naming
   this script as the regenerator.

5. SELF-GATES (all hard failures, printed on success too). No golden
   oracle exists for this wave, so the generator refuses to emit anything
   internally inconsistent: row counts vs rcmeta's own dialog_count/
   control_count fields; exactly one is_root (and it is dialogs[0]);
   pre-order (every dialog's nearest enclosing dialog appears earlier);
   every idd_symbol AND control symbol joins the _ids.h #define of the
   same name with the same value (the whole point of the table, so it is
   gated, both id namespaces); tabhost census == TPageControl nodes ==
   IR TABHOST-kind rows, cross-checked against the layout table's
   '"TABHOST"' row count when <form>_layout.gen.cpp exists (advisory-
   skipped with a warning when it does not: B1c output is a sibling
   artefact this generator must not hard-depend on -- same parallel-
   pipeline principle as emit_rc/emit_layout); source_path uniqueness
   (real paths must exist in the IR, invented paths must not).

6. INCLUDE SHAPE OF THE GENERATED HEADER: bare '#include "UiFormMap.h"'.
   The schema header lives in ui/layout/ (a different tree from rc_out/),
   which is on the include path per the GA-4 build contract -- unlike the
   layout emitter's '../DfmLayoutTypes.h' relative includes, whose schema
   header lives inside layout_out/ itself.
"""
from __future__ import print_function

import os
import re
import sys
import json

# Sibling imports (colorconsts/classmap/emit_rc chain) must resolve when the
# script is invoked from any cwd, same as run_b1*.py runners do by living in
# this directory.
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

# PURE, mechanical helpers shared with the sibling emitters (see module
# docstring point 3 -- importing, not copying, is the anti-drift measure).
from emit_layout import EmitError, c_str, c_bool, form_stem_of

GENERATOR_VERSION = 'emit_uimap.py GA-4 v1.0'

# Invented-path tags per synthesis rule (docstring point 1). The three rules
# here are exactly gates.py::_ALLOWED_SYNTH_RULES / emit_rc.py docstring
# point 4's closed set; an unknown rule is a hard error, never a guess.
SYNTH_PATH_TAG = {
    'SYNTH_CONTAINER_FRAME': '#frame',
    'SYNTH_LABELEDEDIT_LABEL': '#label',
}
RE_ITEM_SUFFIX = re.compile(r'_ITEM(\d+)$')


def load_json(path):
    with open(path, 'r', encoding='utf-8') as fh:
        return json.load(fh)


def uimap_source_path(ctrl):
    """rcmeta source_path -> UiMapControl.source_path (docstring point 1)."""
    if not ctrl['synthesized']:
        return ctrl['source_path']
    rule = ctrl['rule']
    if rule == 'SYNTH_RADIOGROUP_ITEM':
        m = RE_ITEM_SUFFIX.search(ctrl['symbol'])
        if not m:
            raise EmitError('SYNTH_RADIOGROUP_ITEM symbol %r lacks the '
                            'mechanical _ITEM<N> suffix' % ctrl['symbol'])
        return '%s#item%s' % (ctrl['source_path'], m.group(1))
    tag = SYNTH_PATH_TAG.get(rule)
    if tag is None:
        raise EmitError('unknown synthesis rule %r on %r -- refusing to '
                        'invent a join key for a rule this generator has '
                        'never audited' % (rule, ctrl['symbol']))
    return ctrl['source_path'] + tag


def ir_caption(node):
    """IR Caption -> UTF-8 text, '' when absent; non-STR is a hard error
    (docstring point 2 -- never guess at data this corpus never produces)."""
    pv = node['properties'].get('Caption')
    if pv is None:
        return ''
    if pv['type'] != 'STR':
        raise EmitError('node %s has a non-STR Caption (%r) -- unhandled '
                        'shape, refusing to stringify blindly'
                        % (node['path'], pv['type']))
    return pv['value']


def scalar_int(node, key, default):
    pv = node['properties'].get(key)
    if pv is None:
        return default
    v = pv['value']
    if not isinstance(v, int):
        raise EmitError('node %s property %s: expected int, got %r'
                        % (node['path'], key, v))
    return v


def resolve_active_page(pc_node, by_path):
    """ActivePage stores a component NAME ('tsMain'); resolve it to the full
    dotted path via the TPageControl's own children (docstring point 2)."""
    pv = pc_node['properties'].get('ActivePage')
    if pv is None:
        return ''
    name = pv['value']
    for cp in pc_node['child_paths']:
        child = by_path.get(cp)
        if child is not None and child['name'] == name:
            return child['path']
    raise EmitError('TPageControl %s: ActivePage %r matches no child (children: %r)'
                    % (pc_node['path'], name,
                       [by_path[cp]['name'] for cp in pc_node['child_paths'] if cp in by_path]))


def root_client_size(root_node):
    """ClientWidth/ClientHeight, falling back to geometry width/height (the
    .dfm Width/Height), then 0 with a loud warning -- same absence-handling
    shape as emit_rc.py's root_size(), but 0 instead of a fabricated 400x300
    because this table's contract (UiFormMap.h) says 0-and-recorded, not
    invented (DfmLayoutTypes.h note 3's no-fabrication sibling rule)."""
    props = root_node['properties']
    geom = root_node.get('geometry') or {}

    def one(client_key, geom_key):
        pv = props.get(client_key)
        if pv is not None and isinstance(pv['value'], int):
            return pv['value']
        v = geom.get(geom_key)
        if isinstance(v, int):
            print('WARNING: ROOT %s absent; fell back to .dfm %s=%d'
                  % (client_key, geom_key, v))
            return v
        print('WARNING: ROOT has neither %s nor %s -- emitting 0 '
              '(record this in the wave report)' % (client_key, geom_key))
        return 0

    return one('ClientWidth', 'width'), one('ClientHeight', 'height')


# ---------------------------------------------------------------------------
# Collection (pure data, no C++ formatting -- same split as emit_layout.py)
# ---------------------------------------------------------------------------

def collect_uimap(rcmeta, ir):
    form_stem = form_stem_of(rcmeta['source_dfm'])
    if form_stem != form_stem_of(ir['source_dfm']):
        raise EmitError('rcmeta/IR form stem mismatch: %r vs %r'
                        % (rcmeta['source_dfm'], ir['source_dfm']))

    by_path = dict((nd['path'], nd) for nd in ir['nodes'])
    root_node = ir['nodes'][0]  # same nodes[0] convention as emit_rc/emit_layout
    if root_node['kind'] != 'ROOT' or '.' in root_node['path']:
        raise EmitError('IR nodes[0] is not the ROOT node (path=%r kind=%r)'
                        % (root_node['path'], root_node['kind']))

    dialogs = []   # (dfm_path, idd_value, owner_class, caption_utf8, is_root, idd_symbol)
    controls = []  # (source_path, id_value, host_dfm_path, synthesized, rule, symbol)
    for d in rcmeta['dialogs']:
        owner = by_path.get(d['owner_path'])
        if owner is None:
            raise EmitError('rcmeta dialog owner_path %r has no IR node'
                            % d['owner_path'])
        dialogs.append((d['owner_path'], d['idd_value'], d['owner_class'],
                        ir_caption(owner), d['is_root'], d['idd_symbol']))
        for c in d['controls']:
            # host_dfm_path = the ENCLOSING rcmeta dialog's owner_path: it is
            # carried per-row because a synthesized control has no layout row
            # to learn its host from (UiFormMap.h field comment).
            controls.append((uimap_source_path(c), c['value'], d['owner_path'],
                             c['synthesized'], c['rule'] or '', c['symbol']))

    # Tab hosts in IR nodes[] order (pre-order, deterministic).
    tabhosts = []  # (dfm_path, active_page_path, tab_width)
    for nd in ir['nodes']:
        if nd['class'] == 'TPageControl':
            tabhosts.append((nd['path'],
                             resolve_active_page(nd, by_path),
                             scalar_int(nd, 'TabWidth', 0)))

    w, h = root_client_size(root_node)
    return {
        'form_stem': form_stem,
        'root_client_w': w,
        'root_client_h': h,
        'dialogs': dialogs,
        'controls': controls,
        'tabhosts': tabhosts,
        'root_path': root_node['path'],
    }


# ---------------------------------------------------------------------------
# Self-gates (docstring point 5) -- every one prints, every one is fatal.
# ---------------------------------------------------------------------------

def parse_ids_header(ids_path):
    defs = {}
    with open(ids_path, 'r', encoding='ascii') as fh:
        for line in fh:
            m = re.match(r'#define\s+(ID[CD]_\w+)\s+(\d+)', line)
            if m:
                defs[m.group(1)] = int(m.group(2))
    return defs


def gate(cond, label, detail=''):
    if not cond:
        raise EmitError('GATE FAILED: %s%s' % (label, (' -- ' + detail) if detail else ''))
    print('[gate] PASS %s%s' % (label, (' (%s)' % detail) if detail else ''))


def run_gates(um, rcmeta, ir, ids_path, layout_cpp_path):
    dialogs, controls, tabhosts = um['dialogs'], um['controls'], um['tabhosts']

    gate(len(dialogs) == rcmeta['dialog_count'],
         'dialog rows == rcmeta dialog_count',
         '%d' % len(dialogs))
    per_dialog_sum = sum(len(d['controls']) for d in rcmeta['dialogs'])
    gate(len(controls) == per_dialog_sum == rcmeta['control_count'],
         'control rows == sum(per-dialog controls) == rcmeta control_count',
         '%d' % len(controls))

    roots = [d for d in dialogs if d[4]]
    gate(len(roots) == 1, 'exactly one is_root dialog row', roots[0][0] if roots else 'none')
    gate(dialogs[0][4] and dialogs[0][0] == um['root_path'],
         'dialogs[0] is the root and matches IR nodes[0]', dialogs[0][0])

    # Pre-order: each non-root dialog's nearest enclosing dialog (walk the
    # dotted owner_path upward) must appear EARLIER in the array -- the
    # creation-order guarantee DialogTreeEngine asserts against (D-GA4-3).
    index_of = dict((d[0], i) for i, d in enumerate(dialogs))
    for i, d in enumerate(dialogs):
        if d[4]:
            continue
        p = d[0]
        parent_idx = None
        while '.' in p:
            p = p.rsplit('.', 1)[0]
            if p in index_of:
                parent_idx = index_of[p]
                break
        if parent_idx is None:
            raise EmitError('GATE FAILED: dialog %r has no enclosing dialog '
                            'ancestor in the array' % d[0])
        if parent_idx >= i:
            raise EmitError('GATE FAILED: pre-order violated -- parent %r '
                            '(index %d) not earlier than %r (index %d)'
                            % (dialogs[parent_idx][0], parent_idx, d[0], i))
    print('[gate] PASS pre-order (every dialog\'s enclosing dialog appears earlier)')

    # The join gate: this table EXISTS to bake symbol->value, so both id
    # namespaces are verified against the _ids.h #defines by name.
    defs = parse_ids_header(ids_path)
    for d in dialogs:
        if defs.get(d[5]) != d[1]:
            raise EmitError('GATE FAILED: ids join -- %s: rcmeta=%r, _ids.h=%r'
                            % (d[5], d[1], defs.get(d[5])))
    for c in controls:
        if defs.get(c[5]) != c[1]:
            raise EmitError('GATE FAILED: ids join -- %s: rcmeta=%r, _ids.h=%r'
                            % (c[5], c[1], defs.get(c[5])))
    gate(True, 'ids join: %d IDD + %d IDC symbols match %s'
         % (len(dialogs), len(controls), os.path.basename(ids_path)))

    # Tab-host census: uimap rows == TPageControl IR nodes == TABHOST-kind
    # IR rows == the layout table's TABHOST rows (when B1c's output exists).
    n_pc = sum(1 for nd in ir['nodes'] if nd['class'] == 'TPageControl')
    n_tabhost_kind = sum(1 for nd in ir['nodes'] if nd['kind'] == 'TABHOST')
    gate(len(tabhosts) == n_pc == n_tabhost_kind,
         'tabhost rows == TPageControl nodes == IR TABHOST-kind rows',
         '%d' % len(tabhosts))
    if os.path.isfile(layout_cpp_path):
        with open(layout_cpp_path, 'r', encoding='utf-8') as fh:
            n_layout = fh.read().count('"TABHOST"')
        gate(n_layout == len(tabhosts),
             'layout table cross-check: "TABHOST" rows in %s'
             % os.path.basename(layout_cpp_path), '%d' % n_layout)
    else:
        print('WARNING: %s not found -- layout-table TABHOST cross-check '
              'skipped (B1c output is a sibling artefact, not a hard '
              'dependency)' % layout_cpp_path)

    for th in tabhosts:
        print('[info] tabhost %s: active_page=%s tab_width=%d'
              % (th[0], th[1] or '(absent)', th[2]))

    # source_path discipline: globally unique; real for non-synthesized rows
    # (they must join the layout table), invented-and-alien for synthesized
    # rows (their layout-row MISS is the D-GA4-4 selector).
    ir_paths = set(nd['path'] for nd in ir['nodes'])
    gate(not any('#' in p for p in ir_paths),
         "no real IR path contains '#' (invented-path namespace is safe)")
    seen = set()
    for c in controls:
        if c[0] in seen:
            raise EmitError('GATE FAILED: duplicate uimap source_path %r' % c[0])
        seen.add(c[0])
        if c[3]:
            if c[0] in ir_paths:
                raise EmitError('GATE FAILED: synthesized row %r collides '
                                'with a real IR path' % c[0])
        else:
            if c[0] not in ir_paths:
                raise EmitError('GATE FAILED: non-synthesized row %r has no '
                                'IR node' % c[0])
    gate(True, 'source_path discipline: %d unique (%d synthesized, invented)'
         % (len(seen), sum(1 for c in controls if c[3])))


# ---------------------------------------------------------------------------
# Emission
# ---------------------------------------------------------------------------

HEADER_TEMPLATE = """\
// ===========================================================================
//  %(h_name)s
//  DO NOT EDIT -- GENERATED FILE (GA-4 uimap join table: dfm_path <->
//  numeric dialog/control ids, plus the caption/tab/root-client data the
//  layout table truthfully lacks). Hand edits will be overwritten.
//  Regenerate: python tools/dfm2rc/emit_uimap.py %(form_arg)s
//  Generator:   %(generator)s
//  Sources:     rc_out/%(rel)s.rcmeta.json + ir_out/%(rel)s.dfm.ir.json
//  Census:      dialogs=%(n_dialogs)d controls=%(n_controls)d tabhosts=%(n_tabhosts)d
//
//  AI(W906-GA4) 20260804: generated join table for %(form_stem)s -- see
//  docs/DESIGN_GA4_UI_ENGINES.md D-GA4-1 and ui/layout/UiFormMap.h for the
//  schema + per-field contracts (including why synthesized rows carry
//  invented '#'-suffixed source_paths). "UiFormMap.h" is a bare include:
//  ui/layout/ is on the include path (GA-4 build contract), unlike the
//  layout emitter's schema header which lives inside layout_out/ itself.
// ===========================================================================
#ifndef HT9045_GEN_UIMAP_%(form_stem_upper)s_H
#define HT9045_GEN_UIMAP_%(form_stem_upper)s_H

#include "UiFormMap.h"

namespace ht9045_uimap {

%(extern_decls)s
} // namespace ht9045_uimap

#endif // HT9045_GEN_UIMAP_%(form_stem_upper)s_H
"""

SOURCE_HEADER_TEMPLATE = """\
// ===========================================================================
//  %(cpp_name)s
//  DO NOT EDIT -- GENERATED FILE. See the matching _uimap.gen.h banner.
// ===========================================================================
#include "%(own_header)s"

namespace ht9045_uimap {

"""


def emit_header(um, form_arg, rel):
    stem = um['form_stem']
    decls = []
    if um['dialogs']:
        decls.append('extern const UiMapDialog  k%sUiMapDialogs[];\n' % stem)
    if um['controls']:
        decls.append('extern const UiMapControl k%sUiMapControls[];\n' % stem)
    if um['tabhosts']:
        decls.append('extern const UiMapTabHost k%sUiMapTabHosts[];\n' % stem)
    decls.append('extern const UiMapForm    k%sUiMap;\n' % stem)
    return HEADER_TEMPLATE % {
        'h_name': stem + '_uimap.gen.h',
        'form_arg': form_arg,
        'generator': GENERATOR_VERSION,
        'rel': rel,
        'n_dialogs': len(um['dialogs']),
        'n_controls': len(um['controls']),
        'n_tabhosts': len(um['tabhosts']),
        'form_stem': stem,
        'form_stem_upper': stem.upper(),
        'extern_decls': ''.join(decls),
    }


def emit_source(um, own_header_name):
    stem = um['form_stem']
    out = [SOURCE_HEADER_TEMPLATE % {
        'cpp_name': stem + '_uimap.gen.cpp',
        'own_header': own_header_name,
    }]

    if um['dialogs']:
        out.append('// rcmeta dialogs[] order == dialog-owner discovery order == tree\n')
        out.append('// pre-order (parents first) -- the creation-order contract D-GA4-3\n')
        out.append('// builds on and this generator gates.\n')
        out.append('const UiMapDialog k%sUiMapDialogs[] = {\n' % stem)
        for (path, idd, owner_class, caption, is_root, _sym) in um['dialogs']:
            out.append('    { %s, %d, %s, %s, %s },\n'
                       % (c_str(path), idd, c_str(owner_class), c_str(caption),
                          c_bool(is_root)))
        out.append('};\n\n')

    if um['controls']:
        out.append('// Synthesized rows carry invented \'#\'-suffixed source_paths (see the\n')
        out.append('// generator docstring point 1): unique join keys that by construction\n')
        out.append('// miss the layout table -- that miss selects the D-GA4-4 rescale set.\n')
        out.append('const UiMapControl k%sUiMapControls[] = {\n' % stem)
        for (path, val, host, synth, rule, _sym) in um['controls']:
            out.append('    { %s, %d, %s, %s, %s },\n'
                       % (c_str(path), val, c_str(host), c_bool(synth), c_str(rule)))
        out.append('};\n\n')

    if um['tabhosts']:
        out.append('// One row per TPageControl (IR pre-order). active_page_path is the\n')
        out.append('// ActivePage NAME resolved to a full dotted path at generation time.\n')
        out.append('const UiMapTabHost k%sUiMapTabHosts[] = {\n' % stem)
        for (path, active, width) in um['tabhosts']:
            out.append('    { %s, %s, %d },\n' % (c_str(path), c_str(active), width))
        out.append('};\n\n')

    out.append('const UiMapForm k%sUiMap = {\n' % stem)
    out.append('    %s,\n' % c_str(stem))
    out.append('    %d, %d,  // ROOT ClientWidth/ClientHeight (IR; .dfm ROOT stores no Width/Height)\n'
               % (um['root_client_w'], um['root_client_h']))
    out.append('    %s, %d,\n' % (('k%sUiMapDialogs' % stem) if um['dialogs'] else 'nullptr',
                                  len(um['dialogs'])))
    out.append('    %s, %d,\n' % (('k%sUiMapControls' % stem) if um['controls'] else 'nullptr',
                                  len(um['controls'])))
    out.append('    %s, %d,\n' % (('k%sUiMapTabHosts' % stem) if um['tabhosts'] else 'nullptr',
                                  len(um['tabhosts'])))
    out.append('};\n\n')
    out.append('} // namespace ht9045_uimap\n')
    return ''.join(out)


# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------

def emit_for_form(rel, dfm2rc_root):
    """rel = form path-stem relative to the out roots ('main',
    'ARMS/ARMSLog', ...). Returns the two output paths."""
    rel = rel.replace('\\', '/')
    rcmeta_path = os.path.join(dfm2rc_root, 'rc_out', *(rel + '.rcmeta.json').split('/'))
    ir_path = os.path.join(dfm2rc_root, 'ir_out', *(rel + '.dfm.ir.json').split('/'))
    ids_path = os.path.join(dfm2rc_root, 'rc_out', *(rel + '_ids.h').split('/'))
    layout_cpp_path = os.path.join(dfm2rc_root, 'layout_out', *(rel + '_layout.gen.cpp').split('/'))

    rcmeta = load_json(rcmeta_path)
    ir = load_json(ir_path)
    um = collect_uimap(rcmeta, ir)
    run_gates(um, rcmeta, ir, ids_path, layout_cpp_path)

    out_dir = os.path.dirname(rcmeta_path)
    h_name = um['form_stem'] + '_uimap.gen.h'
    cpp_name = um['form_stem'] + '_uimap.gen.cpp'
    h_path = os.path.join(out_dir, h_name)
    cpp_path = os.path.join(out_dir, cpp_name)

    with open(h_path, 'w', encoding='utf-8', newline='\n') as fh:
        fh.write(emit_header(um, rel, rel))
    with open(cpp_path, 'w', encoding='utf-8', newline='\n') as fh:
        fh.write(emit_source(um, h_name))

    print('emitted %s + %s (dialogs=%d controls=%d tabhosts=%d root_client=%dx%d)'
          % (h_path, cpp_path, len(um['dialogs']), len(um['controls']),
             len(um['tabhosts']), um['root_client_w'], um['root_client_h']))
    return h_path, cpp_path


def main(argv):
    if len(argv) not in (2, 3):
        print('usage: emit_uimap.py <form-relstem> [<dfm2rc-root>]', file=sys.stderr)
        print('  e.g.: emit_uimap.py main', file=sys.stderr)
        print('        emit_uimap.py ARMS/ARMSLog', file=sys.stderr)
        return 2
    rel = argv[1]
    root = argv[2] if len(argv) == 3 else os.path.dirname(os.path.abspath(__file__))
    emit_for_form(rel, root)
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))
