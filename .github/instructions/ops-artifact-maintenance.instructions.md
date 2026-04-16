---
description: "Use when: 維護 Prompt、Skill、Instruction、Hook、Script、日誌、ADR、Registry 等治理資產。適用於 ops maintenance、artifact review、knowledge upkeep。"
applyTo: [".github/prompts/*.prompt.md", ".github/instructions/*.instructions.md", ".github/agents/*.agent.md", ".agents/skills/**/SKILL.md", ".github/hooks/*.json", "scripts/ops/**/*.ps1", "docs/ops/**/*.md"]
---

# OPS Artifact Maintenance

## 基本原則

| 項目 | 規則 | 範例 |
|------|------|------|
| 單一責任 | 一個檔案只做一件事 | 一個 Instruction 不同時混測試、命名、部署 |
| 觸發品質 | description 要能被正確發現 | Use when: skill maintenance、registry review |
| 不重複 | 模板、Prompt、Instruction 不互抄全文 | 細節放模板，流程放 Prompt |
| 變更留痕 | 有語義變更就更新週檢、ADR 或 registry | 修改 Skill 定位後更新 registry |

## Prompt / Skill / Instruction 維護要求

| 類型 | 維護要求 | 驗證點 |
|------|---------|-------|
| Prompt | 任務要單一、步驟要可執行 | 是否一個 Prompt 只做一個任務 |
| Skill | description 要含觸發詞，body 保持精簡 | 是否需要拆 references/ |
| Instruction | 一個檔案一個關注點 | applyTo 是否過寬 |
| Hook | 只做明確且可審查的自動化 | 是否會造成誤攔截 |
| Script | 單一責任、可重跑、可驗證 | 是否能單獨執行 |

## 變更後同步項目

- 若改變流程：更新 Prompt 或模板
- 若改變規則：更新 Instruction 或 Checklist
- 若改變強制機制：更新 Hook、Script 與 Policy
- 若改變定位或用途：更新 skill-registry 或 ADR

## 禁止事項

- 不要把 README 當成 Prompt 或 Skill 的替代品
- 不要用一個大檔案承接所有治理需求
- 不要修改 Hook 卻不檢查對應 Script 或 Policy
- 不要長期保留已失效的 description
