# 功能 SPEC：V899 Multi EP 完整移植 (INSTALL_DOUBLE_EP==3)

## 1. Metadata

| 欄位 | 內容 |
|------|------|
| Feature ID | F899-MEP3-FULL |
| 標題 | INSTALL_DOUBLE_EP==3 由 V899 砍半版改為完全照搬 V874.3 16ch 版 |
| 客戶 / 來源 | SIGURD（先期客戶）/ 跨版移植 |
| 對應 CASE-ID | CASE-20260504-001 |
| 來源版本 | V874.3 (Ifor 完整版) |
| 目標版本 | V899 (Jimmy) |
| 影響模組 | ContactForce, adam6024, APAX |
| 安全等級 | 安全關鍵（壓力輸出錯誤可能撞 IC / 損 socket） |
| 作者 | JimmyChiu |
| 日期 | 2026-05-04 |
| 對應 ADR | ADR-0004 |

## 2. 問題 / 需求

V899 早段我把 INSTALL_DOUBLE_EP==3 實作為「砍半版」（4 sites: Arm1_1/2 + Arm2_1/2，分兩次 ADAMTCP_WriteReg(reg=1, count=8) 與 (reg=33, count=8) 寫入），原始假設是 SIGURD 1x2 模式下用兩塊 ADAM-6024 各只用 ch0~3。

實際硬體確認後：
- 機台只有 1 顆 APAX-5070（Address[2] = 172.16.8.112）+ 2 顆 APAX-5028
- 線路是「16ch 連續寫入」，不是「兩塊各寫 8ch」
- V874.3 原始實作就是單次 `ADAMTCP_WriteReg(Address[2], 1, 1, 16, wData2)`
- 「砍半版」假設與實機線路不符，且未來升 2x4 還要再改

→ 撤回砍半設計，**完全照搬 V874.3 ==3 行為**（16ch 連續寫、8 個 OneByOne site 容器）。

## 3. 硬體規格

| 項目 | 值 |
|------|-----|
| 主控 IP | 172.16.8.112 (Address[2]) |
| 板卡組合 | 1× APAX-5070 + 2× APAX-5028 |
| 連續通道數 | 16 |
| 寫入方式 | `ADAMTCP_WriteReg(Address[2], 1, 1, 16, wData2)` 單次 |
| 通道對應 | 見 §4 |

## 4. Channel Mapping (照搬 V874.3 adam6024.cpp:2280)

```
wData2[0]  = iAPAXDualEPValue[0]   Arm1 Dual1 (內缸 1)
wData2[1]  = iAPAXDualEPValue[1]   Arm1 Dual2 (內缸 2)
wData2[2]  = iAPAXEPValue[0]       Arm1 Site1 (外缸)
wData2[3]  = iAPAXEPValue[1]       Arm1 Site2 (外缸)
wData2[4]  = iAPAXEPValue[2]       Arm1 Site3 (外缸)
wData2[5]  = iAPAXEPValue[3]       Arm1 Site4 (外缸)
wData2[6]  = iAPAXDualEPValue[2]   Arm1 Dual3 (內缸 3)
wData2[7]  = iAPAXDualEPValue[3]   Arm1 Dual4 (內缸 4)
wData2[8]  = iAPAXDualEPValue[4]   Arm2 Dual1
wData2[9]  = iAPAXDualEPValue[5]   Arm2 Dual2
wData2[10] = iAPAXEPValue[4]       Arm2 Site1
wData2[11] = iAPAXEPValue[5]       Arm2 Site2
wData2[12] = iAPAXEPValue[6]       Arm2 Site3
wData2[13] = iAPAXEPValue[7]       Arm2 Site4
wData2[14] = iAPAXDualEPValue[6]   Arm2 Dual3
wData2[15] = iAPAXDualEPValue[7]   Arm2 Dual4
```

ShuttleMode 條件（照搬 V874）：
- ShuttleMode==1 && Shuttle_Sel==0 → 只更新 Arm1 (wData2[0..7])
- ShuttleMode==1 && Shuttle_Sel==1 → 只更新 Arm2 (wData2[8..15])
- ShuttleMode==0 → 同時更新 Arm1 + Arm2

`bDir==true` 時 Site1~4 強制 `wdata*16`（外缸開壓）。

## 5. INI Key 命名（與 V874 100% 相容）

每個 Kit 直徑（DieForceDiameter）會產生 8 個 INI group：

```ini
[DieForceDiameter_60.000mm_Arm1_1]
LoadRate=1.000
ContactOffset=0.000

[DieForceDiameter_60.000mm_Arm1_2]
[DieForceDiameter_60.000mm_Arm1_3]
[DieForceDiameter_60.000mm_Arm1_4]
[DieForceDiameter_60.000mm_Arm2_1]
[DieForceDiameter_60.000mm_Arm2_2]
[DieForceDiameter_60.000mm_Arm2_3]
[DieForceDiameter_60.000mm_Arm2_4]
```

SLKIndClass 同樣產生 8 group：

```ini
[Diameter_60.000mm_Arm1_1] ~ [Diameter_60.000mm_Arm2_4]
```

→ V874 客戶 INI 直接複製可用；V899 寫回 INI 也能丟回 V874 機台讀。**無需 migration script**。

## 6. 三維度錨點

### 6.1 條件錨點

```cpp
INSTALL_DOUBLE_EP == 3
```

涉及點（V899 中要保留 / 擴充的 16 處 ==2||==3 / ==1||==3 條件，已在前次盤點完成）。

### 6.2 資料錨點

| 容器 | size | 索引 |
|------|------|------|
| `DieForceOneByOneSLKClass` | `slDieForceOneByOneSLKType->Count * 8` | `i*8+j`, j=0..7 |
| `SLKIndClass` (==3) | `slSLKTypeInd->Count * 8` | `i*8+j`, j=0..7 |
| `iAPAXDualEPValue[8]` | 8 | k=0..7 |
| `iAPAXEPValue[8]` | 8 | k=0..7 |
| `wData2[16]` | 16 | 見 §4 mapping |

### 6.3 行為錨點

| 函式 | 行為 |
|------|------|
| `THTDieForceOneByOneSLKClass::THTDieForceOneByOneSLKClass` | `iCount=(Tag%8)+1`，`iCount<5` → Arm1，否則 Arm2 |
| `TfContactForce::FormCreate` | `j<8` push `i*8+j` 進 OneByOne 容器 |
| `TfContactForce::ReadFile` / `WriteFile` | OneByOne 與 SLKIndClass 都用 `i*8+j` 8 entry，sprintf 用 `_Arm1_%d`/`_Arm2_%d` |
| `TransformFuntion(==3, !bDualForce)` | 用 OneByOne 容器跑 j<8 迴圈，依 `dDiameter==fDiameter*10` 對應寫滿 `iAPAXDualEPValue[k]` k=0..7 |
| `APAX_WriteData(==3)` | 單次 `ADAMTCP_WriteReg(Address[2], 1, 1, 16, wData2)`，retry 10 次 |

## 7. 不變項（規格保證）

1. INI key 命名與 V874 完全相同（跨版相容）
2. Channel mapping 與 V874 完全相同（線路相容）
3. 不加 `iTestMode` 限制（硬體本來就支援所有測試模式）
4. `MultiTransferKG()` 不移植（V899 無 caller，死碼跳過）
5. `INSTALL_DOUBLE_EP == 1 / 2` 路徑完全不動（既有客戶不受影響）

## 8. 驗收標準

| # | 項目 | 方法 |
|---|------|------|
| AC-1 | 編譯通過 | BCB6 build OK，無 warning 增加 |
| AC-2 | parity check GREEN | `python scripts\check_porting_parity.py multi_ep_v874_to_v899` 全綠 |
| AC-3 | INI 相容 | V874 校正後的 ContactForce.ini 複製到 V899，開機讀值正確 |
| AC-4 | OneByOne UI 顯示 | 設定頁 8 個 GroupBox（Arm1_1~4 / Arm2_1~4）出現於 `tsDieForceOneByOneKit` |
| AC-5 | 寫入正確 ch | 用 ADAM debug 工具監看 reg 1~16，確認寫入值符合 §4 mapping |
| AC-6 | ShuttleMode 切換 | Mode=1 Sel=0 時 Arm2 ch 不變；Sel=1 時 Arm1 ch 不變 |

## 9. 風險

| 風險 | 緩解 |
|------|------|
| 之前 V899 半版若已實機校正，INI 用 i*4+j 命名，更新後讀回 Arm1_3/4/Arm2_3/4 是預設值 | 確認 SIGURD 機台尚未上線校正；若已校正，提供一次性 INI 轉換腳本 |
| 16ch 一次寫入 timeout 比 8ch 高 | 照搬 V874 retry 10 次機制 |
| OneByOne 容器 size 從 i*4 變 i*8，舊 V899 半版資料延用會 index out-of-range | 開機讀取在 ReadFile 重建容器時自動以 8-entry 重建，無延用問題 |

## 10. 變更歷史

| 日期 | 作者 | 內容 |
|------|------|------|
| 2026-05-04 | JimmyChiu | 撤回 V899 半版，落地完全照搬 V874.3 ==3 |

## 11. 參照

- ADR-0004: 撤回 Multi EP 砍半設計，採完整移植
- V874.3 來源：`HT9011UC_Code_V3.33.874.3_20250701_JerryYang4_Merge_ATC33_60_6/`
  - `ContactForce.cpp:298, 605, 1086, 1158, 1278, 1300`
  - `adam6024.cpp:1385, 2250, 2280`
- V899 目標：`HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422/`
  - `ContactForce.cpp:298, 605, 1086, 1278`
  - `adam6024.cpp:1494, 2068`
- 工具：`scripts/check_porting_parity.py` + `scripts/specs/multi_ep_v874_to_v899.yaml`
