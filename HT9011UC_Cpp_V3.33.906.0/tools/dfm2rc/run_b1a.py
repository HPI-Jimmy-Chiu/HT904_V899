# -*- coding: utf-8 -*-
"""
run_b1a.py -- W7-B1a driver: parse all 133 golden .dfm forms, emit the IR
JSON for each, run gates G0-G4 across the whole corpus, and write a single
machine-readable gate report.

AI(W906-W7-B1a) 20260728: new file (docs/W7_UI_ARCHITECTURE_PLAN.md
S6-W7-B1a "全語料跑" driver for this wave's own gates; W7-B1d owns the later
full-corpus run that also wires in B1b/B1c's gates G5-G9).

IR OUTPUT LOCATION -- deliberate deviation from the plan's diagram, flagged
here rather than silently decided: S6's pipeline diagram shows
`<path>/<Form>.dfm.ir.json` mirrored directly under PORTED at the same
relative path as golden. This driver instead writes IR under
`tools/dfm2rc/ir_out/<relpath>/<Form>.dfm.ir.json`, still mirroring
golden's relative path, but *rooted under this wave's own authorized
tools/dfm2rc/ tree* rather than the top of PORTED. Two independent
reasons:
  1. File-ownership safety (plan S8): several golden directories that own
     .dfm files (e.g. "ASE_K Socket", ATC, AutoAlignment, AutoTeach,
     BinDisplay, GroundMan, LoadCCD, MR, Mes, Monitor, OmronLaser,
     PMAlarm, ProductionInfo, RotateKit, SortingBinTray, TempCtrl,
     VacuumUnit) do not exist yet as directories in PORTED at all -- most
     of their C++ has not been translated. Creating those directories
     now, for a JSON side-artifact, is outside this wave's brief ("all
     new files, you touch NO existing source") and would pre-empt a
     directory-layout decision that rightfully belongs to whichever wave
     first translates that module.
  2. It keeps 100% of this wave's output under the single directory the
     brief authorizes (PORTED\tools\dfm2rc\*), which is the simplest
     possible thing to reason about for parallel-wave file ownership.
W7-B1b/B1c (or the B1d integrator) can trivially copy/move these files
next to the .rc they generate once that placement is decided, or just
read them in place from tools/dfm2rc/ir_out/ -- nothing about the JSON
content depends on where the file lives.
"""
from __future__ import print_function

import os
import sys
import json
import collections

import dfm_parse as P
import gates as G

HERE = os.path.dirname(os.path.abspath(__file__))
IR_OUT_DIR = os.path.join(HERE, 'ir_out')
REPORTS_DIR = os.path.join(HERE, 'reports')


def ir_output_path(relpath):
    rel_no_ext = relpath.replace('\\', '/') + '.ir.json'
    return os.path.join(IR_OUT_DIR, *rel_no_ext.split('/'))


def main(argv):
    golden_root = P._find_golden_root()
    files = P.iter_golden_dfm_files(golden_root)
    if not os.path.isdir(golden_root):
        print('GOLDEN root not found: %r' % golden_root, file=sys.stderr)
        return 2

    per_file_reports = []
    all_problems = []
    total_nodes = 0
    total_nonvisual = 0
    total_g4_exceptions = 0
    total_g4_ok_groups = 0
    total_blobs = 0
    total_blob_bytes = 0
    kind_counter = collections.Counter()
    class_counter = collections.Counter()
    graphic_control_count = 0
    graphic_control_count_plan_6760 = 0
    encoding_counter = collections.Counter()

    for f in files:
        rel = os.path.relpath(f, golden_root)
        gate_result, ir = G.run_gates_for_file(f, rel)

        out_path = ir_output_path(rel)
        out_dir = os.path.dirname(out_path)
        if not os.path.isdir(out_dir):
            os.makedirs(out_dir)
        with open(out_path, 'w', encoding='utf-8') as fh:
            json.dump(ir, fh, ensure_ascii=False, indent=1, sort_keys=True)
            # AI(W906-Gate4) 20260802: json.dump leaves no trailing newline, which put every generated .json in violation of plan S12 gate 4; emit one so a fresh regen is gate-clean at birth (the checked-in corpora were swept to match in the same commit, keeping gate G7 byte-identical).
            fh.write('\n')

        per_file_reports.append(gate_result)
        all_problems.extend(gate_result['problems'])
        total_nodes += gate_result['control_node_count']
        total_nonvisual += gate_result['g1_stats']['nonvisual_count']
        total_g4_exceptions += len(gate_result['g4_exceptions'])
        total_g4_ok_groups += gate_result['g4_ok_groups']
        encoding_counter[ir['encoding_used']] += 1

        for node in ir['nodes']:
            is_root = node['parent_path'] is None
            # NOTE: blob properties (e.g. a form's own Icon.Data, see
            # main.dfm:17) can live directly on the ROOT node, not just on
            # child controls -- so the blob scan below deliberately does
            # NOT skip the root. Only the kind/class/graphic-control
            # tallies (which are meaningless for the form object itself)
            # are root-excluded.
            if not is_root:
                kind_counter[node['kind']] += 1
                class_counter[node['class']] += 1
                if node['is_graphic_control']:
                    graphic_control_count += 1
                if node['class'] in P.GRAPHIC_CONTROL_CLASSES_PLAN_6760:
                    graphic_control_count_plan_6760 += 1
            for key, pv in node['properties'].items():
                if pv['type'] == 'BLOB':
                    total_blobs += 1
                    total_blob_bytes += pv['value']['byte_len']

    summary = {
        'schema_version': 1,
        'generator': P.GENERATOR_VERSION,
        'files': len(files),
        'total_control_nodes': total_nodes,
        'total_nonvisual_nodes': total_nonvisual,
        'total_problems': len(all_problems),
        'g4_exception_group_count': total_g4_exceptions,
        'g4_ok_group_count': total_g4_ok_groups,
        'blob_count': total_blobs,
        'blob_bytes': total_blob_bytes,
        'blob_mb': round(total_blob_bytes / 1048576.0, 3),
        'kind_histogram': dict(kind_counter),
        'graphic_control_count_full_set': graphic_control_count,
        'graphic_control_count_plan_cited_6760_set': graphic_control_count_plan_6760,
        'top_classes': class_counter.most_common(25),
        'encoding_used_histogram': dict(encoding_counter),
        'known_plan_cross_checks': {
            'nonvisual_331_expected': total_nonvisual == 331,
            'control_nodes_22627_expected': total_nodes == 22627,
            'blob_554_expected': total_blobs == 554,
            'g4_exceptions_38_cited_actual': total_g4_exceptions,
        },
    }

    if not os.path.isdir(REPORTS_DIR):
        os.makedirs(REPORTS_DIR)
    report = {
        'summary': summary,
        'per_file': per_file_reports,
    }
    report_path = os.path.join(REPORTS_DIR, 'b1a_gate_report.json')
    with open(report_path, 'w', encoding='utf-8') as fh:
        json.dump(report, fh, ensure_ascii=False, indent=1, sort_keys=True)
        # AI(W906-Gate4) 20260802: json.dump leaves no trailing newline, which put every generated .json in violation of plan S12 gate 4; emit one so a fresh regen is gate-clean at birth (the checked-in corpora were swept to match in the same commit, keeping gate G7 byte-identical).
        fh.write('\n')

    print(json.dumps(summary, ensure_ascii=False, indent=2))
    print('Full report: %s' % report_path)
    print('IR JSON files under: %s' % IR_OUT_DIR)
    return 0 if len(all_problems) == 0 else 1


if __name__ == '__main__':
    sys.exit(main(sys.argv))
