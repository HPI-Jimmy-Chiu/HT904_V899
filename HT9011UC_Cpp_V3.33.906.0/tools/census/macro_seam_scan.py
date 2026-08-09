#!/usr/bin/env python3
"""Find EVERY macro seam in the port whose real body now exists -- the trap class that
neither the link nor any collision scan can see.

WHY THIS EXISTS
A macro seam is `static <ret> PREFIX_Name(...) { <offline default> }` followed by
`#define Name PREFIX_Name`. Every call site after that line expands to the stub. When the
real body later lands elsewhere, there is NO duplicate symbol, so:
  * the link is perfectly clean,
  * `nm` shows nothing,
  * a duplicate-definition collision scan finds nothing,
  * and the real body silently has zero callers in that TU.
PT-W5c hit this twice: csystem.cpp's three W7C2 seams (fixed, commit a9bd699) and
aoutarm.cpp's PTW4_ClearFixTray / PTW4_CheckPickerLifeNeedOneCycle (found by audit 4).
Both times the seam's own comment named the retirement condition and both times it was
missed, because nothing mechanical was looking. This is that mechanical check.

OUTPUT: for each seam, whether a real definition of the seamed name now exists ELSEWHERE
in the port, and how many call sites in the seam's own file are downstream of the #define
(those are the ones currently reading the stub).
Read-only.
"""
import io, os, re

PORT = 'D:/HT9045/HT9011UC_Cpp_V3.33.906.0'
SEP = chr(92)

SEAM = re.compile(r'^\s*#\s*define\s+([A-Za-z_]\w*)\s+([A-Za-z_]\w*)\s*$')
HEAD = re.compile(r'^(?!\s)(?:[A-Za-z_~][\w:*&<>,\s]*?[\s*&:])?(~?[A-Za-z_]\w*)\s*[\(\[=;]')
KW = {'if', 'for', 'while', 'switch', 'return', 'else', 'do', 'catch', 'sizeof', 'new',
      'ZeroMemory', 'memset', 'memcpy', 'delete', 'case', 'try', 'throw', 'and', 'or',
      'not', 'defined', 'assert', 'extern', 'typedef', 'using'}


def readlines(p):
    b = io.open(p, 'rb').read()
    eol = '\r\n' if b.count(b'\r\n') else '\n'
    return io.open(p, encoding='utf-8', errors='replace', newline='').read().split(eol)


srcs = []
for root, dirs, fs in os.walk(PORT):
    dirs[:] = [d for d in dirs if not d.startswith('build') and d not in ('.git', '.svn')]
    for f in fs:
        if f.endswith(('.cpp', '.cc')):
            srcs.append(os.path.relpath(os.path.join(root, f), PORT).replace(SEP, '/'))

# index of real (non-static, column-0) definitions per file
defs = {}
for p in srcs:
    L = readlines(os.path.join(PORT, p))
    for i, raw in enumerate(L):
        if not raw or raw[0] in ' \t/#}':
            continue
        if re.match(r'^\s*static\b', raw):
            continue
        m = HEAD.match(raw)
        if m and m.group(1) not in KW:
            defs.setdefault(m.group(1), []).append((p, i + 1))

rows = []
for p in srcs:
    L = readlines(os.path.join(PORT, p))
    for i, raw in enumerate(L):
        m = SEAM.match(raw)
        if not m:
            continue
        name, target = m.group(1), m.group(2)
        if name == target or not re.match(r'^(W\d|PTW|GA\d|W7C2|W6|PT)', target):
            continue
        # is there an #undef of this name later in the same file?
        undef = next((j + 1 for j in range(i + 1, len(L))
                      if re.match(r'^\s*#\s*undef\s+' + name + r'\s*$', L[j])), None)
        # real definitions elsewhere
        elsewhere = [d for d in defs.get(name, []) if not (d[0] == p and d[1] > i)]
        real = [d for d in defs.get(name, []) if d[0] != p]
        # call sites in this file downstream of the #define and upstream of any #undef
        hi = undef if undef else len(L) + 1
        calls = [j + 1 for j in range(i + 1, min(hi, len(L)) - 1)
                 if re.search(r'\b' + name + r'\s*\(', L[j])
                 and not L[j].lstrip().startswith(('//', '*'))
                 and 'define' not in L[j]]
        if real:
            rows.append((p, i + 1, name, target, real, undef, calls))

print('macro seams whose REAL BODY NOW EXISTS elsewhere in the port: %d\n' % len(rows))
for p, ln, name, target, real, undef, calls in sorted(rows, key=lambda r: -len(r[6])):
    print('%s:%d' % (p, ln))
    print('    #define %-32s -> %s' % (name, target))
    print('    real body:        %s' % ', '.join('%s:%d' % r for r in real[:3]))
    print('    #undef in file:   %s' % (undef if undef else 'NONE'))
    print('    call sites still reading the STUB: %d %s'
          % (len(calls), calls[:8] if calls else ''))
