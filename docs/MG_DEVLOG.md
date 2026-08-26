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

## 20260826 16:45 — MG-W3 done（EventLog 引號相依鏈）

- 6 op：note.cpp FOREHOPE 無引號分支＋cObserver ParseEventLogLine 三支 static＋4 解析點。
- port_check PASS（15+59 全 SPLICED）＋bcc32 PASS。儀表 **368 → 361**。
- 裁決：解析端不加 gate 照搬（V899 已 12 客戶 64 台實測出貨）；JamRawData 數字變化
  ＝修正方向，記外溢。fVATMesFileSys 第 5 解析點＝V899 殘留缺口，開獨立任務卡。
- 矩陣校正：note.cpp:1027（0407）是假 MISSING（公司改寫等價），LEDGER 已記。

---

## 20260826 17:05 — MG-W4 done（BootLog＋LOAD_Y stepper）

- 7 op：main.cpp 5 埋點（cBootLog 本體已在 V910 基線免搬）＋HandlerSys OK 套用
  （含 [0] 補齊裁決）＋uMotorTest 測試列。16 條矩陣點。儀表 **361 → 345**。
- gate 升級：bcc_syntax.sh 改「錯誤集合與 .mgbak 基準一致＝零回歸」準則——
  main.cpp 在 -D_VER6 旗標組下有 3 個既有錯誤（非本波），比對基準後 PASS。
- 實測警訊：bcb.exe（IDE）開著——波內單檔編譯安全，Phase 3 全量 build 前必關（A13）。

---

## 20260826 傍晚 — MG-W5 done（HS_Function KYEC 上傳線）

- 9 op＋1 條 C（BVL-3766 假 MISSING 入白名單）。port_check +35 全 SPLICED/removed=0；
  bcc32 0 errors；目檢三要害通過。儀表 **345 → 335**。
- 裁決：跨午夜 swap 照 V899 終態搬（F2 記 method 1/2/3 語意疑慮，屬 V899 議題）。
- LEDGER 新增「假 MISSING 白名單」——收尾驗收準則＝MISSING 集合等於白名單。
- 排程約束：HS_Function 後續波（0504×5/0519×1/0526×1/0630×2）要用
  docs/mg_w5_analysis.md §4.1 位移表或現場重定位。

---

## 20260826 傍晚 — MG-W6 done（CC_CYUEAN AutoTrayFeed）

- 5 op；port_check +17 全 SPLICED/removed=1（死註解）；bcc32 雙檔 0 errors；
  bCleanOutCanTrayEnd 44=44 parity。儀表 **335 → 330**。
- 裁決：帶入未標記承重行（bCleanOutCanTrayEnd=true，矩陣盲區第二例，與 W4 [0] 同型）。
- CYUEAN 行為變更兩項記 LEDGER；TfConfiguration 第二顆 chkAutoTrayFeed=V899 既有缺口不動。

---

## 20260826 晚 — MG-W7 done（Multi EP 主題波，地形重大發現）

- **V910 有公司自製 Multi EP 完整實作**（Eastsun 20260525 整合＋RogerYang 8EP，部分比
  V899 新）→ 53 條裡真搬 6 條（iosetview AV 防護 5＋ContactForce round-trip 1），
  47 條入白名單（32 C／8 X-none 不可刪／7 條 F3–F6 皆建議不搬並照建議執行）。
- F1「搬不搬 8ch」被地形解決＝無需裁決；矩陣升級白名單感知
  （docs/mg_matrix_allowlist.csv，49 條）。儀表：raw 330→324、**淨 MISSING=275**。
- 反向發現 3 筆（V899 有問題不可回搬）；ContactForce iCount<3 已開 V899 任務卡。

---

## 20260826 晚 — MG-W8 done（Power Save 線）

- 16 op／5 檔／170 行（含 116 行無標記新碼——矩陣盲區同型第三例）。
- gate 全綠；PowerSavingMode.h 與 V899 位元組同。淨 MISSING **275 → 240**。
- 新規則入 skill：commit 後清 V910 樹 *.mgbak（port_check/bcc 基準逐波刷新）。

---

## 20260826 晚 — MG-W9 done（InArm watchdog 線）

- 18 條全真缺口（本戰役唯一「矩陣說缺就是真缺」波）、11 op／4 檔、74 行全 SPLICED。
- 歸屬更正：0602=欣銓、0803=全智（收 CC_ARDENTEC 閘）。F7/F8/F9 入決策清單。
- 淨 MISSING **240 → 222**。

---

## 20260826 夜 — MG-W10 done（FTP 大波，本戰役最大單波）

- 2 新增檔（FTPUpload 目錄整檔複製）＋16 op／6 檔＋.bpr 6 處登錄；58 條矩陣點。
- 執行緒 15 接點齊備；OP12 無閘裁決照 V899 現狀（四項實測支撐）。
- gate 全 PASS（新增檔 0 錯誤、uLotInfo 零回歸）。淨 MISSING **222 → 164**。
- bcc_syntax INC 補 FTPUpload。

---

## 20260826 19:55 — MG-W11 done ＋ 額度中斷實錄

- **額度中斷自癒實戰**：17:5x 撞額度（W12 分析 agent 死亡、主迴圈凍結），19:50 重置、
  19:51 心跳/喚醒佇列送達自動續跑——協議如設計運作，全程無人介入。
- MG-W11：16 op（OP6b 92% 留 F10 未搬）＋2 條假 MISSING 入白名單。
  PickPlanner 目錄早在 V910 基線（孤兒標頭由本波補活）；AutoClean.cpp 100% LF 保持。
  gate 全綠。淨 MISSING **164 → 137**。
- **W12 死前殘留**：docs/mg_w12_ops.json（17:58 落地）但無分析文件、無模擬驗證
  ——按「重派勿找復原」重派，草稿只當線索不當依據。
- **同分支並行警訊**：另一 session（V906 FW 戰役）在額度期間推了 commit 且有在製 M 檔
  （HT9011UC_Cpp 樹＋weekly 資產）——MG commit 一律點名檔案，絕不寬 glob。

---

## 20260826 20:00–22:50 — W12–W16 連五波，Phase 2 完成（淨 MISSING=0）

- W12（OutArm rotate 33 條/32 op，56a9815）→ W13（hotair＋PTI 尾波 17 條，3860bee）
  → W14（PTI 0630/0703 案群 42 條/34 op，75bd43f）→ W16（雜項掃尾 20 條，b70f987）
  → W15（機能群 20 條，596f64f）。
- **終局帳**：719＝643 落地＋3 他檔＋73 白名單列管＋**0 缺**。16 波、~1,100 行
  位元組級搬移、2 新增檔、.bpr 6 處、F1–F12（唯一待裁補搬項＝F10）。
- 亮點：A5 破案（uhome 亂碼=V899 原檔天生損壞，V910 有正確版）；
  歸屬更正三筆（0602 欣銓、0703 半數超豐、0625 欣銓）；
  盲區實測（//AI 可見率 3.4%–29%，無標記承重行每波必有）。
- **Phase 3 開跑**：IDE 已關（22:44 實測）；bpr2mak→.mak 改導 Obj910/Out910
  （共用 Obj/EXE 零觸碰；PROJECT 絕對路徑 D:\HT9045\EXE 那筆也抓到）；
  make 背景執行，log=樹內 build_mg_phase3.log。

---

## 🔖 RESUME（20260826 夜／Phase 3）

- **狀態**：Phase 2 done（16 波、淨 MISSING=0）。**Phase 3 全量 build 背景執行中**
  （V910 樹內 `make -f HT9045.mak`，輸出 Obj910/Out910；
  若被 10 分鐘工具限制切斷→同指令重跑即增量續跑）。
- **下一步**：
  1. 收 build：link 成功→Out910\HT9045.exe＝技術收尾；錯誤→逐條歸因
     （缺 lib/res/PCH 屬環境問題，程式錯誤才算回歸）。
  2. SOFT_SIMULTE 確認（MachineType.h 兩樹皆註解態，已知）；版號提醒
     （.bpr Version Info 只有 IDE 會寫）。
  3. 收尾報告（RD5軟體_MG戰役總結_YYYYMMDD_HHMMSS）＋F 清單提交使用者（F10 為主）。
- **不變量**：V910 建置一律 Obj910/Out910（共用 Obj/EXE 零觸碰）；V899 唯讀；
  LEDGER＋白名單＝驗收權威；矩陣儀表隨時可重跑。
