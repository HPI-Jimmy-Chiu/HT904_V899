# 功能 SPEC：F899-001 HotPlate DataSwap (2x4_16 配置)

## 1. Metadata

| 欄位 | 內容 |
|------|------|
| Feature ID | F899-001 |
| 標題 | InArm 在 2x4_16 配置下放料 / 取料 HotPlate 的 DataSwap 修正 |
| 客戶 / 來源 | 內部品質 / 跨版移植 |
| 對應 CASE-ID | CASE-20260423-001 |
| 來源版本 | V895（bug 版） / V897（部分修） |
| 目標版本 | V899 |
| 影響模組 | InArm |
| 安全等級 | 安全關鍵（IC 位置追蹤錯誤會撞機） |
| 作者 | JimmyChiu |
| 日期 | 2026-04-22 |

## 2. 問題 / 需求

在 16-site（2x4_16）InArm 配置下，將 IC 放至 HotPlate 後，HotPlate 內部資料陣列 `HotPlateData[]` 與實際 IC 物理位置會錯位。後續 InArm 從 HotPlate 取料時，會挑到錯誤槽位 → 真空吸不到 / 撞 socket。

## 3. 三維度錨點

### 3.1 條件錨點

```cpp
if (iInArmType == 2x4_16) {
    // 16-site DataSwap 路徑
}
```

| 條件 | 來源檔案 |
|------|---------|
| `iInArmType == 2x4_16` | `cmydef.h` |
| `bZFlgToHP == true` | InArm 內部旗標，表示要放到 HotPlate |
| `bUseHotPlate == true` | `Gerneral.ini` 設定 |

### 3.2 資料錨點

| 變數 / 設定 | 讀 / 寫 | 來源檔 | 備註 |
|------------|---------|--------|------|
| `HotPlateData[i].iIcStatus` | R/W | `cprod.h` | 槽位 IC 狀態 |
| `HotPlateData[i].sLotID` | R/W | `cprod.h` | 槽位批號 |
| `iSuckMap_2x4_16[]` | R | `cmydef.h` | 吸嘴 → site 映射表 |
| `Row2CanPutHP` | R | `ainarm9045_2x4_16.cpp` 內部宣告 | 第二排是否可放 HotPlate |

### 3.3 行為錨點

| 動作 | 函式 | 副作用 |
|------|------|--------|
| 搜尋可放 HotPlate 槽位 | `ainarm_SearchPlacePlate()` | 回傳目標 row/col |
| 放料並交換資料 | `DoPlaceToHPSwapData()` | 寫入 `HotPlateData[]` |
| 從 HotPlate 取料 | `DoInArmPickFromHotPlate_9045_2x4_16()` | 真空吸取 + 清空 `HotPlateData[]` |

## 4. 設計決策

- 採用「先 SearchPlate → 後 SwapData」兩階段，避免在搜尋過程中陣列被改寫造成 race condition。
- 16-site 因有兩排（front / back）需個別維護，採 `Row2CanPutHP` 旗標控制第二排是否啟用。

## 5. 實作清單

- [x] 修改 `ainarm9045_2x4_16.cpp` 的 `DoPlaceToHPSwapData()`
- [x] 修改 `ainarm9045_2x4_16.cpp` 的 `ainarm_SearchPlacePlate()`
- [x] //AI 註解：`//AI(JimmyChiu) 20260422: HotPlate DataSwap 16-site 修正`

## 6. 驗證方式

- [x] BCB6 build V899 通過
- [x] 模擬 16-site 跑 100 顆 IC，HotPlateData 對應一致
- [ ] 客戶 site 實機驗證

## 7. 風險 / 副作用

- 不影響 1x1 / 2x4 / 2x8_32 配置（透過 `iInArmType` 分派）
- 需 regression：InArm Pick from HotPlate、AutoClean recycle 流程

## 8. Release Note 摘要

> **【V899】InArm 16-site HotPlate 取放料資料修正**
> 修正 16-site 配置下 InArm 放料至 HotPlate 後，內部資料陣列與實際槽位錯位的問題。改善後可避免 InArm 從 HotPlate 取料時撞 socket 或吸取失敗。

## 9. 關聯資料

- 模組 SPEC：`modules/inarm.spec.md`
- Skill：`ht9045-v899`、`ht9045-inarm-flow`、`ht9045-inarm-suck-logic`
