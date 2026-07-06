# Mechanism Contract：stacker-cylinder-separator

> **堆疊區 — 上下汽缸驅動 + 分離爪（可上下分堆）**
>
> 跨機台契約。任何採用此機構的機台，其控制程式必須符合本文所列規則。
> 不綁變數命名、不綁檔案組織、不綁實作風格。

| 項目 | 內容 |
|------|------|
| 機構家族 | tray-stacker（料盤堆疊區） |
| 驅動方式 | 上下汽缸（三段位置：升頂 / 中停 / 下降） |
| 能力 | 子能力 A：上下分堆（可從堆疊中段抽單片） |
| 已驗證機台 | HT9045（V874–V903）、HT172（V1.0.25） |
| 最後 review | 2026-05-07 |
| review 人 | JimmyChiu |

---

## §0 範圍宣告

### 本契約描述

只描述「堆疊區」這個**子機構**的升降與分盤行為。

### 本契約不描述

- 料盤水平搬移（CatchTray）
- 料盤輸送 / 進出機台（tray-conveyor，另份契約）
- 料盤到工作位的精定位（tray-positioner，另份契約）
- 料盤內 IC 取放（InArm / OutArm）

### 區域名 vs 機構名

「Loader」「Empty1」「Color」是**區域名**，可能由多個機構組成：

```
[區域：Empty1] ? [機構：本契約描述的堆疊區]
              + [機構：tray-conveyor]
              + [機構：tray-positioner]
              + ...
```

---

## §1 物理組成

```
              ┌─────────────┐
              │  Tray Cap   │  ← 上方限位（防爆衝）
              ├─────────────┤
   ┌──┐       │  頂層 Tray  │  ← 升頂後可被外部手臂取走
   │Z │←─────│  ?????????  │
   │分│       │  次頂層 Tray│  ← 分離爪推出時夾這層
   │離│       │  Tray N-2   │
   │爪│       │  ...        │
   └──┘       │  Tray 1     │  ← 堆疊底
              └──────┬──────┘
                     │
              ┌──────┴──────┐
              │ 升降汽缸 Up  │  ← 三段位置：Lower / Middle / Up
              └─────────────┘
```

物理零件清單：
- **1× 升降汽缸**（三段位置控制）
- **1× 分離爪汽缸**（Z_Select / Separatory，水平 push/pop）
- **1× 工作位料盤偵測 sensor**（CarHasTray）
- **1× 下降到位偵測 sensor**（CCWDete / 或等價）
- **1× 堆疊區頂層料盤偵測 sensor**（SelectHasTray）
- **0–1× 滿盤偵測 sensor**（CarIsFull，可選）
- **0–1× 壓盤輔助汽缸**（如 LoaderUpPress，? 單機特例）

---

## §2 不變量（必須遵守）

### N1 ? 升降三段位置互斥

升降汽缸在任一時刻只能停在 **Lower / Middle / Up** 三個位置之一。
- 中途任何 sensor 異常 → 立即停止並報 JAM
- 不允許同時觸發 Up + Middle 訊號

### N2 ? 分離爪推出時夾「次頂層」

分離爪推出（Push）的物理位置設計，必須夾住**次頂層**而非頂層。
- 否則「上下分堆」能力不成立
- 這是機構設計時的物理高度決定，程式無法改變

### N3 ? 升頂前不得有人在上方工作區

升頂動作會把整柱料盤舉到外部手臂工作高度，
- 升頂前必須確認上方手臂已離開（透過交握 flag，本契約不規範細節）
- 違反 → 撞機

### N4 ? case 編號契約

狀態機 case 編號的語意跨機台一致（已在 HT9045 / HT172 驗證）：

| case | 語意 | 必要性 |
|------|------|-------|
| 1 | 入口檢查（工作位空否、是否有料可供） | 必須 |
| 20 | Middle 氣缸防護 + timeout（防 sensor 卡亮） | 建議 |
| 50 | 升頂前置（滿盤檢查 / 顯示） | 建議 |
| 60 | 升降汽缸 → Up | **必須** |
| 100 | 分離爪 Push（夾次頂層） | **必須** |
| 200 | 升降汽缸 → Middle（中停層） | **必須** |
| 300 | 分離爪 Pop（復位） | **必須** |
| 400 | 延遲（等待外部完成取/放） | 建議 |
| 410 | 升降汽缸 → Lower（下降） | **必須** |
| 420 | 工作位 sensor 驗證；失敗 → JAM | **必須** |
| 500+ | 與下游模組（輸送帶等）的交握區 | 視機台而定 |

> **不允許**重新編號（避免跨機 review 時混亂）。新增子步驟用 `52` `65` `150` 等中間值。

### N5 ? 中停層必須可被外部 hold

case 200 進入中停層後，
- 必須允許外部以 Pause flag 或 handshake 變數讓本狀態機**暫停在 case 200**
- 否則上層手臂無法可靠取盤

### N6 ? 異常停在安全位置

任何 case 報 JAM 後：
- 升降汽缸不可懸空在中段位置
- 必須降回 Lower 或停在 Middle，由操作員處理

### N7 ? JAM 碼分類

異常碼建議使用 `JAM03xx` 區段（HT 系列慣例，非物理必然）。

### N8 ? 壓盤輔助機構為單機特例

HT9045 Loader 區域額外有 `C_LoaderUpPress` 壓盤汽缸，
- **不是本契約一部分**
- Port 新機台時若沒此機構，跳過相關邏輯

---

## §3 狀態機契約（流程圖）

```
[case 1]   入口檢查
   │       條件：工作位空 AND (CarHasTray.Off AND CCWDete.Off) AND 堆疊有料
   │       否則 → 結束本輪（return false）
   ▼
[case 20]  Middle 氣缸防護（建議）
   │       若 Middle sensor 卡亮 → 強制 reset
   ▼
[case 50]  升頂前置（建議）
   │       若 CarIsFull → 提示後停止
   ▼
[case 60]  ★ CylinderUp                         ← 升頂
   │       前置：N3（上方無手臂）
   ▼
[case 100] ★ Separator.Push                     ← 分離爪夾次頂層
   │       此時頂層可被外部取走
   ▼
[case 200] ★ CylinderMiddle                     ← 中停層
   │       N5：可被外部 hold
   ▼
[case 300] Separator.Pop                        ← 分離爪復位
   ▼
[case 400] 延遲（建議）
   │       等待外部完成取/放
   ▼
[case 410] ★ CylinderLower                      ← 下降
   ▼
[case 420] 工作位驗證                            ← N6 安全位置
   │       sensor 不到位 → ShowSystemError(K_RETRY|K_SKIP)
   │       到位 → 通知 tray data 物件 GetNewNullTray
   ▼
[case 500+] 與下游交握（視機台）
```

★ = 必須步驟，跨機台不可省略

---

## §4 角色清單（不綁名）

> **規則**：本契約**只規範角色**，不規範實作命名。實作可叫任何名字。

### Actuator 角色

| 角色 ID | 物理意義 | HT9045 對應 | HT172 對應 |
|---------|---------|------------|-----------|
| `ACT_LIFT` | 三段升降汽缸 | `C_Empty_Up` | `C_Up[idx]` |
| `ACT_SEPARATOR` | 分離爪汽缸 | `C_EmptyLoaderZ_Select` | `C_Separatory[idx]` |

### Sensor 角色

| 角色 ID | 物理意義 | HT9045 對應 | HT172 對應 |
|---------|---------|------------|-----------|
| `SEN_WORK_HAS_TRAY` | 工作位有料盤 | `SenEmptyCarHasTray` | `Sn_CarHasTray[idx]` |
| `SEN_LIFT_DOWN` | 升降已下降到位 | `SenEmptyCCWDete` | （無，由 CarHasTray 兼任） |
| `SEN_STACK_HAS_TRAY` | 堆疊區頂層有料 | `SenEmptySelectHasTray` | `SnSelectHasTray[idx]` |
| `SEN_STACK_FULL` | 堆疊滿（可選） | （無） | `Sn_CarIsFull[idx]` |

### Flag 角色

| 角色 ID | 用途 | 共通性 |
|---------|------|-------|
| `FLAG_PAUSE` | 操作員按暫停 → 停在 case 200 | ? |
| `FLAG_AUTO_RUN` | 自動模式才執行 | ? |
| `FLAG_TRAY_FEED_MODE` | 補盤模式特殊路徑 | ? |
| `FLAG_CLEANOUT_MODE` | 清出模式特殊路徑 | ? |

---

## §5 安全規則

| ID | 規則 | 等級 |
|----|------|------|
| S1 | case 60 升頂前必須確認 `SEN_WORK_HAS_TRAY.Off`（工作位無料才升） | ? |
| S2 | case 60 升頂前必須確認上層手臂已離開（透過交握，本契約不規範細節） | ? |
| S3 | case 200 中停時若收到 `FLAG_PAUSE` → 必須維持 case 200 | ? |
| S4 | case 420 失敗 → 必須 retry 或 skip，**不可**繼續往下走 | ? |
| S5 | 任何步驟 timeout → 升降回 Lower 後再報 JAM | ? |
| S6 | 多實例（同機台多個堆疊區）共用 CatchTray 時，不可同時佔用 | ? |

---

## §6 對外交握介面

### 上游觸發者（誰會呼叫此狀態機）

- 上層 Loader 排程器（HT9045: `cKernel`；HT172: `aLoader::DoEmpty1`）
- 觸發條件：工作位空 + 自動模式 + 非清出 / 補盤特殊狀態

### 下游被影響者（本狀態機完成後通知誰）

- Tray 資料物件（`GetNewNullTray()` 或等價）
- 同區域的輸送帶 / 定位機構（透過 case 500+ 交握）
- CatchTray 搬盤手臂（升頂後才能取盤）

### Hold 點

- **case 200**：唯一可被外部 hold 的 case
- **不可** hold 在 case 60 / 100 / 410（會卡在不安全位置）

---

## §7 驗證 checklist

新機台實作時，逐項勾選：

### 必要結構
- [ ] 函式入口為 `case 1`，含工作位空檢查
- [ ] case 60 呼叫升降汽缸 → Up
- [ ] case 100 推出分離爪
- [ ] case 200 升降汽缸 → Middle
- [ ] case 300 分離爪復位
- [ ] case 410 升降汽缸 → Lower
- [ ] case 420 含工作位驗證 + RETRY/SKIP 選項

### 必要安全
- [ ] case 60 前置條件含 `SEN_WORK_HAS_TRAY.Off`
- [ ] case 200 可被外部 hold
- [ ] case 420 失敗不會掉入 default 繼續執行
- [ ] timeout 處理會把汽缸降回安全位置

### 命名（不強制，但建議）
- [ ] 角色 ID 在程式碼註解中標註對應實際變數
- [ ] case 編號未被改寫
- [ ] JAM 碼採 `03xx` 區段

### 文件
- [ ] 在 `docs/implementations/` 新增對照檔
- [ ] 在本契約 §8 表格新增一列

---

## §8 已知實作對照

| 機台 | 實作檔案 | 對照文件 | 風格 | 備註 |
|------|---------|---------|------|------|
| HT9045 V899 | `asendic_Empty.cpp` | [HT9045-empty-stacker.md](../implementations/HT9045-empty-stacker.md) | 個別檔案派 | 每實例一檔；變數個別命名 |
| HT172 V1.0.25 | `HT172_Module/aLoader.cpp::DoLoadNewTrayToEmpty1Car` | [HT172-empty1-stacker.md](../implementations/HT172-empty1-stacker.md) | 陣列 idx 派 | 多實例共用函式族；變數陣列化 |

---

## §9 實作風格決策表

新機台選風格時參考：

| 決策點 | 個別檔案派（HT9045） | 陣列 idx 派（HT172） | 何時選 |
|--------|---------------------|---------------------|-------|
| 同機構實例數 | 1–3 | 4+ | 實例多 → 陣列派 |
| 維護人員 | 多人各管一檔 | 單人統一 | 多人 → 個別派 |
| 變體差異 | 大（_RT 旋轉版等） | 小（只差 IO） | 變體多 → 個別派 |
| 編譯時間 | 慢 | 快 | 在意 → 陣列派 |
| 新增實例成本 | 複製貼上整檔案 | 新增 enum 值 + IO | 常新增 → 陣列派 |
| 跨實例修改 bug | 改 N 個檔 | 改 1 處 | 常改 → 陣列派 |

---

## §10 變更歷史

| 日期 | 變更 | review |
|------|------|--------|
| 2026-05-07 | 初版（從 HT9045 / HT172 對比萃取） | JimmyChiu |
