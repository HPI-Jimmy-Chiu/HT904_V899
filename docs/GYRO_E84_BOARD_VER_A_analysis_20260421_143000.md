# GYRO\_E84-BOARD-VER\_A 控制器規格書分析
> 原始文件：`Gyro system EQ_E84 1 to2 操作手冊_20250801.pdf`
> 文件日期：2025-08-01
> 分析日期：2026-04-21

---

## 1. 文件深度解析

### 1.1 硬體總覽

| 項目 | 規格 |
|------|------|
| 產品型號 | `GYRO_E84-BOARD-VER_A` |
| 電源 | DC 24 V（SPEC 10–30 V），1000 mA，DC-Jack 5.5×2.1 mm |
| Sensor 介面 A | DB25 母頭（對應 SEMI E84 標準 Figure 35 Pin 定義，支援 HOKUYO／CANTOPS sensor） |
| Sensor 介面 B | RJ-11 ×2（IR 轉 DB25，需搭配 DB25 擴充板） |
| PC 介面 | 標準 RS-232 9-pin（115200 8N1） |
| 輸入 I/O | 12 組可組態 I/O（P1~P24）＋ 1 組固定 Relay（P25/P26） |
| 對應信號 | PLACEMENT (PL) / PRESENCE (PS) / CLAMP / Light Curtain / EQ\_ER / ES / DONE / 開門 / 關門 / Relay |

### 1.2 I/O 組態矩陣（P1~P26）

| Pin 組 | 預設用途 | 備註 |
|-------|----------|------|
| P1~P12 (6 組) | PS / PL（可交換） | 對應 CS0 或 CS1 的 PS1–4、PL1–4 等 |
| P13/P14, P15/P16 | DONE / 開門 / 關門 | 2 組 |
| P17/P18 | Light Curtain | — |
| P19/P20 | EQ\_ER | — |
| P21/P22 | CLAMP | — |
| P23/P24 | ES（Emergency Stop） | — |
| P25/P26 | Relay（固定） | 內部第 13 組 |
| Sensor 極性 | 支援 NPN 與 PNP 混用 | 以命令 `0x8018` / `0x8103` 設定 |

### 1.3 SEMI E84 標準時序（文件附圖 12/13 為單一 handoff，圖 18 為 continuous handoff）

| 階段 | passive → active 信號 | active → passive 信號 | 代表 Timer |
|------|----------------------|----------------------|-----------|
| 初始 | HO\_AVBL = 1, ES = 1 | — | — |
| Ready | READY ↑ | — | 由 Handler 維持 |
| handoff 起始 | L\_REQ ↑ 或 U\_REQ ↑ | CS_0 / CS_1 ↑ → VALID ↑ | TA1 / TD0 |
| 移載中 | — | TR\_REQ ↑ → BUSY ↑ | TP1–TP3 |
| 完成 | L\_REQ ↓ / U\_REQ ↓ | COMPT ↑, CONT (continuous) | TP4–TP5 |
| 釋放 | READY ↓ | CS ↓, VALID ↓ | TP6 / TD1 |

> 文件將雙 Load Port 之 CS\_0（Left = Port 0）、CS\_1（Right = Port 1）做為 LP 選擇信號，LP 模式可透過 `0x8114` 設為 Normal / In-only / Out-only。

### 1.4 RS-232 封包格式

```
Send    : [0x55][0xAA][CMD_Hi][CMD_Lo][DAT_Hi][DAT_Lo][CHK]
Receive : [0xAA][0x55][CMD_Hi][CMD_Lo][DAT_Hi][DAT_Lo][STATUS][CHK]
```

- **CMD 高位元**：`0x8x` 為寫入 / `0x0x` 為讀取（bit7 = 寫旗標）。
- **CHK**：所有 byte 相加後保留低 8-bit。
- **STATUS**：
  - `0x00` 設定成功 / 可自動恢復
  - `0x01` 可自動恢復
  - `0x02` 條件不允許
  - `0x03` 無此指令
  - `0x04` 超過設定值
  - `0x05` 需手動 RESET
  - CS1 側之狀態會再加 `0x10`（故 CS1 的「需手動 RESET」= `0x15`）

### 1.5 指令總覽（重點速查）

| 分類 | 指令 | 功能 |
|------|------|------|
| 通用 | `0x0000` | 讀韌體版本（VER:2001） |
| 通用 | `0x8001` | 通訊介面切換 DB25 / RJ6 / RJ7 |
| 通用 | `0x8002` | CS0 / CS1 Alarm Reset |
| 通用 | `0x8003` | CS0 / CS1 Manual / Auto 切換 |
| I/O 讀取 | `0x0010` / `0x0011` / `0x0012` | OUT / IN / MODE 狀態 bitmap |
| I/O 偵測來源 | `0x8014` / `0x8017` / `0x8102` | ES/Clamp/Light/EQ\_ER／PL／PS 內外部偵測 |
| I/O 極性 | `0x8015` / `0x8018` / `0x8103` | 反向／正向、NPN / PNP |
| I/O 強制 | `0x8016` / `0x8019` / `0x8104` | 強制 ON / OFF（需先切內部偵測） |
| I/O 啟用 | `0x801a` / `0x8105` | PL / PS 啟用旗標 |
| Ready 握手 | `0x801f` / `0x8020` | 等待 Ready 功能 Enable／觸發 Ready ON |
| Timer | `0x8040`–`0x8045` | TP1–TP6（0–999 s） |
| Timer | `0x8046` / `0x8047` | PS／PL OFF 感應時間（0.1–10 s） |
| Timer | `0x8050` / `0x8051` | Link timeout（RS-232 通訊看門狗，2–60 s） |
| Timer | `0x8055` | TD0（0.2–10 s） |
| Timer | `0x8056` | DB25 Delay Time（0–60 s） |
| LP 模式 | `0x8114` | CS0 / CS1：Normal / In Port / Out Port |
| 其他 | `0x8022` | Alarm Bypass |
| Relay | `0x8124` / `0x8125` | 內部 Relay 檢測 / 強制控制 |
| 回報 | `0x0070` | E84 raw 信號翻譯事件（需 `0x8013=0x55AA` 解碼） |
| 回報 | `0x0071` | 應用層事件（Ready to Load/Unload、CLAMP、PS/PL、Door、Relay 等） |
| 異常 | `0x0080` | Alarm code（含 0x0000–0x00BC、0x1000-、0x2000-、0x5000- 四大群） |

### 1.6 Alarm 碼分群

| 前綴 | 意義 |
|------|------|
| `0x0000–0x00BC` | Unload 流程各 handshake 步驟異常 |
| `0x1000–0x10BD` | Load 流程各 handshake 步驟異常 |
| `0x2001–0x20D7` | CONT（continuous handoff）相關異常 |
| `0x5000–0x5021` | Timer（TP/TD/Link）及裝置層異常（Door、PS/PL check、Relay、Clamp、ES 等） |

- 加 `0x1000` ? Load 流程；加 `0x2000` ? CONT 流程；加 `0x5000` ? Timer／裝置。
- CS1 側的 STATUS byte 高位再 `+0x10`。
- SignalsAlarm 可自動恢復（`0x00`）或需手動 RESET（`0x05`），取決於是否影響安全（ES/TP3/TP4/BUSY 後段幾乎全是 `0x05`）。

---

## 2. HT9045 程式碼 E84 變數使用盤點

在 V899 版本目錄 `HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260421/` 內比對：

### 2.1 三組變數命名

| 群組 | 變數 | 實際用途 |
|------|------|----------|
| A. **無 prefix** | `SnE84VALID / CS0 / CS1 / AMAVBL / TRREQ / BUSY / COMPT / CONT / GO` | active 側信號，由 [MR/RFID.cpp](HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260421/MR/RFID.cpp) 單埠使用（MR/OHT handshake） |
| A'. **驗證用** | `SnE84LREQ / UREQ / VA / READY / VS0 / VS1 / HOAVBL / ES / POWER` | passive 側輸出**回讀**驗證用，**目前無控制邏輯引用** |
| B. **雙埠 `_1_`/`_2_`** | `SnE84_1_* / SnE84_2_*` | 由 [Automation/AGV.cpp](HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260421/Automation/AGV.cpp) 使用，涵蓋雙 LP 完整 handshake 狀態機 |

### 2.2 使用點證據

- [cmydef.h L1248](HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260421/cmydef.h#L1248)：`SnE84LREQ` 宣告，上方註解明寫 `//E84驗證用`。
- [cmydef.cpp L1362-L1370](HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260421/cmydef.cpp#L1362-L1370)：只給予 ID 常數（474–482）。
- [cinitial.cpp L2103-L2111](HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260421/cinitial.cpp#L2103-L2111)：只設定 `.Name` 字串，以便在 IO 監控畫面顯示。
- 專案全域搜尋 `\bSnE84(LREQ|UREQ|VA|READY|VS0|VS1|HOAVBL|ES|POWER)\b`：**除了上述三處，無任何 `Sen[...]` 讀取或條件判斷**。
- 對照組 [MR/RFID.cpp L300-L460](HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260421/MR/RFID.cpp#L300-L460) 及 [Automation/AGV.cpp L245-L511](HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260421/Automation/AGV.cpp#L245-L511)：反覆讀取 `SnE84GO / CS0 / VALID / TRREQ / BUSY / COMPT` 等信號，完整走 E84 state machine。

### 2.3 結論

> ? `SnE84LREQ / UREQ / VA / READY / VS0 / VS1 / HOAVBL / ES / POWER` 這 9 顆屬於 Handler **passive 端輸出信號的回讀 sensor**，目前在 V899 只完成宣告、ID 指派與 Name 命名，**尚未接入 state machine 做實際判斷**（dead / reserved）。保留這些變數的原因通常是：
> 1. 方便 Layout / IO 監控畫面顯示。
> 2. 作為輸出迴讀（self-diagnostic feedback）保留擴充餘地，日後可加「輸出指令後讀 sensor 驗證」邏輯。
>
> ? `SnE84_1_*` 與 `SnE84_2_*` 這組**才是現役**（AGV/OHT 雙 LP 流程）；
> ? 無 prefix 的那組則是 MR（RFID）模組 single-port 流程在用。

> ? 建議：若要移除 dead code，可在 V899+ 的 IO\_Table.csv 保留對應槽位但於 `cmydef.h/cpp` 加註 `// AI(remove-candidate) 20260421: 僅 IO 監控用，未進入控制邏輯`，避免誤刪造成 IO 索引錯位（sensor index 484 後的項目全部會位移）。

---

## 3. 文件審視（矛盾／E84 協議落差／需 Double-Check）

### 3.1 與 SEMI E84 標準的落差

| # | 文件內容 | 標準對照 | 風險 |
|---|---------|---------|------|
| 1 | 定義 **TP6 timeout** (`0x8045`) | SEMI E84 只定義 **TP1–TP5**（passive）與 **TA1–TA3**（active）、**TD0/TD1** | TP6 為自定義擴充項，與他廠設備對接時若對方按標準設計會無對應欄位 |
| 2 | **TD1 delay** 未列入設定命令 | SEMI E84 圖 18 continuous handoff 有 TD1 | 若連續 handoff 需要調 TD1，只能改 TD0（`0x8055`），可能打破標準時序 |
| 3 | **TA1/TA2/TA3** 皆未提供設定命令 | active timer 通常由 OHT/AMHS 側設定；passive 側仍需可監控 | 無介面讀 active timer 觸發的 timeout 原因（只能靠 Alarm code 0x5000/0x5001 反推） |
| 4 | Alarm code `0x5002`/`0x5003` 名稱為 **TP3 / TP4 timeout**，狀態 `0x05`（需手動 RESET） | SEMI E84 TP3 為「BUSY→PS/PL 到位」時間，若超時通常視為 carrier 位置異常 | 與標準行為一致，惟文件未註明 TP3 起點是 BUSY↑ 還是 COMPT↑，實作端易混淆 |
| 5 | `0x500D` **CS0_CS1_ALL_ON_ERROR** | SEMI E84 禁止 CS\_0 / CS\_1 同時 ON，但未定義在 time diagram | 文件有補充偵測，但未於時序圖註記，建議補 |
| 6 | HO\_AVBL / ES 在時序圖有畫 baseline，但未標註「**持續保持 ON**」為 prerequisite | SEMI E84 規範 HO\_AVBL、ES 為 handoff 前提 | 文件時序圖對新手易誤解為「可任意切換」 |

### 3.2 文件本身的矛盾／語意不清

| # | 位置 | 問題 | 建議 |
|---|------|------|------|
| A | P4 「總共 12 組 IO 可使用，第 13pin 固定接 Relay」 | 實際 I/O 含 Relay 為 **13 組**（P1/2~P25/26），「12 組 IO + 1 Relay」與「13 pin 固定 Relay」混用；此外 P13/P14 明明是 DONE/開門/關門，不是 Relay | 改為「12 組可程式化 I/O（P1–P24）＋ 1 組專用 Relay（P25/P26）」 |
| B | P7 校驗範例：`0x55+0xAA+0x00+0x11+0x00+0x00=0110（只保留低位元 2 位=10）` | 加總 0x110，低 8-bit = `0x10`；文中寫「0110」實為 0x0110 的 hex 表示，再「保留低 2 位」即 `0x10`，但敘述「只保留低位元 2 位 =10」容易被誤讀為「低 2 bit」 | 明確寫「保留 low byte（低 8-bit）」並顯示 `0x10` |
| C | P7 Receive 範例沿用 Send 的相加值 | Receive 比 Send 多了 **STATUS byte**，校驗必不同；例子未把 STATUS 算入 | 重新計算範例：`0xAA+0x55+0x00+0x11+0x00+0x00+0x00 = 0x0110`，STATUS=0 恰巧相同，但應明寫「STATUS 納入 CHK 計算」 |
| D | P8 資料欄 `0~0x000F / 00~0x00F0 / 000~0x0100` 三行疊寫，同一個命令代表不同 bit 群組分別控制 | 易被誤解為「三選一」或可混用 | 改成 bitmask 表，欄位 `[Reset:bit8][Switch1:bit7]…[ES:bit0]` |
| E | P8 `0x8013` Debug 開關「輸入 55AA 以外任意數值即關閉」 | Sentinel 設計，容易被誤觸（例如校驗失敗重送資料 0xFFFF） | 建議改為明確 `0x0000=Off / 0x55AA=On`，其他值回 STATUS `0x02`（條件不允許） |
| F | P11 `0x8114` LP 模式資料格式 `0~2` 與 `0000~0200` 並列 | CS1 的 In/Out Port 寫 `0x0100` / `0x0200`（高 byte），易被客戶端當成 16-bit 值 `0x0100=256` 誤算；且 CS0 與 CS1 不能用同一個命令位元同時設定會造成歧義 | 拆成兩個獨立命令或明確用 bitfield 表示 |
| G | P9 `0x0011` IN byte 位 bit 定義 `CONT / COMPT / BUSY / TR_REQ / AM_AVBL / CS_1 / CS_0 / Valid` | 與 SEMI E84 active→passive 信號集相符，但 `CONT` 放在 bit7、`AM_AVBL` 放在 bit3，沒有一般 E84 文件常見的排序（易對照錯） | 附 SEMI E84 Figure 35 對照表 |
| H | P16–31 Alarm 碼表內 **狀態欄** 只列 `0x00` 與 `0x05`（CS0）／`0x10` 與 `0x15`（CS1），但上方 P12 定義僅 0x00–0x0F 共 16 種 | CS1 的 `+0x10` 偏移雖一致，但 P12 狀態定義表未涵蓋 0x10–0x1F 區段 | 在 P12 註明「CS1 側狀態 = 對應 CS0 + 0x10」 |
| I | P7 `0x8001` 資料 `0x0000~0x0002` 與 P3 Sensor 介面「DB25 / 2×RJ-11」 | 命令支援 3 種（DB25 / RJ6 / RJ7），但 P3 只畫 2 個 RJ-11；RJ6 vs RJ7 差異未說明（6-pin vs 7-pin？or left vs right channel？） | 補充兩個 RJ 通道的語意 |
| J | P11 `0x8058` CS0 EQ Clamp Enable `0/1` vs CS1 EQ Clamp `0/0100` | CS1 用高 byte 表示，與 `0x8003`、`0x8114` 一致但都沒集中說明；新人閱讀很容易一開始就當成 16-bit 純粹數值 | 在 P7「資料 2 Byte」下方加「本控制器對 CS0 使用低 byte、對 CS1 使用高 byte」整體規則註 |
| K | P14 `0x0071 0x0007 0x10`（Right Auto Recover） | 同表內其他 Right 事件是 `0x0014~0x0019`、`0x002E~0x0031`，Auto Recover 卻沿用 `0x0007` 再靠 STATUS = 0x10 區分 | 容易遺漏處理；建議改用獨立 sub-code `0x001A` 類似方式 |
| L | P14 PS/PL sub-code `0x01xx~0x08xx`（例 `0x021A`=PS2 ON Left） | 4 個基底（0x001A/0x001B/0x001C/0x001D 等）僅低 byte，要組合時高 byte=PS/PL 編號 1-8。組合後會落在 `0x0100+基底` 範圍 | 例子中 `0x001F=PS OFF (Right)` 與 `0x021F=PS2 OFF (Right)`（不是 Left！），須在文件明示 `0x0X1A/0x0X1B`=Left、`0x0X1E/0x0X1F`=Right，避免把 Right 誤解為 Left |
| M | P12 STATUS `0x01`（可自動恢復異常）與 `0x00`（設定成功/可自動恢復異常）語意重疊 | `0x00` 同時涵蓋「OK」與「可自動恢復」兩種語意，上層軟體難以區分「這是成功回應還是 alarm 可自動恢復」 | 建議將 Alarm 強制回 `0x01`，成功回 `0x00` 分離 |
| N | P11 `0x0048/0x004a/0x004b` 等「Button 反應時間」與 `0x004c/0x004d/0x004e/0x004f`「ES/Clamp/Light/EQ_ER 反應時間」 | 這些其實是本控制器的 **debounce 時間**，並非 SEMI E84 timing | 在文件中分區：「§3-4a E84 協議 Timer」 vs 「§3-4b 控制器按鈕 debounce」 |

### 3.3 Double-Check 清單（供你與客戶／原廠對齊）

1. **TP6 定義**：原廠意圖對應 SEMI E84 哪個相位？是否為「COMPT↑→VALID↓」或廠商自訂？
2. **TD0 / TD1**：只開放 TD0（`0x8055`）設定，若 continuous handoff 需 TD1 是否由 TD0 兼任？
3. **CONT 信號歸屬**：文件將 CONT 放在 `0x0011` 的 bit7（IN bitmap，active→passive），符合 SEMI E84；但 `0x20xx` Alarm 群在 CS0 流程條件中出現 `CONT OFF`，實際 handoff flow 正常狀態下 CONT 應由 OHT 維持，斷掉即報 alarm，需確認此條件是否包含「終點 carrier 移交後主動關 CONT」的正常情境。
4. **`0x8050` Link Timeout**：2–60 s，若 RS-232 被 PC 停擺是否會觸發 Alarm `0x5007`？該 Alarm 會回傳到 PC（此時 PC 已斷），需釐清靠 Relay 或 LED 對外通知。
5. **Relay (`0x8125`)**：文件未說明 Relay 預設對應何種動作（通常為 Handler Run/Stop 或 Tower Light），需額外確認機台接線。
6. **Alarm Bypass (`0x8022`)**：Bypass 後 ES/Light Curtain 是否也被忽略？若是，則違反 SEMI S2 安全規範，不得用於線上量產。
7. **韌體版本（`0x0000` VER:2001）**：2001 是 ASCII `"2001"` 還是 binary `0x07D1`？格式未明。
8. **`0x8014` Reset `bit0`=內/外部偵測**：Reset 通常由面板按鈕觸發，切到內部偵測後若軟體未持續下 `0x8016` 則永遠無法 Reset，需注意開機預設值。
9. **`0x500E` Door\_Close\_Timeout** 與 `0x500B` Door\_Open\_Timeout：Door 超時時間未列入可設定命令，是否 hard-code？
10. **PS/PL 感應時間 0.1–10 s**（`0x8046`/`0x8047`）：僅 ON→OFF 方向濾波，OFF→ON 未濾波，OHT 快速放貨瞬間 bounce 可能誤觸發 `0x106C BUSY OFF`。

---

## 4. 對 HT9045 整合的影響評估

| 層面 | 現況 | 建議 |
|------|------|------|
| 本控制器走 RS-232 回報事件 `0x0070/0x0071/0x0080` | HT9045 目前無對應 RS-232 解析器（僅有對 AGV/MR 的 IO 直連方式） | 若要整合此板，需新增串列解析模組，解析完把結果對應到 `SnE84_1_*`／`SnE84_2_*` IO bitmap |
| `SnE84LREQ / UREQ / VA / READY / VS0 / VS1 / HOAVBL / ES` 回讀 | V899 僅宣告未使用（見第 2 節） | 若改接此 E84 控制器，可用來比對「本機發出的 passive 信號」是否真正被控制器接收 → 正是預留的驗證用途 |
| Alarm code 映射 | HT9045 Alarm 編碼系統為 JAM/WAR/MES 前綴；本控制器是 `0x00/0x10/0x20/0x50` 族 | 需要 mapping 表，建議對應至 `sWAR16xxx` / `sJAM01xx` 既有系列 |
| 安全 | `0x8022` Alarm Bypass、`0x8019` PL 強制 ON、`0x8104` PS 強制 ON | 這些命令**不得**出現在 Auto 生產模式；只能在 maintenance / PM 模式。修改時請遵循 `.github/instructions/safety-critical-change.instructions.md` |

---

## 5. 結論

1. **文件解析** — 完整涵蓋硬體配置、RS-232 封包、137 條指令、Load/Unload/CONT 流程 Alarm 碼族。
2. **HT9045 程式碼**：`SnE84LREQ` 等 9 顆屬「passive 端輸出信號之回讀 sensor」，**目前 V899 無控制邏輯使用**，僅保留於宣告、ID 表、IO 監控 Name。可視為**保留項**（非誤宣告），但也可能是歷史遺留；若要精簡，需連動調整後續 sensor index 避免錯位。真正運作的是 `SnE84_1_*`/`SnE84_2_*`（AGV 雙 LP）與無前綴組（MR/RFID single LP）。
3. **文件品質**：與 SEMI E84 標準大致相容，但包含 `TP6`、STATUS 0x00/0x01 語意重疊、低/高 byte 混用的 CS0/CS1 雙槽資料格式、校驗範例缺少 STATUS 計算等議題；`Alarm Bypass`、`PL/PS 強制 ON` 命令具安全風險，整合前務必與原廠確認並在軟體上做模式鎖定。

> 主要待跟原廠確認項目（Must-Check）：
> - `TP6 / TD1` 的語意對應
> - `0x8022 Alarm Bypass` 的涵蓋範圍
> - 韌體版本回傳格式
> - Relay (`0x8125`) 對應的實體功能
> - RJ6 / RJ7 兩組的用途差異
