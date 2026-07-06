# F899-006 — 甬矽 EventLog 時間欄位格式（去除雙引號與前綴空格）

| 欄位 | 值 |
|------|-----|
| Feature ID | F899-006 |
| 版本 | V3.33.899.x |
| 對應 CASE-ID | CASE-20260429-001 |
| 客戶 | 甬矽（FOREHOPE_NINGBO） |
| 狀態 | done |
| 影響模組 | EventLog / Note |

## 1. 背景與動機
甬矽客戶 EventLog 匯出後其下游解析工具發現「時間欄位」帶有雙引號 (`"`) 與前綴空格，導致欄位回歸前的乾淨格式失效。其他客戶仍須維持 Steven 20251107 制定的新格式。

## 2. 需求摘要
針對 `CUSTOMER_CODE == CC_FOREHOPE_NINGBO` 客製化 EventLog 行格式：
- 時間欄位 **不加** 雙引號
- 時間欄位 **不加** 前綴空格
- 其他客戶維持新方法（含雙引號 + 前綴空格）

## 3. 條件錨點（Conditions）
- `CUSTOMER_CODE == CC_FOREHOPE_NINGBO` 走甬矽分支
- 其他 CUSTOMER_CODE 走 default 分支（Steven 20251107 新方法）

## 4. 資料錨點（Data）
- 欄位順序：date, time, sJamArea, sJamCode, sRecovery, sPassTime, sDuplicateError, sMessage, sErrPart, sRecipeName
- 時間格式：`%02d:%02d:%02d.%03d`
- 寫入檔：EventLog 文字檔（由 note.cpp 組裝）

## 5. 行為錨點（Behavior）
- `note.cpp:6680` — `else if(CUSTOMER_CODE==CC_FOREHOPE_NINGBO)` 分支：
  - `Str+=","+AnsiString().sprintf("%02d:%02d:%02d.%03d", h, n, s, ms);`（無雙引號 / 無前綴空格）
- `note.cpp:6694` — default 分支：
  - `Str+=",\""+AnsiString().sprintf(" %02d:%02d:%02d.%03d", h, n, s, ms)+"\"";`（含雙引號 + 前綴空格）
- 註解標記：`//AI(ht9045-v899) 20260429: 甬矽要求 EventLog 時間欄不加雙引號與前綴空格`

## 6. 實作與驗證
- 驗證狀況：已修正並提供安裝包，待客戶驗證。

## 7. 風險與相依
- 限定 `CC_FOREHOPE_NINGBO`，不影響其他客戶 EventLog 解析
- 若新增客戶需要相同格式，須新增 `else if` 分支或抽共用 helper

## 8. Release Note 草稿

### 8.1 customer（對外，給終端客戶）
EventLog 匯出格式調整，時間欄位回歸無引號標準格式。

### 8.2 distributor（給代理商 / FAE）
針對甬矽（FOREHOPE_NINGBO）客戶，EventLog 時間欄位不再加雙引號與前綴空格，回歸舊版乾淨格式；其他客戶維持新版格式不變。

### 8.3 internal（給 RD / QA）
`note.cpp` EventLog 寫入分支新增 `CC_FOREHOPE_NINGBO` 客製化邏輯，去除時間欄雙引號與前綴空格。其他客戶 default 分支維持 Steven 20251107 新方法（含雙引號 + 前綴空格）。
