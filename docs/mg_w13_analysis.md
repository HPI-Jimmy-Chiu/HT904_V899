# MG-W13 分析：20260819 超豐熱風槍 ＋ 20260820 PTI 尾波

- **波次**：MG-W13（V899 → V910），LEDGER 日期列 **20260819（10 條）＋ 20260820（7 條）全結清**
- **來源**：`docs/mg_ai_matrix_missing.csv` **17 條 MISSING**（0819 × 10、0820 × 7）
- **CASE**：`CASE-20260819-001`（超豐：MO 工單 Temperature mode 含 HOT AIR 自動開熱風槍）、
  PTI 20260820 三件（Fix1 rotate module 未指派 Category 的 Pass/Fail 判定、
  `SettingBinCategoryFromServer` 區域陣列越界、O06 週期 log Row A ／站標籤修正）
- **V899 樹**：`D:\HT9045\HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422`
  （唯讀；`git diff b515ed5 HEAD -- <V899 樹>` 為空 → 工作區＝在製收斂端點 `b515ed5`）
- **V910 樹**：`D:\HT9045\HT9011UC_Code_V3.33.910.0_20260716_Jimmy`
  （MG-W12 `56a9815` 後，`git status` 乾淨、`*.mgbak` **0 個**）
- **本文件性質**：分析＋splice op 草案。**全程唯讀，兩棵樹一個位元組都沒有被修改**
  （模擬結果與整棵樹的編譯探針副本都只寫進 scratchpad，用完已刪）
- **op 檔**：`D:\HT9045\docs\mg_w13_ops.json`（**12 個 op，6 個檔**）
- **類別**：A 類（客戶碼閘功能）×2 主題 ＋ **B 類（記憶體安全）×1** ＋ **對外格式 ×1**。
  忠實搬運、錨點全部全檔唯一 → 可執行。**<95% 項目：0 個**（詳見 §6.1）

---

## ⚠ 開工前必須先知道的五件事

1. **本波已做過「套用後 bcc32 實編」**，不是只做模擬。方法：把整棵 V910 複製到 scratchpad
   （2,208 檔）、覆蓋 6 個模擬結果、在副本上跑 `bcc32 -c -H- -w- -D_VER6`。
   結果與波前基準逐檔比對：`CosFunction.cpp` / `ProductionInfo\ProductionInfo.cpp` /
   `cBinSel.cpp` **皆 0 錯誤**（波前也 0），`main.cpp` **恰好 3 個既有錯誤、無回歸**。
   → 本波不存在「符號看不見／缺 include／多載不匹配」的風險，已實編排除（§3.6）。

2. **任務書把 0820 的越界修正掛在 `cBinSel.cpp` 上，實際在 `ProductionInfo.cpp:5596`。**
   矩陣列得很清楚：`cBinSel.cpp` 的 2 條是「Fix1/Fix4 未指派 Category 時預設為 Fail」，
   越界那條是 `ProductionInfo\ProductionInfo.cpp,5596`（`SettingBinCategoryFromServer`）。
   兩者都在 0820、都與 PTI Fix1 有關，但是**不同的檔、不同的缺陷、不同的閘門形狀**（§1.3／§1.4）。

3. **越界修正「程式碼無閘」但「執行期到不了」——不要把它當成全客戶行為變更。**
   `SettingBinCategoryFromServer()` 的到達路徑是
   `CosFunction.bOEEFunction`（**只有 `FUNC_CC_Greatek()` 設 true**，V910 `CosFunction.cpp:1430`）
   → `IniConfig.bN14_21_SetUpConfiguration`（非 Greatek 在 `cConfiguration.cpp:3587` 被註冊為
   `bNoShow, bDisable, bFixedValue, 0`＝恆 0）
   → 函式第一行 `if(sucSetUp.sHANDLER_TYPE!="HT9046") return;`。
   **實際受影響＝超豐且啟用 N14_21 伺服器套 recipe、且 setup file 是 HT9046 型的機台。**
   修正方向是純粹把上界收緊，任何路徑上都不會比現況更壞（§1.4）。

4. **O06 週期 log 是本波唯一「沒有客戶碼閘、且會改變對外檔案格式」的變更**，而且不是
   「多印幾格」而是**欄位數與標籤語意都變**（`iShtRow==1` 的機台原本一格都沒印）。
   消費者是客戶端 RMS（`D:\RMS`）。必須進 LEDGER 外溢欄（§5.2／§6.2）。

5. **`cTemperFrom.cpp` 在 `b515ed5` 也有一行變更，本波刻意不搬**——它不屬於 0819 主題。
   證據三項：(a) 無 `//AI` 標記 → 不是矩陣點、沒有驗收簽章；
   (b) `cTemperFrom.dfm` 的 `object Button6` 是 `Caption = 'AlarmSimulator'` 且
   **`Visible = False`** → 正式機按不到；
   (c) 改的是模擬哪一支警報（`JAM0416`→`WAR07301`，前者是 Shuttle JAM、後者是 SmartAutoClean
   CTF 連續 Fail），與熱風槍毫無關係，是開發者當下除錯用的暫存值（§6.3）。

---

## 0. 前置事實（全部實測）

### 0.1 檔案基本盤（**md5 前提清單**，套用前必須逐檔重驗）

| 檔 | V899 行數 / md5(12) | **V910 行數 / md5(12)** | EOL | cp950 | 被前波動過？ |
|---|---|---|---|---|---|
| `CosFunction.cpp` | 4430 / `a77b6cd3dc2e` | **4508 / `5456c2d54479`** | 兩樹全 CRLF | OK | **是（W6/W8/W9）** |
| `CosFunction.h` | 487 / `bba5b164ad3d` | **503 / `3e7384835f33`** | 兩樹全 CRLF | OK | **是（W6/W8/W9）** |
| `ProductionInfo\ProductionInfo.cpp` | 6147 / `72090b63c9f4` | **6078 / `9da6852ffeb6`** | 兩樹全 CRLF | OK | 否（pristine） |
| `ProductionInfo\ProductionInfo.h` | 596 / `95b04e0a2548` | **595 / `157b95f5bb38`** | 兩樹全 CRLF | OK | 否（pristine） |
| `cBinSel.cpp` | 6526 / `bc7a0d4d2b42` | **6666 / `d5c61fd21890`** | 兩樹全 CRLF | OK | 否（pristine） |
| `main.cpp` | 33575 / `81596aa5852b` | **35350 / `833b10b84827`** | 兩樹全 CRLF | OK | **是（W4/W10/W12）** |

> 本文件所有 V910 行號都量在**上表 md5 對應的磁碟現狀**上，不是任何舊快照。
> V910 樹 `.mgbak` 為 0 個且 `git status` 乾淨 → `port_check.py` 會拿 `git HEAD` 當基準，與上表一致。
>
> 三個關鍵前提（任一不符就停，行號必須重量）：
> **`main.cpp`＝35350 行／`833b10b84827`；`CosFunction.cpp`＝4508 行／`5456c2d54479`；
> `ProductionInfo\ProductionInfo.cpp`＝6078 行／`9da6852ffeb6`。**
> `splice.py` 會因唯一錨不符而**整檔中止**（all-or-nothing），是安全失敗。

### 0.2 範圍權威：V899 只有一個來源 commit

`git log bb69c60..b515ed5` 對本波 6 個檔**恰好 1 個 commit**：

| commit | 時間 | 主題 | 本波取用的檔 |
|---|---|---|---|
| `b515ed5` | 08-26 14:46 | V899 在製收斂：20260817-0820 三案累積變更 | `CosFunction.cpp/h`、`ProductionInfo/ProductionInfo.cpp/h`、`cBinSel.cpp`、`main.cpp` |

`b515ed5` 另外還動了 `cObserver.cpp`(63)、`cTrayAssignment.cpp`(11)、`cTemperFrom.cpp`(1)：

- `cObserver.cpp` ＋ `cTrayAssignment.cpp` ＋ `CosFunction.cpp/h` 的 **0817 那 11 條已於
  MG-W3／W6／W8 全數落地**（實測：V910 `ParseEventLogLine` 命中 5 處、
  `bDisableAutoTrayFeed` 在 `CosFunction.cpp:2879/4391`、`CosFunction.h:357`、
  `cTrayAssignment.cpp:269/851` 都在）→ 與本波無關。
- `cTemperFrom.cpp` 的一行**刻意不搬**（理由見 §⚠5／§6.3）。

**無中間態被撤回、無跨日叢集需要合併。** 本波逐 op 內容已與 `b515ed5` 的 diff 逐 hunk 對帳，
一一對應、無多無少。

### 0.3 符號稽核（V910 全樹，排除 `.svn`）

**4 個新符號在 V910 命中數皆為 0** → 17 條 MISSING 判定成立，無「已搬一半」，也不會撞名：

`bHotAirByMOTemperatureMode` / `ApplyHotAirByMOTemperatureMode` / `bSkipFix1PassFail` / `bNoCategoryToFix1`

同時對 `bb69c60`（V899 pristine）反查：這 4 個符號在 V899 基線也是 **0** → 確認是 `b515ed5` 新增，
不是「V910 從舊 V899 fork 掉的東西」。

新程式用到的**既有**符號全部存在且可見（逐一實測）：

| 符號 | V910 宣告處 | 目標檔看得到嗎 |
|---|---|---|
| `INSTALL_HEAT_GUN` | `cmydef.h:3390` | ✓ `ProductionInfo.cpp` include `cmydef.h` |
| `sLoadMO_TemperatureMode` | `ProductionInfo\ProductionInfo.h:310`（`TfProductionInfo` 成員） | ✓ 本類別內 |
| `Temperature.bActiveHeatGun` / `.bUseCDAOnly` | `cprod.h:1419 / 1421` | ✓ |
| `fTemp_Set` / `cbHeaterGun` | `uTemp_Set.h:1071 / :69` | ✓ `ProductionInfo.cpp:27` include `uTemp_Set.h` |
| `fLotInfo->WriteFTPSetupFileChangeLog` | `uLotInfo.h:1367` | ✓ `ProductionInfo.cpp:16` include `uLotInfo.h` |
| `iStackFail_Pass_Length`（＝9） | `ProductionInfo\ProductionInfo.cpp:5228`（檔案層 const） | ✓ 同檔、在 5525 之前 |
| `CUSTOMER_CODE` / `CC_PTI`(957) / `CC_Greatek`(956) | `cmydef.h:3205` / `MachineType.h:326` / `:325` | ✓ |
| `e3PosFix1`(4) / `e3PosFix4`(7) / `e3Fix1`(3) / `eFix1`(6) / `eFix4`(9) / `eTrayCount`(33) | `MachineType.h:1242/1245/1166/1132/1135/1159` | ✓ |
| `iTestBinCount` / `iCatDataT3Pos` / `iTo3Unload` | 全樹既有（`cprod.h:2636` 等） | ✓ |

**`#include` 閉包：本波不需要新增任何 `#include`。**
`ProductionInfo.cpp` 的 44 行 include 清單在兩樹**逐行位元組相同**（已實測 diff 為空）。

### 0.4 四個主題的閘門形狀

| 變更 | 閘門（由外到內） | 非目標客戶的行為 |
|---|---|---|
| **0819 熱風槍**（W13-01..05、07） | ① `CosFunction.bHotAirByMOTemperatureMode`——`InitialCosFunction()` 設 `false`，**只有 `FUNC_CC_Greatek()` 設 `true`**（B 類原生閘）② `INSTALL_HEAT_GUN<=0` 早退 ③ 呼叫點在 `OEE_StartLot()` 內，而 OEE 整組功能由 `CosFunction.bOEEFunction`（同樣只有 Greatek 開）把 N14 設定頁鎖住 | 函式**第一行就 return**；指令級零變更 |
| **0820 PTI Fix1 Pass/Fail**（W13-06、09、10） | `CUSTOMER_CODE==CC_PTI`（957）——兩處都是「先算旗標、非 PTI 時旗標恆 false」 | 新增的 `if` 主體永不執行；原有敘述一行未動 |
| **0820 越界修正**（W13-08） | **程式碼層面無閘**；執行期由 `bOEEFunction`(Greatek) → `bN14_21_SetUpConfiguration`(非 Greatek 恆 0) → `sHANDLER_TYPE=="HT9046"` 三層收斂 | 到不了該函式；且即使到得了，新上界只會**更小**，不可能比現況差 |
| **0820 O06 週期 log**（W13-11、12） | **無客戶碼閘**。`IniConfig.bO06SaveLogTimePeriod`（`config.ini [Event Log] EnanleTimePeriodSaveLog`，預設 0＝關），外層 `IniConfig.bEventLogAutoSaveFunction`（`InitialCosFunction()` 預設 **true**，全客戶可見可編） | **任何開了 O06 週期存檔的客戶都會看到輸出格式改變** → §5.2／§6.2 |

### 0.5 V910 的公司漂移地圖（本波目標區）

| 檔 | 目標區內／附近的漂移 | 對本波的影響 |
|---|---|---|
| `CosFunction.cpp` | `FUNC_CC_Greatek()` 尾端與 `InitialCosFunction()` 的 PowerSave 群兩樹**逐行相同**（W8 已搬） | 零。插入點與 V899 相對順序一致 |
| `CosFunction.h` | V910 `486-487` 多 `bZHomingAfterPickErr` / `bInitialStartDelayCount_Init`（V899 對應處是 `bUseInArmLoadStageWatchdog`），但都在 PowerSave 群**之前** | 零。新宣告接在 `bPowerSaveShowCaption` 之後，與 V899 同位置 |
| `ProductionInfo\ProductionInfo.h` | **兩樹唯一差異就是本波要搬的那一行**（實測 `diff` 只有 `238d237`） | 零。套用後該檔與 V899 **位元組完全相同**（已實測） |
| `ProductionInfo\ProductionInfo.cpp` | **有真實漂移，且就在插入點的同一個函式裡**：V899 `CheckMOInformation()` 的 `1874-1929` 有一整套 `sTempErrorDetail` 溫度錯誤明細（Mode Mismatch／Temperature Mismatch／Soak Time Mismatch 帶數值），V910 是舊的「Check Temperature Error」單句版。另 V899 `403-404`、`1683-1685` 多幾行 `delete slOEEReport;` 洩漏修補 | **不碰**。經 `bb69c60` 反查，這些**在 V899 pristine 就有**＝fork 期分歧，不在 `bb69c60..b515ed5` 範圍內，也沒有 `//AI` 標記 → 不是本戰役工作量（見 §5.3 R2） |
| `cBinSel.cpp` | 目標區 `1378-1420` 與 V899 `1360-1410` **逐行相同**，兩個新塊是唯二差異 | 零 |
| `main.cpp` | **有真實漂移，就在被取代的那一行上**：V910 `31393` 帶 `//Steven 20260421 : cast to int for %c`，V899 對應行原本帶 `//AI(ht9045-v899) 20260515`——**兩樹各自獨立修過同一個 `%c` 轉型問題** | 見 §5.3 **R1**：V899 的 0820 終態把整個除法拿掉了，Steven 那句註解所描述的程式碼已不存在，隨行一起被取代是正確的 |

### 0.6 未標記承重行（矩陣看不到，漏搬即壞）

本波 12 個 op 共搬 **91 行**：

| 類別 | 行數 |
|---|---:|
| 矩陣列出的 `//AI` 標記行 | **17** |
| **V899 新增但無 `//AI` 標記的程式碼行** → 矩陣盲區 | **63** |
| 新增空行 | 8 |
| V910 原本就有、僅為取得唯一錨點而一併重插的既有行（位元組相同、difflib 會判為 equal） | 3 |

**63 行盲區集中在四個「一條註解帶一整段實作」：**

| V899 位置 | 內容 | 矩陣只列 | 漏搬的後果 |
|---|---|---|---|
| `ProductionInfo.cpp:2052-2090` | `ApplyHotAirByMOTemperatureMode()` **整支函式**（39 行） | `2052/2063/2067/2074/2083` 五行 | **link error**（.h 有宣告、`OEE_StartLot` 有呼叫，卻沒有定義） |
| `ProductionInfo.cpp:1965-1985` | `bool bSkipFix1PassFail=false;` ＋ CC_PTI 偵測迴圈（15 行） | `1965`（註解首行）、`1983` | **compile error**（`1983` 用到未宣告的 `bSkipFix1PassFail`）。註 `1966/1967` 是續行註解，不帶 `//AI(` 前綴 → 矩陣看不到 |
| `cBinSel.cpp:1371-1383` | `bool bNoCategoryToFix1=(CUSTOMER_CODE==CC_PTI);` ＋ 偵測迴圈（13 行） | `1371` 一行 | **compile error**（W13-10 用到未宣告的 `bNoCategoryToFix1`） |
| `cBinSel.cpp:1396-1401` | `if(bNoCategoryToFix1 && (i==eFix1\|\|i==eFix4) && ...) iT6IsFail[i]=1;` 主體（6 行） | `1395` 一行 | **修正等於沒搬**（Fix1/Fix4 仍顯示成綠色好品） |

> 本波所有 op 都以**連續區段**抽取，上述行自然被包含——這正是「用區段不用逐行」的理由。
> 另外八個 op（W13-01/02/03/04/05/08/11/12）都是「標記行本身就是全部變更」，**零盲區**。

---

## 1. 四個主題的變更敘述（搬什麼、為什麼）

### 1.1 依 MO 工單 Temperature mode 自動開關熱風槍（CASE-20260819-001）→ **W13-01..05、07**

超豐（`CC_Greatek`）的 MO 工單有 `Temperature mode` 欄位。原本換工作檔後熱風槍
（`Temperature.bActiveHeatGun`）只跟隨工作檔的 `[Mode] Active_Heat_Gun`，
OP 必須手動去 `fTemp_Set` 勾「Active Heater Gun」，換工單常漏勾。

**最終態＝在 `OEE_StartLot()` 換完工作檔之後，依 MO 欄位自動同步旗標與畫面勾選。**
三條規則（V899 註解已寫死在程式裡，原樣搬運）：

| MO `Temperature mode` | 動作 |
|---|---|
| 去頭尾空白後為空字串（含舊工單無此欄） | **不動作**，保留工作檔原設定（並寫一行 FTP change log） |
| 去空白後轉大寫含 `"HOT AIR"` | 開啟熱風槍 |
| 其餘（例 `"Hot"` / `"Ambient"`） | 關閉熱風槍 |

四個設計細節，V910 端已逐項驗證前提成立：

1. **呼叫點順序承重**：必須在 `AutoDownloadSetupFileByMO()`（V910 `:320`）之後，
   否則會被 `ReadTempFile()` 重讀工作檔蓋掉。V910 的插入點是 `:343`
   （`SaveInfoFileWhenStart()` 之前）→ 順序與 V899 一致。
2. **`bUseCDAOnly` 互斥是照抄 `ReadTempFile()` 的既有規則**，該規則在 V910
   `uTemp_Set.cpp:2341-2352` **原封存在**（`if(INSTALL_HEAT_GUN>1){ if(bUseCDAOnly) bActiveHeatGun=false; } else bUseCDAOnly=false;`）。
   附帶一提：`ApplyHotAir` 的硬體閘是 `INSTALL_HEAT_GUN<=0` 早退，而互斥規則的閘是 `>1`；
   `INSTALL_HEAT_GUN==1` 時 `bUseCDAOnly` 被 `ReadTempFile` 釘成 false → 兩者不會打架。
3. **同步 UI 勾選無副作用**：`cbHeaterGun` 在 `uTemp_Set.dfm:4640` **沒有 `OnClick` 事件**
   → `->Checked=` 只重繪，不會觸發任何 handler（VCL 下這一點要實查，本波已查）。
4. **不回寫工作檔**：函式只動執行期旗標與勾選。（留痕：`uTemp_Set.cpp:4675`
   `SaveSetupFile()` 會把 `cbHeaterGun->Checked` 寫進工作檔——**那是 OP 主動按存檔才會走的路徑**，
   V899 出貨版即如此，不是本波新增的行為。）

### 1.2 PTI Fix1 未指派 Category 時預設為 Fail（`cBinSel::ReadFile`）→ **W13-09、10**

PTI HT-90* 的 **Fix1 位置是 rotate module**，不會被指派 Category。
`MyBinPanel[tag]->iT6IsFail[]` 從 ini 讀 `Pass/Fail`，鍵不存在時預設 **0（Pass）**
→ 沒在用的 Fix1 在畫面上顯示成**與好品相同的綠色**，操作員誤判。

**最終態**：在 `ReadFunctionData(tag)` **之後**（此時 `BinSelect[tag].iCatDataT3Pos[]` 才剛被填好，
實測填值在 `cBinSel.cpp:5370-5401`，而 `ReadFunctionData` 定義在 `:4809`、其後到 `:5450` 沒有別的
`TfBinSel::` 定義 → 順序前提在 V910 成立）先掃一遍
「有沒有任何 bin 被指派到 `e3PosFix1` 或 `e3PosFix4`」；
若整台機都沒有，就在既有的 `for(i=0; i<eTrayCount; i++)` 迴圈裡把 `eFix1`(6) / `eFix4`(9)
的 `iT6IsFail` 由 0 改 1。

- **為什麼 Fix4 要一起**：Fix4 是 Fix1 的下半盤，GPIB `AlarmSetup` 會因為
  「同一個 Fix 盤上下 Pass/Fail 不一致」而擋下設定。
- **不用另外改 T3 映射**：插入點正好在既有那行
  `BinSelect[tag].iStackDefFailCate[iTo3Unload[i]]=MyBinPanel[tag]->iT6IsFail[i];` **之前**，
  T6→T3 換算由既有程式完成 → 與 `iTo3Unload[]` 的實際內容無關，不需假設。

### 1.3 PTI Fix1 造成的 `Check PASSBIN Error` 誤報（`CheckMOInformation`）→ **W13-06**

§1.2 的修正有個必然的副作用：`iStackDefFailCate[e3Fix1]` 變成 1（Fail），
而 `CheckMOInformation()` 會拿它跟 MO 的 `PASSBIN` 字串逐格比對
→ **誤報 `Check MO Information Fail!#Check PASSBIN Error` 擋住 Start**。

**最終態**＝在 PTI 且「確實沒有任何 bin 指派到 `e3PosFix1`」時，比對迴圈跳過 `i==e3Fix1`。

算術對得起來：`iPassFailCount = sLoadMO_PASSBIN.Length()+1` 且 `>5` 直接判 fail
→ 迴圈 `i` 只走 0..4 ＝ `e3Auto1(0)/e3Auto2(1)/e3Auto3(2)/e3Fix1(3)/e3Fix2(4)`。
**PASSBIN 最多 4 碼，最遠只涵蓋到 `e3Fix2`，所以只需要排除 `e3Fix1`**——
V899 註解這句話在 V910 的 `MachineType.h:1163-1167` 逐值核對成立。
（`e3Fix4`＝6，超出迴圈上界，所以 §1.2 對 Fix4 的寫入不可能造成 PASSBIN 誤報。）

> **兩處的偵測述詞刻意不同**：`cBinSel` 查 `e3PosFix1 || e3PosFix4`，
> `CheckMOInformation` 只查 `e3PosFix1`。這不是筆誤——後者關心的是「PASSBIN 會不會比到 Fix1」，
> 而 Fix4 根本比不到。**照搬，不要「順手統一」。**

### 1.4 `SettingBinCategoryFromServer` 區域陣列越界（B 類記憶體安全）→ **W13-08**

```
int iBinSelectLength = sizeof(BinSelect[iTestRunMode].iStackDefFailCate)/sizeof(...[0]);   // = eTrayCount = 33
AnsiString asStackFail_Pass[iStackFail_Pass_Length] = { ...9 個... };                       // = 9
for(int i=0; i<iBinSelectLength; i++)                                                       // 0..32  ← 越界 24 格
    BinSelect[eBinFT].iStackDefFailCate[i] = (asStackFail_Pass[i]=="Pass") ? 0 : 1;
```

`eTrayCount` 實測＝33（`MachineType.h:1126-1159`，`eMag14=32` 之後），
`iStackDefFailCate[eTrayCount]`（`cprod.h:2636`）。
`asStackFail_Pass` 是**堆疊上的 9 元素 `AnsiString` 陣列**——
`i` 走到 9..32 時，程式把堆疊上的任意位元組當成 `AnsiString`（＝一個帶引用計數標頭的堆積指標）
去做 `operator==`，**是解參考任意指標**，不是「讀到怪值」而已。同時還把 24 格垃圾 Pass/Fail
寫進 `BinSelect[eBinFT].iStackDefFailCate[9..32]`。

**最終態＝把上界夾到兩者的最小值**：`for(int i=0; i<iStackFail_Pass_Length && i<iBinSelectLength; i++)`。
`iStackFail_Pass_Length`（＝9）在 V910 `ProductionInfo.cpp:5228` **早就存在**，
就在同一個檔、在 5525 之前 → 直接可用，不需要新增任何宣告。

**行為差**：修正後 `iStackDefFailCate[9..32]` 保留原值（而不是被寫成垃圾）——這是正確方向。

### 1.5 O06 週期 Production log：Row A 整排未記錄 ＋ 站標籤重複 → **W13-11、12**

`TfMain::TimerESDTimer()` 裡的 `IniConfig.bO06SaveLogTimePeriod` 週期存檔，
`str` 尾段會附上每個 Arm×Site 的良率。V910 現況（＝記憶中「其餘 16 棵樹仍有」的舊版）：

```
for(int i=1; i<TestSocket.iShtRow; i++)          //  ← 從 1 起算
    for(int j=0; j<TestSocket.iShtCol; j++)
        sTemp.sprintf("%c%c:%s;",
            'A'+(int)(ChangeToFloatNonPcnt((double)((i-1)), (double)(TestSocket.iShtCol))),
            'a'+((i-1)%TestSocket.iShtCol),      //  ← 第二字元用外層 i，不是內層 j
            sTemp1);
```

兩個缺陷，實測 `iShtRow` 在全樹只跟 0/1/2 比、`iShtCol` 最大 8：

| `iShtRow` | 舊行為 | 新行為 |
|---|---|---|
| **1** | `for(i=1; i<1)` → **零次迴圈，整段 Arm 良率完全沒有出現在 log 裡** | 印出 `Aa..` 共 `iShtCol` 格 |
| **2** | 只跑 `i==1`：印 `iShtCol` 格、**標籤全部是 `"Aa"`**、而且值其實是 row 1（該叫 B 列）的資料 | 印 2×`iShtCol` 格，標籤 `Aa..Ad / Ba..Bd`（2x4 例）各自正確 |

**最終態**＝`i` 由 1 改 0、標籤改成 `'A'+i` / `'a'+j`。
直接取列欄索引就不需要除法，因此 20260515 那次 `ChangeToFloatNonPcnt`+`(int)` 的補丁被一併移除
（語意等同 `cContactCT.cpp:202` 的 flat index／欄數寫法）。

**這是對外格式變更**（消費者是客戶端 RMS，路徑預設 `D:\RMS`，見 `cConfiguration.cpp:4007`）：
欄位數變、既有 `"Aa"` 欄位的語意也變。§5.2／§6.2 有完整記錄。

---

## 2. 逐 op 明細（`docs/mg_w13_ops.json`，12 op）

> 「錨點命中」＝該字串在 V910 該檔**全檔**的位元組級命中次數（實測）。
> `splice.py` 同檔由**下往上**套用。`replace` 的被取代行全部逐位元組驗證過。

### 2.1 `CosFunction.cpp`（2 op，4508 → **4510**，+2）

| op | 矩陣行 | V899 範圍 | V910 動作 | 錨點（命中） | 淨 |
|---|---|---|---|---|---:|
| **W13-01** | 1426 | `1426-1426` | `after 1467` | `CosFunction.bAutoTeachOutShuttleLantch ... =true;`（該字串 `=false` 版在 `4478` 另有一處，含 `=true;` 的形式）→ **1** | +1 |
| **W13-02** | 3881 | `3881-3881` | `after 3942` | `CosFunction.bPowerSaveShowCaption ... =false;`（`=true;` 版在 `1515`）→ **1** | +1 |

### 2.2 `CosFunction.h`（1 op，503 → **504**，+1）

| op | 矩陣行 | V899 範圍 | V910 動作 | 錨點（命中） | 淨 |
|---|---|---|---|---|---:|
| **W13-03** | 477 | `477-477` | `after 493` | `bool bPowerSaveShowCaption;` → **1** | +1 |

### 2.3 `ProductionInfo\ProductionInfo.h`（1 op，595 → **596**，+1）

| op | 矩陣行 | V899 範圍 | V910 動作 | 錨點（命中） | 淨 |
|---|---|---|---|---|---:|
| **W13-04** | 238 | `238-238` | `after 237` | `bool CheckMOInformation();` → **1** | +1 |

> 套用後該檔與 V899 的 `ProductionInfo.h` **位元組完全相同**（已實測）。

### 2.4 `ProductionInfo\ProductionInfo.cpp`（4 op，6078 → **6137**，+59）

| op | 矩陣行 | V899 範圍 | V910 動作 | 錨點（命中） | 淨 |
|---|---|---|---|---|---:|
| **W13-05** | 339 | `339-339` | `before 343` | `SaveInfoFileWhenStart();//Jimmychiu 20211109` → **1** | +1 |
| **W13-06** | 1965, 1983 | `1965-1986` | `replace [1937,1939]` | **跨三行**：`for(int i=0; i<iPassFailCount; i++)` ＋ `{` ＋ `if(BinSelect[iTestRunMode].iStackDefFailCate[i]!=iLoadMO_PassFail[i])` → **1** | +19 |
| **W13-07** | 2052, 2063, 2067, 2074, 2083 | `2052-2090` | `before 2005` | `bool TfProductionInfo::SetMOInformation()` → **1** | +39 |
| **W13-08** | 5596 | `5596-5596` | `replace [5525,5525]` | `for(int i=0;i<iBinSelectLength;i++)` → **1** | 0 |

> **W13-06 的錨點為什麼要三行**：單行 `for(int i=0; i<iPassFailCount; i++)` 在本檔命中 **2** 次
> （另一處在 `SetMOInformation()` 的 `:2101`，該處是同形迴圈）。加到第三行後才唯一——
> 因為 `:2103` 是**賦值** `iStackDefFailCate[i]=iLoadMO_PassFail[i];`，本處是**比較** `!=`。
> 尾行 `if(BinSelect...)` 純粹為取錨而重插，位元組與 V899 `:1986` 相同，
> difflib 會把它判為 equal → **不會出現在 port_check 的 added/removed 裡**。

### 2.5 `cBinSel.cpp`（2 op，6666 → **6688**，+22）

| op | 矩陣行 | V899 範圍 | V910 動作 | 錨點（命中） | 淨 |
|---|---|---|---|---|---:|
| **W13-09** | 1371 | `1370-1383` | `after 1387` | `            ReadFunctionData(tag);`（含 12 空白縮排）→ **1** | +14 |
| **W13-10** | 1395 | `1395-1402` | `before 1399` | `BinSelect[tag].iStackDefFailCate[iTo3Unload[i]]=MyBinPanel[tag]->iT6IsFail[i];` → **1** | +8 |

> 兩個 op 的區段都刻意含前／後空行，套用後的版面與 V899 `1369-1404` **位元組完全相同**（已實測）。

### 2.6 `main.cpp`（2 op，35350 → **35350**，±0）

| op | 矩陣行 | V899 範圍 | V910 動作 | 錨點（命中） | 淨 |
|---|---|---|---|---|---:|
| **W13-11** | 30011 | `30011-30011` | `replace [31371,31371]` | `for(int i=1; i<TestSocket.iShtRow; i++)`（其餘 8 處同名迴圈都是 `i=0`）→ **1** | 0 |
| **W13-12** | 30033 | `30033-30033` | `replace [31393,31393]` | `'a'+((i-1)%TestSocket.iShtCol), sTemp1);` → **1** | 0 |

> 兩處都在 `TfMain::TimerESDTimer()` 內（V910 `:31162` 起、V899 `:29806` 起，同一支函式）。
> 除了這兩行，`V899[30005-30037]` 與 `V910[31365-31397]` **逐行位元組相同**（已實測）。

---

## 3. 事前模擬與實編（記憶體內套用；輸出只寫進 scratchpad，V910 樹未動）

### 3.1 套用結果

| 檔 | 套用前 | 套用後 | Δ | CRLF | bare-LF | cp950 |
|---|---:|---:|---:|---:|---:|---|
| `CosFunction.cpp` | 4508 | **4510** | +2 | 4510 | **0** | OK |
| `CosFunction.h` | 503 | **504** | +1 | 504 | **0** | OK |
| `ProductionInfo\ProductionInfo.h` | 595 | **596** | +1 | 596 | **0** | OK |
| `ProductionInfo\ProductionInfo.cpp` | 6078 | **6137** | +59 | 6137 | **0** | OK |
| `cBinSel.cpp` | 6666 | **6688** | +22 | 6688 | **0** | OK |
| `main.cpp` | 35350 | **35350** | ±0 | 35350 | **0** | OK |

**12 個 op 的錨點檢查全部 `ANCHOR_OK`（FAIL COUNT = 0）**；每檔行數增減與 op 算術逐檔對帳相符。

### 3.2 簽章對帳

模擬結果中 `//AI(ht9045-v899) 20260819` ＝ **10**、`20260820` ＝ **7**，合計 **17**
＝ 矩陣 17 條，一條不多一條不少：

| 檔 | 0819 | 0820 |
|---|---:|---:|
| `CosFunction.cpp` | 2 | 0 |
| `CosFunction.h` | 1 | 0 |
| `ProductionInfo\ProductionInfo.h` | 1 | 0 |
| `ProductionInfo\ProductionInfo.cpp` | 6 | 3 |
| `cBinSel.cpp` | 0 | 2 |
| `main.cpp` | 0 | 2 |

### 3.3 忠實度驗證

- **每個 op 的 payload 在結果中恰好出現 1 次**（12/12）。
- **payload ±6 行的上下文窗：12/12 與 V899 逐位元組相同**（無任何「行尾註解對齊」型差異——
  本波六個檔在目標區沒有 W12 遇到的第 80 欄 vs 第 120 欄漂移）：

  | op | 落點 sim | 對應 V899 | ±6 窗 |
  |---|---|---|---|
  | W13-01 | 1468 | 1426 | IDENTICAL |
  | W13-02 | 3944 | 3881 | IDENTICAL |
  | W13-03 | 494 | 477 | IDENTICAL |
  | W13-04 | 238 | 238 | IDENTICAL |
  | W13-05 | 343 | 339 | IDENTICAL |
  | W13-06 | 1938–1959 | 1965–1986 | IDENTICAL |
  | W13-07 | 2025–2063 | 2052–2090 | IDENTICAL |
  | W13-08 | 5584 | 5596 | IDENTICAL |
  | W13-09 | 1388–1401 | 1370–1383 | IDENTICAL |
  | W13-10 | 1413–1420 | 1395–1402 | IDENTICAL |
  | W13-11 | 31371 | 30011 | IDENTICAL |
  | W13-12 | 31393 | 30033 | IDENTICAL |

- **`ProductionInfo\ProductionInfo.h` 套用後與 V899 整檔位元組相同。**
- **V910 原有內容零遺失**：逐行集合比對，6 個檔只有 3 行消失，**全部是刻意取代**：

  | 檔 | 消失的行 | 為什麼 |
  |---|---|---|
  | `ProductionInfo.cpp` | `    for(int i=0;i<iBinSelectLength;i++)` | 換成夾了上界的版本 |
  | `main.cpp` | `for(int i=1; i<TestSocket.iShtRow; i++)  //記錄Arm良率` | 換成 `i=0` 版 |
  | `main.cpp` | `sTemp.sprintf("%c%c:%s;", 'A'+(int)(ChangeToFloatNonPcnt(...)), 'a'+((i-1)%...), sTemp1);  //Steven 20260421` | 換成 `'A'+i, 'a'+j` 版（含 V910 自有註解，見 §5.3 R1） |

### 3.4 結構健全性（**要先 cp950 解碼再數**，W12 教訓）

解碼後 `{`−`}` 與 `(`−`)` 的差值，**6 個檔套用前後完全相同（delta 全為 0）**。
（`cBinSel.cpp` 的 `()` 基準差是 −6、`main.cpp` 是 `{}`+5／`()`−13——那是註解與字串裡的括號，
波前波後都一樣，不是不平衡。）

### 3.5 port_check 預測（模擬檔跑同一套分類邏輯）

```
[CosFunction.cpp]                   added=2   (spliced=2,   authored=0) removed=0 | EOL (4508,0)->(4510,0)
[CosFunction.h]                     added=1   (spliced=1,   authored=0) removed=0 | EOL (503,0)->(504,0)
[ProductionInfo\ProductionInfo.h]   added=1   (spliced=1,   authored=0) removed=0 | EOL (595,0)->(596,0)
[ProductionInfo\ProductionInfo.cpp] added=60  (spliced=60,  authored=0) removed=1 | EOL (6078,0)->(6137,0)
    REMOVED   5525:     for(int i=0;i<iBinSelectLength;i++)
[cBinSel.cpp]                       added=22  (spliced=22,  authored=0) removed=0 | EOL (6666,0)->(6688,0)
[main.cpp]                          added=2   (spliced=2,   authored=0) removed=2 | EOL (35350,0)->(35350,0)
    REMOVED  31371:             for(int i=1; i<TestSocket.iShtRow; i++)   //記錄Arm良率
    REMOVED  31393:                     sTemp.sprintf("%c%c:%s;", 'A'+(int)(ChangeToFloatNonPcnt(...
TOTAL added=88 spliced=88 authored=0 removed=3
```

**預期 `authored=0`、`comment=0`、`removed=3`、EOL 全 CRLF 無混用。**

### 3.6 **套用後 bcc32 實編（本波已做，不是預測）**

方法：整棵 V910 複製到 scratchpad（2,208 檔）→ 覆蓋 6 個模擬結果 → 在副本上以
`bcc_syntax.sh` 的同一組旗標與 `-I` 清單跑 `bcc32 -c -H- -w- -D_VER6`（`.obj` 進 scratchpad）。
**兩棵正式樹全程未被寫入**（`git status` 對帳為空），探針副本已刪除。

| 檔 | 波前基準（磁碟現況） | 套用後 | 判定 |
|---|---|---|---|
| `CosFunction.cpp` | 0 errors | **0 errors** | PASS |
| `ProductionInfo\ProductionInfo.cpp` | 0 errors | **0 errors** | PASS |
| `cBinSel.cpp` | 0 errors | **0 errors** | PASS |
| `main.cpp` | 3 errors（`E2034`/`E2342` Timer7Timer HWND、`E2096` edSetupFileNameKeyUp） | **同樣 3 個、集合完全相同** | PASS（no-regression） |

> 這一步實編掉了 §0.3 所有「符號可見性／`#include` 缺漏／多載匹配」的殘餘風險，
> 特別是 `ApplyHotAirByMOTemperatureMode()` 用到的 `fTemp_Set` / `fLotInfo` / `INSTALL_HEAT_GUN` /
> `Temperature.*` 全部在 `ProductionInfo.cpp` 現有 include 清單下解析成功。
>
> 中途踩過一個坑，記錄備查：**不能只把改過的檔放到 scratchpad、用 `-I` 把改過的標頭排前面來編。**
> BCB6 的 `#include "x.h"` 先找**包含者所在目錄**再走 `-I`，所以樹裡的舊標頭會贏過 staging 的新標頭
> （實際症狀＝`'bHotAirByMOTemperatureMode' is not a member of 'HT9045_COUSTOMER_FUNCTION'`），
> 而且 `ProductionInfo\` 底下的檔換了目錄後，同名標頭的解析結果也會跟著換。
> **要編就整棵複製，不要只搬檔。**

---

## 4. 錨點強度說明

12 個錨點**全部全檔唯一（count=1）**。其中兩個需要特別說明：

| op | 為什麼不是最直覺的那個錨 |
|---|---|
| **W13-06** | 直覺錨 `for(int i=0; i<iPassFailCount; i++)` 命中 **2**（`SetMOInformation():2101` 同形）→ 改用涵蓋整個取代區的三行跨行字串；第三行的 `!=` 比較 vs `:2103` 的 `=` 賦值是唯一性的來源 |
| **W13-09** | 用 `            ReadFunctionData(tag);`（含 12 空白縮排）而不是裸字串；裸 `ReadFunctionData` 在本檔另有定義行與宣告行，加了縮排後 count=1 |

其餘 10 個都是直覺錨即唯一，且都選在**最能表達本 op 意圖**的那一行
（例 W13-08 直接錨在要被改的迴圈行、W13-12 錨在要被移除的 `(i-1)%iShtCol` 上）。

---

## 5. 行為影響（給 LEDGER）

### 5.1 目標客戶

| # | 客戶 | 變化 | 來源 op | 影響 |
|---|---|---|---|---|
| A | 超豐 `CC_Greatek` | OEE Start Lot 換完工作檔後，依 MO `Temperature mode` 自動開／關熱風槍並同步 `fTemp_Set->cbHeaterGun` | W13-01..05、07 | 消除「換工單漏勾 Active Heater Gun」；MO 欄位為空時**保持原行為**（保留工作檔設定），舊工單不受影響。每次執行寫 1 行 FTP SetupFile change log |
| B | 力成 `CC_PTI` | 未指派 Category 的 Fix1／Fix4 在 Bin 畫面由綠（Pass）變紅（Fail） | W13-09、10 | 消除操作員誤判；連帶讓 GPIB `AlarmSetup` 的 Fix 盤上下一致 |
| C | 力成 `CC_PTI` | `CheckMOInformation` 的 PASSBIN 比對跳過 `e3Fix1` | W13-06 | 讓 B 不會反過來變成 `Check PASSBIN Error` 擋住 Start（B 與 C 必須同波，缺一即壞） |
| D | 超豐（N14_21 路徑） | `SettingBinCategoryFromServer` 不再越界讀 24 格堆疊垃圾 | W13-08 | 消除潛在 AV／堆積破壞；`iStackDefFailCate[9..32]` 由「被寫垃圾」變成「保留原值」 |

### 5.2 其他機台／其他客戶

| 面向 | 影響 |
|---|---|
| 非 `CC_Greatek` | `bHotAirByMOTemperatureMode` 恆 false → `ApplyHotAirByMOTemperatureMode()` 第一行 return。**指令級零變更** |
| 非 `CC_PTI` | `bNoCategoryToFix1` / `bSkipFix1PassFail` 恆 false → 兩個新 `if` 主體永不執行。**指令級零變更** |
| 無熱風槍機構（`INSTALL_HEAT_GUN<=0`） | 第二道 return，即使是 Greatek 也不動作 |
| N14_21 越界修正 | 執行期到達路徑被 `bOEEFunction`(Greatek only) → `bN14_21_SetUpConfiguration`(非 Greatek 恆 0) → `sHANDLER_TYPE=="HT9046"` 三層收斂；且新上界只會更小 → **不可能造成回歸** |
| **O06 週期 Production log（本波唯一外溢）** | **無客戶碼閘**。任何開了 `config.ini [Event Log] EnanleTimePeriodSaveLog=1` 的機台，輸出格式都會變：`iShtRow==1` 的機台由「完全沒有 Arm 良率欄位」變成有 `iShtCol` 格；`iShtRow==2` 的機台由「`iShtCol` 格且標籤全是 `Aa`、值其實是 B 列」變成「2×`iShtCol` 格、標籤各自正確」。**消費者是客戶端 RMS（預設路徑 `D:\RMS`）** → §6.2 |
| 檔案層新增 | 無新增全域變數、無新增建構子、無新增 `#include`。`CosFunction.h` 加 1 個 `bool`、`ProductionInfo.h` 加 1 個方法宣告 → **全量 build 時會廣泛重編**（波內只用 `bcc32 -c` 隔離編譯） |

### 5.3 反向發現（V910 有、V899 沒有）——**不可回搬**

| # | 位置 | V899 | V910 | 判定 |
|---|---|---|---|---|
| **R1** | `main.cpp` O06 標籤行 | 0820 終態：`'A'+i, 'a'+j`（除法整段移除） | `//Steven 20260421 : cast to int for %c` ＋ `(int)(ChangeToFloatNonPcnt(...))` | **兩樹各自獨立修過同一個 `%c` 轉型問題**（V899 是 `//AI 20260515`，V910 是 Steven 20260421）。V899 的 0820 修正把除法整段拿掉，Steven 那句註解所描述的程式碼已不存在 → **隨行取代是正確的，不是遺失**。副作用：矩陣裡原本的 20260515 那條 main.cpp 標記在 V899 端也已被 0820 覆蓋，所以矩陣沒有孤兒 |
| **R2** | `ProductionInfo.cpp` `CheckMOInformation()` `1874-1929`、`403-404`、`1683-1685` | 有 `sTempErrorDetail` 溫度錯誤明細（帶實際數值）＋ 幾行 `delete slOEEReport;` 洩漏修補 | 無 | **經 `bb69c60` 反查，這些在 V899 pristine 就存在** ＝ fork 期分歧，不在 `bb69c60..b515ed5` 範圍、無 `//AI` 標記、不是矩陣點 → **不屬本戰役工作量，本波不碰**。（若日後要收，屬新開的「fork 期差異回收」議題，需另立範圍） |
| **R3** | `CosFunction.h:486-487` | 對應處是 `bUseInArmLoadStageWatchdog` | `bZHomingAfterPickErr` / `bInitialStartDelayCount_Init` | V910 自有成員，在 PowerSave 群之前；本波新宣告插在群之後 → 不受影響 |

---

## 6. <95% 清單、外溢與刻意排除

### 6.1 建議寫入 `MG_FINAL_DECISIONS.md` 的項目：**無**

12 個 op 全部 ≥95%：錨點全檔唯一、payload 全部位元組取自 V899、V910 無反向刻意修改與之衝突、
無邏輯衝突需要改寫，**而且已經實編通過**。**本波不出 F 列。**

（本文件全程唯讀，未動 `MG_FINAL_DECISIONS.md`。）

### 6.2 必須記入 LEDGER 的外溢（**不是阻塞項，但要留痕**）

1. **O06 週期 Production log 是對外格式變更，且無客戶碼閘。**
   消費者是客戶端 RMS（`IniConfig.asProductionAutoSavePath` 預設 `D:\RMS`）。
   `iShtRow==1` 的機台由「零欄位」變「`iShtCol` 欄位」、`iShtRow==2` 由「`iShtCol` 欄位、
   標籤全 `Aa`、值是 B 列」變「2×`iShtCol` 欄位、標籤正確」。
   忠實搬運 V899 出貨終態；**若有客戶的 RMS parser 依欄位數或 `Aa` 標籤解析，需要事先告知**。
   記憶已有相關條目（「O06週期log Arm良率欄位」）——本波把 V910 從「其餘 16 棵樹」那一側移走。
2. **`CosFunction.h` 與 `ProductionInfo.h` 各加一個宣告**，兩者被廣泛 include
   → **全量 build 時大量 `.cpp` 會重編**（Phase 3 要留時間）。
3. **W13-08 是 B 類記憶體安全修正**，程式碼層面無閘、全客戶編譯進去；
   執行期由 Greatek-only 的 N14_21 路徑收斂（§⚠3）。LEDGER 要把「無閘」與「到不了」分開寫，
   免得日後被讀成「這波改了全客戶行為」。
4. **W13-09/10 與 W13-06 是一組，必須同波落地。** 只搬前者會讓 PTI 在 Start 時
   撞 `Check PASSBIN Error`；只搬後者則畫面顏色沒修好。
5. **熱風槍會在 OP 主動存工作檔時被持久化**：`uTemp_Set.cpp:4675 SaveSetupFile()`
   把 `cbHeaterGun->Checked` 寫進工作檔 `[Mode] Active_Heat_Gun`。
   `ApplyHotAirByMOTemperatureMode()` 本身不回寫，但它改過的勾選狀態會被後續存檔帶走。
   V899 出貨版即如此，記錄備查。
6. **案件歸屬**：0819 全部 = 超豐 `CASE-20260819-001`；
   0820 的 `ProductionInfo.cpp:1965/1983` ＋ `cBinSel.cpp:1371/1395` = PTI Fix1 Pass/Fail；
   `ProductionInfo.cpp:5596` = 越界修正；`main.cpp:30011/30033` = O06 log 修正。
   **後三者是同日不同缺陷，LEDGER 分開寫。**

### 6.3 刻意排除：`cTemperFrom.cpp`（`b515ed5` 的第 9 個檔）

| 項目 | 內容 |
|---|---|
| 變更 | `TfTemperFrom::Button6Click()`：`ShowErrorMessage("JAM0416", ...)` → `ShowErrorMessage("WAR07301", ...)` |
| V910 現況 | `cTemperFrom.cpp:2028` 仍是 `JAM0416` |
| 為什麼不搬 | (1) 無 `//AI` 標記 → 不是矩陣點，搬了也沒有驗收簽章；(2) `cTemperFrom.dfm:3274` 的 `object Button6` 是 `Caption = 'AlarmSimulator'` 且 **`Visible = False`**，正式機按不到；(3) `WAR07301` 是 SmartAutoClean CTF 連續 Fail、`JAM0416` 是 Shuttle JAM，與 0819 熱風槍主題無關，是開發者除錯用的暫存值；(4) 無 case 歸屬、無理由說明 |
| 信心 | **99%（排除）**。建議 LEDGER 留一行「已查證、刻意不搬」，避免日後重新發現時再花一輪 |

---

## 7. 收工 gate 建議（給主腦）

1. **硬前提（套用前先驗，任一不符就停）**——見 §0.1 md5 表，重點三個：

   | 檔 | 必須是 | md5(12) |
   |---|---:|---|
   | `main.cpp` | **35350** 行 | `833b10b84827` |
   | `CosFunction.cpp` | **4508** 行 | `5456c2d54479` |
   | `ProductionInfo\ProductionInfo.cpp` | **6078** 行 | `9da6852ffeb6` |

   （其餘 3 檔：`CosFunction.h` 503 / `3e7384835f33`、`ProductionInfo\ProductionInfo.h` 595 /
   `157b95f5bb38`、`cBinSel.cpp` 6666 / `d5c61fd21890`。）

2. `python tools/port_tools/splice.py docs/mg_w13_ops.json`
   - 預期輸出（6 檔）：
     ```
     OK CosFunction.cpp: 2 op(s) applied, eol=CRLF
     OK CosFunction.h: 1 op(s) applied, eol=CRLF
     OK ProductionInfo\ProductionInfo.h: 1 op(s) applied, eol=CRLF
     OK ProductionInfo\ProductionInfo.cpp: 4 op(s) applied, eol=CRLF
     OK cBinSel.cpp: 2 op(s) applied, eol=CRLF
     OK main.cpp: 2 op(s) applied, eol=CRLF
     ```
   - 套用後行數：**4510 / 504 / 596 / 6137 / 6688 / 35350**
   - 產生 6 個 `.mgbak`（目前樹上 `.mgbak` 為 0 個）

3. `python tools/port_tools/port_check.py`（不給參數即可，V910 樹只會有這 6 個 M 檔）
   - 預期與 §3.5 的表**逐檔相同**：`added` 全數 SPLICED、`authored=0`、`comment=0`、
     `removed` 合計 **3**、EOL 全 CRLF 無 bare-LF。
   - **REMOVED 3 行的逐條清單見 §3.3**，主腦只要對照那張表即可。

4. `sh tools/port_tools/bcc_syntax.sh 'CosFunction.cpp' 'ProductionInfo\ProductionInfo.cpp' 'cBinSel.cpp' 'main.cpp'`
   - **本波已在整棵樹複本上實編過**（§3.6），預期：前三支 `PASS (0 errors)`、
     `main.cpp` `PASS (no-regression: 3 個既有錯誤與基準一致)`。
   - 若實際結果與此不同 → 表示套用結果與模擬不一致，**先停下來對帳，不要繼續**。

5. 主腦逐條開檔複驗，**重點五處**：
   - `ProductionInfo\ProductionInfo.h` 套用後應與 V899 該檔**位元組完全相同**
     （`md5` 應為 `95b04e0a2548`、596 行）——本波最容易驗的一支。
   - `ProductionInfo.cpp` 套用後 **2025-2063 行必須與 V899 的 2052-2090 完全相同**
     （`ApplyHotAirByMOTemperatureMode()` 整支），且其後 2064 行是 `//------` 分隔線、
     2065 行才是 `bool TfProductionInfo::SetMOInformation()`。
   - `ProductionInfo.cpp` 套用後 **1938-1959 必須與 V899 1965-1986 完全相同**，
     且 `CheckMOInformation()` 內只有一處 `bSkipFix1PassFail`，
     `SetMOInformation()`（原 `:2101` 那支同形迴圈，套用後往下位移）**沒有被誤動**。
   - `cBinSel.cpp` 套用後 **1388-1401 與 1413-1420** 分別對應 V899 `1370-1383` 與 `1395-1402`，
     且新的 `if(bNoCategoryToFix1...)` 在既有
     `BinSelect[tag].iStackDefFailCate[iTo3Unload[i]]=...` 那一行**之前**。
   - `main.cpp` 套用後 **31371 與 31393** 兩行是新版，且 31393 行**不再含 `ChangeToFloatNonPcnt`**；
     其餘 31365-31397 一行未動。

6. 收工後 `python tools/port_tools/ai_comment_matrix.py` 重算
   → 本波 17 條**全部**轉 PRESENT（矩陣以「`//AI(` 到行尾」的位元組簽章比對，我們是原樣插入）。
   - **只驗「淨 MISSING 減 17、且這 17 條正是本波的」**，不要拿總數當基準。
   - 本波 17 條的檔／行清單：

     | 檔 | 行 |
     |---|---|
     | `CosFunction.cpp` | 1426, 3881 |
     | `CosFunction.h` | 477 |
     | `ProductionInfo\ProductionInfo.h` | 238 |
     | `ProductionInfo\ProductionInfo.cpp` | 339, 1965, 1983, 2052, 2063, 2067, 2074, 2083, 5596 |
     | `cBinSel.cpp` | 1371, 1395 |
     | `main.cpp` | 30011, 30033 |
   - 預期儀表：**淨 MISSING 104 → 87**（實測現況：`mg_ai_matrix_missing.csv` 104 列、
     與 `mg_matrix_allowlist.csv` 50 列**零重疊** → 該檔已是白名單淨值；
     W11 後的 137 扣掉 W12 的 33 ＝ 104，對得上）。

7. **LEDGER 記錄要點**
   - `docs/MG_PORT_LEDGER.md` 第 96、97 行（20260819 / 20260820 兩列）由 `pending` 改
     **done（10/10）／done（7/7）**，波次填 `MG-W13`。
   - 類別：**A 類（客戶碼閘功能）×2 ＋ B 類（記憶體安全）×1 ＋ 對外格式 ×1**，
     忠實搬運，錨點乾淨，**<95% 項目 0 個**，已實編通過。
   - **gate 形狀**：0819＝`CosFunction.bHotAirByMOTemperatureMode`（原生 Greatek 閘，非新增）；
     0820 Fix1＝`CUSTOMER_CODE==CC_PTI`（原生閘，非新增）；
     0820 越界＝無閘（執行期由 Greatek-only 的 N14_21 路徑收斂）；
     0820 O06 log＝**無閘，對外格式變更**。
     **本波沒有新增任何客戶碼閘。**
   - 外溢六項見 §6.2（特別是第 1 項 O06 對外格式）。
   - 反向發現 R1／R2／R3 見 §5.3（R2 的 fork 期分歧值得單獨開一張追蹤卡）。
   - **刻意排除 `cTemperFrom.cpp` 一行**（§6.3），請在 LEDGER 留一行以免日後重查。
   - 教訓一則可入 KNOWLEDGE：**BCB6 的 `#include "x.h"` 先找包含者所在目錄再走 `-I`；
     要在樹外驗證改動，必須整棵複製，不能只搬改過的檔加 `-I` 覆蓋**（§3.6）。

8. **commit 後清 V910 樹 `*.mgbak`**（6 個），讓下一波的差異報告基準回到波前狀態。
