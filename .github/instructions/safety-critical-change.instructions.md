---
description: "Use when: 修改運動控制、IO、互鎖、安全邏輯、模式切換、警報、執行期設定。適用於 safety-critical changes、motion、interlock、runtime config。"
applyTo: ["HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260330/**/*.{cpp,h,hpp}", "system/**/*.{ini,csv,dat,db,DB}", "config/**/*.ini", "CFG/**/*.cfg"]
---

# Safety Critical Change

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
