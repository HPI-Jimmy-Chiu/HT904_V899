# RECON — ICPDAS PISO-MN200（Motionnet）：文件/範例/906 功能差/64-bit

> 20260818。使用者指示（第二條硬體線）。SDK＝`C:\ICPDAS\PISO_MN200`。
> 分支標記：`feat/v906-mn200-audit`（未 checkout）。本文件為 recon 代理報告
> （含 35 符號全量連結實測）的落地版。

## 0. 四個決定性結論

1. **64-bit 完全可行**：`Lib_x64\MN200DLL.dll` 與 x86 版**同批建置**（FileVersion
   1.0.18.1／Product 2026.1.12.1、PE 時間戳相同、**exports 皆 192 個**）；
   `Drivers\Win10_amd64` 有簽章 WDM 驅動包。型別安全乾淨（SPEED_PAR/MOTION_IO 全是
   double/BYTE，無 handle-塞-long；LLP64 下 long 語意兩邊一致）。
2. **MinGW 32-bit 主 oracle 可直連出貨的 x86 `MN200DLL.lib`**（實測）：它是
   **MS-COFF import library**（`!<arch>` magic），nm 列得出 906 全部 35 個符號
   （`_mn_open_all@4` 等）；scratchpad 探針 include 樹內真 vendored 標頭、
   對 35 符號各取址，`g++ -m32` 編譯+連結 **exit 0**。x64 lib 用 MSVC 2022 同樣過。
3. **⚠ 這推翻 CMakeLists.txt:50-53 的既有註解**（「MN200DLL.lib 是 32-bit OMF、
   MinGW 無可用」）——OMF 的是 **`MN200BCB.lib`**（`f0 0d` magic，BCB6 用），
   兩檔被混為一談。**但 offline stub（vendor_offline_motionnet.cpp）仍有獨立價值**
   （無卡機器可建置、避免執行期依賴 vendor DLL/驅動）——真連結可行≠該換掉，
   設計取捨留使用者。
4. **版本差異=純增量**：vendored `Motor/vendor/mn200.h`（86 函式）vs SDK
   `MN200DLL.h`（135）——51 個新增（幾乎全是手冊第 13 章 MN-2072T 高速比較器/
   編碼器擴充模組家族＋新診斷函式），**零移除/改名**；906 用的 35 個符號
   逐行簽名相同（僅 4 處 tab/space 差）。

## 1. API 地圖（Function Reference 1.5TC，頁碼=手冊 TOC）

初始化/環路（mn_open_all p.9、mn_start/stop_line p.16-17、mn_set_comm_speed p.15）／
馬達組態（mn_set_motion_cfg p.18、mn_set_softlimit p.27、mn_servo_on p.29）／
Home（mn_home_start p.33、**mn_home_search p.44**——906 自 20220715 改走這個）／
獨立軸（mn_velocity_move p.48、mn_fix_move p.51、mn_stop_move p.54）／
內插（mn_line2/arc2/line3/linen_move p.58-68）／狀態診斷（mn_motion_done p.89、
mn_get_cmd/enccounter p.90-91、mn_get_error_status p.95）／板卡 IO 8DI/4DO
（mn200_get_di/set_do p.104-107）／**Ring/IP/Port 擴充 IO**（mn_get/set_port_bit/byte
p.123-127——906 的 389 點走這組）／類比（mn_set_ao p.132、mn_get_ai p.136）／
MN-2072T 擴充（p.143-186，40 函式，**全在 51 個新增裡**）。

**Datasheet 規格要點**：Universal PCI；Motionnet 2.5/5/10/20 Mbps（20Mbps 100m）；
單 ring 最多 **64 節點**、每節點 64 點、**單 ring 上限 2048 點**；IO 更新
~15.1μs/模組（64 模組全環 ~0.97ms）；編碼器鎖存 ~19.3μs；SW1 DIP 設 Card ID 0-15。
（906 的 IO_MAXIP=64/IO_MAXPort=4×8bit 與規格量級一致；IO_MAXRing=4 的依據未查證。）

## 2. 範例序列（VC/BCB 一致）

`mn_open_all(&lines)` → `mn_stop_line` → `mn_set_comm_speed` → `mn_start_line(&devs)` →
軸控/IO。golden 開卡（cinitial.cpp:7914/8068 兩條路徑，順序略異——一條缺
set_comm_speed/reset，是否刻意未深究）與範例大致吻合。

## 3. 906 功能差（35 用 / 文件全集）

- **906 實際用 35 個 API**（用 vendor_offline_motionnet.cpp 的 stub 註冊清單交叉驗證）。
- **文件有、906（含 golden）沒用**：內插家族大部分（只用 linen）、
  **latch/FIFO/中斷能力從 BCB6 時代就沒接**（TMyMN200Motor 的 ResetLatch/GetLatch*
  是忠實空殼，golden :749-770 同款）、速度/加減速範圍診斷、群組啟停（除 hold）、
  類比輸出與校正、模組層 word/bit 定址整組、mn_load_ezgo_cfg。
- 906 用的符號**零個**落在新增清單——SDK 升級對既有呼叫面零影響。

## 4. 未查證（誠實揭露）

中文手冊敘述文字（內嵌字型無 ToUnicode CMap，只抽得出簽名/頁碼/英文；English 1.2
版比 SDK 舊、不含新函式）；EzGo 手冊未讀；MN-2072T 各函式語意只核名稱；探針 exe
**未執行**（避免無卡阻塞/硬體風險——連結成功只證符號與呼叫慣例，不證執行期行為；
手冊定義 ERROR_NO_CARD_FOUND/ERROR_CARD_ID_DUPLICATED 未實測）；golden 雙開卡路徑
差異原因；ICPDAS 官網更新版手冊。

## 5. V899 註記（決策權在 ht9045-v899，不代評）

golden 樹的 `Motor/MN200BCB.lib`（18,432B）與 SDK `Lib/MN200BCB.lib`（25,088B）
**不同雜湊、SDK 較新（2026/01/12 批）**——V899 若評估升級 MN200 SDK，先比這個檔。

## 6. 決策含意（給使用者）

- MN200 的 64-bit 路**現成**（同批 x64 DLL/lib/驅動）；32-bit MinGW 也能直連真 lib
  ——與 Galil（64-bit only、無工具鏈可碰）成本結構完全相反。
- 若 1203 統一控制方向吃掉 IO（EthcatDI/DO 取代 Motionnet 遠端 IO），MN200 線的
  未來角色要在架構題裡一起定：**保留 Motionnet（升級 SDK 即可）vs 併入 EtherCAT**。
