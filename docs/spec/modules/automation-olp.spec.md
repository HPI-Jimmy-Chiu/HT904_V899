# 模組 SPEC：Automation OLP（外部連線控制介面）

> Handler 對外 TCP/Socket 控制介面。供應商 Agent / GTK Loader / OLP Server 透過此介面取得機台狀態、下載 Setup File、執行 Lot / Start / Pause 等遠端操作。

## 1. 模組身份

- **模組名稱**：Automation OLP（OLP / Online Programming Interface）
- **負責檔案**：
  - `Automation/automation.cpp` / `automation.h` — 主分派器與封包解析
  - `Automation/auto9045.cpp` / `auto9045.h` — HT9045 機種專屬指令實作
- **適用版本**：V897, V898, V899（V899 修正 MAIN_STATUS_INQUIRE）
- **最後 review 日期**：2026-05-04
- **review 人**：JimmyChiu

## 2. 模組職責

接收外部 Agent 透過 TCP 送來的字串命令，解析後分派給對應實作函式；將機台狀態 / 設定 / 計數器資料以 OLP 封包格式回送。

## 3. 條件錨點

| 條件 | 取值 | 意義 | 程式碼位置 |
|------|------|------|-----------|
| `CosFunction.bOLPFunction` | bool | OLP 介面總開關 | `cprod.h` |
| `CheckSystemStart()` | bool | 機台是否在 Run 中（多數 SET 指令需 false） | `auto9045.cpp` |
| `CheckCanChangeRealDummy()` | bool | 是否允許切換 FT/RT、設定 Lot（Run 中 / 殘料 → false） | `main.cpp` |
| `bLockByServer` | bool | Safety Interlock 由 PAUSE_REQUEST/RESUME_REQUEST 翻轉 | `automation.cpp` |
| `palMainStatus->Caption` | `HALT/RUN/PAUSE/LOCK` | START_REQUEST 只在 HALT 有效 | `main.cpp` |

## 4. 資料錨點

| 資料 | 用途 | 來源 |
|------|------|------|
| `Command`（AnsiString） | OLP 命令字串，如 `LotInfo_REQUEST` | TCP 解析 |
| `V_Total`（AnsiString） | 後續資料筆數 | TCP 第 2 段 |
| `Data[0..N]`（AnsiString） | 命令參數陣列，最大 100 | TCP 後續欄位 |
| OLP 封包 | `STX + CMD + SOH + V_Total + (SOH + Data[i])* + ETX` | 自定義協定 |
| `aDataPath` | PP_DL_REQUEST 接收檔案存放路徑 | `cprod.h` |

## 5. 行為錨點（指令分類速查）

OLP 指令依 `_INQUIRE` / `_REQUEST` 區分讀寫：`_INQUIRE` 為查詢類（不改機台狀態），`_REQUEST` 為動作 / 設定類（改變狀態或檔案）。

### 5.1 狀態查詢類（read-only，安全）
| 指令 | 回傳內容 |
|------|---------|
| `MAIN_STATUS_INQUIRE` | HALT/RUN/PAUSE/LOCK |
| `VERSION_INQUIRE` | 軟體版本 |
| `ACT_TEMP_INQUIRE` | 各區實際溫度 |
| `TRAY_INFO_INQUIRE` | Tray 狀態 |
| `JAM_COUNT_INQUIRE` / `CATEGORY_INQUIRE` | 計數器 |
| `LotInfo_INQUIRE` | 當前 Lot ID / Operator / FT-RT |
| ...（共 ~80 個 _INQUIRE） | 詳見 `automation.cpp` L1017-1500 |

### 5.2 動作類（會改機台狀態）
| 指令 | 對應實作 | 前提條件 |
|------|---------|----------|
| `START_REQUEST` | `fMain->Start("TfAutomation::ProcessBuffer")` | palMainStatus="HALT" 且 SystemStart==false |
| `PAUSE_REQUEST` | `bLockByServer=true; SoftStop=true;` | 任何時候 |
| `RESUME_REQUEST` | `bLockByServer=false;` | 任何時候 |
| `CLEAR_REPORT_REQUEST` | `DoClearReportRequest(iSwitchCase)` | SystemStart==false |
| `LotInfo_REQUEST` | `SetLotInfo(Data)` → `SetLotID + SetLotStart + palFT/RTClick` | `CheckCanChangeRealDummy()` |
| `PP_DL_REQUEST` | 接收 .zip → `DoDLRequest(filename)` | — |
| `StartMode_REQUEST` | `SetStartMode(Data)` 設 InitialStart/ContinuStart 等 | `CheckCanChangeRealDummy()` |
| `ON_LINE_REQUEST` / `INITIATE_REQUEST` | 切上線 / 初始化 | — |

### 5.3 設定類（會改 ini / setup）
- 各種 `SetXxx_REQUEST`（SetAutoHeight、SetContactOffset、SetDropHeight、SetTrayType...）：寫入對應 `*.dat` 或 `*.ini`，部分會即時更新記憶體。

### 5.4 主動推送類（Handler → Agent）
- `ALARM_REQUEST` — 報警上報
- `MESSAGE_REQUEST` — 訊息上報
- `PRODUCTION_REQUEST` / `PRODUCTION_REPLY` — 生產數據
- `TEST_RESULT_REQUEST` — 測試結果上報

## 6. 通訊架構

```
External Agent ──TCP──► OLPServer (TServerSocket)
                               │
                               ▼
                         TCPstr buffer
                               │
                               ▼
                  ProcessBuffer() / ProcessBuffer1()
                               │
                  parse: Command + V_Total + Data[]
                               │
                               ▼
                  if-else if 巨型分派器（automation.cpp L1015-1500）
                               │
                               ▼
                  SetXxx() / DoXxx() / Get
                               │
                               ▼
                  CommandProcess("Xxx_REPLY", N, Data, ...)
                               │
                               ▼
                          STX+...+ETX 回送
```

## 7. 與其他模組的互鎖

- **上游**：外部 Agent / GTK Loader（Tester PC）
- **下游**：
  - `fMain->Start()` — 主啟動流程（含大量檢查，見 §8）
  - `fLotInfo` — Lot 設定 UI
  - `fProductionInfo` — OEE 批次管理（**未在 SetLotInfo 路徑被觸發**，見 F899-008）
  - `DoClearReportRequest()` — 計數器清除
- **互鎖點**：
  - `CheckSystemStart()` → 多數寫入指令拒絕 Run 中執行
  - `CheckCanChangeRealDummy()` → Run 中 / 殘料時拒絕改 Lot
  - `bLockByServer` → SoftStop 阻止 Start
  - 所有 `ShowMyMessage / ShowErrorMessage / ShowMyMessageBox_YES_NO` 在 OLP 路徑都會 modal block 主執行緒（**OLP 模式下需評估靜默化**，見 F899-008）

## 8. fMain->Start() 啟動檢查清單（影響 START_REQUEST 成敗）

`main.cpp:4214 TfMain::Start()` 內含 70+ 檢查點，任一失敗皆 return false。常見大類：

| 大類 | 範例條件 | 失敗時行為 |
|------|---------|-----------|
| 看門 / 重入保護 | `iStartIn!=0` | silent return |
| 機種特定檢查 | PANTHER `CheckMachineStationOnStart` | return false |
| Teach 防呆 | `CompareTechData()` | return false |
| Chamber Boost | `bStartChamberBoost` | RecordProcess + return |
| Bin 設定錯誤 | `CheckOLPError()` | return false |
| Event Log 參數 | `CheckEventLogParameter()` | return false |
| FTP Automation | `CheckingCheckList()` | `ShowErrorMessage("WAR16333")` |
| TCP/IP Tester 連線 | `bConnectOK==false` | `ShowMyMessage("TCP/IP does not connect!!")` |
| TTL RS232 | TTL 連線 / 版本不符 | `ShowMyMessage` |
| PM Alarm | `IsStopDate / IsAlarmDate` | `ShowMyMessage("PM Stop When Lot Start!")` |
| **OEE Start Lot** | `IsOEEStartLotSuccess()==false` | **`ShowMyMessage("Please Start Lot!")`** ← F899-008 觸發點 |
| Lot 必填欄位 | LotID / OperatorID / RunMode 空 | `ShowMyMessage("Please Enter LotID...")` |
| Lot 流程 | 沒按過 Lot Start | `ShowMyMessage("Please Press Lot Start!!")` |
| Run Mode 比對 | RT / FT 切換錯 | `ShowMyMessage("Run RT Start mode error!!")` |
| 2D ID list | source file 缺 / white list 空 | `ShowMyMessage` |
| Magazine / Tray | Teach pos = 0 / sensor 缺 | `ShowMyMessage` |
| 溫度模式 | Temperature Mode High 未開溫 | `ShowErrorMessage("WAR15190")` |
| Y-Pitch 機構 | InArm / OutArm pitch 不符 | `ShowMyMessage("Please Adjust ... Y pitch...")` |
| ATC Ambient | 環溫不在 25-30 | `ShowErrorMessage("WAR15310")` |
| Auto Height | 加熱中 / 未做 height calibration | `ShowMyMessage` |

> ⚠️ **所有 `ShowMyMessage / ShowErrorMessage` 都是 modal dialog**，在 OLP 模式下會阻塞 OLP 回應，造成 Agent 端 timeout。

## 9. Known Issues / 歷史 bug

- V899.8：`MAIN_STATUS_INQUIRE` 任何狀態都回空資料 → 修復於 V899.x（`F899-003`）
- V899.x：`LotInfo_REQUEST` 雖回 `[0]`，但未觸發 OEE_StartLot，導致後續 START_REQUEST 被擋（`F899-008`，超豐 case）
- **SECS S2F41 RCMD `START` 同 bug**：`SECSGEM/uHGemHT9045.cpp` ~L1322 直接呼叫 `fLotInfo->sbSECSLotStartClick`，內部 modal 同樣會卡 SECS 回應主執行緒。目前無 SECS 客戶反映 → **不修**。等將來反映時再開新 SPEC，可重用 F899-008 的 `DoSECSLotStartCore(bSilent=true, ...)` 抽出函式。

## 10. 關聯 SPEC

- `features/F899-003-main-status-inquire-fix.spec.md`
- `features/F899-008-remote-start-lot-replacement.spec.md`

## 11. 關聯 Skill

- `.agents/skills/ht9045-autostart-flow/SKILL.md`
- `.agents/skills/ht9045-lotinfo-flow/SKILL.md`
- `.agents/skills/ht9045-secsgem/SKILL.md`