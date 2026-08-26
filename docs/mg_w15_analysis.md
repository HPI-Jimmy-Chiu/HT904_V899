# MG-W15 分析：機能群收尾（0623 cConfiguration／0625 TesterTCP／0629／0525／0514／0706／0414）

- **波次**：MG-W15（V899 → V910），**20 條零星矩陣點、7 個日期叢集、6 個檔**
- **來源**：`docs/mg_ai_matrix_missing.csv`（全表 40 條 MISSING，本波取 20 條）
- **V899 樹**：`D:\HT9045\HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422`
  （唯讀；本波 6 個來源檔 `git status --porcelain` 空、`git diff b515ed5 HEAD` 對這 6 檔為空 → 工作區＝在製收斂端點 `b515ed5`）
- **V910 樹**：`D:\HT9045\HT9011UC_Code_V3.33.910.0_20260716_Jimmy`
  （分析起手時 MG-W14 後 `git status` 乾淨、`*.mgbak` 0 個；**分析期間有兄弟波併行改了另外 6 個檔，
  與本波零交集，詳 §⚠7**）
- **本文件性質**：分析＋splice op 草案。**全程唯讀，兩棵樹一個位元組都沒有被修改**（模擬結果只寫進 scratchpad `w15_sim\`）
- **op 檔**：`D:\HT9045\docs\mg_w15_ops.json`（**12 個 op、6 個檔**）
- **結果**：**18 條搬移（12 op）＋ 2 條 C 類 allowlist ＝ 20/20 全數結清**
- **`<95%` 清單：空**（理由見 §6）

---

## ⚠ 開工前必須先知道的七件事

1. **`V910\MachineType.h` 是 100% bare-LF，不是 CRLF。**
   位元組實測：`CRLF=0 / LF=1671`（同檔在 V899 是 `CRLF=1647`）。而且 `git show HEAD:` 取出的 blob
   與磁碟**位元組完全相同** → 它在 repo 裡本來就是 LF，不是 working-tree 假象。
   `splice.py` 的 `dominant_eol()` 會自動把 payload 轉成 LF，模擬後 `CRLF=0/1680` 維持純 LF。
   **這與 MG-W11 的 `AutoClean.cpp` 是同一類地雷**；用 `grep -c $'\r$'` 量 EOL 會給出錯誤答案（實測它對這支檔誤報 1671），**只信位元組計數**。

2. **`git show HEAD:` 對本波另外 5 個檔全部是 LF-normalized，與磁碟不同。**
   實測：`cConfiguration.cpp / main.cpp / ainarm9045.cpp / TesterTCP.cpp / TesterTCP.h`
   磁碟是 CRLF、blob 是 LF。`port_check.py` 沒有 `.mgbak` 時會 fallback 到 `git show HEAD`
   → 會產生整檔 EOL 假差異。**splice 第一次觸碰就會建 `.mgbak`，所以正常流程（splice → port_check）沒問題；
   但絕對不要在 splice 之前先跑 port_check，也絕對不要對 V910 路徑 `git checkout`。**

3. **`0625 TesterTCP` 的 case 歸屬，任務書上寫錯了。**
   不是長沙安牧泉。`docs/mg_inventory_weekly_cases.md:129` 白紙黑字：長沙安牧泉
   `CASE-20260625-001` 的 issue.md **明確聲明 `TesterTCP.cpp` 的 `//AI 20260625` 註解不屬本案**。
   正確歸屬是 **欣銓 Ardentec `CASE-ARDENTEC-20260625-001`（V3.33.899.30）**（同文件 :66）。
   LEDGER 的 CASE 欄要照這個填。

4. **`0623` 是兩個客戶的兩個案子，不是一個 cConfiguration 群。**
   - `main.cpp:5640`（移除 `#ifdef SOFT_SIMULTE`）＝**全智 GIGAS `CASE-GIGAS-20260622-001`**（V3.33.899.29）
   - `cConfiguration.cpp` ×4 ＋ `MachineType.h` ×1 ＝**力成 PTI `CASE-20260617-001`**（V3.33.899.30，N06 Enable FTP 存不進去）
   `mg_inventory_weekly_cases.md:64` 把 `MachineType.h` 掛在 GIGAS 那列，但
   **`ChangeToIntNonPcnt` 在 V899 全樹只有一個呼叫點＝`cConfiguration.cpp:153`（實測）**，
   主題上屬 PTI 那案；inventory 那格應視為誤置。

5. **`0414 MNetLog` 不是「V910 沒有」，是「V899 那版根本抓不到檔案」。**
   V899 的 XCOPY 來源寫死 `D:\HT9045_Log\MNetLog\YYYY_MM`，
   但 MNetLog 由 `TMyStringList`（`SaveType=TByDay` + `bFilePathWithDate=true`）產生，
   實際落盤路徑是 **`D:\HT9045_Log\MNetLog\YYYY\MM\`**（`Public\MyStringList.cpp:491-493`）。
   V910 的 `Steven 20260504/20260505` 改寫版本從**根目錄** `robocopy … /S /MAXAGE:2`，
   並在註解裡明寫「MNetLog uses YYYY\MM subdir (TByDay+bFilePathWithDate)」。
   → **C 類 allowlist**，證據見 §4.1。這回答了 MG-W10 留下的「查證後歸波」問號。

6. **`0525 PTI_LotStartTrace` 相反：路徑形狀對得上，而且 V910 已經有生產者、只缺打包。**
   `V910\uLotInfo.cpp:107` 的 `WritePTILotStartTrace()` 寫 `D:\HT9045_Log\PTI_LotStartTrace\%04d_%02d`，
   與 V899 打包端的 XCOPY 路徑**完全同形**。所以 W15-09 搬過去會真的抓到檔。

7. **⚠ 本分析進行期間有另一個波次正在併行改 V910 樹（20260826 22:39 實測）。**
   V910 樹起手是乾淨的、`*.mgbak` 0 個；分析結束時已變成
   `CosFunction.cpp`／`HS_Function.cpp`／`Motor\mymotor.cpp`／`SECSGEM\uHGemHT9045_SV.cpp`／
   `adam6024.cpp`／`csystem.cpp` **六個 `M` ＋ 六個 `.mgbak`**，且 `mg_ai_matrix_missing.csv`
   已被重跑（40 → 23 條）、`mg_matrix_allowlist.csv` 已被追加到 63 資料列。
   - **與本波零交集**：那六個檔沒有一個在本波的 6 個目標檔內；
     本波 6 檔的 md5 已於 22:39 逐檔複驗，**與 §0.1 表格完全相符**。
   - **本波的 20 條矩陣列在重跑後仍全部是 MISSING**（新 csv 23 條 = 本波 20 條 + `cShowBinSelect.cpp` 3 條）。
   - **兩個操作上的後果**：
     (a) `port_check.py` 不給參數時會**自動抓 V910 樹全部 `M` 檔** → 本波**一定要帶明確檔名**（§8 已如此寫），
         否則會把兄弟波的六個檔一起掃進來。
     (b) 收工清 `*.mgbak` 時**只能點名本波的 6 個檔**，不可用 `find … -name "*.mgbak" -delete` 之類的寬掃，
         那會毀掉兄弟波的基準檔。
     (c) `mg_matrix_allowlist.csv` 正被併行追加 → §7 的兩列請**先重讀再 append**，不要整檔覆寫。

---

## 0. 前置事實（全部實測）

### 0.1 檔案基本盤（**md5 前提**，套用前必須逐檔重驗）

| 檔 | V899 行數 / md5(12) | **V910 行數 / md5(12)** | EOL（位元組實測） | cp950 | 被前波動過？ | 套用後 |
|---|---|---|---|---|---|---:|
| `MachineType.h` | 1647 / `8163240f71fe` | **1671 / `1e88dae72f61`** | V899=CRLF **／ V910＝100% bare-LF** | OK | 否 | **1680**（仍 100% LF） |
| `cConfiguration.cpp` | 7573 / `e69531a98d53` | **7868 / `1f910ae02ff5`** | 兩樹全 CRLF | OK | **是（W8）** | **7875** |
| `main.cpp` | 33575 / `81596aa5852b` | **35408 / `e146c4f3220f`** | 兩樹全 CRLF | OK | **是（W4/W10/W12/W13/W14）** | **35462** |
| `Interface\TesterTCP.h` | 268 / `844ab12404fb` | **267 / `1af66dfb22b4`** | 兩樹全 CRLF | OK | 否 | **268** |
| `Interface\TesterTCP.cpp` | 1203 / `f4e0fbe5154d` | **1117 / `2e3dc1e8c316`** | 兩樹全 CRLF | OK | 否 | **1211** |
| `ainarm9045.cpp` | 8646 / `47563b44e9ee` | **9274 / `8d0509c50a9a`** | 兩樹全 CRLF | OK | **是（W9）** | **9334** |

> 本文件所有 V910 行號都量在**上表 md5 對應的磁碟現狀**上。
> **任務書給的「`ainarm9045.cpp` 4961 行」是錯的**——V910 該檔實測 9274 行、V899 8646 行（`main.cpp 35408` 與 `cConfiguration.cpp 7868` 則正確）。
> 六個檔 cp950 全檔可解，U+FFFD 前後皆 0。

### 0.2 範圍權威與 case 歸屬（修正版）

| 日期 | 條數 | 檔 | 客戶／CASE | V899 版號 | 主題 |
|---|---:|---|---|---|---|
| 20260414 | 1 | `main.cpp` | —（N10 FTP 診斷） | — | StateRecord 打包 MNetLog → **C 類** |
| 20260514 | 2 | `ainarm9045.cpp` | —（Phase 3 scaffold，非 `CASE-20260514-001`） | — | Loader pick planner 接線（休眠） |
| 20260525 | 2 | `main.cpp` | 力成PTI `CASE-20260521-001` / `CASE-20260521-002` | V3.33.899.22 | 換工作檔同步 PASS auto ／ 打包 PTI Lot Start trace |
| 20260623 | 1 | `main.cpp` | **全智 `CASE-GIGAS-20260622-001`** | V3.33.899.29 | C08_1 Socket Sensor 互鎖在正式 build 失效 |
| 20260623 | 5 | `cConfiguration.cpp`(4)＋`MachineType.h`(1) | **力成PTI `CASE-20260617-001`** | V3.33.899.30 | N06 Enable FTP 存不進去（O06 picker 例外） |
| 20260625 | 7 | `Interface\TesterTCP.cpp`(6)＋`.h`(1) | **欣銓 `CASE-ARDENTEC-20260625-001`**（**不是**長沙安牧泉） | V3.33.899.30 | N06 recipe 同步硬化 |
| 20260629 | 1 | `main.cpp` | 力成PTI `CASE-PTI-20260629-001` | V3.33.899.31 | Home/Buzzer/Modal 診斷 dump（純診斷） |
| 20260706 | 1 | `ainarm9045.cpp` | **甬矽 `CASE-FOREHOPE_NINGBO-20260706-001`** | V3.33.899.34 | WAR0152：可達性選嘴 |

`bb69c60..b515ed5` 對這 6 個檔的工作區差異為空 → 以磁碟現狀當 V899 最終態，符合「final-state diff，不重播歷史」。

### 0.3 符號稽核（V910 全樹，排除 `.svn`）

**新增符號在 V910 命中數皆為 0**（不撞名）：

`ChangeToIntNonPcnt` / `dtO06Load` / `LogN06Result` / `Get7zPath` / `RunSevenZipSync` /
`iFirstEnableSuck` / `iBestSuck` / `dBestMargin` / `dLimP` / `dLimN` / `dDefX` / `dTryX` / `dMargin` /
`bShadowFound`

新程式用到的**既有**符號全部存在且對目標檔可見（逐一實測）：

| 符號 | V910 宣告／首見處 | 目標檔看得到嗎 |
|---|---|---|
| `SaveEventLogInfo` | `cMyDB.h:68` ← `MachineDefine.h:96` `#include "cMyDB.h"` | ✓ `TesterTCP.cpp:2` include `MachineDefine.h`；且**兩樹 `MachineDefine.h` 的 `#include` 清單 diff 為空**、兩樹 `TesterTCP.cpp` 前 20 行位元組相同 |
| `ShellExecuteEx` / `SHELLEXECUTEINFO` / `SEE_MASK_NOCLOSEPROCESS` / `WaitForSingleObject` / `GetExitCodeProcess` / `CloseHandle` / `WAIT_OBJECT_0` | Win32（shellapi 由既有 `ShellExecute` 鏈提供）；V910 樹 `ShellExecuteEx` 已有 5＋處（`main.cpp:18175/18214/18258`、`cObserver.cpp:4503`、`CCDInterface.cpp:360`） | ✓ |
| `ExtractFilePath(Application->ExeName)` | VCL；V910 `handlerlog.cpp:43`、`main.cpp:281/285` 已在用同一寫法 | ✓ |
| `bUseLoaderPickPlanner` / `g_LoaderPlan` / `GetActiveCount()` / `IsAuthoritative()` | `Common\PickPlanner\cInArmLoaderPickPlanner.h:30,72,47,52`＋`.cpp:17,20` | ✓ **七個 PickPlanner 檔 md5 兩樹全同**、`.bpr` 已登錄、`bcc_syntax.sh` 的 `INC` 也已含 `Common\PickPlanner` |
| `RecordProcess` | 全樹 1400＋處 | ✓ |
| `CheckOutArmDestory()` | `Motor\mymotor.h:328`（`main.cpp:7159` 已在用） | ✓ `main.cpp:60` include `MyMotor.h` |
| `bOutArmPlaceDevice[4][8]` | `cmydef.h:4680` extern | ✓ `main.cpp:12` include `cmydef.h` |
| `SwMusic1` | `cmydef.h:1747` `extern const int` | ✓ |
| `fNote->AlarmType` / `fNote->edErrorCode` | `note.h:432` | ✓ `main.cpp:15` include `note.h` |
| `MyMessageBox->fShow` | `mymessbox.h` | ✓ `main.cpp:61` |
| `fMain->palMainStatus` | `main.h`（同檔 40 處） | ✓ |
| `TfLotInfo::SetFirstTrayCheckOnUnloader()` | `uLotInfo.h:1422` / `uLotInfo.cpp:16133` | ✓ **函式本體兩樹 byte-identical**（見 §1.4） |
| `WritePTILotStartTrace()`（0525 的生產者） | `uLotInfo.cpp:99`（V910 已有；矩陣 20260525 的另 4 條就是 same-file） | ✓ |
| `CC_PTI`(957) / `CC_GIGAS`(970) | `MachineType.h:326` / `:341` | ✓ 且 `main.cpp:1717-1723` 的客戶碼改寫**只動 `CC_ASE_KaohSiung`**，PTI/GIGAS 原樣通過 → 本波兩個客戶碼閘**不是死碼** |
| `MOT[MInArmX].Motor->PSoftLimitP/N`、`InArmSuck.Suck[][].iMyCol`、`Prod.XInArm_Tray_Pick`、`dInArmXPitch_1Step`、`USE_PICKER_COUNT`、`ep1Picker` | `ainarm9045.cpp` 同函式 5198-5205 已在用 | ✓ |

**`#include` 閉包：只需要 W15-10 那一行（`cInArmLoaderPickPlanner.h`），其餘 11 個 op 都不需要新增 include。**
**`.dfm`：本波 0 個 op。**

### 0.4 未標記承重行（矩陣看不到，漏搬即壞）

以 `difflib`（去除 EOL）比對「模擬結果 vs V910 現狀」：

| 檔 | added | deleted | 其中 `//AI` 標記行 |
|---|---:|---:|---:|
| `MachineType.h` | 9 | 0 | 1 |
| `cConfiguration.cpp` | 15 | 8 | 3 |
| `main.cpp` | 57 | 3 | 4 |
| `Interface\TesterTCP.h` | 3 | 2 | 1 |
| `Interface\TesterTCP.cpp` | 126 | 32 | 6 |
| `ainarm9045.cpp` | 65 | 5 | 3 |
| **合計** | **275** | **50** | **18** |

**矩陣可見率 18/275 = 6.5%**（比 MG-W14 的 11.6% 還低）。四塊最危險的「一條註解帶一整段實作」：

| V899 位置 | 內容 | 矩陣只列 | 漏搬的後果 |
|---|---|---|---|
| `TesterTCP.cpp:1051-1098` | `LogN06Result`／`Get7zPath`／`RunSevenZipSync` 三個 file-static 函式（48 行） | `:1050`、`:1063`、`:1072` 三行 | 兩支 `CopyRecipe*` **compile error（未宣告識別字）** |
| `TesterTCP.cpp:1101-1183` | 兩支函式整個重寫（83 行） | `:1100`、`:1145`、`:1155` 三行 | **修正等於沒搬**（仍是 fire-and-forget ShellExecute＋MySleep） |
| `ainarm9045.cpp:4664-4714` | 可達性選嘴整段（51 行）＋「關閉其餘吸嘴」迴圈改寫 | `:4661` 一行 | 只搬註解＝**零效果**；只搬前半段＝`iCurrSuck` 未定義行為 |
| `main.cpp:25419-25455` | 診斷 dump 本體（37 行） | `:25418` 一行 | 診斷等於沒搬 |
| `cConfiguration.cpp:7328-7337` | `try{` … `}catch(...)`（10 行，`catch` 那行是 `/*AI*/` 不是 `//AI`，矩陣抓不到） | `:7327`、`:7330` | **括號不平衡 → compile error** |

> 本波 12 個 op 全部以**連續區段**抽取，上述行自然被包含。

---

## 1. 七個叢集的變更敘述（搬什麼、為什麼）

### 1.1 `0623` PTI `CASE-20260617-001`：N06 Enable FTP 存不進去 → **W15-01～03（＋C 類 1 條）**

**根因**：`TfConfiguration` 的 `[O] EventLog AutoSave` 區用 `dtpO06NextTime`（一個 **`dtkTime` picker**）。
存檔端 `SaveConfiguration()` 寫 `dtpO06NextTime->Date = dtO06_LastDate->Date;` ——
對 time-only picker 指派 `->Date`，當值落在界外日期（`1899-12-30`，即 `TDateTime==0`）時
VCL 直接丟 `EDateTimeError`。這個例外沒人接，**整支 `SaveConfiguration()` 從該行起全部沒執行**，
所以使用者看到的症狀是「N06 的 Enable FTP 勾了、按確定、回來又沒了」——其實是後面所有設定都沒寫進去。

**最終態三件事**：
1. **讀取端**（`W15-02`）：`dtO06_LastDate`/`dtpO06NextTime` 指派前先做界外/空值防呆
   （`(double)dt < 2.0 ? Now() : dt`），避免載入時就拋例外。
2. **存檔端**（`W15-03`）：移除非法的 `->Date` 指派，改用
   `dtO06_LastDate->Date + dtpO06NextTime->Time` 組合出 `TDateTime`；整段包 `try{}catch(...){}`，
   讓「單一 picker 出事」不再能中斷整個 `SaveConfiguration()`。
3. **工具**（`W15-01`）：`MachineType.h` 補 `ChangeToIntNonPcnt` 整數安全除法模板
   （零防護＋取整），對應 KNOWLEDGE `ht9045-divide-safety-int-to-float-regression`
   ——search-division 技能當年把 `iCount/iCbColCount` 換成浮點版造成 CheckBox 階梯式位移。

> `cConfiguration.cpp:153`（該模板的唯一呼叫點）**V910 已有等價修正 → C 類**，見 §4.2。
> 因此 W15-01 搬進去後在 V910 是**沒有呼叫點的模板**；C++ 模板不實例化＝零產碼、零風險，
> 但這一點必須寫進 LEDGER，否則下一波會以為是漏接。

### 1.2 `0623` 全智 `CASE-GIGAS-20260622-001`：C08_1 Socket Sensor 互鎖在正式 build 消失 → **W15-04**

`TfMain::Start()` 裡的 GIGAS 專屬 Start 前檢查（`TestIF_File.bEnSocketSensor==false` → 擋 Start）
整段被 `#ifdef SOFT_SIMULTE … #endif` 包住。正式 build 不定義 `SOFT_SIMULTE`（見 KNOWLEDGE
`ht9045-soft-simulte-flag`），**所以這個互鎖在實機上根本沒被編進去**——典型的「測試會、實機不會」。
V899 把 `#ifdef/#endif` 兩行拿掉、換成一行 `//AI` 註解說明。

外層雙閘 `CUSTOMER_CODE==CC_GIGAS && IniConfig.bC08_1_CheckSocketSensorDetectON==true` 一行未動
→ **非 GIGAS 客戶行為 0 變化**；GIGAS 且旗標開的機台會恢復這個互鎖（＝本案要的）。

### 1.3 `0625` 欣銓 `CASE-ARDENTEC-20260625-001`：N06 recipe 同步硬化 → **W15-05、06**

**根因**：`CopyRecipeToTester()` / `CopyRecipeFromTester()` 用
`ShellExecute(this,"open","D:\\HT9045\\7z.exe",…)` **fire-and-forget**，回傳型別 `void`，
失敗完全靜默（`if(int(hInstance)<=32) { ; }` ——一個空的 if），
後面只 `MySleep(500)`／`MySleep(2000)` 就當作完成了。

**最終態五件事**（全在 W15-06 一個 op 內）：
1. `LogN06Result()`：去重式記錄器，成敗寫進既有 EventLog（`N06000001` OK / `N06000002` NG），
   同 key 連續只記第一次 → 不膨脹日誌。
2. `Get7zPath()`：先取 `ExtractFilePath(Application->ExeName)+"7z.exe"`，`FileExists` 為 false 才
   fallback 到硬編碼 `D:\HT9045\7z.exe`。
3. `RunSevenZipSync()`：改用 `ShellExecuteEx(SEE_MASK_NOCLOSEPROCESS)` →
   `WaitForSingleObject(h, 10000)` → `GetExitCodeProcess()` → `CloseHandle()`。
   （與 `main.cpp:18175` 起既有的三處 `ShellExecuteEx` 同模式。）
4. 兩支函式改 `bool` 回傳，前置檢查來源 zip／目的網路磁碟／7z 三者存在性，各自記錄失敗原因
   （`SRC_ZIP_MISSING`／`DEST_PATH_MISSING`／`SRC_INI_MISSING`／`7Z_MISSING`／
   `EXEC_FAIL_OR_TIMEOUT`／`7Z_EXIT_<n>`）。
5. `.h` 同步改型別（W15-05）。

**WORKFILE 交握不但沒破壞，而且是被修好的那一半。** 呼叫端 `TfMain::Timer2Timer`：

```
fTesterTCP->CopyRecipeToTester(fTesterTCP->OSRecipe);      // V910:21682
MySleep(100);
Str.sprintf("WORKFILE,%s,", fTesterTCP->OSRecipe);          // V910:21684
fTesterTCP->SendTCPIPCommand(0, "Change Recipe", Str);
```

舊版：7z 非同步解壓 + `MySleep(500)` + `MySleep(100)` ＝ 只給 600 ms，**7z 慢一點就會在檔案還沒解完時
就把 `WORKFILE,<name>,` 送給 Tester**。新版同步等到 7z 結束才回，`WORKFILE` 一定在檔案就緒之後才送
——這正是 OS-Tester TCP 交握（KNOWLEDGE `ht9045-ostester-tcp-workfile-handshake`）要的順序保證。

**殘留風險（已量化，非阻擋）**：`MySleep()` 實作是 `::Sleep()`（`common.cpp:1793-1796`，**不 pump 訊息**），
所以新舊都是硬阻塞 UI thread。舊版固定 500 ms（To）／2000 ms（From）；
新版是「等到 7z 結束，上限 10 s」。小 zip 的一般情況新版**更快**（7z <1 s 就返回）；
病態情況（7z 卡住）從 500 ms 惡化到 10 s。另一方面「網路磁碟不見」這個最常見的失敗
新版反而更快返回（先 `DirectoryExists()` 就 return，舊版要等 7z 自己在網芳上逾時）。
這是 V899 出貨中的行為，照搬。

### 1.4 `0525` PTI `CASE-20260521-001`：換工作檔後同步 PASS auto 位置 → **W15-08**

`TfMain::cbSetupFileNameChange()` 尾端原本只把勾選框打勾：
`fLotInfo->cbFirstTrayCheckOnUnloader->Checked=true;`
V899 改成呼叫 `fLotInfo->SetFirstTrayCheckOnUnloader()`。

**`SetFirstTrayCheckOnUnloader()` 在兩棵樹是 byte-identical**（V899 `uLotInfo.cpp:15171-15180`
vs V910 `uLotInfo.cpp:16133-16142`），內容是舊行為的**嚴格超集**：

```
cbFirstTrayCheckOnUnloader->Checked=true;             // ← 舊行為就是這一行
IniConfig.bP62Auto1=(Prod.iIsFailT6[eAuto1]==0);      // ← 新增：依 Bin 屬性同步三個 PASS auto
IniConfig.bP62Auto2=(Prod.iIsFailT6[eAuto2]==0);
IniConfig.bP62Auto3=(Prod.iIsFailT6[eAuto3]==0);
cb1stCheck_Auto1/2/3->Checked=…;
```

外層雙閘 `CosFunction.bFirstTrayCheckOnUnloader && IniConfig.bP62FirstTrayCheckOnUnloader` 未動。

### 1.5 `0525` PTI `CASE-20260521-002`：StateRecord 打包 PTI Lot Start trace → **W15-09**

同案的 `uLotInfo.cpp` 側（`WritePTILotStartTrace()` ＋ 4 個呼叫點）**V910 已有**
（`uLotInfo.cpp:99-120`、`:154/1751/1757/7486`，且 `:122` 就是 `//AI(ht9045-v899) 20260525` 簽章）。
缺的只有 `main.cpp` 的打包端。

**結構落差要注意**：V910 的 log 打包區被 `Steven 20260504/20260505` 整段改寫過
（全月 `XCOPY`+`7z`+`RMDIR` → 根目錄 `robocopy /S /MAXAGE:2` 近 2 天 ＋ 跨月 `SystemDate<=2` 補抓）。
本 op **不動 robocopy 那幾行**，只在 `TCP_IP_MODE` 分支尾端補上 V899 的 PTI 區塊
（該區塊仍用 `XCOPY`+`7z`+`RMDIR`，與同函式仍在用同一 idiom 的 `Automation Log`（26862-26873）
與 MG-W12 已搬的 `FtpUpload`（26876-26894）風格一致）。
路徑形狀已驗證對得上（見 §⚠6）。

### 1.6 `0629` PTI `CASE-PTI-20260629-001`：Home/Buzzer/Modal 診斷 dump → **W15-07**

Handler 自動 Initial 回 HOME 偶發 hang，需要定位「早退點」與「modal 來源」。
在 `DumpMainFormSnapshot()`（V910 `main.cpp:26539`；20260604 那支診斷函式，矩陣該日三條皆 same-file）
既有的 `---- Loader / TrayArm Handoff Diag ----` 之後、`slSnap->SaveToFile()` 之前，
補一段 `---- Home / Buzzer / Modal Diag ----`：

`CheckOutArmDestory()` ／ `fAllMotorHome`/`iHome`/`InitialOK` ／
`SoftStart`/`SystemStart`/`bAlarmBuzzer`/`RunState` ／
`bFinshTest`/`bI01TesterFinishThenHome`/`bHomeinitialCheckPushZ1`/`bContactModeNeedOpenDoor` ／
`LastSet.iTester` ／ `fNote`（fShow/AlarmType/edErrorCode）／ `MyMessageBox->fShow` ／
`fMain->palMainStatus->Caption` ／ 七軸 `HomeFlag` ／ 四顆 `SwMusic` 狀態 ／
`bOutArmPlaceDevice[4][8]`。

**純寫 `TStringList`，零控制流程副作用**；整段落在既有的 `try{ … } __finally{ delete slSnap; } catch(...){}`
內（V910 26649-26658 是 `__finally`／`catch`），所以連例外都被吃掉。
`fNote`／`MyMessageBox`／`fMain` 三個指標都有 `if(ptr)` 守衛（V899 原樣）。

### 1.7 `0514` Phase 3 Loader pick planner 接線（休眠）→ **W15-10、11**

`Common\PickPlanner\` 七個檔 **早在 V910 基線**（MG-W11 已查證：md5 全同、`.bpr` 已登錄），
MG-W11 也已把 `AutoClean.cpp` 那側的 6 條 20260514 接活。
本波補的是 InArm 這側的兩行接線：

- `W15-10`：`ainarm9045.cpp` 補 `#include "cInArmLoaderPickPlanner.h"`
  （V910 的 include 清單比 V899 多兩行 `MyCCLinkSensor.h`／`LtcSensor.h`，所以是**接在清單尾**而不是 V899 的第 40 行位置）。
- `W15-11`：`SearchAndMoveInArmXYToLoad_9045()` 開頭補 shadow hook。

**三重休眠保證**（全部實測）：
1. `bUseLoaderPickPlanner = false`（`cInArmLoaderPickPlanner.cpp:17`），**V910 全樹沒有任何寫入點、沒有 ini／UI 綁定**。
2. 即使手動改成 true，hook 內只有 `Init()`／`Search()`／`RecordProcess(…)`，**沒有 early return**
   （V899 註解自己寫死 "No early return; fall through to legacy path below."）。
3. `IsAuthoritative()` 是 `{ return false; }` 硬編碼（`cInArmLoaderPickPlanner.h:52`），
   `Apply()` 是 no-op。

> `mg_inventory_weekly_cases.md:183` 曾標注 PickPlanner「是死骨架，搬移前先確認 V910 是否需要」。
> MG-W11 已作出「補活」的裁決並落地 AutoClean 側；本波與該裁決一致。
> 若主腦想改判「不搬死骨架」，這 2 條就必須進 `MG_FINAL_DECISIONS.md`（**不能走 allowlist——
> V910 確實沒有等價物**）。以「風險=0、可清 2 條矩陣、與 W11 一致」三點衡量，建議照搬。

### 1.8 `0706` 甬矽 `CASE-FOREHOPE_NINGBO-20260706-001`：WAR0152 可達性選嘴 → **W15-12（D 類）**

**根因**（對應 KNOWLEDGE `ht9045-war0152-pitch-fallback-wrong-direction`）：
`GetInArmToLoaderPosition_Single()` 舊版**寫死取「最左邊啟用的吸嘴」**：

```
for(int i=0; i<4; i++)
    if(InArmSuckUse[iSelRow][i]==true) { iCurrSuck=i; for(j=i+1..3) 關掉其餘; break; }
```

盤尾殘料單吸時，最左嘴算出的 X 可能超出 `MInArmX` 軟極限 → 落進下游的「縮 pitch 救援」，
而該救援對小 IC 會**往張大的方向調**、反而更超限 → `WAR0152`。

**新版**：先算最左嘴在**正常 pitch** 下的 X（`dDefX`，公式與後面真正算 `iXPos` 的那行完全相同）；
只有當 `dDefX` 落在 `[PSoftLimitN+100, PSoftLimitP-100]` **之外**時，才在所有啟用吸嘴中
改選「X 落在極限內、且離兩端餘裕（`min(dLimP-X, X-dLimN)`）最大」者。
**pitch 維持不縮**（避免縮 pitch 讓閒置鄰嘴壓到隔壁 IC 掉料）；
**不更動 Loader tray 的 IC 狀態**；沒有任何嘴可達就保留最左嘴，交給下游既有預檢。

**預設路徑等價性（已逐條驗證）**：
- `iCurrSuck` 初值 = `iFirstEnableSuck` → 一般情況與舊版選同一支。
- 「關閉其餘吸嘴」迴圈從 `j>i` 改成 `j != iCurrSuck`：因為 `iFirstEnableSuck` 是**第一支**啟用的，
  所有 `j < i` 本來就已經是 `false`，所以在 `iCurrSuck==iFirstEnableSuck` 時**兩者完全等價**；
  只有救援命中（`iCurrSuck != iFirstEnableSuck`）時新版才會多關掉最左那支——正是需要的。
- 全部吸嘴都沒啟用時：舊版 `iCurrSuck=-1` → 早退；新版 `iFirstEnableSuck=-1` → `iCurrSuck=-1`，
  迴圈有 `iCurrSuck>=0` 守衛什麼都不做 → 一樣早退。**等價。**
- 救援只在「舊版本來就會超限」的情境觸發，`USE_PICKER_COUNT!=ep1Picker` 再加一道閘。

---

## 2. 逐 op 對照表

| op | V910 檔 | mode | V910 目標 | V899 來源 | 錨字串 | 全檔唯一？ | 清掉的矩陣列 |
|---|---|---|---|---|---|---|---|
| W15-01 | `MachineType.h` | before | 1664 | 1631-1639 (9) | `template<typename T>` | ✓ 1 | `MachineType.h:1631` |
| W15-02 | `cConfiguration.cpp` | replace | 5202-5203 | 4983-4986 (4) | `dtO06_LastDate->Date        =IniConfig.…` | ✓ 1 | `cConfiguration.cpp:4983` |
| W15-03 | `cConfiguration.cpp` | replace | 7594-7599 | 7327-7337 (11) | `dtpO06NextTime->Date … =dtO06_LastDate->Date;` | ✓ 1 | `:7327`、`:7330` |
| W15-04 | `main.cpp` | replace | 5910-5914 | 5640-5643 (4) | `ShowMyMessage("SocketSensor Detect Function is OFF…")` | ✓ 1 | `main.cpp:5640` |
| W15-05 | `Interface\TesterTCP.h` | replace | 258-259 | 258-260 (3) | `    void __fastcall CopyRecipeToTester(AnsiString FileName);` | ✓ 1 | `TesterTCP.h:258` |
| W15-06 | `Interface\TesterTCP.cpp` | replace | 1058-1097 | 1050-1183 (134) | `void __fastcall TfTesterTCP::CopyRecipeToTester(AnsiString FileName)` | ✓ 1 | `:1050 :1063 :1072 :1100 :1145 :1155` |
| W15-07 | `main.cpp` | before | 26648 | 25417-25456 (40) | `slSnap->SaveToFile(sSnapPath);` | ✓ 1 | `main.cpp:25418` |
| W15-08 | `main.cpp` | replace | 25209 | 24003-24004 (2) | `fLotInfo->cbFirstTrayCheckOnUnloader->Checked=true;` | ✓ 1 | `main.cpp:24003` |
| **W15-09** | `main.cpp` | replace | 26822 | 25603-25617 (15) | `    }` | **✗ 1385**（見下） | `main.cpp:25604` |
| W15-10 | `ainarm9045.cpp` | after | 41 | 40-41 (2) | `#include "LtcSensor.h"` | ✓ 1 | `ainarm9045.cpp:40` |
| W15-11 | `ainarm9045.cpp` | before | 6966 | 6436-6452 (17) | `if(MOT[MMTrayY].fHasTray==false \|\|` | ✓ 1 | `ainarm9045.cpp:6436` |
| W15-12 | `ainarm9045.cpp` | replace | 5180-5192 | 4661-4714 (54) | `            for(int j=(i+1); j<4; j++)` | ✓ 1 | `ainarm9045.cpp:4661` |

> 同檔多 op 由 `splice.py` **由下往上**套用，行號互不位移。排序 key 實測：
> `main.cpp` = 26822 → 26648 → 25209 → 5910；`ainarm9045.cpp` = 6966 → 5180 → 41；
> `cConfiguration.cpp` = 7594 → 5202。**無交疊。**

### 2.1 W15-09 的非唯一錨——套用前必做的目視確認

`anchor_expect` 只能是單行子字串，而本 op 忠實地把 PTI 區塊放在 `TCP_IP_MODE` 分支**尾端**，
該處錨行就是 `    }`。套用前請先確認 V910 `main.cpp` 這四行**逐字**如下（md5 `e146c4f3220f` 下實測）：

```
26821|        }
26822|    }          ← replace 目標（唯一一行）
26823|    else
26824|    {
```

若主腦偏好唯一錨勝過忠實位置，替代寫法（**語意等價**，只是把區塊放到分支開頭；
TestList 內各組 XCOPY/7z/RMDIR 針對不同目錄、彼此獨立，順序不影響結果）：

```json
{ "op":"W15-09b", "v910_file":"main.cpp", "v899_lines":[25603,25616],
  "mode":"before", "anchor_line":26812,
  "anchor_expect":"MyForceDirectories(NewPath+\"\\\\TCP_LOG\");" }
```

---

## 3. 客戶影響面

| op | 閘 | 影響客戶 | 非目標客戶行為 |
|---|---|---|---|
| W15-01 | 無（模板，V910 無呼叫點） | 無 | 完全不變（模板不實例化） |
| W15-02／03 | `IniConfig.bEventLogAutoSaveFunction`（config 旗標，非客戶碼） | 所有開 O06 EventLog AutoSave 的機台 | 未開 O06 者：完全不變。開 O06 者：**修的是 crash-path**，正常值下 `Date+Time` 與原 `DateTime` 等值 |
| W15-04 | `CUSTOMER_CODE==CC_GIGAS && IniConfig.bC08_1_CheckSocketSensorDetectON` | 全智（970） | 一行未動，完全不變 |
| W15-05／06 | `IniConfig.bN06_CopyTesterFile` | 開 N06 OS-Tester recipe 同步者（欣銓／田揚志 OS 線） | 旗標關 → 第一行就 `return false`，**與舊版 `if(…==true){}` 完全等價** |
| W15-07 | 無 | 全客戶（僅 StateRecord 內容變多） | 只是 snapshot 檔多幾十行文字 |
| W15-08 | `CosFunction.bFirstTrayCheckOnUnloader && IniConfig.bP62FirstTrayCheckOnUnloader` | 力成 PTI（該 CosFunction 旗標的開啟者） | 雙閘未動，完全不變 |
| W15-09 | `TestIF_File.iTestType==TCP_IP_MODE && CUSTOMER_CODE==CC_PTI` | 力成 PTI（957） | 完全不變（多一個 `if` 判斷） |
| W15-10／11 | `bUseLoaderPickPlanner`（硬編碼 false，無寫入點） | 無 | 完全不變 |
| **W15-12** | **無客戶碼閘** | **全客戶**（但只在最左嘴超軟極限時才改變路徑） | 未超限＝**逐條驗證等價**（§1.8）；超限＝原本就走錯誤的縮 pitch 救援 |

> W15-12 是本波唯一沒有客戶碼閘的行為變更。V899 出貨版就是無閘（甬矽案的修法本來就是通用修正），
> 照搬。若主腦要收窄，可加 `CUSTOMER_CODE==CC_FOREHOPE_NINGBO` 閘——但那是**改寫不是搬移**，
> 依政策要進 `MG_FINAL_DECISIONS.md`。建議照搬（等價性已逐條證明）。

---

## 4. C 類（allowlist）判定與證據

### 4.1 `main.cpp:25658`（20260414 MNetLog）→ **C 類**

**矩陣列**：`//AI(ht9045-v899) 20260414: pack MNetLog into StateRecord when N10-3 daily upload enabled for FTP diagnostic`

**三條獨立證據**：

1. **V910 已有等價實作，而且是刻意改寫的新版**
   - `main.cpp:26732-26741`（`Steven 20260505`）：同步複製**當前作用中**的 MNetLog 檔
     （`slMNetLog->GetFileName()` → `CopyFile` 到 `NewPath\MNetLog\`）。
   - `main.cpp:26848-26857`（`Steven 20260504/20260505`）：
     `robocopy "D:\HT9045_Log\MNetLog" "<NewPath>\MNetLog" /S /MAXAGE:2 …`，**無條件執行**
     （V899 那版還要 `IniConfig.bN10_DailyUploadProdData` 才做）。
2. **V899 那版的來源路徑形狀是錯的，實際抓不到任何檔案**
   - V899 寫 `sprintf("%s\\%04d_%02d", "D:\\HT9045_Log\\MNetLog", SystemYear, SystemMonth)`
     → `D:\HT9045_Log\MNetLog\2026_08`。
   - 但 MNetLog 是 `TMyStringList("D:\\HT9045_Log\\MNetLog", "MNetLog", "")`（`main.cpp:1533`），
     建構子設 `SaveType=TByDay`、`bFilePathWithDate=true`（`Public\MyStringList.cpp:45,49`），
     `GetFileName()` 因此走 `sPathName.sprintf("%s\\%04d\\%02d", HTPath, SystemYear, SystemMonth)`
     （`Public\MyStringList.cpp:491-493`）→ 實際路徑是 **`D:\HT9045_Log\MNetLog\2026\08\`**。
   - `2026_08` 這個夾**不存在** → XCOPY 抓 0 檔、7z 壓空、RMDIR 刪不存在的夾。
3. **V910 的作者知道這件事並寫在註解裡**：
   `main.cpp:26849-26851` —「robocopy from root dir; /S /MAXAGE:2 handles all subfolder structures /
   Galil uses YYYYMM subdir; **MNetLog uses YYYY\MM subdir (TByDay+bFilePathWithDate)** /
   month-boundary handled by /MAXAGE:2 naturally」。

**結論**：搬過去只會在 `1.bat` 加三行無效指令，並且與 `Steven 20260504` 的「全月→近 2 天」
刻意瘦身決策相衝突。**入 allowlist，不搬。**
（這同時把 MG-W10 留在 LEDGER 的「V910 無 0414 MNetLog 區塊，查證後歸波」問號結掉——
V910 有，只是簽章不同。）

### 4.2 `cConfiguration.cpp:153`（20260623 整數安全除法呼叫點）→ **C 類**

**矩陣列**：`//AI(ht9045-v899) 20260623: 改用整數安全除法還原離散欄號,修正浮點除法造成 CheckBox 階梯式位移`

V899（`cConfiguration.cpp:153`）：
```cpp
cbTempSelsct[i]->Left=iCbLeft+iCbColPitch*ChangeToIntNonPcnt(iCount, iCbColCount);
```
V910（`cConfiguration.cpp:158`，`//JerryYang 20260512 : add`）：
```cpp
cbTempSelsct[i]->Left=iCbLeft+iCbColPitch*int((ChangeToFloatNonPcnt((double)(iCount), (double)(iCbColCount))));
```

**等價性證明**：
- 兩者都有 `Denominator!=0` 零防護，除數為 0 時同樣得 0。
- V899：`(int)((double)a/(double)b)`；V910：`int( (float)((double)a/(double)b) )`。
  差別只在中間多一次 `double→float` 窄化。
- `iCbColCount==36` 是常數。當真商是整數 `k` 時，`double` 除法是正確捨入 → 結果**恰為** `k`，
  轉 `float` 仍是 `k`。當真商非整數時，它與最近整數至少差 `1/36 ≈ 0.0278`，
  遠大於 `float` 在該量級的相對誤差（`iCount` 上限是 `tcTotalCount`，商是小整數量級，
  `float` eps ≈ 1.2e-7）→ **窄化不可能跨過整數邊界**。
- ⇒ 兩式對本呼叫點的所有可能輸入**輸出完全相同**。

V910 的版本帶著 `JerryYang 20260512` 的署名，是公司自己對同一個 bug 的獨立修正（同方向，非反向）。
依政策「某條 V910 已有等價 → C 類列 allowlist」。**入 allowlist，不搬。**

> 但 `MachineType.h:1631` 的 `ChangeToIntNonPcnt` 模板 **V910 全樹 0 命中**，沒有等價符號，
> 所以那一條仍照搬（W15-01）。搬完後它在 V910 是零呼叫點的模板——**這是刻意的，已記錄**。

---

## 5. 模擬驗收（唯讀，結果只在 scratchpad）

模擬腳本：`…\scratchpad\w15_sim.py`（套 op）、`w15_verify.py`（difflib 統計）。
輸出：`…\scratchpad\w15_sim\*`。**兩棵樹一個位元組都沒被修改。**

```
=== MachineType.h        (pre 1671, LF)   → post 1680  (+9)   CRLF=0/1680  ✔ 仍純 LF
=== cConfiguration.cpp   (pre 7868, CRLF) → post 7875  (+7)   CRLF=7875/7875
=== main.cpp             (pre 35408,CRLF) → post 35462 (+54)  CRLF=35462/35462
=== Interface\TesterTCP.h(pre 267,  CRLF) → post 268   (+1)   CRLF=268/268
=== Interface\TesterTCP.cpp(pre 1117,CRLF)→ post 1211  (+94)  CRLF=1211/1211
=== ainarm9045.cpp       (pre 9274, CRLF) → post 9334  (+60)  CRLF=9334/9334
### 12 op 全部 OK，FAIL=0；payload 295 行、刪除 70 行
```

| 檢查 | 結果 |
|---|---|
| 12 個 op 錨字串比對 | **12/12 PASS**（0 FAIL） |
| U+FFFD（cp950 解碼失敗）前→後 | 六檔全部 **0 → 0** |
| EOL 混用 | 無。`MachineType.h` 維持 100% LF，其餘五檔維持 100% CRLF |
| 大括號平衡（**cp950 解碼後**計數）delta | 六檔全部 **0** |
| 小括號平衡 delta | 六檔全部 **0** |

> ⚠ **陷阱記錄**：若直接對 Big5 位元組數 `{`/`}`，`TesterTCP.cpp` 與 `ainarm9045.cpp` 會假報 `+1`。
> 原因是 Big5 的 trail byte 合法範圍含 `0x7B`/`0x7D`，中文字會被誤計成大括號。
> **平衡檢查一定要先 `decode("cp950")` 再數。**

拼接處已逐一目視（節錄）：
`MachineType.h:1663-1672`（模板落在 `ChangeToFloatNonPcnt` 與 `vec_clr` 之間，前後分隔線各一）／
`cConfiguration.cpp:5202-5205`、`7596-7606`／`main.cpp:5910-5914`、`25208-25210`、`26648-26686`、`26863-26876`／
`TesterTCP.h:258-260`／`TesterTCP.cpp:1058-1191`／`ainarm9045.cpp:42-43`、`5182-5235`、`7009-7024`。
全部縮排、分隔線、空行與 V899 一致。

---

## 6. `<95%` 清單：**空**

12 個 op 全數 ≥95%，理由如下（最低的三個列出來）：

| op | 信心 | 為什麼還是 ≥95% |
|---|---|---|
| W15-09 | ~96% | 唯一的非唯一錨。但（a）`replace_lines` 只有 1 行、（b）md5 `e146c4f3220f` 是硬前提、（c）前後 4 行內容已逐字列在 §2.1、（d）模擬結果已目視。另備有唯一錨的等價替代方案 W15-09b。 |
| W15-12 | ~96% | D 類、無客戶碼閘。但預設路徑等價性已逐條證明（§1.8），救援只在「舊版本來就超限」時觸發，且錨點全檔唯一、區段收斂在單一函式內。 |
| W15-06 | ~96% | 行為變更（非同步→同步，最長阻塞 500 ms→10 s）。但這是 V899 出貨行為、修的正是 WORKFILE 交握的時序缺口，且常見失敗路徑反而更快返回。 |

其餘 9 個 op（W15-01～05、07、08、10、11）皆為 ≥98%：錨點全檔唯一、符號稽核全綠、
閘門未動或天然休眠、模擬無 FAIL。

**沒有任何一條需要進 `MG_FINAL_DECISIONS.md`。**

---

## 7. LEDGER 更新建議（MG-W15 收工時填）

日期列（`docs/MG_PORT_LEDGER.md` 表格）：

| 日期 | MISSING | 建議寫法 |
|---|---:|---|
| 20260414 | 1 | `A/C` → **`10/10 done`**：main.cpp:25658 = **skipped-C MG-W15**（V910 `Steven 20260504/05` robocopy 版已等價，且 V899 版路徑形狀錯誤抓不到檔）→ 入 allowlist |
| 20260514 | 2 | `A` → **`8/8 done`**：ainarm9045 2 條 = MG-W15（PickPlanner 接線，休眠搬運，與 MG-W11 補活裁決一致） |
| 20260525 | 2 | `A` → **done MG-W15**；CASE 欄補 `CASE-20260521-001 / CASE-20260521-002`（力成PTI） |
| 20260623 | 6 | `A/C` → **done MG-W15（5 搬＋1 allowlist）**；CASE 欄要**拆兩案**：`CASE-GIGAS-20260622-001`（全智，main.cpp:5640）＋`CASE-20260617-001`（力成PTI，cConfiguration ×4＋MachineType.h）。cConfiguration.cpp:153 = skipped-C（V910 `JerryYang 20260512` 等價） |
| 20260625 | 7 | `A` → **done MG-W15**；CASE 欄填 **`CASE-ARDENTEC-20260625-001`（欣銓）**——**不是**長沙安牧泉的 `CASE-20260625-001`（該案 issue.md 已聲明不含這些註解） |
| 20260629 | 1 | `A` → **done MG-W15**（`CASE-PTI-20260629-001`，純診斷） |
| 20260706 | 1 | `A` → **done MG-W15**（`CASE-FOREHOPE_NINGBO-20260706-001`，D 類，全客戶無閘） |

allowlist 追加 2 列（`docs/mg_matrix_allowlist.csv`，欄位＝`file,date,comment_prefix,reason,wave`，
UTF-8-sig，**reason 內不可含半形逗號**；比對規則是「同檔同日 **且** comment 以 prefix 開頭」）：

```
main.cpp,20260414,//AI(ht9045-v899) 20260414: pack MNetLog into StateRecord,MG-W15判定C：V910 Steven 20260504/05 robocopy /S /MAXAGE:2 已等價且無條件執行；V899版來源路徑 YYYY_MM 與 TMyStringList 實際落盤 YYYY\MM 不符本就抓不到檔；詳 mg_w15_analysis.md §4.1,MG-W15
cConfiguration.cpp,20260623,//AI(ht9045-v899) 20260623: 改用整數安全除法還原離散欄號,MG-W15判定C：V910 JerryYang 20260512 的 int(ChangeToFloatNonPcnt(...)) 與 ChangeToIntNonPcnt 對本呼叫點所有輸入輸出相同；詳 mg_w15_analysis.md §4.2,MG-W15
```

> 兩個 prefix 都已驗證能唯一命中目標列：`main.cpp`+`20260414` 全表只有 1 列；
> `cConfiguration.cpp`+`20260623` 有 4 列，另三列分別以「界外/空日期」「[O]EventLog存檔包try/catch」
> 「dtpO06NextTime為dtkTime picker」開頭，不會被這個 prefix 誤命中。

收工後 `python tools/port_tools/ai_comment_matrix.py` 應為
**MISSING 40 → 20**（same-file +18、ALLOWLISTED 58 → 60）。

**LEDGER 必記的兩個「刻意狀態」**（否則下一波會誤判成漏接）：
1. `ChangeToIntNonPcnt` 搬進 V910 後**沒有呼叫點**（唯一呼叫點判 C 類）——模板不實例化，零產碼。
2. `bUseLoaderPickPlanner` 在 V910 仍是**無寫入點的硬編碼 false**——W15-10/11 是休眠接線。

---

## 8. Gate 執行順序（照原樣，不放寬）

```sh
# 1) splice（會自動為 6 個檔建 .mgbak）
python tools/port_tools/splice.py docs/mg_w15_ops.json

# 2) port_check（.mgbak 已存在，不會 fallback 到 LF-normalized 的 git blob）
python tools/port_tools/port_check.py MachineType.h cConfiguration.cpp main.cpp \
       "Interface\TesterTCP.h" "Interface\TesterTCP.cpp" ainarm9045.cpp

# 3) bcc32 -c -H- 隔離語法編譯（驗收＝錯誤集合與 .mgbak 基準相同，不是零錯誤）
sh tools/port_tools/bcc_syntax.sh MachineType.h cConfiguration.cpp main.cpp \
       "Interface\TesterTCP.h" "Interface\TesterTCP.cpp" ainarm9045.cpp

# 4) 主腦逐條開檔複驗（尤其 W15-09 的非唯一錨、W15-12 的 D 類等價性）
# 5) commit → 清 *.mgbak（**只點名本波 6 檔**，勿寬掃，見 §⚠7）→ LEDGER / DEVLOG / RESUME
rm HT9011UC_Code_V3.33.910.0_20260716_Jimmy/MachineType.h.mgbak \
   HT9011UC_Code_V3.33.910.0_20260716_Jimmy/cConfiguration.cpp.mgbak \
   HT9011UC_Code_V3.33.910.0_20260716_Jimmy/main.cpp.mgbak \
   HT9011UC_Code_V3.33.910.0_20260716_Jimmy/Interface/TesterTCP.h.mgbak \
   HT9011UC_Code_V3.33.910.0_20260716_Jimmy/Interface/TesterTCP.cpp.mgbak \
   HT9011UC_Code_V3.33.910.0_20260716_Jimmy/ainarm9045.cpp.mgbak
```

**gate 注意事項**

- `bcc_syntax.sh` 的 `INC` **已含 `Common\PickPlanner`**（實測），W15-10 新增的 include 解得到。
- `main.cpp` 在此 gate 旗標組下**有既有錯誤**（`Timer7Timer` HWND ×2、`edSetupFileNameKeyUp` E2096），
  依 gate 說明屬 `PASS(no-regression)`，比對的是**正規化錯誤集合**不是零錯誤。
- `MachineType.h` 是純標頭，`bcc_syntax.sh` 對 `.h` 會加 `-P` 強制 C++；它同時被幾乎全樹 include，
  真正的把關在 `cConfiguration.cpp` / `main.cpp` / `ainarm9045.cpp` 三支 `.cpp` 過不過。
- **不要在 splice 之前跑 port_check**（§⚠2）。
- **絕不對 V910 路徑 `git checkout`**——本波實測 5 個檔的 git blob 是 LF、磁碟是 CRLF，
  checkout 會整檔改寫行尾。
- **commit 只 `git add` 本波 6 個檔的明確路徑，不用寬 glob**（兄弟波正在改另外 6 個檔，§⚠7），
  而且 **`git commit` 不加尾隨 pathspec**（加了會整檔吃 working-tree 內容，蓋掉 staged 結果）。
- 需要復原時**只能用 `.mgbak` + md5 對帳**，不可 `git checkout`。
