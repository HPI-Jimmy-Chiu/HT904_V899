# -*- coding: cp950 -*-
"""
Generate Greatek Remote Control Operation Manual aligned with 2026-04-15 V1.0
PPT structure, applying today's v3 (F899-008) updates, with HonPrecision red theme.

Output (UTF-8):
  docs/customer_docs/Greatek/RD5軟體_HT9045_遠端控制操作說明書_<TS>.md
  docs/customer_docs/Greatek/RD5軟體_HT9045_遠端控制操作說明書_<TS>.html
"""
from __future__ import annotations
import sys, html as _html, re
from pathlib import Path
from datetime import datetime

ROOT = Path(__file__).resolve().parents[1]
OUT_DIR = ROOT / "docs" / "customer_docs" / "Greatek"
OUT_DIR.mkdir(parents=True, exist_ok=True)

TS = sys.argv[1] if len(sys.argv) > 1 else datetime.now().strftime("%Y%m%d_%H%M%S")
DATE = f"{TS[0:4]}-{TS[4:6]}-{TS[6:8]}"
STEM = f"RD5軟體_HT9045_遠端控制操作說明書_{TS}"

MD = f"""# HT9045 Handler 遠端控制操作說明書

**TCP Socket 遠端控制功能 — 客戶端操作指南**

| 欄位 | 內容 |
|------|------|
| 適用客戶 | Greatek（超豐電子） |
| 適用機種 | HT9045 / HT9046（OEE Function 開啟） |
| 文件版本 | V2.0 |
| 制訂日期 | {DATE} |
| 發行日期 | {DATE} |
| 制訂單位 | RD5（鴻勁精密 HonPrecision, Inc.） |
| 對應 SPEC | F899-008 LotInfo_REQUEST 內嵌 OEE Start Lot（v3） |
| 機密等級 | HonPrecision Confidential Information |

---

## 目 錄

1. 功能概述與實作狀態
2. Handler 端設定
3. 客戶端 Socket 通訊規格
4. 指令集說明
5. 完整操作流程
6. 錯誤處理與注意事項
7. 附錄：快速參考表
8. V2.0 變更說明（vs. V1.0 2026-04-15）
9. 修訂歷史

---

## 1. 功能概述

HT9045 Handler 支援透過 TCP Socket 進行遠端控制，允許客戶端程式（Agent）執行：

- **遠端啟動 (Auto Start)**：遠端觸發機台從閒置進入運行狀態
- **清除計數器 (Clear Counters)**：清除上一批次累計的計數器（時間、裝載、TraySort、Contact、Tester 分類）
- **啟動批次 (Start Lot)**：建立新批次並由 OEE 雲端 MO file 自動帶入 LotID/OperatorID/RunMode/Recipe/Setup File 等資料
- **安全鎖定 / 解鎖**：遠端鎖定或解鎖機台，防止意外啟動
- **機台狀態查詢**：查詢機台運行狀態、批次資訊、軟體版本等

### 1.1 功能實作狀態

| 功能 | 規格書需求 | 實作狀態 | 備註 |
|------|-----------|---------|------|
| Auto Start（遠端啟動） | Requirement #1 | 已完成 | 機台需處於 HALT |
| Safety Interlock（Lock/Unlock） | Requirement #3 | 已完成 | Lock 時無法啟動 |
| Clear Counters（清除計數器） | Requirement #5 | 已完成 | 支援全清及分項；**僅清計數，不關 OEE 雲端 Lot** |
| OEE Lot End（關閉雲端 Lot） | — | **未提供 OLP 指令** | 由操作員於 Handler UI 按 `Lot End` 按鈕觸發 |
| Start Lot — 批次資訊 | Requirement #4-1 | 已完成（v3 更新）| **Handler 忽略 Data 欄位**，由 OEE MO file 帶入 |
| Start Lot — Setup File | Requirement #4-2 | 已完成 | zip 壓縮格式，亦可由 MO file 自動下載 |
| Start Lot — 內嵌 OEE Start Lot | F899-008（v3 新增）| 已完成 | `LotInfo_REQUEST` 成功即完成 OEE Start Lot |

> **V2.0 重點**：超豐 OEE 模式下，`LotInfo_REQUEST` 內嵌 silent OEE Start Lot；`LotID / OperatorID / FT-RT` 三欄位由雲端 MO file 帶入，Handler 忽略客戶送入的 Data。

---

## 2. Handler 端設定

### 2.1 客戶碼確認

- Handler 需設定為 **Greatek（超豐）客戶碼**，系統啟動時自動開啟 OLP Server
- 設定由設備供應商在出廠或維護時配置，**客戶端無需調整**

### 2.2 通訊服務狀態確認

- Handler 啟動後，在 Automation 監控畫面確認 **OLP 狀態已開啟**
- 客戶端成功連線時，畫面會顯示連線狀態

### 2.3 網路環境要求

- Handler PC 需具備乙太網路連線
- **防火牆需開放 TCP Port 6671（入站規則）**
- 客戶端 PC 與 Handler PC 需在同一網段或可路由網段

### 2.4 OEE 系統前置（v3 新增）

| 項目 | 要求 | 備註 |
|------|------|------|
| `bN14_1_EnableOEEFunction` | `1`（啟用） | `system/Gerneral.ini` |
| `bN14_3_OEEFTPUpload` | `1`（啟用） | OEE FTP 同步必須開 |
| Machine ID | 不可空 | 對應雲端 MO file 識別 |
| OEE FTP server | 可連線、帳密正確 | MO file / Setup File 來源 |
| 對應 Machine ID 的 MO file | 雲端存在且格式正確 | 含 PASSBIN / InterfaceType / Temperature 等欄位 |

> 上述任一條件不符，`LotInfo_REQUEST` 將回傳 Reply Code `60 / 61 / 62 / 90`（見 §4.3 Reply Code 對應表）。

---

## 3. 客戶端 Socket 通訊規格

| 參數 | 值 |
|------|------|
| 通訊協定 | TCP/IP |
| 連線模式 | Client → Server（客戶端主動連線 Handler）|
| Handler IP | Handler PC 的 IP 位址（請洽設備管理人員）|
| Port | **6671** |
| 編碼 | ASCII |
| 最大連線數 | **1**（同一時間僅允許一個客戶端連線）|

> 若連線數超過上限，Handler 將自動重置 OLP Server。請確保只有一個客戶端連線。

### 3.1 封包格式

所有指令與回傳均採用以下固定封包格式：

```
[STX] + Command + [SOH] + V_Total + [SOH] + Data[0] + ... + [ETX]
```

控制字元定義：

| 符號 | Hex | 說明 |
|------|-----|------|
| STX | `0x02` | 封包起始（Start of Text）|
| SOH | `0x01` | 欄位分隔符（Start of Heading）|
| ETX | `0x03` | 封包結束（End of Text）|

封包組裝範例：

```
Auto Start : 0x02 "START_REQUEST"        0x01 "0" 0x03
End Lot    : 0x02 "CLEAR_REPORT_REQUEST" 0x01 "1" 0x01 "1111" 0x03
Start Lot  : 0x02 "LotInfo_REQUEST"      0x01 "0" 0x03
             ↑ V2.0：Data 欄位皆可省略，Handler 由 OEE MO file 帶資料
```

---

## 4. 指令集說明

### 4.1 Safety Unlock / Lock

#### Safety Unlock — 解除安全鎖定

啟動機台前，必須先解除安全鎖定。

| 方向 | 內容 |
|------|------|
| 發送 | `RESUME_REQUEST` `V_Total=0` |
| 回傳 | `RESUME_REPLY` `Data[0]=0`（成功）|

#### Safety Lock — 安全鎖定

良率異常或需緊急停止時，鎖定機台。鎖定狀態下 Auto Start 將被拒絕。

| 方向 | 內容 |
|------|------|
| 發送 | `PAUSE_REQUEST` `V_Total=0` |
| 回傳 | `PAUSE_REPLY` `Data[0]=0`（成功）|

> **注意**：Lock 效果立即生效，運行中機台將觸發 **Soft Stop**。

### 4.2 Clear Counters（清除計數器）

清除上一批次累計的計數器，為新批次做準備。

> **重要說明**：本指令**僅清除 Handler 端的計數器**（時間 / 裝載 / TraySort / Contact / Tester 分類），**不會關閉 OEE 雲端的 Lot、不會送 SECS LotEnd Event、不會上傳 BinQty/Socket/TrayMapping 報告**。OEE 雲端 Lot 須由操作員於 Handler UI 按 `Lot End` 按鈕結束（詳見 §6.3 注意事項 8）。

> **前提條件**：機台必須處於停止狀態（非運行中）

| 方向 | 內容 |
|------|------|
| 發送 | `CLEAR_REPORT_REQUEST` `V_Total=1` `Data[0]=清除代碼` |
| 回傳 | `CLEAR_REPORT_REPLY` `Data[0]=0`（成功）|

清除代碼：

| 代碼 | 清除範圍 |
|------|---------|
| **1111** | 全部計數器清除（推薦用於批次切換）|
| 2 | 僅清除時間資料（TimeData）|
| 3 | 僅清除裝載計數（LoadingCounts）|
| 4 | 僅清除 Tray Sort 計數 |
| 5 | 僅清除 Contact 計數 |
| 6 | 僅清除測試分類計數（Tester Category）|

### 4.3 Start Lot（V2.0 重大更新）

本指令為 V2.0 核心：以單一 `LotInfo_REQUEST` 內嵌 OEE Start Lot，並可由雲端 MO file 自動帶入 Setup File，無需客戶端額外發送下載指令。

> **前提條件**：機台非運行中，且內部無殘留 IC。

#### 發送格式

| 欄位 | V1.0 規格 | V2.0 規格（v3）|
|------|-----------|---------------|
| Command | `LotInfo_REQUEST` | `LotInfo_REQUEST` |
| V_Total | `3` | **`0` ~ `3` 皆可** |
| Data[0] | Lot ID（必填）| **保留欄位**，Handler **忽略**，由 OEE MO file 帶入 |
| Data[1] | Operator ID（必填）| **保留欄位**，Handler **忽略**，由 OEE MO file 帶入 |
| Data[2] | `0` (FT) / `1` (RT)（必填）| **保留欄位**，Handler **忽略**，由 OEE MO file 帶入 |

封包範例（皆合法）：

```
0x02 "LotInfo_REQUEST" 0x01 "0" 0x03                                        ← V2.0 推薦
0x02 "LotInfo_REQUEST" 0x01 "3" 0x01 "" 0x01 "" 0x01 "" 0x03                ← V2.0 三欄空值
0x02 "LotInfo_REQUEST" 0x01 "3" 0x01 "LOT001" 0x01 "OP001" 0x01 "0" 0x03    ← V1.0 寫法（向後相容）
```

> **重要**：V2.0 維持 framing 向後相容，客戶 Agent 即使繼續送 V1.0 寫法也不會錯，但 Handler 不會讀取任何 Data 欄位。所有 Lot 資訊一律由 OEE 雲端 MO file 帶入。

#### 回傳格式

| 情境 | V_Total | Data[0] | Data[1] |
|------|--------|---------|---------|
| 成功 | 1 | `0` | （無） |
| 失敗 | 2 | Reply Code | Detail Message（英文）|

#### Reply Code 對應表（V2.0 簡化版）

| Code | 觸發條件 | Detail Message | 客戶端建議處理 |
|------|---------|----------------|----------------|
| **0** | Start Lot 成功 |（無） | 繼續送 `START_REQUEST` |
| **2** | 機台 Run 中 / 有殘料 | `Machine running or has residual IC` | 操作員 (1) 退出殘料 (2) **於 Handler UI 按 `Lot End` 按鈕**結束舊 OEE Lot；Agent 再重送 `LotInfo_REQUEST`（單送 `CLEAR_REPORT_REQUEST` 不會解此狀態）|
| **60** | Monitor 軟體未連線 | `Please check Monitor connect` | 通知操作員檢查 Monitor 軟體 |
| **61** | OEE MO file 下載 / 解析失敗 | `TfProductionInfo::OEE_StartLot Set MO Information Fail!` 或 `Load MO Information Fail!` | 通知 RD/FAE 檢查 OEE FTP / 雲端 MO file |
| **62** | OEE Machine ID 為空 | `Machine ID is Empty in LoadMOInformation` | 檢查 `system/Gerneral.ini` Machine ID |
| **90** | OEE_StartLot 內部其他失敗 | 帶 Handler 回傳的英文錯誤字串 | 將 Detail Message 完整提供給 RD 分析 |

> V1.0 的 `20 / 21 / 24 / 30 / 31 / 40 / 41 / 50 / 51 / 91` 等代碼在 V2.0 已**收斂入 0/2/60/61/62/90**，客戶端可移除對應分支判斷。

#### Setup File 下載（選用）

若雲端 MO file 內含 `SetupFileName` 欄位，Handler 會由 `AutoDownloadSetupFileByMO` 在 `LotInfo_REQUEST` 處理流程中自動下載對應 Setup File，**客戶端可省略此步驟**。

以下 `PP_DL_REQUEST` 僅在「雲端 MO file 未指定 Setup File」或「需手動下傳測試」時使用，與 §4.3 上方 `LotInfo_REQUEST` 屬同一 Start Lot 流程的選用補強：

| 方向 | 內容 |
|------|------|
| 發送 | `PP_DL_REQUEST` `V_Total=3` |
| Data[0] | 檔案名稱（含 `.zip` 副檔名） |
| Data[1] | 檔案大小（byte，字串） |
| Data[2] | 檔案內容（Hex 字串編碼，每 byte 以 2 碼 hex 表示，例 byte `0xAB` → 字串 `"AB"`） |
| 回傳 | `PP_DL_REPLY` `Data[0]=1`（成功）/ `Data[0]=0`（失敗） |

### 4.4 Auto Start — 遠端啟動

觸發機台從閒置狀態進入運行狀態。

| 方向 | 內容 |
|------|------|
| 發送 | `START_REQUEST` `V_Total=0` |
| 回傳 | `START_REPLY` `Data[0]=0`（固定回傳） |

執行條件：

| 執行條件 | 說明 |
|---------|------|
| 機台狀態 = HALT | 機台必須處於閒置停止狀態 |
| 未運行中 | 機台尚未啟動 |
| 未被鎖定 | 安全鎖定必須已解除（需先送 `RESUME_REQUEST`） |

> 若條件不滿足，Handler 仍回傳 `START_REPLY`，但**不會實際啟動機台**。建議先送 `MAIN_STATUS_INQUIRE` 確認狀態為 `HALT` 再送 Auto Start。

### 4.5 狀態查詢（INQUIRE）

#### 機台狀態查詢

| 方向 | 內容 |
|------|------|
| 發送 | `MAIN_STATUS_INQUIRE` `V_Total=0` |
| 回傳 | `MAIN_STATUS_GRANT` `Data[0]=狀態字串` |

| 狀態值 | 說明 |
|--------|------|
| `RUN` | 運行中 |
| `PAUSE` | 暫停中 |
| `HALT` | 停止（可接受 Auto Start） |
| `LOCK` | 鎖定中（需先解鎖） |

#### 其他查詢指令

| Inquire | Grant | 說明 |
|---------|-------|------|
| `LotInfo_INQUIRE` | `LotInfo_GRANT` | 批次資訊 |
| `SETUP_FILE_NAME_INQUIRE` | `SETUP_FILE_NAME_GRANT` | Setup File 名稱 |
| `VERSION_INQUIRE` | `VERSION_GRANT` | 軟體版本 |

---

## 5. 完整操作流程

### 5.1 標準 Auto Start 流程（依序執行）

| 步驟 | 動作 | 指令 |
|------|------|------|
| 1 | 建立 TCP 連線 | Client 連線至 Handler IP:6671 |
| 2 | 查詢機台狀態（建議）| `MAIN_STATUS_INQUIRE` → 確認為 `HALT` |
| 3 | 清除計數器（Clear Counters）| `CLEAR_REPORT_REQUEST [1111]`；**註**：上一個 OEE Lot 須由操作員按 Handler UI `Lot End`，或由 Tray Feed 流程關閉，本指令不負責 |
| 4 | **設定批次資訊（內嵌 OEE Start Lot）** | `LotInfo_REQUEST` → 確認回傳碼 = `0`（V2.0：Data 可省略） |
| 5 | 下載 Setup File（如需要）| `PP_DL_REQUEST [filename.zip, size, hex]` → 確認回傳碼 = `1` |
| 6 | 解除安全鎖定 | `RESUME_REQUEST` → `RESUME_REPLY` |
| 7 | 遠端啟動（Auto Start）| `START_REQUEST` → `START_REPLY` |
| 8 | 確認機台已啟動（建議）| `MAIN_STATUS_INQUIRE` → 確認為 `RUN` |

> **V2.0 提醒**：步驟 4 的 `LotInfo_REQUEST` 在 v3 後**已內嵌 OEE Start Lot**，成功回傳 `0` 即代表「Lot Info 寫入 + OEE Start Lot」皆完成。客戶端**不需要**也**不應該**再呼叫額外指令觸發 OEE Start Lot。

### 5.2 緊急鎖定 / 解除後重新啟動

#### 緊急鎖定流程

當偵測到異常（如良率異常）時：

1. 發送 `PAUSE_REQUEST` → Handler 立即鎖定
2. 運行中的機台將執行 Soft Stop
3. 等待 `PAUSE_REPLY`

#### 解除鎖定後重新啟動

1. 發送 `RESUME_REQUEST` → 解除鎖定
2. 查詢 `MAIN_STATUS_INQUIRE` → 確認為 `HALT`
3. 發送 `START_REQUEST` → 重新啟動

---

## 6. 錯誤處理與注意事項

### 6.1 常見情境

| 情境 | 現象 | 處理建議 |
|------|------|---------|
| 非 HALT 狀態送 Auto Start | 不執行啟動，仍回傳 `REPLY` | 先查詢確認為 HALT |
| 機台運行中送 LotInfo | 回傳碼 = `2`（失敗）| 等待停止後再設定 |
| **OEE MO file 下載失敗（v3 新增）** | LotInfo 回傳碼 = `61` | 檢查 OEE FTP / Machine ID / 雲端 MO file |
| **Machine ID 為空（v3 新增）** | LotInfo 回傳碼 = `62` | 檢查 `system/Gerneral.ini` Machine ID |
| Monitor 軟體未連線 | LotInfo 回傳碼 = `60` | 通知操作員啟動 Monitor 軟體 |
| Setup File 解壓失敗 | `PP_DL_REPLY` 回傳 `0` | 檢查 zip 完整性與格式 |
| 連線中斷 | 無回傳 | 重新建立 TCP 連線 |
| 安全鎖定未解除送 Auto Start | 機台不啟動 | 先送 `RESUME_REQUEST` |

### 6.2 建議逾時（V2.0 更新）

| 指令 | V1.0 建議逾時 | **V2.0 建議逾時** | 原因 |
|------|---------------|-------------------|------|
| 一般指令（Start/Pause/Resume/Clear）| 5 秒 | 5 秒 | — |
| **`LotInfo_REQUEST`** | 5 秒 | **>= 30 秒** | v3 內嵌 OEE Start Lot，含 MO file 下載 + Recipe 處理；RD 端 OLP simulator 實測達 11.32 秒 |
| `PP_DL_REQUEST`（Setup File）| 30 秒 | 30 秒 | — |
| 狀態查詢（INQUIRE）| 3 秒 | 3 秒 | — |

### 6.3 重要注意事項

1. **同一時間僅允許一個客戶端連線**，多重連線將導致 Server 重置
2. **指令需逐一發送**，等待前一個指令回傳後再發送下一個
3. **Safety Lock (`PAUSE_REQUEST`) 效果立即生效**，運行中機台將觸發 Soft Stop
4. Setup File 必須為 zip 格式，檔案內容以 Hex 字串傳送
5. 批次切換時建議使用清除代碼 **1111**（全部清除），確保上一批計數器完全清空
6. **V2.0 新增**：`LotInfo_REQUEST` 與 `START_REQUEST` 之間建議間隔 **>= 500 ms**
7. **V2.0 新增**：所有 Lot 欄位由 OEE 雲端 MO file 帶入，請於雲端維護正確的 Machine ID 對應 MO file
8. **V2.0 新增（OEE Lot End 範圍說明）**：本版本未提供「關閉 OEE 雲端 Lot」的 OLP 指令。`CLEAR_REPORT_REQUEST` **僅清 Handler 計數器**，不會呼叫 `OEE_EndLot()`、不會送 SECS `LotEnd` Event、不會上傳 BinQty/SocketLifeTime/TrayMapping。OEE 雲端 Lot 結束須由操作員於 Handler UI 按 `Lot End` 按鈕完成；Agent 應在操作員結束舊 Lot 之後再送下一個 `LotInfo_REQUEST`，否則將收到 Reply Code `2`

---

## 7. 附錄

### 7.1 指令速查表

| 功能 | 指令（Request）| 回傳（Reply）|
|------|----------------|--------------|
| 安全解鎖 | `RESUME_REQUEST` | `RESUME_REPLY` |
| 安全鎖定 | `PAUSE_REQUEST` | `PAUSE_REPLY` |
| 清除計數器（不關 OEE Lot）| `CLEAR_REPORT_REQUEST` | `CLEAR_REPORT_REPLY` |
| 設定批次（內嵌 OEE Start Lot）| `LotInfo_REQUEST` | `LotInfo_REPLY` |
| 下載 Setup File | `PP_DL_REQUEST` | `PP_DL_REPLY` |
| 遠端啟動 | `START_REQUEST` | `START_REPLY` |
| 查詢狀態 | `MAIN_STATUS_INQUIRE` | `MAIN_STATUS_GRANT` |
| 查詢批次 | `LotInfo_INQUIRE` | `LotInfo_GRANT` |
| 查詢版本 | `VERSION_INQUIRE` | `VERSION_GRANT` |
| 查詢 Setup File | `SETUP_FILE_NAME_INQUIRE` | `SETUP_FILE_NAME_GRANT` |

### 7.2 TCP 連線速查

```
Protocol  : TCP/IP
IP        : [Handler PC IP]
Port      : 6671
Mode      : Client connects to Handler (Server)
Encoding  : ASCII
Max Conn  : 1
```

封包格式：

```
STX(0x02) + Command + SOH(0x01) + V_Total + [SOH(0x01) + Data]... + ETX(0x03)
```

---

## 8. V2.0 變更說明（vs. V1.0 2026-04-15）

依超豐 2026-05-15 確認決議，本版本對 V1.0 做下列調整：

| 項目 | V1.0（2026-04-15） | V2.0（{DATE}）|
|------|---------------------|----------------|
| `LotInfo_REQUEST` Data | 三欄必填（LotID / OperatorID / FT-RT）| Handler **完全忽略**，由 OEE MO file 帶入 |
| `V_Total` | 必為 `3` | `0` ~ `3` 皆可 |
| Reply Code | 多個 | **僅 0 / 2 / 60 / 61 / 62 / 90** |
| 內嵌 OEE Start Lot | 無 | **`LotInfo_REQUEST` 成功即完成 OEE Start Lot** |
| 客戶端必送指令 | LotInfo + START | LotInfo + START（不變，但 LotInfo Data 可省略）|
| `LotInfo_REQUEST` 逾時建議 | 5 秒 | **>= 30 秒** |
| Setup File 下載 | 必送 `PP_DL_REQUEST` | 可由雲端 MO file 自動下載 |

> 客戶端 Agent 若已實作 V1.0，**僅需放寬 LotInfo timeout 至 30 秒、移除 20~51 區段的 Reply Code 判斷**即可向後相容；framing、序列、欄位皆不變。

---

## 9. 修訂歷史

| 版本 | 日期 | 修訂者 | 內容 |
|------|------|--------|------|
| V1.0 | 2026-04-15 | RD5 | 初版發行（PPT 格式寄送客戶）|
| V2.0 | {DATE} | RD5 | 依 2026-05-15 超豐確認決議：`LotInfo_REQUEST` 內嵌 OEE Start Lot；Handler 忽略 Data 欄位；Reply Code 簡化為 0/2/60/61/62/90；timeout 建議延長至 30 秒；對應 SPEC F899-008 v3 |

---

**Headquarters**
鴻勁精密股份有限公司　HON.PRECISION, INC. (HPI)
No. 11, Ln. 758, Sec. 3, Zhongqing Rd. Daya Dist., Taichung City, Taiwan 428012
Mail: sales@honprec.com / Tel: +886-425608752 / Fax: +886-4-25608755
www.honprec.com

Copyright (c) Hon. Precision, Inc. All Rights Reserved. — HonPrecision Confidential Information
"""

# ----------------- Markdown → HTML (only what we need) -----------------
def _inline(s: str) -> str:
    s = _html.escape(s)
    s = re.sub(r"\*\*(.+?)\*\*", r"<strong>\1</strong>", s)
    s = re.sub(r"`([^`]+)`", r"<code>\1</code>", s)
    return s

def md_to_html(md: str) -> str:
    lines = md.splitlines()
    out: list[str] = []
    i = 0
    in_code = False
    while i < len(lines):
        ln = lines[i]
        if ln.startswith("```"):
            if not in_code:
                out.append('<pre><code>')
                in_code = True
            else:
                out.append('</code></pre>')
                in_code = False
            i += 1
            continue
        if in_code:
            out.append(_html.escape(ln))
            i += 1
            continue
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
        if ln.startswith(">"):
            block = []
            while i < len(lines) and lines[i].startswith(">"):
                block.append(lines[i][1:].strip())
                i += 1
            out.append("<blockquote>" + "<br/>".join(_inline(b) for b in block) + "</blockquote>")
            continue
        if ln.lstrip().startswith(("- ", "* ")):
            block = []
            while i < len(lines) and lines[i].lstrip().startswith(("- ", "* ")):
                block.append(lines[i].lstrip()[2:])
                i += 1
            out.append("<ul>" + "".join(f"<li>{_inline(b)}</li>" for b in block) + "</ul>")
            continue
        m = re.match(r"^(\d+)\.\s+(.*)$", ln)
        if m:
            block = []
            while i < len(lines):
                m2 = re.match(r"^(\d+)\.\s+(.*)$", lines[i])
                if not m2:
                    break
                block.append(m2.group(2))
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

# HonPrecision red-themed HTML wrapper
HTML_TEMPLATE = """<!DOCTYPE html>
<html lang="zh-TW">
<head>
<meta charset="UTF-8"/>
<title>HT9045 Handler 遠端控制操作說明書 V2.0</title>
<style>
  :root {{
    --hp-red:    #C8102E;     /* 鴻勁紅 */
    --hp-red-dk: #9A0C23;
    --hp-navy:   #1F3864;     /* 標題深藍 */
    --hp-navy-2: #2E5BA8;
    --hp-bg:     #FFFFFF;
    --hp-bg-alt: #FAFBFC;
    --hp-border: #D9DEE6;
    --hp-text:   #1A1A1A;
    --hp-muted:  #6B7280;
    --hp-warn:   #FFFBEB;
    --hp-warn-b: #F59E0B;
  }}
  * {{ box-sizing: border-box; }}
  body {{
    font-family: "Microsoft JhengHei","Segoe UI","Noto Sans TC",Arial,sans-serif;
    max-width: 1080px; margin: 0 auto; padding: 0;
    color: var(--hp-text); line-height: 1.65; background: var(--hp-bg);
  }}
  .hp-banner {{
    background: linear-gradient(135deg, var(--hp-red) 0%, var(--hp-red-dk) 100%);
    color: #fff; padding: 28px 36px; margin-bottom: 0;
    border-bottom: 4px solid var(--hp-navy);
  }}
  .hp-banner .brand {{ font-size: 13px; letter-spacing: 2px; opacity: 0.9; }}
  .hp-banner h1 {{
    margin: 8px 0 4px 0; font-size: 28px; color: #fff; border: none; padding: 0;
  }}
  .hp-banner .subtitle {{ font-size: 14px; opacity: 0.95; }}
  .hp-content {{ padding: 24px 36px 60px 36px; }}
  h1 {{
    color: var(--hp-navy); border-bottom: 3px solid var(--hp-red);
    padding-bottom: 8px; margin-top: 32px; font-size: 24px;
  }}
  h2 {{
    color: var(--hp-navy); margin-top: 32px; padding: 8px 0 8px 14px;
    border-left: 6px solid var(--hp-red); font-size: 20px; background: var(--hp-bg-alt);
  }}
  h3 {{ color: var(--hp-navy-2); margin-top: 24px; font-size: 17px; }}
  h4 {{ color: var(--hp-navy-2); font-size: 15px; }}
  table {{
    border-collapse: collapse; width: 100%;
    margin: 12px 0 20px; font-size: 14px;
    box-shadow: 0 1px 2px rgba(0,0,0,0.04);
  }}
  th, td {{ border: 1px solid var(--hp-border); padding: 8px 12px; vertical-align: top; }}
  th {{
    background: var(--hp-navy); color: #fff; text-align: left; font-weight: 600;
  }}
  tr:nth-child(even) td {{ background: var(--hp-bg-alt); }}
  tr:hover td {{ background: #FFF5F6; }}
  code {{
    background: #F3F4F6; padding: 1px 6px; border-radius: 3px;
    font-family: Consolas,"Courier New",monospace; font-size: 13px; color: var(--hp-red-dk);
  }}
  pre {{
    background: #0F172A; color: #E2E8F0; padding: 14px 18px;
    border-radius: 6px; overflow-x: auto; font-size: 13px; line-height: 1.5;
    border-left: 4px solid var(--hp-red);
  }}
  pre code {{ background: transparent; color: inherit; padding: 0; }}
  blockquote {{
    border-left: 4px solid var(--hp-warn-b);
    background: var(--hp-warn); padding: 10px 14px; margin: 14px 0;
    color: #78350F; border-radius: 0 4px 4px 0;
  }}
  hr {{ border: 0; border-top: 1px dashed var(--hp-border); margin: 28px 0; }}
  ul, ol {{ padding-left: 24px; }}
  li {{ margin: 4px 0; }}
  strong {{ color: var(--hp-red-dk); }}
  .hp-footer {{
    margin-top: 48px; padding: 18px 36px; background: var(--hp-navy);
    color: #fff; font-size: 12px; text-align: center;
  }}
  .hp-footer .conf {{ color: #FCA5A5; font-weight: 600; }}
  @media print {{
    body {{ max-width: none; }}
    .hp-banner {{ break-after: avoid; }}
    h2 {{ break-after: avoid; }}
    table {{ break-inside: avoid; }}
  }}
</style>
</head>
<body>
<div class="hp-banner">
  <div class="brand">Hon.Precision · 鴻勁精密 · RD5</div>
  <h1>HT9045 Handler 遠端控制操作說明書</h1>
  <div class="subtitle">TCP Socket 遠端控制功能 — 客戶端操作指南　|　適用客戶：Greatek（超豐電子）　|　文件版本 V2.0　|　{date}</div>
</div>
<div class="hp-content">
{body}
</div>
<div class="hp-footer">
  鴻勁精密股份有限公司　HON.PRECISION, INC. (HPI)　|　No. 11, Ln. 758, Sec. 3, Zhongqing Rd. Daya Dist., Taichung City, Taiwan 428012<br/>
  Mail: sales@honprec.com　|　Tel: +886-425608752　|　Fax: +886-4-25608755　|　www.honprec.com<br/>
  Copyright (c) Hon. Precision, Inc. All Rights Reserved. — <span class="conf">HonPrecision Confidential Information</span>
</div>
</body>
</html>
"""

def main():
    md_path = OUT_DIR / f"{STEM}.md"
    html_path = OUT_DIR / f"{STEM}.html"
    md_path.write_text(MD, encoding="utf-8")
    body = md_to_html(MD)
    html_path.write_text(HTML_TEMPLATE.format(body=body, date=DATE), encoding="utf-8")
    print(f"[OK] wrote {md_path}")
    print(f"[OK] wrote {html_path}")

if __name__ == "__main__":
    main()
