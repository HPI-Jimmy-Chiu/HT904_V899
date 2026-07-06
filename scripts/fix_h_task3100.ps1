param()
$ErrorActionPreference = 'Stop'

$filePath = "d:\HT9045\HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422\AutoClean\AutoClean.cpp"
$enc = [System.Text.Encoding]::GetEncoding(950)
$raw = [System.IO.File]::ReadAllLines($filePath, $enc)

$list = New-Object System.Collections.Generic.List[string]
$list.AddRange($raw)
Write-Output "Loaded $($list.Count) lines"

# Verify AxxG: L3291 has [0] and L3292 has [3]
if($list[3290] -notlike '*Item*iSuckRow*0*HAS_CLEAN_IC*' -or $list[3291] -notlike '*Item*iSuckRow*3*'){
    Write-Output "ERROR: AxxG mismatch"; Write-Output "L3291: $($list[3290])"; Write-Output "L3292: $($list[3291])"; exit 1
}
Write-Output "AxxG verified (L3291-3295)"

# Verify AxEx: L3265 has [0] and L3266 has [2]
if($list[3264] -notlike '*Item*iSuckRow*0*HAS_CLEAN_IC*' -or $list[3265] -notlike '*Item*iSuckRow*2*'){
    Write-Output "ERROR: AxEx mismatch"; Write-Output "L3265: $($list[3264])"; Write-Output "L3266: $($list[3265])"; exit 1
}
Write-Output "AxEx verified (L3265-3269)"

# Find generic 4-picker: two lines with [0] and [1]
$genIdx = -1
for($i=3300; $i -lt ($list.Count-1); $i++){
    if($list[$i] -like '*Item*iSuckRow*0*HAS_CLEAN_IC*' -and $list[$i+1] -like '*Item*iSuckRow*1*HAS_CLEAN_IC*'){
        $genIdx = $i; break
    }
}
if($genIdx -ne -1){ Write-Output "Generic 4-picker at L$($genIdx+1)" }

# ================================================================
# EDIT 1: AxxG (idx 3290, 5 lines)
# ================================================================
$list.RemoveRange(3290, 5)
$axxg = @(
'                //AI(ht9045-v899) 20260407: allow partial pick when inactive suckers have no pad (Fix H)'
'                if(bUseCKPP)'
'                {'
'                    bool bPickIncomplete=false;'
'                    for(int j=iAutoCleanStart; j<2; j++)'
'                    {'
'                        int sc=GetAutoCleanPickStep(j);'
'                        if(sc==-1) continue;'
'                        if(!bInArmSuckActive[iSuckRow][sc]) continue;'
'                        if(InArmSuck.Item[iSuckRow][sc]!=HAS_CLEAN_IC &&'
'                           InArmSuck.Item[iSuckRow][sc]!=HAS_NULL_CLEAN_IC)'
'                        {'
'                            bPickIncomplete=true;'
'                            break;'
'                        }'
'                    }'
'                    if(bPickIncomplete) Task=10;'
'                }'
'                else'
'                {'
'                    if((InArmSuck.Item[iSuckRow][0]!=HAS_CLEAN_IC && InArmSuck.Item[iSuckRow][0]!=HAS_NULL_CLEAN_IC) ||'
'                       (InArmSuck.Item[iSuckRow][3]!=HAS_CLEAN_IC && InArmSuck.Item[iSuckRow][3]!=HAS_NULL_CLEAN_IC))'
'                    {'
'                        Task=10;'
'                    }'
'                }'
)
$list.InsertRange(3290, [string[]]$axxg)
$d1 = $axxg.Count - 5
Write-Output "AxxG done (+$d1). Count=$($list.Count)"

# ================================================================
# EDIT 2: AxEx (idx 3264, 5 lines) - unaffected since before AxxG
# ================================================================
$list.RemoveRange(3264, 5)
$axex = @(
'                //AI(ht9045-v899) 20260407: allow partial pick when inactive suckers have no pad (Fix H)'
'                if(bUseCKPP)'
'                {'
'                    bool bPickIncomplete=false;'
'                    for(int j=iAutoCleanStart; j<2; j++)'
'                    {'
'                        int sc=GetAutoCleanPickStep(j);'
'                        if(sc==-1) continue;'
'                        if(!bInArmSuckActive[iSuckRow][sc]) continue;'
'                        if(InArmSuck.Item[iSuckRow][sc]!=HAS_CLEAN_IC &&'
'                           InArmSuck.Item[iSuckRow][sc]!=HAS_NULL_CLEAN_IC)'
'                        {'
'                            bPickIncomplete=true;'
'                            break;'
'                        }'
'                    }'
'                    if(bPickIncomplete) Task=10;'
'                }'
'                else'
'                {'
'                    if((InArmSuck.Item[iSuckRow][0]!=HAS_CLEAN_IC && InArmSuck.Item[iSuckRow][0]!=HAS_NULL_CLEAN_IC) ||'
'                       (InArmSuck.Item[iSuckRow][2]!=HAS_CLEAN_IC && InArmSuck.Item[iSuckRow][2]!=HAS_NULL_CLEAN_IC))'
'                    {'
'                        Task=10;'
'                    }'
'                }'
)
$list.InsertRange(3264, [string[]]$axex)
$d2 = $axex.Count - 5
Write-Output "AxEx done (+$d2). Count=$($list.Count)"

# ================================================================
# EDIT 3: Generic 4-picker (7 lines) - shift by d1+d2
# ================================================================
if($genIdx -ne -1){
    $gs = $genIdx + $d1 + $d2
    if($list[$gs] -like '*Item*iSuckRow*0*HAS_CLEAN_IC*' -and $list[$gs+1] -like '*Item*iSuckRow*1*HAS_CLEAN_IC*'){
        Write-Output "Generic verified at shifted L$($gs+1)"
        $list.RemoveRange($gs, 7)
        $gen = @(
'                //AI(ht9045-v899) 20260407: allow partial pick when inactive suckers have no pad (Fix H)'
'                if(bUseCKPP)'
'                {'
'                    bool bPickIncomplete=false;'
'                    for(int j=iAutoCleanStart; j<4; j++)'
'                    {'
'                        int sc=GetAutoCleanPickStep(j);'
'                        if(sc==-1) continue;'
'                        if(!bInArmSuckActive[iSuckRow][sc]) continue;'
'                        if(InArmSuck.Item[iSuckRow][sc]!=HAS_CLEAN_IC &&'
'                           InArmSuck.Item[iSuckRow][sc]!=HAS_NULL_CLEAN_IC)'
'                        {'
'                            bPickIncomplete=true;'
'                            break;'
'                        }'
'                    }'
'                    if(bPickIncomplete) Task=10;'
'                }'
'                else'
'                {'
'                    if((InArmSuck.Item[iSuckRow][0]!=HAS_CLEAN_IC && InArmSuck.Item[iSuckRow][0]!=HAS_NULL_CLEAN_IC) ||'
'                       (InArmSuck.Item[iSuckRow][1]!=HAS_CLEAN_IC && InArmSuck.Item[iSuckRow][1]!=HAS_NULL_CLEAN_IC) ||'
'                       (InArmSuck.Item[iSuckRow][2]!=HAS_CLEAN_IC && InArmSuck.Item[iSuckRow][2]!=HAS_NULL_CLEAN_IC) ||'
'                       (InArmSuck.Item[iSuckRow][3]!=HAS_CLEAN_IC && InArmSuck.Item[iSuckRow][3]!=HAS_NULL_CLEAN_IC)  )'
'                    {'
'                        Task=10;'
'                    }'
'                }'
        )
        $list.InsertRange($gs, [string[]]$gen)
        Write-Output "Generic done (+$($gen.Count - 7)). Count=$($list.Count)"
    } else {
        Write-Output "WARNING: Generic shifted verification failed"
    }
}

# ================================================================
# Save Big5 no BOM
# ================================================================
$sw = New-Object System.IO.StreamWriter($filePath, $false, $enc)
foreach($line in $list){ $sw.WriteLine($line) }
$sw.Close()

$final = [System.IO.File]::ReadAllLines($filePath, $enc)
Write-Output ""
Write-Output "=== Fix H Complete ==="
Write-Output "Final: $($final.Count) lines (was 9319, delta=$($final.Count - 9319))"
