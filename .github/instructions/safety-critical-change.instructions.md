---
description: "Use when: 修改運動控制、IO、互鎖、安全邏輯、模式切換、警報、執行期設定。適用於 safety-critical changes、motion、interlock、runtime config。"
applyTo: ["HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422/**/*.{cpp,h,hpp}", "system/**/*.{ini,csv,dat,db,DB}", "config/**/*.ini", "CFG/**/*.cfg"]
---

# Safety Critical Change

> **本 instruction 觸發時，視為「最高推理等級」(等同 Claude Code `xhigh` / `max` effort)。**
> 不能漏改、不能略過驗證、不能只看單檔。

## 推理等級要求 (Reasoning Effort = XHIGH)

本類變更必須遵守以下「深度推理」流程，不得用「快速回答」處理：

| 階段 | 必做 | 對應 Claude Code 行為 |
|------|------|------|
| 1. Plan | 先列出修改目標、影響面、所有對稱分支（InArm↔OutArm、Front↔Rear、case↔else） | 等同 `xhigh` planning |
| 2. Self-Critique | 在動手前自問：是否漏了 retry / Alarm / Pause / Home / AutoClean 路徑？是否破壞既有互鎖順序？ | 等同 4.7「自我驗證」 |
| 3. Cross-File Read | 至少讀 1 個相依標頭（cpublic.h / MachineType.h / cmydef.h / MyKitSuck.h / mycylin.h），確認型別與旗標語意 | 不允許「只看當前檔」 |
| 4. Implement | 最小差異，含 `//AI(...) YYYYMMDD:` 註解 | 同既有規則 |
| 5. Self-Review | 完成後再做一次「假裝是 reviewer」掃過 diff，列出 Critical / Major / Minor | 等同 `/ultrareview` 精神 |
| 6. Verify Plan | 提出最少一個可驗證流程或 BCB build 建議 | 同 v899-self-review |

> 若使用 Claude Code CLI，本類任務應顯式設 `--effort xhigh`，並在合併前對 diff 跑 `/ultrareview`。
> 在 VS Code Copilot Chat 環境，必須以本 instruction 的「6 階段」取代 effort 參數，不得跳階段。

## 禁止行為（嚴格遵守，不得「順手」做）

- ❌ 不准在沒讀相依標頭的情況下改互鎖順序
- ❌ 不准對「對稱模組」只改一邊（InArm 改了沒改 OutArm 同位置 = 不通過）
- ❌ 不准跳過 Plan / Self-Critique 直接動 code
- ❌ 不准在回覆中省略「影響面」與「驗證方式」
- ❌ 不准用 lambda / auto / nullptr / range-for（pre-C++11）
- ❌ 不准把 Big5 檔案重存成 UTF-8

## 變更前必做

| 檢查項目 | 規則 | 範例 |
|------|------|------|
| 變更分類 | 先判定是否碰到運動、IO、互鎖、模式切換或共享設定 | 修改馬達前提條件屬安全變更 |
| 寫入邊界 | 先確認檔案是否在白名單內 | 舊版或封存碼一律唯讀 |
| 備份 | 共享設定或運行期設定先備份 | system/、config/、CFG/ |
| 回退 | 改之前先定義 rollback 路徑 | 還原檔案、回退旗標、停用 Hook |

## HT9045 特別注意

| 項目 | 規則 |
|------|------|
| 馬達控制 | Motor/ 目錄下修改前必須確認實體機台配置 |
| 氣缸互鎖 | mycylin.h 定義的氣缸含 2 switch + 2 sensor，修改順序敏感 |
| 溫控 | TempCtrl/ 修改需確認加熱器型號（DT4848/KT4H/TMC401/UT100/WT404） |
| 多站點 | 修改 InArm/OutArm 需注意站點配置（1x1, 1x2, 2x4, 2x4_16）影響範圍 |
| 共用設定 | system/Gerneral.ini、Mot_Table.csv、IO_Table.csv 影響全版本 |

## 實作中要求

| 項目 | 規則 |
|------|------|
| 最小修改 | 只改必要區塊，不順手重構 unrelated 區域 |
| 關聯盤點 | 記下受影響的模組、模式與流程 |
| 驗證設計 | 在改碼前先想好如何驗證 |
| AI 註解 | 每次修改加 `//AI(AgentName) YYYYMMDD: 描述` |

## 變更後必做

- 完成 docs/ops/checklists/safety-change-review.md
- 更新 Daily Worklog
- 重大規則變更要新增 ADR
- 若安全規則改變，更新 Hook / Script / Policy / Instruction 至一致狀態
- **強烈建議**：對 diff 跑一次 Claude Code `/ultrareview`（見下方）

## /ultrareview 使用建議（Claude Code CLI）

本類變更**強烈建議**在 BCB6 build 通過後、push 前對 diff 跑一次 `/ultrareview`：

```bash
cd D:\HT9045
claude
> /ultrareview --staged
重點審：機械互鎖順序、Alarm 路徑漏洞、retry/skip 對稱性、Big5 註解完整性、AI 註解格式
```

範圍以「本次受影響模組」為限，避免燒掉免費額度（Pro/Max 各 3 次）：
- 改 InArm → 同時跑 `ARMS/ainarm9045_2x4_16.cpp`
- 改 Home 流程 → 同時跑 `uhome.cpp`
- 改互鎖 → 加上對應的 `csystem.cpp` / `Check_AcMotor`
