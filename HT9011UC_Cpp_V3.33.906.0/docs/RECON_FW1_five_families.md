# RECON: FW-1 五大未知 tag family 偵察（arm/cat/contact/speed/bin）

AI(W906-FW1-recon2) 20260819: 偵察報告，非交付波次。範圍是
`docs/RECON_FW1_tag_crosswalk.md` 第 7 節明列「完全沒有深入調查」的五個 family
（`arm.*` 64、`cat.*` 32、`contact.*` 18、`speed.*` 12、`bin.*` 6，合計 130 個
綁定 tag）。**只寫了本檔一個新檔，未改動任何其他檔案，未 commit，未跑
build/ctest。** 所有 golden 引用皆用 Python `decode('cp950')` 讀取
`HT9011UC_Code_V3.33.906.0_20260618`（唯讀），所有 V906 引用用 Read/Grep 讀取
`HT9011UC_Cpp_V3.33.906.0`（本次未寫入）。時間戳一律 20260819（今天）。

---

## 0. 一句話總覽

五個 family 沒有一個「打開 gate 就能接」；深度分屬五種不同的死法，且**其中三處
發現本樹既有註解已經過期**（cSocket.h 的 ODR 衝突警告、atester_ProcessCount.cpp
與 Command.cpp 的「TastCategory/ShowBinSel 尚未翻譯」gate 註解）——這些過期註解
本身就是本次最大的風險：後續波次若照抄舊註解會重新推導出錯誤結論。

| family | bound | golden 來源 | V906 現況 | 可接數/總數 | 卡點類別 |
|---|---:|---|---|---:|---|
| `arm.*`     | 64 | `UN150Read[tcAa1..tcBh2]`（bthermo.cpp） | 寫入路徑 100% 卡在未翻譯 ATC 介面 | 0/64 | 硬體介面死結 |
| `cat.*`     | 32 | `TastCategory.iCountHeadTotal[arm][row][col]`（cSocket.cpp） | 資料結構活的，唯一填值呼叫點被 `#if 0` | 0/32 | gate 阻斷（可能過期） |
| `contact.*` | 18 | 未確認——18 個 label 對不上任何 golden 命名 | 命名/形狀本身未定案 | 0/18 | 設計未定案（先於 liveness） |
| `speed.*`   | 12 | `cSpeed.cpp/h/dfm`（recipe 專屬 XY 速度/加減速%） | **完全未翻譯**，0 個 facade | 0/12 | 整個表單缺口 |
| `bin.*`     | 6  | `Prod.iT6CatData[]` → `ShowBinSel()` → `MyBinSel[].Caption` | **顯示鏈今天剛翻完且 ACTIVE**，但上游賦值函式「有函式無呼叫者」 | 0/6（最淺） | 缺一個呼叫點（翻譯決策） |

---

## 1. `arm.*`（64 個：`arm.A1`…`arm.P2` 各一值 + `.tone`）

### 1.1 綁定側

`web/js/panels/left.js:13` 匯入 `ARM1_ROWS, ARM2_ROWS`；`:97-98`：
```js
block({ title: "Arm 1" }, valTable("arm", ARM1_ROWS)),
block({ title: "Arm 2" }, valTable("arm", ARM2_ROWS)),
```
`web/js/model/state.js:17-24`：
```js
export const ARM1_ROWS = [["A1","C1","E1","G1","I1","K1","M1","O1"],
                           ["B1","D1","F1","H1","J1","L1","N1","P1"]];
export const ARM2_ROWS = [["A2","C2","E2","G2","I2","K2","M2","O2"],
                           ["B2","D2","F2","H2","J2","L2","N2","P2"]];
```
`web/js/ui/widgets.js:129-138`（`valRow`）：每個 head 產生 `data-tag: arm.${k}` +
`data-tone-tag: arm.${k}.tone` —— 16 個 head × 2（值+tone）× 2 arm = 64。

### 1.2 golden 來源：`ShowTempComp[]` 的底層數字

V906 `forms/fTemperFrom.h` 已在 Wave A 翻譯 `NameTempComp[]`/`ShowTempComp[]`
（71 個 channel panel，`MachineType.h:637-654` 的 `eTempControll` enum），但那是
**顯示層**（`TPanel*` 陣列，只有 Caption/Color）。真正的數字來源是同一個 enum
索引進去的 `UN150Read[]`（present-value 陣列，`cmydef.h`，`WebBridgeTags.cpp:107`
已有註解確認其存在）。

`MachineType.h:639-644`：加熱區代碼是兩字母（`tcAa1..tcAh1`／`tcBa1..tcBh1` 為
Arm1，`tcAa2..tcBh2` 為 Arm2，各 16 個），**不是**網頁用的單字母 `A1..P1`。

golden `cTemperFrom.dfm`（python cp950 解碼，20260819 執行）第 465-1986 行的
Caption 字面值證實：真實顯示名稱是 `'Aa 1'`、`'Ab 1'`…`'Bh 2'`（16 碼 × 2 臂），
沒有任何一個 Caption 字面是 `"A1"`/`"B1"` 這種單字母格式。`web/js/model/tagmap.js:134`
自己也承認：`{ prefix: "arm.", owner: "heater zone channels A1..P2 -- source
form unresolved" }`——這不是本次新發現，但本次坐實了「不只是沒接，連字母對應
規則都沒有人定過」。**在來源解封之前，這是一個獨立於 liveness 的開放設計問題：
16 個雙字母 golden 代碼要按什麼順序對應到 16 個單字母網頁代碼，需要人工拍板，
不能用位置猜。**

### 1.3 為什麼寫不進去：`DOUN150ReadTemp()` 的每一個分支都被 `#if 0` 封印

`bthermo.cpp:3437-3442`（`DoThermoReal()` 狀態機 `case 270`，`Addr` 走
`tcAa1..tcBd2`/`tcAe1..tcBh2`/`tcHead1..tcHead4` 時**確實可達**）：
```cpp
case 270:
    if((Addr>=tcAa1 && Addr<=tcBd2) || (Addr>=tcAe1 && Addr<=tcBh2) || ...)
    {
        UN150Read[Addr]=DOUN150ReadTemp(Addr);
```
`bthermo.cpp:4532` 起的 `DOUN150ReadTemp(int Addr)`：`dTempShow=0.0` 初始化後，
依 `Use_Heat_Count`（4 / 8,16,24,32 / 40）分三個 `switch` case，**每一個 case
的整段真實計算都在 `#if 0` 裡**：
- `case 4`：`:4539-4671`（G27 + G28a），依賴 `ATC_InterfaceForm`/`ATCInterfaceForm`/
  `fWinway`（皆未翻譯）。
- `case 8/16/24/32`：`:4680-4699`（G28b），同依賴。
- `case 40`：`:4705-`（G28c），同依賴。

三個 `#if 0` 區塊內部都還嵌了一層 `#ifdef SOFT_SIMULTE ... dTempShow=Addr; #else
dTempShow=9999; #endif` 的 golden 假資料退路——但因為外層整段被 `#if 0`，連這條
「假資料至少非零」的退路都不可達。**結論：`Use_Heat_Count` 不管等於哪個值，
`DOUN150ReadTemp()` 恆回傳初始值 `0.0`**，`UN150Read[]` 對 `tcAa1..tcBh2` 這
32 個位址永遠是 0.0。

`WebBridgeTags.cpp:110-116` 已經有一個現成的 liveness predicate
`TemperaturePvLoaded()`：
```cpp
bool TemperaturePvLoaded() {
    for (int i = 0; i < tcTotalCount; ++i) if (UN150Read[i] != 0.0) return true;
    return false;
}
```
註解寫「Nothing writes it either」——本次追蹤發現這句話**不完全精確**：
確實有程式碼在寫（`bthermo.cpp:3442`），只是寫進去的值恆為 0.0，所以對外行為
（predicate 恆假）是對的，但「沒人寫」這個因果描述是錯的，正確說法應該是
「有人寫，但寫的永遠是初始值」。這個區別在未來 ATC 介面翻譯完成後很重要：
屆時只要 `case 4/8/16/24/32/40` 任一個 `#if 0` 解封，`UN150Read[]` 就會立刻
開始出現非零值，`TemperaturePvLoaded()` 不用改就會自動生效——這個 predicate
現在就可以直接借來當 `arm.*` 的 liveness key，不必等 ATC 波次完工後再補。

### 1.4 verdict

**0/64 可接**。卡點是硬體介面表單（`ATC_InterfaceForm`/`ATCInterfaceForm`/
`fWinway`，golden `ATC/*`/`acarry.h`）完全未翻譯，屬於整條翻譯戰役級別的缺口，
不是接線波次能解的。附帶的獨立開放問題：16 碼命名順序對應表（`Aa1→?`）需要
人工定案，即使來源解封了也不能省略這一步。

---

## 2. `cat.*`（32 個：`cat.arm{1,2}.s{1..16}`，`muted` 樣式）

### 2.1 綁定側

`web/js/panels/right.js:16-20`：
```js
function testCategoryPanel() {
  return panel({ title: "Test Category" },
    siteGrid({ tagPrefix: "cat", arms: [1, 2], muted: true }));
}
```
`web/js/ui/widgets.js:101-121`（`siteGrid`）：每個 arm 2 列（A/B）× 8 欄，
`data-tag: cat.arm${arm}.s${site}`，`data-bind:"lit"`（純亮/暗指示，無 `.tone`）
——32 個，和已完工的 `site.*` 用同一份產生器，只是 `tagPrefix` 換成 `cat`。

### 2.2 golden 來源：`TastCategory.iCountHeadTotal[iArm][iRow][iCol]`

`cSocket.h:217-247`（`TEST_CATEGORY` struct）：
```cpp
int iCountHeadTotal  [2][MAX_SOCKET_ROW][MAX_SOCKET_COL];   //[Arm][Row][Col]
```
`MachineType.h:391-392`：`MAX_SOCKET_ROW=4`、`MAX_SOCKET_COL=8`。`cSocket.cpp:1258-1296`
（`TEST_CATEGORY::UpdataCount(bool bCheckYield)`，`bCheckYield==true` 分支）：
```cpp
iCountHeadTotal[iArm][iRow][iCol] += ArmData[iArm]->ArmSKET[iRow][iCol]->GetTotal();
```
——這就是「某臂、某 row/col 的 site 累積測試數」，語意上正是 `cat.arm{N}.s{M}`
想要的東西。

**⚠️ 結構性警訊（未解決，留給下一波）**：`iCountHeadTotal` 的維度是
`[2][4][8]`，而網頁的 `cat.arm{1,2}.s{1..16}` 隱含的是「每個 arm 自己 2 列 ×
8 欄 = 16 site」。已完工的 `site.*` 對照組讀的是 `LastSet.bUseTestSocket[a-1]
[(n-1)/8][(n-1)%8]`（`WebBridgeTags.cpp:428-450`），其 row 維度只需要 0-1
（因為 `(n-1)/8` 對 n=1..16 只吐 0 或 1）。`TastCategory` 的 row 維度卻是 0-3，
比 `site.*` 的來源陣列多一倍。這代表 **`iCountHeadTotal` 的 `[Arm][Row]` 兩維
很可能不是「網頁的 arm1/arm2 × 2 列」這種切法**（也許 4 列本身就橫跨兩臂，
`iArm` 維度是另一個獨立概念，例如量測點 1/2）——本次沒有時間逐一比對
`ArmSKET[][]` 的 row/col 填值來源把這件事坐實，**這是接線前必須先解掉的第二個
開放設計問題**，即使 liveness 通了也不能假設索引可以直接位置對位。

### 2.3 為什麼寫不進去：唯一的自動填值呼叫點被 `#if 0`

`atester_ProcessCount.cpp:642-649`：
```cpp
// AI(W5-ProcessCount-Translate) 20260710: gate #1 (TastCategory / cSocket.cpp,
// golden :588-591) -- see file header.
#if 0 // TODO(cSocket-module): TastCategory (TEST_CATEGORY, golden cSocket.h:144-174) has no translated home yet.
    if(CosFunction.bBySiteByBinPercentCompare==true)
    {
        TastCategory.UpdataCount(true);
    }
#endif
```
**這個 gate 註解已經過期**：寫於 20260710（W5-ProcessCount-Translate），當時
`cSocket.cpp` 確實還沒有翻譯家；但 `cSocket.cpp` 已在 PT-W2/PT-W3（20260807-08）
落地並掛進 `ht9045_sm`（`CMakeLists.txt:2068`）。用 `nm` 對現有 build
（`build_fww3g/libht9045_sm.a`，20260819 執行）驗證符號歸屬：

```
libht9045_sm.a:SCK_ART_Remainder.cpp.obj:         U _TastCategory   (未定義，只是引用)
libht9045_sm.a:cSocket.cpp.obj:00031d60 B _TastCategory              (真實定義)
```
`wb_publish.exe` 本身也只有一個 `_TastCategory`（`00e6e800 B`），沒有連結錯誤。
交叉核對 golden 端已經自己記錄了這次除役：`Automation/SCK_ART_Remainder.cpp:360-366`
```
// ---- Gate #16 [RETIRED -- AI(W906-PT-W3-integrate) 20260808]: TastCategory ----
//   The stub definition that used to live here ... is gone. cSocket.cpp landed
//   in PT-W2 and defines the REAL `TEST_CATEGORY TastCategory;` ...
```
也就是說 **`cSocket.h:40-61` 那段「NOT fixed here」的 ODR 衝突警告本身也已經
過期**（同一個問題的另一份文件，`SCK_ART_Remainder.cpp` 已經記載它被解決了，
但 `cSocket.h` 的舊警告沒有跟著更新）——這是本次找到的第三處過期註解，三處
共通點都是「翻譯進度往前走了，但某個檔案裡描述『尚未翻譯/尚未解決』的舊註解
沒有跟著撤下」，值得往後任何波次在引用 gate 註解前先用 `nm`/git log 核對
是否仍然成立，不能只看註解文字。

`UpdataCount(true)` 除了這個死呼叫點外，還有三個活的呼叫點——`cObserver.cpp:2527`
（`TfObserver::WriteCategoryData`，Monitor 視窗刷新，只在該視窗被打開時觸發）、
`Command.cpp:9689`（`TfMain::GetBinCountByDLL`，外部 DLL 主動查詢時才觸發）、
`cCounterClear.cpp:151/396`（Counter Clear 對話框按鈕觸發）——三者皆非
pump tick 期間會自動執行的路徑，本次未逐一排除「是否有任何初始化流程會呼叫
其中之一」，如實列為未查證。

### 2.4 潛在替代路徑（本次未驗證，留給下一波）

`cContactCT.cpp:571-617` 直接讀 `ArmData[Arm]->ArmSKET[i][j]->GetTotal()`（同一份
底層計數器，繞過 `TastCategory.UpdataCount()` 這層彙整），如果 `ArmSKET[][]`
本身的 `SetPassCT`/`SetBinCT`（`cSocket.cpp:415-465`、`TArm::SetPassCT`
`cSocket.cpp:904-920`）在測試流程中是被即時呼叫的（本次未追這條——需要找
`atester.cpp`/`atester_ProcessCount.cpp` 裡誰呼叫 `ArmData[iArm]->SetBinCT(...)`），
那麼繞過 `TastCategory` 直接讀 `ArmData[]->ArmSKET[i][j]->GetTotal()` 可能是比
解開 `#if 0` gate 風險更低的路徑（不涉及翻譯行為決策，只是換一個讀取點）。
**這是下一波該做的第一件事，本次未坐實。**

### 2.5 verdict

**0/32 可接**。資料結構本身是活的（`cSocket.cpp` 已翻譯且真實連結），唯一自動
填值呼叫點被 `#if 0`，解開它是翻譯行為決策（`CosFunction.bBySiteByBinPercentCompare`
本身又掛在恆假的 `CosFunctionLoaded()` 下，即使解開 gate 也不會自動變活）。
索引維度（`[2][4][8]` vs 網頁預期的每臂 16 site）是否能直接位置對應到
`cat.arm{1,2}.s{1..16}`，本次未驗證，是接線前的必答題。

---

## 3. `contact.*`（18 個：`contact.<kind>`，`pct2` 格式）

### 3.1 綁定側

`web/js/panels/right.js:24-46`（`contactPanel`），`web/js/model/state.js:27-30`：
```js
export const CONTACT_KINDS = [
  ["Aa","Ca"], ["Ab","Cb"], ["Ac","Cc"], ["Ad","Cd"],
  ["Af","Cf"], ["Ag","Cg"], ["Ah","Ch"], ["Ba","Da"], ["Bb","Db"],
];
```
表頭是「Arm 2 | Arm 1」，每列 `[left,right]`：`left` 同時是顯示文字**也是**
tag 尾碼（`contact.${left}`），`right` 亦然——18 個 tag = 9 對 × 2。

### 3.2 這 18 個代碼對不上任何已知 golden 命名（本次最大的意外）

一開始假設這組兩字母代碼是 `MachineType.h:639-644` 那組加熱區命名
（`tcAa1..tcAh1`/`tcBa1..tcBh1`）的重用，逐一核對後**不成立**：
- 加熱區命名有 16 碼（`Aa,Ab,Ac,Ad,Ae,Af,Ag,Ah,Ba,Bb,Bc,Bd,Be,Bf,Bg,Bh`，
  無缺漏）；`CONTACT_KINDS` 左半只有 9 碼且**跳過 Ae**、只到 `Bb`（缺
  `Bc..Bh`）；右半是 `Ca,Cb,Cc,Cd,Cf,Cg,Ch,Da,Db`，同樣跳過 `Ce`、只到 `Db`。
  兩組都不是任何一個規則子集，看起來是任意挑的佔位字串。
- 對 golden `cContactCT.dfm`（python cp950 解碼，20260819）逐行掃
  `Caption = 'A...'`/`'B...'`/`'P...'`：**只有 3 個 Caption**
  （`'Contact Counter Kinds'`/`'Count Clear'`/`'Yield Chart'`），沒有任何
  逐格 Caption——golden 這個表格是用 `TStringGrid`（V906 `forms/fContactCT.h:218`
  `class TfContactCTGrid : public vclcompat::TStringGrid`）程式化填值，不是
  `.dfm` 設計期字面值，所以 dfm 掃描本來就找不到這組代碼，這點不構成反證，
  但也代表**沒有設計期文件可以拿來核對這 18 碼是否「真的」對應到什麼**。
- 對 golden `cContactCT.cpp` 全檔搜尋 `"Aa"`：只有 2 處命中（`:617`、`:669`），
  都是**程式註解裡的舉例**（"若 iShuttleMode==0 ... 會一直出現 Aa, Ba Low
  Yield"），指的是「site 代號」的範例，不是這個表格的完整清單。

### 3.3 golden 實際設計：整格只有一個「模式」，列是「site」不是「kind」

`cContactCT.cpp:571-617`（`i,j` 迴圈跑 `TestSocket.iShtRow`/`iShtCol`，即
site 座標），依 `ccMode`/`ccScope`（網頁上兩組 radio：History/Kind/By Head
Yield、Total/Kind(%)）選擇呼叫哪一個 `Get*`：
```cpp
iColA = ArmData[0]->ArmSKET[i][j]->GetTotal();       // 或 GetPassCT / GetPCA /
iColB = ArmData[1]->ArmSKET[i][j]->GetTotal();       // GetBySitePCA / GetByBinLowYieldPassCT /
                                                       // GetByBinLowYieldPassPCA / GetByBinArmYieldPassCT /
                                                       // GetByBinArmYieldPassPCA / GetByBinSiteYieldPassCT /
                                                       // GetByBinSiteYieldPassPCA（共 10 種）
```
也就是說：golden 的「Contact Counter Kinds」面板一次只顯示**一種**統計口徑
（由兩組 radio 選定），對**每個 site**各出一個 Arm1/Arm2 數字，行數 = site
數（16 或更多），不是固定 9 種「kind」同時攤開。這跟網頁目前的形狀（固定
9 列、每列一個代碼、`pct2` 格式暗示永遠顯示百分比）不吻合：
- 若代碼是「site」，golden 一次只顯示一種 metric，不會同時混雜 Total(次數)
  與 PCA(百分比) 兩種單位在同一批 18 個 tag 裡；`pct2` 格式假設每個 tag 都是
  百分比，但 `GetTotal()`/`GetPassCT()` 回傳的是次數（`unsigned int`），套上
  `pct2` 格式會顯示錯誤。
- 若代碼是要固定顯示 10 種模式各一個彙總值（不分 site），golden 沒有現成的
  「跨 site 彙總单一 kind」函式可以直接借——`GetByBinLowYieldPassPCA()` 等都是
  `ArmSKET[i][j]`（單一 site）的方法，彙總需要另外寫迴圈加總，且加總完的
  語意（是否該除以 site 數變成平均百分比）golden 沒有先例可循。

**結論：`contact.*` 目前卡在「這 18 個 tag 到底該代表什麼」還沒有定案，
這是一個先於 liveness 的設計問題**——不管來源翻譯得多完整，沒有人拍板
「18 個位置分別對應哪個 site/哪個 mode」之前，寫任何 getter 都是在編造
語意，不是翻譯忠實度問題（golden 沒有這個形狀），也不是接線問題。

### 3.4 verdict

**0/18 可接，且暫不建議排進下一批**（連候選都算不上，因為候選需要先有一個
「這是什麼」的答案）。V906 端的顯示骨架（`forms/fContactCT.h`
`TfContactCTGrid`/`TfContactCTRadioGroup`）已經翻譯，若日後照 golden 原始
形狀重做這個 web 面板（site 列 + mode 單選，而非固定 9 kind），會比硬湊
現有的 18 個佔位 tag 更省事，但那是 UI 改版範圍，不在本次偵察任務內，
僅記錄於此供決策參考。

---

## 4. `speed.*`（12 個：6 軸 × xy/acc，`pct0` 格式）

### 4.1 綁定側

`web/js/panels/right.js:121-134`（`indexPanel`），`web/js/model/state.js:33-40`：
```js
export const SPEED_ROWS = [
  ["Index Arm","index",true,false], ["Input Arm","input",true,true],
  ["Output Arm","output",true,true], ["Tray Arm","tray",true,false],
  ["Shuttle 1","shuttle1",true,true], ["Shuttle 2","shuttle2",true,true],
];
```
每列產生 `speed.${key}.xy` + `speed.${key}.acc`（`acc` 若該軸無加減速設定則
不產生 tag，但目前 6 軸全部 `hasSpeed=true`，4 軸 `hasAccel=true`）——
6+6=12 個。`tagmap.js:140`：`{ prefix: "speed.", owner: "the Speed form
reached by sbSpeed" }`。

### 4.2 golden 來源：`cSpeed.cpp/h/dfm`——V906 完全沒有翻譯家

`grep -rn "cSpeed\.cpp|fSpeed\.h|TfSpeed"`（20260819，排除 build_*/）在
V906 樹裡只命中 3 處，全部是「不存在」的證據，沒有一處是真正的翻譯：
- `tools/dfm2rc/{layout_out,_b1d_regen/idempotent/layout_out}/cSpeed_layout.gen.cpp`
  ——dfm2rc 工具自動產生的**版面配置**骨架（座標/尺寸），不含任何資料邏輯，
  是表單三件套流程的中間產物，不是翻譯。
- `cprod.cpp:2656-2662`：
  ```cpp
  #if 0 // TODO(GA1-B2): blocked by fSpeed@not declared anywhere in ported tree
        // (golden form, no forms/fSpeed.h port exists)
      fSpeed->ReadFile();
  #endif // TODO(GA1-B2): fSpeed
  ```
  這條 gate 註解本身就是最直接的證據：**沒有 `forms/fSpeed.h`**。

golden `cSpeed.h`（python cp950 解碼，20260819）確認這是一個大型設定對話框
（`TPageControl` 6 個分頁：AllSpeed/InArm/Shuttle/Index/OutArm/TrayArm），每軸
有 `TEdit`+`TUpDown` 對（`edInXYSpd`/`udInXSpd` 等）。`cSpeed.cpp:41-70`
（`FormShow`）：
```cpp
LastFileName=GetLastOpenFN();
ReadFile();
DoIniDataToForm();
```
`cSpeed.cpp:1535-1536`：`WriteIniData(szDir, "Input Arm", "XY Speed",
edInXYSpd->Text);`——確認這組數值是走**獨立的 recipe/setup 檔 INI 讀寫**
（`szDir`/`GetLastOpenFN()`，配方專屬檔案），跟 `LastSet`/`Gerneral.ini`
完全是不同的持久化管道，且依賴 `cAuthority.h`/`RPDefault.h`（Recipe
Default）/`TrayStepMotor.h`/`uCleaning.h`/`uYieldMonitoring.h` 一整串
未在 `cSpeed.cpp` 本身確認是否已翻譯的周邊模組。

### 4.3 verdict

**0/12 可接，且是五個 family 裡缺口最深的一個**——不是某個呼叫點被 gate，
是整個表單（含它自己的 INI 讀寫層與周邊相依模組）連 0% 都還沒開始。這不是
接線波次的範圍，需要排一個完整的「cSpeed 表單翻譯」波次，比照
`forms/fBinSel.h`/`forms/fShowBinSelect.h` 這種規模（cBinSel.cpp 6,652 行）
先評估依賴樹再排期。

---

## 5. `bin.*`（6 個：`bin.auto1/2/3`、`bin.fix1/2/3`，`binmap` 格式）

### 5.1 綁定側

`web/js/panels/right.js:104-117`（`binSelectPanel`，預設分頁「Bin Display
Status」），`web/js/ui/widgets.js:189-194`（`binRow`）：
```js
export function binRow(label, tag) {
  return h("div.binrow", null, h("div.binrow__k", null, label),
    h("div.binrow__v", { "data-tag": tag, "data-bind": "html", "data-fmt": "binmap" }));
}
```
`web/js/ui/bind.js:64-65`：`binmap` 格式把字串裡的「非句點」片段包 `<b>`——
明確暗示每個 tag 的值是一串「數字/句點」文字（例如 `"01 04 05"`），不是單一
數字。這個線索非常關鍵，指向下面找到的 golden 函式。

### 5.2 golden 來源鏈（完整三段，逐段查證）

**第一段（顯示層，已翻譯且 ACTIVE）**：`cShowBinSelect.cpp:388-460`
（`TfShowBinSelect::ShowBinSel()`，golden :388-756）：
```cpp
for(int i=0; i<iTestBinCount; i++) {
    Data=Prod.iT6CatData[i];
    if(Data<0) continue;
    for(int j=0; j<eTrayCount; j++) {
        if(grpBinDisp[j]->Visible==false) continue;
        if(j==Data) {
            ...
            S[j]+=AnsiString(i)+" ";   // S[eAuto1]/S[eAuto2].../S[eFix3] 累積 bin 編號字串
        }
    }
}
```
這正是 `binmap` 格式（一串以空白分隔的 bin 編號）的來源：`S[j]` 之後被寫進
`MyBinSel[j]->Caption`（`forms/fShowBinSelect.h:533`：
`TfShowBinSelectLabel *MyBinSel[e3TrayCount];`）。

`forms/fShowBinSelect.h:74-76`（FW-SBWB2/FW-SBWC，**20260819，也就是今天**）
明確記載 `ShowBinSel` 是 `golden :388-756  ACTIVE, 3 GATEs`，且 `:405-407`
記錄 `FormShow` 會呼叫 `ShowBinSel()`。這是五個 family 裡**唯一一個顯示層
翻譯完成度達到「ACTIVE」的**（其餘四個要嘛整個表單沒翻，要嘛卡在 `#if 0`）。

**第二段（資料來源，同樣已宣告，尚待確認是否被填值）**：`S[j]` 的索引來自
`Prod.iT6CatData[i]`（golden `cprod.h:511`，"Auto1 = 0" 0-based），
`cSocket.cpp:995` 等多處已在讀它，V906 樹裡宣告存在且被廣泛引用（16 個檔案，
`grep -c` 20260819）。

**第三段（寫入端，本次找到的關鍵斷點）**：`cinitial.cpp:16983-16994`
```cpp
//  No caller yet (SetTechDataToProd, golden :11636, is not in this slice and is
//  itself gated at :7040 of this file as N3-G9) -- lands reachable but uncalled.
void SetTechDataToProd_Yield()
{
    ...
    for(int i=0; i<iTestBinCount; i++)
        Prod.iT6CatData[i] =iTo6PosUnload[BinSelect[iTestRunMode].iCatDataT3Pos[i]]-1;   //Auto1 = 0
        ...
}
```
**這行「in-tree」註解是本檔作者自己寫的，明講這個函式目前沒有任何呼叫者**
——`Prod.iT6CatData[]` 因此永遠停在零初始化的預設值，`ShowBinSel()` 的
`Data=Prod.iT6CatData[i]` 迴圈永遠讀到同一個預設索引（很可能是 0，也就是全部
誤歸到 `eAuto1`，其餘 5 個輸出永遠是空字串），而不是真正對應到目前配方的
bin 分類設定。

### 5.3 額外發現：即使資料源解決了，`ShowBinSel()` 的自動刷新路徑也大多被 gate

全樹搜尋 `ShowBinSel()`/`->FormShow(`（20260819）的呼叫點：
- `csystem.cpp:10446-10452`——在 `MainProc` 可達的狀態機裡，但套在
  `if(bPurgeOutAllDevice)`（一個很少見的「清空所有裝置」旗標）內，一般
  `--pump` 跑法幾乎不會經過。
- `Command.cpp:8599`（`TfMain` 的某個方法，需要具體觸發條件，本次未追）、
  `Command.cpp:12277-12281`（`BinPosChange`，golden :11544）：
  ```cpp
  // GATE (kept): fShowBinSelect->ShowBinSel() -- Wave B queue item
  // (forms/fShowBinSelect.h:62), still untranslated.
  #if 0
      fShowBinSelect->ShowBinSel();
  #endif
  ```
  **這條 gate 註解也已過期**——寫的時候 `ShowBinSel` 確實「still
  untranslated」，但 FW-SBWB2/FW-SBWC 已經在同一天稍早把它翻完並標記
  ACTIVE。這是本次找到的第四處過期 gate 註解（連同前面 cat.* 段落的三處，
  合計四處，全部指向同一個模式：翻譯波次往前推進了，但引用它的舊 gate
  沒有跟著重新評估）。
- `auto9045.cpp:501/581` 呼叫的是 `W5FA_FShowBinSelect.ShowBinSel()`——
  變數名帶 `W5FA_` 前綴，是一個 TU-local 的 shim 物件（同名不同型別，
  和 `fCounterClear` 在 csystem.cpp 的 `#define` 手法是同一類「先頂著」
  的替身），**不是**真正的全域 `fShowBinSelect`，呼叫它不會影響
  `MyBinSel[]->Caption`。

`fShowBinSelect` 本身已在建構時 unconditional homecome（`cShowBinSelect.cpp:135`：
`TfShowBinSelect *fShowBinSelect = new TfShowBinSelect();`，`forms/fShowBinSelect.h:200`
註明帶 SIOF guard），建構子只呼叫 `ShowInitialString()`（`:1634-1700`，呼叫
`ShowCategoryBin()`，屬於「Category Info」分頁，跟 `bin.*` 要的「Bin Display
Status」分頁是不同的兩個函式，不要混用）——**不會**在建構時順帶跑
`ShowBinSel()`。

### 5.4 verdict

**0/6 可接，但是五個 family 裡缺口最淺的一個**：顯示邏輯已經翻完且標記
ACTIVE，只差兩件事都是翻譯決策（非本次偵察範圍可自行拍板）——
(1) 給 `SetTechDataToProd_Yield()`/`SetTechDataToProd()` 补一個呼叫點
（golden 自己的呼叫鏈也被 `N3-G9` 這個獨立的 gate 卡住，需要先看那個 gate
放不放得開），(2) 決定 web tag 的 getter 要不要在 publish tick 主動呼叫
`fShowBinSelect->ShowBinSel()`（因為它在正常 pump 路徑下不會自動刷新），
或者直接在 `WebBridgeTags.cpp` 裡重算等價的 `S[j]` 字串（繞開表單，比照
`sort.total` 繞開 `iTo3Unload[]` 地雷的既有先例）。**建議的 liveness key**：
以 `Prod.iT6CatData[]` 是否已被寫入為準（例如新增一個「非全預設值」的 blob
校驗，仿照 `LastSetLoaded()`），而非籠統套用 `lastS`/`cust`。

---

## 6. 下一批建議（本次不建議排入 130 個裡的任何一個，理由如下）

嚴格套用 crosswalk 既有判準（「golden 全域已在 V906 翻譯樹裡宣告 + 有實際
賦值程式碼路徑」），這五個 family **沒有一個現在就能直接排進下一批接線候選**：

| family | 為什麼現在不能排 | 誰該先動 |
|---|---|---|
| `arm.*`     | 來源計算 100% 死於未翻譯 ATC 介面表單 | 排一個「ATC Interface 表單翻譯」波次，且要先拍板 16 碼命名對應表 |
| `cat.*`     | 唯一自動填值路徑 `#if 0`；索引維度是否對得上 site.* 未驗證 | 先驗證 `[2][4][8]` 索引語意，再決定是走 `ArmData[]` 直讀還是解 `UpdataCount` 的 gate |
| `contact.*` | 18 個 tag 名稱本身不對應任何 golden 結構 | 需要使用者/UI 設計決定這個面板到底要顯示「site」還是「mode」，屬於 UI 改版範圍 |
| `speed.*`   | 整個表單 0% 翻譯 | 排一個「cSpeed 表單翻譯」波次（規模比照 fBinSel/fShowBinSelect） |
| `bin.*`     | 缺一個呼叫點（翻譯行為決策）+ 需要決定 getter 要不要主動觸發 `ShowBinSel()` | 排一個小波次專門處理 `SetTechDataToProd_Yield` 的呼叫點決策（含它自己被 `N3-G9` 卡住這件事） |

如果一定要選一個**最快能看到真實資料**的方向，順序建議是
`bin.* > cat.* > arm.* > speed.* ≈ contact.*`（`speed.*` 是純翻譯量問題，
`contact.*` 是純設計未定案問題，兩者性質不同、都不適合塞進「接線」波次）。

---

## 7. 未做 / 風險

- **本次是純靜態程式碼追蹤 + 一次 `nm` 驗證，沒有起 `wb_serve`/`snap_dump.py`
  做動態確認**——因為五個 family 全部 0 published，動態探針不會比靜態證據
  提供更多資訊，故省略（任務書允許但不強制）。
- **四處過期 gate/警告註解**（`cSocket.h:40-61` 的 ODR 警告、
  `atester_ProcessCount.cpp:642-649`、`Command.cpp:12277-12281`，以及間接
  波及 `cSocket.h` 本身）**本次只記錄，未修改**——不在本檔的寫入邊界內
  （這些檔案都在 V906 樹但不是本次任務指定可寫的唯一檔案），留給處理
  `cat.*`/`bin.*` 的下一波一併清理，並建議清理時比照本次做法附 `nm`/
  git log 證據，不要只憑註解文字判斷現況。
- **`cat.*` 的索引維度疑點**（`TastCategory.iCountHeadTotal[2][4][8]` vs
  網頁預期每臂 16 site）沒有追到底——需要讀 `ArmSKET[][]`/`TestSocket.iShtRow`
  /`iShtCol` 的實際初始化值，或找一個已知機型組態反推，本次時間有限未做。
- **`ArmData[]->ArmSKET[i][j]` 的底層計數器（`SetPassCT`/`SetBinCT`）是否
  在測試流程中被即時呼叫**（本次列為 `cat.*` 的潛在替代路徑）——只確認了
  方法存在且被 `cContactCT.cpp` 讀取，沒有追誰在測試完成時呼叫 `Set*`，
  如實列為未查證，不可視為「已確認活」。
- **`contact.*` 的裁決建議**（改走 site 列 + mode 單選）是本次觀察到的
  golden 真實形狀，但要不要照這個方向改網頁面板是 UI 設計決定，超出本次
  偵察任務範圍，只記錄不建議。
- **`speed.*`/`arm.*` 的翻譯規模皆未估算行數或依賴樹深度**——只確認了
  「0% 翻譯」這個事實，沒有進一步評估工作量，需要另外的 recon 或直接排波次
  邊做邊量。
