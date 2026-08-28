# -*- coding: utf-8 -*-
"""兄弟呼叫封包：`screen_methods.py` 的 deep pass 只追自由函式，不追同類別兄弟方法。

這支補那個洞：一個方法只有在「它自己乾淨」**且**「它在類別內能到達的每一支都乾淨」
時才真的可翻。

用法: python sibling_closure.py <golden檔> <類別> <乾淨名單...>
      乾淨名單 = screen_methods.py 尾巴那一行印出來的那些。

輸出全 ASCII（原則 #9）：印方法名與判定，不印中文本體。

20260829 實測（golden HS_Function.cpp / TFormHS）：
  screen_methods.py 說 40 支乾淨 -> 算完兄弟封包剩 **34 支**，**6 支降級**。
  最具代表性的：`RecordLog_HS` 自己乾淨，却遞迴到達
  `UpDataToServerByFTP` / `UpDataToServer_KYEC`（**FTP 上傳，對外通道**）。
  在唯讀優先的規則下，那一支絕不能翻成 ACTIVE。

⚠ 為何不直接寫進 screen_methods.py：那支的 deep pass **刻意只跟一層**
  自由函式（見其原境註解：「兩層以上幾乎全樹染紅，篩選就沒有鑑別力」）。
  兄弟封包不同：它**被類別本身框住**（這裡是 65 支），不會擴散到全樹，
  所以可以算到不動點。兩者是不同的問題，故分兩支。
"""
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from goldenscan import load, code_only          # noqa: E402

GOLD = 'D:/HT9045/HT9011UC_Code_V3.33.906.0_20260618/'

fn, cls = sys.argv[1], sys.argv[2]
clean = set(sys.argv[3:])

lines, live = load(GOLD + fn)
DEF = re.compile(r'^[A-Za-z_][\w :\*&]*\b%s::(\w+)\s*\(' % cls)
defs = [(i, DEF.match(l).group(1)) for i, l in enumerate(lines) if DEF.match(l) and live[i]]
order = [i for i, _ in defs]
ALL = set(n for _, n in defs)


def span(a):
    nxt = next((k for k in order if k > a), len(lines))
    j = a
    while j < nxt and '{' not in code_only(lines[j]):
        j += 1
    d = 0
    k = j
    while k < nxt:
        c = code_only(lines[k])
        d += c.count('{') - c.count('}')
        if d == 0:
            return k
        k += 1
    return nxt - 1


CALL = re.compile(r'(?<![\w>.])([A-Za-z_]\w*)\s*\(')

# name -> set(sibling names it calls)
calls = {}
for a, nm in defs:
    b = span(a)
    body = '\n'.join(code_only(lines[k]) for k in range(a, b + 1))
    got = set(CALL.findall(body)) & ALL
    got.discard(nm)                      # 自我遞迴不算擴散
    calls[nm] = got

print('class %s: %d method definitions, %d given as clean' % (cls, len(ALL), len(clean)))
missing = clean - ALL
if missing:
    print('WARN: %d name(s) in the clean list have no definition here: %s'
          % (len(missing), ' '.join(sorted(missing))))
print('')

def closure(start):
    seen, stack = set(), [start]
    while stack:
        n = stack.pop()
        for m in calls.get(n, ()):
            if m not in seen:
                seen.add(m)
                stack.append(m)
    return seen


still, demoted = [], []
for nm in sorted(clean & ALL):
    reach = closure(nm)
    bad = sorted(m for m in reach if m not in clean)
    if bad:
        demoted.append((nm, bad))
    else:
        still.append(nm)

print('=== DEMOTED: clean on its own, but reaches a NON-clean sibling ===')
for nm, bad in demoted:
    print('  %-34s -> %s' % (nm, ' '.join(bad)))
print('  (none)' if not demoted else '')
print('=== STILL CLEAN after sibling closure: %d ===' % len(still))
print('  ' + ' '.join(still))
print('')
print('summary: %d given clean -> %d survive, %d demoted' % (len(clean & ALL), len(still), len(demoted)))
