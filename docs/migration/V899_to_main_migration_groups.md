# V899 to Main 移轉計畫：14 大移轉群組

> 來源版本 SOURCE：HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422
> 目標主版 TARGET：HT9011UC_Code_V3.33.905.8_20260608_Ken_NEW_Ifor_Jimmy
> 逐行清單（499 筆 //AI）：docs/migration/V899_to_main_manifest.csv
> 建立日期：2026-06-09

---

## 主版 905.8 基底比對結果（2026-06-09 實測）

| 檢查項 | 主版狀態 | 影響 |
|--------|----------|------|
| 我的 //AI 註解總數 | 僅 4 筆已併入（其餘 495 筆全缺） | 近乎全量移轉 |
| 已併入的 4 筆 | aoutarm9045_2x8_8.cpp x3（20260512 outarm fix）、asendic_Loader.cpp x1（20260409 interlock guard） | 移轉時跳過避免重複 |
| DOUBLE_EP_MULTI | 0（無 Multi EP 模式） | 群組 1 需全量移植 + 確認 enum |
| INSTALL_DOUBLE_EP | 11（有 Double EP base） | Multi EP 掛在既有 Double EP 之上 |
| LOAD_Y_USE_MOTOR | 0（全缺） | 群組 6 全量移轉 |
| bRemoteLotStart | 0（全缺） | 群組 9 全量移轉 |
| 新檔 cSiteUseManager / PickPlanner / CKPP / cBootLog | 不存在 | drop-in |

> 注意：905.8 含 Ifor（Multi EP 作者），但主版實測沒有 DOUBLE_EP_MULTI enum，代表該功能未合併進此分支。群組 1 仍需移植，且須先確認 INSTALL_DOUBLE_EP enum 定義是否齊備。

---

## 移轉型態定義

| 型態 | 意義 | 套用方式 |
|------|------|----------|
| Type A 新檔 drop-in | 主版完全沒有的獨立檔 | 直接複製 + 加入 .bpr |
| Type B 共用 header/旗標 | 全域定義、enum、CosFunction 預設值 | 必須最先套用 |
| Type C 跨版本 port | 自 V874.3 / V896 移植 | 先 diff 主版基底 |
| Type D 在地修正/客戶功能 | 多以 CUSTOMER_CODE 守門 | 逐段套用 |

套用依賴順序：B then A then C then D，每群組套完即 BCB6 build 驗證。

---

## 群組 1：Multi EP 全埠 DOUBLE_EP_MULTI｜高風險｜Type C+B

- 來源：port 自 V874.3 / V896（Ifor）。ADR-0004、SPEC-V899-MultiEP-FullPort
- 檔案：adam6024.cpp/.h、cContact.cpp、ContactForce.cpp/.h、cSetUp.cpp、cConfiguration.cpp、HS_Function.cpp、database.cpp、HandlerSys.cpp、iosetview.cpp、csystem.cpp、AutoClean/uCleaning.cpp、AutoClean/AutoClean.cpp
- 主版狀態：有 INSTALL_DOUBLE_EP base、無 DOUBLE_EP_MULTI
- 移轉前必確認：主版 enum 是否含 DOUBLE_EP_MULTI；Ifor 是否已有其他形式 Multi EP（避免雙實作衝突）→ 列入不確定清單
- 驗證：NORMAL / INDIVIAL / MULTI 三模式 build；SwMultiEp ON/OFF 路由；APAX S0/S1 Arm1/Arm2 輸出
- **狀態 2026-06-11（option B 單壓力核心 ?完成，每檔編譯 EXIT=0）**：cmydef.h enum、adam6024.cpp/.h（helpers + APAX MULTI write + calc wrap + routing + MultiTransferKG）、ContactForce.cpp（SLKIndClass i*8+j build/ReadFile/WriteFile + visibility）、cContact.cpp、cConfiguration.cpp、AutoClean/uCleaning.cpp、iosetview.cpp、cSetUp.cpp（含 bIndEPSLK 保留 RogerYang AMKOR/8EP 加 MULTI else-if）、csystem.cpp（WAR0329 wrap）、main.cpp（APAX reset + SwMultiEp DO/DieForce）、HS_Function.cpp/.h（ReadMultiEP）全部已搬。
- **deferred 交主版（option B 不搬）**：DieForceOneByOne dual-force 路徑（`THTDieForceOneByOneSLKClass`/`DieForceOneByOneSLKClass`，主版無對應 DFM）。
- **no-op 保留 RogerYang**：database.cpp L785-795 `iIndEPCnt==8` 分支不動（wrap-don't-rollback）。

## 群組 2：AutoClean CKPP + PickPlanner 架構｜高風險｜Type A+D

- 來源：新架構 + 重構。ADR-0005
- 新檔 Type A：AutoClean/cCleanKitPickPlan.h、Common/PickPlanner/IArmPickPlanContext.h、cArmPickPlan.cpp/.h、cInArmLoaderPickPlanner.cpp/.h、cOutArmPlacePlanner.cpp/.h
- 編輯檔 Type D：AutoClean/AutoClean.cpp（25 筆）、ainarm9045.cpp（Phase 3 hook）
- 驗證：bUseCKPP 預設 off（legacy 路徑）；8 個 call site API 不變；group-skip 不插 phantom IC

## 群組 3：InArm HP 反碎片化 CompactSearch｜中風險｜Type A+D

- 新檔 Type A：cSiteUseManager.cpp/.h
- 編輯檔：ainarm9045_2x4_8.cpp、ainarm9045_1x4_4.cpp、ainarm9045_1x1_1.cpp、cinitial.cpp（Phase4 init SiteUseMgr）
- 設計教訓：CompactSearch 必用 iNeedCol=1, iNeedRow=1 單格搜尋；HP 用 Prod.HotPlateForm[0].iXPitch int；WillExceedMotorLimit 已停用
- 驗證：1x4_4 stride 放料無碎片；HAS_NULL_IC 不佔 HP 格位

## 群組 4：InArm Loader 看門狗｜中風險｜Type D

- 案件：欣銓 CASE-20260602-001
- 檔案：ainarm9045.cpp（case 10/15 no-tray + empty-tray watchdog）、acatchtray.cpp（case 148 LoaderY 歸位）
- 主版狀態：asendic_Loader.cpp 0409 interlock guard 已併入（跳過）
- 驗證：no-tray 30s / empty-tray 60s then WAR09101 K_RETRY；tray 回來/auto-skip 兩路 disarm

## 群組 5：TrayMapping / LaserScan / Home 修正｜中風險｜Type D

- 檔案：cTrayMapping.cpp/.h、TfAOILaserScan.cpp/.h、uhome.cpp、Motor/mymotor.cpp
- 重點 HOTFIX：RestoreLoaderYSpeedRaw 永遠 SetSpeed(100)；auto-home HomeFlag（==1 成功 / ==2 失敗）；用 MotorHome(false) 不用 Home()
- 驗證：30s home timeout 不再誤觸；form 關閉 MLoaderY 不龜速漂移

## 群組 6：Stepper Y 分派 LOAD_Y_USE_MOTOR｜低風險｜Type B+D

- 檔案：asendic.cpp、uMotorTest.cpp、database.cpp、HandlerSys.cpp
- 主版狀態：LOAD_Y_USE_MOTOR 完全不存在 then 全量移轉（含 INI 讀寫 EMPTY/COLOR/AUTO1~3_Y_USE_MOTOR）
- 注意：cassette mode 與 MAuto1Y/MAuto2Y 競用 then force-disable LOAD_Y_USE_MOTOR[3]/[4]
- 驗證：6 軸 stepper dispatch；Motor Test 顯示；Boat Carrier 不衝突

## 群組 7：Gigas Contact 掉料防護｜中風險｜Type B+D

- 案件：全智 CASE-20260608-001、CASE-20260421
- 檔案：cContact.cpp（partial pick 豁免、HAS_IC 守護、iFTestBackItem/iBTestBackItem 還原）、CosFunction.cpp/.h（FUNC_CC_GIGAS 旗標）
- 守門：CUSTOMER_CODE==CC_GIGAS，非全智行為不變
- 驗證：全智雙 Arm 部分上料不誤報掉料；非全智 regression 不變

## 群組 8：PTI Lot Start + FTP｜中風險｜Type D

- 案件：力成 PTI CASE-20260605-001、CASE-20260525
- 檔案：uLotInfo.cpp、cSortCT.cpp、KYECFTP/FTPClient.cpp、CosFunction.cpp、main.cpp
- 重點：FTP TimeOut 20000 to 5000、移除 modal 改 MyDBIProcess log；btnClearCount 手動/程式分流；PTI RunMode auto-normalize
- 依賴：FTPClient 需 include mymessbox.h
- 驗證：Lot End 不誤清計數；FTP fail 不卡停機 modal

## 群組 9：OEE / Remote Lot Start 超豐 Greatek｜中風險｜Type B+D

- 案件：F899-008
- 檔案：Automation/auto9045.cpp/.h、Automation/automation.cpp、Command.cpp、CosFunction.cpp/.h
- 主版狀態：bRemoteLotStart 完全不存在 then 全量移轉
- 重點：HTSET 720 Remote Start/End；LotInfo_REQUEST 內嵌 OEE Start Lot；errMsg 關鍵字分流
- 驗證：CC_Greatek + bOEEFunction 才走內嵌；非超豐不受影響

## 群組 10：OCR 模擬 + 觸發模式｜中風險｜Type D

- 檔案：OCR.cpp/.h（35 筆）、OCRInsp.cpp/.h（35 筆）
- 重點：OCR Area Simulation UI 獨立 tab；Bypass AOI（無 vision PC 跑 handler motion）；iOCRTriggerMode（switch/SE8/command）；per-loop log
- 守門：模擬旗標生產時關閉；bOCRSimBypassAOI 預設 off
- 驗證：模擬不影響生產 OCR；Lot started 不可進模擬

## 群組 11：Yield Monitoring 權限 / RunSite｜低風險｜Type D

- 案件：全智 CASE-20260528-001
- 檔案：uYieldMonitoring.cpp、main.cpp（sbYieldClick [39] 權限）、HS_Function.cpp（GTK info.txt save）
- 驗證：Yield Monitoring 入口權限檢查；low-yield auto site-off 後存 RunSite

## 群組 12：IO 錯誤訊息售服化｜低風險｜Type D

- 檔案：MyLaneIo.cpp/.h（BuildIOErrMessage）
- 重點：log 維持簡短供解析；UI 用 BuildIOErrMessage 提供 5 段完整修法；mn200 DEV_INF enum 翻成售服字串
- 驗證：IO 錯誤 UI 顯示完整；log 解析格式不變

## 群組 13：BootLog 當機診斷｜低風險｜Type A+D

- 新檔 Type A：Public/cBootLog.cpp/.h
- 編輯檔：HT9045.cpp（WinMain checkpoints + 例外捕捉）
- 驗證：24V/硬體未就緒當機時有 boot log；正常啟動不受影響

## 群組 14：Misc 小修彙整｜低風險｜Type D

| 子項 | 檔案 | 案件/說明 |
|------|------|-----------|
| 南茂 site label 強制 int | cContactCT.cpp | 20260515 Yield Chart Aa/Ab 空白 |
| Arm2 offset typo | uTemp_Set.cpp | 20260417 [1][0][1] subscript |
| outarm 2x8 row index fix | aoutarm9045_2x8_8.cpp | 主版已併（跳過） |
| barcode 2x2 mapping | BarCode/BarCode.cpp | 20260519 |
| Tray Edit 截圖防呆 | uTrayEditForm.cpp | 20260519 |
| AutoDeviceEjection 面板 | cShowBinSelect.cpp、database.cpp | 20260417 |
| Greatek 監控 Ch3 | csystem.cpp、Monitor/MonitorInterface.cpp/.h | 20260422 |
| iosetview RAII / null guard | iosetview.cpp | 20260511 |
| 500KG OTD label | iosetview.cpp | CASE-20260506-001 |
| dummy P18 bypass | acatchtray.cpp、asendic_Auto.cpp、csystem.cpp | 20260513 |
| ARDENTEC OCR home bypass | uhome.cpp | 20260504 |
| SortingBinTray CleanOut | CosFunction.cpp | 20260410 FOREHOPE_NINGBO |
| MAIN_STATUS_INQUIRE | Automation/automation.cpp | 20260422 |
| 其他零星 | note.cpp、Public/HTEditList.cpp、ProductionInfo/ProductionInfo.cpp/.h、SECSGEM/uHGemHT9045_SV.cpp、Config.h、cmydef.h/.cpp | 逐筆見 CSV |

---

## 移轉鐵律（必守）

1. Big5 編碼：所有含中文的 .cpp/.h，永不用 replace_string_in_file/create_file，一律 PowerShell Big5 GetEncoding(950)。
2. 依賴順序：Type B header/旗標 then Type A 新檔 then Type C port then Type D 在地修正。
3. 逐群組 build：每套完一群組跑 BCB6 build，綠燈才進下一群組。
4. 跳過已併 4 筆：outarm 2x8 x3、asendic_Loader interlock x1。
5. sprintf 型別安全：HP 相關用 int 欄位不用 double。
6. 不確定先擱置：移轉中遇不確定先記錄，最後一併交付使用者裁決。

---

## 待確認清單（移轉前/中）

### 已查證（2026-06-09 實測，確定事實）
- [x] 主版 enum 僅 `DOUBLE_EP_NORMAL=0, DOUBLE_EP_INDIVIAL`（ConfigType.h:80），**無 DOUBLE_EP_MULTI**。→ 群組 1 須先在 enum 加 MULTI 值（Type B）+ 全量移植（Type C）。主版完全無 Multi EP base，只有 Individual。
- [x] 主版 Galil index（myGALILmotor）= **0**，主版無 RogerYang Galil 變更。→ 群組 3 套 cinitial 時**只取我的 SiteUseMgr init 那一筆**，不可把 V899 內 RogerYang Galil 基底一起帶入。
- [x] 主版專案檔 = `HT9045.bpr`（根目錄）。→ 新檔需手動加入 .bpr。

### 仍需使用者裁決（不確定，最後一併處理）
- [x] **群組 1 重大決策（已定案 2026-06-11）**：採 option B＝只搬單壓力核心。V899 Multi EP 單壓力路徑已整包移進 905.8（每檔編譯 EXIT=0）；dual-force DieForceOneByOne 路徑 deferred 交主版負責人 SVN 三方合併（主版缺對應 DFM）。
- [ ] 群組 1 若決定移植：需確認主版 APAX/ADAM 硬體層（adam6024）與 V874.3 來源是否同基底，避免 register 對應錯位。
- [ ] 主版 905.8 確切衍生基底（影響哪些 Ifor 基底已存在）。
