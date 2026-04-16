//---------------------------------------------------------------------------
#ifndef acatchcassetteH
#define acatchcassetteH
//---------------------------------------------------------------------------
//Sam 20190112 LM
//==>
void DoLoadCassette();              //Sam 20190112 LM MR<>LM切換

//CatchCassette
void DoCatchCassetteLM();                                                       //CatchCassette 主流程
int iWhichCABufferCatch();                                                      //去哪抓
int iWhichCABufferPlace();                                                      //去哪放

//LoadRobot
void InitialLoadRobotCassetteTask();
void DoLoadRobotCassette();                                                     //LoadRobot 主流程
int iWhichLoadPortPick();                                                       //去哪抓
int iWhichLoadPortPlace();                                                      //去哪放
void InitialLoadRobotCassettePickTask();
bool DoLoadRobotCassettePick(int iPortTarget,bool bPick=true);                  //LoadRobot 取放子流程
bool MLoadRobotZCanSafeUpMove();
bool MLoadRobotZCanSafeMove();
bool HasICUnderLoadPort();

//UnloadRobot
void InitialUnloadRobotCassetteTask();
void DoUnloadRobotCassette();
int iWhichUnloadPortPlace();                                                    //去哪放
void InitialUnloadRobotCassettePickTask();
bool DoUnloadRobotCassettePick(int iPortTarget,bool bPick=true);                //LoadRobot 取放子流程
bool MUnloadRobotZCanSafeUpMove();
bool MUnloadRobotZCanSafeMove();
bool HasICUnderUnloadPort();

int iGetMMCABuffer(int BufferTarget);
bool bCheckBufferIsFull();                                                      //Buffer 是否塞滿
int iMatchBufferType(int BufferType);

bool CheckLoadRobotHasCassette();                                               //LoadRobot HasCassette 只檢查硬體
bool CheckLoadRobotHasCassetteSensor();                                         //LoadRobot HasCassette 檢查硬體 & 軟體
bool CheckLoadRobotNoCassetteSensor();                                          //LoadRobot NoCassette 檢查硬體 & 軟體
bool CheckLoadRobotSensor();                                                    //LoadRobot 置偏檢查硬體

bool CheckLoadPortHasCassette(int iPort);                                       //LoadPort HasCassette 只檢查硬體
bool CheckLoadPortHasCassetteSensor(int iPort);                                 //LoadPort HasCassette 檢查硬體 & 軟體
bool CheckLoadPortNoCassetteSensor(int iPort);                                  //LoadPort NoCassette 檢查硬體 & 軟體
bool CheckLoadPortSensor(int iPort);                                            //LoadPort 置偏檢查硬體
int iCheckAllLoadPortSensor();                                                  //全部 LoadPort 置偏檢查硬體 -1~-4 。

bool CheckUnloadRobotHasCassette();                                             //UnloadRobot HasCassette 只檢查硬體
bool CheckUnloadRobotHasCassetteSensor();                                       //UnloadRobot HasCassette 檢查硬體 & 軟體
bool CheckUnloadRobotNoCassetteSensor();                                        //UnloadRobot NoCassette 檢查硬體 & 軟體
bool CheckUnloadRobotSensor();                                                  //UnloadRobot 置偏檢查硬體

bool CheckUnloadPortHasCassette(int iPort);                                     //UnloadPort HasCassette 只檢查硬體
bool CheckUnloadPortHasCassetteSensor(int iPort);                               //UnloadPort HasCassette 檢查硬體 & 軟體
bool CheckUnloadPortNoCassetteSensor(int iPort);                                //UnloadPort NoCassette 檢查硬體 & 軟體
bool CheckUnloadPortSensor(int iPort);                                          //UnloadPort 置偏檢查硬體
int iCheckAllUnloadPortSensor();                                                //全部 UnloadPort 置偏檢查硬體 -1~-4

bool CheckTrayLoaderNoTraySensor();                                             //TrayLoader NoTray 檢查硬體 & 軟體
bool CheckTrayEmptyNoTraySensor();                                              //TrayEmpty NoTray 檢查硬體 & 軟體
bool CheckCAArmHasCassette();                                                   //CassetteArm HasCassette 檢查硬體
bool CheckCAArmHasCassetteSensor();                                             //CassetteArm HasCassette 檢查硬體 & 軟體
//<==
//Sam 20190112 LM




int WhichBufferCanCatch(int Data, int iCheckLot=0);
bool WhichEmptyCanCatch(int Data);
bool CheckHasCassette(int Buffer);
bool CheckHasCassetteSensor(int Buffer);
bool CheckLoadPortHasCassetteSensor(int iPort=0);



int WhichBufferCanPlace();
void ShowCassetteTrayData();
void ShowTrayCountData();
void ShowCassetteData(int iFunction, AnsiString asCassetteLot="");
void SetCassetteTray(int Mot, int Mot1);
AnsiString CheckNewLotID2();
AnsiString CheckNewLotID1();

bool C_Catch_Cassette_Push(int Pos, int CylinderPos, bool bInitial=false);
bool C_Catch_Cassette_Pop(int Pos, int CylinderPos, bool bInitial=false);

bool MCasArmXCanSafeMove();                                                     //判斷z在上
bool MCasArmXCanSafeMoveBuffer1();
void InitialCatchCassetteTask();
void DoCatchCassette();



bool DoCACatchFromBuffer(int BufferTarget);
bool DoCAPlaceToBuffer(int BufferTarget);
void InitialCAPlaceToBufferTask();
void InitialCACatchFromBufferTask();

bool MElevatorCanSafeUpMove();
bool MElevatorCanSafeMove();




void InitialLoadPortCassetteTask();
void DoLoadPortCassette();


void InitialLoadPortElevatorUpTask();    //MR
bool DoLoadPortElevatorUp();             //MR
void InitialLoadPortElevatorDownTask();  //MR
bool DoLoadPortElevatorDown();           //MR

extern bool bBufferHasNewCassette;
extern bool bBufferHasOldCassette;
extern bool bUnloadRobotHasCassette;

bool MTrayBracketCanSafeMove();
void InitialTrayBracketTask();
void DoTrayBracket();
void InitialTrayBracketUpTask();
bool DoTrayBracketUp();
void InitialTrayBracketDownTask();
bool DoTrayBracketDown();


bool bCheckSensor(int Target);
void InitialCatchStackedTrayTask();

void DoCatchStackedTray();

extern bool bLoadNeedTray;
extern bool bLoadNeedFailTray;
extern bool bLoadHasStackedTray;
extern bool bEmptyNeedTray;
extern bool bEmptyHasStackedTray;
//extern bool bAutoFullTray;

extern bool bBuffer6NeedCassette;               //Buffer6 需要 Cassettle Empty Tray
extern bool bBuffer6NoNeedCassette;             //Buffer6 有 Cassettle Empty Tray 不能被搬走
extern bool bLoadPortHaveCassette;
extern bool bBuffer1HaveCassette;
extern bool bBuffer6CanUp;

extern void InitialATRCatchStackedTrayTask();
extern bool DoATRCatchStackedTray();
extern bool bLoadPortHasCassette;
//extern int iMRStartMode;

extern bool bBuffer6CombineTray;
extern bool bBuffer6SeparationTray;
extern bool DoCassetteToUnLoadPort();
extern void InitialCassetteToUnLoadPortTask();
extern int WhichAutoCanCatch();

extern bool bEmptyFull;
extern bool DoNotMRStop();
extern bool CheckMRSafePos();
extern bool bMRQAMode;
extern bool bMRQAModePassBin;

extern bool bBuffer6NeedEmpty;
extern bool bCassetteMessage;
extern bool bSTArmMove[3];

extern int iCatchCassetteTask;
extern int iCACatchFromBufferTask;
extern int iCAPlaceToBufferTask;
extern int iLoadPortCassetteTask;
extern int iLoadPortElevatorUpTask;
extern int iLoadPortElevatorDownTask;
extern int iCatchStackedTrayTask;
extern int iSTCatchFromBufferTask;
extern int iSTPlaceToBufferTask;
extern int iTrayBracketTask;
extern int iTrayBracketUpTask;
extern int iTrayBracketDownTask;
extern int iRFIDReadWaitDataTask;

void DoManualCatchCassette();
void InitialManualCatchCassetteTask();
extern bool bSTMove;
void InitialRFIDReadWaitDataTask();
extern bool DoRFIDReadWaitData();
extern bool CheckNoCassetteSensor(int Buffer);
#endif
