---
name: ht9045-contact-force
description: HT9045 Contact Force 計算知識庫。當使用者詢問 Contact Force、SLK 缸徑、Air Force、CalculateTotalAirForce、GetMinForce、GetMaxIndexForceLimit、D25 設定、Kit Diameter、Compliance Unit、EP 壓力、Index Press Type、Contact Mode、Contact Test、Auto Height、Load Cell、Contact Offset、Min Force、Max Force、Die Force 等相關問題時，應先載入此技能。關鍵字：Contact Force, SLK, Kit Diameter, CalculateTotalAirForce, GetMinForce, GetMaxIndexForceLimit, D25, D28, D04, Compliance Unit, Air Force, EP, Index Press, cContact, ContactForce。
---

# HT9045 Contact Force 計算流程

## 核心檔案

| 檔案 | 說明 |
|------|------|
| `cContact.cpp` | Contact 模式主控（~19000 行），包含 Force 計算、Auto Height、Contact Test 狀態機 |
| `cContact.h` | TfContact 類別宣告 |
| `ContactForce.cpp` | SLK 類別初始化、Load Rate 讀寫、ContactInfo.ini 管理 |
| `ContactForce.h` | THTSLKClass / THTDieForceSLKClass / THTSLKIndClass 類別定義 |

## Contact Force 計算架構

### 計算入口
```
ShowArmAndDeviceForce()
  → CalculateTotalAirForce(dBallCount, dSingleGf)
    → GetMinForce(dKitDiameter, iTag)        // 最小力量保護
    → GetMaxIndexForceLimit()                 // 最大力量上限
```

### 關鍵公式

#### 1. 單顆 IC 所需力量
```
dDeviceGf = dBallCount × dSingleGf × 0.001    // (kg)
```
- `dBallCount`：Pin 數（球數）
- `dSingleGf`：每 Pin 力量 (gf)

#### 2. 總力量計算
```
dTotalForce = dDeviceGf × dDutCount
```
- `dDutCount` 由 `DutCount()` 依 TestMode 決定

#### 3. 每個 Compliance Unit 分攤力量
```
dNowKgPerHead = dTotalForce / (HeadCount × fComplianceUnit)
```
- `HeadCount` 依 TestMode 而定（見下表）
- `fComplianceUnit` 由 scrbSLK->Position 決定

#### 4. D28 最大力量限制（依缸徑計算）
```
dHeadMaxForce = ((dKitDiameter² × π) / 4) × coefficient × 0.0101972 × fComplianceUnit
```
- `coefficient` = 5.0（EP_MAXKPA ≤ 500）或 6.0（EP_MAXKPA > 500）
- 需開啟 `IniConfig.bD28MaxForceLimitByDiameter`

## SLK 缸徑（Kit Diameter）對照表

### 最小力量（Min Force per Compliance）

| 缸徑 (mm) | 預設 Min Force (kg) | D04 覆蓋參數 |
|-----------|---------------------|-------------|
| 20        | 0.5                 | `dD04MinForceByFile_20mm` |
| 28, 30    | 1.5                 | `dD04MinForceByFile_30mm` |
| 40        | 4.0                 | `dD04MinForceByFile_40mm` |
| 58, 60    | 8.0                 | `dD04MinForceByFile_60mm` |
| 80        | 15.0                | `dD04MinForceByFile_80mm` |
| 其他      | 讀 SLKClass.dContactOffset | `dD04MinForceByFile` |

> 若 `IniConfig.bD04MinForceByFile == true`，使用 ini 檔設定值（取較大者）。

### 最大力量（D28 MaxForce by Diameter）

以缸徑 30mm、EP_MAXKPA=499 為例：
```
dHeadMaxForce = ((30² × 3.14) / 4) × 5.0 × 0.0101972 × fComplianceUnit
             = (706.5 / 4) × 5.0 × 0.0101972 × fComplianceUnit
             = 176.625 × 5.0 × 0.0101972 × fComplianceUnit
             ≈ 9.01 × fComplianceUnit (kg)
```

### 各缸徑 D28 MaxForce 速查（fComplianceUnit=1.0）

| 缸徑 (mm) | EP≤500kPa (coeff=5) | EP>500kPa (coeff=6) |
|-----------|---------------------|---------------------|
| 20        | ≈ 4.00 kg           | ≈ 4.80 kg           |
| 28        | ≈ 7.85 kg           | ≈ 9.42 kg           |
| 30        | ≈ 9.01 kg           | ≈ 10.81 kg          |
| 40        | ≈ 16.02 kg          | ≈ 19.22 kg          |
| 58        | ≈ 33.67 kg          | ≈ 40.40 kg          |
| 60        | ≈ 36.02 kg          | ≈ 43.23 kg          |
| 80        | ≈ 64.04 kg          | ≈ 76.85 kg          |

> 此限制僅在 `IniConfig.bD28MaxForceLimitByDiameter == true` 時啟用。

### SLK 類別預設 MinForce / MaxForce（ContactForce.cpp 建構子）

| 條件 | dMinForce | dMaxForce 公式 |
|------|-----------|---------------|
| diameter ≤ 30 | 0.5 | π × (d/100)² × 500 |
| 30 < diameter < 40 | 1.0 | 同上 |
| 40 ≤ diameter < 50 | 2.0 | 同上 |
| 50 ≤ diameter < 60 | 4.0 | 同上 |
| diameter = 402 (40x2) | 4.0 | 同上 |
| diameter ≥ 60 | 8.0 | 同上 |

SLK MaxForce 公式：`dMaxForce = 3.14 × (diameter/100)² × 500`
- 30mm → `3.14 × 0.09 × 500 = 141.3 kgf`
- 40mm → `3.14 × 0.16 × 500 = 251.2 kgf`

## Compliance Unit（scrbSLK Position）

| Position | 說明 | fComplianceUnit |
|----------|------|----------------|
| 2 | 1 Device / 1 Compliance | 1.0 |
| 3 | 2 Device / 1 Compliance | 0.5 |
| 4 | 4 Device / 1 Compliance | 0.25 |
| 5 | 2 Device / 4 Compliance | 2.0 |
| 6 | 8 Device / 1 Compliance | 0.125 |

## Index Press Type 最大力量上限

| INDEX_PRESS_TYPE | Max Limit (kg) |
|-----------------|----------------|
| e85KG (default) | 85 |
| e120KG | 120 |
| e160KG | 160 |
| e240KG | 240 |
| e260KG | 260 |
| e360KG | 360 |
| e400KG | 400 |
| e500KG | 500 |
| e640KG | 640 |
| e800KG | 800 |

> 預設 85 kg 機台：40mm 缸徑+Single Site 最多 55 kg，60mm 以上最多 85 kg。

## DutCount 與 HeadCount（TestMode 對應）

| TestMode | dDutCount | HeadCount（CalculateTotalAirForce 除數） |
|----------|-----------|----------------------------------------|
| SingleSite | 1 | 1 |
| DualSite / 2X2N / 2x1 | 2 | 2 |
| TriSite1X3 / 2X3N | 3 | 3 |
| 1X4 / 2X2 / 1X4_8 / 2X4N | 4 | 4 |
| 2X3_6 | 6 | 6 |
| 2X4_8 | 8 (12 if bOctal_12Kit) | 8 (12) |
| 2X5_10 | 10 | 10 |
| 2X6_12 | 12 | 12 |
| 2X8_16 | 16 (8 if half-site) | 16 (8) |
| 4X4_16 | 8 | 8 |
| 4X8N_32 / 4X8M_32 | 16 | 16 |

## 客戶特殊邏輯

- **ASE_KaohSiung + 2X2**：缸徑 30mm 時 dMinForce 被覆蓋為 1 kg
- **KYEC_LEE**：預設 SLK Type 為 28,40,58,56（非標準 30,40,60,56）
- **D27 (bD27UseSingleSite85kg)**：DualSite 僅開一個 Site 時，可使用整個 85kg 上限

## 設定檔路徑

| 檔案 | 說明 |
|------|------|
| `D:\HT9045\system\ContactInfo.ini` | SLK Type 列表、Load Rate、Contact Offset |
| `D:\HT9045\system\Gerneral.ini` | EP_MAXKPA、EP_MAXA、EP_MINMPA |
| cConfiguration (D04) | bD04MinForceByFile、各徑 Min Force |
| cConfiguration (D27) | bD27UseSingleSite85kg |
| cConfiguration (D28) | bD28MaxForceLimitByDiameter |

## EP 壓力差異與比例閥異常警報

### EP 壓力差異警報（WAR1605）

#### 核心判斷函式

| 函式 | 檔案 | 說明 |
|------|------|------|
| `ADAM_Alarm(iArm)` | adam6024.cpp L491 | PA 回讀值 vs 設定值差異超過 `iADAMRange` |
| `ADAM_Alarm_Kg(iAdd)` | adam6024.cpp L548 | 以 Kg 為單位比較，Range 自動分級 |
| `ADAM_DualAlarm(iType)` | adam6024.cpp L612 | Dual EP 通道差異檢查 |

#### ADAM_Alarm_Kg Range 自動分級

| 力量範圍 | 容許偏差 |
|----------|---------|
| ≤ 5 kg | ± 0.25 kg |
| 6–10 kg | ± 0.5 kg |
| 11–60 kg | ± 1.0 kg |
| 61–120 kg | ± 2.0 kg |

#### 呼叫時機

| 時機 | 函式 | 檔案行號 |
|------|------|---------|
| Index 每次測試前 | `IndexEveryTimeCheckEP()` | atester.cpp L8887 |
| Index 測試中記錄 | `CheckAndRecodrEP(iArm)` | atester.cpp L8995 |
| Contact Z1 取料 | `DoZ1PickFromShuttle` case 110 | cContact.cpp L2504 |
| Contact Z2 取料 | `DoZ2PickFromShuttle` case 110 | cContact.cpp L3475 |
| Auto Height | `Do_Z1/Z2_AutoGetHeight` case 2800 | cContact.cpp L6178/L8752 |

#### EP Leakage 補償
Auto Height 完成時若 `ADAM_Alarm()` 回 true → `bEPLeakage=true` → 高度少補 20 pulse（≈0.2mm）

#### 開關設定

| Config | 變數 | 功能 |
|--------|------|------|
| D24 | `bD24EnableEPCheckFuntion` | EP Check 主開關 |
| — | `bIndexEveryTimeCheckEP` | Index 每次測試前檢查 |
| D26 | `bD26EnableEPEncoderRange` | EP Encoder Range 差異檢查 |
| D26 edD26 | `iD26EPEncoderRange` | 差異容許範圍（kPa），預設 100 |
| D26_1 | `bD26EnableEPLog` | EP Log 記錄 |
| D26_2 | `bD26EnableEncodeShow` | EP Value 顯示 |
| D26_3 | `bD26_3EnableDualEPEncoderRange` | Dual EP 差異檢查 |

### 比例閥電壓異常警報（WAR16322/WAR16323）

| 項目 | 內容 |
|------|------|
| 函式 | `ADAM_ReturnValueCheck(bHome)` |
| 檔案 | adam6024.cpp L2200 |
| 正常電壓範圍 | DC 1.0V ~ 5.0V |
| 異常判定 | 回讀電壓 < 0.8V 或 > 5.2V |
| 累計門檻 | 連續異常 100 次（≈100秒）觸發警報 |
| 警報代碼 | Arm1: WAR16322, Arm2: WAR16323 |
| Home 模式 | `bHome=true` 時立即觸發，並阻止回原點 |

### 警報代碼總覽

| 代碼 | 訊息 | 觸發源 | 操作 |
|------|------|--------|------|
| WAR1605 | 請檢查EP是否漏氣 | EP壓力差異超範圍 | Retry/Skip |
| WAR16322 | EP Controller Arm1 電壓異常 | 比例閥回讀電壓異常 | Retry |
| WAR16323 | EP Controller Arm2 電壓異常 | 比例閥回讀電壓異常 | Retry |

## Contact Mode 列表

| 常數 | 值 | 說明 |
|------|---|------|
| CONTACT_NORMAL | 0 | 一般模式 |
| CONTACT_AUTO_GET_HEIGHT | 1 | 自動取高 |
| CONTACT_MANUAL_GET_HEIGHT | 2 | 手動取高 |
| CONTACT_TEST | 3 | Contact Test |
| AUTO_CONTACT_TEST | 4 | Auto Contact Test |
| STEP_CONTACT_TEST | 5 | Step by Step Contact Test |
| CONTACT_IN_SHUTTLE_CHECK | 6 | InShuttle 檢查 |
| CONTACT_OUT_SHUTTLE_CHECK | 7 | OutShuttle 檢查 |
| CONTACT_LoadCell_AUTO_GET_HEIGHT | 8 | Load Cell 自動取高 |
| CONTACT_DEVICE_MAP_CHECK | 9 | Device Map Check |
| CONTACT_DEVICE_LOOP_TEST | 10 | Device Loop Test |
| K_TEMP_INDEX_MOVE | 11 | K Temperature 模式 |

## 狀態機函式

| 函式 | 說明 |
|------|------|
| `DoTestContactFunction()` | Contact Mode 主排程 |
| `Do_Z1_AutoGetHeight()` | Z1 自動取高（~2700 行） |
| `Do_Z2_AutoGetHeight()` | Z2 自動取高 |
| `Do_ContactTest_32Site()` | 32-site Contact Test |
| `DoZ1PickFromShuttle()` | Z1 從 Shuttle 取 IC |
| `DoZ2PickFromShuttle()` | Z2 從 Shuttle 取 IC |
| `DoZPlaceToShuttle()` | 放 IC 回 Shuttle |
| `Do_AutoContactTest()` | 自動 Contact Test |
| `DoStepContactLoadDevice()` | Step Contact 上料 |
| `DoStepContactUnloadDevice()` | Step Contact 下料 |
| `Do_ROILearning()` | RTC ROI 學習 |
| `DoFullViewCheck()` | FullView 檢查 |
| `Do_LoadCellAutoHigh()` | Load Cell 自動取高 |
| `DoDeviceMapCheck()` | Device Map 檢查 |
| `DoContactDeviceLoopTest()` | Device Loop Test |
| `DoContactKTemperatureTest()` | K Temperature Test |
| `DoRTCAutoTuning()` | RTC Auto Tuning |
| `Do2DIDMapCheck()` | 2DID Map Check |
| `DoIndecxCHECkFunction()` | Index 檢查功能 |
