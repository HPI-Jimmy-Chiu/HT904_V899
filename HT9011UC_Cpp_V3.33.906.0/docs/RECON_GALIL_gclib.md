# RECON — Galil gclib（vs 906 現用 DMC32/DMCCOM 舊世代）

> 20260818。使用者指示（第三條硬體線）：Galil 卡控制，四重點=技術文件/範例/906 功能差/64-bit。
> SDK＝`C:\Program Files\Galil\gclib`。分支標記：`feat/v906-galil-gclib-audit`（未 checkout）。
> 本文件為 recon 代理報告（含實測位元數驗證與最小連結測試）的落地版。

## 0. 最重要的三個結論

1. **這份 gclib 安裝是 64-bit only**：`gclib.dll`/`gclibo.dll`（PE32+ x86-64，`file` 驗證）
   ＋ `.a`/`.lib` 四個匯入庫（抽出成員讀 COFF machine=0x8664，兩種獨立方法互證）。
   本機**沒有任何 32-bit 版本**；官方是否另供 32-bit 安裝包**未查**（離線 recon）。
2. **目前專案沒有任何工具鏈碰得到 gclib**：主 oracle 是 MinGW.org GCC 6.3（`--target=mingw32`
   `--with-arch=i586`，架構上 32-bit only，無 -m64）；MSVC 模式已於 20260817 從 build.bat
   移除（`build.bat:6-8`）且本機無 cl.exe。要動 gclib 得先有 64-bit 工具鏈（MinGW-w64 或
   恢復 MSVC）——**前置缺口是「零」不是「弱」**。
3. **`gclib_compat.h` 不是 DMC32 橋接層**：它是 gclib 自己 2.2.1 以下舊 C API
   （GOpen/GCommand…）對現行 `gclib_*` API 的相容層；對 906 實際用的 7 個 `DMC*` 函式
   **覆蓋率零**（無一同名同簽名）。

## 1. 906 現用面（golden 驗證）

`Motor/myGALILmotor.cpp` 全部 Galil 存取收斂在 **7 個 DMC32 進入點**（獨立重跑 grep 核實）：
`DMCOpen(卡號,HWND,&h)`/:3097、`DMCCommand`（30 呼叫點）、`DMCClose`、`DMCReset`、
`DMCClear`、`DMCDiagnosticsOff`、`DMCGetControllerDesc`。`HANDLEDMC`=LONG、
`GALILCALL`=__stdcall、**HWND 訊息幫浦式非同步**（DMCCOM.H:444）。
mymotor.h 的 ~48 個 `Gali_*` 成員全是這 7 個進入點堆出的高階邏輯。
port 現況：`Motor/vendor_offline_galil.cpp` 已 stub 全部 7 個（回 DMCERROR_CONTROLLER=-3，
刻意非 0），`ht9045_motor` 已接上（CMakeLists:869-870）；**全樹 gclib 引用=零**。
golden 的 `Motor/Dmc32b.lib` 是 Borland OMF 32-bit 匯入庫（DMC32.dll 本體不在 repo）。

## 2. gclib API 地圖（現行 vs 舊式雙軌）

| 主題 | 現行（gclib 3.x） | 舊式（GCLIB_DEPRECATED） |
|---|---|---|
| 連線 | `gclib_open(&h, "位址字串")`（gclib.h:260；PCI="GALILPCI1"/IP/COM） | `GOpen`（gclib_compat.h:106，逾時預設 5000ms） |
| 指令 | `gclib_command(h,cmd,buf,len)`（:358）——**DMC 指令字串模型** | `GCommand`（compat:168） |
| 非同步 | `gclib_subscribe_messages/interrupts/data_records`（callback 於**獨立執行緒**；gclib thread-safe） | `GInterrupt`/`GMessage`/`GRecord`（阻塞式） |
| 運動輔助 | **無非 deprecated 高階函式**——GMotionComplete（gclibo.h:213）整檔 deprecated；官方建議改用 data record/interrupt 判停（文件提的範例函式 x_dr/x_ei_motioncomplete **未隨附**） | gclibo.h 全檔 |
| 資料記錄 | `gclib_data_record`/具名欄位存取（gclib.h:554,624-699）；佈局=gclib_record.h `#pragma pack(1)` union（8 種控制器各一 struct） | `GRecord` |
| 錯誤 | `gclib_result` enum＋`gclib_error(h)` 內部字串；**官方範例慣例**：COMMAND_ERROR 時再送 `TC1` 問控制器錯誤碼（errors.c:10-15） | 負數 GReturn＋`_S` 字串巨集 |

範例走讀重點（examples/c）：connection.c **連 gclib_open 回傳都不檢查**（示範碼無防呆）；
record.c 名字騙人——做的是 RA/RC/RD 陣列輪詢不是 DR/QR；CMakeLists 無任何位元數設定
（隱含假設主機工具鏈位元數正確——本專案不成立）。

## 3. DMC32 → gclib 遷移對照（覆蓋率判定）

| golden DMC* | gclib 對應 | 機械式？ |
|---|---|---|
| DMCCommand | gclib_command | 接近（型別/const 差異，要薄包一層） |
| DMCClose | gclib_close | 可視為機械式 |
| DMCOpen(卡號,HWND,&h) | gclib_open(&h,"GALILPCI1") | **否——結構性**：HWND 通知模型→callback+執行緒模型，非同步邏輯整段重寫 |
| DMCReset | 無 API，自己送 "RS" 指令 | 否 |
| DMCClear / DMCDiagnosticsOff | 無對應（未查證窮盡） | 否 |
| DMCGetControllerDesc(第N卡) | gclib_addresses 列舉後自行比對 | 否（語意從按卡號查→列舉） |

**安全註記**：`ScanIndexMotorCanMove`/`DoIndexVectorMove`（myGALILmotor.h:51-52）的
「移動前逐軸 alarm 閘」建立在 DMC32 同步輪詢慣例上——改 gclib 非同步模型後互鎖時機要
重新設計，**屬運動控制安全關鍵變更，不是 API 移植**。

## 4. 兩個要進 KNOWLEDGE 的陷阱（實測）

1. **64/32 位元數不合的連結失敗長得像「符號不存在」**：ld 對讀不懂的 archive member
   靜默跳過→`undefined reference to GOpen`，不是 format error。查位元數要用
   `file` + 抽成員讀 COFF machine，32-bit binutils 的 nm 對 x64 庫直接 File format not recognized。
2. **gclib 文件對 DMC32/DMCCOM 隻字未提**（doc/html grep dmc32/migrat 零命中）——
   沒有官方遷移指引可抄，對照表只能自己維護（=本文件 §3）。

## 5. 未查證清單（誠實揭露）

doc/gclib.pdf 未讀（讀了同源 doc/html）；examples 7 對檔未逐行；Galil 官網 32-bit 版
存在性未查（離線）；PCI kernel driver 位元數未驗（NSIS 殼是 32-bit 不代表驅動位元數）；
`gclib_open` 無卡逾時行為未實測（無可跑工具鏈）；DMCClear/DMCDiagnosticsOff 的 golden
呼叫現場語意未逐點讀。

## 6. 決策含意（給使用者）

- Galil 線的**先決條件是 64-bit 工具鏈**（MinGW-w64 或恢復 MSVC）——與 PCIE-1203 線
  （32-bit lib 齊備、隨時可開）**成本結構完全不同**。
- 若 906 最終方向是 PCIE-1203 統一控制（20260818 裁決），Galil index 馬達×4 改掛
  EtherCAT 從站的話，**gclib 遷移可能整個免做**——這是架構層面的取捨，建議先定
  「index 馬達上不上 1203 匯流排」再決定要不要投資 gclib。
