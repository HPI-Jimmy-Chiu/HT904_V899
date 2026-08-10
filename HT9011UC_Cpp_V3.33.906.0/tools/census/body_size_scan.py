#!/usr/bin/env python3
r"""Find port functions that EXIST but are far SHORTER than their golden counterpart.

WHY THIS EXISTS
census.py counts a golden function as translated when a port function of the same name
exists and is not wholly `#if 0`. It never compares SIZE. So a body that is present but
ABBREVIATED counts as fully done. Found 20260810: the port's DoAllProcess -- the machine's
top-level process dispatcher -- is 239 lines standing in for golden's 1,141, because its
whole early-return ladder (golden :9165-10047, 883 lines) sits inside ONE `#if 0` whose
contents are a 12-line SUMMARY, not golden text:
    if(LastSet.iRunStartMode==rsmAutoRetest || bART_needRT2) { flag=DoAutoRetest(); /* ... ~300 lines ... */ return; }
census reported that function as DONE, and the expired-gate scanner separately flagged the
same gate as "10/10 callees live". Neither could see that 77% of the body was missing.

WHAT THIS MEASURES
For every golden function with a live same-name port body, the ratio port_span/golden_span.
A low ratio means the port body is a summary, a partial translation, or a stub wearing the
real name -- all of which census currently scores as complete.

FALSE POSITIVES MEASURED 20260810 (PT-W6c), and both were caught by an agent refusing to act
  Of 28 targets this scan produced, 2 were WRONG -- golden's text was already in the tree:
  * `THGem::DataItemOut` -- OVERLOAD COLLAPSE, inherited from census.py. Golden has TWO
    overloads: `(int, unsigned char, char*)` at 198 lines and `(unsigned char, AnsiString)` at
    6. This scan keys by NAME, so it compared the 198-line overload's span against the port's
    4-line forwarder for the OTHER overload. The real body is
    SECSGEM/SecsWireCodec.cpp:713 with const-correctness adaptations.
  * `THGem::SendLocalData` -- EXTRACT-AND-FORWARD with a SUFFIXED name. Golden's 104 lines live
    in `THGem::SendLocalDataFrom(SecsWireCodec&)`, with a 4-line forwarder left at the golden
    name. census.py's alias rule covers `<Prefix>_<Name>` and `<Class>_<Name>` but NOT a
    suffix like `...From`, so neither tool sees the relocation.
  WHY THIS MATTERS MORE THAN THE COUNT: acting on either would have put a SECOND copy of
  already-translated golden text in the tree, which is a double-apply hazard at un-gate time.
  So a hit here is a QUESTION ("is golden's text really absent?"), never an instruction.
  Before restoring any body, grep golden's distinctive statements across the WHOLE port -- not
  just the same file, and not just the same name.

DELIBERATE LIMITS, so the output is not over-read
  * Span is a proxy for content. A legitimately shorter body exists: gate notes REMOVED are
    rare but comments STRIPPED or a golden block correctly relocated elsewhere both shrink a
    body without losing behaviour. Conversely port bodies are often LONGER than golden
    because of banner and gate-note lines, so ratio > 1 is normal and not a defect.
  * It shares census.py's brace-walk, so the 18 brace-unbalanced golden files give unreliable
    spans (see census.py KNOWN DISTORTIONS). Those are reported but flagged.
  * Overloads collapse by name, as in census.py.
So: this is a RANKING of where to look, not a verdict on any single function.

USAGE
    python tools/census/body_size_scan.py            # worst offenders
    python tools/census/body_size_scan.py --all      # every ratio below the threshold
"""
import io, os, re, sys

GOLD = os.environ.get('HT9045_GOLDEN', 'D:/HT9045/HT9011UC_Code_V3.33.906.0_20260618')
PORT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
BS = chr(92)
THRESH = 0.50
SHOW_ALL = '--all' in sys.argv

DEFN = re.compile(r'^(?!\s)(?:[A-Za-z_][\w:*&<>,\s]*?\s+)??'
                  r'(?:([A-Za-z_]\w*)::)?(~?[A-Za-z_]\w*)\s*\([^;]*$')
KW = {'if', 'for', 'while', 'switch', 'return', 'else', 'do', 'catch', 'sizeof', 'new',
      'ZeroMemory', 'memset', 'memcpy', 'delete', 'case', 'try', 'throw', 'and', 'or',
      'not', 'defined', 'assert'}


def gmap(L):
    st, g = [], []
    for l in L:
        t = l.strip()
        if re.match(r'^#\s*if\b|^#\s*ifdef\b|^#\s*ifndef\b', t):
            st.append(bool(re.match(r'^#\s*if\s+0\b', t)) or bool(st[-1] if st else False))
        elif t.startswith('#endif') and st:
            st.pop()
        g.append(bool(st[-1]) if st else False)
    return g


def funcs(path, golden):
    enc = 'cp950' if golden else 'utf-8'
    b = io.open(path, 'rb').read()
    eol = '\r\n' if b.count(b'\r\n') else '\n'
    L = io.open(path, encoding=enc, errors='replace', newline='').read().split(eol)
    g = gmap(L)
    out, i, n = {}, 0, len(L)
    while i < n:
        l = L[i]
        if not l or l[0] in ' \t/#}':
            i += 1
            continue
        m = DEFN.match(l)
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
        span, gated = j - i + 1, g[i]
        prev = out.get(key)
        if prev is None or (prev[2] and not gated) or (not gated and span > prev[1]):
            out[key] = (i + 1, span, gated)
        i = j + 1
    return out


# golden brace-unbalanced files -> spans unreliable (census.py documents this)
def unbalanced(path, golden):
    enc = 'cp950' if golden else 'utf-8'
    L = io.open(path, encoding=enc, errors='replace').read().splitlines()
    return sum(l.count('{') - l.count('}') for l in L) != 0


bpr = io.open(os.path.join(GOLD, 'HT9045.bpr'), encoding='latin-1').read()
objs = re.search(r'<OBJFILES\s+value="(.*?)"/>', bpr, re.S).group(1).split()
idx = {}
for root, dirs, fs in os.walk(GOLD):
    dirs[:] = [d for d in dirs if d.lower() != '.svn']
    for f in fs:
        if f.lower().endswith(('.cpp', '.c')):
            idx.setdefault(f.lower(), []).append(
                os.path.relpath(os.path.join(root, f), GOLD).replace(BS, '/'))

rows, checked = [], 0
for o in objs:
    if not o.lower().endswith('.obj'):
        continue
    base = os.path.basename(o.replace(BS, '/'))[:-4]
    c = idx.get((base + '.cpp').lower()) or idx.get((base + '.c').lower())
    if not c:
        continue
    src = sorted(c, key=lambda p: (p.count('/'), len(p)))[0]
    pf = os.path.join(PORT, src)
    if not os.path.exists(pf):
        continue
    gf = os.path.join(GOLD, src)
    gu = unbalanced(gf, True)
    G, P = funcs(gf, True), funcs(pf, False)
    for name, (gl, gs, _gg) in G.items():
        if name not in P:
            continue
        pl, ps, pg = P[name]
        if pg or gs < 40:
            continue                      # wholly gated is census's job; tiny fns are noise
        checked += 1
        r = ps / float(gs)
        if r < THRESH:
            rows.append((r, src, name, gl, gs, pl, ps, gu))

rows.sort()
print('port bodies compared: %d   with port/golden span ratio < %.0f%%: %d'
      % (checked, THRESH * 100, len(rows)))
print('missing golden lines implied by these: %d'
      % sum(gs - ps for _r, _s, _n, _gl, gs, _pl, ps, _u in rows))
print('\n(RANKING, not a verdict -- read this file\'s header. "!" = golden file is '
      'brace-unbalanced, so its spans are unreliable.)\n')
print('%-6s %-30s %-34s %7s %7s %s' % ('ratio', 'file', 'function', 'golden', 'port', ''))
for r, src, name, gl, gs, pl, ps, gu in (rows if SHOW_ALL else rows[:30]):
    print('%5.0f%% %-30s %-34s %7d %7d %s'
          % (r * 100, src, name, gs, ps, '!' if gu else ''))
