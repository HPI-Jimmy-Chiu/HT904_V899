# HT9045 SPEC 體系

> SPEC = **活的功能契約（Living Contract）**，不是一次寫完的設計文件。
> 目的：跨版移植、合入他人功能、Hangup 根因分析、Release Note 素材。

## 目錄結構

```
docs/spec/
  README.md                 ← 本檔（規範總覽）
  TEMPLATE_module.md        ← 模組 SPEC 模板
  TEMPLATE_feature.md       ← 功能 SPEC 模板
  modules/                  ← 模組級 SPEC（穩定行為，跨版本維護）
    inarm.spec.md
    outarm.spec.md
    autoclean.spec.md
    ...
  features/                 ← 功能級 SPEC（per case / per PR，只增不刪）
    F899-001-hotplate-dataswap.spec.md
    F899-002-multi-ep-write.spec.md
    ...
```

## 何時要寫 SPEC（CP 值表）

| 變更類型 | 是否寫 | 寫哪份 |
|---------|--------|--------|
| 新增 mode / enum / 機種配置 | 必寫 | 功能 SPEC + 更新模組 SPEC |
| 跨版本移植 | 必寫 | 功能 SPEC + 更新模組 SPEC |
| 合入他人模組級新功能 | 必寫（事後補也要補） | 功能 SPEC |
| 合入他人小修（< 50 行單檔） | 更新模組 SPEC 一段 | 模組 SPEC patch |
| 安全關鍵修改（運動 / IO / 互鎖） | 必寫 | 功能 SPEC |
| Bug 修復（根因明確） | 加入原 SPEC 「Known Issues」 | 不新增 |
| UI 文字 / log / 註解 / 變數重命名 | 不寫 | — |

## 三維度錨點（必填）

每份 SPEC 都要有以下三組「錨點」，對應 `multi-angle-thinking` 規則：

- **條件錨點 (Condition)**：`==N`、`!=N`、flag 旗標、判斷式
- **資料錨點 (Data)**：陣列、變數、IP、register、座標、設定檔欄位
- **行為錨點 (Behavior)**：實際呼叫的 API、執行的動作、產出的封包、IO/馬達動作

## 命名規則

- 模組 SPEC：`modules/<module>.spec.md`，名稱小寫
- 功能 SPEC：`features/F<版本號>-<流水號>-<簡短名>.spec.md`
  - 範例：`F899-001-hotplate-dataswap.spec.md`
  - 流水號每個版本內遞增

## 與工具鏈的關係

- `scripts/version_diff.py` — 比對兩版函式差異
- `scripts/check_ai_annotation.py` — 檢查 //AI 註解
- `scripts/spec_sync_from_merge.py` — **合入他人版本時**自動列出 SPEC 缺口（草稿）
- 模組 SPEC 段落同時是 Release Note 的素材來源

## 維護者責任

1. 開新 case → 同步建一份功能 SPEC（可先用 skeleton，事後補三維度）
2. 合入他人版本 → 跑 `spec_sync_from_merge.py` → 補完草稿
3. 月度 review → 確認模組 SPEC 與最新版本程式碼仍同步

## 模組 SPEC 索引

| 模組 | 檔案 | 摘要 |
|------|------|------|
| InArm | `modules/inarm.spec.md` | 入料手臂取放、HotPlate、Shuttle、AutoClean |
| Automation OLP | `modules/automation-olp.spec.md` | 對外 TCP 控制介面、_INQUIRE / _REQUEST 分派 |

## 功能 SPEC 索引

| ID | 標題 | 狀態 |
|----|------|------|
| F899-001 | HotPlate Data Swap | done |
| F899-002 | cSiteUseManager revert | done |
| F899-003 | MAIN_STATUS_INQUIRE 修正 | done |
| F899-004 | AutoClean 延遲預設 1s | done |
| F899-005 | Monitor 第四攝影機紀錄 | done |
| F899-006 | EventLog 永信格式 | done |
| F899-007 | 超豐 RunSite 資訊改寫 | done |
| F899-008 | 新增 OEEStartLot_REQUEST 指令（**Greatek only**） | analysis（待客戶確認 4 項） |
| F899-009 | OLP 路徑 modal dialog 靜默化 | **cancelled**（取代方案併入 F899-008）|
