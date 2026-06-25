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
- ✅ C++ pivot、檔案鏡射慣例、ContactForce 計算核心（W2 一部分，先行示範）。
- ▶ 進行：**W0 基礎**（vclcompat 層 + CMake 骨架 + 首批 Public 葉工具 HTMD5/cJSON 驗證）。
