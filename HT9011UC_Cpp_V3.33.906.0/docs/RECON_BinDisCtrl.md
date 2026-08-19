# RECON: BinDisCtrl opaque 元件查證

唯讀 recon。範圍：cShowBinSelect Wave D（`ChangeBinDispStatus`/`DoShowBinDigital`）與
BinDisplay 族翻譯卡住的「opaque BinDisCtrl」。

所有指令於 `D:\HT9045` 下執行，時間戳 2026-08-20 03:50–04:10（本檔案寫入前）。

---

## 1. 身分與住址（結論先講）

**`BinDisCtrl` 不是 halarm 那種「樹外姊妹元件套件」個案。** 它的宣告與實作**整份都在
golden 樹內** (`HT9011UC_Code_V3.33.906.0_20260618\BinDisplay\`)，不在
`D:\HT9045\elec\Component\`。V906 卡住的原因是**翻譯排序策略**：V906 在
`database.h` 把它 forward-declare 成 opaque 指標、刻意延後到「UI wave」，不是缺料。

| 項目 | 值 |
|---|---|
| 型別 | `TMyBinDispCtrl`（抽象基底，純虛擬 `WriteBin/WriteColor/WriteBin2/WriteColor2/ReadVersion/ReadVersion2/InitialTask`） + `TMyBinDispHT9046`（唯一具體子類） |
| 宣告檔 | `HT9011UC_Code_V3.33.906.0_20260618\BinDisplay\MyBinDisp.h`（196 行；`TMyBinDispCtrl` 34-159、`TMyBinDispHT9046` 161-194） |
| 實作檔 | `HT9011UC_Code_V3.33.906.0_20260618\BinDisplay\MyBinDisp.cpp`（3169 行，cp950 解碼 0 個 U+FFFD，`python3 -c "open('MyBinDisp.cpp','rb').read().decode('cp950')"` 驗證於本次 recon） |
| 是否 design-time VCL 元件 | **不是**。`TMyBinDispCtrl`/`TMyBinDispHT9046` 是純 C++ class，無 `__published`、無自己的 `.dfm`。同檔案內另有 `TDataModule3`（`TComm *BinDisp, *BinDisp2` 兩個 datamodule 成員）**是** design-time，但生產路徑不經過它——`TMyBinDispHT9046` 自己在 ctor/內部持有 `CommBin`/`CommBin2`（`TComm*` 型別，屬性宣告在 `MyBinDisp.h:123-124`）。`TDataModule3` 疑似只服務同目錄下的獨立測試專案。 |
| 持有者 | `SYSTEM_MODULAR::BinDisCtrl`（golden `database.h:204`），全域單例 `extern SYSTEM_MODULAR HSys`，consumer 一律走 `HSys.BinDisCtrl->...` |
| 建構 | `database.cpp:1686 BinDisCtrl=new TMyBinDispHT9046;`（唯一具體子類，唯一 `new` 站點） |
| 是否納入主專案 build | **是**。`HT9045.bpr` 的 `PATHCPP`/`INCLUDEPATH`/`LIBPATH` 都含 `BinDisplay`，且 `<FILE FILENAME="BinDisplay\MyBinDisp.cpp" FORMNAME="DataModule3" ... DESIGNCLASS="TDataModule">` 是專案清單的一員（`HT9045.bpr:407`）。**不是**另一棵獨立小工具。 |
| 獨立測試專案（旁支，非生產路徑） | 同目錄 `BinDisp.h/.cpp/.dfm` + `BinDispTester.bpr`：一個 `TForm1`（12 checkbox + 2 combobox + button）硬體測試台，持有自己的 `TMyBinDispCtrl *BinDisCtrl` 成員（`BinDisp.h:42`），**與生產用的 `HSys.BinDisCtrl` 是不同物件**，只是同名。`grep -rl BinDisp HT9011UC_Code_V3.33.906.0_20260618/**/*.bpr` 只列出這兩個 .bpr，`BinDispTester.bpr` 未被 `HT9045.bpr` 引用。 |

**唯一真正的樹外元件套件依賴**：`TComm`/`CommBin`/`CommBin2` 這個型別本身，來自
`D:\HT9045\elec\Component\Spcomm.pas`（1944 行 Delphi Pascal，`SPCOMM.DCR` 是它的
design-time 圖示）。

**更正（2026-08-20 04:11，背景 `find` 任務跑完後才拿到完整結果）**：本檔案稍早版本曾誤述
「`SPComm.hpp` 不存在於任何原始碼樹」——這是背景指令逾時、只憑部分輸出下的錯誤 absence
宣稱。事後補驗證：`elec\Component\spcomm.hpp`（連同 `aled.hpp`、`gwiopm.hpp`）**確實存在**，
是 BCB6 IDE 為套件裡每個 `.pas` 單元機生並簽入原始碼樹的標準 C++ 橋接頭（檔頭原文
`// (DO NOT EDIT: machine generated header) 'SPComm.pas' rev: 6.00`），已用 Read 逐行讀完
（301 行）。

**這個更正不影響 §3 的翻譯路徑建議**，原因：`spcomm.hpp` 裡的 `TComm` 宣告全部是 BCB6
專屬語法（`__property`/`__fastcall`/`__closure`/`DELPHICLASS`/`PASCALIMPLEMENTATION`/
`namespace Spcomm`），MinGW g++ 與 MSVC 都不認得這些擴充字（除非透過 V906 自己的
`vclcompat` 巨集/型別重新定義），所以**即使頭檔案存在，也不能直接 `#include`**——真的要
把 `TMyBinDispHT9046` 的硬體協定本體搬進 V906，仍然要把 `TComm` 的行為（`StartComm/
StopComm/WriteCommData/GetModemState` + 屬性 + `OnReceiveData` 等事件）重新用標準 C++
表達，工作量估計不變。`spcomm.hpp` 存在的實際好處是：**確認介面簽章不必再靠 `.pas` 反推**
（`spcomm.hpp:236-276` 與 `Spcomm.pas:149-281` 解碼結果逐項核對一致），未來真的要做
「UI/hardware wave」時可以直接照這份頭檔案的簽章翻，不用再解 cp950。

`TComm` 公開介面（`spcomm.hpp:236-276` 與 `Spcomm.pas:149-281` 交叉核對）：
`StartComm/StopComm/WriteCommData/GetModemState` + `CommName/BaudRate/Parity/
ByteSize/StopBits/...` property + 事件 `OnReceiveData/OnReceiveError/
OnModemStateChange/OnRequestHangup/OnSendDataEmpty`。
本樹目前完全沒有動過 `Spcomm.pas`/`spcomm.hpp`（`grep -rn "TComm\b" HT9011UC_Cpp_V3.33.906.0`
排除 build 目錄，只命中 forward-decl 相關的 database.h 系列，無實作，2026-08-20 04:05 驗證）。

---

## 2. cShowBinSelect Wave D 與 BinDisplay 族各需要哪個子集

**方法**：對 golden 生產樹（排除 `.svn`）逐檔 grep `HSys.BinDisCtrl` / `BinDisCtrl->`，
逐條讀函式本體確認呼叫的是基底類別（純資料操作）還是具體子類別（真實序列埠協定）。
指令：
```
grep -n "BinDisCtrl" cShowBinSelect.cpp cBinSel.cpp cSortCT.cpp csystem.cpp \
     database.cpp database.h acatchtray.cpp Motor/myMN200motor.cpp main.cpp
```
（於 `HT9011UC_Code_V3.33.906.0_20260618`，2026-08-20 03:52 執行，逐行列於下表）

### cShowBinSelect（Wave D 兩個卡住的函式）

| 函式 | golden 行號 | 用到的 BinDisCtrl 成員 |
|---|---|---|
| `ChangeBinDispStatus` | 208-386 | `UnitHasInstall(i)`、`GerErrNow(i)`、`GetColorNow(i)`、`GetBinNow(i)`、`bFirstInit`（欄位）、`ProcessStopStart(bool)`、`CommBin->StopComm()`（:346 註解掉、:366 生效，兩處都在 `#ifndef SOFT_SIMULTE`）、`GetRunStatus()` |
| `DoShowBinDigital` | 998-1423 | 僅 `NUMBER_PANEL_TYPE==3||4` 分支（:1000-1325）用到：`WriteTargetBin(i, bin[], color)`、`ProcessStopStart(true)`。**`NUMBER_PANEL_TYPE` 1/2 的舊式分支（:1327-1422）完全不碰 `BinDisCtrl`**——那段直接操作 `SW[]`（IO switch），已經可獨立翻譯，不受 opaque 卡住。 |

即：Wave D 兩個函式加總只用到基底類別的 **9 個成員**（8 個方法 + 1 個欄位），
沒有一個觸碰 `WriteBin/WriteColor/ReadVersion` 等純虛擬（硬體協定）方法。

### 其他 BinDisplay 族消費者（golden 樹內 grep 命中的全部生產呼叫點）

| 檔案:行 | 呼叫 | 性質 |
|---|---|---|
| `cBinSel.cpp:1721` | `ProcessStopStart(false)` | 資料層 |
| `cSortCT.cpp:408` | `WriteTargetCount(idx, count)` | 資料層 |
| `csystem.cpp:4354-4355` | `bFirstInit=true; ProcessStopStart(true)` | 資料層 |
| `csystem.cpp:6739-6740` | `if(BinDisCtrl!=NULL) FlashPro(Pos)` | 資料層（Eastsun 20260513 KYEC 閃爍警示，純狀態機） |
| `csystem.cpp:7412-7413` | `if(BinDisCtrl!=NULL) ClearAutoChangingWarn(Pos)` | 資料層 |
| `acatchtray.cpp:3826-3827` | `FlashPro(AutoTarget)` | 資料層 |
| `acatchtray.cpp:7064-7065` | `ClearAutoChangingWarn(Target-1)` | 資料層 |
| `Motor/myMN200motor.cpp:1172-1173, 2077` | `bFirstInit=true; ProcessStopStart(true)`（斷電回復後重 init） | 資料層 |
| `database.cpp:50,1686-1741` | ctor 歸零、`new TMyBinDispHT9046`、`SetComPort/SetComPort2/InstalledUnit/Alias[]/CloseUnit/SetDelayTime`、dtor `delete` | 資料層（初始化/組態） |
| `main.cpp:10485,11569` | `InitialOK=...`（欄位） | 資料層 |

**結論：目前 golden 樹內已知的每一個生產呼叫點，全部只碰基底類別 `TMyBinDispCtrl`
的資料層介面**（getter/setter/佇列寫入/簡單狀態機切換），**零個**呼叫點觸及
`TMyBinDispHT9046` 的硬體協定方法（`WriteBin`系列、`ReadVersion`系列、
`DoOnce/DoCycle`系列、`command_TFT_Input/Font`、`Magazine*_TFT`）。逐一驗證：

- `WriteTargetBin`（golden `MyBinDisp.cpp:606-638`）：純陣列寫入 + 佈旗標
  （`iSetBin[]/iSetColor[]/bSetBin[]/bStartSetBin/bStartOnce/bStartCycle`），
  **不呼叫** `CommBin`。
- `WriteTargetCount`（:640-646）：純陣列寫入。
- `ProcessStopStart`（:182-195）：`bStopProcess` 欄位切換 + 首次呼叫時 dispatch 到
  純虛擬 `InitialTask()`（**這一步需要一個具體子類別存在**，見 §3）。
- `GetRunStatus`（:156-170）：`switch(iRusStatus)` 回字串常數，純資料。
- `StartFlash/IsAnyFlashing/ProcessFlash/ClearAutoChangingWarn/FlashPro`
  （:3078-3168，Eastsun 20260513 KYEC 新增）：完全自包含的狀態機（`bFlash[]/
  iFlashColorA/B[]/bAutoChangingWarn[]/iAutoColorBak[]`），只讀 `IniConfig.
  bP66AutoChangingFlashWarn`，**不碰任何硬體或 `CommBin`**。
- `CommBin->StopComm()`（cShowBinSelect.cpp:366）：**唯一**真正觸及 `TComm`
  型別本身的生產呼叫點（呼叫其方法而非只是欄位存在），且已被 `#ifndef
  SOFT_SIMULTE` 包住。

---

## 3. 翻譯路徑提案

**不建議照 halarm 前例把整個 `Spcomm.pas`（1944 行）+ `TMyBinDispHT9046` 硬體協定
本體（`MyBinDisp.cpp` 扣掉基底類別後約 2500+ 行的 TFT/RS-232 位元組編碼）現在就搬進來。**
halarm 前例成立是因為 `PopUpAlarm/ClearAllAlarm` 本體小（golden `HAlarm.cpp:234/264`
附近，翻譯後落地約 1477 行是整個 `ckernel.cpp` 波次的量，非 halarm 本身）且邏輯是
「警報清單推導」這種**跨模組真行為**，實驗機上就算沒有真警報硬體，邏輯本身仍可測。
`TMyBinDispHT9046` 的核心是「组一段 16 進位 ASCII 字串送到實體 RS-232/RS-485 七段顯示板」
——這段協定：(a) 在實驗機上沒有物理 Bin 顯示板可以對照驗證，(b) 需要真的移植
`Spcomm.pas`（Win32 `CreateFile`/`OVERLAPPED`/讀寫執行緒的 Delphi 序列埠元件，非
簡單資料類別），跟 Motor/IO 已經做過的「先做 sim HAL、真硬體後補」模式是同一類問題，
不是同一類 halarm 問題。

**建議：facade / data-layer stand-in 現在就做，硬體協定本體另開「UI/hardware wave」。**

具體做法：

1. **`TMyBinDispCtrl` 基底類別忠實翻譯（不是簡化版，是逐字翻譯）**——因為前節證明
   它整個公開介面都是純資料操作，沒有理由簡化，直接照 golden 逐字搬：
   - ctor（golden :39-129，91 行）+ dtor（:133-143，11 行）
   - 10 個一行 getter/setter（:145-154）
   - `GetRunStatus`（:156-170，15 行）
   - `SetComPort/SetComPort2`（:172-180，9 行）
   - `ProcessStopStart`（:182-195，14 行）
   - `CommBinReceiveData/CommBinReceiveData2`（:199-269，71 行——內容是
     `ZeroMemory/memcpy/sprintf`格式化，不呼叫 `CommBin`，可原樣翻）
   - `InstalledUnit`（:273-280，8 行）
   - `WriteTargetBin/WriteTargetCount`（:606-646，41 行）
   - `StartFlash/IsAnyFlashing/ProcessFlash/ClearAutoChangingWarn/FlashPro`
     （:3078-3168，91 行）
   - 小計：**約 350-360 行**本體 + `MyBinDisp.h` 基底類別宣告部分（:34-159，
     扣掉純虛擬方法宣告本身只是簽名）約 **100-120 行**表頭。
2. **一個不含硬體協定的具體子類別**（golden `TMyBinDispHT9046` 的純虛擬覆寫全部
   給空/no-op 或只設 `iRusStatus`），滿足 C++ 不能實例化抽象類別的限制，讓
   `ProcessStopStart` 首次呼叫的 `InitialTask()` 有東西可呼叫。估 **20-40 行**。
3. **`TComm` 的最小 facade**（不是 `Spcomm.pas` 移植）：只需要 `StopComm()`
   （生產有呼叫）+ 為了結構完整性順手補 `StartComm()/WriteCommData()` 的
   no-op 版本，供未來子類別真正接硬體時覆蓋。估 **20-30 行**。
4. `database.h` 的 forward-decl 拿掉、`#include "MyBinDisp.h"`（真的），
   `database.h:227-229`/`database.cpp` 的 `InstallColorBinDisplay`/ctor/dtor
   對應的 `#if 0` 解開（這些本來就已經被 GA-1-B6 筆記標成「等 TMyBinDispHT9046」，
   見 `database.h:19-21`）。

**合計估計新增/解封約 450-550 行**，即可讓 cShowBinSelect Wave D 兩個函式、以及
`csystem.cpp`/`acatchtray.cpp`/`cBinSel.cpp`/`cSortCT.cpp`/`Motor/myMN200motor.cpp`/
`database.cpp`/`main.cpp` 全部 9 個既有 `#if 0` GATE（DEVLOG/ROADMAP 已列出 2 個在
acatchtray.cpp、其餘散落各檔）一次解除，**且是忠實翻譯，非簡化行為**——因為基底類別
本來就是純資料層，golden 自己就是這樣切的（硬體協定被切到子類別是 golden 原始
設計，不是 V906 發明的簡化）。

**明確標注「未做」的部分**（供之後 UI/hardware wave 用）：
`TMyBinDispHT9046` 的 `WriteBin/WriteBin2/WriteColor/WriteColor2/ReadVersion/
ReadVersion2/ReadVersion_TFT/MagazineWriteBin_HTA18/_BT008/_TFT/
MagazineWriteBinFont_TFT/SetNoBackGround_TFT/SetBackGround_TFT/SetFontBin_TFT/
SetFontBinWord_TFT/SetFontEA_TFT/SetFontCount_TFT/WriteBinWord_TFT/WriteEA_TFT/
WriteCount_TFT/WriteBin_TFT/DoOnce/DoOnceTFT/DoCycle/DoCycleTFT/InitialTask`
（golden `MyBinDisp.cpp` 約 :657-3077，估 **2400-2500 行**）+ `Spcomm.pas`
真移植（1944 行 Pascal→C++，含執行緒/Win32 COMM API）。**這條路徑目前零消費端
需要它**，且沒有物理硬體可在實驗機驗證，風險遠高於 §3 的 facade 路徑，建議
明確排入未排波清單、不要因為這次解 opaque 而順手動它。

---

## 4. 其他 V906 gate 也卡樹外元件套件（順手列出，不深入）

掃 `docs/DEVLOG.md`/`docs/MIGRATION_ROADMAP.md` 對 `elec\Component`/`opaque` 的既有記錄
（2026-08-20 04:03 執行，見上方指令），現況：

| 元件套件 | 狀態 |
|---|---|
| `elec\Component\halarm.h` / `HAlarm.cpp`（`PopUpAlarm`/`ClearAllAlarm`） | **已翻譯**（W7-L2，`ckernel.cpp` 1477 行波次） |
| `elec\Component\HAlarm.cpp:136 UpdateSystemNG()`（`SystemNG` 警報清單推導） | **未翻譯，已知安全缺口**——ROADMAP 標為「未排波（安全性，建議優先）」，非本次任務範圍 |
| `elec\Component\htimer.h/.cpp`（`HTimer`） | **已翻譯**（TU-local 移植，同時解掉 KYEC ⚠️） |
| `elec\Component\aled.pas`（LED 元件行為） | **已用不同手法解掉**——不是逐字搬 `.pas`，是 `vclcompat/LedCore.{h,cpp}` + `vclcompat/render/LedRender.{h,cpp}` 重新實作等價行為 |
| `elec\Component\Spcomm.pas`（`TComm`，本次新發現） | **未翻譯**，僅在 `BinDisCtrl` 的具體子類別內部需要；本 recon §3 建議暫不需要（facade 路徑不用真的搬它） |

另外 `forms/fObserver.h` 有一個**不同類別**的 opaque gate（golden `TChart*`），
`grep -n opaque forms/fObserver.h` 命中 :117/:122/:190，是 BCB6 內建/隨附的
TeeChart 圖表元件（**不是** `D:\HT9045\elec\Component\` 裡的自製套件），與本次
BinDisCtrl 查證屬不同類別問題，本 recon 未深入。

---

## 5. 摘要（回報用）

- **BinDisCtrl 身分**：`SYSTEM_MODULAR::BinDisCtrl`（`TMyBinDispCtrl*`，具體型別
  `TMyBinDispHT9046`），純 C++ class，**整份原始碼都在 golden 樹內**
  （`BinDisplay\MyBinDisp.h` 196 行 + `MyBinDisp.cpp` 3169 行），**不是**設計期
  VCL 元件、**不是** elec\Component 樹外套件、**不是**另一個獨立測試專案
  （旁邊的 `BinDisp.h/.cpp/.dfm`+`BinDispTester.bpr` 才是獨立硬體測試台，同名
  不同物件）。V906 卡住是自己刻意 forward-declare 延後（`database.h:63,300`），
  不是找不到料。
- **兩個消費者需要的子集**：`ChangeBinDispStatus`/`DoShowBinDigital`（連同其餘 7 個
  生產呼叫點檔案）**只用基底類別 `TMyBinDispCtrl` 的資料層介面**（9 個方法/欄位：
  `UnitHasInstall/GerErrNow/GetColorNow/GetBinNow/bFirstInit/ProcessStopStart/
  GetRunStatus/WriteTargetBin/WriteTargetCount` + `FlashPro/ClearAutoChangingWarn/
  InitialOK/SetComPort(2)/InstalledUnit/Alias[]/CloseUnit/SetDelayTime` +
  一個 `CommBin->StopComm()`），**零個**觸及硬體協定方法。
- **翻譯路徑建議**：忠實翻譯基底類別 `TMyBinDispCtrl`（~350-360 行本體 + ~100-120
  行表頭，全部逐字，不簡化）+ 一個 no-op 具體子類別（~20-40 行）+ 最小 `TComm`
  facade（~20-30 行，只給 `StopComm()` 有真呼叫），合計約 **450-550 行**，
  即可解除全部 9 個既有 `#if 0`/opaque GATE。`TMyBinDispHT9046` 的真實硬體協定
  （~2400-2500 行）與 `Spcomm.pas` 真移植（1944 行）留給未來「UI/hardware wave」，
  因為目前零消費端需要、且無實體硬體可在實驗機驗證。
- **其他樹外元件缺口**：`elec\Component\HAlarm.cpp:136 UpdateSystemNG()`（`SystemNG`
  推導）仍未翻譯，是既有已知安全缺口（非本次任務）；`Spcomm.pas`（`TComm`）是本次
  recon 新發現的缺口，但只影響 BinDisCtrl 具體子類別的硬體協定路徑，不影響
  facade 路徑。`aled.pas`/`htimer.*`/`halarm.h` 的 `PopUpAlarm/ClearAllAlarm` 三個
  已解決。`fObserver.h` 的 `TChart*` opaque 是不同類別的問題（BCB6 隨附 TeeChart，
  非 elec\Component），未深入。
- **最大風險**：如果之後有人為了「順手做完整」而直接把 `TMyBinDispHT9046` 的硬體
  協定本體和 `Spcomm.pas` 一起搬進來，會在沒有物理 Bin 顯示板可驗證的情況下引入
  約 4000+ 行不可測的序列埠位元組協定程式碼——這比目前的 opaque-NULL 狀態風險更高
  （現狀是「功能不存在」，忠實對照 golden 也是可預期的；貿然搬入卻驗不了，是
  「看起來翻完了但沒人知道對不對」）。建議 facade 路徑先落地，硬體協定路徑
  明確排入未排波清單並標注「需要實體 Bin 顯示板才能驗收」。
