---
description: "Use when: adding or changing HT9045 V899 C/C++ code that must carry AI modification comments. 適用於 //AI(...) YYYYMMDD: 描述 規則。"
applyTo: "HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260330/**/*.{cpp,h,hpp}"
---

# HT9045 V899 AI Annotation Rules

## 註解格式

| 情境 | 規則 | 範例 |
|------|------|------|
| 標準格式 | 一律使用 //AI(AgentName) YYYYMMDD: 描述 | //AI(ht9045-v899) 20260402: add retry guard |
| 日期 | 日期固定使用 YYYYMMDD | 20260402 |
| AgentName | V899 範圍的新變更預設使用 ht9045-v899，以對齊現有註解慣例；若使用者明確指定其他名稱再改 | //AI(ht9045-v899) 20260402: ... |
| 描述內容 | 描述要寫動作與原因，不寫空泛字樣如 fix、update | add sensor guard before retry |

## 放置位置

| 情境 | 規則 | 範例 |
|------|------|------|
| 新增宣告/新行 | 在新增程式碼前一行放獨立註解 | 先放 //AI(...) 再放 bool bFlag; |
| 修改既有單行 | 在該行行尾加註解 | iRetryCount=0; //AI(ht9045-v899) 20260402: reset after alarm |
| 多行區塊 | 只在區塊第一行前加 1 行獨立註解，不要每行都加 | 在 if/for 區塊前標註 |
| 歷史註解 | 不重寫既有 AI 註解格式，只對本次新改動套用新規則 | 保留 //AI(general) 歷史內容 |

## 審查要點

| 項目 | 規則 | 範例 |
|------|------|------|
| 一致性 | 同一個 patch 內的 AI 註解格式保持一致 | 全部使用 //AI(ht9045-v899) |
| 精簡性 | 一個邏輯區塊用一則註解即可，不製造註解噪音 | 區塊前註解一次 |
| 可追溯性 | 註解要讓人看得出為何改，而不是只看出有改 | guard null tray before swap |