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
