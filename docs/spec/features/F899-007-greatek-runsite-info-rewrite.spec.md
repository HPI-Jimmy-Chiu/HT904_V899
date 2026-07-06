# F899-007 — 超豐 Site 開關時即時重寫 GTK info.txt RunSite

| 欄位 | 值 |
|------|-----|
| Feature ID | F899-007 |
| 版本 | V3.33.899.x |
| 對應 CASE-ID | CASE-20260428-002 |
| 客戶 | 超豐（Greatek） |
| 狀態 | done |
| 影響模組 | HS_Function / Command / ProductionInfo |
| 關聯 ticket | BVL-3766 |

## 1. 背景與動機
超豐客戶 GTK Loader 透過 `C:\GTK_Control\flag\info.txt` 中 `RunSite` 欄位判斷哪些 site 可生產。Start Lot 後若操作員手動關 / 開 site，原本的 info.txt 不會即時重寫，造成 RunSite 欄位空白或過時，下游 GTK 動作異常。

## 2. 需求摘要
- Start Lot 後在 HS 介面手動 close / open site 時，即時重寫 `info.txt`
- 透過 GPIB `SETSITEMAP_` 變更 site map 時，也要即時重寫
- 變更限定 `CUSTOMER_CODE == CC_Greatek`，不影響其他客戶

## 3. 條件錨點（Conditions）
- `CUSTOMER_CODE == CC_Greatek`
- `fProductionInfo != NULL`（ProductionInfo 表單已實例化）
- 觸發點：HS Close/Open Site 介面動作 + GPIB `SETSITEMAP_` 指令

## 4. 資料錨點（Data）
- 寫入路徑：`C:\GTK_Control\flag\info.txt`
- 寫入欄位：`RunSite`（site map 字串）
- 來源：`fProductionInfo->SaveInfoFileWhenStart()` 內部組裝

## 5. 行為錨點（Behavior）
- `HS_Function.cpp:26` — `#include "ProductionInfo.h"`（//AI(BVL-3766) 20260420）
- `HS_Function.cpp:3457` — `TFormHS::CloseOpenSite` 結束後：
  - `if(CUSTOMER_CODE==CC_Greatek && fProductionInfo!=NULL) fProductionInfo->SaveInfoFileWhenStart();`
- `Command.cpp:5258` — GPIB `SETSITEMAP_` 處理後同樣重寫 info.txt
- 註解標記：`//AI(BVL-3766) 20260420 / 20260428`

## 6. 實作與驗證
- 驗證狀況：已修正並提供安裝包，列入 BVL-3766 工單追蹤。

## 7. 風險與相依
- 限 Greatek 客戶；其他客戶仍走 Start Lot 一次性寫入舊邏輯
- 相依 `fProductionInfo` 表單：若該表單未開啟則 skip，不會 crash
- 與 `Auto Site Mapping` 旗標 (`bI21RTmodeDonotRunSiteMapping`) 不互斥

## 8. Release Note 草稿

### 8.1 customer（對外，給終端客戶）
Start Lot 後手動關 / 開 site 時，GTK 控制檔案會即時更新 RunSite 欄位，避免下游動作異常。

### 8.2 distributor（給代理商 / FAE）
針對超豐（Greatek）客戶：HS 介面手動 close / open site，以及 GPIB `SETSITEMAP_` 指令觸發後，會即時呼叫 `SaveInfoFileWhenStart()` 重寫 `C:\GTK_Control\flag\info.txt` 的 RunSite 欄位。其他客戶不變。

### 8.3 internal（給 RD / QA）
`HS_Function.cpp::CloseOpenSite` 與 `Command.cpp` GPIB `SETSITEMAP_` handler 末段加入 Greatek 限定的 `fProductionInfo->SaveInfoFileWhenStart()` 呼叫。對應 BVL-3766。注意 `fProductionInfo` 為可選表單，需 NULL 檢查。
