# -*- coding: utf-8 -*-
"""表單 bootstrap 前置量測（第三版：不用 alternation）。

第一版：對 118 個表單各跑 4 次全樹掃描 = 472 趟，逾時。
第二版：單趟索引，但 RX_DEF 兩個群組各 115 個 alternation
        -> 最壞每位置試 115x115 組合，跑 19.5 分鐘仍未完成（實測仍在燒 CPU，非卡住）。
第三版：先用通用形狀抓出「任何 class / 任何 T* g / 任何 g = new」，
        再用 set 查表判斷是不是我們要的名字。零 alternation。
"""
import io, os, re, sys, time
sys.stdout.reconfigure(encoding='utf-8', errors='replace')

ROOT = 'D:/HT9045/HT9011UC_Cpp_V3.33.906.0'
GOLD = 'D:/HT9045/HT9011UC_Code_V3.33.906.0_20260618/HT9045.cpp'
SKIP = {'.git', '.svn', 'third_party', 'docs'}
t0 = time.time()

RX_COMMENT = re.compile(r'/\*.*?\*/|//[^\n]*', re.S)
def blank(m):
    return '\n'.join(' ' * len(s) for s in m.group().split('\n'))

g = io.open(GOLD, encoding='cp950', errors='replace').read()
pairs = re.findall(r'CreateForm\(__classid\((\w+)\)\s*,\s*&(\w+)\)', g)
CLS, GLB = {c for c, _ in pairs}, {v for _, v in pairs}
print('golden CreateForm: %d 對（%d 相異類別 / %d 相異全域）' % (len(pairs), len(CLS), len(GLB)))

RX_CLASS = re.compile(r'^\s*class\s+(\w+)\s*(?::|\{|$)', re.M)
RX_DEF   = re.compile(r'^\s*(?:extern\s+)?(\w+)\s*\*\s*(\w+)\s*(?:=[^;]*)?;', re.M)
RX_NEW   = re.compile(r'\b(\w+)\s*=\s*new\s+(\w+)')

hc, hd, hn = {}, {}, {}
nf = 0
for dp, dn, fn in os.walk(ROOT):
    dn[:] = [d for d in dn if d not in SKIP and not d.startswith('build_')]
    for f in fn:
        if not f.endswith(('.cpp', '.h')):
            continue
        p = os.path.join(dp, f)
        rel = os.path.relpath(p, ROOT).replace(os.sep, '/')
        try:
            b = RX_COMMENT.sub(blank, io.open(p, encoding='utf-8', errors='replace').read())
        except Exception:
            continue
        nf += 1
        for m in RX_CLASS.finditer(b):
            if m.group(1) in CLS:
                hc.setdefault(m.group(1), []).append('%s:%d' % (rel, b.count('\n', 0, m.start()) + 1))
        for m in RX_DEF.finditer(b):
            if m.group(1) in CLS and m.group(2) in GLB:
                hd.setdefault(m.group(2), []).append('%s:%d' % (rel, b.count('\n', 0, m.start()) + 1))
        for m in RX_NEW.finditer(b):
            if m.group(1) in GLB:
                hn.setdefault(m.group(1), []).append('%s:%d' % (rel, b.count('\n', 0, m.start()) + 1))

print('掃描 %d 個 port 檔，耗時 %.1f 秒\n' % (nf, time.time() - t0))

built, cand, nodef, nocls = [], [], [], []
seen = set()
for cls, glb in pairs:
    if glb in seen:
        continue
    seen.add(glb)
    c, d, n = hc.get(cls), hd.get(glb), hn.get(glb)
    row = (glb, cls, c, d, n)
    (built if n else nocls if not c else cand if d else nodef).append(row)

print('=' * 76)
print('  D 已建構（不用管）                   : %3d' % len(built))
print('  ★ 有類別 + 有全域定義 + 沒人建 候選  : %3d' % len(cand))
print('  有類別、全域沒定義（要先補定義）      : %3d' % len(nodef))
print('  類別根本還沒翻（bootstrap 無解）      : %3d' % len(nocls))
print('=' * 76)
print('  合計 %d（相異全域 %d）\n' % (len(built)+len(cand)+len(nodef)+len(nocls), len(CLS)))

print('★ 可直接 bootstrap 的候選（%d）：' % len(cand))
for glb, cls, c, d, _ in cand:
    print('   %-22s %-22s class@%-36s def@%s' % (glb, cls, c[0], d[0]))
print('\n有類別但全域沒定義（%d）：' % len(nodef))
for glb, cls, c, d, _ in nodef:
    print('   %-22s %-22s class@%s' % (glb, cls, c[0]))
print('\n類別還沒翻（%d）：' % len(nocls))
nm = sorted(r[0] for r in nocls)
for i in range(0, len(nm), 6):
    print('   ' + '  '.join('%-18s' % x for x in nm[i:i+6]))
