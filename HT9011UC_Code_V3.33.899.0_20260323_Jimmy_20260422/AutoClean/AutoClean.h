//---------------------------------------------------------------------------
#ifndef AutoCleanH
#define AutoCleanH
enum eWhichShuttle{euShuttle1   =0,
                   euShuttle2   =1};
void SetAutoCleanICCount(bool Work);//,int iCount=0);   //ChungHung 20141027 add for SCK want to record AutoClean_pad count
void InitialAutoCleanAllTask();  //Sam 20230504 : 整理 InitialAutoCleanTask
void InitialAutoCleanTask();
void InitialShuttleAutoCleanTask();
void InitialIndexAutoCleanTask();
void InitPickFromShuttleTask();

//ChungHung 20130402 alter
int SearchCleanNum();           //確認目前的Pad最少做了幾次Clean
void SearchiAutoCleanNum();      //Steven 20171212 (Wei) : 確認目前正要吸取的的Pad位置
bool RestoreCleanKitData(); //ChungHung 20130628 add 回復已被InArm吸走的CleanKit上的IC計數  請勿亂用

void DoAutoCleanKit();
void DoShuttle1AutoClean();
void DoShuttle2AutoClean();
void DoIndexAutoClean();
bool DoInArmPineRelease();  //Ｚ馬達吸不到ＩＣ記錄位置且馬達可以移動
void EnableAutoclean(bool Manual);
void __fastcall AutoCleanWriteData(AnsiString Str, int Data);
void InitialSet();

void ResetAutoClean();
//void ChangeAutoCleanCount();    //Ifor 20171024 : add 可更改Auto Clean 計數       //JerryYang 20180508 (jou) : Mark掉,由SetAutoCleanICCount函式取代
//void SetCleanICToHasIC(bool bInitCount);
void SetAutoCleanStringGrid(int X, int Y, AnsiString Str);                          //Steven 20180524 : Fixed for clean count
extern bool RunAutoCleanByArmPickArm2Test();                                               //Jimmychiu 20230710 : Auto Clean 跑 Arm1 下壓 arm2 測試
extern void CleanSetSpeed(bool bBackup);
void ReadWriteAutoCleanCount(bool bRead, bool bReset=false);                        //Steven 20180524 : Fixed for clean count
extern int iAutoCleanPickFromCleanKitStageTask; //jou 2012-05-22
extern int iAutoCleanPlaceToShuttleTask;
extern int CheckShuttleSensor_Clean(eWhichShuttle iSht, bool alarmflag);//kevin 20150417 add
extern bool MoveInArmZ_Shuttle_Pick(eWhichShuttle iSht, int iSelRow);//kevin 20150430
extern int GetShuttleState(eWhichShuttle iSht, bool bPick);
extern int iInXPos;
extern int iInYPos;
extern void SetAutoCleanTrayPosition(); //Steven 20210825 : 重新整理成Function
//extern bool MoveSuckDataToShuttle_AutoClean(int iShuttleRowKit, bool bActiveSuck);
extern void MoveCleanKitDataToSuck(int j, int j2, int iWhichRow);
extern bool DoPickFromShuttle(eWhichShuttle iSht, int iSelRow);
extern void DoPlaceToKitSwapData(bool bPick, int iSuckRow, int iSuckCol, int iKitRow, int iKitCol);
extern bool Special_2X6_Tray_XItem7();      //Sam 20250712 : 新增特殊流程 2X6 AutoClean Tray XItem=7
void GetInarmSuckRow(int iShtRowKit,int &isuckRow,int &ikitStep);
#endif
