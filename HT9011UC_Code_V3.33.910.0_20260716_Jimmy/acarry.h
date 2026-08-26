//---------------------------------------------------------------------------
#ifndef acarryH
#define acarryH
//---------------------------------------------------------------------------
void Initial_Auto_SHT1_Task();
void Initial_Auto_SHT2_Task();
void Do_Auto_SHT1();
void Do_Auto_SHT2();
void Do_Auto_SHT3();                                                            //RogerYang 20250514 Add for 9046AU
void Do_Auto_SHT1AsOutArmIsRotater();
extern int AutoSHT1Task;
extern int AutoSHT2Task;
extern int AutoSHT3Task;                                                        //RogerYang 20250510 Add for 9046AU

extern bool IsFLCarrKitAllHasIC();                                              //ChungHung 20111230 add
extern bool IsBLCarrKitAllHasIC();                                              //ChungHung 20111230 add

void InitialShuttleThread();
void StartShuttleThread();
void EndShuttleThread();
void StartDetectMotorSensor(int SelSHT);
bool CheckShuttleSensorBroken_1(bool bRefreshCheck, bool bRight);
bool CheckShuttleSensorBroken_2(bool bRefreshCheck, bool bRight);

bool DoInOutARM_SHT_MoveSafe(int iShuttle);                                     //kevin 20161005 add
bool DoINARM_SHT_MoveSafe(int iShuttle);                                        //kevin 20161005 SHUTTLE 移動安全保護
bool DoOutARM_SHT_MoveSafe(int iShuttle);                                       //kevin 20161005
bool DoSortOutARM_SHT_MoveSafe();                                               //RogerYang 20250510 Add for 9046AU
bool DoSortARM_SHT_MoveSafe();                                                  //RogerYang 20250510 Add for 9046AU
bool ShowOutputShuttleICStatus(int iSelSHT, bool bAlarm=true);                  //JerryYang 20200610 Out shuttle lose IC需先讓位再發出alarm
bool ShowOutputShuttleICStatus_M204(int iShuttleNo, bool bAlarm=true);
bool CheckShuttlePos1_M204(bool bCheckHasIC, bool bAlarm=true);
bool CheckShuttlePos2_M204(bool bCheckHasIC, bool bAlarm=true);

extern int DoCheckShuttle1ICByLTC(bool bCheckHasIC);                            //Isaac 20170418 (Steven) 用Y Latch檢測Inshuttle有無IC
extern int DoCheckShuttle2ICByLTC(bool bCheckHasIC);                            //Isaac 20170418 (Steven) 用Y Latch檢測Inshuttle有無IC
int DoCheckShuttle1ICByLTC_AutoLatch(AnsiString &ErrPart, bool bChkData=true);  //KenHsieh 20250722 : InSht sensor 改為2顆，並用Latch 判別疊料以及飛料
int DoCheckShuttle2ICByLTC_AutoLatch(AnsiString &ErrPart, bool bChkData=true);  //KenHsieh 20250722 : InSht sensor 改為2顆，並用Latch 判別疊料以及飛料

extern bool bShuttleHasIC[2][4][8];
extern bool bEnter[2][4][8];
extern bool b1ShuttleMoveToLeft;
extern bool b2ShuttleMoveToLeft;
extern bool b1ShuttleMoveToRight;
extern bool b2ShuttleMoveToRight;

extern bool bSortShtMoveToLeft;                                                 //RogerYang 20250417 for HT9046AU add
extern bool bSortShtMoveToRight;

int GetShuttle1Pitch_9045(int w);                                               //wei 20160914 Auto Shuttle Sensor
int GetShuttle2Pitch_9045(int w);                                               //wei 20160914 Auto Shuttle Sensor
bool DoMoveShuttleSensor();                                                     //wei 20160914 Auto Shuttle Sensor
int CheckShuttlePitch();                                                        //wei 20160914 Auto Shuttle Sensor
extern bool bSHTOfsChangeRight[3];                                              //RogerYang 20250417 for HT9046AU add index //Sam 20221205 : 修正 Shuttle 移動完成後又被修改 Offset 導致 Hang up
extern bool bSHTOfsChangeLeft[3];                                               //RogerYang 20250417 for HT9046AU add index //Sam 20230202 : 修正 Shuttle Left 移動完成後又被修改 Offset 導致 Hang up
extern void CheckShuttle1EncoderPos();                                          //JerryYang 20230131 : add shuttle shift log
extern void CheckShuttle2EncoderPos();                                          //JerryYang 20230131 : add shuttle shift log
bool DoFloodGateClose();                                                        //Ifor 20260401 add:Flood Gate Close
void ResetShtMoveTimeoutWatchdog();                                             //AI(ht9045-staterecord-analysis) 20260525 (RogerYang) : PAUSE/START 邊界重置 Shuttle Move Timeout watchdog
extern bool IsTestZ2NotSafeShuttle2CanNotMove(int &iRetryCT);
#endif
