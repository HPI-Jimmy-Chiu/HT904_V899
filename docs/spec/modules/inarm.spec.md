# 模組 SPEC：InArm（入料手臂）

> 骨架版本。實際內容應根據 `ht9045-inarm-flow` skill 與 V899 程式碼補完。

## 1. 模組身份

- **模組名稱**：InArm（Input Arm / 入料手臂）
- **負責檔案**：
  - `ainarm9045_1x1_1.cpp`
  - `ainarm9045_2x4_4.cpp`
  - `ainarm9045_2x4_16.cpp`
  - `ainarm9045_2x8_32.cpp`
  - `ainarm.h`（注意：實為散落多檔，無集中標頭）
- **適用版本**：V897, V898, V899
- **最後 review 日期**：2026-05-02
- **review 人**：JimmyChiu

## 2. 模組職責

從 Loader 取 IC，放到入料 Shuttle 或 HotPlate；負責吸取真空判定、掉料保護、Auto Site Mapping。

## 3. 條件錨點

| 條件 | 取值 | 意義 | 程式碼位置 |
|------|------|------|-----------|
| `iInArmType` | `1x1` / `2x4` / `2x4_16` / `2x8_32` | 機種配置 | `cmydef.h` |
| `bUseHotPlate` | bool | 是否經過 HotPlate 預熱 | `cprod.h` |
| `iArmTask` | enum | InArm 狀態機目前狀態 | `ainarm.h` |

## 4. 資料錨點

| 變數 / 結構 | 型別 | 用途 | 來源檔 |
|------------|------|------|--------|
| `InArmSuck[]` | `TMySucker` | 真空產生器陣列 | `MyKitSuck.h` |
| `HotPlateData[]` | struct | HotPlate 槽位 IC 狀態 | `cprod.h` |
| `iSuckMap[]` | int | 吸嘴 → site 映射表 | `cmydef.h` |

## 5. 行為錨點

| 函式 | 觸發時機 | 副作用 | 安全等級 |
|------|---------|--------|---------|
| `DoInArm_9045()` | 主迴圈每 cycle | 馬達 + IO | 安全關鍵 |
| `DoArmPickFromLoadStage_9045_2x4_16()` | 從 Loader 取料 | 吸取真空 | 安全關鍵 |
| `DoInArmPickFromHotPlate_9045_2x4_16()` | 從 HotPlate 取料 | DataSwap | 安全關鍵 |
| `DoPlaceToHotPlate()` | 放到 HotPlate | DataSwap | 安全關鍵 |

## 6. 狀態機

詳見 `ht9045-inarm-flow` skill。

## 7. 與其他模組的互鎖

- 上游：主排程器
- 下游：HotPlate、InShuttle1/2、Sucker
- 互鎖點：
  - InShuttle 在位 sensor → 才能放料
  - HotPlate 槽位 `HotPlateData[i]` 必須一致 → DataSwap 保護
  - 吸取真空 sensor → 才能離開取料點

## 8. Known Issues / 歷史 bug

- V895 之前：HotPlate DataSwap 在 16-site 配置會錯位 → 修復於 V899（`F899-001-hotplate-dataswap`）

## 9. 關聯功能 SPEC

- `features/F899-001-hotplate-dataswap.spec.md`
