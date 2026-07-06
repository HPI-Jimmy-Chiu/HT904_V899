---
name: bcb_build
description: 使用 Borland C++ Builder 6 (BCB6) 的命令列工具，對任意 BCB6 專案執行自動化編譯。支援 bpr2mak 產生 Makefile 後以 make 建構，並提供 clean、rebuild 模式。
---

# Borland C++ Builder 6 — 通用 Build Skill

## 環境需求

| 項目 | 預設值 | 說明 |
|------|--------|------|
| BCB6 安裝路徑 | `D:\ProgramFiles\Borland\CBuilder6` | 可透過 `%BCB_ROOT%` 覆寫 |
| 必要工具 | `bpr2mak.exe`, `make.exe`, `bcc32.exe` | 位於 `<BCB_ROOT>\Bin\` |

## 使用方式

### 1. 快速呼叫 (PowerShell)

```powershell
# 基本建構 — 指定專案目錄與 .bpr 檔名
.\scripts\build_bcb.bat "D:\Project\RDCode\HT9011UC_Code_V3.20" "HT9045.bpr"

# 重新建構 (先 clean 再 build)
.\scripts\build_bcb.bat "D:\Project\RDCode\HT9011UC_Code_V3.20" "HT9045.bpr" rebuild

# 僅清除中間產物
.\scripts\build_bcb.bat "D:\Project\RDCode\HT9011UC_Code_V3.20" "HT9045.bpr" clean
```

### 2. 腳本參數說明

```
build_bcb.bat <PROJECT_DIR> <BPR_FILE> [clean|rebuild]

  PROJECT_DIR  — 專案根目錄（包含 .bpr 的資料夾）
  BPR_FILE     — BCB6 專案檔名稱（例如 HT9045.bpr）
  [模式]        — 可選：clean / rebuild（預設為增量編譯）
```

### 3. 手動步驟

```powershell
$BCB  = "D:\ProgramFiles\Borland\CBuilder6"
$env:BCB  = $BCB
$env:PATH = "$BCB\Bin;" + $env:PATH

cd "D:\Project\RDCode\HT9011UC_Code_V3.20"

# Step 1: 產生 Makefile
bpr2mak HT9045.bpr

# Step 2: 執行編譯
make -f HT9045.mak
```

## 建構流程

```
.bpr  ──bpr2mak──►  .mak  ──make──►  .obj (Obj/)  ──ilink32──►  .exe
```

## 注意事項

- **.bpr 內的路徑為硬編碼**：`PROJECT` (輸出 EXE) 與 `OBJFILES` 目錄須確保存在，否則 ilink32 會失敗。
- **Obj 目錄**：BCB6 預設將 `.obj` 輸出至 `.bpr` 中 `OBJFILES` 指定的路徑（通常為 `..\Obj\`）。若不存在請先手動建立。
- **PCH 目錄**：`.bpr` 中 `-H=` 旗標指定的 PCH 快取目錄（例如 `D:\HT9045\Obj\`）也必須事先建立，否則每個編譯單元都會產生 W8058 警告。
- **Precompiled Header**：若 `.bpr` 啟用 PCH，第一次編譯較慢；`clean` 後會重新產生。
- **BCB_ROOT 覆寫**：腳本尊重 `%BCB_ROOT%` 環境變數，若不設則使用預設路徑。
- **Arm 變體重複符號警告**：HT9011UC 等大型專案中，多個 Arm 變體 `.obj` 之間可能產生 `Public symbol defined in both module` 的 linker 警告，這是預期行為（inline 函式重複定義），不影響連結結果。

## HT9011UC 專案首次建構前置步驟

```powershell
# 1. 建立所有必要目錄
New-Item -ItemType Directory -Force "D:\Project\RDCode\Obj"  # OBJFILES 目錄
New-Item -ItemType Directory -Force "D:\HT9045\Obj"          # PCH 快取目錄
New-Item -ItemType Directory -Force "D:\HT9045\EXE"          # EXE 輸出目錄

# 2. 執行建構
.\scripts\build_bcb.bat "D:\Project\RDCode\HT9011UC_Code_V3.20" "HT9045.bpr"
```

## 常見錯誤排查

| 錯誤訊息 | 原因 | 解法 |
|----------|------|------|
| `bpr2mak not found` | PATH 未包含 BCB Bin | 確認 BCB_ROOT 正確 |
| `Fatal: Unable to open file '*.obj'` | Obj 目錄不存在 | `mkdir D:\Project\RDCode\Obj` |
| `Error: Unresolved external` | 缺少 .lib 或 .dll | 核對 .bpr 中 LIBRARIES 設定 |
| `Out of memory` | PCH 過大 | 增加 Windows 虛擬記憶體 |
