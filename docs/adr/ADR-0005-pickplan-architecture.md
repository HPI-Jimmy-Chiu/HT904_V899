# ADR-0005：Arm Pick/Place 通用 Planner 架構

- **狀態**：Accepted
- **日期**：2026-05-14
- **作者**：JimmyChiu
- **影響範圍**：HT9045 V899 — InArm / OutArm / AutoClean 取放料排程
- **關聯**：
  - 程式碼：`Common/PickPlanner/IArmPickPlanContext.h`、`cArmPickPlan.{h,cpp}`、`cInArmLoaderPickPlanner.{h,cpp}`、`cOutArmPlacePlanner.{h,cpp}`
  - Adapter 案例：`AutoClean/cCleanKitPickPlan.h`（Phase 2 重構為 adapter）
  - Memory：`/memories/repo/v899-progress.md`

## 1. 背景 (Context)

HT9045 各模組各自實作「窮舉 lead 位置 → 對每組吸嘴算實際座標 → 檢查可達性 / 目標需求 → 挑 active 數最多者」的 pick/place 排程：

- `AutoClean::cCleanKitPickPlan`：Clean Kit 取/放
- `ainarm9045::SearchLoadTrayUpDown_9045` 系列：InArm 從 Loader Tray 取料
- `aoutarm9045::SearchTrayToPlace_9045` 系列：OutArm 放至 Auto Tray
- `ainarm_SearchPlacePlate.cpp`：HotPlate 放料

這些函式核心邏輯**結構相同**但實作高度耦合各自模組的 tray 資料結構、sucker mapping 與 motor limit。每次新增需求（多吸嘴、新 layout、FIFO 模式）都要修 N 個地方且容易漏改，已造成多次 hangup。

## 2. 考慮過的方案 (Options)

### 方案 A：保持現狀，每個模組獨立優化
- 優點：零風險、零 build 影響
- 缺點：規律重複、bug 易擴散、新人學習成本高、無共用測試
- 成本：長期維護成本持續累積

### 方案 B：模板化（policy class + traits）
- 優點：編譯期解析、零 vtable 開銷
- 缺點：BCB6 不支援 C++11，模板診斷糟糕；違反 [ADR-0003](ADR-0003-bcb6-no-cpp11-policy.md)
- 成本：高，且踩 BCB6 雷

### 方案 C：純虛介面 + 通用引擎 + 各模組 adapter
- 優點：BCB6 native 支援；演算法集中一處；adapter 模式清楚分工；可逐 region pilot
- 缺點：vtable 一次性間接呼叫成本（非 hot path，可忽略）
- 成本：中，但 Phase 1-2 已驗證可行

## 3. 決定 (Decision)

採用 **方案 C**，分階段落地，每階段 default OFF + 通過 BCB6 build verify：

1. **Phase 0**：修現有 `bUseCKPP` 註解（zero behavior change）
2. **Phase 1**：建立 `IArmPickPlanContext` + `cArmPickPlan`（dead code，無 caller）
3. **Phase 2**：重構 CKPP 為 `IArmPickPlanContext` adapter，public API 保持不變
4. **Phase 3**：`cInArmLoaderPickPlanner` scaffold + shadow-only dual-path hook 進 `SearchAndMoveInArmXYToLoad_9045`
5. **Phase 4**：`cOutArmPlacePlanner` scaffold（暫不 wiring，保留下次 pilot）
6. **Phase 3.1 / 4.1（未排程）**：填入真實 region 邏輯，shadow log 與 legacy 結果比對；確認無差異後才允許 `IsAuthoritative()=true`

## 4. 理由 (Rationale)

- **BCB6 相容**：純虛介面 + non-template engine 是 BCB6 / pre-C++11 的天然解
- **逐 region pilot**：每個 region 一個獨立 flag（`bUseCKPP`、`bUseLoaderPickPlanner`、未來 `bUseOutArmPlacePlanner`），可獨立啟停
- **Shadow-only 中介態**：`IsAuthoritative()` 預設 false，即使 dev 環境誤開 flag，planner 結果也不會驅動實際動作
- **既有 API 不變**：CKPP 重構後 8 個 call site 完全未動，證明 adapter 模式可平滑切換

## 5. 後果 (Consequences)

### 正面
- 演算法集中於 `cArmPickPlan::Search()`，未來優化（剪枝、scoring 改進）只動一處
- 新 region pilot 成本下降：建 adapter + scaffold 即可，不必複製演算法
- 提供「shadow log diff」機制可在量產靜默驗證新邏輯
- 三層安全網：(a) `bUse*Planner=false` 預設、(b) stub `GetLogicalPickerCount()=0` 讓 engine 立即 bail、(c) `IsAuthoritative()=false` 阻止 short-circuit

### 負面 / 技術債
- 每個 region 都有 dual-path 期，程式碼一度膨脹（Phase 3-4 期間）
- `IArmPickPlanContext` 加新方法時要更新所有 adapter（目前 CKPP / Loader / OutArm 三個）
- 若未來新增 region 不照 adapter 模式走會稀釋價值

### 後續觀察點
- Phase 3.1 完成後若 shadow log 與 legacy 連續一週無差異，可考慮翻 `IsAuthoritative()=true` 並把 `bUseLoaderPickPlanner` 預設改 true
- 若 Phase 4.1 OutArm 落地後三個 adapter 仍有大量重複碼，考慮抽出 `cTrayBackedPlanner` 中介層
- 若任何 adapter 為了塞語意而扭曲 `IArmPickPlanContext`，視為架構訊號，需重新評估介面

## 6. 修訂紀錄

| 日期 | 變更 | 作者 |
|------|------|------|
| 2026-05-14 | 初版（Phase 0-4 決策歸檔） | JimmyChiu |
