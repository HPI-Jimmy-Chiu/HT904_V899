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

---

## 2026-06-25 — Phase 1 已 commit；開始 Phase 2（config 載入）

- ✅ Phase 1 已自動 commit：`140f1a7`（分支 `feat/csharp-906-migration`）。
- 進行中：**Phase 2 W1 第一塊 ＝ config 載入模組**（Gerneral.ini / Mot_Table.csv / IO_Table.csv）。選這塊先做，因為可**對拍真實 `D:\HT9045\system\` 檔案**驗證（不需機台 log，最腳踏實地）。
- 報表/資料分析 PoC、LotInfo、SECS 組裝待後續（PoC 對拍需使用者提供實機 log）。

### 🔖 下次接續（RESUME — 若中斷/換 session 先看這裡）
1. `cd /d/HT9045 && git log --oneline -5` 看最後 commit；C# 專案在 `HT9011UC_CSharp_V3.33.906.0`，分支 `feat/csharp-906-migration`。
2. 建置/驗證：`cd HT9011UC_CSharp_V3.33.906.0` → `dotnet build HT9045.sln` / `dotnet test HT9045.sln` / `dotnet run --project src/HT9045.App`（離線 smoke，預設 Sim+Stub）。
3. 進行到哪：見本檔最後一則「Phase 2」條目的狀態；config 載入模組在 `src/HT9045.Infrastructure`，對拍測試在 `tests/HT9045.Tests`。
4. 之後：config-load 完成→ LotInfo / SECS（需實機 log）→ Phase 3（HwInterop + 實機接線，待 Galil/MN200 卡確認）。
5. 慣例：每完成一塊就 append DEVLOG + 自動 commit（使用者要求）。

### ✅ Phase 2（config 載入）完成
- 新增 `src/HT9045.Infrastructure/Config/`：`IniFile`、`HandlerHardwareConfig`（14 個硬體鍵 + MotionCardType/IoCardType/IoBaseType enum）、`MotTableRow`/`MotTable`、`IoTableRow`/`IoTable`、`CsvHeaderMap`。
- 測試：ConfigLoadTests（23 單元）+ RealConfigTests（3 整合，對拍**真實 `D:\HT9045\system\` 檔**）+ ConfigColumnReorderTests（4，欄位重排回歸）+ 既有 26 = **共 56 通過 / 0 失敗**。
- 對拍真實檔確認：Mot_Table 45 列全 CardModel=SMC；Gerneral.ini MOTION_CARD_TYPE=1/IO_CARD_TYPE=2/TTL_CARD_TYPE=2/INDEX_MOTION_CARD=0/HEATER_CTRL_TYPE=4；IO_Table 668 列 15 欄、ISABase=0 主導（644/668，24 個=1）。
- **重要修正（correctness）**：spec 發現 C++（database.cpp）是**用 header 名稱**解析 CSV 欄位（AnsiPos，last-match），且 Mot_Table 實體欄序 ≠ enum 序；workflow 初版 loader 是**位置式**（重排會誤對）。已改成 **`CsvHeaderMap` 名稱式解析**（exact、case-insensitive、last-wins，對所有合法 header 行為等價 C++ 且耐重排），並加 4 個重排回歸測試。詳見 KNOWLEDGE「CSV 名稱式解析」。
- build green（net48，0/0）。已 commit。

### 🔖 RESUME 更新
- 進度：Phase 0(待 log)、**Phase 1 ✅、Phase 2 config-load ✅**。下一塊 W1：報表/資料分析 PoC + LotInfo + SECS 組裝（PoC/對拍需使用者提供實機 log → 屬 Phase 0 安全網）。
- 即可動工而不需 log 的：續補 Gerneral.ini 其餘鍵的 typed 對應、或 LotInfo/SECS 的「結構與組裝」骨架（驗證待 log）。
