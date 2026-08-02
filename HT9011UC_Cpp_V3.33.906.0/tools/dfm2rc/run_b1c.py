# -*- coding: utf-8 -*-
"""
run_b1c.py -- W7-B1c driver: generate *_layout.gen.{h,cpp} + *_events.gen.json
for all 133 forms from W7-B1a's IR, run gate G9 (full-corpus, not just a
sample), and write a machine-readable report.

AI(W906-W7-B1c) 20260728: new file (docs/W7_UI_ARCHITECTURE_PLAN.md S6
W7-B1c). Mirrors run_b1a.py's driver shape/conventions on purpose (same
report-directory, same "print summary + write full per-file report" split)
so a reviewer who has already read run_b1a.py recognizes this immediately.

GATE G9 -- INDEPENDENCE. Per the plan: "閘 G9：<Form>_layout.gen.cpp 用既有的
MinGW C++14 CMake 編過（-Wall），加一個 ctest target 拿獨立從 .dfm 導出的
fixture 對表". This driver's Python-side half of G9 reuses gates.py's
independent_scan() -- the SEPARATE line-oriented .dfm scanner already built
and vetted for W7-B1a's G1-G4 (it does not call any of dfm_parse.py's
object/property/value-dispatch code, see gates.py's own docstring) -- as the
independent oracle for every row's geometry (Left/Top/Width/Height) and
TabOrder, across the FULL corpus (22,627+133 nodes), not merely a hand-picked
sample: this is strictly stronger than the brief's minimum ask and costs
nothing extra since the scanner already exists. Re-deriving colour/font/
custom-prop truth independently is NOT attempted here (that would mean
writing a second full property-value parser, which is exactly the
duplicated-complexity gates.py's own docstring says to avoid) -- the C++
side of G9 (tests/test_DfmLayoutGen.cpp) instead hand-verifies a curated
sample of colour/font/custom-prop rows against golden .dfm text read
directly by a human (not through either scanner), which is the genuinely
independent oracle for those fields.
"""
from __future__ import print_function

import os
import sys
import json
import subprocess
import collections

import dfm_parse as P
import gates as G
import emit_layout as E

HERE = os.path.dirname(os.path.abspath(__file__))
IR_OUT_DIR = os.path.join(HERE, 'ir_out')
LAYOUT_OUT_DIR = os.path.join(HERE, 'layout_out')
REPORTS_DIR = os.path.join(HERE, 'reports')


def find_ir_files():
    out = []
    for dp, dn, fn in os.walk(IR_OUT_DIR):
        for f in fn:
            if f.endswith('.dfm.ir.json'):
                out.append(os.path.join(dp, f))
    out.sort()
    return out


def run_g9_geometry_crosscheck(golden_root, ir, rows):
    """Full-corpus independent geometry/TabOrder cross-check for one form.
    Returns list of mismatch dicts (empty == pass for this form)."""
    gpath = os.path.join(golden_root, *ir['source_dfm'].replace('\\', '/').split('/'))
    ind_nodes = G.independent_scan(gpath)
    ind_by_path = dict((n['path'], n) for n in ind_nodes)
    problems = []
    for row in rows:
        ind = ind_by_path.get(row['dfm_path'])
        if ind is None:
            problems.append({'code': 'G9_NO_INDEPENDENT_NODE', 'path': row['dfm_path']})
            continue
        for key, indkey, haskey in (
                ('left', 'Left', 'has_left'), ('top', 'Top', 'has_top'),
                ('width', 'Width', 'has_width'), ('height', 'Height', 'has_height')):
            ind_val = ind['geom'].get(indkey)
            if row[haskey]:
                if ind_val != row[key]:
                    problems.append({'code': 'G9_GEOM_MISMATCH:%s' % key, 'path': row['dfm_path'],
                                      'layout': row[key], 'independent': ind_val})
            elif ind_val is not None:
                problems.append({'code': 'G9_GEOM_MISSING_IN_LAYOUT:%s' % key, 'path': row['dfm_path'],
                                  'independent': ind_val})
        if row['has_tab_order']:
            if ind['tab_order'] != row['tab_order']:
                problems.append({'code': 'G9_TABORDER_MISMATCH', 'path': row['dfm_path'],
                                  'layout': row['tab_order'], 'independent': ind['tab_order']})
        elif ind['tab_order'] is not None:
            problems.append({'code': 'G9_TABORDER_MISSING_IN_LAYOUT', 'path': row['dfm_path'],
                              'independent': ind['tab_order']})
    return problems


def compile_smoke_test(cpp_paths, max_files=None):
    """'the tables must compile under MinGW g++ today' -- batch
    -fsyntax-only over every generated .cpp (fast: no codegen, no link).
    Returns (ok_count, failures) where failures is a list of
    (path, stderr) for anything that did not compile clean."""
    import shutil
    gxx = shutil.which('g++') or shutil.which('g++.exe')
    if gxx is None:
        return None, []  # g++ not on PATH in this environment -- reported, not faked
    failures = []
    ok = 0
    paths = cpp_paths if max_files is None else cpp_paths[:max_files]
    for p in paths:
        try:
            r = subprocess.run(
                [gxx, '-std=c++14', '-Wall', '-Wextra', '-I', LAYOUT_OUT_DIR, '-fsyntax-only', p],
                stdout=subprocess.PIPE, stderr=subprocess.PIPE, timeout=120)
        except Exception as e:
            failures.append((p, 'EXC:%r' % e))
            continue
        if r.returncode != 0:
            failures.append((p, r.stderr.decode('utf-8', 'replace')))
        else:
            ok += 1
    return ok, failures


def main(argv):
    ir_files = find_ir_files()
    if not ir_files:
        print('no IR files found under %s -- run W7-B1a (run_b1a.py) first' % IR_OUT_DIR, file=sys.stderr)
        return 2
    golden_root = P._find_golden_root()

    total_rows = 0
    total_events = 0
    total_custom_props = 0
    kind_counter = collections.Counter()
    custom_class_counter = collections.Counter()
    all_anomalies = []
    all_g9_problems = []
    per_file = []
    generated_cpp_paths = []

    for ir_path in ir_files:
        with open(ir_path, 'r', encoding='utf-8') as fh:
            ir = json.load(fh)

        form_stem, rows, events, anomalies, out_paths = E.emit_form(ir_path, ir, LAYOUT_OUT_DIR)
        generated_cpp_paths.append(out_paths['cpp'])

        total_rows += len(rows)
        total_events += len(events)
        for row in rows:
            kind_counter[row['kind']] += 1
            if row['custom_props']:
                total_custom_props += len(row['custom_props'])
                custom_class_counter[row['vcl_class']] += 1

        for a in anomalies:
            all_anomalies.append({'source_dfm': ir['source_dfm'], 'code': a[0], 'path': a[1],
                                   'prop': a[2], 'token': a[3]})

        g9_problems = run_g9_geometry_crosscheck(golden_root, ir, rows)
        for p in g9_problems:
            p2 = dict(p)
            p2['source_dfm'] = ir['source_dfm']
            all_g9_problems.append(p2)

        per_file.append({
            'source_dfm': ir['source_dfm'], 'form_stem': form_stem,
            'row_count': len(rows), 'event_count': len(events),
            'anomaly_count': len(anomalies), 'g9_problem_count': len(g9_problems),
            'out_h': out_paths['h'], 'out_cpp': out_paths['cpp'], 'out_json': out_paths['json'],
        })

    ok_count, compile_failures = compile_smoke_test(generated_cpp_paths)

    summary = {
        'schema_version': 1,
        'generator': E.GENERATOR_VERSION,
        'files': len(ir_files),
        'total_layout_rows': total_rows,
        'total_events': total_events,
        'total_custom_props': total_custom_props,
        'kind_histogram': dict(kind_counter),
        'custom_prop_class_histogram': dict(custom_class_counter),
        'unresolved_color_tokens': all_anomalies,
        'g9_full_corpus_geometry_taborder_crosscheck': {
            'problem_count': len(all_g9_problems),
            'problems_sample': all_g9_problems[:50],
        },
        'g9_compile_smoke': {
            'gxx_available': ok_count is not None,
            'ok_count': ok_count,
            'failure_count': len(compile_failures),
            'failures_sample': [{'path': p, 'stderr': s[:2000]} for p, s in compile_failures[:10]],
        },
        'known_cross_checks': {
            'control_nodes_matches_b1a_22627': total_rows - len(ir_files) == 22627,
            'unresolved_color_tokens_zero': len(all_anomalies) == 0,
            'g9_geometry_taborder_zero_mismatch': len(all_g9_problems) == 0,
        },
    }

    if not os.path.isdir(REPORTS_DIR):
        os.makedirs(REPORTS_DIR)
    report = {'summary': summary, 'per_file': per_file}
    report_path = os.path.join(REPORTS_DIR, 'b1c_gate_report.json')
    with open(report_path, 'w', encoding='utf-8') as fh:
        json.dump(report, fh, ensure_ascii=False, indent=1, sort_keys=True)
        # AI(W906-Gate4) 20260802: json.dump leaves no trailing newline, which put every generated .json in violation of plan S12 gate 4; emit one so a fresh regen is gate-clean at birth (the checked-in corpora were swept to match in the same commit, keeping gate G7 byte-identical).
        fh.write('\n')

    print(json.dumps(summary, ensure_ascii=False, indent=2))
    print('Full report: %s' % report_path)
    print('Generated layout tables under: %s' % LAYOUT_OUT_DIR)

    fail = (len(all_g9_problems) > 0) or (len(all_anomalies) > 0) or (len(compile_failures) > 0)
    return 1 if fail else 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))
