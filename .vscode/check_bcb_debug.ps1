# BCB6 Debug Flag guard
# Purpose: ensure HT9045.bpr stays in Full Debug before BCB6 build,
# so breakpoints in the IDE always work.
# Checks:
#   1. CFLAG1 must contain -Od -v -y -k
#   2. USERDEFINES must contain _DEBUG and must NOT contain NDEBUG
#   3. LinkDebugVcl must be 1
param(
    [string]$BprPath = "$PSScriptRoot\..\HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422\HT9045.bpr"
)

$ErrorActionPreference = 'Stop'

if (-not (Test-Path $BprPath)) {
    Write-Host "[BCB Debug Check] bpr not found: $BprPath" -ForegroundColor Red
    exit 1
}

$content = Get-Content $BprPath -Raw -Encoding UTF8
$problems = @()

$cflagMatch = [regex]::Match($content, '<CFLAG1\s+value="([^"]*)"')
if (-not $cflagMatch.Success) {
    $problems += "CFLAG1 field not found"
} else {
    $cflag = $cflagMatch.Groups[1].Value
    foreach ($flag in @('-Od', '-v', '-y', '-k')) {
        $pattern = [regex]::Escape($flag) + '(\s|$)'
        if ($cflag -notmatch $pattern) {
            $problems += "CFLAG1 missing $flag (current: $cflag)"
        }
    }
    if ($cflag -match '\s-O[12sx]\b') {
        $problems += "CFLAG1 has optimization flag (-O1/-O2/-Os/-Ox), breakpoints will fail"
    }
}

$udMatch = [regex]::Match($content, '<USERDEFINES\s+value="([^"]*)"')
if ($udMatch.Success) {
    $ud = $udMatch.Groups[1].Value
    if ($ud -notmatch '_DEBUG') { $problems += "USERDEFINES missing _DEBUG (current: $ud)" }
    if ($ud -match 'NDEBUG')    { $problems += "USERDEFINES contains NDEBUG (current: $ud)" }
}

if ($content -notmatch 'LinkDebugVcl=1') {
    $problems += "LinkDebugVcl is not 1 (cannot step into VCL source)"
}

if ($problems.Count -gt 0) {
    Write-Host ""
    Write-Host "===============================================================" -ForegroundColor Red
    Write-Host " [FAIL] BCB6 Debug flags lost. Build is BLOCKED." -ForegroundColor Red
    Write-Host "        Breakpoints will NOT hit if you proceed." -ForegroundColor Red
    Write-Host "===============================================================" -ForegroundColor Red
    foreach ($p in $problems) { Write-Host "  - $p" -ForegroundColor Yellow }
    Write-Host ""
    Write-Host "How to fix:" -ForegroundColor Cyan
    Write-Host "  1. Open HT9045.bpr in BCB6 IDE"
    Write-Host "  2. Project -> Options -> Compiler -> click [Full debug]"
    Write-Host "  3. Click OK to save"
    Write-Host "  4. Re-run this task"
    Write-Host ""
    exit 2
}

Write-Host "[BCB Debug Check] HT9045.bpr is Full Debug. OK." -ForegroundColor Green
exit 0
