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

REM AI(W906-GateA-vsdiscover) 20260811: discover the VS install instead of
REM hard-coding it. This script used to hard-code
REM   C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\...
REM which stopped existing when VS 2022 Professional 17.14.37 was installed on
REM 2026-08-11 and BuildTools went away with it. The very first `if not exist`
REM then fired and the script exited 1 before touching a compiler, so F5 in
REM VS Code reported only "preLaunchTask 'V906: Build UI exe (MSVC/MFC)'
REM terminated with exit code 1" -- no compiler diagnostic, because no
REM compiler ever ran. vswhere is the supported way to locate ANY edition
REM (BuildTools/Community/Professional/Enterprise) and every edition installs
REM it at the same fixed path. -requires pins the answer to an install that
REM really carries the C++ toolset, so a VS with only .NET workloads is
REM rejected here with a readable message instead of at cl.exe.
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
set "VS_INSTALL="
if exist "%VSWHERE%" for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "VS_INSTALL=%%i"
set "VCVARSALL=%VS_INSTALL%\VC\Auxiliary\Build\vcvarsall.bat"
set "NINJA_EXE=%VS_INSTALL%\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"
REM Ninja ships with the "C++ CMake tools for Windows" component. If that
REM component is absent, fall back to any ninja on PATH before giving up.
if not exist "%NINJA_EXE%" for %%n in (ninja.exe) do if not "%%~$PATH:n"=="" set "NINJA_EXE=%%~$PATH:n"
set "VCVARS_VER=14.44"
set "BUILD_DIR=build_msvc_ui"

set "SCRIPT_DIR=%~dp0"
pushd "%SCRIPT_DIR%.."
set "REPO_ROOT=%CD%"
popd

if not defined VS_INSTALL (
    echo [build_msvc_ui] FATAL: no Visual Studio install carrying the C++
    echo [build_msvc_ui] toolset was found via "%VSWHERE%".
    echo [build_msvc_ui] Install the "Desktop development with C++" workload
    echo [build_msvc_ui] ^(or VS Build Tools^), then re-run. This script never
    echo [build_msvc_ui] installs or modifies VS tooling itself.
    exit /b 1
)
echo [build_msvc_ui] VS install: %VS_INSTALL%
if not exist "%VCVARSALL%" (
    echo [build_msvc_ui] FATAL: vcvarsall.bat not found at "%VCVARSALL%"
    exit /b 1
)
if not exist "%NINJA_EXE%" (
    echo [build_msvc_ui] FATAL: ninja.exe not found at "%NINJA_EXE%"
    echo [build_msvc_ui] and no ninja.exe on PATH. It ships with the VS
    echo [build_msvc_ui] component "C++ CMake tools for Windows"
    echo [build_msvc_ui] ^(Microsoft.VisualStudio.Component.VC.CMake.Project^).
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

REM AI(W906-GateA-vsdiscover) 20260811: configure, and on failure wipe the build
REM dir and configure ONCE more.
REM
REM WHY: CMake records the RESOLVED absolute compiler path in
REM   build_msvc_ui\CMakeFiles\<cmake-ver>\CMakeCXXCompiler.cmake
REM and the resolved ninja in CMakeCache.txt. Those records WIN over the
REM -DCMAKE_CXX_COMPILER / -DCMAKE_MAKE_PROGRAM flags below. So once the VS
REM install they point at disappears -- which is exactly what happened when
REM BuildTools was replaced by Professional on 2026-08-11 -- every configure
REM fails with
REM   The CMAKE_C_COMPILER:
REM     C:/...(x86)/.../2022/BuildTools/VC/Tools/MSVC/14.44.35207/.../cl.exe
REM   is not a full path to an existing compiler tool.
REM and re-running never helps, because the poison is in the build dir, not in
REM the environment. Everything under the build dir is generated, so wiping is
REM always safe -- and doing it automatically is what keeps F5 alive across a
REM toolchain move instead of handing the user an unexplained "exit code 1".
set CFG_ARGS=-S . -B "%BUILD_DIR%" -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=cl -DCMAKE_CXX_COMPILER=cl -DCMAKE_MAKE_PROGRAM="%NINJA_EXE%" -DHT9045_UI=ON

echo [build_msvc_ui] Configuring (Ninja, Release, HT9045_UI=ON)...
cmake %CFG_ARGS%
if errorlevel 1 (
    echo [build_msvc_ui] Configure failed. Almost always a build dir left over
    echo [build_msvc_ui] from a toolchain that no longer exists -- wiping
    echo [build_msvc_ui] "%BUILD_DIR%" and configuring once more from scratch.
    rmdir /s /q "%REPO_ROOT%\%BUILD_DIR%"
    cmake %CFG_ARGS%
    if errorlevel 1 (
        echo [build_msvc_ui] FATAL: cmake configure failed even from scratch.
        exit /b 1
    )
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
