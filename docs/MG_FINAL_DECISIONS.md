# MG_FINAL_DECISIONS — 最終決策清單（留使用者裁決）

依使用者 20260826 裁定：<95% 信心的項目記在這裡，戰役收尾一次裁決，過程不停。
每條含：叢集／衝突描述／選項／我的建議／若已依 95% 規則先行的動作。

| # | 叢集/案 | 問題 | 選項 | 建議與先行動作 |
|---|---|---|---|---|
| F1 | Multi EP（0430/0504/0511/0526；欣銓 CASE-20260429/0512） | ~~搬不搬 8ch~~ **已被地形解決（MG-W7 實測）**：V910 有公司自製 Multi EP 完整實作（Eastsun 20260525 整合＋RogerYang 8EP 等後續演進，部分比 V899 新）。真缺口僅 6 條（5 條 iosetview AV 防護＋1 條 ContactForce round-trip）已搬；其餘 C 類/公司新功能不可刪，入白名單 | — | **無需裁決**（原問題不存在）；殘餘個案拆成 F3–F6 |
| F3 | adam6024.cpp 0526 兩塊（V899:1627/1670） | **V910 有反向刻意修改**：Eastsun 20260616 把這兩塊逐行註解掉、改用迴圈內版本 | (a) 尊重 V910 不搬 (b) 回搬 V899 版 | **建議 (a) 不搬**（公司刻意改），已照建議執行（未搬） |
| F4 | adam6024.cpp 0511 三條 bounds guard（V899:1587/1595/1601） | V910 迴圈結構全異，補防呆等於重寫（違反 splice 鐵律）；且 V910 預設值下越界不可達 | (a) 不動 (b) 為 V910 結構重寫防呆（新實作波） | **建議 (a) 不動**，已照建議執行 |
| F5 | ContactForce.cpp:453（0526，**閘外**） | 照搬會讓 INDIVIAL 機台既有 `Diameter_20.000mm_*` 校正群組全部孤兒化回預設 | (a) 不搬 (b) 只在 MULTI 模式鏡射（需另開實作波） | **建議 (a) 不搬**；若欣銓日後真開 MULTI 再議 (b)，已照 (a) 執行 |
| F6 | ContactForce.cpp:884（0430，閘外、純外觀） | INDIVIAL 時 V910 多顯示一個空分頁；搬移需覆寫 Eastsun 區塊，信心 90% | (a) 不搬 (b) 搬 | **建議 (a) 不搬**（外觀差異 vs 覆寫公司區塊的風險不成比例），已照建議執行 |
| F7 | MG-W9 發現：V910 的 `FUNC_CC_ARDENTEC()` 比 V899 少三行（bTrayDeviceCheckFromLoader／bTrayOCR／bFTPFunction，皆非 899 期矩陣範圍） | 後果：本波搬入的 acatchtray case 148 修正在 V910 的**欣銓機台跑不到**（IsRunDeviceRemainLaser 需 bTrayDeviceCheckFromLoader==true；其他有開此 profile 的客戶會生效） | (a) 維持只搬矩陣範圍（現狀） (b) 把三行也補進 V910（超出 899 期範圍） | **建議 (a)**；若欣銓在 V910 機台需要此功能，再以獨立需求補 (b)，已照 (a) 執行 |
| F8 | MG-W9 acatchtray 3 條為**閘外變更**（V899 最終態本就無閘） | 新增一次 MLoaderY MotorMove＋三顆氣缸釋放（D 類動作），但需四道獨立閘全開才會走到（USE_TRAY_MAPPING==etmDeviceRemain＋客戶 profile＋UI 勾選＋INSTALL_OCR_YMot） | (a) 忠實照搬（已執行） (b) 加閘（=新寫碼，違反 splice 鐵律） | **建議 (a)**，已照建議執行；blast radius 已量化記錄於 mg_w9_analysis.md |
| F9 | watchdog 本體的掛鐘缺陷隨波帶進 V910（TQPF_Timer 純 QPC 無 stop 感知；WAR09102/09107 訊息分家未做）＝V899 已知缺陷 D5 | 忠實搬運原則 vs 已知缺陷擴散 | (a) 忠實搬運（已執行），要修另開 V899 案兩樹同步 (b) 搬移時順手修（違反忠實原則） | **建議 (a)**，已照建議執行 |
| F10 | MG-W11 的 OP6b（AutoClean 0407「kit 無 pad 防卡死」，**信心 92% 故本波未搬**） | 搬入後：非 KYEC 修好「kit 全空時 Task 卡 10 永久卡站」；但 CC_KYEC_LEE 的 ShowMyMessage（會設停機）觸發條件由 count==0 反轉為 count!=0——「kit 只剩用過 pad」情境從安靜變跳停機彈窗 | (a) 忠實搬運（V899 出貨現狀，KYEC 行為變化=V899 已接受的行為） (b) 搬運＋KYEC 條件維持 V910 舊式（新寫碼） (c) 不搬（V910 保留卡站缺陷） | **建議 (a)**；已實測單獨補搬不影響其餘 16 op（splice op 在 docs/mg_w11_ops.json 的 OP6b，可單獨執行）。等你點頭後一個 op 收尾 |
| F2 | MG-W5 已搬（9c7b8e1 之後的 W5 commit）：HS_Function ProdLog/GPIBLog「跨午夜換昨天」（0415） | V899 的 swap 是**無條件**的：修好 method 0（00:00 上傳抓昨天檔=正確），但 method 1（20:00）/method 2（每整點）/method 3 會從「抓今天」變「抓昨天」，且與同函式 JamStat 的 hour-aware 基準不一致。V899 出貨版現況即如此（忠實搬運已完成，V910=V899 行為一致） | (a) 維持忠實現狀 (b) 改成 hour-aware（只在 00:00 附近才換昨天）——屬**改 V899 行為**，兩樹要一起改 | **建議 (a) 維持**；若 KYEC/矽格實際在用 method 1/2 且反映抓錯日，再開 V899 案處理（那是 V899 議題非搬移議題） |
