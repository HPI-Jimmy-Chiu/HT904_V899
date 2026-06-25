// =============================================================================
//  Public/MyProductionRecord.h  -- TMyProductionRecord forward-stub
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/Public/MyProductionRecord.h
//  Translation wave: W4 (HAL motor layer, needed by mytray.h)
//  Translator: AI(W4) 20260626
//
//  STATUS: CLASS DECLARATION ONLY.  All method bodies are deferred to W3
//  (Public utils wave) because the .cpp pulls cMyDB / handlerlog / etc.
//  TMyTray holds `TMyProductionRecord *PordRec[][]` as pointers only, so
//  a forward-declaration-level header is sufficient for W4.
// =============================================================================
#ifndef MyProductionRecordH
#define MyProductionRecordH

#include "vclcompat/vcl_compat.h"   // AnsiString

class TMyProductionRecord
{
public:
    TMyProductionRecord();
    void DeleteProductionRecord();
    void InitialRecord();

    void AddLoadRecord(int iNum, int iR, int iC, int iXPos, int iYPos,
                       AnsiString Name, AnsiString Time);    // Sam 20250211
    void AddHPRecord(int iPlate, int iR, int iC);
    void AddArmSiteRecord(int iArm, int iSite);              // Steven 20201204
    void AddTestResultRecord(int iBin, AnsiString SBin, AnsiString ErrorLog = "");
    void AddUnloadRecord(int iWhichAuto, int iNum, int iTrayRow, int iTrayCol,
                         int iXPos, int iYPos, AnsiString ascode,
                         int iXPitch, int iXPitch2, int iYPitch,
                         AnsiString TrayID);                 // Frank 20160727
    void AddIndexCycleTimeRecord(double findexCycleTime);
    void AddOrderTestRecord(int iNum);
    void AddTestTime(AnsiString asStr);
    void AddTSDTime(AnsiString asStr);
    void Add2DIDRecord(AnsiString s2DID);
    void AddDataTimeRecord(AnsiString asStr);
    void AddErrorRecord(AnsiString ErrorLog, bool bAddPosition = false,
                        int iXPos = 0, int iYPos = 0,
                        int iXPitch = 0, int iXPitch2 = 0, int iYPitch = 0);
    void AddErrorLog(AnsiString ErrorLog);                   // Steven 20200518
};

#endif // MyProductionRecordH
