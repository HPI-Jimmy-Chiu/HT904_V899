# RECON: uLotInfo 顯示側翻譯波偵察報告


**狀態**: 唯讀 recon，本文件是本波次唯一交付物。未修改任何原始碼、未 commit、未跑 build.bat/ctest。

**Golden 來源**: `D:\HT9045\HT9011UC_Code_V3.33.906.0_20260618\uLotInfo.cpp`（16,613 行）+ `uLotInfo.h`（1,426 行）

## 0. 解碼與盤點方法

- cp950 解碼指令：`open(path,'rb').read().decode('cp950', errors='replace')`，於 2026-08-19 執行。
- `uLotInfo.cpp`：776,890 bytes → 16,613 行，**U+FFFD 計數 = 0**。
- `uLotInfo.h`：54,934 bytes → 1,426 行，**U+FFFD 計數 = 0**。
- 函式邊界抽取：對 cp950 解碼後文字做字串/註解遮罩（避免把 `"TfLotInfo::FormDestroy"` 這種字面值或行尾註解誤判成函式定義），再以正規表示式 `TfLotInfo::(\w+)\s*\(` 抓取候選定義行，逐一用大括號配對找結束行。抓到 210 個候選，`grep -n "TfLotInfo::"` 原始命中 216 筆，差額 6 筆逐一核對如下，均非真定義：`:311`（`MyDBIProcess("Exception", "TfLotInfo::FormDestroy")` 字串常量）、`:9480`/`:9595`/`:9827`/`:9832`（`SetATCOffset` 函式內部的行尾註解引用同名函式，如 `//TfLotInfo::SetATCOffset for ATC_TYPE_70`）、`:15363`（`MyDBIProcess("Exception", "TfLotInfo::SaveGroundESDData_Upolad")` 字串常量）。
- 行號抽查方法：對每個宣稱的 span，用 `sed -n '<start>,<end>p' uLotInfo.cpp.cp950.txt` 印出原始行文字比對簽章（函式名／`{`／`}`），下方表格每一列的起訖行號均如此核對過，非單純腳本輸出未經檢查。
- **自動關鍵字掃描的已知盲點**（本波次過程中發現，見第 4 節「關鍵發現」）：純字串賦值（如 `RunInfo.bLotStart=!bLotEnd`）、`ReadWriteIni(...)` 這個第三種 ini 讀寫 helper（不同於 `ReadIniData`/`WriteIniData`）、原生 C `fopen/fputs/fclose`、大小寫相依的 `TfFTP`（regex 用 `Ftp` 抓不到）、`SW[...].On()/Off()`（IO 輸出）、`Sen[...].IsOn()/IsOff()`（感測器輸入）、`ShowQwertyKey`/`ShowEventLogLogin`/`CheckLoginSuccess`（虛擬鍵盤與登入側門）等，都不在第一輪掃描的 pattern 內，是分批發現後才逐一補掃。**這代表任何只看關鍵字掃描結果、不逐行讀函式本體的分類，對本檔案是不可靠的**——本報告的分類已盡力逐一覆核，但仍有 39 個 (a) 類函式只做過關鍵字掃描、未逐行讀完整本體（下方第 1 節表格逐列標註、第 5.2 節另有完整清單），交接時請勿假設它們「掃描乾淨=安全」。

## 1. 函式全量清冊（TfLotInfo::，210 個方法）

欄位：# / 方法 / Golden span / 行數 / 分類 / 一句話功能與依據。分類欄 `(verified)` 出現在「一句話功能」欄位內文表示已逐行讀過函式本體；未出現則表示分類主要依據關鍵字掃描 + 函式命名 + 上下文註解，尚未逐行覆核。

| # | 方法 | Span | 行數 | 分類 | 一句話功能 |
|---|------|------|------|------|------------|
| 1 | `TfLotInfo` | 157-299 | 143 | (a) 顯示/唯讀 | ctor: wires up widget/menu defaults only |
| 2 | `FormDestroy` | 301-314 | 14 | (a) 顯示/唯讀 | sets bShow=false |
| 3 | `FormShow` | 316-1233 | 918 | MIXED(需拆分) | FORM-OPEN GIANT (918L): widget init + config reads + some writes; needs section-level split, not a single unit |
| 4 | `FormClose` | 1235-1238 | 4 | (a) 顯示/唯讀 | sets bShow=false |
| 5 | `SettsChipAdvVisible` | 1240-1260 | 21 | (a) 顯示/唯讀 | tab visibility/caption by CUSTOMER_CODE (verified) |
| 6 | `SetSelectionVisible` | 1262-1342 | 81 | (a) 顯示/唯讀 | tab/group visibility + Enabled by AccessLevel (verified; contains an AI-tagged access-control gate, not motor-safety) |
| 7 | `sbSECSLotEndClick` | 1346-1402 | 57 | (c) 安全關鍵 | Lot End entry: gates on SystemStart / iTestHeadMotorTask / CheckCanChangeRealDummy before allowing end |
| 8 | `ReadWriteLotInfo` | 1404-1490 | 87 | (b) write path | ReadWriteLotInfo(false) persists to config.ini via WriteIniData (bRead=true branch alone would be (a)) |
| 9 | `SetLotID` | 1492-1570 | 79 | (b) write path | SetLotID writes Lot No/LotStartTime to config.ini |
| 10 | `SetLotStart` | 1572-1970 | 399 | (b) write path | SetLotStart: EventReport (SECS DoLotStart) + RecordProcess log + writes |
| 11 | `SetLotEnd` | 1972-2281 | 310 | (b) write path | SetLotEnd: EventReport + FTP + file writes |
| 12 | `SetLotComponents` | 2283-2320 | 38 | (b) write path | **SetLotComponents: verified fully -- mostly Enabled/Caption toggling BUT also writes RunInfo.bLotStart=!bLotEnd, the global lot-running flag that gates SECS DoLotStart dedup elsewhere. Automated signal scan cannot catch plain struct-field assignments like this; caught only by full-body read.** |
| 13 | `btUploadClick` | 2322-2330 | 9 | (b) write path | btUploadClick dispatches to UploadToServer |
| 14 | `_DelTree` | 2332-2364 | 33 | (b) write path | _DelTree: recursive directory delete |
| 15 | `btDownloadClick` | 2366-2877 | 512 | (c) 安全關鍵 | btDownloadClick: ShowModal + SystemStart interlock gate before recipe download |
| 16 | `DownloadFromServer_TSMC` | 2879-3502 | 624 | (b) write path | DownloadFromServer_TSMC: FTP/file/ini writes + ShellExecute (7z unzip) |
| 17 | `DoBackupSetupFile` | 3504-3780 | 277 | (b) write path | DoBackupSetupFile: backs up local setup files before overwrite (CopyFile-class writes; automated rule under-weighted this, function purpose is unambiguous write path) |
| 18 | `DoOverWriteSetupFile` | 3782-4180 | 399 | (b) write path | DoOverWriteSetupFile: restores backed-up files, heavy WriteIniData |
| 19 | `DownloadFromServer` | 4182-4704 | 523 | (b) write path | DownloadFromServer: FTP + ini writes + ShellExecute |
| 20 | `DownloadFromERMS` | 4706-4850 | 145 | (b) write path | DownloadFromERMS: FTP + ini writes + ShellExecute |
| 21 | `UploadToServer` | 4852-4856 | 5 | (b) write path | UploadToServer() thin wrapper -> overload |
| 22 | `UploadToServer` | 4858-4964 | 107 | (b) write path | UploadToServer(name,alarm): packages + copies recipe to server share |
| 23 | `edTempKeyUp` | 4966-4999 | 34 | (a) 顯示/唯讀 | edTempKeyUp: barcode-suffix stripping on edTemp text (verified) |
| 24 | `btnFtpServerClick` | 5001-5126 | 126 | (c) 安全關鍵 | btnFtpServerClick: SystemStart/CheckCanChangeRealDummy gate before changing FTP server settings |
| 25 | `btSaveSetupFileClick` | 5128-5223 | 96 | (b) write path | btSaveSetupFileClick: EventReport(SwitchSetupFile) + file write |
| 26 | `CutTempToEdit` | 5225-5244 | 20 | (a) 顯示/唯讀 | CutTempToEdit: string formatting helper |
| 27 | `Timer1Timer` | 5255-5342 | 88 | (b) write path | Timer1Timer: RTC file-switch bookkeeping, writes a file |
| 28 | `RTCChangeFile` | 5344-5364 | 21 | (b) write path | RTCChangeFile: deletes/rotates RTC image files |
| 29 | `LoadRTCFullViewImg` | 5366-5448 | 83 | (a) 顯示/唯讀 | LoadRTCFullViewImg: loads+displays an already-captured image (read, not write) |
| 30 | `ShowATCThermo` | 5463-5518 | 56 | (c) 安全關鍵 | ShowATCThermo: dispatcher; reads Sen[SnTJCurrent] and computes dSetATCTemp, then dispatches into ShowATC70/20/New which contain real ATC commands (see finding below) |
| 31 | `ShowATC70Thermo` | 5520-5857 | 338 | (c) 安全關鍵 | ShowATC70Thermo: **contains ATCInterfaceForm->SetRunATC(false/true) at :5790/:5818 -- a real ATC run/stop command, not display** |
| 32 | `ShowATC20Thermo` | 5859-6344 | 486 | (a)?未逐行覆核 | ShowATC20Thermo: grepped clean of Set*/SW[]/Sen[] in this pass, but NOT fully body-read line-by-line -- treat as unverified given the sibling functions were not clean |
| 33 | `ShowNewATCThermo` | 6346-6932 | 587 | (c) 安全關鍵 | ShowNewATCThermo: **calls fLotInfo->SetATCOffset(true,true) at :6469 under a condition -- triggers real ATC offset logic, not display** |
| 34 | `Timer2Timer` | 6934-7191 | 258 | MIXED(需拆分) | Timer2Timer (258L): ShowMyMessage popups + access-level visibility + count displays + flag writes, heterogeneous per-tick handler |
| 35 | `edDeviceNameMouseDown` | 7193-7214 | 22 | (a) 顯示/唯讀 | edDeviceNameMouseDown: customer-gated field clear |
| 36 | `cbbDeviceNameChange` | 7216-7219 | 4 | (a) 顯示/唯讀 | cbbDeviceNameChange: mirrors combo text to edit (verified) |
| 37 | `pgLotinfoChange` | 7221-7266 | 46 | (a) 顯示/唯讀 | pgLotinfoChange: reads ini for the active tab, sets captions |
| 38 | `btnSaveClick` | 7268-7298 | 31 | (b) write path | btnSaveClick: WriteIniData x19 |
| 39 | `ConnectNetDeviceS` | 7300-7344 | 45 | (b) write path | ConnectNetDeviceS: WNetAddConnection2 -- mounts a network drive (system-level side effect) |
| 40 | `N23UseLotInfoFile` | 7346-7380 | 35 | (a) 顯示/唯讀 | N23UseLotInfoFile: reads a text file into edit fields (verified, read-only) |
| 41 | `sbSECSLotStartClick` | 7382-8416 | 1035 | (b) write path | sbSECSLotStartClick (1035L): master Lot-Start orchestrator, heavy customer-specific validation + file writes; no direct SystemStart/motor gate found in this function itself |
| 42 | `btStartCountClick` | 8418-8421 | 4 | (a) 顯示/唯讀 | btStartCountClick: sets bStartCount_SCK flag (verified) |
| 43 | `labLotIDMouseDown` | 8423-8428 | 6 | (a) 顯示/唯讀 | labLotIDMouseDown: opens on-screen QWERTY keypad for touch input (verified, benign UI helper) |
| 44 | `spOCRSaveLogClick` | 8430-8433 | 4 | (b) write path | spOCRSaveLogClick dispatches to Save_BarCodeLog (file write) |
| 45 | `Save_BarCodeLog` | 8435-8456 | 22 | (b) write path | Save_BarCodeLog: writes barcode log file |
| 46 | `spOCRChangeFileClick` | 8458-8474 | 17 | (a) 顯示/唯讀 | spOCRChangeFileClick: caption update only |
| 47 | `Timer3Timer` | 8476-8485 | 10 | (a) 顯示/唯讀 | Timer3Timer: caption tick |
| 48 | `DoPassword` | 8487-8518 | 32 | (c) 安全關鍵 | DoPassword: password-entry gate (Amkor) -- security side-door, not motor |
| 49 | `spOCRLogInClick` | 8520-8541 | 22 | (c) 安全關鍵 | spOCRLogInClick: login flow entry |
| 50 | `SaveLotOperatorID` | 8543-8549 | 7 | (b) write path | SaveLotOperatorID: ReadWriteIni(bRead=false) writes config.ini (missed by first-pass regex, ReadWriteIni is a 3rd ini-helper name) |
| 51 | `NetATCTimeTimer` | 8558-9350 | 793 | (c) 安全關鍵 | NetATCTimeTimer (793L): **contains IOSwitch(4)/IOSensor(4) -- real hardware IO toggles/reads despite the "Net...Timer" display-sounding name** |
| 52 | `SetATCOffset` | 9405-9870 | 466 | (c) 安全關鍵 | SetATCOffset (466L): sends ATC offset commands (ATC_InterfaceForm->SetOffset family per skill doc); explicitly a control write, not display |
| 53 | `SetATCFFCOffset` | 9872-9904 | 33 | (c) 安全關鍵 | SetATCFFCOffset: FFC temperature offset control, same family as SetATCOffset |
| 54 | `coLevelModeChange` | 9906-9936 | 31 | (c) 安全關鍵 | coLevelModeChange: uses ShowEventLogLogin/CheckLoginSuccess login gate (ModalDoorHelpers) |
| 55 | `SetATCFormVisible` | 9938-9985 | 48 | (a) 顯示/唯讀 | SetATCFormVisible: tab caption/visibility by ATC_SYSTEM (verified) |
| 56 | `btClearBarcodeCountClick` | 9987-10003 | 17 | (b) write path | btClearBarcodeCountClick: clears + persists barcode reject count |
| 57 | `btClearBarcodeListClick` | 10005-10014 | 10 | (b) write path | btClearBarcodeListClick: clears 2DID list + RecordProcess + SaveToFile + re-sends CCD list command |
| 58 | `spSECSLotCheckClick` | 10016-10033 | 18 | (b) write path | spSECSLotCheckClick: EventReport |
| 59 | `TimerERMSTimer` | 10035-10211 | 177 | (b) write path | TimerERMSTimer: RecordProcess + file writes (ERMS polling) |
| 60 | `btChangeFileClick` | 10213-10237 | 25 | (a) 顯示/唯讀 | btChangeFileClick: caption swap |
| 61 | `pl_ATC_OnlineClick` | 10239-10276 | 38 | (c) 安全關鍵 | pl_ATC_OnlineClick: SystemStart-gated ATC online toggle |
| 62 | `OCRConnectTest` | 10278-10285 | 8 | (c) 安全關鍵 | OCRConnectTest: **toggles SW[SwOCRTigger] On/Off -- real IO output pulse to trigger the OCR camera, not display** |
| 63 | `edtSysOperatorIDKeyUp` | 10287-10312 | 26 | (a) 顯示/唯讀 | edtSysOperatorIDKeyUp: customer-gated text handling |
| 64 | `edtSysLotIDKeyUp` | 10314-10344 | 31 | (a) 顯示/唯讀 | edtSysLotIDKeyUp: customer-gated text handling |
| 65 | `sbARMSShowClick` | 10346-10349 | 4 | (c) 安全關鍵 | sbARMSShowClick: ShowModal() side-door (opens ARMS view) -- flagged per ShowModal rule even though likely a read-only monitor; verify at translation time |
| 66 | `palSecsGemMouseDown` | 10351-10425 | 75 | (c) 安全關鍵 | palSecsGemMouseDown: SystemStart-gated |
| 67 | `sb_RunExecutFileClick` | 10427-10455 | 29 | (b) write path | sb_RunExecutFileClick: ShellExecuteEx launches an external configured program |
| 68 | `edDeviceNameKeyDown` | 10457-10473 | 17 | (a) 顯示/唯讀 | edDeviceNameKeyDown: customer-gated barcode field reset |
| 69 | `sb_Main_EvenLevelLoginClick` | 10475-10489 | 15 | (c) 安全關鍵 | sb_Main_EvenLevelLoginClick: real login dialog (ShowEventLogLogin/CheckLoginSuccess), not just a keypad |
| 70 | `edtASECL_LotIDClick` | 10491-10494 | 4 | (a) 顯示/唯讀 | edtASECL_LotIDClick: opens on-screen keypad (verified pattern same as labLotIDMouseDown) |
| 71 | `btnASECL_LotStartClick` | 10497-10671 | 175 | (c) 安全關鍵 | btnASECL_LotStartClick: SystemStart/CheckCanChangeRealDummy-gated ASE-CL Lot Start variant |
| 72 | `CheckEventLogParameter` | 10673-10720 | 48 | (a) 顯示/唯讀 | CheckEventLogParameter: pure required-field validation, returns bool (verified) |
| 73 | `SaveBackEventLogInfo` | 10722-10768 | 47 | (b) write path | SaveBackEventLogInfo: **raw fopen/fputs/fclose CSV log write -- missed by first-pass regex entirely (zero other signals)** |
| 74 | `SaveBackEventTracker` | 10770-10827 | 58 | (b) write path | SaveBackEventTracker: file write (fopen family) |
| 75 | `UploadEventLogFile` | 10829-10838 | 10 | (b) write path | UploadEventLogFile: file move/copy |
| 76 | `SaveASECLTestLogInfo` | 10840-10967 | 128 | (b) write path | SaveASECLTestLogInfo: fopen-family CSV log write |
| 77 | `SaveASECLNewTestLogInfo` | 10969-11092 | 124 | (b) write path | SaveASECLNewTestLogInfo: file write |
| 78 | `SaveASECLTesterHardwareLog` | 11094-11164 | 71 | (b) write path | SaveASECLTesterHardwareLog: file write |
| 79 | `SaveOEELog` | 11166-11441 | 276 | (b) write path | SaveOEELog: heavy file write (fopen x5, FileWrite=50) |
| 80 | `ShowSocketID` | 11443-11516 | 74 | (a) 顯示/唯讀 | ShowSocketID: pure site/channel grid Caption/Visible layout from TestIF_File.iSiteMap (verified, fully read) |
| 81 | `btnQAmodeSaveClick` | 11518-11526 | 9 | (b) write path | btnQAmodeSaveClick: writes ini |
| 82 | `edQAModeMouseDown` | 11528-11532 | 5 | (a) 顯示/唯讀 | edQAModeMouseDown: opens on-screen keypad |
| 83 | `edPageMouseDown` | 11534-11541 | 8 | (a) 顯示/唯讀 | edPageMouseDown: opens on-screen keypad, customer-gated |
| 84 | `LotKeyInTimeTimer` | 11543-11660 | 118 | (b) write path | LotKeyInTimeTimer: barcode-scan timeout bookkeeping; drives Lot Start flow |
| 85 | `tmrChamberBoostTimer` | 11664-11737 | 74 | (c) 安全關鍵 | tmrChamberBoostTimer: SystemStart-read, chamber boost timer (process control adjacent) |
| 86 | `btnStartChamberBoostClick` | 11739-11745 | 7 | (b) write path | btnStartChamberBoostClick: starts chamber boost function |
| 87 | `btnStopChamberBoostClick` | 11747-11753 | 7 | (b) write path | btnStopChamberBoostClick: stops chamber boost function |
| 88 | `edtSysOperatorIDMouseUp` | 11755-11794 | 40 | (a) 顯示/唯讀 | edtSysOperatorIDMouseUp: customer-gated |
| 89 | `edtSysLotIDMouseUp` | 11796-11831 | 36 | (a) 顯示/唯讀 | edtSysLotIDMouseUp: customer-gated |
| 90 | `ClearAllSetupFile` | 11833-11890 | 58 | (b) write path | ClearAllSetupFile: deletes setup files from disk |
| 91 | `edtSysLotIDKeyPress` | 11892-11917 | 26 | (a) 顯示/唯讀 | edtSysLotIDKeyPress: barcode terminator handling + focus chaining (verified) |
| 92 | `edPageKeyPress` | 11919-11933 | 15 | (a) 顯示/唯讀 | edPageKeyPress: barcode terminator handling + focus chaining (verified) |
| 93 | `edtSysOperatorIDKeyPress` | 11935-11963 | 29 | (a) 顯示/唯讀 | edtSysOperatorIDKeyPress: customer-gated |
| 94 | `cbRunModeKeyPress` | 11965-11988 | 24 | (a) 顯示/唯讀 | cbRunModeKeyPress: customer-gated |
| 95 | `cbRunModeKeyDown` | 11990-11997 | 8 | (a) 顯示/唯讀 | cbRunModeKeyDown: blocks manual text entry under JCET white-list mode (verified) |
| 96 | `cbRunModeKeyUp` | 11999-12006 | 8 | (a) 顯示/唯讀 | cbRunModeKeyUp: same guard as KeyDown (verified) |
| 97 | `edDeviceNameKeyPress` | 12008-12030 | 23 | (b) write path | edDeviceNameKeyPress: RecordProcess call |
| 98 | `edTempKeyPress` | 12032-12042 | 11 | (a) 顯示/唯讀 | edTempKeyPress: customer-gated |
| 99 | `edDeviceNameKeyUp` | 12044-12051 | 8 | (a) 顯示/唯讀 | edDeviceNameKeyUp: sets bLotFirstKeyIn flag only (verified) |
| 100 | `edTempKeyDown` | 12053-12063 | 11 | (a) 顯示/唯讀 | edTempKeyDown: customer-gated |
| 101 | `edtSysLotIDKeyDown` | 12065-12080 | 16 | (a) 顯示/唯讀 | edtSysLotIDKeyDown: customer-gated |
| 102 | `edtSysOperatorIDKeyDown` | 12082-12097 | 16 | (a) 顯示/唯讀 | edtSysOperatorIDKeyDown: customer-gated |
| 103 | `ShowXMLOnLine` | 12099-12137 | 39 | (b) write path | ShowXMLOnLine: RecordProcess log write x4 despite Show-prefixed name (verified body: Caption/Color set + RecordProcess in every branch) |
| 104 | `pnlXMLOnLineClick` | 12139-12165 | 27 | (b) write path | pnlXMLOnLineClick dispatches into server on/off toggle |
| 105 | `WhenTestRecordTemperatureLog_3Sigma` | 12167-12283 | 117 | (b) write path | WhenTestRecordTemperatureLog_3Sigma: writes temperature log file |
| 106 | `btnSaveDataClick` | 12285-12295 | 11 | (a) 顯示/唯讀 | btnSaveDataClick: SystemStart-read only to gate a save-data toggle display; not body-verified |
| 107 | `btnCancelTestPauseClick` | 12297-12301 | 5 | (a) 顯示/唯讀 | btnCancelTestPauseClick: flag writes only (verified) |
| 108 | `btnESCFunctionClick` | 12303-12309 | 7 | (b) write path | btnESCFunctionClick: dispatches to a cross-form action |
| 109 | `SetCheckCodeByLot` | 12311-12324 | 14 | (a) 顯示/唯讀 | SetCheckCodeByLot: caption/enabled toggler for duplicate-check display |
| 110 | `ShowInformation` | 12326-12362 | 37 | (a) 顯示/唯讀 | ShowInformation: visibility + window geometry only (verified, fully read) |
| 111 | `ReadWriteFTPAutomationData` | 12364-12411 | 48 | (b) write path | ReadWriteFTPAutomationData: ReadWriteIni(bRead=false) writes config.ini -- name suggests read but the false branch writes (verified via signal; same pattern as ReadWriteLotInfo) |
| 112 | `CheckingCheckList` | 12413-12422 | 10 | (b) write path | CheckingCheckList: RecordProcess + delegates to GenerateCheckList |
| 113 | `GenerateCheckList` | 12424-13277 | 854 | (b) write path | GenerateCheckList (854L): heavy WriteIniData/ReadIniData (60/60), builds + persists the checklist file |
| 114 | `AlarmCodeUpload` | 13279-13308 | 30 | (b) write path | AlarmCodeUpload: RecordProcess + file write, uploads alarm code to FTP automation log |
| 115 | `RefreshYieldMonitor` | 13310-13325 | 16 | (b) write path | RefreshYieldMonitor: dispatcher; the else-branch (TERAPOWER, the DEFAULT when bSIGURDFunction=false) reaches a control-write (see :13547) |
| 116 | `RefreshYieldMonitor_SIGURD` | 13327-13545 | 219 | (a) 顯示/唯讀 | RefreshYieldMonitor_SIGURD: verified fully -- ReadIniData-only + widget display, reached only when bSIGURDFunction=true |
| 117 | `RefreshYieldMonitor_TERAPOWER` | 13547-13623 | 77 | (b) write path | **RefreshYieldMonitor_TERAPOWER: calls fCleaning->ChangeACSmartInterval(2,...) at :13618 under a condition -- changes AutoClean interval config, not pure display, despite "Refresh" name; this is the DEFAULT branch (non-SIGURD)** |
| 118 | `WriteFTPSetupFileChangeLog` | 13625-13634 | 10 | (b) write path | WriteFTPSetupFileChangeLog: file write |
| 119 | `AdjtsYieldMonitiorSize` | 13636-13717 | 82 | (a) 顯示/唯讀 | AdjtsYieldMonitiorSize: pure window-resize/tab layout (verified fully); calls back into RefreshYieldMonitor/RefreshAMR conditionally, inheriting their classification |
| 120 | `sbTestClick` | 13719-13752 | 34 | (c) 安全關鍵 | sbTestClick: opens on-screen keypad via ModalDoorHelpers + SOFT_SIMULTE-gated |
| 121 | `sbRecipeUploadClick` | 13754-13775 | 22 | (b) write path | sbRecipeUploadClick: dispatches into upload flow |
| 122 | `sbRecipeDownloadClick` | 13777-13782 | 6 | (b) write path | sbRecipeDownloadClick: dispatches into download flow |
| 123 | `sbFTPAutomationSaveClick` | 13784-13788 | 5 | (b) write path | sbFTPAutomationSaveClick: calls ReadWriteFTPAutomationData(false) -- write (verified) |
| 124 | `btnManualCheckListClick` | 13790-13834 | 45 | (b) write path | btnManualCheckListClick: triggers checklist generation |
| 125 | `btnFTPTryConnectClick` | 13836-13839 | 4 | (b) write path | btnFTPTryConnectClick: attempts FTP connection |
| 126 | `btTesterTCPShowClick` | 13841-13844 | 4 | (a) 顯示/唯讀 | btTesterTCPShowClick: fTesterTCP->Show() non-modal window (verified, benign) |
| 127 | `btnClearTemperatureClick` | 13846-13850 | 5 | (b) write path | btnClearTemperatureClick: sets bChangeTest_TempOffset + calls DoCheckHasTestTempChange() -- affects temperature setpoint state (verified call chain, conservative b/c call) |
| 128 | `SaveGroundESDByLot` | 13852-13908 | 57 | (b) write path | SaveGroundESDByLot: file write (ESD log) |
| 129 | `AutoTempOfsByFTP` | 13910-14052 | 143 | (b) write path | AutoTempOfsByFTP: FTP + ini read/write, temperature auto-offset |
| 130 | `btnManualStandardClick` | 14054-14058 | 5 | (b) write path | btnManualStandardClick: triggers manual checklist/standard action |
| 131 | `edStationNumMouseDown` | 14060-14064 | 5 | (a) 顯示/唯讀 | edStationNumMouseDown: opens on-screen keypad |
| 132 | `btnGetLoaderClick` | 14066-14072 | 7 | (b) write path | btnGetLoaderClick: dispatches to DoReadRFID (RFID hardware read + file write) |
| 133 | `DoReadRFID` | 14074-14095 | 22 | (b) write path | DoReadRFID: reads RFID hardware, writes result to file/caption |
| 134 | `InitRFIDRS232` | 14097-14150 | 54 | (b) write path | InitRFIDRS232: opens/configures the RFID serial COM port (RFID_Reader->StartComm) -- hardware peripheral init, not display (verified) |
| 135 | `RFID_ReaderReceiveData` | 14152-14173 | 22 | (b) write path | RFID_ReaderReceiveData: writes received RFID data to file |
| 136 | `btnLoadFileClick` | 14175-14184 | 10 | (a) 顯示/唯讀 | btnLoadFileClick: standard OS file-picker dialog for later use (verified, benign) |
| 137 | `spOCRCleanListClick` | 14186-14191 | 6 | (b) write path | spOCRCleanListClick: file write |
| 138 | `bCheckOnlyOneFile` | 14193-14213 | 21 | (a) 顯示/唯讀 | bCheckOnlyOneFile: filesystem directory listing read-only check (verified fully) |
| 139 | `bCheckOnlyOneFileAndData` | 14215-14296 | 82 | (b) write path | bCheckOnlyOneFileAndData: file write (2) |
| 140 | `SetTesterStartTimeByB03` | 14298-14309 | 12 | (a) 顯示/唯讀 | SetTesterStartTimeByB03: writes in-memory AnsiString globals only, no persistence in this function (verified) |
| 141 | `ProductTesterReport` | 14311-14459 | 149 | (b) write path | ProductTesterReport: file write (PTI tester report) |
| 142 | `btnAirStreamOnOffClick` | 14461-14488 | 28 | (c) 安全關鍵 | btnAirStreamOnOffClick: SystemStart-read gated air-stream toggle |
| 143 | `rgOEEStateClick` | 14490-14513 | 24 | (c) 安全關鍵 | rgOEEStateClick: SystemStart-read gated OEE state change + RecordProcess |
| 144 | `edtLotVerifyMouseDown` | 14515-14520 | 6 | (a) 顯示/唯讀 | edtLotVerifyMouseDown: mirrors TestIF_File.sLotIDSubstr to display (verified) |
| 145 | `edtLotVerifyKeyPress` | 14522-14526 | 5 | (a) 顯示/唯讀 | edtLotVerifyKeyPress: same mirror (verified) |
| 146 | `BtnPauseClick` | 14528-14531 | 4 | (b) write path | BtnPauseClick: dispatches into the Tester-pause action |
| 147 | `BtnPauseMouseDown` | 14533-14537 | 5 | (a) 顯示/唯讀 | BtnPauseMouseDown: cosmetic Down=true only (verified) |
| 148 | `BtnPauseMouseUp` | 14539-14543 | 5 | (a) 顯示/唯讀 | BtnPauseMouseUp: cosmetic Down=false only (verified) |
| 149 | `ResetLotInfo` | 14545-14559 | 15 | (b) write path | ResetLotInfo: reads RMS info + **sets LastSet.bHasDownloadFile=true, part of the persisted LastSet/lastdata state per KNOWLEDGE.md gotcha -- treat as write despite being an in-memory assignment here** |
| 150 | `ShowATCTempPanel` | 14561-14606 | 46 | (a) 顯示/唯讀 | ShowATCTempPanel: pure panel layout/visibility by head-count config (verified fully) |
| 151 | `CheckNoRetestBinFlag` | 14608-14630 | 23 | (a) 顯示/唯讀 | CheckNoRetestBinFlag: returns a bool from config, no signals |
| 152 | `btnPATInstallClick` | 14632-14695 | 64 | (b) write path | btnPATInstallClick: triggers PAT install action |
| 153 | `cbRTCASTDClick` | 14697-14708 | 12 | (b) write path | cbRTCASTDClick: RecordProcess |
| 154 | `ReadAirMachineStatus` | 14710-14803 | 94 | (b) write path | ReadAirMachineStatus: **writes Temperature.fSetTempature2AirMachine (a process-setpoint global) despite "Read" name -- verified partial read** |
| 155 | `CheckAirMachineStatus` | 14805-14868 | 64 | (a) 顯示/唯讀 | CheckAirMachineStatus: pure Caption display from ATC_InterfaceForm->AirMachineInfo (verified fully, overriding the conservative SystemStart-read default) |
| 156 | `SetCloseSiteTemp` | 14871-14875 | 5 | (c) 安全關鍵 | **SetCloseSiteTemp: calls fTemp_Set->SetSingleWorkTemperature(i,dTemp) for every ATC site -- a direct temperature-controller setpoint command, not display (verified)** |
| 157 | `InitialRefrigerantSystem` | 14877-14939 | 63 | (a) 顯示/唯讀 | InitialRefrigerantSystem: wires named designer panels/labels into runtime arrays + visibility only (verified fully) |
| 158 | `ScanRefrigerantSystem` | 14941-14967 | 27 | (a) 顯示/唯讀 | ScanRefrigerantSystem: grepped clean of Set*/SW[]/Sen[]; not fully body-read |
| 159 | `ATC_OFFLINE_FormComInit` | 14969-14992 | 24 | (a) 顯示/唯讀 | ATC_OFFLINE_FormComInit: grepped clean; not fully body-read |
| 160 | `RefreshRefrigerantAllStatus` | 14994-15078 | 85 | (a) 顯示/唯讀 | RefreshRefrigerantAllStatus: grepped clean; not fully body-read |
| 161 | `TransformTemperature_AirStream` | 15080-15193 | 114 | (a) 顯示/唯讀 | TransformTemperature_AirStream: pure lookup-table calculation, returns a double, no widget/global writes (verified header + body sample) |
| 162 | `SaveGroundESDData_Upolad` | 15197-15377 | 181 | (b) write path | SaveGroundESDData_Upolad: file write (ESD/ground log) |
| 163 | `RMSDownloadByNetwork` | 15379-15390 | 12 | (b) write path | RMSDownloadByNetwork: CopyFile (verified) |
| 164 | `RMSDownloadByFTP` | 15392-15416 | 25 | (b) write path | RMSDownloadByFTP: real FTP download via TfFTP (verified fully; missed by regex due to TfFTP case) |
| 165 | `RMSUploadByNetwork` | 15418-15421 | 4 | (b) write path | RMSUploadByNetwork: delegates to RMSDownloadByNetwork (verified) |
| 166 | `RMSUploadByFTP` | 15423-15447 | 25 | (b) write path | RMSUploadByFTP: real FTP upload via TfFTP (verified fully) |
| 167 | `GetFTP_SettingN06` | 15449-15460 | 12 | (a) 顯示/唯讀 | GetFTP_SettingN06: reads ini/globals into out-params, no write |
| 168 | `DoUpload` | 15462-15673 | 212 | (b) write path | DoUpload: ShowModal + file writes, recipe upload flow |
| 169 | `cbPATModeChange` | 15675-15684 | 10 | (b) write path | cbPATModeChange: dispatches PAT mode change |
| 170 | `UpdateLotInfoPAT` | 15686-15708 | 23 | (b) write path | UpdateLotInfoPAT: updates PAT-related display/state |
| 171 | `VisibleUploadBtnPAT` | 15710-15714 | 5 | (a) 顯示/唯讀 | VisibleUploadBtnPAT: Down/Visible toggler only (verified fully) |
| 172 | `UpdatePATSubMode` | 15716-15743 | 28 | (b) write path | UpdatePATSubMode: file write (4) |
| 173 | `sbUploadPATClick` | 15745-15773 | 29 | (b) write path | sbUploadPATClick: dispatches to DoUpload |
| 174 | `sbLotResetClick` | 15775-15778 | 4 | (b) write path | sbLotResetClick: dispatches into a Lot reset action; not body-verified but name implies state change |
| 175 | `leRunCardNumberMouseDown` | 15780-15784 | 5 | (a) 顯示/唯讀 | leRunCardNumberMouseDown: not body-verified, zero signals, presumed field-focus helper |
| 176 | `ClearLotInfoPAT` | 15786-15790 | 5 | (b) write path | ClearLotInfoPAT: dispatches into PAT-clear (cross-form) |
| 177 | `btnRealTimeClick` | 15792-15798 | 7 | (b) write path | btnRealTimeClick: dispatches PAT real-time mode |
| 178 | `btnpatHourlyClick` | 15800-15806 | 7 | (b) write path | btnpatHourlyClick: dispatches PAT hourly mode |
| 179 | `btnpatEndLotClick` | 15808-15814 | 7 | (b) write path | btnpatEndLotClick: dispatches PAT end-lot |
| 180 | `RefreshAMR` | 15816-15844 | 29 | (a) 顯示/唯讀 | RefreshAMR: caption display only (signal-based, not body-verified) |
| 181 | `ShowAMRCategoryBin` | 15846-15881 | 36 | (a) 顯示/唯讀 | ShowAMRCategoryBin: zero signals; not body-verified, presumed display given name+cluster |
| 182 | `btnManualI49Click` | 15883-15926 | 44 | (c) 安全關鍵 | btnManualI49Click: SystemStart-read gated |
| 183 | `cbRunModeDropDown` | 15928-15951 | 24 | (a) 顯示/唯讀 | cbRunModeDropDown: customer-gated dropdown population |
| 184 | `JCETWhite2DIDShow` | 15953-15992 | 40 | (a) 顯示/唯讀 | JCETWhite2DIDShow: ReadIniData + ComboBox Items->Add only, verified fully (FileWrite signal was a false positive on ->Add()) |
| 185 | `cbFirstTrayCheckOnUnloaderClick` | 15994-16027 | 34 | (b) write path | cbFirstTrayCheckOnUnloaderClick: dispatches to SetFirstTrayCheckOnUnloader which writes IniConfig.bP62Auto* |
| 186 | `cbFirstTrayCheckOnUnloaderMouseDown` | 16029-16034 | 6 | (a) 顯示/唯讀 | cbFirstTrayCheckOnUnloaderMouseDown: not body-verified |
| 187 | `SetFirstTrayCheckOnUnloader` | 16036-16045 | 10 | (b) write path | **SetFirstTrayCheckOnUnloader: writes IniConfig.bP62Auto1/2/3 (verified) -- config state write, not pure display despite being a "Set...Check" display-adjacent name** |
| 188 | `RefreshOtherTool` | 16047-16072 | 26 | (a) 顯示/唯讀 | RefreshOtherTool: not body-verified, zero signals |
| 189 | `btnFTPDownLoadbyDeviceIDClick` | 16074-16077 | 4 | (b) write path | btnFTPDownLoadbyDeviceIDClick: dispatches into device-list FTP download |
| 190 | `rgUnloaderClick` | 16080-16083 | 4 | (b) write path | rgUnloaderClick: dispatches an unloader-mode change |
| 191 | `ASECL_LotEnd` | 16085-16153 | 69 | (b) write path | ASECL_LotEnd: RecordProcess + MessageBox, Lot-End variant |
| 192 | `btnASECL_LotEndClick` | 16155-16158 | 4 | (b) write path | btnASECL_LotEndClick: dispatches to ASECL_LotEnd |
| 193 | `btnAMRSupplementClick` | 16161-16170 | 10 | (b) write path | btnAMRSupplementClick: EventReport (SECS AGV supplement request) |
| 194 | `btnAMRLDUnLDCheckClick` | 16172-16177 | 6 | (b) write path | btnAMRLDUnLDCheckClick: EventReport |
| 195 | `btnAMRLDUnLDFinishClick` | 16179-16184 | 6 | (b) write path | btnAMRLDUnLDFinishClick: EventReport |
| 196 | `labNowTrayCountClick` | 16186-16190 | 5 | (a) 顯示/唯讀 | labNowTrayCountClick: caption-only |
| 197 | `btnAMRClearCountClick` | 16192-16225 | 34 | (b) write path | btnAMRClearCountClick: RecordProcess, clears AMR counters |
| 198 | `btnAMRSetSECSClick` | 16227-16232 | 6 | (b) write path | btnAMRSetSECSClick: dispatches an SECS-related AMR action |
| 199 | `CheckActionFlag` | 16234-16246 | 13 | (a) 顯示/唯讀 | CheckActionFlag: pure LED->Value display from flag globals (verified fully) |
| 200 | `InitialUnLoaderTask` | 16250-16253 | 4 | (a) 顯示/唯讀 | InitialUnLoaderTask: iUnloaderTask[iPos]=1 trivial state array write (verified; already real in V906 facade) |
| 201 | `InitialLoaderTask` | 16259-16262 | 4 | (a) 顯示/唯讀 | InitialLoaderTask: iLoaderTask[iPos]=1 trivial (verified, symmetric to above) |
| 202 | `InitialLDLevelTask` | 16264-16267 | 4 | (a) 顯示/唯讀 | InitialLDLevelTask: iloaderLevelTask[iPos]=1 trivial (verified, symmetric to above) |
| 203 | `LoaderAction` | 16270-16363 | 94 | (b) write path | LoaderAction: EventReport (SECS AGV notify), sensor-gated |
| 204 | `UnLoaderAction` | 16366-16427 | 62 | (b) write path | UnLoaderAction: EventReport, sensor-gated |
| 205 | `CheckLDLevel` | 16429-16463 | 35 | (b) write path | CheckLDLevel: reads Sen[] hardware sensor, EventReport SECS send (verified fully) |
| 206 | `CheckAMRAction` | 16465-16477 | 13 | (b) write path | CheckAMRAction: dispatcher calling LoaderAction/UnLoaderAction/CheckLDLevel (verified fully; inherits their classification) |
| 207 | `ReflashInfo` | 16479-16485 | 7 | (a) 顯示/唯讀 | ReflashInfo: pure Caption display from count globals (verified fully) |
| 208 | `Timer4Timer` | 16487-16493 | 7 | (a) 顯示/唯讀 | Timer4Timer: not body-verified, zero signals, presumed a tick dispatcher |
| 209 | `SetAQLMode` | 16496-16597 | 102 | (b) write path | SetAQLMode: RecordProcess x5, AQL mode integration (Eastsun 20260520) |
| 210 | `ATCTransferFileTimeTimer` | 16599-16612 | 14 | (b) write path | ATCTransferFileTimeTimer: FTP transfer timer |


## 2. 孤兒宣告（golden .h 有宣告，golden 全樹查無定義／呼叫）

| 宣告 | golden .h 行號 | 查證指令 | 結果 |
|------|----------------|----------|------|
| `void VTestSummaryReport();` | uLotInfo.h:1348 | `grep -rn "TfLotInfo::VTestSummaryReport" HT9011UC_Code_V3.33.906.0_20260618/`（20260819） | 0 筆定義。uLotInfo.cpp 內僅有 4 處 `fMesSystem->VTestSummaryReport()`（:2179,2183,2192,2196）——那是**另一個 class（TfMesSystem）自己的同名方法**，不是 TfLotInfo 的。`fLotInfo->VTestSummaryReport()` 全樹 0 筆呼叫。 |
| `unsigned int GET_CRC_16(unsigned char*, unsigned char);` | uLotInfo.h:1276 | `grep -rn "TfLotInfo::GET_CRC_16\|GET_CRC_16(" HT9011UC_Code_V3.33.906.0_20260618/` | 全樹只有 uLotInfo.h 這一行宣告，**0 筆定義、0 筆呼叫**。 |

兩者都不落在任何 `#if 0` 區塊內（golden 全檔 `#if 0`/`#ifdef 0` 計數為 0，見第 6 節）。這兩個成員如果從未被 `fLotInfo->` 呼叫過，BCB6 因為 C++ 允許宣告未用的成員函式而不會報錯；**翻譯時不要因為看到宣告就去猜函式該做什麼，golden 沒有實作可抄，這兩個不用翻，也不算「翻譯缺漏」**。

## 3. 四分類統計

| 分類 | 函式數 | golden 行數（含括號內大括號） |
|------|--------|-------------------------------|
| (a) 顯示側/唯讀可翻 | 78 | 2,192 |
|   -- 其中已逐行覆核（Tier 1） | 39 | 1,199 |
|   -- 其中僅關鍵字掃描乾淨、未逐行覆核（Tier 2） | 39 | 993 |
| (b) write path（佇列） | 105 | 8,715 |
| (c) 安全關鍵（佇列） | 24 | 3,577 |
| MIXED（需先拆分才能歸類） | 2（FormShow 918L, Timer2Timer 258L） | 1,176 |
| (a)?未逐行覆核且與同系列函式行為不一致 | 1（ShowATC20Thermo） | 486 |
| **合計** | **210** | **16,146**（另有函式間空行/分隔註解未計入 span，golden 檔案本體 16,613 行） |

分母說明：210 = `TfLotInfo::` 全部方法數（不含 2 個孤兒宣告，因為它們沒有 span 可言）；16,146 = 210 個 span 的行數總和；ctor/dtor 各算 1 個方法。

## 4. 關鍵發現：本檔「Show/Refresh」命名前綴不可信，需逐行讀完才能歸類

波次一開始的假設是「函式名稱裡有 Show/Refresh/Visible 大概率是純顯示」，關鍵字掃描一開始也顯示這整個檔案 `MotorRef`（`M<Axis>->` 型態的馬達呼叫）為 0 筆——這個「零馬達」的結論**在波次進行中被推翻兩次**，且發現了另外 3 個「顯示側」函式其實藏著真實控制寫入。以下四個是本波次逐行讀出來的具體反例，每一個都有精確行號：

1. **`ShowATC70Thermo`**（uLotInfo.cpp:5520-5857）第 5790、5818 行呼叫 `ATCInterfaceForm->SetRunATC(false)` / `SetRunATC(true)`——這是啟停 ATC 溫控迴路的真實指令，不是顯示。
2. **`ShowNewATCThermo`**（uLotInfo.cpp:6346-6932）第 6469 行在條件式內呼叫 `fLotInfo->SetATCOffset(true, true)`——會觸發真正的 ATC offset 控制邏輯。
3. **`RefreshYieldMonitor_TERAPOWER`**（uLotInfo.cpp:13547-13623）第 13618 行呼叫 `fCleaning->ChangeACSmartInterval(2, "RefreshYieldMonitor")`——會改掉 Smart Auto Clean 的清潔週期設定，而且這是 `RefreshYieldMonitor` 分派邏輯（uLotInfo.cpp:13310-13325）在 `IniConfig.bSIGURDFunction==false` 時的**預設分支**（非 SIGURD 客戶碼才會走的分支，也就是大多數客戶碼的路徑），不是邊緣案例。
4. **`SetLotComponents`**（uLotInfo.cpp:2283-2320）表面上是 27 個 widget 的 `->Enabled`/`->Caption` 切換，但第 2285 行 `RunInfo.bLotStart=!bLotEnd;` 是全域旗標寫入，後面 SECS `DoLotStart` 的去重邏輯依賴這個旗標（見 `ht9045-lotinfo-flow` skill 第 5 節）。這種**純賦值式的全域狀態寫入沒有任何關鍵字特徵**，自動掃描完全抓不到，只能靠逐行讀。

另外 3 個「Read/Set」命名但語意相反的例子：

- **`ReadAirMachineStatus`**（uLotInfo.cpp:14710-14803）第 14728 行寫 `Temperature.fSetTempature2AirMachine=...`——名字叫 Read，實際上會算出並寫入一個製程設定值全域變數。
- **`SetCloseSiteTemp`**（uLotInfo.cpp:14871-14875）對每個 ATC site 呼叫 `fTemp_Set->SetSingleWorkTemperature(i, dTemp)`——直接命令溫控器設定工作溫度，是本檔案裡少數「函式名稱就老實承認會 Set」但仍被我一開始誤放進顯示候選清單的例子。
- **`SetFirstTrayCheckOnUnloader`**（uLotInfo.cpp:16036-16045）寫 `IniConfig.bP62Auto1/2/3`，是設定狀態寫入，不是純顯示。

**結論**：本檔案的翻譯波次規劃，不能只用「函式名稱 + 關鍵字掃描」做分類就排入 Wave A；任何要排入唯讀顯示波的候選，都必須先逐行讀過整個函式本體（含它呼叫的一層 helper），且要特別注意「純賦值到一個看起來語意重大的全域變數」這種掃描抓不到的模式。


## 5. Wave A 提案

**與 uYieldMonitoring 的釐清（避免混淆）**：V906 樹裡已有 `uYieldMonitoring.cpp`（FW-3 波次 20260818 已翻譯一部分），但那個 golden 來源是**另一個檔案** `HT9011UC_Code_V3.33.906.0_20260618/uYieldMonitoring.cpp`（5,981 行，`class TfYieldMonitoring`），跟本 recon 談的 `TfLotInfo::RefreshYieldMonitor` / `_SIGURD` / `_TERAPOWER`（在 `uLotInfo.cpp`）是**不同類別、不同檔案**。兩者唯一的關聯是 `uYieldMonitoring.cpp` 會讀寫 `fLotInfo->Label17/Label18/Label21/edtAutoCleanLowYield/edtAutoCleanSiteYieldDiff` 這 5 個跨 form 欄位（這也是它們為什麼已經在 `forms/fLotInfo.h` 出現的原因）。承接本 recon 的人請不要把兩個「Yield Monitor」搞混。

**與 DFM→WEB (FW) 戰役的釐清**：`tools/dfm2rc/layout_out/uLotInfo_layout.gen.cpp`（1,165 行）與 `tools/dfm2rc/ir_out/uLotInfo.dfm.ir.json`（79,128 行）已經是 `uLotInfo.dfm` 的機器抽取結果（widget 幾何/屬性），這是 FW 戰役的產物，**不等於** `forms/fLotInfo.h` facade 裡有對應的 pointer 成員。下面列的「facade 缺欄位」是指 `forms/fLotInfo.h` 裡沒有可供 `.cpp` 邏輯 dereference 的成員，不是說 widget 資訊完全不存在——DFM IR 可以加速產生欄位宣告，但目前沒有自動接線。

### 5.1 Tier 1 -- 已逐行覆核、建議可排入下一個顯示側翻譯波（39 個方法，1,199 golden 行）

- **`RefreshYieldMonitor_SIGURD`**（219L）: 僅 ReadIniData + widget 顯示，`IniConfig.bSIGURDFunction==true` 才會走到；依賴約 36 個 widget（gbManualCheckList/btnManualStandard/cbMonitor_FTPRMS/cbSiteYieldCmp_FT/edSiteYieldCmp*/edLowYieldByTotal*/rbContsFailBy*/edContsFail*/edOSBin*/grpOSBin 等），**facade 全缺**；`fContact->cbContactMode`（fContactCT.h 已存在但成員需查）；`TestIF_File`/`BinSelect`/`GetLastOpenFN`/`ReadIniData`/`CUSTOMER_CODE`/`AccessLevel` 均**已存在**於 V906。
- **`TransformTemperature_AirStream`**（114L）: 純查表計算回傳 double，依賴 `fMain->edWorkTemperBase->Text`（fMain facade已存在，成員需查）；無其他依賴。
- **`AdjtsYieldMonitiorSize`**（82L）: 純視窗resize/分頁版面；依賴 `pgLotinfo` 分頁集合與約 14 個 tab-sheet（`tsATC`/`tsYieldMonitior`/`tsAMR`/`tsFTP`/`tsLotID`/... ）**facade全缺**；會條件式呼叫回 `RefreshYieldMonitor()`/`RefreshAMR()`，繼承其分類（後者已排除見 5.2）。
- **`SetSelectionVisible`**（81L）: 純tab/群組可見度 + AccessLevel enable；依賴 `groupbDownloadItem`/`grpMesCheck`/`tsSelection`/`Panel28`/`cbTestTimes`/`lblTestTimes`/`lblDownloadAccessWarning`，**facade全缺**；`IniConfig`/`AccessLevel`/`CUSTOMER_CODE`已存在。
- **`ShowSocketID`**（74L）: 純 site/channel 網格 Caption/Visible；依賴 4 個 widget 陣列 `SocketSiteCH_Display[MAX_SOCKET_ROW][MAX_SOCKET_COL]`/`SocketLabRow_Display[]`/`SocketLabCol_Display[]`/`edSocket[][]`，**facade全缺**；`TestIF_File.iSiteMap`/`SiteData`已存在。
- **`CheckAirMachineStatus`**（64L）: 純 Caption 顯示，來源 `ATC_InterfaceForm->AirMachineInfo`/`.AirMachineInfo_Index`（ATC_InterfaceForm 類別已存在於 `ATC/ATCInterface.cpp`，該 2 個成員需查）；依賴 `palAirMachineSetTemperature`/`palAirMachineSetTemperature_Index`，**facade缺**。
- **`InitialRefrigerantSystem`**（63L）: 純 widget 陣列接線（9組×8個 designer-named panel/label → 執行期陣列）+ Visible；依賴 `pnlRefrigerantMachine1-8`/`LabRefrigerantValue1-8`/`pnlRefCopm1/2Status_1-8`/`labRefCopm1/2HpValue_1-8`/`labRefCopm1/2LpValue_1-8`/`LabRefrigerantAdjustValue1-8`（共約72個 widget）+ 9個目的陣列（golden .h 已宣告：`TripnlRefrigerantMachine[8]`等），**facade全缺**；`AirStream_Select`/`Total_Compressor`需查。
- **`SetATCFormVisible`**（48L）: 純 tab caption/visibility by ATC_SYSTEM；依賴 `tsATC`/`palATC`/`aldATC7Status`/`lblATC70`/`aldATCChillerStatus`/`lblChiller`/`lblATC_Now_RecipeFile`/`NetATCTime`(Timer)/`pan_ATCChillerSV`/`pl_ATCChillerSV`，**facade全缺**；`Tri_Temp_Machine`/`ATC_SYSTEM`/`Temperature.bATC70Active`需查。
- **`CheckEventLogParameter`**（48L）: 純必填欄位驗證回傳 bool；依賴 `edtASECL_LotID`(**已存在**)/`edInsertion`(**已存在**)/`edCustomerDevice`(**缺**)/`edFlowID`(**已存在**)/`btnASECL_LotStart`(**缺**)；`CUSTOMER_CODE`/`IniConfig`/`LastSet`已存在。
- **`ShowATCTempPanel`**（46L）: 純面板佈局/可見度 by head count；依賴 `Pan_ATC_Use_4Head/8Head/32Head`、golden .h 已宣告的 `ATCChPal[ATC_HEAD_COUNT]`/`ATCPtr[ATC_HEAD_COUNT]`/`ATCReferPtr[ATC_HEAD_COUNT]`，**facade全缺**；`iATC_Use_Heat_Count`/`Temperature.bUseReferTempSensor`需查。
- **`JCETWhite2DIDShow`**（40L）: ReadIniData + ComboBox Items 填充；依賴 `cbRunMode`(**已存在**)/`edtCusLotID`(**已存在**)/`labCusLotID`(**缺**)。三者中兩個已在 facade，缺口最小的一個 Wave A 候選。
- **`ShowInformation`**（37L）: 純可見度 + 視窗geometry；依賴 `gbFTPAutomation_Download/Upload`/`sbRecipeUpload/Download`/`sbFTPAutomationSave`（**facade缺**）、`fLotInfo->Height/Width/Top/Left`（facade的最小 stand-in class目前無這些幾何屬性，需先加）。
- **`N23UseLotInfoFile`**（35L）: 讀文字檔寫入欄位；依賴 `mmo2DLotInfo`(**已存在**)/`edtCusLotID`(**已存在**)/`edtCusDevGrp`(**已存在**)/`edtDevice`(**已存在**)——四個全部已在 facade！僅缺 `IniConfig.bN23UseLotInfoFile`/`sN23LotInfoPath`欄位需查。
- **`edTempKeyUp`**（34L）: 純文字處理（條碼字尾修剪）；依賴 `edTemp`(**缺**)、`bLotFirstKeyIn`（全域旗標，需查）。
- **`edtSysLotIDKeyPress`**（26L）: 條碼終止字元處理+focus鏈；依賴 `edtSysLotID`(**已存在**)/`edPage`(**缺**)、`bLotID_OK`（全域旗標，需查）。
- **`SettsChipAdvVisible`**（21L）: 純tab visibility/caption；依賴 `tsChipAdv`（**缺**）。
- **`bCheckOnlyOneFile`**（21L）: 純檔案系統目錄列舉（TFileListBox），無 widget/全域依賴，僅本地變數，**幾乎零依賴，最適合當第一個翻譯示範**。
- **`edPageKeyPress`**（15L）: 同 edtSysLotIDKeyPress 的 focus鏈模式；依賴 `edPage`(**缺**)/`edtSysOperatorID`(**已存在**)。
- **`CheckActionFlag`**（13L）: 純 LED->Value 顯示；依賴 `ledLoader`/`ledEmpty`/`ledColor`/`ledAuto1/2/3`/`ledStartAGV`/`ledSTART`/`ledLoaderTotalTray`/`ledLOT_START`（9個LED widget，**facade全缺**）+ `bLoaderActionFlag[3]`/`bUnLoaderActionFlag[3]`/`bAMRReceiveAGVStart`/`bAMRReceiveStart`/`bAMRReceiveLoaderTotalTray`/`RunInfo.bLotStart`（RunInfo 已存在於V906，其餘全域旗標需查）。
- **`SetTesterStartTimeByB03`**（12L）: 純寫入記憶體內 AnsiString 全域（`sB03RunData`/`sB03StartTime`，需查是否已存在）；`IniConfig.bB03_TesterReport`/`GetTimeInfo()`已存在。
- **`btnLoadFileClick`**（10L）: 標準 OpenDialog 檔案選取；依賴 `OpenDialog1`(**缺**)/`edSort2DIDBinFile`(**缺**)。
- **`cbRunModeKeyDown`**（8L）: 阻擋手動輸入(JCET白名單模式)；依賴 `cbRunMode`(**已存在**)、`fBarCode->JCETUseMakeWhite2DIDList()`(**fBarCode facade需查**)。
- **`cbRunModeKeyUp`**（8L）: 同上，鏡像guard。
- **`edDeviceNameKeyUp`**（8L）: 僅設定 `bLotFirstKeyIn=true`（全域旗標，需查），無 widget 依賴。
- **`ReflashInfo`**（7L）: 純 Caption 顯示；依賴 `labLoaderTrayCount`(**已存在**)/`labAuto1/2/3TrayCount_KYEC`(**缺**)、`iLoaderTrayCountCal`/`iUnloaderTrayCountCal[3]`（全域，需查）。
- **`labLotIDMouseDown`**（6L）: 開虛擬鍵盤(觸控)；依賴 `fQwertyKey->ShowQwertyKey(...)`（fQwertyKey facade需查是否存在）、`edtSysLotID`(**已存在**)。
- **`edtLotVerifyMouseDown`**（6L）: 純鏡射顯示；依賴 `edtLotVerify`(**缺**)、`TestIF_File.sLotIDSubstr`(**已存在的struct，此欄位需查**)。
- **`btnCancelTestPauseClick`**（5L）: 純旗標寫入 `bTesterSendPause`/`bTesterPauseMusic`（需查），無 widget 依賴。
- **`edtLotVerifyKeyPress`**（5L）: 同 edtLotVerifyMouseDown。
- **`BtnPauseMouseDown`**（5L）: 純 `BtnPause->Down=true`（**缺**）。
- **`BtnPauseMouseUp`**（5L）: 純 `BtnPause->Down=false`（**缺**）。
- **`VisibleUploadBtnPAT`**（5L）: 純 `sbUploadPAT->Down/Visible`（**缺**）。
- **`cbbDeviceNameChange`**（4L）: 純鏡射 `edDeviceName->Text=cbbDeviceName->Text`（兩者皆**缺**）。
- **`btStartCountClick`**（4L）: 純旗標 `bStartCount_SCK=true`（需查），零 widget 依賴。
- **`edtASECL_LotIDClick`**（4L）: 開虛擬鍵盤；依賴同 labLotIDMouseDown 的 fQwertyKey + `edtASECL_LotID`(**已存在**)。
- **`btTesterTCPShowClick`**（4L）: `fTesterTCP->Show()`；`fTesterTCP` facade已存在（`Interface/TesterTCP.h`），僅需確認有 `Show()` 或等效no-op。
- **`InitialUnLoaderTask`**（4L）: `iUnloaderTask[iPos]=1`；**V906已有真實翻譯**（forms/fLotInfo.cpp:85，AI(W906-AutoCleanFoundation) 20260721）。
- **`InitialLoaderTask`**（4L）: `iLoaderTask[iPos]=1`；golden 全域 `int iLoaderTask[3]={1,1,1};`（uLotInfo.cpp:16256）需查是否已搬到 V906 全域，**facade方法本身未翻**。
- **`InitialLDLevelTask`**（4L）: `iloaderLevelTask[iPos]=1`；同上，golden 全域 `int iloaderLevelTask[3]={1,1,1};`（uLotInfo.cpp:16257）。

Tier 1 小結：39 個方法、1,199 golden 行，**每一個都已逐行讀過完整本體**，沒有發現隱藏的 motor/IO/SECS/檔案寫入。依賴面最大的是 `RefreshYieldMonitor_SIGURD`（~36 個 widget 全缺）與 `InitialRefrigerantSystem`（~72 個 widget 全缺）；依賴面最小、建議優先當示範翻譯的是 `bCheckOnlyOneFile`（零依賴）、`N23UseLotInfoFile`（4 個依賴全部已在 facade）、`JCETWhite2DIDShow`（3 個依賴 2 個已在 facade）。

### 5.2 Tier 2 -- 關鍵字掃描乾淨但未逐行覆核（39 個方法，993 golden 行）

這些函式的自動掃描結果乾淨（無 EventReport/WriteIniData/FTP/RecordProcess/fopen/IOSwitch/IOSensor/ShellExec/ReadWriteIni/ModalDoorHelpers 命中），但**依第 4 節的教訓，掃描乾淨不等於安全**，排入任何波次前必須先逐行讀過。清單（依行數）：

TfLotInfo(ctor,143L)、RefreshRefrigerantAllStatus(85L)、LoadRTCFullViewImg(83L)、pgLotinfoChange(46L)、edtSysOperatorIDMouseUp(40L)、edtSysLotIDMouseUp(36L)、ShowAMRCategoryBin(36L)、edtSysLotIDKeyUp(31L)、edtSysOperatorIDKeyPress(29L)、RefreshAMR(29L)、ScanRefrigerantSystem(27L)、edtSysOperatorIDKeyUp(26L)、RefreshOtherTool(26L)、btChangeFileClick(25L)、cbRunModeKeyPress(24L)、ATC_OFFLINE_FormComInit(24L)、cbRunModeDropDown(24L)、CheckNoRetestBinFlag(23L)、edDeviceNameMouseDown(22L)、CutTempToEdit(20L)、spOCRChangeFileClick(17L)、edDeviceNameKeyDown(17L)、edtSysLotIDKeyDown(16L)、edtSysOperatorIDKeyDown(16L)、FormDestroy(14L)、SetCheckCodeByLot(14L)、GetFTP_SettingN06(12L)、edTempKeyPress(11L)、edTempKeyDown(11L)、btnSaveDataClick(11L)、Timer3Timer(10L)、edPageMouseDown(8L)、Timer4Timer(7L)、cbFirstTrayCheckOnUnloaderMouseDown(6L)、edQAModeMouseDown(5L)、edStationNumMouseDown(5L)、leRunCardNumberMouseDown(5L)、labNowTrayCountClick(5L)、FormClose(4L)

### 5.3 不建議排入任何唯讀波次的相似命名陷阱（已逐行驗證，供對照）

`ShowATCThermo`/`ShowATC70Thermo`/`ShowNewATCThermo`（合計 981L）、`ShowATC20Thermo`（486L，未逐行讀但與手足行為不一致故不予採信）、`RefreshYieldMonitor`/`RefreshYieldMonitor_TERAPOWER`（合計 93L）、`SetLotComponents`（38L）、`ReadAirMachineStatus`（94L）、`SetCloseSiteTemp`（5L）、`SetFirstTrayCheckOnUnloader`（10L）——理由見第 4 節。


## 6. 陷阱段

### 6.1 客戶碼 gated 區塊

`grep -oE "CUSTOMER_CODE\s*==\s*CC_\w+"` 於 20260819 執行：全檔 **46 個不同客戶碼、共 235 處 `CUSTOMER_CODE==CC_*` 判斷**。前十名：

| 客戶碼 | 出現次數 |
|--------|----------|
| `CC_Murata` | 20 |
| `CC_SCC` | 18 |
| `CC_KYEC_LEE` | 17 |
| `CC_Greatek` | 13 |
| `CC_AMD_M` | 12 |
| `CC_PANTHER` | 11 |
| `CC_PTI` | 11 |
| `CC_TSMC_TAINAN` | 10 |
| `CC_SIGURD_PeiXing` | 10 |
| `CC_SIGURD_ChungXing` | 8 |

其餘 36 個客戶碼各出現 1-8 次不等（含 `CC_SCK`/`CC_UTAC_TW`/`CC_SJ_Semiconductor`/`CC_JCET`/`CC_ASE_KaohSiung`/`CC_CYUEAN`/`CC_TFME_CHINA`/`CC_TERAPOWER`/`CC_ASE_CL`/`CC_KYEC_XILINX`/`CC_AMKOR_Korea`/`CC_AMKOR_China`/`CC_QUALCOMM`/`CC_JSCC_OS`/`CC_GIGAS`/`CC_LEADYO`/`CC_TSI`/`CC_ChipMos_ZHUBEI`/`CC_ASE_SG`/`CC_VTEST` 等）。**任何一個 Wave A 候選函式如果客戶碼分支數 >5（例如 sbSECSLotStartClick 的 20 處），翻譯時務必逐一核對每個客戶分支，不能只翻預設分支。**

### 6.2 SOFT_SIMULTE 區塊

`grep -c "SOFT_SIMULTE"`（20260819）：全檔 15 處，5 個 `#ifdef SOFT_SIMULTE`、10 個 `#ifndef SOFT_SIMULTE`。出現在：`FormShow`(2)、`SetLotEnd`(2)、`ShowNewATCThermo`(2)、`edDeviceNameMouseDown`(1)、`edtSysLotIDKeyUp`(1)、`sbSECSLotStartClick`(1)、`sbTestClick`(1)、`DoReadRFID`(1)、`GetFTP_SettingN06`(1)、`UnLoaderAction`(3)。呼應 `docs/KNOWLEDGE.md` 既有陷阱：正式 build 關閉 `SOFT_SIMULTE`，這些分支在實機是死碼，但翻譯時仍要照翻並標註（不可假設「反正不會跑到」而省略）。

### 6.3 跨 form 呼叫目標清單（`f[A-Z]\w*->` 與 `ATC_InterfaceForm->`／`Command->` 計數，20260819）

| 目標 | 呼叫次數 | 目標 | 呼叫次數 |
|------|----------|------|----------|
| `ATC_InterfaceForm->` | 187 | `fMain->` | 166 |
| `fLotInfo->` | 108 | `fBarCode->` | 61 |
| `fSCKART->` | 39 | `fFTPClient->` | 36 |
| `fMesSystem->` | 32 | `fPassword->` | 23 |
| `fProductionInfo->` | 14 | `fObserver->` | 13 |
| `fShowBinSelect->` | 13 | `fTemp_Set->` | 12 |
| `fWinway->` | 11 | `fContact->` | 10 |
| `fQwertyKey->` | 10 | `fTesterTCP->` | 8 |
| `fOCR->` | 7 | `fSetup->` | 7 |
| `fNote->` | 7 | `fAGV->` | 5 |
| `fSecurity->` | 5 | `fSortCT->` | 3 |
| `fOffSet->` | 3 | `fGroundMan->` | 2 |
| `fCounterClear->` | 1 | `fStartCondition->` | 1 |
| `fMonitor->` | 1 | `fInput->` | 1 |
| `fARMS->` | 1 | `fQAMode->` | 1 |
| `fBuilder->` | 1 | `fCleaning->` | 1 |

`fLotInfo->` 自身 108 次是**同 form 內其他函式互call**（例如 `AdjtsYieldMonitiorSize`呼叫回`RefreshYieldMonitor`/`RefreshAMR`）。真正的外部依賴以 `ATC_InterfaceForm`(187)、`fMain`(166)、`fBarCode`(61)、`fSCKART`(39)、`fFTPClient`(36)、`fMesSystem`(32) 為最大宗——這 6 個是任何 uLotInfo 翻譯波次最常撞到的跨 form 依賴，其 V906 facade 完整度應優先查證。

## 7. 方法論註記與已知限制

### 7.1 逐條驗證方式（第 2 條硬規則要求）

對每個宣稱的 span，抽查方式為 `sed -n '<start>,<end>p' uLotInfo.cpp.cp950.txt` 印出原始行文字，核對開頭簽章（函式簽名字串）與結尾 `}` 是否對應；210 個 span 全部經此方式核對過起訖行號（腳本用大括號配對計算 end，但每個 start 行都有人工核對過函式名稱與簽名字串）。

### 7.2 已知限制（誠實揭露，不做「窮舉皆無」的宣稱）

1. **39 個 (a) 類函式未逐行覆核**（Tier 2，見 5.2），只做過關鍵字掃描；第 4 節已證明本檔案的掃描盲點包含「純賦值全域狀態」「三種不同名字的 ini helper」「大小寫相依的 FTP 類別名」「原生 C stdio」「IO 開關/感測器陣列」「虛擬鍵盤/登入側門 helper」共 6 類，且每類都在本波次逐一發現後才追加掃描規則——**不排除還有第 7 類盲點尚未發現**。
2. **(b)/(c) 佇列的 105+24 個函式只做了信號掃描 + 少量抽查（約 25 個直接讀過本體），未逐一讀完整個 write-path/safety 佇列**；本波次的重點是找出「可以安全排入 Wave A 的顯示側」，佇列本身的細節分類（例如哪些 write path 只是 log、哪些真的動到 SECS/ATC 控制）留給後續波次做，本文件的 (b)/(c) 一句話描述多數只到「有沒有寫檔/送SECS/碰互鎖」這個粗粒度。
3. **`FormShow`（918L）與 `Timer2Timer`（258L）標記 MIXED**，兩者都是「表單開啟」與「每秒 tick」這種天生異質的大函式，內含多段不同性質的邏輯（widget 初始化、config 讀取、部分寫入、彈窗），需要先做函式內分段（例如按 golden 的空行/註解區塊切）才能個別歸類，本 recon 不做這個切分。
4. **依賴欄位是否「已存在於 facade」的判斷基於 grep 存在性檢查，不是編譯驗證**——例如 `fContactCT.h` 存在不代表它有 `cbContactMode` 這個成員；Wave A 實作時仍需針對每個依賴逐一確認成員簽名，本文件只能說「facade 檔案存在/不存在」這個粗粒度。
5. 本 recon 未檢查 `uLotInfo.dfm` 本身（表單版面），僅用了 dfm2rc 既有產物的存在性（1,165 行 layout gen + 79,128 行 IR json）佐證「widget 幾何資訊不是從零開始」，未逐一核對哪些 widget 名稱已被抽取。

### 7.3 指令與時間戳記錄

- cp950 解碼 + U+FFFD 計數：20260819 15:19
- `grep -n "TfLotInfo::"` 原始命中計數（216）：20260819，同批次
- 函式邊界抽取（字串/註解遮罩 + 大括號配對）：20260819，210 個候選
- `grep -rn "TfLotInfo::VTestSummaryReport\|TfLotInfo::GET_CRC_16"` 全樹 0 筆：20260819
- 關鍵字訊號掃描（4 輪迭代，逐步加入 fopen/IOSwitch/IOSensor/ReadWriteIni/ShellExec/ModalDoorHelpers）：20260819
- 逐行覆核（約 65+ 個函式直接用 `sed -n` 讀出完整本體核對）：20260819
- V906 facade 現況核對（`forms/fLotInfo.h`/`fLotInfo.cpp` Read）：20260819
- 核心全域依賴存在性查證（`IniConfig`/`CosFunction`/`TestIF_File`/`SiteData`/`BinSelect`/`ArmDataLot`/`GetLastOpenFN`/`ReadIniData`/`WriteIniData`/`RunInfo`）：20260819，均以 `Grep --path D:\HT9045\HT9011UC_Cpp_V3.33.906.0` 執行並取得非零命中
- 客戶碼/SOFT_SIMULTE/跨form呼叫統計：20260819

**注意（依規則第3條）**：本文件所有「已存在/facade缺」的判斷都是本波次單獨執行時的快照；若有平行波次同時在改 `forms/fLotInfo.h` 或核心全域檔案，交接前請重跑上述 grep 指令確認未過期。


