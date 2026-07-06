---
description: "Use when: editing HT9045 V899 C/C++ source under the only writable version folder. 適用於 BCB6、Big5、VCL、pre-C++11、寫入邊界控管。"
applyTo: "HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422/**/*.{cpp,h,hpp}"
---

# HT9045 V899 C/C++ Guardrails

## 寫入邊界

| 項目 | 規則 | 範例 |
|------|------|------|
| 可寫程式碼 | 只允許修改 HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422 內的 C/C++ 原始碼 | 可修改 ainarm9045_2x4_16.cpp |
| 其他版本 | 其他 HT9011UC_Code_* 與 HT9046LS_Code_* 一律視為唯讀參考 | 可讀不可改 |
| 共用參數 | system、config、CFG、IniData、setup.inf、CurrentSetupData.txt 預設只讀，除非使用者明確要求 | 先分析再確認 |
| 禁改檔 | 驅動與定義檔依 AGENTS.md 禁改清單處理 | 不修改 CSmc.h、sqlite3.h |

## BCB6 / VCL 限制

| 項目 | 規則 | 範例 |
|------|------|------|
| C++ 標準 | 以 pre-C++11 為準，不使用 auto、nullptr、lambda、range-for | 用 int i=0; 不用 auto i |
| 字串型別 | VCL 與既有流程優先使用 AnsiString，避免無必要引入 std::string | AnsiString sMsg |
| 例外擴充 | 不主動引入新 STL/第三方依賴或需要較新編譯器的語法 | 不新增 <thread> |
| 表單檔 | .dfm 不手動重排格式，除非使用者明確要求處理表單 | 保持 Delphi 格式 |

## Big5 編碼安全

| 項目 | 規則 | 範例 |
|------|------|------|
| 最小修改 | 優先只改動 ASCII 範圍的最小區塊，避免整檔重寫 | 只改 if 條件與英文註解 |
| 中文內容 | 若變更會碰到 Big5 中文註解或字串，先評估編碼風險，避免把檔案重存成 UTF-8 | 不大範圍格式化 |
| 全檔操作 | 禁止無關的排序、格式化、批次置換，以免破壞非 ASCII 內容 | 不做 whole-file format |

## 編輯後檢查

| 項目 | 規則 | 範例 |
|------|------|------|
| 影響面 | 說明影響函式、狀態機或模組，特別是 InArm、OutArm、Index、Shuttle | 說明 iArmTask 分支影響 |
| 驗證 | 能建議就提供最小回歸測試或編譯檢查 | 建議跑對應流程與 BCB6 build |
| 變更範圍 | 不順手修 unrelated code，不重排歷史 AI 註解 | 僅修目標區塊 |