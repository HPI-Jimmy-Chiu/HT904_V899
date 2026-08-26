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

# 20260826：本檔原本自帶一份不懂 /* */ 的 code_only，於是**整支包在區塊註解裡的
# 函式會被列成缺口**——golden 根本沒編譯它們，照著翻就是把停用的碼加進 port
# （README 的「陷阱二」，抽取器早就修過，這支漏了）。
# 實例：THGem::SaveTCPIPRecieveData（golden uHGemEquipment.cpp:6958 起整段包在
# /* */ 內，54 行），一度出現在「真正缺 16 支」的清單裡。
# screen_methods.py 早就用 goldenscan.load() 所以正確地跳過它——**兩支工具對同一個
# 方法給出相反答案**才被抓到。現在共用同一份實作。
import os as _os
sys.path.insert(0, _os.path.dirname(_os.path.abspath(__file__)))
from goldenscan import load as _load, code_only

_lines, _live = _load(GOLD_ROOT + fn)

DEF = re.compile(r'^[A-Za-z_][\w :\*&]*\b%s::(\w+)\s*\(' % cls)
defs = [(i, DEF.match(l).group(1))
        for i, l in enumerate(g) if DEF.match(l) and i < len(_live) and _live[i]]
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
SAME_OTHER_FILE = {}   # 同一個類別但定義在別的 port 檔（見下方說明）
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
                loc = '%s:%s' % (os.path.relpath(p, PORT_ROOT).replace('\\', '/'),
                                 t[:m2.start()].count('\n') + 1)
                if m2.group(1) == cls:
                    # 20260826：**同一個類別、不同的 port 檔**。這一格原本被
                    # `!= cls` 過濾掉了，於是同樣被讀成「缺」——那是上面那個
                    # 缺陷的變種，不是「搬到別的類別」而是「同類別拆成兩個檔」。
                    # 實例：TfSetup 橫跨 forms/fSetup.cpp（ht9045_forms 目標）與
                    # 根目錄 cSetUp.cpp（ht9045_sm 目標），這個拆法是刻意的
                    # （見 forms/fSetup.h:165-195 的 link-layer 說明），
                    # 而 XPitchKeyPress / RadioButton1KeyDown 早就翻在前者。
                    # 這種**根本不是缺口**，直接算成已翻。
                    SAME_OTHER_FILE.setdefault(m2.group(2), []).append(loc)
                else:
                    ELSE.setdefault(m2.group(2), []).append('%s::  %s' % (m2.group(1), loc))

miss = [r for r in rows if not r[4] and r[0] not in ELSE and r[0] not in SAME_OTHER_FILE]
same = [r for r in rows if not r[4] and r[0] in SAME_OTHER_FILE]
moved = [r for r in rows if not r[4] and r[0] not in SAME_OTHER_FILE and r[0] in ELSE]
print('%s / %s' % (fn, cls))
print('  golden 方法 %d 個；port 同檔同類別已有 %d'
      % (len(rows), len(rows) - len(miss) - len(moved) - len(same)))
print('  同類別但在別的 port 檔（已翻，非缺口）: %d 個 / %d 行' % (len(same), sum(r[3] for r in same)))
print('  在別的 port 類別（已翻，非缺口）: %d 個 / %d 行' % (len(moved), sum(r[3] for r in moved)))
print('  真正缺: %d 個 / %d 行' % (len(miss), sum(r[3] for r in miss)))
if same:
    print()
    print('--- 同類別、別的檔 ---')
    for n, a, b, c, _ in sorted(same, key=lambda x: -x[3]):
        print('   %-40s golden :%-5d %4d 行  -> %s' % (n, a, c, SAME_OTHER_FILE[n][0]))
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
