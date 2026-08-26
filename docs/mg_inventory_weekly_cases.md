# MG 戰役 — Weekly_AI 客戶案件盤點（V899 程式變更）

- **產出日期**：2026-08-26
- **資料來源（唯讀）**：
  - `d:\Work-jimmychiu\document\WeeklyReport\Weekly_AI\Customer\<客戶>\CASE-*\issue.md`（88 個案件夾，全部掃描）
  - 各案 `02_analysis/`、`03_fix/`、`04_release/` 的 `.md/.txt` 交付文件
  - `d:\Work-jimmychiu\document\WeeklyReport\Weekly_AI\weekly_data.json`
- **交叉驗證來源（唯讀）**：`D:\HT9045\HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422` 樹內全部 `//AI(...) YYYYMMDD` 註解（ground truth）
- **判斷準則**：以「V899 樹內是否留下實際程式碼變更」為準，而非「案件是否有出貨版號」。
  issue.md 敘述與樹內 `//AI` 註解不一致時，**以樹內註解為準**，差異記入問題清單。

---

## 1. 統計摘要

| 分類 | 案數 | 說明 |
|---|---:|---|
| **有碼變更（V899 樹）** | **45** | 要進 MG 搬移總帳 |
| 純分析／純設定（V899 產品線，不搬） | 25 | E 類：無程式碼變更 |
| 不確定（資訊不足） | 4 | 見第 5 節 |
| 排除（非 V899 樹的產品） | 14 | HT172 ×13、HT160S ×1 |
| **合計** | **88** | |

案件狀態：`done` 84 件、`in-progress` 3 件、`new` 1 件。

涉及客戶目錄（依目錄名，不改名）：`Greatek`(config 名「超豐」)、`上海安靠`、`京元竹南`、`全智`、`力成PTI`、`南茂科技`、`台星科`、`欣銓`、`瑞昱`、`甬矽`、`盛合晶微`、`確安科技`、`紹興長電`、`長沙安牧泉`。

版號跨度：**V3.33.899.8 ～ V3.33.899.37**。

---

## 2. 「有碼變更」案件表（按變更日期排序）

> `//AI 日期`欄位若標 ✅ 表示已在 V899 樹內找到對應註解（可直接定位 splice 範圍）；
> 標 ⚠ 表示只有文件敘述、樹內無同日註解。
> 檔案路徑相對於 `HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422/`。

| # | 變更日期 | 客戶目錄 | CASE-ID | 出貨版號 | 涉及檔案（V899 樹相對路徑） | 客戶碼／閘門 | //AI | 一句話描述 |
|---:|---|---|---|---|---|---|:--:|---|
| 1 | 2026-04-23 | Greatek | CASE-20260423-001 | V3.33.899.10 | `AutoClean/uCleaning.cpp` (4 處) | 無（全客戶預設值） | ✅ 20260423 | Auto Clean 的 Vacuum / Air On delay 預設值 60 秒改 1 秒。 |
| 2 | 2026-04-28 | Greatek | CASE-20260428-001 | V3.33.899.11 | `ProductionInfo/ProductionInfo.cpp`（SaveInfoFileWhenStart）、`Command.cpp`（SetSiteMapData） | `CC_Greatek` | ✅ BVL-3766 20260420/20260428 | GTK info.txt 的 RunSite 欄位在 One Side 模式下空白，改兩臂 OR + runtime/LastSet 都吃。 |
| 3 | 2026-04-28 | 瑞昱 | CASE-20260428-001 | V3.33.899.12 | `cSiteUseManager.cpp:265` | 無 | ✅ JimmyChiu 20260428 | 1x4 加熱模式 hang up：撤回 `e9045_1x4_4` 的 force-enable。 |
| 4 | 2026-04-29 | 甬矽 | CASE-20260429-001 | V3.33.899.13 | `note.cpp:6689,6702` | 甬矽分支 vs 其他客戶分支 | ✅ 20260429 | EventLog Alarm 行時間欄位不加雙引號（V871 修正回歸）。**⚠ 此修正把整行所有欄位的引號都拿掉，是 CASE-FOREHOPE_NINGBO-20260813-001 欄位錯位的根因。** |
| 5 | 2026-04-30 ~ 05-26 | 欣銓 | CASE-20260429-001 | 未出貨（決議不交付） | `database.cpp`、`HandlerSys.cpp`/`.h`/`.dfm`、`ContactForce.cpp`/`.h`、`adam6024.cpp`/`.h`（另有 `OCR*.cpp`、`OCRInsp*.cpp`、`cTrayMapping.*`、`TfAOILaserScan.*` 同期群） | `iIndEPCnt==8` runtime 選項 | ✅ 20260430/0504/0505/0511/0522/0526 | Multi EP 8ch（1x2 dual site × 2 Arm）。**程式已在樹內，但 CASE-20260512-001 決議保留 ch16、不交付 8-EP 版本 → 搬不搬需決策。** |
| 6 | 2026-05-04 / 05-15 | Greatek | CASE-20260515-001 | V3.33.899.19 | `ProductionInfo/ProductionInfo.cpp`(7)/`.h`、`HS_Function.cpp`(7)/`.h`、`Automation/auto9045.cpp`(SetLotInfoGreatekOEE)、`Automation/automation.cpp`、`Automation/auto9045.h` | `CC_Greatek` / `bOEEFunction` | ✅ 20260504、JimmyChiu 20260515 | F899-008 v3：LotInfo_REQUEST 內嵌 OEE Start Lot，Reply Code 收斂為 0/2/60/61/62/90。 |
| 7 | 2026-05-06 | 南茂科技 | CASE-20260506-001 | V3.33.899.18 | `iosetview.cpp:789` | 全客戶（僅 Caption） | ✅ 20260506（註解直接寫 CASE-20260506-001） | 500KG 機台 IO 畫面 Auto Docking 標籤 240KG/360KG 改 500KG Y/X。 |
| 8 | 2026-05-12 | 上海安靠 | CASE-20260508-001 | V3.33.899.19 | `aoutarm9045_2x8_8.cpp:411-422`（GetNowShuttleMode_2x8_8） | 全 2x8_8 機台 | ✅ 20260512 ×3 | mode 20201/20300/20301 三處 row index 筆誤 → OutArm 不下壓取 Shuttle 最右站殘料。 |
| 9 | 2026-05-15 | 南茂科技 | CASE-20260515-001 | V3.33.899.19 | `cContactCT.cpp` ×3、`main.cpp:30033` | 全客戶 | ✅ 20260515（`main.cpp` 那處後於 20260820 重新標註） | Yield Chart site 標籤空白：`'A'+ChangeToFloatNonPcnt()` 經 varargs 傳 `%c`，補 `(int)` 轉型。 |
| 10 | 2026-05-18 | 全智 | CASE-20260504-001 | V3.33.899.20 | `cContact.cpp:10548,10774` | `CC_GIGAS` + auto height | ✅ 20260518 | Auto K 完成後 Index 回 Shuttle 掉料無警示：放真空前 recheck。 |
| 11 | 2026-05-18 → 05-22 | 全智 | CASE-20260504-002 | V3.33.899.22 | `cContact.cpp`（20260518 ×2、20260522 ×4） | `CC_GIGAS` + `CONTACT_TEST` | ✅ 20260518/20260522 | Contact Test 未滿 site 允許部分上料；socket→shuttle 放回也保留掉料/真空保護。 |
| 12 | 2026-05-19 | Greatek | CASE-20260519-001 | V3.33.899.20 → .21 | `ProductionInfo/ProductionInfo.cpp:4952`、`main.cpp:28270` | `CC_Greatek` | ✅ 20260519 | RunSite 改由 V812 channel map 輸出；並把 info.txt 重寫移到 site 狀態同步之後。 |
| 13 | 2026-05-20 | 力成PTI | CASE-20260520-001 | V3.33.899.21 | `CosFunction.cpp`（FUNC_CC_PTI 開 `bUseFix3FullTray`） | `FUNC_CC_PTI` + `FIX3_FULL_PLACE==Fix3K_Uninstall` | ✅ 20260520 | 導入 E55 Fix3 Full Tray customer gate（HT-70 P01-1 類自動極限）。 |
| 14 | 2026-05-21 | 南茂科技 | CASE-20260519-001 | V3.33.899.22 | `KYECFTP/FTPClient.cpp:3416` | `CC_ChipMos` | ✅ 20260521 | HEAD 加熱模式上傳 Server 時，依 heat mode 遮蔽未使用的 Chamber 溫度欄位。 |
| 15 | 2026-05-25 | 力成PTI | CASE-20260521-001 | V3.33.899.22 | `main.cpp:24003`（SetFirstTrayCheckOnUnloader） | 無 | ✅ 20260525 | 切換工作檔後同步 Auto1/2/3 PASS Bin 的 First Tray Check position。 |
| 16 | 2026-05-25 | 力成PTI | CASE-20260521-002 | V3.33.899.22 | `main.cpp:25604`、`uLotInfo.cpp:95,119,1612,7256` | PTI 條件 | ✅ 20260525 | Start Lot 前依 Start Mode 自動補 Run Mode；補 PTI Lot Start trace。 |
| 17 | 2026-05-28 | 全智 | CASE-20260528-001 | V3.33.899.23 | `main.cpp`（sbYieldClick） | 全客戶（Security [39]） | ✅ 20260528 | Tools → Yield Monitoring 入口補權限卡控。 |
| 18 | 2026-05-28 → 05-29 | 力成PTI | CASE-20260527-001 | V3.33.899.24 | `uLotInfo.cpp`（20260528 ×1、20260529 ×1） | PTI Offline + Operator | ✅ 20260528/20260529 | Tester Mode Offline 時 Operator 開批卡控；提示文字改 "Please check the tester mode is connected"。 |
| 19 | 2026-06-02 | 欣銓 | CASE-20260602-001 | V3.33.899.22 | `ainarm9045.cpp:7271,7302`(共 10 處)、`acatchtray.cpp` ×3 | **原本無閘門**（後於 20260803 收回 `CC_ARDENTEC`） | ✅ 20260602 | Dummy run 手動取盤／空盤殘留 hang：新增 InArm load-stage 等待逾時 watchdog（30s/60s → WAR09101）。 |
| 20 | 2026-06-05 | 力成PTI | CASE-20260605-001 | V3.33.899.25 | `cSortCT.cpp` ×4、`KYECFTP/FTPClient.cpp` ×4 | PTI | ✅ 20260605 | Tray Feed 後不再自動彈 Clear Sort Count；FTP timeout 20s→5s 且失敗不彈 modal。 |
| 21 | 2026-06-08 | 全智 | CASE-20260608-001 | V3.33.899.26 | `cContact.cpp` ×10 | `CC_GIGAS` + `bContactTestICDropGuard` | ✅ 20260608 | 雙 Arm 滿 site Contact test 部分上料誤判掉料；掉料中止前還原本臂 pick 暫存避免 IndexHasIC 互鎖鎖死。 |
| 22 | 2026-06-09 | 力成PTI | CASE-20260609-001 | V3.33.899.26 | `KYECFTP/FTPClient.cpp`（UploadFileToServer2）×4 | PTI 條件式 | ✅ 20260609 | Lot End FTP 上傳主路徑：timeout 5s、失敗只記 log、不讀 `NMFTP2->LastErrorNo` 避免 Abnormal program termination。 |
| 23 | 2026-06-11 → 06-12 | 力成PTI | CASE-20260611-001 | V3.33.899.28 → .29 | `cTrayAssignment.cpp:353-382`、`uLotInfo.cpp`（20260611）＋**新檔** `FTPUpload/uFtpUploadThread.cpp`/`.h`、`Config.h`、`cprod.cpp`、`main.cpp`、`uLotInfo.cpp`（20260612） | `bFtpUploadBackground`（PTI 預設 true，其他 false） | ✅ 20260611/20260612 | (1) Fix2/Fix5 下半盤方向跟隨上半盤；(2) Lot End FTP 上傳背景執行緒化（fire-and-forget）。**含新增模組目錄，搬移成本最高的一案。** |
| 24 | 2026-06-18 | 南茂科技 | CASE-20260618-001 | V3.33.899.34 | `Motor/mymotor.cpp` | `CC_ChipMos_ZHUBEI` / `CC_ChipMos_TAINAN` | ✅ 20260618 | e360KG 的 `iCheckZ` 由 4000 放寬到 11000（比照 `CC_GIGAS`），消除 Z1UpZ2Down1 誤報。 |
| 25 | 2026-06-23 | 全智 | CASE-GIGAS-20260622-001 | V3.33.899.29 | `main.cpp:5639`（移除 `#ifdef SOFT_SIMULTE`）、`MachineType.h` | `CC_GIGAS`（`bC08_1_CheckSocketSensorDetectON`） | ✅ 20260623 | C08_1 Socket sensor Start 互鎖被 `SOFT_SIMULTE` 整段剔除 → 正式 build 功能不存在。 |
| 26 | 2026-06-23 | 力成PTI | CASE-20260617-001 | V3.33.899.30 | `cConfiguration.cpp` ×4 | 無 | ✅ 20260623 | N06 Enable FTP 存不進去：`dtpO06NextTime->Date` 遇 1899-12-30 拋 EDateTimeError 中斷整個 SaveConfiguration。 |
| 27 | 2026-06-25 | 欣銓 | CASE-ARDENTEC-20260625-001 | V3.33.899.30 | `Config.h`、`HS_Function.cpp`、`Interface/TesterTCP.cpp` ×6、`Interface/TesterTCP.h` | `bN10_DailyUploadProdData` + `bN10_11_Enable_UploadFTPEventLog` | ✅ 20260625 | FTP 排程上傳 EventLog／Production Log；recipe 切換客製（CopyRecipeToTester / N06 硬化）。 |
| 28 | 2026-06-29 | 力成PTI | CASE-PTI-20260629-001 | V3.33.899.31 | `main.cpp:25367`（DumpMainFormSnapshot） | 無 | ✅ 20260629 | **純診斷強化**：State Record 加 Home/Buzzer/Modal 狀態 dump。無行為變更。 |
| 29 | 2026-06-30 | 力成PTI | CASE-PTI-20260630-002 | V3.33.899.32 | `mymessbox.cpp` ×2、`note.cpp`、`uLotInfo.cpp`、`HS_Function.cpp` ×2、`main.cpp`、`csystem.cpp` | PTI / N06 | ✅ 20260630 | Lot End 訊息框持續置頂；N06 JamAlarm FTP 上傳背景防呆 + timeout 30000→5000。 |
| 30 | 2026-06-30 | 力成PTI | CASE-PTI-20260630-003 | V3.33.899.32 | `cpublic.cpp` ×2、`cpublic.h`（ProductionDataLog `[Sort Summary]`） | `CC_PTI` | ✅ 20260630 | Sort Summary 補 Fix4/5/6 位置與 Total 加總欄。 |
| 31 | 2026-07-03 | 力成PTI | CASE-PTI-20260630-001 | V3.33.899.33 | `AutoClean/uCleaning.cpp` ×3、`note.cpp`、`main.cpp`、`cmydef.cpp` ×4、`cmydef.h` ×3、`HT9045.bpr` | 無（行為改全客戶，見 uCleaning gate） | ✅ 20260703；commit `594fce9` | Smart Auto Clean 不鎖機：`CheckSmartAutoCleanCanStart` 恆放行 + `pnlCleanSocket` 非阻塞提示 + `bACSmartNeedReset` 按 START 自動重置。 |
| 32 | 2026-07-03 → 07-09 | Greatek | CASE-20260703-001 | V3.33.899.32 → .34 | `acatchtray.cpp` ×8（IsTrayArmCatchTrayFail、3 處 JAM0610 SKIP、case 570 防抖）、`uhome.cpp` ×2（ProcessMotorHome case 400）、`csystem.cpp` ×3（CheckInitStartSuckStatus/DoReTesetStart/DoInitialStart） | `CC_Greatek` 全程 gating | ✅ 20260703；commits `b2ea143`/`cc7c6f9`/`6539e3b`/`89fdd2a` | ART TrayArm 夾爪到位 sensor 極性校正（🔴 高風險翻轉，**尚未實機回歸**）。 |
| 33 | 2026-07-06 | 甬矽 | CASE-FOREHOPE_NINGBO-20260706-001 | V3.33.899.34 | `ainarm9045.cpp`（GetInArmToLoaderPosition_Single） | 全客戶（非 AOA 路徑） | ✅ 20260706 | WAR0152：盤尾殘料單吸時最左吸嘴超 X 軟極限，改系統性可達性選嘴（pitch 不縮）。 |
| 34 | 2026-08-03 | 全智 | CASE-GIGAS-20260729-001 | V3.33.899.34 | `CosFunction.h`/`CosFunction.cpp:2789,4414`（新旗標 `bUseInArmLoadStageWatchdog`）、`ainarm9045.cpp:7263,7299` | `FUNC_CC_ARDENTEC` only | ✅ 20260803 | 把 20260602 新增的 InArm watchdog 收回只給欣銓，全智不再誤報 WAR09101。 |
| 35 | 2026-08-04 | 力成PTI | CASE-PTI-20260804-001 | V3.33.899.34 | `CosFunction.cpp` ×9、`CosFunction.h` ×6、`PowerSavingMode.cpp` ×5、`PowerSavingMode.h`、`cConfiguration.cpp` ×4 | `FUNC_CC_PTI`（5 個新旗標） | ✅ 20260804 | C05 溫控省電：LOT END + HALT 達時數斷加熱 relay，不切溫度模式。 |
| 36 | 2026-08-10 | 全智 | CASE-20260507-001 | V3.33.899.35 | `cprod.cpp:2535` | `CC_GIGAS` 分支 | ✅ 20260810 | `GaliPosRange` 預設值 10→50（下限維持 5），避免重灌後回到 10 復發。 |
| 37 | 2026-08-10 | 力成PTI | CASE-PTI-20260810-001 | V3.33.899.35 | `aoutarm.cpp:3125-3126` | 全客戶 | ✅ 20260810 | OutArm 超極限縮距救援改「夾到機構下限」，Head4 可放 Auto1 最左欄。 |
| 38 | 2026-08-10 → 08-11 | 力成PTI | CASE-PTI-20260810-002 | V3.33.899.35 → .36 | `aoutarm9045_2x4_4.cpp` ×10、`aoutarm9045.cpp` ×9/`.h` ×2、`aoutarm.cpp`、`RotateKit/aRotateKIT.cpp`、`RotateKit/aRotateKIT_Out.cpp` ×5、`main.cpp` ×2、`cConfiguration.cpp` | `e9045_2x4_4_14` | ✅ 20260810/20260811 | (1) `GetNowShuttleMode_2x4_4` 提前 return 2 遺失左右半 kit → double IC；(2) 旋轉站 stride `i2x2Suck_Out` 與 `SetInOutArmParameter_2x4_4` 不符；(3) 新增 OutArm 逐輪診斷 Log。 |
| 39 | 2026-08-10 | 甬矽 | CASE-FOREHOPE_NINGBO-20260810-001 | V3.33.899.35 | `cConfiguration.cpp:2522` | `CC_FOREHOPE_NINGBO` | ✅ 20260810 | 解鎖既有 `[I21-10] RT Mode Don't Run Site Mapping` 選項給甬矽。 |
| 40 | 2026-08-11 → 08-17 | 力成PTI | CASE-PTI-20260811-001 | V3.33.899.37 | `PowerSavingMode.cpp` ×8、`PowerSavingMode.h` ×2、`main.cpp`（ShowFunctions 列距）、`HT9045.bpr` | `CosFunction.bPowerSaveShowCaption`（`FUNC_CC_PTI`） | ✅ 20260811/20260817；git `83fb4f6`、`7de2301` | 省電模式狀態列黃底放大；並修 899.36 的倒數字串殘留（`ShowPowerSaveHighlight` guard 吃掉還原路徑）。 |
| 41 | 2026-08-17 | 甬矽 | CASE-FOREHOPE_NINGBO-20260813-001 | V3.33.899.36 | `cObserver.cpp` ×5 | 全客戶（顯示端） | ✅ 20260817 | Observer EventLog 解析改只以逗號斷欄，修 Alarm 行欄位錯位＋Filter 查無紀錄＋JAM 統計漏算。**EventLog 檔案格式未變。** |
| 42 | 2026-08-17 | 確安科技 | CASE-20260731-001 | V3.33.899.34 → .36 | `CosFunction.h:354`、`CosFunction.cpp:2848,4320`（`bCleanOutCanTrayEnd` / `bDisableAutoTrayFeed`）、`cTrayAssignment.cpp:238,728` | `CC_CYUEAN` / `FUNC_CC_CYUEAN` | ✅ 20260817 | (899.34) 開 Clean Out 後 `<Tray End>` 選項；(899.36) Auto Tray Feed 固定關閉並鎖定，不再跟隨工作檔。 |
| 43 | 2026-08-19 | Greatek | CASE-20260819-001 | V3.33.899.37 | `CosFunction.cpp` ×2、`CosFunction.h`、`ProductionInfo/ProductionInfo.cpp` ×6、`ProductionInfo/ProductionInfo.h` | `CC_Greatek`（`bHotAirByMOTemperatureMode`） | ✅ 20260819 | MO 工單 Temperature mode 含 HOT AIR 時自動開熱風（`ApplyHotAirByMOTemperatureMode`，OEE_StartLot 之後）。 |
| 44 | 2026-08-20 | Greatek | CASE-20260820-001 | **未出版號（in-progress）** | `ProductionInfo/ProductionInfo.cpp:5596` | 無 | ✅ 20260820 | `SettingBinCategoryFromServer` 迴圈上界由 `iBinSelectLength`(33) 改 `iStackFail_Pass_Length`(9)，修 24 次堆疊越界 AV。**issue.md 仍寫「未修改任何 V899 原始碼」— 見問題清單。** |
| 45 | 2026-08-20 | 力成PTI | CASE-PTI-20260824-001 | V3.33.899.37 | `cBinSel.cpp:1371,1395`（ReadFile，22 行）、`ProductionInfo/ProductionInfo.cpp:1965,1983`（防禦性 skip） | `CC_PTI` | ✅ 20260820 | 未指派 Category 的 Fix1/Fix4 預設改 Fail（不再顯示好品綠色）。ProductionInfo 端對 PTI 是死碼、對超豐是 no-op。 |

### 2.1 搬移優先注意事項（來自上表）

- **共用檔案熱點**（多案交疊、splice 衝突風險最高）：`main.cpp`(12 案)、`ProductionInfo/ProductionInfo.cpp`(6 案)、`CosFunction.cpp/.h`(5 案)、`uLotInfo.cpp`(4 案)、`cContact.cpp`(3 案)、`KYECFTP/FTPClient.cpp`(3 案)、`ainarm9045.cpp`(3 案)、`aoutarm*.cpp`(3 案)、`cConfiguration.cpp`(3 案)、`note.cpp`(3 案)、`csystem.cpp`(3 案)。
- **新增模組（非單純 hunk）**：`FTPUpload/uFtpUploadThread.cpp`/`.h`（#23）。
- **有相依鏈的案群**：
  - EventLog 引號線：#4（20260429 note.cpp）→ #41（20260817 cObserver.cpp）。**搬 #4 就必須一起搬 #41**，否則等於把回歸搬進 V910。
  - InArm watchdog 線：#19（20260602 無閘門）→ #34（20260803 收回 `CC_ARDENTEC`）。**兩者必須同波搬。**
  - PTI 省電線：#35（C05 本體）→ #40（狀態列＋倒數殘留回歸修正）。
  - PTI FTP 線：#20 → #22 → #23 → #29（逐次修同一條 Lot End 上傳路徑，最終態是 #23 的背景執行緒）。
  - Greatek TrayArm 線：#32 一次涵蓋 CASE-20260703-001／-002（-002 無自己的碼）。
- **未經實機回歸的高風險項**：#32（夾爪 sensor 極性翻轉）。
- **已入樹但決議不出貨**：#5（Multi EP 8ch）。

---

## 3. 「純分析／純設定」案件簡表（V899 產品線，**不搬**）

| # | 客戶目錄 | CASE-ID | 結案版號 | 不搬理由 |
|---:|---|---|---|---|
| 1 | Greatek | CASE-20260422-002 | N/A | Monitor 3 頻道為設計規格，Camera 4 預留；純說明。 |
| 2 | Greatek | CASE-20260507-001 | N/A | auto download error＝雲端路徑與 FTPAutomation 參數設錯，純設定。 |
| 3 | Greatek | CASE-20260703-002 | V3.33.899.32 | 程式修正全部掛在 CASE-20260703-001 名下，本案資料夾無 .cpp。 |
| 4 | Greatek | CASE-20260820-002 | — (in-progress) | BIN2/BIN3 手動放盤＝三個現成旗標現值皆 0，純設定。 |
| 5 | 上海安靠 | CASE-20260529-001 | N/A | 工單網路硬碟上下傳＝售服改客戶碼設定，無程式變更。 |
| 6 | 全智 | CASE-20260618-001 | V3.33.899.29 | 12-site 關單 site AutoClean hang up 未重現，明載 no code change。 |
| 7 | 全智 | CASE-GIGAS-20260715-001 | N/A | 2DID CCD 解碼耗時純資料分析（4.5s 中 96% 是機構時間）。 |
| 8 | 力成PTI | CASE-20260615-001 | 無版本 | WAR0707 設計性停機；只補 `Error\<語系>\WAR0707.dat` 資料層說明檔，非程式碼。 |
| 9 | 力成PTI | CASE-PTI-20260727-001 | V3.33.899.0 | ProductData 計數規則 Q&A，客戶認知正確，無需改碼。 |
| 10 | 力成PTI | CASE-PTI-20260817-001 | V3.33.899.36 | 省電 Lot start 復歸／System message 兩需求，客戶決定不實作；純讀碼評估。 |
| 11 | 南茂科技 | CASE-20260422-001 | V3.33.899.18 | Auto Contact 未送 SOT＝客戶選錯 contact mode，操作說明結案。 |
| 12 | 南茂科技 | CASE-20260511-001 | V3.33.899.18 | 明載 `duplicate_of: CASE-20260422-001`。 |
| 13 | 南茂科技 | CASE-ChipMos_TAINAN-20260716-001 | 無 | 不會加熱＝recipe「NS8000H change socket」旗標錯設，config-only。 |
| 14 | 南茂科技 | CASE-ChipMos_TAINAN-20260724-001 | V3.32B2_BETA07.810.8.01 | 客戶回退 810 自行解決，**899 側 6-Auto 路由缺陷未修**（見問題清單）。 |
| 15 | 南茂科技 | CASE-ChipMos_TAINAN-20260731-001 | V3.33.899.34 | JAM0560 假殘料＝切 16-Site 並勾「16 change 12 Direct Heater」，config-only。 |
| 16 | 欣銓 | CASE-20260512-001 | V899 | 評估後決議保留 ch16，不交付 8-EP 版本（取代 CASE-20260429-001）。 |
| 17 | 瑞昱 | CASE-20260423-001 | 無 | HotPlate Data Swap error 2 以 V899 模擬無法重現，客戶驗證 OK。 |
| 18 | 瑞昱 | CASE-Realtek-20260714-001 | V3.33.899.29 | Start Mode 差異＝`Gerneral.ini` CUSTOMER_CODE 894→833，純設定。 |
| 19 | 盛合晶微 | CASE-20260610-001 | N/A | RTC 視覺端字串缺 `@` 前綴；**且現場版本為 V3.33.904.4／驗證版 905.8，非 V899 樹**。 |
| 20 | 確安科技 | CASE-20260422-001 | N/A | 無動作無報警＝非軟體問題，移交 ME。 |
| 21 | 確安科技 | CASE-20260624-001 | V3.33.899.29 | 大尺寸 IC 取最右欄 CCW 超限，以升級 899.29 結案；**V899 樹內查無 20260624 的 //AI 註解**，判定無專屬碼變更。 |
| 22 | 確安科技 | CASE-20260729-001 | V3.33.899.0 | 良率卡控循環報警現況確認，明載無程式修改。 |
| 23 | 紹興長電 | CASE-20260622-001 | V3.33.899.0 | 測試機端 OS2000 90s 內未回 BINON，Handler 端無變更。 |
| 24 | 紹興長電 | CASE-JSI_HAOXING-20260723-001 | V3.33.906.1 | Contact Height 未完成校正；**且現場版本 V3.33.906.1，非 V899 樹**。 |
| 25 | 長沙安牧泉 | CASE-20260625-001 | 無 | OS Tester recipe 同步旗標未啟用；issue.md 明確聲明 `TesterTCP.cpp` 的 `//AI 20260625` 註解**不屬本案**（屬欣銓 CASE-ARDENTEC-20260625-001）。 |

### 3.1 排除（非 V899 樹的產品線，14 件）

| 產品／樹 | 案數 | CASE-ID | 排除理由 |
|---|---:|---|---|
| **HT172 Sorter**（`D:\HT172`，版號 V1.0.25.x，另一個 repo） | 13 | 力成PTI：CASE-20260422-001、CASE-20260512-001、CASE-20260525-001、CASE-20260601-001、CASE-20260617-002、CASE-PTI-20260623-001、CASE-PTI-20260626-001、CASE-PTI-20260626-002、CASE-PTI-20260629-002、CASE-PTI-20260706-001、CASE-PTI-20260709-001、CASE-PTI-20260714-001；台星科：CASE-WINSTEK-20260626-001 | 檔案是 `HT172.bpr` / `aLoader.cpp` / `aMagArm.cpp` / `aSortArm.cpp` / `MyBinDisp.cpp` / `Auto172.cpp` / `setup.cpp`，全部在 `D:\HT172`，V899 樹內不存在。`CosFunction.cpp`/`.h` 同名但屬 HT172 的副本。 |
| **HT160S**（`D:\HT160S`） | 1 | 京元竹南：CASE-KYEC_CHEN-20260715-001 | 檔案 `ht160s.cpp` / `aSortArm.cpp` / `LotWebApiClient.h` 等，V899 樹內不存在（已用 find 確認 0 命中）。此案含 20+ 個 commit（ab8080e、753b51b、b3f9653…），全部在 HT160S 樹。 |

---

## 4. 問題清單（資訊不足或需人為裁決）

### 4.1 「不確定」案件（4 件）

| # | 客戶目錄 / CASE-ID | 缺什麼資訊 | 建議取得方式 |
|---:|---|---|---|
| Q1 | Greatek / **CASE-20260422-001**（MAIN_STATUS_INQUIRE 無回應，V3.33.899.8，2026-04-23 結案） | issue.md 只有「已結案，V3.33.899.8 修正並提供安裝包」一行，**無修改檔案清單、無 03_fix 文件、V899 樹內查無 20260422/20260423 對應 SECS 相關 `//AI` 註解**（20260423 那批是 uCleaning/HandlerSys/HT9045.cpp 等別的工作）。已知只有 component=SECS_INTERFACE、tags=secs-gem,retry-logic。 | 需比對 899.7 → 899.8 的 SVN/git diff，或直接搜尋 V899 樹的 `MAIN_STATUS_INQUIRE` 處理點確認是否留有未標註的變更。 |
| Q2 | 上海安靠 / **CASE-20260511-001**（9046CN OutArm Site4 G 吸嘴 hang，V3.33.899.18） | issue.md 的「待補資料」自己列了 4 項未完成：修正檔案／函式／`//AI` 註解位置、9046CN layout、是否與 CASE-20260508-001 同根因。**且版號矛盾**：本案 899.18（2026-05-11 交付），但 `aoutarm9045_2x8_8.cpp` 的修正註解是 20260512（→899.19），時間上不可能是同一筆。 | 需確認 899.18 當天是否另有一筆未標註的修正，或本案其實是 899.19 才修好而 issue.md 版號記錯。 |
| Q3 | 力成PTI / **CASE-20260514-001**（Stoker Fix3 MOutArmX over soft limit，V3.33.899.19，2026-05-14） | issue.md 只有「已修正並提供安裝包 V3.33.899.19，結案」，**無檔案清單、無 03_fix 內容（僅 .gitkeep）、V899 樹內查無 20260513/20260514 對應 OutArm X 極限的 `//AI` 註解**。同主題的 CASE-20260520-001（E55 Fix3 gate，`CosFunction.cpp` //AI 20260520）是 899.21，不是同一筆。 | 需確認 899.19 是否真有針對本案的碼變更，或當初是靠 CASE-20260508-001 的 `aoutarm9045_2x8_8.cpp` 修正順帶解掉。 |
| Q4 | 全智 / **CASE-GIGAS-20260820-001**（Arm yield 定期轉 snapshot，status=**new**） | 需求本體尚未實作（issue.md 只有 intake 一列 + 可行性評估）。但 V899 樹內 `main.cpp:30011,30033` 有 `//AI 20260820` 的 O06 週期 log **Arm 良率修正**（補漏掉的 Row A、修標籤重複），**沒有任何 case 認領這兩處變更**。 | 需確認 `main.cpp` 這兩處 20260820 變更歸屬哪個 case（或屬無 case 的內部工作），以及是否要一併搬進 V910。 |

### 4.2 issue.md 與 V899 樹不一致（需更正記錄）

| # | 案件 | 不一致內容 |
|---:|---|---|
| P1 | Greatek / CASE-20260820-001 | issue.md 2026-08-20 Weekly Action 明寫「**未修改任何 V899 原始碼**…建議修正為單行改 `i<iStackFail_Pass_Length`（尚未執行，等指示）」，但樹內 `ProductionInfo/ProductionInfo.cpp:5596` **該修正已經套用並帶 `//AI 20260820` 註解**。案件狀態仍是 `in-progress`、無出貨版號。→ 搬移總帳要收，且 case 記錄要補。 |
| P2 | 欣銓 / CASE-20260429-001（Multi EP 8ch） | issue.md 寫「所有變更皆加 `//AI(ht9045-v899) 20260429` 註解」，但樹內 20260429 只有 `main.cpp` ×1、`note.cpp` ×2（後者屬甬矽 EventLog 案）。Multi EP 的實際註解日期是 **20260430**（ContactForce/HandlerSys/adam6024/database）與後續 20260504/0505/0511/0522/0526 的延伸。→ 依 issue.md 日期抓 diff 會抓錯。 |
| P3 | 南茂科技 / CASE-20260515-001（YieldChart） | issue.md 寫「`cContactCT.cpp` 3 處 + `main.cpp` 1 處，補 `//AI 20260515`」，但樹內 `main.cpp` 該處的註解日期是 **20260820**（註解本文自述「故延續 20260515 的 ChangeToFlo…」）。→ 該處與 CASE-PTI-20260824-001 / O06 Arm 良率修正在同一區塊交纏，切 hunk 要小心。 |
| P4 | 確安科技 / CASE-20260624-001 | issue.md 有 `//AI(ht9045-v899) 20260624` 的字樣（出現在 02_analysis 文件），但 **V899 樹全樹 grep `20260624` 零命中**。判定為分析文件中的「建議註解」而非實際落地的修改。→ 本盤點歸類為純分析，若判斷有誤請提出。 |

### 4.3 V899 已知缺陷但**未修**（MG 搬移時要決定是否一併處理）

| # | 來源案件 | 未修內容 |
|---:|---|---|
| D1 | 南茂科技 / CASE-ChipMos_TAINAN-20260724-001 | 899 的 6-Auto 分類路由層（`acatchtray.cpp:373-451` `WhichAutoNeedTray()`，`iT6CatData`/`bLinkTo6Tray`/`iIfErrorT6`：810 為 0 筆、899.34 為 179 筆 / 28 檔）會**靜默把料盤放到錯的 Auto 站且不報警**。客戶已回退 810。issue.md 明載「899 側路由問題未修，尚待另案處理」。 |
| D2 | 南茂科技 / CASE-ChipMos_TAINAN-20260731-001 | `acarry.cpp:1108` 的「跳過第 0、7 欄」殘料遮罩被鎖在 `CC_KYEC_JCTHIU`/`CC_KYEC_LEE`/`bSPILFunction`/`b16Direct12Shuttle` 之後。建議永久修正＝遮罩改為只看 site map、移除客戶碼閘門，**未做**。 |
| D3 | Greatek / CASE-20260703-002 | 啟動自檢 WAR0615 本身（`csystem.cpp CheckInitStartSuckStatus`）**未直接修改**，屬稽核 finding B 延伸項（共用/高風險暫緩）。 |
| D4 | Greatek / CASE-20260703-001 | 夾爪 sensor 極性校正屬 🔴 高風險翻轉，**尚未實機回歸驗證**；結案定性為「開發完成、交付供部署」。 |
| D5 | 全智 / CASE-GIGAS-20260729-001 | `TQPF_Timer`（`myTimer.cpp`）是純 QPC 掛鐘，無 Pause/Stop 概念，把停機＋OP 排除時間算成卡站；本次只做客戶碼隔離，**watchdog 邏輯本體未重寫**（欣銓側仍在跑有缺陷的版本）。另 WAR09102/WAR09107 訊息分家也未做。 |

### 4.4 全域缺口：V899 樹有大量**無 case 對應**的程式變更

盤點時比對「Weekly_AI 88 個 case」與「V899 樹全部 `//AI` 註解日期」，發現以下日期的變更**在 Weekly_AI 找不到對應案件**（多為內部開發／重構，非客戶案）：

```
20260316(Copilot/general) 20260331 20260401 20260405 20260407 20260408 20260409
20260410 20260414 20260415 20260416 20260417 20260421 20260422 20260424
20260430(部分) 20260504(部分) 20260505 20260506(部分) 20260511 20260513
20260514 20260522(部分) 20260526 20260604 20260612(部分)
```

其中影響面較大的幾群：

- **`Common/PickPlanner/` 整個目錄**（`cArmPickPlan`、`cInArmLoaderPickPlanner`、`cOutArmPlacePlanner`，//AI 20260514）— 註解自述 "Phase 3/4 scaffold (default off)"、"currently unreachable"。**是死骨架，搬移前先確認 V910 是否需要。**
- **`OCR.cpp` / `OCRInsp.cpp` / `TfAOILaserScan.cpp` / `cTrayMapping.cpp`**（20260505~20260526，累計 100+ 處）— OCR/AOI/TrayMapping 大改，無 case。
- **`AutoClean/AutoClean.cpp` + `cCleanKitPickPlan.h`**（20260407~20260417、20260514）、**`cSiteUseManager.cpp/.h`**（20260405/20260407）、**`MyLaneIo.cpp/.h`**（20260424）、**`Monitor/MonitorInterface.*`**（20260422）、**`HandlerSys.cpp`**（20260423 ×16）。

> **這代表「照 Weekly_AI case 清單搬」會漏掉相當比例的 V899 變更。** 建議 MG 總帳以「V899 樹 `//AI` 註解全集」為母體，Weekly_AI case 只當作**歸因與客戶碼線索**的來源。

---

## 5. 方法與限制

- 全程唯讀 Weekly_AI；本次只寫出這一個檔案。
- 客戶目錄名一律照磁碟原樣記錄（`Greatek` 的 issue.md `customer` 欄是「超豐」、`確安科技` 的 `customer` 欄是「確安」，兩者不一致屬既知現象，未自行改名）。
- 「涉及檔案」以 issue.md 明列的修正檔案為主，並用 V899 樹 `//AI` 註解交叉確認行號；`02_analysis` 文件中出現的檔名多為「分析時讀過的檔」而非「改過的檔」，**未**採計。
- `//AI` 註解數量僅代表註解則數，不等於變更行數；實際 hunk 範圍仍需逐檔取 diff。
