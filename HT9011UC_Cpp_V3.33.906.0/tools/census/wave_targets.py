#!/usr/bin/env python3
r"""Build a COMPLETE translation-wave target list for one golden source file.

WHY THIS EXISTS
PT-W7b (20260810) selected its 17 targets by FUNCTION span, because census.py counts functions.
Golden's FILE-SCOPE DECLARATIONS live in the gaps BETWEEN functions, so they belonged to no
agent. Eight of them were referenced by the translated bodies and declared nowhere in the port:

    int iHangupCTArm2=0;                          golden aTester_Rear.cpp:76
    bool bReadTorqueOK=false;                                        :78
    TQPF_Timer BTorqueTimeOutDelay;                                  :98
    HTimer DoTestYRearDelay, DoTestYRearDelay2;                      :4253
    TQPF_Timer iWaitIndexArm2;                                       :5358
    TQPF_Timer hTestZ1Delay;                                         :5359
    TQPF_Timer hFRTCTimeOutDelay;                                    :5360

The link failed, and it took two gate cycles to find them all. Any wave that picks targets from
census output alone has the same hole, so this tool reports BOTH halves of the work.

WHAT IT REPORTS
  1. FUNCTIONS  -- golden functions with no live same-name body in the port mirror.
  2. GLOBALS    -- golden file-scope object/variable definitions whose name appears NOWHERE in
                   the port tree (neither declaration nor definition).
For each global it also prints golden's TYPE, because the type is load-bearing: PT-W7b nearly
declared two timers as `HTimer`, whose only port implementation is atester_shims.h:463
    struct HTimer { bool Off(){ return true; } void SetSecAndOn(double){} };
-- Off() hard-coded true, i.e. every dwell expires instantly. That compiles and links CLEAN and
silently zeroes the delay. Golden's real HTimer is D:\HT9045\elec\Component\htimer.h, OUTSIDE the
version tree and never ported. Use TQPF_Timer (myTimer.h) for golden HTimer globals -- the idiom
already established at acatchtray.cpp:114, CanBus/cMyDNM100UD.cpp:85, MyPLC/MyPLC_IO_Modbus.cpp:49.
The tool flags any HTimer-typed global with exactly that warning.

DELIBERATE LIMITS
  * "name appears nowhere in the port tree" is a TEXT search, so a name that appears only inside
    a comment counts as present. That direction is safe (it under-reports, never invents work),
    but it means a clean report is not proof -- the link is still the oracle.
  * Absence claims EXPIRE. A sibling wave landing a file makes them false. Re-run at integration.
  * Overloads collapse by name, same as census.py.

USAGE
    python tools/census/wave_targets.py aTester_Front.cpp
    python tools/census/wave_targets.py Motor/mymotor.cpp --globals-only
"""
import io, os, re, sys

GOLD = os.environ.get('HT9045_GOLDEN', 'D:/HT9045/HT9011UC_Code_V3.33.906.0_20260618')
PORT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

DEFN = re.compile(r'^(?!\s)(?:[A-Za-z_][\w:*&<>,\s]*?\s+)??'
                  r'(?:([A-Za-z_]\w*)::)?(~?[A-Za-z_]\w*)\s*\([^;]*$')
KW = {'if', 'for', 'while', 'switch', 'return', 'else', 'do', 'catch', 'sizeof', 'new',
      'ZeroMemory', 'memset', 'memcpy', 'delete', 'case', 'try', 'throw', 'and', 'or',
      'not', 'defined', 'assert', 'typedef', 'using', 'namespace', 'template', 'class',
      'struct', 'enum', 'union', 'extern', 'static_assert'}

# a file-scope object definition:  <type> name [= init] ;   /  <type> name[dim] ... ;
GLOBAL = re.compile(r'^([A-Za-z_][\w:<>,\s\*&]*?)\s+'      # type
                    r'([A-Za-z_]\w*)\s*'                    # first name
                    r'((?:\[[^\]]*\])*)\s*'                 # optional array dims
                    r'(?:=[^;]*)?'                          # optional initialiser
                    r'((?:\s*,\s*[A-Za-z_]\w*(?:\s*\[[^\]]*\])*(?:=[^;,]*)?)*)'   # more names
                    r'\s*;')
TYPE_KW = {'return', 'if', 'else', 'for', 'while', 'switch', 'case', 'typedef', 'using',
           'extern', 'friend', 'template', 'namespace', 'public', 'private', 'protected'}


def read(path, golden):
    raw = io.open(path, 'rb').read()
    eol = '\r\n' if raw.count(b'\r\n') else '\n'
    enc = 'cp950' if golden else 'utf-8'
    return raw.decode(enc, 'replace').split(eol)


def gatemap(L):
    st, g = [], []
    for l in L:
        t = l.strip()
        if re.match(r'^#\s*if\b|^#\s*ifdef\b|^#\s*ifndef\b', t):
            st.append(bool(re.match(r'^#\s*if\s+0\b', t)) or bool(st[-1] if st else False))
        elif t.startswith('#endif') and st:
            st.pop()
        g.append(bool(st[-1]) if st else False)
    return g


def spans(L):
    """Return (funcs, func_line_set): name -> (line, span, gated), plus lines inside functions."""
    g = gatemap(L)
    out, inside, i, n = {}, set(), 0, len(L)
    while i < n:
        l = L[i]
        if not l or l[0] in ' \t/#}':
            i += 1
            continue
        m = DEFN.match(l.split('//')[0].rstrip())   # see census.py: a ';' in a trailing comment hid real definitions
        if not m or m.group(2) in KW or (m.group(1) or '') in KW:
            i += 1
            continue
        depth, j, seen = 0, i, False
        while j < n:
            depth += L[j].count('{') - L[j].count('}')
            if '{' in L[j]:
                seen = True
            if seen and depth <= 0:
                break
            j += 1
            if j - i > 4000:
                break
        if not seen:
            i += 1
            continue
        key = m.group(2)
        prev = out.get(key)
        if prev is None or (prev[2] and not g[i]) or (not g[i] and (j - i + 1) > prev[1]):
            out[key] = (i + 1, j - i + 1, g[i])
        inside.update(range(i, j + 1))
        i = j + 1
    return out, inside


def globals_of(L, inside):
    g = gatemap(L)
    found = []
    for i, l in enumerate(L):
        if i in inside or not l or l[0] in ' \t/#}':
            continue
        s = l.split('//')[0].rstrip()
        if not s.endswith(';') or '(' in s:
            continue
        m = GLOBAL.match(s)
        if not m:
            continue
        typ = m.group(1).strip()
        if typ.split()[0] in TYPE_KW or typ.startswith('extern'):
            continue
        names = [m.group(2)] + re.findall(r'([A-Za-z_]\w*)', m.group(4) or '')
        found.append((i + 1, typ, names, g[i]))
    return found


def port_has(name, cache={}):
    """Does this identifier appear ANYWHERE in the port tree (.cpp/.h)?"""
    if not cache:
        blob = []
        for dp, dn, fn in os.walk(PORT):
            dn[:] = [d for d in dn
                     if d.lower() not in ('.svn', '.git') and not d.startswith('build')
                     and d not in ('_w7b_parts',)]
            for f in fn:
                if f.lower().endswith(('.cpp', '.h', '.hpp')):
                    try:
                        blob.append(io.open(os.path.join(dp, f), encoding='utf-8',
                                            errors='replace').read())
                    except Exception:
                        pass
        cache['blob'] = '\n'.join(blob)
    return re.search(r'\b' + re.escape(name) + r'\b', cache['blob']) is not None


def main():
    args = [a for a in sys.argv[1:] if not a.startswith('--')]
    if not args:
        print(__doc__)
        return 2
    rel = args[0].replace('\\', '/')
    gf, pf = os.path.join(GOLD, rel), os.path.join(PORT, rel)
    if not os.path.exists(gf):
        print('no such golden file: %s' % gf)
        return 1
    GL = read(gf, True)
    G, ginside = spans(GL)
    if os.path.exists(pf):
        P, _ = spans(read(pf, False))
    else:
        P = {}
        print('NOTE: port has no mirror for %s -- every function is missing.' % rel)

    print('=== %s ===' % rel)
    print('golden %d lines; golden functions %d; port has %d' % (len(GL), len(G), len(P)))

    if '--globals-only' not in sys.argv:
        miss = sorted(((n, l, s) for n, (l, s, _g) in G.items() if n not in P),
                      key=lambda x: -x[2])
        print('\n--- FUNCTIONS to translate: %d (%d golden lines) ---'
              % (len(miss), sum(s for _n, _l, s in miss)))
        print('%-46s %8s %7s' % ('function', 'goldLn', 'span'))
        for n, l, s in miss:
            print('%-46s %8d %7d' % (n, l, s))

    gl = globals_of(GL, ginside)
    absent = []
    for line, typ, names, gated in gl:
        missing_names = [n for n in names if not port_has(n)]
        if missing_names:
            absent.append((line, typ, missing_names, gated))
    print('\n--- FILE-SCOPE GLOBALS golden declares that the PORT TREE lacks: %d ---'
          % len(absent))
    print('(this is the half PT-W7b missed. Text search, so it under-reports; the link is still'
          ' the oracle.)')
    print('%-8s %-34s %s' % ('goldLn', 'golden type', 'name(s)'))
    for line, typ, names, gated in absent:
        warn = ''
        if re.search(r'\bHTimer\b', typ):
            warn = '   <-- USE TQPF_Timer, NOT atester_shims.h:463 HTimer (its Off() is always true)'
        print('%-8d %-34s %s%s%s'
              % (line, typ, ', '.join(names), '  [GATED in golden]' if gated else '', warn))
    if not absent:
        print('  (none -- but re-run at integration: absence claims expire when siblings land)')
    return 0


if __name__ == '__main__':
    sys.exit(main())
