# HT9045 V899 Release Note 草稿

> 自動從 `docs/spec/features/F899-*.spec.md` 第 8 節萃取。
> 三個版本：internal（含技術細節）/ distributor（含風險）/ customer（純功能描述）。

## 摘要：本版共 1 項變更

| Feature ID | 標題 | 影響模組 | 安全等級 |
|------------|------|----------|----------|
| F899-001 | InArm 在 2x4_16 配置下放料 / 取料 HotPlate 的 DataSwap 修正 | InArm | 安全關鍵（IC 位置追蹤錯誤會撞機） |

---

## CUSTOMER 版（給客戶 / 終端使用者）

> **【V899】InArm 16-site HotPlate 取放料資料修正**
> 修正 16-site 配置下 InArm 放料至 HotPlate 後，內部資料陣列與實際槽位錯位的問題。改善後可避免 InArm 從 HotPlate 取料時撞 socket 或吸取失敗。

---

## DISTRIBUTOR 版（給代理商 / FAE）

### F899-001 — InArm 在 2x4_16 配置下放料 / 取料 HotPlate 的 DataSwap 修正

> **【V899】InArm 16-site HotPlate 取放料資料修正**
> 修正 16-site 配置下 InArm 放料至 HotPlate 後，內部資料陣列與實際槽位錯位的問題。改善後可避免 InArm 從 HotPlate 取料時撞 socket 或吸取失敗。

**風險 / 副作用**：

- 不影響 1x1 / 2x4 / 2x8_32 配置（透過 `iInArmType` 分派）
- 需 regression：InArm Pick from HotPlate、AutoClean recycle 流程

---

## INTERNAL 版（給內部開發 / QA）

### F899-001 — InArm 在 2x4_16 配置下放料 / 取料 HotPlate 的 DataSwap 修正

- 客戶 / 來源：內部品質 / 跨版移植
- CASE-ID：（補：對應 weekly_data 中的 case）
- 影響模組：InArm
- 安全等級：安全關鍵（IC 位置追蹤錯誤會撞機）
- SPEC 來源：`docs/spec/features/F899-001-hotplate-dataswap.spec.md`

> **【V899】InArm 16-site HotPlate 取放料資料修正**
> 修正 16-site 配置下 InArm 放料至 HotPlate 後，內部資料陣列與實際槽位錯位的問題。改善後可避免 InArm 從 HotPlate 取料時撞 socket 或吸取失敗。

---

## 下一步

1. 人工 review 三版內容差異是否合適
2. 若有 //AI 註解但無對應 SPEC，請補 SPEC 後重跑
3. 對應 case 的 weekly_data 加入此 release note 連結