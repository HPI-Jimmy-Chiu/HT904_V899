@echo off
REM =============================================================================
REM  build_msvc.bat -- W7-A1 second-compiler oracle: configure/build/test this
REM  repo with MSVC (cl.exe) instead of MinGW g++.
REM
REM  AI(W906-W7-A1) 20260728: added per docs\W7_UI_ARCHITECTURE_PLAN.md SS6
REM  W7-A1. See that section (and SS4-V5 / SS4-V8 / SS9-R13) for the full
REM  rationale; only the load-bearing constraints are repeated here:
REM
REM    * Ninja generator, NOT "Visual Studio 17 2022" -- the VS generator is
REM      multi-config and defaults to Debug. A Debug-CRT assert previously
REM      raised a MODAL "Debug Assertion Failed!" dialog that wedged an
REM      entire ctest run at 94/95 and logged nothing while blocked
REM      (KNOWLEDGE.md Gotcha 9). Ninja is single-config, so
REM      -DCMAKE_BUILD_TYPE=Release takes effect directly and unambiguously.
REM    * -DCMAKE_BUILD_TYPE=Release is MANDATORY. Never remove it, never call
REM      this script's cmake line without it, and never build this tree with
REM      the MSVC Debug CRT (/MDd) for any reason.
REM    * -DHT9045_UI=OFF: the MFC UI layer is not buildable yet (MFC
REM      component not installed on this machine -- see plan SS7). This
REM      oracle only builds/tests the non-UI core, same as the MinGW oracle
REM      does today.
REM    * x86 (32-bit): this project's driver layer is a hard 32-bit
REM      dependency (plan D9), and the /arch:IA32 + _controlfp_s FP-fidelity
REM      pair (root CMakeLists.txt if(MSVC) block + tests\test_bootstrap.cpp)
REM      only applies to 32-bit MSVC.
REM    * ctest --timeout 600: every test process is bounded so a hang cannot
REM      wedge this script indefinitely (hard prohibition #2).
REM
REM  USAGE:  scripts\build_msvc.bat
REM  Run from anywhere; paths below are absolute. Must be run in a plain
REM  cmd.exe (not already inside a vcvarsall-initialized shell) -- vcvarsall
REM  is idempotent-ish but re-running it in an already-initialized shell for
REM  a DIFFERENT arch/toolset can produce a stale/mismatched PATH, so prefer
REM  a fresh shell.
REM
REM  KNOWN GAP (report honestly, do not silently work around): the VS2022
REM  BuildTools install on this machine has VC Tools 14.44.35207 and
REM  14.42.34433 installed side-by-side, but no MFC headers/libs
REM  (atlmfc\include, afxwin.h, mfc*.lib are all absent -- see plan SS1/SS7).
REM  That does not block this script (HT9045_UI=OFF), but a future
REM  build_msvc_ui.bat variant will need it and must NOT attempt to install
REM  it itself (hard prohibition #4 -- UAC-gated, would race other agents).
REM =============================================================================
setlocal enabledelayedexpansion

REM AI(W906-GateA-vsdiscover) 20260811: same change as scripts\build_msvc_ui.bat
REM -- the hard-coded BuildTools path died when VS 2022 Professional 17.14.37
REM replaced BuildTools on 2026-08-11. Discover the install with vswhere so any
REM edition works. See build_msvc_ui.bat for the full note.
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
set "VS_INSTALL="
if exist "%VSWHERE%" for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "VS_INSTALL=%%i"
set "VCVARSALL=%VS_INSTALL%\VC\Auxiliary\Build\vcvarsall.bat"
set "NINJA_EXE=%VS_INSTALL%\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"
if not exist "%NINJA_EXE%" for %%n in (ninja.exe) do if not "%%~$PATH:n"=="" set "NINJA_EXE=%%~$PATH:n"
set "VCVARS_VER=14.44"
set "BUILD_DIR=build_msvc"

REM Resolve repo root as the directory this script lives in, one level up
REM (scripts\build_msvc.bat -> repo root), so it works regardless of cwd.
set "SCRIPT_DIR=%~dp0"
pushd "%SCRIPT_DIR%.."
set "REPO_ROOT=%CD%"
popd

if not defined VS_INSTALL (
    echo [build_msvc] FATAL: no Visual Studio install carrying the C++ toolset
    echo [build_msvc] was found via "%VSWHERE%". Install the "Desktop
    echo [build_msvc] development with C++" workload ^(or VS Build Tools^), then
    echo [build_msvc] re-run -- do NOT install or modify VS tooling from this
    echo [build_msvc] script.
    exit /b 1
)
echo [build_msvc] VS install: %VS_INSTALL%
if not exist "%VCVARSALL%" (
    echo [build_msvc] FATAL: vcvarsall.bat not found at "%VCVARSALL%"
    echo [build_msvc] ^(discovered install: %VS_INSTALL%^) -- do NOT install or
    echo [build_msvc] modify VS tooling from this script.
    exit /b 1
)

if not exist "%NINJA_EXE%" (
    echo [build_msvc] FATAL: ninja.exe not found at "%NINJA_EXE%"
    echo [build_msvc] and no ninja.exe on PATH. It ships with the VS component
    echo [build_msvc] "C++ CMake tools for Windows"
    echo [build_msvc] ^(Microsoft.VisualStudio.Component.VC.CMake.Project^).
    exit /b 1
)

echo [build_msvc] Repo root: %REPO_ROOT%
echo [build_msvc] Build dir: %REPO_ROOT%\%BUILD_DIR%
echo [build_msvc] Initializing MSVC x86 environment (vcvars_ver=%VCVARS_VER%)...

call "%VCVARSALL%" x86 -vcvars_ver=%VCVARS_VER%
if errorlevel 1 (
    echo [build_msvc] FATAL: vcvarsall.bat failed.
    exit /b 1
)

cd /d "%REPO_ROOT%"

echo [build_msvc] Configuring (Ninja, Release, HT9045_UI=OFF)...
cmake -S . -B "%BUILD_DIR%" -G Ninja ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DCMAKE_C_COMPILER=cl ^
    -DCMAKE_CXX_COMPILER=cl ^
    -DCMAKE_MAKE_PROGRAM="%NINJA_EXE%" ^
    -DHT9045_UI=OFF
if errorlevel 1 (
    echo [build_msvc] FATAL: cmake configure failed.
    exit /b 1
)

echo [build_msvc] Building...
cmake --build "%BUILD_DIR%"
if errorlevel 1 (
    echo [build_msvc] FATAL: build failed.
    exit /b 1
)

echo [build_msvc] Testing (ctest --timeout 600 -j4)...
pushd "%BUILD_DIR%"
ctest --timeout 600 -j4 --output-on-failure
set "CTEST_RC=%ERRORLEVEL%"
popd

echo [build_msvc] ctest exit code: %CTEST_RC%
echo [build_msvc] NOTE: per docs\W7_UI_ARCHITECTURE_PLAN.md SS6/SS9-R13, MSVC-only
echo [build_msvc] failures are EXPECTED and advisory until W7-A3 triages them --
echo [build_msvc] do not treat a non-zero exit here as a blocking regression by
echo [build_msvc] itself; compare the printed failing-test list against the
echo [build_msvc] MinGW baseline and W7-A3's known-differences list.

endlocal & exit /b %CTEST_RC%
