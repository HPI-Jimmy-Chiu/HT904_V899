# DFM→WEB 戰役計畫（FW CAMPAIGN）

> 建立：20260817。使用者裁決：**「我的目標是 dfm 轉成 web」**——這句話把 PT 戰役停在表單
> 邊界的「facade 策略未決項」正式定案：**表單的渲染目標是 web，不是任何原生 UI**。
> 執行方式：`/fw-wave` 一次一個波次，搭 `/loop /fw-wave` 夜間自動連續推進；
> 額度中斷後照 §6 的協議自動接續。
>
> 姊妹文件：`PT_CAMPAIGN_PLAN.md`（非表單翻譯，已完成）、`W7_UI_ARCHITECTURE_PLAN.md`
> （MFC 路線，已隨 20260814「web REPLACES the DFM forms」定案退役為歷史）、
> `D:\HT9045\web\docs\ARCHITECTURE.md`（tag/command 契約與 wire protocol）。

---

## §0 定案鏈（每一條都有日期，不要再重新辯論）

| 日期 | 裁決 |
|---|---|
| 20260812 | 「UI 用 web 開發，底層邏輯和控制是 C++」 |
| 20260814 | 移植是 906-only；**web 取代 DFM 表單**；Route B 取消；MFC/MSVC 自產品移除（`7b86cfd`） |
| 20260817 | **「目標是 dfm 轉成 web」＝表單 facade 策略定案**（本計畫）；夜間 loop 執行，額度解除後續跑 |

**由此固定的架構**：`.dfm` 是版面真相來源（Big5，不手改）→ `tools/dfm2rc` 解析（已完成，
133 表單全部有 IR 與 layout 產物）→ 新 emitter 產 web 版面資料 → 瀏覽器泛用渲染。
表單的**業務邏輯**照 PT 慣例忠實翻譯成 C++；widget 讀寫改為 form-core 的 UI-state 欄位；
狀態經 WebBridgeTags（liveness predicate 必備）→ TagSnapshot → wb_gateway → 瀏覽器。
**指令方向（write path）是安全關鍵，不在本戰役夜間範圍**（§7）。

## §1 最終目標（可量測）

1. **畫面活資料**：web 頁面綁定的機台 tag 由「發布且非 null」的比例衡量，
   從 0/296（20260817 量）推進到「來源已翻譯者全部接通」。
2. **表單邏輯**：118 個表單單元（含 `Command.cpp` 的 TfMain 邏輯）之 golden code 行
   翻譯完成度，census 量（20260807 基準：表單 10,366/261,862 = 4.0%）。
3. **表單畫面**：每個完成的表單有一份 dfm2web 產出的版面資料 + web 渲染，
   唯讀顯示正確（e2e 探針 + 人工 F5 抽驗）。
4. 全程 **Debug 與 Release 都 build exit 0**，ctest 失敗集合 ⊆ §5 常駐清單。

**現況一律重量，不引用記憶數字**（census + tag probe，見 §9）。

## §2 開工前的現況（20260817 量測，僅供定錨；每波開工重量）

- 非表單翻譯：**完成**（PT-W10，真正未翻譯 0 行）。
- 表單：118 單元 261,862 行，已翻 10,366 行（4.0%）；`Command.cpp` 9,445 行全是
  `TfMain::` 方法（census 誤判非表單，實為表單工作）。
- dfm2rc 資產（**現成，不用重做**）：`tools/dfm2rc/ir_out/`（全表單 IR）、
  `layout_out/*_layout.gen.cpp|h`（幾何）、`layout_out/*_events.gen.json`（事件表）、
  `rc_out/main_uimap.gen.*`（fMain 91 dialogs / 758 controls 的 leaf-name↔id 對照）。
  ARCHITECTURE.md 量過：133 表單、22,586 CONTROL、3,637 dialog templates。
- web/：296 個綁定 tag（234 data-tag + 62 data-tone-tag）、發布 61、
  畫面活機台資料 0/296，缺口中 **84 個 SOURCE-EXISTS**（來源已翻好只差接線）。
  **web/ 目前 git 零追蹤**——FW-0 要收。
- 常駐 ctest 失敗六項：`config_db`／`IniFiles`／`ini_helpers`／`config_loaders`／
  `dfm2rc_idempotent`／`GA1_ReadGeneralIni`（20260817 全新 dir 只剩 4，
  IniFiles/ini_helpers 狀態相關，驗收線仍用「⊆ 六項」）。
- **未收斂安全項**：跑 V906 測試套件會重寫量產共用的 `D:\HT9045\system\BinCount.txt`
  （內容相同但 mtime 變）。夜間 loop 會反覆 ctest → **FW-0 必修**。

## §3 每個表單的三件套（拆得開，可以分波交付）

一個表單「完成」= (a)+(b)+(c) 都綠。但三件可以由不同波次交付，各自有 gate：

- **(a) 邏輯翻譯**：`TfXxx` 的 `.cpp` 業務邏輯照 PT 慣例忠實翻譯（鏡射檔、GATE REGISTER、
  Big5→UTF-8、EOL 保持）。VCL widget 存取（`edX->Text`、`lbY->Caption`、`->Enabled`…）
  一律改為 form-core 的 **UI-state 欄位**（struct，POD，命名 = dfm leaf name），
  事件 handler 保留函式本體、掛在可呼叫的自由入口。**ctor 禁止呼叫任何會碰
  PT_CAMPAIGN_PLAN §8 那 18 個 NULL 全域的初始化**（陷阱 #4，表單正是高危區）。
- **(b) tag 匯出**：UI-state → `WebBridgeTags.cpp` 註冊，**每個來源自己的 liveness
  predicate 與 value getter 分開**（W B-1 慣例）；null=未載入、""=刻意空白、0=真的 0。
  process/machine 分母規則照 `test_wb_tags.cpp`，不美化分母。
- **(c) web 渲染**：dfm2web 產出的版面 JSON + 泛用 formview 渲染器（FW-2 建），
  綁定 key = **dfm leaf control name**（ARCHITECTURE.md 定案的 join key，
  全 133 表單量過零 intra-form 重名）。

## §4 波次佇列

### FW-0 基礎建設（第一波，一次做完）
1. **web/ 入版控**：`web/.gitignore`（`__pycache__/`）＋ `git add` 逐檔點名
   （repo 根與 web/ 屬本工作區可改範圍；兩 session 共用 repo，禁寬 glob）。
2. **釘出並隔離寫 `system\BinCount.txt` 的測試**：`tests/` 有 6 檔提到 BinCount，
   逐一單跑 + 前後 mtime 對帳釘兇手；修法優先序 = 測試改用 scratch 路徑 >
   測試內 stub 該寫入 > 記錄後跳過該測試。**夜間反覆 ctest 前的安全前置。**
3. **e2e 探針收編**：20260813 的 `ws_probe.py`／`wire_probe.py`／`e2e_verify.ps1`
   在 session scratchpad（可能已消失）。照 DEVLOG 記載的功能重寫進
   `tools/webprobe/`，加 `system/` 550 檔 MD5 前後對帳模式。這是之後每一波的 gate 工具。

### FW-1 tag 接線波（估 2–4 波，機械性）
- 重量 SOURCE-EXISTS 缺口（不是引用 84 這個數字）：對 `web/js/model/tagmap.js` 的
  296 個綁定逐一判「來源全域在 port 是否已翻譯且可量測地載入」。
- 每波接 ~20–30 個 tag 進 `WebBridgeTags.cpp`（liveness + getter 分離），
  e2e 探針驗「發布數上升、null/值語意正確」。
- 需要 `LoadMachineConfig()` 的 tag：只在 `--with-config` 模式發布，
  pump 模式維持 null（20260813 安全決定不推翻）。

### FW-2 dfm2web 產生器（設計波，主迴圈自做，不委派）
- `tools/dfm2rc/emit_web.py`：吃**既有 IR**（ir_out），產每表單
  `web/forms/<form>.layout.json`：widget 樹（type/geometry/caption/color/font 摘要）
  ＋綁定 key（leaf name）＋事件表（events.gen.json 併入，唯讀階段僅存檔備查）。
  Big5 caption 走 dfm2rc 既有 cp950 解碼路徑，產物 UTF-8。
- `web/js/ui/formview.js`：泛用唯讀渲染器（label/edit-as-text/panel/grid/LED/shape），
  吃 layout JSON + tag patch。未知 widget type 畫佔位框並列清單，不靜默丟。
- **idempotent 測試**比照 `dfm2rc_idempotent` 進 ctest。
- pilot：挑最小表單（如 `cTestCategory`，layout 1.5KB）走通全鏈再擴。

### FW-3+ 表單波（每波一個表單；大表單切塊，單波 golden 行數 ≤15k）
優先序原則 = 畫面使用頻率 × 唯讀可完成度（每波開工時重評，此表非死表）：

| 批 | 標的 | 理由 |
|---|---|---|
| 1 | `Command.cpp`（TfMain 邏輯，9,445 行，切 2–3 波） | 主畫面 296 tag 的最大來源 |
| 2 | `cObserver`、`uYieldMonitoring`、`BinDisplay` 族 | 純顯示、零互鎖、客訴常用 |
| 3 | `cTemperFrom`／`uTemp_Set`／`DynamicTemp` 顯示側 | 溫控可視化，tag 已有骨架 |
| 4 | `uLotInfo` 顯示側 | 批次資訊 |
| 5 | 設定「檢視」：`cConfiguration`／`cSetUp`／`cSpeed`／`cStartCondition`／`HandlerSys` | 唯讀現值展示（寫入留 FW-W） |
| 佇列 | `uteach`、`uMotorTest`、`iosetview` 操作側、`login`/`Password`、對話框族 | **需 write path，夜間不做** |

### FW-W 指令通道（write path）——**20260819 使用者已裁決核可，設計見 docs/WEBBRIDGE_WRITEPATH_DESIGN.md**
（裁決三邊界：登入/權限比照 BCB6 fSecurity 體系；單一瀏覽器操作權；golden 沒有的二次確認不加。
原「不在夜間範圍」條款由該設計文件的波次切分 FW-W1~W5 取代；動真機類指令仍屬佇列段。）
#### [歷史條款]
瀏覽器→CommandQueue→UI thread 重過互鎖。安全關鍵設計輪，需使用者在場逐項過
（wire protocol 的 cmd/ack 已在 ARCHITECTURE.md §4，缺的是 handler 側互鎖設計）。
夜間波次撞到「沒有寫入就沒有意義」的表單 → 跳過排佇列，換下一個。

## §5 驗收 gate（承接 PT 兩級制，加 web 專屬）

- **先判有無行為變更**：零行為變更走 `g++ -E` preprocessed 比對＋單檔 `-O3` 探雷＋
  一次 Debug build；有行為變更走**全新 dir、Debug 與 Release 各一次、
  失敗集合逐項相同且 ⊆ §2 常駐六項、數字在最後一次整併之後量**。
- **web/tag 波加 e2e 探針**：`wb_publish --pump`（或 `--with-config --dry`）+
  `wb_gateway` + `tools/webprobe` 驗 frame 與 tag 值。**不得與 ctest 並行**
  （`WB_TcpLink` 撞埠假失敗）。`wb_serve`/`wb_publish` 一律 `--dry`
  （`common.cpp:89` 指向量產共用 `system\Gerneral.ini` 未拆家）。
- **system/ 對帳**：波次收工比對 `D:\HT9045\system` 550 檔 MD5+mtime
  （FW-0 修好 BinCount 前，每波必查；修好後抽查）。
- web/ 的 JS 改動**沒有編譯器把關**（無 node）：formview 走「未知輸入畫佔位不丟例外」
  的防禦式寫法，e2e 探針只驗 transport 層；**渲染回歸的最終防線是使用者早上按 F5**，
  這個侷限誠實寫進每波回報。
- 其餘不變：EOL 逐檔保持、零 U+FFFD、`git add` 逐檔點名、一顆 commit 一件事、
  行為變更獨立 commit 獨立量、V899 與 golden 樹唯讀。

## §6 夜間 loop 作業協議

- **啟動**：使用者輸入 `/loop /fw-wave`（dynamic 自我節奏）。每次 `/fw-wave` 執行
  **一個**波次（步驟見 command 檔），收工 commit + DEVLOG + 🔖RESUME 後立即開下一波
  ——**回合結束前不准閒著**（pt-wave-loop 規則一，全承接）。
- **雙保險**：每輪照樣武裝 ScheduleWakeup（有背景工作時的保險）＋掛一個
  **cron 心跳（20 分鐘，守衛式：先查有沒有在跑，有就不介入）**。
- **額度中斷（實測行為，pt-wave-loop 記錄）**：背景 build/ctest 照跑；
  workflow agent 會死（`resumeFromRunId` 回放）；ScheduleWakeup 鏈斷；
  **cron 心跳是自動復原路徑**——解除後下一次觸發自己回來讀結果續跑。
  **前提是 session 沒關**（client 開著、機器不重開）。session 死了就是磁碟上的
  commit + RESUME 接手：使用者重開後再打一次 `/loop /fw-wave`，冷啟動協議保證無損。
- **冷啟動協議**（每次新 invocation）：`git log -5` → `git status`（在製先收完）→
  查背景工作（在跑就不介入）→ 讀 DEVLOG 檔尾 RESUME → 從「下一步」做起。
  RESUME 與 git log 對不上時信 git log。
- **不准為了問問題而停**：安全預設值判準 (a) 行為不變 > (b) 可逆 > (c) 樹保持綠，
  選擇與被否決方案寫進 DEVLOG 供事後審。安全關鍵項=「不做」不是「停下來問」。
- **model/effort**：波次設計／整併／複驗／emit_web＝主迴圈（xhigh）；
  表單邏輯翻譯 agent＝Sonnet high；唯讀稽核 agent＝Sonnet high
  （BLOCKING findings 主迴圈自己複驗）；文件雜務＝low。

## §7 停止條件（真正要停的只有這些）

| 條件 | 動作 |
|---|---|
| **安全關鍵**：write path、互鎖、任何會下真機動作的、解閘馬達/安全門類 gate | 佇列，**不做也不問**，繼續下一個唯讀項目 |
| 同一標的 gate 紅兩次同根因 | 記錄根因、換下一個標的 |
| 額度耗盡 | 寫完 DEVLOG + RESUME 再停（cron 會自己回來） |
| FW-3 佇列清空且 FW-W 未定 | 停、回報，等使用者裁決 write path 設計輪 |

## §8 風險與既知陷阱

**pt-wave-loop 的五個陷阱與硬邊界全部適用**（archive-extraction 五形狀、absence-claim
過期、前提死≠退役、static-init 18 NULL 全域、同名 class 雙標頭；該 skill 是單一出處，
此處不複寫以免漂移）。表單戰役追加：

1. **表單 ctor 是陷阱 #4 的高危區**：golden 表單 ctor 常在建構時讀全域/檔案。
   form-core ctor 只塞欄位；初始化邏輯搬到顯式 `Init()` 由消費端呼叫。
2. **`.dfm` 是 Big5 且不手改**：一切版面資料走 dfm2rc IR，emit_web 不自己開 .dfm。
3. **events.gen.json 的 handler 在唯讀階段全部不接**：接了=隱性 write path。
4. **web/ 與 906 樹是兩個世界**：web 只認識 tag，永不 import 機台概念；
   `WebBridgeTags.cpp` 仍是唯一允許混兩個世界的 TU。
5. **兩個 session 共用 repo**（V899 在 Claude app 側）：`git add` 逐檔點名、
   絕不 `git checkout` 還原、commit 不加尾隨 pathspec。
6. **量產機資料**：`system/`、`config/`、`CFG/`、`setup.inf` 只讀；
   任何探針/測試的寫入都要有 scratch 重導與事後 MD5 對帳。

## §9 完成度量法（引用必附分母與單位）

| 軸 | 量法 | 分母 |
|---|---|---|
| 表單邏輯 | `python tools/census/census.py --detail` | 118 表單單元 261,862 golden code 行（+Command.cpp 記在表單帳上） |
| 畫面活資料 | `tools/webprobe` 對線上 snapshot 數「發布且非 null」 | tagmap 綁定數（開工重量，20260817=296） |
| 表單畫面 | `web/forms/*.layout.json` 存在且 formview 渲染零未知 widget | 133 個 dfm2rc IR 表單 |

三個軸不可互相換算；回報時三個一起給。
