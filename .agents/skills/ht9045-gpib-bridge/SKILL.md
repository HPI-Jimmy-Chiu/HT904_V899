---
name: ht9045-gpib-bridge
description: HT9045 / HT9046LS 的 GPIB 橋接程式（H9046_32GPIB.exe，原始碼在 d:\GPIB9045）知識庫。此程式是「ATE 測試機（Tester / pgm / MES）」與「HT9045 Handler」之間的 GPIB ↔ WM_COPYDATA 橋接器，負責解析 Tester 送來的 GPIB 字串指令、轉成 MSG_CMD_* 送給 Handler、再把 Handler 回應原樣寫回 Tester。當使用者詢問 GPIB 指令、SETTEMP/SETTEMP?/SETSOAK/SETSITEMAP、H9046_32GPIB、TSerialPoll、MyGPIBWrite、SendMSG_CMD、SetParameter、ibwrt/ibrd、Tester 溫度比對失敗、Handler Temp=Settemp、Temperature check fail、溫度回應格式、各 Tester 廠牌（Advantest/Flex/93K/SPEA/RFMD/Qorvo/Delta Castle/DOOSAN/Novatek）指令差異時，應先載入此技能。關鍵字：GPIB, H9046_32GPIB, GPIB9045, SETTEMP, SETTEMP?, SETSOAK, SETSITEMAP, MyGPIBWrite, SendMSG_CMD, SetParameter, TSerialPoll, ibwrt, ibrd, MSG_CMD, Settemp +25.0, Temperature check fail, iI38SETTEMPRespondSetTemp, Tester, ATE, pgm, MES Temp。
---

# HT9045 GPIB 橋接程式（H9046_32GPIB）知識庫

## 1. 這支程式是什麼

- **輸出檔**：`H9046_32GPIB.exe`（與 HT9045.exe 是**兩支獨立程式**）
- **原始碼根目錄**：`d:\GPIB9045`（**不是** HT9045 版本目錄）
- **目前分析/可寫版本**：`d:\GPIB9045\GPIB_Code_32Site_V12.13.883.0_20250915_Jimmy_20250924\`
- **角色**：ATE 測試機（Tester / pgm / MES）⇄ **GPIB 橋接** ⇄ HT9045 Handler
- **編碼**：Big5（CP950），BCB6 / VCL，pre-C++11，與 HT9045 同樣限制
- 版本命名：`GPIB_Code_32Site_<Version>_<Date>[_<Owner>]`；其餘版本目錄、`back/`、`Backup/`、`H9046_32GPIB_V12*/` 一律唯讀僅供比對。
- GPIB 版本字串定義在 `MessageDef.cpp`（`GPIBVersion`）。

## 2. 兩條通訊鏈路

```
 [ATE Tester / pgm / MES]
        │  GPIB 字串指令 (ibwrt/ibrd)      ▲ MyGPIBWrite 原樣回寫
        ▼                                  │
 ┌──────────────────────────────────────────────┐
 │  H9046_32GPIB.exe  (TSerialPoll, Main.cpp)     │
 │  Timer1Timer 輪詢 → ProcessStatusString 派發    │
 └──────────────────────────────────────────────┘
        │  SendMSG_CMD → WM_COPYDATA       ▲ Handler 回 GHandler2Gpib->Message
        ▼  (GGpib2Handler)                 │  (ProcessAddress)
 [HT9045 Handler]  main.cpp:15369... + Command.cpp Write*Data()
```

| 方向 | 機制 | 關鍵函式 (Main.cpp) |
|------|------|------|
| Tester→Bridge 收指令 | GPIB `ibwrt/ibrd`，`Timer1Timer` 輪詢 | `ProcessStatusString()`（大型 if-else 指令派發鏈） |
| Bridge→Tester 回寫 | GPIB `ibwrt`（可選自動補 `\r\n`） | `MyGPIBWrite(str, Task)`（約 4588 行） |
| Bridge→Handler 送命令 | Windows `WM_COPYDATA` → `HMountWnd` | `SendMSG_CMD(CMD[, Message])`（約 4798 行），封包 `GGpib2Handler` |
| Handler→Bridge 收回應 | `WM_COPYDATA` 回呼 | `ProcessAddress()`（約 3400~3887），封包 `GHandler2Gpib` |

> **回應原樣轉發（重要）**：對查詢類指令，Bridge 在 `ProcessAddress()` 用
> `buffer.sprintf("%s", GHandler2Gpib->Message); MyGPIBWrite(buffer, sGbibTask);`
> （Main.cpp:3652-3653）把 **Handler 回的字串原封不動**寫回 Tester。
> ⇒ 回應字串的「內容/格式」由 **Handler** 決定，Bridge 不加料、不改格式。

## 3. GPIB 溫度相關指令（Tester → Bridge → Handler）

於 `ProcessStatusString()`（Main.cpp，約 5200~7300，含多套 Tester 廠牌分支）以
`Str.Pos("XXX")==1` 比對（**大小寫敏感**，呼叫前 `Str` 已 `strupr` 轉大寫）：

| Tester 送的 GPIB 字串 | 派送 MSG_CMD_* | 行為 | 位置 |
|------------------------|----------------|------|------|
| `SETTEMP?` | `MSG_CMD_HandlerTemperature` (65) | 查詢 Handler 溫度 | Main.cpp:5300, 7238 |
| `SETTEMP +<n>` / `SETTEMP_<n>` | `MSG_CMD_SetTemp` (73) | 設定 Handler 溫度，送 `TempStr` | Main.cpp:5305 |
| `SETTESTTEMP +<n>` | `MSG_CMD_SetTestTemp` (104) | 設定測試溫度 | Main.cpp:5310 |
| `DEVICETEMP` / `SETTESTOFFSET_` | `MSG_CMD_SetTJ` (75) | Tj/偏移（TSMC） | Main.cpp:5425 |
| `SETSOAK?` / `SETSOAK ` / `SETSOAK_` | `HandlerSoakTime`(64)/`SetSoakTime`(74) | 浸泡時間 | Main.cpp:5315 |
| `GETNOWALLTEMP?` | `MSG_CMD_GetNowAllTemp` (69) | 取全部溫度 | — |
| `SETPOINT?...ZONE` / `MASSTEMP?...ZONE` | `HandlerTemperature`/`ActualTemp` | Delta Castle 專用 | Main.cpp:7131 |

**指令清單 `slCmdList`**（Main.cpp:120~196）的註冊順序索引 = `MessageDef.cpp` 的 `MSG_CMD_*` 數值，兩者必須一一對應（如 `SetTemp`=73）。

### SetParameter() — 前綴剝離（Main.cpp:6358）
把 Tester 字串去掉指令前綴、存進 `TempStr` / `Sitemapstr` 成員：

| 前綴 | `Delete(1,N)` | 存入 |
|------|---------------|------|
| `SETTEMP +` | 9 | `TempStr` |
| `SETTEMP_` | 8 | `TempStr` |
| `SETSOAK_`/`SETSOAK` | 8/7 | `TempStr` |
| `SETSITEMAP_`/`SETSITEMAP ` | 11 | `Sitemapstr` |

⚠️ 注意：`SetParameter()` 內用 `AnsiString.Pos("SETTEMP +")` 屬**大小寫敏感**；呼叫端
（Main.cpp:1571/1812/2022）傳入的是「原始大小寫」buffer，而指令派發用的是已 `strupr` 的字串——
分析「設定值沒被正確剝離」類問題時要留意這個不一致。

## 4. ⭐ 溫度比對失敗 / 「Handler Temp=Settemp +25.0」診斷

**典型客訴 log（由 Tester 端 pgm / MES 程式輸出，非 Handler、非 GPIB 程式）：**
```
pgm or MES Temp=25 , Handler Temp=Settemp +25.0
Temperature check fail, please check setting degree !!
```

**完整資料流與根因：**
1. Tester pgm 送 `SETTEMP?` 查 Handler 溫度。
2. GPIB Bridge → `MSG_CMD_HandlerTemperature` → Handler。
3. Handler 在 `TfMain::TempDataStrings()`（**HT9045** `Command.cpp:1533`）組回應字串，
   依 INI 旗標 `IniConfig.iI38SETTEMPRespondSetTemp` 決定格式：

   | 旗標值 | 回應字串 | 對應 Tester |
   |--------|----------|-------------|
   | `0`（預設/多數機台）| `+25.0` | 期望純數值的 Tester |
   | **`1`** | **`Settemp +25.0`** | kevin 20180308 特定客戶需求 |
   | `2` | `25`（整數）| DOOSAN TESNA（Steven 20250701）|

4. GPIB Bridge 把該字串**原樣**寫回 Tester（Main.cpp:3652）。
5. 若 Tester pgm 期望純數值卻收到 `Settemp +25.0` → 解析失敗 → `Temperature check fail`。

**根因＝ Handler 端設定旗標被設成 1，不是 GPIB 程式、也不是 Handler 程式 bug。**
這完全解釋「其他台不會有 Settemp 文字」（其他台 = 0）。

**修正（設定層，非改 code）：**
- 設定區段/鍵：HT9045 `[Tester] bI38SETTEMPRespondSetTemp`，把 `1` 改成 `0`（或依 Tester 需求改 `2`）。
  - 註冊：HT9045 `cConfiguration.cpp:2633`（預設 0）；實機值在 `config/config.ini` 的 `bI38SETTEMPRespondSetTemp=`。
- 也可透過 SECS **ECID 35548**「[I38] Format of SETTEMP?」設定（`SECSGEM/uHGemHT9045_EC.cpp:1433`）。
- 改完用 `SETTEMP?` 實測回應字串應為 `+25.0`（無 `Settemp` 前綴）。

> 排查順序：先確認回應字串是「Bridge 原樣轉發」→ 鎖定 Handler `TempDataStrings()` → 查 `bI38SETTEMPRespondSetTemp` 與 Tester 期望格式是否相符。GPIB 程式任何版本都**不會**自行加 `SETTEMP/Settemp` 前綴。

## 5. Tester 廠牌 / 客製旗標（cmydef.h, general.ini）

不同 Tester 走不同指令子集（`ProcessStatusString` 內分支或獨立處理函式）：
Advantest / Flex / 93K / SPEA（標準）、RFMD/Qorvo（`QRM?`/`QRA?`）、Delta Castle（`SETPOINT?`/`MASSTEMP?`）、
DOOSAN TESNA（`DUTCHK?`）、Ampere（`GetFFC?`）、Novatek（多筆查詢指令）。
影響行為的常見旗標：`i2DIDFormat`(eStandard/eAMD/eIntel)、`bRETURN_GPIB_VERSION`、`bGPIBWriteWithout_r_n`、`iTesterMode`(InterfaceType_*)、`bRunHANA_ART`。

## 6. 檔案清單（V12.13.883.0_20250915_Jimmy_20250924）

| 檔案 | 用途 |
|------|------|
| `H9046_32GPIB.cpp` | 進入點 WinMain、建立表單 |
| `H9046_32GPIB.bpr` | BCB6 專案檔 |
| `Main.cpp` / `Main.h` / `Main.dfm` | 核心 `TSerialPoll`：GPIB 輪詢、指令派發、Handler 通訊 |
| `MessageDef.cpp` / `.h` | `MSG_CMD_*` 常數與編號、版本字串 |
| `cmydef.cpp` / `.h` | 全域變數、客戶代碼、`LAST_GENERAL_SET` 設定、INI 讀寫 |
| `MyDutPanel.*` | 32 站點 UI 面板 |
| `DummyArt.*` | ART 模擬器（`bDummyART`）|
| `RS232.*` | RS232 / AMD 通訊面板（ATC 客戶）|
| `myTimer.*` | 計時器輔助 |
| `Decl-32.h` | NI GPIB 庫函式宣告（`ibwrt/ibrd/ibrsv/ibwait` 等）|

## 7. 跨技能連動

- Handler 端溫度設定/回報、`SetTemp`/`ChangeTempMode`：HT9045 `Command.cpp`、`csystem.cpp`、溫控模組。
- SECS 對應（ECID 35548 等）：載入 `ht9045-secsgem`。
- HANA / ART 自動化溫度比對（`Handler Temp : %f`，`HANA_ART.cpp`）與本案 Tester pgm 比對是**不同來源**，勿混淆。
