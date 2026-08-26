# -*- coding: utf-8 -*-
"""兩訊號覆蓋率探針——**派工前用它把 census 的假缺口篩掉**。

為什麼存在
==========
20260827 一個晚上，`census.py` 的 per-file 缺口清單騙了主迴圈**四次**，
四種機制都不同、四次都靜默、四次都長得像待辦清單：

  1. **括號配對**：port `Command.cpp:1228` 的 `//... Part 2. {` 讓一支函式的 span
     吃掉 4,002 行、吞沒 61 支定義 -> 假缺 813 行。（見 `span_sanity.py`）
  2. **刻意寄放他檔**：golden `LaserSensorShuttle.cpp` 定義 `GetRowCol`，但 port
     因 `atester.cpp:144` 已有定義而只前向宣告 -> 假缺 93 行。
  3. **檔名比對**：`BarCode/BarCode_Sh1.cpp`＋`Sh2.cpp` 被判 no-mirror，於是走
     `missing_lines = gcode` 分支**整檔記為全缺、連名字都不比** -> 假缺 10,348 行，
     而實際 20/20 全已翻在 `BarCode_Shuttle{1,2}_*.cpp`（`BarCode_` 前綴自由函式）。
  4. **本體搬到別的 port 檔**：`ainarm2.cpp` 五支在 `aHotPlateSubstrate.cpp:1486-1530`、
     `csystem.cpp` 四支述詞在 `csystem_predicates.cpp:309-317`。

**census 的「缺」不是待辦清單。** 這支是派工前的過篩器。

判準（刻意設成兩個獨立訊號都要成立才給 STRICT credit）
=====================================================
對 golden 檔 F 的每一支函式 f：

  STRICT（下界）：某個 port 檔**同時**滿足
      (a) 定義了 bare name == f 的函式（或 `<前綴>_<name>`／`Class::name`），且
      (b) 在同一個檔裡引註了 `golden F:<line>`，而 line 落在 f 的 golden span 內。
    兩個獨立訊號互相佐證，且來自同一個檔 -> 過度計數的風險很低。

  LOOSE（上界）：只要全樹任何地方有同名定義就算。
    **會 over-credit**：`Init`／`Reset` 這類短名字會亂命中。

**只報區間，不報單一數字。** 兩者差距大就代表這個檔需要人工判讀。

已知不涵蓋的情況（誠實列出）
==========================
- 只有 STRICT 命中不代表「翻得對」，只代表「有人翻了並宣告了出處」。
- 引註也會出現在 **gate 註記**裡（「golden X:a-b 未翻譯」），所以 (b) 單獨不可信；
  必須配 (a)。
- 整支被 `#if 0` 包住的 port 定義**仍會**被算成命中——這支不做 gate 分析，
  gate 由 `census.py` 負責（它的 GATED 分類是對的）。

用法
====
    python tools/census/coverage_probe.py            # 全部無鏡射檔
    python tools/census/coverage_probe.py --form     # 只看表單軸
    python tools/census/coverage_probe.py BarCode/BarCode_Sh1.cpp   # 單檔逐支
"""
import io
import json
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, '..', '..'))
GOLDEN = os.path.abspath(os.path.join(ROOT, '..', 'HT9011UC_Code_V3.33.906.0_20260618'))

CITE = re.compile(r'golden\s+([A-Za-z0-9_/\\.]+\.cpp)\s*:\s*(\d+)\s*(?:-\s*(\d+))?', re.I)


def _helpers():
    src = io.open(os.path.join(HERE, 'census.py'), encoding='utf-8').read()
    ns = {'__name__': 'census_helpers', '__file__': os.path.join(HERE, 'census.py')}
    exec(compile(src[:src.index('def main(')], 'census_prefix', 'exec'), ns)
    return ns['functions'], ns['read']


def _strip(s):
    out, i, n = [], 0, len(s)
    while i < n:
        if s.startswith('/*', i):
            j = s.find('*/', i + 2)
            j = n if j < 0 else j + 2
            out.append(''.join(c if c == '\n' else ' ' for c in s[i:j]))
            i = j
        elif s.startswith('//', i):
            j = s.find('\n', i)
            j = n if j < 0 else j
            out.append(' ' * (j - i))
            i = j
        elif s[i] in ('"', "'"):
            q = s[i]
            j = i + 1
            while j < n and s[j] != q:
                j += 2 if s[j] == '\\' else 1
            j = min(j + 1, n)
            out.append(''.join(c if c == '\n' else ' ' for c in s[i:j]))
            i = j
        else:
            out.append(s[i])
            i += 1
    return ''.join(out)


def build_port_index(read):
    """回傳 (defs, cites)：
       defs  = { bare_name -> set(port 檔) }
       cites = { port 檔 -> { golden basename -> [(a, b), ...] } }
    """
    defs, cites = {}, {}
    dpat = re.compile(r'(?m)^[A-Za-z_][\w :\*&<>,~]*?\b([A-Za-z_]\w*)\s*\(')
    for dp, dn, fn in os.walk(ROOT):
        dn[:] = [d for d in dn if not d.startswith('build_') and d not in ('.git', '.svn')]
        for f in fn:
            if not f.endswith(('.cpp', '.h', '.c')):
                continue
            p = os.path.join(dp, f)
            rel = os.path.relpath(p, ROOT).replace('\\', '/')
            try:
                raw = read(p, False)
            except Exception:
                continue
            for m in CITE.finditer(raw):
                g = m.group(1).replace('\\', '/').split('/')[-1]
                a = int(m.group(2))
                b = int(m.group(3)) if m.group(3) else a
                if a <= b and b - a < 30000:
                    cites.setdefault(rel, {}).setdefault(g, []).append((a, b))
            for m in dpat.finditer(_strip(raw)):
                defs.setdefault(m.group(1), set()).add(rel)
    return defs, cites


def probe(rel, functions, read, defs, cites, verbose=False):
    gp = os.path.join(GOLDEN, rel)
    if not os.path.exists(gp):
        return None
    base = rel.replace('\\', '/').split('/')[-1]
    strict = loose = none_ = 0
    rowlines = []
    for k, (s, e, span, _g) in functions(read(gp, True)).items():
        bare = k.split('::')[-1]
        owners = set(defs.get(bare, ()))
        for n, ow in defs.items():
            if n != bare and (n.split('_', 1)[-1] == bare or n.endswith('_' + bare)):
                owners |= ow
        if not owners:
            none_ += span
            rowlines.append(('NONE  ', bare, span, ''))
            continue
        hit = ''
        for ow in owners:
            for (a, b) in cites.get(ow, {}).get(base, ()):
                if a <= e and b >= s:
                    hit = ow
                    break
            if hit:
                break
        if hit:
            strict += span
            rowlines.append(('STRICT', bare, span, hit))
        else:
            loose += span
            rowlines.append(('loose ', bare, span, sorted(owners)[0]))
    if verbose:
        for kind, bare, span, where in sorted(rowlines, key=lambda r: -r[2]):
            print('  %s %-42s %6d  %s' % (kind, bare, span, where))
    return strict, loose, none_


def main():
    functions, read = _helpers()
    args = [a for a in sys.argv[1:] if not a.startswith('--')]
    only_form = '--form' in sys.argv
    print('建立 port 索引中（會掃全樹，排除 build_*）...')
    defs, cites = build_port_index(read)
    print('  定義名 %d 個 / 有 golden 引註的 port 檔 %d 個' % (len(defs), len(cites)))

    if args:
        for rel in args:
            r = probe(rel, functions, read, defs, cites, verbose=True)
            if r is None:
                print('%s -- golden 無此檔' % rel)
                continue
            s, l, n = r
            print('%s: STRICT %d / loose %d / 無 %d  (span 行，非 code 行)' % (rel, s, l, n))
        return 0

    jp = os.path.join(ROOT, '_census_w35.json')
    if not os.path.exists(jp):
        print('先跑: python tools/census/census.py --json _census_w35.json')
        return 2
    recs = json.load(io.open(jp, encoding='utf-8'))
    sel = [r for r in recs if not r['mirrored'] and (r['form'] if only_form else True)]
    tot = [0, 0, 0]
    print('%-44s %8s %8s %8s %8s' % ('無鏡射檔', 'gcode', 'STRICT', 'loose', '無'))
    for r in sorted(sel, key=lambda r: -r['gcode']):
        out = probe(r['src'], functions, read, defs, cites)
        if out is None:
            continue
        s, l, n = out
        tot[0] += s
        tot[1] += l
        tot[2] += n
        print('%-44s %8d %8d %8d %8d' % (r['src'], r['gcode'], s, l, n))
    print('%-44s %8d %8d %8d %8d'
          % ('合計 %d 檔' % len(sel), sum(r['gcode'] for r in sel), tot[0], tot[1], tot[2]))
    print('')
    # 註：這裡刻意只用 ASCII。第一版在結尾印了一個 U+26A0，在 cp950 主控台上
    # 直接 UnicodeEncodeError 崩潰，讓整支工具回 exit 1 —— 分析其實已經跑完、
    # 表格也印出來了，但離開碼說它失敗。**輸出端的編碼會偽造成分析失敗。**
    print('NOTE: STRICT/loose/none are GOLDEN SPAN LINES (blank+comment included),')
    print('      NOT gcode code-lines. Different units -- do NOT divide them.')
    print('      Use this table to rank which files need human reading, then run')
    print('      single-file mode on each.')
    return 0


if __name__ == '__main__':
    sys.exit(main())
