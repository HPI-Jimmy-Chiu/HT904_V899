# HT9045 V906 BCB6 → C++（Visual C++）開發紀錄（DEVLOG）

> 開發過程逐步紀錄，供團隊與未來備查。累積知識見同資料夾 `KNOWLEDGE.md`。對外報告見 `D:\HT9045\docs\migration\RD5軟體_HT9045_906_*.md`。

---

## 2026-06-25 — 方向定案：目標是 C++（Visual C++），不是 C#

**核心作戰計畫**：把 `HT9011UC_Code_V3.33.906.0_20260618`（BCB6 / Borland 方言 / VCL C++）**翻譯成標準 / Visual C++**——脫離 Borland 方言（`AnsiString`→`std::string`、移除 `__fastcall`/`__property`、VCL 表單改由 UI 介面承接），**硬體與 UI 都用 interface 切割**（先 stub 開發、之後接實體控制與 UI 顯示）。回到使用者最初「改成 Visual C++」的目標。

**C# 方向已停用並移除**（重要）：先前因目標夾名 `_CSharp_` + repo 既有 899 C# 計畫，我誤判成 C#，並建了一套 C# .NET 骨架（ports / config-loader / EventLog PoC）。使用者澄清「我們沒有要變成 C#，只有 C++」。C# 程式碼已從工作樹移除，**完整保留在 git 歷史**（commit `326df6c` 及之前）可隨時回溯。資料夾已由 `HT9011UC_CSharp_V3.33.906.0` 改名為 `HT9011UC_Cpp_V3.33.906.0`。

**沿用的分析（語言無關，對 C++ 仍完全有效）**：
- 三道牆（VCL / 字串語意 / 64-bit 硬體驅動）。
- §9 HAL 接縫設計（HTMotor 虛擬基底、TLaneIO、KeyPro、tester、SOFT_SIMULTE）。
- 本機硬體 64-bit 範圍：CONTEC SMC ×41 + Galil index ×4 + MN200 IO 389 點 + DTK4848 溫控 + GPIB tester + KeyPro。
- 接縫盤點、log 語料位置、config 欄位語意、EventLog 格式（見 KNOWLEDGE + `docs/migration/RD5軟體_HT9045_906_64bit遷移計畫_20260625_193240.md`）。

**工具鏈（本機）**：MinGW `g++ 6.3.0` + `CMake 4.0.2`（**無 MSVC / clang**）。策略：翻譯成**可攜標準 C++**，用 g++/CMake 編譯驗證可攜性（同樣可在 MSVC 編）。最終 Visual C++/MSVC build 由開發機驗證。

**治理**：分支 `feat/csharp-906-migration`（沿用；內容轉 C++）；906 C++ 樹為唯讀黃金參考；`write-boundary-policy.json` 可寫根已改 `HT9011UC_Cpp_V3.33.906.0/`。

### 🔖 RESUME（下次接續先看這裡）
1. `cd /d/HT9045 && git log --oneline -6`；C++ 專案在 `HT9011UC_Cpp_V3.33.906.0`（目前僅 docs，C++ 結構待建），分支 `feat/csharp-906-migration`。
2. 工具：`/c/MinGW/bin/g++`、`cmake`（無 MSVC）。
3. 方法：**讀懂 9011UC 的 class/function → 忠實翻譯成標準 C++（保留名稱）→ 編譯/邏輯驗證**。原始碼是 source of truth；log/真實資料是驗證的尺（catch 翻譯行為漂移：1-based 字串、CSV 名稱解析等）。
4. 下一步：用 workflow 翻譯第一個 leaf logic class（建議 **ContactForce / cContact**：計算明確、可用已知數值驗證；或 Common/PickPlanner）BCB6→標準 C++，含 interface 切割。
5. 待決（不擋 leaf 邏輯翻譯）：UI 框架（MFC / Qt / Win32）留到觸及表單再定；實機確認 Galil/MN200 卡。
6. 慣例：每完成一塊 append DEVLOG + 自動 commit。

---

## 2026-06-25 — 第一個類別翻譯：ContactForce 計算核心（完成、g++ 驗證）

**目標**：示範「讀懂 9011UC class → 忠實翻譯成標準 C++ → 編譯/數值驗證」，並確立**檔案鏡射 BCB6 位置/檔名**慣例。

**讀碼修正（重要，回應「不清楚就看 9011UC 程式碼」）**：原以為 `ContactForce.cpp` 是 Contact Force 計算類別，實際讀碼發現：
- `ContactForce.cpp/.h` 是 **VCL 表單 `TfContactForce` + 四個 SLK 元件類別**（`THTSLKClass`/`THTSLKIndClass`/`THTDieForceSLKClass`/`THTDieForceOneByOneSLKClass`）——是 **SLK 直徑→力 的「資料產生端」**，不是計算消費端。
- 文件提到的 `CalculateTotalAirForce`/`GetMinForce`/`GetMaxIndexForceLimit` 實際在 **`cContact.cpp`（VCL 表單 TfContact，計算消費端，本次不翻）**，它讀 `fContactForce->SLKClass[i]->dDiameter/dMinForce/dMaxForce`。

**翻譯範圍（extract-calc-core）**：把 ContactForce 的純計算核心抽出成可攜 C++：
- 檔案（**鏡射根目錄**）：`HT9011UC_Cpp_V3.33.906.0\ContactForce.h` + `ContactForce.cpp`（同 BCB6 root 同名）。附加：`CMakeLists.txt`、`tests/`、`.gitignore`。
- API：`struct SlkForceData`、`enum SlkVariant`、`SlkForceData ComputeSlkForce(dia, tag, bDefault, epInstall, variant)`、`double ComputeEpMaxVoltage(dMinVol, dMidVol)`。
- 忠實點：**1-based→0-based 修正**（`Dia.Pos("Arm2_")==0`=未找到 → `find()==npos`；`SubString(6,..)`→`substr(5)`，且保留「Arm2_ 不在時才 strip」的 quirk 並加測試）；**402 特例僅 STANDARD 類**；**字面 `3.14` 保留**（不換 M_PI，維持數值同一）；DIEFORCE_OBO 的 `iCount` 只影響 caption 不影響數值。
- VCL 表單/widget/ReadFile/WriteFile/ShowValue/ini 欄位（dLoadRate/dContactOffset…）**未翻**（待 UI/IO port）；但 struct 保留這些欄位供日後填。

**驗證**（獨立重跑）：CMake(MinGW Makefiles)+g++ 6.3.0、`-Wall -Wextra -Wshadow -Wconversion -Wpedantic` 零警告、build OK、CTest 1/1、harness 45/45。輸出全等於**手算 BCB6 公式值**（STD Dia30→141.3、40→251.2、60→565.2、40x2→dMaxForce 25371.828、EP 1.0/2.0→2.8…）。
**限制**：無 Borland 編譯器→無法跑原 BCB6 binary 對 live diff；等價性＝g++ 乾淨編譯＋輸出等於手算公式值。

### 🔖 RESUME（最新）
- 已完成：(C# 已作廢移除) → C++ pivot、ContactForce 計算核心翻譯（root 鏡射、g++ 驗證、已 commit）。
- 慣例確立：**翻譯檔鏡射 BCB6 相對路徑/檔名**（見 KNOWLEDGE「檔案位置與命名」）；CMake+g++/CTest 驗證；無 Borland 故對手算公式值驗證。
- 下一步候選：(a) 翻 `cContact.cpp` 的計算消費端（CalculateTotalAirForce/GetMinForce/GetMaxIndexForceLimit，消費 ContactForce 的 SLK 資料；但 cContact 是 VCL 表單，需抽 calc-core）；(b) 翻另一個 leaf（Common/PickPlanner 或 Public 工具如 HTMD5/cJSON）；(c) 補 ContactForce 的 ReadFile/ini 載入（接 config）。
- 驗證指令：`cd HT9011UC_Cpp_V3.33.906.0 && export PATH=/c/MinGW/bin:$PATH && cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=C:/MinGW/bin/g++.exe -DCMAKE_C_COMPILER=C:/MinGW/bin/gcc.exe && cmake --build build && ctest --test-dir build`。

---

## 2026-06-26 — W0 基礎完成（vclcompat 層 + CMake + 首批 Public 葉工具）

**全面遷移啟動**（目標：全部轉移；順序見 `MIGRATION_ROADMAP.md`）。W0 = 載體最重的基礎：
- **`vclcompat/` 相容層**（附加目錄）：`AnsiString`（28 個實際用到的方法/運算子，**1-based** Pos/SubString/operator[]/Delete/Insert、int/double 建構格式化、operator+ 串接、UpperCase/Trim/sprintf 等）、`TStringList`、`TDateTime`(OLE double)、`SysUtils`(IntToStr/StrToInt/Format/FileExists/Now/FormatDateTime…)、`vcl_compat.h` 傘狀標頭（中和 __fastcall/__closure/__property/PACKAGE/DELPHICLASS；翻譯檔以此取代 `<vcl.h>`）。**第一次編譯就過，1-based 語意正確、無需修補**。
- **CMake 鏡射骨架** + tests/。
- **首批 Public 葉工具（鏡射路徑）**：`Public/cJSON.c/.h`（純 C，extran "C"，以 C 編譯）、`Public/HTMD5.cpp/.h`（用 compat AnsiString，MD5 演算法 bit-exact）。
- HTMD5 翻譯時處理的相依：VCL Masks `TMask` 萬用字元比對（重建簡易 matcher）、`std::auto_ptr`→`unique_ptr`(C++17)、`MyDBIProcess` logging stub、Win32 FindFirstFile（MinGW windows.h 可攜）；保留既有 buffer-overflow fix 註解。

**驗證**（獨立重跑）：CMake+g++ 6.3 clean build 零警告、**ctest 4/4 PASS**（ContactForce、vclcompat 語意、HTMD5、cJSON）。**MD5 命中 RFC1321 標準向量**（真 golden：""=d41d8c…、"abc"=900150…、"message digest"=f96b69…），cJSON parse/print/re-parse round-trip 14/14。限制：MinGW-only、無 Borland binary diff。

## 2026-06-26 — W1 第一批 Public 葉工具完成
- 翻譯（鏡射路徑）：`Public/ExternFunction`(純函式：Math_AdjustAngle/String_JustNumber/String_EraseL/RSpace/RecordTime)、`Public/WinSocketErrorCode`(GetErrorMsg switch)、`Public/cBootLog`(boot log，never-throw)。用 vclcompat，無需新增 compat 方法。
- **紀律點**：ReadPlan 正確**略過非 leaf**（MyStringList/HonNewDel/MemoryAlarm/MyProductionRecord/HTEdit*），延到對應波（見 ROADMAP DEFERRED 表）；部分檔只翻 leaf 段、耦合段延後。
- Verify 抓並修一個真 bug：`WinSocketErrorCode.h` 的 `#include` 缺引號（translate 階段誤報正確）。
- 驗證（獨立）：CMake+g++ clean build、**ctest 7/7**（W0 4 + W1 3；新 67 斷言）。對拍 golden 源語意（角度正規化、JustNumber、erase、MD5、WSA 錯誤碼），Big5 位元保留。

## 2026-06-26 — W2 邏輯島（部分）完成
- 翻譯：`Common/PickPlanner/`（IArmPickPlanContext.h、cArmPickPlan 通用 enumerate-and-score 引擎＝與 golden 幾乎逐字同，僅 include 換；cInArmLoaderPickPlanner/cOutArmPlacePlanner 為 **scaffold**，Search() 回 false、runtime gate 預設 false，本體綁 MOT[]/Prod/Suck → 延 W6）；`cUnitConvert`（純轉換翻了，Do*Convert glue 延 W6）。
- **SortingBinTray 整檔略過 → W6**（耦合 main.h/MyMotor/mycylin/atester/uLotInfo + cmydef TSortingBinTray_* + VCL AutoForm[]）。
- 無 vclcompat 新增。驗證（獨立）：clean build、**ctest 9/9**（含 test_PickPlanner 72/72，用 stub IArmPickPlanContext 驅動引擎＋邊界守則）。
- **重要瓶頸發現**：W2 多處延後都卡在「等全域標頭」→ 下一步必須先翻全域定義/型別標頭。

### 🔖 RESUME（最新）
- 已完成：C++ pivot、鏡射慣例、ContactForce、**W0 基礎**、**W1 Public 批1**、**W2 邏輯島(部分)**。全 green、已 commit。
- **下一步：W0 尾段＝全域標頭去 VCL 化**（`cmydef.h`/`cprod.h`/`cpublic.h`/`MachineType.h`/`MachineDefine.h`）。這是目前最大瓶頸（解鎖 cUnitConvert glue / SortingBinTray / PickPlanner adapters / W3 / W6）。注意 PROD_INFO_ST ~770 行、大量 enum/extern/`extern PACKAGE TfXxx*`（VCL form 指標→前置宣告/守起來，UI 在 W7）。
- **完整性**：ROADMAP DEFERRED 表追蹤所有半翻/略過項，最終回補。vclcompat 待補 FindFirst/TSearchRec(W3 前)。
- 驗證指令：`cd HT9011UC_Cpp_V3.33.906.0 && export PATH=/c/MinGW/bin:$PATH && cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=C:/MinGW/bin/g++.exe -DCMAKE_C_COMPILER=C:/MinGW/bin/gcc.exe && cmake --build build && ctest --test-dir build`。
