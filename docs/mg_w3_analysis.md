# MG-W3 分析：EventLog 引號相依鏈（0429 寫入端 + 0817 解析端）

- **波次**：MG-W3（V899 → V910），LEDGER 相依鏈 #1
- **來源**：`docs/mg_ai_matrix_missing.csv` **7 條 MISSING**
  - `note.cpp:6689`、`note.cpp:6702`（20260429，CASE-20260429-001）
  - `cObserver.cpp:3790 / 3890 / 3934 / 3969 / 5177`（20260817，CASE-FOREHOPE_NINGBO-20260813-001）
- **V899 樹**：`D:\HT9045\HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422`（唯讀，端點 `b515ed5`，兩檔對 HEAD 無異動）
- **V910 樹**：`D:\HT9045\HT9011UC_Code_V3.33.910.0_20260716_Jimmy`（HEAD `0bee467` = MG-W2 落地後；兩檔對 HEAD 無異動、無 `.mgbak`）
- **本文件性質**：分析＋splice op 草案。**分析階段全程唯讀，未修改任何原始碼。**
- **op 檔**：`D:\HT9045\docs\mg_w3_ops.json`（6 個 op）

---

## 0. 前置事實（全部實測，非推論）

### 0.1 檔案基本盤

| 檔 | V899 行數 | V910 行數 | EOL | 套用後預測行數 |
|---|---|---|---|---|
| `note.cpp` | 6818 | 6862 | 兩樹**全 CRLF**（6818/6818、6862/6862） | **6875** |
| `cObserver.cpp` | 5473 | 5425 | 兩樹**全 CRLF**（5473/5473、5425/5425） | **5480** |

V910 `cObserver.cpp` 目前 `//AI(` 出現次數 **0**；V910 `note.cpp` 有 3 條，全部是別的日期/別的 agent
（5328 `ht9045-config` 20260507、5767 / 6071 `ht9045-staterecord-analysis` 20260519），**與本波 7 條無關**。
→ 7 條 MISSING 判定成立，無「已搬一半」狀態。

### 0.2 V910 現狀：兩端各自有沒有被公司改過？

用 cp950 行級 diff 對全檔逐 hunk 檢查（`.svn` 排除）：

**`note.cpp` — `SaveErrEventLog()` 全函式（V899 6665–6768 / V910 6722–6812，103 行）**
只有 **1 個 hunk** 落在函式內：

```
V899[6688..6702]  ->  V910[6745..6746]
```

即 V899 多出整段 `else if(CUSTOMER_CODE==CC_FOREHOPE_NINGBO){...}`（13 行）＋ 一行 AI 註解。
函式其餘部分**位元組相等**，唯二例外都是公司的註解欄位對齊（非本波製造，也不在取代區內）：

| 位置 | 差異 |
|---|---|
| V899 6701 / V910 6745 | `else` 後的 `//if(CUSTOMER_CODE==...)` 註解欄位（V899 緊接 / V910 推到第 ~81 欄） |
| V899 6764 / V910 6808 | `MySaveFileByFileNameAndType(...)` 行尾註解欄位 |

→ **公司從未在此函式做過反向的寫檔格式修改。** V910 目前就是 Steven 20251107 的引號版原狀。

**`cObserver.cpp` — 全檔 diff 共 25 個 hunk**，其中與本波有關的只有 5 個
（`delete V899[3790..3844]` 一整塊 ＋ 四處 `ParseEventLogLine` 呼叫），其餘全是公司漂移／空白行：

| V910 公司漂移 | 位置 | 是否落在本波取代區 |
|---|---|---|
| KYEC 比數 10 → 21（Eastsun 20260526） | 869 / 877 | 否 |
| `sTestIndexZTime==""` → `="";`（**V910 修好 V899 的筆誤**） | 2897 | 否 —— **本波不得覆蓋** |
| `IsNNMode()==NN_1Row` iRow 計算（Ifor 20260401） | 3381–3394 | 否 |
| 11 處空白行增減 | 580/744/1813/… | 否 |

→ **V910 沒有 `ParseEventLogLine`，也沒有任何等價改寫**：四個呼叫點在 V910 全部仍是原始的
`tsRow->CommaText=tsLogFile->Strings[i];`（實測全樹 grep：`ParseEventLogLine` 在 V910 **0 命中**）。

### 0.3 位元組級證明：V910 的 5 個取代區段 == 變更前原狀

| op | V899 行段 | V910 行段 | 上下文位元組相等 |
|---|---|---|---|
| OP-1 | 6665–6687 / 6703–6713（函式頭 + else 分支主體） | 6722–6744 / 6747–6757 | **True** |
| OP-3 | 3886–3889 | 3844–3847 | **True** |
| OP-4 | 3928–3933 | 3886–3891 | **True** |
| OP-5 | 3970 | 3928 | **True** |
| OP-6 | 5172–5176 | 5126–5130 | **True** |

→ 每個 op 的「被取代區段中不該變的那幾行」在兩樹**位元組完全相同**，公司未反向修改。

### 0.4 `ParseEventLogLine` 完整範圍與呼叫點稽核（全樹窮舉）

**V899 定義範圍**（三支 **`static`** 檔案內函式，都是 20260817 新增，位於 `//---` 分隔線之間）：

| V899 行段 | 內容 |
|---|---|
| 3790–3799 | 10 行說明註解（矩陣簽章行 = 3790） |
| 3801–3812 | `static void AddEventLogCsvField(TStringList*, AnsiString)` — Trim、去頭尾引號、`""`→`"` |
| 3814–3835 | `static void SplitEventLogCsvLine(const AnsiString&, TStringList*)` — 只以**未在引號內的逗號**斷欄 |
| 3837–3843 | `static void ParseEventLogLine(const AnsiString&, TStringList*)` — `bSPILFunction` 走舊 `CommaText`，其餘走新斷欄 |
| 3800 / 3813 / 3836 / 3844 | 四條 `//---` 分隔線 |

**呼叫點：全樹 grep `ParseEventLogLine`（排除 `.svn`）＝ 5 命中，其中 4 個是呼叫點，全在 `cObserver.cpp`：**

| V899 行 | 所在函式 | V910 對應行 |
|---|---|---|
| 3890 | `TfObserver::GetEventLogText()` — `cbbFilter->ItemIndex==0` 全部顯示分支 | 3848 |
| 3934 | `TfObserver::GetEventLogText()` — JAM/WAR/MES 過濾分支 | 3892 |
| 3969 | `TfObserver::GetEventLogText()` — 其餘過濾分支 | 3927 |
| 5177 | `TfObserver::StatisticalJamCount(bool)` | 5131 |

> **矩陣只列了 cObserver 的 4 條（3890/3934/3969/5177）＋1 條註解（3790），實測與矩陣一致，無第五個呼叫點。**

**標頭宣告：不需要。** 三支都是 `static`（TU-local）。
`cObserver.h` 在**兩棵樹**對這三個名字皆 **0 命中**（且 `cObserver.h` 兩樹只差 1 行尾隨空白，見 §0.6）。
V910 全樹對 `AddEventLogCsvField` / `SplitEventLogCsvLine` / `ParseEventLogLine` 亦 **0 命中** → 無命名衝突。

### 0.5 刻意未改的兩個同型呼叫點（重要，避免誤判成漏搬）

全檔 `tsRow->CommaText=tsLogFile->Strings[i];` 在 V910 有 **5 個**命中，本波只改 4 個：

| V910 行 | 所在函式 | 讀的檔 | 本波 |
|---|---|---|---|
| 3848 / 3892 / 3927 | `GetEventLogText()` | EventLog CSV | **改** |
| 5131 | `StatisticalJamCount()` | EventLog CSV（同一份，`slEventLog->Path/FileName` 組出） | **改** |
| **4822** | **`GetTimeDataText()`** | **Time Data log（不同檔）** | **不改**（V899 同樣未改，兩樹該行位元組相同） |

另有全樹**第五個 EventLog 解析點**，V899 也未修：

- `Mes/fVATMesFileSys.cpp:2357` `slLog->CommaText=slEvent->Strings[j];`
  （位於 `TfMesSystem::RecordByTimeOEE`，讀 `RunInfo.slEventLogFile`，用 `Strings[0..7]` 位置式取欄）
- 兩樹該檔在此位置**位元組相同**（diff 無 hunk），故不是 V910 漂移。
- 閘門：唯一呼叫者 `main.cpp:31775`，包在 `if(IniConfig.bVTESTFunction==true)` 內（VTest MES 客戶專屬）。
- **判定**：這是 **V899 自身的殘留缺口**，非本波搬移落差。忠實搬運不代為「修得更好」；
  若要一併修屬 V899 議題，應回 `ht9045-v899` 決定。已列入 §4 給 主腦。

### 0.6 跨檔依賴與符號可用性稽核

| 符號 | V910 出處 | 比對 |
|---|---|---|
| `CC_FOREHOPE_NINGBO` | `MachineType.h:157` `#define ... 790` | 與 V899（`MachineType.h:195`）**同值 790** |
| `CUSTOMER_CODE` | `cmydef.cpp:3404` `int CUSTOMER_CODE=0;` / `cmydef.h:3205` extern | **執行期全域 int**，非編譯期 `#define`（詳見 §2） |
| `IniConfig.bSPILFunction` | `Config.h:131` | 兩樹同宣告 |
| `StringReplace` / `TReplaceFlags` / `rfReplaceAll` | `<SysUtils.hpp>`（`MachineDefine.h:12`） | V910 `cObserver.cpp` **同檔 2226–2227 行已在用**，可見性已由現行程式碼背書 |
| `AnsiString::Trim / SubString / Length` | VCL | — |
| `TStringList` / `slEventLog` / `strngrdEventLog` / `strngrdJamLog` | 已在 V910 同檔使用中 | — |

- include 鏈：`cObserver.cpp` 的 33 個 `#include` 在兩樹**清單完全相同**（逐項比對通過）。
- `cObserver.h` 兩樹 567 行，唯一差異是 V899 第 345 行 `TButton *btnLot5;` 有尾隨空白 —— **本波不動 `.h`**。

### 0.7 事前模擬（記憶體內套用 `mg_w3_ops.json` 全部 6 個 op，未落盤）

```
ok MG-W3-OP1 (+15/-2)   note.cpp       6862 -> 6875 | cp950 errors=0 | CRLF=6875 bareLF=0
ok MG-W3-OP6 (+6/-6)
ok MG-W3-OP5 (+2/-2)
ok MG-W3-OP4 (+7/-7)
ok MG-W3-OP3 (+5/-5)
ok MG-W3-OP2 (+55/-0)   cObserver.cpp  5425 -> 5480 | cp950 errors=0 | CRLF=5480 bareLF=0
```

行數對帳：`cObserver.cpp` 全檔 diff 淨值 −48（含本波的 −55），扣掉本波後 = **+7**；`5473 + 7 = 5480` ✓。

**逐區塊比對模擬結果 vs V899：**

| 區塊 | 結果 |
|---|---|
| `note.cpp` `SaveErrEventLog()` 全函式（103 行） | 僅剩 **1 行**殘差 = §0.2 那條公司註解欄位對齊（V910:6821），**本波未觸及該行** |
| `cObserver.cpp` 新增解析區塊 + 到 `GetEventLogText()` 起點（56 行） | **完全相同（0 差異）** |
| OP-3 / OP-4 / OP-5 / OP-6 四個呼叫點區域 | **各自完全相同（0 差異）** |

模擬後 `ParseEventLogLine` 在 V910 的最終位置：定義 3848，呼叫 3903 / 3947 / 3982 / 5186；
`GetTimeDataText()` 的 4877 維持原樣 ✓。

---

## 1. 各變更明細

### C1 — `SaveErrEventLog()` 甬矽無引號寫檔分支（矩陣 `note.cpp:6689`、`6702`）→ **OP-1**

- **V899 範圍**：`note.cpp:6688-6702`（15 行）
  - `6688` `else if(CUSTOMER_CODE==CC_FOREHOPE_NINGBO)`
  - `6689` `{   //AI(ht9045-v899) 20260429: 甬矽要求 EventLog 時間欄不加雙引號與前綴空格 (CASE-20260429-001)`（**矩陣簽章行**）
  - `6690-6699` 十行 `Str+=","+…`（全欄不加引號、時間欄不加前綴空格）
  - `6700` `}`
  - `6701` `else //if(CUSTOMER_CODE==CC_FOREHOPE_NINGBO)  //Steven 20251107 : 修正event log存檔`
  - `6702` `{   //AI(ht9045-v899) 20260429: 其他客戶維持 Steven 20251107 新方法 (CASE-20260429-001)`（**矩陣簽章行**）
- **V910 錨點 / 被取代行段**：`note.cpp:6745-6746`
  - `6745` = `    else                                    //if(CUSTOMER_CODE==CC_FOREHOPE_NINGBO)  //Steven 20251107 : 修正event log存檔`
  - `6746` = `    {`
- **模式**：`replace`（2 行 → 15 行，淨 +13）
- **anchor_expect**：`//if(CUSTOMER_CODE==CC_FOREHOPE_NINGBO)` → 全檔命中 **[6745] 唯一**，純 ASCII
- **語意**：在既有 `if(bSPILFunction) / else` 之間插入 `else if(CUSTOMER_CODE==CC_FOREHOPE_NINGBO)`。
  兩種格式的差別只在**引號與時間欄前綴空格**，欄位數量與順序（Date, Time, JamArea, JamCode, Recovery,
  PassTime, DuplicateError, Message, ErrPart, RecipeName 共 10 欄）**完全相同**。
- **不動**：函式頭（6722–6744）、SPIL 分支、`SaveEventLog()`、檔尾 `if(CUSTOMER_CODE==CC_FOREHOPE_NINGBO) slEventLog->AddText(Str);`（V910 6788–6789，兩樹已相同）、`CC_PTI` JamAlarmLog 區塊 —— 全部保留。
- **已知副作用（純排版，需 主腦 知悉）**：取代區含 V910 的 `else` 行，套用後該行的
  `//if(CUSTOMER_CODE==...)` 註解欄位會由 V910 的第 ~81 欄還原成 V899 的緊接式。
  **無語意影響**；port_check 會列一條 `REMOVED 6745`。
  （選擇單一 op 而非拆兩個，是因為拆開後第二個 op 的錨行只有 `    {`，防呆形同虛設。）
- **風險**：控制流只多一個 `else if` 分支；非 790 客戶走的 `else` 主體**位元組不變**（§0.3 已證）。
- **信心**：**≥95%**

---

### C2 — EventLog CSV 顯示用斷欄三支新函式（矩陣 `cObserver.cpp:3790`）→ **OP-2**

- **V899 範圍**：`cObserver.cpp:3790-3844`（55 行，內容見 §0.4）
- **V910 對應**：**不存在**，需新增
- **模式**：`before`，`anchor_line: 3801`
- **anchor_expect**：`void TfObserver::GetEventLogText()` → 全檔命中 **[3801] 唯一**，純 ASCII
  （不用前一行的 `//-----` 分隔線當錨 —— 該字串全檔數百次命中，形同無防呆。）
- **語意**：
  - `SplitEventLogCsvLine` 掃描整列，維護 `bInQuote`，**只在引號外的逗號**斷欄；
  - `AddEventLogCsvField` 對每欄 Trim → 去掉起始引號 → 去掉結尾引號（未閉合者比照 `CommaText` 容忍）→ `""` 還原成 `"`；
  - `ParseEventLogLine` 是分派器：`IniConfig.bSPILFunction==true` 時**維持舊的 `tsRow->CommaText=`**（V899 註解已說明「SPIL log 欄位配置不同且手上無樣本可驗」），其餘走新斷欄。
- **為什麼需要它**：VCL `TStringList::CommaText` **把空白也當分隔符**。C1 讓甬矽的 `sJamArea`（例 `07 Tester I/F`）、
  `sMessage`、`sErrPart` 不再有引號保護 → 一列被切出多餘欄位 → 整列右移 → `Strings[3]`（AlarmCode）拿到錯的東西 →
  Filter 顯示 `No Record!!`、JAM 統計比對不到。這正是 CASE-FOREHOPE_NINGBO-20260813-001。
- **不動**：`GetEventLogText()` 本體、`//-----` 分隔線（V910:3800）與其前的 `ReadEventLogFileName` 結尾。
- **風險**：三支皆 `static`，全樹無同名符號（§0.4）；所用符號在 V910 全部可見且同語意（§0.6）。
- **信心**：**≥95%**

---

### C3 / C4 / C5 — `GetEventLogText()` 三個呼叫點（矩陣 `3890` / `3934` / `3969`）→ **OP-3 / OP-4 / OP-5**

| op | V899 範圍 | V910 被取代行段 | anchor_expect（全檔唯一命中） |
|---|---|---|---|
| OP-3 | `3886-3890`（5 行） | `3844-3848`（5 行） | `strngrdEventLog->RowCount=tsLogFile->Count;` → [3844] |
| OP-4 | `3928-3934`（7 行） | `3886-3892`（7 行） | `Str="MES";` → [3886] |
| OP-5 | `3969-3970`（2 行） | `3927-3928`（2 行） | `if(tsRow->Count>2)` → [3928] |

- **模式**：全部 `replace`，行數 **1:1 淨 0**。
- **為何取代區要放寬到含前後文**：被改的那一行本身
  `tsRow->CommaText=tsLogFile->Strings[i];` 在 V910 有 **5 個命中**（§0.5），單行無法當唯一錨點。
  放寬進來的前後文行**兩樹位元組相同**（§0.3 已證），port_check 會全部歸為 SPLICED。
- **語意**：`tsRow->CommaText=…` → `ParseEventLogLine(…, tsRow);`（V899 該行行尾自帶 AI 註解＝矩陣簽章）。
- **不動**：三處外層 `for` / `if(tsRow->Count>N)` / `iJamCol` 分派 / 寫格子的迴圈，全保留。
- **風險**：無。`tsRow` 生命週期、`Clear()` 時機、後續 `tsRow->Count` / `Strings[j]` 用法皆不變。
- **信心**：**≥95%**

---

### C6 — `StatisticalJamCount()` 呼叫點（矩陣 `cObserver.cpp:5177`）→ **OP-6**

- **V899 範圍**：`cObserver.cpp:5172-5177`（6 行）
- **V910 被取代行段**：`5126-5131`（6 行）
- **模式**：`replace`（1:1 淨 0）
- **anchor_expect**：`strngrdJamLog->RowCount=2;` → 全檔命中 **[5126] 唯一**，純 ASCII
- **語意同 C3–C5**，但 **這一處不是純顯示**，需 主腦 知悉：
  `StatisticalJamCount()`（V910 5060 起）讀的是**同一份 EventLog CSV**（`slEventLog->Path` + `FileName` 組出），
  統計結果除了填 `strngrdJamLog` 畫面，還會經 `TMyStringList("D:\HT9045_Log\EventLogTxt", …)` +
  `MySaveSGJamCountToFile()` **寫出 JamRawData CSV**，供「Auto Upload FTP JAMRawData」上傳。
  → 解析修正**會改變統計數字**（方向是「修正」：先前因欄位錯位而漏算的 JAM 列會被算進去）。
  閘門：自動路徑（`main.cpp:11212` 開機、`HS_Function.cpp:178` 跨日上傳）都包在
  `if(IniConfig.bN26_UseJamRawDataRecord)` 內；另有兩顆手動按鈕
  （`btnSG_QueryNowClick` / `btnSG_QueryYesterdayClick`）無閘門。
- **風險**：見 §2 第 3 點。
- **信心**：**≥95%**

---

## 2. 客戶碼隔離判斷

**結論：照 V899 原 gate 搬，不新增任何 `CUSTOMER_CODE` / `FUNC_CC_*` 判斷。** 逐項驗證：

1. **0429 寫入端本來就是 per-customer gated** —— gate 就是
   `else if(CUSTOMER_CODE==CC_FOREHOPE_NINGBO)`，**`CC_FOREHOPE_NINGBO = 790`**，兩樹同值。
   符合「此鏈本就 per-customer gated → 照原 gate 搬，不另加」。

2. **這個 gate 是執行期的，不是編譯期死碼。** 實測：
   - `cmydef.cpp:3404` `int CUSTOMER_CODE=0;`（全域變數，非 `#define`）
   - 值來自 `system\Gerneral.ini` `[System] CUSTOMER_CODE`
     （`main.cpp:1703`、`database.cpp:328` 各讀一次；`HandlerSys.cpp:679` 可由 UI 改寫）
   - 唯一會覆寫它的是 `ASE_KaohSiung` 條件編譯區塊：V910 `MachineType.h:45` 該 `#define` 是**註解掉的**
     → 標準 build 走 `#else`，只把 `CC_ASE_KaohSiung` 改成 `CC_HONPREC_QC`，**790 完全不受影響**。
   → 分支在標準 build 中**是活的**，且只有 `Gerneral.ini` 設 790 的機台會走到。

3. **0817 解析端在 V899 沒有客戶碼 gate**，唯一閘門是 `IniConfig.bSPILFunction`（SPIL 維持舊解析）。
   照原樣搬 → 這是本波**唯一對非 790 客戶可見的行為變化**，必須記進 LEDGER：

   | 面向 | 影響 |
   |---|---|
   | Observer EventLog 頁顯示 | 未加引號但含空白的欄位不再被切散；先前 `Filter 顯示 No Record!!` 的列會正確顯示 |
   | JamRawData CSV（會上 FTP） | 先前因欄位右移而漏算的 JAM 列會被計入 → 數字可能改變（修正方向） |

   **支持照搬的證據**（V899 該段註解自述，非本文件推論）：
   實測 12 家客戶、64 個實機 log 共 100,220 列 —— 99,754 列解析結果完全相同、414 列修好欄位錯位、
   52 列為既有的跨行／內含逗號記錄（無害），**零有害回歸**。且此程式碼**已隨 V899 出貨給所有客戶**。
   → 對 V910 而言是「與 V899 出貨版同步」，非本波新引入的行為。

   **若 主腦 認為仍須加 790 gate**：那會是**新撰寫**的分支（V899 不存在），port_check 會判 AUTHORED，
   且會讓 V910 永久偏離 V899、後續矩陣對帳變難，同時把 414 列的欄位錯位留給其他客戶。
   本文件建議照搬，但把選擇權明確交回 主腦。

4. **相依鏈的落地順序**：兩者**必須同一個 commit 落地**。
   - 只搬 0429（寫入端）不搬 0817（解析端）→ 甬矽機台的 Observer 顯示與 JAM 統計會**當場壞掉**（就是 0813 那個 case）。
   - 只搬 0817 不搬 0429 → 無害（新解析器對引號格式是等價超集）。
   - 因此若因故必須拆波，**安全順序是「先 0817 解析端、後 0429 寫入端」**，與波次任務書字面上的
     「0429 先、0817 後」相反（該敘述應理解為文件敘事順序）。本波兩者同 commit，此點僅供未來拆分時參考。
   - 機械上兩者在**不同檔**，splice 套用先後不影響結果。

5. **檔案格式對外相容性**：0817 只改「讀取顯示／統計」，**EventLog 檔案本身的寫出格式完全不變**
   （寫出格式只由 0429 那個 790-gated 分支決定）→ 非 790 客戶自行開發的 CSV 解析程式不受影響。

---

## 3. 交付與收工 gate 建議（給 主腦）

1. `python tools/port_tools/splice.py docs/mg_w3_ops.json`
   - 預期輸出：`OK note.cpp: 1 op(s) applied, eol=CRLF` / `OK cObserver.cpp: 5 op(s) applied, eol=CRLF`
   - 兩檔各自產生 `.mgbak`（目前 V910 只有 MyLaneIo 的兩個 mgbak，來自 MG-W2）
2. `python tools/port_tools/port_check.py note.cpp cObserver.cpp`
   - 預期：`added` 全數 **SPLICED**、`authored=0`、`comment=0`
   - EOL `(6862,0)->(6875,0)` 與 `(5425,0)->(5480,0)`
   - `REMOVED`：`note.cpp` 2 行、`cObserver.cpp` 20 行（5+7+2+6），全部是 §0.3 證明過的變更前原狀行
3. `bcc32 -c -H- note.cpp` 與 `bcc32 -c -H- cObserver.cpp`（V899 根目錄 include 設定；見 `tools/port_tools/bcc_syntax.sh`）
   - 本波為分析階段唯讀，**未執行任何編譯**
4. 主腦逐條開檔複驗（本文件行號皆為 20260826 實測；套用前請以 `anchor_expect` 自行再確認一次）
5. 收工後 `python tools/port_tools/ai_comment_matrix.py`，MISSING 應由 **368 降 7 條 → 361**
6. LEDGER 需記的外部影響：§2 第 3 點（非 790 客戶的 EventLog 顯示與 JamRawData 統計會與 V899 出貨版對齊）

---

## 4. 結論與待 主腦 裁決事項

- **6 個變更群（7 條矩陣紀錄）→ 6 個 splice op**（1 個 `before`、5 個 `replace`），
  錨點全為**全檔唯一的純 ASCII 字串**。
- **無 <95% 項目。** 五個取代區段的上下文在兩樹位元組相同，公司漂移
  （KYEC 10→21、`IsNNMode` iRow、`sTestIndexZTime` 筆誤修正、註解欄位對齊、空白行）
  **全部落在取代區之外**，不存在 V910 既有行為被覆蓋的情形。
- 記憶體模擬套用後：`cObserver.cpp` 的四個呼叫點區域與新增解析區塊**與 V899 完全相同**；
  `note.cpp` `SaveErrEventLog()` 僅餘 1 行公司註解欄位對齊（本波未觸及該行）。
- **本波不新增客戶碼隔離**；0429 寫入端沿用 V899 的 `CUSTOMER_CODE==CC_FOREHOPE_NINGBO`（790）gate，
  該 gate 為執行期、標準 build 中為活碼、**對非 790 客戶零行為影響**。

**兩件請 主腦 過目（非阻塞）：**

1. **0817 解析端不加客戶碼 gate** —— 依任務書「照原 gate 搬，不另加」執行。
   代價是非 790 客戶的 EventLog 顯示與 JamRawData 統計會跟著修正（V899 已量測背書並出貨）。
   若要改成只開 790，需**新寫**分支，本文件不建議（理由見 §2 第 3 點）。
2. **`Mes/fVATMesFileSys.cpp:2357` 的第五個 EventLog 解析點，V899 自己也沒修**（§0.5）。
   它同樣用 `CommaText` + 位置式 `Strings[0..7]`，閘門是 `IniConfig.bVTESTFunction`。
   兩樹位元組相同，**不是搬移落差**，屬 V899 殘留缺口；建議另開條目回 `ht9045-v899` 決定，本波不動。

**另附一個順手發現（不屬本波，供矩陣校正）**：矩陣列 `note.cpp:1027 20260407 MISSING`，
但 V910 `note.cpp:1036` 已有等價修正（`if(fNote->ReturnCode==K_SKIP && SoftStop==true)`，
公司以 `//Jimmychiu 20260410: V899 add SoftStop guard to prevent false MES2111` 註記）。
該條是**假 MISSING**（矩陣以 AI 註解字串比對，公司改寫了註解故沒命中），排到那一波時可直接判 DONE。
