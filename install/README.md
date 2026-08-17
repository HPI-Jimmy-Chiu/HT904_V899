# install/ — 各角色需要安裝什麼

> AI(W906-Install) 20260817。使用者要求：把「現場機台需要安裝的」和「開發機需要安裝的最新版本」
> 集中放這裡。本檔的每個數字都是**在這台機器上實測**出來的，不是從文件抄的。

---

## 0. 一句話總結

| 角色 | 要裝什麼 |
|---|---|
| **現場機台：只要跑** | **什麼都不用裝。** 複製 exe 過去就能跑 |
| **現場機台：要遠端診斷** | `gdbserver.exe` 一個檔 |
| **現場機台：要現場改 C++** | 整包 MinGW 資料夾（解壓即用，無安裝程式） |
| **現場機台：只要改畫面** | **什麼都不用裝** — 改 HTML/JS，瀏覽器重整就生效 |
| **開發機** | 現有的釘死不動，另加一套新 GDB + 一套 64-bit 編譯器 |

---

## 1. 現場機台 — 只要「跑」產品：什麼都不用裝

實測 `wb_serve.exe` / `wb_publish.exe` 的 import table（`objdump -p`），兩支完全相同：

```
KERNEL32.dll   PSAPI.DLL   USER32.dll   VERSION.dll   WS2_32.dll   msvcrt.dll
```

**六個全部是 Windows 自己帶的。** 沒有 `libstdc++-6.dll`、沒有 `libgcc_s_dw2-1.dll`、
沒有 VC++ Redistributable。

這不是巧合 — `CMakeLists.txt:218-219` 刻意加了 `-static-libgcc -static-libstdc++`，
理由記在 `:205-217`：DLL 找不到時的載入失敗發生在**任何使用者程式碼（含靜態初始化）
執行之前**，`SetErrorMode` 還來不及呼叫，擋不住那個彈窗 —— 2026-07-29 就這樣卡死過一次
批次執行。代價是每支 exe 大幾百 KB，換到「複製過去就能跑」。

> ⚠️ **這只涵蓋軟體本身。** 等 V906 真的接硬體，運動卡 / IO 卡 / 加密狗的**驅動程式**
> 還是要裝（`DMC32.dll`、`MN200DLL.dll`、`KeyProDLL.dll` 那一類）。那些是硬體廠商的東西，
> 跟編譯器無關，另外處理。

---

## 2. 現場機台 — 要「改畫面」：也什麼都不用裝

UI 是 web 之後，改版面 / 文字 / 顏色 / 加一個顯示欄位 =
**改 `D:\HT9045\web` 底下的 HTML / JS / CSS → 存檔 → 瀏覽器重整**。零編譯、零重開機。

這是 web UI 相對 DFM 表單的實質差別：DFM 改一個 label 都要重編整個 exe。

---

## 3. 現場機台 — 要「遠端診斷」：只裝 `gdbserver.exe`

機台上放 `gdbserver.exe`（單一檔案），開發機用 VS Code 連過去下中斷點。
**機台上不放編譯器、不放原始碼、不放符號檔** — 符號留在開發機。

機台端：
```
gdbserver.exe :2345 wb_serve.exe --pump
```
開發機端在 `launch.json` 用 `miDebuggerServerAddress` 指過去。

`gdbserver.exe` 就在 `_downloads/` 那包 i686 zip 的 `bin\` 裡，單獨複製出來即可。

---

## 4. 現場機台 — 要「現場改 C++」：整包 MinGW

有些客戶要求現場修改且工程師不能自備筆電。技術上很簡單，**因為 MinGW 就是一個資料夾** ——
複製過去、把 `bin` 加進 PATH 就能用，不像 BCB6 要跑安裝程式、要授權。

用 `_downloads\winlibs-i686-gcc16.2.0-msvcrt.zip`，解壓到例如 `D:\mingw32`，
PATH 加 `D:\mingw32\bin`。再加上已經在機台上的 CMake 即可完整重建。

> ⚠️ **這是新能力，也是新風險。** 現場直接改機台上的碼＝繞過版控與測試 ——
> 你們現在的 BCB6 流程做不到這件事（要 IDE），所以這個風險是新的。
>
> 兩個降低風險的做法：
> 1. 機台上那份原始碼是 **git working copy**，改了 `git status` 看得出來、能帶回公司合併
> 2. 更好的解法：如果現場改的多半是「參數 / 行為微調」，**把那些做成設定檔**，
>    讓現場根本不需要改碼。那是設計問題，不是工具問題

---

## 5. 開發機 — 現有的（實測，全部已安裝）

| 工具 | 版本 | 位置 | 動不動 |
|---|---|---|---|
| **MinGW.org g++** | **6.3.0**（`-dumpmachine` = `mingw32`） | `C:\MinGW` | 🔒 **釘死，永不更換** |
| gdb | 7.6.1（2013） | `C:\MinGW\bin\gdb.exe` | 保留當退路 |
| CMake | 4.0.2 | PATH | — |
| Python | 3.13.3 | PATH | — |
| Git | 2.53.0.windows.3 | PATH | — |
| Ninja | **未安裝** | — | 目前用 MinGW Makefiles，不需要 |

### 為什麼 g++ 6.3.0 釘死不換

`build.bat:64-69` 有一道 FATAL 檢查擋著，訊息是：

> MinGW is this project's **PRIMARY oracle** — it is the **only** toolchain that reproduces
> BCB6's x87 arithmetic. **Do not substitute another compiler here.**

BCB6 是 32-bit x87 浮點，中間運算用 **80-bit 擴充精度**；現代編譯器預設走 SSE2（64-bit
double），數字會微幅不同。而我們證明「翻譯翻對了」的唯一方法就是數字要跟 golden 一模一樣 ——
**編譯器一換，這把量尺就沒了**。

它是**驗證工具，永遠不出貨**。產品要用什麼編譯器是另一件事（見 §6）。

---

## 6. 開發機 — 新增的兩套

`_downloads/` 底下兩個 zip，都是 **winlibs GCC 16.2.0 + MinGW-w64 14.0.0 + GDB 17.2**，
**MSVCRT** runtime（刻意選 MSVCRT 不選 UCRT — 我們現有的 exe 就是連 `msvcrt.dll`，一致）。

**兩包都已下載並解壓完成，版本為實測（`--version` / `-dumpmachine`）：**

| 解壓位置 | 架構 | 內含（實測） | 用途 |
|---|---|---|---|
| `D:\HT9045\install\mingw32-16.2.0\mingw32\` | `i686-w64-mingw32` | g++ **16.2.0**、gdb **17.2**、gdbserver **17.2** | **新版 GDB**＋現場 `gdbserver` |
| `D:\HT9045\install\mingw64-16.2.0\mingw64\` | `x86_64-w64-mingw32` | g++ **16.2.0**、gdb **17.2** | **64-bit 相容性警報器**（見下） |

> 這正是使用者想要的 **GDB 17.2** — 不用自己編，winlibs 已經包好了。

### 6a. 新版 GDB — 已就位，且**已實測通過**

`.vscode/launch.json` 的 `miDebuggerPath` 已改指
`${workspaceFolder}/../install/mingw32-16.2.0/mingw32/bin/gdb.exe`。
`C:\MinGW\bin\gdb.exe`（7.6.1）**保留未動**，當退路。

**為什麼可以只換 gdb 不換 g++**：gdb 只是**讀** DWARF 除錯資訊，跟產生機器碼無關。
「新 gdb ＋ 舊 g++ 編出來的 exe」是完全合法的組合。

**實測 20260817**（先前記為「未實測項」，現已解除）：用 `C:\MinGW` 的 g++ **6.3.0** 加 `-g`
編一支含 `std::vector` / `std::string` 的探針，再用新的 **gdb 17.2** 開它：

```
Thread 1 hit Breakpoint 1, compute (zs=std::vector of length 2, capacity 2 = {...},
                                    name="arm.A1") at dwarf_probe.cpp:5
zs = std::vector of length 2, capacity 2 = {{slot = 1, pv = 25.5}, {slot = 2, pv = 0}}
name = "arm.A1"
#0  compute (...) at dwarf_probe.cpp:5
#1  0x0040156b in main () at dwarf_probe.cpp:12
```

中斷點綁到原始碼行、STL 容器有正確 pretty-print、backtrace 帶檔名行號。**全部正常。**

### 6a-2. ⚠️ 還缺一步：現在的 build 沒有除錯符號

實測 `build/CMakeFiles/wb_serve.dir/flags.make` → `CXX_FLAGS = -std=c++1z`，
且 `build/CMakeCache.txt` → `CMAKE_BUILD_TYPE:STRING=`（空的）。**沒有 `-g` 就沒有 DWARF**，
中斷點會綁不上。

修法，且只有一個選項是安全的：

| 選項 | 旗標 | 可用？ |
|---|---|---|
| `Release` | `-O3 -DNDEBUG` | ❌ `CMakeLists.txt:411-424` 記著 `-O3` 會弄壞 `MyPLCModbus` 與 `BarCodeBottom2DID`（兩個真實的潛在缺陷） |
| `RelWithDebInfo` | `-O2 -g -DNDEBUG` | ⚠️ 一樣最佳化過，很可能中同樣兩個缺陷；且區域變數會被優化掉 |
| **`Debug`** | **`-g`**（GCC 預設 `-O0`） | ✅ **這個**。產生的碼跟現在完全一樣，只是多了除錯資訊 |

用**獨立的 build dir**，讓 `build/` 保持原樣，驗收數字才能跟每一份既有基線相比：

```
cmake -S . -B build_dbg -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug
cmake --build build_dbg -j
```

> MinGW 的 `Debug` **不會**像 MSVC 那樣切到 Debug CRT，所以不會踩到
> 「modal assert 彈窗卡死 ctest」那個坑 —— 那是 MSVC 專屬的。

### 6b. 64-bit — 現在只當「警報器」，不當產品

使用者已定案：**未來新舊機台都升級到 906，且避免同時維護兩種版本**。而新機台只能 64-bit、
舊機台的 PC 還是 32-bit。所以 906 最終要**同一份原始碼、出兩種 build**（不是兩份原始碼 ——
那才是「維護兩種版本」）。

CMake 做得到（兩個 toolchain file、兩個 build dir），但驗證工作量會加倍，
**所以不是現在做**。現在做的是最便宜的那一步：

> 用 x86_64 這套定期編一次，**不要求通過測試，只要求編得過**。

這樣任何人寫出 64-bit 不相容的碼（指標塞進 `int`、inline `__asm`、呼叫慣例）
**當天就會被抓到**，而不是三年後一次爆。它同時取代了被移除的 MSVC「次 oracle」角色。

已知一定會擋住的第一個點：`SECSGEM/TasmInfo.cpp` 有 x86 inline `__asm`（CPUID/RDTSC），
x64 必須改成 intrinsics。

### 6c. 64-bit 真正的瓶頸是驅動，不是編譯器

| 硬體 | 64-bit 狀態 |
|---|---|
| CONTEC SMC（41 軸運動主控） | 🟡 廠商有 Win11 64-bit 簽章驅動 |
| Galil DMC32（Index 4 軸） | 🔴 但小改，gclib 有原生 x64（約 7 個函式） |
| MN200DLL（389 點數位 IO） | 🟡 ICP DAS 有 64-bit |
| KeyPro 加密狗 | 🟡 **鴻勁自家的**，可控 |
| `SECSGEM/TasmInfo.cpp` | 🔴 x86 inline `__asm`，須改 intrinsics |

---

## 7. 已移除 / 不再需要

| 項目 | 原因 |
|---|---|
| **MSVC / Visual Studio** | 使用者 20260817 定案移除。原本兩個用途：(a) 編 MFC UI — UI 改 web 後不需要；(b) 次 oracle — 由 §6b 的 64-bit 編譯檢查取代 |
| `build_msvc/`、`build_msvc_ui/` | 已刪（112 MB） |
| **C1061** | MSVC 專屬的編譯器巢狀上限錯誤。MSVC 離開後**完全無關** |

---

## 8. 尚未取得

| 項目 | 狀況 |
|---|---|
| `D:\Work-jimmychiu\software\gdb-17.2` | ❌ **那是原始碼不是程式**，整包 0 個 `.exe`。要編它需要 MSYS2（本機沒有）＋ C++17 host 編譯器（我們只有 g++ 6.3，部分支援）＋ GMP/MPFR。**建議不要編**，用 §6a 的現成二進位檔 |

---

## 9. 位置變更（20260817）

這個資料夾原本在 `D:\HT9045\HT9011UC_Cpp_V3.33.906.0\install\`，使用者手動搬到
`D:\HT9045\install\` —— 也就是從**港內**移到**與 906 樹並排**。

搬對了：這裡是 ~2.4 GB 的第三方工具鏈，不是原始碼，本來就不該長在原始碼樹裡面。
（搬移後 906 樹從 3,950 MB 回到約 1,577 MB，其中真正的原始碼只有約 75 MB。）

**跟著改的引用**：`HT9011UC_Cpp_V3.33.906.0/.vscode/launch.json` 的 5 個
`miDebuggerPath` 改成 `${workspaceFolder}/../install/...`（相對寫法，因為 install 是
906 資料夾固定的兄弟目錄）。本檔內的路徑也一併更新。

本資料夾的 `.gitignore` 跟著搬過來了，所以 `_downloads/`、`mingw32-*/`、`mingw64-*/`
仍然不進 git —— 只有這份 README 和 `.gitignore` 本身進版控。已驗證：
`git status --untracked-files=all -- install/` 只列出 2 個檔。
