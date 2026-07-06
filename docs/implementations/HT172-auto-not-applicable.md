# Implementation Note：HT172 Auto — Not Applicable

> 對照契約：[stacker-cylinder-stackonly](../mechanisms/stacker-cylinder-stackonly.md)
> 機台：HT172 V1.0.25
> **本機台不屬於此契約涵蓋家族**

---

## 1. 結論

HT172 的「分類出料」功能由 **Magazine 整柱搬移機構**實作，
不屬於 `stacker-cylinder-*` 機構家族。

---

## 2. 證據

### 2.1 程式碼結構

HT172 對應「出料分類」的模組：
- `HT172_Module/aSortArm.cpp` — 排序手臂（XY motion + Z pick/place）
- `HT172_Module/aMagazine.cpp` — Magazine 控制
- `HT172_Module/aMagArm.cpp` — Magazine 搬移手臂
- `HT172_Module/aTrayArm.cpp` — Tray 搬移手臂

### 2.2 Auto Tray 資料模型

HT172 的 `Auto->MAutoTray[iWhichAuto]` 是**資料層物件**，
不直接控制汽缸升降。實際的 Tray 進出由 `aMagArm` 操作 Magazine 完成。

### 2.3 缺少 Stacker 角色

對應 `stacker-cylinder-stackonly` 契約 §4 的角色：

| 契約角色 ID | HT172 對應 | 說明 |
|-------------|-----------|------|
| `ACT_LIFT` | （無 Auto 區汽缸） | Magazine 內部由步進馬達升降 |
| `ACT_SELECTOR` | （無） | — |
| `ACT_SIDE_FIXER` | （無） | Magazine 結構自身固定 |
| `ACT_EDGE_PUSH` | （無） | — |
| `ACT_UP_PRESS` | （無） | — |
| `SEN_TRACK_HAS_TRAY` | （無對應） | — |
| `SEN_FULL` | Magazine 容量計數 | 邏輯層判斷 |

→ 全部主要 actuator 都對不上 → 機構家族不同。

---

## 3. 為什麼還要寫這份「不適用」對照

### 3.1 防止誤套

未來工程師（或 AI）若拿 HT9045 Auto 程式碼來 port 到 HT172 類似機台，
應該從本份立即得知「**走錯方向了**，要找的契約是 magazine-* 而非 stacker-*」。

### 3.2 觸發新契約規劃

HT172 的 Magazine 機構應該另寫一份契約：

```
docs/mechanisms/
  magazine-stepmotor-vertical.md   ← 待寫
```

涵蓋：
- Magazine 內部 Z 軸步進馬達升降
- 整柱 Tray 進出搬移
- Magazine 與 SortArm 的交握

### 3.3 強化「契約適用範圍」紀律

每份契約都要回答：
- 哪些機台**符合**（在 §8 列出）
- 哪些機台**不符合**（在實作目錄寫 NOT-APPLICABLE 對照）

---

## 4. 對契約的回饋

從 HT172 比對結果，**契約 stacker-cylinder-stackonly 應補強**：

### 4.1 §0 增加排除清單

明確列出：
- ? Magazine 機構（HT172 風格）
- ? 步進馬達升降（無汽缸）
- ? Tray 整柱搬移（非單片堆疊）

### 4.2 §9 決策樹增加分支

新機台選契約時的問題：「你的出料區是堆疊還是 Magazine？」
→ 已在契約 §9 涵蓋。

---

## 5. 變更歷史

| 日期 | 變更 |
|------|------|
| 2026-05-07 | 初版（標註 HT172 不適用，啟發 magazine 契約規劃） |
