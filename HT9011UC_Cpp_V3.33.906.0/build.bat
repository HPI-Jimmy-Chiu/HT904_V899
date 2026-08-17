@echo off
REM =============================================================================
REM  build.bat -- one-stop build entry for the V906 C++ port.
REM
REM  AI(W906-GateA) 20260804: convenience wrapper requested by the user.
REM  AI(W906-DropMSVC) 20260817: the MSVC modes (ui / run / msvc / all) and
REM  their scriptsuild_msvc*.bat delegates are GONE. The UI is web and MFC
REM  left the product, so MinGW is now the only toolchain this file drives.
REM
REM  USAGE (run from anywhere; paths are resolved from this file's location):
REM
REM    build.bat            Fast incremental MinGW build (configure only if
REM                         needed). This is the everyday "did it still compile"
REM                         command.
REM    build.bat gate       Full MinGW gate: configure + build + ctest. This is
REM                         the number you quote when reporting work.
REM    build.bat test       ctest only (assumes an existing build).
REM    build.bat clean      Delete the default MinGW build dir.
REM    build.bat prune      List stale build_* dirs left over from past waves
REM                         (dry run). "prune -y" deletes them; "prune 7" keeps
REM                         anything touched in the last 7 days.
REM    build.bat help       This text.
REM
REM  EXPECTED ctest BASELINE (2026-08-17): 129 passed / 6 failed of 136.
REM  The 6 are ALWAYS config_db / IniFiles / ini_helpers / config_loaders /
REM  dfm2rc_idempotent / GA1_ReadGeneralIni -- environment drift, NOT code
REM  regressions. Anything else failing IS a regression. (A 7th, WB_Crypto,
REM  can show BAD_COMMAND: that is antivirus quarantining the .exe, not a
REM  test failure -- check whether build	ests	est_wb_crypto.exe exists.)
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
if /i "%MODE%"=="prune" goto :prune
if /i "%MODE%"=="test"  goto :testonly
if /i "%MODE%"=="gate"  goto :mingw
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
    call :exenote
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
call :exenote
endlocal & exit /b %CTEST_RC%

REM ---------------------------------------------------------------------------
REM  AI(W906-GateA-fix) 20260804: added after a real confusion -- the user ran
REM  "build.bat", then went looking for an exe and found none.
REM  AI(W906-DropMSVC) 20260817: that text used to say "MinGW cannot produce an
REM  exe: the UI is MFC, which is MSVC-only". Both halves are now false. The UI
REM  is web, MSVC is gone, and this same MinGW invocation already produces the
REM  three product exes below.
:exenote
echo.
echo [build] Executables produced by this build:
if exist "%SCRIPT_DIR%%BUILD_DIR%\wb_serve.exe" (
    echo [build]   %BUILD_DIR%\wb_serve.exe
) else (
    echo [build]   wb_serve.exe ^(not built yet^)
)
if exist "%SCRIPT_DIR%%BUILD_DIR%\wb_publish.exe" (
    echo [build]   %BUILD_DIR%\wb_publish.exe
) else (
    echo [build]   wb_publish.exe ^(not built yet^)
)
if exist "%SCRIPT_DIR%%BUILD_DIR%\wb_gateway.exe" (
    echo [build]   %BUILD_DIR%\wb_gateway.exe
) else (
    echo [build]   wb_gateway.exe ^(not built yet^)
)
echo [build] They are NOT copied to D:\HT9045\EXE\ -- that directory belongs
echo [build] to the BCB6 production build, and this port must never overwrite it.
goto :eof

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
:clean
if exist "%BUILD_DIR%" (
    echo [build] Removing "%BUILD_DIR%"...
    rmdir /s /q "%BUILD_DIR%"
)
echo [build] Clean done. ^(build_dbg, if you made one, is NOT touched --
echo [build] delete those by hand if you really mean to.^)
endlocal & exit /b 0

REM ---------------------------------------------------------------------------
REM  AI(W906-GateA) 20260804: added after the V906 tree was found sitting at
REM  38.7 GB -- of which 38.58 GB was 137 leftover build dirs (10,508 test .exe
REM  files, ~50 MB each because every gtest target statically links
REM  libht9045_sm.a with full debug symbols). The waves create build_<topic>_
REM  {wave,review,verify,final} dirs on purpose -- concurrent builds corrupt a
REM  shared build dir and invent fake "undefined reference" errors (see
REM  :buildfailed) -- but nothing ever deleted them afterwards. This mode is the
REM  missing other half. Everything it removes is .gitignore'd and rebuildable.
REM
REM    build.bat prune        dry run: list what would go (default: all stale)
REM    build.bat prune -y     actually delete them
REM    build.bat prune 7      dry run, keeping anything touched in last 7 days
REM    build.bat prune 7 -y   delete all but the last 7 days
:prune
set "PRUNE_DAYS="
set "PRUNE_GO="
if /i "%~2"=="-y" (set "PRUNE_GO=1") else (if not "%~2"=="" set "PRUNE_DAYS=%~2")
if /i "%~3"=="-y" set "PRUNE_GO=1"

REM The three dirs build.bat itself drives are never candidates. Note "build"
REM cannot match build_* anyway; it is listed for the reader's benefit.
set "PRUNE_KEEP=[build][build_dbg]"

set "PRUNE_LIST=%TEMP%\ht9045_prune_%RANDOM%.txt"
if exist "%PRUNE_LIST%" del /q "%PRUNE_LIST%"
if defined PRUNE_DAYS (
    echo [build] Stale build dirs NOT touched in the last %PRUNE_DAYS% day^(s^):
    REM forfiles /D -N selects entries last modified on or before N days ago.
    forfiles /P "%SCRIPT_DIR%." /M build_* /D -%PRUNE_DAYS% /C "cmd /c if @isdir==TRUE echo @file" > "%PRUNE_LIST%" 2>nul
) else (
    echo [build] All stale build dirs ^(everything except build / build_dbg^):
    for /d %%D in ("%SCRIPT_DIR%build_*") do echo "%%~nxD">> "%PRUNE_LIST%"
)
if not exist "%PRUNE_LIST%" echo [build] Nothing to prune. & endlocal & exit /b 0

set /a PRUNE_N=0
for /f "usebackq delims=" %%D in ("%PRUNE_LIST%") do (
    set "CAND=%%~D"
    echo !PRUNE_KEEP! | findstr /I /C:"[!CAND!]" >nul
    if errorlevel 1 (
        set /a PRUNE_N+=1
        if defined PRUNE_GO (
            rmdir /s /q "%SCRIPT_DIR%!CAND!" 2>nul
            if exist "%SCRIPT_DIR%!CAND!" (echo   FAILED  !CAND!) else (echo   removed !CAND!)
        ) else (
            echo   !CAND!
        )
    )
)
del /q "%PRUNE_LIST%" 2>nul

echo.
if not defined PRUNE_GO (
    echo [build] DRY RUN -- %PRUNE_N% dir^(s^) would be deleted. Nothing was touched.
    echo [build] Re-run with -y to delete: build.bat prune %PRUNE_DAYS% -y
    endlocal & exit /b 0
)
echo [build] Pruned %PRUNE_N% dir^(s^).
for /f "tokens=3" %%F in ('dir /-c "%SCRIPT_DIR%" ^| findstr /C:"bytes free"') do echo [build] Free on this drive now: %%F bytes
echo [build] WARNING: never prune while a build or ctest is running.
endlocal & exit /b 0

REM ---------------------------------------------------------------------------
:help
echo.
echo   build.bat            fast incremental MinGW build   ^(libs+tests+exes^)
echo   build.bat gate       MinGW configure + build + ctest ^(the reportable gate^)
echo   build.bat test       ctest only
echo   build.bat clean      delete the MinGW build dir
echo   build.bat prune      list leftover build_*_{wave,review,verify} dirs
echo                        ^(dry run^); "prune -y" deletes, "prune 7 -y" keeps
echo                        anything touched in the last 7 days
echo.
echo   WHERE IS THE EXE?  build\wb_serve.exe, wb_publish.exe, wb_gateway.exe
echo   -- produced by the SAME MinGW build as the libs. They are never
echo   copied into D:\HT9045\EXE\ ^(that dir is the BCB6 production output^).
echo.
echo   ctest baseline 2026-08-17: 129/136 pass. SIX known failures
echo   ^(config_db/IniFiles/ini_helpers/config_loaders/dfm2rc_idempotent/
echo    GA1_ReadGeneralIni^) are environment drift, not regressions.
echo.
endlocal & exit /b 0
