# MG-W6 分析：20260817 CC_CYUEAN 固定關閉 Auto Tray Feed

- **波次**：MG-W6（V899 → V910）
- **來源**：`docs/mg_ai_matrix_missing.csv` 5 條 MISSING（CosFunction.cpp × 2、CosFunction.h × 1、cTrayAssignment.cpp × 2）
- **V899 樹**：`D:\HT9045\HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422`（唯讀；本波四個來源檔工作區對 HEAD 無異動，且 `git diff b515ed5 HEAD` 對這些檔為空 → 工作區＝在製收斂端點）
- **V910 樹**：`D:\HT9045\HT9011UC_Code_V3.33.910.0_20260716_Jimmy`
  （分析開始時 `git status --porcelain` 空＝乾淨基線；文件完成時 MG-W5 已在並行修改 `HS_Function.cpp`，
  **與本波三個目標檔零重疊**，本文件所有行號在最終複驗時重新對過磁碟，全部仍成立）
- **本文件性質**：分析＋splice op 草案。**分析階段全程唯讀，未修改任何原始碼。**
- **op 檔**：`D:\HT9045\docs\mg_w6_ops.json`
- **類別**：**B 類（客戶碼隔離）**，且 gate 是 V899 原生的（`FUNC_CC_CYUEAN` 函式內），照原 gate 搬，未另加。

---

## 0. 前置事實（全部實測，非推論）

### 0.1 20260817 叢集的歸屬切分

`grep "^20260817," docs/mg_ai_matrix_all.csv` 共 **11 條**：

| 檔 | 條數 | 狀態 | 歸屬 |
|---|---|---|---|
| `cObserver.cpp` | 5 | `same-file`（已搬） | MG-W3（EventLog 引號相依鏈）**已完成** |
| `CosFunction.cpp` | 2 | MISSING | **本波 MG-W6** |
| `CosFunction.h` | 1 | MISSING | **本波 MG-W6** |
| `cTrayAssignment.cpp` | 2 | MISSING | **本波 MG-W6** |
| `PowerSavingMode.cpp` | 1 | MISSING | **不屬本波** — 見下 |

`PowerSavingMode.cpp:363` 的 AI 註解實體是
`//AI(ht9045-v899) 20260817: guard 只擋「重複點亮」,還原一律要能執行。倒數字串是`（後接 3 行續行註解），
落在 `TPowerSaving::ShowPowerSaveHighlight(bool bOn)`，屬 **CASE-PTI-20260811-001 Power Saving 高亮還原** 主題，
與 Auto Tray Feed 無任何符號或控制流交集。**建議另開波次（PowerSaving/PTI 0811 叢集）處理，本波不碰。**

本波 5 條矩陣點：

| # | 檔 | 行 | 註解 |
|---|---|---|---|
| 1 | `CosFunction.h` | 354 | `CC_CYUEAN 固定關閉 Auto Tray Feed(勾選時會架空 Clean Out 的 Tray End 選單)` |
| 2 | `CosFunction.cpp` | 2847 | `客戶反映 Auto Tray Feed 跟隨工作檔, 換工單就把 Clean Out 的 Tray End 選單架空, 故固定關閉` |
| 3 | `CosFunction.cpp` | 4322 | `預設不關閉, 只有 CC_CYUEAN 會開啟` |
| 4 | `cTrayAssignment.cpp` | 237 | `CC_CYUEAN 固定關閉 Auto Tray Feed, 不受工作檔 Tray.Data 與 hanaART 影響` |
| 5 | `cTrayAssignment.cpp` | 727 | `CC_CYUEAN 固定關閉 Auto Tray Feed, 取消勾選並鎖定(只鎖這一顆, 同群組其他兩項不動)` |

### 0.2 檔案基本盤

| 檔 | V899 行數 | V910 行數 | EOL | 套用後預測行數 |
|---|---|---|---|---|
| `CosFunction.cpp` | 4430 | 4489 | 兩樹**全 CRLF** | **4493** |
| `CosFunction.h` | 487 | 495 | 兩樹**全 CRLF** | **496** |
| `cTrayAssignment.cpp` | 1649（末行無 EOL） | 1765 | 兩樹**全 CRLF** | **1776** |

V910 三檔目前 `bDisableAutoTrayFeed` 出現次數皆為 **0** → 5 條 MISSING 判定成立，無「已搬一半」狀態。

### 0.3 20260817 變更的真實範圍（用祖先樹反推，非猜測）

`FUNC_CC_CYUEAN()` 函式本體長度（含 `void`／`{`／`}`）：

| 樹 | 函式起訖 | 行數 | 含 `bCleanOutCanTrayEnd` | 含註解掉的 `bUnloadTrayModeByRecipe` |
|---|---|---|---|---|
| `V3.33.896.0_..._Jimmy_20260310_AI` | 2762–2787 | 26 | 否 | **是**（變更前基準） |
| `V3.33.896.0_..._JerryYang3_SPIL` | 2761–2786 | 26 | 否 | 是 |
| `V3.33.899.0_..._Jimmy_20260422` | 2822–2850 | **29** | **是** | 否（**搬移來源**） |
| `V3.33.903.0_20260417_Jimmy_20260428` | 2811–2836 | 26 | 否 | 是 |
| `V3.33.905.8_20260608_Ken` | 2822–2847 | 26 | 否 | 是 |
| `V3.33.906.0_20260618` | 2836–2861 | 26 | 否 | 是 |
| `V3.33.910.0_20260716_Jimmy` | 2844–2869 | 26 | 否 | 是（**搬移目標**） |

26 → 29 的帳：**刪 1 行**（註解掉的死敘述 `//    CosFunction.bUnloadTrayModeByRecipe   =true;`）＋**加 4 行**，
`26 − 1 + 4 = 29` 完全對帳。→ 5 條矩陣紀錄 = **3 個 CosFunction 變更點 + 2 個 cTrayAssignment 變更點**，無漏、無多。

> **⚠️ 沒有 AI 標記但必搬的一行**：`V899 CosFunction.cpp:2846`
> `IniConfig.bCleanOutCanTrayEnd  =true;  //Steven 20140426 : 客戶要求Clean Out後要按Initial Start`
> 這一行**掛的是 2014 年的舊註解**，所以矩陣不會列它；但 896/903/905.8/906/910 **五棵樹的 `FUNC_CC_CYUEAN` 全都沒有它**，
> 它就是 20260817 這次新加的。下一行 `if(IniConfig.bCleanOutCanTrayEnd==true)` 直接依賴它——
> **只搬 2847–2849 而漏掉 2846，guard 會讀到 `InitialCosFunction` 的預設 `false`，整個搬移變成 no-op。**
> 同 MG-W4 B1 的形狀（「含未標記 AI 的 [0] Loader 行」）。已納入 OP-3。

### 0.4 位元組級證明：V910 的取代／插入區段 == 變更前原狀

| 區段 | V899（未變更鄰行） | V910 | 位元組相等 |
|---|---|---|---|
| `CosFunction.h` 宣告前一行 | 353 | 356 | **True** |
| `CosFunction.h` 宣告後一行 | 355 | 357 | **True** |
| `CosFunction.cpp` 預設值前一行 | 4321 | 4373 | **True** |
| `CosFunction.cpp` 預設值後一行 | 4323 | 4374 | **True** |
| `FUNC_CC_CYUEAN` 函式頭＋前 22 條敘述 | 2822–2845 | 2844–2867 | **True** |
| `cTrayAssignment` hanaART 讀檔區塊 | 233–235 | 264–266 | **True** |
| `cTrayAssignment` hanaART UI 區塊 | 723–725 | 812–814 | **True** |
| `cTrayAssignment` UI 後續（CC_SCC 鏈頭） | 733–735 | 815–817 | **True** |
| **896 vs 910 `FUNC_CC_CYUEAN` 整支** | 896:2762–2787 | 910:2844–2869 | **True** |

→ **V910 在這些位置上完全是變更前的原狀，公司從未反向修改過。**
→ 「V910 既有行為被覆蓋」的風險在本波為 **零**（不是「低」，是實測為零）。

唯一非位元組相等的鄰行是 `cTrayAssignment.cpp` V899:241 vs V910:268
（`TrayForm.bColorTray = ReadIniData(...)`）——差別只是**行尾註解的欄位對齊**（註解由第 ~140 欄推到 ~160 欄），
程式敘述完全相同，且**不在任何插入／取代區段內**。

### 0.5 跨檔依賴稽核

- `bDisableAutoTrayFeed` 在**整棵 V899 樹**（排除 `.svn`）只有 **5 處**：
  `CosFunction.h:354`（宣告）／`CosFunction.cpp:2849`（CYUEAN 設 true）／`CosFunction.cpp:4322`（預設 false）／
  `cTrayAssignment.cpp:238`／`cTrayAssignment.cpp:728`（兩個生效點）。
  **`csystem.cpp` 完全沒有引用它** → 本波不需要動 `csystem.cpp`（見 §0.5.2 的機制說明）。
- 客戶碼兩樹同值：

| 符號 | V899 | V910 | 比對 |
|---|---|---|---|
| `CC_CYUEAN` | `MachineType.h:265` → **868** | `MachineType.h:228` → **868** | **同值** |
| `CC_KYEC_LEE` | **921** | **921** | 同值（供 §0.6 互斥性證明用） |

- 新程式用到的每個符號在 V910 都存在且同語意：

| 符號 | V910 出處 | 比對 |
|---|---|---|
| `CosFunction`（結構實例） | `cTrayAssignment.cpp:258` 已在用 `CosFunction.bUnloadTrayModeByRecipe` | 標頭透過既有 include 鏈可見，無須新增 `#include` |
| `IniConfig.bCleanOutCanTrayEnd` | `Config.h` 宣告；`CosFunction.cpp` 內已有 **39** 處寫入（V899 為 **40** 處，**差的那 1 處正是本波 OP-3 要補的 V899:2846**——獨立佐證 §0.3 的反推） | 兩樹同名同型 |
| `TrayForm.bAutoFeed` | `cTrayAssignment.cpp:260/266/363/509/1381`、`cConfiguration.cpp:4385` 已在用 | 兩樹同名同型 |
| `chkAutoTrayFeed` | `cTrayAssignment.h:67` `TCheckBox *chkAutoTrayFeed;`（與 V899 **同行同宣告**） | 同 |
| `fMain->hanaART->IsHanaArtAvailable()` | `cTrayAssignment.cpp:265/813` 已在用 | 同 |

#### 0.5.1 執行順序（決定 default→customer 覆寫是否成立）

`InitialCosFunction()` 內先把**全部**旗標寫成預設值，函式**最後一行**才呼叫 `DoCustomerFunction()`：

- V910：`CosFunction.cpp:4475` `DoCustomerFunction();  //Steven 20240927 : 拉出並保持在最下方`
- 唯一外部呼叫點：`cprod.cpp:3730` `InitialCosFunction();`（`DoCustomerFunction()` 在全樹沒有其他呼叫點）

→ `bDisableAutoTrayFeed=false`（預設，OP-2）→ `FUNC_CC_CYUEAN()` 內設 `true`（OP-3）。順序正確，兩樹一致。

#### 0.5.2 生效機制（為何不必動 csystem.cpp）

修正是**只改 `TrayForm.bAutoFeed` 這一個既有旗標**，不新增消費端：

- `TrayForm.bAutoFeed` 在 `csystem.cpp` 的引用點：**V899 18 處 / V910 18 處，1:1 對應**。
- 關鍵閘門 `csystem.cpp` V899:14284 ↔ V910:15296
  `if((TrayForm.bAutoFeed && LastSet.bCleanOut_ART==false && ...)` — 這段 **81 行（V899 14280–14360 vs V910 15292–15372）
  除了 3 行行尾註解欄位對齊外位元組完全相同**。
- `IniConfig.bCleanOutCanTrayEnd` 的讀取點：V899 / V910 各 **7 處，全在 `csystem.cpp`**
  （`DoTrayFeed` 1、`DoTrayFeedProcess` 3、`DoOneCycleFinishCheck` 1、`DoCleanOutFinishCheck` 2），
  含 `MES1642` 的 `K_TRAY_FEED|K_TRAY_END` 三選一選單。**寫入點全樹只有 `CosFunction.cpp`**
  （`grep` 全樹：`CosFunction.cpp` 以外 0 個賦值），故 §3 的行為變更範圍是封閉可證的。

### 0.6 V910 的公司漂移（本波唯一需要逐段驗證的項目）

`cTrayAssignment.cpp` `TfTrayAssignment::ReadFile()` 內，V910 多了一段 V899 沒有的
**Eastsun 20260512 F011 Phase 6（KYEC AMR）**：

```
910 cTrayAssignment.cpp:349:     else if(CUSTOMER_CODE==CC_KYEC_LEE)
...
910 cTrayAssignment.cpp:363:         TrayForm.bAutoFeed=true;      //Eastsun 20260512 F011 Phase 6 (a-side L194 KYEC AMR Auto Feed)
910 cTrayAssignment.cpp:364:         chkAutoTrayFeed->Enabled=false;
```

- 它與 OP-4 插入點（V910:266）**在同一支函式內、且位置在其後**，因此形式上「可能覆寫」我們設的 `bAutoFeed=false`。
- **實測互斥**：它整段包在 `else if(CUSTOMER_CODE==CC_KYEC_LEE)` 內，`CC_KYEC_LEE=921`、`CC_CYUEAN=868`，
  兩者不可能同時成立 → **CYUEAN 路徑永遠走不到 363**，反向 KYEC 路徑上 `bDisableAutoTrayFeed` 恆為 `false`，也不受 OP-4 影響。
- 這是 V899 沒有、V910 有的**新增**，本波**完全不動它**。

其餘漂移（`cTrayAssignment.cpp:268`、`csystem.cpp` 三行）皆為行尾註解欄位對齊，見 §0.4。

### 0.7 事前模擬（記憶體內套用，未落盤）

依 `mg_w6_ops.json` 全部 5 個 op 模擬套用後：

| 檔 | 行數 | CRLF | cp950 解碼錯誤 |
|---|---|---|---|
| `CosFunction.cpp` | 4489 → **4493** | 4493/4493 | **0** |
| `CosFunction.h` | 495 → **496** | 496/496 | **0** |
| `cTrayAssignment.cpp` | 1765 → **1776** | 1776/1776 | **0** |

`difflib` 對「原 V910 → 模擬結果」的完整 opcode：**只有 4 個 hunk，一個 `replace`＋三個 `insert`，
移除行總數 1（就是那行註解掉的死敘述），無任何其他位元組變動。**

**套用後五個區段對 V899 的位元組比對（決定性驗收）**：

| 區段 | 模擬結果行段 | V899 行段 | 位元組相等 |
|---|---|---|---|
| `CosFunction.h` 新宣告 | 357 | 354 | **True** |
| `FUNC_CC_CYUEAN` 整支 | 2844–2872 | 2822–2850 | **True** |
| `CosFunction.cpp` 預設值三行 | 4376–4378 | 4321–4323 | **True** |
| `cTrayAssignment` ReadFile 區塊 | 264–271 | 233–240 | **True** |
| `cTrayAssignment` FormShow 區塊 | 816–827 | 723–734 | **True** |

→ 五個搬移點套用後**與 V899 出貨版位元組完全相同**，含縮排、行尾註解欄位與 Big5 中文。

---

## 1. 各變更明細

### C1 — `CosFunction.h` 旗標宣告（矩陣 `CosFunction.h:354`）→ **OP-1**

- **V899 範圍**：`CosFunction.h:354`（1 行，`bool bDisableAutoTrayFeed;` ＋ AI 註解）
- **V910 錨點**：`CosFunction.h:356` = `    bool bUnloadTrayModeByRecipe;  //Steven 20220710 : ...`
  → 全檔命中 **[356] 唯一**，`anchor_expect` 取純 ASCII 前綴 `bool bUnloadTrayModeByRecipe;`
- **模式**：`after`（插在 356 之後、`bFullTrayAlarmAfterUnloadEnd`（V910:357）之前，＝V899 的原位）
- **語意**：`TCosFunction` 結構新增一個 `bool` public 成員。
- **風險**：**結構中段加欄位** → 其後所有成員的 offset 位移。已稽核（V910 全樹、排除 `.svn`）：
  - `sizeof(CosFunction)` / `sizeof(TCosFunction)` / `memcpy(&CosFunction` / `fread`/`fwrite` 帶 `CosFunction` 的路徑：**0 筆**
  - `&CosFunction.<member>`（把成員位址交給 `elConfig->Add` 之類的表格）：**0 筆**
    （對照：`IniConfig` 與 `TrayForm` 有大量此類綁定，`CosFunction` 沒有）
  → 該結構純粹由程式碼寫死（`InitialCosFunction()` + `FUNC_CC_*`），**沒有任何以固定 offset 或整塊二進位存取它的地方**，
  加欄位不會靜默讀錯成員。唯一要求是**觸及此標頭的所有 .cpp 一起重編**（BCB6 全量 build 時自然滿足），
  且 V899 已如此出貨。
- **信心**：**≥95%**（V910 該處與 896 原狀位元組相同；錨點唯一）

---

### C2 — `InitialCosFunction()` 預設值（矩陣 `CosFunction.cpp:4322`）→ **OP-2**

- **V899 範圍**：`CosFunction.cpp:4322`（1 行，`=false;` ＋ AI 註解「預設不關閉, 只有 CC_CYUEAN 會開啟」）
- **V910 錨點**：`CosFunction.cpp:4373` = `    CosFunction.bUnloadTrayModeByRecipe    =false; //Steven 20220710 : ...`
  → `anchor_expect` 取到 `=false;` 為止的完整純 ASCII 前綴（含中間空白），**全檔唯一**
- **模式**：`after`（插在 4373 之後、`bFullTrayAlarmAfterUnloadEnd`（V910:4374）之前，＝V899 的原位）
- **語意**：所有客戶的預設值＝`false`＝**V910 現行行為**。
- **不動**：`InitialCosFunction()` 其他 700+ 行預設值、結尾的 `DoCustomerFunction();`（V910:4475）皆原樣保留。
- **信心**：**≥95%**

---

### C3 — `FUNC_CC_CYUEAN()` 開旗標（矩陣 `CosFunction.cpp:2847`）→ **OP-3**

- **V899 範圍**：`CosFunction.cpp:2846-2849`（4 行）

  ```
  2846     IniConfig.bCleanOutCanTrayEnd    =true;  //Steven 20140426 : 客戶要求Clean Out後要按Initial Start
  2847     //AI(ht9045-v899) 20260817: 客戶反映 Auto Tray Feed 跟隨工作檔, 換工單就把 Clean Out 的 Tray End 選單架空, 故固定關閉
  2848     if(IniConfig.bCleanOutCanTrayEnd==true)
  2849         CosFunction.bDisableAutoTrayFeed    =true;
  ```

- **V910 被取代行段**：`CosFunction.cpp:2868-2868`（1 行）
  = `//    CosFunction.bUnloadTrayModeByRecipe                                         =true;`
- **模式**：`replace`（1 行 → 4 行，淨 +3）
- **anchor_expect**：`//    CosFunction.bUnloadTrayModeByRecipe` → 取代區段內命中，且全檔只有 2868 這一行是註解掉的版本
- **為何用 `replace` 而不是 `after`**：V899 這次變更把這行**註解掉的死敘述刪掉**了。用 `replace`
  可讓 V910 的 `FUNC_CC_CYUEAN` 與 V899 **位元組完全相同**（§0.7 已驗），
  是本波最強的驗收證據；被刪的只是一行**已註解、不產生任何指令的死碼**，且位於 CYUEAN 專屬函式內，
  對其他客戶零影響。若 主腦 偏好「只加不刪」，改成 `mode:"after", anchor_line:2868` 也可，
  代價是失去位元組相等這條 gate，且函式會長成 30 行（與 V899 不一致）。
- **語意**：
  1. `IniConfig.bCleanOutCanTrayEnd=true` — 打開 Clean Out 之後的 **Tray End 選單**（`MES1642`）。**這是 CYUEAN 的新行為，見 §3。**
  2. `if(...==true)` guard 在此函式內恆為真（上一行剛設 true），語意上是**記錄依賴關係**，
     不是條件分支。忠實照搬，不「修得更好」。
- **信心**：**≥95%**

---

### C4 — `TfTrayAssignment::ReadFile()` 生效點（矩陣 `cTrayAssignment.cpp:237`）→ **OP-4**

- **V899 範圍**：`cTrayAssignment.cpp:236-239`（4 行：1 空行 + AI 註解 + `if` + `TrayForm.bAutoFeed=false;`）
- **V910 錨點**：`cTrayAssignment.cpp:266` = `        TrayForm.bAutoFeed          =true;`（hanaART 區塊的 body）
  → `anchor_expect` = `TrayForm.bAutoFeed          =true;`，全檔命中 **[266] 唯一**
  （同檔 363 行是 `TrayForm.bAutoFeed=true;`，無空白，字面不同）
- **模式**：`after`（插在 266 之後、原 267 空行之前，＝V899 的原位；含空行以維持原排版）
- **函式歸屬確認**：V899:237 與 V910:266 都在 `void __fastcall TfTrayAssignment::ReadFile()`
  （V899 起於 129 行、V910 起於 153 行）。
- **語意**：本行以下順序＝
  `工作檔 Tray.Data 的 "Auto Feed"`（V910:260，受 `bUnloadTrayModeByRecipe` 閘門）
  → `hanaART 強制 true`（V910:266）
  → **CYUEAN 強制 false（新插入）**。**最後寫的贏**，故「不受工作檔與 hanaART 影響」成立。
- **不動**：`if(CosFunction.bUnloadTrayModeByRecipe==false)` 區塊（258–263）、hanaART 區塊（265–266）、
  以及 §0.6 的 Eastsun KYEC AMR 區塊（349–366）全部原樣保留。
- **風險**：見 §0.6——KYEC AMR 那段在插入點之後，但客戶碼互斥，實測不可能同時成立。
- **信心**：**≥95%**

---

### C5 — `TfTrayAssignment::FormShow()` UI 鎖定（矩陣 `cTrayAssignment.cpp:727`）→ **OP-5**

- **V899 範圍**：`cTrayAssignment.cpp:726-732`（7 行：1 空行 + AI 註解 + `if` + `{` + `Checked=false` + `Enabled=false` + `}`）
- **V910 錨點**：`cTrayAssignment.cpp:814` = `        chkAutoTrayFeed->Enabled        =false;`（hanaART UI 區塊 body）
  → `anchor_expect` = `chkAutoTrayFeed->Enabled        =false;`，全檔命中 **[814] 唯一**
  （同檔 364 行是 `chkAutoTrayFeed->Enabled=false;`，無空白，字面不同）
- **模式**：`after`（插在 814 之後、原 815 空行之前，＝V899 的原位）
- **函式歸屬確認**：V899:727 與 V910:814 都在 `void __fastcall TfTrayAssignment::FormShow(TObject *Sender)`
  （V899 起於 626 行、V910 起於 698 行）。
- **語意**：只鎖 `chkAutoTrayFeed` **一顆** checkbox（取消勾選 + 禁用），
  **同群組其他兩項（`bFailAutoTrayManual_FT` / `_RT` 對應控制項）完全不動** —— 與矩陣註解字面一致。
- **與 UI 其他寫入點的相容性**：`TfTrayAssignment` 這一顆 `chkAutoTrayFeed`（`cTrayAssignment.h:67`）
  在 V910 只有 4 個存取點：`364`（KYEC AMR，§0.6 已證互斥）／`509`／`813–814`／`1381`，
  **沒有任何一處寫 `Enabled=true`**。
  `509` 是 `DoIniDataToForm()` 的 `chkAutoTrayFeed->Checked = TrayForm.bAutoFeed;`——
  CYUEAN 時 `bAutoFeed` 已被 OP-4 設為 `false`，故不論呼叫順序 `Checked` 都是 `false`，**兩條路徑一致不衝突**。
  `1381` 是存檔 `WriteIniData(..., chkAutoTrayFeed->Checked)`，寫回 `false`，與 OP-4 的執行期值一致。
- **⚠ 同名不同物（非本波缺口，但 主腦 需知悉）**：另有**第二顆** `chkAutoTrayFeed`，
  屬 `TfConfiguration`（`cConfiguration.h:1230`），在 `cConfiguration.cpp:4385` 被綁到同一個變數：
  `elConfig->Add(chkAutoTrayFeed, &TrayForm.bAutoFeed, ECBool, "Flag", "Auto Feed", bShow, bEnable, bReadFromFile, 0);`
  → Configuration 畫面上仍可勾選並寫回 `TrayForm.bAutoFeed`，繞過本波的鎖定。
  **這一行在兩樹是位元組完全相同的**（V899:4219 ↔ V910:4385），且 **V899 出貨版同樣沒有鎖它** ——
  也就是說這是 V899 修正本身的既有缺口，不是搬移遺漏。忠實搬運即等同 V899；
  若日後要一併鎖住 Configuration 那顆，屬 V899 本身的議題，應回 `ht9045-v899` 決定，不在本波處理。
- **不動**：hanaART 區塊（813–814）、其後的 `CUSTOMER_CODE==CC_SCC || CC_SCK` 權限鏈（816 起）全部原樣保留。
- **信心**：**≥95%**

---

## 2. 客戶碼隔離與「非 CYUEAN 路徑零影響」的證明

**本波是 B 類，但 gate 完全沿用 V899 原生的 `FUNC_CC_CYUEAN` 分支，未新增任何 `CUSTOMER_CODE` 判斷。** 逐項證明：

1. **唯一開關點**：`bDisableAutoTrayFeed` 全樹只有一處被設為 `true`，就在 `FUNC_CC_CYUEAN()` 內（OP-3）。
   `FUNC_CC_CYUEAN()` 全樹唯一呼叫點是 `DoCustomerFunction()` 的 `case CC_CYUEAN:`（V910:3852）。
   → 非 CYUEAN 機台，`bDisableAutoTrayFeed` 恆為 `InitialCosFunction()` 寫的 `false`（OP-2）。
2. **兩個生效點都是無 `else` 的單向 guard**：
   `if(CosFunction.bDisableAutoTrayFeed)` 為 `false` 時，兩處各自**執行零條敘述**，
   控制流、回傳值、後續敘述全部不變。→ 非 CYUEAN 的執行路徑**指令級不變**。
3. **唯一被移除的 V910 內容**是一行**註解掉的死敘述**（OP-3），且位於 CYUEAN 專屬函式內 → 對任何客戶皆無指令變化。
4. **結構加欄位（OP-1）的外溢**：`TCosFunction` 多一個 `bool`。全樹沒有把該結構整塊序列化到檔案／
   透過 SECS 傳輸／以固定 offset 存取的路徑，旗標一律由程式碼寫死。唯一要求是**觸及該標頭的 .cpp 全部重編**。
5. **模擬 diff 佐證**：`difflib` 對三個檔的完整 opcode 只有 4 個 hunk，全部落在上述五個點內，無旁生變動（§0.7）。

**結論：非 CC_CYUEAN 客戶路徑位元組不變（除了結構多一個未被讀到的 bool 成員）。**

---

## 3. CYUEAN 端的真實行為變更（**必須記入 LEDGER**）

搬移後 CYUEAN 機台會有 **兩項** 行為變化，而不是一項：

| # | 變化 | 來源 | 影響面 |
|---|---|---|---|
| A | `TrayForm.bAutoFeed` 恆為 `false`（Tray Assignment 的 Auto Tray Feed 取消勾選並鎖定） | OP-3 → OP-4 / OP-5 | Clean Out 完成後不再自動 Tray Feed，改跳 `MES1642` 三選一（`K_TRAY_FEED` / `K_TRAY_END` / `K_RETRY`），Tray End 選單不再被架空 |
| B | **`IniConfig.bCleanOutCanTrayEnd` 由 `false` 變 `true`** | OP-3 第一行 | 打開 `csystem.cpp` 的 **7 個讀取點**：`DoTrayFeed`（1）／`DoTrayFeedProcess`（3）／`DoOneCycleFinishCheck`（1）／`DoCleanOutFinishCheck`（2） |

**B 是這次搬移中唯一「V910 從未有過」的新開關**（896 / 903 / 905.8 / 906 / 910 五棵樹的 `FUNC_CC_CYUEAN` 全都沒有它）。
它是本修正的必要組成——沒有它，Tray End 選單根本不存在，「AutoTrayFeed 架空 Tray End 選單」這個客訴無從修起——
且**已隨 V899 出貨到 CYUEAN 機台**，屬既有現況同步，非本波新發明。

**不阻塞，但 主腦 需知悉**：B 讓 CYUEAN 在 Clean Out / One Cycle 完成後多出「Tray End」這個操作選項，
並讓 `bCleanOutTrayEnd` 狀態旗標對 CYUEAN 首次變成可被設為 `true`（`csystem.cpp` V910:15381）。
`bCleanOutCanTrayEnd` 的**寫入點全樹只有 `CosFunction.cpp`**（其他檔 0 個賦值），故此變更範圍封閉可證，
不會外溢到其他客戶。

---

## 4. 交付與收工 gate 建議（給 主腦）

1. `python tools/port_tools/splice.py docs/mg_w6_ops.json`
   - 預期輸出：
     `OK CosFunction.h: 1 op(s) applied, eol=CRLF`
     `OK CosFunction.cpp: 2 op(s) applied, eol=CRLF`
     `OK cTrayAssignment.cpp: 2 op(s) applied, eol=CRLF`
   - 三檔各自產生 `.mgbak`
   - 套用後行數應為 **4493 / 496 / 1776**（§0.7）
2. `python tools/port_tools/port_check.py CosFunction.cpp CosFunction.h cTrayAssignment.cpp`
   - 預期：`added` 全數歸為 **SPLICED**、`authored=0`、`comment=0`
   - EOL：`(4489,0)->(4493,0)`、`(495,0)->(496,0)`、`(1765,0)->(1776,0)`
   - `REMOVED` 應**只有 1 行**：`//    CosFunction.bUnloadTrayModeByRecipe    =true;`（§0.3 已證為死碼）
3. `bcc32 -c -H- CosFunction.cpp` / `cTrayAssignment.cpp`（V899 根目錄 include 設定；`.h` 由這兩支帶入）
   - `CosFunction.h` 是**廣泛被 include 的定義檔**，加欄位後全量 build 時所有相依 .cpp 都會重編；
     波內只用 `-c` 隔離編譯即可，全量 build 留給 Phase 3。
4. 主腦逐條開檔複驗：本文件 §1 的行號皆為 20260826 實測，套用前請以 `anchor_expect` 自行再確認一次。
   **特別複驗 OP-3 是否確實把 `IniConfig.bCleanOutCanTrayEnd=true`（V899:2846）一起帶進去**——
   漏這行整波變 no-op（§0.3 警告框）。
5. 收工後 `python tools/port_tools/ai_comment_matrix.py`，MISSING 應**減少 5 條**。
   （20260826 本文件量測當下：`mg_ai_matrix_missing.csv` 以 `csv.reader` 計為 **335** 條 MISSING
   ——注意 `wc -l` 會得到 346，因為含換行的多行註解在 CSV 是被引號包住的單一 row，**以 `csv.reader` 為準**；
   且 MG-W5 正在並行推進，收工時的基數請以當下重跑結果為準，只驗「減 5 條、且這 5 條正是本波的」。）
6. LEDGER 需記：§3 的 A、B 兩項行為變更，特別是 **B（CYUEAN 首次開 `bCleanOutCanTrayEnd`）**。

---

## 5. 結論

- **5 個變更點（5 條矩陣紀錄）→ 5 個 splice op**（1 `replace`、4 `after`），錨點全為**全檔唯一的純 ASCII 字串**。
- **無 <95% 項目。** 五個位置在 V910 都與變更前基準（896 樹）位元組相同，公司從未反向修改；
  唯一的公司漂移（Eastsun 20260512 KYEC AMR）被 `CUSTOMER_CODE==CC_KYEC_LEE`（921）鎖住，
  與 `CC_CYUEAN`（868）互斥，實測不可能同時成立。
- **`CC_CYUEAN` 兩樹同值 868**（V899 `MachineType.h:265` / V910 `MachineType.h:228`），
  `CC_KYEC_LEE` 兩樹同值 921。
- 記憶體模擬套用後，五個搬移點與 V899 **位元組完全相同**，`difflib` 全檔只有 4 個 hunk、移除 1 行死註解，
  三檔 cp950 解碼零錯誤、全 CRLF。
- 本波**不需要新增客戶碼隔離**（gate 是 V899 原生的 `FUNC_CC_CYUEAN`）；
  非 CYUEAN 路徑指令級不變，唯一外溢是 `TCosFunction` 多一個 bool 成員（需全量重編）。
- **不屬本波**：`PowerSavingMode.cpp:363`（20260817 但是 CASE-PTI-20260811-001 Power Saving 高亮還原主題），
  以及已在 MG-W3 完成的 `cObserver.cpp` 5 條。
