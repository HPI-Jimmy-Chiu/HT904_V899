# HT9045 ADR (Architecture Decision Record)

> 記錄「**為什麼這樣設計**」的決策日誌。半年後自己也看得懂。

## ADR vs SPEC 差異

| | SPEC | ADR |
|---|------|-----|
| 紀錄什麼 | **是什麼**（行為契約） | **為什麼**（決策理由） |
| 變動性 | 活的（隨版本演進） | 不可變（決策當下凍結） |
| 範例 | 「InArm 在 16-site 配置下會做 DataSwap」 | 「為什麼選『先 SearchPlate 後 SwapData』兩階段，而非單階段 atomic」 |
| 觸發時機 | 每個功能 | 重大設計選擇、否決方案、回退決定 |

## 何時要寫 ADR

- 在 ≥2 個方案中選了一個（A 而非 B）
- 否決某個直覺方案（解釋為什麼不做）
- 回退某個既有設計（如 V899 把 `cSiteUseManager` 強制開啟改回 INI 主導）
- 定義跨模組約定（如「所有 InArm 派生類別都用 `iInArmType` 分派」）
- 引入或淘汰技術元件（如 EtherCAT 取代某 PCI 卡）

## 不需要寫 ADR

- 一般 bug 修復（寫 feature SPEC 即可）
- UI 文字 / log 改動
- 變數重命名

## 命名規則

`docs/adr/ADR-NNNN-<簡短標題>.md`，NNNN 全專案連號（不跟版本綁），例如：

- `ADR-0001-spec-system-introduction.md`
- `ADR-0002-revert-csiteusemgr-hardcode.md`
- `ADR-0003-bcb6-no-cpp11-policy.md`

## 狀態標記

每份 ADR 必有狀態：

- **Proposed**：提案中（討論未定）
- **Accepted**：已採用（生效中）
- **Superseded by ADR-NNNN**：被新 ADR 取代
- **Rejected**：曾經評估但決定不做（仍保留紀錄供未來參考）
- **Deprecated**：歷史決策，已不適用

## 與 SPEC / 工具鏈關係

- ADR 編號可被 SPEC 引用（feature SPEC 第 4 節「設計決策」可寫 `見 ADR-0002`）
- ADR 變更需更新狀態，不刪除舊內容（用 strikethrough 或新增「修訂紀錄」段）
