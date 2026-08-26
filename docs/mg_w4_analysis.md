# MG-W4 分析：20260423 殘項雙主題（BootLog 開機診斷 ／ LOAD_Y stepper 家族啟用）

- **波次**：MG-W4（V899 → V910）
- **來源**：`docs/mg_ai_matrix_missing.csv` **16 條 MISSING**（全檔 MISSING 總數 368）
  - 主題 A：`main.cpp` × 5（6 / 8567 / 10737 / 25798 / 25830，皆 20260423）＋ `main.cpp:10743`（20260429，與 10737 同區塊）＝ **6 條**
  - 主題 B：`HandlerSys.cpp` × 5（519–523）＋ `uMotorTest.cpp` × 5（204–208）＝ **10 條**
  - 註：10743 與 10737 落在同一個 `#ifndef SOFT_SIMULTE` 區塊，一個 op 一次搬齊
  - **已逐條驗證**：16 條簽章在模擬套用後 **16/16 全部命中**模擬產物（見 §0.4）
- **V899 樹**：`D:\HT9045\HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422`（唯讀）
- **V910 樹**：`D:\HT9045\HT9011UC_Code_V3.33.910.0_20260716_Jimmy`（基線 `e06524a`）
- **本文件性質**：分析＋splice op 草案。**全程唯讀，兩棵樹皆未被本分析修改**
- **op 檔**：`D:\HT9045\docs\mg_w4_ops.json`（7 個 op，兩主題同檔，`desc` 標主題）

> ### ⚠ 併行狀態告示（分析當下 20260826 15:48 實測，主腦請先讀）
>
> `git status` 顯示 V910 樹有**兩支非本波的 `M` 檔**，且 `.mgbak` 已存在：
> `cObserver.cpp`、`note.cpp`（mtime 20260826 15:48，即本分析進行中被寫入；
> 新增的 AI 簽章是 `20260429` 與 `20260817`）
> → 這是 **LEDGER 相依鏈 #1「EventLog 引號線」波次正在跑**，不是本波產物、也不是殘留。
>
> **與 MG-W4 無檔案重疊**：本波三支目標檔 `main.cpp`／`HandlerSys.cpp`／`uMotorTest.cpp`
> mtime 仍是 20260716／20260701（pristine 基線），未被任何人改過 → **本文所有 V910 行號有效**。
>
> **但會影響驗收數字**：若該波先 commit，`ai_comment_matrix.py` 的 MISSING 起點就不再是 368，
> 且 §3-5 預測的「20260429 叢集剩 2 條」會變成 0（`note.cpp:6689/6702` 已被那波搬走）。
> → **主腦收工時請以當下實跑數字對帳，不要直接引用本文的 368→352。**

---

## 0. 前置事實（全部實測）

### 0.1 檔案基本盤

| 檔 | V899 行數 | V910 行數 | EOL | 套用後預測 |
|---|---|---|---|---|
| `main.cpp` | 33,575 | 35,227 | 兩樹**全 CRLF**（33575/33575、35227/35227） | **35,286**（+59） |
| `HandlerSys.cpp` | 1,368 | 1,531 | 兩樹**全 CRLF**（1368/1368、1531/1531） | **1,537**（+6） |
| `uMotorTest.cpp` | 2,371 | 2,376 | 兩樹**全 CRLF**（2371/2371、2376/2376） | **2,376**（±0，5→5 替換） |

（行數以 `splitlines(keepends=True)` 計，與 `port_check.py` 同口徑。）

### 0.2 **最重要發現：兩個主題在 V910 都已經搬了一大半**

矩陣的 MISSING 只是**冰山尖端**。用 `mg_ai_matrix_all.csv` 反查同日期叢集，發現 20260423 這批工作
V910 已經吃進去大部分，只剩下面列的幾個點沒進去：

#### 主題 A — BootLog 機制**本體已在 V910 基線內**

| V899 位置 | V910 狀態 | 證據 |
|---|---|---|
| `Public\cBootLog.cpp`（實作） | **已存在** | `cmp` V899 vs V910 → **位元組完全相同** |
| `Public\cBootLog.h`（宣告） | **已存在** | `cmp` → **位元組完全相同** |
| `HT9045.bpr` 專案登錄 | **已存在** | `..\Obj\cBootLog.obj`＋`<FILE FILENAME="Public\cBootLog.cpp" UNITNAME="cBootLog" .../>` |
| `HT9045.cpp` WinMain 全部 8 個埋點 | **已存在** | 矩陣 8 條全 `same-file` |
| `INCLUDEPATH` 含 `Public` | **已存在** | `HT9045.bpr:184` |
| **`main.cpp` 5 個埋點** | **缺** | ← **本波唯一工作** |

`git log -- .../Public/cBootLog.cpp` → `e06524a`（V910 pristine 基線）。
→ **不需要新增檔案、不需要動 `.bpr`、不需要改 INCLUDEPATH。** 本波只動 `main.cpp` 一支。

#### 主題 B — LOAD_Y stepper 家族的設定/載入/存檔/派工**全部已在 V910**

| 環節 | V910 位置 | 狀態 |
|---|---|---|
| 全域宣告 `LOAD_Y_USE_MOTOR[9]` | `cmydef.cpp:3070` / `cmydef.h:2854` | **已存在**（非本波新增） |
| ini → 記憶體（開機載入） | `database.cpp:771-776`（`[0]`～`[5]` 六條全在） | **已存在** |
| 記憶體 → checkbox（開表單） | `HandlerSys.cpp:198-203` | **已存在** |
| Cassette 衝突強制關閉守衛 | `HandlerSys.cpp:608-614` | **已存在** |
| checkbox → ini（按 OK 存檔） | `HandlerSys.cpp:616-621` | **已存在** |
| `.h` checkbox 宣告 `chkAuto3Y` 等 | `HandlerSys.h:451-456` | **已存在** |
| 運轉期派工 `LOAD_Y_USE_MOTOR[iAxis]` | `asendic.cpp:1338/1438/1457` | **已存在** |
| **checkbox → 記憶體（按 OK 立即套用）** | 無 | **缺** ← 本波 |
| **uMotorTest 手動測試列 enable** | `uMotorTest.cpp:204-208` 寫死 `false` | **缺** ← 本波 |

**推論（重要，決定風險等級）**：`database.cpp` 開機讀 ini 那六條已在 V910，`asendic.cpp` 派工也在，
所以 **empty/color/auto1–3 的 Y 軸步進馬達在 V910 早就是「ini 打開就會動」的活功能**。
本波補的不是「讓馬達開始動」，而是：
1. 按下 HandlerSys 的 OK 之後**立即生效**（現況要重開程式才生效，因為 ini 已經有寫）
2. uMotorTest 手動測試畫面**看得到／驅動得了**這五顆馬達（現況 MEmptyY/MColorY/MAuto3Y 完全叫不出來，
   MAuto1Y/MAuto2Y 只在 Cassette 模式叫得出來）

→ 主題 B 的實際風險比字面上的「啟用五顆馬達」**低很多**，屬於補齊 UI／apply 路徑。

### 0.3 祖先樹回溯（判斷「V910 是刻意移除還是從沒進去」）

| 樹 | `LOAD_Y_USE_MOTOR[0]=chkLoaderY` | `LOAD_Y_USE_MOTOR[1]=chkEmptyY` | `chkAuto3Y` 出現數 |
|---|---|---|---|
| `896.0_20260204_Jimmy_20260310_AI`（變更前基準） | 0 | 0 | 0 |
| `899.0_..._Jimmy_20260422`（**搬移來源**） | **1** | **1** | 3 |
| `903.0_20260417_Jimmy_20260428`（同作者下一版，佐證） | 1 | 1 | 3 |
| `905.8_20260608_Ken_NEW_Ifor_Jimmy`（公司線） | 0 | 0 | 2 |
| `906.0_20260618`（公司線） | 0 | 0 | 2 |
| `910.0_20260716_Jimmy`（**搬移目標**） | 0 | 0 | 2 |

→ 整段 apply block（含 `[0]` Loader 那行）在 896／905.8／906／910 **從來沒有存在過**。
**不是 V910 反向刪除，是公司線從沒合進去。** 依「final-state diff」原則直接搬 V899 最終態。

### 0.4 事前模擬（記憶體內套用，未落盤）

依 `mg_w4_ops.json` 全 7 個 op 模擬套用：

| 檔 | 行數 | CRLF 一致 | cp950 解碼錯誤 | payload 與 V899 位元組相同 |
|---|---|---|---|---|
| `main.cpp` | 35,227 → **35,286** | 35,286/35,286 | **0** | 5/5 op **True** |
| `HandlerSys.cpp` | 1,531 → **1,537** | 1,537/1,537 | **0** | 1/1 op **True** |
| `uMotorTest.cpp` | 2,376 → **2,376** | 2,376/2,376 | **0** | 1/1 op **True** |

逐 op 落點上下文已人工目視確認（7/7 落在正確函式／正確敘述之間）。

**矩陣簽章落地驗證**：把 16 條 MISSING 各自的 V899 原行（`//AI(` 起至行尾，去尾端空白）
拿去模擬產物做位元組搜尋 → **16/16 命中**。
→ 收工重跑 `ai_comment_matrix.py` 後，MISSING 應由 **368 降到 352**。

### 0.5 依賴符號在 V910 的存在性（**逐一驗過，並用 bcc32 實編背書**）

先逐一 grep 定位，再把兩主題所有新引入的敘述寫成一支 probe TU，**用 V899 一模一樣的運算式**、
只吃 V910 的標頭，跑 `bcc32 -c -H- -w- -D_VER6`（obj 導到 scratchpad，不碰共用 `Obj\`）：

> `probe_w4.cpp` → **編譯零錯誤零警告，產出 26,405 bytes .obj**

| 符號 | V910 出處 | 驗證 |
|---|---|---|
| `WriteBootLog(AnsiString)` / `(AnsiString, AnsiString)` | `Public\cBootLog.h:19,20` | 兩支 overload 皆在；`.h` 與 V899 **位元組相同** |
| `cBootLog.h` 可被 `main.cpp` 解析 | `HT9045.bpr:184` INCLUDEPATH 含 `Public` | 同樹 `HT9045.cpp:126` 已如此 include |
| `Sen[]`（`TMySensor`） | `mysensor.h:34` `extern TMySensor Sen[MAX_SENSOR_ITEM]` | 兩樹同宣告 |
| `TMySensor::IsOff()` | `mysensor.cpp:119` | **函式本體與 V899 位元組相同**（1198/1198 bytes） |
| `SnMotorPower` / `SnSystemPower` | `cmydef.h:758,759` 宣告；`cmydef.cpp:899,900` 定義（87 / 88） | V910 既有三處同款用法：`ckernel.cpp:1039`、`csystem.cpp:1487`、`Command.cpp:7406` |
| `SW[]`（`TMySwitch`） | `myswitch.h:27` | 兩樹同宣告 |
| `TMySwitch::OutValue` | `myswitch.h:15`（`public:` 區段內 `bool`） | ✓ |
| `SwMotorRelay` / `SwServerON` | `cmydef.h:1745,1765` | V910 既有用法 `main.cpp:7171,9730` `SW[SwServerON].On()` |
| `MessageBox(Application->Handle, …)` ＋ `MB_TOPMOST` | Win32／VCL | V899 `main.cpp` 全檔僅 **1 處**＝本波這段，已出貨背書；`TfMain : public TForm` 兩樹同宣告（`main.h:61`）→ 未限定名稱同樣解析到全域 `::MessageBox` |
| `Exception::HelpContext` / `Message` / `ClassName()` | VCL | V910 `main.cpp:26876` 已用 `E->HelpContext + ":" + E->Message` |
| `SOFT_SIMULTE` | `MachineType.h` | 兩樹同機制 |
| `LOAD_Y_USE_MOTOR[9]` | `cmydef.cpp:3070` / `cmydef.h:2854` | 兩樹同宣告同長度 |
| `chkLoaderY`/`chkEmptyY`/`chkColorY`/`chkAuto1Y`/`chkAuto2Y`/`chkAuto3Y` | `HandlerSys.h:451-456` | **六個全在**（`chkAuto3Y` 是 20260423 帶進去的，已 `same-file`） |
| `MEmptyY` / `MColorY` / `MAuto1Y` / `MAuto2Y` / `MAuto3Y` | V910 `uMotorTest.cpp:204-208` 現行碼已在用 | ✓ |
| `USE_LdUldCassetteMode` | V910 `uMotorTest.cpp:194,195,202,206,207` 現行碼已在用 | ✓ |
| `TMotorTestClass` / `MotorTestClass.push_back` | V910 `uMotorTest.cpp` 現行碼 | ✓ |

**結論：本波不需要補任何旗標定義、不需要新增檔案、不需要動 `.bpr` 或 `Gerneral.ini` 讀取處。**
（原任務書預設「若 V910 缺 `EMPTY_Y_USE_MOTOR` 等定義就把定義處納入本波」——實測**不需要**，
`database.cpp:771-776` 六條 ini 讀取全在 V910。）

---

## 主題 A：BootLog 開機診斷（24V-off 崩潰鑑識）

> 功能語意：客戶回報 24V 沒開時軟體開機即死，`LogSoftwareOnTime()` 受 `DEBUG_SOFTWARE_EXEC_TIME`
> 閘控且用 `TMyStringList`（早期 init 期自己可能丟例外），所以另做一支純 Win32 檔案 I/O 的
> `BootLog.txt`（512KB 自動輪替到 `.old`），在最早期就能落地、且撐得過 VCL 例外。
> **V910 已經有整套機制與 `HT9045.cpp` 的埋點，本波只補 `main.cpp` 這條路徑的 5 個埋點。**

### A1 — `main.cpp` 檔頭 include（矩陣 `main.cpp:6`）→ **OP-A1**

- **V899 範圍**：`main.cpp:6-8`（AI 註解 ＋ `#include "cBootLog.h"` ＋ 空行，3 行）
- **V910 錨點**：`main.cpp:6` = `#include "InOutArmZteach.h"` → 全檔命中 **1 次**，純 ASCII
- **模式**：`before`（插在 6 之前，落在 `#include "main.h"`＋空行之後）
- **模擬結果**：落於 sim 6–8，其後接原 `InOutArmZteach.h`，與 V899 檔頭排列**完全一致**
- **語意**：純 include。位置在 `#pragma hdrstop`（第 2 行）之後，與 V899、與 V910 `HT9045.cpp` 同款。
- **風險**：無。`cBootLog.h` 只 `#include <vcl.h>`，有 include guard（`cBootLogH`），無巨集污染。
- **信心**：**≥95%**

### A2 — `TfMain::FormShow` 進入 checkpoint（矩陣 `main.cpp:8567`）→ **OP-A2**

- **V899 範圍**：`main.cpp:8567-8568`（AI 註解 ＋ `WriteBootLog("TfMain::FormShow Enter");`）
- **V910 錨點**：`main.cpp:9215` = `    bNeedClearFile=false;`（函式第一句）
  - 行號由**全檔唯一**的 `void __fastcall TfMain::FormShow(TObject *Sender)`（`main.cpp:9213`）釘死
  - `bNeedClearFile=false;` 全檔 2 次（9215、32302），錨在 9215 由上述唯一簽名保證正確
- **模式**：`before`（插在函式 `{` 之後、第一句之前）
- **模擬結果**：sim 9218–9219，緊接 `{`（9217），其後 `bNeedClearFile=false;`（9220）→ 與 V899 相同
- **語意**：純寫檔 log，無控制流。
- **風險**：無。
- **信心**：**≥95%**

### A3 — `FormShow_Finish` 24V 未開機友善提示（矩陣 `main.cpp:10737` ＋ `10743`）→ **OP-A3**

- **V899 範圍**：`main.cpp:10736-10778`（空行 ＋ 6 行 20260423 註解 ＋ 5 行 20260429 gate 註解 ＋
  `#ifndef SOFT_SIMULTE { … } #endif` 整塊，共 **43 行**）
- **V910 錨點**：`main.cpp:11436` = `LogSoftwareOnTime("TfMain, FormShow_Finish");` → 全檔命中 **1 次**
- **模式**：`after`（插在 11436 之後、函式收尾 `}`（V910:11437）之前）——與 V899 位置完全對應
- **兩日合併理由**：20260429 的 gate（`bSwCutByCode`）不是新區塊，是**在同一段裡加變數、加
  `&& !bSwCutByCode` 條件、加 `else if(bSwCutByCode)` 分支**。用 `903.0_20260417_Jimmy_20260428`
  樹（0429 之前）比對確認：該樹有 `24V Motor / System Power is OFF` 與
  `FormShow_Finish power-state check`，但 **`bSwCutByCode` 0 筆**。
  → 依「final-state diff，不重播歷史」，一次搬 0429 之後的最終態，**中間的無 gate 版本不搬**。
- **語意**：
  - 讀 `Sen[SnMotorPower]` / `Sen[SnSystemPower]`，把電源狀態寫進 BootLog（一律寫，供事後鑑識）
  - 只有在「感測器說沒電」**且**「不是程式自己切掉繼電器」時，才彈一次 `MB_OK` 提示
  - `bSwCutByCode` 判 `SW[SwMotorRelay].OutValue==false || SW[SwServerON].OutValue==false`，
    避開 `cinitial`／`uhome` 為了保護殘料而主動斷電的情境（那條路徑本來就會跳「請取出 IC」對話框）
- **不動**：`LogSoftwareOnTime` 那行、函式收尾 `}`、其後 `FormClose` 全部原樣。
- **風險評估**（本波唯一有機台行為的一段，逐項查證）：
  1. **會不會在沒裝這兩顆 sensor 的機台亂跳提示？** → **不會**。
     `TMySensor::IsOff()`（`mysensor.cpp:119`，與 V899 **位元組相同**）在 `Enable==false` 時
     `State=-1; return false;` → `bMotorPwrOff`／`bSystemPwrOff` 皆為 `false` → 條件不成立、不彈窗。
  2. **控制流**：整段包在 `#ifndef SOFT_SIMULTE` 內，且位於函式**最後**、`}` 之前，
     無 `return`／無 `Action=` 改寫，不影響後續流程。
  3. **modal 彈窗**：`MB_OK | MB_ICONWARNING | MB_TOPMOST`，是 OP 面前的開機提示（非背景批次），
     且與 V899 出貨版一致。
  4. **是否 D 類（安全關鍵）**：觸及「開機期電源狀態判讀」，判為 **D 類**；
     但**忠實搬運＋錨點乾淨**（policy §決策規則 D 類）→ 可執行，不進最終決策清單。
- **需 主腦 知悉（非阻塞，V899 既有瑕疵，忠實照搬不修）**：
  20260429 註解第 5–6 句寫「Also skip when `IO_CARD_TYPE==0` (SYN-TEK), because
  `CheckPCI_L112State()` in main timer handles 24V loss…」，但**程式碼裡沒有任何 `IO_CARD_TYPE` 判斷**
  （已 grep V899 `main.cpp:10730-10780` 全區段，0 筆）。這是 V899 的**註解承諾多於實作**，
  非搬移過程遺漏。依「忠實優先於寫得更好」原樣搬，若要補實作屬 V899 本身議題，應回 `ht9045-v899` 決定。
- **信心**：**≥95%**

### A4 — `AppException` 併寫 BootLog（矩陣 `main.cpp:25798`）→ **OP-A4**

> **本波唯一的合併（merge）決策，非逐字取代。主腦請重點複驗這一條。**

- **V899 範圍**：`main.cpp:25798-25806`（2 行 AI 註解 ＋ `try{…}catch(...){}` 共 **9 行**）。
  **不含** V899:25807 的 `return;`（V910 自己已經有）。
- **V910 現狀（刻意演進，不可覆蓋）**：`main.cpp:26870-26890` 的 `AppException` 已被
  **RogerYang 20260617 的 AV-log-flood 去重**改寫：
  - `26872-26874`：`extern AnsiString gLastMsgBoxStep;` ＋ `static AnsiString sLastMsg` ＋ `static int iSameCount`
  - `26876`：`sMsg = E->HelpContext + ":" + E->Message + " @MsgBoxStep=" + gLastMsgBoxStep;`
  - `26879-26885`：相同訊息 → `iSameCount++`，只在第 3 次與每 1000 次記一筆，然後 **early return**
  - `26888`：`RecordProcess(sMsg);`
- **為何不能用 replace**：V899 那段是「`RecordProcess(...)` 之後直接 BootLog」的**舊形狀**。
  若照 V899 取代 V910 的 `26872-26889`，會把 RogerYang 的去重機制整個刪掉＝**把 V910 的改進搬掉**。
- **採用作法**：`after` 錨在 `main.cpp:26888` = `    RecordProcess(sMsg);`
  （全檔命中 **1 次**；26883 是 `RecordProcess(sMsg + AnsiString().sprintf(...))`，不含 `RecordProcess(sMsg);`）
  → 插在 `RecordProcess(sMsg);` 與 `return;` 之間。**V899 的 9 行位元組原樣，不改一個字。**
- **模擬結果**：
  ```
  26936    RecordProcess(sMsg);
  26937    //AI(ht9045-v899) 20260423: also write to BootLog so root cause survives
  …
  26945    catch(...) { /* never re-throw from exception handler */ }
  26946    return;        ← V910 原有
  26947 }
  ```
- **語意差異（唯一，且方向正確）**：V899 每次例外都寫一行 BootLog；合併後**重複例外被去重**，
  只有「新的」例外會寫 BootLog。這對 BootLog 是**加分**：`BootLog.txt` 只有 ~512KB 就輪替
  （`cBootLog.cpp` `BOOT_LOG_MAX`），若被 Timer 例外洗版，開機鑑識證據反而會被沖掉。
  首次發生仍完整記錄（第一次 `sMsg != sLastMsg`，必定落到 `RecordProcess(sMsg)` 之後）。
- **`E` 在 V910 仍在作用域**：簽名 `AppException(TObject *Sender, Exception *E)` 兩樹相同；
  `E->HelpContext` / `E->Message` / `E->ClassName()` 三者 V910 同函式 `26876` 已在用。
- **風險**：`catch(...)` 全包，`WriteBootLog` 本身宣告 never throws，不會從例外處理器再丟例外。
- **信心**：**≥95%**（合併路徑唯一、錨點唯一、payload 位元組不變；但屬**合併決策**，列為本波第一複驗點）

### A5 — `TfMain::FormCreate` 結束 checkpoint（矩陣 `main.cpp:25830`）→ **OP-A5**

- **V899 範圍**：`main.cpp:25830-25831`（AI 註解 ＋ `WriteBootLog("TfMain::FormCreate Done");`）
- **V910 錨點**：`main.cpp:26911` = `    BL=new TList;`（函式最後一句）→ 全檔命中 **1 次**
- **模式**：`after`（插在 26911 之後、`}`（26912）之前）
- **模擬結果**：sim 26969–26970，其後緊接 `}`（26971）→ 與 V899 相同
- **語意**：純寫檔 log。用途是「`FormCreate` 有沒有走完而沒丟例外」的分水嶺。
- **風險**：無。
- **信心**：**≥95%**

### 主題 A 小結

- 5 個 op、全在 `main.cpp` 一支、payload 合計 59 行（3+2+43+9+2）
- **BootLog 機制本體（`cBootLog.cpp/.h`、`.bpr` 登錄、`HT9045.cpp` 埋點）V910 已有且位元組相同 → 本波不碰**
- 唯一需要判斷的是 A4 的合併；其餘 4 個是純插入的診斷 log

---

## 主題 B：LOAD_Y stepper 家族啟用（empty／color／auto1-3 Y 軸步進馬達）

> 背景：`LOAD_Y_USE_MOTOR[0..5]` 對應 Loader／Empty／Color／Auto1／Auto2／Auto3 六個進出 Tray 站的
> Y 軸步進馬達。20260423 的工作是把原本只有 Loader（index 0）有的機制，展開到其餘五站。
> **§0.2 已證明：ini 讀取（`database.cpp:771-776`）、checkbox 載入、存檔、Cassette 守衛、
> `asendic.cpp` 派工全部已在 V910。本波只補 apply 與 motor-test 兩處。**

### B1 — `HandlerSys` 按 OK 立即套用（矩陣 `HandlerSys.cpp:519-523`）→ **OP-B1**

- **V899 範圍**：`HandlerSys.cpp:518-523`（**6 行**：`LOAD_Y_USE_MOTOR[0]`～`[5]` ＝ 六個 checkbox）
- **V910 錨點**：`HandlerSys.cpp:639` = `    LOAD_Z_USE_MOTOR[8]=chkAuto6->Checked;`
  → 全檔命中 **1 次**，純 ASCII
- **模式**：`after`（插在 Z 軸 apply 區塊之後、空行＋`LOADUNLOAD_USE_CASSETTE[0]` 之前）
  → 與 V899 的 517→518→…→523→空行→525 排列**完全一致**
- **⚠ 範圍超出矩陣一行，理由如下（請 主腦 裁示，我判定應納入）**：
  矩陣只列 519–523（`[1]`～`[5]`），因為 518 那行掛的是舊註解 `//Jimmychiu 20240307`，
  不符 `//AI(` 簽章格式所以矩陣抓不到。但 §0.3 已證明：
  - `LOAD_Y_USE_MOTOR[0]=chkLoaderY->Checked;` 在 **896／905.8／906／910 皆 0 筆，只有 899／903 有**
  - 亦即 **V910 完全沒有這段 apply block**，不是「有 [0] 缺 [1..5]」
  - 只搬 519–523 會產生 `[1]`~`[5]` 立即生效、`[0]` 要重開程式才生效的**不對稱缺口**，
    與 V899 最終態不符，也不符「final-state diff」原則
  - 該行行為安全：`chkLoaderY->Checked` 本身就是開表單時由 `LOAD_Y_USE_MOTOR[0]` 載入的
    （`HandlerSys.cpp:198`），不動它按 OK 是**冪等**的；且同一函式 `616` 行早就把它寫進 ini 了
- **模擬結果**：sim 640–645，六行齊全，其後空行＋`LOADUNLOAD_USE_CASSETTE[0]`
- **執行順序（已查證，是安全的）**：本 op 落在 V910 既有的 Cassette 守衛（`608-614`）**之後**。
  該守衛在 `USE_LdUldCassetteMode==1` 時會把 `chkAuto1Y`／`chkAuto2Y` 強制取消勾選，
  所以 apply 時 `LOAD_Y_USE_MOTOR[3]/[4]` 必得 `false` → **不會與 Boat Carrier 搶 MAuto1Y/MAuto2Y**。
  順序與 V899（守衛 486-492 → ini 494-499 → apply 518-523）**完全相同**。
- **行為差異**：只有「按 OK 之後不必重開程式」。ini 寫入行為 V910 早已相同（`616-621`），
  開機讀取行為 V910 早已相同（`database.cpp:771-776`）→ **持久化語意零變更**。
- **是否 D 類**：觸及運動控制致能旗標 → 判為 **D 類**；忠實搬運＋錨點乾淨 → 可執行。
- **信心**：**≥95%**

### B2 — `uMotorTest` 手動馬達測試列（矩陣 `uMotorTest.cpp:204-208`）→ **OP-B2**

- **V899 範圍**：`uMotorTest.cpp:204-208`（5 行）
- **V910 被取代行段**：`uMotorTest.cpp:204-208`（5 行，**行號兩樹恰好相同**）
- **anchor_expect**：`new TMotorTestClass(MEmptyY,` → 全檔命中 **1 次**，純 ASCII
- **模式**：`replace`（5 行 → 5 行，行數不變）
- **逐行對照**：

  | 馬達 | V910 現行 enable 條件 | V899（搬入後） | 方向 |
  |---|---|---|---|
  | `MEmptyY` | `false` | `LOAD_Y_USE_MOTOR[1]` | 放寬 |
  | `MColorY` | `false` | `LOAD_Y_USE_MOTOR[2]` | 放寬 |
  | `MAuto1Y` | `USE_LdUldCassetteMode==1` | `(USE_LdUldCassetteMode==1 \|\| LOAD_Y_USE_MOTOR[3])` | 放寬（**嚴格超集**） |
  | `MAuto2Y` | `USE_LdUldCassetteMode==1` | `(USE_LdUldCassetteMode==1 \|\| LOAD_Y_USE_MOTOR[4])` | 放寬（**嚴格超集**） |
  | `MAuto3Y` | `false` | `LOAD_Y_USE_MOTOR[5]` | 放寬 |

  → **五條全是嚴格放寬，沒有任何一條會把 V910 目前已啟用的測試列關掉。**
  （`MAuto1Y`／`MAuto2Y` 的 Cassette 條件原封保留在 `||` 左側。）
- **不動**：`MLoaderY`（`200-202`）**完全不碰** —— 見下方「out-of-scope 觀察」。
- **語意**：`TMotorTestClass(motor, bEnable)` 只決定手動測試畫面上該顆馬達能不能被選取／驅動。
  這是**工程模式 UI**，不影響自動運轉流程（自動流程走 `asendic.cpp`，V910 已在讀同一組旗標）。
- **風險**：低。啟用條件與自動流程用的是**同一個旗標**，不會出現「自動會動但測試叫不出來」
  或反過來的不一致。
- **信心**：**≥95%**

### out-of-scope 觀察（**本波不動，記錄備查**）

`uMotorTest.cpp` 的 `MLoaderY`（V899 `200-202` / V910 `200-202`）兩樹已**分岔**：

- V899：`((INSTALL_OCR_YMot==eocrYMotInstal && CosFunction.bTrayOCR) || USE_LdUldCassetteMode==1 || LOAD_Y_USE_MOTOR[0]==true)`
- V910：`((INSTALL_OCR_YMot==eocrYMotInstal || (INSTALL_OCR!=eocrUninstal && CosFunction.bTrayOCR) || USE_LdUldCassetteMode==1))`
  ← 含 **KenHsieh 20260514「掛上OCR+夾帶」**（比 20260423 晚）

V910 這條**沒有** `LOAD_Y_USE_MOTOR[0]`，且 `INSTALL_OCR_YMot` 那項的 `&&` 已被公司改成 `||` 拆項。
兩者是**不同時期、不同人的刻意演進**，合併需要改寫運算式（非 splice 可及），
且該行**無 `//AI(` 簽章、不在矩陣母體內**。
→ **本波不碰**。若日後要讓 `LOAD_Y_USE_MOTOR[0]` 也影響 MLoaderY 測試列，屬獨立議題，建議另案。

### 主題 B 小結

- 2 個 op、payload 合計 11 行（6+5）
- **不需要補任何旗標定義**：`EMPTY_Y_USE_MOTOR` 等 5 個 ini 鍵的讀取（`database.cpp:771-776`）、
  `LOAD_Y_USE_MOTOR[9]` 全域（`cmydef.cpp:3070`/`cmydef.h:2854`）、六個 checkbox 宣告
  （`HandlerSys.h:451-456`）**在 V910 全部已存在**

---

## 2. 客戶碼隔離需求評估

**結論：兩個主題都不需要 `CUSTOMER_CODE` / `FUNC_CC_*` 隔離。** 逐項查證：

1. **來源就沒有客戶碼**：V899 這 16 條變更全部**沒有**任何 `CUSTOMER_CODE` / `FUNC_CC_*` 條件，
   矩陣 comment 欄也無 CASE-ID（同日的 `AutoClean\uCleaning.cpp` 那 4 條才是 `CASE-20260423-001` /
   Greatek，且已在 MG-W1 裁決為 `skipped-C`，**不屬本波**）。
2. **主題 A 的閘門是機台狀態，不是客戶**：`#ifndef SOFT_SIMULTE` ＋ 感測器實際讀值。
   沒裝該感測器的機台 `IsOff()` 回 `false`（§A3 風險 1）→ 天然不觸發。
3. **主題 B 的閘門是機台配置，不是客戶**：`LOAD_Y_USE_MOTOR[n]` 由 `Gerneral.ini` `[TrayY]` 段
   的 `EMPTY_Y_USE_MOTOR` 等鍵決定，預設 `false`（`CheckAndReadIniDataGeneral(..., false)`）。
   → 沒設定的機台**行為完全不變**，預設路徑＝V910 原行為，符合 policy「預設路徑＝V910 原行為」。
4. **無回傳值／控制流變更**：主題 A 的 5 段全部無 `return`；主題 B 是賦值與建構參數。
5. **時序**：主題 A 只新增檔案 I/O（`WriteBootLog`）與一個條件式 MessageBox；
   主題 B 不新增任何呼叫，只改既有 `push_back` 的布林引數與新增 6 行賦值。

**唯一需記入 LEDGER 的外溢**：
- `D:\HT9045\Error\BootLog.txt` 開始有 `main.cpp` 路徑的紀錄（V910 目前只有 `HT9045.cpp` WinMain 那幾筆）。
  這正是 V899 出貨行為。檔案有 512KB 自動輪替，不會無限長。
- 20260810 那波（`main.cpp:25566`，尚未搬）會把 `BootLog.txt*` 收進 State Record 打包。
  **不是本波前置條件**，但兩者是同一條線，主腦排序時可留意。

---

## 3. 交付與收工 gate 建議（給 主腦）

1. `python tools/port_tools/splice.py docs/mg_w4_ops.json`
   - 預期：`OK main.cpp: 5 op(s) applied, eol=CRLF` / `OK HandlerSys.cpp: 1 op(s) applied, eol=CRLF` /
     `OK uMotorTest.cpp: 1 op(s) applied, eol=CRLF`
   - 三檔各產生 `.mgbak`
2. `python tools/port_tools/port_check.py main.cpp HandlerSys.cpp uMotorTest.cpp`

   **預期輸出（已用 port_check 自身的分類邏輯對模擬結果實跑，非估計）**：
   ```
   [main.cpp]       added=59 (spliced=59, comment=0, authored=0) removed=0 | EOL (35227,0)->(35286,0)
   [HandlerSys.cpp] added=6  (spliced=6,  comment=0, authored=0) removed=0 | EOL (1531,0)->(1537,0)
   [uMotorTest.cpp] added=5  (spliced=5,  comment=0, authored=0) removed=5 | EOL (2376,0)->(2376,0)
       REMOVED    204:     MotorTestClass.push_back(new TMotorTestClass(MEmptyY,        false));
       REMOVED    205:     MotorTestClass.push_back(new TMotorTestClass(MColorY,        false));
       REMOVED    206:     MotorTestClass.push_back(new TMotorTestClass(MAuto1Y,        USE_LdUldCassetteMode==1));
       REMOVED    207:     MotorTestClass.push_back(new TMotorTestClass(MAuto2Y,        USE_LdUldCassetteMode==1));
       REMOVED    208:     MotorTestClass.push_back(new TMotorTestClass(MAuto3Y,        false));
   == port_check PASS ==
   ```
   - `authored=0`：本波**沒有任何自撰程式行**，70 行新增全部是 V899 位元組原樣
   - `comment=0`：純 splice 波不新增 `//AI(mg899to910)` 註解（與 MG-W2 同慣例；
     來源 899 註解原樣保留，本身就是矩陣驗收簽章）
   - 「未變更行位元組被改動」檢查：模擬結果 **0 筆**
   - `REMOVED` 只有 uMotorTest.cpp 那 5 行，即 §B2 表中「V910 現行 enable 條件」那一欄，
     全部是被**放寬**取代，非功能刪除
3. `sh tools/port_tools/bcc_syntax.sh main.cpp HandlerSys.cpp uMotorTest.cpp`
   - **⚠ 開工前先查**：本分析當下 `tasklist` 顯示 **`bcb.exe` PID 25268 正在執行**。
     單檔 `bcc32 -c -n<scratchpad>` 不碰共用 `D:\HT9045\Obj`，安全；
     但**任何全量 build 前必須先確認 IDE 已關**（policy 硬邊界）。
4. **主腦逐條開檔複驗**，優先序：
   1. **OP-A4**（唯一的合併決策：確認 RogerYang 20260617 去重的 `26872-26887` 一行未動，
      且 BootLog 落在 `RecordProcess(sMsg);` 與 `return;` 之間）
   2. **OP-B1**（確認搬了 6 行含 `[0]`，且落在 Cassette 守衛之後）
   3. **OP-A3**（確認 `#ifndef SOFT_SIMULTE` / `#endif` 配對完整、函式收尾 `}` 未被吃掉）
   4. 其餘 4 個純插入 op
5. 收工後 `python tools/port_tools/ai_comment_matrix.py`
   - MISSING 應由 **368 → 352**（減 **16** 條：20260423 的 15 條 ＋ 20260429 的 1 條）
   - **20260423 叢集：19 → 4**。剩下的 4 條是 `AutoClean\uCleaning.cpp:148-151`，
     已在 **MG-W1 裁決為 `skipped-C`**（V910 已演進成 ArmSpeed 動態預設），**不是本波遺漏**
   - **20260429 叢集：3 → 2**。剩下的 2 條是 `note.cpp:6689/6702`，屬 LEDGER 相依鏈 #1
     「EventLog 引號線」，**必須與 20260817 `cObserver.cpp` 同波搬**，不在本波

---

## 4. 結論

- **16 條矩陣紀錄 → 7 個 splice op**（主題 A 5 個、主題 B 2 個），
  6 個 `after`/`before` 純插入 ＋ 1 個 `replace`，錨點**全為全檔唯一的純 ASCII 字串**。
  16 條簽章模擬套用後 **16/16 命中**；MISSING 368 → 352。
- **無 <95% 項目。** 無需寫入 `MG_FINAL_DECISIONS.md`。
- **兩個主題的「機制本體」在 V910 都已存在**（BootLog 三件套位元組相同且已登錄 `.bpr`；
  LOAD_Y 的 ini 讀取／checkbox 載入／存檔／Cassette 守衛／`asendic` 派工全在），
  本波只補 5 個 `main.cpp` 埋點與 2 處 apply/UI 路徑 → **不需新增檔案、不需動 `.bpr`、
  不需補任何旗標定義、不需動 `Gerneral.ini`**。
- 記憶體模擬 7/7 op payload 與 V899 **位元組相同**，cp950 解碼 0 錯誤，CRLF 全一致，落點上下文已目視確認。
- 依賴符號 **17 項逐一驗過**，並以 probe TU 對 V910 標頭實跑 `bcc32 -c` **零錯誤**背書。
- 本波**不需要客戶碼隔離**（兩主題閘門皆為機台配置／感測器實值，預設 `false`＝V910 原行為）。
- 唯一需要人判斷的是 **OP-A4 的合併**（保留 V910 RogerYang 20260617 AV-log-flood 去重），
  以及 **OP-B1 多搬一行 `LOAD_Y_USE_MOTOR[0]`**（矩陣抓不到但屬同一段最終態，理由見 §B1）。
