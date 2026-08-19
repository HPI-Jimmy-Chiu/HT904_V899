# RECON: FW-1 tag 接線 crosswalk（瀏覽器綁定 vs wb_serve 發布）

AI(W906-FW1-recon) 20260819: 偵察報告，非交付波次。目的是量出「瀏覽器實際綁定的 tag
名」與「wb_serve 實際發布的 tag 名」的完整交集/缺口，並對下一批接線提出有根據的候選清單。
**只寫了本檔與 `tools/webprobe/snap_dump.py` 兩個新檔，未改動任何其他檔案，未 commit。**

---

## 1. 量測方法與時間戳

**發布側**（實測 snapshot，非讀文件）：

```
cd D:\HT9045\HT9011UC_Cpp_V3.33.906.0
.\build_fwlicg\wb_serve.exe --port 8061 --seconds 40 --dry     # 20260819 20:14 啟動
python tools\webprobe\snap_dump.py --port 8061 --seconds 15 --out snapshot.json   # 20260819 20:14 執行
```

`wb_serve` 啟動列印：`published 102 tags, 43 of 84 carry a loaded value`（102 = 上線
tag 總數；43/84 是 `HandlerTagCoverage()` 自己定義的「機台資料來源」子集，兩者分母不同，
細節見 `WebBridgeTags.cpp:643-646` 的既有註解，不是本次量測發現的新東西）。

`snap_dump.py` 是新檔，直接 `import` `cmd_probe.py` 的 `ws_handshake()` / `read_frames()`
（未重寫 frame parser），連上 `ws://127.0.0.1:8061/ht9045`，取第一個 `{"type":"snapshot"}`
frame，dump 成 `{tag: value}` 字典。實測結果：**102 個 tag，43 個非 null，59 個 null**（與
啟動列印的 43 一致，是同一個量測，兩處互相印證）。

**綁定側**（靜態程式碼追蹤，非執行瀏覽器 —— 環境沒有 headless browser，改用逐一追蹤
`web/js/panels/*.js` → `web/js/ui/widgets.js` 的呼叫鏈，把所有 `data-tag`/`data-tone-tag`
樣板字串在迴圈/參數代入後完整展開）。追蹤範圍：`web/index.html`、
`web/js/panels/{chrome,left,right,statusrail}.js`、`web/js/ui/{widgets,bind,dom}.js`、
`web/js/model/{state,tagmap}.js`。展開邏輯寫成一次性腳本（scratchpad，非交付物）並列印
family 統計，人工核對迴圈上下界（`site.arm{1,2}.s{1..16}`、`STATUS_SPARE_SLOTS=19` 等）
逐一對照原始碼行號。

兩份資料集都已固化成 JSON（scratchpad：`bound_tags.json` 296 筆、`snapshot.json` 102 筆），
下面所有計數都是對這兩份 JSON 取交集/差集算出來的，不是印象。

---

## 2. headline 數字

| 指標 | 數值 | 單位 |
|---|---|---|
| wb_serve 發布的 tag 總數 | 102 | 個 tag 名（`snap_dump.py` 實測） |
| 其中非 null（本次 `--dry` 執行下有值） | 43 | 個 |
| 瀏覽器 DOM 實際綁定的 tag 名總數 | 296 | 個（含 `data-tone-tag` 次通道） |
| 綁定 ∩ 發布 | 76 | 個 |
| 綁定 ∩ 發布，且本次量測非 null | 33 | 個（全部集中在 `site.arm{1,2}.s*` 32 個 + `startmode.value` 1 個） |
| 綁定 ∩ 發布，但 null | 43 | 個（`WebBridgeTags.cpp` 明確承認的 33 個死源 + auth/control/tower/run/runmode/zone 等已接但本次無值） |
| 發布但未綁定 | 26 | 個（`auth.*`、`control.owner`、`lastset.*`、`machine.customerCode`/`id.*`、`machine.stateSource`、`pump.*` 18 個） |
| 綁定但未發布 | 220 | 個 |
| `web/js/model/tagmap.js` 的 `TAGMAP` 條目數 | **44**（非任務書所稱 43，見下方誤差說明） | 個 |
| 上述 44 筆中 dfm 已解析 | 38 | 個 |
| 上述 44 筆中 dfm 未解析 | 6 | 個：`clock.text` `machine.state` `temp.pv` `status.rms` `status.indexTime` `status.uph` |

**43 vs 44 的落差**：用 `grep -c '": { dfm:'`／regex 對 `TAGMAP` 物件字面量算了兩次，
都是 44（含 `"site.*"` 這個群組條目本身）。任務書寫的 43 可能是不算 `"site.*"`（它代表
32 個 cell 而非單一控制項）或單純筆誤；本檔以現場量測的 44 為準，附算法：對
`js/model/tagmap.js` 的 `export const TAGMAP = {...}` 物件字面量用
`re.findall(r'"([^"]+)":\s*\{ dfm: (null|\`[^\`]*\`)', body)` 抓鍵值，20260819 執行。

---

## 3. 三欄（其實是四欄）crosswalk：family 總覽

`published` = 該 family 裡有幾個 bound tag 同時被 wb_serve 發布（不論 null 與否）；
`non-null` = 其中本次量測有真值的個數。

| family | bound | published (of bound) | non-null |
|---|---:|---:|---:|
| `arm.*`       | 64  | 0  | 0  |
| `bin.*`       | 6   | 0  | 0  |
| `cat.*`       | 32  | 0  | 0  |
| `clock.*`     | 1   | 1  | 0  |
| `contact.*`   | 18  | 0  | 0  |
| `fan.*`       | 1   | 1  | 0  |
| `light.*`     | 1   | 1  | 0  |
| `lot.*`       | 16  | 0  | 0  |
| `machine.*`   | 2   | 1  | 0  |
| `recipe.*`    | 1   | 0  | 0  |
| `run.*`       | 3   | 3  | 0  |
| `runmode.*`   | 3   | 3  | 0  |
| `site.*`      | 32  | 32 | 32 |
| `sort.*`      | 14  | 6  | 0  |
| `speed.*`     | 12  | 0  | 0  |
| `startmode.*` | 1   | 1  | 1  |
| `status.*`    | 60  | 11 | 0  |
| `temp.*`      | 10  | 4  | 0  |
| `tester.*`    | 7   | 1  | 0  |
| `tower.*`     | 3   | 3  | 0  |
| `user.*`      | 1   | 0  | 0  |
| `zone.*`      | 8   | 8  | 0  |
| **TOTAL**     | **296** | **76** | **33** |

`site.*` 是唯一一個「綁定=發布=非null」三個數字疊在一起的 family——這正是 FW-1a 那批
接線的成果，其他所有 family 要嘛完全沒發布（0），要嘛發布了但目前只是誠實的 null。

---

## 4. 完整逐項清單

高度重複、全 0 的 family（每一項都已逐一核對，不是用總數推算）：

- **`arm.*`**（64 個：`arm.A1`…`arm.P2` 各一個值 + 一個 `.tone`，來自
  `left.js:leftColumn -> valTable('arm', ARM1_ROWS/ARM2_ROWS)`）—— 0 個發布。
- **`cat.*`**（32 個：`cat.arm{1,2}.s{1..16}`，`right.js:testCategoryPanel ->
  siteGrid({tagPrefix:'cat'})`）—— 0 個發布。
- **`contact.*`**（18 個，`right.js:contactPanel`，`CONTACT_KINDS` 9 對）—— 0 個發布。
- **`speed.*`**（12 個，`right.js:indexPanel`，6 個軸 × xy/acc）—— 0 個發布。
- **`bin.*`**（6 個，`right.js:binSelectPanel`）—— 0 個發布。

這五個 family 的「未發布」現況與 `web/js/model/tagmap.js` 的 `UNMAPPED_GROUPS`
（`cat.` `contact.` `speed.` `bin.` `lot.` `sort.` `arm.` 都列在裡面）互相印證，
不是本次新發現，本次的貢獻是把「未發布」換成量測過的「published:false」。

有真實訊號、值得逐項列出的 family：

### `site.*`（32/32 已發布，32/32 非 null —— 已完成的對照組）

| bound tag | published | value |
|---|---|---|
| `site.arm1.s1`  | yes | 0 |
| `site.arm1.s2`  | yes | 0 |
| `site.arm1.s3`  | yes | 0 |
| `site.arm1.s4`  | yes | 0 |
| `site.arm1.s5`  | yes | 0 |
| `site.arm1.s6`  | yes | 0 |
| `site.arm1.s7`  | yes | 0 |
| `site.arm1.s8`  | yes | 0 |
| `site.arm1.s9`  | yes | 1 |
| `site.arm1.s10` | yes | 1 |
| `site.arm1.s11` | yes | 1 |
| `site.arm1.s12` | yes | 1 |
| `site.arm1.s13` | yes | 0 |
| `site.arm1.s14` | yes | 0 |
| `site.arm1.s15` | yes | 0 |
| `site.arm1.s16` | yes | 0 |
| `site.arm2.s1..16` | yes (all 16) | 同 arm1 的圖案（0/0/0/0/0/0/0/0/1/1/1/1/0/0/0/0） |

### `sort.*`（14 個：6 個已發布皆 null，8 個未發布）

| bound tag | published | value | 備註 |
|---|---|---|---|
| `sort.auto1.count` | yes | null | `Prod.iTrayType[eAuto1]==tNotUse`（本機未配置該站，非缺陷） |
| `sort.auto2.count` | yes | null | 同上 |
| `sort.auto3.count` | yes | null | 同上 |
| `sort.fix1.count`  | yes | null | 同上 |
| `sort.fix2.count`  | yes | null | 同上 |
| `sort.fix3.count`  | yes | null | 同上 |
| `sort.auto1.pct` … `sort.fix3.pct`（6 個） | no | - | 見第 6 節「不建議接」 |
| `sort.loading` | no | - | 見第 5 節候選 |
| `sort.total`   | no | - | 見第 5 節候選（要繞開已知地雷） |

### `status.*`（60 個：11 個已發布皆 null，49 個未發布）

11 個已發布的是 `WebBridgeTags.cpp` 的 `kUnloadedTags[]` 明列的死源（`powerSaving`
`cleanCount` `ep` `safeDoor` `rtc` `rms` `ftp` `triTemp` `indexTime` `uph`
`saveSummary`），全部誠實回 null。49 個未發布 = 這 11 個各自的 `.tone` 次通道
（11 個，發布端目前不產生 tone） + `status.slot1..19` 及其 `.tone`（38 個，純備用佔位，
`tagmap.js`/`WebBridgeTags.h` 都沒有任何條目提到它們，連「已知未翻譯」都算不上）。

### `temp.*` / `tester.*`（各 1 個 family，一半发布一半沒有）

`temp.pv` `temp.mode` `temp.sv` `temp.soak`（4 個）與 `tester.name`（1 個）都在
`kUnloadedTags[]` 死源清單裡，發布為 null；`temp.led.1..6`、`tester.led.1..6`
（各 6 個，共 12 個）是純 UI 裝飾用的指示燈條，`WebBridgeTags.h`/`tagmap.js` 都沒提過，
沒有對應的機台來源概念。

### `lot.*`（16 個，0 個發布 —— 見第 5 節，這是本次挑出的主候選）

見下方候選清單，含逐一驗證結果。

### `machine.*`（2 個：`machine.state` 已發布 null，`machine.mode` 完全未提及）

`machine.mode` 對應 `tagmap.js` 的 `labARTmode`（`fMain.pgMain.tsMain.Panel2.
Off_lineDisplay.palMainStatus.labARTmode`），寫入點在 golden `main.cpp:20392-20417`。
`WebBridgeTags.h` 已經明文記載 `main.cpp` 是未翻譯檔（同一份 `main.cpp` 也是
`status.indexTime`/`status.uph` 死掉的原因）——`machine.mode` 死於同一個根因，
但**沒有被列進 `kUnloadedTags[]`**，所以它連「已知未翻譯」的身分都沒有，是本次
發現的一個小型衛生問題（見第 7 節）。

### `recipe.*` / `user.*`（各 1 個，皆未發布，本次未深入）

`recipe.current`（`cbSetupFileName`）與 `user.level`（`cbUserSelect`）都需要
`cConfiguration.cpp`（未翻譯，7,808 行）或 setup 目錄枚舉，屬於已知的大範圍翻譯缺口，
本次 recon 時間有限，未逐行驗證，如實列為「未調查」而非「已確認死」。

---

## 5. 結構性發現（本次最大的一個）

**`tagmap.js` 的 44 筆條目跟 `bind.js` 的 DOM registry 是兩套完全獨立的「什麼算一個
tag」定義，彼此沒有互相校驗的機制。**

具體證據：`tagmap.js` 把 `nav.msg` `nav.speed` `nav.calc` `nav.io` `nav.offset`
`nav.tools` `nav.config` `ui.language` `app.exit`（共 9 筆）列為有 `dfm` 對照、
`dir: "w"` 的正式 tag。但追蹤 `chrome.js:29-49` 的實際 DOM 產生碼：

```js
// chrome.js:29-39 -- rail 按鈕迴圈
...RAIL_ITEMS.map(it => h(`button.railbtn...`, {
  type: "button",
  "data-cmd": "nav.open",        // 全部按鈕共用同一個 cmd
  "data-value": it.key,          // 用 data-value 分辨，不是各自的 data-tag
}, icon(it.iconName))),
// chrome.js:43-49 -- 語言 / 離開按鈕
h("button...", { "data-cmd": "ui.language" }, "中"),
h("button...", { "data-cmd": "app.exit" }, icon("exit")),
```

這 9 個按鈕**完全沒有 `data-tag` 屬性**（`widgets.js` 的 `btn()`/選取器都沒被用在
這裡；`chrome.js` 直接手刻 `h()`）。`bind.js:79-103` 的 `register()` 只掃
`[data-tag]`/`[data-tone-tag]`，所以 `boundTags()`（`bind.js:205`）**永遠不會**吐出
`nav.msg` 這 9 個名字——不是「還沒接」，是這 9 個名字在目前的 DOM 產生方式下**結構性
不存在**於 registry 裡，除非改掉 `chrome.js` 讓每顆按鈕各自帶 `data-tag`。

換句話說：`tagmap.js` 的 `coverage()`（第 143-148 行）回報「38/44 已解析 legacy 控制項」，
但這個百分比完全不代表「38 個 tag 在瀏覽器裡是可讀寫的」——其中至少 9 個是純命令
sink，`dfm` 解析得再準，也沒有對應的 `[data-tag]` 節點可以綁定顯示值。兩份文件
（`tagmap.js` 的 dfm 解析度、`bind.js` 的實際 registry）分別由不同的人/波次維護，
目前沒有任何測試或腳本會在其中一份改變時去對另一份報警。

**次要佐證同一個模式**：`temp.apply`（`spbSet` 按鈕）在 `tagmap.js` 裡也有 `dfm`
對照（`dir: "w"`），但 `left.js:49` 呼叫 `btn("Set", { cmd: "temp.apply", ... })`
**沒有傳 `tag` 參數**，所以同樣不會出現在 registry 裡。這不是孤例，是這一批
「按鈕觸發的動作」在 `tagmap.js` 裡系統性地被記成了「tag」，但在程式碼裡它們的真實
形狀是「純 cmd，沒有 tag」。

**建議**（僅供下一波參考，本次未動手）：`tagmap.js` 的 `dir: "w"` 純命令類條目，
若確定永遠不會有 `data-tag`，應該在欄位上明確標注（例如加一個 `hasReadback: false`
或乾脆從 `TAGMAP` 移到獨立的 `COMMAND_MAP`），否則 `coverage()` 的分母會持續把
「已解析 dfm」跟「已在 registry 裡可綁定」混為一談。

---

## 6. 下一批接線候選（≤30，實際列出 7 個，附驗證證據）

**方法論**：只挑「golden 全域已在 V906 翻譯樹裡宣告 + 有實際 Caption/賦值程式碼路徑」
的候選，不是「listed in UNMAPPED_GROUPS 所以隨便猜」。每一項都附 golden 行號與 V906
現況的實測結果，不是臆測。

| # | tag | 來源全域/欄位 | golden 引用 | V906 現況 | liveness key（建議） | getter 形狀 |
|---|---|---|---|---|---|---|
| 1 | `sort.loading` | `LastSet.SendCT[0]` | `cSortCT.cpp:214` `pnlLoader->Caption=LastSet.SendCT[0]` | **高信心**：`LastSet` blob 本來就已經在 `--dry` 下載入（`lastS`，`LastSetLoaded()`），跟 `site.arm.*`/`startmode.value` 同一個資料源，不依賴 `Prod.iTrayType`（這點跟 `sort.*.count` 不同，後者現況全 null 就是卡在 `Prod` 沒配置，`sort.loading` 沒有這個依賴） | `lastS` | `stageInt(snap, "sort.loading", lastS, LastSet.SendCT[0])` |
| 2 | `sort.total` | `RunInfo.iUnloadCount`（= `Sum`） | `cSortCT.cpp:355-380` | **中信心，需繞地雷**：golden 原始公式是對 `iTo3Unload[i]` 逐站加總 `LastSet.BinCT[0][iTo3Unload[i]]`；`iTo3Unload[]` 是 `WebBridgeTags.cpp:464-469` 已經記載的地雷（V906 裡宣告了但從未初始化，讀出全 0，會讓每一站都疊加到同一個 cell）。**不要照翻這個公式**——FW-1b 對 `sort.*.count` 已經做過同樣的決定（直接用 `e3Auto1..e3Fix3` 常數繞開 `iTo3Unload[]`），`sort.total` 應該延續同一個已拍板的作法：`Sum = sort.auto1.count+auto2.count+auto3.count+fix1.count+fix2.count+fix3.count`（用發布端已經算好的 6 個值相加），而不是重新對 golden 的迴圈公式照翻 | 同 `sort.*.count` 的 per-station gate（`lastS && Prod.iTrayType[...] != tNotUse`），或至少 `lastS` | `stageInt(snap, "sort.total", lastS, sumOfSixPublishedCounts)` |
| 3 | `lot.auto1.trayCount` | `iUnloaderTrayCountCal[0]` | `uLotInfo.cpp:6853-6855`；已在 V906 `forms/fLotInfo.cpp:1863-1866` 翻譯為 `TfLotInfo::ReflashInfo()` | **中信心**：`iUnloaderTrayCountCal[MAX_TRACK]` 在 V906 `cmydef.cpp:5459` 已宣告，`asendic_Auto.cpp`/`asendic_Auto2.cpp`/`asendic_Auto_RT.cpp` 三個檔在 V906 樹裡都存在（`ht9045_sm` 函式庫的一部分）。**但本次未逐行核對** golden `asendic_Auto.cpp:545/561/1047/1054` 的遞增邏輯是否已忠實翻譯進 V906 對應檔——這是下一波接線時第一件要做的事，不是可以跳過的細節 | 這是執行期累加的計數器（不是配置 blob），跟 `LastSet` 性質不同；建議比照 `pump.task.*` 的既有慣例，gate 在 `pumping`（沒有 pump 在跑，這個數字沒有意義，跟「有沒有載入」是兩件事） | `stageInt(snap, "lot.auto1.trayCount", pumping, iUnloaderTrayCountCal[0])` |
| 4 | `lot.auto2.trayCount` | `iUnloaderTrayCountCal[1]` | 同上 | 同上 | `pumping` | 同上模式 |
| 5 | `lot.auto3.trayCount` | `iUnloaderTrayCountCal[2]` | 同上 | 同上 | `pumping` | 同上模式 |
| 6 | `lot.loaderLastBundleId` | `asBundleTrayID[ePortLoader]` | `uLotInfo.cpp` 對應處；V906 `forms/fLotInfo.cpp:4070-4074` | **中低信心，客戶限定**：只在 `fAGV->IsSPIL_AMR()==true` 的分支才賦值；`asBundleTrayID` 在 V906 `cmydef.h:5684` 已宣告，`IsSPIL_AMR()` 在 V906 `forms/fAGV.cpp:35` 已翻譯（呼叫 `AGV_IsSPIL_AMR()`，程式碼註解自稱 "confirmed REAL"）。在**非 SPIL 客戶**的開發機上，這條分支不會執行，值會維持初始空字串——這是誠實的空，不是缺陷，但要在 UI 上看到非空值只有 SPIL_AMR 客戶站才有機會 | `cust`（跟 `machine.customerCode` 同一把鑰匙）或更精確地新增 `IsSPIL_AMR()` 專屬 gate | `stageStr(snap, "lot.loaderLastBundleId", cust, asBundleTrayID[ePortLoader])` |
| 7 | `lot.loadercarBundleId` | `asBundleTrayID[ePortEmpty]` | 同上 | 同上 | 同上 | 同上模式 |

**明確不建議放進下一批（附證據，避免下一波重新踩雷）**：

- **`lot.auto1.nowTrayId` / `auto2.nowTrayId` / `auto3.nowTrayId`（3 個）**——
  `grep -rn "labNowAuto1TrayID" HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422/`
  （20260819 執行，範圍含全部 `.cpp`/`.h`，排除 `.svn`）**只找到 `uLotInfo.h:798`
  的宣告，`.cpp` 裡完全沒有任何地方對它賦值**。`labNowAuto2TrayID`/`labNowAuto3TrayID`
  同樣只有宣告。這在 golden 裡本來就是死的（設計時放的 Label，從未被程式碼寫過），
  不是 V906 翻譯缺口——把它翻譯得再完整也不會變成活的，不值得排進下一批。
- **`sort.auto{1,2,3}.pct` / `sort.fix{1,2,3}.pct`（6 個）**——golden 公式
  `RunInfo.sT6AutoYield[i] = ChangeToPercentage(LastSet.BinCT[0][iTo3Unload[i]], Sum)`
  （`cSortCT.cpp:399`）一样吃 `iTo3Unload[]` 地雷，而且還疊了一層「先算對 `sort.total`
  才能算百分比」的相依性。等 `sort.total`（候選 #2）確定用哪個安全公式接了，`pct`
  才有意義去接，順序不能顛倒。

---

## 7. 未做 / 風險

- **本次是靜態程式碼追蹤，不是瀏覽器實測**：環境沒有 headless browser，296 這個
  「綁定總數」是逐檔逐迴圈手動展開後用腳本核對得出的，理論上仍可能漏掉某個
  動態產生路徑（例如某個 panel 在特定條件下才 render 出的節點）。如果之後要做
  gate，建議把這次的展開邏輯轉成真的在瀏覽器（或 jsdom）裡跑 `register(document.body)`
  再讀 `boundTags()`，用真實 DOM 校驗這份靜態分析。
- **`recipe.current` / `user.level` 完全未深入**：本次只確認未發布，沒有去追
  golden 的來源鏈（可能落在未翻譯的 `cConfiguration.cpp`，也可能有更簡單的路徑，
  沒有查證前不敢下結論）。
- **`lot.auto{1,2,3}.trayCount` 的翻譯逐行保真度沒有驗證**：只確認了全域宣告存在、
  `fLotInfo.cpp` 有消費端程式碼、`asendic_Auto*.cpp` 三個檔案存在於 V906 樹，
  **沒有**逐行 diff golden 的遞增邏輯是否原封不動翻過來——候選清單裡已經寫清楚，
  下一波動手前第一步就是做這個 diff。
- **`arm.*`（64 個）、`cat.*`（32 個）、`contact.*`（18 個）、`speed.*`（12 個）、
  `bin.*`（6 個）完全沒有深入調查**，只確認了「未發布」這個既有事實，沒有去找
  各自 golden 來源是否已在 V906 翻譯樹裡可用。這五個 family 合計 130 個 tag，
  是這次 crosswalk 裡最大的未知區塊，值得排一個獨立的偵察波次專門看。
- **`status.slot1..19`（連同 `.tone`，共 38 個）連 `tagmap.js`/`WebBridgeTags.h`
  都没提過**——它們是純備用佔位格，目前沒有任何文件宣稱它們該對應到什麼，
  本檔也不打算猜。
- 本次探針一律 `--dry`（scratch copy of Gerneral.ini，不碰量產共用檔），
  `wb_serve.exe` 已於 40 秒後自然結束（20260819 20:14 啟動，PID 由背景工作管理，
  未殘留常駐行程），未觸發 `ctest`（未與其並行）。
