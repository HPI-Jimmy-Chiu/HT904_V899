param()
$ErrorActionPreference = 'Stop'

$filePath = "d:\HT9045\HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260422\AutoClean\AutoClean.cpp"
$enc = [System.Text.Encoding]::GetEncoding(950)
$raw = [System.IO.File]::ReadAllLines($filePath, $enc)
$list = New-Object System.Collections.Generic.List[string]
$list.AddRange($raw)
Write-Output "Loaded $($list.Count) lines"

# =================================================================
# S5: Replace the entire Task 3100 body with a unified bUseCKPP path
# that uses g_CKPlan, followed by the existing old paths.
#
# Strategy: Insert a new bUseCKPP block right after "case 3100:"
# that handles counting + completion check via plan slots,
# then breaks. The old branches remain as else path.
# =================================================================

# Find "case 3100:" 
$caseIdx = -1
for($i=3200; $i -le 3400; $i++){
    if($list[$i] -like '*case 3100:*'){
        $caseIdx = $i
        break
    }
}
if($caseIdx -eq -1){ Write-Output "ERROR: case 3100 not found"; exit 1 }
Write-Output "case 3100 at L$($caseIdx+1)"

# Insert new unified bUseCKPP block right after "case 3100:" line
$insertIdx = $caseIdx + 1
$newBlock = @(
'            //AI(ht9045-v899) 20260408: unified plan-driven contact count + completion check (S5)'
'            if(bUseCKPP && g_CKPlan.Found())'
'            {'
'                const TPickPlan &plan = g_CKPlan.GetPlan();'
'                bool bPickIncomplete = false;'
'                for(int i = 0; i < plan.iSlotCount; i++)'
'                {'
'                    const TPickSlot &slot = plan.Slots[i];'
'                    if(slot.iPhysical < 0) continue;'
'                    // contact counting: only for slots that have IC'
'                    if(InArmSuck.Item[iSuckRow][slot.iPhysical] == HAS_CLEAN_IC ||'
'                       InArmSuck.Item[iSuckRow][slot.iPhysical] == HAS_NULL_CLEAN_IC)'
'                    {'
'                        iKitRow = iAutoCleanPickPlateY + 1;'
'                        iKitCol = slot.iKitCol;'
'                        iContectCount = atoi(fMain->AutoCleanStringGrid->Cells[iKitCol][iKitRow].c_str());'
'                        iContectCount++;'
'                        SetAutoCleanStringGrid(iKitCol, iKitRow, AnsiString(iContectCount));'
'                    }'
'                    // completion check: only active slots must have IC'
'                    if(slot.bActive)'
'                    {'
'                        if(InArmSuck.Item[iSuckRow][slot.iPhysical] != HAS_CLEAN_IC &&'
'                           InArmSuck.Item[iSuckRow][slot.iPhysical] != HAS_NULL_CLEAN_IC)'
'                            bPickIncomplete = true;'
'                    }'
'                }'
'                if(bPickIncomplete)'
'                    Task = 10;'
''
'                if(Task != 10)'
'                {'
'                    if(USE_PICKER_COUNT == ep1Picker)'
'                        Task = 3300;'
'                    else if(bUse8Picker)'
'                        Task = 3200;'
'                    else'
'                        Task = 3300;'
'                }'
'                break;'
'            }'
)
$list.InsertRange($insertIdx, [string[]]$newBlock)
Write-Output "S5: Inserted $($newBlock.Count) lines at L$($insertIdx+1). Count=$($list.Count)"

# =================================================================
# Save Big5
# =================================================================
$sw = New-Object System.IO.StreamWriter($filePath, $false, $enc)
foreach($line in $list){ $sw.WriteLine($line) }
$sw.Close()

$final = [System.IO.File]::ReadAllLines($filePath, $enc)
Write-Output ""
Write-Output "=== S5 Complete ==="
Write-Output "Final: $($final.Count) lines (was 9448, delta=$($final.Count - 9448))"
