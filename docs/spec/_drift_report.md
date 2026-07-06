# SPEC Drift 偵測報告

- 比對版本：`HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422`
- SPEC 數：6

> 收集程式碼符號中... (這需要幾秒)

## `docs/spec/modules/inarm.spec.md`

### 失蹤函式 (1)

- `DoInArm_9045()` — SPEC 引用但版本內找不到此函式定義

### 失蹤檔案 (1)

- `ainarm.h` — SPEC 引用但版本內找不到此檔案

### 可能失蹤的變數/結構 (5)

> 注意：變數檢查為寬鬆比對，可能有誤報。
- `HotPlateData`
- `bUseHotPlate`
- `iArmTask`
- `iInArmType`
- `iSuckMap`

## `docs/spec/features/F899-001-hotplate-dataswap.spec.md`

### 失蹤函式 (2)

- `DoPlaceToHPSwapData()` — SPEC 引用但版本內找不到此函式定義
- `ainarm_SearchPlacePlate()` — SPEC 引用但版本內找不到此函式定義

### 可能失蹤的變數/結構 (1)

> 注意：變數檢查為寬鬆比對，可能有誤報。
- `HotPlateData`

## `docs/spec/features/F899-002-csiteusemgr-revert.spec.md`

### 可能失蹤的變數/結構 (3)

> 注意：變數檢查為寬鬆比對，可能有誤報。
- `bUseCompactSearch`
- `e9045_1x4_4`
- `iPickCol`

## `docs/spec/features/F899-003-main-status-inquire-fix.spec.md`

[OK] 無 drift

## `docs/spec/features/F899-004-autoclean-delay-default-1s.spec.md`

### 可能失蹤的變數/結構 (1)

> 注意：變數檢查為寬鬆比對，可能有誤報。
- `InArmVacuum`

## `docs/spec/features/F899-005-monitor-fourth-camera-record.spec.md`

[OK] 無 drift

## 摘要

- 總 drift 數：14
- 狀態：[警告] 有 SPEC 過時，請更新