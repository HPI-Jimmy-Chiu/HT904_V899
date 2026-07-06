# Implementation：HT172 Empty1 Stacker

> 對照契約：[stacker-cylinder-separator](../mechanisms/stacker-cylinder-separator.md)
> 機台：HT172 V1.0.25
> 實作檔案：`HT172_Program_V1.0.25.0_20260309/HT172_Module/aLoader.cpp::DoLoadNewTrayToEmpty1Car`
> 風格：陣列 idx 派

---

## 1. 角色映射

| 契約角色 ID | 本機實際命名 | 定義位置 |
|-------------|------------|---------|
| `ACT_LIFT` | `C_Up[eEmpty1]` + `AutoCylinderUp/Middle/Lower(reset, idx)` 包裝 | `database.h` |
| `ACT_SEPARATOR` | `C_Separatory[eEmpty1]` | `database.h` |
| `SEN_WORK_HAS_TRAY` | `Sn_CarHasTray[eEmpty1]` | `database.h` |
| `SEN_LIFT_DOWN` | （無獨立 sensor，由 `Sn_CarHasTray` 兼任） | — |
| `SEN_STACK_HAS_TRAY` | `SnSelectHasTray[eEmpty1]` | `database.h` |
| `SEN_STACK_FULL` | `Sn_CarIsFull[eEmpty1]` | `database.h` |
| `FLAG_PAUSE` | （依 `HSys.Sys.RunMode` 判斷） | `csystem.h` |
| `FLAG_AUTO_RUN` | `HSys.Sys.RunMode == Run_Auto` | `csystem.h` |
| `FLAG_TRAY_FEED_MODE` | `HSys.Sys.RunMode == Run_TrayFeed` | `csystem.h` |
| `FLAG_CLEANOUT_MODE` | `HSys.Sys.RunMode == Run_CleanOut` + `bFlagAutoArmCleanoutFinish` | `cmydef.h` |

---

## 2. 函式進入點

| 進入點 | 位置 | 描述 |
|--------|------|------|
| 主流程入口 | `aLoader.cpp::DoLoadNewTrayToEmpty1Car` L1455 | 含 reset 機制 |
| 升頂動作 | L1488 `AutoCylinderUp(true, iEmpty1)` + L1493 完成檢查 | case 50/60 |
| 分離爪推 | L1499 `C_Separatory[iEmpty1]->Push()` | case 100 |
| 中停動作 | L1504 `AutoCylinderMiddle(true, iEmpty1)` | case 200 |
| 分離爪退 | L1518 `C_Separatory[iEmpty1]->Pop()` | case 300 |
| 下降動作 | L1525 `AutoCylinderLower(true, iEmpty1)` | case 410 |
| 工作位驗證 | L1542 case 420 | sensor 驗證 |
| **case 500 ↑** | L1593 `SwEmptyCW->On()` 啟動輸送帶 | **超出契約範圍**（屬下游交握） |

---

## 3. case 編號對齊

| 契約 case | 本機 case | 一致 |
|-----------|----------|------|
| 1 | 1 | ? |
| 20 | （無，但有 case 1 內補強 Middle 檢查） | ?? 部分缺 |
| 50 | 50 | ? |
| 60 | 60 | ? |
| 100 | 100 | ? |
| 200 | 200 | ? |
| 300 | 300 | ? |
| 400 | 400 | ? |
| 410 | 410 | ? |
| 420 | 420 | ? |
| 500+ | 500（CCD/輸送帶交握） | ? 本機擴充 |

---

## 4. 風格特徵

- **集中檔案**：所有 stacker 實例（Loader / Empty1 / Empty2 ...）寫在同一份 `aLoader.cpp`
- **陣列命名**：`C_Up[idx]` / `Sn_CarHasTray[idx]` 用 `eLoader1/eEmpty1/eEmpty2` enum 索引
- **狀態變數**：`iLoadNewTrayToEmpty1CarTask` / `iLoadNewTrayToEmpty2CarTask` 仍個別宣告
- **函式呼叫**：包裝層 `AutoCylinderUp(reset, idx)` — **兩相呼叫**（先 reset、後 exec）
- **新增實例成本**：新增一個 enum 值 + IO_Table 補欄即可

---

## 5. 與契約的偏差

| 偏差 | 說明 | 是否合規 |
|------|------|---------|
| 缺獨立 `SEN_LIFT_DOWN` | 由 `Sn_CarHasTray` 兼任 | ? 契約 §4 允許 sensor 角色合併 |
| case 20 未獨立 | Middle 防護整合在 case 1 內 | ?? 契約建議獨立但非必須 |
| case 500+ 含輸送帶啟動 | 與下游 conveyor 交握寫在同函式 | ? 契約 §3 允許 case 500+ 視機台而定 |
| `iT09_TrayIDReadPos` 分支 | TrayID/CCD 讀取選配 | ?? 屬機台擴充功能（不在契約） |
| 兩相 `AutoCylinderXxx(reset, idx)` 包裝 | reset 與 exec 分階段 | ? 屬實作風格選擇 |

---

## 6. 同機台兄弟實例

HT172 此機構共 4 個實例（屬本契約涵蓋範圍）：

| 實例 | 函式 | 特殊點 |
|------|------|--------|
| Loader1 | `DoLoadNewTrayToLoaderCar` | 標準 |
| Empty1 | `DoLoadNewTrayToEmpty1Car` | **本份對照基準** + CCD/輸送帶交握 |
| Empty2 | `DoLoadNewTrayToEmpty2Car` | 含 Magazine 整合 |

> 其他 `aMagArm` / `aSortArm` / `aTrayArm` 等屬其他機構，不在本契約涵蓋範圍。

---

## 7. 與 HT9045 風格對比（同契約、不同實作）

| 項目 | HT9045 (`asendic_Empty.cpp`) | HT172 (`aLoader.cpp::DoEmpty1Car`) |
|------|------------------------------|------------------------------------|
| 檔案數 | 每實例一檔（3 檔） | 集中一檔（4 函式） |
| 變數命名 | `C_Empty_Up` 個別 | `C_Up[eEmpty1]` 陣列 |
| 升降呼叫 | `CylinderUp(C_Empty_Up)` 單行 | `AutoCylinderUp(true, idx)` + 完成檢查兩階段 |
| 模式 flag | 全域 bool | `HSys.Sys.RunMode` 列舉 |
| case 20 防護 | 獨立 case | 整合於 case 1 |
| `SEN_LIFT_DOWN` | 獨立 sensor `SenEmptyCCWDete` | 由 `CarHasTray` 兼任 |
| TrayID/CCD | 無 | case 420→500 含完整流程 |
| 新增實例 | 複製整檔 + 全域改名 | 加 enum + 寫一組 task 變數 |

> **此表是新機台選風格時的決策依據**，建議內嵌到契約 §9。

---

## 8. 變更歷史

| 日期 | 變更 |
|------|------|
| 2026-05-07 | 初版（與 HT9045 對照建立） |
