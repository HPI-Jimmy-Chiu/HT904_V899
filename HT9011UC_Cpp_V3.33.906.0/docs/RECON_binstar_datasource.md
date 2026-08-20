# RECON：bin.* tag 解鎖的資料源偵察

日期：20260820
範圍：唯讀 recon。未 build / 未跑 ctest / 未 commit / 未起任何背景行程。

## 0. 前置更正：golden 選錯樹（先記録，避免後續複用時踩同一個坑）

任務指定的 golden 行號是 `cinitial.cpp:11438-11634`。專案裡有兩棵路徑都含「906」：

- `HT9011UC_Code_V3.33.906.0_20260618`（另一棵 BCB6 樹，唯讀，**這才是本任務的 golden**）
- `HT9011UC_Cpp_V3.33.906.0`（本次要接線的 C++ 移植樹）

我一開始誤把 `HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422`（V899 量產樹）當 golden 去對行號，`SetTechDataToProd_Yield` 在那棵樹其實在 `cinitial.cpp:10876-11061`（197 行變 186 行，且少了 `AOICatData`/`bMagazineLink`/`iAOICategData`/`bFailCountEnable` 的 `iTestBinCount<=16` 分支等幾處，代表 899 是較舊版本）。改查 `HT9011UC_Code_V3.33.906.0_20260618\cinitial.cpp:11438-11634` 後，行號、行數（197）與內容逐行對上，確認這才是 V906 移植樹實際翻譯時用的 golden。

驗證指令：
```
Grep "SetTechDataToProd_Yield" HT9011UC_Code_V3.33.906.0_20260618\cinitial.cpp   -> 11438 定義, 11636 呼叫點
```
以下全文皆以 `HT9011UC_Code_V3.33.906.0_20260618` 為 golden。

---

## 1. `SetTechDataToProd_Yield` 讀哪些全域（golden :11438-11634，逐行看過）

寫入端只有一個：`Prod.*`（`iT6CatData/iT6PosCate/AOICatData/DBContact/iIsFailT6/iIsPassT6/bART6Tray/bLinkTo6Tray/bCateRTo6Tray/iTo6StackDefFailCate/bTo6AutoRetest/bTo6AutoLink/bTo6CateR/bIsPassBin/iIsFailBin/iIfErrorT6/iTo6IfError/iTo6CatData/iTo6CatePos/bScanner/bConsFail/bFailure/dFailureLimit/iPersentIgnore/bFailCountEnable/iFailCountIgnore/iFailCountLimit/bSpecialBinByArm/iSpecialBinCountByArm/bSpecialBinBySocket/iSpecialBinCountBySocket/iAutoCleanByBin/iAutoCleanBySite/bSpecBinBySiteCompareEnable/iSpecBinBySiteCompareIgnore/dSpecBinBySiteComparePercent/bSpecBinByArmPerSiteCompareEnable/iSpecBinByArmPerSiteCompareIgnore/dSpecBinByArmPerSiteComparePercent/bSCKART_EnableSPBinAlarm/iSCKART_SPBinSelect/dSCKART_SPBinAlarmYield`），以及一個非 Prod 的寫入副作用：`TestIF.iAutoClean_Function=false`（golden :11631，本身帶了 golden 自己的修正註解「Fix == to = (was comparison, not assignment)」——這棵 906-code golden 已經比 V899 新，這行在 V899 那棵還是原本的 `==` 比較式殘留）。

讀取端（完整清單，按用途分組）：

| 分組 | 全域 | golden 行 |
|---|---|---|
| bin 數量/模式 | `iTestBinCount`、`iTestRunMode`、`iRunStartMode` | :11443 等多處 |
| 3↔6 位置映射（純列舉常數表） | `iTo6PosUnload[]`、`iTo6Unload[]`、`iTo3Unload[]` | :11445/11453/11483 |
| 座數常數 | `eTrayCount`、`MAX_SOCKET_ROW`、`MAX_SOCKET_COL` | :11451/11497/11499 |
| 產線分派旗標 | `bUseTwoArm32Site` | :11519 |
| Bin 設定（recipe 主體，8 個 run-mode 槽全讀） | `BinSelect[iTestRunMode]` 與 `BinSelect[0..7]` 的 `iCatDataT3Pos/iAOICategData/iDBContact/iStackDefFailCate/bAutoRetest/bAutoLink/bCateR/IfErrorT3/bLowYield/bArmYield/bSiteYield/bScanner/bConsFail/bFailure/dFailureLimit/iPersentIgnore/bFailCountEnable/iFailCountIgnore/iFailCountLimit/bSpecialBinByArm/iSpecialBinCountByArm/bSpecialBinBySocket/iSpecialBinCountBySocket/iAutoCleanByBin/iAutoCleanBySite/bSpecBinBySiteCompareEnable/iSpecBinBySiteCompareIgnore/dSpecBinBySiteComparePercent/bSpecBinByArmPerSiteCompareEnable/iSpecBinByArmPerSiteCompareIgnore/dSpecBinByArmPerSiteComparePercent` | :11445-11613 全函式 |
| 客製旗標 | `CosFunction.bDisableRTBinSet`、`CosFunction.bByBinAlarmFromYieldForm` | :11548/11554/11560/11571/11592 |
| Recipe（另一個結構，非 BinSelect） | `TestIF_File.bByBinFailureEnable/dByBinFailurePercent/iByBinFailureIgnore/bFailCountEnable/iFailCountIgnore/iFailCountLimit/bSpecBinBySiteCompareEnable/iSpecBinBySiteCompareIgnore/dSpecBinBySiteComparePercent/bSpecBinByArmPerSiteCompareEnable/iSpecBinByArmPerSiteCompareIgnore/dSpecBinByArmPerSiteComparePercent/bSCKART_EnableSPBinAlarm/iSCKART_SPBinSelect/dSCKART_SPBinAlarmYield` | :11555-11613 |
| 機台設定 | `IniConfig.bEnableAutoCleanFunction` | :11616 |
| 測試介面狀態 | `TestIF.iAutoClean_Tray`、`TestIF.iAutoClean_Function` | :11617-11618/11631 |
| 良率統計物件（呼叫其方法，非讀其欄位） | `ArmData[0..2]`、`ArmDataLot[0..2]`、`ArmHistory[0..2]`、`ArmData_AutoClean[0..2]`，透過 `TArm::SetArmBinCodeStatus(...)` | :11504-11533 |
| 常數 | `iFixRight`、`iFixRightHalf` | :11622/11626（用於 Fix3 判斷，本次未深追其設值處，屬電池外常數） |

V906 側逐一比對：`cinitial.cpp:16986` 的翻譯**逐行、逐符號**對上表，並附有 20260820 當天的驗證註解（`cinitial.cpp:16965-16984`）明確列出「每個符號都已確認存在」的清單，與我自己重新讀 golden 對出來的清單一致。這條翻譯本身沒有問題。

---

## 2 + 3. 各來源的載入鏈與讀寫性判定

### 2a. `iTo6PosUnload[] / iTo6Unload[] / iTo3PosUnload[] / iTo3Unload[]` —— 純讀，零風險

- **golden**：`main.cpp` 的 `TfMain::TfMain(TComponent* Owner)` 建構子本體裡，一段純列舉對列舉賦值（例如 `iTo6PosUnload[e3PosAuto1]=ePosAuto1;`），**不觸碰任何檔案**，是編譯期常數表，只是用執行期程式碼寫出來而已。
- **V906**：`cmydef.cpp:4280-4400`（`namespace { struct W906_TrayIndexMapInit { W906_TrayIndexMapInit() {...} }; } W906_TrayIndexMapInit ...;`）——一個 **TU 內 static 初始化物件**，保證在 `main()` 執行前就把四張表填好，不需要任何函式呼叫。這是 20260817 當天另一波（W906-FW-Fix1）已經修好的：修之前這四張表在 V906 是全零（golden 的填表建構子從未被呼叫到），修之後靠 static-init 保證一定跑。
- **讀寫性**：**純讀**（其實連讀都稱不上，是編譯期常數）。**wb_serve 不需要做任何事**，這四張表現在已經是對的。
- 驗證：`Grep "iTo6PosUnload\[e3PosAuto1\]" HT9011UC_Cpp_V3.33.906.0\cmydef.cpp` → 命中於 static 初始化區塊內。

### 2b. `ArmData[]/ArmDataLot[]/ArmHistory[]/ArmData_AutoClean[]` —— 純記憶體建構，零風險

- **golden**：`main.cpp:2143-2146`（`ArmData[i]=new TArm(...)` 等 4 行，在 TfMain 建構子內，逐一 `new`）。
- **V906**：`cSocket.cpp:211-226`，同樣是 **namespace static 初始化物件**（`ArmDataBootstrap`），對 3 個 arm 索引各 `new TArm(...)` 四個陣列共 12 個物件，guard 過 `==NULL` 才 new（冪等）。註解裡明確記錄了為什麼這樣做是 static-init-order 安全的：`TArm` 建構子只配置自己的 `TStringList`/`TMySocket`，不讀任何其他全域，所以沒有跨 TU 初始化順序風竅。
- **讀寫性**：**純記憶體操作，不碰磁碟**。`SetTechDataToProd_Yield()` 呼叫 `ArmData[0]->SetArmBinCodeStatus(...)` 時，這些指標已保證非空——**不會因為呼叫這個函式而在 wb_serve 裡 segfault**。
- 驗證：`Grep "ArmDataBootstrap" HT9011UC_Cpp_V3.33.906.0\cSocket.cpp` → :212-226。

### 2c. `CosFunction.*` —— 純程式碼賦值，零風險

- **golden/V906 皆同**：`CosFunction.cpp` 開頭是一長串 `CosFunction.bXxx=true/false;` 的字面賦值（例如 `CosFunction.cpp:25-43` 一路往下），本質是「依 `CUSTOMER_CODE` 分支的旗標表」，**完全不涉及任何檔案 I/O**。
- **讀寫性**：**純讀（其實是純程式碼常數）**，wb_serve 呼叫鏈裡不管有沒有跑到這段初始化，都不會碰磁碟。
- 驗證：`Grep "CosFunction\.b\w+\s*=true" HT9011UC_Code_V3.33.906.0_20260618\CosFunction.cpp` 命中大量字面賦值，無 `ReadIniData` 字樣。

### 2d. `IniConfig.bEnableAutoCleanFunction` —— 經 `LoadMachineConfig()`，已在 wb_serve 路徑上，有寫（但已被 --dry 擋住）

- 這條走的就是專案已知的 `common.cpp:89 asGeneralPath -> system\Gerneral.ini` 路徑，`LoadMachineConfig()` 已經是 wb_serve 目前唯一呼叫的載入函式（`tools/wb_serve.cpp:198`）。
- **讀寫性**：**有寫**（`CheckAndReadIniDataGeneral` 家族的懶 seed 行為，已是全專案已知並記錄在案的風險），但 **`--dry` 已經覆蓋這條路徑**（wb_serve.cpp 註解自陳：「LoadMachineConfig() SEEDS missing keys, i.e. it WRITES to asGeneralPath... --dry copies [it] first」）。不是本次新發現，列出來只是確認它在電池的位置。

### 2e. `iTestBinCount` / `TestIF_File.*`（bByBinFailureEnable 等） —— 經 `TFTestIF::ReadTestIFFile()`，**V906 未翻譯**，今天等於安全（因為根本連不到）

- **golden**：`cTesterIF.cpp:562` `void __fastcall TFTestIF::ReadTestIFFile()`。函式一開頭：
  ```
  S=GetLastOpenFN();
  szDir=DataPath+S;
  szDir+="\\Tester.Data";
  ```
  之後上百行 `TestIF_File.xxx=ReadIniData(szDir, Group, Name, Default);`，`iTestBinCount` 的算法在 :908-925（依 `TestIF_File.iTestType/iRs232Mode/iGpibMode` 決定 16/17/33/255/recipe自訂值）。
- **讀寫性（若翻譯進來會怎樣）**：`ReadIniData()` 本身是**純讀**（見下方 3. 的 helper 分析），但 `ReadTestIFFile()` 函式體裡有**至少 3 處明確、寫死在函式裡的 `WriteIniData(...)` 呼叫**（golden :574-578 修正非法 `Tester Type`、:823-824 補 `DIO/TypeName`、:883-892 補 `RS-232C/BaudRate`），都是「讀到不合法/缺漏值就寫回 `Tester.Data`」的模式。**這條函式整體不是純讀**。
- **V906 現況**：`ReadTestIFFile` 這個名字在 V906 樹裡**完全沒有函式定義**，只在別的 gate 註解裡被提到是「還沒翻的依賴」（`Command.cpp:170,3621`）。`iTestBinCount` 停在全域預設值 `=16`（`cmydef.cpp:3601`，跟 golden 沒 recipe 時的預設一致）。
- **結論**：今天呼叫 `SetTechDataToProd_Yield()` 不會觸發這條鏈——因為它根本不存在，`iTestBinCount` 就是編譯期常數 16。**不建議為了「讓 iTestBinCount 更真實」去翻譯 `ReadTestIFFile()`**，因為翻進來就會把 `Tester.Data` 的寫入路徑一起帶進 wb_serve（見下）。

### 2f. `iTestRunMode` —— 經 `SetTestRunMode()` <- `LastSet.iRunStartMode` <- `ReadLastDataFile()`，**V906 未翻譯**，今天等於安全，且**千萬不要翻**

- **golden**：`main.cpp:1117` `void SetTestRunMode()`，整段依 `LastSet.iRunStartMode` 的各種列舉值決定 `iTestRunMode=FT/RT/FT_ART/...`。`LastSet` 是誰填的？`cprod.cpp:1603` `bool ReadLastDataFile()`。
- **這條鏈是本 repo 已知最貴的坑**：`ReadLastDataFile()` 內部**會條件式呼叫 `WriteLastDataFile()`**（golden `cprod.cpp` 內，grep 命中 :1666、:1687，皆在 `ReadLastDataFile` 函式體 :1603 之後、下一個函式之前），路徑是**寫死的字面值**：
  ```
  D:\\HT9045\\system\\lastdata.dat
  d:\\HT9045\\system\\lastdata_backup.dat
  D:\\HT9045\\system\\lastdata_backup2.dat
  ```
  V906 側的翻譯（`HT9011UC_Cpp_V3.33.906.0\cprod.cpp:1701,1729`）**逐字元**保留了這些硬編路徑（與專案既有紀錄的行號完全對上），且同一函式一樣有 `WriteLastDataFile()` 的內部呼叫（V906 :1666/:1687 對應區段）。**`--dry` 完全不重導這條路徑**——這正是 CLAUDE.md 開頭就點名的坑。
- **V906 現況**：`SetTestRunMode` 這個名字在 V906 樹**完全沒有函式定義**（只在別的 gate 註解裡被提到），`iTestRunMode` 停在全域預設值 `=0`（`cmydef.cpp:3339`，對應 `FT`）。
- **結論**：`iTestRunMode` 今天是安全的，**正是因為它連不到 `LastSet`/`lastdata.dat`**。這是本次 recon 最重要的「不建議接」清單第一名——**絕對不要為了讓 `iTestRunMode` 更真實去翻譯 `SetTestRunMode()`**，那條路直通已知會寫壞 `lastdata.dat` 的函式，而且 `--dry` 擋不住。

### 2g. `BinSelect[]` —— 經 `TfBinSel::ReadFile()`/`ReadFunctionData(tag)`，**V906 已翻譯（ACTIVE）**，這是唯一「真的能接、但有寫」的一條

這是本次 recon 的核心發現，分三層說：

**(i) 檔案落在哪裡**

- `golden cBinSel.cpp` 用 `GetRecipeFileName("BinasgnOff.Data")`、`GetRecipeFileName("Binasgn.Data")`、`Binasgn_ART.Data`、`Binasgn_MRT.Data` 等一組**依 tag 命名的 recipe 檔**（都是 ini 格式，副檔名叫 `.Data`）。
- `GetRecipeFileName(FileName)`（`common.cpp:2044`，V906 :2410）= `Common_PathCombin(DataPath + GetLastOpenFN(), FileName)`。
- `DataPath = "D:\\HT9045\\IniData\\Data\\"`（**寫死的絕對路徑**，V906 `common.cpp:104` 與 golden `common.cpp:33` 逐字元相同）——這正是 `AGENTS.md` 點名「量產機共用執行期參數，預設只讀」的 `IniData\` 目錄。
- `GetLastOpenFN()` 讀 `LastDataPath="D:\\HT9045\\SetUp.inf"`（`TStringList::LoadFromFile`，**純讀**，V906 `common.cpp:1317-1346` 已確認無任何寫分支），取檔案第一行當「目前開啟的 recipe 名稱」。
- 兩者合起來：最終路徑是 `D:\HT9045\IniData\Data\<SetUp.inf 第一行內容>\Binasgn*.Data` 這一組**寫死、共用、production-adjacent** 的路徑，**跟 `asGeneralPath` 是完全不同、`wb_serve --dry` 目前完全沒有涵蓋的第三條硬編路徑家族**（前兩條是 `asGeneralPath`=Gerneral.ini 已被 --dry 涵蓋、`lastdata.dat` 已知未涵蓋）。

**(ii) 讀寫性**

- 實際讀值走的是**共用 helper** `CheckAndReadIniData(FileName, Group, Name, Value)`（`common.cpp:432-447` 等 5 個型別重載），行為（以 int 版為例，golden 與 V906 逐字元相同）：
  ```cpp
  if(!INIFile->ValueExists(Group, Name))
      INIFile->WriteInteger(Group, Name, Value);   // 缺 key 就寫回
  else
      Value=INIFile->ReadInteger(Group, Name, Value);
  ```
  這跟已知坑「`CheckAndReadIniDataGeneral` 對 `Gerneral.ini` 的懶 seed」是**同一個模式、同一個作者思路**，只是這次的目標檔案是 recipe 的 `Binasgn*.Data`。
- 好消息：這裡用的是 `TIniFile`（`common.cpp:149`），不是 `TMemIniFile`。`TIniFile` 每次寫入都直接呼叫 Win32 `WritePrivateProfileString`，是**逐 key 定點寫入**，不是整檔重寫——**不會**像已知的 `Gerneral.ini 20260817 被整檔重寫` 事故那樣把整份檔案的排版/註解炸掉。但**仍然是磁碟寫入**：只要目標 recipe 檔缺任何一個被查詢的 key（或該 recipe 目錄根本不存在——`TIniFile` 開一個不存在的檔案不會報錯，之後任何寫入都會就地建立新檔），就會在 `D:\HT9045\IniData\Data\` 下**新增或修改檔案**。
- **V906 現況**：`TfBinSel::ReadFile()`（V906 `cBinSel.cpp:1402`）與 `TfBinSel::ReadFunctionData(int tag, AnsiString szDir)`（V906 `cBinSel.cpp:1901`，golden 對應 :4807-5667）**都已經翻譯完成且標記 ACTIVE**（這是先前一個獨立波次 FW-BinSel-WB 做的），內部同樣呼叫 `CheckAndReadIniData(szDir, Group, Name, Value)`，`szDir` 同樣來自 `GetRecipeFileName(...)`。也就是說：**BinSelect 的載入邏輯本身翻譯完整且忠實，缺的只是「wb_serve 沒有呼叫它」**——這與任務描述的「缺的是上游」精準對應。
- 驗證指令：
  ```
  Grep "GetRecipeFileName\(\"Binasgn" HT9011UC_Cpp_V3.33.906.0\cBinSel.cpp   -> :814,819,824,829,834,839,844
  Grep "CheckAndReadIniData" common.cpp（V906 與 golden 兩棵）逐一比對 int/bool/AnsiString 四個重載，寫回分支存在
  ```

**(iii) 現況：不接的話 bin.* 顯示什麼**

- `BinSelect[8]` 是全域陣列，未呼叫載入前是**零初始化**（C++ 靜態儲存期物件的預設行為）。`BinSelect[iTestRunMode].iCatDataT3Pos[i]==0` 對每個 bin 都成立，`iTo6PosUnload[0]` 對應的是 golden 列舉表裡索引 0 的項（`e3PosNoUse -> ePosNoUse` 這一組「未使用」哨兵，命名上看是保留位，實際整數值我這次沒有再往下查列舉定義去釘死，如需要要另開一輪查 `cmydef.h`/其對應列舉檔）。也就是說**目前 bin.* 六個 tag 顯示的是「全部未使用」這個確定但無意義的佔位值**，不是亂數、不是崩潰，只是不真實。

---

## 4. 提案：wb_serve 要接 bin.* 的最小安全載入序

**建議動作（安全，可以做）：**

1. 不需要對 `iTo6PosUnload/iTo6Unload/iTo3*Unload` 與 `ArmData` 家族做任何事——兩組都已經是 static-init 保證好的，wb_serve 現在跑起來就已經是對的。
2. 在 `LoadMachineConfig()` 成功之後，**直接呼叫 `SetTechDataToProd_Yield()`**（`cinitial.h:13259` 已宣告，可從 `wb_serve.cpp` 直接呼叫）。
   - **不要**呼叫外層的 `SetTechDataToProd()`：golden 裡它一進來先呼叫 `SetTechDataToProd_Tray()`，接著 `if(InitialOK==false) return;`（golden/V906 :15253-15256），wb_serve 目前的唯讀 bring-up 大概率不會把 `InitialOK` 設成 true（那牽涉到 motor.db 之類更大一包東西，超出本次 bin.* 的範圍），呼叫外層會直接被這個 gate 擋掉、什麼都不會發生。另外 `SetTechDataToProd`（不帶 `_Yield` 尾碼）這個名字在 `ckernel_shims.cpp:114` 被 `#define` 成 `W7L2_SetTechDataToProd`（一個空的 no-op）——只要 wb_serve 的編譯單元有機會吃到那個標頭，呼叫這個名字就會靜默呼叫到空函式而非真身。`SetTechDataToProd_Yield` 這個名字本身**沒有**被同一份 shim 檔 `#define` 過（已用 Grep 確認），所以直接點名它是安全的。
   - 這一步在目前的預設值下（`iTestRunMode=0`、`iTestBinCount=16`、`BinSelect[]` 全零）**保證純記憶體運算**，不會有任何新的磁碟 I/O，只是會把 `Prod.iT6CatData[]` 等欄位從「未初始化前的隨機/預設值」變成「已知的全零推導值」——比現況更可預期，但不會讓 bin.* 顯示 recipe 真值。
3. **明確不要**翻譯或呼叫 `SetTestRunMode()`（→ `ReadLastDataFile()` → 硬編 `lastdata.dat` 路徑、`--dry` 不擋、且函式內部會反向呼叫 `WriteLastDataFile()`），也不要翻譯 `TFTestIF::ReadTestIFFile()`（→ 硬編 `Tester.Data`、函式體內至少 3 處明確 `WriteIniData` 補值）。維持 `iTestRunMode=0`、`iTestBinCount=16` 這兩個現有的安全預設值。

**卡住的一步：`BinSelect[]` 的真實載入**

卡在 `GetRecipeFileName()`/`DataPath`（`D:\HT9045\IniData\Data\`）沒有任何等效於 `asGeneralPath` 的 `--dry` 重導，而 `TfBinSel::ReadFile()`/`ReadFunctionData()` 內部全面走 `CheckAndReadIniData`，對任何缺漏的 key 都會直接寫回那個檔案（甚至在目錄不存在時可能就地新建）。**不建議把這條路徑接進 wb_serve**，除非先做以下兩者之一：

- (a) 仿照現有 `asGeneralPath` 的 `--dry` 做法，在 wb_serve 裡新增「把 `D:\HT9045\IniData\Data\<GetLastOpenFN()>\` 整個資料夾複製一份到 scratch，再把 `DataPath` 重導過去」的等效保護，之後才呼叫 `TfBinSel::ReadFile()`；或
- (b) 放棄呼叫真正的 loader，改成 wb_serve 自己在記憶體裡直接對 `BinSelect[iTestRunMode]` 的少數欄位（至少 `iCatDataT3Pos[]`，若要連良率相關 tag 一起有意義還要 `iStackDefFailCate[]`/`bAutoRetest[]`/`bAutoLink[]`/`bCateR[]`）塞入假資料，完全不碰磁碟，再呼叫 `SetTechDataToProd_Yield()`。這樣 bin.* 會顯示「有意義但非 recipe 真值」的資料，純粹是為了讓下游 `ShowBinSel()`/`MyBinSel[].Caption` 顯示鏈有東西可以人工驗證。

兩個選項都需要使用者決定，我不代為選擇。

---

## 5. `SetTechDataToProd` 電池其他成員 —— 一句話評估（僅評估，未深入）

- **`SetTechDataToProd_Tray()`**：電池裡第一個被呼叫者，其後緊接 `if(InitialOK==false) return;`（golden/V906 皆同），代表整條電池（含 `_Yield`）在**走完整編排器**時實際上被 `InitialOK` 閘住；本次建議繞過編排器直接叫 `_Yield`，所以這個閘門對本次提案沒有影響，但若之後有人想把整個電池一起接上 wb_serve，要先處理 `InitialOK` 這個更大的前提。
- **`SetTechDataToProd_TrayArm/Shuttle/InArm/Index/OutArm/AutoClean/AOI()`**：從我讀過的鄰近程式碼（例如 :15150-15218 一段屬於 InArm 電池成員的內容）看，這些函式主要讀 `Tech.*`（教導資料結構）欄位，屬於跟 `LastSet`/`lastdata.dat` 同等級、甚至更敏感的二進位教導檔（`tech.dat`）家族。**本次未追蹤這些函式各自對應的 `Tech` 載入函式（`ReadTeachData` 或同義函式）是否有「讀完寫回」行為**——鑑於本 repo 已有「V899 裝上舊機台會靜默毀掉教導值」的真實前科（`ht9045-v899-teach-ini-migration-destroys-old-teach-data`），**在把這幾個成員接進 wb_serve 之前，必須比照本次對 `BinSelect` 的作法，逐一查證各自的 `Tech` 載入鏈，不能預設安全**。這是明確的未做項，留給下一輪 recon 或翻譯波次。

---

## 附：本次用過的驗證指令（供覆核）

```
Grep "SetTechDataToProd_Yield" -> 定位 golden 與 V906 兩側函式位置與呼叫點
python3 cp950 decode -> 逐行讀 golden cinitial.cpp:11430-11640 全函式
Grep "iTestBinCount\s*=" cTesterIF.cpp -> 定位 iTestBinCount 算法 :908-925
Grep "iTo6PosUnload\[e3" / "iTo3Unload\[e" main.cpp / cmydef.cpp -> 定位列舉表填表處（golden main.cpp TfMain ctor；V906 cmydef.cpp static-init）
Grep "BinSelect\s*\[\s*8\s*\]" -> 定位 SYSTEM_BIN_SELECT BinSelect[8] 宣告於 cprod.cpp:62/cprod.h:2681
Grep "ReadFunctionData\(" / "GetRecipeFileName\(\"Binasgn" cBinSel.cpp（golden 與 V906 兩側）-> 確認翻譯狀態與檔案落點
Read systools.cpp:1-100、common.cpp:390-570（golden）與 common.cpp 對應區塊（V906）-> 逐字元比對 CheckAndReadIniData/ReadIniData 的寫回行為
Grep "ReadLastDataFile|WriteLastDataFile" cprod.cpp（golden 與 V906 兩側）-> 確認 lastdata.dat 讀寫互相呼叫
Grep "ArmData\[i\]\s*=\s*new" / "ArmDataBootstrap" cSocket.cpp -> 確認 static-init 保證非空
Grep "ReadTestIFFile|SetTestRunMode" 全 V906 樹 -> 確認兩者皆未翻譯（只在 gate 註解中被提及）
Grep "#define SetTechDataToProd" ckernel_shims.cpp -> 確認只有不帶 _Yield 的名字被 shim 掉
```

所有 Grep 皆鎖定 `HT9011UC_Code_V3.33.906.0_20260618`（golden）或 `HT9011UC_Cpp_V3.33.906.0`（V906）兩棵樹之一，未對整個 repo 寬掃。
