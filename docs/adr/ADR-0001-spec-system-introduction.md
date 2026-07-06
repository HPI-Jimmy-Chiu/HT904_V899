# ADR-0001：導入 SPEC 體系作為 HT9045 功能契約

- **狀態**：Accepted
- **日期**：2026-05-02
- **作者**：JimmyChiu
- **影響範圍**：全專案（HT9045 / HT9046 跨版本）
- **關聯**：
  - SPEC：`docs/spec/README.md`
  - 工具：`scripts/spec_sync_from_merge.py`
  - Memory：`/memories/repo/spec-system.md`

## 1. 背景

HT9045 專案有以下特性導致「事後看不懂自己改什麼」：

- 多版本並存（V874 ~ V903 + 多開發者分支），跨版搬功能頻繁
- 合入他人版本時，沒有任何文件記錄行為差異
- Hangup 案例與根因分析散落在 `/memories/repo/` 與 `hangup/` 資料夾，無正式契約
- Release Note 每次都要從頭看 commit / //AI 註解組合
- BCB6 + Big5 + 400+ 原始檔，純靠 grep 與記憶維護不可持續

## 2. 考慮過的方案

### 方案 A：寫一份大型整機 SPEC
- 優點：完整覆蓋
- 缺點：500 頁等級、永遠寫不完、寫完即過時
- 成本：極高，且維護不可行

### 方案 B：每個 PR 寫設計文件
- 優點：細節完整
- 缺點：開發者疲勞，文件品質參差不齊，無查找標準
- 成本：中

### 方案 C：分片 SPEC（模組級 + 功能級）+ 工具自動偵測缺口
- 模組 SPEC：穩定行為，跨版本維護（少量、長壽）
- 功能 SPEC：per case，只增不刪（多量、可凍結）
- 工具 `spec_sync_from_merge.py` 在合版時自動列出缺口
- 優點：CP 值高、自動化偵測過時、與既有工具鏈整合
- 缺點：需要養成新的工作習慣
- 成本：低（一次性建置 + 每 case 增量補）

### 方案 D：不做（維持現狀）
- 缺點：技術債持續累積，新人/AI agent 無法快速上手

## 3. 決定

採用 **方案 C：分片 SPEC + 自動化工具**。

目錄結構：
```
docs/spec/
  README.md
  TEMPLATE_module.md / TEMPLATE_feature.md
  modules/<name>.spec.md     ← 模組級
  features/F<ver>-<NNN>-<name>.spec.md  ← 功能級
```

每份 SPEC 必填三維度錨點（條件 / 資料 / 行為），與 `multi-angle-thinking` 規則對齊。

## 4. 理由

- **與既有工具鏈共生**：`version_diff.py` + `check_ai_annotation.py` + 新增的 `spec_sync_from_merge.py` 形成閉環
- **與 multi-angle-thinking 規則對齊**：三維度錨點 = 條件 / 資料 / 行為
- **沉沒成本低**：不回頭補歷史 SPEC，從下一個 case 開始；模組 SPEC 一次寫一個高頻模組
- **AI agent 友善**：未來的 ht9045-v899 agent 可直接讀 SPEC，不必重新 grep

## 5. 後果

### 正面
- 跨版搬功能時間下降（用 SPEC 對比 vs 用 grep 比 code）
- Release Note 可半自動產生（從 SPEC 第 8 節 customer-facing 段落萃取）
- Hangup 根因分析有「期望行為基準線」可比對

### 負面 / 技術債
- 短期內歷史功能無 SPEC（只有新案）→ 接受不回頭補
- 需要使用者養成「寫 case 時順手建 SPEC」的習慣

### 後續觀察點
- 半年後 review：SPEC 數量、被引用次數、過時率
- 若 SPEC 過時率 > 30%，需檢討是否模板太重

## 6. 修訂紀錄

| 日期 | 變更 | 作者 |
|------|------|------|
| 2026-05-02 | 初版 | JimmyChiu |
