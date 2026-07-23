// MainCalcCore.cpp
// Standard C++ translation of 6 pure calc-core functions from BCB6 main.cpp
// (V3.33.906.0_20260618) -- CALC CORE ONLY.
// Source of truth: HT9011UC_Code_V3.33.906.0_20260618/main.cpp
//
// AI(ht9045-v899) 20260722: new file. See MainCalcCore.h for full scope rationale
// (extract-calc-core, same convention as cContact.h/.cpp; the rest of main.cpp/.h/.dfm
// is OUT OF SCOPE this wave).
//
// Toolchain: MinGW g++ 6.3+, C++14 or later.

#include "MainCalcCore.h"

// ---------------------------------------------------------------------------
// ComputeShtModeFlag
//   BCB6 source: main.cpp:34423-34442 (TfMain::GetShtModeFlag)
// ---------------------------------------------------------------------------
int ComputeShtModeFlag(int iShuttleMode, int iShuttle_Sel)
{
    // BCB6 :34425
    int iFlag = 0;

    // BCB6 :34426-34440
    if (iShuttleMode == 1)
    {
        if (iShuttle_Sel == 1)
        {
            iFlag = 1;
        }
        else
        {
            iFlag = 0;
        }
    }
    else
    {
        iFlag = 2;
    }
    return iFlag;
}

// ---------------------------------------------------------------------------
// ComputeCanChangeRealDummy
//   BCB6 source: main.cpp:12374-12380 (TfMain::CheckCanChangeRealDummy)
// ---------------------------------------------------------------------------
bool ComputeCanChangeRealDummy(bool bPlate1HasIC,
                                bool bPlate2HasIC,
                                bool bShuttleHasIC,
                                bool bIndexHasIC,
                                bool bInArmSuckHasIC,
                                bool bOutArmSuckHasIC)
{
    // BCB6 :12376  kevin 20120501  Autoclean
    if (bPlate1HasIC || bPlate2HasIC) return false;
    // BCB6 :12377
    if (bShuttleHasIC || bIndexHasIC) return false;
    // BCB6 :12378
    if (bInArmSuckHasIC || bOutArmSuckHasIC) return false;
    return true;
}

// ---------------------------------------------------------------------------
// ComputeSiteMapIsStander
//   BCB6 source: main.cpp:23526-23543 (TfMain::CheckSiteMapIsStander)
// ---------------------------------------------------------------------------
bool ComputeSiteMapIsStander(int iShtRow,
                              int iShtCol,
                              const int iSiteMap[][MAX_SOCKET_COL])
{
    // BCB6 :23528-23529
    int count = 1;
    int iRowCount = iShtRow;

    // BCB6 :23531-23541
    for (int iR = 0; iR < iShtRow; iR++)
    {
        count = iR + 1;   // 每一個層列的第一個數值 [0][0]=1 [1][0]=2 [2][0]=3 [3][0]=4
        for (int iC = 0; iC < iShtCol; iC++)
        {
            if (iSiteMap[iR][iC] != count &&
                iSiteMap[iR][iC] != 0)                 // Ifor 20251002 add: 避免site誤判
                return false;
            count = count + iRowCount;
        }
    }
    return true;
}

// ---------------------------------------------------------------------------
// ComputeSiteMapPriority
//   BCB6 source: main.cpp:31187-31208 (TfMain::CheckSiteMapPriority)
//
//   NOTE: iMax is computed (running max over iAutoSiteMap) exactly as golden does, but --
//   same as golden -- it is never read again after the loop.  See header comment: this is
//   preserved dead tracking, not "cleaned up", per this wave's faithful-translation mandate.
// ---------------------------------------------------------------------------
bool ComputeSiteMapPriority(const int iAutoSiteMap[][MAX_SOCKET_COL],
                             int iSiteTotal)
{
    // BCB6 :31189-31191  kevin 20180302 32->33
    int iDouble[33];
    int iMax = 0;
    for (int k = 0; k < 33; k++) iDouble[k] = 0;   // BCB6 ZeroMemory(iDouble, sizeof(iDouble))

    // BCB6 :31192-31200
    for (int i = 0; i < MAX_SOCKET_ROW; i++)
    {
        for (int j = 0; j < MAX_SOCKET_COL; j++)
        {
            if (iMax <= iAutoSiteMap[i][j])
                iMax = iAutoSiteMap[i][j];
            iDouble[iAutoSiteMap[i][j]]++;
        }
    }

    // BCB6 :31202-31206
    for (int i = 1; i <= iSiteTotal; i++)
    {
        if (iDouble[i] >= 2)
            return false;
    }
    return true;
}

// ---------------------------------------------------------------------------
// ComputeCanChangeSite
//   BCB6 source: main.cpp:14350-14393 (TfMain::CanChangeSite)
// ---------------------------------------------------------------------------
bool ComputeCanChangeSite(bool bNoIncludeHotplate,
                           bool bCanAutoCloseSite,
                           bool bI28_OnOffSiteOnTheFly,
                           bool bInArmSuckHasIC,
                           bool bInputShuttleHasIC,
                           bool bIndexHasIC,
                           bool bPlate1HasIC,
                           bool bPlate2HasIC,
                           bool bPickFromLoader,
                           int  iPickFromLoadStageTask)
{
    // BCB6 :14352  Steven 20200420 : 獨立Auto Site Off
    if (bCanAutoCloseSite)
    {
        // BCB6 :14354  kevin 20211103 add I28
        if (bI28_OnOffSiteOnTheFly == false)
        {
            if (bInArmSuckHasIC ||
                bInputShuttleHasIC ||
                bIndexHasIC)
                return false;
        }
    }
    else
    {
        // BCB6 :14364  kevin 20211103 add I28
        if (bI28_OnOffSiteOnTheFly == false)
        {
            // BCB6 :14366  ChungHung 20120713 add OneCycle Can Enable Site
            if (bNoIncludeHotplate)
            {
                if (bInArmSuckHasIC ||
                    bInputShuttleHasIC ||
                    bIndexHasIC)
                    return false;
            }
            else
            {
                if (bPlate1HasIC ||
                    bPlate2HasIC ||
                    bInArmSuckHasIC ||
                    bInputShuttleHasIC ||
                    bIndexHasIC)
                    return false;
            }
        }
        else
        {
            if (bPickFromLoader == true ||
                iPickFromLoadStageTask != 1)
            {
                return false;
            }
        }
    }
    return true;
}

// ---------------------------------------------------------------------------
// ComputeCanChangeToSocket
//   BCB6 source: main.cpp:30156-30163 (TfMain::CanChangeToSocket)
// ---------------------------------------------------------------------------
bool ComputeCanChangeToSocket(bool bSystemStart,
                               bool bCanAutoCloseSite,
                               bool bI28_OnOffSiteOnTheFly,
                               bool bInArmSuckHasIC,
                               bool bInputShuttleHasIC,
                               bool bIndexHasIC,
                               bool bPlate1HasIC,
                               bool bPlate2HasIC,
                               bool bPickFromLoader,
                               int  iPickFromLoadStageTask)
{
    // BCB6 :30158
    if (bSystemStart)
        return false;
    // BCB6 :30160  CanChangeSite() called with golden default bNoIncludeHotplate==false
    if (ComputeCanChangeSite(false,
                             bCanAutoCloseSite,
                             bI28_OnOffSiteOnTheFly,
                             bInArmSuckHasIC,
                             bInputShuttleHasIC,
                             bIndexHasIC,
                             bPlate1HasIC,
                             bPlate2HasIC,
                             bPickFromLoader,
                             iPickFromLoadStageTask) == false)
        return false;
    return true;
}
