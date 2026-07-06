# ADR-0002：回退 cSiteUseManager 對 e9045_1x4_4 的強制 hardcode

- **狀態**：Accepted
- **日期**：2026-04-28
- **作者**：JimmyChiu
- **影響範圍**：HT9045 V899 之後所有版本
- **關聯**：
  - Hangup：`/memories/repo/hangup-20260428-csiteusemgr-1x4-hot.md`
  - 對應週報：CASE-20260428-001（瑞昱 PPLR685）
  - 對應交付版本：V3.33.899.12

## 1. 背景

V899 在 `cSiteUseManager::UseCompactSearch()` 對 `iInArmType == e9045_1x4_4` 強制回傳 true，無視客戶 INI 的 `[SiteUseMgr] bUseCompactSearch=0`。

實機問題：CompactSearch 用 1×1 起點掃描，與 `DoPlaceToHotPlate` 的 stride placement 邏輯不對齊（如 col 0,2,4,6），導致：
- `CompactSearchPlateToPlace()` 失敗
- 三處呼叫點直接 `DoHotPlateHangUp("CompactSearch_*")` 立即 alarm
- V898 走舊 `SearchPlateToPlace()` 為「等待/重試」語意，無此 alarm 路徑

## 2. 考慮過的方案

### 方案 A：移除 hardcode，回歸 INI 主導（最小變更）
- 優點：客戶 INI 已是 0，等同回退 V898 行為，立即可交付
- 缺點：CompactSearch 在 1x4_4 模式形同停用
- 成本：極低（5 行 code）

### 方案 B：重寫 CompactSearchPlateToPlace 為 stride-aware
- 優點：CompactSearch 在所有模式都能正常工作
- 缺點：開發 + 測試時間長，客戶停線中無法等
- 成本：高

### 方案 C：CompactSearch 失敗時 fallback 到舊 SearchPlateToPlace
- 優點：保留 CompactSearch 優點 + 安全網
- 缺點：兩套搜尋邏輯共存增加維護負擔
- 成本：中

## 3. 決定

採用 **方案 A：最小變更回退**。

`cSiteUseManager.cpp` L263-L274 移除 `iInArmType==e9045_1x4_4` 的強制 true 分支，回歸 INI 主導。

```cpp
//AI(JimmyChiu) 20260428: revert force-enable for e9045_1x4_4
```

## 4. 理由

- 客戶 PPLR685 停線中，需立即交付
- 客戶 INI 本來就是 0，回退後等同 V898 已驗證行為
- 方案 B 為更佳的長期解，但不可阻塞當下交付
- 方案 C 保留兩套邏輯，但 CompactSearch 在 stride placement 模式下從未被驗證安全

## 5. 後果

### 正面
- 客戶當日恢復運轉
- 回到 V898 已驗證行為，回歸風險低

### 負面 / 技術債
- CompactSearch 在 1x4_4 模式下實質失效
- 未來想啟用 CompactSearch 必須先做方案 B

### 後續觀察點
- 待辦：重寫 `CompactSearchPlateToPlace()` 為 stride-aware（用 `GetHotPlateColStep()` + `InArmSuck.iPickCol`）→ 預定為新 ADR
- QA：1x4_4 / 1x4_8_Hot / 1x2 / 2x4_16 四模式 6h 連續運轉

## 6. 修訂紀錄

| 日期 | 變更 | 作者 |
|------|------|------|
| 2026-04-28 | 初版（回顧記錄） | JimmyChiu |
