# F899-008 — LotInfo_REQUEST 內嵌 OEE Start Lot（超豐遠端 Start Lot）

| 欄位 | 值 |
|------|-----|
| Feature ID | F899-008 |
| 版本 | V3.33.899.x |
| 對應 CASE-ID | CASE-20260504-XXX（待開立） |
| 客戶 | **超豐（Greatek）專用** |
| 狀態 | design（V899.x 第一版實作偏差已校準；2026-05-15 二次校準：客戶確認超豐 Agent **不送** LotID/OPID/RunMode，三欄全部由雲端 MO file 帶入，本 SPEC 對應移除 RunMode 必填、LotID/OPID 寫入與字元檢查）|
| 影響模組 | Automation OLP（**僅改超豐 `LotInfo_REQUEST` 分流**）/ auto9045（新增 `SetLotInfoGreatekOEE`）。**uLotInfo 與 ProductionInfo 零修改**（見 §6.1）|
| **Scope** | **Greatek only — 其他客戶 `LotInfo_REQUEST`、S2F41 與既有手動行為完全不動** |
| 採用方案 | **E-客戶序列不變：`LotInfo_REQUEST` 內嵌 OEE Start Lot** |

## 0. 設計約束（重要）

由使用者於 2026-05-04 明確指定：

> 修改範圍僅限超豐，Automation 模組由其他客戶共用。

衍生規則：
- ❌ 不可修改**非超豐客戶**既有 `LotInfo_REQUEST` 行為
- ❌ 不可修改既有 `START_REQUEST` 行為
- ❌ 不可修改既有 SECS S2F41 `START` RCMD 行為（即使有同樣 bug，亦列為 Known Issue 不在本 SPEC 範圍）
- ❌ 不可全域改寫 `ShowMyMessage`
- ❌ 不新增客戶端可見指令（客戶明確不要 `OEEStartLot_REQUEST`）
- ❌ 不重構 `sbSECSLotStartClick`（2026-05-15 推翻原設計，見 §6.1）
- ✅ 可在 `CUSTOMER_CODE==CC_Greatek` 且 OEE 開啟時，擴充 `LotInfo_REQUEST` 的 Handler 端行為
- ✅ 可新增「超豐專用」`SetLotInfoGreatekOEE` 函式拼起 §1.2 順序，**不經** sbSECSLotStartClick

## 1. 背景與動機

詳細時序與根因分析見 repo memory `chipmos-v8998-lotinfo-not-startlot.md`。

V3.33.899.8 現場驗證：
- 超豐 Agent 送 `LotInfo_REQUEST` → 回 `[0]` 看似成功（**只完成 Lot Info 表單層**）
- 緊接送 `START_REQUEST` → 被 `CheckOEE_WhenStart()` 攔截
- 機台跳出 modal `Please Start Lot!` 等人按 OK
- `START_REPLY` 延遲 12 秒回送，Agent 序列宣告失敗

根因：「OEE 系統層 Start Lot」目前唯一觸發點是手動按 Lot Info 介面的 Start Lot 按鈕（即 `sbSECSLotStartClick` OnClick），OLP 的 `LotInfo_REQUEST` 只執行 `SetLotInfo(Data)`，沒有執行 `OEE_StartLot()`。

2026-05-04 客戶追問後確認：客戶**不要新增 `OEEStartLot_REQUEST`**，希望維持既有兩步順序：

```
LotInfo_REQUEST → START_REQUEST
```

因此本 SPEC 改採方案 E：`LotInfo_REQUEST` 在超豐 OEE 模式下，同步完成「Lot Info 寫入 + silent OEE Start Lot」，成功後客戶再送既有 `START_REQUEST`。

### 1.1 既有 SECS 對等功能（參考用，本案不動）

`SECSGEM/uHGemHT9045.cpp` ~L1322 的 S2F41 RCMD `START` 已有「直接呼叫 OnClick」的慣例：
```cpp
fLotInfo->sbSECSLotStartClick(fLotInfo);
RecordProcess("SECS/GEM LOTSTART!");
if(SystemStart==false) {
    fMain->Start("SECS GEM RCMD : START");
    HCACK=0;
}
```
證明「重用 OnClick 邏輯」是專案既有慣例。但此路徑同樣有 modal 卡死問題（見 [automation-olp.spec.md §9 Known Issues](../modules/automation-olp.spec.md)），本 SPEC **不修復 S2F41**，留給未來 SECS 客戶反映時再開新 SPEC。

### 1.2 手動 `sbSECSLotStartClick` 對超豐的權威呼叫順序（2026-05-15 補加）

> 這是 V899 現況讀完 `uLotInfo.cpp` L7114-L8074 + `auto9045.cpp` 後的 ground truth。`SetLotInfoGreatekOEE`（§6.3）必須**逐步對齊**這個順序，順序錯誤會造成 LotID 早於雲端 MO 下載被覆蓋，EventLog 與 SECS 事件用到舊 LotID。

對 `CUSTOMER_CODE==CC_Greatek` 實際流經的步驟（其他客戶分支自動跳過）：

1. `bReadLotInfoFromART = false`，初始化區域變數，`mmo2DLotInfo->Clear()`，`dtStartLot = Now()`
2. **2D Sort 分支**（`CosFunction.bSortingBy2DList && LastSet.iTester==_2D_SORT`）— 超豐沒開，跳過
3. **2DID Allow List 分支**（`TestIF_File.b2DIDAllowList`）— 超豐沒開，跳過
4. **特殊字元檢查**（TRegExpr）：LotID / OPID / RunMode 含 `\/:*?"<>|` → 清空欄位 + ShowMyMessage，**注意：不 return**，流程繼續往下走（後面才會被空值檢查擋）
5. **SCC 長度檢查**：跳過（非超豐分支）
6. **逐客戶必填欄位檢查** → 進入 `else if(CUSTOMER_CODE==CC_Greatek)` 分支（uLotInfo.cpp L7749-L7752）：
    ```cpp
    else if(CUSTOMER_CODE==CC_Greatek)  // Sam 20171101: 超豐不用檢查 OPID and LotID
    {
    }
    ```
   **完全空白**，LotID / OPID / RunMode **完全不做必填檢查**（手動 UI 上即使欄位是空也可以按 Start Lot）
7. VTEST MES system download — 超豐沒開，跳過
8. 偉測 Summary check — 超豐沒開，跳過
9. **OEE 分支**（`CosFunction.bOEEFunction == true`，uLotInfo.cpp L8021-L8043）：
    1. Monitor 連線檢查（[uLotInfo.cpp#L8023](../../../HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422/uLotInfo.cpp#L8023)：`bC11UseMonitorView && fMonitor->MVCtrl->IsConnect()==false`）→ `ShowMyMessage("Please Check Monitor Connect!!!")` + return
    2. **`ShowMyMessageBox_YES_NO("Are You Sure Start Lot?")`** — 手動才有，選 No 直接 return
    3. **`fProductionInfo->OEE_StartLot()`**（[ProductionInfo.cpp#L229-L340](../../../HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422/ProductionInfo/ProductionInfo.cpp#L229)）內部會：
        - `SetMOInformation()` 解析雲端 MO 設定（取 `_sOEE_MO`、`_sOEE_HandlerID`、`_sOEE_DirectoryName`）
        - **寫入 `fLotInfo->ed_PIOEEMO->Text = GetScheduleName()`** — 只更新 OEE MO 顯示欄位，**不寫 `edtSysLotID->Text`**
        - 若 `IniConfig.bN14_10_DownFileByMO` true，`LoadMOInformation()` 由 OEE FTP 抓 MO 檔 + `AutoDownloadSetupFileByMO()` 下載對應 Setup File（ProductionInfo.cpp L308 / L2369）
        - `_bOEEStartLotSuccess = true` 並 `SaveSetting()`
        - 失敗 → ShowMyMessage("Production Start Lot Fail!#Please Check MO or Machine ID") + return
        - **重要**：grep `edtSysLotID->Text\s*=` 確認 `ProductionInfo.cpp` 內**沒有**任何 `edtSysLotID->Text =` 指派；換言之，雲端下載**不會**改變 `edtSysLotID`，到 step 12 時 `edtSysLotID` 仍是手動使用者輸入（或 Agent 透過 Lot Info 表單寫入）的值
10. CC_Murata DAYDATE 字串置換 — 跳過
11. CC_PANTHER PAT — 跳過
12. **`SetLotID(edtSysLotID->Text, false)`**（uLotInfo.cpp L8061）— 讀取 `edtSysLotID->Text` 當下值寫入 RunInfo / EventLog；超豐手動模式下這個值可能是空字串（使用者沒輸入）
13. **`SetLotStart(__FUNC__)`**（uLotInfo.cpp L8062）— 真正進入 Lot 狀態。`SetLotStart` 內部（[uLotInfo.cpp#L1412-L1545](../../../HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422/uLotInfo.cpp#L1412)）：
    1. `RecordProcess("Start Lot Press", sFunc)` — EventLog
    2. `SetLotID(edtSysLotID->Text)` — 寫 RunInfo.LotNo / config.ini
    3. `lbledtStarTime->Text` 設定 Start Time
    4. `slEventLog->SetLotData(...)` — EventLog 切換到新 LotID
    5. `Msg.sprintf("Lot Start, Lot ID:%s, OP ID:%s, Run Mode:%s", ...)` + `RecordProcess`
    6. `ReadWriteLotInfo(false)` — 寫 LotInfo 檔
    7. `SetTesterStartTimeByB03()` (PTI ART)
    8. `bRunModeFollowLotInfo` 下依 cbRunMode 切 FT/RT/EQC
    9. SECS Event Report（`DoLotStart` 或 `DoVisualSortLotStart`）
    10. **⏫ 關鍵分支 ⏬**：
        ```cpp
        if(edtSysLotID->Text != "")
            SetLotComponents(false);   // 鎖定 UI 欄位、RunInfo.bLotStart=true
        else
            SetLotComponents(true);    // ★ 解鎖、RunInfo.bLotStart=false ★
        ```
        **超豐手動使用者若未輸 LotID**，這裡會走到 `SetLotComponents(true)` — 欄位不鎖、`RunInfo.bLotStart=false`（請見 §1.4）
14. CC_TSMC_TAINAN modal — 跳過
15. `SetFirstTrayCheckOnUnloader()`（若 First Tray Check 開啟）

#### 1.2.1 不可違反的順序 invariant

| Invariant | 為什麼重要 |
|-----------|-----------|
| `OEE_StartLot()` **必須早於** `SetLotStart()` | OEE 會下載 SetupFile（影響後續 TrayForm/SetupName）並設定 `_sOEE_MO` 等內部狀態，這些狀態必須在 SetLotStart 進入 RunInfo / SECS Event 之前就位；OEE 失敗時整批必須 abort |
| `OEE_StartLot()` **不會改寫 `edtSysLotID->Text`** | 設計上 LotID 由 Lot Info 表單欄位提供（手動：使用者輸入；遠端：Agent 透過 LotInfo_REQUEST 寫入 Data[0]）。OEE 只負責 MO 與 SetupFile 同步 |
| 字元檢查只清空欄位，**不 return** | 後段流程仍會用空字串繼續，前段不可 early return |
| 超豐分支對 LotID / OPID **完全不檢查** | 手動 UI 允許空值送入 SetLotID/SetLotStart；遠端對齊（不強制）。RunMode 是唯一例外：手動不檢（UI 下拉預選），但遠端因 OEE 帶不來 RunMode，必須要求 Agent 帶值（見 §12.0 Q8 / §6.3 return 24）|
| OEE_StartLot 失敗 **一律 return** | 失敗代表雲端 MO 取不到，後面的 SetLotStart 絕對不能執行 |

### 1.3 V899.x 第一版實作的偏差（2026-05-15 校準）

目前 `auto9045.cpp` 的 `SetLotInfoGreatekOEE` 已上機，但與 §1.2 順序**不對齊**，需要在本 SPEC 進入 build 前修正：

| 偏差 | 現況 | 對齊手動的正確做法 |
|------|------|--------------------|
| LotID 強制必填（return 20/21） | `if(asLotID=="") return 20;` | 允許空字串（手動 Greatek 分支即不檢查，遠端對齊）|
| OPID 強制必填（return 21/26） | `if(asOPID=="") return 21;` `if(asOPID.Length()<4) return 26;` | 允許空字串、移除長度檢查（手動 Greatek 分支即不檢查）|
| 順序顛倒 | `SetLotInfo(Data)`（內含 SetLotID + SetLotStart）→ 後 `OEE_StartLot()` | **先寫欄位（Data[0]/Data[1]/Data[2] → edtSysLotID/edtSysOperatorID/cbRunMode）→ OEE_StartLot → SetLotID(edtSysLotID->Text) → SetLotStart**；OEE 失敗一律 return |
| 字元檢查在最前 | 直接 return 30/31/32 | 空字串跳過檢查；LotID/OPID **非空且含非法字元時 early return 30/31 + 清欄位**（遠端受控偏離手動「清欄位不 return」設計，避免 silently 走入 OEE；見 §6.3 註解）|
| **不強制鎖定 UI**（依賴 SetLotStart 內部 if-else） | LotID 為空時 → `SetLotComponents(true)` → 欄位未鎖、`RunInfo.bLotStart=false`、`START_REQUEST` 會被 `CheckOEE_WhenStart` 加鎖 | **遠端成功後顯示呼叫 `SetLotComponents(false)` 強制鎖**（見 §1.4 / §6.3）|

### 1.4 Button Lock 機制與遠端路徑的設計決策（2026-05-15 新增）

#### 1.4.1 `SetLotComponents` 是唯一鎖定/解鎖點

[uLotInfo.cpp#L2086-L2123](../../../HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422/uLotInfo.cpp#L2086)：

```cpp
void TfLotInfo::SetLotComponents(bool bLotEnd)
{
    RunInfo.bLotStart            = !bLotEnd;
    edtSysLotID         ->Enabled= bLotEnd;   // false=鎖 / true=解鎖
    sbSECSLotStart      ->Down   =!bLotEnd;
    sbSECSLotEnd        ->Down   = bLotEnd;
    pnlLoader           ->Caption="";
    edPage              ->Enabled= bLotEnd;
    edtSysOperatorID    ->Enabled= bLotEnd;
    edCustomerLotId     ->Enabled= bLotEnd;
    coStation           ->Enabled= bLotEnd;
    edStationNum        ->Enabled= bLotEnd;
    edtJobSeq           ->Enabled= bLotEnd;
    cbRunMode           ->Enabled= bLotEnd;   // PTI B03 例外
    edtCusLotID         ->Enabled= bLotEnd;
    edtCusDevGrp        ->Enabled= bLotEnd;
    edtDevice           ->Enabled= bLotEnd;
    // ... 共 20+ 個 UI 控件
}
```

- `bLotEnd=false` = **Lot Start 狀態**，Lock 所有欄位 + Start 鈕 Down
- `bLotEnd=true`  = **Lot End 狀態**，Unlock 所有欄位 + End 鈕 Down

#### 1.4.2 Lock 取決於 LotID 是否為空的現有 bug

`SetLotStart` L1532-L1535 的 if-else 造成：

| 場景 | edtSysLotID->Text | SetLotComponents() | RunInfo.bLotStart | 欄位狀態 |
|------|-------------------|-------------------|-------------------|----------|
| 手動超豐使用者有掃 cassette / 手打 LotID | 非空 | `false` | `true` | **鎖定** ✅ |
| 手動超豐使用者沒輸 LotID（依赖雲端 MO） | 空 | `true` | `false` | **解鎖** ❌ |
| 遠端 Agent 送 LotID | 非空 | `false` | `true` | **鎖定** ✅ |
| 遠端 Agent **不送 LotID**（依赖雲端 MO） | 空 | `true` | `false` | **解鎖** ❌ |

後兩個場景是遠端使用者主訴的 case。現況是：即使 Start Lot 「表面上成功」（OEE 靜默下載成功、沒拋錯誤），`RunInfo.bLotStart` 仍然 = false，接下來的 `START_REQUEST` 進到 `CheckOEE_WhenStart` 會變成另一個加鎖點。

#### 1.4.3 設計決策：遠端路徑強制 lock

根據使用者需求：

> 「遠端 Start lot 成功後必須像手動一樣 Lock 按鈕，直到 End lot 成功後才能解開」

選择是「在 `SetLotInfoGreatekOEE` 內、`SetLotStart()` 之後顯示呼叫 `fLotInfo->SetLotComponents(false)`」。理由：

1. **不動 `SetLotStart` 原始 if-else**：保護其他客戶路徑（手動 / SECS / 非超豐遠端）零影響
2. **深度最小**：只加 1 行，不重構核心狀態機
3. **對齊使用者語意**：「遠端成功 → 必鎖」是設計意圖，LotID 是否为空不該影響鎖狀態
4. **`SetLotComponents(false)` 會同時設 `RunInfo.bLotStart=true`**，清除 `START_REQUEST` 被 `CheckOEE_WhenStart` 加鎖的風險
5. **隱含設計選擇**：不重構 `sbSECSLotStartClick`（見 §6.1 推翻原設計），由 SetLotInfoGreatekOEE 直接拼起 Greatek 6 步

#### 1.4.4 End Lot 解鎖現況

[uLotInfo.cpp#L2050-L2051](../../../HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422/uLotInfo.cpp#L2050)（`sbSECSLotEndClick` 內，在 `OEE_EndLot()` 之後）：

```cpp
SetLotComponents(true);   // 解鎖所有欄位
SetLotID("");
```

**TCP 端現況：`automation.cpp` grep 全文沒有 `LotEnd_REQUEST` / `END_REQUEST` / `EndLot_REQUEST` 。也就是說 Greatek 遠端 Agent 目前**沒有「送一條 End Lot」的路徑**，End Lot 需為之一：

1. 現場作業員手動按 `sbSECSLotEnd` 按鈕
2. CleanOut 完成時由 [csystem.cpp#L9930 / L10340](../../../HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422/csystem.cpp#L9930) 設 `bNeedLotEndAfterCleanOut=true`，但這只會讓下一次 START 丟出「Please End Lot!」警告，仍需人手介入 End Lot（見 [csystem.cpp#L16330](../../../HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422/csystem.cpp#L16330) 與 [ProductionInfo.cpp#L5072](../../../HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422/ProductionInfo/ProductionInfo.cpp#L5072) `CheckOEE_WhenStart`）
3. 未來另開 SPEC F899-009 新增 `LotEnd_REQUEST`

**重要：`OEE_StartLot` 內部會 `bNeedLotEndAfterCleanOut=false`**（[ProductionInfo.cpp#L270](../../../HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422/ProductionInfo/ProductionInfo.cpp#L270)）。代表遠端 Agent 可以「上一批 CleanOut 後未 End Lot → 直接送下一批 LotInfo_REQUEST」，新一批 OEE_StartLot 會清上一批的旗標。但這樣上一批的 OEE 報表不會被 `OEE_EndLot()` 推進（UpdateTrayCntReport / SaveProductionSummaryReport / SaveProductionDataToPath / InitialOEECount 都不會跳）— 需在 §10 風險表警告並於客戶交付文件明記。

本 SPEC **不新增** End Lot 遠端指令（保持客戶原訂「只動 LotInfo_REQUEST + START_REQUEST」範圍），但必須明確告知客戶 Agent：**End Lot 需走現場手動或原有 CLEANOUT 後人工介入路徑**。

## 2. 需求摘要

擴充**超豐專用** `LotInfo_REQUEST` 行為：
1. 客戶端仍維持 `LotInfo_REQUEST` → `START_REQUEST`，**不新增指令**
2. `LotInfo_REQUEST` 成功時，代表 Lot Info 寫入與 OEE Start Lot 均成功
3. 失敗時由 `LotInfo_REPLY` 回傳明確 Reply Code + 英文 Detail Message
4. OLP 路徑不跳 modal dialog（避免阻塞主執行緒）
5. 非超豐客戶、手動 / SECS 路徑行為 100% 不變

## 3. Agent 流程對照

### 現況（壞）
```
Agent → LotInfo_REQUEST → [0] OK
Agent → START_REQUEST  → modal 卡 12s → [late reply]
```

### 新流程（好）
```
Agent → LotInfo_REQUEST → [0] OK            （同時完成 Lot Info 寫入 + OEE Start Lot）
Agent → START_REQUEST   → [0] OK            （既有，不改）
```

## 4. 條件錨點（Conditions）

| 條件 | 來源 | 說明 |
|------|------|------|
| `CosFunction.bOLPFunction` | cprod.h | OLP 介面總開關 |
| `CosFunction.bOEEFunction` | cprod.h | OEE 系統開關 |
| `IniConfig.bN14_1_EnableOEEFunction` | ini | OEE 細項開關 |
| `_sOEE_MO`, `_sOEE_HandlerID` | ProductionInfo | OEE 必要參數 |
| `CUSTOMER_CODE==CC_Greatek` | customer define | 只有超豐分流會觸發內嵌 OEE Start Lot |
| Lot Info 必須先設定 | 流程 | 由同一個 `LotInfo_REQUEST` 寫入 |

> 注意：本方案**必須檢查 CUSTOMER_CODE**。只有 `CUSTOMER_CODE==CC_Greatek` 且 OEE 開啟時，`LotInfo_REQUEST` 才會多做 OEE Start Lot；其他客戶仍走原 `SetLotInfo(Data)` + `LotInfo_REPLY(1 field)`。

## 5. 資料錨點（Data）

### 設計原則（依 2026-05-04 客戶答覆與追問）

> **Q1 答覆**：「LotInfo_REQUEST 參數沿用之前提供的。」
>
> **追問答覆**：「客戶不要 OEEStartLot_REQUEST，希望依照之前提供的順序。」

因此 `LotInfo_REQUEST` 仍沿用既有 3 欄位（LotID / OperatorID / FT-RT）的 framing，但在超豐 OEE 模式下，**Handler 端忽略全部 3 個欄位**，所有資訊由雲端 MO file 帶入，並在同一流程內接著執行 silent OEE Start Lot。

### 5.1 `LotInfo_REQUEST`（Agent → Handler）

> **2026-05-15 二次校準**：客戶確認超豐 Agent 完全不送 LotID/OPID/RunMode（三欄全部由雲端 MO file 帶入），本 SPEC 對應**移除** RunMode 必填、LotID/OPID 寫入 UI、LotID/OPID 字元檢查。Reply Code 24 / 30 / 31 在 Greatek 路徑不再觸發。RunMode 維持機台 UI 當下選擇值（不被覆寫）。

| Index | 欄位 | 必填（Greatek OEE 分流） | 說明 |
|-------|------|------|------|
| Data[0] | LotID | **不使用** | Handler 忽略（即使 Agent 送也不寫 UI、不影響流程）。LotID 由 OEE_StartLot → MO file 帶入 |
| Data[1] | OperatorID | **不使用** | 同上 |
| Data[2] | FT-RT（測試類型）| **不使用** | RunMode 由現場操作員預先在 UI 選好（FT/RT 切換鍵），Agent 不需帶；機台不依 Data[2] 切換 RunMode |

**呼叫範例**：

```
# 標準（推薦）：三欄全空
LotInfo_REQUEST,,,<EOF>

# 與舊版相容：Agent 仍送舊欄位也接受（內容會被忽略）
LotInfo_REQUEST,LOT12345,OP9876,0<EOF>
```

> 註：保留 3 欄位 framing 是為了兼容舊版 Greatek Agent；Handler 不解析也不檢查內容。

### 5.2 `LotInfo_REPLY`（Handler → Agent）

| Index | 欄位 | 說明 |
|-------|------|------|
| Data[0] | Reply Code | 見 §8 完整對應表 |
| Data[1] | Detail Message | 失敗時帶詳細英文原因字串（供 log 與客服排查）|

> 非超豐客戶維持舊版 `CommandProcess("LotInfo_REPLY", 1, Data, ...)`；超豐 OEE 分流可回 2 欄位，Data[1] 一律英文。

**成功範例**：
```
LotInfo_REPLY,0,<EOF>
```

**失敗範例**（雲端 MO 下載失敗）：
```
LotInfo_REPLY,61,OEE MO download fail (cloud unreachable or MO not found)<EOF>
```

## 6. 行為錨點（Behavior）— 方案 E：內嵌於 `LotInfo_REQUEST`

### 6.1 設計決策：**不重構 `sbSECSLotStartClick`**（2026-05-15 推翻原設計）

#### 6.1.1 廢棄方案：`DoSECSLotStartCore` 薄包裝抽出

原 SPEC 計畫把 `sbSECSLotStartClick`（uLotInfo.cpp L7114-L8074，數百行）拆成：
- `DoSECSLotStartCore(bSilent, replyCode, errMsg)` 核心
- `sbSECSLotStartClick` 薄包裝（一行呼叫 core 並傳 `bSilent=false`）

#### 6.1.2 作廢理由

2026-05-15 重新讀完 [ProductionInfo.cpp#L229-L340](../../../HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422/ProductionInfo/ProductionInfo.cpp#L229) 後發現：

1. **`OEE_StartLot(bool bSilent, AnsiString *pasErrorMsg)` 既有介面已足夠**：2026-05-04 ht9045-v899 已實作 silent + 錯誤訊息回傳，每個失敗分支都已 conditional `ShowMyMessage`，`bSilent=true` 時不跳 modal。
2. **超豐分支在 `sbSECSLotStartClick` 內只有 6 個有效步驟**：字元檢查（清欄位不 return）→ Greatek 必填檢查空白 block → OEE 分支（Monitor 連線 + YES/NO + OEE_StartLot）→ SetLotID → SetLotStart → SetFirstTrayCheckOnUnloader。其他 95% 是 2D Sort / 2DID Allow List / VTEST MES / Murata DAYDATE / PANTHER PAT / TSMC TAINAN modal — Greatek 完全不走。
3. **重構 `sbSECSLotStartClick` = 高回歸風險**：該函式被 PTI / KYEC / Sigurd / TSMC / Murata / VTEST / PANTHER 共用，重構等價性難證明。

#### 6.1.3 取代方案

`SetLotInfoGreatekOEE` **直接拼起** Greatek 6 步，不經 `sbSECSLotStartClick` 也不重構之。詳見 §6.3。

| 維度 | 重構薄包裝（廢棄） | 直接拼起（採用）|
|------|-------------------|------------------|
| 重構規模 | uLotInfo.cpp 數百行 | auto9045.cpp 約 30 行 |
| 回歸風險 | 影響 7+ 客戶手動 / SECS 路徑 | 僅影響超豐 OLP 分流 |
| Code review 負荷 | 必逐行比對 diff | 對照 §1.2 6 步驟即可 |
| 未來客戶複用 | core 可被其他客戶 OLP 重用 | 須複製 6 步（可接受，客戶罕新增）|
| 對齊保證 | 「等價」依賴 modal 改寫無遺漏 | 「對齊」依賴 §1.2 / §1.4 順序鐵則 |

### 6.2 OLP 入口（automation.cpp）

> 回覆欄位數採 **成功 1 欄 / 失敗 2 欄** 的 split，對齊 §10 緩解策略（Greatek Agent 既有 parser 可不改、只看 Data[0]；失敗才需解析 Data[1] 英文 detail）。

```cpp
else if(Command=="LotInfo_REQUEST")
{
    if(CUSTOMER_CODE==CC_Greatek && CosFunction.bOEEFunction)
    {
        AnsiString asErrorMsg="";
        Data[0]=SetLotInfoGreatekOEE(Data, asErrorMsg);   // int → AnsiString 隱式轉換
        Data[1]=asErrorMsg;
        if(Data[0]=="0")
            CommandProcess("LotInfo_REPLY", 1, Data, bClient, iHandle);   // 成功 1 欄
        else
            CommandProcess("LotInfo_REPLY", 2, Data, bClient, iHandle);   // 失敗 2 欄（含 errMsg）
    }
    else
    {
        Data[0]=SetLotInfo(Data);                         // 舊行為完全保留
        CommandProcess("LotInfo_REPLY", 1, Data, bClient, iHandle);
    }
}
```

### 6.3 超豐 `LotInfo_REQUEST` 處理函式（auto9045.cpp / auto9045.h）

> **2026-05-15 重寫 v3**：客戶二次確認 Agent 不送 LotID/OPID/RunMode（全部由雲端 MO file 帶入），故移除 RunMode 必填、UI 欄位寫入、字元檢查三段。最終流程為：預檢查 → OEE_StartLot → SetLotID → SetLotStart → SetLotComponents(false) → SetFirstTrayCheckOnUnloader。不重構 `sbSECSLotStartClick`（見 §6.1）。

```cpp
//AI(JimmyChiu) 20260515: F899-008 v3：客戶不送 LotID/OPID/RunMode，全部由雲端 MO file 帶入。
// 設計決策：不重構 sbSECSLotStartClick（見 §6.1）。
// 等價保證：對齊 §1.2 step 9（OEE 分支）/ 12 / 13 / 15。
int SetLotInfoGreatekOEE(AnsiString *Data, AnsiString &errMsg)
{
    (void)Data;     // 客戶決定不使用，保留參數僅為呼叫端 signature 一致
    errMsg = "";

    // ── (1) 預檢查（手動沒有對應，但 OLP 入口需擋掉根本不該執行的狀態）──
    if(!fMain->CheckCanChangeRealDummy())
    {
        errMsg = "Machine running or has residual IC";
        return 2;
    }

    // Monitor 連線檢查（對齊 §1.2 step 9.1，手動由 sbSECSLotStartClick L8023 直擋）。
    if(IniConfig.bC11UseMonitorView && fMonitor->MVCtrl->IsConnect()==false)
    {
        errMsg = "Please Check Monitor Connect!!!";
        return 60;
    }

    // ── (2) OEE_StartLot（silent，使用 ProductionInfo.cpp#L229 既有介面）──
    //   失敗時依 errMsg 關鍵字細分 Reply Code（見 §8.5 對盤結果）：
    //     - 唯一含 "Machine ID" 的字串 "MO or Machine ID is empty" → 62
    //     - 其餘 5 條（SetMOInformation Fail / Auto Download Setup File by MO ... / LoadMOInformation 等）皆含 "MO" → 61
    //   90 為防禦性 fallback（理論不觸發）。AnsiString.Pos 大小寫敏感。
    if(!fProductionInfo->OEE_StartLot(true, &errMsg))
    {
        if(errMsg.Pos("Machine ID") > 0) return 62;
        if(errMsg.Pos("MO") > 0)         return 61;
        return 90;
    }

    // ── (3) SetLotID + SetLotStart（對齊 §1.2 step 12/13）──
    //   edtSysLotID->Text 由 OEE 雲端下載階段透過 MO 對應流程更新。
    fLotInfo->SetLotID(fLotInfo->edtSysLotID->Text, false);
    fLotInfo->SetLotStart(__FUNC__);

    // ── (4) §1.4 遠端必鎖：LotID 為空時 SetLotStart 內部會走 SetLotComponents(true) 不鎖，
    //    遠端路徑必須強制鎖定以保證「成功 ≡ 鎖定」語意。
    //    並同時設 RunInfo.bLotStart=true，避免後續 START_REQUEST 被 CheckOEE_WhenStart 加鎖。
    fLotInfo->SetLotComponents(false);

    // ── (5) First Tray Check（對齊 §1.2 step 15）──
    fLotInfo->SetFirstTrayCheckOnUnloader();

    return 0;
}
```

### 6.4 順序鐵則：為何 `OEE_StartLot` 必須早於 `SetLotID/SetLotStart` / `SetLotComponents`

以下兩種順序都會產生不同錯誤，**只有前者正確**：

```
[正確 — 對齊手動 §1.2]
    （前置欄位寫入 Agent 傳值）
    OEE_StartLot(true, &errMsg)   ← SetMOInformation、下載 SetupFile/TrayForm、設 _sOEE_MO/_bOEEStartLotSuccess、reset bNeedLotEndAfterCleanOut=false
    SetLotID(edtSysLotID->Text, false)
    SetLotStart(__FUNC__)         ← RunInfo 中的 SetupName/Yield Monitor 狀態反映雲端下載後的設定
    SetLotComponents(false)       ← 補空 LotID 不鎖缺口（見 §1.4.3）
    SetFirstTrayCheckOnUnloader()

[錯誤 — V899.x 第一版實作 §1.3]
    SetLotInfo(Data)    ← 內部已 SetLotID(Data[0]) + SetLotStart(__FUNC__)
                          → RunInfo 、EventLog、SECS Event 都被記錄一次（用舊 SetupFile）
    OEE_StartLot()      ← 實際下載雲端 MO + SetupFile
                          → 區間裡如果 Lot Start 副作用需要依 SetupFile 初始化 (TrayForm/Yield)，已來不及
                          → 若 OEE 失敗，此時 RunInfo.bLotStart 已 = true，無法乾淨回復
```

重點不是「OEE 覆寫 LotID」（實際上不會覆寫 edtSysLotID），而是：
1. **OEE 會設定兩個內部狀態**：`_sOEE_MO`、`_bOEEStartLotSuccess`、以及 SetupFile/TrayForm 下載。SetLotStart 內部依賴這些狀態寫 RunInfo 與 SECS Event。
2. **OEE 失敗必須 abort 整個 Lot Start**；若 SetLotStart 已先執行，OEE 失敗時無法「取消 Lot」，會留下不一致狀態。

### 6.5 依賴的現成 API 摘要（不修改）

| API | 位置 | 副作用 |
|-----|------|--------|
| `OEE_StartLot(bool bSilent, AnsiString *pasErrorMsg)` | [ProductionInfo.cpp#L229](../../../HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422/ProductionInfo/ProductionInfo.cpp#L229) | SetMOInformation + ed_PIOEEMO->Text 寫入 + 雲端 MO/SetupFile 下載 + `_bOEEStartLotSuccess=true` + `_dtOEE_StartLotTime=Now()` + `bNeedLotEndAfterCleanOut=false` |
| `SetLotID(AnsiString, bool bReadFromFile)` | [uLotInfo.cpp#L1352](../../../HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422/uLotInfo.cpp#L1352) | RunInfo.LotNo 寫入 + config.ini |
| `SetLotStart(AnsiString sFunc)` | [uLotInfo.cpp#L1412](../../../HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422/uLotInfo.cpp#L1412) | EventLog + Start Time + ReadWriteLotInfo + SECS DoLotStart + 內部 if-else 走 SetLotComponents |
| `SetLotComponents(bool bLotEnd)` | [uLotInfo.cpp#L2086](../../../HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422/uLotInfo.cpp#L2086) | RunInfo.bLotStart = !bLotEnd + 20+ UI 欄位 ->Enabled = bLotEnd |
| `SetFirstTrayCheckOnUnloader()` | uLotInfo.cpp | First Tray Check 狀態 |
| `CheckCanChangeRealDummy()` | fMain | 機台執行中或有殘 IC 時回 false |
| `palFTClick / palRTClick` | fMain | RunMode 切 FT/RT |

**Cross-reference**：`OEE_StartLot` 內部會 reset `bNeedLotEndAfterCleanOut=false`（見 §1.4.4），意味著 Agent「上一批 CleanOut 未 End Lot → 直接送下一批 LotInfo_REQUEST」可以過 `CheckOEE_WhenStart`，但會跳過上一批的 OEE_EndLot 報表（`UpdateTrayCntReport` / `SaveProductionSummaryReport` / `SaveProductionDataToPath` / `InitialOEECount` 都不會跳）— §10 風險表已標記。

### 6.6 不在本 SPEC 範圍：遠端 End Lot

依使用者 2026-05-15 第二次決策（option A），本 SPEC **不新增** `LotEnd_REQUEST` / `END_REQUEST` 等遠端 End Lot 指令。`automation.cpp` grep 全文已確認無對應 handler。End Lot 路徑維持：

1. 現場手動按 `sbSECSLotEnd`（[uLotInfo.cpp#L1955](../../../HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422/uLotInfo.cpp#L1955) 是唯一呼叫 `OEE_EndLot` 的地方）
2. CleanOut 完成設 `bNeedLotEndAfterCleanOut=true`（見 §1.4.4）

如客戶後續要求遠端 End Lot，另開 **F899-009**。

## 7. 對其他客戶與既有路徑的影響評估

| 客戶 / 路徑 | 影響 |
|------------|------|
| 超豐（Greatek）OLP 新流程 | ✅ `LotInfo_REQUEST` 成功即完成 OEE Start Lot，客戶不需新增指令 |
| 任何客戶手動按 Lot Info Start Lot | ✅ **零影響**：`sbSECSLotStartClick` 整段不修改 |
| SECS S2F41 RCMD `START` | ✅ **零影響**：仍呼叫 `sbSECSLotStartClick(fLotInfo)`，函式內容不變 |
| 非超豐客戶送 `LotInfo_REQUEST` | ✅ **零影響**：仍走舊 `SetLotInfo(Data)` + 1 欄 `LotInfo_REPLY` |
| 超豐送 `LotInfo_REQUEST` 且 OEE 開啟 | ✅ 新行為：回 `LotInfo_REPLY` 2 欄（Reply Code + 英文 Detail Message）|
| 任何客戶送 `START_REQUEST` | ✅ **零影響** |
| `OEE_StartLot()` 函式本體 | ✅ **零影響**（2026-05-04 既有 silent 介面足夠）|
| `uLotInfo.cpp/.h` | ✅ **零修改**（見 §6.1 不重構）|

**對齊保證**：`SetLotInfoGreatekOEE` 對照 §1.2 手動順序 step 4/6/9/12/13/15 直接拼起，code review 用「§1.2 vs §6.3 對應步驟」檢查清單即可。

## 8. Reply Code 完整對應表（Alarm Mapping）

> 依 2026-05-04 Q3 答覆：「失敗就回傳對應代碼，後續要製作對照表給客戶。」
>
> **2026-05-15 範圍說明**（§6.1 不重構決定後）：超豐 `LotInfo_REQUEST` 分流不再經 `sbSECSLotStartClick`，因此並非所有原本 modal 對應碼都會在 Greatek 路徑被觸發。下表「Greatek 觸發」欄標 ✅ 表示 `SetLotInfoGreatekOEE` 會回此碼；標 ❌ 表示該碼僅來自 `sbSECSLotStartClick` 其他客戶分支（保留供文件一致性與其他客戶未來複用）。Greatek 路徑不跳任何 dialog。

### 8.1 成功 / 前置失敗（0~9）

| Code | Greatek 觸發 | 條件 | Detail Message | 客戶端排查 | RD 端原始位置 |
|------|:-:|---------|----------------|-----------|---------------|
| 0 | ✅ | Start Lot 成功 | （空）| — | — |
| 1 | — | 保留 | — | — | — |
| 2 | ✅ | `CheckCanChangeRealDummy()` false | `Machine running or has residual IC` | Clean Out + 確認 HALT | `auto9045.cpp` `SetLotInfoGreatekOEE` |
| 3 | — | OEE 系統未啟用（automation.cpp 入口已過濾，理論不出現）| `OEE function disabled` | 確認 ini `[OEE]` `bN14_1_EnableOEEFunction=1` | `automation.cpp` LotInfo_REQUEST 分流 |

### 8.2 必填欄位缺漏（20~29）

> Greatek 分支對 LotID/OPID 完全不檢查（uLotInfo.cpp L7749 空白 block），所以 20/21/22/23/25/26 在 Greatek 路徑都不會觸發。表格保留供其他客戶未來複用 / 文件一致性。

| Code | Greatek 觸發 | 條件 | Detail Message | 客戶端排查 | RD 端原始位置 |
|------|:-:|---------|----------------|-----------|---------------|
| 20 | ❌ | LotID 空 | `Please Enter LotID` | 補 `LotInfo_REQUEST` Data[0] | uLotInfo.cpp L7901 / L7930 |
| 21 | ❌ | LotID + OPID 任一空 | `Please Enter LotID and Operator ID` | 補 `LotInfo_REQUEST` 兩欄 | uLotInfo.cpp L7663 / L7739 / L7771 / L7808 / L7829 / L7918 / L7944 |
| 22 | ❌ | Cust. LotID 空 | `Please Enter Cust. Lot ID` | 補對應欄位 | uLotInfo.cpp L7911 |
| 23 | ❌ | Tray ID 空 | `Please Enter Tray ID` | 補 Tray ID（部分客戶模式才需）| uLotInfo.cpp L7815 |
| 24 | ❌ | Run Mode 未選 | `Please select run mode (FT-RT)` | （Greatek 路徑不檢查 RunMode；2026-05-15 v3 移除）| 其他客戶手動路徑 |
| 25 | ❌ | Line/Process/Product 任一空 | `Please Enter Line and Process and Product Name` | 補對應欄位（CMS Mode 才需）| uLotInfo.cpp L7718 / L7728 / L7762 |
| 26 | ❌ | OP ID 長度 < 4 | `OP ID Length less than 4` | OP ID 至少 4 字 | uLotInfo.cpp L7786 |

### 8.3 字元檢查（30~39）

> Greatek `SetLotInfoGreatekOEE` 只檢查 LotID / OPID（30/31）。其他欄位 Greatek Agent 不送，不需檢查。33-36 保留供其他客戶複用。

| Code | Greatek 觸發 | 條件 | Detail Message | 客戶端排查 | RD 端原始位置 |
|------|:-:|---------|----------------|-----------|---------------|
| 30 | ❌ | LotID 含 `\\/:*?"<>\|` | `Lot ID contains illegal character` | （Greatek 不檢查；2026-05-15 v3 移除）| 其他客戶手動路徑 |
| 31 | ❌ | OperatorID 含特殊字元 | `OP ID contains illegal character` | 同上 | 其他客戶手動路徑 |
| 32 | ❌ | RunMode 含特殊字元 | `Run Mode contains illegal character` | 同上 | uLotInfo.cpp L7638 |
| 33 | ❌ | Page 含特殊字元 | `Page contains illegal character` | 同上 | uLotInfo.cpp L7676 |
| 34 | ❌ | Line ID 含特殊字元 | `Line ID contains illegal character` | 同上 | uLotInfo.cpp L7687 |
| 35 | ❌ | Process Name 含特殊字元 | `Process Name contains illegal character` | 同上 | uLotInfo.cpp L7698 |
| 36 | ❌ | Product Name 含特殊字元 | `Product Name contains illegal character` | 同上 | uLotInfo.cpp L7709 |

### 8.4 Recipe / 設定檔（40~59）

> Greatek 不走 sbSECSLotStartClick 的 Recipe 段，但 OEE_StartLot 內部 SetupFile 下載失敗會以 90 + errMsg 字串回傳。50/51 在 Greatek 路徑由 OEE_StartLot 內部處理（包進 90/61）。40/41 保留供其他客戶。

| Code | Greatek 觸發 | 條件 | Detail Message | 客戶端排查 | RD 端原始位置 |
|------|:-:|---------|----------------|-----------|---------------|
| 40 | ❌ | Recipe 自 FTP 下載失敗（WAR1684）| `Recipe download fail` | 確認 FTP server / 網路 | uLotInfo.cpp L7177 / L7511 / L7523 / L7532 |
| 41 | ❌ | 2DID sorting list 找不到 | `2DID sorting list missing` | 確認 sorting list 已上傳 | uLotInfo.cpp L7347 / L7392 / L7592 |
| 50 | ❌ | Setup File name = NULL | `Setup File name is NULL, check TestInfo.txt` | 確認 TestInfo.txt | uLotInfo.cpp L7867 |
| 51 | ❌ | Setup File 不存在 | `Setup File not exist, check TestInfo.txt` | 確認檔案實際存在 | uLotInfo.cpp L7877 |

### 8.5 環境 / 連線 / 雲端下載（60~69）

> 雲端下載相關錯誤獨立成 60 段，方便 Agent 端針對「網路/MO 抓不到」做不同重試策略（與「機台狀態問題」區隔）。Greatek 路徑下 60 由 §6.3 預檢查直擋（對齊 §1.2 step 9.1），61/62 由 §6.3 在 `OEE_StartLot` 失敗後依 errMsg 關鍵字（`Machine ID` / `MO`）細分。
>
> **2026-05-15 對盤結論**：實際逐條檢視 OEE_StartLot 6 條 errMsg 字串後，無任何一條含 `FTP`/`upload`，故 **63 為保留碼**（Greatek 不觸發）；`download`/`cloud` 同理也不出現，全部失敗訊息靠 `MO` 子字串兜底分到 61，唯一例外是 `MO or Machine ID is empty` 由 62 先攔截。

| Code | Greatek 觸發 | 條件 | Detail Message | 客戶端排查 | RD 端原始位置 |
|------|:-:|---------|----------------|-----------|---------------|
| 60 | ✅ | Monitor 未連線（`IniConfig.bC11UseMonitorView && fMonitor->MVCtrl->IsConnect()==false`，§6.3 預檢查直擋；對齊手動 sbSECSLotStartClick L8023）| `Please Check Monitor Connect!!!` | 檢查 Monitor 軟體 | `auto9045.cpp` `SetLotInfoGreatekOEE`（對齊 uLotInfo.cpp L8023）|
| 61 | ✅ | OEE FTP 抓 MO 失敗 / MO 不存在（OEE_StartLot errMsg 含 `MO`，且非 `Machine ID`）| `OEE MO download fail (cloud unreachable or MO not found)` | 確認 OEE FTP / MO server / Machine ID 對應 MO 是否上傳 | ProductionInfo.cpp `LoadMOInformation` |
| 62 | ✅ | Machine ID 為空（errMsg 含 `Machine ID`）| `Machine ID is empty` | 確認機台 OEE 設定 Machine ID 已填 | ProductionInfo.cpp L2421 / L2694 |
| 63 | ❌ | （保留）OEE FTP Upload 設定關閉 | `OEE FTP upload disabled` | — | OEE_StartLot 現未產生此 errMsg；保留供未來擴充 |

### 8.6 OEE 確認框（70~79）

| Code | Greatek 觸發 | 條件 | OLP 處理方式 |
|------|:-:|---------|--------------|
| — | n/a | 「Are You Sure Start Lot?」確認框 | Greatek 路徑不經 sbSECSLotStartClick，**沒有此 prompt**（直接呼叫 OEE_StartLot(true)）|

### 8.7 內部失敗 / 通用（90~99）

| Code | Greatek 觸發 | 條件 | Detail Message | 客戶端排查 | RD 端原始位置 |
|------|:-:|---------|----------------|-----------|---------------|
| 90 | ✅ | 其他 OEE_StartLot 內部錯（errMsg 不含 60-63 關鍵字）| 帶實際 errMsg 字串 | 將 Data[1] 提供給 RD | `auto9045.cpp` `SetLotInfoGreatekOEE` fallback |
| 91 | ❌ | Start Lot 完成但需手動按 RUN（手動專屬 prompt）| `Please pressed Start, RUN` | OLP 後續送 `START_REQUEST` 即可 | uLotInfo.cpp L8066 |

### 8.8 客戶端建議處理流程

> 2026-05-15 v3 後，Greatek 路徑實際可能出現的 Reply Code 只有：**0 / 2 / 60 / 61 / 62 / 90**。24/30/31/63 為保留碼或其他客戶用，Greatek Agent 不需處理。

```
rc = LotInfo_REPLY.Data[0]
if rc == 0:                       # 成功，繼續送 START_REQUEST
elif rc == 2:                     # 機台執行中或有殘 IC → Clean Out / HALT 後重試
elif rc == 60:                    # Monitor 未連線 → 通知現場啟動 Monitor 後重試
elif rc == 61:                    # OEE MO 下載失敗 → 確認雲端 / FTP 後重試
elif rc == 62:                    # Machine ID 為空 → 通報 RD / FAE 補設定
else:                              # rc==90 或其他 → 看 Data[1] 英文 detail 上報 RD
```

## 9. 驗證方法

### 9.1 不變性驗證（最重要）
- `version_diff.py` 確認 `uLotInfo.cpp` / `uLotInfo.h` / `ProductionInfo.cpp` / `ProductionInfo.h` 對 V899.x 完全 byte-identical（不修改）
- 手動按 Lot Info Start Lot（任何客戶）→ 行為與舊版相同（含 modal 仍跳）

### 9.2 OLP 新功能
- mock 送超豐 `LotInfo_REQUEST` 各 Data 組合 → 確認回 **0 / 2 / 60 / 61 / 62 / 90** 正確（見 §8.8）；24/30/31/63 為保留碼，Greatek v3 路徑不應出現（即使 Agent 送非法字元也應被忽略而非回 30/31）
- 確認失敗時 `LotInfo_REPLY` Data[1] 帶英文 errMsg 字串
- 非超豐 `LotInfo_REQUEST` 仍只回 1 欄，且回碼與 V899.8 一致

### 9.3 整合
超豐現場 patch 後跑 Auto Start 序列：
```
LotInfo_REQUEST → 0  （同時完成 OEE Start Lot）
START_REQUEST   → 0  (< 1s 內回)
```

### 9.3.1 Button Lock / End Lot 解鎖驗證（2026-05-15 新增）

驗證表（各 case 必需見到 `RunInfo.bLotStart=true` 且 UI 欄位鎖定）：

| Case | Agent 送的 Data[0..2] | 期望結果 |
|------|------------------------|----------|
| C1 | `LotID=ABC, OPID=Op1, FT-RT=0`（v3 後內容被忽略） | LotInfo_REPLY=0、`edtSysLotID->Enabled=false`、`sbSECSLotStart->Down=true`、`RunInfo.bLotStart=true`；`edtSysLotID->Text` 不得被覆寫成 `ABC`、cbRunMode 不得被切換 |
| C2 | `LotID="", OPID="", FT-RT=""`（標準 Greatek 呼叫） | LotInfo_REPLY=0、鎖定狀態同 C1（驗證 §6.3 強制 SetLotComponents(false) 生效）；LotID 由 OEE 雲端 MO 帶入 |
| C3 | （已移除，原 RunMode 24 case 不再適用） | — |
| C4 | C2 成功後手動按 `sbSECSLotEnd` | UI 解鎖 、`RunInfo.bLotStart=false`、`edtSysLotID` 被清空 |
| C5 | C2 成功後連續送第二個 `LotInfo_REQUEST`（未 End Lot） | 期望被 `CheckCanChangeRealDummy` 擋下回 `replyCode=2`（RunInfo.bLotStart=true 走不過）|

**關鍵驗證點**：C2 與 C1 的 lock 狀態不得有差別（這是使用者需求三的核心）。

**End Lot 驗證限制**：本 SPEC 不新增遠端 End Lot，End Lot 仍必須手動按鈕或原有 CLEANOUT 間接路徑（見 §1.4.4）。需補充到客戶文件。

### 9.4 回歸（重要）
- 各客戶手動 Lot Start：PTI / KYEC / Sigurd / TSMC / Murata 至少抽 2 個
- SECS S2F41 RCMD `START`（若有 SECS 客戶在跑）
- 各客戶 OLP `LotInfo_REQUEST` 跑既有 Agent，確認回應與 V899.8 一致

## 10. 風險與相依

| 風險 | 影響 | 緩解 |
|------|------|------|
| ~~重構薄包裝邏輯不等價~~ **（2026-05-15 §6.1 取消重構，風險消除）** | — | 不重構 sbSECSLotStartClick |
| ~~`DoSECSLotStartCore` 內 modal 改寫遺漏~~ **（不適用，不重構）** | — | — |
| 超豐 Agent 原本只解析 `LotInfo_REPLY` 1 欄 | 若失敗時回 2 欄，Agent parser 需確認可接受 | 成功仍可只看 Data[0]；失敗 Data[1] 為英文 detail，必要時先做 parser 相容測試 |
| Data[0..2] 欄位定義錯 | 客戶 Agent 送錯 | 沿用既有 `LotInfo_REQUEST` 參數，不新增欄位 |
| 超豐手動 patch 後測試人員仍按介面 Start Lot | 不影響 OLP 流程 | 文件說明：兩條路徑可並存 |
| **順序顛倒**：先 SetLotID/SetLotStart 再 OEE_StartLot | RunInfo / EventLog / SECS Event 被記錄在 OEE 設定（SetupFile/TrayForm/_sOEE_MO）生效之前；OEE 失敗時 RunInfo.bLotStart 已 = true 無法乾淨 abort | §6.4 鐵則 + code review 必看順序 + 整合測試驗證 OEE 失敗時必須不進 SetLotStart |
| **雲端下載失敗訊息粗略** | Agent 端無法分辨網路問題 vs MO 缺檔 vs Machine ID 設定錯 | §8.5 / §6.3 依 errMsg 關鍵字細分 60/61/62/63 Reply Code |
| Agent 仍送舊欄位（LotID/OPID/RunMode）期望被機台採用 | v3 後 Handler 完全忽略三欄內容（不寫 UI、不切 RunMode、不檢查字元）；舊版 Agent 仍可呼叫但內容無效 | 客戶交付文件明記三欄「不使用，由雲端 MO file 帶入」；2026-05-15 變更標註 |
| Agent 期望 RunMode 由 Data[2] 切換 | v3 後 RunMode 維持 UI 當下值；若現場操作員未事先選對 FT/RT，會以前次值繼續測試 | 客戶 SOP 補充「Lot Start 前現場需先確認 FT/RT 切換鍵狀態」；可考慮未來另開 SPEC 加 RunMode_REQUEST |
| **遠端不強制 SetLotComponents(false)** | LotID 為空時 SetLotStart 內部走 SetLotComponents(true) → UI 未鎖、`RunInfo.bLotStart=false`、後續 `START_REQUEST` 被 `CheckOEE_WhenStart` 加鎖 | §6.3 在 SetLotStart 後必呼叫 `fLotInfo->SetLotComponents(false)`；§9.3.1 C2 驗證必通過 |
| End Lot 遠端路徑不存在 | Agent 無法遠端結束 Lot，需現場人工介入才能解鎖 | §1.4.4 明記判隷路徑；客戶文件需補說明 End Lot 該走手動或 CLEANOUT；未來如需另開 F899-009 |
| **連續多批未 End Lot** | OEE_StartLot 會 reset `bNeedLotEndAfterCleanOut=false`，上一批 CleanOut 後送 LotInfo_REQUEST 依舊能過；但上一批 OEE 報表未推進（SaveProductionSummaryReport / UpdateTrayCntReport / SaveProductionDataToPath / InitialOEECount 都不會跳） | 客戶交付文件明記 Agent 該在 START_REQUEST 回成功、但下一批上送前需安排 End Lot（手動或 CLEANOUT 介入） |

## 11. Release Note 草稿

### 11.1 customer
超豐 `LotInfo_REQUEST` 內嵌 OEE Start Lot：客戶端仍維持 `LotInfo_REQUEST` → `START_REQUEST` 既有順序，不需新增指令。失敗時 `LotInfo_REPLY` 回傳明確 Reply Code 與英文原因。

### 11.2 distributor
超豐專用 `LotInfo_REQUEST` 分流新增 silent OEE Start Lot。**不重構 `sbSECSLotStartClick`**（見 §6.1），手動 / SECS / 非超豐 OLP 路徑零變動。超豐分流直接使用現成 `OEE_StartLot(bSilent, errMsg)` API + 手動 §1.2 順序的 6 步。

### 11.3 internal
- `automation.cpp` 巨型分派器在 `LotInfo_REQUEST` 分支內增加 `CUSTOMER_CODE==CC_Greatek && CosFunction.bOEEFunction` 分流
- `auto9045.cpp/.h` 新增 `SetLotInfoGreatekOEE(AnsiString *Data, AnsiString &errMsg)`：
    1. 預檢查（CheckCanChangeRealDummy / RunMode 必填）
    2. 寫 UI 欄位（LotID/OPID 允許空） + RunMode click
    3. 字元檢查（對齊 §1.2 step 4 但遠端採 early return）
    4. **`fProductionInfo->OEE_StartLot(true, &errMsg)`**（現成 silent API）
    5. `fLotInfo->SetLotID()` + `fLotInfo->SetLotStart()`
    6. **`fLotInfo->SetLotComponents(false)`**（補空 LotID 不鎖缺口，見 §1.4 / §6.3）
    7. `fLotInfo->SetFirstTrayCheckOnUnloader()`
- **不動**：`uLotInfo.cpp/.h` 完全不修改（取消原計畫 `DoSECSLotStartCore` 薄包裝）
- **不動**：`OEE_StartLot()`（2026-05-04 既有 bSilent+pasErrorMsg 介面已足夠）、`OEE_EndLot()`、`SetLotID()`、`SetLotStart()`、`SetLotComponents()`、SECS S2F41 處理

## 12. 客戶答覆紀要（2026-05-04）

| 問題 | 客戶答覆 | SPEC 落點 |
|------|---------|----------|
| Q1：LotInfo_REQUEST 是否要動？| `LotInfo_REQUEST` 參數沿用之前提供的 3 欄位；可在超豐分流內結合 OEE Start Lot | §5.1 / §6.2 |
| Q2：欄位是否帶值？| `LotInfo_REQUEST` 仍帶既有 Data[0..2]；MO/Flow/Ticket/Process/Product/TestTimes 等未來透過其他指令填寫，不新增到 `LotInfo_REQUEST` | §5.1 / §12.2 |
| Q3：失敗時要回什麼？| **失敗回對應代碼**，後續製作對照表給客戶（檔名 `RD5軟體_CC_Greatek_遠端控制操作說明書_YYYYMMDD_HHMMSS`）| §8 對應表 / §13 客戶文件 |
| Q4：兩指令間需要間隔嗎？| **客戶端網路速度慢。** `LotInfo_REQUEST` → `START_REQUEST` 建議 ≥ 500ms（見下說明） | §12.1 |
| Q5：是否接受新增 `OEEStartLot_REQUEST`？| **不接受。** 客戶希望依照之前提供的順序 | §2 / §3 / §6 |
| Q6：Detail Message 是否需中英對照？| **一律英文說明** | §12.3 |

### 12.0 客戶二次答覆（2026-05-15）

| 問題 | 客戶答覆 | SPEC 落點 |
|------|---------|----------|
| Q7：`LotInfo_REQUEST` 的 LotID/OPID 是否必填？| **超豐完全不送**。Handler 忽略 Data[0..1]，由 OEE 雲端 MO file 帶入。| §1.2 / §5.1 / §6.3 v3 |
| Q8：RunMode（FT/RT）由誰帶？| **不由 Agent 帶**（2026-05-15 客戶確認）。Handler 忽略 Data[2]，RunMode 由現場操作員預先在 UI 選定。| §5.1 / §6.3 v3（已移除 24 檢查）|
| Q9：實作前要先確認什麼？| 必須先確認**手動 `sbSECSLotStartClick` 的呼叫順序**，這關係到 LotID / EventLog / SECS Event 的資料判斷。| §1.2 權威呼叫順序 / §6.4 順序鐵則 |

### 12.1 Q4 網速慢之 RD 建議

客戶未提供具體 RTT 數值，依「網路慢」初估：

| 場景 | 建議間隔 | 理由 |
|------|---------|------|
| `LotInfo_REQUEST` → `START_REQUEST` | **≥ 500 ms** | `LotInfo_REQUEST` 內嵌 OEE Start Lot，可能含 Recipe / MO / Setup File 下載 |
| TCP keep-alive timeout | **≥ 30 s** | 避免慢網路下被中間設備斷線 |
| `LotInfo_REPLY` 等待 timeout | **≥ 10 s** | Recipe / MO / Setup File 下載最慢場景觀察值約 7s |

> 若客戶現場驗證時這些值仍不夠，請回報具體 RTT，再調整建議。

### 12.2 `LotInfo_REQUEST` Data[0..2] 詳細規則

#### 規則 1：`LotInfo_REQUEST` 仍需帶既有 3 欄位

```
LotInfo_REQUEST,LotID,OperatorID,FT-RT
```

- Data[0] = LotID
- Data[1] = OperatorID
- Data[2] = FT-RT（既有程式判斷：`0` = FT，非 `0` = RT）

#### 規則 2：`LotInfo_REQUEST` 成功的意義改變（僅超豐 OEE 分流）

超豐 OEE 開啟時：

```
LotInfo_REPLY,0
```

代表兩件事都成功：
1. Lot Info 表單資料寫入成功
2. OEE Start Lot 成功

非超豐客戶仍只代表舊版 `SetLotInfo(Data)` 成功。

#### 規則 3：MO/Flow/Ticket/Process/Product/TestTimes 等欄位為何不在 Data 中

這些欄位本來就不是 `LotInfo_REQUEST` 的一部分。超豐說明「任何資料都透過其他指令填寫」，意思是：

- MO 由其他專屬 MO 設定指令或 ini 設定
- Flow / Ticket / Process / Product 同上
- TestTimes / CusStep 同上

所以這些欄位**不新增到** `LotInfo_REQUEST` 參數，永遠「以機台現讀到的值為準」。若需要透過 OLP 設這些欄位，應使用對應的專屬指令。

### 12.3 Detail Message 語系規則

> **客戶二追問答覆（Q5）**：「一律英文說明。」

- `LotInfo_REPLY` Data[1] 的 Detail Message **一律為英文**
- 不提供中英對照版本
- 與現有其他 OLP REPLY 語系一致（如 `LotInfo_REPLY` / `START_REPLY`）
- 若未來客戶需要本地化 UI 顯示，由客戶 Agent 端依 Reply Code 自行查本地化表，機台不負責翻譯

此規則同步反映到§8 的 Detail Message 欄與客戶交付文件。

## 13. 客戶交付文件

| 文件名稱 | 路徑 | 用途 |
|---------|------|------|
| `RD5軟體_CC_Greatek_遠端控制操作說明書_YYYYMMDD_HHMMSS.md` | `docs/customer_docs/Greatek/` | 含本 SPEC §5、§8 對應表，給超豐 Agent 工程師使用 |

> 依使用者規範：客戶說明書檔名格式 = `RD5軟體_CC_<客戶代號>_<文件主題>_YYYYMMDD_HHMMSS`

## 14. 關聯

- 模組 SPEC：[../modules/automation-olp.spec.md](../modules/automation-olp.spec.md)
- repo memory：`chipmos-v8998-lotinfo-not-startlot.md`
- F899-009：**保留**給未來「遠端 End Lot」需求（見 §6.6）；本 SPEC 範圍不含 End Lot
- SECS S2F41 RCMD `START` 同 bug：列為 [automation-olp.spec.md §9 Known Issues](../modules/automation-olp.spec.md)，本案不修
