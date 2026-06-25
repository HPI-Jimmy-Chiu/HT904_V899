# HT9045 V906 BCB6 → C++（Visual C++）累積知識（KNOWLEDGE）

> 決策、接縫知識、gotcha。逐步累積，供備查。逐步紀錄見 `DEVLOG.md`。
> 目標：把 BCB6/VCL C++ 翻譯成標準/Visual C++（脫離 Borland 方言），硬體與 UI 用 interface 切割（先 stub 後接）。**非 C#**（C# 方向已停用，git `326df6c` 可回溯）。

## 架構（ports-and-adapters，以 C++ 實作）
- 「port」＝**C++ 抽象基底類別（純虛擬）**。能沿用既有 C++ 多型基底就沿用（如運動的 `HTMotor` 本來就是虛擬基底）。
- 編排/邏輯層只依賴抽象基底，不直接依賴具體驅動或 UI 框架。
- 每個硬體 port 與每個 UI view 都有兩個可換實作：**Sim/Stub（開發、無硬體）** 與 **Real/真 UI（之後接）**。
- 由一處 **composition root / factory** 依設定選 Sim↔Real、Stub↔真UI（取代 `cinitial.cpp:3454-3565` 的 dispatch ladder）。
- 即時硬體核心策略（沿用 §9 HAL）：先以 stub/SOFT_SIMULTE 讓離線版可編可跑，再逐接縫換真實驅動、邊測邊接。

## 檔案位置與命名（鏡射 BCB6，使用者指令）
- **翻譯出的 .cpp/.h 一律鏡射 BCB6 原始檔的相對路徑＋檔名**，方便 1:1 反查。例：原 `HT9011UC_Code_..._20260618\ContactForce.cpp` → 譯為 `HT9011UC_Cpp_V3.33.906.0\ContactForce.cpp`（同在 root、同名）；原 `Motor\mymotor.cpp` → `Motor\mymotor.cpp`。**不要另開 src/ 改路徑**。
- 無 BCB6 對應的「附加檔」可用新結構：`CMakeLists.txt`、`tests/`、`docs/`（這些原本是 .bpr/無對應）。`.bpr` → 以 CMake 取代（建置系統不同，屬合理偏離）。
- 例外（無法照做時提出）：含特殊字元/空白的原路徑（如 `ASE_K Socket\`）照樣鏡射但建置需處理空白；`.dfm` 表單不直接鏡射成 .cpp（UI 走介面承接，另議）。

## 已翻譯模組 + 領域發現
- **ContactForce（已翻計算核心）**：`ContactForce.cpp/.h` 其實是 **VCL 表單 `TfContactForce` + 四個 SLK 元件類別**（THTSLKClass / THTSLKIndClass / THTDieForceSLKClass / THTDieForceOneByOneSLKClass），是 **SLK 直徑→力 的資料產生端**。文件常提的 `CalculateTotalAirForce`/`GetMinForce`/`GetMaxIndexForceLimit` 其實在 **`cContact.cpp`（VCL 表單 TfContact，計算消費端）**，讀 `fContactForce->SLKClass[i]->dDiameter/dMinForce/dMaxForce`。已抽出 ContactForce 的純計算核心為 C++（`SlkForceData`/`ComputeSlkForce`/`ComputeEpMaxVoltage`，root 鏡射）。
- 翻譯慣例（已確立）：extract-calc-core（VCL 表單/widget/IO 不翻，純算抽成可測函式，輸入用 struct/參數）；保留字面常數（如 `3.14` 不換 M_PI 以維數值同一）；特例綁對的類別（如 402 僅 STANDARD）；caption-only 變數（iCount）不影響數值。
- 驗證模式（無 Borland）：CMake(MinGW Makefiles)+g++ 6.3 編譯（`-Wall -Wextra -Wshadow -Wconversion -Wpedantic` 零警告）+ CTest；數值對「手算 BCB6 公式值」（非 live diff，已標限制）。

## 命名對照（鏡射 BCB6，供反查；C++ 端保留原名/原方法名）
- 運動：`HTMotor`(Motor/HTMotor.h) 已是虛擬基底（~50 virtual）；方法保留 InitMotor/MoveToPos/ReadPos/HomeObject/HomeFlag/Stop/MotionDone/GetAlarm/SetSpeed(uint,bool=false)/**ServerOnOff**(BCB6 拼法)/SetSoftLimit/JogP/JogN/ScanMotorStatus。加一個 Sim 子類即可。
- IO：`TLaneIO`(MyLaneIo.h)：IOBitOn/IOBitOff/IOByteOut/IOOutBitStatus/IOInputBit/IOInputByte（int 回傳，<0=fail）。
- 授權：`KEYPRO_GET_LEVEL`(Public/HTKeyPro.h, HS_Function.cpp:4933)。
- Tester：`ITesterInterface` 概念（main.h:1213/1277/1337/1392/1400）：Find/RunTestProgram/SendMSG_CMD/CloseGpibProgram/GetTesterResult。
- 全域：`PROD_INFO_ST Prod`(cprod.h)；`iArmTask` 系列。

## 建置 / 工具（本機）
- **g++ 6.3.0 (MinGW) + CMake 4.0.2；無 MSVC/clang**。翻譯成**可攜標準 C++**（建議 C++14/17），用 CMake + g++ 編譯驗證可攜性；最終 Visual C++/MSVC build 在開發機驗。
- 純邏輯/計算 class 可獨立 CMake target + g++ 編譯 + 簡單測試框架（或自寫 assert main）驗證，無需 VCL/硬體。

## Gotcha（踩過/要注意）
1. **AnsiString 1-based vs std::string 0-based（最高風險）**：`AnsiString.Pos()`/`.SubString()` 是 **1-based**（全專案 ~1,910 處）。直翻成 `std::string`(0-based, npos) 會 off-by-one／切錯字串且**編得過**。翻譯時逐處改 index，或先做一個 1-based 相容的 AnsiString-like 包裝（`.Length()/.Pos()/.SubString()/.UpperCase()/.sprintf()` 同名同語意）降低風險。`.UpperCase/.LowerCase/.sprintf/.Trim/.Delete` 在 std::string 無對應，需 helper。
2. **Big5 編碼**：906 原始碼是 Big5。翻譯出的 C++ 檔若含繁中字面值，需確保編譯器以正確 code page 讀（MSVC `/source-charset`；或避免在碼裡放中文、字串走外部資源/Big5 I/O 邊界）。執行期：外部 Big5 資料在 I/O 邊界轉碼，內部統一一種表示；加 U+FFFD 檢查。勿把 Big5 原始碼整檔轉 UTF-8（見記憶 ht9045-big5-edit-corruption）。
3. **CSV 名稱式解析**：`database.cpp`(SetMOTTableNo/SetIOTableNo) 用 **header 名稱**(AnsiPos、last-match) 解析 Mot_Table/IO_Table，**非固定位置**；**Mot_Table 實體欄序 ≠ emot* enum 序**(database.cpp:2047-2079)。翻譯時務必做名稱式解析，勿位置式（會 silently 誤對馬達/IO）。真實 header：Mot_Table 29 欄、IO_Table 15 欄。
4. **config 語意細節**(database.cpp)：IO 欄數須 ==15；Mot 接受 >=28；空 cell：IO Lane/ModuleType/IP/Port/Bit→-1、InType→0、ISABase→0(eMotionNet)、Enable→0；**Port 當 ISABase∈{1,2,4} 以 HEX 解析**(database.cpp:1825-1830)，否則十進位。
5. **全域狀態耦合**：`PROD_INFO_ST Prod`(~770 行 cprod.h:368-1136)、`iXXXTask`、`HotTime[2][50][50]`、offset 陣列是跨模組/跨執行緒耦合熱點（接縫盤點 §B TOP-5），翻譯時先切邊界，勿整包搬。

## 接縫（HAL）與 64-bit 跨位元
- 即時硬體（運動/IO/互鎖/ATC）保留 native C++（既有已驗證 wrapper），翻譯後的 C++ **同程序直接呼叫**（無 P/Invoke、無 managed 邊界）。「interface 切割」＝抽象基底 + Sim/Real 子類。
- **64-bit 跨位元地雷**：若 64-bit Handler 與 **32-bit GPIB 橋**走 WM_COPYDATA，`MessageDef.h` 的 `MV`/`VM` 內嵌 `HWND`(行 259-260) 指標寬度會錯位 → 改 `DWORD`+`HandleToLong` 或移出；扁平 `M_V`(InterfaceSYS.h:222) 才安全。
- KeyPro：64-bit 需自家出 64-bit DLL；`HTKeyPro.h` 以值傳 `std::string` 跨界（綁 Borland RTL）→ 改純 C ABI(char*+len)。906 只用 `KEYPRO_GET_LEVEL`(純 int，C 安全)。

## 本機（CUSTOMER_CODE=838 CC_CSAMQ / GPIB recipe）實際啟用硬體 = 64-bit/接線範圍
- 運動：CONTEC SMC 41 軸 + Galil DMC index 4 軸（INDEX_MOTION_CARD=0 覆寫 SMC 馬達表，cinitial.cpp:3464，**待實機確認有無 Galil 卡**）。
- 數位 IO 389 點：MN200 MotionNet（IO_CARD_TYPE=2，mn_set/get_port_bit，**待確認獨立卡 vs 走 SMC**）。
- 溫控 Delta DTK4848(serial, HEATER_CTRL_TYPE=4)、震動 2 板(serial)、條碼(serial)、Tester=GPIB 橋(外部 32-bit, WM_COPYDATA)、sqlite3、KeyPro(自家 dongle, 只用 GET_LEVEL)。
- 本機關閉(出範圍)：Syntek/Aurotek/PCI-1203/1735U/CC-Link/EtherCAT/RFID-MR/Laser/AOI-CCD/ESD/AGV/TrayMap/FTP/Moxa(EJ1N 2020 改 TComm)。
- Tester 模式(54 recipe)：GPIB 50/RS232 3/TCP 1/TTL 0，Handler 端全可攜(green)。

## Log 語料位置（驗證/對拍來源）
- 大部分：`D:\HT9045_Log`（~3.0GB、9259 檔；csv/txt/xls）。
  - EventLog 告警/事件 CSV：`D:\HT9045_Log\EventLog\HT9045_EventLogBackup_*.csv`，欄 `No, UnitName, AlarmCode, Date, Time, Recovery, StopedTime, Duplicate, Message`（Big5）。
    - **格式由 9011UC 寫入端確認**：`SGDToCSV(grid, ",", ";", path)`(common.cpp:2050-2064，呼叫於 cObserver.cpp:2410/cMyDB.cpp:452)，DB-grid 匯出非 tail-append；欄分隔逗號+TAB；空欄字面 `\t`；**cell 內逗號被 escape 成 `;`(common.cpp:2059)→真實 Message 不含逗號**。另有不同格式的 live tail-append `EventLogTxt`(slEventLog, main.cpp:1503, MyStringList::MySaveToFileShareMode)。
    - 真實檔(2023_07，唯一現存)：3409 資料列、UnitName Process=3382/Motion=19/Message=8、含 MES2108、日期 2022-06-06~30。
  - 其他：EventLogTxt(INI 計數)、Test_TCPIP(TCP log)、Alarm.txt、各模組 csv/xls。
- SECS log：`D:\SECS_GEM_LOGS`（依年份；`SECSGEM_TextLog_*.txt`，`[Send]/[Receive]` trace+時戳）。
- **原則（使用者指令）**：log/行為格式不清楚時，務必檢閱 9011UC 寫入端 C++（handlerlog/csystem/database/cMyDB 等），勿臆測。原始碼是 source of truth，log 是驗證的尺。

> 參考：C++ 接縫分析見 `D:\HT9045\docs\migration\RD5軟體_HT9045_906_64bit遷移計畫_20260625_193240.md` §9（HAL、三道牆、硬體 64-bit 稽核）。
