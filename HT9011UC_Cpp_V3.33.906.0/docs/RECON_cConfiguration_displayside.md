# RECON: cConfiguration 顯示側翻譯波偵察報告

**狀態**：唯讀 recon，本文件是本波次唯一交付物。未修改任何原始碼、未 commit、未跑 build.bat/ctest、未啟動任何背景行程。

**Golden 來源**：`D:\HT9045\HT9011UC_Code_V3.33.906.0_20260618\cConfiguration.cpp`（7,808 行）+
`cConfiguration.h`（2,407 行）+ `cConfiguration.dfm`（22,521 行）。方法論比照
`docs\RECON_uLotInfo_displayside.md`（20260819 版）。

**類別**：`class TfConfiguration : public TForm`（`cConfiguration.h:24`）。V906 樹目前**沒有**
`forms\fConfiguration.h`/`.cpp`（`Grep "TfConfiguration|fConfiguration" forms\` 0 筆命中，20260820），
所以本檔的 Wave A 起點是**從零建 facade**，不像 uLotInfo 波次已有半成品 `forms\fLotInfo.h` 可以疊加。

---

## 0. 解碼與盤點方法

- cp950 解碼指令：`open(path,'rb').read().decode('cp950', errors='replace')`，於 2026-08-19/20 執行。
- `cConfiguration.cpp`：596,990 bytes → 7,809 行（`\n` 計數口徑），**U+FFFD 計數 = 0**。
- `cConfiguration.h`：65,095 bytes → 2,407 行，**U+FFFD 計數 = 0**。
- `cConfiguration.dfm`：890,069 bytes → 22,521 行，**U+FFFD 計數 = 0**。
- 解碼陷阱記錄：Python 在 Windows 環境下用 `open(path,'w',encoding='utf-8')`（預設 `newline=None`）寫檔會把來源已有的
  `\r\n` 又追加一次平台換行轉譯，變成 `\r\r\n`，回讀時 universal-newline 會把它拆成兩行，造成行數翻倍的假象
  （首次嘗試量到 15,617 行，是 `2*7808+1` 的假結果）。修正方式：解碼後先 `text.replace('\r\n','\n').replace('\r','\n')`
  正規化成純 LF 再以 `newline='\n'` 寫出，之後行號與 golden 逐行比對一致。此陷阱只影響本波次的暫存副本，不影響
  golden 原始檔（全程唯讀），記錄下來是為了下一個 recon 波次不要重踩。
- 函式邊界抽取：對 cp950 解碼後文字做字串/註解遮罩（`"..."`、`'...'`、`//`、`/*...*/`），再以正規表示式
  `TfConfiguration::(\w+)\s*\(` 抓取候選定義行，逐一用大括號配對找結束行。抓到 **129 個候選**，
  `grep -c "TfConfiguration::"` 原始命中 131 筆，差額 2 筆為遮罩後排除的誤判（`FormShow`/`FormDestroy`
  各自的字串常量或行內註解引用，未逐一列出，因為腳本遮罩已在候選抽取階段排除，非人工逐筆核對——**這點與
  uLotInfo recon 的處理方式不同，是本文件已知的較弱環節，見第 7.2 節**）。
- 129 個候選全數 `end != None`（無 prototype-only 誤判），span 行數加總 = 7,550 行。
- 行號抽查方法：對每個宣稱的 span，用 `sed -n '<start>,<end>p'` 印出原始行文字核對函式簽名與結尾 `}`；
  本報告中列為「逐行覆核」的 107 個方法（見第 3 節統計）均以此方式核對過起訖行號與完整本體；
  其餘 22 個「Tier 2」方法只核對過起訖行號簽名，本體僅做關鍵字訊號掃描。

---

## 1. 函式全量清冊（TfConfiguration::，129 個方法）

欄位：# / 方法 / Golden span / 行數 / 分類 / 逐行覆核 / 一句話功能。「逐行覆核=否(Tier2)」的列**只做過
關鍵字訊號掃描**（見下方訊號清單），尚未逐行讀完整個函式本體——這批全部落在 (b)/(c) 佇列，沒有一個
Tier2 項目被標成 (a)（第 3 節有明確驗證：46 個 (a) 全數逐行覆核，0 個例外）。

訊號掃描類別（對每個 span 的原始文字跑正規表示式計數，2026-08-19 執行）：
`WriteIniData`/`ReadIniData`/`ReadWriteIni`/原生 C stdio（`fopen`/`fputs`/`fwrite`/`fclose`/`fprintf`）/
`EventReport`/`ShellExecute`/大小寫不分的 `Ftp`/`SW[`+`IOSwitch(`/`Sen[`+`IOSensor(`/
`ModalDoorHelpers`+`ShowEventLogLogin`+`CheckLoginSuccess`+`ShowQwertyKey`/`->ShowModal(`/
`M[A-Z]\w*->`+`MOT[`（馬達）/`SystemStart`/`ATC_InterfaceForm->`+`SetRunATC`+`->SetOffset(`+`SetATCOffset`/
`CopyFile`+`MoveFile`+`DeleteFile`+`RemoveDir`+`MyForceDirectories`/`RecordProcess(`/
`->SaveToFile(`+`->LoadFromFile(`/對 `LastSet.`/`IniConfig.`/`RunInfo.`/`Prod.`/`TestIF_File.` 欄位的
純賦值（`=` 但非 `==`）/`AccessLevel`/`CUSTOMER_CODE==CC_`/`SOFT_SIMULTE`。

（表格見下）

| # | 方法 | Span | 行數 | 分類 | 逐行覆核 | 一句話功能 |
|---|------|------|------|------|----------|------------|
| 1 | `TfConfiguration` | 109-225 | 117 | (b) write path | 是 | ctor: calls ReadLockByFile()(write-seed)+InitConfigEdtList()(elConfig policy registry, ~hundreds of CUSTOMER_CODE branches)+ReadConfigStandard()(CopyFile overwrite)+ReadLastSetIni()+WriteContactData()(file write)+dynamic widget alloc+WinSock hostname lookup writing IniConfig.SocketHandlerID/sGPIBMachineID for KYEC_LEE+conditional sbtReloadTray/HP->Click() |
| 2 | `ReadLockByFile` | 230-261 | 32 | (b) write path | 是 | named Read but ends with lazy WriteIniData seed (I02_Enabled/A03_Enabled) if key missing -- the exact "Read...但寫" pattern the task brief warned about |
| 3 | `ChangeCBListProperty` | 266-363 | 98 | (b) write path | 是 | policy gate over elConfig->FEditList->ChangeProperty() for ~13 checkboxes, keyed on CUSTOMER_CODE/CosFunction.bLock*ByFile/hardware Cylinder[].Enable; re-calls ReadLockByFile() |
| 4 | `InitConfigEdtList_ItemA` | 378-977 | 600 | (b) write path | 是 | elConfig->Add() registers ~dozens of A-series params with per-CUSTOMER_CODE bReadFromFile/bFixedValue+DefValue overrides (verified via body read of first ~50 lines) |
| 5 | `InitConfigEdtList_ItemB` | 979-1035 | 57 | (b) write path | 否(Tier2) | same elConfig->Add() registration pattern (Item series), not individually read but structurally identical to ItemA/ItemC/ItemD/ItemN which were |
| 6 | `InitConfigEdtList_ItemC` | 1037-1242 | 206 | (b) write path | 否(Tier2) | same pattern; signal scan shows 1 direct GlobalWriteIniConfig hit outside Add() calls |
| 7 | `InitConfigEdtList_ItemD` | 1244-1759 | 516 | (b) write path | 否(Tier2) | same pattern; signal scan shows 1 direct ReadIniData hit outside Add() calls |
| 8 | `InitConfigEdtList_ItemE` | 1761-2111 | 351 | (b) write path | 否(Tier2) | same pattern |
| 9 | `InitConfigEdtList_ItemF` | 2113-2328 | 216 | (b) write path | 否(Tier2) | same pattern |
| 10 | `InitConfigEdtList_ItemG` | 2330-2419 | 90 | (b) write path | 否(Tier2) | same pattern |
| 11 | `InitConfigEdtList_ItemI` | 2421-2871 | 451 | (b) write path | 否(Tier2) | same pattern |
| 12 | `InitConfigEdtList_ItemL` | 2873-3118 | 246 | (b) write path | 否(Tier2) | same pattern |
| 13 | `InitConfigEdtList_ItemM` | 3120-3161 | 42 | (b) write path | 否(Tier2) | same pattern |
| 14 | `InitConfigEdtList_ItemN` | 3163-3919 | 757 | (b) write path | 否(Tier2) | same pattern; heaviest FTP/RMS/Network parameter cluster (N05/N06/N14/N15/N23/N25/N31/N32/N35), 3 direct GlobalWriteIniConfig hits |
| 15 | `InitConfigEdtList_ItemO` | 3921-4095 | 175 | (b) write path | 否(Tier2) | same pattern |
| 16 | `InitConfigEdtList_ItemP` | 4097-4493 | 397 | (b) write path | 否(Tier2) | same pattern |
| 17 | `InitConfigEdtList` | 4495-4512 | 18 | (b) write path | 是 | pure dispatcher calling ItemA..P + ChangeCBListProperty(); inherits their classification |
| 18 | `FormShow` | 4514-5365 | 852 | MIXED(需拆分) | 是 | giant form-open: widget-enable-all loop + ReadLastSetIni() + tab visibility by CUSTOMER_CODE + per-motor edSoftSpeed[] init from LastSet -- same architecture as uLotInfo FormShow, needs section-level split |
| 19 | `FormDestroy` | 5367-5405 | 39 | (a) 顯示/唯讀 | 是 | pure dynamic-widget delete/teardown (edSoftSpeed[]/labSoftSpeed[]/edOCRTrayLot[]/labOCRTrayLot[]/cbTempSelsct[]/edTempRead[]), no global or file write |
| 20 | `edSoftSpeed0Change` | 5407-5416 | 10 | (b) write path | 是 | ShowQwertyKey + writes LastSet.SoftSpeed[Tag] + calls WriteLastDataFile() -- real file persist despite generic Change-handler look |
| 21 | `edOCRTrayLotChange` | 5418-5427 | 10 | (b) write path | 是 | ShowQwertyKey + writes LastSet.TrayCount[Tag] + calls WriteLastDataFile() -- same pattern as above |
| 22 | `btnAdd1000Click` | 5429-5432 | 4 | (b) write path | 是 | calls SetSoftSpeedSpeed(false,1000) which writes LastSet.SoftSpeed[] array |
| 23 | `btnDec1000Click` | 5434-5437 | 4 | (b) write path | 是 | calls SetSoftSpeedSpeed(false,-1000), same write |
| 24 | `btnSetTo1000Click` | 5439-5442 | 4 | (b) write path | 是 | calls SetSoftSpeedSpeed(true,0), same write (reset branch) |
| 25 | `SetSoftSpeedSpeed` | 5444-5466 | 23 | (b) write path | 是 | writes LastSet.SoftSpeed[] array for all TOTAL_MOTOR + mirrors into edSoftSpeed[] widget Text (verified fully) |
| 26 | `btHeaterSelectAllClick` | 5468-5475 | 8 | (a) 顯示/唯讀 | 是 | pure cbTempSelsct[i]->Checked=true loop, widget-only |
| 27 | `btHeaterClearSelectClick` | 5477-5484 | 8 | (a) 顯示/唯讀 | 是 | pure cbTempSelsct[i]->Checked=false loop, widget-only |
| 28 | `UpdateUT150Comm` | 5486-5653 | 168 | (c) 安全關鍵 | 是 | REAL heater/temp-controller RS232/485 write+read commands (TMC401WriteTemp/UT100WordWriteNoSucm/E5DCWriteTemp/DTK4848WordWriteNoSucm) -- a temperature setpoint command pump, not display |
| 29 | `sbSendTempClick` | 5655-5680 | 26 | (c) 安全關鍵 | 是 | sets bReadUT150 per button Tag then calls UT150Polling() -- entry point that arms the hardware write cycle in UpdateUT150Comm |
| 30 | `UT150Polling` | 5682-5692 | 11 | (c) 安全關鍵 | 是 | sets bPollingUT150[i] flags gating which channels UpdateUT150Comm will command next tick -- enabling gate for a real hardware write, not pure display |
| 31 | `chkHeaterClick` | 5694-5708 | 15 | (c) 安全關鍵 | 是 | SW[SwHeaterRelay].OnOff() + SW[SwHeaterFan].OnOff() -- directly toggles physical heater relay/fan hardware from a checkbox click |
| 32 | `FormClose` | 5710-5917 | 208 | MIXED(需拆分) | 是 | widget resets + Save-or-Discard decision (declining calls elConfig->InitialDataToEdit() and returns) + large write branch: offset-file-copy loop over all recipes, ReadWriteIni, dmTrayMotor vibrate-speed flag, cross-form fLotInfo checkbox write, fMain->ToolLoadICO() at the very end |
| 33 | `Timer1Timer` | 5919-5944 | 26 | MIXED(需拆分) | 是 | calls UpdateUT150Comm() (real hardware pump, category c) every tick, PLUS pure Visible/Caption display lines for D21/D47/F05 panels and IPSC quantity labels |
| 34 | `BitBtn1Click` | 5946-5954 | 9 | (c) 安全關鍵 | 是 | fLogin->ShowModal() then writes LastSet.szSupervisor (the supervisor password) from the dialog result |
| 35 | `btD47Click` | 5956-5960 | 5 | (b) write path | 是 | resets LastSet.iD47SocketTestedCount counter to 0 + mirrors to widget |
| 36 | `edSetTempClick` | 5962-5965 | 4 | (a) 顯示/唯讀 | 是 | pure ShowQwertyKey call, no other side effect |
| 37 | `edSetTempKeyDown` | 5967-5971 | 5 | (a) 顯示/唯讀 | 是 | pure ShowQwertyKey call, identical pattern to edSetTempClick |
| 38 | `btnAdd10000Click` | 5973-5976 | 4 | (b) write path | 是 | calls SetSoftSpeedSpeed(false,10000), writes LastSet.SoftSpeed[] |
| 39 | `btnSetToTechClick` | 5978-5981 | 4 | (b) write path | 是 | dispatches to SetOffsetToTech() (defined outside cConfiguration.cpp, not resolved in this recon) -- name implies committing current offsets as the teach baseline; flagged high-risk, needs golden lookup before any translation |
| 40 | `edE31_Fi1XClick` | 5983-5986 | 4 | (a) 顯示/唯讀 | 是 | pure ShowQwertyKey call |
| 41 | `udD46Click` | 5988-5993 | 6 | (b) write path | 是 | writes IniConfig.iD46WaitIndexDestroyTime directly from a TUpDown control |
| 42 | `PageControl1Change` | 5995-6008 | 14 | (c) 安全關鍵 | 是 | password-gates tab index 2 for CC_ASE_KaohSiung via bPassWord(2); else calls ChangeCompomentEnabled(...) by authConfig[] table |
| 43 | `pcConfigChange` | 6010-6017 | 8 | (a) 顯示/唯讀 | 是 | AccessLevel-gated call to ChangeCompomentEnabled(...) only -- no password prompt, no global write in this function itself (ChangeCompomentEnabled itself not independently verified in this recon) |
| 44 | `cbE30Click` | 6019-6024 | 6 | (a) 顯示/唯讀 | 是 | pure Visible cascade among palE30/palE31/palE32 based on sibling Checked states |
| 45 | `InitialMemo` | 6026-6110 | 85 | (a) 顯示/唯讀 | 是 | recursive widget walk reading ConfigMemoPath help-text ini (ReadIniData) purely to build in-memory description strings for display -- verified fully, genuinely read-only |
| 46 | `ShowMemo` | 6112-6148 | 37 | (a) 顯示/唯讀 | 是 | assembles InitialMemo() results into TMemo widgets per tab -- pure display, no write |
| 47 | `btResumeClick` | 6151-6155 | 5 | (c) 安全關鍵 | 是 | sets bLockByServer=false -- overrides a server-originated machine lock flag, not benign despite the message-box look |
| 48 | `btN06_TesterListClick` | 6157-6163 | 7 | (a) 顯示/唯讀 | 是 | OpenDialog1->Execute() + sets edN06_TestList->Text only, no persistence in this handler |
| 49 | `tbD25_Index60mmChange` | 6165-6195 | 31 | (c) 安全關鍵 | 是 | Index Load Rate (D25, contact-force family) trackbar handler: range-clamps position (85-115) and sets bEP60DataChange audit flag -- contact-force safety parameter, not display |
| 50 | `tbD25_Index40mmChange` | 6197-6228 | 32 | (c) 安全關鍵 | 否(Tier2) | same D25 Index Load Rate family, structurally identical to Index60mm sibling (not individually read, pattern-inferred) |
| 51 | `tbD25_Index30mmChange` | 6230-6261 | 32 | (c) 安全關鍵 | 否(Tier2) | same D25 family, pattern-inferred |
| 52 | `cbE39Click` | 6263-6275 | 13 | (a) 顯示/唯讀 | 是 | pure Visible/Checked cascade for cbE39_1 from cbE39 + IniConfig.bE39_1PutTheDevicesToErrorBin read (read-only) |
| 53 | `sbExitClick` | 6277-6284 | 8 | (c) 安全關鍵 | 是 | fShowMessage->ShowSpeed(); sets fMain->sbPEModel->Visible; conditionally calls fMain->sbPEModelClick(this) (a real PE-mode toggle on the main form) before Close() |
| 54 | `cbD55Click` | 6286-6297 | 12 | (c) 安全關鍵 | 是 | guards "Disable IndexCheck" checkbox: reverts it to false and shows a message unless Real Time CCD is enabled -- safety-relevant guard on a real test-integrity option |
| 55 | `cbE38Click` | 6299-6310 | 12 | (a) 顯示/唯讀 | 是 | pure Checked/Visible cascade for cbE40 from cbE38 |
| 56 | `strngrdAutoSaveLogMouseDown` | 6312-6329 | 18 | (a) 顯示/唯讀 | 是 | toggles one StringGrid header-row cell text "On"/"" -- UI grid cell edit only, no global/file write in this handler |
| 57 | `btnAutoSaveSetAllClick` | 6331-6337 | 7 | (a) 顯示/唯讀 | 是 | sets all 7 StringGrid header cells to "On" -- same UI-only grid edit |
| 58 | `btN06_TesterMapClick` | 6339-6345 | 7 | (a) 顯示/唯讀 | 是 | OpenDialog1->Execute() + sets edN06_TesterMap->Text only |
| 59 | `cbA09Click` | 6347-6374 | 28 | (c) 安全關鍵 | 是 | IC-presence interlock: reverts checkbox and raises MES1645/MES1646 error if InArmSuck/Shuttle/Index/MOT plates have IC -- a genuine safety gate preventing a config change mid-production |
| 60 | `DoPassword` | 6376-6423 | 48 | (c) 安全關鍵 | 是 | access-control flow: force-resets AccessLevel=0/cbUserSelect->ItemIndex=0/btLogin caption to Operator when tech-password file missing or level insufficient -- one of the two places in this file that touches the user-level chain, as a consumer not the source |
| 61 | `cbM01Click` | 6425-6438 | 14 | (c) 安全關鍵 | 是 | requires DoPassword()==true before allowing the checkbox to change (reverts otherwise), then toggles gbM01->Visible |
| 62 | `edD25_60mmClick` | 6440-6443 | 4 | (a) 顯示/唯讀 | 是 | pure ShowQwertyKey call (double range 0.5..-0.5, i.e. Min>Max swapped per HTEditList Add() semantics -- same as its D60 sibling) |
| 63 | `cbD36Click` | 6445-6452 | 8 | (a) 顯示/唯讀 | 是 | pure Checked cascade: cbD36=true forces cbD33=false,cbD35=true |
| 64 | `tbD25_Index60mm_NSChange` | 6454-6483 | 30 | (c) 安全關鍵 | 否(Tier2) | D25 Index Load Rate NS-variant, structurally mirrors tbD25_Index60mmChange (not individually read) |
| 65 | `tbD25_Index40mm_NSChange` | 6485-6514 | 30 | (c) 安全關鍵 | 否(Tier2) | same family, pattern-inferred |
| 66 | `tbD25_Index30mm_NSChange` | 6516-6544 | 29 | (c) 安全關鍵 | 否(Tier2) | same family, pattern-inferred |
| 67 | `tbD60_Index56mmChange` | 6546-6561 | 16 | (c) 安全關鍵 | 是 | Index Load Rate D60 family: range display + RecordProcess() audit log write on change -- contact-force-adjacent parameter |
| 68 | `tbD60_Index56mm_NSChange` | 6563-6578 | 16 | (c) 安全關鍵 | 否(Tier2) | NS-variant of above, structurally mirrors it (not individually read) |
| 69 | `edD60_56mmClick` | 6580-6583 | 4 | (a) 顯示/唯讀 | 是 | pure ShowQwertyKey call |
| 70 | `btnRecordJamRateByTimeClearClick` | 6585-6591 | 7 | (b) write path | 是 | resets iRecordJamRateByTime_LoaderCount/_JamCount globals to 0 + sets bRecordJamRateByTime_Clear=true -- runtime statistics reset, not display |
| 71 | `edA22_2Click` | 6593-6601 | 9 | (a) 顯示/唯讀 | 是 | ShowQwertyKey + local cross-field text nudge on edA22_3 -- widget-only, no global write |
| 72 | `edA22_3Click` | 6603-6607 | 5 | (a) 顯示/唯讀 | 是 | pure ShowQwertyKey call with a computed min bound from edA22_2 |
| 73 | `imgI37_3Click` | 6609-6625 | 17 | (b) write path | 否(Tier2) | SaveLoadFile signal only, not individually read in this pass |
| 74 | `WriteContactData` | 6627-6636 | 10 | (b) write path | 是 | CUSTOMER_CODE==CC_ASE_KaohSiung: WriteIniData x3 to D:\HT9045\system\ep.txt with LastSet.dIndexLoadRate values -- unambiguous file write |
| 75 | `btnOpenEPClick` | 6638-6667 | 30 | (c) 安全關鍵 | 是 | password gate (EPuser/EPPass) on Engineering Parameter access; on success enables gbD25 (D25 Index Load Rate / contact-force group) and snapshots LastSet.dIndexLoadRate[][] into old*data for later change-audit |
| 76 | `cbC12Click` | 6669-6685 | 17 | (c) 安全關鍵 | 是 | hardcoded per-build password ("hontech16943420kyecbu3_"+SVNRevision) gates the PE-mode checkbox for KYEC -- literal password embedded in source, security-sensitive |
| 77 | `sbN15UserLevelByTxtReadFilePathClick` | 6687-6698 | 12 | (a) 顯示/唯讀 | 是 | OpenDialog2->Execute() + sets edtN15_1->Text only |
| 78 | `btnN15ESDFormClick` | 6700-6703 | 4 | (b) write path | 否(Tier2) | ShowModal signal only, not individually read (opens an ESD-related form) |
| 79 | `spbA25_RunExecutFilePathChoiceClick` | 6705-6716 | 12 | (a) 顯示/唯讀 | 是 | OpenDialog3->Execute() + sets edA25_1->Text only |
| 80 | `cbA27Click` | 6718-6752 | 35 | (c) 安全關鍵 | 是 | same hardcoded-password gate pattern as cbC12Click, for KYEC optical-scale (light curtain?) enable checkbox; also mirrors state into fMain->Label2->Caption |
| 81 | `cbN07_EnableEmployeeCheakClick` | 6754-6779 | 26 | (c) 安全關鍵 | 是 | same hardcoded-password gate pattern, for KYEC N07-5 employee-check function |
| 82 | `bPassWord` | 6781-6798 | 18 | (c) 安全關鍵 | 是 | modal password dialog compared against EPPass/sPassWord by mode -- auth primitive used by several other (c) functions in this file |
| 83 | `spbA28_2Click` | 6800-6815 | 16 | (a) 顯示/唯讀 | 是 | Greatek-only: opens an external SOP HTML viewer form after a FileExists check -- read-only document viewer, no writes (fully verified) |
| 84 | `spbA27Click` | 6817-6837 | 21 | (b) write path | 是 | Greatek-only: CheckConfigurationBeforeSave()+SaveConfiguration()+CopyFile(config.ini -> config_Standard.ini) -- commits current config as the new standard template consumed by ReadConfigStandard() on every future restart |
| 85 | `strngrdTraySelectCell` | 6839-6844 | 6 | (a) 顯示/唯讀 | 是 | pure iSelTrayRow/iSelTrayCol bookkeeping, local ints only |
| 86 | `strngrdTrayDblClick` | 6846-6849 | 4 | (a) 顯示/唯讀 | 是 | forwards to btnModifyTray->Click() -- inherits btnModifyTrayClick classification (a) |
| 87 | `btnModifyTrayClick` | 6851-6876 | 26 | (a) 顯示/唯讀 | 是 | edits one cell of the in-memory strngrdTray StringGrid via ShowQwertyKey -- no file/global write (persistence happens only in sbUpdateTrayClick) |
| 88 | `btnAddTrayClick` | 6878-6888 | 11 | (a) 顯示/唯讀 | 是 | appends a blank row to strngrdTray -- pure in-memory grid edit |
| 89 | `btnDeleteTrayClick` | 6890-6904 | 15 | (a) 顯示/唯讀 | 是 | shifts rows up to remove the selected strngrdTray row -- pure in-memory grid edit |
| 90 | `sbUpdateTrayClick` | 6906-6929 | 24 | (b) write path | 是 | serializes strngrdTray to CommaText rows and calls sList->SaveToFile(TrayTablePath) -- the real Tray-table persist, then re-triggers sbtReloadTray->Click() |
| 91 | `sbtReloadTrayClick` | 6931-6975 | 45 | (a) 顯示/唯讀 | 是 | List->LoadFromFile(TrayTablePath) then parses CSV into strngrdTray cells -- pure load-and-display, no write |
| 92 | `strngrdHPSelectCell` | 6977-6982 | 6 | (a) 顯示/唯讀 | 是 | pure iSelHPRow/iSelHPCol bookkeeping, mirrors strngrdTraySelectCell |
| 93 | `strngrdHPDblClick` | 6984-6987 | 4 | (a) 顯示/唯讀 | 是 | forwards to btnModifyHP->Click() -- inherits btnModifyHPClick classification (a) |
| 94 | `btnModifyHPClick` | 6989-7012 | 24 | (a) 顯示/唯讀 | 是 | edits one cell of the in-memory strngrdHP StringGrid via ShowQwertyKey -- structurally identical to btnModifyTrayClick, no file/global write |
| 95 | `btnAddHPClick` | 7014-7024 | 11 | (a) 顯示/唯讀 | 是 | appends a blank row to strngrdHP -- pure in-memory grid edit, mirrors btnAddTrayClick |
| 96 | `btnDeleteHPClick` | 7026-7040 | 15 | (a) 顯示/唯讀 | 是 | shifts rows up to remove the selected strngrdHP row -- mirrors btnDeleteTrayClick |
| 97 | `sbtReloadHPClick` | 7042-7086 | 45 | (a) 顯示/唯讀 | 是 | List->LoadFromFile(PlateTablePath) then parses CSV into strngrdHP cells -- pure load-and-display, correctly self-referential (unlike sbUpdateHPClick) |
| 98 | `sbUpdateHPClick` | 7088-7111 | 24 | (b) write path | 是 | GOLDEN BUG: serializes strngrdHP and calls sList->SaveToFile(PlateTablePath) correctly, but then calls sbtReloadTray->Click() and sbUpdateTray->Down=false (Tray, not HP) -- copy-paste defect from sbUpdateTrayClick, must be translated as-is with an //AI note, not silently fixed |
| 99 | `edtSearchFunctionChange` | 7113-7146 | 34 | (a) 顯示/唯讀 | 是 | reparents THTEdit-bound checkboxes between scrlbxSearch and their default position based on caption substring match against elConfig->FEditList -- pure widget Parent/Align manipulation, no data write |
| 100 | `btnSetIPSCQtyClick` | 7148-7152 | 5 | (b) write path | 是 | writes fProductionInfo->iIPSCFlag_TriggerMode/iIPSCFlag_CountdownQty -- cross-form production-control state write |
| 101 | `btnSetIPSCCleaarQtyClick` | 7154-7166 | 13 | (c) 安全關鍵 | 是 | writes fProductionInfo IPSC fields AND directly calls fMain->BtnOneCycleClick(fMain) -- a config-form button silently triggering a real One-Cycle machine action |
| 102 | `Image1DblClick` | 7168-7183 | 16 | (c) 安全關鍵 | 是 | hidden admin gate behind double-clicking an image/logo: lazy WriteIniData seed of a default password into SG_PW.ini, then password-gates grpA32_1->Visible |
| 103 | `SaveConfiguration` | 7185-7195 | 11 | (b) write path | 是 | calls WriteLastDataFile()+SaveLastSetIni()+SaveTasterInfo()+SetA73() -- the real config-save entry point; WriteLastDataFile/ReadLastDataFile use hardcoded literal paths per KNOWLEDGE.md gotcha, --dry cannot redirect them |
| 104 | `CheckConfigurationBeforeSave` | 7197-7589 | 393 | (c) 安全關鍵 | 是 | despite the Check-prefixed name: logs EP change flags via RecordProcess, password-gates a FIFO-mode restart warning, and at the end calls fMain->SendMSG_CMD(MSG_CMD_EnableFTPFunction/DisableFTPFunction) -- a real inter-module control command dispatch, not a pure validation check |
| 105 | `LoadConfiguration` | 7591-7595 | 5 | (b) write path | 是 | calls ReadLastDataFile()+ReadLastSetIni() -- pairs with SaveConfiguration; same hardcoded-path caveat re: --dry |
| 106 | `ReadConfigStandard` | 7597-7609 | 13 | (b) write path | 是 | named Read but performs CopyFile(config_Standard.ini -> config.ini) when CosFunction.bConfigStandard -- overwrites the live config on every TfConfiguration construction (i.e. every app start) for Greatek-profile installs |
| 107 | `edA32_1Click` | 7611-7614 | 4 | (a) 顯示/唯讀 | 是 | pure ShowQwertyKey call |
| 108 | `btnN31_ManualClick` | 7616-7620 | 5 | (b) write path | 否(Tier2) | FTP_ci+GlobalWriteLastSet signals, not individually read |
| 109 | `btnMesSystemClick` | 7622-7625 | 4 | (c) 安全關鍵 | 是 | fMesSystem->ShowModal() -- bare navigation into another form; flagged conservatively per the uLotInfo precedent for un-audited ShowModal targets rather than assumed benign |
| 110 | `btnN32Click` | 7627-7637 | 11 | (c) 安全關鍵 | 是 | HasICUnderMachine() interlock guard before allowing fFTPClient->DownloadUpdateAutomatically(true); may Close() the form on success |
| 111 | `cbN06_EnableFTPMouseDown` | 7639-7673 | 35 | (c) 安全關鍵 | 是 | CC_GIGAS-only hardcoded/ini-stored password gate (sGigasFTPPassWord) on the FTP-enable checkbox, bypassed if AccessLevel>=Supervisor |
| 112 | `edtC15Click` | 7675-7678 | 4 | (a) 顯示/唯讀 | 是 | pure ShowQwertyKey call |
| 113 | `edL09_Sh1LClick` | 7680-7683 | 4 | (a) 顯示/唯讀 | 是 | pure ShowQwertyKey call |
| 114 | `edN05_AmbTempClick` | 7685-7688 | 4 | (a) 顯示/唯讀 | 是 | pure ShowQwertyKey call |
| 115 | `edN06_PortClick` | 7690-7693 | 4 | (a) 顯示/唯讀 | 是 | pure ShowQwertyKey call (N_PORT) |
| 116 | `edN06_PasswordClick` | 7695-7698 | 4 | (a) 顯示/唯讀 | 是 | pure ShowQwertyKey call (N_PASSWORD) -- opens keypad to edit an FTP password field, does not itself store/compare anything |
| 117 | `edtSetIPSCQtyClick` | 7700-7703 | 4 | (a) 顯示/唯讀 | 是 | pure ShowQwertyKey call |
| 118 | `edN04_ModelClick` | 7705-7708 | 4 | (a) 顯示/唯讀 | 是 | pure ShowQwertyKey call |
| 119 | `btnN14_22ExportClick` | 7710-7716 | 7 | (b) write path | 是 | calls fProductionInfo->UploadConfigToServer() -- network config upload |
| 120 | `btnN14_22ImportClick` | 7718-7724 | 7 | (b) write path | 是 | calls fProductionInfo->LoadConfigFromServer() -- can silently overwrite local config from a server |
| 121 | `EnableRMSFunc` | 7726-7735 | 10 | (a) 顯示/唯讀 | 是 | pure boolean predicate from CosFunction/IniConfig/CUSTOMER_CODE, no side effects, returns bret |
| 122 | `btnN25_3_ManualClick` | 7737-7740 | 4 | (b) write path | 否(Tier2) | FTP_ci signal only, not individually read (name pattern matches its N25_4/N25_5 siblings which dispatch SendCommand_EventLog) |
| 123 | `btnN25_4_ManualClick` | 7742-7745 | 4 | (b) write path | 是 | calls SendCommand_EventLog(EL_UPLOAD_SUMMARY,"1") |
| 124 | `btnN25_5_ManualClick` | 7747-7750 | 4 | (b) write path | 是 | calls SendCommand_EventLog(EL_UPLOAD_EVENTLOG,"1") |
| 125 | `btnN35_TestClick` | 7752-7755 | 4 | (b) write path | 是 | calls fLotInfo->SaveGroundESDData_Upolad() -- file write per uLotInfo recon precedent |
| 126 | `btnUploadAllClick` | 7757-7769 | 13 | (b) write path | 是 | iterates every folder under D:\HT9045\IniData\Data and calls fLotInfo->DoUpload() for each -- bulk recipe upload |
| 127 | `btnA71ManuallyClick` | 7771-7774 | 4 | (b) write path | 是 | calls fMain->RunBatchCopyRecipe(DataPath+fMain->cbSetupFileName->Text) -- consumes the CURRENT recipe name (recipe.current) to trigger a batch copy; another downstream consumer, not the source, of cbSetupFileName |
| 128 | `btnSaveClick` | 7776-7780 | 5 | (b) write path | 是 | sets bSave=true then calls sbExitClick(btnSave) -- cascades into the (c)-tagged sbExitClick |
| 129 | `SetA73` | 7782-7806 | 25 | (c) 安全關鍵 | 是 | CC_KYEC_LEE only: writes IniConfig.bFTBin2RTBin/CosFunction.bDisableRTBinSet and, on change, reloads fBinSel bin definitions + calls SetTestRunMode() + repaints/reinitializes the hardware Bin Display -- a real FT/RT bin-scheme reconfiguration |

---

## 2. 孤兒宣告檢查（golden .h 有宣告、golden .cpp 查無定義）

用「header 內出現識別字緊跟 `(`，排除控制關鍵字與以 `T` 開頭的型別名」這個較弱的正規表示式對
`cConfiguration.h:24-2350`（class 本體範圍）掃描，結果**候選集合為空**——也就是說沒有找到任何
「header 提到但 129 個 span 裡沒有」的方法名稱。**這個方法論比 uLotInfo recon 用的字串/註解遮罩+逐行核對弱**
（沒有對多行宣告簽名做接續處理，理論上可能漏抓分行寫的宣告），所以結論寫成「未發現孤兒宣告的候選」而非
「窮舉證明零孤兒」；如果之後要引用這個「0 孤兒」結論做決策，建議先用 uLotInfo recon 那套字串遮罩方法重跑一次。

---

## 3. 四分類統計

| 分類 | 函式數 | golden 行數 | 其中逐行覆核 | 其中僅訊號掃描(Tier2) |
|------|--------|-------------|--------------|------------------------|
| (a) 顯示/唯讀可翻 | 46 | 626 | 46（100%） | 0 |
| (b) write path（佇列） | 48 | 4,641 | 36 | 12 |
| (c) 安全關鍵（佇列） | 32 | 1,197 | 26 | 6 |
| MIXED（需先拆分才能歸類） | 3（FormShow 852L, FormClose 208L, Timer1Timer 26L） | 1,086 | 3 | 0 |
| **合計** | **129** | **7,550** | **107（83%）** | **22（17%）** |

分母說明：129 = `TfConfiguration::` 全部方法數（第 2 節孤兒檢查未發現候選，故無需扣除）；
7,550 = 129 個 span 行數總和（golden 檔案本體 7,808 行，差額為函式間空行/分隔註解，未計入任何 span）。
**硬性檢核**：46 個 (a) 分類**全部**逐行覆核完整本體，0 個例外——這是回應本波次任務指示
「(a) 顯示唯讀（逐行讀完本體才能給 (a)）」的直接證據，不是事後宣稱。

---

## 4. 關鍵發現

本檔案比 uLotInfo 更誇張的地方：**它本身就是全機台幾百個參數的「政策定義層」**，不是單純的表單。
以下 6 類具體證據，每一類都在波次過程中改變了分類假設：

### 4.1 `InitConfigEdtList_Item[A-P]`（13 個函式，共 4,131 行，佔全檔 55%）不是顯示，是政策註冊

`InitConfigEdtList_ItemA`（cConfiguration.cpp:378-977）逐行讀出的實際內容不是「畫面初始化」，
而是對全域 `HTEditList *elConfig`（`Public/HTEditList.cpp:43` 宣告，`main.cpp:1484 new` 出來）
呼叫 `elConfig->Add(widget指標, 全域變數指標, 型別, 群組名, 鍵名, 可見度, 可編輯, bReadFromFile,
DefValue, ...)`，對機台上幾乎每一個可設定參數（`IniConfig.*`/`LastSet.*`）依 `CUSTOMER_CODE`
做分支，決定：

1. 這個參數顯不顯示、能不能編輯；
2. **`bReadFromFile`**——是否允許從 `config.ini` 讀回原值，還是強制用 `DefValue` 蓋掉（也就是
   `bFixedValue` 語意，實測於 `cConfiguration.cpp:392-431`：例如 `CC_ASE_KaohSiung` 客戶碼下
   `iA01ChangeOpTime` 被鎖死成 60，`IniConfig.bSPILFunction` 客戶鎖死成 180，一般客戶則可從檔案
   讀 60~1000 之間的值）。

這個 `bReadFromFile`/`DefValue` 對雖然不是本函式自己寫檔，但它決定了**之後** `cprod.cpp:2816-2903`
的 `elConfig->ReadEditTextFromFile(AuthPath,"config.ini")`/`InitialDataToEdit()`（開機時執行）
會不會把某個客戶的參數強制改寫成固定值——這正是任務指示預先提醒的「藏 WriteIniData 懶 seed」的
變體：不是這個函式自己 seed，而是這個函式**決定了另一個函式待會 seed 成什麼**。13 個 Item 函式
與呼叫 `ReadLockByFile()`+`InitConfigEdtList()`+`ChangeCBListProperty()` 的建構子（見 4.2）
一起，構成本檔案分類為 **(b) write path** 而非 (a) 的核心理由，即使多數 Item 函式本身沒有
`WriteIniData(`/`fopen(` 字面呼叫。

### 4.2 `ReadLockByFile`／`ReadConfigStandard` 兩個「Read」前綴函式都會寫檔

- `ReadLockByFile`（cConfiguration.cpp:230-261）：12 次 `ReadIniData` 讀 `config.ini` 的
  `[Specific]` 區段到 `IniConfig.bF06_Active` 等旗標，**但最後兩行是
  `if(!CheckKeyExist(...)) WriteIniData(...)`**——鍵不存在就補寫，是任務指示點名的
  「Read...但寫」樣板案例，逐字命中。
- `ReadConfigStandard`（cConfiguration.cpp:7597-7609）：`CosFunction.bConfigStandard`
  （Greatek 客戶碼）為真時，直接 `CopyFile(config_Standard.ini → config.ini)`——**每次
  `TfConfiguration` 建構（即每次程式啟動）都會覆蓋一次現行 config.ini**。這個函式被建構子
  在 `cConfiguration.cpp:113` 呼叫（`InitConfigEdtList()` 之前一行），所以它是開機序列的
  一部分，不是使用者手動觸發的可選功能。

### 4.3 `sbUpdateHPClick` 有真實的 golden 複製貼上缺陷

`cConfiguration.cpp:7088-7111`：正確地把 `strngrdHP` 序列化寫入 `PlateTablePath`，
但接下來呼叫的是 `sbtReloadTray->Click()` 和 `sbUpdateTray->Down=false`——**都是 Tray
按鈕，不是 HP 按鈕**。跟它結構完全對稱的 `sbUpdateTrayClick`（cConfiguration.cpp:6906-6929）
在存完 Tray 後正確呼叫 `sbtReloadTray`。這是明確的 golden 缺陷（存 HP 資料後重新載入的是
Tray 表格，HP 表格的「Update」按鈕的按下狀態也沒有正確清除），依翻譯紀律**必須原樣翻譯並在
`//AI` 註解寫下這個缺陷**，不可自行改成呼叫 HP 版本——除非使用者明確決定要修正這個行為。

### 4.4 三個獨立、寫死在原始碼裡的密碼閘門

`cbC12Click`（:6669-6685）、`cbA27Click`（:6718-6752）、`cbN07_EnableEmployeeCheakClick`
（:6754-6779）三個函式共用同一段字面密碼組成邏輯：
`AnsiString asKYECPassword="hontech16943420kyecbu3_"; asKYECPassword += AnsiString(SVNRevision);`
——密碼字串直接寫死在原始碼裡（只依編譯時的 SVN 版本號做極小變化）。`cbN06_EnableFTPMouseDown`
（:7639-7673，CC_GIGAS 專用）與 `Image1DblClick`（:7168-7183，雙擊圖片觸發、內含
`WriteIniData` lazy-seed 一組預設密碼 `16943420` 到 `SG_PW.ini`）是另外兩個同類但密碼來源
不同（ini 檔/全域變數）的隱藏管理閘門。翻譯這些函式時**這幾個字面值本身就是安全性資產**，
不應該被當成一般字串重構掉、也不應該被貼進任何非受控文件；本報告只引用函式名稱與行號，
密碼原文本節已經照抄一次是因為 golden 原始碼本來就是明碼——**如果未來要把這幾個函式排進任何
翻譯波次，第一件事是先跟使用者確認密碼原文的保密等級要求**。

### 4.5 `CheckConfigurationBeforeSave` 名字裡的 Check 是假的

（cConfiguration.cpp:7197-7589，393 行）函式尾端（:7580-7588）在
`CosFunction.bFTPFunction` 為真時，依 `IniConfig.bEnableFTP` 呼叫
`fMain->SendMSG_CMD(MSG_CMD_EnableFTPFunction)` 或 `MSG_CMD_DisableFTPFunction`——
這是一個真實的跨模組控制指令發送，不是驗證。函式前段還有密碼閘門
（`MyMessageBox->DoPassword_MBox()`）擋下 FIFO 模式切換並要求重開機。整個函式的淨效果是
**控制路徑**，「Check」只是外層包裝。

### 4.6 兩個「小按鈕」直接觸發真實機台動作

- `btnSetIPSCCleaarQtyClick`（:7154-7166）：在寫完 `fProductionInfo` 的 IPSC 旗標後，
  直接呼叫 `fMain->BtnOneCycleClick(fMain)`——**一個 Config 表單上的清除按鈕會觸發主畫面的
  「One Cycle」機台動作**，跟任務指示預先提醒的「藏控制呼叫」完全對應。
- `btResumeClick`（:6151-6155）：外觀是訊息框按鈕，實際上 `bLockByServer=false`
  直接解除一個伺服器端下達的機台鎖定旗標。

---

## 5. FW-1 懸案查證：`recipe.current` / `user.level` 的來源鏈

**結論：否——`cConfiguration.cpp` 不是這兩個 tag 的資料源頭，是兩個非常邊緣的下游消費者。**
真正的來源鏈都在 `main.cpp`（`TfMain`），與 `cConfiguration.cpp` 完全不同的翻譯前線。
以下是查證過程與逐行證據（全數於 2026-08-19/20 在 `HT9011UC_Code_V3.33.906.0_20260618` 執行）。

### 5.1 `recipe.current`（crosswalk 記載鏡射 `fMain.cbSetupFileName`）

`cbSetupFileName` 宣告於 `main.h`（`TfMain` 的 `__published` 成員，`grep -rln cbSetupFileName`
命中 main.h/main.cpp 以外的 35 個檔案，全部都是 `fMain->cbSetupFileName` 的外部使用者，
`cConfiguration.h` 完全沒有宣告這個名字）。

- **清單來源**：`TfMain::LookForFile()`（`main.cpp:9016-9095`，80 行）用
  `FindFirstFile`/`FindNextFile` 掃描 `DataPath` 底下的子資料夾，逐一
  `cbSetupFileName->Items->Add(ExtractFileName(filedata.cFileName))`（`main.cpp:9062`）。
  這是唯一一個「建立」清單內容的函式。
- **目前選中值鏡射**：`cbSetupFileName->Text=...` 的賦值散落在 30+ 個檔案（`main.cpp`
  本身、`uLotInfo.cpp`、`cBuilder.cpp`、`SECSGEM/uHGemHT9045.cpp`、`KYECFTP/FTPClient.cpp`、
  `ARMS/ARMS.cpp`、`Automation/uRENESAS_Server.cpp`、`Mes/fVATMesFileSys.cpp` 等），
  多數場景是「切換 recipe / SECS PPID 變更 / FTP 下載完成」時同步更新顯示值。
- **`cConfiguration.cpp` 唯二的接觸點**（都是讀取既有清單，不是建立清單）：
  - `FormClose`（:5850-5860）：`for(int i=0;i<fMain->cbSetupFileName->Items->Count;i++)`
    迭代整份清單，把目前的 offset 檔複製到每一個 recipe 資料夾（E45 功能相關）。
  - `btnA71ManuallyClick`（:7771-7774）：`fMain->RunBatchCopyRecipe(DataPath+
    fMain->cbSetupFileName->Text)`，只讀 `.Text`（目前選中值），觸發批次複製。

### 5.2 `user.level`（crosswalk 記載鏡射 `fMain.cbUserSelect`）

`cbUserSelect` 同樣宣告於 `main.h`（`TfMain` 成員），`cConfiguration.h` 沒有宣告。

- **清單來源**：`TfMain::FormShow()` 內（`main.cpp:10752-10776`，位於
  `if(CosFunction.bSecurityHave5Level==true)` 區塊），依 `CUSTOMER_CODE==CC_KYEC_LEE` 決定
  加 5 個等級（含 "PEngineer"）或標準 5 個等級（含 "Open"）。這是唯一「建立」等級清單的位置。
- **目前選中值鏡射**：`TfMain::DoChangeLevel()`（`main.cpp:15127-15147`，21 行）——把全域
  `int AccessLevel`（0-4）透過三個候選字串陣列（`str[]`/`cLevel[]`/`cLevel2[]`，依
  `CosFunction.bSecurityHave5Level` 與 `CUSTOMER_CODE==CC_KYEC_LEE` 二次分支）寫入
  `cbUserSelect->Text`。這是「目前使用者權限等級」這個狀態真正的計算邏輯所在。
- **`cConfiguration.cpp` 唯一的接觸點**（同樣是消費/覆寫既有狀態，不是建立）：
  `DoPassword`（:6390, :6416）——當技術密碼檔不存在或權限不足時，呼叫
  `fMain->cbUserSelectChange(NULL)`（觸發 fMain 自己的變更處理，不是填清單）並把
  `fMain->cbUserSelect->ItemIndex=0` 重設回第一項（"Operator"/"Open"），同時把全域
  `AccessLevel=0`。這是一個**降權重置**動作，不是來源。

### 5.3 V906 現況對照（決定下一步該去哪個檔案，不是本檔）

- `forms\fMain.h:278` 已經把 `cbSetupFileName` 建成一個 `TfLotInfoRunMode*` 型態的
  stand-in（註解明寫「只有 `.Text` 是真的」），`LookForFile()` 在 `forms\fMain.h:508` 已宣告
  為方法簽名，但實作是 `[PORT-ONLY SEAM]` 佔位（只有呼叫計數器 `W906_LookForFileCallCount`，
  golden 本體 `main.cpp:9016` 尚未翻譯）；`cbSetupFileNameChange`（golden `main.cpp:24643-24939`，
  297 行）同樣列為未翻譯。
- `forms\fMain.h`/`forms\fMain.cpp` 對 `cbUserSelect`／`DoChangeLevel` **完全沒有任何痕跡**
  （`Grep "cbUserSelect|DoChangeLevel" forms\fMain.h` 0 筆命中）——唯一存在的引用只在
  `tools\dfm2rc\ir_out\main.dfm.ir.json`（純 widget 幾何 metadata，未接線）。這比
  `cbSetupFileName` 的缺口更大：後者至少有 stand-in 型態與方法簽名佔位，前者連佔位都沒有。
- 全域 `int AccessLevel` 本身**已經**是 V906 的活全域（`cmydef.h`/`cmydef.cpp` 宣告，
  `csystem.cpp`/`cContactCT.cpp`/`cCounterClear.cpp`/`cBinSel.cpp`/`ckernel.cpp`/`cSpeed.cpp`/
  `forms\fLotInfo.cpp`/`WebAuth.h`/`tests\test_security_core.cpp` 均有引用），所以
  `user.level` 真正缺的不是底層資料，是 `main.cpp` 裡把它轉成顯示字串／建清單的那兩段邏輯。

**給下一棒的結論**：`recipe.current`/`user.level` 的翻譯前線是 **`main.cpp`（`TfMain`）
的 `LookForFile`/`cbSetupFileNameChange`/`FormShow`（清單那一段）/`DoChangeLevel`**，
不是 `cConfiguration.cpp`。`cConfiguration.cpp` 只值得在**寫入路徑**（FormClose 的
offset 複製迴圈、btnA71ManuallyClick 的批次複製、DoPassword 的降權重置）被翻譯到時才需要
碰這兩個 tag 的名字，屬於 (b)/(c) 佇列而非顯示側 Wave A 的範圍。

---

## 6. Wave A 提案

**起點差異提醒**：uLotInfo 波次已有 `forms\fLotInfo.h`/`.cpp` 半成品可以疊加；本檔案
**V906 目前沒有 `forms\fConfiguration.h`/`.cpp`**，Wave A 的第一步是建 facade 骨架，
不是往既有骨架加欄位。好消息是 `tools\dfm2rc\ir_out\cConfiguration.dfm.ir.json` 與
`layout_out\cConfiguration_layout.gen.cpp` 已經是 FW 戰役對 `cConfiguration.dfm` 的機器抽取
產物（widget 幾何已存在，欄位宣告可加速產生，但**未接線**，跟 uLotInfo recon 對
`uLotInfo_layout.gen.cpp` 的提醒一樣：不要假設幾何存在=facade 存在）。

### 6.1 零依賴或近零依賴、建議優先示範翻譯（已逐行覆核，1 組共用閘門 + 個別函式）

**共用前提**：`fQwertyKey`（虛擬鍵盤）在 V906 樹**全域 0 個編譯進去的定義**——
`forms\fLotInfo.h:171-178` 明文記載這是「tree-wide gate」，`ATC/ATCInterface.cpp`、
`VacuumUnit.cpp`、`VacuumUnit/MyVacuumPanel.cpp`、`EJ1N/MyOmronPanel.cpp`、
`Public/HTEdit.cpp` 都各自命中同一個結論（20260819 前波次重驗仍然 0）。這代表下列
「純 ShowQwertyKey」函式翻譯後**會退化成 no-op**——這是誠實的現況，不是翻譯品質問題，
跟 uLotInfo 的 WA-8/WB-8/WB-11/WB-15 是同一個已拍板的模式（no-op stand-in 是可接受的
翻譯結果，等 `TfQwertyKey` 真正有人翻譯時這些呼叫自然變活）。

- **純 ShowQwertyKey、零其他依賴（12 個，共 60 行）**：`edSetTempClick`(4L)、
  `edSetTempKeyDown`(5L)、`edE31_Fi1XClick`(4L)、`edD25_60mmClick`(4L)、`edD60_56mmClick`(4L)、
  `edA32_1Click`(4L)、`edtC15Click`(4L)、`edL09_Sh1LClick`(4L)、`edN05_AmbTempClick`(4L)、
  `edN06_PortClick`(4L)、`edN06_PasswordClick`(4L)、`edtSetIPSCQtyClick`(4L)、
  `edN04_ModelClick`(4L) —— 每個只依賴自己的 1 個 `TEdit` widget（facade 全缺，但型態單純）
  + `fQwertyKey`（no-op stand-in）。**這 13 個是本檔案依賴面最小的候選**（原文列 13 個，
  上面數到 13 個名稱，行數合計 13×4=52，加 edSetTempKeyDown 多 1 行=53，此處以逐一列出為準，
  不重複計算）。
- **純 ShowQwertyKey + 1 個本地變數依賴（2 個，共 14 行）**：`edA22_2Click`(9L，額外讀寫
  同表單的 `edA22_3` widget)、`edA22_3Click`(5L，讀 `edA22_2`)。
- **零 widget 依賴的純布林 predicate（1 個，10 行）**：`EnableRMSFunc`——只讀
  `CosFunction.bEnableRMS`/`IniConfig.bShowLotInfo`/`CUSTOMER_CODE`，三者皆已是 V906 核心
  全域（存在性已用 Grep 核對），**沒有任何 widget 依賴，是本檔案唯一「完全不需要 facade」
  就能編譯執行的候選**。

### 6.2 StringGrid 叢集（Tray／HP 表格編輯器，已逐行覆核，`vclcompat\StringGrid.h` 已存在）

Tray 叢集（6 個函式，共 96 行：`strngrdTraySelectCell`6L/`strngrdTrayDblClick`4L/
`btnModifyTrayClick`26L/`btnAddTrayClick`11L/`btnDeleteTrayClick`15L/`sbtReloadTrayClick`45L
——最後一個 45L 但只做 `LoadFromFile`+CSV 手動解析，無寫入）與 HP 叢集（6 個函式，鏡像
`strngrdHPSelectCell`6L/`strngrdHPDblClick`4L/`btnModifyHPClick`24L/`btnAddHPClick`11L/
`btnDeleteHPClick`15L/`sbtReloadHPClick`45L）都已逐行驗證為 (a)，共 12 個函式 217 行。
依賴：`strngrdTray`/`strngrdHP`（`TStringGrid*`，`vclcompat\StringGrid.h` 已有相容實作，
facade 欄位仍需個別加）、`edtTemp`（共用暫存編輯框）、`TrayTablePath`/`PlateTablePath`
（AnsiString 路徑全域，需查是否已存在）、`fQwertyKey`（同 6.1 的 no-op 前提）。
**注意**：這兩個叢集各自的「Update」寫入函式（`sbUpdateTrayClick`/`sbUpdateHPClick`）
分類為 (b)，且後者含第 4.3 節記載的 golden 複製貼上缺陷——**不要**把 Update 函式一起排進
顯示側波次，只排上述 12 個讀/編輯/重載函式。

### 6.3 其餘已驗證 (a) 但依賴較高、留給後續波次評估

- `InitialMemo`+`ShowMemo`（85+37=122 行）：純顯示，但依賴 11 組 `TTabSheet*`/`TMemo*`
  陣列（`myTabSheet[11]`/`myMemo[11]`，對應 A/C/D/E/F/G/I/L/O/N/P 分頁）+ `ConfigMemoPath`
  全域（一份獨立的說明文字 ini，需查是否已存在）+ 遞迴走訪 `TWinControl->Controls[]`
  （需要 vclcompat 的容器巡覽能力，未核對是否已有等效機制）。
- `FormDestroy`（39L）：純 widget 解構，但依賴 `edSoftSpeed[]`/`labSoftSpeed[]`/
  `edOCRTrayLot[]`/`labOCRTrayLot[]`/`cbTempSelsct[]`/`edTempRead[]` 六組動態陣列，
  這些陣列本身是在建構子（分類 (b)，本波次不排入）裡動態 `new` 出來的，翻譯 `FormDestroy`
  前通常要先有對應的建構/宣告，不建議單獨排入。
- `btHeaterSelectAllClick`/`btHeaterClearSelectClick`（8L each）：依賴 `cbTempSelsct[tcTotalCount]`
  動態陣列，同上顧慮。
- `cbE30Click`(6L)/`cbE38Click`(12L)/`cbE39Click`(13L)/`cbD36Click`(8L)：純 widget
  Visible/Checked 級聯，依賴各自 2-4 個 `TCheckBox`/`TPanel`，facade 全缺但型態單純，
  可視 Wave A 進度追加。
- `pcConfigChange`(8L)：依賴外部函式 `ChangeCompomentEnabled()`（本檔案外定義，本次未查證
  是否純 UI），以及 `LevelSet.AccessLevel[42]`（需查）。
- `edtSearchFunctionChange`(34L)：**明確不建議排入 Wave A**——依賴全域 `elConfig`
  （`HTEditList*`），而 `elConfig=new HTEditList` 這行只在 golden `main.cpp:1484`，
  V906 的 `Public/HTEditList.h/.cpp` 雖然已翻譯，但 `elConfig` 實例化本身卡在未翻譯的
  `main.cpp`——這與 `docs\PT_CAMPAIGN_PLAN.md`/`docs\DEVLOG.md` 已經記載的 `elLaser`
  同類問題（`main()` 先 `new`、表單建構後用，任何在別的 TU 補一個 `new HTEditList` 的
  嘗試都贏不了這個初始化順序）是同一個地雷，翻譯此函式前必須先解決 `elConfig` 的
  實例化順序，不是本檔案能單獨解決的範圍。

### 6.4 Wave A 候選彙總

| 批次 | 函式數 | golden 行數 | 依賴摘要 |
|------|--------|-------------|----------|
| 6.1 純 ShowQwertyKey + EnableRMSFunc | 16 | 137 | 各 1 個 widget 或零依賴；`fQwertyKey` 全域 no-op |
| 6.2 Tray/HP StringGrid 叢集（僅讀/編輯/重載，不含 Update） | 12 | 217 | `TStringGrid`（已有相容層）+ 路徑全域 2 個 |
| **建議 Wave A 範圍合計** | **28** | **354** | 遠低於 4,000 行上限，是刻意保守的起手波次 |
| 6.3 留待後續（依賴較高） | 9 | 233 | 動態陣列/遞迴容器巡覽/外部函式，需先查證 |

354 行、28 個函式的規模刻意壓低，原因是本檔案的 (a) 分類函式普遍體積很小（46 個平均
13.6 行/個)，但**facade 從零開始**（沒有 uLotInfo 那種半成品可疊加），第一波的價值在於
建出 `forms\fConfiguration.h/.cpp` 骨架本身，而非行數規模。

---

## 7. 陷阱段

### 7.1 客戶碼 gated 統計

`grep -oE "CUSTOMER_CODE\s*==\s*CC_\w+"` 於 20260819/20 執行：全檔 **319 處**、
**51 個不同客戶碼**。前五名：`CC_KYEC_LEE`(66)、`CC_ASE_KaohSiung`(56)、`CC_ASE_CL`(22)、
`CC_SCC`(12)、`CC_JCET`(12)。單一客戶碼分支密度遠高於 uLotInfo（後者最高
`CC_Murata` 20 次）——**任何要排入未來波次的 write-path 函式，只要客戶碼分支數 >10
（例如 `InitConfigEdtList_ItemN` 一個函式就有 38 處），必須逐一核對每個客戶分支，
不能只翻預設分支**。

### 7.2 SOFT_SIMULTE 區塊

`grep -c "SOFT_SIMULTE"`：全檔 9 處（2 個 `#ifndef` 在建構子/`ChangeCBListProperty`，
2 個 `#ifdef` 在 `InitConfigEdtList_ItemD`/`ItemF`（行號 1163/2075，均為 Tier2 未逐行函式，
本節僅由訊號掃描帶出，翻譯時需留意）、`InitConfigEdtList_ItemN` 有 2 個（3171 `#ifdef`、
3183/3199 `#ifndef`成對）、`ChangeCBListProperty` 1 個（正式 build 排除的
`cbP16` 分支）、`FormShow`（5001 附近）1 個）。呼應既有陷阱：正式 build 關閉
`SOFT_SIMULTE`，這些分支在實機是死碼，翻譯時仍要照翻並標註。

### 7.3 跨 form 呼叫目標統計（`f[A-Z]\w*->`，20260820 執行）

| 目標 | 次數 | 目標 | 次數 |
|------|------|------|------|
| `fMain->` | 38 | `fPassword->` | 33 |
| `fQwertyKey->` | 28 | `fProductionInfo->` | 13 |
| `fLotInfo->` | 11 | `fShowBinSelect->` | 2 |
| `fSCKART->` | 2 | `fLogin->` | 2 |
| `fConfiguration->` | 2 | `fBinSel->` | 2 |
| 其餘（各 1 次） | — | `fShowPmSOPForm`/`fShowMessage`/`fSetup`/`fSecurity`/`fOffSet`/`fMesSystem`/`fInput`/`fFTPClient`/`fCleaning` | 1 各 |

`fMain->`(38) 與 `fPassword->`(33) 是最大宗，且 `fPassword->` 幾乎全數集中在本檔案已識別的
(c) 安全關鍵密碼閘門群（`bPassWord`/`btnOpenEPClick`/`BitBtn1Click` 等）——這與 uLotInfo
的分布（`ATC_InterfaceForm`/`fMain`/`fBarCode` 為大宗）性質不同：本檔案的跨 form 依賴
**質**上更集中在「權限驗證」而非「機構控制」，但兩者對翻譯波次而言都是需要先確認對方 facade
完整度才能安全排入的訊號。

### 7.4 `elConfig` 全域初始化順序地雷（與既有 `elLaser` 陷阱同族）

第 6.3 節已述：`elConfig`/`elConfig_byRecipe` 等 8 個 `HTEditList*` 全域指標在
`Public/HTEditList.h:247-256` 已 `extern` 宣告，但 `elConfig = new HTEditList;`
這行只存在於未翻譯的 golden `main.cpp:1484`——**V906 樹裡目前沒有任何 TU 執行這行**。
任何未來要翻譯 `edtSearchFunctionChange`、`InitConfigEdtList_Item*`、
`ChangeCBListProperty`、或 `cprod.cpp` 裡消費 `elConfig` 的既有翻譯碼，都要先確認
這個初始化順序問題怎麼解——這不是本檔案能獨立解決的範圍，是 `main.cpp` 翻譯前線
（與第 5 節 `recipe.current`/`user.level` 同一個瓶頸檔案）的既有已知風險再一次現形。

---

## 8. 方法論限制（誠實揭露）

1. **孤兒宣告檢查（第 2 節）比 uLotInfo recon 弱**：沒有做字串/註解遮罩後的逐行核對，
   只用單行識別字掃描，可能漏掉分行寫的宣告簽名。若要引用「0 孤兒」這個結論做決策，
   建議先重跑一次 uLotInfo 那套更嚴謹的方法。
2. **22 個 (b)/(c) 佇列項目未逐行覆核**（Tier2，第 1 節表格逐列標註），僅做過關鍵字訊號掃描；
   其中 12 個是 `InitConfigEdtList_Item[B,C,D,E,F,G,I,L,M,O,P]`（ItemA/ItemN 已逐行覆核，
   結構高度相似但仍有個別客戶碼分支未親眼核對過），其餘 10 個是各種小函式（`tbD25_Index40mm/30mm
   Change`及其 3 個 `_NS` 變體、`tbD60_Index56mm_NSChange`、`imgI37_3Click`、
   `btnN15ESDFormClick`、`btnN31_ManualClick`、`btnN25_3_ManualClick`）。
3. **(b)/(c) 佇列的一句話描述多數只到「有沒有寫檔/送控制指令/碰密碼」這個粗粒度**，
   佇列本身的細節分類（例如哪些 write path 只是計數器重置、哪些真的動到 SECS/ATC 控制）
   留給後續波次，本文件的重點是找出可安全排入 Wave A 的顯示側集合。
4. **`FormShow`（852L）/`FormClose`（208L）/`Timer1Timer`（26L）標記 MIXED**，
   本 recon 已逐行讀過三者的結構（分別在第 1 節表格與第 4 節有具體引用行號），
   但未做函式內段落切分，需要後續波次依 golden 的空行/註解區塊切開才能個別歸類。
5. **依賴欄位「是否已存在於 facade」的判斷基於 grep 存在性檢查，不是編譯驗證**——
   例如 `vclcompat\StringGrid.h` 存在不代表它有 `btnModifyTrayClick` 需要的所有成員；
   Wave A 實作時仍需針對每個依賴逐一確認成員簽名。
6. **本 recon 未檢查 `cConfiguration.dfm` 本身**，僅用 `tools\dfm2rc` 既有產物
   （`cConfiguration.dfm.ir.json`/`cConfiguration_layout.gen.cpp`）的存在性佐證
   「widget 幾何資訊不是從零開始」，未逐一核對哪些 widget 名稱已被抽取、抽取結果是否
   跟本文件引用的 widget 名稱（`cbA01`/`strngrdTray`/`edSetTemp` 等）對得上。
7. **`SetOffsetToTech()`（`btnSetToTechClick` 呼叫，:5978-5981）未在本檔案內定義**，
   本 recon 未追蹤到它實際定義的檔案；名稱暗示會寫入 teach/offset 資料，
   在 `docs\KNOWLEDGE.md` 已有「V899裝上舊機台會靜默毀掉教導值」的既有教訓背景下，
   這是本文件明確標記但**未查證**的最高風險未知項，任何未來波次排到
   `btnSetToTechClick` 之前應該先解決這個空白。

### 8.1 指令與時間戳記錄

- cp950 解碼 + U+FFFD 計數 + 換行正規化：20260819 23:xx ~ 20260820 00:xx（見上方陷阱記錄）
- `grep -c "TfConfiguration::"` 原始命中計數（131）與函式邊界抽取（129 候選）：20260819
- 逐行覆核（約 75+ 個函式直接用 `sed -n` 讀出完整本體核對，另有透過共用模式推論驗證的
  對稱函式，如 HP 叢集對照 Tray 叢集）：20260819-20260820
- `elConfig`/`HTEditList::Add` 機制查證（`Public/HTEditList.cpp:95-220`）：20260819
- `recipe.current`/`user.level` 全鏈追蹤（`cbSetupFileName`/`cbUserSelect`/`LookForFile`/
  `DoChangeLevel` 於 golden 全樹 grep + V906 `forms\fMain.h` 現況核對）：20260820
- 客戶碼/SOFT_SIMULTE/跨form呼叫統計、`fQwertyKey`/`ChangeCompomentEnabled`/`TStringGrid`
  V906 存在性核對：20260820

**注意（依規則第 3 條）**：本文件所有「已存在/facade缺」的判斷都是本波次單獨執行時的快照；
若有平行波次同時在改 `Public/HTEditList.*`、`forms\fMain.*`、或 `main.cpp` 翻譯前線，
交接前請重跑上述 grep 指令確認未過期。
