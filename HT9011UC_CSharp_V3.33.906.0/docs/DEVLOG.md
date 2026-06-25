# HT9045 V906 → C# 開發紀錄（DEVLOG）

> 開發過程逐步紀錄，供團隊與未來備查。對外正式報告另見 `D:\HT9045\docs\migration\RD5軟體_HT9045_906_CSharp遷移計畫_*.md`。
> 累積知識/決策/gotcha 見同資料夾 `KNOWLEDGE.md`。

---

## 2026-06-25 — Phase 0 決策 + Phase 1 離線骨架完成

### 背景與方向
- 來源：`HT9011UC_Code_V3.33.906.0_20260618`（BCB6/VCL C++，黃金參考、唯讀）。
- 目標：本專案 `HT9011UC_CSharp_V3.33.906.0`，**C#（.NET Framework 4.8 + WinForms）**。
- 策略：Strangler Fig + 凍結 interop 接縫；硬體與 UI 都用 interface 切割，先 Sim/Stub 開發、之後接實體控制與 WinForms。
- 動機：未來 OS 升級 / 64-bit / 脫離 BCB6 生態、長期可維護。

### Phase 0 決策（已定）
- 語言/平台：C#，**.NET Framework 4.8**（最低互通風險、Big5 開箱即用、貼近 899 計畫；Core/抽象框架無關，日後可多目標到現代 .NET 仍可逆）。
- 分支：`feat/csharp-906-migration`（不直接動 main）。
- 寫入邊界：`.github/ops/write-boundary-policy.json` 已加 `HT9011UC_CSharp_V3.33.906.0/` 為可寫、`HT9011UC_Code_V3.33.906.0_20260618/` 為唯讀。
- 開發順序：先 Phase 1 離線骨架；實機 log（golden baseline）由使用者並行準備。

### Phase 1 — 離線骨架（完成，已驗證綠燈）
**目標**：方案能編譯，且離線無頭「一個 lot cycle」端到端跑通（零硬體、零真 WinForms）。✅ 達成。

建立 9 個專案（ports-and-adapters）：

| 專案 | 角色 |
|------|------|
| HT9045.HardwareAbstraction | 硬體 ports：IMotorPort / IIoPort / IKeyProPort / ITesterInterface / ITempControlPort |
| HT9045.Presentation.Abstraction | UI ports：IView / IMainView / PresenterBase`<TView>` |
| HT9045.Core | 編排：ProdInfo、事件、IHandlerOrchestrator / HandlerOrchestrator（只依賴抽象）|
| HT9045.Hardware.Sim | Sim 轉接器（**開發用**）：SimMotorPort/SimIoPort/SimKeyProPort(回 1)/SimTesterInterface/SimTempControlPort |
| HT9045.Hardware.Native | Real 轉接器**佔位**：各方法 throw NotImplementedException（Phase 3 接 HwInterop）|
| HT9045.Views.Stub | StubMainView（無頭，記錄呼叫）|
| HT9045.Infrastructure | Big5Codec(cp950)、ConfigLoader 骨架、IHandlerDb/SqliteHandlerDb(Microsoft.Data.Sqlite)|
| HT9045.App | Console 組合根：依旗標選 Sim/Real + Stub/WinForms（預設 Sim+Stub），跑離線 smoke |
| HT9045.Tests | xUnit：Big5 round-trip、離線 smoke、架構依賴守則 |

**驗證結果（獨立重跑確認）**：
- `dotnet build HT9045.sln -c Debug` → **建置成功，0 警告 0 錯誤**（9 個組件，net48）。
- `dotnet run --project src/HT9045.App` → 完整 Sim+Stub trace（KeyPro gate=3 → Home → MoveToPos → IOInputBit → tester Find/RunTestProgram/GetTesterResult → bin=PASS）→ **`OFFLINE SMOKE OK`，exit 0**。
- `dotnet test HT9045.sln` → **通過 26，失敗 0，略過 0**。
- 架構守則測試通過：Core 不引用 Sim/Native/WinForms/Infrastructure。

**本階段偏離計畫處（已處理）**：
- 原計畫 Core ↔ Presentation.Abstraction 互相參考 → .NET 不允許循環參考。改為單向 `Core → Presentation.Abstraction`，`PresenterBase<TView> where TView:IView` 泛型化；IMainView 只用基本型別，不洩漏 Core 型別。
- `ProdInfo` 僅取代表性切片（真實 PROD_INFO_ST 約 770 行 cprod.h:368-1136）；LotID/DeviceName 不在 PROD_INFO_ST（在別的結構），未臆造。
- LangVersion 用 7.3（保守，避免 net48 record/init-only 問題）。

**工具環境**：dotnet SDK 10.0.201（無 Visual Studio）；net48 靠 `Microsoft.NETFramework.ReferenceAssemblies 1.0.3` 在無 VS 機器上 headless 編譯成功。NuGet 套件已在全域快取，restore 離線可用。

### 下一步
- Phase 0 golden baseline：待使用者提供實機 log（Task_ListWithTime / EventLog / State Record / SECS / GPIB）。
- Phase 2：搬 W1 非即時（報表/資料分析 PoC → config 載入 → LotInfo → SECS 組裝），與 C++ 並行對拍。
- 待實機確認：Galil index 卡、MN200 IO 卡是否實體存在（影響 Phase 3 範圍）。
