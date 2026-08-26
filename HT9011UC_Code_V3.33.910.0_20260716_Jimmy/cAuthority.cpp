#include "MachineDefine.h"
#pragma hdrstop
//==============================================================================
// 用來設定該功能或選項是否顯示
// Use "D:\HT9045\config\Security_new.def" to set Enable/Disable of functions.
// Steven 2009-07-31
//==============================================================================

#include "cAuthority.h"
#include "common.h"
#include "cprod.h"
#include "cpublic.h"
#include "cmydef.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
extern int  CUSTOMER_CODE;

bool authMainForm[12];
bool authTool[9];
bool authMaintaince[9];
bool authConfig[5];
bool authConf[14];      //JerryYang 20180511 (Steven) 12 -> 14
bool authCounterClr[9];
bool authObserver[2];
bool authBinSetting[8]; //Steven 20120330 : FT跟RT的Enable          //Jou 20141110 : 3 --> 5  I//Ifor 20170316 (wei) BinSetting 5--->8
//bool authSetup[7];      //Steven 20120405 : Set Up加入Security_new權限
bool authSetup[8];  //ChungHung 20130118 add OCR - 關閉要密碼
bool bAuthCriticalPara[26];  //JerryYang 20220310 : ATP要求鎖定critical parameter
int  iSetupSiteMapping;
AnsiString asRunStartModePassword="12345";          //jou 20170214 (Steven) : RunStartMode need keyin Password

AnsiString SetUpForm[]=     //Steven 20120405 : Set Up加入Security_new權限
{
    "Mode and Pitch",
    "Destroy Mode",
    "Suck Mode",
    "Kit",
    "Shuttle Mode",
    "Site Map",
    "RTC",
    "OCR"
};
AnsiString MainForm[]=      //Main Form Enable/Disable Control
{
    "Tool",
    "Maintance",
    "Offset",
    "Speed",
    "IO",
    "Message",
    "Temperature Set",
    "Tester",
    "RunMode",
    "Start Mode Select",
    "Scanner",
    "Teaching"
};
AnsiString funcTool[]={     //Tool Button Enable/Disable Control
    "Tray Form",
    "Plate Form",
    "Tray Assign",
    "Temp. Offset",
    "Contact",
    "Test IF",
    "Bin",
    "Set Up",
    "Load/Unload"
};
AnsiString funMaintaince[]={    //Maintaince button Enable/Disable Control
    "Builder",
    "Start Mode",
    "C. Select",
    "C. Clear",
    "Temp. Monitor",
    "Tower Light",
    "Password",
    "Configuration",
    "DIO Setting"
};
AnsiString funConfig[]={    //Configuration Tab Enable/Disable Control
    "Soft",
    "Comm",
    "Config",
    "Tray",
    "Hot Plate",
};
AnsiString funConf[]={      //Config Tab Enable/Disable Control
    "Function",
    "Report",               //JerryYang 20180511 (Steven) 修正Config分頁被鎖定對應錯誤的問題
    "Hardware",
    "Index",
    "In/Out Arm",
    "Shuttle",
    "Visible",
    "Tester",
    "Tempture",
    "Count",
    "Network",
    "Tray",
    "In/Out Arm Order"
};
AnsiString funCounterClr[]={    //Counter Clear CheckBox Enable/Disable Control
    "Alarm Data",
    "Tester Category",
    "Scanner Category",
    "Loading Count",
    "Contact Count(Kind)",
    "Contact Count(Total)",
    "Sorting Count",
    "Time Data"
};
AnsiString funLimit[]={     //Offset Limitation
    "Offset XY High",
    "Offset XY Low",
    "Offset Z High",
    "Offset Z Low",
    "Temp High",
    "Temp Low",
    "Heater Gun High",
    "Heater Gun Low",
    "Contact High",     //Steven 20140123 : Contact Height的Offset限制
    "Contact Low",      //Steven 20140123 : Contact Height的Offset限制
    "Shuttle High",     //ChungHung 20150115 add for ATK +/-2 mm
    "Shuttle Low",      //ChungHung 20150115 add for ATK +/-2 mm
    "Contact Cnt Alm",  //JerryYang 20200504 銦片計數限制
    "Offset Unloader Z High",       //Steven 20210317 : 通富微要求unloader獨立設置
    "Offset Unloader Z Low",        //Steven 20210317 : 通富微要求unloader獨立設置
    "Loader Offset XY High",        //JerryYang 20220923 : 矽品蘇州要求offset limit要By區域設定
    "Loader Offset XY Low",
    "Loader Offset Z High",
    "Loader Offset Z Low",
    "HP Offset XY High",
    "HP Offset XY Low",
    "HP Offset Z High",
    "HP Offset Z Low",
    "HP Offset Release Z High",
    "HP Offset Release Z Low",
    "InShuttle Offset XY High",
    "InShuttle Offset XY Low",
    "InShuttle Offset Z High",
    "InShuttle Offset Z Low",
    "OutShuttle Offset XY High",
    "OutShuttle Offset XY Low",
    "OutShuttle Offset Z High",
    "OutShuttle Offset Z Low",
    "Unload Offset XY High",
    "Unload Offset XY Low",
    "Unload Offset Z High",
    "Unload Offset Z Low",
    "Autoclean Contact High",  //JerryYang 20240111 : 矽品二林要求設定auto clean contact height上下限
    "Autoclean Contact Low"
};
AnsiString funObser[]={     //Observer View
    "Temperature Chart",
    "Contact Force Record"
};
AnsiString BinSetting[eBinTypeTotal]={   //Steven 20120330 : FT跟RT的Enable
    "RT Bin",
    "FT Bin",
    "OffLine",
    "RT ART Bin",   //ChungHung 20141002 add for KYEC AutoRetest
    "FT ART Bin",   //ChungHung 20141002 add for KYEC AutoRetest
    "RT MRT Bin",   //Ifor 20170316 (wei) add MRT Mode
    "FT MRT Bin"    //Ifor 20170316 (wei) add MRT Mode
};
AnsiString funCriticalPara[]={      //JerryYang 20220310 : ATP要求鎖定critical parameter
    "Temperature_Setting_Value",
    "Soake_Time",
    "Bin_Setting",
    "Active_Site",
    "Test_Site_Assign_Handling_Mode_Site_Map",
    "Contact_Force",
    "Vacuum_On_Off_Time",
    "Socket_Pitch",
    "Package_Dimensions_Pin_Count",
    "Contact_Parameter",
    "Drop_Wait_Time",
    "Contact_Speed",
    "Double_Device_Height",
    "Temperature_Mode",
    "QA_Sampling_Count",
    "Socket_Sensor",
    "PiggyBackFunction",
    "Index_Unit_Speed",
    "Tray_Form",
    "Plate_Form",
    "Tray_Assign",
    "Test_IF",
    "Yield_Site_To_Site",
    "Yield_Continous_Fail",
    "Yield_Yield",
    "ACC_Function"
};
//---------------------------------------------------------------------------
void __fastcall GetBinSettingAuth()                                             //Steven 20120330 : FT跟RT的Enable
{
    AnsiString szDir=CheckFile(AuthPath, "Security_new.def");
    authBinSetting[0]=CheckAndReadIniData(szDir, "Bin Setting", BinSetting[0], 1);
    authBinSetting[1]=CheckAndReadIniData(szDir, "Bin Setting", BinSetting[1], 1);
    authBinSetting[2]=CheckAndReadIniData(szDir, "Bin Setting", BinSetting[2], 1);
    authBinSetting[3]=CheckAndReadIniData(szDir, "Bin Setting", BinSetting[3], 1);  //ChungHung 20141002 add for KYEC AutoRetest
    authBinSetting[4]=CheckAndReadIniData(szDir, "Bin Setting", BinSetting[4], 1);  //ChungHung 20141002 add for KYEC AutoRetest
    authBinSetting[5]=CheckAndReadIniData(szDir, "Bin Setting", BinSetting[5], 1);  //Ifor 20170316 (wei) add MRT Mode
    authBinSetting[6]=CheckAndReadIniData(szDir, "Bin Setting", BinSetting[6], 1);  //Ifor 20170316 (wei) add MRT Mode
}
//---------------------------------------------------------------------------
void __fastcall GetObserAuth()
{
    AnsiString szDir=CheckFile(AuthPath, "Security_new.def");
    authObserver[0]=CheckAndReadIniData(szDir, "Observer", funObser[0], 0);
    authObserver[1]=CheckAndReadIniData(szDir, "Observer", funObser[1], 0);
}
//---------------------------------------------------------------------------
void __fastcall GetLimitAuth()
{
    AnsiString szDir=CheckFile(AuthPath, "Security_new.def");
    if(CUSTOMER_CODE==CC_ASE_CL)        //JerryYang 20250120 : modify
    {
        InputLimit.iOffsetXYHigh=CheckRange(CheckAndReadIniData(szDir, "Input Limit", funLimit[0],  5      ), 0, 10);
        InputLimit.iOffsetXYLow =CheckRange(CheckAndReadIniData(szDir, "Input Limit", funLimit[1], -5      ), -10, 0);
        InputLimit.iOffsetZHigh =CheckRange(CheckAndReadIniData(szDir, "Input Limit", funLimit[2],  3      ), 0, 10);
        InputLimit.iOffsetZLow  =CheckRange(CheckAndReadIniData(szDir, "Input Limit", funLimit[3], -3      ), -10, 0);
    }
    else
    {
        InputLimit.iOffsetXYHigh        =CheckAndReadIniData(szDir, "Input Limit", funLimit[0],  5      );
        InputLimit.iOffsetXYLow         =CheckAndReadIniData(szDir, "Input Limit", funLimit[1], -5      );
        InputLimit.iOffsetZHigh         =CheckAndReadIniData(szDir, "Input Limit", funLimit[2],  3      );
        InputLimit.iOffsetZLow          =CheckAndReadIniData(szDir, "Input Limit", funLimit[3], -3      );
    }

    InputLimit.iOffsetUnloaderZHigh =CheckAndReadIniData(szDir, "Input Limit", funLimit[13], InputLimit.iOffsetZHigh);  //Steven 20210317 : 通富微要求unloader獨立設置
    InputLimit.iOffsetUnloaderZLow  =CheckAndReadIniData(szDir, "Input Limit", funLimit[14], InputLimit.iOffsetZLow);

    InputLimit.iTempHigh            =CheckAndReadIniData(szDir, "Input Limit", funLimit[4],  60.0   );
    InputLimit.iTempLow             =CheckAndReadIniData(szDir, "Input Limit", funLimit[5], -60.0   );

    if(CUSTOMER_CODE==CC_KYEC_LEE)                                              //Eastsun 20260710 Merge
    {
        InputLimit.iTempHigh = 10;
        InputLimit.iTempLow  = -10;
    }

    if(IniConfig.bSPILFunction)
    {
        InputLimit.dLoaderOffsetXYHigh  =CheckAndReadIniData(szDir, "Input Limit", funLimit[15],  5.0      ); //JerryYang 20220923 : 矽品蘇州要求offset limit要By區域設定
        InputLimit.dLoaderOffsetXYLow   =CheckAndReadIniData(szDir, "Input Limit", funLimit[16], -5.0      );
        InputLimit.dLoaderOffsetZHigh   =CheckAndReadIniData(szDir, "Input Limit", funLimit[17],  3.0      );
        InputLimit.dLoaderOffsetZLow    =CheckAndReadIniData(szDir, "Input Limit", funLimit[18], -3.0      );

        InputLimit.dHPOffsetXYHigh      =CheckAndReadIniData(szDir, "Input Limit", funLimit[19],  5.0      );
        InputLimit.dHPOffsetXYLow       =CheckAndReadIniData(szDir, "Input Limit", funLimit[20], -5.0      );
        InputLimit.dHPOffsetZHigh       =CheckAndReadIniData(szDir, "Input Limit", funLimit[21],  3.0      );
        InputLimit.dHPOffsetZLow        =CheckAndReadIniData(szDir, "Input Limit", funLimit[22], -3.0      );
        InputLimit.dHPOffsetZRelHigh    =CheckAndReadIniData(szDir, "Input Limit", funLimit[23],  3.0      );
        InputLimit.dHPOffsetZRelLow     =CheckAndReadIniData(szDir, "Input Limit", funLimit[24], -3.0      );

        InputLimit.dInShtOffsetXYHigh   =CheckAndReadIniData(szDir, "Input Limit", funLimit[25],  5.0      );
        InputLimit.dInShtOffsetXYLow    =CheckAndReadIniData(szDir, "Input Limit", funLimit[26], -5.0      );
        InputLimit.dInShtOffsetZHigh    =CheckAndReadIniData(szDir, "Input Limit", funLimit[27],  3.0      );
        InputLimit.dInShtOffsetZLow     =CheckAndReadIniData(szDir, "Input Limit", funLimit[28], -3.0      );

        InputLimit.dOutShtOffsetXYHigh  =CheckAndReadIniData(szDir, "Input Limit", funLimit[29],  5.0      );
        InputLimit.dOutShtOffsetXYLow   =CheckAndReadIniData(szDir, "Input Limit", funLimit[30], -5.0      );
        InputLimit.dOutShtOffsetZHigh   =CheckAndReadIniData(szDir, "Input Limit", funLimit[31],  3.0      );
        InputLimit.dOutShtOffsetZLow    =CheckAndReadIniData(szDir, "Input Limit", funLimit[32], -3.0      );

        InputLimit.dUnloadOffsetXYHigh  =CheckAndReadIniData(szDir, "Input Limit", funLimit[33],  5.0      );
        InputLimit.dUnloadOffsetXYLow   =CheckAndReadIniData(szDir, "Input Limit", funLimit[34], -5.0      );
        InputLimit.dUnloadOffsetZHigh   =CheckAndReadIniData(szDir, "Input Limit", funLimit[35],  3.0      );
        InputLimit.dUnloadOffsetZLow    =CheckAndReadIniData(szDir, "Input Limit", funLimit[36], -3.0      );

        InputLimit.dAutoCleanContactHigh =CheckAndReadIniData(szDir, "Input Limit", funLimit[37],  1.0     );   //JerryYang 20240111 : 矽品二林要求設定auto clean contact height上下限
        InputLimit.dAutoCleanContactLow  =CheckAndReadIniData(szDir, "Input Limit", funLimit[38],  0.0     );
    }

    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                         //kevin 20180301 (Steven) add -60  =>-80   Bob need for k21
    {
        InputLimit.iTempLow = -80;
        InputLimit.iTempHigh = 60;
        if(CosFunction.bHiSiliconFunction==true)                                //kevin 20200513 add define
        {
            InputLimit.iTempLow  = -60;
            InputLimit.iTempHigh = 60;
        }
        InputLimit.iIlitialTempHigh = InputLimit.iTempHigh;                     //kevin 20210421 ilitial temp offset 個別範圍
        InputLimit.iIlitialTempLow  = 0;                                        //kevin 20210421 JL 要求溫度只有往上補 所以最低0
    }

    InputLimit.iHeaterGunH  =CheckAndReadIniData(szDir, "Input Limit", funLimit[6],  150.0);
    InputLimit.iHeaterGunL  =CheckAndReadIniData(szDir, "Input Limit", funLimit[7], -150.0);
    //InputLimit.iHeaterGunL  =CheckAndReadIniData(szDir, "Input Limit", funLimit[7], -150.0  );
    if(CosFunction.bHiSiliconFunction==true ||                                  //Ifor 20190905 :add 海思 V02.30 版  Contact Offset +-0.5mm
       CUSTOMER_CODE==CC_KYEC_LEE)                                              //Ifor 20190917 KYEC 要求同Hisi
    {
        InputLimit.dContactHigh =10;                                            //Ifor 20191004 : Contact Hight 上限改10 下限固定-0.5
        InputLimit.dContactLow  =-0.5;
    }
    else if(CUSTOMER_CODE==CC_KYEC_XILINX &&
            IniConfig.bChangeKitNoHardStop==true)                               //jou 20170407 (wei) : Universal Change Kit function 限制 contect offset
    {
        InputLimit.dContactHigh =2.0;
        InputLimit.dContactLow  =-0.8;
    }
    else if(CUSTOMER_CODE==CC_ASE_M)                                            //Ifor 20170117 add ASEM ContactOffset +60.0 ~ -1.0
    {
        InputLimit.dContactHigh =60.0;
        InputLimit.dContactLow  =-1.0;
    }
    else if(IniConfig.bSPILFunction==true)                                      //JerryYang 20180726 (Steven) : 矽品 世明要求contact height offset只能調正負1
    {
        InputLimit.dContactHigh =CheckAndReadIniData(szDir, "Input Limit", funLimit[8], 1.0);           //Steven 20140123 : Contact Height的Offset限制
        InputLimit.dContactLow  =CheckAndReadIniData(szDir, "Input Limit", funLimit[9], -1.0);          //Steven 20140123 : Contact Height的Offset限制

        if(InputLimit.dContactHigh>1)
            InputLimit.dContactHigh=1.0;

        if(InputLimit.dContactLow<-1)
            InputLimit.dContactLow=-1.0;
    }
    else
    {
        InputLimit.dContactHigh =CheckAndReadIniData(szDir, "Input Limit", funLimit[8], 2.0);           //Steven 20140123 : Contact Height的Offset限制
        InputLimit.dContactLow  =CheckAndReadIniData(szDir, "Input Limit", funLimit[9], -2.0);          //Steven 20140123 : Contact Height的Offset限制
    }

    if(CUSTOMER_CODE==CC_AMKOR_Korea)                                           //ChungHung 20150115 add for ATK +/-2 mm
    {
        InputLimit.dShuttleHigh =CheckAndReadIniData(szDir, "Input Limit", funLimit[10], 1.0);          //ChungHung 20150115 add for ATK +/-2 mm
        InputLimit.dShuttleLow  =CheckAndReadIniData(szDir, "Input Limit", funLimit[11], -1.0);         //ChungHung 20150115 add for ATK +/-2 mm
    }
    else if(CUSTOMER_CODE==CC_KYEC_XILINX &&
            IniConfig.bChangeKitNoHardStop==true)                               //jou 20170407 (wei) : Universal Change Kit function 限制 contect offset
    {
        InputLimit.dShuttleHigh =2.0;
        InputLimit.dShuttleLow  =-1.5;
    }
    else if(IniConfig.bSPILFunction==true)                                      //JerryYang 20180726 (Steven) : 矽品 世明要求Shuttle height offset只能調正負1
    {
        InputLimit.dShuttleHigh =CheckAndReadIniData(szDir, "Input Limit", funLimit[10], 1.0);
        InputLimit.dShuttleLow  =CheckAndReadIniData(szDir, "Input Limit", funLimit[11], -1.0);

        if(InputLimit.dShuttleHigh>1)
            InputLimit.dShuttleHigh=1.0;

        if(InputLimit.dShuttleLow<-1)
            InputLimit.dShuttleLow=-1.0;
    }
    else
    {
        InputLimit.dShuttleHigh =CheckAndReadIniData(szDir, "Input Limit", funLimit[10], 2.0);          //ChungHung 20150115 add for ATK +/-2 mm
        InputLimit.dShuttleLow  =CheckAndReadIniData(szDir, "Input Limit", funLimit[11], -2.0);         //ChungHung 20150115 add for ATK +/-2 mm
    }
    InputLimit.iContactCntAlm   =CheckAndReadIniData(szDir, "Input Limit", funLimit[12], 300000);       //JerryYang 20200504 銦片計數限制

    InputLimit.dForcePerpinNHigh=CheckAndReadIniData(szDir, "Input Limit", "ForcePerpinNHigh", 100.0);  //Isaac 20210517 : Pinforce加上上下限
    InputLimit.dForcePerpinNLow =CheckAndReadIniData(szDir, "Input Limit", "ForcePerpinNLow",  0.1);
    InputLimit.dForcePerpinHigh =CheckAndReadIniData(szDir, "Input Limit", "ForcePerpinHigh",  100.0);  //Steven 20220216 : Pinforce加上上下限
    InputLimit.dForcePerpinLow  =CheckAndReadIniData(szDir, "Input Limit", "ForcePerpinLow",   0.1);
}
//---------------------------------------------------------------------------
void __fastcall GetMainAuth()
{
    AnsiString szDir=CheckFile(AuthPath, "Security_new.def");
    for(int i=0; i<sizeof(MainForm)/sizeof(MainForm[0]); i++)           authMainForm[i]  =CheckAndReadIniData(szDir, "Main",       MainForm[i],      1);
    for(int i=0; i<sizeof(funcTool)/sizeof(funcTool[0]); i++)           authTool[i]      =CheckAndReadIniData(szDir, "Tool",       funcTool[i],      1);
    for(int i=0; i<sizeof(funMaintaince)/sizeof(funMaintaince[0]); i++) authMaintaince[i]=CheckAndReadIniData(szDir, "Maintaince", funMaintaince[i], 1);

    if((CUSTOMER_CODE==CC_AMKOR_China ||
        CUSTOMER_CODE==CC_QUALCOMM) &&                                          //JerryYang 20170412 (Steven) add QUALCOMM
       IniConfig.bFTBin2RTBin==false)                                           //jou 20170214 (Steven) : RunStartMode need keyin Password
    {
        asRunStartModePassword=CheckAndReadIniData(szDir, "Main", "RunStartMode", AnsiString("12345"));
    }
}
//---------------------------------------------------------------------------
void __fastcall GetConfAuth()
{
    AnsiString szDir=CheckFile(AuthPath, "Security_new.def");
    for(int i=0; i<sizeof(funConfig)/sizeof(funConfig[0]); i++)     authConfig[i]=CheckAndReadIniData(szDir, "Configuration", funConfig[i], 1);
    for(int i=0; i<sizeof(funConf)/sizeof(funConf[0]); i++)         authConf[i]  =CheckAndReadIniData(szDir, "Config",        funConf[i],   1);
}
//---------------------------------------------------------------------------
void __fastcall GetCountClrAuth()
{
    AnsiString szDir="";
    szDir=CheckFile(AuthPath, "Security_new.def");
    for(int i=0; i<sizeof(funCounterClr)/sizeof(funCounterClr[0]); i++)  authCounterClr[i]=CheckAndReadIniData(szDir, "Counter Clear", funCounterClr[i], 1);
}
//---------------------------------------------------------------------------
void __fastcall GetDummyVacuum()
{
    AnsiString szDir=CheckFile(AuthPath, "Security_new.def");
    DummyVacuum.iArmVacuumOn   =CheckAndReadIniData(szDir, "Dummy Vacuum", "Arm Vacuum On",    90);
    DummyVacuum.iArmVacuumOff  =CheckAndReadIniData(szDir, "Dummy Vacuum", "Arm Vacuum Off",   32);
    DummyVacuum.iIndexVacuumOn =CheckAndReadIniData(szDir, "Dummy Vacuum", "Index Vacuum On",  90);
    DummyVacuum.iIndexVacuumOff=CheckAndReadIniData(szDir, "Dummy Vacuum", "Index Vacuum Off", 32);
}
//---------------------------------------------------------------------------
void __fastcall GetSetupAuth()      //Steven 20120405 : Set Up加入Security_new權限
{
    AnsiString szDir=CheckFile(AuthPath, "Security_new.def");
    iSetupSiteMapping   =CheckAndReadIniData(szDir, "Setup", "Machine Setup",    1);
    for(int i=0; i<sizeof(SetUpForm)/sizeof(SetUpForm[0]); i++)
        authSetup[i]=CheckAndReadIniData(szDir, "Setup", SetUpForm[i], 1);
}
//---------------------------------------------------------------------------
AnsiString __fastcall CheckFile(AnsiString szDir, AnsiString str)
{
    MyForceDirectories(szDir, "CheckFile");
    szDir+=str;
    if(!FileExists(szDir))
    {
        WriteIniData(szDir, "Main",          "Note", "Main Form Enable/Disable Control");
        WriteIniData(szDir, "Tool",          "Note", "Tool Button Enable/Disable Control");
        WriteIniData(szDir, "Maintaince",    "Note", "Maintaince button Enable/Disable Control");
        WriteIniData(szDir, "Configuration", "Note", "Tab in Configuration Form Enable/Disable Control");
        WriteIniData(szDir, "Config",        "Note", "Tab in Config Form Enable/Disable Control");
        WriteIniData(szDir, "Counter Clear", "Note", "Counter Clear CheckBox Enable/Disable Control");
        WriteIniData(szDir, "Input Limit",   "Note", "Offset Limitation");

        for(int i=0; i<sizeof(MainForm)/sizeof(MainForm[0]); i++)           WriteIniData(szDir, "Main",          MainForm[i],      1);
        for(int i=0; i<sizeof(funcTool)/sizeof(funcTool[0]); i++)           WriteIniData(szDir, "Tool",          funcTool[i],      1);
        for(int i=0; i<sizeof(funMaintaince)/sizeof(funMaintaince[0]); i++) WriteIniData(szDir, "Maintaince",    funMaintaince[i], 1);
        for(int i=0; i<sizeof(funConfig)/sizeof(funConfig[0]); i++)         WriteIniData(szDir, "Configuration", funConfig[i],     1);
        for(int i=0; i<sizeof(funConf)/sizeof(funConf[0]); i++)             WriteIniData(szDir, "Config",        funConf[i],       1);
        for(int i=0; i<sizeof(funCounterClr)/sizeof(funCounterClr[0]); i++) WriteIniData(szDir, "Counter Clear", funCounterClr[i], 1);
        WriteIniData(szDir, "Input Limit", funLimit[0], 5);
        WriteIniData(szDir, "Input Limit", funLimit[1], -5);
        WriteIniData(szDir, "Input Limit", funLimit[2], 3);
        WriteIniData(szDir, "Input Limit", funLimit[3], -3);
        WriteIniData(szDir, "Input Limit", funLimit[4], 60);
        WriteIniData(szDir, "Input Limit", funLimit[5], -60);

        WriteIniData(szDir, "Dummy Vacuum", "Arm Vacuum On",    90);
        WriteIniData(szDir, "Dummy Vacuum", "Arm Vacuum Off",   32);
        WriteIniData(szDir, "Dummy Vacuum", "Index Vacuum On",  90);
        WriteIniData(szDir, "Dummy Vacuum", "Index Vacuum Off", 32);
    }
    return szDir;
}
//---------------------------------------------------------------------------
void __fastcall ChangeCompomentEnabled(TWinControl *PCtrl, bool bEnable, bool bMustEnable)
{
    // 先將主物件的資料寫入 ......................
    TPanel          *PanelPtr  = dynamic_cast <TPanel       *>(PCtrl);
    TTabSheet       *ShtPtr    = dynamic_cast <TTabSheet    *>(PCtrl);
    TForm           *FrmPtr    = dynamic_cast <TForm        *>(PCtrl);
    TGroupBox       *PGroupBox = dynamic_cast <TGroupBox    *>(PCtrl);
    TRadioGroup     *RadGup    = dynamic_cast <TRadioGroup  *>(PCtrl);
    if(bMustEnable==false)
    {
        if     (PanelPtr  !=NULL) PanelPtr   ->Enabled=(bEnable==false)?false:PanelPtr   ->Enabled;
        else if(ShtPtr    !=NULL) ShtPtr     ->Enabled=(bEnable==false)?false:ShtPtr     ->Enabled;
        else if(FrmPtr    !=NULL) FrmPtr     ->Enabled=(bEnable==false)?false:FrmPtr     ->Enabled;
        else if(PGroupBox !=NULL) PGroupBox  ->Enabled=(bEnable==false)?false:PGroupBox  ->Enabled;
        else if(RadGup    !=NULL) RadGup     ->Enabled=(bEnable==false)?false:RadGup     ->Enabled;
    }
    else
    {
        if     (PanelPtr  !=NULL) PanelPtr   ->Enabled=bEnable;
        else if(ShtPtr    !=NULL) ShtPtr     ->Enabled=bEnable;
        else if(FrmPtr    !=NULL) FrmPtr     ->Enabled=bEnable;
        else if(PGroupBox !=NULL) PGroupBox  ->Enabled=bEnable;
        else if(RadGup    !=NULL) RadGup     ->Enabled=bEnable;
    }

    // 再將附屬物件的資料寫入 .....................
    for(int iP=0; iP<PCtrl->ControlCount; iP++)
    {
        TControl *P=PCtrl->Controls[iP];
        if(dynamic_cast <TPanel       *>(P)!=NULL ||     // 有子節點
            dynamic_cast <TPageControl *>(P)!=NULL ||
            dynamic_cast <TTabSheet    *>(P)!=NULL ||
            dynamic_cast <TTabControl  *>(P)!=NULL ||
            dynamic_cast <TForm        *>(P)!=NULL ||
            dynamic_cast <THeader      *>(P)!=NULL ||
            dynamic_cast <TPage        *>(P)!=NULL ||
            dynamic_cast <TGroupBox    *>(P)!=NULL ||
            dynamic_cast <TScrollBox   *>(P)!=NULL ||
            dynamic_cast <TRadioGroup  *>(P)!=NULL
            )
        {
            ChangeCompomentEnabled((TWinControl *) P, bEnable, bMustEnable);       // 找該物件附屬的物件(遞迴) //Steven 20120912 : 會發生被Disable後,無法Enable的狀況
        }
        TLabel          *PLabel       = dynamic_cast <TLabel        *>(P);
        TSpeedButton    *PSpeedButton = dynamic_cast <TSpeedButton  *>(P);
        TButton         *PButton      = dynamic_cast <TButton       *>(P);
        TCheckBox       *PCheckBox    = dynamic_cast <TCheckBox     *>(P);
        TRadioButton    *PRadioButton = dynamic_cast <TRadioButton  *>(P);
        if(bMustEnable==false)
        {
            if     (PLabel      !=NULL) PLabel      ->Enabled=(bEnable==false)?false:PLabel         ->Enabled;
            else if(PSpeedButton!=NULL) PSpeedButton->Enabled=(bEnable==false)?false:PSpeedButton   ->Enabled;
            else if(PButton     !=NULL) PButton     ->Enabled=(bEnable==false)?false:PButton        ->Enabled;
            else if(PCheckBox   !=NULL) PCheckBox   ->Enabled=(bEnable==false)?false:PCheckBox      ->Enabled;
            else if(PRadioButton!=NULL) PRadioButton->Enabled=(bEnable==false)?false:PRadioButton   ->Enabled;
        }
        else
        {
            if     (PLabel      !=NULL) PLabel      ->Enabled=bEnable;
            else if(PSpeedButton!=NULL) PSpeedButton->Enabled=bEnable;
            else if(PButton     !=NULL) PButton     ->Enabled=bEnable;
            else if(PCheckBox   !=NULL) PCheckBox   ->Enabled=bEnable;
            else if(PRadioButton!=NULL) PRadioButton->Enabled=bEnable;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall GetCriticalParaAuth()                                           //JerryYang 20220310 : ATP要求鎖定critical parameter
{
    AnsiString szDir=AuthPath+"CriticalParaControl.ini";

    bAuthCriticalPara[0]=CheckAndReadIniData(szDir,  "Parameter Control", funCriticalPara[0],  1);              //"Temperature_Setting_Value",
    bAuthCriticalPara[1]=CheckAndReadIniData(szDir,  "Parameter Control", funCriticalPara[1],  1);              //"Soake_Time",
    bAuthCriticalPara[2]=CheckAndReadIniData(szDir,  "Parameter Control", funCriticalPara[2],  1);              //"Bin_Setting",
    bAuthCriticalPara[3]=CheckAndReadIniData(szDir,  "Parameter Control", funCriticalPara[3],  0);              //"Active_Site",
    bAuthCriticalPara[4]=CheckAndReadIniData(szDir,  "Parameter Control", funCriticalPara[4],  1);              //"Test_Site_Assign_Handling_Mode_Site_Map",
    bAuthCriticalPara[5]=CheckAndReadIniData(szDir,  "Parameter Control", funCriticalPara[5],  1);              //"Contact_Force",
    bAuthCriticalPara[6]=CheckAndReadIniData(szDir,  "Parameter Control", funCriticalPara[6],  0);              //"Vacuum_On_Off_Time",
    bAuthCriticalPara[7]=CheckAndReadIniData(szDir,  "Parameter Control", funCriticalPara[7],  1);              //"Socket_Pitch",
    bAuthCriticalPara[8]=CheckAndReadIniData(szDir,  "Parameter Control", funCriticalPara[8],  1);              //"Package_Dimensions_Pin_Count",
    bAuthCriticalPara[9]=CheckAndReadIniData(szDir,  "Parameter Control", funCriticalPara[9],  1);              //"Contact_Parameter",
    bAuthCriticalPara[10]=CheckAndReadIniData(szDir, "Parameter Control", funCriticalPara[10], 0);              //"Drop_Wait_Time",
    bAuthCriticalPara[11]=CheckAndReadIniData(szDir, "Parameter Control", funCriticalPara[11], 0);              //"Contact_Speed",
    bAuthCriticalPara[12]=CheckAndReadIniData(szDir, "Parameter Control", funCriticalPara[12], 1);              //"Double_Device_Height",
    bAuthCriticalPara[13]=CheckAndReadIniData(szDir, "Parameter Control", funCriticalPara[13], 1);              //"Temperature_Mode",
    bAuthCriticalPara[14]=CheckAndReadIniData(szDir, "Parameter Control", funCriticalPara[14], 0);              //"QA_Sampling_Count",
    bAuthCriticalPara[15]=CheckAndReadIniData(szDir, "Parameter Control", funCriticalPara[15], 1);              //"Socket_Sensor",
    bAuthCriticalPara[16]=CheckAndReadIniData(szDir, "Parameter Control", funCriticalPara[16], 1);              //"PiggyBackFunction",
    bAuthCriticalPara[17]=CheckAndReadIniData(szDir, "Parameter Control", funCriticalPara[17], 0);              //"Index_Unit_Speed",
    bAuthCriticalPara[18]=CheckAndReadIniData(szDir, "Parameter Control", funCriticalPara[18], 1);              //"Tray_Form",
    bAuthCriticalPara[19]=CheckAndReadIniData(szDir, "Parameter Control", funCriticalPara[19], 1);              //"Plate_Form",
    bAuthCriticalPara[20]=CheckAndReadIniData(szDir, "Parameter Control", funCriticalPara[20], 1);              //"Tray_Assign",
    bAuthCriticalPara[21]=CheckAndReadIniData(szDir, "Parameter Control", funCriticalPara[21], 1);              //"Test_IF",
    bAuthCriticalPara[22]=CheckAndReadIniData(szDir, "Parameter Control", funCriticalPara[22], 0);              //"Yield_Site_To_Site",
    bAuthCriticalPara[23]=CheckAndReadIniData(szDir, "Parameter Control", funCriticalPara[23], 0);              //"Yield_Continous_Fail",
    bAuthCriticalPara[24]=CheckAndReadIniData(szDir, "Parameter Control", funCriticalPara[24], 0);              //"Yield_Yield",
    bAuthCriticalPara[25]=CheckAndReadIniData(szDir, "Parameter Control", funCriticalPara[25], 0);              //"ACC_Function"
}
//---------------------------------------------------------------------------
