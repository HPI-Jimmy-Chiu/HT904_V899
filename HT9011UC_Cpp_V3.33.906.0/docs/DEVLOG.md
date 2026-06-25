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

## 2026-06-26 — W0 尾段：全域標頭去 VCL 化完成（瓶頸解除）
- 翻譯（鏡射 root）：`MachineType.h`、`Config.h`、`CosFunction.h`、`myTimer.h`(+.cpp)、`cpublic.h`(+.cpp)、`cprod.h`(+.cpp)、`cmydef.h`(+.cpp)、`MachineDefine.h`。全部去 VCL 化、單一 TU 一起編可過（lib `ht9045_globals`）。
- **契約逐位保留**：PROD_INFO_ST 欄位 layout、enum 值/序、~250 個 CC_* 與 eTestMode/eContactMode/eInArmType… #define——用 static_assert/offsetof + runtime self-check 對拍。
- 處理：form 指標→前置宣告(W7)；前置宣告 TMyLog/TMyStringList/TColor=int/uPoint2D/TLargeInteger；ARM_OFFSET `_fastcall` typo 修；丟未用 include；`.cpp` 觸及狀態機 body `#if 0`→W3/W6（DEFERRED 表已記）。
- 驗證（獨立）：clean build、**ctest 11/11**（10 prior + globals）。無 vclcompat 缺口、Verify 階段 0 修補（headers 一次到位）。

## 2026-06-26 — W3 config/DB：前置已備（readers 待續）
- 誠實結果：production config/DB readers（cinitial/database.cpp/cMyDB/common.cpp）**本批未翻**——它們卡在缺少 TIniFile/TMemIniFile shim、且 database.cpp ReadGeneralIni 牽連 SECS/GEM + 數百全域。沒有硬翻，改先補前置：
  - vclcompat 檔案系統 API：FindFirst/FindNext/FindClose/TSearchRec/faAnyFile/faDirectory/faReadOnly/RemoveDir/FileSetAttr/FileGetAttr/HexStrToInt。
  - `third_party/sqlite3/`（CMake 模組，優先序：vendored amalgamation→find_package→header-only stub；現為 stub，因 SRC 只有 sqlite3.h+OMF lib、無 sqlite3.c）。
  - `tests/test_config_db.cpp` 真檔 oracle：對拍真實 system/ 檔（MOTION=1、IO=2、Mot 45 列全 SMC[名稱式 col22]、IO 15 欄 668 列 ISABase 644:24[名稱式 col8]），runtime-skip if absent。
  - 修 vclcompat TStringList SetCommaText/SetDelimitedText 尾分隔重複計數 fidelity bug。
- 驗證（獨立）：clean build、**ctest 12/12**。

## 2026-06-26 — W3-cont：config readers 閘門完成
- vclcompat `TIniFile`/`TMemIniFile`（IniFiles.h/.cpp）+ `common.cpp` ini-helper 家族 38 函式翻譯。真檔 oracle 過（譯後 CheckAndReadIniDataGeneral 鏈讀真 Gerneral.ini：MOTION=1/IO=2/TTL=2/INDEX=0/HEATER=4、missing→default；用 scratch copy 不動唯讀原檔；iconv CP950 交叉核對；原檔 mtime/size 未變）。
- 保留 faithful bug：CloseGeneralIniFile delete 後未 null INIFileGeneral（同 golden）。
- GetLastOpenFN/WriteLastDataFN 仍 `#if 0`（VCL ShowMyMessage/BCB6 FileCreate）；common.cpp 其餘區（path/file/timing/canvas/ui）gated。
- **發現 Win32 巨集衝突**（DeleteFile/CopyFile→…A shadow vclcompat 多載）→ 見 KNOWLEDGE，待在 vcl_compat.h 加 undef guard。
- 驗證（獨立）：clean build、**ctest 14/14**（+ini_helpers 25/25、IniFiles 43/43）。

## 2026-06-26 — W3-cont2：config-table loaders（真檔解析成功）
- 翻 database.cpp 8 個 config-table 函式（LoadMotData/LoadIoData/SetMOTTableNo/SetIOTableNo/TIODATA/TMOTDATA ctor）。**真檔 oracle 過**：LoadMotData 45 列、CardModel **名稱式** col22 解析(enum fallback 9→證明非位置式)、全 SMC(41 讀+4 index 因 INDEX_MOTION_CARD==0 強制 SMC)、M00→MInArmX/0/0；LoadIoData 668 列、ISABase 名稱式 col8、644:24。保留 faithful：catch log copy-paste bug、HomeDirectior 拼錯、SetMOTTableNo 的 `// else` 註解掉故 scan 無條件跑。
- Win32 巨集 guard 進 vcl_compat.h（undef DeleteFile/CopyFile/MoveFile，probe 確認、不破既有 TU）。
- **修 compat bug**：AnsiString 有 ctor(int) 但無 operator=(int)→`s=7` 綁到 operator=(char)(int→char 勝 user-defined)→存控制位元。加 operator=(int/uint/long/long long/double) 比照 BCB6。
- 已知連結缺口：cpublic.cpp 整檔 gated→cmydef.cpp 的 active queue 全域 ctor 未定義（測試用 stub 繞）；用 queue 的 exe 連結前須先 ungate cpublic 的 queue 方法(W6/W7)。
- 驗證（獨立，fresh build/）：clean build、**ctest 15/15**（config_loaders 22/22）。

## 2026-06-26 — W4 HAL 馬達層：interface-cut 離線證明
- 翻 Motor/HTMotor.h(.cpp) 虛擬基底 + Motor/mymotor(.h/.cpp 部分) + 新 Motor/mySimMotor(.h/.cpp)。**離線零 vendor SDK 可編可跑**：test_sim_motor 33/33（HTMotor* 與 TMyMotor 兩路徑：InitMotor ok、HomeObject→HomeFlag true+ReadPos 0、MoveToPos(N)→MotionDone+ReadPos N、GetAlarm false、Stop ok；含完整 MotorHome() 多 tick 狀態機收斂）。nm/strings 稽核：lib 零 vendor 符號、exe 零 vendor DLL import。**interface-cut 成立**（TMySimMotor 可頂任何品牌驅動於同一 HTMotor*）。
- **決策：6 個品牌驅動延 W4-part2**——每個 .cpp 都拉 ~30 模組 god-stack(main/csystem/sensors/cylinders/UI)，**卡在 W6 與 vendor SDK 無關**；故 #if HAVE_xxx 不足以解鎖、需先 W6。各品牌 vendor 符號族已記於 W4 workflow 輸出。cinitial 品牌派發(日後)預設 `new TMySimMotor()`。
- mymotor.cpp gated：MotorMovePosition/MotorMove/Galil 分支/sensor compare/TrayArm/continuous-move 本體 TODO(W6)；pHTray(VCL)→W7。
- 驗證（獨立，fresh build/）：clean build、**ctest 17/17**。

### 🔖 RESUME（最新）
- 已完成：…W0 基礎+尾段、W1、W2、W3(前置+ini+config-loaders)、**W4 HAL 馬達層(interface-cut 離線證明)**。全 green、已 commit。
- **下一步：W4-IO HAL** = `TLaneIO` 抽 `TIOBackend` 虛基底 + 新 `TSimIOBackend`(離線) + myswitch/mysensor/mycylin/mykitsuck 物件層；vendor IO(mn_*/Acm_*) `#if HAVE_xxx` 守。是 sensors/cylinders/W6 前置。之後 W4-part2(品牌馬達,需W6)、W5 comms、W6 root、W7 UI。
- **重要 ordering 發現**：品牌驅動/部分 mymotor 方法 include main.h/csystem/sensors → 它們其實在「狀態機之上」的 include 依賴，故須先 W6(sensors/cylinders/狀態機)再回補品牌驅動本體；Sim 路徑讓一切先可編可跑。
- 驗證指令：`cd HT9011UC_Cpp_V3.33.906.0 && export PATH=/c/MinGW/bin:$PATH && cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=C:/MinGW/bin/g++.exe -DCMAKE_C_COMPILER=C:/MinGW/bin/gcc.exe && cmake --build build && ctest --test-dir build`。
