---
description: "Use when: 記錄每日工作日誌、工作交接、Session 收尾。適用於 daily worklog、handover、今日進度、明日接續。"
---

# 更新每日工作日誌

請在 HT9045 專案中新增或更新當日工作日誌。

## 目標路徑

- 日誌：docs/ops/daily/YYYY-MM-DD.md
- 模板：docs/ops/templates/daily-worklog.tmpl.md

## 執行步驟

1. 先檢查今天的日誌檔是否存在；若不存在，依模板建立。
2. 整理今天完成的工作、觸及模組、驗證與文件更新。
3. 把風險、阻塞與明日接續點寫清楚。
4. 如果今天有修改 Skill、Prompt、Instruction、Hook、Script，寫入經驗留存欄位。
5. 如果任務會跨 Session 延續，同步更新 memories/repo/ACTIVE.md。

## HT9045 專案脈絡

- 目前版本：V3.33.899.0_20260323_Jimmy_20260330
- 版本目錄：HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260330/
- 共用設定：system/、config/、CFG/、IniData/
- 16 個 Skill 位於 .agents/skills/、3 個 Instruction 位於 .github/instructions/

## 輸出要求

- 保持精簡，但要讓明天的人能直接接手。
- 不寫空話，只寫已完成、已確認、待處理與風險。
- 如果需要沉澱成 ADR、Skill 或規則，請在最後列出建議。
