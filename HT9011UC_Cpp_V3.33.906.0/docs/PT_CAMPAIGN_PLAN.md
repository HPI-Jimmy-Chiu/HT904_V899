# 純翻譯完成戰役計畫（PT CAMPAIGN）— 「翻完（不含 .dfm）且編得起來」

> AI(W906-PT-plan) 20260807: 本檔把「完成翻譯」從一句口號變成**可量測的剩餘量**與
> 波次清單。所有數字都是量出來的，量法附在下面，任何人都能重跑。

---

## §1 目標定義（與使用者 20260807 確認）

1. **把 golden 的 C/C++ 全部翻完**——`.dfm` 不手翻，走 `tools/dfm2rc` 產生器（閘 G1–G7）。
   **注意界線**：「不含 dfm」指的是**資源檔本身**，**不包含表單的 `.cpp` 實作**。
   110 個 VCL 表單單元的 `.cpp`（221,682 行）仍然要翻，那是剩餘工作的**大宗**。
2. **編譯要成功**。目前未最佳化 build 已綠；驗收要**連 Release（-O3）一起綠**，因為
   出貨 build 是 Release，而 20260805 已量到兩個只在 -O3 現形的真缺陷（見 §5）。

---

## §2 量法（可重跑）

範圍 = `HT9045.bpr` 的 `FILELIST`（權威建置集合；MR/ 那種死目錄不會灌水）。
單位 = **code line**（去空行、去註解的實體行）。

- 檔案層：golden `.cpp` 是否有同相對路徑的 port 鏡射檔。
- 函式層：golden 頂層函式定義名 vs port 同名定義；缺的函式**按 golden span 加總行數**，
  所以「port 檔存在但只翻了 10%」不會被算成翻完。

腳本：`census.py` / `classify.py` / `remaining.py`（本次留在 session scratchpad；
數字如下，重跑請照本節重建，勿信任何未附量法的百分比）。

**已知失真**：函式名比對是 regex，對 `extract-calc-core` 這種「改名抽核心」的檔會
**高估缺口**。已人工抽驗兩個最大的：`cContact.cpp`（port 544 行僅 calc core，golden
20,819 行）與 `csystem.cpp`（port 4,849 行僅 spine，其餘 `#if 0 // TODO(W7)`）——
兩個缺口都是**真的**，不是 regex 假象。

---

## §3 現況（20260807 量測）

```
golden .cpp 全部            339 檔   643,130 行
  在 HT9045.bpr 範圍內      289 檔   598,367 行   ← 分母
  範圍外                     50 檔    44,763 行   （不列入）

已翻                                 233,586 行   = 39.0%
剩餘                                 364,781 行   = 61.0%
  ├ 完全沒開始   163 檔            288,917 行
  │    ├ VCL 表單單元 110 檔       221,682 行  ← 大宗
  │    └ 非表單單元    53 檔        67,235 行
  └ 翻一半的      26 檔             75,864 行（這 26 檔共 110,459 行）
```

**同一份普查，切成表單／非表單（這才是能跟舊數字對照的形狀）**：

| 分群 | 檔數 | 總行 | 已翻 | 完成度 | 剩餘 |
|---|---:|---:|---:|---:|---:|
| 非表單（無同名 `.dfm`） | 171 | 336,505 | 223,220 | **66.3%** | 113,285 |
| 表單（有同名 `.dfm`） | 118 | 261,862 | 10,366 | **4.0%** | 251,496 |
| 合計 | 289 | 598,367 | 233,586 | **39.0%** | 364,781 |

> **與先前「54.8%」的關係——舊說法沒有被推翻，是引用時少寫了限定詞。**
> 20260807 早先記錄的 54.8% 是**只算非表單單元**、且以「golden function body 行」為單位
> （186,906 / 341,077）；同一次記錄裡的「全專案含表單 33.6%」才是能跟本次 39.0% 對照的那個數。
> 本次非表單 66.3% 與它同向：單位換成「整檔 code line」，再加上 PT-W1 剛落地的 ~18.5k 行。
> **本次真正修掉的是「只看檔案存不存在」的普查方式**——26 個「port 檔存在但只有骨架」的檔
> 必須按缺席函式的 golden span 扣行，`cContact.cpp` 一個檔就差 20,713 行。
> 引用任何百分比時，**分母（表單／非表單／全部）與單位（body 行／code 行）要一起講**，
> 否則下一個人會像這次一樣把兩個不同分母的數字當成互相矛盾。

### 翻一半的 26 檔（缺口由大到小）

| 缺口行數 | 缺函式 | 檔案 | 備註 |
|---:|---:|---|---|
| 20,713 | 138 | `cContact.cpp` | port 只有 extract-calc-core |
| 8,169 | 142 | `csystem.cpp` | port 只有 MainProc/DoAllProcess spine，其餘 `#if 0 TODO(W7)` |
| 7,141 | 17 | `aTester_Rear.cpp` | |
| 5,984 | 16 | `aTester_Front.cpp` | |
| 5,315 | 84 | `ainarm9045.cpp` | |
| 5,018 | 20 | `SECSGEM/uHGemHT9045.cpp` | |
| 3,876 | 54 | `Automation/SCK_ART.cpp` | |
| 3,445 | 82 | `ainarm2.cpp` | |
| 2,847 | 50 | `Motor/mymotor.cpp` | 安全關鍵 |
| 2,796 | 76 | `SECSGEM/uHGemEquipment.cpp` | |
| 2,699 | 40 | `aoutarm9045.cpp` | |
| 1,749 | 10 | `cinitial.cpp` | |
| 1,352 | 29 | `ContactForce.cpp` | |
| 947 | 23 | `Interface/TesterTCP.cpp` | |
| 943 | 25 | `cMyDB.cpp` | |
| 899 | 20 | `SECSGEM/uHGemClass.cpp` | |
| 777 | 7 | `atester_ProcessCount.cpp` | |
| 557 | 12 | `Public/MyProductionRecord.cpp` | |
| 208 | 7 | `cUnitConvert.cpp` | |
| 115 | 4 | `Public/ExternFunction.cpp` | |
| 106 | 11 | `mytray.cpp` | |
| 102 | 7 | `Automation/automation.cpp` | |
| 63 | 5 | `common.cpp` | |
| 28 | 2 | `database.cpp` | |
| 11 | 1 | `Public/WinSocketErrorCode.cpp` | |
| 4 | 1 | `Public/HTMD5.cpp` | |

---

## §4 波次計畫

排序原則：**先清掉相依最少的層**，讓後面的波不必再開 gate。
「deps」＝這個 golden 單元 `#include` 到、但**還沒翻**的其他 golden 單元數。

| 波次 | 內容 | 檔數 | 行數 | 狀態 |
|---|---|---:|---:|---|
| **PT-W1** | RotateKit×3、TriTemp、bthermo、AutoRetest、OCRInsp、SortingBinTray、uHeaterThread、MyStringList、EJ1N/TextProcess | 11 | ~18.5k | ✅ 完成（本檔同批 commit） |
| **PT-W2** | 非表單 deps≤1 整層一次清光 | 27 | ~11.4k | 🔄 進行中 |
| **PT-W3** | 非表單 deps 2–6（CosFunction、mykitsuck、ATCSystem、myEthercatmotor、myMN200motor、uRENESAS_Server…） | ~14 | ~15k | 待排 |
| **PT-W4** | 非表單 deps≥8（BarCode_Sh1/Sh2、myGALILmotor、asortarm、aoutarm、Command.cpp、uHGemHT9045_SV/EC） | ~12 | ~40k | 待排 |
| **PT-W5..** | 26 個翻一半的補完（`cContact` / `csystem` / `aTester_*` 為最大三塊） | 26 | ~75.9k | 待排 |
| **PT-F1..** | 110 個 VCL 表單單元，經 `forms/` facade | 110 | ~221.7k | 待排；需先定 facade 覆蓋策略 |

**表單波的前置決策（尚未做）**：`forms/` 目前是**手寫 facade**（fMain/fLotInfo/fAOI…），
而 golden 表單單元有 110 個。要嘛把 facade 補到 110 個（工作量大但形狀已知），
要嘛讓 `dfm2rc` 的 `emit_uimap` 產物承接 widget 綁定。**這是整個戰役最大的單一未決項**，
在 PT-F1 開波前必須先定案，否則 110 個檔會各自發明自己的 widget 存取方式。

---

## §5 「編譯成功」的驗收條件

1. 未最佳化 build：`cmake --build` exit 0（**目前已達成**）。
2. ctest 失敗集合不擴大（目前基準見 DEVLOG 各波記錄）。
3. **Release（-O3 + NDEBUG）build 綠**——**20260807 達成**。全新 `build_0807_rel`：
   build exit 0、0 compile error、ctest 128/134，**失敗集合與未最佳化 build 逐位元相同**。
   先前兩個缺陷都已修，且兩個都證實是 **golden 自己的潛伏缺陷**、不是移植產生的：
   - `BarCodeBottom2DID`：golden `BarCode.cpp:7150` 宣告 `sSimuCode[4]`，golden
     `:7203-7217` 的迴圈卻寫到 `BAR_CODE_COUNT`(=8)。golden `:9954` 的姊妹拷貝是 `[8]`
     ——`4-->8` 改動漏掉這一份。真機上每次掃碼都在越界寫。
   - `MyPLCModbus`：golden `MyPLC/ModbusTCPClient.cpp:10-20` 的 ctor 沒初始化
     `bConnected`／`iIP`／`iPort`。
   **往後每波交付要同時量 unoptimised 與 Release 兩組數字**——這兩個缺陷是靠
   「同一份 source、兩種建法對照」才現形的，單一建法的綠燈涵蓋不到這一類。
4. MSVC 第二 oracle（`build.bat msvc`）不退步。

---

## §6 波次作業規則（沿用，勿再重新發明）

- 翻譯 agent **只准新增自己那幾個鏡射檔**；`CMakeLists.txt` 與既有檔的整併**一律主迴圈序列做**。
- 每個新檔要有 aRotateKIT.cpp 式的 banner：ROLE / WAVE SCOPE（ACTIVE vs
  SATISFIED-BY-SUBSTRATE，附 golden 行號）/ **GATE REGISTER**（每個 gate 要寫
  為什麼那個 default 是忠實的、以及真機上的行為差異）。
- 落地前每檔各自 `g++ -std=c++17 -fsyntax-only` 要乾淨。
- 交付數字**只在全新 build dir 量**；併發 build 會弄出假的 undefined reference。
- 稽核 agent **唯讀**。它們最常見的錯不是漏看程式碼，是**引用造假**——每條 finding
  都要有真的 golden 行號與 port 行號，主迴圈要抽驗。
- **absence-claim 會在同一波內過期（PT-W2 一次中三個，務必當常態防範）**。
  翻譯 agent 開工時 grep 全樹得到「X 不存在」，據此開 gate 並寫進 GATE REGISTER；
  但兄弟 group 會在那之後才把 X 落地，於是 gate 帶著假前提上樹。
  - `Monitor/MonitorTCPIP.cpp` 說 `class TCPIP` 全樹沒有 → 同一波早 11 分鐘落地的
    `ATC/TCPData.h:74` 就是。後果：9 個 call site 被寫死成 `IsConnect()=false`／
    `SendCommand()=-1`／`AdapterData()=0`——真機在跑，畫面說沒連上。
  - `EJ1N/MyOmronPanel.h` 說 golden 全樹零消費者 → 實際 7 個檔，`fDTME08.cpp:112`
    與 `OmronEJ1N.cpp:97` 都真的 `new TMyOmronPanel`。這個是 grep 自己漏。
  - `Public/HTEditList.cpp` 說 `class FileInfo` 不存在 → 同一波的
    `ProductionInfo/FileInfo.h:129` 就是。
  **規則**：(1) GATE REGISTER 的 absence-claim 要附**量測指令與時間**；
  (2) 單元**收工時重跑一次** grep，不是開工跑一次就算；
  (3) 主迴圈整併時把每個新 gate 的 absence-claim 當預設可疑、逐條複驗；
  (4) 「降級值剛好等價」不算沒事——缺陷是假前提本身。
- **`-fsyntax-only` 抓不到連結錯誤**。PT-W2 每個檔都 syntax-clean，整併後仍冒出
  `RecordChangeLogProcess` 未定義——真因是 `cMyDB.cpp` 早就翻好卻**從來沒被註冊進任何
  archive**。波次自檢要加一句：新單元呼叫的每個外部符號，確認它的 body 所在的 .cpp
  **有在 CMakeLists 裡**，不是只存在於樹上。
- 檔案編碼：golden cp950 → port UTF-8 + LF，U+FFFD 出現即為缺陷。

---

## §7 目前那 6 個常駐 ctest 失敗，各自到底是什麼（20260807 逐條查證）

「失敗集合不變」被當成健康指標很多波了，但沒人逐條查過它們是什麼。查完的結論：
**這 6 個沒有一個是 port 的程式缺陷。** 5 個是測試把某一台機器的設定值寫死當斷言，
1 個是 GA-4 未完成的工作污染了 canonical 輸出樹。

### （A）5 個「設定快照不符」— `config_db`／`IniFiles`／`ini_helpers`／`config_loaders`／`GA1_ReadGeneralIni`

這些測試直接讀**活的** `D:/HT9045/system` 與 `D:/HT9045/config`，並斷言某一台機器的值。
磁碟上現在放的是**另一台機器的快照**，所以它們紅：

| 測試斷言 | 磁碟實際值 |
|---|---|
| `IO_CARD_TYPE == 2` | `1` |
| `TTL_CARD_TYPE == 2` | `0` |
| `HEATER_CTRL_TYPE == 4` | `1` |
| `CUSTOMER_CODE == 970` | `957` |
| IO_Table 資料列 `== 668` | `IO_Table.csv` 共 772 行 |
| Mot_Table 資料列 `== 45` | `Mot_Table.csv` 共 45 行（含標頭） |

用改動前的 binary 跑會得到相同失敗，所以不是任何一次修改造成的。

**這是設計缺陷，不只是環境問題**：本 repo 的日常作業就包含「把客戶快照倒進 `system/`
來查異常」（而且 `system/` 沒進 git），所以這些測試會隨著有沒有人在查客戶案子而
紅綠跳動。**修法**：改成讀**與測試一起版控的 fixture 設定**，而不是活的 `system/`；
真的要驗活樹的測試應該獨立成一組、且預設 skip。在那之前，「失敗集合不變」這個指標
只能證明「沒有人動壞東西」，不能證明資料層是對的。
（附帶一提：磁碟這份 `IO_CARD_TYPE=1`，依 `cinitial.cpp` 的分派根本不會去讀
`IO_Table.csv`——卡型 1 走 `cylinder_AutoRetest.DB` + `Sensor_<機型>.DB`。
也就是說有兩個斷言連讀的來源都跟真實流程不一致。）

### （B）1 個工作樹污染 — `dfm2rc_idempotent`

`G7_MISSING_IN_REGEN: rc_out/main_uimap.gen.h`。成因：GA-4 的 `tools/dfm2rc/emit_uimap.py`
**還沒接進 `run_b1d.py` 的 `regenerate_all()`**，但有人手動跑過它、把
`rc_out/main_uimap.gen.{h,cpp}` 留在 canonical 輸出樹裡（兩個檔目前都**未 track**，
`ui/forms/FMainFirstLightDlg.cpp` 也未 track 且 include 它）。於是 regen 產不出這個檔、
canonical 樹卻有 → G7 判缺件。**乾淨的 repo 不會有這個失敗。**

**不要用 `_STATIC_NONGENERATED_FILES` 把它排除掉**——那個名單是給「手寫的、本來就不該被
regen 的」檔用的，`*_uimap.gen.*` 是**產生出來的**，排除等於默默解除對一個產物的覆蓋。
正解是 GA-4 決定 emit_uimap 要對 133 個表單全跑還是只跑 main，然後接進 `regenerate_all()`
並把產物一起 commit。**這是 GA-4 的未結項，不在 PT 戰役範圍內。**

---

## §8 `main.cpp` 建構缺口 —— 18 個全域指標目前是 NULL（20260807 普查）

**怎麼發現的**：PT-W2 整併時退役了 `aHotPlateSubstrate.cpp` 裡 `uPlateInfo` 的 stub
（真本體隨 `Public/HTEditList.cpp` 落地了），結果三個原本綠的測試變成 **SEGFAULT**
（AutoClean／W6_2_InArmCanary／W6_2_InArmSearch）。追下去不是退役做錯，是**stub 一直在
遮一個真的洞**：

- golden `Public/HTEditList.cpp:56/:59` 只宣告裸指標 `uPlateInfo *PickFromHPList;`，
- 真正 `new` 它的是 **`main.cpp:2149-2150`**，
- 而 `main.cpp` 是 163 個完全沒開始的單元之一。

port 忠實翻了 `:56/:59`，所以兩個指標是 NULL；ported 的 `ainarm9045_*` 家族每一句
`PickFromHPList->...` 都是等著發生的 NULL 解參考，只是被 stub 指向自己的 file-static
物件掩蓋住。

**這不是單一個案**。掃 golden `main.cpp` 全部 67 個 `X = new T;` 站點、比對 port 中
以裸指標定義同名全域的檔案，得到 24 個，其中 **18 個在 port 裡從來沒有人 new 過**：

| 全域 | 型別 | golden main.cpp | port 定義處 |
|---|---|---:|---|
| `HeaterThread` | `THeaterThread` | 22481 | `uHeaterThread.cpp` |
| `MyThread` | `TRunControl` | 22475 | `uruncontrol.cpp` |
| `elConfig` | `HTEditList` | 1484 | `Public/HTEditList.cpp` |
| `elConfig_byRecipe` | `HTEditList` | 1489 | 同上 |
| `elContact` | `HTEditList` | 1485 | 同上 |
| `elLaser` | `HTEditList` | 1483 | 同上 |
| `elTeach` | `HTEditList` | 1488 | 同上 |
| `elTrayForm` | `HTEditList` | 1487 | 同上 |
| `elUdUld` | `HTEditList` | 1486 | 同上 |
| `elVacuumUnit` | `HTEditList` | 1493 | 同上 |
| `cbLastSet` | `HTEditList` | 1492 | 同上 |
| `slEventLog` | `TMyStringList` | 1503/1509 | `cmydef.cpp` |
| `sl2DMappingLog` | `TMyStringList` | 1513 | `cmydef.cpp` |
| `slGroundManLog` | `TMyStringList` | 1671 | `cmydef.cpp` |
| `slBundlID` | `TStringList` | 1562 | `cmydef.cpp` |
| `slDupBundlID` | `TStringList` | 1564 | `cmydef.cpp` |
| `slDupUnloadBundlID` | `TStringList` | 1568 | `cmydef.cpp` |

（腳本 `nullsweep.py`，可重跑。`PickFromHPList`／`PlaceToCleanList` 已於本波補上，
`sList` 是函式內區域變數、誤入清單，不算。）

**本波只補了擋路的那兩個**，做法是 `aHotPlateSubstrate.cpp` 一個 static-init 物件，
照抄 golden `main.cpp:2149-2150`，並在原地寫明它是 unported 單元的 stand-in、
以及為什麼 static-init 在這兩個身上是安全的（只被 runtime 函式讀，不被別的 TU 的
static initialiser 讀——**查過，不是假設**）。

**其餘 16 個刻意不一起補**，理由是這不是機械填空：
- `HeaterThread`／`MyThread` 是**執行緒**，在 static-init 期建構等於在 `main()` 之前
  起執行緒，絕不可盲目照抄。
- 那批 `HTEditList` 實例（`elConfig`/`elTeach`/…）在 golden 裡 `new` 完緊接著就
  `LoadFile()` 讀設定檔，順序和路徑都有意義。
- 跨 TU static-init 順序標準零保證（`5bad9aa` 的 `LoadMachineConfig()` 已經為同一個
  理由**刻意不重現** golden 的 static-init 形狀）。

**歸屬**：這整塊是 **GA-3（main.cpp 啟動鏈垂直切片）** 的工作，不是 PT 戰役的。
在 GA-3 落地之前，**任何一波只要讓上表其中一個全域第一次有呼叫者，就會 segfault**——
這不是回歸，是本來就在那裡。波次驗證看到新的 SEGFAULT 時，第一個要查的就是這張表。
