# 功能 SPEC：F899-002 cSiteUseManager 對 e9045_1x4_4 強制 hardcode 回退

## 1. Metadata

| 欄位 | 內容 |
|------|------|
| Feature ID | F899-002 |
| 標題 | 回退 cSiteUseManager::UseCompactSearch() 對 e9045_1x4_4 的 hardcode 強制 ON |
| 客戶 / 來源 | 瑞昱 PPLR685 現場 hangup |
| 對應 CASE-ID | CASE-20260428-001 |
| 來源版本 | V899（bug 引入） |
| 目標版本 | V3.33.899.12（修復出貨版本） |
| 影響模組 | SiteUseManager / InArm / HotPlate |
| 安全等級 | 安全關鍵（hangup 阻斷生產） |
| 作者 | JimmyChiu |
| 日期 | 2026-04-28 |

## 2. 問題 / 需求

V899 在 `cSiteUseManager::UseCompactSearch()` 對 `iInArmType == e9045_1x4_4` 強制回傳 true，
無視客戶 INI `[SiteUseMgr] bUseCompactSearch=0`。

實機問題：
- CompactSearch 用 1x1 起點掃描，與 `DoPlaceToHotPlate` 的 stride placement（如 col 0,2,4,6）邏輯不對齊
- `CompactSearchPlateToPlace()` 失敗時三處呼叫點直接 `DoHotPlateHangUp("CompactSearch_*")` 立即 alarm
- V898 走舊 `SearchPlateToPlace()` 為「等待/重試」語意，無此 alarm 路徑

## 3. 三維度錨點

### 3.1 條件錨點

| 條件 | 來源檔案 |
|------|---------|
| `iInArmType == e9045_1x4_4` | `cmydef.h` |
| `[SiteUseMgr] bUseCompactSearch` INI 設定 | `Gerneral.ini` 或 `SiteUseMgr.ini` |
| `cSiteUseManager::UseCompactSearch()` 回傳值 | `cSiteUseManager.cpp` L263-L274 |

### 3.2 資料錨點

| 變數 / 設定 | 讀 / 寫 | 來源檔 |
|------------|---------|--------|
| `bUseCompactSearch` | R | `cSiteUseManager.h` |
| `PickHPRec.HPSuckTeam[].PlateCol[]` | R | hangup PickHPRec.json 證據 |
| `iPickCol` (`InArmSuck`) | R | nozzle stride |

### 3.3 行為錨點

| 動作 | 函式 | 副作用 |
|------|------|--------|
| 判斷是否啟用 CompactSearch | `cSiteUseManager::UseCompactSearch()` | 影響後續搜尋路徑分派 |
| CompactSearch 失敗時觸發 alarm | `DoHotPlateHangUp("CompactSearch_*")` | 立即 hangup（阻斷生產） |
| 回退路徑 | `SearchPlateToPlace()` | 等待/重試語意 |

## 4. 設計決策

採用最小變更方案：移除 `iInArmType == e9045_1x4_4` 強制 true 分支，回歸 INI 主導。
完整決策理由見 `docs/adr/ADR-0002-revert-csiteusemgr-hardcode.md`。

## 5. 實作清單

- [x] 修改 `cSiteUseManager.cpp` L263-L274 移除 hardcode
- [x] //AI 註解：`//AI(JimmyChiu) 20260428: revert force-enable for e9045_1x4_4`
- [x] 編譯 V3.33.899.12 安裝包
- [x] 客戶交付驗證

## 6. 驗證方式

- [x] BCB6 build V3.33.899.12 通過
- [x] SOFT_SIMULTE 模擬重現 hangup（PickHPRec.json `PlateCol=[0,-1,3,-1,...]`）
- [x] 客戶 PPLR685 實機驗證：恢復運轉
- [ ] 待辦：1x4_4 / 1x4_8_Hot / 1x2 / 2x4_16 四模式 6h 連續運轉

## 7. 風險 / 副作用

- 不影響其他 InArmType（hardcode 只針對 e9045_1x4_4）
- CompactSearch 在 1x4_4 模式形同停用（待方案 B 重寫 stride-aware 版本）
- 客戶 INI 已是 0，回退後等同 V898 已驗證行為，回歸風險低

## 8. Release Note 摘要

> **【V899.12】1x4 加熱模式 hangup 修正**
> 修正 1x4 加熱模式（`e9045_1x4_4`）下，新版搜尋演算法與實際取放料 stride 不對齊，
> 導致 IC 放至 HotPlate 後立即觸發 hangup 的問題。本版恢復由 INI `bUseCompactSearch` 控制，
> 客戶可依設定維持原有的搜尋行為。

## 9. 關聯資料

- 模組 SPEC：`docs/spec/modules/inarm.spec.md`
- ADR：`docs/adr/ADR-0002-revert-csiteusemgr-hardcode.md`
- Hangup 紀錄：`hangup/2026-04-28 15_45_51/`
- Memory 筆記：`/memories/repo/hangup-20260428-csiteusemgr-1x4-hot.md`
- Weekly Case：`Customer/瑞昱/CASE-20260428-001_1x4_HeatMode_Hangup_cSiteUseManager/`
