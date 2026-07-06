# Mechanism Contract：stacker-cylinder-stackonly

> **堆疊區 — 上下汽缸驅動 + 只能往上堆（無分離爪）**
>
> 跨機台契約。單向疊放：手臂從上方放盤入堆疊，滿盤後整柱回收。
> **本契約目前為「單機驗證」狀態**（僅 HT9045 Auto 系列），標籤等級偏 ?。

| 項目 | 內容 |
|------|------|
| 機構家族 | tray-stacker（料盤堆疊區） |
| 驅動方式 | 上下汽缸（多段位置：升頂 / 接盤 Middle / 下降） |
| 能力 | 子能力 B：只能往上堆（單向疊放，無中段抽片） |
| 已驗證機台 | HT9045（V874–V903）— **單機**，待找第二實作 |
| 不適用機台 | HT172（採 Magazine 整柱搬移，無此機構家族） |
| 最後 review | 2026-05-07 |
| review 人 | JimmyChiu |

---

## §0 範圍宣告

### 本契約描述

只描述「Auto 出料堆疊區」的**核心收盤週期**：
- 升降汽缸如何切換 接盤位置 ? 待命位置
- 接盤偵測如何觸發固定機構
- 滿盤如何觸發回收

### 本契約不描述

- Auto 內的「Color Tray 偵測」分支（屬客戶選配，?）
- Auto 內的「Recycle」整柱回收子流程（屬下游 tray-recycle 機構）
- Auto 內的「TrayFeed / CleanOut / Init」特殊模式分支
- 與 OutArm / SortArm 的握手細節（屬 L3 handshake）

### 與 stacker-cylinder-separator 的差異

| 項目 | separator（分離爪版） | stackonly（只往上堆，本契約） |
|------|---------------------|---------------------------|
| 物理流向 | 取盤（堆疊 → 外部） | 放盤（外部 → 堆疊） |
| 分離爪 | 必須有 | 無 |
| 接盤位置 | Middle（中停取頂層） | Middle 或 Up（依機台 ini 配置） |
| 滿盤偵測 | 通常無 | 必須有 |
| 觸發條件 | 工作位空 | 手臂帶 Tray 來 |
| 完成動作 | 把盤放回堆疊 | 滿盤觸發回收 |

---

## §1 物理組成

```
              ┌─────────────┐
   外部手臂 →  │  接盤窗口   │  ← 升降到 Middle/Up 時對齊手臂
              │  ?????????  │
              │  Tray N+1  │  ← 剛收到的最新一片
              │  Tray N    │
              │  ...        │
              │  Tray 1    │  ← 堆疊底
              └──────┬──────┘
                     │
              ┌──────┴──────┐
              │ 升降汽缸    │  ← 三段位置
              └─────────────┘

  輔助機構（部分機台）：
  - Side Fixer：固定堆疊側邊
  - Edge Push：邊緣推正
  - UpPress：上方壓盤防晃
  - Selector：分階段升降輔助
```

物理零件清單：
- **1× 升降汽缸**（多段位置）
- **0–1× 分階輔助汽缸**（Selector / Selector Cylinder）
- **1× Tray 偵測 sensor**（軌道上有盤）
- **1× 工作位偵測 sensor**（堆疊頂部有盤）
- **1× 滿盤偵測 sensor**（必要）
- **0–1× Color Tray 偵測 sensor**（選配 ?）
- **0–3× 固定/壓盤輔助汽缸**（Side Fixer / Edge Push / UpPress，?）

---

## §2 不變量

### N1 ? 接盤位置必須對準外部手臂的放盤高度

升降汽缸停在「接盤位置」時，堆疊頂層必須對準外部手臂的 Z 軸放盤點。
- 此位置在不同機台可能是 Middle 或 Up（由機構物理高度決定）
- 由 ini 旗標切換（如 HT9045 的 `IniConfig.bP37bAutoCylinderUP`）

### N2 ? 收盤期間升降汽缸必須穩定

外部手臂進行放盤動作時，升降汽缸**不可同時動作**。
- 違反 → 撞手臂或盤掉落

### N3 ? 滿盤後必須先固定再觸發回收

`SEN_FULL` 觸發後，必須：
1. 先把固定 / 壓盤汽缸鎖緊
2. 再啟動下游回收流程

否則回收搬移過程中盤會散落。

### N4 ? case 編號契約（Auto 系列）

跨 HT9045 Auto 1/2/3 共通的 case 編號（**僅 HT9045 內部驗證**，未跨機台）：

| case | 語意 | 必要性 |
|------|------|-------|
| 1 | 入口：buffer 一致性檢查 | 必須 |
| 100 | 待命動作：Side Fixer / Edge Push / UpPress 復位 | 必須 |
| 110–113 | 升降到接盤位置（多步） | 必須 |
| 200 | 是否有 Tray 分流 | 必須 |
| 300 | 等待 CatchArm/外部手臂補 Tray | 必須 |
| 500 | 偵測收到 Tray + Color 檢查 | 必須 |
| 750 | Tray 沒放好的錯誤處理 | 必須 |
| 800 | 固定剛收到的 Tray | **必須**（N3） |
| 850 | 偵測沒有 Tray 的錯誤處理 | 必須 |
| 900–970 | 整柱回收 / 退盤子流程 | 視機台 |
| 1000+ | TrayFeed / CleanOut 特殊模式 | ? 機台特定 |
| 1100 | 滿盤判斷與停機策略 | 必須 |
| 1200+ | Recycle 完整流程 | 視機台 |
| 1350+ | 分離氣缸版本的補強流程 | ? 機台特定 |

> ?? 與 separator 契約相比，本契約 case 編號 **跨度大但單機驗證**，等找到第二實作再加固。

### N5 ? 滿盤策略需可選「立即停機 / 完成後停機」

`CosFunction.bAutoFullDoNotStopImmediately` 或等價旗標必須可配置：
- `true`：滿盤後完成本輪 unload 才停
- `false`：滿盤立即停

兩種策略對 cycle time 有重大影響，必須由機台設計決定，**非可省略**。

### N6 ? 三組 Auto（Auto1/2/3）行為必須一致

HT9045 慣例：Auto1 / Auto2 / Auto3 共用同一 `DoAutoReceiveBinTray(Pos)` 函式，差別只在 `Pos` 參數。
- 跨 Pos 行為差異應全部由 IO 配置 / ini 旗標控制
- **不允許**用 `if(Pos==0)` 做硬編碼分流
- 例外：客戶特例（如 `CC_TERAPOWER` 的 Color sensor）需明確標註

---

## §3 狀態機契約（核心收盤週期）

```
[case 1]   入口 buffer 一致性檢查
   │       條件：Buffer 紀錄 vs sensor 偵測
   ▼
[case 100] 待命動作 reset（Side Fixer / Edge Push / UpPress 鬆開）
   │       依 IniConfig 決定走 case 110（汽缸上）或 case 200（汽缸下）
   ▼
[case 110→113] 升降汽缸 → 接盤位置（多步保護）
   │       N1：對準手臂高度
   ▼
[case 200] 分流：fHasTray ?
   │       ├─ Yes → case 500
   │       └─ No  → case 300
   ▼
[case 300] 等外部手臂補 Tray
   │       手臂放盤完成 → case 500 / 1600
   ▼
[case 500] Tray 偵測 + Color 檢查
   │       sensor 確認有盤 → case 800
   │       sensor 異常     → case 750
   ▼
[case 750] 錯誤：Tray 沒放好
   │       K_SKIP → 清空 Tray 資料 → case 100
   │       K_RETRY → 鎖固定 → case 500
   ▼
[case 800] ★ 固定剛收到的 Tray（N3）
   │       Side Fixer / Edge Push / UpPress 全部 ON
   ▼
[case 850] 確認 sensor 有盤
   │       3000 次 retry → JAM
   │       成功 → 結束本輪 → case 1
   ▼
[case 1100] 滿盤策略判斷（每輪檢查）
   │       依 N5 策略決定要不要停機
```

★ = 必須步驟，跨機台不可省略

---

## §4 角色清單（不綁名）

### Actuator 角色

| 角色 ID | 物理意義 | HT9045 對應 |
|---------|---------|------------|
| `ACT_LIFT` | 主升降汽缸 | `C_Auto_Up[Pos]` |
| `ACT_SELECTOR` | 分階輔助汽缸 | `C_Auto_Selector[Pos]` |
| `ACT_SIDE_FIXER` | 側邊固定 | `C_AutoSide_Fixer[Pos]` |
| `ACT_EDGE_PUSH` | 邊緣推正 | `C_AutoEdgePush[Pos]` |
| `ACT_UP_PRESS` | 上方壓盤 | `C_AutoUpPress[Pos]` |
| `ACT_Z_SELECT` | 後段分離氣缸（部分機台） | `C_AutoZ_Select[Pos]` |

### Sensor 角色

| 角色 ID | 物理意義 | HT9045 對應 |
|---------|---------|------------|
| `SEN_TRACK_HAS_TRAY` | 軌道上有盤 | `SnAutoTrackDetect[Pos]` |
| `SEN_TRAY_DETECT` | 收盤窗口偵測 | `SnAutoTrayDetect[Pos]` |
| `SEN_CAR_HAS_TRAY` | 工作位（車體）有盤 | `SnAutoTrayCar[Pos]` |
| `SEN_FULL` | 堆疊滿盤 | （由 `MOT[iMMAuto[Pos]].fHasTray` 計數推導） |
| `SEN_COLOR_TRAY` | Color Tray 偵測（選配） | `SnAutoColorTrayDete[Pos]` |

### Flag 角色

| 角色 ID | 用途 | 共通性 |
|---------|------|-------|
| `INI_LIFT_DEFAULT_UP` | 接盤位置 = Up（true）/ Middle（false） | ? |
| `INI_FULL_NOT_STOP_IMMEDIATELY` | 滿盤策略（N5） | ? |
| `INI_FULL_AFTER_UNLOAD_END` | 滿盤等 Unload 完才報警 | ? |
| `FLAG_DUP_ERROR` | 防重複 alarm | ? |

---

## §5 安全規則

| ID | 規則 | 等級 |
|----|------|------|
| S1 | 接盤位置切換期間外部手臂必須在等待區 | ? |
| S2 | case 800 固定動作必須在 case 500 確認有盤後才執行 | ? |
| S3 | case 850 retry 上限（3000 次）達到 → JAM，不可無限重試 | ? |
| S4 | 滿盤後 Recycle 流程啟動前必須確認所有固定汽缸 ON | ? |
| S5 | Color sensor 異常（CC_TERAPOWER）→ 等手臂離開後再 alarm | ? 客戶特例 |

---

## §6 對外交握介面

### 上游觸發者

- `OutArm` / `SortArm` / `CatchArm`（外部手臂帶 Tray 來）
- 觸發條件：手臂帶有測完 IC 的 Tray + Auto 接盤位置就緒

### 下游被影響者

- `tray-recycle` 機構（滿盤觸發整柱回收）
- 統計模組（`fProductionInfo->CalTrayICCount`）
- AI CCD 檢查模組（`fFixAICCD->bCheckUnloaderHasAiNG`）

### Hold 點

- **case 300**：等手臂期間可被外部 hold（無時限）
- **case 500**：只在 sensor 等待期間 hold（有 retry 上限）

---

## §7 驗證 checklist

新機台實作此機構時：

### 必要結構
- [ ] case 1 入口含 buffer / sensor 一致性檢查
- [ ] case 100 含 Side Fixer 等輔助汽缸 reset
- [ ] case 110–113 或等價：含「先 Lower 再 Middle 再 Up」的 reset 順序
- [ ] case 200 / 300：含等待手臂分流
- [ ] case 500：含 Tray 偵測
- [ ] case 800：含固定動作（N3）
- [ ] case 850：含 retry + JAM
- [ ] case 1100 或等價：含滿盤判斷

### 必要安全
- [ ] N3：滿盤前必鎖固定汽缸
- [ ] S3：retry 有上限
- [ ] N5：滿盤策略可配置

### 命名（建議）
- [ ] 三組以上 Auto 共用同一函式 + Pos 參數（N6）

### 文件
- [ ] 在 `docs/implementations/` 新增對照檔
- [ ] 本契約 §8 表格新增一列

---

## §8 已知實作對照

| 機台 | 實作位置 | 對照文件 | 風格 | 備註 |
|------|---------|---------|------|------|
| HT9045 V899 | `asendic_Auto.cpp::DoAutoReceiveBinTray` | [HT9045-auto-stackonly.md](../implementations/HT9045-auto-stackonly.md) | 個別檔案派 + Pos 參數 | Auto1/2/3 共用 |
| HT172 V1.0.25 | **不適用** | [HT172-auto-not-applicable.md](../implementations/HT172-auto-not-applicable.md) | — | 採 Magazine 機構，不屬本契約家族 |

> ?? 本契約目前 **單機驗證**。N4 的 case 編號契約等找到第二實作後升級為 ? 跨機驗證。

---

## §9 與 separator 契約的決策樹

新機台要選哪個契約？依下表：

| 機構特徵 | 選擇 |
|---------|------|
| 從堆疊取盤給外部 | **separator** |
| 外部放盤入堆疊 | **stackonly**（本契約） |
| 兩者都需要 | 兩個契約都實作（不同函式） |
| 機構是 Magazine（盒裝整柱） | **不適用本家族**，另寫 magazine 契約 |
| 沒有汽缸（馬達升降） | 等 `stacker-motor-*` 契約 |

---

## §10 變更歷史

| 日期 | 變更 | review |
|------|------|--------|
| 2026-05-07 | 初版（HT9045 單機驗證；HT172 經查無對應機構） | JimmyChiu |
