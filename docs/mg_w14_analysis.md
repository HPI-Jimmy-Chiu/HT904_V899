# MG-W14 分析：PTI 0630/0703 案群（State Record 非阻塞化 ＋ Smart AutoClean 不鎖機 ＋ 超豐 TrayArm 極性校正）

- **波次**：MG-W14（V899 → V910），LEDGER 日期列 **20260630（17 條）** ＋ **20260703（25 條）**
- **來源**：`docs/mg_ai_matrix_missing.csv` **42 條 MISSING**
  （0630 的 `uLotInfo.cpp:1952` 已於 MG-W10 結清，故 LEDGER 記 18、現行 csv 只剩 17）
- **CASE**：`CASE-PTI-20260630-001`（低良率按 One Cycle 後 hang＝Smart AutoClean 鎖機）、
  `CASE-PTI-20260630-002`（Tray feed 退料完成後 Alarm 音消不掉／訊息框被蓋住）、
  另含 **超豐 Greatek TrayArm 夾爪極性校正**（日期撞號的他案，見 §1.4）
- **V899 樹**：`D:\HT9045\HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422`
  （唯讀；本波 12 個來源檔 `git status --porcelain` 空、`git diff b515ed5 HEAD` 對這些檔為空 → 工作區＝在製收斂端點 `b515ed5`）
- **V910 樹**：`D:\HT9045\HT9011UC_Code_V3.33.910.0_20260716_Jimmy`（MG-W13 `3860bee` 後，`git status` 乾淨、`*.mgbak` **0 個**）
- **本文件性質**：分析＋splice op 草案。**全程唯讀，兩棵樹一個位元組都沒有被修改**（模擬結果只寫進 scratchpad）
- **op 檔**：`D:\HT9045\docs\mg_w14_ops.json`（**34 個 op，12 個檔**）
- **類別**：**D 類（安全關鍵：鎖機閘門／State Record 收尾／TrayArm 夾爪動作）為主**。
  忠實搬運、錨點 33/34 全檔唯一 → 可執行。
  **<95% 項目：2 條**（acatchtray.cpp:1221、2007，詳見 §6.1）→ **本波可結清 40/42 條**

---

## ⚠ 開工前必須先知道的五件事

1. **「0703 叢集」不是一個案子，是兩個客戶的兩件事。**
   25 條裡只有 **9 條是力成 PTI 的 Smart AutoClean 不鎖機**（`594fce9`，V3.33.899.33），
   另外 **13 條是超豐 Greatek 的 TrayArm 夾爪 sensor 極性校正**（`b2ea143`／`cc7c6f9`／`6539e3b`／
   `89fdd2a`／`655feb2`／`0d2fc78`，commit 日期 07-03～07-10，但 `//AI` 日期都寫 20260703），
   再加 **2 條是 ART 夾爪誤報（無客戶碼，全客戶）**。LEDGER 的 case 歸屬必須分開寫（§7）。

2. **本波唯一的真衝突在 `acatchtray.cpp` case 570 的 `iCnt` 防抖。**
   V910 已經有一個**同名同型別同位置**的 `static int iCnt=0;`（`acatchtray.cpp:1550`，
   Eastsun 20260515 F017「KYEC iTrayDetectCount debounce」），且防抖判斷式是
   `(CUSTOMER_CODE!=CC_KYEC_LEE || iCnt>iTrayDetectCount) && bCoverTrayInProgress==false`
   （Eastsun 20260710 追加 `bCoverTrayInProgress` guard）。
   V899 要的是**無條件 `iCnt>100`**。兩者無安全合併解 → **矩陣 1221／2007 兩條進 `MG_FINAL_DECISIONS.md`**。
   **同一個 hunk 裡的「SKIP 後補釋放夾爪」（矩陣 2020）與防抖完全正交，已單獨切出（W14-27）照搬。**

3. **`main.cpp` 的 `StateRecordImage()` 在 V910 有反向刻意修改，整支函式不可覆蓋。**
   V910 有 `Steven 20260504 : Support 1920x1080 and 1280x1024`（`iScrW`/`iScrH`，case 1–4 用），
   V899 還是硬編碼 1280×1024。本波把 op 切成三刀（宣告區／`int iret`／case 6 內文），
   **case 1–5 一行不動**，`iScrW`/`iScrH` 出現次數 8→8（實測）。

4. **`uhome.cpp` 的 V910 中文註解比 V899 乾淨（V899 那幾行是 `嚙…` mojibake）。**
   `uhome.cpp:2889` V910 是 `//jou 2012-10-26 修正歸零catch tray 會把tray丟在中途`，
   V899 對應行是 `//jou 2012-10-26 嚙論伐蕭嚙糊嚙編catch tray …`。
   本波 W14-32/34 用「**在前後插入 `else {` 與 `}`、被包住的既有區塊一行不重插**」的形狀，
   因此 V910 的乾淨註解全數保留。**模擬後 12 個檔的 mojibake 計數與 U+FFFD 計數全部 0→0。**

5. **Smart AutoClean 解鎖沒有客戶碼閘，但有天然閘。**
   `CheckSmartAutoCleanCanStart()` 恆回 `true` 是**全客戶**生效的（V899 出貨現狀，照搬）。
   實務外溢僅及三個客戶：`CosFunction.bSmartAutoClean` 只在
   **`FUNC_CC_PTI` / `FUNC_CC_TERAPOWER` / `FUNC_CC_TeraProbe`** 設 true
   （`InitialCosFunction` 預設 false），它閘住 `ResetSmartAutoClean()` 本體與 note 面板。
   詳見 §4.1 與 §6.2。

---

## 0. 前置事實（全部實測）

### 0.1 檔案基本盤（**md5 前提**，套用前必須逐檔重驗）

| 檔 | V899 行數 / md5(12) | **V910 行數 / md5(12)** | EOL | cp950 | 被前波動過？ | 套用後 |
|---|---|---|---|---|---|---:|
| `cpublic.h` | 386 / `1e5a12587cb6` | **385 / `ad0d04361719`** | 兩樹全 CRLF | OK | 否 | **386** |
| `cpublic.cpp` | 2237 / `8e520e0b4eb3` | **2235 / `e1478dcf4a14`** | 兩樹全 CRLF | OK | 否 | **2288** |
| `csystem.cpp` | 23201 / `50973facff3f` | **25649 / `db440cd3906c`** | 兩樹全 CRLF | OK | 否 | **25690** |
| `main.cpp` | 33575 / `81596aa5852b` | **35350 / `5ea391b1c9f2`** | 兩樹全 CRLF | OK | **是（W4/W10/W12/W13）** | **35408** |
| `mymessbox.cpp` | 1343 / `cd775d8b749a` | **1400 / `b5f31ad83de9`** | 兩樹全 CRLF | OK | 否 | **1404** |
| `note.cpp` | 6818 / `248ef6db104b` | **6875 / `015d398c421a`** | 兩樹全 CRLF | OK | 否 | **6884** |
| `HS_Function.cpp` | 4838 / `a2fa16b35de3` | **5381 / `ccae6a71b287`** | 兩樹全 CRLF | OK | 否（W5 只驗未改） | **5395** |
| `AutoClean\uCleaning.cpp` | 2880 / `d227f5405e00` | **2923 / `0c52d8619868`** | 兩樹全 CRLF | OK | **是（W11 +2/−1）** | **2943** |
| `cmydef.cpp` | 5865 / `c01247d95559` | **6005 / `d6d7c2e34c0f`** | 兩樹全 CRLF | OK | 否 | **6009** |
| `cmydef.h` | 5804 / `b49a9f8ad9b8` | **6024 / `82da779722bc`** | 兩樹全 CRLF | OK | 否 | **6027** |
| `acatchtray.cpp` | 7798 / `d80798e8d34c` | **9338 / `effd6ba47e00`** | 兩樹全 CRLF | OK | **是（W9 +20）** | **9391** |
| `uhome.cpp` | 4910 / `56dfc1d9b3bd` | **5198 / `224385a138a2`** | 兩樹全 CRLF | OK | 否 | **5240** |

> 本文件所有 V910 行號都量在**上表 md5 對應的磁碟現狀**上，不是任何舊快照。
> V910 樹目前 `.mgbak` 為 0 個且 `git status -- <V910>` 乾淨 → `port_check.py` 會拿 `git HEAD` 當基準，與上表一致。
> **`acatchtray.cpp` md5 是 W14-27 的硬前提**（該 op 是本波唯一非全檔唯一錨，見 §2.6）。

### 0.2 範圍權威：V899 的七個來源 commit（`bb69c60..b515ed5`）

`git log -G "AI\(ht9045-v899\) 2026063\|20260703"` 對本波 12 個檔恰好落在下列 commit，無遺漏、無中間態被撤回：

| commit | 時間 | 主題 | 客戶 | 本波動到的檔 |
|---|---|---|---|---|
| `292bd72` | 06-30 14:18 | V3.33.899.32 力成PTI 三案修正＋累積出貨內容 | PTI | `main.cpp`、`csystem.cpp`、`cpublic.cpp/h`、`mymessbox.cpp`、`note.cpp`、`HS_Function.cpp` |
| `b2ea143` | 07-03 13:34 | ART 夾爪誤報：Home 幽靈盤告警化＋JAM0610 SKIP 補釋放夾爪 | **無閘（全客戶）** | `acatchtray.cpp`、`uhome.cpp` |
| `cc7c6f9` | 07-03 14:03 | case 570 JAM0610 補 `iCnt>100` 防抖 | **無閘（全客戶）** | `acatchtray.cpp` |
| `6539e3b` | 07-03 14:21 | 啟動自檢夾緊位改報 WAR0614 | 超豐 | `csystem.cpp` |
| `594fce9` | 07-03 16:25 | V3.33.899.33 力成PTI Smart AutoClean 不鎖機 | PTI | `uCleaning.cpp`、`cmydef.cpp/h`、`main.cpp`、`note.cpp` |
| `89fdd2a` | 07-09 11:41 | 夾爪 sensor 極性校正（Greatek 專用） | 超豐 | `acatchtray.cpp`、`csystem.cpp`、`uhome.cpp` |
| `655feb2` | 07-10 09:37 | 夾緊改 `On()` 不等到位 sensor（避 JAM31047） | 超豐 | `acatchtray.cpp` |
| `0d2fc78` | 07-10 13:17 | case 301 夾盤避 JAM0613（極性校正補完） | 超豐 | `acatchtray.cpp`、`csystem.cpp` |

> `292bd72` 是 29 檔的累積 commit，其中 FTPUpload／TesterTCP／cContact／ainarm 等內容
> 屬 MG-W4／W7／W9／W10，**已結清**；本波只取帶 `//AI … 20260630` 簽章的 hunk。
> 三個 TrayArm 檔的「0703 最終差」直接以 `git diff 292bd72 b515ed5 -- <3 檔>` 取得，
> 與逐 commit 疊加結果一致（該區間內只有上述 6 個 commit 動過它們）。

### 0.3 符號稽核（V910 全樹，排除 `.svn`）

**17 個新符號在 V910 命中數皆為 0** → 42 條 MISSING 判定成立，無「已搬一半」狀態，也不會撞名：

`ExecZipCommandHandle` / `g_hStateRecordBat` / `g_hStateRecordZip` / `g_dwStateRecordWaitStart` /
`g_iStateRecordStage` / `dwWaitRet` / `dwZipExit` / `sACSmartNoteEN` / `sACSmartNoteCH` /
`bACSmartNeedReset` / `iSubRetry` / `iDelRetry` / `iRmRetry` / `asDelFile` / `iRelDetCnt` /
`iFixOnDetCnt` / `sACAlmMsg`

> **唯一的例外就是 §⚠2 的 `iCnt`**——它在 V910 已存在（Eastsun 20260515），所以矩陣 1221 不能照搬。

新程式用到的**既有**符號全部存在且可見（逐一實測）：

| 符號 | V910 宣告／首見處 | 目標檔看得到嗎 |
|---|---|---|
| `HANDLE`（`cpublic.h` 新宣告的回傳型別） | `cpublic.h` **兩樹都沒有任何 `#include`**；由 includer 的 `MachineDefine.h`／`vcl.h` 鏈提供 | ✓ **實測 V910 include `cpublic.h` 的檔集合＝V899 的 85 檔，逐檔相同、無 V910 獨有者** → V899 出貨 build 即為背書 |
| `GetExitCodeProcess` / `WaitForSingleObject` / `CloseHandle` / `GetTickCount` / `WAIT_TIMEOUT` | Win32；`main.cpp:18083` 已在用 `GetExitCodeProcess`、全樹 8 處 `WaitForSingleObject` | ✓ |
| `RecordProcess` | 全樹 1421 處 | ✓ |
| `MySleep` | 全樹 340 處 | ✓ |
| `MyDBIProcess` | `cMyDB.h:20` | ✓ `HS_Function.cpp` |
| `HS_ERR_FTPUploadError` | `HS_Function.h:18` | ✓ |
| `GetMyDBIMessage` | `cMyDB.h:48` | ✓ `uCleaning.cpp`（同檔 2721 已在用） |
| `NewRecordProcess` | `cMyDB.h:62`（3 參數，第 3 個有預設值） | ✓（同檔 2723 已在用同形呼叫） |
| `ResetSmartAutoClean()` | `uCleaning.h:357` / `uCleaning.cpp:2743`，同檔 2738 已有無限定呼叫 | ✓ |
| `sACSmartNoteEN/CH`（note.cpp 消費） | 本波新增於 `cmydef.h`；`note.cpp:48` 有 `#include "cmydef.h"` | ✓ |
| `CC_PTI`(957) / `CC_Greatek`(956) | `MachineType.h:326` / `:325` | ✓ |
| `e3Fix4/5/6` | `MachineType.h:1169-1171` | ✓ |
| `C_CatchTray_FixOff` / `OnStatus()` / `__FUNC__` | 全樹 32／1444 處 | ✓ |
| `"WAR0614"` 警報碼 | V910 `uhome.cpp:4183` 已在用；資料層 `D:\HT9045\Error\{Chinese,English}\WAR0614.dat` **實體存在** | ✓ |
| `lblSmartCleanEN/CH`、`pnlCleanSocket`、`tsHandler` | `note.dfm` 既有（V910 `note.cpp:2009-2016` 已在用） | ✓ |

**`#include` 閉包：本波不需要新增任何 `#include`。**
**`.dfm`：本波 0 個 op**——新程式只用既有控制項（上表最後一列），
`292bd72` 動到的三個 `.dfm`（`cConfiguration`/`cSortCT`/`cTrayMapping`）屬其他主題，MG-W8 已鑑定為設計器分頁狀態噪音。

### 0.4 未標記承重行（矩陣看不到，漏搬即壞）

本波 34 個 op 共抽 **401 行** V899 位元組：

| 類別 | 行數 |
|---|---:|
| 矩陣列出的 `//AI` 標記行 | **40** |
| V910 原本就有、僅為取得唯一錨而一併重插的既有行（位元組相同、無功能變化） | **55** |
| **V899 新增但無 `//AI` 標記** → **矩陣盲區** | **306** |

**矩陣可見率僅 40/346 = 11.6%**（分母＝port_check 會判定為 added 的 346 行）。四塊最危險的「一條註解帶一整段實作」：

| V899 位置 | 內容 | 矩陣只列 | 漏搬的後果 |
|---|---|---|---|
| `cpublic.cpp:655-691` | `ExecZipCommandHandle()` 函式本體（37 行） | `:655` 一行 | `main.cpp` 兩個呼叫點 **link error** |
| `main.cpp:25226-25294` | case 6 分段非阻塞輪詢整段（69 行） | `:25226`、`:25260` 兩行 | **修正等於沒搬**（仍同步 `system()` 擋 UI） |
| `HS_Function.cpp:2020-2032` | `try` 的收尾 `}` ＋ 兩個 `catch`（13 行） | **一行都沒有** | W14-19 的 `try` 沒有收尾 → **編譯錯** |
| `uhome.cpp:2743-2748` | Greatek `else` 區塊的收尾 `}`（6 行） | **一行都沒有** | 括號不平衡 → **編譯錯** |

其餘大宗：`csystem.cpp` Del_Tree 三段重試迴圈（24 行）、`uhome.cpp` Greatek Home 分支（26 行）、
`acatchtray.cpp` 三處「SKIP 後釋放夾爪」各 9 行、`uCleaning.cpp` 觸發端提示（11 行）與
`CheckSmartAutoCleanCanStart` 尾段重置（14 行）。

> 本波所有 op 都以**連續區段**抽取，上述行自然被包含——這正是「用區段不用逐行」的理由。

---

## 1. 四個主題的變更敘述（搬什麼、為什麼）

### 1.1 State Record 非阻塞化（`292bd72`，0630）→ **W14-01～06、10～13**

**根因**（與 KNOWLEDGE `ht9045-staterecord-flow-and-leftover-fix` 一致）：
`DoStateRecord()` 與 `StateRecordImage()` **全程跑在 UI thread**，而 case 6 用同步 `system("7z …")`
壓縮整包。月底 log 量大時 7z 一跑數十秒，UI／SECS 30s timer／motion 全被凍住；
同時 `1.bat`（非同步 XCOPY 複製）與 case 6 的「延後刪夾」形成 race，留下同名殘留夾。

**最終態三件事**：
1. `cpublic.cpp` 新增 `ExecZipCommandHandle()`：與 `ExecZipCommand` 同樣 `CreateProcess`，
   但**關掉 thread handle、回傳 process handle**，讓呼叫端能 `WaitForSingleObject(h,0)` 非阻塞輪詢。
   （原 `ExecZipCommand` 的 handle 洩漏**刻意不一併修**——V899 註解裡寫明了。）
2. `main.cpp` 檔案層新增 4 個 static（`g_hStateRecordBat`／`g_hStateRecordZip`／
   `g_dwStateRecordWaitStart`／`g_iStateRecordStage`），`DoStateRecord()` 啟動 1.bat 時保留 handle
   並先清掉前一次未收尾的 handle/stage；case 6 改成 stage0（顯示 alarm）→ stage1（輪詢 1.bat，
   完成後**非同步**啟動整包 7z）→ stage2（輪詢 7z，`exit==0` 才 `Del_Tree`）。
   每 stage 未完成就 `break` 讓 timer 下一拍重入，並以 `GetTickCount` 設 **300 秒**上限保護。
3. `csystem.cpp` `Del_Tree()` 三處加**最多 3 次、每次間隔 100ms** 的重試
   （子目錄 `RemoveDir`、檔案 `DeleteFile`、最後 `RemoveDir`），解決「檔案被占用→靜默略過→殘缺同名夾」。

**行為保持**：`if(dwZipExit==0)` 完全對應原本的 `if(iret==0)` 才刪目錄，安全條件不變。

### 1.2 訊息框 z-order（`292bd72`，CASE-PTI-20260630-002）→ **W14-15、16、18**

Tray feed 退料完成後 Lot End 訊息框被其他視窗蓋住，操作員看不到 → 以為 Alarm 音消不掉。
三處修正**都只用 app 內置頂，刻意不用 `HWND_TOPMOST` / `SetForegroundWindow`**（以免蓋住 app 外視窗）：
- `TMyMessageBox::FormShow` 尾端 `SetWindowPos(Handle, HWND_TOP, …, SWP_NOACTIVATE)`（不搶焦點）
- `ShowLotEndMessage()` 在 `ShowModal()` 前 `MyMessageBox->BringToFront()`
- `TfNote::Timer1Timer` 既有置頂鏈（`fPassword2`→`fPassword`→`fCCLink`→`fNote`）**尾端補一支 `else`**：
  `MyMessageBox->fShow==true` 時改拉 MyMessageBox 到前（原本這種情況什麼都不做）

### 1.3 PTI Smart AutoClean 不鎖機（`594fce9`，CASE-PTI-20260630-001）→ **W14-14、17、22～25**

**現狀（V910 仍是舊鎖機版，已實測）**：`uCleaning.cpp:2913-2919`
```
if(bCanStart==false)  { ShowErrorMessage(sACRecAlarmCode, K_RETRY, MMInterface, false, sACRecEPortCode); }
return bCanStart;
```
→ `main.cpp:4790` 拿到 false 就 `return false`，START 被擋死；WAR0707（SiteToSite）與
WAR07301（CTF 連續 Fail）共用這唯一閘門，**無法分離**（與既有知識一致）。

**最終態＝使用者裁定的三需求，全部照搬**：
1. **需求1 不鎖機**：拿掉那句「重貼 alarm」的 `ShowErrorMessage`，函式尾**恆 `return true`**。
   （`main.cpp:4790` 的外殼與呼叫**一行不動**，只在其上加一行說明註解＝矩陣 4554。）
2. **需求2 觸發當下非阻塞提示**：`CheckSmartAutoClean()` 的 `iACUseParam==0` 分支裡，
   組出「原因（alarm 描述＋code）＋處置（已暫停／已重新初始化偵測／續低良率請離線清潔 Socket）」，
   走 `NewRecordProcess()` 記錄並寫入 `sACSmartNoteEN/CH` 兩個新全域，
   由 `note.cpp` 的 `pnlCleanSocket` 常駐面板動態帶入。
   **`bACSmartNeedReset==false` 防重複，只做一次。**
   **註解明寫：提示必須留在此觸發當下，不得改用 `ShowMyMessage`（會 `StopAllMotor`）** ——與既有知識一致。
3. **需求3 週期性重置偵測**：`bACSmartNeedReset` 置位後，於 START 閘門放行前呼叫
   `ResetSmartAutoClean()`（歸零 `iACUseParam`/`iACSmartCount`/`iACSmartCount_CTF` 並
   `RefreshYieldMonitor()`），再清面板提示，避免每 cycle 重複觸發同一 alarm。

### 1.4 超豐 Greatek TrayArm 夾爪極性校正（6 個 commit）→ **W14-07～09、26、30～34**

**這是日期撞號的他案**（客戶＝超豐 Greatek，非 PTI）。硬體極性與軟體假設相反：
- 正常持盤時 **`C_CatchTray_FixOn` 的到位 sensor 不會 ON**；`FixOn` 到位 ON 反而代表**空夾／掉盤**。
- 因此「兩顆到位 sensor 皆 OFF」＝有盤。

五處對應修正（**全部包在 `CUSTOMER_CODE==CC_Greatek` 內，非超豐維持原判斷**）：

| op | 位置 | 原行為 | Greatek 分支 |
|---|---|---|---|
| W14-31 | `IsTrayArmCatchTrayFail()` case 2/4 | `FixOn.OnStatus()==false` → fail | 任一到位 ON（FixOn 或 FixOff）→ fail |
| W14-30 | `C_CatchTray_Fix_Puch()` case 100 | `Push()` 等到位 → 逾時誤觸 **JAM31047**（缸 47） | `On()` 直接驅動不等 sensor，`bflag=true` |
| W14-26 | `DoCatchFromLoader` case 301 | `FixOn.OnSensor()==false` → Task=400（**JAM0613**） | 直接 `Task=310`，夾到與否交給下游 `IsTrayArmCatchTrayFail()` |
| W14-07/08/09 | `CheckInitStartSuckStatus` / `DoReTesetStart` / `DoInitialStart` | 一律 **WAR0615** | `FixOn` ON → WAR0615（空夾/掉盤）；否則 **WAR0614**（有盤→取盤） |
| W14-32 | `ProcessMotorHome` case 400 | 用 `FixOn.OnStatus()` 猜有沒有盤 | 以**軟體 `MOT[MTrayX].fHasTray`** 為準：有盤維持夾持、無盤驅動放開並確認 `FixOff` 到位（未到位防抖 100 次後 WAR0615） |

**另外兩條無客戶碼（全客戶）**：
- **W14-33**（`uhome.cpp:2706`）：Home 時 `FixOn` 讀 ON 但軟體無盤記錄，原邏輯**不經確認就夾持＋標記有盤**
  走自動退盤（非預期夾爪動作）→ 改成防抖 100 次後跳 **WAR0614** 由操作員確認
  （RETRY＝已取下盤重新檢查；SKIP＝確認有盤，維持舊有自動退盤）。
- **W14-27/28/29**（`acatchtray.cpp` 三處 JAM0610 SKIP）：SKIP 後補**釋放夾爪**
  （`eartInstall` → `FixOn.Off()+FixOff.On()`；否則 `C_CatchTray_Fix.Off()`），
  避免「空夾閉合殘留（兩顆到位 sensor 皆 OFF）」造成下次 Initial Start／ART 自檢 **WAR0615 循環**。
  這與既有知識 `ht9045-trayarm-phantom-tray-fix`（空夾閉合＝WAR0615 風暴根源；JAM0610 SKIP 要補釋放夾爪）**完全一致**。

### 1.5 CASE-PTI-20260630-002 的 FTP 加固（`292bd72`）→ **W14-19～21**

`TFormHS::UpDataToServerByFTP()` 最外層包 `try/catch`，讓 socket 例外不再以**空 UnitName**
逸出到 `main.cpp` 的 `AppException`（＝那個 benign 10038 的來源，見 KNOWLEDGE
`ht9045-lotend-10038-alarm-buzzer-cluster`），並保留原 success 回傳碼語義（例外時回 `HS_ERR_FTPUploadError`）。
同時 `fFTP.Connect(...)` 的 timeout **30000 → 5000ms**，與 PTI N06 路徑一致，降低同步阻塞最壞值。

> **注意**：V899 的 `try` **沒有把函式本體重新縮排**（`try{` 之後的行維持原縮排）。忠實搬運照原樣，
> 因此套用後 V910 也會是這個版面。這是 V899 出貨現狀，非本波引入。

### 1.6 PTI Sort Summary 補欄（`292bd72`）→ **W14-03**

`ProductionDataLog()` 的 `[Sort Summary]` 原本只到 `Fix3`。為 PTI 補 `Fix4`/`Fix5`/`Fix6`
與 `Total`（Auto1-3 ＋ Fix1-6 九槽加總）。**`CUSTOMER_CODE==CC_PTI` 閘，非 PTI 輸出位元組不變**
（原本 `"Fix3=%d\n\n"` 拆成 `"Fix3=%d\n"` ＋ 尾端 `"\n"`，非 PTI 路徑輸出結果完全相同）。

---

## 2. 逐 op 明細（`docs/mg_w14_ops.json`，34 op）

> 「錨點命中」＝該字串在 V910 該檔**全檔**的命中次數（實測）。`splice.py` 同檔由**下往上**套用。
> `pay`／`rep`／`net` ＝ 抽取行數／被取代行數／淨增。

### 2.1 `cpublic.h`（1 op，385 → **386**，+1）

| op | 矩陣行 | V899 範圍 | V910 動作 | 錨點（命中） | 淨 |
|---|---|---|---|---|---:|
| **W14-01** | 46 | `46-46` | `after 45` | `bool ExecZipCommand(AnsiString Path, AnsiString Param);` → **1** | +1 |

### 2.2 `cpublic.cpp`（2 op，2235 → **2288**，+53）

| op | 矩陣行 | V899 範圍 | V910 動作 | 錨點（命中） | 淨 |
|---|---|---|---|---|---:|
| **W14-02** | 655 | `655-691` | `before 657` | `AnsiString GetOnlyTimeInfoByString(AnsiString asSign)` → **1** | +37 |
| **W14-03** | 768 | `766-783` | `replace [732,733]` | `tmps.sprintf("Fix3=%d\n\n", LastSet.BinCT[0][e3Fix3]);` → **1** | +16 |

### 2.3 `csystem.cpp`（6 op，25649 → **25690**，+41）

| op | 矩陣行 | V899 範圍 | V910 動作 | 錨點（命中） | 淨 |
|---|---|---|---|---|---:|
| **W14-04** | 21812 | `21812-21818` | `replace [23225,23225]` | `RemoveDir(d+SearchRec.Name);` → **1** | +6 |
| **W14-05** | 21823 | `21823-21832` | `replace [23230,23231]` | `FileSetAttr(d+SearchRec.Name,faArchive);` → **1** | +8 |
| **W14-06** | 21840 | `21840-21846` | `replace [23239,23239]` | `    RemoveDir(d);`（含 4 空白縮排）→ **1** | +6 |
| **W14-07** | 5489 | `5489-5497` | `replace [5883,5884]` | 含 `//jou 2011-11-08 retry -> skip` 的整行 → **1** | +7 |
| **W14-08** | 5570 | `5570-5578` | `replace [5957,5958]` | 跨行（18 空白縮排版）→ **1** | +7 |
| **W14-09** | 6229 | `6229-6237` | `replace [6690,6691]` | 跨行（16 空白縮排版）→ **1** | +7 |

> W14-08／09 的兩個 `if(USE_CATCH_TRAY_MODEL==2 ||` 縮排不同（18 vs 16 空白），跨行錨因此全檔唯一。
> **三個 op 的第二行 `USE_CATCH_TRAY_MODEL==3)` 是位元組級重插**（V910 的行尾註解對齊未被改動）。

### 2.4 `main.cpp`（5 op，35350 → **35408**，+58）

| op | 矩陣行 | V899 範圍 | V910 動作 | 錨點（命中） | 淨 |
|---|---|---|---|---|---:|
| **W14-10** | 25154 | `25154-25160` | `before 26380` | `void TfMain::StateRecordImage()` → **1** | +7 |
| **W14-11** | 25164, 25165 | `25164-25165` | `replace [26383,26383]` | `int iret=-1;` → **1** | +1 |
| **W14-12** | 25226, 25260 | `25226-25294` | `replace [26448,26475]` | `str1.sprintf("d:\\HT9045\\7z.exe a -tzip …` → **1** | +41 |
| **W14-13** | 25727 | `25727-25735` | `replace [26923,26923]` | `        ExecZipCommand(BatFile, " ");  //Steven 20160205` → **1** | +8 |
| **W14-14** | 4554 | `4554-4554` | `before 4790` | `if(fCleaning->CheckSmartAutoCleanCanStart()==false)` → **1** | +1 |

> **W14-12 的 replace 範圍嚴格限定 case 6 內文（`if(iSaveImgae==1)` 到 `ShellExecute(...)`），
> 不含 `case 6:` 標籤與其後的 `break;`**；case 1–5 的 `iScrW`/`iScrH` 一行不動（實測 8→8）。

### 2.5 `mymessbox.cpp`（2 op，1400 → **1404**，+4）／`note.cpp`（2 op，6875 → **6884**，+9）

| op | 矩陣行 | V899 範圍 | V910 動作 | 錨點（命中） | 淨 |
|---|---|---|---|---|---:|
| **W14-15** | 344 | `341-345` | `replace [381,382]` | `fMain->machineTime.Pause();` → **1** | +3 |
| **W14-16** | 943 | `931-944` | `replace [989,1001]` | `MyMessageBox->pnlNo->Caption        ="Full lot end";` → **1** | +1 |
| **W14-17** | 1995 | `1995-1999` | `after 2014` | `lblSmartCleanCH->Caption="auto clean ` → **1** | +5 |
| **W14-18** | 3486 | `3482-3489` | `replace [3511,3514]` | `else if(MyMessageBox->fShow==false)` → **1** | +4 |

> W14-16 的 replace 上界拉到 989 **只為取唯一錨**（`MyMessageBox->ShowModal();` 在本檔命中 7 次、
> `FTCTManStartUnlock();` 命中 5 次，兩者的三行組合在 5 個函式裡長得一模一樣）。
> 被含進來的 13 行**逐位元組驗證與 V899 [931,943] 相同**，port_check 的 `removed` 因此為 0。

### 2.6 `acatchtray.cpp`（6 op，9338 → **9391**，+53）

| op | 矩陣行 | V899 範圍 | V910 動作 | 錨點（命中） | 淨 |
|---|---|---|---|---|---:|
| **W14-26** | 1729 | `1729-1734` | `replace [2074,2074]` | `if(Cylinder[C_CatchTray_FixOn].OnSensor()==false)` → **1** | +5 |
| **W14-27** | 2020 | `2017-2030` | `replace [2392,2393]` | 跨行 `else if(ret==K_SKIP)` + `MOT[MTrayX].fHasTray=false;` → **⚠2** | +12 |
| **W14-28** | 3628 | `3624-3639` | `replace [4132,4137]` | `"DoPlaceTrayToAuto_250"` → **1** | +10 |
| **W14-29** | 4591 | `4587-4602` | `replace [5212,5217]` | `"DoPlaceTrayToBuffer_2100"` → **1** | +10 |
| **W14-30** | 7239 | `7238-7247` | `replace [8205,8205]` | `bflag=Cylinder[C_CatchTray_FixOn].Push();` → **1** | +9 |
| **W14-31** | 7350 | `7350-7357` | `replace [8308,8308]` | `if(Cylinder[C_CatchTray_FixOn].OnStatus()==false)               //wei 20150415` → **1** | +7 |

> **⚠ W14-27 是本波唯一非全檔唯一錨（命中 2）。**
> 另一處是 `2365-2366`——`DoCatchFromLoader` case 570 的孿生分支
> （`if(LastSet.iRealDummy!=DUMMY && bC03UseCatchTray==false && fHasTray)`，V899 那邊**沒有**加釋放夾爪），
> 兩者的兩行縮排與內容完全相同。`splice.py` 的 `replace` 只對 `replace_lines` 範圍內做包含檢查，
> **行號由 `acatchtray.cpp` 的 md5 前提（`effd6ba47e00`）鎖定**；套用前必須先驗 md5。
> 附近唯一能區分兩者的行是 `2380`（`if(MOT[MTrayX].fHasTray && IsTrayArmCatchTrayFail())`）
> vs `2353`（`if(CatchTraySuck.Suck[0][0].Enable && …)`），但把 replace 拉到 2380 會蓋掉
> Eastsun 的 `//==> Eastsun 20260515 F017` 區塊＝§6.1 的 F1 衝突，故不採。

### 2.7 `uhome.cpp`（3 op，5198 → **5240**，+42）／`AutoClean\uCleaning.cpp`（2 op，2923 → **2943**，+20）／`cmydef.cpp`（1 op，+4）／`cmydef.h`（1 op，+3）

| op | 矩陣行 | V899 範圍 | V910 動作 | 錨點（命中） | 淨 |
|---|---|---|---|---|---:|
| **W14-32** | 2664 | `2664-2689` | `before 2889` | `if(MOT[MTrayX].fHasTray ||` → **1** | +26 |
| **W14-33** | 2706 | `2706-2728` | `replace [2905,2912]` | `Tray arm detect a tray on it.` → **1** | +15 |
| **W14-34** | （無標記） | `2743-2748` | `replace [2927,2931]` | 跨行 `Cylinder[C_CatchTray_Fix].Off();`＋`}`＋`}` → **1** | +1 |
| **W14-22** | 2650 | `2650-2660` | `after 2712` | `bShowNoteCleanSocket=true;` → **1** | +11 |
| **W14-23** | 2863, 2867 | `2863-2876` | `replace [2915,2919]` | `ShowErrorMessage(sACRecAlarmCode, K_RETRY, MMInterface, false, sACRecEPortCode);` → **1** | +9 |
| **W14-24** | 5457-5460 | `5457-5460` | `after 5522` | `bool bShowNoteCleanSocket=false;` → **1** | +4 |
| **W14-25** | 5508-5510 | `5508-5510` | `after 5590` | `extern AnsiString sACRecEPortCode;` → **1** | +3 |

---

## 3. 事前模擬（記憶體內套用；輸出只寫進 scratchpad，V910 樹未動）

### 3.1 套用結果

| 檔 | 套用前 | 套用後 | Δ | CRLF | bare-LF | cp950 strict |
|---|---:|---:|---:|---:|---:|---|
| `cpublic.h` | 385 | **386** | +1 | 386 | **0** | OK |
| `cpublic.cpp` | 2235 | **2288** | +53 | 2288 | **0** | OK |
| `csystem.cpp` | 25649 | **25690** | +41 | 25690 | **0** | OK |
| `main.cpp` | 35350 | **35408** | +58 | 35408 | **0** | OK |
| `mymessbox.cpp` | 1400 | **1404** | +4 | 1404 | **0** | OK |
| `note.cpp` | 6875 | **6884** | +9 | 6884 | **0** | OK |
| `HS_Function.cpp` | 5381 | **5395** | +14 | 5395 | **0** | OK |
| `AutoClean\uCleaning.cpp` | 2923 | **2943** | +20 | 2943 | **0** | OK |
| `cmydef.cpp` | 6005 | **6009** | +4 | 6009 | **0** | OK |
| `cmydef.h` | 6024 | **6027** | +3 | 6027 | **0** | OK |
| `acatchtray.cpp` | 9338 | **9391** | +53 | 9391 | **0** | OK |
| `uhome.cpp` | 5198 | **5240** | +42 | 5240 | **0** | OK |

**34 個 op 的錨點檢查全部通過（FAIL COUNT = 0）**；每檔行數增減與 op 算術逐檔對帳相符。

### 3.2 簽章對帳

模擬結果中 `//AI(ht9045-v899) 20260630` ＝ **17**、`20260703` ＝ **23**，合計 **40**
＝ 42 條矩陣點 **減去 §6.1 的 2 條**，一條不多一條不少：

| 日期 | 檔 | 條 |
|---|---|---:|
| 20260630 | `cpublic.h` 1、`cpublic.cpp` 2、`csystem.cpp` 3、`main.cpp` 6、`mymessbox.cpp` 2、`note.cpp` 1、`HS_Function.cpp` 2 | **17** |
| 20260703 | `csystem.cpp` 3、`main.cpp` 1、`note.cpp` 1、`uCleaning.cpp` 3、`cmydef.cpp` 4、`cmydef.h` 3、`acatchtray.cpp` 6、`uhome.cpp` 2 | **23** |

### 3.3 忠實度驗證

- **每個 op 的 payload 在結果中恰好出現 1 次**（34/34）。
- **關鍵區段位元組相等**（逐段實測 IDENTICAL）：

  | 區段 | 行數 |
  |---|---:|
  | `main.cpp` statics 宣告區（V899 25154-25160） | 7 |
  | `main.cpp` case 6 內文（V899 25226-25294） | 69 |
  | `main.cpp` `DoStateRecord` handle 初始化（V899 25727-25735） | 9 |
  | `cpublic.cpp` `ExecZipCommandHandle` 整支（V899 655-691） | 37 |
  | `cpublic.cpp` `ProductionDataLog` Fix4-6 段（V899 766-783） | 18 |
  | `csystem.cpp` `Del_Tree` **整支函式**（V899 21791-21847） | 57 |
  | `uCleaning.cpp` `CheckSmartAutoCleanCanStart` **整支函式**（V899 2834-2881） | 48 |
  | `uCleaning.cpp` `CheckSmartAutoClean` 觸發段（V899 2649-2661） | 13 |
  | `acatchtray.cpp` `IsTrayArmCatchTrayFail` case2/4（V899 7348-7361） | 14 |
  | `acatchtray.cpp` `C_CatchTray_Fix_Puch` case100（V899 7237-7248） | 12 |
  | `uhome.cpp` Greatek Home 分支（V899 2664-2689） | 26 |
  | `uhome.cpp` WAR0614 防抖段（V899 2706-2728） | 23 |
  | `HS_Function.cpp` try 頭（V899 1967-1969）＋ catch 尾（V899 2022-2034） | 3 + 13 |

- **V910 專屬內容零遺失**（出現次數 before → after，實測）：
  `iScrW` 8→8、`iScrH` 8→8、`Steven 20260504` 7→7、`DumpMainFormSnapshot` 3→3、
  `Eastsun 20260515 F017` 25→25、`Eastsun 20260710` 30→30、`bCoverTrayInProgress` 10→10、
  `iTrayDetectCount` 13→13、`CC_KYEC_LEE` 24→24、`Eastsun 20260511` 47→47、
  `AI(ht9045-v899) 20260414` 7→7、`Jimmychiu 20251208` 1→1、`ChangeACSmartInterval` 3→3、`WAR0611` 3→3。
- **編碼健全**：12 個檔 cp950 **strict** 解碼全過；mojibake（`嚙`）計數與 U+FFFD 計數 **全部 0→0**。
- **結構健全**（**先 cp950 解碼再數字元**，Big5 尾位元組可能是 `0x7B`/`0x7D`）：
  12 個檔的 `{`−`}` 與 `(`−`)` 差值**套用前後完全相同**（delta 全部 SAME）。

### 3.4 removed 44 行全部是刻意取代（逐條列出供主腦對照）

| 檔 | 行 | 內容摘要 |
|---|---:|---|
| `cpublic.cpp` | 1 | `tmps.sprintf("Fix3=%d\n\n", …)`（拆成 `\n` ＋尾端 `\n`） |
| `csystem.cpp` | 7 | Del_Tree 三處單行版（`RemoveDir`×2、`FileSetAttr`+`DeleteFile`）＋三處 `if(USE_CATCH_TRAY_MODEL==2 \|\|`（換成 `else if`） |
| `main.cpp` | 24 | `int iret=-1;` ＋ 舊 case 6 整段（同步 `system()` 版） ＋ `ExecZipCommand(BatFile," ");` |
| `HS_Function.cpp` | 1 | `fFTP.Connect(…, 30000, …)`（換成 5000 ＋ AI 註解） |
| `AutoClean\uCleaning.cpp` | 2 | `ShowErrorMessage(sACRecAlarmCode, …)`（鎖機行為，客戶否決） ＋ `return bCanStart;`（換成 `return true;`） |
| `acatchtray.cpp` | 3 | `if(FixOn.OnSensor()==false)`（換成 Greatek 分支＋`else if`）、`bflag=…Push();`、`if(FixOn.OnStatus()==false)` |
| `uhome.cpp` | 6 | `if(fHasTray==false)`＋`Str.sprintf("Tray arm detect a tray…")`＋`ShowMyMessage(Str)`＋`fHasTray=true;`＋`FixOn.On()`＋`FixOff.Off()`（改成防抖 WAR0614 分流） |
| `mymessbox.cpp` / `note.cpp` / `cmydef.*` / `cpublic.h` | 0 | — |

### 3.5 port_check 預測（模擬檔跑同一套分類邏輯）

```
[cpublic.h              ] added=1   (spliced=1,  authored=0) removed=0
[cpublic.cpp            ] added=54  (spliced=54, authored=0) removed=1
[csystem.cpp            ] added=48  (spliced=48, authored=0) removed=7
[main.cpp               ] added=82  (spliced=82, authored=0) removed=24
[mymessbox.cpp          ] added=4   (spliced=4,  authored=0) removed=0
[note.cpp               ] added=9   (spliced=9,  authored=0) removed=0
[HS_Function.cpp        ] added=15  (spliced=15, authored=0) removed=1
[AutoClean\uCleaning.cpp] added=22  (spliced=22, authored=0) removed=2
[cmydef.cpp             ] added=4   (spliced=4,  authored=0) removed=0
[cmydef.h               ] added=3   (spliced=3,  authored=0) removed=0
[acatchtray.cpp         ] added=56  (spliced=56, authored=0) removed=3
[uhome.cpp              ] added=48  (spliced=48, authored=0) removed=6
TOTAL added=346 spliced=346 authored=0 removed=44
```

**預期 `authored=0`、`comment=0`、`removed=44`、EOL 全 CRLF 無混用。**

---

## 4. gate 形狀（**照 V899 出貨現狀，本波不新增任何客戶碼判斷**）

### 4.1 逐項閘門

| 變更 | 閘門 | 非目標機台／客戶的行為 |
|---|---|---|
| Sort Summary Fix4-6＋Total（W14-03） | **`CUSTOMER_CODE==CC_PTI`**（V899 原生） | 輸出位元組完全不變 |
| Greatek 夾爪極性五處（W14-07/08/09/26/30/31/32） | **`CUSTOMER_CODE==CC_Greatek`**（V899 原生），部分再 `&& USE_CATCH_TRAY_MODEL==2` 或 `&& USE_AUTO_RETEST==eartInstall` | 走 `else` 原路徑，指令級零變更 |
| Smart AutoClean 恆放行（W14-14/22/23） | **無客戶碼**。函式自身閘＝`IniConfig.bEnableAutoCleanFunction && TestIF.iAutoClean_Function && TestIF.bACSmart && TestIF.iACSmart_Count!=0 && TestIF.iACSmart_Count_CTF!=0` | 五條件任一不成立 → 原本就 `return true`，**零變更**。實務上再受 `CosFunction.bSmartAutoClean` 天然收斂（只有 `FUNC_CC_PTI`／`FUNC_CC_TERAPOWER`／`FUNC_CC_TeraProbe` 為 true，`InitialCosFunction` 預設 false） |
| 面板動態提示（W14-17）／新全域（W14-24/25） | `bShowNoteCleanSocket && CosFunction.bSmartAutoClean`（既有） | 其他客戶面板不顯示；新全域恆為空字串 |
| State Record 非阻塞化（W14-01/02/10/11/12/13） | **無閘，全客戶** | **外溢**，見 §6.2 |
| `Del_Tree` 重試（W14-04/05/06） | **無閘，全客戶** | **外溢**（純健壯性；失敗行為由「靜默略過」變「重試 3 次後仍略過」） |
| FTP try/catch ＋ timeout 5000（W14-19/20/21） | **無閘，全客戶** | **外溢**，見 §6.2 |
| 訊息框 z-order 三處（W14-15/16/18） | **無閘，全客戶** | **外溢**（純 UI 置頂，不搶焦點、不用 TOPMOST） |
| JAM0610 SKIP 補釋放夾爪（W14-27/28/29） | **無客戶碼**，內層 `USE_AUTO_RETEST==eartInstall` 二分 | **外溢**（只在操作員按 SKIP 後執行） |
| Home 幽靈盤 WAR0614 防抖（W14-33） | **無客戶碼**，在 `USE_AUTO_RETEST==eartInstall` 分支內 | **外溢**，見 §6.2（`ShowMyMessage` → 防抖 100 次後 `WAR0614`） |

### 4.2 `main.cpp:4790` 呼叫點確認（任務書指定必查）

- V910 **仍是舊鎖機版**：`uCleaning.cpp:2913-2919` 的 `ShowErrorMessage(sACRecAlarmCode,…)` ＋ `return bCanStart;` 原封存在。
- `CheckSmartAutoCleanCanStart()` 在 V910 全樹**只有 1 個呼叫點**（`main.cpp:4790`），
  宣告在 `uCleaning.h:358`。→ 「唯一閘門」的既有知識在 V910 同樣成立，改函式端即可，**外殼不動**。
- 呼叫點外層是 `#ifndef SOFT_SIMULTE`（V910:4789／V899:4553，兩樹相同）。

---

## 5. 反向發現（V910 有、V899 沒有）——**不可回搬**

| # | 位置 | V899 | V910 | 本波處置 |
|---|---|---|---|---|
| **R1** | `main.cpp` `StateRecordImage()` 前段 | 硬編碼 `1280`/`1024` | `iScrW`/`iScrH` ＝ `XResolution`/`YResolution`（`Steven 20260504 : Support 1920x1080 and 1280x1024`） | **保留**。op 切成三刀，case 1–5 一行不動（實測 8→8） |
| **R2** | `acatchtray.cpp` case 570 兩處防抖 | 無（`cc7c6f9` 才加無條件 `iCnt>100`） | `static int iCnt`（Eastsun 20260515 F017）＋ `(CUSTOMER_CODE!=CC_KYEC_LEE \|\| iCnt>iTrayDetectCount) && bCoverTrayInProgress==false`（Eastsun 20260710） | **真衝突 → §6.1 F1/F2**。本波不動它，只搬正交的 SKIP 釋放夾爪 |
| **R3** | `acatchtray.cpp` `DoPlaceTrayToAuto_250` / `DoPlaceToBuffer_2100` | `if(iCnt>100)` | `if(iCnt>100 && bCoverTrayInProgress==false)`（Eastsun 20260710） | **保留**。W14-28/29 的 replace 從 `ShowErrorMessage` 那行才開始，判斷式一行不動 |
| **R4** | `uhome.cpp:2889` 等處中文註解 | `嚙論伐蕭嚙糊…`（mojibake） | 乾淨中文 `修正歸零catch tray 會把tray丟在中途` | **保留**。W14-32/34 用「包住不重插」形狀 |
| **R5** | `HS_Function.cpp:2234-2241` | 無 | `Eastsun 20260511 整合`：`CC_KYEC_LEE` 上傳失敗改 `RecordProcess` 不報警 | **保留**，位於新 `try{}` 內部，未被觸碰（47→47） |
| **R6** | `cpublic.cpp:743-747` | 同內容但無前導空行 | `CUSTOMER_CODE==CC_PTI` Error Bin 區塊（Jimmychiu 20251208） | **保留**，在 W14-03 插入點之後 |
| **R7** | 多檔行尾註解對齊 | 第 ~80 欄 | 推到第 ~120 欄 | 本波所有 replace 都避開純對齊差異的行；**唯一被反向的是 `acatchtray.cpp:2074`**（該行本來就要改成 `else if`），對齊由 ~120 欄回到 ~80 欄——純排版 |

---

## 6. <95% 清單與外溢

### 6.1 建議寫入 `MG_FINAL_DECISIONS.md` 的項目：**2 條**（同一根因）

> （本文件全程唯讀，**未動** `MG_FINAL_DECISIONS.md`。）

**F1 — `acatchtray.cpp:2007`（case 570 JAM0610 `iCnt>100` 防抖）**

| | V899 最終態 | V910 現狀（`acatchtray.cpp:2380-2402`） |
|---|---|---|
| 判斷式 | `iCnt++; if(iCnt>100)` | `iCnt++; if((CUSTOMER_CODE!=CC_KYEC_LEE \|\| iCnt>iTrayDetectCount) && bCoverTrayInProgress==false)` |
| else 重置 | `else { iCnt=0; }`（全客戶） | `else if(CUSTOMER_CODE==CC_KYEC_LEE) { iCnt=0; }` |
| 非 KYEC 行為 | 累到 100 次才報 JAM0610 | **立即**報 JAM0610 |
| KYEC 行為 | 累到 100 次（硬編碼） | 累到工作檔 `iTrayDetectCount` 次（可設定） |
| cover-tray 進行中 | 無保護 | `bCoverTrayInProgress==false` 才報（Eastsun 20260710） |

**照搬的三個代價**：(a) 丟掉 Eastsun 20260710 的 `bCoverTrayInProgress` guard（cover tray 進行中會誤報）；
(b) KYEC 的可設定門檻 `iTrayDetectCount` 被硬編碼 100 取代；
(c) 其餘全部客戶由「立即報警」變成「100 次防抖」。
**三者都是行為變更，沒有安全的機械合併解 → 需使用者裁定。**
候選寫法（供裁決參考，**本波未採用**）：
```
if((CUSTOMER_CODE!=CC_KYEC_LEE || iCnt>iTrayDetectCount) && iCnt>100 && bCoverTrayInProgress==false)
```
→ 但這會讓 `iTrayDetectCount<100` 的 KYEC 機台實質失效，**仍是行為變更**。

**F2 — `acatchtray.cpp:1221`（`static int iCnt=0;` 宣告）** —— 屬 **C 類（等價物已存在）**
V910 `acatchtray.cpp:1550` 已有**同名同型別同作用域**的 `static int iCnt=0;`（Eastsun 20260515 F017）。
照搬 = 重複宣告 = 編譯錯。**與 F1 綁定**：F1 若裁定要搬，才需要把 V899 的 `//AI` 註解併進既有那行；
F1 若裁定不搬，這條就是純簽章缺口、無功能影響。

> **兩條合計 2 個矩陣點無法轉 PRESENT**，其餘 **40/42 條**本波可全數結清。

### 6.2 必須記入 LEDGER 的外溢（**不是阻塞項，但要留痕**）

1. **【最高】Smart AutoClean 的唯一鎖機閘門被解除，且無客戶碼。**
   `CheckSmartAutoCleanCanStart()` 從「條件不成立就擋 START」變成**恆 `return true`**。
   實務外溢限於 `CosFunction.bSmartAutoClean==true` 的三個客戶
   （`FUNC_CC_PTI` / `FUNC_CC_TERAPOWER` / `FUNC_CC_TeraProbe`），但**程式面確實是全客戶**。
   **殘留風險**：若某客戶 profile 沒開 `bSmartAutoClean` 卻在工作檔開了那五個條件，
   `ResetSmartAutoClean()` 的函式體被 `if(CosFunction.bSmartAutoClean)` 包住＝**空轉**，
   但 `return true` 照樣解鎖 → 偵測計數不會歸零、面板提示也不會出現。
   V899 出貨版即如此；要收緊屬**新需求**。
2. **`sACRecAlarmCode` / `sACRecEPortCode` 變成 write-only。**
   W14-23 移除後，V910 全樹唯一的讀取點消失（只剩 `uCleaning.cpp:2669-2670` 兩處寫入）。
   BCB6 不會對未使用的全域告警；V899 出貨版即如此，記錄備查。
3. **State Record 收尾改成 timer 重入的非阻塞輪詢，對全體客戶生效。**
   副作用：case 6 會佔用 `iSaveImageTask` 更多拍；極少數「前一份還在輪詢時又觸發新一份」的重入，
   舊 `1.bat` 不會被 kill（只關 handle），舊 `NewPath` 夾會被放棄成孤兒夾——
   **V899 的註解已明寫這是已知取捨**（低機率、僅磁碟殘留、非控制/SECS 風險）。
4. **N10 FTP connect timeout 30000 → 5000ms，對全體客戶生效。**
   MG-W5 當時刻意保留 30000（LEDGER 記「20260630 timeout 30000 未動」），本波正式改掉。
   慢速或跨網段 FTP server 的客戶可能出現連線失敗率上升 → 失敗走既有 `else` 分支
   （`HS_ERR_FTPUploadError` ＋ MNetLog trace）。
5. **`Del_Tree` 對全體客戶多出最多 3×100ms 的同步等待**（僅在刪除失敗時）。
   最壞情況：一層目錄下 N 個被占用檔 → N×300ms。`Del_Tree` 由 State Record／log 清理路徑呼叫。
6. **`uhome.cpp` Home 幽靈盤由 `ShowMyMessage` 改成 `WAR0614`（無客戶碼）。**
   語意變更：原本是不阻塞的訊息框、之後**無條件**夾持並標記有盤；
   現在是防抖 100 次後跳可 RETRY/SKIP 的警報，且 **RETRY 不再自動夾持**。
   這正是既有知識 `ht9045-trayarm-phantom-tray-fix` 說的「幽靈盤」修法，但它會**改變非超豐機台的 Home 行為**。
7. **`main.cpp` 新增 4 個檔案層 static**（非 `TfMain` 成員），跨 `DoStateRecord` 與 `StateRecordImage` 共享。
   V899 出貨版即如此；BCB6 全量 build 下無問題，記錄備查。
8. **`cpublic.h` 新增使用 `HANDLE` 的宣告。** 該標頭**兩樹都沒有任何 `#include`**，
   `HANDLE` 靠 includer 的 `MachineDefine.h`／`vcl.h` 鏈提供。
   實測 V910 include `cpublic.h` 的檔集合與 V899 **完全相同（85 檔、0 個 V910 獨有）**，
   故 V899 出貨 build 即為背書；仍請以 bcc32 實編確認（見 §7 步驟 4）。

---

## 7. 收工 gate 建議（給主腦）

1. **硬前提（套用前先驗，任一不符就停）**——見 §0.1 的 md5 表。四個重點：

   | 檔 | 必須是 | md5(12) | 為什麼是重點 |
   |---|---:|---|---|
   | `acatchtray.cpp` | **9338** 行 | `effd6ba47e00` | **W14-27 的非唯一錨全靠它鎖行號** |
   | `main.cpp` | **35350** 行 | `5ea391b1c9f2` | 已被 W4/W10/W12/W13 動過 |
   | `AutoClean\uCleaning.cpp` | **2923** 行 | `0c52d8619868` | 已被 W11 動過 |
   | `csystem.cpp` | **25649** 行 | `db440cd3906c` | 6 個 op、跨三個函式 |

   （其餘 8 檔為 pristine：385 / 2235 / 1400 / 6875 / 5381 / 6005 / 6024 / 5198，md5 見 §0.1。
   若期間有別波先動了這些檔，本波行號必須重量；`splice.py` 會因唯一錨不符而**整檔中止**，是安全失敗。）

2. `python tools/port_tools/splice.py docs/mg_w14_ops.json`
   - 預期輸出（12 檔，全部 `eol=CRLF`）：
     ```
     OK cpublic.h: 1 op(s) applied, eol=CRLF
     OK cpublic.cpp: 2 op(s) applied, eol=CRLF
     OK csystem.cpp: 6 op(s) applied, eol=CRLF
     OK main.cpp: 5 op(s) applied, eol=CRLF
     OK mymessbox.cpp: 2 op(s) applied, eol=CRLF
     OK note.cpp: 2 op(s) applied, eol=CRLF
     OK HS_Function.cpp: 3 op(s) applied, eol=CRLF
     OK AutoClean\uCleaning.cpp: 2 op(s) applied, eol=CRLF
     OK cmydef.cpp: 1 op(s) applied, eol=CRLF
     OK cmydef.h: 1 op(s) applied, eol=CRLF
     OK acatchtray.cpp: 6 op(s) applied, eol=CRLF
     OK uhome.cpp: 3 op(s) applied, eol=CRLF
     ```
   - 套用後行數：**386 / 2288 / 25690 / 35408 / 1404 / 6884 / 5395 / 2943 / 6009 / 6027 / 9391 / 5240**
   - 產生 12 個 `.mgbak`（目前樹上 `.mgbak` 為 0 個）

3. `python tools/port_tools/port_check.py`（不給參數即可）
   - 預期與 §3.5 的表**逐檔相同**：`added` 全數 SPLICED、`authored=0`、`comment=0`、
     `removed` 合計 **44**、EOL 全 CRLF 無 bare-LF。
   - **REMOVED 44 行的逐條清單見 §3.4**，主腦只要對照那張表即可。

4. `bcc32 -c -H-` 單檔隔離編譯 **9 個 .cpp**：`cpublic.cpp`、`csystem.cpp`、`main.cpp`、
   `mymessbox.cpp`、`note.cpp`、`HS_Function.cpp`、`AutoClean\uCleaning.cpp`、`cmydef.cpp`、
   `acatchtray.cpp`、`uhome.cpp`。
   §0.3 已把可預見的編譯風險（符號可見性、`#include` 缺漏、名稱衝突、多載匹配）逐項排除，仍請實編確認。
   **最需要編譯背書的三支**：
   - `cpublic.cpp` / 任一 include `cpublic.h` 的 TU —— 驗 `HANDLE` 在 `cpublic.h` 可見（§6.2-8）
   - `main.cpp` —— 69 行新 case 6 ＋ 4 個 static ＋ 跨函式共享
   - `acatchtray.cpp` —— **必驗 `iCnt` 沒有被重複宣告**（§6.1 F2 的反向確認：W14-01～34 都沒有搬那行）

5. 主腦逐條開檔複驗，**重點六處**：
   - `main.cpp` 套用後 `StateRecordImage()` 的 **case 1–4 必須仍是 `iScrW`/`iScrH`**（不是 1280/1024）。
   - `main.cpp` 套用後 case 6 內文與 V899[25226,25294] **逐位元組相同**，且其後仍接 `break;` ＋ `}` ＋ `}`。
   - `acatchtray.cpp` 套用後 **2380 附近的 Eastsun 20260515/20260710 區塊完整保留**
     （`(CUSTOMER_CODE!=CC_KYEC_LEE || iCnt>iTrayDetectCount) && bCoverTrayInProgress==false`），
     且**只有 `else if(ret==K_SKIP)` 那兩行被換成帶大括號的釋放夾爪版**。
   - `acatchtray.cpp` 套用後 `2365-2366`（孿生分支）**必須維持原樣未被誤改**——W14-27 非唯一錨的驗收點。
   - `uhome.cpp` 套用後 `if(MOT[MTrayX].fHasTray ||` 那行的中文註解仍是**乾淨的**
     （`修正歸零catch tray 會把tray丟在中途`，不是 `嚙…`）。
   - `uCleaning.cpp` 套用後 `CheckSmartAutoCleanCanStart()` 整支與 V899[2834,2881] **逐位元組相同**，
     函式尾是 `return true;` 而非 `return bCanStart;`。

6. 收工後 `python tools/port_tools/ai_comment_matrix.py` 重算
   → 本波 **40 條**轉 PRESENT，**`acatchtray.cpp:1221` 與 `acatchtray.cpp:2007` 仍為 MISSING（設計如此，見 §6.1）**。
   - **只驗「減 40 條、且這 40 條正是本波的」**，不要拿總數當基準（基數會被別波漂移）。
   - 本波 40 條的檔／行清單：

     | 檔 | 行 |
     |---|---|
     | `cpublic.h` | 46 |
     | `cpublic.cpp` | 655, 768 |
     | `csystem.cpp` | 5489, 5570, 6229, 21812, 21823, 21840 |
     | `main.cpp` | 4554, 25154, 25164, 25165, 25226, 25260, 25727 |
     | `mymessbox.cpp` | 344, 943 |
     | `note.cpp` | 1995, 3486 |
     | `HS_Function.cpp` | 1967, 1987 |
     | `AutoClean\uCleaning.cpp` | 2650, 2863, 2867 |
     | `cmydef.cpp` | 5457, 5458, 5459, 5460 |
     | `cmydef.h` | 5508, 5509, 5510 |
     | `acatchtray.cpp` | 1729, 2020, 3628, 4591, 7239, 7350 |
     | `uhome.cpp` | 2664, 2706 |

7. **LEDGER 記錄要點**
   - 20260630（17 條）**全結清**；20260703 結清 23／25，**餘 2 條進 F 列**（`acatchtray.cpp:1221, 2007`）。
   - 類別：**D 類（安全關鍵：鎖機閘門／夾爪動作／State Record 收尾）**，忠實搬運，
     錨點 33/34 全檔唯一（W14-27 為 2，靠 md5 前提）。
   - **case 歸屬要分開寫**：
     `CASE-PTI-20260630-001` = Smart AutoClean 不鎖機（W14-14/17/22/23/24/25）；
     `CASE-PTI-20260630-002` = 訊息框 z-order ＋ FTP try/catch/timeout（W14-15/16/18/19/20/21）；
     PTI 出貨內容 = State Record 非阻塞化 ＋ Del_Tree 重試 ＋ Sort Summary 補欄
     （W14-01～06/10～13）；
     **超豐 Greatek TrayArm 極性校正（日期撞號的他案）** = W14-07/08/09/26/30/31/32；
     **無客戶碼的 ART 夾爪誤報修正** = W14-27/28/29/33。
   - **本波不新增任何客戶碼判斷**；PTI／Greatek 閘門全部是 V899 原生的。
   - 外溢八項見 §6.2（特別是 1、4、6 三項是行為變更）。
   - 反向發現 R1–R7 已確認全部未被倒回（§5，含實測次數）。
   - 教訓兩則可入 KNOWLEDGE：
     (a) **「日期叢集」不等於「同一個案子」**——0703 的 25 條橫跨兩個客戶、七個 commit，
     其中一半是超豐而非 PTI；分派波次前先跑 `git log -G` 看 commit 歸屬。
     (b) **V910 可能已經有同名變數**（`iCnt`）——搬移前的符號稽核要含「新增的區域/靜態變數」，
     不能只查全域與函式名。

8. **commit 後清 V910 樹 `*.mgbak`**（12 個），讓下一波的差異報告基準回到波前狀態。
