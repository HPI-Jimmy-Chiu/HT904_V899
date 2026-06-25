// =============================================================================
//  mytray.cpp  -- TMyTray method bodies (PARTIAL W4 stub)
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/mytray.cpp
//  Translation wave: W4 (HAL motor layer prerequisite)
//  Translator: AI(W4) 20260626
//
//  Only the methods called by TTrayMotor in mymotor.cpp are implemented here.
//  Remaining methods (SaveUnloaderInfo, ReadUnloaderInfo, HowManyIC, etc.)
//  are stubs -- bodies deferred to W3/W4-part2 when needed by higher layers.
// =============================================================================
#include "MachineDefine.h"
#include "mytray.h"
#include <string.h>     // memset

// ---------------------------------------------------------------------------
TMyTray::TMyTray()
{
    XItem  = 1;
    YItem  = 1;
    XBItem = 1;
    YBItem = 1;
    XBWidth = 1;
    YBWidth = 1;
    bMustToEmpty     = false;
    iNeedManualRemoved = -1;
    iCassetteCount[0] = 0;
    iCassetteCount[1] = 0;
    ClearData();
}

// ---------------------------------------------------------------------------
TMyTray::~TMyTray()
{
}

// ---------------------------------------------------------------------------
void TMyTray::ClearData()
{
    for (int i = 0; i < _MAX_COL_ITEM; i++)
    {
        for (int j = 0; j < _MAX_ROW_ITEM; j++)
        {
            Data[i][j]        = NULL_IC;
            iWhichSite[i][j]  = 0;
            iNeedRotAng[i][j] = 0;
            iCurrRotAng[i][j] = 0;
            iWhichIndex[i][j] = 0;
            iBinCode[i][j]    = 0;
            BufferData[i][j]  = NULL_IC;
            iTarget[i][j]     = 0;
            iCleanCount[i][j] = 0;
            iWhichAuto[i][j]  = 0;
            iBinData[i][j]    = 0;
            iAOIResult[i][j]  = 0;
            SiteMapData[i][j] = 0;
            b2DIDNG[i][j]     = false;
            bFliped[i][j]     = false;
            bTrayMapNGData[i][j] = false;
            cDeviceInf[i][j]  = "";
            cSBin[i][j]       = "";
            cReDeviceInf[i][j] = "";
            PordRec[i][j]     = NULL;
        }
    }
    cCassetteID  = "";
    cCassetteLot = "";
    cTrayID      = "";
}

// ---------------------------------------------------------------------------
void TMyTray::SetData(int data)
{
    for (int i = 0; i < XItem; i++)
        for (int j = 0; j < YItem; j++)
            Data[i][j] = data;
}

// ---------------------------------------------------------------------------
bool TMyTray::SetXYItem(int iColItem, int iRowItem)    // Steven 20160614
{
    if (iColItem > _MAX_COL_ITEM || iRowItem > _MAX_ROW_ITEM)
        return false;
    XItem = iColItem;
    YItem = iRowItem;
    return true;
}

// ---------------------------------------------------------------------------
bool TMyTray::SetBlockXYItem(int iBColItem, int iBRowItem)
{
    XBItem = iBColItem;
    YBItem = iBRowItem;
    return true;
}

// ---------------------------------------------------------------------------
bool TMyTray::HasIC()
{
    for (int i = 0; i < XItem; i++)
        for (int j = 0; j < YItem; j++)
            if (Data[i][j] == HAS_IC || Data[i][j] == HAS_NULL_IC)
                return true;
    return false;
}

// ---------------------------------------------------------------------------
bool TMyTray::HasRealIC()
{
    for (int i = 0; i < XItem; i++)
        for (int j = 0; j < YItem; j++)
            if (Data[i][j] == HAS_IC)
                return true;
    return false;
}

// ---------------------------------------------------------------------------
bool TMyTray::FullIC()
{
    for (int i = 0; i < XItem; i++)
        for (int j = 0; j < YItem; j++)
            if (Data[i][j] == NULL_IC)
                return false;
    return true;
}

// ---------------------------------------------------------------------------
bool TMyTray::HasCleanPad()     // kevin 20150505
{
    // Stub -- real check reads pad-specific data; TODO(W4-part2)
    return false;
}

// ---------------------------------------------------------------------------
bool TMyTray::CleanPlate2HasIC()    // kevin 20120217
{
    // Stub -- TODO(W4-part2)
    return false;
}

// ---------------------------------------------------------------------------
bool TMyTray::HasOnlyDataICAndNullIC(int DataType)  // JimmyChiu 20220908
{
    for (int i = 0; i < XItem; i++)
        for (int j = 0; j < YItem; j++)
        {
            int d = Data[i][j];
            if (d != NULL_IC && d != DataType)
                return false;
        }
    return true;
}

// ---------------------------------------------------------------------------
void TMyTray::ClearNullIC()         // RogerYang 20260427
{
    for (int i = 0; i < XItem; i++)
        for (int j = 0; j < YItem; j++)
            if (Data[i][j] == HAS_NULL_IC)
                Data[i][j] = NULL_IC;
}

// ---------------------------------------------------------------------------
//  Remaining method stubs (bodies deferred to W3/W4-part2)
// ---------------------------------------------------------------------------
bool TMyTray::HasOCRIC()            { return false; }
bool TMyTray::HasICCassette()       { return false; }
bool TMyTray::HasDataIC(int)        { return false; }
bool TMyTray::HasEmptyCassette()    { return false; }
int  TMyTray::HowManyIC()           { return 0; }
int  TMyTray::HowManyUpperHalfIC()  { return 0; }
int  TMyTray::HowManyLowerHalfIC()  { return 0; }
int  TMyTray::HowManyICInBuffer(int){ return 0; }
int  TMyTray::HowManyBinICInTray(int){ return 0; }
void TMyTray::SaveUnloaderInfo(AnsiString) {}
void TMyTray::ReadUnloaderInfo(AnsiString) {}
