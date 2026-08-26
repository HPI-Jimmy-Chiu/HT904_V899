# MG_DEVLOG — V899→V910 功能搬移戰役

格式：每段收工附 commit、波次、量測。檔尾 🔖RESUME 是冷啟動唯一起點。

---

## 20260826 下午 — Phase 0/1：基線、盤點、harness

- 使用者核可計畫＋四裁定（殭屍先查證後處置／95% 決策規則＋17:00 問題清單／
  V910 入庫／V899 先收／.dfm 直接完成）。effort 不限；subagent 上限 Opus 5。
- `b515ed5` V899 在製收斂（20260817-0820 三案）；`e06524a` V910 pristine 基線（846 檔）。
- 矩陣實測：V899 期 //AI 617 處 → V910 已合併 277、他檔 3、**MISSING 337**；
  公司合併截止 ≈20260608 且有洞（0423/0430/0504/0602…），0609 後幾乎全缺。
- harness：`tools/port_tools/`（ai_comment_matrix／mg_ledger_skeleton／extract_hunks／
  splice／port_check，全 cp950 位元組級）＋ `/mg-wave` ＋ `mg-wave-loop` skill
  （Copilot 鏡像已同步）＋寫入邊界開通 V910。
- 文件：MG899TO910_CAMPAIGN_PLAN（含裁定）／MG_PORT_LEDGER（34 叢集骨架）／
  MG_PROBLEMS_20260826（17:00 交付）。
- weekly 案件盤點 agent（opus）背景執行中 → 產 `docs/mg_inventory_weekly_cases.md`。

---

## 🔖 RESUME（20260826）

- **狀態**：Phase 0 大致完成（缺 weekly join）；Phase 1 harness 就緒未實戰。
- **下一步**：
  1. 併入 weekly 盤點 → LEDGER 補案件歸屬 → 17:00 前交付問題清單給使用者。
  2. 啟動 Phase 2：`/loop /mg-wave`，第一波建議 MG-W1＝小而乾淨的叢集
     （20260423 Greatek AutoClean default 60→1，4 條同檔）驗證整條 splice→port_check→
     bcc32 流水線，再上大叢集（Multi EP 0429–0512 合併波）。
  3. 掛 60 分鐘守衛 cron（殭屍協議＝先查證後處置，見 skill）。
- **不變量**：V899 唯讀（bb69c60..b515ed5 是來源端點）；V910 基線 e06524a；
  bcc32 -c 波內 gate；全量 build 只在 Phase 3。
