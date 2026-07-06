---
name: ht9045-motor-home
description: "HT9045 馬達回 Home / 單軸 Home 機制核心知識庫。Use when: 撰寫或除錯任何單軸 home 程式、客戶反應馬達抖動 / Home 30s timeout / Home 卡住、自動 home 子流程 (如 Check Device 進場前自動 home MLoaderY)、看到 HomeFlag 判斷不一致、混淆 Home() / MotorHome() / MotorInitial() 三個函式用途、Home 後速度殘留導致漂移。關鍵字：HomeFlag, MotorHome, MotorInitial, Home(), HomeReset, HomeObject, iMyHomeTask, ResetTime, RESET_TIMES, auto-home, Home All, Home timeout, 馬達抖動, MLoaderY home, Teach Home。"
---

# HT9045 馬達回 Home 機制 SPEC

> 來源：2026-05-05 V899 客戶端 MLoaderY auto-home timeout 除錯（cTrayMapping case 10/15/20 第一版誤用 `Home()` + `HomeFlag==2` 判斷成功，導致馬達抖動 30 秒 timeout）。
>
> 適用：所有透過 `MOT[]` 操作的馬達（mymotor.cpp 統一介面）。

## 1. 三個函式用途分清楚

| 函式 | 層級 | 用途 | 副作用 |
|------|------|------|--------|
| `MOT[i].Home()` | **底層** | 直接呼叫硬體 `Motor->HomeObject()` 觸發一次 home 動作 | **不更新 `HomeFlag`**、不跑狀態機 |
| `MOT[i].MotorHome(bool Flag)` | **中層狀態機** | 完整 home 序列：Reset → Home → 等 sensor → Retry → 設 HomeFlag | 會更新 `HomeFlag`；`Flag=true` 重置 timeout |
| `MOT[i].MotorInitial()` | **重置** | 清狀態：`Task=1`、`HomeFlag=0`、reset `ResetTime=RESET_TIMES*0.1s` | 不啟動 home，只清旗標 |

**重點**：要等 `HomeFlag` 變化的程式碼，**必須呼叫 `MotorHome(false)`**，不能呼叫 `Home()`。

## 2. HomeFlag 三態語意

| 值 | 意義 | 進入條件 |
|----|------|----------|
| `0` | 未開始 / 進行中 | `MotorInitial()` 後初始狀態 |
| **`1`** | **Home 成功** | `MotorHome` 內偵測 `Motor->HomeFlag()` 為 true，或馬達 disable 直接 pass |
| **`2`** | **Home 失敗** | retry 用完（`iHomeRetryCT==0`）/ 30s `ResetTime` 超時 |

**常見誤解**：
- ? `if(HomeFlag==2)` 當作「home 完成」→ 實際是失敗
- ? `if(HomeFlag!=0)` 當作「不在 home 中」→ 1/2 都會通過但語意混亂
- ? 成功判斷：`if(HomeFlag==1)`
- ? 失敗判斷：`if(HomeFlag==2)`
- ? 進行中：`if(HomeFlag==0)`

## 3. 標準單軸自動 Home 子流程範本

適用情境：某入口（如 Check Device、AutoStart 模擬）需要在 motor 沒 home 過時自動補 home，又不想要求使用者手動按 Home All。

```cpp
// 觸發條件：HomeFlag != 1
case ENTRY_TASK:
    if(MOT[mid].HomeFlag != 1)
    {
        // 1. 安全互鎖：關掉所有「會跟此軸干涉的固定機構氣缸」
        //    (台車載具上的氣缸隨軸移動，不需要動)
        Cylinder[C_FIXED_OBSTRUCTION].Off();
        tHomeWait.SetSecAndOn(2);                // 等氣缸退回
        Task = AUTO_HOME_WAIT;
        return false;
    }
    // ... 正常流程

case AUTO_HOME_WAIT:
    if(!tHomeWait.Off())                         // 等 2 秒到
        return false;
    // 2. Sensor sanity check：氣缸真的退回了嗎？
    if(Cylinder[C_FIXED_OBSTRUCTION].OnSensor() &&
      !Cylinder[C_FIXED_OBSTRUCTION].OffSensor())
    {
        asError = "C_FIXED_OBSTRUCTION sensor still ON. 機台不可移動，請檢查氣壓。";
        return false;                            // 中止避免撞機
    }
    MOT[mid].MotorInitial();                     // 3. 重置 home 狀態機
    tHomeWait.SetSecAndOn(30);                   //    外部 timeout 守門員
    Task = AUTO_HOME_RUN;
    return false;

case AUTO_HOME_RUN:
    MOT[mid].MotorHome(false);                   // 4. 必呼叫 MotorHome (非 Home)
    if(MOT[mid].HomeFlag == 1)                   //    成功
    {
        Task = ENTRY_TASK;                       //    回主流程
    }
    else if(MOT[mid].HomeFlag == 2)              // 5. 馬達自己判定失敗 → 立刻報錯
    {
        asError = "Auto-home failed (motor reported HomeFlag=2). Check home sensor / servo alarm.";
    }
    else if(tHomeWait.Off())                     // 6. 外部 timeout 兜底
    {
        asError = "Auto-home external timeout (30s).";
    }
    return false;
```

## 4. 反模式（會抖動 / 會 timeout）

```cpp
// ? 反模式 1：直接呼叫 Home() 並等 HomeFlag 變化
case 20:
    MOT[mid].Home();                  // 每 loop 觸發 HomeReset() → Task 一直被踢回 1
    if(MOT[mid].HomeFlag == 2) ...    // HomeFlag 永遠不會變（Home() 不更新它）
                                      // 馬達剛動就被打斷 → 抖動 → 30s timeout
```

```cpp
// ? 反模式 2：把 HomeFlag==2 當成功
if(MOT[mid].HomeFlag == 2)
{
    // 進入後續流程 → 實際 home 失敗，後續 MotorMove 會用無效座標撞限位
}
```

```cpp
// ? 反模式 3：不呼叫 MotorInitial 直接 MotorHome
// 若上次 home 結果是 HomeFlag=2，狀態機殘留，需要 MotorInitial() 才能重新觸發
```

## 5. 與全機 Home 的差異

| 項目 | uhome.cpp 全機 Home | 單軸 Auto Home (本 SPEC) |
|------|-------------------|------------------------|
| 觸發 | 使用者按 Home All / 開機 | 程式入口檢測未 home |
| 涵蓋範圍 | 所有 `TOTAL_MOTOR` | 單一 motor |
| 序列 | `MotorInitial()` 全部 → 持續呼叫 `MotorHome()` 直到 ret==1 | 同 SPEC §3 |
| 安全保護 | 全機氣缸 reset / safe pos | **只關自己這軸會撞到的固定機構氣缸** |

## 6. 速度狀態還原陷阱（與 Home 直接相關）

`TMyMotor::GetSpeed()` 回傳 **`Motor->ReadSpeed()` 瞬間 RPM**（停下時 0~5），**不是設定值**。

? 錯誤備份還原：
```cpp
int spd = MOT[m].GetSpeed();   // 抓到 0 (馬達停著)
MOT[m].SetSpeed(slow);         // 工作
MOT[m].SetSpeed(spd);          // 把 0 當 100% 還原 → 後續 Home/Move 龜速漂移、撞限位
```

? 正確：直接還原為已知預設值
```cpp
MOT[m].SetSpeed(100);          // 用業務上 known-good 預設值
```

uhome.cpp L3549 / L3559 對 `MLoaderY` 的處理就是直接 `SetSpeed(100)`，這是業界標準做法。

## 7. Teach 畫面為何「都正常」

Teach 介面呼叫的是「使用者按按鈕一次 → MotorHome 跑完整序列」單次模式，不會在外層迴圈中再呼叫 `Home()`，所以不會踩到反模式 1。**客戶反應 Teach OK 但程式內 Home 失敗 → 99% 是程式呼叫方式錯誤，不是硬體問題**，先檢查程式再叫客戶查 servo。

## 8. 診斷 log 必備欄位

馬達 home 異常的 log 必須包含：
```
Pos    : MOT[m].ReadPos()                   // 看馬達有沒有移動
Spd    : MOT[m].GetSpeed()                  // 確認速度設定（停下時不準但仍可看趨勢）
Home   : (int)MOT[m].HomeFlag               // 0/1/2 三態
Sensor : Cylinder[interlock].OnSensor() / OffSensor()  // 互鎖氣缸實際狀態
```

範例見 [cTrayMapping.cpp `DumpLaserScanState()`](../../HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422/cTrayMapping.cpp)。

## 9. 已知案例索引

| 日期 | 案例 | 失誤點 | 修正 |
|------|------|--------|------|
| 2026-05-05 | 欣銓 V899 MLoaderY auto-home timeout | 用 `Home()` + 等 `HomeFlag==2` | 改 `MotorHome(false)` + 等 `HomeFlag==1`；新增 `==2` 立即報錯 |
| 2026-05-05 | RestoreLoaderYSpeedRaw 用 GetSpeed 備份 | 把瞬間 RPM 當設定值 | hotfix：永遠 `SetSpeed(100)` |

## 10. 實作位置參考

- `Motor/mymotor.cpp` L1602-1742：`MotorInitial / Home / MotorHome` 完整實作
- `cTrayMapping.cpp` case 10/15/20：標準自動 Home 子流程範例
- `uhome.cpp` ProcessMotorHome：全機 Home 主控（透過 `HomeClass` push_back + `THomeFlag`）
