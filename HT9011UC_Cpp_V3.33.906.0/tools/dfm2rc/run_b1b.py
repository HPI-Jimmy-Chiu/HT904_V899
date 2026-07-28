# -*- coding: utf-8 -*-
"""
run_b1b.py -- W7-B1b: full-corpus driver. Generates `.rc`/`_ids.h`/
`.rcmeta.json` for every `<Form>.dfm.ir.json` produced by W7-B1a, compiles
every `.rc` with both `rc.exe` (authority, S9-R2) and `windres` (secondary
runner) if available, runs gate G6 against the `rc.exe` output, and writes
a machine-readable gate report mirroring B1a's `reports/b1a_gate_report.json`
shape.

AI(W906-W7-B1b) 20260728: new file (plan doc:
HT9011UC_Cpp_V3.33.906.0/docs/W7_UI_ARCHITECTURE_PLAN.md, S6-W7-B1b gates
G5/G6). Every external process invocation is timeout-bounded (30s per
compile -- these are small, deterministic, non-interactive resource
compiles; a hang would indicate a real bug, not legitimate slowness) and
Windows resource compilers never raise UI, so no SetErrorMode guard is
needed (unlike a probe that might crash into a Debug-CRT dialog).

OUTPUT LAYOUT (deliberately staged under tools/dfm2rc/, NOT mirrored into
the PORTED tree root -- see the wave's final report for why: B1a itself
already established this "generated artifacts live under tools/dfm2rc/,
not scattered into module directories that mostly don't exist yet for
un-translated UI forms" convention (ir_out/), and this file follows it for
consistency + zero collision risk with any sibling wave):
    tools/dfm2rc/rc_out/<mirrored golden relpath>/<Form>.rc
    tools/dfm2rc/rc_out/<mirrored golden relpath>/<Form>_ids.h
    tools/dfm2rc/rc_out/<mirrored golden relpath>/<Form>.rcmeta.json
    tools/dfm2rc/res_out/<mirrored golden relpath>/<Form>.res           (rc.exe)
    tools/dfm2rc/res_out/<mirrored golden relpath>/<Form>.windres.res   (windres, if present)
    tools/dfm2rc/reports/b1b_gate_report.json
"""
from __future__ import print_function

import os
import re
import sys
import json
import glob
import subprocess
import collections

import emit_rc as ER
import check_g6 as G6
import classmap as CM

HERE = os.path.dirname(os.path.abspath(__file__))
IR_OUT = os.path.join(HERE, 'ir_out')
RC_OUT = os.path.join(HERE, 'rc_out')
RES_OUT = os.path.join(HERE, 'res_out')
REPORTS = os.path.join(HERE, 'reports')

COMPILE_TIMEOUT_SEC = 30

# Matches an actual #include directive naming afxres.h -- deliberately
# anchored to '#include' so this does NOT false-positive on the emitted
# .rc's own banner/comment text that *talks about* afxres.h being avoided
# (an early version of this driver did exactly that and had to be fixed).
RE_AFXRES_INCLUDE = re.compile(r'^\s*#\s*include\s*[<"]afxres\.h[>"]', re.MULTILINE)


def find_rc_exe():
    candidates = glob.glob(r'C:\Program Files (x86)\Windows Kits\10\bin\*\x64\rc.exe')
    candidates.sort(reverse=True)
    return candidates[0] if candidates else None


def find_sdk_include_dirs(rc_exe_path):
    # .../Windows Kits/10/bin/<ver>/x64/rc.exe -> .../Windows Kits/10/Include/<ver>/{um,shared}
    bindir = os.path.dirname(os.path.dirname(rc_exe_path))  # .../bin/<ver>
    ver = os.path.basename(bindir)
    root = os.path.dirname(os.path.dirname(bindir))  # .../Windows Kits/10
    inc = os.path.join(root, 'Include', ver)
    return [os.path.join(inc, 'um'), os.path.join(inc, 'shared')]


def find_windres():
    for cand in (r'C:\MinGW\bin\windres.exe',):
        if os.path.isfile(cand):
            return cand
    return None


def iter_ir_files():
    files = glob.glob(os.path.join(IR_OUT, '**', '*.dfm.ir.json'), recursive=True)
    files.sort()
    return files


def relpath_no_ext(ir_path):
    rel = os.path.relpath(ir_path, IR_OUT)
    rel = rel.replace('\\', '/')
    assert rel.endswith('.dfm.ir.json')
    return rel[:-len('.dfm.ir.json')]


def run_compiler(argv, cwd, env):
    try:
        p = subprocess.run(argv, cwd=cwd, env=env, stdout=subprocess.PIPE,
                            stderr=subprocess.STDOUT, timeout=COMPILE_TIMEOUT_SEC)
        return p.returncode, p.stdout.decode('utf-8', errors='replace')
    except subprocess.TimeoutExpired:
        return -1, 'TIMEOUT after %ds' % COMPILE_TIMEOUT_SEC


def main(argv):
    limit = None
    if len(argv) > 1:
        limit = int(argv[1])

    rc_exe = find_rc_exe()
    windres = find_windres()
    if rc_exe is None:
        print('FATAL: rc.exe not found under Windows Kits', file=sys.stderr)
        return 3
    inc_dirs = find_sdk_include_dirs(rc_exe)
    print('rc.exe =', rc_exe)
    print('windres =', windres)
    print('include dirs =', inc_dirs)

    env = dict(os.environ)
    env['MSYS_NO_PATHCONV'] = '1'
    env['MSYS2_ARG_CONV_EXCL'] = '*'

    ir_files = iter_ir_files()
    if limit:
        ir_files = ir_files[:limit]

    per_file = []
    total_dialogs = 0
    total_controls = 0
    total_fallback_classes = collections.Counter()
    total_problems = 0
    rc_fail = []
    windres_fail = []
    skip_vendor_hits = []
    skip_no_source_hits = []

    for ir_path in ir_files:
        rel = relpath_no_ext(ir_path)
        form_name = os.path.basename(rel)
        out_dir_rc = os.path.join(RC_OUT, os.path.dirname(rel)) if os.path.dirname(rel) else RC_OUT
        out_dir_res = os.path.join(RES_OUT, os.path.dirname(rel)) if os.path.dirname(rel) else RES_OUT
        os.makedirs(out_dir_rc, exist_ok=True)
        os.makedirs(out_dir_res, exist_ok=True)

        rc_path = os.path.join(out_dir_rc, form_name + '.rc')
        ids_path = os.path.join(out_dir_rc, form_name + '_ids.h')
        meta_path = os.path.join(out_dir_rc, form_name + '.rcmeta.json')
        res_path = os.path.join(out_dir_res, form_name + '.res')
        windres_res_path = os.path.join(out_dir_res, form_name + '.windres.res')

        entry = {'source_dfm': rel + '.dfm', 'problems': []}
        try:
            fe, meta = ER.emit_from_ir_file(ir_path, rc_path, ids_path, meta_path)
        except Exception as e:
            entry['problems'].append({'gate': 'EMIT', 'code': 'EXCEPTION', 'detail': repr(e)})
            per_file.append(entry)
            total_problems += 1
            continue

        entry['dialog_count'] = meta['dialog_count']
        entry['control_count'] = meta['control_count']
        entry['fallback_classes_used'] = meta['fallback_classes_used']
        total_dialogs += meta['dialog_count']
        total_controls += meta['control_count']
        for c in meta['fallback_classes_used']:
            total_fallback_classes[c] += 1
        for d in meta['dialogs']:
            for ctl in d['controls']:
                cls = None
                # recover original vcl class isn't stored per-control in
                # meta; fallback_classes_used already captures the class
                # names that hit the safety net, which is what matters.
                pass

        # G5: rc.exe (authority)
        rc_argv = [rc_exe, '/nologo']
        for d in inc_dirs:
            rc_argv += ['/i', d]
        rc_argv += ['/fo', res_path, rc_path]
        rc_rc, rc_out_txt = run_compiler(rc_argv, out_dir_rc, env)
        entry['rc_exe_returncode'] = rc_rc
        if rc_rc != 0:
            entry['problems'].append({'gate': 'G5', 'code': 'RC_EXE_FAIL', 'detail': rc_out_txt[-2000:]})
            rc_fail.append(rel)

        # G5: windres (secondary runner, S9-R2 -- not authoritative)
        if windres is not None:
            wr_argv = [windres, '--input-format=rc', '--output-format=res',
                       '-o', windres_res_path, rc_path]
            wr_rc, wr_out_txt = run_compiler(wr_argv, out_dir_rc, env)
            entry['windres_returncode'] = wr_rc
            if wr_rc != 0:
                entry['problems'].append({'gate': 'G5', 'code': 'WINDRES_FAIL', 'detail': wr_out_txt[-2000:]})
                windres_fail.append(rel)

        # G6: binary read-back diff, against the rc.exe-compiled .res only
        # (rc.exe is the authority per S9-R2; windres is a secondary CI
        # runner whose job is only "did it also compile", not re-diffed).
        if rc_rc == 0:
            try:
                g6_problems, g6_stats = G6.check_one_form(meta_path, res_path)
                entry['problems'].extend(g6_problems)
            except Exception as e:
                entry['problems'].append({'gate': 'G6', 'code': 'EXCEPTION', 'detail': repr(e)})

        # afxres.h / DS_CONTROL leak check (the G10 note in plan S6-B1b's
        # DIALOGEX rule -- no formal gate number is defined for this in
        # the plan text, so this driver applies it as a direct textual
        # sanity check on the generated .rc rather than inventing a gate
        # name that isn't in the plan).
        with open(rc_path, 'r', encoding='ascii') as f:
            rc_text = f.read()
        if RE_AFXRES_INCLUDE.search(rc_text):
            entry['problems'].append({'gate': 'NOTE', 'code': 'AFXRES_LEAK', 'detail': rc_path})

        for c in meta['fallback_classes_used']:
            if c in CM.SKIP_VENDOR_CLASSES:
                skip_vendor_hits.append((rel, c))
            if c in CM.SKIP_NO_SOURCE_CLASSES:
                skip_no_source_hits.append((rel, c))
        # SKIP-listed classes go through the *curated* map (not the
        # fallback path) so they never show up in fallback_classes_used;
        # detect them directly from the sidecar's control win32_class.
        for d in meta['dialogs']:
            for ctl in d['controls']:
                for vendor_cls, reg_name in (('TEasyMain', 'HT9045.EasyMain'),
                                              ('TECameraPicolo', 'HT9045.ECameraPicolo'),
                                              ('TAPHeadLabel', 'HT9045.APHeadLabel')):
                    if ctl['win32_class'] == reg_name:
                        if vendor_cls in CM.SKIP_VENDOR_CLASSES:
                            skip_vendor_hits.append((rel, vendor_cls))
                        else:
                            skip_no_source_hits.append((rel, vendor_cls))

        total_problems += len(entry['problems'])
        per_file.append(entry)

    summary = {
        'generator': ER.GENERATOR_VERSION,
        'files': len(ir_files),
        'total_dialogs': total_dialogs,
        'total_controls': total_controls,
        'total_problems': total_problems,
        'rc_exe_fail_count': len(rc_fail),
        'rc_exe_fail_forms': rc_fail,
        'windres_fail_count': len(windres_fail),
        'windres_fail_forms': windres_fail,
        'fallback_classes_used_histogram': dict(total_fallback_classes),
        'skip_vendor_hits': sorted(set(skip_vendor_hits)),
        'skip_no_source_hits': sorted(set(skip_no_source_hits)),
    }

    os.makedirs(REPORTS, exist_ok=True)
    report = {'summary': summary, 'per_file': per_file}
    with open(os.path.join(REPORTS, 'b1b_gate_report.json'), 'w', encoding='utf-8') as f:
        json.dump(report, f, ensure_ascii=False, indent=2)

    print(json.dumps(summary, indent=2, ensure_ascii=False))
    return 0 if total_problems == 0 else 1


if __name__ == '__main__':
    sys.exit(main(sys.argv))
