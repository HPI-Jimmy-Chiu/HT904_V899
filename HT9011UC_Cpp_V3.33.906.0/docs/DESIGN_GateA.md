# DESIGN_GateA — GA-0 四項設計決策（Gate A 首次點亮）

> AI(W906-GateA-0) 20260804: 本檔是 `GATE_A_FIRST_LIGHT_PLAN.md` §4-GA0 第 5 項的產出。
> 每項決策含：裁決、理由、落地位置、blast radius。狀態標記：✅ 已裁決／🔄 等 recon。

---

## D-A0-0（前置更正）DA2 `_WIN32_WINNT` 已落地，GA-0 不再做

根 `CMakeLists.txt:43` 已有 `add_compile_definitions(_WIN32_WINNT=0x0601 WINVER=0x0601)`——
2026-08-01 `W7-L1-Wave0` 落地，含警告 diff 驗證（277→284，+7 全是同批 LastSet 欄位的
-Wmissing-field-initializers）。W7 plan §2-DA2「落地仍未完成」與 Gate A 計畫初版的轉抄皆過時，
兩處已（或將）修正。**教訓重申：跨文件轉抄的「未完成」宣稱，動筆當下要重新 grep。**

## D-A0-1 ✅ WinMain 語意搬運的偏離清單（app 骨架）

裁決：`ui/HT9045App.cpp` 承載 golden `HT9045.cpp:127-301` 語意，偏離五項全部記在該檔
banner（[DEV-1]~[DEV-5]）：OSK kiosk 區塊跳過（部署波恢復）、exe 路徑檢查忠實但加
`--devpath` 開發旁路、118 個 CreateForm（見 D-A0-2 的數字更正）延到 GA-3 FormRegistry、
`Application->Run()`→`DoModal()`、`LogSoftwareOnTime` 等 main.cpp 住戶延到 GA-3。
另加 `--smoke <ms>`（自動關窗 + `SetErrorMode` 抑制 WER modal）——批次驗證不得卡彈窗
是本專案硬規則（ctest INTERFACE guard 同源）。
**忠實保留**：`CreateMutex("MyMutexName")`+`GetLastError` 單實例（同名互斥 → 與運行中的
BCB6 HT9045.exe 天然互斥，此為 golden 語意的自然延伸，蓄意保留）；BootLog 檢查點順序。

## D-A0-2 ✅ FormRegistry（CreateForm 鏈的 Gate A 形狀）

**先更正一個數字**：golden 的 CreateForm 是 **118 次呼叫 / 115 個唯一指標**，不是本檔初版與
Gate A 計畫寫的「120」——:284/:285 是含 "CreateForm" 字樣的 log 呼叫誤入計數；且 golden 自己
有 3 組 copy-paste 重複建構（`fBinAOISel`/`fObserveMagazine`/`frmFileTransfer`）。主迴圈已用
獨立 regex 對 golden 重數證實。完整 118 列順序×ported 狀態×副作用表見
`docs/RECON_GateA_FormRegistry.md`。

裁決：
1. 只有 fMain 拿真 HWND（W7 plan D3：`CDialog` 持有 `TfMainImpl : public TfMain`，`fMain=&impl`）。
2. 其餘 114 個唯一指標按 golden 順序處置；**3 組重複建構坍縮為一次**（除非未來把「golden
   崩潰對等」列為目標——揭露性偏離，記於 FormRegistry banner）。
3. **順序忠實度的範圍條款**：現況 18 個 facade 是 static-init 期建構（連結順序決定），
   Gate A **接受現狀 + 文件揭露**，不做 eager→lazy 全面改造；FormRegistry 只對「新建構」
   的物件負責 golden 相對順序。全面 lazy 化留給 GA-3 brief 重新評估（若屆時發現真實跨
   ctor 依賴斷裂，再升級）。
4. **SHIM-NULL 兩地雷**（`HGem`＝`uHGemEquipment.cpp:3521` 恆 NULL、`DataModule1`＝
   `database.cpp:103` ctor 在 `#if 0`）：FormRegistry 必須在對應槽位（#70/#83）給出裁決——
   `HGem` 在 GA-3 槽位真建構（THGem 已翻，ctor 231 行+FormCreate SV 註冊已翻）；
   `DataModule1` 掛 deref 斷言（BDE 波未到）。
5. 調和債順手清：`fContactCT`/`fCounterClear`/`fShowBinSelect` 各有兩份 TU-local stand-in
   （csystem.cpp `W7C1_*` × auto9045.cpp `W5FA_*`），FormRegistry 落地時是天然整併點。

**落地位置**：GA-3 的 `ui/FormRegistry.{h,cpp}`（新檔）。

## D-A0-3 ✅ 主 pump：UI-thread timer 等價替代 TRunControl

裁決：Gate A 用 fMain 視窗的高頻 `SetTimer`（1ms 級）+ 重入 guard 直接呼叫
`if(InitialOK) MainProc();`，**不開 worker thread**。

等價性論證（golden `uruncontrol.cpp` 全檔 86 行，親讀）：
- golden `TRunControl::Execute` 的工作本體是 `Synchronize(ThreadProcess)`——Synchronize 把
  `ThreadProcess`（即 `if(InitialOK) MainProc();`）**排到 UI thread 執行並等待完成**。
  也就是說 golden 的 MainProc 從來不在 worker thread 跑；worker thread 只貢獻「節奏」
  （`SystemStart==false || iHome==1 || fContact->fShow==true` 時每 tick sleep 1ms，
  否則每 3 tick 才 sleep 1ms，`timeBeginPeriod(1)` 拉高 timer 解析度）。
- 因此「UI thread timer 驅動 MainProc」不是近似，是 golden 實際的執行緒歸屬；差異只在
  節奏來源（timer 間隔 vs thread 迴圈密度）。WM_TIMER 最低 ~10-15ms 解析度低於 golden
  有效節奏——**Gate A 接受**（首燈只要 pump 活著；節奏保真留給 Gate B 換真 thread 時一併驗）。
- `StartMainThread`/`EndMainThread` 語意（bEnd/Resume/Terminate）映射為 timer 的啟停。
**風險**：若未來發現 golden 有依賴 sub-10ms 節奏的行為（如 sensor 去彈跳窗口），Gate B 換真
thread 時要回歸驗證；已記入 Gate A 計畫 §7。

## D-A0-4 ✅ link graph：不加 library 邊，局部 RESCAN 升級（9 個 targets）

問題：`cpublic.cpp` 3 處（`TMyStrQueue100::SafeData`/`TMyTimerQueue100::SafeData`/
`UDPErrorLog`）與 `TempChangeLog` 需要 `common.cpp`（住 `ht9045_core`）的
`MyForceDirectories`/`WriteDataToFile`，但 `ht9045_globals` 不 link `ht9045_core`
（ROADMAP DEFERRED common.cpp 列）。

**盤點（2026-08-04 親量，tests/CMakeLists.txt）**：82 個 targets link `ht9045_globals`，
其中 **73 個已同時 link `ht9045_core`**；只剩 **9 個**沒有：
`test_FTPClient_EventHandlers`、`test_MyCCLink`、`test_MyNUEC1`、`test_MyProductionRecord`、
`test_cMyNUDN1`、`test_cpublic_foundation`、`test_globals`、`test_myplc_modbus`、
`test_w0tail_headers`。

**裁決：方案 B（局部升級）**——GA-1 B3 ungate cpublic 時，把上列 9 個 targets 的 link 行
比照 repo 既有 66 處 `$<LINK_GROUP:RESCAN,...>` 慣例補上 `ht9045_core`（+其遞移依賴）。
**不採**方案 A（`target_link_libraries(ht9045_globals PUBLIC ht9045_core)`）：
(a) library 層加邊會改動全部 82 個 consumers 的 link 序，與既有 66 處手排 RESCAN 群組
交互作用未知，驗證面失控；(b) core 物件層本就反向引用 globals 符號（common.cpp 讀
IniConfig 等），單向 CMake 邊解不掉符號層循環，最終 link 仍靠 RESCAN——那就照慣例只在
final target 排群組；(c) blast radius 已量化為 9，方案 B 的變更面小一個數量級。

---

## 驗證閘（GA-0 收尾時全部要過）

1. `HT9045_UI` 未指定（預設 OFF）：MinGW fresh dir 全套 ctest 不退步（基準 114/118+trace）。
2. `build_msvc.bat`（UI=OFF）：續綠，證明 option 定義後舊指令列不變行為。
3. `build_msvc_ui`（Ninja、Release、`-DHT9045_UI=ON`）：連結出 `HT9045.exe`（skeleton）。
4. `HT9045.exe --devpath --smoke 800`：exit code 0、BootLog 出現「WinMain Enter →
   Application Initialize Done → normal exit」序列、無殘留行程。
5. `rc.exe` 過 `ui/app.rc`（Ninja 內建）；mojibake/EOL 掃描 0 異常。
