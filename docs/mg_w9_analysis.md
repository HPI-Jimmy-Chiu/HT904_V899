# MG-W9 分析：20260602 / 20260803 InArm load-stage watchdog 線

- **波次**：MG-W9（V899 → V910），LEDGER **相依鏈 #2**
- **來源**：`docs/mg_ai_matrix_missing.csv` **18 條 MISSING**
  （0602×13：`ainarm9045.cpp`10＋`acatchtray.cpp`3；0803×5：`CosFunction.cpp`2＋`ainarm9045.cpp`2＋`CosFunction.h`1）
- **V899 樹**：`D:\HT9045\HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422`（唯讀）
- **V910 樹**：`D:\HT9045\HT9011UC_Code_V3.33.910.0_20260716_Jimmy`
- **本文件性質**：分析＋splice op 草案。**全程唯讀；兩棵樹一行原始碼都沒有被修改**
  （模擬結果只寫進 scratchpad）。
- **op 檔**：`D:\HT9045\docs\mg_w9_ops.json`（**11 個 op，4 個檔**）
- **分類結果**：**18 條全部是 P 類（真缺口）**。C 類 0、X 類 0、X-none 類 0。
  這是 MG 戰役目前唯一一個「矩陣說缺、實測就是真的全缺」的波次。

---

## ⚠ 三件開工前必須先知道的事

1. **本波的基準在分析途中被 MG-W8 移動過。**
   開工當下量到的 `CosFunction.cpp` = 4493 行 / `CosFunction.h` = 496 行，是 **MG-W6 後、MG-W8 前**
   的狀態；分析進行到一半 MG-W8 落地並 commit（`e5589ee`），兩檔變成 **4506 / 502**。
   **本文件與 `mg_w9_ops.json` 的所有行號都已重新量在 W8 之後的樹上**（見 §0.1 md5 表）。
   `ainarm9045.cpp` 與 `acatchtray.cpp` 完全沒被 W6/W7/W8 動過。

2. **案件歸屬與交派任務書上的敘述不同（以 LEDGER 盤點為準）。**
   `docs/mg_inventory_weekly_cases.md` 第 19 列：**20260602 是「欣銓」CASE-20260602-001**
   （Dummy run 手動取盤／空盤殘留 hang）；第 34 列：**20260803 是「全智」CASE-GIGAS-20260729-001**。
   兩者的關係是：**欣銓要的功能，害全智誤報**，所以 0803 把它收回 `CC_ARDENTEC`（＝欣銓，`870`）專用。
   交派任務書寫「0602 全智案」是把兩案的客戶對調了。

3. **0803 的 5 條裡沒有「獨立的 WAR09101 誤報修正」。**
   五條註解逐條讀完＝**同一件事的五個落點**（宣告／預設 false／ARDENTEC 開 true／兩個使用點加閘）。
   「WAR09101 誤報」就是收閘門要解的症狀本身，不是另一個修正。故本文件不分段列。

---

## 0. 前置事實（全部實測，非推論）

### 0.1 檔案基本盤（收工前後 md5 相同＝證明唯讀）

| 檔 | V899 行數 / md5(12) | V910 行數 / md5(12) | EOL | cp950 | 被前波動過？ |
|---|---|---|---|---|---|
| `ainarm9045.cpp` | 8646 / `47563b44e9ee` | **9222** / `50c6e28e745e` | 兩樹全 CRLF | OK / OK | 否（pristine） |
| `acatchtray.cpp` | 7798 / `d80798e8d34c` | **9320** / `33bb3708306a` | 兩樹全 CRLF | OK / OK | 否（pristine） |
| `CosFunction.cpp` | 4430 / `a77b6cd3dc2e` | **4506** / `66bcd93cdedc` | 兩樹全 CRLF | OK / OK | **W6(+5)＋W8(+13)** |
| `CosFunction.h` | 487 / `bba5b164ad3d` | **502** / `374076924655` | 兩樹全 CRLF | OK / OK | **W6(+1)＋W8(+6)** |

> V910 樹目前 `*.mgbak` 已依 skill 新規則清空（0 個），故 `port_check.py` 會拿 `git HEAD` 當基準。

### 0.2 閘門形狀確認（兩棵樹都齊備，**不需要一併搬任何前置物**）

| 項目 | V899 | V910 | 結論 |
|---|---|---|---|
| `#define CC_ARDENTEC 870` | `MachineType.h:268` | **`MachineType.h:231`（存在，值同為 870）** | ✓ 註解都是「欣詮」 |
| `void FUNC_CC_ARDENTEC()` | `CosFunction.cpp:2784` | **`CosFunction.cpp:2823`（存在，但函式體是空的）** | ✓ 有掛點 |
| `case CC_ARDENTEC: FUNC_CC_ARDENTEC();` | `CosFunction.cpp:3779` | **`CosFunction.cpp:3833`（在 `DoCustomerFunction()` 內，該函式起於 3744）** | ✓ 真的會被呼叫 |
| 呼叫順序 | 預設值 → `DoCustomerFunction()` | **同（V910:4482 預設在前、4492 `DoCustomerFunction()` 在後）** | ✓ 客戶碼覆蓋預設 |
| `bUseInArmLoadStageWatchdog` | `CosFunction.h:470` | **全樹 0 命中** | 本波新增（OP9） |
| `HT9045_COUSTOMER_FUNCTION` 是否被二進位序列化 | — | **否**（全樹 0 個 `sizeof(...)`／`fwrite(&CosFunction`／`memcpy(&CosFunction`） | ✓ 插入成員不會像 `tech.dat` 那樣位移毀資料 |

**結論：`CUSTOMER_CODE != CC_ARDENTEC(870)` 的機台，`bUseInArmLoadStageWatchdog` 恆為 `false`，
watchdog 兩個使用點都直接走「解除 latch、什麼都不做」的分支，V910 行為 100% 不變。**

### 0.3 `WAR09101` 不是新警報碼——兩棵樹本來就有

| | V899 | V910 |
|---|---|---|
| 既有使用點 | `ainarm9045.cpp:6794` `ShowErrorMessage("WAR09101", K_TRAY_END, MInArmX);` | **`ainarm9045.cpp:7359` 同一行、同 3 個引數** |

watchdog 只是**重用**這支既有警報碼、換成 `K_RETRY` 鍵。
→ **不需要動 `Error\<語系>\*.dat`、不需要註冊新 code、3 引數多載在 V910 已有先例。**

### 0.4 符號可解析性（＝編不編得過的真正問題）

| 符號 | V910 宣告處 | 目標檔看得到嗎 |
|---|---|---|
| `CosFunction` | `CosFunction.h` | ✓ `ainarm9045.cpp` 已有 6+ 處 `CosFunction.xxx` |
| `TQPF_Timer` / `SetSecAndOn(double)` / `Off()` | `myTimer.h:7,35` | ✓ 同檔已有檔案層 `TQPF_Timer DoArmPickFromLoadStage_Delay;` |
| `INSTALL_OCR_YMot` | `cmydef.h:2979` | ✓ `acatchtray.cpp` include 了 `cmydef.h` |
| `eocrYMotInstal` | `MachineType.h:509` | ✓ include 了 `MachineType.h` |
| `MLoaderY` | `cmydef.h:2172` | ✓（`acatchtray.cpp` 已出現 2 次 `MLoaderY`） |
| `Prod.iMLoaderYSurePos` | `cprod.h:1114` | ✓ include 了 `cprod.h`（同檔已有 121 處 `Prod.`） |
| `C_LoaderUpPress` / `C_TrayY_Fixer` / `C_LoaderEdgePush` | `cmydef.h:388/327/343` | ✓ 同檔已大量使用 |

**`acatchtray.cpp` 的 `#include` 區：V910 是 V899 的嚴格超集**（V899 30 個，V910 同樣 30 個
＋`Magazine.h`、`common.h`、`database.h`）。→ **本波不需要新增任何 `#include`。**

### 0.5 名稱不會撞

`DoInArmLoadStageNoTray_Watchdog` / `DoInArmLoadStageEmptyTray_Watchdog` /
`dInArmLoadStageNoTrayTimeoutSec` / `dInArmLoadStageEmptyTrayTimeoutSec` /
`bLoadStageNoTrayArmed` / `bLoadStageEmptyTrayArmed`
→ **V910 全樹（`*.cpp`/`*.h`）各 0 命中。**
`case 148:` 在 V910 `acatchtray.cpp` 全檔 **0 命中**（該 switch 只有 145 / 147 / 150），
→ 不會產生重複 case label。

### 0.6 `goto` 跳過 static 初始化——不是問題

OP2 插入的兩個變數是 **function-local `static`**，位置在 `IN_ARM_LOADER:` 標籤**之前**，
與同處既有的 `static int iRetryCT=0, iBuf=0; static bool bTrayDuplicateErr=false;` 完全同一段。
C++ 只禁止 `goto` 跳過**automatic** storage 且帶初始化的變數；`static` 不在此限，
且 V899 用同樣佈局由 BCB6 編出過。

---

## 1. 功能敘述（搬什麼、為什麼）

### 1.1 InArm load-stage watchdog（0602，欣銓 CASE-20260602-001）

`DoInArmPickFromLoadStage_9045()` 的 `case 10 ↔ case 15` 是「等 Loader 有料／有盤」的等待迴圈。
V910（＝0602 之前的 V899）在兩種情況下會**無聲卡住**，畫面沒有任何警示：

| 死結情境 | 觸發路徑 | 0602 的處置 |
|---|---|---|
| **A. 無盤** | OP 手動把 Loader tray 拿走，且 `DoAutoSkipCheck()` 回 false（非 ASE auto-skip 客戶） | `case 15` else 分支再開一個 `else`：起 **30s** `DoInArmLoadStageNoTray_Watchdog`，逾時 `ShowErrorMessage("WAR09101", K_RETRY, MInArmX)` |
| **B. 殘留空盤** | OP 在 `WAR16122` 按了 SKIP 但沒把空盤拿走 → `fHasTray` 為真但 `HasIC()` 為假，`case10→case15→case10` 空轉 | `case 15` 有盤分支：起 **60s** `DoInArmLoadStageEmptyTray_Watchdog`，逾時同樣 `WAR09101 / K_RETRY` |

兩個 watchdog 各配一個 `static bool ...Armed` latch（**arm once**，避免每次掃描重新計時），
並在四個「情況已解除」的點主動 disarm：
`case 10` 真的有 IC（OP3）／`case 15` 有盤（OP4 開頭）／`case 15` 真的整站空（OP5 開頭）／
`DoAutoSkipCheck()` 成立（OP5 尾）。**漏搬任一個 disarm 點都會變成誤報。**

### 1.2 CC_ARDENTEC 收回閘門（0803，全智 CASE-GIGAS-20260729-001）

0602 上線時**沒有任何閘門**，所有客戶都跑。全智回報：每次 `MES1020`（空盤滿）停機、
OP 排除後按 START，**1 秒內就跳 WAR09101**。
根因寫在 V899 `ainarm9045.cpp:7263` 的註解裡：
`TQPF_Timer` 是**純 QPC 掛鐘**，沒有 Pause/Stop 概念，**把停機時間＋OP 排除時間全算成 InArm 卡站**。
0803 的處置是**只做客戶碼隔離**（新旗標 `CosFunction.bUseInArmLoadStageWatchdog`，
預設 `false`，只有 `FUNC_CC_ARDENTEC()` 設 `true`），**watchdog 邏輯本體沒有重寫**。

> 這一點與 `mg_inventory_weekly_cases.md` 的已知缺陷 **D5** 完全一致：
> 「本次只做客戶碼隔離，watchdog 邏輯本體未重寫（欣銓側仍在跑有缺陷的版本）。
> 另 WAR09102/WAR09107 訊息分家也未做。」
> **本波依鐵律 #1「搬最終態」＝把這個已知不完美的版本原樣帶進 V910。**（見 §5 F-列）

### 1.3 TrayArm 配套（0602，`acatchtray.cpp` 3 條）

**與 watchdog 是同案同日、但是另一條缺陷線**，交派任務書猜「可能是 watchdog 對 TrayArm 的配套
或 JAM 處置」——實測是後者的變體：**雷射殘料檢查收尾把 LoaderY 停錯位置**。

`DoCatchTray` 的 `case 140/145` 是「殘料檢查 From Loader」（雷射掃盤）。
掃描流程收尾的 `DoMoveOut` 會把 `MLoaderY` 停在 **front（CarPos）**；
V910 現況是檢查 pass 後直接 `Task=100` 進取盤流程，於是 **TrayArm 去夾一個不在吸料位的盤 → 夾空 → JAM0604**。

0602 的處置：pass 後改走新的中間狀態 **`case 148`**——
先退 `C_LoaderUpPress`（固定壓桿，台車才能動）→ `MOT[MLoaderY].MotorMove(Prod.iMLoaderYSurePos)`（回 rear 吸料位）
→ 放開 `C_TrayY_Fixer`、`C_LoaderEdgePush` → 才 `Task=100`。
`INSTALL_OCR_YMot != eocrYMotInstal`（沒裝 OCR Y 軸／非台車機）時 `case 148` 直接 `Task=100`。

**觸發前提鏈（＝blast radius，實測）**
`case 145` 需要 `fTrayMapping->IsRunDeviceRemainLaser()`，而
`cTrayMapping.cpp:5372-5378` 的定義是：
```
USE_TRAY_MAPPING==etmDeviceRemain && CosFunction.bTrayDeviceCheckFromLoader
&& cbEnableDeviceRemainLaser->Checked && LastSet.iRealDummy!=DUMMY
```
再加上 `case 148` 內層的 `INSTALL_OCR_YMot==eocrYMotInstal`。
→ **四道獨立閘（機台設定＋客戶 profile＋UI 勾選＋硬體選配）全開才會有新動作**；
其餘所有機台的差別只是多走一個 state tick（`Task=148` 立刻 `Task=100`）。

---

## 2. 逐 op 明細（`docs/mg_w9_ops.json`）

> 錨點欄的「唯一」＝該字串在 V910 該檔**全檔只命中一次**（實測，非估計）。
> `splice.py` 對同一檔由**下往上**套用，故同檔多 op 的行號互不位移。

### 2.1 `ainarm9045.cpp`（6 op，9222 → **9274**，+52）

| op | 矩陣行 | V899 範圍 | V910 動作 | 錨點（全檔命中） | 淨 |
|---|---|---|---|---|---:|
| **OP1** | 7057, 7060 | `7057-7062` | `after 7618` | `TQPF_Timer DoArmPickFromLoadStage_Delay;` → **[7618] 唯一** | +6 |
| **OP2** | 7080, 7082 | `7080-7083` | `after 7635` | `static bool bTrayDuplicateErr=false;` → **[7635] 唯一** | +4 |
| **OP3** | 7172 | `7170-7174` | `replace [7720,7722]` | `iFlag=CheckLoaderHasTray(false, 0, false);` → **[7722] 唯一** | +2 |
| **OP4** | 7260, 7262, **7263** | `7256-7278` | `replace [7804,7808]` | `if(MOT[MMTrayY_Car].fHasTray \|\|` → **[7804] 唯一** | +18 |
| **OP5** | 7283, 7287 | `7283-7287` | `replace [7813,7814]` | `if(DoAutoSkipCheck())` → **[7813] 唯一** | +3 |
| **OP6** | 7298, **7299** | `7288-7315` | `replace [7815,7823]` | `SetInArmUseSuckToHasNullIC(iSht, iKit);` **＋行尾 `//Steven 20241226`** → **[7816] 唯一** | +19 |

**為什麼 OP3–OP6 用 `replace` 而不是 `after`／`before`**
四個插入點的前後行都是 `{`、`}`、`Task=10;` 這種全檔重複幾百次的字串，
`after`/`before` 只能拿到弱錨。改成 `replace` 之後 `splice.py` 會在**整個取代區**內找錨字串，
於是可以把取代區擴到最近的唯一錨行，而被取代掉的上下文行**已逐位元組驗證與 V899 對應行相同**
（見 §3 的逐行對照，全部標 `=`）。

**OP5 / OP6 的相鄰關係**：取代區 `[7813,7814]` 與 `[7815,7823]` 互不重疊；
`splice.py` 依 `replace_lines[0]` 降序先跑 OP6 再跑 OP5，OP6 只動 7815 以下，不影響 OP5。

### 2.2 `acatchtray.cpp`（2 op，9320 → **9338**，+18）

| op | 矩陣行 | V899 範圍 | V910 動作 | 錨點（全檔命中） | 淨 |
|---|---|---|---|---|---:|
| **OP7** | 5915 | `5915-5918` | `replace [6648,6650]` | `fTrayMapping->yieldRemainIC.AddYieldData(true);` → **[6650] 唯一** | +1 |
| **OP8** | 5948, 5949 | `5948-5964` | `before 6680` | `        case 150:` ＋ `//kevin 20220401 loadTRAY MAP`（整行）→ **[6680] 唯一** | +17 |

> OP7 把 `Task=100;  //正常繼續跑` 換成 AI 註解 ＋ `Task=148;  //正常繼續跑(改走LoaderY歸位中間步驟)`；
> 其後兩行（`bDoTrayDeviceCheckFromLoader=false;`、`yieldRemainIC.AddYieldData(true);`）逐位元組相同。
> OP8 插在 `case 147` 的 `break;` 之後、`case 150:` 之前，與 V899 同序。

### 2.3 `CosFunction.h`（1 op，502 → **503**，+1）

| op | 矩陣行 | V899 範圍 | V910 動作 | 錨點 | 淨 |
|---|---|---|---|---|---:|
| **OP9** | 470 | `470-470` | `after 475` | `bool bCleanCountAlarmByMin;` → **[475] 唯一** | +1 |

放在 `bCleanCountAlarmByMin` 之後＝**與 V899 相對位置一致**（V899 也是 469→470）。
V910 在其後還有 11 個 V910 自有成員（`bLoginASECL`…`bInitialStartDelayCount_Init`）
與 MG-W8 剛搬進來的 0804 C05 六成員，**全部不動**。
§0.2 已實測此結構未被二進位序列化，插在中段沒有資料相容性風險。

### 2.4 `CosFunction.cpp`（2 op，4506 → **4508**，+2）

| op | 矩陣行 | V899 範圍 | V910 動作 | 錨點（全檔命中） | 淨 |
|---|---|---|---|---|---:|
| **OP10** | 2789 | `2789-2789` | `after 2824` | `{` → **250 命中，不唯一**（見下） | +1 |
| **OP11** | 4414 | `4414-4414` | `after 4482` | `CosFunction.bCleanCountAlarmByMin ... =false;` → **[4482] 唯一** | +1 |

**OP10 是本波唯一的弱錨，處置如下（不是「將就」，是可證明的機制）**

V910 的 `FUNC_CC_ARDENTEC()` 是 `void FUNC_CC_ARDENTEC()` / `{` / `}` 三行空殼，
函式內唯一可插入的位置前後就只有 `{` 和 `}`。而**能一次拿到強錨的 `replace` 寫法會連帶
把 V899 的另外三行搬進來**（`bTrayDeviceCheckFromLoader` / `bTrayOCR` / `bFTPFunction`），
那三行**不在本波矩陣範圍**且會改變 V910 欣銓機台行為 → 依範圍權威**不搬**（見 §5 F-列）。

安全網（機械性，非人為紀律）：
1. `splice.py` 同檔依錨行**降序**套用 → **OP11（4482，唯一錨）一定先跑**。
2. `splice.py` 是**整檔 all-or-nothing**：任一 op 錨點不符 `sys.exit(1)`，**在寫檔之前**中止。
3. 任何會讓第 2824 行漂移的位移，必定同時讓第 4482 行漂移（位移發生在 2824 之上時兩者同漂；
   發生在 2824 與 4482 之間時 4482 單獨漂）→ **OP11 必先失敗並整檔中止**。
   反之，發生在 4482 之下的位移對 2824 無影響。
   ⇒ **OP10 只可能在「4482 仍是那一行」的檔案上執行。**
4. 額外硬前提：套用時 `CosFunction.cpp` 必須是 **4506 行 / md5 `66bcd93cdedc`**（見 §6 gate）。

---

## 3. 事前模擬（記憶體內套用；輸出只寫進 scratchpad，V910 樹未動）

| 檔 | 套用前 | 套用後 | Δ | cp950 | CRLF / bare-LF | 0602+0803 簽章 |
|---|---:|---:|---:|---|---|---:|
| `ainarm9045.cpp` | 9222 | **9274** | +52 | OK | 9274 / **0** | **12 / 12** |
| `acatchtray.cpp` | 9320 | **9338** | +18 | OK | 9338 / **0** | **3 / 3** |
| `CosFunction.cpp` | 4506 | **4508** | +2 | OK | 4508 / **0** | **2 / 2** |
| `CosFunction.h` | 502 | **503** | +1 | OK | 503 / **0** | **1 / 1** |

逐 op 對帳：`+6 +4 +2 +18 +3 +19 = +52`；`+1 +17 = +18`；`+1 +1 = +2`；`+1` ✓
簽章合計 **18 / 18** ＝ 矩陣 18 條，一條不多一條不少。

### 3.1 結構性證明（比對「敘述集合」而不是行數）

把套用後的模擬檔與 V899 對應區段整段 unified diff：

| 比對窗 | 結果 |
|---|---|
| `ainarm9045.cpp` V899 `[7055-7320]`（266 行） vs SIM `[7617-7882]`（266 行） | **只有 2 個 diff hunk，兩者都是 `case 1` 裡 V899 多出的空白行**（V899:7124、7130 附近），與本波無關、屬兩樹既有的排版差異。**watchdog 相關的每一行都對得上。** |
| `acatchtray.cpp` V899 `[5905-5965]`（61 行） vs SIM `[6638-6698]`（61 行） | **完全相同（0 diff）** |

### 3.2 附帶驗證：`DoAutoSkipCheck()` 兩樹等價

watchdog 的觸發頻率取決於 `DoAutoSkipCheck()`。
兩樹整支函式（V899:6776 / V910:7341，各 45 行）unified diff **只有 1 個 hunk，且是註解對齊欄位的空白差**，
程式邏輯逐字相同 → 搬過去之後 watchdog 的行為與 V899 現場一致。

---

## 4. 反向發現（V910 有、V899 沒有）——**不可回搬**

| # | 位置 | V899 | V910 | 判定 |
|---|---|---|---|---|
| R1 | `CosFunction.cpp` `InitialCosFunction()` 尾 | `4425` `if(fAGV->IsATK_AMR())` | **`4501` `if(fAGV!=NULL && fAGV->IsATK_AMR())`** | **V910 多一道 NULL 防呆，較安全**。本波不碰該行（OP11 插在 4482 之後、4483 之前），確認不會倒回去。 |
| R2 | `ainarm9045.cpp` `case 1` | 多兩個空白行 | 無 | 純排版，兩樹既有差異，本波不動 |
| R3 | `CosFunction.cpp` 各 `FUNC_CC_*` 空殼 | 函式體內有一個空白行 | 無空白行 | 純排版；也是 OP10 不能用 `replace` 擴大取代區的次要原因 |

---

## 5. 建議寫入 `MG_FINAL_DECISIONS.md` 的項目（<95%，本波不出 op）

> **本分析 agent 全程唯讀，未動 `MG_FINAL_DECISIONS.md`。** 以下是給主腦直接貼上的建議列。

```
| F7 | MG-W9（0602/0803 欣銓 watchdog 線）— V910 的 FUNC_CC_ARDENTEC 是空殼 |
V899 `CosFunction.cpp:2786-2789` 的 FUNC_CC_ARDENTEC 有四行；V910 `CosFunction.cpp:2823-2825` 是空函式。
本波只補第四行（`bUseInArmLoadStageWatchdog=true`，屬矩陣範圍），
另三行 `bTrayDeviceCheckFromLoader` / `bTrayOCR` / `bFTPFunction`（JimmyChiu 20220219 起）**不在矩陣範圍故未搬**。
後果：`IsRunDeviceRemainLaser()` 需要 `CosFunction.bTrayDeviceCheckFromLoader==true`
（`cTrayMapping.cpp:5372-5378`），而 V910 只有另外 5 個客戶 profile 設它
（CosFunction.cpp:75 / 653 / 1446 / 3642 / 3708，**不含 ARDENTEC**）
→ **本波搬進去的 acatchtray `case 148` 修正，在 V910 的欣銓機台上目前跑不到**（在別的客戶機台上會生效）|
(a) 維持現狀，只搬矩陣範圍（已執行） (b) 另補 ARDENTEC 那三行（會同時打開欣銓的 Tray OCR＋FTP＋殘料檢查，屬功能開關不是 bug 修正） (c) 只補 bTrayDeviceCheckFromLoader 一行 |
**建議 (a) 先維持**，並把 (b)/(c) 當成獨立需求問客戶：V910 的 ARDENTEC profile 為什麼是空的，
本來就是搬移範圍以外的既存差異，不應由本波夾帶決定。 |

| F8 | MG-W9（0602/0803）— acatchtray `case 148` 是**閘外**變更 |
0602 的 TrayArm 配套（OP7/OP8）在 V899 最終態就**沒有客戶碼閘門**，本波忠實照搬。
blast radius 實測為四道獨立閘全開才有新動作：`USE_TRAY_MAPPING==etmDeviceRemain`
＋客戶 profile `bTrayDeviceCheckFromLoader` ＋ UI `cbEnableDeviceRemainLaser` 勾選
＋ `INSTALL_OCR_YMot==eocrYMotInstal`；其餘機台只是多一個 state tick。
但它確實新增了一次 `MOT[MLoaderY].MotorMove()` 與三顆氣缸釋放（D 類）|
(a) 忠實照搬不加閘（已依 ≥95% 規則執行） (b) 額外加 `CUSTOMER_CODE==CC_ARDENTEC` 閘 |
**建議 (a)**：加閘＝新寫程式碼（違反 splice 鐵律），且這是明確的缺陷修正（LoaderY 停 CarPos 導致夾空 JAM0604），
其他有相同硬體組態的客戶同樣會踩到。若使用者要求最保守，(b) 需另開實作波。 |

| F9 | MG-W9（0803）— watchdog 本體的掛鐘缺陷一併帶進 V910 |
依鐵律 #1 搬最終態，等於把「`TQPF_Timer` 是純 QPC 掛鐘、把停機＋OP 排除時間算成卡站」
這個已知缺陷原樣帶進 V910（僅以 CC_ARDENTEC 隔離）。
與 `mg_inventory_weekly_cases.md` 已知缺陷 D5 同一件事；`WAR09102/WAR09107` 訊息分家也同樣沒做 |
(a) 忠實搬運，缺陷歸 V899 議題（已執行） (b) 搬移時順手改成 stop-aware（**兩樹都要改**，屬 V899 案不是搬移案）|
**建議 (a)**：V899 出貨版現況即如此，兩樹一致才比得對；要修應由 `ht9045-v899` 另開案，修好再同步。 |
```

---

## 6. 收工 gate 建議（給主腦）

1. **硬前提（套用前先驗，任一不符就停）**

   | 檔 | 必須是 | md5(12) |
   |---|---:|---|
   | `ainarm9045.cpp` | 9222 行 | `50c6e28e745e` |
   | `acatchtray.cpp` | 9320 行 | `33bb3708306a` |
   | `CosFunction.cpp` | **4506** 行 | `66bcd93cdedc` |
   | `CosFunction.h` | **502** 行 | `374076924655` |

   （`CosFunction.*` 兩檔的數字**已含 MG-W6 與 MG-W8**。若之後又有別的波先動這兩檔，
   本波 op 的行號必須重量；`splice.py` 會因 OP11／OP9 的唯一錨失敗而整檔中止，是安全失敗。）

2. `python tools/port_tools/splice.py docs/mg_w9_ops.json`
   - 預期輸出：
     ```
     OK ainarm9045.cpp: 6 op(s) applied, eol=CRLF
     OK acatchtray.cpp: 2 op(s) applied, eol=CRLF
     OK CosFunction.h: 1 op(s) applied, eol=CRLF
     OK CosFunction.cpp: 2 op(s) applied, eol=CRLF
     ```
   - 產生 4 個 `.mgbak`（目前樹上 `.mgbak` 為 0 個）
   - 套用後行數：**9274 / 9338 / 503 / 4508**

3. **套用後位移表**（後續波若要動這四檔，行號用這張表換算）

   `ainarm9045.cpp`（9222 → 9274）
   ```
   N ≤ 7618          → N
   7619 ≤ N ≤ 7635   → N+6        （OP1 插 6 行於 7618 之後）
   7636 ≤ N ≤ 7719   → N+10       （OP2 插 4 行於 7635 之後）
   N = 7720..7722    → 7730..7734 （OP3 replace 3→5）
   7723 ≤ N ≤ 7803   → N+12
   N = 7804..7808    → 7816..7838 （OP4 replace 5→23）
   7809 ≤ N ≤ 7812   → N+30
   N = 7813..7814    → 7843..7847 （OP5 replace 2→5）
   N = 7815..7823    → 7848..7875 （OP6 replace 9→28）
   N ≥ 7824          → N+52
   ```
   `acatchtray.cpp`（9320 → 9338）
   ```
   N ≤ 6647          → N
   N = 6648..6650    → 6648..6651 （OP7 replace 3→4）
   6651 ≤ N ≤ 6679   → N+1
   N ≥ 6680          → N+18       （OP8 插 17 行於 6680 之前）
   ```
   `CosFunction.h`（502 → 503）
   ```
   N ≤ 475 → N ; N ≥ 476 → N+1
   ```
   `CosFunction.cpp`（4506 → 4508）
   ```
   N ≤ 2824 → N ; 2825 ≤ N ≤ 4482 → N+1 ; N ≥ 4483 → N+2
   ```

4. `bcc32 -c -H-` 單檔隔離編譯 **四個檔**（`ainarm9045.cpp`、`acatchtray.cpp`、
   `CosFunction.cpp`、`CosFunction.h` 隨 cpp 帶入）。
   §0.4／§0.5／§0.6 已把可預見的編譯風險（符號可見性、`#include` 缺漏、重複 case label、
   名稱衝突、`goto` 跨初始化）逐項排除，仍請實編確認。

5. Gate 驗證
   - `python tools/port_tools/port_check.py`
     → 預期**新增行 100% 判 SPLICED**（每一行都逐位元組取自 V899 同路徑檔）、
       **0 個 AUTHORED**、cp950 錯誤數 0、EOL 全 CRLF 無混用。
   - `python tools/port_tools/ai_comment_matrix.py` 重算
     → 總 MISSING **240 → 222**（本波 18 條**全部**轉 PRESENT，沒有任何一條會留下）。

6. **LEDGER 記錄要點**
   - 相依鏈 **#2 本波結清**：0602（13）＋0803（5）＝18 條，**P 類 18 / C 類 0 / X 類 0**。
   - 這是目前唯一「矩陣說缺就是真的全缺」的波次；前幾波的 C 類地形在這條線上不存在
     （V910 的 `case 15` 與 `case 145` 就是 0602 之前的 V899，逐位元組相同）。
   - 閘門形狀：`CC_ARDENTEC=870` 兩樹相同，`FUNC_CC_ARDENTEC()` 兩樹都在
     `DoCustomerFunction()` 的 dispatch 內；非欣銓機台行為 0 變更。
   - **`acatchtray` 那 3 條是閘外**（見 F8），blast radius 已量化。
   - 反向發現 R1（`fAGV!=NULL` 防呆）已確認未被本波倒回。
   - 三條建議進 `MG_FINAL_DECISIONS.md`（§5 的 F7/F8/F9）。
   - 交派任務書的客戶歸屬需更正：**0602＝欣銓、0803＝全智**（依 inventory #19/#34）。
