//---------------------------------------------------------------------------
#ifndef acatchtrayH
#define acatchtrayH
//---------------------------------------------------------------------------
void DoCatchTray();
void InitialCatchTrayTask();
bool IsTrayArmMoveAvoidOutArmCrash();
int WhichAutoNeedTray();
bool IsTrayArmCatchTrayFail();                                                  //JerryYang 20200926 包成函式,偵測tray arm夾tray是否異常
extern bool IsTrayArmAtEmptyOrColor();
void CatchFromLoaderSetItemData();                                              //Sam 20220712 : 整合 CatchFromLoader SetItemData
void PlaceToBufferSetItemData();                                                //Sam 20221027 : 整合 CatchTray SetItemData
void CatchTraySetItemData();                                                    //Sam 20221027 : 整合 CatchTray SetItemData
bool DoLoadCarRotArmReadRFID(bool bAlarm=false);
void InitialLDRotArmReadRFIDTask();
//ChungHung 20140624 add AutoRetest catch Tray use 2 Output
bool C_CatchTray_Fix_Puch(bool bInitial=false);
bool C_CatchTray_Fix_Pop(bool bInitial=false);
bool bCheckNoCatchEmptyTrayToAuto();    //Frank 20170221 add

bool DoSlapTray(bool bInit=false); //ChungHung 20140701 add AutoRetest
bool MTrayXCanSafeMove();          //kevin 20170619 (wei) add
extern int CatchTrayTask;
extern int iCatchFromLoaderTask;
extern int iPlaceTrayToAutoTask;
extern int iPlaceToBufferTask;
extern int iSlapTrayTask;
//extern bool bTrayMode;                            //Sam 20221027 : 整合 P24 功能 Mark  //Alick 20160719 SIGURD用旗標
//extern bool bforceToMoveColorTrack;               //Sam 20221027 : 整合 P24 功能 Mark  //Sam 20220530 : 當 Loader 發生 Skip/Edit 時，此盤做完後搬到 Empty 軌道後，會收盤起來並報警提示人員收盤(連兩盤)
//extern bool bColorMoveOutAlarm;                   //Sam 20221027 : 整合 P24 功能 Mark  //Sam 20220530 : 當 Loader 發生 Skip/Edit 時，此盤做完後搬到 Empty 軌道後，會收盤起來並報警提示人員收盤(連兩盤)
extern void InitialTrayIDTask();                    //wei 20161219 (Steven) Tray Mapping
extern void InitialTrayID2Task();                   //wei 20180808 MR Tray ID 2
extern void InitialTrayMapTask(int iTrayMap);       //Sam 20200323 : Modify Tray Function //wei 20161219 (Steven) Tray Mapping
extern void InitialDeviceRemainTask();              //wei 20170317 (steven) Device Remain 殘料檢測
extern bool IsOutArmSafe();
extern void AddFixTrayCount();
extern void AddTrayCount(int iAuto);
extern void ClearAllTrayCount();
extern void SwapTrayID();
#endif
