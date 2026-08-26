//---------------------------------------------------------------------------
// Steven 20170111 : Add SCK_ART.cpp for Auto Retest function in SCK and ATK
//---------------------------------------------------------------------------

#include "MachineDefine.h"
#pragma hdrstop

#include "SCK_ART.h"
#include "cmydef.h"
#include "common.h"
#include "uLotInfo.h"
#include "cprod.h"
#include "atester.h"
#include "main.h"
#include "cSocket.h"
#include "myQwertyKeyBoard.h"
#include "cCounterClear.h"
#include "cMyDB.h"
#include "cTesterIF.h"
#include "cBinSel.h"
#include "cObserver.h"
#include "cSocket.h"
#include "Note.h"
#include "mymessbox.h"
#include "cConfiguration.h"
#include "csystem.h"
#include "TesterTCP.h"
#include "HS_Function.h"
#include "TfFTP.h"

#pragma package(smart_init)
#pragma link "ALed"
#pragma resource "*.dfm"
TfSCKART *fSCKART;
//---------------------------------------------------------------------------
__fastcall TfSCKART::TfSCKART(TComponent* Owner)
    : TForm(Owner)
{
    slExe=new TStringList;
    sSetupFilePath="";
    sLotStartTime="";
    iTesterType=0;                                                              //0: Flex,  1: 93K
    iLOTSTATUS_NONE=0;
    iLOTSTATUS_W=1;
    iLOTSTATUS_T=2;
    iLOTSTATUS_L=3;
    iLOTSTATUS_R=4;
    iLOTSTATUS_F=5;
    iLOTSTATUS_A=6;
    sAlarmPath="D:\\HT9045_Log\\Alarm.txt";
    sLOTSTATUS="NONE";
//    iRTUnitCount=0;
    iWaitGPIBLotR=0;
    iInputJamCnt=0;
    iOutputJamCnt=0;
    //iTCPModeLotState=0;                                                       //Sam 20200311 : Fix TCP ART MODE Mark//Sam 20191113 : TCP ART
    bShowSetting=false;
    bShow=false;

    sInfo_Customer="";                                                          //JerryYang 20220923 : add Lot Info for SPIL
    sInfo_InnerLotID="";
    sInfo_CustLotID="";
    sInfo_CustDevGup="";
    sInfo_DeviceName="";
    sInfo_Stage="";
    sInfo_Step="";
    sInfo_ReportCnt="";
    sInfo_ProgramName="";
    sInfo_TestBinNo="";
    sInfo_TesterID="";
    sInfo_HandlerID="";
    sInfo_Temperauture="";
    sInfo_CurrQty="";
    sInfo_OperatorID="";
    sInfo_BinSet="";                                                            //Jerryyang 20250529 : add

    iInfo_MultiLotCnt=0;

    for(int i=0; i<5; i++)
    {
        sInfoArr_Customer[i]="";
        sInfoArr_InnerLotID[i]="";
        sInfoArr_CustLotID[i]="";
        sInfoArr_CustDevGup[i]="";
        sInfoArr_DeviceName[i]="";
        sInfoArr_Stage[i]="";
        sInfoArr_Step[i]="";
        sInfoArr_ReportCnt[i]="";
        sInfoArr_ProgramName[i]="";
        sInfoArr_TestBinNo[i]="";
        sInfoArr_TesterID[i]="";
        sInfoArr_HandlerID[i]="";
        sInfoArr_Temperauture[i]="";
        sInfoArr_CurrQty[i]="";
        sInfoArr_OperatorID[i]="";
        sInfoArr_BinSet[i]="";                                                  //Jerryyang 20250529 : add
    }

    if(FileExists(sAlarmPath))
        memoAlarmList->Lines->LoadFromFile(sAlarmPath);

    if(CUSTOMER_CODE==CC_SCK)                                                   //Steven 20170829 (wei) : 除了SCK之外,全部的ART都鎖定成93K
    {                                                                           //Steven 20251020 : JSCK工程師要求修改顯示
        lblInputJamCountFix->Visible=false;
        lblInputRemoveCountFix->Visible=false;
        palInputJamCnt->Visible=false;
        palInRemoveCnt->Visible=false;
        lblOutputJamCountFix->Caption="Jam count need RT";
        lblOutputRemoveCountFix->Caption="Remove device";
    }
    else
    {
        iTesterType=1;
    }

    if(CUSTOMER_CODE==CC_TERAPOWER)                                             //Sam 20200203 : 晶兆成英傑要求改顯示 Host Name
    {
        lblTesterType->Caption="Host Name";
    }
}
//---------------------------------------------------------------------------
void __fastcall TfSCKART::FormShow(TObject *Sender)
{
    bShow=true;
    AccessFile(true, -1);
    SettingPanelOnOff(false);
    edtInputCount->Text      =iInputCount;
    edtMRejectCnt->Text      =iManualRejectCnt;                                 //JerryYang 20220923 : SPIL手動輸入reject數量(要重測的)

    if(CUSTOMER_CODE==CC_RF360 ||
       CUSTOMER_CODE==CC_QUALCOMM)                                              //Steven 20251104 : for QTI
    {
        cbLotDeviceCheck->Enabled=true;
    }
    else
    {
        cbLotDeviceCheck->Enabled=false;
    }

    TimerSCKARTFlow->Enabled =true;
    edtVersion->Enabled=!IniConfig.bA10TestModeForART;
    for(int i=0; i< iTestBinCount; i++)                                         //Isaac 20171108
    {
        AnsiString S="";
        S.sprintf("Bin%d",i);
        cbSPBinSelect->Items->Add(S);
    }

    if(CosFunction.iAutoRetestTCPmode==2)                                       //RogerYang 20250917 : 瑞薩FT-CT
        palLotNumber->Caption="Lot ID";

    if(CUSTOMER_CODE==CC_QUALCOMM)                                              //RogerYang 20251224 : Rf360需求 lotcheck加入上下限設定
    {
        lblLdCntLimN->Visible=true;
        edtLdCntLimN->Visible=true;
        lblLdCntLimP->Visible=true;
        edtLdCntLimP->Visible=true;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfSCKART::FormClose(TObject *Sender, TCloseAction &Action)
{
    bShow=false;
    TimerSCKARTFlow->Enabled=false;
}
//---------------------------------------------------------------------------
void __fastcall TfSCKART::FormDestroy(TObject *Sender)
{
    try
    {
        slExe->Clear();
        delete slExe;
        TimerSCKARTFlow->Enabled=false;
        memoAlarmList->Lines->SaveToFile(sAlarmPath);
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfSCKART::FormDestroy");
    }
    LogSoftwareOffTime("TfSCKART, FormDestroy");                                //Steven 20210526 : 紀錄軟體執行時間
}
//---------------------------------------------------------------------------
void TfSCKART::SetSetupFilePath()
{
    sSetupFilePath=GetRecipePath();
    MyForceDirectories(sSetupFilePath);
    sSetupFilePath=GetRecipeFileName("Tester.Data");
}
//---------------------------------------------------------------------------
void TfSCKART::AccessFile(bool bRead, int iAccess)                              // -1: all, 0: lot status, 1: count, 10: setting
{
    AnsiString Str, s1;
    bool bRefreshStartMode=bCanRunSCKART;

    if(CosFunction.bUseSCKART==false)                                           //Steven 20161201 : For SCK 93K ART
        return;

    SetSetupFilePath();

    if(iAccess==-1 || iAccess==0 || iAccess==10)
    {
        sLOTSTATUS      =ReadWriteIni(sSetupFilePath, "AutoRetest", "LOTSTATUS",       sLOTSTATUS,     AnsiString("NONE")  , bRead);
        iCurrentStatus  =ReadWriteIni(sSetupFilePath, "AutoRetest", "iCurrentStatus",  iCurrentStatus, iLOTSTATUS_W        , bRead);
        iTesterType     =ReadWriteIni(sSetupFilePath, "AutoRetest", "iTesterType",     iTesterType,    0                   , bRead);
        if(iTesterType==1)                                                      //Steven 20161201 (wei) : For SCK 93K ART
            CosFunction.bAutoRetestGPIBmode         =true;
        else
            CosFunction.bAutoRetestGPIBmode         =false;
    }

    if(iAccess==-1 || iAccess==1)
    {
        sLotID          =ReadWriteIni(sSetupFilePath, "AutoRetest", "sLotID",          sLotID,         AnsiString("")      , bRead);
        sProcessCode    =ReadWriteIni(sSetupFilePath, "AutoRetest", "sProceddCode",    sProcessCode,   AnsiString("")      , bRead); //Steven 20190521 : ATK lot count
        iLotCount       =ReadWriteIni(sSetupFilePath, "AutoRetest", "iLotCount",       iLotCount,      0                   , bRead);
        iInputCount     =ReadWriteIni(sSetupFilePath, "AutoRetest", "iInputCount",     iInputCount,    0                   , bRead);
        iFTRTCount      =ReadWriteIni(sSetupFilePath, "AutoRetest", "iFTRTCount",      iFTRTCount,     0                   , bRead);
        iInputJamCnt    =ReadWriteIni(sSetupFilePath, "AutoRetest", "iInputJamCnt",    iInputJamCnt,   0                   , bRead);
        iOutputJamCnt   =ReadWriteIni(sSetupFilePath, "AutoRetest", "iOutputJamCnt",   iOutputJamCnt,  0                   , bRead);
        iManualRejectCnt=ReadWriteIni(sSetupFilePath, "AutoRetest", "iManualRejectCnt",iManualRejectCnt, 0                 , bRead);  //JerryYang 20220923 : SPIL手動輸入reject數量(要重測的)

        sInfo_Customer          =ReadWriteIni(sSetupFilePath, "AutoRetest", "sInfoCustomer",     sInfo_Customer,         AnsiString("")      , bRead);  //JerryYang 20220923 : add Lot Info for SPIL
        sInfo_CustLotID         =ReadWriteIni(sSetupFilePath, "AutoRetest", "sInfoCustLotID",    sInfo_CustLotID,        AnsiString("")      , bRead);
        sInfo_CustDevGup        =ReadWriteIni(sSetupFilePath, "AutoRetest", "sInfoCustDevGup",   sInfo_CustDevGup,       AnsiString("")      , bRead);
        sInfo_DeviceName        =ReadWriteIni(sSetupFilePath, "AutoRetest", "sInfoDeviceName",   sInfo_DeviceName,       AnsiString("")      , bRead);
        sInfo_Stage             =ReadWriteIni(sSetupFilePath, "AutoRetest", "sInfoStage",        sInfo_Stage,            AnsiString("")      , bRead);
        sInfo_Step              =ReadWriteIni(sSetupFilePath, "AutoRetest", "sInfoStep",         sInfo_Step,             AnsiString("")      , bRead);
        sInfo_ReportCnt         =ReadWriteIni(sSetupFilePath, "AutoRetest", "sInfoReportCnt",    sInfo_ReportCnt,        AnsiString("")      , bRead);
        sInfo_ProgramName       =ReadWriteIni(sSetupFilePath, "AutoRetest", "sInfoProgramName",  sInfo_ProgramName,      AnsiString("")      , bRead);
        sInfo_TestBinNo         =ReadWriteIni(sSetupFilePath, "AutoRetest", "sInfoTestBinNo",    sInfo_TestBinNo,        AnsiString("")      , bRead);
        sInfo_TesterID          =ReadWriteIni(sSetupFilePath, "AutoRetest", "sInfoTesterID",     sInfo_TesterID,         AnsiString("")      , bRead);
        sInfo_HandlerID         =ReadWriteIni(sSetupFilePath, "AutoRetest", "sInfoHandlerID",    sInfo_HandlerID,        AnsiString("")      , bRead);
        sInfo_Temperauture      =ReadWriteIni(sSetupFilePath, "AutoRetest", "sInfoTemperauture", sInfo_Temperauture,     AnsiString("")      , bRead);
        sInfo_CurrQty           =ReadWriteIni(sSetupFilePath, "AutoRetest", "sInfoCurrQty",      sInfo_CurrQty,          AnsiString("")      , bRead);
        sInfo_OperatorID        =ReadWriteIni(sSetupFilePath, "AutoRetest", "sInfoOperatorID",   sInfo_OperatorID,       AnsiString("")      , bRead);
        sInfo_BinSet            =ReadWriteIni(sSetupFilePath, "AutoRetest", "sInfo_BinSet",      sInfo_BinSet,           AnsiString("")      , bRead);        //Jerryyang 20250529 : add

        iInfo_MultiLotCnt       =ReadWriteIni(sSetupFilePath, "AutoRetest", "iInfo_MultiLotCnt", iInfo_MultiLotCnt,                   0      , bRead);

        sBundleList             =ReadWriteIni(sSetupFilePath, "AutoRetest", "sBundleList",       sBundleList,            AnsiString("")      , bRead);
        iBundleInCnt            =ReadWriteIni(sSetupFilePath, "AutoRetest", "iBundleInCnt",      iBundleInCnt,                       12      , bRead);
        iBundleOutCnt           =ReadWriteIni(sSetupFilePath, "AutoRetest", "iBundleOutCnt",     iBundleOutCnt,                      12      , bRead);

        for(int i=0; i<5; i++)
        {
            s1=IntToStr(i);
            sInfoArr_InnerLotID[i]        =ReadWriteIni(sSetupFilePath, "AutoRetest", "sInfoArr_InnerLotID_"+s1,   sInfoArr_InnerLotID[i],       AnsiString("")      , bRead);
            sInfoArr_Customer[i]          =ReadWriteIni(sSetupFilePath, "AutoRetest", "sInfoArr_Customer_"+s1,     sInfoArr_Customer[i],         AnsiString("")      , bRead);
            sInfoArr_CustLotID[i]         =ReadWriteIni(sSetupFilePath, "AutoRetest", "sInfoArr_CustLotID_"+s1,    sInfoArr_CustLotID[i],        AnsiString("")      , bRead);
            sInfoArr_CustDevGup[i]        =ReadWriteIni(sSetupFilePath, "AutoRetest", "sInfoArr_CustDevGup_"+s1,   sInfoArr_CustDevGup[i],       AnsiString("")      , bRead);
            sInfoArr_DeviceName[i]        =ReadWriteIni(sSetupFilePath, "AutoRetest", "sInfoArr_DeviceName_"+s1,   sInfoArr_DeviceName[i],       AnsiString("")      , bRead);
            sInfoArr_Stage[i]             =ReadWriteIni(sSetupFilePath, "AutoRetest", "sInfoArr_Stage_"+s1,        sInfoArr_Stage[i],            AnsiString("")      , bRead);
            sInfoArr_Step[i]              =ReadWriteIni(sSetupFilePath, "AutoRetest", "sInfoArr_Step_"+s1,         sInfoArr_Step[i],             AnsiString("")      , bRead);
            sInfoArr_ReportCnt[i]         =ReadWriteIni(sSetupFilePath, "AutoRetest", "sInfoArr_ReportCnt_"+s1,    sInfoArr_ReportCnt[i],        AnsiString("")      , bRead);
            sInfoArr_ProgramName[i]       =ReadWriteIni(sSetupFilePath, "AutoRetest", "sInfoArr_ProgramName_"+s1,  sInfoArr_ProgramName[i],      AnsiString("")      , bRead);
            sInfoArr_TestBinNo[i]         =ReadWriteIni(sSetupFilePath, "AutoRetest", "sInfoArr_TestBinNo_"+s1,    sInfoArr_TestBinNo[i],        AnsiString("")      , bRead);
            sInfoArr_TesterID[i]          =ReadWriteIni(sSetupFilePath, "AutoRetest", "sInfoArr_TesterID_"+s1,     sInfoArr_TesterID[i],         AnsiString("")      , bRead);
            sInfoArr_HandlerID[i]         =ReadWriteIni(sSetupFilePath, "AutoRetest", "sInfoArr_HandlerID_"+s1,    sInfoArr_HandlerID[i],        AnsiString("")      , bRead);
            sInfoArr_Temperauture[i]      =ReadWriteIni(sSetupFilePath, "AutoRetest", "sInfoArr_Temperauture_"+s1, sInfoArr_Temperauture[i],     AnsiString("")      , bRead);
            sInfoArr_CurrQty[i]           =ReadWriteIni(sSetupFilePath, "AutoRetest", "sInfoArr_CurrQty_"+s1,      sInfoArr_CurrQty[i],          AnsiString("")      , bRead);
            sInfoArr_OperatorID[i]        =ReadWriteIni(sSetupFilePath, "AutoRetest", "sInfoArr_OperatorID_"+s1,   sInfoArr_OperatorID[i],       AnsiString("")      , bRead);
            sInfoArr_BinSet[i]            =ReadWriteIni(sSetupFilePath, "AutoRetest", "sInfoArr_BinSet_"+s1,       sInfoArr_BinSet[i],           AnsiString("")      , bRead);      //Jerryyang 20250529 : add
        }

        sLotStartTime           =ReadWriteIni(sSetupFilePath, "AutoRetest", "sLotStartTime",     sLotStartTime,          AnsiString("")       , bRead);
        iManualStart            =ReadWriteIni(sSetupFilePath, "AutoRetest", "iManualStart",      iManualStart,           1                    , bRead);  //RogerYang 20250918 : 瑞薩FT-CT
        bFirstFullSkip          =ReadWriteIni(sSetupFilePath, "AutoRetest", "bFirstFullSkip",    bFirstFullSkip,         false                , bRead);  //RogerYang 20251112 : ART數量到，如果inarm上有IC直接丟到shuttle
    }

    if(iAccess==-1 || iAccess==10 || iAccess==20)
    {
        if(CUSTOMER_CODE==CC_AMKOR_Japan)                                       //RogerYang 20251108 : 瑞薩FT-CT 設定需要by機台
        {
            if(bRead==true)
            {
                TestIF_File.bSCKART_EnableART           =IniConfig.bA10_AutoReTest;
                TestIF_File.bRENESAS_EnableFTCT         =IniConfig.bA10_7_Renesas_FTCT;

                if(TestIF_File.bSCKART_EnableART==false && TestIF_File.bSCKART_EnableART==true)  //讀取的時候檢查 錯誤的資料要寫回
                {
                    TestIF_File.bRENESAS_EnableFTCT=false;
                    IniConfig.bA10_7_Renesas_FTCT=false;
                    TestIF_File.bSCKART_RunARTWithoutCmd=true;

                    AnsiString sPath=AuthPath+"config.ini";
                    WriteIniData(sPath, "Function", "bA10_7_Renesas_FTCT",      IniConfig.bA10_7_Renesas_FTCT);
                }
            }
            TestIF_File.bSCKART_RunARTWithoutCmd=!TestIF_File.bRENESAS_EnableFTCT;   //沒開FTCT要打啟用
        }
        else
        {
            TestIF_File.bSCKART_EnableART   =ReadWriteIni(sSetupFilePath, "AutoRetest", "Enable ART",                   TestIF_File.bSCKART_EnableART,          IniConfig.bA10_AutoReTest,  bRead);     //Steven 20170919 (wei) : Add for SCK ART
            TestIF_File.bSCKART_RunARTWithoutCmd    =ReadWriteIni(sSetupFilePath, "AutoRetest", "Run ART Without Cmd",  TestIF_File.bSCKART_RunARTWithoutCmd,   false,                      bRead);                         //Steven 20180830 : Run ART without tester command
        }

        if(TestIF_File.bSCKART_RunARTWithoutCmd)
        {
            TestIF_File.iSCKART_RTStartMode=1;
        }
        TestIF_File.dSCKART_Yield               =ReadWriteIni(sSetupFilePath, "AutoRetest", "Pass Rate",            TestIF_File.dSCKART_Yield,              100.0,  bRead, true, 0.01,  100.0);     //Steven 20170327 (wei) : 預設值改為100
        TestIF_File.iSCKART_TryCnt              =ReadWriteIni(sSetupFilePath, "AutoRetest", "Try Count",            int(TestIF_File.iSCKART_TryCnt),        3,      bRead, true, 1,     20);
        TestIF_File.bSCKART_AutoSkip            =ReadWriteIni(sSetupFilePath, "AutoRetest", "Auto Skip",            TestIF_File.bSCKART_AutoSkip,           true,   bRead);                         //Steven 20170315 (wei) : For SCK ART Auto Skip
        TestIF_File.iSCKART_AutoSkipCount       =ReadWriteIni(sSetupFilePath, "AutoRetest", "Auto Skip Count",      TestIF_File.iSCKART_AutoSkipCount,      5,      bRead, true, 1,     1000);      //Steven 20170315 (wei) : For SCK ART Auto Skip
        TestIF_File.iSCKART_MinAutoCloseSite    =ReadWriteIni(sSetupFilePath, "AutoRetest", "Min Auto Close Site",  TestIF_File.iSCKART_MinAutoCloseSite,   2,      bRead, true, 1,     32);        //JerryYang 20220923 : 最小關site數量
        TestIF_File.iSCKART_SortMode            =ReadWriteIni(sSetupFilePath, "AutoRetest", "Sort Mode",            TestIF_File.iSCKART_SortMode,           0,      bRead, true, 1,     0);
        TestIF_File.iSCKART_RTStartMode         =ReadWriteIni(sSetupFilePath, "AutoRetest", "RT Start Mode",        TestIF_File.iSCKART_RTStartMode,        0,      bRead, true, 1,     0);
        TestIF_File.bSCKART_AutoCleanOut        =ReadWriteIni(sSetupFilePath, "AutoRetest", "Auto Clean Out",       TestIF_File.bSCKART_AutoCleanOut,       true,   bRead);
        TestIF_File.bSCKART_AutoSocketOff       =ReadWriteIni(sSetupFilePath, "AutoRetest", "Auto Socket Off",      TestIF_File.bSCKART_AutoSocketOff,      false,  bRead);
        TestIF_File.iSCKART_AutoSocketOffMode   =ReadWriteIni(sSetupFilePath, "AutoRetest", "Auto Socket Off Mode", TestIF_File.iSCKART_AutoSocketOffMode,  0,      bRead, true,  0, 2);
        TestIF_File.dSCKART_AutoSocketOffYield  =ReadWriteIni(sSetupFilePath, "AutoRetest", "Auto Socket Off Yield",TestIF_File.dSCKART_AutoSocketOffYield, 10.0,   bRead, true, 0.0, 100.0);       //Steven 20170327 (wei) : 預設值改為10.0
        TestIF_File.bSCKART_AutoLotEnd          =ReadWriteIni(sSetupFilePath, "AutoRetest", "Auto Lot End",         TestIF_File.bSCKART_AutoLotEnd,         true,   bRead);
        TestIF_File.bSCKART_FinalLotEndMsg      =ReadWriteIni(sSetupFilePath, "AutoRetest", "Final Lot End Msg",    TestIF_File.bSCKART_FinalLotEndMsg,     false,  bRead);
        TestIF_File.iSCKLdCntLimN               =ReadWriteIni(sSetupFilePath, "AutoRetest", "iSCKLdCntLimN",        TestIF_File.iSCKLdCntLimN,              0,      bRead);  //RogerYang 20251224 : Rf360需求 lotcheck加入上下限設定
        TestIF_File.iSCKLdCntLimP               =ReadWriteIni(sSetupFilePath, "AutoRetest", "iSCKLdCntLimP",        TestIF_File.iSCKLdCntLimP,              0,      bRead);  //RogerYang 20251224 : Rf360需求 lotcheck加入上下限設定

        if(CUSTOMER_CODE==CC_RF360 ||
           CUSTOMER_CODE==CC_QUALCOMM)                                          //Steven 20251104 : for QTI
            TestIF_File.bSCKART_LotDeviceCheck  =ReadWriteIni(sSetupFilePath, "AutoRetest", "Lot Device Check",     TestIF_File.bSCKART_LotDeviceCheck,     false,  bRead);
        else if(LastSet.bBreakSCKART)                                           //Sam 20240402 : 修正 BreakSCKART 問題。
            TestIF_File.bSCKART_LotDeviceCheck  =false;
        else
            TestIF_File.bSCKART_LotDeviceCheck  =true;

        TestIF_File.bSCKART_EnableSPBinAlarm    =ReadWriteIni(sSetupFilePath, "AutoRetest", "Enable Special Bin Alarm", TestIF_File.bSCKART_EnableSPBinAlarm,   false,  bRead);                  //Isaac 20171113 (Steven) : add ATK Special Bin Yield alarm
        TestIF_File.iSCKART_SPBinSelect         =ReadWriteIni(sSetupFilePath, "AutoRetest", "Special Bin Select",       TestIF_File.iSCKART_SPBinSelect,        0,      bRead, true, 1,     255);
        TestIF_File.dSCKART_SPBinAlarmYield     =ReadWriteIni(sSetupFilePath, "AutoRetest", "Special Bin Alarm Yield",  TestIF_File.dSCKART_SPBinAlarmYield,    0.00,   bRead, true, 0.00,  100.00);
        TestIF_File.bLowYieldForArtFT           =ReadWriteIni(sSetupFilePath, "AutoRetest", "Enable Low Yield Limit",   TestIF_File.bLowYieldForArtFT,          false,  bRead);     //Steven 20220208 : Add for ATK, ART low Yield
        TestIF_File.dLowYieldForArtFT           =ReadWriteIni(sSetupFilePath, "AutoRetest", "Yield of Low Yield Limit", TestIF_File.dLowYieldForArtFT,          0.00,   bRead, true, 0.00,  100.00);
        TestIF_File.bAlarmAfterSendSRQKIND2     =ReadWriteIni(sSetupFilePath, "AutoRetest", "bAlarmAfterSendSRQKIND2",  TestIF_File.bAlarmAfterSendSRQKIND2,    false,  bRead); ;    //Steven 20230119 : ATK 要在SRQKIND2之後Alarm

//        SetGPIBVersion(edtVersion->Text);                                     //Steven 20230811 : 針對GPIB發送Version命令, 統一處理         //Jimmychiu 20231205 : Mark SetGPIBVersion function
    }

    palInputQty->Caption        =iLotCount;
    palLotNumber->Caption       =sLotID;
    pnlProcessCode->Caption     =sProcessCode;
    if(CUSTOMER_CODE==CC_TERAPOWER)                                             //Sam 20200203 : 晶兆成英傑要求改顯示 Host Name
    {
        char HostName[80];                                                      //存放本機名稱
        ZeroMemory(HostName , sizeof(HostName));
        gethostname(HostName, sizeof(HostName));                                //取得本機名稱
        palTesterType->Caption =HostName;
    }
    else
    {
        if(fMain->hanaART->IsHanaArtAvailable())                                //Steven 20250414 : HANA ART Function
        {
            if(IniConfig.iA10_6_HANA_ART_TestMode==0)
                palTesterType->Caption  =AnsiString("General");
            else
                palTesterType->Caption  =AnsiString("SMILL");
        }
        else
        {
            palTesterType->Caption  =(iTesterType==1)?AnsiString("93K"):AnsiString("Flex");
        }
    }
    palTestCnt->Caption         =iFTRTCount;
    palLotStatus->Caption       =sLOTSTATUS;
    Str.sprintf(">%0.2f%%", TestIF_File.dSCKART_Yield);
    palARTPassRate->Caption     =Str;
    edLotCount->Text            =AnsiString(iLotCount);
    edARTPassRate->Text         =AnsiString(TestIF_File.dSCKART_Yield);
    edlRTTryCnt->Text           =AnsiString(TestIF_File.iSCKART_TryCnt);
    chkNoTesterCmd->Checked     =TestIF_File.bSCKART_RunARTWithoutCmd;          //Steven 20180830 : Run ART without tester command
    cbAutoSkip->Checked         =TestIF_File.bSCKART_AutoSkip;                  //Steven 20170315 (wei) : For SCK ART Auto Skip
    edARTAutoSkip->Text         =AnsiString(TestIF_File.iSCKART_AutoSkipCount); //Steven 20170315 (wei) : For SCK ART Auto Skip
    edtAlmAutoCloseSite->Text   =AnsiString(TestIF_File.iSCKART_MinAutoCloseSite); //JerryYang 20220923 : 最小關site數量
    chkAlarmKIND2->Checked      =TestIF_File.bAlarmAfterSendSRQKIND2;           //Steven 20230119 : ATK 要在SRQKIND2之後Alarm
    cbAutoSocketOff->Checked    =TestIF_File.bSCKART_AutoSocketOff;
    cbRTOffMode->ItemIndex      =TestIF_File.iSCKART_AutoSocketOffMode;
    edBestYield->Text           =TestIF_File.dSCKART_AutoSocketOffYield;
    cbSortMode->ItemIndex       =TestIF_File.iSCKART_SortMode;
    cbRTStartMode->ItemIndex    =TestIF_File.iSCKART_RTStartMode;
    cbAutoCleanOut->Checked     =TestIF_File.bSCKART_AutoCleanOut;
    cbAutoLotEnd->Checked       =TestIF_File.bSCKART_AutoLotEnd;
    cbFinalLotEndMsg->Checked   =TestIF_File.bSCKART_FinalLotEndMsg;
    cbLotDeviceCheck->Checked   =TestIF_File.bSCKART_LotDeviceCheck;
    edtLdCntLimN->Text          =TestIF_File.iSCKLdCntLimN;                     //RogerYang 20251224 : Rf360需求 lotcheck加入上下限設定
    edtLdCntLimP->Text          =TestIF_File.iSCKLdCntLimP;                     //RogerYang 20251224 : Rf360需求 lotcheck加入上下限設定

    edtInputCount->Text         =AnsiString(iInputCount);
    edtMRejectCnt->Text         =AnsiString(iManualRejectCnt);                  //JerryYang 20220923 : SPIL手動輸入reject數量(要重測的)

    chkEnableART->Checked       =TestIF_File.bSCKART_EnableART;                 //Steven 20170919 (wei) : Add for SCK ART
//    chkARTTestMode->Checked     =TestIF_File.bSCKART_LockTestMode;
//    cbbARTTestMode->ItemIndex   =TestIF_File.iSCKART_TestMode;
    if(CosFunction.bAmkorFunction || CUSTOMER_CODE==CC_QUALCOMM)
    {
//        chkVersion->Checked     =TestIF_File.bSCKART_OverWriteVersion;
        edtVersion->Text        =TestIF_File.sSCKART_VersionCmd;
    }
    cbSPBinAlarm->Checked       =TestIF_File.bSCKART_EnableSPBinAlarm;          //Isaac 20171113 (Steven) : add ATK Special Bin Yield alarm
    cbSPBinSelect->ItemIndex    =TestIF_File.iSCKART_SPBinSelect;
    edSPBinYield->Text          =TestIF_File.dSCKART_SPBinAlarmYield;
    chkLowYieldForFT->Checked   =TestIF_File.bLowYieldForArtFT;                 //Steven 20220208 : Add for ATK, ART low Yield
    edtLowYieldForFT->Text      =TestIF_File.dLowYieldForArtFT;
    chkEnableFTCT->Checked      =TestIF_File.bRENESAS_EnableFTCT;               //RogerYang 20250916 : 瑞薩FT-CT

//    if(CosFunction.bUseSCKART==false)                                         //Steven 20170919 (wei) : Add for SCK ART
//        IniConfig.bA10TestModeForART=false;

    if(USE_AUTO_RETEST==eartInstall       &&
       CosFunction.bUseSCKART==true       &&
       IniConfig.bA10_AutoReTest==true    &&
       IniConfig.bA10TestModeForART==true)
    {
        if(TestIF_File.iGpibMode!=IniConfig.iA10TestModeForART)
        {
            TestIF_File.iGpibMode=IniConfig.iA10TestModeForART;
        }
    }

    if(USE_AUTO_RETEST==eartInstall       &&
       CosFunction.bUseSCKART==true       &&
       IniConfig.bA10_AutoReTest==true    &&
       TestIF_File.bSCKART_EnableART)
    {
        bCanRunSCKART=true;
    }
    else
    {
        bCanRunSCKART=false;
    }

    if(bRefreshStartMode!=bCanRunSCKART)                                        //Steven 20170209 : 修正開關ART時候,要重新整理Start Mode
    {
        fMain->SetStartModeData();
    }

    FTestIF->ReadTestIFFile();
    fMain->LoadTestModePicture();

    if(bRead==false)
        WriteLastDataFile();
    CustomerFunctionSelect();
    UpdateCount();

    if(bRead==false)
    {
        fMain->BackupSetupFile();                                               //Ifor 20170620 (wei) add Auto BackUp Setup File & Last Data
    }
}
//---------------------------------------------------------------------------
void TfSCKART::SetGPIBVersion(AnsiString Str)
{
    if(CosFunction.bAmkorFunction || CUSTOMER_CODE==CC_QUALCOMM)                //Richard 20230428 : Enable sSCKART_VersionCmd
    {
        if(IniConfig.bA10TestModeForART==false)
        {
            WriteIniData("D:\\GPIB9045\\system\\general.ini", "SystemSetup", "Version",  Str);
        }
        else
        {
            if(TestIF_File.iGpibMode==InterfaceType_16BinGS || TestIF_File.iGpibMode==InterfaceType_32BinGS)
            {
                WriteIniData(AnsiString("D:\\GPIB9045\\system\\general.ini"), "SystemSetup", "Version", "ADVANTEST M4871 Rev.1.1P3 FULLSITEB");
            }
            else
            {
                WriteIniData(AnsiString("D:\\GPIB9045\\system\\general.ini"), "SystemSetup", "Version", "IFUNT200 Version B17");
            }
        }
    }

    if(TestIF_File.iGpibMode==InterfaceType_16BinGS || TestIF_File.iGpibMode==InterfaceType_32BinGS)
    {
        TestIF_File.sSCKART_VersionCmd=ReadWriteIni(AnsiString("D:\\GPIB9045\\system\\general.ini"), AnsiString("SystemSetup"), AnsiString("Version"), TestIF_File.sSCKART_VersionCmd, AnsiString("ADVANTEST M4871 Rev.1.1P3 FULLSITEB"), true);
    }
    else
    {
        TestIF_File.sSCKART_VersionCmd=ReadWriteIni(AnsiString("D:\\GPIB9045\\system\\general.ini"), AnsiString("SystemSetup"), AnsiString("Version"), TestIF_File.sSCKART_VersionCmd, AnsiString("IFUNT200 Version B17"), true);
    }
}
//---------------------------------------------------------------------------
void TfSCKART::UpdateCount()
{
    int iUnloadCnt=0;
    int iTotalUnloadCount=0;
    AnsiString Str;

    for(int i=0; i<10; i++)
    {
        iTotalUnloadCount+=LastSet.lSCKARTBinCT[i];
    }

    palInputCount->Caption  =iInputCount;
    palInputCnt->Caption    =LastSet.iSCKARTInputCT;                            //Steven 20170327 (wei) : Fixed for ART count
    palInShtCnt->Caption    =LastSet.lShuttleCount;
    if(LastSet.iTester==OFF_LINE)
    {
        if(BinSelect[OffT].bAutoRetest[0])
            iUnloadCnt+=LastSet.lSCKARTBinCT[0];                                //Steven 20170113 : For count unload count
        if(BinSelect[OffT].bAutoRetest[1])
            iUnloadCnt+=LastSet.lSCKARTBinCT[1];
        if(BinSelect[OffT].bAutoRetest[2])
            iUnloadCnt+=LastSet.lSCKARTBinCT[2];
        palAutoTrayCnt1->Caption=(BinSelect[OffT].bAutoRetest[0])?AnsiString(LastSet.lSCKARTBinCT[0]):AnsiString("---");
        palAutoTrayCnt2->Caption=(BinSelect[OffT].bAutoRetest[1])?AnsiString(LastSet.lSCKARTBinCT[1]):AnsiString("---");
        palAutoTrayCnt3->Caption=(BinSelect[OffT].bAutoRetest[2])?AnsiString(LastSet.lSCKARTBinCT[2]):AnsiString("---");
    }
    else
    {
        if(BinSelect[FT].bAutoRetest[0])
            iUnloadCnt+=LastSet.lSCKARTBinCT[0];                                //Steven 20170113 : For count unload count
        if(BinSelect[FT].bAutoRetest[1])
            iUnloadCnt+=LastSet.lSCKARTBinCT[1];
        if(BinSelect[FT].bAutoRetest[2])
            iUnloadCnt+=LastSet.lSCKARTBinCT[2];
        palAutoTrayCnt1->Caption=(BinSelect[FT].bAutoRetest[0])?AnsiString(LastSet.lSCKARTBinCT[0]):AnsiString("---");
        palAutoTrayCnt2->Caption=(BinSelect[FT].bAutoRetest[1])?AnsiString(LastSet.lSCKARTBinCT[1]):AnsiString("---");
        palAutoTrayCnt3->Caption=(BinSelect[FT].bAutoRetest[2])?AnsiString(LastSet.lSCKARTBinCT[2]):AnsiString("---");
    }

    palOutputCnt->Caption       =iTotalUnloadCount;
    palInShtCntGap->Caption     =iInputCount-LastSet.lShuttleCount;
    palOutputGap->Caption       =iInputCount-iTotalUnloadCount;
    palRejectCnt->Caption       =iUnloadCnt;
    palInputJamCount->Caption   =iInputJamCnt;
    palOutputJamCount->Caption  =iOutputJamCnt;
    palManualRejectCnt->Caption =iManualRejectCnt;                              //JerryYang 20220923 : SPIL手動輸入reject數量(要重測的)
    palRTTryCnt->Caption        =TestIF_File.iSCKART_TryCnt;

    if(TestIF_File.bRENESAS_EnableFTCT==true)                                   //RogerYang 20250923 : 瑞薩FT-CT
    {
        LastSet.iHdPass             =iTotalUnloadCount-iUnloadCnt;
        LastSet.iHdFail             =iUnloadCnt;
        //LastSet.iHdRejectBT         =iInputCount-LastSet.lShuttleCount;
        LastSet.iHdRejectBT         =iInputJamCnt;
        LastSet.iHdPickUp           =LastSet.iSCKARTInputCT;
    }

    if(TestIF_File.bSCKART_LotDeviceCheck==false)                               //Steven 20251124 : for disable LotDeviceCheck
    {
        if(iInputCount!=0 &&
           LastSet.iSCKARTInputCT!=0 &&                                         //Steven 20250307 : 要有出料才能算
           iUnloadCnt==0)                                                       //Steven 20241002 : 修正ART Yield 100% 誤報alarm
        {
            dCurrYield=100.0;
        }
        else if(iInputCount!=0 &&
                iUnloadCnt!=0  &&                                               //Steven 20241101 : 要有出料才能算
                LastSet.iSCKARTInputCT-iUnloadCnt==0)                           //Steven 20241002 : 修正ART Yield 100% 誤報alarm
        {
            dCurrYield=0.0;
        }
        else
        {
            if(LastSet.iSCKARTInputCT==0)                                       //Steven 20170327 (wei) : Fixed for ART count
            {
                dCurrYield=0.0;
            }
            else
            {
                dCurrYield=double(LastSet.iSCKARTInputCT-iUnloadCnt)/double(LastSet.iSCKARTInputCT)*100.0;
            }
        }
    }
    else
    {
        if(iInputCount!=0 &&
           LastSet.iSCKARTInputCT!=0 &&                                         //Steven 20250307 : 要有出料才能算
           iUnloadCnt==0)                                                       //Steven 20241002 : 修正ART Yield 100% 誤報alarm
        {
            dCurrYield=100.0;
        }
        else if(iInputCount!=0 &&
                iUnloadCnt!=0  &&                                               //Steven 20241101 : 要有出料才能算
                iLotCount-iUnloadCnt==0)                                        //Steven 20241002 : 修正ART Yield 100% 誤報alarm
        {
            dCurrYield=0.0;
        }
        else
        {
            if(iLotCount==0)                                                    //Steven 20170327 (wei) : Fixed for ART count
            {
                dCurrYield=0.0;
            }
            else
            {
                dCurrYield=double(iLotCount-iUnloadCnt)/double(iLotCount)*100.0;
            }
        }
    }
    Str.sprintf("%0.2f%%", dCurrYield);

    palCurrentRate->Caption     =Str;
    palLotStatus->Visible       =(iTesterType==0);
    lblHandlerStatus->Visible   =(iTesterType==0);
}
//---------------------------------------------------------------------------
void TfSCKART::AddAlarmCode(AnsiString Code, AnsiString Message, int iDuplicate)//Steven 20161025 : SCK ART function
{
    if(CUSTOMER_CODE==CC_SCK && iTesterType!=0)
    {
    }
    else
    {
        if(memoAlarmList->Lines->Count==0)
        {
            memoAlarmList->Lines->Add("ALARM:"+Code);
        }
        else
        {
            memoAlarmList->Lines->Add(Code);
        }

        try
        {
            memoAlarmList->Lines->SaveToFile(sAlarmPath);
        }
        catch(...)
        {
            MyDBIProcess("Exception", "TfSCKART->AddAlarmCode");
        }
    }

    if(iDuplicate==0 &&
       Code.AnsiPos("JAM")!=0)
    {
        RunInfo.AddAlarm(Code, Message);
    }
}
//---------------------------------------------------------------------------
void TfSCKART::ClearAlarmCode()
{
    memoAlarmList->Clear();
    try
    {
        memoAlarmList->Lines->SaveToFile(sAlarmPath);
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfSCKART->ClearAlarmCode");
    }
}
//---------------------------------------------------------------------------
void TfSCKART::SetLotStatus(int iStatus)
{
    switch(iStatus)
    {
        case 1:                                                                 //iLOTSTATUS_W
            sLOTSTATUS="LOTSTATUS_W";
            break;
        case 2:                                                                 //iLOTSTATUS_T
            sLOTSTATUS="LOTSTATUS_T";
            break;
        case 3:                                                                 //iLOTSTATUS_L
            sLOTSTATUS="LOTSTATUS_L";
            break;
        case 4:                                                                 //iLOTSTATUS_R
            sLOTSTATUS="LOTSTATUS_R";
            break;
        case 5:                                                                 //iLOTSTATUS_F
            sLOTSTATUS="LOTSTATUS_F";
            break;
        case 6:                                                                 //iLOTSTATUS_A
            sLOTSTATUS="LOTSTATUS_A";
            break;
        default:
            sLOTSTATUS="NONE";
    }
    cbLotStatus->ItemIndex=iStatus;
    iCurrentStatus=iStatus;
//    SafeFile();
}
//---------------------------------------------------------------------------
AnsiString TfSCKART::GetLotStatus()
{
    return sLOTSTATUS;
}
//---------------------------------------------------------------------------
void TfSCKART::SettingPanelOnOff(bool bOn)
{
    if(bOn)
        AccessFile(true, 20);
    bShowSetting                =(bOn && AccessLevel>=LevelSet.AccessLevel[129]);
    palARTSetting->Visible      =(bShowSetting && AccessLevel>=LevelSet.AccessLevel[130]);
    gbUserFix->Visible          =bShowSetting;
    cbLotStatus->Visible        =(bShowSetting && AccessLevel>=LevelSet.AccessLevel[130] && iTesterType==0);
    edlRTTryCnt->Visible        =(bShowSetting && AccessLevel>=LevelSet.AccessLevel[130]);
    edARTPassRate->Visible      =(bShowSetting && AccessLevel>=LevelSet.AccessLevel[130]);
    btnApplyLotInfo->Visible    =(bShowSetting && AccessLevel>=LevelSet.AccessLevel[130]);
    edLotCount->Visible         =(bShowSetting && AccessLevel>=LevelSet.AccessLevel[130]);
    edtInputCount->Visible      =(bShowSetting && AccessLevel>=LevelSet.AccessLevel[130]);
    btnApplyQty->Visible        =(bShowSetting && AccessLevel>=LevelSet.AccessLevel[130]);
    grpARTSetting->Enabled      =(bShowSetting && AccessLevel>=LevelSet.AccessLevel[130]);
    gdAlarmList->Visible        =(bShowSetting && CUSTOMER_CODE==CC_SCK);
    edtMRejectCnt->Visible      =(bShowSetting && AccessLevel>=LevelSet.AccessLevel[130]);  //JerryYang 20220923 : SPIL手動輸入reject數量(要重測的)

    if(HasICUnderMachine() || HasAnyICInMachine())                              //JerryYang 20230204 : 機台內有IC不得開關ART功能
    {
        chkEnableART->Enabled=false;
    }
    else
    {
        chkEnableART->Enabled=true;
    }

    if(bShowSetting)
        fSCKART->Width=780;
    else
        fSCKART->Width=258;

    if(CosFunction.iAutoRetestTCPmode==2)                                       //RogerYang 20250916 : 瑞薩FT-CT
    {
        ledShowFTCTData->Visible=true;
        chkEnableFTCT->Visible=true;
        btnFTCTReset->Visible=true;                                             //RogerYang 20251114 : add
    }
    else
    {
        ledShowFTCTData->Visible=false;
        chkEnableFTCT->Visible=false;
        btnFTCTReset->Visible=false;                                            //RogerYang 20251205 : add
    }

    //這一行請保持在最下面!!-----------------
//    myLog.Do_Log(this, asUser, asLogPath);                                    //Steven 20170327 (wei) : Add change log in ART
}
//---------------------------------------------------------------------------
void __fastcall TfSCKART::btnApplyCountClick(TObject *Sender)
{
    AnsiString str;
    int i=1;
    if(LastSet.iTester==OFF_LINE)                                               //Steven 20170327 (wei) : Add change log in ART
    {
        if(BinSelect[OffT].bAutoRetest[0])
            i=0;
        else if(BinSelect[OffT].bAutoRetest[2])
            i=2;
        else
            i=1;
    }
    else
    {
        if(BinSelect[FT].bAutoRetest[0])
            i=0;
        else if(BinSelect[FT].bAutoRetest[2])
            i=2;
        else
            i=1;
    }

    str.sprintf("ART before count correction: InputPickCount: %s, Input Jam Count: %s, Input Shuttle Count: %s, Output Loader Count: %s, Output Jam Count: %s, Merged(Auto) Reject Count: %s",
                        palInputCnt->Caption,
                        palInputJamCount->Caption,
                        palInShtCnt->Caption,
                        palOutputCnt->Caption,
                        palOutputJamCount->Caption,
                        palRejectCnt->Caption);
    RecordProcess(str);

    iInputJamCnt=0;
    iOutputJamCnt=0;
    LastSet.iSCKARTInputCT-=atoi(palInputJamCnt->Caption.c_str());              //Steven 20170825 (wei) : Fixed for SCK ART loading count

//    if(CUSTOMER_CODE==CC_SCK)                                                   //Richard 20230427 : 把manual Correct Count內輸入的 "palInputJamCnt"丟入RejectCnt。
//    {
//        int iMissMatchQty=atoi(fSCKART->palInputCount->Caption.c_str())-atoi(fSCKART->palInputCnt->Caption.c_str());
//        LastSet.iSCKARTInputCT+=iMissMatchQty;
//        LastSet.lShuttleCount+=iMissMatchQty;
//        LastSet.iSCKART_RTUnitCount+=iMissMatchQty;
//        LastSet.lSCKARTBinCT[i]+=iMissMatchQty;
//    }

//    LastSet.lSCKARTBinCT[i]+=atoi(palInputJamCnt->Caption.c_str());
    LastSet.lSCKARTBinCT[i]+=atoi(palOutputJamCnt->Caption.c_str());
    LastSet.lSCKARTBinCT[6]+=atoi(palInRemoveCnt->Caption.c_str());
    LastSet.lSCKARTBinCT[6]+=atoi(palOutRemoveCnt->Caption.c_str());
    LastSet.lShuttleCount+=atoi(palInRemoveCnt->Caption.c_str());
    LastSet.iSCKART_RTUnitCount+=atoi(palOutputJamCnt->Caption.c_str());

    str.sprintf("ART count correction: In Jam cnt: %s, In Remove cnt: %s, Out Jam cnt: %s, Out remove cnt: %s",     //Steven 20170327 : Add change log in ART
                        palInputJamCnt->Caption,
                        palInRemoveCnt->Caption,
                        palOutputJamCnt->Caption,
                        palOutRemoveCnt->Caption);
    RecordProcess(str);

    palInputJamCnt->Caption=0;
    palInRemoveCnt->Caption=0;
    palOutputJamCnt->Caption=0;
    palOutRemoveCnt->Caption=0;
    UpdateCount();

    str.sprintf("ART after count correction: InputPickCount: %s, Input Jam Count: %s, Input Shuttle Count: %s, Output Loader Count: %s, Output Jam Count: %s, Merged(Auto) Reject Count: %s",
                        palInputCnt->Caption,
                        palInputJamCount->Caption,
                        palInShtCnt->Caption,
                        palOutputCnt->Caption,
                        palOutputJamCount->Caption,
                        palRejectCnt->Caption);
    RecordProcess(str);

    AccessFile(false, -1);
}
//---------------------------------------------------------------------------
void __fastcall TfSCKART::palInputJamCntClick(TObject *Sender)
{
    TPanel *Ptr=(TPanel *)Sender;
    edTemp->Text=Ptr->Caption;
    fQwertyKey->ShowQwertyKey(edTemp, N_INTEGER);
    Ptr->Caption=edTemp->Text;
}
//---------------------------------------------------------------------------
void __fastcall TfSCKART::btnExitClick(TObject *Sender)
{
    fSCKART->palARTCount->Parent=fSCKART->palARTLeft;
    fSCKART->palARTCount->Align=alTop;
    SettingPanelOnOff(false);
    btnExit->Down=false;
    btnExit1->Down=false;

    if(CosFunction.bUseSCKART && USE_AUTO_RETEST==eartInstall && IniConfig.bA10_AutoReTest && TestIF_File.bSCKART_EnableART)  //Steven 20171128 (Wei) : If ART off mode, ART menu disappeared
        ;//fSCKART->Show();
    else
        fSCKART->Close();
}
//---------------------------------------------------------------------------
void __fastcall TfSCKART::edlRTTryCntClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 0, true, 1, 20);
}
//---------------------------------------------------------------------------
void __fastcall TfSCKART::edARTPassRateClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 0.01, 100.00);
}
//---------------------------------------------------------------------------
void __fastcall TfSCKART::edLotCountClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 100000, 0);
}
//---------------------------------------------------------------------------
void TfSCKART::ClearLotInfo()
{
    iNeedRT         =1;
    iFTRTCount      =0;
    iManualRejectCnt=0;                                                         //JerryYang 20220923 : SPIL手動輸入reject數量(要重測的)

    if(TestIF_File.bRENESAS_EnableFTCT==true)                                   //RogerYang 20250911 : 瑞薩FT-CT 不在這清掉
    {
    }
    else
    {
        iLotCount       =0;
        iInputCount =0;
        sLotID          ="";
        sProcessCode    ="";
        sLotStartTime   ="";
        fSCKART->SetLotStatus(iLOTSTATUS_NONE);
    }
    LastSet.iSCKART_RTUnitCount    =0;                                          //Steven 20170327 (wei) : Fixed for ART count
    iInputJamCnt    =0;
    iOutputJamCnt   =0;

    for(int i=0; i<10; i++)
    {
        LastSet.lSCKARTBinCT[i]=0;
    }
    LastSet.iSCKARTInputCT=0;                                                   //Steven 20170327 (wei) : Fixed for ART count
    LastSet.lShuttleCount=0;
    LastSet.iHdPass=0;                                                          //RogerYang 20250923 : 瑞薩FT-CT
    LastSet.iHdFail=0;                                                          //RogerYang 20250923 : 瑞薩FT-CT
    LastSet.iHdRejectATPass=0;                                                  //RogerYang 20250923 : 瑞薩FT-CT
    LastSet.iHdRejectATFail=0;                                                  //RogerYang 20250923 : 瑞薩FT-CT
    LastSet.iHdRejectBT=0;                                                      //RogerYang 20250923 : 瑞薩FT-CT
    LastSet.iTesterMatch=0;                                                     //RogerYang 20250923 : 瑞薩FT-CT
    LastSet.iTesterUnmatch=0;                                                   //RogerYang 20250923 : 瑞薩FT-CT
    LastSet.iTestIgnore=0;                                                      //RogerYang 20250923 : 瑞薩FT-CT
    LastSet.iHdPickUp=0;                                                        //RogerYang 20250923 : 瑞薩FT-CT

    LotSummary.ClearAllData();

    DoAutoSocketOff(true);

    sInfo_Customer="";                                                          //JerryYang 20220331 : 緯平要求不要Show NA直接存空值
    sInfo_InnerLotID="";
    sInfo_CustLotID="";
    sInfo_CustDevGup="";
    sInfo_DeviceName="";
    sInfo_Stage="";
    sInfo_Step="";
    sInfo_ReportCnt="";
    sInfo_ProgramName="";
    sInfo_TestBinNo="";
    sInfo_TesterID="";
    sInfo_HandlerID="";
    sInfo_Temperauture="";
    sInfo_CurrQty="";
    sInfo_OperatorID="";
    sInfo_BinSet="";                                                            //Jerryyang 20250529 : add
    iInfo_MultiLotCnt=0;
    //sLotStartTime="";

    sBundleList="";

    for(int i=0; i<5; i++)
    {
        sInfoArr_Customer[i]="";                                                //JerryYang 20220331 : 緯平要求不要Show NA直接存空值
        sInfoArr_InnerLotID[i]="";
        sInfoArr_CustLotID[i]="";
        sInfoArr_CustDevGup[i]="";
        sInfoArr_DeviceName[i]="";
        sInfoArr_Stage[i]="";
        sInfoArr_Step[i]="";
        sInfoArr_ReportCnt[i]="";
        sInfoArr_ProgramName[i]="";
        sInfoArr_TestBinNo[i]="";
        sInfoArr_TesterID[i]="";
        sInfoArr_HandlerID[i]="";
        sInfoArr_Temperauture[i]="";
        sInfoArr_CurrQty[i]="";
        sInfoArr_OperatorID[i]="";
        sInfoArr_BinSet[i]="";                                                  //Jerryyang 20250529 : add
        LotSummary.iByLotLoadCount[i]=0;
    }

    fSCKART->AccessFile(false);
    bWaitTSV=false;
}
//---------------------------------------------------------------------------
void __fastcall TfSCKART::edARTAutoSkipClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 1000, 1);
}
//---------------------------------------------------------------------------
int TfSCKART::CheckLoadingCount()                                               //Steven 20161123 : For SCK ART  0:not enough, 1:same, 2:over count
{
    if((CosFunction.bUseSCKART &&
        USE_AUTO_RETEST==eartInstall &&
        IniConfig.bA10_AutoReTest &&
        TestIF_File.bSCKART_EnableART) ||
       IniConfig.bA65_BundleIDList)                                             //Steven 20161201 : For SCK 93K ART
    {
        if(CUSTOMER_CODE==CC_PTI && IniConfig.bB03_TesterReport)                //Sam 20240809 : PTI ART 模式
        {
            if(fSCKART->iFTRTCount==0 || TestIF_File.iSCKART_TryCnt<fSCKART->iFTRTCount)
                return 0;
        }
        #ifdef SOFT_SIMULTE
        if(fMain->hanaART->IsHanaArtAvailable()==true)
        {
            return 0;
        }
        #endif
        int iLoadingCount=LastSet.iSCKARTInputCT;                               //Steven 20170327 (wei) : Fixed for ART count
        if(iLoadingCount<fSCKART->iInputCount)
        {
            bLdCntExdInputCnt=false;                                            //RogerYang 20250923 : 瑞薩FT-CT
            return 0;
        }
        else if(fSCKART->iInputCount==iLoadingCount)                            //到了的時候要一顆一顆檢查下去
        {
            bLdCntExdInputCnt=false;                                            //RogerYang 20250923 : 瑞薩FT-CT
            return 1;
        }
        else                                                                    //need alarm
        {
            if(TestIF_File.bRENESAS_EnableFTCT==true)                           //RogerYang 20250923 : 瑞薩FT-CT
            {
                //若實際數量比設定數量多，做到底全部完畢之後跳出，供給/不供給視窗，顯示數量多xx枚。
                //return fMain->RENESAS_Server->DoNeedSupplyOrNot();
                bLdCntExdInputCnt=true;
                return 1;
            }
            else
            {
                return 2;
            }
        }
    }
    else if(IniConfig.bP57LoaderAutoCleanOutByInputCT &&                        //Sam 20250605 : Loader Count AutoCleanOut
            LastSet.iP57_InputCT!=0)
    {
        int iLoadingCount=LastSet.SendCT[0];
        if(iLoadingCount<LastSet.iP57_InputCT)
        {
            return 0;
        }
        else if(LastSet.iP57_InputCT==iLoadingCount)                            //到了的時候要一顆一顆檢查下去
        {
            return 1;
        }
        else                                                                    //need alarm
        {
            return 2;
        }
    }
    return -1;
}
//---------------------------------------------------------------------------
void TfSCKART::CheckInArmNeedVariModeFIX()                                      //Sam 20211118 : 整合 ART InOut Arm 一顆一顆放
{
    int iDevice=20;                                                             //Sam 20211207 : In/Out 都倒數 20 顆 一顆一顆放

    if(CosFunction.bUseSCKART &&
       USE_AUTO_RETEST==eartInstall &&
       IniConfig.bA10_AutoReTest &&
       TestIF_File.bSCKART_EnableART &&
       TestIF_File.bRENESAS_EnableFTCT==false)                                  //RogerYang 20251111 : 瑞薩FT-CT no need
    {
        int iLoadingCount=LastSet.iSCKARTInputCT;
        if(iLoadingCount<=fSCKART->iInputCount)
        {
            if(TestIF_File.bSCKART_LotDeviceCheck)
            {
                if(iLoadingCount>fSCKART->iInputCount-iDevice)                  //數量接近時，要改用Fix方式生產
                    ArmSpeed[InArm].bVariModeFIX=true;
                else
                    bBackUpInArmMode=false;
            }
        }
    }
    else if(IniConfig.bP57LoaderAutoCleanOutByInputCT &&                        //Sam 20250605 : Loader Count AutoCleanOut
            LastSet.iP57_InputCT!=0)
    {
        int iLoadingCount=LastSet.SendCT[0];
        if(iLoadingCount<=LastSet.iP57_InputCT)
        {
            if(iLoadingCount>LastSet.iP57_InputCT-iDevice)                      //數量接近時，要改用Fix方式生產
                ArmSpeed[InArm].bVariModeFIX=true;
            else
                bBackUpInArmMode=false;
        }
    }
}
//---------------------------------------------------------------------------
void TfSCKART::CheckOutArmNeedVariModeFIX(int iAuto)                            //Sam 20211118 : 整合 ART InOut Arm 一顆一顆放
{
    int iDevice=20;                                                             //Sam 20211207 : Out 都倒數 20 顆 一顆一顆放
    int iTotalUnloadCount=0;
    if(CUSTOMER_CODE==CC_TERAPOWER)
    {
    }
    else
    {
        iDevice=30;
    }

    if(CosFunction.bUseSCKART &&
       USE_AUTO_RETEST==eartInstall &&
       IniConfig.bA10_AutoReTest &&
       TestIF_File.bSCKART_EnableART)
    {
        for(int i=0; i<10; i++)
        {
            iTotalUnloadCount+=LastSet.lSCKARTBinCT[i];
        }

        if(Prod.iIsFailT6[iAuto]==1)                                            //Steven 20240105 : Prod.bIsPass --> Prod.iIsFailT6  //Fail IC 強制一顆一顆放
            ArmSpeed[OutArm].bVariModeFIX=true;
        else
            ArmSpeed[OutArm].bVariModeFIX=false;

        int iLoadingCount=LastSet.iSCKARTInputCT;
        if(fSCKART->iInputCount>=iLoadingCount)
        {
            if(TestIF_File.bSCKART_LotDeviceCheck)
            {
                if(fSCKART->iInputCount-iTotalUnloadCount<=iDevice)             //數量接近時，要改用Fix方式生產
                    ArmSpeed[OutArm].bVariModeFIX=true;
                else
                    bBackUpOutArmMode=false;
            }
        }
    }
}
//---------------------------------------------------------------------------
void TfSCKART::CheckNeedRT()
{
    int iDevice=0;
    int iHanaNeedToRt;
    if(CUSTOMER_CODE==CC_TERAPOWER)                                             //Sam 20211105 : TPW 建勳要求改為300顆
        iDevice=300;
    else
        iDevice=20;

    int iRet=K_RETRY;

    if(fMain->hanaART->IsHanaArtAvailable()==true)                              //Steven 20250414 : HANA ART Function
    {
        iHanaNeedToRt=fMain->hanaART->NeedToRT();                               //0:Waiting command, 1:Need to RT, 2:Lot End
        if(iHanaNeedToRt==0)
        {
            return;
        }
        else
        {
            iFTRTCount++;
            AccessFile(false, -1);
            if(iHanaNeedToRt==2)                                                //Steven 20251219 : fixed for Hana ART
            {
                iNeedRT=0;
            }
            else
            {
                iNeedRT=1;
                if(iFTRTCount>TestIF_File.iSCKART_TryCnt)                       //Steven 20251020 : fixed for Hana ART
                {
                    iNeedRT=0;
                }
                else if(iFTRTCount==TestIF_File.iSCKART_TryCnt)                 //最後一次要執行Final ART
                {
                    iNeedRT=2;
                }
            }
        }
    }
    else if(CosFunction.iAutoRetestTCPmode==2 &&                                //RogerYang 20251008 : 瑞薩FT-CT
            TestIF_File.bRENESAS_EnableFTCT==true)
    {
//        if(fMain->RENESAS_Server->iNeedToRT==0)  //收到"20"就會設定
//            iNeedRT=0;
//        else
//            iNeedRT=1;

//        iFTRTCount++;
    }
    else
    {
        iFTRTCount++;

        if(iFTRTCount<=1 &&
           TestIF_File.bLowYieldForArtFT==true &&                               //Steven 20220208 : Add for ATK, ART low Yield
           TestIF_File.dLowYieldForArtFT<dCurrYield)
        {
            iRet=ShowErrorMessage("WAR07361", K_RETRY|K_TRAY_FEED, MMSystem, false, "CheckNeedRT");
        }

        if(iRet==K_TRAY_FEED)
        {
            iNeedRT=0;                                                          //Low Yield, 不用RT
        }
        else if(iFTRTCount>TestIF_File.iSCKART_TryCnt)                          //Steven 20250325 : fixed for ART
        {
            if(CUSTOMER_CODE==CC_TERAPOWER)
                iNeedRT=2;                                                      //Sam 20250702 : Final RT 後還會++ iNeedRT 狀態還需要維持"2"
            else
                iNeedRT=0;
        }
        else if(iFTRTCount==TestIF_File.iSCKART_TryCnt)                         //最後一次要執行Final ART
        {
            iNeedRT=2;
        }
        else if(iFTRTCount<TestIF_File.iSCKART_TryCnt &&                        //當次數還沒到
                dCurrYield<TestIF_File.dSCKART_Yield)                           //而且Yield還是小於設定
        {
            iNeedRT=1;
        }
        else if(dCurrYield>=TestIF_File.dSCKART_Yield &&                        //Yield已經到了
                iNeedRT!=2)                                                     //而且不是已經執行過Final ART, 要執行一次的Final ART
        {
            iNeedRT=2;
        }
        else
        {
            iNeedRT=0;                                                          //不用RT
        }
    }

    if(TestIF_File.bRENESAS_EnableFTCT==false)                                  //RogerYang 20251013 : RogerYang 瑞薩FT-CT 不在這裡變更
    {
        if(iFTRTCount==1 && iNeedRT==1)
            iCurrent93KARTStep=5;
        else if(iNeedRT==1)
            iCurrent93KARTStep=10;
    }

    if(iNeedRT!=0)
    {
        if(TestIF_File.bRENESAS_EnableFTCT==true)                               //RogerYang 20251015 : 瑞薩FTCT不能在這裡清零(20指令會把iSCKART_RTUnitCount設為零，重複再接收20會變成沒有RT)
        {
            if(iInputCount==0 &&                                                //沒有人員手動輸入的情況(30報表產出會清空)，且真的有RT數量
                LastSet.iSCKART_RTUnitCount!=0)
            {
                iInputCount=LastSet.iSCKART_RTUnitCount;
                iInputCount+=(LastSet.iHdRejectATFail+                          //Fail收納(＋JAM去除數)"＝投入數(Qty)
                              LastSet.iHdRejectATPass+
                              LastSet.iHdRejectBT);
            }
            LastSet.iSCKART_RTUnitCount=0;
            AccessFile(false, -1);
        }
        else
        {
            if(LastSet.iSCKART_RTUnitCount==0)
            {
                iNeedRT=0;
            }

            if(iTesterType==1)
            {
                if(IniConfig.bSPILFunction)                                     //JerryYang 20220923 : SPIL手動輸入reject數量(要重測的)
                {
                    iInputCount=LastSet.iSCKART_RTUnitCount+iManualRejectCnt;
                    iManualRejectCnt=0;
                }
                else
                {
                    iInputCount=LastSet.iSCKART_RTUnitCount;
                }
                LastSet.iSCKART_RTUnitCount=0;
                AccessFile(false, 1);
            }
        }

        if(LastSet.iSCKART_RTUnitCount<iDevice &&
           LastSet.iSCKART_RTUnitCount!=0)                                      //Steven 20170717 (wei) : RT的時候可能數量比較少,就要使用Fix模式
        {
            ArmSpeed[InArm].bVariModeFIX=true;
            if(CUSTOMER_CODE==CC_TERAPOWER)                                     //Sam 20211108 : TPW OutArm 也要一顆一顆放
                ArmSpeed[OutArm].bVariModeFIX=true;
        }
        else
        {
            ArmSpeed[InArm].bVariModeFIX=bBackUpInArmMode;
            if(CUSTOMER_CODE==CC_TERAPOWER)                                     //Sam 20211108 : TPW OutArm 也要一顆一顆放
                ArmSpeed[OutArm].bVariModeFIX=bBackUpOutArmMode;
        }
    }
    else
    {
        ArmSpeed[InArm].bVariModeFIX=bBackUpInArmMode;
        if(CUSTOMER_CODE==CC_TERAPOWER)                                         //Sam 20211108 : TPW OutArm 也要一顆一顆放
            ArmSpeed[OutArm].bVariModeFIX=bBackUpOutArmMode;
    }
}
//------------------------------------------------------------------------------
void TfSCKART::DoAutoSocketOff(bool bAllSiteOn)
{
    int iSiteCount=0;                                                           //JerryYang 20220923 : 最小關site數量
    double dMaxYield=0, dYield[2][MAX_SOCKET_ROW][MAX_SOCKET_COL];
    double dTargetYield;

    if(TestIF_File.bSCKART_AutoSocketOff)
    {
        if(bAllSiteOn==false && iFTRTCount!=0)
        {
            if(TestIF_File.iSCKART_AutoSocketOffMode==0)                        //Auto Head
            {
                for(int i=0; i<TestSocket.iShtRow; i++)
                {
                    for(int j=0; j<TestSocket.iShtCol; j++)
                    {
                        dYield[0][i][j]=ArmData[0]->ArmSKET[i][j]->GetPCA();
                        if(dYield[0][i][j]>dMaxYield)
                        {
                            dMaxYield=dYield[0][i][j];
                        }

                        dYield[1][i][j]=ArmData[1]->ArmSKET[i][j]->GetPCA();
                        if(dYield[1][i][j]>dMaxYield)
                        {
                            dMaxYield=dYield[1][i][j];
                        }
                    }
                }

                dTargetYield=dMaxYield-TestIF_File.dSCKART_AutoSocketOffYield;

                if(dTargetYield>0)
                {
                    for(int k=0; k<2; k++)
                    {
                        for(int i=0; i<TestSocket.iShtRow; i++)
                        {
                            for(int j=0; j<TestSocket.iShtCol; j++)
                            {
                                if(dYield[k][i][j]<dTargetYield)
                                {
                                    LastSet.bUseTestSocket[k][i][j]=false;
                                }
                            }
                        }
                    }
                }
            }
            else if(TestIF_File.iSCKART_AutoSocketOffMode==1)                   //Auto Socket
            {
                for(int i=0; i<TestSocket.iShtRow; i++)
                {
                    for(int j=0; j<TestSocket.iShtCol; j++)
                    {
                        iSiteCount++;                                           //JerryYang 20220923 : 最小關site數量
                        dYield[0][i][j]=0;
                        if(LastSet.bUseTestSocket[0][i][j] && LastSet.bUseTestSocket[1][i][j])
                            dYield[0][i][j]=(ArmData[0]->ArmSKET[i][j]->GetPCA()+ArmData[1]->ArmSKET[i][j]->GetPCA())/2.0;
                        else if(LastSet.bUseTestSocket[0][i][j])
                            dYield[0][i][j]=ArmData[0]->ArmSKET[i][j]->GetPCA();
                        else
                            dYield[1][i][j]=ArmData[1]->ArmSKET[i][j]->GetPCA();

                        if(dYield[0][i][j]>dMaxYield)
                        {
                            dMaxYield=dYield[0][i][j];
                        }
                    }
                }

                dTargetYield=dMaxYield-TestIF_File.dSCKART_AutoSocketOffYield;
                if(dTargetYield>0)
                {
                    for(int i=0; i<TestSocket.iShtRow; i++)
                    {
                        for(int j=0; j<TestSocket.iShtCol; j++)
                        {
                            if(dYield[0][i][j]<dTargetYield)
                            {
                                iSiteCount--;                                   //JerryYang 20220923 : 最小關site數量
                                LastSet.bUseTestSocket[0][i][j]=false;
                                LastSet.bUseTestSocket[1][i][j]=false;
                            }
                        }
                    }
                }

                if(iSiteCount<TestIF_File.iSCKART_MinAutoCloseSite)             //JerryYang 20220923 : 最小關site數量
                {
                    ShowErrorMessage("WAR0724", K_RETRY, MMInterface);
                }
            }
            else                                                                //Manual
            {
            }
            fMain->ShowTestHeadComp(false);
        }
        else //if(iFTRTCount==0)                                                //Final RT後, 全開
        {
            for(int i=0; i<TestSocket.iShtRow; i++)
            {
                for(int j=0; j<TestSocket.iShtCol; j++)
                {
                    if(IniConfig.bI35UseThirdSiteControlByEngineer)             //Steven 20180829 : #P180828-ATK-H9-02 Auto Enable Site after finish ART even disable site by using "Eng Site" (White Color)
                    {
                        if(TestIF_File.iSiteMap[i][j]!=0)
                        {
                            if(LastSet.bUseTestSocketEE[0][i][j]==true)
                                LastSet.bUseTestSocket[0][i][j]=true;
                            if(LastSet.bUseTestSocketEE[1][i][j]==true)
                                LastSet.bUseTestSocket[1][i][j]=true;
                        }
                    }
                    else
                    {
                        if(TestIF_File.iSiteMap[i][j]!=0)
                        {
                            LastSet.bUseTestSocket[0][i][j]=true;
                            LastSet.bUseTestSocket[1][i][j]=true;
                        }
                    }
                }
            }
            fMain->ShowTestHeadComp(true);
        }
    }
}
//---------------------------------------------------------------------------
void TfSCKART::AddOutputJamCnt(int row, int col, int ret, int iBinOnCarryKit)   //RogerYang 20260226 : 修正計數 //RogerYang 20250923 : 整合ART OutArm JamCount
{
    fSCKART->iOutputJamCnt++;
    if(TestIF_File.bRENESAS_EnableFTCT==true && ret==K_SKIP)
    {
        if(iBinOnCarryKit!=0)                                                   //RogerYang 20260226 : 修正計數 //PickUp Error發生在Shuttle上
        {
            if(iBinOnCarryKit==1)   //(bIsPassBinOnCarryKit==true?)1:2;
            {
                LastSet.iHdRejectATPass++;
            }
            else
            {
                LastSet.iHdRejectATFail++;
            }
        }
        else
        {
            if(Prod.bIsPassBin[OutArmSuck.iBinData[row][col]]==true)
            {
                LastSet.iHdRejectATPass++;
            }
            else
            {
                LastSet.iHdRejectATFail++;
            }
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfSCKART::btnDeleteLotInfoClick(TObject *Sender)
{
    LastSet.lShuttleCount=0;
    LastSet.iSCKARTInputCT=0;                                                   //Steven 20170327 (wei) : Fixed for ART count
    sLotID="";
    sProcessCode="";
    sLotStartTime="";
    iLotCount=0;
    ClearLotInfo();
    fCounterClear->ClearCount(ctTraySortCount);
}
//------------------------------------------------------------------------------
void __fastcall TfSCKART::TimerSCKARTFlowTimer(TObject *Sender)
{
    if(CUSTOMER_CODE!=CC_SCK)                                                   //Steven 20171211 (Wei) : 只有SCK有Flex版本
        iTesterType=1;

    if(iTesterType==1)
    {
        ts93KFlow->TabVisible=true;
        tsFlexFlow->TabVisible=false;
        if(CUSTOMER_CODE!=CC_SCK)
            ts93KFlow->Caption="ART FLOW";

        TALed *ledList[]={aledInitART,                  //0
                          aledFTLotStart,               //1
                          aledWaitingTestercommand1,    //2
                          aledFTTesting,                //3
                          aledFTLotEnd,                 //4
                          aledWaitingTestercommand2,    //5
                          aledMoveTrayToLoader,         //6
                          aledRTLotStart,               //7
                          aledWaitingTestercommand3,    //8
                          aledRTTesting,                //9
                          aledRTLotEnd,                 //10
                          aledWaitingTestercommand4,    //11
                          aledARTFinish                 //12
                         };

        for(int i=0; i<13; i++)
        {
            if(iCurrent93KARTStep==0)
            {
                ledList[i]->TrueColor=clLime;
                ledList[i]->Value=false;
                ledList[i]->Blink=false;
            }
            else
            {
                if(i<iCurrent93KARTStep)
                {
                    ledList[i]->TrueColor=clLime;
                    ledList[i]->Value=true;
                    ledList[i]->Blink=false;
                }
                else if(i==iCurrent93KARTStep)
                {
                    ledList[i]->Value=true;
                    ledList[i]->TrueColor=clYellow;
                    ledList[i]->Blink=true;
                }
                else
                {
                    ledList[i]->TrueColor=clLime;
                    ledList[i]->Value=false;
                    ledList[i]->Blink=false;
                }
            }
        }
    }
    else
    {
        ts93KFlow->TabVisible=false;
        tsFlexFlow->TabVisible=true;

        TALed *ledList[]={ledFlexInitART,           //0
                          ledFlexFTQTY,             //1
                          ledFlexLotClear,          //2
                          ledFlexLotQty,            //3
                          ledFlexFTTesting,         //4
                          ledFlexFTStsL,            //5
                          ledFlexFTAlarm,           //6
                          ledFlexFTStsR,            //7
                          ledFlexRTQTY,             //8
                          ledFlexLotRTClear,        //9
                          ledFlexRTTesting,         //10
                          ledFlexRTStsL,            //11
                          ledFlexRTAlarm,           //12
                          ledFlexRTStsR,            //13
                          ledFlexStsF               //14
                         };

        for(int i=0; i<15; i++)
        {
            if(iCurrentFlexARTStep==0)
            {
                ledList[i]->TrueColor=clLime;
                ledList[i]->Value=false;
                ledList[i]->Blink=false;
            }
            else
            {
                if(i<iCurrentFlexARTStep)
                {
                    ledList[i]->TrueColor=clLime;
                    ledList[i]->Value=true;
                    ledList[i]->Blink=false;
                }
                else if(i==iCurrentFlexARTStep)
                {
                    ledList[i]->Value=true;
                    ledList[i]->TrueColor=clYellow;
                    ledList[i]->Blink=true;
                }
                else
                {
                    ledList[i]->TrueColor=clLime;
                    ledList[i]->Value=false;
                    ledList[i]->Blink=false;
                }
            }
        }
    }

    if(CosFunction.iAutoRetestTCPmode==2)                                       //RogerYang 20250916 : 瑞薩FT-CT
    {
        ledShowFTCTData->Value=(fMain->RENESAS_Server->ServerFrm->iOnLineStatus==1);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfSCKART::btnApplyQtyClick(TObject *Sender)
{
    if(bShowSetting)
    {
        iInputCount=atoi(edtInputCount->Text.c_str());
        palInputCount->Caption  =iInputCount;
        iManualRejectCnt=atoi(edtMRejectCnt->Text.c_str());                     //JerryYang 20220923 : SPIL手動輸入reject數量(要重測的)
        AccessFile(false, -1);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfSCKART::btnApplySettingClick(TObject *Sender)
{
    if(bShowSetting)
    {
        if(chkEnableART->Checked==false && chkEnableFTCT->Checked==true)        //RogerYang 20250916 : 瑞薩FT-CT
        {
            chkEnableFTCT->Checked=false;
            if(SystemInitialOK==true)
            {
                ShowMyMessage("Need Enable ART First!");
                return;
            }
        }

        TestIF_File.bSCKART_AutoSkip            =cbAutoSkip->Checked;               //Steven 20170315 (wei) : For SCK ART Auto Skip
        TestIF_File.iSCKART_AutoSkipCount       =atoi(edARTAutoSkip->Text.c_str()); //Steven 20170315 (wei) : For SCK ART Auto Skip
        TestIF_File.iSCKART_MinAutoCloseSite    =atoi(edtAlmAutoCloseSite->Text.c_str()); //JerryYang 20220923 : 最小關site數量
        TestIF_File.bAlarmAfterSendSRQKIND2     =chkAlarmKIND2->Checked;            //Steven 20230119 : ATK 要在SRQKIND2之後Alarm
        TestIF_File.bSCKART_AutoSocketOff       =cbAutoSocketOff->Checked;
        TestIF_File.iSCKART_AutoSocketOffMode   =cbRTOffMode->ItemIndex;
        TestIF_File.dSCKART_AutoSocketOffYield  =atof(edBestYield->Text.c_str());
        TestIF_File.iSCKART_SortMode            =cbSortMode->ItemIndex;
        TestIF_File.iSCKART_RTStartMode         =cbRTStartMode->ItemIndex;
        TestIF_File.bSCKART_AutoCleanOut        =cbAutoCleanOut->Checked;
        TestIF_File.bSCKART_AutoLotEnd          =cbAutoLotEnd->Checked;
        TestIF_File.bSCKART_FinalLotEndMsg      =cbFinalLotEndMsg->Checked;
        TestIF_File.bSCKART_LotDeviceCheck      =cbLotDeviceCheck->Checked;
        TestIF_File.bSCKART_RunARTWithoutCmd    =chkNoTesterCmd->Checked;       //Steven 20180830 : Run ART without tester command
        TestIF_File.iSCKLdCntLimN               =atoi(edtLdCntLimN->Text.c_str()); //RogerYang 20251224 : Rf360需求 lotcheck加入上下限設定
        TestIF_File.iSCKLdCntLimP               =atoi(edtLdCntLimP->Text.c_str()); //RogerYang 20251224 : Rf360需求 lotcheck加入上下限設定

        if(TestIF_File.bSCKART_RunARTWithoutCmd)
        {
            TestIF_File.iSCKART_RTStartMode=1;
        }

        TestIF_File.bSCKART_EnableART           =chkEnableART->Checked;         //Steven 20170919 (wei) : Add for SCK ART
//        TestIF_File.bSCKART_LockTestMode        =chkARTTestMode->Checked;
//        TestIF_File.iSCKART_TestMode            =cbbARTTestMode->ItemIndex;
//        TestIF_File.bSCKART_OverWriteVersion    =chkVersion->Checked;
        TestIF_File.sSCKART_VersionCmd          =edtVersion->Text;              //Richard 20230428 : Enable sSCKART_VersionCmd
        TestIF_File.bSCKART_EnableSPBinAlarm    =cbSPBinAlarm->Checked;         //Isaac 20171113 (Steven) : add ATK Special Bin Yield alarm
        TestIF_File.iSCKART_SPBinSelect         =cbSPBinSelect->ItemIndex;
        TestIF_File.bLowYieldForArtFT           =chkLowYieldForFT->Checked;     //Steven 20220208 : Add for ATK, ART low Yield
        TestIF_File.dLowYieldForArtFT           =atof(edtLowYieldForFT->Text.c_str());
        TestIF_File.dSCKART_SPBinAlarmYield     =atof(edSPBinYield->Text.c_str());
        TestIF_File.bRENESAS_EnableFTCT         =chkEnableART->Checked?chkEnableFTCT->Checked:false;  //RogerYang 20250916 : 瑞薩FT-CT

        if(SystemInitialOK==true)
            iManualStart=chkEnableFTCT->Checked?0:1;                            //RogerYang 20250916 : 瑞薩FT-CT 解鎖用

        if(CUSTOMER_CODE==CC_AMKOR_Japan)                                       //RogerYang 20251108 : 瑞薩FT-CT 設定需要by機台
        {
            IniConfig.bA10_AutoReTest=TestIF_File.bSCKART_EnableART;
            IniConfig.bA10_7_Renesas_FTCT=TestIF_File.bRENESAS_EnableFTCT;
            AnsiString sPath=AuthPath+"config.ini";
            WriteIniData(sPath, "Function", "bAutoReTest",              IniConfig.bA10_AutoReTest);
            WriteIniData(sPath, "Function", "bA10_7_Renesas_FTCT",      IniConfig.bA10_7_Renesas_FTCT);
        }

        AccessFile(false, -1);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfSCKART::btnApplyLotInfoClick(TObject *Sender)
{
    if(bShowSetting)
    {
        TestIF_File.dSCKART_Yield               =atof(edARTPassRate->Text.c_str());
        TestIF_File.iSCKART_TryCnt              =atoi(edlRTTryCnt->Text.c_str());
        iLotCount                               =atoi(edLotCount->Text.c_str());
        SetLotStatus(cbLotStatus->ItemIndex);
        AccessFile(false, -1);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfSCKART::edtVersionMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_NO_SYMBOL);
}
//---------------------------------------------------------------------------
void __fastcall TfSCKART::edSPBinYieldClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 0.0, 100.00);
}
//------------------------------------------------------------------------------
void TfSCKART::SaveTestSummary(int iSaveData)                                   //Steven 20190521 : ATK lot count
{
    if(CosFunction.bSortingBy2DList==true &&                                    //Steven 20240830 : 簡化2D sort summary判斷式
       LastSet.iTester==_2D_SORT &&                                             //JerryYang 20230322 : SPIL 2D SORT summary修改
       TestIF_File.bSortingBy2DIDList==true)
    {
        Save2DSortingSummary(iSaveData);
    }
    else if(CosFunction.bART_SECSGEM_93K==true)
    {
        SaveTestSummarySECS(iSaveData);
    }
    else if(TestIF_File.iTestType==TCP_IP_MODE)
    {
        if(iSaveData)
            fTesterTCP->ProcessOSPrint();                                       //Steven 20250515 : 整合Open Short測試報表

        SaveTestSummaryTSV(iSaveData);
    }
    else //if(IniConfig.bN09_LotCountAutoFunc)                                  //Steven 20240830 : N09獨立出來
    {
        if(iSaveData==1)
            SaveSummaryTrayFeed();

        SaveTestSummaryTSV(iSaveData);
    }
}
//------------------------------------------------------------------------------
void TfSCKART::SaveTestSummarySECS(int iSaveData)                               //Steven 20190521 : ATK lot count
{
    AnsiString FileName, PathName, PathName2, Str, Str1, Str2, IP, tmps1, strPath, strFileName="", str2, str3="";
    AnsiString sTestBinNo=StringReplace(fSCKART->sInfo_TestBinNo, ":", "-", TReplaceFlags()<<rfReplaceAll);
    int iCount=0, iUnloadCount=0;
    bool bResult=false;
    map<AnsiString, AnsiString>mapIPList;
    map<AnsiString, AnsiString>::iterator mapIPIter;
    AnsiString Data="";
    AnsiString sLotID1=fSCKART->sLotID;

    if(sLotID1=="" && fLotInfo->edtSysLotID->Text!="")                          //Steven 20230317 : 避免lot ID是空值
        sLotID1=fLotInfo->edtSysLotID->Text;
    if(sLotID1=="")                                                             //Steven 20230814 : 沒有Lot ID, 就帶入日期時間
        sLotID1.sprintf("%04d%02d%02d%02d%02d%02d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);

    bool bHaveBinData=false;
    int temp;
    AnsiString aUnloader[eTrayCount];
    int iUnloadCnt[eTrayCount]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0};
    GetTimeInfo();

    fSCKART->sLotEndTime.sprintf("%04d%02d%02d%02d%02d%02d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);

    PathName2.sprintf("%s\\%04d\\%02d\\", asSummaryPath, SystemYear, SystemMonth);    //Steven 20230215 : 存檔路徑加上年月
    MyForceDirectories(PathName2, "TfSCKART::SaveTestSummarySECS");

    if(sProcessCode=="")
        sProcessCode="FT1";

    if(IniConfig.bSPILFunction==true)
    {
        if(IniConfig.bA38_SLT_Summary)
        {
            FileName.sprintf("%s_%s_%s_%s_%s_%s_%s_%s_%s.txt",  fSCKART->sInfo_Stage, fSCKART->sInfo_Step, fSCKART->sInfo_ReportCnt, fSCKART->sInfo_CustLotID,
                                                                sLotID1, fSCKART->sInfo_TesterID, fSCKART->sInfo_HandlerID, sTestBinNo, fSCKART->sLotEndTime);
        }
        else
        {
            FileName.sprintf("%s_%s_%s_%s_%s_%s_%s_%s_%s.txt",  fSCKART->sInfo_Stage, fSCKART->sInfo_Step, fSCKART->sInfo_ReportCnt, fSCKART->sInfo_CustLotID,
                                                                sLotID1, fSCKART->sInfo_TesterID, fSCKART->sInfo_HandlerID, sTestBinNo, fSCKART->sLotEndTime);
        }
    }
    else
    {
        FileName.sprintf("%s_%s_%s_%s_%s_%s_%s_%s_%04d%02d%02d%02d%02d.txt", fSCKART->sInfo_Stage, fSCKART->sInfo_Step, fSCKART->sInfo_ReportCnt, fSCKART->sInfo_CustLotID,
                                                                             sLotID1, fSCKART->sInfo_TesterID, fSCKART->sInfo_HandlerID, sTestBinNo, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin);
    }

    if(iSaveData==0)
        return;

    TStringList *sList=new TStringList();

    if(IniConfig.bSPILFunction==true)                                           //JerryYang 20240705 : 丁曉冬要求的格式
    {
        Str.sprintf("CUSTOMER:%s", fSCKART->sInfo_Customer);
        sList->Add(Str);
        Str.sprintf("STAGE:%s", fSCKART->sInfo_Stage);
        sList->Add(Str);
        Str.sprintf("INNER_LOT_ID:%s", sLotID1);
        sList->Add(Str);
        Str.sprintf("CURR_QTY:%s", fSCKART->sInfo_CurrQty);
        sList->Add(Str);
        Str.sprintf("TEST_BIN_NO:%s", fSCKART->sInfo_TestBinNo);
        sList->Add(Str);
        Str.sprintf("DEVICE_NAME:%s", fSCKART->sInfo_DeviceName);
        sList->Add(Str);
        Str.sprintf("STEP:%s", fSCKART->sInfo_Step);
        sList->Add(Str);
        Str.sprintf("CUST_LOT_ID:%s", fSCKART->sInfo_CustLotID);
        sList->Add(Str);
        Str.sprintf("OPERATOR_ID:%s", fSCKART->sInfo_OperatorID);
        sList->Add(Str);
        Str.sprintf("TEMPERATURE:%s", fSCKART->sInfo_Temperauture);
        sList->Add(Str);
        Str.sprintf("REPORTCOUNT:%s", fSCKART->sInfo_ReportCnt);
        sList->Add(Str);
        Str.sprintf("PROGRAM_NAME:%s", fSCKART->sInfo_ProgramName);
        sList->Add(Str);
        Str.sprintf("HANDLER_ID:%s", fSCKART->sInfo_HandlerID);
        sList->Add(Str);
        Str.sprintf("TESTER_ID:%s", fSCKART->sInfo_TesterID);
        sList->Add(Str);
        Str.sprintf("CUSTOMER_DEVICE_GROUP:%s", fSCKART->sInfo_CustDevGup);
        sList->Add(Str);

        if(fSCKART->sLotStartTime!="")                                          //Richard 20230111 : Fix fSCKART->sLotStartTime 為NULL。
        {
            Str.sprintf("SUMMARY_START_TIME:%s", fSCKART->sLotStartTime);
        }
        else
        {
            Str.sprintf("SUMMARY_START_TIME:%s%s%s%s%s%s", RunInfo.LotStartTime.SubString(1, 4),  RunInfo.LotStartTime.SubString(6, 2),  RunInfo.LotStartTime.SubString(9, 2),
                                                           RunInfo.LotStartTime.SubString(12, 2), RunInfo.LotStartTime.SubString(15, 2), RunInfo.LotStartTime.SubString(18, 2));
        }
        sList->Add(Str);
        Str.sprintf("SUMMARY_END_TIME:%s", fSCKART->sLotEndTime);
        sList->Add(Str);
    }
    else
    {
        Str.sprintf("CUSTOMER:%s", fSCKART->sInfo_Customer);
        sList->Add(Str);
        Str.sprintf("INNER_LOT_ID:%s", sLotID1);
        sList->Add(Str);
        Str.sprintf("CUST_LOT_ID:%s", fSCKART->sInfo_CustLotID);
        sList->Add(Str);
        Str.sprintf("CUSTOMER_DEVICE_GROUP:%s", fSCKART->sInfo_CustDevGup);
        sList->Add(Str);
        Str.sprintf("DEVICE_NAME:%s", fSCKART->sInfo_DeviceName);
        sList->Add(Str);
        Str.sprintf("STAGE:%s", fSCKART->sInfo_Stage);
        sList->Add(Str);
        Str.sprintf("STEP:%s", fSCKART->sInfo_Step);
        sList->Add(Str);
        Str.sprintf("REPORTCOUNT:%s", fSCKART->sInfo_ReportCnt);
        sList->Add(Str);
        Str.sprintf("PROGRAM_NAME:%s", fSCKART->sInfo_ProgramName);
        sList->Add(Str);
        Str.sprintf("TEST_BIN_NO:%s", fSCKART->sInfo_TestBinNo);
        sList->Add(Str);
        Str.sprintf("TESTER_ID:%s", fSCKART->sInfo_TesterID);
        sList->Add(Str);
        Str.sprintf("HANDLER_ID:%s", fSCKART->sInfo_HandlerID);
        sList->Add(Str);
        Str.sprintf("TEMPERATURE:%s", fSCKART->sInfo_Temperauture);
        sList->Add(Str);
        Str.sprintf("CURR_QTY:%s", fSCKART->sInfo_CurrQty);
        sList->Add(Str);
        Str.sprintf("OPERATOR_ID:%s", fSCKART->sInfo_OperatorID);
        sList->Add(Str);
        if(fSCKART->sLotStartTime!="")                                          //Richard 20230111 : Fix fSCKART->sLotStartTime 為NULL。
        {
            Str.sprintf("SUMMARY_START_TIME:%s", fSCKART->sLotStartTime);
        }
        else
        {
            Str.sprintf("SUMMARY_START_TIME:%s%s%s%s%s%s", RunInfo.LotStartTime.SubString(1, 4), RunInfo.LotStartTime.SubString(6, 2), RunInfo.LotStartTime.SubString(9, 2),
                                                           RunInfo.LotStartTime.SubString(12, 2), RunInfo.LotStartTime.SubString(15, 2), RunInfo.LotStartTime.SubString(18, 2));
        }
        sList->Add(Str);
        Str.sprintf("SUMMARY_END_TIME:%s", fSCKART->sLotEndTime);
        sList->Add(Str);
    }

    for(int iBin=0; iBin<=iTestBinCount; iBin++)
    {
        iCount=0;
        for(int i=0; i<TestSocket.iShtRow; i++)
        {
            for(int j=0; j<TestSocket.iShtCol; j++)
            {
                iUnloadCount+=LotSummary.iCountCategory[iCount][iBin];
                iCount++;
            }
        }
    }

    for(int i=0; i<eTrayCount; i++)
    {
        aUnloader[i]=s6ShortTrayName[i]+AnsiString("(");
        bHaveBinData=false;
        if(Prod.iTrayType[i]!=tNotUse)
        {
            for(int j=0; j<=iTestBinCount; j++)
            {
                temp=Prod.iT6PosCate[j];
                if(temp<=0 && j!=iTestBinCount)
                    continue;
                if(i==temp-1)
                {
                    if(bHaveBinData==false)
                        aUnloader[i]+=AnsiString(j);
                    else
                        aUnloader[i]+=AnsiString(",")+AnsiString(j);

                    bHaveBinData=true;
                    iUnloadCnt[i]+=LotSummary.iTotalCategory[j];
                }

                if(Prod.iIfErrorT6==i && j==iTestBinCount)
                {
                    iUnloadCnt[i]+=LotSummary.iTotalCategory[j];
                }
            }

            if(Prod.iIfErrorT6==i)
            {
                aUnloader[i]+="error";
                bHaveBinData=true;
            }
            else
            {
                if(bHaveBinData==false)
                    aUnloader[i]+="Null";
            }
            aUnloader[i]+=")";
        }
    }

    Str.sprintf("Handler Load Qty:%d", LastSet.iSCKARTInputCT);                 //JerryYang 20200319 fix summary
    sList->Add(Str);
    Str.sprintf("Handler Unload Qty:%d", iUnloadCount);
    sList->Add(Str);
    sList->Add("");
    sList->Add(" ========================================================================");
    sList->Add(" =========================  Group Bin Summary  ===========================");
    sList->Add(" ========================================================================");

    for(int i=0; i<eTrayCount; i++)
    {
        if(Prod.iTrayType[i]!=tNotUse)
        {
            Str.sprintf("%s:%d", aUnloader[i], iUnloadCnt[i]);
            sList->Add(Str);
        }
    }

    sList->Add("");
    sList->Add(" ========================================================================");
    sList->Add(" =========================  Hard Bin Summary  ===========================");
    sList->Add(" ========================================================================");

    iCount=0;
    Str=" Hard P/F ";
    Str2=" ---- ----";
    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            iCount++;
            Str1.sprintf(" Site%d", iCount);
            Str=Str+Str1;
            Str2=Str2+" -----";
        }
    }
    Str=Str+" Total  Yield";
    sList->Add(Str);
    Str2=Str2+" ----- --------";
    sList->Add(Str2);

    for(int iBin=0; iBin<iTestBinCount; iBin++)
    {
        temp=Prod.iT6CatData[iBin];
        if(IniConfig.bSPILFunction==true && (temp<0 || temp>=eTrayCount))
            continue;

            Str.sprintf("%d", iBin);
        while(Str.Length()<5)
        {
            Str=" "+Str;
        }

        if(Prod.bIsPassBin[iBin])
            Str=Str+" PASS";
        else
            Str=Str+" FAIL";

        iCount=0;
        for(int i=0; i<TestSocket.iShtRow; i++)
        {
            for(int j=0; j<TestSocket.iShtCol; j++)
            {
                Str2.sprintf("%d", LotSummary.iCountCategory[iCount][iBin]);
                while(Str2.Length()<6)
                {
                    Str2=" "+Str2;
                }
                Str=Str+Str2;
                iCount++;
            }
        }

        if(IniConfig.bSPILFunction==true && LotSummary.iTotalCategory[iBin]<=0)
            continue;

        Str2.sprintf("%d", LotSummary.iTotalCategory[iBin]);
        while(Str2.Length()<6)
        {
            Str2=" "+Str2;
        }
        Str=Str+Str2;

        if(IniConfig.bSPILFunction==true)                                       //JerryYang 20220923 : unload數量為0不要存log
        {
            Str2=ChangeToPercentage(LotSummary.iTotalCategory[iBin], iUnloadCount);
        }
        else
        {
            Str2=ChangeToPercentage(LotSummary.iTotalCategory[iBin], iLotCount);
        }

        while(Str2.Length()<9)
        {
            Str2=" "+Str2;
        }
        Str=Str+Str2;

        sList->Add(Str);
    }

    sList->Add(" ========================================================================");

    if(FileExists(PathName2+FileName)==false)                                   //Steven 20230317 : 避免連續存兩次把資料蓋掉了
    {
        if(IniConfig.bSPILFunction==true)                                       //JerryYang 20220923 : unload數量為0不要存log
        {
            if(iUnloadCount>0)
                sList->SaveToFile(PathName2+FileName);
        }
        else
        {
            sList->SaveToFile(PathName2+FileName);
        }
        fObserver->memoLotSummary->Lines=sList;
    }
    sList->Clear();
    delete sList;

    if(IniConfig.bN17UploadLotSummary)                                          //JerryYang 20220923 : Upload lot summary
    {
        if(DirectoryExists(IniConfig.asN17LotSummaryPath))
        {
            if(IniConfig.bA38_SLT_Summary &&
               fSCKART->sLotID!="NA" &&
               fSCKART->sLotID!="" &&
               iUnloadCount>0)
            {
                strFileName=PathName2+FileName;                                 //Steven 20230215 : 存檔路徑加上年月
                if(FileExists(strFileName))
                {
                    strPath.sprintf("%s\\%s", IniConfig.asN17LotSummaryPath, FileName);
                    bResult=CopyFile(strFileName.c_str(), strPath.c_str(), true);
                    if(bResult==false)
                    {
                        ShowMyMessage("Uploaded lot summary error\r\nPlease check the path of N-17.","上傳lot summary失敗\r\n請檢查N-17路徑是否存在");
                    }
                    else
                    {
                        RecordProcess("Uploaded lot summary successfully.");    //Steven 20190722 : add TSV log
                    }
                }
            }
            else
            {
                if(fSCKART->sInfo_CustLotID=="NA" || fSCKART->sInfo_CurrQty=="NA" ||
                   fSCKART->sInfo_CustLotID=="" || fSCKART->sInfo_CurrQty=="")
                {
                }
                else
                {
                    strFileName=PathName2+FileName;                             //Steven 20230215 : 存檔路徑加上年月
                    if(FileExists(strFileName))
                    {
                        strPath.sprintf("%s\\%s", IniConfig.asN17LotSummaryPath, FileName);
                        bResult=CopyFile(strFileName.c_str(), strPath.c_str(), true);
                        if(bResult==false)
                        {
                            ShowMyMessage("Uploaded lot summary error\r\nPlease check the path of N-17.","上傳lot summary失敗\r\n請檢查N-17路徑是否存在");
                        }
                        else
                        {
                            RecordProcess("Uploaded lot summary successfully.");//Steven 20190722 : add TSV log
                        }
                    }
                }
            }
        }
        else
        {
            ShowMyMessage("Uploaded lot summary error\r\nPlease check the path of N-17.", "上傳lot summary失敗\r\n請檢查N-17路徑是否存在");
        }
    }

    if(IniConfig.bA38_SLT_Summary && FileName!="" && iUnloadCount>0)
    {
        ShellExecute(NULL, NULL, strFileName.c_str(), NULL, NULL, SW_SHOW);
    }

    if(CosFunction.bUseTSVFunction &&                                           //Steven 20240904 : for ATK的TSV功能
       IniConfig.bN09_LotCountAutoFunc)
    {
        bWaitTSV=true;
        bShowTSVMsg=false;
        sTSVMsg="";
        RecordProcess("Lot end and send summary and waiting TSV reply.");       //Steven 20190722 : add TSV log
        TSVdelay.SetSecAndOn(IniConfig.dN09_SearchTime);
        TimerTSV->Enabled=true;
    }

    if(iSaveData==1)
        LotSummary.ClearAllData();
}
//------------------------------------------------------------------------------
void TfSCKART::SaveMultiLotTestSummary(bool bSaveData)
{
    AnsiString FileName, PathName, PathName2, Str="", Str1="", Str2, IP, tmps1, strPath, strFileName="", str2, str3="";
    int iCount=0, iUnloadCount=0;
    bool bResult=false;
    AnsiString Data="";
    AnsiString sLotID1=fSCKART->sLotID;

    if(sLotID1=="" && fLotInfo->edtSysLotID->Text!="")                          //Steven 20230317 : 避免lot ID是空值
        sLotID1=fLotInfo->edtSysLotID->Text;

    bool bHaveBinData=false;
    int temp;
    AnsiString aUnloader[eTrayCount];
    int iUnloadCnt[eTrayCount]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0};
    GetTimeInfo();

    fSCKART->sLotEndTime.sprintf("%04d%02d%02d%02d%02d%02d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);

    for(int k=0; k<=fSCKART->iInfo_MultiLotCnt; k++)
    {
        if(k==fSCKART->iInfo_MultiLotCnt && iE1Count==0 && iE2Count==0 && iE3Count==0 && iENotDefinedCount==0)
        {
            return;
        }

        for(int i=0; i<9; i++)
        {
            iUnloadCnt[i]=0;
        }

        iUnloadCount=0;

        PathName2.sprintf("%s\\%04d\\%02d\\", asSummaryPath, SystemYear, SystemMonth);    //Steven 20230215 : 存檔路徑加上年月
        MyForceDirectories(PathName2, "TfSCKART::SaveMultiLotTestSummary");

        if(MachineTypeChoice==Type_HT9045 ||
           MachineTypeChoice==Type_HT9045_12Site)                               //JerryYang 20230322 : SPIL 2D SORT summary修改
            fSCKART->sInfo_ProgramName="HT9045";
        else if(MachineTypeChoice==Type_HT9046)
            fSCKART->sInfo_ProgramName="HT9046";
        else if(MachineTypeChoice==Type_HT9046_LS)
            fSCKART->sInfo_ProgramName="HT9046LS";

        if(k==fSCKART->iInfo_MultiLotCnt)
        {
            Str="";
            for(int i=0; i<fSCKART->iInfo_MultiLotCnt; i++)
            {
                Str+=fSCKART->sInfoArr_InnerLotID[i];
            }
            FileName.sprintf("%s_%04d%02d%02d%02d%02d.txt", Str, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin);
        }
        else
        {
            FileName.sprintf("%s_%s_%s_%04d%02d%02d%02d%02d.txt", fSCKART->sInfoArr_InnerLotID[k], fSCKART->sInfoArr_CustLotID[k], fSCKART->sInfoArr_CustDevGup[k], SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin);
        }

        if(bSaveData==false)
            return;

//        TStringList *sIPList=new TStringList();
        TStringList *sList=new TStringList();

    //Lot_id:       0000C912X8Z.0100#
    //Handler ID:   H-xxxx
    //Handler IP:   255.255.255.255
    //DCC:          NONE
    //TEST_OPR:     FT1
    //RT_CODE:      1
    //TEMP:         25
    //Hostname:     k3tv9369
    //Total_Inqty:  86
    //
    // ========================================================================
    // =========================  Hard Bin Summary  ===========================
    // ========================================================================
    // Hard P/F  Site1 Site2 Site3 Site4 Site5 Site6 Site7 Site8 Total  Yield
    // ---- ---- ----- ----- ----- ----- ----- ----- ----- ----- ----- --------
    //    1 PASS     0     0     0     0    40    41     0     0    81  94.186%
    //    5 FAIL     0     0     0     0     0     2     0     0     2   2.326%
    //    6 FAIL     0     0     0     0     3     0     0     0     3   3.488%
    // ========================================================================
        if(k==fSCKART->iInfo_MultiLotCnt)
        {
            Str1="";
            for(int i=0; i<fSCKART->iInfo_MultiLotCnt; i++)
            {
                if(i==fSCKART->iInfo_MultiLotCnt-1)
                {
                    Str1+=fSCKART->sInfoArr_Customer[i];
                }
                else
                {
                    Str1=Str1+fSCKART->sInfoArr_Customer[i]+"_";
                }
            }
            Str.sprintf("CUSTOMER:%s", Str1);                                   //JerryYang 20230322 : SPIL 2D SORT summary修改
            sList->Add(Str);
            Str.sprintf("ASSEMBLY SITE:SPIL");
            sList->Add(Str);

            Str1="";
            for(int i=0; i<fSCKART->iInfo_MultiLotCnt; i++)
            {
                if(i==fSCKART->iInfo_MultiLotCnt-1)
                {
                    Str1+=fSCKART->sInfoArr_InnerLotID[i];
                }
                else
                {
                    Str1=Str1+fSCKART->sInfoArr_InnerLotID[i]+"_";
                }
            }
            Str.sprintf("INNER_LOT_ID:%s", Str1);
            sList->Add(Str);

            Str1="";
            for(int i=0; i<fSCKART->iInfo_MultiLotCnt; i++)
            {
                if(i==fSCKART->iInfo_MultiLotCnt-1)
                {
                    Str1+=fSCKART->sInfoArr_CustLotID[i];
                }
                else
                {
                    Str1=Str1+fSCKART->sInfoArr_CustLotID[i]+"_";
                }
            }
            Str.sprintf("CUST_LOT_ID:%s", Str1);                                //JerryYang 20230322 : SPIL 2D SORT summary修改
            sList->Add(Str);

            Str1="";
            for(int i=0; i<fSCKART->iInfo_MultiLotCnt; i++)
            {
                if(i==fSCKART->iInfo_MultiLotCnt-1)
                {
                    Str1+=fSCKART->sInfoArr_CustDevGup[i];
                }
                else
                {
                    Str1=Str1+fSCKART->sInfoArr_CustDevGup[i]+"_";
                }
            }
            Str.sprintf("CUSTOMER_DEVICE_GROUP:%s", Str1);
            sList->Add(Str);

            Str1="";
            for(int i=0; i<fSCKART->iInfo_MultiLotCnt; i++)
            {
                if(i==fSCKART->iInfo_MultiLotCnt-1)
                {
                    Str1+=fSCKART->sInfoArr_DeviceName[i];
                }
                else
                {
                    Str1=Str1+fSCKART->sInfoArr_DeviceName[i]+"_";
                }
            }
            Str.sprintf("DEVICE_NAME:%s", Str1);
            sList->Add(Str);

            #ifdef HiSilicon
            Str.sprintf("PROGRAM_NAME:%s.%s", fSCKART->sInfo_ProgramName, HISI_VERSION);
            #else
            Str.sprintf("PROGRAM_NAME:%s.%s", fSCKART->sInfo_ProgramName, MainVersion);
            #endif
            sList->Add(Str);
            Str.sprintf("HANDLER_ID:%s", IniConfig.SocketHandlerID);
            sList->Add(Str);
            Str.sprintf("OPERATOR_ID:%s", fSCKART->sInfoArr_OperatorID[k]);
            sList->Add(Str);
            Str.sprintf("OPERATION:VS");
            sList->Add(Str);
            Str1="";
            for(int i=0; i<fSCKART->iInfo_MultiLotCnt; i++)
            {
                if(i==fSCKART->iInfo_MultiLotCnt-1)
                {
                    Str1+=fSCKART->sInfoArr_Stage[i];
                }
                else
                {
                    Str1=Str1+fSCKART->sInfoArr_Stage[i]+"_";
                }
            }
            Str.sprintf("STAGE:%s", Str1);
            sList->Add(Str);
        }
        else
        {
            Str.sprintf("CUSTOMER:%s", fSCKART->sInfoArr_Customer[k]);          //JerryYang 20230322 : SPIL 2D SORT summary修改
            sList->Add(Str);
            Str.sprintf("ASSEMBLY SITE:SPIL");
            sList->Add(Str);
            Str.sprintf("INNER_LOT_ID:%s", fSCKART->sInfoArr_InnerLotID[k]);
            sList->Add(Str);
            Str.sprintf("CUST_LOT_ID:%s", fSCKART->sInfoArr_CustLotID[k]);      //JerryYang 20230322 : SPIL 2D SORT summary修改
            sList->Add(Str);
            Str.sprintf("CUSTOMER_DEVICE_GROUP:%s", fSCKART->sInfoArr_CustDevGup[k]);
            sList->Add(Str);
            Str.sprintf("DEVICE_NAME:%s", fSCKART->sInfoArr_DeviceName[k]);
            sList->Add(Str);
            #ifdef HiSilicon
            Str.sprintf("PROGRAM_NAME:%s.%s", fSCKART->sInfo_ProgramName, HISI_VERSION);  //JerryYang 20230822 : 修改檔名
            #else
            Str.sprintf("PROGRAM_NAME:%s.%s", fSCKART->sInfo_ProgramName, MainVersion);
            #endif
            sList->Add(Str);
            Str.sprintf("HANDLER_ID:%s", IniConfig.SocketHandlerID);
            sList->Add(Str);
            Str.sprintf("OPERATOR_ID:%s", fSCKART->sInfoArr_OperatorID[k]);
            sList->Add(Str);
            Str.sprintf("OPERATION:VS");
            sList->Add(Str);
            Str.sprintf("STAGE:%s", fSCKART->sInfoArr_Stage[k]);
            sList->Add(Str);
        }

        if(fSCKART->sLotStartTime!="")                                          //Richard 20230111 : Fix fSCKART->sLotStartTime 為NULL。
        {
            Str.sprintf("SUMMARY_START_TIME:%s", fSCKART->sLotStartTime);
        }
        else
        {
            Str.sprintf("SUMMARY_START_TIME:%s%s%s%s%s%s", RunInfo.LotStartTime.SubString(1,4),RunInfo.LotStartTime.SubString(6,2),RunInfo.LotStartTime.SubString(9,2),
                                                           RunInfo.LotStartTime.SubString(12,2),RunInfo.LotStartTime.SubString(15,2),RunInfo.LotStartTime.SubString(18,2));
        }
        sList->Add(Str);
        Str.sprintf("SUMMARY_END_TIME:%s", fSCKART->sLotEndTime);
        sList->Add(Str);

        if(k==fSCKART->iInfo_MultiLotCnt)
        {
            for(int iBin=0; iBin<=iTestBinCount; iBin++)
            {
                iCount=0;
                for(int i=0; i<TestSocket.iShtRow; i++)
                {
                    for(int j=0; j<TestSocket.iShtCol; j++)
                    {
                        iUnloadCount+=LotSummary.iCountCategory[iCount][iBin];
                        iCount++;
                    }
                }
            }
        }
        else
        {
            for(int iBin=0; iBin<=iTestBinCount; iBin++)
            {
                iCount=0;
                for(int i=0; i<TestSocket.iShtRow; i++)
                {
                    for(int j=0; j<TestSocket.iShtCol; j++)
                    {
                        iUnloadCount+=LotSummary.iByLotCountCategory[k][iCount][iBin];
                        iCount++;
                    }
                }
            }
        }

        for(int i=0; i<eTrayCount; i++)
        {
            if(Prod.iTrayType[i]!=tNotUse)
            {
                aUnloader[i]=s6ShortTrayName[i]+AnsiString("(");
                bHaveBinData=false;
                for(int j=0; j<=iTestBinCount; j++)
                {
                    temp=Prod.iT6PosCate[j];
                    if(temp<=0 && j!=iTestBinCount)
                    continue;
                    if(i==temp-1)
                    {
                        if(bHaveBinData==false)
                            aUnloader[i]+=AnsiString(j);
                        else
                            aUnloader[i]+=AnsiString(",")+AnsiString(j);
                        bHaveBinData=true;

                        if(k==fSCKART->iInfo_MultiLotCnt)
                            iUnloadCnt[i]+=LotSummary.iTotalCategory[j];
                        else
                            iUnloadCnt[i]+=LotSummary.iByLotTotalCategory[k][j];
                    }

                    if(Prod.iIfErrorT6==i && j==iTestBinCount)
                    {
                        if(k==fSCKART->iInfo_MultiLotCnt)
                            iUnloadCnt[i]+=LotSummary.iTotalCategory[j];
                        else
                            iUnloadCnt[i]+=LotSummary.iByLotTotalCategory[k][j];
                    }
                }

                if(Prod.iIfErrorT6==i)
                {
                    aUnloader[i]+="error";
                    bHaveBinData=true;
                }
                else
                {
                    if(bHaveBinData==false)
                        aUnloader[i]+="Null";
                }
                aUnloader[i]+=")";
            }
        }

        if(k==fSCKART->iInfo_MultiLotCnt)
        {
            Str.sprintf("Handler Load Qty:%d", LastSet.iSCKARTInputCT);         //JerryYang 20200319 fix summary
        }
        else
        {
            Str.sprintf("Handler Load Qty:%d", LotSummary.iByLotLoadCount[k]);  //JerryYang 20200319 fix summary
        }
        sList->Add(Str);

        Str.sprintf("Handler Unload Qty:%d", iUnloadCount);
        sList->Add(Str);
        sList->Add("");
        sList->Add(" ========================================================================");
        sList->Add(" =========================  Group Bin Summary  ===========================");
        sList->Add(" ========================================================================");

        for(int i=0; i<eTrayCount; i++)
        {
            if(Prod.iTrayType[i]!=tNotUse)
            {
                Str.sprintf("%s:%d", aUnloader[i], iUnloadCnt[i]);
                sList->Add(Str);
            }
        }

        sList->Add("");
        sList->Add(" ========================================================================");
        sList->Add(" =========================  Hard Bin Summary  ===========================");
        sList->Add(" ========================================================================");

        iCount=0;
        Str=" Hard P/F ";
        Str2=" ---- ----";
        for(int i=0; i<TestSocket.iShtRow; i++)
        {
            for(int j=0; j<TestSocket.iShtCol; j++)
            {
                iCount++;
                Str1.sprintf(" Site%d", iCount);
                Str=Str+Str1;
                Str2=Str2+" -----";
            }
        }
        Str=Str+" Total  Yield";
        sList->Add(Str);
        Str2=Str2+" ----- --------";
        sList->Add(Str2);

        for(int iBin=0; iBin<iTestBinCount+1; iBin++)
        {
            if(iBin==iTestBinCount)
            {
            }
            else
            {
                temp=Prod.iT6PosCate[iBin];
                if(temp<0 || temp>=eTrayCount ||
                   Prod.iTrayType[temp]==tNotUse)
                    continue;
            }

            if(iBin==iTestBinCount)
            {
                Str="Err";
            }
            else
            {
                Str.sprintf("%d", iBin);
            }

            while(Str.Length()<5)
            {
                Str=" "+Str;
            }

            if(iBin==iTestBinCount)
            {
                Str=Str+" FAIL";
            }
            else
            {
                if(Prod.bIsPassBin[iBin]==0)
                    Str=Str+" PASS";
                else
                    Str=Str+" FAIL";
            }

            if(k==fSCKART->iInfo_MultiLotCnt)
            {
                iCount=0;
                for(int i=0; i<TestSocket.iShtRow; i++)
                {
                    for(int j=0; j<TestSocket.iShtCol; j++)
                    {
                        Str2.sprintf("%d", LotSummary.iCountCategory[iCount][iBin]);
                        while(Str2.Length()<6)
                        {
                            Str2=" "+Str2;
                        }
                        Str=Str+Str2;
                        iCount++;
                    }
                }

                if(IniConfig.bSPILFunction==true && LotSummary.iTotalCategory[iBin]<=0)
                    continue;

                Str2.sprintf("%d", LotSummary.iTotalCategory[iBin]);
                while(Str2.Length()<6)
                {
                    Str2=" "+Str2;
                }
                Str=Str+Str2;
                Str2=ChangeToPercentage(LotSummary.iTotalCategory[iBin], iUnloadCount);

                while(Str2.Length()<9)
                {
                    Str2=" "+Str2;
                }
                Str=Str+Str2;

                sList->Add(Str);
            }
            else
            {
                iCount=0;
                for(int i=0; i<TestSocket.iShtRow; i++)
                {
                    for(int j=0; j<TestSocket.iShtCol; j++)
                    {
                        Str2.sprintf("%d", LotSummary.iByLotCountCategory[k][iCount][iBin]);
                        while(Str2.Length()<6)
                        {
                            Str2=" "+Str2;
                        }
                        Str=Str+Str2;
                        iCount++;
                    }
                }

                if(IniConfig.bSPILFunction==true && LotSummary.iByLotTotalCategory[k][iBin]<=0)
                    continue;

                Str2.sprintf("%d", LotSummary.iByLotTotalCategory[k][iBin]);
                while(Str2.Length()<6)
                {
                    Str2=" "+Str2;
                }
                Str=Str+Str2;
                Str2=ChangeToPercentage(LotSummary.iByLotTotalCategory[k][iBin], iUnloadCount);

                while(Str2.Length()<9)
                {
                    Str2=" "+Str2;
                }
                Str=Str+Str2;

                sList->Add(Str);
            }
        }

        sList->Add("");
        sList->Add(" ========================================================================");
        sList->Add(" ==========================  Yield Summary  =============================");
        sList->Add(" ========================================================================");

        Str ="  P/F    Qty    Yield";
        sList->Add(Str);
        Str2=" ----- ------ --------";
        sList->Add(Str2);

        int iPassCount=0;
        int iFailCount=0;
        if(k==fSCKART->iInfo_MultiLotCnt)
        {
            for(int iBin=0; iBin<iTestBinCount+1; iBin++)
            {
                if(iBin==iTestBinCount)
                {
                }
                else
                {
                    temp=Prod.iT6PosCate[iBin];
                    if(temp<0 || temp>=eTrayCount ||
                       Prod.iTrayType[temp]==tNotUse)
                        continue;
                }

                iCount++;

                if(IniConfig.bSPILFunction==true && LotSummary.iTotalCategory[iBin]<=0)
                    continue;

                if(Prod.bIsPassBin[iBin]==1 || iBin==iTestBinCount)
                {
                    iFailCount+=LotSummary.iTotalCategory[iBin];
                }
                else
                {
                    iPassCount+=LotSummary.iTotalCategory[iBin];
                }
            }
        }
        else
        {
            for(int iBin=0; iBin<iTestBinCount+1; iBin++)
            {
                if(iBin==iTestBinCount)
                {
                }
                else
                {
                    temp=Prod.iT6PosCate[iBin];
                    if(temp<0 || temp>=eTrayCount ||
                       Prod.iTrayType[temp]==tNotUse)
                        continue;
                }

                iCount++;

                if(IniConfig.bSPILFunction==true &&
                   LotSummary.iByLotTotalCategory[k][iBin]<=0)
                    continue;

                if(Prod.bIsPassBin[iBin]==1 || iBin==iTestBinCount)
                {
                    iFailCount+=LotSummary.iByLotTotalCategory[k][iBin];
                }
                else
                {
                    iPassCount+=LotSummary.iByLotTotalCategory[k][iBin];
                }
            }
        }

        Str=" PASS";

        Str2.sprintf("%d", iPassCount);
        while(Str2.Length()<6)
        {
            Str2=" "+Str2;
        }
        Str=Str+Str2;

        Str2=ChangeToPercentage(iPassCount, iUnloadCount);
        while(Str2.Length()<9)
        {
            Str2=" "+Str2;
        }
        Str=Str+Str2;

        sList->Add(Str);

        Str=" FAIL";

        Str2.sprintf("%d", iFailCount);
        while(Str2.Length()<6)
        {
            Str2=" "+Str2;
        }
        Str=Str+Str2;

        Str2=ChangeToPercentage(iFailCount, iUnloadCount);
        while(Str2.Length()<9)
        {
            Str2=" "+Str2;
        }
        Str=Str+Str2;

        sList->Add(Str);
        sList->Add("");
        sList->Add(" ========================================================================");
        sList->Add(" ==========================  Error Bin Summary  =========================");
        sList->Add(" 991(Barcode Read Error)");
        sList->Add(" 992(Duplicate 2DID error)");
        sList->Add(" 993(2DID not existed in list)");
        sList->Add(" 994(Not defined)");
        sList->Add(" ========================================================================");

        if(k==fSCKART->iInfo_MultiLotCnt)
        {
            LotSummary.iTotalCategory[iTestBinCount];

            Str2.sprintf(" 991: %d", iE1Count);
            sList->Add(Str2);
            Str2.sprintf(" 992: %d", iE2Count);
            sList->Add(Str2);
            Str2.sprintf(" 993: %d", iE3Count);
            sList->Add(Str2);
            Str2.sprintf(" 994: %d", LotSummary.iTotalCategory[iTestBinCount]-iE1Count-iE2Count-iE3Count);
            sList->Add(Str2);
        }
        else
        {
            LotSummary.iByLotTotalCategory[k][iTestBinCount];

            Str2.sprintf(" 991: %d", 0);
            sList->Add(Str2);
            Str2.sprintf(" 992: %d", 0);
            sList->Add(Str2);
            Str2.sprintf(" 993: %d", 0);
            sList->Add(Str2);
            Str2.sprintf(" 994: %d", 0);
            sList->Add(Str2);
        }

        sList->Add(" ========================================================================");

        if(CosFunction.bUseTSVFunction)                                         //Steven 20240904 : for ATK的TSV功能
        {
            if(srvrscktTSV->Active)
                srvrscktTSV->Close();
            srvrscktTSV->Port=IniConfig.iN09_TSV_Port;
            srvrscktTSV->Open();

            if(FileExists(PathName2+FileName)==false)                           //Steven 20230317 : 避免連續存兩次把資料蓋掉了
            {
                if(IniConfig.bN09_LotCountAutoFunc)
                {
                    if(fSCKART->iNeedRT==0)
                    {
                        if(IniConfig.iN09_4_UploadMethod==0)
                        {
                            sList->SaveToFile(PathName2+FileName);              //Steven 20230215 : 存檔路徑加上年月
                            FTP_Upload(PathName2, IniConfig.sN09_5_Path, FileName);
                        }
                        else
                        {
                            sList->SaveToFile(PathName+FileName);
                        }
                    }
                }
                else
                {
                    if(IniConfig.bSPILFunction==true)                               //JerryYang 20220923 : unload數量為0不要存log
                    {
                        if(iUnloadCount>0)
                            sList->SaveToFile(PathName2+FileName);
                    }
                    else
                    {
                        sList->SaveToFile(PathName2+FileName);
                    }
                }
                fObserver->memoLotSummary->Lines=sList;
            }
        }
        else
        {
            if(FileExists(PathName2+FileName)==false)                           //Steven 20230317 : 避免連續存兩次把資料蓋掉了
            {
                if(IniConfig.bSPILFunction==true)                               //JerryYang 20220923 : unload數量為0不要存log
                {
                    if(iUnloadCount>0)
                        sList->SaveToFile(PathName2+FileName);
                }
                else
                {
                    sList->SaveToFile(PathName2+FileName);
                }
                fObserver->memoLotSummary->Lines=sList;
            }
        }
        sList->Clear();
        delete sList;

        if(IniConfig.bN17UploadLotSummary)                                      //JerryYang 20220923 : Upload lot summary
        {
            if(DirectoryExists(IniConfig.asN17LotSummaryPath))
            {
                if(IniConfig.bA38_SLT_Summary && fSCKART->sLotID!="NA" && fSCKART->sLotID!="" && iUnloadCount>0)
                {
                    strFileName=PathName2+FileName;                             //Steven 20230215 : 存檔路徑加上年月
                    if(FileExists(strFileName))
                    {
                        strPath.sprintf("%s\\%s", IniConfig.asN17LotSummaryPath, FileName);
                        bResult=CopyFile(strFileName.c_str(), strPath.c_str(), true);
                        if(bResult==false)
                        {
                            ShowMyMessage("Uploaded lot summary error\r\nPlease check the path of N-17.","上傳lot summary失敗\r\n請檢查N-17路徑是否存在");
                        }
                        else
                        {
                            RecordProcess("Uploaded lot summary successfully.");//Steven 20190722 : add TSV log
                        }
                    }
                }
                else
                {
                    if(fSCKART->sInfo_CustLotID=="NA" || fSCKART->sInfo_CurrQty=="NA" ||
                       fSCKART->sInfo_CustLotID=="" || fSCKART->sInfo_CurrQty=="")
                    {
                    }
                    else
                    {
                        strFileName=PathName2+FileName;                         //Steven 20230215 : 存檔路徑加上年月
                        if(FileExists(strFileName))
                        {
                            strPath.sprintf("%s\\%s", IniConfig.asN17LotSummaryPath, FileName);
                            bResult=CopyFile(strFileName.c_str(), strPath.c_str(), true);
                            if(bResult==false)
                            {
                                ShowMyMessage("Uploaded lot summary error\r\nPlease check the path of N-17.","上傳lot summary失敗\r\n請檢查N-17路徑是否存在");
                            }
                            else
                            {
                                RecordProcess("Uploaded lot summary successfully.");   //Steven 20190722 : add TSV log
                            }
                        }
                    }
                }
            }
            else
            {
                ShowMyMessage("Uploaded lot summary error\r\nPlease check the path of N-17.","上傳lot summary失敗\r\n請檢查N-17路徑是否存在");
            }
        }

        if(IniConfig.bA38_SLT_Summary && FileName!="" && iUnloadCount>0)
        {
            ShellExecute(NULL, NULL, strFileName.c_str(), NULL,NULL,SW_SHOW);
        }

        if(CosFunction.bSortingBy2DList==true &&
           LastSet.iTester==_2D_SORT &&
           TestIF_File.bSortingBy2DIDList==true)                                //Frank 20221122 : 2DID sorting for ATK
        {
            Str2.sprintf("%s\\SortBy2DID_%s.csv", "D:\\HT9045_Log\\2D_SortList", fLotInfo->edtSysLotID->Text);
            str3.sprintf("%s\\SortBy2DID_%s.csv", asBackup2DSortListPath, fLotInfo->edtSysLotID->Text);
            if(FileExists(Str2))                                                //Steven 20160505 : 加上保護, 不然開程式會跳Error
            {
                CopyFile(Str2.c_str(), str3.c_str(), true);                     //複製到Backup資料夾
                DeleteFile(Str2);
            }
            Str2=PathName2+FileName;
            str3=asBackup2DSummaryPath+FileName;
            if(FileExists(Str2))                                                //Steven 20160505 : 加上保護, 不然開程式會跳Error
            {
                CopyFile(Str2.c_str(), str3.c_str(), true);                     //複製到Backup資料夾
            }
        }
    }
    LotSummary.ClearAllData();
}
//------------------------------------------------------------------------------
void TfSCKART::SaveTestSummaryTSV(int iSaveData)                                //Steven 20190521 : ATK lot count
{
    AnsiString FileName, PathName, PathName2, Str, Str1, Str2, IP, tmps1;
    int iCount=0, iUnloadCount=0;
    bool bGotIp=false;
    map<AnsiString, AnsiString>mapIPList;
    map<AnsiString, AnsiString>::iterator mapIPIter;
    AnsiString Data="", asFileName;
    int temp;
    AnsiString sLotID1=fSCKART->sLotID;

    if(sLotID1=="" && fLotInfo->edtSysLotID->Text!="")                          //Steven 20230317 : 避免lot ID是空值
        sLotID1=fLotInfo->edtSysLotID->Text;
    if(sLotID1=="")                                                             //Steven 20230814 : 沒有Lot ID, 就帶入日期時間
        sLotID1.sprintf("%04d%02d%02d%02d%02d%02d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);

    if(CosFunction.bUseTSVFunction)                                             //Steven 20240904 : for ATK的TSV功能
    {
        PathName.sprintf("%s\\%04d\\%02d\\", IniConfig.sN09_HandlerFolder, SystemYear, SystemMonth);        //Steven 20230215 : 存檔路徑加上年月
    }
    else
    {
        PathName.sprintf("D:\\HT9045_Log\\TestSummary\\%04d\\%02d\\", SystemYear, SystemMonth);
    }
    MyForceDirectories(PathName, "TfSCKART::SaveTestSummaryTSV_1");

    PathName2.sprintf("%s\\%04d\\%02d\\", asSummaryPath, SystemYear, SystemMonth);    //Steven 20230215 : 存檔路徑加上年月
    MyForceDirectories(PathName2, "TfSCKART::SaveTestSummaryTSV_2");

    if(sProcessCode=="")
        sProcessCode="FT1";

    //<Lot ID>_<Operation>_<Retest Info>_<YYYYMMDDHHMM>.txt
    if(iFTRTCount==1)
        FileName.sprintf("%s_%s_FT_%04d%02d%02d%02d%02d.txt", sLotID1, sProcessCode, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin);
    else
        FileName.sprintf("%s_%s_RT%d_%04d%02d%02d%02d%02d.txt", sLotID1, sProcessCode, iFTRTCount-1, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin);

    for(int iBin=0; iBin<iTestBinCount; iBin++)
    {
        for(int i=0; i<MAX_SOCKET_ROW*MAX_SOCKET_COL; i++)
        {
            if(iNeedRT!=0 && LotSummary.bIsRTBin[iBin]==true)
            {
                LotSummary.iCountCategory[i][iBin]=0;
                LotSummary.iTotalCategory[iBin]=0;
            }
        }
    }

    if(iSaveData==0)
        return;

    TStringList *sIPList=new TStringList();
    TStringList *sList=new TStringList();
    TStringList *sTotalList=new TStringList();
    TStringList *sListTemp=new TStringList();

//Lot_id:       0000C912X8Z.0100#
//Handler ID:   H-xxxx
//Handler IP:   255.255.255.255
//DCC:          NONE
//TEST_OPR:     FT1
//RT_CODE:      1
//TEMP:         25
//Hostname:     k3tv9369
//Total_Inqty:  86
//
// ========================================================================
// =========================  Hard Bin Summary  ===========================
// ========================================================================
// Hard P/F  Site1 Site2 Site3 Site4 Site5 Site6 Site7 Site8 Total  Yield
// ---- ---- ----- ----- ----- ----- ----- ----- ----- ----- ----- --------
//    1 PASS     0     0     0     0    40    41     0     0    81  94.186%
//    5 FAIL     0     0     0     0     0     2     0     0     2   2.326%
//    6 FAIL     0     0     0     0     3     0     0     0     3   3.488%
// ========================================================================
    if(CUSTOMER_CODE==CC_ASE_CL)
    {
        Str.sprintf("Lot_ID:%s", fLotInfo->edtASECL_LotID->Text);
    }
    else
    {
        Str.sprintf("Lot_id:\t\t%s", sLotID1);
    }
    sList->Add(Str);
    Str.sprintf("Handler ID: %s", IniConfig.sGPIBMachineID);
    sList->Add(Str);

    if(CosFunction.bUseTSVFunction)                                             //Steven 20240904 : for ATK的TSV功能
    {
        sIPList->CommaText=IniConfig.sN09_7_SkipIP;
        mapIPList.clear();
        for(int j=0; j<sIPList->Count; j++)
        {
            mapIPList[sIPList->Strings[j]]=j;
        }

        for(int i=0; i<fConfiguration->mmoN04_IP->Lines->Count; i++)
        {
            Str1=fConfiguration->mmoN04_IP->Lines->Strings[i];
            Str2=Str1.SubString(1, Str1.AnsiPos(".")-1);
            if(bGotIp==false && Str1!="172.16.8.100")
            {
                mapIPIter=mapIPList.find(Str2);
                if(mapIPList.size()!=0 && mapIPIter==mapIPList.end())
                {
                    IP=Str1;
                    bGotIp=true;
                }
            }
        }

        if(bGotIp)
        {
            Str.sprintf("Handler IP: %s", IP);
        }
        else
        {
            Str.sprintf("Handler IP: ");
        }
    }

    sList->Add(Str);
    sList->Add("DCC:\t\tNONE");
    Str.sprintf("TEST_OPR:\t%s", sProcessCode);
    sList->Add(Str);

    if(iFTRTCount==1)
        Str.sprintf("RT_CODE:\tFT");
    else
        Str.sprintf("RT_CODE:\tRT%d", iFTRTCount-1);
    sList->Add(Str);

    if(LastSet.iTemperature==Tempture_Ambient)
        Str.sprintf("TEMP:\t\t25");
    else
        Str.sprintf("TEMP:\t\t%f", Temperature.fWorkTemperBase);
    sList->Add(Str);
    Str.sprintf("Hostname:\t");
    sList->Add(Str);

    if(CUSTOMER_CODE==CC_ASE_CL)
    {
        Str.sprintf("Load_qty:\t%d", LotSummary.iLoadTotal);  //JerryYang 20240423 : fix
    }
    else
    {
        Str.sprintf("Total_Inqty:\t%d", iLotCount);
    }
    sList->Add(Str);

    sList->Add("");
    sList->Add(" ========================================================================");
    sList->Add(" =========================  Hard Bin Summary  ===========================");
    sList->Add(" ========================================================================");

    iCount=0;
    Str=" Hard P/F ";
    Str2=" ---- ----";
    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            iCount++;
            Str1.sprintf(" Site%d", iCount);
            Str=Str+Str1;
            Str2=Str2+" -----";
        }
    }
    Str=Str+" Total  Yield";
    sList->Add(Str);
    Str2=Str2+" ----- --------";
    sList->Add(Str2);

    for(int iBin=0; iBin<iTestBinCount; iBin++)
    {
        temp=Prod.iT6CatData[iBin];
        if(IniConfig.bSPILFunction==true && (temp<0 || temp>=eTrayCount))
            continue;

        Str.sprintf("%d", iBin);
        while(Str.Length()<5)
        {
            Str=" "+Str;
        }

        if(Prod.bIsPassBin[iBin])
            Str=Str+" PASS";
        else
            Str=Str+" FAIL";

        iCount=0;
        for(int i=0; i<TestSocket.iShtRow; i++)
        {
            for(int j=0; j<TestSocket.iShtCol; j++)
            {
                Str2.sprintf("%d", LotSummary.iCountCategory[iCount][iBin]);
                while(Str2.Length()<6)
                {
                    Str2=" "+Str2;
                }
                Str=Str+Str2;
                iCount++;
            }
        }

        if(IniConfig.bSPILFunction==true && LotSummary.iTotalCategory[iBin]<=0)
            continue;

        Str2.sprintf("%d", LotSummary.iTotalCategory[iBin]);
        while(Str2.Length()<6)
        {
            Str2=" "+Str2;
        }
        Str=Str+Str2;

        if(IniConfig.bSPILFunction==true)                                   //JerryYang 20220923 : unload數量為0不要存log
        {
            Str2=ChangeToPercentage(LotSummary.iTotalCategory[iBin], iUnloadCount);
        }
        else
        {
            Str2=ChangeToPercentage(LotSummary.iTotalCategory[iBin], iLotCount);
        }

        while(Str2.Length()<9)
        {
            Str2=" "+Str2;
        }
        Str=Str+Str2;

        sList->Add(Str);
    }

    sList->Add(" ========================================================================");

    if(CosFunction.bUseTSVFunction)                                             //Steven 20240904 : for ATK的TSV功能
    {
        if(srvrscktTSV->Active)
            srvrscktTSV->Close();
        srvrscktTSV->Port=IniConfig.iN09_TSV_Port;
        srvrscktTSV->Open();

        if(fSCKART->iNeedRT==0)
        {
            if(IniConfig.iN09_4_UploadMethod==0)
            {
                sList->SaveToFile(PathName2+FileName);
                FTP_Upload(PathName2, IniConfig.sN09_5_Path, FileName);
            }
            else
            {
                sList->SaveToFile(IniConfig.sN09_HandlerFolder+"\\"+FileName);
            }
        }

        if(FileExists(PathName2+FileName)==false)                               //Steven 20230317 : 避免連續存兩次把資料蓋掉了
        {
            if(fSCKART->iNeedRT==0)
            {
                if(IniConfig.iN09_4_UploadMethod==0)
                {
                    sList->SaveToFile(PathName2+FileName);                      //Steven 20230215 : 存檔路徑加上年月
                    FTP_Upload(PathName2, IniConfig.sN09_5_Path, FileName);
                }
                else
                {
                    sList->SaveToFile(PathName+FileName);
                }
            }
            fObserver->memoLotSummary->Lines=sList;
        }
    }
    else
    {
        sList->SaveToFile(PathName2+FileName);
        fObserver->memoLotSummary->Lines=sList;
    }

    if(CUSTOMER_CODE==CC_ASE_CL && USE_TRAY_MAPPING==1 && TestIF_File.bEnableTrayID2 && LastSet.iTester==ON_LINE)
    {
        asFileName.sprintf("%s\\%s@WAFERLOT@%sXX@99@SWBIN@%s.txt", asTravelingLogPath, fLotInfo->edtASECL_LotID->Text, fLotInfo->edInsertion->Text, fLotInfo->edFlowID->Text);     //Steven 20170123 (Jou) : 修改檔案命名格式
        sTotalList->LoadFromFile("D:\\HT9045_Log\\Summary\\"+FileName);
        sTotalList->Add("JHT_LOG_START");

        if(FileExists(asFileName))
        {
            sListTemp->LoadFromFile(asFileName);
            for(int i=0; i<sListTemp->Count; i++)
            {
                sTotalList->Add(sListTemp->Strings[i]);
            }

            sTotalList->SaveToFile(asFileName);
        }
        else
        {
            ShowMyMessage("SaveJHTLog fail!");
        }
    }

    sList->Clear();
    mapIPList.clear();
    sIPList->Clear();
    sListTemp->Clear();
    sTotalList->Clear();
    delete sIPList;
    delete sList;
    delete sTotalList;
    delete sListTemp;

    if(CosFunction.bUseTSVFunction)                                             //Steven 20240904 : for ATK的TSV功能
    {
        bWaitTSV=true;
        bShowTSVMsg=false;
        sTSVMsg="";
        RecordProcess("Lot end and send summary and waiting TSV reply.");       //Steven 20190722 : add TSV log
        TSVdelay.SetSecAndOn(IniConfig.dN09_SearchTime);
        TimerTSV->Enabled=true;
    }
    LotSummary.ClearAllData();
}
//------------------------------------------------------------------------------
void TfSCKART::SaveSummaryTrayFeed()
{
    int iSiteCh=0;
    int iTotalCh=TestSocket.iShtRow*TestSocket.iShtCol;
    int iSiteTotalCt=0;
    int iSitePassCt=0;
    int iSiteFailCt=0;
    AnsiString sPassYield;
    AnsiString sFailYield;
    AnsiString asFileName, asFolderName;
    AnsiString sTemp, str, s, sTotal, sTempT, sTempP, sTempF, sP, sF;
    TStringList *redtSummary=new TStringList;
    redtSummary->Clear();
    TastCategory.UpdataCount(false);                                            //Steven 20250514 : 統一計算數量

    if(RunInfo.LotStartTime=="2020-01-01 00:00:00")
        sPassYield.sprintf("%04d-%02d-%02d %02d:%02d:%02d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
    else
        sPassYield=RunInfo.LotStartTime;

    if(RunInfo.LotEndTime=="")
        sFailYield.sprintf("%04d-%02d-%02d %02d:%02d:%02d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
    else
        sFailYield=RunInfo.LotEndTime;

    if(fLotInfo->lbledtCustomer->Text=="")
        sTemp=" ";
    else
        sTemp=fLotInfo->lbledtCustomer->Text;

    if(fLotInfo->edtSysOperatorID->Text=="")
        sTotal=" ";
    else
        sTotal=fLotInfo->edtSysOperatorID->Text;

    redtSummary->Add(str.sprintf("============================ SUMMARY REPORT ============================"));
    redtSummary->Add(str.sprintf(" "                                     ));
    redtSummary->Add(str.sprintf("%-34s %s",  AnsiString("Lot#:"),       RunInfo.LotNo));
    redtSummary->Add(str.sprintf("%-34s %s",  AnsiString("Start:"),      sPassYield));
    redtSummary->Add(str.sprintf("%-34s %s",  AnsiString("End:"),        sFailYield));
    redtSummary->Add(str.sprintf("%-34s %s",  AnsiString("Customer:"),   sTemp));
    redtSummary->Add(str.sprintf("%-34s %s",  AnsiString("Program:"),    fMain->cbSetupFileName->Text));
//    SummaryHead->Add(str.sprintf("%-34s ",    AnsiString("LoadBoard ID:")));
    redtSummary->Add(str.sprintf("%-34s %s",  AnsiString("Operator ID:"),sTotal));
    redtSummary->Add(str.sprintf("%-34s %s",  AnsiString("Machine ID:"), IniConfig.SocketHandlerID));
    redtSummary->Add(str.sprintf("%-34s %d",  AnsiString("Input:"),      TastCategory.iTotalSocket));
    redtSummary->Add(str.sprintf("%-34s %d",  AnsiString("Pass:"),       TastCategory.iPassSocket));
    redtSummary->Add(str.sprintf("%-34s %d",  AnsiString("Fail:"),       TastCategory.iFailSocket));
    redtSummary->Add(str.sprintf("%-34s %d",  AnsiString("Reject:"),     TastCategory.iRejectCount));
    redtSummary->Add(str.sprintf("%-34s %s",  AnsiString("Yield:"),      ChangeToPercentage(TastCategory.iPassSocket, TastCategory.iTotalSocket)));
    redtSummary->Add(str.sprintf(" "                                     ));

    redtSummary->Add(str.sprintf("============================= BY TRAY COUNT ============================"));
    redtSummary->Add(str.sprintf(" "                                     ));

    for(int i=0; i<eTrayCount; i++)
    {
        if(Prod.iTrayType[i]!=tNotUse)
        {
            sTemp.sprintf("%-34s %d", s6TrayName[i]+AnsiString(":"), TastCategory.iUnloadCnt[i]);
            redtSummary->Add(sTemp);
        }
    }

    redtSummary->Add(str.sprintf(" "                                     ));
    redtSummary->Add(str.sprintf("============================= BY SITE COUNT ============================"));
    redtSummary->Add(str.sprintf(" "                                     ));

    str.sprintf("%-34s", "Total Tested DUT Count:");                            //Total Tested DUT Count: DUT1 DUT2 DUT3 DUT4 SUM
    for(int i=0; i<iTotalCh; i++)
    {
        s.sprintf("DUT%d", i+1);
        sTemp.sprintf(" %-20s", s);
        str+=sTemp;
    }
    sTemp.sprintf(" %-20s", "SUM");
    str+=sTemp;
    redtSummary->Add(str);

    TStringList *sListTotal=new TStringList();
    TStringList *sListPass =new TStringList();
    TStringList *sListFail =new TStringList();
    sListTotal->Clear();
    sListPass->Clear();
    sListFail->Clear();
    for(int i=0; i<TestSocket.iMaxRow; i++)
    {
        for(int j=0; j<TestSocket.iMaxCol; j++)
        {
            sListTotal->Add("0");
            sListPass->Add("0(0.00\%)");
            sListFail->Add("0(0.00\%)");
        }
    }

    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            if(TestIF.iSiteMap[i][j]>0)
            {
                iSiteCh=TestIF.iSiteMap[i][j]-1;
                iSiteTotalCt=TastCategory.iBySiteTotal[iSiteCh];
                iSitePassCt =TastCategory.iBySitePass[iSiteCh];
                iSiteFailCt =TastCategory.iBySiteFail[iSiteCh];

                if(iSiteTotalCt>0)
                {
                    sPassYield.sprintf("%d(%s)", iSitePassCt, ChangeToPercentage(iSitePassCt, iSiteTotalCt));
                    sFailYield.sprintf("%d(%s)", iSiteFailCt, ChangeToPercentage(iSiteFailCt, iSiteTotalCt));
                    sListTotal->Strings[iSiteCh]=AnsiString().sprintf("%d", iSiteTotalCt);
                    sListPass->Strings[iSiteCh] =AnsiString().sprintf("%s", sPassYield);
                    sListFail->Strings[iSiteCh] =AnsiString().sprintf("%s", sFailYield);
                }
            }
        }
    }

    sTotal.sprintf("%-34s", "Total");                                           //Total DUT1 DUT2 DUT3 DUT4 SUM
    sPassYield.sprintf("%-34s", "PASS");                                        //PASS DUT1 DUT2 DUT3 DUT4 SUM
    sFailYield.sprintf("%-34s", "FAIL");                                        //FAIL DUT1 DUT2 DUT3 DUT4 SUM

    for(int i=0; i<iTotalCh; i++)
    {
        sTempT.sprintf(" %-20s", sListTotal->Strings[i]);
        sTempP.sprintf(" %-20s", sListPass->Strings[i]);
        sTempF.sprintf(" %-20s", sListFail->Strings[i]);

        sTotal+=sTempT;
        sPassYield+=sTempP;
        sFailYield+=sTempF;
    }

    sP.sprintf("%d(%s)", TastCategory.iPassSocket, ChangeToPercentage(TastCategory.iPassSocket, TastCategory.iTotalSocket));
    sF.sprintf("%d(%s)", TastCategory.iFailSocket, ChangeToPercentage(TastCategory.iFailSocket, TastCategory.iTotalSocket));
    sTempT.sprintf(" %-20d", TastCategory.iTotalSocket);
    sTempP.sprintf(" %-20s", sP);
    sTempF.sprintf(" %-20s", sF);

    sTotal+=sTempT;
    sPassYield+=sTempP;
    sFailYield+=sTempF;

    redtSummary->Add(sTotal);
    redtSummary->Add(sPassYield);
    redtSummary->Add(sFailYield);
    redtSummary->Add(str.sprintf(" "                                     ));
    redtSummary->Add(str.sprintf("============================= BY BIN COUNT ============================="));
    redtSummary->Add(str.sprintf(" "                                     ));

    str.sprintf("%-34s", "HW BIN Count:");                                      //HW BIN Count: DUT1 DUT2 DUT3 DUT4 SUM
    for(int i=0; i<iTotalCh; i++)
    {
        s.sprintf("DUT%d", i+1);
        sTemp.sprintf(" %-20s", s);
        str+=sTemp;
    }
    sTemp.sprintf(" %-20s", "SUM");
    str+=sTemp;
    redtSummary->Add(str);

    for(int iCat=0; iCat<iTestBinCount; iCat++)
    {
        s.sprintf("BIN %d", iCat);
        sTotal.sprintf("%-34s", s);
        for(int iDut=0; iDut<iTotalCh; iDut++)
        {
            sPassYield.sprintf("%d(%s)", TastCategory.iBySiteCate[iDut][iCat], ChangeToPercentage(TastCategory.iBySiteCate[iDut][iCat], TastCategory.iTotalCategory[iCat]));
            sTempT.sprintf(" %-20s", sPassYield);
            sTotal+=sTempT;
        }

        sPassYield.sprintf("%d(%s)", TastCategory.iTotalCategory[iCat], ChangeToPercentage(TastCategory.iTotalCategory[iCat], TastCategory.iTotalSocket));
        sTempT.sprintf(" %-20s", sPassYield);
        sTotal+=sTempT;
        redtSummary->Add(sTotal);
    }

    s.sprintf("REJECT");
    sTotal.sprintf("%-34s", s);
    for(int iDut=0; iDut<iTotalCh; iDut++)
    {
        sPassYield.sprintf("%d(%s)", TastCategory.iBySiteCate[iDut][iTestBinCount], ChangeToPercentage(TastCategory.iBySiteCate[iDut][iTestBinCount], TastCategory.iTotalCategory[iTestBinCount]));
        sTempT.sprintf(" %-20s", sPassYield);
        sTotal+=sTempT;
    }

    sPassYield.sprintf("%d(%s)", TastCategory.iTotalCategory[iTestBinCount], ChangeToPercentage(TastCategory.iTotalCategory[iTestBinCount], TastCategory.iTotalSocket));
    sTempT.sprintf(" %-20s", sPassYield);
    sTotal+=sTempT;
    redtSummary->Add(sTotal);

    asFolderName.sprintf("D:\\HT9045_Log\\Summary_Lot\\%04d%02d", SystemYear, SystemMonth);
    MyForceDirectories(asFolderName, "ProcessOSPrint");

    asFileName.sprintf("%s\\%s %04d%02d%02d-%02d%02d%02d %s Summary.txt",       asFolderName,
                                                                                IniConfig.SocketHandlerID,
                                                                                SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec,
                                                                                RunInfo.LotNo);

    redtSummary->SaveToFile(asFileName);
    redtSummary->Clear();
    delete redtSummary;

    RunInfo.SaveJamRateByLot();                                                 //Steven 20200415 : SCC要By Lot Jam Rate

    if(IniConfig.bVTESTFunction==false)
    {
        if(IniConfig.bN10_UploadSummaryToFTP==true)                             //Steven 20230216 : 調整位置
        {
            if(IniConfig.iN10UploadMethod==0)
            {
                if(FileExists(asFileName)==true)
                    FormHS->UpDataToServerByFTP(ExtractFilePath(asFileName), ExtractFileName(asFileName), "SLT_Report");

                if(FileExists(slEventLog->sLotFileName))                        //Steven 20250812 : 修正上傳檔名
                {
                    FormHS->UpDataToServerByFTP(ExtractFilePath(slEventLog->sLotFileName), ExtractFileName(slEventLog->sLotFileName), "EventLog");
                }
            }
            else
            {
                asFolderName.sprintf("%sSummaryByLot\\%04d", IncludeTrailingPathDelimiter(IniConfig.sN10UploadDrivePath), SystemYear);
                if(MyForceDirectories(asFolderName, "[N10] Upload_SummaryByLot")!=1)
                {
                    ;
                }
                else
                {
                    str.sprintf("XCOPY /y/a/e/c/i/h/f/r \"%s\" \"%s\"", asFileName, asFolderName);
                    slExe->Add(str);

                    if(FileExists(slEventLog->sLotFileName))                    //Steven 20250812 : 修正上傳檔名
                    {
                        asFolderName.sprintf("%sEventLotByLot\\%04d", IncludeTrailingPathDelimiter(IniConfig.sN10UploadDrivePath), SystemYear);
                        if(MyForceDirectories(asFolderName, "[N10] Upload_SummaryByLot")!=1)
                        {
                            ;
                        }
                        str.sprintf("XCOPY /y/a/e/c/i/h/f/r \"%s\" \"%s\"", slEventLog->sLotFileName, asFolderName);
                        slExe->Add(str);
                    }

                    try
                    {
                        slExe->SaveToFile("D:\\HT9045_Log\\SummaryByLot.bat");
                    }
                    catch(...)
                    {
                        MyDBIProcess("Exception", "SaveSummaryTrayFeed");
                    }

                    for(int i=0; i<slExe->Count; i++)
                    {
                        RecordProcess(slExe->Strings[i]);
                    }

                    try
                    {
                        ExecZipCommand("D:\\HT9045_Log\\SummaryByLot.bat", " ");
                    }
                    catch(...)
                    {
                        MyDBIProcess("Exception", "SaveSummaryTrayFeed ExecZipCommand");
                    }

                    slExe->Clear();
                }
            }
        }
    }
}
//------------------------------------------------------------------------------
void TfSCKART::Save2DSortingSummary(int iSaveData)                              //Steven 20240830 : 2D sort獨立出來
{
    AnsiString FileName, PathName, PathName2, Str, Str1, Str2, IP, tmps1, strPath, strFileName="", str2, str3="";
    AnsiString sTestBinNo=StringReplace(fSCKART->sInfo_TestBinNo, ":", "-", TReplaceFlags()<<rfReplaceAll);
    int iCount=0, iUnloadCount=0;
    bool bResult=false;
    map<AnsiString, AnsiString>mapIPList;
    map<AnsiString, AnsiString>::iterator mapIPIter;
    AnsiString Data="";
    AnsiString sLotID1=fSCKART->sLotID;

    if(sLotID1=="" && fLotInfo->edtSysLotID->Text!="")                          //Steven 20230317 : 避免lot ID是空值
        sLotID1=fLotInfo->edtSysLotID->Text;
    if(sLotID1=="")                                                             //Steven 20230814 : 沒有Lot ID, 就帶入日期時間
        sLotID1.sprintf("%04d%02d%02d%02d%02d%02d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);

    bool bHaveBinData=false;
    int temp;
    AnsiString aUnloader[eTrayCount]={"Auto1-", "Auto2-", "Auto3-", "Auto4-", "Auto5-", "Auto6-",
                                      "Fix1-", "Fix2-", "Fix3-", "Fix4-", "Fix5-", "Fix6-", "Fix7-", "Fix8-", "Fix9-", "Fix10-", "Fix11-", "Fix12-",
                                      "BulkBox-",
                                      "Mag1-", "Mag2" , "Mag3", "Mag4", "Mag5", "Mag6", "Mag7", "Mag8", "Mag9", "Mag10", "Mag11", "Mag12", "Mag13", "Mag14"};
    int iUnloadCnt[eTrayCount]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                0, 0};
    GetTimeInfo();

    fSCKART->sLotEndTime.sprintf("%04d%02d%02d%02d%02d%02d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
    if(CosFunction.bUseTSVFunction)                                             //Steven 20240904 : for ATK的TSV功能
    {
        if(IniConfig.bN09_LotCountAutoFunc)
        {
            PathName.sprintf("%s\\%04d\\%02d\\", IniConfig.sN09_HandlerFolder, SystemYear, SystemMonth);        //Steven 20230215 : 存檔路徑加上年月
            MyForceDirectories(PathName, "TfSCKART::Save2DSortingSummary_1");
        }
        else
        {
            PathName.sprintf("%s\\%04d\\%02d\\", asSummaryPath, SystemYear, SystemMonth);
            MyForceDirectories(PathName, "TfSCKART::Save2DSortingSummary_2");
        }
    }
    else
    {
        PathName.sprintf("%s\\%04d\\%02d\\", asSummaryPath, SystemYear, SystemMonth);
        MyForceDirectories(PathName, "TfSCKART::Save2DSortingSummary_3");
    }

    if(IniConfig.bN25FolderWithoutYYMM==true)                                   //JerryYang 20230822 : 佳熹要求路徑不要加年月日
    {
        PathName2.sprintf("%s\\", asSummaryPath);                               //Steven 20230215 : 存檔路徑加上年月
        MyForceDirectories(PathName2, "TfSCKART::Save2DSortingSummary_4");
    }
    else
    {
        PathName2.sprintf("%s\\%04d\\%02d\\", asSummaryPath, SystemYear, SystemMonth);    //Steven 20230215 : 存檔路徑加上年月
        MyForceDirectories(PathName2, "TfSCKART::Save2DSortingSummary_5");
    }

    if(sProcessCode=="")
        sProcessCode="FT1";

    if(IniConfig.bN23UseLotInfoFile)
    {
        FileName.sprintf("%s_%s_%s_%04d%02d%02d%02d%02d.txt", fLotInfo->edtSysLotID->Text, fLotInfo->edtCusLotID->Text, fLotInfo->edtCusDevGrp->Text,
                                                              SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin);  //JerryYang 20230822 : 修改檔名
    }
    else if(IniConfig.bSPILFunction==true &&    //JerryYang 20260201 : modify log
            IniConfig.bA66_2D_Sort==true)
    {
        if(MachineTypeChoice==Type_HT9045 ||
           MachineTypeChoice==Type_HT9045_12Site)
            fSCKART->sInfo_ProgramName="HT9045";
        else if(MachineTypeChoice==Type_HT9046)
            fSCKART->sInfo_ProgramName="HT9046";
        else if(MachineTypeChoice==Type_HT9046_LS)
            fSCKART->sInfo_ProgramName="HT9046LS";

        fSCKART->sInfo_ProgramName="HT9046LS";
        FileName.sprintf("%s_%s_%s_%s_%s_%s_ALL_%04d%02d%02d%02d%02d%02d.txt", fSCKART->sInfo_Stage, fSCKART->sInfo_Step, fSCKART->sInfo_InnerLotID,
                                                              fLotInfo->edtCusLotID->Text, fLotInfo->edtCusDevGrp->Text, fLotInfo->edtCusStep->Text,
                                                              SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);  //JerryYang 20230822 : 修改檔名
    }
    else if(bReadLotInfoFromART==true)
    {
        FileName.sprintf("%s_%s_%s_%04d%02d%02d%02d%02d.txt", fSCKART->sLotID, fSCKART->sInfo_CustLotID, fSCKART->sInfo_CustDevGup,
                                                              SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin);  //JerryYang 20230822 : 修改檔名
    }
    else
    {
        if(MachineTypeChoice==Type_HT9045 ||
           MachineTypeChoice==Type_HT9045_12Site)                               //JerryYang 20230322 : SPIL 2D SORT summary修改
            fSCKART->sInfo_ProgramName="HT9045";
        else if(MachineTypeChoice==Type_HT9046)
            fSCKART->sInfo_ProgramName="HT9046";
        else if(MachineTypeChoice==Type_HT9046_LS)
            fSCKART->sInfo_ProgramName="HT9046LS";

        fSCKART->sInfo_ProgramName="HT9046LS";
        FileName.sprintf("%s_%s_%s_%04d%02d%02d%02d%02d.txt", fLotInfo->edtSysLotID->Text, fLotInfo->edtCusLotID->Text, fLotInfo->edtCusDevGrp->Text,
                                                              SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin);  //JerryYang 20230822 : 修改檔名
    }

    if(CosFunction.bART_SECSGEM_93K==false)                                     //JerryYang 20220927 : SPIL不清除
    {
        for(int iBin=0; iBin<iTestBinCount; iBin++)
        {
            for(int i=0; i<MAX_SOCKET_ROW*MAX_SOCKET_COL; i++)
            {
                if(iNeedRT!=0 && LotSummary.bIsRTBin[iBin]==true)
                {
                    LotSummary.iCountCategory[i][iBin]=0;
                    LotSummary.iTotalCategory[iBin]=0;
                }
            }
        }
        iE1Count=0;                                                             //JerryYang 20230322 : SPIL 2D SORT summary修改
        iE2Count=0;
        iE3Count=0;
    }

    if(iSaveData==0)
        return;

//    TStringList *sIPList=new TStringList();
    TStringList *sList  =new TStringList();

    if(TestIF_File.bSCKART_EnableART==false &&                                  //Steven 20230322 : Fixed for summary count
       IniConfig.bA37LotStartLotEnd==false &&
       IniConfig.bSPILFunction==false)                                          //JerryYang 20230822 : fix unloader count
    {
        iLotCount   =LastSet.SendCT[0];
        iUnloadCount=RunInfo.iUnloadCount;
    }

    if(IniConfig.bSPILFunction==true)
    {
        if(IniConfig.bA66_2D_Sort==false)   //JerryYang 20260201 : modify log
        {
            Str.sprintf("CUSTOMER:NVIDIA");                                         //JerryYang 20230322 : SPIL 2D SORT summary修改
            sList->Add(Str);
            Str.sprintf("ASSEMBLY SITE:SPIL");
            sList->Add(Str);
            Str.sprintf("INNER_LOT_ID:%s", fLotInfo->edtSysLotID->Text);
            sList->Add(Str);
            Str.sprintf("CUST_LOT_ID:%s", fLotInfo->edtCusLotID->Text);             //JerryYang 20230322 : SPIL 2D SORT summary修改
            sList->Add(Str);
            Str.sprintf("CUSTOMER_DEVICE_GROUP:%s", fLotInfo->edtCusDevGrp->Text);
            sList->Add(Str);
            Str.sprintf("DEVICE_NAME:%s", fLotInfo->edtDevice->Text);
            sList->Add(Str);
            #ifdef HiSilicon
            Str.sprintf("PROGRAM_NAME:%s.%s", fSCKART->sInfo_ProgramName, HISI_VERSION);  //JerryYang 20230822 : 修改檔名
            #else
            Str.sprintf("PROGRAM_NAME:%s.%s", fSCKART->sInfo_ProgramName, MainVersion);  //JerryYang 20230822 : 修改檔名
            #endif
            sList->Add(Str);
            Str.sprintf("HANDLER_ID:%s", IniConfig.SocketHandlerID);
            sList->Add(Str);
            Str.sprintf("OPERATOR_ID:%s", fLotInfo->edtSysOperatorID->Text);
            sList->Add(Str);
            Str.sprintf("OPERATION:VS");
            sList->Add(Str);
            Str.sprintf("STAGE:%s", fLotInfo->cbRunMode->Text);
            sList->Add(Str);
        }
        else        //JerryYang 20260201 : modify log
        {
            Str.sprintf("CUSTOMER:%s", fSCKART->sInfo_Customer);                                         //JerryYang 20230322 : SPIL 2D SORT summary修改
            sList->Add(Str);
            Str.sprintf("ASSEMBLY SITE:SPIL");
            sList->Add(Str);
            Str.sprintf("INNER_LOT_ID:%s", fLotInfo->edtSysLotID->Text);
            sList->Add(Str);
            Str.sprintf("CUST_LOT_ID:%s", fLotInfo->edtCusLotID->Text);             //JerryYang 20230322 : SPIL 2D SORT summary修改
            sList->Add(Str);
            Str.sprintf("CUSTOMER_DEVICE_GROUP:%s", fLotInfo->edtCusDevGrp->Text);
            sList->Add(Str);
            Str.sprintf("DEVICE_NAME:%s", fSCKART->sInfo_DeviceName);
            sList->Add(Str);
            Str.sprintf("PROGRAM_NAME:%s.%s", fSCKART->sInfo_ProgramName, MainVersion);  //JerryYang 20230822 : 修改檔名
            sList->Add(Str);
            Str.sprintf("HANDLER_ID:%s", IniConfig.SocketHandlerID);
            sList->Add(Str);
            Str.sprintf("OPERATOR_ID:%s", fLotInfo->edtSysOperatorID->Text);
            sList->Add(Str);
            Str.sprintf("OPERATION:VS");
            sList->Add(Str);
            Str.sprintf("STAGE:%s", fSCKART->sInfo_Stage);
            sList->Add(Str);
            Str.sprintf("STEP:%s", fSCKART->sInfo_Step);
            sList->Add(Str);
            Str.sprintf("CUST STEP NO:%s", fLotInfo->edtCusStep->Text);
            sList->Add(Str);
        }
    }
    else if(bReadLotInfoFromART==true)
    {
        Str.sprintf("CUSTOMER:%s", fSCKART->sInfo_Customer);
        sList->Add(Str);
        if(IniConfig.bSPILFunction==true)
            Str.sprintf("ASSEMBLY SITE:SPIL");
        else
            Str.sprintf("ASSEMBLY SITE:%s", fObserver->labFactory->Caption);
        sList->Add(Str);
        Str.sprintf("INNER_LOT_ID:%s", fSCKART->sLotID);
        sList->Add(Str);
        Str.sprintf("CUST_LOT_ID:%s", fSCKART->sInfo_CustLotID);
        sList->Add(Str);
        Str.sprintf("CUSTOMER_DEVICE_GROUP:%s", fSCKART->sInfo_CustDevGup);
        sList->Add(Str);
        Str.sprintf("DEVICE_NAME:%s", fSCKART->sInfo_DeviceName);
        sList->Add(Str);
        #ifdef HiSilicon
        Str.sprintf("PROGRAM_NAME:%s.%s", fSCKART->sInfo_ProgramName, HISI_VERSION);
        #else
        Str.sprintf("PROGRAM_NAME:%s.%s", fSCKART->sInfo_ProgramName, MainVersion);
        #endif
        sList->Add(Str);
        Str.sprintf("HANDLER_ID:%s", fSCKART->sInfo_HandlerID);
        sList->Add(Str);
        Str.sprintf("OPERATOR_ID:%s", fSCKART->sInfo_OperatorID);
        sList->Add(Str);
    }
    else if(IniConfig.bN23UseLotInfoFile)
    {
        sList->Text=fLotInfo->mmo2DLotInfo->Text;
        Str.sprintf("PROGRAM_NAME:%s", asHandlerVersion);
        sList->Add(Str);
        Str.sprintf("HANDLER_ID:%s", IniConfig.SocketHandlerID);
        sList->Add(Str);
    }
    else
    {
        Str.sprintf("LOT_ID:%s", fLotInfo->edtSysLotID);
        sList->Add(Str);
        Str.sprintf("ASSEMBLY SITE:%s", fObserver->labFactory->Caption);
        sList->Add(Str);
        Str.sprintf("PROGRAM_NAME:%s", asHandlerVersion);
        sList->Add(Str);
        Str.sprintf("HANDLER_ID:%s", IniConfig.SocketHandlerID);
        sList->Add(Str);
    }

    if(fSCKART->sLotStartTime!="")                                              //Richard 20230111 : Fix fSCKART->sLotStartTime 為NULL。
    {
        Str.sprintf("SUMMARY_START_TIME:%s", fSCKART->sLotStartTime);
    }
    else
    {
        Str.sprintf("SUMMARY_START_TIME:%s%s%s%s%s%s", RunInfo.LotStartTime.SubString(1, 4), RunInfo.LotStartTime.SubString(6, 2), RunInfo.LotStartTime.SubString(9, 2),
                                                       RunInfo.LotStartTime.SubString(12, 2), RunInfo.LotStartTime.SubString(15, 2), RunInfo.LotStartTime.SubString(18, 2));
    }
    sList->Add(Str);
    if(fSCKART->sLotStartTime!="")                                              //Richard 20230111 : Fix fSCKART->sLotStartTime 為NULL。
    {
        Str.sprintf("SUMMARY_END_TIME:%s", fSCKART->sLotEndTime);
    }
    else
    {
        Str.sprintf("SUMMARY_START_TIME:%s%s%s%s%s%s", RunInfo.LotEndTime.SubString(1, 4), RunInfo.LotEndTime.SubString(6, 2), RunInfo.LotEndTime.SubString(9, 2),
                                                       RunInfo.LotEndTime.SubString(12, 2), RunInfo.LotEndTime.SubString(15, 2), RunInfo.LotEndTime.SubString(18, 2));
    }
    sList->Add(Str);

    for(int iBin=0; iBin<=iTestBinCount; iBin++)
    {
        iCount=0;
        for(int i=0; i<TestSocket.iShtRow; i++)
        {
            for(int j=0; j<TestSocket.iShtCol; j++)
            {
                iUnloadCount+=LotSummary.iCountCategory[iCount][iBin];
                iCount++;
            }
        }
    }

    for(int i=0; i<eTrayCount; i++)
    {
        aUnloader[i]=s6ShortTrayName[i]+AnsiString("(");
        bHaveBinData=false;
        if(Prod.iTrayType[i]!=tNotUse)
        {
            for(int j=0; j<=iTestBinCount; j++)
            {
                temp=Prod.iT6PosCate[j];
                if(temp<=0 && j!=iTestBinCount)
                    continue;
                if(i==temp-1)
                {
                    if(bHaveBinData==false)
                        aUnloader[i]+=AnsiString(j);
                    else
                        aUnloader[i]+=AnsiString(",")+AnsiString(j);

                    bHaveBinData=true;
//                    aUnloader[i]+=AnsiString(j);                              //JerryYang 20260124 : mark掉
                    iUnloadCnt[i]+=LotSummary.iTotalCategory[j];
                }

                if(Prod.iIfErrorT6==i && j==iTestBinCount)
                {
                    iUnloadCnt[i]+=LotSummary.iTotalCategory[j];
                }
            }

            if(Prod.iIfErrorT6==i)
            {
                aUnloader[i]+="error";
                bHaveBinData=true;
            }
            else
            {
                if(bHaveBinData==false)
                    aUnloader[i]+="Null";
            }
            aUnloader[i]+=")";
        }
    }

    Str.sprintf("Handler Load Qty:%d", LastSet.iSCKARTInputCT);                 //JerryYang 20200319 fix summary
    sList->Add(Str);
    Str.sprintf("Handler Unload Qty:%d", iUnloadCount);
    sList->Add(Str);
    sList->Add("");
    sList->Add(" ========================================================================");
    sList->Add(" =========================  Group Bin Summary  ===========================");
    sList->Add(" ========================================================================");

    for(int i=0; i<eTrayCount; i++)
    {
        if(Prod.iTrayType[i]!=tNotUse)
        {
            Str.sprintf("%s:%d", aUnloader[i], iUnloadCnt[i]);
            sList->Add(Str);
        }
    }

    sList->Add("");
    sList->Add(" ========================================================================");
    sList->Add(" =========================  Hard Bin Summary  ===========================");
    sList->Add(" ========================================================================");

    iCount=0;
    Str=" Hard P/F ";
    Str2=" ---- ----";
    for(int i=0; i<TestSocket.iShtRow; i++)
    {
        for(int j=0; j<TestSocket.iShtCol; j++)
        {
            iCount++;
            Str1.sprintf(" Site%d", iCount);
            Str=Str+Str1;
            Str2=Str2+" -----";
        }
    }
    Str=Str+" Total  Yield";
    sList->Add(Str);
    Str2=Str2+" ----- --------";
    sList->Add(Str2);

    for(int iBin=0; iBin<iTestBinCount+1; iBin++)
    {
        if(iBin==iTestBinCount)
        {
        }
        else
        {
            temp=Prod.iT6CatData[iBin];
            if(temp<0 || temp>=eTrayCount)
                continue;
        }

        if(iBin==iTestBinCount)
        {
            Str="Err";
        }
        else
        {
            Str.sprintf("%d", iBin);
        }

        while(Str.Length()<5)
        {
            Str=" "+Str;
        }

        if(iBin==iTestBinCount)
        {
            Str=Str+" FAIL";
        }
        else
        {
            if(Prod.bIsPassBin[iBin])
                Str=Str+" PASS";
            else
                Str=Str+" FAIL";
        }

        iCount=0;
        for(int i=0; i<TestSocket.iShtRow; i++)
        {
            for(int j=0; j<TestSocket.iShtCol; j++)
            {
                Str2.sprintf("%d", LotSummary.iCountCategory[iCount][iBin]);
                while(Str2.Length()<6)
                {
                    Str2=" "+Str2;
                }
                Str=Str+Str2;
                iCount++;
            }
        }

        if(IniConfig.bSPILFunction==true && LotSummary.iTotalCategory[iBin]<=0)
            continue;

        Str2.sprintf("%d", LotSummary.iTotalCategory[iBin]);
        while(Str2.Length()<6)
        {
            Str2=" "+Str2;
        }
        Str=Str+Str2;
        Str2=ChangeToPercentage(LotSummary.iTotalCategory[iBin], iUnloadCount);

        while(Str2.Length()<9)
        {
            Str2=" "+Str2;
        }
        Str=Str+Str2;

        sList->Add(Str);
    }

    sList->Add("");
    sList->Add(" ========================================================================");
    sList->Add(" ==========================  Yield Summary  =============================");
    sList->Add(" ========================================================================");

    Str ="  P/F    Qty    Yield";
    sList->Add(Str);
    Str2=" ----- ------ --------";
    sList->Add(Str2);

    int iPassCount=0;
    int iFailCount=0;
    for(int iBin=0; iBin<iTestBinCount+1; iBin++)
    {
        if(iBin==iTestBinCount)
        {
        }
        else
        {
            temp=Prod.iT6CatData[iBin];
            if(temp<0 || temp>=eTrayCount)
                continue;
        }

        iCount++;

        if(IniConfig.bSPILFunction==true && LotSummary.iTotalCategory[iBin]<=0)
            continue;

        if(Prod.bIsPassBin[iBin]==false || iBin==iTestBinCount)
        {
            iFailCount+=LotSummary.iTotalCategory[iBin];
        }
        else
        {
            iPassCount+=LotSummary.iTotalCategory[iBin];
        }
    }

    Str=" PASS";

    Str2.sprintf("%d", iPassCount);
    while(Str2.Length()<6)
    {
        Str2=" "+Str2;
    }
    Str=Str+Str2;

    Str2=ChangeToPercentage(iPassCount, iUnloadCount);
    while(Str2.Length()<9)
    {
        Str2=" "+Str2;
    }
    Str=Str+Str2;

    sList->Add(Str);

    Str=" FAIL";

    Str2.sprintf("%d", iFailCount);
    while(Str2.Length()<6)
    {
        Str2=" "+Str2;
    }
    Str=Str+Str2;

    Str2=ChangeToPercentage(iFailCount, iUnloadCount);
    while(Str2.Length()<9)
    {
        Str2=" "+Str2;
    }
    Str=Str+Str2;
    sList->Add(Str);

    sList->Add("");
    sList->Add(" ========================================================================");
    sList->Add(" ==========================  Error Bin Summary  =========================");
    sList->Add(" 991(Barcode Read Error)");
    sList->Add(" 992(Duplicate 2DID error)");
    sList->Add(" 993(2DID not existed in list)");
    sList->Add(" 994(Not defined)");
    sList->Add(" ========================================================================");

    LotSummary.iTotalCategory[iTestBinCount];

    Str2.sprintf(" 991: %d", iE1Count);
    sList->Add(Str2);
    Str2.sprintf(" 992: %d", iE2Count);
    sList->Add(Str2);
    Str2.sprintf(" 993: %d", iE3Count);
    sList->Add(Str2);
    Str2.sprintf(" 994: %d", LotSummary.iTotalCategory[iTestBinCount]-iE1Count-iE2Count-iE3Count);
    sList->Add(Str2);
    sList->Add(" ========================================================================");

    if(FileExists(PathName2+FileName)==false)                                   //Steven 20230317 : 避免連續存兩次把資料蓋掉了
    {
        if(CosFunction.bUseTSVFunction &&                                       //Steven 20240904 : for ATK的TSV功能
           IniConfig.bN09_LotCountAutoFunc)
        {
            if(fSCKART->iNeedRT==0)
            {
                if(IniConfig.iN09_4_UploadMethod==0)
                {
                    sList->SaveToFile(PathName2+FileName);                      //Steven 20230215 : 存檔路徑加上年月
                    FTP_Upload(PathName2, IniConfig.sN09_5_Path, FileName);
                }
                else
                {
                    sList->SaveToFile(PathName+FileName);
                }
            }
        }
        else
        {
            if(IniConfig.bSPILFunction==true)                                   //JerryYang 20220923 : unload數量為0不要存log
            {
                if(iUnloadCount>0)
                    sList->SaveToFile(PathName2+FileName);
            }
            else
            {
                sList->SaveToFile(PathName2+FileName);
            }
        }
        fObserver->memoLotSummary->Lines=sList;
    }
    sList->Clear();
//    mapIPList.clear();
//    sIPList->Clear();
//    delete sIPList;
    delete sList;

    if(IniConfig.bN17UploadLotSummary)                                          //JerryYang 20220923 : Upload lot summary
    {
        if(DirectoryExists(IniConfig.asN17LotSummaryPath))
        {
            if(IniConfig.bA38_SLT_Summary &&
               fSCKART->sLotID!="NA" &&
               fSCKART->sLotID!="" &&
               iUnloadCount>0)
            {
                strFileName=PathName2+FileName;                                 //Steven 20230215 : 存檔路徑加上年月
                if(FileExists(strFileName))
                {
                    strPath.sprintf("%s\\%s", IniConfig.asN17LotSummaryPath, FileName);
                    bResult=CopyFile(strFileName.c_str(), strPath.c_str(), true);
                    if(bResult==false)
                    {
                        ShowMyMessage("Uploaded lot summary error\r\nPlease check the path of N-17.","上傳lot summary失敗\r\n請檢查N-17路徑是否存在");
                    }
                    else
                    {
                        RecordProcess("Uploaded lot summary successfully.");    //Steven 20190722 : add TSV log
                    }
                }
            }
            else
            {
                if(fSCKART->sInfo_CustLotID=="NA" || fSCKART->sInfo_CurrQty=="NA" ||
                   fSCKART->sInfo_CustLotID=="" || fSCKART->sInfo_CurrQty=="")
                {
                }
                else
                {
                    strFileName=PathName2+FileName;                             //Steven 20230215 : 存檔路徑加上年月
                    if(FileExists(strFileName))
                    {
                        strPath.sprintf("%s\\%s", IniConfig.asN17LotSummaryPath, FileName);
                        bResult=CopyFile(strFileName.c_str(), strPath.c_str(), true);
                        if(bResult==false)
                        {
                            ShowMyMessage("Uploaded lot summary error\r\nPlease check the path of N-17.","上傳lot summary失敗\r\n請檢查N-17路徑是否存在");
                        }
                        else
                        {
                            RecordProcess("Uploaded lot summary successfully.");//Steven 20190722 : add TSV log
                        }
                    }
                }
            }
        }
        else
        {
            ShowMyMessage("Uploaded lot summary error\r\nPlease check the path of N-17.", "上傳lot summary失敗\r\n請檢查N-17路徑是否存在");
        }
    }

    if(IniConfig.bA38_SLT_Summary && FileName!="" && iUnloadCount>0)
    {
        ShellExecute(NULL, NULL, strFileName.c_str(), NULL, NULL, SW_SHOW);
    }

    if(CosFunction.bUseTSVFunction &&                                           //Steven 20240904 : for ATK的TSV功能
       IniConfig.bN09_LotCountAutoFunc)
    {
        bWaitTSV=true;
        bShowTSVMsg=false;
        sTSVMsg="";
        RecordProcess("Lot end and send summary and waiting TSV reply.");       //Steven 20190722 : add TSV log
        TSVdelay.SetSecAndOn(IniConfig.dN09_SearchTime);
        TimerTSV->Enabled=true;
    }

    if(iSaveData==1)
        LotSummary.ClearAllData();

    Str2.sprintf("%s\\SortBy2DID_%s.csv", "D:\\HT9045_Log\\2D_SortList", fLotInfo->edtSysLotID->Text);
    str3.sprintf("%s\\SortBy2DID_%s.csv", asBackup2DSortListPath, fLotInfo->edtSysLotID->Text);  //JerryYang 20230822 : 佳熹要求備份2D sort list
    if(FileExists(Str2))                                                        //Steven 20160505 : 加上保護, 不然開程式會跳Error
    {
        CopyFile(Str2.c_str(), str3.c_str(), true);                             //複製到Backup資料夾  //JerryYang 20230822 : 佳熹要求備份2D sort list
        DeleteFile(Str2);
    }

    if(IniConfig.bSPILFunction)                                                 //JerryYang 20231205 : 備份2DID summary
    {
        Str2=PathName2+FileName;
        str3=asBackup2DSummaryPath+FileName;
        if(FileExists(Str2))
        {
            CopyFile(Str2.c_str(), str3.c_str(), true);                         //複製到Backup資料夾
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfSCKART::srvrscktTSVClientRead(TObject *Sender,
      TCustomWinSocket *Socket)
{
    AnsiString Str;
    if(Socket->ReceiveLength()>0)
    {
        sTSVMsg=Socket->ReceiveText();
        bShowTSVMsg=true;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfSCKART::TimerTSVTimer(TObject *Sender)
{
    static bool bTimerRun=false;

    if(bTimerRun)
        return;

    if(bTimerRun==false)
    {
        bTimerRun=true;
        if(bShowTSVMsg)
        {
            bWaitTSV=false;
            ShowMyMessage(sTSVMsg);
            TimerTSV->Enabled=false;
        }
        else if(TSVdelay.Off())
        {
            bWaitTSV=false;
            if(IniConfig.bN09_Enable_TSV)                                       //Steven 20231017 : add for ATK
                ShowMyMessage("Get LCA result time out!!");
            else
                RecordProcess("Get LCA result time out!!");
            TimerTSV->Enabled=false;
        }
    }
    bTimerRun=false;
}
//---------------------------------------------------------------------------
bool __fastcall TfSCKART::FTP_Upload(AnsiString sSourcesFilePath, AnsiString sTargetFilePath, AnsiString sULFileName)
{
    AnsiString sMsg="", sUploadFile, str;
    bool bFTPAction=false;
    //防止連續 FTP 連續
    if(bFTPAction==true)
        return true;
    bFTPAction=true;

    //檢查檔案路徑是否為空值
    if(sSourcesFilePath.Trim()=="" || sTargetFilePath.Trim()=="")
    {
        bFTPAction=false;
        sMsg="TSV Upload FilePath or FileName Empty Error!";
        ShowMyMessage(sMsg);
        return false;
    }

    AnsiString asError, SourceFile, TargetFile;
    RecordProcess("Start TSV procedure.");                                      //Steven 20200522 : add TSV log
    TfFTP fFTP(IniConfig.sN09_5_User, IniConfig.sN09_5_Password, IniConfig.sN09_5_Host);
    if(fFTP.Connect(IniConfig.sN09_5_User, IniConfig.sN09_5_Password, IniConfig.sN09_5_Host, 30000, NMOS_AUTO, true, 21))
    {
        fFTP.CheckLocalFilePath(sSourcesFilePath);
        fFTP.ChangeDirectories(sTargetFilePath);
        if(sULFileName!="")
        {
            SourceFile=FileInfo().PathCombin(sSourcesFilePath, sULFileName);
            TargetFile=FileInfo().PathCombin(sTargetFilePath, sULFileName);
            if(FileExists(SourceFile))
            {
                str.sprintf("Upload file to TSV FTP. %s --> %s", SourceFile, TargetFile);
                RecordProcess(str);                                             //Steven 20200522 : add TSV log

                if(fFTP.Upload(sSourcesFilePath, sTargetFilePath, sULFileName, sULFileName, asError))
                {
                    ;
                }
                else
                {
                    RecordProcess("Upload file to TSV FTP Fail"+SourceFile);
                }
            }
            else
            {
                RecordProcess("Can not find : "+SourceFile);
            }
        }
        else
        {
            //上傳路徑內所有檔案
            AnsiString sFindFile=sSourcesFilePath+"*.*";
            TSearchRec srFile;
            if(FindFirst(sFindFile, 0, srFile)==0)
            {
                do
                {
                    sULFileName=srFile.Name;
                    SourceFile=FileInfo().PathCombin(sSourcesFilePath, sULFileName);
                    TargetFile=FileInfo().PathCombin(sTargetFilePath, sULFileName);

                    if(FileExists(SourceFile))
                    {
                        str.sprintf("Upload file to TSV FTP. %s --> %s", SourceFile, TargetFile);
                        RecordProcess(str);                                     //Steven 20200522 : add TSV log
                        fFTP.Upload(sSourcesFilePath, sTargetFilePath, sULFileName, sULFileName, asError);
                    }
                    MySleep(50);
                }
                while(FindNext(srFile)==0);
            }
        }
    }
    bFTPAction=false;

    return true;
}
//------------------------------------------------------------------------------
void __fastcall TfSCKART::edtMRejectCntClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 1000, -1000);
}
//---------------------------------------------------------------------------
void __fastcall TfSCKART::edtAlmAutoCloseSiteClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 32, 1);
}
//---------------------------------------------------------------------------
void TfSCKART::DoARTLotStart(AnsiString _sLotID, AnsiString _sProcess, int _iLotCount)
{
    bReadLotInfoFromART=true;
    bool bChangeLotID=false;
    if(HasICUnderMachine()==false)
    {
        bChangeLotID=true;
    }
    else
    {
        if(sLotID==_sLotID && (sLotID!="" && _sLotID!=" "))
        {
            bChangeLotID=false;
        }
        else
        {
            bChangeLotID=true;
        }
    }

    if(bChangeLotID==true)
    {
        iCurrent93KARTStep=1;
        iCurrentFlexARTStep=4;
        ClearLotInfo();
        SetLotStatus(iLOTSTATUS_W);
        sLotID=_sLotID;
        sProcessCode=_sProcess;                                                 //Steven 20190521 : ATK lot count
        if(TestIF_File.bRENESAS_EnableFTCT==false)                              //RogerYang 20250930 : RogerYang 瑞薩FT-CT 指令"20"已更新
            sLotStartTime=Now().FormatString("yyyymmdd_hhnnss");
        fLotInfo->cbProcess->Text=_sProcess;
        fLotInfo->SetLotID(_sLotID, false);
        fLotInfo->SetLotStart(__FUNC__);

        if(bQAModeFlag==true)                                                   //Steven 20170830 (wei) : QA mode for ATK ART
        {
            iLotCount  =TestIF_File.iQAModeCount;
            iInputCount=TestIF_File.iQAModeCount;
            AccessFile(false, 1);                                               //必須在轉換模式前先存檔一次, 不然轉模式的裡面會讀取到舊的資料
            SetRunStartMode(rsmInitial_ART);
            bQAModeFlag=false;
        }
        else
        {
            iLotCount  =_iLotCount;                                             //Steven 20250410 : fixed for ART
            iInputCount=_iLotCount;
            AccessFile(false, 1);
        }

        RecordProcess(AnsiString().sprintf("Lot start: %s, %d", sLotID, iLotCount));    //Steven 20190722 : add TSV log
    }

    if(iTesterType==1)
    {
        iNeedRT=0;
        LastSet.bEndLotAutoRetestGPIB=false;
        LastSet.bWaitStartLotAutoRetestGPIB=false;
        LastSet.bFirstTestAutoRetestGPIB=true;
        fMain->SetLotState(2);                                                  //ART FT Start for SCK
        if(TestIF_File.bAlarmAfterSendSRQKIND2)                                 //Steven 20230202 : ATK 要在SRQKIND2之後Alarm
        {
            fMain->tESDError->Add("MES07399");
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfSCKART::ledShowFTCTDataClick(TObject *Sender)                 //RogerYang 20250916 : 瑞薩FT-CT
{
    fMain->RENESAS_Server->ShowServerForm();
}
//---------------------------------------------------------------------------
__fastcall TFormInputQty::TFormInputQty(TComponent* Owner, int idx) : TForm(Owner, 0)    //RogerYang 20251002 : 瑞薩FT-CT //第二參數 0 = 不要載入 DFM
{
    index=idx;

    Width=300;
    Height=150;

    // 保留右上角的 X 關閉按鈕
    BorderStyle=bsDialog;   // 或 bsSingle，看樣式
    Position=poDesigned;    // 計算 Left / Top
    if(idx==1)
    {
        Caption="Edit Input QTY Manually";
        Color=(TColor)0x00FFDDDD; // 淺紫色
    }
    else if(idx==2)
    {
        Caption="Edit Manual Reject count";
        Color=(TColor)0x0000A5FF; // 橘色
    }

    edt=new TEdit(this);
    edt->Parent=this;
    edt->Left=120;
    edt->Top=25;
    edt->Left=(ClientWidth-edt->Width)/2;
    edt->Top=(ClientHeight-edt->Height)/2-20;
    edt->OnClick=BtnInputQTYEdtClick;

    btn=new TButton(this);
    btn->Parent=this;
    btn->Left=80;
    btn->Top=30;
    btn->Caption="Apply";
    btn->Left=(ClientWidth-btn->Width)/2;
    btn->Top=edt->Top+edt->Height+20;
    btn->OnClick=BtnInputQTYApplyClick;

    // 把整個 Form 移到螢幕右側中間
    Left=Screen->Width-Width-20;
    Top=(Screen->Height-Height)/2;
}
//---------------------------------------------------------------------------
void __fastcall TFormInputQty::BtnInputQTYEdtClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 100000, 0);
}
//---------------------------------------------------------------------------
void __fastcall TFormInputQty::BtnInputQTYApplyClick(TObject *Sender)
{
    int val=atoi(edt->Text.c_str());
    if(index==1)
    {
        fSCKART->iInputCount=val;
        fSCKART->palInputCount->Caption=fSCKART->iInputCount;
    }
    else if(index==2)
    {
        fSCKART->iManualRejectCnt=val;
    }
    fSCKART->AccessFile(false, -1);
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfSCKART::palInputCountMouseDown(TObject *Sender,               //RogerYang 20251002 : RogerYang 瑞薩FT-CT
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TPanel *pnl=dynamic_cast<TPanel*>(Sender); // 判斷是 Label
    if (!pnl) return;

    int idx = 0;
    if (pnl==palInputCount)
        idx = 1;
    else if (pnl==palManualRejectCnt)
        idx = 2;

    if (Shift.Contains(ssDouble))   // 判斷是否為雙擊
    {
        if(TestIF_File.bRENESAS_EnableFTCT==false)
            return;
        TFormInputQty *formQty=new TFormInputQty(this, idx);
        formQty->Show();
        fMain->RENESAS_Server->bLoadingCountFullFlag=false;                     //釋放掉供給/不供給旗標
        bFirstFullSkip=false;                                                   //RogerYang 20251112 : ART數量到，如果inarm上有IC直接丟到shuttle
    }
}
//---------------------------------------------------------------------------
void __fastcall TfSCKART::btnFTCTResetClick(TObject *Sender)                    //RogerYang 20251017 : 瑞薩FT-CT Reset初始化
{
    fMain->RENESAS_Server->ResetFTCTSeq();
}
//---------------------------------------------------------------------------
void __fastcall TfSCKART::edtLdCntLimNClick(TObject *Sender)                    //RogerYang 20251224 : for RF360ART
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 0, 9999);
}
//---------------------------------------------------------------------------
bool TfSCKART::DoChkInputCntAlarm(bool bExcess)                                 //RogerYang 20251224 : Rf360需求 lotcheck加入上下限設定
{
    if(CUSTOMER_CODE==CC_QUALCOMM)
    {
        int iLoadingCount=LastSet.iSCKARTInputCT;
        if(bExcess==true)  //數量超過
        {
            if(iLoadingCount<=iInputCount+TestIF_File.iSCKLdCntLimP)            //允許超過設定數量"前"不報警
            {
                return false;
            }
            else
            {
                return true;
            }
        }
        else               //數量不足
        {
            if(iLoadingCount>=iInputCount-TestIF_File.iSCKLdCntLimN)            //允許超過設定數量"後"不報警
            {
                return false;
            }
            else
            {
                return true;
            }
        }
    }
    else
    {
        return true;
    }
}
//---------------------------------------------------------------------------
