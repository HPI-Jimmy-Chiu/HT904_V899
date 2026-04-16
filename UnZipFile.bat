@echo off
chcp 65001  // 设置为UTF-8编码
setlocal

set zPath=C:\Program Files\7-Zip\7z.exe
set password=BVL-3766

if "%~1"=="" (
    echo 請將要解壓縮的壓縮檔拖曳到此批次檔上
    pause
    goto :eof
)

"%zPath%" x "%~1" -o"%~dp1" -p%password%

if %errorlevel% equ 0 (
    echo 解壓縮成功
) else (
    echo 解壓縮失敗
)
endlocal
