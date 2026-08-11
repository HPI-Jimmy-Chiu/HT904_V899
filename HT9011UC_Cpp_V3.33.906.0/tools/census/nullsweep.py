"""Which globals does golden's main.cpp allocate that the port never allocates?

WHY THIS EXISTS
PT_CAMPAIGN_PLAN section 8 documents 18 global pointers that are NULL in the port
because golden constructs them in main.cpp -- a unit with no port at all. Its own
warning is the load-bearing part:

    "在 GA-3 落地之前,任何一波只要讓上表其中一個全域第一次有呼叫者,就會 segfault
     -- 這不是回歸,是本來就在那裡。波次驗證看到新的 SEGFAULT 時,第一個要查的就是這張表。"

That has now come true twice. The second time (PT-W8, 20260811) the global was NOT
in the table: golden allocates it as

    ArmData[i] = new TArm("Arm"+AnsiString(i));          // main.cpp:2143

an ARRAY-ELEMENT assignment, and the 20260807 sweep only matched the scalar form
`X = new T;`. golden main.cpp has 67 scalar sites and 17 array-element sites, so
that blind spot covered a fifth of the surface -- including ArmData / ArmDataLot /
ArmHistory / ArmData_AutoClean, which cost a gate cycle and a SEGFAULT to find.

The original sweep script was never committed (it lived in a scratchpad and went
away with the session, same as the first census scripts). This one is in the repo
so the table can actually be re-derived instead of trusted.

WHAT IT REPORTS
For every global golden's main.cpp allocates, whether the PORT allocates it too.
A global the port defines but never allocates is a NULL dereference waiting for
its first caller.

DELIBERATE LIMITS
  * Allocation is detected textually. A port that allocates through a helper, or
    inside a #if 0, may be misread -- ALWAYS open the port site before concluding.
  * "the port defines it" is matched by name, so a same-named local shadows.
  * Absence claims expire; re-run at integration, like every other tool here.

USAGE
    python tools/census/nullsweep.py            # table of still-NULL globals
    python tools/census/nullsweep.py --all      # every main.cpp allocation site
"""
import io, os, re, sys

GOLD = os.environ.get('HT9045_GOLDEN', 'D:/HT9045/HT9011UC_Code_V3.33.906.0_20260618')
PORT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
SKIP_DIR = ('build', '.git', '.svn', 'vendor', 'third_party', 'scratchpad', 'tests')

# Both shapes. The array one is the whole point of this rewrite.
SCALAR = re.compile(r'^([A-Za-z_]\w*)\s*=\s*new\s+([A-Za-z_]\w*)')
ARRAY = re.compile(r'^([A-Za-z_]\w*)\s*\[[^\]]*\]\s*=\s*new\s+([A-Za-z_]\w*)')


def gatemap(lines):
    out, stack = [], []
    for l in lines:
        t = l.strip()
        if re.match(r'#\s*if\s+0\b', t):
            stack.append(True)
        elif re.match(r'#\s*if', t):
            stack.append(False)
        elif re.match(r'#\s*endif', t) and stack:
            stack.pop()
        out.append(bool(stack))
    return out


def golden_allocations():
    """name -> (golden main.cpp line, type, 'scalar'|'array')"""
    path = os.path.join(GOLD, 'main.cpp')
    lines = io.open(path, encoding='cp950', errors='replace').read().split('\n')
    g = gatemap(lines)
    out = {}
    for i, l in enumerate(lines):
        if g[i]:
            continue
        t = re.sub(r'//.*', '', l).strip()
        m = ARRAY.match(t)
        kind = 'array'
        if not m:
            m = SCALAR.match(t)
            kind = 'scalar'
        if not m:
            continue
        out.setdefault(m.group(1), (i + 1, m.group(2), kind))
    return out


def port_files():
    for root, dirs, fs in os.walk(PORT):
        dirs[:] = [d for d in dirs
                   if not any(s in d for s in SKIP_DIR)
                   and not d.startswith('_') and not d.startswith('build')]
        for f in fs:
            if f.endswith(('.cpp', '.h')):
                yield os.path.join(root, f)


def port_state(names):
    """name -> (defined_at or None, allocated_at or None)"""
    state = dict((n, [None, None]) for n in names)
    alloc_pat = dict(
        (n, re.compile(r'\b' + re.escape(n) + r'\s*(\[[^\]]*\])?\s*=\s*new\b'))
        for n in names)
    # A definition is a file-scope line naming the global that is NOT an
    # `extern` declaration and NOT an assignment.
    for p in port_files():
        try:
            lines = io.open(p, encoding='utf-8', errors='replace').read().split('\n')
        except Exception:
            continue
        g = gatemap(lines)
        rel = os.path.relpath(p, PORT).replace(os.sep, '/')
        for i, l in enumerate(lines):
            if g[i] or not l.strip() or l.lstrip().startswith('//'):
                continue
            for n in names:
                if n not in l:
                    continue
                if state[n][1] is None and alloc_pat[n].search(l):
                    state[n][1] = '%s:%d' % (rel, i + 1)
                if (state[n][0] is None and p.endswith('.cpp') and l[0] not in ' \t#'
                        and not l.lstrip().startswith('extern')
                        and re.search(r'\b' + re.escape(n) + r'\s*(\[[^\]]*\])?\s*;', l)):
                    state[n][0] = '%s:%d' % (rel, i + 1)
    return state


def main(show_all):
    ga = golden_allocations()
    st = port_state(list(ga))
    rows = []
    for n, (ln, typ, kind) in sorted(ga.items(), key=lambda kv: kv[1][0]):
        defined, allocated = st[n]
        if show_all or (defined and not allocated):
            rows.append((n, typ, ln, kind, defined, allocated))

    scal = sum(1 for v in ga.values() if v[2] == 'scalar')
    arr = sum(1 for v in ga.values() if v[2] == 'array')
    print('golden main.cpp allocation sites: %d scalar + %d array-element = %d'
          % (scal, arr, scal + arr))
    print('(the array-element shape is invisible to a `^X = new T` pattern -- that'
          ' blind spot is why section 8\'s table was incomplete)')
    print('')
    hdr = 'EVERY main.cpp ALLOCATION' if show_all else \
          'DEFINED IN THE PORT BUT NEVER ALLOCATED THERE -- NULL, waiting for a first caller'
    print('--- %s: %d ---' % (hdr, len(rows)))
    print('%-26s %-22s %9s %-7s %s' % ('global', 'type', 'main.cpp', 'shape', 'port definition'))
    for n, typ, ln, kind, defined, allocated in rows:
        note = defined or '(no port definition)'
        if show_all and allocated:
            note += '   ALLOCATED at ' + allocated
        print('%-26s %-22s %9d %-7s %s' % (n, typ, ln, kind, note))
    print('')
    print('Textual detection: open the port site before acting. Absence claims expire.')
    return 0


if __name__ == '__main__':
    sys.exit(main('--all' in sys.argv))
