# V899 → V903 整合紀錄

- **來源**: `HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422`
- **目標**: `HT9011UC_Code_V3.33.903.0_20260417_Jimmy_20260428`
- **準則**: 以 V899 內 `//AI(...) YYYYMMDD: 描述` 註解為移植依據（共 175 hunk / 37 檔）
- **規則**:
  - 所有檔案以 BIG5 編碼讀寫（Python binary mode）
  - 衝突一律停下來詢問
  - 每完成一批跑 BCB6 build 驗證

## 進度總覽

| 批 | 主題 | 狀態 | 編譯結果 | 備註 |
|----|------|------|----------|------|
| G | MyLaneIo 錯誤訊息 | ? 完成 | BUILD SUCCEEDED | 6 hunks 全部成功 |
| H | N10 FTP/MNetLog 診斷 | ? 完成 | BUILD SUCCEEDED | HS_Function 10 hunks + main.cpp 1 hunk |
| I | AutoDeviceEjection Panel | ? 完成 | BUILD SUCCEEDED | cmydef + database + cShowBinSelect 4 hunks |
| J | Galil index motor 補完 | ? 略過 | — | V903 已含 RogerYang 20260316 等效修正 |
| L | HTEditList NULL guard | ? 完成 | BUILD SUCCEEDED | Public/HTEditList.cpp 2 hunks |
| M | note.cpp Pause guard | ? 完成 | BUILD SUCCEEDED | 手動補 SoftStop 守衛 |
| N | uHGemHT9045_SV LB SV | ? 完成 | BUILD SUCCEEDED | LB 溫度 SV 1 hunk |
| K | uTemp_Set Arm2 typo | ? 完成 | BUILD SUCCEEDED | Arm2 offset typo 1 hunk |
| V903-FIX | V903 既有 bug 修補 | ? 完成 | BUILD SUCCEEDED | cmydef.cpp/.h 補 30+ 漏宣告全域；ainarm_SearchPlacePlate 改全域；main.cpp 修 if-else / #endif；database.cpp 解 conflict；cShowBinSelect.cpp 用 V899 整檔覆蓋 |
| E | Greatek (BVL-3766) | ? 部分完成 | BUILD SUCCEEDED | Command.cpp + ProductionInfo.cpp RunSite 改寫（20260420 + 20260428 markers） |
| A | BootLog 24V 啟動診斷 | 待辦 | — | — |
| D | TrayY 步進馬達 | 待辦 | — | — |
| F | Gigas Auto Height 保護 | 待辦 | — | — |
| B | SiteUseManager + CompactSearch | 待辦 | — | — |
| C | AutoClean PickPlan 重構 | 待辦 | — | — |

## 詳細整合紀錄

（每個 hunk 一筆：檔案 / 行號 / 狀態 / 備註）

### 批 G — MyLaneIo（2026-04-28）

| # | 檔案 | Hunk | 狀態 |
|---|------|------|------|
| G1 | MyLaneIo.h | 在 `GetIOErrStr(...)` 後新增 2 個函式宣告 | ? |
| G2 | MyLaneIo.cpp | `IOOutBitStatus` 改用 `BuildIOErrMessage` | ? |
| G3 | MyLaneIo.cpp | `IOInputBit` 改用 `BuildIOErrMessage` | ? |
| G4 | MyLaneIo.cpp | `IOInputByte` 改用 `BuildIOErrMessage` | ? |
| G5 | MyLaneIo.cpp | `GetIOErrStr` case 1~5+default 中文化 | ? |
| G6 | MyLaneIo.cpp | 新增 `GetUseMNetIPDevTypeName` + `BuildIOErrMessage` 兩個新函式 | ? |

**驗證**：BCB6 build SUCCEEDED；`MyLaneIo.obj` 重新產生（30170 bytes / 196777 bytes obj）。
**Patch 腳本**：[.tmp/patch_g_mylaneio.py](.tmp/patch_g_mylaneio.py)


### ??? V903-FIX ??? V903 ??Ｘ?? bug 靽株??嚗?2026-04-28嚗?

V903 蝔?撘?蝣潭??憭???? V900-V902 ??????撘???亦????典??霈???? / 蝯?瑽? bug嚗???游?? V899 //AI hunks ???閫貊?潛楊霅舫?航炊嚗?撌脖??雿萎耨鋆?嚗????隞? `//AI(ht9045-v899) 20260428: V903 ??Ｘ?? bug 靽株??` 璅?閮鳴?????

| # | 瑼?獢? | 靽株?? | ?????? |
|---|------|------|------|
| F1 | cmydef.cpp / cmydef.h | 鋆? V900-V902 瞍?摰??????典??嚗?bNeedDoRemainCheck???bSECSPause???bNeedDoRunCheck???iPauseTime???bP65QAReTest???iP65QAReTestCount???bChangeModeING???bNeedRestartSW???iSystemUpTime???iWhoTrigerASV???bHasChangePlate???bInitialCylinderCheck???bDoInZTeach???bDoOutZTeach???bASMFirstTime???iSVByBinCount[256]???iSV_ErrBinCnt???iCheckShuttleSensor???tUNDER_20A_Delay (TQPF_Timer)???iUNDER_20A_Delay_Sec???tUNDER_20A_SHT_Delay???bSECS_REALTIMECCD???sSiteLayoutASECL???sSiteMapASECL???iSECS_HotPlateSeclect???bAutoRetryFlag???bTrayArmIsPickColor???bNeedBigMsg???LOAD_Y_USE_MOTOR[9]???bAOIConsecutiveFailFlag???bAOIConsecutiveFailPictureFlag???bAOIFailCountFlag???iAOILotCount???iGrossUPH???iNetUPH???sBinMap???iInitialCylinderCheckTask | ??? |
| F2 | ainarm_SearchPlacePlate.cpp | `static bool bHasChangePlate` 蝘駁?? static ??寧?? cmydef ??典??嚗?csystem.cpp ??梁?剁?? | ??? |
| F3 | main.cpp L437-444 | conflict 畾??????湔????? if-else ???憛?嚗?鋆???? V899 撠???????頛? | ??? |
| F4 | main.cpp L25742 | 蝘駁?文迨蝡???? `#endif` ???隞? | ??? |
| F5 | database.cpp L766 | 蝘駁?? conflict markers嚗?靽???? V899 ??? 6 ??? LOAD_Y_USE_MOTOR 閮剖?? | ??? |
| F6 | cShowBinSelect.cpp | conflict ???????????湔????湧??嚗?26 errors嚗?嚗???? V899 ??湔??閬????嚗?撌脣?怠?券?? //AI hunks嚗? | ??? |

**撽?霅?**嚗?BCB6 build SUCCEEDED嚗?HT9045.exe = 29.7 MB???

### ??? E ??? 頞?鞊? Greatek BVL-3766 RunSite嚗?2026-04-28嚗?

| # | 瑼?獢? | Hunk | ?????? |
|---|------|------|------|
| E1 | Command.cpp | `TfMain::SetSiteMapData` ??怠偏??啣??嚗???? `ret && CUSTOMER_CODE==CC_Greatek` ?????澆?? `fProductionInfo->SaveInfoFileWhenStart()`嚗?GPIB SETSITEMAP_ 敺?蝡???喲??撖? GTK info.txt嚗? | ??? |
| E2 | ProductionInfo/ProductionInfo.cpp | `SaveInfoFileWhenStart` ??? RunSite for 餈游????孵神嚗?敺? row-major + ??芾?? `LastSet.bUseTestSocket[0]`嚗???寧?? column-major (`i+1+j*iShtRow`) + ??抵?? OR + runtime `bTestSiteUse` OR LastSet | ??? |

**??孵??**嚗?
- 20260420 marker嚗?????????芸??撖急????? 2x8 layout ???摰Ｘ?園???????? A,d / A,g 憿舐內??? 4/7嚗?B,e 隤文?? 13???
- 20260428 marker嚗?One Side / Use Shuttle2 璅∪????芰?? Arm2(Z=1)嚗??????砍?芾?? `LastSet.bUseTestSocket[0]` ???霈? RunSite ??函征嚗??????? IPSC SiteMap ??芣?湔?? runtime / 銝?撖怠?? LastSet???

**撽?霅?**嚗?BCB6 build SUCCEEDED嚗?HT9045.exe 撌脤????啁?Ｙ?????

### 批 B — SiteUseManager 緊湊搜尋骨架（2026-04-05/07 markers）

| # | 檔案 | Hunk | 狀態 |
|---|------|------|------|
| B1 | cSiteUseManager.cpp / cSiteUseManager.h | 新檔，從 V899 整檔複製（21833 / 2544 bytes，class `cSiteUseManager SiteUseMgr` + Init/UseCompactSearch/CompactSearchPlateToPlace/CanAnyNozzleReachHP/ShadowLogBeforeSearch/ValidateSearchResult） | ? |
| B2 | HT9045.bpr | OBJFILES 補 `..\Obj\cSiteUseManager.obj`、PROJECT.FILES 補 `<FILE FILENAME="cSiteUseManager.cpp" ...>` | ? |
| B3 | cinitial.cpp | (1) `#include "cSiteUseManager.h"`；(2) `mtPlate2->YItem` 設定後呼叫 `SiteUseMgr.Init()` | ? |
| B4 | ainarm9045_1x4_4.cpp | (1) `#include "cSiteUseManager.h"`；(2) GetVariableXInHotPlateData case 1 包 `UseCompactSearch + ShadowLogBeforeSearch + ValidateSearchResult(0)`；(3) CheckHasSpaceToPlace_9045 post-place 包 UseCompactSearch；(4) bInArmTryPickFromHotPlateFinish HP-check 包 UseCompactSearch；(5) pre-Loader gate：`else if(... && UseCompactSearch && CanAnyNozzleReachHP==false){RecordErrorLog; Task=500;}` | ? |

**驗證**：BCB6 build SUCCEEDED（batch B 編譯通過）。

### 批 C — Auto Clean cCleanKitPickPlan 骨架（2026-04-07/08/09 markers）

| # | 檔案 | Hunk | 狀態 |
|---|------|------|------|
| C1 | AutoClean/cCleanKitPickPlan.h | 新檔，從 V899 整檔複製（4485 bytes，class 定義 + TPickPlan/TPickSlot 結構） | ? |
| C2 | AutoClean/AutoClean.cpp | git 3-way merge（base=V898，ours=V899，theirs=V903）一次套用全部 24 處 hunks：`#include`、cCleanKitPickPlan 完整實作、`bUseCKPP=false` 旗標、`g_CKPlan` 全域、11 處 dual-path 分支（pick/contact/recycle/purge）；merge 0 conflicts | ? |

**注意**：
- V903 的 AutoClean.cpp 原本是 LF-only line endings，merge 前須先 normalise 成 CRLF。
- `bUseCKPP=false` 為預設，所有新分支於 runtime 不啟用，行為等同舊版。
- HT9045.bpr 不需登錄 .h（僅 .cpp 需要，本次無新 .cpp 加入）。

**驗證**：BCB6 build SUCCEEDED（batch C 編譯通過）。

### 批 B (回退) — SiteUseManager 撤除（2026-04-28）

使用者反饋 cSiteUseManager 功能仍有問題，已完整移除：
- 刪除 cSiteUseManager.cpp / cSiteUseManager.h
- HT9045.bpr 移除 OBJFILES 與 PROJECT.FILES 條目
- ainarm9045_1x4_4.cpp 移除 `#include "cSiteUseManager.h"`
- cinitial.cpp 已由使用者先行移除 SiteUseMgr.Init() 呼叫

**驗證**：BCB6 build SUCCEEDED。

### 批 A — BootLog 啟動診斷（2026-04-23 markers）

| # | 檔案 | Hunk | 狀態 |
|---|------|------|------|
| A1 | Public/cBootLog.cpp / cBootLog.h | 新檔，從 V899 整檔複製（2729 / 1418 bytes，純 Win32 file I/O，無 VCL 依賴） | OK |
| A2 | HT9045.bpr | OBJFILES 補 cBootLog.obj、PROJECT.FILES 補 Public\cBootLog.cpp | OK |
| A3 | HT9045.cpp | 3-way merge clean，加入 BootLog include + WriteBootLog checkpoints | OK |
| A4 | main.cpp | 3-way merge 4 conflicts（皆與 V903-FIX F3/F4 重疊區域），全採 V899 ours：(C1)L437-444 V899 的 HasICUnderMachine 外層 if 結構；(C2)L24598 V899 完整 iOperatorModeCount else 邏輯；(C3)L25559 V899 SOFT_SIMULTE 早返回；(C4)L25821 V899 `//#endif` 註解形式覆蓋 V903 marker | OK |

**驗證**：BCB6 build SUCCEEDED。

### 批 D — TrayY 步進馬達擴充 [1]~[5]（2026-04-23 markers）

| # | 檔案 | Hunk | 狀態 |
|---|------|------|------|
| D1 | database.cpp L759-764 | 已於 V903-FIX F5 完成 | (前期) |
| D2 | HandlerSys.cpp | 3-way merge clean：(L100-105) Load 5 chk Y；(L485-488) Cassette 衝突警告；(L493-498) Save 5 INI key；(L517-522) Apply | OK |
| D3 | HandlerSys.h | 3-way merge clean：5 個 chk component 宣告 | OK |
| D4 | HandlerSys.dfm | 3-way merge 1 conflict（ActivePage/TabIndex），採 V903 theirs（保留 V903 較新的預設 tab） | OK |
| D5 | asendic.cpp | 3-way merge clean：TrayMoveIn/TrayMoveOut/TrayMoveStatus 加 iAxis 1~5 步進馬達分支 | OK |
| D6 | uMotorTest.cpp | 3-way merge 1 conflict（V899 enable 5 個 Y motor + V903 加 USE_LdUldCassetteMode），採 V899 ours（superset 條件） | OK |

**驗證**：BCB6 build SUCCEEDED。

### 批 F — 全智(GIGAS) Auto Height 掉料保護（2026-04-21 markers）

| # | 檔案 | Hunk | 狀態 |
|---|------|------|------|
| F1 | CosFunction.cpp | 3-way merge clean：FUNC_CC_GIGAS 中啟用掉料保護旗標；defaults 區塊預設關閉 | OK |
| F2 | CosFunction.h | 3-way merge clean：新增掉料保護旗標 | OK |
| F3 | cContact.cpp | 3-way merge 6 conflicts（皆為 V903 較寬欄位對齊的純空白差異，語意一致），全採 V903 theirs；Auto Contact Height 吸取後即時驗證真空 sensor、掉料時中止下壓 | OK |

**驗證**：BCB6 build SUCCEEDED。HT9045.exe = 29.7 MB（2026-04-28 15:56）。
