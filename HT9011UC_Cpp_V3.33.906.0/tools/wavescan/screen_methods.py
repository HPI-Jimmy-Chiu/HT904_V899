# -*- coding: utf-8 -*-
"""逐方法安全篩選：翻譯前先問「這個函式會不會動到真機／寫檔／叫別的表單做事」。

本 session 已經有五次是靠人工讀 golden 才擋下來的（cTemperFrom 三個、
chkHeaterClick、FormClose）。這支把那個動作變成每波開工的第一步。

用法: python screen_methods.py <golden檔> <類別> <方法名...>
      不給方法名就掃該類別所有方法。
"""
import io
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from goldenscan import load, code_only

GOLD = 'D:/HT9045/HT9011UC_Code_V3.33.906.0_20260618/'

RISK = [
    ('IO/繼電器', r'\bSW\[[^\]]+\]\s*\.\s*OnOff\s*\(|\bDoSw\w+\s*\('),
    ('馬達', r'\bMotorMove\b|\bStopAllMotor\b|\bSetServo\b|\bHome\s*\('),
    ('氣缸', r'\bCylinder\[[^\]]+\]\s*\.\s*(On|Off|Move)\b'),
    ('硬體指令', r'Send_Command\w*\s*\(|\bWriteCommand\b|\bSendTCPIPCommand\b'),
    # 20260825 二次加強：第一版漏了 CopyFile 這一族，於是 ReadConfigStandard
    # 被判成「乾淨」——它其實會 CopyFile(config_Standard.ini -> config.ini)，
    # 直接覆蓋產線設定檔。檔案搬移／刪除與寫入同級。
    ('寫檔/寫ini', r'\bWriteIniData\s*\(|\bSaveToFile\s*\(|\bWriteFile\s*\(|\bCreateFile\s*\('
                   r'|SaveEventLogAutoSaveInfo\s*\(|\bCopyFile\w*\s*\(|\bMoveFile\w*\s*\('
                   r'|\bDeleteFile\w*\s*\(|\bRemoveDir\w*\s*\(|\bRenameFile\s*\('),
    ('緒/參數初始化', r'\bInit\w*ThreadParameter\s*\('),
    ('警報/對話框', r'\bShowErrorMessage\s*\(|\bShowMyMessage\w*\s*\(|->ShowModal\s*\('),
    ('golden 自帶安全標記', r'AI\(safety'),
    # 20260825 加：前一版漏了這一類，btnN31_Manual/btnN25_3_Manual/btnN35_Test/
    # btnA71Manually 都被判成「乾淨」，實際上分別會觸發 FTP 上傳、送 EventLog 命令、
    # 存檔上傳、批次複製 recipe。跨表單呼叫一律要人看過。
    ('跨表單呼叫', r'\bf[A-Z]\w+\s*->\s*\w+\s*\('),
    ('送命令/上傳', r'\bSendCommand\w*\s*\(|\bUpload\w*\s*\(|\w+_Upolad\s*\(|RunBatch\w*\s*\('),
]

fn, cls = sys.argv[1], sys.argv[2]
want = sys.argv[3:]

lines, live = load(GOLD + fn)
DEF = re.compile(r'^[A-Za-z_][\w :\*&]*\b%s::(\w+)\s*\(' % cls)
defs = [(i, DEF.match(l).group(1)) for i, l in enumerate(lines) if DEF.match(l) and live[i]]
order = [i for i, _ in defs]


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


print('%-38s %5s  %s' % ('方法', '行數', '風險'))
print('-' * 96)
clean = []
for a, name in defs:
    if want and name not in want:
        continue
    b = span(a)
    body = '\n'.join(l for i, l in enumerate(lines[a:b + 1]) if not l.strip().startswith('//'))
    hits = [k for k, p in RISK if re.search(p, body)]
    mark = '、'.join(hits) if hits else '—'
    print('%-38s %5d  %s' % (name, b - a + 1, mark))
    if not hits:
        clean.append(name)

print()
print('乾淨（無上列任何風險）: %d 個' % len(clean))
print(' '.join(clean))
