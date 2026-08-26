# MG-W11 分析：AutoClean CKPP 主題波（0407 / 0408 / 0409 / 0414 / 0416 / 0417 ＋ **0514**）

- **波次**：MG-W11（V899 → V910），LEDGER 相依鏈 **#4**
- **來源**：`docs/mg_ai_matrix_missing.csv`
  - 交派範圍 **20 條**（0407×10、0408×6、0409×1、0416×1、0417×2，扣掉不屬本波的 cShowBinSelect 1 條）
  - **實際本波 28 條**（見 §0.3：**20260514 的 6 條與 0407-0409 是同一片連續位元組，無法分割**）
- **V899 樹**：`D:\HT9045\HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422`（唯讀）
- **V910 樹**：`D:\HT9045\HT9011UC_Code_V3.33.910.0_20260716_Jimmy`（分析期間 `git status --porcelain` 全空、無 `.mgbak`）
- **本文件性質**：分析＋splice op 草案。**全程唯讀；6 個檔的 md5 收工前後完全相同（§0.1），未修改任何原始碼、未動 `.bpr`、未動 allowlist。**
- **op 檔**：`D:\HT9045\docs\mg_w11_ops.json`（**17 個 op**，只碰 **2 個檔**）

---

## ⚠ 本波最重要的三句話

1. **V910 出廠版（pristine `e06524a`）已經含有整個 `Common\PickPlanner\` 目錄（7 檔）與
   `AutoClean\cCleanKitPickPlan.h`，而且與 V899 位元組完全相同、`HT9045.bpr` 已完整登錄。
   → 本波「新增檔」數量＝0，`.bpr` 變更＝0。**
2. **唯一缺的是 `AutoClean\AutoClean.cpp` 裡的實作。** V910 的 `AutoClean.cpp` 全檔
   **0 個 `//AI(` 註解**、0 處引用 `bUseCKPP`／`g_CKPlan`／`cCleanKitPickPlan`
   ——公司拿到了新檔，卻沒收 `AutoClean.cpp` 的改動（該檔另有自家 13 處演進）。
   `cCleanKitPickPlan.h` 目前是 V910 樹裡的**孤兒標頭**（無人 `#include`），
   `extern bool bUseCKPP;` / `extern cCleanKitPickPlan g_CKPlan;` **只有宣告、沒有定義**。
3. **`bUseCKPP` 預設 `false`，且全樹沒有第二處賦值、沒有 ini key、沒有 UI 綁定
   → 15 個 op 中有 12 個是休眠搬運，行為零變更（§3）。真正會改變執行期行為的只有 3 個 op（§5）。**

| 分類 | 條數 | 處置 |
|---|---:|---|
| **P 類**（真缺口 → 出 op，休眠／註解，零行為） | 23 | `mg_w11_ops.json` OP1–OP5、OP6a、OP6c、OP7–OP13 |
| **P 類**（真缺口 → 出 op，**閘外行為變更**） | 3 | OP6b（<95%）、OP14、OP15 |
| **C 類**（假 MISSING，V910 已有等價碼，只差註解位元組） | 2 | `ainarm9045_1x1_1.cpp` → 進 allowlist（§6） |
| 合計 | **28** | |

---

## 0. 前置事實（全部實測，非推論）

### 0.1 檔案基本盤（收工前後 md5 相同＝證明唯讀）

| 檔 | V899 行數 / md5(12) | V910 行數 / md5(12) | 本波動它？ |
|---|---|---|---|
| `AutoClean\AutoClean.cpp` | 9537 / `9380cbd85c68` | **9137** / `79c310c8ddc2` | ✅ 13 op |
| `AutoClean\uCleaning.cpp` | 2880 / `d227f5405e00` | 2922 / `df0cd9d3d1e6` | ✅ 2 op |
| `AutoClean\cCleanKitPickPlan.h` | 131 / `27ca513b7700` | 131 / **`27ca513b7700`（相同）** | ❌ 不動 |
| `AutoClean\AutoClean.h` | 48 / `0c42697487ca` | 48 / **`0c42697487ca`（相同）** | ❌ 不動 |
| `ainarm9045_1x1_1.cpp` | 2400 / `d3fb66664b21` | 2432 / `88d230e827cf` | ❌ 不動（C 類，§6） |
| `HT9045.bpr` | — / `58a5798039f5` | — / `f76f53e5e13d` | ❌ **不動**（§2） |

### 0.2 ⚠ EOL 地雷：**`AutoClean\AutoClean.cpp` 在 V910 是純 bare-LF**

| 檔 | V899 | V910 |
|---|---|---|
| `AutoClean\AutoClean.cpp` | CRLF 9537 / bare-LF **0** | CRLF **0** / bare-LF **9137** |
| `AutoClean\uCleaning.cpp` | CRLF 2880 / bare-LF 0 | CRLF 2922 / bare-LF 0 |

這是 MG 政策 §鐵律 3 講的「V910 有 ~34 檔是 LF」其中一檔。
`splice.py` 的 `dominant_eol()` 會判成 LF 並把 V899 的 CRLF payload 轉成 LF ——**行為正確**，
事前模擬實測套用後 **CRLF=0 / bare-LF=9682**，無混用（§7）。
**收工 gate 必須確認 `AutoClean.cpp` 仍是 0 個 CRLF；若出現 CRLF＞0 就是出事了。**
**絕不對這個路徑 `git checkout`**（autocrlf 會把整檔改寫成 CRLF）。

### 0.3 範圍修正：**20260514 的 6 條必須同波，不能留給別波**

`grep` 交派的六個日期只涵蓋 `AutoClean.cpp` 的 18 條；但矩陣裡該檔另有
**20260514 × 6 條 MISSING**，內容是「把 CKPP 的窮舉邏輯改成委派給 `cArmPickPlan`」的重構：

| 矩陣行 | 內容 |
|---:|---|
| 1650 | align comment with actual default (off; legacy SearchCleanKitUpDown is in use) |
| 1755 | legacy EvaluatePlan removed; enumeration delegated to cArmPickPlan |
| 1756 | IArmPickPlanContext implementation begins |
| 1816 | preserve legacy "skip plans where lead has no pad / unreachable" |
| 1827 | translate generic plan back into legacy TPickPlan/TPickSlot |
| 1896 | Search() now delegates enumeration to generic cArmPickPlan |

這 6 條全部落在 **V899 `AutoClean.cpp:1644-1953` 這一片連續 310 行**之內，
和 0407 的 `cCleanKitPickPlan` 實作、0408 的 `g_CKPlan` 全域是**同一個位元組區段**，
**物理上不可能只搬其中一部分**。且 `cCleanKitPickPlan.h`（V910 已有）的宣告就是 0514 重構後的版本：
若只搬 0407 版實作，會與 V910 已在的標頭**簽名不符、編不過**。

> **另注**：`AutoClean.cpp:1652`（0408 global plan instance）在矩陣裡是 `other-file` 不是 MISSING
> （簽章在 `cCleanKitPickPlan.h` 命中），但它就在同一片區段內，**必然隨 OP2 一起搬**。
> 所以 op 實際落地 **25 個 `//AI` 標記**，矩陣只會少 24 條（AutoClean.cpp）。

### 0.4 交派敘述的一處更正：**V910 `AutoClean.cpp` 沒有 fAOI／0527 的公司漂移**

實測：V910 `AutoClean.cpp` 全檔 `fAOI` 命中 **0**、無 `20260527`。
該檔 2026 年的公司註解日期只有
`20260109 / 0126 / 0130 / 0212 / 0312 / 0324 / 0408 / 0414 / 0415 / 0421 / 0427 / 0504 / 0506`
（JerryYang、Steven、RogerYang、KevinCheng、Jimmychiu）。fAOI/0527 應是別檔的 histogram。

### 0.5 V910 `AutoClean.cpp` 的公司漂移地圖（**本波全部保留、一個位元組不動**）

`git diff --no-index V910→V899` 共 **72 hunk（+564 V899-only / −176 V910-only）**。
其中 **19 個 hunk 是本波標的**，其餘 53 個是公司自家演進。與本波 op 相鄰、最需要小心的四處：

| V910 位置 | 公司內容 | 本波處置 |
|---|---|---|
| 2997-3010（`case 3100` 內） | `if(TestIF_File.iTestMode==SingleSite) iKitCol=iAutoCleanPickPlateX;` **JerryYang 20260415 fix 1x1 auto clean** | **OP7 插在它之前**，模擬輸出實測仍完整保留（§7.2） |
| 198 / 2915 | `Prod.bSingleUseOtherSuck \|\| Prod.bSingleInArmUseOtherSuck` **JerryYang 20260414** | 不碰（V910 為超集） |
| 1685-1717（`SearchCleanKitRowCol`） | **JerryYang 20260427** 把整段 `_12Site2X6/_6Site2X3N` 條件 `//` 掉 | 不碰（公司反向刻意修改） |
| 5190-5292（`case 2000/2001/2002`） | **KevinCheng 20250919** `bA81WaitSECS` 等 SECS 等待狀態 | 不碰 |

模擬結果 vs V899 全檔比對剩 **53 個差異 hunk**，逐一檢視**全部**是上述公司漂移
＋ 6 行空白行差異（V899 有、V910 無的純空行）——**沒有任何一條是漏搬**（§7.1）。

---

## 1. PickPlanner 註解日期分布專節（交派問題 2 的答案）

**V899 `Common\PickPlanner\` 7 檔共 28 個 `//AI` 註解，日期 100% 是 `20260514`。
沒有任何一條早於 20260323（V899 分支點）。→ 不存在「898 期工作」的範圍問題。**

| 檔 | `//AI` 數 | 日期 | 矩陣狀態 |
|---|---:|---|---|
| `IArmPickPlanContext.h` | 2 | 20260514 | `same-file` ×2 |
| `cArmPickPlan.h` | 1 | 20260514 | `same-file` ×1 |
| `cArmPickPlan.cpp` | 2 | 20260514 | `same-file` ×2 |
| `cInArmLoaderPickPlanner.h` | 3 | 20260514 | `same-file` ×3 |
| `cInArmLoaderPickPlanner.cpp` | 10 | 20260514 | `same-file` ×10 |
| `cOutArmPlacePlanner.h` | 3 | 20260514 | `same-file` ×3 |
| `cOutArmPlacePlanner.cpp` | 7 | 20260514 | `same-file` ×7 |
| **合計** | **28** | **全 20260514** | **28 條全部 `same-file`（0 條 MISSING）** |

`AutoClean\cCleanKitPickPlan.h` 另有 9 條（0407×2、0408×3、0514×4），矩陣狀態亦**全部 `same-file`**。

**這 37 條從來就不在 MISSING 清單裡**——因為 V910 早就有這些檔，且位元組相同。

---

## 2. 新增檔與 `.bpr` 影響專節（交派問題 1、3 的答案）

### 2.1 V910 **有** `Common\PickPlanner` 目錄，且是公司出廠版就帶的

```
git log --oneline --all -- "HT9011UC_Code_V3.33.910.0_20260716_Jimmy/Common/PickPlanner/"
→ e06524a  Track HT9045 V910 20260716 pristine source（MG 戰役基線）      ← 只有這一筆
```

**不是前面哪一波搬進去的**：`--diff-filter=A` 指向基線 commit 本身，之後 0 次修改。
（目錄 mtime 顯示今天 14:13 是 git checkout 的時間戳，**不是**檔案內容的日期；
檔案 mtime 全部是 5/14，與 V899 一致。）

7 檔 md5 **全部相同**：

| 檔 | md5(12) 兩樹 |
|---|---|
| `IArmPickPlanContext.h` | `be3c2b1143c1` |
| `cArmPickPlan.h` | `a6e858a7feaf` |
| `cArmPickPlan.cpp` | `e1e80f418c2c` |
| `cInArmLoaderPickPlanner.h` | `64721d2b5d3d` |
| `cInArmLoaderPickPlanner.cpp` | `9b105e3ff8c7` |
| `cOutArmPlacePlanner.h` | `a94bcecb7322` |
| `cOutArmPlacePlanner.cpp` | `9a7202f236a7` |
| （另）`AutoClean\cCleanKitPickPlan.h` | `27ca513b7700` |

### 2.2 `.bpr` **不需要任何變更**（實測）

V910 `HT9045.bpr` 已含：

```xml
<FILE FILENAME="AutoClean\cCleanKitPickPlan.h"                 UNITNAME="cCleanKitPickPlan.h"     CONTAINERID=""/>
<FILE FILENAME="Common\PickPlanner\IArmPickPlanContext.h"      UNITNAME="IArmPickPlanContext.h"   CONTAINERID=""/>
<FILE FILENAME="Common\PickPlanner\cArmPickPlan.h"             UNITNAME="cArmPickPlan.h"          CONTAINERID=""/>
<FILE FILENAME="Common\PickPlanner\cArmPickPlan.cpp"           UNITNAME="cArmPickPlan"            CONTAINERID="CCompiler"/>
<FILE FILENAME="Common\PickPlanner\cInArmLoaderPickPlanner.h"  UNITNAME="cInArmLoaderPickPlanner.h" CONTAINERID=""/>
<FILE FILENAME="Common\PickPlanner\cInArmLoaderPickPlanner.cpp" UNITNAME="cInArmLoaderPickPlanner" CONTAINERID="CCompiler"/>
<FILE FILENAME="Common\PickPlanner\cOutArmPlacePlanner.h"      UNITNAME="cOutArmPlacePlanner.h"   CONTAINERID=""/>
<FILE FILENAME="Common\PickPlanner\cOutArmPlacePlanner.cpp"    UNITNAME="cOutArmPlacePlanner"     CONTAINERID="CCompiler"/>
```

- 3 支 `.cpp` 都是 `CONTAINERID="CCompiler"` → **會被編譯並連結進 `HT9045.exe`**。
- `Common\PickPlanner` 出現在 V910 `.bpr` 的**全部 4 條 INCLUDEPATH／SRCPATH 串**的最前面。
- `tools/port_tools/bcc_syntax.sh` 的 `INC` 變數**第一項就是 `Common\PickPlanner`** → gate 不用改。

**結論：本波不新增任何檔案、不做任何位元組級整檔複製、不動 `.bpr`。**

### 2.3 CKPP 雙路開關對這些檔的編譯相依（交派問題 3 的答案）

**是的，`AutoClean.cpp` 的 CKPP 區段依賴這些檔才能編譯**，相依鏈：

```
AutoClean.cpp  --OP1-->  #include "cCleanKitPickPlan.h"       (V910 已有，位元組相同)
                              |
                              +--> #include "cArmPickPlan.h"  (V910 Common\PickPlanner，位元組相同)
                              |         +--> IArmPickPlanContext.h  (同上)
                              |         +--> TGenericPickPlan / TGenericPickSlot / ARM_PICK_PLAN_MAX_SLOTS
                              +--> class cCleanKitPickPlan : public IArmPickPlanContext
                                        成員 cArmPickPlan m_Engine
                                        （實作在 cArmPickPlan.cpp，已在 .bpr 的 CCompiler 清單）
```

**但因為 V910 這三層全部就位，實際不需要做任何相依準備工作。**
已用 bcc32 實編證實（§7.3）：套用後的 `AutoClean.cpp` **0 error**。

補充兩點安全查核：

- `cArmPickPlan::cArmPickPlan()` 只碰自己的成員（`m_pCtx=0; m_bFound=false; ResetPlan(m_Best);`），
  **不觸碰任何全域** → OP2 新增的 `cCleanKitPickPlan g_CKPlan;` 靜態初始化**無 SIOF 風險**。
- `cCleanKitPickPlan.h` 自給自足（只 `#include "cArmPickPlan.h"`，不需 VCL 或機台全域），
  放在 V899 原位置（`#include "AutoClean.h"` 之後、`aArmHeader.h` 之前）即可。

---

## 3. `bUseCKPP` 預設值證明（交派問題「預設走哪路」的答案）

**預設 `false`＝走舊的 `SearchCleanKitUpDown`。休眠搬運，行為零變更。**

四道獨立證據：

1. **唯一定義**（V899 `AutoClean.cpp:1651`，本波 OP2 payload 內）：
   ```cpp
   bool bUseCKPP = false;  // default: false -> use legacy SearchCleanKitUpDown; set true to enable cCleanKitPickPlan
   ```
2. **全樹唯一賦值**：`grep -rn "bUseCKPP" V899`（排除 `.svn`）共 15 個命中，
   1 個是 `cCleanKitPickPlan.h:24` 的 `extern`，1 個是上面的定義，
   其餘 **13 個全部是唯讀比較**（`if(bUseCKPP)` / `if(bUseCKPP && ...)`）。
   **沒有任何一處寫入。**
3. **無外部開關**：`bUseCKPP` 不出現在任何 `ReadIniData`／`CheckAndReadIniData`／`elConfig->Add`／
   `.dfm`／`HandlerCondition.Data`。**只能改原始碼重編才會變 true。**
4. **作者自述**（20260514，本波矩陣行 1650）：
   `align comment with actual default (off; legacy SearchCleanKitUpDown is in use)`
   ——這條註解本身就是為了把說明改成「實際上是關的」而加的。

**推論：OP2/OP3/OP4/OP6a/OP6c/OP7/OP8/OP9/OP10/OP11 共 10 個 op 的新程式碼在實機上永不執行；
其中 OP8/OP9/OP10 的 `else` 分支經逐位元組確認＝V910 原邏輯（僅整體縮排 +4）。**

---

## 4. 各 op 明細與閘控歸屬

| op | V910 動作 | V899 payload | Δ | 主題（矩陣日期） | 閘控 | 實機行為 |
|---|---|---|---:|---|---|---|
| OP1 | `after 4` | `[5,5]` | +1 | include CKPP 標頭 | — | 無 |
| OP2 | `replace [1661,1682]` | `[1622,1953]` | **+310** | CKPP 類實作（0407/0408/**0514**） | `bUseCKPP` 定義處 | **無**（全部只被 `if(bUseCKPP)` 呼叫） |
| OP3 | `replace [1833,1843]` | `[2102,2135]` | +23 | `SearchCleanKitUpDown` 雙路開關（0407）＋`CLEAN_FINISH_IC` 回收（0409） | `if(bUseCKPP){...return;}` | **無** |
| OP4 | `replace [2497,2507]` | `[2772,2847]` | +65 | `PickFromCleanKit` plan-driven pick（0408） | `if(bUseCKPP && g_CKPlan.Found())` | **無** |
| OP5 | `replace [2560,2564]` | `[2900,2905]` | +1 | 0416 Gigas 回退記錄 | — | **無**（純註解，見下） |
| OP6a | `replace [2697,2699]` | `[3038,3049]` | +9 | kit 空但已 partial pick（0408 S6） | `if(bUseCKPP)…else{Task=3000;}` | **無** |
| **OP6b** | `replace [2706,2706]` | `[3056,3065]` | +9 | **`iCleanPadCount==0` 防卡死（0407）** | **閘外** | **有（§5.1，<95%）** |
| OP6c | `replace [2722,2723]` | `[3081,3094]` | +12 | CKPP safety net（0408）＋ debug log（0407） | `if(bUseCKPP && !Found())` | **無** |
| OP7 | `replace [2997,3000]` | `[3369,3413]` | +41 | 統一 contact count＋完成度檢查（0408 S5） | `if(bUseCKPP && g_CKPlan.Found())` | **無** |
| OP8 | `replace [3046,3050]` | `[3456,3481]` | +21 | Fix H #1（0407，2-picker） | `if(bUseCKPP)…else{V910 原邏輯}` | **無** |
| OP9 | `replace [3072,3076]` | `[3503,3528]` | +21 | Fix H #2（0407，AxxG/1x4） | 同上 | **無** |
| OP10 | `replace [3104,3110]` | `[3556,3583]` | +21 | Fix H #3（0407，4-picker） | 同上 | **無** |
| OP11 | `replace [4646,4650]` | `[5113,5126]` | +9 | 清除幽靈 `HAS_NULL_CLEAN_IC`（0408） | `if(bUseCKPP && …)` | **無** |
| OP12 | `replace [4712,4723]` | `[5188,5200]` | +1 | 0417 ARM1 回退 V898 記錄 | — | **無**（純註解，見下） |
| OP13 | `after 5371` | `[5809,5809]` | +1 | 0417 ARM2 回退 V898 記錄 | — | **無**（純註解） |
| **OP14** | `replace [570,571]` | `[559,561]` | +1 | **pad count 歸零（0414）** | **閘外** | **有（§5.2）** |
| **OP15** | `replace [1969,1969]` | `[1949,1949]` | 0 | **運算子優先序（0414）** | **閘外** | **有（§5.3）** |

`AutoClean.cpp` 合計 **+545 → 9137 行變 9682 行**；`uCleaning.cpp` **+1 → 2922 變 2923**。

### 4.1 三個「純註解」op（OP5 / OP12 / OP13）為什麼還是要出 op

這三條的 V899 註解描述的是**「把某段程式碼撤回成 V898 樣子」的紀錄**，而 V910 的程式碼
**本來就是 V898 樣子**——也就是說**沒有可搬的程式碼，只有那一行註解**。逐位元組驗證：

| 條目 | V899 註解行 | 註解「上下文」在兩樹是否相同 |
|---|---|---|
| 0416 `AutoClean.cpp:2903` | revert phantom-marking skip per Gigas feedback | V899 2900-2902/2904-2905 ≡ V910 2560-2562/2563-2564（**位元組相同**） |
| 0417 `AutoClean.cpp:5199` | revert to V898 logic（ARM1） | V899 5188-5198/5200 ≡ V910 4712-4722/4723（**位元組相同**） |
| 0417 `AutoClean.cpp:5809` | revert to V898 logic（ARM2） | V899 5808 ≡ V910 5371（**位元組相同**） |

**與 MG-W7 §2.6「不主張補純註解」的差別**：MG-W7 那兩條的註解描述的是
「公司在 V910 新加、絕不可刪的功能」，補上去會誤導；本波這三條的註解**準確描述了 V910 的現況**
（碼確實是 V898 邏輯），補上去是正確的檔案紀錄，而且能把矩陣清乾淨。
**三條都是零風險零行為，主腦若要維持 MG-W7 慣例把它們拿掉，其餘 op 完全不受影響
（代價：矩陣永久留 3 條 MISSING）。**

### 4.2 搬移行數拆帳（MG-W8 §0.7 方法）

17 個 op 共搬 **643 行**：

| 類別 | 行數 |
|---|---:|
| 矩陣列出的 `//AI` 標記行 | **27**（AutoClean.cpp 25＋uCleaning.cpp 2） |
| **V899 新增但無 `//AI` 標記** → 矩陣看不到，漏搬即壞 | **520** |
| V910 原本就有、僅為取得唯一錨點而一併重插的既有行（位元組相同） | **96** |

**520 行未標記碼裡最危險的**：`cCleanKitPickPlan` 的 **21 支成員函式本體**
（`CalcX` / `IsPadAt` / `BuildDemand` / `GetLogicalPickerCount` / `GetPhysicalSucker` /
`GetTargetRowMax` / `GetTargetColMax` / `ComputeSlot` / `IsReachable` / `IsTargetCandidate` /
`IsSuckerDemanded` / `IsLeadValid` / `CopyGenericPlanToBest` / `Init` / `Search` / `Apply` / `HasDemand`
＋ ctor），矩陣只列了其中 10 行。
**本波全部以連續區段抽取，這些行自然被包含——這正是「用區段不用逐行」的理由。**

### 4.3 V910 端符號可用性（OP2 payload 逐符號稽核）

CKPP 實作用到的 40 個外部符號在 V910 全部存在，簽名一致（重點幾支）：

| 符號 | V910 定義處 | 簽名 |
|---|---|---|
| `GetAutoCleanPickCount()` | `AutoClean/AutoClean.cpp:163` | `int GetAutoCleanPickCount()` ✓ |
| `GetXPitchOfCleanKit()` | `AutoClean/AutoClean.cpp:548` | `int GetXPitchOfCleanKit()` ✓ |
| `GetInarmSuckRow()` | `AutoClean/AutoClean.h:47` | `void GetInarmSuckRow(int,int&,int&)` ✓ |
| `TrayHasCleanICCount()` | `AutoClean/AutoClean.cpp:2207` | `int TrayHasCleanICCount()` ✓ |
| `Special_2X6_Tray_XItem7()` | `AutoClean/AutoClean.h:46` | `extern bool …()` ✓ |
| `ReadWriteAutoCleanCount()` | `AutoClean/AutoClean.h:33` | `void (bool bRead, bool bReset=false)` ✓ **（OP14 用到 2 引數形式）** |
| `SetArrPlateXY()` | `Public/HTEditList.h:210` | 6 引數（第 6 個有預設值）✓ |
| `AddPickCleanPad()` | `Public/MyProductionRecord.h:43` | 5 引數 ✓ |
| `AddHPSuckGroup()` | `Public/HTEditList.h:202` | 無引數多載存在 ✓ |
| `InArmSuck.iMaxCol / iAutoCleanRecX / iAutoCleanRecY / PordRec` | `mykitsuck.h:159/244/245/236` | ✓ |
| `CC_KYEC_LEE` | `MachineType.h:287` `#define … 921` | ✓ |
| 區域變數 `flag1` / `iResult` | `AutoClean.cpp:2484` / `2593` | ✓（OP4 `return flag1;`、OP6a/6c `iResult=1;` 都合法） |

---

## 5. 三個閘外行為變更（**必須記入 LEDGER**）

### 5.1 OP6b — `iCleanPadCount==0` 防卡死（**本波唯一 <95% 項目，信心 92%**）

**位置**：`DoAutoCleanPickfromCleanKit` `case 10` → `TrayHasCleanIC()==false` → `InArmSuck.HasIC()==false`
→ `CheckCleaningCount()==true` 之後。

| | V910 現況（2705-2709） | V899 最終態（3055-3068） |
|---|---|---|
| 碼 | `iCleanPadCount=TrayHasCleanICCount();`<br>`if(CUSTOMER_CODE==CC_KYEC_LEE && iCleanPadCount==0)`<br>`{ ShowMyMessage("Clean Pad Count Different Site Count","End Auto Clean"); }` | `iCleanPadCount=TrayHasCleanICCount();`<br>`if(iCleanPadCount==0){ Task=1; iResult=1; }`<br>`else if(CUSTOMER_CODE==CC_KYEC_LEE)`<br>`{ ShowMyMessage(…); }` |

**逐情境影響**（`TrayHasCleanICCount()` 計 `HAS_CLEAN_IC`＋`CLEAN_FINISH_IC`＋`HAS_NULL_CLEAN_IC`）：

| 情境 | V910 | V899（搬入後） | 判定 |
|---|---|---|---|
| **非 KYEC，kit 全空（count==0）** | 什麼都不做 → `Task` 停在 10 → **每個掃描週期原地打轉＝永久卡站** | `Task=1; iResult=1;` → 正常結束取料 | ✅ **這就是本 op 的目的，明確修好一個 hang** |
| **KYEC，kit 全空（count==0）** | 跳 `ShowMyMessage`（會設 `bHandlerPause=true` **停機等操作員**）**然後照樣卡在 Task 10** | 靜默結束（**不再跳訊息**） | ⚠ KYEC 少一個提示，但原本提示完也還是卡住 |
| **KYEC，kit 只剩用過的 pad（count>0，無 `HAS_CLEAN_IC`/`CLEAN_FINISH_IC`）** | 什麼都不做 | **跳 `ShowMyMessage` 停機** | ⚠⚠ **V910 原本安靜的情境變成停機彈窗——本 op 唯一真正的回歸風險** |

**為什麼是 92% 而不是 ≥95%**：MG 決策規則「影響其他客戶 → 用該案客戶碼只包行為變更點，
預設路徑＝V910 原行為」。本 op 確實改到 `CC_KYEC_LEE` 的行為，而且第三列是
**V910 從未有過的方向**（新增一個會 `bHandlerPause` 的彈窗）。要照規則隔離就得**新寫條件式**，
違反「splice 不重打字」鐵律 → 交主腦裁決。

```
| F? | MG-W11（20260407 AutoClean iCleanPadCount==0 防卡死） | V899 把
`if(KYEC && count==0) ShowMyMessage` 改成 `if(count==0){Task=1;iResult=1;} else if(KYEC){ShowMyMessage}`。
非 KYEC 客戶因此修好一個永久卡站（V910 在 kit 全空時 Task 停在 10 不動）；但 CC_KYEC_LEE 的
ShowMyMessage 觸發條件由 count==0 反轉成 count!=0 —— 在「kit 只剩用過 pad」這個 V910 原本安靜的情境，
會變成跳出會 bHandlerPause 的停機訊息 |
(a) 忠實搬運 V899 出貨態（含 KYEC 行為變更） (b) 整條不搬（保住 KYEC 現況，非 KYEC 的卡站不修）
(c) 只補 `if(count==0){Task=1;iResult=1;}`、保留 V910 那行 KYEC 判斷（需新寫，非純 splice） |
**建議 (a)**：V899 是量產出貨版、此為其最終態；且 KYEC 在 count==0 時原本「彈窗完仍卡住」，
本來就不是可用路徑。唯一新風險是第三列情境。若主腦要零風險，刪掉 `mg_w11_ops.json` 的 OP6b 即可，
其餘 16 個 op **完全不受影響**（已實測：刪 OP6b 後 +536 行、bcc32 仍 0 error）。 |
```

### 5.2 OP14 — pad count 歸零（`uCleaning.cpp`，閘外，全客戶，信心 ≥95%）

`TfCleaning::LoadAutoCleanData()` 內，`iAutoClean_DeveicePices` 變更**或** `iAutoClean_Function`
開關切換（`bNeedReset`）且 `bRunAutoClean==false` 時，V899 多呼叫一行：

```cpp
ReadWriteAutoCleanCount(false, true);   //AI(…) 20260414: zero all pad counts … prevent … false WAR1922
```

`ReadWriteAutoCleanCount(bRead=false, bReset=true)`（V910 `AutoClean.cpp:1190-1240`）會把
`fMain->AutoCleanStringGrid` 每一格寫成 `"0"`，並把 `"0"` 寫回
`HandlerCondition.Data`／`IniData\DefineAutoClean\AutoCleanCount.Data`。

**行為變更**：換 DevicePices 或切換 AutoClean 開關時，**已累積的 clean pad 接觸次數會被清空（含落盤）**。
這是刻意的——舊 tray layout 的殘值會讓 `CheckCleaningCount()` 誤判並丟
`WAR1922`（V910 `AutoClean.cpp:2738/2748/2766`）。方向是**更保守**（歸零而非沿用殘值），
但它**會刪掉客戶累積的計數**，且**閘外、全客戶生效**，必須寫進 LEDGER。

### 5.3 OP15 — 運算子優先序（`uCleaning.cpp`，閘外，全客戶，信心 ≥95%）

```
V910 1968-1969:   if(TestIF_File.iTestMode==_12Site2X6 &&
                     iDeviceNum==12 || iDeviceNum==24)        →  (A && B) || C
V899 1948-1949:   if(TestIF_File.iTestMode==_12Site2X6 &&
                     (iDeviceNum==12 || iDeviceNum==24))      →  A && (B || C)
```

**行為變更**：V910 只要 `iDeviceNum==24`，這個 `if` 就恆真，於是
`iAutoClean_XDivision==4/8` 的「跳過第 0（或 0、1）欄」規則會套用到**任何** test mode；
修正後只在 `_12Site2X6` 套用。這是**收窄**行為，屬明確的 bug 修正（V899 已出貨），
但若有客戶在非 `_12Site2X6`＋`iDeviceNum==24` 的組態下已習慣目前的跳欄，會看到差異。

---

## 6. C 類：`ainarm9045_1x1_1.cpp` 兩條是**假 MISSING**（不出 op，進 allowlist）

**V910 已經有這兩條 `//AI` 註解和它們對應的全部程式碼。矩陣沒命中，是因為破折號的位元組不同。**

```
V899:894  b'…align with 2x4_8 baseline \xa1\x58 add bCheckInarmPlaceShuttle + CheckInArmDestroyICFail\r'
V910:891  b'…align with 2x4_8 baseline --   add bCheckInarmPlaceShuttle + CheckInArmDestroyICFail\r'
                                       ↑ Big5 全形破折號 vs ASCII "--"
```

| 矩陣條目 | V910 對應行 | 程式碼 |
|---|---|---|
| `ainarm9045_1x1_1.cpp:894`（0407） | **`:891`** | `bCheckInarmPlaceShuttle=true;` 及其後 destroy 區塊 → V910 **完全相同** |
| `ainarm9045_1x1_1.cpp:1243`（0407） | **`:1241`** | `ret=CheckInArmDestroyICFail(); if(ret==false) return false;` → V910 **完全相同** |

正規化（把 `\xa1X` 當成 `--`、壓縮空白）後逐行比對兩處 ±30 行區段，**差異只剩註解破折號與註解對齊欄位**。
另外 `ainarm9045_1x1_1.cpp:564`（0407 `int ret;`）本來就是 `same-file`。
→ **V910 收過 0407 的 ainarm 這條線；只有 `AutoClean.cpp` 那條線沒收。**

**建議 append 到 `docs/mg_matrix_allowlist.csv`（本 agent 唯讀，未動該檔）：**

```csv
ainarm9045_1x1_1.cpp,20260407,//AI(ht9045-v899) 20260407: align with 2x4_8 baseline,假MISSING：V910:891/:1241 已有同註解同程式碼，僅 Big5 全形破折號被正規化成 ASCII "--",MG-W11
ainarm9045_1x1_1.cpp,20260407,//AI(ht9045-v899) 20260407: align with 2x4_8 baseline,假MISSING：V910:891/:1241 已有同註解同程式碼，僅 Big5 全形破折號被正規化成 ASCII "--",MG-W11
```

> `ai_comment_matrix.py:86` 的比對是 `comment.startswith(a["comment_prefix"])`，
> 上面的 prefix 停在破折號**之前**，兩條都會命中。

---

## 7. 事前模擬與實編證據（記憶體內套用，未落盤；模擬檔在 scratchpad）

### 7.1 模擬結果

| 檔 | 套用前 | 套用後 | Δ | cp950 | U+FFFD | CRLF / bare-LF | 大括號平衡 |
|---|---:|---:|---:|---|---:|---|---|
| `AutoClean\AutoClean.cpp` | 9137 | **9682** | **+545** | OK | **0** | **0 / 9682** ✅ | 前 0 → 後 0 |
| `AutoClean\uCleaning.cpp` | 2922 | **2923** | +1 | OK | **0** | 2923 / 0 ✅ | 前 0 → 後 0 |

- **17 個 op 的錨點檢查全部通過**（錨點皆經 V910 **全檔唯一性**實測，17/17 唯一）。
- **40 組 `verify_same` 上下文行逐位元組比對全部通過**（確保 replace 區的既有行是原地重插）。
- 逐 op Δ 對帳：`1+310+23+65+1+9+9+12+41+21+21+21+9+1+1 = +545` ✓、`+1+0 = +1` ✓。
- **`//AI` 標記落地數與 V899 完全相同**：
  `AutoClean.cpp` `{0407:8, 0408:7, 0409:1, 0416:1, 0417:2, 0514:6}` ＝ V899 同檔數字，一條不差。

### 7.2 模擬輸出 vs V899 全檔比對 —— **53 個差異 hunk 全部是公司漂移**

`difflib` 比對「模擬結果 vs V899 出貨版」：**模擬側多 187 行（＝V910 公司內容保住了）、
V899 側多 42 行**。逐 hunk 檢視 V899 側那 42 行：

- 36 行是 §0.5 表列的公司超集／反向修改的**對應舊版**（JerryYang 20260414/0415/0427、Steven 20260427/0504、
  KevinCheng 20250919、RogerYang 20250515 等），**不搬是正確的**；
- **6 行全部是純空白行**（V899:3165 / 3905 / 3967 / 4366 / 5946 / 9516，實測皆為 `''`）。

**沒有任何一條 V899 的實質敘述被漏搬。**
關鍵保留點已目視確認：模擬輸出 3478-3481 仍是
`if(TestIF_File.iTestMode==SingleSite) … //JerryYang 20260415 : fix 1x1 auto clean`。

### 7.3 bcc32 隔離語法實編（**兩棵樹全程唯讀，obj 寫進 scratchpad**）

用 `bcc_syntax.sh` 的同一組旗標（`-c -H- -w- -D_VER6`，INC 換成絕對路徑指向 V910 樹）：

| 目標 | Error 數 |
|---|---:|
| **基準**：V910 pristine `AutoClean\AutoClean.cpp` | **0** |
| **套用後**：模擬結果 `AutoClean.cpp`（17 op 全上） | **0** ✅ |
| **套用後（刪 OP6b 變體）**：`AutoClean.cpp` | **0** ✅ |
| 基準：V910 pristine `AutoClean\uCleaning.cpp` | 0 |
| 套用後：模擬結果 `uCleaning.cpp` | **0** ✅ |
| `Common\PickPlanner\cArmPickPlan.cpp` | 0 |
| `Common\PickPlanner\cInArmLoaderPickPlanner.cpp` | 0 |
| `Common\PickPlanner\cOutArmPlacePlanner.cpp` | 0 |
| `ainarm9045_1x1_1.cpp`（不動，僅確認基準乾淨） | 0 |

> 執行前已確認**沒有 BCB IDE 在跑**（`tasklist` 無 `bcb`/`bcc32`/`make`），
> 且 `-n` 指向 scratchpad，**未寫入共用 `D:\HT9045\Obj`**。

---

## 8. 收工 gate 建議（給主腦）

### 8.1 前提

- 兩檔皆 pristine（**無 `.mgbak`**）；V910 樹 `git status --porcelain` 全空。
- **本波 op 的行號量在**：`AutoClean.cpp`（md5 `79c310c8ddc2`、9137 行、bare-LF）
  與 `uCleaning.cpp`（md5 `df0cd9d3d1e6`、2922 行、CRLF）。
  **若有別波先動了這兩檔，行號全部作廢，必須重量。**
  ⚠ `uCleaning.cpp` 另有 **20260423 Greatek ×4（V899:148-151，在 OP14 之上）**
  與 **20260703 SmartAutoClean ×3（V899:2650/2863/2867，在 OP15 之下）** 仍是 MISSING，屬別波。

### 8.2 執行

```
python tools/port_tools/splice.py docs/mg_w11_ops.json
```
預期輸出：
```
OK AutoClean\AutoClean.cpp: 15 op(s) applied, eol=LF     ← 注意是 LF 不是 CRLF
OK AutoClean\uCleaning.cpp: 2 op(s) applied, eol=CRLF
```
套用後行數應為 **9682 / 2923**；各產生 `.mgbak`。

### 8.3 驗收

1. `python tools/port_tools/port_check.py AutoClean\AutoClean.cpp AutoClean\uCleaning.cpp`
   - `added` 全數 **SPLICED**、`authored=0`
   - **EOL：`AutoClean.cpp` 必須是 `(0,9137)->(0,9682)`（CRLF 欄恆為 0）**；
     `uCleaning.cpp` `(2922,0)->(2923,0)`
   - `REMOVED` 行主要來自 12 個 `replace` op 的原地重插（96 行），其中**真正語意被取代的只有 4 行**：
     `Task=3000;`（OP6a，改成 `else{ Task=3000; }`）、
     `if(CUSTOMER_CODE==CC_KYEC_LEE && iCleanPadCount==0)`（OP6b）、
     `SetAutoCleanICCount(false);` 後缺的那行（OP14 為插入非刪除）、
     `iDeviceNum==12 || iDeviceNum==24`（OP15，加括號）。
2. `sh tools/port_tools/bcc_syntax.sh 'AutoClean\AutoClean.cpp' 'AutoClean\uCleaning.cpp'`
   → 預期 **PASS (0 errors)** 兩支（已於 §7.3 事前實編證實）。
3. **主腦逐條開檔複驗，重點五處**：
   - 套用後 **1691 行**必須是 `bool bUseCKPP = false;`（**若不是 `false` 就是搬錯**）
   - 套用後 **1994 行**必須是 `bool SearchCleanKitRowCol(int& iKRow,int& iKCol)`，
     且其下 **1996-1999** 必須完整保留 JerryYang 20260427 的 4 行 `//` 註解區塊
   - 套用後 **3478-3481** 必須完整保留 `//JerryYang 20260415 : fix 1x1 auto clean` 的 4 行
   - 套用後 `AutoClean.cpp` **不得出現任何 `\r\n`**
   - `uCleaning.cpp` 套用後 **1970 行**必須是 `(iDeviceNum==12 || iDeviceNum==24))`（括號齊全）
4. `python tools/port_tools/ai_comment_matrix.py` 重算
   - **只驗「本波這 26 條消失」，不要拿絕對總數當基準**——
     分析期間 missing.csv 由 222 列掉到 165 列（並行波次在推進）。
   - 本波 splice 清掉的 26 條：

     | 檔 | 日期 | 行 |
     |---|---|---|
     | `AutoClean\AutoClean.cpp` | 20260407 | 1645, 1649, 2112, 3056, 3092, 3456, 3503, 3556 |
     | `AutoClean\AutoClean.cpp` | 20260408 | 1941, 2782, 3040, 3082, 3370, 5116 |
     | `AutoClean\AutoClean.cpp` | 20260409 | 2116 |
     | `AutoClean\AutoClean.cpp` | 20260416 | 2903 |
     | `AutoClean\AutoClean.cpp` | 20260417 | 5199, 5809 |
     | `AutoClean\AutoClean.cpp` | 20260514 | 1650, 1755, 1756, 1816, 1827, 1896 |
     | `AutoClean\uCleaning.cpp` | 20260414 | 561, 1949 |

   - 再加 allowlist 2 條（§6）→ **本波共結清 28 條**。
   - 若主腦決定刪 OP6b，則少清 1 條（`AutoClean.cpp:3056`），變 25＋2＝27 條。

### 8.4 套用後位移表（後續波若要動這兩檔，行號用這張表換算；**已對模擬輸出逐點驗證通過**）

`AutoClean\AutoClean.cpp`（9137 → 9682）
```
N ≤ 4                → N
5    ≤ N ≤ 1660      → N+1
1661 ≤ N ≤ 1682      → 1662..1993   （OP2  replace 22→332）
1683 ≤ N ≤ 1832      → N+311
1833 ≤ N ≤ 1843      → 2144..2177   （OP3  replace 11→34）
1844 ≤ N ≤ 2496      → N+334
2497 ≤ N ≤ 2507      → 2831..2906   （OP4  replace 11→76）
2508 ≤ N ≤ 2559      → N+399
2560 ≤ N ≤ 2564      → 2959..2964   （OP5  replace 5→6）
2565 ≤ N ≤ 2696      → N+400
2697 ≤ N ≤ 2699      → 3097..3108   （OP6a replace 3→12）
2700 ≤ N ≤ 2705      → N+409
2706 ≤ N = 2706      → 3115..3124   （OP6b replace 1→10）
2707 ≤ N ≤ 2721      → N+418
2722 ≤ N ≤ 2723      → 3140..3153   （OP6c replace 2→14）
2724 ≤ N ≤ 2996      → N+430
2997 ≤ N ≤ 3000      → 3427..3471   （OP7  replace 4→45）
3001 ≤ N ≤ 3045      → N+471
3046 ≤ N ≤ 3050      → 3517..3542   （OP8  replace 5→26）
3051 ≤ N ≤ 3071      → N+492
3072 ≤ N ≤ 3076      → 3564..3589   （OP9  replace 5→26）
3077 ≤ N ≤ 3103      → N+513
3104 ≤ N ≤ 3110      → 3617..3644   （OP10 replace 7→28）
3111 ≤ N ≤ 4645      → N+534
4646 ≤ N ≤ 4650      → 5180..5193   （OP11 replace 5→14）
4651 ≤ N ≤ 4711      → N+543
4712 ≤ N ≤ 4723      → 5255..5267   （OP12 replace 12→13）
4724 ≤ N ≤ 5371      → N+544
5372 ≤ N             → N+545        （OP13 於 5371 之後插 1 行）
```
`AutoClean\uCleaning.cpp`（2922 → 2923）
```
N ≤ 569              → N
570  ≤ N ≤ 571       → 570..572     （OP14 replace 2→3）
N ≥ 572              → N+1          （OP15 於 1969→1970 原地 1→1 取代，不再位移）
```

### 8.5 LEDGER 要記的點

1. **相依鏈 #4 的地形**：V910 出廠版已含 `Common\PickPlanner`(7 檔)＋`cCleanKitPickPlan.h`，
   位元組相同且 `.bpr` 已登錄 → **本波零新增檔、零 `.bpr` 變更**；缺的只有 `AutoClean.cpp` 實作。
2. **20260514 的 6 條被併入本波**（同一片連續位元組，不可分割），LEDGER 的日期叢集要更新。
3. **`bUseCKPP` 預設 `false` 且無外部開關** → 15 個 op 中 12 個是休眠搬運。
4. **三個閘外行為變更**：OP6b（KYEC，§5.1，**唯一 <95%**）、OP14（pad count 歸零）、OP15（優先序）。
5. **`AutoClean.cpp` 是 bare-LF 檔**，後續任何人碰它都要注意（**絕不 `git checkout`**）。
6. **`ainarm9045_1x1_1.cpp` 兩條是全形破折號造成的假 MISSING**——
   這是繼 note.cpp:1027 之後第二個同型案例，**建議把「V899 Big5 全形標點 vs V910 ASCII 正規化」
   列為矩陣已知假陽性模式**，後續波遇到 `MISSING` 先用正規化比對確認。

---

## 9. 不屬本波（明確排除）

| 條目 | 日期 | 理由 |
|---|---|---|
| `cShowBinSelect.cpp:809`（AutoDeviceEjection panel for QLE） | 20260417 | UI 主題，留 UI 波（交派已註明） |
| `main.cpp:25658`（MNetLog 打包進 StateRecord） | 20260414 | N10-3 FTP 診斷主題，與 AutoClean 無關 |
| `note.cpp:1027`（SoftStop guard） | 20260407 | **已在 `mg_matrix_allowlist.csv:49` 由 MG-W3 處理**（狀態 `ALLOWLISTED`，本來就不在 missing.csv），本波無事可做 |
| `uCleaning.cpp:148-151`（Greatek default 60→1） | 20260423 | 別波（CASE-20260423-001） |
| `uCleaning.cpp:2650/2863/2867`（SmartAutoClean 不鎖機） | 20260703 | 別波（CASE-PTI-20260630-001） |
| `Common\PickPlanner\*` 28 條 | 20260514 | **矩陣狀態全部 `same-file`，本來就不是 MISSING** |
| `AutoClean\cCleanKitPickPlan.h` 9 條 | 0407/0408/0514 | 同上，全部 `same-file` |

---

## 10. 結論

- **28 條矩陣紀錄 → 17 個 splice op**（15 `replace`、2 `after`），只碰 2 個檔。
  錨點全部經全檔唯一性實測（17/17 唯一），43 組上下文行逐位元組驗證通過。
  共搬 **643 行 ＝ 27 行矩陣標記 ＋ 520 行未標記新程式 ＋ 96 行為錨點重插的既有行**。
- **交派三問的答案**：
  1. **V910 有 PickPlanner 目錄**，7 檔位元組相同、`.bpr` 已登錄 → **不新增檔、不改 `.bpr`**。
  2. **`bUseCKPP` 預設 `false`**（唯一定義處，全樹無第二次賦值、無 ini/UI 開關）。
  3. **PickPlanner 註解日期 100% 是 20260514**，**無任何 <20260323 的日期**，全 28 條矩陣狀態 `same-file`。
- **<95% 清單：只有 1 條 —— OP6b**（0407 `iCleanPadCount==0` 防卡死改到 `CC_KYEC_LEE` 行為，§5.1）。
  其餘 16 個 op 信心 ≥95%；**刪掉 OP6b 不影響任何其他 op**（已實測：+536 行、bcc32 0 error）。
- **另外兩個閘外行為變更**（OP14 pad count 歸零、OP15 運算子優先序）信心 ≥95%，但**必須進 LEDGER**。
- **本波最大的「不做」**：`ainarm9045_1x1_1.cpp` 兩條是全形破折號造成的假 MISSING，
  **不出 op，走 allowlist**——若照矩陣硬搬會把 V910 已有的正確碼重複一次。
