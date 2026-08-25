# -*- coding: utf-8 -*-
"""FormShow 的擁有者報告：把缺失識別字分成「本表單的 widget」與「別人的」。

用法: python owner_report.py <TAG> <golden header> <port header> <port cpp>
"""
import io
import re
import sys
import collections

S = ('C:/Users/JIMMYC~1/AppData/Local/Temp/claude/D--HT9045/'
     'd7001041-df73-42c5-96e5-4db4f6b22354/scratchpad')
GOLD = 'D:/HT9045/HT9011UC_Code_V3.33.906.0_20260618/'
PORT = 'D:/HT9045/HT9011UC_Cpp_V3.33.906.0/'

TAG, ghname, phname, pcname = sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4]
body = io.open('%s/%s_body.inc' % (S, TAG), encoding='utf-8').read()

owned = collections.defaultdict(set)
for m in re.finditer(r'\b([A-Za-z_]\w*)\s*(?:\[[^\]]*\])?\s*->\s*([A-Za-z_]\w*)', body):
    owned[m.group(2)].add(m.group(1))

gh = io.open(GOLD + ghname, encoding='cp950', errors='replace').read().split('\n')
gt = {}
for i, l in enumerate(gh):
    m = re.match(r'\s*(T\w+)\s*\*?\s*([\w, \[\]]+);', l)
    if m:
        for nm in re.findall(r'[A-Za-z_]\w*', m.group(2)):
            gt.setdefault(nm, (m.group(1), i + 1))

ptok = set()
for p in (phname, pcname):
    ptok |= set(re.findall(r'\w+', io.open(PORT + p, encoding='utf-8', errors='replace').read()))

ids = set(re.findall(r'\b([A-Za-z_]\w*)\s*(?:\[[^\]]*\])?\s*->', body))
missing = sorted(x for x in ids if x not in ptok)

mine, foreign, unknown = [], [], []
for n in missing:
    owners = sorted(owned.get(n, set()))
    if n in gt:
        mine.append((n, gt[n][0], gt[n][1]))
    elif owners:
        foreign.append((n, owners))
    else:
        unknown.append(n)

print('%s：缺 %d 個以 -> 引用的識別字' % (TAG, len(missing)))
print()
print('=== 本表單自己的 widget（golden header 查得到型別）: %d 個 ===' % len(mine))
print('型別分布:', dict(collections.Counter(t for _, t, _ in mine)))
print()
print('=== 屬於別的物件、不可加進本表單: %d 個 ===' % len(foreign))
for n, o in foreign:
    print('   %-28s <- %s' % (n, ','.join(o)))
print()
print('=== 裸引用（全域或函式，需 include 或 gate）: %d 個 ===' % len(unknown))
print('   ' + ', '.join(unknown))

io.open('%s/%s_mine.txt' % (S, TAG), 'w', encoding='utf-8', newline='\n').write(
    '\n'.join('%s\t%s\t%d' % (n, t, ln) for n, t, ln in mine))
print()
print('本表單 widget 清單已寫到 %s_mine.txt' % TAG)
