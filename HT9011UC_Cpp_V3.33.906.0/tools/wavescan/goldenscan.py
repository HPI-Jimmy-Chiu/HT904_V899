# -*- coding: utf-8 -*-
"""golden 掃描的共用前處理 —— 兩個已付代價的洞都在這裡補起來。

洞 1（20260825 FW-CFG-W2）：行尾 `//` 註解裡的 `}` 會騙過括號配對。
洞 2（20260825 FW-TF-W1）：整段包在 `/* */` 裡的函式會被當成真函式抽出來，
      而 golden 根本沒有編譯它們（cTemperFrom.cpp:1904-1915 / :1941-1948 就是）。

用法：
    from goldenscan import load, code_only
    lines, live = load(path)      # live[i] 為 False 代表該行在 /* */ 區塊註解內
"""
import io
import re

STR = re.compile(r'"(?:\\.|[^"\\])*"')
CHR = re.compile(r"'(?:\\.|[^'\\])*'")


def code_only(line):
    """剝掉字串常值與行註解，只留參與語法的部分。"""
    line = STR.sub('""', line)
    line = CHR.sub("''", line)
    i = line.find('//')
    return line if i < 0 else line[:i]


def load(path, encoding='cp950'):
    """讀 golden 檔，回傳 (lines, live)。

    live[i] == False 表示第 i 行落在 /* */ 區塊註解內（含起訖行本身只要
    該行的程式碼部分被註解吃掉就算）。字串裡的 /* 不會誤判。
    """
    lines = io.open(path, encoding=encoding, errors='replace').read().split('\n')
    live = []
    in_block = False
    for raw in lines:
        s = STR.sub('""', raw)
        s = CHR.sub("''", s)
        line_live = not in_block
        i = 0
        while i < len(s) - 1:
            two = s[i:i + 2]
            if not in_block and two == '//':
                break
            if not in_block and two == '/*':
                in_block = True
                if i == 0:
                    line_live = False
                i += 2
                continue
            if in_block and two == '*/':
                in_block = False
                i += 2
                # 收尾之後同一行還有東西才算 live
                rest = s[i:].strip()
                if rest and not rest.startswith('//'):
                    line_live = True
                continue
            i += 1
        if in_block:
            line_live = False
        live.append(line_live)
    return lines, live


if __name__ == '__main__':
    import sys
    p = sys.argv[1] if len(sys.argv) > 1 else \
        'D:/HT9045/HT9011UC_Code_V3.33.906.0_20260618/cTemperFrom.cpp'
    lines, live = load(p)
    dead = [i + 1 for i, ok in enumerate(live) if not ok]
    print('%s：%d 行，其中 %d 行在 /* */ 區塊註解內' % (p.split('/')[-1], len(lines), len(dead)))
    # 印出落在區塊註解裡的函式定義
    DEF = re.compile(r'^[A-Za-z_][\w :\*&]*\bTf\w+::(\w+)\s*\(')
    for i, l in enumerate(lines):
        if DEF.match(l) and not live[i]:
            print('   :%-5d 定義在區塊註解內 -> %s' % (i + 1, DEF.match(l).group(1)))
