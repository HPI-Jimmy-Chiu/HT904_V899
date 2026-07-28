# -*- coding: utf-8 -*-
"""
gates.py -- W7-B1a verification gates G0-G4.

AI(W906-W7-B1a) 20260728: new file. Implements the "pipeline must be
VERIFIABLE WITHOUT A COMPILER" principle from
docs/W7_UI_ARCHITECTURE_PLAN.md S6-W7-B1a: parse the source .dfm and the
IR independently and assert structural isomorphism, so a dropped or
invented control is caught mechanically.

Gate definitions (plan wording, S6-W7-B1a):
  G0  parser completeness: 0 unparsed lines, 0 unbalanced object/end
      across all 133 forms.
  G1  census bijection: IR <-> .dfm one-to-one, with exactly two allowed
      asymmetries -- 331 NONVISUAL-excluded nodes, and synthesized nodes
      (0 at this stage; synthesis is W7-B1d's job).
  G2  geometry verbatim: IR pixel values == .dfm integer literals,
      exact equality.
  G3  sibling order == .dfm declaration order.
  G4  TabOrder is a 0..n-1 permutation per sibling group (38 named
      exceptions expected, per plan).

INDEPENDENCE. dfm_parse.py's recursive-descent parser is the primary
producer of the IR. To make G1-G3 a genuine cross-check rather than the
same code asking itself "am I right?", this module re-derives the facts
those gates need (node identity/depth/order, and Left/Top/Width/Height/
TabOrder scalars) via `independent_scan()`, a *separate*, deliberately
much simpler line-oriented scanner that does not import or call any of
dfm_parse.py's object/property parsing functions. It only understands:
  - object/inline/inherited headers and bare 'end' (for tree shape),
  - a tiny amount of collection-awareness (`Key = <` ... `end>` / `>`)
    so the 7 real `<item ... end>` collections in this corpus don't
    desync its 'end' bookkeeping against the enclosing object stack,
  - `Left|Top|Width|Height|TabOrder = <int>` scalar lines, attributed to
    whichever object is topmost on its stack at that point (always
    correct because those properties are declared immediately after the
    object header, before any nested child is opened).
It deliberately does NOT understand strings, blobs, or generic property
values -- it does not need to, and not teaching it that grammar is what
keeps it independent of dfm_parse.py's value-dispatch code (a bug in
one is very unlikely to be mirrored in the other).
"""
from __future__ import print_function

import os
import re
import sys
import json
import collections

import dfm_parse as P
import classmap as CM

RE_OBJ2 = re.compile(
    r'^(object|inline|inherited)\s+(?:([A-Za-z_]\w*)\s*:\s*)?([A-Za-z_]\w*)\s*(?:\[(\d+)\])?\s*$')
RE_GEOM2 = re.compile(r'^(Left|Top|Width|Height)\s*=\s*(-?\d+)\s*$')
RE_TABORDER2 = re.compile(r'^TabOrder\s*=\s*(-?\d+)\s*$')


def independent_scan(path):
    """Second, deliberately-separate implementation of .dfm tree-shape +
    geometry extraction. See module docstring."""
    text, _enc = P.read_text(path)
    lines = [l.rstrip('\r') for l in text.split('\n')]
    stack = []          # list of dict records, innermost last
    sib_counters = {}
    nodes = []
    coll_depth = 0

    for raw in lines:
        s = raw.strip()
        if not s:
            continue

        if coll_depth > 0:
            if s == 'item' or s == 'end':
                continue
            if s == 'end>' or s == '>':
                coll_depth -= 1
                continue
            continue  # any property line inside a collection item: ignore

        if s.endswith('= <') and not s.endswith('= <>'):
            coll_depth += 1
            continue

        m = RE_OBJ2.match(s)
        if m:
            objkind, name, cls, idxs = m.groups()
            if name is None:
                name = '_anon_L%d' % (lines.index(raw) + 1)
            parent = stack[-1] if stack else None
            parent_path = parent['path'] if parent else None
            key = parent_path
            sib = sib_counters.get(key, 0)
            sib_counters[key] = sib + 1
            path_ = (parent_path + '.' + name) if parent_path else name
            rec = {
                'path': path_, 'name': name, 'cls': cls,
                'depth': len(stack), 'sibling_index': sib,
                'parent_path': parent_path,
                'geom': {}, 'tab_order': None,
            }
            nodes.append(rec)
            stack.append(rec)
            continue

        if s == 'end':
            if stack:
                stack.pop()
            continue

        if stack:
            gm = RE_GEOM2.match(s)
            if gm:
                stack[-1]['geom'][gm.group(1)] = int(gm.group(2))
                continue
            tm = RE_TABORDER2.match(s)
            if tm:
                stack[-1]['tab_order'] = int(tm.group(1))
                continue

    return nodes


# ---------------------------------------------------------------------------
# G0
# ---------------------------------------------------------------------------

def gate_g0(form):
    """Parser completeness for one form: 0 unparsed lines, 0 unbalanced
    object/end. Returns list of problems (empty == pass)."""
    problems = []
    for code, detail in form.errors:
        problems.append({'gate': 'G0', 'code': code, 'detail': detail})
    return problems


# ---------------------------------------------------------------------------
# G1 census bijection
# ---------------------------------------------------------------------------

def gate_g1(form, ir_nodes, independent_nodes):
    """IR <-> .dfm bijection. Allowed asymmetry: NONVISUAL nodes and
    synthesized nodes (none at B1a). Cross-checks total counts AND,
    node-by-node, (name, class, depth, sibling_index, parent_path)."""
    problems = []
    if len(ir_nodes) != len(independent_nodes):
        problems.append({
            'gate': 'G1', 'code': 'COUNT_MISMATCH',
            'detail': 'ir=%d independent=%d' % (len(ir_nodes), len(independent_nodes)),
        })
    by_path_ir = dict((n['path'], n) for n in ir_nodes)
    by_path_ind = dict((n['path'], n) for n in independent_nodes)
    ir_paths = set(by_path_ir)
    ind_paths = set(by_path_ind)
    missing_in_ir = sorted(ind_paths - ir_paths)
    invented_in_ir = sorted(ir_paths - ind_paths)
    for p in missing_in_ir:
        problems.append({'gate': 'G1', 'code': 'DROPPED_IN_IR', 'detail': p})
    for p in invented_in_ir:
        node = by_path_ir[p]
        if not node.get('synthesized'):
            problems.append({'gate': 'G1', 'code': 'INVENTED_IN_IR', 'detail': p})
    for p in sorted(ir_paths & ind_paths):
        a = by_path_ir[p]
        b = by_path_ind[p]
        for field in ('name', 'class', 'depth', 'sibling_index', 'parent_path'):
            av = a[field] if field != 'class' else a['class']
            bv = b['cls'] if field == 'class' else b[field]
            if av != bv:
                problems.append({
                    'gate': 'G1', 'code': 'FIELD_MISMATCH:%s' % field,
                    'detail': '%s ir=%r independent=%r' % (p, av, bv),
                })
    nonvisual = [n for n in ir_nodes if n['kind'] == 'NONVISUAL']
    synthesized = [n for n in ir_nodes if n.get('synthesized')]
    return problems, {'nonvisual_count': len(nonvisual), 'synthesized_count': len(synthesized)}


# ---------------------------------------------------------------------------
# G2 geometry verbatim
# ---------------------------------------------------------------------------

def gate_g2(ir_nodes, independent_nodes):
    problems = []
    by_path_ind = dict((n['path'], n) for n in independent_nodes)
    for n in ir_nodes:
        geom = n.get('geometry')
        if not geom:
            continue
        ind = by_path_ind.get(n['path'])
        if ind is None:
            continue  # already reported by G1
        for key, indkey in (('left', 'Left'), ('top', 'Top'), ('width', 'Width'), ('height', 'Height')):
            ir_val = geom.get(key)
            ind_val = ind['geom'].get(indkey)
            if ir_val != ind_val:
                problems.append({
                    'gate': 'G2', 'code': 'GEOM_MISMATCH:%s' % key,
                    'detail': '%s ir=%r independent=%r' % (n['path'], ir_val, ind_val),
                })
    return problems


# ---------------------------------------------------------------------------
# G3 sibling declaration order
# ---------------------------------------------------------------------------

def gate_g3(ir_nodes, independent_nodes):
    problems = []
    ir_order = [n['path'] for n in ir_nodes]
    ind_order = [n['path'] for n in independent_nodes]
    if ir_order != ind_order:
        # Report first divergence point rather than a huge diff dump.
        for i, (a, b) in enumerate(zip(ir_order, ind_order)):
            if a != b:
                problems.append({
                    'gate': 'G3', 'code': 'ORDER_DIVERGE_AT_%d' % i,
                    'detail': 'ir=%r independent=%r' % (a, b),
                })
                break
        if len(ir_order) != len(ind_order):
            problems.append({
                'gate': 'G3', 'code': 'ORDER_LENGTH_MISMATCH',
                'detail': 'ir=%d independent=%d' % (len(ir_order), len(ind_order)),
            })
    return problems


# ---------------------------------------------------------------------------
# G4 TabOrder permutation
# ---------------------------------------------------------------------------

def gate_g4(ir_nodes):
    """Group children by parent_path; among those that carry a TabOrder,
    it must be a 0..n-1 permutation. Non-permutation groups are collected
    as *named exceptions* (plan expects ~38 across the whole corpus), not
    treated as G0-style hard failures."""
    by_parent = collections.defaultdict(list)
    for n in ir_nodes:
        if n.get('tab_order') is not None:
            by_parent[n['parent_path']].append(n)
    exceptions = []
    ok_groups = 0
    for parent_path, kids in by_parent.items():
        vals = sorted(k['tab_order'] for k in kids)
        expected = list(range(len(kids)))
        if vals == expected:
            ok_groups += 1
        else:
            exceptions.append({
                'parent_path': parent_path,
                'count': len(kids),
                'tab_orders': vals,
            })
    return exceptions, ok_groups


# ---------------------------------------------------------------------------
# Driver: run all gates for one file
# ---------------------------------------------------------------------------

def run_gates_for_file(path, relpath):
    form = P.parse_dfm_file(path)
    ir = P.form_to_ir(form, relpath)
    ir_nodes = ir['nodes']
    independent_nodes = independent_scan(path)

    problems = []
    problems += gate_g0(form)
    g1_problems, g1_stats = gate_g1(form, ir_nodes, independent_nodes)
    problems += g1_problems
    problems += gate_g2(ir_nodes, independent_nodes)
    problems += gate_g3(ir_nodes, independent_nodes)
    g4_exceptions, g4_ok_groups = gate_g4(ir_nodes)

    return {
        'source_dfm': relpath.replace('\\', '/'),
        'problems': problems,
        'g1_stats': g1_stats,
        'g4_exceptions': g4_exceptions,
        'g4_ok_groups': g4_ok_groups,
        'control_node_count': ir['control_node_count'],
    }, ir


# ---------------------------------------------------------------------------
# RC_CENSUS -- IR <-> .rcmeta.json control-emission bijection.
#
# AI(W906-W7-B1-review) 20260728: new gate, added in direct response to an
# independent review finding (docs/W7_UI_ARCHITECTURE_PLAN.md S6 W7-B1d
# "checker 必須知道...否則會誤報/漏報" principle, applied to a hole neither
# G1 nor G6 covers). NOT one of the plan's pre-enumerated G0-G10 -- it is
# tagged 'RC_CENSUS' rather than a G-number so it is never confused with
# those. The hole: emit_rc.py's dialog-owner walk (collect_dialog_owner_
# paths / direct_leaf_entries) only ever descended into a child when that
# child's *own* kind was SUBDLG or TABHOST. A node classified LEAF (e.g.
# TChart, which is a real HWND-owning VCL control, unlike most other LEAF
# classes) can still have real further .dfm children of its own -- golden
# cObserver.dfm:1919 'object ChartYield: TChart' contains :1959
# 'object SpeedButton1: TSpeedButton', :1973 'object edYieldMax: TEdit',
# :1982 'object edYieldMin: TEdit' (all three event-wired: OnClick=...).
# The old walk never visited them at all: not as a new dialog owner
# (right -- LEAF never gets its own template) and not as a CONTROL entry
# of the *enclosing* dialog owner either (wrong -- direct_leaf_entries
# only looked at an owner's *direct* .dfm children, never a LEAF child's
# own children). They vanished from the .rc/_ids.h/.rcmeta.json with zero
# trace and zero gate failure: G1 (above) only proves .dfm<->IR fidelity
# (ChartYield's children are still real IR nodes, G1 passes); G6
# (check_g6.py) only proves .rc-text<->.res-binary fidelity (nothing to
# diff against once the emitter itself never wrote them). This gate closes
# that hole at the one place it is actually visible: reconciling the full
# IR node census against what the emitter's own .rcmeta.json sidecar says
# it emitted.
# ---------------------------------------------------------------------------

def gate_rc_census(ir_nodes, meta):
    """Every IR node whose kind is LEAF, TABHOST or SUBDLG must appear
    EXACTLY ONCE in the form's .rcmeta.json -- either as some dialog's
    owner_path (SUBDLG only; the form ROOT also owns a dialog, but its own
    IR kind is 'ROOT', deliberately excluded from this invariant since it
    is not one of the three kinds being reconciled) or as a non-
    synthesized control's source_path (LEAF/TABHOST). Synthesized entries
    (SYNTH_RADIOGROUP_ITEM / SYNTH_LABELEDEDIT_LABEL / SYNTH_CONTAINER_FRAME)
    are excluded on purpose -- they have no real IR node of their own to
    reconcile against; that is precisely what "synthesized" means (S6-B1d's
    two permitted synthesis rules plus the TGroupBox/TRadioGroup self-drawn
    frame). Returns a list of problems (empty == pass)."""
    problems = []
    expected = set(n['path'] for n in ir_nodes if n['kind'] in ('LEAF', 'TABHOST', 'SUBDLG'))
    counts = collections.Counter()
    for d in meta['dialogs']:
        counts[d['owner_path']] += 1
        for c in d['controls']:
            if not c['synthesized']:
                counts[c['source_path']] += 1
    for p in sorted(expected):
        n = counts.get(p, 0)
        if n == 0:
            problems.append({'gate': 'RC_CENSUS', 'code': 'DROPPED', 'detail': p})
        elif n > 1:
            problems.append({
                'gate': 'RC_CENSUS', 'code': 'DUPLICATE',
                'detail': '%s (%d occurrences)' % (p, n),
            })
    return problems


# ---------------------------------------------------------------------------
# SYNTH_PREDICTED -- IR <-> .rcmeta.json synthesized-entry equality.
#
# AI(W906-W7-B1-review) 20260728: new gate (MEDIUM-3 from the independent
# review: "a plan-mandated checker was never implemented, so nothing
# constrains invention"). gate_rc_census (above) proves emit_rc.py dropped
# nothing REAL; nothing previously proved the converse -- that its three
# permitted synthesis rules (module docstring point 4 of emit_rc.py:
# SYNTH_CONTAINER_FRAME one BS_GROUPBOX per TGroupBox/TRadioGroup dialog
# owner, SYNTH_RADIOGROUP_ITEM one per TRadioGroup Items.Strings entry,
# SYNTH_LABELEDEDIT_LABEL one Static label per classmap.py
# synthesizes_label=True control) are the ONLY thing the emitter invents,
# in the exact counts those rules predict. Deliberately independent of
# emit_rc.py's own synthesis code (make_container_frame_and_synth /
# make_leaf_entry are never imported or called here) -- this recomputes
# the expected set straight from raw IR node/class/property data, reusing
# only classmap.py's declarative tables (a data fact, the same kind of
# reuse gate_rc_census's own docstring already sanctions), then diffs
# against whatever the sidecar's synthesized:true entries actually say.
# ---------------------------------------------------------------------------

_ALLOWED_SYNTH_RULES = frozenset([
    'SYNTH_CONTAINER_FRAME', 'SYNTH_RADIOGROUP_ITEM', 'SYNTH_LABELEDEDIT_LABEL',
])


def gate_synth_predicted(ir_nodes, meta):
    """Every synthesized entry in the sidecar must be predicted by one of
    the three rules above, in the predicted count, keyed by (owning node
    path, rule) -- and nothing predicted may be missing either. Returns a
    list of problems (empty == pass)."""
    problems = []
    expected = collections.Counter()
    for n in ir_nodes:
        if n['kind'] == 'SUBDLG' and n['class'] in CM.CONTAINER_FRAME_CLASSES:
            expected[(n['path'], 'SYNTH_CONTAINER_FRAME')] += 1
        if n['kind'] == 'SUBDLG' and n['class'] in CM.CONTAINER_RADIO_SYNTH_CLASSES:
            items_pv = n['properties'].get('Items.Strings')
            items = items_pv['value'] if items_pv and items_pv['type'] == 'LIST' else []
            if items:
                expected[(n['path'], 'SYNTH_RADIOGROUP_ITEM')] += len(items)
        if n['kind'] in ('LEAF', 'TABHOST'):
            spec, _is_fallback = CM.lookup(n['class'])
            if spec.synthesizes_label:
                expected[(n['path'], 'SYNTH_LABELEDEDIT_LABEL')] += 1

    actual = collections.Counter()
    for d in meta['dialogs']:
        for c in d['controls']:
            if not c['synthesized']:
                continue
            rule = c['rule']
            actual[(c['source_path'], rule)] += 1
            if rule not in _ALLOWED_SYNTH_RULES:
                problems.append({
                    'gate': 'SYNTH_PREDICTED', 'code': 'UNKNOWN_SYNTH_RULE',
                    'detail': '%s rule=%r' % (c['source_path'], rule),
                })

    for key in sorted(set(expected) | set(actual)):
        exp_n = expected.get(key, 0)
        act_n = actual.get(key, 0)
        if exp_n != act_n:
            path, rule = key
            problems.append({
                'gate': 'SYNTH_PREDICTED', 'code': 'COUNT_MISMATCH',
                'detail': '%s rule=%s expected=%d actual=%d' % (path, rule, exp_n, act_n),
            })
    return problems


# ---------------------------------------------------------------------------
# ID_SYMBOL_JOIN -- W7-B1c layout-table id_symbol <-> W7-B1b .rcmeta.json
# symbol, actually asserted equal.
#
# AI(W906-W7-B1-review) 20260728: new gate (MEDIUM-2 from the independent
# review: DfmLayoutTypes.h note 1 told W7-B1d to "JOIN the two artifacts on
# that string" but nothing ever ran the join -- it was a promise in a
# comment, and emit_layout.compute_id_symbol was in fact not kind-aware,
# silently wrong for every SUBDLG/NONVISUAL row (3,835 of 22,627) until
# this same review pass fixed it. This gate is the actual, asserted join:
# for every layout row, its id_symbol string must be exactly the symbol
# W7-B1b's .rcmeta.json records for the same dfm_path (a dialog's
# idd_symbol keyed by owner_path for SUBDLG rows, a non-synthesized
# control's symbol keyed by source_path for LEAF/TABHOST rows), and ROOT/
# NONVISUAL rows must carry '' (neither owns a B1b symbol -- ROOT's own
# IDD_ symbol is tracked by B1b under a different key, its own dialog
# entry with owner_path == the ROOT path itself, which is deliberately
# excluded from this join since D11 does not ask this table to duplicate
# it; NONVISUAL is excluded from the .rc entirely, G1's own documented
# asymmetry).
# ---------------------------------------------------------------------------

def gate_id_symbol_join(ir_nodes, meta, layout_rows):
    """Returns a list of problems (empty == pass). See module comment
    above for exactly what is being joined and why."""
    problems = []
    dialog_symbol_by_owner = dict((d['owner_path'], d['idd_symbol']) for d in meta['dialogs'])
    control_symbol_by_source = {}
    for d in meta['dialogs']:
        for c in d['controls']:
            if not c['synthesized']:
                control_symbol_by_source[c['source_path']] = c['symbol']

    for row in layout_rows:
        path = row['dfm_path']
        kind = row['kind']
        if kind in ('ROOT', 'NONVISUAL'):
            if row['id_symbol'] != '':
                problems.append({'gate': 'ID_SYMBOL_JOIN', 'code': 'EXPECTED_EMPTY', 'detail': path})
            continue
        if kind == 'SUBDLG':
            b1b_symbol = dialog_symbol_by_owner.get(path)
        else:  # LEAF / TABHOST
            b1b_symbol = control_symbol_by_source.get(path)
        if b1b_symbol is None:
            problems.append({'gate': 'ID_SYMBOL_JOIN', 'code': 'NO_B1B_COUNTERPART', 'detail': path})
        elif b1b_symbol != row['id_symbol']:
            problems.append({
                'gate': 'ID_SYMBOL_JOIN', 'code': 'SYMBOL_MISMATCH',
                'detail': '%s layout=%r b1b=%r' % (path, row['id_symbol'], b1b_symbol),
            })
    return problems


def main(argv):
    golden_root = P._find_golden_root()
    files = P.iter_golden_dfm_files(golden_root)
    all_problems = []
    total_nodes = 0
    total_nonvisual = 0
    total_g4_exceptions = 0
    total_g4_ok_groups = 0
    per_file = []
    for f in files:
        rel = os.path.relpath(f, golden_root)
        result, _ir = run_gates_for_file(f, rel)
        per_file.append(result)
        all_problems.extend(result['problems'])
        total_nodes += result['control_node_count']
        total_nonvisual += result['g1_stats']['nonvisual_count']
        total_g4_exceptions += len(result['g4_exceptions'])
        total_g4_ok_groups += result['g4_ok_groups']

    summary = {
        'files': len(files),
        'total_control_nodes': total_nodes,
        'total_nonvisual_nodes': total_nonvisual,
        'total_problems': len(all_problems),
        'g4_exception_groups': total_g4_exceptions,
        'g4_ok_groups': total_g4_ok_groups,
        'per_file': per_file,
    }
    print(json.dumps({
        'files': summary['files'],
        'total_control_nodes': summary['total_control_nodes'],
        'total_nonvisual_nodes': summary['total_nonvisual_nodes'],
        'total_problems': summary['total_problems'],
        'g4_exception_groups': summary['g4_exception_groups'],
        'g4_ok_groups': summary['g4_ok_groups'],
    }, indent=2))
    if all_problems:
        print('First 20 problems:', file=sys.stderr)
        for p in all_problems[:20]:
            print(' ', p, file=sys.stderr)
    return summary


if __name__ == '__main__':
    s = main(sys.argv)
    sys.exit(0 if s['total_problems'] == 0 else 1)
