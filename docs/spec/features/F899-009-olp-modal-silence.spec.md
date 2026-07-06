# F899-009 — OLP 路徑 modal dialog 靜默化（**已取消**）

| 欄位 | 值 |
|------|-----|
| Feature ID | F899-009 |
| 狀態 | **cancelled**（取代方案內嵌於 F899-008）|
| 取消日期 | 2026-05-04 |
| 取消原因 | scope 限縮為「僅超豐」後不需全域改動 |

## 取消說明

原 F899-009 規劃在 OLP 路徑下全域抑制 `ShowMyMessage / ShowErrorMessage / ShowMyMessageBox_YES_NO`，做法是新增 `bInOLPCallStack` 旗標 + 改寫 500+ 呼叫點的 dialog 函式。

由使用者於 2026-05-04 明確指定設計約束：

> 修改範圍僅限超豐，Automation 模組由其他客戶共用。

此規則使「全域改寫 modal」方案不可行（會影響所有 OLP 客戶行為）。

### 取代方案

modal 靜默化需求改在 [F899-008 §6.3](./F899-008-remote-start-lot-replacement.spec.md) 內以**函式級隔離**達成：

- 新增 `TfProductionInfo::OEE_StartLot_Silent(Data, errMsg)` 函式
- 函式內部不呼叫任何 modal，改回傳 `errMsg` 字串
- 既有 `OEE_StartLot()` 一行未改 → 手動操作路徑與其他客戶完全不受影響

若未來其他客戶提出類似需求，再個案開新 SPEC，不再走全域改寫。
