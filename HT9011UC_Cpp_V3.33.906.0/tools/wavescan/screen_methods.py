# -*- coding: utf-8 -*-
"""逐方法安全篩選：翻譯前先問「這個函式會不會動到真機／寫檔／叫別的表單做事」。

本 session 已經有五次是靠人工讀 golden 才擋下來的（cTemperFrom 三個、
chkHeaterClick、FormClose）。這支把那個動作變成每波開工的第一步。

用法: python screen_methods.py <golden檔> <類別> <方法名...>
      不給方法名就掃該類別所有方法。

⚠⚠ **它不查什麼（三個已知盲點，引用本工具結果前先讀）**

1. **只查安全軸，不查可達軸**。「乾淨」不代表本樹連得到。
   20260828 實例：`CalculateOEEReport` 在本工具下乾淨，實際三次解參考
   `fObserver`（golden ProductionInfo.cpp:805/:806/:813），而 `fObserver` 在 ht9045_sm。
   見 DEVLOG 20260828 XII。

2. **deep pass 只追自由函式，不追同類別的兄弟方法**。
   本檔 `FFIDX = freefunc_index.get()` / `freefunc_index.body(nm, FFIDX)`
   解的是**自由函式索引**；`TFormHS::RecordESDLog_HS` 這種成員方法不在裡面。
   **後果：一個「乾淨」的方法若呼叫了有風險的兄弟方法，會被讀成乾淨。**
   20260829 實例：`RecordLog_HS`（golden HS_Function.cpp:584-645）被列為乾淨，
   它的本體卻呼叫 `RecordESDLog_HS(...)`——而那一支**不在**乾淨名單裡。
   -> **用 `sibling_closure.py` 算兄弟呼叫的封包（closure）**（同目錄，20260829 新增）：
      `python sibling_closure.py <golden檔> <類別> <本支印出的乾淨名單...>`
      它將每一支在類別內能到達的兄弟遞迴展開，碰到一支不在乾淨名單裡就降級。
      20260829 實測（TFormHS）：**40 乾淨 -> 34 存活、6 降級**；
      其中 `RecordLog_HS` 遞迴到達 `UpDataToServerByFTP`（**FTP 上傳，對外通道**）。

3. **名字不是證據**。`Get*` 可以寫全域設定（PI2 那則 banner 的
   `GetBinTraySetting`），`Record*Log*` 也可以不寫檔。
   名字可疑的，**開 golden 看本體**，不要只看本工具的一行結論。
"""
import io
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from goldenscan import load, code_only
import freefunc_index

GOLD = 'D:/HT9045/HT9011UC_Code_V3.33.906.0_20260618/'

RISK = [
    ('IO/繼電器', r'\bSW\[[^\]]+\]\s*\.\s*OnOff\s*\(|\bDoSw\w+\s*\('),
    ('馬達', r'\bMotorMove\b|\bStopAllMotor\b|\bSetServo\b|\bHome\s*\('),
    ('氣缸', r'\bCylinder\[[^\]]+\]\s*\.\s*(On|Off|Move)\b'),
    ('硬體指令', r'Send_Command\w*\s*\(|\bWriteCommand\b|\bSendTCPIPCommand\b'),
    # 20260825 二次加強：第一版漏了 CopyFile 這一族，於是 ReadConfigStandard
    # 被判成「乾淨」——它其實會 CopyFile(config_Standard.ini -> config.ini)，
    # 直接覆蓋產線設定檔。檔案搬移／刪除與寫入同級。
    # CreateFile 在 Win32 讀寫共用，所以只認寫模式；不然每個讀檔函式都會亮紅燈，
    # deep pass 會失去鑑別力（實例：ReadLastDataFile 三個 CreateFile 全是
    # GENERIC_READ/OPEN_EXISTING，cprod.cpp:1642/1654/1675）。
    ('寫檔/寫ini', r'\bWriteIniData\w*\s*\(|\bSaveToFile\s*\(|\bWriteFile\s*\('
                   r'|\bCreateFile\s*\((?![^)]*GENERIC_READ)'
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
    # 20260829 四次加強：`HandlerClientSocketRead`（golden HS_Function.cpp:4508-4557）
    # 差一點被判成乾淨。它解析進來的 socket 資料並依內容分支，然後：
    #   fMain->iResetCurrent=3;                     <- 跨表單**欄位寫入**
    #   EventReport(SECS_EVENT.PowerSavingStart);   <- **對外 SECS/GEM 事件回報**
    # 舊的「跨表單呼叫」樣式只抓**呼叫**（末尾是左括弧），
    # 抓不到純欄位費值；`EventReport` 也不在任何樣式裡。
    # 它最後是被 `sibling_closure.py` 拉下來的（恰巧也呼叫了一支髒兄弟）
    # ——**那是運氣，不是覆蓋**。
    ('跨表單寫欄位', r'\bf[A-Z]\w+\s*->\s*\w+\s*=(?!=)'),
    ('SECS/GEM 事件回報', r'\bEventReport\s*\(|\bSECS_EVENT\b'),
    # 20260829 五次加強：讀 golden 本體時拓到的六類漏網
    #（TFormHS 最小的 11 支裡至少 7 支本來會被判成乾淨）：
    #   CloseWindowsKeyboard      WinExec("taskkill.exe ...")   <- 執行外部程式殺行程
    #   Check_RecordFolder        ForceDirectories(...)          <- 建目錄（舊樣式只有 MyForceDirectories）
    #   RecordGroundManLog_HS     WriteDataToFile(...)           <- 舊樣式是 `WriteFile(`，拓不到衍生名
    #   RTMServerSocketClientRead SendText(...) / fMain->RTMCommand->Add(...)
    #   ATC_FFCTrigger            ATC_InterfaceForm->ChannelFFCTrigger(...)
    #                             <- 跨表單樣式寫死 `f[A-Z]`，這個全域以 `A` 開頭
    #   *SocketClientError x3     socket Close()/Open()
    ('執行外部程式', r'\bWinExec\s*\(|\bShellExecute\w*\s*\(|\bCreateProcess\w*\s*\('),
    ('建目錄', r'\bForceDirectories\s*\(|\bCreateDir\w*\s*\(|\bMkDir\s*\('),
    ('寫檔（衍生名）', r'\b\w*WriteData\w*\s*\(|\b\w*WriteLog\w*\s*\('),
    ('socket 送出/開關', r'\b\w*Socket\w*\s*->|\bSendText\s*\(|\bSendBuf\s*\('),
    ('跨表單呼叫（非 f 開頭的全域）', r'\b[A-Z]\w*(?:Form|Interface)\w*\s*->\s*\w+\s*\('),
    ('跨表單容器寫入', r'\bf[A-Z]\w+\s*->\s*\w+\s*->\s*(?:Add|Clear|Delete|Insert|Assign)\s*\('),
    # 20260829 六次加強：讀 TATC_InterfaceForm 本體時拓到的兩類：
    #   ShowATC_Page    ATCINIFile->WriteInteger("System","iATC_MODE_TYPE",iType)
    #                   <- **寫 ini 檔**；舊樣式是 `WriteIniData*(`，抓不到透過 TIniFile 物件的寫入
    #   FileSocketRead  frmFileTransfer->OnDataSocketRead(Socket, probRecipe)
    #                   <- 進來的 socket 資料交給 recipe 檔傳輸；
    #                      跨表單樣式要 `f`+大寫，而 `frmFileTransfer` 第二字是小寫 `r`
    # ⚠ `ini 物件寫入` 已驗過**不會誤傷 ReadInteger**（反向探針）。
    ('ini 物件寫入', r'->\s*Write(?:String|Integer|Bool|Float|Date|Time|DateTime|BinaryStream)\s*\('),
    ('跨表單呼叫（frm 前綴）', r'\bfrm[A-Z]\w*\s*->\s*\w+\s*\('),
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


# 20260825 三次加強：只掃 handler 自己的本體會漏掉「本體只有一行函式呼叫」的那種。
# btnSetToTechClick 的全部內容就是 `SetOffsetToTech();`，而那支（cinitial.cpp:13925）
# 會把 offset 累加進 Tech.* 教導座標。所以再跟**一層**自由函式。
# 只跟一層：兩層以上幾乎全樹染紅，篩選就沒有鑑別力了。
FFIDX = freefunc_index.get()
CALL = re.compile(r'(?<![\w>.])([A-Za-z_]\w*)\s*\(')
SKIPCALL = {'if', 'for', 'while', 'switch', 'return', 'sizeof', 'atoi', 'atof',
            'sprintf', 'strcpy', 'strlen', 'memset', 'int', 'double', 'float',
            'bool', 'char', 'dynamic_cast', 'static_cast'}
_bodycache = {}


def deep_hits(body_text):
    """對本體裡呼叫到的自由函式各掃一次風險，回傳 [(函式名, 風險)]。"""
    out = []
    for nm in sorted(set(CALL.findall(body_text))):
        if nm in SKIPCALL or nm not in FFIDX:
            continue
        if nm not in _bodycache:
            try:
                _bodycache[nm] = freefunc_index.body(nm, FFIDX) or ''
            except Exception:
                _bodycache[nm] = ''
        sub = _bodycache[nm]
        h = [k for k, p in RISK if re.search(p, sub)]
        if h:
            f, s, e = FFIDX[nm]
            out.append(('%s (%s:%d)' % (nm, f, s), h))
    return out


print('%-38s %5s  %s' % ('方法', '行數', '風險'))
print('-' * 96)
clean = []
for a, name in defs:
    if want and name not in want:
        continue
    b = span(a)
    body = '\n'.join(l for i, l in enumerate(lines[a:b + 1]) if not l.strip().startswith('//'))
    hits = [k for k, p in RISK if re.search(p, body)]
    deep = deep_hits(body)
    mark = '、'.join(hits) if hits else ('—' if not deep else '')
    print('%-38s %5d  %s' % (name, b - a + 1, mark))
    for fnm, h in deep:
        print('%-38s %5s  [deep] %s -> %s' % ('', '', fnm, '、'.join(h)))
    if not hits and not deep:
        clean.append(name)

print()
print('乾淨（無上列任何風險）: %d 個' % len(clean))
print(' '.join(clean))
