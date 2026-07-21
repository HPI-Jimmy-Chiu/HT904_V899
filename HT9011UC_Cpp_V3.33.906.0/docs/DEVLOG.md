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

## 2026-06-26 — W4-IO HAL：IO interface-cut 離線證明
- 新 `IOBackend.h/.cpp`：TIOBackend 虛基底 + TSimIOBackend(離線記憶體 bit/port map)；real backends(mn_*/Acm_*/_mnet_*/raw) `#if HAVE_MN200/PCI1203/MNET/RAWPORT` 預設 OFF。`MyLaneIo`(facade)6 method 經 `pIO->` 派發、保留 IdleCheckSafeDoor/CheckPortRangeErr/OutPortData/MyBitMask 記帳，ctor 預設 `new TSimIOBackend`。`myswitch`(TMySwitch)/`mysensor`(TMySensor) 路由到 facade（pad/raw 分支 #if 0→W6）。
- 驗證（獨立）：clean build、**ctest 19/19**（+test_sim_io 28 斷言：IOBitOn→IOOutBitStatus 反映、IOByteOut/IOInputBit round-trip、TMySwitch.On→bit、TMySensor.IsOn 讀回）。無 vendor SDK。
- gated→W6：mycylin Push/Pop 狀態機、TMySucker Suck/Destroy、TMyKitSuck item grid(2894行 god-stack)、myio raw-port。

## 2026-06-26 — HAL enablers：KeyPro shim + TComm serial shim
- `Public/HTKeyProShim.{h,cpp}`：`KeyPro_GetLevel(unsigned)`→LoadLibraryA("KeyProDLL.dll")+GetProcAddress("KEYPRO_GET_LEVEL"，fallback "_..@4")，離線回 1（valid dongle stub），**非靜態連結 OMF lib**。decoration＝undecorated（objdump 真 DLL 確認）；std::string 那幾支 906 零呼叫不翻。call site CheckKeyPro(VCL form)→W7。
- `vclcompat/Comm.{h,cpp}`：TComm shim（14 method：CommName/BaudRate/Parity/.../StartComm/StopComm/WriteCommData/OnReceiveData/...），Win32 \\.\COMx + SIM 模式（WriteCommData 入 SimTxBuffer、SimInjectReceive 觸發 OnReceiveData）。加入 vcl_compat.h 傘狀。
- 驗證（獨立）：clean build、**ctest 20/20**（test_keypro_tcomm 26 斷言：KeyPro_GetLevel(3)==1 offline、TComm sim tx buffer + rx callback）。
- 待辦註記：TComm real \\.\COMx 路徑已編未測(無硬體)；WriteCommData(char*) vs c_str()const 之 const-correctness 待 W7 consumer 調和。

## 2026-06-26 — ungate cpublic.cpp 基礎完成
- ungate 6 區（依賴僅 globals+vclcompat+TComm）：DTK4848 溫控 seam(LRC/WordWrite/WordRead 經 TComm，frame/LRC sim 驗證 exact memcmp ':0106470100FAB7\r\n')、queue 本體(TMyQueue10/100/StrQueue100/TimerQueue100，ring MAX_Q_10=500 對 golden)、ByteUnionBit/CRC_Check(Modbus CRC-16)、time/format/math/string util、VerInfo、Win32/file util。**移除 cpublic_ctors_stub.cpp**（real queue ctor 現由 cpublic.cpp 提供，nm 驗證 cmydef 全域解析到真 T 符號）。god-stack（logging/UT100/E5DC/temp-parse 需 T_ASXII2HEX）續 gate→W5/W6。
- 驗證（獨立）：clean build、**ctest 21/21**（test_cpublic_foundation 87 斷言）。
- **⚠ 發現中文註解亂碼**：cprod.cpp(283)/cpublic.cpp(62) U+FFFD（Read 工具 cp950→UTF-8 解錯所致；只在註解、編譯OK、golden 有原文）。見 KNOWLEDGE go-forward 規則。

## 2026-06-26 — W6.5 SHUTTLE/CARRY（InShuttle/OutShuttle 飛梭）引擎完成
- `acarry.cpp`(golden 8521 行) + `.h` 忠實翻譯，名稱/簽章/預設參數全保留。**CASE 覆蓋率 136/136 ACTIVE（100%）、acarry.cpp 內 0 個 `#if 0` gate**——本波的低外部耦合(0 tester-comm、0 cContact、fMain 走 FormsFacade)讓 shuttle 移動 case 本體全部 ACTIVE 翻譯，非 stub（如 prompt 所預期，類 acatchtray 全尺寸）。
- **核心 shuttle task SM 保持 ACTIVE 並 Sim HAL pump 過**：`Do_Auto_SHT1()`(cursor AutoSHT1Task) 從 entry 一 tick **1→10**(case 1 final `Task=10;`，golden:3342)；64-tick pump 後 cursor 落在 120(左移主線 100→…→120) 全程留在記錄的 switch(Task) universe {1,10,100,105,110,115,120,124,125,130,135,140,200,201,202,203,210,211,212,220,225,230,235,250,251,252,300,400,500,2300,2400,2500,2600,3000,3100,3101,3102,5000,5100}、無 crash、無逃逸。`Do_Auto_SHT2`(對稱 BRCarryKit/BLCarryKit/MInShuttle2) + 9046AU `Do_Auto_SHT3` 亦 ACTIVE 並連結。case 200→201 fall-through、所有 goto/fall-through verbatim。
- **5 個 oracle 全過(12 斷言)**：(O1) cursor 1→10；(O3) `GetShuttle1Pitch_9045` double→int **截斷**：w=12000→8000、w=18000→14000、i1180=14003/w=15000→r=11001.5→(int)**11001 不進位成 11002**；(O4) `CheckShuttleOutputHasICError` 零偵測位防護(Tech.*DetectPos=0 離線)→switch 前回 false、即使 bShuttleHasIC 標殘料也不誤報；(O5) `CheckShuttleSensorBroken_1(_,false)` 左向 pass：static iCheckCount=0 跳過 bRight 聚合臂→回 false。
- **uShuttleThread**：SM 邏輯 verbatim 翻；Win32 thread wrapper(HThreadCtrlShuttle::ThreadProcess + Initial/Start/EndShuttleThread)做成薄離線 driver（MySleepEx/MySleep 離線 no-op、HANDLE/DWORD token 走 acarry_shims）。
- 新增 `acarry_shims.{h,cpp}`（透明命名離線 cross-module shim，本體全 ACTIVE 無 `#if 0`）：Tech(6 偵測位 int=0)、fLtcSensor(latch count=0)、OutSht3Kit、ATC_InterfaceForm/ATC_Data(無 flood-gate)、CheckShtFloating 系列(false)、InShtInLF/RT(int) 路由真 predicate、SortSHT*/Check32site*(false)、DoStepShuttleCheck/EmptyIC/MustHasIC/Prominent、MoveOutArmXY_ToShuttleAlarmArea/SendHTTPRequest/MoveSortArmToAutoSafe。**修譯期連結缺口**(prompt 翻譯階段遺留)：IsNNMode/SetMotorScaleSpeed 改 forward-decl(真本體在 sibling shim)、MoveOutArmToAutoSafe/MySleep/MySleepEx/SystemNG/iCloseSiteStep_2x8/bCarryControlOutarm1/2/bPickShuttleError/OutShuttleLog/LogSoftwareOffTime 改離線 shim 定義(golden 初值 false/0/no-op)。
- substrate 擴：TMyKitSuck 加 iWhichAuto/bPass/bNeedReTest grid + TQPF_Timer TSoakTime + **忠實 golden 本體** SetAllRealIC2InterfaceBin/CountRealIC/HasDefineIC(僅讀 iMaxRow/iMaxCol/Item，離線 global grid 0 自然 no-op)；TfBarCode_Shim 加 17 個 in/out-shuttle 2D-barcode + float-check method(離線 false/no-op、IsSHT2DIDScanFinish 回 true 不卡死)；LastSet 加 iShuttlePurgeCount。**ODR 解決**：IsFLCarrKitAllHasIC/IsBLCarrKitAllHasIC 由 acarry.cpp 真本體(golden:175/197)持有，aHotPlateSubstrate.cpp 的 W6.2 placeholder stub 改 `#if 0` 移除。
- **mojibake=0**(全 9 檔)；acarry.cpp 真 line-leading #if/#endif 15/15 平衡、**SOFT_SIMULTE 11/11 directive 與 golden verbatim 全等**(本波低耦合無 gate 大 SM，故 SOFT_SIMULTE 全保=golden，異於 arm/tester 引擎的 gating 必然減損)。
- 新 test `tests/test_w6_5_shuttle.cpp`(W6_5_Shuttle)。**驗證(fresh from-scratch dir build_w6_5_clean)：clean build、ctest 32/32**(原 30/30 committed baseline 全綠無回歸 + 工作樹 W6_4_TesterAnchor + 本波 W6_5_Shuttle)。

## 2026-06-26 — W6.6 csystem ORCHESTRATION HUB spine 完成（🏁 整機週期 pump 里程碑）
- 🏁 **里程碑：完整機台週期現在 over Sim HAL 一個 tick 一個 tick pump 起來了**。`csystem.cpp`(golden 25,483 行 god-file)的**整合 SPINE** 翻譯完成，把 W6.0→W6.5 五個已翻引擎(in-arm/out-arm/tray-arm/tester/shuttle)由 `DoAllProcess()` 主迴圈**串在一起 tick**，全程 Sim HAL、無硬體、無 crash。
- **主迴圈 fn + 引擎 tick 序**：`void DoAllProcess()`(golden:9115) 是 per-tick 引擎驅動。function-static `bDoProcess` 每 tick toggle(golden:10254 `bDoProcess=!bDoProcess;`)，給出 2-tick 交錯：
  - **TICK A**(`bDoProcess==true`)：ProcessSensorScan → DoLoad → **DoInArm** → **Do_Auto_SHT1** → **DoTestHeadMotor** → **DoCatchTray**
  - **TICK B**(`bDoProcess==false`)：ProcessSensorScan → DoLoad → **Do_Auto_SHT2** → **DoTestHeadMotor** → **DoCatchTray** → **DoOutArm**
  - `Do_Auto_SHT3`/`DoSortArm` 僅 `USE_OUT_SORT_ARM!=eartUninstall`(預設 uninstall→skip，9046AU 才有)。`MainProc()`(golden:16711) 是薄外層 entry，dispatch `DoAllProcess()→DoOneCycleFinishCheck()→DoCleanOutFinishCheck()`(golden:18728-18740)。
- **ACTIVE spine 方法**：DoAllProcess(引擎 tick skeleton verbatim—bDoProcess toggle、8 個 master `SoftStop/SystemStart/fAllMotorHome` guard、5 引擎呼叫、DoLoad/DoCatchTray/DoAutoReceiveBinTray loop、tail vibration/alignment loop)、MainProc(instrumentation head + 薄 spine dispatch)、alive-instrumentation `GetMainProcCallCount/GetMainProcLastEnterTime[String]/GetMainProcSilentSeconds/IsMainProcAlive`(verbatim)、InitAllProcessTask、CheckMotorHome、SetInitialICCheck、InitOneCycle、InitCleanOutFunction、InitTrayEndFunction。
- **GATE 清單(`#if 0 // TODO(W7)` + active path)**：MainProc 內 2000+ 行 mode/SECS/AGV/temp ladder(golden:16730-19101)；DoAllProcess 內 AutoRetest/Laser/InitialICCheck/Magazine/Cylinder/LoadCell/ATCTemp/Decay/AutoClean/EmptySocketCheck/OCR/GPIB-wait/UTAC-wait/ClearSocket early-return ladder(golden:9165-10047)；InitCleanOutFunction 的 AutoSiteMap 分支(HotPlateForm/fMain mode setter)；DoHotplateEdgeCylinderLoop/DoLoaderVibrateLoop 本體。DoOneCycleFinishCheck/DoCleanOutFinishCheck 大宗(~2900 行)→active no-op stub(shim)。
- **shim 路由(ACTIVE，csystem_shims.cpp，全無 `#if 0`)**：DoInArm(golden head guard→真 `DoInArm_9045()`)、DoOutArm(`SetFixTrayMiddleDtata`+真 `DoOutArm_9045()`)、DoLoad/DoSortArm no-op、bShuttleShake=false、InitLoadTask/InitOutArmTask/InitialDoLockUnloader/Initial_Auto_BinTray_Task/InitAutoColorTask/4×Magazine init、DoAuto3Magazine/NewDoAutoTrayEdgeCylinderLoop/DoAuto2、DoInArm/OutArmTeachAlignmentProcess(回 true)、DoOneCycleFinishCheck/DoCleanOutFinishCheck。
- **ODR skip(已記錄)**：HasIC 家族 + InSHT*/OutSHT*/CheckSafeDoorIsClosed/DoAutoSiteMappingDropError/HasICUnderMachine/HasAutoICInMachine/HasAnyICInMachine/HasICUnderHotPlate→留 csystem_predicates.cpp(本波**零交集**已稽核)。DoTestHeadMotor/DoTestY(atester)、DoInArm_9045/DoOutArm_9045/Do_Auto_SHT1/2/3/DoCatchTray + 其 init→引擎(被呼叫、永不重定義)。
- **CASE 覆蓋率(誠實，spine 波)**：csystem.cpp 翻譯 **672 行**(active)，含 **~16 個 active 方法定義** vs golden **~243 個 top-level 函式**(≈6% 方法)、~2.6% 行——其餘 ~227 函式/~24,800 行 gated/deferred(mode/SECS/AGV/temp ladder、end-of-lot finish check、teach/setup、alarm-UI)。**如 spine 波預期是少數**(類 atester 引擎)，非全檔翻。
- **SOFT_SIMULTE 誠實註記**：golden csystem.cpp 有 50 處 SOFT_SIMULTE，但**全部落在被 `#if 0` gate 的大宗 ladder 內**(如 golden:9681 AutoClean ladder、golden:5745 CheckInitStartSuckStatus 皆非 spine)；active spine 範圍(DoAllProcess/MainProc head/InitAllProcessTask/小 setter)在 golden 本就 0 處 SOFT_SIMULTE。故譯檔 0 處=忠實，非漏譯。**所有 SOFT_SIMULTE 隨其所在的 gated 本體一起 gate、無一被改寫**。
- **substrate 補**：`TfMain::ProcessSensorScan()`(golden main.h，DoAllProcess:9130 唯一 active fMain deref) + `int iAllArmZHomeTask`(InitAllProcessTask:5718 設、consumer DoArmZHome 未翻)。一個記錄的 substrate GAP：`TMyKitSuck::SetHasNullIcToNullIc` 尚未在 substrate，故 DoOutArm golden:1197-1209 kit-prep skip(離線 carry kit 起始空→本就 no-op；TODO W7)。
- **驗證 TU(本波 Verify task)**：新 `tests/test_w6_6_csystem_cycle.cpp`(W6_6_CSystemCycle)——**整合機台週期** plain-assert harness。設 KNOWN Sim HAL state→pump 主迴圈 N=64 cycle→斷言 4 oracle(全 hand-derived from golden、每條附 golden 行號)：
  - **O1 TICK-SEQUENCE**(golden:9115/:10254)：4-tick(T,F,T,F) 引擎 token 流逐字相等；DoInArm/SHT1 僅奇 tick(:10072)、SHT2/OutArm 僅偶 tick(:10121)、DoLoad/TestHead/CatchTray 每 tick、SHT3/SortArm 缺席(eartUninstall:10112/:10125)。
  - **O2 INSTRUMENTATION/NUMERIC**(golden:16687-16728)：CallCount 呼叫前==0、5×MainProc 後==5(:16728 每次+1)；LastEnterTimeString 呼叫前=="N/A"(:16689)、後≠"N/A"；IsMainProcAlive(60) 呼叫前 false(:16705)、後 true。
  - **O3 GUARD**(golden:10049)：fAllMotorHome=false / SystemStart=false / SoftStop=true 三種→DoAllProcess 跑完 ProcessSensorScan 即 return、trace **只有 [SensorScan]、零引擎 token**。
  - **O4 TOGGLE/PUMP STABILITY**(golden:10254)：64 cycle pump，每 tick clean A 或 B、嚴格交錯=**32 A + 32 B**、head 永遠 [SensorScan,DoLoad]、無 crash。
  - 結果：**20 PASS / 0 FAIL**。(離線時 `DoInArm_9045()` 印 `iInArmType=0 Program Error` 是引擎自身對未載 setup 的離線診斷—不 crash、graceful return、週期照常 A/B pump。)
- 既有 W6_6_Hub(前一階段建)亦續綠。**mojibake=0**(本波全 6 檔：test_w6_6_csystem_cycle.cpp / tests/CMakeLists.txt / csystem.cpp / csystem_shims.{cpp,h} / test_w6_6_hub.cpp)。csystem.cpp 真 line-leading #if/#endif **7/7 平衡**。
- **驗證(fresh from-scratch dir `build_w6_6_verify`，最大 build—5 引擎+hub)**：clean configure+build+link `ht9045_sm`、**ctest 34/34 PASS**(prior baseline 32 全綠無回歸 + W6_6_Hub + 本波 W6_6_CSystemCycle)。

## 2026-06-26 — W6.2c batch-1：5 個 in-arm site variants（接手平行途徑 in-flight 工作）
- **接手脈絡**：暫停期間平行途徑已 commit W6.4/W6.5/W6.6(csystem HUB)；工作區留有 in-flight 未提交的 `ainarm9045_1x3_4.cpp`(2996 行,完整) + 4 個 header stub + substrate +13 行。使用者指示「繼續未完成」→ 我接手 W6.2b in-arm 變體 fan-out。
- **記取 W6.4 教訓的安全架構**：atester.cpp(11819 行)單 agent 翻譯曾在 ~105 分連線中斷。本波改 **parallel translate（每變體 ~3000 行,各 agent 只寫自己的 .cpp/.h、不碰共享檔、回報 substrate 需求）→ serial integrate（單一 agent 統一改 CMake/ainarm9045.cpp dispatch/substrate，杜絕平行撞車）→ clean verify**。
- **本批 5 變體**：`1x3_4`(incorporate in-flight,validated faithful) + `1x2_2`/`1x2_2_14`/`1x2_4_Hot`/`1x3_2_14`(新翻)。整合：10 externs + 5 DoInArm arms + 5 SuckerMap arms 移出 `#if 0 // TODO(W6.2b variants)` 區塊到 active(1x2_2 測 e9045_1x2_2_13；三個 1x2_* SuckerMap 帶 i1x2_4UseACEGPicker=0 prefix verbatim；剩 17 變體續 gated)。substrate 新增忠實本體：ArmUp/Down/Left/RightSideHaveRealIC(golden mykitsuck.cpp Item-grid 掃描)、flag1_1x2_2、**Zteach facade 單例(全專案首個 Zteach 編譯參考)**、csystem_shims 的 XPitchIsStand/DoSiteMappingCHK/AutoSiteMappingCheckShuttle。
- **驗證(獨立 clean from-scratch)**：build green、**ctest 36/36**(prior 35 + 新 W6_2c 22 斷言:XPHSuckToSht_1x2_4 表/Arm*SideHaveRealIC grid/GetShuttleState_1x3_4/dispatch routing + gated Program-Error witness)、mojibake 0(15 檔)、directive 21/21 平衡、126 譯出源檔。commit `168db2a`(30 ahead)。
- **下一步：W6.2c batch-2**(剩 17 個 in-arm 變體分批,同架構) → 26 個 out-arm 變體 → W7。

## 2026-06-26 — W6.2c batch-3 + batch-4：in-arm 變體 fan-out 收尾（25/25 ainarm9045_* 全翻）
- **batch-3(commit `dd084f3`)**：6 變體 `1x4_4`/`1x4_4_Back`/`2x3_6`/`2x3_6_14`/`2x4_4`/`All_1Pick`（前一途徑 commit，本 session 未經手；無獨立 DEVLOG 敘述，僅在此記 tally）。
- **batch-4（本 session，reusable workflow `wqet285gb`/run `wf_aed1c7ad`，8 agents/1.04M tok/~74min）**：最後 5 變體 `2x2_8_Hot`/`2x4_8`/`2x5_8`/`2x6_8`/`2x8_8`（golden 3529–5186 行）。沿用三段式架構（parallel translate 只寫自己 .cpp/.h+回報 substrate 需求 → serial integrate 統一改共享檔 → clean verify），記取 atester 單 agent 中斷教訓。接手在途 `2x2_8_Hot.cpp`(4030 行)+`2x4_8.h`(stub)由各自 agent 重新忠實翻譯覆蓋。
- **整合（serial）**：ainarm9045.cpp 兩個分派 ladder 各移出 6 arm 到 active（DoInArm 981-986 + SuckerMap 1064-1069），含 **`e9045_2x8_32→DoInArm_9045_2x8_8()`**（golden :4639/:4804 路由，2x8_32 自身檔為 dead-but-compiled、其函式永不被呼叫，忠實重現）；5 變體無 picker-prefix（對照 golden :4603-4641/:4768-4806 確認）。enum 值核對 MachineType.h（2x2_8_Hot=17/2x4_8=22/2x5_8=23/2x6_8=24/2x8_32=25/2x8_8=26）。
- **substrate 新增（忠實 golden + offline-safe）**：`TMyKitSuck::SetNullIcToHasNullIc()`(golden MyKitSuck.cpp:340-352 Item-grid 掃描，2x2_8_Hot 用)、`RowCanDualSite()`(golden ainarm2.cpp:1554，offline 回 false=唯一 true 早出 gate 在未模擬的 ArmCanSuck4IC)、`IsMoveInArm2XYToShuttle2Wait()`(acatchtray_shims，golden ainarm2.h:142，offline false)、`XPHSuckToSht_2x8_8_OutArm[][2][8]`(out-arm 波未翻先 zero-init，in-arm CheckSTMMode_2x8_8 寫先於讀)。**解 6 個 link/decl 衝突**：移除 substrate 的 placeholder `iCloseSiteModeFor2x6=0`/`iCloseSiteModeFor2x8=0`(2x6_8/2x8_8.cpp 現持真 def =e2x6Standard/e2x8Standard，皆 0)、acarry_shims 的 `iCloseSiteStep_2x8=0`、`enum{e2x6OneByOne=2}` 以 `#ifndef ainarm9045_2x6_8H` guard（變體檔現持完整 `enum e2x6Mode`，值 2 不變）。
- **驗證（獨立 clean from-scratch + 本 session 再獨立複驗）**：ctest **39/39**（prior 38 + 新 `test_w6_2c_inarm_variants_batch4.cpp`，26/26 斷言:XPHSuckToSht_2x2_8/2x4_8/2x5_8/2x8_8/2x6_6 幾何表 + 5 變體 differential dispatch witness(engine==direct) + 2x8_32→2x8_8 routing + 2 gate control + SuckerMap grid 對 fInArmSuck4x8 的 true/false falsify-guard）。**mojibake 0**(23 檔 EF BF BD byte-count)、**directive 平衡**(ainarm9045.cpp 21/21 + 5 變體各自平衡)、5 變體 entry+SuckerMap nm T defined。修 4 個 prior-batch ctest 的 stale gate-control（原指向已 un-gate 的 e9045_2x4_8/e9045_2x8_32，改指真正仍 gated 的 e9045_2x4_4_13/e9045_1x4_4_13）。
- **誠實 tally**：**25 個 `ainarm9045_*.cpp` 變體檔全翻+dispatched ACTIVE**（除 2x8_32 dead-but-compiled）。整合時浮現 **2 個仍 gated 的 dispatch 入口** `e9045_1x4_4_13→DoInArm_9045S_1x4_4` / `e9045_2x4_4_13→DoInArm_9045_2x4_4_13`，其 callee 在**獨立的 `ainarm9045S_*` 檔家族**（golden `ainarm9045S_1x4_4.cpp`/`ainarm9045S_2x4_4_13.cpp`，非原 25 檔之一，未翻）→ 小型後續波。
- **下一步**：(a) `ainarm9045S_*` 2 檔（解最後 2 個 gated in-arm 入口）；(b) **26 個 out-arm 變體**（同三段式架構，reuse workflow script 改 BATCH + 換 aoutarm9045.cpp dispatch 錨點）；(c) W7。

## 2026-06-26 — W6.2c batch-5：in-arm S-family（🏁 in-arm dispatch 100% un-gated）
- **batch-5（workflow `wlxdwkg7t`/run `wf_4f423afa`，5 agents/583K tok/~36min）**：2 個 irregular-named S-family 檔 `ainarm9045S_1x4_4.cpp`(2822 行,定義 `DoInArm_9045S_1x4_4`+`_SuckerMap`，**S 在 func 名**) + `ainarm9045S_2x4_4_13.cpp`(3111 行,定義 `DoInArm_9045_2x4_4_13`+`_SuckerMap`，**func 名無 S**)。名稱不規則故 workflow 用 explicit file→func→enum 映射（非 `<v>` 模板）。
- **整合**：兩個 ladder 都接上——DoInArm_9045 ladder（995/996）`e9045_1x4_4_13→DoInArm_9045S_1x4_4`(golden :4567)、`e9045_2x4_4_13→DoInArm_9045_2x4_4_13`(golden :4615)；SuckerMap ladder（1077/1078）兩個 `_SuckerMap` arm（golden :4732/:4780，無 picker-prefix）。**DoInArm_9045 ladder 的 `#if 0 // TODO(W6.2b variants)` 區塊整個移除**；SuckerMap ladder 留一個 EMPTY `#if 0` placeholder（1079-1081，僅註解，balanced，vestigial 無害）。substrate **零新增**（成熟 substrate 全解析）。
- **連帶修正**：un-gate 後，5 個 prior-batch test 檔內 8 條把這 2 enum 標為 gated（Program-Error/iArmTask 不變）的 stale negative assertion 被正確翻成 live-route witness（cursor 前進 / engine==direct callee），保留各 test 原意。
- **驗證（clean from-scratch + 本 session 獨立複驗）**：ctest **40/40**（新 `test_w6_2c_inarm_Sfamily.cpp`/test #32，15/15 oracle）、4 個 S-family func nm T defined、兩 ladder runtime dispatch witness 過、mojibake **0**(13 檔)、directive ainarm9045.cpp **20/20** + 2 變體各 1/1。
- **🏁 里程碑：in-arm 變體 dispatch 100% un-gated**——25 個 `ainarm9045_*` + 2 個 `ainarm9045S_*`，全部 ACTIVE（2x8_32 dead-but-compiled 例外）。in-arm 側全清。

## 2026-06-27 — W6.2c-OUT batch-1：接手 in-flight 6 個 out-arm 變體（驗證 green、commit）
- **接手脈絡**：上一途徑在工作樹留下未提交、未驗證的 out-arm batch-1（6 個 LIVE 變體 `1x1_1`/`1x2_2`/`1x2_4`/`1x3_2_14`/`1x3_4`/`1x4_2` 的 `.cpp`/`.h` + 新 `aoutarm_shims.cpp` + 新 test `test_w6_2_outarm_variants.cpp`，外加 CMakeLists/aoutarm9045.cpp/aHotPlateSubstrate.{h,cpp} 的整合改動）。本 session 接手：先**獨立 clean from-scratch 驗證**再決定提交。
- **out-arm dispatch 雙 ladder（golden 權威，已對讀）**：`DoOutArm_9045()`(golden :539-628) 20 個 LIVE callee（`USE_PICKER_COUNT==ep1Picker`→All_1Picker 先判；`e9045_1x4_4_Back`→**空 body**=dead；**無 final else**=靜默 no-op）；`DoPickFromShuttle_9045(int)`(golden :2038-2131) 19 個 callee + **有 final else=Program Error**（無 All_1Picker）。整合 LIVE 變體 = 加 CMake + 移除該變體 2 個 `static` no-op stub（`DoOutArm_9045_<v>(){}` + `DoPickFromShuttle_9045_<v>(int){return false;}`）+ 把 2 個真 extern 移出 `#if 0 // TODO(W6.2b variants)`；dispatch ladder 本身已 active、不動。
- **substrate（batch-1 加在 aoutarm9045.cpp shim 區，offline-safe、golden-cited）**：`ErrorBinBoxDetect(int)`→false(golden aoutarm.h:124)、`PorcessJAM0201OutArmPickUpErrorSkip(...)`→no-op(golden Steven 20161214)。
- **環境修正（重要、影響後續所有波）**：Bash 工具啟動時 PATH 被剝離——coreutils（`rm`/`head`/`cat`）與 `cmake`/`ctest` 都不在預設 PATH，原 workflow 的 `export PATH=/c/MinGW/bin:$PATH` 會讓 agent 的 build/verify 步驟 exit 127。正解：`export PATH="/c/Program Files/Git/usr/bin:/c/MinGW/bin:/c/Program Files/CMake/bin:$PATH"`。已更新 scratchpad 的 `w62_outarm_variant_batch.js`（TOOLCHAIN + syntax-check 兩處）。
- **驗證（本 session 獨立 clean from-scratch `build_oa_baseline`）**：configure OK → build OK → **ctest 41/41 PASS（0 fail，370s）**（prior 40 + 新 `W6_2_OutArmVariants`）；aoutarm9045.cpp directive **22/22 平衡**；batch-1 全 16 檔 **mojibake 0**（U+FFFD byte count）。→ 提交。
- **下一步**：B2（7 LIVE：`1x4_4S`/`1x4_4`/`1x4_8`/`2x1_2`/`2x2_4`/`2x2_8`/`2x3_6_14`）→ B3（7 LIVE：`2x3_6`/`2x4_4`/`2x4_8`/`2x5_8`/`2x6_8`/`2x8_8`/`All_1Picker`）→ B4（6 DEAD）→ B5（S-prefix `aoutarm9045S_1x4_4`，dead-but-compiled、不規則命名）。

## 2026-06-27 — W6.2c-OUT batch-2：7 個 out-arm LIVE 變體（workflow、clean verify、commit）
- **batch-2（workflow run `wf_0bcdc6f8-fc6`，10 agents/1.15M tok/~30min）**：7 個 LIVE 變體 `1x4_4S`/`1x4_4`/`1x4_8`/`2x1_2`/`2x2_4`/`2x2_8`/`2x3_6_14`（golden 1230–1570 行）。三段式：parallel translate（7 agents 各只寫自己 `.cpp/.h`、回報 substrate 需求）→ serial integrate（統一改 CMake/aoutarm9045.cpp/aoutarm_shims.cpp）→ clean verify。修好的 reusable script PATH 讓 agent build/verify 全程無 exit 127。
- **整合（serial）**：CMake 加 7 個變體；aoutarm9045.cpp **移除 14 個 static no-op stub**（7×DoOutArm_9045_<v> + 7×DoPickFromShuttle_9045_<v>）、**un-gate 14 個真 extern**（移出 `#if 0 // TODO(W6.2b variants)`）；dispatch ladder arms 未動（已 active、verbatim）。directive 22/22 平衡。`#if 0` 區只剩真正未翻的 All_1Picker/2x3_6/2x4_4/2x4_8/2x5_8/2x6_8/2x8_8。
- **substrate**：新增 1 個 golden-cited offline shim `CheckDuplicateBarCode()→true`（aoutarm.h:130，僅 1x4_4 在 case 3000 於 `CosFunction.bBarcodeDuplicateFileByOutArm`(離線 false)下呼叫）到 aoutarm_shims.cpp；變體以 `#ifndef AOUT_CHECKDUPLICATEBARCODE_FWD` forward-declare。
- **⚠ 一個未預期 link fix（記下供 B3）**：`2x3_6_14` 的 DoPickFromShuttle case-1 verbatim 呼叫**兄弟匯出** `MoveOutArmToShuttleIncludeZ_9045_2x4_4(int,int,bool)`（golden aoutarm9045_2x3_6_14.cpp:472），但 `2x4_4` 不在本批 → undefined reference。integrate 加了**標記待移除的暫時 offline stub** `bool MoveOutArmToShuttleIncludeZ_9045_2x4_4(int,int,bool){return true;}` 到 aoutarm_shims.cpp。**B3 翻到 2x4_4 時必須移除此 stub**（否則 duplicate symbol）——已寫進 B3 script 的 carve-out。
- **驗證（workflow verify clean-from-scratch + 本 session 獨立複驗 `build_oa_b2`）**：configure→build 100%→**ctest 41/41 PASS（0 fail）**；nm 確認 14 個 entry symbol 全 `T`（無 static stub、無 dup）；dispatch ladder 路由正確；extended test exe 30/30；directive 22/22 + 各變體平衡；touched 全 14+4 檔 **mojibake 0**。oracle：`GetNowShuttleMode_2x1_2`(golden:30) 離線 (0)==0/(1)==0（ptrOutSHT 綁全 NULL_IC carry-kit → UseSiteNoIC()==true → return 0）。
- **下一步**：B3（7 LIVE：`2x3_6`/`2x4_4`/`2x4_8`/`2x5_8`/`2x6_8`/`2x8_8`/`All_1Picker`；All_1Picker 是 DoOutArm-only/picker-gated、無 DoPickFromShuttle callee；integrate 移除 2x4_4 暫時 stub）→ B4（6 DEAD）→ B5（S-prefix `aoutarm9045S_1x4_4`）。

## 2026-06-27 — W6.2c-OUT batch-3（FINAL LIVE）：🏁 out-arm 變體 dispatch 100% live
- **batch-3（workflow run `wf_f99b95da-51f`，10 agents/1.35M tok/~38min）**：最後 7 個 LIVE 變體 `2x3_6`/`2x4_4`/`2x4_8`/`2x5_8`/`2x6_8`/`2x8_8`/`All_1Picker`（golden 1408–2923 行）。三段式同前。
- **整合（serial）**：CMake 加 7 檔；aoutarm9045.cpp **把整個 `#if 0 // TODO(W6.2b variants)`/`#else`/`#endif` 區塊整個收掉**（刪 7 個 DoOutArm + 6 個 DoPickFromShuttle static stub、un-gate 對應 extern）→ directive 22→**21/21**（少一對 #if/#endif=該 variant-stub 區）。兩個 dispatch ladder 未動、verbatim、nm 確認全 callee `T`+`U`。
- **carve-out 兌現**：(1) **All_1Picker DoOutArm-ONLY**——只定義/un-gate `DoOutArm_9045_All_1Picker()`，無 DoPickFromShuttle callee（picker-gated 首支 `USE_PICKER_COUNT==ep1Picker`）；其檔內 file-local `DoPickFromShuttle_9045_All_1Pick`(`_1Pick` 拼寫)未動。(2) **移除 B2 遺留的 2x4_4 暫時 stub**——`aoutarm9045_2x4_4.cpp:614` 現持真 `MoveOutArmToShuttleIncludeZ_9045_2x4_4`，shims 只留一行 provenance 註解（非定義）。
- **substrate**：`TMyKitSuck` 加兩個 golden-verbatim member `HasRealIC_Left(int,int)`/`HasRealIC_Right(int,int)`（golden MyKitSuck.cpp:458/486，左半 j2=j、右半 j2=j+Mid，回 true iff 有 real IC），供 2x5_8/2x6_8/2x8_8。`XPHSuckToSht_2x8_8_OutArm` ODR：移除 substrate 的 zero-init placeholder（2x8_8.cpp 現 own 真陣列、nm 單一 D）。aoutarm_shims 加 9046AU sort-engine offline shim（`USE_OUT_SORT_ARM==eartUninstall` 永不 dispatch 但需 link）+ AutoTeach close-pitch + out-rotate kit（All_1Picker 用）。
- **驗證（workflow verify clean-from-scratch + 本 session 獨立複驗 `build_oa_b3`）**：**ctest 41/41 PASS（0 fail）**；變體 TU `W6_2_OutArmVariants` **46/46 斷言**（24 batch-1/2 + 16 batch-3 dispatch witness + 2 Oracle-2 + 4 geometry）；nm 7 DoOutArm + 6 DoPickFromShuttle 全 `T`+`U`、static stub 全消；directive 21/21 + 各變體平衡；21 檔 **mojibake 0**。oracle：`GetNowShuttleMode_2x4_4`(golden:38) single-pick (iXStep==1&&iYStep==1) early-return ==2；7 變體 dispatch witness（OutArmTask 1→…→200）。
- **🏁 里程碑：OUT-ARM 變體 dispatch 100% LIVE**——20/20 LIVE 變體全 ACTIVE+dispatched。in/out 兩臂變體側全清。剩 out-arm **6 DEAD**(B4) + **1 S-prefix dead**(B5) 為 compile-only 完整性收尾，之後 W7。

## 2026-06-27 — W6.2c-OUT batch-4（DEAD）：6 個 compile-only 變體
- **batch-4（workflow run `wf_f3a6e4b1-390`，9 agents/1.6M tok/~51min）**：6 個 DEAD 變體 `1x4_4_Back`/`2x2_4_14`/`2x2_4_23`/`2x4_16`/`2x8_16`/`2x8_32`（golden 1916–2719 行，最重一批）。MODE=dead：忠實翻譯 + 只加 CMake、**不動 dispatch ladder / extern / stub**。
- **dead-by-design 證明**：6 個 `DoOutArm_9045_<deadv>` 全 nm `T`（帶完整 `_<deadv>` 後綴=唯一、永不被呼叫），engine aoutarm9045.cpp 對它們 **零 reference**（grep 證明、不在任何 ladder）。enum 對照：`e9045_1x4_4_Back=10`→空 body+1x4_4 pick、`e9045_2x2_4_14=16`→live 2x2_4、`e9045_2x8_32=25`→live 2x8_8；`2x2_4_23`/`2x4_16`/`2x8_16` enum 根本不在 MachineType.h（compile-time 可證 dead）。
- **這批是「forked 全 place-to-auto」碼**（不像 20 個 live 變體把放料走 engine），故引用大量尚無 offline home 的符號（iOutArmPlaceOrder/fProductionInfo->CalTrayICCount/OutputForm/FixForm/OutArmContinuousMove 8-arg/AutoCalculateOutArmClosePitch/CheckUseFixBinBoxFunction/XPHNotStander…）。translate agents 以 `#ifndef *_FWD_GUARD` forward-decl 全包；**integrate 實測 substrate 零新增**（20 live 變體+engine 已把所需 offline surface 帶進樹、forward-decl 在 link 期解析）。sibling cross-call 自然解析（2x2_4_14→live MoveOutArmToShuttleIncludeZ_9045_2x2_4；2x8_32 自持唯一 3-param 版）。
- **誠實註記（非 build blocker）**：dead `.obj` 帶 by-design 無法離線滿足的 cross-module/UI externs（~28 個），GNU ld **只在符號被引用時才拉 archive member**——live 引擎+20 live 變體從不 name 任何 dead 符號，故 dead `.obj` 永不被拉、link 乾淨。把 dead helper 從 test force-link 反而會拉進 .obj 暴露那 28 個未解析 extern（誤報正確的 dead weight 為壞）→ verify agent 正確 revert，改以 nm(defined)+grep(never dispatched) 證 deadness（同 in-arm 2x8_32 前例）。oracle（GetNowShuttleMode_2x4_16→(iSht==0)?102:202、_2x8_32→0）以原始碼對讀確認、未 runtime assert。
- **驗證（workflow clean-from-scratch + 本 session 獨立複驗 `build_oa_b4`）**：**ctest 41/41 PASS（0 fail）**；6 dead .obj 全編入 lib；唯一 shared 改動=CMakeLists.txt（+ test 檔加 dead-mode 文件註解）；aoutarm9045.cpp **未動**；directive 全平衡（4/4/8/5/55/77）；14 檔 **mojibake 0**。
- **下一步**：B5（S-prefix `aoutarm9045S_1x4_4`，dead-but-compiled、不規則命名，已備專用 mini-workflow `w62_outarm_Sprefix.js`）→ 收尾 27 檔 → **W7**。

## 2026-06-29 — W6.2c-OUT batch-5（FINAL，S-prefix dead-but-compiled）：🏁 out-arm 27/27 收尾
- **B5（verify-and-commit；翻譯+CMake 整合在前 session 已落盤、未提交未驗）**：最後 1 檔 `aoutarm9045S_1x4_4.cpp`(golden ~2169 行/116KB)+`.h`。S-prefix dead-but-compiled——export `DoOutArm_9045S_1x4_4()` 及全部 `_9045S_1x4_4` infix helper 定義但**永不被任何 ladder 呼叫**（out-arm dispatch 走 suffix-S `DoOutArm_9045_1x4_4S`，定義於已提交的另一檔 aoutarm9045_1x4_4S.cpp）。
- **本 session 未重翻**：依鐵律「不信任未驗證/未提交波次」改走 verify-only workflow（前景 build、禁背景 poll、全新 build dir `build_b5_verify` 避 busy-handle race），agent 回報無需任何修補（已綠、filesTouched 空）。
- **獨立複驗（主迴圈親跑，非僅信 workflow 自報）**：clean `build_b5_verify` 100%、**ctest 41/41 PASS（0 fail，維持 B4 baseline）**；**nm dead-symbol witness**：`__Z20DoOutArm_9045S_1x4_4v` = `T`(定義於 aoutarm9045S_1x4_4.o)、engine aoutarm9045.o 對它 **0 reference**（objdump 證）；live suffix-S `__Z20DoOutArm_9045_1x4_4Sv` engine 帶 `U`、定義為 `T`(aoutarm9045_1x4_4S.o)→ 正確 live routing。directive cpp 4/4、h 1/1。mojibake 0/0。
- **🏁 里程碑：OUT-ARM 變體 fan-out 27/27 全清**（20 LIVE + 6 DEAD + 1 S-prefix DEAD）。**in/out 兩臂 site-variant fan-out 全部完成**（in-arm 27 + out-arm 27）。
- **下一步＝W7**：解 csystem/atester 等 gated 大宗（MainProc mode/SECS/AGV/temp ladder ~2000 行、DoOneCycleFinishCheck/DoCleanOutFinishCheck ~2900 行、cContact 22761 行 index SM、W5 comms ~22k LOC）；皆需接真實/半真實 HAL 端到端 pump。見 KNOWLEDGE「工具導入評估決策」：真風險在覆蓋率/驗證深度（此處）、非導航效率，故 CodeGraph/Superpowers 暫不導入。

## 2026-06-29 — W7 sub-wave A1（首個 live W7 覆蓋）：in-arm shuttle floating/latch pair un-gate+pump
- **流程**：W7 recon（4 平行調查+synth，排序 candidate）→ A1 translate（bodies only）→ serial-integrate（facade+shims+test）→ clean-verify → **主迴圈獨立複驗** → commit。
- **un-gate 兩個原 `#if 0` whole-body stub**：`CheckInShuttleSensor_Latch`(golden ainarm9045.cpp:3810-4174,~365 行)+`DoInArmCheckShuttleFloating`(golden :3349-3757,~409 行)，忠實譯回真本體（cp950-aware、0 U+FFFD；cursor/switch/goto/fall-through/SOFT_SIMULTE 全 verbatim）。
- **可 pump 關鍵（motion-free，這就是 recon 選它當第一刀的理由）**：latch 以 verbatim entry guard `if(MOTION_CARD_TYPE!=MotionCard_Contec || LastSet.iRealDummy!=REALLY) return 1;`（ainarm9045.cpp:1869-1871）在 sim canary(iRealDummy==DUMMY)下自我關閉→回 1(Finish)、零 motion/CCLink/Ltc 依賴；floating 因此收斂 **1→1301→3000→9000→(Task=1,bResult=true)**，case 9000 side-effects(IndexZCanMove[]=true、bShuttleKnock=false、bNo9Action true→false)。**真 terminal 由真實 predicate 讀取驅動，非 finish-check 的 false instant-finish**。
- **接縫（小、link-only/inert）**：FormsFacade 加 1 個 bool `cInplace->bNo9Action`(default false,golden-faithful、grep 確認僅 ainarm9045 引用)；新 in-arm shims TU `ainarm9045_w7_shims.{h,cpp}`（DoShakeShuttle/DoKnockShuttle/DoKnockShuttleFirst/DoVibrateShuttle→no-op true、RecordShtSuperflous→no-op、IndexZCanMove[2]/bShuttleKnock 全域）。integrate 另抓到 translator **漏報**的 link gap `InitDoInShZHome`/`DoInShZHome`(csystem.h:296-297,真本體 W7-gated)→同 shims TU 補 faithful 離線 stub。ODR：IsTestZ1/Z2NotSafeShuttleNCanNotMove 真本體在 acarry.cpp、僅 extern forward-decl 不重定義。
- **獨立複驗（主迴圈親跑 fresh dir `build_w7a1_indep`，非僅信 workflow 自報）**：configure/build exit 0、51 targets 100%、0 error/undefined-ref；**ctest 42/42 PASS（0 fail，新增 W7_A1_InArmFloatingLatch；W6.6 hub 64-tick A/B regression 仍綠）**。oracle 19/19：latch self-guard 回 1 不動 MOT/Ltc、floating 收斂 9000、clean empty-grid 路徑無 spurious alarm（latch 為唯一 alarm carrier、自我關閉→JAM04xx 離線不可達）。directive ainarm9045.cpp 25/25＋各檔平衡、mojibake 0/0（全 8 檔，主迴圈 byte-level 複查）。
- **⚠ 誠實覆蓋註記（DEFERRED，勿當全譯）**：(1) **No9 placement recycle 子路徑**(floating case 2000/2100 的 `bRunNo9`/`DoSuckToRecycleFromShuttle`)仍 `#ifdef W7A1_NO9_MEMBERS_PENDING` gated（5 處）——離線 `InArmPlacementEnable()==false` 本就 dead、行為等價，待 cInArmPlacement 真 form/W7 還原。(2) `DoInShZHome` + 4× shuttle-motion engine 為離線 stub，待 shuttle-motion 引擎 W7 翻譯時還原真本體。
- **下一步（recon 排序，皆 A1 不需要）**：**S0**（MotorMove→Sim 收斂 enabler，獨立小 PR＋專屬 motor ctest，守 Motor==NULL fast-path 不回歸）→ **grid-occupancy staging**（HT9045_KITSUCK_GRID_AVAILABLE seedable，先加 accessor 再 flip）→ **C1**（csystem DoCleanOutFinishCheck end-of-lot drain）。UI-framework fork 續延 2-3 波（A1/S0/C1 皆不跨真 form）。

## 2026-06-29 — W7 enabler S0：offline 馬達 move family 收斂（解鎖 motion-bearing 主路徑）
- **S0（非覆蓋 slice，是 enabler）**：給 `Motor/mymotor.cpp` 的 offline（`Motor==NULL || !Motor->Enable`）move family 加「瞬間收斂捷徑」（`Position←target`、回 success），讓 motion-bearing SM 能在 Sim HAL 上抵達 arrival terminal。4 函式各加一段守在既有 gated real-driver TODO **之前**的 guard：`MotorMovePosition`(Position=Tar;return 1)、`MotorMove`(Position=p;refresh ScreenPos;fCMD=false;return 1)、`MotorMove2SpeedForPicker`(Position=FinalPos;return true)、`MotorMoveShuttleShake`(Position=p;return true)。success code 對照 golden BCB6 mymotor.cpp:821-823/953-956/999。real-driver 路徑、`ReadPos`/`MotorHome` offline 收斂、atester `Gali_*/ISNormal/GalilTwoY` 一行式**全未動**（diff 限 mymotor.cpp:356-389 單一 hunk）。
- **獨立複驗（主迴圈親跑 fresh dir `build_w7s0_indep`）**：configure/build exit 0、0 error/undefined-ref；**全套 ctest 43/43 PASS（42→43，新增 W7_S0_MotorConvergence 18/18）**。**全域變更 headline 把關通過**：42 個既有 suite（全 W6 引擎/W6.6 hub/W7_A1/motor_w4/sim_motor）**結果無一改變**——瞬間收斂正是 golden arrival terminal 的正解、guard 嚴格限 offline 故未過度放大。fast-path 不回歸（Gali_*/ISNormal/GalilTwoY 離線仍 true）。directive 0/0（純 runtime guard、無 preprocessor）、mojibake 0（全 3 檔，主迴圈 byte-level 複查）。
- **⚠ 非阻斷觀察（pre-existing，未修）**：`TMyMotor` ctor（mymotor.h:139 / ctor :115-184）未初始化 raw `HTMotor* Motor` 指標——production 由 cinitial 一律指派而被遮蔽；offline 測試以建構後 `mot.Motor=NULL` 規避。S0 範圍不動 mymotor.cpp 既有邏輯故留待後續（建議併入 ctor 初始化清理）。
- **下一步（recon 排序）**：**grid-occupancy staging**（`HT9045_KITSUCK_GRID_AVAILABLE` 先加 seedable accessor／驗 predicate 仍 link 再 flip）→ **C1**（csystem `DoCleanOutFinishCheck` end-of-lot drain）。**T1**（atester press 下壓 SM）現 S0 落地後已具 motion 收斂前提，可於 C1 後或視需要提前。UI-framework fork 續延。

## 2026-06-29 — W7 substrate staging：KitSuck occupancy grid 上線（解鎖 C1 真 drain）
- **staging（非覆蓋 slice，是 enabler）**：把 `aHotPlateSubstrate.cpp` 兩個 stub leaf `UseSiteHasIC`/`UseSiteNoIC` 換成 golden faithful Item-grid 掃描（`for i<iShtRow,j<iShtCol: if Item!=NULL_IC`，golden MyKitSuck.cpp:288/297 逐字），並把 `csystem_predicates.cpp` 的 `HT9045_KITSUCK_GRID_AVAILABLE 0→1`——cross-arm HasIC predicate 家族（InputShuttle*/OutputShuttle*/SortShuttle/TestHead*/TestSocket/IndexHasRealIC）改查真實可播種 Item grid。flag 一翻需補 2 個 include（`aHotPlateSubstrate.h`+`acarry_shims.h`）供 kit 全域 extern decl（純宣告、predicate body 全未動）。
- **零回歸不變式（關鍵）**：`NULL_IC==0`（cmydef.cpp:153）+ kit 全域 zero-init → 未播種 grid 全 NULL_IC → faithful `UseSiteHasIC()`==false（＝舊 stub）、`UseSiteNoIC()`==true（＝舊 stub）。故 flag flip 對任何未播種 grid **行為完全等同**，43 既有 suite 依建構保持綠；唯一新增＝播種 `Item=HAS_IC(2)` 後 predicate 才翻 true。不需動 ctor。
- **獨立複驗（主迴圈親跑 fresh dir `build_w7grid_indep`，並親檢 diff）**：configure/build exit 0、0 error/undefined-ref；**全套 ctest 44/44 PASS（43→44，新增 W7_GridOccupancy 21/21）**。**全域 flag flip headline 把關通過**：43 個既有 suite 結果無一改變。新 oracle 證：leaf empty→false／seed→true／clear→false，且**端到端 flag-flip 證明**——播種全域 `FLCarryKit.Item` use-site → `InputShuttleFrontHasIC()`/`ShuttleHasIC()`==true；播種 `FTestSuck` → `IndexHasRealIC()`==true；清空/還原→false（W7_GridOccupancy save/restore 全域，不汙染他 suite）。directive csystem_predicates 10/10、aHotPlateSubstrate 3/3；mojibake 0（全 4 檔）。
- **下一步（recon 排序）**：**C1**（csystem `DoCleanOutFinishCheck` end-of-lot drain，golden csystem.cpp:14713-15748 ~1035 行）——現播種能力到位，可 un-gate 並以「播種 occupancy → pump 至清空 terminal」做**真 drain** 驗證（非 false instant-finish）。其後 C2（DoOneCycleFinishCheck）／T1（atester press）。UI-framework fork 續延。

## 2026-06-29 — W7 C1：DoCleanOutFinishCheck end-of-lot drain（首條「真正做事」主路徑覆蓋）
- **C1（translate→serial-integrate→clean-verify→主迴圈獨立複驗）**：把 csystem `DoCleanOutFinishCheck`（golden csystem.cpp:14713-15748 ~1035 行）真本體譯進 `csystem.cpp:1103`，移除 csystem_shims no-op stub（ODR：nm 確認單一 `T DoCleanOutFinishCheck`）。cursor＝`iCleanOutCycleTask`（37 transitions），entry guard `if(iOneCycle){} else if(iCleanOut==1){…}` 逐字。
- **真 drain 證明（grid-staging 的回報，本波 deliverable）**：W7_C1_CleanOutFinish 7/7——(a) empty(iCleanOut=1/IndexStatus=Z1_Z2_Normal、kit 清空)→cursor=5（往 index-home drain step 走，**非** false instant-finish）；(b) **播種殘料**(InArmSuck use-site=HAS_IC)→`iCleanOutCycleTask==1001` 且 iCleanOut 維持 1（**不 finish**——舊 no-op stub 永遠做不到的新能力）；(c) 清空→cursor=5（drain 繼續）。
- **接縫（integrate 補）**：FormsFacade 加 `fMain->Start(AnsiString)`+ChangeLevelAttr/ModifyTester/CleanYieldCount/BtnOneCycle（離線 no-op/false）。integrate 另抓到 translator 誤列為「present」的 4 個 declared-only 符號並**忠實補譯**：`AllArmZIsSafe`(golden csystem.cpp:343，含 SOFT_SIMULTE return true + ep16Picker selector)、`CheckIndexIsNormal`(golden :12676，MTestZ1.MovFlag double-OR typo verbatim) 進 csystem_predicates.cpp、`hAutoCleanHangUp` timer 定義一次、`ReadTestMode` TU-local no-op(僅 offline-false 分支呼叫)。
- **獨立複驗（主迴圈親跑 fresh dir `build_w7c1_indep` + 親檢 diff）**：configure/build exit 0、0 error/undefined/multiple-def；**全套 ctest 45/45 PASS（44→45）**；ODR nm 單一 def。**no-regression 把關**：entry guard 使 body 在 iCleanOut≠1 時 inert → W6.6 hub（不設 iCleanOut）+ 全 44 既有 suite 結果無一改變、MainProc pump byte-identical。directive 全平衡、mojibake 0（全 9 檔）。
- **⚠ 誠實覆蓋（DEFERRED，本波最大宗）**：C1 **核心 drain 路徑 active 且已驗**，但深層 config-gated customer/mode 分支（離線 inert）經 **155 個 W7C1_ TU-local seam** 接住，真本體待各自波次還原：SCKART AutoRetest 欄位/方法(→Automation/SCK_ART)、fOCR/fContact/fTrayMapping/fBarCode form 呼叫(→W7-UI)、fAGV IsSPIL_AMR/AMR tray-count(→AMR 波)、SECS_EVENT.CleanOutFinish/AGVSupplement(→W5 SECS)、QA/Low-Yield/tray-feed finish ladder、LastSet 4 欄位、多個 free func/global/object stub。見 ROADMAP DEFERRED「C1」列。
- **下一步**：C2（`DoOneCycleFinishCheck` ~1899 行，同 predicate-driven，重用 fMain->Start/SCKART seam，但多 fHome/fSocketCommunication）或 T1（atester press 下壓 SM，S0 已備 motion 收斂前提）。UI-framework fork 續延。

## 2026-07-01 — W7 C2：DoOneCycleFinishCheck + DoART_AfterCleanOut（One-Cycle 收尾 SM 真本體）
- **C2（translate→serial-integrate→clean-verify→主迴圈獨立複驗）**：golden csystem.cpp:12813-14712 實含**兩支**函式——`DoOneCycleFinishCheck`(12813-14047,1235 行,cursor iOneCycleTask) + `DoART_AfterCleanOut`(14049-14711,663 行)——皆忠實譯進 csystem.cpp（cp950→UTF-8 via iconv，行數逐一對拍 1235→1235 / 663→663；29+14 個 return 全保留；27 個 iOneCycleTask transition(1..21,1003,0) verbatim；2 個 SOFT_SIMULTE 區塊本就是 golden 註解態，原樣重現）。移除 csystem_shims 的 `DoOneCycleFinishCheck` no-op stub、decl 移入 csystem.h（ODR：nm 單一 T）。`DoART_AfterCleanOut` 由 C1 的 W7C1 inert 別名 rewire 到真本體（刪 #undef+seam alias、decl 入 csystem.h）。
- **真行為 oracle（W7_C2_OneCycleFinish 7/7）**：設全域 iOneCycle=1 進 active path——(a) 空機→cursor≠5(=9)；(b) 播種 InArmSuck 殘料→`iOneCycleTask==5`(golden 12933 residual wait gate)且 iOneCycle 維持 1（**不 false-finish**，golden 13931 不可達）；(c) 清空→cursor≠5；(d) iOneCycle=0→sentinel 不動（BtnOneCycle->Down 離線 false，golden 14025 else 分支）。save/restore 全域不汙染他 suite。
- **接縫（W7C2 seam block，mirror W7C1）**：30 個離線 inert stand-in——fSCKART/hanaART/RENESAS_Server 擴充成員、slLowYieldAlarm/cbRunStartMode/Label12/SetLotState 等 fMain、fSocketCommunication/fHome/fAutoTeach/fContact 等 form、SECS_EVENT.OneCycleFinish/ArtFTFinish/ArtRTFinish、LastSet/LotSummary 擴充、BLCarryKit.SetHasNullIcToNullIc。integrate 另補 6 個 translator 漏報的 link gap（DoSiteMappingResult/ReadWriteBinCountMode/AutoTrayCylinderFree/EmptySocketCheckModeBeUse/WriteLastDataFile/iClearSocketFunctionTask——皆 header-decl 但 body `#if 0`-gated、僅 csystem.cpp 引用、位於 finish terminal 或離線-false AutoSiteMap 子路徑）。
- **獨立複驗（主迴圈親跑 fresh dir build_w7c2_indep + 親檢 diff）**：configure/build exit 0、0 error/undef；**全套 ctest 46/46 PASS（45→46）**；45 既有 suite 無一改變（no-regression：body gated 於 iOneCycle，hub/suite 從不設 1；DoART 的 ART 分支 bAutoReTest_ART/bUseSCKART 離線 false 不可達）；ODR nm 單一 T×2、stub 消失；mojibake 0（全 6 檔）；directive 真實平衡（raw grep 16/17 是 SOFT_SIMULTE #2 註解態 `/* #ifdef … #endif*/` 造成的 artifact，編譯器層平衡由 clean build 證）。3 個 transition 對 golden 逐行 spot-check verbatim（iOneCycleTask=8/1003/21）。
- **⚠ 誠實覆蓋（DEFERRED）**：C2 **核心 One-Cycle finish + ART-after-cleanout drain 路徑 active 且已驗**，深層 config-gated customer/mode 分支（SCKART AutoRetest 全套、RENESAS/HANA-ART、SECS EventReport、Low-Yield/QA/Magazine/tray-feed、fSocketCommunication ASE_KR）經 30 個 W7C2 seam 接住離線 inert，真本體待各自波次（Automation/SCK_ART、uRENESAS/uHANA、W5 SECS、W7-UI）還原。見 ROADMAP DEFERRED「C2✅」列。
- **下一步**：T1（atester press 下壓 SM，S0 已備 motion 收斂前提，golden 6232-8655 case 樹）；或 MainProc mode/SECS/AGV/temp ladder（~2000 行，需 W5+home）。UI-framework fork 續延 2-3 波。

## 2026-07-06 — W7 T1：DoTestHeadMotor 下壓/test/torque/EP case 樹 un-gate（接手前 session 未提交波次）
- **接手脈絡**：本 session 開場時工作樹已有 6 檔未提交、未驗證的改動（atester.cpp/.h、atester_shims.cpp/.h、canary_support.h、tests/test_w6_4_tester.cpp，帶 `AI(W7T1-Translate)`/`AI(W7T1-Integrate) 20260701` 註解）——前一 session 已完成 T1 translate+integrate 但未跑複驗、未 commit。依鐵律「不信任未驗證/未提交波次」，本 session 先獨立複驗再決定提交。
- **T1 範圍**：`DoTestHeadMotor()` 內原 `#if 0` 的下壓/test/torque/EP/load-cell 密集 case 樹（golden atester.cpp:6229-8650，case 30..600/12000../14000../122100../142../1500../10000../20000../30000../40000../50000../60000../80000..）忠實譯入，取代原 default-park stub。107 個 case label（含 `iCASE_REAL_CCD2..6`）逐一保留、`break` 113/113、SOFT_SIMULTE directive 6/13 與 golden 一致。un-gate 邊界緊接前一 gate(`:6055-6167`)與後一 gate(`:8656-8684`)，無縫隙無重疊。
- **一個真正的 ODR/collision 修正（前 session integrate 抓到）**：W6.4 波曾在 `atester_shims.cpp` 暫時把 `iCASE_REAL_CCD2` 定義為 `9`（借用未 gated 的 plain index-check case 值，當時 RTC 樹整段 gated 故安全）。T1 un-gate 後 `case 9:` 變成真正 ACTIVE 分支，若沿用 `=9` 會與其相撞；本波把 `iCASE_REAL_CCD2..6` 的真正定義移回 golden file-scope 位置（atester.cpp，值 40200/40300/40400/40500/40510，golden :5551-5555），並移除 shims 的暫時定義+extern。`bool IndexEveryTimeCheckEP()` 簽名同步復原（golden case 30000 `if(IndexEveryTimeCheckEP())` 需要 bool，W6.4 誤譯成 void wrapper）。`canary_support.h` 補 2 個 LastSet 欄位（`bD41TestSocketICCheckSkip`/`iD41SocketInitialICCheckPosition`，golden LastSet.h:132/:274，型別逐字對拍）供新 active 樹解引用。
- **獨立複驗（主迴圈親跑 fresh dir `build_w7t1_indep`）**：configure/build exit 0、0 error/undefined-ref；**全套 ctest 46/46 PASS（維持 C2 baseline，無新增 test，因 T1 是既有 W6_4_Tester 覆蓋範圍擴大而非新 SM）**；mojibake 0（全 6 檔）；atester.cpp directive 57/57 平衡、atester.h 1/1；nm 確認 atester.cpp.obj 與 atester_shims.cpp.obj 無重複 `T` 符號（無 ODR）。
- **獨立 fidelity review（另一 agent，逐行對 golden 交叉驗證）**：全綠、zero discrepancies——107 case label 順序/數值全同、break 計數 113/113 相同、SOFT_SIMULTE directive 計數相同；`myswitch.h`(`SW[SwIndexChangeToque1/2].Off()`，case 12000/12200/14000/14200)、`vcl_compat.h` `AnsiString(int)` ctor（`labUser->Caption`，case 1675）用法皆與 golden 逐字相符；`iCASE_REAL_CCD2..6`/`IndexEveryTimeCheckEP`/canary_support 三處修正皆對 golden 核實無誤。
- **誠實覆蓋（未變，仍 DEFERRED）**：`GetTesterResult`/`ProcessTestResult`/`ProcessTesterTimeOut`（tester-comm，W5/W7）、`iIndexTask@cContact` 下壓交握（cContact.cpp 22761 行 VCL form 整檔未翻，W7）、torque(COM2)/EP-DAQ/RTC-vision/cContact-ROI/torque-UI 等經既有 atester_shims 離線 stand-in（本波未新增 stand-in 類別，僅補 2 個 canary_support 欄位）——皆 T1 case 樹內部呼叫的深層 cross-module 本體，隨各自子系統波次還原。
- **下一步**：MainProc mode/SECS/AGV/temp dispatch ladder（golden csystem.cpp:16730-19101，~2000+ 行，需 W5 SECS + home）；或 W6.4b（atester_32Site/ProcessCount/Front/Rear sibling + cContact iIndexTask 下壓 SM，22761 行 VCL form）；或 W6.2b in/out site variant 收尾檢查（已 100% 完成，僅需 spot audit）。UI-framework fork 續延。

## 2026-07-06 — W6.4b recon + Wave 1：atester_Front/Rear「Destroy IC」下壓 SM 對，首個 sibling 檔真實覆蓋
- **recon（5 平行調查 + synth，workflow）**：對 5 檔（`atester_32Site.cpp`4749行/`atester_ProcessCount.cpp`2161行/`aTester_Front.cpp`8580行/`aTester_Rear.cpp`9787行/`cContact.cpp`22761行）+ 既有樹 blast-radius 做完整盤點。**重大修正 ROADMAP 既有錯誤標籤**：`iIndexTask@cContact` 這個符號**根本不存在**——真正的下壓 cursor 是 `TfContact::DoTestContactFunction`（cpp:11755-13963,2209行,~78 case）內的 `CarlibrationTask`（cContact.h:556），加上 6 個私有 sub-SM helper（`DoZ1/Z2PickFromShuttle`/`Do_Z1/Z2_AutoGetHeight`/`Do_ContactTest_32Site`/`DoZPlaceToShuttle`），總計 ~11347 行≈整檔 50%。VCL widget 存取**穿插在 switch-case 內部**（非僅入口/出口），比 atester.cpp 的 T1 case 樹更破碎，非本波範圍（全部 DEFERRED 到獨立 W7 波）。
- **synthesis 排序**：Front+Rear Destroy 對（已有真實非-shim 呼叫點、依賴集在既有樹 100% 滿足、雙檔耦合但範圍小）→ cContact 純算對 `GetMinForce`/`GetMaxIndexForceLimit`（下一波候選）→ ProcessCount 葉節點 → Front/Rear Suck 對 → atester_32Site（延後，85%+ 卡在完全未翻的 `COM2`/RTC-CCD serial 物件）→ cContact 下壓叢集整塊延後全 W7。主迴圈獨立驗證 synthesis 的 call-graph 主張（grep 精確比對 `DoFrontTestDestroyIC`/`DoRearTestDestroyIC` 呼叫點）與 recon 結論逐字相符。
- **Wave 1（workflow：2 平行 translate + 1 序列 integrate）**：新增 `aTester_Front.cpp`(835行)/`.h` + `aTester_Rear.cpp`/`.h`，忠實翻譯 `DoFrontTestDestroyIC(bool)`(golden :309-865)/`DoRearTestDestroyIC(bool)`(golden :309-886) 下壓/毀壞 IC SM + 6 個小葉依賴（`CheckZ1/2IsDown`/`TestZ1/2OutRandge`/`TestZ1/2SetPos`/`ResetArm1/2PutFlag`/`Init*TestDestroyICTask`）。兩個平行 translate agent **各自獨立**發現同一組 ~9 個 substrate 缺口（`TMyKitSuck::bNeedCheck`/`MoveSuckData`/`SetHasNullIcToNullIc`、`TMyProductionRecord::AddIndex*Record`、`TMySucker` off-delay 計時欄位、`LastSet.iIndexInputOutPut[]`…），逐一用 golden-cited `#if 0 // TODO(W7)` gate（非近似值），互相佐證翻譯正確性。
- **integrate 抓到 2 個真正 undefined-reference cross-module 缺口**（非本波規劃內，翻譯期才浮現）：`OutSHT1InLF`/`OutSHT2InLF`(golden csystem.cpp:698/700，一行 delegate) + `CheckIndexAllSuckICFallDown`(golden csystem.cpp:2417,~40行 drop-detect scan) 補進 `csystem_predicates.cpp`（忠實翻譯非 gate）；`atester_shims.cpp` 新增 `GetSiteCount(bool)`（golden cprod.cpp:305，真本體困在 cprod.cpp 整檔 `#if 0` 區，比照既有 `IsInArmOneCycleFinish`/`IsInArmCleanOutFinish` 前例做 faithful duplicate）。`atester_shims.h/.cpp` 移除 12 個被取代的 stub 宣告/定義（6 symbol × Front/Rear）。`atester.cpp` 加 2 個 `#include`（鏡射 golden atester.cpp:7-8）。
- **獨立複驗（主迴圈親跑 fresh dir，兩輪）**：第一輪 `build_w64b_indep`：configure/build exit 0、**ctest 47/47 中 1 個新測試 FAIL**（`W6_4b_FrontRearDestroy`，17 斷言中 1 個 precondition 斷言失敗）。**根因非翻譯缺陷、是測試自身撰寫錯誤**：`TMySimMotor` 建構子把 `Enable` 設 `true`("sim motor is always enabled"，mySimMotor.cpp:17/30)，蓋過 `HTMotor` 基底類別的預設 `false`（HTMotor.cpp:41）；測試假設沿用基底預設，實際上 `CheckZ1/2IsDown()==true` 斷言雖仍數值通過，卻是繞道另一個巧合分支（`Gali_ReadEncoderPos()==0<=Prod.iTestDown_Z1==0`）而非測試文件宣稱要驗證的 `Motor->Enable==false` early-out 分支。**主迴圈親自修正**：測試裡 attach sim motor 後明確 `.Enable=false` 覆寫，讓斷言真正走到意圖驗證的分支。第二輪 `build_w64b_final`（修正後全新 clean-from-scratch）：**ctest 47/47 全綠（46 既有 + 1 新，無回歸）**；mojibake 0（全 9 觸及/新增檔）；directive 平衡（Front 16/16、Rear 11/11）。
- **獨立 fidelity review（另一 agent，逐 case 對 golden cp950 交叉驗證）**：全綠、zero discrepancies。特別驗證：golden 自身的 case-100 `#ifdef DEBUG_INDEX_UPH` 死碼括號缺陷（Front :354/Rear :352）原樣保留未「修正」；golden 本身的 Front/Rear **不對稱**（Rear case 500 讀 `Prod.TestZ1_Safe` 而非 `TestZ2_Safe`——真實 golden quirk；`BRCarryKit.SetHasNullIcToNullIc()` 只在 Rear 呼叫、Front 對應分支沒有）皆忠實重現、未被「同質化」修掉。`ResetArm2PutFlag` 正確迴圈 `BTestSuck`(非 `FTestSuck`)，對應主 session 交辦時特別標記的驗證點。ODR 稽核：10 個 symbol 各僅 1 個定義+1 個宣告。僅 1 項無關痛癢風格差異（Front.h 全部 `extern` 前綴、Rear.h 省略，兩者合法等價）。
- **下一步**：cContact `GetMinForce`(cContact.h:609,cpp:18957-19022,66行) + `GetMaxIndexForceLimit`(18893-18955,63行) 純算對（比照 ContactForce extract-calc-core 前例，零 motion/VCL 接觸）；或 `atester_ProcessCount.cpp` 易翻葉節點(`ProcessSocketPurgeCount`/`GetMultiplierNum`/`ProcessAutoloadcellMeasureCount`，零新 shim 面)；或 Front/Rear Suck 對（`DoFrontTestSuckIC`/`DoRearTestSuckIC`，~1350/1313 行，recon 確認範圍內無 fContact 接觸）。**cContact 的 `DoTestContactFunction`+6-helper 下壓叢集(~11347行)不排進 W6.4b，需獨立 W7 波**（VCL widget 穿插 switch-case 內部，需比 T1 更細碎的逐處 gate）。`atester_32Site.cpp` 亦延後（需 COM2/RTC-CCD 獨立波）。使用者已指示每波做完暫停回報，下一波需使用者明確再叫續做。

## 2026-07-10 — W6.4b 續：cContact GetMinForce/GetMaxIndexForceLimit（Contact Force 最小/最大力量純算對）完成
- **範圍**：比照 ContactForce extract-calc-core 前例，抽出 `TfContact::GetMaxIndexForceLimit()`(golden cContact.cpp:18893-18955) + `TfContact::GetMinForce(double dKitDiameter,int iTag)`(golden :18957-19022) 兩個純算函式，轉成自由函式 `ComputeMaxIndexForceLimit`/`ComputeMinForce`（新增 `cContact.h`/`cContact.cpp`，鏡射 golden root 路徑；`CalculateTotalAirForce`(:18675-18891，VCL widget 密集)與 cContact.cpp 其餘 22761 行整體本波不翻，留給獨立 W7 波，見 ROADMAP DEFERRED）。
- **extract-calc-core 設計**：`GetMaxIndexForceLimit` 原讀一個 VCL widget(`rgKitDiameter->ItemIndex`)，抽成 int 參數；`GetMinForce` 原讀 `fContactForce->SLKClass[iTag]->dContactOffset[_NS]`(TfContactForce 尚未翻譯的 VCL 實例)，抽成兩個已解析 double 純量參數。兩者皆重用既有已翻譯的 `MachineType.h`(eIndexPressType/eTestMode) + `ContactForce.h`(SlkForceData 欄位命名對照)，零新增 vclcompat/ht9045_globals link 依賴（cContact.cpp 只 `#include "MachineType.h"`，經實測編譯確認純 header-only，`ht9045_core` 不需再連 `ht9045_globals`）。
- **忠實保留 2 個 golden 缺口**：(1) `GetMaxIndexForceLimit` 內層 `ItemIndex==1→55/==2→85` 無 trailing else(golden :18940-18948)——其他 ItemIndex 值讓 dMaxLimit 停在初始值 0.0，非 85，翻譯**未**加預設分支「修正」它；(2) golden `dMinForce` 是 TfContact member(mutate+return)，翻譯改純區域變數回傳——已逐一 grep 全樹確認 cContact.cpp 內唯一呼叫點(:18763 `CalculateTotalAirForce`)只是把回傳值指派回同一 member，故對該呼叫點行為 100% 等價；member 在其他呼叫點的持久狀態語意(:5650/:8414 `ADAM_WriteVoltage(dMinForce/2.0)`，屬未翻的 Z1/Z2 AutoGetHeight)明確記為 DEFERRED，留給未來完整 TfContact 翻譯波次接回。
- **推理過程修正一則 scoping 提示**：規劃階段提示提到「dMinForce 另在 `AutoClean/uCleaning.cpp:1741` 被讀」，經 agent 與主迴圈各自 grep golden 樹核實：uCleaning.cpp 完全沒有 `dMinForce`，該行實際呼叫的是 `fContact->GetMaxIndexForceLimit()`(不同函式，無 dMinForce 牽涉)——GOLDEN WINS，該引用已從 cContact.h 的 DEFERRED 註解中修正，不視為翻譯缺陷。
- **CMake**：`cContact.cpp` 併入既有 `ht9045_core`(ContactForce.cpp 同組)；新增一則有理由的 `-Wno-pedantic`(僅此檔)——起因是 `MachineType.h:1607` 既有一處 BCB6 保留的 template 函式後贅餘 `;`，在 `-Wpedantic` 下觸發「extra ';'」，cContact.cpp 是 ht9045_core 內第一個 `#include MachineType.h` 的檔案才首次踩到；已比照 HTMD5.cpp/ht9045_globals 前例的「窄範圍抑制單一 TU、其餘警告類別(`-Wall -Wextra -Wshadow -Wconversion`)全開」慣例處理，非蓋掉本檔自己的問題（主迴圈已獨立核對 MachineType.h:1601-1607 確認警報來源屬實）。
- **驗證（主迴圈親跑，兩輪 fresh from-scratch，非僅信 agent 自報）**：
  - 先在**未套用本波 CMake 變更**(git stash 只還原 CMakeLists.txt，cContact.cpp/.h 因此不會被建置)的乾淨樹上單獨 build+ctest，得到 **44/47 PASS、`config_db`/`IniFiles`/`ini_helpers` 3 個測試已經失敗**——證實這 3 個失敗與本波無關，是既有環境漂移（grep 現場 `system/Gerneral.ini` 確認：`IO_CARD_TYPE`已從文件記載的 2 變 1、`TTL_CARD_TYPE`從 2 變 0、`HEATER_CTRL_TYPE`從 4 變 2，是這台機器的 config 檔隨時間被現場調整，測試斷言的是舊快照值）。
  - 還原 stash 套用完整本波變更後，第二輪獨立 fresh build：**ctest 45/48 PASS（新增 `cContact` 套件 100% 過、其餘 44 條全綠、同一組 3 個既有失敗不變、無新回歸）**；`cContact.cpp.obj`/`test_cContact.cpp.obj` 編譯零警告；U+FFFD 逐檔核對(`cContact.h`/`.cpp`/`test_cContact.cpp`/兩個 CMakeLists.txt)皆 0。
  - 獨立 fidelity-review agent（未見翻譯過程，重新對 golden cp950 解碼比對）：8 項檢核(分支/常數逐一相符、內層無 else 缺口保留、外層三值 AND 保留、KYEC 28/58 別名分組保留、單向 max-override 方向未反、bNSKitPress 選擇未調換、零殘留 TfContact/fContactForce/rgKitDiameter 依賴、U+FFFD=0)全數 PASS，零差異。
- **下一步（W6.4b 續，同一份候選清單，使用者需明確再叫續做）**：`atester_ProcessCount.cpp` 易翻葉節點(ProcessSocketPurgeCount/GetMultiplierNum/ProcessAutoloadcellMeasureCount，零新 shim 面)；或 Front/Rear Suck 對(`DoFrontTestSuckIC`/`DoRearTestSuckIC`，~1350/1313 行)；或轉向 **W7** 續(csystem MainProc mode/SECS/AGV/temp ladder 等既有 gated 大宗)。cContact 本體(`DoTestContactFunction`的`CarlibrationTask`+6 sub-SM，~11347 行，VCL widget 穿插 switch-case 內部)仍是獨立大波，不因這次純算對翻譯而變小。

## 2026-07-11 — 使用者澄清暫停規則為情境性（非常態），開啟 W5 全面平行展開

**使用者澄清**：先前「做一個段落後暫停」是因為當時要關機需要暫停運算，非常態規則；使用者有空時要連續推進，並明確邀請「workflow 平行全面展開轉移」。自本日起，波次間不再預設暫停回報，改為主動連續推進+適時回報重大里程碑（獨立複驗紀律不受影響，見下）。

### Recon（9 路平行調查，1.6M tokens）
對全部剩餘未觸碰戰場做一次性 recon：SECSGEM、Automation、MR/RFID、KYECFTP、CCLink、Interface+CanBus+MyPLC+EtherCAT 小型通訊群、BarCode（原計畫外）、W6.4b 剩餘確認(atester_ProcessCount+Front/Rear Suck)、atester_32Site 卡點再確認。9 個 recon agent 各自回報 7 項結構化發現(inventory/VCL 耦合度/外部依賴/跨戰場依賴/會解鎖什麼/批次切法建議/地雷)。因單一 agent 回報過大(197K+ 字元)，改用一個獨立 synthesis agent 讀 journal.jsonl 全文、產出精簡決策用戰役計畫（而非自己吞下全部 raw text）。

**主迴圈獨立核實 synthesis 的關鍵主張**（勿照單全收）：grep `HT9045.bpr` FILELIST 確認 `SECSGEM\SECSGEM.cpp`(2052 行) 完全不在編譯清單、`SECSGEM.h` 全樹不存在 → **死碼，非本波範圍**；grep 確認 `MR\` 前綴檔案**完全不在** `HT9045.bpr` FILELIST → **MR/RFID 全目錄死碼**，非本波範圍(recon 一度誤列為活著的第 9 條戰場)；抽查確認 10 個提案單元(AMR/HANA_ART/uRENESAS_Other/InterfaceSYS/cMyNUDN1/MyNUEC1/MyCCLink/MyCCLinkBoard/MyPLC_IO_Modbus/ModbusTCPClient/SCK_ART)皆真實在 FILELIST 內；抽查確認 `AGV.cpp:1315/1327` 的 `IsSPIL_AMR`/`IsATK_AMR`、`uHGemHT9045.h:12` 的 `struct ETypeStruct`/`SECS_EVENT` 行號正確。

### Wave 1 翻譯戰役（14/15 單元完成、2 個序列 integrate、主迴圈兩輪獨立複驗）

**Batch 1 — 通訊叢集（6 平行 unit + 1 序列 integrate）**：
- `SECSGEM/uHGemClass.{h,cpp}`(`HTGem` 抽象基底,~90 virtual，~33 個 golden 本就 inline 者逐字照搬、其餘 57 個因需要尚未翻譯的 `THGem`(uHGemEquipment.h,~6200行) god-form 而個別 `#if 0` 帶 golden 行號+ACTIVE 保守預設) + `SECSGEM/SecsEventType.{h,cpp}`(從 `uHGemHT9045.h:12-313` 抽出的純資料 `SECS_EVENT`/`ETypeStruct`)。
- `Interface/InterfaceSYS.{h,cpp}`(整檔，~0% VCL)。
- `CCLink/MyCCLink.{h,cpp}` + `MyCCLinkBoard.{h,cpp}`(整檔，`#if HAVE_CCLINK` 比照 IOBackend.cpp 既有 HAVE_MN200/HAVE_PCI1203 慣例) + `CCLink/MyCCLinkSensor_predicates.{h,cpp}`(從 `MyCCLinkSensor.cpp:2584-2598` 抽出 `UseCanBusOrEtherCAT`/`UseSensorMod3` 兩個純 predicate，刻意不叫 MyCCLinkSensor.cpp 以免與未來延後的整檔翻譯混淆)。
- `CanBus/cMyNUDN1.{h,cpp}`(整檔)。
- `MyPLC/MyPLC_IO_Modbus.{h,cpp}` + `ModbusTCPClient.{h,cpp}`(整檔) + 新 `vclcompat/ClientSocket.{h,cpp}`(**本波唯一被授權新增的 vclcompat 元件**——Sim-預設/Real 走 WinSock2，仿 `Comm.h`/TComm 的既有 Sim/Real 二分模式，因為只有這個 unit 需要 TCP client shim，經 recon 確認無其他 unit 需要而避免重複發明)。
- `EtherCAT/MyNUEC1.{h,cpp}`(整檔,忠實保留位元封裝 union 佈局)。
- **Integrate-Comms**：新增 `ht9045_secsgem`(嚴格 HT9045_WARNFLAGS) + `ht9045_comms`(比照 ht9045_io/ht9045_motor 寬鬆 posture) 兩個 library；`Interface/InterfaceSYS.cpp` 因與 `ht9045_sm` 雙向依賴(避免 CMake library 循環)改直接併入 `ht9045_sm` 原始碼清單(unit 自己在報告中建議的解法)；新增 `SECSGEM/SecsEventReport.{h,cpp}`(極簡 Sim-first `EventReport(unsigned Ceid)`,刻意不 instantiate `HTGem`,避免逼所有連 `ht9045_sm` 的執行檔都要解 `HTGem::~HTGem()` 的 3-arg `MyDBIProcess`)，接上 csystem.cpp 既有 5 個 `W7C1_EVENTREPORT_*`/`W7C2_EVENTREPORT_*` no-op 巨集(真呼叫,offline-safe)；移除 `acatchtray_shims.{h,cpp}` 內一個早期 W6.3 波留下的 `SECS_EVENT`/`EventReport(){}` 佔位 struct(已核對數值逐一相符再換成真 header)；解決 25 個既有 test 的 `RESCAN` link-group 需要一併加入 `ht9045_secsgem`(CMake target-group 循環偵測觸發)。ctest 51/54(3 個既有無關失敗、新增 6 測試全過)。

**Batch 2 — Automation+atester 家族（8 平行 unit，1 個因 API 連線中斷失敗、integrate 仍成功接手其殘留檔案、1 序列 integrate）**：
- `Automation/AMR.{h,cpp}`(整檔) + `Automation/AGV_predicates.{h,cpp}`(從 `AGV.cpp:1315-1348` 抽出 `IsSPIL_AMR`/`IsATK_AMR`/`Use_AMR` 三個純 predicate，**本次 recon 找到最高槓桿的單點**——`FormsFacade.h` 原本 `fAGV->IsATK_AMR()` 等恆回 false 的 stub 換成真呼叫後，`acatchtray.cpp` 現有 ~18 個呼叫點首次評估真條件；主迴圈已逐行比對 golden `AGV.cpp:1315-1348` 與譯文 `AGV_predicates.cpp`，**逐字元完全相符**)。
- `Automation/HANA_ART.{h,cpp}`(整檔，填入既有 csystem.cpp 的 `W7C2_TfHanaSeam` 占位)。
- `Automation/uRENESAS_Other.{h,cpp}`(整檔，~22 個純資料 record，未來 uRENESAS_Server.cpp 波前置)。
- `Automation/SCK_ART.{h,cpp}`(**新檔，非整檔**——只翻 8 個函式 `SetLotStatus`/`GetLotStatus`/`CheckLoadingCount`/`CheckInArmNeedVariModeFIX`/`CheckOutArmNeedVariModeFIX`/`CheckNeedRT`/`DoAutoSocketOff`/`DoChkInputCntAlarm`，golden 4392 行剩餘部分仍延後；填 csystem.cpp 既有 `W7C1_TfSCKARTSeam`/`W7C2_TfSCKARTSeam` 占位)。
- **`atester_ProcessCount.{h,cpp}`（新檔，部分——14 個函式，golden 2161 行取 ~1234 行的乾淨子集）：翻譯 agent 在回報階段遭遇 API 連線中斷(`Connection closed mid-response`)、整個 workflow 該 unit 標記失敗、無報告文字送回**。integrate agent 沒有照單全收「無報告=無工作」，而是主動核對磁碟——檔案確實存在且完整(1418 行、頭部有完整的 14 函式範圍表+8 個 GATED DEPENDENCIES 清單+翻譯規則說明，逐條附 golden 行號)，判定為可用，正常併入 CMake+wiring。主迴圈事後複查：檔案結尾正確收合、無殘缺函式、9 個「未使用」的 yield 變數warning 全部對應到檔案自己標注的 `#if 0 // TODO(cSocket-module)` gate(消費端在延後的 cSocket.cpp 模組，golden :428-443 的區塊變數本就只服務被 gate 掉的 `bBySiteByBinPercentCompare` 分支)，非崩潰造成的缺漏。**
- `aTester_Front.cpp`/`.h` 擴充(新增 `DoFrontTestSuckIC`,延伸 W6.4b Wave 1 已有的 Destroy-IC 翻譯)、`aTester_Rear.cpp`/`.h` 擴充(新增 `DoRearTestSuckIC`，對稱擴充，Front/Rear 分屬不同檔平行無衝突)。
- `atester_32Site.{h,cpp}`(**新檔，整檔扣掉 2 個明確 gate 的 vision 孤島**——`DoTestY_TwoArm32Site` cases 55-68(golden ~230行,RTC 全景複驗) + `Do32RTCAutoModelVerify` 的 vision-half cases(golden ~318行,`COM2->SendCommToVision`/RTC 握手)，兩處**刻意不假造 return true**(會偽造未經驗證的 vision-pass 訊號，是真正的行為風險非只是編譯方便)，各自 `#if 0 // TODO(W5-atester32site-vision)` + golden 行號帶入。前次 recon「85%+ 卡在 COM2/RTC」的估計經本波修正——實測僅 ~10-15% 需延後，其餘 ~89% 忠實譯入。)
- **Integrate-Automation**：`Automation/{AMR,AGV_predicates,HANA_ART,SCK_ART}.cpp`+`atester_32Site.cpp`+`atester_ProcessCount.cpp` 併入既有 `ht9045_sm`；`uRENESAS_Other.cpp` 因目前零消費者、純資料，獨立成新 `ht9045_automation` library。**自行找到並修正 4 個真問題**（未被任何單元回報）：(1) `acatchtray_shims.h` 的 `TAMRShim AMR` 與 `Automation/AMR.h` 的 `TTeraPowerAMR AMR` 同名不同型 ODR 碰撞，移除舊 shim 改指向真 header；(2) `aTester_Front.cpp` 宣告 `extern int iFrontTestSuckICTask` 卻從未真的定義(過去被 atester_shims.cpp 的 stub 遮蔽)，補上遺漏的定義，比照 aTester_Rear.cpp 正確寫法；(3) `atester_32Site.cpp` 需要的 `CheckContactOver()`/`ShowIndexTime(int)`/`TemperatureStorageLog(int)`/`CheckTestSuckICOn(...)` 等 csystem.h 宣告但從未實作的 predicate，補上保守 offline 版本進 atester_shims.cpp(比照既有 IsInArmCleanOutFinish 等前例)；(4) `test_SCK_ART.cpp` 自身兩處測試撰寫錯誤(誤設錯的 LastSet 欄位、殘留 stale 值)，逐行對照 golden 確認 SCK_ART.cpp 本身 byte-exact 無誤後修正測試。ctest 58/61(3 個既有無關失敗、新增 7 測試全過)。

### 主迴圈獨立複驗（兩輪 fresh from-scratch + 一次 stash 前後對照，非僅信 2 個 integrate agent 自報）
- **clean build 全綠(exit 0)**，唯一 6 個 grep 命中的「error」字樣皆為誤報(檔名/函式名含 Error 字面，如 `WinSocketErrorCode`/`PorcessJAM0201OutArmPickUpErrorSkip`，無真正編譯錯誤)。
- **ctest 58/61 PASS**，3 個失敗與前波(cContact 波)完全同組(`config_db`/`IniFiles`/`ini_helpers`，現場 `system/Gerneral.ini` 設定漂移，非本波造成)，14 個新增測試(6 comms + 8 automation，扣掉失敗的 atester_ProcessCount 沒有專屬測試)全數通過，無回歸。
- **警告數表面看 720 筆嚇人，逐一比對後只有 ~29 筆是真的新增**：用 git stash 只還原本波所有共用檔(CMakeLists.txt 等 17 檔)重跑一次乾淨 baseline build，得到 **668 筆早已存在的既有警告**(主因是 `cmydef.h`/`MachineType.h` 這兩個 W0 尾端頭檔本身有幾處 BCB6 遺留寫法，在不同 library 的警告旗標組合下反覆被觸發——並非本波引入)；把兩份警告集合做正規化 diff 後，真正的 29 筆新警告逐一核實：`atester.cpp`/`acatchtray.cpp` 的看似「新」警告其實是既有程式碼因新增 `#include` 造成行號位移的**假陽性**(同一段程式碼、同一則警告，只是行號變了)；`atester_32Site.cpp`(iRetry/bVerifyNG/ret 共 3 筆)與 `atester_ProcessCount.cpp`(9 筆 yield 變數)皆對應到檔案自己文件化的 gated 區塊(消費端刻意延後，非缺陷)；`canary_support.cpp` 的 9 筆 missing-field-initializer 屬新增 LAST_GENERAL_SET 欄位的良性語法噪音(該欄位仍會 zero-init)；`uHGemClass.cpp/.h` 的 2 筆(trailing `;`/unused-parameter)是翻譯報告已預期、integrate agent 卻忘了補的 `-Wno-pedantic` carve-out——**主迴圈事後補上**(比照 cContact.cpp 前例，只窄範圍抑制這兩類、其餘警告類別仍全開)，重跑確認消除。
- **U+FFFD 逐檔核對(自建 python script，涵蓋全部 68 個本波新增/異動檔)：0**。
- 主迴圈逐行核對 3 個最高風險/最高槓桿處：(1) `AGV_predicates.cpp` 對 golden `AGV.cpp:1315-1348` **逐字元完全相符**；(2) `atester_ProcessCount.cpp` 崩潰倖存內容經確認為完整、有清楚文件的合法部分翻譯，非殘缺；(3) `atester_32Site.cpp` 的 2 個 gate island 確認正確以 `#if 0 // TODO(W5-atester32site-vision)` 圈住並附精確 golden 行號、無外洩。

### 本波明確排除範圍（recon 判定風險過高/耦合過深，留待專屬未來波次，非遺漏）
- **BarCode**：兩個個別超過 2300 行的函式(`DoBarcodeCCDInShuttle_1`/`_2`)+需要全新 `TClientSocket` 完整對等層才能碰核心引擎(`BarCode.cpp` 11966 行，全樹最大單檔)。
- **KYECFTP**：`FTPClient.cpp`(5439 行)需要從零打造 `TNMFTP` shim(無既有對應)，值得獨立設計波次而非快速搭便車。
- **SECSGEM 協定引擎+站點覆寫層**：`uHGemEquipment.cpp`(~8800行,THGem 真本體) + `uHGemHT9045.{h,cpp}`(~5200行,~22 個表單 fan-out，全樹最寬)兩者皆延後。
- **MR/RFID 全目錄**：確認為死碼(不在 HT9045.bpr FILELIST)，除非有明確客戶需求，否則不排入任何未來波次。
- **CCLink/CanBus/EtherCAT 的 VCL 表單殘餘+真廠商呼叫本體**：`MyCCLinkSensor.cpp` 表單殘餘(~2560行,含 672 行即時刷新迴圈)、`MyEtherCAT.cpp`(與另外延後的 Motor EtherCAT 軸驅動共享 `uiDevhand`/`MOT[]`,需與馬達 HAL 波次聯合決策)。
- **Automation 剩餘**：`Interface/TesterTCP.cpp`(70-85% VCL 穿插協定解析)、`auto9045.cpp`/`automation.cpp`(~5500行,需 ~40 個新 seam 決策)、`AGV.cpp` E84 本體(~1860行)、`SCK_ART.cpp` 剩餘 VCL widget 密集片段(~3500行,部分卡在未翻的 LastSet.h 欄位)。

### 下一步候選（隨時可續，不預設暫停）
BarCode 分批(先切小 helper，核心留專屬波)；KYECFTP 的 TNMFTP shim 設計波；SECSGEM 協定引擎(需先設計 headless StringGrid helper + HSMS TClientSocket/TServerSocket)；Automation 剩餘(TesterTCP/auto9045/automation.cpp/AGV E84 本體/SCK_ART 剩餘)；或轉回 **W7** 續(csystem MainProc mode/SECS/AGV/temp ladder——現在 SECS_EVENT/EventReport 已有基礎骨架，AGV/SCKART/HANA seam 已局部填實，此路徑的既有卡點有所鬆動，值得重新評估)。

## 2026-07-11 — BarCode Wave 1：TfBarCode_Shim 17→20 方法 18/20 真本體完成

**使用者指示**：「繼續 BarCode」——延續 W5 Wave 1 recon 已標記的 out-of-plan 大戰場（原 MIGRATION_ROADMAP 未列名，只以 `TfBarCode_Shim` 離線 stub 形式間接被 4 個已完成 W6 引擎依賴）。

- **範圍修正（recon 誤差）**：`TfBarCode_Shim` 實際有 **20 個方法**(非先前估計的 17 個)——完整清單：`InitBottom2DIDScan`/`DoBottom2DIDScan`/`DoBottom2DID_8CCD_Scan` + `InitialBarcodeScanIn/OutShuttle1/2`(4) + `DoBarcodeScanIn/OutShuttle_1/2`(4) + `DoBarcodeTriggerInShuttle_1/2`(2) + `DoBarcodeCCDInShuttle_1/2`(2) + `InitialShuttleFloatCheck1/2`(2) + `DoShuttleFloatCheck_1/2`(2) + `IsSHT2DIDScanFinish`(1) = 20。
- **7 平行 translate unit**（golden `BarCode/{BarCode,BarCode_Sh1,BarCode_Sh2}.cpp`，全樹最大單類別 `TfBarCode`，跨 3 檔約 23,163 行；本波刻意用**獨立命名檔**而非鏡射 golden 檔名，避免多個 unit 同時寫入同一個 `BarCode.cpp`/`BarCode_Sh1.cpp` 造成平行衝突——`BarCode/BarCode_Helpers.cpp`(TfHIKROBOTcmd+uCCDUnloaderClip+IsSHT2DIDScanFinish)、`BarCode/BarCode_Bottom2DID.cpp`、`BarCode_Bottom2DID8CCD.cpp`(**唯一未遵照指示鏡射進 BarCode/ 子目錄、落在專案根目錄的檔案**，已在 CMakeLists.txt 註解記錄此已知偏差、功能正常不影響驗證，故保留不搬動)、`BarCode/BarCode_Shuttle1_Scan.cpp`、`BarCode/BarCode_Shuttle2_Scan.cpp`、`BarCode/BarCode_Shuttle1_CCDScan.cpp`、`BarCode/BarCode_Shuttle2_CCDScan.cpp`)。
- **忠實保留多個 golden 缺陷/不對稱**：`uCCDUnloaderClip::SendCommandStr` 一處 golden `return`(無分號)+下一行語句被 C++ 文法摺疊成單一 `return SendCommandSocket(...)`陳述式，效果與視覺排版相反(`bSimulationCommand==true` 才真的呼叫，`false` 什麼都不做)——忠實翻譯成等價 `if{call;return;}`並詳細註解；`DoBarcodeTriggerInShuttle_2` case 3400 誤寫 Shuttle-**1** 的全域變數(`iSH1_1BarcodePosition`等，應為 SH2 對)，與同檔其餘處不一致——判定為 golden 自身 copy-paste bug，逐字重現不「修正」；`DoBarcodeScanOutShuttle_2` case 1000 用列索引 `[0]`(其餘所有 `iShtCol` 分支皆用 `[1]`)——同樣忠實保留。
- **誠實的部分完成（Shuttle2Scan unit）**：分配的 7 個函式中，`DoBarcodeScanInShuttle_2`(golden ~1222行)+`DoShuttleFloatCheck_2`(golden ~474行)經讀取後發現遠比「small helper」複雜(各自牽出一批全新未 home 符號如 `fNote->t2DCode`/`mtSFC_Shuttle` VCL grid/`Prod.iInSFCPos1x1_2`參數表)，依專案既有 1500-2000 行停止門檻，**只完成 5/7、其餘 2 個乾淨交接**(附精確 golden 行界+已讀進度百分比)，`TfBarCode_Shim` 這 2 個方法維持原離線 stub 不變。該 unit 過程中還自行抓到並修正一個自己引入的 bug(`-Wconversion` 加轉型時把 `pos+=Offset.iSHLeft2D[1]` 誤改成只轉型 offset 而非整個和，已修正為整體轉型以保留原本截斷行為)。
- **Integrate（序列，1 agent）**：18/20 方法接上真本體(`aHotPlateSubstrate.cpp` 的 `TfBarCode_Shim` 委派)，2 個未交付方法維持原 stub。自行找到並修正 3 個真問題：(1) `bCCDcheckShuttle[2]` 與 `cmydef.cpp` 既有全域重複定義；(2) 8 個 `TfBarCode` 成員全域(`bStepError`/`iCurrentStep`/`s2DIDYield`等)被 `BarCode_Bottom2DID.cpp` 與 `BarCode_Shuttle2_CCDScan.cpp` 兩個 unit 各自誤認為自己是唯一持有者，改為前者單一持有、後者改 `#include`；(3) 真連結缺口 `TMyProductionRecord::Add2DIDRecord` 宣告但無本體，補一行 offline no-op(同既有 4 個兄弟 no-op 慣例)。另修 1 個測試 include 路徑漏字首碼的筆誤，並**清除 2 個某 unit shell session 意外留下的垃圾檔**(`t2DCode`/`usage`，未被任何交付物引用)。
- **主迴圈獨立複驗（fresh from-scratch build + 一次 stash 前後對照，非僅信 integrate 自報）**：clean build exit 0(6 個 grep "error" 命中皆為既有誤報)；**ctest 65/68 PASS**(3 個既有無關失敗同前波、7 個新 BarCode 測試全過、總數 61→68 精確吻合)；**明確重跑 4 個反向依賴引擎的既有 oracle**(`W6_2_InArmCore`/`W6_2_OutArmCore`/`W6_3_CatchTray`(+Core)/`W6_5_Shuttle`/`W6_6_Hub`+`CSystemCycle`/`W7_C1`+`C2`共 9 個測試)——**100% PASS、斷言數與 DEVLOG 既有基準逐一相符(如 W6_5_Shuttle 仍是 12 斷言)，零回歸**；U+FFFD 逐檔核對(24 檔)為 0；警告數表面 747 筆，經 stash 前後 baseline 對照(718 筆既有)只有 **6 筆表面新增，其中 2 筆(`tRotateShim` 缺欄位初始化)為既有警告的行號位移假陽性，剩 4 筆(2×`iStepCount` 已設未讀、`bNeedCheckWhitleList` 對應到誠實交接後的閒置 gated 區塊、`Gated_InitSht2DCodeComPort` 對應到未交付的 2 個函式而閒置)皆逐一核對為良性、非缺陷**。
- **下一步（BarCode 續，2/20 未交付 + 原本就明確延後的項目）**：`DoBarcodeScanInShuttle_2`/`DoShuttleFloatCheck_2`(需先 home `fNote`/`mtSFC_Shuttle` VCL grid 等一批新符號)；或跳去 recon 已標記的「Category C」相關函式(`DoSFCAutoTune_1/2`/`Do2DIDCheckSh1/2`/`CCDBarcode_Decoding`系列)；或 `TClientSocket` HAL 專屬波(解鎖 8-CCD 連線生命週期 glue + `BarcodeXML.cpp` 的 `SendTestResultToHttp`)；或轉去 KYECFTP/SECSGEM 協定引擎/Automation 剩餘/W7 續。

### 🔖 RESUME（最新）
- **✅ BarCode Wave 1 完成（2026-07-11）**：`TfBarCode_Shim`(20 方法，先前估 17 為 recon 誤差) 18/20 接上真本體，7 平行 unit+1 integrate，詳見上方 2026-07-11 BarCode 區段。翻轉 4 個既有反向依賴引擎(`acarry`/`ainarm9045`/`aoutarm9045`/`csystem`)從恆假 stub 變真行為，主迴圈已明確重跑並核對這 4 引擎既有 oracle 斷言數逐一相符、零回歸。ctest 65/68(3 個既有無關失敗)、mojibake 0、真新增警告 6 筆全數核實良性。剩 2/20 方法(`DoBarcodeScanInShuttle_2`/`DoShuttleFloatCheck_2`)誠實交接延後。**下一步不預設暫停**：BarCode 剩餘 2 方法或 Category C、或 TClientSocket HAL 波、或轉去 KYECFTP/SECSGEM/Automation/W7 續。
- **✅ W5 Wave 1 完成（2026-07-11，comms 叢集+automation/atester 家族，14/15 unit+2 integrate）**：詳見上方 2026-07-11 區段。新 library `ht9045_secsgem`/`ht9045_comms`/`ht9045_automation`；`Interface/InterfaceSYS.cpp` 併入 `ht9045_sm`。SECS_EVENT/EventReport 骨架已接上 csystem.cpp 的 W7C1/W7C2 EventReport 占位；AGV/SCKART/HANA seam 已局部填實（`fAGV->IsSPIL_AMR/IsATK_AMR/Use_AMR` 現為真呼叫，翻轉 acatchtray.cpp ~18 個既有呼叫點）。`atester_32Site.cpp` 整檔翻完(僅 2 個 vision 孤島延後)、`atester_ProcessCount.cpp` 部分翻完(14 函式，翻譯 agent 崩潰但檔案完整可用)、Front/Rear Suck 對完成(W6.4b 收尾)。ctest 58/61(3 個既有無關失敗、無回歸)、mojibake 0(68 檔)、真新增警告僅 ~29 筆且皆已核實非缺陷。**使用者已於 2026-07-11 澄清舊「每波暫停」規則僅因當時要關機、非常態；有空時應連續推進+適時回報里程碑。**
- **▶ W7 進行中**：✅ A1/S0/grid-occupancy staging/C1/C2/T1（詳見上方各自日期區段，全部 commit+主迴圈獨立複驗綠）。
- **▶ W6.4b 進行中（recon 完成、Wave 1 完成+commit 2026-07-06、cContact 純算對完成 2026-07-10）**：recon 修正 ROADMAP 錯誤標籤——`iIndexTask@cContact` 不存在，真正下壓 cursor 是 `TfContact::DoTestContactFunction` 的 `CarlibrationTask`(cContact.cpp:11755-13963)+6 個 sub-SM，共~11347行≈cContact 50%，VCL 穿插 switch-case 內部，需獨立 W7 波（非 W6.4b 範圍）。**✅ Wave 1(Front/Rear「Destroy IC」下壓 SM 對)完成+commit**：新增 `aTester_Front.cpp/.h`+`aTester_Rear.cpp/.h`，`DoFrontTestDestroyIC`/`DoRearTestDestroyIC`(golden :309-865/:309-886)+6 葉依賴忠實譯入；2 個平行 translate agent 各自發現同一組 ~9 substrate 缺口(golden-cited gate，互相佐證)；integrate 抓到 2 個真正 cross-module undefined-reference(`OutSHT1/2InLF`+`CheckIndexAllSuckICFallDown` 補進 csystem_predicates.cpp)+1 個 `GetSiteCount` duplicate(cprod.cpp 本體仍 gated)；全套 ctest **47/47**(46 既有+1 新，無回歸)、mojibake 0、獨立 fidelity-review agent 逐 case 對 golden zero discrepancies(含驗證 golden 自身 case-100 括號缺陷+Front/Rear 不對稱皆忠實保留未「修正」)；主迴圈複驗時抓到並修正 1 個測試自身撰寫錯誤(`TMySimMotor.Enable` 建構子預設 true 非 false)。**✅ cContact `GetMinForce`/`GetMaxIndexForceLimit` 純算對完成(2026-07-10)**：新增 `cContact.h`/`.cpp`(extract-calc-core，零 ht9045_globals link)，忠實保留 2 個 golden 缺口(ItemIndex 無 else 停 0.0；dMinForce member→純區域變數，對唯一呼叫點等價)；ctest **45/48 PASS**(新增 cContact 套件 100%、既有 3 個 config_db/IniFiles/ini_helpers 失敗經 stash 前後對照證實為**既有環境漂移、與本波無關**，非回歸)；獨立 fidelity-review agent 8 項檢核零差異。**下一步序（W6.4b 剩餘）**：`atester_ProcessCount.cpp` 易翻葉節點 或 Front/Rear Suck 對(`DoFrontTestSuckIC`/`DoRearTestSuckIC`)。**⏸ 使用者已指示「做一個段落後暫停」——下一波需使用者明確再叫續做，不可自動接續**。W6.4b recon 全文見上方 2026-07-06 區段、本波細節見上方 2026-07-10 區段。
- **其餘 W7 下一步候選**：MainProc mode/SECS/AGV/temp dispatch ladder(golden csystem.cpp:16730-19101,~2000+行,需 W5 SECS+home)；或 W6.2b in/out site variant 收尾檢查(已 100% 完成，僅需 spot audit)。UI-framework fork 續延 2-3 波。W7 recon 全文見上方 2026-06-29 區段＋scratchpad workflow script。
- **✅ W6.2c in-arm 變體 fan-out 完成（25/25 `ainarm9045_*.cpp` 全翻）**：canary 3 + batch-1 5 + batch-2 6 + batch-3 6 + **batch-4 5**(`2x2_8_Hot`/`2x4_8`/`2x5_8`/`2x6_8`/`2x8_8`，本 session)。全 dispatched ACTIVE（`2x8_32` dead-but-compiled、其 enum 依 golden :4639 路由到 `DoInArm_9045_2x8_8`）。ctest **39/39**、mojibake 0、directive 平衡。可重用 workflow script 在 scratchpad `w62c_inarm_variant_batch.js`(改 `BATCH`+`BATCH_LABEL` 即換波，golden=`HT9011UC_Code_V3.33.906.0_20260618`)。**架構=parallel translate(只寫自己檔、回報 substrate 需求)→serial integrate(統一改共享檔)→clean verify**(記取 atester 11819 行單 agent 連線中斷教訓:單檔別超 ~6-8K、批次有界)。
- **🏁 IN-ARM 變體 dispatch 100% 完成**：25 `ainarm9045_*` + 2 `ainarm9045S_*`(batch-5) 全 ACTIVE（2x8_32 dead-but-compiled）。in-arm 側全清、ctest 40/40。
- **▶ OUT-ARM 變體 fan-out 進行中（27 檔總量，「跑全部」campaign）**：總量 = **20 LIVE**(1x1_1/1x2_2/1x2_4/1x3_2_14/1x3_4/1x4_2/1x4_4S/1x4_4/1x4_8/2x1_2/2x2_4/2x2_8/2x3_6_14/2x3_6/2x4_4/2x4_8/2x5_8/2x6_8/2x8_8/All_1Picker) + **6 DEAD**(1x4_4_Back/2x2_4_14/2x2_4_23/2x4_16/2x8_16/2x8_32) + **1 S-prefix DEAD**(`aoutarm9045S_1x4_4`，原 RESUME 漏算的第 27 檔；out-arm 用 suffix-S `_1x4_4S` dispatch，S-prefix `DoOutArm_9045S_1x4_4` 全程未被任何 ladder 呼叫=dead-but-compiled)。engine dispatch ladder 已 active。整合 live=加 CMake+移除該變體 2 個 static stub+un-gate 2 extern；dead=只加 CMake。**🏁 LIVE 20/20 完成**：B1(6)+B2(7)+B3(7) 全 commit、ctest 41/41、out-arm dispatch 100% live（directive 21/21）。**✅ B4=6 DEAD 完成+commit**(1x4_4_Back/2x2_4_14/2x2_4_23/2x4_16/2x8_16/2x8_32，CMake-only、aoutarm9045.cpp 未動、6 dead 符號 nm T 且 engine 零 reference、ctest 41/41)。**剩 1 檔**：**B5=1 S-prefix dead**(`aoutarm9045S_1x4_4`，DoOutArm_9045S_1x4_4 全樹零外部 caller=dead-but-compiled；不規則命名→專用 mini-workflow `w62_outarm_Sprefix.js` 已備、translate→integrate(CMake-only)→verify)。**🏁 27/27 out-arm 檔完成**（B5 `aoutarm9045S_1x4_4` 已 verify+commit 2026-06-29、in/out 兩臂 site-variant fan-out 全清）。之後 **W7**(MainProc mode/SECS/AGV/temp ladder + end-of-lot finish-check 本體 + home/setup + 解 dead 檔 forward-decl 的真正 home)。
- **🏁 W6 CORE LAYER 完成**：五引擎(in/out-arm/tray-arm/tester/shuttle) **+ csystem 整合 HUB** 全 over Sim HAL pump。**W6.6 完成**：csystem.cpp(25,483 行)spine 672 行 active(~16 方法/~6%)、DoAllProcess 主迴圈把 5 引擎以記錄 A/B tick 序串起來、64 cycle pump=32A+32B 無 crash、4 oracle 過(20 PASS)、ctest **34/34**。W7=(MainProc mode/SECS/AGV/temp ladder + end-of-lot finish-check 本體 + home/setup) 解 gated 大宗。
- 已完成：…W3、W4 馬達/IO HAL、KeyPro+TComm、cpublic 基礎、**W6.0→W6.5 全 root 引擎(in-arm/out-arm/tray-arm/tester/shuttle)over Sim HAL**。全 green。
- **W6.5 SHUTTLE 完成**：acarry.cpp(8521 行)136/136 case ACTIVE、0 `#if 0`(本波最高覆蓋)；Do_Auto_SHT1 1→10 pump 過、5 oracle 過(truncation 11001/zero-guard/sensor-broken left-pass)；acarry_shims 修譯期連結缺口；ctest 32/32。
- 已完成：…W3、W4 馬達/IO HAL、KeyPro+TComm、**cpublic 基礎 ungate(stub 移除)**。HAL Sim 層完整。全 green、已 commit（branch 19 commits）。
- 中文註解亂碼：**確認 100% 在 `//` 註解內、無 string literal 受損**（非功能性缺陷；golden 有原文）→ 降為低優先 cosmetic TODO（ROADMAP DEFERRED），go-forward 規則已記。
- **W6 recon 完成**（見 ROADMAP「W6 計畫」）：單根 fan-out、csystem.h predicate 介面、iXXXTask 各 arm 自有、main.h/TfMain→FormsFacade 解耦。子波 W6.0 scaffold→W6.1 canary(asendic_Empty)→W6.2 in-arm→…→W6.6 hub。
- **W6.0 SCAFFOLD + W6.1 CANARY 完成（W6 解耦策略端到端證明）**：csystem.h 凍結為介面、`csystem_predicates.cpp`(15 個 HasIC 於 Sim HAL+Prod)、`aArmHeader.h` shim(god-header gated)、非 VCL `FormsFacade`(TfAGV stub)、新 `mycylin`(TMyCylinder over Sim IO，substrate 補洞)、asendic/canary_support helpers。**canary `asendic_Empty` 翻譯+收斂**：DoAutoEmpty 經 Sim HAL pump 走 1→20→…→70→100→150→1 收斂(wall-clock 計時 gated，Sleep(5)/6s deadline 過 timer)、無 crash、自帶 cursor、無 TfMain/vendor。predicates 21 斷言(OR-aggregation+purity)。**mojibake=0**(go-forward Big5 規則奏效)。ctest 23/23、新 lib ht9045_sm。
- **W6.2 IN-ARM 基礎完成**：`ainarm_SearchPickPlate`+`ainarm_SearchPlacePlate` 翻譯；新 `aHotPlateSubstrate`(InArmSuck=TMyKitSuck/PickFromHPList/ainarm2 cursors/HP arrays，最小 scope)；FormsFacade 擴(TfMain/SortCT/LotInfo/OffSet/SCKART，no-op)；DoInArmPickFromHotPlate_9045 Sim HAL pump 過。ctest 25/25、mojibake 0。
- **W6.2-CORE IN-ARM 引擎完成**：`ainarm9045.cpp`(9222 行,302 iInArmType 分派) + `.h` 忠實翻譯，名稱全保留。3 個核心 task SM 保持 ACTIVE 並 Sim HAL pump 過：DoInArmAdditionalFunction(cursor 1→100 fall-through 保留)、DoInArmIonFanGiveWay(1→5)、ProcessSCKARTLoadingCount(1→100→200)。DoInArm_9045 / _SuckerMap 分派 ladder 結構 ACTIVE(final else Program-Error 路徑活)。21 個 `#if 0` gate 全平衡(各帶 active stub)：25 個 per-variant call-arm gated→W6.2b；GetShuttleCol XPHSuckToSht_* 表 gated→W7；4 個整體 SM(DoInArmPickFromLoadStage_9045 中央取料/CheckShuttleFloating/CheckInShuttleSensor_Latch/AutoCalSuckZ) gated→W7(保 cursor+golden default)。substrate 擴 A/B/C/D(iArmTask/iPickFromLoadStageTask=1、iCloseSiteModeFor1x4、TMyKitSuck 新成員、fBarCode shim)；11 個 substrate stub 移除改由 ainarm9045.cpp 真實定義。8 個 numeric/geometry oracle 過(含 GetInArmPitch_9045 float→int 截斷契約 5000.625→5000)。ctest 26/26(clean build)、mojibake 0、gate 21/21 平衡。
- **W6.2-OUT 出料臂引擎完成**：`aoutarm9045.cpp`(golden 4753 行) + `.h` 忠實翻譯(in-arm 鏡像)。4 核心 SM 保持 ACTIVE 並 Sim HAL pump 過：DoOutArmPlaceToAuto_9045(iPlaceToAutoTask，1→10→…→500，1→10/50→100 fall-through)、DoOutArmAfterPlaceToAuto(iDoOutArmAfterPlaceToAutoTask)、DoOutArmAdditionalFunction(iOutArmAdditionalFunctionTask，1→100 fall-through)、DoOutArmIonFanGiveWay(iOutArmIonFanGiveWayTask)。DoOutArm_9045 / DoPickFromShuttle_9045 分派 ladder ACTIVE(DoOutArm_9045 無 final else，silent no-op 忠實重現)。substrate 擴 OutArmSuck/OutArm2Suck/FRCarryKit/BRCarryKit + TMySucker/TMyKitSuck 新成員 + 出料 cursor(iPlaceToAutoTask 等=1)。22 個 #if/#ifdef gate 全平衡(1×W6.2b variants + 20×W7 硬體 + 1×SOFT_SIMULTE 原樣)。oracle 過(GetOutArmPitch_9045 truncation 12001,5000→5000.125→5000；Y 零除防護)。**catch**：EnableFix3UseCylinder/ReversionEmptyPoint 的 cprod.cpp 本體仍 #if0(W6)→暫加 offline stub(待 cprod ungate 移除)。ctest 27/27(clean build)、mojibake 0。
- **W6.3 CATCHTRAY (TrayArm) 引擎完成**：`acatchtray.cpp`(golden 9142 行) + `.h` 忠實翻譯。**6 核心 SM 全 ACTIVE**：中央 `DoCatchTray()`(cursor CatchTrayTask) 分派 5 子 SM `DoCatchFromLoader`/`CatchNewTrayFromBuffer`/`DoPlaceTrayToAuto`/`DoPlaceToBuffer`/`DoSlapTray`(各自有 cursor，皆 acatchtray.cpp 自有)。**非 iXxxType 變體分派、是 config 分派**(TRAY_ARM_MODE/TrayForm/IniConfig/CUSTOMER_CODE 純決策 if/else 全 verbatim、無 default 的 switch fall-out 忠實重現)。Sim HAL pump 過：DoSlapTray(true)→cursor=1/return false、case 1→5(eUnderCoveyor 短路)；DoCatchTray case 1→50(eUnderCoveyor)。5 oracle 過(IsTrayArmAtEmptyOrColor 的 +100/-100 整數視窗、IsTrayArmMoveAvoidOutArmCrash、Led 判斷；MOT==NULL 離線時 ReadPos 保 Position 為確定性旋鈕)。**只有 2 個真 `#if 0`**(BinDisCtrl→W7-UI，NULL-guard 離線本就 dead)；32 個 SOFT_SIMULTE 區塊 verbatim(bFailAlarm=true 活)；36/36 directive 平衡。新增 `acatchtray_shims.{h,cpp}`(透明命名離線 cross-module shim：fTrayMapping/Magazine/OCR/AMR/cassette 本體 gated)。**ODR 解決**：`WhichAutoNeedTray` 真本體(golden:410)移入 acatchtray.cpp、canary_support.cpp 的 return-0 stub 刪除(改註解)。ctest 29/29(clean build)、mojibake 0、96 譯出源檔。
- **W6.4 TESTER/TEST-HEAD (Index 下壓) 引擎完成**：`atester.cpp`(golden 11819 行，最大 TU) + `.h` 忠實翻譯。**核心 test-head task SM 保持 ACTIVE 並 Sim HAL pump 過**：`DoTestY()`(cursor iTestYTask) 測試循環分派 SM——oracle 路徑 1→50(golden:4890)→100 **FALL-THROUGH 到 110**(golden:5180 無 break)同一 tick；`DoTestHeadMotor()`(cursor iTestHeadMotorTask) 中央 index SM——oracle 路徑 **4→9→10→15** over Sim HAL(離線 Motor==NULL → Gali Z/Y 移動立即回報完成、ISNormal()==true)；entry preamble 做成 offline-safe。decode 消費者 `Check_TTL_Status`/`CheckTestSocketIsError`(忠實 golden 本體，**翻譯期一度誤植被 re-read golden 修正**)、`IndexEveryTimeCheckEP`(case 1 early-out + SOFT_SIMULTE #else 樹 verbatim) 亦 ACTIVE。**5 個 oracle 全過**：ArrayConvertSite(1,2)@_8Site2X4==6 / (1,1)@DualSite2x1==3 / (0,2)@DualSite==2；GetRowCol@_8Site2X4→2,4 ret4 / SingleSite→1,1 ret0 / QualSite2X2→2,2 ret3；GetIndexZSpeed **整數除法** 50000*30/100==15000、scale=0 clamp 1→500、!slowdown→GailSpeed(離線安全)。**大宗 gate**(49/49 directive 平衡)：GetTesterResult(:849 GPIB/TCP+MOT/Suck/Socket ~1700 行,W5/W7)、ProcessTestResult/ProcessTesterTimeOut/DoCheckSocketHasIC/DoTestHeadMotorLoadCell/torque/EP/RTC/socket-sensor 本體 + DoTestHeadMotor 6232-8655 下壓/12000../14000.. case 樹(含 cContact iIndexTask 交握，**cContact 是 22761 行 VCL form 非本波**) 全 `#if 0` 帶 active stub；32Site/Front/Rear/ProcessCount/fContact/ADAM_* 走 atester_shims。**HAL 改動**：Motor==NULL 快路徑(Gali_Two_ZAxis_Move/GalilTwoY_Move/ISNormal 離線回 true) — 無 active 呼叫者於 arm/tray 引擎、無測試斷言、無回歸。substrate 擴：TMyKitSuck AlreadyTest/AlreadyTestNotIncludeErrorBin/PartAlreadyTest(忠實 golden)、LastSet iD47SocketTestedCount/bUseTestSocket、fMain LightOn/lbCCDStatus、atester_shims COM2/CCDInterfaceForm/fAutomation/fObserver/fiosetview/IsNNMode/CheckHeaterOK/EPSwitchOnOff/iCASE_REAL_CCD2。**ODR**：MoveInArm2XYToWait/CheckInArmFinishAllPickerAction 由 acatchtray_shims 單一持有(atester_shims 只宣告不重定義)。新 test `test_w6_4_tester.cpp`(W6_4_Tester) 17 斷言。ctest **30/30**(clean from-scratch build)、mojibake 0(全 12 檔)。
- **下一步：W6.4b** atester_32Site/atester_ProcessCount/aTester_Front/aTester_Rear(現 shim COMPLETE) + cContact iIndexTask(下壓 SM,22761 行 VCL form)；或 **W6.5 shuttle/carry**(acarry.cpp)；或 **W6.2b** in/out site variants(25+26) pipeline。同套 scaffold 縫。
- ⚠ 技術債：(1) `bInArmCheckDestroyACT`+4×`bPickFormHotplate*` 暫由 aHotPlateSubstrate.cpp 持有(避 ODR)。(2) EnableFix3UseCylinder/ReversionEmptyPoint 暫由 aoutarm9045.cpp stub(待 cprod.cpp W6 ungate)。(3) W7 需 MOT[]/sensor/fProductionInfo home 才能讓 in/out 整體 gated SM 端到端 pump。(4) catchtray 的 fTrayMapping/Magazine/OCR/AMR/cassette/cover-tray 本體 gated 於 acatchtray_shims(W7 + AMR 波)。(5) **W6.4**：DoTestHeadMotor 下壓/test/torque/EP/load-cell case 樹(6232-8655) + GetTesterResult/ProcessTestResult 本體 gated→W5(tester-comm)/W7；cContact iIndexTask 下壓 SM 整檔未翻(W7)；COM2/CCDInterfaceForm/fAutomation/fObserver/fiosetview/IsNNMode/CheckHeaterOK 為離線 shim(待 rs232/CCDInterface/automation/cObserver/iosetview/cinitial 翻譯時還原)。
- ⚠ SOFT_SIMULTE 計數誠實註記：golden atester.cpp 有 21 處 SOFT_SIMULTE，譯檔僅保 ACTIVE 函式內的(TestArmBackPos/Test_GetTestResulSub/IndexEveryTimeCheckEP_impl) verbatim；其餘 16 處在被 `#if 0` gate 的大 SM 本體內，隨本體 gate 掉——與 arm 引擎前例一致(ainarm 1 vs golden 5、aoutarm 3 vs golden 12)。非全等是 gating 必然，非漂移。
- ⚠ Windows build dir busy-handle race：clean build 若遇 `rm: build Device or resource busy`/`ranlib: libvclcompat.a No such file`，是檔案 handle race 非 source 缺陷；換新 build dir(build_v2)即過。
- ⚠ Big5 go-forward 規則持續(cp950/gloss，0 U+FFFD)。99 譯出源檔、branch 27 commits。
- 驗證指令同前（cmake MinGW Makefiles + ctest）。

---

## 2026-07-11(執行) / 2026-07-13(復原+複驗+commit) — W5-Final：KYECFTP + Automation 剩餘 + TesterTCP_Socket + BarCode 收尾(20/20) + SECSGEM codec/registration 前置切片

**背景（workflow crash 復原案例）**：本波實際翻譯工作於 2026-07-11 11:13–13:30 執行完成（與同日稍早的 BarCode Wave 1 同一天，但是後續的另一場戰役），CMakeLists.txt/tests/CMakeLists.txt 已完整接上、每個檔案都留有詳盡的 integrate-agent 註解（golden 行號引註、依賴理由、ODR/命名衝突的明確判斷），且各自的獨立 verify build 目錄（`build_w5final_*`）也顯示個別單元測試通過——但主迴圈的「跨戰線合併複驗＋DEVLOG/ROADMAP 記錄＋commit」三步從未執行，工作樹留下 2 天未提交、未記錄的狀態。2026-07-13 接續時，**沒有相信任何殘留的 agent 文字或 cached 摘要**，改為直接檢視 git status/檔案時間戳/舊 build 目錄的 ctest log 逐一重建事實，再跑一次全新 from-scratch build 定案（見下方「主迴圈獨立複驗」）。

**KYECFTP（新 library `ht9045_kyecftp`，3 檔，目前是前置切片、尚無真呼叫端消費）**：
- `MiniFtpEngine.{h,cpp}`：`Nmftp::TNMFTP`，從零打造的 FTP 協定引擎，頂替 repo 內完全不存在原始碼的 BCB6 `TNMFTP` 元件（每個推斷的線路行為判斷都留檔頭記錄：async→sync、僅 PASV、`Vendor`/`ParseList` 閒置、`CurrentDir` 走真實 PWD round-trip、`Mode()` 的 MODE_BYTE→`"TYPE L 8"` 映射、502→`OnUnSupportedFunction` 分類）。建於 `vclcompat/ClientSocket.h` 之上（控制通道一條連線+每次傳輸開一條 PASV 資料通道）。
- `FTPClient_EventHandlers.{h,cpp}`：`TfFTPClient` 的 NMFTP1 事件處理常式本體，改寫成符合 `TNMFTP` callback-slot 簽章的自由函式；需要 `CUSTOMER_CODE`/`MyDBIProcess`/`ShowMyMessage`。
- `FTPClient_Transfer.{h,cpp}`：4 個真正的 FTP 傳輸階段函式（`LoadFileFormServer2`/`UploadFileToServer2`/`Download_2DSortingList`/`Download_2DID_WhiteList`）。
- 3 個測試各自繞開 god-stack（自帶 TU-local stand-in 取代 `CUSTOMER_CODE`/`IniConfig`/`ShowMyMessage`/`MyDBIProcess` 等），比照 `test_config_loaders.cpp`/`test_MyCCLink.cpp` 既有慣例，避免與真本體發生 multiple-definition 連結衝突；production `ht9045_kyecftp` library 本身仍 PUBLIC-link 真依賴，用來證明這 3 檔對真標頭型別檢查過關。

**Automation 剩餘（3 檔）**：
- `auto9045.cpp/.h`：148 個 golden 自由函式，OLP/GPIB host-command 設定存取層，**確認非 VCL TForm**。觸及的 VCL 表單缺口（`fBinSel`/`fShowBinSelect`/`fTrayAssignment`/`fTestCategory`/`fContactCT`/`fCounterClear`/`fMonitor`/`fProductionInfo`/`fTemp_Set`等）暫由本檔自有 TU-local `W5FA_` 伴生物件承接，**本波刻意不併入** `FormsFacade.h`/`atester_shims.h`（需要全新共用表單類別、目前無其他消費端，留待未來整併波）。
- `AGV_E84.cpp/.h`：E84 loader/unloader 握手核心（`DoE84Loader`/`DoE84Unloader`，各 15/15 case label 忠實譯入）+ `ShowE84Log`/`E84StatusLog` 檔案記錄；`FormsFacade.h` 的 `TfAGV` 新增 `mmE84Log` 成員（連帶補上原本隱式的 ctor，改為顯式建構以初始化該成員）。
- `SCK_ART_Remainder.cpp/.h`：接續姊妹檔 `SCK_ART.cpp` 既有 8 函式抽取後的下一段連續 golden 切片（`SetSetupFilePath`/`AccessFile`/`SetGPIBVersion`/`UpdateCount`/`AddAlarmCode`/`ClearLotInfo`/`AddOutputJamCnt`/`SaveTestSummary`-dispatcher）。**刻意擁有自己獨立的 `SckArtRemainderState`**（非姊妹檔的 `SckArtState`）——留下明確的 3 方協調債（姊妹檔 `SckArtState` / 本檔 `SckArtRemainderState` / `csystem.cpp` 的 `W7C2_TfSCKARTSeam`，皆持有 `dCurrYield`/`iFTRTCount`/`iNeedRT`/`iInputCount`/`iManualRejectCnt`/`iTesterType`/`sLOTSTATUS`/`iCurrentStatus` 等同名欄位的各自副本），本波不解決（會需要動到姊妹檔內部，超出本前線範圍，記錄留給未來的 `TfSCKART`/FormsFacade 整併波）。

**Interface/TesterTCP_Socket.cpp/.h**：`Interface/TesterTCP.cpp` 的有界 socket 管理子集（Connect/Disconnect/Error/Read 事件常式、30-tick 重連計時器、`SendTCPIPCommand`/`AddTCPIPCommunicationLog`）；`FormsFacade.h` 的 `TfLotInfo` 新增 `labTCPIPStatus`/`mmTesterLog`。依賴本波新增的 `vclcompat/ClientSocket.h` `Tag`/`Open()`/`SendText()`/`ReceiveText()` 四個擴充 + `vclcompat/SysUtils.h` 新增的 `StringReplace`/`TReplaceFlags`。

**BarCode 收尾（`TfBarCode_Shim` 20/20 真本體達成）**：
- `BarCode_Shuttle2_ScanRemainder1.cpp/.h`：`DoBarcodeScanInShuttle_2`（Wave 1 誠實交接延後的 2 個方法之一，golden ~1222 行）。
- `BarCode_Shuttle2_ScanRemainder2.cpp/.h`：`DoShuttleFloatCheck_2`（另一個延後方法，golden ~474 行）。
- `BarCode_8CCD_Glue.cpp/.h`：BarCode.h 13 個 `__published TClientSocket*` 成員首次材料化為真正的 `Scktcomp::TClientSocket*` 物件（Shuttle1_A/B、Shuttle2_A/B、Bottom_1..8、BarcodeChangeFile）+ 真本體 `SendCCDCommand`（依 index 派發到 13 個 socket 之一，寫入 `Msg2+"\r\n"`，OCR 安裝走裸 `Msg2`）+ `TimerBotton8CCDConnectTimer`/`TimerBottom8CCDInitialTimer` 兩個計時器狀態機（前者含一個忠實保留的 golden copy-paste 缺陷）。
- **Integrate 決策**：既有 6 個各檔自帶的 gated `SendCCDCommand`-style 離線 stand-in（`BarCode_Bottom2DID.cpp`/`BarCode_Shuttle1_CCDScan.cpp`/`BarCode_Shuttle1_Scan.cpp`/`BarCode_Shuttle2_CCDScan.cpp`/`BarCode_Shuttle2_Scan.cpp`/`BarCode_Bottom2DID8CCD.cpp`）**刻意保持獨立、不重新導向到新的真 `SendCCDCommand`**：(1) 這 6 檔已是本前線範圍外、各自完成整合的既有產出；(2) 每個都有自己通過的 oracle 測試斷言其離線 no-op/log 行為，重新導向有零效益的行為改變風險；(3) 整併呼叫點是清理而非正確性修正，應留給有完整重驗證計畫的未來波次。

**SECSGEM `THGem` 前置切片（2 檔，尚無真消費端，等待未來完整 `uHGemEquipment.cpp` 波）**：
- `SecsWireCodec.{h,cpp}`：`THGem` 純位元組層 SECS-II item codec（`GetLengthOfType`/`GetLengthByte`/`GetSMLLenthByte`/`DataItemOut`×2/`DataItemInSub`/`DataItemIn`×2/`DataItemInNew`/`GetDataItemLenAndType(Sub)`/`StringOut`）+ 共用的 `HType`/`HTypeStruct`/`STypeStruct`/`HSMS_Head_Struct` 全域。零依賴（僅 vclcompat）。
- `SecsSvEcRegistration.{h,cpp}`：`THGem` 的 SV/EC 註冊 API（`SetSVDataPointer`×4/`SetECDataPointer`×4/`GetECDataValue`）。**刻意與 `SecsWireCodec` 分成獨立類別**（透過 `#include` 重用其 `HType` 全域、非繼承擴充）；依賴本波新增的 `vclcompat/TList.h`（見下）。

**vclcompat 新增/擴充**：
- `TList.h`（新檔）：VCL.Classes `TList` 泛型 `void*` list shim（`Add`/`Count`/`IndexOf`/`Items[]` 讀寫/`Delete`/`Clear`，non-owning 語意——絕不釋放持有的指標，區別於 `TObjectList`）。**刻意不 `using` 進全域命名空間**（`vcl_compat.h` 只加 `#include`，未加 `using vclcompat::TList;`）——因為 `aHotPlateSubstrate.h`（硬邊界共用檔）已存在一個**不相關**、範圍較窄的全域 `class TList`（僅 Add/Clear/Count/Items[] 唯讀），兩者同時可見會在全域命名空間重定義衝突（已實測編譯確認）；後續消費端（如未來 SECSGEM 波）需用完整限定名 `vclcompat::TList`，或自行在不含 `aHotPlateSubstrate.h` 的 TU 內加 `using`。
- `ClientSocket.h/.cpp`：新增 `Tag`（`int`，模擬真 VCL `TComponent.Tag`，供 BarCode 的 13 個共用 socket 與其事件處理常式透過 `(TClientSocket*)Sender)->Tag` 判斷是哪一個實際觸發）、`Open()`（`Active=true` 的薄別名，走同一條 `DoConnect_()` 私有路徑）、`SendText()`/`ReceiveText()`（原始位元組透傳，不附加/剝除行終止符——由呼叫端自行處理，同真 VCL `ScktComp` 語意）。
- `SysUtils.h/.cpp`：新增 `StringReplace` + `TReplaceFlags`/`TReplaceFlag`(`rfReplaceAll`/`rfIgnoreCase`)，支援 Borland `Set<>` `TReplaceFlags()<<rfReplaceAll` 疊加寫法；空 `OldPattern` 視為 no-op（防止無窮迴圈，同 Delphi 官方文件行為）。

### 主迴圈獨立複驗（2026-07-13，全新 from-scratch build、不沿用任何殘留 build_* 目錄）
- **Clean build exit 0**（新建 `build_resume_verify_20260713`，非重用任何舊 build 目錄）。
- **ctest 79/82 PASS**：14 個新增測試（`ClientSocketExt`/`BarCode_Shuttle2ScanRemainder1`/`BarCode_Shuttle2ScanRemainder2`/`BarCode8CCDGlue`/`TList`/`SecsWireCodec`/`SecsSvEcRegistration`/`Auto9045`/`AGV_E84`/`SCK_ART_Remainder`/`Interface_TesterTCPSocket`/`MiniFtpEngine`/`FTPClient_EventHandlers`/`FTPClient_Transfer`）**全數通過**；僅剩的 3 個失敗（`config_db`/`IniFiles`/`ini_helpers`）與前幾波完全同組——現場 `system/Gerneral.ini` 已被手動改成 `IO_CARD_TYPE=1`/`TTL_CARD_TYPE=0`/`HEATER_CTRL_TYPE=2`（測試 oracle 期待 `=2`/`=2`/`=4`），確認是既有環境漂移、非本波回歸。
- **BarCodeBottom2DID 疑慮已排除**：本波整合期間某個較早、範圍較窄的中間 verify build（`build_w5final_barcode_verify`，只結合 BarCode 前線自己 3 檔）曾出現 `test_barcode_bottom2did.exe` 於全部 12 項斷言 PASS 之後、程式結束時丟出 `__gnu_cxx::recursive_init_error` 崩潰。本次獨立複驗**沒有只看「這次沒崩潰」就結案**，而是用 `nm` 逐一核對最終合併後的實際連結符號：`BarCode_Bottom2DID.cpp` 的 `SendCCDCommand`/`AddCCDCommunicationLog`/`ClearBuffer` 全在匿名命名空間內（internal linkage、TU-local），從未 `#include BarCode_8CCD_Glue.h`、也從未呼叫新的外部真 `SendCCDCommand`；`ht9045_sm` 是純 static archive，`test_barcode_bottom2did.exe` 的相依閉包內沒有任何符號需要 `BarCode_8CCD_Glue.cpp`，故連結器的選擇性 archive-member 抽取從未把該 `.o` 拉進這顆執行檔——`nm` 實測確認（`test_barcode_bottom2did.exe` 內只有各檔自己的匿名/gated stand-in 符號，`test_barcode_8ccd_glue.exe` 才含有 13 個 `ClientSocket_Bottom_*` 全域+`MakeCcdSocket`）。回頭核對舊崩潰 log：崩潰點在全部 PASS 訊息印出「之後」（程式退出/靜態解構期，非測試中途），與「當時的中間連結圖把 `BarCode_8CCD_Glue.cpp` 一起拉進同一顆執行檔、13 個新 `Scktcomp::TClientSocket*` 全域解構順序衝突」的假設吻合——**結構性排除，非僥倖過關**。
- **mojibake 0**：本波 35 個新增/異動檔逐位元組掃描 U+FFFD，零命中。

### 下一步候選（隨時可續，不預設暫停）
SECSGEM 協定引擎本體（`uHGemEquipment.cpp`，需先設計 headless StringGrid helper + HSMS `TClientSocket`/`TServerSocket`，現在 `SecsWireCodec`/`SecsSvEcRegistration` 兩個前置切片已就緒可重用）；`uHGemHT9045.{h,cpp}`+`_SV`/`_EC` 站點覆寫層（全樹最寬表單 fan-out，需前者先完成）；`Interface/TesterTCP.cpp` 本體剩餘（70-85% VCL widget 穿插協定解析，`TesterTCP_Socket.cpp` 只取走了有界的 socket 管理子集）；`automation.cpp`(~5500行)；`AGV.cpp` E84 本體以外的其餘部分；或轉回 **W7**（csystem MainProc mode/SECS/AGV/temp dispatch ladder + cContact `CarlibrationTask`+6 sub-SM 下壓叢集）。

### 🔖 RESUME（最新）
- **✅ W5-Final 完成（2026-07-11 執行、2026-07-13 復原+獨立複驗+commit）**：KYECFTP(`ht9045_kyecftp` 新 library，3 檔)、Automation 剩餘(`auto9045.cpp` 148 函式、`AGV_E84.cpp` E84 握手核心、`SCK_ART_Remainder.cpp`)、`Interface/TesterTCP_Socket.cpp`(有界 socket 管理子集)、BarCode 收尾(`TfBarCode_Shim` **20/20 真本體達成**，`BarCode_8CCD_Glue.cpp` 13 真 TClientSocket)、SECSGEM `THGem` 前置切片(`SecsWireCodec`+`SecsSvEcRegistration`)、vclcompat 新增(`TList.h`+`ClientSocket` Tag/Open/SendText/ReceiveText+`SysUtils` StringReplace)。**流程教訓**：這整批工作在 2026-07-11 就已完成且各自驗證過，但因當時 session/workflow 中斷，主迴圈的合併複驗+文件+commit 三步從未執行，工作樹帶著 2 天未提交狀態——2026-07-13 接續時改用「直接查 git status/檔案時間戳/舊 build log 重建事實」而非相信任何殘留 agent 文字，這正是既有記憶「V906 workflow crash 復原」的又一實例，往後每次接續前應比照辦理。ctest 79/82(3 個既有無關環境漂移失敗、14 個新測試全過)、mojibake 0(35 檔)、BarCodeBottom2DID 疑似回歸經 `nm` 逐符號核對確認結構性排除。**下一步不預設暫停**：SECSGEM 協定引擎本體、TesterTCP.cpp 本體剩餘、automation.cpp、AGV E84 本體以外部分、或 W7 續。
- 驗證指令：`cd HT9011UC_Cpp_V3.33.906.0 && export PATH=/c/MinGW/bin:$PATH && cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=C:/MinGW/bin/g++.exe -DCMAKE_C_COMPILER=C:/MinGW/bin/gcc.exe && cmake --build build && ctest --test-dir build`。

---

## 2026-07-13 — SecsWireCodec Wave 2：THGem 訊框 header 組/解 + SML byte-stream 解碼器（15 函式 + 2 前置依賴）

**背景**：W5-Final 落地後，用 workflow 對剩餘候選（SECSGEM 協定引擎本體、`automation.cpp`、`Interface/TesterTCP.cpp` 剩餘、`AGV.cpp` 剩餘、`SCK_ART.cpp` 剩餘）做 5 路平行 recon + 1 個綜合決策 agent，選出**風險最低、槓桿最高**的下一波：延伸已落地的 `SecsWireCodec` 類別，而非直接啃整個 `uHGemEquipment.cpp`（9353 行、75% VCL 耦合）。刻意排除該檔真正高風險的兩塊（StringGrid CEID/Report 資料庫、TServerSocket 多連線+`Timer1Timer`連線生命週期 SM）——這兩塊都需要全新 vclcompat 元件設計，留給未來專屬設計波（且與 `automation.cpp` 需要的 TServerSocket 需求重疊，值得合併設計一次）。

**範圍**：延伸 `SECSGEM/SecsWireCodec.h/.cpp`（原地擴充，非新檔）：
- **Group A（訊框 header 組/解，~155 行）**：`ConvertLocalData`/`CreateLocalHead`/`InitLocalHead`（outbound：MSB-first 把 `Local` packed 進 `LocalBuffer[4..13]`、回填 4-byte 長度 header、W-bit 塞進 MessageID_S wire byte 最高位）+ `HostConvertToData`/`ProcessRemoteHead`（inbound 反向解析，解出 `RemoteLength`/`Remote`、unmask W-bit）+ `LocalAcknowledge`（組 1-byte BINARY ack/deny 回應）+ `StoreToReceiveString`。
- **Group B（byte-stream 解碼器/pretty-printer，~597 行）**：`ProcessSML`（最大函式，~410 行，遞迴 SECS-II item 解碼——LIST/ASCII/BINARY/BOOLEAN/UINT1-8/INT1-8/FT4-8 全分支）+ `ProcessSMLBinary`/`ShowSMLBinary`（平行的 raw hex dump 版本）+ `ShowSML`（頂層入口，含兩段式重試 quirk）+ `MakeSMLSpaceString`/`ShowSMLSpaceString`/`ShowSMLSpaceBinaryString`（縮排追蹤 pretty-printer）+ `ShowSFDescription`（180 項 `SFCodeAndMean[]` 靜態表查找，非 widget）。
- **另加 2 個直接被呼叫的前置依賴**（recon 未列入原 15 個，integrate 期間發現直接需要）：`StringBinaryOut`(golden :401-404，golden 本身就是空/註解掉的 body，忠實譯為真正 no-op)、`SendLocalData`(golden :1985-2107，`LocalAcknowledge` 會呼叫；因需要範圍外的 HSMS socket 通訊層而 GATE，只保留其唯一真副作用 `bReceiveData=false`)。
- 新增 instance state（golden THGem 成員，因 Group A 需要而變成真實成員，不再只是型別宣告）：`Local`/`Remote`(HSMS_Head_Struct)、`EquipmentSystemByte`、`RemoteLength`、`Host_HSMS_RunLength`、`bReceiveData`、`bDisableBinaryShow`、`DownLoadFilePtr`(non-owning)、`bOutputBusy`、`iSMLSpace`/`SpaceStr`，皆比照 golden THGem 建構子的初值(含未初始化欄位的忠實對應)。
- **4 個 gated widget stub**：`ProcessRemoteHead` 的 `GemCheckBoxShowHeadInformation->Checked`、`ShowSML` 的 `DB->Clear()`(TMemo)、`ShowSMLBinary`+`ShowSMLSpaceBinaryString` 共用的 `GemCheckBoxShowBinary->Checked`（2 處呼叫點）——全部只影響 trace/顯示輸出，不影響任何真實解碼/狀態變更邏輯，同既有 `SecsSvEcRegistration.cpp` 的 `Gated_*` idiom。
- **忠實保留的 golden 缺陷/quirk（不修正）**：`ProcessSML` 的 FT_8 分支先讀 `Ptr[]` 才做邊界檢查(golden :1839-1844，真實 1-byte OOB 讀取風險，逐字重現並加註記)；各數值分支的迴圈邊界寫法不一致(部分用 `ChangeToFloatNonPcnt`、部分用原始整數除法)；`ShowSFDescription` 的 `Pos()==2` 子字串比對仰賴表格遞增排序(非精確比對)+ 一個永遠命中不到的 `==""` 死碼分支；`ShowSML` 若第一次 `ProcessSML` 沒吃完整個 buffer 會再呼叫一次(非迴圈)；`SFCodeAndMean[]` 表格自身的重複標籤 bug(`[S5F73] Alarm Notification Send`/`Acknowledge` 都標 S5F73)。
- **Big5 資料表風險處理**：180 項 `SFCodeAndMean[]` 含 ~26 筆繁中字面值（**是資料非註解**，天真轉譯風險最高）。以 `cp950` 感知讀取 golden，額外寫一支獨立校驗腳本重新從 golden cp950→UTF-8 反推、逐位元組對照譯出檔——0 誤差，含雙重標籤 bug 與雙空白 quirk 皆忠實保留。

**測試**：`tests/test_SecsWireCodec.cpp` 從 109 條斷言擴充到 **203 條**。新增涵蓋每個新函式/分支的單元測試 + 1 個**真實擷取流量 oracle**（`D:\SECS_GEM_LOGS\2023_11_08\SECSGEM_TextLog_16.txt` 一筆真實 `[S6F11] Event Report Send`；因該 log 只留存 `ShowSML` 已解碼的 pretty-print 文字、不存在原始位元組擷取，故手動依該真實結構重建 26 個原始 SECS-II bytes，逐手核對 golden 遞迴/縮排邏輯後驗證 `ProcessSML` 精確重現該筆真實 log 的 11 行輸出，並確認 token 可透過既有 `DataItemInSub` 解回真實值 1/92/92——誠實標註這是「真結構+手工編碼」而非位元組級擷取 oracle，非杜撰）+ 1 個貫穿全部 15 個函式的手工端到端往返測試(`InitLocalHead`+`DataItemOut` 組訊息 → `ProcessRemoteHead`+`ProcessSML` 解回同一份 buffer，往返值一致)。

### 主迴圈獨立複驗
- **獨立 fidelity-review agent**（未參與翻譯、逐行對照 golden，非僅信任翻譯報告）：重新核對全部 15+2 個函式的 golden 行號引註（少數幾處 recon 估計值有 1-2 行漂移，均由 fidelity agent 重新確認校正值無誤）；`ProcessSML`(最大函式)逐分支核對控制流程/運算/位元組序完全相符，含 FT_8 OOB bug 與迴圈邊界不一致皆忠實保留；4 個 gated widget stub 確認只影響顯示輸出、不影響真實解碼/狀態邏輯；`SFCodeAndMean[]` 180 項獨立重新 cp950 解碼比對——0 誤差；建構子初值化逐一核對符合 golden。**結論：0 discrepancies，overall verdict = clean**。
- **獨立 build+ctest（全新 build 目錄，非沿用任何舊 build_* 殘留）**：clean build exit 0；**ctest 79/82**（`SecsWireCodec` 測試本身 203/203 全過；剩餘 3 個失敗與所有前波完全同組——`config_db`/`IniFiles`/`ini_helpers`，現場 `system/Gerneral.ini` 環境漂移，非本波回歸）；mojibake 0（3 個異動檔逐位元組掃描）。

### 下一步候選
本波刻意只做「延伸既有 codec 類別」這一小塊，**尚未**觸碰 `SECSGEM/uHGemClass.cpp` 的 57 個 gated virtual method（那些仍等待一個未來的「把 uHGemClass 的 gate 換成呼叫 SecsWireCodec/SecsSvEcRegistration 真本體」整合波，本波只是讓那個未來整合波變得可行，尚未執行）。真正的 `uHGemEquipment.cpp` 兩大剩餘難塊（StringGrid CEID/Report 資料庫 ~40 method、TServerSocket 多連線+`Timer1Timer` ~50 method）建議合併成一個專屬設計優先波（`automation.cpp` 也需要同一個 TServerSocket shim，值得一次設計共用）。或轉去 recon 已核實過的 runner-up：`Automation/AGV.cpp` 剩餘（recon 發現实際剩 ~1057 行/25 函式，比先前「幾乎translate完」的樂觀估計多，其中有 9 個函式是零新依賴的乾淨子集可先取）。或 W7 續。

### 🔖 RESUME（最新）
- **✅ SecsWireCodec Wave 2 完成（2026-07-13）**：`SecsWireCodec.{h,cpp}` 15 個 THGem 訊框 header 組/解 + SML byte-stream 解碼器函式 + 2 個前置依賴(`StringBinaryOut`/`SendLocalData`-gated)。僅動 3 檔(`SecsWireCodec.h`/`.cpp`/`tests/test_SecsWireCodec.cpp`)，CMake 無需改動(原地擴充既有 target)。獨立 fidelity-review 0 discrepancies、獨立 build+ctest 79/82(`SecsWireCodec` 自身 203/203)、mojibake 0。

---

## 2026-07-13 — 三前線同波：uHGemClass 解鎖 + AGV.cpp 剩餘 + 新 vclcompat TServerSocket shim

**背景**：使用者要求「workflow 火力全開」，本波刻意用 3 個平行 translate 前線（各自只碰自己的檔案）+ 1 個序列 integrate（統一改 CMake 共用檔）+ 1 個平行 verify（3 個獨立 fidelity-review + 1 個全新 fresh build），是本專案至今最大規模的單波編排（8 個 agent）。

**Front A — `SECSGEM/uHGemClass.cpp` 解鎖整合**：57 個 gated virtual method 逐一核對「golden 真實依賴鏈是否完全落在 `SecsWireCodec`+`SecsSvEcRegistration` 已覆蓋範圍內」，**誠實只解鎖 10 個**(非湊數)：`S9F1/S9F3/S9F5/S9F7/S9F9_*`(5 個同形狀的 StringOut+InitLocalHead+DataItemOut+SendLocalData)、`S2F26_DiagnosticLoopbackData`、`S2F42_Host_Command_Acknowledge`、`S2F44_ResetSpoolingAcknowledge`、`S2F24_TraceInitializeAcknowledge`、`CheckECValue`(忠實保留一個 golden 死碼分支：本該對應 FT_8 的重複 `FT_4_TYPE` 判斷式)。`HTGem` 新增 `SecsWireCodec WireCodec;`+`SecsSvEcRegistration SvEcReg;` 兩個 by-value 成員(RAII，非 new/delete)，取代原本呼叫 `HGemPtr->`/`HGem->`。**剩 47 個仍 gated**，按缺口分類標註（非空泛「等 THGem」）：`GetDataItemLenAndTypeAndDelete`(peek+consume 版本，尚未翻譯，~17 個方法最大宗)、VCL StringGrid 資料庫(`strGrdCEID`等 5 個)、VCL widget 型別缺口(`TCheckBox`/`TCheckListBox`等 5-6 個)、THGem-only 資料成員無 home(~12 個)、THGem-only 方法無 home(~18 個)、Borland `dos.h` 舊 API(`settime`/`setdate`，1 個)。`tests/test_uHGemClass.cpp` 71/71 全過。獨立 fidelity-review：0 個功能性差異，僅 2 處低嚴重度 cosmetic 引註行號誤差(S2F26/S2F44 各差 7-8 行)——**已由主迴圈修正**。**另修正**：Front A 翻譯 agent 誤把 59+2 處 AI 註解標成 V899 專用的 `AI(ht9045-v899)` 標籤(該標籤依 CLAUDE.md 慣例專屬 V899 BCB6 檔案，本檔是 V906)，主迴圈已全數改為 `AI(W906-uHGemClass-Unlock)`。

**Front B — `Automation/AGV.cpp` 剩餘**：recon 先前估計「~1057 行/25 函式」，實測修正為**13 函式/~730 行**（差額是 recon 誤把翻譯不到位的 VCL 設定編輯器 glue 與寬 widget 遙測也算進「剩餘」，實際兩者本就該延後）。新增 `Automation/AGV_PortScan.h/.cpp` + `tests/test_agv_portscan.cpp`（61 斷言全過）。**Group 1（零新依賴，9 函式/479 行）**：`NowE84Status`/`E84StatusChange`/`ScanLoadPort`(分派器)/`bIsStackBusy`/`bScanFixPortState_SPIL`/`bScanLoadPortState_Analog`/`bScanUnLoadPortState_Analog`/`bScanFixPortState_Analog`/`bScanLoadPortState_ATK`。**Group 2（需 3 個新 FormsFacade 成員，4 函式/~274 行）**：`bScanLoadPortState_SPIL`/`bScanUnLoadPortState_SPIL`/`DoE84LoaderScan`/`DoE84UnloaderScan`，新增 `TfLedValue`(共用 `{bool Value;}` stand-in，`TfMain::ALed1`+`TfLotInfo::ALedLoader`+`aLedAuto[3]` 共用)、`TfMain::labAutomation`、`TfLotInfo::palRemoveTray`。**刻意排除** `Timer2Timer`(唯一守門是 `fNote->fShow`，`TfNote` 全樹無 facade，新建一個表單類別成本大於效益且離線本就永遠不可達)；確認「ini 編輯器六件套」(`spbSaveClick`等)真的是獨立的 VCL 設定對話框 glue，非本波範圍延後合理。忠實保留 2 個 golden quirk：`E84StatusChange` log 表的 COMPT/CONT 標籤互換、`bScanLoadPortState_ATK` 內重複的 `MOT[MMTrayY].fHasTray` 檢查。獨立 fidelity-review：**0 discrepancies，clean**。

**Front C — 新 `vclcompat/ServerSocket.h/.cpp`（TServerSocket 多連線 shim，尚無真消費端）**：以 2 個具體 golden 呼叫點為根據（`uHGemEquipment.cpp::SendLocalData` 的 `srvGem`分支 :1985-2091；`automation.cpp` 的 `OLPServer`，`automation.h:37`+`.dfm:456-466`+多處 `Connections[i]->SendBuf`呼叫）設計 `TServerSocket`(`Port`/`Active`/`OnClientConnect/Disconnect/Read/Error`/`Socket`)+`TServerWinSocket`(`ActiveConnections`+`Connections[]`)。**關鍵發現迫使擴充既有 `ClientSocket.h`而非只加新檔**：golden 的 `clientGemRead` 事件常式**同時**被接到 `TClientSocket`的`OnRead`與`TServerSocket`的`OnClientRead`(`.dfm:550`+`:569`)——代表連線物件必須是**同一個**`TCustomWinSocket`可重用型別，不能另開平行型別；因此 `ClientSocket.h/.cpp`新增`Connected`/`LocalAddress`/`LocalPort`/`Handle`/`SocketHandle`欄位+`SetReadNotifyHook`/`SetCloseNotifyHook`/`StartReaderThread`/`AttachRealSocket_`四個 additive-only 掛鉤(絕不取代既有 `TClientSocket owner->OnRead` 路徑，只是額外觸發)。`tests/test_serversocket.cpp` 66/66 過(68 個 CHECK 中 2 個因設計上不可達而不執行)。

**獨立 fidelity-review 抓到 1 個真缺陷（medium severity，非 golden 保真度問題，是 shim 自身正確性 bug）**：`TServerSocket`的 Real-mode 拆卸(`DoClose_`/`~TServerSocket`)只關閉 socket、從未等待每條連線自己的背景 reader thread(`ReaderProc_`，卡在`recv()`)真正結束，就直接 `delete` 該連線物件——若 reader thread 在刪除當下仍在執行，會 dereference 已釋放記憶體(甚至透過其 closeNotify hook 回頭碰到同樣正在被析構的`TServerSocket`本身)。目前**休眠中**(尚無 Real-mode 消費端，只有 Sim-mode 測試)，但是真的 bug，非僅是文件化的簡化。**已由主迴圈修正**：新增 `TCustomWinSocket::StopReaderThread()`(比照 `TClientSocket::DoClose_`既有的 shutdown+`WaitForSingleObject`+`CloseHandle`序列，因為 `ServerSocket.cpp`是獨立 TU、無法直接碰 `ClientSocket.cpp`的 private `Impl`)，`~TServerSocket()`現在對**每一個**曾建立過的連線(不只是仍「存活」的)都先呼叫這個新方法才真正 `delete`；`DoClose_()`的既有迴圈也同步補上。`ServerSocket.h`原本「deferred-delete 就足以避開 UAF」的說法過於樂觀，已修正註解說明這只解決「同執行緒自我刪除」那一種 hazard，真正關閉 UAF 缺口的是這次新增的 join。

**Integrate 步驟自行抓到並修正 1 個真正的連結期整合 bug**：`AGV_PortScan.cpp`引用的 `bPortIsBusy`/`iLastPortStatus`/`bAskStopPort`/`bForceSendLoaderIsEmpty` 雖在 `cmydef.h`有`extern`宣告(能編過)，但其**唯一**定義卡在 `cmydef.cpp`一段 `#if 0 // TODO(W6)`尾端 gate 內(從未被編進任何目標)——link 期才會現形的 undefined reference。Front B 翻譯 agent 誤信「有 extern 宣告=已是 live 全域」，integrate 步驟實際嘗試連結後抓到，比照 `acatchtray_shims.cpp`既有的`iThisPortNo`/`iThisPortStatus`/`iPortStatus`兄弟寫法，補上這 4 個定義(引用真實 golden `cmydef.cpp:5916/5917/5921/5924`)。CMake 改動：`AGV_PortScan.cpp`加入`ht9045_sm`(緊接既有`AGV_E84.cpp`後)、`ServerSocket.cpp`加入`vclcompat`(緊接`ClientSocket.cpp`後)、`tests/CMakeLists.txt`新增`test_agv_portscan`+`test_serversocket`兩個執行檔(比照`test_agv_e84`/`test_clientsocket_ext`既有寫法)。3 前線間零 ODR/命名衝突（逐一核對確認）。

### 主迴圈獨立複驗（含親自動手修 Front C 的 UAF bug + Front A 的標籤/引註誤差，非僅信任 agent 報告）
- 3 個獨立 fidelity-review agent 逐一核對 golden 引註與行為：Front A(2 低嚴重度 cosmetic 已修)、Front B(0 discrepancies)、Front C(1 medium severity 真 bug 已修)。
- 主迴圈**親自**（非再派 agent）用本機 `g++ 6.3.0`+`cmake`全新建置目錄複驗：clean build exit 0；**ctest 81/84**(僅 3 個既有無關 `config_db`/`IniFiles`/`ini_helpers` 環境漂移失敗，同前幾波)；`uHGemClass`/`ServerSocket`/`AGV_PortScan`三個新/擴充測試逐一確認 PASS；mojibake 0(含親自修改的 6 個檔案：`ClientSocket.h/.cpp`/`ServerSocket.h/.cpp`/`uHGemClass.h/.cpp`)。

### 下一步候選
`uHGemClass.cpp` 剩餘 47 個 gated method 按缺口分類，其中 `GetDataItemLenAndTypeAndDelete`(peek+consume 版本)是最大單一槓桿——翻完它可望再解鎖一批（~17 個方法的共同缺口）；`AGV.cpp` 已無更多可翻(剩下的都是明確延後的 VCL glue)；`uHGemEquipment.cpp` 剩餘兩大難塊(StringGrid CEID/Report 資料庫、Real-mode `srvGem`+`Timer1Timer`連線生命週期，`TServerSocket`shim 現已就緒可用)；`automation.cpp`(`OLPServer`的 `TServerSocket` shim 依賴現也已就緒)；`Interface/TesterTCP.cpp`/`SCK_ART.cpp` 剩餘；或 W7 續。

### 🔖 RESUME（最新）
- **✅ 三前線同波完成（2026-07-13）**：`uHGemClass.cpp` 10/57 解鎖(WireCodec+SvEcReg 埋為 by-value 成員)、`AGV.cpp` 剩餘 13 函式全清(zero-dep 9 + FormsFacade-need 4，僅存明確延後的 VCL glue)、新 `vclcompat/ServerSocket.h`(TServerSocket 多連線 shim，尚無真消費端，供 SECSGEM Real-mode 與 `automation.cpp` OLPServer 未來重用)。3 個獨立 fidelity-review 抓到並經主迴圈修正：Front C 的 TServerSocket 拆卸 UAF race(medium，已修)、Front A 的 2 處 cosmetic 引註誤差(已修)、Front A 的 V899/V906 AI 標籤誤植(59+2 處已改名)；integrate 步驟自行抓到 1 個真連結期 bug(4 個假 extern 全域缺定義，已修)。主迴圈親自(非再派 agent)完成最終獨立複驗：ctest 81/84(3 個既有無關環境漂移失敗)、mojibake 0。**下一步不預設暫停**：`uHGemClass.cpp` 剩餘 47 個 gated method(先攻 `GetDataItemLenAndTypeAndDelete` 這個最大共同缺口)、`uHGemEquipment.cpp`/`automation.cpp` 的 `TServerSocket` 消費端整合(shim 已就緒)、`Interface/TesterTCP.cpp`/`SCK_ART.cpp` 剩餘、或 W7 續。

---

## 2026-07-16（執行）/ 2026-07-17（補記+獨立複驗+commit）— Automation 網路層 + TesterTCP OS-recipe 對 + uHGemEquipment 首建(StringGrid CEID/Report 資料庫) + uHGemClass 再解鎖 3 個

**背景（第三次 workflow crash 復原案例）**：本波實際翻譯工作於 2026-07-16 14:49–16:32 執行完成（`build_w906_fire_verify_20260716` 全新 build exit=0，各檔皆有詳盡的 integrate-agent 註解），但「跨戰線合併複驗＋DEVLOG/ROADMAP 記錄＋commit」三步同樣從未執行，工作樹留下未提交、未記錄狀態——與 2026-07-13 那次(W5-Final)同一種情境，是既有記憶「V906 workflow crash 復原」的第三個實例。2026-07-17 接續時，比照既有慣例：不信任何殘留 agent 文字，改用 4 路平行稽核 workflow 逐檔重新核對 golden 引註/函式數/gate 狀態後才動筆記錄，另重跑 ctest 與 mojibake 掃描做獨立複驗。

**`Automation/automation.cpp/.h`（新，`TfAutomation` OLP 主機指令引擎，golden 2501+153 行）**：翻譯 44 個成員函式 + 6 個自由函式（`SplitDataBySoh`/`GetCRC`/`AddBlock`/`IsStartWord`/`crc16`/`charcmp`），其中 41 個為真實忠實翻譯（建構子客戶碼分支、event API 家族 `GetEventNum/SetEventResult/GetEventResult/CompareEvent/ClearEvent/GetEventStrResult`、`CommandProcess`/`SendClient`/`SendServer`、`OLPServer`/`OLPClient` socket event handler、`tmrOLPTimer`、`Show*`/`SaveRecord` 記錄、`GetHTMLJSONDatas`/`GetMachineStatus`/`GetWorkOrder`/`GetMainTemp`/`GetTemperatures`）。**3 個刻意留空殼 stub**：`ProcessBuffer`(golden :951-1979，~1029 行)、`ProcessBuffer1`(:1980-2045)、`SendReportRequest`(:2046-2131)——三者合計約佔 golden 全檔 47%，是 OLP 命令派工/上報的真正核心，明確標註 `// GATED (deferred future wave)` 並附精確 golden 行號。NMUDP1/Amkor-UDP 6 個方法與虛擬鍵盤 handler 整組未宣告，刻意排除（無明確消費端）。真本體單例刻意命名 `fAutomationEngine`(非 `fAutomation`)以避開既有 `atester_shims.h` 的 `TfAutomationShim` ODR/命名衝突。

**`Interface/TesterTCP.cpp/.h`（新，`TfTesterTCP` 剩餘切片之二，與既有 `TesterTCP_Socket.cpp` 互斥不重疊）**：僅 3 個函式、golden 1117 行中的 58 行——`TesterTCP_CopyRecipeToTester`/`TesterTCP_CopyRecipeFromTester`(golden :1058-1097，OS recipe zip 收發對)、`TesterTCP_btnSaveClick`(:1099-1115，3 個 `TCheckBox->Checked` + `LastFileName` 因無 FormsFacade widget home 而升格為明確參數)。翻譯期間順手修正 1 個真行為偏差：golden 的非同步 `ShellExecute` 先錯譯成同步 blocking `system()`，複驗時發現並改為 `start "" /B` 非阻塞啟動。此波仍**明確不含**該檔真正的協定解碼核心 `TimerProcessTCPDataTimer`（需 5 個新 FormsFacade 成員 + 整個未翻譯的 `WritePERSITETemperature` 姊妹函式）、ctor/FormDestroy/FormShow/`rgUnloaderClick`（~140 行 widget 建構群）、以及卡在 `Public/MyProductionRecord.cpp`（`GetSiteNo`/`GetOrderOfContact` 缺口未翻譯）的 `CopyOSTestResult`/`PlaceOSTestResultToTray`。

**`vclcompat/StringGrid.h/.cpp`（新，headless `TStringGrid` shim）+ `SECSGEM/uHGemEquipment.cpp/.h`（新，`THGem` 類別首次建立）**：新建 0-based `Cells[ACol][ARow]` + 可變 `RowCount`/`ColCount` 的最小 headless StringGrid（越界丟 `std::out_of_range`，刻意比照 VCL `ERangeError` 而非自動擴張/靜默夾住——這是重現下方 golden bug 的關鍵設計）。`THGem` 首建覆蓋 ROADMAP 點名的 **CEID/Report/Alarm StringGrid「資料庫」家族共 26 個方法**：`SetCEIDContent`(2 多載)/`GetCEIDContent`/`SetReportIDContent`/`DeleteReportID`/`DeleteReportIDOfCeid`/`DeleteAllHostDefineReportID`/`GetReportIDContent`/`IsEnableEvent`/`CheckCeidExist`/`CheckCEIDExist`/`GetReportIDType`/`CheckCeidAlreadyDefine`/`CheckReportIDExist`/`AddCeidReportID`/`DeleteAllHostReportIDOfCeid`/`DeleteHostReportIDOfAssignCeid`/`DeleteAllHostDefineCeid`/`CheckReportAlreadyDefine`/`EnableDisableEventReport`(+gated stub `EnableDisableEventReportAcknowledgeError`)/`ReadEventReportData`/`SaveEventReportData` + tab-format `CopyStringGridAsTabFormat`/`PasteStringGridAsTabFormat` + 3 個 Alarm-grid 姊妹 `SetAlamData`/`ReadAlamData`/`WriteAlamData` + `GemTimer` 小工具。**忠實保留 2 個 golden bug**：`GetCEIDContent`/`GetReportIDContent` 內迴圈誤用 `->RowCount` 而非 `->ColCount`(提早截斷)；`ReadAlamData` 例外路徑 use-after-free(`memoPtr` 在 `catch` 內 delete 後，unconditional tail 區塊仍 dereference)。`ReadEventReportData`/`SaveEventReportData` 寫死正式機路徑 `D:\HT9045\SECS\SECS\SYSTEM\EventReport_*.def`(未走 `GemSystemPath`)，`SaveEventReportData` 標註「DANGEROUS IF EXECUTED AS-IS」，測試刻意不呼叫任何會觸達它的 3 個方法以避免真的寫到機台正式路徑外的檔案。`TStringGrid` 建構子本身有一個 2nd/3rd `new` 拋例外會洩漏的 exception-safety 缺口，integrate 時發現並用 try/catch 補上。**明確未觸碰**：`TServerSocket` 多連線層 + `Timer1Timer` 連線生命週期狀態機(~50 method 規模，2026-07-13 就緒的 `vclcompat/ServerSocket.h` shim 仍待這裡消費)、wire codec 成員化(`SecsWireCodec`/`SecsSvEcRegistration` 尚未嵌入 `THGem`)。

**`SECSGEM/uHGemClass.cpp/.h` + `SecsWireCodec`（Wave 2/3 整合）**：再解鎖 3 個 virtual method(10/57→**13/57**)：`S7F2_ProcessProgramLoadGrant`(golden :2081-2113)、`S2F15_UpdateNewEquipmentConstant`(:2884-3024，忠實保留 1 個 golden 記憶體洩漏：ASCII 分支 `new char[]` 從未 `delete[]`)、`S2F15_CheckNewEquipmentConstant`(:3026-3190)——三者共同缺口 `GetDataItemLenAndTypeAndDelete` 現已解決。`SecsWireCodec` 為此新增 3 個方法：`GetDataItemLenAndTypeAndDeleteSub`(golden :2453-2462)、`GetDataItemLenAndTypeAndDelete`(:7099-7106)、`SendInvalidDataMessageToHost`(:7353-7358，S9F7 造型的畸形封包回覆)。額外評估 8 個「伸展目標」method，僅 `SendInvalidDataMessageToHost` 乾淨可翻，其餘 7 個(`CheckSFFormatOnlyHead`/`IsValidSVID`/`GetTimeInfo`/`DeleteAllHostDefineReportID`/`DeleteAllHostDefineCeid`/`GetAlarmIndex`/`EnableDisableAlarmAll`/`EnableDisableAlarm`/`EnableDisableECDataAll`/`EnableDisableECData`)確認仍卡在 VCL `TStringGrid`/`TCheckBox` 或 THGem-only 成員，誠實回報而非硬翻。`uHGemClass.h` 新增 16 行純註解：核對過剩餘 44 個 gated method 皆未被新 `THGem` 切片解封，故決定暫不把 `class THGem;` 前置宣告換成 `#include "uHGemEquipment.h"`。`SetECValue` 仍 gated（VCL widget-cast 叢集：`TPanel`/`TCustomEdit`/`TComboBox`/`TLabel`/`TCheckBox`/`TRadioGroup`，vclcompat 皆無）。

**支援性變更**：`FormsFacade` 新增 `TfMain` 成員 `palMainStatus`/`cbSetupFileName`/`edWorkTemperBase` + 方法 `Home(AnsiString Func)`(offline 恆回 false)，供 `automation.cpp` 的 `GetMachineStatus`/`GetWorkOrder`/`GetMainTemp` 及仍 gated 的 `ProcessBuffer` 使用；`canary_support.h` 新增 `LAST_GENERAL_SET.OLPSetBinErr[10]`(golden `LastSet.h:472`，供仍 gated 的 `ProcessBuffer` 用，目前未被實際引用)；`vclcompat/SysUtils.cpp` 的 `DirectoryExists` 修正一個真 bug——MinGW `stat()` 對帶尾端 `/`/`\` 的路徑會誤判不存在，導致 `DataPath`/`sSaveByMachine` 這類全域路徑（恆帶尾端分隔符）永遠假陰性，由 `test_TesterTCP.cpp` 複驗時發現並修正（strip 尾端分隔符後再 `stat()`）；`CMakeLists.txt`/`tests/CMakeLists.txt` 新增對應 lib 來源與 3 個新 test target(`test_automation`/`test_TesterTCP`/`test_uHGemEquipment`)，`test_TesterTCP` 移除了自帶的 `MySleep` 本地 stub(因連結 `ht9045_sm` 會拉入 `acarry_shims.cpp` 的真定義，避免 multiple-definition)。

### 主迴圈獨立複驗(2026-07-17，4 路平行稽核 workflow + 親自重跑)
- 4 個獨立稽核 agent 逐檔核對 golden 引註、函式數、gate 狀態(不信任殘留 agent 文字)：確認以上函式數/行號/bug 保留清單。
- 親自(非再派 agent)在既有 `build_w906_fire_verify_20260716` 重跑 `ctest`：**83/87 通過**，失敗的 4 個(`config_db`/`IniFiles`/`ini_helpers`/`config_loaders`)與歷次波次同組——現場 `system/Gerneral.ini` 環境漂移，非本波回歸。
- mojibake 掃描(18 個新/異動檔)：**0**。

### ROADMAP DEFERRED 表更新（僅窄化，無一列可關閉）
- `Automation/automation.cpp`(:134)：剩 `ProcessBuffer`/`ProcessBuffer1`/`SendReportRequest`(~1181 行 OLP 命令派工核心) + NMUDP1/Amkor-UDP 6 方法 + 虛擬鍵盤 handler。
- `Interface/TesterTCP.cpp`(:127)：剩 ctor/FormDestroy/FormShow/`rgUnloaderClick`、`CopyOSTestResult`/`PlaceOSTestResultToTray`(卡 `MyProductionRecord.cpp`)、`SimulateBin`/`ProcessOSPrint`/`ProcessOSTrayData`、**`TimerProcessTCPDataTimer`(真正協定解碼核心，仍完全 VCL/FormsFacade 卡關)**。
- `SECSGEM/uHGemEquipment.cpp`(:124)：StringGrid 前置需求+CEID/Report-ID 資料庫子集已交付；剩 `TServerSocket` 多連線層 + `Timer1Timer` 連線生命週期 SM(~50 method)。
- `SECSGEM/uHGemClass.cpp`(:123)：47→**44** 個仍 gated(`GetDataItemLenAndTypeAndDelete` cluster 3/17 已解)。

### 下一步候選
`uHGemEquipment.cpp` 的 `TServerSocket` 多連線 + `Timer1Timer` 連線生命週期 SM(shim 已就緒，且是 `automation.cpp` `OLPServer`/`uHGemClass.cpp` 多個 gated method 的共同前置)為目前最高槓桿候選；`uHGemClass.cpp` 剩 44/57(多數卡 VCL widget/StringGrid-alarm/SV-EC 註冊家族，需逐個評估)；`Interface/TesterTCP.cpp` 的 `TimerProcessTCPDataTimer`(需先設計 5 個 FormsFacade 成員)；`automation.cpp` 的 `ProcessBuffer` 派工核心(需先解 `TServerSocket` 消費端，本質與 uHGemEquipment 同一個前置)；或 W7 續。**下一步不預設暫停**。

### 🔖 RESUME（最新）
- **✅ Automation 網路層 + TesterTCP OS-recipe 對 + uHGemEquipment 首建 + uHGemClass 再解鎖 3 個完成（2026-07-16 執行、2026-07-17 補記+獨立複驗+commit，第三次 workflow crash 復原案例）**：新增 `Automation/automation.cpp/.h`(44 method+6 free func，41 真譯/3 gated stub)、`Interface/TesterTCP.cpp/.h`(3 函式)、`vclcompat/StringGrid.h/.cpp`(headless shim)、`SECSGEM/uHGemEquipment.cpp/.h`(`THGem` 首建，CEID/Report/Alarm StringGrid 家族 26 method)；`uHGemClass.cpp` 10/57→13/57、`SecsWireCodec` +3 method。ROADMAP :123/:124/:127/:134 四列窄化(無一關閉)。4 路平行稽核 + 親自複驗：ctest 83/87(4 個既有無關環境漂移失敗)、mojibake 0(18 檔)。**下一步不預設暫停**：`uHGemEquipment.cpp` 的 `TServerSocket`+`Timer1Timer`(最高槓桿，shim 已就緒)、`uHGemClass.cpp` 剩 44/57、`TesterTCP.cpp` 的 `TimerProcessTCPDataTimer`、`automation.cpp` 的 `ProcessBuffer`、或 W7 續。

---

## 2026-07-17 — uHGemEquipment 連線生命週期 Bucket A（clientGem/srvGem 開通+連線事件家族，`TServerSocket`/`ClientSocket` shim 首次真消費端）

**背景**：延續上一條目「下一步」點名的最高槓桿候選。先跑一個獨立 recon agent 逐段讀 golden `uHGemEquipment.cpp` 的連線生命週期區(建構子連線狀態初始化、`clientGem*`/`srvGem*` event handler、`DoOpenCommuncation`、`DoOnLine`、`Timer1Timer` 本體、`Connect`/`DisConnect` 家族)，依「現在可翻(A)/需 1-2 個小 widget(B)/深層卡關(C)」三桶分類。**誠實結論：ROADMAP 原估的「~50 method TServerSocket+Timer1Timer」範圍其實橫跨三桶，`Timer1Timer` 本身與 `DoConnect`/`DoSelect`/`DoSeparate`(需 `SecsWireCodec` 成員化)、`DoProcessSFNoResponse`(需未設計的 `TCriticalSection` shim)、`clientGemRead`/`ProcessSocketReceiveData`(需未設計的 `TMemoryStream` shim，是「真正協定解碼核心」)全部落在 C 桶，翻空殼没有測試價值——本波刻意只吃 A 桶**，B 桶(`InitialHGem`/`SaveSystemDefault`/`DoUpdateStatus`/`ProcessShow`/`FormClose`/`ManualCreatergRoleClick`/`FormShow`，需 ~15 個 `TEdit`/`TRadioGroup`/`TCheckBox`/`TPanel`/`TSpeedButton`/`TColor` 小型 stand-in)留給下一個近期波，C 桶留給更遠的未來。

**範圍(A 桶，golden 行號見 recon)**：header 新增 `clientGem`(`TClientSocket*`)/`srvGem`(`TServerSocket*`) + 連線狀態旗標(`bConnect`/`bOnLine`/`bOnLineLocal`/`bAutoConnect`/`bStartConnect`/`bStartOnLine`/`bTCPIP_Error`/`bServoSocketConnect`/`bReceiveMultiConnect`)+ 計數器 + 2 個 `GemTimer`(既有型別重用)+ `WaitShowString`/`LogDataString`(`TStringList*`)+ `TimeString`/`GemClock`/`SystemYear..MSec`。翻譯：`clientGemConnect`/`Disconnect`/`Error`/`Connecting`、`DoOpenCommuncation`、`OnlineLocalOrRemote`(golden 兩分支皆死碼，忠實保留惰性)、`DoOnLine`、`CheckSocketActiveFalse`、`CloseCommuncation`/`Connect`/`DisConnect`/`IsConnect`/`SetEstablishCommunicationsTryCount`/`OnLine`/`OnLineLocal`/`OnLineRemote`/`OffLine`/`IsOnLine`/`GetOnLineMode`/`SetCanAcceptHostOnLineRequest`(golden 空body)、`srvGemClientConnect`/`GetSocketErrorMsg`/`srvGemClientError`/`srvGemClientDisconnect`、`StringOut`(1-arg)/`StringBinaryOut`(golden 全註解=真no-op)、`ClearDefaultEvenReport`(免費項，只呼叫既有已翻方法)、`GetTimeInfo`(僅 TimeString/GemClock 半段，磁碟/記憶體輪詢尾段依既有 `EnableDisableEventReportAcknowledgeError` gated-stub 先例原樣 gate)、`SaveSECSGEMErrToLog`(`TDateTime::FormatString`→`FormatDateTime` 語法適配；`MyForceDirectories` 依 `Interface/TesterTCP.cpp` 既有 `Gated_MyForceDirectories` TU-local 手法複製簡化版)。**這是 `vclcompat/ServerSocket.h`(2026-07-13 就緒、一直沒有真消費端)的第一個真消費端**。

**忠實保留的 golden 安全關鍵行為**：`DoOpenCommuncation` 對 `srvGem->Open()` 恆用 `!srvGem->Active` 守門，絕不無條件 Close()+Open()；`srvGemClientDisconnect` 刻意**不**呼叫 `srvGem->Close()`——golden 原處有一段「斷線重連的秘密」註解警告在此處 close+reopen 會當掉，忠實翻譯此段警告註解供未來翻 `Timer1Timer`/`DoConnect` 的人看到。`SetCanAcceptHostOnLineRequest`(golden 空body)、`StringBinaryOut`(golden 全註解)、`OnlineLocalOrRemote`(golden 兩分支皆死碼)三者忠實保留惰性，不「順手補完」。

**翻譯期間發現並修正 1 個真 use-after-free**：原始 `~THGem()` 先刪 `WaitShowString`/`LogDataString` 才刪 `clientGem`/`srvGem`；但 `~TServerSocket()` 可能同步觸發 `OnClientDisconnect`(接到 `srvGemClientDisconnect`，內部呼叫 `StringOut`，會 deref 已刪除的 StringList)。已改為先刪 sockets 再刪 StringList，並在解構子留註解說明順序原因。

**測試衛生修正（使用者澄清後）**：`srvGemClientError` 忠實呼叫 `SaveSECSGEMErrToLog`，該函式對 golden 的硬編碼絕對路徑 `D:\SECS_GEM_LOGS\<yyyy>\<mm_dd>\SECSGEM_ErrLog_<hh>.txt` 無條件寫入——這是 repo 外、非 git 管控的真實歷史 log 歸檔（本專案其他波次曾用它當唯讀 oracle）。第一次跑測試時意外真的寫入 2 行到 `2026\07_17\SECSGEM_ErrLog_10.txt`。**使用者裁示**：開發/測試時使用這個真實路徑沒關係，但測試跑完不能讓它變得更髒——因此保留當時已寫入的 2 行作為基準，並修正測試 `test_srvgemclient_handlers`：呼叫前用同一套 `yyyy`/`mm_dd`/`hh` token 算出目標檔案路徑、讀出原始 byte 內容，呼叫後無條件寫回(檔案原本不存在則整個刪除、視情況一併清掉新建的空資料夾)。已實測連跑 2 次確認 log 檔內容前後 byte-for-byte 相同。

### 獨立複驗
- 獨立 fidelity-review agent(未參與翻譯)：逐一核對 28 個函式的 golden 行號引註、安全關鍵行為(open-guard/不 blind-reopen)、3 個刻意惰性函式未被「順手補完」、解構子順序修正、2 個 TU-local stand-in 的正確性、測試衛生修正的實際正確性(親自編譯執行、對照 `D:\SECS_GEM_LOGS` 前後 byte 數確認)。**結論：0 discrepancies，clean**。
- 親自(非再派 agent)全新 build 目錄(`build_conn_lifecycle_translate`)：clean build，**ctest 83/87**(同組 4 個既有 `system/Gerneral.ini` 環境漂移失敗，非本波回歸)；mojibake 掃描異動檔：**0**。

### ROADMAP DEFERRED 表更新
- `SECSGEM/uHGemEquipment.cpp`(:124)：連線開通 A 桶(clientGem/srvGem 生命週期、connect/disconnect/error handler)已交付，是 `ServerSocket.h` shim 首個真消費端。**仍未觸碰**：B 桶(`InitialHGem`/`SaveSystemDefault`/`DoUpdateStatus`/`ProcessShow`/`FormClose`/`ManualCreatergRoleClick`/`FormShow`，需 ~15 個小型 widget stand-in)、C 桶(`Timer1Timer`本體、`DoConnect`/`DoSelect`/`DoSeparate`、`DoProcessSFNoResponse`、`clientGemRead`/`ProcessSocketReceiveData`——真正的協定解碼核心，需未設計的 `TCriticalSection`/`TMemoryStream` shim)。

### 下一步候選
B 桶(~15 個小 widget stand-in，解鎖 `InitialHGem`/`SaveSystemDefault`/`DoUpdateStatus`)是自然的下一個小波；C 桶的 `clientGemRead`/`ProcessSocketReceiveData`/`Timer1Timer` 是本檔案最終、最大的剩餘難塊，需要先設計 `TCriticalSection`/`TMemoryStream` shim + `SecsWireCodec`/`SecsSvEcRegistration` 成員化。或轉 `uHGemClass.cpp` 剩 44/57、`TesterTCP.cpp` 的 `TimerProcessTCPDataTimer`、`automation.cpp` 的 `ProcessBuffer`、或 W7 續。**下一步不預設暫停**。

### 🔖 RESUME（最新，已被下一條目取代，見下方新 RESUME）
- **✅ uHGemEquipment 連線生命週期 Bucket A 完成（2026-07-17）**：新增 `clientGem`/`srvGem` 成員+連線狀態旗標、翻譯 28 個函式(`clientGem*`/`srvGem*` event handler、`DoOpenCommuncation`、`DoOnLine`、`OnlineLocalOrRemote`、`Connect`/`DisConnect`/`OnLine`/`OffLine` 家族、`ClearDefaultEvenReport`、`GetTimeInfo`半段、`SaveSECSGEMErrToLog`)，`ServerSocket.h` shim 首個真消費端。翻譯期間修正 1 個真 use-after-free(解構順序)。測試意外寫入真實 `D:\SECS_GEM_LOGS` 後，依使用者裁示修正測試做 capture/restore，確認連跑 2 次 byte-for-byte 不變。獨立 fidelity-review 0 discrepancies、親自複驗 ctest 83/87(4 個既有無關失敗)、mojibake 0。

---

## 2026-07-17 — uHGemEquipment 連線生命週期 Bucket B（~15 個小型 widget stand-in，InitialHGem/SaveSystemDefault/DoUpdateStatus/ProcessShow/FormClose/ManualCreatergRoleClick/FormShow）

**背景**：緊接 Bucket A 完成後同日繼續（使用者要求「火力全開不要停」）。範圍延用先前 recon 的 B 桶清單。

**範圍**：新增 7 個 widget stand-in struct(`THGemRadioGroup`/`THGemEdit`/`THGemCheckBox`/`THGemComboBox`/`THGemPanel`/`THGemSpeedButton`/`THGemMemo`，`TMemo` 版本內部真的用 `vclcompat::TStringList` 承接內容而非 no-op)+ `TColor` 型別 + `clRed`/`clLime`/`clYellow` 常數 + 全域單例指標 `extern THGem *HGem`(golden 本來就用這個全域指標而非 `this`)。翻譯：`StringOut`(2-arg,golden 409-417)、`InitialHGem`(5011-5099)、`SaveSystemDefault`(5101-5140)、`ProcessShow`(5165-5173)、`DoUpdateStatus`(4747-4985，含 KYEC 30 秒強制斷線分支)、`FormClose`(6887-6892)、`ManualCreatergRoleClick`(6915-6931)、`FormShow`(6936-6950，僅譯有實際行為的兩行；`Left/Top/PageControl1` 純 UI 座標視為此非真表單port的天生無 home，略過)、外加補上 Bucket A 遺留的 `srvGemClientConnect` 內 `TerminalMemoPtr` 分支(golden 6832-6835，Big5 訊息「有2台以上EAP連接Handler,請確認」逐位元組 cp950 解碼核對無誤)。

**依賴擴大（誠實記錄，非隱性擴權）**：本波需要 `cmydef.h`(`CUSTOMER_CODE`/`CC_KYEC_LEE`/`CC_SIGURD_ChungXing`/`CC_MAXIM_THAILAND`/`CosFunction`/`bSECSGEMbyPass`/`bSECSGEMConnectionFail`)+ `common.h`(`ReadWriteIni`/`ReadIniData`/`WriteIniData`)，因此 `ht9045_secsgem` 首次連結 `ht9045_core`+`ht9045_globals`——比照既有 `KYECFTP/FTPClient_Transfer.cpp` 同形狀先例，且獨立複驗確認 `ht9045_core`/`ht9045_globals` 皆不回頭連結 `ht9045_secsgem`/`ht9045_public`，無循環依賴。

**⚠️ 重要限制（KYEC 客戶實際部署前必讀）**：`DoUpdateStatus` 的 KYEC 30 秒強制斷線分支(golden :4805-4836，`CUSTOMER_CODE==CC_KYEC_LEE`)需要一個 `HTimer`(`bSECSGEM_DoSeparate`/`SECSGEM_DoSeparate`，golden 本身也是全域變數非 THGem 成員，已忠實對應)。本波沿用 `atester_shims.h` 既有的 `HTimer` stand-in——**該 stub 的 `Off()` 恆回 true(永遠報告「已到時」)**。`atester_shims.h` 原本的用途是一個很少被觸及的 gated 路徑，這個「恆真」瑕疵基本無害；**但這是本波第一次讓它從一個真實、無條件的客戶分支(KYEC)被觸發**——意味著這個翻譯版本目前對 KYEC 客戶不會真的等 30 秒，而是每次輪詢就立刻觸發強制斷線動作。已在程式碼三處(HTimer 定義處、KYEC 呼叫點、test [22])清楚註記，**非隱藏瑕疵，但在真正對 KYEC 機台上線前，必須先設計一個會真的計時的 `HTimer`**。

**忠實保留的 golden bug**：`DoUpdateStatus` 的 client-active 分支即使 socket 真的變 Active 仍強制 `bConnect=false`(golden :4771-4776)；`StringOut(S,C)` 的顏色參數 `C` 全檔未使用(golden 本身如此)；`ManualCreatergRoleClick` 對應的 `rgRole->OnClick` 動態指定(golden 建構子:653)因 stand-in 無 callback slot 而未還原，已記錄為已知落差。

### 獨立複驗
- 獨立 fidelity-review agent：逐一核對 8 個函式的 golden 行號、KYEC/HTimer 限制的真實性與註記完整度、EventReport 重用正確性(沿用既有 `SecsEventReport.h` 而非另起爐灶)、Big5 訊息逐位元組核對、測試是否誤觸真實外部路徑(確認皆用 scratch 路徑，未重蹈 Bucket A 覆轍)、依賴擴大是否合理對應實際用量。**結論：clean，僅 1 處文件措辭上的小提醒**(review 指出「這是既有已知限制」的說法略微低估了本波是這個 stub 第一次從活的客戶分支被觸發，本 entry 已據此加重上面的 ⚠️ 標註)。
- 親自(非再派 agent)全新 build 目錄(`build_bucketb_verify`)：clean build，**ctest 83/87**(同組 4 個既有環境漂移失敗，非本波回歸)；mojibake 掃描異動檔：**0**；確認 `D:\SECS_GEM_LOGS` 未被本波觸及(檔案內容與 Bucket A 結束時一致)。

### ROADMAP DEFERRED 表更新
- `SECSGEM/uHGemEquipment.cpp`(:124)：A+B 兩桶皆已交付(連線開通+ini 持久化+狀態面板刷新)。**仍未觸碰**：C 桶——`Timer1Timer`本體、`DoConnect`/`DoSelect`/`DoSeparate`、`DoProcessSFNoResponse`、`clientGemRead`/`ProcessSocketReceiveData`(真正協定解碼核心，需未設計的 `TCriticalSection`/`TMemoryStream` shim)。新增待辦：KYEC 客戶real deployment前需設計真計時 `HTimer`(目前 `atester_shims.h`/本檔共用的 stub 恆真)。

### 下一步候選
C 桶(`clientGemRead`/`ProcessSocketReceiveData`/`Timer1Timer`)是本檔案最終、最大的剩餘難塊，需先設計 `TCriticalSection`/`TMemoryStream` shim + `SecsWireCodec`/`SecsSvEcRegistration` 成員化——這也是解鎖 `uHGemClass.cpp` 剩餘 gated method 與 `automation.cpp`/`TesterTCP.cpp` 各自 Timer 核心的關鍵前置。或轉 `uHGemClass.cpp` 剩 44/57、`TesterTCP.cpp` 的 `TimerProcessTCPDataTimer`、`automation.cpp` 的 `ProcessBuffer`、真計時 `HTimer` 設計(小而獨立，可解上述 KYEC 限制)、或 W7 續。**下一步不預設暫停**。

### 🔖 RESUME（已被下方 Bucket C 條目取代）
- **✅ uHGemEquipment 連線生命週期 Bucket A+B 皆完成（2026-07-17，同日連續兩波）**：A 桶(clientGem/srvGem 生命週期，28 函式)+ B 桶(7 個 widget stand-in、`InitialHGem`/`SaveSystemDefault`/`DoUpdateStatus`/`ProcessShow`/`FormClose`/`ManualCreatergRoleClick`/`FormShow`)。`ht9045_secsgem` 新連結 `ht9045_core`+`ht9045_globals`(確認無循環依賴)。**⚠️ KYEC 客戶部署前待辦**：`DoUpdateStatus` 的 30 秒強制斷線依賴的 `HTimer` 目前恆真(不會真的等 30 秒)，已三處清楚標註，尚未修。兩波皆獨立 fidelity-review clean、親自複驗 ctest 83/87(4 個既有無關失敗)、mojibake 0。**下一步不預設暫停**：C 桶 `Timer1Timer`/`clientGemRead`/`ProcessSocketReceiveData`(本檔最終難塊，需新 `TCriticalSection`/`TMemoryStream` shim)、真計時 `HTimer` 設計(可獨立解 KYEC 限制)、`uHGemClass.cpp` 剩 44/57、`TesterTCP.cpp` 的 `TimerProcessTCPDataTimer`、`automation.cpp` 的 `ProcessBuffer`、或 W7 續。

---

## 2026-07-17 — uHGemEquipment 連線生命週期 Bucket C（本檔最終難塊：Timer1Timer 主 SM + 協定解碼泵 + SendLocalData 真本體 + 真 HTimer）

**設定聲明（依 2026-07-17 新規則）**：主迴圈 Fable 5 + xhigh；設計 recon 用 Fable subagent、翻譯 Sonnet 5、獨立審查 Sonnet 5——與建議表一致。

**流程**：設計 agent 先產出 560 行可執行設計書（`SendLocalDataHook` 注入式設計、WireCodec 成員化、HSys.MyGem null-guard seam、ProcessReceiceData 部分真/部分 gate 切法、狀態別名路由、真 HTimer 移植來源、兩個新 shim 規格、T1-T10 測試計畫、~25 成員清單、7 步執行序）→ 翻譯 agent 照書執行 → 獨立審查 → 主迴圈親修審查發現 + 全新建置定案。

**新 vclcompat shim**：`MemoryStream.{h,cpp}`（`TMemoryStream`：WriteBuffer 在 Position 覆寫保尾、ReadBuffer/Seek/Clear/LoadFromStream/Size/Position，API=golden 實際用到的 9 種呼叫形）+ `SyncObjs.h`（`TCriticalSection`，Win32 `CRITICAL_SECTION` 支撐——翻譯 agent 實測本機 MinGW libstdc++ 無 `<mutex>`，偏離設計書的 `std::recursive_mutex` 有據）。`TFixedCriticalSection` 依 golden 宣告位置(:25-29)直接進 uHGemEquipment.h。

**真 HTimer（同時解掉 Bucket B 的 KYEC ⚠️ 待辦）**：設計 agent 找到 canonical golden 原始碼 `D:\HT9045\elec\Component\htimer.{h,cpp}`（component 庫檔，不在 golden snapshot 內），TU-local 移植含 never-armed→false、DWORD wraparound 分支逐字保留、「到期後 InUsed=false 但持續回 true」quirk。`atester_shims.h` 的恆真 stub 不動（其 gated 消費端依賴恆真語意）。test [22] 的 KYEC 斷言同步改為驗證「不會立刻觸發」的真行為。

**SecsWireCodec 唯一增項**：`std::function<void(SecsWireCodec&)> SendLocalDataHook`——空則維持原 stub 行為（`bReceiveData=false`），零波及既有消費端（`test_SecsWireCodec` 226/226、`test_uHGemClass` 86/86 皆原樣通過佐證）。THGem ctor 對自己的 `WireCodec` 裝上 hook → `SendLocalDataFrom(SecsWireCodec&)` 承載 golden :1985-2107 真本體（client/server 送出分支、`bServoSocketConnect` close/reopen dance、`SFCodeResponseList`/`TimeLeft` T3 記錄、例外路徑全逐字）。

**THGem 翻譯清單**（golden 行號見各函式註解）：`InitSTypeStruct`/`DoSelect`/`DoSeparate`(取代 Bucket B stub)/`DoConnect`(含 Ifor 20260420 Task=200 死結修正註解逐字保留)/`DoProcessSFNoResponse`(T3 每秒掃描+S9F9)/`DoLocalAllProcessLoop`(殼+4 gated 子系統 stub)/`SelectRsp`/`DeselectRsp`/`LinktestRsp`/`ProcessReceiceData`(control-message head 真翻譯——Select/Deselect/Linktest 握手 e2e 可測；S,F data-message tail 整塊 `#if 0` 待 SystemModularInitial 接線波)/`CheckSFCodeResponse`/`SaveSECSGEMTextToLog`/`ShowLocalBufferBinaryData`/`ShowLocalHeadInfo`/`clientGemRead`(RecvMemoryBuffer 上鎖汲取)/`ProcessSocketReceiveData`(訊框長度重組三段式+多訊息 do-while+Ifor 20260402/20260421 兩個 dated 加固逐字)/`Timer1Timer`(golden :5176-5527 主 SM，13 個 case 值全逐字含 KYEC 分支與死 case 350)。`WaitShowString`/`LogDataString` 改為 WireCodec 同名成員的別名（恢復 golden 單一顯示/log 流，dtor 跳過刪除別名對）。HSys.MyGem 4 個呼叫點以 `!=NULL` guard 接（比照 cprod.cpp:2199 先例；S9F9/S9F7 在 HTGem 已真、S1F13 仍 gated → DoConnect 忠實停在 Task=200）。

**忠實保留的 golden bug**（各處已註記）：ProcessBuffer 逐訊息洩漏(:9119)、EthernetBuffer=NULL 不 delete[](:9198)、未上鎖的 RecvMemoryBuffer 讀取與上鎖寫入 race(:9062-9068)、bSeprate 兩分支皆 false(:4731-4737)、Timer1Timer case 350 死碼(全檔 grep 證實無人指派 350)、EthernetBuffer 成員/區域變數遮蔽。

**獨立審查（10 項全查）＋主迴圈親修 2 個發現**：
- **HIGH（休眠地雷，審查 agent 以獨立重現 segfault 證實）**：`MyDBIProcess` 3-arg 呼叫慣例不一致——`uHGemClass.cpp:251` 宣告無 `__fastcall`、`database.cpp:64` 有；本波新增的 `ht9045_secsgem→ht9045_db` 連結邊讓兩者首次同進一個 link，ld 以 fixup 啟發式「解決」並僅發警告，實際是錯誤呼叫慣例（i686 fastcall 走暫存器傳參）。現行測試全過只因受影響呼叫點全在例外路徑。**根因是系統性缺陷**：`vcl_compat.h` 的 `#ifndef __fastcall` 中和從未生效（`__fastcall` 是 MinGW 編譯器內建巨集，`#ifndef` 恆假）——已列 ROADMAP 追蹤。主迴圈親修：`uHGemClass.cpp:251`+`test_uHGemClass.cpp` stub 補上 `__fastcall`（golden cMyDB.h:20 正確形式），重連結證實 fixup 警告消失、86/86+252/252 續過。
- **LOW**：uHGemEquipment.h D1 註解的成員宣告順序主張誤述——已改寫為正確的 C++ 語意說明（ctor body 在全部成員建構完才跑，宣告相對順序無關）。

**測試**：`test_uHGemEquipment` 181→**252**（T1-T10：MemoryStream 語意/HTimer 鏡像行為/真 Select.req→Select.rsp Sim 握手/分割與併連訊框重組/T3 逾時→S9F9 經 hook seam/Timer1Timer pump）。測試衛生：新增 `TextLogSnapshot` capture/restore 包住會觸達 `SaveSECSGEMTextToLog` 的 T3/T4/T5（翻譯 agent 自己的 dev-loop 曾殘留一個 2872-byte TextLog，經確認為本 session 產物後刪除）；`D:\SECS_GEM_LOGS\2026\07_17\` 前後 md5 相同、僅存基準 128-byte ErrLog。
**主迴圈親自定案**：全新 `build_bucketc_verify` from-scratch build exit 0、build log 全文 grep `resolving`=0、**ctest 83/87**（同組 4 個既有 `Gerneral.ini` 環境漂移失敗）、mojibake 0（12 檔）。

### ROADMAP DEFERRED 表更新
- `uHGemEquipment.cpp`：A+B+C 三桶皆完成。剩餘窄化為：ProcessReceiceData 的 S,F data-message dispatch tail(golden :8812-8988，`#if 0`，待 SystemModularInitial 接線波)+`DoSpool`/`DoTraceDataResponse`/`DoUploadFileToHost`/`DoDownLoadRemoteFile` 4 個獨立子系統+FormCreate SV 註冊(待 SvEcReg 成員化波)+two-codec-instance 收訊側合併(inline 已註記)。KYEC HTimer ⚠️ 已解除。
- 新增系統性追蹤項：`vcl_compat.h` `__fastcall` 中和失效（編譯器內建巨集），全樹 `__fastcall` 標註宣告皆受影響，需獨立稽核波。

### 下一步候選
**SystemModularInitial 接線波**（接上 `HSys.MyGem`→un-gate ProcessReceiceData S,F tail+DoConnect S1F13，並處理 two-codec 收訊合併）現在是本檔收尾的天然下一步，也同時解鎖 `uHGemClass.cpp` 的實戰消費；或 `uHGemClass.cpp` 剩 44/57、`TesterTCP.cpp` 的 `TimerProcessTCPDataTimer`、`automation.cpp` 的 `ProcessBuffer`、`__fastcall` 系統性稽核（獨立小波）、或 W7 續。**下一步不預設暫停**。

### 🔖 RESUME（已被下方 2026-07-19 條目取代）
- **✅ uHGemEquipment Bucket A+B+C 全數完成（2026-07-17，同日三連波，本檔翻譯主體收尾）**：C 桶交付 Timer1Timer 主 SM(13 case 全逐字)+clientGemRead/ProcessSocketReceiveData 協定解碼泵+SendLocalData 真本體(hook 注入式)+Select/Deselect/Linktest 握手+T3 逾時機制+新 vclcompat `MemoryStream`/`SyncObjs` shim+真 `HTimer`(順帶解除 KYEC ⚠️)。獨立審查抓到 HIGH 級 `MyDBIProcess` `__fastcall` ABI 地雷(實證 segfault)，主迴圈已修並揭露系統性 `vcl_compat.h` 中和失效(入 ROADMAP 追蹤)。fresh build 83/87、630 斷言跨 4 套件全綠、mojibake 0、`SECS_GEM_LOGS` byte-identity 確認。**下一步不預設暫停**：SystemModularInitial 接線波(天然收尾)、`uHGemClass.cpp` 44/57、`TesterTCP.cpp` Timer 核心、`automation.cpp` ProcessBuffer、`__fastcall` 稽核小波、或 W7 續。

---

## 2026-07-19 — SystemModularInitial 接線波啟動（in-flight 檢查點，使用者關機暫停）

**設定聲明**：主迴圈 Fable 5 + xhigh；設計 agent Fable 5（背景執行中）；後續規劃：翻譯 Sonnet 5 + high、獨立審查 Sonnet 5。

**波次範圍**（DEVLOG 2026-07-17 候選定案的「天然下一步」）：
1. 接活 `HSys.MyGem`——golden `database.cpp:1539-1546`（`SystemModularInitial` 本體僅 8 行，核心是 `MyGem=new HT9045Gem("HT9045", HGem)`）；已譯樹 `database.cpp:148-155` 現為 `#if 0` gate、`database.h:235-238` MyGem 恆 NULL。
2. 解閘 `ProcessReceiceData` S,F data-message dispatch tail——golden `uHGemEquipment.cpp:8812-8988`；已譯樹 `SECSGEM/uHGemEquipment.cpp:2957` gate（缺口：MyGem wiring + 6 個 bReceive* flag + MoveCheckCallBack + ~15 THGem method）。
3. `DoConnect` S1F13 接活確認（已譯樹 uHGemEquipment.cpp:2617-2667 呼叫點已有 `!=NULL` guard）。
4. two-codec 收訊側合併（`THGem.WireCodec` vs `HTGem.WireCodec`；inline 註記在 uHGemEquipment.cpp:3152 附近與 SecsWireCodec.h:397）。

**設計書目標路徑**：`C:\Users\JIMMYC~1\AppData\Local\Temp\claude\D--HT9045\452108d5-1416-413a-b6dc-35801b9adf0d\scratchpad\DESIGN_SystemModularInitial_wiring.md`
設計書須逐項定案 A-G：A=MyGem 具體型別（HT9045Gem ~9200 行未翻且本波不翻——THGem 直接實例化 vs 薄 shim 擇一，含 override 行為差異分析與未來換回路徑）；B=golden ctor 第二參數 `HGem` 是什麼+生命週期/所有權（SYSTEM_MODULAR ctor/dtor 皆仍 gated）；C=S,F tail 精確切法（缺口逐項盤點，部分真/部分 gate，比照 Bucket C 前例）；D=two-codec 定案；E=__fastcall ABI 配對檢查清單+強制 build log grep `resolving`=0（Bucket C 已實證系統性中和失效）；F=T1-Tn 測試計畫（沿用 test_uHGemEquipment 252 斷言+TextLogSnapshot 慣例）；G=翻譯 agent 施工序。

### 🔖 RESUME（已被下方 2026-07-20 條目取代）
- **▶ SystemModularInitial 接線波 in-flight（2026-07-19）**：程式碼零變更、工作樹乾淨（僅本 DEVLOG 條目）。下次接續：(1) 檢查上方 scratchpad 路徑的設計書是否存在——**存在**→直接派翻譯 agent（Sonnet 5 + high）照書施工→獨立審查（Sonnet 5，協定核心用 xhigh）→主迴圈親自 fresh from-scratch build + ctest + mojibake 0 + build log grep `resolving`=0 複驗→commit+更新 DEVLOG/ROADMAP/RESUME；**不存在**（關機時設計 agent 未完成即遺失）→依上方 A-G 清單重派設計 agent（Fable 5），prompt 要點全在本條目。(2) 驗證基準：ctest 83/87（4 個既有 Gerneral.ini 環境漂移失敗非回歸）、630 斷言跨 4 套件。(3) 分支 fix/v899.32-pti（勿信 feat/cpp-906-migration stale 指標）。

---

## 2026-07-20 — SysModWire 翻譯施工啟動 + 三路讀取型 pipeline 平行展開（in-flight 檢查點）

**設定聲明**：主迴圈 Fable 5 + xhigh；翻譯 agent Sonnet 5（施工中）；設計 recon×2 Fable 5；`__fastcall` 稽核 Sonnet 5。

**狀態**：設計書 `DESIGN_SystemModularInitial_wiring.md`（452108d5 scratchpad，50KB）確認存活且完整（定案 A–G/§7 執行序/§9 驗收清單俱全）→ 已派翻譯 agent 照書施工（背景）。同時依「讀取型可平行、寫入型單波」原則平行派出 3 個唯讀 agent，產出物路徑（本 session scratchpad `...\D--HT9045\be8fe31a-b61f-4841-9bc7-87c3a238ca62\scratchpad\`）：
1. `DESIGN_TesterTCP_TimerProcessTCPDataTimer.md` — TesterTCP Timer 核心波設計書（Fable 5）。
2. `DESIGN_automation_ProcessBuffer.md` — automation ProcessBuffer 波設計書（Fable 5）。
3. `AUDIT_fastcall_tree.md` — 全樹 `__fastcall` ABI 靜態稽核（ROADMAP 追蹤項；純靜態不 build；volatile 檔標 post-SysModWire 重驗）。

**三個唯讀 agent 全數交付（同日）＋主迴圈裁決**：
- **⚠ Golden 路徑勘誤（重要，永久記錄）**：主迴圈派工 prompt 曾把 golden 誤植為 V899 樹；被 automation 設計 agent 抓到並經鐵證核實（DEVLOG 既往引註 database.cpp:1539 只與 V906 snapshot 吻合）。**本遷移 golden ＝ `HT9011UC_Code_V3.33.906.0_20260618`**；V899 樹只是 AGENTS.md 除錯預設目標，與遷移無關。已更正兩個在途 agent、已入主迴圈持久記憶。今後所有派工 prompt 一律寫 V906 snapshot。
- **`AUDIT_fastcall_tree.md`**：92 組 `__fastcall` 符號 91 組一致；唯一不一致=HIGH：`MyDBIProcess` 3-arg 版——兩處外部宣告 `__fastcall`（database.cpp/uHGemClass.cpp），唯一本體在 uHGemEquipment.cpp:1731 無 `__fastcall` 且關在匿名 namespace（nm 證實 fastcall 裝飾符號 undefined、無人提供）。既往 build 能過是因 archive member 未被拉入；**SysModWire 接活 database.cpp 後會浮出**→已列入 SysModWire 審查驗收清單，屆時修（本體移出匿名 ns + 補 `__fastcall`，golden cMyDB.h:20 為準）。vcl_compat.h 無效中和段的處置（方案 A 刪除改註記/方案 B `-D__fastcall=`）併入 fastcall 修復小波裁決。
- **`DESIGN_automation_ProcessBuffer.md`**（W906-AutoPB）：ProcessBuffer+ProcessBuffer1+SendReportRequest 三支全真翻 ~1181 行，僅動 3 檔、零 CMake/shim/link 邊；與 SysModWire 檔面交集零，唯一硬依賴=波次時序。裁決：Q2 PORT-ONLY UB 防護 D1-D3（最小防護+loud 註記）**接受**（UB 非「行為」，逐位保留只會讓譯樹隨機炸）；Q3 btnConnectClick 2 行修正**拔出本波、獨立 commit**（blame isolation）；Q4 dispatch 深處炸=回報不自修 **接受**。
- **`DESIGN_TesterTCP_TimerProcessTCPDataTimer.md`**：TimerProcessTCPDataTimer(:349-552)+SimulateBin 全 ACTIVE 翻譯，落點擴充 TesterTCP_Socket 切片#1，零新檔零 CMake；與 SysModWire 交集零。裁決：Q2 ECHOCODE oracle 施工時實跑定案 **接受**；Q3 `PERSITETemperatureStrings` 真本體（~538 行）留未來獨立波、本波只裝 seam **接受**（入 ROADMAP DEFERRED）。
- 兩本設計書＋稽核報告皆在本 session scratchpad（路徑見上）；golden cp950 解碼中間產物同目錄。

### 🔖 RESUME（已被下方 SysModWire 完成條目取代）
- **▶ SysModWire 翻譯施工中；3 個唯讀交付物已完成並裁決（2026-07-20）**。下次接續：(1) 翻譯 agent 完成→派獨立審查（Sonnet 5，協定核心 xhigh，照設計書 §9 驗收清單＋fastcall 稽核的 `MyDBIProcess` 發現）→主迴圈親自 fresh build + ctest + mojibake 0 + grep `resolving`=0 →commit+更新 DEVLOG/ROADMAP。若關機遺失翻譯成果：工作樹 `HT9011UC_Cpp_V3.33.906.0` 若有半成品先 `git status`/diff 評估，設計書仍在 452108d5 scratchpad 可重派。(2) SysModWire commit 後的寫入波佇列（一次一波）：TesterTCP 切片#1（設計書就緒）→ automation W906-AutoPB（設計書就緒；開工前重取 ctest 基線）→ fastcall 修復小波（含 vcl_compat.h 中和段處置）。(3) 驗證基準 ctest 83/87、630 斷言跨 4 套件；分支 fix/v899.32-pti。(4) **golden＝`HT9011UC_Code_V3.33.906.0_20260618`，勿再誤植 V899**。

---

## 2026-07-20 — SysModWire 接線波完成（HSys.MyGem 接活 + S,F tail 解閘 + HT9045Gem shim + uHGemClass 21/57）

**設定聲明**：主迴圈 Fable 5 + xhigh；翻譯 Sonnet 5；獨立審查 Sonnet 5；皆照 2026-07-19 設計書（Fable 5，452108d5 scratchpad）施工。

**交付**（commit `799bcdb`，14 檔 +1648/-202）：`database.cpp/.h` SystemModularInitial 真本體（golden :1539-1546）+新 `SECSGEM/uHGemHT9045_Shim.{h,cpp}`（`HT9045Gem` 薄 shim，真 uHGemHT9045 波落地時先刪 shim 再編）；`ProcessReceiceData` S,F data-message dispatch tail（golden :8812-8988）整塊真解閘（單一 MyGem null-guard）；`DoConnect` S1F13 接活；two-codec 收訊側以 `HTGem.ActiveWire` 指標合併（預設自指，保留雙實例）；`uHGemClass.cpp` 再解 8 個 gated method（S1F1/S1F2/S1F13/S1F14/Process_S1F14/S1F16/S1F18/S2F18，13/57→**21/57**）；test_uHGemEquipment 252→**317** 斷言（W1-W10）。

**翻譯 agent 自報偏離 5 項，獨立審查逐項裁定全部成立**：(1) test_uHGemClass.cpp 非零 diff——解閘後既有 NULL-HGemPtr stub 樣本會真 segfault，移出換 2 支仍 gated 者（審查讀碼證實）；(2) 新 link stubs 檔+RESCAN——跨 archive 循環真實存在（uHGemClass→uHGemEquipment→HSys(ht9045_db)→HT9045Gem(ht9045_secsgem)）；(3) test_config_loaders 2-arg 樁必要；(4) W6(f) 改用 check-before-read 的 -2 回傳點（原 oracle 依賴越界讀 UB 實測 flaky）；(5) 3 處 gate 註解缺口清單修準。另翻譯 agent 正確識別任務書兩處 V899 樣板誤植（golden 路徑、AI 註解 tag），依設計書權威處理（tag=`AI(W906-SysModWire)`）。

**審查 findings 與處置**：
- **HIGH（未解，裁定移交下一波）**：`MyDBIProcess` 3-arg ABI 雷未被本波解決——唯一本體仍在 uHGemEquipment.cpp 匿名 ns、無 `__fastcall`（=對外 undefined）；build 全綠只因 3 個測試各自帶本地 fastcall 樁搶先滿足符號（nm 證實生產提供者 0、無 fixup 候選→引爆=大聲連結失敗非無聲損壞）。此為 Bucket C（20260717）既有缺陷，本波未引入未惡化；為保 blame isolation 不混入本波，**fastcall 修復小波提前為下一波**。
- **LOW（主迴圈親修後 commit）**：翻譯 agent 把 `ht9045_secsgem` 邊防禦性加到 5 個 motor/io 目標，但 nm 證實它們從不拉 `database.cpp.o`——已回退 5 個推測性邊+修正 root CMake 註解為準確 3 目標清單（config_loaders/uHGemClass/uHGemEquipment）；回退後全綠佐證邊確實未用。

**主迴圈親自定案**（全新 `build_sysmodwire_final`，含 LOW 修正後）：from-scratch build exit 0、`grep -ic resolving`=**0**、ctest **83/87**（同 4 個既有環境漂移：config_db/IniFiles/ini_helpers/config_loaders）、test_uHGemEquipment **317/317**、mojibake **0/14**、`D:\SECS_GEM_LOGS` md5 **11 檔 byte-identical**。

**ROADMAP 同步**：W5 進度段（含補修 C 桶完成後未同步的「剩」清單漂移）、下一步佇列、database.cpp/uHGemClass(36 剩)/uHGemEquipment/vcl_compat 四列 DEFERRED 表。

### 🔖 RESUME（最新）
- **✅ SysModWire 完成（2026-07-20，commit `799bcdb`+docs）。下一波佇列（寫入型一次一波）**：(1) **fastcall 修復小波**——`MyDBIProcess` 3-arg 本體移出匿名 ns+補 `__fastcall`（暫居 uHGemEquipment.cpp，註明 golden 家 cMyDB.cpp:788）＋刪 3 個測試 3-arg 樁（斷言捕捉遷 2-arg 樁，逐一檢查各樁是否被斷言消費）＋vcl_compat 無效 `#ifndef __fastcall` 段刪除改真實註記（方案 A；不做 `-D__fastcall=`）＋nm 驗證生產 obj 恰一定義＋全套五道閘。(2) TesterTCP `TimerProcessTCPDataTimer`（設計書 `DESIGN_TesterTCP_TimerProcessTCPDataTimer.md` 於 be8fe31a scratchpad）。(3) automation W906-AutoPB（`DESIGN_automation_ProcessBuffer.md` 同目錄；開工前重取 ctest 基線；btnConnectClick 2 行修正獨立 commit）。驗證基準更新：test_uHGemEquipment=317、四套件斷言 317+86+226+config；ctest 83/87（4 既有漂移）。golden=`HT9011UC_Code_V3.33.906.0_20260618`（勿再誤植 V899）；分支 fix/v899.32-pti。

---

## 2026-07-20 — FastcallFix 小波完成（MyDBIProcess 3-arg 外部化，稽核 HIGH 結案）

**設定聲明**：主迴圈 Fable 5 + xhigh；施工 Sonnet 5（過程中兩度停在等背景 build/ctest，主迴圈 SendMessage 喚醒接續——W6.2b 模式的良性變體，poller 有正常回喚）。

**交付**（commit `7376490`，5 檔 +144/-61）：`uHGemEquipment.cpp` 3-arg `MyDBIProcess` 移出匿名 ns+補 `__fastcall`（全樹唯一生產提供者；**定義不帶 S3 預設值**——帶了會讓同 TU 8 處 2-arg 呼叫 ambiguous，預設值只在 database.cpp:75/uHGemClass.cpp:310 宣告端=golden cMyDB.h:20 同形；暫居此檔至 cMyDB.cpp 翻譯波）；刪 3 個遮蔽性測試樁（test_uHGemClass 的 `g_dbiCalls` 確認 dead-write 無斷言消費後連同移除）；`vcl_compat.h` 無效 `#ifndef __fastcall` 段刪除，改為配對紀律註記＋明文禁止 `-D__fastcall=`。唯一偏離（施工 agent 顯式標註）：`test_uHGemClass.cpp` 打破 SysModWire 波「diff 為空」舊約——重複定義連結錯誤無法迴避，裁定成立。

**主迴圈親自定案**（diff 逐行親審+全新 `build_fastcallfix_final`）：build exit 0、`resolving`=0、ctest 83/87（同 4 既有漂移）、nm 全樹掃描 fastcall 裝飾 3-arg 符號恰 1 個 T（`ht9045_secsgem/uHGemEquipment.cpp.obj`）、317/86/226 全綠、mojibake 0/5、SECS_GEM_LOGS md5 11 檔一致。本波 diff 小，獨立審查以主迴圈親審 diff 取代（nm 證據齊備）。

### 🔖 RESUME（最新）
- **✅ SysModWire（`799bcdb`）+ FastcallFix（`7376490`）皆完成（2026-07-20）。下一波佇列（寫入型一次一波）**：(1) **TesterTCP `TimerProcessTCPDataTimer` 波**——設計書 `DESIGN_TesterTCP_TimerProcessTCPDataTimer.md`（be8fe31a scratchpad，golden cp950 解碼輔助檔同目錄）；裁決已定：golden=V906 snapshot、ECHOCODE oracle 施工時實跑定案、`cbSiteOn[32]` 不搬留真 UI 波；施工前重驗：ctest 基線以當下 fresh build 為準、確認 tests/CMakeLists 的 test_testertcp_socket/test_interfacesys target 未被前兩波改動。(2) automation W906-AutoPB（`DESIGN_automation_ProcessBuffer.md` 同目錄；btnConnectClick 2 行修正獨立 commit；開工前重取基線）。驗證基準：ctest 83/87（4 既有漂移：config_db/IniFiles/ini_helpers/config_loaders）、套件 317/86/226。golden=`HT9011UC_Code_V3.33.906.0_20260618`；分支 fix/v899.32-pti；工作樹另有無關 V899/config 殘留變更（PTI 案）勿誤圈入 commit。

---

## 2026-07-20 — TesterTCP Timer 波完成（TimerProcessTCPDataTimer 十分支解碼 pump + SimulateBin，切片 #1 就地長大）

**設定聲明**：主迴圈 Fable 5 + xhigh；翻譯 Sonnet 5；獨立審查 Sonnet 5；設計書為 2026-07-20 稍早 Fable 5 設計 agent 產出（be8fe31a scratchpad）。

**交付**（commit `0dfecd9`，11 檔 +1044/-17，零新檔零 CMake）：`TimerProcessTCPDataTimer`（golden :349-552，WORKFILE_OK/FAIL、BARCODE?、ECHOCODE:、Test Arm?、TempArm?、BINON:、ECHOOK、GETOSSETUP、GetTPSVersionOK 註解死碼——十分支全 ACTIVE）+`SimulateBin`（:554-613）；FormsFacade +3 成員（`tTestResult`/`tBarCodeList` ctor `"0;"` quirk 對 golden main.cpp:2235-2240 逐字/`SVID1190_OSSetup`）+`WritePERSITETemperature` wrapper+`PERSITETemperatureStrings` seam；`iBin[4][8]` 生產定義入 atester_shims（nm：恰一定義三 extern）；vclcompat `random(int)`（Borland 0..n-1 語意）。test_testertcp_socket 27→**80** 斷言。16 條 golden quirk 逐字保留＋註記（含 bTimerRun 從未上膛、sBarCodeList/sBinList 每訊息洩漏、SimulateStart 死觸發）。**ECHOCODE oracle 實跑定案**：ctor 預設值 echo 回來=ECHOCODENG（slot 31 `"0;"` 內嵌分號讓 golden 解析器截斷第 32 token）——golden 演算法本身的必然行為，已寫入測試註解。

**獨立審查：CLEAN 零 findings**——十分支+SimulateBin+facade ctor 預填逐行對 golden（cp950）核對逐字相符；16 quirk 逐條打勾；2 項自報偏離（T-A 測試繞道隔離 timer guard、check_i 改 check_b/check_s）裁定成立且僅測試層；ECHOCODE oracle 獨立手算+重現；全部數字獨立重跑一致；AI tag 100% `AI(W906-TesterTCPTimer) 20260720` 無混入。

**主迴圈親自定案**（全新 `build_testertcp_final`）：build exit 0、`resolving`=0、ctest 83/87（同 4 既有漂移）、80/80、mojibake 0/11、SECS_GEM_LOGS md5 11 檔一致。

### 🔖 RESUME（最新）
- **✅ 2026-07-20 四波連發全落地**：SysModWire（`799bcdb`）→ FastcallFix（`7376490`）→ TesterTCP Timer（`0dfecd9`）＋各自 docs commit。**下一波（佇列最後一項）：automation W906-AutoPB**——設計書 `DESIGN_automation_ProcessBuffer.md`（be8fe31a scratchpad）；範圍=`ProcessBuffer`(:951-1978)+`ProcessBuffer1`(:1980-2044)+`SendReportRequest`(:2046-2130) 三支全真翻 ~1181 行、只動 3 檔零 CMake；裁決已定：PORT-ONLY UB 防護 D1-D3（最小防護+loud 註記）接受、btnConnectClick 2 行修正獨立 commit、dispatch 深處炸=回報不自修 auto9045；施工前重驗：fresh 基線（現為 ctest 83/87；test_testertcp_socket=80、uHGemEquipment=317、uHGemClass=86、SecsWireCodec=226、interfacesys=80、TesterTCP=16）、確認 tests/CMakeLists 的 test_automation link 行未變。之後候選見 ROADMAP 下一步段（uHGemClass 36 剩、W7 續、SECSGEM 子系統波等）。golden=`HT9011UC_Code_V3.33.906.0_20260618`；分支 fix/v899.32-pti；工作樹另有無關 V899/config 殘留（PTI 案）勿圈入。

---

## 2026-07-20 — automation W906-AutoPB 完成（ProcessBuffer 家族三支全真翻，OLP 派工核心落地；今日第四波）

**設定聲明**：主迴圈 Fable 5 + xhigh；翻譯 Sonnet 5；獨立審查 Sonnet 5；設計書為 2026-07-20 稍早 Fable 5 產出。

**交付**（wave commit `b810c0e`，3 檔 +1924/-53；D5 獨立 commit `e4808c2`）：`ProcessBuffer`(golden :951-1978，~130 分支 OLP 命令派工 ladder)+`ProcessBuffer1`(:1980-2044)+`SendReportRequest`(:2046-2130) 全真翻取代空殼 stub——auto9045 的 148 函式面**首次被真實 OLP 訊框實戰消費**，全部驅動路徑乾淨。P1-P18 golden quirk 逐字（Data[0] 雙寫外洩×9、PP_DL hex 恆 0 全 NUL zip、RESUME 不清 SoftStop、死 PAUSE 分支、stale TCPstr）；D1-D3 PORT-ONLY 防護（僅此三處不逐位，loud 註記+oracle 覆蓋）。test_automation 85→**127**（基線實測修正：85 非設計書估的 86）。D5=btnConnectClick `!=0`→`!=AnsiString(0)`（vclcompat char* overload 使裸 `!=0` 比對 ""，golden BCB6 比對 "0"——依裁決獨立 commit，並更正前波留下的錯誤註解=審查 LOW#2）。

**實質偏離（審查裁定成立、揭露充分）**：Part 22 活測範圍縮小——`DoDLRequest`(auto9045.cpp:2274，前波 W5-Final 交付)有未防護真實檔案副作用（production `DataPath`/`OffsetPath`=repo 內 IniData 路徑、CopyFile 7z.exe、SetCurrentDirectory；`DoULRequest` :2197 更含 `system("del")`），`aDataPath` 重導防不到→PP_DL_REQUEST 不活跑，P11/P13 兩 quirk 改逐行核對（審查獨立重做核對，一致）。**已入 ROADMAP 追蹤列：防護 seam 落地前任何測試不得活跑 PP_DL/PP_UL_REQUEST。**

**獨立審查：CLEAN**（2 個 LOW 文件級）——審查自寫 tokenizer 做註解/空白剝離後的 **token-level 全文 diff**：三支函式對 golden 除 `__fastcall` 移除+4 個已揭露偏離外**零未揭露差異**；175 個分支條件序列完全對齊；P1-P18/D1-D5 逐條打勾；Big5 7 段 CJK 逐字相符。LOW#1（Part 22 過時註解）主迴圈已修入波；LOW#2 併入 D5 commit。

**主迴圈親自定案**（全新 `build_autopb_final`，波+D5 合併態）：build exit 0、`resolving`=0、ctest 83/87（同 4 既有漂移）、test_automation 127/127、mojibake 0/3、SECS_GEM_LOGS md5 一致、IniData 未被測試觸及。

### 🔖 RESUME（最新）
- **✅ 2026-07-20 全日五連發收官**：SysModWire(`799bcdb`)→FastcallFix(`7376490`)→TesterTCP Timer(`0dfecd9`)→automation W906-AutoPB(`b810c0e`)+D5(`e4808c2`)，各附 docs commit，全部獨立審查+主迴圈五道閘。**寫入佇列已清空。**驗證基準：ctest 83/87（4 既有漂移）；套件 317/86/226/80/80/16+automation 127。**下一輪候選**（無既成設計書，開波前先派設計 recon）：`uHGemClass.cpp` 剩 36、SECSGEM 4 子系統(DoSpool/Trace/上下傳)+FormCreate SV 註冊、`uHGemHT9045.*` 站點覆寫層(先刪 shim)、DoDLRequest/DoULRequest 防護 seam 小波、W7 csystem gated 大宗、或 KNOWLEDGE「真實/半真實 HAL pump」方向評估。golden=`HT9011UC_Code_V3.33.906.0_20260618`；分支 fix/v899.32-pti；工作樹另有無關 V899/config 殘留（PTI 案）勿圈入。進度儀表板 artifact：https://claude.ai/code/artifact/26e3926c-c145-4699-9743-a7ae9db614e4（同 session 重發同檔路徑即可更新）。

---

## 2026-07-20 — 讀取型 pipeline 六路齊發（1 寫入波 Wave 1 在途時的火力全開；全部唯讀交付）

**設定聲明**：主迴圈 Fable 5 + xhigh。6 個唯讀 agent 與 SECSGEM Wave 1 寫入波同時跑，零檔案碰撞（只讀 golden+譯樹、只寫 scratchpad、SECSGEM 檔標 volatile、稽核不 build）。產出物全在 be8fe31a scratchpad。

### 兩份稽核（de-risk）
- **`AUDIT_test_hazards_tree.md`**（HIGH 2/MED 4/LOW 9）：**HIGH#1 `Public/cBootLog.cpp`**——`test_cBootLog.cpp` 每跑一次就對硬編碼 `D:\HT9045\Error\BootLog.txt`(#define 非可重導)真 append/rotate，**是已提交、ctest 實跑的測試**，無隔離→需儘早修。**HIGH#2 `SaveEventReportData()`**(uHGemEquipment.cpp:2403)覆寫 production `EventReport_*.def`，僅靠測試作者「記得不呼叫」擋著，4 個呼叫點是後續波自然會補測的候選→SECSGEM W3 前須建結構性 seam。MED：cprod AddAlarm→ExecZipCommand(system/XCOPY)鏈零可達、no-op 宏擋著(W6 排定解除)；test_SCK_ART_Remainder.cpp:301 過時誤述。命名風險：`Gated_<Name>` 有兩義(no-op 樁 vs 忠實真 I/O)。
- **`AUDIT_gated_inventory.md`**（338 gate/52 檔）：**假 gate/舊帳 7 處**——5 個真「已解決」ODR-dup(cpublic.cpp:1044/1235/1349/1512 + aHotPlateSubstrate.cpp:494，可安全刪、零功能增益)；atester_32Site.cpp:1698 部分過時(SendMSG_CMD 現已存在，只剩 MSG_CMD_Arm1Down 常數真缺)；2 處 golden-side dead ref(正確保留)。**MOT[] gate 非假**(MOT[] 陣列全樹未宣告，30 個 gate 正確擋著)。**最高槓桿零成本候選：common.cpp/h「wave-file」叢集**(WriteDataToFile/MyForceDirectories/CheckFileIsEmpty/ReadDataFromFile)——fan-out 最高，串聯解鎖 ≥5 個下游 gate(AGV_E84/HANA_ART/BarCode_Bottom2DID/cpublic/**SECSGEM S7F18**)，只需既有 vclcompat 檔案 API。

### 四份設計書（前置寫入 pipeline）
- **`DESIGN_DoDLRequest_test_seam.md`**：推薦候選 A(測試期重導 5 全域 DataPath/OffsetPath/**AuthPath**/LastDataPath/aDataPath)+C(golden zip-existence 守門)。**零 golden/common/CMake 變更**。關鍵發現：AuthPath 必須一併重導(CheckAndReadIniData 缺 key 會寫回預設值污染 config\Security_new.def)；W5FA_FMain 在匿名 ns→DoULRequest 檔名不可控。**主迴圈裁決 Q1：ctest 預設不起外部行程**(保 CI 可攜+符合零副作用紀律)→DoDLRequest 淺路徑重導+不餵 zip 活測(零行程)、深路徑與 DoULRequest 掛 7z-present 閘或維持手動核對。
- **`DESIGN_VCL_widget_cast_wave.md`**：recon 校正前提——**只 4 位點真共用 7 型別 cast 叢集**(SetECValue+DataItemOutSV/EC+GetECDataValue 的 IsVCL==1 分支)。關鍵事實 RTTI 已可用、`vclcompat::TObject` 已在→直接真 `dynamic_cast`，golden cast 行逐字可編。定案新建 `vclcompat/Controls.h`(header-only 6 型別)+**R1：TStringList 須移入 TObject 樹(TStrings:TObject)**。桶切 VCW-1 核心(~550 行)/VCW-2 終端/分流 C→SECSGEM W3b/分流 D(S2F32 時鐘)→獨立微波。
- **`DESIGN_uHGemHT9045_siteoverride.md`**：⚠**改寫 roadmap 假設的大發現**——真本體 ~9140 行(22 override)有**三道依賴牆，SECSGEM 收尾群只拆一道**：牆①(THGem/wire/SvEcReg，收尾群解)、**牆②(fMain ~22 未翻表單 fan-out，FormsFacade 缺 ~19，收尾群不涵蓋)**、**牆③(Handler 自由函式 GetAlarmCodeList/MyDBVProcess/ProcessLotInfo/DoAutoRetest/SaveAllFile 未翻，收尾群不涵蓋)**。**整檔無法一次 swap 全翻**。定案漸進式真類別：Bucket 0 一次性 swap-back(刪 shim→真骨架 header 只宣告已翻 method、未翻者繼承 base=與現 shim 逐位同行為→逐桶填 body 零 churn)。**只有 Bucket 0 在 W1 後可立即施工**；其餘 5 桶主阻塞是牆②③(比 W2/W3 更關鍵、需獨立 FormsFacade/free-func 前置波)。
- **`RECON_W7_breakdown.md`**：W7=10 區(+1 vision)。cContact ~11478 行 0 譯(最破碎)；main.cpp 34972/uLotInfo 16613/ckernel+AutoClean 0 譯。建議 8 交付波(含 2 enabler)。**HAL pump 最小切入點：E0(半真實 timed-sensor 泵)→M1(MainProc 模式選擇器,泵頂)→M2(DoAllProcess 決策梯)**——現 Sim HAL 瞬間收斂會遮排序 bug，E0 讓覆蓋「有牙齒」。關鍵發現：**AutoClean.cpp 9137 行 0 譯是 Z2 最大 gate**；ScanSystemSensor 在 ckernel.cpp:359(非 csystem)；fMain-> seam=767 相異成員(258 純 widget/**509 method-like 含真業務邏輯，勿整包當 UI 跳過**)。最大不確定性=cContact 下壓 SM 的 widget-in-switch-case 可分離性。

### 重排的寫入波佇列（一次一波；主迴圈裁決）
**近期(設計就緒，可即施工)**：W1 SvEcDataItem(在途)→W2 AlarmReportAck→**uHGemHT9045 Bucket 0**(swap-back，零行為變更、消除未來 churn)→**common.cpp 完成波**(最高槓桿：解 GetLastOpenFN+S7F18+≥4 gate，先前 SECSGEM Q2 選 B 暫留的 S7F18 由此解)→W3 Subsystems(3a 純 wire/3b 檔寫+TCheckListBox+吸收 VCL 分流 C，需先建 SaveEventReportData/檔寫 seam=HIGH#2)→VCW-1 cast 基礎→VCW-2 終端→S2F32 時鐘微波→DoDLRequest 測試 seam(獨立)。
**零成本清理波(可併)**：刪 5 個假 ODR-dup gate + atester_32Site.cpp:1698 補 MSG_CMD_Arm1Down + 修過時註解。
**測試衛生**：HIGH#1 cBootLog 測試隔離(獨立小波)。
**中長期(需前置波)**：FormsFacade/Handler-free-func 前置波(解 uHGemHT9045 牆②③)→uHGemHT9045 Bucket 1-5→W7 E0/M1/M2 HAL pump 群(含 AutoClean.cpp 大塊)。

### 🔖 RESUME（已被下方 2026-07-20 關機交接條目取代）
- **▶ SECSGEM Wave 1 寫入施工中；6 份唯讀交付物全數完成並裁決(2026-07-20)**。下次接續：(1) Wave 1 完成→審查+主迴圈五道閘+commit(SysModWire 以來的既定流程)。(2) 上列重排佇列施工，設計書全在 be8fe31a scratchpad(DESIGN_DoDLRequest_test_seam/VCL_widget_cast_wave/uHGemHT9045_siteoverride、RECON_W7_breakdown、AUDIT_test_hazards_tree/gated_inventory + gate_list.txt/gate_context.txt/extract_gates.py 可重用)。(3) 已裁決：DoDLRequest Q1=ctest 不起外部行程；uHGemHT9045 只 Bucket 0 近期可做(牆②③擋其餘)；common.cpp 完成波升為高槓桿優先。(4) 驗證基準 ctest 83/87(4 既有漂移)、套件 317/86/226/80/80/16/127；golden=`HT9011UC_Code_V3.33.906.0_20260618`；分支 fix/v899.32-pti；工作樹另有無關 V899/config 殘留(PTI 案)勿圈入。進度儀表板 https://claude.ai/code/artifact/26e3926c-c145-4699-9743-a7ae9db614e4。

---

## 2026-07-20 — SECSGEM Wave 1 完成 + 關機交接檢查點（今日收工）

**設定聲明**：主迴圈 Fable 5 + xhigh；翻譯 Sonnet 5；獨立審查 Sonnet 5。

**Wave 1 交付**（commit `21900fd`，7 檔 +2587/-50）：THGem 內嵌 `SecsSvEcRegistration SvEcReg`+DataItemOutSV/EC/NameList 族+Send* 4+FormCreate SV 註冊+新 `TasmInfo.{h,cpp}`(保守佔位:GetCPUFreq→0/GetManID→"GenuineIntel"/GetCPUType→""、disk/mem 真 Win32)。un-gate uHGemClass **13 支**(S1F4/S1F12/S1F24/S2F14/S2F30/S5F8/S6F16/S6F18/S6F20/S100F4/S101F6/S101F8/S103F12)，gated 36→**23**。test_uHGemEquipment 317→**390**。

**獨立審查 CLEAN**（clean-room fresh build 重現全部閘）＋三偏離裁決：**(A) 設計書 Q4 錯、翻譯對**——審查親 cp950 讀 golden，`THGem::DataItemOutSV/EC`(:2472-3113)用 implicit `this->`；設計書誤把它和 `HTGem::SetECValue`(uHGemClass.cpp:2699，那支才用 `HGem->`)混為一談且行號誤植檔案。**日後用 DESIGN_SECSGEM_closing_waves.md 的 Wave 2/3 章節時，Q4 前綴描述不可信、須實讀 golden**。**(B) 新 golden bug**：S6F18 錯誤路徑送 `InitLocalHead(6,16,0)` 應為 6,18(S6F16 複製貼上)，逐字保留+註記。**(C)** CMake 落根(對)、NULL 替代(len==0 不解參照)、SaveEventReportData 未觸達(.def 停 7/16)。

**主迴圈定案**：關機壓力下不跑第三次重建；秒級親驗 mojibake 0/7+SECS md5 一致，compile/ctest/resolving/390×3 由審查對抗性 clean-room build 背書後 commit。

### 今日全日戰果（fix/v899.32-pti，7 個完整波次/交付 commit + docs）
1. SysModWire 接線波 `799bcdb`（+docs `b703881`）— HSys.MyGem 接活、S,F tail 解閘、uHGemClass 13→21。
2. FastcallFix 小波 `7376490`（+`57b0635`）— MyDBIProcess ABI 雷結案，稽核 HIGH 清除。
3. TesterTCP Timer 波 `0dfecd9`（+`fd34995`）— TimerProcessTCPDataTimer 十分支 pump。
4. automation W906-AutoPB `b810c0e` + D5 `e4808c2`（+`4c85a60`）— ProcessBuffer 家族 OLP 派工核心。
5. 六路唯讀 recon 記錄 `ce414b0`（2 稽核+4 設計書）。
6. SECSGEM Wave 1 `21900fd`（本條）— uHGemClass 21→已解至剩 23 gated。
- 全部經獨立審查+主迴圈驗證。ctest 全程守在 83/87（4 既有環境漂移）。

### 🔖 RESUME（最新，關機交接 2026-07-20 16:4x）
- **工作樹狀態**：**乾淨**（Wave 1 已 commit `21900fd`；本 docs 條目 commit 後亦乾淨）。唯一非本專案殘留＝無關的 V899 `MachineType.h`/`config/*`/`setup.inf`（PTI 案，勿圈入 V906 commit）。分支 `fix/v899.32-pti`。golden=`HT9011UC_Code_V3.33.906.0_20260618`。
- **驗證基準**：ctest **83/87**（4 既有漂移 config_db/IniFiles/ini_helpers/config_loaders）；套件 test_uHGemClass 86(gated 23)、test_uHGemEquipment **390**、SecsWireCodec 226、testertcp_socket 80、interfacesys 80、TesterTCP 16、automation 127。
- **下一波(寫入單線，一次一波)＝SECSGEM 收尾群 Wave 2 `W906-AlarmReportAck`**：設計書 `DESIGN_SECSGEM_closing_waves.md`(be8fe31a scratchpad) Wave 2 章節；範圍 GetAlarmIndex/EnableDisableAlarm+Report-Ack composer+5 temp-list 成員，un-gate 9 支，零 CMake。**⚠ 讀該書時 Q4 前綴描述不可信(見上)、須實讀 golden 定 this->/HGem-> 慣用語**。
- **其後佇列(全部設計書就緒，見 ce414b0 條目的重排佇列)**：uHGemHT9045 Bucket 0(swap-back,唯一近期可做,牆②③擋其餘)→common.cpp 完成波(最高槓桿,解 GetLastOpenFN+S7F18+≥4 gate)→SECSGEM W3 3a/3b(3b 需先建 SaveEventReportData 檔寫 seam=稽核 HIGH#2)→VCW-1 cast 基礎(需 TStringList 移入 TObject 樹)→VCW-2 終端→S2F32 時鐘微波→DoDLRequest 測試 seam(裁決 ctest 不起外部行程)。零成本清理波：刪 5 個假 ODR-dup gate+atester_32Site.cpp:1698 補 MSG_CMD_Arm1Down。測試衛生：cBootLog 測試隔離(稽核 HIGH#1)。中長期：FormsFacade/Handler-free-func 前置波→uHGemHT9045 Bucket 1-5；W7 E0/M1/M2 HAL pump 群(含 AutoClean.cpp 9137 行,見 RECON_W7_breakdown.md)。
- **scratchpad 交付物清單**（be8fe31a\scratchpad，關機後仍在磁碟；若清空則各設計書可依 DEVLOG 摘要重派）：DESIGN_SECSGEM_closing_waves / DESIGN_DoDLRequest_test_seam / DESIGN_VCL_widget_cast_wave / DESIGN_uHGemHT9045_siteoverride / RECON_W7_breakdown / AUDIT_test_hazards_tree / AUDIT_gated_inventory（+gate_list.txt/gate_context.txt/extract_gates.py 可重用）。進度儀表板 https://claude.ai/code/artifact/26e3926c-c145-4699-9743-a7ae9db614e4。
- **整體進度估計(今日量測)**：邏輯翻譯 ~40-45%（行數面 golden 覆蓋 39.8%、檔面 32%）；含整合期(真 UI/實機 HAL/MSVC 64-bit/e2e pump)整體專案 ~30%。詳見本日「評估%」對話與 KNOWLEDGE。

---

## 2026-07-21 — SECSGEM Wave 2「W906-AlarmReportAck」完成（THGem alarm+report-ack 本體 + uHGemClass 再解 8 支）

**設定聲明**：主迴圈 Sonnet 5 + xhigh（本波起，主迴圈由 Fable 5 切換為使用者當下 session 預設 Sonnet 5；效率導向，機械翻譯階段適配）；翻譯 Sonnet 5；獨立審查 Sonnet 5。使用者指示「進行機械翻譯且火力全開，除非有異常、疑問或完成才能停下」，本波起連續推進、不逐波停下請示。

**Recon 修正遺失設計書**：原 `DESIGN_SECSGEM_closing_waves.md`(be8fe31a scratchpad) 在本 session 不可見；派 read-only agent 直讀 golden 重建範圍，**發現原估計有誤**：實際是 **8 支**(非 9)、**4 個 temp-list 成員**(非 5)——golden 自己的 `THGem::ProcessHostSendReportID()` 全樹零定義零呼叫點(確認死碼，不翻)。另發現 Report-Ack composer helper(`InitLocalHead`/`DataItemOut` 等)在 `THGem` 側根本不存在，需先補（`EnableDisableEventReportAcknowledgeError` 既有 no-op stub 正是等這批補齊）。**裁決：走 full-real 路徑**(補 `THGem::InitLocalHead`/`DataItemOut` 一行 forwarder 到自己的 `WireCodec` 成員，比照既有 `SendLocalData` forwarder 先例)而非再留 stub——阻塞點瑣碎、full-real 只多約 100 行、可消除未來債務。

**交付**（commit `4c2ec4c`，5 檔 +1325/-59）：`THGem::EnableDisableAlarm/EnableDisableAlarmAll/GetAlarmIndex`(golden uHGemEquipment.cpp:3337-3372) + 4 個 temp-list 成員(golden .h:530-533) + `InitLocalHead`/`DataItemOut` forwarder + `ProcessHostSendReportLinkID`/`ReportAcknowledge` 家族/`ReportLinkAcknowledgeError` 家族(golden :7838-8049)，`EnableDisableEventReportAcknowledgeError` 補真本體取代舊 no-op stub。`uHGemClass.cpp` 再解鎖 **8 支**(golden :1218-1890)：S2F34 三支(DefineReportAcknowledgeSub/ProcessHostSendReportID/DefineReportAcknowledge)、S2F36 兩支(LinkEventReportAcknowledgeSub/LinkEventReportAcknowledge)、S2F38_EnableDisableEventReportAcknowledge、S5F4_EnableDisableAlarmAcknowledge、S5F6_ListAlarmData(最大，265 行)。**uHGemClass gated 23→15**。

**翻譯 agent 自報並修正一個真回歸**：un-gate 後 `S2F34_DefineReportAcknowledgeSub`/`S2F36_LinkEventReportAcknowledgeSub` 的既有測試用 `HGemPtr==NULL` 呼叫會真 segfault——查證 **golden 本身該處也不 null-check `HGemPtr`**(非既有 guard 被翻壞，是 golden 這兩支本就不設防護，該情境在真實情境下不會發生)，比照先前 S1F1/S1F2 的「移出」先例，把這兩個檢查搬到 `test_uHGemEquipment.cpp` 用真接線 `THGem` 重做(`test_w906_alarmreportack_e2e`，21 斷言)。

**獨立審查 CLEAN**（1 個揭露性 LOW，不擋 commit）：clean-room fresh build 獨立重現全部聲稱；golden 逐行核對 8 支方法(含 S5F6 265 行)零未揭露差異；確認 `S5F6` 的 `DataItemOut(0, HType.BINARY_TYPE, &Mode)` 零長度 quirk 逐字保留(故意行為非 bug)；`HGemPtr->` vs `this->WireCodec` 無交叉污染；獨立 grep 確認 golden `ProcessHostSendReportID` 真零呼叫點；獨立讀 golden 判定 NULL-deref 測試移動合理非掩蓋；追蹤新測試 10 個區塊全部止步於 format-error 提前返回或僅觸及 scratch-dir 重導的 `WriteAlamData`，未觸達生產路徑；其餘 15 個既有 gated 方法/`uHGemHT9045.cpp` 均確認未動；AI tag 100% `AI(W906-AlarmReportAck) 20260721`；mojibake 0。LOW：`unsigned __int64→AnsiString` 需 `(long long)` cast(vclcompat 對該型別多載有歧義)，比照既有 `SecsWireCodec.cpp` 先例，已揭露不擋。

**主迴圈親自定案**（全新 `build_w906_alarmreportack_final`，本波起主迴圈換 Sonnet 5 親自執行五道閘，非委派）：build exit 0、`resolving`=0、`undefined reference`=0、`error:`=0；ctest **83/87**（同 4 個既有環境漂移：config_db/IniFiles/ini_helpers/config_loaders）；`uHGemClass`+`uHGemEquipment` 套件 ctest 內個別 Passed；mojibake 0/5；**`D:\SECS_GEM_LOGS` 11 檔全部 mtime 早於 2026-07-21，今日三次 build(翻譯/審查/主迴圈)均未觸碰**（首次檢查用 `/d/SECS_GEM_LOGS/*` glob 誤判為空目錄——只是 glob 不遞迴年份/日期子目錄的方法論失誤，非真異常；`find -type f` 遞迴後確認 11 檔存在且 md5/mtime 皆穩定）。

### 🔖 RESUME（最新）
- **✅ SECSGEM Wave 2「W906-AlarmReportAck」完成（2026-07-21，commit `4c2ec4c`）**。**下一波＝Wave 3 `uHGemHT9045 Bucket 0` swap-back**（刪 shim→真骨架 header 只宣告已翻 method、未翻者繼承 base，零行為變更、消除未來 churn；唯一近期可做的桶，牆②③(fMain fan-out/Handler free-func)擋其餘 5 桶——見 `DESIGN_uHGemHT9045_siteoverride.md` 摘要，原書已不可見，開工前同比照本波方式重新 recon golden）。**其後佇列**：common.cpp 完成波(最高槓桿，解 GetLastOpenFN+S7F18+≥4 gate)→SECSGEM W3 3a/3b(3b 需先建 SaveEventReportData 檔寫 seam=稽核 HIGH#2)→VCW-1/VCW-2 cast 波→S2F32 時鐘微波→DoDLRequest 測試 seam。零成本清理波：刪 5 個假 ODR-dup gate+atester_32Site.cpp:1698 補 MSG_CMD_Arm1Down。測試衛生：cBootLog 測試隔離(稽核 HIGH#1)。中長期：FormsFacade/Handler-free-func 前置波→uHGemHT9045 Bucket 1-5；W7 E0/M1/M2 HAL pump 群。
- **驗證基準更新**：ctest 83/87（同 4 既有漂移）；`uHGemClass` gated 23→**15**。golden=`HT9011UC_Code_V3.33.906.0_20260618`；分支 `fix/v899.32-pti`；工作樹另有無關 V899/config 殘留(PTI 案)勿圈入 V906 commit。
- **執行模式變更（使用者 2026-07-21 明確指示）**：「進行機械翻譯且火力全開，除非有異常、疑問或完成才能停下」——本波起主迴圈由 Fable 5 切為 Sonnet 5 + xhigh，連續推進多波不逐波停下請示；仍在每個真異常/疑問點停下澄清（本波唯一疑似異常＝SECS_GEM_LOGS 初次 glob 誤判，已自行查證排除，非真異常，未停下請示）。

---

## 2026-07-21 — uHGemHT9045 Bucket 0 swap-back 完成（shim→真名永久骨架，零行為變更）

**設定聲明**：主迴圈 Sonnet 5 + xhigh；翻譯 Sonnet 5；獨立審查 Sonnet 5。

**Recon 修正遺失設計書**（`DESIGN_uHGemHT9045_siteoverride.md` 同樣在本 session 不可見）：派 read-only agent 直讀 golden `uHGemHT9045.h/.cpp/_SV.cpp/_EC.cpp` 重建範圍。確認核心結論成立：class `HT9045Gem:public HTGem`，golden .cpp 本體合計 **9140 行**(6215+1013+1912，非單一 6215 檔)，**22 個** virtual override(精確非約略，含 `S2F42_Host_Command_Acknowledge` 單支 ~3044 行的巨無霸)。三道牆逐一直讀驗證成立：牆①(THGem/wire/SvEcReg)已由近期 SECSGEM 波解；牆②(`fMain->` 82 處觸及 28 個成員，`FormsFacade.h` 現缺 ~19 個+1 部分)；牆③(`GetAlarmCodeList`/`ProcessLotInfo` 全樹未翻，`DoAutoRetest` 被 csystem.cpp 一個 TU-local no-op 巨集遮蔽不可鏈接真版，`SaveAllFile` 宣告無定義會在連結期炸)。**Go/no-go 裁決：GO**——現有 shim 本就零宣告 22 個 override(已經是「骨架」)，`database.h` 已用 base class `HTGem*` 持有(虛派發，換具體類別對呼叫端透明)，純結構搬遷不涉邏輯翻譯。**唯一風險點**：shim 建構子帶有 golden 沒有的 port 專屬 `ActiveWire` rebind(橋接兩個 `SecsWireCodec` 實例)，若照「golden-mirror」直覺重譯建構子會靜默丟掉這段+讓既有測試斷言(`test_uHGemEquipment.cpp` W1(a)/W1(b))壞掉——recon 明確標注「複製 shim 現有建構子逐位，不得從 golden 重譯」。

**交付**（commit `ee85c32`，5 檔 +101/-94，git 自動偵測為 rename）：刪 `SECSGEM/uHGemHT9045_Shim.{h,cpp}`→新建 `SECSGEM/uHGemHT9045.{h,cpp}`(同名同 base、單一建構子、刻意零宣告 22 個 override)；建構子逐位複製自 shim(含原 `AI(W906-SysModWire) 20260720` 註解一併保留，未動)；`database.cpp` include 改向、`CMakeLists.txt` `ht9045_secsgem` library 原始檔清單改向(本波唯一需要動 CMake 的一步，因是結構搬遷非邏輯翻譯)。**未翻的兩個資料成員**(`TAsyncRCMDState m_AsyncRCMD`/`AnsiString EventDescription[...]`)裁決：兩者皆不加，留給未來實際用到的 bucket，避免骨架波 scope creep。

**獨立審查 CLEAN**（1 個 LOW 揭露、不擋）：clean-room fresh build 獨立重跑；逐位比對建構子與 git 歷史中的舊 shim 內容確認 byte-for-byte 相同；grep golden 22 個 override 名稱確認新 header 一個都沒宣告；確認 `aArmHeader.h` 的 dormant include(仍 `#if 0`)透過 `ht9045_secsgem` 的 PUBLIC include 目錄若未來解閘會正確解析到新真檔、無路徑陷阱；`nm -C` 掃描 87 個測試執行檔確認恰 3 個含 `HT9045Gem::HT9045Gem` 符號(與宣稱一致)。LOW：一個既有測試(非本波)寫入的 scratch 副產物檔未在自報中提及，已確認 untracked/gitignored、不影響本波 diff。

**主迴圈親自定案**（全新 `build_w906_bucket0_final`）：build exit 0、resolving/undefined reference=0、ctest **83/87**(同 4 既有環境漂移)、mojibake 0/4。

### 🔖 RESUME（最新）
- **✅ SECSGEM Wave 1(`21900fd`)+Wave 2(`4c2ec4c`)+uHGemHT9045 Bucket 0(`ee85c32`)三波連發完成（2026-07-21）**。`uHGemClass` 累計 42/57 已解(gated 57→15)；`uHGemHT9045` 容器已真名落地，但 22 個 override+AddSV+AddEC(~9140 行)本體仍 0 翻，牆②③未解、Bucket 1-5 暫不可做。**下一波候選(尚無設計書，開工前先 recon)**：`uHGemClass.cpp` 剩 15 個 gated method、SECSGEM 4 個獨立子系統(DoSpool/Trace/上下傳)+FormCreate SV 註冊、**common.cpp 完成波(最高槓桿，解 GetLastOpenFN+S7F18+≥4 個下游 gate，只需既有 vclcompat 檔案 API)**、VCW-1/VCW-2 cast 波(需先讓 TStringList 入 TObject 樹)、S2F32 時鐘微波、DoDLRequest/DoULRequest 防護 seam(裁決：ctest 不起外部行程)、`SCK_ART.cpp` 剩餘協調債。中長期：FormsFacade/Handler free-func 前置波(解牆②③)→uHGemHT9045 Bucket 1-5；W7 E0/M1/M2 HAL pump 群。
- **驗證基準**：ctest 83/87(同 4 既有漂移)；`uHGemClass` gated 57→15。golden=`HT9011UC_Code_V3.33.906.0_20260618`；分支 `fix/v899.32-pti`；工作樹另有無關 V899/config 殘留(PTI 案)勿圈入 V906 commit。
- **執行模式**：使用者 2026-07-21 指示「機械翻譯火力全開，除非異常/疑問/完成才停下」持續有效，本波起第三個連續波次，過程無真異常/疑問，未停下請示。

---

## 2026-07-21 — common.cpp「wave-file」叢集完成（6 函式解鎖 + 修正上一版 recon 誤判 + 2 個真 golden bug 發現）

**設定聲明**：主迴圈 Sonnet 5 + xhigh；翻譯 Sonnet 5；獨立審查 Sonnet 5。

**Recon 修正先前唯讀稽核的誇大宣稱**：`AUDIT_gated_inventory.md`(遠端稽核，本 session 不可見原文，僅信其結論作為線索)聲稱此叢集「解鎖 ≥5 個下游檔案，含 SECSGEM S7F18」。派 read-only agent 逐一直讀驗證，**結果修正**：S7F18 **不解鎖**(卡 `THGem::UpLoadPath` 缺成員 + `DeleteDirectory` 全樹未翻，兩者皆與 common.cpp 無關)；`BarCode_Bottom2DID8CCD.cpp` **本就能跑**(靠 `acatchtray.cpp` 既有 no-op stub，此波只是把它從 no-op 升級成真寫檔，非解鎖新功能)。真正、驗證過的解鎖＝`Automation/AGV_E84.cpp`(2 處)+`Automation/HANA_ART.cpp`(1 處)+`cpublic.cpp`(3 處可淨解，但另有架構阻塞，見下)。

**交付**（commit `38157cc`，8 檔 +626/-62，新增 `tests/test_common.cpp`）：`common.cpp`/`.h` 解鎖 `WriteDataToFile`(2 多載)/`CheckFileIsEmpty`/`ReadDataFromFile`/`MyForceDirectories`/`GetLastOpenFN`/`WriteLastDataFN`(golden :1252-1721)。新增 `common.cpp` 本地 `static ShowMyMessage` no-op stub(比照既有 `RecordProcess` 前例，避免 `ht9045_core`→`ht9045_sm` 形成 CMake link cycle)。`WriteLastDataFN` 首跑分支用既有 `TStringList` idiom 替代 golden 的 `FileCreate`/`FileWrite`/`FileClose`(vclcompat 無此三支 shim，且 `LastDataPath` 全 golden 只有 `GetLastOpenFN` 一個讀者，功能等價)。**必要配套**：刪 `acatchtray.cpp` 既有全域 `WriteDataToFile` no-op stub(否則真本體解鎖後與之在最終連結期產生 multiple-definition 錯誤)；`cpublic.cpp` 3 處嘗試後**主動擱置**——實測發現 `ht9045_globals` library 未 link `ht9045_core`，多個測試 target 會斷鏈，留 gated 加註解說明(遵循「無法安全解就不要硬解」的既定紀律)。

**兩個真 golden bug 發現(1 修 1 記錄)**：
1. **翻譯期間自己引入又自己抓到並修正**：golden `WriteDataToFile` 的 `char*`(非 const)多載，在標準 C++ 下 `AnsiString::c_str()`(回傳 `const char*`)無法綁定該多載，靜默改綁 `AnsiString` 多載，而該多載的一行 forwarder 又呼叫自己→**無窮遞迴**(獨立審查以 gdb backtrace 21600+ 層佐證)。修正：該多載參數改 `const char*`(fopen/fputs 皆不改動字串內容，行為等價，非邏輯變更)。
2. **`CheckFileIsEmpty` 語意與函式名相反**(記錄不改)：`bResult` 預設 `true`；只有「開檔成功且第一個 `fgetc` 立刻 `EOF`」才設 `false`。即：檔案不存在或非空皆回 `true`，只有「存在且真的是空檔」才回 `false`——與函式名字面意義相反。獨立審查逐行重新推導 golden 確認無誤，新測試斷言的是這個真實(反直覺)行為。
3. **`ReadDataFromFile` text-mode CRLF bug**(記錄不改)：golden `fopen(...,"r")` 是文字模式，Windows CRLF→LF 轉換會讓實際讀到位元組數少於用 `ftell` 算出的檔案大小，任何含換行的檔案結尾會有未初始化垃圾位元組(在強制補的 NUL 之前)。新測試只斷言確定性前綴、不做全字串相等比對，以避開這段 UB。

**獨立審查 CLEAN**（3 項高風險自報宣稱全部獨立重新推導確認屬實，非照單全收）：(1) 無窮遞迴成因與修法都獨立驗證正確；(2)/(3) 兩個 golden bug 都獨立逐行重讀 golden 確認邏輯正確；`ShowMyMessage` stub 簽章比對 golden 呼叫點皆吻合；`acatchtray.cpp` stub 刪除確認乾淨無殘留；`cpublic.cpp` 擱置理由查證屬實(且發現受影響 test target 比自報的還多，強化「不硬解」的判斷)；新測試全部限定 scratch 路徑；範圍紀律確認其餘所有 `wave-*` gated 區塊皆未被動到。

**主迴圈親自定案**（全新 `build_w906_commonwavefile_final`）：build exit 0、resolving/undefined reference=0、ctest **84/88**(同 4 既有環境漂移——本波進一步查明成因＝本機真實 `system/{Mot_Table,IO_Table,Gerneral.ini}` 快照列數與測試硬編碼 oracle 不符[如 44 vs 期望 45 列 SMC]，屬環境漂移非回歸)、mojibake 0/7。

### 🔖 RESUME（最新）
- **✅ 2026-07-21 四波連發完成**：SECSGEM Wave 1(`21900fd`，上日)+Wave 2「AlarmReportAck」(`4c2ec4c`)+uHGemHT9045 Bucket 0(`ee85c32`)+common.cpp wave-file 叢集(`38157cc`)，各附獨立審查+主迴圈五道閘+docs commit。**寫入佇列已清空**。**下一波候選(尚無設計書，開工前先 recon)**：`uHGemClass.cpp` 剩 15 個 gated method、SECSGEM 4 個獨立子系統(DoSpool/Trace/上下傳)+FormCreate SV 註冊、VCW-1/VCW-2 cast 波、S2F32 時鐘微波、DoDLRequest/DoULRequest 防護 seam、`SCK_ART.cpp` 剩餘協調債、或評估 `ht9045_globals`→`ht9045_core` link 擴大(解 cpublic.cpp 3 處僅存阻塞)。中長期：FormsFacade/Handler free-func 前置波(解 uHGemHT9045 牆②③)；W7 E0/M1/M2 HAL pump 群。
- **驗證基準**：ctest 84/88(同 4 既有漂移，成因已查明＝本機 system/ 快照列數漂移)；`uHGemClass` gated 57→15。golden=`HT9011UC_Code_V3.33.906.0_20260618`；分支 `fix/v899.32-pti`；工作樹另有無關 V899/config 殘留(PTI 案)勿圈入 V906 commit。
- **執行模式**：使用者 2026-07-21 指示持續有效，本波為當日第四個連續波次，過程無真異常/疑問，未停下請示。
