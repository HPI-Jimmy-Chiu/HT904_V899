//---------------------------------------------------------------------------

#ifndef ainarm2H
#define ainarm2H
//---------------------------------------------------------------------------

//#include "cprod.h"
#include "myTimer.h"
extern bool flag1_1x2_2[2];
typedef struct                                                                  //Sam 20181201 : Auto Alignment
{
    int iP;
    int iPlateR;
    int iPlateC;
    int iSuckR;
    int iSuckC;
    void ClearData()
    {
        iP=-1;
        iPlateR=-1;
        iPlateC=-1;
        iSuckR=-1;
        iSuckC=-1;
    };
    void UpdateData(int _iP, int _iPlateR, int _iPlateC, int _iSuckR, int _iSuckC)
    {
        iP      =_iP;
        iPlateR =_iPlateR;
        iPlateC =_iPlateC;
        iSuckR  =_iSuckR;
        iSuckC  =_iSuckC;
    };
}strAUTOSITEMAP;
extern strAUTOSITEMAP InArmSiteMapData;                                         //Steven 20211209 : 紀錄Site map資料

extern int  iInXPToSht[X_PITCH_COUNT];
extern int  iZPosToSht[MAX_ARM_Row][MAX_ARM_Col];
extern bool bZFlgToSht[MAX_ARM_Row][MAX_ARM_Col];
extern bool bInArmCheckDestroyACT[MAX_ARM_Row][MAX_ARM_Col];                    //jou 981130 確認device確實destroy完成
extern void ResetInToShtFlag();

extern const bool ZAxisDown;
extern const bool ZAxisNotDown;

extern TQPF_Timer InArmReleaseDelayToHot;                                       //JerryYang 20160127 for TSMC inarm release device前delay
extern TQPF_Timer InArmReleaseDelay;                                            //JerryYang 20160127 for TSMC inarm release device前delay
extern TQPF_Timer MyInArmAtShuttleTimer;                                        //Steven 20151201
extern bool IndexZCanMove[2];
extern bool bHangTimePause;                                                     //Steven 20090827 : Hang Up dectector

extern bool bPlaceToHotplate;
extern bool bPickFromHotplate;
extern bool bPlaceShuttle;
extern bool bPickFromLoader;                                                    //Steven 20171226 (Wei) : 確認Loader吸取完成
//extern bool bDestoryOnSht;                                                    //RogerYang 20251021 : 搬去cmydef  //Steven 20170905 (wei) : 在Shuttle吹氣與資料交換的Flag
extern bool bPlaceToShuttle2Step;                                               //Steven 20160721 : 避免放一半歸零,會Hangup
extern bool bPlaceToCleanKit;                                                   //Steven 20171204 (Wei) : 避免放到一半要讓位不回來原本位置放
extern bool bDoPreciser;

extern bool bInArmHasHotIC;                                                     //Sam 20211012 : Debug 用

extern int iPickPlate[2],  iPickPlateX[2],  iPickPlateY[2];
extern int iPlacePlate[2], iPlacePlateX[2], iPlacePlateY[2];

extern int iBackupPlate, iBackupPlateR, iBackupPlateC;                          //ChungHung 20120206 Hotplate check

extern bool InArmSuckUse[MAX_ARM_Row][MAX_ARM_Col];

extern void InArmAddSpeed();
extern void InArmSubSpeed();

extern bool bBackMMTrayY_Car;
extern bool bBackMMTrayY;
extern const int cMaxInArmOrder;
extern int cMaxKit;
extern int iBackInArmHotCount;
extern int iHotCount;                                                           //   用以記錄同一次 Shuttle 的IC
extern int iHotPlateCount[2][50][50];                                           //   plate 上的 iHotCount 值
extern int iHotInArmOrder[2][50][50];                                           //   用以記錄 InArm 的前後組吸嘴
extern int iHotWhichKit[2][50][50];                                             //   plate 上的 IC 預備要放到那一個 shuttle 左,或右組 Kit
extern int iHotWhichShuttle[2][50][50];                                         //   plate 上的 IC 預備要放到那一個 shuttle
extern int  iXPosition[8], iYPosition;
extern bool InArmXMoveSafe, InShuttle2;
extern bool bShuttleShake;
extern bool bShuttleKnock;                                                      //Jou 2013-03-08 修改敲敲功能

extern int iAutoCleanStart;
extern int iAutoCleanNum;
extern int iAutoCleanCnt;                                                       //JerryYang 20171127 (Steven) add auto clean清潔次數

extern int iNextPlateRow;
extern int iRecordTrayPickPosX[16][16];
extern int iRecordTrayPickPosY[16][16];

extern bool CheckInArmFinishAllPickerAction();                                  //Steven 20171226 (Wei) : 修改in arm讓開的flag

extern int  iArmTask;
extern void InitInArmTask();
extern void DoInArm();

extern int  iPickFromLoadStageTask;
extern void InitArmPickFromLoadStageTask();

extern void ChangeHotPlateData(bool bSwapSht=false);
extern void ChangeHotPlateDataToOneShuttle();

extern int  iInArmPlaceToHotPlateTask;
extern void InitInArmPlaceToHotPlateTask();
extern void InitInArmPlaceToHotPlateTask400();                                  //ChungHung 20120502 HangUp 解除
extern void InitInArmPlaceToHotPlateTask100();
extern bool CheckHotPlateHasSpaceToPlace();
extern void SearchPlateToPlace();

extern int  iInArmPickFromHotPlateTask;
extern void InitInArmPickFromHotPlateTask();
extern void InitInArmPickFromHotPlateTask340();                                 //ChungHung 20120502 HangUp 解除
extern void InitInArmPickFromHotPlateTask50();                                  //ChungHung 20120112 Hang Up 解除
extern bool SearchPlateToPick();

extern int  iInArmTryPickFromHotPlateTask;                                      //ChungHung 20120206 Hotplate check
extern void InitInArmTryPickFromHotPlateTask();                                 //ChungHung 20120206 Hotplate check
extern void InitInArmTryPickFromHotPlateTask100();                              //ChungHung 20120206 Hotplate check

extern int  iInArmPlaceToShuttleTask;
extern void InitInArmPlaceToShuttleTask();

extern bool CheckInArmZ();
extern void AdjustShuttlePlaceOrder(int iReset=-1);
extern void AdjustShuttlePlaceOrderForASM(bool bResetSht=false);
extern void AdjustShuttleWhichKitOrder();                                       //Steven 20140626 : Modify to Function
extern void AdjustShuttlePlaceOrder_AutoSiteMapping();                          //jou 2016-11-07 JCET fix Auto Site mapping Hot mode hangup
extern void AdjustShtOrderWhenPlaceToSht(int iMode);                            //Steven 20180601 : 整合放完蝦頭後的調整
extern void ResetShuttleWhichKit();                                             //Steven 20140710

extern bool CheckInArmSuckInitial();
extern void SetInArmHome(bool bPrecisorNeedHome=false);                         //jou 981203 alarm時，in arm z motor home
extern bool InArm2SuckNeedPlace();
extern bool IsMoveInArm2XYToWait();

extern bool MoveInArmZToPlateSafeAndCheckLoaderTray(int iTask);                 //Ifor 20221227 add:新增Loader 取料後判斷是否吸到Loader Tray
extern bool MoveInArmZToPlateSafe(int Task);
extern bool IsMoveInArm2XYToShuttle2Wait();                                     //Sam 20250522 : 增加保護，避免已經在跑殘料檢測時 InArm 在干涉區
extern bool MoveInArm2XYToShuttle2Wait();
extern bool MoveInArm2XYToLoaderWait();                                         //Steven 20120314 : 要移動到不熱的等待位置
extern bool MoveInArm2XYToDecayTeach();                                         //Ifor 20151210 新增InArm Move Decay
extern bool MoveInArm2XYToWait();

extern void DoInArm_SuckerMap();
extern void DoInArm_SuckerMapForCloseArm();

extern void SetInArmUseSuckToHasNullIC(int iSht, int iKit);
extern void SetInArmUseSuckToHasTrySuckIC(int iSht, int iKit);
extern void SetInArm_Unuse_SuckToNullICForHP();

extern bool RowCanDualSite();
extern int  CloseSiteState(bool bPlace=true);

extern void TransferHotPlateRatio(bool bPick, int *iXPos, int *iYPos);          //Steven 20110324 : 加熱盤的軟體齒輪比
extern void TransferInShuttleRatio(int iSht, int *iXPos, int *iYPos, int iRow, int iCol);                               //Steven 20110324 : In Shuttle的軟體齒輪比
extern void TransferLoaderRatio(int *iXPos, int *iYPos);                        //Steven 20141029 : Loader的軟體齒輪比

extern bool bPitchOver12000;                                                    //jou 2010-01-20 Hotplate 8x12 Pitch=26.67 (Quad Site 2x2)，吸嘴pitch over 12000

extern void BackupPlacePos();                                                   //ChungHung 20120206 Hotplate check
extern void RestorePlacePos();                                                  //ChungHung 20120206 Hotplate check
extern bool IsHotPlateCheckFinsih();                                            //ChungHung 20120206 Hotplate check

extern bool CheckClearAllHotICThenPickLoadIC();                                 //ChungHung 201205104 Clear HotIC then Pick Load IC
extern void InArmAddBlockPitch(int &iXPos, int &iYPos, int indexRow, int indexCol, int iXY);                            //Frank 20160928 add Subtray Function
extern bool bNeedPickupErrorICToRecycleBin();                                   //JimmyChiu 20220908 add Pickup Error Placement
//------------------------------------------------------------------------------
// AUTOCLEAN       kevin 20120217
extern bool bInArmZNeedCheck[4];
extern bool bInArmToLoaderUsage[MAX_ARM_Row][MAX_ARM_Col];
extern bool bInArmSuckActive[MAX_ARM_Row][MAX_ARM_Col];
extern int iAutoCleanPickPlateX, iAutoCleanPickPlateY;
extern int iAutoCleanUseXPitch;
extern bool bAutoCleanUse_13_24;                                                //ChungHung 20131118 add for SCK AutoClean use HotPlate

enum eShakeShuttle{essSetFlag=1,
                   essMoveRight,
                   essMoveLeft,
                   essKnockOn,
                   essKnockOff,
                   essEnd};
extern int iShakeShuttleTask;
extern int iKnockShuttleTask;
extern int iKnockShtFirstTask;
extern bool DoShakeShuttle(int iShuttle, bool bNeedInitial=false);              //Steven 20120801 : 修改抖抖功能
extern bool DoKnockShuttle(int iShuttle, bool bNeedInitial=false);              //Jou 2013-03-08 修改敲敲功能
extern bool DoKnockShuttleFirst(int iShuttle, bool bNeedInitial=false);         //jou 2015-12-09 SCS 要求 Shuttle 每次入料前 敲擊

extern void InitInDieCleanTask();
extern bool DoInDieClean();                                                     //wei 20170418

extern int  iProcessTrayMapDataErrorTask;                                       //wei 20170302 (Steven) Tray map data error
extern bool ProcessTrayMapDataError(bool bReset=false);                         //wei 20170302 (Steven) Tray map data error
extern void DoCheckAutoSiteMappingPosition();                                   //Ifor 20170928 (Steven) : Check Auto Site Mapping Position
extern void bOffsetClean();                                                     //kevin 20171117 add
extern int LoadTrayCanUse8Suck();                                               //kevin 20171120 (wei) 判斷LOAD TRAY Y PITCH是否是 ARM Y PITCH可以一次8吸嘴吸IC
//bool DoInArmZCheckPos();                                                        //wei 20171116
extern void InitInArmZCheckPos();                                               //wei 20171116
extern bool DoInArmDevicePosPrecise();                                          //Frank 20180410 (Steven) : InArm Preciser Station
extern void InitInArmDevicePosPrecise();
extern void PreciserPitchCalculate(int &iXPitch);
extern void InArmAddSpeedDisplay();                                             //KaiChen 20171225 (Steven)：Add Speed Display
extern void InArmSubSpeedDisplay();                                             //KaiChen 20171225 (Steven)：Add Speed Display
extern bool DoLoadNewICTray();
extern void InitLoadNewICTrayTask();
extern bool DoSupplyNewICTray();
extern void InitLoadNewICTrayTask();
extern HTimer hInArmYpitchHomeTimer;                                            //kevin 20180822  Ypitch
extern HTimer hOutArmYpitchHomeTimer;                                           //kevin 20180822 (Steven) : Ypitch home
extern bool DoVibrateShuttle(int iShuttle, bool bNeedInitial=false);            //JerryYang 20190123 shuttle震動馬達
extern bool DoVibrateOutShuttle(int iShuttle, bool bNeedInitial=false);         //JerryYang 20190123 shuttle震動馬達
extern bool EnableTraymapCheckFunction(int iCheck=0);
extern void CheckTrayMapData(int iTrayRow, int iTrayCol);
extern void RecordShtSuperflous(int iShuttle);                                  //JerryYang 20181121 (Steven) : add log,記錄in shuttle device是被震動馬達or搖搖功能導正
extern int  iInArmWaitPosition;                                                 //Ifor 20191105 : add In Arm 目前移動的等待位置 0:Wait 1:LoaderWait 2:DecayWait 3:Shuttle2
extern void RestoreLoadeIC(int iLoaderY=0);                                     //JerryYang 20200422 Auto skip次數到達後, 自動再去最後一排吸吸看
extern int iInArmZCheckPosTask;
extern int iInArmDevicePosPrecise;
extern bool bRecordVacuumTim;                                                   //kevin 20210224 記錄計算真空產生器壽命
extern void VacuumOnOffLog(AnsiString sLog);                                    //Sam 20210204 : Auto Teach Loader Z Record Vacuum On off time
extern bool bRecordVacuumTim;                                                   //kevin 20210224 記錄計算真空產生器壽命

extern void DisableAutoSiteMapWhenCleanOut();                                   //Steven 20220902 : 修正Auto Site Map, Loader沒有IC
extern void RecordAutoSiteMapStart();                                           //Steven 20230117 : 修正Auto Site map的訊息
extern void RecordAutoSiteMapFinish();                                          //Steven 20230117 : 修正Auto Site map的訊息
extern void SetInArmNeedDestory(bool bPlace, int iShtRow, int iShtCol, int iRow, int iCol);
extern void SetShuttlefCanMoveL(int iShuttle, bool bCanMoveL, AnsiString sFun, AnsiString sTask="");                    //Steven 20231124 : 換位置
extern void InitInOCRWaitTask();                                                //wei 20170901
extern bool OCRMoveInArm2XYToWait();                                            //wei 20170901
#endif
