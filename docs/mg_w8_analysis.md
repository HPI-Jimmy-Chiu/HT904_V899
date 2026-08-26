# MG-W8 分析：C05 Power Saving 主題線（0804 profile → 0811 UI 高亮 → 0817 還原 guard）

- **波次**：MG-W8（V899 → V910）
- **相依鏈**：LEDGER #5。三個主題**必須同波**，因為 0811 的高亮完全靠 0804 新增的
  `CosFunction.bPowerSaveShowCaption` 當閘門，而 0817 的 guard 是 0811 那支函式的一部分（同一段位元組）。
- **來源**：`docs/mg_ai_matrix_missing.csv` **35 條 MISSING**（0804 × 25、0811 × 9、0817 × 1）
- **CASE**：`CASE-PTI-20260804-001`（C05 per-customer profile）、`CASE-PTI-20260811-001`（主畫面高亮＋倒數字串凍結）
- **V899 樹**：`D:\HT9045\HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422`（唯讀；本波五個來源檔
  `git status --porcelain` 空，且 `git diff b515ed5 HEAD` 對這五檔為空 → 工作區＝在製收斂端點 `b515ed5`）
- **V910 樹**：`D:\HT9045\HT9011UC_Code_V3.33.910.0_20260716_Jimmy`
  （MG-W6（commit `26ec286`）已落地並**已含在下列所有 V910 行號中**。
  分析期間主迴圈並行推進 **MG-W7**，修改 `ContactForce.cpp` 與 `iosetview.cpp` ——
  **與本波五個目標檔零重疊**；本文件所有 V910 行號在收工前已對當下磁碟重新驗證一次，16 個 op 全部仍成立，
  五個目標檔的 mtime 也證實未被本波或 MG-W7 觸碰。）
- **本文件性質**：分析＋splice op 草案。**分析階段全程唯讀，未修改任何原始碼。**
- **op 檔**：`D:\HT9045\docs\mg_w8_ops.json`（**16 個 op**）
- **類別**：**B 類（客戶碼隔離）**，且 gate 是 V899 原生的（`FUNC_CC_PTI()` ＋五個 `CosFunction.bPowerSave*` 旗標），
  **未新增任何 `CUSTOMER_CODE` 判斷**。

---

## 0. 前置事實（全部實測，非推論）

### 0.1 三個日期叢集的歸屬切分

| 日期 | 檔 | 條數 | 歸屬 |
|---|---|---:|---|
| 20260804 | `CosFunction.cpp` | 9 | **本波** |
| 20260804 | `CosFunction.h` | 6 | **本波** |
| 20260804 | `PowerSavingMode.cpp` | 5 | **本波** |
| 20260804 | `PowerSavingMode.h` | 1 | **本波** |
| 20260804 | `cConfiguration.cpp` | 4 | **本波** |
| 20260811 | `PowerSavingMode.cpp` | 7 | **本波** |
| 20260811 | `PowerSavingMode.h` | 2 | **本波** |
| 20260811 | `aoutarm9045.cpp` | 7 | ❌ OutArm rotate 主題 → **留 W9+** |
| 20260811 | `RotateKit\aRotateKIT_Out.cpp` | 4 | ❌ OutArm rotate 主題 → **留 W9+** |
| 20260811 | `aoutarm9045.h` | 1 | ❌ OutArm rotate 主題 → **留 W9+** |
| 20260817 | `PowerSavingMode.cpp:363` | 1 | **本波**（W6 已鑑定歸此主題） |

**本波 35 條，留給別波 12 條。** 兩邊**零符號交集**：全 V899 樹（700 檔，排除 `.svn`）掃描
本波 12 個新符號，`aoutarm9045.*` 與 `aRotateKIT_Out.cpp` 命中數皆為 **0**。

> **0817 那條就是 commit `7de2301` 的內容。** `git show 7de2301 --stat` 只動兩個檔：
> `HT9045.bpr`（版號 899.37）＋`PowerSavingMode.cpp`（+12/−1）。它是 899.36 高亮工作（`83fb4f6`）的回歸修正，
> `//AI` 日期落在 20260817，**已包含在矩陣裡**，不需另外撈。`7de2301` 是 `b515ed5` 的祖先（實測 `git merge-base --is-ancestor`）。

### 0.2 `.dfm` 有無變更 —— **明確答案：本波是純程式碼，不做任何 .dfm op**

三項獨立實測：

1. **V899 端**：`git diff --stat bb69c60..b515ed5 -- '<V899>/*.dfm'` 共 4 檔
   （`cConfiguration.dfm` 6+/6−、`cSortCT.dfm`、`cTrayMapping.dfm`、`iosetview.dfm`）。
   `cConfiguration.dfm` 的 6 行差異**全部是 BCB6 表單設計器狀態**：
   `ActivePage = tsI00 → tsN00`、`TabIndex = 7 → 10`、`ActivePage = tsN12 → tsN10`、`TabIndex = 7 → 5`、
   `ActivePage = tsN10_1_10 → tsN10_11_20`、`TabIndex = 0 → 1`。
   **沒有任何控制項新增、刪除或屬性變更**，與 C05 無關（存檔時停在哪個分頁而已）。
2. **V910 端控制項存在性**：本波新程式用到的 `gbC05` / `rgC05` / `cbC05_Motor|Temp|Vacuum|ATC` /
   `edC05_Motor|Temp|Vacuum|ATC` / `labC05_2|4|6|8` **全部存在於 V910 的 `cConfiguration.dfm`**
   （`labC05_2/4/6/8` 分別在 2728 / 2735 / 2742 / 2749 行，`object ...: TLabel`），
   標頭宣告在 `cConfiguration.h:1596-1603`，**與 V899 同行同宣告**。
   > ⚠️ 這一項是本波最關鍵的單點驗證：`labC05_2/4/6/8` 在 **V910 全樹 `.cpp` 的引用數是 0**
   > （只有標頭那一行宣告）。若它們只有宣告而 `.dfm` 沒有對應物件，VCL streaming 後指標為 NULL，
   > `labC05_4->Caption=...` 會直接空指標當掉。**實測 `.dfm` 有，不是問題。**
3. **`gbC05` 整個 TGroupBox 區塊位元組比對**：V899 `cConfiguration.dfm[2587-2680]` vs
   V910 `[2668-2761]`，**94 行位元組完全相同**。

**結論：`.dfm` 在 V899 這次確有 diff，但是設計器分頁狀態噪音，與 C05／高亮主題零關係；
V910 的 C05 控制項與 `main.dfm` 的 `pnlPowerSaving` 都已就位。本波 16 個 op 全部是 `.cpp`／`.h`，
不含任何 `.dfm` 變更。**

### 0.3 `main.dfm` 的還原常數對得上（否則「還原」會還錯）

0811 高亮把面板改成 `Height=28 / Font.Height=-20 / Color=clYellow / Font.Color=clRed`，
還原時寫回硬編碼的 `PSNormalPanelHeight=20` / `PSNormalFontHeight=-16` / `clBtnFace` / `clBlue` / `"PowerSaving"`。
**這組常數必須等於 V910 `main.dfm` 的原始值，否則還原會把面板還成錯的樣子。**

實測 V910 `main.dfm:4403-4418` 的 `object pnlPowerSaving: TPanel` 區塊與 V899 `main.dfm:4369-4384`
**位元組完全相同**：`Height = 20`、`Font.Height = -16`、`Font.Color = clBlue`、`Caption = 'PowerSaving'`。
→ **常數正確，還原不會失真。**

### 0.4 檔案基本盤與 V910 公司漂移地圖

| 檔 | V899 | V910 | EOL | 套用後 | 對 V899 pristine 的公司漂移 |
|---|---:|---:|---|---:|---|
| `PowerSavingMode.h` | 72 | 68 | 兩樹**全 CRLF** | **72** | **0 個 hunk（正規化 EOL 後位元組完全相同）** |
| `PowerSavingMode.cpp` | 535 | 437 | 兩樹**全 CRLF** | **535** | **2 個 hunk，全是空行**（見下） |
| `CosFunction.h` | 487 | 496 | 兩樹**全 CRLF** | **502** | 9 個 hunk（含 MG-W6 的 1 行） |
| `CosFunction.cpp` | 4430 | 4493 | 兩樹**全 CRLF** | **4506** | 大量（含 MG-W6 的 5 行） |
| `cConfiguration.cpp` | 7573 | 7840 | 兩樹**全 CRLF** | **7867** | 108 個 hunk（本波唯一需要逐段驗的檔） |

**`PowerSavingMode.cpp` 的公司漂移只有兩個空行**（決定性事實，本波信心的主要來源）：

- V910 在 `OnScanTmr` case 2 的 `}` 與 `if(tModule->iCountDown>OverDayAM)` 之間**多一個空行**（V910:140）
- V910 把空的解構子 `__fastcall TModule::~TModule() { }` 裡的**空行拿掉**（V899 pristine:339）

兩者互相抵銷，行數同為 437。**沒有任何敘述層變更 —— 公司在 899→910 期間從未動過省電模組的邏輯。**

### 0.5 跨檔依賴稽核

#### 0.5.1 本波 12 個新符號在 V899 全樹的**每一個**出現點（700 檔，排除 `.svn`）

| 符號 | V899 全樹命中 | 是否**全部**落在本波 op 範圍內 |
|---|---|---|
| `iPowerSaveMaxMinute` | 5（CosFunction.cpp 1471/3876、CosFunction.h 472、PSM.cpp 47/48） | **是** |
| `bPowerSaveTempOnly` | 4（cConfiguration.cpp 1013、CosFunction.cpp 1472/3877、CosFunction.h 473） | **是** |
| `bPowerSaveLotEndOnly` | 4（CosFunction.cpp 1473/3878、CosFunction.h 474、PSM.cpp 139） | **是** |
| `bPowerSaveSkipAmbient` | 4（CosFunction.cpp 1474/3879、CosFunction.h 475、PSM.cpp 145） | **是** |
| `bPowerSaveShowCaption` | 4（CosFunction.cpp 1475/3880、CosFunction.h 476、PSM.cpp 360） | **是** |
| `GetPowerSaveMaxMinute` | 7（cConfiguration.cpp 38/1024/1028、PSM.cpp 43/289/290、PSM.h 69） | **是** |
| `ShowPowerSaveHighlight` | 7（PSM.cpp 106/118/258/259/358/408、PSM.h 46） | **是** |
| `bHighlightOn` | 7（PSM.cpp 66/364/365/369/372/376、PSM.h 45） | **是** |
| `PSNormalPanelHeight` / `PSNormalFontHeight` / `PSHighlightPanelHeight` / `PSHighlightFontHeight` | 各 2（PSM.cpp 34-37 定義、383/384/391/392 使用） | **是** |

→ **本波只需動 5 個檔，沒有第六個檔。定義與消費點 100% 覆蓋，無遺漏。**

#### 0.5.2 V910 端符號存在性（712 檔掃描，排除 `.svn`）

- 12 個新符號在 V910 命中數**皆為 0** → 35 條 MISSING 判定成立，**無「已搬一半」狀態**。
- 新程式用到的既有符號全部存在且可見。以 `#include` 遞移閉包實測
  （`PowerSavingMode.cpp` 閉包 93 個標頭、`cConfiguration.cpp` 閉包 125 個標頭）：

| 符號 | V910 宣告處 | 在閉包內 |
|---|---|---|
| `CosFunction` | `CosFunction.h:488` `extern HT9045_COUSTOMER_FUNCTION CosFunction;` | **是** |
| `RunInfo` | `cprod.h:2820` `extern RUN_INFO RunInfo;` | **是** |
| `LastSet` | `LastSet.h:514` `extern LAST_GENERAL_SET LastSet;` | **是** |
| `Tempture_Ambient` | `cmydef.h:2783` `extern const int Tempture_Ambient;` | **是** |
| `fMain->pnlPowerSaving` | `main.h:812` `TPanel *pnlPowerSaving;` | **是** |
| `fMain->btnView` | `main.h:805` `TSpeedButton *btnView;` | **是** |
| `fMain->ShowFunctions()` | `main.h:1689` `int __fastcall ShowFunctions();`（**回傳 int**） | **是** |
| `elConfig->Add(...,1,false,1,<max>)` 9 引數多載 | V910 `cConfiguration.cpp:1097-1100` 已在用 | **是** |

- **`PowerSavingMode.cpp` 的 `#include` 清單兩樹前 31 行位元組完全相同** → 同一條閉包，
  V899 能編出來 V910 就能。
- `TfMain::ShowFunctions()` 兩樹比對：V899 8117-8220（104 行）vs V910 8771-8873（103 行），
  **唯一差異是一個空的 `if{}` 裡少一行空白**，功能相同；`iTop=ShowFunctions();` 的既有慣用法
  兩樹都在（V899 main.cpp:8070 / V910 main.cpp:8724），OP10 的寫法與之一致。

#### 0.5.3 結構加欄位（OP12）的外溢 —— **獨立重驗，不引用 MG-W6**

`HT9045_COUSTOMER_FUNCTION` 加 5 個 `bool` ＋ 1 個 `int`。V910 全樹 712 檔（排除 `.svn`）稽核：

| 樣式 | 命中 |
|---|---:|
| `sizeof(CosFunction` / `sizeof(HT9045_COUSTOMER_FUNCTION` | **0** |
| `memcpy(&CosFunction` / `memset(&CosFunction` | **0** |
| `fread(&CosFunction` / `fwrite(&CosFunction` | **0** |
| `&CosFunction.<member>`（交給 `elConfig->Add` 之類的表格綁定） | **0** |
| `(char*)&CosFunction` / `(void*)&CosFunction` | **0** |

→ 該結構純由程式碼寫死（`InitialCosFunction()` ＋ `FUNC_CC_*`），**沒有任何以固定 offset 或整塊二進位存取它的地方**，
加欄位（含改變 padding 的 `int`）不會靜默讀錯成員。唯一要求是**觸及此標頭的所有 .cpp 一起重編**
（BCB6 全量 build 自然滿足），且 V899 已如此出貨。

#### 0.5.4 執行順序

`InitialCosFunction()` 全樹唯一呼叫點 `cprod.cpp:3730`；`DoCustomerFunction()` 全樹唯一呼叫點是
`InitialCosFunction()` 的**最後一行**（V910 `CosFunction.cpp:4479`）。
→ 預設值（OP14）→ 客戶覆寫（OP13）順序正確，且只跑一次。兩樹一致。

### 0.6 V910 的公司漂移：本波唯一的真實碰撞區（`cConfiguration.cpp` [C05] 區段）

V910 的 `InitConfigEdtList_ItemC()` 被 **Eastsun 20260710 整合**改寫過：把原本 4 行
`elConfig->Add(edC05_*)` 註解掉，改成 `if(USE_BU5_Function==false){...200...} else {...400...}`。

本波的做法是**只把新分支插在前面，把 V910 既有的 `if` 降級成 `else if`，
V910 分支本體（含 Eastsun 的 200/400 分岔）一個位元組都不動**：

```
1075|     （空行，V910 原有）
1076| ┌── 被取代：if(IniConfig.bPowerSaveFunction)
     └── 換成 V899 1007-1035（新 PTI 分支 + 收尾的 else if）
...
1102|     }                                       ← 新 PTI 分支結束
1103|     else if(IniConfig.bPowerSaveFunction)   ← V910 既有分支被降級（唯一「移除」的補償）
1104|     {                                       ← 以下全是 V910 原文，含 Eastsun BU5
1110|         if(ATC_SYSTEM!=eNewATCSystem)
1118|         if(USE_BU5_Function==false)         //Eastsun 20260710整合
```

- **`USE_BU5_Function` 在 V910 是死旗標**：`cmydef.cpp:5638` `int USE_BU5_Function=0;`，
  全樹**無任何賦值、無 `&USE_BU5_Function` 綁定、無 ini key**（實測 0 命中）
  → V910 既有分支恆走 `==false`（上限 200），與 V899 變更前完全一致。
  因此「V899 的 PTI 分支沒有 200/400 分岔」**在實機上零差異**（§3 仍記 LEDGER）。

其餘 107 個漂移 hunk 皆與 [C05] 無關（ASE_CL / SPIL / heater-control / AOI 等），本波完全不碰。

### 0.7 未標記承重行（矩陣不會列，但必須一起搬）

本波 16 個 op 共搬 **170 行**，拆帳：

| 類別 | 行數 |
|---|---:|
| 矩陣列出的 `//AI` 標記行 | **35** |
| **V899 新增但無 `//AI` 標記** → 矩陣看不到，漏搬即壞 | **116** |
| V910 原本就有、僅為取得**唯一錨點**而一併重插的既有行（位元組相同，無功能變化） | **19** |

**116 行裡最危險的（只搬「有註解的行」必炸）：**

| V899 位置 | 內容 | 漏搬的後果 |
|---|---|---|
| `PowerSavingMode.h:69` | `extern int GetPowerSaveMaxMinute();` | 矩陣只列 :68（註解行）。漏掉 → `cConfiguration.cpp` **link error** |
| `CosFunction.cpp:3877-3880` | 4 行 `bPowerSave*=false;`（**純敘述、無註解**） | 矩陣只列 3875/3876。與 MG-W6 的「未標記 [0] Loader 行」同型 |
| `cConfiguration.cpp:1035` | `else if(IniConfig.bPowerSaveFunction)` | 漏掉 → 兩個平行的 `if`，V910 分支變成無條件執行，**[C05] 畫面重複註冊控制項** |
| `PowerSavingMode.cpp:290` | `Minute=GetPowerSaveMaxMinute();` | 矩陣只列 :289 |
| `PowerSavingMode.cpp:259` | `ShowPowerSaveHighlight(true);` | 矩陣只列 :256/:258（兩行都是註解） |
| `PowerSavingMode.cpp:408` | `ShowPowerSaveHighlight(false);` | 矩陣只列 :406/:407（兩行都是註解） |
| `PowerSavingMode.cpp:34-37` | 四個 `PS*` 常數 | 矩陣只列 :32 |
| `PowerSavingMode.cpp:43-54` | `GetPowerSaveMaxMinute()` 函式本體 12 行 | 矩陣只列 :39 |
| `PowerSavingMode.cpp:358-398` | `ShowPowerSaveHighlight()` 函式本體 41 行 | 矩陣只列 :354 與 :363 |

> 本波的 op 全部以**連續區段**方式抽取，上述行自然被包含 —— 這正是「用區段不用逐行」的理由。

### 0.8 事前模擬（記憶體內套用，未落盤）

依 `mg_w8_ops.json` 全部 16 個 op 模擬：

| 檔 | 行數 | CRLF | cp950 解碼 | U+FFFD |
|---|---|---|---|---|
| `PowerSavingMode.h` | 68 → **72** | 72/72 | OK | 0 |
| `PowerSavingMode.cpp` | 437 → **535** | 535/535 | OK | 0 |
| `CosFunction.h` | 496 → **502** | 502/502 | OK | 0 |
| `CosFunction.cpp` | 4493 → **4506** | 4506/4506 | OK | 0 |
| `cConfiguration.cpp` | 7840 → **7867** | 7867/7867 | OK | 0 |

**16 個 op 的錨點檢查全部 `ANCHOR_OK=True`；16 個 payload 區段在結果中各出現「恰好一次」。**

**移除行共 3 行，全部是刻意的取代**：

- `PowerSavingMode.cpp` × 2：`else if(Minute>MaxMinute)` / `Minute=MaxMinute;`（OP9 換成 per-customer 版）
- `cConfiguration.cpp` × 1：`if(IniConfig.bPowerSaveFunction)`（OP16 換成 `else if(...)` 的降級形式，**語意零損失**）

**決定性驗收（結果 vs V899 出貨版全檔比對）**：

| 檔 | 結果 vs V899 差異 hunk 數 | 說明 |
|---|---:|---|
| `PowerSavingMode.h` | **0** | **結果與 V899 出貨版位元組完全相同（72 行）** |
| `PowerSavingMode.cpp` | **2** | **僅 §0.4 那兩個既有空行**，無其他任何差異 |
| `CosFunction.h` | 9 | 全是 899→910 公司內容（含 0803/0819 屬別波的兩行） |
| `CosFunction.cpp` | 98 | 同上 |
| `cConfiguration.cpp` | 110 | 同上 |

→ 本波核心的兩個檔（省電模組本體）**套用後等於 V899 出貨版**，這是最強的一條 gate。

---

## 1. 各變更明細（依主題分段）

### 主題 A — C05 power-save per-customer profile（`CASE-PTI-20260804-001`，25 條）

#### A1 — `CosFunction.h` 五旗標＋一整數 → **OP12**

- **V899 範圍**：`CosFunction.h:471-476`（6 行：1 行主註解 ＋ `int iPowerSaveMaxMinute` ＋ 4 個 `bool`）
- **V910 錨點**：`before` 行 **487** = `} HT9045_COUSTOMER_FUNCTION;`（全檔唯一）
- **模式**：`before`
- **位置差異（刻意）**：V899 把這組放在 `bUseInArmLoadStageWatchdog`(0803) 之後、`bHotAirByMOTemperatureMode`(0819) 之前；
  V910 因公司多了 11 個成員（`bLoginASECL` … `bInitialStartDelayCount_Init`），本波放在**結構最尾端**。
  由 §0.5.3 實測，`CosFunction` 沒有任何佈局相依存取，成員順序不影響語意。
- **刻意排除**：V899:470（0803 ARDENTEC watchdog）、V899:477（0819 超豐熱風槍）—— 屬別波。
- **gate 歸屬**：無（純宣告）
- **信心**：**≥95%**

#### A2 — `InitialCosFunction()` 預設值 → **OP14**

- **V899 範圍**：`CosFunction.cpp:3875-3880`（6 行；**其中 3877-3880 無 AI 註解**，見 §0.7）
- **V910 錨點**：`after` 行 **3928** = `IniConfig.bPowerSaveFunction ... =false; //省電模式`（全檔唯一）
- **模式**：`after`
- **語意**：`iPowerSaveMaxMinute=200`（＝原本硬編碼的 `MaxMinute`）、四個 `bool=false`。
  **所有客戶的預設＝V910 現行行為。**
- **刻意排除**：V899:3881（0819 熱風槍）
- **gate 歸屬**：預設路徑（非 PTI 的行為基準）
- **信心**：**≥95%**

#### A3 — `FUNC_CC_PTI()` 開啟 profile → **OP13**

- **V899 範圍**：`CosFunction.cpp:1467-1475`（9 行 = 2 行不變的既有敘述 ＋ 7 行新增）
- **V910 被取代**：`CosFunction.cpp:1507-1508`（2 行）
- **模式**：`replace`（2 → 9，淨 +7）
- **anchor_expect**：`CosFunction.bFirstTrayCheckOnUnloader ... //Jimmychiu 20251205 : First Tray Check On Unloader`
  → 全檔唯一。（**不能用 `bEndLotAfterTrayFeed` 那行 —— 它在 1508 與 2876 各出現一次，非唯一。**）
- **為何用 `replace`**：唯一錨點需求。被取代的 2 行與 V899:1467-1468 **位元組完全相同**（實測），
  等同原地重插，無任何內容變化。
- **語意**：`IniConfig.bPowerSaveFunction=true` ＋ `iPowerSaveMaxMinute=720` ＋ 四個 `bool=true`。
- **刻意排除**：V899:1466 `bUseFix3FullTray`（20260520，V910 尚缺，屬別波）—— 區段起點刻意設在 1467。
- **gate 歸屬**：**本波的客戶碼閘門就是這裡**（`DoCustomerFunction()` 的 `case CC_PTI:`）
- **信心**：**≥95%**

#### A4 — `GetPowerSaveMaxMinute()` 與 `SetAlarmTime` → **OP3（後半）＋ OP2 ＋ OP9**

- **OP3 後半**：V899 `PowerSavingMode.cpp:38-54`（分隔線＋3 行註解＋12 行函式本體）
- **OP2**：V899 `PowerSavingMode.h:68-69`（註解＋`extern int GetPowerSaveMaxMinute();`），
  `before` 行 66 = `extern class TPowerSaving *tPowerSaving;`（唯一）
- **OP9**：V899 `PowerSavingMode.cpp:289-290` 取代 V910 `242-243`，
  anchor `else if(Minute>MaxMinute)`（唯一）
- **語意**：`iLimit = (iPowerSaveMaxMinute>0 ? iPowerSaveMaxMinute : MaxMinute)`，再 clamp 到 1439
  （因為 `SetAlarmTime` 餵給 `EncodeTime()`，hour>23 會拋例外）。
  **非 PTI：`iPowerSaveMaxMinute=200 > 0` → `iLimit=200` → 與原本的 `MaxMinute` 完全同值。**
- **名稱衝突**：`GetPowerSaveMaxMinute` 在 V910 全樹命中 0 → 無衝突。
- **gate 歸屬**：值閘門（非 PTI 值不變）
- **信心**：**≥95%**

#### A5 — 兩道 PTI 省電閘門 → **OP7**

- **V899 範圍**：`PowerSavingMode.cpp:131-149`（19 行 = 5 行不變的 `fContact` guard ＋ 14 行新增）
- **V910 被取代**：`PowerSavingMode.cpp:101-105`（5 行）
- **模式**：`replace`（5 → 19，淨 +14）；anchor `if(fContact->fShow)`（全檔唯一）
- **為何用 `replace`**：自然插入點的前一行是裸 `}`，不是可靠錨點；往前包到 `if(fContact->fShow)`
  才拿得到唯一錨。被取代的 5 行與 V899:131-135 **位元組完全相同**（實測）。
- **語意**：`bPowerSaveLotEndOnly && RunInfo.bLotStart` → 不進省電並重新計數；
  `bPowerSaveSkipAmbient && LastSet.iTemperature==Tempture_Ambient` → 同。
  兩者都是**無 `else` 的單向 guard**。
- **gate 歸屬**：`CosFunction.bPowerSaveLotEndOnly` / `bPowerSaveSkipAmbient`（預設 false）
- **信心**：**≥95%**

#### A6 — `[C05]` 設定頁 temp-only 分支 → **OP15 ＋ OP16**

- **OP15**：V899 `cConfiguration.cpp:38`（`#include "PowerSavingMode.h"`），`after` 行 37
  = `#include "HTEditList.h"`（唯一）
- **OP16**：V899 `cConfiguration.cpp:1007-1035`（29 行 = 2 行不變的既有行 ＋ 27 行新增）
  取代 V910 `1074-1076`（3 行）；anchor
  `"bC04EnableTestTempIC",             bNoShow, bEnable, bFixedValue, 0);`（全檔唯一）
- **為何取代 3 行而不是 1 行**：`if(IniConfig.bPowerSaveFunction)` 在 V910 命中 **2 次**（1076、5868），
  不是唯一錨；往前包兩行到 cbC04 那行才唯一。被取代的 1074/1075 與 V899:1007/1008 **位元組完全相同**（實測）。
- **語意**：新分支 `if(bPowerSaveFunction && bPowerSaveTempOnly)` 只開 `cbC05_Temp` 與 `edC05_Temp`
  （上限 `GetPowerSaveMaxMinute()`），其餘 motor/vacuum/ATC/mode 一律 `bNoShow/bDisable/bFixedValue`；
  `labC05_4` 顯示 `Max:%d min / Unit`，`labC05_2/6/8` 隱藏。
- **對 V910 既有內容的影響**：**零**。既有 `if` 只是降級為 `else if`，Eastsun 20260710 的
  `USE_BU5_Function` 200/400 分岔完全保留（見 §0.6）。
- **gate 歸屬**：`CosFunction.bPowerSaveTempOnly`（預設 false → 走 `else if`，＝V910 現行行為）
- **信心**：**≥95%**（本波唯一有實質公司漂移的位置，已逐行驗證）

---

### 主題 B — 主畫面高亮（`CASE-PTI-20260811-001`，9 條）

#### B1 — `PowerSavingMode.h` private 成員 → **OP1**

- **V899 範圍**：`PowerSavingMode.h:45-46`（`bool bHighlightOn;` ＋ `void ShowPowerSaveHighlight(bool bOn);`）
- **V910 錨點**：`after` 行 **44** = `TDateTime StartTmr;`（全檔唯一；行 19 的
  `bool TimeOut(TDateTime StartTmr);` 因結尾是 `)` 不是 `;` 而不匹配）
- **信心**：**≥95%**

#### B2 — 四個面板常數 → **OP3（前半）**

- **V899 範圍**：`PowerSavingMode.cpp:32-37`；`after` 行 31 = `TDateTime OverDayAM=StrToTime("am 12:00:00");`（唯一）
- `const` 在檔案範圍具內部連結，且四個名字在 V910 全樹命中 0 → 無 ODR／衝突風險。
- 常數值已對 V910 `main.dfm` 驗證（§0.3）。
- **信心**：**≥95%**

#### B3 — ctor 初始化 → **OP4**

- **V899 範圍**：`PowerSavingMode.cpp:61-66`（6 行 = 5 行不變 ＋ `bHighlightOn=false;`）
- **V910 被取代**：`38-42`；anchor `ScanTmr->Interval=1000;`（全檔唯一）
- **為何用 `replace`**：`flagStartTmr=false;` 在 V910 命中 2 次（41、119），非唯一；
  往前包到 `ScanTmr->Interval=1000;` 才唯一。被取代 5 行與 V899:61-65 位元組相同（實測）。
- **信心**：**≥95%**

#### B4 — 三個還原呼叫點 → **OP5 / OP6 / OP11**

| op | V899 範圍 | V910 錨點 | 模式 |
|---|---|---|---|
| OP5 | `103-107`（5 行） | `before` 79 = `if(SystemStart \|\| IniConfig.bPowerSaveFunction==false)`（唯一） | `before` |
| OP6 | `118`（1 行） | `before` 89 = `fMain->pnlPowerSaving->Visible=false;`（唯一） | `before` |
| OP11 | `405-408`（4 行） | `after` 311 = `tModule->iCountDown=tModule->AlarmTmr;`（唯一） | `after` |

- **語意**：OP5 把「所有中止／重新計數路徑」的還原集中在 `OnScanTmr` 開頭一處
  （因為那些路徑都會先把 `bRestart` 立起來）；OP6 在面板即將隱藏前先還原高度與底色；
  OP11 在 `Restart()` 還原，讓下一輪倒數字串可讀。
- **信心**：**≥95%**

#### B5 — 點亮呼叫點 → **OP8**

- **V899 範圍**：`PowerSavingMode.cpp:252-260`（9 行 = 5 行不變 ＋ 4 行新增）
- **V910 被取代**：`209-213`（5 行）；anchor `if(MtrModule->Enabled==false &&`（全檔唯一）
- **為何用 `replace`**：自然插入點的下一行是 `Task++;`，在狀態機裡到處都是，不可當錨。
  被取代 5 行與 V899:252-255＋260 位元組相同（實測）。
- **信心**：**≥95%**

#### B6 — `ShowPowerSaveHighlight()` 函式本體 → **OP10**（含主題 C）

- **V899 範圍**：`PowerSavingMode.cpp:354-399`（46 行，含結尾分隔線）
- **V910 錨點**：`before` 行 **307** = `void TPowerSaving::Restart()`（全檔唯一）
- **語意**：`bPowerSaveShowCaption==false || fMain==NULL` 直接 return（**這是整個高亮功能的總閘門**）；
  點亮＝黃底紅字、面板 20→28、字高 −16→−20；還原＝寫回 `main.dfm` 原值；
  最後 `int iTop=fMain->ShowFunctions(); if(iTop>0) fMain->btnView->Top=iTop;` 重排狀態列。
- **信心**：**≥95%**

---

### 主題 C — 倒數字串凍結的還原 guard（commit `7de2301`，20260817，1 條）

#### C1 — 分離式 guard（矩陣 `PowerSavingMode.cpp:363`）→ **併入 OP10**

這條**不是獨立 op**：它是 `ShowPowerSaveHighlight()` 函式本體（V899:358-398）內的一段，
與 0811 的函式在同一片連續位元組上，因此**必然隨 OP10 一起搬，不可能漏**。

- **V899 範圍**：`PowerSavingMode.cpp:363-374`（4 行註解 ＋ 8 行 guard），落在 OP10 的 354-399 內
- **修正內容**（`7de2301` 的核心）：

  ```
  舊（899.36，會吃掉還原）：  if(bHighlightOn==bOn) return;

  新（899.37）：
      if(bOn)
      {
          if(bHighlightOn)                    //重複點亮才擋
              return;
      }
      else if(bHighlightOn==false &&
              fMain->pnlPowerSaving->Caption=="PowerSaving")   //已在還原狀態才略過
          return;
  ```

- **根因（V899 commit message 已載明，本波僅忠實搬運）**：倒數字串由 `OnScanTmr` case 2
  每秒直接寫 `pnlPowerSaving->Caption`（V910 `PowerSavingMode.cpp:141-142`），
  **不經過 `ShowPowerSaveHighlight()` 也不動 `bHighlightOn`**。所以倒數中 `bHighlightOn` 是 `false`
  而 Caption 已非 `"PowerSaving"`；此時切回生產呼叫 `ShowPowerSaveHighlight(false)`，
  舊 guard 因 `bOn==bHighlightOn==false` 直接 return，Caption 就凍在最後一個倒數值
  （客戶截圖：機台 Running 但顯示 `PowerSaving:01:29:45`）。
- **為何不能直接拿掉 guard**：進入生產後 `bRestart` 每秒被設為 true（OP5 的 `if(bRestart)` 路徑），
  無 guard 會每秒重寫 Caption/Color/Font/Height 並重跑 `fMain->ShowFunctions()`
  （23 個狀態面板全重排）→ 就是 guard 當初要防的閃爍。
- **重要語意**：新 guard 用**面板自己的 Caption**判斷是否已在還原狀態，而不是用自家旗標。
  **`"PowerSaving"` 這個字面值必須與 `main.dfm` 的 `Caption` 完全一致 —— 已於 §0.3 驗證 V910 相同。**
- **信心**：**≥95%**

---

## 2. 非 PTI 路徑零影響的證明

**本波是 B 類，但 gate 完全沿用 V899 原生的 `FUNC_CC_PTI` 分支與五個旗標，未新增任何 `CUSTOMER_CODE` 判斷。**

1. **唯一開關點**：五個 `bPowerSave*` 旗標與 `iPowerSaveMaxMinute` 全樹只有一處被設為非預設值，
   就在 `FUNC_CC_PTI()` 內（OP13）。`FUNC_CC_PTI()` 全樹唯一呼叫點是 `DoCustomerFunction()` 的 `case CC_PTI:`。
   → 非 PTI 機台，六個值恆為 `InitialCosFunction()` 寫的預設（OP14）。
2. **逐個消費點的非 PTI 行為**：

| 消費點 | 非 PTI 時 | 指令級影響 |
|---|---|---|
| `GetPowerSaveMaxMinute()`（OP3/OP9） | `iPowerSaveMaxMinute=200 > 0` → 回 200 | **與原 `MaxMinute`(200) 同值**，`SetAlarmTime` 結果不變 |
| `bPowerSaveLotEndOnly` guard（OP7） | `false` → 短路，不進 body | 執行 0 條敘述，控制流不變 |
| `bPowerSaveSkipAmbient` guard（OP7） | `false` → 短路 | 同上 |
| `ShowPowerSaveHighlight()` ×4 呼叫點（OP5/6/8/11） | `bPowerSaveShowCaption==false` → **函式第一行就 return** | 面板 Caption/Color/Font/Height **完全不被觸碰**，`ShowFunctions()` 不被呼叫 |
| `bPowerSaveTempOnly` 分支（OP16） | `false` → 落到 `else if(bPowerSaveFunction)` | **＝V910 現行分支，位元組未動** |
| `IniConfig.bPowerSaveFunction`（OP13） | 只在 `FUNC_CC_PTI` 設 true | 其他客戶維持各自原值 |

3. **唯一可量測的非 PTI 成本**：OP5 讓 `OnScanTmr`（1 秒 timer）每次多一次
   `ShowPowerSaveHighlight(false)` 呼叫，而該函式對非 PTI **第一行就 return**。
   即每秒多一次「讀一個 bool、比較、返回」。**忠實照搬 V899，不做最佳化。**
4. **移除行**：全波只移除 3 行，兩行是 OP9 的等值取代，一行是 OP16 的 `if`→`else if` 降級。
   **沒有任何 V910 既有功能被刪除。**
5. **結構加欄位（OP12）**：多 5 bool + 1 int，§0.5.3 實測無任何佈局相依存取。

**結論：非 CC_PTI 客戶路徑指令級不變（唯一例外是每秒一次立即返回的函式呼叫，以及結構多 6 個未被讀到的成員）。**

---

## 3. PTI 端的真實行為變更（**必須記入 LEDGER**）

| # | 變化 | 來源 | 影響面 |
|---|---|---|---|
| **A** | **`IniConfig.bPowerSaveFunction` 由各機台原值改為恆 `true`** | OP13 第 2 行 | **V910 的 `FUNC_CC_PTI` 從未設過它** → 這是 PTI 在 V910 上的**全新開關**：`[C05]` 群組在 Configuration 現身，省電倒數計時器開始運作 |
| B | `[C05]` 頁只開溫控模組；motor/vacuum/ATC/mode 全部 `bNoShow/bDisable/bFixedValue`，且 mode 強制 0 | OP16 | mode 1 會跳 YES/NO 對話框等操作員，會讓無人值守的整夜省電失效，故刻意鎖 0 |
| C | `[C05]` 溫控 halt time 上限 200 → **720 分（12 小時）** | OP13 + OP3 + OP16 | 讓 8 小時（480 分）可設；`GetPowerSaveMaxMinute()` 再 clamp 到 1439（`EncodeTime` 硬上限） |
| D | Lot 進行中（`RunInfo.bLotStart`）不進省電 | OP7 | one-cycle 維修仍保有 lot-start，在那裡斷加熱會白白多一次 re-soak |
| E | Ambient 模式停機不介入 | OP7 | 依客戶說明超出範圍 |
| F | 進入省電後主畫面 `pnlPowerSaving` 黃底紅字、面板 20→28、字高 −16→−20，並重排狀態列 | OP10 | 每次點亮/還原各觸發一次 `fMain->ShowFunctions()`（23 個狀態面板重排）＋`btnView` 重新定位 |
| G | 倒數字串凍結修正 | OP10（0817 guard） | 倒數中切回生產時 Caption 正確還原成 `"PowerSaving"`，不再凍在 `PowerSaving:hh:nn:ss` |

**已知的刻意分歧（不阻塞，記 LEDGER）**：
V899 的 PTI 分支**沒有** V910 Eastsun 20260710 的 `USE_BU5_Function` 200/400 分岔。
實測 `USE_BU5_Function` 在 V910 是死旗標（`cmydef.cpp:5638` 初始 0，全樹無賦值、無綁定、無 ini key），
恆為 0 → 既有分支恆走 200 路徑。**因此此分歧在實機上零差異**；且 PTI 分支只開 `edC05_Temp`
一個編輯欄，其上限 720 本來就 > 400。忠實搬運 V899，不自行合併。

---

## 4. 交付與收工 gate 建議（給 主腦）

1. `python tools/port_tools/splice.py docs/mg_w8_ops.json`
   - 預期輸出（5 檔）：
     `OK PowerSavingMode.h: 2 op(s) applied, eol=CRLF`
     `OK PowerSavingMode.cpp: 9 op(s) applied, eol=CRLF`
     `OK CosFunction.h: 1 op(s) applied, eol=CRLF`
     `OK CosFunction.cpp: 2 op(s) applied, eol=CRLF`
     `OK cConfiguration.cpp: 2 op(s) applied, eol=CRLF`
   - 套用後行數應為 **72 / 535 / 502 / 4506 / 7867**（§0.8）
   - 五檔各自產生 `.mgbak`
2. `python tools/port_tools/port_check.py PowerSavingMode.h PowerSavingMode.cpp CosFunction.h CosFunction.cpp cConfiguration.cpp`
   - 預期：`added` 全數歸為 **SPLICED**、`authored=0`、`comment=0`
   - EOL：`(68,0)->(72,0)`、`(437,0)->(535,0)`、`(496,0)->(502,0)`、`(4493,0)->(4506,0)`、`(7840,0)->(7867,0)`
   - `REMOVED` 應**恰好 3 行**，且只有這 3 行：
     `else if(Minute>MaxMinute)` / `Minute=MaxMinute;` / `if(IniConfig.bPowerSaveFunction)`（§0.8）
3. `bcc32 -c -H- PowerSavingMode.cpp` / `CosFunction.cpp` / `cConfiguration.cpp`
   （V899 根目錄 include 設定；兩個 `.h` 由這三支帶入）
   - `CosFunction.h` 是廣泛被 include 的定義檔，加欄位後全量 build 時所有相依 .cpp 都會重編；
     波內只用 `-c` 隔離編譯，全量 build 留給 Phase 3。
   - **`cConfiguration.cpp` 是本波最需要編譯背書的一支**（新 `#include` ＋ 27 行新分支 ＋ 9 引數
     `elConfig->Add` 多載 ＋ 首次使用 `labC05_2/4/6/8`）。
4. 主腦逐條開檔複驗，**重點三處**：
   - **`cConfiguration.cpp` 的 `else if` 降級**：套用後第 **1103** 行必須是
     `    else if(IniConfig.bPowerSaveFunction)`，第 1104 行起必須完整保留 V910 的 Eastsun BU5 內容。
     這是全波唯一可能損毀公司既有功能的位置。
   - **`CosFunction.cpp:3936-3941` 六行都在**（3877-3880 那四行無 AI 註解，最容易被漏，§0.7）。
   - **`PowerSavingMode.h` 套用後與 V899 出貨版位元組完全相同**（72 行，0 差異）——
     這條若不成立代表 op 有錯。
5. 收工後 `python tools/port_tools/ai_comment_matrix.py`，MISSING 應**減少 35 條**。
   - 注意 `wc -l` 會多算，因為含換行的多行註解在 CSV 是被引號包住的單一 row，**一律以 `csv.reader` 為準**。
   - **基數會漂移，不要拿本文件的數字當收工基準**：本波分析期間主迴圈並行推進 MG-W7
     （`ContactForce.cpp`／`iosetview.cpp`，與本波五檔零重疊）並新增了
     `docs/mg_matrix_allowlist.csv`，`csv.reader` 計數在分析開始時是 **330**、結束時是 **275**。
   - **只驗「減 35 條、且這 35 條正是本波的」**。本波 35 條的檔／行清單（收工前已用當下 CSV 重新確認仍全數存在）：

     | 檔 | 行 |
     |---|---|
     | `CosFunction.cpp` | 1469, 1470, 1471, 1472, 1473, 1474, 1475, 3875, 3876 |
     | `CosFunction.h` | 471, 472, 473, 474, 475, 476 |
     | `PowerSavingMode.cpp` | 32, 39, 66, 103, 118, 137, 256, 258, 289, 354, 363, 406, 407 |
     | `PowerSavingMode.h` | 45, 46, 68 |
     | `cConfiguration.cpp` | 38, 1009, 1029, 1031 |
6. LEDGER 需記：§3 的 A–G 七項，**特別是 A（PTI 首次開 `bPowerSaveFunction`）**與
   「`USE_BU5_Function` 分歧（實測零差異）」。

---

## 5. 結論

- **35 條矩陣紀錄 → 16 個 splice op**（6 `replace`、5 `after`、5 `before`），
  錨點全部經過**全檔唯一性實測**（16/16 唯一）。共搬 170 行：35 行矩陣標記 ＋ **116 行未標記新程式** ＋
  19 行為錨點唯一性重插的既有行。
- **無 <95% 項目。** 16 個 op 信心全部 ≥95%。
- **`.dfm` 明確答案：不做 .dfm op。** V899 的 `cConfiguration.dfm` 確有 6 行 diff，但全是設計器
  `ActivePage`/`TabIndex` 狀態，與 C05 無關；V910 的 `gbC05` 區塊與 V899 位元組相同（94 行），
  `labC05_2/4/6/8` 四個 TLabel 都在，`main.dfm` 的 `pnlPowerSaving`（`Height=20` / `Font.Height=-16`
  / `clBlue` / `Caption='PowerSaving'`）兩樹位元組相同。
- **省電模組本體在 899→910 期間公司從未動過**：`PowerSavingMode.h` 兩樹（正規化 EOL 後）位元組完全相同，
  `PowerSavingMode.cpp` 只差兩個空行。套用後這兩檔**等於 V899 出貨版**。
- **唯一的真實碰撞區是 `cConfiguration.cpp` 的 `[C05]` 區段**（Eastsun 20260710 `USE_BU5_Function` 改寫）。
  解法是把新 PTI 分支插在前面、既有 `if` 降級為 `else if`，**V910 分支本體零位元組變動**；
  且 `USE_BU5_Function` 實測是死旗標（無賦值/無綁定/無 ini key），既有分支恆走 200 路徑。
- 本波**不需要新增客戶碼隔離**；非 PTI 路徑指令級不變，唯一外溢是每秒一次立即返回的
  `ShowPowerSaveHighlight()` 呼叫，以及 `TCosFunction` 多 6 個未被讀到的成員（需全量重編）。
- **不屬本波**：20260811 的 `aoutarm9045.cpp`(7) / `aRotateKIT_Out.cpp`(4) / `aoutarm9045.h`(1) 共 **12 條**
  （OutArm rotate 主題，與本波零符號交集，留 W9+）；以及 V899 同區段但屬別波的
  `bUseInArmLoadStageWatchdog`(0803)、`bHotAirByMOTemperatureMode`(0819)、`bUseFix3FullTray`(0520)——
  三者皆已從 op 區段刻意排除。
