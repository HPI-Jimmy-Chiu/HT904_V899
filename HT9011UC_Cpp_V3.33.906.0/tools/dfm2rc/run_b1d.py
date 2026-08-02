# -*- coding: utf-8 -*-
"""
run_b1d.py -- W7-B1d: full-corpus run + gate triage + CMake wiring driver.

AI(W906-W7-B1d) 20260728: new file (docs/W7_UI_ARCHITECTURE_PLAN.md S6
W7-B1d "全語料跑 + 閘門分流 + CMake 接線"). This is the aggregation/ratchet
layer over the 3 already-landed B1a/B1b/B1c generators -- it does NOT
reimplement parsing/emission logic (that would duplicate dfm_parse.py/
emit_rc.py/emit_layout.py, exactly the "second full property parser"
gates.py's own docstring warns against). It imports those modules directly
and drives them itself (does not shell out to run_b1a.py/run_b1b.py/
run_b1c.py, so it is not coupled to their hardcoded output-directory
globals) so every mode below can target an arbitrary output root -- needed
for the G7 idempotency proof, which must write to a *second* location and
byte-diff, never overwrite the checked-in canonical artifacts in place.

Four independently-invokable modes, one per CMake/ctest target this wave
wires up (see tests/CMakeLists.txt, "W7-B1d" block):

  fidelity     Re-derive IR + .rc/_ids.h/.rcmeta.json + layout tables from
               golden .dfm, FROM SCRATCH, for all 133 forms, into a fresh
               root. Runs G0-G6 and G9's geometry/taborder cross-check
               against that fresh output (not against whatever happens to
               already sit in ir_out/rc_out/layout_out -- this is the
               "don't trust a possibly-stale committed report" gate).
               Also runs the mojibake/BOM scan (S12 ratchet #4) over the
               freshly generated files.

  idempotent   Runs the exact same regeneration as `fidelity` into a THIRD
               location, then byte-diffs every generated .dfm.ir.json /
               .rc / _ids.h / .rcmeta.json / _layout.gen.{h,cpp} /
               _events.gen.json against the checked-in canonical copies
               under ir_out/ rc_out/ layout_out/ (G7). Deliberately does
               NOT diff the compiled .res binaries -- those are a
               downstream compiler artifact already covered by G5/G6, not
               a claim this generator itself makes about its own output.

  rc_compiles  Fast path: compiles the EXISTING (already checked-in)
               rc_out/**/*.rc corpus with both rc.exe (authority) and
               windres (secondary), full 133/133, without regenerating
               anything. This is what a normal `ctest` run should use day
               to day (regen from golden .dfm on every `ctest` invocation
               would make the suite golden-directory-dependent at test
               time, which the other 95 tests deliberately are not).

  layout_full  Fast path: g++ -fsyntax-only over the EXISTING (checked-in)
               layout_out/**/*_layout.gen.cpp corpus, full 133/133 -- the
               "test_DfmLayout" full-corpus counterpart to the sibling
               W7-B1c wave's curated 4-form linked ctest
               (test_DfmLayoutGen).

Every mode prints a JSON summary to stdout and a non-zero-if-any-problem
return code, and (for fidelity/idempotent) writes a per-form pass/fail
table plus the aggregate to reports/b1d_gate_report.json. Kept dependency-
free of run_b1a.py/run_b1b.py/run_b1c.py's own report files on purpose --
those remain each wave's own claim, this is B1d's independent one.
"""
from __future__ import print_function

import os
import re
import sys
import json
import glob
import shutil
import filecmp
import subprocess
import collections

import dfm_parse as P
import gates as G
import emit_rc as ER
import emit_layout as EL
import check_g6 as G6
import classmap as CM

HERE = os.path.dirname(os.path.abspath(__file__))
IR_OUT_CANON = os.path.join(HERE, 'ir_out')
RC_OUT_CANON = os.path.join(HERE, 'rc_out')
LAYOUT_OUT_CANON = os.path.join(HERE, 'layout_out')
REPORTS_DIR = os.path.join(HERE, 'reports')

COMPILE_TIMEOUT_SEC = 30
GXX_TIMEOUT_SEC = 120

RE_AFXRES_INCLUDE = re.compile(r'^\s*#\s*include\s*[<"]afxres\.h[>"]', re.MULTILINE)

# U+FFFD (REPLACEMENT CHARACTER) as UTF-8 bytes, and a UTF-8 BOM -- the two
# things S12 ratchet #4 requires be zero across the whole generated tree.
_UFFFD_UTF8 = b'\xef\xbf\xbd'
_UTF8_BOM = b'\xef\xbb\xbf'


def find_rc_exe():
    candidates = glob.glob(r'C:\Program Files (x86)\Windows Kits\10\bin\*\x64\rc.exe')
    candidates.sort(reverse=True)
    return candidates[0] if candidates else None


def find_sdk_include_dirs(rc_exe_path):
    bindir = os.path.dirname(os.path.dirname(rc_exe_path))
    ver = os.path.basename(bindir)
    root = os.path.dirname(os.path.dirname(bindir))
    inc = os.path.join(root, 'Include', ver)
    return [os.path.join(inc, 'um'), os.path.join(inc, 'shared')]


def find_windres():
    for cand in (r'C:\MinGW\bin\windres.exe',):
        if os.path.isfile(cand):
            return cand
    return None


def find_gxx():
    return shutil.which('g++') or shutil.which('g++.exe')


def run_proc(argv, cwd=None, env=None, timeout=COMPILE_TIMEOUT_SEC):
    try:
        p = subprocess.run(argv, cwd=cwd, env=env, stdout=subprocess.PIPE,
                            stderr=subprocess.STDOUT, timeout=timeout)
        return p.returncode, p.stdout.decode('utf-8', errors='replace')
    except subprocess.TimeoutExpired:
        return -1, 'TIMEOUT after %ds' % timeout


def scan_mojibake_bom(root):
    """S12 ratchet #4: 0 U+FFFD, 0 BOM across the given tree. Only looks at
    the text artifacts this pipeline itself emits (.json/.rc/.h/.cpp) --
    binary .res/.bmp/.ico blobs are excluded on purpose, a BOM/FFFD check
    on binary data is meaningless noise, not a real gate."""
    hits = []
    exts = ('.json', '.rc', '.h', '.cpp', '.hpp')
    for dp, dn, fn in os.walk(root):
        for f in fn:
            if not f.endswith(exts):
                continue
            p = os.path.join(dp, f)
            with open(p, 'rb') as fh:
                data = fh.read()
            if _UFFFD_UTF8 in data:
                hits.append({'file': p, 'code': 'MOJIBAKE_UFFFD'})
            if data.startswith(_UTF8_BOM):
                hits.append({'file': p, 'code': 'BOM'})
    return hits


# ---------------------------------------------------------------------------
# Shared regeneration core (used by both `fidelity` and `idempotent`)
# ---------------------------------------------------------------------------

def regenerate_all(out_root, rc_exe, windres, gxx, inc_dirs, env, do_compile):
    """Full pipeline B1a(parse)->B1b(rc emit[+compile+G6])->B1c(layout emit
    [+G9 crosscheck+compile smoke]) from golden .dfm into out_root. Returns
    (per_form list, summary dict, generated file lists) for the caller."""
    golden_root = P._find_golden_root()
    files = P.iter_golden_dfm_files(golden_root)

    ir_out = os.path.join(out_root, 'ir_out')
    rc_out = os.path.join(out_root, 'rc_out')
    res_out = os.path.join(out_root, 'res_out')
    layout_out = os.path.join(out_root, 'layout_out')

    per_form = []
    total_problems = 0
    total_nodes = 0
    total_nonvisual = 0
    total_dialogs = 0
    total_controls_rc = 0
    total_layout_rows = 0
    total_events = 0
    rc_fail = []
    windres_fail = []
    layout_cpp_paths = []
    g9_problems_total = 0
    rc_census_problems_total = 0
    synth_predicted_problems_total = 0
    id_symbol_join_problems_total = 0

    for f in files:
        rel = os.path.relpath(f, golden_root)
        form_problems = []

        # --- B1a: parse + G0-G4 ---
        gate_result, ir = G.run_gates_for_file(f, rel)
        form_problems.extend(gate_result['problems'])
        total_nodes += gate_result['control_node_count']
        total_nonvisual += gate_result['g1_stats']['nonvisual_count']

        rel_no_ext = rel.replace('\\', '/') + '.ir.json'
        ir_path = os.path.join(ir_out, *rel_no_ext.split('/'))
        os.makedirs(os.path.dirname(ir_path), exist_ok=True)
        with open(ir_path, 'w', encoding='utf-8') as fh:
            json.dump(ir, fh, ensure_ascii=False, indent=1, sort_keys=True)
            # AI(W906-Gate4) 20260802: json.dump leaves no trailing newline, which put every generated .json in violation of plan S12 gate 4; emit one so a fresh regen is gate-clean at birth (the checked-in corpora were swept to match in the same commit, keeping gate G7 byte-identical).
            fh.write('\n')

        # --- B1b: .rc/_ids.h/.rcmeta.json emit (+ compile + G6) ---
        rel_stem = ir['source_dfm'].rsplit('.', 1)[0]
        form_name = os.path.basename(rel_stem)
        out_dir_rc = os.path.join(rc_out, os.path.dirname(rel_stem)) if os.path.dirname(rel_stem) else rc_out
        out_dir_res = os.path.join(res_out, os.path.dirname(rel_stem)) if os.path.dirname(rel_stem) else res_out
        os.makedirs(out_dir_rc, exist_ok=True)

        rc_path = os.path.join(out_dir_rc, form_name + '.rc')
        ids_path = os.path.join(out_dir_rc, form_name + '_ids.h')
        meta_path = os.path.join(out_dir_rc, form_name + '.rcmeta.json')
        res_path = os.path.join(out_dir_res, form_name + '.res')
        windres_res_path = os.path.join(out_dir_res, form_name + '.windres.res')

        fe, meta = ER.emit_from_ir_file(ir_path, rc_path, ids_path, meta_path)
        total_dialogs += meta['dialog_count']
        total_controls_rc += meta['control_count']

        # RC_CENSUS (gates.py; NOT one of the plan's G0-G10 -- added post-
        # hoc by an independent review): every IR node whose kind is
        # LEAF/TABHOST/SUBDLG must show up exactly once in this form's own
        # .rcmeta.json. Catches emit_rc.py silently dropping a node's
        # children (see gates.py's own docstring for the ChartYield case
        # this was written against).
        rc_census_problems = G.gate_rc_census(ir['nodes'], meta)
        form_problems.extend(rc_census_problems)
        rc_census_problems_total += len(rc_census_problems)

        # SYNTH_PREDICTED (gates.py; MEDIUM-3 from independent review): the
        # mirror image of RC_CENSUS above -- proves the emitter invented
        # nothing beyond its own three documented synthesis rules, in
        # exactly the counts those rules predict from the IR.
        synth_predicted_problems = G.gate_synth_predicted(ir['nodes'], meta)
        form_problems.extend(synth_predicted_problems)
        synth_predicted_problems_total += len(synth_predicted_problems)

        if do_compile:
            os.makedirs(out_dir_res, exist_ok=True)
            rc_argv = [rc_exe, '/nologo']
            for d in inc_dirs:
                rc_argv += ['/i', d]
            rc_argv += ['/fo', res_path, rc_path]
            rc_rc, rc_txt = run_proc(rc_argv, cwd=out_dir_rc, env=env)
            if rc_rc != 0:
                form_problems.append({'gate': 'G5', 'code': 'RC_EXE_FAIL', 'detail': rc_txt[-2000:]})
                rc_fail.append(rel_stem)

            if windres is not None:
                wr_argv = [windres, '--input-format=rc', '--output-format=res',
                           '-o', windres_res_path, rc_path]
                wr_rc, wr_txt = run_proc(wr_argv, cwd=out_dir_rc, env=env)
                if wr_rc != 0:
                    form_problems.append({'gate': 'G5', 'code': 'WINDRES_FAIL', 'detail': wr_txt[-2000:]})
                    windres_fail.append(rel_stem)

            if rc_rc == 0:
                try:
                    g6_problems, _ = G6.check_one_form(meta_path, res_path)
                    form_problems.extend(g6_problems)
                except Exception as e:
                    form_problems.append({'gate': 'G6', 'code': 'EXCEPTION', 'detail': repr(e)})

            # AI(W906-W7-B1-review) 20260728: MEDIUM-4 fix. Delete each
            # form's compiled .res scratch files right after G6 has read
            # them, the same way mode_rc_compiles() already does below --
            # rather than let 133 forms' worth accumulate under out_root
            # until the *next* run's shutil.rmtree has to remove them all
            # at once (see _clean_dir()'s docstring for why that rmtree
            # must already be tolerant, and why this reduces how often it
            # needs to be).
            for extra in (res_path, windres_res_path):
                if os.path.isfile(extra):
                    try:
                        os.remove(extra)
                    except OSError:
                        pass

        with open(rc_path, 'r', encoding='ascii') as f2:
            if RE_AFXRES_INCLUDE.search(f2.read()):
                form_problems.append({'gate': 'NOTE', 'code': 'AFXRES_LEAK', 'detail': rc_path})

        # --- B1c: layout/events emit (+ G9 crosscheck) ---
        form_stem, rows, events, anomalies, out_paths = EL.emit_form(ir_path, ir, layout_out)
        total_layout_rows += len(rows)
        total_events += len(events)
        layout_cpp_paths.append(out_paths['cpp'])
        for a in anomalies:
            form_problems.append({'gate': 'G9', 'code': 'UNRESOLVED_COLOR:' + a[0], 'detail': a[1]})

        # ID_SYMBOL_JOIN (gates.py; MEDIUM-2 from independent review): the
        # actual, asserted join between this form's B1c layout rows'
        # id_symbol strings and B1b's own .rcmeta.json symbols -- was
        # previously only a promise in DfmLayoutTypes.h note 1's prose.
        id_symbol_join_problems = G.gate_id_symbol_join(ir['nodes'], meta, rows)
        form_problems.extend(id_symbol_join_problems)
        id_symbol_join_problems_total += len(id_symbol_join_problems)

        ind_nodes = G.independent_scan(f)
        ind_by_path = dict((n['path'], n) for n in ind_nodes)
        for row in rows:
            ind = ind_by_path.get(row['dfm_path'])
            if ind is None:
                form_problems.append({'gate': 'G9', 'code': 'NO_INDEPENDENT_NODE', 'detail': row['dfm_path']})
                continue
            for key, indkey, haskey in (
                    ('left', 'Left', 'has_left'), ('top', 'Top', 'has_top'),
                    ('width', 'Width', 'has_width'), ('height', 'Height', 'has_height')):
                ind_val = ind['geom'].get(indkey)
                if row[haskey]:
                    if ind_val != row[key]:
                        form_problems.append({'gate': 'G9', 'code': 'GEOM_MISMATCH:' + key, 'detail': row['dfm_path']})
                elif ind_val is not None:
                    form_problems.append({'gate': 'G9', 'code': 'GEOM_MISSING:' + key, 'detail': row['dfm_path']})
            if row['has_tab_order']:
                if ind['tab_order'] != row['tab_order']:
                    form_problems.append({'gate': 'G9', 'code': 'TABORDER_MISMATCH', 'detail': row['dfm_path']})
            elif ind['tab_order'] is not None:
                form_problems.append({'gate': 'G9', 'code': 'TABORDER_MISSING', 'detail': row['dfm_path']})
        g9_problems_total += sum(1 for p2 in form_problems if p2['gate'] == 'G9')

        total_problems += len(form_problems)
        per_form.append({
            'source_dfm': rel.replace('\\', '/'),
            'problem_count': len(form_problems),
            'problems': form_problems,
            'status': 'PASS' if not form_problems else 'FAIL',
        })

    layout_ok = layout_fail_list = None
    if do_compile and gxx is not None:
        # DfmLayoutTypes.h is hand-authored schema (not per-form generated,
        # see its own banner) that every generated *_layout.gen.h #includes
        # by relative path -- copy it into the fresh regen root so the
        # compile smoke test can actually resolve it, exactly like a real
        # checkout of this tree already has it sitting next to the
        # generated files it ships alongside.
        shutil.copy2(os.path.join(LAYOUT_OUT_CANON, 'DfmLayoutTypes.h'),
                     os.path.join(layout_out, 'DfmLayoutTypes.h'))
        layout_ok = 0
        layout_fail_list = []
        for p in layout_cpp_paths:
            r_rc, r_txt = run_proc(
                [gxx, '-std=c++14', '-Wall', '-Wextra', '-I', layout_out, '-fsyntax-only', p],
                timeout=GXX_TIMEOUT_SEC)
            if r_rc != 0:
                layout_fail_list.append({'path': p, 'detail': r_txt[-2000:]})
            else:
                layout_ok += 1

    summary = {
        'files': len(files),
        'total_problems': total_problems,
        'total_control_nodes': total_nodes,
        'total_nonvisual_nodes': total_nonvisual,
        'total_dialogs': total_dialogs,
        'total_controls_rc': total_controls_rc,
        'total_layout_rows': total_layout_rows,
        'total_events': total_events,
        'g9_problem_count': g9_problems_total,
        'rc_census_problem_count': rc_census_problems_total,
        'synth_predicted_problem_count': synth_predicted_problems_total,
        'id_symbol_join_problem_count': id_symbol_join_problems_total,
        'rc_exe_fail_count': len(rc_fail),
        'rc_exe_fail_forms': rc_fail,
        'windres_fail_count': len(windres_fail),
        'windres_fail_forms': windres_fail,
        'layout_compile_ok_count': layout_ok,
        'layout_compile_fail_count': None if layout_fail_list is None else len(layout_fail_list),
        'layout_compile_failures_sample': None if layout_fail_list is None else layout_fail_list[:10],
    }
    return per_form, summary, {'ir_out': ir_out, 'rc_out': rc_out, 'layout_out': layout_out}


#  Hand-authored (not per-form generated) files that legitimately live
#  inside a canonical output tree alongside the generated ones. Excluded
#  from the G7 idempotency diff on purpose -- diffing them would either be
#  a permanent false G7_MISSING_IN_REGEN (regenerate_all() only calls each
#  generator's per-form emit function, never a "write the shared schema
#  header" step, by design -- see DfmLayoutTypes.h's own banner) or require
#  this driver to duplicate that one-time setup step for no gate value.
_STATIC_NONGENERATED_FILES = frozenset(['DfmLayoutTypes.h'])


def diff_tree(regen_dir, canon_dir, exts):
    """Byte-diff every file under regen_dir with the given extensions
    against its counterpart at the same relative path under canon_dir.
    Returns list of mismatch dicts (empty == perfect match)."""
    problems = []
    regen_files = set()
    for dp, dn, fn in os.walk(regen_dir):
        for f in fn:
            if f.endswith(exts) and f not in _STATIC_NONGENERATED_FILES:
                rel = os.path.relpath(os.path.join(dp, f), regen_dir)
                regen_files.add(rel)

    canon_files = set()
    for dp, dn, fn in os.walk(canon_dir):
        for f in fn:
            if f.endswith(exts) and f not in _STATIC_NONGENERATED_FILES:
                rel = os.path.relpath(os.path.join(dp, f), canon_dir)
                canon_files.add(rel)

    for rel in sorted(regen_files - canon_files):
        problems.append({'code': 'G7_EXTRA_IN_REGEN', 'path': rel})
    for rel in sorted(canon_files - regen_files):
        problems.append({'code': 'G7_MISSING_IN_REGEN', 'path': rel})
    for rel in sorted(regen_files & canon_files):
        a = os.path.join(regen_dir, rel)
        b = os.path.join(canon_dir, rel)
        if not filecmp.cmp(a, b, shallow=False):
            problems.append({'code': 'G7_BYTE_MISMATCH', 'path': rel})
    return problems


# ---------------------------------------------------------------------------
# Modes
# ---------------------------------------------------------------------------

def _clean_dir(path):
    """Best-effort replacement for the old bare
    `if os.path.isdir(path): shutil.rmtree(path)` (MEDIUM-4 fix).

    AI(W906-W7-B1-review) 20260728: independent review's plain `ctest` run
    hit dfm2rc_fidelity AND dfm2rc_idempotent both FAILING with an
    exception out of shutil.rmtree() here -- a leftover rc.exe-compiled
    .res scratch file from a previous interrupted/timed-out run (or a
    file transiently held open by an AV scanner right after rc.exe wrote
    it) made rmtree raise, which crashed the whole test rather than
    reporting a real gate failure. That is worse than a consistent
    failure: it trains people to just re-run ctest until it goes green.
    Fix: try the delete with ignore_errors=True (never raises); if
    anything survived (a genuinely locked file), don't fight it -- hand
    back a fresh, uniquely-suffixed sibling directory instead, so this
    run is never blocked by a previous run's leftovers. Combined with
    run_b1d.py's own per-form cleanup of res_path/windres_res_path right
    after G6 reads them (see regenerate_all's do_compile branch) and each
    mode now getting its own top-level regen root (see main()/CMakeLists,
    'dfm2rc_regen_fidelity' vs 'dfm2rc_regen_idempotent'), this closes the
    finding at all three of its stated angles."""
    if os.path.isdir(path):
        shutil.rmtree(path, ignore_errors=True)
    if not os.path.isdir(path):
        return path
    for suffix in range(1, 1000):
        alt = '%s_%d' % (path, suffix)
        if not os.path.isdir(alt):
            return alt
    raise RuntimeError('could not find a free regen directory near %r' % path)


def mode_fidelity(regen_root):
    rc_exe = find_rc_exe()
    windres = find_windres()
    gxx = find_gxx()
    if rc_exe is None:
        print('FATAL: rc.exe not found under Windows Kits', file=sys.stderr)
        return 3
    inc_dirs = find_sdk_include_dirs(rc_exe)
    env = dict(os.environ)
    env['MSYS_NO_PATHCONV'] = '1'
    env['MSYS2_ARG_CONV_EXCL'] = '*'

    out_root = _clean_dir(os.path.join(regen_root, 'fidelity'))
    os.makedirs(out_root, exist_ok=True)

    per_form, summary, paths = regenerate_all(out_root, rc_exe, windres, gxx, inc_dirs, env, do_compile=True)

    moji = []
    for p in paths.values():
        moji.extend(scan_mojibake_bom(p))
    summary['mojibake_bom_hits'] = moji

    fail_forms = [f['source_dfm'] for f in per_form if f['status'] == 'FAIL']
    summary['fail_form_count'] = len(fail_forms)
    summary['fail_forms'] = fail_forms

    _write_report('b1d_fidelity_report.json', summary, per_form)
    print(json.dumps(summary, ensure_ascii=False, indent=2))
    ok = (summary['total_problems'] == 0 and not moji
          and (summary['layout_compile_fail_count'] in (0, None)))
    return 0 if ok else 1


def mode_idempotent(regen_root):
    rc_exe = find_rc_exe()
    if rc_exe is None:
        print('FATAL: rc.exe not found under Windows Kits', file=sys.stderr)
        return 3
    inc_dirs = find_sdk_include_dirs(rc_exe)
    windres = find_windres()
    env = dict(os.environ)

    out_root = _clean_dir(os.path.join(regen_root, 'idempotent'))
    os.makedirs(out_root, exist_ok=True)

    # No need to compile .rc/.res here -- G7 only cares whether the
    # GENERATOR's own text output is byte-identical to what is checked in.
    # G5/G6 (compiling and reading it back) are separately covered by
    # `rc_compiles` and by `fidelity`.
    per_form, summary, paths = regenerate_all(out_root, rc_exe, windres, None, inc_dirs, env, do_compile=False)

    diffs = []
    diffs += [dict(d, tree='ir_out') for d in diff_tree(paths['ir_out'], IR_OUT_CANON, ('.json',))]
    diffs += [dict(d, tree='rc_out') for d in diff_tree(paths['rc_out'], RC_OUT_CANON, ('.rc', '.h', '.json'))]
    diffs += [dict(d, tree='layout_out') for d in diff_tree(paths['layout_out'], LAYOUT_OUT_CANON,
                                                             ('.h', '.cpp', '.json'))]

    summary2 = {
        'files': summary['files'],
        'regen_problem_count': summary['total_problems'],
        'g7_diff_count': len(diffs),
        'g7_diffs_sample': diffs[:50],
    }
    _write_report('b1d_idempotent_report.json', summary2, per_form)
    print(json.dumps(summary2, ensure_ascii=False, indent=2))
    return 0 if (len(diffs) == 0 and summary['total_problems'] == 0) else 1


def mode_rc_compiles():
    rc_exe = find_rc_exe()
    windres = find_windres()
    if rc_exe is None:
        print('FATAL: rc.exe not found under Windows Kits', file=sys.stderr)
        return 3
    inc_dirs = find_sdk_include_dirs(rc_exe)
    env = dict(os.environ)
    env['MSYS_NO_PATHCONV'] = '1'
    env['MSYS2_ARG_CONV_EXCL'] = '*'

    rc_files = glob.glob(os.path.join(RC_OUT_CANON, '**', '*.rc'), recursive=True)
    rc_files.sort()
    rc_fail = []
    windres_fail = []
    for rc_path in rc_files:
        d = os.path.dirname(rc_path)
        stem = os.path.splitext(os.path.basename(rc_path))[0]
        res_path = os.path.join(d, stem + '._b1d_check.res')
        rc_argv = [rc_exe, '/nologo']
        for inc in inc_dirs:
            rc_argv += ['/i', inc]
        rc_argv += ['/fo', res_path, rc_path]
        rc_rc, rc_txt = run_proc(rc_argv, cwd=d, env=env)
        if rc_rc != 0:
            rc_fail.append({'file': rc_path, 'detail': rc_txt[-1000:]})
        if windres is not None:
            wres_path = os.path.join(d, stem + '._b1d_check.windres.res')
            wr_argv = [windres, '--input-format=rc', '--output-format=res', '-o', wres_path, rc_path]
            wr_rc, wr_txt = run_proc(wr_argv, cwd=d, env=env)
            if wr_rc != 0:
                windres_fail.append({'file': rc_path, 'detail': wr_txt[-1000:]})
        for extra in (res_path, os.path.join(d, stem + '._b1d_check.windres.res')):
            if os.path.isfile(extra):
                os.remove(extra)

    summary = {
        'files': len(rc_files),
        'rc_exe_fail_count': len(rc_fail),
        'rc_exe_fail_sample': rc_fail[:10],
        'windres_available': windres is not None,
        'windres_fail_count': len(windres_fail),
        'windres_fail_sample': windres_fail[:10],
    }
    print(json.dumps(summary, ensure_ascii=False, indent=2))
    return 0 if (len(rc_fail) == 0 and len(windres_fail) == 0) else 1


def mode_layout_full():
    gxx = find_gxx()
    if gxx is None:
        print('FATAL: g++ not found on PATH', file=sys.stderr)
        return 3
    cpp_files = glob.glob(os.path.join(LAYOUT_OUT_CANON, '**', '*_layout.gen.cpp'), recursive=True)
    cpp_files.sort()
    fail = []
    ok = 0
    for p in cpp_files:
        r_rc, r_txt = run_proc(
            [gxx, '-std=c++14', '-Wall', '-Wextra', '-I', LAYOUT_OUT_CANON, '-fsyntax-only', p],
            timeout=GXX_TIMEOUT_SEC)
        if r_rc != 0:
            fail.append({'file': p, 'detail': r_txt[-1500:]})
        else:
            ok += 1
    summary = {'files': len(cpp_files), 'ok_count': ok, 'fail_count': len(fail), 'fail_sample': fail[:10]}
    print(json.dumps(summary, ensure_ascii=False, indent=2))
    return 0 if len(fail) == 0 else 1


def _write_report(name, summary, per_form):
    if not os.path.isdir(REPORTS_DIR):
        os.makedirs(REPORTS_DIR)
    with open(os.path.join(REPORTS_DIR, name), 'w', encoding='utf-8') as fh:
        json.dump({'summary': summary, 'per_form': per_form}, fh, ensure_ascii=False, indent=1, sort_keys=True)
        # AI(W906-Gate4) 20260802: json.dump leaves no trailing newline, which put every generated .json in violation of plan S12 gate 4; emit one so a fresh regen is gate-clean at birth (the checked-in corpora were swept to match in the same commit, keeping gate G7 byte-identical).
        fh.write('\n')


def main(argv):
    if len(argv) < 2:
        print(__doc__, file=sys.stderr)
        return 2
    mode = argv[1]
    if mode == 'fidelity':
        regen_root = argv[2] if len(argv) > 2 else os.path.join(HERE, '_b1d_regen')
        return mode_fidelity(regen_root)
    elif mode == 'idempotent':
        regen_root = argv[2] if len(argv) > 2 else os.path.join(HERE, '_b1d_regen')
        return mode_idempotent(regen_root)
    elif mode == 'rc_compiles':
        return mode_rc_compiles()
    elif mode == 'layout_full':
        return mode_layout_full()
    else:
        print('unknown mode %r (expected fidelity|idempotent|rc_compiles|layout_full)' % mode, file=sys.stderr)
        return 2


if __name__ == '__main__':
    sys.exit(main(sys.argv))
