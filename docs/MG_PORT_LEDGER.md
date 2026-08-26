# MG_PORT_LEDGER — V899→V910 搬移總帳

- **母體原則（20260826 定案）**：總帳母體＝`//AI` 註解全集（矩陣），weekly 案件只當
  歸因與客戶碼線索——weekly 盤點證實樹內大量變更無對應 case（孤兒變更），照 case 搬會漏。
- 資料源：docs/mg_ai_matrix_missing.csv（**376 條 MISSING**，DATE_MIN=20260323 修正後）
  ＋ docs/mg_inventory_weekly_cases.md（45 個有碼案）＋ git log bb69c60..b515ed5
- 進度儀表＝重跑 `python tools/port_tools/ai_comment_matrix.py`：搬完的條目自動從 MISSING 消失。
- 狀態值：pending / in-wave / done(commit) / skipped-C / queued-decision / excluded-E
- 分類：A 乾淨搬｜B 客戶碼隔離｜C V910已有｜D 安全關鍵(忠實+錨點乾淨→可執行,否則進最終決策清單)｜E 純分析
- 本表自 20260826 起**手工維護**，不再重跑 skeleton 產生器（會蓋掉狀態欄）。

## 已完成波次

| 波 | 內容 | 結果 | commit |
|---|---|---|---|
| MG-W1 | 20260422 automation.cpp MAIN_STATUS_INQUIRE（9 行插入）＋ 20260423 uCleaning 4 條裁決 | splice+port_check PASS+bcc32 PASS；uCleaning 4 條=skipped-C（V910 已演進成 ArmSpeed 動態預設，原案「別用 60 秒荒謬預設」意圖已滿足，不搬） | b71780d |
| MG-W2 | 20260424 MyLaneIo.cpp/.h 售服 IO 錯誤訊息（8 條矩陣點、5 op：3 個呼叫點 replace＋GetIOErrStr 中文化整段＋.h 宣告） | port_check PASS（cpp added=88 全 SPLICED/removed=34、h added=4）＋bcc32 PASS；驗證 V910 取代區與 896 基準位元組相同、公司漂移（Safe PLC 區）在取代區外不受影響；無客戶碼隔離需求。**外溢記錄**：GetIOErrStr 回傳變多行 Big5，V910 三個未動呼叫點（IOBitOn:126/IOBitOff:193/IOByteOut:260）的 MNetLog 日誌從單行變多行——與 V899 出貨行為一致（忠實搬運），客戶端若逐行 parse MNetLog*.txt 需知悉 | 0bee467 |
| MG-W3 | EventLog 引號相依鏈（0429 note.cpp 寫入端 CC_FOREHOPE_NINGBO 無引號分支 2 條＋0817 cObserver ParseEventLogLine 三支 static＋4 解析點，共 7 條矩陣點、6 op） | port_check PASS（15+59 全 SPLICED、removed=2+4 逐條複驗＝else 兩行與 4 個 CommaText 點）＋bcc32 PASS。寫入端 gate=執行期 CUSTOMER_CODE==790，非 790 客戶 else 主體位元組不變（實測）；解析端不加 gate 照搬（V899 已 12 客戶 64 台 100,220 列實測出貨）。**外溢記錄**：JamRawData FTP 統計為修正方向的數字變化。V899 樹內 .bak_20260817_csvquote diff＝5 hunk 與變更集完全吻合 | 570ae0f |
| MG-W4 | BootLog 開機診斷（main.cpp 5 埋點 59 行；cBootLog.cpp/h 已在 V910 基線內免搬）＋LOAD_Y stepper（HandlerSys OK 套用 6 行含 [0] 補齊＋uMotorTest 測試列 5 行），16 條矩陣點、7 op | port_check PASS（59+6+5 全 SPLICED、removed=5 逐條複驗＝uMotorTest 舊 false 列）＋bcc32 PASS（main.cpp 3 個既有錯誤與 .mgbak 基準一致＝零回歸，見 gate 準則）。裁決：OP-A4 用 after 保留 V910 RogerYang AV 去重改寫；OP-B1 含未標記的 [0] Loader 行（整段 apply block V910 從未有過，補齊避免不對稱）。無客戶碼隔離需求（機台配置閘控） | 9c7b8e1 |
| MG-W5 | HS_Function KYEC 上傳線（0414 trace 診斷 6 點＋0415 前置宣告與跨午夜換昨天＋0420 假 MISSING 判 C；11 條矩陣點、9 op） | port_check PASS（+35 全 SPLICED、removed=0＝V910 零損失）＋bcc32 PASS（0 errors）。目檢三要害：20260630 timeout 30000 未動、Eastsun CC_KYEC_LEE 區塊完整、connect-fail else 到位。OP9 補 connect-fail 回傳碼（全樹無消費者，可觀察面零）。跨午夜 swap 行為外溢記 F2。**閘門真相：非 KYEC 專屬**（bN10* ini 旗標閘控，函式名是歷史命名） | 59952cc |
| MG-W6 | 0817 CC_CYUEAN 固定關閉 Auto Tray Feed（CosFunction.h 成員＋預設 false＋FUNC_CC_CYUEAN 4 行含未標記承重行＋cTrayAssignment 生效點與 UI 鎖；5 條矩陣點＋1 未標記行、5 op） | port_check PASS（+17 全 SPLICED、removed=1＝V910 死註解行）＋bcc32 雙檔 0 errors；bCleanOutCanTrayEnd 兩樹同口徑 44=44 parity。**B 類，gate=FUNC_CC_CYUEAN(868) 僅 CYUEAN 執行**；KYEC_LEE(921) AMR 漂移互斥已證。**CYUEAN 行為變更記兩項**：(A) bAutoFeed 恆 false (B) bCleanOutCanTrayEnd false→true（開 MES1642 TrayEnd 選單家族 7 讀取點）——皆 V899 出貨現狀。附帶：TfConfiguration 第二顆 chkAutoTrayFeed 可繞過鎖定＝V899 自身既有缺口（兩樹位元組同，不動） | 26ec286 |
| MG-W7 | Multi EP 主題波（0430/0504/0511/0526 共 53 條）——**地形重大發現：V910 有公司自製 Multi EP 完整實作（Eastsun 20260525 整合＋RogerYang 8EP），部分比 V899 新**。真搬 6 條（iosetview 0511 AV 防護 5 op＋ContactForce round-trip 1 op）；32 條 C 類、8 條刪除記錄不可執行（會砍掉公司 8EP 活功能）、7 條入 F3–F6（皆建議不搬並已照建議）；47 條入矩陣白名單 | port_check PASS（15+2 全 SPLICED、removed=4 逐條複驗）＋bcc32 雙檔 0 errors。反向發現 3 筆（V899 有問題 V910 沒有，不可回搬）：ContactForce iCount<3 殘留（已開 V899 任務卡）、MultiTransferKG int vs double、ReadMultiEP 未啟用 | 484123a |
| MG-W8 | Power Save 線（0804 C05 profile 25 條＋0811 UI 高亮 9 條＋0817:363 還原 guard；16 op／5 檔／170 行，含 116 行無標記新碼——矩陣盲區同型第三例） | port_check PASS（157 行全 SPLICED）＋bcc32 三檔 0 errors。PowerSavingMode.h 套用後與 V899 位元組完全相同。**B 類：gate=FUNC_CC_PTI＋bPowerSaveShowCaption（預設 false）**，非 PTI 全路徑不變；GetPowerSaveMaxMinute 非 PTI 回傳 200=原硬編碼。**PTI 行為變更記錄：IniConfig.bPowerSaveFunction=true 是 V910 上全新開關**（與 W6 bCleanOutCanTrayEnd 同型）。碰撞區 cConfiguration [C05]：PTI 分支插前、既有 if 降級 else if，V910 分支本體零位元組變動（USE_BU5_Function 實測死旗標，200/400 分岔無實機差異）。.dfm 驗證免動（gbC05/labC05_* 兩樹 94 行位元組同） | e5589ee |
| MG-W9 | InArm watchdog 線（0602 欣銓 13 條＋0803 全智收閘 5 條；18 條全真缺口、11 op／4 檔）——歸屬更正：0602=欣銓 CASE-20260602-001、0803=全智 CASE-GIGAS-20260729-001，「欣銓要的功能害全智誤報→收回 CC_ARDENTEC(870)」 | port_check PASS（74 行全 SPLICED、removed=1=改道 Task=148 的舊 Task=100）＋bcc32 三檔 0 errors。**B 類：gate=bUseInArmLoadStageWatchdog 僅 FUNC_CC_ARDENTEC 設 true**，非欣銓機台兩使用點恆走 disarm，V910 行為零變更。弱錨 OP10 靠同檔降序＋all-or-nothing＋md5 前提防護，落點目檢正確。F7（V910 ARDENTEC 空殼致 case 148 對欣銓不生效）/F8（acatchtray 閘外、四重閘量化）/F9（掛鐘缺陷隨行）入決策清單。反向發現 fAGV NULL 防呆不倒回 | c428a0f |
| MG-W10 | FTP 大波（0609 KYECFTP 4＋0611 3＋0612 50＋0630:1952 保險路徑；58 條、**2 新增檔**＋16 op／6 檔＋.bpr 6 處） | **V910 完全沒有 FTPUpload 背景上傳執行緒**（uFtpUploadThread.cpp/h 整檔位元組級複製＋md5 對帳）。執行緒 15 接點齊備、不用 Synchronize（結果佇列避 WaitFor 死結）。port_check PASS（216 行全 SPLICED、removed=14 逐條複驗＝移進 else 的同步碼＋非阻塞化取代）＋bcc32 六檔 PASS（新增檔 0 錯誤；uLotInfo 9 個既有錯誤與基準一致零回歸）。開關 bFtpUploadBackground 預設=(CUSTOMER_CODE==CC_PTI)；0609 四處 CC_PTI gated。**OP12（Fix 下半盤 Direction/iTrayType 鏡像）無客戶碼閘＝V899 出貨現狀**：四項實測（enum 零重疊/無第二寫入者/與既有鏡像同構/工作檔旗標閘控）。無標記承重行 127 行全數帶入（矩陣可見率僅 3.4%） | （本次收工 commit） |

## 假 MISSING 白名單

**權威清單＝`docs/mg_matrix_allowlist.csv`**（矩陣自動讀取，命中者計 ALLOWLISTED 不計 MISSING；
20260826 起 49 條：MG-W7 的 47 條 C/X 類＋下表兩條先期發現）。
收尾驗收準則＝**淨 MISSING 歸零**（即 MISSING 集合⊆白名單）。

| 先期發現 | 真相 |
|---|---|
| note.cpp:1027（20260407 SoftStop guard） | V910:1036 公司已改寫等價（自家註解 `//Jimmychiu 20260410`），簽章不同故不命中 |
| HS_Function.cpp:26（20260420 BVL-3766） | 功能早在 V910（Command.cpp/ProductionInfo.cpp 三條位元組相同），僅 #include 行尾註解被公司改寫成英文 |

## 相依鏈（weekly 盤點 20260826，必須同波搬，防把回歸搬進 V910）

1. **EventLog 引號線**：20260429 note.cpp（拿掉引號）→ 20260817 cObserver.cpp
   （ParseEventLogLine 欄位錯位修正）——同波，順序 0429 先。
2. **InArm watchdog 線**：20260602 ainarm9045.cpp（無閘門版）→ 20260803
   CC_ARDENTEC 收回閘門——同波，直接搬最終態（閘門版）。
3. **Multi EP 主題波**：0430＋0504＋0511＋0526（含 rolled-back 中間態）——以最終態搬，
   另見 MG_FINAL_DECISIONS（欣銓結案決議「保留 ch16」）。
4. **AutoClean CKPP 主題波**：0407/0408/0409/0416/0417（cCleanKitPickPlan 新舊雙路）
   ＋ Common\PickPlanner 整目錄（V910 無此目錄，屬新增檔搬移）。
5. **Power Save 線**：0804 C05 profile → 0811 PowerSaveMode 底色/字型 → 0819-0820
   倒數字串修正——按日序同主題連波。

| 叢集(日期) | 註解數 | 檔數 | comment 內 CASE/SPEC 線索 | 主要檔案 | 分類初判 | 狀態 |
|---|---|---|---|---|---|---|
| 20260401 | 1 | 1 | — | SECSGEM\uHGemHT9045_SV.cpp(1) | pending | pending |
| 20260407 | 11 | 3 | — | AutoClean\AutoClean.cpp(8), ainarm9045_1x1_1.cpp(2), note.cpp(1) | pending | pending（⚠note.cpp:1027 是假 MISSING：V910:1036 公司已改寫等價 SoftStop guard，該波直接判 C） |
| 20260408 | 6 | 1 | — | AutoClean\AutoClean.cpp(6) | pending | pending |
| 20260409 | 1 | 1 | — | AutoClean\AutoClean.cpp(1) | pending | pending |
| 20260410 | 1 | 1 | — | CosFunction.cpp(1) | pending | pending |
| 20260414 | 10 | 3 | — | HS_Function.cpp(7), AutoClean\uCleaning.cpp(2), main.cpp(1) | A | **HS_Function 7 條 done MG-W5**；uCleaning 2＋main 1 歸各主題波 pending |
| 20260415 | 5 | 2 | — | HS_Function.cpp(3), cShowBinSelect.cpp(2) | A | **HS_Function 3 條 done MG-W5**；cShowBinSelect 2 歸 UI 主題波 pending |
| 20260416 | 1 | 1 | — | AutoClean\AutoClean.cpp(1) | pending | pending |
| 20260417 | 3 | 2 | — | AutoClean\AutoClean.cpp(2), cShowBinSelect.cpp(1) | pending | pending |
| 20260420 | 1 | 1 | — | HS_Function.cpp(1) | C | **skipped-C MG-W5**（BVL-3766 假 MISSING，入白名單） |
| 20260423 | 19 | 4 | CASE-20260423-001 | HandlerSys.cpp(5), main.cpp(5), uMotorTest.cpp(5), AutoClean\uCleaning.cpp(4) | A/C | **done**：main+HandlerSys+uMotorTest 15 條=MG-W4；uCleaning 4 條=skipped-C（MG-W1） |
| 20260424 | 8 | 2 | — | MyLaneIo.cpp(6), MyLaneIo.h(2) | A | **done MG-W2** |
| 20260429 | 3 | 2 | CASE-20260429-001 | note.cpp(2), main.cpp(1) | A | **done**：note.cpp 2 條=MG-W3；main.cpp:10743=MG-W4（BootLog 24V 提示 gate） |
| 20260430 | 21 | 10 | — | adam6024.cpp(6), ContactForce.cpp(4), AutoClean\uCleaning.cpp(3), HandlerSys.cpp(2) +6檔 | C/X | **resolved MG-W7**：1 條留 F6，其餘 C/X-none 入白名單 |
| 20260504 | 17 | 6 | ADR-0004; SPEC-V899-MultiEP-FullPort | ContactForce.cpp(8), HS_Function.cpp(5), HS_Function.h(1), adam6024.h(1) +2檔 | C | **resolved MG-W7**：全數 V910 已有（含更新版），入白名單 |
| 20260505 | 2 | 2 | — | adam6024.cpp(1), uhome.cpp(1) | pending | pending |
| 20260511 | 8 | 2 | — | iosetview.cpp(5), adam6024.cpp(3) | A/X | **done MG-W7**：iosetview 5 條已搬（AV 防護）；adam6024 3 條 F4 不動 |
| 20260513 | 3 | 3 | — | acatchtray.cpp(1), asendic_Auto.cpp(1), csystem.cpp(1) | pending | pending |
| 20260514 | 8 | 2 | — | AutoClean\AutoClean.cpp(6), ainarm9045.cpp(2) | pending | pending |
| 20260515 | 3 | 1 | — | cContactCT.cpp(3) | pending | pending |
| 20260519 | 4 | 3 | — | uTrayEditForm.cpp(2), BarCode\BarCode.cpp(1), HS_Function.cpp(1) | pending | pending |
| 20260520 | 1 | 1 | — | CosFunction.cpp(1) | pending | pending |
| 20260525 | 2 | 1 | — | main.cpp(2) | pending | pending |
| 20260526 | 7 | 4 | — | ContactForce.cpp(3), adam6024.cpp(2), HS_Function.cpp(1), cContact.cpp(1) | A/X | **resolved MG-W7**：ContactForce round-trip 1 條已搬；1627/1670=F3、453=F5 不搬 |
| 20260602 | 13 | 2 | — | ainarm9045.cpp(10), acatchtray.cpp(3) | B | **done MG-W9**（watchdog，CC_ARDENTEC 閘） |
| 20260605 | 1 | 1 | — | cSortCT.cpp(1) | pending | pending |
| 20260609 | 4 | 1 | — | KYECFTP\FTPClient.cpp(4) | A | **done MG-W10**（CC_PTI 非阻塞化） |
| 20260611 | 3 | 2 | CASE-20260611-001 | uLotInfo.cpp(2), cTrayAssignment.cpp(1) | A | **done MG-W10** |
| 20260612 | 50 | 6 | CASE-20260611-001 | FTPUpload\uFtpUploadThread.cpp(23), uLotInfo.cpp(11), FTPUpload\uFtpUploadThread.h(8), main.cpp(6) +2檔 | A | **done MG-W10**（背景上傳執行緒＋新增檔） |
| 20260618 | 1 | 1 | CASE-20260618-001 | Motor\mymotor.cpp(1) | pending | pending |
| 20260623 | 6 | 3 | — | cConfiguration.cpp(4), MachineType.h(1), main.cpp(1) | pending | pending |
| 20260625 | 7 | 2 | — | Interface\TesterTCP.cpp(6), Interface\TesterTCP.h(1) | pending | pending |
| 20260629 | 1 | 1 | CASE-PTI-20260629-001 | main.cpp(1) | pending | pending |
| 20260630 | 18 | 8 | CASE-PTI-20260630-002 | main.cpp(6), csystem.cpp(3), HS_Function.cpp(2), cpublic.cpp(2) +4檔 | pending | pending |
| 20260703 | 25 | 8 | CASE-PTI-20260630-001 | acatchtray.cpp(8), cmydef.cpp(4), AutoClean\uCleaning.cpp(3), cmydef.h(3) +4檔 | pending | pending |
| 20260706 | 1 | 1 | — | ainarm9045.cpp(1) | pending | pending |
| 20260803 | 5 | 3 | CASE-GIGAS-20260729-001 | CosFunction.cpp(2), ainarm9045.cpp(2), CosFunction.h(1) | B | **done MG-W9**（收閘五落點） |
| 20260804 | 25 | 5 | CASE-PTI-20260804-001 | CosFunction.cpp(9), CosFunction.h(6), PowerSavingMode.cpp(5), cConfiguration.cpp(4) +1檔 | B | **done MG-W8**（C05 profile，FUNC_CC_PTI gate） |
| 20260810 | 21 | 9 | — | aoutarm9045_2x4_4.cpp(10), aoutarm.cpp(2), aoutarm9045.cpp(2), main.cpp(2) +5檔 | pending | pending |
| 20260811 | 21 | 5 | CASE-PTI-20260811-001 | PowerSavingMode.cpp(7), aoutarm9045.cpp(7), RotateKit\aRotateKIT_Out.cpp(4), PowerSavingMode.h(2) +1檔 | pending | pending |
| 20260817 | 11 | 5 | CASE-FOREHOPE_NINGBO-20260813-001 | cObserver.cpp(5), CosFunction.cpp(2), cTrayAssignment.cpp(2), CosFunction.h(1) +1檔 | A/B | **done（11/11）**：cObserver 5=MG-W3；CYUEAN 5=MG-W6；PowerSavingMode:363=MG-W8 |
| 20260819 | 10 | 4 | — | ProductionInfo\ProductionInfo.cpp(6), CosFunction.cpp(2), CosFunction.h(1), ProductionInfo\ProductionInfo.h(1) | pending | pending |
| 20260820 | 7 | 3 | — | ProductionInfo\ProductionInfo.cpp(3), cBinSel.cpp(2), main.cpp(2) | pending | pending |

## 叢集明細（每叢集樣本註解）

### 20260401（1 條）

- `SECSGEM\uHGemHT9045_SV.cpp:360` //AI(ht9045-v899) 20260401: Add LB Up/Down temperature and limits for SECS collection

### 20260407（11 條）

- `AutoClean\AutoClean.cpp:1645` //AI(ht9045-v899) 20260407: cCleanKitPickPlan implementation
- `AutoClean\AutoClean.cpp:1649` //AI(ht9045-v899) 20260407: runtime switch for new/old auto clean search logic
- `AutoClean\AutoClean.cpp:2112` //AI(ht9045-v899) 20260407: dual-path switch  bUseCKPP selects new class vs old logic
- `AutoClean\AutoClean.cpp:3056` //AI(ht9045-v899) 20260407: prevent hang when no clean pads remain
- `AutoClean\AutoClean.cpp:3092` //AI(ht9045-v899) 20260407: AC debug log - PickfromCleanKit Task20 result
- `AutoClean\AutoClean.cpp:3456` //AI(ht9045-v899) 20260407: allow partial pick when inactive suckers have no pad (Fix H)

### 20260408（6 條）

- `AutoClean\AutoClean.cpp:1941` //AI(ht9045-v899) 20260408: check if current shuttle group has any sucker demand
- `AutoClean\AutoClean.cpp:2782` //AI(ht9045-v899) 20260408: plan-driven pick - iterate plan slots instead of hardcoded j=0..3
- `AutoClean\AutoClean.cpp:3040` //AI(ht9045-v899) 20260408: kit empty but arm has partial pick - skip 3000-3100 loop, go finish (S6)
- `AutoClean\AutoClean.cpp:3082` //AI(ht9045-v899) 20260408: safety net - if CKPP plan not found, return pick complete
- `AutoClean\AutoClean.cpp:3370` //AI(ht9045-v899) 20260408: unified plan-driven contact count + completion check (S5)
- `AutoClean\AutoClean.cpp:5116` //AI(ht9045-v899) 20260408: purge phantom HAS_NULL_CLEAN_IC from InArm before dispatch

### 20260409（1 條）

- `AutoClean\AutoClean.cpp:2116` //AI(ht9045-v899) 20260409: recycle CLEAN_FINISH_IC -> HAS_CLEAN_IC before search so returned pads are reusabl

### 20260410（1 條）

- `CosFunction.cpp:3381` //AI(ht9045-v899) 20260410: enable SortingBinTray when CleanOut for FOREHOPE_NINGBO

### 20260414（10 條）

- `AutoClean\uCleaning.cpp:561` //AI(ht9045-v899) 20260414: zero all pad counts when DevicePices or tray layout changes to prevent stale count
- `AutoClean\uCleaning.cpp:1949` //AI(ht9045-v899) 20260414: fix operator precedence - was iDeviceNum==24 always true
- `HS_Function.cpp:1360` //AI(ht9045-v899) 20260414: trace daily-upload entry only (iLog>=5) to avoid timer-driven log bloat
- `HS_Function.cpp:1371` //AI(ht9045-v899) 20260414: trace skip reason
- `HS_Function.cpp:1983` //AI(ht9045-v899) 20260414: diagnostic log only on daily-upload path to avoid timer-driven bloat
- `HS_Function.cpp:1997` //AI(ht9045-v899) 20260414: trace success

### 20260415（5 條）

- `HS_Function.cpp:48` //AI(ht9045-v899) 20260415: forward-declare MNetLog before first use at UpDataToServer_KYEC
- `HS_Function.cpp:1773` //AI(ht9045-v899) 20260415: swap SystemDate to yesterday before GetProdLog to fix cross-midnight filename mism
- `HS_Function.cpp:1802` //AI(ht9045-v899) 20260415: swap SystemDate to yesterday before GetGPIBLog to fix cross-midnight filename mism
- `cShowBinSelect.cpp:815` //AI(ht9045-v899) 20260415: dynamic stack layout for tsIndex tab to prevent overlapping panels
- `cShowBinSelect.cpp:1491` //AI(ht9045-v899) 20260415: dynamic height for tsIndex based on visible panel stack

### 20260416（1 條）

- `AutoClean\AutoClean.cpp:2903` //AI(ht9045-v899) 20260416: revert phantom-marking skip per Gigas feedback - V898 did not have this guard

### 20260417（3 條）

- `AutoClean\AutoClean.cpp:5199` //AI(ht9045-v899) 20260417: revert to V898 logic - premature CLEAN_FINISH_IC recycling caused pick to restart 
- `cShowBinSelect.cpp:809` //AI(ht9045-v899) 20260417: show AutoDeviceEjection panel for QLE or by config flag

### 20260420（1 條）

- `HS_Function.cpp:26` //AI(BVL-3766) 20260420: 開關 site 時要即時重寫 GTK info.txt

### 20260423（19 條）

- `AutoClean\uCleaning.cpp:148` //AI(ht9045-v899) 20260423: Greatek要求 default 60->1 (CASE-20260423-001)
- `HandlerSys.cpp:519` //AI(ht9045-v899) 20260423: apply empty y stepper
- `HandlerSys.cpp:520` //AI(ht9045-v899) 20260423: apply color y stepper
- `HandlerSys.cpp:521` //AI(ht9045-v899) 20260423: apply auto1 y stepper
- `main.cpp:6` //AI(ht9045-v899) 20260423: BootLog for crash diagnosis (24V-off scenario)
- `main.cpp:8567` //AI(ht9045-v899) 20260423: BootLog checkpoint

### 20260424（8 條）

- `MyLaneIo.cpp:327` //AI(ht9045-v899) 20260424: log 維持簡短供解析, UI 改用 BuildIOErrMessage 提供完整修法
- `MyLaneIo.cpp:695` //AI(ht9045-v899) 20260424: 加上中文說明與修法, 讓售服直接看得懂
- `MyLaneIo.cpp:726` //AI(ht9045-v899) 20260424: 將 mn200.h 內 DEV_INF_xxx enum 翻成售服看得懂的字串
- `MyLaneIo.cpp:757` //AI(ht9045-v899) 20260424: 統一 IO 錯誤訊息格式, 給售服 5 段資訊一次到位
- `MyLaneIo.h:40` //AI(ht9045-v899) 20260424: 把 mn200 device type enum 翻成可讀字串給售服
- `MyLaneIo.h:41` //AI(ht9045-v899) 20260424: 統一 IO 錯誤訊息格式給售服

### 20260429（3 條）

- `main.cpp:10743` //AI(ht9045-v899) 20260429: gate the prompt so it only fires for the real
- `note.cpp:6689` //AI(ht9045-v899) 20260429: 甬矽要求 EventLog 時間欄不加雙引號與前綴空格 (CASE-20260429-001)
- `note.cpp:6702` //AI(ht9045-v899) 20260429: 其他客戶維持 Steven 20251107 新方法 (CASE-20260429-001)

### 20260430（21 條）

- `AutoClean\uCleaning.cpp:174` //AI(ht9045-v899) 20260430: extend AutoClean DieForce to Multi EP half.
- `ContactForce.cpp:532` //AI(ht9045-v899) 20260430: removed iIndEPCnt==8 branch (was 4 site outer-only); replaced by INSTALL_DOUBLE_EP
- `ContactForce.cpp:710` //AI(ht9045-v899) 20260430: 16 EP independent + Multi EP half (INSTALL_DOUBLE_EP==DOUBLE_EP_MULTI)
- `ContactForce.cpp:884` //AI(ht9045-v899) 20260430: Multi EP half - show OneByOne tab.
- `ContactForce.cpp:1432` //AI(ht9045-v899) 20260430: ported from V874.3 Ifor.
- `ContactForce.h:352` //AI(ht9045-v899) 20260430: Multi EP half (INSTALL_DOUBLE_EP==DOUBLE_EP_MULTI) per-site Die Force container.

### 20260504（17 條）

- `ContactForce.cpp:300` //AI(ht9045-v899) 20260504: full port from V874.3 Ifor (撤回 20260430 砍半版); iCount=(Tag%8)+1, 1..4=Arm1, 5..8=Ar
- `ContactForce.cpp:333` //AI(ht9045-v899) 20260504: full port V874 (Tag%8): 1..4=Arm1, 5..8=Arm2.
- `ContactForce.cpp:502` //AI(ht9045-v899) 20260504: full port V874.3 INSTALL_DOUBLE_EP==DOUBLE_EP_MULTI SLKIndClass: i*8+j (8 sites: A
- `ContactForce.cpp:1103` //AI(ht9045-v899) 20260504: full port V874.3 OneByOne ReadFile. Per Kit 8 entries: j<4=Arm1_(j+1), j>=4=Arm2_(
- `ContactForce.cpp:1158` //AI(ht9045-v899) 20260504: full port V874.3 SLKIndClass ==3 ReadFile (Arm1_1..4 / Arm2_1..4). SPEC-V899-Multi
- `ContactForce.cpp:1308` //AI(ht9045-v899) 20260504: full port V874.3 SLKIndClass ==3 WriteFile (Arm1_1..4 / Arm2_1..4). SPEC-V899-Mult

### 20260505（2 條）

- `adam6024.cpp:263` //AI(ht9045-v899) 20260505: set target IP before status check so guidance uses the field-selected address
- `uhome.cpp:1864` //AI(ht9045-v899) 20260505: �����脖葉��� Home (�����ａ��蝔� hangup 敺���� Home All) ���嚗�

### 20260511（8 條）

- `adam6024.cpp:1587` //AI(ht9045-v899) 20260511: 把舊 Independent EP loop 限制在 DOUBLE_EP_INDIVIAL 才跑。
- `adam6024.cpp:1595` //AI(ht9045-v899) 20260511: bounds guard
- `adam6024.cpp:1601` //AI(ht9045-v899) 20260511: 防止 SLKIndClass 與 slSLKTypeInd 不同步時越界
- `iosetview.cpp:47` //AI(ht9045-v899) 20260511: init NULL to prevent dangling deref in Timer1Timer when click handler did not assi
- `iosetview.cpp:108` //AI(ht9045-v899) 20260511: RAII guard - destructor always clears bTimerRun on any function exit (normal retur
- `iosetview.cpp:165` //AI(ht9045-v899) 20260511: null-guard before deref; if pointer not assigned yet, force state Off to avoid AV 

### 20260513（3 條）

- `acatchtray.cpp:520` //AI(ht9045-v899) 20260513: dummy 空跑不要觸發 P18 人工補 Fail Auto Tray 流程
- `asendic_Auto.cpp:203` //AI(ht9045-v899) 20260513: dummy 空跑不啟用 P18 旁路, 仍走原本 tray mismatch 處理
- `csystem.cpp:15437` //AI(ht9045-v899) 20260513: dummy 空跑不檢查人工補 Fail Auto Tray 的 Color Sensor

### 20260514（8 條）

- `AutoClean\AutoClean.cpp:1650` //AI(ht9045-v899) 20260514: align comment with actual default (off; legacy SearchCleanKitUpDown is in use)
- `AutoClean\AutoClean.cpp:1755` //AI(ht9045-v899) 20260514: legacy EvaluatePlan removed; enumeration delegated to cArmPickPlan
- `AutoClean\AutoClean.cpp:1756` //AI(ht9045-v899) 20260514: IArmPickPlanContext implementation begins
- `AutoClean\AutoClean.cpp:1816` //AI(ht9045-v899) 20260514: preserve legacy "skip plans where lead has no pad / unreachable"
- `AutoClean\AutoClean.cpp:1827` //AI(ht9045-v899) 20260514: translate generic plan back into legacy TPickPlan/TPickSlot
- `AutoClean\AutoClean.cpp:1896` //AI(ht9045-v899) 20260514: Search() now delegates enumeration to generic cArmPickPlan

### 20260515（3 條）

- `cContactCT.cpp:202` //AI(ht9045-v899) 20260515: 修正南茂回報Yield Chart左欄site標籤(Aa/Ab/Ba...)空白; ChangeToFloatNonPcnt回傳float, 'A'+float 走
- `cContactCT.cpp:242` //AI(ht9045-v899) 20260515: 同上修正, NN_2Row site標籤強制int

### 20260519（4 條）

- `BarCode\BarCode.cpp:4550` //AI(ht9045-v899) 20260519: match e2x2In2CCD missing barcode check with 2x2 buffer mapping.
- `HS_Function.cpp:3549` //AI(ht9045-v899) 20260519: GTK info.txt is saved by callers after site-state synchronization.
- `uTrayEditForm.cpp:504` //AI(ht9045-v899) 20260519: keep timer refresh from re-triggering Enter screenshot save.
- `uTrayEditForm.cpp:707` //AI(ht9045-v899) 20260519: keep screenshot save failures from hanging Tray Edit UI.

### 20260520（1 條）

- `CosFunction.cpp:1466` //AI(ht9045-v899) 20260520: enable E55 Fix3 full tray gate for PTI import plan

### 20260525（2 條）

- `main.cpp:24003` //AI(ht9045-v899) 20260525: sync PASS auto positions after workfile switch
- `main.cpp:25604` //AI(ht9045-v899) 20260525: pack PTI Lot Start trace with TCP log for permission/runmode diagnosis

### 20260526（7 條）

- `ContactForce.cpp:453` //AI(ht9045-v899) 20260526: align EP one-by-one site trim diameters with V874/general SLK types instead of leg
- `ContactForce.cpp:694` //AI(ht9045-v899) 20260526: Show Multi EP site trim page for outer EP per-site calibration.
- `ContactForce.cpp:970` //AI(ht9045-v899) 20260526: reload saved one-by-one load rates so the next TfContact SAVE uses current trim va
- `HS_Function.cpp:1040` //AI(ht9045-v899) 20260526: do not read/log Multi EP data while common EP route is selected.
- `adam6024.cpp:1627` //AI(ht9045-v899) 20260526: in Multi EP production, normal air force fills outer/site EP values.
- `adam6024.cpp:1670` //AI(ht9045-v899) 20260526: in Multi EP production, DoubleForce fills inner/dual EP values.

### 20260602（13 條）

- `acatchtray.cpp:5915` //AI(ht9045-v899) 20260602: 殘料檢查pass後先把LoaderY送回SurePos(rear)再進取盤流程，避免LoaderY停在front(CarPos)導致TrayArm夾空JAM0604
- `acatchtray.cpp:5948` //AI(ht9045-v899) 20260602: 殘料檢查(台車模式)pass後LoaderY歸位
- `acatchtray.cpp:5949` //AI(ht9045-v899) 20260602: 雷射流程收尾DoMoveOut會把LoaderY停在front(CarPos)，這裡先放開固定壓桿並把盤送回SurePos(rear吸料位)、放開夾爪，TrayAr
- `ainarm9045.cpp:7057` //AI(ht9045-v899) 20260602: watchdog for InArm load-stage no-tray dead-lock (case 15) on non-auto-skip custome
- `ainarm9045.cpp:7060` //AI(ht9045-v899) 20260602: watchdog for InArm load-stage empty-tray residual dead-lock (case 15) when the emp
- `ainarm9045.cpp:7080` //AI(ht9045-v899) 20260602: latch for case 15 no-tray watchdog (arm once, avoid restart every scan)

### 20260605（1 條）

- `cSortCT.cpp:11` //AI(ht9045-v899) 20260605: 改用 ShowMyMessageBox_YES_NO 需引入 mymessbox.h

### 20260609（4 條）

- `KYECFTP\FTPClient.cpp:401` //AI(ht9045-v899) 20260609: PTI 縮短 timeout 5s,避免 FTP server 不可達時 Lot End 主執行緒長時間阻塞 //Landam
- `KYECFTP\FTPClient.cpp:475` //AI(ht9045-v899) 20260609: PTI 連線失敗只記 log,不彈阻塞 modal(避免 Lot End 卡住消音/動作)
- `KYECFTP\FTPClient.cpp:795` //AI(ht9045-v899) 20260609: PTI Lot End 上傳失敗只記 log,不彈阻塞 modal(避免停機卡住消音/動作),且不讀 LastErrorNo 避免存取異常 socket 觸發崩潰
- `KYECFTP\FTPClient.cpp:815` //AI(ht9045-v899) 20260609: PTI 上傳失敗只記 log,不讀 LastErrorNo 避免存取異常 socket 觸發崩潰

### 20260611（3 條）

- `cTrayAssignment.cpp:358` //AI(ht9045-v899) 20260611(CASE-20260611-001): 上面迴圈只讀到iFixRight為止,Fix下半盤(eFix4~6 / eFix7~12)的Direction/iTrayTy
- `uLotInfo.cpp:1959` //AI(ht9045-v899) 20260611(CASE-20260611-001): 上傳前清旗標,作為連線失敗判斷依據
- `uLotInfo.cpp:1967` //AI(ht9045-v899) 20260611(CASE-20260611-001): N06/FTP 連線失敗(socket 10038)即中止整批上傳,避免每個檔重複 blocking connect/retr

### 20260612（50 條）

- `Config.h:957` //AI(ht9045-v899) 20260612(CASE-20260611-001): Lot End FTP上傳背景化開關, PTI預設true其餘false, 供S6/S7 uLotInfo分流(背景Enque
- `FTPUpload\uFtpUploadThread.cpp:4` //AI(ht9045-v899) 20260612(CASE-20260611-001): 新增 FTP 背景上傳 thread 骨架（S1）。
- `FTPUpload\uFtpUploadThread.cpp:5` //AI(ht9045-v899) 20260612(CASE-20260611-001): S2 完成 — UploadOneJob 改用 WinINet
- `FTPUpload\uFtpUploadThread.cpp:7` //AI(ht9045-v899) 20260612(CASE-20260611-001): S3 完成 — Enqueue 去重（jobKind+localFile 識別鍵
- `FTPUpload\uFtpUploadThread.cpp:18` //AI(ht9045-v899) 20260612(CASE-20260611-001): WinINet FTP API（InternetOpen/InternetConnect/FtpPutFile）
- `FTPUpload\uFtpUploadThread.cpp:22` //AI(ht9045-v899) 20260612(CASE-20260611-001): S4 移除此處暫時定義，改由 main.cpp 定義。

### 20260618（1 條）

- `Motor\mymotor.cpp:415` //AI(ht9045-v899) 20260618: CASE-20260618-001 南茂(CC_ChipMos)/e360KG 落入 else 使 iCheckZ=4000 門檻過緊，Z2 到位較慢提早觸發 Z1

### 20260623（6 條）

- `MachineType.h:1631` //AI(ht9045-v899) 20260623: 新增整數安全除法(零防護+取整),供格線欄號/索引等整數情境使用,避免誤用浮點版造成階梯式位移
- `cConfiguration.cpp:153` //AI(ht9045-v899) 20260623: 改用整數安全除法還原離散欄號,修正浮點除法造成 CheckBox 階梯式位移
- `cConfiguration.cpp:4983` //AI(ht9045-v899) 20260623: 界外/空日期(1899-12-30)防呆,無效時回填Now()避免picker指派拋例外
- `cConfiguration.cpp:7327` //AI(ht9045-v899) 20260623: [O]EventLog存檔包try/catch，避免單一picker例外中斷整個SaveConfiguration(原會使Enable FTP等設定全寫不進)
- `cConfiguration.cpp:7330` //AI(ht9045-v899) 20260623: dtpO06NextTime為dtkTime picker,指派->Date在界外日期(1899-12-30)拋EDateTimeError;改用日期picker的
- `main.cpp:5640` //AI(ht9045-v899) 20260623: 移除 #ifdef SOFT_SIMULTE 包裝，使 C08_1 Socket Sensor 偵測在正式 build 也生效（原僅模擬 build 編入導致正式機

### 20260625（7 條）

- `Interface\TesterTCP.cpp:1050` //AI(ht9045-v899) 20260625: 新增去重式記錄器，N06 recipe 同步成敗寫入既有 EventLog，避免靜默失敗且不膨脹日誌
- `Interface\TesterTCP.cpp:1063` //AI(ht9045-v899) 20260625: 取 7z 完整路徑(不硬編碼)，FileExists 為 false 才 fallback
- `Interface\TesterTCP.cpp:1072` //AI(ht9045-v899) 20260625: 同步執行 7z 並取得 exit code(沿用 main.cpp ShellExecuteEx+WaitForSingleObject 模式)；回傳 false 
- `Interface\TesterTCP.cpp:1100` //AI(ht9045-v899) 20260625: 改回傳 bool + 同步等待 + 取 exit code + 存在性檢查，避免 N06 上傳靜默失敗
- `Interface\TesterTCP.cpp:1145` //AI(ht9045-v899) 20260625: 同 CopyRecipeToTester，改 bool 回傳 + 同步取 exit code + 存在性檢查 + 去重記錄
- `Interface\TesterTCP.cpp:1155` //AI(ht9045-v899) 20260625: 原註解掉的 log 改為呼叫去重記錄器

### 20260629（1 條）

- `main.cpp:25418` //AI(ht9045-v899) 20260629(CASE-PTI-20260629-001): 新增 Home/Buzzer/Modal 診斷 dump,供 auto-init 回HOME hang 偶發時定位早退

### 20260630（18 條）

- `HS_Function.cpp:1967` //AI(ht9045-v899) 20260630(CASE-PTI-20260630-002): 最外層 try/catch 加固,讓 socket 例外不再以空 UnitName 逸出到 main.cpp AppE
- `HS_Function.cpp:1987` //AI(ht9045-v899) 20260630(CASE-PTI-20260630-002): connect timeout 30000->5000ms 與 PTI N06 路徑一致,降低同步阻塞最壞值
- `cpublic.cpp:655` //AI(ht9045-v899) 20260630: ExecZipCommand 的 handle 版,回傳子行程 process handle(失敗回 NULL),
- `cpublic.cpp:768` //AI(ht9045-v899) 20260630: 為 PTI 在 Sort Summary 補出 Fix4-6 與 Total 出料加總欄（PTI 恆輸出，非 PTI 輸出不變）
- `cpublic.h:46` //AI(ht9045-v899) 20260630: 同 ExecZipCommand 但回傳 process handle 供非阻塞輪詢(State Record 確認 1.bat 複製完才壓縮/刪資料夾)
- `csystem.cpp:21812` //AI(ht9045-v899) 20260630: 子目錄移除同樣重試,與最後 RemoveDir 對稱(子層已先清空檔案)

### 20260703（25 條）

- `AutoClean\uCleaning.cpp:2650` //AI(ht9045-v899) 20260703: CASE-PTI-20260630-001 觸發當下做非阻塞提示(原因=alarm 描述+code, 處置=自動清潔已暫停/已重新初始化偵測, 續低良率請清潔 So
- `AutoClean\uCleaning.cpp:2863` //AI(ht9045-v899) 20260703: CASE-PTI-20260630-001 需求1 移除此處「重貼 alarm 擋機」ShowErrorMessage(客戶否決的鎖機行為)。改為函式尾恆放行 ST
- `AutoClean\uCleaning.cpp:2867` //AI(ht9045-v899) 20260703: CASE-PTI-20260630-001 需求3 若偵測待重置(前一輪 iACUseParam==0 觸發)則於放行前呼叫 ResetSmartAutoClean
- `acatchtray.cpp:1221` //AI(ht9045-v899) 20260703: case 570 JAM0610 防抖計數,比照 DoPlaceTrayToAuto_250
- `acatchtray.cpp:1729` //AI(ht9045-v899) 20260703: Greatek極性校正-有盤時FixOn_On不會到位,不靠FixOn確認直接進Task=310略過JAM0613;夾到與否由下游IsTrayArmCatchTra
- `acatchtray.cpp:2007` //AI(ht9045-v899) 20260703: 補 iCnt>100 防抖,避免搬運中 FixOn sensor 單次彈跳即誤報 JAM0610(頻繁 Alarm 頂層主因);比照 DoPlaceTrayToAu

### 20260706（1 條）

- `ainarm9045.cpp:4661` //AI(ht9045-v899) 20260706: 系統性可達性選嘴(不寫死吸嘴代號/欄位):預設仍取最左啟用吸嘴維持原行為,

### 20260803（5 條）

- `CosFunction.cpp:2789` //AI(ht9045-v899) 20260803: the 20260602 InArm case-15 dead-lock watchdog was added for this customer only (CA
- `CosFunction.cpp:4414` //AI(ht9045-v899) 20260803: default off - the 20260602 case-15 watchdog must only run for CC_ARDENTEC (CASE-GI
- `CosFunction.h:470` //AI(ht9045-v899) 20260803: InArm load-stage dead-lock watchdog (case 15), CC_ARDENTEC only (CASE-GIGAS-202607
- `ainarm9045.cpp:7263` //AI(ht9045-v899) 20260803: gate this 20260602 watchdog to CC_ARDENTEC only; TQPF_Timer is a pure wall clock w

### 20260804（25 條）

- `CosFunction.cpp:1469` //AI(ht9045-v899) 20260804: turn on C05 power saving for PTI; stop heating after a long HALT once the lot has 
- `CosFunction.cpp:1470` //AI(ht9045-v899) 20260804: unhide the [C05] group, it was forced off by InitialCosFunction
- `CosFunction.cpp:1471` //AI(ht9045-v899) 20260804: 720min=12h so 8h(480min) is reachable; EncodeTime caps the hard ceiling at 1439
- `CosFunction.cpp:1472` //AI(ht9045-v899) 20260804: temp module only; motor/vacuum/ATC/mode stay forced to 0 exactly as today
- `CosFunction.cpp:1473` //AI(ht9045-v899) 20260804: one-cycle repair keeps lot-start, cutting the heater there would waste a re-soak
- `CosFunction.cpp:1474` //AI(ht9045-v899) 20260804: ambient halt is out of scope per the customer remark

### 20260810（21 條）

- `RotateKit\aRotateKIT.cpp:114` //AI(ht9045-v899) 20260810: 2x4 交錯 4 站(e9045_2x4_4_13/_14)時 SetInOutArmParameter_2x4_4() 已用 CopyInitSuck 把 Out
- `RotateKit\aRotateKIT_Out.cpp:1131` //AI(ht9045-v899) 20260810: 記錄這一趟到底把哪幾支吸嘴放到旋轉站。列舉用的 stride 是 i2x2Suck_Out, 只要它跟吸嘴實際欄位對不上, 就會有 IC 沒旋轉就進 Auto tr
- `aoutarm.cpp:3125` //AI(ht9045-v899) 20260810: 超出極限救援時, 最小階(i=1)若僅略低於機構下限則夾到下限
- `aoutarm.cpp:3126` //AI(ht9045-v899) 20260810: 原本會被丟棄改用下一倍數, 間距反而比原來更大而仍然超限
- `aoutarm9045.cpp:2100` //AI(ht9045-v899) 20260810: OutArm 逐輪診斷 Log 實作。宣告與用途見 aoutarm9045.h
- `aoutarm9045.cpp:2341` //AI(ht9045-v899) 20260810: 記錄旋轉/AOI 閘門的判定結果, 讓「為什麼沒去旋轉」有跡可循

### 20260811（21 條）

- `PowerSavingMode.cpp:32` //AI(ht9045-v899) 20260811: PTI 要求進入省電後狀態列要更顯眼,底色反黃並放大字型;
- `PowerSavingMode.cpp:66` //AI(ht9045-v899) 20260811: 狀態列反黃起始為關 (CASE-PTI-20260811-001)
- `PowerSavingMode.cpp:103` //AI(ht9045-v899) 20260811: 所有中止/重新計數的路徑都會先把 bRestart 立起來,在這裡一次還原狀態列,
- `PowerSavingMode.cpp:118` //AI(ht9045-v899) 20260811: C05 全關時面板會隱藏,先還原反黃與高度 (CASE-PTI-20260811-001)
- `PowerSavingMode.cpp:258` //AI(ht9045-v899) 20260811: 反黃與放大字型一併交給 ShowPowerSaveHighlight() 處理 (CASE-PTI-20260811-001)
- `PowerSavingMode.cpp:354` //AI(ht9045-v899) 20260811: PTI 要求省電模式在主畫面要更明顯: 底色反黃 + 字型放大。

### 20260817（11 條）

- `CosFunction.cpp:2847` //AI(ht9045-v899) 20260817: 客戶反映 Auto Tray Feed 跟隨工作檔, 換工單就把 Clean Out 的 Tray End 選單架空, 故固定關閉
- `CosFunction.cpp:4322` //AI(ht9045-v899) 20260817: 預設不關閉, 只有 CC_CYUEAN 會開啟
- `CosFunction.h:354` //AI(ht9045-v899) 20260817: CC_CYUEAN 固定關閉 Auto Tray Feed(勾選時會架空 Clean Out 的 Tray End 選單)
- `PowerSavingMode.cpp:363` //AI(ht9045-v899) 20260817: guard 只擋「重複點亮」,還原一律要能執行。倒數字串是
- `cObserver.cpp:3790` //AI(ht9045-v899) 20260817: EventLog CSV 顯示用斷欄。只以逗號分隔，並保留雙引號內的空白與逗號。
- `cObserver.cpp:3890` //AI(ht9045-v899) 20260817: 改用 ParseEventLogLine (CASE-FOREHOPE_NINGBO-20260813-001)

### 20260819（10 條）

- `CosFunction.cpp:1426` //AI(ht9045-v899) 20260819: 依 MO 工單 Temperature mode 含 HOT AIR 字眼自動開關熱風槍
- `CosFunction.cpp:3881` //AI(ht9045-v899) 20260819: 預設關閉, 只有 CC_Greatek 開啟
- `CosFunction.h:477` //AI(ht9045-v899) 20260819: 超豐要求依 MO 工單 Temperature mode 是否含 HOT AIR 字眼自動開關熱風槍
- `ProductionInfo\ProductionInfo.cpp:339` //AI(ht9045-v899) 20260819: 換完工作檔後再依 MO Temperature mode 自動開關熱風槍
- `ProductionInfo\ProductionInfo.cpp:2052` //AI(ht9045-v899) 20260819: 超豐要求依 MO 工單 Temperature mode 欄位自動開關熱風槍(Active Heater Gun)
- `ProductionInfo\ProductionInfo.cpp:2063` //AI(ht9045-v899) 20260819: 沒有熱風槍機構就不動作

### 20260820（7 條）

- `ProductionInfo\ProductionInfo.cpp:1965` //AI(ht9045-v899) 20260820: PTI 的 Fix1 位置是 rotate module,未指派 Category 時程式會把
- `ProductionInfo\ProductionInfo.cpp:1983` //AI(ht9045-v899) 20260820: 見上方說明
- `ProductionInfo\ProductionInfo.cpp:5596` //AI(ht9045-v899) 20260820: bound loop by asStackFail_Pass length(9); iBinSelectLength is eTrayCount(33) and o
- `cBinSel.cpp:1371` //AI(ht9045-v899) 20260820: PTI HT-90* 的 Fix1 位置是 rotate module,不會指派 Category;
- `cBinSel.cpp:1395` //AI(ht9045-v899) 20260820: 未指派 Category 的 Fix1/Fix4 預設為 Fail(說明見本函式上方)
- `main.cpp:30011` //AI(ht9045-v899) 20260820: i由1改0, 原本Row A(index 0)整排未被記錄; 全樹其餘ArmSKET迴圈(cSocket/auto9045/SCK_ART等)皆由0起算

