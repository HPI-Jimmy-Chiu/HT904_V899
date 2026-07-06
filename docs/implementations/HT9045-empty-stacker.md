# Implementation：HT9045 Empty Stacker

> 對照契約：[stacker-cylinder-separator](../mechanisms/stacker-cylinder-separator.md)
> 機台：HT9045 V3.33.899.0
> 實作檔案：`HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422/asendic_Empty.cpp`
> 風格：個別檔案派

---

## 1. 角色映射

| 契約角色 ID | 本機實際命名 | 定義位置 |
|-------------|------------|---------|
| `ACT_LIFT` | `C_Empty_Up` | `cmydef.h` |
| `ACT_SEPARATOR` | `Cylinder[C_EmptyLoaderZ_Select]` | `cmydef.h` |
| `SEN_WORK_HAS_TRAY` | `Sen[SenEmptyCarHasTray]` | `cmydef.h` |
| `SEN_LIFT_DOWN` | `Sen[SenEmptyCCWDete]` | `cmydef.h` |
| `SEN_STACK_HAS_TRAY` | `Sen[SenEmptySelectHasTray]` | `cmydef.h` |
| `SEN_STACK_FULL` | （無對應 sensor） | — |
| `FLAG_PAUSE` | `Pause` | `cmydef.h` |
| `FLAG_AUTO_RUN` | `bAutoRunMode` | `cmydef.h` |
| `FLAG_CLEANOUT_MODE` | `bAutoArmCleanoutFinish` 等 | `cmydef.h` |

---

## 2. 函式進入點

| 進入點 | 行號 | 描述 |
|--------|------|------|
| 主流程入口 | `asendic_Empty.cpp` L55 起的 switch | case 1 入口檢查 |
| 升頂動作 | L191 `CylinderUp(C_Empty_Up)` | case 60 |
| 分離爪推 | L200 `Cylinder[C_EmptyLoaderZ_Select].Push()` | case 100 |
| 中停動作 | L204 `CylinderMiddle(C_Empty_Up)` | case 200 |
| 分離爪退 | L221 `Cylinder[C_EmptyLoaderZ_Select].Pop()` | case 300 |
| 下降動作 | L234 `CylinderLower(C_Empty_Up)` | case 410 |
| 工作位驗證 | L240–L260 case 420 | sensor 驗證 |

---

## 3. case 編號對齊

| 契約 case | 本機 case | 一致 |
|-----------|----------|------|
| 1 | 1 | ? |
| 20 | 20（含 JerryYang 20161128 註解：「新增防護,避免Empty的Middle氣缸sensor一直亮著導致hang up」） | ? |
| 50 | 50 / 52 / 53（細分多步） | ?（中間步用 52/53） |
| 60 | 60 | ? |
| 100 | 100 | ? |
| 200 | 200 | ? |
| 300 | 300 | ? |
| 400 | 400 | ? |
| 410 | 410 | ? |
| 420 | 420 | ? |

---

## 4. 風格特徵

- **個別檔案**：`asendic_Loader.cpp` / `asendic_Empty.cpp` / `asendic_Color.cpp` 各自一檔
- **變數命名**：每實例獨立命名（`C_Empty_Up` vs `C_Color_Up` vs `C_Load_Up`）
- **狀態變數**：個別 task 變數
- **函式呼叫**：直接 `CylinderUp(C_Empty_Up)` 不傳 idx

---

## 5. 與契約的偏差

| 偏差 | 說明 | 是否合規 |
|------|------|---------|
| 缺 `SEN_STACK_FULL` 對應 sensor | 機構未配備滿盤偵測 | ? 契約允許可選 |
| case 50 拆成 50/52/53 | 細分前置步驟 | ? 契約允許中間值 |
| 含 `bFlagAutoArmCleanoutFinish` 等多個模式 flag | 機台特定路徑 | ? 屬 §4 慣例範圍 |
| Loader 區域額外用 `C_LoaderUpPress` | 壓盤輔助 | ?? ? 單機特例（契約 N8） |

---

## 6. 同機台兄弟實例

HT9045 此機構共 3 個實例（屬本契約涵蓋範圍）：

| 實例 | 主檔 | 特殊點 |
|------|------|--------|
| Loader | `asendic_Loader.cpp` | 額外 `C_LoaderUpPress`（?） |
| Empty | `asendic_Empty.cpp` | **本份對照基準** |
| Color | `asendic_Color.cpp` | 標準 |

> 其他 `asendic_*.cpp`（Auto / Auto2 / Auto_RT / Loader_RT / Scanner）屬其他子能力或變體，不在本契約涵蓋範圍。

---

## 7. 已知 bug 歷史（可填）

> 從 `//AI(...)` 註解或 ADR 反查後填入

| 日期 | bug | 修復 case |
|------|-----|----------|
| 2016-11-28 | Empty Middle 氣缸 sensor 卡亮致 hangup | 新增 case 20 防護 |
| 2021-07-28 | 分離汽缸未確認打開即動作（Sam） | case 1 加 `OnStatus()` 檢查 |
| 2022-07-12 | CarHasTray 判斷漏（kevin） | case 100 補條件 |

---

## 8. 變更歷史

| 日期 | 變更 |
|------|------|
| 2026-05-07 | 初版（與 HT172 對照建立） |
