---
name: ht9045-outarm-flow
description: HT9045 IC Test Handler OutArm 流程知識庫。當使用者詢問 OutArm、Output Arm、Shuttle 取料、Unloader Tray 放料、Auto Tray 放料、Fix Tray 放料、Bin 分類、Magazine、吸嘴（Sucker）、真空吸取（Vacuum Suck）、Destroy 吹氣、掉料（IC Fall Down）、Pick Error、Retry/Skip/Home、AOI、Rotator、Fix AI CCD、Clean Out 整盤、BinBox 等相關問題時，應先載入此技能以理解 OutArm 完整處理流程。關鍵字：DoOutArm, DoOutArm_9045, OutArmSuck, OutArmTask, iInArmType, Shuttle, Unloader, Auto Tray, Fix Tray, Magazine, DoPickFromShuttle, DoOutArmPlaceToAuto, DoOutArmAfterPlaceToAuto。
---

# HT9045 OutArm Flow Knowledge

## 適用場景

當使用者詢問以下主題時，載入此技能：
- OutArm / Output Arm 的動作流程、狀態機、case 數值意義
- Shuttle 取料（Pick from Shuttle 1 / Shuttle 2）
- Unloader Tray 放料（Place to Auto Tray / Fix Tray）
- Bin 分類與 Tray 搜尋（SearchTrayToPlace, CheckBin）
- Magazine 相關流程（Magazine Tray / Buffer）
- 附加功能（Rotator / AOI / Fix AI CCD）
- 吸取異常處理（Retry / Skip / Home）
- IC 掉料（Fall Down）、Destroy 吹氣確認
- Clean Out 整盤（Auto Sorting BinTray）
- OutArm sucker 幾何配置（iInArmType）

## 專案資訊

- **專案**: HT9045 IC Test Handler
- **原始碼根路徑（可自定義）**: `${HT9045_SOURCE_ROOT}`
- **預設值（範例）**: `d:\HT9045\HT9011UC_Code_V3.33.897.0_20260306\`
- **語言**: C++ (Borland C++ Builder 6, VCL framework, AnsiString)
- **架構**: State Machine pattern — `switch(Task)` 搭配 `int &Task` 參考變數
- **呼叫方式**: 函式回傳 `bool`，`true` = 動作完成，`false` = 尚未完成（主迴圈反覆呼叫）
- **FlowChart 文件**: `${HT9045_SOURCE_ROOT}\..\FlowChart\`（或依你的目錄結構調整）

## 關鍵原始檔

| 檔案 | 主要函式 | Task 變數 |
|---|---|---|
| `aoutarm.cpp` | `DoOutArm()` — 最上層入口，資料前處理後呼叫 `DoOutArm_9045()` | （無 switch） |
| `aoutarm.cpp` | `CheckOutArmCleanOut()` — Clean Out 判斷（整盤 / Magazine / ATK AMR） | （無 switch） |
| `aoutarm.cpp` | `DoOutArmPlaceToAuto()` — 放料到 Auto/Fix Tray 的 Destroy 吹氣主流程 | （迴圈遍歷，無 switch） |
| `aoutarm.cpp` | `CheckBin()` — 依據 BIN 分配 IC 到對應 Tray | （無 switch） |
| `aoutarm.cpp` | `CheckOutSuckICFallDown()` — 掉料檢查 | （無 switch） |
| `aoutarm9045.cpp` | `DoOutArm_9045()` — Dispatch，依 `iInArmType` 分派到對應 sub-function | （Dispatch，無 switch） |
| `aoutarm9045.cpp` | `DoPickFromShuttle_9045()` — Shuttle 取料 Dispatch | （Dispatch，無 switch） |
| `aoutarm9045.cpp` | `DoOutArmPlaceToAuto_9045()` — 放料到 Auto Tray 主控制流程 | `iPlaceToAutoTask` |
| `aoutarm9045.cpp` | `DoOutArmAfterPlaceToAuto()` — 放料後的後處理（整盤 / Magazine / CleanOut） | `iDoOutArmAfterPlaceToAutoTask` |
| `aoutarm9045.cpp` | `DoOutArmAdditionalFunction()` — 附加功能（Rotator / AOI / Fix AI CCD） | `iOutArmAdditionalFunctionTask` |
| `aoutarm9045.cpp` | `SearchTrayToPlace_9045()` — 搜尋可放 IC 的 Tray | （無 switch） |
| `aoutarm9045_2x8_8.cpp` | `DoOutArm_9045_2x8_8()` — 2x8 模式主狀態機 | `OutArmTask` |
| `aoutarm9045_2x8_8.cpp` | `DoPickFromShuttle_9045_2x8_8()` — 2x8 模式 Shuttle 取料 | `iPickFromShuttle1Task` / `iPickFromShuttle2Task` |

> 其他 `aoutarm9045_*.cpp` 檔案（如 `_1x4_4.cpp`, `_2x2_4.cpp` 等）的狀態機結構與 `_2x8_8` 相同，僅 sucker 幾何處理不同。

---

## 1. 呼叫階層總覽

```
DoOutArm()                                 <- aoutarm.cpp (最上層入口)
  └─ DoOutArm_9045()                       <- aoutarm9045.cpp (Dispatch)
       └─ DoOutArm_9045_2x8_8()            <- aoutarm9045_2x8_8.cpp (主狀態機, 以 2x8 為例)
            ├─ case 1200/2200: DoPickFromShuttle_9045_2x8_8(iSht)
            │                    ├─ case 200: MoveOutArmToShuttleIncludeZ (XY/Z 定位)
            │                    ├─ case iOUTARM_SUCK: 逐一 Suck() 吸取 IC
            │                    └─ case 2000: Error Retry/Skip/Home
            ├─ case 7000: DoOutArmAdditionalFunction()
            │               ├─ DoOutArmRotateKIT()
            │               ├─ DoAOIFunction()
            │               └─ DoFix2AICCDFunction()
            ├─ case 3010: SearchTrayToPlace_9045() (搜尋放料 Tray)
            ├─ case 3310: DoOutArmPlaceToAuto_9045()
            │               └─ DoOutArmPlaceToAuto(iWhichAuto) (Destroy 吹氣放料)
            └─ case 3500: DoOutArmAfterPlaceToAuto()
                            ├─ case 1000: DoFix3FullTray() (Fix3 整盤)
                            ├─ case 5000: DoSortingBinTray() (Clean Out 整盤)
                            └─ case 11100: DoPickFromMagazineBuffer()
```

---

## 2. DoOutArm() — 入口

> Source: `aoutarm.cpp` line 1117-1135

入口函式非常簡潔，主要做資料前處理：

1. `FRCarryKit.SetHasNullIcToNullIc()` / `BRCarryKit.SetHasNullIcToNullIc()` — 清理 Shuttle 上的 NULL IC 標記
2. 將 Shuttle 上的 `HAS_IC` / `HAS_HOT_IC` 統一轉換為 `TEST_PASS + iTestBinCount`（模擬未分類的測試結果）
3. `SetFixTrayMiddleDtata()` — 設定 Fix Tray 中間資料
4. 呼叫 `DoOutArm_9045()` — 進入 Dispatch

> 與 InArm 不同：DoOutArm() 沒有複雜的 Guard Checks，Guard 邏輯全在 DoOutArm_9045() 中。

---

## 3. DoOutArm_9045() — Dispatch

> Source: `aoutarm9045.cpp` line ~403-620

### Pre-Dispatch Guard Checks

1. `TestingNeedStopAllMotor` → return
2. `bAlarmNeedServoOff && bMyServoOffOutArm` → return
3. `CheckOutArmDestroyActive()` → 確認 Destroy 吹氣完成，若未完成 → return
4. `QA Mode` — `Check_QA_ModeUnloadCount()`
5. `iPauseBackUp != -1 && Suck/Destroy 完成` → return
6. `bResetOutArmTask` → `InitOutArmTask()` → return
7. **Auto Alignment CCD** — 複雜的 Tray 補料確認與自動對位流程
   - `bAutoNeedTrayMustFinish` — 需要等 Tray 補好
   - `bRunOutArmAutoAlignment` — 正在對位中 → return

### iInArmType Dispatch Table

| iInArmType | Sub-function | Picker Config |
|---|---|---|
| `ep1Picker` | `DoOutArm_9045_All_1Picker()` | Single picker |
| `e9045_1x1_1` / `e9045_1x4_1_Ac` | `DoOutArm_9045_1x1_1()` | 1x1, 1 sucker |
| `e9045_1x2_2_14` / `e9045_1x2_2_13` | `DoOutArm_9045_1x2_2()` | 1x2, 2 suckers |
| `e9045_1x2_4_Hot` | `DoOutArm_9045_1x2_4()` | 1x2, 4 suckers |
| `e9045_1x3_2_14` | `DoOutArm_9045_1x3_2_14()` | 1x3, 2 suckers |
| `e9045_1x3_4` | `DoOutArm_9045_1x3_4()` | 1x3, 4 suckers |
| `e9045_1x4_2_14` | `DoOutArm_9045_1x4_2()` | 1x4, 2 suckers |
| `e9045_1x4_4_13` | `DoOutArm_9045_1x4_4S()` | 1x4, 4 suckers (S) |
| `e9045_1x4_4` | `DoOutArm_9045_1x4_4()` | 1x4, 4 suckers |
| `e9045_1x4_8_Hot` | `DoOutArm_9045_1x4_8()` | 1x4, 8 suckers |
| `e9045_2x1_2_13` | `DoOutArm_9045_2x1_2()` | 2x1, 2 suckers |
| `e9045_2x2_4_12/13/14` | `DoOutArm_9045_2x2_4()` | 2x2, 4 suckers |
| `e9045_2x2_8_Hot` | `DoOutArm_9045_2x2_8()` | 2x2, 8 suckers |
| `e9045_2x3_6_14` | `DoOutArm_9045_2x3_6_14()` | 2x3, 6 suckers |
| `e9045_2x3_6` | `DoOutArm_9045_2x3_6()` | 2x3, 6 suckers |
| `e9045_2x4_4_13/14` | `DoOutArm_9045_2x4_4()` | 2x4, 4 suckers |
| `e9045_2x4_8` | `DoOutArm_9045_2x4_8()` | 2x4, 8 suckers |
| `e9045_2x5_8` | `DoOutArm_9045_2x5_8()` | 2x5, 8 suckers |
| `e9045_2x6_8` | `DoOutArm_9045_2x6_8()` | 2x6, 8 suckers |
| `e9045_2x8_8` / `e9045_2x8_32` | `DoOutArm_9045_2x8_8()` | 2x8, 8/32 suckers |

> Dispatch 結構與 InArm 完全對稱，使用相同的 `iInArmType` 作為分派依據。

---

## 4. 主狀態機 — DoOutArm_9045_2x8_8() (代表所有 XxY_Z 版本)

> Source: `aoutarm9045_2x8_8.cpp` line ~1766  
> Task variable: `int &Task = OutArmTask`

### Pre-Switch Guard

- `bCarryControlOutarm1 || bCarryControlOutarm2` → return（Carry 控制中）

### 主流程

```
case 1   → case 5    → case 10   → case 50
(Reset)    (Z Safe)    (Init      (Z Safe +
                        Check)     Choose Shuttle)
                                       |
                    ┌──────────────────┘
                    ↓
              case 100 (Decide Shuttle)
              ┌─ OldPos==1 ──→ case 1000 (Shuttle 1 Flow)
              └─ OldPos==0 ──→ case 2000 (Shuttle 2 Flow)
```

### Shuttle 1 Pick Flow (case 1000 ~ 1235)

```
case 1000 → case 1010~1030 (Y Pitch Home, optional)
          → case 1140 (Check: CleanOut / Has IC / Shuttle Ready)
          → case 1150 (Move XY to Shuttle 1, Sub Speed)
          → case 1100 (Wait Shuttle Ready, Check Tray Need)
          → case 1200 (DoPickFromShuttle_9045_2x8_8(0))
               ├─ Still has IC on same side → loop 1200
               ├─ STM/TW153 mode → switch kit side → loop 1200
               ├─ No IC picked → case 50
               └─ Pick done → case 3000
```

### Shuttle 2 Pick Flow (case 2000 ~ 2235)

```
case 2000 → case 2010~2030 (Y Pitch Home, optional)
          → case 2040 (Check: CleanOut / Has IC / Shuttle Ready)
          → case 2050 (Move XY to Shuttle 2, Sub Speed)
          → case 2100 (Wait Shuttle Ready, Check Tray Need)
          → case 2200 (DoPickFromShuttle_9045_2x8_8(1))
               ├─ Still has IC on same side → loop 2200
               ├─ STM/TW153 mode → switch kit side → loop 2200
               ├─ No IC picked → case 50
               └─ Pick done → case 3000
```

### Place to Tray Flow (case 3000 ~ 3500)

```
case 3000 (Z Safe + IC Fall Down Check + Release Shuttle)
     ├─ Need Additional Function → case 7000
     └─ No Additional → case 3010

case 3010 (SearchTrayToPlace_9045 + VerifyFixTrayLink)
     ├─ Fix Tray no tray → case 4000
     ├─ Magazine busy → wait
     └─ Ready → case 3100

case 3100 → case 3300 (Verify Fix Tray Sensor)
          → case 3305 (Auto Alignment mode, optional)
          → case 3310: DoOutArmPlaceToAuto_9045()
               Done → DoOutArmAfterPlaceToAuto(init)
               → case 3500

case 3500: DoOutArmAfterPlaceToAuto()
     ├─ return 3010 → 吸嘴還有 IC, 繼續放
     ├─ return 100  → 全部放完, 回去拿下一批
     ├─ return 11100 → Magazine Buffer 處理
     └─ return 5000 → Clean Out 整盤
```

### Special Cases

| case | Name | Description |
|---|---|---|
| **200** | Yield Position | Z Safe 後移到讓位位置（等待 Shuttle 就位） |
| **300** | Yield Move | 32Site: 移到 Shuttle 2 上等待; 否則移到 Fix Tray Full 讓位點 |
| **310/311** | Auto Z Teach | OutArm Z 軸自動校正高度流程 |
| **4000** | Fix Tray Missing | 移到讓位位置 |
| **4100** | Fix Tray Full Alarm | `DoFixTrayFullAlarm()` — Fix Tray 滿盤處理 |
| **7000** | Additional Function | `DoOutArmAdditionalFunction()` — Rotator / AOI / Fix AI CCD |
| **10000** | Magazine Tray | `CheckPlaceToMagazineTray()` |
| **11100** | Magazine Buffer Pick | `DoPickFromMagazineBuffer()` |
| **12000** | Magazine Place | 等待 Magazine 就位 → case 3100 繼續放料 |

### State Descriptions

| case | Name | Description |
|---|---|---|
| **1** | Reset | 重置 SortingAllBinTrayFinish 旗標 |
| **5** | Z Safe | `MoveOutArmToAutoSafe()` |
| **10** | Init Check | `CheckOutArmInitState()` |
| **50** | Shuttle Decision | Z Safe → 釋放 Shuttle 可移動 → 判斷有 IC 否: 有 → 3000, 無 → 100 |
| **100** | Shuttle Select | 決定先取 Shuttle 1 還是 2（32Site: SHT2 優先; 一般: SHT1 優先）|
| **1000-1030** | SHT1 Y Pitch Home | Y Pitch 馬達歸位（若啟用 Auto Pitch） |
| **1140** | SHT1 Pre-Check | CleanOut 檢查、確認 FRCarryKit 有 IC、Shuttle 1 就位 |
| **1150** | SHT1 Move XY | 移到 Shuttle 1 上方，降速 |
| **1100** | SHT1 Wait Ready | 等 Shuttle 就位 / Tray 換盤讓路 |
| **1200** | SHT1 Pick IC | `DoPickFromShuttle_9045_2x8_8(0)` — 吸取 Shuttle 1 上的 IC |
| **2000-2030** | SHT2 Y Pitch Home | Y Pitch 馬達歸位（若啟用 Auto Pitch） |
| **2040** | SHT2 Pre-Check | CleanOut 檢查、確認 BRCarryKit 有 IC、Shuttle 2 就位 |
| **2050** | SHT2 Move XY | 移到 Shuttle 2 上方，降速 |
| **2100** | SHT2 Wait Ready | 等 Shuttle 就位 / Tray 換盤讓路 |
| **2200** | SHT2 Pick IC | `DoPickFromShuttle_9045_2x8_8(1)` — 吸取 Shuttle 2 上的 IC |
| **3000** | Post-Pick | Z Safe → IC Fall Down 檢查 → 釋放 Shuttle → Additional Function? |
| **3010** | Search Tray | `SearchTrayToPlace_9045()` → `VerifyFixTrayLink()` |
| **3100** | Verify Tray | Tray 狀態驗證 |
| **3300** | Fix Sensor Check | Fix Tray Sensor 偵測 |
| **3310** | Place to Auto | `DoOutArmPlaceToAuto_9045()` — 放料到 Tray |
| **3500** | After Place | `DoOutArmAfterPlaceToAuto()` — 後處理 |

---

## 5. DoPickFromShuttle_9045_2x8_8() — Shuttle 取料

> Source: `aoutarm9045_2x8_8.cpp` line ~1333  
> Task: `iPickFromShuttle1Task` (SHT1) / `iPickFromShuttle2Task` (SHT2)

### 流程摘要

從 Shuttle 吸取 IC：等待（optional）→ XY/Z 定位 → Offset Check → 逐一 Suck() → 資料交換 → Error Retry/Skip/Home

### 關鍵 Case

| case | 動作 |
|---|---|
| 1 | 若設定 `dWaitOnSH != 0` 且為首次吸取 → 移到 Shuttle 上等待 → case 2 |
| 2 | 等待時間到 (`bOutShtwaitPick`) → case 10 |
| 10 | 初始化 → case 200 |
| 200 | `MoveOutArmToShuttleIncludeZ_9045_2x8_8()` 移動 XY/Z → Offset check → case iOUTARM_SUCK |
| 500 | Offset 完成 / AutoTeach 處理 → case iOUTARM_SUCK |
| iOUTARM_SUCK | 逐一吸取所有 Sucker：`Suck()` → `SwapShuttleDataToOutArm()` 資料交換 → 全部完成且無 Error: return true |
| 2000 | Error 處理：Z Safe → Retry (若未超限) 或 `OutArmPickShuttleAlarm()` alarm → K_SKIP / K_HOME / K_RETRY |
| 2200 | Home: `SetOutArmHome()` → 重置 |

### 吸取邏輯（case iOUTARM_SUCK）

1. 遍歷所有 `OutArmSuck.Suck[i][j]`
2. 對每個需要吸取的 sucker，呼叫 `Suck()` 進行真空吸取
3. 吸取成功 → `SwapShuttleDataToOutArm()` 將 Shuttle 上的 IC 資料搬到 OutArm
4. 如有任一 `Error` → 進入 case 2000 Retry 流程
5. 全部完成無錯誤 → `iOutShtRetryCount = 0` → return true

### Error 處理（case 2000）

- `iOutShtRetryCount <= RetryCT` → 自動 Retry（回 case 1）
- `iOutShtRetryCount > RetryCT` → 顯示 Alarm：
  - `K_SKIP` → `PorcessJAM0201OutArmPickUpErrorSkip()` → 跳過 Error IC
  - `K_HOME` → case 2200 → `SetOutArmHome()`
  - `K_RETRY` → 回 case 1 / case 2200 重試

---

## 6. DoOutArmAdditionalFunction() — 附加功能

> Source: `aoutarm9045.cpp` line ~2208  
> Task: `iOutArmAdditionalFunctionTask`

### 流程摘要

依序檢查並執行三種附加功能，每個完成後回到 case 100 檢查下一個：

```
case 1 → case 100 (check flags, Z Safe, IC Fall Down Check)
           ├─ Rotator   → case 10000: DoOutArmRotateKIT()
           ├─ AOI       → case 20000: DoAOIFunction()
           ├─ Fix AI CCD→ case 30000: DoFix2AICCDFunction()
           └─ All done  → return true
```

### 觸發條件（CheekNeedToDoOutArmAdditionalFunction）

| 功能 | 條件 |
|---|---|
| **Rotator** | `USE_ROTATE_KIT==1 && tRotate.ActiveRotate && iOutRotateFinish==0/1` |
| **AOI** | `tAOISetup.bEnabledAOI` 或 Scanner AOI / Top Scanner AOI 啟用 |
| **Fix AI CCD** | `USE_Fix_AI_CCD && TestIF_File.bEnableFix2BGAAICCD && fFixAICCD->NeedToGrabImage()` |

> 與 InArm 的 DoInArmAdditionalFunction() 不同：OutArm 有 AOI 和 Fix AI CCD，而 InArm 有 Die Clean / Precisor / Bottom 2DID / Rotator。

---

## 7. DoOutArmPlaceToAuto_9045() — 放料到 Tray 控制

> Source: `aoutarm9045.cpp` line ~2935  
> Task: `iPlaceToAutoTask`

### 流程摘要

搜尋 Tray → IC Fall Down 檢查 → Fix3 Cylinder → 設定放料位置 → Offset Check → Destroy 延遲 → 放料 → 偵測 Tray

### 關鍵 Case

| case | 動作 |
|---|---|
| 1 | 初始化、Flag Reset → case 10 |
| 10 | IC Fall Down 檢查 → `SearchTrayToPlace_9045()` → `IfUseOnebyOne()` → Tray 有空? → Magazine? → `SetOutArm_9045()` 計算放料位置 → case 50 |
| 30 | 等待 Tray 有空位（fHasTray && !FullIC） |
| 50 | `CheckOutSuckICFallDown()` 全時掉料檢查 → case 100 |
| 100 | `OutArmNeedCheckOffset()` Offset 校正 → Destroy 延遲 → case 300 |
| 110 | Destroy 暫停延遲計時 → case 300 |
| 200 | Offset 完成 → case 300；Offset 要回 → case 220 |
| 220 | Z Safe → 回 case 1 重新搜尋 |
| 300 | `DoOutArmPlaceToAuto(iWhichAuto)` — 實際 Destroy 吹氣放料 → case 400 |
| 400 | Rotate 處理、Auto Tray Detect → case 500 |
| 500 | `DetectAutoTray()` 偵測 Tray → return true |

---

## 8. DoOutArmPlaceToAuto() — 實際放料 Destroy

> Source: `aoutarm.cpp` line ~2483  
> 此函式不使用 switch/Task，而是以迴圈遍歷所有 Sucker

### 流程摘要

遍歷所有 `bOutArmSuckActive[i][j]` 為 true 的吸嘴：

1. **NULL IC 處理** — `HAS_NULL_IC` 直接標記為 `NULL_IC`
2. **Destroy 吹氣** — `OutArmSuck.Suck[i][j].Destroy()` 放料
3. **資料記錄** — 將測試結果寫入 Tray：
   - `MOT[Motor].Tray.iBinCode` → BIN 代碼
   - `MOT[Motor].Tray.iWhichSite` → 測試 Site
   - `LotSummary.AddCount()` → 累計 Lot 數量
   - `PordRec.AddUnloadRecord()` → Production Log
   - `PordRec.SaveRecord()` → 存檔
4. **Barcode/2DID 記錄** — 若啟用
5. **Bin 計數** — `iByBinTotal[]`, `LastSet.BinCT[]`, `LastSet.iBinData32[]` 累計
6. **ART 計數** — SCK ART / HANA ART 相關計數
7. **Error 處理** — `JAM0217` (Vacuum sensor OFF error)
8. **Tray 滿盤檢查** — BinBox / TraySortCntFunc
9. **Yield 檢查** — `fSortCT->CheckTheYieldAfterPlaceAuto()`

---

## 9. DoOutArmAfterPlaceToAuto() — 放料後處理

> Source: `aoutarm9045.cpp` line ~3139  
> Task: `iDoOutArmAfterPlaceToAutoTask`  
> 回傳值: int（跳轉目標 case，0 = 尚未完成）

### 流程摘要

```
case 1 (判斷吸嘴是否還有 IC)
  ├─ 還有 IC:
  │   ├─ Fix3 Cylinder → case 500 → return 3010 (繼續放)
  │   └─ return 3010 (直接繼續放)
  └─ 沒有 IC:
      ├─ ATK AMR Fix Tray → return 11100
      ├─ Magazine Buffer 需清 → return 11100
      ├─ Fix3 Full Tray → case 1000: DoFix3FullTray()
      └─ 正常 → case 2000

case 500: Fix3 Cylinder 縮回 → return 3010

case 1000: DoFix3FullTray() 整盤 → case 2000

case 2000: Fix3 Cylinder 確認 → case 3000

case 3000: CheckOutArmCleanOut() → Auto Speed 調整 → case 3100

case 3100: return 100 (回主狀態機 case 100)
           若 Rotate 啟用: return 110

case 5000: Clean Out 整盤
  ├─ P27 Auto Sorting → case 5100: DoSortingBinTray()
  └─ MoveOutArmXY_ToFix_Tray_Full() → 等待其他 Arm 清完
```

### 回傳值對照

| 回傳值 | 意義 | 主狀態機跳轉 |
|---|---|---|
| 0 | 尚未完成 | 維持 case 3500 |
| 1 | CleanOut 完成/無動作 | 回 case 1 |
| 100 | 正常完成 | 回 case 100 |
| 110 | 完成 + Rotate | 回 case 100 + Rotate offset |
| 3010 | 吸嘴還有 IC | 回 case 3010 繼續放 |
| 5000 | Clean Out + 整盤 | case 5000 |
| 11100 | Magazine Buffer 處理 | case 11100 |

---

## 10. CheckOutArmCleanOut() — Clean Out 判斷

> Source: `aoutarm.cpp` line ~215

### 流程摘要

在主狀態機 case 300（讓位等待）和 DoOutArmAfterPlaceToAuto case 3000 中呼叫。

判斷邏輯：

1. **P27 整盤模式** — `bP27AutoSortingBinTrayByOutArmwhenCleanOut` 啟用且所有 Arm/Shuttle/Index 都沒有 IC → return 5000（進入整盤）
2. **Magazine Buffer 模式** — `AUTO3_IS_MAGAZINE && iMagFixTrayType==1` 且無 IC → return 11100
3. **ATK AMR 模式** — `fAGV->IsATK_AMR()` 且有 Buffer IC → return 11100
4. **Auto Z Teach** — `bOutarmAutoHigh` → return 310
5. 以上都不符合 → return 原 Task（不改變流程）

---

## 11. OutArm vs InArm 差異對照

| 項目 | InArm | OutArm |
|---|---|---|
| **功能** | Loader Tray → (HotPlate) → Shuttle | Shuttle → Unloader Tray |
| **取料來源** | Loader Tray / HotPlate | Shuttle 1 / Shuttle 2 |
| **放料目標** | HotPlate / Shuttle | Auto Tray / Fix Tray / Magazine |
| **附加功能** | Die Clean / Precisor / 2DID / Rotator | Rotator / AOI / Fix AI CCD |
| **HotPlate 相關** | 有（Place/Pick from HP） | 無 |
| **Bin 分類** | 無 | 有（CheckBin / SearchTrayToPlace） |
| **整盤功能** | 無 | 有（Fix3 Full Tray / P27 Sorting BinTray） |
| **Magazine** | 無 | 有（Magazine Tray / Buffer 取放） |
| **Production Log** | 無 | 有（AddUnloadRecord / SaveRecord） |
| **Lot Summary** | 無 | 有（LotSummary.AddCount / iBinData32） |
| **Task 變數** | `iArmTask` | `OutArmTask` |
| **入口 Guard** | 複雜（DoInArm 中有多重檢查） | 簡潔（DoOutArm 無 Guard，Guard 在 DoOutArm_9045） |

---

## 12. FAQ / 常見問題索引

| 問題 | 參考章節 |
|---|---|
| OutArm 的主流程是什麼？ | §4 主狀態機 |
| Shuttle 1 跟 Shuttle 2 怎麼選擇？ | §4 case 100（OldPos / 32Site 優先級） |
| OutArm 怎麼從 Shuttle 吸 IC？ | §5 DoPickFromShuttle |
| 吸取失敗怎麼處理？ | §5 case 2000 Error 處理 |
| IC 放到哪個 Tray？ | §7 case 10 SearchTrayToPlace_9045 / §8 DoOutArmPlaceToAuto |
| 放料時的 Destroy 吹氣如何工作？ | §8 DoOutArmPlaceToAuto (Destroy loop) |
| OutArm 有哪些附加功能？ | §6 Rotator / AOI / Fix AI CCD |
| Clean Out 時的整盤功能？ | §9 case 5000 / §10 CheckOutArmCleanOut |
| Magazine 怎麼運作？ | §4 case 10000-12000 / §9 case 11100 |
| DoOutArmAfterPlaceToAuto 回傳值代表什麼？ | §9 回傳值對照表 |
| OutArm 和 InArm 有什麼區別？ | §11 差異對照表 |
| STM/TW153 交錯模式怎麼影響 OutArm？ | §4 case 1200/2200 內的 `iCloseSiteModeFor2x8` 判斷 |