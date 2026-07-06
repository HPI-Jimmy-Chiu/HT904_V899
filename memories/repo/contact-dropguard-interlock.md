# Contact 掉料防護 / 模式互鎖雷點 (CASE-20260608-001 全智)

> 對應 Skill：`ht9045-contact-pick-interlock`。改 Contact 掉料/切模式前先讀此檔。

## 一句話結論
Contact 掉料中止後「切不回 Contact test / 模式 widget 變灰」，
**只清測試頭(FTestSuck/BTestSuck)是不夠的** —— 互鎖還看 TestSocket + 4 個 CarryKit。

## 互鎖鏈 (FACT csystem.cpp #815-895)
- `IndexHasIC() = (FTestSuck||BTestSuck) || TestSocket`
- `ShuttleHasIC() = (FLCarryKit||BLCarryKit) || (FRCarryKit||BRCarryKit)`
- 兩個互鎖點都用 `IndexHasIC()||InArmSuck.HasIC()||OutArmSuck.HasIC()||ShuttleHasIC()`：
  - `DoTestContactFunction()` case 1 (#10954)：擋啟動 → "Please finish ONE CYCLE before Contact Test!"
  - `timerContactTimer()` (#20156, JerryYang 20251118)：擋切模式 → rgHandlerMode/rbModeNormal 變灰

## 退出還原只清一半 (FACT #1783-1790)
FormClose 只 `FTestSuck/BTestSuck ← iFTestBackItem/iBTestBackItem`。
**TestSocket 與 4 個 CarryKit 無人清。**

## SetItemData vs MoveSuckData (踩雷重點)
- `SetItemData(i,j,HAS_IC)` = 只標記，**不搬空飛梭** FLCarryKit
- `MoveSuckData(FLCarryKit,i,j)` = 搬移並清空來源
- case 3050 一般分支走 SetItemData → 飛梭 HAS_IC 殘留 → ShuttleHasIC() 仍 true

## V899 已做 / 未做
- ? 已做：case 3050 掉料中止前還原 FTestSuck/BTestSuck（等於補了 FormClose 本來就會清的那半）
- ? 未做：TestSocket + FLCarryKit/BLCarryKit/FRCarryKit/BRCarryKit 殘留未清
- ?? 問題 b（切不回）大概率未完全解；真兇最可能是飛梭 FL/BL 或 TestSocket

## 排查 SOP
掉料中止當下**不要關畫面**（關了 FormClose 會清掉 FTestSuck/BTestSuck 反而看不到真兇），
逐一查 FLCarryKit/BLCarryKit/TestSocket/FRCarryKit/BRCarryKit 的 Item!=NULL_IC → 那個就是鎖死源。

## 補清的鐵律
補清飛梭/Socket 前**必先確認是假殘留資料還是真有實體散料**。
真有散料卻清成 NULL_IC → 下一刀壓壞料。原 code 只敢清 TestHead 就是這原因。
補清一律綁客戶碼 (CC_GIGAS) + 雙層閘門 (CosFunction.bContactTestICDropGuard 外層)。

## 客戶碼隔離雙層閘門範式
```cpp
if(CosFunction.bContactTestICDropGuard==true && <模式>) {   // 外層 INI 開關，預設false不影響他人
    bool bGigasXxx = (CUSTOMER_CODE==CC_GIGAS && <模式>);    // 內層客戶鎖
    if(bGigasXxx) {/*新邏輯*/} else {/*原行為*/}
}
```
