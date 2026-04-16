---
mode: ask
model: GPT-5.3-Codex
description: "HT9045 版本問題調查模板（版本程式 + 共用參數）"
---

請用 HT9045 除錯流程協助我排查問題，並同時分析版本程式與共用參數。

輸入資訊：
- 目標版本：${input:targetVersion:例如 HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260323}
- 問題現象：${input:symptom:例如 OutArm 偶發取料失敗並 Alarm}
- 發生站點/模組：${input:module:例如 OutArm + OutShuttle}
- 觸發條件：${input:trigger:例如 連續運轉 2 小時後、切換 lot 後}
- 期望輸出：${input:expectation:例如 根因 + 最小修正方案 + 驗證步驟}

請依序輸出：
1. 問題定位（版本程式面）
2. 參數比對重點（system/config/CFG/IniData/setup.inf/CurrentSetupData.txt）
3. 根因與證據
4. 最小修正與風險
5. 現場驗證步驟
