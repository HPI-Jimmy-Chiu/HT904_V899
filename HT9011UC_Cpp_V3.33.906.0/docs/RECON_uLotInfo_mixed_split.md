# PROPOSAL: uLotInfo MIXED 巨頭拆分提案（FormShow / Timer2Timer）

**狀態**: 唯讀提案，本文件是本子任務唯一交付物。未修改 `forms/fLotInfo.{h,cpp}` 之外的任何內容、未動 FormShow/Timer2Timer 本體一行程式碼、未 commit、未跑 build.bat/ctest。

**任務來源**: uLotInfo 顯示側 Wave B（`AI(W906-FW3-LotInfo-WB) 20260819`）的附帶交付物。這兩個函式在
`docs/RECON_uLotInfo_displayside.md` 第 1 節被列為 `MIXED(需拆分)`，Wave B 的 39 個 Tier 2 函式範圍不含它們——本文件只做拆分提案，不翻譯。

**解碼方法**: 與 Wave B 相同，`open(...,'rb').read().decode('cp950', errors='replace')`，20260819 執行，兩個函式的 span 皆 0 個 U+FFFD。逐行讀完整本體後才分段（非關鍵字掃描）。

---

## 0. 精度說明（誠實揭露）

下面每個「段落」的 golden 行號範圍都是逐行核對過的真實區間。但段落之間會有連接性的空行/單行註解沒有被計入任何一段（例如段落分隔的純空行、`//===` 分隔註解），所以每個函式的「段落行數加總」會略小於 golden 的總行數（FormShow 加總 887L vs 真實 918L，缺口 31L；Timer2Timer 加總 240L vs 真實 258L，缺口 18L）。這個缺口是連接性內容，不是遺漏的邏輯段落——但不假裝這是逐位元組（byte-perfect）分割，特此聲明。「可落地行數估計」一節的數字因此是**估計值，非精確值**，且分母是本文件自己分段所得的加總（887 / 240），不是 918 / 258。

---

## 1. FormShow（golden uLotInfo.cpp:316-1233，918L）

### 1.1 段落表

| # | golden 行段 | 行數 | 分類 | 說明 |
|---|------------|------|------|------|
| S1 | 316-346 | 31 | 顯示 | bShow=true、GetTimeInfo()、靜態頁籤 TabVisible、SettsChipAdvVisible()（已是 Tier1 真實方法）、清空多個 Cus 欄位可見度、ActivePage=tsLotID |
| S2 | 348-362 | 15 | 顯示/客戶碼 | CyuEan/JSI_HAOXING/NEXPERIA_Guangdong 的 RunMode 下拉選單顯示設定 |
| S3 | 364-380 | 17 | **MIXED** | MTI/`IniConfig.bShowLotInfo` 分派；內嵌 `ResetLotInfo()`（1 行，RECON #149 分類 (b) write-path，寫 `LastSet.bHasDownloadFile`）+ `SetSelectionVisible()`（已是 Tier1 真實方法） |
| S4 | 381-398 | 18 | **MIXED** | CC_TSI/CC_PANTHER 分支；內嵌 `cbPATModeChange(cbPATMode)`（1 行，RECON #169 分類 (b) write-path dispatcher） |
| S5 | 399 | 1 | **write** | `ShowXMLOnLine()`（RECON #103：儘管 Show 開頭，實為 RecordProcess 寫檔 x4） |
| S6 | 401-410 | 10 | 顯示 | FTP 頁籤可見度（JCET 特判）+ SECS_GEM 面板可見度 |
| S7 | 412-425 | 14 | 顯示* | CC_KYEC_LEE/AMD_M：`edtSysLotID->PopupMenu=FormBarcodeReader->pmBarcode`（PopupMenu 屬性未建模、`FormBarcodeReader` facade 存在性待查）+ `LotKeyInTime->Enabled` |
| S8 | 427 | 1 | **write** | `fLotInfo->ResetLotInfo()`（GOLDEN ODDITY：與 S3 的呼叫重複，同一顆函式被呼叫兩次） |
| S9 | 429-522 | 94 | **MIXED** | grpRFID/PTI-TFME 隱藏 FTP/SCK AutoCleanMonitor/AMKOR_Korea/SCC-SCK/SPIL/TERAPOWER/AMD_M/AMKOR_China-QUALCOMM/ChipMos_ZHUBEI 客戶碼分支；內嵌 `btnSaveClick(this)` x2（:486, :513，RECON #38 分類 (b) write-path，`WriteIniData` x19） |
| S10 | 523-541 | 19 | 顯示 | REAL_TIME_CCD 頁籤、`!TrayForm.bEnableAMR` 隱藏 KYEC_AMR 頁籤、ASE_KaohSiung ART 面板佈局 |
| S11 | 543 | 1 | 顯示 | `SetATCFormVisible()`（已是 Tier1 真實方法） |
| S12 | 545-562 | 18 | **MIXED** | BarCode/ESD/OCR/Socket 頁籤可見度；內嵌 HT9046_LS 分支 `imgRTCFullView1-4->Width/=2`（4 行，TImage 全樹無 port，見 Wave B GATE WB-1 同一發現） |
| S13 | 564-590 | 27 | 顯示 | CC_TSMC_TAINAN vs else：FTP/SECSLot 按鈕佈局（純 Visible/Enabled/Down/Top/Left） |
| S14 | 592-602 | 11 | **write** | Yield Monitor 頁籤 + `RefreshYieldMonitor()`（RECON #115 (b) write-path；Wave A 自己的 WA-3 發現：非 SIGURD 分支呼叫 `fCleaning->ChangeACSmartInterval`） |
| S15 | 604-608 | 5 | 顯示* | `fBarCode->mtBarcodeSetDefaultView()`（`fBarCode` 部分已存在但此成員存在性待查）+ chkTestMode/labLevelMode/coLevelMode 可見度 |
| S16 | 610-641 | 32 | 顯示 | sgBarcode/sgOCR/sgATRCount 三個 StringGrid 表頭文字初始化——**全函式唯一零客戶碼分支的乾淨區塊** |
| S17 | 643-649 | 7 | 顯示 | `ShowATCTempPanel()`（已是 Tier1 真實方法）+ ATC離線顏色/文字 |
| S18 | 651-713 | 63 | 顯示/客戶碼 | PTI cbRunMode Enabled、ONSEMI_M/JSCC_OS 裝置欄位佈局、SIGURD_ChungXing 隱藏、CosFunction.bOEEFunction 超豐分支（純 Visible/Caption/Top/Left） |
| S19 | 715-978 | 264 | **MIXED（全函式最大段）** | Murata/VTest/ASE_KaohSiung_K12/PANTHER/2DIDAllowList-JCET/SCC-SJ_Semiconductor/PTI-B03 共 8+ 個客戶碼分支的 RunMode/TestTimes ComboBox `Items->Add` 級聯；內嵌 `WriteIniData` x2（:811,:815，VTEST 分支鎖 Temp Offset/Auto Clean）+ `fBarCode->JCETUseMakeWhite2DIDList()` x2（:877,:893，Wave A WA-7 / Wave B WB-7/12/13 已建立的同一個 GATE） |
| S20 | 980-1013 | 34 | 顯示/客戶碼 | CC_GIGAS：FTP 啟用時把 17 個 TCheckBox 隱藏並鎖 Checked=true |
| S21 | 1014-1021 | 8 | 顯示 | ASEMARMS/ASECLEventLog 頁籤 + pnlLotInfo_ASECL/pnlLotStart_ASECL 面板可見度 |
| S22 | 1023-1032 | 10 | **顯示（本波後已轉為可落地）** | AMR 頁籤 + `RefreshAMR()`/`ShowAMRCategoryBin(true)`——**這兩個呼叫本身就是本 Wave B 剛翻好的真實方法**，這段在 Wave B 之前屬於 write-queue，現在起可視為顯示側 |
| S23 | 1034-1043 | 10 | 顯示/客戶碼 | CC_ASE_CL：`edSocket[i][j]->Text=LastSet.strSocketID[i][j]` 迴圈填字（`edSocket` 已存在，`LastSet.strSocketID` 存在性待查） |
| S24 | 1045-1067 | 23 | 顯示 | `IniConfig.bEnableFTP`/`bEnableRms`/OEE/REAL_TIME_CCD/WinWay/ATC_SYSTEM/Tri_Temp_Machine 條件級聯決定 `pgLotinfo->ActivePage` |
| S25 | 1069-1118 | 50 | 顯示/客戶碼 | SPIL `bSPILFunction` EQC-mode 頁面佈局（palQAMode/labLotID/edtSysLotID 尺寸調整）+ KYEC_LEE QA Mode 分支 |
| S26 | 1120 | 1 | 顯示 | `ShowSocketID()`（已是 Tier1 真實方法） |
| S27 | 1122-1123 | 2 | 顯示* | `pan_DewPoint`/`pl_DewPoint` 可見度（`DewPoint_Hardware_Install` 全域存在性待查） |
| S28 | 1125-1146 | 22 | 顯示* | `myInShuttleLotInfo`/`mtBarcodeInShLotInfo` 的 `SetColorMap`/`SetCellNumber`/`SetCellColorIndex` 呼叫（自訂控制項方法，TrayCore 家族，該兩個具名實例的 facade 存在性待查） |
| S29 | 1147-1154 | 8 | 顯示 | `grpBarcodeDisplayLotInfo` 可見度（2DID format==eAMD 分支） |
| S30 | 1156-1158 | 3 | **安全關鍵** | `tmrChamberBoost->Enabled` + **`SetLotStart("fLotInfo::FormShow", true)`**——RECON #10：399 行的 SECS Lot-Start 主控函式，EventReport(SECS DoLotStart)+RecordProcess+寫檔；這 3 行是全函式風險最集中的單點 |
| S31 | 1160 | 1 | 顯示 | `edtJobSeq->Text=ReadIniData(...)`（純讀） |
| S32 | 1166-1192 | 27 | **安全關鍵** | `CosFunction.bLotStartLockCriticalPara` 分支：`fMain->CheckCanChangeRealDummy()`/`HasICUnderMachine()` 存取控制檢查；設定 `bEPLogStart_KYEC`/`bTempLogStart_KYEC`/`bESDLogStart_KYEC`/`bArmTestInfoEvenLogStart_KYEC` 等旗標，這些旗標門控下游稽核記錄行為，不是純顯示 |
| S33 | 1194-1219 | 26 | 顯示/客戶碼 | ASE_SG/LEADYO/FirstTrayCheckOnUnloader 分支（純 Visible） |
| S34 | 1220-1229 | 10 | 顯示 | AQL Bin/Count Caption + `ReadWriteFTPAutomationData(true)`——RECON #111 自己註記「bRead=true 分支單獨看是 (a)」，這裡傳 `true` 正是純讀分支，**不需要 GATE** |
| S35 | 1230-1233 | 4 | 顯示* | `AdjtsYieldMonitiorSize()`/`InitialRefrigerantSystem()`（皆已是 Tier1 真實方法，各自內部已有自己的 GATE）+ `FrmAOI->AOIFailCountRefresh()`（`FrmAOI` facade 存在性待查） |

### 1.2 可翻段落清單（建議排入未來顯示側波次）

零客戶碼、零依賴缺口的最佳示範候選：**S16**（32L，三個 StringGrid 表頭純文字初始化）。

其餘可翻（部分需先查證 facade 依賴，部分需要在函式內部對 1-4 行做小範圍 GATE，做法比照 Wave A/B 已建立的「留真實部分、gate 掉缺口呼叫」idiom）：
S1, S2, S6, S10, S11, S13, S17, S18, S20, S21, **S22（Wave B 後已可落地）**, S23, S24, S25, S26, S29, S31, S33, S34
以及 MIXED 段落扣掉內嵌 write/gate 呼叫後的剩餘部分：S3（扣 ResetLotInfo 呼叫）、S4（扣 cbPATModeChange 呼叫）、S9（扣 btnSaveClick x2）、S12（扣 imgRTCFullView x4）、S19（扣 WriteIniData x2 + fBarCode 呼叫 x2）。
另有 4 個「顯示*」段落（S7, S15, S27, S28, S35）在正式排入前需先查一次 facade 依賴是否存在（`FormBarcodeReader`/`fBarCode->mtBarcodeSetDefaultView`/`DewPoint_Hardware_Install`/`myInShuttleLotInfo` 家族/`FrmAOI`），查證方式與 Wave A/B 相同（`grep -rn` 存在性檢查），本文件未做，留給承接波次。

**可落地行數估計**：對段落表逐段加總「顯示」「顯示*」「顯示/客戶碼」分類的行數，再加上 MIXED 段落扣除內嵌 write/gate 呼叫後的剩餘部分，估計約 **820-840 行 / 887（本文件段落加總分母）**，約 **89-92%**。真正扛住幾乎全部風險的是 S30（3L，`SetLotStart` 真實呼叫）與 S32（27L，critical-para 存取控制 + 稽核旗標），這 30 行本身雖少，但涵蓋了本函式唯一直接觸發 SECS Lot-Start 與存取控制的路徑，是承接波次絕對不能囫圇吞棗一起翻掉的部分。

### 1.3 GATE 段落清單（建議排除或個別小範圍 GATE，佇列給 write-path/safety 波次）

- **S5**（1L）：`ShowXMLOnLine()` 整行排除，等該函式本身被翻譯過再解禁。
- **S8**（1L）：`ResetLotInfo()` 第二次呼叫，排除（與 S3 重複，golden 本身的冗餘）。
- **S14**（11L）：`RefreshYieldMonitor()` 呼叫排除，等該函式（連同其 TERAPOWER 分支的 `ChangeACSmartInterval`）被翻譯過。
- **S30**（3L）：`SetLotStart(...)` 呼叫——**安全關鍵，不建議個別 GATE 後翻剩餘 2 行**，因為 `tmrChamberBoost->Enabled` 與 `SetLotStart` 在 golden 是同一個決策區塊（有 IC 在機台上或 Lot 已在跑才會進入這個 if），拆開翻譯有失去上下文的風險，建議整段 3 行一起排除。
- **S32**（27L）：critical-para 存取控制與稽核旗標，整段排除（存取控制檢查 + 稽核旗標門控，屬性質上的安全/合規段落，不是顯示）。

以上 GATE 段落合計約 43L（S5+S8+S14+S30+S32），加上 MIXED 段落內嵌的約 12L 呼叫點（ResetLotInfo/cbPATModeChange/btnSaveClick x2/imgRTCFullView x4/WriteIniData x2/fBarCode x2），總計約 **55L 需要排除或個別 GATE**，其餘皆為顯示候選。

---

## 2. Timer2Timer（golden uLotInfo.cpp:6934-7191，258L）

### 2.1 段落表

| # | golden 行段 | 行數 | 分類 | 說明 |
|---|------------|------|------|------|
| T1 | 6936-6939 | 4 | 顯示 | 靜態變數宣告 + `InitialOK==false` 早退（與 Wave B 的 Timer3Timer 同一個全域旗標） |
| T2 | 6941-6945 | 5 | 顯示 | CC_TSMC_TAINAN：`ShowMyMessage("DOWNLOAD_RECIPE_BY_EA Finish!!")`（彈窗提示，非警報） |
| T3 | 6947-6950 | 4 | 顯示 | CC_KYEC_XILINX：`btClearBarcodeList->Visible` by AccessLevel（`btClearBarcodeList` 已存在，PT-W3-integrate ADD） |
| T4 | 6952-6956 | 5 | 顯示* | `fAGV->IsSPIL_AMR()` 為真時更新 Bundle Tray ID 標籤（`fAGV` facade 存在性/`IsSPIL_AMR()`成員待查） |
| T5 | 6958-6985 | 28 | 顯示 | `bSecsGemDownloadFTP=false` + OCR 連線測試狀態機（`ShowMyMessage` x2 彈窗，非警報；純旗標翻轉） |
| T6 | 6987-7016 | 30 | 顯示* | OCR 檔名/Log In-Out Caption（`fMain->cbSetupFileName` 依賴待查） |
| T7 | 7018-7029 | 12 | **MIXED** | Tester 連線面板 Caption/Color；內嵌 `fOCR->sTesterLotId`（`fOCR` 已存在但僅有 `ChangeLightValue`，這個成員不存在——與 Wave B GATE WB-6 同一個 `TfOCR` shim，同一種發現）+ `fSetup->edOcrText->Text`（`fSetup` facade 存在性待查） |
| T8 | 7030-7034 | 5 | 顯示 | AutoRetest/OCRBarCode 頁籤可見度 |
| T9 | 7035-7085 | 51 | **安全關鍵（全函式最大安全區塊）** | `ATC_SYSTEM==eWinWay` 分支：`fWinway->OpenCommPort()`/`arrATC_Site[i]->GetST()`/`SetTempratureAll()`/`WinwayCOM->StopComm()`/`SetPT()`——**對 WinWay ATC 控制器下達真實的溫度設定與通訊控制指令**，性質等同本檔案已建立的 `SetRunATC`/`SetATCOffset` 類 GATE（Wave A WA-1），不是顯示 |
| T10 | 7087-7094 | 8 | 顯示 | CC_GIGAS/bVTESTFunction FTP 頁籤可見度分支 |
| T11 | 7096-7119 | 24 | **安全關鍵** | bVTESTFunction 分支：Server Caption + 3 小時送檢報警計時器，`iProduceTimeCT++`累積後呼叫 `ShowErrorMessage("WAR16123", K_SKIP, MMSystem)`——真實警報，與本檔案既有的所有 `ShowErrorMessage` GATE（forms/fTemperFrom.h T1、Wave A WA-6、Wave B WB-14）同一類別 |
| T12 | 7121-7123 | 2 | 顯示 | else 分支：`tsDeviceInfo->TabVisible=IniConfig.bEnableRms` |
| T13 | 7124-7125 | 2 | 顯示 | `tsTesterLog`/`grpBarcodeDisplayLotInfo` 可見度 |
| T14 | 7127-7149 | 23 | **MIXED** | FTP Automation 頁籤可見度變化邊緣偵測；內嵌 `RefreshYieldMonitor()`（1 行，同 FormShow S14 的 GATE），其餘（`fLotInfo->ShowInformation(false)`/`Height`/`Width`/`sbTest->Top`/`AdjtsYieldMonitiorSize()`）皆已是 Tier1 真實方法或純幾何寫入 |
| T15 | 7152-7164 | 13 | 顯示 | 2DID Multi-Recipe：`labBarcodeRecipe`/`edtBarcodeRecipe` 可見度 |
| T16 | 7166-7187 | 22 | 顯示/客戶碼 | OCR 比對清單可見度 + SPIL 2DID `cbRunMode->Enabled` 存取層級邏輯（純 Enabled 切換，非存取控制執行點） |
| T17 | 7189 | 1 | 顯示 | `ScanRefrigerantSystem()`（本 Wave B 剛翻好的真實方法，內部自帶 WB-16 GATE） |
| T18 | 7190 | 1 | 顯示 | `palAQLMode->Visible=IniConfig.bI52_bAQLSortMode` |

### 2.2 可翻段落清單

T1, T2, T3, T5, T8, T10, T12, T13, T15, T16, T17, T18 為直接可翻（部分需要新增少量 widget 成員，無 GATE）。
T4, T6 為「顯示*」，排入前先查 `fAGV`/`fMain->cbSetupFileName` 依賴存在性。
T7 扣掉 `fOCR->sTesterLotId`（1 行 GATE）後，面板 Caption/Color 邏輯可翻（`fSetup->edOcrText` 依賴待查）。
T14 扣掉 `RefreshYieldMonitor()`（1 行 GATE）後，邊緣偵測 + 幾何寫入邏輯可翻。

**可落地行數估計**：約 **160-170 行 / 240（本文件段落加總分母）**，約 **65-70%**。

### 2.3 GATE 段落清單

- **T9**（51L）：WinWay ATC 硬體溫控指令，整段排除，性質等同 SetRunATC/SetATCOffset 類既有 GATE。
- **T11**（24L）：3 小時送檢報警（`ShowErrorMessage("WAR16123",...)`），整段排除。
- **T7** 內嵌 1 行（`fOCR->sTesterLotId`）。
- **T14** 內嵌 1 行（`RefreshYieldMonitor()`）。

以上合計約 77L 需要排除或個別 GATE，佔全函式 258L 的約 30%，其中 T9+T11 這兩段（75L）就是幾乎全部的風險來源——這與 Wave A/B 反覆發現的模式一致：**「Timer」命名前綴同樣不可信**，Timer2Timer 裡藏著本檔案已知最重的一塊真實硬體控制邏輯（WinWay ATC）與一個真實警報（WAR16123），兩者合計只佔函式 29% 的行數，卻是唯一不能被「顯示側波次」自動吃下去的部分。

---

## 3. 方法論註記

1. 本文件的段落邊界是人工逐行讀完整個函式本體後劃分的邏輯區塊，不是機器抽取；每個段落的 golden 行號範圍都用 `sed`-等效的行號核對過（本次用 python 直接依行號切片，逐段核對簽章）。
2. 「顯示*」標記的段落表示：該段落表面看是顯示邏輯，但依賴的 facade 成員/全域變數尚未在這次拆分過程中逐一查證存在性，交接時請先查證（`grep -rn`），不要假設「顯示*」跟「顯示」一樣安全。
3. 本文件沒有查證的 facade 依賴清單（供承接波次直接使用）：`FormBarcodeReader`、`fBarCode->mtBarcodeSetDefaultView`、`DewPoint_Hardware_Install`、`myInShuttleLotInfo`/`mtBarcodeInShLotInfo`（TrayCore 家族具名實例）、`FrmAOI`、`fAGV->IsSPIL_AMR()`、`fMain->cbSetupFileName`、`fSetup->edOcrText`、`LastSet.strSocketID`。
4. 兩個函式都出現同一個模式：**GATE 段落雖然行數佔比不高（FormShow 約 6%、Timer2Timer 約 30%），但集中了幾乎全部的安全風險**——這與 `docs/RECON_uLotInfo_displayside.md` 第 4 節「Show/Refresh 命名前綴不可信」的結論完全一致，本文件是同一個結論在兩個 MIXED 巨頭身上的具體印證。
5. 本文件不建議把 FormShow/Timer2Timer 直接「整顆函式」排入任何波次（無論顯示側或 write-path 側）：正確做法是承接波次先把本文件的段落表當作切割藍圖，在 `forms/fLotInfo.cpp` 用同一個函式簽名下、按 golden 原始順序把可翻段落串接起來，GATE 段落比照 Wave A/B 已建立的 `#if 0` + 引用 GATE 編號的 idiom 處理，而不是嘗試把 918/258 行一次性讀懂再翻。

---

## 4. 指令與時間戳記錄

- cp950 解碼 + U+FFFD 計數（FormShow/Timer2Timer 兩個 span）：20260819
- 逐行讀完整個函式本體（兩次 Read，各約 900-1200 行輸出）：20260819
- facade 依賴存在性查證：本文件**未做**（見第 3 節第 3 點清單），與 Wave B 的 39 個函式不同，Wave B 對每個依賴都做了 `grep -rn` 存在性檢查，本 MIXED 拆分提案僅到「識別出待查清單」為止。
