---
description: "Use when: completing or reviewing HT9045 V899 C/C++ edits and needing a fixed validation checklist. 適用於 V899 變更後自我審查、編譯檢查、最小回歸驗證。"
applyTo: "HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422/**/*.{cpp,h,hpp}"
---

# HT9045 V899 Self Review

## 必做檢查

| 項目 | 規則 | 範例 |
|------|------|------|
| 寫入邊界 | 只確認本次變更落在 V899 版本目錄內，且未碰觸 AGENTS.md 禁改清單中的驅動與定義檔 | 僅修改 ainarm9045_2x4_16.cpp |
| AI 註解 | 新增或修改的邏輯需符合 AI 註解規範，格式與日期一致 | //AI(ht9045-v899) 20260402: guard retry path |
| 最小差異 | 不順手重排無關程式碼、不批次格式化、不改歷史註解風格 | 只改目標 if 區塊 |
| 編碼安全 | 若檔案含 Big5 中文註解或字串，避免整檔改寫；審查是否出現亂碼或替代字元 | 不出現 U+FFFD |
| 影響說明 | 回覆時說明影響函式、狀態機、風險與驗證方式 | 說明 iArmTask case 1000 影響 |

## 驗證層級

| 層級 | 何時需要 | 規則 |
|------|----------|------|
| 快速檢查 | 每次修改後都要做 | 先檢查本次變更檔案是否有語法或前端格式錯誤，並確認 AI 註解存在 |
| 專案編譯 | 修改共用標頭、核心狀態機、跨模組函式、全域變數時 | 優先對 V899 根目錄的 HT9045.bpr 做 BCB6 build 檢查 |
| 模組回歸 | 修改會改變流程判斷、重試、互鎖、Alarm 路徑時 | 提供對應模組的最小流程驗證建議 |
| 說明例外 | 因環境或工具限制無法編譯時 | 明確說明未執行 build、原因、以及建議現場補驗證 |

## BCB6 Build 檢查

| 項目 | 規則 | 範例 |
|------|------|------|
| 專案目標 | 以 V899 根目錄的 HT9045.bpr 為主，不使用其他版本的專案檔 | HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422/HT9045.bpr |
| 建構模式 | 一般變更先做增量 build；懷疑中間產物汙染或改動大範圍標頭時再做 rebuild | 先 build，必要時 rebuild |
| 前置條件 | 若缺少 BCB6 工具或必要輸出目錄，需在回覆中說明而非假設成功 | 說明未找到 bpr2mak 或 make |
| 結果呈現 | 簡要回報是否執行 build、成功或失敗、失敗重點訊息 | build failed at ilink32 unresolved external |

## 模組最小回歸

| 模組 | 最小驗證重點 | 範例 |
|------|--------------|------|
| InArm | Home 後取料、重試、放料、Alarm 分支至少覆蓋到被修改的 case | 驗證 iArmTask 1000 後是否能回到主流程 |
| OutArm | 從 Shuttle 取料、放到 Auto 或 Fix、失敗重試與清料路徑 | 驗證 DoPickFromShuttle 後放料結果 |
| Index | Front/Rear 動作、Socket 下壓、EP 或吸嘴互鎖、Destroy 路徑 | 驗證 DoTestYFront 條件切換 |
| Shuttle | 左右站移動、淨空互鎖、殘料或浮料檢查 | 驗證 AutoSHT1Task 主要分支 |
| 共用標頭 | 至少做一次專案編譯，必要時補受影響模組的流程驗證 | 修改 cpublic.h 後執行 build |