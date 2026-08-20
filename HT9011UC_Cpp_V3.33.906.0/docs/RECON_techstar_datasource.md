# RECON：SetTechDataToProd 電池「其餘七個成員」資料源偵察（W906-FW-TECH-RECON）

日期：20260820
範圍：唯讀 recon。未 build / 未跑 ctest / 未起任何背景行程 / 未 commit。除本檔外未動任何檔案。
golden：`HT9011UC_Code_V3.33.906.0_20260618`（與 `docs/RECON_binstar_datasource.md` 同一棵，`_Code_` 非 `_Cpp_`，唯讀）。
port 樹：`HT9011UC_Cpp_V3.33.906.0`（唯讀，本次只新增這一份 .md）。

## 0. 開頭結論（先講重點，避免被表格淹沒）

**`SetTechDataToProd` 電池的全部 11 個成員（orchestrator 本身 + 10 個電池成員）今天在 V906 樹裡
都已經翻譯完成、且都是 ACTIVE（不是 stub、不是 `#if 0` 死碼），只有一個內部細節例外**
（Shuttle 內部一行 `fShuttleMove->ReadData()` 因為依賴的 form 方法沒翻，被 `#if 0` 掉，
但那一行本來就被一個預設 false 的旗標擋住，詳見 §2）。這跟本任務原本预期「未翻就標 UNPORTED」
的假設不同——沒有任何一個成員是 UNPORTED，全部翻完了。

**但「翻完」不等於「wb_serve 呼叫得到」**：orchestrator `SetTechDataToProd()`
本身在 wb_serve 的執行環境下呼叫，只會跑到第一行 `SetTechDataToProd_Tray()`
就被 `InitialOK==false` 擋下提早 return（§3 有量測證據：`InitialOK` 全域預設 `false`，
V906 樹裡把它設成 `true` 的地方只有 tests/*.cpp，沒有任何非測試程式碼會在 wb_serve
執行路徑上把它設成 true）。所以想要用這 7 個成員，**唯一可行路徑是比照 `_Yield` 的先例，
繞過 orchestrator，直接呼叫個別電池成員**——這件事對全部 7 個成員都成立，不是個案。

**讀寫性：這 7 個成員全部是純記憶體運算（`Tech.*`/`DeviceForm.*`/`Offset.*`/`TestIF.*`/
`IniConfig.*` 讀進來，`Prod.*` 寫出去），對磁碟的接觸只有一條，而且目前是死碼**：
Shuttle 內部的 `fShuttleMove->ReadData()`（讀 `IniData\Data\<recipe>\HandlerCondition.Data`，
純讀無回寫，但今天在 V906 被 `#if 0` 掉，見 §2-b）。**沒有任何一個成員會像 `BinSelect`
一樣觸發 `CheckAndReadIniData` 的懶 seed 回寫。**

## 1. 成員清單

golden orchestrator 本體（`cinitial.cpp:11636-11673`）依序呼叫：

```
11638   SetTechDataToProd_Tray();
        if(InitialOK==false) return;      // <-- Tray 在這個 gate 之前，其餘全部在之後
11660   SetTechDataToProd_TrayArm();
11661   SetTechDataToProd_Shuttle();
11662   SetTechDataToProd_InArm();
11663   SetTechDataToProd_Index();
11664   SetTechDataToProd_OutArm();
11665   SetTechDataToProd_AutoClean();
11666   SetTechDataToProd_AOI();
11667   SetTechDataToProd_Yield();          // 已由 FW-BIN1 直接呼叫（bin.* 鏈）
11669   if(USE_OUT_SORT_ARM!=eartUninstall) // 預設 eartUninstall（見下），一般 9045 不會進來
11671       SetTechDataToProd_SortShuttle();
11672       SetTechDataToProd_SortArm();
```

任務指定的「其餘七個成員」= `TrayArm / Shuttle / InArm / Index / OutArm / AutoClean / AOI`
（排除 `_Tray`：它在 `InitialOK` gate 之前，性質上是「Tray 表單幾何整理」而非教導資料下推；
排除 `_SortArm`/`_SortShuttle`：只在 `USE_OUT_SORT_ARM!=eartUninstall` 時才跑，而
`USE_OUT_SORT_ARM` 預設值就是 `eartUninstall`（`cmydef.cpp:3134 int USE_OUT_SORT_ARM=eartUninstall;`），
由 `database.cpp:706 CheckAndReadIniDataGeneral("OutSortArm","USE_OUT_SORT_ARM",eartUninstall)`
從 Gerneral.ini 載入，是 HT9046AU 專用的機型分支，一般 9045 產線不會進這段）。
`_Tray`/`_SortArm`/`_SortShuttle` 仍在下面附一句話評估，不深入。

| # | 函式 | golden 行號 | 翻譯狀態 | V906 位置 | 波次/日期 |
|---|---|---|---|---|---|
| 1 | `SetTechDataToProd_TrayArm` | `cinitial.cpp:10204-10395` | ACTIVE，0 gate | `cinitial.cpp:5751`（註解區塊 :5743-5750） | AI(W906-PT-n3) 20260810 |
| 2 | `SetTechDataToProd_Shuttle` | `cinitial.cpp:10397-11230` | ACTIVE，1 內部行被 `#if 0`（見 §2-b） | `cinitial.cpp:5966`（註解區塊 :5945-5965） | AI(W906-PT-n3) 20260810 |
| 3 | `SetTechDataToProd_InArm` | `cinitial.cpp:8572-9256` | ACTIVE，0 gate | `cinitial.cpp:14535`（註解區塊 :14498-14534） | AI(W906-PT-W6-n5) 20260810 |
| 4 | `SetTechDataToProd_Index` | `cinitial.cpp:9258-9509` | ACTIVE，1 gate（N3-G4，純記憶體，見 §2-a） | `cinitial.cpp:5470`（註解區塊 :5462-5469） | AI(W906-PT-n3) 20260810 |
| 5 | `SetTechDataToProd_OutArm` | `cinitial.cpp:9511-10080` | ACTIVE，0 gate | `cinitial.cpp:11218`（註解區塊 :11204-11217） | AI(W906-PT-n4) 20260810 |
| 6 | `SetTechDataToProd_AutoClean` | `cinitial.cpp:11269-11315` | ACTIVE（有兩份定義，一份是真身一份 `#if 0`，見 §2-f） | `AutoClean/AutoClean.cpp:313`（真身，ACTIVE）；`cinitial.cpp:11816`（`#if 0` 重複件） | AutoClean.cpp 版：AI(W906-AutoCleanCluster) 20260722/0728；cinitial.cpp 重複件：AI(W906-PT-n4) 20260810，GATE n4-6 |
| 7 | `SetTechDataToProd_AOI` | `cinitial.cpp:11317-11436` | ACTIVE，0 gate | `cinitial.cpp:6832`（註解區塊 :6826-6831） | AI(W906-PT-n3) 20260810 |

**附：另外三個成員（非本次「七個」範圍，一句話評估，不深入）**

| 函式 | golden 行號 | 翻譯狀態 | V906 位置 | 備註 |
|---|---|---|---|---|
| `SetTechDataToProd_Tray` | `:8461-8570` | ACTIVE，0 gate | `cinitial.cpp:14386` | 在 `InitialOK` gate **之前**，永遠跑；純 Tray 表單方向/型別整理，不碰 `Tech.*` |
| `SetTechDataToProd_SortArm` | `:10082-10202` | ACTIVE，0 gate | `cinitial.cpp:10061` | HT9046AU 專用，一般 9045 因 `USE_OUT_SORT_ARM` 預設值不會執行 |
| `SetTechDataToProd_SortShuttle` | `:11232-11267` | ACTIVE，0 gate | `cinitial.cpp:10192` | 同上 |

驗證指令（20260820 下午執行）：
```
Grep "SetTechDataToProd" HT9011UC_Code_V3.33.906.0_20260618\*.cpp        -> 定位 orchestrator 呼叫點 :11636-11673
Read  HT9011UC_Code_V3.33.906.0_20260618\cinitial.cpp:11636-11680        -> 逐行核對呼叫順序與 InitialOK gate 位置
Grep "SetTechDataToProd_(Tray|TrayArm|Shuttle|InArm|Index|OutArm|AutoClean|AOI|SortArm|SortShuttle)\b" HT9011UC_Cpp_V3.33.906.0\*.cpp
    -> 逐一定位 V906 側定義行號與波次註解（cinitial.cpp:4558-4565、7414-7427、10437-10446、13092-13102）
Grep "USE_OUT_SORT_ARM\s*=" HT9011UC_Code_V3.33.906.0_20260618\*.cpp     -> 確認預設值 eartUninstall（cmydef.cpp:3134, database.cpp:706）
```

## 2. 逐成員讀寫性分析

### 2-a. `SetTechDataToProd_Index`（golden :9258-9509）—— 純記憶體，唯一「gate」也是純記憶體

**讀**：`Tech.iTestZDown/iHT9040TestY1_Front/.../iTestZ1ShutlePick/iTestZ2ShutlePick`、
`DeviceForm.ContactMode/IndexArmPick[]/IndexPlace[]`、`Offset.iIndexArmPickUp[]/iIndexArmPlace[]`、
`TestIF.bNS7000kit/dSiteYPitch`、`TestIF_File.iYPitchOffsetMode/iShuttleMode/iShuttle_Sel`、
`Teach.iLoadCellY1/Y2`、`DeviceForm_File.dLoadCellZ1Down/Z2Down`、`CosFunction.*`、`IniConfig.*`、
`MOT[MTestZ1/MTestZ2].Motor->Enable` 與 `.IndexPickLimit`（僅在 `#ifndef SOFT_SIMULTE` 區塊內比較，
不寫）。**寫**：`Prod.TestY1_Front/Middle/.../TestZ1_Safe/Pick/Place/Test/TestZ2_.../
iTestDown_Z1/Z2/InSHT[0/1].iRight/iLoadCellY1/Y2/dLoadCellZ1Down/Z2Down`，全部是 `Prod` 成員賦值。

**「gate」的真相**：golden :9395-9399 `if(fAutoTeach->IsRun()) { Prod.TestZ1_Pick+=...; }`。
V906 把這標成 gate（N3-G4，`cinitial.cpp:4599-4605`）是因為 `TfAutoTeach`（golden
`AutoAlignment/AutoTeach.h`）在這棵樹裡沒有 port body/facade，所以整個 `if` 連同 `fAutoTeach->`
呼叫都沒辦法翻——但我對 golden 的 `TfAutoTeach::IsRun()`（`AutoTeach.cpp:217-240`）與
`GetShuttlePickUpOffsetWhenAutoTeach()`（`AutoTeach.cpp:1212-1215`）逐行讀過：**兩者都是
純記憶體**——`IsRun()` 只比較 `CosFunction.bManualSteplAutoTeach`/`IniConfig.bA56EnableAutoTeachFunciton`/
`LastSet.iAutoTeachStep`（全部已載入的既有全域），`GetShuttlePickUpOffsetWhenAutoTeach()`
只是 `return IniConfig.iA56ShuttlePickUpOffsetWhenAutoTeach;`。**沒有任何檔案 I/O**，
就算這個 gate 補上去，也不會給這個函式帶來磁碟風險——唯一影響是「Auto-Teach 模式時
Index Z pick 高度不會加教導模式偏移」，正常量產不受影響（`IsRun()` 在沒開教導表單時恆為 false）。

**`ShowMyMessage` 呼叫**（golden :9406/9412/9418/9424，`#ifndef SOFT_SIMULTE` 區塊內，超限保護訊息）：
V906 的 `ShowMyMessage`（`canary_support.cpp:143-158`）是**純 log stub**（`std::printf`
+ 計數器，不彈窗、無 modal），已核對原始碼確認不是 Windows `MessageBox`/VCL `MessageDlg`。
這條線在 headless wb_serve 裡呼叫是安全的，不會卡住行程。

**結論：`SetTechDataToProd_Index()` 今天 100% 可以在 wb_serve 裡直接呼叫，零額外磁碟風險。**

### 2-b. `SetTechDataToProd_Shuttle`（golden :10397-11230）—— 唯一真的碰到磁碟路徑的成員，而且那條路徑今天是死碼

**讀**：`Tech.iOutSH1/2BarCodePos/iInShuttle1/2Right/iShuttle1/2BusyHalfOffset/iShuttle1/2*Pitch/
iInSH1/2SenICDetectZ1/Z2/iInSH1/2SenICDetectPos` 等一大票 `Tech.*` 教導欄位、`TestIF.dSiteXPitch`、
`Offset.iSHHalft[]`、`In_Shuttle_Auto_Latch`（全域 int，見下）、`iInShtZRange`
（`CheckAndReadIniDataGeneral("Shuttle","iInShtZRange",250)`，走 Gerneral.ini，已被
`--dry` 涵蓋）。**寫**：`Prod.iOutSHBarCodeDetectPos2x8[][]/iBS1/2Right_Half/
iShuttle1/2120/180Pitch/iInSH1/2SenICDetectZ1/Z2`。

**唯一磁碟接觸點**：golden :11224-11225：
```cpp
if(In_Shuttle_Auto_Latch==eInSHAutoLtc)   // In_Shuttle_Auto_Latch 預設 0 (eInSH8Sen)，eInSHAutoLtc=1
    fShuttleMove->ReadData();
```
`TfShuttleMove::ReadData()`（golden `ShuttleMove.cpp:2252-2264`）本體：
```cpp
S=GetLastOpenFN();
szDir.sprintf("%s%s\\HandlerCondition.Data", DataPath, S);
Prod.iInSH1SenICAddPos=ReadIniData(szDir, "Shuttle", "InSH1SenICAddPos", 0);
Prod.iInSH2SenICAddPos=ReadIniData(szDir, "Shuttle", "InSH2SenICAddPos", 0);
fCCLink->iInShtLtcPercent=ReadIniData(szDir, "Shuttle", "InShtLtcPercent", 80);
```
路徑家族是 `DataPath + <recipe>\HandlerCondition.Data`——跟 bin recon 已經點名的
`Binasgn*.Data`（`docs/RECON_binstar_datasource.md` §2g）**同一個目錄家族**
（`D:\HT9045\IniData\Data\<recipe>\`），但**這次是不同檔名**（`HandlerCondition.Data`）。

**這次是純讀，不是懶 seed 回寫**：我逐行核對過 `ReadIniData`（`common.cpp:510-564`，
plain 版，注意跟 `CheckAndReadIniData` 是兩個不同名字的 helper 家族）的四個型別重載，
**每一個都只呼叫 `INIFile->ReadXxx(...)`，沒有任何一行 `WriteXxx`**——跟
`CheckAndReadIniData`（`common.cpp:397-508`）逐一比對過，只有後者在 `ValueExists==false`
時才 `WriteInteger/WriteBool/...`。`OpenIniFile()`（`common.cpp:323-334`）也只是
`new TIniFile(FileName)`，VCL/vclcompat 的 `TIniFile` 建構子不主動建檔（缺檔時
`ReadXxx` 走 Win32 `GetPrivateProfileString` 語意回傳預設值，不建立檔案）。
**所以就算這行被呼叫，也不會在 `D:\HT9045\IniData\Data\` 底下新增或修改任何位元組。**

**今天的狀態：這一行在 V906 是死碼**——`cinitial.cpp:6815-6816`：
```cpp
#if 0 // N3-G5: blocked by TfShuttleMove::ReadData (forms/fShuttleMove.h:53-108 stand-in has bShuttleRetry + btRetry only)
    if(In_Shuttle_Auto_Latch==eInSHAutoLtc)
```
因為 V906 的 `fShuttleMove` facade（`forms/fShuttleMove.h:53-108`）只有兩個成員
（`bShuttleRetry`、`btRetry`），沒有 `ReadData()` 方法，這行連翻都翻不了，被整段
`#if 0`。**Gate 生效範圍精確地只影響 `In_Shuttle_Auto_Latch==eInSHAutoLtc`（新式雙感測器
Latch 判別）這個分支，且 `In_Shuttle_Auto_Latch` 預設值是 0（`eInSH8Sen`，
`cmydef.cpp:3273`/V906 對應同值），要靠 Gerneral.ini 的 `[System]In_Shuttle_Auto_Latch=1`
才會走到這裡——這個值本身走 `CheckAndReadIniDataGeneral`，已被 `--dry` 涵蓋。**
下面四行 `Prod.iInSH1/2SenICDetectZ1/Z2=Tech.iInSH1/2SenICDetectZ1/Z2-iInShtZRange;`
（golden :11226-11229）**保持 ACTIVE**（不受這個 `#if 0` 影響），只是吃的是既有 `Tech.*`
值而非剛重新讀出的值——跟 N3-G5 註解自己講的一致。

**結論：`SetTechDataToProd_Shuttle()` 今天呼叫，磁碟接觸為零（唯一的磁碟路徑本身是死碼）。
就算日後把 `TfShuttleMove::ReadData()` 翻譯進來，也是純讀不回寫，風險遠低於 `BinSelect`
的 `CheckAndReadIniData` 懶 seed 模式——但仍然是一條「這次 `--dry` 沒有專門重導」的
第三方硬編路徑家族（跟 bin recon 點名的 `Binasgn*.Data` 屬同一目錄，若 recipe scratch-copy
窗口已經生效——FW-BIN1 已經把整個 recipe 資料夾複製到 scratch 並重導 `DataPath`——
那麼 `HandlerCondition.Data` 也會被一起複製進 scratch，這條路徑其實已經間接受到保護，
不需要額外處理）。**

### 2-c. `SetTechDataToProd_TrayArm`（golden :10204-10395）—— 純記憶體

**讀**：`Tech.iTrayXLoader/iTrayXEmpty/iTrayXColor/iTrayXAuto1-6/iTrayXClean/iTrayXOCR`、
`Offset.iTrayArmX[]/iTrayArmX_ART[]`、`LoadForm->/EmptyForm->/ColorForm->/AutoForm[]->iPickUp`、
`CUSTOMER_CODE`、`SubMachineType`。**寫**：`Prod.iXTrayLoad/iXTrayEmpty/iXTrayColor/
iXTrayAuto[]/iXTrayClean/iXTrayOCR/iXTrayLoad_ART/...`（純 X 座標表）。
全文（190 行）掃過一遍 `ReadIniData|WriteIniData|TIniFile|DataPath|GetRecipeFileName|
fopen|TFileStream` 等關鍵字：**零命中**。零 I/O，零 gate。

### 2-d. `SetTechDataToProd_InArm`（golden :8572-9256）—— 純記憶體，但有一個「資料不完整」的隱藏依賴

**讀**：`Tech.iInArmPlate1/2X/Y/iHP1/2LaserX/Y/iInArmLoadStageX/Y/PickZ2/iInArmZHeightSub[][]/
iInArmZHeightSub_16[][]/iInArmPlatePickZ2/iInArmAutoCleanX/Y`、`HotPlateForm.*`、
`InArmOffSet[]->GetX/GetY/GetPickUp/GetPlace`、`IniConfig.*`、`TestIF.dAutoClean_XStart/YStart`、
`TestIF_File.bAutoClean_UseNSKit/UseTray`、`Teach.iAutoCleanPick`、`UserDefForm_File[].bUseThickTray`。
**寫**：`Prod.XInArm_Tray_Pick/Plate1_Pick/Plate2_Pick/AutoClean_Pick、YInArm_.../ZInArm_.../
ZInArmSafe[][]、iHP1/2LaserX/Y、iInArmDailyCorrelationX/Y/PickZ`。**全文掃過一遍同一組 I/O
關鍵字：零命中。**

**隱藏依賴（不是 I/O 風險，是資料完整度風險）**：InArm 的多站點展開迴圈是
`for(int i=0;i<InArmSuck.iMaxRow;i++) for(int j=0;j<InArmSuck.iMaxCol;j++)`
（golden :8689-8711 等多處）。`InArmSuck` 是 `TMyKitSuck` 的全域實例，它的 `iMaxRow`/
`iMaxCol` **只有一個寫入者**：`SetMyKitSuckItemAmount()`（golden :5504-5612）——而這個函式
在 V906 是**整個函式體被 `#if 0`**（GATE n4-1，`cinitial.cpp:10450-10458`，因為它要呼叫的
`TMyKitSuck::SetItemAmount/SetMotorCount` 只存在於**另一個** ODR 不相容的 `TMyKitSuck`
版本——`mykitsuck.h:248-249`，該檔案 `CMakeLists.txt:2099` 刻意不註冊，跟 `KNOWLEDGE.md`
記錄的「V906 有兩個 TMyKitSuck」陷阱是同一個坑）。所以今天 `InArmSuck.iMaxRow`/`iMaxCol`
**恆為 0**（`aHotPlateSubstrate.cpp:692` 自己的註解也寫了：「Offline the global carry-kits
have iMaxRow==iMaxCol==0」），InArm 的多站點展開迴圈**一次都不會執行**——但迴圈外那些
用 `[iInArmYBase][iInArmXBase]` base cell 的賦值（例如 `Prod.XInArm_Tray_Pick[iInArmYBase]
[iInArmXBase]=...`）**是迴圈外的獨立陳述式，不受影響，一定會執行**。**結論：InArm 今天呼叫
只會填出第一顆嘴（base cell）的位置資料，其餘 nozzle 的位置表維持零初始化，這不是 InArm
自己的翻譯缺陷，是它與 SortArm/SortShuttle 以外、另一個尚未整併的 GATE n4-1 共用的依賴。**

### 2-e. `SetTechDataToProd_OutArm`（golden :9511-10080）—— 與 InArm 同構，同一個隱藏依賴

**讀/寫**結構與 InArm 完全對稱（`Tech.iOutArm*`→`Prod.XOutArm_.../YOutArm_.../ZOutArm_...`），
迴圈同樣是 `for(i<OutArmSuck.iMaxRow) for(j<OutArmSuck.iMaxCol)`——**同一個 GATE n4-1
（`SetMyKitSuckItemAmount` 未執行）連帶效應**：`OutArmSuck.iMaxRow/iMaxCol` 恆為 0，
多站點展開迴圈不執行，只有 base cell（`[iOutArmYBase][iOutArmXBase]`）會被填。
這件事 V906 自己的 PT-n4 波次頭部註解已經點名（`cinitial.cpp:10504-10511`：
「SetTechDataToProd_OutArm fills only the [iOutArmYBase][iOutArmXBase] base cell of each
Prod grid」），我重新核對過 golden 逐行確認屬實。**零 I/O 關鍵字命中，零 gate（body 本身）。**

另外 OutArm 讀取 `iShuttleTempPos[2][2]`（golden 檔案作用域全域，:75），這個全域是
**`SetTechDataToProd_Shuttle` 的唯一寫入者**（golden :10429-10470）——這代表**呼叫順序敏感**：
若 wb_serve 只挑著呼叫 OutArm 而跳過 Shuttle，`iShuttleTempPos` 會停在編譯期預設值
`{{0,0},{0,0}}`（`cinitial.cpp:4731` 的 static 初始化），OutArm 仍能正常執行，只是
「高溫 130 度位置補償」這個修正量會是 0（golden 原意如此，不算錯，只是資料不完整）。
**若要接這 7 個成員，呼叫順序必須照 golden orchestrator 的順序：TrayArm → Shuttle →
InArm → Index → OutArm → AutoClean → AOI，不能任意挑子集。**

### 2-f. `SetTechDataToProd_AutoClean`（golden :11269-11315）—— 純記憶體，但有一個已知、已解決的重複定義

**讀**：`IniConfig.bEnableAutoCleanFunction`、`iRunACSmart`（全域 int，預設 0，只在生產期
運行邏輯裡被寫，`cmydef.cpp:5511`，非 ini 載入值，boot 時恆為 0）、`TestIF_File.bACSmart/
iACSmart_Count/iACSmart_ContactMode/DropHigh/ContactTime/ContactCount`、`TestIF.iAutoClean_
ContactCleanHeight/iPadThickness/iAutoClean_ContactShiftHeight`、`IniConfig.bSPILFunction`、
`Prod.TestZ1/2_Test`、`Prod.TestZ1/2_Drop_Offset`、`Offset.iIndexArmContact[]`。
**寫**：`Prod.iAutoClean_ContactMode/DropHigh/ContactTime/ContactCount/iAutoCleanZ_Contact[]/
Drop[]/Shift[]`。**全文掃過同一組 I/O 關鍵字：零命中。**

**重複定義（已由 V906 自己解決，非新發現，但值得記一筆避免日後誤動）**：這個函式在 V906
存在**兩份原始碼**：
1. `AutoClean/AutoClean.cpp:313-359`——**真身，ACTIVE，會被連結**。由更早的
   AI(W906-AutoCleanCluster) 20260722/0728 波次翻譯（當時 `cinitial.cpp` 這個檔案在
   port 樹裡還不存在，所以那個波次把這一個函式單獨搬到它唯一呼叫點所在的檔案裡）。
2. `cinitial.cpp:11816-11859`——**`#if 0` 死碼**（`GATE n4-6`，`cinitial.cpp:11790-11814`
   有完整的整合者說明）。這份是後來 PT-n4 波次（20260810）依 golden 順序翻譯時重新產生的，
   落地時發現 (1) 已經存在，為了避免 `multiple definition` link error 主動把自己 `#if 0` 掉，
   並在註解裡留下明確的整合建議：「保留 cinitial.cpp 這份（因為 AutoClean.cpp 那份把
   `優化`/`SPIL佶融要求` 誤植成 `優先`/`SPIL客戶需求`，是打字誤差，執行邏輯兩份完全相同）,
   刪掉 AutoClean.cpp:302-355 那份，再刪掉這個 `#if 0`」——**這是尚待整合者執行的家務事，
   不影響今天呼叫的正確性（兩份邏輯相同），但如果日後有人「順手」把 cinitial.cpp 這份的
   `#if 0` 拿掉而沒有同時處理 AutoClean.cpp 那份，會立刻是連結期 `multiple definition`
   錯誤（不是靜默錯誤，安全失敗模式）。**

**結論：`SetTechDataToProd_AutoClean()` 今天已經是 ACTIVE（`AutoClean.cpp:313` 那份），
零 I/O，可以直接呼叫，不需要任何額外動作。**

### 2-g. `SetTechDataToProd_AOI`（golden :11317-11436）—— 純記憶體，一個分支有跟 InArm/OutArm 相同的隱藏依賴

**讀**：`tAOISetup.bEnabledAOI`、`USE_Fix_AI_CCD`、`TestIF_File.bEnableFix2BGAAICCD`、
`USE_Scanner_AOI_Inspection`、`MACHINE_HAS_AUTO_ALIGNMENT_CCD`、`Tech.iInArmXCCDAlignmentPos/
YCCDAlignmentPos/.../M_iTopView_X/Y/Pick/Place/M_iPADView_X/Y/Z/M_iBGAView_X/Y/Z/
M_ScannerAOI_X/Y/Z/iMLoaderYCarPos/YOCRPos/YSurePos`、`OutArmOffSet[]->GetX/GetY/
GetPickUp/GetPlace`。**寫**：`Prod.OutArm_BGA_Place[][]/OutArm_Pad_Place[][]/
OutArm_ScannerAOI_Pick[][]/Place[][]/iInArmXCCDAlignmentPos/.../iTopView_X/Y/Pick/Place/
iPADView_.../iBGAView_.../iScannerAOI_.../iMLoaderYCarPos/...`。**零 I/O 關鍵字命中。**

**同一個隱藏依賴，範圍更窄**：函式開頭兩個分支（`tAOISetup.bEnabledAOI`、
`USE_Fix_AI_CCD && TestIF_File.bEnableFix2BGAAICCD`）用
`for(i<OutArmSuck.iMaxRow) for(j<OutArmSuck.iMaxCol)` 迴圈填 `Prod.OutArm_BGA_Place[][]`/
`Prod.OutArm_Pad_Place[][]`——**跟 InArm/OutArm 同一個 `OutArmSuck.iMaxRow==0` 效應**，
但這裡**沒有 base-cell 迴圈外賦值**（不像 InArm/OutArm 先填 base cell 再展開），所以
若 `iMaxRow==0`，這兩個陣列**完全不會被寫入**，維持零初始化。**實務影響很小**：
`tAOISetup.bEnabledAOI`（Vitrox AOI 選配）與 `USE_Fix_AI_CCD`（Fix2 BGA AI CCD 選配）
都是特定客戶硬體選配旗標，一般機台預設關閉，這個分支平常就不會進入。
第三個分支（`USE_Scanner_AOI_Inspection`）結構相同，同樣的隱藏依賴、同樣是選配功能。
**函式其餘 80% 內容（CCD 對位、TopView/PADView/BGAView/ScannerAOI 座標）不依賴
`OutArmSuck`，正常執行。**

## 3. boot 鏈可行性判定

**前提量測**：`InitialOK` 全域預設 `bool InitialOK=false;`（`cmydef.cpp:285`）。
全樹 grep `InitialOK\s*=\s*true` 只命中 `tests/*.cpp`（`test_barcode_8ccd_glue.cpp`、
`test_automation.cpp`、`test_AutoClean.cpp`、`test_auto9045.cpp`、`test_testertcp_socket.cpp`、
`test_sim_io.cpp`、`test_showbinselect_core.cpp`、`test_w6_2_inarm_search.cpp`、
`test_uHGemEquipment.cpp` 等），**沒有任何非測試程式碼會把它設成 true**——這代表：

**若 wb_serve 呼叫 orchestrator 本身 `SetTechDataToProd()`，只會執行到 `SetTechDataToProd_Tray()`
就在 `if(InitialOK==false) return;`（golden :11640-11643）被擋下，本次盤點的 7 個成員
一個都不會執行。** 這跟 bin recon 對 `_Yield` 下的結論完全一致，而且對全部 7 個成員都成立，
不是巧合——golden 把這 7 個成員全部放在 `InitialOK` gate 之後，是設計上要求「motor.db 讀完
才下推教導資料」，wb_serve 今天的唯讀 bring-up 不做完整的 motor.db 載入（那是更大一包
硬體初始化工作，超出本次範圍），所以 `InitialOK` 停在 false 是符合現況的，不是 bug。

**可行路徑：比照 `_Yield` 先例，繞過 orchestrator，直接呼叫個別電池成員函式。**
7 個成員都通過本次的讀寫性審查（§2），都可以直接呼叫，不需要任何 scratch-redirect
或額外保護：

| 成員 | 可行性 | 條件 |
|---|---|---|
| TrayArm | 可直接呼叫 | 無 |
| Shuttle | 可直接呼叫 | 內部磁碟路徑已是死碼，即使日後補上也是純讀 |
| InArm | 可直接呼叫 | 只會填出 base cell（單站點），多站點表維持零值，非本次翻譯缺陷 |
| Index | 可直接呼叫 | `fAutoTeach` gate 邏輯確認純記憶體，不影響安全性 |
| OutArm | 可直接呼叫 | 同 InArm 的 base-cell 限制；且依賴 Shuttle 已先跑過（見下） |
| AutoClean | 可直接呼叫 | 已是 ACTIVE，兩份重複定義中一份是死碼，不影響正確性 |
| AOI | 可直接呼叫 | 大部分內容安全；`bEnabledAOI`/`Fix2BGAAICCD` 分支若開啟會受 base-cell 限制影響（無 base cell 可退），但這兩個選配旗標一般機台預設關閉 |

**呼叫順序是硬性要求，不是建議**：`iShuttleTempPos`（Shuttle 寫、InArm/OutArm 讀）與
`Prod.TestZ1/2_Test`（Index 寫/由更早的 `DoSetupSystemToProd()` 寫、AutoClean 讀）
都是跨成員的資料依賴。**若要接這 7 個成員，必須嚴格按照 golden orchestrator 的順序呼叫：
TrayArm → Shuttle → InArm → Index → OutArm → AutoClean → AOI**，不能像 `_Yield`
那樣單獨挑一個出來呼叫（`_Yield` 之所以能單獨呼叫，是因為它讀的 `BinSelect[]`/
`iTo6PosUnload[]`/`ArmData[]` 家族都是自給自足或已被其他 static-init 保證，這 7 個成員
彼此之間有直接的讀寫先後關係，`_Yield` 沒有）。

**一個範圍外但相關的依賴，只記錄不深入**：`SetTechDataToProd_AutoClean` 讀的
`Prod.TestZ1_Test`/`Prod.TestZ2_Test` 除了被 `SetTechDataToProd_Index` 在特定分支寫
（golden :9475/9504，`TestIF_File.iShuttleMode==1` 時歸零)，主要寫入者其實是**電池外**
的 `DoSetupSystemToProd()`（golden :6650-7555，PT-W8-n2 波次翻譯，:6701-6887 一大段
`Prod.TestZ1/2_Test=DeviceForm.IndexContact[]+Offset.iIndexArmContact[]+...`）。
**這個函式不在本次「七個成員」範圍內，也已經是 ACTIVE（PT-W8-n2 波次翻譯完成，
10 個 gate，見該波次頭部註解 `cinitial.cpp:7398-7488`）**——如果 wb_serve 只呼叫這 7 個
成員而不呼叫 `DoSetupSystemToProd()`，`Prod.TestZ1/2_Test` 會停在編譯期預設值 0，
AutoClean 算出來的 `iAutoCleanZ_Contact/Drop/Shift` 會是「用 0 當基準」的數字，不是
真實的 contact 測試高度。這不影響「能不能安全呼叫」，只影響「算出來的值有沒有意義」——
若要讓 AutoClean 的診斷值有意義，`DoSetupSystemToProd()` 也要接進同一條 boot 鏈，
但那是一個 906 行、10 個 gate 的大函式，值得另開一輪 recon，本次不深入判定它的安全性。

## 4. 消費端地圖

**現況：這 7 個成員寫出的 `Prod.*` 欄位，今天 `WebBridgeTags.cpp`/`WebBridgeTags.h`
一個都沒有引用**（`Grep "Prod\.(XInArm|YInArm|ZInArm|XOutArm|YOutArm|ZOutArm|TestZ|TestY|
iAutoClean|iBS1|iBS2|iShuttle|iOutSH|iInSH|Fix_Pick)"` 對 `WebBridge/`、`WebBridgeTags.h`
兩處皆零命中）。這跟 `_Yield` 的 `BinSelect[]`→`bin.*` 六個 tag 已經是現成消費者的情況
完全不同——**這 7 個成員今天沒有任何下游消費者，接了也不會讓任何現有 tag 從 null 變成
有值，需要先新增一個 tag 家族才有意義。**

這 7 個成員的性質分兩類：

**(a) 位置/幾何 setpoint（TrayArm/Shuttle/InArm/OutArm）**：`Prod.XInArm_Tray_Pick`/
`ZOutArm_Shuttle1_Pick`/`iXTrayLoad` 這類欄位是**內部運動控制用的 XYZ 座標表**，
本質上是「教導值 + offset 運算後」的 motor 目標位置，不是一般操作員會盯著看的即時狀態值
（更接近 `system\lastdata.dat`/`tech.dat` 的衍生產物）。若日後瀏覽器要做教導編輯 UI
（write path 設計文件 `docs/WEBBRIDGE_WRITEPATH_DESIGN.md` 已經在討論的方向），
這些欄位才會變成「顯示目前算出來的教導點」的直接消費者；在那之前，單獨接上是
「解鎖了一個沒人看的數字」，價值低。

**(b) 製程參數/診斷值（Index/AutoClean/AOI）**：
- `Index` 的 `Prod.TestZ1/2_Safe/Pick/Place/Test` 是 Index Arm 的安全高度與下壓高度，
  跟既有 `ht9045-contact-force`/`ht9045-index-flow` 這兩個 skill 描述的診斷場景相關，
  若配合 §3 提到的 `DoSetupSystemToProd()` 一起接上，對 contact force 相關案件排查
  有實際價值。
- `AutoClean` 的 `Prod.iAutoClean_ContactMode/ContactTime/ContactCount/DropHigh/
  iAutoCleanZ_Contact/Drop/Shift` 是 Auto Clean 製程參數，跟既有
  `ht9045-autoclean-flow` skill 的場景（`IntervalContact`/`Smart Auto Clean`/低良率清潔）
  直接相關，是這 7 個成員裡**跟現有維運知識庫關聯最強**的一組。
- `AOI` 的欄位高度客製化（Vitrox/Fix-AI-CCD/Scanner-AOI 選配），一般機台用不到，
  價值最低。

**tag 化價值排序（由高到低，僅基於本次 recon 觀察，非正式優先序決策）**：
1. `AutoClean`——製程參數，跟既有 skill 場景關聯最強，且零 I/O、零額外依賴。
2. `Index`——安全/下壓高度，但要有意義需要先接 `DoSetupSystemToProd()`（範圍外）。
3. `TrayArm`/`Shuttle`/`InArm`/`OutArm`——內部座標表，量大但目前沒有消費場景，
   且 InArm/OutArm 受 `OutArmSuck.iMaxRow==0` 限制只有單站點資料。
4. `AOI`——選配功能，一般機台用不到，價值最低。

## 5. 陷阱段

**(1) InitialOK gate 讓 orchestrator 本身變成假動作**：呼叫 `SetTechDataToProd()`
（不帶尾碼）在 wb_serve 環境下等於只呼叫了 `SetTechDataToProd_Tray()`——這點
`docs/RECON_binstar_datasource.md` §4 已經對 `_Yield` 講過一次，本次確認對全部
7 個成員同樣成立，且是**同一個 gate**（不是巧合的兩個獨立发現）。

**(2) `ckernel_shims.cpp` 的 no-op `#define` 只影響不帶尾碼的名字**：bin recon 已確認
`#define SetTechDataToProd W7L2_SetTechDataToProd`（`ckernel_shims.cpp:114`）只 shadow
不帶尾碼的 orchestrator 名字，本次核對 7 個電池成員的名字（`SetTechDataToProd_TrayArm`
等）都**沒有**被同一份 shim 檔 `#define` 掉——這件事本次重新對 `ckernel_shims.cpp`
全文 grep `#define SetTechDataToProd` 只有一筆命中（不帶尾碼那個），與 bin recon
的結論一致。

**(3) `TMyKitSuck` 兩個版本的 ODR 陷阱，這次是「資料不完整」而不是「讀錯偏移」的變種**：
`KNOWLEDGE.md` 記錄的是「選錯標頭會讀到錯的偏移」，本次發現的是同一個根因的**另一種
症狀**——沒有選錯標頭（`cinitial.cpp` 用的是對的那個 `aHotPlateSubstrate.h:365`），
但因為對的那個版本的 `SetItemAmount`/`SetMotorCount` 被整個 `#if 0`（GATE n4-1），
`iMaxRow`/`iMaxCol` 從未被設過非零值，導致 InArm/OutArm/AOI 的多站點迴圈全部
「執行零次」而非「讀錯資料」。**兩種症狀都源自同一個 ODR 分家，但表現完全不同：
一個是靜默錯誤（讀錯偏移），一個是靜默不完整（迴圈不跑）——判斷某個成員「有沒有問題」
不能只看它自己的翻譯是否忠實，還要往上查它讀的全域是不是被另一個 gate 保持在零值。**

**(4) `SetTechDataToProd_AutoClean` 的重複定義是已知且已解決的，不要「順手」清理**：
§2-f 已詳述。**如果之後有人要接手清理 `cinitial.cpp:11816` 那個 `#if 0`，必須同時處理
`AutoClean/AutoClean.cpp:302-355`，否則會是連結期 `multiple definition`（不是靜默錯誤，
但仍記錄在此避免有人誤以為兩份是「一份忘記刪的舊稿」而隨手動了其中一份）。**

**(5) `ReadIniData` 與 `CheckAndReadIniData` 是兩個不同語意的 helper 家族，命名容易混淆**：
本次核對 `common.cpp:397-508`（`CheckAndReadIniData`，缺 key 就回寫）與 `common.cpp:510-564`
（`ReadIniData`，純讀，缺 key 也只回傳預設值，不碰檔案）逐行對比，確認是**兩組獨立的
四型別重載函式，不是同一組的兩個入口**。`docs/RECON_binstar_datasource.md` 已經在講
`CheckAndReadIniData` 的懶 seed 回寫；本次額外確認 `ReadIniData`（不帶 `CheckAnd` 前綴）
是真正的純讀 helper。**日後看到程式碼呼叫「XxxReadIniData」這類名字，第一件事是看
清楚是不是有 `CheckAnd` 前綴，兩者風險等級完全不同，不能用名字長得像就假設行為一樣。**

**(6) absence-claim 全部驗證時間戳**：本文件所有「V906 沒有 XXX」/「golden 沒有 XXX」
的陳述都在 20260820 下午以下列指令量測，並在收尾前用 Read 工具重新開檔核對過行號字面
（而非只信 Grep 摘要）：

```
Grep "SetTechDataToProd_(Tray|TrayArm|Shuttle|InArm|Index|OutArm|AutoClean|AOI|SortArm|SortShuttle)\b" HT9011UC_Cpp_V3.33.906.0\*.cpp,*.h
Grep "ReadIniData|WriteIniData|TIniFile|LoadFromFile|SaveToFile|GetRecipeFileName|DataPath|asGeneralPath|WriteData\(|fopen|TFileStream|MyForceDirectories|CheckAndReadIniData|ReadTeachData|WriteTeachData|fAutoTeach|ReadData\(\)|::ReadFile\(|CreateDir" HT9011UC_Code_V3.33.906.0_20260618\cinitial.cpp
    -> 251 行輸出，逐一核對落在哪個函式範圍內（僅 Index :9395-9399 與 Shuttle :11225 落在本次 7 個成員範圍內）
Grep "AssignFile|Rewrite\(|WriteLn|SaveToFile|->Append\(|DeleteFile|CreateFile|_open\(|fprintf|remove\(|ForceDirectories|WriteInteger|WriteBool|WriteString|WriteFloat|WriteDateTime" HT9011UC_Code_V3.33.906.0_20260618\cinitial.cpp
    -> 5 行輸出，全部落在 7709-7784（SetUnloaderInfoFile/SaveUnloaderInfo，範圍外）
Grep "bool __fastcall ReadIniData|int __fastcall ReadIniData|AnsiString __fastcall ReadIniData|double __fastcall ReadIniData" HT9011UC_Code_V3.33.906.0_20260618\common.cpp
Read  HT9011UC_Code_V3.33.906.0_20260618\common.cpp:510-564 / :323-380        -> ReadIniData/OpenIniFile 逐行核對純讀
Read  HT9011UC_Code_V3.33.906.0_20260618\AutoTeach\AutoTeach.h:1-240 / AutoTeach.cpp:217-240,1207-1216 -> fAutoTeach 純記憶體確認
Read  HT9011UC_Code_V3.33.906.0_20260618\ShuttleMove.cpp:2252-2264            -> TfShuttleMove::ReadData 磁碟路徑確認
Grep "In_Shuttle_Auto_Latch\s*=\s*[^=]" HT9011UC_Code_V3.33.906.0_20260618\*.cpp -> 預設值 0 確認（database.cpp:1536）
Grep "In_Shuttle_Auto_Latch|eInSHAutoLtc" HT9011UC_Cpp_V3.33.906.0\*.cpp        -> 確認 cinitial.cpp:6815-6816 的 #if 0 死碼狀態
Grep "InitialOK\s*=\s*true|bool\s+InitialOK" HT9011UC_Cpp_V3.33.906.0\*.cpp    -> 確認只有 tests/*.cpp 設為 true
Grep "iRunACSmart\s*=" HT9011UC_Code_V3.33.906.0_20260618\*.cpp                -> 確認預設 0、非 ini 載入值
Grep "USE_OUT_SORT_ARM\s*=" HT9011UC_Code_V3.33.906.0_20260618\*.cpp           -> 確認預設 eartUninstall
Grep "Prod\.(XInArm|YInArm|ZInArm|XOutArm|YOutArm|ZOutArm|TestZ|TestY|iAutoClean|iBS1|iBS2|iShuttle|iOutSH|iInSH|Fix_Pick)" HT9011UC_Cpp_V3.33.906.0\WebBridge*, WebBridgeTags.h
    -> 零命中，確認無現有 tag 消費者
Read  tools/wb_serve.cpp:190-330                                              -> 確認今天只直接呼叫 _Yield，未呼叫其餘成員；DataPath scratch-redirect 窗口 :225-554
```
