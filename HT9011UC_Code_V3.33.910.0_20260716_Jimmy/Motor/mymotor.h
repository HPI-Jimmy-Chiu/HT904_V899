//---------------------------------------------------------------------------
#ifndef MyMotorH
#define MyMotorH

#include "HTMotor.h"

#include "mytray.h"
#include "HTray.h"
#include "cprod.h"
#include "myTimer.h"
#include "myGALILmotor.h"
#define MAX_DEVICES 100                                                         //Sam 20230707 : EtherCAT Shuttle sensor

//---------------------------------------------------------------------------
class TMyMotor
{
    private:
        int  RefStart;
        int  RefEnd;
        int  FactStart;
        int  FactEnd;
        double  Scale;
        int  MotorMovePosition(int &Position, int speed, int Target);
        TQPF_Timer ResetTime;
        int iMyHomeTask;
        int iHomeRetryCT;
        TQPF_Timer htWaitHomeSensorOnDelay;
        bool bPanelUse;
        bool bUpDownMove;
        TWinControl *PWinCtrl;
        int iHomeCount;
    protected:
        std::map<AnsiString, AnsiString>mapLockList;                            //Steven 20210825 : 吹氣完成才可以歸零
        std::map<AnsiString, AnsiString>::iterator mapLockIter;                 //Steven 20210825 : 吹氣完成才可以歸零
        AnsiString Gali_GetMOT(int MOT);
        bool CheckPos(bool Front);
        bool CheckYPos();                                                       //JerryYang 20180411 (jou) : Z軸移動前確認Y軸位置保護
        bool CheckPos_nnMode(bool bOpen);
    private:
        TQPF_Timer DelayCT;
        int iEncoderCheckCT;
        TQPF_Timer tSh1FloodgateOpenTimeout;                                    //Ifor 20260401 add: Floodgate open timeout
        TQPF_Timer tSh2FloodgateOpenTimeout;                                    //Ifor 20260401 add: Floodgate open timeout
        bool bSh1FloodgateOpenWaiting;                                          //Ifor 20260401 add: Floodgate open timeout flag
        bool bSh2FloodgateOpenWaiting;                                          //Ifor 20260401 add: Floodgate open timeout flag
    public:
        __fastcall TMyMotor();
        ~TMyMotor();
        int HomeFlag;

        int Mot_Name;
        AnsiString Alias;
        AnsiString NumberAlias;
        void SetAlias(int iNo, AnsiString Name);

        HTMotor *Motor;
        int   speed;                                                            // run speed
        bool  fCanMove;
        bool  fCanMoveR;
        bool  fCanMoveM;
        bool  fCanMoveL;

        int  TargetPosition;
        int  Position;                                                          // motor now speed
        int  EncoderPosition;
        int  ScreenPos;

        int  OldSpeed;

        void SetPanel(TWinControl *PCtrl, bool b);
        void SetScreenScale(int s1, int e1, int s2, int e2);
        void SetSpeed(double p, bool bSetJog=false);                            //RogerYang 20250729 Add for 9046AU

        void EnableMotorMove();
        int  MotorMove(int p);
        bool MotorMoveShuttleShake(int p);                                      //JerryYang 20190628 shuttle shake專用command
        bool MotorMove2SpeedForPicker(int FinalPos, ARM_CONDITION *ARM, bool bIsLoader=false);  //Steven 20140217 : 兩段速移動  //JerryYang 20190729 二段速功能可選擇only at loader

        void MotorInitial();
        void ScanMotorStatus();

        int  MotorHome(bool);
        int  ReadPos();
        int  GetSpeed();
        bool GetMotorAlarm();
        int  GetErrorIndex();

        bool Led[10];
        bool Home2Led;
        bool MovFlag;

        bool Home(void);
        int  HomeTask;
        void HomeReset();

        AnsiString CardType;                                                    //RogerYang 20250411 ECAT Motor need reinitiated when reopencard
        void MagazineUp();
        void MagazineDown();

        bool bIsFullIC;
        bool bIsEmptyIC;

        int  GailSpeed;
        bool Gali_MotorAlarm;
        void Gali_ScanMotStatusTIMO();
        void Gali_ScanMotStatus();
        void Gali_ScanAlarmStatus();
        bool Gali_MotMove(int Pos, int Speed, AnsiString _Func="");
        bool Gali_MovePR(int Pos, int Speed);
        bool Gali_MotMove2(int Pos, int Speed, int SpeedDec);
        bool Gali_MotMoveNoWait(int Pos, int Speed, int iNeedDelayTime, bool bCheckZ=true);
        bool Gali_MotMoveSkipEncoder(int Pos, int Speed);
        bool Z1UpZ2Down1(int Speed);
        bool Z1DownZ2Up1(int Speed);
        bool Z1UpZ2Down2(int Speed, bool bPickErr);
        bool Z1DownZ2Up2(int Speed, bool bPickErr);
        bool Z1UpZ2Down(int Speed, bool TMode, bool bPickErr);
        bool Z1DownZ2Up(int Speed, bool TMode, bool bPickErr);

        long Gali_Command(AnsiString str, AnsiString sFunc="");
        long Gali_ReadPos();
        long Gali_ReadEncoderPos();

        void Gali_MotHome(AnsiString HomeAxis);
        void Gali_MotHome_HighSpeed(AnsiString HomeAxis, int Speed);            //RogerYang 20250410 for 9046AU
        void Gali_MotHomeFindZ(AnsiString HomeAxis);                            //ChungHung 20141016 add find Z相
        bool Gali_SingalHome(bool IndexZFirstHome=false);                       //RogerYang 20161116 加入 bool flag
        bool Gali_FindZPhase();                                                 //Ifor 20170817 (wei) add Find Z Phase

        bool Gali_Two_ZAxis_Move(int Pos, int Speed, AnsiString sFunc="", bool bTwoPos=false, int Pos2=0);
        void Gali_JogP(int Speed);
        void Gali_JogPSetup(int Speed);
        void Gali_JogPAndCount(int Speed, int Count);
        void Gali_JogN(int Speed);
        void Gali_JogNSetup(int Speed);
        void Gali_JogNAndCount(int Speed, int Count);

        int  IndexPickLimit;
        bool bScanFlag;
        int  GaliSofDelayCount;
        bool ISNormal();
        bool ISZ1Up_Z2Down();
        bool ISZ1Down_Z2Up();
        bool ISZ1Up_Z2DownNoWait();
        bool ISZ1Down_Z2UpNoWait();

        bool Gali_ReadEncoderInRandge(long checkpos);
        bool Gali_ReadEncoderInRandgeNoWait(long checkpos);
        bool Gali_ReadEncoderOver(long checkpos);
        bool Gali_ReadEncoderMaxRandge(long checkpos);
        bool Gali_ReadEncoderInRandgeMinLimit(long checkpos);
        int  iGali_SingalHomeTask;
        int  iGali_FindZPhaseTask[4];                                           //Ifor 20170817 (wei) Find Z Phase       //Isaac 20201110 : Index Y find motor phase
        bool GalilTwoY_Move(int YPos, int Y2Pos, int Speed, AnsiString sFunc="");
        bool IsCanMove();
        bool Gali_ReadEncoderBelowCheckHeight(long checkpos);

        bool Gali_nnMode_Z1Z2_Down(int Speed, bool TMode);                      //2012-10-22  Dell for HT9046LS
        bool Gali_nnMode_Z1Z2_Up(int Speed, bool TMode);
        bool ISZ1Down_Z2Down();
        bool ISZ1Up_Z2Up();

        void GetRealPos(int *iPos);
        int  CompareEncoderPos(int iPos, int iGap);                             //Isaac 20201217 : 若齒輪比大於1，換算有機會和目標位置差1條
        int  CompareCommandPos(int iPos, int iGap);                             //Isaac 20201217 : 若齒輪比大於1，換算有機會和目標位置差1條
        int  iOldPos;
        bool fCMD;
        void PCIL132_SetPos(int Pos);
        void PCIL132_StopMotor();
        void PCIL132_ResetPos();
        void JogP(int Speed);
        void JogN(int Speed);
        void ServoOnOff(bool IsOn);
        int  ReadEncoderPos();
        int  iEncoderPos;
        void InitMOTParameter();
        void SetADCRate(int Scale);
        int iCheckStatusCT;
        int SimulateSpeed;
        bool GaliScanServo;
        void SetArmMaxSpeed();
        bool bZ1Z2Exute;                                                        //kevin 20110630 判斷目前是否已下4軸同動命令 避免因暫停而執行一次
        bool bCheckEncoderEveryTime;
        bool bShowMotorMove;                                                    //Steven 20111116 : 為了不要include main.h

        bool IsStartGali_Pr();                                                  //Chunghung 20131111 add
        void StartGali_Pr();                                                    //Chunghung 20131111 add
        void EndGali_Pr();                                                      //Chunghung 20131111 add
        long GetGali_Pr_Result();                                               //Chunghung 20131111 add
        void SetGali_Pr_ER(long ERA,long ERB,long ERC,long ERD);                //Chunghung 20131111 add
        void GetGali_Pr_ER(long &ERA,long &ERB,long &ERC,long &ERD);            //Chunghung 20131111 add

        int  GetLockCount();                                                    //Steven 20210825 : 吹氣完成才可以歸零
        void Lock(AnsiString MotorAlias, AnsiString FunctionName, int Task);    //Steven 20210825 : 吹氣完成才可以歸零
        void UnLock(AnsiString MotorAlias, AnsiString FunctionName);            //Steven 20210825 : 吹氣完成才可以歸零
        void ClearLock();                                                       //Steven 20210825 : 吹氣完成才可以歸零
        AnsiString  GetLockString(int Index);                                   //Steven 20210825 : 吹氣完成才可以歸零
        bool CheckYPosWhenZDown(int Pos,int iOrgPos,AnsiString asErrorFunc);    //jimmychiu 20220815 for index arm checking y pos before z down
        bool CheckArmPosInRange(int iNowPos,int iMin,int iMax);                 //jimmychiu 20220815 for index arm checking y pos before z down
        bool CheckArmPosArrival(int iNowPos,int iDestination,int iTolerance);   //jimmychiu 20220815 for index arm checking y pos before z down
        bool CheckIndexYPos(bool Front);                                        //Ztex 2023.05.11 Add CheckIndexY
        bool Check_SHUTTLE_FLOODGATE_Staste(int iIndex);                        //Ztex 2023.06.02 Add Check_SHUTTLE_FLOODGATE_Staste

        void TrayArmInitial();
        bool Check_Y1Y2_TargetPosWillCrash(int iY1_RelPos, int iY2_RelPos);     //JerryYang 20250822 : fix index y保護  //JerryYang 20241129 : 計算Index Y移動後點位是否兩支ARM同時在SOCKET
        bool CheckY1Y2TargetPos(int Pos, AnsiString asErrorFunc);
        bool Check_Y1_TargetPosWillCrash(int iY1TarPos,AnsiString sFunc);                           //JerryYang 20241129 : 計算Index Y移動後點位是否兩支ARM同時在SOCKET
        bool Check_Y2_TargetPosWillCrash(int iY2TarPos,AnsiString sFunc);                           //JerryYang 20241129 : 計算Index Y移動後點位是否兩支ARM同時在SOCKET
        bool Check_Y1_TargetPosInTeachPos(int iY1TarPos,AnsiString sFunc);
        bool Check_Y2_TargetPosInTeachPos(int iY2TarPos,AnsiString sFunc);
        bool iLastRotatorDirP;                                                  //RogerYang 20260113 : 用來紀錄Rotator最近一次旋轉方向
        int GetRotatorBacklash(int iGoalPos, bool bInRotator, int iTechData=0); //RogerYang 20260113 : Rotator新增背隙補償
};
//-------------
class TTrayMotor: public TMyMotor
{
    private:

    protected:
        bool  fHTary;
        TTMyTray *pHTray;
    public:
        __fastcall TTrayMotor();
        TMyTray Tray;
        int   iDirection;                                                       //Steven 20170302 (wei) : FIFO MODE
        int   iSiteCount;                                                       //Steven 20170302 (wei) : FIFO MODE

        bool  bShowSiteMap;                                                     //Steven 20170302 (wei) : FIFO MODE
        bool  fHasTray;

        void  SetHTrayPanel(TTMyTray *ptr);

        void  InitNewTray(int data, bool bShowSiteMapFlag, AnsiString Func);
        void  InitEmptyTray(AnsiString Func);
        void  ClearTray(AnsiString Func);

        void  SetTray(int data, AnsiString Func);

        void  MoveTrayAllItem(class TTrayMotor *Source);                        //Sam 20240108 : 新增退 Tray 時顯示裡面有多少 Error Bin
        bool  HasIC();
        bool  HasRealIC();
        bool  FullIC()
        {
              return Tray.FullIC();
        }
        void  SetTraySingleData(int x, int y, int data, int iTarget=0);
        void  SetTraySiteMap(int x, int y, int iSiteMap);                       //Steven 20220510 : For QTI SD Device Map Function
        void  SetTrayBinData(int x, int y, int data, AnsiString iInfo);
        void  SetNullIcToHasNullIc();
        void  SetNullIcToHasIc();                                               //Sam 20240424 : 修正 P54 Fix 分盤功能失效問題。
        void  SetTrayBufferSingleData(int x, int y, int data);                  //Q_Q 2010.09.29 SKIP功能

        bool  UpHalfIsFull();
        bool  DownHalfIsFull();
        int   WhichBufferIsFull();                                              //JerryYang 20221215 : add Magazine
        void  Refresh();

        bool SearchHasEmpryToPlace(int iY);                                     //jou 2010-03-05 start : for TestIF.bSearchLastMode use
        bool TrayFeedHasIC();                                                   //kevin 20130508 Trayfeed 判斷fix tray 有無放ic 有ic發alrm取出
        bool HasCleanPad();                                                     //kevin 20150505 判斷裡面有沒有CLEAN PAD
        int  HowManyDevice(int iType);                                          //Ifor 20160829 add
        int  HowManyDevice();                                                   //Steven 20190627 : 計算Tray上Device數量
        void SetHasNullIcToNullIc();                                            //Frank 20170822 (wei): 清空Rotate資料
        bool HasOnlyDataICAndNullIC(int DataType);                              //JimmyChiu 20220908 add Pickup Error Placement
        void MoveTrayData( TTrayMotor &TrayMotor );                             //JerryYang 20221215 : add Magazine
        int iIsCoverTray;
        AnsiString sUnloaderAlarmMsg;                                           //Jimmychiu 20240902 : Need Alarm After Unloader Full
        AnsiString sTrayID;
};
#define MAX_TRAY_MOTOR 300
extern class TTrayMotor MOT[MAX_TRAY_MOTOR];                                               //Steven 20210621 : Modify from 200 to 210  //KenHsieh 20210813 : Modify from 210 to 215

//Steven for HT1032
//==>
bool InArmContinuousMove_9045(int X, int Y, int Vari[X_PITCH_COUNT], int YVari, bool ZDownSel[MAX_ARM_Row][MAX_ARM_Col], int iZPos[MAX_ARM_Row][MAX_ARM_Col], bool ZNeedDown, bool bLoader=false);
int  InArmPitchMove(int Vari[X_PITCH_COUNT], int YVari, bool bInit=false);
void SetInArmPitchSpeed(bool bMoveClose);
extern int iInArmZMoveTask;
bool InArmCynMove(bool ZDownSel[MAX_ARM_Row][MAX_ARM_Col]);
bool InArmZMoveDown(bool ZDownSel[MAX_ARM_Row][MAX_ARM_Col], int iZPos[MAX_ARM_Row][MAX_ARM_Col], bool bLoader=false, bool bPreOn=false);
bool InArmZMoveUp(int iZPos, bool bLoader=false);
bool OutArmContinuousMove_9045(int X, int Y, int Vari[X_PITCH_COUNT], int YVari, bool ZDownSel[MAX_ARM_Row][MAX_ARM_Col], int iZPos[MAX_ARM_Row][MAX_ARM_Col], bool ZNeedDown, bool bLoader=false);
bool OutArmPitchMove(int Vari[X_PITCH_COUNT], int YVari, bool bInit=false);
extern int  iOutArmZMoveTask;
bool OutArmCynMove(bool ZDownSel[MAX_ARM_Row][MAX_ARM_Col]);
bool OutArmZMoveDown(bool ZDownSel[MAX_ARM_Row][MAX_ARM_Col], int iZPos[MAX_ARM_Row][MAX_ARM_Col], bool bLoader=false, bool bPreOn=false);
bool OutArmZMoveUp(int iZPos, bool bLoader=false);
//<==
//Steven for HT1032

//RogerYang 20250510 Add for 9046AU
//==>
extern bool bPauseSortMotor;
bool SortArmContinuousMove(int X, int Y, int Vari, bool ZDownSel[MAX_ARM_Row][MAX_ARM_Col], int iZPos[MAX_ARM_Row][MAX_ARM_Col], bool ZNeedDown, bool bLoader=false);
bool SortArmPitchMove(int Vari);
void InitPCIL112_SortArmXYMoveTask();
int  PCIL112_SortArmXYMove(int iXComPos, int iYComPos);
AnsiString sSortArmZHomeState();
bool SortArmZMoveDown(bool ZDownSel[MAX_ARM_Row][MAX_ARM_Col], int iZPos[MAX_ARM_Row][MAX_ARM_Col], bool bLoader=false, bool bPreOn=false);
bool SortArmZMoveUp(int iZPos, bool bLoader=false);
//<==
//RogerYang 20250510 Add for 9046AU

bool CatchMgzTrayMove(int FinalPos);                                            //Sam 20221116 : Magazine TrayArm 自動補 Tray

void OpenPCI132Card(bool bfirst);                                               //ChungHung 20111101 add 24v壓降
void ServoOnAllMOT();
int  OutArmZSafe(int iFlag);                                                    // -1 = safe
int  InArmZSafe(int iFlag);                                                     // -1 = safe
int  SortArmZSafe(int iFlag);  // -1 = safe                                     //RogerYang 20250510 Add for 9046AU
AnsiString sInArmZHomeState();                                                  //Sam 20230707 : 新增 InOutArm Z Home前Home sensor 狀態
AnsiString sOutArmZHomeState();                                                 //Sam 20230707 : 新增 InOutArm Z Home前Home sensor 狀態

void InitPCIL112_InArmXYMoveTask();                                             //jou 2011-04-03 start
int  PCIL112_InArmXYMove(int iXComPos, int iYComPos);
void InitPCIL112_OutArmXYMoveTask();
int  PCIL112_OutArmXYMove(int iXComPos, int iYComPos);

int CheckOutArmZNeedHome();                                                     //ChungHung 20140605 add Fix Shuttle hit In/OutArm
int CheckInArmZNeedHome();
void ShowIndexMotorError(AnsiString Debug="", bool bCompareTeachPos=false);

AnsiString __fastcall SaveLog(AnsiString FileNamePath);                         //kevin 20150909 儲存LOG  FileNamePath 檔案路徑 回傳整個檔案日期檔名
void RecordIndexPositionError(AnsiString sFunction, bool A1, bool A2, bool A3, bool A4, long *Pos);
bool TrayArmContinuousMoveForOCR(int X, int Y);                                 //wei 20150923
bool TrayMoveHome();                                                            //wei 20150923
bool ShuttleSensorContinuousMove(int SH1, int SH2, bool Result);                //wei 20160914 Auto Shuttle Sensor
int CheckOutArmDestory();                                                       //kevin 20180119
void RecordIndexPosition(int iArm,int Part);                                    //Isaac 20200922 : 紀錄indexArmY encoder值和command值
void EncoderTeachingMaxMinCount(int iRecordArm);                                //Isaac 20201012 : 計算Encoder和commandpos/Teaching的差值
void InitialMaxMinValue(AnsiString StrTask="");                                 //Isaac 20201012 : 計算Encoder和commandpos/Teaching的差值，歸零
void TrigerIndexAxisHome();                                                     //Isaac 20201012 : index Y超過範圍，做一次Tmode，初始化，開始自動校正
extern int ZSafePos;                                                            //Steven 20220207 : 整合In/Out Arm Z軸安全位置
extern int ZlimitPos;                                                           //Ifor 20221026 add: In/Out Arm 吸嘴極限位置
extern bool TrayArmMotorMove(int p, bool bCheckPos=true);                       //Sam 20241206 : Tray Arm 新增 Teach 保護
#endif
