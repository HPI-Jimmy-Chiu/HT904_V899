---
name: ht9045-catchtray-flow
description: HT9045 IC Test Handler CatchTray（Tray Arm）流程知識庫。當使用者詢問 DoCatchTray、DoCatchFromLoader、CatchNewTrayFromBuffer、DoPlaceTrayToAuto、DoPlaceToBuffer、DoSlapTray、DoCatchUnderTray、DoSupportUnderTray、DoPlaceBufferTray、Loader/Empty/Color 補盤、Auto1/2/3 放盤、Tray Mapping、CatchTray Fix 氣缸、RFID/TrayID 相關問題時，應先載入此技能以理解 CatchTray 完整狀態機流程。關鍵字：DoCatchTray, CatchTrayTask, iCatchFromLoaderTask, iCatchNewTrayFromBufferTask, iPlaceTrayToAutoTask, iPlaceToBufferTask, iSlapTrayTask, DoCatchUnderTray, DoSupportUnderTray, DoPlaceBufferTray, C_CatchTray_Fix_Puch, C_CatchTray_Fix_Pop。
---

# HT9045 CatchTray Flow Knowledge

## 適用場景

當使用者詢問以下主題時，載入此技能：
- CatchTray / TrayArm 的動作流程、狀態機、case 數值意義
- Loader 吸空 Tray 流程（DoCatchFromLoader）
- Empty/Color/Auto2 補盤流程（CatchNewTrayFromBuffer）
- Auto1/Auto2/Auto3 放盤流程（DoPlaceTrayToAuto）
- Buffer 放盤流程（DoPlaceToBuffer / DoPlaceBufferTray）
- 下方 Conveyor 模式（DoCatchUnderTray / DoSupportUnderTray）
- 拍盤（DoSlapTray）與 CatchTray Fix 氣缸 push/pop
- Tray Mapping / RFID 讀取相關流程

## 專案資訊

- **專案**: HT9045 IC Test Handler
- **原始碼根路徑（可自定義）**: `${HT9045_SOURCE_ROOT}`
- **預設值（範例）**: `d:\HT9045\HT9011UC_Code_V3.33.897.0_20260306\`
- **語言**: C++ (Borland C++ Builder 6, VCL framework, AnsiString)
- **架構**: State Machine pattern — `switch(Task)` 搭配 `int &Task` 參考變數
- **FlowChart 文件**: `${HT9045_SOURCE_ROOT}\..\FlowChart\DoCatchTray_ProcessFlow.md`

## 關鍵原始檔

| 檔案 | 主要函式 | Task 變數 |
|---|---|---|
| `acatchtray.cpp` | `DoCatchTray()` — CatchTray 主狀態機 | `CatchTrayTask` |
| `acatchtray.cpp` | `DoCatchFromLoader()` — 從 Loader 吸空 Tray | `iCatchFromLoaderTask` |
| `acatchtray.cpp` | `CatchNewTrayFromBuffer()` — 從 Empty/Color/Auto2 補盤 | `iCatchNewTrayFromBufferTask` |
| `acatchtray.cpp` | `DoPlaceTrayToAuto()` — 放 Tray 到 Auto | `iPlaceTrayToAutoTask` |
| `acatchtray.cpp` | `DoPlaceToBuffer()` — 放 Tray 到 Buffer | `iPlaceToBufferTask` |
| `acatchtray.cpp` | `DoSlapTray()` — 拍盤流程 | `iSlapTrayTask` |
| `acatchtray.cpp` | `DoCatchUnderTray()` — 下方夾盤流程 | `iCatchUnderTrayTask` |
| `acatchtray.cpp` | `DoSupportUnderTray()` — 下方補盤流程 | `iSupportUnderTrayTask` |
| `acatchtray.cpp` | `DoPlaceBufferTray()` — Buffer 放盤流程 | `iPlaceBufferTrayTask` |
| `acatchtray.cpp` | `C_CatchTray_Fix_Puch()` — Fix Push | `iCatchTray_Fix_Puch` |
| `acatchtray.cpp` | `C_CatchTray_Fix_Pop()` — Fix Pop | `iCatchTray_Fix_Pop` |
| `acatchtray.cpp` | `DoLoadCarRotArmReadRFID()` — LoaderCar RFID 讀取 | `iCoverTrayIDTask[iKeyenceCoverTrayID_LoaderCar]` |

---

## 1. 呼叫階層總覽

```text
DoCatchTray()                               [acatchtray.cpp]
  |- DoCatchFromLoader()                    [switch Task]
  |   |- C_CatchTray_Fix_Pop()              [switch iTask]
  |   `- C_CatchTray_Fix_Puch()             [switch iTask]
  |
  |- DoCatchUnderTray()                     [switch Task]
  |- CatchNewTrayFromBuffer()               [switch Task]
  |- DoPlaceTrayToAuto()                    [switch Task]
  |   `- DoSupportUnderTray()               [switch Task]
  |
  |- DoPlaceToBuffer()                      [switch Task]
  |   `- DoPlaceBufferTray()                [switch Task]
  |
  |- DoSlapTray()                           [switch Task]
  `- DoLoadCarRotArmReadRFID()              [switch Task]
```

---

## 2. DoCatchTray() 主流程

> Task variable: `int &Task = CatchTrayTask`

### Case 清單

`1, 10, 15, 20, 30, 40, 50, 100, 140, 145, 147, 150, 160, 170, 200, 250, 260, 300, 350, 400, 500, 600, 1000, 1100, 1150, 1200, 2000, 2100, 2120, 2130, 2140, 2160, 3000, 3010, 3100, 4000, 4100, 4200, 5000, 5100, 5150, 5200, 6000, 6010, 6030, 6100`

### 主流程段落

1. 初始化與安全檢查：`1~50`
2. Loader 吸空 Tray：`100~260`
3. 判斷哪個 Auto 需要 Tray：`300~400`
4. 從 Buffer 補新 Tray：`500/600`（呼叫 `CatchNewTrayFromBuffer`）
5. 放 Tray 到 Auto：`1000~1200`（呼叫 `DoPlaceTrayToAuto`）
6. 放 Tray 到 Buffer：`2000~2160`（呼叫 `DoPlaceToBuffer`）
7. 手動/映射/收尾：`3000~6100`

---

## 3. 下一層函式 Case 摘要

### 3.1 DoCatchFromLoader
- Task: `iCatchFromLoaderTask`
- Cases: `1, 10, 50, 90, 95, 100, 110, 120, 145, 150, 155, 156, 160, 161, 162, 163, 165, 170, 230, 240, 250, 300, 301, 305, 310, 400, 410, 500, 501, 502, 503, 550, 560, 570, 600`

### 3.2 CatchNewTrayFromBuffer
- Task: `iCatchNewTrayFromBufferTask`
- Cases: `1, 2100, 2101, 2102, 2103, 2149, 2150, 2160, 2165, 2180, 2185, 2190, 2191, 2199, 2200, 2250, 2260, 2300, 2310, 2340, 2350, 2351, 2352, 2353, 2355, 2360, 2400, 2450, 2500, 2550, 2600, 2700, 4000`

### 3.3 DoPlaceTrayToAuto
- Task: `iPlaceTrayToAutoTask`
- Cases: `1, 100, 150, 200, 210, 220, 240, 250, 260, 300, 400, 401, 500, 510, 520, 530, 540, 600, 900, 1000, 1030, 1040, 1050, 1060, 1070, 1100, 1130, 1140, 1150, 1160, 1200, 1300`

### 3.4 DoPlaceToBuffer
- Task: `iPlaceToBufferTask`
- Cases: `1, 2100, 2150, 2160, 2250, 2251, 2260, 2270, 2280, 2300, 2350, 2351, 2352, 2500, 2600, 2900, 3000`

### 3.5 DoSlapTray
- Task: `iSlapTrayTask`
- Cases: `1, 5, 10, 20, 50, 100, 200, 300, 400, 450, 500`

### 3.6 DoCatchUnderTray
- Task: `iCatchUnderTrayTask`
- Cases: `1, 50, 100, 200, 300, 400, 500, 600, 700`

### 3.7 DoSupportUnderTray
- Task: `iSupportUnderTrayTask`
- Cases: `1, 50, 100, 110, 200, 300, 400, 600`

### 3.8 DoPlaceBufferTray
- Task: `iPlaceBufferTrayTask`
- Cases: `1, 50, 100, 200, 300, 400, 600, 1000`

---

## 4. 子函式 switch（下一層內）

### 4.1 C_CatchTray_Fix_Puch
- Task: `iCatchTray_Fix_Puch`
- Cases: `1, 100`

### 4.2 C_CatchTray_Fix_Pop
- Task: `iCatchTray_Fix_Pop`
- Cases: `1, 100`

### 4.3 DoLoadCarRotArmReadRFID
- Task: `iCoverTrayIDTask[iKeyenceCoverTrayID_LoaderCar]`
- Cases: `1, 2, 3, 100, 500, 1000, 2000, 2500, 3000, 3100, 4000, 4100, 5000, 6000`

---

## 使用指引

回答 CatchTray 相關問題時：

1. 先讀 `CatchTrayTask`，定位主流程段落（吸盤、補盤、放盤、收尾）。
2. 再看對應子流程 Task（`iCatchFromLoaderTask`、`iCatchNewTrayFromBufferTask`、`iPlaceTrayToAutoTask`）。
3. 若涉及夾爪異常，優先檢查 `C_CatchTray_Fix_Puch/Pop` case 與 sensor 狀態。
4. 若涉及 RFID/TrayID，再切到 `DoLoadCarRotArmReadRFID()`。
