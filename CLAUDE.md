# CLAUDE.md — HT9045（Claude Code 入口）

本檔是 Claude Code 的專案記憶入口。完整專案說明沿用既有的 `AGENTS.md`，以下用 import 串接，避免兩份漂移。

@AGENTS.md

---

## Claude Code 資產對照（由 Copilot 設定轉接）

| 類型 | Claude Code 位置 | Copilot 原始位置（鏡像，須同步） |
|------|------------------|-----------------------------------|
| Skills | `.claude/skills/<name>/SKILL.md` | `.agents/skills/<name>/SKILL.md` |
| Agents（子代理） | `.claude/agents/<name>.md` | `.github/agents/<name>.agent.md` |
| Commands（斜線指令） | `.claude/commands/<name>.md` | `.github/prompts/<name>.prompt.md` |
| Hooks（寫入邊界） | `.claude/settings.json` → `scripts/ops/check-write-boundary.ps1` | `.github/hooks/pretool-write-boundary.json` |
| 寫入邊界政策 | `.github/ops/write-boundary-policy.json`（兩邊共用同一份） | 同左 |

> 維護提醒：修改任一 Skill / Agent / Command 後，若仍同時使用 Copilot，請同步更新對應的鏡像檔。

### 可用子代理（Task 工具呼叫）
- `ht9045-v899` — V3.33.899.0 版本除錯與修正，鎖定 `HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422`。**量產機台使用中的出貨版本。**
- `ht9045-v906` — V906 C++ 移植版（**實驗機**）開發，鎖定 `HT9011UC_Cpp_V3.33.906.0`。C++17 / UTF-8 / CMake，與 V899 的 BCB6 / Big5 / pre-C++11 規則完全相反。負責翻譯波次、CMake/ctest、MinGW+MSVC 雙 oracle、MFC UI、WebBridge 與瀏覽器 HMI。
- `case-coordinator` — 客戶異常案件協調入口，分派 intake/analysis/closure。
- `weekly-report` — 週報與客戶異常 case 管理（Hub 模式，操作 Weekly_AI 工作區 `d:\Work-jimmychiu\document\WeeklyReport\Weekly_AI` 的 Python 工具）。破壞性動作（建下週週報、建 case、重產 Excel）執行前先確認。

### 可用斜線指令
- HT9045：`/ht9045-debug`、`/ht9045-v899-build`、`/ht9045-skill-factory`
- V906 純翻譯戰役：`/pt-wave`（執行一個完整波次；搭 `/loop /pt-wave` 自動連續推進，
  政策見 `pt-wave-loop` skill。**範圍只到非表單翻完為止**，表單 facade 策略要使用者先定案）
- 治理：`/ops-daily-worklog`、`/ops-weekly-review`、`/ops-skill-maintenance`、`/ops-new-project-bootstrap`
- 週報/案件（Hub，操作 Weekly_AI）：`/update-weekly`、`/weekly-status`、`/weekly-case-intake`、`/weekly-case-integrity`、`/weekly-next-week`、`/weekly-help`

> Weekly_AI 工作區為 Hub 模式接入：agent/指令在 HT9045，實際 Python 工具與 `weekly_data.json`、`Customer/` 資料留在 Weekly_AI。修改 weekly-report agent 或指令時，Weekly_AI 的 `.github/` 原始定義為鏡像，視需要同步。

---

## 路徑範圍指令（取代 Copilot `applyTo`）

Claude Code 無原生路徑範圍指令機制，故將原 `.github/instructions/*.instructions.md` 的關鍵守則統整於此。詳細表格仍可參考對應 instruction 檔。

> **⚠️ 兩棵樹的規則是相反的，先確認你在哪一棵。**
> `HT9011UC_Code_V3.33.899.0_...`（**量產機**，BCB6 / Big5 / pre-C++11）
> `HT9011UC_Cpp_V3.33.906.0`（**實驗機**，C++17 / UTF-8 / CMake）
> 另注意 `HT9011UC_Code_V3.33.906.0_20260618` 是**另一棵 BCB6 樹**，同樣叫 906 但唯讀；差別在 `_Code_` 與 `_Cpp_`。

### 編輯 V899 C/C++（`HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422/**/*.{cpp,h,hpp}`）

**寫入邊界**
- 只允許修改上述 V899 版本目錄；其他 `HT9011UC_Code_*`、`HT9046LS_Code_*` 一律唯讀。
- `system/`、`config/`、`CFG/`、`IniData/`、`setup.inf`、`CurrentSetupData.txt` 為共用參數，預設只讀，使用者明確要求才改。
- 不得修改 `AGENTS.md` 禁改清單中的驅動與定義檔（如 `CSmc.h`、`sqlite3.h` 等）。

**BCB6 / VCL / 編碼**
- 以 pre-C++11 為準：不用 `auto`、`nullptr`、lambda、range-for；不引入新 STL/第三方依賴。
- 字串優先用 VCL `AnsiString`，非必要不引入 `std::string`。
- 原始碼為 Big5：最小化變更範圍，避免整檔重寫成 UTF-8，審查不得出現亂碼（U+FFFD）。
- `.dfm` 為 Delphi 格式，不手動重排，除非使用者明確要求處理表單。

**AI 修改註解**（格式：`//AI(AgentName) YYYYMMDD: 描述`）
- 日期固定 `YYYYMMDD`；V899 新變更 AgentName 預設 `ht9045-v899`。
- 描述寫「動作 + 原因」，不寫 fix/update 等空泛字樣。
- 新增行：在前一行放獨立註解；修改單行：行尾加註解；多行區塊：只在區塊第一行前加一則。
- 不重寫既有歷史 AI 註解格式。

**完成後自我審查**
- 確認落在 V899 目錄內、未碰禁改檔、最小差異、無編碼亂碼。
- 回覆需說明：影響函式/狀態機、風險、驗證方式。
- 修改共用標頭、核心狀態機、跨模組函式、全域變數時，優先對 V899 根目錄 `HT9045.bpr` 做 BCB6 build 檢查；環境不足無法編譯時要明說，不可假設成功。

### 編輯 V906 C++ 移植版（`HT9011UC_Cpp_V3.33.906.0/**/*.{cpp,h,hpp}`）

> 本節規則**與上面 V899 那節相反**，不要混用。專屬代理：`ht9045-v906`。

**寫入邊界**
- 只允許修改 `HT9011UC_Cpp_V3.33.906.0/`。**V899 是量產出貨版，一律唯讀**，要改請交回 `ht9045-v899`。
- `D:\HT9045\EXE\` 是 BCB6 量產建置的輸出目錄，V906 的 exe 永不複製或覆寫進去。
- `common.cpp:89` 的 `asGeneralPath` 目前指向**量產機共用的** `system\Gerneral.ini`，且 `LoadMachineConfig()` 會回寫它 → 跑 `wb_serve` / `wb_publish` 一律加 `--dry`。

**語言 / 編碼 / 工具鏈**
- **C++17**（`CMAKE_CXX_STANDARD 17`）；新寫的基礎建設開 `-Wall -Wextra`。
- 原始碼 **UTF-8**。EOL 是**混合**的，逐檔保持原樣；`build.bat` 必須純 CRLF 無 BOM（cmd 掃 `goto` 承重）。
- 主 oracle 是 **MinGW g++**（唯一重現 BCB6 x87 算術，不可替換）；MSVC 是次 oracle，且**只有 MSVC 能編 MFC UI**。
- 建置一律走 `build.bat`（quick / gate / test / ui / run / msvc / clean / prune），不要自己另組 cmake 指令。

**AI 修改註解**：`//AI(W906-<工作代號>) YYYYMMDD: 描述`（例 `AI(W906-PT-W1)`、`AI(W906-GateA-0)`、`AI(W906-WebBridge-Tcp)`）。

**完成後自我審查**
- 翻譯以**忠實**優先於「寫得更好」；golden 不合理處照翻並在註解說明，改行為須使用者決定。
- 驗收比對 **ctest 失敗清單**，不看數字或百分比。既有失敗（非回歸）：`config_db`、`IniFiles`、`ini_helpers`、`config_loaders`、`dfm2rc_idempotent`、`GA1_ReadGeneralIni`。
- 「build 綠」不等於「接上了」；交付數字只在全新 build dir 量，收工最後一個動作是 build 不是 commit。
- 引用完成度百分比必附分母與單位。

### 安全關鍵變更（運動控制 / IO / 互鎖 / 模式切換 / 警報 / 執行期設定）
- 修改前先描述風險與影響範圍，修改後提供回歸與驗證建議。
- 觸及 `system/`、`config/`、`CFG/` 的 `.ini/.csv/.dat` 等執行期設定屬高風險，預設只讀並需備份確認（見寫入邊界 hook）。

### 報告 / 文件輸出
- 報告檔名須以 `RD5軟體` 前綴開頭，並含 `YYYYMMDD_HHMMSS`。
- `.svn` 資料夾一律不搜尋、不修改。
