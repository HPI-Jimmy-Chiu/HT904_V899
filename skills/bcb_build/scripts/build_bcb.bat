@echo off
setlocal enabledelayedexpansion

:: ============================================================
:: build_bcb.bat — 通用 Borland C++ Builder 6 建構腳本
:: 用法: build_bcb.bat <PROJECT_DIR> <BPR_FILE> [clean|rebuild]
:: ============================================================

:: ---------- 參數解析 ----------
set "PROJECT_DIR=%~1"
set "BPR_FILE=%~2"
set "BUILD_MODE=%~3"

if "%PROJECT_DIR%"=="" (
    echo [BCB Build] ERROR: 請指定專案目錄
    echo 用法: build_bcb.bat ^<PROJECT_DIR^> ^<BPR_FILE^> [clean^|rebuild]
    exit /b 1
)
if "%BPR_FILE%"=="" (
    echo [BCB Build] ERROR: 請指定 .bpr 檔名
    echo 用法: build_bcb.bat ^<PROJECT_DIR^> ^<BPR_FILE^> [clean^|rebuild]
    exit /b 1
)

:: ---------- BCB 路徑設定 ----------
if not defined BCB_ROOT set "BCB_ROOT=D:\ProgramFiles\Borland\CBuilder6"

if not exist "%BCB_ROOT%\Bin\bpr2mak.exe" (
    echo [BCB Build] ERROR: 找不到 bpr2mak.exe，請確認 BCB_ROOT=%BCB_ROOT%
    exit /b 1
)

set "BCB=%BCB_ROOT%"
set "PATH=%BCB_ROOT%\Bin;%PATH%"

:: ---------- 計算 MAK 檔名 ----------
set "MAK_FILE=%BPR_FILE:.bpr=.mak%"

:: ---------- 切換到專案目錄 ----------
if not exist "%PROJECT_DIR%" (
    echo [BCB Build] ERROR: 專案目錄不存在: %PROJECT_DIR%
    exit /b 1
)
cd /d "%PROJECT_DIR%"
echo [BCB Build] 工作目錄: %CD%
echo [BCB Build] 專案檔案: %BPR_FILE%
echo [BCB Build] 模式:     %BUILD_MODE%
echo [BCB Build] BCB 路徑: %BCB_ROOT%
echo.

:: ---------- Clean 模式 ----------
if /I "%BUILD_MODE%"=="clean" goto :do_clean
if /I "%BUILD_MODE%"=="rebuild" goto :do_clean
goto :do_build

:do_clean
:: 先產生 Makefile 以讀取實際輸出路徑
echo [BCB Build] === Step 1: 解析專案路徑 ===
bpr2mak "%BPR_FILE%" 2>nul

:: 從 Makefile 動態解析 OBJ 目錄與 PCH 目錄
set "OBJ_DIR=..\Obj"
set "PCH_DIR="
if exist "%MAK_FILE%" (
    for /f "usebackq delims=" %%A in (`powershell -NoProfile -Command "try{$m=Get-Content '%MAK_FILE%' -Raw;$o=([regex]'OBJFILES\s*=\s*(\S+)').Match($m).Groups[1].Value;if($o){[IO.Path]::GetDirectoryName($o)}else{'..\Obj'}}catch{'..\Obj'}"`) do set "OBJ_DIR=%%A"
    for /f "usebackq delims=" %%B in (`powershell -NoProfile -Command "try{$m=Get-Content '%MAK_FILE%' -Raw;$h=([regex]'-H=(\S+)').Match($m).Groups[1].Value;if($h){[IO.Path]::GetDirectoryName($h)}else{''}}catch{''}"`) do set "PCH_DIR=%%B"
)

echo [BCB Build] === 清除中間產物 ===
echo [BCB Build]   OBJ DIR: !OBJ_DIR!
if defined PCH_DIR echo [BCB Build]   PCH DIR: !PCH_DIR!
echo.

:: 清除 .obj 和 .res 檔案
if exist "!OBJ_DIR!\*.obj" (
    del /Q "!OBJ_DIR!\*.obj"
    echo [BCB Build] 已刪除 .obj 檔案
)
if exist "!OBJ_DIR!\*.res" (
    del /Q "!OBJ_DIR!\*.res"
    echo [BCB Build] 已刪除 .res 檔案
)
:: 清除 PCH 快取 (vcl60.csm / vcl60.#00)
if defined PCH_DIR (
    if exist "!PCH_DIR!\*.csm" del /Q "!PCH_DIR!\*.csm"
    if exist "!PCH_DIR!\*.#00" del /Q "!PCH_DIR!\*.#00"
    echo [BCB Build] 已清除 PCH 快取
)
:: 刪除 Makefile
if exist "%MAK_FILE%" (
    del /Q "%MAK_FILE%"
    echo [BCB Build] 已刪除舊 Makefile
)

if /I "%BUILD_MODE%"=="clean" (
    echo [BCB Build] Clean 完成
    exit /b 0
)
echo.

:do_build
:: ---------- Step 1: 產生 Makefile ----------
echo [BCB Build] === Step 1: 產生 Makefile ===
bpr2mak "%BPR_FILE%"
if %ERRORLEVEL% NEQ 0 (
    echo [BCB Build] ERROR: bpr2mak 失敗，錯誤碼 %ERRORLEVEL%
    exit /b %ERRORLEVEL%
)
if not exist "%MAK_FILE%" (
    echo [BCB Build] ERROR: bpr2mak 未產生 %MAK_FILE%
    exit /b 1
)
echo [BCB Build] Makefile 已產生: %MAK_FILE%
echo.

:: ---------- Step 2: 執行編譯 ----------
echo [BCB Build] === Step 2: 執行 make ===
make -f "%MAK_FILE%"
set BUILD_RESULT=%ERRORLEVEL%

echo.
if %BUILD_RESULT% NEQ 0 (
    echo [BCB Build] BUILD FAILED — 錯誤碼 %BUILD_RESULT%
    exit /b %BUILD_RESULT%
)

echo [BCB Build] BUILD SUCCEEDED
echo.

:: ---------- 嘗試尋找並顯示輸出 EXE ----------
for /f "tokens=2 delims==" %%A in ('findstr /i "^PROJECT=" "%MAK_FILE%" 2^>nul') do (
    set "EXE_PATH=%%A"
)
if defined EXE_PATH (
    set "EXE_PATH=!EXE_PATH: =!"
    if exist "!EXE_PATH!" (
        echo [BCB Build] 輸出檔案:
        dir "!EXE_PATH!" | findstr /i ".exe"
    ) else (
        echo [BCB Build] 注意: 未在 !EXE_PATH! 找到 EXE（可能輸出至其他位置）
    )
)

endlocal
