# RECON — PCI/PCIE-1203（Advantech Common Motion）導入評估

> 20260818。使用者裁決：906 後續導入 PCI/PCIE-1203，**所有馬達、汽缸、Sensor 統一控制**。
> SDK＝`C:\Program Files (x86)\Advantech\Common Motion`（ADVMOT.dll **2.0.15.2**，x86/x64 同版）。
> 分支標記：`feat/v906-pcie1203-hal`（未 checkout，兩 session 共用 repo）。
> 本文件為 recon 代理報告（含實測連結驗證）的落地版；證據行號皆經核對。

## 1. 最重要的結論：HAL 已經存在，不是空白畫布

- **馬達側**：`Motor/myEthercatmotor.{h,cpp}` 的 `TMyEtherCatMotor : HTMotor`（PT-W3 已翻，
  標頭自述 "Advantech PCI1203 EtherCAT axis driver"）——HTMotor 全虛方法都有對映
  （InitMotor→Acm_AxOpen/SetSvOn、Home→Acm_AxMoveHome、MoveToPos→Acm_AxMoveAbs/Rel、
  Stop→Acm_AxStopDec/Emg、ReadPos→Acm_AxGetCmd/ActualPosition、狀態→Acm_AxGetState
  對 STA_AX_*）。
- **IO 側**：`IOBackend.h:137-144`/`IOBackend.cpp:120-161` 的 `TPci1203Backend : TIOBackend`
  （W4-IO 已翻，**用的是正確的 Ring/IP-aware Ex 版 API**）：
  WriteBit→`Acm_DaqDoSetBitEx`、WriteByte→`Acm_DaqDoSetByteEx`、
  ReadBit→`Acm_DaqDiGetBitEx`、ReadByte→`Acm_DaqDiGetByteEx`。
  汽缸/Sensor 鏈：`TMyCylinder/TMySensor/TMySwitch` → `TLaneIO` →（iISABase==ePCI1203）→
  `TPci1203Backend`——**鏈已打通**，缺的是開 `HAVE_PCI1203` 與設定接線。
- **類比（真空壓力）**：golden 走 bespoke SDO（`Acm_DevRead/WriteSDOData`，CoE
  0x8000+Port*0x10:0x13，`MyLaneIo.cpp:761-854`）而非標準 `Acm_DaqAi*`——接新類比模組
  時應走標準 `Acm_DaqAiGetVolt/CurrData`，勿照抄 SDO 位址。

## 2. 版本判定：vendored 標頭可原地升級（但非必要）

`EtherCAT/vendor/*.h` 五檔與 SDK 全 DIFFERS（AdvMotApi 65,869→74,348B 等），但逐符號比對
**純加法演進**：已用到的 40 個 `Acm_*`＋rollout 需要的 13 個（DaqDi/Do/Ai/Ao、LoadENI、
DevLoadConfig、ChannelProperty、AxOpenbyID、MasStart/StopRing）簽名 **100% 相同**；
38 個 PropID 值零改動。⚠ 僅比對 53/622 符號（Acm_Gm*/Rb* 兩家族未查）。

## 3. 典型呼叫序列（範例檔名可回查）

- **開卡→軸控**（VC\Home/JOG/CMove/PTP）：`Acm_GetAvailableDevs`→`Acm_DevOpen`（失敗
  重試10次）→`FT_DevAxesCount`→`Acm_AxOpen` 每軸→`Acm_AxSetSvOn`→
  `PAR_AxHomeVel*/Acc/Dec/Jerk`→`Acm_AxMoveHome(homeMode,dir)`→`Acm_AxGetState` 輪詢。
  HomeMode：0-16=Advantech 原生；**101-137=CiA402 method 1-37 加 100**（1203 常用這組）。
- **1203 設定檔**（VC\PCI1203 Path\PathDlg.cpp:346,505）：`Acm_DevLoadConfig(hDev, .cfg)`
  ——Advantech Utility 產的自家格式；**泛用 EtherCAT 分支才用 `Acm_LoadENI`**
  （EthcatDIDlg.cpp:216-227 依 devType==Adv_EtherCAT 判斷）。
- **遠端 IO**（VC\EthcatDI/DO/AI）：`FT_DaqDiMaxChan/DoMaxChan` 查通道數→
  `Acm_DaqDiGetByte` 輪詢／`Acm_DaqDoSetBit` 寫＋`Acm_DaqDoGetByte` 回讀確認。
- **硬體級 DI→停軸互鎖**（VC\DI STOP\DIStopDlg.cpp:888-951）：
  `CFG_AxIN1StopAssign/Enable/React/Logic`——卡內建、不經 CPU 輪詢；write-path 設計輪
  評估安全鏈時值得納入。

## 4. 實測發現（recon 代理在 scratchpad 做的最小連結驗證）

1. **⚠ 新陷阱：MinGW 必加 `-D_STDCALL_SUPPORTED`**。AdvMotApi.h:137-141 只在
   `_MSC_VER>=800 || _STDCALL_SUPPORTED` 時定義 `ADVCMNAPI=__stdcall`；MinGW 兩者皆無
   → `ADVCMNAPI` 裸露 → **每一個 Acm_* 宣告解析失敗**。至今沒踩到只因 `HAVE_PCI1203`
   從未真的開過。開閘時 CMake 要同時 `-DHAVE_PCI1203 -D_STDCALL_SUPPORTED`。
2. **32-bit MinGW 直連 x86 ADVMOT.lib 成功**（COFF import lib，`_Acm_DevOpen@8` stdcall
   decoration 對齊；不需 gendef/dlltool）。x64 lib 對 32-bit oracle 無用
   （binutils 不認 0x8664）；x64 驅動經 WOW64 服務 32-bit 程式（實測載入 SysWOW64 版 DLL）。
3. **⚠ 無卡（且無虛擬卡）時 `Acm_GetAvailableDevs`/`Acm_DevOpen` 阻塞 ≥15s 不返回**
   ——開發機 smoke/probe 一律要包 timeout，不可裸呼叫當健康檢查。
4. SDK 支援**虛擬卡**：`Adv_V_PCI1203`（AdvMotDev.h:82,92），由 Common Motion Utility
   註冊，可無卡開發（本次未操作——會改系統驅動註冊狀態）。

## 5. 既有已知風險（重申，非新發現）

- `myEthercatmotor.cpp:119-133`：golden 自己的 bug——`ethercat_set_output_bit/byte`
  誤用被遮蔽的靜態 `gDevhand`（恆 0）呼叫不帶 Ring/IP 的短版 API。**目前全樹零呼叫點
  （死碼）**；真正在用的 DO/DI 路徑是乾淨的 `TPci1203Backend`。啟用波若要解這三個方法，
  先回頭讀該檔 GATE REGISTER。
- `TPci1203Backend::WriteBit` 的 Bit 參數被吞掉（BCB6 原設計把 Bit 編進 Port，
  IOBackend.cpp:129 有 NOTE）——啟用波要逐行核對 mycylin/mysensor 的位址映射。
- `ADVMOTBCB.lib`（Borland OMF，2024-04 版，比主 lib 舊近兩年）＝Advantech 官方也照顧
  BCB6；**是否讓 V899 用它屬 ht9045-v899 的決策**，本線不代評。

## 6. 啟用波（未來）的最小步驟清單

1. CMake：`HAVE_PCI1203=1`＋`_STDCALL_SUPPORTED`＋link `Public/ADVMOT.lib`（x86）。
2. （可選）vendored 標頭原地升級到 2.0.15.2。
3. 設定層：`Mot_Table.csv`/`IO_Table.csv`/Gerneral.ini 的 1203 佈局（ring/slaveIP/axis 對映）
   ——照 `cinitial.cpp` 讀取格式，先讀懂再動。
4. 無卡開發：Common Motion Utility 註冊虛擬 1203；所有 probe 包 timeout。
5. 汽缸/sensor：逐行核對 TMyCylinder→TLaneIO→TPci1203Backend 位址映射（Bit-in-Port 怪癖）。
6. write-path 設計輪（等使用者）決定互鎖形狀後，才輪到 DO 真輸出的啟用。
