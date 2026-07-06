# F899-005 — Monitor 第四鏡頭錄影記錄補齊

| 欄位 | 值 |
|------|-----|
| Feature ID | F899-005 |
| 版本 | V3.33.899.x |
| 對應 CASE-ID | CASE-20260422-002 |
| 客戶 | 超豐 |
| 狀態 | done |
| 影響模組 | Monitor |

## 1. 背景與動機
Monitor 配置四個鏡頭，但實際錄影僅三個鏡頭有記錄，造成事故追溯不全。

## 2. 需求摘要
補齊第四鏡頭的錄影記錄路徑，確保四個鏡頭皆有對應 log。

## 3. 條件錨點（Conditions）
- 客戶為超豐 (`CUSTOMER_CODE == CC_Greatek`) 時，第 4 支鏡頭 (Ch3) 與 Ch0 同步開關
- Monitor 模組初始化與斷線重連時各 channel 數迴圈上限為 4

## 4. 資料錨點（Data）
- `Monitor/MonitorInterface.h:93` — `int iStatus[4]`（陣列 size 3→4）
- Channel index：Ch0、Ch1、Ch2、Ch3（共 4 支）

## 5. 行為錨點（Behavior）
- `Monitor/MonitorInterface.cpp:39` — init 迴圈 `for(int i=0; i<4; i++)`（3→4）
- `Monitor/MonitorInterface.cpp:220` — 斷線重連補開 Ch3
- `csystem.cpp:4374` — 超豐 Ch3 同步 Ch0 開啟
- `Monitor/MonitorInterface.cpp:318` — 超豐 Ch3 同步 Ch0 關閉
- 註解標記：`//AI(+Ch3 ...) 20260422: 超豐增加第 4 支鏡頭`

## 6. 實作與驗證
- 驗證狀況：已排除問題。

## 7. 風險與相依
- (待補) 已知副作用 / 相依模組 / 回歸風險

## 8. Release Note 草稿

### 8.1 customer（對外，給終端客戶）
Monitor 四個鏡頭皆可正常記錄影像。

### 8.2 distributor（給代理商 / FAE）
修正 Monitor 第四鏡頭未錄影問題，現所有鏡頭皆有對應錄影檔。

### 8.3 internal（給 RD / QA）
修正 Monitor 模組第四鏡頭的錄影 init 邏輯，避免被 skip。
