# -*- coding: utf-8 -*-
"""Rank golden files as wave targets by whether a delivery could ever be REACHED.

Why this exists
===============
Three consecutive waves (IOSV1, HS1, ATC1) delivered code with zero callers, and
that was structural, not incidental. Each of those golden files' globals is
already owned by a shim, so the facade may not declare one, so nothing can ever
obtain an instance -- and, worse, the `#if 0`-gated call sites naming those
members can never be satisfied by our facade either: un-gating binds to the shim,
which does not carry the members. HS had 16 such names and ATC 37, all
unreachable until the shim is retired, which is a queued behaviour change.

So the useful question when picking a target is not "how many bodies" but:

    Is the global name FREE, and are there gated call sites waiting for it?

Both true  -> the facade can declare the global, and un-gating those sites later
              binds to OUR class. That is FW3-MES1's shape.
Global taken -> the delivery is inert today AND the reserved names stay
              unreachable until someone retires the shim. Deprioritise.
No gated sites -> nobody is waiting at all; translation coverage only.

wave_preflight.py answers this for ONE file and rescans the whole port tree each
time. This does one pass for every candidate, so target selection stops being
guesswork.

Usage
=====
    python tools/census/wave_candidates.py                 # all golden .cpp with
                                                           # a sibling .h global
    python tools/census/wave_candidates.py --min-gated 1   # only ones with waiters
    python tools/census/wave_candidates.py --out ranked.txt

Output is ASCII-only on purpose: printing a non-cp950 character kills the script
before it writes, which looks exactly like an analysis failure (RESUME #9).

Caveats, stated rather than hidden:
  - "Global free" means no declaration/definition of that identifier was found in
    the port outside comments. It does NOT prove a future declaration is safe.
  - Gated-site counts come from census's gate_depth_map, which models `#if 0` and
    does NOT evaluate macros -- see KNOWLEDGE #22. Sites under `#ifdef <undefined>`
    are reported as live here; wave_preflight's per-file report separates them.
  - Every absence is true as of this run only (trap #2).
"""
import io
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, '..', '..'))
GOLDEN = os.path.abspath(os.path.join(ROOT, '..', 'HT9011UC_Code_V3.33.906.0_20260618'))

SKIP_DIRS = ('build', '_', '.git', '.vs', 'node_modules', 'third_party', 'vendor')
EXTERN_RE = re.compile(r'^\s*extern\s+(?:PACKAGE\s+)?(\w+)\s*\*\s*(\w+)\s*;', re.M)


def _prefix_exec(fname):
    path = os.path.join(HERE, fname)
    src = io.open(path, encoding='utf-8').read()
    ns = {'__name__': fname[:-3] + '_helpers', '__file__': path}
    exec(compile(src[:src.index('\ndef main(')], fname + '_prefix', 'exec'), ns)
    return ns


def main():
    min_gated = 0
    outpath = None
    if '--min-gated' in sys.argv:
        min_gated = int(sys.argv[sys.argv.index('--min-gated') + 1])
    if '--out' in sys.argv:
        outpath = sys.argv[sys.argv.index('--out') + 1]

    C = _prefix_exec('census.py')
    S = _prefix_exec('span_sanity.py')
    gate_depth_map, strip_comments = C['gate_depth_map'], S['strip_comments']

    # --- candidates: golden .cpp whose sibling .h declares `extern T *g;` ----
    cands = {}          # global -> (rel_cpp, class)
    for dp, dns, fns in os.walk(GOLDEN):
        dns[:] = [d for d in dns if not d.lower().startswith(('.svn', '.git'))]
        for fn in fns:
            if not fn.endswith('.h'):
                continue
            hp = os.path.join(dp, fn)
            cp = hp[:-2] + '.cpp'
            if not os.path.exists(cp):
                continue
            try:
                t = io.open(hp, encoding='cp950', errors='replace').read()
            except Exception:
                continue
            m = EXTERN_RE.search(t)
            if m:
                rel = os.path.relpath(cp, GOLDEN).replace(os.sep, '/')
                cands[m.group(2)] = (rel, m.group(1))
    if not cands:
        print('no candidates found')
        return 2

    globs = set(cands)
    site_re = re.compile(r'\b(%s)\s*->' % '|'.join(re.escape(g) for g in sorted(globs)))
    decl_re = re.compile(r'\b(?:extern\s+)?\w+\s*\*\s*(%s)\s*[;=]'
                         % '|'.join(re.escape(g) for g in sorted(globs)))

    live = dict((g, 0) for g in globs)
    gated = dict((g, 0) for g in globs)
    owner = dict((g, []) for g in globs)     # port files declaring the global

    # --- ONE pass over the port tree ---------------------------------------
    nfiles = 0
    for dp, dns, fns in os.walk(ROOT):
        dns[:] = [d for d in dns if not d.lower().startswith(SKIP_DIRS)]
        for fn in fns:
            if not fn.endswith(('.cpp', '.h', '.hpp', '.c')):
                continue
            p = os.path.join(dp, fn)
            try:
                raw = io.open(p, encoding='utf-8', errors='replace').read()
            except Exception:
                continue
            nfiles += 1
            if not any(g in raw for g in globs):
                continue
            rel = os.path.relpath(p, ROOT).replace(os.sep, '/')
            s = strip_comments(raw)
            for m in decl_re.finditer(s):
                owner[m.group(1)].append(rel)
            if '->' not in s:
                continue
            lines = s.splitlines()
            g_map = gate_depth_map(lines)
            for i, ln in enumerate(lines):
                for m in site_re.finditer(ln):
                    (gated if g_map[i] else live)[m.group(1)] += 1

    rows = []
    for g, (rel, cls) in cands.items():
        own = sorted(set(owner[g]))
        # a declaration inside forms/ that we wrote is ours, not an occupation
        ours = [o for o in own if o.startswith('forms/')]
        free = not own
        rows.append((gated[g], live[g], free, bool(ours), g, cls, rel, own))

    rows.sort(key=lambda r: (-r[0], -r[1], r[6]))

    L = []
    def w(x=''):
        L.append(x)
    w('=' * 96)
    w('WAVE CANDIDATES -- ranked by "could a delivery ever be reached"')
    w('scanned %d port files, %d golden candidates' % (nfiles, len(cands)))
    w('=' * 96)
    w()
    w('%-34s %-26s %6s %5s  %s' % ('golden .cpp', 'global', 'gated', 'live', 'global owner'))
    w('-' * 96)
    for gt, lv, free, ours, g, cls, rel, own in rows:
        if gt < min_gated:
            continue
        if free:
            st = 'FREE  <-- facade may declare it'
        elif ours:
            st = 'ours: ' + ', '.join(own[:2])
        else:
            st = 'TAKEN by ' + ', '.join(own[:2])
        w('%-34s %-26s %6d %5d  %s' % (rel[:34], g[:26], gt, lv, st))
    w()
    w('READ THIS AS:')
    w('  gated>0 AND global FREE  -> best: un-gating later binds to your class')
    w('  gated>0 AND global TAKEN -> those reserved names are UNREACHABLE until')
    w('                              the shim is retired (a queued behaviour change)')
    w('  gated=0                  -> nobody is waiting; translation coverage only')
    w()
    w('Caveats: gate depth models #if 0 and does NOT evaluate macros (KNOWLEDGE')
    w('#22); "FREE" is an absence claim and expires (trap #2). Re-run before use.')

    body = '\n'.join(L)
    if outpath:
        io.open(outpath, 'w', encoding='utf-8', newline='\n').write(body + '\n')
        print('written: %s (%d lines)' % (outpath, len(L)))
    else:
        print(body)
    return 0


if __name__ == '__main__':
    sys.exit(main())
