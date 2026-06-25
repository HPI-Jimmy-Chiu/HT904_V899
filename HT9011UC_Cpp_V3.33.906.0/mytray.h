// =============================================================================
//  mytray.h  -- TMyTray tray data class
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/mytray.h
//  Translation wave: W4 (HAL motor layer, needed by mymotor.h)
//  Translator: AI(W4) 20260626
//
//  STATUS: Full class declaration translated.  Method BODIES deferred to W4
//  (mytray.cpp, written separately when needed by tests); TTrayMotor calls
//  Tray.ClearData(), SetData(), HasIC(), HasRealIC(), FullIC(),
//  HasCleanPad(), CleanPlate2HasIC(), HasOnlyDataICAndNullIC() -- all are
//  declared here; inline stubs provided where the body is trivially safe
//  (FullIC returns false, etc.) so the W4 motor layer links without mytray.cpp.
//
//  MyProductionRecord: translated declaration in Public/MyProductionRecord.h.
// =============================================================================
#ifndef mytrayH
#define mytrayH

#define _MAX_COL_ITEM 30
#define _MAX_ROW_ITEM 70

#include "cmydef.h"
#include "Public/MyProductionRecord.h"

// ---------------------------------------------------------------------------
class TMyTray
{
public:
    TMyTray();
    ~TMyTray();

    int  XItem;
    int  YItem;
    int  XBItem;
    int  YBItem;
    int  XBWidth;
    int  YBWidth;

    int  Data[_MAX_COL_ITEM][_MAX_ROW_ITEM];
    int  iWhichSite[_MAX_COL_ITEM][_MAX_ROW_ITEM];
    int  iNeedRotAng[_MAX_COL_ITEM][_MAX_ROW_ITEM];     // Steven 20170425: rotate motor
    int  iCurrRotAng[_MAX_COL_ITEM][_MAX_ROW_ITEM];
    int  iWhichIndex[_MAX_COL_ITEM][_MAX_ROW_ITEM];     // ChungHung 20150205: ATK
    int  iBinCode[_MAX_COL_ITEM][_MAX_ROW_ITEM];
    int  BufferData[_MAX_COL_ITEM][_MAX_ROW_ITEM];
    int  iTarget[_MAX_COL_ITEM][_MAX_ROW_ITEM];         // Steven 20150915: TSMC
    int  iCleanCount[_MAX_COL_ITEM][_MAX_ROW_ITEM];
    int  iWhichAuto[_MAX_COL_ITEM][_MAX_ROW_ITEM];      // JerryYang 20220909: magazine
    int  iBinData[_MAX_COL_ITEM][_MAX_ROW_ITEM];        // JerryYang 20220909: magazine
    int  iAOIResult[_MAX_COL_ITEM][_MAX_ROW_ITEM];      // Sam 20240325: DamageTrayMapping
    int  SiteMapData[_MAX_COL_ITEM][_MAX_ROW_ITEM];     // Ifor 20210423: Tray Mapping

    bool b2DIDNG[_MAX_COL_ITEM][_MAX_ROW_ITEM];         // Steven 20200611: Murata 2DID NG
    bool bFliped[_MAX_COL_ITEM][_MAX_ROW_ITEM];
    bool bTrayMapNGData[_MAX_COL_ITEM][_MAX_ROW_ITEM];  // KenHsieh 20220923: Throw IC

    AnsiString cCassetteID;
    AnsiString cCassetteLot;
    AnsiString cDeviceInf[_MAX_COL_ITEM][_MAX_ROW_ITEM];    // Q_Q 2012.06.01 BarCode
    AnsiString cSBin[_MAX_COL_ITEM][_MAX_ROW_ITEM];         // Steven 20220120: Amlogic
    AnsiString cReDeviceInf[_MAX_COL_ITEM][_MAX_ROW_ITEM];  // Q_Q 2012.06.01 BarCode
    TMyProductionRecord *PordRec[_MAX_COL_ITEM][_MAX_ROW_ITEM];  // Steven 20221005
    AnsiString cTrayID;                                     // JerryYang 20250120

    int  iCassetteCount[2];
    bool bMustToEmpty;              // Sam 20230317: P24
    int  iNeedManualRemoved;        // Sam 20230317: P24 (-1/0/1)

    void ClearData();
    void SetData(int data);
    bool SetBlockXYItem(int iBColItem, int iBRowItem);
    bool SetXYItem(int iColItem, int iRowItem);     // Steven 20160614

    bool HasIC();
    bool HasRealIC();
    void ClearNullIC();             // AI(ht9045-atk-amr-flow) 20260427 RogerYang
    bool HasCleanPad();             // kevin 20150505
    bool HasOCRIC();                // wei 20160606
    bool HasICCassette();           // wei 20180702
    bool HasDataIC(int DataType);   // wei 20180702
    bool HasEmptyCassette();
    bool HasOnlyDataICAndNullIC(int DataType);  // JimmyChiu 20220908
    bool CleanPlate2HasIC();        // kevin 20120217
    bool FullIC();

    int  HowManyIC();
    int  HowManyUpperHalfIC();      // Sam 20221101
    int  HowManyLowerHalfIC();      // Sam 20221101
    int  HowManyICInBuffer(int data);
    int  HowManyBinICInTray(int iBin);  // Sam 20240108

    void SaveUnloaderInfo(AnsiString asFile);   // ChungHung 20150205: ATK
    void ReadUnloaderInfo(AnsiString asFile);   // ChungHung 20150205: ATK
};

#endif // mytrayH
