---
description: "Use when: 跨版本搬功能、檢查 //AI 註解規範、build 前掃除以零風險、合入他人版本同步 SPEC、SPEC 與程式碼漂移偵測、產 release note 草稿、hangup 與 SPEC/ADR 交叉參照、寫 ADR 設計決策、CASE 與 SPEC 雙向綁定、客戶 SPEC 覆蓋率盤點。Agent 收到對應提問時應主動呼叫對應腳本。"
applyTo: "**"
---

# HT9045 自動化檢查工具觸發規則

> 八個 Python 腳本位於 `scripts/`，同時提供 VS Code task 與直接 CLI 入口。
> Agent 接到下列關鍵字時應**主動執行對應腳本**，不要只用 grep 手工比對。

## 工具一覽

| 腳本 | 用途 | Agent 觸發關鍵字 | 輸入 |
|------|------|------------------|------|
| [scripts/version_diff.py](../../scripts/version_diff.py) | 比對兩個版本資料夾的函式級差異 | 「V897 跟 V899 差在哪」「跨版搬移」「移植」「版本差異」「version diff」 | `V???` `V???` |
| [scripts/check_ai_annotation.py](../../scripts/check_ai_annotation.py) | 檢查 `//AI(AgentName) YYYYMMDD: 描述` 格式 | 「檢查 //AI 註解」「release note 前掃描」「補 //AI」 | （預設 V899 全資料夾） |
| [scripts/scan_div_zero.py](../../scripts/scan_div_zero.py) | 快速掃除以零風險（search-division 的 Phase 1） | 「除以零」「divide by zero」「除法掃描」「build 前自檢」 | 無 |
| [scripts/spec_sync_from_merge.py](../../scripts/spec_sync_from_merge.py) | 合入他人版本時偵測 SPEC 缺口 | 「合入 V897」「合入 Roger 版本」「同步 SPEC」「SPEC 缺口」 | `--from V??? --to V???` |
| [scripts/spec_drift_check.py](../../scripts/spec_drift_check.py) | 偵測 SPEC 與實際程式碼的偏差（失蹤函式 / 檔案 / 變數） | 「SPEC 過時」「SPEC drift」「SPEC 與 code 不一致」「review SPEC」 | `--version V899` |
| [scripts/spec_to_release_note.py](../../scripts/spec_to_release_note.py) | 從功能 SPEC 第 8 節萃取 release note 草稿（三版分層） | 「產 release note」「V899 release note 草稿」「客戶版說明」 | `--version 899` |
| [scripts/sync_case_spec.py](../../scripts/sync_case_spec.py) | 把 weekly_data.json 的 case 與 docs/spec/, docs/adr/ 雙向綁定，並產證據鏈健康報告 | 「CASE 對應 SPEC」「客戶 SPEC 覆蓋率」「done case 缺 SPEC」「證據鏈報告」「綁定 case」 | `--write` 寫回 / `--out` 健康報告 |
| [scripts/link_hangup_to_spec.py](../../scripts/link_hangup_to_spec.py) | 建立 hangup / SPEC / ADR 交叉參照表 | 「hangup 對應 SPEC」「孤兒 hangup」「SPEC 覆蓋率」「ADR 對應 case」 | 無 |

## 三種觸發模式並存

1. **使用者手動跑**：terminal 直接打 `python scripts/<工具>.py`。
2. **VS Code Task**：部分工具掛了 task（version_diff / check_ai_annotation / scan_div_zero）。
3. **Agent 自動執行**：使用者提問命中關鍵字時，agent 應主動呼叫對應腳本，再依輸出回答。

## Agent 執行範例

### 範例 1：跨版搬功能
> 「我要把 V897 InArm 的某段邏輯搬到 V899，先列出兩版差異」

```
python scripts/version_diff.py V897 V899 --module InArm
```

### 範例 2：產 release note 前
> 「準備出 V899 的 release note」

第一步檢查 //AI 註解：
```
python scripts/check_ai_annotation.py
```
第二步從 SPEC 萃取 customer-facing 描述：
```
python scripts/spec_to_release_note.py --version 899 --out docs/release_notes/V899_draft.md
```

### 範例 3：build 前自檢
> 「V899 改完了，準備 build」

```
python scripts/scan_div_zero.py
```

### 範例 4：合入他人版本後同步 SPEC
> 「我把 RogerYang V897 的 InArm 改動合入 V899 了」

```
python scripts/spec_sync_from_merge.py --from V897 --to V899 --module InArm --out docs/spec/_gap_report.md
```

### 範例 5：定期 SPEC review（每月或每 release 前）
> 「幫我看哪些 SPEC 已經過時」

```
python scripts/spec_drift_check.py --version V899 --out docs/spec/_drift_report.md
```
→ 報告會列出每份 SPEC 的「失蹤函式 / 失蹤檔案 / 失蹤變數」，據此更新 SPEC。

### 範例 6：hangup 案例追蹤
> 「我們有哪些 hangup 還沒有對應 SPEC？」

```
python scripts/link_hangup_to_spec.py --out docs/spec/_hangup_crossref.md
```
→ 報告會列出每個 hangup 資料夾對應的 SPEC / ADR，以及「孤兒 hangup」。

### 範例 7：寫 ADR 記錄重大決策
> 「為什麼 V899 要把 cSiteUseManager 改回 INI 主導？」「我們為什麼還在用 BCB6？」

→ 不是執行腳本，而是建議使用者建立 ADR：
```
複製 docs/adr/TEMPLATE.md → docs/adr/ADR-NNNN-<簡短標題>.md
```
參考既有：
- `docs/adr/ADR-0001-spec-system-introduction.md`（導入 SPEC 體系）
- `docs/adr/ADR-0002-revert-csiteusemgr-hardcode.md`（回退 hardcode）
- `docs/adr/ADR-0003-bcb6-no-cpp11-policy.md`（BCB6 限制）

### 範例 8：CASE 與 SPEC 雙向綁定 / 客戶覆蓋率盤點
> 「我每週要看哪些客戶 case 還沒寫 SPEC」「幫我把週報 case 與 SPEC 串起來」

```
python scripts/sync_case_spec.py --write --out docs/spec/_case_health.md
```
- `--write` 會把 spec_refs / adr_refs / case_id 寫回 weekly_data.json（自動 backup）
- 健康報告含：done 但無 SPEC 的 case 清單、SPEC 引用 CASE-ID 是否存在、客戶 SPEC 覆蓋率
- 跨 workspace：預設讀 `D:/Work-jimmychiu/document/WeeklyReport/Weekly_AI/weekly_data.json`，可用 `--weekly` 指定
- 若報告顯示「CASE-ID 衝突」段非空，先在 Weekly_AI workspace 跑 `python tools/dedupe_case_ids.py --write` 重新發 ID，再回來跑 sync_case_spec --write

## 與既有 skill 的關係

- `scan_div_zero.py` = `search-division` skill 的快速版（只做 Phase 1）
- `version_diff.py` 取代手動 grep 跨版比對
- `check_ai_annotation.py` 是 release note 前置檢查
- `spec_sync_from_merge.py` / `spec_drift_check.py` / `spec_to_release_note.py` / `link_hangup_to_spec.py` 是 SPEC 體系（`docs/spec/`）的維護工具鏈
- ADR 是「為什麼」的決策日誌，與 SPEC 互補
- `sync_case_spec.py` 是 Weekly_AI 與 HT9045 的橋樑：把週報 case 與 SPEC/ADR 雙向串起來

## 文件體系連結

### SPEC 體系（`docs/spec/`）
- `docs/spec/README.md` — 總覽 + CP 值表
- `docs/spec/TEMPLATE_module.md` / `TEMPLATE_feature.md` — 模板
- `docs/spec/modules/` — 模組級 SPEC（穩定行為）
- `docs/spec/features/` — 功能級 SPEC（per case，只增不刪）

### ADR 體系（`docs/adr/`）
- `docs/adr/README.md` — ADR 規範與何時要寫
- `docs/adr/TEMPLATE.md` — 模板
- `docs/adr/ADR-NNNN-*.md` — 連號決策紀錄

### Release Notes
- `docs/release_notes/V???_draft.md` — 由 `spec_to_release_note.py` 產出

## 編碼注意

腳本檔頭依語言宣告編碼：
- 純 ASCII / 英文 docstring 的 .py：`# -*- coding: cp950 -*-`
- 含特殊符號（如箭頭）的 .py：`# -*- coding: utf-8 -*-`

HT9045 workspace 的 `create_file` 會把中文寫成 BIG5。
**新增工具或 SPEC/ADR markdown 要透過 Weekly_AI workspace 用 Python `encoding="utf-8"` 寫入。**

## 推薦執行順序（month-end / release 前）

1. `check_ai_annotation.py` — 確保所有變更有 //AI 註解
2. `spec_drift_check.py` — 確保 SPEC 與目前 code 同步
3. `link_hangup_to_spec.py` — 確保所有 hangup 有對應 SPEC/ADR
4. `spec_to_release_note.py --version XXX` — 產 release note 草稿
5. `sync_case_spec.py --write` — 把 SPEC/ADR 同步回週報 case
6. 人工 review 草稿 → 交付
