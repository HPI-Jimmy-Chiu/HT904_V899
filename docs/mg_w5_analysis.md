# MG-W5 分析：20260414 / 20260415 / 20260420 HS_Function KYEC 上傳線

- **波次**：MG-W5（V899 → V910）
- **來源**：`docs/mg_ai_matrix_missing.csv` **11 條 MISSING**（全部在 `HS_Function.cpp`；20260414×7、20260415×3、20260420×1）
- **V899 樹**：`D:\HT9045\HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422`（唯讀；`HS_Function.cpp` md5 `a2fa16b35de33cec1a4028e1acdd5f91`）
- **V910 樹**：`D:\HT9045\HT9011UC_Code_V3.33.910.0_20260716_Jimmy`（本檔 pristine，git status 對 HEAD 無異動；`HS_Function.cpp` md5 `da3af4c25e7cd9dee926b95ee58bda96`，無 `.mgbak`）
- **本文件性質**：分析＋splice op 草案。**分析階段全程唯讀，兩棵樹的 md5 收工前後相同，未修改任何原始碼。**
- **op 檔**：`D:\HT9045\docs\mg_w5_ops.json`（9 個 op）

---

## 0. 前置事實（全部實測，非推論）

### 0.1 檔案基本盤

| 檔 | V899 行數 | V910 行數 | EOL | cp950 解碼錯誤 | 套用後預測行數 |
|---|---|---|---|---|---|
| `HS_Function.cpp` | 4838 | 5346 | 兩樹**全 CRLF**（4838/4838、5346/5346） | 0 / 0 | **5381**（+35） |

`.h` 本波不動（20260504 的 `HS_Function.h:90` 屬 Multi EP 波，非本波）。

### 0.2 矩陣 11 條的完整清單（`grep "HS_Function" docs/mg_ai_matrix_missing.csv` 取全量，一條未漏）

| # | 日期 | V899 行 | 註解 | 落到哪個 op |
|---:|---|---:|---|---|
| 1 | 20260414 | 1360 | trace daily-upload entry only (iLog>=5) to avoid timer-driven log bloat | OP2 |
| 2 | 20260414 | 1371 | trace skip reason | OP2 |
| 3 | 20260414 | 1983 | diagnostic log only on daily-upload path to avoid timer-driven bloat | OP5 |
| 4 | 20260414 | 1997 | trace success | OP6 |
| 5 | 20260414 | 2002 | trace failure | OP7 |
| 6 | 20260414 | 2008 | trace missing file | OP8 |
| 7 | 20260414 | 2014 | trace connect failure | OP9 |
| 8 | 20260415 | 48 | forward-declare MNetLog before first use at UpDataToServer_KYEC | OP1 |
| 9 | 20260415 | 1773 | swap SystemDate to yesterday before GetProdLog（跨午夜檔名） | OP3 |
| 10 | 20260415 | 1802 | swap SystemDate to yesterday before GetGPIBLog（跨午夜檔名） | OP4 |
| 11 | 20260420 | 26 | `//AI(BVL-3766)` 開關 site 時要即時重寫 GTK info.txt | **無 op ＝ C 類**（見 §3） |

> 交派任務原列 8 條，實際 11 條；多出的 3 條是 20260414 的 2002 / 2008 / 2014（同一支
> `UpDataToServerByFTP` 的另外三個 trace 點）。已全數納入。

### 0.3 20260414 變更的真實範圍（用祖先樹反推，不靠猜）

`UpDataToServerByFTP` 在六棵樹的最終態：

| 樹 | 4 個 trace | connect-fail `else` | timeout | try/catch | Eastsun 20260511 KYEC 區塊 |
|---|---|---|---|---|---|
| V896（變更前基準） | ✗ | **✗** | 30000 | ✗ | ✗ |
| **V903**（Jimmy 20260417/0428） | **✓** | **✓** | **30000** | **✗** | ✗ |
| V905.8（Ken 20260608 公司線） | ✗ | ✗ | 30000 | ✗ | ✗ |
| V906（20260618 公司線） | ✗ | ✗ | 30000 | ✗ | **✓** |
| V899（最終態＝搬移來源） | ✓ | ✓ | **5000**（20260630） | **✓**（20260630） | ✗ |
| **V910（搬移目標）** | ✗ | ✗ | 30000 | ✗ | **✓** |

**V903 是決定性證人**：它在 20260414/0415 之後、20260630 之前，含 4 個 trace **與 connect-fail `else`**、
但不含 20260630 的 try/catch 與 5000ms。→ 20260414 的真實 delta 就是「4 個 trace ＋ 1 個 else 區塊」，
`else` 屬本波、try/catch 與 timeout 屬 20260630 波。切分點以此為據，不是猜的。

### 0.4 位元組級證明：V910 的取代區＝變更前原狀，公司未反向修改

| 比對 | 結果 |
|---|---|
| V910 `HS_Function.cpp` 42–52（檔頭）vs V906 42–52 | **IDENTICAL** |
| V910 1580–1601（`UpDataToServer_KYEC` 入口）vs V906 1524–1545 | **IDENTICAL** |
| V910 2175–2225（`UpDataToServerByFTP` 全函式）vs V906 2119–2169 | **IDENTICAL** |

逐行核對（core bytes，忽略 EOL）— **25 個上下文行全部 SAME，唯一 DIFF 是刻意排除的 20260630 timeout 行**：

| 檢查點 | V910 | V899 | 結果 |
|---|---:|---:|---|
| OP1 錨 `#pragma link` | 46 | 46 | SAME |
| OP2 上下文 ×4 | 1587/1588/1589/1591 | 1368/1369/1370/1373 | SAME |
| OP3 上下文 ×2 | 1991/1992 | 1776/1777 | SAME |
| OP4 上下文 ×2 | 2015/2016 | 1805/1806 | SAME |
| OP5 上下文 `TfFTP fFTP` | 2189 | 1982 | SAME |
| OP5 錨 `fFTP.Connect(...)` | 2190 | 1987 | **DIFF（預期）**：V899 已被 20260630 改成 5000ms＋帶註解；本波用 `before` 插入、**不取代**該行 |
| OP6 上下文 ×4 | 2198/2199/2200/2201 | 1995/1996/1998/1999 | SAME |
| OP7 上下文 ×3 | 2202/2203/2204 | 2000/2001/2003 | SAME |
| OP8 上下文 ×4 | 2215/2216/2217/2218 | 2006/2007/2009/2010 | SAME |
| OP9 上下文 ×3 | 2219/2220/2221 | 2011/2017/2018 | SAME |

### 0.5 錨點唯一性（全檔命中次數，V910）

| op | anchor_expect | 命中 |
|---|---|---|
| OP1 | `#pragma link "KeyProDLL.lib"` | **[46]** 唯一 |
| OP2 | `IniConfig.bN10Enable_FTPUpLoadLog==false &&` | **[1587]** 唯一 |
| OP3 | `GetProdLogFileName(false)` | **[1992]** 唯一 |
| OP4 | `lsFileName=GetGPIBLogFileName()` | **[2016]** 唯一 |
| OP5 | `fFTP.Connect(asUserID, asPassword, asHost, 30000` | **[2190]** 唯一 |
| OP6 | `fFTP.Upload(asDirPath, asFtpUplaodPath, sFileName, sFileName, asError)` | **[2198]** 唯一 |
| OP7 | `iReturn=HS_ERR_FTPUploadError;` | **[2204]** 唯一 |
| OP8 | `iReturn=HS_ERR_FTPNoFileForUploadError;` | **[2217]** 唯一 |
| OP9 | `fFTP.Close();` | **[2221]** 唯一 |

全部純 ASCII，9/9 全檔唯一。（對照：`iReturn=HS_ERR_NoError;` 命中 [2177, 2200] ＝**不唯一**，
所以 OP6 刻意把取代區往上擴到 `fFTP.Upload(...)` 那行，換一個唯一錨；同理 OP9 把取代區
往下擴到 `fFTP.Close();`。）

### 0.6 跨檔依賴稽核（新程式用到的每個符號在 V910 都存在且同義）

| 符號 | V910 出處 | 比對 |
|---|---|---|
| `MNetLog` | 定義 `Motor/myMN200motor.cpp:2150`；同檔 `HS_Function.cpp:2102` 已有 extern | 兩樹同簽名 |
| `SystemYear/Month/Date`、`SystemYearYesterday/…` | `cmydef.h:252` `extern Word …` | 與 V899 `cmydef.h:215` **同宣告** |
| `Word` 型別、`GetYesterdayInfo()` | `cpublic.cpp:304`；`HS_Function.cpp:1890` 已在同函式內呼叫 | 交換前 Yesterday 三變數必為新鮮值 |
| `TMyProductionRecord::GetProdLogFilePath/FileName` | `Public/MyProductionRecord.cpp:1150 / 1126` | 兩樹皆有 |
| `TFormHS::GetGPIBLogFilePath/FileName` | `HS_Function.cpp:5080 / 5087` | 兩樹皆有 |
| `HS_ERR_FunctionDisable` / `HS_ERR_FTPUploadError` / `HS_ERR_FTPNoFileForUploadError` / `NMOS_AUTO` | V910 同檔已在用 | — |
| `IniConfig.bN10Enable_FTPUpLoadLog / bN10_DailyUploadProdData / iN10UploadProductMethod / iN10UploadMethod / iN10FtpPort / bN10FtpPassive` | V910 同檔已在用 | — |
| include 鏈 1–17 行 | V910 與 V899 **逐行相同** | 差別只在 `ProductionInfo.h` 的插入位置（V899:26 / V910:29），集合相同 |

- 新區域變數 `wSavYear / wSavMonth / wSavDate / wSavYear2 / wSavMonth2 / wSavDate2`：
  在 V910 `HS_Function.cpp` grep **0 命中** → 無命名衝突。
- `MNetLog` 執行緒歸屬：它會寫 `fMain->slMNetLog` 與 VCL `fMain->mmoMNet->Lines`，屬 UI 執行緒物件。
  但 V910 現況 `UpDataToServerByFTP:2194` 已經無條件呼叫 `MNetLog`，而 `UpDataToServer_KYEC` →
  `UpDataToServerByFTP` 是同一條呼叫鏈（四個呼叫點都在 `TimerAutoBackupTimer` 這個 VCL TTimer 內）。
  → **本波不引入任何新的執行緒暴露面**。

### 0.7 事前模擬（記憶體內套用，未落盤）

依 `docs/mg_w5_ops.json` 全部 9 個 op 模擬套用後：

- `HS_Function.cpp` → **5381 行**（5346 + 35，逐 op 對帳：+4/+9/+5/+5/+4/+1/+1/+1/+5 = +35 ✓）
- cp950 解碼錯誤 **0**；CRLF **5381/5381**、bare-LF **0**
- **11 條矩陣簽章 → 10 條變成 PRESENT**，剩 1 條＝20260420（見 §3，非功能缺口）
- V910 的 Eastsun 20260511 `CC_KYEC_LEE` RecordProcess 區塊（原 2205–2212）**原封不動**保留在模擬結果 2234–2241
- 20260630 的 `30000` timeout（原 2190）**原封不動**保留在模擬結果 2217

---

## 1. 各變更明細

### C1 — `extern bool MNetLog` 前置宣告（矩陣 `HS_Function.cpp:48`，20260415）→ **OP1**

- **V899 範圍**：`47-50`（空行 + AI 註解 + `extern bool MNetLog(AnsiString Message);` + 空行）
- **V910 錨點**：`46` = `#pragma link "KeyProDLL.lib"                                                    //Ifor 20230720 add" Keypro`
- **模式**：`after`（插在 46 之後、V910:47 `TFormHS *FormHS;` 之前）
- **為何必要（非裝飾）**：V910 `HS_Function.cpp` 的 `extern bool MNetLog` 只在 **2102 行**，
  而本波 OP2 會在 **~1591 行**首次呼叫 `MNetLog`。沒有這個前置宣告，OP2 會直接編譯失敗。
  → **OP1 是 OP2/OP3/OP4 的先決條件，不可單獨略過。**
- **不會撞名**：C++ 允許同一 TU 內對同一函式重複 extern 宣告；2102 行那條原樣保留。
- **副作用**：把 V899 的空行一併帶入，使 V910 檔頭與 V899 完全一致（模擬後 44–54 行逐行相同）。
- **風險**：無。
- **信心**：**≥95%**

---

### C2 — `UpDataToServer_KYEC` 進入點／略過原因 trace（矩陣 1360 + 1371，20260414）→ **OP2**

- **V899 範圍**：`1360-1373`（14 行：AI 註解 + `if(iLog>=5){ MNetLog(3 行) }` + 空行 + 原有的
  `if(N10_1==false && N10_3==false){ … }` 四行，其中 1371 是新增的 skip trace）
- **V910 被取代行段**：`1587-1591`（5 行）
- **模式**：`replace`（5 → 14，淨 +9）
- **anchor_expect**：`IniConfig.bN10Enable_FTPUpLoadLog==false &&` → **[1587] 唯一**，純 ASCII
- **語意**：兩條純新增的診斷 log，都被 `iLog>=5` 包住。`iLog>=5` ＝「每日上傳」路徑
  （5=EventLog/Jam 統計/MTBF、6=Power_Saving、7=ATC Power_Saving），
  `iLog 0~4`（EP/溫度/ESD/網芳/ATC Event，由 timer 高頻觸發）**不會**印。
  這正是註解自述的「avoid timer-driven log bloat」。
- **控制流**：`return HS_ERR_FunctionDisable;`（V910:1590 → 新 1603）原樣保留，回傳值不變。
- **風險**：無。純附加輸出。
- **信心**：**≥95%**

---

### C3 — ProdLog 跨午夜檔名修正（矩陣 1773，20260415）→ **OP3**

- **V899 範圍**：`1773-1779`（7 行）
  ```
  1773  //AI(ht9045-v899) 20260415: swap SystemDate to yesterday before GetProdLog …
  1774  Word wSavYear=SystemYear, wSavMonth=SystemMonth, wSavDate=SystemDate;      ← 存
  1775  SystemYear=SystemYearYesterday; SystemMonth=…; SystemDate=…;               ← 換
  1776  asDirPath =TMyProductionRecord().GetProdLogFilePath();
  1777  asFileName=TMyProductionRecord().GetProdLogFileName(false);
  1778  SystemYear=wSavYear; SystemMonth=wSavMonth; SystemDate=wSavDate;           ← 還原
  1779  (空行)
  ```
- **V910 被取代行段**：`1991-1992`（那兩支 Get 呼叫）
- **模式**：`replace`（2 → 7，淨 +5）
- **anchor_expect**：`GetProdLogFileName(false)` → **[1992] 唯一**，純 ASCII
- **成對兩端檢查（交派任務指定項）**：**save / set / restore 三段全部落在同一段連續 7 行內，
  一個 op 原子套用，不存在「只搬一半」的可能。** 已逐行確認 1778 的還原對應 1774 的存檔，
  三個變數一一對上。
- **swap 視窗內會執行什麼**：只有 `GetProdLogFilePath()`（一行 `sprintf`）與
  `GetProdLogFileName(false)`（`sprintf` ＋ 讀 `fLotInfo->` 的 Text property）。
  **無 `Application->ProcessMessages`、無對話框、無 IO 阻塞、無 `MySleep`** → 不會被重入。
- **無 try/finally**：若視窗內丟例外，三個全域會停在「昨天」。實測 `UpDataToServer_KYEC` 的
  try/catch 只在 V910:2066/2080（本區之後），所以例外會逸出並留下髒值。但視窗內只有 sprintf，
  實務上只有 OOM 會丟；且 `GetTimeInfo()` 全樹 122 個呼叫點會很快沖掉。
  **這是 V899 出貨版的原樣行為，忠實搬運不另加 try/finally。**
- **⚠ 行為外溢（見 §2.2，需 主腦 知悉）**：swap 是**無條件**的，但呼叫者有四種
  `iN10UploadProductMethod`（0=00:00、1=08:00&20:00、2=整點、3=CYUEAN 指定時刻）。
- **信心**：機械面 **≥95%**；行為面見 §2.2。

---

### C4 — GPIBLog 跨午夜檔名修正（矩陣 1802，20260415）→ **OP4**

- **V899 範圍**：`1802-1808`（7 行，結構與 C3 完全同型，變數是 `wSavYear2/…2/…2`）
- **V910 被取代行段**：`2015-2016`
- **模式**：`replace`（2 → 7，淨 +5）
- **anchor_expect**：`lsFileName=GetGPIBLogFileName()` → **[2016] 唯一**，純 ASCII
- **成對兩端**：同 C3，save/set/restore 在同一連續段內，原子。
- **swap 視窗內**：`GetGPIBLogFilePath()`（sprintf）＋ `GetGPIBLogFileName()`
  （`FileInfo().GetAllFileNamesInFolder()` FindFirst/FindNext 列資料夾 + 過濾）。
  同樣**無訊息幫浦、無對話框**。列資料夾比 C3 慢（毫秒級），但不重入。
- **信心**：機械面 **≥95%**；行為面見 §2.2。

---

### C5 — `UpDataToServerByFTP` 連線前診斷（矩陣 1983，20260414）→ **OP5**

- **V899 範圍**：`1983-1986`（AI 註解 + `if(bDailyReport) MNetLog(…"FTP connecting: Host/Port/Passive/Type/File"…)`）
- **V910 錨點**：`2190` = `if(fFTP.Connect(asUserID, asPassword, asHost, 30000, NMOS_AUTO, …))`
- **模式**：`before`（插在 2190 之前、`TfFTP fFTP(...)`（2189）之後）
- **為何用 `before` 而不是 `replace`**：錨行本身在 V899 已被 **20260630** 改成 `5000`
  並帶 20260630 的 AI 註解。用 `before` 只插不取代，**20260630 的 timeout 與註解完整留給該波處理**，
  本波不越界。
- **anchor_expect**：`fFTP.Connect(asUserID, asPassword, asHost, 30000` → **[2190] 唯一**。
  刻意把 `30000` 寫進錨字串，等於同時斷言「20260630 波尚未套用」——順序若顛倒會直接紅燈。
- **語意**：`bDailyReport` 只在每日上傳的 6 個呼叫點為 `true`
  （EventLog/JamStatLog/TimeData/ProductionLog/GPIBLog/PowerSavingLog），
  其餘 6+ 個常態呼叫點（EPLog/TemperatureLog/ESDLog/ATCEventLog/ArmTestInfoEvenLog/UnloaderLog）
  預設 `false`（`HS_Function.h:93` 預設引數）→ 不印。同樣是「避免 timer 灌爆 log」的設計。
- **風險**：無。純附加輸出。
- **信心**：**≥95%**

---

### C6 — FTP upload OK trace（矩陣 1997，20260414）→ **OP6**

- **V899 範圍**：`1995-1999`（`if(fFTP.Upload(...))` / `{` / **trace** / `iReturn=HS_ERR_NoError;` / `}`）
- **V910 被取代行段**：`2198-2201`（同四行，缺 trace）
- **模式**：`replace`（4 → 5，淨 +1）
- **anchor_expect**：`fFTP.Upload(asDirPath, asFtpUplaodPath, sFileName, sFileName, asError)` → **[2198] 唯一**
- **為何擴大取代區**：只插一行的話，天然錨是 `iReturn=HS_ERR_NoError;`，該字串在 V910 命中
  **[2177, 2200] 兩處**，防呆會失效。擴到含 `fFTP.Upload(...)` 才有全檔唯一錨。
  四個上下文行已證位元組相同（§0.4），擴大不引入任何額外變更。
- **信心**：**≥95%**

---

### C7 — FTP upload FAIL trace（矩陣 2002，20260414）→ **OP7**

- **V899 範圍**：`2000-2003`（`else` / `{` / **trace** / `iReturn=HS_ERR_FTPUploadError;`）
- **V910 被取代行段**：`2202-2204`（三行）
- **模式**：`replace`（3 → 4，淨 +1）
- **anchor_expect**：`iReturn=HS_ERR_FTPUploadError;` → **[2204] 唯一**
- **⚠ 關鍵：取代區刻意停在 2204。** V910 在 `2205-2212` 有
  **Eastsun 20260511 整合的 `if(CUSTOMER_CODE==CC_KYEC_LEE){ RecordProcess(…) }` 區塊**
  （V899 沒有這段；它來自 V906 公司線）。若照 V899 整塊搬，這段會被吃掉，
  **KYEC 的「FTP Upload Fail 只記錄不報警」會被靜默撤銷**。本 op 只換前三行，
  2205–2213 原封不動。模擬結果已驗證該區塊完整留在 2234–2241。
- **信心**：**≥95%**

---

### C8 — FTP file missing trace（矩陣 2008，20260414）→ **OP8**

- **V899 範圍**：`2006-2010`（`else` / `{` / **trace** / `iReturn=HS_ERR_FTPNoFileForUploadError;` / `}`）
- **V910 被取代行段**：`2215-2218`
- **模式**：`replace`（4 → 5，淨 +1）
- **anchor_expect**：`iReturn=HS_ERR_FTPNoFileForUploadError;` → **[2217] 唯一**
- **信心**：**≥95%**

---

### C9 — connect-fail `else` 分支（矩陣 2014，20260414）→ **OP9**　**← 本波唯一的真行為變更**

- **V899 範圍**：`2011-2018`（`}` / `else` / `{` / **trace** / `iReturn=HS_ERR_FTPUploadError;` / `}` / 空行 / `fFTP.Close();`）
- **V910 被取代行段**：`2219-2221`（`}` / 空行 / `fFTP.Close();`）
- **模式**：`replace`（3 → 8，淨 +5）
- **anchor_expect**：`fFTP.Close();` → **[2221] 唯一**
- **這不只是 trace**：V896 / V905.8 / V906 / V910 在 `if(fFTP.Connect(...))` 之後**完全沒有 else**。
  也就是說 **FTP 連線失敗時，`UpDataToServerByFTP` 目前回傳 `HS_ERR_NoError`（＝0＝成功）**。
  20260414 補上的 `else` 把它改成 `HS_ERR_FTPUploadError`。V903 佐證此 else 屬 20260414 而非 20260630。
- **回傳值消費者稽核（實測，兩棵樹都查）**：
  - `HS_Function.cpp` 內 12 個呼叫點，只有 3 個接了回傳值：
    `iFTPStatus[0]=`（V910:1801）、`iFTPStatus[1]=`（1946）、`iFTPStatus[2]=`（1978）
  - `int iFTPStatus[3]={0,0,0};`（V910:1599 / V899:1381）**宣告後只被寫、從未被讀**
    （全檔 grep `iFTPStatus` 僅 4 命中＝1 宣告 + 3 賦值）。**V899 也一樣。**
  - 樹內其他呼叫點：`Automation/SCK_ART.cpp:3342,3346`、`cprod.cpp:1066,1276` 全部丟棄回傳值；
    `csystem.cpp:11531,11535` 已註解掉。`main.cpp` 用的是另一支 `BackUpDataToServerByFTP`。
  - grep `=.*UpDataToServerByFTP` 與 `if(.*UpDataToServerByFTP` 全樹：**只有上述 3 條 iFTPStatus**。
  → **回傳碼在兩棵樹都是死值，這個行為變更的實際可觀察面為零。**
- **風險**：低。若日後有人開始讀 `iFTPStatus`，行為會與 V899 一致（這正是我們要的）。
- **信心**：**≥95%**（不是「應該沒人用」，是實測全樹無消費者）

---

## 2. Gate 判斷與客戶碼隔離評估

### 2.1 「這條線是否全程在 KYEC gate 內？」→ **不是。**（逐層實測）

| 層 | gate | 是否 KYEC 專屬 |
|---|---|---|
| 四個 `UpDataToServer_KYEC(5)` 呼叫點（V910:192/215/246/501） | `IniConfig.bN10_DailyUploadProdData && iN10UploadProductMethod==0/1/2/3` | **否**，純 ini 旗標 |
| `UpDataToServer_KYEC(0,…)`（V910:640） | `IniConfig.bN10Enable_FTPUpLoadLog` ＋ **`CUSTOMER_CODE!=CC_KYEC_LEE`**（Eastsun 20260511） | **反向**：這條**排除** KYEC |
| `UpDataToServer_KYEC` 函式入口（OP2 位置） | `bN10Enable_FTPUpLoadLog \|\| bN10_DailyUploadProdData` | **否** |
| ProdLog 區塊（OP3 位置） | `CUSTOMER_CODE==CC_SIGURD_HUKOU \|\| IniConfig.bN10_11_Enable_UploadFTPEventLog` | **否**，而且明確點名的是**矽格湖口**不是 KYEC |
| GPIBLog 區塊（OP4 位置） | `IniConfig.bN10_12_Enable_UploadFTPGPIBLog` | **否** |
| `UpDataToServerByFTP`（OP5–OP9 位置） | 無 gate；12+ 個呼叫點含 `SCK_ART.cpp`（ART/SLT Report）、`cprod.cpp`（JamRate） | **否** |

> **函式名字裡的 `_KYEC` 是歷史命名，不是閘門。** 整條線由 `IniConfig.bN10*` 系列 ini 旗標控制，
> 任何開了 N10 的客戶都會走。唯一出現的客戶碼是 `CC_SIGURD_HUKOU`（矽格湖口，ProdLog 區塊）
> 與 `CC_KYEC_LEE`（Eastsun 20260511 那兩段，本波都不動）。

### 2.2 需不需要加客戶碼隔離？

**結論：不加。** 但有一項行為外溢必須記進 LEDGER 並提請 主腦 裁決。

- **OP1 / OP2 / OP5 / OP6 / OP7 / OP8**：純附加 log，且已被 `iLog>=5` / `bDailyReport` 收斂在
  每日上傳路徑。控制流、回傳值、例外行為 **0 變更**。→ 不需隔離。
- **OP9**：回傳碼變更，但全樹無消費者（§C9 實測）。→ 不需隔離。
- **OP3 / OP4（⚠ 外溢）**：`SystemDate` 換成昨天是**無條件**的，但呼叫時機有四種：

  | method | 觸發 | 交換前（V910 現況） | 交換後（V899 行為） | 判斷 |
  |---|---|---|---|---|
  | 0 | 00:00 | 抓**今天**的檔（此刻還不存在）→ `File not exists!` | 抓昨天 → 正確 | **這就是被修的 bug** |
  | 1 | 08:00 | 抓今天 | 抓昨天 | 08:00 抓昨天正確（與同區塊 JamStat 的 `SystemHour<8 → Yesterday` 分支一致） |
  | 1 | **20:00** | 抓今天 | 抓昨天 | **與鄰近 JamStat 區塊不一致**（JamStat 在 8≤hour<20 走**今天**） |
  | 2 | 每整點 | 抓今天（成長中的當日檔） | 抓昨天 | **語意改變**：伺服器當日不再收到當天資料 |
  | 3 | CYUEAN 指定時刻 | 抓今天 | 抓昨天 | 依指定時刻而定 |

  另註：當 `IniConfig.bO24_ProductionLogByLot` 或 `CUSTOMER_CODE==CC_SJ_Semiconductor` 時，
  `GetProdLogFileName()` 的檔名用 LotID＋開批時間、**不含日期**，此時 swap 只影響
  `GetProdLogFilePath()` 的 `\YYYYMM\` 子目錄 → 只在月初 00:00 有差，且方向是**正確的**。

  **本波處置（依 MG 鐵律「忠實優先於寫得更好」＋MG-W2 §C4/C5 先例）**：照 V899 最終態原樣搬，
  不自行加 hour-aware 條件、不加客戶碼。理由：
  1. V899 是**量產出貨版**，此碼自 20260415 起實跑約 4 個月，含 `CC_SIGURD_HUKOU` 機台，
     `docs/mg_inventory_weekly_cases.md` 未見任何相關客訴案。
  2. 20260414/20260415 是**孤兒叢集**（`mg_inventory_weekly_cases.md` §4.4 明列這兩天無對應 case），
     **沒有任何客戶碼可以拿來當隔離依據**——硬造一個 gate 反而是自行發明行為。
  3. 若判定 method 1/2/3 的日期基準需要改成 hour-aware，那是 **V899 本身的議題**，
     應回 `ht9045-v899` 決定，不在 MG 搬移波內偷改。

  **本分析 agent 全程唯讀，未動 `MG_FINAL_DECISIONS.md`。** 以下是給 主腦 直接貼上的建議列：

  ```
  | F? | MG-W5（20260415 HS_Function ProdLog/GPIBLog 跨午夜） | OP3/OP4 的 SystemDate→昨天是無條件的，
  但 UpDataToServer_KYEC 有四種 iN10UploadProductMethod（0=00:00 / 1=08:00&20:00 / 2=整點 /
  3=CYUEAN 指定時刻）。method 0 是被修的 bug（正確）；method 1@20:00、method 2 每整點、method 3
  會從「抓今天的 ProdLog/GPIBLog」變成「抓昨天的」，且與同區塊 JamStat 的 hour-aware 日期基準不一致 |
  (a) 照搬（＝V899 出貨行為） (b) 改成 hour-aware，與 JamStat 對齊 (c) 不搬 OP3/OP4 |
  **建議 (a)**：V899 自 20260415 出貨實跑約 4 個月（含 CC_SIGURD_HUKOU 機台）無相關客訴案；
  且 20260414/0415 是孤兒叢集、無 case 可提供客戶碼，硬造 gate 等於自行發明行為。
  已按 95% 規則排入波次照搬；若你選 (b)，那是 V899 本身的議題應回 ht9045-v899 修，再由 MG 同步。 |
  ```

---

## 3. 20260420 BVL-3766：判 **C 類（V910 已有），不出 op**

矩陣把 `HS_Function.cpp:26` 列成 MISSING，但**功能早已在 V910**，缺的只是註解文字：

| 位置 | V899 | V910 |
|---|---|---|
| `HS_Function.cpp` | `26: #include "ProductionInfo.h"  //AI(BVL-3766) 20260420: 開關 site 時要即時重寫 GTK info.txt` | `29: #include "ProductionInfo.h"  //AI(BVL-3766) 20260420: rewrite GTK info.txt RunSite when site on/off` |
| `Command.cpp` | `5258` BVL-3766 20260428 | `5333` **同 Big5 註解，位元組相同**（故不在 MISSING 清單） |
| `ProductionInfo/ProductionInfo.cpp` | `4922 / 4946 / 4948` | `4849 / 4873 / 4875` **三條同 Big5 註解，位元組相同** |

- `#include` 那一行的**程式碼部分完全相同**，只有行尾註解被公司改寫成英文；
  `ai_comment_matrix.py` 的簽章是「`//AI(` 起到行尾」的位元組串，所以英文版必然判 MISSING。
- V906（公司線）第 29 行已經是英文版 → 這是 **V906→V910 繼承下來的公司改寫**，不是缺功能。
- 交叉佐證：`docs/mg_inventory_weekly_cases.md` 第 41 列
  「Greatek / CASE-20260428-001 / `CC_Greatek` / ✅ BVL-3766 20260420/20260428」已標記完成。
- include 位置不同（V899 在 `database.h` 之後；V910 在 `uYieldMonitoring.h` 之後）：
  include **集合完全相同**，順序差異對 header 無影響，且 V910 現況本來就編得過 → **不動**。

**處置**：`skipped-C`，不改 V910。代價是這一條矩陣列**永遠不會消失**——收工驗收時
MISSING 只會由 376 降 **10** 條（不是 11），這是預期值不是漏搬。
（若 主腦 決定要清帳，唯一方法是把 V910:29 的英文註解換回 V899 的 Big5 版；
那是純註解改動、無功能，本波不主張做。）

---

## 4. 交付與收工 gate 建議（給 主腦）

1. **順序前提**：本波 op 的行號是對 **pristine 的 V910 `HS_Function.cpp`（md5 `da3af4c2…`、無 `.mgbak`）**量的。
   `HS_Function.cpp` 還有 4 個 pending 波（20260504×5、20260519×1、20260526×1、20260630×2）。
   **MG-W5 必須先跑；後續任何 HS_Function.cpp 波都要在 MG-W5 落地後重新定位行號。**
   位移表（套用 MG-W5 後）：
   ```
   N ≤ 46            → N
   47 ≤ N ≤ 1586     → N+4
   1592 ≤ N ≤ 1990   → N+13
   1993 ≤ N ≤ 2014   → N+18
   2017 ≤ N ≤ 2189   → N+23
   2190 ≤ N ≤ 2197   → N+27
   2205 ≤ N ≤ 2214   → N+29      （Eastsun 20260511 區塊）
   N ≥ 2222          → N+35
   ```
   （1587–1591 / 1991–1992 / 2015–2016 / 2198–2201 / 2202–2204 / 2215–2218 / 2219–2221 是被取代區）
   特別提醒 20260630 波：它的兩個點在 pristine 行號 2175(`try` 要插的函式開頭區) 與 2190(timeout)，
   套 MG-W5 後分別是 2198 與 **2217**。

2. `python tools/port_tools/splice.py docs/mg_w5_ops.json`
   - 預期輸出：`OK HS_Function.cpp: 9 op(s) applied, eol=CRLF`
   - 產生 `HS_Function.cpp.mgbak`

3. `python tools/port_tools/port_check.py HS_Function.cpp`
   - **預期輸出（已用同一份 difflib 邏輯對模擬結果實測，不是估算）**：
     ```
     [HS_Function.cpp] added=35 (spliced=35, comment=0, authored=0) removed=0 | EOL (5346,0)->(5381,0)
     ```
   - `removed=0` 是本波最強的一項保證：**沒有任何一行 V910 既有內容被丟掉**。
     九個 op 裡雖有七個是 `replace`，但被取代的 23 行全部與 payload 中的對應行位元組相同
     （§0.4 已逐行證明），difflib 會把它們判為 `equal`，實際淨效果等同 35 行純插入。
     Eastsun 20260511 KYEC 區塊、20260630 的 30000 timeout 都因此毫髮無傷。
   - `authored=0` ＝本波沒有任何一行是我們自己打的字，全部從 V899 位元組抽出。

4. `bcc32 -c -H- HS_Function.cpp`（V899 根目錄 include 設定；先確認無 BCB IDE 開著佔用 `Obj/`）

5. **主腦逐條開檔複驗**（本文件 §1 行號皆為 20260826 實測；套用前請用 `anchor_expect` 自行再確認一次）。
   最該親眼看的三處：
   - 新 2234–2241 的 Eastsun `CC_KYEC_LEE` 區塊**還在**（OP7 沒吃掉它）
   - 新 2217 的 `fFTP.Connect(..., 30000, ...)` **還是 30000**（20260630 沒被本波帶進來）
   - 新 2249–2254 的 connect-fail `else`（OP9，本波唯一真行為變更）

6. 收工後 `python tools/port_tools/ai_comment_matrix.py`，
   MISSING 應由 376 降 **10** 條（20260420 那條依 §3 刻意留著）。

---

## 5. 結論

- **11 條矩陣點 → 10 條搬（9 個 splice op）＋ 1 條判 C 類不搬。**
- **無機械面 <95% 項目**：9 個錨點全為全檔唯一的純 ASCII 字串；V910 三個目標區與 V906 位元組相同，
  公司無反向修改；25 個上下文行逐行位元組相同（唯一 DIFF 是刻意排除的 20260630 timeout 行）。
- **`removed=0`（實測）**：淨效果是 35 行純插入，**V910 既有內容一行都沒少**——
  七個 `replace` 只是為了取得唯一錨點而擴大的取代區，被取代行與 payload 對應行位元組相同。
- **本條線不在 KYEC gate 內**（§2.1）：函式名的 `_KYEC` 是歷史命名，實際由 `IniConfig.bN10*`
  ini 旗標控制，出現的客戶碼是 `CC_SIGURD_HUKOU`（ProdLog）與 `CC_KYEC_LEE`（Eastsun 兩段，不動）。
- **跨午夜 SystemDate swap 的成對兩端都在同一段連續行內，各由一個 op 原子套用**，不存在只搬一半。
- **兩項須記進 LEDGER 的外溢**：
  1. OP9 讓 FTP 連線失敗不再回報成功（回傳碼實測全樹無消費者，可觀察面為零）。
  2. OP3/OP4 的無條件「抓昨天」對 `iN10UploadProductMethod` 1/2/3 是行為變更
     （§2.2 表），已依「忠實優先」照搬並送 `MG_FINAL_DECISIONS` 作資訊型條目。
- **排程約束**：本波必須是 `HS_Function.cpp` 的第一個波；其餘 4 個 pending 波要用 §4.1 的位移表重定位。
