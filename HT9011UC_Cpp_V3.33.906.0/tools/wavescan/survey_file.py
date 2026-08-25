# -*- coding: utf-8 -*-
"""通用檔案盤點：某個 golden 檔的方法清單、port 有沒有、缺多少行。

用法: python survey_file.py <檔名> <類別名>
例:   python survey_file.py cTemperFrom.cpp TfTemperFrom

括號配對用修正版（先剝字串→再剝 // →才數），並以下一個頂層定義為硬上界。
"""
import io
import re
import sys

GOLD_ROOT = 'D:/HT9045/HT9011UC_Code_V3.33.906.0_20260618/'
PORT_ROOT = 'D:/HT9045/HT9011UC_Cpp_V3.33.906.0/'

fn = sys.argv[1]
cls = sys.argv[2]

g = io.open(GOLD_ROOT + fn, encoding='cp950', errors='replace').read().split('\n')
try:
    port = io.open(PORT_ROOT + fn, encoding='utf-8', errors='replace').read()
except FileNotFoundError:
    port = ''

STR = re.compile(r'"(?:\\.|[^"\\])*"')
CHR = re.compile(r"'(?:\\.|[^'\\])*'")

def code_only(line):
    line = STR.sub('""', line)
    line = CHR.sub("''", line)
    i = line.find('//')
    return line if i < 0 else line[:i]

DEF = re.compile(r'^[A-Za-z_][\w :\*&]*\b%s::(\w+)\s*\(' % cls)
defs = [(i, DEF.match(l).group(1)) for i, l in enumerate(g) if DEF.match(l)]
order = [i for i, _ in defs]

rows = []
for k, (i, n) in enumerate(defs):
    nxt = order[k + 1] if k + 1 < len(order) else len(g)
    j = i
    while j < nxt and '{' not in code_only(g[j]):
        j += 1
    d = 0
    end = None
    m = j
    while m < nxt:
        c = code_only(g[m])
        d += c.count('{') - c.count('}')
        if d == 0:
            end = m
            break
        m += 1
    if end is None:
        end = nxt - 1
    have = bool(re.search(r'\b%s::%s\s*\(' % (cls, n), port))
    rows.append((n, i + 1, end + 1, end - i + 1, have))

# 20260825：這支原本只在同名 port 檔裡找 `<cls>::<name>(`，於是**被搬到別的
# port 類別的方法一律讀成「缺」**。本專案刻意把 golden 的大類別拆開
# （THGem -> SecsWireCodec / SecsSvEcRegistration / HTGem 是最明顯的一組），
# 所以這個盲點會系統性高估缺口。
# 實例：FW-GEM-W8 選了 THGem 的 SML/local-head 家族 14 支，**其中 13 支早就
# 以 SecsWireCodec 成員的身分翻好了**（SecsWireCodec.cpp:429/454/728/1282/
# 1300/1491/1517/1538/1551/1569/1661/1696/2228）。差點整波重翻。
# 所以第二輪要對全樹掃 `AnyClass::<name>(`，把「在別的類別」單獨列出來。
ELSE = {}
if miss_scan := [r for r in rows if not r[4]]:
    import os
    names = {r[0] for r in miss_scan}
    pat = re.compile(r'\b(\w+)::(%s)\s*\(' % '|'.join(re.escape(n) for n in names))
    for root, dirs, files in os.walk(PORT_ROOT):
        dirs[:] = [d for d in dirs
                   if not d.startswith(('build', '.git', '.svn')) and d != 'tests']
        for f in files:
            if not f.endswith(('.cpp', '.h', '.hpp')):
                continue
            p = os.path.join(root, f)
            if os.path.relpath(p, PORT_ROOT).replace('\\', '/') == fn:
                continue
            try:
                t = io.open(p, encoding='utf-8', errors='replace').read()
            except OSError:
                continue
            for m2 in pat.finditer(t):
                if m2.group(1) != cls:
                    ELSE.setdefault(m2.group(2), []).append(
                        '%s::  %s:%s' % (m2.group(1),
                                         os.path.relpath(p, PORT_ROOT).replace('\\', '/'),
                                         t[:m2.start()].count('\n') + 1))

miss = [r for r in rows if not r[4] and r[0] not in ELSE]
moved = [r for r in rows if not r[4] and r[0] in ELSE]
print('%s / %s' % (fn, cls))
print('  golden 方法 %d 個；port 同類別已有 %d' % (len(rows), len(rows) - len(miss) - len(moved)))
print('  在別的 port 類別（已翻，非缺口）: %d 個 / %d 行' % (len(moved), sum(r[3] for r in moved)))
print('  真正缺: %d 個 / %d 行' % (len(miss), sum(r[3] for r in miss)))
print()
if moved:
    print('--- 已在別的 port 類別（注意：逐條看擁有者類別再信，同名不同類會誤入本表，')
    print('    實例 THGem::FormDestroy 命中的是 cBinSel.cpp 的 TfBinSel::FormDestroy）---')
    for n, a, b, c, _ in sorted(moved, key=lambda x: -x[3]):
        print('   %-40s golden :%-5d %4d 行  -> %s' % (n, a, c, ELSE[n][0]))
    print()
print('--- 真正缺 ---')
for n, a, b, c, _ in sorted(miss, key=lambda x: -x[3]):
    print('   %-40s golden :%-5d-%-5d  %5d 行' % (n, a, b, c))
