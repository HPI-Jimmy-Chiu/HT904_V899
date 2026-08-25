# -*- coding: utf-8 -*-
"""通用抽取器：從任一 golden 檔抽指定方法，並列出 port 缺哪些識別字。

用法: python wave_extract.py <TAG> <golden檔名> <類別名> <方法名...>
產出: scratchpad/<TAG>_body.inc 與 <TAG>_sigs.txt
只讀，不寫 repo。
"""
import io
import re
import sys

S = ('C:/Users/JIMMYC~1/AppData/Local/Temp/claude/D--HT9045/'
     'd7001041-df73-42c5-96e5-4db4f6b22354/scratchpad')
GOLD_ROOT = 'D:/HT9045/HT9011UC_Code_V3.33.906.0_20260618/'
PORT_ROOT = 'D:/HT9045/HT9011UC_Cpp_V3.33.906.0/'

TAG, fn, cls = sys.argv[1], sys.argv[2], sys.argv[3]
names = sys.argv[4:]

g = io.open(GOLD_ROOT + fn, encoding='cp950', errors='strict').read().split('\n')
STR = re.compile(r'"(?:\\.|[^"\\])*"')
CHR = re.compile(r"'(?:\\.|[^'\\])*'")

def code_only(line):
    line = STR.sub('""', line)
    line = CHR.sub("''", line)
    i = line.find('//')
    return line if i < 0 else line[:i]

DEF = re.compile(r'^[A-Za-z_][\w :\*&]*\b%s::(\w+)\s*\(' % cls)
defs = [(i, DEF.match(l).group(1)) for i, l in enumerate(g) if DEF.match(l)]
at = {}
for i, n in defs:
    at.setdefault(n, i)
order = sorted(i for i, _ in defs)

def span(nm):
    i = at[nm]
    nxt = next((k for k in order if k > i), len(g))
    j = i
    while j < nxt and '{' not in code_only(g[j]):
        j += 1
    d = 0
    k = j
    while k < nxt:
        c = code_only(g[k])
        d += c.count('{') - c.count('}')
        if d == 0:
            return i, k
        k += 1
    raise AssertionError('%s 在下一個定義前沒有收尾' % nm)

out = []
body = ''
sigs = {}
for nm in names:
    a, b = span(nm)
    sigs[nm] = '%d-%d' % (a + 1, b + 1)
    print('%-38s golden :%d-%d (%d 行)' % (nm, a + 1, b + 1, b - a + 1))
    out.append('// AI(W906-%s) 20260825: golden %s:%d-%d, transcribed VERBATIM'
               % (TAG, fn, a + 1, b + 1))
    out.append('// (cp950 -> UTF-8) unless a deviation is marked inline.')
    out.extend(g[a:b + 1])
    out.append('')
    body += '\n'.join(g[a:b + 1]) + '\n'

io.open('%s/%s_body.inc' % (S, TAG), 'w', encoding='utf-8', newline='\n').write('\n'.join(out))
io.open('%s/%s_sigs.txt' % (S, TAG), 'w', encoding='utf-8', newline='\n').write(
    '\n'.join('%s\t%s' % (k, v) for k, v in sigs.items()))
print('抽出 %d 行（含 banner）' % len(out))

ids = set(re.findall(r'\b([A-Za-z_]\w*)\s*->', body))
ids |= set(re.findall(r'\b([A-Za-z_]\w*)\s*\[', body))
ids |= set(re.findall(r'\b([A-Za-z_]\w*)\s*\(', body))
noise = {'if', 'else', 'for', 'while', 'return', 'true', 'false', 'bool', 'int', 'void',
         'AnsiString', 'switch', 'case', 'sizeof', 'new', 'delete', 'double', 'char',
         'catch', 'try', 'float', 'long', 'unsigned', 'static', 'const'}
ids = {x for x in ids if x.isascii() and not x[0].isdigit()} - noise

# 20260825：原本只猜 `forms/f<檔名>.h`，那是表單波的慣例。非表單的類別
# （THGem 在 SECSGEM/uHGemEquipment.h）會猜成 `forms/fECSGEM/uHGemEquipment.h`
# 這種不存在的路徑，於是**該 header 宣告的成員全部被列成「缺」**，
# 清單塞滿偽陽性而看不出真的缺什麼。現在三個候選都試，並印出實際讀到哪幾個。
import os as _os
_base = fn[:-4]
cands = [fn, _base + '.h',
         'forms/f%s.h' % _os.path.basename(_base)[1:] if _os.path.basename(_base)[:1] in 'cu'
         else 'forms/f%s.h' % _os.path.basename(_base)]
ptok = set()
loaded = []
for p in dict.fromkeys(cands):
    try:
        ptok |= set(re.findall(r'\w+', io.open(PORT_ROOT + p, encoding='utf-8', errors='replace').read()))
        loaded.append(p)
    except FileNotFoundError:
        pass
print('  （讀到: %s）' % ('、'.join(loaded) if loaded else '無'))
missing = sorted(x for x in ids if x not in ptok)
print()
print('用到 %d 個識別字；port 的 %s + 對應 header 找不到的 %d 個:' % (len(ids), fn, len(missing)))
for m in missing:
    print('   ', m)
