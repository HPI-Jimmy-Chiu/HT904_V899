# -*- coding: cp950 -*-
"""
Generate Greatek Remote Control Operation Manual v2.0 (md + html).
Reflects F899-008 v3 simplification:
  Handler ignores LotInfo_REQUEST Data[0..2]; reply codes only 0/2/60/61/62/90.

Output (UTF-8):
  docs/customer_docs/Greatek/RD5軟體_CC_Greatek_遠端控制操作說明書_<TS>.md
  docs/customer_docs/Greatek/RD5軟體_CC_Greatek_遠端控制操作說明書_<TS>.html
"""
from __future__ import annotations
import sys
from pathlib import Path
from datetime import datetime
import html as _html

ROOT = Path(__file__).resolve().parents[1]
OUT_DIR = ROOT / "docs" / "customer_docs" / "Greatek"
OUT_DIR.mkdir(parents=True, exist_ok=True)

TS = sys.argv[1] if len(sys.argv) > 1 else datetime.now().strftime("%Y%m%d_%H%M%S")
DATE = f"{TS[0:4]}-{TS[4:6]}-{TS[6:8]}"
STEM = f"RD5軟體_CC_Greatek_遠端控制操作說明書_{TS}"

MD = f"""# RD5軟體 CC Greatek 遠端控制操作說明書

| 欄位 | 內容 |
|------|------|
| 文件版本 | 2.0 |
| 適用客戶 | 超豐電子（Greatek） |
| 適用機種 | HT9045 / HT9046（OEE Function 開啟） |
| 適用 Handler 韌體 | V3.33.899.x（含本方案）以後 |
| 對應 SPEC | F899-008 LotInfo_REQUEST 內嵌 OEE Start Lot（v3） |
| 撰寫日期 | {DATE} |
| 撰寫單位 | RD5 |

---

## 0. 與 v1.0 的關鍵差異（必讀）

依超豐 2026-05-15 確認決議：

1. **`LotInfo_REQUEST` 的 LotID / OperatorID / FT-RT 三欄位 Handler 全部忽略**，所有 Lot 資訊改由 OEE 雲端 MO file 帶入。
2. **不再做欄位必填檢查**（移除 v1.0 §4.2 Code 20/21/22/23/24/25/26）。
3. **不再做特殊字元檢查**（移除 v1.0 §4.3 Code 30~36）。
4. **Reply Code 簡化為 6 種**：`0 / 2 / 60 / 61 / 62 / 90`。
5. Recipe 下載 / Sorting list / Setup File 缺漏改由 OEE_StartLot 內部處理，失敗統一收斂為 `61` 或 `90`。
6. Detail Message 仍為英文，由 Handler 端 `OEE_StartLot()` 直接回傳。

> 客戶端 Agent 若已實作 v1.0 規格，僅需「不再判斷 20~51 區段」即可向後相容；framing 不變、欄位不變、序列不變。

---

## 1. 文件目的

本文件提供超豐 Agent 工程師整合 HT9045 Handler **遠端 Lot Start** 流程所需資訊：

1. `LotInfo_REQUEST` 內嵌 OEE Start Lot 的封包格式
2. `LotInfo_REPLY` **異常代碼（Reply Code）對應表** — 客戶可依此快速判斷失敗原因
3. 建議的指令間隔與 timeout 設定（針對「網路速度較慢」場景）

---

## 2. 為什麼要調整流程

V3.33.899.8 之前，超豐 Agent 走以下序列：

```
Agent → LotInfo_REQUEST  → [0] OK
Agent → START_REQUEST    → 機台跳出「Please Start Lot!」modal → 序列卡 12 秒
```

原因：`LotInfo_REQUEST` 只完成「Lot Info 表單層」資料寫入，**未觸發 OEE 系統層的 Start Lot**；後續 `START_REQUEST` 被 OEE 檢查擋下，跳出對話框等人按 OK。

**V3.33.899.x（本方案 v3）之後**：

```
Agent → LotInfo_REQUEST  → [0] OK              (同時完成 OEE Start Lot)
Agent → START_REQUEST    → [0] OK (< 1 秒)     (既有，不變)
```

本方案**不新增客戶端可見指令**。客戶仍依照原先順序送 `LotInfo_REQUEST` → `START_REQUEST`，差異只在 Handler 端：當 `CUSTOMER_CODE==CC_Greatek` 且 OEE 開啟時，`LotInfo_REQUEST` 會在內部執行原本手動按 Lot Info Start Lot 按鈕才會做的 OEE Start Lot 動作。

---

## 3. 指令封包格式

### 3.1 Request：`LotInfo_REQUEST`

| Index | 欄位 | 必填？ | 說明 |
|-------|------|------|------|
| Data[0] | LotID | **否（保留）** | Handler **忽略**。Lot ID 由 OEE 雲端 MO file 帶入。可空字串、可省略整個欄位。 |
| Data[1] | OperatorID | **否（保留）** | 同上。Handler 忽略。 |
| Data[2] | FT-RT | **否（保留）** | 同上。Run Mode 由 MO file 帶入（`PASSBIN` 等欄位）。 |

> v3 規格：framing 仍維持既有 3 欄位以保留向後相容性，但 Handler 不讀取任何欄位內容。客戶 Agent 可直接送 `LotInfo_REQUEST<EOF>`、`LotInfo_REQUEST,,,<EOF>` 或仍帶舊值 `LotInfo_REQUEST,LOT12345,OP9876,0<EOF>`，三種寫法行為完全一致。

#### 3.1.1 為什麼可以結合在 `LotInfo_REQUEST`

| 規則 | 說明 |
|------|------|
| 1. 雲端資料權威 | 所有 Lot 資訊（LotID / OPID / RunMode / PASSBIN / TestFlow / YieldLimit / Temperature / SoakTime / InterfaceType / SetupFile）由 OEE FTP 上的 MO file 帶入；客戶端 Agent **不需要**也**不應該**重複送。 |
| 2. 客戶端序列不變 | 客戶仍只送 `LotInfo_REQUEST` → `START_REQUEST`，不用新增任何指令。 |
| 3. 超豐專用分流 | 只有 `CUSTOMER_CODE==CC_Greatek` 且 OEE 開啟時才會進入內嵌 OEE Start Lot；其他客戶維持舊版行為。 |
| 4. 異常不跳窗 | 原本會彈出的 OEE Start Lot alarm 改由 `LotInfo_REPLY` 回傳 Reply Code 與英文 Detail Message。 |

#### 3.1.2 標準呼叫序列（建議所有正式生產採用）

```
1. LotInfo_REQUEST(,,)                 → [0] OK   ← 已完成 OEE Start Lot
2. START_REQUEST                       → [0] OK
```

#### 3.1.3 Handler 端處理順序（v3 簡化版）

```cpp
// 偽碼說明（對應 auto9045.cpp::SetLotInfoGreatekOEE）
1. CheckCanChangeRealDummy()  → false 回 2（機台 Run 中 / 有殘料）
2. 檢查 Monitor 連線           → false 回 60
3. OEE_StartLot(silent, errMsg)
     - errMsg 含 "Machine ID" → 回 62
     - errMsg 含 "MO"         → 回 61
     - 其他失敗               → 回 90
4. SetLotID + SetLotStart      → 寫入 MO file 帶來的 LotID 並啟動 Lot
5. SetLotComponents(false)     → 由 MO file 設定其餘欄位
6. SetFirstTrayCheckOnUnloader → 完成
   全部成功回 0
```

**Data[0..2] 完全沒讀**——標記為 `(void)Data;`。

#### 3.1.4 為什麼 LotID / OPID / RunMode / MO / Flow / Ticket / Process / Product / TestTimes 都不在 Data 裡

依超豐 2026-05-15 確認：所有資料一律由 OEE 雲端 MO file 帶入，客戶 Agent 端不負責填寫任何 Lot 欄位。

| 資料 | 來源 |
|------|------|
| LotID | MO file（沿用 `OEE_StartLot()` 既有行為） |
| OperatorID | MO file |
| RunMode (FT/RT) | MO file `PASSBIN` 欄位推導 |
| MO Number | MO file |
| TestFlow / Ticket / Process / Product / TestTimes | MO file |
| Recipe / Setup File | MO file 內 `SetupFileName` 欄位 → `AutoDownloadSetupFileByMO` 自動下載 |
| Temperature / SoakTime | MO file |
| Interface Type (GPIB/TTL/RS232) | MO file |

→ 因此本規格**不在 `LotInfo_REQUEST` 新增任何欄位**。所有上述欄位永遠以「OEE 雲端 MO file 現讀到的值」為準。

#### 範例

標準呼叫（生產建議）：
```
LotInfo_REQUEST,,,<EOF>
```

省略整個 Data 區段（亦合法）：
```
LotInfo_REQUEST<EOF>
```

仍帶舊值（向後相容，Handler 忽略）：
```
LotInfo_REQUEST,LOT12345,OP9876,0<EOF>
```

### 3.2 Reply：`LotInfo_REPLY`

| 情境 | V_TOTAL | Data[0] | Data[1] |
|------|--------|---------|---------|
| 成功 | 1 | `0` | （無此欄位） |
| 失敗 | 2 | Reply Code | Detail Message（英文）|

> **語系規則**：Detail Message **一律為英文**，與其他 OLP REPLY 一致。機台不負責中英對照；若客戶端 UI 需中文，請由 Agent 端依 Reply Code 自行查本地化表。

#### 範例

成功：
```
LotInfo_REPLY,0<EOF>
```

失敗（OEE MO 下載失敗）：
```
LotInfo_REPLY,61,TfProductionInfo::OEE_StartLot Set MO Information Fail!<EOF>
```

失敗（Machine ID 為空）：
```
LotInfo_REPLY,62,Machine ID is Empty in LoadMOInformation<EOF>
```

---

## 4. Reply Code 完整對應表（v3 簡化版）

> v3 規格只回 6 種代碼。所有資料層欄位必填 / 字元檢查 / Recipe / Sorting list / Setup File 等失敗，均收斂入 `61` 或 `90`。

| Code | 觸發條件 | Detail Message | 客戶端建議處理 |
|------|---------|----------------|----------------|
| **0**  | Start Lot 成功 |（無 Detail 欄位） | 繼續送 `START_REQUEST` |
| **2**  | 機台 Run 中 / 有殘料 / 未經 CleanOut+End Lot 直接重送 | `Machine running or has residual IC` | 操作員執行 Clean Out + End Lot 後重試 |
| **60** | Monitor 軟體未連線 | `Please check Monitor connect` | 通知操作員檢查 Monitor 軟體 |
| **61** | OEE MO file 下載 / 解析失敗（含 PASSBIN 格式錯、Interface Type 錯、MO file 不存在、FTP 連線失敗等） | 由 Handler `OEE_StartLot()` 回傳之英文字串，常見：`TfProductionInfo::OEE_StartLot Set MO Information Fail!` 或 `Load MO Information Fail!` | 通知 RD/FAE 檢查 OEE FTP server、Machine ID 對應 MO 是否存在、MO file 內容格式 |
| **62** | OEE Machine ID 設定為空 | `Machine ID is Empty in LoadMOInformation` | 通知 RD/FAE 確認 `system/Gerneral.ini` 內 Machine ID 設定 |
| **90** | OEE_StartLot 內部其他未分類失敗 | 帶 Handler 回傳的英文錯誤字串 | 將 Detail Message 完整提供給 RD 分析 |

> **保留碼**：`1 / 3 / 20~26 / 30~36 / 40~41 / 50~51 / 91` 在 v1.0 曾出現；v3 路徑**不再回傳**這些代碼（已收斂入 0/2/60/61/62/90）。客戶端可移除對應分支。

### 4.1 OEE 確認框

> 機台原本在 Start Lot 前彈出「Are You Sure Start Lot?」確認框；OLP 路徑 `OEE_StartLot(bSilent=true)` **自動視為 YES**，不會回傳代碼，也不會阻塞流程。

---

## 5. 客戶端建議處理流程（虛擬碼）

```python
reply = send("LotInfo_REQUEST,,,")        # 三欄位皆可省略
rc = int(reply.Data[0])
msg = reply.Data[1] if len(reply.Data) > 1 else ""

if rc == 0:
    send("START_REQUEST")                  # 成功，繼續
elif rc == 2:
    notify_operator("Clean Out + End Lot 後重試")
elif rc == 60:
    notify_operator("Monitor 軟體未連線")
elif rc in (61, 62):
    escalate_to_rd_fae(rc, msg)            # OEE 環境問題
else:  # 90
    log_and_review(rc, msg)
```

---

## 6. 通訊建議參數

| 場景 | 建議值 | 說明 |
|------|-------|------|
| `LotInfo_REQUEST` → `START_REQUEST` 間隔 | **? 500 ms** | LotInfo 內嵌 OEE Start Lot，且可能含 Recipe / MO / Setup File 下載觸發 |
| `LotInfo_REPLY` 等待 timeout | **? 30 s** | OEE_StartLot 內部 `SetMOInformation()` 在慢網路下實測可達 11~15 秒；建議 timeout ? 30 秒 |
| TCP keep-alive timeout | **? 30 s** | 避免慢網路下被中間設備斷線 |

> 2026-05-15 RD 端 OLP simulator 實測：`LotInfo_REQUEST` 成功來回 11.32 秒（含 MO file 下載 + Recipe / Setup File 處理）。

---

## 7. 與既有指令的關係

| 既有指令 | 影響 |
|---------|------|
| `LotInfo_REQUEST` | 超豐 OEE 開啟時會多做 OEE Start Lot；其他客戶完全不變 |
| `START_REQUEST` | **完全不變**，仍須在 `LotInfo_REPLY=0` 後送 |
| 操作員手動按 Lot Info Start Lot 按鈕 | **完全不變**，與 OLP 路徑可並存 |

---

## 8. 異常排查 SOP

收到非 0 Reply Code 時：

1. **記錄完整 reply 字串**（含 Code 與 Detail Message）
2. 對照本文件 §4 找出對應分類
3. **Code 2 / 60** → 操作員處理
4. **Code 61 / 62 / 90** → 將完整 reply + 機台時間戳轉給 RD/FAE

RD 端需提供時，請一併附上：
- 該時間點的機台 `Error/` 目錄 log
- `system/Gerneral.ini` 設定（Machine ID、OEE FTP 設定）
- OEE 雲端對應 MO file 檔名與內容

### 8.1 Code 61 常見根因

| 根因 | 排查 |
|------|------|
| `LoadMOInformation()` 失敗 — OEE FTP 抓不到對應 MO file | 檢查 OEE FTP server / 帳密 / 該 Machine ID 的 MO file 在雲端是否存在 |
| `bN14_3_OEEFTPUpload==false` | 確認 `system/Gerneral.ini` 設定為 `1` |
| MO file 內 `PASSBIN` 字串長度 > 4 或非 P/F | 修正 MO file 內容 |
| MO file `InterfaceType` 不是 GPIB / TTL / RS232 | 修正 MO file 內容 |
| MO file 不是 `.txt` 結尾 | 雲端命名規則 |

> Handler 端 `SetMOInformation()` 失敗時可能會跳出 `ShowMyMessage` modal（`Local MO File Lose Error` / `TempLoadMO Download Error` / `Load MO Information Fail!` / `Machine ID is Empty in LoadMOInformation` 等），需到機台現場按 OK 並抄寫 dialog 內英文訊息以精確定位。

---

## 9. 修訂歷史

| 版本 | 日期 | 修訂者 | 內容 |
|------|------|--------|------|
| 1.0 | 2026-05-04 | RD5 | 初版發行（`LotInfo_REQUEST` 內嵌 OEE Start Lot；客戶端序列維持兩步） |
| 2.0 | {DATE} | RD5 | 依 2026-05-15 超豐確認決議：Handler 完全忽略 Data[0..2]；Reply Code 簡化為 0/2/60/61/62/90；移除欄位必填、字元檢查、Recipe/Sorting list/Setup File 個別錯誤碼 |

---

## 10. 聯絡窗口

- RD：JimmyChiu
- 對應 SPEC：HT9045 `docs/spec/features/F899-008-remote-start-lot-replacement.spec.md`（v3）
"""

# ---------------- HTML ----------------
# Minimal markdown → HTML converter (handles only what this doc needs)
def md_to_html(md: str) -> str:
    lines = md.splitlines()
    out: list[str] = []
    i = 0
    in_code = False
    code_lang = ""
    while i < len(lines):
        ln = lines[i]
        if ln.startswith("```"):
            if not in_code:
                code_lang = ln[3:].strip()
                out.append(f'<pre><code class="lang-{_html.escape(code_lang)}">')
                in_code = True
            else:
                out.append("</code></pre>")
                in_code = False
            i += 1
            continue
        if in_code:
            out.append(_html.escape(ln))
            i += 1
            continue
        # table
        if ln.startswith("|") and i + 1 < len(lines) and set(lines[i+1].replace("|","").strip()) <= set("-: "):
            header = [c.strip() for c in ln.strip().strip("|").split("|")]
            i += 2
            rows = []
            while i < len(lines) and lines[i].startswith("|"):
                rows.append([c.strip() for c in lines[i].strip().strip("|").split("|")])
                i += 1
            out.append("<table>")
            out.append("<thead><tr>" + "".join(f"<th>{_inline(c)}</th>" for c in header) + "</tr></thead>")
            out.append("<tbody>")
            for r in rows:
                out.append("<tr>" + "".join(f"<td>{_inline(c)}</td>" for c in r) + "</tr>")
            out.append("</tbody></table>")
            continue
        # headings
        if ln.startswith("#"):
            level = len(ln) - len(ln.lstrip("#"))
            text = ln[level:].strip()
            out.append(f"<h{level}>{_inline(text)}</h{level}>")
            i += 1
            continue
        if ln.strip() == "---":
            out.append("<hr/>")
            i += 1
            continue
        # blockquote
        if ln.startswith(">"):
            block = []
            while i < len(lines) and lines[i].startswith(">"):
                block.append(lines[i][1:].strip())
                i += 1
            out.append("<blockquote>" + "<br/>".join(_inline(b) for b in block) + "</blockquote>")
            continue
        # ordered/unordered list
        if ln.lstrip().startswith(("- ", "* ")):
            block = []
            while i < len(lines) and lines[i].lstrip().startswith(("- ", "* ")):
                block.append(lines[i].lstrip()[2:])
                i += 1
            out.append("<ul>" + "".join(f"<li>{_inline(b)}</li>" for b in block) + "</ul>")
            continue
        if ln.strip() and ln.lstrip()[:2].rstrip(".").isdigit() and ". " in ln:
            block = []
            while i < len(lines) and lines[i].strip() and lines[i].lstrip()[:2].rstrip(".").isdigit() and ". " in lines[i]:
                block.append(lines[i].split(". ", 1)[1])
                i += 1
            out.append("<ol>" + "".join(f"<li>{_inline(b)}</li>" for b in block) + "</ol>")
            continue
        if ln.strip() == "":
            out.append("")
            i += 1
            continue
        out.append(f"<p>{_inline(ln)}</p>")
        i += 1
    return "\n".join(out)

def _inline(s: str) -> str:
    s = _html.escape(s)
    # bold
    import re
    s = re.sub(r"\*\*(.+?)\*\*", r"<strong>\1</strong>", s)
    s = re.sub(r"`([^`]+)`", r"<code>\1</code>", s)
    return s

HTML_TEMPLATE = """<!DOCTYPE html>
<html lang="zh-TW">
<head>
<meta charset="UTF-8"/>
<title>RD5軟體 CC Greatek 遠端控制操作說明書 v2.0</title>
<style>
  body {{ font-family: "Microsoft JhengHei","Segoe UI",Arial,sans-serif; max-width: 980px; margin: 24px auto; padding: 0 24px; color:#222; line-height: 1.55; }}
  h1 {{ border-bottom: 2px solid #2563eb; padding-bottom: 6px; }}
  h2 {{ border-left: 4px solid #2563eb; padding-left: 8px; margin-top: 28px; color:#1e3a8a; }}
  h3 {{ color:#1e40af; margin-top: 20px; }}
  h4 {{ color:#374151; }}
  table {{ border-collapse: collapse; width: 100%; margin: 8px 0 16px; font-size: 14px; }}
  th, td {{ border: 1px solid #d1d5db; padding: 6px 10px; vertical-align: top; }}
  th {{ background: #f3f4f6; text-align: left; }}
  tr:nth-child(even) td {{ background: #fafafa; }}
  code {{ background: #f3f4f6; padding: 1px 5px; border-radius: 3px; font-family: Consolas, "Courier New", monospace; font-size: 13px; }}
  pre {{ background: #0f172a; color: #e2e8f0; padding: 12px 16px; border-radius: 6px; overflow-x: auto; font-size: 13px; }}
  pre code {{ background: transparent; color: inherit; padding: 0; }}
  blockquote {{ border-left: 4px solid #f59e0b; background: #fffbeb; padding: 8px 12px; margin: 12px 0; color:#78350f; }}
  hr {{ border: 0; border-top: 1px dashed #cbd5e1; margin: 24px 0; }}
  ul, ol {{ padding-left: 22px; }}
  .footer {{ margin-top: 40px; padding-top: 12px; border-top: 1px solid #e5e7eb; font-size: 12px; color:#6b7280; text-align:center; }}
</style>
</head>
<body>
{body}
<div class="footer">RD5軟體 · HT9045 · F899-008 v3 · {date}</div>
</body>
</html>
"""

def main():
    md_path = OUT_DIR / f"{STEM}.md"
    html_path = OUT_DIR / f"{STEM}.html"
    md_path.write_text(MD, encoding="utf-8")
    html_body = md_to_html(MD)
    html_path.write_text(HTML_TEMPLATE.format(body=html_body, date=DATE), encoding="utf-8")
    print(f"[OK] wrote {md_path}")
    print(f"[OK] wrote {html_path}")

if __name__ == "__main__":
    main()
