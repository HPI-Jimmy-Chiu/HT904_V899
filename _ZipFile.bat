@echo off
chcp 65001 >nul
setlocal

set "zPath=C:\Program Files\7-Zip\7z.exe"
set "password=BVL-3766"

if "%~1"=="" (
    echo Usage: %~nx0 folder_or_file_to_zip
    pause
    exit /b 1
)

if not exist "%zPath%" (
    echo 7-Zip not found: "%zPath%"
    pause
    exit /b 1
)

set "target=%~1"
if "%target:~-1%"=="\" set "target=%target:~0,-1%"
for %%I in ("%target%") do set "archiveName=%%~nxI"

"%zPath%" a -t7z "%archiveName%.7z" "%target%" "-p%password%" -mhe=on

if errorlevel 1 (
    echo Archive failed.
    exit /b %errorlevel%
)
echo Archive completed: "%archiveName%.7z"
endlocal
exit /b 0