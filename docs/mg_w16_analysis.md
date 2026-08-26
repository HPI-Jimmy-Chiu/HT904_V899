# MG-W16 分析：雜項掃尾（20 條零星矩陣點，14 個來源檔）

- **波次**：MG-W16（V899 → V910），LEDGER 日期列 **0401 / 0410 / 0415＋0417 / 0505 / 0513 / 0515 / 0519 / 0520 / 0605 / 0618**
- **來源**：`docs/mg_ai_matrix_missing.csv` **20 條 MISSING**（全 40 條中屬本波的 20 條；另 20 條屬 MG-W15，兩波檔案零重疊）
- **CASE**：`CASE-20260515-001`（南茂 YieldChart site 標籤）、`CASE-20260519-001`（南茂 Server 溫度欄位相關批次）、
  `CASE-20260520-001`（PTI Fix3 auto limit）、`CASE-20260605-001`（PTI ClearSortCount 誤清）、
  `CASE-20260618-001`（南茂 Index Z2 Position Error）
- **V899 樹**：`D:\HT9045\HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422`
  （唯讀；本波 14 個來源檔 `git status --porcelain` 空、`git diff b515ed5 HEAD` 對這 14 檔全空 → 工作區＝在製收斂端點 `b515ed5`）
- **V910 樹**：`D:\HT9045\HT9011UC_Code_V3.33.910.0_20260716_Jimmy`（MG-W14 `75bd43f` 後，`git status` 乾淨、`*.mgbak` **0 個**）
- **本文件性質**：分析＋splice op 草案。**全程唯讀，兩棵樹一個位元組都沒有被修改**（模擬結果只寫進 scratchpad）
- **op 檔**：`D:\HT9045\docs\mg_w16_ops.json`（**8 個 op，6 個檔**）
- **結果分佈**：**A 類 7 條可搬** ／ **C 類 10 條入白名單** ／ **F 類 3 條進最終決策清單**（＝20 條全部有歸屬）

---

## ⚠ 開工前必須先知道的四件事

1. **本波一半以上是「假 MISSING」——公司在 V910 基線裡自己做過了。**
   10 條 C 類全部落在 V910 的 **pristine 基線 `e06524a`**（不是前面哪一波搬進去的，實測 `git show e06524a:...`）。
   公司改寫的四種形狀都出現了：**改寫成英文**（0513 兩條）、**換自家署名但程式碼位元組相同**（0515 三條、0519 BarCode）、
   **修好編碼**（0505 uhome，V899 那三行原本是 UTF-8 位元組塞在 Big5 檔裡）、**自己另寫一版**（0519 uTrayEditForm 兩條）。
   → 這些**不能照搬**，照搬只會製造重複或倒退。

2. **`cShowBinSelect.cpp` 三條是真衝突，而且 V910 的 `.dfm` 多一個 V899 沒有的控制項。**
   V910 `cShowBinSelect.dfm` 的 `tsIndex` 分頁多一顆 **`btnASM`（Top=229, Height=30）**，V899 的 `.dfm` **完全沒有這顆**（實測 0 命中）。
   V899 的動態堆疊從 `gbAutoCleanCount->Top+Height+4 = 108+121+4 = 233` 開始排——**正好壓在 btnASM 身上**。
   而 V910 line 879 的 `KevinCheng 20260415 : 避免遮住按鈕` 就是在修這件事（同一天、不同作者、不同解法）。→ §6.1 F12。

3. **`adam6024.cpp` 目前是「半搬」狀態，本波兩個 op 才把它接上。**
   公司把 `ShowDoubleEPConnectGuide()` 的**函式本體與旗標搬進 V910 了**（`adam6024.cpp:55/73`，連 `//AI … 20260505` 註解都在），
   但**兩個呼叫點一個都沒搬**，也沒把 `Address[Num]=IP;` 移到狀態檢查之前 → 這支函式在 V910 現在是**死碼**。
   同型還有 0417：`bAutoDeviceEjection` 全域（`cmydef.cpp:5061`）＋ini 載入（`database.cpp:742`）都在 V910，
   但**沒有任何讀取點** → 該設定在 V910 是 **write-only 死旗標**（見 §6.1 F12 的補充）。

4. **`adam6024.cpp:369` 是矩陣看不到的承重行。**
   矩陣只列 `adam6024.cpp:263` 一行。V899 的第二個 `ShowDoubleEPConnectGuide` 呼叫在 `:369`（TCP connect 失敗路徑）**沒有 `//AI` 標記**。
   漏搬不會編譯錯，但「連線導引」只剩一半觸發條件——這正是 §0.4 說的矩陣盲區。

---

## 0. 前置事實（全部實測）

### 0.1 檔案基本盤（**md5 前提**，套用前必須逐檔重驗）

> 行數＝`splitlines(keepends=True)` 的元素數（＝`splice.py` 用的索引基準）。

| 檔 | V899 行 / md5(12) | **V910 行 / md5(12)** | V910 EOL | cp950 | 本波處置 | 套用後 |
|---|---|---|---|---|---|---:|
| `SECSGEM\uHGemHT9045_SV.cpp` | 893 / `cd261f0de57b` | **1018 / `42663cfd221e`** | CRLF | OK | **搬（1 op）** | **1025** |
| `CosFunction.cpp` | 4430 / `a77b6cd3dc2e` | **4510 / `45cca3de1280`** | CRLF | OK | **搬（2 op）** | **4513** |
| `adam6024.cpp` | 3003 / `6b8d9072a628` | **3103 / `04a499cb14e5`** | CRLF | OK | **搬（2 op）** | **3107** |
| `csystem.cpp` | 23202 / `50973facff3f` | **25690 / `687c6590a046`** | CRLF | OK | **搬（1 op）**；已被 W14 動過 | **25691** |
| `HS_Function.cpp` | 4838 / `a2fa16b35de3` | **5395 / `a6851c4205f3`** | CRLF | OK | **搬（1 op）**；已被 W5/W14 動過 | **5396** |
| `Motor\mymotor.cpp` | 5936 / `0a1191a01591` | **6625 / `0d5211848509`** | CRLF | OK | **搬（1 op）** | **6630** |
| `cShowBinSelect.cpp` | 2780 / `161b62def2df` | **3046 / `f9f722046357`** | **LF（bare-LF 3045、末行無換行）** | **V899 端 26 個 U+FFFD** | **F12 不搬** | 3046 |
| `uhome.cpp` | 4910 / `56dfc1d9b3bd` | **5240 / `408bef507ae6`** | CRLF | **V899 端 136 個 U+FFFD** | C 類白名單 | 5240 |
| `acatchtray.cpp` | 7798 / `d80798e8d34c` | **9391 / `5c7290eb3701`** | CRLF | OK | C 類白名單 | 9391 |
| `asendic_Auto.cpp` | 2481 / `c3e8f4875bcb` | **2640 / `560400a5a53e`** | CRLF | OK | C 類白名單 | 2640 |
| `cContactCT.cpp` | 1327 / `36f9c95bb238` | **1330 / `19bea6b8b80a`** | CRLF | OK | C 類白名單 | 1330 |
| `BarCode\BarCode.cpp` | 11448 / `e0f84e98dc3d` | **12318 / `9558d6d2de5d`** | CRLF | OK | C 類白名單 | 12318 |
| `uTrayEditForm.cpp` | 774 / `434a9cc9d511` | **764 / `780a5c4026de`** | CRLF | OK | C 類白名單 | 764 |
| `cSortCT.cpp` | 1893 / `64585bd1ced1` | **1910 / `edb60ebc25fa`** | CRLF | OK | C 類白名單 | 1910 |

> 本文件所有 V910 行號都量在**上表 md5 對應的磁碟現狀**上。
> V910 `.mgbak` 為 0 個且 `git status -- <V910>` 乾淨 → `port_check.py` 會拿 `git HEAD` 當基準，與上表一致。
> `acatchtray.cpp` / `csystem.cpp` / `uhome.cpp` / `HS_Function.cpp` 的 md5 與 MG-W14 §7 預測的套用後行數
> （9391 / 25690 / 5240 / 5395）**逐檔吻合** → W14 已落地無漂移。
> **`cShowBinSelect.cpp` 是 V910 那 ~34 個 LF 檔之一**：若日後 F12 裁定要搬，`splice.py` 的 `dominant_eol` 會自動輸出 LF，
> 但**它的末行沒有換行符**，任何以「行數」對帳的工具要記得 3045 個 `\n` ＝ 3046 行。

### 0.2 範圍權威：V899 端無中間態被撤回

`git log bb69c60..b515ed5` 對本波 14 檔：`CosFunction.cpp`（`292bd72`/`f4cec0b`/`b515ed5`）、
`csystem.cpp`（`292bd72`/`6539e3b`/`89fdd2a`/`0d2fc78`）、`HS_Function.cpp`（`292bd72`）、
`Motor\mymotor.cpp`（`292bd72`）有後續 commit；其餘 10 檔在該區間**零 commit**（內容即 `bb69c60` 追蹤時的最終態）。
逐條 `git log -S` 確認引入點：0618 在 `292bd72`，其餘六條 A 類全在 `bb69c60`。
**本波 12 個 A 類 hunk 之外的內容（0703 TrayArm 極性、0630 State Record 等）已由 MG-W14 結清，未被本波觸碰。**

### 0.3 符號稽核（V910 全樹，排除 `.svn`）——**本波 0 個新符號、0 個新 `#include`、0 個 `.dfm` op**

A 類 7 條用到的符號全部已存在於 V910 且在目標 TU 可見（逐一實測）：

| 符號 | V910 宣告／首見處 | 目標檔看得到嗎 |
|---|---|---|
| `tcLBUp` / `tcLBDown` | `MachineType.h:648`（Frank 20241231） | ✓ `uHGemHT9045_SV.cpp:6` include `MachineType.h` |
| `Temperature.dLBTempHighSettingValue` / `LowSettingValue` | `cprod.h:1619-1620` | ✓ 同檔 include `cprod.h`，且 `Temperature.` 已用 6 次 |
| `HType.FT_8_TYPE` | 同檔 2003/2005/2007 已在用 | ✓ |
| **SVID 1474–1479** | **全樹 0 命中**（`SetSVDataPointer`／`AddSV` 皆無） | ✓ 無 SVID 撞號 |
| `CosFunction.bSortingBinTraywhenCleanOut` | `CosFunction.h:156`；`CosFunction.cpp` 已 5 處 | ✓ |
| `CosFunction.bUseFix3FullTray` | `CosFunction.h`；`CosFunction.cpp` 已 7 處 | ✓ |
| `ShowDoubleEPConnectGuide` / `bDoubleEPConnectGuideShown` | `adam6024.cpp:73` / `:55`（**static，定義在 263/365 之前**） | ✓ |
| `LastSet.iRealDummy` / `DUMMY` | `csystem.cpp` 已 37 處 | ✓ |
| `NewRecordProcess` | `cMyDB.h`；`HS_Function.cpp:3861` 已在用 | ✓（0519 是純註解，無符號需求） |
| `CC_ChipMos_ZHUBEI`(851) / `CC_ChipMos_TAINAN`(850) | `MachineType.h:209-210` | ✓ `Motor\mymotor.cpp:6` include `MachineType.h`，同檔已用 `CC_JCET`/`CC_GIGAS` |

### 0.4 未標記承重行（矩陣看不到，漏搬即壞或即無效）

本波 8 個 op 共讓 **24 行**進入 V910（port_check 判定為 added 的行數）：

| 類別 | 行數 |
|---|---:|
| 矩陣列出的 `//AI` 標記行 | **7** |
| **V899 新增但無 `//AI` 標記** → **矩陣盲區** | **17** |

**矩陣可見率 7/24 = 29.2%。** 四塊「一條註解帶一整段實作」：

| V899 位置 | 內容 | 矩陣只列 | 漏搬的後果 |
|---|---|---|---|
| `SECSGEM\uHGemHT9045_SV.cpp:361-366` | 6 行 `SetSVDataPointer(1474..1479)` | `:360`（純註解） | **功能等於沒搬**（SECS 收不到 LB Up/Down） |
| `adam6024.cpp:369` | 第二個 `ShowDoubleEPConnectGuide` 呼叫 | **一行都沒有** | connect 失敗路徑不出導引；V910 的 helper 仍是半死碼 |
| `csystem.cpp:15438-15440` | 重新加括號的 3 行條件式 | `:15437` | `&&` 沒有右運算元 → **編譯錯** |
| `CosFunction.cpp:3382` | `bSortingBinTraywhenCleanOut=true;` | `:3381`（純註解） | **功能等於沒搬** |

> 另 `Motor\mymotor.cpp:416-419`（`else if` ＋ `{` ＋ `iCheckZ=11000;` ＋ `}`）矩陣同樣只列 `:415`。
> 本波所有 op 都以**連續區段**抽取，上述行自然被包含。

---

## 1. 十個叢集的變更敘述

### 1.1 【A】0401 SECS：LB Up/Down 溫度與上下限 → **W16-01**

`uHGemHT9045_SV.cpp` 的 `SetSV()` 在 `1473 ESD Air Temperature` 之後補 6 個 SVID：
`1474/1475` = `RunInfo.ShowTempComp[tcLBUp/tcLBDown]`（ASCII），
`1476–1479` = LB 上下限（FT_8）。V910 現況止於 1473，1474 起是空號（全樹 0 命中）。

> **忠實搬運要注意的一點**：V899 的 **1478/1479（LB Down 上下限）指到與 1476/1477（LB Up）完全相同的兩個變數**
> （`Temperature.dLBTempHighSettingValue` / `dLBTempLowSettingValue`）——`cprod.h` 本來就只有這一組。
> 這是 V899 出貨現狀（很可能是刻意：LB 上下表面共用同一組設定），**照搬不改**，記 LEDGER 備查。

### 1.2 【A】0410 甬矽 FOREHOPE：Clean Out 時啟用 SortingBinTray → **W16-02**

`FUNC_CC_FOREHOPE_NINGBO()` 尾端加 `CosFunction.bSortingBinTraywhenCleanOut=true;`。
**閘門＝客戶 profile 本身**（`CUSTOMER_CODE==CC_FOREHOPE_NINGBO` 才會呼叫這支），其他客戶零影響。
消費端 `aoutarm.cpp:224` / `aoutarm9045.cpp:3639` / `asortarm.cpp:113,2609` / `csystem.cpp:14891` 在 V910 都在。
與既有知識 `ht9045-eventlog-csv-quoting-load-bearing`（甬矽＝`CC_FOREHOPE_NINGBO`＝790）一致。

### 1.3 【F】0415＋0417 `cShowBinSelect.cpp` UI 動態排版（3 條）→ **不搬，F12**

見 §6.1。三條是同一件事的三刀（面板顯示條件／面板堆疊定位／分頁高度），V910 有**同日、不同作者、不同解法**的既有實作。

### 1.4 【A＋C】0505（2 條，一搬一入白名單）

- **【A】`adam6024.cpp:263` → W16-04／W16-05**
  V899 把 `Address[Num]=IP;` **移到 `fCheckConnectStatus_ADAM6024(Num)` 之前**（否則導引訊息會用到上一輪的 IP），
  並在狀態檢查失敗時呼叫 `ShowDoubleEPConnectGuide(IP, Num, "Status check failed before TCP connect.")`；
  另在 TCP connect 失敗（`iCount>100`）路徑也呼叫一次（**V899:369，無 `//AI` 標記**）。
  V910 已有 helper 本體但**零呼叫點**，本波兩個 op 把它接上。**無客戶碼，全客戶生效** → §6.2 外溢 1。
- **【C】`uhome.cpp:1864` → 白名單**
  V910 `uhome.cpp:2051-2055` **已有完全相同的實作**：程式碼兩行（`if(INSTALL_OCR_YMot==eocrYMotInstal)` ＋
  `Cylinder[C_LoaderUpPress].Off();`）**位元組相同**；三行中文註解**文字相同**，差別只在**編碼**——
  V899 那三行是 **UTF-8 位元組躺在 Big5 檔裡**（`\xe9\xa0\x90...`，cp950 解不開，故矩陣的位元組簽章比不中），
  V910 是**正確的 cp950 Big5**。**V910 版本嚴格較佳，照搬會把亂碼帶回去。**
  > 任務書要求的「以位元組原樣搬運／若 V899 原檔本來就損壞則照原樣並記 LEDGER」已查證：
  > **V899 原檔確實本來就損壞**（`uhome.cpp` 全檔 136 個 U+FFFD），但因為 V910 已有修好版，**結論是不搬**。

### 1.5 【A＋C】0513 dummy 空跑不觸發 P18 人工補盤（3 條，一搬兩入白名單）

三處都是同一個 guard：`if(LastSet.iRealDummy!=DUMMY && (…P18 三條件…))`。

| 矩陣點 | V910 現況 | 判定 |
|---|---|---|
| `acatchtray.cpp:520` | **V910:734 已有**，註解被公司改寫成英文 `dummy run skip P18 manual Fail Auto Tray refill flow`（在 `e06524a` 基線內） | **C** |
| `asendic_Auto.cpp:203` | **V910:206 已有**，同樣英文化 | **C** |
| `csystem.cpp:15437` | **V910:16523 沒有 guard**（`ScanColorFixTrayStatus`，`Prod.iTrayType[i]==tTrayAuto` 分支） | **A → W16-06** |

### 1.6 【C】0515 南茂 YieldChart Site 標籤（3 條，CASE-20260515-001）→ 全數白名單

V899 的修法是 `'A' + (int)(ChangeToFloatNonPcnt(...))`：`ChangeToFloatNonPcnt` 回 float，`'A'+float` 走
`%c` 的變數引數會被提升成 double 傳入 → 印出亂碼／空白；強制轉 int 還原 V897 行為。

**V910 `cContactCT.cpp:202/242/247` 三行的程式碼部分與 V899 逐位元組相同**，只是行尾註解是
`//Steven 20260421 : cast to int for %c`（在 `e06524a` 基線內，比 V899 的 20260515 還早六天）。
**全檔剝註解正規化後 V899 與 V910 各 1222 行、diff 為空** → 兩棵樹的 `cContactCT.cpp` 程式碼完全相同。

> 與既有知識 `ht9045-divide-safety-int-to-float-regression` 一致（除法安全樣板誤把整數除法變浮點）。
> **不自行改成 `ChangeToIntNonPcnt`**——V910 全樹沒有這個函式，且 V899 終態就是 `(int)(ChangeToFloatNonPcnt(...))`。

### 1.7 【A＋C】0519（4 條，一搬三入白名單）

| 矩陣點 | 內容 | V910 現況 | 判定 |
|---|---|---|---|
| `HS_Function.cpp:3549` | **純註解**：GTK `info.txt` 改由呼叫端在 site 狀態同步後存檔 | `SaveCloseOpenSiteEven()` 整支（V899 3512-3550 vs V910 3825-3862）**除這一行外逐位元組相同** | **A → W16-07** |
| `BarCode\BarCode.cpp:4550` | `e2x2In2CCD` 缺碼判定改用 2x2 buffer mapping（4 行條件式） | **V910:4819-4822 已有**，條件式（空白正規化後）完全相同，註解是 `//Wei 20260519 : 需要修改 否則沒掃到不會異常` | **C** |
| `uTrayEditForm.cpp:504` | Timer 重新整理不要重觸發 Enter 截圖 | **V910:506-507 已有另一版**：`FormShow(this); bEnterSave=false;` | **C** |
| `uTrayEditForm.cpp:707` | 截圖失敗不要卡住 Tray Edit UI | **V910:688-735 已有另一版**：整段包 `try{...}catch(Exception &e){}` | **C** |

> `uTrayEditForm.cpp` 兩條的**殘差差異**（不搬，但記 LEDGER 備查，見 §5 R3/R4）：
> (a) V899 版在 `FormShow` 前後**保存並還原** `bEnterSave` 與 `CheckBox1->Checked`；V910 版直接把 `bEnterSave` 壓成 false，
>     而 `FormShow`（`uTrayEditForm.cpp:72`）會把 `CheckBox1->Checked` 設回 false
>     → **V910 在工作檔切換時會丟掉操作員的「No Save Image」勾選與待處理的 Enter 截圖**。
> (b) V899 版把四個資源指標 NULL 初始化、`catch(...)`、**清理放在 try 之外**；V910 把清理放在 try 內
>     → **V910 在例外時會洩漏 `pBmp`/`pCanvas`/`jpeg`/`hDc`**，且 `catch(Exception&)` 比 `catch(...)` 窄。
> 兩者都是「公司已用自己的方式滿足需求」，屬 C；要不要改進屬**新需求**，不在本戰役範圍。

### 1.8 【A】0520 PTI Fix3 auto limit（CASE-20260520-001）→ **W16-03**

`FUNC_CC_PTI()` 補 `CosFunction.bUseFix3FullTray=true;`（V899 行尾就帶 `//AI … 20260520` 標記）。
V910 的 PTI profile **完全沒有這一行**（其他 45 行程式碼與 V899 逐行相同，唯一差就是這條）。
閘門＝PTI profile，其他客戶零影響。

### 1.9 【C】0605 PTI ClearSortCount 誤清（CASE-20260605-001）→ 白名單

矩陣只剩 `cSortCT.cpp:11`（`//AI … 20260605: 改用 ShowMyMessageBox_YES_NO 需引入 mymessbox.h` ＋ `#include "mymessbox.h"`）。
**功能本體（`:576` / `:611` / `:613` 的 `bManualClear` 判斷與 YES/NO 確認框）在 V910 `:582 / :627 / :629` 已存在**（矩陣判 PRESENT）。
而 `#include "mymessbox.h"` V910 在 `:29` 就有（`//Sam 20171213`）→ **照搬只會多一個重複 include，功能零增益**。

### 1.10 【A / D 類邊緣】0618 南茂 Index Z2 Position Error（CASE-20260618-001）→ **W16-08**

`TMyMotor` 建構子的 `iCheckZ` 判斷鏈，在 `CC_GIGAS`（20260512 已在 V910）之後補一段：

```
else if(CUSTOMER_CODE==CC_ChipMos_ZHUBEI || CUSTOMER_CODE==CC_ChipMos_TAINAN)
{
    iCheckZ=11000;
}
```

南茂 `e360KG` 原本落進 `else` 拿到 `iCheckZ=4000`，門檻過緊 → Z2 到位較慢就提早觸發 `Z1UpZ2Down1` alarm。

**D 類邊緣判定（任務書指定必查）**：
- **搬移範圍只含這一則註解所屬的完整變更**：V899 `[415,419]` 5 行，**不含任何其他馬達碼**。
  同檔的其他 V899↔V910 差異（`iGali_FindZPhaseTask[0..3]`、`bSh1/2FloodgateOpenWaiting`、
  V910 移除的 `InitializeCriticalSection(&g_cs)`）**一律不碰**——它們不在本波矩陣範圍內。
- **錨點乾淨**：`else if(INDEX_PRESS_TYPE==e85KG || INDEX_PRESS_TYPE==e240KG ||` 在 `Motor\mymotor.cpp` **全檔唯一**。
- **忠實**：只是在 if-else 鏈中間插一個分支，**不改任何既有分支**（`iCheckZ` 三個既有取值 11000/9000/4000 都不動）。
- **只放寬兩個南茂客戶碼**（850/851）；其他客戶指令級零變更。
→ **忠實搬運且錨點乾淨 ⇒ 依 skill 決策規則「D 類：忠實搬運且錨點乾淨＝可執行」**。

---

## 2. 逐 op 明細（`docs/mg_w16_ops.json`，8 op）

> 「錨點命中」＝該字串（或多行組合）在 V910 該檔**全檔**的命中次數（實測）。`splice.py` 同檔由**下往上**套用。
> `pay`／`rep`／`net` ＝ 抽取行數／被取代行數／淨增。**8 個錨點全部全檔唯一。**

| op | 矩陣行 | V899 範圍 | V910 動作 | 錨點（命中） | pay/rep/net |
|---|---|---|---|---|---|
| **W16-01** | `uHGemHT9045_SV.cpp:360` | `360-366` | `after 365` | `SetSVDataPointer(1473 , HType.ASCII_TYPE,   "ESD Air Temperature"` → **1** | 7/0/**+7** |
| **W16-02** | `CosFunction.cpp:3381` | `3380-3383` | `replace [3422,3423]` | 跨行：`bUseDefineAutoCleanOffset…//Ifor 20181106…` ＋ `}` → **1**（單行版命中 2，故用跨行） | 4/2/**+2** |
| **W16-03** | `CosFunction.cpp:1466` | `1464-1466` | `replace [1506,1507]` | 跨行：`bAdaptiveYield…//Sam 20230914…` ＋ `bShowYieldMonitor…` → **1**（`bShowYieldMonitor` 單行命中 4） | 3/2/**+1** |
| **W16-04** | `adam6024.cpp:263` | `263-269` | `replace [263,266]` | `if(!fCheckConnectStatus_ADAM6024(Num))  //Hmy 20170120 add check Adam6024 Connect Status ->` → **1** | 7/4/**+3** |
| **W16-05** | （無標記，矩陣盲區） | `369-369` | `before 365` | `ShowMyMessage("Connect Fail! Please Check ADAM IP!", IP, fAdam6024->ADAMErrorMessage[iRet]);` → **1** | 1/0/**+1** |
| **W16-06** | `csystem.cpp:15437` | `15437-15440` | `replace [16523,16525]` | `IniConfig.bP18FailAutoTrayManual==true ||                //jou 2012-03-16 Fail Auto Tray` → **1** | 4/3/**+1** |
| **W16-07** | `HS_Function.cpp:3549` | `3549-3549` | `after 3861` | `NewRecordProcess("MES2105", "Close Open Site", asTmp);` → **1** | 1/0/**+1** |
| **W16-08** | `Motor\mymotor.cpp:415` | `415-419` | `before 373` | `else if(INDEX_PRESS_TYPE==e85KG || INDEX_PRESS_TYPE==e240KG ||` → **1** | 5/0/**+5** |
| | | | | **TOTAL** | 32/11/**+21** |

**W16-02／W16-03 的 replace 上界拉大只為取唯一錨**：被含進來的 V910 行
（`CosFunction.cpp:3422`、`:3423`、`:1506`、`:1507`）**與 V899 的 `3380`/`3383`/`1464`/`1465` 逐位元組相同**（實測），
因此 port_check 的 `removed` 對 `CosFunction.cpp` 為 **0**。

**W16-04／W16-05 的套用順序**：`splice.py` 依 `anchor_line`／`replace_lines[0]` 由大到小排序 → 先 365 再 263，
行號互不位移（模擬已驗證）。

**兩個 replace 的多行錨含 Big5 中文**（`新增Auto Clean Offset By 本機` / `自適應性良率監控`）。
`splice.py` 是 `anchor_expect.encode("cp950", errors="ignore")`——已實測這兩串 **cp950 round-trip 無損**
（`errors="ignore"` 與嚴格編碼結果相同），不會因為 `ignore` 掉字元而誤配。

---

## 3. 事前模擬（記憶體內套用；輸出只寫進 scratchpad，V910 樹未動）

### 3.1 套用結果

| 檔 | 套用前 | 套用後 | Δ | EOL | bare-LF | cp950 strict | U+FFFD | mojibake(嚙) | `{}`／`()` 差值 |
|---|---:|---:|---:|---|---:|---|---|---|---|
| `SECSGEM\uHGemHT9045_SV.cpp` | 1018 | **1025** | +7 | CRLF | **0** | OK | 0→0 | 0→0 | SAME / SAME |
| `CosFunction.cpp` | 4510 | **4513** | +3 | CRLF | **0** | OK | 0→0 | 0→0 | SAME / SAME |
| `adam6024.cpp` | 3103 | **3107** | +4 | CRLF | **0** | OK | 0→0 | 0→0 | SAME / SAME |
| `csystem.cpp` | 25690 | **25691** | +1 | CRLF | **0** | OK | 0→0 | 0→0 | SAME / SAME |
| `HS_Function.cpp` | 5395 | **5396** | +1 | CRLF | **0** | OK | 0→0 | 0→0 | SAME / SAME |
| `Motor\mymotor.cpp` | 6625 | **6630** | +5 | CRLF | **0** | OK | 0→0 | 0→0 | SAME / SAME |

**8 個 op 的錨點檢查全部通過（FAIL COUNT = 0）**；每檔行數增減與 op 算術逐檔對帳相符（合計 +21）。
`{}`／`()` 差值採「**先 cp950 解碼再數字元**」（Big5 尾位元組可能是 `0x7B`/`0x7D`），套用前後完全相同。

### 3.2 簽章對帳

模擬結果中本波 **7 條**簽章各出現 **1 次**，一條不多一條不少：

| 日期 | 檔 : V899 行 | 簽章摘要 |
|---|---|---|
| 20260401 | `SECSGEM\uHGemHT9045_SV.cpp:360` | Add LB Up/Down temperature and limits for SECS collection |
| 20260410 | `CosFunction.cpp:3381` | enable SortingBinTray when CleanOut for FOREHOPE_NINGBO |
| 20260505 | `adam6024.cpp:263` | set target IP before status check so guidance uses the field-selected address |
| 20260513 | `csystem.cpp:15437` | dummy 空跑不檢查人工補 Fail Auto Tray 的 Color Sensor |
| 20260519 | `HS_Function.cpp:3549` | GTK info.txt is saved by callers after site-state synchronization. |
| 20260520 | `CosFunction.cpp:1466` | enable E55 Fix3 full tray gate for PTI import plan |
| 20260618 | `Motor\mymotor.cpp:415` | CASE-20260618-001 南茂(CC_ChipMos)/e360KG … 放寬 iCheckZ 至 11000 |

### 3.3 忠實度驗證

- **每個 op 的 payload 在結果中恰好出現 1 次**（8/8）。
- **關鍵區段位元組相等**（逐段實測 IDENTICAL）：

  | 區段 | 行數 |
  |---|---:|
  | `uHGemHT9045_SV.cpp` LB Up/Down 6 SVID ＋ AI 註解（V899 360-366） | 7 |
  | `CosFunction.cpp` FOREHOPE_NINGBO 尾段（V899 3380-3383） | 4 |
  | `CosFunction.cpp` PTI `bUseFix3FullTray`（V899 1464-1466） | 3 |
  | `adam6024.cpp` `Open_ADAM_6024` 狀態檢查段（V899 261-269） | 9 |
  | `adam6024.cpp` connect fail 導引段（V899 362-373） | 12 |
  | `csystem.cpp` `ScanColorFixTrayStatus` P18 guard（V899 15435-15441） | 7 |
  | `HS_Function.cpp` `SaveCloseOpenSiteEven` **整支**（V899 3512-3550） | 39 |
  | `Motor\mymotor.cpp` `iCheckZ` 判斷鏈**整段**（V899 406-428） | 23 |

- **V910 專屬內容零遺失**（出現次數 before → after，實測）：
  `ATC Arm1 Head 1` 4→4、`Magazine 1 Count` 4→4、`SetSVDataPointer(1473` 1→1、
  `AI(ht9045-v899) 20260804` 9→9、`bUseDefineAutoCleanOffset` 7→7、`bShowYieldMonitor` 4→4、`bAdaptiveYield` 4→4、
  `bDoubleEPConnectGuideShown` 3→3、`Address[Num]` 15→15、`AI(ht9045-v899) 20260610` 3→3、
  `AI(ht9045-v899) 20260526` 16→16、`Eastsun 20260513 F011` 10→10、`SwRKSafeLock` 4→4、
  `Eastsun 20260511` 47→47、`AI(BVL-3766) 20260420` 1→1、`iGali_FindZPhaseTask` 4→4、`AI(ht9045-v899) 20260512` 1→1。
  預期增加者：`bSortingBinTraywhenCleanOut` 5→6、`bUseFix3FullTray` 7→8、
  `ShowDoubleEPConnectGuide` **1→3**（死碼接上）、`iRealDummy` 37→38、`iCheckZ` 6→9、`CC_GIGAS` 2→3。

### 3.4 removed 3 行全部是刻意取代（逐條列出供主腦對照）

| 檔 | V910 行 | 內容 | 為什麼被換掉 |
|---|---:|---|---|
| `adam6024.cpp` | 266 | `Address[Num]=IP;   //Ifor 20150709 ：開啟ADAM 設備時將IP位置寫入暫存器` | 同一句被移到 263（狀態檢查之前），行尾註解換成 V899 的 `//AI … 20260505` |
| `csystem.cpp` | 16523 | `if(IniConfig.bP18FailAutoTrayManual==true ||   //jou 2012-03-16 …` | 換成加了 `iRealDummy!=DUMMY &&` 與外層括號的版本 |
| `csystem.cpp` | 16525 | `((iRunStartMode==RT …RT==true))   //Steven 20150116 …` | 同上（多一個 `)` 收外層括號） |

### 3.5 port_check 預測（模擬檔跑同一套分類邏輯）

```
[CosFunction.cpp             ] added=3   (spliced=3,  authored=0) removed=0
[HS_Function.cpp             ] added=1   (spliced=1,  authored=0) removed=0
[Motor\mymotor.cpp           ] added=5   (spliced=5,  authored=0) removed=0
[SECSGEM\uHGemHT9045_SV.cpp  ] added=7   (spliced=7,  authored=0) removed=0
[adam6024.cpp                ] added=5   (spliced=5,  authored=0) removed=1
[csystem.cpp                 ] added=3   (spliced=3,  authored=0) removed=2
TOTAL added=24 spliced=24 authored=0 removed=3
```

**預期 `authored=0`、`comment=0`、`removed=3`、EOL 全 CRLF 無混用。**

---

## 4. gate 形狀（**照 V899 出貨現狀，本波不新增任何客戶碼判斷**）

| 變更 | 閘門 | 非目標客戶的行為 |
|---|---|---|
| LB Up/Down SVID（W16-01） | **無客戶碼**，但只是**註冊 SV 指標**；資料要有值需 `LB_TEMP_UpDown`（`main.cpp:20909-20910` 的 `bUT150Install[tcLBUp/Down]`）＋ SECS host 主動查詢 | 沒裝 LB 上下溫控的機台：SV 讀到空字串／0.0；**不影響控制流程** |
| SortingBinTray（W16-02） | **`FUNC_CC_FOREHOPE_NINGBO()` 內**（V899 原生 profile 閘） | 其他客戶指令級零變更 |
| PTI Fix3 full tray（W16-03） | **`FUNC_CC_PTI()` 內**（V899 原生 profile 閘） | 其他客戶指令級零變更 |
| Double EP 連線導引（W16-04/05） | **無客戶碼，全客戶** | **外溢**，見 §6.2-1 |
| dummy 空跑 Color Sensor guard（W16-06） | **無客戶碼**，判斷式是 `LastSet.iRealDummy!=DUMMY`（跑 dummy 才改變行為） | 非 dummy 執行時**指令級零變更**（`A && B` 的 A 恆真） |
| GTK info.txt 註解（W16-07） | 純註解 | **零指令變更** |
| 南茂 iCheckZ（W16-08） | **`CUSTOMER_CODE==CC_ChipMos_ZHUBEI \|\| CC_ChipMos_TAINAN`**（V899 原生） | 走原 `else if`/`else` 路徑，指令級零變更 |

**七條裡有五條是「客戶碼／dummy 旗標／純註解」天然收斂，只有 Double EP 導引一條真正全客戶生效。**

---

## 5. 反向發現（V910 有、V899 沒有）——**不可回搬**

| # | 位置 | V899 | V910 | 本波處置 |
|---|---|---|---|---|
| **R1** | `cShowBinSelect.dfm` `tsIndex` | **沒有 `btnASM`** | 有 `btnASM`（Auto Site Mapping，Top=229 H=30） | **保留**。這是 F12 的核心（V899 的堆疊起點 233 正好壓上它） |
| **R2** | `cShowBinSelect.cpp:869-888` | `if(palAutoDeviceEjection) Visible=(SPIL_FOR_QLE==1 \|\| bAutoDeviceEjection==1);` | `CUSTOMER_CODE==CC_GIGAS \|\| SPIL_FOR_QLE==1` ＋ `KevinCheng 20260415 : 避免遮住按鈕` 的 btnASM 定位 | **保留**（F12）。照搬會刪掉 GIGAS 分支與避讓 btnASM 的定位 |
| **R3** | `uTrayEditForm.cpp:506-507` | 保存／還原 `bEnterSave`＋`CheckBox1->Checked` | `FormShow(this); bEnterSave=false;`（`Steven 20260519`） | **保留**（C）。殘差差異記 §1.7 |
| **R4** | `uTrayEditForm.cpp:688-735` | 指標 NULL 初始化＋`catch(...)`＋清理在 try 外 | 整段包 try、`catch(Exception &e)`、清理在 try 內（`Steven 20260519`） | **保留**（C）。例外時洩漏資源，記 §1.7(b) |
| **R5** | `cContactCT.cpp:202/242/247` | 行尾 `//AI … 20260515 …`（含根因說明） | 行尾 `//Steven 20260421 : cast to int for %c` | **保留**（C）。程式碼位元組相同 |
| **R6** | `BarCode\BarCode.cpp:4819-4822` | 特殊縮排（多 10 個空白）、無空行 | 正規縮排、`if` 區塊間有空行、`//Wei 20260519` | **保留**（C）。條件式邏輯相同 |
| **R7** | `uhome.cpp:2051-2053` | 註解是 UTF-8 位元組（cp950 解不開） | 同文字的**正確 cp950 Big5** | **保留**（C）。V910 較佳 |
| **R8** | `csystem.cpp` `ScanFixTrayStatus`（本波 op 之外） | 有 `FIX3_FULL_PLACE==Fix3K_UseCylinder46LA && i==eFix2 &&` 這段條件 | **沒有** | **不碰**（無 `//AI` 標記，不在 899 期矩陣範圍）；記 LEDGER 供日後盤點 |
| **R9** | `Motor\mymotor.cpp` 建構子 | 有 `InitializeCriticalSection(&g_cs);` | **沒有**；另多 `iGali_FindZPhaseTask[0..3]`、`bSh1/2FloodgateOpenWaiting` | **不碰**（不在本波範圍，且屬 D 類馬達碼——任務書明令不順手動） |

---

## 6. <95% 清單、C 類白名單與外溢

### 6.1 建議寫入 `MG_FINAL_DECISIONS.md` 的項目：**3 條（同一根因，合併為 F12）**

> （本文件全程唯讀，**未動** `MG_FINAL_DECISIONS.md`。下表是可直接貼上的 F 列草稿。）

**F12 — `cShowBinSelect.cpp` 的 tsIndex 版面（矩陣 `:809`／`:815`／`:1491`，3 條）**

| | V899 最終態 | V910 現狀 |
|---|---|---|
| **面板顯示條件**（V899:809-813 / V910:869-888） | `if(palAutoDeviceEjection) Visible=(SPIL_FOR_QLE==1 \|\| bAutoDeviceEjection==1);` | `if(CUSTOMER_CODE==CC_GIGAS \|\| SPIL_FOR_QLE==1)` → Visible=true；`else` Visible=false。另在 Panel1／pnlSpeciallYield 都不可見時定位：`SPIL_FOR_QLE==1` → `btnASM->Top+Height+10`（**`KevinCheng 20260415 : 避免遮住按鈕`**），否則 `gbAutoCleanCount->Top+Height+10` |
| **面板堆疊**（V899:815-841） | 從 `gbAutoCleanCount->Top+Height+4 = 233` 起，依序重排 `Panel1`／`pnlSpeciallYield`／`gbTriggerAlm`／`palAutoDeviceEjection`／`gbCopyRecipe` | 無（各面板用 `.dfm` 設計期 Top：264/305/352/473/560） |
| **分頁高度**（V899:1490-1504 / V910:1554-1555） | 由可見面板堆疊末端動態算，下限 350：`Height = iBottomIndex + btReturn->Height + 60` | 固定 `Height=450`（`Sam 20211221 : 調整高度`） |
| **`.dfm`** | `tsIndex` **無 `btnASM`** | `tsIndex` **有 `btnASM`**（Top=229, H=30） |

**照搬的三個代價**：
(a) **`btnASM` 會被壓住**——V899 的堆疊起點 233 落在 btnASM 的 229–259 之內，等於**把 V910 `KevinCheng 20260415` 修的 bug 原樣搬回去**；
(b) **GIGAS 機台可能失去 `palAutoDeviceEjection`**——V899 的條件沒有 `CC_GIGAS`，改用 `bAutoDeviceEjection` ini 旗標，
    而 `database.cpp:742` 的讀取預設值是 **0**（`CheckAndReadIniDataGeneral("System","bAutoDeviceEjection",0)`）
    → 除非現場已在 `Gerneral.ini` 明寫 1，否則 GIGAS 機台的該面板會從「一定顯示」變成「不顯示」
    （**現場 ini 實際值未查證，`system\` 屬硬邊界不在本波讀取範圍**）；
(c) **分頁高度對全客戶改變**——`gbCopyRecipe` 在 `.dfm` 設計期 `Visible` 預設 true、Top=560 H=65，
    不套堆疊而只套動態高度會算出 `629+25+60 = 714`（V910 現為 450），**在 768/1024 高的機台可能超出畫面**。
**三者互相耦合、沒有安全的機械合併解 → 需使用者裁定。**

**補充事實（支持「還是要處理」的一方）**：V910 已經把 0417 的**基礎建設搬進去了**——
`bAutoDeviceEjection` 全域（`cmydef.cpp:5061`，連 `//AI … 20260417` 註解都在）＋ ini 載入（`database.cpp:742`），
但**全樹沒有任何讀取點** → 該設定在 V910 現在是 **write-only 死旗標**。

候選寫法（供裁決參考，**本波未採用，且屬 authored code 違反 splice 鐵律**）：
```
if(CUSTOMER_CODE==CC_GIGAS || SPIL_FOR_QLE==1 || bAutoDeviceEjection==1)   // 只擴充條件，V910 定位邏輯全留
```
→ 這樣可以單獨解掉 `:809` 一條（讓 ini 旗標活過來），`:815`／`:1491` 兩條仍建議不搬。

**建議**：(a) 三條全部不搬（尊重 V910 同日的 `KevinCheng 20260415` 解法），或
(b) 只用上面那行擴充條件解 `:809`，`:815`／`:1491` 不搬。**預設先行動作＝(a)，本波未改任何 `cShowBinSelect` 位元組。**

> F 列草稿（貼進 `MG_FINAL_DECISIONS.md` 表尾）：
>
> `| F12 | MG-W16 的 cShowBinSelect.cpp:809/815/1491（tsIndex 動態排版，0415＋0417） | **V910 有同日不同作者的競品解法**（JerryYang 20251020 + KevinCheng 20260415「避免遮住按鈕」），且 V910 的 .dfm 多一顆 V899 沒有的 btnASM(Top=229,H=30)——V899 的堆疊起點 233 正好壓上它；另 V899 條件刪掉 CC_GIGAS 分支、動態高度會把 450 變 714 | (a) 三條全不搬 (b) 只把 :809 擴成 `CC_GIGAS \|\| SPIL_FOR_QLE==1 \|\| bAutoDeviceEjection==1`（讓 V910 已載入卻無人讀的 bAutoDeviceEjection 旗標活過來），:815/:1491 不搬 (c) 全搬 | **建議 (a)**，已照建議執行（未動任何 cShowBinSelect 位元組）；若確認要讓 bAutoDeviceEjection 生效再走 (b) |`

### 6.2 C 類白名單建議行（**CSV 格式，可直接附加到 `docs/mg_matrix_allowlist.csv`**）

> 欄位＝`file,date,comment_prefix,reason,wave`；比對規則＝同檔同日且 `comment` 以 prefix 開頭（`ai_comment_matrix.py:88`）。
> **10 條全部實測過「V910 已有等價或更佳實作，且在 pristine 基線 `e06524a` 內」**。

```csv
uhome.cpp,20260505,//AI(ht9045-v899) 20260505:,MG-W16 假MISSING：V910:2051-2055 程式碼位元組相同、三行註解文字相同，差別只在 V899 那三行是 UTF-8 位元組躺在 Big5 檔（136 個 U+FFFD），V910 已修成正確 cp950；照搬會把亂碼帶回去,MG-W16
acatchtray.cpp,20260513,//AI(ht9045-v899) 20260513:,MG-W16 假MISSING：V910:734 已有同一 iRealDummy!=DUMMY guard，公司把註解改寫成英文（在 e06524a 基線內）,MG-W16
asendic_Auto.cpp,20260513,//AI(ht9045-v899) 20260513:,MG-W16 假MISSING：V910:206 已有同一 guard，公司英文化註解（在 e06524a 基線內）,MG-W16
cContactCT.cpp,20260515,//AI(ht9045-v899) 20260515:,MG-W16 假MISSING：V910:202/242/247 的 (int)(ChangeToFloatNonPcnt(...)) 與 V899 逐位元組相同，行尾署名為 //Steven 20260421；全檔剝註解正規化後兩樹各 1222 行 diff 為空,MG-W16
BarCode\BarCode.cpp,20260519,//AI(ht9045-v899) 20260519: match e2x2In2CCD,MG-W16 假MISSING：V910:4819-4822 條件式（空白正規化後）與 V899 相同，署名 //Wei 20260519,MG-W16
uTrayEditForm.cpp,20260519,//AI(ht9045-v899) 20260519: keep timer refresh,MG-W16 C類：V910:506-507 公司自寫等價版（FormShow 後強制 bEnterSave=false，Steven 20260519）；殘差＝V910 會丟掉操作員 CheckBox1 勾選與待處理 Enter 截圖，記 LEDGER 不搬,MG-W16
uTrayEditForm.cpp,20260519,//AI(ht9045-v899) 20260519: keep screenshot save,MG-W16 C類：V910:688-735 公司自寫等價版（整段包 try/catch(Exception&)，Steven 20260519）；殘差＝例外時洩漏 pBmp/pCanvas/jpeg/hDc，記 LEDGER 不搬,MG-W16
cSortCT.cpp,20260605,//AI(ht9045-v899) 20260605: 改用 ShowMyMessageBox_YES_NO 需引入 mymessbox.h,MG-W16 假MISSING：功能本體(:582/:627/:629)V910 已有；#include "mymessbox.h" V910:29 早就存在（//Sam 20171213），照搬只會多一個重複 include,MG-W16
```

> **注意**：`cSortCT.cpp` 20260605 共 4 條註解，只有 `:11` 是 MISSING；`:613` 的註解開頭
> 也是 `//AI(ht9045-v899) 20260605: 改用 ShowMyMessageBox_YES_NO `，所以上表刻意用**完整字串**當 prefix，
> 避免日後 `:613` 若回歸 MISSING 被誤蓋。
> `cContactCT.cpp` 那一列的 prefix 只到日期冒號，是刻意的——三條全部都是 C。

### 6.3 必須記入 LEDGER 的外溢（**不是阻塞項，但要留痕**）

1. **【最高】Double EP 連線導引對全客戶生效（W16-04/05，無客戶碼）。**
   兩處失敗路徑（狀態檢查失敗、TCP connect 連續失敗 >100 次）會新增一個**一次性訊息框**
   （`bDoubleEPConnectGuideShown` 每次執行只顯示一次）。
   V910 現況是「helper 存在但零呼叫＝死碼」，搬入後才會真的跳。
   同時 `Address[Num]=IP;` **提前執行**：即使狀態檢查失敗、提早 `return false`，`Address[Num]` 也已被寫入
   ——這是 V899 刻意的（導引訊息要顯示現場選的 IP），但它讓 `Address[]` 在連線失敗時也被更新。
   後續 `ADAMTCP_Read6KAI(Address[Num]...)` 等呼叫在 `bADAM6420Install` 為 false 時不會走到，**風險低但要留痕**。
2. **`csystem.cpp` 的 dummy guard 讓 dummy 空跑不再檢查 Auto 盤 Color Sensor（無客戶碼）。**
   非 dummy 完全不變；跑 dummy 時少一組 `WAR1151/1251/1351`。V899 出貨版即如此。
3. **LB SVID 1478/1479 與 1476/1477 共用同一組變數**（`Temperature.dLBTempHigh/LowSettingValue`）。
   SECS host 讀 LB Down 上下限會拿到與 LB Up 相同的值。**V899 出貨現狀，照搬**；要分開屬**新需求**（需 `cprod.h` 新欄位）。
4. **`Motor\mymotor.cpp` 的 `iCheckZ` 對兩個南茂客戶碼由 4000 放寬到 11000（D 類）。**
   `iCheckZ` 是 Z 軸到位容許範圍，**放寬＝檢查變鬆**：`Z1UpZ2Down1` 這類保護 alarm 的觸發門檻變高。
   僅 `CC_ChipMos_TAINAN`(850)／`CC_ChipMos_ZHUBEI`(851) 受影響，且與既有 `CC_GIGAS`／`CC_JCET` 的處置一致。
   V899 出貨版即如此（CASE-20260618-001 客戶已驗證）。
5. **`bAutoDeviceEjection` 在 V910 仍是 write-only 死旗標**（F12 不搬的直接後果）。
   `Gerneral.ini` 的 `[System] bAutoDeviceEjection` 設 1 在 V910 **不會有任何效果**。若現場已有客戶在用，需走 F12 選項 (b)。
6. **`uTrayEditForm.cpp` 的兩個殘差**（§1.7）：V910 會在工作檔切換時丟掉 `CheckBox1`（No Save Image）勾選；
   `SaveJPG` 例外時洩漏四個資源。屬 V910 既有狀態，本波不改，記錄備查。
7. **`csystem.cpp` `ScanFixTrayStatus` 的 `Fix3K_UseCylinder46LA` 條件 V910 沒有**（R8）。
   無 `//AI` 標記、不在 899 期矩陣範圍，本波不碰；日後若要做「非矩陣差異盤點」是候選項。

---

## 7. 收工 gate 建議（給主腦）

1. **硬前提（套用前先驗，任一不符就停）**——見 §0.1 的 md5 表。六個要動的檔：

   | 檔 | 必須是 | md5(12) | 為什麼是重點 |
   |---|---:|---|---|
   | `csystem.cpp` | **25690** 行 | `687c6590a046` | 已被 MG-W14 動過（W14 預測值即 25690，已對上） |
   | `HS_Function.cpp` | **5395** 行 | `a6851c4205f3` | 已被 MG-W5／W14 動過 |
   | `adam6024.cpp` | **3103** 行 | `04a499cb14e5` | 兩個 op、跨 100 行，順序靠 splice 的由下往上 |
   | `CosFunction.cpp` | **4510** 行 | `45cca3de1280` | 兩個 op 都用多行錨 |
   | `SECSGEM\uHGemHT9045_SV.cpp` | **1018** 行 | `42663cfd221e` | pristine |
   | `Motor\mymotor.cpp` | **6625** 行 | `0d5211848509` | pristine；D 類馬達檔 |

   （`splice.py` 會因唯一錨不符而**整檔中止**，是安全失敗。）

2. `python tools/port_tools/splice.py docs/mg_w16_ops.json`
   - 預期輸出（6 檔，全部 `eol=CRLF`）：
     ```
     OK SECSGEM\uHGemHT9045_SV.cpp: 1 op(s) applied, eol=CRLF
     OK CosFunction.cpp: 2 op(s) applied, eol=CRLF
     OK adam6024.cpp: 2 op(s) applied, eol=CRLF
     OK csystem.cpp: 1 op(s) applied, eol=CRLF
     OK HS_Function.cpp: 1 op(s) applied, eol=CRLF
     OK Motor\mymotor.cpp: 1 op(s) applied, eol=CRLF
     ```
   - 套用後行數：**1025 / 4513 / 3107 / 25691 / 5396 / 6630**
   - 產生 6 個 `.mgbak`（目前樹上 `.mgbak` 為 0 個）

3. `python tools/port_tools/port_check.py`（不給參數即可）
   - 預期與 §3.5 的表**逐檔相同**：`added` 全數 SPLICED、`authored=0`、`comment=0`、
     `removed` 合計 **3**、EOL 全 CRLF 無 bare-LF。
   - **REMOVED 3 行的逐條清單見 §3.4**。

4. `bcc32 -c -H-` 單檔隔離編譯 **6 個 .cpp**：`SECSGEM\uHGemHT9045_SV.cpp`、`CosFunction.cpp`、
   `adam6024.cpp`、`csystem.cpp`、`HS_Function.cpp`、`Motor\mymotor.cpp`。
   §0.3 已把可預見的編譯風險（符號可見性、`#include` 缺漏、SVID 撞號、名稱衝突）逐項排除，仍請實編確認。
   **最需要編譯背書的兩支**：
   - `SECSGEM\uHGemHT9045_SV.cpp` —— 驗 `tcLBUp`/`tcLBDown`/`Temperature.dLBTemp*SettingValue` 四個符號在該 TU 可見
   - `csystem.cpp` —— 驗 3 行取代後括號平衡（`if(A && (B || C || D))` 的外層括號）

5. 主腦逐條開檔複驗，**重點五處**：
   - `adam6024.cpp` 套用後 **`Open_ADAM_6024(IP,Num)` 內只剩一句 `Address[Num]=IP;`**（在 `fCheckConnectStatus` 之前），
     且 `ShowDoubleEPConnectGuide` 全檔命中 **3**（1 定義 ＋ 2 呼叫）。
   - `csystem.cpp` 套用後 `ScanColorFixTrayStatus` 的判斷式是
     `if(LastSet.iRealDummy!=DUMMY && (IniConfig.bP18FailAutoTrayManual==true || … || …))`，括號收在 `true)))`。
   - `Motor\mymotor.cpp` 套用後 `iCheckZ` 判斷鏈是 **JCET/160KG → GIGAS → ChipMos → 85/240/120/160KG → else**
     五段，且**既有四段一行未動**（`InitializeCriticalSection` 仍然不存在＝V910 原狀，勿順手補）。
   - `CosFunction.cpp` 套用後 `bUseDefineAutoCleanOffset` 仍 **7** 次、`bShowYieldMonitor` 仍 **4** 次
     （＝多行錨取代沒有誤刪 V910 原行），`bSortingBinTraywhenCleanOut` 由 5→**6**、`bUseFix3FullTray` 由 7→**8**。
   - `uHGemHT9045_SV.cpp` 套用後 1473 之後緊接 1474…1479，再接 1702；`Magazine 1 Count`（V910 專屬）仍在。

6. **白名單**：把 §6.2 的 8 列 CSV 附加到 `docs/mg_matrix_allowlist.csv`（**10 個矩陣點**，`cContactCT` 一列涵蓋 3 條、
   `uTrayEditForm` 兩列各 1 條）。

7. 收工後 `python tools/port_tools/ai_comment_matrix.py` 重算
   → 本波 **7 條轉 PRESENT ＋ 10 條轉 ALLOWLISTED**，**`cShowBinSelect.cpp:809/815/1491` 仍為 MISSING（設計如此，見 §6.1）**。
   - **只驗「本波這 17 條離開 MISSING、且正是這 17 條」**，不要拿總數當基準。
   - 本波 7 條轉 PRESENT 的檔／行：

     | 檔 | 行 |
     |---|---|
     | `SECSGEM\uHGemHT9045_SV.cpp` | 360 |
     | `CosFunction.cpp` | 1466, 3381 |
     | `adam6024.cpp` | 263 |
     | `csystem.cpp` | 15437 |
     | `HS_Function.cpp` | 3549 |
     | `Motor\mymotor.cpp` | 415 |

8. **LEDGER 記錄要點**
   - 日期列更新：`20260401`(1/1 done)、`20260410`(1/1 done)、`20260415`(cShowBinSelect 2 → **F12**)、
     `20260417`(cShowBinSelect 1 → **F12**)、`20260505`(adam 1 done ／ uhome 1 → **白名單**)、
     `20260513`(csystem 1 done ／ acatchtray+asendic 2 → **白名單**)、`20260515`(3 → **白名單**)、
     `20260519`(HS 1 done ／ BarCode+uTrayEdit 3 → **白名單**)、`20260520`(1/1 done)、
     `20260605`(1 → **白名單**)、`20260618`(1/1 done)。
   - 類別：**A 類為主（7 條 profile 旗標／SV 註冊／註解／dummy guard），其中 0618 一條屬 D 類邊緣**
     （馬達 `iCheckZ`，忠實搬運＋錨點全檔唯一 ⇒ 可執行；搬移範圍嚴格限定該註解所屬 5 行）。
   - **case 歸屬**：
     `CASE-20260618-001` = W16-08（南茂 Index Z2 Position Error）；
     `CASE-20260520-001` = W16-03（PTI Fix3 auto limit）；
     `CASE-20260515-001` = cContactCT 3 條（**白名單，V910 已有**）；
     `CASE-20260605-001` = cSortCT（**白名單，功能本體 V910 已有**）；
     其餘 0401／0410／0505／0513／0519 為 899 期累積出貨內容。
   - **本波不新增任何客戶碼判斷**；FOREHOPE／PTI／ChipMos 閘門全部是 V899 原生的。
   - 外溢七項見 §6.3（特別是 1、4、5 三項）。
   - 反向發現 R1–R9 已確認全部未被倒回（§5，含實測次數）。
   - 教訓三則可入 KNOWLEDGE：
     (a) **「MISSING」有一半可能是公司已經做了**——本波 20 條裡 10 條的等價實作就在 V910 pristine 基線 `e06524a`；
     判 A/C 之前一定要先跑「剝註解正規化 diff」，不要只看 `//AI` 簽章在不在。
     (b) **半搬狀態要主動找**：`ShowDoubleEPConnectGuide`（有函式無呼叫）與 `bAutoDeviceEjection`（有載入無讀取）
     都是「V910 收了基礎建設沒收消費端」，矩陣只會顯示消費端那一條，看起來像小事，實際是整個功能沒接上。
     (c) **UI 版面案要先看 `.dfm` 有沒有分家**——`cShowBinSelect.dfm` 差一顆 `btnASM` 就讓整組座標算式從「修正」變「回歸」；
     光看 `.cpp` 完全看不出來。

9. **commit 後清 V910 樹 `*.mgbak`**（6 個），讓下一波的差異報告基準回到波前狀態。
