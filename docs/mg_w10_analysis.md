# MG-W10 分析：FTP 上傳主題大波（0609 PTI 非阻塞 → 0611 bError 早退 → 0612 背景執行緒）

- **波次**：MG-W10（V899 → V910）
- **主題**：`CASE-20260611-001`／`CASE-PTI-20260630-002`（力成 PTI N06 FTP：Lot End 被同步 FTP 上傳阻塞，
  導致卡站、alarm 無法消除、socket 10038 AppException）。三個日期是**同一條相依鏈**，必須同波：
  - **0609**（4 條，`KYECFTP\FTPClient.cpp`）＝同步上傳路徑「不再阻塞主執行緒」的第一刀
    （timeout 20s→5s、連線失敗不彈 modal、例外不讀 `LastErrorNo`）。
  - **0611**（3 條）＝`fFTPClient->bError` 早退（整批中止），**它讀的旗標就是 0609 那條路徑寫的**；
    另一條 `cTrayAssignment.cpp:358` 是同 CASE-ID 但不同子系統（見 §1.6）。
  - **0612**（50 條）＝背景 `TFtpUploadThread` fire-and-forget，把 PTI 主路徑整條移出主執行緒。
- **來源**：`docs/mg_ai_matrix_missing.csv` **58 條 MISSING**
  （0612 × 50、0611 × 3、0609 × 4，**外加 `uLotInfo.cpp:1952` 的 20260630 一條**——
  它落在 0612 區段的連續位元組內，見 §0.2）
- **V899 樹**：`D:\HT9045\HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422`
  （唯讀；本波六個來源檔＋`FTPUpload\` 目錄 `git status --porcelain` 空，
  且 `git diff b515ed5 HEAD` 對它們為空 → 工作區＝在製收斂端點 `b515ed5`）
- **V910 樹**：`D:\HT9045\HT9011UC_Code_V3.33.910.0_20260716_Jimmy`
  （分析期間主迴圈並行推進 **MG-W9**，工作區有四個未 commit 檔：`CosFunction.cpp/.h`、`acatchtray.cpp`、
  `ainarm9045.cpp` ——**與本波六個目標檔零重疊**，實測 `git status` 已確認。
  本文件所有 V910 行號取自 **20260826 分析當下的磁碟內容**，已含 MG-W4（main.cpp）與 MG-W6（cTrayAssignment.cpp）。）
- **本文件性質**：分析＋splice op 草案。**分析階段全程唯讀，未修改任何原始碼。**
- **op 檔**：`D:\HT9045\docs\mg_w10_ops.json`（**16 個 op**）
- **另有 2 個新增檔**（整檔複製，不做 op）＋ **6 處 `.bpr` 變更**（見 §4，**列給主腦手動確認，本分析不改 .bpr**）
- **類別**：**B 類（客戶碼隔離）**——0609/0612 主路徑 gate 全部是 V899 原生的
  `CUSTOMER_CODE==CC_PTI` 與 `IniConfig.bFtpUploadBackground`（預設依客戶碼），**未新增任何 `CUSTOMER_CODE` 判斷**。
  例外是 OP12（`cTrayAssignment`），它**沒有客戶碼閘門**，走的是機台工作檔閘門（見 §1.6，仍判 ≥95%）。

---

## 0. 前置事實（全部實測，非推論）

### 0.1 ⚠ V910 有沒有 `uFtpUploadThread`？——**明確答案：完全沒有，`FTPUpload\` 目錄本身不存在**

```
V899  FTPUpload\uFtpUploadThread.cpp   20,204 bytes / 498 lines / 100% CRLF / cp950 解碼乾淨(U+FFFD=0)
V899  FTPUpload\uFtpUploadThread.h      4,801 bytes /  88 lines / 100% CRLF / cp950 解碼乾淨(U+FFFD=0)
V910  FTPUpload\                        ls: No such file or directory
V910  HT9045.bpr                        "uFtpUploadThread" 命中 0 次、"FTPUpload" 命中 0 次
```

全樹符號掃描（V910 全部 `.cpp/.h/.hpp/.c`，排除 `.svn`）：

| 符號 | V910 命中 | V899 命中 |
|---|---:|---:|
| `TFtpUploadThread` / `TFtpUploadJob` / `FtpUploadThd` | **0 / 0 / 0** | 18 / 16 / 18 |
| `bFtpUploadBackground` | **0** | 6 |
| `EnqueueUpload` / `FetchResult` / `MakeJobKey` / `WriteBgLog` / `sInFlightKey` | **0** | 8 / 3 / 5 / 8 / 6 |
| `FTP_UPLOAD_FAIL` / `FTPUP_LOG_DIR` | **0 / 0** | 1 / 3 |

→ **零「已搬一半」狀態，58 條 MISSING 判定成立。本波是新增檔搬移（whole-file copy）＋ 16 個 splice op。**

V899 端「所有碰到 `FtpUpload*` 的檔」窮舉＝**恰好 6 個**，全在本波範圍內，沒有第七個：

```
Config.h(1)  cprod.cpp(1)  main.cpp(18)  uLotInfo.cpp(9)
FTPUpload\uFtpUploadThread.cpp(34)  FTPUpload\uFtpUploadThread.h(14)
```

### 0.2 日期叢集歸屬切分

| 日期 | 檔 | 條數 | 歸屬 |
|---|---|---:|---|
| 20260609 | `KYECFTP\FTPClient.cpp` | 4 | **本波**（同步路徑非阻塞化＝0611 `bError` 的上游） |
| 20260611 | `uLotInfo.cpp` (1959, 1967) | 2 | **本波** |
| 20260611 | `cTrayAssignment.cpp` (358) | 1 | **本波**（同 CASE-ID，不同子系統；§1.6 單獨論證） |
| 20260612 | `FTPUpload\uFtpUploadThread.cpp` | 23 | **本波**（新增檔） |
| 20260612 | `FTPUpload\uFtpUploadThread.h` | 8 | **本波**（新增檔） |
| 20260612 | `uLotInfo.cpp` | 11 | **本波** |
| 20260612 | `main.cpp` | 6 | **本波** |
| 20260612 | `Config.h` (957) / `cprod.cpp` (2245) | 1 / 1 | **本波** |
| **20260630** | **`uLotInfo.cpp:1952`** | **1** | **本波（矩陣沒被列進目標,但在 0612 區段的連續位元組內,必然隨 OP10 一起搬）** |
| 20260630 | `main.cpp`(6)／`csystem.cpp`(3)／`cpublic.cpp`(2)／`cpublic.h`／`mymessbox.cpp`(2)／`note.cpp`／`HS_Function.cpp`(2) | 17 | ❌ **State Record handle／MyMessageBox z-order／HS socket 主題 → 留別波** |

**本波 58 條，同日期(0630)留給別波 17 條。**

> `uLotInfo.cpp:1952` 的內容是
> `//AI(ht9045-v899) 20260630(CASE-PTI-20260630-002): FtpUploadThd 尚未建立時記 log 並跳過,寧可不傳也不落回同步阻塞 Lot End`
> ——它是 0612 背景分支的 `else` 保險路徑，與 0612 那段是同一片連續位元組，
> **技術上不可能只搬 0612 而不搬它**。這也是為什麼要用「連續區段」而不是「逐行」抽取。

> 0630 其餘 17 條裡唯一長得像 FTP 的是 `HS_Function.cpp:1987`
> （`connect timeout 30000->5000ms 與 PTI N06 路徑一致`），但那是 **HS_Function 的 socket**，
> 與 `FTPUpload` / `KYECFTP` 兩個模組都無呼叫關係，留別波。

### 0.3 `KYECFTP\FTPClient.cpp`（0609）與 `FTPUpload\`（0612）是不是同一模組？——**不是，但同一條相依鏈**

- `KYECFTP\FTPClient.cpp` ＝ **既有同步**上傳（`TfFTPClient` 是個 VCL Form，用 `TNMFTP` 元件）。
- `FTPUpload\uFtpUploadThread.cpp` ＝ **新的背景**上傳（純 `TThread` ＋ WinINet，
  註解明寫「不碰 `fFTPClient` / `FileListBox1` / VCL / `MOT[]` / `Sen[]`」）。
- **兩者零符號交集**（V899 全樹實測：`FTPUpload\*` 不引用任何 `fFTPClient` 符號，
  `KYECFTP\FTPClient.cpp` 不引用任何 `FtpUpload*` 符號）。

**為什麼仍要同波**：
1. 0611 的 `if(fFTPClient->bError==true) break;`（`uLotInfo.cpp:1967`）讀的 `bError`
   是 `UploadFileToServer2()` 在連線失敗路徑寫的——而 0609 改的就是那條路徑。
   `bError=true` 本身 V910 已有（`FTPClient.cpp:490/832/840`），故 0611 不會因為 0609 缺席而壞掉，
   但兩者是同一個「Lot End 不要卡」的修法，分開搬會讓中間狀態的行為說不清楚。
2. **背景/同步兩條路徑在同一個 `if/else` 裡**（OP10/OP11）。0612 的 `else` 分支**逐字包含** 0611 的兩行。
   只搬 0612 而不搬 0611，等於把 V899 的 `else` 主體改寫，違反忠實搬運。

### 0.4 執行緒生命週期完整性（本波最關鍵的專節）

`TFtpUploadThread` 的**每一個生命週期接點**都必須到齊，缺一即當機或洩漏。以下逐點對帳：

| # | 生命週期接點 | V899 位置 | 本波交付 | 狀態 |
|---|---|---|---|---|
| 1 | 類別宣告（`class TFtpUploadThread : public TThread`） | `uFtpUploadThread.h:33-83` | 新增檔整檔複製 | ✅ |
| 2 | job 結構 `TFtpUploadJob` | `uFtpUploadThread.h:17-27` | 同上 | ✅ |
| 3 | `extern TFtpUploadThread *FtpUploadThd;` | `uFtpUploadThread.h:86` | 同上 | ✅ |
| 4 | 建構子（建 2 個 manual-reset event ＋ 2 把 `TCriticalSection`） | `uFtpUploadThread.cpp:33-45` | 同上 | ✅ |
| 5 | 解構子（`CloseHandle` ×2、`delete` ×2、`catch(...)` 吞例外） | `uFtpUploadThread.cpp:47-60` | 同上 | ✅ |
| 6 | `Execute()` 本體（`WaitForMultipleObjects` ×2 handle 迴圈） | `uFtpUploadThread.cpp:453-489` | 同上 | ✅ |
| 7 | `EndThread()`（設旗標＋`SetEvent(eEnd)`＋`SetEvent(eJob)`） | `uFtpUploadThread.cpp:492-497` | 同上 | ✅ |
| 8 | **全域單例的唯一正式定義** `TFtpUploadThread *FtpUploadThd = NULL;` | `main.cpp:217` | **OP4** | ✅ |
| 9 | **建立**：`FtpUploadThd = new TFtpUploadThread(false);` | `main.cpp:9974`（`TfMain::FormShow`） | **OP6** | ✅ |
| 10 | **回收**：`EndThread(); WaitFor(); delete; =NULL;` | `main.cpp:10918-10924`（`TfMain::FormClose`） | **OP7** | ✅ |
| 11 | **生產端** ×2：`FtpUploadThd->EnqueueUpload(...)` | `uLotInfo.cpp:1947`／`2008`（`TfLotInfo::SetLotEnd`） | **OP10／OP11** | ✅ |
| 12 | **消費端（主執行緒）**：`FtpUploadThd->FetchResult(...)` | `main.cpp:2638`（`TfMain::Timer1Timer`，1 秒 timer） | **OP5** | ✅ |
| 13 | `#include "uFtpUploadThread.h"` ×2 | `main.cpp:180`／`uLotInfo.cpp:76` | **OP3／OP9** | ✅ |
| 14 | 開關宣告＋ini 讀寫 | `Config.h:957`／`cprod.cpp:2245-2246` | **OP1／OP2** | ✅ |
| 15 | 診斷 log 打包 | `main.cpp:25676`（`TfMain::DoStateRecord`） | **OP8** | ✅ |

**與 UI thread 的互動 —— 明確答案：完全沒有用 `Synchronize()`。**
`uFtpUploadThread.cpp` 的 `Synchronize` 命中 **0 次**；`.h` 有 2 次但**都在註解裡**，
說明的正是「為何刻意不用」：

> 採「結果佇列 + 主執行緒撈取」而非 Synchronize：避免 FormClose `EndThread()+WaitFor()` 與
> worker 卡在 Synchronize 互等的死結；worker 端永不阻塞。

因此 **OP5（主執行緒 `FetchResult` 撈取）與 OP7（`WaitFor` 回收）是同一個設計的兩半，
漏搬 OP5 會讓 `lstResults` 永遠堆積、GIVEUP 事件永遠不進主 EventLog；
漏搬 OP7 會留下 thread 與 4 個 kernel/CS handle 洩漏。**

**函式歸屬雙樹對帳**（防止插到錯的函式，逐一實測）：

| 接點 | V899 所屬函式 | V910 所屬函式 | 相同 |
|---|---|---|---|
| OP5 | `TfMain::Timer1Timer`（V899:2592） | `TfMain::Timer1Timer`（V910:2709） | ✅ |
| OP6 | `TfMain::FormShow`（V899:8565） | `TfMain::FormShow`（V910:9216） | ✅ |
| OP7 | `TfMain::FormClose`（V899:10782） | `TfMain::FormClose`（V910:11488） | ✅ |
| OP8 | `TfMain::DoStateRecord`（V899:25470） | `TfMain::DoStateRecord`（V910:26578） | ✅ |
| OP10/11 | `TfLotInfo::SetLotEnd`（V899:1851） | `TfLotInfo::SetLotEnd`（V910:1987） | ✅ |
| OP12 | `TfTrayAssignment::ReadFile`（V899:129） | `TfTrayAssignment::ReadFile`（V910:153） | ✅ |
| OP13-16 | `TfFTPClient::UploadFileToServer2`（V899:381） | `TfFTPClient::UploadFileToServer2`（V910:390） | ✅ |

**忠實搬運下仍存在的兩個既有性質（不是本波引入，不改）**：
1. `bEndThread` 是裸 `bool`，跨執行緒讀寫無 `volatile`／無 interlocked。V899 出貨即如此。
2. `TfMain::FormShow` 開頭沒有再入守衛（`bShowFirst` 的兩個賦值點不在此函式）。
   若 FormShow 被重複觸發，`FtpUploadThd = new ...` 會洩漏——**但 `ScanBtnThd = new ScanBtn(false)`
   在同一段、同樣沒有守衛，兩樹皆然**，本波只是沿用既有形狀。

### 0.5 `wininet.lib` 連結問題 —— **明確答案：不需要動 `<LIBFILES>`／`<LIBRARIES>`／`<ALLLIB>`**

這是新增檔搬移最容易踩的雷（新 `.cpp` 引入 `#include <wininet.h>` ＋ `InternetOpen`／`InternetConnect`／
`FtpPutFile`／`FtpCreateDirectory`／`InternetGetLastResponseInfo`）。三項實測：

1. **V899 `.bpr` 沒有任何 `wininet` 字樣**（grep -i 命中 0），`<ALLLIB>` 是
   `$(LIBFILES) $(LIBRARIES) import32.lib cp32mt.lib`。
   `import32.lib` 實測**不含** `InternetOpenA`（byte 掃描 0 次）；
   `$(BCB)\lib\PSDK\wininet.lib` 有，但**沒被列進去**。
   → 提供符號的是 `<LIBRARIES>` 裡已在的 `soaprtl.lib`／`dsnapcon.lib`（兩者實測含 `InternetOpenA`）。
2. **V910 的 `<LIBFILES>` / `<LIBRARIES>` / `<ALLLIB>` 與 V899 逐行位元組完全相同**（實測 3/3 True）。
3. **決定性證據：已出貨的 V899 EXE 真的連結成功了。**
   `D:\HT9045\EXE\HT9045_V3.33.899.35_CASE-PTI-20260810-001.exe`（29,164,032 bytes）
   的匯入表含 `WININET.DLL`（2 次）、`InternetOpenA`、`FtpPutFileA`，且字串區含 `FtpUploadThread`。

→ **`.bpr` 的 lib 段一個字都不用改。** 若 ilink32 仍報 unresolved `_InternetOpenA` 之類，
才在 `<LIBFILES>` 追加 `$(BCB)\lib\PSDK\wininet.lib`（`<LIBPATH>` 已含 `$(BCB)\lib\Psdk`）。

### 0.6 V910 端外部相依存在性（新程式用到的既有符號）

| 符號 | V910 宣告處 | 簽章相容 |
|---|---|---|
| `SaveEventLogInfo` | `cMyDB.h:68` `void __fastcall SaveEventLogInfo(AnsiString, AnsiString, int, AnsiString, AnsiString="")` | ✅ OP5 用 4 引數 |
| `MyDBIProcess` | `cMyDB.h:20` `void __fastcall MyDBIProcess(AnsiString, AnsiString, AnsiString="")` | ✅ OP10 用 3 引數、OP14/15/16 用 2~3 引數 |
| `ReadWriteIni`（bool 多載） | `common.h:52` `bool __fastcall ReadWriteIni(AnsiString,AnsiString,AnsiString,bool,bool,bool)` | ✅ OP2 的 `(CUSTOMER_CODE==CC_PTI)` 當 `DefaultValue` |
| `IniConfig.FtpHost/FtpUserName/FtpPassword/N06_FtpPort/FtpUplaodPath` | `Config.h:975-983` 區 | ✅ |
| `IniConfig.asN12_FtpHost/asN12_FtpUserName/asN12_FtpPassword/asN12_FtpUplaodPath` | `Config.h:1053-1056` | ✅ |
| `IniConfig.bN12_EnableSocketIdProductDataFTP` | `Config.h:1052`（V910 `uLotInfo.cpp:2112` 已在用） | ✅ |
| `LastSet.iSocketContactCount` | 全樹 89 命中 | ✅ |
| `MachineDefine.h` | V910 根目錄存在（2,763 bytes） | ✅ |
| `SOFT_SIMULTE` | `MachineType.h:43` **註解掉**（V899 `MachineType.h:42` 也註解掉） | ✅ 新檔的 `#ifdef SOFT_SIMULTE` 區塊（281-302 行）在正式組建是死碼，兩樹一致 |

`Classes.hpp` / `SyncObjs.hpp`（`TCriticalSection`）/ `<list>` / `<stdio.h>` / `<windows.h>` / `<wininet.h>`
皆為 BCB6 標準標頭，不需新增依賴。

### 0.7 `IniConfig` 加欄位（OP1）的外溢 —— 與 MG-W8 的 `CosFunction` 同型稽核

`bFtpUploadBackground` 插在 `HT9045_CONFIG` 結構**中段**（`bEnableFTP` 之後），會位移其後所有成員 offset。
V910 全樹稽核：

| 樣式 | 命中 |
|---|---:|
| `sizeof(IniConfig` / `sizeof(HT9045_CONFIG` | **0** |
| `memcpy(&IniConfig` / `memset(&IniConfig` | **0** |
| `fread(&IniConfig` / `fwrite(&IniConfig` | **0** |
| `(char*)&IniConfig` / `(void*)&IniConfig` / `&IniConfig,` | **0** |
| `&IniConfig.<member>`（`elConfig->Add` 之類的**逐成員**指標綁定） | 2,243（**只依賴成員位址，不依賴 offset**） |

→ 沒有任何以固定 offset 或整塊二進位存取 `IniConfig` 的地方，加中段欄位不會靜默讀錯成員。
**唯一要求是觸及 `Config.h` 的所有 `.cpp` 一起重編**（BCB6 全量 build 自然滿足），V899 已如此出貨。

### 0.8 事前模擬（記憶體內套用，未落盤）

依 `mg_w10_ops.json` 全部 16 個 op 模擬：

| 檔 | V910 現況 | 套用後 | CRLF | cp950 解碼 | U+FFFD |
|---|---:|---:|---|---|---|
| `Config.h` | 1503 | **1504**（+1） | 1504/1504 | OK | 0 |
| `cprod.cpp` | 4084 | **4086**（+2） | 4086/4086 | OK | 0 |
| `main.cpp` | 35286 | **35339**（+53） | 35339/35339 | OK | 0 |
| `uLotInfo.cpp` | 16713 | **16766**（+53） | 16766/16766 | OK | 0 |
| `cTrayAssignment.cpp` | 1776 | **1806**（+30） | 1806/1806 | OK | 0 |
| `KYECFTP\FTPClient.cpp` | 5440 | **5454**（+14） | 5454/5454 | OK | 0 |

- 六個檔在 V910 皆為 **100% CRLF、0 條 bare-LF**（無 EOL 混合風險）。
- **16 個 op 的錨點檢查全部 `ANCHOR_OK=True`，且 16 個 `anchor_expect` 在各自 V910 檔內都是唯一（UNIQ=1/16）。**
- **16 個 payload 區段在結果中各出現「恰好一次」。**
- 5 個 `replace` op 的「不變前後綴」逐位元組實測相同（OP6 兩段、OP13 一段、OP14 一段、OP15 兩段）。

**移除行帳目：`replace` 共取代 50 行，其中 49 行原樣重新出現在 payload 內
（**37 行位元組完全相同**、**12 行只是縮排 +4** 移進新的 `else` 分支），
全波「真正被移除的敘述」只有 1 行：**

```
KYECFTP\FTPClient.cpp   NMFTP2->TimeOut  =20000;   //Landam
        → 換成 NMFTP2->TimeOut =(CUSTOMER_CODE==CC_PTI)?5000:20000;   （OP13，刻意）
```

**決定性驗收（結果 vs V899 出貨版，區段級比對）**：

| 區段 | 結果 vs V899 差異 | 說明 |
|---|---|---|
| `uLotInfo.cpp` `SetLotEnd` FTP 區（V899 1904-2026） | **只有 1 行** | `fSortCT->btnClearCountClick(fSortCT);`——V910 是活的、V899 註解掉，**既有公司漂移，刻意不碰**。其餘 122 行位元組相同。 |
| `KYECFTP\FTPClient.cpp` `UploadFileToServer2` 全函式 | 8 個 hunk | 套用前是 12 個 hunk，**4 個 0609 hunk 全數消失**；剩 8 個全是 V910 公司內容（Eastsun 20260522 ATC File Transfer、RogerYang 20260616 JCET `_QA`、2 個空行差） |
| `cTrayAssignment.cpp` ReadFile 迴圈＋新區塊 | **只有 1 行** | `FromBuffer` 那行的**註解對齊空白**差（V910 多 6 空格），既有漂移 |
| `main.cpp` 五個區段 | **0** | 全域定義／Timer1／FormShow／FormClose／DoStateRecord 五處逐行目檢與 V899 相同 |

---

## 1. 各變更明細（依主題分段）

### 1.1 新增檔（**不做 op，整檔複製**）

| 檔 | 行數 | bytes | EOL | 編碼 |
|---|---:|---:|---|---|
| `FTPUpload\uFtpUploadThread.h` | 88 | 4,801 | 100% CRLF | Big5(cp950)，566 個非 ASCII 字元，U+FFFD=0 |
| `FTPUpload\uFtpUploadThread.cpp` | 498 | 20,204 | 100% CRLF | Big5(cp950)，1,469 個非 ASCII 字元，U+FFFD=0 |

**複製方式（必須位元組級，不可經 Edit/Write 工具，不可讓 git autocrlf 介入）**：

```
mkdir "D:\HT9045\HT9011UC_Code_V3.33.910.0_20260716_Jimmy\FTPUpload"
copy /B "D:\HT9045\HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422\FTPUpload\uFtpUploadThread.h"   "D:\HT9045\HT9011UC_Code_V3.33.910.0_20260716_Jimmy\FTPUpload\uFtpUploadThread.h"
copy /B "D:\HT9045\HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422\FTPUpload\uFtpUploadThread.cpp" "D:\HT9045\HT9011UC_Code_V3.33.910.0_20260716_Jimmy\FTPUpload\uFtpUploadThread.cpp"
```

複製後對帳：兩邊 md5 相同、bytes 分別為 4801／20204。
（新增檔**不會有 `.mgbak`**，`port_check.py` 對它們沒有基準可比——見 §5 gate 說明。）

**內容摘要（供複驗，非重述程式碼）**：
- `TFtpUploadJob`：8 個欄位（local/remote/host/user/pwd/port/retry/jobKind）。
- `TFtpUploadThread`：`eJob`/`eEnd` 兩個 manual-reset event、`csQueue` 保護 `lstJobs`＋`sInFlightKey`、
  `csResult` 保護 `lstResults`（兩把鎖分開，無鎖序問題）。
- 去重鍵＝`"%d|%s"`(jobKind, localFile)；`sRemotePath` 刻意不納入。
- 上傳＝WinINet `InternetOpen`→`InternetConnect(PASSIVE)`→`FtpCreateDirectory`(best-effort)→
  `FtpPutFile(BINARY)`；connect timeout 5000ms；重試間隔 `::Sleep(2000)`。
- 背景 log＝`D:\HT9045_Log\FtpUpload\YYYYMMDD\FtpUploadThread_YYYYMMDD.log`，
  純 `CreateDirectory`＋`fopen`（刻意不用 `TMyStringList`／`MyDBIProcess`，非 thread-safe）。
- 只有 `GIVEUP` 才 `PushResult` 給主執行緒（成功與中途失敗只寫背景 log）。

### 1.2 開關（`Config.h` ＋ `cprod.cpp`）→ **OP1 ＋ OP2**

- **OP1**：`Config.h:957`（1 行）→ V910 `after` 975（`bool       bEnableFTP;`，全檔唯一）。
- **OP2**：`cprod.cpp:2245-2246`（2 行：註解＋`ReadWriteIni`）→ V910 `after` 2299
  （`"FTP", "Enable FTP",`，全檔唯一）。
  - **2246 是未標記承重行**：矩陣只列 2245（註解）。漏掉 → `bFtpUploadBackground` 永遠是
    `InitialConfig` 的值、ini 不生效，等於整個功能對所有客戶失效。
- **預設值語意**：`ReadWriteIni(..., IniConfig.bFtpUploadBackground, (CUSTOMER_CODE==CC_PTI), bRead)`
  → **PTI 預設 true、其餘客戶預設 false**；且此段包在
  `if(CosFunction.bFTPFunction || IniConfig.bFTPJamCodeUpload)` 內（兩樹同，未改）。
- **信心：≥95%**

### 1.3 執行緒宿主（`main.cpp` 五處）→ **OP3–OP8**

| op | 內容 | V899 範圍 | V910 錨點 | 模式 |
|---|---|---|---|---|
| OP3 | `#include` | 180（1 行） | `after` 180 = `#include "AGV.h"`（唯一） | after |
| OP4 | 全域單例定義 | 215-217（3 行） | `before` 216 = `HWND HReceveFuleWnd;`（唯一） | before |
| OP5 | Timer1 撈 GIVEUP 回報 | 2630-2643（14 行） | `after` 2746 = `StringGrid4->Cells[0][0]=2;`（唯一） | after |
| OP6 | FormShow 建立 thread | 9970-9978（9 行） | `replace` [10645,10648]，anchor `ScanBtnThd = new ScanBtn(false);`（唯一） | replace |
| OP7 | FormClose 回收 thread | 10916-10925（10 行） | `before` 11623 = `LogSoftwareOffTime("TfMain, DoReleaseAndInspEnd")`（唯一） | before |
| OP8 | DoStateRecord 打包 log | 25672-25691（20 行） | `before` 26784 = `if(bRunAutoClean && TestIF_File.iAutoClean_Function)`（唯一） | before |

- **OP4 為何 `before` 而不是 `after 214`**：`ScanBtn *ScanBtnThd;` 雖唯一，但 `HReceveFuleWnd` 那行
  同樣唯一且位置更貼合 V899 佈局（V899:218 就是它）。套用後結果與 V899 213-218 逐行相同。
- **OP6 為何用 `replace`**：自然插入點在 `ScanBtnThd=NULL;` 之後，但該字串在 V910 命中 **2 次**
  （10645 與 12102，後者只是縮排更深，含 substring）；`//----- by dell ccd realtime---` 命中 **7 次**；
  `if(REAL_TIME_CCD)` 也非唯一。往下包到 `ScanBtnThd = new ScanBtn(false);` 才拿到唯一錨。
  被取代的 4 行與 V899 9970/9976/9977/9978 **位元組完全相同**（實測），等同原地重插。
  另外 V910 在 `ScanBtnThd=NULL;` 與 `if(REAL_TIME_CCD)` 之間**沒有空行**、V899 有，
  payload 一併把空行帶回，結果與 V899 完全一致。
- **OP8 的地形陷阱（重要）**：V899 這段緊接在 **20260414 MNetLog 打包區塊**（`main.cpp:25658-25670`）之後，
  但 **V910 沒有那個區塊**——`bN10_DailyUploadProdData` 在 V910 `main.cpp` 只命中 1 次（11571，別的函式），
  且 `main.cpp:25658` 本身就是另一條 MISSING（20260414，別波）。
  因此錨點**不能**用 MNetLog 區塊，改用其後的 `bRunAutoClean` 那行（V910 全檔唯一）。
  套用後 FTP 區塊會落在 OLP Automation 區塊與 AutoClean 區塊之間，
  與 V899 的相對位置一致（只是中間少了尚未搬的 MNetLog 區塊）。
- **未標記承重行**：OP4 的 `TFtpUploadThread *FtpUploadThd = NULL;`（V899:217）**沒有 `//AI` 標記**
  ——矩陣只列 215。漏搬 → **link error（`FtpUploadThd` unresolved）**。這是本波最容易漏的一行。
  OP5/OP7/OP8 的整段函式本體（13／8／19 行）同樣無標記。
- **信心：≥95%**（六個 op 錨點全檔唯一，函式歸屬雙樹對帳一致）

### 1.4 生產端（`uLotInfo.cpp`）→ **OP9–OP11**

- **OP9**：`#include`，`after` V910:76（`#include "AGV.h"`，唯一）。
- **OP10**：`uLotInfo.cpp:1933-1971`（39 行）取代 V910 `[2076,2084]`（9 行，淨 **+30**）。
  anchor `fFTPClient->UploadFileToServer2(IniConfig.FtpUplaodPath, asJamCodeFileName);`（全檔唯一）。
  - **為何用 `replace`**：V899 把原本的同步迴圈整段**移進 `else` 分支**並縮排 +4。
    被取代的 9 行不是「被刪掉」，而是**原樣（僅縮排改變）保留在 payload 的 `else` 裡**（逐行實測）。
  - 內含三個日期：0612（背景分支）、0630（`FtpUploadThd==NULL` 記 log 跳過）、0611（`bError` 清旗標與早退）。
  - **未標記承重行 22 行**，最關鍵的是
    `FtpUploadThd->EnqueueUpload(0, asJamCodeFileName, asBgRemote, asBgHost, asBgUser, asBgPwd, iBgPort, 2);`
    （1947-1948，兩行一句）——矩陣只列 1946（組 remote 路徑的註解行）。
- **OP11**：`uLotInfo.cpp:2001-2024`（24 行）取代 V910 `[2114,2115]`（2 行，淨 **+22**）。
  anchor `UpSocketIdPoductDataToServerByFTP`（全檔唯一）。
  - 同樣是「原同步兩行移進 `else`、縮排 +4」。
  - **未標記承重行 17 行**，關鍵是背景分支的 `EnqueueUpload(1, ...)`（2008-2009）
    與**呼叫端補清 `LastSet.iSocketContactCount` 的 4×8 雙迴圈**（2011-2017）。
    後者是「決議 2：丟佇列即清」——因為背景分支不再呼叫
    `UpSocketIdPoductDataToServerByFTP()`，而清除碼原本在那支函式的成功路徑裡。
    **漏搬這段 → PTI 的 SocketID contact count 永遠不歸零**。
- **公司漂移（刻意不碰）**：V910 `uLotInfo.cpp:2062` 的 `fSortCT->btnClearCountClick(fSortCT);` 是活的，
  V899:1919 是註解掉的。這行**在兩個 op 的區段之外**，本波零位元組變動。
- **信心：≥95%**

### 1.5 同步路徑非阻塞化（`KYECFTP\FTPClient.cpp`，0609）→ **OP13–OP16**

四處全在 `TfFTPClient::UploadFileToServer2()`，全部 `CUSTOMER_CODE==CC_PTI` gated。

| op | 語意 | V899 | V910 replace | anchor（全檔唯一） |
|---|---|---|---|---|
| OP13 | `TimeOut` 20000 → `(CC_PTI)?5000:20000` | 390-401 | [399,410] | `AnsiString ActivePath=FileListBox1->Directory+"\\";` |
| OP14 | 連線失敗：PTI 走 `MyDBIProcess` log，非 PTI 維持 `ShowMyMessage` modal | 459-478 | [469,485] | `                    NMFTP2->Connect();` |
| OP15 | `catch(Exception&)`：新增 `else if(CC_PTI)` 只記 log，**不讀 `LastErrorNo`** | 792-799 | [819,822] | `NewRecordProcess("Message", str, e.Message);` |
| OP16 | `catch(...)`：新增 `if(CC_PTI)` 只記 log，原兩行降級進 `else` | 815-823 | [838,839] | `MyDBIProcess("Exception", str);` |

- **為何四個都用 `replace` 且範圍比修改點大**：這支函式裡
  `if(!NMFTP2->Connected)`（9 次）、`ShowMyMessage("FTP Server is not connected","")`（8 次）、
  `NMFTP2->TimeOut = 20000; //Landam`（4 次）、`NMFTP2->Vendor = NMOS_AUTO;`（5 次）
  **全部不唯一**（同一檔內有 8 支類似的 FTP 函式）。往外擴到帶有唯一字串的區段才拿得到錨。
  OP13/14/15 的「不變前後綴」逐位元組實測與 V899 相同。
- **不讀 `LastErrorNo` 是重點**：`NMFTP2` 在例外路徑可能已是異常 socket，
  讀 `LastErrorNo` 本身會再觸發存取違例（V899 註解已載明）。
  非 PTI 的 `else` 分支**照舊會讀**——忠實搬運，不順手修別的客戶。
- **公司漂移（在 op 區段外，零變動）**：Eastsun 20260522 ATC File Transfer（V910:131-152 相對段）、
  RogerYang 20260616 JCET `_QA`（V910:326-334 相對段）。
  套用後全函式對 V899 仍有 8 個 hunk，全部是這些 V910 專屬內容。
- **信心：≥95%**

### 1.6 `cTrayAssignment.cpp:358`（0611，Fix 下半盤方向）→ **OP12** —— 本波唯一無客戶碼閘門的 op

- **V899 範圍**：`cTrayAssignment.cpp:358-387`（30 行 = 1 行帶註解的 `if` ＋ 28 行本體 ＋ 1 行空行）
- **V910 錨點**：`before` 443 = `if(bUseAuto2Empty==true)`（全檔唯一）
- **矩陣只列 358 那一行；358 以外的 28 行全部無標記**——本波第二危險的漏搬點（第一是 `main.cpp:217`）。

**為什麼判 ≥95%（四項實測，不是推論）**：

1. **它只補洞，不可能覆寫任何從 ini 讀進來的值。**
   上面的迴圈是 `for(int i=0; i<=iFixRight; i++)`（V910:406）。`iFixRight` 兩個賦值點在
   `main.cpp:1822`／`1842`，且**與新區塊自己的 `if(AUTO_EMPTY_COLOR>=3)` 是同一個條件**：

   | `AUTO_EMPTY_COLOR` | `iFixRight` | 迴圈涵蓋 | 新區塊寫入 | 重疊 |
   |---|---|---|---|---|
   | `>=3` | `eFix6`(=11) | `eAuto1..eFix6` | `eFix7..eFix12`(12..17) | **無** |
   | `<3` | `eFix3`(=8) | `eAuto1..eFix3` | `eFix4..eFix6`(9..11) | **無** |

   （enum 值取自 `MachineType.h:1126-1143`。）

2. **V910 全樹對 `TrayForm.Auto[eFix4..eFix12].Direction／.iTrayType` 的賦值命中數＝0。**
   → 沒有公司自製的反向修法會被覆蓋，也沒有第二個寫入者會打架。V899 端則恰好是這 18 行。

3. **它是既有已出貨模式的補完。** V910 `cTrayAssignment.cpp:197-222` 早就有
   `bTrayUpDownSet` 的同構鏡像：同樣 `if(TrayForm.iFixTrayMode==1)`、同樣 `if(AUTO_EMPTY_COLOR>=3)` 分岔、
   同樣 `eFix7←eFix1 … eFix12←eFix6` ／ `eFix4←eFix1 … eFix6←eFix3` 配對。
   V899 註解自陳「此處仿 `bTrayUpDownSet` 同步」——實測**逐個索引配對完全一致**。

4. **閘門是工作檔設定，不是客戶碼。** `TrayForm.iFixTrayMode` 來自
   `ReadIniData(szDir, "Flag", "Fix Tray Mode", 0)`（兩樹同，預設 **0**，`//wei 20160224 TSMC FIX UPDOWN`）。
   `iFixTrayMode!=1` 的機台，新區塊一條敘述都不執行。

**因此不加客戶碼隔離**——理由與 **MG-W3 的「解析端不加 gate 照搬」同型**：
V899 是量產出貨版且已 un-gated 出貨，而本區塊在非 Fix-Up/Down 機台是完全不執行的死路徑，
在 Fix-Up/Down 機台則是把「從未被寫過、恆為 0」的欄位補成正確鏡像。
**這仍是本波唯一需要主腦點頭的判斷**（見 §3.C）。

**一個無害的邊角（記錄，不處置）**：V910 有 `Ifor 20260702` 的 AOI 區塊
（`cTrayAssignment.cpp:129-152`），在 `USE_Scanner_AOI_Inspection==eBtnAOI_TopBottomInstall` 時
把 `Prod.iTrayType[eFix2/eFix3/eFix8/eFix9]` 設 `tNotUse`。
OP12 仍會把 `TrayForm.Auto[eFix9].iTrayType` 設成 `TrayForm.Auto[eFix3].iTrayType`——
但 (a) 兩者是不同陣列（`Prod.iTrayType` 是「用途」、`TrayForm.Auto[].iTrayType` 是「盤型」），
(b) `eFix9` 既然是 `tNotUse`，該值不會被消費，
(c) 既有的 `bTrayUpDownSet` 鏡像（V910:214）本來就是同樣做法。**無衝突。**

---

## 2. 非 PTI 路徑影響的證明

**本波是 B 類，gate 全部沿用 V899 原生條件，未新增任何 `CUSTOMER_CODE` 判斷。**

| 消費點 | 非 PTI 時 | 指令級影響 |
|---|---|---|
| `IniConfig.bFtpUploadBackground`（OP2） | ini 預設 `(CUSTOMER_CODE==CC_PTI)`＝**false** | 恆 false |
| `uLotInfo` JamAlarm 分流（OP10） | `false` → 落到 `else` | **`else` 主體＝V910 現行同步碼，逐敘述相同（僅縮排）**，外加 0611 的 `bError` 清旗標與早退 |
| `uLotInfo` SocketID 分流（OP11） | `false` → 落到 `else` | 同上，`MySleep(1000)`＋原函式呼叫一字不差 |
| `main.cpp` DoStateRecord（OP8） | `if(IniConfig.bFtpUploadBackground)`＝false | 不進 body，`TestList` 一條指令都不加 |
| `main.cpp` Timer1 撈回報（OP5） | `FtpUploadThd!=NULL` 為 **true**（所有客戶都建立），但佇列恆空 | 每秒多一次「取一把 CS、看 list 空、放鎖、回 false」＝ 1 次空撈 |
| `main.cpp` FormShow／FormClose（OP6/OP7） | **無條件建立與回收**（V899 註解明寫「所有客戶都建立」） | 多一條閒置 thread，恆阻塞在 `WaitForMultipleObjects(INFINITE)` |
| `FTPClient` 四處（OP13-16） | `CUSTOMER_CODE!=CC_PTI` → 走 `else` | `TimeOut` 仍 20000；連線失敗仍彈 `ShowMyMessage` modal；例外仍讀 `LastErrorNo`——**位元組級與 V910 現況相同** |
| `cTrayAssignment`（OP12） | `iFixTrayMode!=1`（預設 0） → 不進 body | 執行 0 條敘述 |
| `IniConfig` 加欄位（OP1） | — | §0.7 實測無任何佈局相依存取，僅需全量重編 |

**唯二可量測的非 PTI 成本**：
1. 一條永遠閒置的背景 thread（2 個 event handle ＋ 2 個 CRITICAL_SECTION）。
2. `Timer1Timer`（1 秒）每 tick 多一次空的 `FetchResult()`。

**忠實照搬 V899 出貨行為，不做最佳化、不加額外客戶碼。**

---

## 3. 真實行為變更（**必須記入 LEDGER**）

| # | 變化 | 來源 | 影響面 |
|---|---|---|---|
| **A** | **PTI 的 Lot End JamAlarm 與 SocketID CSV 上傳改為 fire-and-forget**：丟佇列即返回，不 `MySleep`、不等成功 | OP10/OP11 | **語意變更：Lot End 完成 ≠ 上傳完成。** 這是整案的目的（解 Lot End 卡住／alarm 消不掉／10038） |
| **B** | **PTI 的 SocketID contact count 改為「丟佇列即清」** | OP11（呼叫端補清 4×8 迴圈） | 上傳失敗時 count 已清，計數不會重傳。V899 決議 2，忠實搬運 |
| **C** | **`cTrayAssignment` Fix 下半盤方向補齊：對所有客戶生效（僅 `iFixTrayMode==1` 機台）** | OP12 | 唯一無客戶碼閘門的變更。§1.6 已證只補洞不覆寫、V910 無第二寫入者、與既有 `bTrayUpDownSet` 鏡像同構。**建議照搬，但需主腦點頭** |
| D | PTI 同步 FTP `TimeOut` 20s → 5s | OP13 | 只影響 `UploadFileToServer2` 這一支；其餘 7 支 FTP 函式的 timeout 不變 |
| E | PTI 連線失敗／例外不再彈 modal，改寫 `MyDBIProcess` log | OP14/15/16 | **操作員看不到 FTP 失敗彈窗了**；改由背景 log ＋（背景路徑）主 EventLog `FTP_UPLOAD_FAIL`（iType=20）呈現 |
| F | 新增磁碟寫入路徑 `D:\HT9045_Log\FtpUpload\YYYYMMDD\*.log`（**所有客戶**，只要 thread 有動作） | 新增檔 | 非 PTI 佇列恆空 → 只會有 `FtpUploadThread started.` / `stopped.` 兩行/次開機 |
| G | StateRecord 多打包一個 `FtpUpload.7z`（**只在 `bFtpUploadBackground` 為 true 時**） | OP8 | PTI 的 StateRecord 變大；非 PTI 不變 |
| H | 主 EventLog 新增 alarm code `FTP_UPLOAD_FAIL`（iType=20 WARNING，不跳 alarm、不計錯誤統計） | OP5 | 只有 PTI 會出現。**`Error\<語系>\FTP_UPLOAD_FAIL.dat` 說明檔兩樹皆無，屬既有狀態，不在本波處理** |

---

## 4. `.bpr` 變更清單（**列給主腦手動確認，本分析未修改 `.bpr`**）

目標檔：`D:\HT9045\HT9011UC_Code_V3.33.910.0_20260716_Jimmy\HT9045.bpr`（cp950，710 行）
**六處新增，零處刪除；`<LIBFILES>` / `<LIBRARIES>` / `<ALLLIB>` 一律不動（§0.5）。**

| # | 位置 | 現況 | 改成 |
|---|---|---|---|
| 1 | `<OBJFILES>`，第 **69** 行 | `      ..\Obj\CCDInterface.obj ..\Obj\FTPClient.obj ..\Obj\LtcSensor.obj ` | 在 `..\Obj\FTPClient.obj ` 之後插入 `..\Obj\uFtpUploadThread.obj `（V899 第 67 行正是此形狀） |
| 2 | `<PATHCPP>`，第 **174** 行 | `...;CCLink;KYECFTP;&quot;ASE_K Socket&quot;;...` | `KYECFTP;` 之後插入 `FTPUpload;` |
| 3 | `<INCLUDEPATH>`，第 **184** 行 | `...;FingerprintRead;KYECFTP;Automation;...` | `KYECFTP;` 之後插入 `FTPUpload;` |
| 4 | `<LIBPATH>`，第 **185** 行 | `...;FingerprintRead;KYECFTP;Automation;...` | `KYECFTP;` 之後插入 `FTPUpload;` |
| 5 | `<IDLCFLAGS>`，第 **197** 行 | `      -IKYECFTP -IAutomation -ICCLink -IBinDisplay -IInterface -IATC -IEJ1N ` | `-IKYECFTP ` 之後插入 `-IFTPUpload `（**與 V899 第 194 行對齊；IDL 編譯器旗標，非 C++ 編譯必需，但保持與 V899 一致**） |
| 6 | `<FILELIST>`，第 **413** 行之後新增一行 | （413＝`<FILE FILENAME="KYECFTP\FTPClient.cpp" .../>`） | 插入：<br>`      <FILE FILENAME="FTPUpload\uFtpUploadThread.cpp" FORMNAME="" UNITNAME="uFtpUploadThread" CONTAINERID="CCompiler" DESIGNCLASS="" LOCALCOMMAND=""/>`<br>（**逐字取自 V899 第 402 行**；縮排 6 空格；`.h` 不需要 FILELIST 條目——V899 也沒有） |

**可選（不影響建置）**：V910 第 **591**／**624** 行的 `Item0=`（`[HistoryLists]` IDE 目錄歷史）
在 V899 對應行含 `FTPUpload;`。純 IDE UI 記憶，`bpr2mak` 不讀，**建議不動**（改了會製造無意義 diff）。

**確認方式**：改完後 `grep -c "FTPUpload" HT9045.bpr` 應為 **5**（第 1 項是 `uFtpUploadThread.obj` 不含 `FTPUpload` 字串，
第 2–5 項各 1 次、第 6 項 1 次 → 精確值請以 `grep -o` 逐條核對），
且 `grep -c "uFtpUploadThread" HT9045.bpr` 應為 **2**（obj ＋ FILE），與 V899 相同。

---

## 5. 交付與收工 gate 建議（給主腦）

1. **先建目錄＋複製兩個新增檔**（§1.1 的 `copy /B`）。md5 對帳後再跑 splice。
2. `python tools/port_tools/splice.py docs/mg_w10_ops.json`
   - 預期輸出（6 檔）：
     ```
     OK Config.h: 1 op(s) applied, eol=CRLF
     OK cprod.cpp: 1 op(s) applied, eol=CRLF
     OK main.cpp: 6 op(s) applied, eol=CRLF
     OK uLotInfo.cpp: 3 op(s) applied, eol=CRLF
     OK cTrayAssignment.cpp: 1 op(s) applied, eol=CRLF
     OK KYECFTP\FTPClient.cpp: 4 op(s) applied, eol=CRLF
     ```
   - 套用後行數應為 **1504 / 4086 / 35339 / 16766 / 1806 / 5454**（§0.8）
   - 六檔各自產生 `.mgbak`
3. `python tools/port_tools/port_check.py Config.h cprod.cpp main.cpp uLotInfo.cpp cTrayAssignment.cpp "KYECFTP\FTPClient.cpp"`
   - 預期：`added` 全數 **SPLICED**、`authored=0`、`comment=0`
   - EOL：`(1503,0)->(1504,0)`、`(4084,0)->(4086,0)`、`(35286,0)->(35339,0)`、
     `(16713,0)->(16766,0)`、`(1776,0)->(1806,0)`、`(5440,0)->(5454,0)`
   - **`REMOVED` 應為 50 行，但其中 49 行會同時出現在 `added` 裡**（37 行位元組相同、12 行縮排 +4）。
     **真正消失的敘述只有 1 條**，逐條複驗時只要確認它是：
     `NMFTP2->TimeOut                 =20000;                                 //Landam`
   - 兩個新增檔**沒有 `.mgbak`**，port_check 無基準；改用 md5 對 V899 原檔即可。
4. ⚠ **`tools/port_tools/bcc_syntax.sh` 的 `INC` 沒有 `FTPUpload`**（實測），
   直接跑會讓 `main.cpp` / `uLotInfo.cpp` 報 `Unable to open include file 'uFtpUploadThread.h'`
   ——**那不是搬移失敗，是 gate 設定缺口**。
   請在該腳本的 `INC=` 字串裡把 `KYECFTP;` 改成 `KYECFTP;FTPUpload;`（與 §4 第 3 項同步），再跑：
   ```
   sh tools/port_tools/bcc_syntax.sh main.cpp uLotInfo.cpp cprod.cpp cTrayAssignment.cpp 'KYECFTP\FTPClient.cpp' 'FTPUpload\uFtpUploadThread.cpp'
   ```
   - `Config.h` 由 `cprod.cpp` 帶入；`uFtpUploadThread.h` 由新 `.cpp` 帶入。
   - `main.cpp` 有已知既有錯誤，驗收準則是**錯誤集合與 `.mgbak` 基準相同**，不是零錯誤。
   - `FTPUpload\uFtpUploadThread.cpp` **沒有 `.mgbak`**，所以它必須**零錯誤**才算過。
   - `Config.h` 是廣泛被 include 的定義檔，加欄位後全量 build 時所有相依 `.cpp` 都會重編；波內只用 `-c` 隔離編譯。
5. **`.bpr` 六處變更**（§4）——**請主腦手動套用並確認格式**（本分析刻意不改）。
6. 主腦逐條開檔複驗，**重點四處**：
   - **`main.cpp` 的 `TFtpUploadThread *FtpUploadThd = NULL;` 在不在**（套用後約第 219 行）。
     無標記、漏了就 link error，是本波最大單點風險。
   - **`uLotInfo.cpp` 的兩個 `else` 分支主體**：套用後必須完整保留原同步碼
     （`MySleep(1000)` ＋ `UploadFileToServer2` ／ `UpSocketIdPoductDataToServerByFTP`），
     且 `fSortCT->btnClearCountClick(fSortCT);`（V910 約 2062 行）**仍是活的、未被註解掉**。
   - **`cTrayAssignment.cpp` 新區塊的 28 行本體全在**（套用後約 443-471 行），
     且其後緊接 `if(bUseAuto2Empty==true)`。
   - **`KYECFTP\FTPClient.cpp` 的公司內容未被動到**：套用後 `Eastsun 20260522 整合` 的 ATC File Transfer
     區塊與 `RogerYang 20260616 : 新增QA` 都必須完整存在。
7. **不要為本波新增 `//AI(mg899to910)` 註解行**——port_check 會把它們判成 `authored`／`comment`，
   而 gate 準則是 `authored=0 / comment=0`（沿用 MG-W8 做法）。V899 原註解是矩陣驗收簽章，原樣保留。
8. 收工後 `python tools/port_tools/ai_comment_matrix.py`，MISSING 應**減少 58 條**。
   基數會漂移（MG-W9 並行推進中），**只驗「減 58 條、且這 58 條正是本波的」**：

   | 檔 | 行 |
   |---|---|
   | `FTPUpload\uFtpUploadThread.cpp` | 4, 5, 7, 18, 22, 28, 41, 44, 54, 63, 101, 111, 128, 166, 214, 245, 258, 273, 282, 298, 348, 442, 480（23 條，整檔複製） |
   | `FTPUpload\uFtpUploadThread.h` | 4, 11, 15, 44, 48, 61, 71, 79（8 條，整檔複製） |
   | `uLotInfo.cpp` | 76, 1933, 1935, 1937, 1946, **1952**, 1957, 1959, 1967, 2001, 2003, 2005, 2011, 2020（14 條） |
   | `main.cpp` | 180, 215, 2631, 9972, 10916, 25672（6 條） |
   | `KYECFTP\FTPClient.cpp` | 401, 475, 795, 815（4 條） |
   | `Config.h` / `cprod.cpp` / `cTrayAssignment.cpp` | 957 / 2245 / 358（各 1 條） |

9. **commit 後清 V910 樹 `*.mgbak`**（本波六檔）。
   注意：分析時樹上已有 MG-W9 未清的四個 `.mgbak`
   （`CosFunction.cpp/.h`、`acatchtray.cpp`、`ainarm9045.cpp`），與本波無關，由 MG-W9 收工時處理。
10. LEDGER 需記：§3 的 A–H 八項，**特別是 A（Lot End 完成 ≠ 上傳完成的語意變更）、
    B（丟佇列即清 count）、C（OP12 無客戶碼閘門）與 E（PTI 不再彈 FTP 失敗 modal）**。

---

## 6. 結論

- **V910 有沒有 `uFtpUploadThread`：沒有，連 `FTPUpload\` 目錄都不存在，`.bpr` 也零命中。
  本波是「2 個新增檔整檔複製 ＋ 16 個 splice op ＋ 6 處 `.bpr` 變更」。**
- **58 條矩陣紀錄**（0612 × 50、0611 × 3、0609 × 4、**0630 × 1**）→
  **16 個 op（7 `replace`、5 `after`、4 `before`）＋ 2 個新增檔**。
  錨點全部經全檔唯一性實測（**16/16 唯一**）。
- 搬移總行數 **789**：新增檔 586 行（88 + 498）＋ op payload 203 行。
  op payload 拆帳：矩陣標記 **27** 行、**未標記新碼 127 行**、為錨點唯一性重插的既有行 **49** 行。
  → **矩陣盲區同型第四例**（W6／W8 之後）：本波「有標記」只佔搬移量的 3.4%（27/789）。
- **無 <95% 項目。** 16 個 op 與 2 個新增檔信心全部 ≥95%。
  唯一需要主腦點頭的判斷是 **OP12 不加客戶碼隔離**（§1.6／§3.C），
  依據是 MG-W3「V899 已出貨且無 gate → 忠實照搬」的先例，
  加上四項實測（只補洞不覆寫、V910 零第二寫入者、與既有 `bTrayUpDownSet` 鏡像同構、閘門是工作檔非客戶碼）。
- **執行緒生命週期 15 個接點全部到齊**（§0.4），且**設計上完全不用 `Synchronize()`**
  ——結果佇列＋主執行緒 `Timer1Timer` 撈取，正是為了避開 `FormClose` 的 `WaitFor()` 死結。
  最易漏的兩處是**無標記**的 `main.cpp:217`（全域單例定義，漏＝link error）
  與 `cTrayAssignment.cpp:359-386`（28 行函式本體，漏＝功能整段不存在）。
- **`wininet.lib` 不用加**：V899 `.bpr` 的 lib 段與 V910 逐行位元組相同，
  而已出貨的 V899 EXE 匯入表證明 `WININET.DLL` 確實連上了（`soaprtl.lib`／`dsnapcon.lib` 提供）。
- **全波真正被移除的敘述只有 1 行**（`NMFTP2->TimeOut=20000;`，OP13 刻意取代）。
  其餘 49 行「removed」都原樣重新出現在 payload 內（37 行位元組相同、12 行只是縮排 +4）。
- **公司漂移零損傷**：`uLotInfo` 的 `fSortCT->btnClearCountClick`、
  `FTPClient` 的 Eastsun 20260522 ATC File Transfer 與 RogerYang 20260616 JCET `_QA`、
  `cTrayAssignment` 的 Ifor 20260702 AOI 區塊，全部在 op 區段外，零位元組變動。
- **不屬本波**：20260630 的 State Record handle 線（`main.cpp`×6／`csystem.cpp`×3／
  `cpublic.cpp`×2／`cpublic.h`／`mymessbox.cpp`×2／`note.cpp`／`HS_Function.cpp`×2，共 17 條）；
  20260414 的 `main.cpp:25658` MNetLog 打包（**OP8 的錨點特意繞開它**）。
