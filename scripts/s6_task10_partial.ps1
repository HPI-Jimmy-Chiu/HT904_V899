param()
$ErrorActionPreference = 'Stop'

$filePath = "d:\HT9045\HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260330\AutoClean\AutoClean.cpp"
$enc = [System.Text.Encoding]::GetEncoding(950)
$raw = [System.IO.File]::ReadAllLines($filePath, $enc)
$list = New-Object System.Collections.Generic.List[string]
$list.AddRange($raw)
Write-Output "Loaded $($list.Count) lines"

# =================================================================
# S6: Fix Task 10 — when TrayHasCleanIC()==false and InArmSuck.HasIC(),
# if bUseCKPP, skip the 3000→3100 loop and go directly to 3300 (finish).
#
# The arm already has clean pads from a previous pick — kit is empty.
# Going to 3000→3100 would just check counting and then if any active
# slot is missing IC, loop back to 10 forever.
# =================================================================

# Find "if(InArmSuck.HasIC())" in Task 10 else branch
# We know it's in the TrayHasCleanIC()==false else block
$hitIdx = -1
for($i=2960; $i -le 2985; $i++){
    if($list[$i] -like '*InArmSuck.HasIC()*' -and $list[$i] -like '*//已被*'){
        $hitIdx = $i
        break
    }
}
# Fallback: search for line with "InArmSuck.HasIC()" near Task=3000
if($hitIdx -eq -1){
    for($i=2960; $i -le 2985; $i++){
        if($list[$i] -like '*InArmSuck.HasIC()*'){
            $hitIdx = $i
            break
        }
    }
}
if($hitIdx -eq -1){ Write-Output "ERROR: Cannot find HasIC check in Task 10"; exit 1 }
Write-Output "Found HasIC at L$($hitIdx+1)"

# Verify the next lines have Task=3000
$checkLine = $list[$hitIdx + 2]
if($checkLine -notlike '*Task=3000*'){
    Write-Output "ERROR: Expected Task=3000 near HasIC, got: $checkLine"
    exit 1
}
Write-Output "Verified Task=3000 at L$($hitIdx+3)"

# Replace the block:
#   if(InArmSuck.HasIC())         //已被In arm吸起來
#   {
#       Task=3000;
#   }
# With:
#   if(InArmSuck.HasIC())
#   {
#       if(bUseCKPP) { Task=3300; } else { Task=3000; }
#   }

$list.RemoveRange($hitIdx, 4)
$newLines = @(
'                if(InArmSuck.HasIC())'
'                {'
'                    //AI(ht9045-v899) 20260408: kit empty but arm has partial pick - skip 3000-3100 loop, go finish (S6)'
'                    if(bUseCKPP)'
'                    {'
'                        PlaceToCleanList->AddHPSuckGroup();'
'                        Task=3300;'
'                    }'
'                    else'
'                    {'
'                        Task=3000;'
'                    }'
'                }'
)
$list.InsertRange($hitIdx, [string[]]$newLines)
$delta = $newLines.Count - 4
Write-Output "S6: Replaced 4 lines with $($newLines.Count) lines (delta=$delta). Count=$($list.Count)"

# =================================================================
# Save Big5
# =================================================================
$sw = New-Object System.IO.StreamWriter($filePath, $false, $enc)
foreach($line in $list){ $sw.WriteLine($line) }
$sw.Close()

$final = [System.IO.File]::ReadAllLines($filePath, $enc)
Write-Output ""
Write-Output "=== S6 Complete ==="
Write-Output "Final: $($final.Count) lines (was 9489, delta=$($final.Count - 9489))"
