---
name: ht9045-tray-group-mechanism
description: HT9045 料盤升降機構（Tray Group GoUp/GoDown）通用知識庫。Loader、Color、Empty 三組料盤站的升降機構原理完全相同，僅機台定義名稱不同。當使用者詢問 Loader GoUp、Loader GoDown、Color GoUp、Color GoDown、Empty GoUp、Empty GoDown、料盤升降、Tray Group 升降原理、C_Color_Up、C_Empty_Up、C_Load_Up、C_LoaderUpPress、CylinderUp、CylinderMiddle、CylinderLower、DoLoadNewColorTrayToCar、DoLoadNewEmptyTrayToCar、DoSupplyNewICTray、asendic_Loader、asendic_Color、asendic_Empty、SnLoaderUpSafedetect、SnEmptyUpSafedetect、SnColorUpSafedetect、料盤站機構差異、三組機構對比 等相關問題時，應先載入此技能。關鍵字：Tray Group, GoUp, GoDown, Loader, Color, Empty, C_Color_Up, C_Empty_Up, C_Load_Up, CylinderUp, CylinderMiddle, CylinderLower, asendic_Loader, asendic_Color, asendic_Empty, 料盤升降, 升降機構。
---

# HT9045 Tray Group GoUp/GoDown Mechanism

## 核心知識

**Loader、Color、Empty 三組料盤站的升降機構結構完全相同，僅名稱不同。**

三者共用：
- **同一個 `TMyCylinder` 類別**（定義於 `mycylin.h`）
- **相同的三階位置控制**：`CylinderUp()` → `CylinderMiddle()` → `CylinderLower()`
- **相同的狀態機流程**（`switch(Task)` 結構，case 編號對應一致）
- **相同的安全感測邏輯**（UpSafedetect sensor）

差異僅在於：變數名稱、氣缸/感測器常數 ID、JAM 報警碼、檔案名稱。

## 原始檔位置

| 機構 | 主程式 | 關鍵函式 |
|------|--------|----------|
| Loader | `asendic_Loader.cpp` | `DoSupplyNewICTray()` |
| Color | `asendic_Color.cpp` | `DoLoadNewColorTrayToCar()` |
| Empty | `asendic_Empty.cpp` | `DoLoadNewEmptyTrayToCar()` |

> 三個檔案結構一致，include 相同模組（MyMotor, mycylin, myswitch, cmydef 等）。

## 共用狀態機流程

以下是三組機構共用的升降 GoUp/GoDown 狀態機骨架：

```
case 1:   初始化 → 檢查是否已有 Tray
case 20:  Middle 汽缸防護（timeout 保護）
case 50:  ASE Report / TrayID 讀取前處理
case 60:  ★ CylinderUp(C_XXX_Up) → 料盤升到最高位
case 100: Z_Select 汽缸推出（分離爪）
case 200: ★ CylinderMiddle(C_XXX_Up) → 中層停止（含 pause 檢查）
case 300: Z_Select 汽缸復位
case 400: 延遲等待（5 秒）
case 410: ★ CylinderLower(C_XXX_Up) → 下降到最低位
case 420: 感測器到位驗證（SnXXXCCWDete / CarHasTray sensor），失敗報 JAM
```

其中 `C_XXX_Up` 依機構替換為 `C_Color_Up` / `C_Empty_Up` / `C_Load_Up`。

## 命名對照表

詳見 [references/naming-map.md](references/naming-map.md)，包含：
- 氣缸常數對照
- 感測器常數對照
- 函式名稱對照
- JAM 報警碼對照

## 實務應用

1. **修改一組升降邏輯時**，務必確認另外兩組是否需同步修改
2. **新增 Tray 站（如 Auto4~6 升降）**，可直接複製任一組狀態機，替換名稱常數即可
3. **除錯時**，若 Color 升降異常，可參照 Empty/Loader 的正常行為做比對
4. **Loader 特殊性**：Loader 使用額外的 `C_LoaderUpPress`（ID=59）氣缸做壓盤，Color/Empty 無此機構
