---
name: ht9045-inarm-flow
description: HT9045 IC Test Handler InArm 流程知識庫。當使用者詢問 InArm、Input Arm、Loader 取料、HotPlate 放料/取料、Shuttle 放料、吸嘴（Sucker）、真空吸取（Vacuum Suck）、Destroy 吹氣、掉料（IC Fall Down）、黏貨（Sticky）、Floating 浮起、Auto Clean、Tray End、Clean Out、Pick Error、Retry/Skip/Home、Auto Site Mapping、Precisor、Rotator、Die Clean、Bottom 2DID 等相關問題時，應先載入此技能以理解 InArm 完整處理流程。關鍵字：DoInArm, DoInArm_9045, InArmSuck, iArmTask, iInArmType, Shuttle, HotPlate, Loader, AutoClean。
---

# HT9045 InArm Flow Knowledge

## 適用場景

當使用者詢問以下主題時，載入此技能：
- InArm / Input Arm 的動作流程、狀態機、case 數值意義
- Loader Tray 吸取（Pick from Load Stage）
- HotPlate 放料（Place to HotPlate）/ 取料（Pick from HotPlate）
- Shuttle 放料（Place to Shuttle）
- 附加功能（Precisor / Rotator / Die Clean / Bottom 2DID）
- 吸取異常處理（Retry / Skip / Home / Tray End / Clean Out）
- IC 掉料（Fall Down）、黏貨（Sticky）、浮起（Floating）檢查
- Auto Site Mapping 相關流程
- InArm sucker 幾何配置（iInArmType）

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
| `ainarm2.cpp` | `DoInArm()` — 最上層入口，Guard checks 後呼叫 `DoInArm_9045()` | （無 switch） |
| `ainarm9045.cpp` | `DoInArm_9045()` — Dispatch，依 `iInArmType` 分派到對應 sub-function | （Dispatch，無 switch） |
| `ainarm9045.cpp` | `DoInArmPickFromLoadStage_9045()` — 從 Loader Tray 吸取 IC | `iPickFromLoadStageTask` |
| `ainarm9045.cpp` | `DoInArmAdditionalFunction()` — 附加功能（Die Clean / Precisor / 2DID / Rotator） | `iInArmAdditionalFunctionTask` |
| `ainarm9045_2x8_8.cpp` | `DoInArm_9045_2x8_8()` — 2x8 模式主狀態機 | `iArmTask` |
| `ainarm9045_2x8_8.cpp` | `DoPlaceToHotPlate_9045_2x8_8()` — 放料到 HotPlate | `iInArmPlaceToHotPlateTask` |
| `ainarm9045_2x8_8.cpp` | `DoInArmPlaceToShuttle_9045_2x8_8()` — 放料到 Shuttle | `iInArmPlaceToShuttleTask` |
| `ainarm_SearchPickPlate.cpp` | `DoInArmPickFromHotPlate_9045()` — 從 HotPlate 吸取 IC | `iInArmPickFromHotPlateTask` |

> 其他 `ainarm9045_*.cpp` 檔案（如 `_1x4_4.cpp`, `_2x2_4.cpp` 等）的狀態機結構與 `_2x8_8` 相同，僅 sucker 幾何處理不同。

---

## 1. 呼叫階層總覽

```
DoInArm()                              ← ainarm2.cpp (最上層入口)
  └─ DoInArm_9045()                    ← ainarm9045.cpp (Dispatch)
       └─ DoInArm_9045_2x8_8()        ← ainarm9045_2x8_8.cpp (主狀態機, 以 2x8 為例)
            ├─ case 100:  DoInArmPickFromLoadStage_9045()
            ├─ case 400:  DoInArmAdditionalFunction()
            │               ├─ DoInDieClean()
            │               ├─ DoInArmDevicePosPrecise()
            │               ├─ DoBottom2DIDScan()
            │               └─ DoInArmRotateKIT()
            ├─ case 1100: DoPlaceToHotPlate_9045_2x8_8()
            ├─ case 1500: DoInArmPickFromHotPlate_9045()
            └─ case 2000: DoInArmPlaceToShuttle_9045_2x8_8()
```

---

## 2. DoInArm() — 入口 Guard Checks

> Source: `ainarm2.cpp` line 1588-1731

在進入 `DoInArm_9045()` 之前的檢查順序：

1. `bInitialStartIndexCheckDone == false` → return（初始化未完成）
2. `iHPHangUpCount != 0` → HotPlate HangUp alarm WAR0150 → return
3. `bF16CheckShuttleSensorBroken && bDoingF16` → Shuttle sensor 異常 → return
4. **QA Mode** — `Check_QA_ModeCount()` → 設定 `HAS_NULL_IC`
5. **Auto Alignment CCD** — Loader 需求檢查、自動對位流程
6. **IndexJam** — `bIndexJam` 檢查
7. 以上都通過 → 呼叫 `DoInArm_9045()`

---

## 3. DoInArm_9045() — Dispatch

> Source: `ainarm9045.cpp` line ~3907

### Pre-Dispatch Guard Checks

1. AUTO_ALIGNMENT_CCD running → return
2. AutoSiteMappingUseHotPlate → set `bRunAutoSiteMapping`
3. TestingNeedStopAllMotor → `StopAllMotor()` → return
4. bAlarmNeedServoOff && bMyServoOffInArm → return
5. CheckInArmDestroyActive() → if active → return
6. iPauseBackUp != -1 → return
7. bResetInArmTask → `InitInArmTask()` → return

### iInArmType Dispatch Table

| iInArmType | Sub-function | Picker Config |
|---|---|---|
| `ep1Picker` | `DoInArm_9045_All_1Pick()` | Single picker |
| `e9045_1x1_1` / `e9045_1x4_1_Ac` | `DoInArm_9045_1x1_1()` | 1×1, 1 sucker |
| `e9045_1x2_2_13` | `DoInArm_9045_1x2_2()` | 1×2, 2 suckers |
| `e9045_1x2_2_14` | `DoInArm_9045_1x2_2_14()` | 1×2, 2 suckers (1-4) |
| `e9045_1x2_4_Hot` | `DoInArm_9045_1x2_4_Hot()` | 1×2, 4 suckers (Hot) |
| `e9045_1x3_2_14` | `DoInArm_9045_1x3_2_14()` | 1×3, 2 suckers |
| `e9045_1x3_4` | `DoInArm_9045_1x3_4()` | 1×3, 4 suckers |
| `e9045_1x4_4_13` | `DoInArm_9045S_1x4_4()` | 1×4, 4 suckers (S) |
| `e9045_1x4_2_14` | `DoInArm_9045_1x4_2()` | 1×4, 2 suckers |
| `e9045_1x4_4_Back` | `DoInArm_9045_1x4_4_Back()` | 1×4, 4 suckers (Back) |
| `e9045_1x4_4` | `DoInArm_9045_1x4_4()` | 1×4, 4 suckers |
| `e9045_1x4_8_Hot` | `DoInArm_9045_1x4_8_Hot()` | 1×4, 8 suckers (Hot) |
| `e9045_2x1_2_13` | `DoInArm_9045_2x1_2()` | 2×1, 2 suckers |
| `e9045_2x2_4_12` | `DoInArm_9045_2x2_4_12()` | 2×2, 4 suckers (1-2) |
| `e9045_2x2_4_13` | `DoInArm_9045_2x2_4()` | 2×2, 4 suckers |
| `e9045_2x2_4_14` | `DoInArm_9045_2x2_4_14()` | 2×2, 4 suckers (1-4) |
| `e9045_2x2_8_Hot` | `DoInArm_9045_2x2_8_Hot()` | 2×2, 8 suckers (Hot) |
| `e9045_2x3_6_14` | `DoInArm_9045_2x3_6_14()` | 2×3, 6 suckers |
| `e9045_2x3_6` | `DoInArm_9045_2x3_6()` | 2×3, 6 suckers |
| `e9045_2x4_4_13` | `DoInArm_9045_2x4_4_13()` | 2×4, 4 suckers |
| `e9045_2x4_4_14` | `DoInArm_9045_2x4_4()` | 2×4, 4 suckers |
| `e9045_2x4_8` | `DoInArm_9045_2x4_8()` | 2×4, 8 suckers |
| `e9045_2x5_8` | `DoInArm_9045_2x5_8()` | 2×5, 8 suckers |
| `e9045_2x6_8` | `DoInArm_9045_2x6_8()` | 2×6, 8 suckers |
| `e9045_2x8_8` / `e9045_2x8_32` | `DoInArm_9045_2x8_8()` | 2×8, 8/32 suckers |

---

## 4. 主狀態機 — DoInArm_9045_2x8_8() (代表所有 XxY_Z 版本)

> Source: `ainarm9045_2x8_8.cpp` line ~2966  
> Task variable: `int &Task = iArmTask`

### Pre-Switch Guard

- `DoInArmAutoSiteMapping()` || `bIndexAlarmInArmAway` → return

### Ambient Mode 流程（常溫：Loader → Shuttle）

```
case 1  →  case 10  →  case 50  →  case 75  →  case 100
(Wait)     (Decide)    (Prepare)   (HP Check)   (Pick from Loader)
                                                       ↓
case 2000  ←  case 400  ←  case 200  ←──────────────────
(Place to     (Additional   (Close Site
 Shuttle)      Functions)    Check)
```

### Hot Mode 流程（高溫：Loader → HotPlate → Shuttle）

```
case 1  →  case 10  →  case 50  →  case 75  →  case 100
(Wait)     (Decide)    (Prepare)   (HP Check)   (Pick from Loader)
                                                       ↓
                                          case 200  →  case 400
                                          (Close Site) (Additional)
                                                          ↓
              case 1500/1550  ←  case 1100  ←  case 1000
              (Pick from HP)     (Place to HP)  (Pre-Place HP)
                    ↓
              case 2000
              (Place to Shuttle)  →  case 50 (loop back)
```

### HotPlate Verification Flow

```
case 75  →  case 15000  (TryPick from HP Verification)
                  ↓
            case 2000 (if has IC)
            case 100  (if no IC, normal)
            case 75   (if no IC, re-check)
```

### State Descriptions

| case | Name | Description |
|---|---|---|
| **1** | Wait | 移動 InArm XY 到等待位置；檢查 OneCycle/CleanOut |
| **10** | Decision Hub | 檢查 Sucker 上 IC 狀態，決定下一步路由 |
| **50** | Prepare Pick | 檢查 Heater/Tray/FIFO/CleanOut，準備 Loader 取料 |
| **75** | HP Pre-Check | 判斷 HotPlate 是否需要 TrySuck 驗證 |
| **100** | Pick from Loader | 呼叫 `DoInArmPickFromLoadStage_9045()` |
| **200** | Close Site Check | 確認已關閉的 Site 沒有 IC |
| **300** | Close Site Error | 處理 JAM0114 alarm |
| **400** | Additional Functions | 呼叫 `DoInArmAdditionalFunction()` |
| **500** | HP Wait/Pick Ready | 檢查 HotPlate 是否有 Ready IC |
| **600** | Z Safe Move | Z 軸安全上升 |
| **1000** | Pre-Place to HP | 判斷是否能跳過 HP 直接放 Shuttle |
| **1100** | Place to HotPlate | 呼叫 `DoPlaceToHotPlate_9045()` |
| **1500/1550** | Pick from HotPlate | 呼叫 `DoInArmPickFromHotPlate_9045()` |
| **1600** | Post-HP Pick Error | HP 取料後的 Close Site 異常處理 |
| **2000** | Place to Shuttle | 呼叫 `DoInArmPlaceToShuttle_9045()` |
| **15000** | TryPick HP Verify | `DoInArmTryPickFromHotPlate_9045_2x8_8()` HP 驗證 |

---

## 5. DoInArmPickFromLoadStage_9045() — Loader 取料

> Source: `ainarm9045.cpp` line ~7000  
> Task: `iPickFromLoadStageTask`

### 流程摘要

從 Loader Tray 吸取 IC：Z 軸安全 → Tray 有 IC → 搜尋 XY → Z 下降吸取 → 真空驗證 → 異常處理

### 關鍵 Case

| case | 動作 |
|---|---|
| 1 | Tray arm 安全確認、Z safe、OneCycle/DoPickLoaderOK 檢查 |
| 10 | Loader Tray 有無 IC 判斷 → 有: case 12, 無: case 15 |
| 12 | SearchAndMoveInArmXYToLoad_9045() → case 200 |
| 200 | MoveInArmZToLoaderPick → Offset check → case 1000 |
| 1000 | 逐一 Suck() 所有 Sucker → 全部完成: case 2000, 異常: case 1010/1050 |
| 1010 | Retry（Z safe → 重新吸取）|
| 1050 | 超過 Retry 上限 → case 1100（Error Alarm）|
| 1100 | ProcessMES0101InArmPickLoaderError → K_TRAY_END / K_CLEAN_OUT / K_SKIP / K_HOME / K_RETRY |
| 2000 | Z safe + IC Fall Down 檢查 → case 2100 |
| 2100 | ArmFinishForLoader() → return true |

---

## 6. DoInArmAdditionalFunction() — 附加功能

> Source: `ainarm9045.cpp` line ~3182  
> Task: `iInArmAdditionalFunctionTask`

### 流程摘要

依序檢查並執行四種附加功能，每個完成後回到 case 100 檢查下一個：

```
case 1 → case 100 (check flags)
           ├─ Die Clean    → case 10000: DoInDieClean()
           ├─ Precisor     → case 20000: DoInArmDevicePosPrecise()
           ├─ Bottom 2DID  → case 30000: DoBottom2DIDScan()
           ├─ Rotator      → case 40000: Z safe → case 41000: DoInArmRotateKIT()
           └─ All done     → return true
```

---

## 7. DoPlaceToHotPlate_9045() — HotPlate 放料

> Source: `ainarm9045_2x8_8.cpp` line ~1733  
> Task: `iInArmPlaceToHotPlateTask`

### 流程摘要

搜尋 HP 位置 → 移動 XY → Release Delay → Destroy 吹氣放料 → Z safe → 檢查剩餘 IC（分段放料）→ 驗證黏貨

### 關鍵 Case

| case | 動作 |
|---|---|
| 1 | SearchPlateToPlace() → case 100 |
| 100 | MoveInArmXYToHotPlatePlace → case 200；掉料 → case 110 |
| 200 | Offset check + Release delay → case 340/350 |
| 350 | Destroy 吹氣放料（loop 2 rows × N cols）→ case 400 |
| 400 | Z safe + HasIC? → YES: case 1 (分段), NO: case 500 |
| 500 | CheckInArmDestroyICFail（黏貨檢查）→ case 501 |
| 501 | AdjustShuttleWhichKitOrder + Laser check → return true |

---

## 8. DoInArmPickFromHotPlate_9045() — HotPlate 取料

> Source: `ainarm_SearchPickPlate.cpp` line ~683  
> Task: `iInArmPickFromHotPlateTask`

### 流程摘要

搜尋 HP 位置 → 取得 Team 分組 → 移動 XY → 等待 Shuttle 就位 → Z 下降吸取 → HotplateDataConversion 資料轉換 → 異常 Retry/Skip/Home

### 關鍵 Case

| case | 動作 |
|---|---|
| 1 | SearchPlateToPick + SuckerMap → case 50 |
| 50 | Z safe → case 100 |
| 100 | GetHPFirstTeamMotUse / SuckUse（Team 分組）→ case 110 |
| 110 | MoveInArmXYPickHotPlate_9045 → case 150 |
| 150 | 等待 Shuttle 在 Load-Free 位置 → case 190 |
| 200 | Z 下降吸取（MoveInArmZToHotPlatePick）→ case 300 |
| 300 | HotplateDataConversion（資料轉換）→ 異常: case 320, OK: case 340 |
| 330 | JAM0109 alarm → K_SKIP / K_HOME / K_RETRY |
| 350 | DataForwardAndNextTeam → 還有: case 1, 全完成: case 400 |
| 400 | return true |

---

## 9. DoInArmPlaceToShuttle_9045() — Shuttle 放料

> Source: `ainarm9045_2x8_8.cpp` line ~2039  
> Task: `iInArmPlaceToShuttleTask`

### 流程摘要

判斷放 Shuttle 1/2 → 等待 Shuttle Load-Free → 速度調整 → XY/Z 定位 → Destroy 吹氣到 Shuttle → 驗證粘黏/浮起 → 調整順序 → Y Pitch Home

### 架構

兩個 Shuttle 邏輯對稱：
- **Shuttle 1**: case 900 ~ 1610
- **Shuttle 2**: case 1900 ~ 2610（結構相同，case 編號 +1000）

### 關鍵 Case（以 Shuttle 1 為例）

| case | 動作 |
|---|---|
| 1 | SetInArmUseSuckToHasNullIC、HasRealIC 檢查 → case 100 |
| 100 | 選擇 Shuttle 1 (case 900) 或 Shuttle 2 (case 1900) |
| 900 | 等待 InSHT1InLF → speed adjust → case 930/950 |
| 930 | MoveInArmXYToWaitTrayArm(ZAxisNotDown) → case 1000 |
| 950 | MoveInArmXYToWaitTrayArm(ZAxisDown) → case 1100 |
| 1000 | 檢查 carrier kit / bCanFreeShuttle → case 1050 or 900 |
| 1100 | MoveInArmZToShuttlePlace_9045 + Offset → case 1200 |
| 1200 | Destroy 吹氣（loop 2×4）→ 還有: case 1250, 全放完: case 1300 |
| 1300 | InitDoInArmCheckShtFloatTask → case 1400 |
| 1400 | DoInArmCheckShuttleFloating（浮起檢查: 搖搖敲敲）→ case 1500 |
| 1500 | AdjustShtOrderWhenPlaceToSht + Soak time → return true |
| 1600 | Y Pitch Home (bE57YPitchHome) → return true |

---

## 10. 常見問題索引

| 問題 | 查看 |
|---|---|
| InArm 在哪個 case 卡住？ | §4 State Descriptions 對照 `iArmTask` 值 |
| Loader 吸料異常怎麼處理？ | §5 case 1100（MES0101 Alarm: Tray End / Skip / Home / Retry） |
| HotPlate 黏貨怎麼檢查？ | §7 case 500 `CheckInArmDestroyICFail()` |
| Shuttle 浮起怎麼處理？ | §9 case 1300-1400 `DoInArmCheckShuttleFloating()` |
| IC 掉料怎麼檢測？ | 各函式中的 `CheckInArmSuckICFallDownToHasNullIC()` |
| Ambient/Hot 模式差異？ | §4 兩張 ASCII 流程圖：Ambient 跳過 HP，Hot 經過 HP |
| 附加功能執行順序？ | §6：Die Clean → Precisor → Bottom 2DID → Rotator |
| iInArmType 對應哪個函式？ | §3 Dispatch Table |
| 詳細 FlowChart 文件在哪？ | `d:\HT9045\FlowChart\DoInArm_FlowChart.md` 和 `DoInArm_9045_FlowChart.md` |

---

## 使用指引

當回答 InArm 相關問題時：

1. **先定位問題層級** — 是入口層（DoInArm）、Dispatch 層（DoInArm_9045）、還是子函式層
2. **確認溫度模式** — Ambient（常溫）還是 Hot（高溫），流程路徑不同
3. **查對應 case 值** — 使用 State Descriptions 找到 `iArmTask` / 子函式 Task 的意義
4. **需要原始碼** — 使用 Explore agent 到對應檔案讀取詳細邏輯
5. **詳細 Process Flow** — 參考 `d:\HT9045\FlowChart\DoInArm_9045_FlowChart.md` 中各函式的完整 case-by-case 流程