//---------------------------------------------------------------------------

#ifndef MyProductionRecordH
#define MyProductionRecordH
//---------------------------------------------------------------------------

//------------------------------------------------------------------------------
//Steven 20160113 : 記錄每一顆IC 狀態
//------------------------------------------------------------------------------
class TMyProductionRecord
{
  public:
    TMyProductionRecord();
    void DeleteProductionRecord();
    void InitialRecord();

    void AddLoadRecord(int iNum, int iR, int iC, int iXPos, int iYPos, AnsiString Name, AnsiString Time);   //Sam 20250211 : Add Load Pos
    void AddHPRecord(int iPlate, int iR, int iC);
//    void AddTestRecord(int iArm, int iSite);
    void AddArmSiteRecord(int iArm, int iSite);                                 //Steven 20201204 : 修正Site No在非標準Pitch對應不上的問題
    void AddTestResultRecord(int iBin, AnsiString SBin, AnsiString ErrorLog="");
    void AddUnloadRecord(int iWhichAuto, int iNum, int iTrayRow, int iTrayCol, int iXPos, int iYPos, AnsiString ascode, int iXPitch, int iXPitch2, int iYPitch, AnsiString TrayID);   //Frank 20160727 add iXpos iYpos
    void AddIndexCycleTimeRecord(double findexCycleTime);
    void AddOrderTestRecord(int iNum);
    void AddTestTime(AnsiString asStr);
    void AddTSDTime(AnsiString asStr);
    void Add2DIDRecord(AnsiString s2DID);
    void AddDataTimeRecord(AnsiString asStr);
    void AddErrorRecord(AnsiString ErrorLog, bool bAddPosition=false, int iXPos=0, int iYPos=0, int iXPitch=0, int iXPitch2=0, int iYPitch=0);
    void AddErrorLog(AnsiString ErrorLog);                                      //Steven 20200518 : 把多個ErrorLog接起來
    void AddErrorRecordNoSave(AnsiString ErrorLog, bool bAddPosition=false, int iXPos=0, int iYPos=0, int iXPitch=0, int iXPitch2=0, int iYPitch=0);    //Steven 20190110 : 有Error先記錄,不存檔,要記錄是不是有做Retry
    void AddInArmLoaderPickRecord(int iSuckRow, int iSuckCol);                  //Sam 20200623 : Add record
    void AddIndexPickShuttleRecord(int iSite, int iSiteRow, int iSiteCol);      //Sam 20201216 : Add Site Row/Col //Sam 20200623 : Add record
    void AddIndexPlaceShuttleRecord(int iSite, int iSiteRow, int iSiteCol);     //Sam 20201216 : Add Site Row/Col //Sam 20200623 : Add record
    void AddOutShuttleDetectRecord(int iSite, int iSiteRow, int iSiteCol);      //Sam 20201216 : Add Site Row/Col //Sam 20200623 : Add record
    void AddOutArmShuttlePickRecord(int iSuckRow, int iSuckCol);                //Sam 20200623 : Add record
    void AddInArmHotplatePickRecord(int iSuckRow, int iSuckCol);                //Sam 20200716 : Add record
    void AddTestModeRecord(int iTestMode);                                      //Sam 20201216 : Add record
    void AddDataTimeEOTRecord(AnsiString asStr);                                //wei 20181211 更換位置SOT
    void AddGroundRecord(AnsiString sGround);                                   //Sam 20211223 : 每顆 IC 測試完畢都要記錄當時的 Ground & ESD 數值。
    void AddESDRecord(AnsiString sESD);                                         //Sam 20211223 : 每顆 IC 測試完畢都要記錄當時的 Ground & ESD 數值。
    void AddInRotationAngRecord(int iAngle);                                    //Sam 20221103 : 新增In旋轉角度
    void AddPickCleanPad(int iSuckRow, int iSuckCol, int iKitRow, int iKitCol, int iData);  //Sam 20230616 : Add Auto Clean Record
    void AddPlaceCleanPadToShuttle(int iSuckRow, int iSuckCol, int iShtRow, int iShtCol);
    void AddPickCleanPadFormShuttle(int iSuckRow, int iSuckCol, int iShtRow, int iShtCol);
    void AddPlaceCleanPad(int iSuckRow, int iSuckCol, int iKitRow, int iKitCol);
    void AddIndexPickVacuum(double dKpa);                                       //Sam 20230210 : 新增 VacuumUnit 通訊模組

    void SaveRecord(AnsiString ErrorLog="na");
    void SaveJHT_Record(AnsiString ErrorLog="na");     //JerryYang 20240405 : add
    void SaveDataForASE(AnsiString sPcName);
    void SaveDataForGreatek(AnsiString asPath);                                 //Sam 20200707 : for Greatek
    void SaveGroundESDData(AnsiString sLot);                                    //Sam 20211223 : 每顆 IC 測試完畢都要記錄當時的 Ground & ESD 數值。
    void SaveRecordCleanPad(AnsiString ErrorLog="na");
    void SaveRecordForLeadYo();                                                 //KenHsieh 20230727 : 更改工作檔與資料 By NetFile
    void Save2DIDForJCET(AnsiString SubLOTID, AnsiString CustLOTID, AnsiString Modecode);   //RogerYang 20251210 : JCET 2D FT1白名單/FT2比對功能

    bool bUse;

    int GetAutoX();
    int GetAutoY();
    int GetWhcihAuto();
    int GetLoaderX();
    int GetLoaderY();
    int GetLoaderNum();                                                         //Frank 20181121 add iLoaderNum
    int GetSiteNo();
    int GetOrderOfContact();
    AnsiString GetSOTTime();
    AnsiString GetEOTTime();
    AnsiString GetTestTime();
    AnsiString GetErrorLog();
    AnsiString GetInRotationAngRecord();                                        //Sam 20221103 : 新增In旋轉角度
    AnsiString asDataTitle;                                                     //Steven 20200416 : 整合Eventlog的title
    AnsiString asDataTitleGreatek;
    AnsiString asDataTitle_SG;                                                  //Sam 20211223 : 每顆 IC 測試完畢都要記錄當時的 Ground & ESD 數值。
    AnsiString asDataTitleCleanPad;                                             //Sam 20230616 : Add Auto Clean Record
    AnsiString asJCET2DIDList;                                                  //RogerYang 20251210 : JCET 2D FT1白名單/FT2比對功能
    TStringList *asBuffer;
    AnsiString GetProdLogFileName(bool bTryCatch);
    AnsiString GetProdLogFilePath();
};
//------------------------------------------------------------------------------
enum eMyProdRec
{
    eScheduleName           =0,
    eStartTime              =1,
    eLoadNum                =2,
    eLoadX                  =3,
    eLoadY                  =4,
    eLoadTime               =5,
    eHotX                   =6,
    eHotY                   =7,
    eHotTime                =8,
    e2DCode                 =9,
    eSiteNO                 =10,                                                //1-32
    eArmNO                  =11,                                                //0: arm1  1: arm2
    eArmTime                =12,
    eOrderTest              =13,                                                //touch-down
    eSOTTime                =14,                                                //kevin 20140918  yyyymmdd_hhmmss
    eIndexCycleTime         =15,                                                //第一次測試 是 0 (1st device)
    eTestCate               =16,                                                //測試 BIN別
    eWhichAuto              =17,
    eOutTrayNum             =18,                                                //Auto1 :O1-1  Auto2 :2-1  Auto3 :3-1  Fix 1:4-1 Fix 2:5-1 Fix 3:6-1
    eUnloadX                =19,
    eUnloadY                =20,
    eUnloadTime             =21,
    eUnloadXPos             =22,                                                //Frank 20160727 add
    eUnloadYPos             =23,                                                //Frank 20160727 add
    eUnloadPosTime          =24,
    eErrorCode              =25,
    eOCRCode                =26,                                                //wei 20161005
    eTestTime               =27,
    eTSDTime                =28,
    eEOTTime                =29,                                                //wei 20181211 更換位置SOT
    eInArmLoaderPick        =30,                                                //Sam 20200623 : Add record
    eIndexPickSHTSiteNo     =31,                                                //Sam 20200623 : Add record
    eIndexPlaceSHTSiteNo    =32,                                                //Sam 20200623 : Add record
    eOutSHTDetectSiteNo     =33,                                                //Sam 20200623 : Add record
    eOutArmShuttlePick      =34,                                                //Sam 20200623 : Add record
    eHotplateNo             =35,                                                //Sam 20200716 : Add record
    eInArmHotplatePick      =36,                                                //Sam 20200716 : Add record
    eTestMode               =37,                                                //Sam 20201216 : Add record
    eUnloadXpPos            =38,
    eUnloadXp2Pos           =39,
    eUnloadYpPos            =40,
    eTrayForm               =41,                                                //Sam 20210907 : Add record
    eHotPlateForm           =42,                                                //Sam 20210907 : Add record
    eTrayID                 =43,                                                //Steven 20220120 : Amlogic需要收SBIN
    eSBin                   =44,                                                //Steven 20220120 : Amlogic需要收SBIN
    eGround                 =45,                                                //Sam 20211223 : 每顆 IC 測試完畢都要記錄當時的 Ground & ESD 數值。
    eESD                    =46,                                                //Sam 20211223 : 每顆 IC 測試完畢都要記錄當時的 Ground & ESD 數值。
    eInRotationAngle        =47,                                                //Sam 20221103 : 新增In旋轉角度
    ePickCleanPadData       =48,                                                //Sam 20230616 : Add Auto Clean Record
    ePickCleanPadTime       =49,
    ePickCleanPadSuck       =50,
    ePickCleanPadRow        =51,
    ePickCleanPadCol        =52,
    ePlaceCleanPadToShtTime =53,
    ePlaceCleanPadToShtSuck =54,
    ePlaceCleanPadToShtRow  =55,
    ePlaceCleanPadToShtCol  =56,
    ePickCleanPadFormShtTime=57,
    ePickCleanPadFormShtSuck=58,
    ePickCleanPadFormShtRow =59,
    ePickCleanPadFormShtCol =60,
    ePlaceCleanPadTime      =61,
    ePlaceCleanPadSuck      =62,
    ePlaceCleanPadRow       =63,
    ePlaceCleanPadCol       =64,
    eLoaderPickVacuum       =65,                                                //Sam 20230210 : 新增 VacuumUnit 通訊模組
    eHPPickVacuum           =66,
    eOSHTPickVacuum         =67,
    eIndexPickVacuum        =68,
    eLoadXPos               =69,                                                //Sam 20250211 : Add Load Pos
    eLoadYPos               =70,
    eDataTotal                                                                  //擺在最下面
};
//------------------------------------------------------------------------------
#endif
