# Implementation：HT9045 Auto Stack-Only

> 對照契約：[stacker-cylinder-stackonly](../mechanisms/stacker-cylinder-stackonly.md)
> 機台：HT9045 V3.33.899.0
> 實作位置：`HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422/asendic_Auto.cpp::DoAutoReceiveBinTray(int Pos)`
> 風格：個別檔案派 + Pos 參數（Auto1/2/3 共用）

---

## 1. 角色映射

| 契約角色 ID | 本機實際命名 | 定義位置 |
|-------------|------------|---------|
| `ACT_LIFT` | `C_Auto_Up[Pos]` | `cmydef.h` |
| `ACT_SELECTOR` | `C_Auto_Selector[Pos]` | `cmydef.h` |
| `ACT_SIDE_FIXER` | `Cylinder[C_AutoSide_Fixer[Pos]]` | `cmydef.h` |
| `ACT_EDGE_PUSH` | `Cylinder[C_AutoEdgePush[Pos]]` | `cmydef.h` |
| `ACT_UP_PRESS` | `Cylinder[C_AutoUpPress[Pos]]` | `cmydef.h` |
| `ACT_Z_SELECT` | `Cylinder[C_AutoZ_Select[Pos]]` | `cmydef.h` |
| `SEN_TRACK_HAS_TRAY` | `Sen[SnAutoTrackDetect[Pos]]` | `cmydef.h` |
| `SEN_TRAY_DETECT` | `Sen[SnAutoTrayDetect[Pos]]` | `cmydef.h` |
| `SEN_CAR_HAS_TRAY` | `Sen[SnAutoTrayCar[Pos]]` | `cmydef.h` |
| `SEN_COLOR_TRAY` | `Sen[SnAutoColorTrayDete[Pos]]` | `cmydef.h` |
| `INI_LIFT_DEFAULT_UP` | `IniConfig.bP37bAutoCylinderUP` | `IniConfig.h` |
| `INI_FULL_NOT_STOP_IMMEDIATELY` | `CosFunction.bAutoFullDoNotStopImmediately` | `CosFunction.h` |
| `INI_FULL_AFTER_UNLOAD_END` | `CosFunction.bFullTrayAlarmAfterUnloadEnd` | `CosFunction.h` |

---

## 2. 函式進入點

| 進入點 | 行號 | 描述 |
|--------|------|------|
| 主流程入口 | `asendic_Auto.cpp::DoAutoReceiveBinTray(int Pos)` L142 | case 1 入口 |
| 待命 reset | L259 case 100 | 各輔助汽缸鬆開 |
| 升降到接盤 | L271–L319 case 110–113 | 多步保護 |
| 分流 | L320 case 200 | fHasTray 判斷 |
| 等手臂補盤 | L326 case 300 | 等待外部 |
| Tray 偵測 + Color | L347 case 500 | 含 CC_TERAPOWER 特例 |
| 錯誤 retry/skip | L389 case 750 | 含 K_SKIP 清盤 |
| **固定剛收的 Tray** | L416 case 800 | N3 關鍵 |
| Sensor retry | L428 case 850 | 3000 次上限 |
| 滿盤策略 | L1081 case 1100 | N5 |

---

## 3. case 編號對齊

| 契約 case | 本機 case | 一致 |
|-----------|----------|------|
| 1 | 1 | ? |
| 100 | 100 | ? |
| 110–113 | 110, 111, 112, 113 | ? |
| 200 | 200 | ? |
| 300 | 300 | ? |
| 500 | 500 | ? |
| 750 | 750 | ? |
| 800 | 800 | ?（N3 固定動作） |
| 850 | 850 | ? |
| 900–970 | 900, 910, 920, 950, 955, 956, 960, 970 | ? Recycle 子流程 |
| 1000+ | 1000, 1010, 1020, 1021, 1022, 1025, 1030, 1050, 1052 | ? 機台特定模式 |
| 1100 | 1100 | ? |
| 1200+ | 1200, 1210, 1215, 1220 | ? Recycle 完整 |
| 1350+ | 1350, 1351, 1352, 1353, ..., 1358, 1400 | ? 後段分離氣缸版 |

> case 數量比 separator 契約多很多（從 ~10 個增至 30+ 個），主要是因為 Auto 同時處理「收盤 / 回收 / TrayFeed / CleanOut」多種模式。
> 本契約只規範**核心收盤週期**（case 1–1100），其餘為機台擴充。

---

## 4. 風格特徵

- **單檔多 Pos 共用**：`DoAutoReceiveBinTray(int Pos)` 一份函式服務 Auto1/2/3
- **多輔助汽缸**：Side Fixer + Edge Push + UpPress 共 3 組
- **接盤位置可配置**：`IniConfig.bP37bAutoCylinderUP` 切換 Up / Middle
- **滿盤策略可配置**：兩個 CosFunction 旗標
- **客戶特例分支**：`CUSTOMER_CODE == CC_TERAPOWER` 等硬編碼分支（?）

---

## 5. 與契約的偏差

| 偏差 | 說明 | 是否合規 |
|------|------|---------|
| case 1100 滿盤判斷分散在多處 | 不只 case 1100，900+ 也含 | ? 契約允許但建議集中 |
| `CC_TERAPOWER` 客戶硬編碼 | Color sensor 特殊處理 | ?? ? 違反 N6 但有註解標註 |
| 含 1350+ 後段分離氣缸版 | 部分機型才用 | ? 應抽出至子契約 |
| 多輔助汽缸（Side+Edge+Up）皆必備 | 契約標 ?（部分機台無） | ? 契約 §1 允許 0–3× |

---

## 6. 同機台兄弟實例

HT9045 此機構共 3 個 Pos：

| Pos | enum | 對應 Auto |
|-----|------|----------|
| 0 | `eAuto1` | Auto1 |
| 1 | `eAuto2` | Auto2 |
| 2 | `eAuto3` | Auto3 |

> 跨 Pos 行為一致（N6 合規），唯有 case 300 含 `if(Pos==0)` 處理 Auto1 收 Loader 空盤特例（ `TrayForm.LoaderToEmptyColor[iRunStartMode]==2 && Pos==0`）—— 屬合理特例（物理位置 Auto1 在 Loader 旁）。

---

## 7. 已知 bug 歷史（從註解反查）

| 日期 | 修改人 | 內容 | case |
|------|-------|------|------|
| 2014-04-09 | Steven | AutoCylinderXX 加 Reset | 110, 1191, 1206, 1222 |
| 2018-07-26 | kevin (wei) | Auto 123 氣缸常態在上 | 110, 1556 |
| 2019-04-23 | JerryYang | 新增 unloader 壓 tray | 100, 750, 800 |
| 2020-12-08 | Steven | Auto Tray 沒放好,重複 Alarm | 750, 1 |
| 2021-06-02 | Sam | Unload 完後再 Alarm | 1097 |
| 2021-10-28 | Sam | Color sensor 增 60s 保護防 hangup | 500 |
| 2021-11-15 | Sam | 確保汽缸上前皮帶持續轉 | 1215 |
| 2024-01-08 | Sam | 退 Tray 顯示 Error Bin 數量 | 750, 850 |
| 2025-07-21 | JerryYang | fix non ART 汽缸沒下降 | 751, 782 |

---

## 8. 變更歷史

| 日期 | 變更 |
|------|------|
| 2026-05-07 | 初版（HT9045 單機；對照契約 stackonly 初版） |
