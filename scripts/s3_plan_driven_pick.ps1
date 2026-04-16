param()
$ErrorActionPreference = 'Stop'

$filePath = "d:\HT9045\HT9011UC_Code_V3.33.899.0_20260323_Jimmy_20260330\AutoClean\AutoClean.cpp"
$enc = [System.Text.Encoding]::GetEncoding(950)
$raw = [System.IO.File]::ReadAllLines($filePath, $enc)
$list = New-Object System.Collections.Generic.List[string]
$list.AddRange($raw)
Write-Output "Loaded $($list.Count) lines"

# =================================================================
# EDIT 1: Add g_CKPlan global after bUseCKPP definition (L1650)
# Insert after line index 1649 (0-based)
# =================================================================
$target1 = $list[1649]
if($target1 -notlike '*bUseCKPP*true*'){
    Write-Output "ERROR: Expected bUseCKPP at L1650, got: $target1"
    exit 1
}
$list.Insert(1650, "//AI(ht9045-v899) 20260408: global plan instance shared across pick/move/check functions")
$list.Insert(1651, "cCleanKitPickPlan g_CKPlan;")
Write-Output "EDIT 1: Added g_CKPlan at L1651-1652. Count=$($list.Count)"

# =================================================================
# EDIT 2: Replace local plan with g_CKPlan in SearchCleanKitUpDown
# Original was at L2045 (cCleanKitPickPlan plan;), now shifted +2
# =================================================================
$shift1 = 2
$idx2 = 2044 + $shift1  # 0-based for original L2045
if($list[$idx2] -notlike '*cCleanKitPickPlan plan*'){
    Write-Output "ERROR: Expected local plan at shifted idx $idx2, got: $($list[$idx2])"
    # Try to find it
    for($i=2040;$i -le 2060;$i++){
        if($list[$i] -like '*cCleanKitPickPlan plan*'){
            Write-Output "Found at idx ${i}: $($list[$i])"
            $idx2 = $i
            break
        }
    }
}

# Replace 10 lines (L2045-2054 original, now shifted by $shift1)
# From "cCleanKitPickPlan plan;" to end of RecordProcess
$blockStart = $idx2
$blockEnd = $idx2
for($i=$idx2; $i -le ($idx2+15); $i++){
    if($list[$i] -like '*return;*'){
        $blockEnd = $i
        break
    }
}
Write-Output "EDIT 2: Replacing lines $($blockStart+1)-$($blockEnd+1)"
$removeCount = $blockEnd - $blockStart + 1
$list.RemoveRange($blockStart, $removeCount)

$newLines2 = @(
'        g_CKPlan.Init((int)iSht, iRow);'
'        if(g_CKPlan.Search())'
'        {'
'            g_CKPlan.Apply();'
'            RecordProcess(AnsiString().sprintf('
'                "AC_SEARCH [CKPP] iAutoCleanStart=%d iAutoCleanPickPlateX=%d Y=%d active=%d/%d XItem=%d",'
'                iAutoCleanStart, iAutoCleanPickPlateX,'
'                iAutoCleanPickPlateY, g_CKPlan.GetActiveCount(), g_CKPlan.GetSlotCount(), MOT[MMAutoCleanKit].Tray.XItem));'
'        }'
'        return;'
)
$list.InsertRange($blockStart, [string[]]$newLines2)
$shift2 = $newLines2.Count - $removeCount
Write-Output "EDIT 2: Done (delta=$shift2). Count=$($list.Count)"

# =================================================================
# EDIT 3: Add bUseCKPP path in PickFromCleanKit
# Original L2714 (for loop start): for(int j=0; j<4; j++)
# Now shifted by $shift1 + $shift2
# =================================================================
$totalShift = $shift1 + $shift2
# Find PickFromCleanKit function
$pickFuncIdx = -1
for($i=2680+$totalShift; $i -le 2710+$totalShift; $i++){
    if($list[$i] -like '*bool PickFromCleanKit*'){
        $pickFuncIdx = $i
        break
    }
}
if($pickFuncIdx -eq -1){
    Write-Output "ERROR: Cannot find PickFromCleanKit"
    exit 1
}
Write-Output "Found PickFromCleanKit at L$($pickFuncIdx+1)"

# Find the for(int j=0; j<4; j++) loop inside
$forLoopIdx = -1
for($i=$pickFuncIdx; $i -le ($pickFuncIdx+30); $i++){
    if($list[$i] -like '*for(int j=0; j<4; j++)*'){
        $forLoopIdx = $i
        break
    }
}
if($forLoopIdx -eq -1){
    Write-Output "ERROR: Cannot find for loop in PickFromCleanKit"
    exit 1
}
Write-Output "Found for-loop at L$($forLoopIdx+1)"

# Find the return statement at the end (return flag1;)
$returnIdx = -1
for($i=$forLoopIdx; $i -le ($forLoopIdx+100); $i++){
    if($list[$i] -like '*return flag1;*'){
        $returnIdx = $i
        break
    }
}
if($returnIdx -eq -1){
    Write-Output "ERROR: Cannot find return flag1"
    exit 1
}
Write-Output "Found return at L$($returnIdx+1)"

# Insert bUseCKPP block BEFORE the for loop (at $forLoopIdx)
$newPickBlock = @(
'    //AI(ht9045-v899) 20260408: plan-driven pick - iterate plan slots instead of hardcoded j=0..3'
'    if(bUseCKPP && g_CKPlan.Found())'
'    {'
'        const TPickPlan &plan = g_CKPlan.GetPlan();'
'        for(int i = 0; i < plan.iSlotCount; i++)'
'        {'
'            const TPickSlot &slot = plan.Slots[i];'
'            if(slot.iPhysical < 0) continue;'
'            iSuckCol = slot.iPhysical;'
'            iKitCol  = slot.iKitCol;'
'            iKitRow  = plan.iKitRow;'
''
'            if(iKitCol < 0 || iKitRow < 0 || iKitCol >= 50 || iKitRow >= 50)'
'                continue;'
''
'            if(InArmSuck.Suck[iSuckRow][iSuckCol].Error == true)'
'            {   // error already flagged, skip'
'            }'
'            else if(slot.bActive && InArmSuck.Item[iSuckRow][iSuckCol] == NULL_IC)'
'            {'
'                if(InArmSuck.Suck[iSuckRow][iSuckCol].Suck())'
'                {'
'                    if(Special_2X6_Tray_XItem7() && (iRowKit==3 || iRowKit==4))'
'                    {'
'                        if(iKitCol >= iAutoCleanUseXPitch)'
'                            iKitCol = iKitCol - iAutoCleanUseXPitch;'
'                    }'
'                    InspectInArmPosition(MMAutoCleanKit, iSuckRow, iSuckCol, iKitRow, iKitCol, true);'
'                    PlaceToCleanList->SetArrPlateXY(iSuckRow, iSuckCol, 0, iKitRow, iKitCol, HAS_CLEAN_IC);'
'                    bInArmSuckActive[iSuckRow][iSuckCol] = false;'
'                    MOT[MMAutoCleanKit].SetTraySingleData(iKitCol, iKitRow, HAS_NULL_CLEAN_IC);'
'                    sTime.sprintf("%02d:%02d:%02d.%03d", SystemHour, SystemMin, SystemSec, SystemMSec);'
'                    CleanKitRecord[iKitRow][iKitCol] = sTime;'
'                    bCleanKitSuckDuplicateErr[iSuckRow][iSuckCol] = false;'
'                    InArmSuck.SetItemData(iSuckRow, iSuckCol, HAS_CLEAN_IC);'
'                    InArmSuck.iAutoCleanRecX[iSuckRow][iSuckCol] = iKitCol;'
'                    InArmSuck.iAutoCleanRecY[iSuckRow][iSuckCol] = iKitRow;'
'                    InArmSuck.PordRec[iSuckRow][iSuckCol].AddPickCleanPad(iSuckRow, iSuckCol, iKitRow, iKitCol, HAS_CLEAN_IC);'
'                }'
'                else if(InArmSuck.Suck[iSuckRow][iSuckCol].Error == false)'
'                {   flag1 = false; }'
'            }'
'            else if(!slot.bActive && InArmSuck.Item[iSuckRow][iSuckCol] == NULL_IC)'
'            {'
'                // inactive slot: mark phantom (pad consumed but not picked)'
'                if(slot.bHasPad && iKitCol < MOT[MMAutoCleanKit].Tray.XItem)'
'                {'
'                    if(Special_2X6_Tray_XItem7() && (iRowKit==3 || iRowKit==4))'
'                        iKitCol = iKitCol + iAutoCleanUseXPitch * 3;'
'                    PlaceToCleanList->SetArrPlateXY(iSuckRow, iSuckCol, 0, iKitRow, iKitCol, -1);'
'                    MOT[MMAutoCleanKit].SetTraySingleData(iKitCol, iKitRow, HAS_NULL_CLEAN_IC);'
'                    sTime.sprintf("%02d:%02d:%02d.%03d", SystemHour, SystemMin, SystemSec, SystemMSec);'
'                    CleanKitRecord[iKitRow][iKitCol] = sTime;'
'                    bCleanKitSuckDuplicateErr[iSuckRow][iSuckCol] = false;'
'                    InArmSuck.SetItemData(iSuckRow, iSuckCol, HAS_NULL_CLEAN_IC);'
'                    InArmSuck.iAutoCleanRecX[iSuckRow][iSuckCol] = iKitCol;'
'                    InArmSuck.iAutoCleanRecY[iSuckRow][iSuckCol] = iKitRow;'
'                    InArmSuck.PordRec[iSuckRow][iSuckCol].AddPickCleanPad(iSuckRow, iSuckCol, iKitCol, iKitRow, HAS_NULL_CLEAN_IC);'
'                }'
'            }'
'        }'
'        return flag1;'
'    }'
''
'    // ========== OLD PATH: original PickFromCleanKit =========='
)
$list.InsertRange($forLoopIdx, [string[]]$newPickBlock)
$shift3 = $newPickBlock.Count
Write-Output "EDIT 3: Inserted $shift3 lines at L$($forLoopIdx+1). Count=$($list.Count)"

# =================================================================
# Save Big5
# =================================================================
$sw = New-Object System.IO.StreamWriter($filePath, $false, $enc)
foreach($line in $list){ $sw.WriteLine($line) }
$sw.Close()

$final = [System.IO.File]::ReadAllLines($filePath, $enc)
Write-Output ""
Write-Output "=== S3 Edit Complete ==="
Write-Output "Final: $($final.Count) lines (was 9382, delta=$($final.Count - 9382))"
