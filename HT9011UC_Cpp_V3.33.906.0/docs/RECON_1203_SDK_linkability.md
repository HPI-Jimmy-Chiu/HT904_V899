# RECON — Advantech PCIE-1203 SDK 的 MinGW 可連性稽核（1203HAL-3 前置情報）

> 20260820。唯讀稽核，`nm`/`objdump`/PowerShell `VersionInfo` 皆為唯讀檢視；未 build、未
> link、未跑 ctest、未起長駐行程。目標讀者：1203HAL-3（真的把 SDK import lib 掛進
> `ht9045_pci1203_probe`）。
>
> 本文件補的是**格式層證據**；業務脈絡（HAL 已存在的呼叫序列、無卡阻塞、虛擬卡）已在
> `docs/RECON_PCIE1203_CommonMotion.md`（20260818）記過，不重複，只在有新資訊時標注更新。
> CMake 現況：`CMakeLists.txt` 目前有未 commit 的變更（`ht9045_pci1203_probe` STATIC 探針
> target，`AI(W906-1203HAL-2) 20260820`），疑似主迴圈並行在做——本稽核未碰該檔，純讀。

## 結論先講

| 問題 | 結論 |
|---|---|
| x86 能不能用原生 import lib 直連 MinGW？ | **能。`Public/ADVMOT.lib` 是標準 MS COFF import lib**，787/818 個匯出符號是 `_Name@N` stdcall 裝飾，與 GNU ld 的 stdcall 慣例逐位元組相同，不需 gendef/dlltool 轉檔。 |
| `ADVMOTBCB.lib` 能不能用？ | **不能，且不需要。** 它是 Borland OMF 格式（`objdump` 回報 "File format not recognized"），MinGW `ld` 讀不懂；但因為(a)已可行，這條路只是備援，本來就用不到。 |
| x64 lib 是什麼格式？ | **也是 COFF import lib**（`!<arch>` 魔數），但本樹釘死的 binutils 2.28（32-bit MinGW.org 工具鏈）**不認得 PE32+/機器碼 0x8664**——`objdump` 直接報錯。64-bit 是三道牆之一，這裡是第 N 個具體佐證，不是新結論。 |
| 標頭版本差多少？ | **零實質差異**——`EtherCAT/vendor/*.h` 五檔與已裝 SDK 2.0.15.2 逐位元組相同，唯一差異是 SDK 檔多一個 UTF-8 BOM。20260818 那份 recon 記的「65,869→74,348 bytes」落差已經在同一天（樹內檔案 mtime 08/18 10:24-25）被原地升級收斂掉了，現在是同步的。 |
| DLL 執行期需不需要驅動/實體卡？ | 需要（`pci1203s.sys`/`pcie1203s.sys` 隨 `DPInst.exe`/`.inf` 安裝的核心模式驅動）；無卡時的阻塞行為與虛擬卡選項，20260818 recon 已測過，本次未重跑（唯讀稽核不啟動探針）。 |
| 建議的 1203HAL-3 形狀 | CMake 對 `ht9045_pci1203_probe`（或其可執行化分身）`target_link_libraries` 直接指向 x86 `Public/ADVMOT.lib`；不用碰 BCB lib、不用 gendef、不用動態載入層；x64 目前無解、留給日後 64-bit 遷移波次。 |

---

## 1. SDK 安裝路徑與清冊

搜尋範圍：`C:\Advantech`（不存在）、`C:\Program Files\Advantech`（不存在）、
`C:\Program Files (x86)\Advantech`（存在）。指令：

```
ls "/c/Advantech"                          # No such file or directory
ls "/c/Program Files/Advantech"            # No such file or directory
ls "/c/Program Files (x86)/Advantech"      # -> Common Motion/
```

SDK 根：`C:\Program Files (x86)\Advantech\Common Motion`。與 1203 相關的子目錄：

- `PCI_PCIE-1203\` — 驅動安裝套件（`.sys`/`.inf`/`.cat`/`DPInst.exe`），無 lib/header。
- `Public\` — **API 本體**：DLL、x86 import lib、header，全機種共用（不分卡型）。
- `Public\X64\` — x64 版 DLL/import lib 子集。

### 1.1 清冊 + 版本（PowerShell `(Get-Item $f).VersionInfo`）

| 檔案 | 位元 | 大小 | 檔案日期 | FileVersion |
|---|---|---:|---|---|
| `Public\ADVMOT.dll` | x86 | 1,398,640 B | 2025-12-23 | **2.0.15.2** |
| `Public\X64\ADVMOT.dll` | x64 | 1,801,584 B | 2025-12-23 | **2.0.15.2**（與 x86 同版） |
| `Public\ADVMOT.lib` | x86 | 201,316 B | 2025-12-23 | COFF import lib（對映 ADVMOT.dll） |
| `Public\X64\ADVMOT.lib` | x64 | 187,120 B | 2025-12-23 | COFF import lib（對映 X64\ADVMOT.dll） |
| `Public\ADVMOTBCB.lib` | x86 | 94,208 B | **2024-04-01** | Borland OMF import lib，比主 DLL 舊約 20 個月 |
| `Public\AdvMotAPI.dll` | x86 | 303,104 B | — | FileVersion **2.1.2.4**——**注意這不是同一顆元件**，命名極像（`AdvMotAPI` vs `ADVMOT`），版本序列也不同；header 宣告的 `Acm_*` API 對映的是 `ADVMOT.dll`（`BCB.bat` 明寫 `implib ADVMOTBCB.lib ...\ADVMOT.dll`），`AdvMotAPI.dll` 疑似是 Utility 用的另一元件，未深究，**連結時不要選錯這顆**。 |
| `PCI_PCIE-1203\pci1203s.sys` / `pcie1203s.sys` | — | ~128 KB | 2026-01-14 | FileVersion 1.0.21.1（核心驅動，PCI 與 PCIe 兩型號分開簽） |
| `EtherCAT\vendor\AdvMotApi.h`（樹內） | — | 74,345 B | 2026-08-18 | 見第 4 節：與 SDK 逐位元組相同 |

`AdvMotApi.h:2` 版權年份沒標 SDK 版號；版號認證改用上表的 DLL `FileVersion`（`Public\ADVMOT.dll` 與 `Public\X64\ADVMOT.dll` 皆為 2.0.15.2，x86/x64 同版，與 20260818 那份 recon 的記錄一致）。

`Public\IMPLIB.EXE` 與 `Public\BCB.bat` 的存在，加上 `BCB.bat` 內容 `implib ADVMOTBCB.lib c:\windows\system32\ADVMOT.dll`，坐實 `ADVMOTBCB.lib` 是**廠商自己用 Borland 的 `IMPLIB.EXE` 從同一顆 `ADVMOT.dll` 現場產生**的 OMF 版本，不是獨立維護的第二套 API——語意上與 `ADVMOT.lib` 完全一致，差別純粹是檔案格式。

---

## 2. 32-bit link 可行性——三條路的實測

### 2.1 原生 x86 import lib（`Public\ADVMOT.lib`）——**可行**

```
$ objdump -f "Public/ADVMOT.lib"
In archive .../ADVMOT.lib:
ADVMOT.dll:     file format pe-i386
architecture: i386, flags 0x0000003d: HAS_RELOC, HAS_LINENO, HAS_DEBUG, HAS_SYMS, HAS_LOCALS
...
```

`xxd` 開頭 8 bytes 是 `21 3c 61 72 63 68 3e 0a`（`!<arch>\n`）——標準 Unix ar / MS COFF import
library 的檔頭魔數，`objdump`（MinGW binutils 2.28）能完整解析每個 archive member 為
`pe-i386`。這代表它就是 MSVC `lib.exe` 產生的標準 32-bit import lib，GNU `ld` 原生吃得下，
**不需要任何轉檔工具**。

符號裝飾核對（`nm "Public/ADVMOT.lib"`）：

```
00000000 I __imp__Acm_DevOpen@8
00000000 T _Acm_DevOpen@8
00000000 I __imp__Acm_DevClose@4
00000000 T _Acm_DevClose@4
00000000 I __imp__Acm2_GetErrorMessage@12
00000000 T _Acm2_GetErrorMessage@12
00000000 I __imp__Acm_GetErrorMessage@12
00000000 T _Acm_GetErrorMessage@12
```

`_Name@N` 是標準 stdcall 裝飾（前導底線 + `@` + 參數位元組數），GNU ld 對 stdcall DLL 匯入的
慣例與這個格式**逐位元組相同**——這正是 `EtherCAT/AdvMotCompat.h` 已經確保
`ADVCMNAPI == __stdcall` 這件事在 MinGW 端有意義的原因（若沒有那個 shim，宣告會退化成
cdecl，符號名對不上，link 會全炸）。

統計（818 個匯出符號的全體）：

```
$ nm "Public/ADVMOT.lib" | grep -E '^[0-9a-f]+ T _[A-Za-z0-9_]+@[0-9]+$' | wc -l
787
$ nm "Public/ADVMOT.lib" | grep -E '^[0-9a-f]+ T \?' | wc -l
31
```

- **787 個**是純 C stdcall 裝飾（`_Acm_*@N` / `_Acm2_*@N`），MinGW 直連零阻礙。
- **31 個**是 MSVC C++ name-mangling 格式（如 `?Acm2_Ax2DCompensateInAx@@YGKKKPAN0K@Z`）。
  逐一核對 `AdvMotApi.h`：**整份標頭從第 14 行到第 1213 行（幾乎整檔）都在同一個
  `extern "C" { ... }` 區塊內**，沒有任何 `Acm2_Ax2DCompensateInAx` 之類的宣告出現在
  `AdvMotApi.h`／`AdvMotDrv.h` 的公開介面裡（grep 兩檔皆 0 命中）——這 31 個是 DLL 內部殘留、
  未經 `extern "C"` 導出的次要/舊版重載符號，**不在任何公開 header 的宣告範圍內，樹內程式碼
  不可能呼叫到它們**，不構成連結阻礙。若未來要用到（不建議），需要手刻對應的
  `#pragma comment(linker,...)` 或 MSVC mangling 手工比對，成本高、價值低。

**判定：x86 直連 `Public/ADVMOT.lib` 對 MinGW 是可行路徑，且是唯一需要的路徑。**

### 2.2 Borland OMF import lib（`Public\ADVMOTBCB.lib`）——**不可行，也不需要**

```
$ objdump -f "Public/ADVMOTBCB.lib"
objdump.exe: .../ADVMOTBCB.lib: File format not recognized
```

`xxd` 開頭是 `f0 0d 00 00 0a 01 00 33 ...`，不是 `!<arch>`，且往後幾個 byte 就能直接讀到
ASCII 字串（`Acm2_Ax1DCompensateTable`、`ADVMOT.dll`）——這是 Borland/TLIB 的 OMF library
格式（symbol-in-page 結構，非 archive-of-COFF-objects），GNU `ld`/`ar` 完全無法解析。
**這條路徑對 MinGW 是死路**，但因為 2.1 已經有現成可用的原生 COFF lib，這件事無關緊要——
`ADVMOTBCB.lib` 存在的唯一理由是伺候 BCB6（V899 那邊的事，不代評，見
`RECON_PCIE1203_CommonMotion.md` §5 已有結論）。

### 2.3 備援：gendef / 動態載入層——評估但用不到

- `gendef`（mingw-utils 常見工具，從 DLL 匯出表反推 `.def`）**本機未安裝**
  （`which gendef` → not found）。
- `dlltool` **本機已有**（`C:\MinGW\bin\dlltool.exe`，GNU Binutils 2.28），理論上可以
  `objdump -p ADVMOT.dll` 撈匯出表 → 手工/腳本產生 `.def` → `dlltool --input-def` 產生
  MinGW 風格 import lib。`objdump -p` 驗證匯出表可讀：

  ```
  $ objdump -p "Public/ADVMOT.dll" | grep -c 'Acm'
  818
  [Ordinal/Name Pointer] Table
      [  31] Acm2_Ax1DCompensateTable
      [   0] Acm2_Ax2DCompensateInAx
      ...
  ```

  可行，但**因為 2.1 的原生 lib 已直接可用，這條路徑是多餘工序**，不建議在 1203HAL-3 採用。
  記錄在此純粹是完整交代備援路徑存在與否，不是建議走向。
- LoadLibrary/GetProcAddress 動態載入層：同樣是「用不到」的備援——原生靜態 import 已可行，
  動態載入只在「執行期才決定要不要有這顆卡」這種需求下才有價值（例如同一顆 exe 要同時支援
  有卡/無卡環境而不想在編譯期二選一）。這屬於產品需求判斷，非本稽核範圍，留給 1203HAL-3
  決策者。

---

## 3. x64 路徑

```
$ xxd "Public/X64/ADVMOT.lib" | head -2
00000000: 213c 6172 6368 3e0a ...          !<arch>.
$ objdump -f "Public/X64/ADVMOT.lib"
objdump.exe: ADVMOT.dll: File format not recognized
objdump.exe: .../X64/ADVMOT.lib(ADVMOT.dll): Recognised but unhandled machine type (0x8664)
    in Import Library Format archive
```

X64 lib **也是標準 COFF import lib**（同樣的 `!<arch>` 魔數、同樣的 archive 結構），格式本身
沒有問題——問題是**本樹釘死的主 oracle 工具鏈**（`g++.exe (MinGW.org GCC-6.3.0-1) 6.3.0`
配 `GNU Binutils 2.28`，兩者都是這個 32-bit-only 的 MinGW.org 發行版）**認得 PE32（0x14c，
i386）但不認得 PE32+（0x8664，AMD64）**，`objdump` 明確報錯 "unhandled machine type"，不是
檔案本身壞掉。

**對遷移的意義（一句話）**：x64 SDK 本身格式健康、可用，但需要換一套支援 x86_64 目標的
binutils/工具鏈（例如 MinGW-w64）才能碰——這正是既有 KNOWLEDGE 記的「64-bit 是三道牆之一」
在 PCIE-1203 這條線上的具體樣子，不是新牆，是同一道牆的新佐證；本次不需要也不建議現在解，
主 oracle 換掉會動搖 x87 算術忠實度這個更高優先的不可退讓項。

---

## 4. 標頭版本差

逐檔比對 `EtherCAT/vendor/*.h`（樹內，906 讀取的版本）vs `Public/*.h`（SDK 2.0.15.2 現場安裝）：

```
$ for f in AdvMotApi.h AdvMotDev.h AdvMotDrv.h AdvMotErr.h AdvMotPropID.h; do
    wc -l tree/$f pub/$f; diff tree/$f pub/$f; done

AdvMotApi.h   : 1214 行 / 1214 行 — diff: 只有第 1 行 BOM 差異
AdvMotDev.h   :  148 行 /  148 行 — diff: 只有第 1 行 BOM 差異
AdvMotDrv.h   : 2874 行 / 2874 行 — diff: 只有第 1 行 BOM 差異
AdvMotErr.h   :  870 行 /  870 行 — diff: 只有第 1 行 BOM 差異
AdvMotPropID.h:  842 行 /  842 行 — diff: 只有第 1 行 BOM 差異
```

五個檔案，diff 只有一種差異：SDK 現場檔第一行帶 UTF-8 BOM（`﻿//`），樹內檔案沒有。行數、
內容、每一個 byte（BOM 之後）完全相同。

**與 20260818 recon 的落差對帳**：那份文件記的是「AdvMotApi 65,869→74,348 bytes」的真實內容
差異（`RECON_PCIE1203_CommonMotion.md` §2）。樹內 `EtherCAT/vendor/AdvMotApi.h` 現在是
74,345 bytes（= 74,348 − 3 bytes BOM），mtime 為 2026-08-18 10:24——換句話說，**那份 recon
記錄的「可原地升級（但非必要）」這件事，在同一天稍後已經被執行**，五個 vendor 標頭現在與
SDK 2.0.15.2 完全同步。20260818 recon 那條「僅比對 53/622 符號」的警語因此失去意義——
既然是逐位元組相同的檔案，符號簽名 100% 不可能有差異，不需要再顧慮這件事。

---

## 5. DLL runtime 需求

`PCI_PCIE-1203\` 目錄內容：`DPInst.exe`（驅動安裝程式）＋`PCI1203_X64.inf`／
`PCIE1203_X64.inf`（設備安裝資訊）＋`pci1203s.sys`／`pcie1203s.sys`（核心模式驅動，
FileVersion 1.0.21.1，WDF coinstaller 隨附）＋對應 `.cat` 簽章檔。這是標準 Windows KMDF
驅動安裝套件，**代表 `Acm_DevOpen` 這類函式在執行期需要對應的核心驅動已安裝、且
（除非用虛擬卡）需要實體 PCI/PCIe 卡已被驅動列舉到**——DLL 本身是純使用者模式 API 層，
`link` 得起來跟「執行期能不能真的開卡」是兩件事。

無卡/無驅動時的實際行為（阻塞多久、虛擬卡選項）**本次未重新測試**——這需要跑一個會呼叫
`Acm_GetAvailableDevs`/`Acm_DevOpen` 的探針執行檔，屬於「build/背景行程」，本稽核的唯讀
邊界不允許。`RECON_PCIE1203_CommonMotion.md` §4.3-4.4（20260818）已經測過並記錄：
「無卡（且無虛擬卡）時 `Acm_GetAvailableDevs`/`Acm_DevOpen` 阻塞 ≥15s 不返回」、
「SDK 支援虛擬卡 `Adv_V_PCI1203`（`AdvMotDev.h:82,92`），由 Common Motion Utility 註冊」。
1203HAL-3 若要做真連結後的 smoke probe，直接沿用那份記錄的結論（probe 一律包 timeout），
不必重測。

---

## 6. 建議的 1203HAL-3 形狀（一句話）

在 `ht9045_pci1203_probe`（或其可執行化分身）加一行
`target_link_libraries(... "${CM_SDK}/Public/ADVMOT.lib")` 並把 `Public/` 加進
`target_include_directories`（僅 lib 搜尋路徑用，標頭仍走樹內 `EtherCAT/vendor/` 那份已同步
的複本，不要改 include 順序），就能把 787 個 `Acm_*`/`Acm2_*` stdcall 符號解出來；
`ADVMOTBCB.lib`、gendef、動態載入層都不需要碰；x64 留給日後真的啟動 MinGW-w64/64-bit 遷移
波次時再處理，現在強行解只會製造與 x87 oracle 無關的額外風險。

---

## 附錄：工具鏈版本（證據可重現性）

```
g++.exe (MinGW.org GCC-6.3.0-1) 6.3.0
GNU nm (GNU Binutils) 2.28
GNU objdump (GNU Binutils) 2.28
GNU dlltool (GNU Binutils) 2.28（存在但本次未使用；備援路徑用）
gendef：未安裝
```

全部指令與輸出摘錄的原始擷取時間：20260820（本次稽核 session），操作機為本機開發機
（非量產機），路徑皆為只讀瀏覽 + `nm`/`objdump`/PowerShell `Get-Item .VersionInfo`，未寫入
`Public/`、`PCI_PCIE-1203/`、`EtherCAT/vendor/` 或倉庫任何既有檔案。
