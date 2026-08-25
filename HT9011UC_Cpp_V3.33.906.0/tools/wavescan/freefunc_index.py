# -*- coding: utf-8 -*-
"""golden 全樹自由函式索引：名稱 -> (檔, 起行, 迄行)。

存在的理由（20260825，第三次付代價）：
`screen_methods.py` 只掃 handler **自己的本體**。於是
`TfConfiguration::btnSetToTechClick`（golden cConfiguration.cpp:5978）被判成乾淨——
它的本體只有一行 `SetOffsetToTech();`，而那支自由函式在 cinitial.cpp:13925，
會把目前 offset 累加進 `Tech.*`（手臂與 shuttle 的教導座標，馬達實際會走到的位置），
golden 自己還要先跳 MessageDlg 要人確認。

前兩個缺口是同一種病的別的器官：
  #1 跨表單呼叫（btnN31_Manual 那批 -> FTP 上傳 / EventLog 命令 / recipe 複製）
  #2 CopyFile（ReadConfigStandard -> 覆蓋產線 config.ini）
  #3 自由函式（本檔）

只跟一層。兩層以上會把幾乎所有東西都染紅而失去篩選力；一層已經抓到本波這隻。
"""
import io
import os
import re
import sys

GOLD = 'D:/HT9045/HT9011UC_Code_V3.33.906.0_20260618/'
CACHE = os.path.join(os.path.dirname(os.path.abspath(__file__)), '.freefunc_cache.tsv')

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from goldenscan import load, code_only

# 檔案層、非成員的函式定義：回傳型別 + 名稱 + '(' 在第 0 欄，且不含 '::'
DEF = re.compile(r'^([A-Za-z_][\w\*& ]*?[\w\*&])\s+([A-Za-z_]\w*)\s*\(')
NOTFN = {'if', 'for', 'while', 'switch', 'return', 'else', 'do', 'catch'}


def _span(lines, live, a):
    """從定義行往下找本體結束。用 code_only 數括號（陷阱一）。"""
    j = a
    while j < len(lines) and '{' not in code_only(lines[j]):
        if ';' in code_only(lines[j]):
            return None                      # 只是宣告，不是定義
        j += 1
        if j - a > 6:
            return None
    if j >= len(lines):
        return None
    d = 0
    for k in range(j, min(j + 4000, len(lines))):
        if not live[k]:
            continue
        c = code_only(lines[k])
        d += c.count('{') - c.count('}')
        if d == 0:
            return k
    return None


def build(verbose=False):
    idx = {}
    for fn in sorted(os.listdir(GOLD)):
        if not fn.endswith('.cpp'):
            continue
        try:
            lines, live = load(GOLD + fn)
        except Exception:
            continue
        for i, l in enumerate(lines):
            if not live[i] or '::' in l or l.startswith(' ') or l.startswith('\t'):
                continue
            m = DEF.match(l)
            if not m or m.group(2) in NOTFN or m.group(1).split()[-1].strip('*& ') in NOTFN:
                continue
            b = _span(lines, live, i)
            if b is None:
                continue
            idx.setdefault(m.group(2), (fn, i + 1, b + 1))
        if verbose:
            print('%-32s %d' % (fn, len(idx)))
    return idx


def get():
    """讀快取，沒有就建。快取過期與否由呼叫端決定（golden 是唯讀樹）。"""
    if os.path.exists(CACHE):
        out = {}
        for l in io.open(CACHE, encoding='utf-8'):
            p = l.rstrip('\n').split('\t')
            if len(p) == 4:
                out[p[0]] = (p[1], int(p[2]), int(p[3]))
        return out
    idx = build()
    with io.open(CACHE, 'w', encoding='utf-8') as f:
        for k, v in sorted(idx.items()):
            f.write('%s\t%s\t%d\t%d\n' % (k, v[0], v[1], v[2]))
    return idx


def body(name, idx=None):
    """回傳該自由函式的本體字串；找不到回 None。"""
    idx = idx or get()
    if name not in idx:
        return None
    fn, a, b = idx[name]
    lines, _ = load(GOLD + fn)
    return '\n'.join(lines[a - 1:b])


if __name__ == '__main__':
    if len(sys.argv) > 1:
        for n in sys.argv[1:]:
            i = get()
            print('%-28s %s' % (n, i.get(n, '(索引裡沒有)')))
    else:
        if os.path.exists(CACHE):
            os.remove(CACHE)
        idx = get()
        print('索引 %d 個自由函式 -> %s' % (len(idx), CACHE))
