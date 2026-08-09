#!/usr/bin/env python3
"""HT9045 V906 translation census -- FUNCTION-LEVEL, re-runnable, version-controlled.

WHY THIS FILE EXISTS IN THE REPO
The previous census (`census.py` / `classify.py` / `remaining.py`, 20260807) was left in a
session scratchpad and vanished with the session, which is why
docs/PT_CAMPAIGN_PLAN.md section 3's percentages had to be blocked from quotation. This is
the replacement and it lives in the tree.

METHOD (per PT_CAMPAIGN_PLAN section 2)
  scope  = golden HT9045.bpr <OBJFILES>. Those entries are all `..\\Obj\\<Name>.obj`
           (a flat output dir), so sources are matched by BASENAME against an index of
           the golden tree, never looking inside .svn.
  unit   = code line (blank- and comment-stripped) and raw line, both reported.
  file   = does a port mirror exist at the same relative path?
  func   = golden top-level function definitions vs port definitions OF THE SAME NAME.
           A golden function with no port counterpart is charged at its full golden span,
           so "the file exists but only 10% is translated" cannot read as complete.
  form   = a same-named .dfm exists beside the golden source.

KNOWN DISTORTIONS, stated because a number without them is misleading
  * extract-calc-core renames. This tree deliberately renames when it extracts a
    computational core (golden `TfBarCode::DoBarcodeCCDInShuttle_1` ->
    `BarCode_DoBarcodeCCDInShuttle_1`). Pure name matching UNDER-counts. An alias rule
    covers `<Prefix>_<GoldenName>` and `<Class>_<GoldenName>`; it is deliberately NOT a
    bare cross-class match, which would mis-pair `HT9045Gem::AddEC` onto a base method.
  * `#if 0` in a port file. Golden code kept inside `#if 0` is NOT translated behaviour.
    Port functions whose body is entirely gated are reported separately as GATED rather
    than counted as translated.
  * form/non-form by .dfm is mechanically right and semantically wrong for a class that
    spans files (TfMain across main.cpp + Command.cpp). Reported, not hidden.

USAGE
    python tools/census/census.py                 # summary
    python tools/census/census.py --detail        # + per-file remaining table
    python tools/census/census.py --json out.json
"""
import io, os, re, sys, json, argparse

GOLD = os.environ.get('HT9045_GOLDEN', 'D:/HT9045/HT9011UC_Code_V3.33.906.0_20260618')
PORT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
BS = chr(92)

# A top-level definition: starts at column 0, has a parameter list, is not a call or a
# declaration ending in ';'.
DEFN = re.compile(
    r'^(?!\s)(?:[A-Za-z_][\w:*&<>,\s]*?\s+)??'      # optional return type
    r'(?:([A-Za-z_]\w*)::)?'                         # optional Class::
    r'(~?[A-Za-z_]\w*)\s*'                           # name
    r'\([^;]*$')                                     # '(' and no ';' on this line
KEYWORDS = {'if', 'for', 'while', 'switch', 'return', 'else', 'do', 'catch', 'sizeof',
            'ZeroMemory', 'memset', 'memcpy', 'delete', 'new', 'case', 'try', 'throw',
            'and', 'or', 'not', 'defined', 'assert'}


def read(path, golden):
    enc = 'cp950' if golden else 'utf-8'
    return io.open(path, encoding=enc, errors='replace').read()


def code_lines(text):
    raw = len(text.splitlines())
    t = re.sub(r'/\*.*?\*/', '', text, flags=re.S)
    n = sum(1 for l in t.splitlines() if re.sub(r'//.*', '', l).strip())
    return raw, n


def gate_depth_map(lines):
    """line index (0-based) -> True if inside any `#if 0`."""
    out, stack = [], []
    for l in lines:
        t = l.strip()
        if re.match(r'^#\s*if\b|^#\s*ifdef\b|^#\s*ifndef\b', t):
            zero = bool(re.match(r'^#\s*if\s+0\b', t))
            stack.append(zero or (stack and stack[-1]))
        elif t.startswith('#endif') and stack:
            stack.pop()
        out.append(bool(stack and stack[-1]))
    return out


def functions(text):
    """name -> (start, end, span, gated) using brace balance."""
    lines = text.splitlines()
    gated = gate_depth_map(lines)
    out, i, n = {}, 0, len(lines)
    while i < n:
        l = lines[i]
        if not l or l[0] in ' \t/#}':
            i += 1
            continue
        m = DEFN.match(l)
        if not m:
            i += 1
            continue
        name = m.group(2)
        if name in KEYWORDS or (m.group(1) or '') in KEYWORDS:
            i += 1
            continue
        # walk to the opening brace, then to balance
        depth, j, seen = 0, i, False
        while j < n:
            depth += lines[j].count('{') - lines[j].count('}')
            if '{' in lines[j]:
                seen = True
            if seen and depth <= 0:
                break
            j += 1
            if j - i > 4000:
                break
        if not seen:
            i += 1
            continue
        key = (m.group(1) + '::' + name) if m.group(1) else name
        out.setdefault(key, (i + 1, j + 1, j - i + 1, gated[i]))
        i = j + 1
    return out


def index_golden():
    idx = {}
    for root, dirs, files in os.walk(GOLD):
        dirs[:] = [d for d in dirs if d.lower() != '.svn']
        for f in files:
            if f.lower().endswith(('.cpp', '.c')):
                rel = os.path.relpath(os.path.join(root, f), GOLD).replace(BS, '/')
                idx.setdefault(f.lower(), []).append(rel)
    return idx


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--detail', action='store_true')
    ap.add_argument('--json')
    a = ap.parse_args()

    idx = index_golden()
    bpr = io.open(os.path.join(GOLD, 'HT9045.bpr'), encoding='latin-1').read()
    objs = re.search(r'<OBJFILES\s+value="(.*?)"/>', bpr, re.S).group(1).split()

    rows, unresolved = [], []
    for o in objs:
        o = o.strip()
        if not o.lower().endswith('.obj'):
            continue
        base = os.path.basename(o.replace(BS, '/'))[:-4]
        cands = idx.get((base + '.cpp').lower()) or idx.get((base + '.c').lower())
        if not cands:
            unresolved.append(o)
            continue
        src = sorted(cands, key=lambda p: (p.count('/'), len(p)))[0]
        gtext = read(os.path.join(GOLD, src), True)
        graw, gcode = code_lines(gtext)
        gfun = functions(gtext)
        form = os.path.exists(os.path.splitext(os.path.join(GOLD, src))[0] + '.dfm')

        pfile = os.path.join(PORT, src)
        if os.path.exists(pfile):
            ptext = read(pfile, False)
            pfun = functions(ptext)
            # alias set: bare names, Class::name, and Prefix_name extractions
            pnames = set()
            for k, v in pfun.items():
                bare = k.split('::')[-1]
                pnames.add(k)
                pnames.add(bare)
                if not v[3]:
                    pnames.add('LIVE:' + bare)
            extracted = {k.split('_', 1)[1] for k in pnames if '_' in k}
            missing, gated_ct, done_ct = [], 0, 0
            for k, (s, e, span, _g) in gfun.items():
                bare = k.split('::')[-1]
                hit = (k in pnames) or (bare in pnames) or (bare in extracted)
                if not hit:
                    missing.append((k, span))
                elif ('LIVE:' + bare) not in pnames and bare in pnames:
                    gated_ct += 1
                    missing.append((k, span))     # gated == not translated behaviour
                else:
                    done_ct += 1
            miss_lines = sum(s for _, s in missing)
            rows.append(dict(src=src, form=form, mirrored=True, graw=graw, gcode=gcode,
                             gfuncs=len(gfun), missing_funcs=len(missing),
                             gated_funcs=gated_ct, missing_lines=miss_lines))
        else:
            rows.append(dict(src=src, form=form, mirrored=False, graw=graw, gcode=gcode,
                             gfuncs=len(gfun), missing_funcs=len(gfun),
                             gated_funcs=0, missing_lines=gcode))

    def tot(rs, k):
        return sum(r[k] for r in rs)

    print('scope: %d OBJFILES -> %d sources resolved, %d unresolved'
          % (len(objs), len(rows), len(unresolved)))
    if unresolved:
        print('  UNRESOLVED:', unresolved)
    print()
    print('%-10s %5s %9s %9s %9s %7s' % ('group', 'files', 'goldCode', 'missing', 'done', 'done%'))
    for label, rs in (('non-form', [r for r in rows if not r['form']]),
                      ('form', [r for r in rows if r['form']]),
                      ('ALL', rows)):
        gc, ml = tot(rs, 'gcode'), tot(rs, 'missing_lines')
        print('%-10s %5d %9d %9d %9d %6.1f%%'
              % (label, len(rs), gc, ml, gc - ml, 100.0 * (gc - ml) / gc if gc else 0))
    print()
    nomirror = [r for r in rows if not r['mirrored']]
    print('files with NO port mirror: %d non-form / %d form'
          % (len([r for r in nomirror if not r['form']]),
             len([r for r in nomirror if r['form']])))
    partial = sorted([r for r in rows if r['mirrored'] and r['missing_lines'] > 0],
                     key=lambda r: -r['missing_lines'])
    print('mirrored but INCOMPLETE: %d files, %d golden code lines still missing'
          % (len(partial), sum(r['missing_lines'] for r in partial)))
    if a.detail:
        print()
        print('%-42s %5s %6s %6s %8s %s' % ('file', 'funcs', 'miss', 'gated', 'missLine', 'form'))
        for r in partial[:60]:
            print('%-42s %5d %6d %6d %8d %s'
                  % (r['src'], r['gfuncs'], r['missing_funcs'], r['gated_funcs'],
                     r['missing_lines'], 'FORM' if r['form'] else ''))
    if a.json:
        io.open(a.json, 'w', encoding='utf-8').write(json.dumps(rows, indent=1))
        print('\nwrote', a.json)
    return 0


sys.exit(main())
