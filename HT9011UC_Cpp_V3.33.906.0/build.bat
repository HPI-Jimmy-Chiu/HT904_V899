@echo off
REM =============================================================================
REM  build.bat -- one-stop build entry for the V906 C++ port.
REM
REM  AI(W906-GateA) 20260804: convenience wrapper requested by the user. It does
REM  NOT reimplement anything: the MSVC paths delegate to the existing
REM  scripts\build_msvc.bat (W7-A1 second oracle) and scripts\build_msvc_ui.bat
REM  (W7-U0 / Gate A MFC exe), so there is exactly one definition of each
REM  toolchain's rules.
REM
REM  USAGE (run from anywhere; paths are resolved from this file's location):
REM
REM    build.bat            Fast incremental MinGW build (configure only if
REM                         needed). This is the everyday "did it still compile"
REM                         command.
REM    build.bat gate       Full MinGW gate: configure + build + ctest. This is
REM                         the number you quote when reporting work.
REM    build.bat test       ctest only (assumes an existing build).
REM    build.bat ui         MSVC MFC UI: builds HT9045.exe, runs the headless
REM                         control probe, then the --smoke window check.
REM    build.bat msvc       MSVC second oracle (non-UI libs + tests).
REM    build.bat all        gate + ui.
REM    build.bat clean      Delete the default MinGW build dir.
REM    build.bat help       This text.
REM
REM  EXPECTED ctest BASELINE (2026-08-04): 122 passed / 4 failed of 126.
REM  The 4 failures are ALWAYS config_db / IniFiles / ini_helpers /
REM  config_loaders -- they are environment drift (the real system\Gerneral.ini
REM  on this machine has been edited since those oracles were pinned), NOT code
REM  regressions. Anything else failing IS a regression.
REM
REM  NOTE ON LINE ENDINGS: this file is CRLF on purpose, unlike the LF-only
REM  convention of the rest of the tree -- cmd.exe's `goto` label scanning is
REM  unreliable in LF-only batch files, and this script dispatches with goto.
REM =============================================================================
setlocal enabledelayedexpansion

set "MINGW_BIN=C:\MinGW\bin"
set "BUILD_DIR=build"
set "SCRIPT_DIR=%~dp0"
cd /d "%SCRIPT_DIR%"

set "MODE=%~1"
if "%MODE%"=="" set "MODE=quick"

if /i "%MODE%"=="help"  goto :help
if /i "%MODE%"=="clean" goto :clean
if /i "%MODE%"=="ui"    goto :ui
if /i "%MODE%"=="msvc"  goto :msvc
if /i "%MODE%"=="test"  goto :testonly
if /i "%MODE%"=="gate"  goto :mingw
if /i "%MODE%"=="all"   goto :mingw
if /i "%MODE%"=="quick" goto :mingw
echo [build] Unknown mode "%MODE%".
goto :help

REM ---------------------------------------------------------------------------
:mingw
if not exist "%MINGW_BIN%\g++.exe" (
    echo [build] FATAL: MinGW g++ not found at "%MINGW_BIN%\g++.exe".
    echo [build] MinGW is this project's PRIMARY oracle -- it is the only
    echo [build] toolchain that reproduces BCB6's x87 arithmetic. Do not
    echo [build] substitute another compiler here.
    exit /b 1
)
set "PATH=%MINGW_BIN%;%PATH%"

REM Number of parallel jobs: leave 2 cores free so the machine stays usable.
set /a JOBS=%NUMBER_OF_PROCESSORS%-2
if %JOBS% LSS 1 set JOBS=1

if not exist "%BUILD_DIR%\CMakeCache.txt" (
    echo [build] Configuring MinGW build dir "%BUILD_DIR%" ^(first time^)...
    cmake -S . -B "%BUILD_DIR%" -G "MinGW Makefiles" ^
        -DCMAKE_CXX_COMPILER=%MINGW_BIN:\=/%/g++.exe ^
        -DCMAKE_C_COMPILER=%MINGW_BIN:\=/%/gcc.exe
    if errorlevel 1 (
        echo [build] FATAL: cmake configure failed.
        exit /b 1
    )
) else (
    REM Cheap re-configure: picks up new sources/targets added to CMakeLists.
    cmake -S . -B "%BUILD_DIR%" >nul
    if errorlevel 1 (
        echo [build] FATAL: cmake re-configure failed ^(run "build.bat clean" then retry^).
        exit /b 1
    )
)

REM Live output AND a log: tee if available (MSYS/Git ship it), else log-then-show.
REM The log matters because -j%JOBS% interleaves output -- a compiler error can
REM scroll past hundreds of progress lines, so :errsummary re-extracts it below.
set "TEE="
if exist "C:\MinGW\msys\1.0\bin\tee.exe" set "TEE=C:\MinGW\msys\1.0\bin\tee.exe"
if not defined TEE if exist "C:\Program Files\Git\usr\bin\tee.exe" set "TEE=C:\Program Files\Git\usr\bin\tee.exe"

set "BUILD_LOG=%SCRIPT_DIR%build_last.log"
echo [build] Building ^(MinGW, -j%JOBS%^)... log: build_last.log
if defined TEE (
    cmake --build "%BUILD_DIR%" -j %JOBS% 2>&1 | "%TEE%" "%BUILD_LOG%"
    REM With a pipe, ERRORLEVEL is tee's -- detect failure from the log instead.
    findstr /C:" error:" /C:"Error 1" /C:"Error 2" /C:"undefined reference" /C:"multiple definition" /C:"CMake Error" "%BUILD_LOG%" >nul 2>&1
    if not errorlevel 1 goto :buildfailed
) else (
    cmake --build "%BUILD_DIR%" -j %JOBS% > "%BUILD_LOG%" 2>&1
    if errorlevel 1 (
        type "%BUILD_LOG%"
        goto :buildfailed
    )
    type "%BUILD_LOG%"
)
echo [build] Build OK.

if /i "%MODE%"=="quick" (
    echo [build] Skipping ctest ^(use "build.bat gate" to run it^).
    endlocal & exit /b 0
)

:testonly
if not exist "%BUILD_DIR%\CMakeCache.txt" (
    echo [build] FATAL: no build dir -- run "build.bat" first.
    exit /b 1
)
set "PATH=%MINGW_BIN%;%PATH%"
echo [build] Running ctest ^(--timeout 600^)...
ctest --test-dir "%BUILD_DIR%" --timeout 600
set "CTEST_RC=%ERRORLEVEL%"
echo [build] ctest exit code: %CTEST_RC%
echo [build] REMINDER: 4 failures ^(config_db/IniFiles/ini_helpers/config_loaders^)
echo [build] are the known environment drift, not regressions. Compare the
echo [build] failing-test LIST, never just the exit code.
if /i "%MODE%"=="all" goto :ui
endlocal & exit /b %CTEST_RC%

REM ---------------------------------------------------------------------------
REM  Compact failure report. -j builds interleave output, so the real error is
REM  usually NOT the last thing on screen -- this pulls it back out of the log
REM  with file:line intact (g++ emits "path\file.cpp:LINE:COL: error: ...").
:buildfailed
echo.
echo ==========================================================================
echo  BUILD FAILED -- compiler/linker diagnostics ^(file:line^):
echo ==========================================================================
findstr /N /C:" error:" /C:"CMake Error" /C:"undefined reference" /C:"multiple definition" /C:"first defined here" "%BUILD_LOG%"
echo ==========================================================================
echo  Full log: %BUILD_LOG%
echo  Tips:
echo    * "undefined reference" = a symbol has no definition in the link closure
echo      -- usually a missing library in tests\CMakeLists.txt's LINK_GROUP, or
echo      a still-gated ^(#if 0^) body. NOT necessarily your edit.
echo    * "multiple definition" = a stand-in stub now collides with a real body
echo      that just landed; retire the stub ^(see DEVLOG's stub-retirement notes^).
echo    * Concurrent builds corrupt a build dir and invent fake undefined
echo      references -- never run two builds against "%BUILD_DIR%" at once.
echo ==========================================================================
endlocal & exit /b 1

REM ---------------------------------------------------------------------------
:ui
echo [build] Delegating to scripts\build_msvc_ui.bat ^(MSVC + MFC^)...
call "%SCRIPT_DIR%scripts\build_msvc_ui.bat"
set "UI_RC=%ERRORLEVEL%"
echo [build] UI pipeline exit code: %UI_RC%
if "%UI_RC%"=="0" (
    echo [build] HT9045.exe: %SCRIPT_DIR%%BUILD_DIR%_msvc_ui\HT9045.exe
    echo [build]   interactive : HT9045.exe --devpath
    echo [build]   headless    : HT9045.exe --devpath --smoke 800
    echo [build] NOTE: run it from cmd.exe, not PowerShell -- PowerShell does
    echo [build] not wait on GUI-subsystem exes and reports no exit code.
)
endlocal & exit /b %UI_RC%

REM ---------------------------------------------------------------------------
:msvc
echo [build] Delegating to scripts\build_msvc.bat ^(MSVC second oracle^)...
call "%SCRIPT_DIR%scripts\build_msvc.bat"
set "MSVC_RC=%ERRORLEVEL%"
echo [build] MSVC oracle exit code: %MSVC_RC%
echo [build] REMINDER: MSVC-only failures are advisory until W7-A3 triages
echo [build] them ^(see docs\W7_UI_ARCHITECTURE_PLAN.md SS6/SS9-R13^).
endlocal & exit /b %MSVC_RC%

REM ---------------------------------------------------------------------------
:clean
if exist "%BUILD_DIR%" (
    echo [build] Removing "%BUILD_DIR%"...
    rmdir /s /q "%BUILD_DIR%"
)
echo [build] Clean done. ^(build_msvc / build_msvc_ui are NOT touched --
echo [build] delete those by hand if you really mean to.^)
endlocal & exit /b 0

REM ---------------------------------------------------------------------------
:help
echo.
echo   build.bat            fast incremental MinGW build
echo   build.bat gate       MinGW configure + build + ctest   ^(the reportable gate^)
echo   build.bat test       ctest only
echo   build.bat ui         MSVC MFC UI exe + probe + smoke
echo   build.bat msvc       MSVC second oracle
echo   build.bat all        gate + ui
echo   build.bat clean      delete the MinGW build dir
echo.
echo   ctest baseline 2026-08-04: 122/126 pass; the 4 failures
echo   ^(config_db/IniFiles/ini_helpers/config_loaders^) are environment drift.
echo.
endlocal & exit /b 0
