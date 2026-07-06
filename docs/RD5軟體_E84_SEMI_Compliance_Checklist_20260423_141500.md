# RD5軟體 — HT9045 V899 SEMI E84 相容性檢查清單

> 版本：HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422
> 對照標準：SEMI E84-0418 Specification for Enhanced Carrier Handoff Parallel I/O Interface
> 產生時間：2026-04-23 14:15:00

---

## 1. 訊號介面盤點

### 1.1 Equipment → AGV (Active Output)
| SEMI 訊號 | V899 實作 | 行為 | 相容性 |
|----------|----------|------|:------:|
| L_REQ | `SwE84_2_LREQ` (Unload only) | case 200 ON / case 600 OFF | ? |
| U_REQ | `SwE84_1_UREQ` (Load only) | case 200 ON / case 600 OFF | ? |
| READY | `SwE84_x_READY` | case 400 ON / case 800 OFF | ? |
| HO_AVBL | `SwE84_x_HOAVBL` | **每 tick 強制 ON**（[L240](../HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422/Automation/AGV.cpp#L240)） | ?? 不反映 Door/FullTray/Alarm |
| ES | `SwE84_x_ES` | **每 tick 強制 ON**（[L239](../HT9011uC_Code_V3.33.899.0_20260323_Jimmy_20260422/Automation/AGV.cpp#L239)） | ?? 永不出 ES |

### 1.2 AGV → Equipment (Passive Input)
| SEMI 訊號 | V899 實作 | 相容性 |
|----------|----------|:------:|
| VALID | `SnE84_x_VALID` | ? |
| CS_0 | `SnE84_x_CS0` | ?? 編碼非標準 |
| CS_1 | `SnE84_x_CS1` | ?? 編碼非標準 |
| TR_REQ | `SnE84_x_TRREQ` | ? |
| BUSY | `SnE84_x_BUSY` | ? |
| COMPT | `SnE84_x_COMPT` | ? |
| AM_AVBL | `SnE84_x_AMAVBL` | ? 顯示用 |
| CONT | `SnE84_x_CONT` | ?? 僅顯示，**無 Continuous Handoff 邏輯** |
| GO | `SnE84_x_GO` | ? **9045 自訂訊號**，標準未定義 |

---

## 2. 計時器相容性

| 計時器 | SEMI 標準範圍 | V899 預設 | 狀態 |
|:------:|:-------------:|:--------:|:----:|
| TP1 | 2 s | 2 s | ? |
| TP2 | 2 s | 2 s | ? |
| TP3 | ? 60 s | 60 s | ? |
| TP4 | 60 s | 60 s | ? |
| TP5 | 2 s | 2 s | ? |
| TA1 | 2 s | 2 s | ? |
| TA2 | ? 2 s | 120 s | ?（保守） |
| TA3 | 60 s | 60 s | ? |
| TD0 | ? TA1 | 60 s | ? |
| TD1 | — | 60 s | ?? 宣告但未使用 |

---

## 3. 狀態機完整性檢查

| SEMI 必要動作 | V899 對應 | 狀態 |
|--------------|-----------|:----:|
| Idle 等待 CS+VALID | `case 1 → 50` | ? |
| 解碼 CS_0/CS_1 | `case 100` | ?? 把「同時 ON」當第三 buffer |
| 驅動 L_REQ/U_REQ | `case 200` | ? |
| 等 TR_REQ + 驅動 READY | `case 300 → 400` | ? |
| BUSY 期間等實體完成 | `case 500 → 600` | ? |
| Handoff 完成 → 撤 L_REQ/U_REQ | `case 600` | ? |
| 等 TR_REQ↓+COMPT↑ → 撤 READY | `case 700 → 800` | ? |
| 等 VALID/COMPT/CS 全清 | `case 900` | ? |
| Cycle 結束回 Idle | `case 1000` | ? |
| **異常 → 出 HO_AVBL OFF** | ? 未實作 | ? |
| **異常 → 出 ES OFF** | ? 未實作 | ? |
| Continuous Handoff (CONT) | ? 未實作 | ? |
| Pin Lock / Pin Free | ? 未實作（9045 用機械 cylinder） | N/A |

---

## 4. 弱點與建議優先序

| # | 風險 | 影響 | 建議修正 | 優先 |
|:-:|------|------|---------|:----:|
| 1 | HO_AVBL 永遠 ON | 機台異常時 AGV 仍會送料，造成衝撞或 hang up | 在 [AGV.cpp#L240](../HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422/Automation/AGV.cpp#L240) 改為條件式：機台 Auto/無嚴重 Alarm/Buffer 未滿 才 ON | ? 高 |
| 2 | ES 永遠 ON | EMO 觸發後 AGV 不知道 | 接 EMO sensor → ES OFF | ? 高 |
| 3 | Timeout 時未顯式 OFF SW | 異常後 UREQ/READY 殘留 ON，下次 cycle 不乾淨 | case 5000 加 SW Off + 適當延遲 | ? 中 |
| 4 | CS 第三 buffer 編碼非標準 | AGV 廠商必須客製化 | 文件化此規格 (RD5_E84_CustomSpec) 或改用標準 multi-port 接線 | ? 中 |
| 5 | `case 2000` 跳對話框 | 自動運轉中遇 CS 不一致會被阻塞 | 改成 ShowE84Log + Alarm，不阻塞執行緒 | ? 中 |
| 6 | 主迴圈無 `USE_E84_Sensor` gating | 未啟用 E84 但實體 IO 接錯仍可能誤動 | [csystem.cpp#L15871](../HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422/csystem.cpp#L15871) 包 `if(USE_E84_Sensor)` | ? 低 |
| 7 | `GO` 訊號自訂 | 與其他客戶 AGV 整合困難 | 評估是否能用 VALID 取代 | ? 低 |
| 8 | CONT 訊號未處理 | 不支援 Continuous Handoff | 視客戶需求補實作 | ? 低 |

---

## 5. 驗證建議步驟（出貨前）

1. **訊號 Loopback 測試**：用模擬器分別觸發 CS0、CS1、CS0+CS1 三組合，確認 `iPlaceWhichBuffer` 解碼正確、各 Tray 進入正確 Buffer。
2. **Timeout 測試**：模擬器在每個 case 故意不回 → 觀察 5000 是否回到 case 1 且下一 cycle 仍可正常啟動。
3. **異常注入**：機台 Alarm（FullTray、Door Open、EMO）情境下，AGV 端應收到 HO_AVBL OFF / ES OFF（**目前不會，需先補強再測**）。
4. **錯誤 buffer**：故意讓 AGV 在 case 800 之後改變 CS → 確認 case 2000 對話框出現，不會卡死自動。
5. **Log 檢視**：跑完一個完整 cycle 後檢查 `D:\HT9045\Error\` 內 `ShowE84Log` 紀錄，每個 step 應有 `Sensor_xxx ON / Switch_xxx ON / OFF` 完整序列。

---

## 6. 變更影響範圍備忘

- 任何修改 `SwE84_*_HOAVBL/ES` 的條件式 → 需通報 AGV/OHT 廠商重做整合測試。
- 任何修改 `iE84TimeOut_K12[][]` 預設值 → 需更新 `config/AGV.ini` template 與 release note。
- 任何 CS 編碼修改 → 屬 **safety-critical-change**，須走 ADR 流程。
