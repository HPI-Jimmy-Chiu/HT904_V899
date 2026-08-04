# RECON_GateA_FormShow — `TfMain::FormShow` 逐段 gating 分類（GA-0 決策 4 材料）

> AI(W906-GateA-0) 20260804: 本檔是 GA-0 派出的唯讀 recon agent（Sonnet 5 high）產出，
> 供 GA-3（main.cpp 啟動鏈垂直切片）的翻譯 brief 直接取用。
>
> **主迴圈抽查驗證**（照「agent 論證要開 golden 對字面」紀律，抽承重宣稱）：
> - ✅ `SOFT_SIMULTE` 在 ported `MachineType.h:48` 確為 `//#define SOFT_SIMULTE`（關閉）。
> - ✅ `TMyQueue10::SetAliasAndTask` 確為兩行賦值（ported `cpublic.cpp:1075-1079`；
>   宣告在 `cpublic.h:261`——agent 引 259，off-by-2，非承重）。
> - 其餘逐段行號未逐一複核；**agent 自列的 UNCERTAIN 清單（§5 尾）在 GA-3 動筆前必須逐一核實**。
>
> 範圍：golden `main.cpp:9141-11365`（FormShow，inclusive 2,227 行）；`InitialOK=true` @ :10464。
> golden `main.cpp` 全檔無任何 `#if 0`——SKIP-GATE 類以「深客戶碼分支／純外觀」為準。

## 三個關鍵前置發現

1. **`SOFT_SIMULTE` 是關的**：FormShow 內所有 `#ifndef SOFT_SIMULTE`（真實硬體）分支目前
   **會編譯**、`#ifdef`（模擬）分支不會。GA-3 不翻轉此旗標（與全樹既有姿勢一致：真硬體分支
   照翻，硬體物件本身走 Sim HAL/no-op backend）。
2. **`SetAliasAndTask`×281 是診斷登記非 IO 綁定**：`TMyQueue10`（500 筆環狀任務狀態歷史，
   消費端 `csystem.cpp` `QueueTaskList[].CheckTaskChange()`）。可直接忠實翻譯；唯一前置是
   281 個目標任務變數指標在編譯期存在（多數住已翻的 arm 模組；少數掛 facade 欄位要補）——
   是「補欄位檢查清單」不是邏輯風險。
3. **FormsFacade seam 比預期完備**：`forms/fMain.h` 已現成提供 `LookForFile`（含 callcount）/
   `LoadRunModePicture`/`SetStartModeData`/`LoadTestModePicture`/`ShowTestHeadComp`/
   `ChangeLevelAttr`/`Clarn_Data`/`LightOn`/`hanaART->IsHanaArtAvailable` 等 no-op seam。

## 統計（分母 2,227 行）

| 分類 | 行數 | 佔比 | 意義 |
|---|---:|---:|---|
| TRANSLATE | ~1,356 | 60.9% | GA-3 忠實翻譯（下層基礎 common/cprod/asendic/myswitch/myTimer/myGALILmotor/acarry/database/uruncontrol 皆已翻） |
| SEAM | ~592 | 26.6% | 掛 no-op/counter seam，**保留呼叫序**（golden 對先後次序敏感） |
| SKIP-GATE | ~279 | 12.5% | 深客戶碼分支＋純外觀（`MainFormSizeToEpson`×2、Rotate Kit 版位、KYEC_LEE 版位、JCET dummy site 等） |

## 三個 thread 啟動點（GA-0 決策已定形）

| Thread | golden 行 | 條件 | ported 狀態 | Gate A 處置 |
|---|---|---|---|---|
| TRunControl（主 pump） | :10603 `StartMainThread()` | 無條件 | `uruncontrol.cpp` 已翻 | **D-A0-3：UI-thread timer 等價替代** |
| THeaterThread | :10138 `StartHeaterThread()` | 無條件 | `uHeaterThread.cpp` 未翻 | SEAM（no-op+counter），blast radius 見下 #2 |
| ScanBtn（CCD） | :10568-10591 `new ScanBtn(false)` | `if(REAL_TIME_CCD)`（執行期 bool 非巨集） | `ScanBtnThread.cpp` 未翻 | SEAM；離線 REAL_TIME_CCD=false 天然 dormant |
| （第 4 個，agent 額外發現）MyPad232Thread | :10142 | `iControlPanelMode==1` | 未核實 | SEAM，條件預設關 |

## Blast radius——被 SEAM/SKIP 跳過的全域狀態寫入（依風險排序，GA-5 首燈與 Gate B 要回頭看）

1. **`ServoOnAllMOT()`**（:10165）：全 `MOT[]` 伺服致能。**明確 no-op+計數器，不偽造 true**。
2. **`StartHeaterThread()`**（:10138）：加熱安全迴圈整條不跑，`bHeatOverTenErrorOK` 等停初值。
3. **`ScanBtnThd` + `SendCommToVision()`×6**（:10568-10591）：CCD 燈控/AutoTeach 訊號不送。
4. **`FSECS->GemInitial` + `bReadSecsGemData`**（:10734-10747）：SECS 上線靜默失效。
5. **五個 `*BreakerOFF()`**（:9570-9575）：馬達斷路器繼電器「先關閉」的電氣安全初始化被跳過
   ——方向是安全問題非功能問題，Gate B 優先補。
6. `fShowBinSelect` Bin 顯示/AutoClean 計時（:10186-10211）；7. ATC 全家族（散落 6+ 處）；
8. `FormHS` ESD socket（:10963-10966）；9. `fGroundMan` RS232（:11093-11116）；
10. `RENESAS_Server->Listen()`（:11286-11295，facade 無此真物件）；
11. `fCleaning` AutoClean 計數（:11073-11086）；12. `fAirCon`/`fOmron`/`frmDTME08` 加熱通訊
（:10466-10531）；13. 三個獨立 socket/UDP 初始化（ASE KR/ASESendMessage/HanderTcpIp）。

## 逐段分類表（GA-3 brief 用；「已翻」判定以 2026-08-04 樹為準，動筆前重核）

| 行範圍 | 摘要 | 分類 | 關鍵依賴 |
|---|---|---|---|
| 9141-9167 | 螢幕縮放、initLifterTask/initAutoTask、bHandlerModel 早退 | TRANSLATE | asendic.cpp 已翻 |
| 9168-9170 | CreateForm(fQwertyKey2/fPassword2) | SEAM | 未翻表單 |
| 9171-9182 | ReadLastDataFile/ReadJamRateByDay/ReadWriteBinCountMode | TRANSLATE | cprod 已翻；後二者位置未核 |
| 9183-9213 | FormHS->CheckSetupFileData 早退；SMC/ATP DLL 版本檢查 | SEAM | FormHS 未翻 |
| 9214-9351 | MachineTypeChoice Caption + 客戶碼版本字串梯 | TRANSLATE | 純字串 |
| 9352-9385 | 權限/ATC 初始化/fSmartDiagnostic | SEAM | ATC 未翻 |
| 9386-9399 | fShowMessage Show/解析度/版位 | TRANSLATE | facade 已有 |
| 9400 | MainFormSizeToEpson(true) | SKIP-GATE | 純外觀（main.cpp:8541） |
| 9401-9408 | pgMotionView/dmTrayMotor/LookForFile | SEAM(ready) | facade 現成 |
| 9409-9445 | fSetup bFirstTime/LastSet 迴圈/cbSetupFileName | TRANSLATE | cprod 已翻 |
| 9446-9457 | CCDInterfaceForm/fTestCategory/Open_ADAM_6024 | SEAM | 未翻 |
| 9458-9532 | **MyForceDirectories ×~50** | TRANSLATE | common.cpp 已翻（最大安全段） |
| 9533 | FormHS->ClearLastFileLogName_HS | SEAM | 未翻 |
| 9534-9555 | TempFuseLimitType/CSV FileExists | TRANSLATE | — |
| 9556-9565 | **InitialHandler/ReadTechData/DoReadLastData/ReadLastSetIni** | TRANSLATE* | 依賴 GA-2；DoReadLastData（main.cpp:8839）併入 GA-3 |
| 9566-9583 | 路徑/LoadRunModePicture 群/fConfiguration | SEAM(部分 ready) | fConfiguration 未翻 |
| 9584-9654 | HT9046_LS 判斷/AutoSiteMap 觸發/SetRunStartMode | TRANSLATE | Sen[] 已翻 |
| 9655-9676 | SW[SwServerON].On/SystemInitialOK/Open_GaliCard/Suck Normal | TRANSLATE | myGALILmotor 已翻 |
| 9677-9709 | SwClear TTL 迴圈+`IOByteOut(0x2a0)` 裸埠寫 | SEAM | 真 ISA 埠寫入，stub |
| 9710-10027 | 雜項+**SetAliasAndTask×281** | TRANSLATE | 補欄位清單 |
| 10028-10104 | sgTaskList/StringGrid2 除錯格 | SEAM/SKIP | 顯示性 |
| 10105-10133 | InitDIOStstus/SetMotorMonitor；CheckRange 溫度 | SEAM/TRANSLATE | — |
| 10134-10146 | szSupervisor；**StartHeaterThread**；InitialShuttleThread（acarry 已翻）；Pad232 | 混合 | 見 thread 表 |
| 10147-10165 | KYEC ART 續跑（SKIP）；fCounterClear；**ServoOnAllMOT** | SEAM | 見 blast #1 |
| 10166-10228 | 可見性/Timer2/3/fShowBinSelect/LightOn/MyDBUpdateDB | TRANSLATE+SEAM | database 已翻 |
| 10229-10336 | labHonQC/StatusBar 版本梯/fAGV->IsATK_AMR（已翻）/logo | TRANSLATE | — |
| 10337-10449 | CompareTechData/labAutomation/pwPath/ReadTasterInfo | TRANSLATE+SEAM | — |
| 10450-10465 | UpdateForm(90/100)/**InitialOK=true**/InitShuttleThreadParameter | TRANSLATE | 里程碑 |
| 10466-10553 | OCR RS232/fAirCon/CCLink/VacuumUnit/加熱 Resume 區/fTemp_Set | SEAM | 大型硬體通訊上線 |
| 10554-10603 | fLotInfo ResetLotInfo/HAS_TRY_SUCK 清理/**ScanBtn+Vision**/**StartMainThread** | TRANSLATE+SEAM | 見 thread 表 |
| 10604-10747 | fAutomation Amkor/SW off/ASE KR socket/Rotate版位(SKIP)/**GemInitial** | SEAM 為主 | 見 blast #4 |
| 10748-10936 | 使用者選單/TSMC gbRemoteControl/CCD Timer 群/PM Alarm/EngSite | TRANSLATE+SEAM | — |
| 10937-10962 | AlarmUnlock.ini 讀寫 | TRANSLATE | 純檔案 IO |
| 10963-11052 | FormHS ESD/fSCKART/HanderTcpIp/KYEC_LEE 版位(SKIP) | SEAM/SKIP | — |
| 11053-11092 | FTP 快照 SetWorkParameter/fCleaning/HostName | TRANSLATE+SEAM | — |
| 11093-11225 | GroundMan RS232/DLL IPC/SIGURD/JCET 深分支 | SEAM/SKIP | — |
| 11226-11295 | FrmAOI RS232Init（facade 缺方法）/RENESAS_Server new+Listen | SEAM | facade 需補 |
| 11296-11365 | fShuttleMove ReadData（facade 有）/SW[SwAirOff].On/2DID 檢查/SetFocus/收尾 log | TRANSLATE | cContact 已翻 |

## GA-3 動筆前必核清單（agent 自標 UNCERTAIN，不可當已驗證）

`GetLimitAuth`/`GetCriticalParaAuth`/`ChangeArmSiteView`/`CheckSiteMapState`/`SetRunStartMode`/
`HasICUnderMachine`/`ToolLoadICO`/`IndexHeatMode`/`CompareTechData`/`ReadTasterInfo`/
`DoShowUserDefFrom`/`ShowFormPos`/`GetHotPlateYHalfPos`/`SearchForm`/`AdjZLedPos`/
`FormChangeAction`/`ReadEventLogAutoSaveInfo`/`ReadLightScaleTable`/`InitialReserveEmptyPoint`/
`InitialGaliDelayCount`/`SetTechDataToProd`(+`_Yield`)/`ResetShuttleWhichKit`/`LotSummary.ReadFile`
——確切定義位置與硬體依賴未逐一 grep，僅依命名分類。
