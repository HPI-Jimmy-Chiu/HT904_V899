# W7 UI 架構定案與波次計畫（W7_UI_ARCHITECTURE_PLAN）

> 本檔是 **W7（UI 層 + 其前置改造）的唯一權威計畫**，由 2026-07-28 的五路平行調查（formsfacade-fate / dfm-to-rc / custom-controls / dual-toolchain-build / remaining-cpp-recon）經**衝突裁決 + 獨立複驗**後合併而成。
>
> - 總路線仍見 `MIGRATION_ROADMAP.md`；逐步紀錄見 `DEVLOG.md`；gotcha 見 `KNOWLEDGE.md`。本檔**不取代**它們，只是把 ROADMAP 第 20 行「W7 — UI（133 .dfm 表單）：先定 UI 框架＝獨立大決策」這一格展開成可執行內容。
> - **ROADMAP 第 9 行與第 20 行的「UI 框架延到 W7 再定」已過時**——使用者已定案 **MFC**。本檔第 §2 D1 起即以此為前提，不再重新討論。修正 ROADMAP 本體由主迴圈負責，本檔僅記錄（見 §10）。
> - 本檔設計成**自足**：任何沒有本場次記憶的 agent，只讀本檔即可承接 §6 任何一波並知道它的範圍、檔案、驗證閘。

---

## §1 名詞與座標

| 名稱 | 位置 | 權限 |
|------|------|------|
| GOLDEN（黃金唯讀真相） | `D:\HT9045\HT9011UC_Code_V3.33.906.0_20260618\` | **永不修改** |
| PORTED（譯出樹） | `D:\HT9045\HT9011UC_Cpp_V3.33.906.0\` | 可讀寫 |
| 元件庫 A | `D:\HT9045\elec\myvcl\` | 唯讀參考（**5 個主要自製控制項在此**） |
| 元件庫 B | `D:\HT9045\elec\Component\` | 唯讀參考（`aled.pas`/`aled.res`/`apheadlabel.hpp`/`MyMemo.*` 在此） |
| 主要建置目錄（MinGW） | `PORTED\build_resume_verify_20260727` | 既有 configured 目錄 |
| 基準（baseline） | ctest **91/95**，4 個既有環境漂移失敗：`config_db`、`IniFiles`、`ini_helpers`、`config_loaders` | 任何波次不得惡化 |

工具鏈事實（2026-07-28 實測）：

- `C:/MinGW/bin/g++.exe` 6.3.0（`-dumpmachine` = `mingw32`，**32-bit x87**），CMake 4.0.2，`MinGW Makefiles` generator。
- MSVC **有**：VS 2022 BuildTools 17.14.3（`cl.exe` 14.44.35207 / 14.42.34433）+ VS 2019 BuildTools 16.11.47（14.29.30133）。
- **MFC 元件未安裝**：`atlmfc\` 只剩 `lib\spectre\arm64` 空殼，無 `include\`、無 `afxwin.h`、無 `mfc*.lib`。需使用者跑 VS Installer 加「C++ MFC for latest v143 build tools」。**agent 不得自行安裝。**
- `rc.exe` **有**：`C:\Program Files (x86)\Windows Kits\10\bin\10.0.22621.0\x64\rc.exe`（屬 Windows SDK，**不屬 MFC**）。`windres` 也有：`C:\MinGW\bin\windres.exe`。→ **資源編譯閘門今天就能跑，不等 MFC。**

---

## §2 已鎖定的決策（不再重新討論）

| # | 決策 | 一句話理由 |
|---|------|-----------|
| **D1** | UI 框架 = **MFC**（使用者已定案） | 已定案，本檔在此框架內設計 |
| **D2** | `FormsFacade` **永久保留**，且是「**具體可綁定 facade**」——資料成員維持具體（`fMain->edX->Text` 逐字可編）、方法面改 `virtual` | 實測 golden `fMain->` 4,266 處中 **69.3% 是資料成員語法**（NESTED 56.2% + DATA 13.1%），C++ 無 property，純虛擬介面**在算術上不可能**表達 |
| **D3** | **不刪 facade**。MFC `CDialog` **持有** `TfXxxImpl : public TfMain` 成員並 `fMain=&impl`（組合，非多重繼承 `CObject`） | facade 是測試套件的主要 fixture（15 個 test 檔直接 deref），`CDialog` 在 MinGW ctest 行程裡建不出來；刪 facade＝刪唯一 oracle |
| **D4** | Widget stand-in 統一到 **`vclcompat/Controls.h`（只放 stock VCL 控制項，~20 型）**，且**每型必須 `: public TObject`** | 現況已有 ~13 份平行 `{AnsiString Text;}` 型別在漂移；且無 `TObject` base 會靜默綁到 `SetSVDataPointer` 的 `void*` 多載（實測 `SecsSvEcRegistration.h:133` vs `:149`） |
| **D5** | 3 個**自製**控制項核心（`TrayCore`/`BtnPanelCore`/`LedCore`）放**各自新檔**，**不進 `Controls.h`** | 避免 F0 與 C1 波搶同一檔；且它們是有行為的核心，不是 value-holder |
| **D6** | `FormsFacade` 下沉成 **`ht9045_forms` 底層 library**（只依賴 `vclcompat`），並**按表單拆成 `forms/fXxx.{h,cpp}`** | 實測 `FormsFacade.cpp` 在 `ht9045_sm`（CMakeLists.txt:728/731），`ht9045_secsgem` 只 link `vclcompat ht9045_public ht9045_core ht9045_globals ht9045_db`（:588），而 `ht9045_sm` link `ht9045_secsgem`（:1208）→ **牆②真正的阻塞是 CMake 循環依賴，不是缺 stub** |
| **D7** | `.dfm` → **IR(JSON) → 4 產物**（`.rc` + `_ids.h` + `_layout.gen.{h,cpp}` + blob）。**幾何權威在 `_layout.gen.cpp` 的逐字像素表，`.rc` 的 DLU 只是可編輯佔位** | px→DLU→px 只有 **17.2–17.4%** 控制項像素精確；且 `DIALOGEX` 只允許**一個** `FONT`，而語料有 **9,933 組 per-control 字型**——`OnInitDialog` 修正 pass 本來就無可避免，`MoveWindow` 順手免費 |
| **D8** | **真 HWND 巢狀**：每個容器（TPanel/TGroupBox/TScrollBox/TTabSheet/TRadioGroup）自成一個 child `DIALOGEX`。**不扁平化** | 1,201 個容器的 `Visible`/`Enabled` 被 golden 程式碼切換，遞移管轄 33,557 個後代槽位；扁平化＝手寫 1,201 份 cascade 模擬 |
| **D9** | **雙工具鏈永久共存**，以 build 目錄區分：`build_mingw`（主 oracle）/ `build_msvc`（Release，第二 oracle，跑**同一批** 95 測試）/ `build_msvc_ui`（`HT9045_UI=ON`，卡 MFC） | MinGW 是**目前唯一能重現 BCB6 x87 算術的工具鏈**（實測見 §4-V5）；MSVC 抓 MinGW 結構上看不見的 return-type / `__fastcall` ABI 不符 |
| **D10** | **不用 `Visual Studio 17 2022` generator，用 `Ninja`** | VS generator 是 multi-config、預設 **Debug**——正是 2026-07-28 弄出 modal「Debug Assertion Failed!」洗版事故的載體（KNOWLEDGE Gotcha 9）。Ninja 單 config，`CMAKE_BUILD_TYPE=Release` 直接生效，且指令形狀與 MinGW 一致 |
| **D11** | **邏輯↔widget 的唯一 join key = `.dfm` 的葉層控制項名稱** | 實測 **133 個表單內、表單內重名 = 0**，所以葉名在表單內已唯一。`fMain->cbSetupFileName`（C++ 名）與 `IDC_MAIN_CBSETUPFILENAME`（資源 ID）由同一個葉名機械推導，兩個命名空間天然對齊 |
| **D12** | 自製控制項**照樣進 `.rc`**（`CONTROL "", id, "HT9045.MyLedLane", ...`），其 `Alias`/`LEDStyle`/IO metadata 走 **layout table**，不走 `.rc` | 保住 `.dfm`↔`.rc` 結構同構（＝checker 能做精確 isomorphism 檢查）＋保住 tab/z-order 槽位；`Alias` 的承載需求由 layout table 完全滿足 |

---

## §3 三方衝突與裁決（不粉飾）

調查是刻意互相不知情跑的，以下是真實衝突與裁決結果。

### C1（最關鍵）「邏輯程式碼怎麼碰到 widget」——三方必須一致

- **formsfacade-fate** 主張 C++ 端維持 `fMain->cbSetupFileName->Text`。
- **dfm-to-rc** 主張資源端用 `IDC_<FORMSTEM>_<CONTROLNAME>`。
- **custom-controls** 主張自製控制項**不要進 `.rc`**、改用 per-form `CustomCtrlDef[]` + `OnInitDialog` 動態 `CreateWindow`，理由是「`.rc` 沒有欄位放 `Alias`，而 `Alias` 是 IO 點的身分（`iosetview.cpp:1040-1055` 靠 `Ptr->Alias.AnsiPos("TestSuck")` 分派）」。

**裁決**：前兩者**不衝突**——它們是兩個命名空間，靠 D11 的葉名 join key 對齊。第三者**部分讓步**：

- `custom-controls` 的**結論**（不進 `.rc`）**不採納**；`dfm-to-rc` 的 D12 採納。
- 但 `custom-controls` 的**理由**（Alias 必須被承載）**完全成立且已被滿足**——`dfm-to-rc` 自己的設計就把 component-specific props（`LEDStyle`/`Alias`/`XItem`/`YItem`/`True*Color`）放進 layout table，只是它不知道對方在擔心這件事。兩者本來就相容。
- `custom-controls` 反對 `.rc` 的另一個理由（「iosetview 要手寫 1,984 行 `CONTROL`」）**不成立**——那些行是**產生的**，不是手寫的。
- `custom-controls` 提出的**真實新需求採納**：registered window class 必須在任何 `CreateDialog` 之前註冊。作法＝單一 `HT9045_RegisterAllCustomClasses()`，在 App `InitInstance` 呼叫一次（不是 39 個表單各自處理）。

### C2 巢狀 vs 扁平

- 任務簡報寫「MFC dialog 慣例上把控制項保持扁平兄弟」。
- `dfm-to-rc` 實測反對：巢狀最深 14 層、只有 3.1% 控制項是表單直屬子、**1,201 個容器被程式碼切換 Visible/Enabled 管轄 33,557 個後代**。

**裁決**：簡報那句是**對 MFC 慣例的描述**，不是需求；實測資料勝出。**採真 HWND 巢狀（D8）**。代價（HWND 配額）記為已量化風險，見 §9-R3。

### C3 F0 硬凍結 vs 「平行火力全開」

- `formsfacade-fate` 要一個橫跨全樹的**序列凍結波 F0**（6 個子步驟，含 SckArt state 整併）。
- `remaining-cpp-recon` 要**立刻**展開 7 路 asendic_* 平行翻譯，且自承「asendic 波與 ckernel 波**都會**要新的 FormsFacade 成員」。
- `dual-toolchain` 要在**下一波翻譯之前**先落地 `vcl_compat.h` 修正。

**裁決**：三者都對一部分，但 F0 的**範圍被砍**：

- 凍結時間必須極短，所以 **F0 只保留真正非序列不可的部分**（`Controls.h` 統一 + FormsFacade 別名化 + `ht9045_forms` library + per-form 拆檔 + virtual 化）。
- **TU-local seam 退場 + SckArt state 整併移出凍結**，改成凍結後的 **F2**（它是分析工作，不是機械別名化）。
- `dual-toolchain` 的 `vcl_compat.h` 修正**排在 F0 之前**（一檔、~30 分鐘、且是**所有** MFC TU 的前置）。
- `recon` 的 asendic/ckernel 波**排在 F0 之後的第一輪 fan-out**——凍結很短，這只是排程細節，不是矛盾。
- **凍結期間仍可平行的**：所有**只新增檔案**的工作（B1 整條 `.dfm` 管線、C1/C2 自製控制項核心與 renderer）、所有唯讀 recon、以及 A2（27 個 `aoutarm9045_*.cpp`，與 F0 零重疊）。

### C4 SckArt 是「3 方」還是「4 方」重複

- `formsfacade-fate` 引 `Automation/SCK_ART.h:34-45` 說是「3-way reconciliation debt」。
- **本專案自己的 `MIGRATION_ROADMAP.md:145` 早在 2026-07-21 就把它更正成 4 方**：`SckArtState`（`Automation/SCK_ART.h`）、`SckArtRemainderState`（`SCK_ART_Remainder.h`）、`csystem.cpp` 的 `W7C1_TfSCKARTSeam` **與** `W7C2_TfSCKARTSeam`。

**裁決**：**`formsfacade-fate` 引用的是過時來源，採 ROADMAP 的 4 方版本。** F2 波必須整併 **4 份**，不是 3 份。

### C5 自製控制項的原始碼在哪

- 任務簡報與 `dfm-to-rc` 都說在 `D:\HT9045\elec\Component\`。
- `custom-controls` 說**只有 2 個在那**，其餘 5 個在 `D:\HT9045\elec\myvcl\`。

**裁決**：**`custom-controls` 正確（已複驗，見 §4-V3）**。簡報與 `dfm-to-rc` 在這點上被推翻。任何 C 系列波次一律以 §1 的元件庫 A/B 分工為準。

### C6 控制項總數 22,751 vs 22,627

- 簡報（regex 計數）說 22,751；`dfm-to-rc`（完整 parser）說 22,627。

**裁決**：**22,627 正確（已複驗：22,760 個 object 節點 − 133 個 root = 22,627）**。簡報數字作廢。

### C7 「牆②是缺 19 個 stub」

- `MIGRATION_ROADMAP.md:133` 把牆② 歸因於「~19 個 FormsFacade 缺口」。
- `formsfacade-fate` 說成員數對，但**真正阻塞是 CMake 循環依賴**，補 19 個 stub 不會解鎖。

**裁決**：**`formsfacade-fate` 正確（已複驗，見 §4-V1）**。ROADMAP:133 的描述不完整、會誤導下一波。修正記在 §10 交主迴圈。

### C8 測試套件對 facade 的耦合度

- `formsfacade-fate` 說「30 個 test 檔直接 deref facade」。
- 複驗只有 **15 個**。

**裁決**：數字誇大 2 倍，但**結論不變**——15 個 test 檔＋`CDialog` 在 MinGW 建不出來，仍足以否決「刪 facade」。採 15。

---

## §4 獨立複驗結果（本檔作者親自跑，不採信自我宣稱）

本專案有「自稱 verified/FAITHFUL 一律獨立重查」的硬規矩，以下是實際重跑結果。

| # | 被查宣稱 | 結果 | 實測證據 |
|---|---------|------|---------|
| **V1** | 牆② 是 CMake 循環依賴，非缺 stub | ✅ **成立** | `CMakeLists.txt:728` `add_library(ht9045_sm STATIC`，`:731` `FormsFacade.cpp`；`:588` `target_link_libraries(ht9045_secsgem PUBLIC vclcompat ht9045_public ht9045_core ht9045_globals ht9045_db)`（無 `ht9045_sm`）；`:1204-1208` `ht9045_sm` link `ht9045_secsgem` |
| **V2** | 無 `TObject` base 會靜默綁到 `void*` 多載 | ✅ **成立** | `SECSGEM/SecsSvEcRegistration.h:133` = `void *P`，`:149` = `TObject *P`。兩者同 arity，一個裸 struct 指標只能選 `void*` |
| **V3** | 5/7 自製控制項在 `elec\myvcl\` 不在 `elec\Component\`；LED/Button 家族**零繪圖碼** | ✅ **成立** | `elec\myvcl\` 有 `HTray.cpp`(517)/`HTray256.cpp`(389)/`MyLed.cpp`(126)/`MyLedLane.cpp`(136)/`butPa1.cpp`(217)/`BtnPanelLane.cpp`(178)。`Canvas|Paint|DrawText|Rectangle|Ellipse|StretchDraw` 命中數：MyLed **0**、MyLedLane **0**、butPa1 **0**、BtnPanelLane **0**、HTray 22、HTray256 23。base：`TMyLed:TALed`、`TMyLedLane:TALed`、`TBtnPanel:TPanel`、`TBtnPanelLane:TPanel`、`TTMyTray:TCustomControl` |
| **V4** | 133 個 `.dfm` 全是文字格式、22,627 個控制項、**表單內重名 = 0** | ✅ **成立** | 133 檔、binary(`TPF0`) = **0**；object 節點 22,760 − 133 root = **22,627**；**0 個表單有表單內重名** |
| **V5** | MinGW 與 MSVC 算術不同（`(int)(1.234*1000.0)`） | ✅ **成立，但有重要細節** | ⚠️ **第一次複驗給 1234，是常數折疊假象**。改用 runtime 值後：MinGW **1233**、`sizeof(long double)==12`、`mingw32`。且 `tests/test_cUnitConvert.cpp:83` 就是寫 `CHECK_INT(iUnitMultiply1000(1.234), 1233)`。→ **宣稱方向正確，但只對 runtime 值成立；編譯期常數折疊兩邊都是 1234。任何後續驗證腳本必須用 volatile/runtime 輸入，否則會得到假結論。** |
| **V6** | `vcl_compat.h` 在 `windows.h` 先進來時會編不過（＝擋住整個 MFC 波） | ✅ **成立，親自重現** | `#include <windows.h>` 後再 `#include "vclcompat/vcl_compat.h"` → `vcl_compat.h:211: error: 'vclcompat::DeleteFile' has not been declared` + `:212: 'vclcompat::CopyFile'`。反序則乾淨。**注意：該檔 `:127-131` 自己的註解宣稱「safe whether or not windows.h happened to be included earlier」——這句話是錯的**，它只保護了 macro shadowing，沒保護 `SysUtils.h`(:102) 早於 `windows.h`(:142) 的宣告污染 |
| **V7** | `SearchTrayToPlace_Magazine` 有 return-type ODR 違反 | ⚠️ **成立，但數字要修正** | golden：`aoutarm9045.h:63` = `int`，定義 `aoutarm9045.cpp:1420` = `int`。ported：**14 個 `void` 宣告 / 13 個 `int` 宣告**（原宣稱 15/13），定義 `aoutarm_shims.cpp:95` 是 `void`。**額外發現**：golden `aoutarm9045.cpp:1369` 是 `return SearchTrayToPlace_Magazine();`——**回傳值真的有被用**。該呼叫點在 ported 樹尚未翻譯，這才是今天無害的真正原因 |
| **V8** | `LINK_GROUP:RESCAN` 有 66 個站點 | ✅ **成立** | `tests/CMakeLists.txt` 有 **67** 個 `LINK_GROUP` 出現，其中 1 個在註解（:281）→ **66 個真站點**。（`RESCAN` 字串出現 117 次是因為註解也提到） |
| **V9** | `rc.exe` 與 `windres` 都在、`.dfm` 資源閘門不卡 MFC | ✅ **成立** | 兩支都存在於 §1 列出的路徑 |
| **V10** | `Interface/TesterTCP.cpp` 的 `GetSiteNo`/`GetOrderOfContact` 阻塞已過時 | ✅ **成立** | `Public/MyProductionRecord.cpp:666` `GetSiteNo()`、`:671` `GetOrderOfContact()` 皆已存在 → `MIGRATION_ROADMAP.md:135` 的該項阻塞描述**已過時** |
| **V11** | `ckernel.cpp` 0% 已翻 | ✅ **成立** | ported 樹無 `ckernel.cpp`；golden 2,588 行（原宣稱 2,589，差 1 行、無實質影響） |
| **V12** | `TAPHeadLabel` 原始碼不存在、`TTMyTray256` 是動態建立所以 `.dfm` 直方圖抓不到 | ✅ **成立** | `elec\Component\` 只有 `apheadlabel.hpp`（10,393 bytes），無 `.pas`/`.cpp`；`aled.res` 2,440 bytes 存在。`TTMyTray256` 在 golden `cBinSel.cpp:129-132` 宣告、`:232-235` `new TTMyTray256(this)` |
| **V13** | facade 是測試套件 fixture | ⚠️ **成立但數字修正** | **15** 個 test 檔 deref facade（非 30），其中 10 個用 `fMain->`。`FormsFacade.h` 571 行 / `.cpp` 316 行。ctest 95 個 target 確認 |

**沒有任何一項核心決策因複驗而翻案**，但 V5/V7/V13 的數字與 C4/C5/C6/C7/C8 的歸屬必須照本檔更正版使用。

---

## §5 慣例（所有 W7 波次必遵）

沿用專案既有規矩，加上 W7 專屬的幾條。

### 既有規矩（不變）

1. 譯出檔**鏡射 golden 相對路徑+檔名**；附加檔（`tools/`、`forms/`、`ui/`、`tests/`）才用新結構。
2. 每個翻譯函式帶 banner 註解，引用**確切** golden `檔名:起-迄`。
3. golden 的 bug/quirk **逐位保留**，標 `GOLDEN BUG` / `GOLDEN QUIRK`，**絕不靜默修掉**。
4. 範圍外相依 → 明確標示的 no-op stand-in 或 `#if 0 // TODO(<WaveName>, <reason>)`，**絕不發明功能**。
5. 新 AI 註解格式：`AI(W906-<wave-name>) YYYYMMDD: <動作 + 原因>`。W7 波次代號見 §6（例：`AI(W906-W7-F0) 20260728: ...`）。**不要用 V899 的 `AI(ht9045-v899)` 格式**（KNOWLEDGE Gotcha 7）。
6. 譯出樹一律 **純 ASCII/UTF-8、無 BOM、無 U+FFFD**。golden 是 Big5——讀它的中文註解**必須用 cp950/big5 解碼**（python `open(encoding='cp950')`），**不可用 Read 工具直接搬中文**（KNOWLEDGE §37）。
7. 每個翻譯出來的東西都要有單元測試——**W7 不豁免**，見 §7 的分層驗證。

### W7 新增

8. **葉名 join key（D11）**：某個控制項在三個地方的名字必須機械可推導自 `.dfm` 的葉層 `object <Name>:`：
   - C++ facade 成員：`fXxx-><Name>`（原名照抄，同 golden）
   - 資源 ID 符號：`IDC_<FORMSTEM>_<NAME 全大寫>`
   - layout table 列：`{ IDC_..., ... }`
   `<FORMSTEM>` = `.dfm` 檔名去副檔名、大寫（例 `main.dfm` → `MAIN`、`cConfiguration.dfm` → `CCONFIGURATION`）。
9. **產生檔一律帶 DO-NOT-EDIT banner**，內含來源 golden `.dfm` 路徑與產生器版本。手改產生檔會被 idempotence 閘（G7）擋下。
10. **`Controls.h` 只放 stock VCL 控制項**；自製控制項核心各自開檔（D5）。
11. **任何在背景 agent 裡跑的程式都不得有能彈出視窗的路徑**（KNOWLEDGE Gotcha 9）。repo 內的 test target 已由 `tests/test_bootstrap.cpp`（commit `a9d2bc7`）+ 全部 95 個 `TIMEOUT 600` 保護；**你自己寫的獨立 probe 不在保護範圍**，必須自己呼叫 `SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX|SEM_NOOPENFILEERRORBOX)`。
12. **絕不用 MSVC Debug CRT build 或跑測試**；一律 `-DCMAKE_BUILD_TYPE=Release` + `/MD`。所有執行一律加 timeout。
13. **不得安裝/移除任何軟體或 VS Installer 元件。**
14. **FP 驗證腳本必須用 runtime/volatile 輸入**（§4-V5 的教訓），否則常數折疊會給假結論。

---

## §6 波次計畫（定序）

### 依賴總覽

```
   [立刻並行，互不重疊]
   W7-A0  vcl_compat.h include-order hoist  (1 檔)  ──┐
   W7-A2  SearchTrayToPlace_Magazine 修正   (27 檔)   │
   W7-B1a .dfm parser + IR      (全新檔)              │
   W7-B1b .rc + _ids.h 產生器   (全新檔, 需 B1a)      │
   W7-B1c layout table 產生器   (全新檔, 需 B1a)      │
   W7-C1  3 個控制項核心+測試   (全新檔)              │
   W7-C2  HDC renderer + 像素測試 (全新檔, 需 C1)     │
                                                      │
   W7-A1  MSVC 第二 oracle 啟用 (CMakeLists 頂部) ◄───┘ (A0 後)
        ↓
   ██ W7-F0  FormsFacade 重構【短凍結，全樹序列】██
        ↓
   ┌────────────┬──────────────┬─────────────┬──────────────┐
   W7-F1        W7-F2          W7-C4         W7-L*
   牆② 解鎖    seam 退場       核心接回       邏輯長尾
                +SckArt 4方整併  facade         (asendic×7 / ckernel /
                                               MainCalcCore / …)
        ↓
   W7-B1d  全語料跑 + 閘門分流 (需 B1b/B1c)
        ↓
   ══════════ 以下卡使用者安裝 MFC 元件 ══════════
   W7-U0 binder 基礎 → W7-C5 三個 CWnd shell → W7-U1..Un 逐表單
```

---

### W7-A0 — `vcl_compat.h` include-order 加固【**第一個做，1 檔**】

- **為什麼第一**：§4-V6 親自重現——`windows.h` 先進來就編不過。**每一個** MFC TU 都會先 `#include <afxwin.h>`（進而 `windows.h`），所以這一條擋住的不是一個檔，是**整個 MFC 波**。順帶修好 MSVC 下的 `Public/WinSocketErrorCode.cpp`。
- **檔案**：`PORTED\vclcompat\vcl_compat.h`（僅此一檔）
- **做什麼**：把 `#if defined(_WIN32)` / `#include <winsock2.h>` / `#include <windows.h>` / `#undef DeleteFile` / `#undef CopyFile` / `#undef MoveFile` / `#endif` 整塊，從現在的 **:142** 位置**上移到 :98 的 `#include "vclcompat/AnsiString.h"` 之前**。新增 `<winsock2.h>`（它 define `_WINSOCKAPI_`，擋掉 `windows.h` 拖進 winsock v1）。保留現有那段策略註解，並**把其中「safe whether or not windows.h happened to be included earlier」這句改寫成正確描述**（見 §4-V6：原句是錯的）。
- **AI tag**：`AI(W906-W7-A0) 20260728: hoist winsock2+windows.h+A/W undefs above SysUtils.h so the umbrella is include-order-robust (was: broke whenever any TU saw windows.h first -- blocks Public/WinSocketErrorCode.cpp under MSVC and every future MFC TU).`
- **驗證閘**：
  1. `g++ -std=c++14 -fsyntax-only` 對這兩個順序都要過（可直接抄 §4-V6 的兩個 3 行 TU）：`windows.h` 先 / `vcl_compat.h` 先。
  2. `g++ -fsyntax-only` 對 `Public/WinSocketErrorCode.cpp` 與 `acarry.cpp` 都要乾淨。
  3. `build_resume_verify_20260727` full build + `ctest --timeout 300` = **91/95**、同 4 個既有失敗。
  4. 全樹 mojibake/BOM 掃描 = 0。

---

### W7-A1 — MSVC 第二 oracle 啟用（A0 之後，F0 之前）

- **檔案**：`PORTED\CMakeLists.txt`（**頂部**，`project()` 之後、任何 `add_library` 之前）、`PORTED\tests\test_bootstrap.cpp`、新 `PORTED\scripts\build_msvc.bat`
- **做什麼**：
  1. 加 `if(MSVC)` 區塊：
     - `CMAKE_{C,CXX}_LINK_GROUP_USING_RESCAN_SUPPORTED TRUE` + `CMAKE_{C,CXX}_LINK_GROUP_USING_RESCAN "" ""`。**沒有這 4 行 MSVC 連 configure 都會硬失敗**（`Feature 'RESCAN' ... is not supported`），因為 `tests/CMakeLists.txt` 有 **66 個真站點**（§4-V8）。MSVC 的 `link.exe` 本來就會重掃 archive，所以這個 group 在那邊本來就是 no-op。**66 個站點一行都不用改。**
     - `add_compile_options(/utf-8 /W3)`：源碼含 UTF-8 非 ASCII，MSVC 預設用 ACP（本機 cp950）解會 C4819 並重編窄字面值。
     - `add_compile_definitions(_HAS_STD_BYTE=0 _CRT_SECURE_NO_WARNINGS)`：`cmydef.h:127`/`MyLaneIo.h:76-78,98`/`Motor/HTMotor.h:92` 用 `::byte` 而 scope 內有 `using namespace std;`（`MachineDefine.h:30,67`、`database.h:39`）→ MSVC C++17 的 `std::byte` 造成 C2872 歧義；g++ 6.3 無 `std::byte` 所以 MinGW 從沒看到。
     - **FP 保真**：`if(CMAKE_SIZEOF_VOID_P EQUAL 4) add_compile_options(/arch:IA32) endif()`。
  2. `tests/test_bootstrap.cpp` 加 `#if defined(_MSC_VER) && defined(_M_IX86)` 的 `_controlfp_s(&old, _PC_64, _MCW_PC)` static initializer。**`/arch:IA32` 單獨不夠、`_controlfp` 單獨也不夠，必須兩個一起**（§4-V5）。
  3. `scripts\build_msvc.bat`：`vcvarsall.bat x86 -vcvars_ver=14.44` → `cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl -DHT9045_UI=OFF` → build → `ctest --timeout 600 -j4`。Ninja 在 `...\BuildTools\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe`。
     **`-DCMAKE_BUILD_TYPE=Release` 是強制的，不可省（D10 + §5-12）。**
- **驗證閘**：MinGW 側維持 **91/95**（`if(MSVC)` 對 GNU 是 no-op，不該有任何變化）；MSVC 側先取得 baseline 並記錄差異清單，**MSVC 差異不擋此波交付**，交給 A3 分流。
- **注意**：既有每個 target 的 warning 區塊都已被 `if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")` 包住，所以 13 個 library 定義**一行都不用動**。

---

### W7-A2 — `SearchTrayToPlace_Magazine` 保真修正（可與 A0/A1 完全平行）

- **檔案**：`PORTED\aoutarm_shims.cpp:95` + 14 個宣告成 `void` 的 `aoutarm9045_*.cpp`（清單用 `grep -rn "SearchTrayToPlace_Magazine" --include=*.cpp .` 取，注意 `int` 版寫成 `int  `（兩空格），單空格 grep 會漏）
- **做什麼**：全部統一成 golden 的 `int`（`aoutarm9045.h:63`），定義回傳 offline-safe 的 `0`。
- **這是保真修正，不是 build 修正**：golden `aoutarm9045.cpp:1369` 真的寫 `return SearchTrayToPlace_Magazine();`。今天無害只是因為那個呼叫點還沒翻（§4-V7）。MSVC 會直接 `LNK2019` 拒絕連結。
- **AI tag**：`AI(W906-W7-A2) 20260728: unify SearchTrayToPlace_Magazine to golden's int return (aoutarm9045.h:63) across 14 void decls + the shim definition -- ported tree had a 14-void/13-int ODR split that MinGW links silently; golden aoutarm9045.cpp:1369 genuinely consumes the return value.`
- **驗證閘**：MinGW 91/95；`grep -ic resolving` build log = 0；MSVC 側該 `LNK2019` 消失。

---

### W7-A3 — MSVC-only 差異分流（A1 之後，可與 F0 並行，唯讀分析為主）

- 上一輪（被中止的那場）在 **Debug CRT + `/RTC1`** 下看到 5 個 MSVC-only 失敗：`common`(234s)、`CanBus_cMyNUDN1`、`MyPLCModbus`、`BarCodeBottom2DID`（**SegFault**）、`uHGemEquipment`（沒跑完＝就是那個 assert 對話框）。
- **先在 Release 下重跑**——部分可能是 `/RTC1` 假象。活下來的就是真的第二-oracle 收穫，走正常 golden 對照流程處理。
- 已知**好消息**：4 個既有環境漂移失敗（`config_db`/`IniFiles`/`ini_helpers`/`config_loaders`）在 MSVC 下**完全一樣重現**，證明 harness 本身與編譯器無關。
- **驗證閘**：目標是把 MSVC 拉到與 MinGW **同樣的 91/95**。在達成之前，**MSVC 閘門是諮詢性的、不是強制的**——一個常態紅的 oracle 會被忽略，那比沒有更糟。

---

### ██ W7-F0 — FormsFacade 重構【全樹短凍結，序列，其他寫入波必須停】██

**這是唯一需要凍結的波。** 它是純重構（零行為變更），所以閘門異常清晰、凍結時間短。

- **前置**：A0 已落地（per-form header 之後會被 MFC TU include）、A1 已落地（避免與 F0 搶 `CMakeLists.txt`）。
- **凍結範圍**：任何會寫 `FormsFacade.h/.cpp`、`vclcompat/Controls.h`、根 `CMakeLists.txt` 的波次。**唯讀 recon、B1/C1/C2（純新檔）、A2 不受凍結影響，繼續跑。**

**F0-a 統一 widget stand-in**：把 `vclcompat/Controls.h` 從現在的 6 個 SECS-cast-only 型別擴成 **~20 個 stock VCL 控制項型別**，覆蓋量測到的 top-20 nested property（`Text` 2193 / `Caption` 834 / `Cells` 446 / `Checked` 415 / `Visible` 374 / `Enabled` 192 / `Glyph` 173 / `Lines` 149 / `SetCellNumber` 116 / `Items` 111 / `Color` 96 / `Click` 95 / `ItemIndex` 94 / `Clear` 85 / `SetCellColorIndex` 77 / `Value` 72 / `Strings` 62 / `AddTextWithDateTime` 58 / `Add` 57 / `XItem`+`YItem` 43+43；top-10 蓋 75.9%、top-20 蓋 87.1%）。每型：
  - **public 資料成員，不是 accessor**（這樣 `fMain->cbSetupFileName->Text` 逐字可編、零呼叫點修改）
  - **`: public vclcompat::TObject` + virtual dtor**（D4／§4-V2）
  - 惰性綁定槽 `void* hCtl;` + no-op 的 `PullFromControl()` / `PushToControl()`
  - `Cells` 重用既有 `vclcompat::TStringGrid`、`Lines`/`Items`/`Strings` 重用既有 `vclcompat::TStringList`（兩者都已有真語意，不要重造）
  - **`SetCellNumber`/`SetCellColorIndex`/`XItem`/`YItem` 屬 `TTMyTray` 家族，不放這裡**——它們是 W7-C1 的 `TrayCore`（D5）。`Controls.h` 只放 stock。

**F0-b overload 守衛**：加一個 compile-only 測試，對每個控制項型別 `static_assert` 它會選到 `SetSVDataPointer`/`SetECDataPointer` 的 **`TObject*` 多載（`SecsSvEcRegistration.h:149`）而非 `void*` 多載（`:133`）**。這條在 1,740 個 SV/EC 註冊落地之前必須先就位，否則會在中途靜默引爆。

**F0-c FormsFacade 別名化**：`FormsFacade.h` 裡每個 bespoke widget struct 改成 `typedef`/`using` 指向統一型別——`TfLotInfoEdit`(:434)→`TCustomEdit`、`TfMainPanel`(:225)/`TfSortCTPanel`(:409)/`TfLotInfoLabel`(:433)→`TPanel`/`TLabel`、`TfMainCheckBox`(:184)→`TCheckBox`、`TfLotInfoRunMode`(:432)→`TComboBox`、`TfLedValue`(:215)、`TfMainTrayPanel`(:152)、`TfLotInfoStatusLabel`(:439)、`TfLotInfoPanel`(:445)、`TfMainFont`(:189)、`TfMainMemo`/`TfMainMemoLines`(:202-203)、`TfMainGrid`(:185)、`TfMainAutoCleanGrid`(:196)、`TfMainPageControl`(:204)、`TfMainSpeedButton`(:205)、`TfMainSiteMapLog`(:136)。
  - **開工前先 re-grep 確認「全樹對這些型別的使用都是透過指標」**（零 by-value、零 aggregate-init）。這是別名化 source-compatible 的前提，加 vtable 才安全。
  - `TfMainAutoCleanGrid` / `TfLedValue` 是 `TTMyTray` / `TALed` 的臨時替身——**F0 只把它們別名到暫時型別，真正接回 `TrayCore`/`LedCore` 是 W7-C4**（避免 F0 依賴 C1 完工）。
  - 順手修 `FormsFacade.h:385` 的錯誤註解：`tmyAutoClean` 的 golden 型別**不是** `THeatTable*`（全樹無此類別），是 `TTMyTray*`，同它的兄弟 `mtPlate2`（`main.h:164`，`FormsFacade.h:389-395` 那邊標對了）。

**F0-d 下沉 + 拆檔【真正解牆② 的一步】**：
  - 新 `add_library(ht9045_forms STATIC ...)`，**只依賴 `vclcompat`**。
  - `FormsFacade.cpp` 從 `ht9045_sm` 的 source list（`CMakeLists.txt:731`，屬 `:728` 的 `add_library`）**移出**到新 library。
  - `FormsFacade.h/.cpp` 拆成 `forms/fMain.{h,cpp}`、`forms/fLotInfo`、`forms/fSCKART`、`forms/fSortCT`、`forms/fNote`、`forms/fAGV`、`forms/fOffSet`、`forms/fCleaning`、`forms/fShowMessage`。
  - **保留一個薄的 `FormsFacade.h` 把它們全部 `#include`**，這樣既有的所有 `#include "FormsFacade.h"` 一行都不用改。
  - 把 `ht9045_forms` 加進 `ht9045_secsgem`(:588)、`ht9045_sm`(:1204-1208) 及其他消費端的 link line。**確認循環消失。**

**F0-e virtual 化**：每個 `TfXxx` 加 `virtual ~TfXxx()`，方法面標 `virtual`，**現有 no-op body 原封不動留著當永久的離線實作**。在 `forms/fMain.h` 檔頭寫死契約：
> methods = virtual，由 MFC binder 覆寫；data members = 具體儲存，由 DDX 保持新鮮；**永遠不要把 data member 改成 accessor**。

- **驗證閘（F0 是純重構，閘門要比平常嚴）**：
  1. `build_resume_verify_20260727` fresh build + `ctest --timeout 300` = **91/95**、同 4 個既有失敗。
  2. **每個 test target 的 assertion 數 build 前後逐一 diff，必須完全相同**。這是本專案自己在 `uHGemHT9045` Bucket 0 swap-back（commit `ee85c32`）建立的「零行為變更」證明法，能抓到「測試被靜默閹掉」——單看 pass/fail 數抓不到。
  3. `grep -ic resolving` = 0（`__fastcall` 配對紀律，ROADMAP:132）。
  4. 全樹 mojibake/BOM = 0。**F0 搬的是已翻譯的 ASCII/UTF-8 文字，不是 golden Big5，所以曝險比一般波低——但掃描照跑，且拆檔期間絕不重讀 golden。**
  5. 通過後才解除凍結。

---

### W7-F1 — 牆② 解鎖（F0 後，2 agent 可平行）

- **前置**：F0-d（沒有 layering 修正，補再多 stub 都不會解鎖——§4-V1）。
- **做什麼**：往 `forms/fMain.h` 補 `SECSGEM/uHGemHT9045.cpp` 需要但 facade 缺的成員。golden 該檔**恰好觸及 28 個相異 `fMain` 成員**（其中 25 個是 `cbSetupFileName->Text`；18 個是方法呼叫）。缺的約 19 個：
  - 方法：`cbSetupFileNameChange`、`Clarn_Data`、`BtnPauseClick`、`LoadRunModePicture`、`CanChangeSite`、`BtnTrayEndClick`、`UpdateMainOperateMode`、`LoadStartModePicture`、`LookForFile`、`ChangeTesterConnect`、`SetTemp`
  - 資料：`tSiteOnOff`；widget：`edSoakTime`
  - **已有可重用**：`Start`、`ShowTestHeadComp`、`CleanOut`、`LoadTestModePicture`、`SetStartModeData`、`BtnOneCycleClick`、`Reset`、`cbSetupFileName`、`edWorkTemperBase`
- 平行支線：清點 `uHGemHT9045.cpp` 另外觸及的 `fLotInfo`(38)/`fSCKART`(31)/`fNote`(19)/`fSetup`(18) 面。
- **注意**：`uHGemHT9045_SV.cpp`/`_EC.cpp` 主要是**別的表單**——`fBinSel` 147、`fGroundMan` 28、`fSmartDiagnostic` 20——這三個目前**沒有任何 facade 落腳處**，是 F1 之後、Bucket 1-5 之前要另外處理的。
- **牆③ 仍在**（ROADMAP:133）：`GetAlarmCodeList`/`ProcessLotInfo`(BarcodeXML.cpp) 全樹未翻、`DoAutoRetest` 被 `csystem.cpp` 內 TU-local no-op 巨集遮蔽、`SaveAllFile` 已宣告無定義。**F1 只解牆②。**
- **驗證閘**：91/95；`uHGemHT9045` 的 22 個 override 從「無法引用 fMain」變成「可以開始逐個翻」；新增 stub 一律走既有的 `W906_..._Sim` 可餵資料 seam 慣例（`FormsFacade.h:361` 是範本），不要裸 no-op，否則缺口在測試裡看不見。

---

### W7-F2 — TU-local seam 退場 + SckArt **4 方** state 整併（F0 後，不需凍結）

- **機械部分（可平行）**：把 ~15 個 TU-local 重複型別別名到統一型別——`atester.cpp:1594` `W7T1_TfMainTorqueSeam` + `W7T1_TCheckSeam/TEditSeam/TLabelSeam`；`atester_32Site.cpp:262-265` `W5_32S_*`；`aTester_Front.cpp:1046-1048` `W64B_*`；`aTester_Rear.cpp:1015` `W64bT2_*`；`csystem.cpp:1048-1209` `W7C1_*` 與 `:2320-2380` `W7C2_*`；`Automation/auto9045.cpp` `W5FA_*`；`Interface/TesterTCP_Socket.h` `TesterTCPSocket_Label/Panel/Combo`；`atester_shims.h` `TfObserverLabel`；`acatchtray_shims.h` `TListTrayIDShim`；`Automation/automation.h` `TfAutomationTextField/TfAutomationCheckFlag`。
- **分析部分（獨立子任務，不可機械別名）**：**SckArt 是 4 份不是 3 份**（§3-C4）——`Automation/SCK_ART.h` `SckArtState`、`Automation/SCK_ART_Remainder.h` `SckArtRemainderState`、`csystem.cpp:1189` `W7C1_TfSCKARTSeam`、`csystem.cpp:2332` `W7C2_TfSCKARTSeam`。重疊欄位：`dCurrYield`/`iFTRTCount`/`iNeedRT`/`iCurrent93KARTStep`/`iCurrentFlexARTStep`/`iInputCount`/`iManualRejectCnt`/`iTesterType`/`sLOTSTATUS`/`iCurrentStatus`/`iSCKART_RTUnitCount`/`lSCKARTBinCT[256]`。
  - **這 4 份可能已經行為性分歧**——合併可能改變行為，不只是改結構。
  - 作法：**逐欄位對 golden `SCK_ART.h` 做 diff 才合併**；任何目前值非預設的欄位補一條回歸測試。**若發現分歧，那是 golden-comparison 發現，必須回報，不可靜默調和。**
- **驗證閘**：91/95 + assertion 數 diff（同 F0）；`CMakeLists.txt:1028` 那句「a future TfSCKART/FormsFacade unification wave」的 TODO 可以拿掉。

---

### W7-B1 — `.dfm` → IR → 4 產物 管線【**立刻開工，完全不卡 MFC，也不卡 F0**】

全部是**新檔**，與任何既有檔零重疊，所以在 F0 凍結期間照跑。

**產物形狀**（每個 golden `.dfm` 一組，鏡射相對路徑）：

```
golden <path>/<Form>.dfm
  → PORTED <path>/<Form>.dfm.ir.json      標準 IR，diff/review 的表面
  → PORTED <path>/<Form>.rc               純 ASCII DIALOGEX
  → PORTED <path>/<Form>_ids.h            控制項 + dialog ID
  → PORTED <path>/<Form>_layout.gen.{h,cpp}   ★ 像素精確 C++ 表 = 幾何權威
  → PORTED <path>/<Form>_res/*.bmp|.ico   抽出的 554 個 blob / 3.70 MB
  → PORTED <path>/<Form>_events.gen.json  7,280 個事件綁定 = 下一波的工作佇列
```

#### W7-B1a — parser + IR + 閘 G0-G4

- **檔案**：新 `PORTED\tools\dfm2rc\dfm_parse.py`
- **起點**：把 scratchpad 的已驗證原型 `b1_dfmparse2.py` 升成正式檔（**但要 spot-check，不可照單全收**）。它已處理完整真實文法：`object`/`inline`/`inherited` + 可選 `[index]`；巢狀 `end`；點號子屬性（`Font.Charset`）；binary blob `{ hex }`；括號清單 `( ... )` 且要 paren-safe 地跳過字串內括號；角括號集合 `< item ... end >` 含空的 `<>`；方括號集合可跨行；**值從下一行才開始**（`Caption =` 然後兩行續行——光這一項就是多數 naive parser 失敗的原因，`cConfiguration.dfm:726` 加該檔另外 134 處）；尾隨 `+` 串接；`#NNN` 十進位字元碼與引號段落串接（`ARMS.dfm:427` `Font.Name = '@'#26032#32048#26126#39636`）；`''` 逃逸單引號（`cBinSel.dfm:413`）。以 **cp950**（fallback big5）解碼。
- **驗收**：133 檔 **0 行未解析、0 個 object/end 不平衡、22,627 個控制項節點**（已複驗，§4-V4）。
- **IR 每節點欄位**：點號路徑、name、VCL class、parent 路徑、sibling index、depth、逐字 `{Left,Top,Width,Height}`（無則 null）、`TabOrder`、`Visible`、`Align`、`Anchors`、所有 `Font.*`、所有 `*Color`、所有 `On*` handler、blob 參照、自製元件屬性（`LEDStyle`/`Alias`/`XItem`/`YItem`/`True*`/`False*`），以及分類旗標：`kind ∈ {LEAF, SUBDLG, TABHOST, NONVISUAL}`、`is_graphic_control`（TLabel/TSpeedButton/TShape/TImage/TBevel + 自製 LED，共 **6,760** 個在 VCL 裡**沒有 HWND**）、`synthesized:true`。
- **閘門**：G0 parser 完整性、G1 census 雙射（IR↔`.dfm` 一一對應，只允許兩種不對稱：331 個 NONVISUAL 排除、`synthesized` 節點）、G2 幾何逐字（IR px == `.dfm` 整數，**精確相等**）、G3 sibling 順序 == `.dfm` 宣告序、G4 TabOrder 是 0..n-1 排列（**38 個 group 不是，列成具名例外**）。
- **全部只需要 Python，零編譯器、零 MFC。**

#### W7-B1b — `.rc` + `_ids.h` 產生器 + 閘 G5/G6

- **檔案**：新 `tools\dfm2rc\emit_rc.py`、`tools\dfm2rc\classmap.py`、`tools\dfm2rc\res_parse.py`
- **class map（72 個 class 全表，摘要）**：
  - Static：`TLabel`(4978)→`"Static"`+`SS_LEFTNOWORDWRAP`；`TBevel`(8)/`TSplitter`(1)→`SS_ETCHEDFRAME`；`TImage`(103)/`TEBW8Image`(4)→`SS_BITMAP|SS_REALSIZECONTROL`
  - Edit：`TEdit`(3717)/`TLabeledEdit`(198，另合成一個 `LTEXT` 給 `EditLabel`)→`"Edit"`+`ES_AUTOHSCROLL|WS_BORDER`；`TMemo`(156)/`TMyMemo`(2)→`ES_MULTILINE|ES_AUTOVSCROLL|WS_VSCROLL`；`TRichEdit`(5)→`"RICHEDIT50W"`
  - Button：`TButton`(725)→`BS_PUSHBUTTON`；`TBitBtn`(110)→`BS_PUSHBUTTON|BS_BITMAP`；**`TSpeedButton`(1609)→`BS_OWNERDRAW`**（它是 `TGraphicControl` + `Glyph` + `GroupIndex` 閂鎖：511 個有 `Glyph.Data`、262 個有 `GroupIndex`，owner-draw 是**必要**不是選配）；`TCheckBox`(1553)→`BS_AUTOCHECKBOX`；`TRadioButton`(78)→`BS_AUTORADIOBUTTON`
  - List：`TComboBox`(372)/`TDriveComboBox`(1)→`CBS_DROPDOWNLIST`；`TListBox`(19)/`TFileListBox`(13)/`TDirectoryListBox`(7)→`"ListBox"`；`TCheckListBox`(8)→`LBS_OWNERDRAWFIXED|LBS_HASSTRINGS`
  - Common control：**一律用字面 class name 字串，絕不用 `WC_*`/`TRACKBAR_CLASS` 等巨集**（見 §9-R2）——`"SysTabControl32"`、`"msctls_updown32"`、`"msctls_trackbar32"`、`"msctls_progress32"`、`"SysDateTimePick32"`、`"SysMonthCal32"`、`"SysTreeView32"`、`"msctls_statusbar32"`、`"ScrollBar"`
  - SUBDLG（容器）：`TPanel`(2991，其中 1177 容器 / 1814 葉)、`TGroupBox`(1248)、`TScrollBox`(51)、`TTabSheet`(691)、`TRadioGroup`(336)、`TframeProdInfo`(1)。TABHOST：`TPageControl`(143)
  - **NONVISUAL，完全不進 `.rc`**（331 個；它們有 Left/Top 設計期圖示座標但**沒有 Width/Height**，checker 必須知道這份清單否則會誤報 dropped）：`TTimer`(122)、`TClientSocket`(48)、`TComm`(40)、`TServerSocket`(9)、`TOpenDialog`(9)、`TSaveDialog`(6)、`TSavePictureDialog`(1)、`TPopupMenu`(5)、`TMenuItem`(16)、`TActionList`(1)、`TAction`(1)、`TTable`(6)、`TQuery`(1)、`TDataSource`(1)、`TXMLDocument`(1)、`TIdHTTP`(3)、`THTTPRIO`(2)、`TNMUDP`(3)、`TNMFTP`(1)、`TLineSeries`(50)、`TFastLineSeries`(2)、`TBarSeries`(1)
  - **無乾淨對應 → 註冊式自訂 class（見 §9-R5）**：7 個自製 VCL 元件、`TStringGrid`(119)+`TDBGrid`(1)、`TShape`(62)、`TChart`(5)+53 個 series、`TCppWebBrowser`(2)、`TMediaPlayer`(1)、`TEBW8ROI`(2)、`TEasyMain`(1)、`TECameraPicolo`(1)
- **DIALOGEX 規則（D8）**：root = `DS_SETFONT|WS_POPUP|WS_CAPTION|WS_SYSMENU|WS_CLIPCHILDREN` + `WS_EX_CONTROLPARENT`；容器 = `DS_SETFONT|DS_CONTROL|WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN` + `WS_EX_CONTROLPARENT`（**本地 `#define DS_CONTROL 0x0400L`，不要拉 `afxres.h`**——見 G10）。`TGroupBox` 另在自己的 template 內 `(0,0,w,h)` 放一個 `GROUPBOX` 畫框。`TPageControl` 出 `SysTabControl32` + 每個 `TTabSheet` 一個 child dialog（`TTabSheet` 在 `.dfm` 裡**沒有幾何**，691 個都要從父的 display rect 合成）。
- **絕不在父 template 裡替容器放 placeholder Static**——那會多燒 5,594 個 HWND（§9-R3）。容器的位置由 layout table 定位。
- **絕不對無子容器 emit `LTEXT "", -1` 之類的填充**——那正是 checker 首次執行就抓到的自造 bug（cShowBinSelect 37 筆、main 54 筆 `DUP_ID_IN_RES(4294967295)`）。
- **字串**：29,094 個字串屬性中只有 887 個非 ASCII（其中 355 個是 `Font.Name`，根本不進 `.rc`）。**每個 caption 一律寫成 `L"..."` 寬字面值**，>126 的碼位用 `\xHHHH`。**理由見 §9-R4：`rc.exe` 的 `\xHH` 在 ANSI 字面值裡是逐 byte 且會靜默毀字。** 一律用 `L"..."`（不只非 ASCII 的那些），保持單一路徑。
- **ID（D11）**：`IDC_<FORMSTEM>_<CONTROLNAME>`，數字從 `0x0400` 起、**對該表單整棵樹（含所有 child dialog）循序配發**（一個表單 + 它的 child dialog 是同一個 `WM_COMMAND` routing domain）。單表單最大 = `iosetview` 3,498，遠在 16-bit 限度內。Dialog template ID 是另一個 namespace：`IDD_<FORMSTEM>[_<CONTAINERPATH>]` 從 `0x2000`。**控制項符號不要用點號路徑**（原型這樣做，`iosetview_ids.h` 炸到 384 KB）。
- **閘門**：G5 `.rc` 編譯（**`rc.exe` 是權威，`windres` 是 MinGW CI 跑者，兩者都在就都要跑**——見 §9-R2）；G6 **`.res` 二進位回讀**（用純 Python 解 RES container + `DLGTEMPLATEEX` + `DLGITEMTEMPLATEEX`，逐項 diff 回 IR：template 數、每個 ID 恰好出現一次、class 字串、caption 逐字）。**G6 是不需 MFC 的最強 oracle，且它在原型階段就真的抓到過自造 bug。**

#### W7-B1c — layout / events / colour / font 表 + 閘 G9（與 B1b 平行）

- **檔案**：新 `tools\dfm2rc\emit_layout.py`
- **產物**：`<Form>_layout.gen.{h,cpp}` = 一個 static 陣列，欄位 `{id, parent_dialog_id, sibling_index, left, top, width, height, taborder, visible, is_graphic_control, wclass, font_face, font_height_px, font_style_bits, fg_color, bg_color, align_mode, anchors_mask}`，**幾何用 `.dfm` 整數逐字**。加 `<Form>_events.gen.json`（7,280 個 `(control, event, handler)` 三元組）。
- **自製控制項的 component-specific props（`Alias`/`LEDStyle`/`XItem`/`YItem`/`True*Color`/`Out*`/`In*`）也放這張表**——這是 D12 對 `custom-controls` 顧慮的正式答覆。
- **`.dfm` 的 Width/Height 直接照抄，不要「修正」**：看起來 `aled.pas:111-155` 的 `CreateLedBitmap` 會用 `LEDStyle` 覆寫 Width/Height，而 `LEDStyle` 在所有 1,840 個實例裡都**晚於** Width/Height 被 stream 進來——像是 `.dfm` 幾何會被丟掉。**實測不會**：`ChangeStyle`(`aled.pas:213-220`) 在 style 未變時是 no-op，而 `TMyLed`/`TMyLedLane` 的 ctor 已先設 `LEDSqLarge`，所以那 312 個 setter 從不觸發；其餘 1,528 個的 `.dfm` 尺寸本來就等於 style 強制尺寸（Delphi IDE 設計期就寫回去了）。**量測結果：1,840 個裡 0 個被丟棄。** 後來的 agent 若「好心修正」這點會**改掉 262 個 LED 的尺寸**。（此條建議進 KNOWLEDGE，見 §10）
- **閘 G9**：`<Form>_layout.gen.cpp` 用**既有的 MinGW C++14 CMake** 編過（`-Wall`），加一個 ctest target 拿獨立從 `.dfm` 導出的 fixture 對表。**這就是「每個翻譯物都要有測試」在 UI 層的存活方式**：UI 的事實內容（幾何/ID/階層/字型/顏色/tab order）被降維成純 C++ 資料，headless 測試就能精確檢查。

#### W7-B1d — 全語料跑 + 閘門分流 + CMake 接線（需 B1b + B1c）

- 把 G0-G10 全部接進 `PORTED\tests\CMakeLists.txt`（**append-only，MERGE-AT-INTEGRATE，見 §8**）：`dfm2rc_fidelity`（全 133 表單跑 checker）、`dfm2rc_idempotent`（重產到 `${CMAKE_BINARY_DIR}/dfm2rc_regen` 後 byte 比對）、`dfm2rc_rc_compiles`（`rc.exe` + 有的話 `windres`）、`test_DfmLayout`（C++ target）。**每一個都要明確 `TIMEOUT`，並繼承既有的 `ht9045_test_bootstrap` INTERFACE library。**
- **預期殘留，照使用者「遇到真阻塞就 SKIP 並記錄」的指示寫進 `docs/W7-UI-SKIPPED.md`，不要卡住**：9 個無乾淨對應的 class、802 個合成的 `TRadioGroup` radio button、38 個 TabOrder 非排列的 parent、3,338 對幾何重疊的 sibling（z-order vs tab-order 衝突）、91 個負座標、22 個超大座標、554 個 blob 的抽取與去重（很多 `Glyph.Data` 跨表單重複，值得做 content-hash 去重）。
- **合成規則只准兩條，且要能被 checker 精確預測**（否則「不得發明控制項」這條就破功）：(1) `TRadioGroup` → 從 `Items.Strings` 生 N 個 radio button，ID `IDC_<FORM>_<GROUP>_ITEM<n>`；(2) `TTabSheet` → 幾何從父 `TPageControl` 的 display rect 合成。checker 必須 assert 合成集合**恰好等於**這兩條規則的預測，其他任何發明照樣 FAIL。

**全語料規模（供評估 review 負擔）**：5,594 個 `DIALOGEX` template、22,296 個 visual 控制項、331 個 NONVISUAL 排除、~532 個新檔、約 5-8 MB 產生文字 + 3.7 MB 抽出二進位。**Reviewer 要讀的是產生器與 checker，不是產出的 `.rc`**——那正是 G1-G8 存在的目的。

---

### W7-C — 自製控制項（立刻開工，C1/C2 完全不卡 MFC 也不卡 F0）

**核心洞見（已複驗，§4-V3）：2,853 個實例塌縮成 3 個家族、約 2,000 行原始碼、約 30 個成員的執行期 API。** 因為 `TMyLed`/`TMyLedLane` **零繪圖碼**（只是 `TALed` + IO 位址 metadata），`TBtnPanel`/`TBtnPanelLane` **完全不自繪**（只設 stock `TPanel` 的 `Color`/`Font->Color`/`BevelInner`/`BevelOuter`）。

| 家族 | 新檔 | 支撐的 .dfm 實例 | golden 原始碼 |
|------|------|-----------------|--------------|
| Tray | `vclcompat/TrayCore.{h,cpp}` | `TTMyTray` 246（+ `TTMyTray256` **0 個 .dfm 實例但 7 個動態指標**） | `elec\myvcl\HTray.{h,cpp}` 517+114、`HTray256.{h,cpp}` 389+92 |
| Button | `vclcompat/BtnPanelCore.{h,cpp}` | `TBtnPanel` 56 + `TBtnPanelLane` 701 = 757 | `elec\myvcl\butPa1.{h,cpp}` 217+67、`BtnPanelLane.{h,cpp}` 178+78 |
| LED | `vclcompat/LedCore.{h,cpp}` | `TALed` 530 + `TMyLed` 96 + `TMyLedLane` 1214 = 1840 | `elec\Component\aled.pas` 306 行 Delphi + `elec\myvcl\MyLed.{h,cpp}` 126+39、`MyLedLane.{h,cpp}` 136+50 |

**完整執行期 API（量測自 golden `.cpp/.h`，這就是全部，其餘都是繪圖）**：
- Tray：`SetCellNumber` 873、`SetCellColorIndex` 538、`SetColorMap` 244、`GetCellText` 17、`ConvertIndexCells` 16、`ClearCell`/`GetCellData`/`SaveCellTextToFile` 各 2；`->XItem` 114、`->YItem` 86 = **1,655 個呼叫點**
- Button：`->Down` **671**、`OutPort` 39、`OutBit` 38、`OutIP` 21、`OutType` 20、`OutRing` 17、`FalseFontColor` 31、`TrueFontColor` 17
- LED：`->Value` 387、`->Alias` 44、`->Interval` 19、`->Blink` 15、`->LEDStyle` 5、`InPort` 18、`InBit` 14、`InType` 10、`InRing` 6、`InIP` 6
- 共用：`->TrueColor` 72、`->FalseColor` 66、`->ISABase` 60
- 注意 `ChangeValue`/`SetToTrueColor`/`ChangeBlink` **從未被具名呼叫**——互動全走 property。

#### W7-C1 — 3 個 framework-free 核心 + 單元測試（3 agent 可完全平行，零檔案重疊）

- 每個方法帶 golden `檔名:起-迄` banner。新增 `tests/test_traycore.cpp`/`test_btnpanelcore.cpp`/`test_ledcore.cpp`（95 → 98 個 ctest target，`TIMEOUT 600`）。
- **`TrayCore` 已有可用起點**：`...\scratchpad\b2\traycore.h`/`traycore.cpp`/`t.cpp`（`g++ -std=c++14 -Wall -Wextra` 零警告、30/30 通過）。**搬進樹裡時把 `std::string` 換成 `vclcompat::AnsiString`**，並補齊完整公開面（`SetBlockYItem`/`SetXBlockWidth`/`SetYBlockWidth`/`SetLineWidth`/`SetEdgeWidth`/`SetDirectWidth`/`SetTrayDirect`/`SetShowFont`/`SetShape`/`SetColorMap`/`SetFrameColor`/`SetTrayColor`）。**照本專案規矩，搬進來時仍要對 golden 抽查，不可因為它自稱 30/30 就照單全收。**
- **`TTMyTray256` 併進 C1**（同一設計加寬到 `MAX_ITEM 300`、無 block/visibility 支援）。它**沒有任何 `.dfm` 實例**所以直方圖永遠抓不到，但 golden `cBinSel.cpp:129-132`/`:232-235` 與 `uYieldMonitoring.{cpp,h}` 動態 `new` 它（§4-V12）。**通則教訓：`.dfm` 直方圖找不到動態建立的控制項，必須另外 grep 型別指標宣告交叉比對。**

**必須逐位保留、且要有「修好就會失敗」的回歸測試的 6 個 golden bug**（前 3 個 scratchpad 原型已有 assertion）：
1. `HTray.cpp:133-136` `SetYItem` 重設的是 `bXVisible` 不是 `bYVisible`——同樣的複製貼上錯誤在 `SetBlockXItem`:154-157 與 `SetBlockYItem`:176-179，導致**沒有任何 setter 會重設 `bYVisible`，但 `FYVisibleItem` 會被重設**，狀態不一致。
2. `HTray.cpp:437-454` `ConvertIndexCells` 少了 `DrawSingleIC`(:241-246) 會套的 `iBX`/`iBY` block 偏移 → 死區。實測：`XItem=4`/`XBlockItem=2`/`XBlockWidth=6` 在 100x200 下，cell 3 畫在 Left=75..95，但 pixel 95 hit-test 回 **-1**。
3. `ConvertIndexCells` 在它的 `-1` 早退之前就地改寫 `X`/`Y` → 16 個 golden 呼叫點在失敗後**不可**再用那兩個值。
4. `HTray.cpp:264-267` 除以 `FXVisibleItem`/`FYVisibleItem` 無零防護——用 `SetXVisible` 把每列都藏起來即可觸發。
5. `HTray.cpp:241-242` `X/(FXItem/FXBlock)` 在 `FXBlock > FXItem` 時除零。
6. `butPa1.cpp:21-22` **與** `BtnPanelLane.cpp:22-23` 把 `TColor tcTrueColor=clBtnFace; TColor tcFalseColor=clBtnFace;` 宣告成 **ctor 內的區域變數**，遮蔽同名 protected 成員 → 4 個顏色成員（`tcTrueColor`/`tcFalseColor`/`tcTrueFontColor`/`tcFalseFontColor`）在 golden 裡**從未被初始化**。

**唯一一個「保留語意但不保留 UB」的例外**：`MyLedLane.cpp:74-104` 的 `WriteRing`/`WriteIP`/`WritePort`/`WriteBit` 做 `char str[2]; strcpy(str, Value.c_str()); str[1]='\0';`（`WriteIP` 是 `char str[3]`）——只要輸入超過 1 字元就是真的 stack buffer overflow。**保留可觀察語意（只解析前 1 或 2 個字元），不要保留 UB**，標記 `GOLDEN BUG / behaviour-preserved-not-UB-preserved`。注意不對稱：`BtnPanelLane.cpp` 的對應函式用普通 `atoi`、沒有固定 buffer，是安全的。另注意這些路徑目前從資源端不可達（沒有任何 `.dfm` 對 `TMyLedLane` 設 Ring/IP/Port/Bit）。

#### W7-C2 — HDC renderer + headless 像素測試（C1 之後，3 agent 平行）

- **檔案**：新 `vclcompat/render/{LedRender,BtnPanelRender,TrayRender}.{h,cpp}`，簽名如 `void TrayRender(HDC hdc, const TrayCore& c, int w, int h)`，link `gdi32`。
- **為什麼 renderer 也不用等 MFC**：`CDC` 只是 `HDC` 的薄包裝，而 golden 用到的每個 primitive 在 MinGW 都有。對應幾乎 1:1——`Canvas->Brush->Color=X; Canvas->Rectangle(...)` → `CreateSolidBrush`+`SelectObject`+`::Rectangle`；`Canvas->Ellipse` → `::Ellipse`；`Canvas->MoveTo/LineTo` → `::MoveToEx`/`::LineTo`；`DrawText(Canvas->Handle,...)` 本來就是 raw Win32，原樣可搬（`DT_SINGLELINE|DT_VCENTER|DT_CENTER`）；`Canvas->StretchDraw(Rect, Bitmap)` → memory DC + `StretchBlt`；`FloodFill(W/2,H/2,clLime,fsSurface)` → `ExtFloodFill(hdc,w/2,h/2,RGB(0,255,0),FLOODFILLSURFACE)`。**`TColor` 是 `0x00BBGGRR`，與 `COLORREF` byte-identical，直接 cast，不要 swizzle。**
- **LED 美術資源**：`elec\Component\aled.res`（2,440 bytes）內含 `aled.pas:116-152` 依名載入的 6 個 `RT_BITMAP`——`ALEDSM`(368B)/`ALEDLG`(616B)/`ALEDSQLG`(368B)/`ALEDVT`(280B)/`ALEDHZ`(272B)/`ALEDSQSM`(232B)，共 2,136 bytes。**嵌成 `static const unsigned char[]` 進 `LedRender.cpp`**（免二進位資產相依、免 MFC 資源）。1,840 個實例只用到 4 種 style：`LEDHorizontal` 1357 / `LEDSqLarge` 459 / `LEDVertical` 23 / `LEDLarge` 1；`LEDSmall`/`LEDSqSmall` 是死碼，加註解 gate 掉。
- **測試（headless，絕不開視窗）**：畫進 `CreateDIBSection` 的 memory DC（`CreateCompatibleDC(NULL)`），(a) hash pixel buffer 對 checked-in 期望值，(b) assert 特定像素——cell 中心的填色、邊框的 `FFrameColor`、4 種 `TTrayDirectStyle` 各自的角落方向缺口、LED 在 `Value=true/false` 下的鏡片顏色、button 家族在 `Down` 時的 `Color`/`Font->Color` 對調。**沒有 `CreateWindow`、沒有 message loop，所以結構上不可能彈視窗（§5-11）。** scratchpad 的 `gdiprobe.cpp`/`gdiprobe2.cpp`/`szprobe.cpp` 可當起點。

#### W7-C3 — 併入 B1c（不是獨立波）

自製控制項的 metadata 欄位需求直接寫進 W7-B1c 的 layout table schema（見該波）。**不要另建一套平行的 `CustomCtrlDef` 產生器**——那正是 D12 要避免的雙來源。

#### W7-C4 — 把既有臨時替身接回真核心（**F0 與 C1 都完成後**）

- 這一波是**止血**：ad-hoc 替身正在持續累積、持續分歧，每多一波就更深。
- `FormsFacade.h:196-201` `TfMainAutoCleanGrid`（被 `fMain->tmyAutoClean`、`fMain->mtPlate2` 用）→ `vclcompat::TrayCore`；`FormsFacade.h:215` `TfLedValue`（被 `fMain->ALed1`、`fLotInfo->ALedLoader`、`fLotInfo->aLedAuto[3]` 用）→ `vclcompat::LedCore`。
- 解除 ~10 個 `BarCode/*.cpp` 目前掛著「no-op offline, TTMyTray not translated」註解的真 `SetCellNumber` 呼叫點：`BarCode_Bottom2DID.cpp:74`、`BarCode_Shuttle1_Scan.cpp:101`、`BarCode_Shuttle2_Scan.cpp:129`、`BarCode_Shuttle2_ScanRemainder1.cpp:20`、`BarCode_Shuttle2_ScanRemainder2.cpp:33`、`BarCode_Bottom2DID8CCD.cpp:176-183` 及對應 header。這些會變成真的、可測的狀態寫入。
- **驗證閘**：91/95 + 新增的 BarCode 狀態寫入有對應斷言。

#### W7-C5 — 3 個 MFC `CWnd` shell【**卡 MFC**】

見 §7 的 MFC 阻塞清單。約 150 行。

#### SKIP-AND-RECORD（不要卡在這兩個）

- **`TAPHeadLabel`（10 個實例，`main.dfm` 類的裝飾性漸層橫幅）：原始碼不存在**——只有 `D:\HT9045\dclusr50\apheadlabel.dcu`（編譯後的 Delphi）與機器產生的 `elec\Component\apheadlabel.hpp`（§4-V12）。從 `.hpp` 的公開屬性面（`Alignment`/`AutoBounds`/`BoundLines`/`BoundColor`/`Gradient.StartColor`/`Gradient.Style`/`SubCaption.Ellipsis`/`SubCaption.Style`/`Caption`/`Font.*`）用 `GradientFill` + `DrawText` + 邊線**近似**重做，並在最終彙整報告裡明載這是**近似，不是忠實移植**。
- `TMyMemo`（2 個實例，`elec\Component\MyMemo.{h,cpp}` 346 行，`: TMemo`）——也是自製、也不在簡報清單裡，很簡單，併進一般 memo 工作。

---

### W7-L — 邏輯長尾（F0 之後持續平行，與 UI 無關）

`remaining-cpp-recon` 的產出，全部不卡 MFC。**排在 F0 之後只是因為它們多半要新增 FormsFacade 成員。**

| 波 | 範圍 | 備註 |
|----|------|------|
| **W7-L1** | 7 個 `asendic_*.cpp`：`_Loader`(3312)/`_Auto`(2561)/`_Color`(1553)/`_Auto_RT`(1047)/`_Loader_RT`(905)/`_Auto2`(749)/`_Scanner`(372)，共 ~10,559 行 | 與已完成的 `asendic_Empty.cpp`（W6.1 canary）**同一套配方**；7 個獨立檔 → 最多 7 個平行 agent，零檔案碰撞（但**都會要新 FormsFacade 成員**，見 §8） |
| **W7-L2** | `ckernel.cpp` 的 ~1,565 個非 UI 行 | golden 2,588 行（§4-V11），0% 已翻。**「卡 E0/M1/M2 HAL 設計」的說法對這個檔不成立**——那是 recon 文件對 Z1/Z2 解鎖的**排序建議**，不是 `ckernel.cpp` 的技術阻塞。可翻：`CheckThermo`(38)/`StopAllDestroy`(22)/`CheckBinSet`(42)/`ChangeUseSuckMode`(106)/`DoPanelLamp`(171，只碰 `SW[]` IO 不碰 VCL)/`DoSystemMessage`(20)/`DoScanLevelSensor`(24)/`GetMotorAlarmCode`(69，`TComponent*` 只做身分比對，換 `HTMotor*` 即可)/`ProcessAlarm`(28)/`InitialTestDelayStatus`(60)/`WaitManualStepKey`+`WaitManualStartKey`+`WaitManualRetryKey`(97，各需 1 個新 `fContact` gated stub)/`ScanSystemSensor`(345，只 3 處 `fMain->`，`csystem.cpp:16894` 已有預留呼叫點)/`ScanPannelKey`(489，只 2 處 `fMain->`)。**延後**：`ShowRunLed`(231/32 refs) + `ShowRunLabel`(793/151 refs) = 1,024 行（佔全檔 40%）純 VCL 狀態顯示 → W7-U。**另注意**：`ckernel.h` 有 8 個宣告（`CheckReceiveTrayBuildFull`/`SetWarn`/`ClearWarn`/`ProcessRunStatus` 等）**全樹零定義**＝孤兒宣告；`InitialSensor` 的真定義在 `cinitial.cpp` 不在 `ckernel.cpp` |
| **W7-L3** | `Public/cSiteUseManager.cpp`（golden 556 行，只 29 行已翻 = 5.2%） | **新發現，不在任何 DEFERRED 列**。小、可能獨立 |
| **W7-L4** | `Interface/TesterTCP.cpp` 的 `CopyOSTestResult`/`PlaceOSTestResultToTray` | **阻塞已過時**（§4-V10）：`GetSiteNo`/`GetOrderOfContact` 已在 `Public/MyProductionRecord.cpp:666,671`（Wave18，`f6bf156`）。現在可開工 |
| **W7-L5** | `MainCalcCore` 續批（從 `main.cpp`/`uLotInfo.cpp` 抽純算葉節點） | 續既有 pattern |
| **W7-L6** | `main.cpp` / `uLotInfo.cpp` 的**業務邏輯**抽取（非 UI） | **最大槓桿也最大工程**。實測分佈：`main.cpp` 34,972 行 / 383 函式中 **65.0%（22,570 行）是硬體/業務邏輯**（`Index16Heater` 1,653 行 / 1,020 hw refs、`OnMyCopyMsg` 1,830 行＝`WM_COPYDATA` 命令分派器、`Start` 1,876、`ChangeATCSiteUse` 815、`SetRunStartMode` 754、ctor 927），只有 **28.4%（9,853 行）真的是 widget-heavy**。`uLotInfo.cpp` 16,613 行 / 210 函式：**56.4% 業務**（`sbSECSLotStartClick` 1,036 是最大單支），40.3% UI（`ShowNewATCThermo` 588 / `ShowATC20Thermo` 487 / `ShowATC70Thermo` 339 是**溫度圖表繪製**，不是溫控邏輯）。**這是多波、跨多場次的工程，不是一波** |
| **W7-L7** | `cContact.cpp` 的 `CarlibrationTask` + 6 個 sub-SM 叢集（~11,478 行 = 全檔 50%） | **全樹剩餘最高風險單一區塊**。golden 行號已複驗逐字正確：`CarlibrationTask@11755` len 2210、`Do_Z1_AutoGetHeight@5389` len 2785、`Do_Z2_AutoGetHeight@8174` len 2339、`Do_ContactTest_32Site@10513` len 842、`Do_ROILearning@14497` len 515、`DoZPlaceToShuttle@11355` len 386。widget 讀取**穿插在 switch-case 內部**，不能像 site-variant 那樣機械 clone。**必須先做 CT0「widget-facade 抽取」spike 量化可行性，才承諾多波 CT1 計畫。** F0 落地後這件事會變簡單很多（統一的 `Controls.h` 型別就是那個 facade） |

---

### W7-U — MFC UI 本體【**全部卡 MFC 元件安裝**】

見 §7。順序照**跨模組 deref 數**排，**不是照 `.dfm` 大小**——`iosetview.dfm` 是最大檔（37,752 行）但低耦合，`main.dfm`（17,411 行）撐著 3,811 個跨模組 deref。前 8 個表單（`fMain` 3,811 / `fLotInfo` 1,337 / `fSCKART` 913 / `fContact` 513 / `fNote` 469 / `fBarCode` 375 / `fTeach` 365 / `fObserver`）就佔了 14,432 個跨模組 deref 的約 **7,900（>一半），而它們不到表單總數的 10%**。

---

## §7 卡 MFC 元件的完整清單（**只有這些**）

使用者必須跑 VS Installer 加「**C++ MFC for latest v143 build tools**」。在那之前，以下**且僅以下**無法進行：

1. **`ui/FormsFacadeMfc.cpp` 與各表單的 `CDialog`/`CWnd` 子類**，以及它們持有的 `TfXxxImpl : public TfMain` binder（D3）。
2. **`OnInitDialog` 的 `ApplyLayout` pass**：`MoveWindow` 逐控制項套 layout table + `CreateFontIndirect`/`SetFont` 套 9,933 組 per-control 字型 + 建立 child dialog + z-order 修正。
3. **`OnCtlColor` / owner-draw**，處理約 **14,600** 個顏色屬性（`Font.Color` 9,936 + `Color` 4,705 + LED/BtnPanel 的 `TrueColor`/`FalseColor`/`TrueFontColor`/`FalseFontColor`）。MFC dialog resource 沒有宣告式顏色，這是真程式碼。
4. **`WM_SIZE` dock 程式碼**，取代 VCL 的自動佈局。注意**主要是 `Align`（2,361 個：`alTop` 1262 / `alClient` 523 / `alLeft` 251 / `alBottom` 248 / `alRight` 64 / `alCustom` 11），不是 `Anchors`（只有 54 個）**。
5. **7,280 個事件綁定的 message map**（`OnClick` 4,519 / `OnMouseDown` 1,029 / `OnKeyPress` 375 / …，共 50 種）。
6. **W7-C5：3 個 `CWnd` shell**（`CLedCtrl`/`CBtnPanelCtrl`/`CTrayCtrl`，各約 50 行）。`CBtnPanelCtrl` 應從 **`CStatic` + `SS_NOTIFY`** 派生而非 `CButton`——golden 的 base 是 `TPanel`，沒有 focus rect、沒有鍵盤啟動，而 757 個實例的 714 個 `.dfm` `OnClick`/`OnMouseDown` 只用得到點擊。
7. **`HT9045_RegisterAllCustomClasses()` 的實際 `AfxRegisterWndClass` 呼叫**（§3-C1 的裁決）——不過這一支也可以先用純 `RegisterClassEx` 寫成不含 MFC 的版本先驗證。
8. **`CWinApp`/`WinMain` bootstrap** 與 `ht9045_app` 的 link。
9. **`build_msvc_ui`（`HT9045_UI=ON`）這個 build 目錄本身。**
10. **只有真的把視窗跑起來才能確認的事**（記進 `docs/W7-UI-SKIPPED.md` 的「首次執行目視檢查表」）：VCL vs Win32 的 **z-order 極性**（VCL 把後宣告的堆在上面；dialog manager 的 template 順序極性必須實測）、3,338 對重疊 sibling 的目視確認、30+ 種字型（含 `新細明體`/`微軟正黑體`/`標楷體` 等 CJK 字面）的替代行為。

**上游全部不卡。** 特別要說清楚的三件事：

- **`rc.exe` 屬 Windows SDK，不屬 MFC**（§4-V9）。所以 **`.rc` 產生、編譯、`.res` 二進位回讀驗證（G5/G6）今天就能全部跑完**。
- **renderer 不卡**：MinGW 有完整 `gdi32`，`CDC` 只是 `HDC` 的包裝（W7-C2）。
- **binder 機制不卡**：`PullFromControl`/`PushToControl` 可以先用純 Win32（`GetWindowTextA`/`SetWindowTextA`/`BM_GETCHECK`/`BM_SETCHECK`/`CB_GETCURSEL`/`ShowWindow`/`EnableWindow`）實作，並對 **`HWND_MESSAGE` 為 parent 的真控制項**做 headless 測試——message-only window **結構上不可能被顯示**，所以這滿足「不得彈視窗」是靠構造而非靠紀律。MFC 的 DDX 只是這些呼叫的便利包裝，所以之後那一步是薄 adapter，不是新設計。

---

## §8 檔案碰撞矩陣（平行派工必讀）

| 檔案 | 狀態 | 規則 |
|------|------|------|
| `FormsFacade.h` / `FormsFacade.cpp` | **HARD BOUNDARY（F0 期間獨佔）** | F0 凍結期間任何其他波不得寫。**F0 之後拆成 `forms/fXxx.{h,cpp}`，碰撞面自然消失——這正是 F0 的主要收益之一** |
| `forms/fXxx.{h,cpp}`（F0 之後） | 可平行 | 不同波動不同表單的檔即可平行；同表單仍要序列 |
| `tests/CMakeLists.txt` | **MERGE-AT-INTEGRATE，不凍結** | 幾乎每個波都要在底部 append 一個 target。專案已有可行做法（ROADMAP 2026-07-28 記載「依共用檔案連通分量合併，非逐波拆分」）。**平行翻譯 → 序列 integrate → 由 integrator 手動合 hunk。** |
| 根 `CMakeLists.txt` | 序列 | A1 動頂部、F0 動 library 區。**排 A1 → F0，不要同時** |
| `vclcompat/Controls.h` | **F0 獨佔** | C 系列**不得**碰它（D5：自製核心各自開檔） |
| `vclcompat/{TrayCore,BtnPanelCore,LedCore}.*` + `vclcompat/render/*` | 全新檔，可 3 路平行 | C1/C2 內部零重疊 |
| `tools/dfm2rc/*` + 產生的 `<Form>.{rc,ir.json}`/`_ids.h`/`_layout.gen.*` | 全新檔，不碰任何既有檔 | **B1 整條在 F0 凍結期間照跑** |
| `vclcompat/vcl_compat.h` | A0 獨佔（單檔、單次） | A0 之後任何人都不該再動 include 順序 |
| `aoutarm9045_*.cpp` ×27 | A2 獨佔 | 與所有其他波零重疊 |
| `Automation/SCK_ART.h` / `SCK_ART_Remainder.h` / `csystem.cpp` | F2 獨佔（4 方整併） | 這是分析工作，不要跟機械別名化混在同一個 agent |
| `MainCalcCore.{cpp,h}` | W7-L5 獨佔 | 只跟自己碰撞 |

**歷史教訓**：本 repo 已經因為平行波次互相干擾發生過一次**孤兒 git stash 事件**（ROADMAP `SaveTestSummarySECS` 那段）。派平行 agent 時務必：每個 agent 只被授權寫它自己的檔案清單、integrate 一律序列、**agent 自己要用 `grep`/`diff` 驗證磁碟內容而不是相信工具回報**。

---

## §9 已知風險與未解問題

| # | 風險 | 嚴重度 | 處置 |
|---|------|--------|------|
| **R1** | **VCL vs Win32 的 z-order 極性未解，且 headless 無法解**。更糟的是 Win32 把 z-order 與 tab order 綁成**同一個**排序，而 VCL 是分開的——實測 2,180 個 parent group 裡 **1,004 個（46.1%）兩者真的不一致**，單一 template 順序無法同時滿足 | MEDIUM | 先縮小損害範圍：226,023 對 sibling 裡只有 **3,338 對（1.477%）幾何重疊**，所以 98.5% 的語料 z-order 根本無所謂。**照 `.dfm` 宣告序 emit**（維持 `.rc`↔`.dfm` 1:1 可 diff、checker 精確），layout table **同時**記 `taborder` 與 `zindex`，並把排序做成單一產生器開關 `--zorder=asdeclared\|reversed`，等有人能跑視窗時一個 flag 全語料翻轉。3,338 對的清單直接 emit 給 code-behind 波（最大群是 TPanel/TPanel 919 對，通常是互斥的堆疊頁，執行期靠 `Visible` 決定） |
| **R2** | **`windres` 比 `rc.exe` 寬鬆，只用 `windres` 當閘門會給假信心**。實證：壓力測試 `.rc` 用了 `WC_TABCONTROL`（一個展開成 L 前綴寬字面值的 C 巨集），`windres` exit 0 並產出 `.res`，`rc.exe` 正確地以 `RC2104 undefined keyword or key name` 失敗 | MEDIUM | **`rc.exe` 是權威閘、`windres` 是次要/可攜性跑者，兩者都在就都要過**。且**永遠不 emit `WC_*`/`TRACKBAR_CLASS`/`UPDOWN_CLASS`/`DATETIMEPICK_CLASS`/`PROGRESS_CLASS`/`WC_TREEVIEW` 巨集，一律用字面 class name 字串**。G6（`.res` 回讀）無論哪個編譯器出手都能抓到 |
| **R3** | **HWND（USER object）耗盡**。golden VCL 若所有表單都活著約需 13,570 個 HWND，因為 **6,760 個實例是 `TGraphicControl`（TLabel/TSpeedButton/TShape/TImage/TBevel + 自製 LED）在 VCL 裡根本沒有視窗**。本設計把它們全變成真視窗 → 約 22,429，**+65%**。行程預設配額 10,000。最糟單一表單 `iosetview`：golden 1,289 vs MFC 2,928 | MEDIUM | 已藉「拒絕 placeholder Static 變體」省下 5,594 個（`iosetview` 本來會是 3,491）。IR 對全部 6,760 個標了 `is_graphic_control`，code-behind 波可以**資料驅動地**改成在 parent 上 owner-draw，這一個開關就能回到 golden 同等級。配額可經 `HKLM\...\Windows\USERProcessHandleQuota` 提到 18,000。VCL 本來就是按需建表單、不是一次全建。**等表單能真的被實例化時要實測** |
| **R4** | **`rc.exe` 的 `\xHH` 在 ANSI 字面值裡是逐 byte 且靜默毀字**：`"Group \x4E2D\x6587"` 編出來變成 `Group N2De87`，無警告。語料有 887 個非 ASCII 字串屬性 | LOW | 已驗證解法：用 `L"..."` 寬字面值前綴，`\xHHHH` 會被正確處理（`L"A\x4E2D\x6587B"` 經 `rc.exe` 進 `.res` 再出來仍是 `A中文B`）。**一律用 `L"..."`**（不只非 ASCII 的），單一路徑。G6 assert 解碼後的 `.res` title 等於 cp950 解碼的 `.dfm` 字串。**此條建議進 KNOWLEDGE（§10）** |
| **R5** | 9 類 class 沒有乾淨對應：7 個自製 VCL 元件（2,853）、`TStringGrid`(119)+`TDBGrid`(1)、`TShape`(62)、`TChart`(5)+53 series、`TCppWebBrowser`(2)、`TMediaPlayer`(1)、`TEBW8ROI`(2)、以及 `cheksocket.dfm` 裡的兩個單例第三方 class `TEasyMain`(1)/`TECameraPicolo`(1)（Euresys frame-grabber SDK） | MEDIUM | B1 不被擋：全部 emit 成 `CONTROL "", id, "HT9045.<Name>", ...`（`rc.exe` 接受任意註冊 class name 字串，已驗證），props 走 layout table。**`TStringGrid` 絕不可映成 `CListCtrl`/`LVS_REPORT`**——VCL `TStringGrid` 是可自由定址的 `Cells[c][r]` 稀疏格 + `OnDrawCell`/`OnSelectCell`，而**譯出樹已經有 headless `vclcompat::TStringGrid` shim**，目標應是 `"HT9045.StringGrid"` 後接那個 shim。`TEasyMain`/`TECameraPicolo` 直接進 SKIP 報告——那是廠商 SDK 表面，不是可翻譯的 UI |
| **R6** | **336 個 `TRadioGroup` 在 `.dfm` 裡有零個子物件**，802 個 radio button 隱含在 `Items.Strings` 裡。產生器必須**發明控制項**，與「絕不發明功能」及 checker 的零容忍衝突 | LOW | 把合成做成**明確、具名、可列舉的規則**而不是隱性行為（見 W7-B1d 的兩條規則 + `synthesized:true` + rule tag），checker assert 合成集合恰好等於規則預測 |
| **R7** | **F0 是不可避免的全樹序列點**，而本 repo 已有平行波次互相干擾的前科 | HIGH | F0 範圍已砍（§3-C3）：SckArt 整併與 seam 退場移出成 F2。凍結期間 B1/C1/C2/A2/唯讀 recon 照跑。F0 是純重構所以閘門異常清晰，凍結很短 |
| **R8** | **`void*` overload 誤綁現在就已武裝**，會在 1,740 個 SV/EC 註冊中途靜默引爆 | HIGH | F0-a 的 `TObject` base 修掉、F0-b 的 `static_assert` 永久守住。**必須在任何 `uHGemHT9045_SV.cpp`/`_EC.cpp` 波開始之前完成** |
| **R9** | **4 份 SckArt state 可能已行為性分歧**，合併可能改行為不只改結構 | MEDIUM | F2 的獨立子任務，逐欄位對 golden diff，每個非預設值欄位補回歸測試，發現分歧就**回報不調和** |
| **R10** | **UI 層的視覺保真度沒有可自動化的 oracle** | MEDIUM | 拆兩半：可轉移的部分（幾何/z-order/tab-order/parent chain）在 B1 以**資料**驗證、零 MFC；不可轉移的部分（顏色、扁平化、Align）是使用者已接受的手寫程式碼，刻意排到最後 |
| **R11** | **FP 語意分歧**：MinGW 1233 / MSVC 1234（runtime 值）。在滿是 `double→int` 截斷的碼庫（`iUnitMultiply100/1000`、`ChangeToIntNonPcnt`、pitch/offset 幾何）這會改變出貨行為。本專案已有前科（記憶 `ht9045-divide-safety-int-to-float-regression`） | HIGH | `/arch:IA32` **與** `_controlfp(_PC_64,_MCW_PC)` 必須**同時**（單獨任一都不夠，§4-V5）。`_controlfp` 要進 `tests/test_bootstrap.cpp` **與**未來 app 的 `WinMain`。在這個到位之前，任何 MSVC-only 的數值失敗都視為預期、不是 bug。`/arch:IA32` 只在 x86，而本 build 為了硬體驅動本來就必須維持 32-bit，所以無副作用 |
| **R12** | ~210 個 `__fastcall` 標記的 decl/def lockstep **目前只靠「記得 grep linker log 找 resolving」的程序性紀律**守著（`vclcompat/vcl_compat.h:46-48`） | MEDIUM | MSVC 把它變成自動 `LNK2019`（MSVC 把 calling convention 編進 mangled name，GCC Itanium ABI 不會）。在 Gate 2 上線前，**grep 習慣照舊必須保留** |
| **R13** | 兩個 build 目錄 → 一個波可能 MinGW 綠、MSVC 紅，時間壓力下 agent 會學會忽略 MSVC | LOW | **先把 MSVC 拉到乾淨的 91/95 再讓 Gate 2 變強制**（A3）。常態紅的 oracle 會被忽略；常態綠且與另一個編譯器完全一致的 oracle 才會被信任 |

---

## §10 建議給主迴圈的文件更正（**本檔作者不動 ROADMAP/DEVLOG**）

以下是複驗過程中發現、應由主迴圈套用到 `MIGRATION_ROADMAP.md` / `KNOWLEDGE.md` 的更正：

1. **`MIGRATION_ROADMAP.md:9` 與 `:20`**：「UI 框架（MFC/Qt/Win32）延到 W7 再定」「先定 UI 框架＝獨立大決策」**已過時**——MFC 已定案，改指向本檔。
2. **`MIGRATION_ROADMAP.md:133`（牆②）**：現在把阻塞單獨歸給「~19 個 FormsFacade 缺口」。成員數對（golden `uHGemHT9045.cpp` 恰觸及 28 個相異 `fMain` 成員），但**真正的結構性阻塞是 CMake 循環依賴**（`FormsFacade.cpp` 在 `ht9045_sm`:731，`ht9045_secsgem`:588 不 link `ht9045_sm`，`ht9045_sm`:1208 link `ht9045_secsgem`）。**補 19 個 stub 不會解鎖任何東西。** 已複驗（§4-V1）。
3. **`MIGRATION_ROADMAP.md:135`（`Interface/TesterTCP.cpp`）**：`CopyOSTestResult`/`PlaceOSTestResultToTray` 那句「卡 `GetSiteNo`/`GetOrderOfContact` 缺口」**已過時**——兩者自 Wave18（`f6bf156`）起已存在於 `Public/MyProductionRecord.cpp:666,671`。已複驗（§4-V10）。
4. **`MIGRATION_ROADMAP.md:49`（`csystem.cpp` 覆蓋率）**：「~6% 方法 / ~2.6% 行」是 W6.6-spine-only 的快照。把後續 `DoCleanOutFinishCheck`(1036)+`DoOneCycleFinishCheck`(1235)+`DoART_AfterCleanOut`(663)+spine(672)+AutoSiteMap 分支(35)+Hotplate/LoaderVibrate 迴圈算進去，golden-line-equivalent 覆蓋約 **3,600-3,750 / 25,483 ≈ 14.5%**。（此項為 `remaining-cpp-recon` 的宣稱，本檔作者**未**獨立複驗，主迴圈套用前宜自行確認。）
5. **`FormsFacade.h:385`**：`tmyAutoClean` 的 golden 型別標成 `THeatTable*` 是錯的——全樹（golden 與元件庫）無此 class。正確是 `TTMyTray*`，同它的兄弟 `mtPlate2`（`main.h:164`，`FormsFacade.h:389-395` 標對了）。純註解修正，已排入 F0-c。
6. **`vclcompat/vcl_compat.h:127-131` 的策略註解**：「safe whether or not windows.h happened to be included earlier」**是錯的**——已親自重現反例（§4-V6）。A0 波要一併改寫這句。
7. **建議新增 KNOWLEDGE gotcha #10**：`rc.exe` 的 `\xHH` byte-escape 陷阱 + `windres` 比 `rc.exe` 寬鬆的落差（§9-R2/R4）。
8. **建議新增 KNOWLEDGE gotcha #11**：`aled.pas` 的 `CreateLedBitmap` 尺寸覆寫**看起來**會丟掉 `.dfm` 幾何，**實測 1,840 個裡 0 個被丟棄**；後來的 agent 若「好心修正」會改掉 262 個 LED 的尺寸（W7-B1c）。
9. **建議新增 KNOWLEDGE gotcha #12**：FP 驗證腳本必須用 runtime/volatile 輸入——`(int)(1.234*1000.0)` 寫成編譯期常數時 MinGW 也給 1234（常數折疊），只有 runtime 值才顯出 x87 的 1233（§4-V5）。本檔作者第一次複驗就踩到這個。
10. **建議新增 KNOWLEDGE gotcha #13**：`.dfm` 直方圖找不到**動態建立**的控制項——`TTMyTray256` 有 0 個 `.dfm` 實例卻在 `cBinSel.cpp:232-235` 被 `new`。清點自訂元件時必須同時 grep 型別指標宣告（§4-V12）。
11. **`MIGRATION_ROADMAP.md:8` 與 `KNOWLEDGE.md:58-59` 的 MSVC 更正已經在檔內、內容正確，不需再改。** 唯一可補的是「`rc.exe` 與 `windres` 兩者都在，所以 `.rc` 閘門不等 MFC」這件事目前哪份文件都沒記。
12. **本檔 §6 W7-A0 條目本身（:214）有誤，不是實作跟丟**：該條目指示「加 `#include <winsock2.h>`」不分編譯器一律加在 `windows.h` 之前。W7-A0 的實作沒有照做，改成 `#if defined(_MSC_VER)` 只在 MSVC 才加——這個偏離是對的，已重新獨立複驗：在 MinGW 下把 `<winsock2.h>` 無條件放在 `<windows.h>` 之前會讓一般 Win32 錯誤碼（`ERROR_SHARING_VIOLATION`、`ERROR_LOCK_VIOLATION` 等）整批不被定義（根因：`winsock2.h`→`winsock.h` 先設 `__WINSOCK_H_SOURCED__` 才 `#include <windows.h>`/`<winerror.h>`，導致 `winerror.h:36-41` 那段本來會 `#define _WINERROR_H` 並展開一般錯誤碼的區塊被跳過兩次，且該巨集要到 `winsock.h:682` 才解除，那時 `windows.h` 自己的 include guard 早已生效，後面任何 `#include <windows.h>` 都不會回頭重新處理 `winerror.h`）。實測：`#include <winsock2.h>` + `#include <windows.h>` 後 `ERROR_SHARING_VIOLATION`/`ERROR_LOCK_VIOLATION` 兩者皆 `-fsyntax-only` 報「未宣告」；只 `#include <windows.h>` 則乾淨。`common.cpp:2313`（該檔 :45 先 include 本 umbrella）確實測試這兩個常數，會被打中。**結論：§6 這條指示本身要修正為「MSVC-only 加 `<winsock2.h>`」，現有程式碼（`vcl_compat.h` 的 `#if defined(_MSC_VER)` 分流）是對的，不要「復原」成無條件版本。** 本檔 §6 原文保留不動（append-only），更正記在此處供主迴圈與後續 agent 對照。

---

## §11 立刻可開工清單（給下一個 agent 的最短路徑）

**現在就能派、且今天就能在 MinGW 下驗證完的：**

| 波 | 平行度 | 前置 | 一句話閘門 |
|----|--------|------|-----------|
| **W7-A0** | 1 agent（1 檔） | 無 | 兩種 include 順序都要 `-fsyntax-only` 過 + ctest 91/95 |
| **W7-A2** | 1 agent（27 檔） | 無 | ctest 91/95 + `grep -ic resolving` = 0 |
| **W7-B1a** | 1 agent（全新檔） | 無 | 133 檔 0 解析錯誤、22,627 節點 |
| **W7-B1b** | 1 agent（全新檔） | B1a | 全 133 個 `.rc` 過 `rc.exe`；`.res` 回讀 diff 零差異 |
| **W7-B1c** | 1 agent（全新檔） | B1a（**與 B1b 平行**） | `_layout.gen.cpp` 在既有 MinGW CMake 下 `-Wall` 編過 + ctest 對表 |
| **W7-C1** | **3 agent 平行**（各自新檔） | 無 | 3 個新 ctest target 綠 + 6 個 golden bug 各有「修好就失敗」的回歸斷言 |
| **W7-C2** | **3 agent 平行**（各自新檔） | C1 | memory-DC pixel hash + 逐點斷言，零視窗 |
| **W7-A1** | 1 agent | **A0** | MinGW 側維持 91/95；MSVC 側取得 baseline |

**接著（序列）**：`W7-F0`（短凍結）→ 解凍 → `W7-F1` / `W7-F2` / `W7-C4` / `W7-L1..L5` 全面 fan-out + `W7-B1d`。

**最後（卡使用者安裝 MFC）**：`W7-C5` → `W7-U0` → `W7-U1..Un`（照 §6 的跨模組 deref 排序）。

---

## §12 每一波共同的收尾閘（ratchet，不可跳）

1. **Fresh from-scratch** build（不是增量）+ `ctest --timeout 300`（或 600）= **91/95**、且失敗的就是那 4 個既有環境漂移（`config_db`/`IniFiles`/`ini_helpers`/`config_loaders`）。
2. 純重構波另加：**每個 test target 的 assertion 數 build 前後逐一 diff 必須相同**。
3. `grep -ic resolving` build log = **0**。
4. 全樹 **mojibake（U+FFFD）/ BOM 掃描 = 0**。
5. **獨立 fidelity-review agent** 逐行對 golden（不重譯），發現一律由主迴圈核實 golden 後修正。
6. 遇到真正的阻塞：**SKIP 並記錄到 `docs/W7-UI-SKIPPED.md`**，不要停下整條線（使用者常設指示）。
7. **不要 `git commit`**（commit 由主迴圈負責）。
