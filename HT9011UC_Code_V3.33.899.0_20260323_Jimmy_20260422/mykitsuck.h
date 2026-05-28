//---------------------------------------------------------------------------
#ifndef MyKitSuckH
#define MyKitSuckH

//---------------------------------------------------------------------------
#include "MyLed.h"
#include "cmydef.h"
#include "myTimer.h"
#include "MyProductionRecord.h"

#define _MAX_SUCK_ROW_ITEM 4
#define _MAX_SUCK_COL_ITEM 8

class TMySucker
{
    private:
        int OnTask;
        int OffTask;
        int OnOff;
        int RetryCT;
        int rct;
        TQPF_Timer tSuckTimer;

        bool bSuckOK;                                                           //ChungHung 20110901 add
        bool bDestroyOK;                                                        //ChungHung 20110901 add

        TQPF_Timer  TOn;                                                        // 真空產生
        TQPF_Timer  TOff;                                                       // 真空破壞
        TQPF_Timer  TOnDelay;
        TQPF_Timer  TOffDelay;

        TQPF_Timer  TAgainDestroyDelay;                                         //ChungHung 20130413 add 針對 阿凡達IC 無法Relase
        int ract;                                                               //ChungHung 20130413 add 針對 阿凡達IC 無法Relase

        void DoOnIO(bool bOn);                                                  //Steven 20230721 : 統一真空開關控制, 使用Private是不讓外部使用, 外部操作用原本的function
        void DoOffIO(bool bOn);                                                 //Steven 20230721 : 統一破壞開關控制

    protected:
    public:
        _fastcall TMySucker::TMySucker();
        _fastcall TMySucker::~TMySucker();
        DWORD VacuumOnTime;
        DWORD VacuumOffTime;
        int   VacuumOnTimeBuffer[20];
        int   VacuumOffTimeBuffer[20];
        DWORD VacuumOnTimeRecord;
        DWORD VacuumOffTimeRecord;

        AnsiString SensorName;
        AnsiString OnPortName;
        AnsiString OffPortName;
        AnsiString SuckerName;
        bool fAlarm;
        bool Error;
        bool Enable;
        bool Status;
        bool AlarmEnable;
        int  OnAlarmTime;
        int  OffAlarmTime;
        int  OnDelayTime;
        int  OffDelayTime;

        AnsiString OnUsing;
        int  OnRing;
        int  OnIP;
        int  OnPort;                                                            // 真空產生
        int  OnBit;
        int  OnType;                                                            // A or B Type
        int  OnISABase;
        bool OnEnable;

        AnsiString OffUsing;
        int  OffRing;
        int  OffIP;
        int  OffPort;                                                           // 真空破壞
        int  OffBit;
        int  OffType;                                                           // A or B Type
        int  OffISABase;
        bool OffEnable;

        AnsiString SenUsing;
        int  SenRing;
        int  SenIP;
        int  SenPort;
        int  SenBit;
        int  SenType;
        int  SenISABase;

        int  iManualOffTask;

        bool Suck();                                                            //真空電磁閥 ON  破壞電磁閥 OFF
        bool Destroy();                                                         //真空電磁閥 OFF 破壞電磁閥 ON
        void On();                                                              //真空電磁閥 ON  破壞電磁閥 OFF
        void Off();                                                             //真空電磁閥 OFF 破壞電磁閥 ON
        void OnSuck();                                                          //真空電磁閥 ON
        bool OffSuck();                                                         //真空電磁閥 OFF
        void OnDestroy();                                                       //破壞電磁閥 ON
        void OffDestroy();                                                      //破壞電磁閥 OFF
        void Normal();                                                          //全部關閉

        void Reset();
        void ReStart();
        bool GetStatus();
        bool Sensor();
        bool GetOnBit();
        bool GetOffBit();

        double GetIOValue();                                                    //Sam 20230210 : 新增 VacuumUnit 通訊模組
        double GetIOValueThread();
        bool SetIOValueThread(double dKpa);
        bool FinshFullMotion();
        void ResetSuckTask();
        void SetRetryCount(int);
        void CheckIsFallDown();
        bool bIsRotate;
        void PushOnTime();
        void PushOffTime();
        bool RealTimeRefreshVacuumOnOffTime;
        bool bNeedSuck;
        bool bNeedDestroy;
        void SetNeedSuck(bool Value)        {bNeedSuck=Value;}
        void SetNeedDestroy(bool Value)     {bNeedDestroy=Value;}
        bool GetNeedSuckStatus()            {return bNeedSuck;}
        bool GetNeedDestroyStatus()         {return bNeedDestroy;}
        void InitSuckFlag()                 {SetNeedSuck(false);SetNeedDestroy(false);}

        bool IsSuckFinish();
        bool IsDestroyFinish();

        int  DestroyAgainCount;                                                 //ChungHung 20130413 add 針對 阿凡達IC 無法Relase
        double DestroyAgainTime;                                                //ChungHung 20130413 add 針對 阿凡達IC 無法Relase
        int  iNozzleEvent;                                                      //Klutter 20210817 方便修正吸嘴回home異常事件       //Steven 20210825 : 吹氣完成才可以歸零
        int  ISABase;                                                           //Nickliu 20230306 add IO Use ISABase

        AnsiString sRecordStartOffDelayTime;                                    //Sam 20220329 : Record Index Air On Time
        AnsiString sRecordEndOffDelayTime;
        int  iRecordOffDelayTime;
        int  iMotNo;                                                            //Steven for HT1032
        int  iMyRow;                                                            //實體Row
        int  iMyCol;                                                            //實體Col
        AnsiString sName;                                                       //Alarm時, 顯示吸嘴的位置
        double GetVaccum();                                                     //Sam 20230210 : 新增 VacuumUnit 通訊模組
};

class TMyKitSuck{
    private:
    protected:
    public:
    TALed *pLed[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];
    bool   bLed[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];

    _fastcall TMyKitSuck();
    _fastcall ~TMyKitSuck();

    TMySucker Suck[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];
    int  iMotRow;
    int  iMotCol;
    int  iMaxRow;
    int  iMaxCol;
    int  iMaxCnt;
    int  iPickRow;
    int  iPickCol;
    int  iShtRow;
    int  iShtCol;
    int  iShtCnt;
    int  iPickStep;                                                             //Pitch倍數, 13吸嘴就寫2, 14吸嘴就寫3
    int  iPickKitStep;
    int  iShtKitStep;
    int  iXStep;                                                                //X pitch要跑幾次
    int  iYStep;                                                                //Y pitch要跑幾次
    int  iModeX;                                                                //針對NON-Standard的X陣列要用哪一組

    int  Item[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];
    bool bNeedCheck[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];
//    char iErrorCode[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM][16];
//    int  iPixelDrop[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];
    int  iWhichSite[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];
    int  iWhichAuto[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];
    int  iNeedRotAng[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];                   //預計旋轉的角度   //Steven 20170425 (wei) : Add rotate motor
    int  iCurrRotAng[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];                   //目前產品的角度
    bool bQATray[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];
    int  iCleanCount[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];
    bool bFliped[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];
    int  iWhichIndex[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];
    int  iNeedSuck[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];                     //0:不需要吸, 其他:IC type

    bool bPass[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];
    bool bNeedReTest[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];
    int  iBinData[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];
    int  iBinDataBackUp[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];                //Sam 20180612 : 有開啟 D22 Double Contact Contact， 以第一次的測試結果來做 ProcessCount
    bool bNeedRotate[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];
    int  iWhichSht;
    int  iWhichKit;
    int  iWhichTray;
    int  iWhichShtPickFor32;                                                    //Steven 20140512: For HT-9047
    int  iWhichKitPickFor32;                                                    //Steven 20140512: For HT-9047
    int  HotCount;

    AnsiString cDeviceInfAutoSitemapping[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];   //Ifor 20170925 (Steven) : add
    AnsiString cReDeviceInfAutoSitemapping[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM]; //Ifor 20170925 (Steven) : add
    int  iNeedRotAngAutoSitemapping[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];        //Ifor 20170925 (Steven) : add 預計旋轉的角度
    int  iCurrRotAngAutoSitemapping[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];        //Ifor 20170925 (Steven) : add 目前產品的角度
//    int  iTrayXAutoSitemapping[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];           //Ifor 20210524 add:mykitsuck移至cmydef
//    int  iTrayYAutoSitemapping[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];           //Ifor 20210524 add:mykitsuck移至cmydef
    int  iWhichShuttleAutoSitemapping[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];
    int  iWhichKitAutoSitemapping[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];
    int  iHotCountAutoSitemapping[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];
    int  iWhichShuttleBackup;
    int  iWhichKitBackup;                                                       //wei 20171211 (jou) InArmSuck.iWhichKit-->InArmSuck.iWhichKitBackup

//    bool fPin1Check[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];
//    int  iScanResult[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];
    bool bScan[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];
//    bool fSearch[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];
    int  iAOIStation[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];                   //2014-03-04    Dell    for SPIL WLP Add 5S Inspection

//    int iStartMin[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];
//    int iStartSec[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];
//    int iStartMSec[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];
//    int iEndMin[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];
//    int iEndSec[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];
//    int iEndMSec[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];
//    int iTestOrder[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];                   // wait for HT90160T upadte
//    int iRecevieMin[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];                  //KaiChen 20171127 (Steven) ：超豐 OEE 新增 Test Time、Index Time
//    int iRecevieSec[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];                  //KaiChen 20171127 (Steven) ：超豐 OEE 新增 Test Time、Index Time
//    int iRecevieMSec[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];                 //KaiChen 20171127 (Steven) ：超豐 OEE 新增 Test Time、Index Time

    AnsiString cDeviceInf[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];              // 2012.06.01 Q_Q BarCode Mode
    AnsiString cReDeviceInf[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];            // 2012.06.01 Q_Q BarCode Mode
    AnsiString cSBin[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];                   //Steven 20220120 : Amlogic需要收SBIN
//    int iCassettePos[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];                 // 2012.06.01 Q_Q BarCode Mode
//    AnsiString cDeviceTrayID[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];         //wei 20180529
//    AnsiString cDeviceTrayLot[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];        //wei 20180529
    bool b2DIDNG[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];                       //Steven 20200611 : for Murata, 2DID NG不測試
    int iAOIResult[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];                     //Sam 20240325 : 新增 DamageTrayMapping 功能
    TMyProductionRecord PordRec[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];

    bool bTrayMapNGData[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];                //KenHsieh 20220923 : add Tray Map Throw IC Function

//    int  AlarmList;                                                           //Steven 20091123

    TQPF_Timer  TSoakTime;                                                      //jou 2012-06-29 Index Pick up need wait Soak Time

    int iAutoCleanRecX[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];                 //jou 2013-03-13 Auto Clean Record X
    int iAutoCleanRecY[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];                 //jou 2013-03-13 Auto Clean Record Y

    void SetMyLed(int iRow, int iCol, TALed *ledPtr);                           //設定燈號
    void SetItemAmount(int iRow, int iCol);                                     //設定數量
    void SetMotorCount(int iRow, int iCol);
    void SetPickerCount(int _iPickRow, int _iPickCol, int _iShtRow, int _iShtCol, int _iPickStep, int _iKitStep, int _iShtStep);
    void SetPickerCount(int _iPickRow, int _iPickCol);

    void ResetAll();                                                            //重置SuckTask
    bool CheckLedStatus();
    void CheckVaccumIsIniaialON(int iRow, int iCol, bool &flag);                // if sensor Off  ,off vacuum ,flag not chage
                                                                                // if        On   ,if has not Real IC then off vacuum,flag=true
                                                                                //                 else do not change
    //兩個資料交換----------------
    void MoveAllItem(class TMyKitSuck &Source);
    void MoveSingalItem(class TMyKitSuck &Source, int iWhichSuck);
    void CopyFrom (class TMyKitSuck &Source);
    void MergeSuck(class TMyKitSuck &Source1, class TMyKitSuck &Source2, bool InsertCol);
    void SplitSuck(class TMyKitSuck &Source1, class TMyKitSuck &Source2, bool InsertCol);
    void MergeSuck1Row(class TMyKitSuck &Source1, class TMyKitSuck &Source2);
    void SplitSuck1Row(class TMyKitSuck &Source1, class TMyKitSuck &Source2);
    void MoveSuckData(class TMyKitSuck &Source, int SourceR, int SourceC, int TargetR=-1, int TargetC=-1);
    void MoveSuckDataDiff(class TMyKitSuck &Source, int SourceR, int SourceC, int TargetR, int TargetC);
    void CopyFromTray(int iSuckR, int iSuckC, int iSuckData, class TTrayMotor &Mot, int TrayR, int TrayC, int iTrayData, int iTarget=-1, bool bHP2Inarm=false);   //Steven 20150714 : Manual Sort
    void CopyToTray  (int iSuckR, int iSuckC, int iSuckData, class TTrayMotor &Mot, int TrayR, int TrayC, int iTrayData, int iTarget=-1);   //Steven 20150714 : Manual Sort

    //設定資料----------------
    void SetAll(int Type);
    void SetAllToNullIC();                                                      //kevin 20150505
    void SetAllRealIC2InterfaceBin();
    void SetAllHASIC2ErrorBin();
    void SetUnuseToNullIC();                                                    //Steven 20241017 : 清除沒用到的資料
    void SetUnuseAndHasNullICToNullIC();                                        //Steven 20241017 : 清除沒用到的資料 與 HAS_NULL_IC資料
    void SetNullIcToHasNullIc();
    void SetHasNullIcToNullIc();                                                //Steven 20160524 : ADD SetHasNullIcToNullIc
    void SetItemData(int iRow, int iCol, int data, int iTarget=-1);

    //清除資料----------------
    void ClearAll();
    void ClearAllError();
//    void ClearSingle(int i, int j);

    //檢查內容---------------------
//    bool HasNoScanIC();
    bool FindNoIC();                                                            //判斷是否有空位置  kevin 20120531
    bool HasIC();
    bool HasRealIC();
    bool HasRealIC_Left(int iRow, int Mid);
    bool HasRealIC_Right(int iRow, int Mid);
    bool HAS_NO_IC();
    bool HasDefineIC(int IC_TYPE);                                              //Steven 20130620 : 改成Function
    bool RowHasDefineIC(int iRow, int IC_TYPE);                                 //Steven 20221005 : 針對上下排判斷有沒有指定的IC
//    bool HasDefineICFor12Site(int IC_TYPE);                                   //JerryYang 20160616 : 12site只判斷所使用的site
//    bool HasDefineICFor10Site(int IC_TYPE);
    bool HasType(int);
    bool HasNotTestYet();
    bool Tested();
    bool NoIC();

    bool All_HAS_NULL_IC();
    bool All_HasIC();
    bool ShtAll_HasICType(int IC_TYPE1, int IC_TYPE2);
    bool ArmAll_HasICType(int IC_TYPE1, int IC_TYPE2);
//    bool All_HasICType(int IC_TYPE1);                                         //JerryYang 20160419
    bool UseSiteFullIC();                                                       //Ifor 20161215 add 判斷Use Site 有無NULL_IC
    bool UseSiteHasIC();
    bool UseSiteNoIC();

    int  CountRealIC();
//    int  CountReal_Has_NUll_IC();                                             //kevin 20140903 判斷是否在第二次放IC因關SITE 造成無法清除  IN ARM HAS_NULL_IC

    bool AlreadyTest();
    bool AlreadyTestNotIncludeErrorBin();                                       //Steven 20200611 : for Murata, 2DID NG不測試
    bool PartAlreadyTest();

    bool RightSideNoIC(int MiddleValue);
    bool LeftSideNoIC(int MiddleValue);
//    bool ArmRightSideNoIC(int MiddleValue);
//    bool ArmLeftSideNoIC(int MiddleValue);
    bool ArmLeftSideHaveRealIC(int MiddleValue);
    bool ArmRightSideHaveRealIC(int MiddleValue);
    bool ArmUpSideNoIC();
    bool ArmDownSideNoIC();
    bool ArmRow0HaveRealIC();
    bool ArmRow1HaveRealIC();
    bool ArmUpSideHaveRealIC(bool left=true);                                   //ChungHung 20130708 add left=true
    bool ArmDownSideHaveRealIC(bool left=true);                                 //ChungHung 20130708 add left=true;
    bool ArmUpSideAllTypeIC(int IC_TYPE, int iOffset, int iCol);                //Steven 20220930 : CheckSpecialSiteMode run Auto Clean
    bool ArmDownSideAllTypeIC(int IC_TYPE, int iOffset, int iCol);              //Steven 20220930 : CheckSpecialSiteMode run Auto Clean

    bool IsShtSuckFinish();
    bool IsShtDestroyFinish();
    bool IsPickSuckFinish();
    bool IsPickDestroyFinish();
//    bool HAS_CleanIC();                                                       //kevin 20180425 沒有Clean ic 進index或掉料

//    bool ArmUpSideNullIC(int iMax, int iCol);                                 //kevin 20201006 add 12 site
//    bool ArmDownSideNullIC(int iMax, int iCol);                               //kevin 20201006 add 12 site
    bool CheckDestoryFinish();                                                  //Steven 20210825 : 吹氣完成才可以歸零

    bool bAlreadyRotate;                                                        //Steven 20210609 : 整合Precisor, Rotator, Bottom CCD, Die Clean
    bool bAlreadyPreciser;
    bool bAlreadyDieClean;
    bool bAlready2DID;
    bool bAlreadyAOI;
    bool bAlreadyFlipper;                                                       //Frank 20210612 : Flipper Function
    bool bAlreadyFixAI;                                                         //Sam 20211220 : 整合 Fix AI AOI
    bool bAlreadyThrowIC;                                                       //KenHsieh 20220923 : add Tray Map Throw IC Function
};

extern TMyKitSuck InArmSuck;
extern TMyKitSuck FLCarryKit;
extern TMyKitSuck FRCarryKit;
extern TMyKitSuck BLCarryKit;
extern TMyKitSuck BRCarryKit;
extern TMyKitSuck OutSht3Kit;                                                   //Steven 20240822 : For HT-9046AU
extern TMyKitSuck FTestSuck;
extern TMyKitSuck BTestSuck;
extern TMyKitSuck OutArmSuck;
extern TMyKitSuck OutArm2Suck;                                                  //Steven 20240822 : For HT-9046AU
extern TMyKitSuck CatchTraySuck;
extern TMyKitSuck TestSocket;
extern TMyKitSuck CheckKitSuck;                                                 //20111130  Dell
extern TMyKitSuck AOIKit;                                                       //2014-03-04    Dell    for SPIL WLP Add 5S Inspection
extern TMyKitSuck InArmPlaceSuck;                                               //JimmyChiu 20220908 add Pickup Error Placement
//---------------------------------------------------------------------------
extern TMyKitSuck FTestSuckBackup;
extern TMyKitSuck BTestSuckBackup;

extern TMyKitSuck InArmSuckBackup;
extern TMyKitSuck OutArmSuckBackup;

extern TMyKitSuck *ptrInSHT;                                                    //Auto Clean Kit use
extern TMyKitSuck ptrInSHTBackup;                                               //Auto Clean Kit use
extern TMyKitSuck *ptrOutSHT;
extern TMyKitSuck *ptr2DIDSHT;
extern TMyKitSuck *ptrMulti2DIDSHT;

extern TList *pSuck;
extern TMySucker *pTempSuck;

extern void ClearAllManualSuckTask();
extern void InitialSuckerName();
extern void SetAllRealIC2InterfaceBin();
extern void CopyKitSuck(TMyKitSuck *Source, TMyKitSuck *Target);
extern void CopySuck(TMySucker *Source , TMySucker *Target);
extern void CopyInitSuck(TMyKitSuck *Source, TMyKitSuck *Target, int SourceR, int SourceC, int TargetR, int TargetC);
extern TMyProductionRecord OutArmPordRec[_MAX_SUCK_ROW_ITEM][_MAX_SUCK_COL_ITEM];   //Steven 20210316 : 掉料的時候, 清除Unloader tray上的資料
#endif
