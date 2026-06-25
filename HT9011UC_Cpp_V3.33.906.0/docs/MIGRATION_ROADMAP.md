# HT9045 V906 BCB6 → C++ 全面遷移順序（ROADMAP）

> 目標：把 `HT9011UC_Code_V3.33.906.0_20260618`（BCB6/VCL C++，唯讀黃金參考）**全部**翻譯成標準/Visual C++。本檔是總路線；逐步紀錄見 `DEVLOG.md`，知識/gotcha 見 `KNOWLEDGE.md`。

## 關鍵策略（keystone decisions）
1. **VCL-compat 層（最關鍵）**：在 `vclcompat/`（附加目錄）自製「只實作專案實際用到的 API 子集」的相容層——`AnsiString`（**1-based** `Pos`/`SubString`/`Length`/`UpperCase`/`Trim`/`sprintf`/operator+ 等，語意完全照 BCB6）、`TStringList`、`TDateTime`、SysUtils 風格自由函式（`IntToStr`/`StrToInt`/`Format`/`FileExists`/`Now`/`FormatDateTime`…），以及巨集中和（`__fastcall`/`__closure`→空、`__property`→getter/setter 指引、`PACKAGE`/`DELPHICLASS`→空）。**好處**：大量檔案可忠實、保名、低風險地機械翻譯（避開 ~1,910 個 1-based off-by-one 改寫），日後可選擇性重構成 idiomatic std。翻譯檔以 `#include "vclcompat/vcl_compat.h"` 取代 `<vcl.h>`。
2. **檔案鏡射**：翻譯出的 `.cpp/.h` 鏡射 BCB6 相對路徑+檔名（見 KNOWLEDGE）；附加檔（`vclcompat/`、`CMakeLists.txt`、`tests/`、`docs/`）才用新結構；`.bpr`→CMake。
3. **建置/驗證**：CMake 鏡射樹 + MinGW g++（本機無 MSVC；最終 MSVC build 在開發機）。每模組有 golden oracle 就對拍（MD5 向量、真實 config/log 檔、手算公式值）；無 Borland 故不對原 binary live diff（已知限制）。
4. **硬體/UI 接縫**：即時硬體核心保留 native、同程序直呼；硬體與 UI 走抽象基底 + Sim/Real、Stub/真UI（§9 HAL）。**UI 框架（MFC/Qt/Win32）延到 W7 再定**（不擋 W0–W6）。
5. **節奏**：一波一波、一個模組一個 workflow；每批完成自動 commit + 更新 DEVLOG/KNOWLEDGE/RESUME。這是長期多輪工程。

## 翻譯順序（低風險/底層 → 高耦合/UI）
- **W0 — 基礎**：`vclcompat/` 相容層 + CMake 鏡射骨架 + 翻譯全域定義/型別標頭（`MachineType.h`、`cmydef.h`、`cprod.h`、`cpublic.h`、`MachineDefine.h`…）去 VCL 化。**出場**：基礎可編、compat 層單元測試過。
- **W1 — Public/ 葉節點工具**：`cJSON`(純C)、`HTMD5`(MD5 標準向量對拍)、`MyStringList`、`HonNewDel`、`MemoryAlarm`、`WinSocketErrorCode`、`HTMD5`、`systools` 等。**出場**：各自可編 + golden 測試。
- **W2 — 純邏輯島**：`Common/PickPlanner`(cArmPickPlan/cInArmLoaderPickPlanner/cOutArmPlacePlanner)、`SortingBinTray`、`cUnitConvert`、`ContactForce`(✅已翻計算核心) + `cContact` 計算消費端、`ContactForce::ReadFile`。
- **W3 — 設定/DB/基礎服務**：`cinitial`(ini/csv/dat 讀取，名稱式解析)、`cMyDB`+sqlite3(可攜C)、`database.cpp`、`common.cpp`(SGDToCSV 等)、`ExternFunction`、`handlerlog`、`language`。
- **W4 — 硬體抽象層(HAL)**：`HTMotor` 基底 + `mymotor` 派發 + 各廠牌馬達(CSMC/Galil/MN200/Syntek/EtherCAT) 後接抽象基底 + Sim；`myio`/`MyLaneIo`/`myswitch`/`mysensor`/`mycylin`/`mykitsuck`(IO)；KeyPro shim；tester(GPIB/TCP/Sim)；`TempCtrl` 驅動。廠商標頭唯讀；real 接 native、Sim 供離線。
- **W5 — 通訊/協定**：`SECSGEM`(uHGemHT9045…)、`Interface/TesterTCP`/`InterfaceSYS`、`KYECFTP`、`Automation` 網路、`CanBus`/`CCLink`/`MyPLC`/`EtherCAT` 通訊、`MR/RFID`。
- **W6 — 邏輯狀態機（root，最大宗）**：`ainarm9045*`/`aoutarm9045*`/`atester*`/`asendic*`/`acatchtray`/`ShuttleMove`/`auto9045`/`automation`、`csystem`/`cprod`/`cpublic` 邏輯、`ckernel`；切 `Prod`/`iXXXTask` 全域狀態邊界。
- **W7 — UI（133 .dfm 表單）**：先定 UI 框架（MFC 最近 VCL / Qt 現代 / Win32）＝**獨立大決策**；逐表單重建於 view 介面後，接已翻邏輯。最大、最後。

## 進度
- ✅ C++ pivot、檔案鏡射慣例、ContactForce 計算核心（W2 先行示範）。
- ✅ **W0 基礎完成**：vclcompat 層（AnsiString 1-based 等）+ CMake 鏡射骨架 + Public/cJSON + Public/HTMD5；ctest 4/4、MD5 命中 RFC1321。
- ✅ **W1 第一批 Public 葉工具**：`ExternFunction`(純函式部分)、`WinSocketErrorCode`(GetErrorMsg)、`cBootLog`；ctest 7/7。
- ✅ **W2 邏輯島（部分）**：`Common/PickPlanner` 通用引擎 cArmPickPlan（與 golden 幾乎逐字同，僅換 include）+ 兩個 scaffold adapter（gated off）；`cUnitConvert` 純轉換部分。ctest 9/9。
- ✅ **W0 尾段＝全域標頭完成**：`MachineType.h`/`Config.h`/`CosFunction.h`/`myTimer.h`/`cpublic.h`/`cprod.h`/`cmydef.h`/`MachineDefine.h` 全部去 VCL 化、**單一 TU 一起編可過**（ctest 11/11，含 globals：PROD_INFO_ST 的 static_assert/offsetof 錨點 + enum/#define 值對拍）。**瓶頸已解除**。form 指標前置宣告→W7；.cpp 觸及狀態機的 body `#if 0`→W3/W6。
- ◐ **W3 config/DB — 前置已備，readers 待續**：已加 vclcompat 檔案系統 API（FindFirst/FindNext/FindClose/TSearchRec/fa*/RemoveDir/FileSetAttr/FileGetAttr/HexStrToInt）、`third_party/sqlite3/` 外部相依 scaffold（現 header-only stub；待 vendor amalgamation）、`tests/test_config_db.cpp` 真檔 oracle（鎖定 45 SMC/MOTION=1/IO=2/IO_Table 644:24）、修 vclcompat TStringList SetCommaText/SetDelimitedText 尾分隔重複計數 bug。ctest 12/12。
- ✅ **W3-cont 閘門完成**：vclcompat `TIniFile`/`TMemIniFile`（IniFiles.h/.cpp，write-through/eager-load、Big5-safe、case-insensitive、default-fallback；43/43 shim 測試）+ `common.cpp` ini-helper 家族 38 函式（OpenGeneralIniFile/CheckAndReadIniDataGeneral 多載/WriteIniDataGeneral）。**真檔 oracle 過**：經譯後 helper 鏈讀真 Gerneral.ini 得 MOTION=1/IO=2/TTL=2/INDEX=0/HEATER=4、missing→default。ctest 14/14。
- ✅ **W3-cont2 config-table loaders 完成**：database.cpp 的 LoadMotData/LoadIoData/SetMOTTableNo/SetIOTableNo/TIODATA/TMOTDATA（8 函式）翻譯，**真檔 oracle 過**（45 列全 SMC、名稱式 col 解析證實、IO 668 列 ISABase 644:24、M00→MInArmX）。Win32 巨集 guard 進 vcl_compat.h。修 AnsiString 缺 operator=(int) bug。ctest 15/15。
- ✅ **W4 HAL 馬達層完成（interface-cut 離線證明）**：HTMotor(虛擬基底)+TMyMotor+新 `TMySimMotor` 翻譯，**離線零 vendor SDK 可編可跑**（test_sim_motor 33/33：home→0、MoveToPos(N)→N、no alarm、MotorHome 狀態機收斂；nm/strings 稽核零 vendor 符號）。**6 個品牌驅動 → W4-part2**（每個 .cpp 都拉 ~30 模組 god-stack [main/csystem/sensors/cylinders]，故卡在 W6，與 vendor SDK 無關；Sim 先頂著）。mymotor 大量 motion/Galil/sensor 方法 gated TODO(W6)、pHTray→W7。ctest 17/17。
- ✅ **W4-IO HAL 完成（IO interface-cut 離線證明）**：`IOBackend.h/.cpp`（TIOBackend 虛基底 + TSimIOBackend 離線；real backends mn_*/Acm_*/_mnet_*/raw `#if HAVE_xxx` 預設 OFF）+ `MyLaneIo`(facade，6 method 經 pIO 派發、保留 range/OutPortData 記帳) + `myswitch`(TMySwitch)/`mysensor`(TMySensor) 物件層路由到 facade。test_sim_io 等通過、ctest 19/19。
- ▶ **下一步：HAL Sim 收尾（KeyPro + TempCtrl）** 補齊本機 active 硬體的離線 Sim 層（馬達✓/IO✓/license/溫控），讓 W6 狀態機能對全 Sim HAL 離線端到端跑。tester 較纏(歸 W5/SECS)。
- ⏳ 待：W4-part2 品牌馬達驅動(需 W6)、cylinder/sucker 狀態機+TMyKitSuck(W6)、database.cpp ReadGeneralIni/SECS、cMyDB(待 vendor sqlite)、MyStringList、common.cpp 其餘、W5 comms、W6 root、W7 UI。

## 延後項目追蹤（DEFERRED — 完整性，勿遺漏，全部轉移用）
> 部分檔案只翻了 leaf 部分，耦合段延到對應波次。最終各波結束前要回頭補完這些。
| 來源 | 延後的部分 | 目標波 | 原因 |
|------|-----------|--------|------|
| Public/ExternFunction | StringGrid_Insert_Row/Delete_Row、StatusBar_ItemText、ShowRecordTime | W7 | VCL TStringGrid/TStatusBar/TEdit |
| Public/ExternFunction | DeleteDirectory | W3 | 需 SysUtils FindFirst/FindNext/TSearchRec shim（未在 vclcompat） |
| Public/WinSocketErrorCode | LogClientSocketExceptionError | W5 | 耦合 VCL TClientSocket(Name/Address/Port)+MyDBIProcess |
| Public/MyStringList | 整檔 | W3 | 耦合 IniConfig/RunInfo/LastSet/CUSTOMER_CODE、VCL 表單、Win32 file |
| Public/MyProductionRecord | 整檔 | W6 | Prod/IniConfig/RunInfo/CUSTOMER_CODE、state-machine 消費端 |
| Public/HonNewDel | 整檔 | W7 | template 依賴 MemoryAlarmForm（VCL form）|
| Public/MemoryAlarm | 整檔（本身是 TForm）| W7 | VCL 表單 |
| Public/HTEdit, HTEditList | 整檔 | W7 | VCL 編輯控制項 |
| vclcompat 待補 | SysUtils FindFirst/FindNext/TSearchRec、faAnyFile/faDirectory | W3 前 | DeleteDirectory 等檔案搜尋需要 |
| cUnitConvert | Do*Convert glue (DoTestIFConvert/DoDeviceConvert/DoHotPlateConvert/DoArmOffsetConvert/…) | W6 | 依 cprod.h/cmydef.h 結構 + fShowMessage/ATC VCL form |
| SortingBinTray | 整檔 | W6 | main.h/MyMotor/mycylin/atester/uLotInfo + cmydef TSortingBinTray_* + AutoForm[] VCL form |
| Common/PickPlanner adapters | cInArmLoaderPickPlanner/cOutArmPlacePlanner 的 Search 本體（現為 scaffold，gate 預設 false）| W6 | 綁 MOT[].Tray.Data / InArmSuck / OutArmSuck（未翻全域）|
| cmydef.cpp / cprod.cpp / cpublic.cpp | 觸及 main.h/csystem/rs232/MyMotor/state machine 的函式 body（現 `#if 0`）| W3/W5/W6 | 等對應模組翻完 |
| cmydef.cpp | `TMyLog myLog` 全域實例定義（header 已前置宣告）| W7 | TMyLog 綁 VCL form/TWinControl |
| cmydef.cpp | 5 個 `TMyStringList*` 目標定義 | W3 | TMyStringList 在 Public/MyStringList.h(__property+MyMemo.h) |
| MachineDefine.h | driver/app include 區塊 | W3/W6/W7 | 等驅動/app 模組 |
| database.cpp | ReadGeneralIni(~1240行)、SystemModularInitial(SECS HT9045Gem)、TDataModule1(BDE)、InstallColorBinDisplay | W5/W6/W7 | 牽連 SECS/BDE/UI；本批只翻 config-table loaders |
| cpublic.cpp | queue/union/VerInfo 方法本體（現整檔 #if 0）| W6/W7 | cmydef.cpp 的 active queue 全域(QueueTaskList[]…)需其 ctor；**任何用到 queue 的 exe 連結前必須先 ungate**（目前測試用 stub ctor 繞過）|
| Motor 品牌驅動 ×6 (mySMCmotor/myGALILmotor/myMN200motor/mySYNTEKmotor/myEthercatmotor/HTMC88X1Motor) | 整檔 | W4-part2(需 W6) | 各 .cpp 拉 main.h/csystem/sensors/cylinders god-stack（卡 W6）+ vendor 呼叫需 #if HAVE_CSMC/GALIL/MN200/SYNTEK/ETHERCAT/MC88X1(預設 OFF，無 SDK)。vendor 符號族見 W4 workflow 紀錄 |
| Motor/mymotor.cpp | MotorMovePosition/MotorMove/Galil 分支(~80)/sensor compare/TrayArm/continuous-move 本體（gated）| W6 | 拉 sensor/cylinder/狀態機；pHTray(VCL)→W7 |
| mycylin (TMyCylinder) | Push/Pop/On/Off/SetSimulateCompoment 狀態機（IO 路由 method 已翻）| W6 | 拉 Alarm/SystemStart/bHandlerPause/SmartDiagnostic/VCL TControl |
| MyKitSuck | TMySucker Suck()/Destroy() 狀態機；**TMyKitSuck 整個 item-state grid（2894 行）** | W6 | TQPF_Timer/alarm；TMyKitSuck 拉 TALed/TMyProductionRecord/TTrayMotor/MyMotor/cprod IC 常數（god-stack，非 IO-HAL）|
| myio.cpp | raw-port free funcs（outportb/IOSetOutport/TTL_CARD_TYPE）| W6/x86-dead | 本機 TTL_CARD_TYPE>0 短路、ISABase 全 0→實際不走；x64 無 |
