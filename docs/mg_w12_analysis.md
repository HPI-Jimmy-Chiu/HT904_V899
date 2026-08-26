# MG-W12 分析：20260810 / 20260811 OutArm rotate 群（力成 PTI 2x4 交錯 4 站）

- **波次**：MG-W12（V899 → V910），LEDGER 日期列 **20260810（21 條）全結清** ＋ **20260811 殘餘 12 條**
- **來源**：`docs/mg_ai_matrix_missing.csv` **33 條 MISSING**
  （0810 × 21、0811 × 12；0811 的 PowerSavingMode 9 條已於 MG-W8 完成，不屬本波）
- **CASE**：`CASE-PTI-20260810-001`（Auto1 未滿盤退盤無警報）、`CASE-PTI-20260810-002`（DUT 重複吸取 / double IC）、
  `CASE-PTI-20260811-001 part 2`（旋轉站位置檢查）；另含兩條**日期撞號**的他案（見 §1.6）
- **V899 樹**：`D:\HT9045\HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422`（唯讀；本波 9 個來源檔
  `git status --porcelain` 空、`git diff b515ed5 HEAD` 對這些檔為空 → 工作區＝在製收斂端點 `b515ed5`）
- **V910 樹**：`D:\HT9045\HT9011UC_Code_V3.33.910.0_20260716_Jimmy`（MG-W11 `f4eab05` 後，`git status` 乾淨、`*.mgbak` 0 個）
- **本文件性質**：分析＋splice op 草案。**全程唯讀，兩棵樹一個位元組都沒有被修改**（模擬結果只寫進 scratchpad）
- **op 檔**：`D:\HT9045\docs\mg_w12_ops.json`（**32 個 op，9 個檔**）
- **類別**：**D 類（OutArm 運動路由）為主**。忠實搬運、錨點全部唯一 → 可執行。
  **<95% 項目：0 個**（詳見 §6）

---

## ⚠ 開工前必須先知道的四件事

1. **本波沒有任何客戶碼閘門，也不需要新增。** 全部是**機型天然隔離**（`iInArmType==e9045_2x4_4_13/_14`、
   `USE_PICKER_COUNT!=ep16Picker`）與**既有旗標**（`IniConfig.bE74_InspectArmPosition`、
   `config.ini [Debug] bOutArmRoundLog`）。V899 出貨版就是這個形狀，照搬即可（§4）。

2. **0811 的 `InspectOutArmPosition` 全套（7 條裡的 6 條 ＋ rotate 呼叫點）在正式 build 是「編得進去、跑不到」。**
   `chkE74` 在 `cConfiguration.cpp:2119-2123` 被 `#ifdef SOFT_SIMULTE` 包住，正式 build 走 `#else`
   註冊為 `bNoShow, bDisable, bFixedValue, 0` → `IniConfig.bE74_InspectArmPosition` 恆為 0 →
   `InspectOutArmPosition()` 第一行就 return。V899 也是同一形狀（`cConfiguration.cpp:2016/2018`），
   兩樹 `MachineType.h` 的 `//#define SOFT_SIMULTE` 都是註解掉的。
   **這一點決定了本波唯一「解除既有 early return」的變更（MG-W12-18）在量產機上為零風險。**

3. **0810 的逐輪診斷 Log 是「所有客戶預設開啟」的**（`CheckAndReadIniData(..., "Debug", "bOutArmRoundLog", 1)`），
   而 `CheckAndReadIniData` 在鍵不存在時**會把預設值寫回 config.ini**（`common.cpp:442-443`）。
   這是 V899 出貨版行為，忠實搬運；但它是**本波唯一對全體客戶可見的外溢**，必須記 LEDGER（§6.2）。

4. **Big5 原始碼不可用「位元組層數括號」做結構檢查。** Big5 雙位元組字的尾位元組可能是 `0x7B`/`0x7D`，
   本波第一次跑括號平衡時得到 4 個檔「不平衡」，**全部是假警報**；改成先 `cp950` 解碼再數字元後，
   9 個檔的 `{}` 與 `()` 差值**全部為 0**（§3.4）。與記憶中的「golden span 抽取括號陷阱」同型。

---

## 0. 前置事實（全部實測）

### 0.1 檔案基本盤（**md5 前提**，套用前必須逐檔重驗）

| 檔 | V899 行數 / md5(12) | **V910 行數 / md5(12)** | EOL | cp950 | 被前波動過？ |
|---|---|---|---|---|---|
| `aoutarm.cpp` | 3874 / `499907494008` | **3984 / `2d2e146ff75d`** | 兩樹全 CRLF | OK | 否 |
| `RotateKit\aRotateKIT.cpp` | 205 / `f0c4be6a31bf` | **208 / `c346ed5402ad`** | 兩樹全 CRLF | OK | 否 |
| `aoutarm9045_2x4_4.cpp` | 1733 / `5aa5fbf656c2` | **1686 / `64da1e90aecd`** | 兩樹全 CRLF | OK | 否 |
| `aoutarm9045.h` | 103 / `8f9181c55a35` | **106 / `1742f85ac130`** | 兩樹全 CRLF | OK | 否 |
| `aoutarm9045.cpp` | 4494 / `07fd3fa02d06` | **4783 / `cc9f698611a4`** | 兩樹全 CRLF | OK | 否 |
| `RotateKit\aRotateKIT_Out.cpp` | 4091 / `66c1f6e19bc7` | **4038 / `e658847a4508`** | 兩樹全 CRLF | OK | 否 |
| `main.cpp` | 33575 / `81596aa5852b` | **35339 / `908ad65319dc`** | 兩樹全 CRLF | OK | **是（W4/W10）** |
| `cprod.cpp` | 3899 / `11b8d7503eef` | **4086 / `ed7908e3b1d9`** | 兩樹全 CRLF | OK | **是（前波）** |
| `cConfiguration.cpp` | 7573 / `e69531a98d53` | **7867 / `7057c81d976f`** | 兩樹全 CRLF | OK | **是（W8 +27）** |

> 本文件所有 V910 行號都量在**上表 md5 對應的磁碟現狀**上，不是任何舊快照。
> V910 樹目前 `.mgbak` 為 0 個且 `git status` 乾淨 → `port_check.py` 會拿 `git HEAD` 當基準，與上表一致。

### 0.2 範圍權威：V899 的五個來源 commit（`bb69c60..b515ed5`）

`git log bb69c60..b515ed5 -- <本波 5 個 OutArm 檔>` 恰好 **5 個 commit，無遺漏、無中間態被撤回**：

| commit | 時間 | 主題 | 動到的檔 |
|---|---|---|---|
| `6f1df8c` | 08-10 13:37 | V899.35：救援 pitch 反而**往上取到更大倍數**（CASE-PTI-20260810-001） | `aoutarm.cpp` |
| `1e31f82` | 08-10 15:50 | double IC：資料說右半、座標算左半（CASE-PTI-20260810-002） | `aoutarm9045_2x4_4.cpp` |
| `f715979` | 08-10 17:00 | 一半 IC 沒進旋轉站：stride 仍指向吸嘴 3 | `RotateKit\aRotateKIT.cpp` |
| `5d5d7d3` | 08-10 17:24 | OutArm 逐輪診斷 Log（含 State Record 帶檔） | `aoutarm9045.cpp/h`、`aoutarm9045_2x4_4.cpp`、`aRotateKIT_Out.cpp`、`main.cpp` |
| `f641734` | 08-17 13:11 | V899.36：旋轉站位置檢查到第二格＋不再謊報期望位置（AI 日期標 20260811） | `aRotateKIT_Out.cpp`、`aoutarm9045.cpp/h` |

另有 **`3200884`**（State Record 收 `Error\` 下兩份 log）供應 `main.cpp:25566` 那條。
**本波逐 op 的內容已與這 6 個 commit 的 diff 逐 hunk 對帳，一一對應、無多無少。**

### 0.3 符號稽核（V910 全樹 712 檔，排除 `.svn`）

**8 個新符號在 V910 命中數皆為 0** → 33 條 MISSING 判定成立，無「已搬一半」狀態，也不會撞名：

`OutArmRoundLog_*`(6 支) / `OUTARM_ROUND_LOG_MAX` / `sOutArmRoundLog` / `iOutArmRoundHead` /
`iOutArmRoundTgtX`(等 5 個 static) / `OutArmRoundLog_SuckMap` / `iOutPlaceToRotate` / `bOutRotateUse13Picker`

新程式用到的**既有**符號全部存在且可見（逐一實測）：

| 符號 | V910 宣告處 | 目標檔看得到嗎 |
|---|---|---|
| `CheckAndReadIniData(AnsiString,AnsiString,AnsiString,int)` | `common.h:45`（**與 V899 逐行相同的 6 個多載**） | ✓ `aoutarm9045.cpp` include `common.h` |
| `AuthPath` | `common.h:119`（V899 是 118，只是位置差） | ✓ |
| `iRotateKIT_Start_X_H` / `_Pitch_X_H` / `_Start_Y_H` / `_Pitch_Y_H` | `RotateKit\aRotateKIT.h:34,37…` | ✓ `aoutarm9045.cpp` 與 `aRotateKIT_Out.cpp` 都 include |
| `tRotate`（`ColCount`/`DutNum`/`RotateKit_PitchX`/`ActiveRotate`） | `RotateKit\fRotate.h:148` | ✓ |
| `iOutRotateFinish` | `RotateKit\aRotateKIT.h:19`（**兩樹同行同註解**） | ✓ |
| `Prod.iOutArm_RotateX/Y`、`OutArmSuck.iPickStep`、`MOutRotateKit` | `cprod.h` / `MyKitSuck.h` / `cmydef.h` | ✓ |
| `i2x2Suck_Out` | `RotateKit\aRotateKIT.h:16` | ✓ |

**`#include` 閉包：本波不需要新增任何 `#include`。**
`aoutarm9045_2x4_4.cpp` / `aRotateKIT_Out.cpp` / `aRotateKIT.cpp` / `main.cpp` 都 include `aArmHeader.h`，
而 `aArmHeader.h` include `aoutarm9045.h` → `OutArmRoundLog_*` 與 `OUTARM_ROUND_LOG_MAX` 全部看得到。
`aoutarm9045.cpp` 直接 include `aoutarm9045.h`、`aRotateKIT.h`、`fRotate.h`、`common.h`。
六個目標 `.cpp` 的 include 清單兩樹**完全一致**（唯二差別：V910 的 `aRotateKIT*.cpp` 多 `fAOI.h`、
`main.cpp` 多 `ObserveMagazine.h`，都與本波無關）。

### 0.4 三個 gate 的形狀（**本波沒有客戶碼閘，也不新增**）

| 變更 | 閘門 | 非目標機台的行為 |
|---|---|---|
| `GetNowShuttleMode_2x4_4` 左右半（W12-03） | **整個 `aoutarm9045_2x4_4.cpp` 模組**只在 `iInArmType==e9045_2x4_4_13 \|\| _14` 才被呼叫（`aoutarm9045.cpp:607/2132` 兩處 dispatch，兩樹相同） | 指令級零變更 |
| `i2x2Suck_Out=1`（W12-02）／`bOutRotateUse13Picker`（W12-24/25） | `TestIF.iTestMode==_8Site2X4\|_16Site4X4\|_16Site2X8` && `ArmCanSuck4IC(0)==false` **＋新增的內層 `USE_PICKER_COUNT!=ep16Picker && (iInArmType==e9045_2x4_4_13\|_14)`** | 內層條件不成立 → 走原路徑，值與判斷式皆不變 |
| pitch 救援夾下限（W12-01） | `bUseDeviceDinemsion==true`（＝呼叫端傳 `bOutArmXOverLimit`）＋落在最後一個 `else`（4-pick）分支＋`i==1`＋`OutArmClose_PitchX<iMinXPitch4Pick` | 四個條件缺一 → 完全不執行；`bUseDeviceDinemsion==false` 的正常路徑一行都沒動 |
| `InspectOutArmPosition` 全套（W12-17..23、W12-28） | `IniConfig.bE74_InspectArmPosition`，正式 build 由 `#ifdef SOFT_SIMULTE` 的 `#else` 支釘死為 `bFixedValue, 0` | 函式第一行 return，**量產機完全不執行** |
| 逐輪診斷 Log（W12-04..12、W12-14/15/16、W12-26/27、W12-29/30） | `config.ini [Debug] bOutArmRoundLog`，**預設 1（開）**，`OutArmRoundLog_Enabled()` 只讀一次並快取 | **所有客戶都會執行**（純觀察、全部 try/catch 包住）→ 記 LEDGER（§6.2） |

`AutoCalculateOutArmXClosePitch` 的呼叫點兩樹**完全一致**（`aoutarm.cpp:2259/2339`、`aoutarm.cpp:3304/3405`、
`aoutarm9045.cpp:2443/2368` 與 `2475/2400`、`aoutarm9045_2x8_8.cpp`、`Magazine.cpp` ×2），
且 6f1df8c 論證所依賴的「`bOutArmXOverLimit` 為真時 `bCanPick2ICAtOnceTime` 被強制 false」
在 V910 `aoutarm9045.cpp:2493-2494` 同樣成立 → **該 commit 的 blast-radius 論證在 V910 完整成立**。

### 0.5 V910 的公司漂移地圖（本波目標區）

| 檔 | 目標區內的漂移 | 對本波的影響 |
|---|---|---|
| `aoutarm9045_2x4_4.cpp` | 正規化後 **V910[30-890] vs V899[30-950] 只有本波要搬的 10 處**；差異全是行尾註解對齊欄位（V910 推到第 ~120 欄、V899 在第 80 欄） | 零。`CheckOutArmXYPitch_2x4_4` 兩樹敘述逐行相同，`iModeCol==110` 分支（`:412/431/450/469`）**原封存在**，可直接被復活 |
| `aoutarm9045.cpp` | 目標區內只有註解對齊；`CheekNeedToDoOutArmAdditionalFunction` 與 `InspectOutArmPosition` 主體兩樹敘述相同 | 零 |
| `aRotateKIT_Out.cpp` | 目標區內只有註解對齊 | 零 |
| `aoutarm.cpp` | **有真實漂移，但在鄰居分支**：V910 `3106-3107/3114/3116` 是 `RogerYang 20260326 : Fix for 1x4`（`iStep` 改回活的、`iMinXPitch2Pick_13`→`_14`、`/4.0`→`/6.0`），V899 對應處是舊的被註解版本 | **不碰**（見 §5 反向發現 R1）。本波插入點在其後約 120 行的 4-pick `else` 分支 |
| `cprod.cpp` | V910 多 `Eastsun 20260710` 的 `CC_KYEC_LEE && USE_BU5_Function` `GaliPosRange+=5`（`2662-2663`），在本波取代行**之後** | 不碰 |
| `cConfiguration.cpp` | 本波取代行前後為 V910 原文；`[I21-10]` 區段無其他漂移 | 零 |
| `main.cpp` | V910 多 `Steven 20260504` 的 EventLogTxt robocopy（`26715-26718`），在插入點**之前** | 零；插入位置（config 之後、GPIB 之前）與 V899 相對順序一致 |
| `aoutarm9045.h` | V910 尾端多 4 個自有宣告（`IsMotorArrival` / `DoOutArmIonFanGiveWay` / `InitDoOutArmIonFanGiveWayTask` / `MoveOutArmXY_To_ESDSafePos`） | 刻意保留在新宣告之後（V899 那裡是 `#endif`） |

### 0.6 未標記承重行（矩陣看不到，漏搬即壞）

本波 32 個 op 共搬 **393 行**：

| 類別 | 行數 |
|---|---:|
| 矩陣列出的 `//AI` 標記行 | **33** |
| **V899 新增但無 `//AI` 標記**（含 9 個空行）→ 矩陣盲區 | **335** |
| V910 原本就有、僅為取得唯一錨點而一併重插的既有行（位元組相同、無功能變化） | **25** |

**335 是本戰役目前最大的盲區比例（W8 是 116）**——因為本波有兩大塊「一條註解帶一整段實作」：

| V899 位置 | 內容 | 矩陣只列 | 漏搬的後果 |
|---|---|---|---|
| `aoutarm9045.cpp:2101-2232` | 逐輪 Log 的 4 個 static ＋ 6 支函式本體（132 行） | `:2100` 一行 | 其他 6 個檔全部 **link error** |
| `aoutarm9045_2x4_4.cpp:560-597` | `OutArmRoundLog_SuckMap()` 函式本體（38 行） | `:558` 一行 | 同檔兩個呼叫點 link error |
| `aoutarm9045.h:96-102` | `#define OUTARM_ROUND_LOG_MAX 20` ＋ 6 個宣告 | `:95` 一行 | 全部 link error |
| `aoutarm9045_2x4_4.cpp:41-46` | `if/else if/else` 三條 return（左右半判斷本體） | `:40` 一行 | **修正等於沒搬**（仍恆 return 2） |
| `aoutarm.cpp:3127-3128` | `if(i==1 && ...) OutArmClose_PitchX=iMinXPitch4Pick;` | `:3125,3126` 兩行註解 | **修正等於沒搬** |
| `RotateKit\aRotateKIT.cpp:115-119` | `if(USE_PICKER_COUNT!=ep16Picker && ...) i2x2Suck_Out=1;` | `:114` 一行 | 同上 |
| `aoutarm9045.cpp:3795` | `int iOutPlaceToRotate=2;` | `:3792-3794` 三行註解 | 未定義符號 |
| `aoutarm9045.cpp:3849-3850` / `3858-3859` / `3935-3939` | BulkBox return、加了 `iAction` 的判斷式、旋轉站幾何式 | 各只列註解行 | 修正等於沒搬 |

> 本波所有 op 都以**連續區段**抽取，上述行自然被包含——這正是「用區段不用逐行」的理由。

---

## 1. 六個主題的變更敘述（搬什麼、為什麼）

### 1.1 pitch 救援「往上取更大倍數」（`6f1df8c`，CASE-PTI-20260810-001）→ **W12-01**

`AutoCalculateOutArmXClosePitch()` 的 device-dimension 救援路徑算 `XDimension*step+100`。
12mm 元件的最小階＝13.00mm，比 13.33mm 機構下限低 0.33mm，於是迴圈**丟棄它改取下一倍數 25.00mm**——
比 16.00mm 的 tray pitch 還寬，比它要救的還糟。Head4 需要多一個 pitch 的負向行程，
`-53595 - 2500 = -56095` 仍超出 `-55843` 軟體極限，該格被寫成 `HAS_NULL_IC`；
`Find_OutArm_Single` 之後找不到 NULL_IC → `SetOutArm_9045` 立 `bOverTray` → `DoOutArmPlaceToAuto_9045`
呼叫 `SetTray(HAS_IC)` 把 168 格全標成已佔用 → **盤子少一顆 IC 就退，機台自己的計數還多一顆，全程無警報**
（`WAR16318` 被 `iHWFix_BinBox==1 && iWhichAuto==eBulkBox` 擋掉）。

**最終態＝把最小階夾到下限（往下夾，不是往上取倍數）**：
```
if(i==1 && OutArmClose_PitchX<iMinXPitch4Pick)
    OutArmClose_PitchX=iMinXPitch4Pick;
```
13.33mm → `-54928`，餘裕 9.15mm。**任務書要的「向下取整方向」＝這個形狀，已確認並照搬。**
`bUseDeviceDinemsion==false` 的正常路徑（仍用 32.00mm）一行未動。

### 1.2 double IC：資料在右半、手臂去左半（`1e31f82`，CASE-PTI-20260810-002）→ **W12-03**

MT2K010 跑 8SITE2X4 但只開交錯四站（Aa/Ac/Bb/Bd）→ `iInArmType` 降成 `e9045_2x4_4_14`，
OutArm 分兩趟清出料飛梭 2：kit0＝欄 0,1（DUT2,DUT1），kit1＝欄 2,3（DUT4,DUT3）。
資料側一直知道（`Item[i][j+iPickKit]`、`iOutArmiWhichKit` 在左半清空後翻 1），
**座標側不知道**：`GetNowShuttleMode_2x4_4()` 在 `iXStep==1 && iYStep==1` 時直接 `return 2`，
`iModeCol` 兩趟都是 0 → `CheckOutArmXYPitch_2x4_4` 的 `iModeCol==110`（右半）分支變死碼 →
兩趟瞄同一個 X → 手臂停在 DUT1,2 上方卻對 DUT3,4 開真空 → `JAM0202 " AF"` ×5；
按 SKIP 又把手臂沒去過的 `BRCarryKit(0,2)/(1,3)` 清成 `NULL_IC` → 兩顆實體 IC 留在飛梭上回 index → double IC。

最終態：無料仍 `return 2`（**空的情境位元組級不變**），有料時用**與 `iOutArmiWhichKit` 同一個述詞**
`LeftSideNoIC(iShtKitStep)` 回 `11002`（右半）/ `10002`（左半）。
V910 的 X 算式（`aoutarm9045_2x4_4.cpp:458-476`，「基準為第三隻吸嘴」分支）與 V899 逐字相同：
`iModeCol==0/100` 是 `+dMovePitchX*2.0-dSiteXPitch*1.5`、`iModeCol==110` 是 `+dMovePitchX*2.0+dSiteXPitch*0.5`
→ **差值恰為 `+2*dSiteXPitch`（該機台 120mm），與 V3.32 的行為相同、與 commit 描述相同。**
`iModeRow` 全程維持 2，Y 數學與 `OutArmZNeedDown_2x4_4` 覆蓋範圍不動；kit0 完全不受影響。

### 1.3 一半 IC 沒進旋轉站（`f715979`）→ **W12-02**

`iRotate_Type==e1MotRotate` 且 `Rotate.Data` 的 DutNum＝0（`tDutType_4`），
`M_DoOutArmRotateKIT_Motor` case 1000 落到泛用 else、`M_MoveOutArmZ_ToRotateKIT_Place` 落到「只有一顆旋轉 sensor」的 else，
兩邊都用 `j = iKit; j < MAX_ARM_Col; j += i2x2Suck_Out` 列舉。
`i2x2Suck_Out=3` 只會走到欄 0 與 3；但 `SetInOutArmParameter_2x4_4` 早就把 OutArm 資料搬到**邏輯欄 0 與 1**
→ 欄 1（DUT1/DUT3）永遠碰不到 → 沒旋轉就進 Auto tray。
最終態＝**比照 2x2 的既有先例**（`aRotateKIT.cpp:128` 早就為 2x2 設 1），在 `_8Site2X4` 分支內對
`e9045_2x4_4_13/_14` 把 stride 改成 1；`i2x2Suck`（InArm）刻意維持 3（2x4_4 只搬 OutArm 的資料）。
**`i2x2Suck_Out` 在 V910 全樹的讀取點實測只有 `aRotateKIT_Out.cpp` 一個檔（10 處：
`1077/1224/1345/1847/1849/2178/2731/2847/2884/2942`），全部是「欄位偏移」語意**
（另加 `aRotateKIT.cpp` 的定義與 4 個賦值、`aRotateKIT.h:16` 的 extern）→ 影響面與 V899 一致。

### 1.4 旋轉站位置檢查（`f641734`，CASE-PTI-20260811-001 part 2）→ **W12-13、17～25、28**

三件事：
1. **幾何**（W12-24/25）：`CalcPosition_OutArm()` 新增 `bOutRotateUse13Picker`，
   把 `2x4_4_13/_14` 從 Sam 20211224 的「1 4 吸嘴」分支（兩吸嘴實距只有 `2*(Pitch_X_H/3)`）
   改走「吸嘴 1 中心到吸嘴 3 中心」（`Pitch_X_H*3/2`）分支，才夠得到旋轉站第二格。
   因 `iOutArmXBase==2`，兩式對實體欄 0 那支吸嘴算出的位置**完全相同**，只有遠端吸嘴由欄 3 換成欄 2。
2. **檢查**（W12-13/17/18/28）：`InspectOutArmPosition()` 新增第三種動作 `iOutPlaceToRotate=2`
   （不能重用 `iOutPlaceToAuto`：那條路徑 `s6TrayName[iTarget]` 只有 `eTrayCount(33)` 個元素，
   `MOutRotateKit=42` 會越界），並把 `Steven 20241220 : 暫時跳過` 的 `return;` 註解掉。
3. **撞號與基準**（W12-19～23）：`eFix12(17)==MOutShuttle1(17)`、`eBulkBox(18)==MOutShuttle2(18)`；
   分流與 BulkBox 跳過都改成**帶 `iAction`** 判斷（只看值 18 會連 `MOutShuttle2` 的取料檢查一起關掉）；
   picker 重映射 offset 兩邊基準對齊；旋轉站期望位置由單點改為幾何推算
   （**刻意不呼叫 `CalcPosition_OutArm()`——那是算指令值的人，拿它當基準就是自己驗自己**）。

### 1.5 OutArm 逐輪診斷 Log（`5d5d7d3` ＋ `3200884`）→ **W12-04～12、14～16、26/27、29/30**

RAM ring buffer 保留最近 20 輪，回 HOME 不清（只重置 task 變數），
**發 alarm** 與**做 State Record** 時各 flush 一次到檔（`D:\HT9045\Error\OutArmRoundLog.txt`）。
記錄內容：CFG（機型/吸嘴幾何/旋轉站選項）、MOVE（目標 vs 實際 encoder）、SUCK-FAIL/SUCK-OK（吸嘴↔Shuttle 欄位對應表）、
ALARM（JAM020n / ErrPart / 操作員選擇）、ROT-GATE（為什麼沒去旋轉）、ROT-XY、ROT-PLACE。
**純觀察，每一支都 `try{}catch(...){}` 包住，且第一行就檢查 `OutArmRoundLog_Enabled()`。**
`3200884` 另外讓 State Record 逐檔點名收 `BootLog.txt*` 與 `OutArmRoundLog.txt`
（**刻意不整夾複製也不用 `/e`**——`Error\` 下有 3700 多個語系 `.dat`、約 5MB）。

### 1.6 兩條**日期撞號**的他案（非 OutArm 主題）→ **W12-31、32**

矩陣 20260810 那 21 條裡有 2 條與 OutArm 無關，來自不同客戶的不同 case；
**納入本波只是為了把 LEDGER 的 20260810 一次結清**，行為上互不相干：

| op | 檔 | 客戶 | 內容 | 影響面 |
|---|---|---|---|---|
| W12-31 | `cprod.cpp:2535` | 全智 `CC_GIGAS` | `GaliPosRange` 預設 10 → 50（與其他客戶原廠標準一致；Y1 定位殘差恆為 10 與門檻相等造成 `Index 4 Axis Need Home` 誤報），下限維持 5 | 只在 `CUSTOMER_CODE==CC_GIGAS` 分支內；且只改**鍵不存在時的預設**，已有 ini 的機台讀原值 |
| W12-32 | `cConfiguration.cpp:2522` | 甬矽 `CC_FOREHOPE_NINGBO` | `[I21-10] bI21RTmodeDonotRunSiteMapping` 對甬矽也解鎖（原本只有 `CC_JCET` 看得到） | 只是讓選項可見可編，**預設值仍為 0**，未勾選前行為不變 |

---

## 2. 逐 op 明細（`docs/mg_w12_ops.json`，32 op）

> 「錨點命中」＝該字串在 V910 該檔**全檔**的命中次數（實測）。`splice.py` 同檔由**下往上**套用。
> `replace` 的被取代行**全部逐位元組驗證過**與 V899 對應行相同（除了本來就要改的那幾行）。

### 2.1 `aoutarm.cpp`（1 op，3984 → **3989**，+5）

| op | 矩陣行 | V899 範圍 | V910 動作 | 錨點（命中） | 淨 |
|---|---|---|---|---|---:|
| **W12-01** | 3125, 3126 | `3124-3130` | `replace [3230,3231]` | 跨行：空行＋16 空白縮排的 4Pick 判斷式 → **1**（單行版命中 2，故用跨行） | +5 |

### 2.2 `RotateKit\aRotateKIT.cpp`（1 op，208 → **214**，+6）

| op | 矩陣行 | V899 範圍 | V910 動作 | 錨點（命中） | 淨 |
|---|---|---|---|---|---:|
| **W12-02** | 114 | `114-119` | `after 122` | `i2x2Suck_Out=3;` → **1** | +6 |

### 2.3 `aoutarm9045_2x4_4.cpp`（10 op，1686 → **1769**，+83）

| op | 矩陣行 | V899 範圍 | V910 動作 | 錨點（命中） | 淨 |
|---|---|---|---|---|---:|
| **W12-03** | 40 | `38-47` | `replace [38,41]` | 跨行：`if(...)`+`{`+`return 2;`+`}` → **1** | +6 |
| **W12-04** | 155 | `155-157` | `before 149` | `bool CheckOutArmXYPitch_2x4_4(...)` → **1** | +3 |
| **W12-05** | 535 | `535-540` | `before 526` | `if(CheckOutArmXYPitch_2x4_4(&iXPos, ...)` → **1** | +6 |
| **W12-06** | 558 | `558-598` | `before 543` | `bool DoPickFromShuttle_9045_2x4_4(int iSht)` → **1** | +41 |
| **W12-07** | 648 | `647-650` | `replace [591,592]` | 跨行 `iWitchErrBin=0;`+`Task=200;` → **1**（單行版會命中宣告行） | +2 |
| **W12-08** | 654 | `654-664` | `before 596` | `fAutoTeach->iATOutArmWhichKit=iOutArmiWhichKit;` → **1** | +11 |
| **W12-09** | 754 | `754-755` | `before 685` | `iOutShtRetryCount++;` → **1** | +2 |
| **W12-10** | 779 | `776-781` | `replace [705,708]` | `fFixAICCD->OutArmCycleCounterUpdate();` → **1**（下一行 `iOutShtRetryCount=0;` 命中 4） | +2 |
| **W12-11** | 816 | `816-823` | `after 742` | `ret=OutArmPickShuttleAlarm(iSht, bHasDuplicateErr, ErrPart);` → **1** | +8 |
| **W12-12** | 901 | `901-902` | `before 820` | `SetOutArmHome();` → **1** | +2 |

> W12-06 的區段含結尾 `//====` 分隔線，插在 V910 既有 `//====` 之後，**結果與 V899 版面完全一致**。
> W12-07/08/10 的 payload 在模擬結果中落在 **647-650 / 654-664 / 776-781**——與 V899 行號**完全相同**，
> 是兩檔在該區結構對齊的旁證。

### 2.4 `aoutarm9045.h`（2 op，106 → **115**，+9）

| op | 矩陣行 | V899 範圍 | V910 動作 | 錨點（命中） | 淨 |
|---|---|---|---|---|---:|
| **W12-13** | 67 | `67-67` | `after 74` | `extern int iOutPlaceToAuto;` → **1** | +1 |
| **W12-14** | 95 | `95-102` | `after 101` | `bool InitNewFixTrayForUnloaderClipRead(...)` → **1** | +8 |

### 2.5 `aoutarm9045.cpp`（9 op，4783 → **4961**，+178）

| op | 矩陣行 | V899 範圍 | V910 動作 | 錨點（命中） | 淨 |
|---|---|---|---|---|---:|
| **W12-15** | 2100 | `2099-2233` | `before 2162` | `bool bOutRotator;` → **1** | +135 |
| **W12-16** | 2341 | `2338-2350` | `replace [2270,2272]` | `InitDoOutArmAdditionalFunction();` → **1** | +10 |
| **W12-17** | 3792 | `3792-3795` | `after 3893` | `int iOutPlaceToAuto=1;` → **1** | +4 |
| **W12-18** | 3826 | `3825-3827` | `replace [3923,3924]` | `        return; ... //Steven 20241220` → **1** | +1 |
| **W12-19** | 3842 | `3842-3851` | `before 3939` | `int XEncoder=0, YEncoder=0, HardwarePosX=0, HardwarePosY=0;` → **1** | +10 |
| **W12-20** | 3857 | `3857-3859` | `replace [3944,3945]` | 跨行 `if(iTarget==MOutShuttle1 ||`+`iTarget==MOutShuttle2)` → **1** | +1 |
| **W12-21** | 3866 | `3866-3874` | `replace [3952,3957]` | `HardwarePosX=Prod.iOutArmBinBoxX+dOutArmXPitch_1Step*iTargetCol;` → **1** | +3 |
| **W12-22** | 3910 | `3910-3914` | `replace [3993,3993]` | `HardwarePosX+=(iXoffset/3)*(iSuckCol-iOutArmXBase);` → **1** | +4 |
| **W12-23** | 3928 | `3928-3939` | `replace [4007,4008]` | `HardwarePosX=Prod.iOutArmRotateToUnloaderX;` → **1** | +10 |

### 2.6 `RotateKit\aRotateKIT_Out.cpp`（5 op，4038 → **4096**，+58）

| op | 矩陣行 | V899 範圍 | V910 動作 | 錨點（命中） | 淨 |
|---|---|---|---|---|---:|
| **W12-24** | 694 | `694-702` | `after 694` | `int OneSuckPitch;` → **1** | +9 |
| **W12-25** | 816 | `815-816` | `replace [806,806]` | `ArmCanSuck4IC(0)==false)` → **1** | +1 |
| **W12-26** | 918 | `917-936` | `after 906` | `CalcPosition_OutArm(iXPos, iYPos, iMovePitchX, iMovePitchY, iKit);` → **1** | +20 |
| **W12-27** | 1131 | `1129-1154` | `replace [1099,1103]` | `bool M_MoveOutArmZ_ToRotateKIT_Pick(int iKit, int PickAngle)` → **1** | +21 |
| **W12-28** | 2252 | `2246-2258` | `replace [2195,2200]` | 跨行（相對縮排承重）` == HAS_NULL_IC)`+`{`+`if(...==HAS_NULL_IC` → **1** | +7 |

> W12-28 的 CopyToTray 那行（含 32 空白縮排）在本檔命中 **6** 次，故**不能**用單行錨；
> 死前草稿選的跨行錨經實測**確實全檔唯一**（見 §4），故採用它，並確認被取代的 6 行與 V899[2246-2251] 位元組相同。

### 2.7 `main.cpp`（2 op，35339 → **35350**，+11）

| op | 矩陣行 | V899 範圍 | V910 動作 | 錨點（命中） | 淨 |
|---|---|---|---|---|---:|
| **W12-29** | 25566 | `25566-25574` | `before 26728` | `str1="D:\\GPIB9045\\system";` → **1** | +9 |
| **W12-30** | 25791 | `25791-25792` | `after 26968` | `DumpMainFormSnapshot(NewPath);` → **1** | +2 |

### 2.8 `cprod.cpp`（1 op，4086 → **4086**，±0）／`cConfiguration.cpp`（1 op，7867 → **7868**，+1）

| op | 矩陣行 | V899 範圍 | V910 動作 | 錨點（命中） | 淨 |
|---|---|---|---|---|---:|
| **W12-31** | 2535 | `2535-2535` | `replace [2655,2655]` | 含 `10` 與 `5, 100` 的整段 `ReadWriteIni(...)` → **1**（單獨 `IniConfig.GaliPosRange` 命中 4） | 0 |
| **W12-32** | 2522 | `2522-2523` | `replace [2669,2669]` | `if(CUSTOMER_CODE==CC_JCET)` ＋ `//Richard 20230427` → **1**（單獨判斷式命中 7） | +1 |

---

## 3. 事前模擬（記憶體內套用；輸出只寫進 scratchpad，V910 樹未動）

### 3.1 套用結果

| 檔 | 套用前 | 套用後 | Δ | CRLF | bare-LF | cp950 |
|---|---:|---:|---:|---:|---:|---|
| `aoutarm.cpp` | 3984 | **3989** | +5 | 3989 | **0** | OK |
| `RotateKit\aRotateKIT.cpp` | 208 | **214** | +6 | 214 | **0** | OK |
| `aoutarm9045_2x4_4.cpp` | 1686 | **1769** | +83 | 1769 | **0** | OK |
| `aoutarm9045.h` | 106 | **115** | +9 | 115 | **0** | OK |
| `aoutarm9045.cpp` | 4783 | **4961** | +178 | 4961 | **0** | OK |
| `RotateKit\aRotateKIT_Out.cpp` | 4038 | **4096** | +58 | 4096 | **0** | OK |
| `main.cpp` | 35339 | **35350** | +11 | 35350 | **0** | OK |
| `cprod.cpp` | 4086 | **4086** | ±0 | 4086 | **0** | OK |
| `cConfiguration.cpp` | 7867 | **7868** | +1 | 7868 | **0** | OK |

**32 個 op 的錨點檢查全部 `ANCHOR_OK=True`（FAIL COUNT = 0）**；
每檔的行數增減與 op 算術逐檔對帳相符（`payload - replaced`）。

### 3.2 簽章對帳

模擬結果中 `//AI(ht9045-v899) 20260810` ＝ **21**、`20260811` ＝ **12**，合計 **33**
＝ 矩陣 33 條，一條不多一條不少（`aoutarm.cpp` 2、`aRotateKIT.cpp` 1、`2x4_4` 10、`aoutarm9045.h` 1+1、
`aoutarm9045.cpp` 2+7、`aRotateKIT_Out.cpp` 1+4、`main.cpp` 2、`cprod.cpp` 1、`cConfiguration.cpp` 1）。

### 3.3 忠實度驗證

- **每個 op 的 payload 在結果中恰好出現 1 次**（32/32）。
- **payload ±3 行的上下文窗**：26/32 與 V899 逐位元組相同；另 6 個（W12-07/08/10/14/21/22）的差異
  **逐條開檔確認全部是 V910 較寬的行尾註解對齊**（正規化後相同），唯一的結構性差異是
  `aoutarm9045.h` 的 ctx+8：V899 那裡是 `#endif`、V910 是自有的 `IsMotorArrival(...)`——**設計如此**。
- **關鍵區段位元組相等**：`GetNowShuttleMode_2x4_4` 整支（sim[33-61] ≡ V899[33-61]）、
  逐輪 Log statics（sim[154-157] ≡ V899[154-157]）、`InitSuckState` `_8Site2X4` 分支、
  `bOutRotateUse13Picker` 宣告、`iOutPlaceToRotate` 宣告、pitch 救援夾下限段——**全部 IDENTICAL**。
- **移除行共 17 行，全部是刻意取代**（V899 最終態就是這樣）：

  | 檔 | 行數 | 內容 |
  |---|---:|---|
  | `aoutarm9045_2x4_4.cpp` | 1 | `return 2;`（換成左右半判斷） |
  | `aRotateKIT_Out.cpp` | 1 | `ArmCanSuck4IC(0)==false)`（換成帶 `&&` 的兩行） |
  | `aoutarm9045.cpp` | 13 | `return;`＋`sPlace="Pick from";`／`if(iTarget==MOutShuttle1 \|\|`＋下一行／BulkBox 區塊 6 行／offset 那行／旋轉站兩行 |
  | `cprod.cpp` | 1 | `GaliPosRange` 預設 10 那行 |
  | `cConfiguration.cpp` | 1 | `if(CUSTOMER_CODE==CC_JCET)` 那行 |

- **V910 原有內容零遺失**：逐行集合比對，9 個檔的原有行在結果中皆存在（除上述 17 行刻意取代）。

### 3.4 結構健全性（**要先 cp950 解碼再數**）

解碼後 `{` − `}` 與 `(` − `)` 的差值，**9 個檔套用前後完全相同（delta 全為 0）**。
（位元組層直接數會有 4 個檔假性不平衡——Big5 尾位元組可能是 `0x7B`/`0x7D`；已列為 §⚠4 的教訓。）

### 3.5 port_check 預測（模擬檔跑同一套分類邏輯）

```
[RotateKit\aRotateKIT.cpp]      added=6   (spliced=6,   authored=0) removed=0  | EOL (208,0)->(214,0)
[RotateKit\aRotateKIT_Out.cpp]  added=59  (spliced=59,  authored=0) removed=1  | EOL (4038,0)->(4096,0)
[aoutarm.cpp]                   added=5   (spliced=5,   authored=0) removed=0  | EOL (3984,0)->(3989,0)
[aoutarm9045.cpp]               added=191 (spliced=191, authored=0) removed=13 | EOL (4783,0)->(4961,0)
[aoutarm9045.h]                 added=9   (spliced=9,   authored=0) removed=0  | EOL (106,0)->(115,0)
[aoutarm9045_2x4_4.cpp]         added=84  (spliced=84,  authored=0) removed=1  | EOL (1686,0)->(1769,0)
[cConfiguration.cpp]            added=2   (spliced=2,   authored=0) removed=1  | EOL (7867,0)->(7868,0)
[cprod.cpp]                     added=1   (spliced=1,   authored=0) removed=1  | EOL (4086,0)->(4086,0)
[main.cpp]                      added=11  (spliced=11,  authored=0) removed=0  | EOL (35339,0)->(35350,0)
TOTAL added=368 spliced=368 authored=0 removed=17
```

**預期 `authored=0`、`comment=0`、`removed=17`、EOL 全 CRLF 無混用。**

---

## 4. 與死前草稿 `mg_w12_ops.json`（17:58 落地）的差異

草稿有 32 個 op，覆蓋面與我的一致（33 條矩陣點全中，無漏無多）。
**逐條開檔複驗後：29 個 op 我維持原案，3 個改寫，1 個先改後又改回草稿版。** 明細：

| 草稿 | 本版 | 判定 | 說明 |
|---|---|---|---|
| OP1 | W12-01 | **一致** | v899[3124,3130] replace [3230,3231]、跨行錨——實測跨行錨全檔唯一（單行版命中 2），草稿選擇正確 |
| OP2 | W12-02 | 一致 | — |
| OP3 | W12-03 | **強化錨點** | 區段與取代區相同；錨點由單行 `if(OutArmSuck.iXStep==1 && ...)` 改為涵蓋整個取代區的四行跨行字串，把 `{ return 2; }` 的形狀一併鎖住（若日後該區被別波改過會直接失敗而非誤套） |
| OP4–OP10 | W12-04–W12-10 | 一致 | 含 OP10 的 `replace [705,708]`（下一行 `iOutShtRetryCount=0;` 命中 4，草稿判斷正確） |
| OP11 | **W12-11 改寫** | **簡化** | 草稿：`replace [742,743]` ＋ v899[815,824]。實測 `ret=OutArmPickShuttleAlarm(...)` 全檔唯一 → 改為 `after 742` ＋ v899[816,823]，結果版面完全相同但**少 2 行非必要 REMOVED** |
| OP12 | **W12-12 改寫** | **簡化** | 草稿：`replace [819,820]` ＋ v899[900,903]。`SetOutArmHome();` 實測全檔唯一 → 改為 `before 820` ＋ v899[901,902]，**少 2 行 REMOVED** |
| OP13/14/15/16 | W12-24/25/26/27 | 一致 | — |
| OP17 | W12-28 | **我先改寫、複驗後改回草稿版** | 我原本要改成 `after 2200` ＋ 32 空白縮排單行錨；實測那個單行錨在本檔命中 **6** 次，而**草稿的跨行錨實測 count=1（真的唯一）**，且被取代的 6 行與 V899[2246-2251] 位元組相同 → **採用草稿版**。草稿在這一點上是對的，我的替代方案比較弱 |
| OP18/OP19 | W12-13/W12-14 | 一致 | — |
| OP20 | **W12-15 改寫** | **簡化** | 草稿：`replace [2157,2161]` ＋ v899[2094,2233]（多搬 5 行 `ShowMyMessage` 區塊只為取錨）。實測 `bool bOutRotator;` 全檔唯一 → 改為 `before 2162` ＋ v899[2099,2233]，**少 5 行非必要 REMOVED**，版面結果相同 |
| OP21/22 | W12-16/W12-17 | 一致 | — |
| OP23 | W12-18 | **換錨點** | 草稿錨 `sPlace="Pick from";`（取代區第二行）→ 改用第一行（那個要被註解掉的 `return;` 含 8 空白縮排與 `//Steven 20241220`），同樣全檔唯一，且更能表達本 op 的意圖 |
| OP24 | W12-19 | 一致 | — |
| OP25 | **W12-20 縮小範圍** | **最小化** | 草稿：`replace [3944,3946]` ＋ v899[3857,3860]（連 `GetOutArmToShtCellPos(...)` 那行一起重插）→ 改為 `replace [3944,3945]` ＋ v899[3857,3859]，那行留在原地不動；錨改為跨行兩行判斷式（唯一） |
| OP26/27/28 | W12-21/22/23 | 一致 | — |
| OP29/30 | W12-29/30 | 一致 | 含 `str1=\"D:\\\\GPIB9045\\\\system\";` 的 JSON 逸出——草稿寫法正確（實測命中 1） |
| OP31 | W12-31 | **加長錨點** | 草稿錨已含 `10` 與 `5, 100`；再往前加長到含 `ReadWriteIni(sPath, "Index", "GaliPosRange", ...)`，讓它在任何情況下都不可能命中 `else` 分支那行 |
| OP32 | W12-32 | **加長錨點** | 單獨 `if(CUSTOMER_CODE==CC_JCET)` 在該檔命中 **7** 次；加長到含 `//Richard 20230427` 後唯一 |

**草稿裡沒有被我發現的錯誤**（行號、區段、模式全部經得起逐檔複驗）；
差異集中在「錨點強度」與「不要為了取錨多搬既有行」。
另外草稿完全沒有做過模擬、也沒有任何 blast-radius 論證（§0.4／§5 全是本次新增）。

淨效果：**REMOVED 由草稿的 26 行降到 17 行**（少的 9 行全是位元組相同的重插），
32 個 op 的錨點**全部全檔唯一**（草稿有 1 個弱錨宣稱、2 個未查證的加長機會）。

---

## 5. 行為影響（給 LEDGER）

### 5.1 目標機台（PTI 2x4 交錯 4 站，`iInArmType==e9045_2x4_4_13/_14`）

| # | 變化 | 來源 op | 影響 |
|---|---|---|---|
| A | OutArm 取右半 kit 時 X **多走 `2*dSiteXPitch`**（該機台 120mm） | W12-03 | 修掉 JAM0202 連發與 SKIP 造成的 double IC；kit0、Y 軸、`OutArmZNeedDown_2x4_4` 覆蓋範圍完全不動 |
| B | 旋轉站列舉 stride 由 3 改 1（欄 0,1 而非 0,3） | W12-02 | DUT1/DUT3 才會被放上旋轉站；`i2x2Suck`(InArm) 不動 |
| C | 旋轉站 X 幾何改走「吸嘴1↔吸嘴3」式 | W12-24/25 | 近端吸嘴位置不變，遠端由欄 3 移到欄 2 |
| D | device-dimension 救援 pitch 夾到機構下限 | W12-01 | Auto1 少一顆 IC 就退盤且計數多一顆的鏈路整條停止 |

### 5.2 其他機台 / 其他客戶

| 面向 | 影響 |
|---|---|
| 非 `2x4_4_13/_14` 機型 | `aoutarm9045_2x4_4.cpp` 整個模組不會被呼叫；`bOutRotateUse13Picker` 恆 false；`i2x2Suck_Out` 維持原值 → **指令級零變更** |
| `bUseDeviceDinemsion==false`（正常 pitch 路徑） | 一行未動 |
| `InspectOutArmPosition` 全套 | 正式 build 恆不執行（§⚠2）；**SOFT_SIMULTE 模擬 build 才會生效** |
| 逐輪診斷 Log | **全客戶預設開啟**：每輪取料多幾次字串組裝與 ring 寫入；alarm 當下多一次 `fopen/fprintf/fclose`；State Record 多帶一個檔；首次執行會在 `config.ini` 寫入 `[Debug] bOutArmRoundLog=1`。全部 try/catch 包住，且 `iOutArmRoundHead<0` 時 `OutArmRoundLog_Line` 直接 return |
| State Record | 多兩行 `XCOPY`（`BootLog.txt*`、`OutArmRoundLog.txt`）；來源檔不存在時 XCOPY 只是失敗，不影響其餘步驟 |
| `CC_GIGAS` | `GaliPosRange` 的**預設**由 10 變 50（已有 ini 值的機台不受影響） |
| `CC_FOREHOPE_NINGBO` | Configuration 多看得到一個 `[I21-10]` 選項，預設仍 0 |

### 5.3 反向發現（V910 有、V899 沒有）——**不可回搬**

| # | 位置 | V899 | V910 | 判定 |
|---|---|---|---|---|
| **R1** | `aoutarm.cpp` 2-pick 分支 | `//iStep=XDivision/2+1;` ＋ `for(int i=1; i>=1; i--)` ＋ `iMinXPitch2Pick_13` ＋ `/4.0` | `iStep=XDivision/2+1;` `//RogerYang 20260326 : Fix for 1x4` ＋ `for(int i=iStep; ...)` ＋ `_14` ＋ `/6.0` | **V910 較新且與本波無關**，本波插入點在其後約 120 行的 4-pick `else` 分支，**確認未被倒回**（模擬結果逐行驗過） |
| **R2** | `cprod.cpp:2662-2663` | 無 | `CC_KYEC_LEE && USE_BU5_Function` → `GaliPosRange+=5`（Eastsun 20260710） | 在取代行之後，未被碰到 |
| **R3** | `aoutarm9045.h:102-105` | `#endif` | 4 個 V910 自有宣告 | 新宣告插在其前，V910 內容全數保留 |
| **R4** | `aoutarm9045.cpp` `InspectOutArmPosition` 內 | V899 在 `if(bX==false \|\| bY==false)` 之後多一個空行 | 無 | 純排版，**刻意不搬**（不在矩陣、無功能） |
| **R5** | 六個目標檔的行尾註解 | 對齊第 ~80 欄 | 推到第 ~120 欄 | 本波所有 `replace` 都避開了純對齊差異的行；唯一被反向的是 `cprod.cpp` 那一行（該行本來就要改值） |

---

## 6. <95% 清單與 LEDGER 記錄

### 6.1 建議寫入 `MG_FINAL_DECISIONS.md` 的項目：**無**

32 個 op 全部 ≥95%：錨點全檔唯一、payload 全部位元組取自 V899、V910 無反向刻意修改與之衝突、
無邏輯衝突需要改寫。**本波不出 F 列。**

（本文件全程唯讀，未動 `MG_FINAL_DECISIONS.md`。）

### 6.2 必須記入 LEDGER 的外溢（**不是阻塞項，但要留痕**）

1. **逐輪診斷 Log 對全體客戶預設開啟**，且首次執行會往機台 `config.ini` 寫入 `[Debug] bOutArmRoundLog=1`
   （`CheckAndReadIniData` 在鍵不存在時寫回預設值）。忠實搬運 V899 出貨版；
   若日後要改成預設關閉，屬**新需求**，兩棵樹要一起改。
2. **W12-18 是本波唯一解除既有 early return 的變更**（`Steven 20241220 : 暫時跳過`）。
   正式 build 因 `bE74_InspectArmPosition` 被 `#ifdef SOFT_SIMULTE` 釘死為 0 而不會執行；
   **但如果哪天有人打開 SOFT_SIMULTE 或改掉 `chkE74` 的註冊，這條路徑就會活過來**，
   屆時 `InspectOutArmPosition` 會對 shuttle 取料與旋轉站放料做位置比對並可能跳 `ShowMyMessage`。
3. **W12-31 / W12-32 是日期撞號的他案**（全智 `CC_GIGAS`、甬矽 `CC_FOREHOPE_NINGBO`），
   與 OutArm 主題無關，納入本波只為結清 20260810。LEDGER 的 case 歸屬要分開寫。
4. `aoutarm9045.cpp` 新增檔案層 `static AnsiString sOutArmRoundLog[20]`（全域建構子）。
   V899 出貨版即如此，BCB6 全量 build 下無問題；記錄備查。
5. `aoutarm9045.h` 加了 6 個宣告與 1 個 `#define`，該標頭被 `aArmHeader.h` 廣泛 include
   → **全量 build 時大量 .cpp 會重編**（波內只用 `bcc32 -c` 隔離編譯）。

---

## 7. 收工 gate 建議（給主腦）

1. **硬前提（套用前先驗，任一不符就停）**——見 §0.1 的 md5 表，重點三個：

   | 檔 | 必須是 | md5(12) |
   |---|---:|---|
   | `main.cpp` | **35339** 行 | `908ad65319dc` |
   | `cConfiguration.cpp` | **7867** 行 | `7057c81d976f` |
   | `cprod.cpp` | **4086** 行 | `ed7908e3b1d9` |

   （其餘 6 檔為 pristine：3984 / 208 / 1686 / 106 / 4783 / 4038，md5 見 §0.1。
   若期間有別波先動了這些檔，本波行號必須重量；`splice.py` 會因唯一錨不符而**整檔中止**，是安全失敗。）

2. `python tools/port_tools/splice.py docs/mg_w12_ops.json`
   - 預期輸出（9 檔）：
     ```
     OK aoutarm.cpp: 1 op(s) applied, eol=CRLF
     OK RotateKit\aRotateKIT.cpp: 1 op(s) applied, eol=CRLF
     OK aoutarm9045_2x4_4.cpp: 10 op(s) applied, eol=CRLF
     OK aoutarm9045.h: 2 op(s) applied, eol=CRLF
     OK aoutarm9045.cpp: 9 op(s) applied, eol=CRLF
     OK RotateKit\aRotateKIT_Out.cpp: 5 op(s) applied, eol=CRLF
     OK main.cpp: 2 op(s) applied, eol=CRLF
     OK cprod.cpp: 1 op(s) applied, eol=CRLF
     OK cConfiguration.cpp: 1 op(s) applied, eol=CRLF
     ```
   - 套用後行數：**3989 / 214 / 1769 / 115 / 4961 / 4096 / 35350 / 4086 / 7868**
   - 產生 9 個 `.mgbak`（目前樹上 `.mgbak` 為 0 個）

3. `python tools/port_tools/port_check.py`（不給參數即可，V910 樹只有這 9 個 M 檔）
   - 預期與 §3.5 的表**逐檔相同**：`added` 全數 SPLICED、`authored=0`、`comment=0`、
     `removed` 合計 **17**、EOL 全 CRLF 無 bare-LF。
   - **REMOVED 17 行的逐條清單見 §3.3**，主腦只要對照那張表即可。

4. `bcc32 -c -H-` 單檔隔離編譯 **6 個 .cpp**：`aoutarm.cpp`、`RotateKit\aRotateKIT.cpp`、
   `aoutarm9045_2x4_4.cpp`、`aoutarm9045.cpp`、`RotateKit\aRotateKIT_Out.cpp`、`main.cpp`
   （`cprod.cpp`、`cConfiguration.cpp` 各只改一行，建議一併編）。
   §0.3 已把可預見的編譯風險（符號可見性、`#include` 缺漏、名稱衝突、多載匹配）逐項排除，仍請實編確認。
   **`aoutarm9045.cpp` 是最需要編譯背書的一支**（135 行新程式 ＋ 6 處運動路由改寫）。

5. 主腦逐條開檔複驗，**重點四處**：
   - `aoutarm9045_2x4_4.cpp` 套用後 **33-61 行必須與 V899 的 33-61 完全相同**（`GetNowShuttleMode_2x4_4` 整支）。
   - `aoutarm9045.cpp` 套用後 `InspectOutArmPosition` 內的 **BulkBox 六行必須是 `//` 開頭**（W12-21），
     且其後的 `{` 與 `HardwarePosX=Prod.XStart[...]` 區塊完整保留。
   - `aoutarm9045.h` 套用後 **102-105 行仍是 V910 自有的四個宣告**、106 行才是 `#endif`。
   - `cprod.cpp` 套用後 **2662-2663 的 Eastsun `CC_KYEC_LEE` 兩行仍在**。

6. 收工後 `python tools/port_tools/ai_comment_matrix.py` 重算
   → 本波 33 條**全部**轉 PRESENT（矩陣以「`//AI(` 到行尾」的位元組簽章比對，我們是原樣插入）。
   - **只驗「減 33 條、且這 33 條正是本波的」**，不要拿總數當基準（基數會被別波漂移）。
   - 本波 33 條的檔／行清單：

     | 檔 | 行 |
     |---|---|
     | `aoutarm.cpp` | 3125, 3126 |
     | `RotateKit\aRotateKIT.cpp` | 114 |
     | `aoutarm9045_2x4_4.cpp` | 40, 155, 535, 558, 648, 654, 754, 779, 816, 901 |
     | `aoutarm9045.h` | 67, 95 |
     | `aoutarm9045.cpp` | 2100, 2341, 3792, 3826, 3842, 3857, 3866, 3910, 3928 |
     | `RotateKit\aRotateKIT_Out.cpp` | 694, 816, 918, 1131, 2252 |
     | `main.cpp` | 25566, 25791 |
     | `cprod.cpp` | 2535 |
     | `cConfiguration.cpp` | 2522 |

7. **LEDGER 記錄要點**
   - 20260810（21 條）**全結清**；20260811 殘餘 12 條結清 → **該日期列可標 done（PowerSavingMode 9 條 = MG-W8）**。
   - 類別：**D 類（OutArm 運動路由）**，忠實搬運，錨點乾淨，**<95% 項目 0 個**。
   - **本波沒有客戶碼閘門**，隔離來自機型（`e9045_2x4_4_13/_14`）與既有旗標（`bE74`、`bOutArmRoundLog`）。
   - 外溢五項見 §6.2（特別是「診斷 Log 全客戶預設開」與「W12-18 解除 early return」）。
   - 反向發現 R1（`aoutarm.cpp` RogerYang 20260326）已確認未被倒回。
   - 案件歸屬：0810 OutArm 群 = CASE-PTI-20260810-001/002；0811 = CASE-PTI-20260811-001 part 2；
     **cprod.cpp:2535 = 全智、cConfiguration.cpp:2522 = 甬矽（日期撞號，非 PTI）**。
   - 教訓一則可入 KNOWLEDGE：**Big5 原始碼不能用位元組層數括號**（§⚠4）。

8. **commit 後清 V910 樹 `*.mgbak`**（9 個），讓下一波的差異報告基準回到波前狀態。
