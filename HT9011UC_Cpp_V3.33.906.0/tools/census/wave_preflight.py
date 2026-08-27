# -*- coding: utf-8 -*-
"""Wave pre-flight: run this BEFORE dispatching a translation agent at a golden file.

Why this exists
===============
Every FW-3 wave needs the same four checks, and three of them have each cost a
real wave when skipped.  They were being re-improvised by hand each time, which
is how they get skipped.  This bundles them into one command.

  1. SPAN SANITY -- does census's brace matching swallow definitions in this
     file?  If it does, the "missing" list is fiction and the denominator you
     hand the agent is wrong.  (Command.cpp: one `{` inside a comment swallowed
     61 definitions and sent an entire wave out to translate already-translated
     code.)  Delegates to span_sanity.py's own logic.

  2. INVENTORY -- the real denominator, split into `TClass::` members vs
     file-scope bodies.  OCR1's golden had 88 bodies but only 82 members; a
     wave reported as "3/88" would understate itself, and one reported as
     "3/82" has to say so explicitly.

  3. SHIM OCCUPATION, ALL FOUR SHAPES -- the port tree may already own the name
     you are about to introduce, in four different ways, each found the hard way:
       (a) class name      -- W32,   `class TfTrayMapping` in a shim header
       (b) function name   -- W34,   four `Initial*Task` bodies
       (c) global name ONLY-- IOSV1, `fiosetview` taken, class name free
       (d) TU-local seam + `#define` -- OCR1, `W906OCR_TfOCRSeam` plus
           `#define fOCR W906OCR_fOCR`, which makes the whole delivery inert:
           it compiles, it links, and nothing calls it.
     Shape (d) is invisible to (a)-(c) checks, which is why all four run here.

  4. CALL SITES, LIVE vs GATED -- `fX->member` sites split by whether the
     compiler can actually see them.  This answers two questions at once:
     which members MUST exist (live sites -- absent ones are a build break),
     and which member NAMES are spoken for by `#if 0` sites waiting to be
     un-gated (the facade must use those exact names or a future un-gate will
     not match).  For AutoClean the split was 3 live / 33 gated, which is the
     only reason a 4-field facade coexists with 36 dereferenced members.

Deliberately NOT modified: census.py.  Changing `functions()` breaks
comparability with every historical number, and the last attempt to "fix" it
produced parked > gcode and 35,289 lines of false credit.  This is a read-only
second opinion, like span_sanity.py and coverage_probe.py.

Output is ASCII-only ON PURPOSE.  coverage_probe.py v1 printed U+26A0, died on
cp950, and returned exit 1 with the analysis already complete -- an output-side
encoding fault that looked exactly like an analysis failure.

Usage
=====
    python tools/census/wave_preflight.py AutoClean/uCleaning.cpp
    python tools/census/wave_preflight.py forms/cOffSet.cpp --out report.txt

An absence-claim printed here is true AS OF NOW.  Sibling agents landing files
later in the same wave can falsify it -- re-run at closeout (trap #2).
"""
import io
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, '..', '..'))
GOLDEN = os.path.abspath(os.path.join(ROOT, '..', 'HT9011UC_Code_V3.33.906.0_20260618'))

SKIP_DIRS = ('build', '_', '.git', '.vs', 'node_modules', 'third_party', 'vendor')
SRC_EXT = ('.cpp', '.h', '.hpp', '.c')

# Macros measured to be UNDEFINED in this build, so `#ifdef <macro>` regions are
# dead code.  census's gate_depth_map models `#if 0` only -- it does not evaluate
# macros -- so without this list a site inside `#ifdef SOFT_SIMULTE` is reported
# LIVE, and a wave gets told a facade "must declare" a member it must not.
# That is not hypothetical: csystem.cpp:17614 calls
# fCleaning->btnResetCleanCountClick() inside `#ifdef SOFT_SIMULTE`, and the tree
# builds green with no such member declared.
#
# Evidence for SOFT_SIMULTE (20260827): no `#define SOFT_SIMULTE` anywhere in the
# tree, nothing in CMakeLists.txt or build.bat defines it, and the compiler itself
# agrees -- `#include "MachineType.h"` + `#ifdef SOFT_SIMULTE / #error` compiles
# clean with the standard flag set.
#
# Add to this list ONLY with the same kind of evidence.  A macro that is actually
# defined would be silently misfiled in the other direction.
KNOWN_OFF = {'SOFT_SIMULTE'}

COND_RE = re.compile(r'^\s*#\s*(if|ifdef|ifndef|elif|else|endif)\b(.*)$')


def cond_stack_map(lines):
    """line index -> innermost enclosing preprocessor condition text ('' if none).

    Deliberately does NOT evaluate anything.  It only reports what condition a
    line sits under, so a human (or KNOWN_OFF) can judge.  Pretending to
    evaluate is how you get a confident wrong answer.
    """
    out, stack = [], []
    for ln in lines:
        m = COND_RE.match(ln)
        if m:
            kw, rest = m.group(1), m.group(2).strip()
            if kw in ('if', 'ifdef', 'ifndef'):
                stack.append('#%s %s' % (kw, rest))
            elif kw in ('elif', 'else'):
                if stack:
                    stack[-1] = '#%s %s' % (kw, rest) if rest else '#%s' % kw
            elif kw == 'endif':
                if stack:
                    stack.pop()
            out.append(stack[-1] if stack else '')
            continue
        out.append(stack[-1] if stack else '')
    return out


def cond_is_known_off(cond):
    """True when the condition is `#ifdef M` / `#if defined(M)` for a KNOWN_OFF M."""
    if not cond:
        return None
    m = re.match(r'#ifdef\s+(\w+)', cond) or re.match(r'#if\s+defined\s*\(?\s*(\w+)', cond)
    if m and m.group(1) in KNOWN_OFF:
        return m.group(1)
    return None


def _prefix_exec(fname):
    """exec only the part before main() -- importing these modules runs main().

    The cut must anchor on a COLUMN-0 `\\ndef main(`.  span_sanity.py contains the
    literal 'def main(' inside its own loader, so a bare .index() cuts mid-function
    and raises an unterminated-string SyntaxError from the truncated docstring.
    """
    path = os.path.join(HERE, fname)
    src = io.open(path, encoding='utf-8').read()
    cut = src.index('\ndef main(')
    ns = {'__name__': fname[:-3] + '_helpers', '__file__': path}
    exec(compile(src[:cut], fname + '_prefix', 'exec'), ns)
    return ns


def _census_helpers():
    return _prefix_exec('census.py')


def _span_sanity_helpers():
    return _prefix_exec('span_sanity.py')


DEFN_LINE = re.compile(r'(?<![A-Za-z0-9_])(\w+)::(~?\w+)\s*\(')


def overload_sets(text, strip_comments):
    """name -> [line numbers], for names defined more than once at column 0.

    census's `functions()` stores results in a dict keyed by `Class::name`, so a
    C++ OVERLOAD SET collapses to a single entry and the golden denominator is
    silently short.  Found 20260827 on golden ATC/ATC_Handler_Side.cpp, which
    defines ChangeRecipe twice -- `(AnsiString)` at :1920 and
    `(AnsiString, double)` at :1927, both real independent bodies -- and which
    census therefore reports as 168 bodies when column-0 definition lines number
    169.

    This is the FIRST of the census defects that understates the GOLDEN side;
    the four earlier ones all understated the port side.  For census's own
    percentages the effect largely cancels, because an overload pair collapses
    on both sides -- but it stops cancelling the moment only one overload is
    translated, and it always understates a wave's denominator.

    census.py is deliberately left alone (changing functions() would break
    comparability with every historical number).  This is a read-only warning.
    """
    out = {}
    for i, ln in enumerate(strip_comments(text).splitlines()):
        if ln[:1] in (' ', '\t') or not ln.strip():
            continue
        m = DEFN_LINE.search(ln)
        if m:
            out.setdefault('%s::%s' % (m.group(1), m.group(2)), []).append(i + 1)
    return dict((k, v) for k, v in out.items() if len(v) > 1)


def port_files():
    out = []
    for dp, dns, fns in os.walk(ROOT):
        dns[:] = [d for d in dns if not d.lower().startswith(SKIP_DIRS)]
        for fn in fns:
            if fn.endswith(SRC_EXT):
                out.append(os.path.join(dp, fn).replace(os.sep, '/'))
    return out


def discover_names(golden_rel):
    """class name + global name, taken from golden's sibling header."""
    gh = os.path.join(GOLDEN, os.path.splitext(golden_rel)[0] + '.h')
    if not os.path.exists(gh):
        return None, None, '(no sibling .h at %s)' % os.path.basename(gh)
    t = io.open(gh, encoding='cp950', errors='replace').read()
    cls = glob = None
    m = re.search(r'^\s*class\s+(\w+)\s*:\s*public\s+TForm', t, re.M)
    if m:
        cls = m.group(1)
    m = re.search(r'^\s*extern\s+(?:PACKAGE\s+)?(\w+)\s*\*\s*(\w+)\s*;', t, re.M)
    if m:
        if cls is None:
            cls = m.group(1)
        glob = m.group(2)
    return cls, glob, os.path.basename(gh)


def main():
    if len(sys.argv) < 2:
        print(__doc__.split('Usage')[1])
        return 2
    rel = sys.argv[1].replace('\\', '/')
    outpath = None
    if '--out' in sys.argv:
        outpath = sys.argv[sys.argv.index('--out') + 1]

    gpath = os.path.join(GOLDEN, rel)
    if not os.path.exists(gpath):
        print('golden file not found: %s' % gpath)
        return 2

    C = _census_helpers()
    S = _span_sanity_helpers()
    functions, read, gate_depth_map = C['functions'], C['read'], C['gate_depth_map']
    strip_comments = S['strip_comments']

    L = []
    def w(s=''):
        L.append(s)

    text = read(gpath, True)
    w('=' * 78)
    w('WAVE PRE-FLIGHT  golden %s' % rel)
    w('=' * 78)

    # --- 1. span sanity -----------------------------------------------------
    a = functions(text)
    b = functions(strip_comments(text))
    lost = sorted(set(b) - set(a), key=lambda k: b[k][0])
    w()
    w('[1] SPAN SANITY  (census brace matching vs comment-stripped)')
    w('    census sees %d bodies / stripped sees %d / swallowed %d'
      % (len(a), len(b), len(b) - len(a)))
    if lost:
        w('    !! DENOMINATOR IS UNRELIABLE -- these definitions are swallowed:')
        for k in lost:
            w('       %-46s :%d-%d' % (k, b[k][0], b[k][1]))
        w('    Use the stripped count, and say so in the wave brief.')
    else:
        w('    clean -- no brace swallowing.')

    # --- 1b. overload collapse ---------------------------------------------
    ov = overload_sets(text, strip_comments)
    extra = sum(len(v) - 1 for v in ov.values())
    w('    overload check: %d name(s) defined more than once, %d body(ies)'
      % (len(ov), extra))
    w('                    hidden by census keying its dict on the name')
    if ov:
        for k in sorted(ov):
            w('       !! %-40s lines %s' % (k, ', '.join(str(x) for x in ov[k])))
        base = len(b) if lost else len(a)
        w('    ** TRUE DENOMINATOR IS %d BODIES, not %d. Quote the larger one.'
          % (base + extra, base))

    # --- 2. inventory -------------------------------------------------------
    use = b if lost else a
    rows = [(v[2], k, v[0], v[1], v[3]) for k, v in use.items()]
    rows.sort(reverse=True)
    members = [r for r in rows if '::' in r[1]]
    frees = [r for r in rows if '::' not in r[1]]
    classes = {}
    for r in members:
        classes[r[1].split('::')[0]] = classes.get(r[1].split('::')[0], 0) + 1
    w()
    w('[2] INVENTORY  %d bodies = %d member(s) + %d file-scope, %d span lines'
      % (len(rows), len(members), len(frees), sum(r[0] for r in rows)))
    for c in sorted(classes, key=lambda x: -classes[x]):
        w('    %-34s %d bodies' % (c + '::', classes[c]))
    free_names = [r[1] for r in frees]
    if frees:
        w('    (file-scope: %s)' % ', '.join(free_names[:8]))
    w('    -- QUOTE THE MEMBER COUNT AS THE DENOMINATOR, not the body count,')
    w('       if the wave only translates members.')
    w()
    w('    largest bodies (these decide whether the wave is thick or thin):')
    for s, k, x, y, g in rows[:12]:
        w('      %5d  %-46s :%d-%d%s' % (s, k, x, y, '  [GATED]' if g else ''))

    # --- 3. occupation, four shapes ----------------------------------------
    cls, glob, hsrc = discover_names(rel)
    w()
    w('[3] SHIM OCCUPATION  (golden header %s -> class %s / global %s)'
      % (hsrc, cls, glob))
    files = port_files()
    hits = {'a_class': [], 'b_func': [], 'c_global': [], 'd_seam': []}
    mem_names = set(r[1].split('::')[-1] for r in members)
    stem = re.sub(r'^Tf|^T', '', cls or '') or 'ZZZZ'
    seam_re = re.compile(r'\bclass\s+(\w*%s\w*Seam\w*|W906\w*%s\w*)\b' % (stem, stem))
    def_re = re.compile(r'#\s*define\s+(%s|%s)\b' % (re.escape(cls or 'ZZZZ'),
                                                     re.escape(glob or 'ZZZZ')))
    for p in files:
        rel_p = p[len(ROOT) + 1:].replace(os.sep, '/')
        try:
            t = io.open(p, encoding='utf-8', errors='replace').read()
        except Exception:
            continue
        if stem not in t and (glob or 'ZZZZ') not in t:
            continue
        s = strip_comments(t)
        # Detect on the STRIPPED text.  v1 searched the raw text and reported
        # forms/fOCR.h as carrying a `#define fOCR` -- it was a comment quoting
        # the seam it documents.  Principle #4: grep finding the string only
        # proves the text exists, not that the compiler sees it.
        m = def_re.search(s)
        if m:
            hits['d_seam'].append((rel_p, '#define %s' % m.group(1)))
        for m in seam_re.finditer(s):
            hits['d_seam'].append((rel_p, 'class ' + m.group(1)))
        if cls and re.search(r'\bclass\s+%s\b' % re.escape(cls), s):
            hits['a_class'].append(rel_p)
        if glob and re.search(r'\b(?:extern\s+)?\w+\s*\*\s*%s\s*[;=]' % re.escape(glob), s):
            hits['c_global'].append(rel_p)
        for m in re.finditer(r'(\w+)\s*::\s*(\w+)\s*\(', s):
            if m.group(2) in mem_names and m.group(1) != cls:
                hits['b_func'].append('%s  [%s::%s]' % (rel_p, m.group(1), m.group(2)))
    w('    scanned %d port files (build_*/vendor/third_party excluded)' % len(files))
    for key, label in (('a_class', '(a) class name'),
                       ('c_global', '(c) global name'),
                       ('b_func', '(b) function name, OTHER class'),
                       ('d_seam', '(d) TU-local seam / #define')):
        v = hits[key]
        if not v:
            w('    %-32s clear' % label)
        else:
            w('    %-32s %d hit(s):' % (label, len(v)))
            for x in sorted(set(map(str, v)))[:12]:
                w('        %s' % x)
    w('    NOTE: hits under (a)/(c) that are the facade you are about to extend')
    w('          are yours, not an occupation. Read the paths before concluding.')

    # --- 4. call sites, live vs gated --------------------------------------
    w()
    w('[4] CALL SITES  %s->member, split by gate depth' % glob)
    live, gated, redirect, macro_off, cond_live = {}, {}, {}, {}, {}
    redirected_files = {}
    cond_seen = {}
    if glob:
        pat = re.compile(r'\b%s\s*->\s*(\w+)' % re.escape(glob))
        rdr = re.compile(r'#\s*define\s+%s\s+(\w+)' % re.escape(glob))
        for p in files:
            rel_p = p[len(ROOT) + 1:].replace(os.sep, '/')
            try:
                t = io.open(p, encoding='utf-8', errors='replace').read()
            except Exception:
                continue
            if glob not in t:
                continue
            stripped = strip_comments(t)
            lines = stripped.splitlines()
            g = gate_depth_map(lines)
            conds = cond_stack_map(lines)
            # A file that `#define`s the global away binds every `glob->` in it
            # to something ELSE.  Counting those as live requirements on the
            # facade is exactly how a wave gets sent to add 33 fields that
            # belong to a seam.  v1 did that; this is the fix.
            rm = rdr.search(stripped)
            if rm:
                redirected_files[rel_p] = rm.group(1)
            for i, ln in enumerate(lines):
                for m in pat.finditer(ln):
                    where = '%s:%d' % (rel_p, i + 1)
                    if rm:
                        bucket = redirect
                    elif g[i]:
                        bucket = gated
                    else:
                        off = cond_is_known_off(conds[i])
                        if off:
                            bucket = macro_off
                            cond_seen[where] = off
                        elif conds[i]:
                            bucket = cond_live
                            cond_seen[where] = conds[i]
                        else:
                            bucket = live
                    bucket.setdefault(m.group(1), []).append(where)

    def dump(d):
        for k in sorted(d):
            w('      %-32s %s' % (k, ', '.join(d[k][:3]) + (' ...' if len(d[k]) > 3 else '')))

    w('    LIVE %d member(s), unconditional -- the facade MUST declare every one'
      % len(live))
    w('         of these, or the tree does not build:')
    dump(live)
    if macro_off:
        w('    MACRO-OFF %d member(s) -- inside `#ifdef <macro>` for a macro'
          % len(macro_off))
        w('         measured UNDEFINED in this build (%s). Dead code.'
          % ', '.join(sorted(KNOWN_OFF)))
        w('         NOT a requirement on the facade:')
        for k in sorted(macro_off):
            w('      %-32s %s  [%s]' % (k, macro_off[k][0],
                                        cond_seen.get(macro_off[k][0], '?')))
    if cond_live:
        w('    CONDITIONAL %d member(s) -- under a preprocessor condition this'
          % len(cond_live))
        w('         tool does NOT evaluate. Judge each one yourself; do not')
        w('         treat them as live or as dead on this report alone:')
        for k in sorted(cond_live):
            w('      %-32s %s  [%s]' % (k, cond_live[k][0],
                                        cond_seen.get(cond_live[k][0], '?')[:44]))
    w('    GATED %d member(s) -- text exists, compiler cannot see it.' % len(gated))
    w('         New fields MUST reuse these exact names, or a future un-gate')
    w('         will silently fail to match:')
    dump(gated)
    if redirected_files:
        w('    REDIRECTED %d member(s) in %d file(s) that #define the global away:'
          % (len(redirect), len(redirected_files)))
        for f in sorted(redirected_files):
            w('         %s  ->  %s' % (f, redirected_files[f]))
        w('         These bind to the seam, NOT to the facade. They are NOT')
        w('         requirements on your header, and they are the reason a')
        w('         delivery can compile, link, and still have zero callers.')
        dump(redirect)
    both = sorted(set(live) & set(gated))
    if both:
        w('    !! %d name(s) appear BOTH live and gated -- check for two classes'
          % len(both))
        w('       sharing a name (trap #5): %s' % ', '.join(both))

    # --- 4b. file-scope call sites -----------------------------------------
    # Section [4] above only matches `glob->member`, so a golden file's
    # FILE-SCOPE functions have no call sites reported at all -- the tool says
    # nothing and reads as "nobody calls it".  FW3-HSP1 hit this: golden
    # cShowBinSet.cpp's ShowSiteMapping_YES_NO() is called from csystem.cpp:11222
    # inside a #if 0, and only a hand-written grep found it.  Same family as the
    # two defects already fixed here: the report was silent where it should have
    # spoken.
    if free_names:
        fs_live, fs_gated = {}, {}
        alt = '|'.join(re.escape(n) for n in free_names)
        fs_re = re.compile(r'(?<![A-Za-z0-9_])(%s)\s*\(' % alt)
        for p in files:
            rel_p = p[len(ROOT) + 1:].replace(os.sep, '/')
            try:
                t = io.open(p, encoding='utf-8', errors='replace').read()
            except Exception:
                continue
            if not any(n in t for n in free_names):
                continue
            lines = strip_comments(t).splitlines()
            g = gate_depth_map(lines)
            for i, ln in enumerate(lines):
                # A DEFINITION or DECLARATION sits at column 0 in this tree; a CALL
                # is inside a function body and therefore indented.  The first
                # attempt tried to recognise definitions by regex and silently
                # failed -- it reported forms/fShowBinSet.cpp:93 (the definition)
                # and .h:293 (the declaration) as "live call sites".  Calling a
                # definition a call site is the same class of imprecision this
                # whole tool exists to remove, so the rule is now the simple one
                # the tree's own style supports.  LIMITATION, stated rather than
                # hidden: an indented declaration or a definition written with a
                # leading space would be misfiled.
                if ln[:1] not in (' ', '	'):
                    continue
                for m in fs_re.finditer(ln):
                    (fs_gated if g[i] else fs_live).setdefault(
                        m.group(1), []).append('%s:%d' % (rel_p, i + 1))
        w()
        w('[4b] FILE-SCOPE CALL SITES  (names from [2]; section [4] only sees')
        w('     glob->member, so these would otherwise be reported as nothing)')
        w('     LIVE %d, GATED %d   -- indented matches only; column-0 lines are'
          % (len(fs_live), len(fs_gated)))
        w('     treated as definitions/declarations, not calls')
        for k in sorted(fs_live):
            w('       live   %-30s %s' % (k, ', '.join(fs_live[k][:3])))
        for k in sorted(fs_gated):
            w('       gated  %-30s %s' % (k, ', '.join(fs_gated[k][:3])))
        if not fs_live and not fs_gated:
            w('       (no call sites found)')

    w()
    w('reminder: every absence above is true as of this run only. Re-run at')
    w('closeout -- sibling agents can land files that falsify it (trap #2).')

    body = '\n'.join(L)
    if outpath:
        io.open(outpath, 'w', encoding='utf-8', newline='\n').write(body + '\n')
        print('written: %s (%d lines)' % (outpath, len(L)))
    else:
        print(body)
    return 0


if __name__ == '__main__':
    sys.exit(main())
