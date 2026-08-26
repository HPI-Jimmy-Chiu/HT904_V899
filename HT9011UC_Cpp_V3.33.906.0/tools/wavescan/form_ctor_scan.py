# -*- coding: utf-8 -*-
"""港樹裡表單全域到底有沒有人建？"""
import io
import os
import re
import sys

sys.stdout.reconfigure(encoding='utf-8', errors='replace')
ROOT = 'D:/HT9045/HT9011UC_Cpp_V3.33.906.0'
GOLD = 'D:/HT9045/HT9011UC_Code_V3.33.906.0_20260618/HT9045.cpp'

# golden 的 CreateForm 清單 = 權威的「該存在的表單全域」
g = io.open(GOLD, encoding='cp950', errors='replace').read()
pairs = re.findall(r'CreateForm\(__classid\((\w+)\)\s*,\s*&(\w+)\)', g)
print('golden HT9045.cpp CreateForm: %d 個 (類別, 全域)' % len(pairs))

globals_ = [v for _, v in pairs]
pat = re.compile(r'\b(%s)\s*=\s*new\b' % '|'.join(map(re.escape, globals_)))
made = {}
for dp, dn, fn in os.walk(ROOT):
    dn[:] = [d for d in dn if not d.startswith('build_')
             and d not in ('.git', '.svn', 'third_party', 'docs')]
    for f in fn:
        if not f.endswith(('.cpp', '.h')):
            continue
        p = os.path.join(dp, f)
        try:
            s = io.open(p, encoding='utf-8', errors='replace').read()
        except Exception:
            continue
        for i, l in enumerate(s.split('\n'), 1):
            if l.lstrip().startswith('//'):
                continue
            m = pat.search(l)
            if m:
                rel = os.path.relpath(p, ROOT).replace(os.sep, '/')
                made.setdefault(m.group(1), []).append('%s:%d' % (rel, i))

print('\n=== 有人建的（%d / %d）===' % (len(made), len(globals_)))
for k in sorted(made):
    print('  %-22s %s' % (k, ', '.join(made[k][:2])))
never = [v for v in globals_ if v not in made]
print('\n=== 全樹無人建立，恆為 NULL（%d 個）===' % len(never))
for i in range(0, len(never), 6):
    print('  ' + '  '.join('%-18s' % x for x in never[i:i + 6]))
