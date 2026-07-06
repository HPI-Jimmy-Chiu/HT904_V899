# F899-004 — Auto Clean Vacuum / Air On delay 預設值改為 1 秒

| 欄位 | 值 |
|------|-----|
| Feature ID | F899-004 |
| 版本 | V3.33.899.x |
| 對應 CASE-ID | CASE-20260423-002 |
| 客戶 | 超豐 |
| 狀態 | done |
| 影響模組 | AutoClean |

## 1. 背景與動機
Auto Clean 中 Vacuum delay time 與 Air On delay time 預設值原為 60，導致每次清潔耗時過長。

## 2. 需求摘要
將 Auto Clean 的 Vacuum delay 與 Air On delay 預設值改為 1 秒，加快清潔週期。

## 3. 條件錨點（Conditions）
- Auto Clean 流程啟動時讀取 `setup.inf` 的 `[AutoClean]` 區段
- 當設定檔中 `InArmVacuum` 缺項時套用 default 值

## 4. 資料錨點（Data）
- `cprod.h:2372` — `double dAutoClean_InArmVacuum`
- 設定檔：`setup.inf [AutoClean] InArmVacuum`
- default 由 60 改為 1.00（單位：秒）

## 5. 行為錨點（Behavior）
- `AutoClean/uCleaning.cpp:148` — `ReadIniData(... "InArmVacuum", 1.00)` default 由 60→1
- `AutoClean/uCleaning.cpp:926` — UI 寫回 `atof(edtInArmVacuum->Text)`
- `AutoClean/uCleaning.cpp:1124` — `WriteIniData(... "InArmVacuum", ...)`
- `cinitial.cpp:6343` — Auto Clean PnP delay：`dArmData[0] = ... * 100`
- 註解標記：`//AI(ht9045-v899) 20260423: Greatek 要求 default 60->1`

## 6. 實作與驗證
- 驗證狀況：已修正並提供安裝包。

## 7. 風險與相依
- (待補) 已知副作用 / 相依模組 / 回歸風險

## 8. Release Note 草稿

### 8.1 customer（對外，給終端客戶）
Auto Clean 預設延遲時間優化為 1 秒，縮短清潔週期。

### 8.2 distributor（給代理商 / FAE）
Auto Clean Vacuum/Air On delay 預設值由 60 改為 1，可由 setup 介面調整。

### 8.3 internal（給 RD / QA）
修改 AutoClean 預設值寫入點，新建機台預設套用 1s。已存在的設定檔不會自動覆寫。
