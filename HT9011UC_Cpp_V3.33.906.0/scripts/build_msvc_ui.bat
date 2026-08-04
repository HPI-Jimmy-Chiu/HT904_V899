@echo off
REM =============================================================================
REM  build_msvc_ui.bat -- W7-U0 / Gate A: configure+build the MFC UI executable
REM  (HT9045_UI=ON) in its own build dir, then run the exe's smoke check.
REM
REM  AI(W906-GateA-0) 20260804: added per docs\GATE_A_FIRST_LIGHT_PLAN.md GA-0
REM  and docs\DESIGN_GateA.md. Mirrors scripts\build_msvc.bat (W7-A1) exactly --
REM  same vcvarsall x86 -vcvars_ver=14.44, same Ninja, same Release-only rule
REM  (never /MDd: KNOWLEDGE.md Gotcha 9) -- with three differences:
REM
REM    1. -DHT9045_UI=ON and BUILD_DIR=build_msvc_ui (never share build_msvc:
REM       flipping the option in-place would churn that oracle's dir).
REM    2. Builds ONLY the ht9045_app target -- this script is NOT a test
REM       oracle; the non-UI oracle stays scripts\build_msvc.bat.
REM    3. Smoke: runs the produced exe with --devpath --smoke 800 and reports
REM       its exit code. --smoke sets SEM_NOGPFAULTERRORBOX and auto-closes,
REM       so this cannot wedge on a modal dialog (project hard rule).
REM
REM  MFC NOTE: requires "C++ MFC for latest v143 build tools" (installed and
REM  probe-verified 2026-07-29, see MIGRATION_ROADMAP.md keystone 3). If MFC
REM  headers go missing, FAIL -- do not attempt to install anything from here.
REM
REM  USAGE:  scripts\build_msvc_ui.bat
REM =============================================================================
setlocal enabledelayedexpansion

set "VCVARSALL=C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvarsall.bat"
set "NINJA_EXE=C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"
set "VCVARS_VER=14.44"
set "BUILD_DIR=build_msvc_ui"

set "SCRIPT_DIR=%~dp0"
pushd "%SCRIPT_DIR%.."
set "REPO_ROOT=%CD%"
popd

if not exist "%VCVARSALL%" (
    echo [build_msvc_ui] FATAL: vcvarsall.bat not found at "%VCVARSALL%"
    exit /b 1
)
if not exist "%NINJA_EXE%" (
    echo [build_msvc_ui] FATAL: ninja.exe not found at "%NINJA_EXE%"
    exit /b 1
)

echo [build_msvc_ui] Repo root: %REPO_ROOT%
echo [build_msvc_ui] Build dir: %REPO_ROOT%\%BUILD_DIR%
call "%VCVARSALL%" x86 -vcvars_ver=%VCVARS_VER%
if errorlevel 1 (
    echo [build_msvc_ui] FATAL: vcvarsall.bat failed.
    exit /b 1
)

cd /d "%REPO_ROOT%"

echo [build_msvc_ui] Configuring (Ninja, Release, HT9045_UI=ON)...
cmake -S . -B "%BUILD_DIR%" -G Ninja ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DCMAKE_C_COMPILER=cl ^
    -DCMAKE_CXX_COMPILER=cl ^
    -DCMAKE_MAKE_PROGRAM="%NINJA_EXE%" ^
    -DHT9045_UI=ON
if errorlevel 1 (
    echo [build_msvc_ui] FATAL: cmake configure failed.
    exit /b 1
)

echo [build_msvc_ui] Building ht9045_app...
cmake --build "%BUILD_DIR%" --target ht9045_app
if errorlevel 1 (
    echo [build_msvc_ui] FATAL: build failed.
    exit /b 1
)

REM AI(W906-GateA-4-C5) 20260804: W7-C5 headless control probe. Own exe, own
REM build+run step. It creates the three custom controls under an HWND_MESSAGE
REM parent and paints them into a memory DC, so like the smoke run it cannot
REM wedge on a window. Exit code 0 == every check passed.
echo [build_msvc_ui] Building ht9045_ctrl_probe...
cmake --build "%BUILD_DIR%" --target ht9045_ctrl_probe
if errorlevel 1 (
    echo [build_msvc_ui] FATAL: ht9045_ctrl_probe build failed.
    exit /b 1
)

echo [build_msvc_ui] Headless custom-control probe...
"%REPO_ROOT%\%BUILD_DIR%\ht9045_ctrl_probe.exe"
set "PROBE_RC=%ERRORLEVEL%"
echo [build_msvc_ui] probe exit code: %PROBE_RC%
if not "%PROBE_RC%"=="0" (
    echo [build_msvc_ui] FATAL: headless control probe reported failures.
    exit /b %PROBE_RC%
)

echo [build_msvc_ui] Smoke run (--devpath --smoke 800)...
"%REPO_ROOT%\%BUILD_DIR%\HT9045.exe" --devpath --smoke 800
set "SMOKE_RC=%ERRORLEVEL%"
echo [build_msvc_ui] smoke exit code: %SMOKE_RC%

endlocal & exit /b %SMOKE_RC%
