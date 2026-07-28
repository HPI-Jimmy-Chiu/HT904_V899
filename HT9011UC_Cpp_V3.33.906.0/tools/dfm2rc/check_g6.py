# -*- coding: utf-8 -*-
"""
check_g6.py -- W7-B1b: gate G6, ".res 二進位回讀" (binary read-back diff).

AI(W906-W7-B1b) 20260728: new file (plan doc:
HT9011UC_Cpp_V3.33.906.0/docs/W7_UI_ARCHITECTURE_PLAN.md, S6-W7-B1b: "G6
.res 二進位回讀（用純 Python 解 RES container + DLGTEMPLATEEX +
DLGITEMTEMPLATEEX，逐項 diff 回 IR：template 數、每個 ID 恰好出現一次、
class 字串、caption 逐字）"). This is the strongest gate available without
MFC, and per the plan it caught a real self-authored bug in the prototype
(DUP_ID_IN_RES from placeholder-Static containers).

INDEPENDENCE, and how it is achieved here (documented plainly rather than
asserted): this checker does NOT import emit_rc.py and does NOT re-run any
part of its dialog-owner walk or ID-assignment algorithm. What it *does*
share with the generator is classmap.py's declarative class table (a data
fact, not logic -- the same reuse gates.py itself sanctions for B1a's
SUBDLG/NONVISUAL class sets) and the emitter's `<Form>.rcmeta.json`
sidecar (which is regenerated fresh from the IR by emit_rc.py on every
run, not a hand-maintained fixture). What is independently verified here
is the thing the plan actually cares about catching: whether what rc.exe
*actually put in the compiled binary* matches what the emitter *intended*
to emit -- i.e. this gate is the oracle for the .rc-text -> .res-binary
step, the step where the corpus's own prototype previously introduced a
real bug that no amount of re-reading the .rc *text* would have caught
(rc.exe silently accepted duplicate IDs; only reading the compiled
template's item list back out revealed it). Three checks constitute this:
  1. Every dialog named in the sidecar appears in the .res exactly once
     (dialog-count / template-count fidelity).
  2. Within each dialog, every control ID is unique (this is the exact
     shape of the historical DUP_ID_IN_RES bug) and every sidecar-listed
     ID actually appears (no control silently dropped by the compiler).
  3. Each control's compiled `window_class` (resolved through
     PREDEFINED_CLASS_ORDINALS where applicable) and compiled `title`
     (decoded straight out of the binary by res_parse.py, not re-derived
     from the .dfm) match the sidecar's expectation exactly.
Additionally (not required by the plan's G6 wording, but free once the
comparison machinery exists and directly strengthens D7's "exact-copy"
geometry claim, see emit_rc.py docstring point 2): every control's
compiled (x, y, cx, cy) is compared too.

AI(W906-W7-B1-review) 20260728: MEDIUM-1 fix. The paragraph above already
described this geometry round-trip check, but it did not exist -- neither
build_meta()'s sidecar output nor this file's comparison loop carried
x/y/cx/cy at all, so the "additionally" sentence was aspirational, not
true. emit_rc.py's build_meta() now records each control's compiled-target
(x, y, cx, cy) in the sidecar; check_g6.py's check_one_form() below now
diffs it against the real compiled DLGITEMTEMPLATEEX fields (already
decoded onto res_parse.py's DlgItem -- .x/.y/.cx/.cy -- so this needed only
a few lines, not new decoding). Verified two ways: (a) `python run_b1d.py
fidelity <dir>` -- recompiles all 133 forms fresh and runs this exact
check -- reports 0 GEOMETRY_MISMATCH problems (see tools/dfm2rc/reports/
b1d_fidelity_report.json, refreshed by this review pass); (b) the check is
proven load-bearing, not merely quiet, by feeding _geometry_mismatches() a
deliberately wrong compiled coordinate in isolation and confirming it is
reported.
"""
from __future__ import print_function

import os
import sys
import json
import glob

import classmap as CM
import res_parse as RP


def _class_matches(expected_win32_class, compiled_class):
    """compiled_class is res_parse's decoded ('ORD', n) or ('STR', s) or
    None. expected_win32_class is the literal string emit_rc.py wrote into
    the CONTROL statement (e.g. "Static", "SysTabControl32",
    "HT9045.MyLedLane")."""
    if compiled_class is None:
        return False
    kind, val = compiled_class
    if kind == 'STR':
        return val == expected_win32_class
    if kind == 'ORD':
        return CM.PREDEFINED_CLASS_ORDINALS.get(val) == expected_win32_class
    return False


def _title_matches(expected_caption, compiled_title):
    if expected_caption is None:
        return compiled_title is None
    if compiled_title is None or compiled_title[0] != 'STR':
        return False
    return compiled_title[1] == expected_caption


def _geometry_mismatches(expected_control, compiled_item):
    """(field, expected, compiled) tuples for every one of x/y/cx/cy that
    differs -- empty list on an exact match. `expected_control` is a
    .rcmeta.json control dict (has 'x'/'y'/'cx'/'cy', added by emit_rc.py's
    build_meta -- MEDIUM-1 fix); `compiled_item` is a res_parse.DlgItem
    (already-decoded .x/.y/.cx/.cy from the real compiled
    DLGITEMTEMPLATEEX, SHORT-typed per res_parse.py's format notes)."""
    out = []
    for key in ('x', 'y', 'cx', 'cy'):
        exp = expected_control.get(key)
        got = getattr(compiled_item, key)
        if exp != got:
            out.append((key, exp, got))
    return out


def check_one_form(meta_path, res_path):
    """Returns (problems list, stats dict). problems is empty on success."""
    problems = []
    meta = json.load(open(meta_path, 'r', encoding='utf-8'))
    compiled_dialogs = RP.dialogs_in_res(res_path)

    expected_dialog_names = [d['idd_symbol'] for d in meta['dialogs']]

    if len(compiled_dialogs) != len(expected_dialog_names):
        problems.append({
            'gate': 'G6', 'code': 'TEMPLATE_COUNT_MISMATCH',
            'detail': 'expected=%d compiled=%d' % (len(expected_dialog_names), len(compiled_dialogs)),
        })

    total_controls_checked = 0
    for d in meta['dialogs']:
        name = d['idd_symbol']
        # RT_DIALOG resource Name: since the .rc #includes the generated
        # _ids.h, IDD_* symbols are macro-expanded to their numeric literal
        # *before* rc.exe ever sees them, so the compiled Name is the
        # numeric ordinal ('ORD', value) -- verified empirically (see
        # emit_rc.py/check_g6.py smoke test). Fall back to a bare string
        # key for robustness if some future variant leaves it undefined.
        tmpl = compiled_dialogs.get(('ORD', d['idd_value']))
        if tmpl is None:
            tmpl = compiled_dialogs.get(name)
        if tmpl is None:
            problems.append({'gate': 'G6', 'code': 'DIALOG_MISSING_IN_RES', 'detail': name})
            continue

        by_id = {}
        dup_ids = set()
        for it in tmpl.items:
            if it.id in by_id:
                dup_ids.add(it.id)
            by_id.setdefault(it.id, []).append(it)
        for did in sorted(dup_ids):
            problems.append({
                'gate': 'G6', 'code': 'DUP_ID_IN_RES', 'detail': '%s id=%d (%d occurrences)' % (
                    name, did, len(by_id[did])),
            })

        expected_ids = set(c['value'] for c in d['controls'])
        compiled_ids = set(by_id.keys())
        missing = expected_ids - compiled_ids
        extra = compiled_ids - expected_ids
        for mid in sorted(missing):
            problems.append({'gate': 'G6', 'code': 'CONTROL_MISSING_IN_RES', 'detail': '%s id=%d' % (name, mid)})
        for eid in sorted(extra):
            problems.append({'gate': 'G6', 'code': 'CONTROL_UNEXPECTED_IN_RES', 'detail': '%s id=%d' % (name, eid)})

        for c in d['controls']:
            total_controls_checked += 1
            items = by_id.get(c['value'])
            if not items:
                continue  # already reported as CONTROL_MISSING_IN_RES
            it = items[0]
            if not _class_matches(c['win32_class'], it.window_class):
                problems.append({
                    'gate': 'G6', 'code': 'CLASS_MISMATCH',
                    'detail': '%s id=%d expected=%r compiled=%r' % (
                        name, c['value'], c['win32_class'], it.window_class),
                })
            if not _title_matches(c['caption'], it.title):
                problems.append({
                    'gate': 'G6', 'code': 'CAPTION_MISMATCH',
                    'detail': '%s id=%d expected=%r compiled=%r' % (
                        name, c['value'], c['caption'], it.title),
                })
            for field, exp, got in _geometry_mismatches(c, it):
                problems.append({
                    'gate': 'G6', 'code': 'GEOMETRY_MISMATCH:%s' % field,
                    'detail': '%s id=%d field=%s expected=%r compiled=%r' % (
                        name, c['value'], field, exp, got),
                })

    return problems, {
        'source_dfm': meta['source_dfm'],
        'dialog_count': len(meta['dialogs']),
        'control_count': total_controls_checked,
    }


def main(argv):
    if len(argv) != 3:
        print('usage: check_g6.py <form.rcmeta.json> <form.res>', file=sys.stderr)
        return 2
    problems, stats = check_one_form(argv[1], argv[2])
    print(json.dumps({'stats': stats, 'problems': problems}, indent=2))
    return 0 if not problems else 1


if __name__ == '__main__':
    sys.exit(main(sys.argv))
