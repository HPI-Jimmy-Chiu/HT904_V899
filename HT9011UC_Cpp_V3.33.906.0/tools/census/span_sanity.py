# -*- coding: utf-8 -*-
"""census 的括號配對健全性檢查——**選波次標的前必跑**。

為什麼存在
==========
`census.py` 的 `functions()` 用括號平衡切函式 span，但它**不剝註解與字串字面值**。
所以一個藏在 `//` 註解裡的 `{` 會讓深度永遠多 1，那支函式的 span 就一路吃下去，
把後面數十支定義**靜默地**吞掉——被吞掉的名字不在 `pnames` 裡，於是 census 把
它們算成「golden 有、port 沒有」＝**未翻譯**。

20260827 的實例（本檔的起因）：
    port `Command.cpp:1228`  //2013.01.24 Q_Q TSMC GPIB COMMAND Part 2. {
    這一個註解裡的 `{` 讓 `TfMain::PERSITETemperatureStrings` 的 span 變成
    1218..5219（4,002 行），吞掉 61 支定義，於是 census 回報 `Command.cpp`
    缺 43 支 / 813 行——**而那 43 支早在 20260817 的 FW-3 Wave A 就翻完了**。
    主迴圈照著那份清單派了一整波翻譯 agent 出去，agent 查完回報「無事可做」。
    那個 `{` 還是忠實翻譯的產物：golden 自己就這樣寫。

**census 的「缺」不是待辦清單。** 這支工具就是用來在派工前把假缺口篩掉。

做法
====
同一份文字跑兩次 census 自己的 `functions()`：一次原樣、一次先把註解與字串
換成等長空白（行號不變）。兩者的差就是註解裡的括號造成的損害。
**刻意不修改 `census.py`**——改 `functions()` 會讓所有歷史數字失去可比性，
而且上一次「修好」census 的嘗試算出過 parked > gcode 與 35,289 行的假 credit。
這支是唯讀的旁證工具，不動權威量尺。

用法
====
    python tools/census/span_sanity.py            # 全樹掃描（port + golden）
    python tools/census/span_sanity.py Command.cpp  # 單檔，列出被吞的定義名
"""
import io
import json
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.abspath(os.path.join(HERE, '..', '..'))
GOLDEN = os.path.abspath(os.path.join(ROOT, '..', 'HT9011UC_Code_V3.33.906.0_20260618'))


def _census_helpers():
    """只 exec `def main(` 之前的部分——直接 import 會跑 main()。"""
    src = io.open(os.path.join(HERE, 'census.py'), encoding='utf-8').read()
    ns = {'__name__': 'census_helpers',
          '__file__': os.path.join(HERE, 'census.py')}
    exec(compile(src[:src.index('def main(')], 'census_prefix', 'exec'), ns)
    return ns['functions'], ns['read']


def strip_comments(s):
    """註解與字串字面值換成等長空白，換行保留 -> 行號完全不變。"""
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


def first_comment_brace(text):
    """回傳第一個「註解裡含括號」的行號（1-based），沒有就 None。"""
    for i, ln in enumerate(text.split('\n')):
        if '//' in ln:
            tail = ln.split('//', 1)[1]
            if '{' in tail or '}' in tail:
                return i + 1
    return None


def scan_one(path, golden, functions, read):
    text = read(path, golden)
    a = functions(text)
    b = functions(strip_comments(text))
    return a, b, text


def main():
    functions, read = _census_helpers()

    if len(sys.argv) > 1:
        rel = sys.argv[1]
        for label, base, gold in (('port', ROOT, False), ('golden', GOLDEN, True)):
            p = os.path.join(base, rel)
            if not os.path.exists(p):
                print('%-7s %s -- 不存在' % (label, rel))
                continue
            a, b, text = scan_one(p, gold, functions, read)
            print('%-7s %s: census 抽 %d 支 / 剝註解後 %d 支 / 少抽 %d'
                  % (label, rel, len(a), len(b), len(b) - len(a)))
            fb = first_comment_brace(text)
            if fb:
                print('        第一個「註解含括號」在 :%d' % fb)
            lost = sorted(set(b) - set(a), key=lambda k: b[k][0])
            for k in lost:
                print('        被吞: %-44s :%d-%d' % (k, b[k][0], b[k][1]))
        return 0

    jp = os.path.join(ROOT, '_census_w35.json')
    if not os.path.exists(jp):
        print('先跑: python tools/census/census.py --json _census_w35.json')
        return 2
    recs = json.load(io.open(jp, encoding='utf-8'))

    for label, base, gold in (('port', ROOT, False), ('golden', GOLDEN, True)):
        rows = []
        for r in recs:
            p = os.path.join(base, r['src'])
            if not os.path.exists(p):
                continue
            a, b, _ = scan_one(p, gold, functions, read)
            if len(b) > len(a):
                rows.append((r['src'], len(a), len(b), len(b) - len(a),
                             r['missing_funcs'], r['missing_lines']))
        rows.sort(key=lambda x: -x[3])
        print('=== %s 側：%d 檔受影響，合計少抽 %d 支定義 ==='
              % (label, len(rows), sum(x[3] for x in rows)))
        print('%-42s %7s %8s %6s %8s %10s'
              % ('file', 'census', 'stripped', 'lost', 'miss_fn', 'miss_line'))
        for row in rows:
            print('%-42s %7d %8d %6d %8d %10d' % row)
        if label == 'port':
            hot = [r for r in rows if r[4] > 0]
            print('  ** 其中 miss_fn > 0 的才是「假缺口」，派工前必須逐檔複驗: %s'
                  % ([r[0] for r in hot] or '無'))
        print('')
    return 0


if __name__ == '__main__':
    sys.exit(main())
