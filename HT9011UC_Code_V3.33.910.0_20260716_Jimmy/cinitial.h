//---------------------------------------------------------------------------
#ifndef cinitialH
#define cinitialH

#include "cmydef.h"         //KenHsieh 20210813 : add CCD AUTO ALIGNMENT
#include "mykitsuck.h"      //KenHsieh 20210813 : add CCD AUTO ALIGNMENT
//---------------------------------------------------------------------------

void InitialHandler();
void InitCylinder();
void InitSucker();
//TColor GetICColor(int Type);
void InitialMotorParameter();
extern int SpeedScale;
void SetMotorSpeed();
int GetTrueMotorScaleSpeed(int index,int sp);
bool SetWorkParameter();
bool ReadTechData();
bool ChangeRunParameter();
void SetSuckRetryCount();
//void SetMotorScaleSpeed(int Index,int ScaleSpeed);
void SetAllMotorServoOn();
void InitialMachine();
//----- by dell ccd realtime-------------
void SaveMachineRecord(bool bSpare=false);
void LoadMachineRecord(bool bSpare=false);
//---------------------------------------
void SetSingleMotorSpeed(int MotorIndex);
//bool SetWorkParameter();
extern void GetIndexParm();
void SetAllMotorSpeed(int speed);
void SetTechDataToProd();
void SetTechDataToProd_Yield();
bool CheckFix3FullPlaceTechData();                                              //Steven 20130126 : Fix3滿盤功能
void InitialSensor();
void DoSetupSystemToProd();
void UpdateMyKitSuckDelayTimeToProd();                                          //Steven 20250319 : 針對OnDelayTime轉換包成function
void CheckNewFunction();
void ShowMainScreenPresure(int index);
void ChangeSite();
//void SetArmRowCount();
void SetOffsetToTech();
//void SetMyKitSuckItemAmount(int iSiteType);
void SetMyKitSuckItemAmount();
void InitShuttleThreadParameter();                                              //Steven 20110407
void SetInArmSpeed(bool bShow, bool bReset=false);                              //Steven 20140909 : Clean Out後要重置Speed
void SetOutArmSpeed(bool bShow);
void SetSortArmSpeed(bool bShow);                                               //RogerYang 20250515 Add for 9046AU
void InitialHeaterDoor();
void SetMotorScaleSpeed(int Index, int ScaleSpeed);
void SetMotorAccelSpeed(int Index, int ADCSpeed);
void SetUnloaderInfoFile(int iUnloader);                                        //ChungHung 20150205 add for ATK
void LoadUnloaderInfo();                                                        //ChungHung 20150205 add for ATK
int GetSHCHKPos(int iSite, int iCenterBase);                                    //JerryYang 20160728
void InitialHT9045SModule();                                                    //Steven 20161117 : for HT-9045S
bool CompareTechData();                                                         //Steven 20220221 : 針對Teaching值做防呆
//void GetShuttleSize(int &XItem, int &YItem, int &PitchX, int &PitchY );         //ChungHung 20210113 add for Alignment CCD
void InitialSafeDoor();
void SetHangupMaxTime();                                                        //Wei 20230224 : 重置死機時間
int IsNNMode();                                                                 //Steven 20240916 : NN mode 判斷
int GetInArmSuckBaseHeight(int iRow, int iCol);
int GetOutArmSuckBaseHeight(int iRow, int iCol);
void TestTimeSetSpeed();                                                        //Ztex 2024.08.11 Add Test Time Set Speed

#endif
