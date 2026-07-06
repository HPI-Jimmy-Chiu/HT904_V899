# F899-003 — MAIN_STATUS_INQUIRE 任意狀態回覆修正

| 欄位 | 值 |
|------|-----|
| Feature ID | F899-003 |
| 版本 | V3.33.899.x |
| 對應 CASE-ID | CASE-20260422-004 |
| 客戶 | 超豐 |
| 狀態 | done |
| 影響模組 | Automation / SECSGEM |

## 1. 背景與動機
V3.33.899.8 測試發現 MAIN_STATUS_INQUIRE 指令在任何狀態下皆回覆「無資料」，無法正確反映機台目前狀態。

## 2. 需求摘要
修正狀態查詢指令邏輯，使其在所有正常運行狀態下都能回傳對應資料。

## 3. 條件錨點（Conditions）
- 收到 SECS/GEM 指令字串 `Command == "MAIN_STATUS_INQUIRE"` 時觸發回覆

## 4. 資料錨點（Data）
- 標準枚舉值統一以大寫 + 底線取代空白
- 來源：客戶側 GTK 命令字串

## 5. 行為錨點（Behavior）
- `Automation/automation.cpp:1027` — `else if(Command=="MAIN_STATUS_INQUIRE")` 加入分派分支，回覆當前主狀態
- 註解標記：`//AI(+漏接的MAIN_STATUS指令) 20260422`

## 6. 實作與驗證
- 驗證狀況：已修正並提供安裝包，待客戶現場驗證。

## 7. 風險與相依
- (待補) 已知副作用 / 相依模組 / 回歸風險

## 8. Release Note 草稿

### 8.1 customer（對外，給終端客戶）
修正狀態查詢指令異常，現可正確回報機台目前狀態。

### 8.2 distributor（給代理商 / FAE）
修正 MAIN_STATUS_INQUIRE 在 V899.8 後回覆「無資料」的問題，恢復正常狀態回報。

### 8.3 internal（給 RD / QA）
修正狀態查詢指令的回覆邏輯，避免在所有狀態下都進入 no-data 分支。
