# add-defender-exclusions.ps1
# Must run as Administrator (right-click -> Run as Administrator)
# Adds Windows Defender exclusions for BCB6 build paths to prevent ilink32 Access Violation

#Requires -RunAsAdministrator

$paths = @(
    "D:\HT9045\Obj",
    "D:\HT9045\EXE",
    "D:\ProgramFiles\Borland\CBuilder6\Bin"
)

$processes = @(
    "D:\ProgramFiles\Borland\CBuilder6\Bin\ilink32.exe",
    "D:\ProgramFiles\Borland\CBuilder6\Bin\bcc32.exe",
    "D:\ProgramFiles\Borland\CBuilder6\Bin\make.exe"
)

Write-Host "Adding Windows Defender folder exclusions..." -ForegroundColor Cyan
foreach ($p in $paths) {
    if (Test-Path $p) {
        Add-MpPreference -ExclusionPath $p
        Write-Host "  Added: $p" -ForegroundColor Green
    } else {
        Write-Host "  SKIP (not found): $p" -ForegroundColor Yellow
    }
}

Write-Host "`nAdding Windows Defender process exclusions..." -ForegroundColor Cyan
foreach ($proc in $processes) {
    if (Test-Path $proc) {
        Add-MpPreference -ExclusionProcess $proc
        Write-Host "  Added: $proc" -ForegroundColor Green
    } else {
        Write-Host "  SKIP (not found): $proc" -ForegroundColor Yellow
    }
}

Write-Host "`nVerifying exclusions..." -ForegroundColor Cyan
$prefs = Get-MpPreference
Write-Host "Excluded paths:"
$prefs.ExclusionPath | Where-Object { $_ -like "*HT9045*" -or $_ -like "*Borland*" } | ForEach-Object { Write-Host "  $_" -ForegroundColor Green }
Write-Host "Excluded processes:"
$prefs.ExclusionProcess | Where-Object { $_ -like "*Borland*" } | ForEach-Object { Write-Host "  $_" -ForegroundColor Green }

Write-Host "`nDone! Now retry building HT9045." -ForegroundColor Cyan
