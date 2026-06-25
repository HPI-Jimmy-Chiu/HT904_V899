# HT9045 V906 → C# 累積知識（KNOWLEDGE）

> 開發過程的決策、接縫/marshalling 知識、與 gotcha。逐步累積，供備查。逐步紀錄見 `DEVLOG.md`。

## 架構（ports-and-adapters / 六角）
- `Core`（編排/狀態機）只依賴抽象專案（HardwareAbstraction、Presentation.Abstraction），**永不**引用 WinForms / P/Invoke / SQLite。
- 每個硬體 port 與每個 UI 介面都有兩個可互換轉接器：Sim/Stub（開發）與 Real/WinForms（之後）。
- 切換只在 `HT9045.App` 組合根一處，依旗標選擇（取代 C++ `cinitial.cpp:3454-3565` 的 dispatch ladder）。
- 依賴方向（無環）：App→全部；Core→{HardwareAbstraction, Presentation.Abstraction}；Sim/Native→HardwareAbstraction；Presentation.Abstraction 不依賴 Core（見下方 gotcha）。

## 命名對照（鏡射 BCB6，供反查）
- `HTMotor`(Motor/HTMotor.h) → `IMotorPort`：InitMotor / MoveToPos / ReadPos / HomeObject / HomeFlag / Stop / MotionDone / GetAlarm / SetSpeed(uint,bool=false) / **ServerOnOff**(保留 BCB6 拼法，非 ServoOnOff) / SetSoftLimit / JogP / JogN / ScanMotorStatus。
- `TLaneIO`(MyLaneIo.h) → `IIoPort`：IOBitOn / IOBitOff / IOByteOut / IOOutBitStatus / IOInputBit / IOInputByte（int 回傳，<0=fail，沿用 C++ `ret` 慣例）。
- `KEYPRO_GET_LEVEL`(Public/HTKeyPro.h, HS_Function.cpp:4933) → `IKeyProPort.GetLevel(uint)`。
- `ITesterInterface`（main.h:1213/1277/1337/1392/1400）：Find / RunTestProgram / SendMSG_CMD / CloseGpibProgram / GetTesterResult。
- `PROD_INFO_ST`(cprod.h) → `ProdInfo`（目前僅代表性切片）。

## 建置 / 工具
- 平台 net48；用 dotnet SDK 10（無 Visual Studio）即可 headless 編譯，關鍵是 `Directory.Build.props` 內的 `Microsoft.NETFramework.ReferenceAssemblies 1.0.3`（PrivateAssets=all）。
- 常用指令（cwd = 專案根）：
  - `dotnet build HT9045.sln -c Debug`
  - `dotnet run --project src/HT9045.App`（離線 smoke，預設 Sim+Stub）
  - `dotnet test HT9045.sln`
- DI 旗標：環境變數 / args `HT9045_HW=Sim|Real`、`HT9045_UI=Stub|WinForms`（預設 Sim+Stub）。
- LangVersion 7.3（保守，pre-modern；避免 net48 record/init-only polyfill）。

## Gotcha（踩過/要注意）
1. **專案循環參考**：.NET 不允許 ProjectReference 循環。Core 與 Presentation.Abstraction 不可互參考 → 單向 Core→Presentation.Abstraction，`PresenterBase<TView>` 泛型化；IView 只用基本型別避免洩漏 Core 型別。
2. **Big5 原始碼檔案編碼**：含繁中字面值的 C# 檔（如 Big5RoundTripTests.cs）必須存成 **UTF-8**（編譯器讀檔用 UTF-8），但**執行期**對外 Big5 資料一律經 `Big5Codec`（`Encoding.GetEncoding(950)`）轉 UTF-16，含 U+FFFD 亂碼檢查。勿混淆「原始碼檔編碼」與「執行期資料編碼」。
3. **Native 佔位**：Hardware.Native 各方法 throw NotImplementedException，是 Phase 3 接 HwInterop 的佔位；離線路徑（Sim）不會碰到。
4. **ProdInfo 切片**：真實 PROD_INFO_ST ~770 行（cprod.h:368-1136），勿一次搬整包（接縫盤點 §B：Prod native 權威 + C# 唯讀快取）。

## 接縫 / Marshalling（Phase 3 接實體控制時用）
- HwInterop 為薄 C-ABI 層，接在既有 C++ wrapper（非原始 vendor DLL）。
- 結構一律 `[StructLayout(LayoutKind.Sequential)]` + 對齊單元測試（如 MN200 `SPEED_PAR`）；`HANDLEDMC`→`IntPtr`；陣列 `Marshal.AllocHGlobal` + 明確釋放。
- **KeyPro/SECS 用 char*+length，禁 std::string 跨界**；KeyPro 匯出名可能為 `_KEYPRO_GET_LEVEL@4`（先 dumpbin 確認再 GetProcAddress）。
- **跨位元地雷**：GPIB 橋為 32-bit 外部程序、走 WM_COPYDATA；`MessageDef.h` 的 `MV`/`VM` 內嵌 `HWND`(行 259-260) 指標寬度會錯位 → 改 `DWORD`+`HandleToLong` 或移出；扁平 `M_V`(InterfaceSYS.h:222) 才安全。

## 本機（CC_CSAMQ / GPIB recipe）實際啟用硬體 = 64-bit/接線範圍
- 運動：CONTEC SMC 41 軸 + Galil DMC index 4 軸（INDEX_MOTION_CARD=0 覆寫，待實機確認有無 Galil 卡）。
- 數位 IO 389 點：MN200 MotionNet（待確認獨立卡 vs 走 SMC）。
- 溫控 Delta DTK4848（serial）、震動 2 板、條碼、Tester=GPIB 橋、sqlite3、KeyPro（自家 dongle，只用 GET_LEVEL）。
- 本機關閉（出範圍）：Syntek/Aurotek/CC-Link/EtherCAT/RFID/AOI/AGV/FTP/Moxa 等。
- Tester 模式統計（54 recipe）：GPIB 50 / RS232 3 / TCP 1 / TTL 0，Handler 端全 green。

> 參考：對外計畫 `D:\HT9045\docs\migration\RD5軟體_HT9045_906_CSharp遷移計畫_20260625_201101.md`；C++ 接縫分析見 `RD5軟體_HT9045_906_64bit遷移計畫_20260625_193240.md` §9。
