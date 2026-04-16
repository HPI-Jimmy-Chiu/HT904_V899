#include "MachineDefine.h"
#pragma hdrstop

#include "uCleaning.h"
#include "myQwertyKeyBoard.h"
#include "cprod.h"
#include "cpublic.h"
#include "AutoClean.h"
//#include "INPUT.h"
#include "cContact.h"
#include "cmydef.h"
#include "common.h"
#include "mymessbox.h"
#include "main.h"
#include "atester.h"
#include "cUnitConvert.h"
#include "BarcodeReader.h"                                                      // 2013.11.29 , Joye , KYEC Barcode Reader  20140103 wei
#include "cMyDB.h"
#include "cSecurity.h"
#include "cShowBinSelect.h"
#include "cConfiguration.h"
#include "uHGemHT9045.h"
#include "csystem.h"
//#include "AutoClean_ASEKH.h"
#include "aArmHeader.h"
#include "RPDefault.h"
#include "mymessbox.h"
#include "MyMotor.h"
#include "uLotInfo.h"
#include "cAuthority.h"
#include "note.h"
#include "cinitial.h"
//------------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "HTray"
#pragma resource "*.dfm"
TfCleaning *fCleaning;
int iBackupCleaningDeveicePices;
int iBackupCleaningItemX;
int iBackupCleaningItemY;
int iBackupAutoClean_Function;
//------------------------------------------------------------------------------
__fastcall TfCleaning::TfCleaning(TComponent* Owner)
    : TForm(Owner)
{
    edIntervalContact->Hint="AutoClean_Cleaning Mode Interval Contact Count";   //Sam 20201209 : Default Recipe ChangeLog
    chkAutoCleanMode5->Hint="AutoClean_Cleaning Mode Socket Alarm Count";       //Sam 20201209 : Default Recipe ChangeLog
    bResetCleanCount=false;
    fShowBinSelect->btnCleanReset->Enabled=false;
    LoadAutoCleanData();                                                        //Steven 20211129 : Add for Auto Clean初始化的資料是錯的
    iDeviceCount=0;
}
//------------------------------------------------------------------------------
void TfCleaning::SetArmCaption()
{
    lblInArm->Caption="Input Arm ";
    lblInArmZ->Caption="Input Arm Z";
    if(CUSTOMER_CODE==CC_Greatek)
    {
        OutArmSpeed->Hint="AutoClean_InArm Speed";                              //Sam 20210412 : 修正顯示
    }
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::LoadAutoCleanData()
{
    static bool bAutoCleanOnOff=false;
    static bool bFirstIn=true;
    bool bNeedReset=false;
    bool bFix3HasBin=false;
    AnsiString S="", Str;
    S=GetLastOpenFN();
    AnsiString szDir="", szDir1="";
    int iTempCnt=0;

    if(bFirstIn==true)
    {
        bFirstIn=false;
        fSpeed->ReadFile();
    }

    if(CUSTOMER_CODE==CC_TSMC_TAINAN ||
       CosFunction.bUseDefineAutoCleanOffset==true)                             //wei 20160219 Auto Clean by機台存檔   //Ifor 20181106 add 新增Auto Clean Offset By 本機
    {
        szDir.sprintf("D:\\HT9045\\IniData\\DefineAutoClean");
        MyForceDirectories(szDir);
        szDir.sprintf("D:\\HT9045\\IniData\\DefineAutoClean\\AutoClean.Data");
        szDir1.sprintf("%s%s", DataPath, S);
        szDir1+="\\HandlerCondition.Data";
        if(!FileExists(szDir))
        {
            CopyFile(szDir1.c_str(),szDir.c_str(),true);
        }

        if(CUSTOMER_CODE!=CC_TSMC_TAINAN)
        {
            szDir.sprintf("%s%s\\HandlerCondition.Data", DataPath, S);
        }
    }
    else
    {
        szDir.sprintf("%s%s\\HandlerCondition.Data", DataPath, S);
    }

    if(CosFunction.bAutoCleanUseHPSetByRecipe)                                  //Steven 20210825 : Auto Clean使用加熱盤要改成在工作檔設定
    {
        IniConfig.bE43AutoCleanUseHotplate    =ReadIniData(szDir, "Configuration", "bAutoCleanUseHotplate",      false);
        chkE43->Checked=IniConfig.bE43AutoCleanUseHotplate;
        tsKit->Caption=IniConfig.bE43AutoCleanUseHotplate?"Hot Plate":"Kit";
        SetAutoCleanTrayPosition();
    }

    TestIF_File.iAutoClean_Function           =ReadIniData(szDir, "Configuration", "iAutoClean_Function",            0);
    TestIF_File.iAutoClean_Mode               =ReadIniData(szDir, "Configuration", "iAutoClean_Mode",       M_INTERVAL);
    TestIF_File.iAutoClean_Tray               =ReadIniData(szDir, "Configuration", "iAutoClean_Tray",  eCKPos_CleanKit);        //Steven 20120217 : 選擇把CleanPad放在哪邊
    iTempCnt                                  =ReadIniData(szDir, "Configuration", "iIndexArmAutoCleanCnt",  0);                //Sam 20250820 : AutoClean 在 Index Arm 下壓清潔一次就++
    if(iTempCnt>=TestIF_File.iIndexArmAutoCleanCnt)
        TestIF_File.iIndexArmAutoCleanCnt=iTempCnt;
    if(TestIF_File.iAutoClean_Mode==0)                                          //Ifor 20180823 (Steven) : Add Auto Clean 功能選項無開啟就不使用
    {
        TestIF_File.iAutoClean_Function=false;
        rgAutoCleanOnOff->ItemIndex=0;                                          //Ifor 20180730 (Steven) : add Auto Clean 未選取功能強制關閉
        WriteIniData(szDir, "Configuration", "iAutoClean_Function", TestIF_File.iAutoClean_Function);
    }

    if(CLEAN_AIR)
    {
        TestIF_File.iAutoClean_Tray=eCKPos_CleanAir;
    }
    else
    {
        if(bUse_NewAutoCleanForm==1)                                            //kevin 20190620
            TestIF_File.iAutoClean_Tray=eCKPos_CleanKit;
        else if(TestIF_File.iAutoClean_Tray==eCKPos_Fix3)
            TestIF_File.iAutoClean_Tray=eCKPos_CleanKit;
    }

    TestIF_File.iAutoClean_AlarmCount           =ReadIniData(szDir, "Configuration", "iAutoClean_AlarmCount",         10);
    TestIF_File.iPadThickness                   =ReadIniData(szDir, "Configuration", "iAutoClean_iPadThickness",      10);//kevin 20180630 add clean pad - device
    TestIF_File.iAutoClean_MotorSpeed[0]        =ReadIniData(szDir, "Configuration", "iAutoClean_MotorSpeed[0]",      10);
    TestIF_File.iAutoClean_MotorSpeed[1]        =ReadIniData(szDir, "Configuration", "iAutoClean_MotorSpeed[1]",      10);
    TestIF_File.iAutoClean_MotorSpeed[2]        =ReadIniData(szDir, "Configuration", "iAutoClean_MotorSpeed[2]",      10);
    TestIF_File.iAutoClean_MotorSpeed[3]        =ReadIniData(szDir, "Configuration", "iAutoClean_MotorSpeed[3]",      10);
    TestIF_File.iAutoClean_ContactMode          =ReadIniData(szDir, "Configuration", "iAutoClean_ContactMode",         0);
    TestIF_File.iAutoCleanDropHigh              =ReadIniData(szDir, "Configuration", "iAutoClean_DropHigh",            0); //kevin 20180717 autoClean drop high
    TestIF_File.iAutoClean_ContactTime          =ReadIniData(szDir, "Configuration", "iAutoClean_ContactTime",         5);
    TestIF_File.iAutoClean_ContactCount         =ReadIniData(szDir, "Configuration", "iAutoClean_ContactCount",        3);

    TestIF_File.dAutoClean_InArmVacuum          =ReadIniData(szDir, "AutoClean", "InArmVacuum",            ArmSpeed_File[InArm].dVacuumTI);         //Steven 20250319 : Auto Clean使用另外一組Delay Time
    TestIF_File.dAutoClean_InArmAirOn           =ReadIniData(szDir, "AutoClean", "InArmAirOn",             ArmSpeed_File[InArm].dCTAirOn);
    TestIF_File.dAutoClean_IndexVacuum          =ReadIniData(szDir, "AutoClean", "IndexVacuum",            ArmSpeed_File[IndexArm].dVacuumTI);
    TestIF_File.dAutoClean_IndexAirOn           =ReadIniData(szDir, "AutoClean", "IndexAirOn",             ArmSpeed_File[IndexArm].dCTAirOn);

    if(CosFunction.bSmartAutoClean)                                             //Sam 20230111 : Smart Auto Clean
    {
        TestIF_File.bACSmart                    =ReadIniData(szDir, "Configuration", "bACSmart",                    false);
        TestIF_File.iACSmart_Count              =ReadIniData(szDir, "Configuration", "iACSmart_Count",              2);
        TestIF_File.iACSmart_Count_CTF          =ReadIniData(szDir, "Configuration", "iACSmart_Count CTF",          1); //Sam 20240726 : AI Clean
        TestIF_File.iACSmart_ContactMode        =ReadIniData(szDir, "Configuration", "iACSmart_ContactMode",        0);
        TestIF_File.iACSmart_DropHigh           =ReadIniData(szDir, "Configuration", "iACSmart_DropHigh",           0);
        TestIF_File.iACSmart_ContactTime        =ReadIniData(szDir, "Configuration", "iACSmart_ContactTime",        5);
        TestIF_File.iACSmart_ContactCount       =ReadIniData(szDir, "Configuration", "iACSmart_ContactCount",       3);

        TestIF_File.iAdaptiveACIntervalMax      =ReadIniData(szDir, "Configuration", "iAdaptiveACIntervalMax",       100);
        TestIF_File.iAdaptiveACIntervalMin      =ReadIniData(szDir, "Configuration", "iAdaptiveACIntervalMin",      5);
        TestIF_File.iAdaptiveACIntervalAdj      =ReadIniData(szDir, "Configuration", "iAdaptiveACIntervalAdj",      1);
    }

    if(CosFunction.bSetContactForceToAutoClean==true)                           //Steven 20250915 : 複製Contact Force給Auto Clean用
    {
        TestIF_File.iAutoClean_DevicePinCount   =DeviceForm_File.iPinCT;
        TestIF_File.fAutoClean_ForcePerPin      =DeviceForm_File.ForcePerPinN;
        TestIF_File.fAutoClean_DevicePinForceGf =DeviceForm_File.ForcePerPinG;
        TestIF_File.fAutoClean_AireForce        =DeviceForm_File.dPress;
        if(INSTALL_DOUBLE_EP==1)
            TestIF_File.fAutoClean_DieForce     =DeviceForm_File.DoubleForce;
    }
    else
    {
        TestIF_File.iAutoClean_DevicePinCount   =ReadIniData(szDir, "Configuration", "iAutoClean_DevicePinCount",      0);
        TestIF_File.fAutoClean_ForcePerPin      =ReadIniData(szDir, "Configuration", "fAutoClean_ForcePerPin",         0.0);//kevin 20150826
        TestIF_File.fAutoClean_DevicePinForceGf =ReadIniData(szDir, "Configuration", "fAutoClean_DevicePinForceGf",    0.0);
        TestIF_File.fAutoClean_AireForce        =ReadIniData(szDir, "Configuration", "fAutoClean_AireForce",           0.0);
        if(INSTALL_DOUBLE_EP==1)
            TestIF_File.fAutoClean_DieForce     =ReadIniData(szDir, "Configuration", "fAutoClean_DieForce",            DeviceForm_File.DoubleForce);//Steven 20240719 : Die force for auto clean
    }

    TestIF_File.iAutoClean_ContactShiftHeight   =ReadIniData(szDir, "Configuration", "iAutoClean_ContactShiftHeight",  1000);
    TestIF_File.iAutoClean_ContactCleanHeight   =ReadIniData(szDir, "Configuration", "iAutoClean_ContactCleanHeight",  0);
    TestIF_File.iAutoClean_IndexPickOffset      =ReadIniData(szDir, "Configuration", "iAutoClean_IndexPickOffset",     0);    //wei 20150318 Auto clean Index Pick Offset
    TestIF_File.iAutoClean_IndexReleaseOffset   =ReadIniData(szDir, "Configuration", "iAutoClean_IndexReleaseOffset",  0);    //Jou 2015-08-22 Auto clean Index Release Offset
    if(IniConfig.bE48_ShuttleUse4Offset_Autoclean==false)                       //Steven 20241102 : fixed for Auto Clean offset
    {
        TestIF_File.iAutoClean_Shuttle1PickOffset =ReadIniData(szDir, "Configuration", "iAutoClean_Shuttle1PickOffset",  -200);
        TestIF_File.iAutoClean_Shuttle1PlaceOffset=ReadIniData(szDir, "Configuration", "iAutoClean_Shuttle1PlaceOffset", 0);
        TestIF_File.iAutoClean_Shuttle1XOffset    =ReadIniData(szDir, "Configuration", "iAutoClean_Shuttle1XOffset",     0);
        TestIF_File.iAutoClean_Shuttle1YOffset    =ReadIniData(szDir, "Configuration", "iAutoClean_Shuttle1YOffset",     0);

        TestIF_File.iAutoClean_Shuttle2PickOffset =ReadIniData(szDir, "Configuration", "iAutoClean_Shuttle2PickOffset",  -200); //ChungHung 20141103 add AutoClean Input Arm Shuttle2 Offset
        TestIF_File.iAutoClean_Shuttle2PlaceOffset=ReadIniData(szDir, "Configuration", "iAutoClean_Shuttle2PlaceOffset", 0);
        TestIF_File.iAutoClean_Shuttle2XOffset    =ReadIniData(szDir, "Configuration", "iAutoClean_Shuttle2XOffset",     0);
        TestIF_File.iAutoClean_Shuttle2YOffset    =ReadIniData(szDir, "Configuration", "iAutoClean_Shuttle2YOffset",     0);
    }
    else
    {
        TestIF_File.iAutoClean_Shuttle1PickOffset =0;
        TestIF_File.iAutoClean_Shuttle1PlaceOffset=0;
        TestIF_File.iAutoClean_Shuttle1XOffset    =0;
        TestIF_File.iAutoClean_Shuttle1YOffset    =0;

        TestIF_File.iAutoClean_Shuttle2PickOffset =0;
        TestIF_File.iAutoClean_Shuttle2PlaceOffset=0;
        TestIF_File.iAutoClean_Shuttle2XOffset    =0;
        TestIF_File.iAutoClean_Shuttle2YOffset    =0;
    }

    TestIF_File.dBufferKitLTX    =ReadIniData(szDir, "Configuration", "dBufferKitLTX",  0.0);
    TestIF_File.dBufferKitLTY    =ReadIniData(szDir, "Configuration", "dBufferKitLTY",  0.0);
    TestIF_File.dBufferKitLDX    =ReadIniData(szDir, "Configuration", "dBufferKitLDX",  0.0);
    TestIF_File.dBufferKitLDY    =ReadIniData(szDir, "Configuration", "dBufferKitLDY",  0.0);
    TestIF_File.dBufferKitRTX    =ReadIniData(szDir, "Configuration", "dBufferKitRTX",  0.0);
    TestIF_File.dBufferKitRTY    =ReadIniData(szDir, "Configuration", "dBufferKitRTY",  0.0);
    TestIF_File.dBufferKitRDX    =ReadIniData(szDir, "Configuration", "dBufferKitRDX",  0.0);
    TestIF_File.dBufferKitRDY    =ReadIniData(szDir, "Configuration", "dBufferKitRDY",  0.0);
    if(TestIF_File.iAutoClean_ContactShiftHeight<=0)
    {
        TestIF_File.iAutoClean_ContactShiftHeight=1000;
        WriteIniData(szDir, "Configuration", "iAutoClean_ContactShiftHeight",   TestIF_File.iAutoClean_ContactShiftHeight);
    }

    int iAutoCleanOffset=0;

    if(IniConfig.bE43AutoCleanUseHotplate)                                      //ChungHung 20131120 AutoClean use Hotplate1 start
    {
        TestIF_File.iAutoClean_YDivision_Kit     =ReadIniData(szDir, "Configuration", "iAutoClean_YDivision_Kit",  2);
        TestIF_File.iAutoClean_YDivision_Tray    =ReadIniData(szDir, "Configuration", "iAutoClean_YDivision_Tray", 2);

        S=GetLastOpenFN();
        szDir="";
        szDir.sprintf("%s%s", DataPath, S);
        szDir+="\\HotPlate.Data";

        TestIF_File.dAutoClean_XPitch_Kit=iUnitMultiply100(ReadIniData(szDir, "Hotplate Form", "X Pitch",    22.00));
        TestIF_File.dAutoClean_YPitch_Kit=iUnitMultiply100(ReadIniData(szDir, "Hotplate Form", "Y Pitch",    33.00));
        TestIF_File.dAutoClean_XStart_Kit=iUnitMultiply100(ReadIniData(szDir, "Hotplate Form", "X Start",    33.00));
        TestIF_File.dAutoClean_YStart_Kit=iUnitMultiply100(ReadIniData(szDir, "Hotplate Form", "Y Start",    18.00));
        TestIF_File.iAutoClean_XDivision_Kit=ReadIniData(szDir, "Hotplate Form", "X Division", 4);
//        TestIF_File.iAutoClean_YDivision_Kit=ReadIniData(szDir, "Hotplate Form", "Y Division", 4);
        if(TestIF.iTestMode==DualSite &&
           TestIF_File.iAutoClean_XDivision_Kit==3)                             //JerryYang 20160321 強制修改HotPlate,避免Input Arm行程不足
        {
            TestIF_File.iAutoClean_XDivision_Kit=2;
        }
        if(TestIF_File.iAutoClean_XDivision_Kit>=6 &&
           TestIF_File.iTestMode!=_16Site2X8)                                   //Ztex 2025.04.09 Add HT-1132 Auto Clean
        {
            iAutoCleanOffset=TestIF_File.iAutoClean_XDivision_Kit-4;
            TestIF_File.iAutoClean_XDivision_Kit=4;
            TestIF_File.dAutoClean_XStart_Kit+=iAutoCleanOffset*TestIF_File.dAutoClean_XPitch_Kit;
        }

        S=GetLastOpenFN();
        szDir.sprintf("%s%s", DataPath, S);
        szDir+="\\Tray.Data";
        int iType=ReadIniData(szDir, "Loader",   "Tray Type",     0);
        S="Type"+AnsiString(iType);
        TestIF_File.dAutoClean_XStart_Tray       =iUnitMultiply100(ReadIniData(szDir, S, "X Start",      0.1));
        TestIF_File.dAutoClean_YStart_Tray       =iUnitMultiply100(ReadIniData(szDir, S, "Y Start",      0.1));
        TestIF_File.dAutoClean_XPitch_Tray       =iUnitMultiply100(ReadIniData(szDir, S, "X Pitch",      0.1));
        TestIF_File.dAutoClean_YPitch_Tray       =iUnitMultiply100(ReadIniData(szDir, S, "Y Pitch",      0.1));
        TestIF_File.iAutoClean_XDivision_Tray    =ReadIniData(szDir, S, "X Division",   14);

        if(TestIF_File.iAutoClean_XDivision_Tray<4)
        {
            iAutoCleanOffset=TestIF_File.iAutoClean_XDivision_Tray-(TestIF_File.iAutoClean_XDivision_Tray/2-(TestIF_File.iAutoClean_XDivision_Tray/2)/2)*2*2;
            TestIF_File.iAutoClean_XDivision_Tray=TestIF_File.iAutoClean_XDivision_Tray-iAutoCleanOffset;
            TestIF_File.dAutoClean_XStart_Tray+=iAutoCleanOffset*TestIF_File.dAutoClean_XPitch_Tray;
        }
        else
        {
            iAutoCleanOffset=TestIF_File.iAutoClean_XDivision_Tray-(TestIF_File.iAutoClean_XDivision_Tray/4-(TestIF_File.iAutoClean_XDivision_Tray/4)/2)*2*4;
            TestIF_File.iAutoClean_XDivision_Tray=TestIF_File.iAutoClean_XDivision_Tray-iAutoCleanOffset;
            TestIF_File.dAutoClean_XStart_Tray+=iAutoCleanOffset*TestIF_File.dAutoClean_XPitch_Tray;
        }
    }
    else
    {
        TestIF_File.dAutoClean_XPitch_Kit    =ReadIniData(szDir, "Configuration", "dAutoClean_XPitch_Kit",     2200.0);
        TestIF_File.dAutoClean_YPitch_Kit    =ReadIniData(szDir, "Configuration", "dAutoClean_YPitch_Kit",     2200.0);
        TestIF_File.dAutoClean_XStart_Kit    =ReadIniData(szDir, "Configuration", "dAutoClean_XStart_Kit",     3300.0);
        TestIF_File.dAutoClean_YStart_Kit    =ReadIniData(szDir, "Configuration", "dAutoClean_YStart_Kit",     1800.0);
        TestIF_File.iAutoClean_XDivision_Kit =ReadIniData(szDir, "Configuration", "iAutoClean_XDivision_Kit",  8);
        TestIF_File.iAutoClean_YDivision_Kit =ReadIniData(szDir, "Configuration", "iAutoClean_YDivision_Kit",  2);

        if(CUSTOMER_CODE==CC_KYEC_LEE)                                          //wei 20150908
        {
            TestIF_File.dAutoClean_XPitch_Tray   =UserDefForm_File[0].XPitch*100;
            TestIF_File.dAutoClean_YPitch_Tray   =UserDefForm_File[0].YPitch*100;
            TestIF_File.dAutoClean_XStart_Tray   =UserDefForm_File[0].XStart*100;
            TestIF_File.dAutoClean_YStart_Tray   =UserDefForm_File[0].YStart*100;
        }
        else
        {
            TestIF_File.dAutoClean_XPitch_Tray   =ReadIniData(szDir, "Configuration", "dAutoClean_XPitch_Tray",    2200.0);
            TestIF_File.dAutoClean_YPitch_Tray   =ReadIniData(szDir, "Configuration", "dAutoClean_YPitch_Tray",    2200.0);
            TestIF_File.dAutoClean_XStart_Tray   =ReadIniData(szDir, "Configuration", "dAutoClean_XStart_Tray",    3300.0);
            TestIF_File.dAutoClean_YStart_Tray   =ReadIniData(szDir, "Configuration", "dAutoClean_YStart_Tray",    1800.0);
        }
        TestIF_File.iAutoClean_XDivision_Tray=ReadIniData(szDir, "Configuration", "iAutoClean_XDivision_Tray", 8);
        TestIF_File.iAutoClean_YDivision_Tray=ReadIniData(szDir, "Configuration", "iAutoClean_YDivision_Tray", 2);

        TestIF_File.cAutoClean_PackageTray=ReadIniData(szDir, "Configuration", "cAutoClean_PackageTray", AnsiString("BGA"));
    }

    if(CUSTOMER_CODE==CC_ASE_KaohSiung ||
       CUSTOMER_CODE==CC_OSE ||                                                 //kevin 20180309 add OSE
       CUSTOMER_CODE==CC_ASE_KaohSiung_K3)                                      //kevin 20210929 add 938
    {
        chkAutoCleanMode2->Visible=false;
        chkAutoCleanMode3->Visible=false;
        chkAutoCleanMode5->Visible=false;
        pcCleanYield->Visible=false;
        rgCleanKitType->Enabled=false;
        rgCleanKitType->ItemIndex=1;
        btInclude->Visible=false;
    }
    S=GetLastOpenFN();
    szDir="";

    if(CUSTOMER_CODE==CC_TSMC_TAINAN)                                           //wei 20160219 Auto Clean by機台存檔
    {
        szDir.sprintf("D:\\HT9045\\IniData\\DefineAutoClean\\AutoClean.Data");
    }
    else
    {
        szDir.sprintf("%s%s\\HandlerCondition.Data", DataPath, S);
    }

    ChangeEditToHPMode();                                                       //Steven20160628 : 整合Auto Clean畫面顯示
    TestIF_File.iAutoClean_IntervalContact    =ReadIniData(szDir, "Configuration", "iAutoClean_IntervalContact",    20);
    TestIF_File.iAutoClean_DeveicePices       =ReadIniData(szDir, "Configuration", "iAutoClean_DeveicePices",       8);

    XPitch1->Text=ConvertTouMType(TestIF_File.dAutoClean_XPitch_Kit);           //Steven 20240826 : 往上移動
    YPitch1->Text=ConvertTouMType(TestIF_File.dAutoClean_YPitch_Kit);
    XST1->Text=ConvertTouMType(TestIF_File.dAutoClean_XStart_Kit);
    YST1->Text=ConvertTouMType(TestIF_File.dAutoClean_YStart_Kit);
    XCT1->Text=TestIF_File.iAutoClean_XDivision_Kit;
    YCT1->Text=TestIF_File.iAutoClean_YDivision_Kit;

    XPitch2->Text=ConvertTouMType(TestIF_File.dAutoClean_XPitch_Tray);
    YPitch2->Text=ConvertTouMType(TestIF_File.dAutoClean_YPitch_Tray);
    XST2->Text=ConvertTouMType(TestIF_File.dAutoClean_XStart_Tray);
    YST2->Text=ConvertTouMType(TestIF_File.dAutoClean_YStart_Tray);

    XCT2->Text=TestIF_File.iAutoClean_XDivision_Tray;
    if(atoi(XCT2->Text.c_str())!=TestIF_File.iAutoClean_XDivision_Tray)         //Steven 20181226 : 避免畫面顯示跟跑的數值不一樣
        TestIF_File.iAutoClean_XDivision_Tray=atoi(XCT2->Text.c_str());

    YCT2->Text=TestIF_File.iAutoClean_YDivision_Tray;
    PageTypeName->Text=TestIF_File.cAutoClean_PackageTray;                      //kevin 20150521

    int iCount=GetMinCleanPadCount();                                           //Steven 20221006 : 計算最少需要的Clean Pad數量
    if(TestIF_File.iAutoClean_DeveicePices<iCount)
        TestIF_File.iAutoClean_DeveicePices=iCount;

    if(IniConfig.bE43AutoCleanUseHotplate==false)                               //ChungHung 20131120 AutoClean use Hotplate1
    {
        if(TestIF_File.iAutoClean_DeveicePices>100)                             //QQ : 待確認
            TestIF_File.iAutoClean_DeveicePices=100;
    }
    iBackupCleaningDeveicePices         =TestIF_File.iAutoClean_DeveicePices;

    SetDeviceMaxMin();                                                          //kevin 20221011 read data and set different
    if(TestIF_File.iTestMode==_12Site2X6)
    {
        cbbCleanPadCount->Text          =TestIF_File.iAutoClean_DeveicePices;   //Steven 20241113 : for 2x6 auto clean
        if(udDeviceCT->Min>=cbbCleanPadCount->Text)                             //kevin 20221010 kevin 20221011 add clean pad load data
        {
            cbbCleanPadCount->Text      =udDeviceCT->Min;
            edDevicePices->Text         =udDeviceCT->Min;
        }
        TestIF_File.iAutoClean_DeveicePices   =atoi(cbbCleanPadCount->Text.c_str());
    }
    else
    {
        edDevicePices->Text             =TestIF_File.iAutoClean_DeveicePices;   //Steven 20221028 : 修正重開程式Auto Clean數量錯誤的問題
        if(udDeviceCT->Min>=edDevicePices->Text)                                //kevin 20221010 kevin 20221011 add clean pad load data
        {
            edDevicePices->Text=udDeviceCT->Min;
        }
        TestIF_File.iAutoClean_DeveicePices   =atoi(edDevicePices->Text.c_str());
    }

    if(InitialOK==false)                                                        //Steven 20180524 : Fixed for clean count, 系統還沒讀取檔案不可以做動作
    {
        if(TestIF_File.bAutoClean_UseTray)
        {
            iBackupCleaningItemX              =TestIF_File.iAutoClean_XDivision_Tray;
            iBackupCleaningItemY              =TestIF_File.iAutoClean_YDivision_Tray;
        }
        else
        {
            iBackupCleaningItemX              =TestIF_File.iAutoClean_XDivision_Kit;
            iBackupCleaningItemY              =TestIF_File.iAutoClean_YDivision_Kit;
        }
    }
    else
    {
        if(TestIF_File.bAutoClean_UseTray)
        {
            iBackupCleaningItemX              =TestIF_File.iAutoClean_XDivision_Tray;
            iBackupCleaningItemY              =TestIF_File.iAutoClean_YDivision_Tray;
            if(iBackupCleaningItemX!=TestIF.iAutoClean_XDivision_Tray || iBackupCleaningItemY!=TestIF.iAutoClean_YDivision_Tray)
                bNeedReset=true;
        }
        else
        {
            iBackupCleaningItemX              =TestIF_File.iAutoClean_XDivision_Kit;
            iBackupCleaningItemY              =TestIF_File.iAutoClean_YDivision_Kit;
            if(iBackupCleaningItemX!=TestIF.iAutoClean_XDivision_Kit || iBackupCleaningItemY!=TestIF.iAutoClean_YDivision_Kit)
                bNeedReset=true;
        }
    }

    TestIF_File.bCleanIndexOtherArm =ReadIniData(szDir, "Configuration", "iCleanIndexOtherArm", false);
    TestIF_File.iAutoClean_SelectArm=ReadIniData(szDir, "Configuration", "iAutoClean_SelectArm", 0);  //0:Arm1 1:Arm2 2:Arm1 & Arm2

    if((MachineTypeChoice==Type_HT9045 ||
        MachineTypeChoice==Type_HT9046 ||
        MachineTypeChoice==Type_HT9045_12Site) &&
            TestIF.iTestMode>=QualSite2X2  &&
            TestIF.iTestMode!=QualSite2X2N &&
            TestIF_File.dSiteYPitch>=65.0)                                      //Steven 20211227 : HT9045跑兩排的, Site Y pitch大於65只能用Arm 1
    {
        if(TestIF_File.iAutoClean_Function && TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==1 && TestIF_File.iAutoClean_SelectArm==1)
        {
            ShowMyMessage("The site Y-pitch can not use arm 2 for auto clean!!");
        }
        TestIF_File.iAutoClean_SelectArm=0;
    }
    else if(TestIF_File.iShuttleMode==1)                                        //JerryYang 20160309 切換為單Arm時 auto clean 也強制切換為單Arm模式
    {
        if(TestIF_File.iShuttle_Sel==0)
        {
            TestIF_File.iAutoClean_SelectArm=0;
        }
        else if(TestIF_File.iShuttle_Sel==1)
        {
            TestIF_File.iAutoClean_SelectArm=1;
        }
    }
    else
    {
        if(bUseTwoArm32Site==true)
        {
            TestIF_File.iAutoClean_SelectArm=2;                                 //強制用兩隻Arm
        }
    }

    TestIF_File.bAutoClean_FailAlarmLowYield                =ReadIniData(szDir, "Configuration","bAutoClean_FailAlarmLowYield", false);
    TestIF_File.iAutoClean_LowYieldLimit                    =ReadIniData(szDir, "Configuration", "iAutoClean_LowYieldLimit", 80);
    TestIF_File.iAutoClean_LowYieldCount                    =ReadIniData(szDir, "Configuration", "iAutoClean_LowYieldCount", 1000);

    TestIF_File.bAutoClean_FailAlarmSiteYieldDifferent      =ReadIniData(szDir, "Configuration", "bAutoClean_FailAlarmSiteYieldDifferent", false);
    TestIF_File.iAutoClean_FailAlarmSiteYield               =ReadIniData(szDir, "Configuration", "iAutoClean_FailAlarmSiteYield", 80);
    TestIF_File.iAutoClean_FailAlarmSiteYieldDifferentCount =ReadIniData(szDir, "Configuration", "iAutoClean_FailAlarmSiteYieldDifferentCount", 1000);

    TestIF_File.bAutoClean_ConseFailureBySocket_Normal      =ReadIniData(szDir, "Configuration", "bAutoClean_ConseFailureBySocket_Normal", false);
    TestIF_File.iAutoClean_ConseFailureCountBySocket_Normal =ReadIniData(szDir, "Configuration", "iAutoClean_ConseFailureCountBySocket_Normal", 10);
    TestIF_File.bAutoClean_ConseFailureBySocket_Retest      =ReadIniData(szDir, "Configuration", "bAutoClean_ConseFailureBySocket_Retest", false);
    TestIF_File.iAutoClean_ConseFailureCountBySocket_Retest =ReadIniData(szDir, "Configuration", "iAutoClean_ConseFailureCountBySocket_Retest", 10);
    TestIF_File.bAutoClean_ConseFailureByHead_Normal        =ReadIniData(szDir, "Configuration", "bAutoClean_ConseFailureByHead_Normal", false);
    TestIF_File.iAutoClean_ConseFailureCountByHead_Normal   =ReadIniData(szDir, "Configuration", "iAutoClean_ConseFailureCountByHead_Normal", 10);
    TestIF_File.bAutoClean_ConseFailureByHead_Retest        =ReadIniData(szDir, "Configuration", "bAutoClean_ConseFailureByHead_Retest", false);
    TestIF_File.iAutoClean_ConseFailureCountByHead_Retest   =ReadIniData(szDir, "Configuration", "iAutoClean_ConseFailureCountByHead_Retest", 10);

    if(CosFunction.bAutoCleanTimeCT==true)                                      //jou 20250102 : auto clean triger time count
    {
        TestIF_File.bEnabledAutoCleanTimeCT                 =ReadIniData(szDir, "Configuration", "bEnabledAutoCleanTimeCT", false);
        TestIF_File.iAutoCleanTimeCT                        =ReadIniData(szDir, "Configuration", "iAutoCleanTimeCT", 10);
    }
    else
    {
        TestIF_File.bEnabledAutoCleanTimeCT=false;
    }

    if(IniConfig.bE43AutoCleanUseHotplate)
        TestIF_File.bAutoClean_UseTray=false;
    else
        TestIF_File.bAutoClean_UseTray=ReadIniData(szDir, "Configuration", "bAutoClean_UseTray", false);
    if(CUSTOMER_CODE==CC_ASE_KaohSiung)
        TestIF_File.bAutoClean_UseTray=true;

    TestIF_File.bAutoClean_UseNSKit=ReadIniData(szDir, "Configuration", "bAutoClean_UseNSKit", false);   //jou 2013-08-08 SPIL use NS Clean kit

    if(TestIF_File.bAutoClean_UseTray)
    {
        TestIF_File.dAutoClean_XPitch=TestIF_File.dAutoClean_XPitch_Tray;
        TestIF_File.dAutoClean_YPitch=TestIF_File.dAutoClean_YPitch_Tray;
        TestIF_File.dAutoClean_XStart=TestIF_File.dAutoClean_XStart_Tray;
        TestIF_File.dAutoClean_YStart=TestIF_File.dAutoClean_YStart_Tray;
        TestIF_File.iAutoClean_XDivision=TestIF_File.iAutoClean_XDivision_Tray;
        TestIF_File.iAutoClean_YDivision=TestIF_File.iAutoClean_YDivision_Tray;
    }
    else
    {
        TestIF_File.dAutoClean_XPitch=TestIF_File.dAutoClean_XPitch_Kit;
        TestIF_File.dAutoClean_YPitch=TestIF_File.dAutoClean_YPitch_Kit;
        TestIF_File.dAutoClean_XStart=TestIF_File.dAutoClean_XStart_Kit;
        TestIF_File.dAutoClean_YStart=TestIF_File.dAutoClean_YStart_Kit;
        TestIF_File.iAutoClean_XDivision=TestIF_File.iAutoClean_XDivision_Kit;
        TestIF_File.iAutoClean_YDivision=TestIF_File.iAutoClean_YDivision_Kit;
    }

    iAutoCleanShuttle=ReadIniData(szDir, "Configuration", "iAutoCleanShuttle", 1); //kevin 20120710
    if(CUSTOMER_CODE==CC_ASE_KaohSiung ||
       CUSTOMER_CODE==CC_OSE ||                                                 //kevin 20211123 add OSE
       CUSTOMER_CODE==CC_ASE_KaohSiung_K3  ||
       CUSTOMER_CODE==CC_ASE_KaohSiung_K11)                                     //Steven 20201210 : Fixed for auto clean offset
    {
        TestIF_File.HotplatlXOffset     =ReadIniData(szDir, "Configuration", "HotplatlXOffset",     0.0);
        TestIF_File.HotplatlYOffset     =ReadIniData(szDir, "Configuration", "HotplatlYOffset",     0.0);
        TestIF_File.HotplatlPickOffset  =ReadIniData(szDir, "Configuration", "HotplatlPickOffset",  0.0);
        TestIF_File.HotplatlPlaceOffset =ReadIniData(szDir, "Configuration", "HotplatlPlaceOffset", 0.0); //kevin 20150209 end add offset
        TestIF_File.HotplatlPitchOffset =ReadIniData(szDir, "Configuration", "HotplatlPitchOffset", 0.0); //kevin 20150526
        TestIF_File.ShuttlePitchOffset  =ReadIniData(szDir, "Configuration", "ShuttlePitchOffset",  0.0); //kevin 20150526
    }
    else
    {
        TestIF_File.HotplatlXOffset     =0.0;
        TestIF_File.HotplatlYOffset     =0.0;
        TestIF_File.HotplatlPickOffset  =0.0;
        TestIF_File.HotplatlPlaceOffset =0.0;
        TestIF_File.HotplatlPitchOffset =0.0;
        TestIF_File.ShuttlePitchOffset  =0.0;
    }

    fMain->AutoCleanStringGrid->ColCount=TestIF_File.iAutoClean_XDivision;      //Steven 20180524 : Fixed for clean count
    fMain->AutoCleanStringGrid->RowCount=(TestIF_File.iAutoClean_YDivision+1)*3;//ChungHung 20140317 alter 如果設為1 RowCount 會不夠 TestIF_File.iAutoClean_YDivision*3 --->  (TestIF_File.iAutoClean_YDivision+1)*3

    fMain->tmyAutoClean->XItem=TestIF_File.iAutoClean_XDivision;
    fMain->tmyAutoClean->YItem=TestIF_File.iAutoClean_YDivision;
    MOT[MMAutoCleanKit].Tray.SetXYItem(TestIF_File.iAutoClean_XDivision, TestIF_File.iAutoClean_YDivision);     //Steven 20160614 : 設定Tray XY Item改用function加上保護

    ReadWriteAutoCleanCount(true);

    if(InitialOK==true)
    {
        if(bAutoCleanOnOff!=TestIF_File.iAutoClean_Function)                    //Steven 20180828 : 修正生產到一半開Auto Clean無效
        {
            bNeedReset=true;
            bAutoCleanOnOff=TestIF_File.iAutoClean_Function;
        }

        if(iBackupCleaningDeveicePices!=TestIF.iAutoClean_DeveicePices || bNeedReset)
        {
            WriteIniData(szDir, "Configuration", "iAutoClean_DeveicePices", TestIF_File.iAutoClean_DeveicePices);
            if(bRunAutoClean==false)
            {
                RecordProcess("Auto clean : reset data because DevicePices was change");
                SetAutoCleanICCount(false);
                ReadWriteAutoCleanCount(false, true);                           //AI(ht9045-v899) 20260414: zero all pad counts when DevicePices or tray layout changes to prevent stale counts causing false WAR1922
            }
        }
    }
    else
    {
        SetAutoCleanICCount(true);
    }

    if(TestIF_File.iAutoClean_Tray==eCKPos_Fix3)
    {
        for(int i=0; i<15; i++)
        {
            if(Prod.iT6CatData[i]==eFix3 || Prod.iT6CatData[i]==eFix6)          //Steven 20120210 Start: Fix3有Bin不能做Auto Clean
                bFix3HasBin=true;
        }
        if(Prod.iIfErrorT6==eFix3 || Prod.iIfErrorT6==eFix6)
            bFix3HasBin=true;

        if(bFix3HasBin==true && TestIF_File.iAutoClean_Function==true)
        {
            TestIF_File.iAutoClean_Function==false;
            WriteIniData(szDir, "Configuration", "iAutoClean_Function",             TestIF_File.iAutoClean_Function);
        }
    }

    if(IniConfig.bEnableAutoCleanFunction==false)
    {
        TestIF_File.iAutoClean_Function=false;
        WriteIniData(szDir, "Configuration", "iAutoClean_Function",             TestIF_File.iAutoClean_Function);
    }

    if(CosFunction.bUseDefineAutoCleanOffset==true)                             //Ifor 20181106 add 新增Auto Clean Offset By 本機
    {
        szDir.sprintf("D:\\HT9045\\IniData\\DefineAutoClean\\AutoClean.Data");

        TestIF_File.iAutoClean_ContactShiftHeight =ReadIniData(szDir, "Configuration", "iAutoClean_ContactShiftHeight",  1000);
        TestIF_File.iAutoClean_ContactCleanHeight =ReadIniData(szDir, "Configuration", "iAutoClean_ContactCleanHeight",  0);
        TestIF_File.iAutoClean_IndexPickOffset    =ReadIniData(szDir, "Configuration", "iAutoClean_IndexPickOffset",     0);    //wei 20150318 Auto clean Index Pick Offset
        TestIF_File.iAutoClean_IndexReleaseOffset =ReadIniData(szDir, "Configuration", "iAutoClean_IndexReleaseOffset",  0);    //Jou 2015-08-22 Auto clean Index Release Offset

        if(IniConfig.bE48_ShuttleUse4Offset_Autoclean==false)                   //Steven 20241102 : fixed for Auto Clean offset
        {
            TestIF_File.iAutoClean_Shuttle1PickOffset =ReadIniData(szDir, "Configuration", "iAutoClean_Shuttle1PickOffset",  -200);
            TestIF_File.iAutoClean_Shuttle1PlaceOffset=ReadIniData(szDir, "Configuration", "iAutoClean_Shuttle1PlaceOffset", 0);
            TestIF_File.iAutoClean_Shuttle1XOffset    =ReadIniData(szDir, "Configuration", "iAutoClean_Shuttle1XOffset",     0);
            TestIF_File.iAutoClean_Shuttle1YOffset    =ReadIniData(szDir, "Configuration", "iAutoClean_Shuttle1YOffset",     0);

            TestIF_File.iAutoClean_Shuttle2PickOffset =ReadIniData(szDir, "Configuration", "iAutoClean_Shuttle2PickOffset",  -200);
            TestIF_File.iAutoClean_Shuttle2PlaceOffset=ReadIniData(szDir, "Configuration", "iAutoClean_Shuttle2PlaceOffset", 0);
            TestIF_File.iAutoClean_Shuttle2XOffset    =ReadIniData(szDir, "Configuration", "iAutoClean_Shuttle2XOffset",     0);
            TestIF_File.iAutoClean_Shuttle2YOffset    =ReadIniData(szDir, "Configuration", "iAutoClean_Shuttle2YOffset",     0);
        }
        else
        {
            TestIF_File.iAutoClean_Shuttle1PickOffset =0;
            TestIF_File.iAutoClean_Shuttle1PlaceOffset=0;
            TestIF_File.iAutoClean_Shuttle1XOffset    =0;
            TestIF_File.iAutoClean_Shuttle1YOffset    =0;

            TestIF_File.iAutoClean_Shuttle2PickOffset =0;
            TestIF_File.iAutoClean_Shuttle2PlaceOffset=0;
            TestIF_File.iAutoClean_Shuttle2XOffset    =0;
            TestIF_File.iAutoClean_Shuttle2YOffset    =0;
        }

        TestIF_File.HotplatlXOffset     =ReadIniData(szDir, "Configuration", "HotplatlXOffset",     0.0);
        TestIF_File.HotplatlYOffset     =ReadIniData(szDir, "Configuration", "HotplatlYOffset",     0.0);
        TestIF_File.HotplatlPickOffset  =ReadIniData(szDir, "Configuration", "HotplatlPickOffset",  0.0);
        TestIF_File.HotplatlPlaceOffset =ReadIniData(szDir, "Configuration", "HotplatlPlaceOffset", 0.0); //kevin 20150209 end add offset
        TestIF_File.HotplatlPitchOffset =ReadIniData(szDir, "Configuration", "HotplatlPitchOffset", 0.0); //kevin 20150526
        TestIF_File.ShuttlePitchOffset  =ReadIniData(szDir, "Configuration", "ShuttlePitchOffset",  0.0); //kevin 20150526
    }
    rgAutoCleanOnOff->ItemIndex =TestIF_File.iAutoClean_Function;
    iBackupAutoClean_Function   =TestIF_File.iAutoClean_Function;
    chkAutoCleanMode1->Checked  =TestIF_File.iAutoClean_Mode & M_INIT_START;
    chkAutoCleanMode2->Checked  =TestIF_File.iAutoClean_Mode & M_INIT_RESTART;
    chkAutoCleanMode3->Checked  =TestIF_File.iAutoClean_Mode & M_FINISH;
    chkAutoCleanMode4->Checked  =TestIF_File.iAutoClean_Mode & M_MANUAL;
    chkAutoCleanMode5->Checked  =TestIF_File.iAutoClean_Mode & M_SOCKET_ALARM;
    chkAutoCleanMode6->Checked  =TestIF_File.iAutoClean_Mode & M_INTERVAL;
    edIntervalContact->Text     =TestIF_File.iAutoClean_IntervalContact;

    if(TestIF_File.iTestMode==_12Site2X6)
        cbbCleanPadCount->Text  =TestIF_File.iAutoClean_DeveicePices;           //Steven 20241113 : for 2x6 auto clean
    else
        edDevicePices->Text     =TestIF_File.iAutoClean_DeveicePices;
    iBackupCleaningDeveicePices =TestIF_File.iAutoClean_DeveicePices;
    edAlarmCount->Text          =TestIF_File.iAutoClean_AlarmCount;

    OutArmSpeed->Text           =TestIF_File.iAutoClean_MotorSpeed[0];
    ShuttleSpeed->Text          =TestIF_File.iAutoClean_MotorSpeed[1];
    IndexArmSpeed->Text         =TestIF_File.iAutoClean_MotorSpeed[2];
    edtInArmZSpeed->Text        =TestIF_File.iAutoClean_MotorSpeed[3];
    ContactMode->ItemIndex      =TestIF_File.iAutoClean_ContactMode;
    edtInArmVacuum->Text        =FormatFloat("0.00", TestIF_File.dAutoClean_InArmVacuum);            //Steven 20250319 : Auto Clean使用另外一組Delay Time
    edtInArmAirOn->Text         =FormatFloat("0.00", TestIF_File.dAutoClean_InArmAirOn);
    edtIndexVacuum->Text        =FormatFloat("0.00", TestIF_File.dAutoClean_IndexVacuum);
    edtIndexAirOn->Text         =FormatFloat("0.00", TestIF_File.dAutoClean_IndexAirOn);

    edDropOffset1->Text         =ConvertTouMType(TestIF_File.iAutoCleanDropHigh);                           //kevin 20180717 autoClean drop high
    edContactTime->Text         =(double)TestIF_File.iAutoClean_ContactTime/10.0;
    edACContactCount->Text      =TestIF_File.iAutoClean_ContactCount;
    rgKitPosition->ItemIndex    =TestIF_File.iAutoClean_Tray;                   //0 :Fix3 1:hotplate 2 2:Clean kit 3:Clean Air      //Steven 20120217 : 選擇把CleanPad放在哪邊
    if(CosFunction.bSmartAutoClean)                                             //Sam 20230111 : Smart Auto Clean
    {
        chkACSmart->Checked                 =TestIF_File.bACSmart;
        edtACSmart->Text                    =TestIF_File.iACSmart_Count;
        edACSmartCTF->Text                  =TestIF_File.iACSmart_Count_CTF;    //Sam 20240726 : AI Clean
        cbbACSmart_ContactMode->ItemIndex   =TestIF_File.iACSmart_ContactMode;
        edtACSmart_DropOffset1->Text        =ConvertTouMType(TestIF_File.iACSmart_DropHigh);
        edtACSmart_ContactTime->Text        =(double)TestIF_File.iACSmart_ContactTime/10.0;
        edtACSmart_ACContactCount->Text     =TestIF_File.iACSmart_ContactCount;

        edAdaptiveIntervalMax->Text         =TestIF_File.iAdaptiveACIntervalMax;
        edAdaptiveIntervalMin->Text         =TestIF_File.iAdaptiveACIntervalMin;
        edAdaptiveIntervalAdj->Text         =TestIF_File.iAdaptiveACIntervalAdj;
    }
    SetArmCaption();
    edIndexArmAutoCleanCnt->Text=TestIF_File.iIndexArmAutoCleanCnt;             //Sam 20250820 : AutoClean 在 Index Arm 下壓清潔一次就++
    if(TestIF_File.iAutoClean_DevicePinCount==0)
        TestIF_File.iAutoClean_DevicePinCount=DeviceForm_File.iPinCT;
    edPinsCount->Text=TestIF_File.iAutoClean_DevicePinCount;

    edPinSingleGf->Text=TestIF_File.fAutoClean_DevicePinForceGf;

    if(TestIF_File.fAutoClean_ForcePerPin==0.0)                                 //kevin 20150826 add
    {
        edPinSingleN->Text=DeviceForm_File.ForcePerPinN;
    }
    else
    {
        edPinSingleN->Text =FormatFloat("0.0000", TestIF_File.fAutoClean_ForcePerPin);
    }

    edtAutoCleanDieForce->Text      =FormatFloat("0.00", TestIF_File.fAutoClean_DieForce);          //Steven 20240719 : Die force for auto clean
    edAutoCleanAirForce_Kg->Text    =FormatFloat("0.00", TestIF_File.fAutoClean_AireForce);         //Ifor 20190506 :TStaticText  ==>  TEdit
    edACContactShiftHeight->Text    =ConvertTouMType(TestIF_File.iAutoClean_ContactShiftHeight);
    edACContactCleanHeight->Text    =ConvertTouMType(TestIF_File.iAutoClean_ContactCleanHeight);

    edIndexPickOffset->Text         =ConvertTouMType(TestIF_File.iAutoClean_IndexPickOffset);       //wei 20150318 Auto clean Index Pick Offset
    edIndexReleaseOffset->Text      =ConvertTouMType(TestIF_File.iAutoClean_IndexReleaseOffset);    //Jou 2015-08-22 Auto clean Index Release Offset
    edShuttle1PickOffset->Text      =ConvertToMMType(TestIF_File.iAutoClean_Shuttle1PickOffset);    //2008/10/22 lee

    edtShuttle1PlaceOffset->Text    =ConvertToMMType(TestIF_File.iAutoClean_Shuttle1PlaceOffset);
    edtShuttle1XOffset->Text        =ConvertToMMType(TestIF_File.iAutoClean_Shuttle1XOffset);
    edtShuttle1YOffset->Text        =ConvertToMMType(TestIF_File.iAutoClean_Shuttle1YOffset);
    edCleanPadDeviation->Text       =ConvertToMMType(TestIF_File.iPadThickness);                    //kevin 20180630 add clean pad - devive

    edShuttle2PickOffset->Text      =ConvertToMMType(TestIF_File.iAutoClean_Shuttle2PickOffset);    //2008/10/22 lee
    edtShuttle2PlaceOffset->Text    =ConvertToMMType(TestIF_File.iAutoClean_Shuttle2PlaceOffset);
    edtShuttle2XOffset->Text        =ConvertToMMType(TestIF_File.iAutoClean_Shuttle2XOffset);
    edtShuttle2YOffset->Text        =ConvertToMMType(TestIF_File.iAutoClean_Shuttle2YOffset);

    if(IniConfig.bSPILFunction==true)                                           //wei 20150708 12site 16site 不能使用雙Arm  //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction
    {
        if(MachineTypeChoice!=Type_HT9046_LS &&                                 //JerryYang 20220922 : LS可以跑16 site雙ARM auto clean
           ((iInArmType==e9045_2x6_8 && iCloseSiteModeFor2x6<=e2x6OneByOne)  || //Steven 20240417 : 12site關中間4site
            (iInArmType==e9045_2x8_8 && iCloseSiteModeFor2x8<=e2x8OneByOne)))
        {
            TestIF_File.bCleanIndexOtherArm=false;
        }
    }
    chkCleanOtherArm->Checked=TestIF_File.bCleanIndexOtherArm;

    rgAutoCleanSelectArm->ItemIndex  =TestIF_File.iAutoClean_SelectArm;         //ChungHung 20131218 add for SCK request  //0:Arm1 1:Arm2 2:Arm1 & Arm2

    cbFailAlarmLowYield->Checked     =TestIF_File.bAutoClean_FailAlarmLowYield;
    edtLowYieldLimit->Text           =TestIF_File.iAutoClean_LowYieldLimit;
    edtLowYieldCount->Text           =TestIF_File.iAutoClean_LowYieldCount;

    cbFailAlarmSiteYieldDifferent->Checked =TestIF_File.bAutoClean_FailAlarmSiteYieldDifferent;
    edtFailAlarmSiteYield->Text            =TestIF_File.iAutoClean_FailAlarmSiteYield;
    edtFailAlarmSiteYieldDifferent->Text   =TestIF_File.iAutoClean_FailAlarmSiteYieldDifferentCount;

    cbConseFailureBySocket_Normal->Checked      =TestIF_File.bAutoClean_ConseFailureBySocket_Normal;
    edtConseFailureCountBySocket_Normal->Text   =TestIF_File.iAutoClean_ConseFailureCountBySocket_Normal;
    cbConseFailureBySocket_Retest->Checked      =TestIF_File.bAutoClean_ConseFailureBySocket_Retest;
    edtConseFailureCountBySocket_Retest->Text   =TestIF_File.iAutoClean_ConseFailureCountBySocket_Retest;
    cbConseFailureByHead_Normal->Checked        =TestIF_File.bAutoClean_ConseFailureByHead_Normal;
    edtConseFailureCountByHead_Normal->Text     =TestIF_File.iAutoClean_ConseFailureCountByHead_Normal;
    cbConseFailureByHead_Retest->Checked        =TestIF_File.bAutoClean_ConseFailureByHead_Retest;
    edtConseFailureCountByHead_Retest->Text     =TestIF_File.iAutoClean_ConseFailureCountByHead_Retest;

    if(CosFunction.bAutoCleanTimeCT==true)                                      //jou 20250102 : auto clean triger time count
    {
        chkTimeCT->Checked     =TestIF_File.bEnabledAutoCleanTimeCT;
        edTimeCT->Text         =TestIF_File.iAutoCleanTimeCT;
    }
    chkUseNSKit->Checked= TestIF_File.bAutoClean_UseNSKit;                      //jou 2013-08-08 SPIL use NS Clean kit

    if(TestIF_File.bAutoClean_UseTray)
        rgCleanKitType->ItemIndex=1;
    else
        rgCleanKitType->ItemIndex=0;

    if(rgCleanKitType->ItemIndex==0)
    {
        pgCleanType->ActivePageIndex=0;
    }
    else
    {
        pgCleanType->ActivePageIndex=1;
    }

    rgShuttleCheck->ItemIndex       =iAutoCleanShuttle;

    edtBufferKitLTX->Text=TestIF_File.dBufferKitLTX;
    edtBufferKitLTY->Text=TestIF_File.dBufferKitLTY;
    edtBufferKitLDX->Text=TestIF_File.dBufferKitLDX;
    edtBufferKitLDY->Text=TestIF_File.dBufferKitLDY;
    edtBufferKitRTX->Text=TestIF_File.dBufferKitRTX;
    edtBufferKitRTY->Text=TestIF_File.dBufferKitRTY;
    edtBufferKitRDX->Text=TestIF_File.dBufferKitRDX;
    edtBufferKitRDY->Text=TestIF_File.dBufferKitRDY;

    edtHotplateXOffset->Text        = ConvertToMMType(TestIF_File.HotplatlXOffset);       //kevin 20150720 add Kit or tray Xpos
    edtHotplateYOffset->Text        = ConvertToMMType(TestIF_File.HotplatlYOffset);       //kevin 20150720 add Kit or tray Ypos
    edtHotplatePickOffset->Text     = ConvertToMMType(TestIF_File.HotplatlPickOffset);    //kevin 20150720 add Kit or tray Pick
    edtHotplatePlaceOffset->Text    = ConvertToMMType(TestIF_File.HotplatlPlaceOffset);   //kevin 20150720 add Kit or tray Place

    edtShuttlePitch->Text           = ConvertToMMType(TestIF_File.ShuttlePitchOffset);    //kevin 20150720 add Kit or tray Pick
    edtHotplatePitchOffset->Text    = ConvertToMMType(TestIF_File.HotplatlPitchOffset);   //kevin 20150720 add Kit or tray Place

    ShowTranNToGf();
    ShowCleanContactForce();
    SearchCleanNum();                                                           //Steven 20180524 : Fixed for clean count

    if(IniConfig.bSPILFunction==true)                                           //JerryYang 20220330 SPIL佶融要求把contact offset與auto clean offset切開
    {
        Prod.iAutoCleanZ_Contact[0] =Prod.TestZ1_Test-Offset.iIndexArmContact[0]-Prod.TestZ1_Drop_Offset+TestIF_File.iAutoClean_ContactCleanHeight+TestIF_File.iPadThickness;     //JerryYang 20181119 (Steven) : fix auto clean 高度異常    //Steven 20181108 : 修正Auto Clean位置異常
        Prod.iAutoCleanZ_Drop[0]    =Prod.TestZ1_Test-Offset.iIndexArmContact[0]-Prod.TestZ1_Drop_Offset+TestIF_File.iAutoClean_ContactCleanHeight+TestIF_File.iPadThickness+TestIF_File.iAutoCleanDropHigh;
        Prod.iAutoCleanZ_Shift[0]   =Prod.TestZ1_Test-Offset.iIndexArmContact[0]-Prod.TestZ1_Drop_Offset+TestIF_File.iAutoClean_ContactCleanHeight+TestIF_File.iPadThickness+TestIF_File.iAutoClean_ContactShiftHeight;
        Prod.iAutoCleanZ_Contact[1] =Prod.TestZ2_Test-Offset.iIndexArmContact[1]-Prod.TestZ2_Drop_Offset+TestIF_File.iAutoClean_ContactCleanHeight+TestIF_File.iPadThickness;
        Prod.iAutoCleanZ_Drop[1]    =Prod.TestZ2_Test-Offset.iIndexArmContact[1]-Prod.TestZ2_Drop_Offset+TestIF_File.iAutoClean_ContactCleanHeight+TestIF_File.iPadThickness+TestIF_File.iAutoCleanDropHigh;
        Prod.iAutoCleanZ_Shift[1]   =Prod.TestZ2_Test-Offset.iIndexArmContact[1]-Prod.TestZ2_Drop_Offset+TestIF_File.iAutoClean_ContactCleanHeight+TestIF_File.iPadThickness+TestIF_File.iAutoClean_ContactShiftHeight;
    }
    else
    {
        Prod.iAutoCleanZ_Contact[0] =Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset+TestIF.iAutoClean_ContactCleanHeight+TestIF.iPadThickness;     //JerryYang 20181119 (Steven) : fix auto clean 高度異常    //Steven 20181108 : 修正Auto Clean位置異常
        Prod.iAutoCleanZ_Drop[0]    =Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset+TestIF.iAutoClean_ContactCleanHeight+TestIF.iPadThickness+TestIF_File.iAutoCleanDropHigh;
        Prod.iAutoCleanZ_Shift[0]   =Prod.TestZ1_Test-Prod.TestZ1_Drop_Offset+TestIF.iAutoClean_ContactCleanHeight+TestIF.iPadThickness+TestIF.iAutoClean_ContactShiftHeight;
        Prod.iAutoCleanZ_Contact[1] =Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset+TestIF.iAutoClean_ContactCleanHeight+TestIF.iPadThickness;
        Prod.iAutoCleanZ_Drop[1]    =Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset+TestIF.iAutoClean_ContactCleanHeight+TestIF.iPadThickness+TestIF_File.iAutoCleanDropHigh;
        Prod.iAutoCleanZ_Shift[1]   =Prod.TestZ2_Test-Prod.TestZ2_Drop_Offset+TestIF.iAutoClean_ContactCleanHeight+TestIF.iPadThickness+TestIF.iAutoClean_ContactShiftHeight;
    }

    fMain->bEnableAutoclean();                                                  //Steven 20211104 : 修正Manual Auto Clean的按鈕顯示時機點
    DrawAutoClean();

    fLotInfo->RefreshYieldMonitor();                                            //Sam 20230111 : Smart Auto Clean
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::SaveAutoCleanData()
{
    AnsiString S="";
    AnsiString sData="";
    S=GetLastOpenFN();
    AnsiString szDir="";
    if(CUSTOMER_CODE==CC_TSMC_TAINAN)                                           //wei 20160219 Auto Clean by機台存檔
    {
        szDir.sprintf("D:\\HT9045\\IniData\\DefineAutoClean\\AutoClean.Data");
    }
    else
    {
        szDir.sprintf("%s%s\\HandlerCondition.Data", DataPath, S);
    }

    if(CosFunction.bAutoCleanUseHPSetByRecipe)                                  //Steven 20210825 : Auto Clean使用加熱盤要改成在工作檔設定
    {
        IniConfig.bE43AutoCleanUseHotplate=chkE43->Checked;
    }

    TestIF_File.iAutoClean_Function=rgAutoCleanOnOff->ItemIndex;

    TestIF_File.iAutoClean_Mode = 0;

    if(chkAutoCleanMode1->Checked && chkAutoCleanMode1->Visible) TestIF_File.iAutoClean_Mode |= M_INIT_START;    //kevin 20170126 (Steven) Modify for Auto Clean 元件功能沒開啟就不能使用
    if(chkAutoCleanMode2->Checked && chkAutoCleanMode2->Visible) TestIF_File.iAutoClean_Mode |= M_INIT_RESTART;
    if(chkAutoCleanMode3->Checked && chkAutoCleanMode3->Visible) TestIF_File.iAutoClean_Mode |= M_FINISH;
    if(chkAutoCleanMode4->Checked && chkAutoCleanMode4->Visible) TestIF_File.iAutoClean_Mode |= M_MANUAL;
    if(chkAutoCleanMode5->Checked && chkAutoCleanMode5->Visible) TestIF_File.iAutoClean_Mode |= M_SOCKET_ALARM;
    if(chkAutoCleanMode6->Checked && chkAutoCleanMode6->Visible) TestIF_File.iAutoClean_Mode |= M_INTERVAL;

    if(TestIF_File.iAutoClean_Mode==0)
    {
        TestIF_File.iAutoClean_Function=false;
        rgAutoCleanOnOff->ItemIndex=0;                                          //Ifor 20180730 (Steven) : add Auto Clean 未選取功能強制關閉
    }

    if(CosFunction.bSmartAutoClean)                                             //Sam 20230111 : Smart Auto Clean
    {
        if(TestIF_File.iAutoClean_Function==false && chkACSmart->Checked)
        {
            ShowMyMessage("Please enable auto clean function", "請開啟自動清潔功能");
            return;
        }
    }

    TestIF_File.iAutoClean_IntervalContact      =atoi(edIntervalContact->Text.c_str());

    if(TestIF.iShuttleMode!=0)                                                  //JerryYang 20160308 Start:修正切換模式時 clean pad數量沒有更新的問題
    {
        rgAutoCleanSelectArm->ItemIndex=TestIF.iShuttle_Sel;                    //ChungHung 20131218 add for SCK request  //0:Arm1 1:Arm2 2:Arm1 & Arm2
    }
    TestIF_File.bCleanIndexOtherArm             =chkCleanOtherArm->Checked;

    if(TestIF_File.iAutoClean_Function &&
       (MachineTypeChoice==Type_HT9045 ||
        MachineTypeChoice==Type_HT9046 ||
        MachineTypeChoice==Type_HT9045_12Site) &&
       TestIF.iTestMode>=QualSite2X2  &&
       TestIF.iTestMode!=QualSite2X2N &&
       TestIF_File.dSiteYPitch>=65.0)                                           //Steven 20211227 : HT9045跑兩排的, Site Y pitch大於65只能用Arm 1
    {
        if((TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==1 && TestIF_File.iAutoClean_SelectArm==1) ||
           rgAutoCleanSelectArm->ItemIndex==1)
        {
            ShowMyMessage("The site Y-pitch can not use arm 2 for auto clean!!");
        }
        rgAutoCleanSelectArm->ItemIndex=0;
    }

    TestIF_File.iAutoClean_SelectArm=rgAutoCleanSelectArm->ItemIndex;
    iBackupCleaningDeveicePices =TestIF_File.iAutoClean_DeveicePices;

    SetDeviceMaxMin();
    if(udDeviceCT->Min>=edDevicePices->Text)
    {
        edDevicePices->Text=udDeviceCT->Min;
    }

    if(TestIF_File.iTestMode==_12Site2X6)
        TestIF_File.iAutoClean_DeveicePices=atoi(cbbCleanPadCount->Text.c_str());   //Steven 20241113 : for 2x6 auto clean
    else
        TestIF_File.iAutoClean_DeveicePices=atoi(edDevicePices->Text.c_str());

//    if(chkCleanOtherArm->Checked==true)
//    {
//        if(MachineTypeChoice!=Type_HT9046_LS &&                                 //JerryYang 20220922 : LS可以跑16 site雙ARM auto clean
//           (TestIF_File.iTestMode==_12Site2X6 ||
//            TestIF_File.iTestMode==_10Site2X5 ||
//            TestIF_File.iTestMode==_16Site2X8))
//        {
//            ShowMyMessage("No Use Index Arm 2, Suck Clean Pad Over Y Limit !!");
//            chkCleanOtherArm->Checked=false;
//        }
//        else if(TestIF_File.iAutoClean_DeveicePices<(TestSocket.iShtCnt*2))
//        {
//            ShowMyMessage("Click Clean Index Arm 2 Clean Pad need Double!!");
//            edDevicePices->Text=TestSocket.iShtCnt*2;
//            TestIF_File.iAutoClean_DeveicePices=TestSocket.iShtCnt*2;
//            DrawAutoClean();
//        }
//    }
    TestIF_File.iAutoClean_Tray                 =rgKitPosition->ItemIndex;      //Steven 20120217 : 選擇把CleanPad放在哪邊
    TestIF_File.iIndexArmAutoCleanCnt           =atoi(edIndexArmAutoCleanCnt->Text.c_str());    //Sam 20250820 : AutoClean 在 Index Arm 下壓清潔一次就++
    TestIF_File.iAutoClean_AlarmCount           =atoi(edAlarmCount->Text.c_str());

    TestIF_File.iAutoClean_MotorSpeed[0]        =atoi(OutArmSpeed->Text.c_str());
    TestIF_File.iAutoClean_MotorSpeed[1]        =atoi(ShuttleSpeed->Text.c_str());
    TestIF_File.iAutoClean_MotorSpeed[2]        =atoi(IndexArmSpeed->Text.c_str());
    TestIF_File.iAutoClean_MotorSpeed[3]        =atoi(edtInArmZSpeed->Text.c_str());
    TestIF_File.iAutoClean_ContactMode          =ContactMode->ItemIndex;

    TestIF_File.dAutoClean_InArmVacuum          =atof(edtInArmVacuum->Text.c_str());    //Steven 20250319 : Auto Clean使用另外一組Delay Time
    TestIF_File.dAutoClean_InArmAirOn           =atof(edtInArmAirOn->Text.c_str());
    TestIF_File.dAutoClean_IndexVacuum          =atof(edtIndexVacuum->Text.c_str());
    TestIF_File.dAutoClean_IndexAirOn           =atof(edtIndexAirOn->Text.c_str());

    TestIF_File.iAutoCleanDropHigh              =Get0_01MMType(edDropOffset1->Text.c_str()); //kevin 20180717 autoClean drop high

    double dAutoClean_ContactTime=0.0;
    dAutoClean_ContactTime=atof(edContactTime->Text.c_str())*10.0;
    TestIF_File.iAutoClean_ContactTime          =dAutoClean_ContactTime;
    TestIF_File.iAutoClean_ContactCount         =atoi(edACContactCount->Text.c_str());
    if(CosFunction.bSmartAutoClean)                                             //Sam 20230111 : Smart Auto Clean
    {
        TestIF_File.bACSmart=chkACSmart->Checked;
        TestIF_File.iACSmart_Count          =atoi(edtACSmart->Text.c_str());
        TestIF_File.iACSmart_Count_CTF      =atoi(edACSmartCTF->Text.c_str());  //Sam 20240726 : AI Clean
        TestIF_File.iACSmart_ContactMode    =cbbACSmart_ContactMode->ItemIndex;
        TestIF_File.iACSmart_DropHigh       =Get0_01MMType(edtACSmart_DropOffset1->Text.c_str());
        dAutoClean_ContactTime=atof(edtACSmart_ContactTime->Text.c_str())*10.0;
        TestIF_File.iACSmart_ContactTime    =dAutoClean_ContactTime;
        TestIF_File.iACSmart_ContactCount   =atoi(edtACSmart_ACContactCount->Text.c_str());

        TestIF_File.iAdaptiveACIntervalMax=atoi(edAdaptiveIntervalMax->Text.c_str());
        TestIF_File.iAdaptiveACIntervalMin  =atoi(edAdaptiveIntervalMin->Text.c_str());
        TestIF_File.iAdaptiveACIntervalAdj  =atoi(edAdaptiveIntervalAdj->Text.c_str());
    }
    TestIF_File.fAutoClean_AireForce            =atof(edAutoCleanAirForce_Kg->Text.c_str());//Ifor 20190506 :TStaticText  ==>  TEdit
    TestIF_File.fAutoClean_DieForce             =atof(edtAutoCleanDieForce->Text.c_str());  //Steven 20240719 : Die force for auto clean
    TestIF_File.iAutoClean_DevicePinCount       =atoi(edPinsCount->Text.c_str());
    TestIF_File.fAutoClean_ForcePerPin          =atof(edPinSingleN->Text.c_str());//kevin 2015086 add

    TestIF_File.fAutoClean_DevicePinForceGf     =atof(edPinSingleGf->Text.c_str());
    TestIF_File.iAutoClean_ContactShiftHeight   =Get0_01MMType(edACContactShiftHeight->Text.c_str());
    TestIF_File.iAutoClean_ContactCleanHeight   =Get0_01MMType(edACContactCleanHeight->Text.c_str());
    TestIF_File.iAutoClean_IndexPickOffset      =Get0_01MMType(edIndexPickOffset->Text.c_str());      //wei 20150318 Auto clean Index Pick Offset
    TestIF_File.iAutoClean_IndexReleaseOffset   =Get0_01MMType(edIndexReleaseOffset->Text.c_str());   //Jou 2015-08-22 Auto clean Index Release Offset
    TestIF_File.iAutoClean_Shuttle1PickOffset   =Get0_01MMType(edShuttle1PickOffset->Text.c_str());   //2008/10/22 lee

    TestIF_File.iAutoClean_Shuttle1PlaceOffset  =Get0_01MMType(edtShuttle1PlaceOffset->Text.c_str());
    TestIF_File.iAutoClean_Shuttle1XOffset      =Get0_01MMType(edtShuttle1XOffset->Text.c_str());
    TestIF_File.iAutoClean_Shuttle1YOffset      =Get0_01MMType(edtShuttle1YOffset->Text.c_str());
    TestIF_File.iPadThickness                   =Get0_01MMType(edCleanPadDeviation->Text.c_str()); //kevin 20180630 add clean pad - devive

    TestIF_File.iAutoClean_Shuttle2PickOffset   =Get0_01MMType(edShuttle2PickOffset->Text.c_str());   //2008/10/22 lee
    TestIF_File.iAutoClean_Shuttle2PlaceOffset  =Get0_01MMType(edtShuttle2PlaceOffset->Text.c_str());
    TestIF_File.iAutoClean_Shuttle2XOffset      =Get0_01MMType(edtShuttle2XOffset->Text.c_str());
    TestIF_File.iAutoClean_Shuttle2YOffset      =Get0_01MMType(edtShuttle2YOffset->Text.c_str());

    TestIF_File.dAutoClean_XPitch_Kit           =Get0_01MMType(XPitch1->Text.c_str());
    TestIF_File.dAutoClean_YPitch_Kit           =Get0_01MMType(YPitch1->Text.c_str());
    TestIF_File.dAutoClean_XStart_Kit           =Get0_01MMType(XST1->Text.c_str());
    TestIF_File.dAutoClean_YStart_Kit           =Get0_01MMType(YST1->Text.c_str());
    TestIF_File.iAutoClean_XDivision_Kit        =atoi(XCT1->Text.c_str());
    TestIF_File.iAutoClean_YDivision_Kit        =atoi(YCT1->Text.c_str());

    if(CUSTOMER_CODE==CC_KYEC_LEE)                                              //wei 20150908
    {
        TestIF_File.dAutoClean_XPitch_Tray      =UserDefForm_File[0].XPitch*100;
        TestIF_File.dAutoClean_YPitch_Tray      =UserDefForm_File[0].YPitch*100;
        TestIF_File.dAutoClean_XStart_Tray      =UserDefForm_File[0].XStart*100;
        TestIF_File.dAutoClean_YStart_Tray      =UserDefForm_File[0].YStart*100;
    }
    else
    {
        TestIF_File.dAutoClean_XPitch_Tray      =Get0_01MMType(XPitch2->Text.c_str());
        TestIF_File.dAutoClean_YPitch_Tray      =Get0_01MMType(YPitch2->Text.c_str());
        TestIF_File.dAutoClean_XStart_Tray      =Get0_01MMType(XST2->Text.c_str());
        TestIF_File.dAutoClean_YStart_Tray      =Get0_01MMType(YST2->Text.c_str());
    }
    TestIF_File.iAutoClean_XDivision_Tray       =atoi(XCT2->Text.c_str());
    TestIF_File.iAutoClean_YDivision_Tray       =atoi(YCT2->Text.c_str());
    TestIF_File.cAutoClean_PackageTray          =PageTypeName->Text;            //kevin 20150521

    iAutoCleanShuttle                           =rgShuttleCheck->ItemIndex;

    if(TestIF.iShuttleMode!=0)                                                  //wei 20150519 1-->0
    {
        chkCleanOtherArm->Checked=false;
    }

    TestIF_File.bAutoClean_FailAlarmLowYield = cbFailAlarmLowYield->Checked;
    TestIF_File.iAutoClean_LowYieldLimit     = atoi(edtLowYieldLimit->Text.c_str());
    TestIF_File.iAutoClean_LowYieldCount     = atoi(edtLowYieldCount->Text.c_str());

    TestIF_File.bAutoClean_FailAlarmSiteYieldDifferent      =cbFailAlarmSiteYieldDifferent->Checked;
    TestIF_File.iAutoClean_FailAlarmSiteYield               =atoi(edtFailAlarmSiteYield->Text.c_str());
    TestIF_File.iAutoClean_FailAlarmSiteYieldDifferentCount =atoi(edtFailAlarmSiteYieldDifferent->Text.c_str());

    TestIF_File.bAutoClean_ConseFailureBySocket_Normal      =cbConseFailureBySocket_Normal->Checked;
    TestIF_File.iAutoClean_ConseFailureCountBySocket_Normal =atoi(edtConseFailureCountBySocket_Normal->Text.c_str());
    TestIF_File.bAutoClean_ConseFailureBySocket_Retest      =cbConseFailureBySocket_Retest->Checked;
    TestIF_File.iAutoClean_ConseFailureCountBySocket_Retest =atoi(edtConseFailureCountBySocket_Retest->Text.c_str());
    TestIF_File.bAutoClean_ConseFailureByHead_Normal        =cbConseFailureByHead_Normal->Checked;
    TestIF_File.iAutoClean_ConseFailureCountByHead_Normal   =atoi(edtConseFailureCountByHead_Normal->Text.c_str());
    TestIF_File.bAutoClean_ConseFailureByHead_Retest        =cbConseFailureByHead_Retest->Checked;
    TestIF_File.iAutoClean_ConseFailureCountByHead_Retest   =atoi(edtConseFailureCountByHead_Retest->Text.c_str());

    if(CosFunction.bAutoCleanTimeCT==true)                                      //jou 20250102 : auto clean triger time count
    {
        TestIF_File.bEnabledAutoCleanTimeCT = chkTimeCT->Checked;
        TestIF_File.iAutoCleanTimeCT        = atoi(edTimeCT->Text.c_str());
    }

    if(rgCleanKitType->ItemIndex==1)
        TestIF_File.bAutoClean_UseTray=true;
    else
        TestIF_File.bAutoClean_UseTray=false;

    TestIF_File.dBufferKitLTX=Get0_01MMType(edtBufferKitLTX->Text.c_str());
    TestIF_File.dBufferKitLTY=Get0_01MMType(edtBufferKitLTY->Text.c_str());
    TestIF_File.dBufferKitLDX=Get0_01MMType(edtBufferKitLDX->Text.c_str());
    TestIF_File.dBufferKitLDY=Get0_01MMType(edtBufferKitLDY->Text.c_str());
    TestIF_File.dBufferKitRTX=Get0_01MMType(edtBufferKitRTX->Text.c_str());
    TestIF_File.dBufferKitRTY=Get0_01MMType(edtBufferKitRTY->Text.c_str());
    TestIF_File.dBufferKitRDX=Get0_01MMType(edtBufferKitRDX->Text.c_str());
    TestIF_File.dBufferKitRDY=Get0_01MMType(edtBufferKitRDY->Text.c_str());

    if((iBackupCleaningDeveicePices!=TestIF_File.iAutoClean_DeveicePices) ||    //ChungHung 201304226修正變換AutoClean Tray盤資料時圖片不會及時更新
       (TestIF.bAutoClean_UseTray!=TestIF_File.bAutoClean_UseTray       ) ||
       (TestIF_File.bAutoClean_UseTray==false && (TestIF_File.iAutoClean_XDivision!=TestIF_File.iAutoClean_XDivision_Kit  || TestIF_File.iAutoClean_YDivision!=TestIF_File.iAutoClean_YDivision_Kit))    ||
       (TestIF_File.bAutoClean_UseTray==true  && (TestIF_File.iAutoClean_XDivision!=TestIF_File.iAutoClean_XDivision_Tray || TestIF_File.iAutoClean_YDivision!=TestIF_File.iAutoClean_YDivision_Tray))   ||
       (iBackupAutoClean_Function!=TestIF_File.iAutoClean_Function) ||          //KEVIN 20120217
       (CosFunction.bCanChangeAutoCleanCount &&
        atoi(edCleaningCount->Text.c_str())!=iAutoCleanCnt))                    //JerryYang 20171128 (Steven) auto clean 清潔次數可修改
    {
        TestIF.bAutoClean_UseTray=TestIF_File.bAutoClean_UseTray;
        if(TestIF_File.bAutoClean_UseTray)
        {
            TestIF_File.dAutoClean_XPitch=TestIF_File.dAutoClean_XPitch_Tray;
            TestIF_File.dAutoClean_YPitch=TestIF_File.dAutoClean_YPitch_Tray;
            TestIF_File.dAutoClean_XStart=TestIF_File.dAutoClean_XStart_Tray;
            TestIF_File.dAutoClean_YStart=TestIF_File.dAutoClean_YStart_Tray;
            TestIF_File.iAutoClean_XDivision=TestIF_File.iAutoClean_XDivision_Tray;
            TestIF_File.iAutoClean_YDivision=TestIF_File.iAutoClean_YDivision_Tray;
        }
        else
        {
            TestIF_File.dAutoClean_XPitch=TestIF_File.dAutoClean_XPitch_Kit;
            TestIF_File.dAutoClean_YPitch=TestIF_File.dAutoClean_YPitch_Kit;
            TestIF_File.dAutoClean_XStart=TestIF_File.dAutoClean_XStart_Kit;
            TestIF_File.dAutoClean_YStart=TestIF_File.dAutoClean_YStart_Kit;
            TestIF_File.iAutoClean_XDivision=TestIF_File.iAutoClean_XDivision_Kit;
            TestIF_File.iAutoClean_YDivision=TestIF_File.iAutoClean_YDivision_Kit;
        }

        if(bRunAutoClean==false)
        {
            RecordProcess("Auto clean : reset data because AutoClean Kit Data was change");

            for(int Y=0; Y<MOT[MMAutoCleanKit].Tray.YItem; Y++)                 //Steven 20180524 : Fixed for clean count
            {
                for(int X=0; X<MOT[MMAutoCleanKit].Tray.XItem; X++)
                {
                    if(MOT[MMAutoCleanKit].Tray.Data[X][Y]!=NULL_IC)            //Steven 20210127 : 修正Auto clean存檔時候, 會改變Count
                        SetAutoCleanStringGrid(X, Y+1, edCleaningCount->Text);
                }
            }
            ReadWriteAutoCleanCount(false);
        }
    }

    iBackupAutoClean_Function=TestIF_File.iAutoClean_Function;                  //Ifor 20171024 : add 避免造成資料異常 rgAutoCleanOnOff->ItemIndex => TestIF_File.iAutoClean_Function
    TestIF_File.bAutoClean_UseNSKit=chkUseNSKit->Checked;                       //jou 2013-08-08 SPIL use NS Clean kit

    TestIF_File.HotplatlPickOffset              =Get0_01MMType(edtHotplatePickOffset->Text.c_str());    //kevin 20150720 add kit or tray pick
    TestIF_File.HotplatlPlaceOffset             =Get0_01MMType(edtHotplatePlaceOffset->Text.c_str());   //kevin 20150720 add kit or tray place
    TestIF_File.HotplatlXOffset                 =Get0_01MMType(edtHotplateXOffset->Text.c_str());       //kevin 20150720 add kit or tray Xpos
    TestIF_File.HotplatlYOffset                 =Get0_01MMType(edtHotplateYOffset->Text.c_str());       //kevin 20150720 add kit or tray Ypos

    TestIF_File.HotplatlPitchOffset             =Get0_01MMType(edtHotplatePitchOffset->Text.c_str());   //kevin 20150720 addkit or tray pitch
    TestIF_File.ShuttlePitchOffset              =Get0_01MMType(edtShuttlePitch->Text.c_str());          //kevin 20150720 add shuttle Pitch

    if(CosFunction.bAutoCleanUseHPSetByRecipe)                                  //Steven 20210825 : Auto Clean使用加熱盤要改成在工作檔設定
    {
        WriteIniData(szDir, "Configuration", "bAutoCleanUseHotplate",       IniConfig.bE43AutoCleanUseHotplate);
//        if(TestIF_File.iAutoClean_Function && IniConfig.bE43AutoCleanUseHotplate)
        if(IniConfig.bE43AutoCleanUseHotplate)   //Ifor 20251204 fix:
        {
            HotPlateForm_File.iPlateSelect=2;
            HotPlateForm.iPlateSelect=2;
            WriteIniData(szDir, "Hotplate Form", "Using Flag", HotPlateForm.iPlateSelect);
        }
    }
    WriteIniData(szDir, "Configuration", "iAutoClean_Function",             TestIF_File.iAutoClean_Function);
    WriteIniData(szDir, "Configuration", "iAutoClean_Mode",                 TestIF_File.iAutoClean_Mode);
    WriteIniData(szDir, "Configuration", "iAutoClean_IntervalContact",      TestIF_File.iAutoClean_IntervalContact);
    WriteIniData(szDir, "Configuration", "iAutoClean_DeveicePices",         TestIF_File.iAutoClean_DeveicePices);
    WriteIniData(szDir, "Configuration", "iAutoClean_Tray",                 TestIF_File.iAutoClean_Tray);   //Steven 20120217 : 選擇把CleanPad放在哪邊
    WriteIniData(szDir, "Configuration", "iAutoClean_AlarmCount",           TestIF_File.iAutoClean_AlarmCount);
    WriteIniData(szDir, "Configuration", "iAutoClean_iPadThickness",        TestIF_File.iPadThickness);//kevin 20180630 add clean pad - device

    WriteIniData(szDir, "Configuration", "iAutoClean_MotorSpeed[0]",        TestIF_File.iAutoClean_MotorSpeed[0]);
    WriteIniData(szDir, "Configuration", "iAutoClean_MotorSpeed[1]",        TestIF_File.iAutoClean_MotorSpeed[1]);
    WriteIniData(szDir, "Configuration", "iAutoClean_MotorSpeed[2]",        TestIF_File.iAutoClean_MotorSpeed[2]);
    WriteIniData(szDir, "Configuration", "iAutoClean_MotorSpeed[3]",        TestIF_File.iAutoClean_MotorSpeed[3]);
    WriteIniData(szDir, "Configuration", "iAutoClean_ContactMode",          TestIF_File.iAutoClean_ContactMode);
    WriteIniData(szDir, "Configuration", "iAutoClean_DropHigh",             TestIF_File.iAutoCleanDropHigh);//kevin 20180717 autoClean drop high

    WriteIniData(szDir, "AutoClean", "InArmVacuum",                         TestIF_File.dAutoClean_InArmVacuum);       //Steven 20250319 : Auto Clean使用另外一組Delay Time
    WriteIniData(szDir, "AutoClean", "InArmAirOn",                          TestIF_File.dAutoClean_InArmAirOn);
    WriteIniData(szDir, "AutoClean", "IndexVacuum",                         TestIF_File.dAutoClean_IndexVacuum);
    WriteIniData(szDir, "AutoClean", "IndexAirOn",                          TestIF_File.dAutoClean_IndexAirOn);

    WriteIniData(szDir, "Configuration", "iAutoClean_ContactTime",          TestIF_File.iAutoClean_ContactTime);
    WriteIniData(szDir, "Configuration", "iAutoClean_ContactCount",         TestIF_File.iAutoClean_ContactCount);
    if(CosFunction.bSmartAutoClean)                                             //Sam 20230111 : Smart Auto Clean
    {
        WriteIniData(szDir, "Configuration", "bACSmart",                    TestIF_File.bACSmart);
        WriteIniData(szDir, "Configuration", "iACSmart_Count",              TestIF_File.iACSmart_Count);
        WriteIniData(szDir, "Configuration", "iACSmart_Count CTF",          TestIF_File.iACSmart_Count_CTF);    //Sam 20240726 : AI Clean
        WriteIniData(szDir, "Configuration", "iACSmart_ContactMode",        TestIF_File.iACSmart_ContactMode);
        WriteIniData(szDir, "Configuration", "iACSmart_DropHigh",           TestIF_File.iACSmart_DropHigh);
        WriteIniData(szDir, "Configuration", "iACSmart_ContactTime",        TestIF_File.iACSmart_ContactTime);
        WriteIniData(szDir, "Configuration", "iACSmart_ContactCount",       TestIF_File.iACSmart_ContactCount);

        WriteIniData(szDir, "Configuration", "iAdaptiveACIntervalMax",      TestIF_File.iAdaptiveACIntervalMax);
        WriteIniData(szDir, "Configuration", "iAdaptiveACIntervalMin",      TestIF_File.iAdaptiveACIntervalMin);
        WriteIniData(szDir, "Configuration", "iAdaptiveACIntervalAdj",      TestIF_File.iAdaptiveACIntervalAdj);
    }
    WriteIniData(szDir, "Configuration", "fAutoClean_AireForce",            TestIF_File.fAutoClean_AireForce);
    if(INSTALL_DOUBLE_EP==1)
        WriteIniData(szDir, "Configuration", "fAutoClean_DieForce",         TestIF_File.fAutoClean_DieForce);   //Steven 20240719 : Die force for auto clean
    WriteIniData(szDir, "Configuration", "iAutoClean_DevicePinCount",       TestIF_File.iAutoClean_DevicePinCount);
    WriteIniData(szDir, "Configuration", "fAutoClean_ForcePerPin",          TestIF_File.fAutoClean_ForcePerPin);//kevin 20150826
    WriteIniData(szDir, "Configuration", "fAutoClean_DevicePinForceGf",     TestIF_File.fAutoClean_DevicePinForceGf);

    WriteIniData(szDir, "Configuration", "dAutoClean_XPitch_Kit",    TestIF_File.dAutoClean_XPitch_Kit);
    WriteIniData(szDir, "Configuration", "dAutoClean_YPitch_Kit",    TestIF_File.dAutoClean_YPitch_Kit);
    WriteIniData(szDir, "Configuration", "dAutoClean_XStart_Kit",    TestIF_File.dAutoClean_XStart_Kit);
    WriteIniData(szDir, "Configuration", "dAutoClean_YStart_Kit",    TestIF_File.dAutoClean_YStart_Kit);
    WriteIniData(szDir, "Configuration", "iAutoClean_XDivision_Kit", TestIF_File.iAutoClean_XDivision_Kit);
    WriteIniData(szDir, "Configuration", "iAutoClean_YDivision_Kit", TestIF_File.iAutoClean_YDivision_Kit);

    WriteIniData(szDir, "Configuration", "dAutoClean_XPitch_Tray",   TestIF_File.dAutoClean_XPitch_Tray);
    WriteIniData(szDir, "Configuration", "dAutoClean_YPitch_Tray",   TestIF_File.dAutoClean_YPitch_Tray);
    WriteIniData(szDir, "Configuration", "dAutoClean_XStart_Tray",   TestIF_File.dAutoClean_XStart_Tray);
    WriteIniData(szDir, "Configuration", "dAutoClean_YStart_Tray",   TestIF_File.dAutoClean_YStart_Tray);
    WriteIniData(szDir, "Configuration", "iAutoClean_XDivision_Tray",TestIF_File.iAutoClean_XDivision_Tray);
    WriteIniData(szDir, "Configuration", "iAutoClean_YDivision_Tray",TestIF_File.iAutoClean_YDivision_Tray);
    WriteIniData(szDir, "Configuration", "cAutoClean_PackageTray",   TestIF_File.cAutoClean_PackageTray);  //kevin 20150521
    WriteIniData(szDir, "Configuration", "iCleanIndexOtherArm",      TestIF_File.bCleanIndexOtherArm);

    WriteIniData(szDir, "Configuration", "iAutoClean_SelectArm",                        TestIF_File.iAutoClean_SelectArm);

    WriteIniData(szDir, "Configuration", "bAutoClean_FailAlarmLowYield",                TestIF_File.bAutoClean_FailAlarmLowYield);
    WriteIniData(szDir, "Configuration", "iAutoClean_LowYieldLimit",                    TestIF_File.iAutoClean_LowYieldLimit);
    WriteIniData(szDir, "Configuration", "iAutoClean_LowYieldCount",                    TestIF_File.iAutoClean_LowYieldCount);

    WriteIniData(szDir, "Configuration", "bAutoClean_FailAlarmSiteYieldDifferent",      TestIF_File.bAutoClean_FailAlarmSiteYieldDifferent);
    WriteIniData(szDir, "Configuration", "iAutoClean_FailAlarmSiteYield",               TestIF_File.iAutoClean_FailAlarmSiteYield);
    WriteIniData(szDir, "Configuration", "iAutoClean_FailAlarmSiteYieldDifferentCount", TestIF_File.iAutoClean_FailAlarmSiteYieldDifferentCount);

    WriteIniData(szDir, "Configuration", "bAutoClean_ConseFailureBySocket_Normal",      TestIF_File.bAutoClean_ConseFailureBySocket_Normal);
    WriteIniData(szDir, "Configuration", "iAutoClean_ConseFailureCountBySocket_Normal", TestIF_File.iAutoClean_ConseFailureCountBySocket_Normal);
    WriteIniData(szDir, "Configuration", "bAutoClean_ConseFailureBySocket_Retest",      TestIF_File.bAutoClean_ConseFailureBySocket_Retest);
    WriteIniData(szDir, "Configuration", "iAutoClean_ConseFailureCountBySocket_Retest", TestIF_File.iAutoClean_ConseFailureCountBySocket_Retest);
    WriteIniData(szDir, "Configuration", "bAutoClean_ConseFailureByHead_Normal",        TestIF_File.bAutoClean_ConseFailureByHead_Normal);
    WriteIniData(szDir, "Configuration", "iAutoClean_ConseFailureCountByHead_Normal",   TestIF_File.iAutoClean_ConseFailureCountByHead_Normal);
    WriteIniData(szDir, "Configuration", "bAutoClean_ConseFailureByHead_Retest",        TestIF_File.bAutoClean_ConseFailureByHead_Retest);
    WriteIniData(szDir, "Configuration", "iAutoClean_ConseFailureCountByHead_Retest",   TestIF_File.iAutoClean_ConseFailureCountByHead_Retest);

    if(CosFunction.bAutoCleanTimeCT==true)                                      //jou 20250102 : auto clean triger time count
    {
        WriteIniData(szDir, "Configuration", "bEnabledAutoCleanTimeCT",                 TestIF_File.bEnabledAutoCleanTimeCT);
        WriteIniData(szDir, "Configuration", "iAutoCleanTimeCT",                        TestIF_File.iAutoCleanTimeCT);
    }

    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                         //kevin 20150616
        TestIF_File.bAutoClean_UseTray=true;
    WriteIniData(szDir, "Configuration", "bAutoClean_UseTray", TestIF_File.bAutoClean_UseTray);

    WriteIniData(szDir, "Configuration", "iAutoCleanShuttle",  iAutoCleanShuttle);                             //kevin 20120711

    WriteIniData(szDir, "Configuration", "dBufferKitLTX", TestIF_File.dBufferKitLTX);
    WriteIniData(szDir, "Configuration", "dBufferKitLTY", TestIF_File.dBufferKitLTY);
    WriteIniData(szDir, "Configuration", "dBufferKitLDX", TestIF_File.dBufferKitLDX);
    WriteIniData(szDir, "Configuration", "dBufferKitLDY", TestIF_File.dBufferKitLDY);
    WriteIniData(szDir, "Configuration", "dBufferKitRTX", TestIF_File.dBufferKitRTX);
    WriteIniData(szDir, "Configuration", "dBufferKitRTY", TestIF_File.dBufferKitRTY);
    WriteIniData(szDir, "Configuration", "dBufferKitRDX", TestIF_File.dBufferKitRDX);
    WriteIniData(szDir, "Configuration", "dBufferKitRDY", TestIF_File.dBufferKitRDY);

    WriteIniData(szDir, "Configuration", "bAutoClean_UseNSKit",TestIF_File.bAutoClean_UseNSKit);    //jou 2013-08-08 SPIL use NS Clean kit

    if(CosFunction.bUseDefineAutoCleanOffset==true)                             //Ifor 20181106 add 新增Auto Clean Offset By 本機
    {
        szDir.sprintf("D:\\HT9045\\IniData\\DefineAutoClean\\AutoClean.Data");
    }
    WriteIniData(szDir, "Configuration", "iAutoClean_ContactShiftHeight",   TestIF_File.iAutoClean_ContactShiftHeight);
    WriteIniData(szDir, "Configuration", "iAutoClean_ContactCleanHeight",   TestIF_File.iAutoClean_ContactCleanHeight);
    WriteIniData(szDir, "Configuration", "iAutoClean_IndexPickOffset",      TestIF_File.iAutoClean_IndexPickOffset);          //wei 20150318 Auto clean Index Pick Offset
    WriteIniData(szDir, "Configuration", "iAutoClean_IndexReleaseOffset",   TestIF_File.iAutoClean_IndexReleaseOffset);       //Jou 2015-08-22 Auto clean Index Release Offset

    if(IniConfig.bE48_ShuttleUse4Offset_Autoclean==false)                       //Steven 20241102 : fixed for Auto Clean offset
    {
        WriteIniData(szDir, "Configuration", "iAutoClean_Shuttle1PickOffset",   TestIF_File.iAutoClean_Shuttle1PickOffset);
        WriteIniData(szDir, "Configuration", "iAutoClean_Shuttle1PlaceOffset",  TestIF_File.iAutoClean_Shuttle1PlaceOffset);
        WriteIniData(szDir, "Configuration", "iAutoClean_Shuttle1XOffset",      TestIF_File.iAutoClean_Shuttle1XOffset);
        WriteIniData(szDir, "Configuration", "iAutoClean_Shuttle1YOffset",      TestIF_File.iAutoClean_Shuttle1YOffset);
        WriteIniData(szDir, "Configuration", "iAutoClean_Shuttle2PickOffset",   TestIF_File.iAutoClean_Shuttle2PickOffset);
        WriteIniData(szDir, "Configuration", "iAutoClean_Shuttle2PlaceOffset",  TestIF_File.iAutoClean_Shuttle2PlaceOffset);
        WriteIniData(szDir, "Configuration", "iAutoClean_Shuttle2XOffset",      TestIF_File.iAutoClean_Shuttle2XOffset);
        WriteIniData(szDir, "Configuration", "iAutoClean_Shuttle2YOffset",      TestIF_File.iAutoClean_Shuttle2YOffset);
        WriteIniData(szDir, "Configuration", "ShuttlePitchOffset",              TestIF_File.ShuttlePitchOffset);    //kevin 20150720
    }

    WriteIniData(szDir, "Configuration", "HotplatlXOffset",                 TestIF_File.HotplatlXOffset);       //kevin 20150720
    WriteIniData(szDir, "Configuration", "HotplatlYOffset",                 TestIF_File.HotplatlYOffset);       //kevin 20150720
    WriteIniData(szDir, "Configuration", "HotplatlPickOffset",              TestIF_File.HotplatlPickOffset);    //kevin 20150720
    WriteIniData(szDir, "Configuration", "HotplatlPlaceOffset",             TestIF_File.HotplatlPlaceOffset);   //kevin 20150720
    WriteIniData(szDir, "Configuration", "HotplatlPitchOffset",             TestIF_File.HotplatlPitchOffset);   //kevin 20150720
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::LoadImage()
{
    Image3->Picture->LoadFromFile(BmpPath+"HoatPlate1.bmp");
    Image1->Picture->LoadFromFile(BmpPath+"HoatPlate1.bmp");
}
//------------------------------------------------------------------------------
extern void DoInArm_9045_Type();
int TfCleaning::GetMinCleanPadCount()                                           //Steven 20221006 : 計算最少需要的Clean Pad數量
{
    int iCount=1;
    DoInArm_9045_Type();
    fSetup->CheckSTMMode();                                                     //Steven 20221006 : 確認STM模式
    if(TestIF.iTestMode==DualSite && TestIF_File.iSiteMap[0][1]<=0)
        b1x2SiteAbClosePutDummy=true;
    else
        b1x2SiteAbClosePutDummy=false;

    if(TestIF_File.iTestMode==SingleSite ||
       iInArmType==e9045_1x4_1_Ac        ||
       b1x2SiteAbClosePutDummy==true)
    {
        iCount=1;
    }
    else if(TestIF_File.iTestMode==DualSite     ||
            TestIF_File.iTestMode==DualSite2x1  ||
            TestIF_File.iTestMode==QualSite2X2N)
    {
        iCount=2;
    }
    else if(TestIF_File.iTestMode==TriSite1X3 ||
            TestIF_File.iTestMode==_6Site2X3N)
    {
//        if(iInArmType==e9045_1x3_2_14)                                        //Steven 20240508 : 修正最小值
//            iCount=3;
//        else
            iCount=3;
    }
    else if(TestIF_File.iTestMode==QualSite1X4 ||
            TestIF_File.iTestMode==QualSite2X2)
    {
        iCount=4;
    }
    else if(iCloseSiteModeFor2x8==e2x8Run2x2_13 ||                              //Steven 20250502 : Fixed for 2x8 auto clean
            iCloseSiteModeFor2x8==e2x8Run2x2_14)
    {
        iCount=4;
    }
    else if(TestIF_File.iTestMode==_6Site2X3 ||
            TestIF_File.iTestMode==_8Site2X4 ||
            TestIF_File.iTestMode==_16Site4X4 ||
            TestIF_File.iTestMode==_8Site2X4N ||                                //Wei 20231211 : 2X4NN Mode
            iCloseSiteModeFor2x6>e2x6OneByOne ||
            iCloseSiteModeFor2x8>e2x8OneByOne)
    {
        iCount=8;
    }
    else if(TestIF_File.iTestMode==_12Site2X6)                                  //Steven 20241113 : for 2x6 auto clean
    {
        iCount=12;
    }
    else if(TestIF_File.iTestMode==_10Site2X5  ||
            TestIF_File.iTestMode==_16Site2X8  ||
            TestIF_File.iTestMode==_32Site4X8N)
    {
        iCount=16;
    }
    else
    {
        ShowMyMessage("The mode is not support!!", "Please save state record and provide to HonPrec software engineer", "GetMinCleanPadCount");
    }
    return iCount;
}
//------------------------------------------------------------------------------
bool TfCleaning::CleanPadCountCanSupport2Arm()                                  //Steven 20221006 : 修正雙arm auto clean只有一半的clean pad
{
    bool bSupport2Arm=true;

    if(TestIF_File.iTestMode==SingleSite ||
       iInArmType==e9045_1x4_1_Ac ||
       b1x2SiteAbClosePutDummy==true)
    {
        if(TestIF_File.iAutoClean_DeveicePices==1)
            bSupport2Arm=false;
    }
    else if(TestIF_File.iTestMode==DualSite     ||
            TestIF_File.iTestMode==DualSite2x1  ||
            TestIF_File.iTestMode==QualSite2X2N)
    {
        if(TestIF_File.iAutoClean_DeveicePices<=2)
            bSupport2Arm=false;
    }
    else if(TestIF_File.iTestMode==TriSite1X3 ||
            TestIF_File.iTestMode==_6Site2X3N)
    {
//        if(iInArmType==e9045_1x3_2_14)                                          //Steven 20240508 : 修正最小值
//        {
//            if(TestIF_File.iAutoClean_DeveicePices<=3)
//                bSupport2Arm=false;
//        }
//        else
//        {
            if(TestIF_File.iAutoClean_DeveicePices<=3)
                bSupport2Arm=false;
//        }
    }
    else if(TestIF_File.iTestMode==QualSite1X4 ||
            TestIF_File.iTestMode==QualSite2X2 ||
            TestIF_File.iTestMode==_8Site2X4N)                                  //Wei 20231211 : 2X4NN Mode
    {
        if(TestIF_File.iAutoClean_DeveicePices<=4)
            bSupport2Arm=false;
    }
    else if(TestIF_File.iTestMode==_6Site2X3 ||
            TestIF_File.iTestMode==_8Site2X4 ||
            TestIF_File.iTestMode==_16Site4X4 ||
            iCloseSiteModeFor2x6>e2x6OneByOne ||
            iCloseSiteModeFor2x8>e2x8OneByOne)
    {
        if(TestIF_File.iAutoClean_DeveicePices<=8)
            bSupport2Arm=false;
    }
    else if(TestIF_File.iTestMode==_12Site2X6)                                  //Steven 20241113 : for 2x6 auto clean
    {
        if(TestIF_File.iAutoClean_DeveicePices<=12)
            bSupport2Arm=false;
    }
    else if(TestIF_File.iTestMode==_10Site2X5  ||
            TestIF_File.iTestMode==_16Site2X8  ||
            TestIF_File.iTestMode==_32Site4X8N)
    {
        if(TestIF_File.iAutoClean_DeveicePices<=16)
            bSupport2Arm=false;
    }
    else
    {
        ShowMyMessage("The mode is not support!!", "Please save state record and provide to HonPrec software engineer", "CleanPadCountCanSupport2Arm");
    }
    return bSupport2Arm;
}
//------------------------------------------------------------------------------
int __fastcall TfCleaning::SetDeviceMaxMin()
{
    AnsiString sBuffet;
    int iMin, iCount;
    int iMax;
    int iCol=0, iRow;

    iMin=GetMinCleanPadCount();                                                 //Steven 20221006 : 計算最少需要的Clean Pad數量

    udDeviceCT->Increment=iMin;
    udDeviceCT->Min=iMin;
    if(rgCleanKitType->ItemIndex==0)
    {
        iCol=atoi(XCT1->Text.c_str());
        iRow=atoi(YCT1->Text.c_str());
    }
    else
    {
        iCol=atoi(XCT2->Text.c_str());
        iRow=atoi(YCT2->Text.c_str());
    }
    iMax=iCol*iRow;

    if(iMax<iMin)
    {
        if(InitialOK)
        {
            sBuffet.sprintf("Auto Clean X-Division or Y-Division set Count Error : MIN=%d", udDeviceCT->Min);
            if(TestIF_File.iAutoClean_Function)
                ShowMyMessage(sBuffet);
        }
    }
    else
    {
        if(TestIF_File.iTestMode==_12Site2X6)
        {
            udDeviceCT->Max=16;
        }
        else
        {
            iCount=(ChangeToFloatNonPcnt((double)(iMax), (double)(iMin)));
            if(iCount==1 || iCount%2==0)
                udDeviceCT->Max=iMin*iCount;                                    //指定成最小值的倍數
            else
                udDeviceCT->Max=iMin*(iCount-1);                                //不可以是奇數倍,避免異常
        }
    }

    if(atoi(edDevicePices->Text.c_str())>udDeviceCT->Max)                       //JerryYang 20180829 (Steven) : 新增clean pad數量保護
    {
        edDevicePices->Text=udDeviceCT->Max;
    }

    if(atoi(edDevicePices->Text.c_str())<udDeviceCT->Min)
    {
        edDevicePices->Text=udDeviceCT->Min;
    }

    DrawAutoClean();
    return iCount;
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::FormShow(TObject *Sender)
{
    Left=100;                                                                   //Steven 20220407 : 避免跑到畫面外
    Top =10;
    fShow=true;
    try
    {
        btFocusOnly->SetFocus();                                                //Ifor 20231030 add: 避免修改設定後重新下載顯示異常
    }
    catch(...){}
    cbbSelectTray->Clear();                                                     //kevin 20201104 清除避免纍 加資料
    cbbSelectTray->Text="Select from Database...";                              //kevin 20150521 start
    AnsiString S;
    S.sprintf("Tray Form  ''%s''  ", GetLastOpenFN());
    Caption=S;
    grpCleanPara->Enabled=true;
    grpContactForce->Enabled=true;                                              //JerryYang 20220311 : ATP鎖定Critical parameter
    pnlContactMode->Enabled=true;
    rgAutoCleanOnOff->Enabled=true;
    rgAutoCleanOnOff->SetFocus();
    ShowTranGfToN();
    ShowCleanContactForce();

    if(bHasTrayCSV)                                                             //Steven 20210629 : Tray Form改成CSV
    {
        fConfiguration->sbtReloadTray->Click();
        for(int i=0; i<fConfiguration->strngrdTray->RowCount; i++)
        {
            if(fConfiguration->strngrdTray->Cells[0][i]!="" &&
               fConfiguration->strngrdTray->Cells[0][i]!=" ")
            {
                cbbSelectTray->Items->Add(fConfiguration->strngrdTray->Cells[0][i]);
            }
        }
    }

    cbbCleanPadCount->Visible=(TestIF_File.iTestMode==_12Site2X6);              //Steven 20241113 : for 2x6 auto clean
    edDevicePices->Visible=(TestIF_File.iTestMode!=_12Site2X6);
    udDeviceCT->Visible=(TestIF_File.iTestMode!=_12Site2X6);

    if(CosFunction.bForecePerPinKGf)                                            //JerryYang 20230204 : 輸入每個pin的力量,單位以kgf為主,避免轉換成牛頓又換回KGf會有小數點兩位的差異
    {
        edPinSingleN->Enabled=false;
    }

    pnlKitSetting->Enabled      =true;                                          //Steven 20220218 : 修正, auto clean執行中, 不可以修改Kit格式
    grpCleanPara->Enabled       =fSecurity->Insufficient(43, false);
    grpCleanDevice->Enabled     =fSecurity->Insufficient(97, false);
    grpSpeed->Enabled           =fSecurity->Insufficient(43, false);

    if(IniConfig.bVTESTFunction==true)
    {
        pnlContactMode->Enabled =fSecurity->Insufficient(43, false);
        grpContactForce->Enabled=fSecurity->Insufficient(43, false);
        pnlCleanArm->Enabled    =fSecurity->Insufficient(43, false);
        grpHeight->Enabled      =(AccessLevel>=1);
    }
    else
    {
        grpContactPara->Enabled =fSecurity->Insufficient(43, false);
    }

    grpDieForce->Visible=(INSTALL_DOUBLE_EP==1);                                //Steven 20240719 : Die force for auto clean

    grpTrayData->Enabled        =fSecurity->Insufficient(43, false);
    btnResetCleanCount->Enabled =fSecurity->Insufficient(97, false);
    edIndexArmAutoCleanCnt->Visible=(CUSTOMER_CODE==CC_TERAPOWER);              //Sam 20250820 : AutoClean 在 Index Arm 下壓清潔一次就++
    lbledIndexArmAutoCleanCnt->Visible=(CUSTOMER_CODE==CC_TERAPOWER);
    if(CUSTOMER_CODE==CC_ASE_KaohSiung ||
       CUSTOMER_CODE==CC_OSE ||                                                 //kevin 20180309 add OSE
       CUSTOMER_CODE==CC_ASE_KaohSiung_K3)                                      //kevin 20210929 add 938
    {
        chkAutoCleanMode2->Visible=false;
        chkAutoCleanMode3->Visible=false;
        chkAutoCleanMode5->Visible=false;
        pcCleanYield->Visible=false;
        rgCleanKitType->Enabled=false;
        rgCleanKitType->ItemIndex=1;
        btInclude->Visible=false;
        PageTypeName->Visible=true;
        cbbSelectTray->Visible=true;
        tsKit->TabVisible=false;                                                //kevin 20180309 只顯示 tray
        gbCleanKitOffset->Visible=true;                                         //kevin 20190211 add  change offset
    }

    if(bRunAutoClean==true)
    {
        pnlKitSetting->Enabled=false;                                           //Steven 20220218 : 修正, auto clean執行中, 不可以修改Kit格式
        rgAutoCleanOnOff->Enabled=false;
        udDeviceCT->Enabled=false;
        edAlarmCount->Enabled=false;
        chkCleanOtherArm->Enabled=false;
        rgAutoCleanSelectArm->Enabled=false;
        if(bResetCleanCount==true)
        {
            btnResetCleanCount->Enabled=true;
        }
        else
        {
            btnResetCleanCount->Enabled=false;
        }
    }
    else
    {
        rgAutoCleanOnOff->Enabled       =fSecurity->Insufficient(43, false);
        grpCleanDevice->Enabled         =fSecurity->Insufficient(97, false);
        udDeviceCT->Enabled             =fSecurity->Insufficient(43, false);
        btnResetCleanCount->Enabled     =fSecurity->Insufficient(97, false);
        edAlarmCount->Enabled           =fSecurity->Insufficient(43, false);
        chkCleanOtherArm->Enabled       =fSecurity->Insufficient(43, false);
        rgAutoCleanSelectArm->Enabled   =true;
    }

    if(TestIF.iTestMode==_32Site4X8N)                                           //Steven 20140512: For HT-9047
    {
        chkCleanOtherArm->Enabled=false;
        rgAutoCleanSelectArm->Enabled=false;
    }
    SetDeviceMaxMin();                                                          //JerryYang 20160513
    fCleaning->LoadAutoCleanData();

    LoadImage();

    rgKitPosition->Visible=false;
    Label288->Visible=false;
    bEnterOffset=true;

    if(CLEAN_AIR)
    {
        grpTrayData->Visible=false;
        grpCleanDevice->Visible=false;
    }
    else
    {
        grpTrayData->Visible=true;
        grpCleanDevice->Visible=true;
    }

    if(CosFunction.bAutoCleanAutoSelIndexArm==true)
    {
        rgAutoCleanSelectArm->ItemIndex=0;
        rgAutoCleanSelectArm->Visible=false;
        chkCleanOtherArm->Visible=true;
        sbTrayAssign->Visible=true;
    }
    else
    {
        chkCleanOtherArm->Visible=false;
        chkCleanOtherArm->Checked=false;
        rgAutoCleanSelectArm->Visible=true;
        sbTrayAssign->Visible=false;
    }

    if(CosFunction.bAutoCleanShuttleDisable==true)                              //jou 2013-02-27 Auto Clean disable shuttle sensor detect
    {
        Label21->Visible=false;
        rgShuttleCheck->Visible=false;
        tsOffset->TabVisible=false;
    }
    else
    {
        Label21->Visible=true;
        rgShuttleCheck->Visible=true;
        tsOffset->TabVisible=false;
    }

    gbYieldAlarm->Visible=true;
    chkAutoCleanMode5->Caption="Yield Alarm";

    chkUseNSKit->Visible=(IniConfig.bSPILFunction==true);                       //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction
    SearchCleanNum();

    if(CUSTOMER_CODE==CC_KYEC_LEE ||                                            //20140320 wei   KYEC 低於權限顯示不能修改
       CUSTOMER_CODE==CC_KYEC_XILINX)
    {
        if(AccessLevel<LevelSet.AccessLevel[43])
        {
            grpContactPara->Enabled=false;
            grpTrayData->Enabled=false;
        }
        else
        {
            grpContactPara->Enabled=true;
            grpTrayData->Enabled=true;
        }
    }

    chkE43->Visible=CosFunction.bAutoCleanUseHPSetByRecipe;                     //Steven 20210825 : Auto Clean使用加熱盤要改成在工作檔設定
    if(CosFunction.bAutoCleanUseHPSetByRecipe==false)                           //Steven 20210825 : Auto Clean使用加熱盤要改成在工作檔設定)
        tsKit->Caption=IniConfig.bE43AutoCleanUseHotplate?"Hot Plate":"Kit";
    ChangeEditToHPMode();                                                       //Steven20160628 : 整合Auto Clean畫面顯示
    pnlInArmOffset->Visible=(IniConfig.bE48_ShuttleUse4Offset_Autoclean==false);//20140923 wei : For Shuttle Auto Clean
    pnlNotify->Visible=(IniConfig.bE48_ShuttleUse4Offset_Autoclean);

    if(CosFunction.bLotStartLockCriticalPara &&                                 //JerryYang 20220311 : ATP鎖定Critical parameter
       RunInfo.bLotStart)
    {
        if(bAuthCriticalPara[25])
        {
            grpTrayData->Enabled=false;
            grpContactForce->Enabled=false;
            pnlContactMode->Enabled=false;
        }
    }

    if(CosFunction.bSetContactForceToAutoClean==true)                           //Steven 20250915 : 複製Contact Force給Auto Clean用
    {
        grpContactForce->Enabled=false;
    }

    if(CosFunction.bCanChangeAutoCleanCount==true &&                            //Ifor 20171024 : add 可更改Auto Clean 計數
       bRunAutoClean==false)                                                    //Steven 20220105 : 執行中不可以修改Clean Count
    {
        edCleaningCount->Enabled=true;
    }
    tsSmart->TabVisible=CosFunction.bSmartAutoClean;                            //Sam 20230111 : Smart Auto Clean

    tsTimeCT->Visible=CosFunction.bAutoCleanTimeCT;                             //jou 20250102 : auto clean triger time count
    //這一行請保持在最下面!!-----------------
//    myLog.Do_Log(Sender, asUser, asLogPath);                                  //Steven 20100629
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::ShowTranGfToN()
{
    double f=atof(edPinSingleGf->Text.c_str());
    f=f*9.8/1000.0;
    AnsiString str;
    str.sprintf("%8.4f", f);
    edPinSingleN->Text=str.Trim();
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::edPinSingleGfClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, InputLimit.dForcePerpinHigh, InputLimit.dForcePerpinLow);
    ShowCleanContactForce();
    ShowTranGfToN();
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::ShowTranNToGf()
{
    if(CosFunction.bForecePerPinKGf==false)                                     //JerryYang 20230204 : 輸入每個pin的力量,單位以kgf為主,避免轉換成牛頓又換回KGf會有小數點兩位的差異
    {
        double f=atof(edPinSingleN->Text.c_str());
        f=f*1000.0/9.8;
        AnsiString str="";
        str.sprintf("%8.4f", f);
        edPinSingleGf->Text=str.Trim();
    }
    ShowCleanContactForce();
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::edPinSingleNClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, InputLimit.dForcePerpinNHigh, InputLimit.dForcePerpinNLow);
    ShowCleanContactForce();
    ShowTranNToGf();
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::edPinsCountClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 20000, 1);
    ShowCleanContactForce();
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::ShowCleanContactForce()
{
    double iBallCount;
    double iTotalN;
    double iTotalGfAC;

    double iDeviceN;
    double iDeviceGf;

    double iSingleN;
    double iSingleGf;
    fContact->DutCount();
    iBallCount =atof(edPinsCount->Text.c_str());
    iSingleN   =atof(edPinSingleN->Text.c_str());
    iSingleGf  =atof(edPinSingleGf->Text.c_str());

    iDeviceN   =iBallCount*iSingleN;                                            //device要壓的牛頓
    iDeviceGf  =iBallCount*iSingleGf/1000.0;                                    //device要壓的公斤

    iTotalN   =iDeviceN  *fContact->dDutCount;                                  //1個arm要壓的牛頓
    iTotalGfAC=iDeviceGf *fContact->dDutCount;                                  //1個arm要壓的公斤

    edAutoCleanAirForce_Kg->Color=clWhite;                                      //Ifor 20190506 :TStaticText  ==>  TEdit
    double dMaxLimit=fContact->GetMaxIndexForceLimit();                         //JimmyChiu 20220119 獨立計算空氣壓力
    if(dMaxLimit>0 && iTotalGfAC>dMaxLimit)
    {
        edAutoCleanAirForce_Kg->Color=clRed;                                    //Ifor 20190506 :TStaticText  ==>  TEdit
        iTotalGfAC=dMaxLimit;
    }

    if(IniConfig.bSPILFunction==true ||
       CUSTOMER_CODE==CC_SCS)
    {
        if(iTotalGfAC<16)
        {
            edAutoCleanAirForce_Kg->Color=cl3DLight;                            //Ifor 20190506 :TStaticText  ==>  TEdit
            iTotalGfAC=16;
        }
    }
    edAutoCleanAirForce_Kg->Text=FormatFloat("0.00", iTotalGfAC);               //Ifor 20190506 :TStaticText  ==>  TEdit
    edAutoCleanAirForce_N->Text =FormatFloat("0.00", iTotalN);                  //Ifor 20190506 :TStaticText  ==>  TEdit
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::sbCleanSaveClick(TObject *Sender)
{
    if(IniConfig.bA02DisableSaveParsWhenSwitchToOp==true &&                     //RogerYang 20260305 張寧說時間到都要登出，且不可被更改任何設定(A01_2)
        AccessLevel==0)
    {
        ShowMyMessage("[A01_2]目前已切換到Operator權限，\r\n請重新登入再做設定!");
        Close();
    }

    bool bFix3HasBin=false;                                                     //Steven 20120210 Start: Fix3有Bin不能做Auto Clean

//    if(fCleaning->rgAutoCleanOnOff->ItemIndex==1 &&                           //Steven 20241111 : Mark for 1x4 run 3 col tray auto clean  //Ifor 20180227 (Steven) : add 1*4 模式無法使用 3*x Tray from
//       TestIF.iTestMode==QualSite1X4 &&
//       iInArmType!=e9045_1x4_1_Ac &&                                            //Steven 20200720 : 1x4只開site Ac
//       fCleaning->rgCleanKitType->ItemIndex==1)
//    {
//        if(TrayForm.Loader.XDivision==3)                                       //Steven 20220927 : <4 --> ==3
//        {
//            ShowMyMessage("This Tray form is not supported, Please use the Clean Kit", "Cannot enable the auto clean function.");
//            return;
//        }
//    }

    if(rgCleanKitType->ItemIndex==1)                                            //Jimmychiu 20221027 add only Tray mode
    {
        AnsiString sBuffet="";
        int iXCT2=atoi(XCT2->Text.c_str());
        int iYCT2=atoi(YCT2->Text.c_str());
        if((iXCT2*iYCT2)<udDeviceCT->Min)
        {
            sBuffet.sprintf("Auto Clean X-Division or Y-Division set Count Error : Count=%d", udDeviceCT->Min);   //kevin 20220906
            if(fCleaning->fShow)
                ShowMyMessage(sBuffet);

            return;
        }
    }

    if(rgKitPosition->ItemIndex==0)                                             //Fix 3
    {
        for(int i=0; i<15; i++)
        {
            if((Prod.iT6CatData[i]==eFix3 ||
                Prod.iT6CatData[i]==eFix6))                                     //放在FIX 3
                bFix3HasBin=true;
        }

        if(Prod.iIfErrorT6==eFix3 ||
           Prod.iIfErrorT6==eFix6)                                              //放在FIX 3
            bFix3HasBin=true;

        if(bFix3HasBin==true && rgAutoCleanOnOff->ItemIndex==1)
        {
            ShowMyMessage("Fix 3 already has bin!!", "Cannot enable the auto clean function.");
            rgAutoCleanOnOff->ItemIndex=0;
        }
    }

    SaveAutoCleanData();
    SearchCleanNum();                                                           //Steven 20180524 : Fixed for clean count

    sbCleanSave->Down=false;

    InitialSet();                                                               //kevin 20150720

    fMain->bEnableAutoclean();                                                  //kevin 20150521 autoclean

    if(IniConfig.bEnable_SECS_GEM==true)
    {
        bHasSaveSet=true;                                                       //Ifor 20151204 新增判斷機台有無修改設定檔
    }
    DrawAutoClean();                                                            //JerryYang 20160308 更新AutoClean畫框

    if(CosFunction.bRecipeParameterDefault)                                     //Isaac 20170527 (Steven) defalut值比較功能
    {
        fRPDefault->RP_Name->Clear();
        fRPDefault->RP_Default->Clear();
        fRPDefault->RP_Value->Clear();

        if(CosFunction.bRecipeParameterDefaultLog)                              //Sam 20201209 : Default Recipe ChangeLog
        {
            fSpeed->SearchRecipeParameter(fSpeed->tsAllSpeed, -1);              //Isaac 20171227 (Steven) : AutoSpeed RPD一鍵復原
            fSpeed->SearchRecipeParameter(fSpeed->tsIndex, 0);
            fSpeed->SearchRecipeParameter(fSpeed->tsInArm, 1);
            fSpeed->SearchRecipeParameter(fSpeed->tsOutArm, 2);
            fSpeed->SearchRecipeParameter(fSpeed->tsTrayArm, 3);
            fSpeed->SearchRecipeParameter(fSpeed->tsShuttle, 4);
        }

        SearchRecipeParameter(fCleaning, 0);

        if(CosFunction.bRecipeParameterDefaultLog)                              //Sam 20201209 : Default Recipe ChangeLog
        {
            FTestIF->SearchRecipeParameter(FTestIF, 0);
            fYieldMonitoring->SearchRecipeParameter(fYieldMonitoring,0);
        }

        fRPDefault->CompareRPDefaultAndValue(fRPDefault->RP_Name, fRPDefault->RP_Default, fRPDefault->RP_Value);
    }
    LoadAutoCleanData();
    fMain->BackupSetupFile();                                                   //Ifor 20170620 (wei) add Auto BackUp Setup File & Last Data
    if(CUSTOMER_CODE==CC_GIGAS)
    {
        fRPDefault->ShowMonitoredParameter();                                   //Isaac 20210222 : 全智要求把參數拉出來看，方便監控
    }
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::sbCleanExitClick(TObject *Sender)
{
    Close();
    sbCleanExit->Down=false;
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::XST1KeyPress(TObject *Sender, char &Key)
{
    if(OnlyNumberAndDotInPut(Key)==false)
        Key=NULL;
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::XCT1KeyPress(TObject *Sender, char &Key)
{
    if(OnlyNumberInPut(Key)==false)
        Key=NULL;
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::rgKitPositionClick(TObject *Sender)
{
    SetArmCaption();
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::YCT1Change(TObject *Sender)
{
    TEdit *ptr=(TEdit *)Sender;

    if(IniConfig.bE43AutoCleanUseHotplate==false)
    {
        if(atoi(ptr->Text.c_str())>10)                                          //kevin 20210911 change 4->10
            ptr->Text=10;
    }
//    SetDeviceMaxMin();                                                        //Jimmychiu 20250121 : 調換位置，避免換工單檢測錯誤
}
//------------------------------------------------------------------------------
//Steven 20220120 : DrawAutoClean()的判斷式要跟SetAutoCleanICCount()裡面的一樣
//------------------------------------------------------------------------------
void __fastcall TfCleaning::DrawAutoClean()
{
    iDeviceCount=atoi(edDevicePices->Text.c_str());                             //RogerYang 20260102 : 搬到show之前，避免iDeviceCount未更新
    if(TestIF_File.iTestMode==_12Site2X6)
        iDeviceCount=atoi(cbbCleanPadCount->Text.c_str());                      //Steven 20241113 : for 2x6 auto clean

    if(fShow==false)
        return;

    if(rgCleanKitType->ItemIndex==0 || IniConfig.bE43AutoCleanUseHotplate)
    {
        tmyAutoClean->XItem=atoi(XCT1->Text.c_str());
        tmyAutoClean->YItem=atoi(YCT1->Text.c_str());
    }
    else
    {
        tmyAutoClean->XItem=atoi(XCT2->Text.c_str());
        tmyAutoClean->YItem=atoi(YCT2->Text.c_str());
    }

    tmyAutoClean->SetColorMap(HAS_CLEAN_IC, clInactiveCaption);
    int iXItem=TestIF.iAutoClean_XDivision;
    int iYItem=tmyAutoClean->YItem;
//    iDeviceCount=atoi(edDevicePices->Text.c_str());
//    if(TestIF_File.iTestMode==_12Site2X6)
//        iDeviceCount=atoi(cbbCleanPadCount->Text.c_str());                      //Steven 20241113 : for 2x6 auto clean
    SetDeviceInTray(iXItem, iYItem, iDeviceCount, eUcleanUsed);                 //Jimmychiu 20221027 優化Cleanpad配置方式
}
//------------------------------------------------------------------------------
void TfCleaning::SetDeviceInTray(int iXItem, int iYItem, int iDeviceNum, int iMode)  //Jimmychiu 20221027 優化Cleanpad配置方式
{
    int iHadSetDevNum=0;
    for(int iYPos=0; iYPos<iYItem; iYPos++)
    {
        for(int iXPos=0; iXPos<iXItem; iXPos++)
        {
            if(iHadSetDevNum>=iDeviceNum)
            {
                return;
            }

            if(TestIF_File.iTestMode==TriSite1X3 ||                             //Steven 20240112 : fixed for 1x3_14
               TestIF_File.iTestMode==_6Site2X3N)
            {
//                if(iInArmType==e9045_1x3_2_14)                                  //Steven 20240508 : 修正最小值
//                {
//                    if(iXItem==4 && iXpos==0)
//                        continue;
//                }
//                else
//                {
                    if(iXItem==4 && iXPos==0)
                        continue;
//                }
            }

            if(TestIF_File.iTestMode==_12Site2X6 &&
               (iDeviceNum==12 || iDeviceNum==24))                              //Steven 20241111 : for 1x4 run 3 col tray auto clean  //AI(ht9045-v899) 20260414: fix operator precedence - was iDeviceNum==24 always true
            {
                if(TestIF_File.iAutoClean_XDivision==4)
                {
                    if(iXPos==0)
                        continue;
                }
                else if(TestIF_File.iAutoClean_XDivision==8)
                {
                    if(iXPos==0 || iXPos==1)
                        continue;
                }
            }

            if(TestIF_File.iTestMode==QualSite1X4 &&
               TestIF_File.iAutoClean_XDivision==3)                             //Steven 20241111 : for 1x4 run 3 col tray auto clean
            {
                if(iXPos==0)
                    continue;
            }

            if(TestIF_File.iTestMode==DualSite ||                               //Steven 20190509 : 換位置
               TestIF_File.iTestMode==QualSite2X2N)                             //Steven 20201014 : for 2x2 nn mode auto clean
            {
                if(USE_PICKER_COUNT==0)
                {
                    if(iXPos>=2)
                    {
                        SetCleanCellValue((iXPos*2)-3, iYPos, HAS_CLEAN_IC, iMode);
                        iHadSetDevNum++;
                    }
                    else
                    {
                        SetCleanCellValue(iXPos*2, iYPos, HAS_CLEAN_IC, iMode);
                        iHadSetDevNum++;
                    }
                }
                else
                {
                    SetCleanCellValue(iXPos, iYPos, HAS_CLEAN_IC, iMode);
                    iHadSetDevNum++;
                }
            }
            else
            {
                if(TestIF_File.iTestMode==QualSite2X2 && iAutoCleanUseXPitch==2)
                {
                    SetCleanCellValue(iXPos, iYPos, HAS_CLEAN_IC, iMode);
                    iHadSetDevNum++;
                }
                else if((TestIF_File.iTestMode==_16Site2X8 ||
                         TestIF_File.iTestMode==_32Site4X8N) &&                 //Steven 20250912 : for 16site auto clean with 12x16 Hot plate
                         iXItem==12 && iYItem==4)
                {
                    if(iXPos<8)
                        continue;

                    SetCleanCellValue(iXPos, iYPos, HAS_CLEAN_IC, iMode);
                    iHadSetDevNum++;
                }
                else if(TestIF_File.iAutoClean_XDivision>4 &&
                        ((bCleanKitPitchLess4000 && TestIF_File.iAutoClean_DeveicePices<=4) ||   //Alick 20161107 add 修正CLEAN TRAY XPITCH<13.33 & CleanIC<=4，AUTOCLEAN顯示CLEAN IC取放位置錯誤
                         (iAutoCleanUseXPitch==2 && TestIF_File.iAutoClean_DeveicePices<4)))     //Alick 20170322 (wei) add 1*4時，Tray X>4，iAutoCleanUseXPitch=2，修正使用4顆CLEANPAD時取放會變成兩次 //Sam 20180208 "<=4" => "<4"
                {
                    SetCleanCellValue(iXPos*2, iYPos, HAS_CLEAN_IC, iMode);
                    iHadSetDevNum++;
                }
                else
                {
                    SetCleanCellValue(iXPos, iYPos, HAS_CLEAN_IC, iMode);
                    iHadSetDevNum++;
                }
            }
        }
    }
}
//------------------------------------------------------------------------------
void TfCleaning::SetCleanCellValue(int iXPos, int iYPos, int iType, int iMode)  //Jimmychiu 20221027 統一Cleanpad配置公式  iMode:0=SetCellColorIndex    1=SetTraySingleData
{
    if(iMode==eAutoCleanUsed)                                                   //1=SetTraySingleData
    {
        MOT[MMAutoCleanKit].SetTraySingleData(iXPos, iYPos, iType);
    }
    else                                                                        //0=SetCellColorIndex
    {
        tmyAutoClean->SetCellColorIndex(iXPos, iYPos, iType);
    }
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::udDeviceCTClick(TObject *Sender,
      TUDBtnType Button)
{
    int iPos=udDeviceCT->Position;
    int iDirect=(iPos>iPosTemp)?1:-1;                                           //1是增加, -1是減少

    if((iPos%udDeviceCT->Increment)!=0)                                         //2013-06-11    Dell    Fix Clean pad 只能是Site number 倍數
    {
        edDevicePices->Text=IntToStr(iPosTemp);                                 //Jimmychiu 20221027 fixed int to string
        return;
    }
    else if(iPos==udDeviceCT->Increment)                                        //Jimmychiu 20221027 允許最小site 數值
    {
        //pass
    }
    else if((((int)ChangeToFloatNonPcnt((double)(iPos), (double)(udDeviceCT->Increment)))%2)!=0)                                //Jimmychiu 20221027 後續數量需要成雙避免bug
    {
        udDeviceCT->Position+=(udDeviceCT->Increment*iDirect);
    }

    DrawAutoClean();
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::edCleanCountClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, (atoi(edAlarmCount->Text.c_str()))-1, 0);
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    SetWorkParameter();
    rgAutoCleanOnOff->Enabled=false;
    fShow=false;
    //這一行請保持在最下面!!-----------------
//    myLog.Do_Log(Sender, asUser, asLogPath);                                  //Steven 20100629
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::btnResetCleanCountClick(TObject *Sender)
{
    if(Barcode_Reader(bcAutoClean)==0)                                          // 20140103 wei KYEC Barcode Reader
    {
        return;
    }

    ReadWriteAutoCleanCount(false, true);                                       //Steven 20190523 : 只要按下Reset就先把Clean Count存檔
    fShowBinSelect->ed_AutoCleanCount->Text=0;
    fCleaning->edCleaningCount->Text=0;
    btnResetCleanCount->Enabled=false;
    bResetCleanCount=false;
    SearchCleanNum();                                                           //Steven 20180524 : Fixed for clean count

    if(iAutoCleanAlarm==1)                                                      //Ifor 20180727 : 需有Alarm 才可修改變數避免發生異常
        iAutoCleanAlarm=2;                                                      //Ifor 20180727 ：Auto Clean Clean Count > Alarm Count 0:正常 1: Alarm 2:Clean Count
    SetAutoCleanICCount(false);                                                 //kevin 20150616
    iAutoCleanAlarm=0;                                                          //Ifor 20180727 ：Auto Clean Clean Count > Alarm Count 0:正常 1: Alarm 2:Clean Count
    TestIF_File.iIndexArmAutoCleanCnt=0;                                        //Sam 20250820 : AutoClean 在 Index Arm 下壓清潔一次就++
    fCleaning->edIndexArmAutoCleanCnt->Text=0;
    EventReport(SECS_EVENT.AutoCleanClearCount);                                //wei 20150627
    bErrorAutoClean=false;                                                      //kevin 20200527 add clean
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::XCT1Change(TObject *Sender)
{
    int iData1=atoi(XCT1->Text.c_str());
    int iData2=atoi(XCT2->Text.c_str());

    if(pgCleanType->ActivePageIndex==1)                                         //Steven 20220128 : Fixed for Auto Clean X Item亂跳
    {
//        if(TestIF_File.iTestMode==TriSite1X3 ||                               //Steven 20240416 : 修改1x3使用3ea Clean Pad
//           TestIF_File.iTestMode==_6Site2X3N)
//        {
//            iCount=3;
//        }
//        else if(TestIF_File.iTestMode==_6Site2X3)                               //Steven 20240416 : 修改2x3使用6ea Clean Pad
//        {
//            iCount=6;
//        }
//        else if(TestIF_File.iTestMode==_10Site2X5 &&                            //Steven 20240416 : 修改2x5使用10ea Clean Pad
//                b12SiteRun2x4==false)
//        {
//            iCount=10;
//        }
        if(TestIF_File.iTestMode==_12Site2X6 &&                                 //Steven 20250809 : 修改2x6使用12ea Clean Pad
           b12SiteRun2x4==false &&
           (iData2==6 || iData2==8 || iData2==4 || iData2==12))
        {
            ;
        }
        else if(iData2<8 && iData2>=4)                                          //Alick 20170329 (wei) modify 原本用字串比對數字有錯
        {
            XCT2->Text=4;
        }
        else if(iData2%4==0)                                                    //kevin 20220906
        {
            XCT2->Text=(iData2/4)*4;
        }
        else
        {
            XCT2->Text=UserDefForm_File[TrayForm.Loader.iTrayType].XDivision;
        }
    }
    else
    {
        if(iData1<8 &&
           iData1>=4)                                                           //Alick 20170329 (wei) modify 原本用字串比對數字有錯
        {
            XCT1->Text=4;
        }
        else if(TestIF_File.iTestMode==_10Site2X5 &&                            //Steven 20220125 : for 2x5 mode
                iData1>=10)
        {
            XCT1->Text=10;
        }
        else if(TestIF_File.iTestMode==_12Site2X6 &&                            //Steven 20250809 : 修改2x6使用12ea Clean Pad
                b12SiteRun2x4==false &&
                (iData1==6 || iData1==8 || iData1==4 || iData1==12))
        {
            ;
        }
        else if((TestIF_File.iTestMode==_16Site2X8 ||
                 TestIF_File.iTestMode==_32Site4X8N) &&
                iData1==12)                                                     //Steven 20250912 : for 16site auto clean with 12x16 Hot plate
        {
            XCT1->Text=12;
        }
        else if(iData1>=8)
        {
            XCT1->Text=8;
        }
    }

//    SetDeviceMaxMin();                                                        //Jimmychiu 20250121 : 調換位置，避免換工單檢測錯誤
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::rgCleanKitTypeClick(TObject *Sender)
{
    if(rgCleanKitType->ItemIndex==0)
    {
        pgCleanType->ActivePageIndex=0;
    }
    else
    {
        pgCleanType->ActivePageIndex=1;
    }
    SetDeviceMaxMin();
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::udDeviceCTChangingEx(TObject *Sender,
      bool &AllowChange, short NewValue, TUpDownDirection Direction)
{
    iPosTemp=udDeviceCT->Position;                                              //2013-06-11    Dell    Fix Clean pad 只能是Site number 倍數
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::btIncludeClick(TObject *Sender)
{
    XST2->Text      = GetFloatFormatString(TrayForm.Loader.XStart/100, 3, 3);
    XPitch2->Text   = GetFloatFormatString(TrayForm.Loader.XPitch/100, 3, 3);
    XCT2->Text      = AnsiString(TrayForm.Loader.XDivision);
    YST2->Text      = GetFloatFormatString(TrayForm.Loader.YStart/100, 3, 3);
    YPitch2->Text   = GetFloatFormatString(TrayForm.Loader.YPitch/100, 3, 3);
    YCT2->Text      = 2;                                                        //JerryYang 20160811 Clean tray Y數量預設1->2
    SetDeviceMaxMin();                                                          //wei 20150422 載入Tray時，顯示最少Pad數量
    if(TestIF_File.iTestMode==_12Site2X6)
        cbbCleanPadCount->Text=udDeviceCT->Min;                                 //Steven 20241113 : for 2x6 auto clean
    else
        edDevicePices->Text=udDeviceCT->Min;                                    //wei 20150422 載入Tray時，顯示最少Pad數量
    DrawAutoClean();                                                            //wei 20150422 載入Tray時，顯示最少Pad數量
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::edtLowYieldLimitKeyPress(TObject *Sender,
      char &Key)
{
    if(OnlyNumberInPut(Key)==false)
        Key=NULL;
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::rgAutoCleanOnOffClick(TObject *Sender)
{
    if(rgAutoCleanOnOff->ItemIndex!=TestIF_File.iAutoClean_Function && Barcode_Reader(bcAutoClean)==0)
    {
        rgAutoCleanOnOff->ItemIndex=TestIF_File.iAutoClean_Function;
        return;
    }
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::chkAutoCleanMode6MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if(Barcode_Reader(bcAutoClean)==0)                                          // 20140103 wei KYEC Barcode Reader
    {
        chkAutoCleanMode6->Checked=TestIF_File.iAutoClean_Mode & M_INTERVAL;
        return;
    }
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::sbTrayAssignClick(TObject *Sender)
{
    fMain->MainFormSizeToEpson(false);
    fMain->AutoCleanStringGrid->Visible=true;
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::XPitch2Change(TObject *Sender)
{
    DrawAutoClean();                                                            //jou 2015-03-24 Tray x pitch 變化
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::cbbSelectTrayChange(TObject *Sender)
{
    int iCount=0, iX=0, iB=0;//, iY=0;
    int index=cbbSelectTray->ItemIndex;
    if(bHasTrayCSV)                                                             //Steven 20210810 : Tray Form改成CSV
    {
        if(index<1)
            return;

        fConfiguration->sbtReloadTray->Click();
        PageTypeName->Text  =fConfiguration->strngrdTray->Cells[0][index].Trim();           //"Package Type"
        XST2->Text          =fConfiguration->strngrdTray->Cells[1][index].Trim();           //"X Start Pos"
        YST2->Text          =fConfiguration->strngrdTray->Cells[2][index].Trim();           //"Y Start Pos"
        XPitch2->Text       =fConfiguration->strngrdTray->Cells[3][index].Trim();           //"X Pitch"
        YPitch2->Text       =fConfiguration->strngrdTray->Cells[4][index].Trim();           //"Y Pitch"
        XCT2->Text          =fConfiguration->strngrdTray->Cells[5][index].Trim();           //"Columns (X)"
//        YCT2->Text          =fConfiguration->strngrdTray->Cells[6][index].Trim();           //"Rows (Y)"
        iX                  =atoi(fConfiguration->strngrdTray->Cells[5][index].c_str());    //"Columns (X)
//        iY                  =atoi(fConfiguration->strngrdTray->Cells[6][index].c_str());    //"Rows (Y)")
    }

    iCount=SetDeviceMaxMin();                                                   //wei 20150422 載入Tray時，顯示最少Pad數量
    if(TestIF_File.iTestMode==_12Site2X6)
        cbbCleanPadCount->Text=udDeviceCT->Min;                                 //Steven 20241113 : for 2x6 auto clean
    else
        edDevicePices->Text=udDeviceCT->Min;                                    //wei 20150422 載入Tray時，顯示最少Pad數量
    if((iX*2) >=iCount)
    {
        YCT2->Text=2;
    }
    else
    {
        iB=ChangeToFloatNonPcnt((double)(iCount), (double)(iX));
        YCT2->Text=iB;
    }
}
//------------------------------------------------------------------------------
void TfCleaning::ChangeEditToHPMode()                                           //Steven20160628 : 整合Auto Clean畫面顯示
{
    XCT1->Enabled   =true;                                                      //Ifor 20191119 : add 避免非Auto Clean 執行狀態下被鎖定無法修改
    YCT1->Enabled   =true;
    XCT2->Enabled   =true;
    YCT2->Enabled   =true;

    if(IniConfig.bE43AutoCleanUseHotplate)                                      //ChungHung 20131120 AutoClean use Hotplate1 start
    {
        rgCleanKitType->Visible=false;
        XCT1->Enabled   =false;
        XPitch1->Enabled=false;
        XST1->Enabled   =false;

        YCT1->Enabled   =fSecurity->Insufficient(43, false);
        YPitch1->Enabled=false;
        YST1->Enabled   =false;

        XCT2->Enabled   =false;
        XPitch2->Enabled=false;
        XST2->Enabled   =false;

        YCT2->Enabled   =true;
        YPitch2->Enabled=false;
        YST2->Enabled   =false;
        tabTray->TabVisible=false;
        pgCleanType->ActivePageIndex=0;
    }
    else
    {
        rgCleanKitType->Visible=true;
        XCT1->Enabled   =fSecurity->Insufficient(43, false);
        XPitch1->Enabled=true;
        XST1->Enabled   =true;

        YCT1->Enabled   =fSecurity->Insufficient(43, false);
        YPitch1->Enabled=true;
        YST1->Enabled   =true;
        tabTray->TabVisible=true;
    }

    if(bRunAutoClean==true)
    {
        XCT1->Enabled   =false;
        YCT1->Enabled   =false;
        XCT2->Enabled   =false;
        YCT2->Enabled   =false;
    }

    if(CUSTOMER_CODE==CC_KYEC_LEE)                                              //wei 20150904
    {
        XPitch2->Enabled=false;
        YPitch2->Enabled=false;
        XST2->Enabled   =false;
        YST2->Enabled   =false;
    }
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::DoSetRPDefault(TWinControl *PCtrl, int iSection)    //Isaac 20170527 (Steven) defalut值比較功能
{
    AnsiString szDir="D:\\HT9045\\IniData\\RPDefault.ini", aSection="";
    AnsiString aHintName;
    AnsiString sRet;                                                            //Sam 20201209 : Default Recipe ChangeLog
    if(iSection==0)
        aSection="Auto Clean";

    for(int iP=0; iP<PCtrl->ControlCount; iP++)
    {
        TControl *P=PCtrl->Controls[iP];
        if(dynamic_cast<TWinControl *>(P)!=NULL)
        {
            DoSetRPDefault((TWinControl *) P, iSection);                        // 找該物件附屬的物件(遞迴)
        }
        TEdit        *PEdit      =dynamic_cast<TEdit           *>(P);
        TLabeledEdit *PTLEdit    =dynamic_cast<TLabeledEdit    *>(P);           //Steven 20200715 : 補上TLabeledEdit
        TCheckBox    *PTCheckBox =dynamic_cast<TCheckBox       *>(P);           //Sam 20201209 : Default Recipe ChangeLog

        if(PEdit!=NULL && PEdit->Hint!="")
        {
            aHintName=PEdit->Hint;
            WriteIniData(szDir, aSection, aHintName, PEdit->Text);
        }
        else if(PTLEdit!=NULL && PTLEdit->Hint!="")
        {
            aHintName=PTLEdit->Hint;
            WriteIniData(szDir, aSection, aHintName, PTLEdit->Text);
        }
        else if(PTCheckBox!=NULL && PTCheckBox->Hint!="")                       //Sam 20201209 : Default Recipe ChangeLog
        {
            aHintName=PTCheckBox->Hint;
            sRet=(PTCheckBox->Checked)?"1":"0";
            WriteIniData(szDir, aSection, aHintName, sRet);
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::DoReplyDefaultToForm(TWinControl *PCtrl, int iSection)
{
    AnsiString aHintName;
    AnsiString szDir="D:\\HT9045\\IniData\\RPDefault.ini";
    AnsiString aSection="";
    if(iSection==0)
        aSection="Auto Clean";

    AnsiString Stringtemp="";
    for(int iP=0; iP<PCtrl->ControlCount; iP++)
    {
        TControl *P=PCtrl->Controls[iP];
        if(dynamic_cast<TWinControl *>(P)!=NULL)
        {
            DoReplyDefaultToForm((TWinControl *) P, iSection);                  // 找該物件附屬的物件(遞迴)
        }
        TEdit        *PEdit      =dynamic_cast<TEdit           *>(P);
        TLabeledEdit *PTLEdit    =dynamic_cast<TLabeledEdit    *>(P);
        TCheckBox    *PTCheckBox =dynamic_cast<TCheckBox       *>(P);           //Sam 20201209 : Default Recipe ChangeLog

        if(PEdit!=NULL)
        {
            aHintName=PEdit->Hint;
            if(CheckIniData(szDir, aSection, aHintName))
            {
                Stringtemp  =ReadIniData(szDir, aSection, aHintName, Stringtemp);
                PEdit->Text =Stringtemp;
            }
        }
        else if(PTLEdit!=NULL && PTLEdit->Hint!="")
        {
            aHintName=PTLEdit->Hint;
            if(CheckIniData(szDir, aSection, aHintName))
            {
                Stringtemp   =ReadIniData(szDir, aSection, aHintName, Stringtemp);
                PTLEdit->Text=Stringtemp;
            }
        }
        else if(PTCheckBox!=NULL && PTCheckBox->Hint!="")                       //Sam 20201209 : Default Recipe ChangeLog
        {
            aHintName=PTCheckBox->Hint;
            if(CheckIniData(szDir, aSection, aHintName))
            {
                Stringtemp          =ReadIniData(szDir, aSection, aHintName, Stringtemp);
                PTCheckBox->Checked =(Stringtemp=="1")?true:false;
            }
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::SearchRecipeParameter(TWinControl *PCtrl, int iSection)
{
    AnsiString aHintName;
    AnsiString szDir="D:\\HT9045\\IniData\\RPDefault.ini";
    AnsiString aSection="";
    AnsiString asDefaultValue="";
    if(iSection==0)
        aSection="Auto Clean";

    AnsiString Stringtemp="";
    for(int iP=0; iP<PCtrl->ControlCount; iP++)
    {
        TControl *P=PCtrl->Controls[iP];
        if(dynamic_cast <TWinControl *>(P)!=NULL)
        {
            SearchRecipeParameter((TWinControl *) P,iSection);                  // 找該物件附屬的物件(遞迴)
        }
        TEdit        *PEdit      =dynamic_cast<TEdit           *>(P);
        TLabeledEdit *PTLEdit    =dynamic_cast<TLabeledEdit    *>(P);
        TCheckBox    *PTCheckBox =dynamic_cast<TCheckBox       *>(P);

        if(PEdit!=NULL)
        {
            aHintName=PEdit->Hint;
            if(CheckIniData(szDir, aSection, aHintName))
            {
                asDefaultValue=CheckAndReadIniData(szDir, aSection, aHintName, Stringtemp);
                if(CosFunction.bRecipeParameterDefaultLog &&                    //Sam 20201209 : Default Recipe ChangeLog
                   asDefaultValue=="")
                {
                    //Default 數值被手動刪除掉的話，就不要監控了
                }
                else
                {
                    fRPDefault->RP_Name->Add(aHintName);
                    fRPDefault->RP_Value->Add(PEdit->Text);
                    fRPDefault->RP_Default->Add(asDefaultValue);
                }
            }
        }
        else if(PTLEdit!=NULL)
        {
            aHintName=PTLEdit->Hint;
            if(CheckIniData(szDir, aSection, aHintName))
            {
                asDefaultValue=CheckAndReadIniData(szDir, aSection, aHintName, Stringtemp);
                if(CosFunction.bRecipeParameterDefaultLog &&                    //Sam 20201209 : Default Recipe ChangeLog
                   asDefaultValue=="")
                {
                    //Default 數值被手動刪除掉的話，就不要監控了
                }
                else
                {
                    fRPDefault->RP_Name->Add(aHintName);
                    fRPDefault->RP_Value->Add(PTLEdit->Text);
                    fRPDefault->RP_Default->Add(asDefaultValue);
                }
            }
        }
        else if(PTCheckBox!=NULL)                                               //Sam 20201209 : Default Recipe ChangeLog
        {
            aHintName=PTCheckBox->Hint;
            if(CheckIniData(szDir, aSection, aHintName))
            {
                asDefaultValue=CheckAndReadIniData(szDir, aSection, aHintName, Stringtemp);
                if(CosFunction.bRecipeParameterDefaultLog &&                    //Sam 20201209 : Default Recipe ChangeLog
                   asDefaultValue=="")
                {
                    //Default 數值被手動刪除掉的話，就不要監控了
                }
                else
                {
                    fRPDefault->RP_Name->Add(aHintName);
                    fRPDefault->RP_Value->Add((PTCheckBox->Checked)?"1":"0");
                    fRPDefault->RP_Default->Add(asDefaultValue);
                }
            }
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::FormShortCut(TWMKey &Msg, bool &Handled)
{
    if(Msg.CharCode==VK_F5)                                                     //Sam 20220725 : 介面新增鍵盤 F5 將視窗恢復初始位置
    {
        Left=75;
        Top=10;
    }
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::edtBufferKitLTXClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, (double)InputLimit.iOffsetXYHigh, (double)InputLimit.iOffsetXYLow);       //Steven 20141120 : Modify
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::edtHotplatePickOffsetClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, (double)InputLimit.iOffsetZHigh, (double)InputLimit.iOffsetZLow);       //Steven 20141120 : Modify
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::edACContactCleanHeightClick(TObject *Sender)
{
    if(IniConfig.bSPILFunction)                                                 //JerryYang 20240111 : 矽品二林要求設定auto clean contact height上下限
    {
        fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_DOUBLE, 2, true, InputLimit.dAutoCleanContactHigh, InputLimit.dAutoCleanContactLow);
    }
    else
    {
        fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, (double)InputLimit.iOffsetZHigh, (double)InputLimit.iOffsetZLow);
    }
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::edtLowYieldLimitClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 100, 1);
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::edtLowYieldCountClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 100000, 1);
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::XST1Click(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 0.00, 1000.00);       //Steven 20250812 : min 0.00
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::XCT1Click(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 100, 1);
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::edACContactShiftHeightClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 0.01, 30.0);
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::edDropOffset1Click(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_DOUBLE, 1, true, 10.0, 0.0);
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::edDevicePicesClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, udDeviceCT->Max, udDeviceCT->Min);
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::edAutoCleanAirForce_KgClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_DOUBLE, 2, true, 500.0, 0.0);
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::edAutoCleanAirForce_NClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_DOUBLE, 2, true, 5000.0, 0.0);
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::edtACSmartClick(TObject *Sender)                    //Sam 20230111 : Smart Auto Clean
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 10, 1);
}
//------------------------------------------------------------------------------
bool __fastcall TfCleaning::CheckSmartAutoClean(AnsiString aAlarmCode, AnsiString ErrPart) //Sam 20250916 : Alarm後需要清除資料才能Start          //Sam 20230111 : Smart Auto Clean
{
    bool bFlag=false;
    AnsiString s="", sAlarmMsg="";

    if(CosFunction.bSmartAutoClean==false)
        return false;

    if(IniConfig.bEnableAutoCleanFunction &&
       TestIF.iAutoClean_Function &&
       TestIF.bACSmart &&
       TestIF.iACSmart_Count!=0           &&
       TestIF.iACSmart_Count_CTF!=0)
    {
        if(iRunACSmart==0)                                                      //避免連續 Low Yield
        {
            sACRecAlarmCode=aAlarmCode;                                         //Sam 20250916 : Alarm後需要清除資料才能Start
            sACRecEPortCode=ErrPart;
            if(aAlarmCode=="WAR07301" ||                                        //CTF 連續 Fail
               aAlarmCode=="WAR07321" ||
               aAlarmCode=="WAR07329")
            {
                iACSmartCount_CTF++;
                if(iACSmartCount_CTF==1)                                        //使用原本 AutoClean 參數
                    iACUseParam=1;
                else if(iACSmartCount_CTF<=TestIF.iACSmart_Count_CTF)           //使用第二組 AutoClean 參數
                    iACUseParam=2;
                else                                                            //Alarm
                    iACUseParam=0;
            }
            else
            {
                iACSmartCount++;
                if(iACSmartCount==1)                                            //使用原本 AutoClean 參數
                    iACUseParam=1;
                else if(iACSmartCount<=TestIF.iACSmart_Count)                   //使用第二組 AutoClean 參數
                    iACUseParam=2;
                else                                                            //Alarm
                    iACUseParam=0;
            }
        }

        if(iACUseParam==2)
        {
            Prod.iAutoClean_ContactMode =TestIF_File.iACSmart_ContactMode;
            Prod.iAutoCleanDropHigh     =TestIF_File.iACSmart_DropHigh;
            Prod.iAutoClean_ContactTime =TestIF_File.iACSmart_ContactTime;
            Prod.iAutoClean_ContactCount=TestIF_File.iACSmart_ContactCount;
        }
        else
        {
            Prod.iAutoClean_ContactMode =TestIF_File.iAutoClean_ContactMode;
            Prod.iAutoCleanDropHigh     =TestIF_File.iAutoCleanDropHigh;
            Prod.iAutoClean_ContactTime =TestIF_File.iAutoClean_ContactTime;
            Prod.iAutoClean_ContactCount=TestIF_File.iAutoClean_ContactCount;
        }

        if(iACUseParam==0)
        {
            bShowNoteCleanSocket=true;
            bFlag=false;                                                        //直接報警
        }
        else
        {
            bFlag=true;
            if(iRunACSmart==0)                                                  //避免連續 Low Yield
            {
                iRunACSmart=1;
                sAlarmMsg=GetMyDBIMessage(aAlarmCode);
                s.sprintf("Start smart auto Clean use %d parameter by %s", iACUseParam, sAlarmMsg);
                NewRecordProcess("", s, aAlarmCode);
                fCleaning->ChangeACSmartInterval(-1, s);

                InitialAutoCleanTask();
                InitialShuttleAutoCleanTask();
                InitialIndexAutoCleanTask();
                bIsAutoOneCycle=true;
                fMain->BtnOneCycleClick(fMain);
            }
        }
        fLotInfo->RefreshYieldMonitor();
    }
    else
    {
        bShowNoteCleanSocket=false;
        ResetSmartAutoClean();
    }
    return bFlag;
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::ResetSmartAutoClean()                               //Sam 20230111 : Smart Auto Clean
{
    if(CosFunction.bSmartAutoClean)
    {
        iACUseParam=0;
        iACSmartCount=0;
        iACSmartCount_CTF=0;                                                    //Sam 20240726 : AI Clean
        fLotInfo->RefreshYieldMonitor();
    }
}
//------------------------------------------------------------------------------
void TfCleaning::ChangeACSmartInterval(int iChangeMode, AnsiString sMsg)        //Sam 20240726 : AI Clean
{
    if(CosFunction.bSmartAutoClean && TestIF.bACSmart)
    {
        if(iChangeMode==0)                                                      //清空
        {
            iAdaptiveACInterval=-1;
        }
        else if(iChangeMode==1)                                                 //微調+
        {
            if(iAdaptiveACInterval==TestIF.iAdaptiveACIntervalMin)              //表示良率剛從 Min 回到到 Normal 要回復之前的 AdaptiveACInterval
            {
                iAdaptiveACInterval=iAdaptiveACIntervalNor;
            }
            else
            {
                iAdaptiveACInterval+=TestIF.iAdaptiveACIntervalAdj;
                if(iAdaptiveACInterval>TestIF.iAdaptiveACIntervalMax)
                     iAdaptiveACInterval=TestIF.iAdaptiveACIntervalMax;
            }
        }
        else if(iChangeMode==99)                                                //升到最大
        {
            iAdaptiveACInterval=TestIF.iAdaptiveACIntervalMax;
        }
        else if(iChangeMode==-1)                                                //微調-
        {
            iAdaptiveACInterval-=TestIF.iAdaptiveACIntervalAdj;
            if(iAdaptiveACInterval<TestIF.iAdaptiveACIntervalMin)
                 iAdaptiveACInterval=TestIF.iAdaptiveACIntervalMin;
        }
        else if(iChangeMode==-99)                                               //降到最低
        {
            if(iAdaptiveACInterval!=TestIF.iAdaptiveACIntervalMin)              //表示良率剛從 Normal 掉到 Min 要記錄當下的 AdaptiveACInterval
                iAdaptiveACIntervalNor=iAdaptiveACInterval;

            iAdaptiveACInterval=TestIF.iAdaptiveACIntervalMin;
        }
        else if(iChangeMode==2)                                                 //恢復到標準值
        {
            iAdaptiveACInterval=TestIF_File.iAutoClean_IntervalContact;
        }

        AnsiString sLog="", sFile="", sFileName="", S="";                       //紀錄 AC Smart Interval change log
        sFile.sprintf("%s\\%04d%02d\\", asACSmartIntervalLogPath, SystemYear, SystemMonth); //kevin 20150908
        S=GetLastOpenFN();
        if(DirectoryExists(sFile)==false)
        {
            ForceDirectories(sFile);
        }
        sFileName.sprintf("%s%s_%s.csv", sFile, Now().FormatString("yyyy-mm-dd"), S);
        sLog.sprintf(" %s, %d, %s", Now().FormatString("hh:nn:ss"), iAdaptiveACInterval, sMsg);
        WriteDataToFile(sFileName, sLog);

        AnsiString szDir="";
        szDir.sprintf("%s%s\\HandlerCondition.Data", DataPath, S);
        TestIF.iAutoClean_IntervalContact=iAdaptiveACInterval;
        TestIF_File.iAutoClean_IntervalContact=iAdaptiveACInterval;
        fCleaning->edIntervalContact->Text=iAdaptiveACInterval;
        WriteIniData(szDir, "Configuration", "iAutoClean_IntervalContact", TestIF.iAutoClean_IntervalContact);
    }
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::btnResetIntervalClick(TObject *Sender)
{
    //iAdaptiveACInterval=-1;
    ChangeACSmartInterval(0, "btnResetIntervalClick");                          //Sam 20240726 : AI Clean
    fLotInfo->RefreshYieldMonitor();
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::edAdaptiveIntervalMaxMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 10, true, 9999, 10);
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::edAdaptiveIntervalMinMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 10, true, 9999, 1);
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::edAdaptiveIntervalAdjMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 10, true, 1000, 1);
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::edACSmartCTFMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, 1, 10, 1);
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::edtACSmart_ContactTimeMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, 0.01, 100.0, 0.0);
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::edtACSmartMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, 1, 10, 1);
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::cbbCleanPadCountChange(TObject *Sender)
{
    DrawAutoClean();
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::edTimeCTClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, 1, 5000, 5);
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::edtIndexVacuumClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 10.0, 0.01);
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::btnStartAutoCleanClick(TObject *Sender)             //Ifor 20220415 add:Auto Clean 按鈕移置Clean頁面
{
    fShowBinSelect->btnAutoCleanClick(fShowBinSelect);
}
//------------------------------------------------------------------------------
void __fastcall TfCleaning::edACInitalContactCountClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 100, 0);
}
//---------------------------------------------------------------------------
bool TfCleaning::CheckSmartAutoCleanCanStart()                                  //Sam 20250916 : Alarm後需要清除資料才能Start
{
    if(IniConfig.bEnableAutoCleanFunction &&
       TestIF.iAutoClean_Function         &&
       TestIF.bACSmart                    &&
       TestIF.iACSmart_Count!=0           &&
       TestIF.iACSmart_Count_CTF!=0)
    {
        bool bCanStart=true;

        if(iACSmartCount_CTF==1)
            bCanStart=true;
        else if(iACSmartCount_CTF<=TestIF.iACSmart_Count_CTF)
            bCanStart=true;
        else
            bCanStart=false;

        if(bCanStart)
        {
            if(iACSmartCount==1)
                bCanStart=true;
            else if(iACSmartCount<=TestIF.iACSmart_Count)
                bCanStart=true;
            else
                bCanStart=false;
        }

        if(bCanStart==false)                                                    //Sam 20250916 : Alarm後需要清除資料才能Start
        {
            ShowErrorMessage(sACRecAlarmCode, K_RETRY, MMInterface, false, sACRecEPortCode);
            //ShowMyMessage("Please Reset AI AutoClean","");
        }

        return bCanStart;
    }
    return true;
}
//------------------------------------------------------------------------------
