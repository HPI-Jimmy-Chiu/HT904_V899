# MG-W2 分析：20260424 MyLaneIo 售服訊息叢集

- **波次**：MG-W2（V899 → V910）
- **來源**：`docs/mg_ai_matrix_missing.csv` 8 條 MISSING（MyLaneIo.cpp × 6、MyLaneIo.h × 2）
- **V899 樹**：`D:\HT9045\HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422`（唯讀，端點 `bb69c60`，工作區對 HEAD 無異動）
- **V910 樹**：`D:\HT9045\HT9011UC_Code_V3.33.910.0_20260716_Jimmy`（基線 `e06524a`）
- **本文件性質**：分析＋splice op 草案。**分析階段全程唯讀，未修改任何原始碼。**
- **op 檔**：`D:\HT9045\docs\mg_w2_ops.json`

---

## 0. 前置事實（全部實測，非推論）

### 0.1 檔案基本盤

| 檔 | V899 行數 | V910 行數 | EOL | 套用後預測行數 |
|---|---|---|---|---|
| `MyLaneIo.cpp` | 788 | 744 | 兩樹**全 CRLF**（788/788、744/744） | **798** |
| `MyLaneIo.h` | 46 | 42 | 兩樹**全 CRLF**（46/46、42/42） | **46** |

V910 兩檔目前 `//AI(` 出現次數皆為 **0** → 8 條 MISSING 判定成立，無「已搬一半」狀態。

### 0.2 20260424 變更的真實範圍（用祖先樹反推，非猜測）

| 樹 | MyLaneIo.cpp 行數 | 含 `BuildIOErrMessage` |
|---|---|---|
| `V3.33.896.0_20260204_Jimmy_20260310_AI` | 735 | 0（**變更前基準**） |
| `V3.33.899.0_..._Jimmy_20260422` | 788 | 7（**變更後 = 搬移來源**） |
| `V3.33.903.0_20260417_Jimmy_20260428` | 788 | 7（同作者下一版，佐證） |
| `V3.33.905.8_20260608_Ken` | 734 | 0（公司線，未合併） |
| `V3.33.906.0_20260618` | 734 | 0（公司線，未合併） |
| `V3.33.910.0_20260716_Jimmy` | 744 | 0（**搬移目標**） |

`diff 896 → 899` 恰好 **4 個 hunk**，行數增減 `+1 −10 −10 +72 = +53`，與 `735 → 788` 完全對帳。
→ 8 條矩陣紀錄 = **4 個 cpp 變更點 + 1 個 h 變更點**，無漏、無多。

### 0.3 V910 的公司漂移（決定「會不會覆蓋 V910 行為」的關鍵）

`diff 906 → 910`（MyLaneIo.cpp）只有 **2 個 hunk**，共 39 行 diff：

1. 檔頭新增 `extern TPLCIOInfo g_tPLCIOInfo;`
2. `IOInputBit()` 內的 **Safe PLC 輸入區塊**改寫（`IsSafePLCIOInstall()` / `g_tPLCIOInfo.m_wInputStartRegNum` / `ePLCGrp_Count` / `iSlave`，支援 HT9064LS 雙 PLCIO 模組）

**兩者都不落在本波任何一個取代區段內。**（第 2 項在 `IOInputBit` 的 `bPLCIO[Port][Bit]` 分支，位於 OP-2 取代區之**後**。）

`diff 896 → 906` 則幾乎全是**註解欄位對齊**（註解由第 ~80 欄推到 ~120 欄）＋刪除檔尾一個空行，同樣不碰任何取代區段的**程式敘述**。

### 0.4 位元組級證明：V910 的 4 個取代區段 == 896 原狀

| op | V910 行段 | 896 行段 | 位元組相等 |
|---|---|---|---|
| OP-1 | 329–331 | 327–329 | **True** |
| OP-2 | 360–373 | 358–371 | **True** |
| OP-3 | 458–471 | 448–461 | **True** |
| OP-4 | 715–743 | 705–733 | **True** |

→ **V910 在這四個位置上完全是變更前的原狀，公司從未反向修改過。**
→ 「V910 既有行為被覆蓋」的風險在本波為 **零**（不是「低」，是實測為零）。

### 0.5 跨檔依賴稽核

- `BuildIOErrMessage` / `GetUseMNetIPDevTypeName` 在 **整棵 V899 樹**的呼叫點：**只有 MyLaneIo.cpp 與 MyLaneIo.h 兩檔**（grep 全樹，排除 `.svn`）。無跨檔依賴。
- 新程式用到的每個符號在 V910 都存在且**值相同**：

| 符號 | V910 出處 | 比對 |
|---|---|---|
| `MAXRing` / `MAXIP` / `MAXPort` | `Motor/myMN200motor.h`（`const int` 4 / 64 / 4） | 與 V899 **同值** |
| `DEV_INF_NO_DEV / MOTION_DEV / IO_32OUT_DEV / IO_32IN_DEV / IO_16IN_16OUT_DEV / AI_8IN_DEV` | `Motor/mn200.h:183-188`（`#define`） | 與 V899 `184-189` **同值** |
| `iUseMNetIP[MAXRing][MAXIP]` | `MyLaneIo.h:31`（public 成員） | 兩樹同宣告 |
| `ShowMyMessage(AnsiString S1, AnsiString S2="", ...)` | `mymessbox.h:58` | 兩樹**同簽名**；V899 的單引數呼叫合法 |
| `MNetLog` / `fiosetview` / `str` | 已在 V910 同檔使用中 | — |

- include 鏈：`MyLaneIo.cpp` → `MyLaneIo.h` → `myMN200motor.h` → `MN200.h`（即 `Motor/mn200.h`）。兩樹 include 清單**完全相同**（V910 僅多一行 `extern TPLCIOInfo g_tPLCIOInfo;`）。→ `DEV_INF_*` 在 V910 的 TU 內可見。

### 0.6 事前模擬（記憶體內套用，未落盤）

依 `mg_w2_ops.json` 全部 5 個 op 模擬套用後：

- `MyLaneIo.cpp` → **798 行**（= V899 788 + 公司 SafePLC 漂移 10 行，對帳吻合）；cp950 解碼錯誤 **0**；CRLF **798/798**
- `MyLaneIo.h` → **46 行**；cp950 解碼錯誤 **0**；CRLF **46/46**
- `diff 模擬結果 vs V899`：
  - `.h` — **完全相同（0 差異）**
  - `.cpp` — **只剩** 0.3 節那兩項公司漂移 ＋ 8 處註解欄位對齊，**無任何殘餘功能落差**

---

## 1. 各變更明細

### C1 — `IOOutBitStatus()` 錯誤訊息（矩陣 MyLaneIo.cpp:327）→ **OP-1**

- **V899 範圍**：`MyLaneIo.cpp:327-330`（AI 註解 + `str.sprintf` + `MNetLog(str)` + `ShowMyMessage(...)` 共 4 行）
- **V910 錨點 / 被取代行段**：`MyLaneIo.cpp:329-331`
  - `329` = `        str.sprintf("IOOutBitStatus (%s) Ring %d | IP %d | Port %d | Byte %d Fail.", Alias, Ring, IP, Port, Bit);`
  - `330` = `        MNetLog(str);`
  - `331` = `        ShowMyMessage(str, GetIOErrStr(iHasErr));`
- **模式**：`replace`（3 行 → 4 行）
- **anchor_expect**：`IOOutBitStatus (%s) Ring %d | IP %d | Port %d | Byte %d Fail.`
  → 全檔命中 **[329] 唯一**，純 ASCII
- **語意**：log 改為短格式並帶 `iErr=%d`（供解析）；UI 改叫 `BuildIOErrMessage(true, ...)`。順帶修掉原本 log 把 `Bit` 印成 `Byte %d` 的字面錯字。
- **不動**：外層 `#ifndef SOFT_SIMULTE` / `#endif`（328、332）與 `return false;` 皆保留。
- **風險**：無。控制流、回傳值、`SOFT_SIMULTE` 守衛都不變。
- **信心**：**≥95%**（V910 該三行與 896 原狀位元組相同；錨點唯一）

---

### C2 — `IOInputBit()` 錯誤訊息（矩陣 MyLaneIo.cpp:359）→ **OP-2**

- **V899 範圍**：`MyLaneIo.cpp:359-362`（4 行）
- **V910 錨點 / 被取代行段**：`MyLaneIo.cpp:360-373`（14 行）
  - `360` = `            str.sprintf("IOInputBit (%s) Ring %d, IP %d, Port %d, Bit %d Fail. %s", Alias, Ring, IP, Port, Bit, GetIOErrStr(iHasErr));`
  - `361` = `            MNetLog(str);`
  - `362-373` = 整段 `if(MachineTypeChoice==Type_HT9045) / else if(Type_HT9045_12Site) / else` 三分支（各自 `ShowMyMessage(str, "HTxxxx : Maybe input wrong IO position!")`）
- **模式**：`replace`（14 行 → 4 行）
- **anchor_expect**：`IOInputBit (%s) Ring %d, IP %d, Port %d, Bit %d Fail. %s`
  → 全檔命中 **[360] 唯一**（同檔 437 行是 `... Bit %d Fail, Code: %d`，不同字串），純 ASCII
- **語意**：三個機種分支只是選三句不同的英文提示字，無行為差異；一律換成 `BuildIOErrMessage(false, "IOInputBit", ...)`。
- **不動**：外層 `if(fiosetview->fShow==false)`（357）、`#ifndef SOFT_SIMULTE`（359）/`#endif`（374）、`return false;`（376）全保留。**V910 之後的 Safe PLC 公司改寫（378-404）完全不在取代區內。**
- **風險**：`MachineTypeChoice` 在本檔還有 6 處使用（128/132、195/199、262/266，屬 `IOBitOn`/`IOBitOff`/`IOByteOut`，本波不動），移除本處不會產生未用符號問題。
- **信心**：**≥95%**

---

### C3 — `IOInputByte()` 錯誤訊息（矩陣 MyLaneIo.cpp:439）→ **OP-3**

- **V899 範圍**：`MyLaneIo.cpp:439-442`（4 行）
- **V910 錨點 / 被取代行段**：`MyLaneIo.cpp:458-471`（14 行）
  - `458` = `        str.sprintf("IOInputByte Ring %d | IP %d | Port %d Fail. %s", Ring, IP, Port, GetIOErrStr(iHasErr));`
  - `459` = `        MNetLog(str);`
  - `460-471` = 同 C2 的 `MachineTypeChoice` 三分支
- **模式**：`replace`（14 行 → 4 行）
- **anchor_expect**：`IOInputByte Ring %d | IP %d | Port %d Fail. %s`
  → 全檔命中 **[458] 唯一**（同檔 ~487 行是逗號版 `Fail, Code: %d`），純 ASCII
- **語意**：Byte 操作沒有 Bit，`BuildIOErrMessage` 的 `Bit` 引數傳 `-1`，`sAlias` 傳字面 `AnsiString("(整 Byte)")`。
- **不動**：`#ifndef SOFT_SIMULTE`（457）/`#endif`（472）、`return 0;`（473）保留。
- **風險**：無。
- **信心**：**≥95%**

---

### C4 — `GetIOErrStr()` 中文化（矩陣 MyLaneIo.cpp:695）→ **OP-4（前半）**

- **V899 範圍**：`MyLaneIo.cpp:686-724`（整支 `AnsiString TLaneIO::GetIOErrStr(int iErr)` 函式，`case 0..5` + `default` 全部換成含 `\r\n` 的中文說明＋修法箭頭）
- **V910 對應**：`MyLaneIo.cpp:715-743`（英文短句版，與 896 原狀位元組相同）

### C5 — 兩支新函式（矩陣 MyLaneIo.cpp:726、757）→ **OP-4（後半）**

- **V899 範圍**：`MyLaneIo.cpp:725-787`
  - `725` 分隔線 + `726-755` `GetUseMNetIPDevTypeName()`（`DEV_INF_*` → 中文裝置名）
  - `756` 分隔線 + `757-787` `BuildIOErrMessage()`（5 段式訊息：點位名稱／CSV 位置／實際模組／錯誤說明／修正建議）
- **V910 對應**：**不存在**，需新增

#### OP-4 合併說明（C4 + C5 合成單一 op）

- **V899 範圍**：`MyLaneIo.cpp:686-788`（`GetIOErrStr` 起、到檔尾分隔線止，共 103 行）
- **V910 被取代行段**：`MyLaneIo.cpp:715-744`（`GetIOErrStr` 起、到檔尾分隔線止，共 30 行）
- **模式**：`replace`（30 行 → 103 行，淨 +73）
- **anchor_expect**：`UnKnown Error,iErr=%d` → 全檔命中 **[739] 唯一**，純 ASCII
- **為何合併**：C5 若獨立成 `mode:"after", anchor_line:743`，該行內容是單一個 `}`，`anchor_expect` 只能填 `"}"`——**防呆形同虛設**（`splice.py` 只檢查 `expect in dst[a-1]`）。改用 `before` 錨在 744 的分隔線同樣不獨特。合併成一個 replace 後，錨點是全檔唯一的 `UnKnown Error,iErr=%d`，且兩檔的「`GetIOErrStr` 到檔尾」正好是同一段最終態，**原子性與防呆同時最佳**。
- **語意 / 副作用（需 主腦 知悉，非阻塞）**：
  強化後的 `GetIOErrStr()` 回傳**多行 Big5 文字（內含 `\r\n`）**。V910 另有**三個本波不動的呼叫點**會把它塞進 `MNetLog(str)`：
  - `MyLaneIo.cpp:126` `IOBitOn`
  - `MyLaneIo.cpp:193` `IOBitOff`
  - `MyLaneIo.cpp:260` `IOByteOut`

  → 這三筆 MNetLog 紀錄會從單行變多行。
  **但這正是 V899 出貨版的實際行為**（V899 同三處在 124/191/258，字串完全相同且同樣未改）。忠實搬運即等同 V899，不另行「修得更好」。若日後要讓 MNetLog 保持單行，屬 V899 本身的議題，應回 `ht9045-v899` 決定，不在本波處理。
- **其他檢查**：字串內 `D:\\HT9045\\Error\\MNetLog*.txt`、`D:\\HT9045\\System\\IO_Table.csv` 為正確 C 轉義；`\"%s\"` 轉義正確；`sret.sprintf("未知裝置 (0x%02X)", iDevType)` 引數型別相符。這批位元組已在 V899 / V903 兩樹實際編譯出貨，Big5 雙位元組尾碼相容性有出貨背書。
- **風險**：`BuildIOErrMessage` 內 `Ring>=0 && Ring<MAXRing && IP>=0 && IP<MAXIP` 已自帶邊界守衛，`iUseMNetIP[Ring][IP]` 不會越界。
- **信心**：**≥95%**

---

### C6 — `MyLaneIo.h` 兩支宣告（矩陣 MyLaneIo.h:40、41）→ **OP-5**

- **V899 範圍**：`MyLaneIo.h:40-43`（4 行連續：`GetUseMNetIPDevTypeName` 宣告 1 行 + `BuildIOErrMessage` 宣告 3 行續行）
- **V910 錨點**：`MyLaneIo.h:39` = `        AnsiString GetIOErrStr(int iErr);`
  → 全檔命中 **[39] 唯一**，純 ASCII
- **模式**：`after`（插在 39 之後、`};`（V910:40）之前）
- **語意**：兩條 public 成員宣告，落在 `public:` 區段內（V910 `public:` 在 20 行、class 結尾 `};` 在 40 行）。
- **風險**：無。矩陣兩條紀錄（40、41）落在同一連續區塊，一個 op 一次補齊。
- **信心**：**≥95%**（模擬後 `.h` 與 V899 **完全相同**）

---

## 2. 客戶碼隔離需求評估

**結論：本波不需要任何 `CUSTOMER_CODE` / `FUNC_CC_*` 隔離。** 逐項驗證：

1. **觸發條件**：四個變更點全都在 `if(iHasErr!=0)` 之內，`iHasErr` 來自 `CheckPortRangeErr()`。也就是只有 `IO_Table.csv` / Sensor DB **設定本身非法**時才會走到。此時機台已處於設定錯誤狀態，非量產路徑。
2. **控制流**：`return false;` / `return 0;` / `#ifndef SOFT_SIMULTE` 守衛四處皆原樣保留 —— 回傳值與流程 **0 變更**。
3. **時序**：`MNetLog` 一次、`ShowMyMessage` 一次，呼叫次數與順序不變（C2/C3 的 `MachineTypeChoice` 三分支本來就只會執行其中一個 `ShowMyMessage`）。
4. **機種差異**：被移除的 `MachineTypeChoice` 分支只挑選三句不同的英文提示字（`HT9045` / `HT9045_12Site` / `HT9046`），三條路徑行為完全相同。改成統一訊息不造成機種行為差異。
5. **`ShowMyMessage` 引數**：由 2 個減為 1 個，`S2` 走預設值 `""`。純視覺變化，訊息框行為不變。
6. **`GetIOErrStr` 文案**：唯一外溢是 §C4/C5 記的三處 MNetLog 變多行 —— 那是**日誌文字**，非機台行為，且與 V899 出貨版一致。

**唯一需要記在 LEDGER 的外部影響**：MNetLog 該三行變多行。若客戶端有逐行 parse `D:\HT9045\Error\MNetLog*.txt` 的工具，需知悉；但 V899 已如此出貨，屬既有現況同步，非本波新引入。

---

## 3. 交付與收工 gate 建議（給 主腦）

1. `python tools/port_tools/splice.py docs/mg_w2_ops.json`
   - 預期輸出：`OK MyLaneIo.cpp: 4 op(s) applied, eol=CRLF` / `OK MyLaneIo.h: 1 op(s) applied, eol=CRLF`
   - 兩檔各自產生 `.mgbak`
2. `python tools/port_tools/port_check.py MyLaneIo.cpp MyLaneIo.h`
   - 預期：`added` 全數歸為 **SPLICED**（每一行都能在 V899 同路徑檔找到位元組相同的核心）、`authored=0`、`comment=0`、EOL `(744,0)->(798,0)` 與 `(42,0)->(46,0)`
   - `REMOVED` 應列出 60 行（3+14+14+29），全部是 §0.4 證明過的 896 原狀行
3. `bcc32 -c -H- MyLaneIo.cpp`（V899 根目錄 include 設定）
4. 主腦逐條開檔複驗（本文件 §1 的行號皆為 20260826 實測，套用前請以 `anchor_expect` 自行再確認一次）
5. 收工後 `python tools/port_tools/ai_comment_matrix.py`，MISSING 應由 376 降 **8** 條

---

## 4. 結論

- **6 個變更群（8 條矩陣紀錄）→ 5 個 splice op**，全部 `replace`／`after`，錨點全為**全檔唯一的純 ASCII 字串**。
- **無 <95% 項目。** V910 在四個取代區段上與變更前基準（896 樹）位元組完全相同，公司漂移（`g_tPLCIOInfo` extern ＋ Safe PLC 區塊改寫）全部落在取代區之外，**不存在 V910 行為被覆蓋的情形**。
- 記憶體模擬套用後，`.h` 與 V899 完全相同、`.cpp` 僅餘上述公司漂移與註解欄位對齊，證明搬移是忠實且完整的。
- 本波**不需要客戶碼隔離**；唯一外溢（三處 MNetLog 變多行）為 V899 既有出貨行為，已記錄於 §C4/C5 與 §2。
