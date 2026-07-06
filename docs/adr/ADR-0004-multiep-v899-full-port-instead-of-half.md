# ADR-0004: V899 Multi EP 採完整移植 V874.3，撤回砍半設計

## 狀態

Accepted (2026-05-04)

## 背景

V899 早段為 SIGURD 客戶導入 `INSTALL_DOUBLE_EP==3`（Multi EP 多獨立壓力源）功能時，初次實作採「砍半版」假設：
- 假設硬體是 2 顆 ADAM-6024，各只用 ch0~3
- ContactForce OneByOne 容器使用 `i*4+j`（j<4，每直徑 4 entry）
- INI key 命名為 `_Arm1_1/2`、`_Arm2_1/2`（每直徑 4 group）
- APAX_WriteData 拆兩段寫入：`(reg=1, count=8)` + `(reg=33, count=8)`

實機交付前確認硬體規格時發現：
- 機台僅 1 顆 APAX-5070 (Address[2] = 172.16.8.112) + 2 顆 APAX-5028
- 線路設計是 16ch 連續寫入，非兩塊獨立板卡
- V874.3 (Ifor 原始版) 是單次 `ADAMTCP_WriteReg(Address[2], 1, 1, 16, wData2)`
- 半版假設與實機線路不符

## 決策

**撤回砍半版，完全照搬 V874.3 ==3 行為**：

1. ContactForce 容器改為 `i*8+j`（j<8，每直徑 8 entry）
2. INI key 採用 V874 命名 `_Arm1_1/2/3/4`、`_Arm2_1/2/3/4`（每直徑 8 group）
3. APAX_WriteData ==3 改為單次 `ADAMTCP_WriteReg(Address[2], 1, 1, 16, wData2)`
4. SLKIndClass ==3 分支也一併補齊（V899 原本只走 ==2 路徑，i*16+j）
5. 不加 `iTestMode` 限制
6. `MultiTransferKG()` 不移植（V899 無 caller）

## 替代方案

### A. 維持砍半版

- ? 改動少
- ? 與實機線路不符，2x4 模式直接壞掉
- ? INI 與 V874 不相容，客戶換版要重校
- ? 未來客戶升級需再改一次

### B. 完全照搬 V874（採用）

- ? 與實機線路相符
- ? INI 與 V874 100% 相容（客戶 INI 可直接搬）
- ? 升 2x4 不需再改
- ? `check_porting_parity.py` 可機械驗證對等性
- ? 改動較大（10 處），含 SLKIndClass 三段新增

## 後果

### 正面

- 跨版本 INI 相容性建立（V874 ? V899 機台 INI 可互換）
- 為未來新客戶 / 新配置奠定 16ch 完整基礎
- 機械化驗證工具 (`check_porting_parity.py`) 可長期維護對等性

### 負面

- 若 SIGURD 已用 V899 半版實機校正過，INI 升版後 `Arm1_3/4 / Arm2_3/4` 會用預設值
  - 緩解：實裝前確認尚未上線校正；若已校正提供轉換腳本
- 程式碼改動量增加（半版回退 + V874 完整路徑搬入）

## 相關文件

- SPEC: `docs/spec/features/SPEC-V899-MultiEP-FullPort.md`
- 工具: `scripts/check_porting_parity.py` + `scripts/specs/multi_ep_v874_to_v899.yaml`
- 來源版本: `HT9011UC_Code_V3.33.874.3_20250701_JerryYang4_Merge_ATC33_60_6/`
- 目標版本: `HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422/`

## 紀錄

| 日期 | 作者 | 動作 |
|------|------|------|
| 2026-05-04 | JimmyChiu | 撰寫並 Accepted |
