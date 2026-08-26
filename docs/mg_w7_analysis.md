# MG-W7 分析：20260430 / 0504 / 0511 / 0526 Multi EP 主題波

- **波次**：MG-W7（V899 → V910），LEDGER 相依鏈 #3
- **來源**：`docs/mg_ai_matrix_missing.csv` **53 條 MISSING**
  （0430×21、0504×17、0511×8、0526×7；**0429 在本檔案集 0 條**，見 §0.2）
- **V899 樹**：`D:\HT9045\HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422`（唯讀）
- **V910 樹**：`D:\HT9045\HT9011UC_Code_V3.33.910.0_20260716_Jimmy`（見 §0.1 md5 表）
- **本文件性質**：分析＋splice op 草案。**全程唯讀；兩棵樹 14 個檔的 md5 收工前後完全相同，未修改任何原始碼。**
- **op 檔**：`D:\HT9045\docs\mg_w7_ops.json`（**6 個 op**，只碰 2 個檔）

---

## ⚠ 本波最重要的一句話

**V910 已經有一份「公司自己做的」Multi EP 完整實作，而且有些地方比 V899 新。**

V910 樹內遍佈 `//==> //Eastsun 20260525 INSTALL_DOUBLE_EP_3 整合 begin/end` 標記
（7 個檔、20 組 begin），這是 Eastsun 在 **20260525** 獨立從 V874.3 整合 Multi EP 的結果；
之後還有 `Eastsun 20260616`、`Eastsun 20260625`、`Eastsun 20260710 Merge`、
`Frank 20260710`、`RogerYang 20260603` 等後續演進。

因此 **53 條 MISSING 裡有 40 條不是「缺功能」，而是 `ai_comment_matrix.py` 的
位元組簽章比對被「同功能、不同註解文字」騙了**（工具自己在 summary 裡也警告過
「未命中≠功能一定缺」）。本波真正要搬的只有 **6 條**。

| 分類 | 條數 | 意義 |
|---|---:|---|
| **C 類**（V910 已有，等價或更完整，只差註解文字） | **32** | 不動 |
| **X-none 類**（V899 的「刪除記錄」，但 V910 的分支是公司**新加**的功能） | **8** | 不動，**且絕對不可刪** |
| **X 類**（V910 有反向刻意修改／行為外溢 → `MG_FINAL_DECISIONS.md`） | **7** | 不出 op |
| **P 類**（真缺口 → 出 op） | **6** | `mg_w7_ops.json` |
| 合計 | **53** | ✓ |

---

## 0. 前置事實（全部實測，非推論）

### 0.1 檔案基本盤（收工前後 md5 相同＝證明唯讀）

| 檔 | V899 行數 / md5(12) | V910 行數 / md5(12) | EOL | cp950 |
|---|---|---|---|---|
| `AutoClean\uCleaning.cpp` | 2880 / `d227f5405e00` | 2922 / `df0cd9d3d1e6` | — | OK |
| `ContactForce.cpp` | 1585 / `846ba2c64865` | 1599 / `7ed89f72114e` | **兩樹全 CRLF** | OK / OK |
| `ContactForce.h` | 361 / `f836edc5313c` | 363 / `3c45979ce698` | — | OK |
| `HandlerSys.cpp` | 1368 / `ea2b17eca3a0` | 1537 / `50637b385db3`（**MG-W4 已動過**） | — | OK |
| `adam6024.cpp` | 3003 / `6b8d9072a628` | 3103 / `04a499cb14e5` | — | OK |
| `adam6024.h` | 91 / `34c81eca67f7` | 92 / `0fcdcf90b1a8` | — | OK |
| `cContact.cpp` | 20231 / `e48a55dc2565` | 22859 / `7ac509aad0f5` | — | OK |
| `cmydef.cpp` | 5865 / `c01247d95559` | 6005 / `d6d7c2e34c0f` | — | OK |
| `cmydef.h` | 5804 / `b49a9f8ad9b8` | 6024 / `82da779722bc` | — | OK |
| `database.cpp` | 2901 / `76ea2177e5f1` | 2921 / `dae79bf587b6` | — | OK |
| `iosetview.cpp` | 4045 / `d1e4f106e2ee` | 4255 / `952d53cf46db` | **兩樹全 CRLF** | OK / OK |
| `HS_Function.cpp` | 4838 / `a2fa16b35de3` | **5381** / `ccae6a71b287`（**MG-W5 已動過 +35**） | — | OK |
| `HS_Function.h` | 143 / `face57e25414` | 150 / `b7a8d38e6905` | — | OK |
| `cSetUp.cpp` | 4694 / `2f0edcb7414e` | 4867 / `684a5d776ae7` | — | OK |

> **本波只寫 `iosetview.cpp` 與 `ContactForce.cpp`**，兩者都是 pristine（無 `.mgbak`）、
> 全 CRLF、cp950 全檔可解。`HS_Function.cpp`（MG-W5 +35 行）與 `HandlerSys.cpp`（MG-W4）
> **本波一行都不碰**，所以 MG-W5/W4 的位移表不影響本波，本波也不製造新的位移給它們。

### 0.2 為什麼 0429 沒有條目

`grep "^20260429" docs/mg_ai_matrix_all.csv` 全量 **3 條**，狀態全是 `same-file`
（`main.cpp:10743`、`note.cpp:6689`、`note.cpp:6702`），且是甬矽 EventLog 引號案
（CASE-20260429-001）＝**MG-W3 的範圍，早已落地**。與 Multi EP 無關。
交派任務寫「0429 欣銓」是 LEDGER 相依鏈的標題文字，實際條目落在 0430 起。

### 0.3 休眠閘控驗證（F1 裁決前提）——**兩棵樹都齊備，不需一併搬**

| 項目 | V899 | V910 | 結論 |
|---|---|---|---|
| `#define DOUBLE_EP_NONE 0 / NORMAL 1 / INDIVIAL 2 / MULTI 3` | `cmydef.h:4075-4078` | **`cmydef.h:4151-4154`（存在）** | ✓ 值相同，`MULTI==3` |
| `//AI(...) 20260504: INSTALL_DOUBLE_EP value constants` 註解 | `cmydef.h:4072` | **`cmydef.h:4148`（已存在，故不在 MISSING）** | ✓ |
| `extern int INSTALL_DOUBLE_EP;` | `cmydef.h:4071` | `cmydef.h:4147` | ✓ |
| `int INSTALL_DOUBLE_EP=0;` | `cmydef.cpp:4223` | `cmydef.cpp:4281` | ✓ **預設 0＝DOUBLE_EP_NONE＝休眠** |
| 讀取來源 | `database.cpp:1089` `CheckAndReadIniDataGeneral("System","INSTALL_DOUBLE_EP",0)` | `database.cpp:1104` **同一行、同 section／key／預設值** | ✓ `Gerneral.ini [System] INSTALL_DOUBLE_EP` |

**結論：`INSTALL_DOUBLE_EP` / `DOUBLE_EP_MULTI` 在 V910 全部存在，無須一併搬；
未在 `Gerneral.ini` 設 3 的機台完全不受 Multi EP 相關碼影響。**

### 0.4 Eastsun 整合的覆蓋面（`grep "INSTALL_DOUBLE_EP_3"` on V910）

| 檔 | 標記數 |
|---|---:|
| `ContactForce.cpp` | 17 |
| `cSetUp.cpp` | 6 |
| `ContactForce.h` | 4 |
| `main.cpp` | 5 |
| `adam6024.cpp` | 3 |
| `HS_Function.cpp` | 3 |
| `iosetview.cpp` | 3 |

> 注意 `main.cpp`：V910 有 `labMultiEP`（`main.h:912`、`main.cpp:11117-11128`）
> 這個主畫面標籤，**V899 全樹 0 命中**。V910 在這一點是超集。

---

## 1. X-none 類：**「V910 需刪除的過時分支」清單 ＝ 空**

這是本波最容易踩死的地方。8 條 0430 條目的註解自述是
「removed iIndEPCnt==8 branch (rolled back)」，字面上會誘導人去 V910 刪 `iIndEPCnt==8`。
**實測結論：不可以刪。V910 的 `iIndEPCnt==8` 是 `RogerYang 20260603 : Add 8EP`，
是 Jimmy 20260430 移除之後一個月、公司在 V910 線上新加的功能，兩者是不同的碼。**

證據鏈（全部位元組級實測）：

| 層 | V899 | V910 | 判定 |
|---|---|---|---|
| `HandlerSys.dfm` `cbIndEPCnt` Items | `'16' '4'`（2 項） | **`'16' '4' '8'`（3 項）** | V910 UI 有 8EP 選項 |
| `HandlerSys.cpp` 讀 | 413-420 只有 16/else | **532-543 多 `else if(iIndEPCnt==8) ItemIndex=2` //RogerYang 20260603** | 活的 |
| `HandlerSys.cpp` 寫 | 937-941 `case 0:"16" case 1:"4"` | **1078-1083 多 `case 2: Str2="8";` //RogerYang 20260603** | 活的 |
| `database.cpp` | 778-785 只有 `=="16"` / else→4 | **790-801 多 `else if(sIndEPCnt=="8") iIndEPCnt=8;` //RogerYang 20260603** | 活的 |
| `adam6024.cpp` APAX 寫值 | 2401-2470 只有 `iIndEPCnt==4` / else | **2474-2567 五處 `iIndEPCnt==8` //RogerYang 20260603** | 活的 |
| `adam6024.cpp` 送出閘門 | 2472 `iArm==0\|\|iArm==2\|\|iIndEPCnt==4` | **2571-2572 多 `\|\| iIndEPCnt==8`** | 活的 |
| `ContactForce.cpp` SLKIndClass | 524 只有 `iIndEPCnt==4 && (...)` | **522-531 `(iIndEPCnt==4 \|\| (iIndEPCnt==8 && iTestMode==DualSite)) && (...)` ＋ `else if(iIndEPCnt==8 && j>=8)` //RogerYang 20260603** | 活的 |
| `cSetUp.cpp` bIndEPSLK | 2655 `INDIVIAL && DualSite` | **2714-2725 `if(INDIVIAL){ if(DualSite) … else if(CC_AMKOR_Korea && QualSite1X4) … }` //RogerYang 20260603** | 活的（AMKOR 客戶） |

**處置：這 8 條（`ContactForce.cpp:532`、`HandlerSys.cpp:421/942`、
`adam6024.cpp:2413/2431/2450/2468`、`database.cpp:786`）一律 no-op。
矩陣列會永遠留著，這是預期值不是漏搬。**
（它們在 V899 都是純註解行，本來就沒有可搬的程式碼。）

> 另注：`adam6024.cpp:2503` 的 `if(iIndEPCnt==16)` 在 V910:2603 **程式碼位元組相同**，
> 只是註解不同 → 已列入 C 類，不在上面 8 條內。

---

## 2. C 類明細（32 條，V910 已有，不出 op）

驗證方法：對每條抽出 V899 該行的**程式碼部分**（去 `//` 後正規化空白），在 V910 同檔全檔比對；
註解行則比對其所在區塊。以下「V910 位置」都是實際命中行號。

### 2.1 `AutoClean\uCleaning.cpp`（3 條，全 20260430）

| V899 | V910 | 備註 |
|---:|---:|---|
| 174 | **175** | 程式碼位元組相同；註解 `extend AutoClean DieForce to Multi EP half.` → `extend AutoClean Die Force visibility to Multi EP half.` |
| 183 | **184** | 同上 |
| 1146 | **1157** | 同上 |

（V899:1503 `grpDieForce->Visible=` 那條本來就是 `same-file`，故不在 53 條內。）

### 2.2 `ContactForce.cpp`（11 條 C）

| V899 | V910 | 說明 |
|---:|---:|---|
| 300 / 333（0504） | **209-328** | `THTDieForceOneByOneSLKClass` ctor。V910 把它移到 `THTDieForceSLKClass` **之前**（V899 在之後），內容其餘相同。**⚠ 反向發現見 §5.1：V899 353-356 有殘留的「砍半版」bug（`iCount<3` / `iCount-2`），V910 是正確的 `<5` / `-4`。** |
| 502（0504） | **500-513** | `if(MULTI){ for i; for j<8; SLKIndClass.push_back(..., i*8+j, ...) }` 逐字相同 |
| 615（0504） | **616-637** | OneByOne container。差異只有 630/632 用 `slDieForceSLKType` 而非 `slDieForceOneByOneSLKType`；兩者 `CommaText` **同樣來自 `edtDieForceCurrentType->Text`**（V910:605 vs 619）→ 內容恆等，**行為等價** |
| 694（0526） | **695-708** | V910 是**超集**：除了 `tsDynamicKit`/`TabSheet1`，還一併處理 `tsDieForceDynamicKit`/`tsDieForceOneByOneKit` |
| 710（0430） | **713-715** | `grpEPForDieForce->Visible` 與 `grpEPForArm2->Visible` 兩行逐字相同 |
| 1103（0504） | **1088-1105** | OneByOne ReadFile。V910 把它**內嵌在 `for(i<DieForceSLKClass.size())` 迴圈裡**、V899 是獨立迴圈跑 `slDieForceOneByOneSLKType->Count`；兩者對每個 i 都跑滿 j=0..7，**net 效果相同**（且兩樹都有同一個「字串無效時 index 位移」的既有隱患，非本波引入） |
| 1158（0504） | **1142-1160** | 逐字相同 |
| 1308（0504） | **1292-1307** | 逐字相同 |
| 1356（0504） | **1309-1321** | OneByOne WriteFile。V910 併進 MULTI 分支內、迴圈變數改 `ii`，**行為相同** |
| 1432（0430） | **1412-1418** | `trckbrDieForceOneByOneDiameter_Change`，V910 用 Eastsun 標記包住 |

### 2.3 `ContactForce.h`（1 條）

`ndiff` 結果：**V899 335 code-lines vs V910 335 code-lines，0 個 diff hunk**。
矩陣 352 的 per-site Die Force container 宣告在 V910 已存在（`ContactForce.h:353-357` Eastsun 標記內）。

### 2.4 `adam6024.cpp` C（2 條）／`adam6024.h`（1 條）

| 條目 | V910 | 說明 |
|---|---|---|
| `adam6024.cpp:407`（0430） | **402** | `if(INSTALL_DOUBLE_EP==DOUBLE_EP_INDIVIAL \|\| ==DOUBLE_EP_MULTI) Open_ADAM_6024("172.16.8.112", 2);` 逐字相同；註解是 `20260610` 版 |
| `adam6024.cpp:2503`（0430） | **2603** | `if(iIndEPCnt==16)` 逐字相同 |
| `adam6024.h:26`（0504） | **27** | **⚠ 不可搬，搬了會退版**：V899 是 `double MultiTransferKG(int fKpa, …)`，V910 是 `double MultiTransferKG(double fKpa, …)`（`//Eastsun 20260710 Merge`，V910 把舊的 int 版整行註解掉留在 26 行）。V910 的 `.cpp` 定義（974）同步是 `double`，**兩樹各自自洽**；V910 的 double 版避免 Kpa 小數被截掉 |

### 2.5 `cContact.cpp`（3 條）

| V899 | V910 | 說明 |
|---:|---:|---|
| 189（0430） | **196** | 逐字相同 |
| 13247（0504） | **14420** | 逐字相同 |
| 13089（0526） | **14262-14266** | `if(MULTI && IsMultiEPPressureRouteActive()) APAX_WriteData(false,0); else ADAM_DirectWriteData(...)` 逐字相同；連下方 14269-14271 的 `IsIndependentEPPressureRouteActive()` 刷新也一併在 V910（那條本來就是 `same-file`） |

### 2.6 `cmydef.cpp:4984` / `cmydef.h:4829`（2 條）

**這兩條在 V899 是純註解行、沒有任何程式碼。** 真正的宣告／定義
（`extern int iAPAXDualEPValue[16];  //Ifor 20250319`、`int iAPAXDualEPValue[16]={0,…};  //Ifor 20250319`）
在 V910 `cmydef.h:4910`、`cmydef.cpp:5051` **位元組完全相同**。

> 若 主腦 想清帳，可加兩個純註解 `after` op（零風險、零功能）。
> 本分析**依 MG-W5 §3 先例不主張做**，故不列入 `mg_w7_ops.json`。

### 2.7 `iosetview.cpp:157`（0430，1 條）

→ V910:**154** 逐字相同（`if(INSTALL_DOUBLE_EP==DOUBLE_EP_NORMAL || ==DOUBLE_EP_MULTI)` 包住 `labDOUBLEPA`/`labDOUBLEEPValue`）。

### 2.8 `HS_Function.cpp`（6 條）／`HS_Function.h`（1 條）——**本波一行都不碰**

| V899 | V910 | 說明 |
|---:|---:|---|
| 835 / 909 / 946 / 1001（0504） | **946 / 1032 / 1069 / 1125** | 四處 `==DOUBLE_EP_NORMAL \|\| ==DOUBLE_EP_MULTI` 逐字相同（1:1 對應，無多無少） |
| 973（0504，KYEC per-site EP log） | **1096-1098** | `if(CUSTOMER_CODE==CC_KYEC_LEE) sRecord=sRecord+anMultiEPData;` 逐字相同，V910 用 `//==> Eastsun 20260511 整合` 標記 |
| 1040（0526） | **1164** | `if(IsMultiEPPressureRouteActive()==true)` 逐字相同 |
| `HS_Function.h:90` | **`HS_Function.h:139`** | `bool __fastcall ReadMultiEP();` 存在（V910 把它挪到 private/後段） |

> **V910 的 `ReadMultiEP` 比 V899 領先一大截**，不可回搬：
> - V899:1042 `// ADAMTCP_ReadReg(ip, 5, 1, 24, buffer);  //V896 left commented; enable after hardware verify.` ＝**功能其實沒開**
> - V910:1166 `iRet=ADAMTCP_ReadReg(ip, 4, 33, 24, buffer);` ＝**真的讀了**，
>   加上 `Frank 20260710` 現場內插校正（`((((v-36922)/5343)*0.374)+0.056)*1000`）
>   與 `Eastsun 20260625` 京元要求的 ARM1/ARM2 遮罩。
> - 附帶差異：V899:1048 `MultiTransferKG(...)/4`，V910:1176 **無 `/4`**。這是兩邊各自校正
>   常數體系的一部分（V899 讀的是 raw、V910 讀的是內插後 Kpa），**不可單獨搬其中一半**。

### 2.9 `cSetUp.cpp:2655`（0504，1 條）

→ V910:**2726** `else if(INSTALL_DOUBLE_EP==DOUBLE_EP_MULTI && TestIF_File.iTestMode<=QualSite2X2N)` 逐字相同，
**而且 V910 是超集**：它把 V899 的 `INDIVIAL && DualSite` 展開成
`if(INDIVIAL){ if(DualSite){…} else if(CC_AMKOR_Korea && QualSite1X4){…} }`，保住 RogerYang 的 AMKOR 8EP。

> 附記（非本波範圍，但值得記一筆）：V910 這個展開讓
> 「`INDIVIAL` 且 非DualSite 且 非AMKOR」時 `bIndEPSLK` **不被賦值**（V899 會走 `else` 設成 false），
> 會沿用上一個 recipe 的殘值。屬 RogerYang 20260603 重構的既有缺陷，與 Multi EP 無關，本波不動。

---

## 3. X 類（7 條）→ 建議寫入 `MG_FINAL_DECISIONS.md`，本波不出 op

> **本分析 agent 全程唯讀，未動 `MG_FINAL_DECISIONS.md`。** 以下是給 主腦 直接貼上的建議列。

### 3.1 `adam6024.cpp:1627` / `1670`（20260526）——**V910 有反向刻意修改**

V910 **確實收過** Jimmy 這兩塊，但 **Eastsun 在 20260616 把它們整段註解掉**，
改成寫在 `for(i<slSLKTypeInd->Count)` 迴圈內部的版本：

| V910 行 | 內容 |
|---:|---|
| 1658 | `//if(INSTALL_DOUBLE_EP!=DOUBLE_EP_MULTI)  //AI(ht9045-v899) 20260610: …` → `{  //Eastsun 20260616 上面註解掉` |
| 1663-1692 | Eastsun 版：`if(MULTI && bDualForce==false){ for(j<8){ …SLKIndClass[i*8+j]… } continue; }` |
| 1693-1694 | `if(INSTALL_DOUBLE_EP==DOUBLE_EP_MULTI) continue;  //Eastsun 20260616 加個保險` |
| 1735-1790 | Jimmy 的 20260526 兩塊**被逐行 `//` 掉**，行尾寫著 `//Eastsun 20260616 註解掉` |

→ **這是白紙黑字的公司反向決定**，觸發 MG 規則「V910 有反向刻意修改 → <95%」。

```
| F? | MG-W7（20260526 adam6024 Multi EP 壓力填值） | V910 收過 Jimmy 的 20260526 兩塊（normal air force → outer/site、
DoubleForce → inner/dual），但 Eastsun 於 20260616 將其整段註解掉（V910 adam6024.cpp:1735-1790 逐行 //，
行尾註明「Eastsun 20260616 註解掉」），改用 1663-1694 的迴圈內版本＋兩道 continue 保險。
兩版對 iAPAXEPValue 的填法等價；差別是 Jimmy 版另外有 iAPAXDualEPValue（bDualForce==true）那一塊，
Eastsun 版沒有對應物 |
(a) 尊重 V910 現狀不搬 (b) 只補回 bDualForce==true 的 iAPAXDualEPValue 區塊 (c) 全部回搬並移除 Eastsun 版 |
**建議 (a)**：這是公司在 20260616 明確做過的反向決定，且 V899 的 Multi EP 是 SIGURD 專案碼、
V910 未必是同一台機。若要 (b)，需先向 Eastsun 確認 20260616 註解掉的理由。 |
```

### 3.2 `adam6024.cpp:1587` / `1595` / `1601`（20260511 bounds guard）

V899 的修法是：把舊 Independent EP loop 用 `if(INSTALL_DOUBLE_EP==DOUBLE_EP_INDIVIAL)` 圈住，
並加 `int iSLKIndSize=SLKIndClass.size(); if(idx>=iSLKIndSize) break;`。

V910 的等效保護是 **1693-1694 的 `if(MULTI) continue;`**（Eastsun 20260616 加）——
V899 註解裡描述的那個 OOB 情境（MULTI 時 `SLKIndClass[i*16+j]` 越界）在 V910 **無法發生**。

但 V910 **沒有** `idx>=size` 這道通用邊界防呆：若 `[SLK Type Ind]` 或 `SLK Type` 字串裡
出現空項／`<=15.0` 的項，`SLKIndClass` 的實際筆數會少於 `Count*16`（或 `Count*8`），
`SLKIndClass[i*16+j]` 仍可能越界。**V910 出廠預設 `"20,30"` 兩項皆有效 → 實務上碰不到。**

→ 補這道防呆等於**在 V910 重寫迴圈**（不是 splice V899 的行），違反「splice 不重打字」鐵律。

```
| F? | MG-W7（20260511 adam6024 SLKIndClass bounds guard） | V899 20260511 的 INDIVIAL 收斂＋
`idx>=SLKIndClass.size() break` 防呆，V910 用 Eastsun 20260616 的 `if(MULTI) continue;` 達到同等保護，
但缺通用邊界檢查。V910 迴圈結構與 V899 完全不同，無法 splice，只能重寫 |
(a) 不動（實務不可達） (b) 另開小波，在 V910 迴圈內手寫等效 bounds guard |
**建議 (a)**：V910 預設 `[SLK Type Ind]="20,30"` 兩項皆有效，`i*16+j` 上界 31 < 32 筆，
越界情境不可達；且此段在 EP 壓力計算路徑（D 類），重寫風險 > 收益。 |
```

### 3.3 `ContactForce.cpp:453`（20260526）——**閘外、且會孤兒化既有校正資料**

| | V899 | V910 |
|---|---|---|
| 行 | 454-455 | 452-453 |
| 碼 | `edtCurrentTypeInd->Text=edtCurrentType->Text;`<br>`edtVisibleInd->Text=edtVisible->Text;` | `edtCurrentTypeInd->Text=CheckAndReadIniData(FileName,"SLK Type Ind","Type","20,30");`<br>`edtVisibleInd->Text=CheckAndReadIniData(FileName,"SLK Type Ind","Visible","1,0");` |

實測影響（`slSLKTypeInd` 是 `INSTALL_DOUBLE_EP` **所有模式**共用的，不只 MULTI）：

1. `slSLKTypeInd->Count` 由 **2**（`20,30`）變成 **4**（`30,40,60,56`；KYEC 為 `28,40,58,56`）。
2. 非 MULTI 分支建 `THTSLKIndClass` 由 `2*16=32` 顆變成 `4*16=64` 顆；MULTI 由 16 變 32。
3. `adam6024.cpp` `TransformFuntion`（V910:1696-1731）以
   `SLKIndClass[i*16+j]->dDiameter==fDiameter*10` 配對 → **40/56/60mm kit 從「配不到、不寫
   iAPAXEPValue」變成「配得到、開始寫 per-site 值」**。這是 INDIVIAL（mode 2）機台的實質行為變更。
4. recipe 群組名由 `Diameter_20.000mm_0..15` / `Diameter_30.000mm_*` 變成
   `Diameter_30.000mm_*` / `Diameter_40.000mm_*` / … → **既有 INDIVIAL 機台存過的
   per-site LoadRate/ContactOffset 全部變孤兒，重讀時回預設 1.0 / 0.0。**
5. `[SLK Type Ind]` 這個 section 在 V910 `ContactForce.cpp` **只有讀、從來沒有寫**
   （全檔 grep 只有 452/453 兩處）→ 搬了之後客戶調過的值會被靜默忽略。

```
| F? | MG-W7（20260526 ContactForce SLK Type Ind 對齊） | V899 把 edtCurrentTypeInd/edtVisibleInd
改成直接鏡射一般 SLK Type（30,40,60,56），V910 仍讀 recipe [SLK Type Ind] 的 legacy "20,30"。
此變更**不在 INSTALL_DOUBLE_EP 閘內**：它改變所有 INDIVIAL(mode 2) 機台的 SLKIndClass 筆數、
讓 40/56/60mm kit 開始套用 per-site EP 值，並使既有 recipe 的 Diameter_20.000mm_* 校正群組
全部孤兒化回預設 |
(a) 不搬（V910 現狀） (b) 照搬 (c) 只在 INSTALL_DOUBLE_EP==DOUBLE_EP_MULTI 時鏡射，其餘維持讀 [SLK Type Ind] |
**建議 (c)**，但這需要新寫條件式（非 splice）→ 應由 主腦 裁決後另開實作波。
單純 (b) 會造成客戶端 EP 校正資料靜默歸零，屬 D 類風險。 |
```

### 3.4 `ContactForce.cpp:884`（20260430 Die Force OneByOne 分頁顯示）

`V899:874-898` 與 `V910:876-888` 都在 **`TfContactForce::FormShow`** 內（V899 fn 起於 684、V910 起於 685）。

| 模式 | V899 最終態（874-898） | V910 最終態（695-708 ＋ 876-888） | 差異 |
|---|---|---|---|
| `NORMAL(1)` | DynamicKit=true, OneByOne=false | 707→OneByOne=false；878→DynamicKit=true | **相同** |
| `MULTI(3)` + `bIndEPSLK` | DynamicKit=false, OneByOne=**true** | 700/701→(false,true)；887→DynamicKit=false | **相同** |
| `INDIVIAL(2)` + `bIndEPSLK` | 896/897 → 兩者皆 false | 700/701→OneByOne=**true**，887→DynamicKit=false | **不同**：V910 會顯示一個**空的** Die Force One-By-One 分頁（`DieForceOneByOneSLKClass` 只在 MULTI 時才 populate，V910:624） |
| `bUseDynamicKitDiameter==false` | 877/897 仍會設 OneByOne=false | 完全不碰 → 沿用 `.dfm` 設計期值 | 不同（同為外觀） |

- V899:888-892 那圈 `trckbrDieForceOneByOneDiameter->Min/Max=80/150` 在 V910 已由 ctor（298-299）設好，**冗餘**。
- 影響**純外觀**（多一個空分頁），無運動／IO／壓力後果。
- 但要修就得覆蓋 Eastsun 在 700-708 的寫法 → 屬「改公司碼」。

```
| F? | MG-W7（20260430 ContactForce Die Force OneByOne 分頁） | V899 FormShow 用
`else if(INSTALL_DOUBLE_EP==DOUBLE_EP_MULTI)` 決定 tsDieForceOneByOneKit 可見性；
V910 由 Eastsun 掛在 `(INDIVIAL||MULTI) && bIndEPSLK` 那個 if/else 裡。
NORMAL 與 MULTI 兩模式最終態相同；只有 INDIVIAL(mode 2)+bIndEPSLK 時 V910 會多顯示一個空分頁 |
(a) 不搬（純外觀差異） (b) 補 V899 的 else-if 與兩行 TabVisible=false |
**建議 (a)**：純外觀、無安全後果，且需覆寫 Eastsun 20260525 的區塊。信心 90% < 95%，不自行執行。 |
```

---

## 4. P 類：6 個 op（`docs/mg_w7_ops.json`）

### 4.0 為什麼只有 6 條

53 條裡 47 條要嘛 V910 已有（32）、要嘛是不可執行的刪除記錄（8）、要嘛 V910 反向改過（7）。
真正「V899 有、V910 沒有、且可位元組級 splice」的只有這 6 條，
**且全部集中在 20260511 的 AV 防護叢集 ＋ 一條 20260526 的 SAVE-reload。**

### 4.1 iosetview.cpp 20260511 AV 防護叢集（OP1–OP5，5 條）

> 交派任務點名的「iosetview 的 0511 條目有一部分是 RAII timer guard/null-guard（非 Multi EP，是 AV 防護）」
> ——實測：**0511 在 `iosetview.cpp` 的 5 條全部都是 AV 防護，一條 Multi EP 都沒有。**
> 該檔唯一的 Multi EP 條目是 0430 的 157，已是 C 類。

`Tfiosetview` ＝ **手動 IO 操作／測試畫面**（吸嘴、氣缸、EP、SafePLC LED）。

| op | 矩陣 | V899 範圍 | V910 動作 | 錨點（V910 全檔命中） | 淨行數 |
|---|---:|---|---|---|---:|
| **OP1** | 47 | `47-47` | `replace [50,50]` | `TMySucker *mySuckerTemp;` → **[50] 唯一** | ±0 |
| **OP2** | 108 | `108-114` | `after 110` | `bTimerRun=true;` → **[110] 唯一** | +7 |
| **OP3** | 165 | `165-169` | `replace [162,162]` | `if(bChangeSuckStatus==SStatusSuck)` → **[162] 唯一** | +4 |
| **OP4** | 222 | `220-222` | `replace [213,216]` | `RefreshSafePLCLed();` → **[213] 唯一** | −1 |
| **OP5** | 247 | `247-247` | `before 241` | `InitPairInfo_SafePLCIOLed();` → **[241] 唯一** | +1 |

合計 `iosetview.cpp` **4255 → 4266（+11）**。

**逐條說明**

- **OP1 — `mySuckerTemp` 初值**
  V910:50 是 `TMySucker *mySuckerTemp;`（**未初始化的全域指標**），V899:47 是 `=NULL`。
  `Timer1Timer` 每 tick 都會 deref 它。程式啟動後若在任何按鈕 handler 指派它之前
  就開啟本畫面，V910 會 deref 不定值。**嚴格更安全，零行為代價。**

- **OP2 — `TTimerRunGuard` RAII**
  V899:108-114（含結尾空行）插在 `bTimerRun=true;` 之後。
  ⚠ **精確描述**：實測 `Timer1Timer` 在 V899 與 V910 **body 內都沒有任何 early `return`**
  （grep `\breturn\b` 於 V910:110-216 與 V899:114-222 均 0 命中）。
  所以這個 guard 的價值**不是**擋 early return，而是**例外逸出**：body 內會呼叫
  `ADAM_ReadPA` / `ADAM_ReadVoltage` / `GetAD8Value_HotGenFlow` / `RefreshSafePLCLed`
  等硬體 IO，任一支丟例外就會跳過 `bTimerRun=false;`，之後這個 timer **永遠不再進入**
  （因為入口 `if(bTimerRun==true) return;`）→ LED 停止刷新、按下的按鈕不會變色。
  V899 註解自述的正是這一點。
  `bTimerRun` 兩樹都是 `static bool bTimerRun=false;`（V899:95 / V910:98）＝ function-local static，取址合法。
  BCB6 支援 local class 帶 ctor/dtor（C++98），V899 本身就是 BCB6 編出來的。

- **OP3 — null-guard**
  V910:162 `if(bChangeSuckStatus==SStatusSuck)` 直接 deref `mySuckerTemp->Suck()`；
  V899 在前面加 `if(mySuckerTemp==NULL){ bChangeSuckStatus=SStatusOff; }`，
  原來那行變成 `else if`。取代區**只有錨行一行**，下面 Suck/Destory 兩個 body 完全不動。

- **OP4 — 移除單一出口的 `bTimerRun=false;`**
  ⚠ **OP2 是 OP4 的硬前提**：OP4 把 V910:216 `bTimerRun=false;` 換成 V899:222 的註解行，
  之後只剩 guard 的解構子在清旗標。`splice.py` 對同一檔是 all-or-nothing，**不可能只落一半**。
  取代區刻意從 `RefreshSafePLCLed();`（213）起，是為了拿到唯一錨；
  **V910 自己的 `IsSafePLCIOInstall()` 閘（211）在取代區外，原封不動保留**
  （若照抄 V899:218 會把 V910 的 `Enable_PLCSafety_IO` → `IsSafePLCIOInstall()` 重構倒回去）。
  模擬結果已驗證 V910:222 仍是 `if(IsSafePLCIOInstall()==true)`。

- **OP5 — FormShow 清殘留**
  V899 的 `FormShow` 第一行就是 `bChangeSuckStatus=SStatusOff;`；V910 第一行是
  `InitPairInfo_SafePLCIOLed();`（公司新加）。用 `before` 插在它之前 ＝ 與 V899 同序，
  且 V910 那行完全不動。語意：上次關閉畫面時若停在 Suck/Destory 中途，
  重開時不會拿舊的 `mySuckerTemp` 繼續動作。

**閘控歸屬：閘外（無 `INSTALL_DOUBLE_EP` 閘、無客戶碼閘）。**
但五條全是**單向更安全**的防護（NULL 初值、NULL 檢查、例外清旗標、開窗清狀態），
不改任何致動邏輯、不改任何氣缸／吸嘴的實際輸出條件。
**信心：≥95%。**

### 4.2 `ContactForce.cpp:970`（OP6，1 條，20260526）

| | 內容 |
|---|---|
| 位置 | `TfContactForce::btSaveClick`（V899:945-973 / V910:935-961） |
| V899 範圍 | `967-972`（`SaveLastSetIni();` / 空行 / `WriteFile();` / **AI 註解** / **`ReadFile();`** / `ADAM_WriteVoltage(DeviceForm.dPress);`） |
| V910 取代區 | `957-960`（同四行，缺註解與 `ReadFile();`） |
| 模式 | `replace`（4 → 6，淨 **+2**） |
| 錨點 | `SaveLastSetIni();` → **V910 全檔 [957] 唯一** |
| 為何擴大取代區 | `WriteFile();` 在 V910 命中 **[643, 959, 1139]**、`ADAM_WriteVoltage(DeviceForm.dPress);` 命中 **[923, 931, 960]**，兩者都不唯一；擴到 `SaveLastSetIni();` 才有全檔唯一錨。四個上下文行已逐位元組驗證 V899[967/968/969/972] ≡ V910[957/958/959/960]（全部 SAME） |

**語意**：SAVE 之後多呼叫一次 `ReadFile()`，把剛寫進 recipe 的值重新讀回記憶體與 UI。
主要對 MULTI 有意義（`DieForceOneByOneSLKClass` 的 LoadRate/ContactOffset 由
`WriteFile` 1309-1321 寫、`ReadFile` 1088-1105 讀），對其他模式是一次 round-trip 重讀。

**風險評估**
- `WriteFile()` 剛把 UI 值全部寫入同一個 `FileName`，`ReadFile()` 再讀回 ⇒ 值 round-trip 相同。
- 唯一實質差異：`ReadFile()` 會做 `CheckRange`（LoadRate 0.8~1.5、ContactOffset ±10）夾限，
  使用者若輸入超界值，SAVE 後記憶體會變成夾限值。方向是**更保守**。
- `ReadFile()` 內的 `if(bNeedWriteFile==true)` 分支（KYEC 30→28 轉換）只在 ctor 路徑會被設 true
  且當場清為 false，此處恆為 false，不會遞迴 `WriteFile()`。
- 之後的 `ADAM_WriteVoltage(DeviceForm.dPress);` 用的是重讀後的值 —— 這正是註解說的目的。

**閘控歸屬：閘外（無條件，所有客戶／所有 `INSTALL_DOUBLE_EP` 值都跑）。**
屬 **D 類（Contact Force 壓力設定路徑）**，但變更是「忠實搬運＋錨點乾淨」，
不改寫任何計算式。**信心：≥95%。**

### 4.3 事前模擬（記憶體內套用，未落盤；模擬檔已刪除）

| 檔 | 套用前 | 套用後 | Δ | cp950 | CRLF / bare-LF | AI 簽章落地 |
|---|---:|---:|---:|---|---|---:|
| `iosetview.cpp` | 4255 | **4266** | +11 | OK | 4266 / **0** | 5 / 5 |
| `ContactForce.cpp` | 1599 | **1601** | +2 | OK | 全 CRLF / **0** | 1 / 1 |

六個 op 的錨點檢查全部 `OK`；逐 op 對帳 `+0 / +7 / +4 / −1 / +1 = +11`、`+2` ✓。
模擬輸出的關鍵區段已逐行目視確認（guard 結構正確、`else if` 接得上、
`IsSafePLCIOInstall()` 保留、`InitPairInfo_SafePLCIOLed()` 保留、`ReadFile();` 落在 `WriteFile();` 與 `ADAM_WriteVoltage` 之間）。

**矩陣預期**：53 條 MISSING 中 **6 條轉 PRESENT**，剩 **47 條永久保留**
（32 C 類＋8 X-none＋7 X 類）。這是預期值，不是漏搬。

---

## 5. 反向發現（V899 有問題、V910 沒有）——**不可回搬**

### 5.1 `ContactForce.cpp` — V899 殘留「砍半版」bug

`THTDieForceOneByOneSLKClass` ctor 內，三處 Multi EP caption 的分支條件應該一致：

| 位置 | V899 | V910 |
|---|---|---|
| GroupBox caption（`Arm1_%d` / `Arm2_%d`） | 310-313 `iCount<5` / `iCount-4` ✓ | 220-223 `iCount<5` / `iCount-4` ✓ |
| Diameter label（`%s mm 1_%d:`） | 334-337 `iCount<5` / `iCount-4` ✓ | 243-246 `iCount<5` / `iCount-4` ✓ |
| **ContactOffset label** | **353-356 `iCount<3` / `iCount-2`** ✗ | **262-265 `iCount<5` / `iCount-4`** ✓ |

`iCount=(Tag%8)+1`（1..8），設計是 1-4→Arm1、5-8→Arm2。
V899 的第三處用 `<3` / `-2`，是 20260430「砍半版」（2 site×2 arm）的殘留，
20260504 full port 時漏改 → **同一顆 GroupBox 的標題寫 `Arm1_3`、裡面的 offset 標籤卻寫 `2_1`**。
純標籤文字、不影響數值，但 **V910 是對的**。

→ **鐵律 #1 的教科書案例**：若照「V899 最終態」盲搬 ctor，會把這個 bug 帶進 V910。本波不動。

### 5.2 `adam6024.h` / `adam6024.cpp` — `MultiTransferKG` 參數型別

V899 `int fKpa`（Kpa 小數被截斷）vs V910 `double fKpa`（Eastsun 20260710 Merge，
行尾註明「Eastsun 修正沒有小數點」）。**V910 較佳，不可回搬。**

### 5.3 `HS_Function.cpp` — `ReadMultiEP` 在 V899 其實沒開

V899:1042 把 `ADAMTCP_ReadReg` 註解掉並寫 `enable after hardware verify`；
V910:1166 已實際啟用並帶 `Frank 20260710` 現場校正常數。**V910 領先，不可回搬。**

---

## 6. 「V910 需刪除的過時分支」清單

## **＝ 空。一條都沒有。**

理由見 §1：V899 的 5 條 `iIndEPCnt==8` 刪除記錄 ＋ 3 條相關 ComboBox/ini 刪除記錄，
在 V910 對應的不是「Jimmy 刪掉的舊碼」，而是 **`RogerYang 20260603 : Add 8EP`
在 V910 線上新加的完整功能**（`.dfm` 選項、`HandlerSys` 讀寫、`database` 解析、
`adam6024` APAX 寫值與送出閘門、`cSetUp` AMKOR 專案分支，六個層次全齊）。
**刪掉等於砍掉公司一個活的客戶功能（含 `CC_AMKOR_Korea`）。**

---

## 7. 收工 gate 建議（給 主腦）

1. **順序前提**：本波 op 的行號量在
   `iosetview.cpp`（md5 `952d53cf46db`、4255 行、無 `.mgbak`）與
   `ContactForce.cpp`（md5 `7ed89f72114e`、1599 行、無 `.mgbak`）上。兩檔皆 pristine。
   本波**不碰** `HS_Function.cpp`（MG-W5 已 +35）與 `HandlerSys.cpp`（MG-W4 已動），
   所以與 W4/W5 無交互位移。

2. `python tools/port_tools/splice.py docs/mg_w7_ops.json`
   - 預期：`OK iosetview.cpp: 5 op(s) applied, eol=CRLF`
     ／`OK ContactForce.cpp: 1 op(s) applied, eol=CRLF`
   - 產生 `iosetview.cpp.mgbak`、`ContactForce.cpp.mgbak`

3. **套用後位移表**（後續波若要動這兩檔，行號請用這張表換算）

   `iosetview.cpp`（4255 → 4266）
   ```
   N ≤ 49            → N
   N = 50            → 50            （OP1 replace 1→1）
   51 ≤ N ≤ 110      → N
   111 ≤ N ≤ 161     → N+7           （OP2 插 7 行於 110 之後）
   N = 162           → 169..173      （OP3 replace 1→5）
   163 ≤ N ≤ 212     → N+11
   213 ≤ N ≤ 216     → 224..226      （OP4 replace 4→3）
   217 ≤ N ≤ 240     → N+10
   241 ≤ N           → N+11          （OP5 插 1 行於 241 之前）
   ```
   `ContactForce.cpp`（1599 → 1601）
   ```
   N ≤ 956           → N
   957 ≤ N ≤ 960     → 957..962      （OP6 replace 4→6）
   N ≥ 961           → N+2
   ```

4. `bcc32 -c -H-` 單檔隔離編譯 `iosetview.cpp`、`ContactForce.cpp`。
   OP2 的 local struct 帶 ctor/dtor 是 C++98 合法寫法，V899 本身即以 BCB6 編出；仍請實編確認。

5. Gate 驗證：
   - `python tools/port_tools/port_check.py`（不給參數即自動抓 V910 樹的 M 檔）
     → 預期新增行全部判 **SPLICED**（本波所有 payload 皆逐位元組取自 V899 同路徑檔），
       **0 個 AUTHORED**、cp950 錯誤數 0、EOL 全 CRLF 無混用。
   - `python tools/port_tools/ai_comment_matrix.py` 重算矩陣
     → 總 MISSING **330 → 324**（本波 53 條中只有 6 條會消失，其餘 47 條依 §1–§3 永久保留）。

6. **LEDGER 記錄要點**（本波的價值不在行數）
   - 相依鏈 #3「Multi EP」**實質已由 Eastsun 20260525 在 V910 完成**，V899→V910 只剩 6 條。
   - 「V910 需刪除的過時分支」清單經實測為**空**；`iIndEPCnt==8` 是 RogerYang 20260603 的新功能。
   - 三筆**反向發現**（§5）已記錄，V899 側若要修屬 `ht9045-v899` 的議題。
   - 7 條建議進 `MG_FINAL_DECISIONS.md`（§3 四個 F 列草稿）。
