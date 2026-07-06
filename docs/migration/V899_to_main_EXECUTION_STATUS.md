# V899 to 主版(905.8) 移轉執行狀態與延後分析

> 產生日期：2026-06-09
> 來源：HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422
> 目標主版：HT9011UC_Code_V3.33.905.8_20260608_Ken_NEW_Ifor_Jimmy
> 用途：記錄本次移轉哪些已完成、哪些延後、為何延後，供日後主版正式釋放版本後比對正確性。

---

## 一、重大前提發現（影響所有判斷）

主版 905.8（Ken/Ifor 分支）**不是「V899 拿掉我的 //AI 修改」**，而是一條
**在某個更早 base 之前就分歧的平行分支**。它連我「非 //AI 標記」的舊 base
功能都可能缺。因此移轉前必逐功能查證主版是否有對應 base，不能假設。

### 實測證據
| 功能 | 主版狀態 | 含意 |
|------|----------|------|
| LOAD_Y_USE_MOTOR 陣列 | 0 處（連 2024 Jimmychiu base 都無） | 主版改用 LoaderUnload_StepMotor 不同機制 |
| chkLoaderY / chkEmptyY... UI 控制項 | 0（HandlerSys.dfm 無此控制項） | 移植需動 .dfm 新增 VCL 控制項（高風險） |
| bRemoteLotStart | 0 | 我新增旗標，主版無 |
| iOCRTriggerMode / bOCRSimBypassAOI | 0 | 我新增，主版無 |
| BuildIOErrMessage | 0 | 我新增方法，主版無 |
| RestoreLoaderYSpeedRaw | 0 | 我新增方法，主版無 |
| DOUBLE_EP_MULTI enum | 0（但 INSTALL_DOUBLE_EP=48 base 有） | Multi EP 未併入此分支 |
| bOEEFunction / CC_GIGAS / RunSite | 56 / 49 / 7 | 這些 base 主版有 |

### 第二個關鍵問題：跨群組檔案高度交織
同一檔案被多個群組（含已延後的 G1 Multi EP）共用：
- `cContact.cpp`：G1 Multi EP（L189/1615/13085/13089/13096…）+ G7 Gigas drop-guard（L3300-4374/10548-10775/18193-18311）交織
- `HS_Function.cpp`：大量 G1 Multi EP「port from V896 Ifor」（L819-1040）+ G11 + 診斷 log
- `database.cpp` / `HandlerSys.cpp`：G1 Multi EP + G6 Stepper Y + G14 交織

→ 在 G1（Multi EP）未裁決前，動這些共用檔的其他群組會與 G1 的延後狀態互相牽連，
  風險與一致性都不可控。

---

## 二、已完成（已逐 obj 編譯驗證 exit 0）

這些都是「新檔/新類別 drop-in」型，主版完全缺整塊 → 乾淨移植、低風險。

### Group 13：BootLog 當機診斷 ?
- 新檔 `Public\cBootLog.cpp/.h` drop-in（純 Win32 file I/O，無 VCL 相依）
- `.bpr` 登記 OBJ+FILE
- `HT9045.cpp` 注入 8 個 BootLog 檢查點 + catch(...)（Big5 寫入，保留主版獨有 fBinAOISel 5 行）
- 驗證：cBootLog.obj + HT9045.obj 編譯 exit 0

### Group 3：InArm HP 反碎片化 CompactSearch ?
- 新檔 `cSiteUseManager.cpp/.h` drop-in + `.bpr` 登記（OBJ 置於 cinitial.obj 後）
- `ainarm9045_1x4_4.cpp`：4 個 CompactSearch hook 移植（含 main 無大括號處已包裹）
- `ainarm9045_1x1_1.cpp`：3 處 baseline 對齊（int ret 宣告 / case 1200 / SHT2）
- `cinitial.cpp`：加 `SiteUseMgr.Init();`
- `ainarm9045_2x4_8.cpp`：我方 0 筆，免動
- 驗證：cSiteUseManager.obj / ainarm9045_1x4_4.obj / ainarm9045_1x1_1.obj / cinitial.obj 全編譯 exit 0

### Group 2（Type A 部分）：AutoClean CKPP + PickPlanner 新檔 ?
- 8 新檔 drop-in：`AutoClean\cCleanKitPickPlan.h`、`Common\PickPlanner\*`（7 檔）
- `.bpr` 登記 path 巨集 + 3 OBJ + 8 FILE 條目
- PickPlanner cpp 全自含（只 include 自身 header + vcl.h），無 AutoClean 相依 → 不影響連結
- 驗證：cArmPickPlan.obj / cInArmLoaderPickPlanner.obj / cOutArmPlacePlanner.obj 編譯 exit 0

---

## 三、延後（需使用者裁決後再處理）

判定原則：凡需要 (a) 改 .dfm 新增 VCL 控制項、(b) 與主版不同機制結構性調和、
(c) 在分歧函式 body 上 in-place 改安全關鍵運動/IO 碼、(d) 與已延後 G1 共用檔交織，
一律延後，避免在不確定基底上機械式硬移植。

| 群組 | 延後原因 |
|------|----------|
| **G2 Type D** AutoClean.cpp(25筆)+ainarm9045.cpp Phase3 | 含大型結構改寫（IArmPickPlanContext ~140行 / plan-driven pick 改寫）+ 多筆「revert to V898」（依賴主版確切基底）+ 安全關鍵。bUseCKPP 預設 OFF → 未整合無行為損失。 |
| **G6** Stepper Y LOAD_Y_USE_MOTOR | 主版 0（連 2024 base 都無）；需補非-//AI 陣列定義 + 將主版 LoaderUnload_StepMotor 結構性改寫 + **HandlerSys.dfm 新增 chkEmptyY/chkColorY/chkAuto1~3Y VCL 控制項**；馬達 dispatch 安全關鍵。 |
| **G1** Multi EP DOUBLE_EP_MULTI | 主版有 INSTALL_DOUBLE_EP base 但無 DOUBLE_EP_MULTI enum；Ifor 是否已有其他形式 Multi EP 未知（恐雙實作衝突）；牽動 adam6024/cContact/ContactForce/HandlerSys/database/HS_Function/AutoClean 多檔。**最高風險，原本就標延後。** |
| **G7** Gigas Contact 掉料防護 | cContact.cpp 與 G1 Multi EP 同檔交織；屬安全關鍵運動碼（case 3050 / SetItemData / iFTestBackItem 還原 / IndexHasIC 假死防護）；分歧 body in-place 改危險。 |
| **G9** OEE/Remote Lot Start | bRemoteLotStart 主版無（新增）；CosFunction/automation/Command/auto9045 in-place；需確認 HTSET 720 與主版 OEE base 整合點。 |
| **G8** PTI Lot Start + FTP | uLotInfo/cSortCT/FTPClient/main in-place；FTP timeout + log 改 + btnClearCount 分流，分歧 body 需逐處驗證。 |
| **G4** InArm Loader 看門狗 | ainarm9045.cpp case 10/15 + acatchtray.cpp case 148 in-place；需確認主版 case 結構一致。 |
| **G5** TrayMapping/LaserScan/Home hotfix | RestoreLoaderYSpeedRaw 主版無（新增）；mymotor/uhome/cTrayMapping in-place；屬馬達 Home/速度安全碼。 |
| **G10** OCR 模擬 + 觸發模式 | iOCRTriggerMode/bOCRSimBypassAOI 主版無；含「OCR Area Simulation 獨立 UI tab」→ 恐需動 .dfm。 |
| **G11** Yield Monitoring | uYieldMonitoring + main.cpp sbYieldClick[39] 權限 + HS_Function（與 G1 交織）。 |
| **G12** IO 錯誤訊息售服化 | 主版 GetIOErrStr 為短英文且結果寫進 log；V899 設計「log 短 / UI 用 BuildIOErrMessage」兩者耦合，主版 call-site 結構分歧（fiosetview/MachineTypeChoice 分支）；單獨加方法為無用 dead code。 |
| **G14** Misc 小修彙整 | 多筆獨立小修，需逐筆在分歧基底驗證；可在主版正式釋放後逐項處理。 |

---

## 四、使用者需裁決事項（最後一併處理）

1. **G1 Multi EP 策略**：主版無 DOUBLE_EP_MULTI enum。現在從 V874.3/V896 移植，
   還是等主版正式釋放（Ifor 可能已含某形式 Multi EP）再 diff？牽動最多共用檔，
   建議先決定，因為 G7/G9/G11 與其同檔交織。
2. **G6 / G10 的 .dfm**：是否同意動 HandlerSys.dfm / OCR 相關 .dfm 新增 VCL 控制項？
   若不動 .dfm，這兩組無法完成。
3. **G6 Loader-Y 機制**：主版用 LoaderUnload_StepMotor，V899 用 LOAD_Y_USE_MOTOR[]。
   要全 feature graft（含 2024 base）取代主版機制，還是維持主版機制？
4. **移轉時機**：是否等主版正式釋放版本後，以「主版正式版 vs V899」逐群組 diff 再移轉，
   會比現在這條中間分支（905.8 Ken/Ifor）穩。

---

## 五、移轉鐵律（續用）

- Big5 .cpp/.h 永不用 IDE 編輯器改 → 一律 PowerShell `GetEncoding(950)`
- 新檔含中文用 binary `Copy-Item`（byte-exact 保 Big5）；`.bpr` 純 ASCII 可 ASCII 編輯
- 任何寫入前 anchor-count==1 驗證；先備份（.bak_premigration / .bak_g2）
- old_str 一律取自「主版逐行 bytes」（主版註解對齊空格與 V899 不同）
- 已併入主版的 4 筆我方 //AI 跳過：aoutarm9045_2x8_8.cpp x3(0512)、asendic_Loader.cpp x1(0409)
- 每群組以 targeted `make -f HT9045.mak "..\..\Obj\<name>.obj"` 驗證 exit 0
- 不確定一律延後，不在分歧基底上機械硬移植（安全關鍵運動/IO 尤甚）