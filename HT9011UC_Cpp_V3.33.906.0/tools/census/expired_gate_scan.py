#!/usr/bin/env python3
"""Find every `#if 0` gate whose named blocker now has a LIVE definition in the port.

WHY THIS EXISTS
A translation wave gates a call it cannot satisfy, stating the blocker in the gate comment
("X is declared but defined nowhere"). A LATER wave lands X. Nothing then re-opens the gate,
because the group that wrote it is long gone and the group that landed X never saw it. The
result is landed, faithful code that no call site reaches -- and, unlike a link error, an
expired gate is completely silent: the build is clean and the tests pass.

PT-W5c produced six of these in one wave (IsEMGPressed, IsIndexMotorOutOfPower,
LockIndexMotorAndDoHomeProcess, CountMotorPowerDelay, HeaterDoorIsOpen, RecordSafeDoorStates,
ArmCanSuck4IC -- all bodies landed by the wave, all consumers left gated), because each
translate group could only see its own slice and could not know a sibling would land its
dependency. That is structural to parallel wave translation, so it needs a mechanical check
rather than reviewer attention.

THIS IS A CANDIDATE FINDER, NOT A VERDICT. A gate can legitimately stay closed even when its
named symbol exists:
  * the gate may depend on SEVERAL symbols and only some have landed (reported as a ratio);
  * the body may be reachable but still wrong to run -- e.g. csystem.cpp's G31a/G31b, where
    DoMotorPowerOn exists but its own inner G9 brake-release gate is still closed, so opening
    it would apply motor power with brakes engaged;
  * the real definition may be a stub (see macro_seam_scan.py for that class).
So: treat every hit as "re-derive this gate's premise", never as "delete this gate".

USAGE
    python tools/census/expired_gate_scan.py            # summary
    python tools/census/expired_gate_scan.py --detail   # every candidate with its comment
"""
import io, os, re, sys

PORT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
SEP = chr(92)
DETAIL = '--detail' in sys.argv

DEFN = re.compile(r'^(?!\s)(?:[A-Za-z_~][\w:*&<>,\s]*?[\s*&:])?(~?[A-Za-z_]\w*)\s*\(')
CALL = re.compile(r'\b([A-Za-z_]\w{3,})\s*\(')
KW = {'if', 'for', 'while', 'switch', 'return', 'else', 'do', 'catch', 'sizeof', 'new',
      'ZeroMemory', 'memset', 'memcpy', 'delete', 'case', 'try', 'throw', 'and', 'or',
      'not', 'defined', 'assert', 'extern', 'typedef', 'using', 'sprintf', 'printf',
      'strcmp', 'strlen', 'atoi', 'atof', 'sizeof', 'static_cast', 'reinterpret_cast'}


def defn_probe(line):
    r"""Reduce a source line to just its would-be signature, for DEFN matching.

    DEFN ends in `\([^;]*$` so that a DECLARATION (which ends in ';') is not mistaken for a
    definition. Two things defeated that, both found 20260811 and both causing real definitions to
    be counted as MISSING:
      1. a trailing COMMENT containing ';' --
             bool bUseAxxGPicker()   // golden :8304 (REAL-defined here; stub removed)
      2. a ONE-LINE BODY, where the ';' is inside the braces --
             int CheckShuttleSensor_9045_2x5(int, bool, bool) { return 0; }
         This one hid 35 already-translated functions from a single wave's target list, and only
         surfaced as 35 redefinition errors after the stitch.
    So: drop the comment, then drop anything from the first '{' onwards. A real declaration still
    keeps its ';' and is still correctly rejected.
    """
    s = line.split('//')[0]
    if '{' in s:
        s = s.split('{')[0]
    return s.rstrip()


def load(p):
    b = io.open(p, 'rb').read()
    eol = '\r\n' if b.count(b'\r\n') else '\n'
    return io.open(p, encoding='utf-8', errors='replace', newline='').read().split(eol)


def gate_spans(L):
    """yield (start, end, comment) for each outermost `#if 0` block."""
    out, stack = [], []
    for i, l in enumerate(L):
        t = l.strip()
        if re.match(r'^#\s*if\b|^#\s*ifdef\b|^#\s*ifndef\b', t):
            stack.append((i, bool(re.match(r'^#\s*if\s+0\b', t))))
        elif t.startswith('#endif') and stack:
            s, zero = stack.pop()
            if zero and not any(z for _, z in stack):
                out.append((s, i, L[s].strip()))
    return out


srcs = []
for root, dirs, fs in os.walk(PORT):
    dirs[:] = [d for d in dirs if not d.startswith('build') and d not in ('.git', '.svn')]
    for f in fs:
        if f.endswith(('.cpp', '.cc')):
            srcs.append(os.path.relpath(os.path.join(root, f), PORT).replace(SEP, '/'))

# LIVE (non-gated, non-static) definitions across the port
live = {}
for p in srcs:
    L = load(os.path.join(PORT, p))
    gated = set()
    for s, e, _c in gate_spans(L):
        gated.update(range(s, e + 1))
    # tests/ definitions are TU-local stand-ins, never a reason to open a production gate
    if p.startswith('tests/'):
        continue
    for i, raw in enumerate(L):
        if i in gated or not raw or raw[0] in ' \t/#}':
            continue
        if re.match(r'^\s*static\b', raw):
            continue
        # A CLASS METHOD does not satisfy a gate: the gate's blocker is almost always the
        # missing OBJECT / facade member, not the method text. Counting `Class::Method` here
        # is what inflated the first run of this scan from a usable number to an unusable
        # one (it "satisfied" gates with TATCSystem::IsConnected, TDateTime::Time, and a
        # SaveEventLog defined inside tests/). Free functions only.
        if '::' in raw.split('(')[0]:
            continue
        m = DEFN.match(defn_probe(raw))
        if m and m.group(1) not in KW:
            live.setdefault(m.group(1), '%s:%d' % (p, i + 1))

cands, total_gates = [], 0
for p in srcs:
    L = load(os.path.join(PORT, p))
    for s, e, comment in gate_spans(L):
        total_gates += 1
        called = set()
        for j in range(s + 1, e):
            body = re.sub(r'//.*', '', L[j])
            for m in CALL.finditer(body):
                n = m.group(1)
                if n not in KW:
                    called.add(n)
        if not called:
            continue
        have = sorted(n for n in called if n in live and not live[n].startswith(p + ':'))
        havein = sorted(n for n in called if n in live and live[n].startswith(p + ':'))
        allhave = have + havein
        # A RETIREMENT WRAPPER is not an expired gate. When a stand-in is retired by wrapping
        # it in `#if 0`, its body naturally calls the very symbols whose real bodies replaced
        # it, so it always looks "fully satisfied". Those are the intended end state, not work.
        # (This filter exists because the first clean run was dominated by PT-W5c's own
        # csystem_predicates.cpp / acarry_shims.cpp retirement wrappers.)
        if re.search(r'RETIRED|SUPERSED', comment, re.I):
            continue
        if allhave:
            cands.append((p, s + 1, e + 1, len(called), allhave, comment))

print('gates scanned: %d   candidates with >=1 now-live callee: %d'
      % (total_gates, len(cands)))
full = [c for c in cands if len(c[4]) == c[3]]
print('  of those, EVERY callee is now live: %d  <- strongest candidates' % len(full))
print('\n(Candidate finder, not a verdict -- read this file\'s header before opening any gate.)\n')
for p, s, e, ncall, have, comment in sorted(full, key=lambda c: -len(c[4]))[:30]:
    print('%-34s gate %d-%d   %d/%d callees live' % ('%s:%d' % (p, s), s, e, len(have), ncall))
    print('    now live: %s' % ', '.join('%s (%s)' % (n, live[n]) for n in have[:4]))
    if DETAIL:
        print('    gate says: %s' % comment[:200])
