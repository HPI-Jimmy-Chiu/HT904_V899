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

## 20260826 15:50 — Phase 0 完成＋MG-W1 試點實戰

- weekly 盤點回收：88 案→45 有碼案；兩方向性發現（母體改矩陣、五相依鏈）已採納。
- 矩陣 DATE_MIN 修正 20260422→20260323（weekly 交叉驗證抓到漏）：
  真數 **719 處/376 缺席/43 叢集**。矩陣重跑＝進度儀表（搬完自動消失）。
- **MG-W1 done**：automation.cpp MAIN_STATUS_INQUIRE 9 行 splice
  （port_check PASS＋bcc32 PASS）；uCleaning 4 條裁決 skipped-C（V910 動態預設已等價）。
- port_check 修正：old 基準優先 .mgbak，git-show fallback 重建 CRLF（autocrlf 陷阱）。
- 17:00 問題清單已交付（MG_PROBLEMS_20260826.md）；F1 Multi EP 進 MG_FINAL_DECISIONS。

---

## 20260826 16:20 — MG-W2 done（MyLaneIo 售服訊息）

- 5 op（3 replace 呼叫點＋GetIOErrStr 整段＋.h 宣告）；port_check PASS（88+4 全 SPLICED、
  removed 34 全數人工複驗＝舊格式與 MachineTypeChoice 三分支）；bcc32 PASS。
- 儀表：**MISSING 376 → 368**。外溢（MNetLog 多行化）記 LEDGER。

---

## 🔖 RESUME（20260826 16:20）

- **狀態**：Phase 2 連續波次進行中。MG-W1、MG-W2 done。儀表 MISSING=368。
- **下一步（按序）**：
  1. MG-W3＝EventLog 引號線（0429 note.cpp → 0817 cObserver.cpp 同波，相依鏈#1；
     甬矽案 CASE-20260429-001＋CASE-FOREHOPE_NINGBO-20260813-001；
     注意 EventLog CSV 引號是承重的——CommaText 解析，改格式要回驗 Observer）。
  2. MG-W4 候選＝20260414/0415/0420 HS_Function KYEC 上傳線（同主題連波）。
  3. 之後照 LEDGER 由小到大，主題波優先（AutoClean CKPP＋PickPlanner 目錄、
     Power Save 線 0804→0811→0819/0820、Multi EP、0612 FTP 大波）。
  4. 60 分鐘守衛 cron 已掛（每時 :23；殭屍＝先查證後處置）。
- **量測**：`python tools/port_tools/ai_comment_matrix.py`（MISSING 單調下降；
  起點 376、現 368）。
- **不變量**：V899 唯讀（來源端點 b515ed5）；V910 基線 e06524a；LEDGER 手工維護；
  bcc32 -c 波內 gate；全量 build 只在 Phase 3。
