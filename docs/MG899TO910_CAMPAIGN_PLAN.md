# MG 戰役計畫 — V899 → V910 功能搬移

- 戰役代號：**MG**（MiGrate 899→910），波次編號 MG-W1、MG-W2…
- 建立：20260826，依使用者十點指示制定
- 狀態：**已核可**（使用者 20260826 14:30 裁定），另加四項裁定：
  1. 殭屍處置＝**先查證後處置**（見 §4 確認程序），確認無運作才能收殮
  2. 決策規則＝20260826 17:00 前交付完整問題清單；過程中 **≥95% 信心依建議執行**，
     <95% 記入「最終決策清單」留使用者，**不停下**
  3. V910 pristine 基線 commit：`e06524a`；V899 在製收斂 commit：`b515ed5`（皆已執行）
  4. `.dfm` 案**直接完成**，不留使用者 review（仍遵守不重排、文字級套用）

---

## 0. 兩棵樹與寫入邊界

| 樹 | 角色 | 狀態 |
|---|---|---|
| `HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422` | **來源**（量產機出貨版） | git 基線 `bb69c60`（20260422 pristine）+ 逐案 commit；目前仍有未 commit 在製變更 |
| `HT9011UC_Code_V3.33.910.0_20260716_Jimmy` | **目標**（公司最新版） | 目前 **untracked**；已含 642 處 //AI 註解＝公司已合併部分修正，且混有其他開發線（heater-control、contact-force、config、secs 等） |

- 戰役期間 V899 視為**唯讀來源**（量產！不回改）；其他 `HT9011UC_Code_*`／`HT9046*` 一律唯讀。
- `system/`、`config/`、`CFG/`、`EXE/` 不動、不覆寫。
- 開跑前更新寫入邊界：`CLAUDE.md` 路徑範圍節 + `.github/ops/write-boundary-policy.json` 把 V910 樹加入可寫清單，註記「使用者 20260826 授權」。

## 1. 範圍權威與已知地形（20260826 偵察）

範圍 = **weekly 紀錄中「在 899 樹上有程式變更」的案件**。純分析案（無碼變更）不搬。

三層證據 join（行層以 git 為權威，不只靠註解）：
1. Weekly_AI：`weekly_data.json` + `Customer/*/CASE-*/issue.md`（≥60 案提及 899，此數觸到掃描截斷，真數 Phase 0 產出）
2. `git diff bb69c60..HEAD` + working tree（**行層權威**，涵蓋沒掛 AI 註解的手改）
3. //AI 註解做歸屬與定位（V899 期 20260430–20260820 約 546 處、97+ 檔）

已知地形（20260826 位元組級矩陣，`tools/port_tools/ai_comment_matrix.py` 實測）：
- V899 期（20260422–20260820）//AI 註解共 **617 處**：**277 處**公司已合併（same-file 命中）、
  3 處他檔命中、**337 處 MISSING**＝搬移工作量的註解粒度上限。
- 公司合併截止 ≈ **20260608**，之前有洞（0423:19、0430:21、0504:17、0602:13 等），
  **20260609 之後幾乎全缺**。
- 缺口集中檔案：main.cpp(27)、uFtpUploadThread.cpp/h(31)、CosFunction.cpp/h(25)、
  ainarm9045.cpp(15)、ContactForce.cpp(15)、uLotInfo.cpp(14)、PowerSavingMode.cpp/h(16)、
  acatchtray.cpp(12)、adam6024.cpp(12)。明細：`docs/mg_ai_matrix_missing.csv`。
- V910 另有 ~100+ 處**非 899 線**的註解（heater-control、contact-force、config 等公司/其他線）
  → splice 時是錨點漂移源，且**絕不可覆蓋**。
- V899 樹 git 歸屬骨幹：pristine `bb69c60` → 22 個案件 commit → `b515ed5`；
  20260422–0630 的早期工作壓在 `292bd72` 累積 commit，歸屬靠 //AI 日期 × weekly 案件。
- ⚠ V910 樹**行尾混合**：git add 時 ~34 檔警告 LF（core.autocrlf 環境）→ 全程逐檔保持原樣、
  絕不對這些路徑 git checkout；port_check 以磁碟現狀為準。

## 2. 案件分類法（每案一類，記入 LEDGER）

| 類 | 定義 | 動作 |
|---|---|---|
| **A 乾淨可搬** | 錨點未漂移；行為對所有客戶等價，或 899 原本就有客戶碼 gate | 波次自動搬 |
| **B 需隔離** | 會影響其他客戶，或與 V910 演進後的邏輯相violated | 用該案客戶的 `CUSTOMER_CODE`／`FUNC_CC_*` 包住行為變更點，**預設路徑＝V910 原行為**；隔離理由入 LEDGER |
| **C V910 已有** | 公司已合併（等價實作也算） | 跳過；LEDGER 記證據（檔:行） |
| **D 安全關鍵** | 運動控制／IO 互鎖／警報行為／模式切換的行為變更 | **不自動搬**，佇列累積等使用者逐案裁決 |
| **E 純分析** | 無程式變更 | 不搬 |

B 類細則：gate 只包**行為變更點**，不包宣告／資料結構；同一旗標群沿用 899 的 gate 寫法。

## 3. 階段

### Phase 0 — 基線與盤點（主腦 + opus 盤點 agent）
1. ✅ **V899 在製工作先收乾淨**：`b515ed5`（20260817-0820 三案累積變更）。
2. ✅ **V910 pristine 基線 commit**：`e06524a`（846 檔原樣入庫）。
3. ✅ 註解存在性矩陣：617 處 → 337 MISSING（`docs/mg_ai_matrix_*.csv`）。
4. 產出 **`docs/MG_PORT_LEDGER.md`（總帳）**：每案一列 — CASE-ID／客戶／客戶碼／日期／涉及檔案／hunk 數／V910 現況（已有｜缺｜漂移）／分類 A–E／隔離策略／狀態。
5. **20260826 17:00 前交付完整問題清單**（使用者裁定取代原「審核後才開跑」）：
   清單交付即開跑 Phase 1/2，不等回覆；問題項照 §決策規則處理。

### Phase 1 — 戰役 harness（一次性，與 Phase 0 平行）
- `mg-wave-loop` skill + `/mg-wave` command，骨架照 pt/fw 已驗證政策複用：冷啟動協議、額度中斷實測表、「不准閒著」三規則。
- `tools/port_tools/`（Python，全程 cp950）：
  - `extract_hunks.py` — 從 V899 以位元組級抽出變更區塊（git diff + //AI 錨）
  - `splice.py` — 以錨點插入 V910，**絕不重打字 → 對齊天然保留**
  - `port_check.py` — 每波 gate：(a) cp950 可解碼、無 U+FFFD (b) EOL 逐檔不變 (c) 插入區塊與 899 原文**位元組級一致**（＝對齊檢查的機械保證）(d) 插入處縮排與周圍一致性報告、tab/space 混用警示
- `bcc32 -c -H-` 單檔語法 gate 腳本（隔離編譯，避開共用 Obj／IDE 衝突）。

### Phase 2 — 波次 loop（主體，夜間可自動）
每波 = 一個案（同日同主題可併波）：
1. 主腦從 LEDGER 選下一個 A/B 類案
2. **opus** agent：讀 899 該案 diff → 在 V910 找錨點 → 判讀漂移 → 產搬移方案（hunk 清單、錨點、是否需 gate）
3. **sonnet** agent 或主腦直跑腳本：splice + port_check
4. **主腦逐條複驗**（開檔對字面——agent 論證比程式碼更常錯）
5. Gate：port_check 全綠 + 觸及檔 bcc32 -c 全過
6. AI 註解：`//AI(mg899to910) YYYYMMDD: 描述 [CASE-ID]`
7. commit（訊息帶 CASE-ID）→ LEDGER 勾銷 → DEVLOG + 🔖RESUME
8. **立即下一波，不准閒著結束回合**

**決策規則（使用者 20260826 裁定）**：
- 信心 **≥95%** → 依建議直接執行並在 LEDGER 記錄決策與理由。
- 信心 **<95%**（錨點找不到、漂移過大、邏輯衝突無明顯安全解）→ 記入
  `docs/MG_FINAL_DECISIONS.md`（最終決策清單）留使用者，**跳下一案不停**。
- **D 類落地細則**：忠實搬運（899 已量產驗證的原樣變更）且錨點乾淨 → 視同 ≥95% 可執行；
  有漂移或需改寫 → 一律 <95% 進最終決策清單。
- 同檔多案按日期順序搬；依賴記 LEDGER。
- 含 `.dfm` 的案（如 PowerSaveMode 底色）：dfm 屬性以文字級精準套用、不重排，
  **直接完成不留 review**（使用者 20260826 裁定）。

### Phase 3 — 整合驗收
1. 全量 build：`bpr2mak + make`（bcb_build skill）；前置確認無 BCB IDE 開著、共用 `D:\HT9045\Obj` 先 clean。
2. 出貨三坑檢查：SOFT_SIMULTE 關閉、版號只有 IDE 會寫、建出的 EXE 另存含戰役標記檔名。
3. LEDGER 對帳：每個「已搬」案在 V910 grep 到位；公司 900–910 期間自身變更未被覆蓋（diff 審）。
4. 總結報告 `RD5軟體_MG戰役總結_YYYYMMDD_HHMMSS`。

## 4. Loop／額度／殭屍防護（使用者指示 5、7）

- `/loop /mg-wave` 動態節奏推進。
- 雙保險（pt-wave 20260809-10 實測背書）：
  - 每輪武裝 `ScheduleWakeup`（有東西在跑時的保險；額度中斷會斷鏈）
  - **CronCreate 60 分鐘守衛心跳**（使用者指定 1 小時）：檢查有無進度停滯；
    **額度中斷期間該次心跳失敗、下一次照排程自動回來** ← 真正的復原路徑
- **殭屍判定與處置（使用者 20260826 裁定：先查證後處置，不准直接刪）**：
  心跳發現 60 分鐘無進度時，走確認程序，**全部**項目確認無活動才能宣告死亡：
  1. 背景任務清單有無該任務仍在列
  2. OS 進程檢查（`tasklist` 查 bcc32／make／python）——build 不經 API，額度中斷時仍在跑
  3. 產出檔／日誌 **mtime** 是否還在前進（檔案存在 ≠ 在跑，看 mtime）
  4. `git status` 對帳——agent 常常已寫完檔才死，殺前先收成果
  任一項有活動 → **不動它**，記錄觀察，下一輪心跳再看。
  確認死亡 → 先收殮（把已落地的工作驗證後收進 commit／LEDGER）→ 照 🔖RESUME 重派。
- 額度中斷期間：背景 OS 進程（build）照跑不停；agent 死用 `resumeFromRunId` 回放。
- **前提：session 不關、機器不重開**（cron／喚醒都是 session-scoped）。session 死＝磁碟上的 commit + LEDGER + RESUME 就是全部狀態，重開後 `/loop /mg-wave` 冷啟動協議（git log → git status → LEDGER → RESUME）無損接續。
- 每波收工＝**先 commit 再結束回合**。

## 5. 模型政策（使用者指示 9 + 20260826 補充）

| 角色 | 模型 | 用途 |
|---|---|---|
| 主腦（本 session） | **Fable 5** | 選案、逐條複驗、gate 裁決、commit、LEDGER/RESUME、心跳應答 |
| 分析 agent | **Opus 5**（subagent 上限，**禁派 Fable**） | 漂移判讀、搬移方案、衝突/隔離分析 |
| 機械 agent | **Sonnet 5** | splice 執行、port_check 批跑、文件整理 |

- **effort 不設限**（使用者 20260826 補充）：opus 用 high/xhigh、sonnet 用 high。
- 老規矩全套用：agent 論證比程式碼更常錯 → 主腦逐條開檔複驗；mutation agent 不與主腦併行改同檔（snapshot+md5 對帳）；absence-claim 附指令與時間、收工前重跑。

## 6. 硬邊界（永不觸碰）

- V899 與其他版本樹唯讀；`EXE/` 永不覆寫。
- Big5：兩棵樹原始碼**禁用 Edit/Write 工具直改**，一律 Python cp950；每波 port_check 驗編碼。
- 絕不 `git checkout` 復原自己的編輯（用自備備份 + md5 對帳）。
- D 類安全關鍵不自動搬。
- `.svn` 不搜尋不修改；`.dfm` 不手動重排。

## 7. 主要風險與對策

| 風險 | 對策 |
|---|---|
| 錨點漂移（V910 混入其他開發線 + 公司自改） | opus 每案判讀；找不到錨點→needs-user 佇列，不硬套 |
| 公司合併「有洞」非單純日期截止 | 以 git diff 為權威逐案驗 V910 現況，不用日期線推定 C 類 |
| 覆蓋公司 900–910 自身變更 | splice 只插入不取代；取代型 hunk 必經主腦複驗 + Phase 3 diff 審 |
| 對齊跑掉（使用者指示 8） | splice 位元組級搬運 + port_check (c)(d) 機械驗證，波波必跑 |
| 量產樹在製變更未收 | Phase 0.1 先 commit |
| 共用 Obj/EXE 建置互撞 | 波內只用 bcc32 -c 隔離編譯；全量 build 只在 Phase 3 且先查 IDE |

## 8. 估算

- 規模：30–50 案、≥220 註解點、觸及 ~100 檔（真數以 LEDGER 為準）
- Phase 0+1：約一個晚上；Phase 2：視額度 2–5 個晚上；Phase 3：半天
- 波次交付節奏：每案一 commit，隨時可中止、隨時可審
