//---------------------------------------------------------------------------

#ifndef cAuthorityH
#define cAuthorityH

#include "MachineType.h"
//---------------------------------------------------------------------------
extern bool authMainForm[12];
extern bool authTool[9];
extern bool authMaintaince[9];
extern bool authConfig[5];
extern bool authConf[14];         //JerryYang 20180511 12 -> 14
extern bool authCounterClr[9];
extern bool authObserver[2];
extern bool authBinSetting[8];    //Jou 20141110 : 3 --> 5  //Ifor 20170316 (wei) 5--->8
extern bool bAuthCriticalPara[26];//JerryYang 20220310 : ATP要求鎖定critical parameter
//extern bool authSetup[7];
extern bool authSetup[8]; //ChungHung 20130118 add OCR - 關閉要密碼
extern AnsiString funLimit[39];
extern int  iSetupSiteMapping;
extern AnsiString asRunStartModePassword;   //jou 20170214 (Steven) : RunStartMode need keyin Password

extern AnsiString BinSetting[eBinTypeTotal];
extern AnsiString funCriticalPara[];//JerryYang 20220310 : ATP要求鎖定critical parameter

void __fastcall GetMainAuth();
void __fastcall GetConfAuth();
void __fastcall GetCountClrAuth();
void __fastcall GetLimitAuth();
void __fastcall GetDummyVacuum();
void __fastcall GetObserAuth();
void __fastcall GetSetupAuth();
void __fastcall GetBinSettingAuth();    //Steven 20120330 : FT跟RT的Enable
void __fastcall ChangeCompomentEnabled(TWinControl *PCtrl , bool bEnable, bool bMustEnable=false);
AnsiString __fastcall CheckFile(AnsiString szDir, AnsiString str);
void __fastcall GetCriticalParaAuth();  //JerryYang 20220310 : ATP要求鎖定critical parameter

#endif
