---
name: fw-wave-loop
description: HT9045 V906 DFM→WEB 戰役（FW CAMPAIGN）的自動波次政策。定義最終目標、三件套交付、選標的規則、驗收 gate、夜間 loop 與額度中斷協議、安全邊界。Use when：執行 /fw-wave、規劃表單→web 波次、tag 接線波、dfm2web 產生器、判斷某表單能不能夜間自動做。關鍵字：FW-W, DFM→WEB, 表單波次, facade, tag 接線, formview, emit_web, WebBridgeTags, 夜間 loop
---

# FW 波次迴圈政策（HT9045 V906 DFM→WEB）

**權威計畫書：`HT9011UC_Cpp_V3.33.906.0/docs/DFM2WEB_CAMPAIGN_PLAN.md`**（§0 定案鏈、
§4 波次佇列、§5 gate、§6 loop 協議、§7 停止條件）。本 skill 是執行摘要；細節以計畫書為準，
**兩邊不一致時以計畫書為準並回頭修這裡**。

## 先讀 pt-wave-loop（單一出處，不複寫）

`pt-wave-loop` skill 的以下段落**全部適用**，動工前載入它：
五個已付代價的陷阱／硬邊界（Big5、EOL、唯讀樹）／「不准自行停下」三規則／
冷啟動協議／額度中斷實測表／「agent 論證比程式碼更常錯」。

## FW 專屬要點

1. **目標**：dfm→web。`.dfm` 走 dfm2rc 既有 IR（解析層 133 表單已完成），
   新 emitter `emit_web.py` 產 `web/forms/<form>.layout.json`；表單業務邏輯忠實翻 C++，
   widget 讀寫改 form-core UI-state 欄位；狀態經 WebBridgeTags（liveness 必備）進瀏覽器。
2. **三件套可分波**：(a) 邏輯翻譯 (b) tag 匯出 (c) web 渲染，各自有 gate（計畫書 §3）。
3. **唯讀是硬邊界**：write path（指令通道）是安全關鍵，夜間永不做，撞到就佇列換標的。
   events handler 一律不接線。
4. **表單 ctor 高危**：form-core ctor 只塞欄位，初始化搬顯式 Init()——陷阱 #4 的
   18 個 NULL 全域在表單區最容易踩。
5. **web 專屬 gate**：e2e 探針（`tools/webprobe`）不與 ctest 並行（WB_TcpLink 撞埠）；
   `wb_publish`/`wb_serve` 一律 `--dry`；收工比對 `D:\HT9045\system` MD5+mtime；
   JS 無編譯器把關→防禦式渲染＋誠實回報「渲染回歸靠人工 F5」。
5b. **驗收 gate 一律用 `tools/dualgate.sh <tag>`（序列版），不要用 `dualgate2.sh`。**
   `dualgate2` 已於 20260827 停用：它把 Debug ctest 疊在 Release build 上，實測把
   `dfm2rc_fidelity` 從單獨跑 565.94s（Debug，通過）／355.37s（Release，通過）
   推過 600s 逾時，**兩側都假紅**，而換到的加速只有 2m25s／37m43（6%）。
   保留該檔只為留紀錄。**「多出 dfm2rc_fidelity 就重跑」不是解法，那是在替這個根因打補丁。**
   ⚠ 這條原本只活在會漂移的心跳提示文字裡（提示說 dualgate2、腳本檔頭說別用它），
   20260828 才落到這份 skill。判定一律由 `tools/gateverdict.sh` 給：
   **`ctest` 對任何失敗數都回 exit 8，exit code 零鑑別力**，只看
   `*_VERDICT` 與 `*_EXTRA`／`*_ABSENT`，並比對**失敗集合逐項**等於常駐五項。
5c. **gate 執行期間不准改任何進入建置的檔**（原始碼與 CMakeLists）。
   20260828 實測代價：波次 agent 在 gate 起跑 26 分鐘後回頭修正自己的行號引用，
   當時 Release 正在編譯 → 物件是改動前後的混合，**整次量測作廢重跑**。
   波次 agent 是在「它的檔案停止變動」時才算結束，**不是在它說結束時**。
   啟動 gate 前一刻記下交付檔 mtime，收工用 `build_<tag>g/cfg.log`（**起跑**錨點，
   不是 sentinel 寫入時刻）比對；任何一個較新就作廢。
5d. **開 gate 前先花 20 秒量環境：單獨跑一次 `dfm2rc_idempotent`。**
   它是純 Python、`do_compile=False`（`run_b1d.py:505`），**零編譯、零 spawn**，
   所以它量到的就是 **build 目錄的檔案 I/O 成本**。
   ```
   build_<tag>r/tests/test_dfm2rc_pipeline.exe idempotent <build_dir>/dfm2rc_regen_idempotent
   ```
   **基線 17-25 秒。它是單向的：綠燈可靠，紅燈不可靠。**
   - **≤40 秒 → 直接開 gate**。128 次中有 105 次落在這裏，**其中只有 1 次 `dfm2rc_fidelity` 失敗**。
   - **>40 秒 → 意思是「未知」，不是「不要開」。**
     實測：>40 秒的 23 次裡 **fidelity 通過 16 次、失敗 7 次（偽陰性 70%）**；
     兩邊的 idempotent 區間幾乎完全重疊（通過 7.3-289.9、失敗 22.7-335.5）。
     此時**改直接量 `dfm2rc_fidelity` 本體**（單獨、無上限，2.5-11 分鐘）：
     ```
     build_<tag>r/tests/test_dfm2rc_pipeline.exe fidelity <build_dir>/dfm2rc_regen_fidelity
     ```
     **<550 秒 → 開 gate；>550 秒 → 等。** 直接量比代理量準，而且仍然比 80 分鐘的 gate 便宜。
   ⚠ **這條規則第一版寫成「>40 秒就不要開 gate」，那是錯的**（寫完 90 分鐘後自己驗出來）：
     照那個寫法，歷史上會有 **16 次本來會綠的 gate 被白白延後**。
     **一個門檻在一個方向可靠，不代表它在另一個方向也可靠。**
   20260828 實測：92 秒（「未知」）-> 直接量 fidelity **363 秒通過** -> 開 gate。
   若照舊規則就會在這裏錯過一次可用的 gate 窗口。
   ⚠ **`dfm2rc_fidelity` 的基線是 ~150 秒，600 秒預算是它的 4 倍。**
   它偶發的紅燈**不是「預算太緊」**，不要去放寬 `tests/CMakeLists.txt:3246`；
   那只是把儀表關掉。實測波中真值 635 秒（`RC=0`、133/133、零問題），
   **只差 35 秒**；而同型的波會**自己過去**（08-27 六連逾時後 08:13 自行恢復）。
   `idempotent` 跟 `rc_compiles` 合看還能**分辨兩種不同成因的事件**：
   `rc_compiles` 偏高而 `idempotent` 正常 = **CPU 競爭**（重疊排程、失控行程）；
   `rc_compiles` 正常而 `idempotent` 爆掉 = **檔案 I/O 風暴**（端點防護 on-access 掃描）。
   詳見 `docs/DEVLOG.md` 20260828 VII。
   ⚠ **通則：連續更正四次通常不是運氣差，是基準沒有建立。**
   沒有基線時，每一個新樣本都會長得像一個新發現。
6. **選標的**：照計畫書 §4 佇列順序（FW-0 基建 → FW-1 tag 接線 → FW-2 產生器 →
   FW-3+ 表單波），每波開工以「使用頻率 × 唯讀可完成度」重評；單波 golden ≤15k 行，
   大表單切塊；`Command.cpp`（TfMain）記表單帳。
7. **完成度三軸不可換算**（census 行數／活 tag 數／layout.json 覆蓋數），
   引用必附分母與單位。

## 夜間 loop 與額度

- 每波收工＝commit + DEVLOG + 🔖RESUME，然後**立即開下一波，不准閒著結束回合**。
- ScheduleWakeup 當保險 + cron 心跳（20 分鐘守衛式）雙保險；
  額度中斷後 cron 自己回來續跑，**前提 session 沒關**。
- session 死了：磁碟上的 commit + RESUME 就是全部狀態，使用者重開後
  `/loop /fw-wave` 冷啟動即無損接續。
