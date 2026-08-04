# Gate A「首次點亮」波次計畫（GATE_A_FIRST_LIGHT_PLAN）

> AI(W906-GateA-plan) 20260804: 本檔把「HT9045.exe 第一次開得起來」定義成一條**垂直切片**並展開成可執行波次。
>
> - 定位：**不取代** `MIGRATION_ROADMAP.md`（總路線）與 `W7_UI_ARCHITECTURE_PLAN.md`（UI 架構權威）；本檔是把 W7 plan 的 W7-U0～W7-U 首表單 + 必要前置 substrate 重排成「以 exe 點亮為出場條件」的執行順序。與 W7 plan 衝突時，**架構決策以 W7 plan §2 為準（D1–D12/DA1–DA4 全部沿用）**，執行順序以本檔為準。
> - 動機（2026-08-04 實測）：全 corpus 735,867 行已翻 30.8%，但根 `CMakeLists.txt` **`add_executable` 出現 0 次**——照現行由下而上順序，視窗會在非常後面才出現。使用者裁決：把首次點亮拉成優先垂直切片。
> - 本檔自足：無本場次記憶的 agent 只讀本檔 + W7 plan 即可承接任何一波。
> - **本檔另一個核心目的：每波開工前的 model/effort 建議值（§5）**——使用者要在導入過程動態調整模型，本表就是預設值與調整規則。

---

## §1 Gate A 定義（出場條件，全部要過）

1. `build_msvc_ui`（Ninja、Release、`-DHT9045_UI=ON`、`/MD` + `/D_AFXDLL`、32-bit `vcvarsall x86 -vcvars_ver=14.44`）連結出 `HT9045.exe`。
2. 開發機直接執行：fMain 視窗出現，layout 來自 `main.rc` + `main_layout.gen.cpp` 像素表（D7），自製控制項（LED/BtnPanel/Tray）以 CWnd 殼真實繪製（W7-C 的 core+renderer 已完成，只缺殼）。
3. 訊息迴圈存活：至少一個可目視的心跳（時鐘 label 或 RunState LED 由 timer 更新）。
4. `MainProc()` 在 exe 內以 Gate A pump 機制（§4-GA0 決策 2）跑在 Sim HAL 上——W6.6 spine 已在 ctest pump 過，這裡是「同一條 spine 在真 exe 裡跑」。
5. 正常關閉，啟動過程無 modal 錯誤彈窗。
6. 非 UI oracle 不退步：MinGW 全新 build dir ctest 基準 **114/118**（4 個永遠的 config 環境漂移）、mojibake 0、EOL 逐檔保持。

**Gate A 明確不含**（見 §9 排除清單）：其餘 119 表單真 UI、SECS 上線、實機硬體、Timer1 大 ladder 全量、ShowRunLed/ShowRunLabel 全量。

---

## §2 實測地基（2026-08-04 recon，全部親自量測非轉述）

### 2-1 golden 啟動鏈解剖

| 環節 | golden 位置 | 行數 | 現況 |
|---|---|---|---|
| WinMain | `HT9045.cpp:127-301` | 303（全檔） | 0% 翻。內容：taskkill OSK + 4 個 OSK reg、`CreateMutex("MyMutexName")`+`GetLastError` 單實例檢查、exe 路徑必須含 `D:\HT9045\EXE\`、BootLog 三檢查點、**118 個 `Application->CreateForm`**（初版誤記 120——:284/:285 是 log 字串；115 唯一指標+3 組 golden 重複建構，GA-0 recon 已實測，見 `RECON_GateA_FormRegistry.md`）（fMain 第一、dmTrayMotor 第二）、`Application->Run()`、雙 catch |
| TfMain ctor | `main.cpp:1340-2266` | 926 | 0%。主要呼叫：`CheckAndReadIniDataGeneral`×25、`WriteIniDataGeneral`×14、`TMyStringList`×27、`SetColorMap`×11、`TArm`×4、`RegisterHotKey`×5、`MyDBOpenDB`×1（→需 cMyDB+sqlite3）、`uPlateInfo`/`cInArmPlacement` 建構 |
| FormCreate | `main.cpp:26686-26707` | 21 | 0%。只有 CheckWindowsIsXP/DirectoryExists/MoveFile——便宜 |
| FormShow | `main.cpp:9141-11367` | 2,226 | 0%。`SetAliasAndTask`×281、`MyForceDirectories`×70（已翻）、`InitialHandler`（→cinitial）、`ReadTechData`/`DoReadLastData`/`ReadLastSetIni`（→cinitial+LastSet）、`SetRunStartMode`×5、CCD `SendCommToVision`×5、`UpdateForm`×10；**`InitialOK=true` 在 :10464**——它是 Timer1 與 MainProc 的總閘 |
| 主 pump | `uruncontrol.cpp`（全檔 86 行） | 86 | 0%。`TRunControl : TThread`，`Execute()` 迴圈 `Synchronize(ThreadProcess)`→`if(InitialOK) MainProc();`——**golden 的 MainProc 本來就全部跑在 UI thread**（Synchronize 語意），`SystemStart`/`iHome`/`fContact->fShow` 決定 sleep 節奏 |

### 2-2 已在手上的資產（不用重做）

- `tools/dfm2rc/rc_out/main.rc`（1,420 行：**91 個 DIALOGEX**［1 root + 90 child 容器，D8 真巢狀］、~946 個控制項語句）+ `main_ids.h` + `layout_out/main_layout.gen.{h,cpp}`（1,250 行像素權威表）——**fMain 的資源與幾何已產好**。133 表單全套皆有。
- `vclcompat/{LedCore,BtnPanelCore,TrayCore}.{h,cpp}` + `vclcompat/render/`——自製控制項 core+renderer 完成，缺 3 個 ~50 行 CWnd 殼（W7-C5）。
- `forms/` 已有 18 組 facade 表單檔 + `FormWidgets.h`；`FormsFacade.h` 是 3,655-byte 薄傘（W7-F0 完成）。
- MSVC 第二 oracle（W7-A1）已落地；MFC 元件已安裝並實編驗證（ROADMAP §3 2026-07-29 更正）。
- csystem `DoAllProcess`/`MainProc` spine + 五引擎已能在 Sim HAL pump（W6.6）。
- fMain HWND 預算：~1,040（946 控制 + 91 dialog + 殼），**遠低於** 10,000 行程配額——HWND 風險（W7 plan §9-R3）是全表單問題，不是 Gate A 問題。

### 2-3 Gate A 的硬缺口（本計畫的全部工作量）

| 缺口 | golden 行數 | 波次 |
|---|---:|---|
| WinMain/app 骨架 + FormRegistry | ~300 譯 + ~500 新寫 | GA-0 |
| LastSet.h 全量（~450 欄）+ LastSet.cpp | 1,162+18 | GA-1-B1 |
| cprod.cpp ungate | 3,845 gated | GA-1-B2 |
| cpublic.cpp queue ungate（含 link-graph 決策） | 818 gated | GA-1-B3 |
| sqlite3 amalgamation vendor + cMyDB.cpp | 2,047 譯 | GA-1-B4 |
| language.cpp | 746 | GA-1-B5 |
| database.cpp ReadGeneralIni | ~1,240 | GA-1-B6 |
| cinitial.cpp 全檔（64 函式） | 15,242 | GA-2 |
| main.cpp 啟動鏈垂直切片 + uruncontrol | ~5,500–6,500 | GA-3 |
| MFC 首燈本體（殼/ApplyLayout/binder/巢狀引擎） | ~2,000–3,000 全新寫 | GA-4 |
| link 閉包 + 首燈整合除錯 | 未知（含 ckernel 三符號，已有 brief） | GA-5 |

合計 ≈ **31k 行翻譯 + ~3k 行新寫**（比 2026-08-04 對話中的粗估 45–55k 低，因為 main.cpp 由全檔改為垂直切片）。

---

## §3 波次總覽與依賴

```
GA-0 (SERIAL, 架構定案+app骨架)
 ├──> GA-1 (substrate ×6 批, 平行)   ──┐
 ├──> GA-2 (cinitial 全檔, 單檔切塊)  ──┼──> GA-3 (main.cpp 垂直切片, SERIAL) ──> GA-5 (整合+首燈, SERIAL)
 └──> GA-4 (MFC 首燈 UI, 新寫) ────────┘                                    ↗
      （GA-4 與 GA-1/2/3 全程平行，只在 GA-5 會合）
```

- GA-1 六批互相獨立可 6 agent 平行；B3 需 GA-0 決策 3 先定案。
- GA-2 是**單一檔案**——切塊序列寫入（或 fragments+integrator），不可多 agent 同時寫同檔。
- GA-3 動 `main.cpp` 鏡射檔與 `forms/fMain.*`（皆 SERIAL 檔）→ 翻譯 agent 產 fragment、主迴圈序列 integrate。
- 節奏依既有使用者指示：連續推進、不逐波停下請示；重大問題跳過並最後條列。

**時程量級**（以 23 個實測工作日 / 226,515 行 ≈ 9.8k 行/日的既往速率，只適用機械翻譯部分）：GA-0 ~1 日、GA-1 ~2 日、GA-2 ~1.5 日、GA-3 ~1.5 日、GA-4 ~2–3 日（大半與前三者平行）、GA-5 ~1–2 日 → **關鍵路徑約 7–10 個工作日**。GA-4/GA-5 是新寫+未知 fallout，此估計信心最低。

---

## §4 各波詳述

### GA-0 — 架構定案 + app 骨架【SERIAL，第一個做】

**範圍**：
1. ~~落地 **DA2**：根 `CMakeLists.txt` 釘 `_WIN32_WINNT=0x0601`~~ **✅ AI(W906-GateA-0) 20260804 更正：DA2 早已於 2026-08-01（`W7-L1-Wave0`）落地**——根 `CMakeLists.txt:43` `add_compile_definitions(_WIN32_WINNT=0x0601 WINVER=0x0601)`，含前後警告 diff 驗證（277→284 行，+7 全是同批 LastSet 欄位的 -Wmissing-field-initializers）。本檔 2026-08-04 初版寫「grep 零命中」是照抄 W7 plan §2-DA2 的 2026-08-01 快照、未重新查證，違反了自家「行號引用動筆當下重新推導」紀律，特此記錄。
2. **W7-U0**：補 `option(HT9045_UI "Build the MFC UI executable (MSVC only)" OFF)`——`scripts/build_msvc.bat:90` 已在傳這個未定義的 `-D`；`ht9045_app` target 包在 `if(HT9045_UI AND MSVC)`，**預設 OFF 保住 MinGW oracle**（ROADMAP :22）。
3. 建 `build_msvc_ui`（**Ninja** generator，D10——絕不用 VS multi-config，那是 2026-07-28 modal assert 洗版事故的載體）。
4. 空殼 app：`ui/HT9045App.{h,cpp}`（CWinApp）+ WinMain 語意翻譯——mutex 單實例、BootLog 檢查點接已翻的 `cBootLog`、exe 路徑檢查（**決策：參數化或保留 golden 字面 `D:\HT9045\EXE\`**）、OSK taskkill/reg（**建議 Gate A skip + 揭露性註記**，它是機台 kiosk 行為非啟動必要）。里程碑：**空視窗 exe 能開能關**。
5. 四個設計決策寫成 `docs/DESIGN_GateA.md`：
   - **決策 1（FormRegistry）**：golden 118 個 CreateForm（115 唯一）→ Gate A 只有 fMain 拿真 HWND（D3：`CDialog` 持有 `TfMainImpl : public TfMain`，`fMain=&impl`）；其餘照 golden CreateForm **順序**建構既有 facade 物件（無 HWND）——有些 ctor 有真副作用（例：`THGem` FormCreate SV 註冊已翻），順序不可亂。✅ 已裁決，見 `DESIGN_GateA.md` D-A0-2。
   - **決策 2（pump）**：golden `TRunControl::Execute` 的 `Synchronize(ThreadProcess)` 使 MainProc 實際全跑在 UI thread → Gate A 建議直接用 UI thread 高頻 timer（`SetTimer` 1ms + 重入 guard）等價替代，**不開真 worker thread**（等價性論證寫進 DESIGN；真 thread 版留給 Gate B）。
   - **決策 3（link graph）**：裁決 `ht9045_globals` → link `ht9045_core`（解 cpublic.cpp 3 處 + TempChangeLog 的既有阻塞，ROADMAP DEFERRED 表 common.cpp 列）——需先盤點受影響 test target 清單。
   - **決策 4（FormShow gating 清單）**：逐段判 FormShow 2,226 行哪些 Gate A 真翻、哪些掛 seam（CCD `SendCommToVision`→seam；SECS 開通→維持既有 offline；`uHeaterThread`/`ScanBtnThread`→**不啟動**，掛計數器 stub）。
6. 開場條款：先 `git status` 處理工作樹在製工作（目前已知：`tests/test_w906_trace.cpp` + `tests/CMakeLists.txt` 是 DoTraceDataResponse 補測試的半成品，**第四次**同型事件）——照既有復原 SOP（fresh build+ctest+審查非重譯）先落地或明確擱置，再開 GA-0。

**驗證閘**：空殼 exe 連結+啟動+關閉；MinGW 全新 dir ctest 114/118 不動；`build_msvc`（無 UI）續綠。

### GA-1 — substrate 六批【平行 fan-out】

| 批 | 內容 | oracle |
|---|---|---|
| B1 | `LastSet.h` 全量（1,162 行 ~450 欄）+ `LastSet.cpp`（18） + `canary_support.h` minimal shim 退場 | PROD_INFO_ST 的 offsetof/static_assert 前例；shim 退場後全樹重編 |
| B2 | `cprod.cpp` ungate（3,845 gated 行）——範圍條款：**ungate what links, document rest**（有些 body 依賴仍未翻的模組，逐函式判定，不硬翻） | 既有 cprod 測試 + 新增 ungate 函式 spot oracle |
| B3 | `cpublic.cpp` queue/union/VerInfo ungate（818 行）+ 移除測試 stub ctor（ROADMAP :126 明言 exe 連結前必須 ungate）。**前置：GA-0 決策 3** | queue 行為單元測試 |
| B4 | `third_party/sqlite3/` amalgamation 真 vendor（目前只有 CMakeLists+README）+ `cMyDB.cpp`（2,047）。注意 `MyDBIProcess` `__fastcall` 本體要從 uHGemEquipment.cpp **遷回 golden 家 cMyDB.cpp**（ROADMAP vcl_compat 列的既定計畫） | 開 `system/*.DB` 真檔讀寫對拍 |
| B5 | `language.cpp`（746，語系 csv 載入） | 真語系檔 |
| B6 | `database.cpp` `ReadGeneralIni`（~1,240） | 真 `Gerneral.ini`（test_config_db 前例） |

**驗證閘**：每批獨立 fidelity review；B2/B3 涉狀態機/互鎖旗標 → 複驗升 xhigh；全批完成後 fresh MinGW build+ctest。

### GA-2 — cinitial.cpp 全檔【單檔切塊，序列寫入】

64 函式 15,242 行，切 5 塊（golden 行界）：
1. `:77-3060` Sucker/Switch/Sensor 名稱+初始化
2. `:3061-5613` Motor/Cylinder 參數 + 速度家族
3. `:5614-7709` `InitialClass`～`InitialMachine`（含 `InitialHandler`——FormShow 的主要 callee）
4. `:7710-13583` MachineRecord + `SetTechDataToProd_*` 家族（9 支同型）
5. `:13584-15242` 尾段（`ReadTechData`/`SetOffsetToTech`/`InitialHT9045SModule`/`InitShuttleThreadParameter`）

它寫 `MOT[]`/`Sen[]`/`SW[]`/`Cylinder[]` 的名稱/參數映射=**安全關鍵資料流**（錯一欄=實機動錯軸），複驗不可省。oracle：真 config 檔（`Gerneral.ini`/`Mot_Table.csv`/`IO_Table.csv`）+ SetTechDataToProd 數值對拍。同檔序列：一次一個翻譯 agent 寫檔，或 fragments 由 integrator 合併（兩法皆有前例）。

### GA-3 — main.cpp 啟動鏈垂直切片【SERIAL integrate】

**前置**：GA-1 B1/B2（ctor 大量讀 LastSet/Prod）、GA-2（FormShow 呼叫 InitialHandler/ReadTechData）。
**範圍**：ctor 926 + FormCreate 21 + FormShow 2,226（依 GA-0 決策 4 的 gating 清單）+ `uruncontrol.cpp` 86（依決策 2 改 timer pump）+ callee 閉包（`SetRunStartMode` ~754、`ProcessTimeUpdate`、`MainFormSizeToEpson` 等，逐個 recon 判翻/seam）+ Timer1/2/3 **骨架+重入 guard**（`InitialOK` 總閘照翻；大 ladder 掛 seam 並逐項記 DEFERRED）。
**紀律**：`main.cpp` 鏡射檔與 `forms/fMain.*` 都是 SERIAL 檔（W7 plan §8 擁有權例外）——翻譯 agent 只產 fragment + 需求清單，主迴圈序列落地；所有行號引用動筆當下重新推導。

### GA-4 — MFC 首燈 UI 本體【全新寫，與 GA-1..3 平行】

**範圍**（全部是新檔，零碰撞）：
1. **W7-C5**：`CLedCtrl`/`CBtnPanelCtrl`/`CTrayCtrl` 三個 CWnd 殼（各 ~50 行，`CBtnPanelCtrl` 從 `CStatic`+`SS_NOTIFY` 派生——W7 plan §7-6）+ `HT9045_RegisterAllCustomClasses()`。
2. **泛用 ApplyLayout 引擎**：吃 `_layout.gen.cpp` 像素表 → `MoveWindow` + `CreateFontIndirect`/`SetFont`（per-control 字型）+ z-order。**寫一次、全 133 表單日後受益**——這是本波最大槓桿，介面要 data-driven 不要 fMain 特化。
3. **child DIALOGEX 巢狀建立引擎**（D8：91 個 DIALOGEX 的樹狀 CreateDialogIndirect 掛載）。
4. **泛用 OnCtlColor**（背景/前景色表驅動）。
5. **TfMainImpl binder**：`PullFromControl`/`PushToControl` 只涵蓋 GA-3 切片實際觸碰的 widget 子集（不是全部 946 個）；可先用純 Win32 + `HWND_MESSAGE` parent 做 headless 測試（W7 plan §7 已設計，構造上不彈窗）。
6. message map 子集：`OnClose`、timer、GA-3 切片內的按鈕。

**驗證閘**（無 golden 可對拍，此波驗證方式不同）：`rc.exe` 權威閘 + `.res` 二進位回讀（G5/G6 既有）+ headless binder 測試 + 建置雙 oracle 不退步；目視延到 GA-5。

### GA-5 — 整合 + 首次點亮【SERIAL，主迴圈】

1. link 閉包：以 **整個 link 閉包的殘餘未解析集合**（nm 對差，DEVLOG 經驗 7）收斂 undefined symbols；已知的 ckernel 三符號（`AccelateTask`/`IsSafeLockCheck()`/`InitDoArmZHome()`）已有可執行 brief（ROADMAP DEFERRED :152），落地時同批刪 test 的 LINK-TIME SUBSTRATE FILL（有 duplicate-symbol 絆線提醒）。
2. 首次執行 + `W7-UI-SKIPPED.md` 目視檢查表：z-order 極性（`--zorder` 開關一次翻轉）、CJK 字型替代、3,338 對重疊 sibling 抽查。
3. Sim 冒煙：START → MainProc tick 可目視；關閉乾淨。
4. fallout 修復迴圈。任何自動化冒煙跑法必須掛 SetErrorMode/_CrtSetReportMode（既有 INTERFACE library 前例——批次跑的 Windows 程式不可有 modal 彈窗）；互動首燈由使用者或主迴圈在桌面環境跑。

---

## §5 Model / Effort 對照表與動態調整規則（本檔核心）

> 慣例：**每波開工，主迴圈先用一句話報該波設定**（「本波：主迴圈 X + effort Y；翻譯 subagent Z；複驗 W」），預設值取自本表，臨場偏離要附一句理由。主迴圈 model 由使用者 `/model` 手動切（成本槓桿在使用者）；subagent model/effort 由主迴圈 per-call 指定，不需使用者切。

### 5-1 預設值

| 波 | 主迴圈（session model） | 翻譯 subagent | 複驗 subagent | 理由 |
|---|---|---|---|---|
| GA-0 | **Fable 5 + xhigh** | —（recon：Sonnet 5 high） | — | 架構決策×4，錯地基重工成本最高 |
| GA-1 | Opus 5 high（可降 Sonnet 5 high：純 integrate 對帳時） | Sonnet 5 high | Sonnet 5 high；**B2/B3 升 xhigh**（互鎖/狀態機旗標） | 六批多為機械翻譯，本專案實證 Sonnet high 多波 0-discrepancy |
| GA-2 | Opus 5 high | Sonnet 5 high（切 5 塊防長 ladder 一致性衰減） | **Sonnet 5 xhigh 固定**（MOT/Sen/SW/Cylinder 映射=安全關鍵） | 同型家族多（SetTechDataToProd×9），適用降級規則 D-a |
| GA-3 | **Fable 5 + xhigh**（切片設計+SERIAL integrate 都在主迴圈） | Sonnet 5 high（產 fragment） | Sonnet 5 xhigh | 啟動鏈錯=全盤白工；SERIAL 檔寫入只在主迴圈 |
| GA-4 | **Fable 5 + xhigh** | Fable/Opus xhigh（新寫碼不派 Sonnet） | 主迴圈親審 + headless 測試 | **全計畫唯一不許降級的波**：無 golden oracle，論證品質就是唯一防線 |
| GA-5 | **Fable 5 + xhigh** | recon/除錯輔助：Sonnet 5 high | — | 未知 fallout 除錯 |
| 文件/commit/mojibake·EOL 掃描（各波共用） | — | Haiku 4.5 或 Sonnet 5 low | — | 機械檢查 |

### 5-2 動態調整規則

**升級觸發**：
- U1 複驗在同一 agent 連兩輪抓到引用造假/off-by-N → **該檔收回主迴圈自己寫**（既有教訓：agent 的論證比程式碼更常錯，12 findings 裡 11 個是引用造假）。
- U2 安全關鍵 finding 只到 PLAUSIBLE 收不到 CONFIRMED → 複驗升 xhigh 換一種獨立方法重導（差分 mutation / reference-function 差分，DEVLOG 經驗 9）。
- U3 翻譯中發現需要**新設計**（golden 無對應可逐字對拍）→ 停該塊，升 Fable xhigh 出 DESIGN brief 再續（前例：DoDLRequest 防護 seam）。
- U4 GA-5 除錯同一症狀 >2 小時無收斂 → 維持 Fable，另開獨立視角 recon agent（不換模型，換方法）。

**降級觸發**：
- D-a 同型批次連 2 批 0-discrepancy（如 SetTechDataToProd 家族、GA-1 B5/B6 這類低風險 reader）→ 複驗降一級或改抽查；**翻譯保持 Sonnet 5 high 不再降**（本專案實證甜蜜點，降過頭省小錢賠重譯）。
- D-b 純機械對帳（md5/行數/EOL/mojibake）→ Haiku。
- D-c 主迴圈在長 fan-out 波只做派工+收攏 → 使用者可把 session 切到 Sonnet 5 省成本，**但 GA-3/GA-4/GA-5 期間不要**。

**不可降級清單**：GA-4 全部；任何 SERIAL 檔（`csystem.cpp`/`forms/fMain.cpp`/`main.cpp` 鏡射/`canary_support.h`）的寫入；互鎖・運動控制・警報相關 ungate 的複驗。

---

## §6 碰撞矩陣增補（沿用 W7 plan §8，Gate A 新增列）

| 檔案 | 規則 |
|---|---|
| 根 `CMakeLists.txt` | GA-0 獨佔（`_WIN32_WINNT` + `HT9045_UI` + `ht9045_app`）；GA-1 B3/B4 若需動 library link 區，序列排在 GA-0 之後 |
| `canary_support.h` | GA-1 B1（LastSet shim 退場）大改它——**只由主迴圈寫**（既有 SERIAL 規則，123 個 TU 的 include 面） |
| `main.cpp` 鏡射 / `forms/fMain.{h,cpp}` | SERIAL，只由主迴圈 integrate（既有規則） |
| `cinitial.cpp`（新鏡射檔） | GA-2 同檔序列，一次一 agent |
| `ui/`（GA-0/GA-4 新目錄） | 全新檔，GA-4 內部三路（殼/引擎/binder）可平行 |
| `tests/CMakeLists.txt` | MERGE-AT-INTEGRATE（既有慣例） |

---

## §7 風險（Gate A 特有）

| # | 風險 | 處置 |
|---|---|---|
| G1 | **x87 vs MSVC 浮點**：MinGW 是 BCB6 算術 oracle，exe 卻是 MSVC——首燈階段 UI 不做浮點對拍即可，但 GA-3 若切片內有算術，測試留在 MinGW 側跑 | 已知結構（D9 雙 oracle），標記不擴大 |
| G2 | FormShow 的 281 處 `SetAliasAndTask` 等大量 widget 觸碰 → binder 覆蓋面被迫擴大 | GA-0 決策 4 逐段裁 gating；binder 只做切片實際觸碰集 |
| G3 | cprod ungate 拉出未翻依賴鏈 | B2 範圍條款「ungate what links, document rest」，不硬翻 |
| G4 | z-order 極性 / CJK 字型——headless 無法解 | 既有設計（§9-R1）：`--zorder` 產生器開關一次翻轉；首燈目視時裁決 |
| G5 | 118 CreateForm 順序性副作用（facade ctor 有真邏輯；SIDE-EFFECT 48/115） | FormRegistry 照 golden 序建構；副作用 ctor 清單已 recon 完成（`RECON_GateA_FormRegistry.md`） |
| G6 | 開場工作樹在製工作（trace 測試半成品）與 GA-0 混雜 | GA-0 開場條款：先照復原 SOP 處理再動工 |

---

## §8 每波共同收尾閘

沿用 W7 plan §12 ratchet 全部，基準更新為：**ctest 114/118**（fresh MinGW dir）、mojibake/U+FFFD 全樹 0、EOL 逐檔保持、`grep -ic resolving`=0（`__fastcall` 配對紀律）、每波獨立 commit + DEVLOG/RESUME 更新；UI 波加 `rc.exe` 權威閘 + `.res` 回讀；MSVC `build_msvc` 第二 oracle 同批 95+ 測試續綠。

---

## §9 明確排除（Gate A 不做，防 scope creep）

其餘 119 表單真 UI（含 iosetview 的 HWND 大戶）；Timer1/2/3 大 ladder 全量與 `OnMyCopyMsg`（1,830 行 WM_COPYDATA 分派器）；`ShowRunLed`/`ShowRunLabel` 全量（僅狀態寫入清單維持 DEFERRED :154 追蹤）；SECS 上線與 `uHGemHT9045` 22 override；`uHeaterThread`/`ScanBtnThread` 真啟動；廠商馬達驅動×6 與 `TempCtrl/`；`cContact` 下壓叢集；`Command.cpp`/`rs232.cpp`；`uLotInfo.cpp` 業務面。這些分屬 Gate B（離線功能完整）與 Gate C（實機）。
