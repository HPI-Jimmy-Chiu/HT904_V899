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

### 🔖 RESUME（最新）
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
- 驗證指令：`cd HT9011UC_Cpp_V3.33.906.0 && export PATH=/c/MinGW/bin:$PATH && cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=C:/MinGW/bin/g++.exe -DCMAKE_C_COMPILER=C:/MinGW/bin/gcc.exe && cmake --build build && ctest --test-dir build`。
