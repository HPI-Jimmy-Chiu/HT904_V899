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

miss = [r for r in rows if not r[4]]
print('%s / %s' % (fn, cls))
print('  golden 方法 %d 個；port 已有 %d、缺 %d' % (len(rows), len(rows) - len(miss), len(miss)))
print('  缺的總行數: %d' % sum(r[3] for r in miss))
print()
for n, a, b, c, _ in sorted(miss, key=lambda x: -x[3]):
    print('   %-40s golden :%-5d-%-5d  %5d 行' % (n, a, b, c))
