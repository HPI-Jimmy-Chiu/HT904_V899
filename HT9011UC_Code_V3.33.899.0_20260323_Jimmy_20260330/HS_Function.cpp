#include "MachineDefine.h"
#pragma hdrstop

#include "HS_Function.h"

#include "MachineType.h"
#include "TextProcess.h"
#include "adam6024.h"
#include "cmydef.h"
#include "common.h"
#include "cpublic.h"
#include "InterfaceSYS.h"
#include "main.h"
#include "UsecegemMainFrom.h"
#include "uHGemHT9045.h"
#include "mymessbox.h"
#include "uLotInfo.h"
#include "cContact.h"
#include "ATCInterface.h"
#include "ATC_Handler_Side.h"                                                   //Ifor 20151230 :add New ATC Interface
#include "note.h"                                                               //Ifor 20160819 add Show SIHI Start Alarm
#include "cprod.h"                                                              //Ifor 20160829 add Check CosFunction
#include "mysensor.h"
#include "MyMotor.h"
#include "database.h"
#include "cMyDB.h"
#include "Password.h"
#include "uYieldMonitoring.h"
#include "cObserver.h"
#include "aoutarm.h"

#include <map>
#include "csystem.h"
#include "uCleaning.h"
#include "cConfiguration.h"
#include "HTKeyPro.h"                                                           //Ifor 20230720 add" Keypro
#include "TfFTP.h"
#include "SCK_ART.h"                                                            //RogerYang 20250918 : 瑞薩FT-CT
#include "BarCode.h"                                                            //RogerYang 20251202 : JCET 2D FT1白名單/FT2比對功能

using namespace std;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
#pragma link "KeyProDLL.lib"                                                    //Ifor 20230720 add" Keypro

//AI(ht9045-v899) 20260415: forward-declare MNetLog before first use at UpDataToServer_KYEC
extern bool MNetLog(AnsiString Message);

TFormHS *FormHS;

bool bRestESDConnect=false;                                                     //Ifor 20170603 (wei) add ESD Socket 清除連線
bool bResetRTMConnect=true;
//---------------------------------------------------------------------------
__fastcall TFormHS::TFormHS(TComponent* Owner)
    : TForm(Owner)
{
    bNeedUpload_N10=false;
    TimerAutoBackup->Enabled=true;
    TimerRTMMsg->Enabled=true;
    bHaltHandler=false;
    bN10_3_1_Flag=false;
}
//---------------------------------------------------------------------------
void __fastcall TFormHS::FormCreate(TObject *Sender)
{
    tHiSiRecord=new TStringList();
    ClearLastFileLogName_HS(-1);
}
//---------------------------------------------------------------------------
void __fastcall TFormHS::FormDestroy(TObject *Sender)
{
    try
    {
        TimerAutoBackup->Enabled=false;
        TimerRTMMsg->Enabled=false;
        tHiSiRecord->Clear();                                                   //Ifor 20170603 (wei) TStringList 刪除前先 Clean
        delete tHiSiRecord;
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TFormHS::FormDestroy");
    }
    LogSoftwareOffTime("TFormHS, FormDestroy");                                 //Steven 20210526 : 紀錄軟體執行時間
}
//---------------------------------------------------------------------------
void __fastcall TFormHS::TimerAutoBackupTimer(TObject *Sender)
{
    static bool bTimerRunning=false;

    static int iATCSelfTest=0;
    int iATCSelfTestResultCheck=0;
    static int iFlickerCount=0;                                                 //Ifor 20160824 add ATC Self Test 狀態閃爍計時
    static bool bFlickerStatus=0;                                               //Ifor 20160824 add ATC Self Test 狀態閃爍
    static int iReadAdamCT=0;                                                   //jou 20170413 (Steven) : Read Adam EP 提升UPH
    bool bHasAlarm=false;
    int iNowTime;
    AnsiString str;
    double dValue=0.0;
    int iWeek=1;                                                                //RogerYang 20251104 : 偉測MTBF文件生成
    int iHour, iMin;                                                            //RogerYang 20251117 : 偉測MTBF文件生成

//    static bool bIdelTrigger_0000AM=false;

    if(InitialOK==false || bTimerRunning==true ||
       (ATC_InterfaceForm->HasAlarmMsg()==true))
    {
        return;
    }
    bTimerRunning=true;

    if(USE_NOVX3360==true)                                                      //Ifor 20160316 add 每5秒要一次ESD 資料
    {
        try
        {
            if(bRestESDConnect==true)                                           //Ifor 20170603 (wei) ESD Socket 重置
            {
                bRestESDConnect=false;
                ESDServerSocket->Close();
                ESDServerSocket->Open();
            }
        }
        catch(...)
        {
            MyDBIProcess("Exception", "TFormHS::TimerAutoBackupTimer");
        }
    }

    if(IniConfig.bD26EnableEncodeShow)                                          //Ifor 20160304 add for Read EP Encoder value start
    {
        fMain->lbEPenconder->Visible=true;
        if(CUSTOMER_CODE==CC_KYEC_LEE)                                          //jou 20170413 (Steven) : Read Adam EP 提升UPH
        {
            fMain->lbEPenconder->Caption=ADAM_ReadPA(&dValue);                  //wei 20220309 Add EP Return Voltage
        }
        else
        {
            if(SystemStart==true && IniConfig.bD26EnableEPLog==true)            //jou 20170413 (Steven) : Read Adam EP 提升UPH
            {
                str.sprintf("EP: %0.0f / %d", iAdamOutValue, iReadAdamEP);      //Steven 20190122 : 修改EP Encoder顯示
                fMain->lbEPenconder->Caption=str;
            }
            else
            {
                iReadAdamCT++;
                if(iReadAdamCT>10)
                {
                    str.sprintf("EP: %0.0f / %d", iAdamOutValue, ADAM_ReadPA(&dValue));    //Steven 20190122 : 修改EP Encoder顯示   //wei 20220309 Add EP Return Voltage
                    fMain->lbEPenconder->Caption=str;
                    iReadAdamCT=0;
                }
            }
        }
    }
    else
    {
        fMain->lbEPenconder->Visible=false;
    }

    CalculateUploadToHostIntervalTimeFunction();                                //Ifor 20160302 Add FTP Up load Log To Host

    if(CheckClockTrigger(0))                                                    //00:00AM
    {                                                                           //Ifor 20160408 每天00:00:00 上傳Log資料並重新建立新Log
        NewRecordProcess("", "00:00 AM", asHandlerVersion +"."+ AnsiString(SVNRevision));   //Sam 20230324 : 跨日的時候紀錄目前軟體版本

        if(IniConfig.bSPILFunction==true)                                       //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction
        {
            bSysLotStart=false;
            RecordLog_HS(true);
        }

        if(IniConfig.bN26_UseJamRawDataRecord)                                  //Sam 20210302 : 跨日上傳 JamRawData
            fObserver->StatisticalJamCount(true);

        if(CosFunction.bDownloadUpdateAutomatically &&                          //Sam 20220824 : FTP 自動下載安裝更新包
           IniConfig.bN32_CheckForUpdatesOnceDay)
        {
            if(bUpdateAutomaticallyChecked)
                bUpdateAutomaticallyChecked=false;
        }

        RunInfo.SaveJamRateByDay();

        if(IniConfig.bN10_DailyUploadProdData &&
           IniConfig.iN10UploadProductMethod==0)                                //Steven 20250809 : 換位子
        {
            UpDataToServer_KYEC(5);
            if(IniConfig.bPowerSaveFunction)
            {
                UpDataToServer_KYEC(6);
                if(ATC_SYSTEM==eNewATCSystem)
                {
                    UpDataToServer_KYEC(7);
                }
            }
        }
    }

    if(CheckClockTrigger(1))                                                    //01:00AM
    {
        fMain->UploadProdLog();                                                 //JerryYang 20200706 AM 0100 上傳Production log
    }

    if(CheckClockTrigger(8) ||                                                  //08:00AM
       CheckClockTrigger(20))                                                   //08:00PM
    {
        if(IniConfig.bN10_DailyUploadProdData &&                                //Steven 20250809 : 換位子
           IniConfig.iN10UploadProductMethod==1)                                //JerryYang 20190131 上傳production log可選擇00:00 or 08:00
        {
            UpDataToServer_KYEC(5);
            if(IniConfig.bPowerSaveFunction)
            {
                UpDataToServer_KYEC(6);
                if(ATC_SYSTEM==eNewATCSystem)
                {
                    UpDataToServer_KYEC(7);
                }
            }
        }
    }

    if(CheckClockTrigger(30))
    {
        if(IniConfig.bN35_Ground_ESD_Upload && IniConfig.iN35_Interval==0)      //Sam 20250609 : Record Ground and ESD at intervals and upload
            fLotInfo->SaveGroundESDData_Upolad();
    }

    if(CheckClockTrigger(60))                                                   //JerryYang 20180515 記錄Time data
    {
        if(IniConfig.iN10UploadProductMethod==1)                                //第三組是整點
            RecordTimeData(2);
        else
            RecordTimeData(3);

        if(IniConfig.bN35_Ground_ESD_Upload && IniConfig.iN35_Interval==1)      //Sam 20250609 : Record Ground and ESD at intervals and upload
            fLotInfo->SaveGroundESDData_Upolad();

        if(IniConfig.bN10_DailyUploadProdData &&                                //Steven 20250809 : 換位子
           IniConfig.iN10UploadProductMethod==2)                                //第三組是整點
        {
            UpDataToServer_KYEC(5);
            if(IniConfig.bPowerSaveFunction)
            {
                UpDataToServer_KYEC(6);
                if(ATC_SYSTEM==eNewATCSystem)
                {
                    UpDataToServer_KYEC(7);
                }
            }
        }

        if(IniConfig.bVTESTFunction==true)                                      //RogerYang 20251104 : 偉測MTBF文件生成
        {
            if(IniConfig.bO19_AutoRecordReportByEveryWeek)
            {
                iWeek=Now().DayOfWeek()-1;                                      //0=Sun.
                iHour=Now().FormatString("hh").ToInt();
                iMin=Now().FormatString("nn").ToInt();

                if(iWeek==IniConfig.iO19_WeekPeriod &&                          //RogerYang 20251117 :判斷跨週，時間在當天前五分鐘內
                    (iHour==0 && iMin<=5))
                {
                    SendCommand_EventLog(EL_VTEST_MTBF_SUM, "1");               //通知eventlog小程式取前七天的資料做MTBF文件
                    RecordProcess("SendCommand_EventLog:EL_VTEST_MTBF_SUM");
                }
            }
        }
    }

    if(CheckClockTrigger(90))
    {
        if(IniConfig.bN35_Ground_ESD_Upload && IniConfig.iN35_Interval==2)      //Sam 20250609 : Record Ground and ESD at intervals and upload
            fLotInfo->SaveGroundESDData_Upolad();
    }

    if(CheckClockTrigger(120))
    {
        if(IniConfig.bN35_Ground_ESD_Upload && IniConfig.iN35_Interval==3)      //Sam 20250609 : Record Ground and ESD at intervals and upload
            fLotInfo->SaveGroundESDData_Upolad();
    }

    iNowTime=StrToInt(GetOnlyTimeInfoByString());
    if(IniConfig.bVTESTFunction==true)                                          //jou 20210108 : 上海偉測要求新增每日jam rate統計
    {
        if(iNowTime>=0 && iNowTime<3)
        {
            LastSet.iDaySendCT=0;
            LastSet.iDayJamCount=0;
        }
    }

    if(bNeedWaitATCRunSelfTestFinish==true)                                     //Ifor 20160721 add ATC Self Test Time 計數
    {
        iATCSelfTest++;
        if(iATCSelfTest>=9000)
        {
            ShowErrorMessage("WAR15318", 0, MMATC_ATC);                         //ATC Self-Test Time Out
            bNeedWaitATCRunSelfTestFinish=false;
            bNeedSendATCRunSelfTest=true;
        }

        if(ATC_SYSTEM==eNewATCSystem)                                           //Ifor 20160818 先保留後續需將IF資料移除(IF更新)
        {                                                                       //Ifor 20160724 方法修改至當收到ATC回傳資料立即Alarm
            iATCSelfTestResultCheck=ATC_InterfaceForm->GetRunSelfTestResult();  //Ifor 20160824 add Get ATC Self Test Result
        }
        else
        {
            iATCSelfTestResultCheck=ATCInterfaceForm->GetRunSelfTestResult();   //Ifor 20160824 add Get ATC Self Test Result
        }

        iFlickerCount++;
        if(iFlickerCount>=5)                                                    //Ifor 20160830 add 0.5秒閃爍一次
        {
            iFlickerCount=0;
            bFlickerStatus=!bFlickerStatus;
        }

        if(iATCSelfTestResultCheck>0 && iATCSelfTestResultCheck<5)              //Ifor 20160830 收到回覆
        {
            bHasAlarm=true;
            bFlickerStatus=true;
        }

        ShowATCSelfTestSatus(bFlickerStatus,bHasAlarm);                         //Ifor 20160829 add Show ATC Self Test Satus

        switch(iATCSelfTestResultCheck)                                         //Ifor 20160724 add 新增 ATC Slef Test Fail
        {
            case 1:                                                             //Ifor 20160724 add ATC自我測試+5C結果失敗
                ShowErrorMessage("WAR15313", 0, MMATC_ATC);                     //ATC Self-Test Result +5C Fail
                break;
            case 2:                                                             //Ifor 20160724 add ATC自我測試-5C結果失敗
                ShowErrorMessage("WAR15314", 0, MMATC_ATC);                     //ATC Self-Test Result -5C Fail
                break;
            case 3:                                                             //Ifor 20160724 add ATC自我測試+0C結果失敗
                ShowErrorMessage("WAR15315", 0, MMATC_ATC);                     //ATC Self-Test Result +0C Fail
                break;
            case 4:                                                             //Ifor 20160724 add ATC自我測試結果異常失敗
                ShowErrorMessage("WAR15316", 0, MMATC_ATC);                     //ATC Self-Test Result Other Fail
                break;
            case 5:                                                             //Ifor 20160724 add ATC自我測試結果結果成功
                //ShowErrorMessage("WAR15317", 0, MMATC_ATC);                   //ATC Self-Test Result Success
                RecordProcess("ATC Self-Test Result Success");                  //Ifor 20160908 KYEC 要求 ATC Self Test Success 不要秀出提示畫面
                break;
        }

        if(iATCSelfTestResultCheck>0 && iATCSelfTestResultCheck<=5)             //Ifor 20160823 Add ATC Slef Test 結果判斷
        {
            if(iATCSelfTestResultCheck==5)
            {
                bNeedSendATCRunSelfTest     =false;
                bInitialATCSelfTest         =false;
            }
            else
            {
                bNeedSendATCRunSelfTest     =true;
            }
            bNeedWaitATCRunSelfTestFinish   =false;
            pl_ATCSelfTestSatus_01->Color=clBtnFace;
            pl_ATCSelfTestSatus_02->Color=clBtnFace;
            pl_ATCSelfTestSatus_03->Color=clBtnFace;
            pl_ATCSelfTestSatus_99->Color=clBtnFace;
            if(FormHS->Visible==true)
                Close();
        }
        if(FormHS->Visible==true &&                                             //Ifor 20160908 避免畫面重整時被蓋掉看不到
           fNote->fShow==false &&                                               //Steven 20190714 : Fixed for Self test.
           MyMessageBox->fShow==false)
        {
            FormHS->BringToFront();
        }
    }
    else
    {
        iATCSelfTest=0;
    }

    if(CosFunction.bSaveProductionLogByUnloaderTray &&
       IniConfig.bN10_9_UploadUnloadTrayToFTP==true)                            //Steven 20200330 : production log by unloader tray存檔
    {
        if(fMain->UnloadTrayLog->Count!=0)
        {
            TStringList *List=new TStringList();
            List->CommaText=fMain->UnloadTrayLog->Strings[0];
            if(List->Count==2)                                                  //Steven 20200518 : 修正FTP上傳檔案的路徑
            {
                UpDataToServerByFTP(List->Strings[0], List->Strings[1], "UnloaderLog");
            }
            fMain->UnloadTrayLog->Delete(0);
            List->Clear();
            delete List;
        }
    }

//    if(bNeedUpload_N10==true)
//    {
//        bNeedUpload_N10=false;
//        SendCommand_EventLog(EL_UPLOAD_BYFILE_N10, "1");
//    }

    if(CUSTOMER_CODE==CC_SCC &&                                                 //Steven 20200409 : JSCC RTM功能
       IniConfig.bN24_EnableRTM)
    {
        try
        {
            if(RTMServerSocket->Port!=IniConfig.iN24_RTMPort)
            {
                RTMServerSocket->Close();
                RTMServerSocket->Port==IniConfig.iN24_RTMPort;
                bResetRTMConnect=true;
            }

            if(bResetRTMConnect==true)
            {
                bResetRTMConnect=false;
                RTMServerSocket->Close();
                RTMServerSocket->Open();
            }
        }
        catch(...)
        {
            MyDBIProcess("Exception", "TFormHS::TimerAutoBackupTimer");
        }

        if(fMain->RTMCommand->Count!=0)
        {
            str=fMain->RTMCommand->Strings[0];
            fMain->RTMCommand->Delete(0);
            if(str.AnsiPos("STOP")!=0)
            {
                bHaltHandler=true;
                RTMStr=str;
                MyDBIProcess("RTM", str);
            }
            else if(str.AnsiPos("RESUME")!=0)
            {
                bHaltHandler=false;
                RTMStr="";
                MyDBIProcess("RTM", str);
                if(MyMessageBox->fShow)
                {
                    MyMessageBox->pnlPauseClick(this);
                }
            }
        }
        else
        {

        }
    }

    if(IniConfig.bN10_DailyUploadProdData &&                                    //Steven 20250809 : 換位子
       IniConfig.iN10UploadProductMethod==3)                                    //Jimmychiu 20250912 : CYUEAN wants to upload the log at a specified time.
    {
        unsigned short hour, minute, second, millisecond;
        TDateTime dtPickerTime(IniConfig.dN10_3_1_SpecifiedTime);
        unsigned short specifiedhour, specifiedminute;
        DecodeTime(dtPickerTime, specifiedhour, specifiedminute, second, millisecond);
        DecodeTime(Now(), hour, minute, second, millisecond);
        if(cMathTool().IsDoubleEqual(specifiedhour,hour) &&
           cMathTool().IsDoubleEqual(specifiedminute,minute))
        {
            if(bN10_3_1_Flag==false)
            {
                UpDataToServer_KYEC(5);
                bN10_3_1_Flag=true;
            }
        }
        else
        {
            bN10_3_1_Flag=false;
        }
    }
    bTimerRunning=false;
}
//---------------------------------------------------------------------------
void TFormHS::CalculateUploadToHostIntervalTimeFunction()                       //Ifor 20160302 Add FTP Upload Log To Host start
{
    if(IniConfig.bN10Enable_FTPUpLoadLog==false ||
       IniConfig.iN10UploadToHostIntervalTime==0)
        return;

    static clock_t ctStart=clock();
    clock_t ctEnd=clock();
    static bool bFirst=true;

    if(bFirst)
    {
        bFirst=false;
        ctStart=clock();
    }

    int iSec=IniConfig.iN10UploadToHostIntervalTime;
    if(iSec<=5)
        iSec=5;
    int iIntervalMSec=iSec*1000;

    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                         //kevin 20191213 add ESD send HISI
    {
        if(ctEnd-ctStart>=iIntervalMSec)
        {
            bFirst=true;
            RecordESDLog_HS("");
        }
        return;
    }
    else if(IniConfig.bN10Enable_FTPUpLoadLog==false ||
            IniConfig.iN10UploadToHostIntervalTime==0)
    {
        return;
    }

    if(ctEnd-ctStart>=iIntervalMSec)
    {
        bFirst=true;
        if(CosFunction.bUseLogUploadToFTPFunction==true)
        {
            RecordLog_HS();
        }

        if(CUSTOMER_CODE==CC_KYEC_LEE)
        {
            if(ATC_SYSTEM!=eATCUninstall && ATC_SYSTEM!=eNonChamber)            //Ifor 20160504 客戶要求非ATC機台不上報Secs Gem 資料
            {
                if(IniConfig.bEnable_SECS_GEM==true &&                          //Ifor 20160329 KYEC 要求 Secs Gem Event僅留80即可，並調整為Lot Start後才開始上報
                   bSysLotStart==true &&
                   bSECSGEMConnectionFail==false)                               //Ifor 20170607 (wei) add secs gem
                {
                    EventReport(SECS_EVENT.ReadNowHandlerData);
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormHS::RecordLog_HS(bool bAnyWaySend)                         //ChungHung 20151117 modify for KYEC_HS start
{
    TDateTime dtPresent=Now();
    static TDateTime dtBefore=dtPresent;
    if(bSysLotStart==true)
    {
        if(bEPLogStart_KYEC)                                                    //Ifor 20160329 精簡 HISI FTP Log 流程
        {
            sEPLogFile=GetLastFileLogName_HS(0);                                //iType 0:EP 1:Temperature 2:ESD
            RecordEPLog_HS(sEPLogFile);
        }

        if(bTempLogStart_KYEC)
        {
            sTemperatureLogFile=GetLastFileLogName_HS(1);
            RecordTemperatureLog_HS(sTemperatureLogFile);
        }

        if(bESDLogStart_KYEC)
        {
            sESDLogFile=GetLastFileLogName_HS(2);
            RecordESDLog_HS(sESDLogFile);
        }

        if((ATC_SYSTEM==eATCHonPrecType ||
            ATC_SYSTEM==eNewATCSystem) &&
           Temperature.bATCActiveCooling==true)                                 //Ifor 20161202 add 取得 ATC EvenLog 檔案名稱
        {
            asATCEvenLogFile=GetLastFileLogName_HS(3);
            RecordATCEvenLog_HS(asATCEvenLogFile);
        }
    }
    else if((SystemStart==false && bSysLotStart==false) || bAnyWaySend)
    {
        if(IniConfig.bN10Enable_FTPUpLoadLog==true)                             //上傳
        {
            if(bEPLogEnd_KYEC ||
               bTempLogEnd_KYEC ||
               bESDLogEnd_KYEC ||
               bATCEvenLogEnd_KYEC ||
               bArmTestInfoEvenLogEnd_KYEC ||
               bAnyWaySend)                                                     //JerryYang 20190426 fix server上傳問題
            {
                UpDataToServer_KYEC(0, bAnyWaySend);                            //iLog 0: temperature 1: EP //ChungHung 20151117 modify for KYEC_HS
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormHS::ClearLastFileLogName_HS(int iType)                     //iType 0: EP 1: temperature 2:ESD 3:矽品使用網路芳鄰 4:ATC Even
{
    try
    {
        AnsiString sRecord_FilePath="D:\\HandlerLog\\LogFileName_Record.ini";

        if(iType==-1)
        {
            tHiSiRecord->Clear();
            tHiSiRecord->Add("[LastFileName]");
            tHiSiRecord->Add("EP_Log=NULL");
            tHiSiRecord->Add("Temperature_Log=NULL");
            tHiSiRecord->Add("ESD_Log=NULL");
            tHiSiRecord->Add("ATCEven_Log=NULL");
            tHiSiRecord->Add("ArmTestInfoEvent_Log=NULL");                      //Ifor 20190912 :add 海思 V02.30 版 Record Torque
            if(CosFunction.bUseChangeLogByLot)
            {
                tHiSiRecord->Add("ChangeEvent_Log=NULL");                       //Ifor 20191002 : add Change Log By Lot
            }
        }

        switch(iType)
        {
            case 0:
                tHiSiRecord->Strings[1]="EP_Log=NULL";
                break;
            case 1:
                tHiSiRecord->Strings[2]="Temperature_Log=NULL";
                break;
            case 2:
                tHiSiRecord->Strings[3]="ESD_Log=NULL";
                break;
            case 3:
                tHiSiRecord->Strings[1]="EP_Log=NULL";
                tHiSiRecord->Strings[2]="Temperature_Log=NULL";
                tHiSiRecord->Strings[3]="ESD_Log=NULL";
                tHiSiRecord->Strings[4]="ATCEven_Log=NULL";
                tHiSiRecord->Strings[5]="ArmTestInfoEvent_Log=NULL";
                if(CosFunction.bUseChangeLogByLot)
                {
                    tHiSiRecord->Strings[6]="ChangeEvent_Log=NULL";
                }
                break;
            case 4:
                tHiSiRecord->Strings[4]="ATCEven_Log=NULL";
                break;
            case 5:                                                             //Ifor 20190912 :add 海思 V02.30 版 Record Torque
                tHiSiRecord->Strings[5]="ArmTestInfoEvent_Log=NULL";
                break;
            case 6:                                                             //Ifor 20191002 : add Change Log By Lot
                tHiSiRecord->Strings[6]="ChangeEvent_Log=NULL";
                break;
        }
        tHiSiRecord->SaveToFile(sRecord_FilePath);                              //Steven 20160604 : add protect of fopen
    }
    catch(...)
    {

        RecordProcess("Exception of ClearLastFileLogName_HS");
    }
}
//---------------------------------------------------------------------------
AnsiString __fastcall TFormHS::GetLastFileLogName_HS(int iType)                 //iType 0:EP 1:Temperature 2:ESD 3:ATCEvent 5:ArmTestInfoEvent_Log 6:ChangeEvent_Log
{
    AnsiString sRecord_FilePath="D:\\HandlerLog\\LogFileName_Record.ini";
    AnsiString asLastFileName="NULL", aFileName="";

    try
    {
        switch(iType)
        {
            case 0:
                if(CheckIniData(sRecord_FilePath, "LastFileName", "EP_Log"))
                {
                    asLastFileName=ReadIniData(sRecord_FilePath, "LastFileName", "EP_Log", AnsiString("NULL"));
                }

                if(asLastFileName=="NULL")
                {
                    if(IniConfig.bSPILFunction==true)                           //Ifor 20160408 矽品需求一天一檔案
                    {                                                           //EP-287__20151120.Log //EP_機台號碼_日期.Log
                        aFileName.sprintf("EP_%s_%s", IniConfig.SocketHandlerID, GetDateInfoByString());
                    }
                    else
                    {                                                           //EP-287_LFB0985AR1_20151120112230.Log
                        aFileName.sprintf("EP_%s_%s_%s", IniConfig.SocketHandlerID, asATCEvenLotID, GetDateInfoByString() + GetOnlyTimeInfoByString() );    //Ifor 20170125 修改變數名稱避免Tray Feed 後元件資料被清空 fLotInfo->edtSysLotID->Text =>asATCEvenLotID
                    }
                    WriteIniData( sRecord_FilePath, "LastFileName", "EP_Log", aFileName);
                }
                else
                {
                    aFileName=asLastFileName;
                }
                break;
            case 1:
                if(CheckIniData(sRecord_FilePath, "LastFileName", "Temperature_Log"))
                {
                    asLastFileName=ReadIniData(sRecord_FilePath, "LastFileName", "Temperature_Log", AnsiString("NULL"));
                }

                if(asLastFileName=="NULL")
                {
                    if(IniConfig.bSPILFunction==true)                           //Ifor 20160408 矽品需求一天一檔案
                    {                                                           //Temperature_HT-287__20151120.Log //Temperature_機台號碼_日期.Log
                        aFileName.sprintf("Temperature_%s_%s", IniConfig.SocketHandlerID, GetDateInfoByString());
                    }
                    else
                    {                                                           //Temperature_HT-287_LFB0985AR1_20151120112230.Log
                        aFileName.sprintf("Temperature_%s_%s_%s", IniConfig.SocketHandlerID, asATCEvenLotID, GetDateInfoByString() + GetOnlyTimeInfoByString() );   //Ifor 20170125 (Steven) 修改變數名稱避免Tray Feed 後元件資料被清空 fLotInfo->edtSysLotID->Text =>asATCEvenLotID
                    }
                    WriteIniData(sRecord_FilePath, "LastFileName", "Temperature_Log", aFileName);
                }
                else
                {
                    aFileName=asLastFileName;
                }
                break;
            case 2:
                if(CheckIniData(sRecord_FilePath, "LastFileName", "ESD_Log"))
                {
                    asLastFileName=ReadIniData(sRecord_FilePath, "LastFileName", "ESD_Log", AnsiString("NULL"));
                }

                if(asLastFileName=="NULL")
                {
                    if(IniConfig.bSPILFunction==true)                           //Ifor 20160408 矽品需求一天一檔案
                    {                                                           //ESD_HT-287__20151120.Log //Temperature_機台號碼_日期.Log
                        aFileName.sprintf("ESD_%s_%s", IniConfig.SocketHandlerID, GetDateInfoByString());
                    }
                    else
                    {                                                           //ESD_HT-287_LFB0985AR1_20151120112230.Log
                        aFileName.sprintf("ESD_%s_%s_%s", IniConfig.SocketHandlerID, asATCEvenLotID, GetDateInfoByString() + GetOnlyTimeInfoByString() );   //Ifor 20170125 修改變數名稱避免Tray Feed 後元件資料被清空 fLotInfo->edtSysLotID->Text =>asATCEvenLotID
                    }
                    WriteIniData(sRecord_FilePath, "LastFileName", "ESD_Log", aFileName);
                }
                else
                {
                    aFileName=asLastFileName;
                }
                break;
            case 3:
                if(CheckIniData(sRecord_FilePath, "LastFileName", "ATCEven_Log"))
                {
                    asLastFileName=ReadIniData(sRecord_FilePath, "LastFileName", "ATCEven_Log", AnsiString("NULL"));
                }

                if(asLastFileName=="NULL")
                {                                                               //ESD_HT-287_LFB0985AR1_20151120112230.Log
                    aFileName.sprintf("ATCEven_%s_%s_%s", IniConfig.SocketHandlerID, asATCEvenLotID, GetDateInfoByString()+GetOnlyTimeInfoByString());   //Ifor 20170125 (Steven) 修改變數名稱避免Tray Feed 後元件資料被清空 fLotInfo->edtSysLotID->Text =>asATCEvenLotID
                    WriteIniData(sRecord_FilePath, "LastFileName", "ATCEven_Log", aFileName);
                }
                else
                {
                    aFileName=asLastFileName;
                }
                break;
            case 5:                                                             //Ifor 20190912 :add 海思 V02.30 版 Record Torque
                if(CheckIniData(sRecord_FilePath, "LastFileName", "ArmTestInfoEvent_Log"))
                {
                    asLastFileName = ReadIniData(sRecord_FilePath, "LastFileName", "ArmTestInfoEvent_Log", AnsiString("NULL"));
                }

                if(asLastFileName=="NULL")
                {
                    aFileName.sprintf("ArmTestInfoEvent_%s_%s_%s", IniConfig.SocketHandlerID, asATCEvenLotID, GetDateInfoByString()+GetOnlyTimeInfoByString());
                    WriteIniData(sRecord_FilePath, "LastFileName", "ArmTestInfoEvent_Log", aFileName);
                }
                else
                {
                    aFileName=asLastFileName;
                }
                break;
            case 6:                                                             //Ifor 20191002 : add Change Log By Lot
                if(CheckIniData(sRecord_FilePath, "LastFileName", "ChangeEvent_Log"))
                {
                    asLastFileName=ReadIniData(sRecord_FilePath, "LastFileName", "ChangeEvent_Log", AnsiString("NULL"));
                }

                if(asLastFileName=="NULL")
                {
                    aFileName.sprintf("ChangeEvent_%s_%s_%s", IniConfig.SocketHandlerID, asATCEvenLotID, GetDateInfoByString()+GetOnlyTimeInfoByString());
                    WriteIniData(sRecord_FilePath , "LastFileName", "ChangeEvent_Log", aFileName);
                }
                else
                {
                    aFileName=asLastFileName;
                }
                break;
            case 7:                                                             //Ifor 20191002 : add Change Log By Lot
                if(CheckIniData(sRecord_FilePath , "LastFileName", "ChangeEvent_Log") )
                {
                    asLastFileName=ReadIniData(sRecord_FilePath , "LastFileName", "ChangeEvent_Log", AnsiString("NULL"));
                }

                if(asLastFileName=="NULL")
                {
                    aFileName.sprintf("ChangeEvent_%s_%s_%s", IniConfig.SocketHandlerID, asATCEvenLotID, GetDateInfoByString()+GetOnlyTimeInfoByString());
                    WriteIniData(sRecord_FilePath , "LastFileName", "ChangeEvent_Log", aFileName);
                }
                else
                {
                    aFileName=asLastFileName;
                }
                break;
        }
        aFileName+=".csv";
    }
    catch(...)
    {
        RecordProcess("Exception of GetLastFileLogName_HS");
    }
    return aFileName;
}
//---------------------------------------------------------------------------
extern int iTestHeadMotorTask;
void __fastcall TFormHS::RecordEPLog_HS(AnsiString aFileName)
{
    AnsiString sRecord;
    double dTempKg=0.0, dValue=0.0, dDualTempKg=0.0;
    int iTempPA=0, iDualTempPA=0;
    int iType=0;
    AnsiString strKg, strDualKg;
    AnsiString strADAM_STATUS, strDualADAM_STATUS, strTester_STATUS;

    AnsiString sFullFileName=asEPLog_HS+aFileName;

    if(FileExists(sFullFileName)==false)
    {
        GetTitleName(sRecord, eReocrd_EP);
        WriteDataToFile(sFullFileName.c_str(), sRecord.c_str());
    }
    sRecord="";
    AnsiString sTime=GetDateInfoByString("/")+" "+GetOnlyTimeInfoByString(":");

    iTempPA=ADAM_ReadPA(&dValue);                                               //JerryYang 20171030 (wei) EP log公斤數記錄到小數點第二位
    dTempKg=KpaTransferKG(iTempPA);                                             //Ifor 20160303 add KYEC EP Up Load Log Data
    if(ADAM_Alarm())
    {
        strADAM_STATUS=",Alarm";
    }
    else
    {
        strADAM_STATUS=",Pass";
    }

    if(INSTALL_DOUBLE_EP==true)
    {
        iDualTempPA=ADAM_ReadPA(&dValue,2);                                     //Ifor 20221215 add:讀取Dual EP
        dDualTempKg=KpaTransferKG(iDualTempPA, true);                           //Ifor 20221215 add:轉換Dual EP 公斤數

        if(bEnable_KLT_Function==true)
        {
            iType=1;
        }
        else
        {
            iType=0;
        }

        if(ADAM_DualAlarm(iType))
        {
            strDualADAM_STATUS=",Alarm";
        }
        else
        {
            strDualADAM_STATUS=",Pass";
        }

        strDualKg.sprintf("%.2f", dDualTempKg);
    }

    double fComplianceUnit=1.0;
    if(bEnable_KLT_Function==true)                                              //Steven 20220610 : KLT羅昱良要求改用By SITE FORCE
    {
        switch(DeviceForm_File.iHeadDeviceCT)
        {
            case 2:                                                             // 1 Device with 1 Compliance Unit
                fComplianceUnit=1.0;    break;
            case 3:                                                             // 2 Device with 1 Compliance Unit
                fComplianceUnit=0.5;    break;
            case 4:                                                             // 4 Device with 1 Compliance Unit
                fComplianceUnit=0.25;   break;
            case 5:                                                             // 2 Device with 4 Compliance Unit
                fComplianceUnit=2.0;    break;
            case 6:                                                             // 8 Device with 1 Compliance Unit
                fComplianceUnit=0.125;  break;
        }
        strKg.sprintf("%.2f", dTempKg*fComplianceUnit);
    }
    else
    {
        strKg.sprintf("%.2f", dTempKg);
    }

    if(LastSet.iTester==ON_LINE)
    {
        strTester_STATUS=",OnLine";
    }
    else
    {
        strTester_STATUS=",OffLine";
    }

    if(bEnable_KLT_Function==true)                                              //Steven 20220610 : KLT羅昱良要求改用By SITE FORCE
    {
        sRecord=sTime+","+fContact->edForcePerDeviceKG->Text+","+AnsiString(iTempPA)+","+strKg;
        if(IndexStatus==Z1Down_Z2Up)                                            //ARM1 在下
        {
            sRecord=sTime+","+"ARM1,"+fContact->edForcePerDeviceKG->Text+","+AnsiString(iTempPA)+","+strKg+strADAM_STATUS+strTester_STATUS;
        }
        else if(IndexStatus==Z1Up_Z2Down)                                       //ARM2 在下
        {
            sRecord=sTime+","+"ARM2,"+fContact->edForcePerDeviceKG->Text+","+AnsiString(iTempPA)+","+strKg+strADAM_STATUS+strTester_STATUS;
        }
        else
        {
            sRecord=sTime+","+"None,"+fContact->edForcePerDeviceKG->Text+","+AnsiString(iTempPA)+","+strKg+strADAM_STATUS+strTester_STATUS;
        }

        if(INSTALL_DOUBLE_EP==true)
        {
//            sRecord=sRecord+","+fContact->edDoubleForce->Text+","+AnsiString(iDualTempPA)+","+strDualKg;
            if(IndexStatus==Z1Down_Z2Up)                                        //ARM1 在下
            {
                sRecord=sRecord+","+fContact->edDoubleForce->Text+","+AnsiString(iDualTempPA)+","+strDualKg+strDualADAM_STATUS;
            }
            else if(IndexStatus==Z1Up_Z2Down)                                   //ARM2 在下
            {
                sRecord=sRecord+","+fContact->edDoubleForce->Text+","+AnsiString(iDualTempPA)+","+strDualKg+strDualADAM_STATUS;
            }
            else
            {
                sRecord=sRecord+","+fContact->edDoubleForce->Text+","+AnsiString(iDualTempPA)+","+strDualKg+strDualADAM_STATUS;
            }
        }
        else
        {
            sRecord=sRecord+",NA,NA,NA,NA";
        }
    }
    else
    {
        sRecord=sTime+","+fContact->edSetKg->Text+","+AnsiString(iTempPA)+","+strKg;
        if(IndexStatus==Z1Down_Z2Up)                                            //ARM1 在下
        {
            sRecord=sTime+","+"ARM1,"+fContact->edSetKg->Text+","+AnsiString(iTempPA)+","+strKg+strADAM_STATUS+strTester_STATUS;
        }
        else if(IndexStatus==Z1Up_Z2Down)                                       //ARM2 在下
        {
            sRecord=sTime+","+"ARM2,"+fContact->edSetKg->Text+","+AnsiString(iTempPA)+","+strKg+strADAM_STATUS+strTester_STATUS;
        }
        else
        {
            sRecord=sTime+","+"None,"+fContact->edSetKg->Text+","+AnsiString(iTempPA)+","+strKg+strADAM_STATUS+strTester_STATUS;
        }

        if(INSTALL_DOUBLE_EP==true)
        {
//            sRecord=sRecord+","+fContact->edDoubleForce->Text+","+AnsiString(iDualTempPA)+","+strDualKg;
            if(IndexStatus==Z1Down_Z2Up)                                        //ARM1 在下
            {
                sRecord=sRecord+","+fContact->edDoubleForce->Text+","+AnsiString(iDualTempPA)+","+strDualKg+strDualADAM_STATUS;
            }
            else if(IndexStatus==Z1Up_Z2Down)                                   //ARM2 在下
            {
                sRecord=sRecord+","+fContact->edDoubleForce->Text+","+AnsiString(iDualTempPA)+","+strDualKg+strDualADAM_STATUS;
            }
            else
            {
                sRecord=sRecord+","+fContact->edDoubleForce->Text+","+AnsiString(iDualTempPA)+","+strDualKg+strDualADAM_STATUS;
            }
        }
        else
        {
             sRecord=sRecord+",NA,NA,NA,NA";
        }
    }

//    if(CosFunction.bHiSiliconFunction==true ||                                //Steven 20240925 : mark for 上傳Server資料增加Lot ID欄位
//       CUSTOMER_CODE==CC_KYEC_LEE)                                            //Ifor 20190905 :add 海思 V02.30 版  上傳Server資料增加Lot ID欄位
    {
        sRecord=sRecord+","+sHisiAddTempLog(false);                             //kevin 20191016 ASE KH HIS add Lot LOG no temp
    }
    WriteDataToFile(sFullFileName.c_str(), sRecord.c_str());

    if(bEnableAutoMotive==true)                                                 //Ifor 20221024 add: KLT Auto Motive Function
    {                                                                           //Steven 20240516 : 換位置, 先紀錄再alarm
        RecordAutoMotive("\\EP_Log", aFileName, sRecord, eReocrd_EP);
    }

#ifndef SOFT_SIMULTE
    if(SystemStart &&
       iContactMode==CONTACT_NORMAL &&
       iTestHeadMotorTask==600 &&
       bRunAutoClean==false &&
       fContact->fShow==false)                                                  //JerryYang 20190115 (Steven) 避免EP check誤發alarm
    {
        if(LastSet.iRealDummy==REALLY)                                          //JerryYang 20181129 (Steven) : REAL模式才發alarm
        {
            if(IniConfig.bD24EnableEPCheckFuntion)
            {
                if(ADAM_Alarm())                                                //Steven 20190114 : EP Alarm換位置
                {
                    ShowErrorMessage("WAR1605", K_SKIP, MMSystem, "TfContact::RecordEPLog_HS");  //"請檢查EP是否漏氣!"    //jou 20171120 (Steven) : 修正EP alarm位置錯誤
                }
            }

            if(IniConfig.bD26_3EnableDualEPEncoderRange)
            {
                if(INSTALL_DOUBLE_EP==true)
                {
                    if(ADAM_DualAlarm(iType))
                    {
                        ShowErrorMessage("WAR1610", K_SKIP, MMSystem);
                    }
                }
            }
        }
    }
#endif
}
//---------------------------------------------------------------------------
void __fastcall TFormHS::RecordTemperatureLog_HS(AnsiString aFileName)
{
    AnsiString sFullFileName=asTempLog_HS+aFileName;

    AnsiString sRecord="", sTemp;
    if(FileExists(sFullFileName)==false)
    {
        if(CUSTOMER_CODE==CC_KYEC_LEE)
            sRecord=GetTempUseName(1);
        else
            sRecord=GetTempUseName(0);
        WriteDataToFile(sFullFileName, sRecord);
    }
    sTemp=GetTempUseNamevalue(1);

    WriteDataToFile(sFullFileName, sTemp);
    if(bEnableAutoMotive==true)                                                 //Ifor 20221024 add: KLT Auto Motive Function
    {
        RecordAutoMotive("\\Temperature_Log", aFileName, sTemp, eReocrd_Temp);
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormHS::RecordESDLog_HS(AnsiString aFileName)
{
    static TPanel *ESDPtr[]={fLotInfo->pl_ESDProx1_1, fLotInfo->pl_ESDProx1_2, fLotInfo->pl_ESDProx1_3,
                             fLotInfo->pl_ESDProx2_1, fLotInfo->pl_ESDProx2_2, fLotInfo->pl_ESDProx2_3,
                             fLotInfo->pl_ESDProx3_1, fLotInfo->pl_ESDProx3_2, fLotInfo->pl_ESDProx3_3};

    AnsiString sDirPath=AnsiString("D:\\HandlerLog"), sRecord, sHisFile;

    sHisFile.sprintf("%s\\%04d%02d\\", asESDLogPath, SystemYear, SystemMonth);  //kevin 20191031
    AnsiString sESDCopyPath=asESDLogPath+"\\Current\\";
    AnsiString sESDCopyFileName=sESDCopyPath;                                   //kevin 20191210 add

    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                         //kevin 20191212 add   "D:\\HT9045_Log";
    {
         aFileName.sprintf("ESD_%s_%s", IniConfig.SocketHandlerID, GetDateInfoByString());   //Ifor 20170125 修改變數名稱避免Tray Feed 後元件資料被清空 fLotInfo->edtSysLotID->Text =>asATCEvenLotID
         sDirPath=asESDLogPath;
    }

    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                         //kevin 20191212 add   "D:\\HT9045_Log";
    {
        sDirPath=sHisFile;
    }
    else
    {
        sDirPath=sDirPath+"\\ESD_Log\\";
    }

    MyForceDirectories(sDirPath, "TFormHS::RecordESDLog_HS");

    AnsiString sFullFileName=sDirPath;

    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                         //kevin 20191213
    {
        beKeepLotNumber();                                                      //kevin 20191018 ASE KH read lot ID LOG
        sHisFile=sDirPath+"ASE_ESD_"+sHandleID+"_"+sInsertion+"_"+sDeviceType+"_"+sTestProgram+"_"+sHiLotID+"_"+sOSATLotID+"_"+ GetDateInfoByString()+".csv";
        sESDCopyFileName=sESDCopyFileName+"ASE_ESD_"+sHandleID+"_"+sInsertion+"_"+sDeviceType+"_"+sTestProgram+"_"+sHiLotID+"_"+sOSATLotID+"_"+GetDateInfoByString()+".csv";
        sFullFileName=sFullFileName+aFileName+ ".csv";
    }
    else
    {
        sFullFileName=sFullFileName+aFileName;
    }

    if(FileExists(sFullFileName)==false || FileExists(sHisFile)==false)
    {                                                                           //Ifor 20160314 ESD Log 格式變更
        sRecord ="Ver:"+asHandlerVersion+"\n";                                  //kevin 20191212 add ver
        sRecord =   sRecord+
                    "Time"                  +","+
                    "EnableAlarm"           +","+
                    "AlarmContinuousTimes"  +","+
                    "AlarmOccurrence"       +","+
                    "ScanInterval"          +","+
                    "Station1_Status"       +","+
                    "Station1_1_Status"     +","+
                    "Station1_1_Setting"    +","+
                    "Station1_1_Value"      +","+
                    "Station1_2_Status"     +","+
                    "Station1_2_Setting"    +","+
                    "Station1_2_Value"      +","+
                    "Station1_3_Status"     +","+
                    "Station1_3_Setting"    +","+
                    "Station1_3_Value"      +","+
                    "Station2_Status"       +","+
                    "Station2_1_Status"     +","+
                    "Station2_1_Setting"    +","+
                    "Station2_1_Value"      +","+
                    "Station2_2_Status"     +","+
                    "Station2_2_Setting"    +","+
                    "Station2_2_Value"      +","+
                    "Station2_3_Status"     +","+
                    "Station2_3_Setting"    +","+
                    "Station2_3_Value"      +","+
                    "Station3_Status"       +","+
                    "Station3_1_Status"     +","+
                    "Station3_1_Setting"    +","+
                    "Station3_1_Value"      +","+
                    "Station3_2_Status"     +","+
                    "Station3_2_Setting"    +","+
                    "Station3_2_Value"      +","+
                    "Station3_3_Status"     +","+
                    "Station3_3_Setting"    +","+
                    "Station3_3_Value"      +","+
                    "Station4_Status"       +","+
                    "Station4_1_Status"     +","+
                    "Station4_1_Setting"    +","+
                    "Station4_1_Value"      +","+
                    "Station4_2_Status"     +","+
                    "Station4_2_Setting"    +","+
                    "Station4_2_Value"      +","+
                    "Station4_3_Status"     +","+
                    "Station4_3_Setting"    +","+
                    "Station4_3_Value"      +","+
                    "Station5_Status"       +","+
                    "Station5_1_Status"     +","+
                    "Station5_1_Setting"    +","+
                    "Station5_1_Value"      +","+
                    "Station5_2_Status"     +","+
                    "Station5_2_Setting"    +","+
                    "Station5_2_Value"      +","+
                    "Station5_3_Status"     +","+
                    "Station5_3_Setting"    +","+
                    "Station5_3_Value"      +","+
                    "Station6_Status"       +","+
                    "Station6_1_Status"     +","+
                    "Station6_1_Setting"    +","+
                    "Station6_1_Value"      +","+
                    "Station6_2_Status"     +","+
                    "Station6_2_Setting"    +","+
                    "Station6_2_Value"      +","+
                    "Station6_3_Status"     +","+
                    "Station6_3_Setting"    +","+
                    "Station6_3_Value"      ;

//        if(CosFunction.bHiSiliconFunction==true ||                            //Ifor 20190905 :add 海思 V02.30 版  上傳Server資料增加Lot ID欄位
//           CUSTOMER_CODE==CC_KYEC_LEE ||                                      //Steven 20240925 : mark for 上傳Server資料增加Lot ID欄位
//           CUSTOMER_CODE==CC_ASE_KaohSiung)                                   //kevin 20191212 add
        {
            if(CUSTOMER_CODE==CC_KYEC_LEE)
                sRecord=sRecord+","+"Lot ID";
            else
                sRecord=sRecord+",Lot ID, OSAT Lot number " ;                   //kevin 20191210 add
        }

        if(FileExists(sFullFileName)==false)                                    //kevin 20191212
            WriteDataToFile(sFullFileName, sRecord);

        if(FileExists(sHisFile)==false)                                         //kevin 20191212
        {
            if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                 //kevin 20191210 add HISI log file name
            {
                Del_Tree(sESDCopyPath);
                bBuildFilter(sESDCopyPath,asESDLogPath);
                WriteDataToFile(sHisFile, sRecord);
            }
        }
    }

    if(sESDLogData=="" || sESDLogData=="NULL")
        return;

    AnsiString sTime=GetDateInfoByString("/")+" "+GetOnlyTimeInfoByString(":");
    sRecord=sTime+","+sESDLogData;

    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                         //kevin 20191212
    {
        beKeepLotNumber();                                                      //kevin 20191018 ASE KH lot ID LOG
        sRecord = sRecord+sHisiAddTempLog(false);                               //kevin 20191016 ASE KH HIS add Lot LOG no temp
        WriteDataToFile(sHisFile, sRecord);
    }
    else //if(CosFunction.bHiSiliconFunction==true ||                           //Steven 20240925 : mark for 上傳Server資料增加Lot ID欄位
         //   CUSTOMER_CODE==CC_KYEC_LEE)                                       //Ifor 20190905 :add 海思 V02.30 版  上傳Server資料增加Lot ID欄位
    {
        sRecord=sRecord+","+asATCEvenLotID;
    }

    WriteDataToFile(sFullFileName, sRecord);
//    if(CosFunction.bHiSiliconFunction &&                                      //Steven 20240925 : mark for 上傳Server資料增加Lot ID欄位
//       CUSTOMER_CODE==CC_ASE_KaohSiung)                                       //kevin 201912010  copy 海思檔案到上傳路徑
    {
        MyForceDirectories(sESDCopyPath, "TFormHS::RecordESDLog_HS");
        CopyFile(sHisFile.c_str(), sESDCopyFileName.c_str(), FALSE);
    }
    sESDLogData="";
    if(bEnableAutoMotive==true)                                                 //Ifor 20221024 add: KLT Auto Motive Function
    {
        RecordAutoMotive("\\ESD_Log", aFileName, sRecord, eReocrd_ESD);
    }
}
//------------------------------------------------------------------------------
void __fastcall TFormHS::RecordATCEvenLog_HS(AnsiString aFileName)              //Ifor 20161202 add Record ATC Even Log
{
    if(bEnableAutoMotive==true)                                                 //Ifor 20221024 add: KLT Auto Motive Function
    {
        RecordAutoMotive("\\ATCEven_Log", aFileName, "", eReocrd_ATCEven);
    }
}
//------------------------------------------------------------------------------
void __fastcall TFormHS::RecordArmTestInfoLog_HS(AnsiString aFileName)          //Ifor 20190912 :add 海思 V02.30 版 Record Torque
{
    AnsiString sRecord="";
    AnsiString sFullFileName=asArmTestLog_HS+aFileName;

    if(FileExists(sFullFileName)==false)
    {
        sRecord="Time,TestArm,Torque(%),Auto Height(mm),Contact Offset(mm),Lot ID";

        WriteDataToFile(sFullFileName.c_str(), sRecord.c_str());
    }
    sRecord="";
    AnsiString sTime=GetDateInfoByString("/")+" "+GetOnlyTimeInfoByString(":");

    if(IndexStatus==Z1Down_Z2Up)                                                //ARM1 在下
    {
        sRecord=sTime+","+"ARM1"+","+fMain->edTorue0->Text+","+DeviceForm_File.IndexContact[0]+","+Offset_File.iIndexArmContact[0]+","+asATCEvenLotID;
    }
    else  if(IndexStatus==Z1Up_Z2Down)                                          //ARM2 在下
    {
        sRecord=sTime+","+"ARM2"+","+fMain->edTorue1->Text+","+DeviceForm_File.IndexContact[1]+","+Offset_File.iIndexArmContact[1]+","+asATCEvenLotID;
    }
    WriteDataToFile(sFullFileName.c_str(), sRecord.c_str());
    if(bEnableAutoMotive==true)                                                 //Ifor 20221024 add: KLT Auto Motive Function
    {
        RecordAutoMotive("\\ArmTestInfo", aFileName, sRecord, eReocrd_TestArm);
    }
}
//------------------------------------------------------------------------------
void __fastcall TFormHS::RecordChangeLogByLot(AnsiString S, AnsiString S2)      //Ifor 20191002 : add Change Log By Lot
{
    AnsiString sFullFileName="", sRecord="";

    sChangeEvenLogFile=GetLastFileLogName_HS(6);
    sFullFileName=asChangeLogByLotPath+"\\"+sChangeEvenLogFile;;

    if(FileExists(sFullFileName)==false)
    {
        sRecord="Time,Record Message,Value";
        WriteDataToFile(sFullFileName.c_str(), sRecord.c_str());
    }

    AnsiString sTime=GetDateInfoByString("/")+" "+GetOnlyTimeInfoByString(":");
    sRecord=sTime+","+S+","+S2;

    WriteDataToFile(sFullFileName.c_str(), sRecord.c_str());
}
//------------------------------------------------------------------------------
typedef struct
{
    AnsiString sJamCode;
    AnsiString sMessage;
    AnsiString sPart;
    int iStopTime;
    int iJamCount;
}_MyMsgStruct;

//------------------------------------------------------------------------------
//iLog
//  0: EP
//  1: temperature
//  2: ESD
//  3: 矽品使用網路芳鄰
//  4: ATC Event
//  5: EventLog, Jam統計表, MTBF, MUBF資料
//  6: Power_Saving
//  7: ATC Power_Saving
//------------------------------------------------------------------------------
int __fastcall TFormHS::UpDataToServer_KYEC(int iLog, bool bForceSend)          //JerryYang 20190426 fix server上傳問題
{
    AnsiString asDirPath;
    AnsiString asFileName, asFullName, aEveltLogFileName;
    AnsiString str, str1;
//    AnsiString asDataDirection;                                                 //Ifor 20170913 (Steven) add 新增網路芳鄰目標位置

    //AI(ht9045-v899) 20260414: trace daily-upload entry only (iLog>=5) to avoid timer-driven log bloat
    if(iLog>=5)
    {
        MNetLog(AnsiString().sprintf("[N10] UpDataToServer_KYEC: iLog=%d, N10_1=%d, N10_3=%d, Method=%d, UploadMethod=%d",
            iLog, (int)IniConfig.bN10Enable_FTPUpLoadLog, (int)IniConfig.bN10_DailyUploadProdData,
            IniConfig.iN10UploadProductMethod, IniConfig.iN10UploadMethod));
    }

    if(IniConfig.bN10Enable_FTPUpLoadLog==false &&
       IniConfig.bN10_DailyUploadProdData==false)
    {
        if(iLog>=5) MNetLog("[N10] UpDataToServer_KYEC skip: N10_1 and N10_3 both disabled"); //AI(ht9045-v899) 20260414: trace skip reason
        return HS_ERR_FunctionDisable;
    }

    _MyMsgStruct MsgStruct;
    map<AnsiString, _MyMsgStruct>mapMsgList;                                    //把結構塞在map中, 方便一起使用
    map<AnsiString, _MyMsgStruct>::iterator mapMsgIter;
    TStringList *TestList=new TStringList;
    TStringList *EventLogList=new TStringList;
    TStringList *JamStatList=new TStringList;
    int iFTPStatus[3]={0, 0, 0};
    int iResult=HS_ERR_NoError;
    bool bExec=false, bFlag=false;

    if(iLog==0)
    {
        if(bEPLogEnd_KYEC==true || bForceSend)
        {
            bEPLogEnd_KYEC=false;
            asFullName=asEPLog_HS+sEPLogFile;
            if(FileExists(asFullName)==true)                                    //Ifor 20160409 判斷EP Log 是否存在，存在才上傳資料
            {
                if(IniConfig.iN10UploadMethod==1)
                {
                    if(MyForceDirectories(IniConfig.sN10UploadDrivePath, "[N10] Update_EP_Log")!=1)       //Steven 20210112 : 針對資料夾加上保護
                    {
                        return HS_ERR_DirectoryError;
                    }
                    else
                    {                                                           //Ifor 20160407 複製EP Log 至網路芳鄰 Even Log 資料位置 for SPIL
                        str.sprintf("XCOPY /y/a/e/c/i/h/f/r \"%s\" \"%s\"", asFullName, IniConfig.sN10UploadDrivePath);
                        TestList->Add(str);
                        bExec=true;
                    }
                }
                else
                {
                    UpDataToServerByFTP(asEPLog_HS, sEPLogFile, "EPLog");
                }
            }
            else
            {
                RecordProcess("UpdateToServer :: File not exists!", asFullName);
            }
            ClearLastFileLogName_HS(0);
        }

        if(bTempLogEnd_KYEC || bForceSend)
        {
            bTempLogEnd_KYEC=false;
            asFullName=asTempLog_HS+sTemperatureLogFile;
            if(FileExists(asFullName)==true)                                    //Ifor 20160409 判斷Temperature Log 是否存在，存在才上傳資料
            {
                if(IniConfig.iN10UploadMethod==1)
                {
                    if(MyForceDirectories(IniConfig.sN10UploadDrivePath, "[N10] Update_Temp_Log")!=1)       //Steven 20210112 : 針對資料夾加上保護
                    {
                        return HS_ERR_DirectoryError;
                    }
                    else
                    {                                                           //Ifor 20160407 複製Temperature Log 至網路芳鄰 Even Log 資料位置 for SPIL
                        str.sprintf("XCOPY /y/a/e/c/i/h/f/r \"%s\" \"%s\"", asFullName, IniConfig.sN10UploadDrivePath);
                        TestList->Add(str);
                        bExec=true;
                    }
                }
                else
                {
                    UpDataToServerByFTP(asTempLog_HS, sTemperatureLogFile, "TemperatureLog");
                }
            }
            else
            {
                RecordProcess("UpdateToServer :: File not exists!", asFullName);
            }
            ClearLastFileLogName_HS(1);
        }

        if(bESDLogEnd_KYEC==true ||
           (USE_NOVX3360==true && bForceSend))
        {
            bESDLogEnd_KYEC=false;
            asFullName=asESDLog_HS+sESDLogFile;
            if(FileExists(asFullName)==true)
            {
                if(IniConfig.iN10UploadMethod==1)
                {
                    if(MyForceDirectories(IniConfig.sN10UploadDrivePath, "[N10] Update_ESD_Log")!=1)       //Steven 20210112 : 針對資料夾加上保護
                    {
                        return HS_ERR_DirectoryError;
                    }
                    else
                    {                                                           //Ifor 20160407 複製ESD Log 至網路芳鄰 Even Log 資料位置 for SPIL
                        str.sprintf("XCOPY /y/a/e/c/i/h/f/r \"%s\" \"%s\"", asFullName, IniConfig.sN10UploadDrivePath);
                        TestList->Add(str);
                        bExec=true;
                    }
                }
                else
                {
                    UpDataToServerByFTP(asESDLog_HS, sESDLogFile, "ESDLog");
                }
            }
            else
            {
                RecordProcess("UpdateToServer :: File not exists!", asFullName);
            }
            ClearLastFileLogName_HS(2);
        }

        if((ATC_SYSTEM==eATCHonPrecType || ATC_SYSTEM==eNewATCSystem) &&
           Temperature.bATCActiveCooling==true)
        {
            if(bATCEvenLogEnd_KYEC==true ||
               (CUSTOMER_CODE==CC_KYEC_LEE && bForceSend))
            {
                bATCEvenLogEnd_KYEC=false;
                asFullName=asATCEventLog_HS+asATCEvenLogFile;
                if(FileExists(asFullName)==true)
                {
                    if(IniConfig.iN10UploadMethod==1)
                    {
                        if(MyForceDirectories(IniConfig.sN10UploadDrivePath, "[N10] Update_ATC_Log")!=1)       //Steven 20210112 : 針對資料夾加上保護
                        {
                            return HS_ERR_DirectoryError;
                        }
                        else
                        {                                                       //Ifor 20160407 複製ATC Log 至網路芳鄰 Even Log 資料位置 for SPIL
                            str.sprintf("XCOPY /y/a/e/c/i/h/f/r \"%s\" \"%s\"", asFullName, IniConfig.sN10UploadDrivePath);
                            TestList->Add(str);
                            bExec=true;
                        }
                    }
                    else
                    {
                        UpDataToServerByFTP(asATCEventLog_HS, asATCEvenLogFile, "ATCEventLog");//Ifor 20170126 (Steven) Fix 路徑錯誤 sTemperatureLogFile => asATCEvenLogFile
                    }
                }
                else
                {
                    RecordProcess("UpdateToServer :: File not exists!", asFullName);
                }
                ClearLastFileLogName_HS(4);
            }
        }

        if(bArmTestInfoEvenLogEnd_KYEC ||
           (CosFunction.bUploadTestArmTorqueLog && bForceSend))                 //Ifor 20190912 :add 海思 V02.30 版 Record Torque
        {
            bArmTestInfoEvenLogEnd_KYEC=false;
            asFullName=asArmTestLog_HS+sArmTestInfoEvenLogFile;
            if(FileExists(asFullName)==true)
            {
                if(IniConfig.iN10UploadMethod==1)
                {
                    if(MyForceDirectories(IniConfig.sN10UploadDrivePath, "[N10] Update_Arm_Log")!=1)       //Steven 20210112 : 針對資料夾加上保護
                    {
                        return HS_ERR_DirectoryError;
                    }
                    else
                    {
                        str.sprintf("XCOPY /y/a/e/c/i/h/f/r \"%s\" \"%s\"", asFullName, IniConfig.sN10UploadDrivePath);
                        TestList->Add(str);
                        bExec=true;
                    }
                }
                else
                {
                    UpDataToServerByFTP(asArmTestLog_HS, sArmTestInfoEvenLogFile, "ArmTestInfoEvenLog");
                }
            }
            else
            {
                RecordProcess("UpdateToServer :: File not exists!", asFullName);
            }
            ClearLastFileLogName_HS(5);
        }
    }
    else if(iLog==5)                                                            //Steven 20180514 : JCET吳如春要求每日上傳Event Log, Jam統計表, MTBF, MUBF資料
    {
        //EventLog---
        if(slEventLog->bChangeFile==true)                                       //Steven 20250809 : 修正event log上傳
        {
            asFileName=slEventLog->sPrevFileName;
            slEventLog->bChangeFile=false;
        }
        else
        {
            asFileName=slEventLog->sLastFileName;
        }

        if(FileExists(asFileName))                                              //Steven 20190510 : fixed for eventlog upload
        {
            if(IniConfig.iN10UploadMethod==1)
            {
                if(MyForceDirectories(IniConfig.sN10UploadDrivePath, "[N10] Update_Event_Log")!=1)       //Steven 20210112 : 針對資料夾加上保護
                {
                    return HS_ERR_DirectoryError;
                }
                else
                {
                    if(FileExists(asFileName)==true)                            //Steven 20250809 : 修正event log上傳
                    {                                                           //Ifor 20160407 複製Event Log 至網路芳鄰 Event Log 資料位置
                        str.sprintf("XCOPY /y/a/e/c/i/h/f/r \"%s\" \"%s\"", asFileName, IniConfig.sN10UploadDrivePath);
                        TestList->Add(str);
                        bExec=true;
                    }
                }
            }
            else
            {
//                asDataDirection=IniConfig.cN10FtpUplaodPath;
                iFTPStatus[0]=UpDataToServerByFTP(ExtractFilePath(asFileName), ExtractFileName(asFileName), "EventLog", true);
            }
        }
        else
        {
            MyDBIProcess("Process", "FTP no event log file to upload.", asFileName);
            return HS_ERR_FTPNoFileForUploadError;
        }

        //Jam統計---
        EventLogList->Clear();
        if(FileExists(asFileName))                                              //Steven 20190717 : 修正檔名錯誤問題
        {
            EventLogList->LoadFromFile(asFileName);
        }
        mapMsgList.clear();                                                     //清除map資料
        if(EventLogList->Count>1)
        {
            for(int i=1; i<EventLogList->Count; i++)
            {
                JamStatList->Clear();
                JamStatList->CommaText=EventLogList->Strings[i];

                if(JamStatList->Count>7)                                        //jou 20220127 : 修正jam report上傳跳出例外
                {
                    if(IniConfig.bVTESTFunction==true)
                        bFlag=(JamStatList->Strings[3].AnsiPos("J")!=0);
                    else
                        bFlag=(JamStatList->Strings[3].AnsiPos("M")!=0 ||
                               JamStatList->Strings[3].AnsiPos("W")!=0 ||
                               JamStatList->Strings[3].AnsiPos("J")!=0);

                    if(bFlag==true)
                    {
                        MsgStruct.sJamCode=JamStatList->Strings[3];
                        mapMsgIter=mapMsgList.find(MsgStruct.sJamCode);         //使用迭代器搜尋Key
                        if(mapMsgIter==mapMsgList.end())                        //當該Key沒有被找到
                        {
                            MsgStruct.iJamCount=1;                              //先將結構的資料準備好
                            MsgStruct.sPart=JamStatList->Strings[2];
                            MsgStruct.sMessage=JamStatList->Strings[7];
                            MsgStruct.iStopTime=atoi(JamStatList->Strings[5].c_str());
                            if(atoi(JamStatList->Strings[6].c_str())==0)        //重複的或Off Line的不要算進去
                            {
                                mapMsgList[MsgStruct.sJamCode]=MsgStruct;       //把結構(Value)指定給Key
                            }
                        }
                        else if(mapMsgList.size()!=0 &&
                                mapMsgIter!=mapMsgList.end())                   //如果有找到資料
                        {
                            if(atoi(JamStatList->Strings[6].c_str())==0)        //讀取資料內容 (讀Key使用->first, 讀Value使用->second)
                            {
                                mapMsgIter->second.iJamCount=(mapMsgIter->second.iJamCount+1);                //更新數值
                                mapMsgIter->second.iStopTime=(mapMsgIter->second.iStopTime+atoi(JamStatList->Strings[5].c_str()));
                            }
                        }
                    }
                }
            }
        }

        JamStatList->Clear();

        if(CUSTOMER_CODE==CC_HANA_MICRON)                                       //Steven 20251007 : maintance mode for Hana
        {
            JamStatList->Add("Date, Handler, Part, ErrNum, ErrComent, ErrorCount, StopTime");
            for(mapMsgIter=mapMsgList.begin(); mapMsgIter!=mapMsgList.end(); mapMsgIter++)
            {
                str.sprintf("%02d.%02d.%02d, %s, \"%s\", %s, \"%s\", %d, %d",
                                                                        SystemYearYesterday, SystemMonthYesterday, SystemDateYesterday,
                                                                        IniConfig.SocketHandlerID,
                                                                        mapMsgIter->second.sPart,
                                                                        mapMsgIter->second.sJamCode,
                                                                        mapMsgIter->second.sMessage,
                                                                        mapMsgIter->second.iJamCount,
                                                                        mapMsgIter->second.iStopTime);
                JamStatList->Add(str);
            }
        }
        else
        {
            JamStatList->Add("JamCode, JamCount, StopTime, Message");
            for(mapMsgIter=mapMsgList.begin(); mapMsgIter!=mapMsgList.end(); mapMsgIter++)
            {
                str.sprintf("%s, %d, %d, \"%s\"", mapMsgIter->first, mapMsgIter->second.iJamCount, mapMsgIter->second.iStopTime, mapMsgIter->second.sMessage);
                JamStatList->Add(str);
            }
        }

        GetYesterdayInfo();
        if(IniConfig.bVTESTFunction==true)                                      //RogerYang 20250818 偉測照舊版，跟EventLogTxt放一起
            asJamStatPath="D:\\HT9045_Log\\EventLogTxt";

        if(CUSTOMER_CODE==CC_HANA_MICRON)                                       //Steven 20251007 : maintance mode for Hana
        {
            asDirPath.sprintf("%s\\%04d\\%02d\\", asJamStatPath, SystemYearYesterday, SystemMonthYesterday);
            asFileName.sprintf("%04d%02d%02d_%s_JamList.csv", SystemYearYesterday, SystemMonthYesterday, SystemDateYesterday, IniConfig.SocketHandlerID);
        }
        else if(IniConfig.iN10UploadProductMethod==0)                           //JerryYang 20190131 上傳production log可選擇00:00 or 08:00
        {
            asDirPath.sprintf("%s\\%04d\\%02d\\", asJamStatPath, SystemYearYesterday, SystemMonthYesterday);  //Steven 20210810 : 變更JamStatList的路徑
            asFileName.sprintf("%s_%s_JamStat_%04d%02d%02d.csv", IniConfig.sMachineType, IniConfig.SocketHandlerID, SystemYearYesterday, SystemMonthYesterday, SystemDateYesterday);
        }
        else
        {
            if((SystemHour==8 && SystemMin==0) ||
                SystemHour<8)
            {
                asDirPath.sprintf("%s\\%04d\\%02d\\%02d\\", asJamStatPath, SystemYearYesterday, SystemMonthYesterday, SystemDateYesterday);   //Steven 20210810 : 變更JamStatList的路徑
                asFileName.sprintf("%s_%s_JamStat_%04d%02d%02d%02d00.csv", IniConfig.sMachineType, IniConfig.SocketHandlerID, SystemYearYesterday, SystemMonthYesterday, SystemDateYesterday, 20);
            }
            else if((SystemHour==20 && SystemMin==0) ||
                    (SystemHour>=8 && SystemHour<20))
            {
                asDirPath.sprintf("%s\\%04d\\%02d\\%02d\\", asJamStatPath, SystemYear, SystemMonth, SystemDate);  //Steven 20210810 : 變更JamStatList的路徑
                asFileName.sprintf("%s_%s_JamStat_%04d%02d%02d%02d00.csv", IniConfig.sMachineType, IniConfig.SocketHandlerID, SystemYear, SystemMonth, SystemDate, 8);
            }
            else
            {
                asDirPath.sprintf("%s\\%04d\\%02d\\%02d\\", asJamStatPath, SystemYear, SystemMonth, SystemDate);  //Steven 20210810 : 變更JamStatList的路徑
                asFileName.sprintf("%s_%s_JamStat_%04d%02d%02d%02d00.csv", IniConfig.sMachineType, IniConfig.SocketHandlerID, SystemYear, SystemMonth, SystemDate, 20);
            }
        }

        MyForceDirectories(asDirPath, "TFormHS::UpDataToServer_KYEC_JamStatList");
        asFullName=asDirPath+asFileName;                                        //Ifor 20160407 複製JamStat Log 至網路芳鄰 Even Log 資料位置 for SPIL
        JamStatList->SaveToFile(asFullName);

        if(FileExists(asFullName)==true)
        {
            if(IniConfig.iN10UploadMethod==1)
            {
                if(MyForceDirectories(IniConfig.sN10UploadDrivePath, "[N10] Update_JamStat_Log")!=1)       //Steven 20210112 : 針對資料夾加上保護
                {
                    return HS_ERR_DirectoryError;
                }
                else
                {
                    str.sprintf("XCOPY /y/a/e/c/i/h/f/r \"%s\" \"%s\"", asFullName, IniConfig.sN10UploadDrivePath);
                    TestList->Add(str);
                    bExec=true;
                }
            }
            else
            {
                iFTPStatus[1]=UpDataToServerByFTP(asDirPath, asFileName, "JamStatLog", true);
            }
        }
        else
        {
            RecordProcess("UpdateToServer :: File not exists!", asFullName);
        }

        if(IniConfig.bVTESTFunction==false)
        {
            //Time Data統計---
            asDirPath.sprintf("D:\\HT9045_Log\\TimeData\\%04d\\", SystemYearYesterday);
            MyForceDirectories(asDirPath, "TFormHS::UpDataToServer_KYEC_TimeData");
            asFileName.sprintf("TimeData_%04d.csv", SystemYearYesterday);
            asFullName=asDirPath+asFileName;
            if(FileExists(asFullName)==true)
            {
                if(IniConfig.iN10UploadMethod==1)
                {
                    if(MyForceDirectories(IniConfig.sN10UploadDrivePath, "[N10] Update_Time_Data")!=1)       //Steven 20210112 : 針對資料夾加上保護
                    {
                        return HS_ERR_DirectoryError;
                    }
                    else
                    {
                        str.sprintf("XCOPY /y/a/e/c/i/h/f/r \"%s\" \"%s\"", asFullName, IniConfig.sN10UploadDrivePath);
                        TestList->Add(str);
                        bExec=true;
                    }
                }
                else
                {
                    iFTPStatus[2]=UpDataToServerByFTP(asDirPath, asFileName, "TimeData", true);
                }
            }
            else
            {
                RecordProcess("UpdateToServer :: File not exists!", asFullName);
            }
        }

        //Production_Log
        if(CUSTOMER_CODE==CC_SIGURD_HUKOU ||                                    //Sam 20210313 : 矽格湖口增加上傳 Production_Log
           IniConfig.bN10_11_Enable_UploadFTPEventLog)
        {
            //AI(ht9045-v899) 20260415: swap SystemDate to yesterday before GetProdLog to fix cross-midnight filename mismatch
            Word wSavYear=SystemYear, wSavMonth=SystemMonth, wSavDate=SystemDate;
            SystemYear=SystemYearYesterday; SystemMonth=SystemMonthYesterday; SystemDate=SystemDateYesterday;
            asDirPath=TMyProductionRecord().GetProdLogFilePath();
            asFileName=TMyProductionRecord().GetProdLogFileName(false);
            SystemYear=wSavYear; SystemMonth=wSavMonth; SystemDate=wSavDate;

            asFullName=FileInfo().PathCombin(asDirPath,asFileName);
            if(FileExists(asFullName)==true)
            {
                if(IniConfig.iN10UploadMethod==1)
                {
                    str.sprintf("XCOPY /y/a/e/c/i/h/f/r \"%s\" \"%s\"", asFullName, IniConfig.sN10UploadDrivePath);
                    TestList->Add(str);
                    bExec=true;
                }
                else
                {
                    UpDataToServerByFTP(asDirPath, asFileName, "ProductionLog", true);
                }
            }
            else
            {
                RecordProcess("UpdateToServer :: File not exists!", asFullName);
            }
        }

        if(IniConfig.bN10_12_Enable_UploadFTPGPIBLog)                           //GPIB
        {
            //AI(ht9045-v899) 20260415: swap SystemDate to yesterday before GetGPIBLog to fix cross-midnight filename mismatch
            Word wSavYear2=SystemYear, wSavMonth2=SystemMonth, wSavDate2=SystemDate;
            SystemYear=SystemYearYesterday; SystemMonth=SystemMonthYesterday; SystemDate=SystemDateYesterday;
            asDirPath=GetGPIBLogFilePath();
            std::vector<AnsiString> lsFileName=GetGPIBLogFileName();
            SystemYear=wSavYear2; SystemMonth=wSavMonth2; SystemDate=wSavDate2;

            for(unsigned int i=0;i<lsFileName.size();i++)
            {
                asFileName=lsFileName[i];
                asFullName=FileInfo().PathCombin(asDirPath,asFileName);
                if(FileExists(asFullName)==true)
                {
                    if(IniConfig.iN10UploadMethod==1)
                    {
                        str.sprintf("XCOPY /y/a/e/c/i/h/f/r \"%s\" \"%s\"", asFullName, IniConfig.sN10UploadDrivePath);
                        TestList->Add(str);
                        bExec=true;
                    }
                    else
                    {
                        UpDataToServerByFTP(asDirPath, asFileName, "GPIBLog", true);
                    }
                }
                else
                {
                    RecordProcess("UpdateToServer :: File not exists!", asFullName);
                }
            }
        }
    }
    else if(iLog==6 || iLog==7)
    {
        GetTimeInfo();
        if(iLog==6)
        {
            asDirPath=asPowerSavePath+"\\";
        }
        else
        {
            asDirPath=asATCPowerSavePath+"\\";
        }
        asFileName.sprintf("%s_Power_Saving_%04d.csv", IniConfig.SocketHandlerID, SystemYear);
        asFullName=asDirPath+asFileName;
        if(FileExists(asFullName)==true)
        {
            UpDataToServerByFTP(asDirPath, asFileName, "PowerSavingLog", true);
        }
        else
        {
            RecordProcess("UpdateToServer :: File not exists!", asFullName);
        }
    }

    if(bExec)
    {
        try
        {
            TestList->SaveToFile("D:\\HT9045\\system\\2.bat");                  //Ifor 20160408 建立檔案與寫入檔案
        }
        catch(...)
        {
            MyDBIProcess("Exception", "TFormHS::UpDataToServer_KYEC");
        }

        for(int i=0; i<TestList->Count; i++)                                    //Steven 20200316 : 增加上傳檔案的log
        {
            RecordProcess(TestList->Strings[i]);
        }

        try
        {
            ExecZipCommand("D:\\HT9045\\system\\2.bat", " ");                   //Ifor 20160408 避免複製檔案時出現Dos執行畫面
        }
        catch(...)
        {
            MyDBIProcess("Exception", "TFormHS::UpDataToServer_KYEC ExecZipCommand");
        }
//        if(iLog==0)                                                           //Steven 20190801 : 修正, 只有結批的時候才能清除
//        {
//            fLotInfo->edtSysLotID->Text="";
//            fLotInfo->edtSysOperatorID->Text="";
//        }
    }
    TestList->Clear();
    delete TestList;
    delete EventLogList;
    delete JamStatList;
    return iResult;
}
extern bool MNetLog(AnsiString Message);                                        //Steven 20110406
//---------------------------------------------------------------------------
AnsiString TFormHS::GetUploadServerByFTPPath(AnsiString asFileType, bool bDailyReport)
{
    AnsiString asFtpUplaodPath="",asFTPSettingPath="";
    if(bDailyReport==false &&                                                   //Sam 20170823 (wei) : 矽格中興 FTP Log 上傳增
       (CUSTOMER_CODE==CC_SIGURD_ChungXing ||
        CUSTOMER_CODE==CC_SIGURD_PeiXing))                                      //Ifor 20180213 : add 矽格北興使用FTP Log 上傳增加時間格式選擇
    {
        GetTimeInfo();
        if(IniConfig.iN10DataType==0)
            asFtpUplaodPath.sprintf("%s%04d/", IniConfig.cN10FtpUplaodPath, SystemYear);
        else if(IniConfig.iN10DataType==1)
            asFtpUplaodPath.sprintf("%s%04d%02d/", IniConfig.cN10FtpUplaodPath, SystemYear, SystemMonth);
        else
            asFtpUplaodPath.sprintf("%s%04d%02d%02d/", IniConfig.cN10FtpUplaodPath, SystemYear, SystemMonth, SystemDate);
    }
    else
    {
        asFtpUplaodPath.sprintf("%s", IniConfig.cN10FtpUplaodPath);

        if(CosFunction.bUploadLogByTypeFolder)                                  //Steven 20250217 : 根據Log內容作資料夾分類
        {                                                                       //檔案路徑:  /年_月/日/
            asFTPSettingPath.sprintf("%s%s", IniConfig.cN10FtpUplaodPath, asFileType);
            asFtpUplaodPath.sprintf("%s", asFTPSettingPath);
            asFTPSettingPath.sprintf("%s%s/", IniConfig.cN10FtpUplaodPath, asFileType);
        }
        else
        {
            asFTPSettingPath.sprintf("%s", IniConfig.cN10FtpUplaodPath);        //檔案路徑:  /年_月/日/機台型號_機台ID/
        }

        GetYesterdayInfo();
        if(IniConfig.iN10UploadProductMethod==0)                                //JerryYang 20190131 上傳production log可選擇00:00 or 08:00
        {
            asFtpUplaodPath=FileInfo().PathCombin(asFTPSettingPath, AnsiString().sprintf("%04d_%02d", SystemYearYesterday, SystemMonthYesterday));
        }
        else
        {
            if((SystemHour==8 && SystemMin==0) || SystemHour<8)
            {
                asFtpUplaodPath=FileInfo().PathCombin(asFTPSettingPath, AnsiString().sprintf("%04d_%02d", SystemYearYesterday, SystemMonthYesterday));
            }
            else
            {
                asFtpUplaodPath=FileInfo().PathCombin(asFTPSettingPath, AnsiString().sprintf("%04d_%02d", SystemYear, SystemMonth));
            }
        }

        if(IniConfig.iN10UploadProductMethod==0)                                //JerryYang 20190131 上傳production log可選擇00:00 or 08:00
        {
            asFtpUplaodPath=FileInfo().PathCombin(asFtpUplaodPath, AnsiString().sprintf("%02d", SystemDateYesterday));
        }
        else
        {
            if((SystemHour==8 && SystemMin==0) || SystemHour<8)
            {
                asFtpUplaodPath=FileInfo().PathCombin(asFtpUplaodPath, AnsiString().sprintf("%02d", SystemDateYesterday));
            }
            else
            {
                asFtpUplaodPath=FileInfo().PathCombin(asFtpUplaodPath, AnsiString().sprintf("%02d", SystemDate));
            }
        }

        if(CosFunction.bUploadLogByTypeFolder==false)                           //Steven 20250217 : 根據Log內容作資料夾分類
        {
            asFtpUplaodPath=FileInfo().PathCombin(asFtpUplaodPath, AnsiString().sprintf("%s_%s", IniConfig.sMachineType, IniConfig.SocketHandlerID));
        }
    }
    return asFtpUplaodPath;
}
//---------------------------------------------------------------------------
int __fastcall TFormHS::UpDataToServerByFTP(AnsiString asDirPath, AnsiString sFileName, AnsiString asFileType, bool bDailyReport)   //Steven 20250716 : 修正N10上傳FTP的動作
{
    int iReturn=HS_ERR_NoError;
    AnsiString asFtpUplaodPath=GetUploadServerByFTPPath(asFileType,bDailyReport);                                                 //Sam 20170823 (wei) : 矽格中興 FTP Log 上傳增加時間格式選擇
    AnsiString asUserID="",asPassword="",asHost="";
    #ifdef SOFT_SIMULTE
    asUserID="HONPREC";
    asPassword="27025312";
    asHost="127.0.0.1";
    #else
    asUserID=IniConfig.cN10FtpUserName;
    asPassword=IniConfig.cN10FtpPassword;
    asHost=IniConfig.cN10FtpHost;
    #endif
    TfFTP fFTP(asUserID, asPassword, asHost);
    //AI(ht9045-v899) 20260414: diagnostic log only on daily-upload path to avoid timer-driven bloat
    if(bDailyReport)
        MNetLog(AnsiString().sprintf("[N10] FTP connecting: Host=%s, Port=%d, Passive=%d, Type=%s, File=%s",
            asHost, IniConfig.iN10FtpPort, (int)IniConfig.bN10FtpPassive, asFileType, sFileName));
    if(fFTP.Connect(asUserID, asPassword, asHost, 30000, NMOS_AUTO, IniConfig.bN10FtpPassive, IniConfig.iN10FtpPort))
    {
        AnsiString asError="";
        fFTP.CheckLocalFilePath(asDirPath);
        MNetLog(AnsiString().sprintf("FTP, Change folder : %s, %s", asFtpUplaodPath, sFileName));
        fFTP.ChangeDirectories(asFtpUplaodPath);
        if(FileExists(FileInfo().PathCombin(asDirPath, sFileName)))             //Sam 20180802 (wei) : 增加保護
        {
            if(fFTP.Upload(asDirPath, asFtpUplaodPath, sFileName, sFileName, asError))
            {
                if(bDailyReport) MNetLog(AnsiString().sprintf("[N10] FTP upload OK: %s", sFileName)); //AI(ht9045-v899) 20260414: trace success
                iReturn=HS_ERR_NoError;
            }
            else
            {
                if(bDailyReport) MNetLog(AnsiString().sprintf("[N10] FTP upload FAIL: %s, Err=%s", sFileName, asError)); //AI(ht9045-v899) 20260414: trace failure
                iReturn=HS_ERR_FTPUploadError;
            }
        }
        else
        {
            if(bDailyReport) MNetLog(AnsiString().sprintf("[N10] FTP file missing: %s", FileInfo().PathCombin(asDirPath, sFileName))); //AI(ht9045-v899) 20260414: trace missing file
            iReturn=HS_ERR_FTPNoFileForUploadError;
        }
    }
    else
    {
        if(bDailyReport) MNetLog(AnsiString().sprintf("[N10] FTP connect FAIL: Host=%s, Port=%d", asHost, IniConfig.iN10FtpPort)); //AI(ht9045-v899) 20260414: trace connect failure
        iReturn=HS_ERR_FTPUploadError;
    }

    fFTP.Close();

//    bNeedUpload_N10=true;
    return iReturn;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TFormHS::GetTempUseName(int source)                       //Ifor 20160303 依據溫控器開啟狀況變更Log顯示名稱
{
    String NowTemperature="";
    NowTemperature ="Ver:"+asHandlerVersion+"\n";                               //kevin 20191212 add
    AnsiString sTempName[tcTotalCount]={"Plate1OFS",    "Plate2OFS",    "Shut1OFS",     "Shut2OFS",     "Head1OFS",   //溫控器要一起改
                                        "Head2OFS",     "Head3OFS",     "Head4OFS",     "DutOFS",       "ChamberOFS",
                                        "CCDOFS",       "Aa1OFS",       "Ab1OFS",       "Ac1OFS",       "Ad1OFS",
                                        "Ba1OFS",       "Bb1OFS",       "Bc1OFS",       "Bd1OFS",       "Aa2OFS",
                                        "Ab2OFS",       "Ac2OFS",       "Ad2OFS",       "Ba2OFS",       "Bb2OFS",
                                        "Bc2OFS",       "Bd2OFS",       "HeatGun1OFS",  "HeatGun2OFS",  "DUT1OFS",
                                        "DUT2OFS",      "DUT3OFS",      "DUT4OFS",      "Ae1OFS",       "Af1OFS",
                                        "Ag1OFS",       "Ah1OFS",       "Be1OFS",       "Bf1OFS",       "Bg1OFS",
                                        "Bh1OFS",       "Ae2OFS",       "Af2OFS",       "Ag2OFS",       "Ah2OFS",
                                        "Be2OFS",       "Bf2OFS",       "Bg2OFS",       "Bh2OFS",       "2DOFS",
                                        "LBOFS",        "IndexESDOFS",  "CCD_2OFS",     "ATCAir1OFS",   "ATCAir2OFS",
                                        "OutSht1OFS",   "OutSht2OFS",   "Base1OFS",     "Base2OFS",     "Base3OFS",
                                        "Base4OFS",     "Base5OFS",     "Base6OFS",     "HotPlate3OFS", "HotPlate4OFS",
                                        "Shuttle3OFS",  "Shuttle4OFS",  "Door1OFS",     "Door2OFS",     "LBUpOFS",
                                        "LBDownOFS"
                                       };

    if(source==1)
    {
        NowTemperature=NowTemperature+
        "Time"                                  +","+
        "Plate1"                                +","+
        "Plate2"                                +","+
        "Shut1"                                 +","+
        "Shut2"                                 ;
    }
    else
    {
        if(USE_16_HEATER==eht4Heater)
        {
            if(iSocketBaseTempCount==eDut2ea)
            {
                NowTemperature=NowTemperature+                                  //Richard 20230224 : 修正溫度顯示內容
                "Time"                              +","+
                "TempType"                          +","+
                "Plate1"                            +","+
                "Plate2"                            +","+
                "Shut1"                             +","+
                "Shut2"                             +","+
                "Chamber"                           +","+
                "CCD"                               +","+
                "Dut1"                              +","+
                "Dut2"                              +","+
                "HeatGun1"                          +","+
                "HeatGun2"                          +","+
                "Head1"                             +","+
                "Head2"                             +","+
                "Head3"                             +","+
                "Head4"                             ;
            }
            else if(iSocketBaseTempCount==eDut1ea)
            {
                NowTemperature=NowTemperature+
                "Time"                              +","+
                "TempType"                          +","+
                "Plate1"                            +","+
                "Plate2"                            +","+
                "Shut1"                             +","+
                "Shut2"                             +","+
                "Chamber"                           +","+
                "CCD"                               +","+
                "Dut"                               +","+
                "HeatGun1"                          +","+
                "HeatGun2"                          +","+
                "Head1"                             +","+
                "Head2"                             +","+
                "Head3"                             +","+
                "Head4"                             ;
            }
        }
        else
        {
            NowTemperature=NowTemperature+
            "Time"                              +","+
            "TempType"                          +","+
            "Plate1"                            +","+
            "Plate2"                            +","+
            "Shut1"                             +","+
            "Shut2"                             +","+
            "Chamber"                           +","+
            "CCD"                               ;

            if(CUSTOMER_CODE==CC_ASE_KaohSiung)
            {
                  //kevin 20200520 log 4 dut
                  NowTemperature=NowTemperature            +","+
                  "DUT1"                          +","+
                  "DUT2"                          +","+
                  "DUT3"                          +","+
                  "DUT4"                          ;
            }
            else
            {
            //JerryYang 20180417 (jou) : 補上Dut溫度log
            //==>
                if(iSocketBaseTempCount==eDut4ea)
                {
                    NowTemperature=NowTemperature            +","+
                    "DUT1"                          +","+
                    "DUT2"                          +","+
                    "DUT3"                          +","+
                    "DUT4"                          ;
                }
                else if(iSocketBaseTempCount==eDut2ea)
                {
                    NowTemperature=NowTemperature            +","+
                    "DUT1"                                   +","+
                    "DUT2"                                   ;
                }
                else
                {
                    NowTemperature=NowTemperature   +","+
                    "Socket"                        ;
                }
            }
            //<==
            //JerryYang 20180417 (jou) : 補上Dut溫度log

            NowTemperature=NowTemperature       +","+
            "Aa1"                               +","+
            "Ab1"                               +","+
            "Ac1"                               +","+
            "Ad1"                               +","+
            "Aa2"                               +","+
            "Ab2"                               +","+
            "Ac2"                               +","+
            "Ad2"                               +","+
            "Ba1"                               +","+
            "Bb1"                               +","+
            "Bc1"                               +","+
            "Bd1"                               +","+
            "Ba2"                               +","+
            "Bb2"                               +","+
            "Bc2"                               +","+
            "Bd2"                               ;
            if(USE_16_HEATER>2)
            {
                NowTemperature=NowTemperature   +","+
                "Ae1"                           +","+
                "Af1"                           +","+
                "Ag1"                           +","+
                "Ah1"                           +","+
                "Ae2"                           +","+
                "Af2"                           +","+
                "Ag2"                           +","+
                "Ah2"                           +","+
                "Be1"                           +","+
                "Bf1"                           +","+
                "Bg1"                           +","+
                "Bh1"                           +","+
                "Be2"                           +","+
                "Bf2"                           +","+
                "Bg2"                           +","+
                "Bh2"                           ;
            }
        }
    }

    if(ATC_SYSTEM==eATCHonPrecType)                                             //Ifor 20151119 新增海思ATC2.0 溫度Log
    {
        NowTemperature=NowTemperature           +","+
        "ATC First CH1"                         +","+
        "ATC Second CH1"                        +","+
        "ATC First CH2"                         +","+
        "ATC Second CH2"                        +","+
        "ATC First CH3"                         +","+
        "ATC Second CH3"                        +","+
        "ATC First CH4"                         +","+
        "ATC Second CH4"                        +","+
        "ATC Working Temp"                      +","+
        "ATC Chiller SV "                       ;
    }
    else if(ATC_SYSTEM==eNewATCSystem)                                          //Ifor 20160303 新增 NewATC 溫度Log
    {
        if(iATC_Use_Heat_Count<=8)                                              //Ifor 20190906 : 海思尚未使用 ATC 8Site機台
        {
            NowTemperature=NowTemperature           +","+
            "ATC First CH1"                         +","+
            "ATC Second CH1"                        +","+
            "ATC First CH2"                         +","+
            "ATC Second CH2"                        +","+
            "ATC First CH3"                         +","+
            "ATC Second CH3"                        +","+
            "ATC First CH4"                         +","+
            "ATC Second CH4"                        ;

            if(iATC_Use_Heat_Count>4)
            {
                NowTemperature=NowTemperature       +","+
                "ATC First CH5"                     +","+
                "ATC Second CH5"                    +","+
                "ATC First CH6"                     +","+
                "ATC Second CH6"                    +","+
                "ATC First CH7"                     +","+
                "ATC Second CH7"                    +","+
                "ATC First CH8"                     +","+
                "ATC Second CH8"                    ;
            }
            NowTemperature=NowTemperature+","+"ATC Working Temp";
        }
        else                                                                    //Ifor 20190909 :預留 32Site ATC Log
        {

        }
    }
    if(BAR_CODE_INSTALL==ebctInShtIntel)
    {
         NowTemperature=NowTemperature           +","+
         "2D,"                                   ;
    }

    if(source==1)
    {
        if(ATC_SYSTEM==eATCHonPrecType || ATC_SYSTEM==eNewATCSystem)
        {
            NowTemperature=NowTemperature           +","+
            "RunState"                              +","+
            "Run ATC State"                         +","+
            "Monitor ATC State"                     +","+
            "Set Range"                             ;
        }
    }
    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                         //kevin 20190615 tester on line offline
    {
        NowTemperature=NowTemperature           +","+
        "Tester"                                ;
    }

    NowTemperature=NowTemperature +","+                                         //kevin 20191226 change
    "Hot Gun 1"                   +","+
    "Hot Gun 2";

    //Ifor 20190905 :add 海思 V02.30 版  上傳Server資料增加Lot ID欄位
    /*
    if(CosFunction.bHiSiliconFunction==true || CUSTOMER_CODE==CC_KYEC_LEE)
    {
        NowTemperature=NowTemperature           +","+"Lot ID";
    } */

//    if(CosFunction.bHiSiliconFunction==true ||                                //Steven 20240925 : mark for 上傳Server資料增加Lot ID欄位
//       CUSTOMER_CODE==CC_KYEC_LEE  ||
//       CUSTOMER_CODE==CC_ASE_KaohSiung)                                       //kevin 20191210 add
    {
        NowTemperature=NowTemperature+",Lot ID , Temp Target  , Low Limit , High Limit , OSAT Lot number," ;  //kevin 20191210 add
        for(int i=0; i<tcTotalCount; i++)                                       //kevin 20200305 change
        {
            NowTemperature=NowTemperature+sTempName[i]+",";
        }
    }
    return NowTemperature;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TFormHS::GetTempUseNamevalue(int source)                  //Ifor 20160303 依據溫控器開啟狀況變更Log 顯示數值
{
    String NowTemperature="";
    bool bnormal=false;
    AnsiString sBuffer;

    if(source==1)                                                               //Ifor 20160330 修改 海思版本 僅記錄ATC
    {
        if(CUSTOMER_CODE==CC_KYEC_LEE)                                          //NowTemperature=Now().FormatString("hh:nn:ss");
        {                                                                       //Ifor 20160329 修改 溫度時間格式 時:分:秒 -> 年/月/日 時:分:秒
            AnsiString sTime = GetDateInfoByString("/") + " " + GetOnlyTimeInfoByString(":");
            NowTemperature=sTime                                    +","+
            RunInfo.ShowTempComp[tcHotPlate1]                       +","+
            RunInfo.ShowTempComp[tcHotPlate2]                       +","+
            RunInfo.ShowTempComp[tcShuttle1]                        +","+
            RunInfo.ShowTempComp[tcShuttle2]                        ;

            if(ATC_SYSTEM==eATCHonPrecType ||                                   //Ifor 20190909 : 整合KYEC ATC溫度Log
               ATC_SYSTEM==eNewATCSystem)
            {
                if(Temperature.bATCActiveCooling==true)
                {
                    for(int i=0; i<iATC_Use_Heat_Count; i++)
                    {
                        if(iATC_Use_Heat_Count<=8)
                        {
                            if(bATC_EnablesChannel[i]==true)
                            {
                                if(Temperature.bUseReferTempSensor==true)
                                {
                                     NowTemperature=NowTemperature  +","+
                                     fLotInfo->ATCPtr[i]->Caption             +","+
                                     fLotInfo->ATCReferPtr[i]->Caption        ;
                                }
                                else
                                {
                                     NowTemperature=NowTemperature  +","+
                                     fLotInfo->ATCPtr[i]->Caption             +","+
                                     "0.0 'C"                       ;
                                }
                            }
                            else
                            {
                                if(Temperature.bUseReferTempSensor==true)
                                {
                                     NowTemperature=NowTemperature  +","+
                                     "--"                           +","+
                                     "--"                           ;
                                }
                                else
                                {
                                     NowTemperature=NowTemperature  +","+
                                     "--"                           +","+
                                     "0.0 'C"                       ;
                                }
                            }
                        }
                        else                                                    //Ifor 20190909 :預留 32Site ATC Log
                        {

                        }
                    }
                }
                else
                {
                    if(iATC_Use_Heat_Count<=8)
                    {
                        NowTemperature=NowTemperature               +","+
                        "0.0 'C"                                    +","+
                        "0.0 'C"                                    +","+
                        "0.0 'C"                                    +","+
                        "0.0 'C"                                    +","+
                        "0.0 'C"                                    +","+
                        "0.0 'C"                                    +","+
                        "0.0 'C"                                    +","+
                        "0.0 'C"                                    ;

                        if(iATC_Use_Heat_Count>4)
                        {
                            NowTemperature=NowTemperature               +","+
                            "0.0 'C"                                    +","+
                            "0.0 'C"                                    +","+
                            "0.0 'C"                                    +","+
                            "0.0 'C"                                    +","+
                            "0.0 'C"                                    +","+
                            "0.0 'C"                                    +","+
                            "0.0 'C"                                    +","+
                            "0.0 'C"                                    ;
                        }
                    }
                }
                NowTemperature=NowTemperature                           +","+
                fLotInfo->palATCWorkingTemp->Caption                    ;

                if(ATC_SYSTEM==eATCHonPrecType)                                 //Ifor 20151119 新增海思ATC2.0 溫度Log
                {
                    NowTemperature=NowTemperature                       +","+
                    fLotInfo->pl_ATCChillerSV->Caption                  +","+
                    fMain->palMainStatus->Caption                       +",";

                    if(Temperature.bATCActiveCooling==true)
                        NowTemperature=NowTemperature+"Run ATC,";
                    else
                        NowTemperature=NowTemperature+"STOP ATC,";

                    if(ATCInterfaceForm->btnATCPower->Caption=="RUN ATC")
                        NowTemperature=NowTemperature+"STOP ATC,";
                    else
                        NowTemperature=NowTemperature+"Run ATC,";
                }
                else if(ATC_SYSTEM==eNewATCSystem)                              //Ifor 20160303 新增 NewATC 溫度Log
                {
                    NowTemperature=NowTemperature                       +","+
                    fMain->palMainStatus->Caption                       +",";

                    if(Temperature.bATCActiveCooling==true)
                        NowTemperature=NowTemperature+"Run ATC,";
                    else
                        NowTemperature=NowTemperature+"STOP ATC,";

                    if(ATC_InterfaceForm->GetRunState()==false)
                        NowTemperature=NowTemperature+"STOP ATC,";
                    else
                        NowTemperature=NowTemperature+"Run ATC,";
                }
                NowTemperature=NowTemperature+IniConfig.iATCTemperatureOutside;
            }
        }
        else
        {
            AnsiString sTime=GetDateInfoByString("/") + " " + GetOnlyTimeInfoByString(":");
            NowTemperature=sTime+",";
            if(LastSet.iTemperature==Tempture_Ambient)
            {
                NowTemperature+="Ambient:";
                NowTemperature+=IniConfig.dATCAmbientTemperature;
            }
            else if(LastSet.iTemperature==Tempture_AmbientHot)                  //kevin 20150813 恆溫控制 add
            {
                NowTemperature+="  Ambient Control:";
                NowTemperature+=Temperature.fWorkTemperBase;
            }
            else
            {
                NowTemperature+="HOT:";
                NowTemperature+=Temperature.fWorkTemperBase;
            }
            bnormal=true;
        }
    }
    else
    {
        bnormal=true;
    }

    if(bnormal==true)
    {
        if(USE_16_HEATER==eht4Heater)
        {
            if(iSocketBaseTempCount==eDut2ea)                                   //Richard 20230224 : 修正溫度顯示內容
            {
                NowTemperature=NowTemperature+    ","+
                RunInfo.ShowTempComp[tcHotPlate1] +","+
                RunInfo.ShowTempComp[tcHotPlate2] +","+
                RunInfo.ShowTempComp[tcShuttle1]  +","+
                RunInfo.ShowTempComp[tcShuttle2]  +","+
                RunInfo.ShowTempComp[tcChamber]   +","+
                RunInfo.ShowTempComp[tcCCD]       +","+
                RunInfo.ShowTempComp[tcDUT1]      +","+
                RunInfo.ShowTempComp[tcDUT2]      +","+
                RunInfo.ShowTempComp[tcHeatGun1]  +","+
                RunInfo.ShowTempComp[tcHeatGun2]  +","+
                RunInfo.ShowTempComp[tcHead1]     +","+
                RunInfo.ShowTempComp[tcHead2]     +","+
                RunInfo.ShowTempComp[tcHead3]     +","+
                RunInfo.ShowTempComp[tcHead4]     ;
            }
            else if(iSocketBaseTempCount==eDut1ea)
            {
                NowTemperature=NowTemperature+    ","+
                RunInfo.ShowTempComp[tcHotPlate1] +","+
                RunInfo.ShowTempComp[tcHotPlate2] +","+
                RunInfo.ShowTempComp[tcShuttle1]  +","+
                RunInfo.ShowTempComp[tcShuttle2]  +","+
                RunInfo.ShowTempComp[tcChamber]   +","+
                RunInfo.ShowTempComp[tcCCD]       +","+
                RunInfo.ShowTempComp[tcSocket]    +","+
                RunInfo.ShowTempComp[tcHeatGun1]  +","+
                RunInfo.ShowTempComp[tcHeatGun2]  +","+
                RunInfo.ShowTempComp[tcHead1]     +","+
                RunInfo.ShowTempComp[tcHead2]     +","+
                RunInfo.ShowTempComp[tcHead3]     +","+
                RunInfo.ShowTempComp[tcHead4]     ;
            }
        }
        else
        {
            NowTemperature=NowTemperature     +","+
            RunInfo.ShowTempComp[tcHotPlate1] +","+
            RunInfo.ShowTempComp[tcHotPlate2] +","+
            RunInfo.ShowTempComp[tcShuttle1]  +","+
            RunInfo.ShowTempComp[tcShuttle2]  +","+
            RunInfo.ShowTempComp[tcChamber]   +","+
            RunInfo.ShowTempComp[tcCCD]       ;

            if(iSocketBaseTempCount==eDut4ea)                                   //JerryYang 20180417 (jou) : 補上Dut溫度log
            {
                NowTemperature=NowTemperature     +","+
                RunInfo.ShowTempComp[tcDUT1]      +","+
                RunInfo.ShowTempComp[tcDUT2]      +","+
                RunInfo.ShowTempComp[tcDUT3]      +","+
                RunInfo.ShowTempComp[tcDUT4]      ;
            }
            else if(iSocketBaseTempCount==eDut2ea)                              //Steven 20140116 : Socket數量,改用數字設定
            {
                NowTemperature=NowTemperature +","+
                RunInfo.ShowTempComp[tcDUT1]  +","+
                RunInfo.ShowTempComp[tcDUT2]  ;
            }
            else
            {
                NowTemperature=NowTemperature     +","+
                RunInfo.ShowTempComp[tcSocket];
            }

            NowTemperature=NowTemperature +","+
            RunInfo.ShowTempComp[tcAa1]   +","+
            RunInfo.ShowTempComp[tcAb1]   +","+
            RunInfo.ShowTempComp[tcAc1]   +","+
            RunInfo.ShowTempComp[tcAd1]   +","+
            RunInfo.ShowTempComp[tcAa2]   +","+
            RunInfo.ShowTempComp[tcAb2]   +","+
            RunInfo.ShowTempComp[tcAc2]   +","+
            RunInfo.ShowTempComp[tcAd2]   +","+
            RunInfo.ShowTempComp[tcBa1]   +","+
            RunInfo.ShowTempComp[tcBb1]   +","+
            RunInfo.ShowTempComp[tcBc1]   +","+
            RunInfo.ShowTempComp[tcBd1]   +","+
            RunInfo.ShowTempComp[tcBa2]   +","+
            RunInfo.ShowTempComp[tcBb2]   +","+
            RunInfo.ShowTempComp[tcBc2]   +","+
            RunInfo.ShowTempComp[tcBd2]   ;
            if(USE_16_HEATER>2)
            {
                NowTemperature=NowTemperature +","+
                RunInfo.ShowTempComp[tcAe1]   +","+
                RunInfo.ShowTempComp[tcAf1]   +","+
                RunInfo.ShowTempComp[tcAg1]   +","+
                RunInfo.ShowTempComp[tcAh1]   +","+
                RunInfo.ShowTempComp[tcAe2]   +","+
                RunInfo.ShowTempComp[tcAf2]   +","+
                RunInfo.ShowTempComp[tcAg2]   +","+
                RunInfo.ShowTempComp[tcAh2]   +","+
                RunInfo.ShowTempComp[tcBe1]   +","+
                RunInfo.ShowTempComp[tcBf1]   +","+
                RunInfo.ShowTempComp[tcBg1]   +","+
                RunInfo.ShowTempComp[tcBh1]   +","+
                RunInfo.ShowTempComp[tcBe2]   +","+
                RunInfo.ShowTempComp[tcBf2]   +","+
                RunInfo.ShowTempComp[tcBg2]   +","+
                RunInfo.ShowTempComp[tcBh2]   ;
            }
        }

        if(ATC_SYSTEM==eATCHonPrecType)                                         //Ifor 20151119 新增海思ATC2.0 溫度Log
        {
            NowTemperature=NowTemperature+ ","+
            fLotInfo->pl_ATCTempHead01   ->Caption  +","+
            fLotInfo->pl_ATCRefHead01    ->Caption  +","+
            fLotInfo->pl_ATCTempHead02   ->Caption  +","+
            fLotInfo->pl_ATCRefHead02    ->Caption  +","+
            fLotInfo->pl_ATCTempHead03   ->Caption  +","+
            fLotInfo->pl_ATCRefHead03    ->Caption  +","+
            fLotInfo->pl_ATCTempHead04   ->Caption  +","+
            fLotInfo->pl_ATCRefHead04    ->Caption  +","+
            fLotInfo->palATCWorkingTemp ->Caption  +","+
            fLotInfo->pl_ATCChillerSV   ->Caption  ;
        }
        else if(ATC_SYSTEM==eNewATCSystem)                                      //Ifor 20160303 新增 NewATC 溫度Log
        {
            if(iATC_Use_Heat_Count<=8)                                          //Ifor 20160516 add ATC 8Site 溫度Log
            {
                NowTemperature=NowTemperature+ ","+
                fLotInfo->pl_ATCTempHead01   ->Caption  +","+
                fLotInfo->pl_ATCRefHead01    ->Caption  +","+
                fLotInfo->pl_ATCTempHead02   ->Caption  +","+
                fLotInfo->pl_ATCRefHead02    ->Caption  +","+
                fLotInfo->pl_ATCTempHead03   ->Caption  +","+
                fLotInfo->pl_ATCRefHead03    ->Caption  +","+
                fLotInfo->pl_ATCTempHead04   ->Caption  +","+
                fLotInfo->pl_ATCRefHead04    ->Caption  +",";

                if(iATC_Use_Heat_Count>4)
                {
                    NowTemperature=NowTemperature+
                    fLotInfo->pl_ATCTempHead05   ->Caption  +","+
                    fLotInfo->pl_ATCRefHead05    ->Caption  +","+
                    fLotInfo->pl_ATCTempHead06   ->Caption  +","+
                    fLotInfo->pl_ATCRefHead06    ->Caption  +","+
                    fLotInfo->pl_ATCTempHead07   ->Caption  +","+
                    fLotInfo->pl_ATCRefHead07    ->Caption  +","+
                    fLotInfo->pl_ATCTempHead08   ->Caption  +","+
                    fLotInfo->pl_ATCRefHead08    ->Caption  +",";
                }
                NowTemperature=NowTemperature+fLotInfo->palATCWorkingTemp ->Caption;
            }
            else
            {
//                NowTemperature=NowTemperature+ ","+
            }
        }

        if(BAR_CODE_INSTALL==ebctInShtIntel)
        {
            NowTemperature=NowTemperature +","+ RunInfo.ShowTempComp[tc2D];
        }

        if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                     //kevin 20170418 (wei) add 新增溫度log 參數
        {
            if(LastSet.iTester==OFF_LINE)
                sBuffer = ",Offline";
            else
                sBuffer = ",Online";                                            //kevin 20190615 add
            NowTemperature+= sBuffer;                                           //kevin 20190615 change
        }

        if(INSTALL_HEAT_GUN>0 && Temperature.bActiveHeatGun)                    //kevin 20190615 add hot gun log
        {
            NowTemperature+=","+RunInfo.ShowTempComp[tcHeatGun1]+","+RunInfo.ShowTempComp[tcHeatGun2]; //kevin 20191226 delete ,
        }
        else
        {
            NowTemperature+=",---,---"; //kevin 20191226 delete ,
        }
    }
    NowTemperature=NowTemperature +","+ sHisiAddTempLog(true)+",";              //kevin 20191210 ASE KH HIS add TEMP LOG
    for(int i=0; i<tcTotalCount; i++)                                           //kevin 20191213   //StevenHong 20260119 : 溫度Log內容無資料都要顯示---, tcTotalCount-2 -> tcTotalCount
    {
        if((i==26 ||i==27) && Temperature.bActiveHeatGun==false)                //kevin 20191214 add
        {
            NowTemperature=NowTemperature+"---,";
            continue;
        }

        if(bUT150Install[i])
            NowTemperature=NowTemperature+Temperature.fTempOffSet[3][i]+",";
        else
            NowTemperature=NowTemperature+"---,";
    }
    return NowTemperature;
}
//---------------------------------------------------------------------------
bool __fastcall TFormHS::CheckSetupNamelist_Hisi()                              //Ifor 20160310 add for Hisi Setup name check start
{
    TStringList *CheckFileList;
    AnsiString FileName=asSetupFileCheckList;                                   //Ifor 20200914 add:Setup File Check List
    AnsiString DirPath="D:\\HT9045\\system\\";
    AnsiString FilePath=DirPath+FileName;
    AnsiString CheckFileName;
    CheckFileList = new TStringList();

    if(FileExists(FilePath)==false)
        WriteDataToFile(FilePath.c_str(), "");

    CheckFileList->LoadFromFile(FilePath);
    for(int i=0; i<CheckFileList->Count; i++)
    {
        CheckFileName=CheckFileList->Strings[i].UpperCase();
        CheckFileName=CheckFileName.Trim();
        CheckFileList->Strings[i]=CheckFileName;
    }

    if(CheckFileList->IndexOf(fMain->cbSetupFileName->Text)==-1)                //不在名單內
    {
        if(ATC_SYSTEM!=eATCHonPrecType && ATC_SYSTEM!=eNewATCSystem)            //ChungHung 20160218 modify || ---> &&
        {
            CheckFileList->Clear();                                             //ChungHung 20160225 add
            delete CheckFileList;                                               //ChungHung 20160225 add
            return false;
        }
    }
    CheckFileList->Clear();                                                     //ChungHung 20160225 add
    delete CheckFileList;                                                       //ChungHung 20160225 add
    return true;
}
//---------------------------------------------------------------------------
bool __fastcall TFormHS::CheckCanRunStart_HS()
{
    AnsiString str1, str2;
    bResetGalilTwoYMove=true;                                                   //Ifor 20220729 add Reset Galil Two Y Move時間計時

    if(Temperature.bATCActiveCooling==true &&                                   //Ifor 20160826 移動海思判斷條件至判斷式內
       fLotInfo->aldATCPower->Value==true)
    {
        if(ATC_SYSTEM==eATCHonPrecType || ATC_SYSTEM==eNewATCSystem)
        {
            int iATCSelfTestStatusCheck=0;
            if(ATC_SYSTEM==eNewATCSystem)                                       //Ifor 20161118 add KYEC Start 時送出 Lot ID 給ATC系統(By Lot ATC Even Log)
            {
                if(CosFunction.bHiSiliconFunction==true ||
                   bUseATC_SelfTestFunction==true)                              //Ifor 20170202 (Steven) add KYEC 一般版本 Self Test 開關
                    iATCSelfTestStatusCheck=ATC_InterfaceForm->GetRunSelfTestStatus();//Ifor 20160824 add Get ATC Self Test Result

                if(CUSTOMER_CODE==CC_KYEC_LEE)
                {
                    if(ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_31   ||      //Ifor 20180419 : add 傳送 Lot ID 給 ATC 3.2
                       ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_32   )
                    {
                        ATC_InterfaceForm->LotStart(asATCEvenLotID);            //Ifor 20161118 (Steven) add Lot ID 傳送至 ATC3.1 System    //Ifor 20170125 (Steven) 修改變數名稱避免Tray Feed 後元件資料被清空 fLotInfo->edtSysLotID->Text =>asATCEvenLotID
                        bSendATCLotStart=true;                                  //Ifor 20161118 (Steven) Set Send ATC Lot Start Flag
                        bSendATCLotEnd=false;                                   //Ifor 20161118 (Steven) Clean Send ATC Lot End Flag
                    }
                }
            }
            else
            {
                if(CosFunction.bHiSiliconFunction==true ||
                   bUseATC_SelfTestFunction==true)                              //Ifor 20170202 (Steven) add KYEC 一般版本 Self Test 開關
                    iATCSelfTestStatusCheck=ATCInterfaceForm->GetRunSelfTestStatus();     //Ifor 20160824 add Get ATC Self Test Result

                if(CUSTOMER_CODE==CC_KYEC_LEE)                                  //Ifor 20161117 add Lot ID 傳送至 ATC2.0 System
                {
                    ATCInterfaceForm->LotStart(asATCEvenLotID);                 //Ifor 20161118 (Steven) add Lot ID 傳送至 ATC2.0 System
                    bSendATCLotStart=true;                                      //Ifor 20161118 (Steven) Set Send ATC Lot Start Flag
                    bSendATCLotEnd=false;                                       //Ifor 20161118 (Steven) Clean Send ATC Lot End Flag
                }
            }

            if(iATCSelfTestStatusCheck!=0)
            {
                bNeedWaitATCRunSelfTestFinish=true;
                FormHS->Show();
            }
        }
    }

    if(CosFunction.bHiSiliconFunction==false &&                                 //Ifor 20160818 一般版本不可跑海思工作擋 TfMain::Start ==> TFormHS::CheckCanRunStart_HS
       CUSTOMER_CODE==CC_KYEC_LEE)                                              //Ifor 20170424 (wei) KYEC 要求海思版本新增客戶碼
    {
        if(fMain->cbSetupFileName->Text.Pos("KL")==1   ||                       //Ifor 20200914 Fix: HISI => KL
//           fMain->cbSetupFileName->Text.Pos("Hisi")==1 ||
//           fMain->cbSetupFileName->Text.Pos("hisi")==1 ||
           fMain->cbSetupFileName->Text.Pos("9203")==1 ||
           fMain->cbSetupFileName->Text.Pos("5611")==1 ||
           fMain->cbSetupFileName->Text.Pos("9287")==1 ||
           fMain->cbSetupFileName->Text.Pos("3971")==1 ||
           fMain->cbSetupFileName->Text.Pos("9606")==1 ||
           fMain->cbSetupFileName->Text.Pos("9378")==1 ||                       //Ifor 20190711 : KYEC 要求新增客戶代碼9378
           fMain->cbSetupFileName->Text.Pos("1634")==1 ||                       //Ifor 20210930 : KLT 要求新增客戶代碼1634
           fMain->cbSetupFileName->Text.Pos("1633")==1 ||                       //Ifor 20210930 : KLT 要求新增客戶代碼1633
           fMain->cbSetupFileName->Text.Pos("1665")==1 ||                       //Ifor 20220406 : KLT 要求新增客戶代碼1665
           fMain->cbSetupFileName->Text.Pos("1667")==1 ||                       //Ifor 20220406 : KLT 要求新增客戶代碼1667
           fMain->cbSetupFileName->Text.Pos("1752")==1 ||                       //Ifor 20221208 : KLT 要求新增客戶代碼1752
           fMain->cbSetupFileName->Text.Pos("1783")==1  )                       //Ifor 20230227 : KLT 要求新增客戶代碼1783
        {
            ShowMyMessage("This Setup file for KL dedicated!!!Please check again");  //Ifor 20200914 Fix: HISI => KL
            return true;
        }
    }

    if((CUSTOMER_CODE==CC_KYEC_LEE ||                                           //Ifor 20160818 Handler Start 切換至ATC頁面 並啟動 ATC 與 Chiller   TfMain::Start ==> TFormHS::CheckCanRunStart_HS
        CosFunction.bHiSiliconFunction==true) &&
       ATC_SYSTEM!=eATCUninstall)
    {
        if(ATC_SYSTEM!=eNonChamber && Temperature.bATCActiveCooling==true)
            fLotInfo->pgLotinfo->ActivePageIndex=5;
        if(ATC_SYSTEM==eATCHonPrecType)
        {
            if(ATCInterfaceForm->ATC_SYS_PAL[0]->LedATCConnect->Value==true)
            {
                ATCInterfaceForm->ATCChillerSwitch(true);                       //Ifor 20160309 Start時強制開啟ATC2.0 Chiller
                ATCInterfaceForm->SetRunATC(true);                              //Ifor 20160309 Start時強制開啟ATC2.0 RUN
            }
            bRunATC=true;                                                       //ChungHung 20160118 add for Hisi V102
        }
        else if(ATC_SYSTEM==eNewATCSystem)
        {
            if(ATC_InterfaceForm->IsConnect()==true)
                ATC_InterfaceForm->Run();                                       //Ifor 20160309 Start時強制開啟ATC3.1 RUN
        }
        bRunATC=true;                                                           //ChungHung 20160118 add for Hisi V102
    }

    if(CosFunction.bHiSiliconFunction==true &&
       Temperature.bATCActiveCooling==true)                                     //Ifor 20160910 海思版本 && 啟動ATC Function 才檢查版本別是否正確
    {
        if(ATC_SYSTEM==eATCHonPrecType || ATC_SYSTEM==eNewATCSystem)
        {
            if(iATCForHSMode==0)
            {
                if(CUSTOMER_CODE==CC_KYEC_LEE)                                  //Ifor 20230608 add:KYEC要求改用Message
                {
                    ShowMyMessage("Please Switch ATC On-Line!!", "請將ATC On-Line!!");
                }
                else
                {
                    ShowErrorMessage("WAR15311", 0, MMATC_Handler, false, "START");//ATC Not Running!! Please confirm whether to open ATC
                }
                return true;
            }
            else if(iATCForHSMode==2)
            {
                ShowErrorMessage("WAR15312", 0, MMATC_ATC, false, "START");     //ATC Version Error!! Please Check ATC Version!! Must for HS Ver
                return true;
            }
            else                                                                //Ifor 20160727 add 判斷ATC 版號是否與Handler版號相同，不同不可跑
            {
                int iCheckATCVer=0;
                AnsiString strHandlerVerName=asHandlerVersion;                  //Ifor 20161109 Handler Version Modify
                strHandlerVerName=strHandlerVerName.SubString((strHandlerVerName.Length())-10, 6);

                iCheckATCVer=0;                                                 //Ifor 20180808 : add 修改ATC版號再指定版本以上皆可跑
                iCheckATCVer=atoi(sATCVerRead.SubString(11,2).c_str())*100;
                iCheckATCVer=iCheckATCVer+atoi(sATCVerRead.SubString(14,2).c_str());
                if(iCheckATCVer<atoi(AnsiString(HISI_ATCVER).c_str()))
                {
                    ShowErrorMessage("WAR15312", 0, MMATC_ATC, false, "START");//ATC Version Error!! Please Check ATC Version!! Must for HS Ver
                    return true;
                }
            }
        }
    }

    if(ATC_SYSTEM==eATCHonPrecType)                                             //Ifor 20160223 啟動時檢查ATC 2.0 & 7.0 是否連線
    {
        if(Temperature.bATCActiveCooling==true)
        {
            if(ATCInterfaceForm->ATC_SYS_PAL[0]->LedATCConnect->Value==false ||
               bStartATCRun==false)                                             //Ifor 20160830 add ATC2.0 啟動判斷 bStartATCRun
            {
                if(CUSTOMER_CODE==CC_KYEC_LEE)                                  //Ifor 20230608 add:KYEC要求改用Message
                {
                    ShowMyMessage("Please Switch ATC On-Line!!", "請將ATC On-Line!!");
                }
                else
                {
                    ShowErrorMessage("WAR15311", 0, MMATC_Handler, false, "START");//ATC Not Running!! Please confirm whether to open ATC
                }
                return true;
            }
        }
        else if(Temperature.bATC70Active==true)
        {
            if(fLotInfo->aldATCPower->Value==false)
            {
                if(CUSTOMER_CODE==CC_KYEC_LEE)                                  //Ifor 20230608 add:KYEC要求改用Message
                {
                    ShowMyMessage("Please Switch ATC On-Line!!", "請將ATC On-Line!!");
                }
                else
                {
                    ShowErrorMessage("WAR15311", 0, MMATC_Handler, false, "START");//ATC Not Running!! Please confirm whether to open ATC
                }
                return true;
            }
        }
    }

    if(ATC_SYSTEM==eNewATCSystem && Temperature.bATCActiveCooling==true)        //Ifor 20160223 啟動時檢查ATC 3.0 是否連線
    {
        if(bStartATCRun==false)                                                 //Ifor 20160830 Mark 整合ATC 按鍵功能 cbActiveNewATC->Checked ==> bStartATCRun
        {
            if(bATCHasAlarmBinNeedToError==true)
            {
                str1.sprintf("Please confirm whether ATC is connected");
                str2.sprintf("請確認ATC 是否連線");
                ShowMyMessage(str1,str2);
            }
            else
            {
                if(CUSTOMER_CODE==CC_KYEC_LEE)                                  //Ifor 20230608 add:KYEC要求改用Message
                {
                    ShowMyMessage("Please Switch ATC On-Line!!", "請將ATC On-Line!!");
                }
                else
                {
                    ShowErrorMessage("WAR15311", 0, MMATC_Handler);             //ATC Function Not yet started!! Please Check ATC Active!
                }
            }
            return true;
        }
        if(ATC_InterfaceForm->IsConnect()==false)
        {
            if(LastSet.iTester==ON_LINE)                                        //JerryYang 20230322 : 新增2DID模式, != offline改為 ==online  //Ifor 20180222 (wei) :HT7045 Offline 不判斷 ATC是否連線
            {
                if(CUSTOMER_CODE==CC_KYEC_LEE)                                  //Ifor 20230719 add:KYEC要求改用Message
                {
                    if(bStartATCRun)
                        ShowMyMessage("ATC System Connect Error!!", "ATC 系統連線異常!!");
                    else
                        ShowMyMessage("Please Switch ATC On-Line!!", "請將ATC On-Line!!");
                }
                else
                {
                    ShowErrorMessage("WAR15309", 0, MMATC_TCPIP, false, "START");  //ATC connect error, Please confirm whether to open ATC
                }
                return true;
            }
        }
    }

    if(IniConfig.bSPILFunction==true ||                                         //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction
       CUSTOMER_CODE==CC_SIGURD_ChungXing)                                      //JerryYang 20170324 add矽格中興
    {                                                                           //Ifor 20160408 SPIL 無使用Lot，Log 記錄修改至Start時啟動 TfMain::Start ==> TFormHS::CheckCanRunStart_HS
        bSysLotStart=true;                                                      //Ifor 20160302 add for KYEC_HS LotStart
        bEPLogStart_KYEC=true;                                                  //Ifor 20160302 add for KYEC_HS EPLogStart
        bTempLogStart_KYEC=true;                                                //Ifor 20160302 add for KYEC_HS TempLogStart
        if(USE_NOVX3360==true)
        {
            bESDLogStart_KYEC=true;                                             //Ifor 20160302 add for KYEC_HS ESD
        }
    }

    if(CUSTOMER_CODE==CC_KYEC_LEE &&                                            //Ifor 20180108 (Steven) : add KYEC 要求 ESD 版本卡控
       USE_NOVX3360==true)
    {
        #ifndef SOFT_SIMULTE
        int iCheckESDVer=atoi(RunInfo.ESDSoftwareVersion.SubString(7,3).c_str());
        if(bEnable_KLT_Function==true)                                          //Ifor 20200111 add: KLT 要求ESD 卡控 5V ==> 10V
        {
            if(iCheckESDVer!=590)
            {
                str1.sprintf("Please confirm the ESD version, you must use 590 version");
                str2.sprintf("請確認ESD版本，您必須使用590版本");
                ShowMyMessage(str1,str2);
                return true;
            }
        }
        else
        {
            if(iCheckESDVer!=594)                                               //Ifor 20210302 add:KYEC 新增 ESD Auto Decay 591=>592
            {
                str1.sprintf("Please confirm the ESD version, you must use 594 version");
                str2.sprintf("請確認ESD版本，您必須使用594版本");
                ShowMyMessage(str1,str2);
                return true;
            }
        }
        #endif
    }

    if(CUSTOMER_CODE==CC_KYEC_LEE  &&
       bEnable_KLT_Function==false &&
       BAR_CODE_INSTALL==ebctUseCCDMode &&
       TestIF_File.bEnableBarCode==true)
    {
        bool b2DHasErr=false;
        double d2DVisionVer=atof(RunInfo.BarcodeSoftwareVersion.c_str());
        if(d2DVisionVer>=2)                                                     //Ifor 20240318 2D 1.0/2.0 版本卡控
        {
            if(d2DVisionVer>=2.3)
            {

            }
            else
            {
                b2DHasErr=true;
            }
            if(b2DHasErr==true)
            {
                str1.sprintf("Please confirm the 2D Barcode version, you must use 2.3 version");
                str2.sprintf("請確認2D Barcode版本，您必須使用2.3版本");
                ShowMyMessage(str1,str2);
                return true;
            }
        }
        else
        {
            if(d2DVisionVer>=1.81)
            {

            }
            else
            {
                b2DHasErr=true;
            }
            if(b2DHasErr==true)
            {
                str1.sprintf("Please confirm the 2D Barcode version, you must use 1.81 version");
                str2.sprintf("請確認2D Barcode版本，您必須使用1.81版本");
                ShowMyMessage(str1,str2);
                return true;
            }
        }
    }

    if(CUSTOMER_CODE==CC_KYEC_LEE &&
       bEnable_KLT_Function==false &&
       fCleaning->Visible)
    {
        str1.sprintf("Please close the Auto Clean window before starting");
        str2.sprintf("Start前請關閉Auto Clean視窗");
        ShowMyMessage(str1,str2);
        return true;
    }

    if(USE_ATC_RS232_Check==true)                                               //Ifor 20211116 add: ATC RS232 Check Function
    {
        if(ATC_SYSTEM==eNewATCSystem &&
           Temperature.bATCActiveCooling==true)                                 //Ifor 20200407 : Fix ATC不使用還是會Alarm RS232 未連線問題
        {
            if(fMain->bAMDRs232ConnectError==true &&
               TestIF_File.i2DIDFormat==eAMD)                                   //JerryYang 20200422 2DID format選項改用下拉選單
            {
                str1.sprintf("Please confirm whether the ATC RS232 connector is connected");
                str2.sprintf("請確認 ATC RS232 接頭是否連接");
                ShowMyMessage(str1,str2);
                return true;
            }
        }
    }

    if(IniConfig.bD63CheckIndexZHomeToZPhaseDistanceRange==true)
    {
        AnsiString str="",sTime="";
        if(fContact->fShow==false || (fContact->fShow==true && iContactMode!=CONTACT_AUTO_GET_HEIGHT))
        {
            if(bZ1ModifyDistanceRef==true || bZ2ModifyDistanceRef==true)
            {
                ShowMyMessage("ZPhase has SKIP must do auto height again!", "ZPhase有SKIP.須重新做Auto Height!", "CheckCanRunStart");
                return true;
            }
        }

        if(bY1ModifyDistanceRef==true || bY2ModifyDistanceRef==true)
        {
            str.sprintf("%s (Start Button Press)Error!! Now Teaching Position Y1:Front:%d,Middle:%d; Y2:Middle:%d,Rear:%d",sTime, Tech.iHT9040TestY1_Front, Tech.iHT9040TestY1_Middle, Tech.iHT9040TestY2_Middle, Tech.iHT9040TestY2_Rear);
            SaveFile(asIndexZphasePath,str);//kevin 20170511

            if(bY1ModifyDistanceRef==true && bY2ModifyDistanceRef==true)
            {
                ShowMyMessage("YPhase has SKIP. Please check index Y1 and Y2 teaching Position!", "Y1 and Y2 Phase有SKIP.請重新確認Idex Y軸教導點位", "CheckCanRunStart");
                return true;
            }
            else if(bY1ModifyDistanceRef==true)
            {
                ShowMyMessage("YPhase has SKIP. Please check index Y1 teaching Position!", "Y1 Phase有SKIP.請重新確認Idex Y軸教導點位", "CheckCanRunStart");
                return true;
            }
            else//(bY2ModifyDistanceRef==true)
            {
                ShowMyMessage("YPhase has SKIP. Please check index Y2 teaching Position!", "Y2 Phase有SKIP.請重新確認Idex Y軸教導點位", "CheckCanRunStart");
                return true;
            }
        }
    }
    
    if(IniConfig.bEnableRmsCheckSetupFile==true)                                //Ifor 20230516 add: TFAMD 要求工作檔驗證
    {
        if(fLotInfo->edDeviceID->Text=="")
        {
            ShowMyMessage("Please Scan Device ID Info!", "請掃描設備ID資訊");
            return true;
        }
        AnsiString asSourcePath, str1, str2;
        asSourcePath=IniConfig.sRmsPath+"\\Barcode.ini";

        if(FileExists(asSourcePath))
        {
            str1=ReadIniData(asSourcePath, "Barcode", fLotInfo->edDeviceID->Text, AnsiString("99999"));
            if(str1=="99999")
            {
                ShowMyMessage("Undefined Device ID.", "未定義的設備 ID.");
                return true;
            }
            else
            {
                if(str1.Pos(fMain->cbSetupFileName->Text)==0)
                {
                    ShowMyMessage("Scan In Info Not Match With The Setup File.", "掃描資訊與工作檔文件不匹配.");
                    return true;
                }
            }
        }
        else
        {
            ShowMyMessage("Barcode.ini file is not found in the configured path.", "設定的路徑中找不到 Barcode.ini 文件。");
            return true;
        }
    }

    if(AUTO3_IS_MAGAZINE==1)                                                    //Ifor 20240802 add:START 前確認 Setup File 是否為Magazine
    {
        if(iMagazineStatus!=0 &&                                                //JerryYang 20250616 : add
          (iCatchTrayChangeTrayTask==2004 || iAuto3MagazineTask==2004) &&
           bHasOpenMagDoor==false)
        {
            AnsiString str1;
            str1.sprintf("Magazine tray out, please remove the tray in slot.");
            ShowMyMessage(str1, asMagazineBinFull);
            return true;
        }

//        if(fBinSel->sMagazineSetup[iTestRunMode]->CommaText != "1")           //JerryYang 20250917 : 暫時MARK掉
//        {
//            ShowMyMessage("Bin setting error, please reconfigure the Bin data.", "Tools > Bin > Check Bin data then Save!!");
//            return true;
//        }
        if(iMagazineStatus==3 && bHasOpenMagDoor==false)
        {
            str1.sprintf("please open door Check Magazine%d Has Tray", iAuto3MagazineIndex+1);
            str2.sprintf("請開門確認Magazine%d是否有Tray", iAuto3MagazineIndex+1);
            ShowMyMessage(str1, str2);
            return true;
        }
    }
    else
    {
        if(fBinSel->sMagazineSetup[iTestRunMode]->CommaText == "1")
        {
//            ShowMyMessage("Bin setting error, please reconfigure the Bin data.", "Bin設定錯誤，請重新設定Bin資料。");
            ShowMyMessage("Bin setting error, please reconfigure the Bin data.", "Tools > Bin > Check Bin data then Save!!");
            return true;
        }
    }

    if(CUSTOMER_CODE==CC_KYEC_LEE &&
       bEnable_KLT_Function==false &&
       Temperature.bATCActiveCooling==true)                                     //Ifor 20230410 add: 按下Start確認ATC溫度是否達到設定
    {
        if(ATC_SYSTEM==eATCHonPrecType || ATC_SYSTEM==eNewATCSystem)
        {
            bCheckATCTemp=true;
        }
        else
        {
            bCheckATCTemp=false;
        }
    }
    else
    {
        bCheckATCTemp=false;
    }

    if(CUSTOMER_CODE==CC_KYEC_LEE &&
       bEnable_KLT_Function==true)                                              //Ifor 20210911 add: KLT 要求out shuttle loss IC alarm之後，按下Start要跳出確認視窗
    {
        int ret=0;
        if(bHasOutShuttleLose==true)
        {
            bHasOutShuttleLose=false;
            str1.sprintf("Out Shuttle loss IC, please check if there is any residual IC in the socket");
            str2.sprintf("Out Shuttle 遺失IC，請檢查Socket中是否有殘留IC");
            ShowMyMessage(str1,str2);
            return true;
        }

        if(HasICUnderMachine()==false && HasAnyICInMachine()==false && LastSet.iTester==OFF_LINE)  //Ifor 20230614 add: KLT 要求生產前測試機Offlin 跳出提示
        {
            ret=ShowMyMessageBox_YES_NO("目前為offline狀態，請確認是否進行測試","");
            if(ret==2)
            {
                return true;
            }
        }
    }

    if(TestIF_File.bRENESAS_EnableFTCT==true)                                   //RogerYang 20250916 : 瑞薩FT-CT
    {
        if(bAutoRestartAfterFTCTAlarm==true) //by 90 alarm
        {

        }
        else
        {
            if(fSCKART->iManualStart==false)                                        //FTCT模式下，只有觸發過start之後才允許再啟動
            {
                str1.sprintf("Start blocked by FTCT command(%s)",
                fMain->RENESAS_Server->ServerFrm->asHandlerLastSeqStatus);
                ShowMyMessage(str1);
                return true;
            }
            fMain->RENESAS_Server->FTCTManStartLock();   //復原原本的ManualStart狀態
        }
    }

    fMain->LabHisi_Set->Caption="NA";                                           //Ifor 20200811 add: 清除顯示資料
    iIonAlarmState=0;                                                           //0:初始化 1:設定時間 2:完成延遲
    bResetMagGetTime=true;
    bDoRTCVerify=true;
    return false;
}
//---------------------------------------------------------------------------
bool __fastcall TFormHS::CheckSetupFileData(AnsiString strSetupFile)            //Ifor 20160822 add 判斷Setup File 檔案是否遺失 Start
{
    bool bHasError=false;

    TStringList *lstFiles = new TStringList;
    TSearchRec sr;
    AnsiString strSetupFileDir="";
    strSetupFileDir.sprintf("%s%s", DataPath, strSetupFile);

    if(FindFirst(strSetupFileDir+"\\*.*", faAnyFile, sr)==0)
    {
        do
        {
            if(sr.Name!="." && sr.Name!="..")
                lstFiles->Add(sr.Name);
        }while(FindNext(sr)==0);
        FindClose(sr);
    }
    if(lstFiles->Count<8)
    {
        InitialOK=true;                                                         //Ifor 20160822 讓Message 可以顯示
        bHasError=true;
        if(fMain->iHasChangeFile!=9)
        {
            ShowMyMessage("Setup file error, Please check\r\n"+strSetupFileDir+"\r\n"+"Whether the missing files");
        }
        fMain->iHasChangeFile=2;
    }
    lstFiles->Clear();                                                          //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    delete lstFiles;
    return bHasError;
}
//---------------------------------------------------------------------------
void __fastcall TFormHS::ShowATCSelfTestSatus(bool bFlash, bool bHasAlarm)      //Ifor 20160829 add Show ATC Self Test Satus
{
    int iATCSelfTestStatusCheck=0;
    int iATCSelfTestResultCheck=0;

    if((CosFunction.bHiSiliconFunction==true ||
        bUseATC_SelfTestFunction==true) &&
       Temperature.bATCActiveCooling==true &&
       fLotInfo->aldATCPower->Value==true)                                      //Ifor 20170202 (Steven) add KYEC ATC Self Test 燈號顯示 for 一般版本使用開關切換
    {
        if(ATC_SYSTEM==eNewATCSystem)
        {
            iATCSelfTestResultCheck=ATC_InterfaceForm->GetRunSelfTestResult();  //Ifor 20160824 add Get ATC Self Test Result
            iATCSelfTestStatusCheck=ATC_InterfaceForm->GetRunSelfTestStatus();  //Ifor 20160824 add Get ATC Self Test Result
        }
        else
        {
            iATCSelfTestResultCheck=ATCInterfaceForm->GetRunSelfTestResult();   //Ifor 20160824 add Get ATC Self Test Result
            iATCSelfTestStatusCheck=ATCInterfaceForm->GetRunSelfTestStatus();   //Ifor 20160824 add Get ATC Self Test Result
        }

        switch(iATCSelfTestStatusCheck)                                         //Ifor 20160824 add ATC Self Test Status 顯示
        {
            case 0:
                pl_ATCSelfTestSatus_01->Color=clBtnFace;
                pl_ATCSelfTestSatus_02->Color=clBtnFace;
                pl_ATCSelfTestSatus_03->Color=clBtnFace;
                break;
            case 1:                                                             //Ifor 20160824 add ATC Self Test Run +5C
                if(bFlash)
                    pl_ATCSelfTestSatus_01->Color=clYellow;
                else
                    pl_ATCSelfTestSatus_01->Color=clBtnFace;
                pl_ATCSelfTestSatus_02->Color=clBtnFace;
                pl_ATCSelfTestSatus_03->Color=clBtnFace;
                break;
            case 2:                                                             //Ifor 20160824 add ATC Self Test Run -5C
                pl_ATCSelfTestSatus_01->Color=clLime;
                if(bFlash)
                    pl_ATCSelfTestSatus_02->Color=clYellow;
                else
                    pl_ATCSelfTestSatus_02->Color=clBtnFace;
                pl_ATCSelfTestSatus_03->Color=clBtnFace;
                break;
            case 3:                                                             //Ifor 20160824 add ATC Self Test Run +0C
                pl_ATCSelfTestSatus_01->Color=clLime;
                pl_ATCSelfTestSatus_02->Color=clLime;
                if(bFlash)
                    pl_ATCSelfTestSatus_03->Color=clYellow;
                else
                    pl_ATCSelfTestSatus_03->Color=clBtnFace;

                if(iATCSelfTestResultCheck==5)                                  //Ifor 20160830 若ATC Self Test 完成狀態需要Show綠燈
                    pl_ATCSelfTestSatus_03->Color=clLime;
                break;
            case 4:
                if(FormHS->Visible==true)
                    Close();
                break;
        }

        if(bHasAlarm)
            pl_ATCSelfTestSatus_99->Color=clRed;
        else
            pl_ATCSelfTestSatus_99->Color=clBtnFace;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormHS::LoaderBufPreAlarm()                                    //Ifor 20160829 add Loader 最後一盤入料Alarm不停機
{
    AnsiString str1, str2;
    static bool bHasLoaderPreAlarm=false;                                       //Ifor 20160901 add 一個Tray Alarm 僅Alarm 一次

    if(IniConfig.bP31LoaderTryLastOneFeedContinueRun==true)
    {
        if(Sen[SnLoaderTrayHasTray_ART].Enable)                                 //Ifor 20191125 : add ART Loader 雙 Sensor mode
        {
            if(IniConfig.bLastLoaderNoInSide==false &&
               Sen[SnLoaderTrayHasTray].IsOn()   &&
               ((Sen[SnLoaderTrayHasTray_ART].IsOff() &&
                 Sen[SnLoaderCarHasTray].IsOn()) ||                             //Ifor 20160901 Modify Loader Car Has Try && Loader Tray 無Try才Alarm
                (Sen[SnLoaderIsPreAlarm].Enable &&
                 Sen[SnLoaderIsPreAlarm].IsOn())))                              //wei 20170802 Pre alarm sensor
            {
                if(bHasLoaderPreAlarm==false)
                {
                    if(MyMessageBox->Visible==true)                             //Ifor 20160901 Alarm 前若form有開啟先關閉
                        MyMessageBox->Close();
                    bHasLoaderPreAlarm=true;
                    iUnLoaderCount=8;                                           //Ifor 20160829 必須不為0 Handler才不停機
                    str1.sprintf("Loader Will No Tray Soon,Please put Loader Tray first");
                    str2.sprintf("Loader 很快就要無Tray盤，請放Tray至Loader");
                    RunInfo.iPreAlarmPosition=1;                                //Ifor 20171024 (wei) : add Pre Alarm Position 1:LD 2:Auto1 3:Auto2 4:Auto3 5:Fix1 6:Fix2 7:Fix3 8:Empty 9:Color
                    ShowUnloaderTrayMessage(str1, str2);
                }
            }
            else
            {
                bHasLoaderPreAlarm=false;
            }
        }
        else
        {
            if(IniConfig.bLastLoaderNoInSide==false &&
               ((Sen[SnLoaderTrayHasTray].IsOff() &&
                 Sen[SnLoaderCarHasTray].IsOn()) ||                             //Ifor 20160901 Modify Loader Car Has Try && Loader Tray 無Try才Alarm
                (Sen[SnLoaderIsPreAlarm].Enable &&
                 Sen[SnLoaderIsPreAlarm].IsOn())))                              //wei 20170802 Pre alarm sensor
            {
                if(bHasLoaderPreAlarm==false)
                {
                    if(MyMessageBox->Visible==true)                             //Ifor 20160901 Alarm 前若form有開啟先關閉
                        MyMessageBox->Close();
                    bHasLoaderPreAlarm=true;
                    iUnLoaderCount=8;                                           //Ifor 20160829 必須不為0 Handler才不停機
                    str1.sprintf("Loader Will No Tray Soon,Please put Loader Tray first");
                    str2.sprintf("Loader 很快就要無Tray盤，請放Tray至Loader");
                    RunInfo.iPreAlarmPosition=1;                                //Ifor 20171024 (wei) : add Pre Alarm Position 1:LD 2:Auto1 3:Auto2 4:Auto3 5:Fix1 6:Fix2 7:Fix3 8:Empty 9:Color
                    ShowUnloaderTrayMessage(str1, str2);
                }
            }
            else
            {
                bHasLoaderPreAlarm=false;
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormHS::FixTrayPreAlarm()                                      //Ifor 20160829 add Fix Tray 剩餘設定顆數Alarm 不停機
{
    static bool bHasFixTrayPreAlarm[MAX_FIX_TRAY]={false, false, false, false, false, false};      //Ifor 20160901 add 一個Tray Alarm 僅Alarm 一次

    if(IniConfig.bP30FixTryCheckRemainingAmount==false)
        return;

    AnsiString str1, str2;
    int iFix, iPreAlarm[MAX_FIX_TRAY]={5, 6, 7, 13, 14, 15};                    //Ifor 20171024 (wei) : add Pre Alarm Position 1:LD 2:Auto1 3:Auto2 4:Auto3 5:Fix1 6:Fix2 7:Fix3 8:Empty 9:Color 10:Auto4 11:Auto5 12:Auto6 13:Fix4 14:Fix5 15:Fix6
    int pcs=IniConfig.iP30FixCheckRemainingAmountInterval;

    for(int i=iFixMin; i<=iFixMax; i++)
    {
        iFix=iAutoIndex[i];
        if(MOT[iMMAuto[i]].fHasTray==true &&
           MOT[iMMAuto[i]].HowManyDevice(NULL_IC)<=pcs)
        {
            if(bHasFixTrayPreAlarm[iFix]==false &&
               Sen[SnFixedTrayDetect[iFix]].IsOn())                             //Ifor 20160901 add 有實體Tray才Pre Alarm
            {
                if(MyMessageBox->Visible==true)                                 //Ifor 20160901 Alarm 前若form有開啟先關閉
                    MyMessageBox->Close();
                bHasFixTrayPreAlarm[iFix]=true;
                iUnLoaderCount=8;                                               //Ifor 20160829 必須不為0 Handler才不停機
                str1.sprintf("%s Tray Will Full", s6TrayName[i]);
                str2.sprintf("%s Tray 即將滿盤", s6TrayName[i]);
                RunInfo.iPreAlarmPosition=iPreAlarm[i];                         //Ifor 20171024 (wei) : add Pre Alarm Position 1:LD 2:Auto1 3:Auto2 4:Auto3 5:Fix1 6:Fix2 7:Fix3 8:Empty 9:Color 10:Auto4 11:Auto5 12:Auto6 13:Fix4 14:Fix5 15:Fix6
                ShowUnloaderTrayMessage(str1, str2);
            }
        }
        else
        {
            bHasFixTrayPreAlarm[iFix]=false;
        }
    }
}
//---------------------------------------------------------------------------
int __fastcall TFormHS::ShowATCAlarmPosition(int iCode)                         //Ifor 20160830 add ATC Alarm 位置顯示
{
    int iATCPos=0;
    //Ifor 20170330 (wei) add ATC Alarm Code 相關說明
    //======MMATC_TCPIP======//
    //WAR15230 ATC system alarm: Network disconnect!!
    //======MMATC_WaterValve======//
    //WAR15223 ATC system alarm: <ALM06>Water valve motor error
    //======MMATC_RS232======//
    //WAR15227 ATC system alarm: ATC3.1 module connect fail!!
    //WAR15228 ATC system alarm: ATC Command time out!!
    //WAR15229 ATC system alarm: Receive #SETFAIL!!
    //======MMATC_NI======//
    //WAR15209 ATC system alarm: Ts Thermocouple Continuous Error
    //WAR15224 ATC system alarm: NI module initial fail!!
    //WAR15226 ATC system alarm: DA module connect fail!!
    //======MMATC_Head======//
    //WAR15205 ATC system alarm: Water temperature too high!!
    //WAR15206 ATC system alarm: Temperature over setting !!
    //WAR15207 ATC system alarm: Tc Thermocouple Continuous Error
    //WAR15208 ATC system alarm: Ts thermocouple over setting
    //WAR15242 ATC system alarm: Temperature over setting !!
    //======MMATC_PowerSupply======//
    //WAR15218 ATC system alarm: <ALM01>GPUTJ board alarm/INT thermal couple error
    //WAR15219 ATC system alarm: <ALM02>PV over max temp error.
    //WAR15220 ATC system alarm: <ALM03>GPUTJ board sensor mode error (TJ/TC)
    //WAR15221 ATC system alarm: <ALM04>GPUTJ board communication error
    //WAR15238 ATC system alarm: ATC power supply off error

    //WAR15246 ATC system alarm: Undefined Serial number!!
    //WAR15247 ATC system alarm: Serial number does not match record!!
    //WAR15248 ATC system alarm: The serial number conflicts with other channels!!
    //WAR15319 ATC alarm: ATC temperature Index always same error
    //======MMATC_Chiller======//
    //WAR15200 ATC system alarm: Water tank leaking!!
    //WAR15201 ATC system alarm: Water tank over high limit!!
    //WAR15202 ATC system alarm: Water tank over low limit!!
    //WAR15203 ATC system alarm: Water flow error!!
    //WAR15204 ATC system alarm: [ARM] Water flow error!!
    //WAR15210 ATC system alarm: Chiller circulating fluid pressure too high!!
    //WAR15211 ATC system alarm: Chiller no power!!
    //WAR15212 ATC system alarm: Low level in tank!!
    //WAR15213 ATC system alarm: Over high temperature in chiller!!
    //WAR15214 ATC system alarm: Over Low temperature in chiller!!
    //WAR15215 ATC system alarm: Circulating fluid discharge pressure rise!!
    //WAR15216 ATC system alarm: Circulating fluid discharge pressure drop!!
    //WAR15217 ATC system alarm: <Chiller can't running!!>
    //WAR15234 ATC system alarm: Chiller has not started error
    //WAR15235 ATC system alarm: Chiller connect error
    //WAR15236 ATC system alarm: Water flow error
    //WAR15241 ATC system alarm: Leak Water
    //WAR15245 ATC system alarm: Chiller cannot communication!!
    //======MMATC_ATC======//
    //WAR15222 ATC system alarm: <ALM05>ATC IPC is idle
    //WAR15225 ATC system alarm: Read recipe file error!!
    //WAR15231 ATC system alarm: Unknown alarm code!!
    //WAR15232 ATC system alarm: Unknown Command code!!
    //WAR15233 ATC system alarm: Write Five same Temperature
    //WAR15237 ATC system alarm: Water temperature sensor over high
    //WAR15239 ATC system alarm: ATC Disable Site
    //WAR15240 ATC system alarm: Temperature below error

    if(iCode==15230)
        iATCPos=MMATC_TCPIP;
    else if(iCode==15223)
        iATCPos=MMATC_WaterValve;
    else if(iCode>=15227 && iCode<=15229)
        iATCPos=MMATC_RS232;
    else if(iCode==15209 || iCode==15224 || iCode==15226)
        iATCPos=MMATC_NI;
    else if((iCode>=15205 && iCode<=15208) || iCode==15242)
        iATCPos=MMATC_Head;
    else if(iCode>=15218 && iCode<=15221)
        iATCPos=MMATC_PowerSupply;
    else if(iCode==15238 || iCode==15319)
        iATCPos=MMATC_PowerSupply;
    else if(iCode>=15244 && iCode<=15246)                                       //Ifor 20170330 (wei) add ATC3.1 Serial number Alarm
        iATCPos=MMATC_PowerSupply;
    else if(iCode>=15200 && iCode<=15204)
        iATCPos=MMATC_Chiller;
    else if(iCode>=15210 && iCode<=15217)
        iATCPos=MMATC_Chiller;
    else if(iCode>=15234 && iCode<=15236)
        iATCPos=MMATC_Chiller;
    else if(iCode==15241 || iCode==15243)
        iATCPos=MMATC_Chiller;
    else
        iATCPos=MMATC_ATC;

    return iATCPos;
}
//---------------------------------------------------------------------------
void __fastcall TFormHS::ShowATCAlarmMessage()                                  //Ifor 20160831 add Show ATC Alarm Message
{
    AnsiString Msg, Msg1, sAlarmCode, sErrorPart;
    int iPos;

    Msg=ATC_InterfaceForm->GetAlarmMsg();                                       //Steven 20160706 : Add ATC alarm code
    Msg1=Msg;
    iPos=Msg.AnsiPos(")");
    if(iPos!=0)
    {
        sAlarmCode=Msg.SubString(2, iPos-2);
        Msg=Msg.SubString(iPos+1, Msg.Length());
        iPos=Msg.AnsiPos("]");
        if(iPos!=0)
        {
            sErrorPart=Msg.SubString(2, iPos-1);
        }
        else
        {
            sErrorPart="";
        }
        HSys.mapATCAlarmCodeIter=HSys.mapATCAlarmCode.find(sAlarmCode);
        if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                     //kevin 20201207 ATC
        {
            fLotInfo->pl_ATC_Online->Caption="ATC Off Line";                    //kevin 20201207 add display
            fLotInfo->pl_ATC_Online->Color=clRed;
        }

        if(Msg1.AnsiPos("WAR")>=1)                                              //JerryYang 20251020 : Dell說WAR開頭的一律alarm不停機
        {
            iUnLoaderCount=6;        
            ShowUnloaderTrayMessage(Msg1, "");
        }
        else if(HSys.mapATCAlarmCodeIter!=HSys.mapATCAlarmCode.end())
        {
            ShowErrorMessage(HSys.mapATCAlarmCode[sAlarmCode], 0, MMSystem, false, sErrorPart);
        }
        else
        {
            ShowMyMessage(Msg1, "No alarm code");
        }
    }

    if(Tri_Temp_Machine!=1)                                                     //Ztex 2024.03.25 Add HT-1032 TriTemp Function
        bSetTempChange=true;                                                    //Ifor 20160511 ATC Alarm 需重新送出命令避免發生異常
}
//---------------------------------------------------------------------------
void __fastcall TFormHS::SaveCloseOpenSiteEven(int iZ, int iRow, int iCol, bool bUse)
{                                                                               //Ifor 20161219 (Steven) add KYEC 開關Site Even Log 資料格式 開Site:O 關Site:X
    AnsiString asRow[4]={"A", "B", "C", "D"};
    AnsiString asCol[8]={"a", "b", "c", "d", "e", "f", "g", "h"};
    AnsiString asTmp="";
    bool bSave = false;

    if(IniConfig.bI35UseThirdSiteControlByEngineer==true)                       //Ifor 20161220 add 若有開啟I35第三組工程師用開關SITE功能才做判斷
    {
        if(bTestSiteUseEE[iZ][iRow][iCol]==true)
            bSave=true;
    }
    else
    {
        bSave=true;
    }

    if(bSave==true && fYieldMonitoring->bHasCloseSite==true)                    //Isaac 20171227 : 記錄low yield auto site off log
    {
        if(bUse)
            asTmp.sprintf("Auto site off : Site  %s%s Change X -> O", asRow[iRow], asCol[iCol]);
        else
            asTmp.sprintf("Auto site off : Site  %s%s Change O -> X", asRow[iRow], asCol[iCol]);
    }
    else if(bSave==true)
    {
        if(bUse)
            asTmp.sprintf("Site  %s%s Change X -> O", asRow[iRow], asCol[iCol]);
        else
            asTmp.sprintf("Site  %s%s Change O -> X", asRow[iRow], asCol[iCol]);
    }
    else
    {
        asTmp.sprintf("Site  %s%s is locked by the I35 function can not be switched", asRow[iRow], asCol[iCol]);
    }

    NewRecordProcess("MES2105", "Close Open Site", asTmp);
}
//---------------------------------------------------------------------------
void __fastcall TFormHS::AutoConnectATCNetwork()                                //Ifor 20170202 (Steven) add KYEC 網路磁碟機自動連線
{
    if(CUSTOMER_CODE==CC_KYEC_LEE &&
       Temperature.bATCActiveCooling==true)
    {
        if(ATC_SYSTEM==eATCHonPrecType || ATC_SYSTEM==eNewATCSystem)
        {
            AnsiString str;
            TStringList *TestList=new TStringList;                              //Ifor 20170210 (wei) New 物件移至判斷式內部，避免判斷式不成立時不會刪除

            str.sprintf("NET CONFIG SERVER /AUTODISCONNECT:-1");
            TestList->Add(str);
            str.sprintf("net use X: /delete");
            TestList->Add(str);
            if(ATC_SYSTEM==eNewATCSystem)
                str.sprintf("net use X: \\\\%s\\d\\ATC_Record\\EventLog\\ByLotID", HSys.asATCSYSTEMIP);
            else
                str.sprintf("net use X: \\\\%s\\d\\ATC_Recoder\\EventLog\\ByLotID", HSys.asATCSYSTEMIP);
            TestList->Add(str);

            TestList->SaveToFile("D:\\HT9045\\ATC_NetworkConnect.bat");         //Ifor 20170202 (Steven) 建立ATC連線檔案與寫入檔案
            ExecZipCommand("D:\\HT9045\\ATC_NetworkConnect.bat", " ");          //Ifor 20170202 (Steven) 避免出現Dos執行畫面
            MySleep(1200);
            TestList->Clear();
            delete TestList;
            if(DirectoryExists("X:\\")==false)
            {
                RecordProcess("Event Log Route X: Disk Fail!!");
            }
        }
    }
}
//---------------------------------------------------------------------------
//V3.27F.529 Ifor 20170603 (wei) add ESD Socket 連線
//---------------------------------------------------------------------------
void __fastcall TFormHS::ESDServerSocketClientConnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    int iConnectCount=(int)(ESDServerSocket->Socket->ActiveConnections);

    if(iConnectCount!=1)
        bRestESDConnect=true;
}
//---------------------------------------------------------------------------
//V3.27F.529 Ifor 20170603 (wei) add ESD Socket 異常處理
//---------------------------------------------------------------------------
void __fastcall TFormHS::ESDServerSocketClientError(TObject *Sender,
      TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
    try
    {
        ESDServerSocket->Close();
    }
    catch(...)
    {
        LogClientSocketExceptionError(Sender, "ESDServerSocket Error");
    }

    ESDServerSocket->Open();
}
//---------------------------------------------------------------------------
//V3.27F.529 Ifor 20170603 (wei) add ESD Socket 資料接收與處理
//---------------------------------------------------------------------------
void __fastcall TFormHS::ESDServerSocketClientRead(TObject *Sender,
      TCustomWinSocket *Socket)
{
    AnsiString ReceiveString="";
    TStringList *sList;
    int Lengh1=0;
    int Lengh2=0;
    int Lengh3=0;
    //接收字串
    ReceiveString=ESDServerSocket->Socket->Connections[0]->ReceiveText();
    if(ReceiveString.IsEmpty())
        return;

    if(ReceiveString.Pos("ESDReport")==1)
    {
        sList=new TStringList();
        sList->CommaText=ReceiveString;                                         //Ifor 20160316 add 利用接收資料欄位中的資料長度取出正確資料
        Lengh1=sList->Strings[0].Length()+sList->Strings[1].Length()+3;         //資料起始位置
        Lengh2=atoi(sList->Strings[1].c_str());                                 //接收資料的長度欄位
        Lengh3=ReceiveString.Length();                                          //接收到的資料長度
        if(Lengh3==(Lengh2+Lengh1-1))
        {
            ReceiveString = ReceiveString.SubString(Lengh1,Lengh2);
            sList->CommaText=ReceiveString;
            sESDLogData=ReceiveString;
            for(int i=0; i<6; i++)
            {
                if(sList->Count>7+(i*10))                                       //Steven 20190712 : 加上保護
                    fLotInfo->ESD_DataPtr[0+(i*3)]->Caption=sList->Strings[ 7+(i*10)].c_str();
                if(sList->Count>10+(i*10))
                    fLotInfo->ESD_DataPtr[1+(i*3)]->Caption=sList->Strings[10+(i*10)].c_str();
                if(sList->Count>13+(i*10))
                    fLotInfo->ESD_DataPtr[2+(i*3)]->Caption=sList->Strings[13+(i*10)].c_str();
            }
            fLotInfo->lbESDReportData->Caption=sESDLogData;
        }
        sList->Clear();
        delete sList;
    }
    else if(ReceiveString.Pos("DecayReport")==1)
    {
        sList=new TStringList();
        sList->CommaText=ReceiveString;                                         //Ifor 20160316 add 利用接收資料欄位中的資料長度取出正確資料
        Lengh1=sList->Strings[0].Length()+sList->Strings[1].Length()+3;         //資料起始位置
        Lengh2=atoi(sList->Strings[1].c_str());                                 //接收資料的長度欄位
        Lengh3=ReceiveString.Length();                                          //接收到的資料長度
        if(Lengh3==(Lengh2+Lengh1-1))
        {
            ReceiveString=ReceiveString.SubString(Lengh1,Lengh2);
            sList->CommaText=ReceiveString;

            for(int i=0; i<6; i++)
            {
                fLotInfo->ESD_DECAY_DATA_Ptr[0+(i*3)]->Caption=sList->Strings[1+(i*9)].c_str();
                fLotInfo->ESD_DECAY_DATA_Ptr[1+(i*3)]->Caption=sList->Strings[4+(i*9)].c_str();
                fLotInfo->ESD_DECAY_DATA_Ptr[2+(i*3)]->Caption=sList->Strings[7+(i*9)].c_str();

                fLotInfo->ESD_DECAY_DATA_Ptr[18+(i*3)]->Caption=sList->Strings[2+(i*9)].c_str();
                fLotInfo->ESD_DECAY_DATA_Ptr[19+(i*3)]->Caption=sList->Strings[5+(i*9)].c_str();
                fLotInfo->ESD_DECAY_DATA_Ptr[20+(i*3)]->Caption=sList->Strings[8+(i*9)].c_str();
            }
            fLotInfo->lbESDDecayReportData->Caption=ReceiveString;
        }
        sList->Clear();
        delete sList;
    }
}
//---------------------------------------------------------------------------
//V3.27K.538 Ifor 20170925 (Steven) add Confirm the independent password
//---------------------------------------------------------------------------
bool __fastcall TFormHS::CheckIndependentPassWord()
{
    bool bHasError=false;
    fPassword->edUserName->Text="";                                             // name
    fPassword->edPassword->Text="";                                             // password
    fPassword->ShowModal();
    if(fPassword->edUserName->Text.UpperCase()==EPuser.UpperCase() &&
       fPassword->edPassword->Text.UpperCase()==EPPass.UpperCase())
    {
        bHasError=false;
    }
    else
    {
        bHasError=true;
    }

    return bHasError;
}
//---------------------------------------------------------------------------
void __fastcall TFormHS::NMFTP1Failure(bool &Handled, TCmdType Trans_Type)
{
    sFTPErrMsg="";
    switch(Trans_Type)
    {
        case cmdChangeDir   : sFTPErrMsg="FTP Failure -- ChangeDir failed"      ; break;
        case cmdMakeDir     : sFTPErrMsg="FTP Failure -- MakeDir failed"        ; break;
        case cmdDelete      : sFTPErrMsg="FTP Failure -- Delete failed"         ; break;
        case cmdRemoveDir   : sFTPErrMsg="FTP Failure -- RemoveDir failed"      ; break;
        case cmdList        : sFTPErrMsg="FTP Failure -- List failed"           ; break;
        case cmdRename      : sFTPErrMsg="FTP Failure -- Rename failed"         ; break;
        case cmdUpRestore   : sFTPErrMsg="FTP Failure -- UploadRestore failed"  ; break;
        case cmdDownRestore : sFTPErrMsg="FTP Failure -- DownloadRestore failed"; break;
        case cmdDownload    : sFTPErrMsg="FTP Failure -- Download failed"       ; break;
        case cmdUpload      : sFTPErrMsg="FTP Failure -- Upload failed"         ; break;
        case cmdAppend      : sFTPErrMsg="FTP Failure -- UploadAppend failed"   ; break;
        case cmdReInit      : sFTPErrMsg="FTP Failure -- Reinitialize failed"   ; break;
        case cmdAllocate    : sFTPErrMsg="FTP Failure -- Allocate failed"       ; break;
        case cmdNList       : sFTPErrMsg="FTP Failure -- NList failed"          ; break;
        case cmdDoCommand   : sFTPErrMsg="FTP Failure -- DoCommand failed"      ; break;
        case cmdCurrentDir  : sFTPErrMsg="FTP Failure -- CurrentDir failed"     ; break;
    }
    MyDBIProcess("Process", sFTPErrMsg);
    bFTPError=true;
}
//---------------------------------------------------------------------------
void __fastcall TFormHS::NMFTP1UnSupportedFunction(TCmdType Trans_Type)
{
    sFTPErrMsg="";
    switch(Trans_Type)
    {
        case cmdChangeDir   : sFTPErrMsg="FTP UnSupported -- ChangeDir not supported by this server"       ; break;
        case cmdMakeDir     : sFTPErrMsg="FTP UnSupported -- MakeDir not supported by this server"         ; break;
        case cmdDelete      : sFTPErrMsg="FTP UnSupported -- Delete not supported by this server"          ; break;
        case cmdRemoveDir   : sFTPErrMsg="FTP UnSupported -- RemoveDir not supported by this server"       ; break;
        case cmdList        : sFTPErrMsg="FTP UnSupported -- List not supported by this server"            ; break;
        case cmdRename      : sFTPErrMsg="FTP UnSupported -- Rename not supported by this server"          ; break;
        case cmdUpRestore   : sFTPErrMsg="FTP UnSupported -- UploadRestore not supported by this server"   ; break;
        case cmdDownRestore : sFTPErrMsg="FTP UnSupported -- DownloadRestore not supported by this server" ; break;
        case cmdDownload    : sFTPErrMsg="FTP UnSupported -- Download not supported by this server"        ; break;
        case cmdUpload      : sFTPErrMsg="FTP UnSupported -- Upload not supported by this server"          ; break;
        case cmdAppend      : sFTPErrMsg="FTP UnSupported -- UploadAppend not supported by this server"    ; break;
        case cmdReInit      : sFTPErrMsg="FTP UnSupported -- Reinitialize not supported by this server"    ; break;
        case cmdAllocate    : sFTPErrMsg="FTP UnSupported -- Allocate not supported by this server"        ; break;
        case cmdNList       : sFTPErrMsg="FTP UnSupported -- NList not supported by this server"           ; break;
        case cmdDoCommand   : sFTPErrMsg="FTP UnSupported -- DoCommand not supported by this server"       ; break;
        case cmdCurrentDir  : sFTPErrMsg="FTP UnSupported -- CurrentDir not supported by this server"      ; break;
    }
    MyDBIProcess("Process", sFTPErrMsg);
    bFTPError=true;
}
//---------------------------------------------------------------------------
void __fastcall TFormHS::NMFTP1Error(TComponent *Sender, WORD Errno,
      AnsiString Errmsg)
{
    ShowMyMessage(Errmsg, "");
}
//---------------------------------------------------------------------------
//V3.28A.561 Ifor 20180809 (Steven) : add Check the temperature Offset limit
//---------------------------------------------------------------------------
//extern AnsiString asTempCtrl[tcTotalCount];
bool __fastcall TFormHS::CheckTempOffset(int Addr, double SetTemp, bool bSendAlarm)
{
    bool bOverSet=false;
    static AnsiString asErrorAddr="";
    static bool bHasErr=false;
    static double dAlarmTime=0;                                                 //Ifor 20180828 : add 記錄溫度異常時間
    double dTime=0;                                                             //Ifor 20180828 : add 記錄溫度異常時間

    if(Addr==tcHeatGun1 || Addr==tcHeatGun2)                                    //kevin 20190620
    {
        if(SetTemp>250)
        {
            bOverSet=true;
        }
    }
    else if(Addr==tcChamber)
    {
        if(SetTemp>165)
        {
            bOverSet=true;
        }
    }
    else if(Tri_Temp_Machine==1)                                                //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    {
        if(SetTemp>=Tri_Temperature_MaxDegree)
        {
            bOverSet=true;
        }
        if(SetTemp<0)
        {
            if(SetTemp<Tri_Temperature_MinDegree)
                bOverSet=true;
        }
    }
    else if(CUSTOMER_CODE==CC_KYEC_LEE ||                                       //Ifor 20220610 add:KYEC要求取消150溫度限制
            CUSTOMER_CODE==CC_PTI)
    {
        if(SetTemp>=175)
        {
            bOverSet=true;
        }
    }
    else if(Tri_Temp_Machine==1 &&
            MachineTypeChoice==Type_HT1032)                                     //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    {
        if(SetTemp>Tri_Temperature_MaxDegree)
        {
            bOverSet=true;
        }
    }
    else
    {
        if(SetTemp>TempFuseLimitType)                                           //Steven 20250701 : 整合機台溫度設定
        {
            bOverSet=true;
        }
    }

    if(bOverSet==true)
    {
        if(bHasErr==true)
        {
            if(asErrorAddr.Pos(asTempCtrl[Addr])==0)                            //Ifor 20180828 :add 避免溫度Alarm 位置重複顯示
                asErrorAddr=asErrorAddr+ ", "+ asTempCtrl[Addr];
        }
        else
        {
            bHasErr=true;
            asErrorAddr=asTempCtrl[Addr];
            dAlarmTime = MyTickCount();                                         //取得Alarm 時間
        }
    }

    if(asErrorAddr!="")                                                         //字串有資料時判斷是否超過Alarm時間
    {
        dTime = MyTickCount();                                                  //取得現在時間
        if(abs(dTime-dAlarmTime)>=60000)                                        //超過一分鐘無Alarm清除位置與旗標
        {
            bHasErr=false;                                                      //避免Alarm後條件成立無更新時間導致無位置顯示問題
            asErrorAddr="";                                                     //清除溫控器位置避免誤報Alarm
        }
    }

    if(bSendAlarm==true)
    {
        ShowErrorMessage("WAR15194", 0, MMSystem, false, asErrorAddr);
        asErrorAddr="";
        bHasErr=false;
    }
    return bOverSet;
}
//---------------------------------------------------------------------------
void __fastcall TFormHS::CloseWindowsKeyboard()                                 //Ifor 20190920 : add 關閉 Windows 小鍵盤
{
    WinExec("taskkill.exe /im \"OSK.exe\" /f", SW_HIDE);
}
//---------------------------------------------------------------------------
void __fastcall TFormHS::RecordContact_SLTLog_HS()                              //Ifor 20191218 : add KYEC 要求 同SLT輸出表格
{
    AnsiString sDirPath=AnsiString("D:\\SiteContact");
    AnsiString aFileName;
    AnsiString sRecord="";
    AnsiString str1;
    MyForceDirectories(sDirPath, "TFormHS::RecordContact_SLTLog_HS");
    aFileName.sprintf("Site Contact_%s_%s_%s.csv", IniConfig.SocketHandlerID, asATCEvenLotID, GetDateInfoByString()+GetOnlyTimeInfoByString());
    AnsiString sFullFileName=sDirPath+AnsiString("\\")+aFileName;

    if(FileExists(sFullFileName)==false)
    {
        str1="";
        sRecord =   sRecord+
                    "Lot NO"            +","+
                    "Start time"        +","+
                    "End time"          +",";

        for(int i=0; i<2; i++)
        {
            for(int p=0; p<16; p++)
            {
                str1.sprintf("Arm%d S%02d", i+1,p+1);
                if(i==1 && p==15)
                {
                    sRecord=sRecord+str1;
                }
                else
                {
                    sRecord=sRecord+str1+",";
                }
            }
        }
        WriteDataToFile(sFullFileName.c_str(), sRecord.c_str());
    }
    sRecord="";

    sRecord=sRecord+
            asATCEvenLotID      +","+
            asSLT_LotStartTime  +","+
            asSLT_LotEndTime    +",";

    for(int i=0; i<2; i++)
    {
        for(int p=0; p<16; p++)
        {
            if(i==1 && p==15)
            {
                sRecord=sRecord+iSLT_HeadContactCount[i][p];
            }
            else
            {
                sRecord=sRecord+iSLT_HeadContactCount[i][p]+",";
            }
        }
    }

    WriteDataToFile(sFullFileName.c_str(), sRecord.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TFormHS::RTMServerSocketClientConnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    int iConnectCount=(int)(RTMServerSocket->Socket->ActiveConnections);

    if(iConnectCount!=1)
        bResetRTMConnect=true;

    MyDBIProcess("RTM", "Server connected.");                                   //Steven 20200715 : 增加RTM log
}
//---------------------------------------------------------------------------
void __fastcall TFormHS::RTMServerSocketClientDisconnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    MyDBIProcess("RTM", "Server disconnected.");                                //Steven 20200715 : 增加RTM log
}
//---------------------------------------------------------------------------
void __fastcall TFormHS::RTMServerSocketClientError(TObject *Sender,
      TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
    MyDBIProcess("RTM", GetErrorMsg(Sender, ErrorCode));                        //Steven 20200715 : 增加RTM log
    try
    {
        RTMServerSocket->Close();
    }
    catch(...)
    {
        LogClientSocketExceptionError(Sender, "RTMServerSocket Error");
    }
    RTMServerSocket->Open();
}
//---------------------------------------------------------------------------
void __fastcall TFormHS::RTMServerSocketClientRead(TObject *Sender,
      TCustomWinSocket *Socket)
{
    AnsiString ReceiveString="";
    ReceiveString=Socket->ReceiveText();
    if(ReceiveString.IsEmpty())
        return;

    if(fMain->RTMCommand->Count==0)
        fMain->RTMCommand->Clear();
    fMain->RTMCommand->Add(ReceiveString);
    MyDBIProcess("RTM", AnsiString("Recv: ")+ReceiveString);                    //Steven 20200715 : 增加RTM log
    RTMServerSocket->Socket->Connections[0]->SendText("Status:00,Message:");
}
//---------------------------------------------------------------------------
void __fastcall TFormHS::TimerRTMMsgTimer(TObject *Sender)
{
    static bool bTimerRunning=false;

    if(InitialOK==false || bTimerRunning==true)
    {
        return;
    }

    bTimerRunning=true;
    if(CUSTOMER_CODE==CC_SCC && IniConfig.bN24_EnableRTM)
    {
        if(bHaltHandler)
        {
            SystemStart=false;
            RTMStr=StringReplace(RTMStr, "\",\"", "\",\r\n\"", TReplaceFlags()<<rfReplaceAll);
            ShowMyMessage(RTMStr);
        }
    }
    bTimerRunning=false;
}
//---------------------------------------------------------------------------
TQPF_Timer hRecordRunStateWaitTime;
void __fastcall TFormHS::RecordRunState()                                       //Ifor 20230420 add:ASEM要求新增Run Status Log Function
{
    static bool bFirstIn=true;
    static bool bIsRunning=false;
    AnsiString sBuffer;

    if(bIsRunning==true)
    {
        return;
    }
    bIsRunning=true;

    if(IniConfig.bA39RecordRunState==true)
    {
        if(bFirstIn==true)
        {
            bFirstIn=false;
            hRecordRunStateWaitTime.SetSecAndOn(IniConfig.iA39RecordTime);
        }
        else if(hRecordRunStateWaitTime.Off())
        {
            TStringList *sList;
            sList=new TStringList();

            sList->Add(IniConfig.SocketHandlerID);
            sList->Add(fMain->palMainStatus->Caption);

            if(fNote->fShow)
            {
                sList->Add(fNote->ShowMessageEdit1->Text);
            }
            else if(MyMessageBox->fShow)
            {
                sList->Add(MyMessageBox->lblMainMsg->Caption);
            }
            else
            {
                sList->Add("");
            }
            fMain->slRecordRunState->AddTextWithDateTime(sList->CommaText);
            sList->Clear();
            delete sList;
            bFirstIn=true;
        }
    }
    bIsRunning=false;
}
//---------------------------------------------------------------------------
bool TFormHS::CheckClockTrigger(int iClock)                                     //Sam 20230504 : 整合 Clock 觸發執行動作
{
    static bool b0000AM=false, b0100AM=false, b0800AM=false, b0800PM=false, bPerHours=false;
    static bool b30_0min=false,b30_1min=false, b90_0min=false, b90_1min=false, b120min=false;

    bool bResult=false;
    int iNowTime=0,iTime=0;
    int iInterVal=0;

    iNowTime=StrToInt(GetOnlyTimeInfoByString());
    if(iClock==0)                                                               //00:00AM
    {
        iTime=0;
        if(b0000AM)
        {
            if(iNowTime>iTime+3)
            {
                b0000AM=false;
                bResult=true;
            }
        }
        else
        {
            if(iNowTime>iTime && iNowTime<iTime+3)
                b0000AM=true;
        }
    }
    else if(iClock==1)                                                          //01:00AM
    {
        iTime=10000;
        if(b0100AM)
        {
            if(iNowTime>iTime+3)
            {
                b0100AM=false;
                bResult=true;
            }
        }
        else
        {
            if(iNowTime>iTime && iNowTime<iTime+3)
                b0100AM=true;
        }
    }
    else if(iClock==8)                                                          //08:00AM
    {
        iTime=80000;
        if(b0800AM)
        {
            if(iNowTime>iTime+3)
            {
                b0800AM=false;
                bResult=true;
            }
        }
        else
        {
            if(iNowTime>iTime && iNowTime<iTime+3)
                b0800AM=true;
        }
    }
    else if(iClock==20)                                                         //08:00PM
    {
        iTime=200000;
        if(b0800PM)
        {
            if(iNowTime>iTime+3)
            {
                b0800PM=false;
                bResult=true;
            }
        }
        else
        {
            if(iNowTime>iTime && iNowTime<iTime+3)
                b0800PM=true;
        }
    }
    else if(iClock==60)                                                         //整點
    {
        iTime=10000;
        if(bPerHours)
        {
            if(iNowTime%iTime>3)
            {
                bPerHours=false;
                bResult=true;
            }
        }
        else
        {
            if(iNowTime%iTime<3)
                bPerHours=true;
        }
    }
    else if(iClock==30)                                                         //30min     00:00 00:30 01:00 01:30 ......
    {
        iTime=10000;
        iInterVal=iNowTime/iTime;
        if(b30_0min)
        {
            if(iNowTime>(iInterVal*iTime)+3)
            {
                b30_0min=false;
                bResult=true;
            }
        }
        else
        {
            if((iNowTime>(iInterVal*iTime) &&
                iNowTime<(iInterVal*iTime)+3))
                b30_0min=true;
        }

        if(b30_1min)
        {
            if(iNowTime>(iInterVal*iTime)+3000+3)
            {
                b30_1min=false;
                bResult=true;
            }
        }
        else
        {
            if(iNowTime>(iInterVal*iTime)+3000 &&
               iNowTime<(iInterVal*iTime)+3000+3)
                b30_1min=true;
        }
    }
    else if(iClock==90)                                                         //90min     00:00 01:30 03:00 04:30 ..........
    {
        iTime=30000;
        iInterVal=iNowTime/iTime;
        if(b90_0min)
        {
            if(iNowTime>(iInterVal*iTime)+3)
            {
                b90_0min=false;
                bResult=true;
            }
        }
        else
        {
            if(iNowTime>(iInterVal*iTime) &&
               iNowTime<(iInterVal*iTime)+3)
                b90_0min=true;
        }
        if(b90_1min)
        {
            if(iNowTime>(iInterVal*iTime)+1300+3)
            {
                b90_1min=false;
                bResult=true;
            }
        }
        else
        {
            if(iNowTime>(iInterVal*iTime)+1300 &&
               iNowTime<(iInterVal*iTime)+1300+3)
                b90_1min=true;
        }
    }
    else if(iClock==120)                                                        //120min
    {
        iTime=20000;
        iInterVal=iNowTime/iTime;
        if(b120min)
        {
            if(iNowTime>(iInterVal*iTime)+3)
            {
                b120min=false;
                bResult=true;
            }
        }
        else
        {
            if(iNowTime>(iInterVal*iTime) &&
               iNowTime<(iInterVal*iTime)+3)
                b120min=true;
        }
    }
    return bResult;
}
//---------------------------------------------------------------------------
void __fastcall TFormHS::HandlerClientSocketConnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    fMain->iResetCurrent=1;                                                     //Ifor 20221007 add: 重新連線需要丟狀態給小軟體
}
//---------------------------------------------------------------------------
void __fastcall TFormHS::CheckEPRange()
{
    int iEPValue=0;
    double dKGRange=0;
//    AnsiString str;

    if(DeviceForm.dPress<=8)                                                    //500 g
    {
        dKGRange=0.5;
    }
    else if(DeviceForm.dPress<=60)
    {
        dKGRange=1;
    }
    else if(DeviceForm.dPress<=120)
    {
        dKGRange=2;
    }
    else
    {
        dKGRange=4;
    }

    switch(TestIF.iTestMode)
    {
        case SingleSite:
            dKGRange=dKGRange*1;
            break;
        case DualSite:
//        case DualSiteBS:
//            dKGRange=dKGRange*2;
//            break;
        case TriSite1X3:
        case QualSite1X4:
        case DualSite2x1:
        case QualSite2X2:
//        case QualSite2X2BS:
            dKGRange=dKGRange*4;
            break;
        default :
            dKGRange=dKGRange*8;
            break;
    }
    iEPValue=TransformFuntion(dKGRange);
    IniConfig.iD26EPEncoderRange=iEPValue;
//    str.sprintf("( %0.2f KG)", dKGRange);
//    fConfiguration->lblD26->Caption=str;
    fConfiguration->edD26->Text=dKGRange;
}
//---------------------------------------------------------------------------
void __fastcall TFormHS::ATC_FFCTrigger(bool bOnOff)                            //Ifor 20221003 add: ATC FFC Trigger Even
{
    if(CosFunction.bUseFFCTempOffset==true &&
       Temperature.bATC_FFCEnable==true)
    {
        if(iWhichArmDown==1)                                                    //ARM1 在下
        {
            ATC_InterfaceForm->ChannelFFCTrigger(iATC_Use_Heat_Count, 0, bOnOff);
        }
        else if(iWhichArmDown==2)                                               //ARM2 在下
        {
            ATC_InterfaceForm->ChannelFFCTrigger(iATC_Use_Heat_Count, 1, bOnOff);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormHS::HandlerClientSocketError(TObject *Sender,
      TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
    try
    {
        HandlerClientSocket->Close();
    }
    catch(...)
    {
        LogClientSocketExceptionError(Sender, "HandlerClientSocket Error");
    }
    HandlerClientSocket->Open();
}
//---------------------------------------------------------------------------
void __fastcall TFormHS::HandlerClientSocketRead(TObject *Sender,
      TCustomWinSocket *Socket)
{
    int iBufferLength=Socket->ReceiveLength();

    if(iBufferLength>0)
    {
        AnsiString StrBuff=Socket->ReceiveText();

        if(StrBuff.Pos("OFF")!=0)//=="STATUS:POWEROFF")
        {
            bPowersavingStart=false;
            fMain->iResetCurrent=3;                                             //Ifor 20240301 add:硬體進入省電模式
            FormHS->RecordPowerSaveEvenLog_HS("Power Saving Status : Power Off");
            GetTimeInfo();
            RunInfo.PowerSavingStartTime.sprintf("%04d/%02d/%02d %02d:%02d:%02d", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
            RunInfo.PowerSavingEndTime="";
            RunInfo.bSecsGemHeaterEnergySavingState=true;
            RunInfo.bSecsGemMotorEnergySavingState=true;
            if(IniConfig.bEnable_SECS_GEM==true)                                //Ifor 20240402 add:SECS/GEM Power Saving Mode
            {
                EventReport(SECS_EVENT.PowerSavingStart);
            }
        }
        else
        {
//            bPowersavingStart=true;                                           //Ifor 20240229 add:避免復歸時異常報警
            fMain->iResetCurrent=0;
            FormHS->RecordPowerSaveEvenLog_HS("Power Saving Status : Power On");
        }
    }
//    test=True;
}
//---------------------------------------------------------------------------
//Ifor 20220929 add Record Power Save Even Log
//==>
void __fastcall TFormHS::RecordPowerSaveEvenLog_HS(AnsiString aDataSRecordData, bool bIsATC)
{
    AnsiString sDirPath=asPowerSavePath;

    if(bIsATC==true)                                                            //Ifor 20240410 add:ATC Power Save Path
    {
        sDirPath=asATCPowerSavePath;
    }

    MyForceDirectories(sDirPath, "TFormHS::RecordPowerSaveEvenLog_HS");

    AnsiString sRecord="", aFileName="";
    GetTimeInfo();
    aFileName.sprintf("%s_Power_Saving_%04d.csv", IniConfig.SocketHandlerID, SystemYear);
    AnsiString sFullFileName=sDirPath+AnsiString("\\")+aFileName;

    AnsiString sTime;
    sTime.sprintf("%02d/%02d", SystemMonth, SystemDate);

    sRecord=sTime+" "+GetOnlyTimeInfoByString(":")+","+aDataSRecordData;

    WriteDataToFile(sFullFileName.c_str(), sRecord.c_str());
}
//<==
//Ifor 20220929 add Record Power Save Even Log
//---------------------------------------------------------------------------
void __fastcall TFormHS::RecordAutoMotive(AnsiString sDirPath, AnsiString aFileName, AnsiString sD1, eRecordType iType)   //Ifor 20221024 add: KLT Auto Motive Function
{
    AnsiString sHeaderName="";
    AnsiString sAutoPath=AnsiString("D:\\AutoMotive")+sDirPath;
    MyForceDirectories(sAutoPath, AnsiString("TFormHS::RecordAutoMotive_")+sDirPath);
    AnsiString sFullFileName=sAutoPath+AnsiString("\\")+aFileName;

    if(sD1!="")
    {
        if(FileExists(sFullFileName)==false)
        {
            GetTitleName(sHeaderName, iType);
            WriteDataToFile(sFullFileName.c_str(), sHeaderName.c_str());
        }
        WriteDataToFile(sFullFileName.c_str(), sD1.c_str());
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormHS::BackupGPIBLog()                                        //Ifor 20221024 add: KLT Auto Motive Function
{
    AnsiString sFullFileName;
    AnsiString str, str1, str2, str3, str4;
    str1.sprintf("%04d_%02d\\", SystemYear, SystemMonth);
    str3.sprintf("%04d-%02d-%02d*.*", SystemYear, SystemMonth, SystemDate);
    str4.sprintf("%04d-%02d-%02d*.*", SystemYear, SystemMonth, SystemDate-1);
    TStringList *TestList=new TStringList;

    AnsiString sAutoPath="D:\\AutoMotive\\GPIBLOG\\Log\\"+str1;
    MyForceDirectories(sAutoPath, "TFormHS::BackupGPIBLog");

    //取得GPIB來源資料名稱
    str2.sprintf("D:\\GPIBLOG\\Log%s%s", str1, str3);
    //複製來源檔案至目的位置
    str.sprintf("XCOPY /y/a/e/c/i/h/f/r %s %s", str2, sAutoPath);
    TestList->Add(str);

    //取得GPIB來源資料名稱
    str2.sprintf("D:\\GPIBLOG\\Log%s%s",str1,str4);
    //複製來源檔案至目的位置
    str.sprintf("XCOPY /y/a/e/c/i/h/f/r %s %s", str2, sAutoPath);
    TestList->Add(str);

    TestList->SaveToFile("D:\\HT9045\\EXE\\GPIBLOG.bat");                       //Ifor 20160408 (Steven) 建立檔案與寫入檔案
    ExecZipCommand("D:\\HT9045\\EXE\\GPIBLOG.bat", " ");                        //Ifor 20160408 (Steven) 避免複製檔案時出現Dos執行畫面
    TestList->Clear();
    delete TestList;
}
//---------------------------------------------------------------------------
void __fastcall TFormHS::GetTitleName(AnsiString &S, eRecordType iType)         //Ifor 20221024 add: KLT Auto Motive Function
{
    switch (iType)
    {
        case eReocrd_EP:
            S ="Ver:"+asHandlerVersion+"\n";                                    //kevin 20191212 add ver
            S=S+"Time,Index,Setting,Kpa,Kg,Alarm,Status";

            S=S+",Dual Setting,Dual Kpa,Dual Kg,Dual Alarm";                    //Ifor 20221228 add:新增Dual Ep Log

//            if(CosFunction.bHiSiliconFunction==true ||                        //Steven 20240925 : mark for 上傳Server資料增加Lot ID欄位
//               CUSTOMER_CODE==CC_KYEC_LEE)                                    //Ifor 20190905 :add 海思 V02.30 版  上傳Server資料增加Lot ID欄位
            {
                S=S+","+"Lot ID, OSAT Lot number ";
            }
            break;
        case eReocrd_Temp:
            if(CUSTOMER_CODE==CC_KYEC_LEE)
                S=GetTempUseName(1);
            else
                S=GetTempUseName(0);
            break;
        case eReocrd_ESD:
            S ="Ver:"+asHandlerVersion+"\n";                                    //kevin 20191212 add ver
            S = S+
                "Time"                  +","+
                "EnableAlarm"           +","+
                "AlarmContinuousTimes"  +","+
                "AlarmOccurrence"       +","+
                "ScanInterval"          +","+
                "Station1_Status"       +","+
                "Station1_1_Status"     +","+
                "Station1_1_Setting"    +","+
                "Station1_1_Value"      +","+
                "Station1_2_Status"     +","+
                "Station1_2_Setting"    +","+
                "Station1_2_Value"      +","+
                "Station1_3_Status"     +","+
                "Station1_3_Setting"    +","+
                "Station1_3_Value"      +","+
                "Station2_Status"       +","+
                "Station2_1_Status"     +","+
                "Station2_1_Setting"    +","+
                "Station2_1_Value"      +","+
                "Station2_2_Status"     +","+
                "Station2_2_Setting"    +","+
                "Station2_2_Value"      +","+
                "Station2_3_Status"     +","+
                "Station2_3_Setting"    +","+
                "Station2_3_Value"      +","+
                "Station3_Status"       +","+
                "Station3_1_Status"     +","+
                "Station3_1_Setting"    +","+
                "Station3_1_Value"      +","+
                "Station3_2_Status"     +","+
                "Station3_2_Setting"    +","+
                "Station3_2_Value"      +","+
                "Station3_3_Status"     +","+
                "Station3_3_Setting"    +","+
                "Station3_3_Value"      +","+
                "Station4_Status"       +","+
                "Station4_1_Status"     +","+
                "Station4_1_Setting"    +","+
                "Station4_1_Value"      +","+
                "Station4_2_Status"     +","+
                "Station4_2_Setting"    +","+
                "Station4_2_Value"      +","+
                "Station4_3_Status"     +","+
                "Station4_3_Setting"    +","+
                "Station4_3_Value"      +","+
                "Station5_Status"       +","+
                "Station5_1_Status"     +","+
                "Station5_1_Setting"    +","+
                "Station5_1_Value"      +","+
                "Station5_2_Status"     +","+
                "Station5_2_Setting"    +","+
                "Station5_2_Value"      +","+
                "Station5_3_Status"     +","+
                "Station5_3_Setting"    +","+
                "Station5_3_Value"      +","+
                "Station6_Status"       +","+
                "Station6_1_Status"     +","+
                "Station6_1_Setting"    +","+
                "Station6_1_Value"      +","+
                "Station6_2_Status"     +","+
                "Station6_2_Setting"    +","+
                "Station6_2_Value"      +","+
                "Station6_3_Status"     +","+
                "Station6_3_Setting"    +","+
                "Station6_3_Value"      ;

//            if(CosFunction.bHiSiliconFunction==true ||                        //Ifor 20190905 :add 海思 V02.30 版  上傳Server資料增加Lot ID欄位
//               CUSTOMER_CODE==CC_KYEC_LEE ||                                  //Steven 20240925 : mark for 上傳Server資料增加Lot ID欄位
//               CUSTOMER_CODE==CC_ASE_KaohSiung)                               //kevin 20191212 add
            {
                S=S+",Lot ID, OSAT Lot number " ;                               //kevin 20191210 add
            }
            break;
        case eReocrd_ATCEven:
            break;
        case eReocrd_Space:
            break;
        case eReocrd_TestArm:
            S="Time,TestArm,Torque(%),Auto Height(mm),Contact Offset(mm),Lot ID";
            break;
        case eReocrd_ChangEven:
            break;
    }
}
//---------------------------------------------------------------------------
bool __fastcall TFormHS::CheckATCTempWait()                                     //Ifor 20230608 add:KYEC 要求新增ATC 溫度等待功能
{
    bool bHasTempWait=false;
    static bool bNeedWait=false;
    int i=0;
    double dTempWorkBase;

    if(CUSTOMER_CODE==CC_KYEC_LEE &&
       bEnable_KLT_Function==false &&
       bCheckATCTemp==true &&
       Temperature.bATCActiveCooling==true)                                     //Ifor 20230608 add:
    {
        if(LastSet.iTemperature==Tempture_Hot ||
           LastSet.iTemperature==Tempture_AmbientHot)
            dTempWorkBase=Temperature.fWorkTemperBase;
        else
            dTempWorkBase=IniConfig.dATCAmbientTemperature;

        for(i=0; i<tcTotalCount; i++)
        {
            if(bUT150Install[i]==true)
            {
                if((i>=tcAa1 && i<=tcBd1) ||
                   (i>=tcAe1 && i<=tcBh1) ||
                   (i>=tcAa2 && i<=tcBd2) ||
                   (i>=tcAe2 && i<=tcBh2))
                {
                    if(IniConfig.bL07UseSingleTenmpertureLimit)
                    {
                        if(UN150Read[i]>(dTempWorkBase+IniConfig.dSingleTempLimit[i]))
                        {
                            bHasTempWait=true;
                        }
                        else if(UN150Read[i]<(dTempWorkBase-IniConfig.dSingleTempLimit[i]))
                        {
                            bHasTempWait=true;
                        }
                    }
                    else
                    {
                        if(UN150Read[i]>(dTempWorkBase+IniConfig.iATCTemperatureRange))
                        {
                            bHasTempWait=true;
                        }
                        else if(UN150Read[i]<(dTempWorkBase-IniConfig.iATCTemperatureRange))
                        {
                            bHasTempWait=true;
                        }
                    }
                }
            }
        }

        if(bNeedWait==false)                                                    //Ifor 20230615 fix:避免倒數等待中溫度剛好到達設定取消等待
        {
            if(bHasTempWait==true)
            {
                bNeedWait=true;
                iATCTempWaitTimer=180;
                iATCTempWaitTimer_Start = MyTickCount();
                RecordProcess("ATC Temp Wait Start!");
            }
            else
            {
                bCheckATCTemp=false;
            }
        }

        if(iATCTempWaitTimer<=0 && bNeedWait==true)
        {
            bCheckATCTemp=false;
            bNeedWait=false;
            RecordProcess("ATC Temp Wait Finish!");
        }
    }
    else
    {
        bCheckATCTemp=false;
        bNeedWait=false;
    }

    return bNeedWait;
}
//---------------------------------------------------------------------------
void __fastcall TFormHS::TESTTEMPSETTING(int iTemp)                             //Ifor 20240117 add:SECSGEM測試中修改ATC溫度
{
    double dbSetATCTemp=0;

    if(LastSet.iTemperature==Tempture_Hot ||
       LastSet.iTemperature==Tempture_AmbientHot)
        dbSetATCTemp=Temperature.fWorkTemperBase;
    else
        dbSetATCTemp=IniConfig.dATCAmbientTemperature;

    bChangeTest_TempOffset=iTemp-dbSetATCTemp;

    if(bChangeTest_TempOffset!=0)
    {
        bChangeTest_TempAlarm=true;
        if(iWhichArmDown==1)
        {
            if(iATC_Use_Heat_Count<=4)
            {
                ATC_InterfaceForm->SetSingleTemp(0,dbSetATCTemp+bChangeTest_TempOffset);
                ATC_InterfaceForm->SetSingleTemp(1,dbSetATCTemp+bChangeTest_TempOffset);
                ATC_InterfaceForm->SetSingleTemp(2,dbSetATCTemp);
                ATC_InterfaceForm->SetSingleTemp(3,dbSetATCTemp);
            }
            else if(iATC_Use_Heat_Count==8)
            {
                ATC_InterfaceForm->SetSingleTemp(0,dbSetATCTemp+bChangeTest_TempOffset);
                ATC_InterfaceForm->SetSingleTemp(1,dbSetATCTemp+bChangeTest_TempOffset);
                ATC_InterfaceForm->SetSingleTemp(2,dbSetATCTemp+bChangeTest_TempOffset);
                ATC_InterfaceForm->SetSingleTemp(3,dbSetATCTemp+bChangeTest_TempOffset);

                ATC_InterfaceForm->SetSingleTemp(4,dbSetATCTemp);
                ATC_InterfaceForm->SetSingleTemp(5,dbSetATCTemp);
                ATC_InterfaceForm->SetSingleTemp(6,dbSetATCTemp);
                ATC_InterfaceForm->SetSingleTemp(7,dbSetATCTemp);
            }
        }
        else if(iWhichArmDown==2)
        {
            if(iATC_Use_Heat_Count<=4)
            {
                ATC_InterfaceForm->SetSingleTemp(0,dbSetATCTemp);
                ATC_InterfaceForm->SetSingleTemp(1,dbSetATCTemp);
                ATC_InterfaceForm->SetSingleTemp(2,dbSetATCTemp+bChangeTest_TempOffset);
                ATC_InterfaceForm->SetSingleTemp(3,dbSetATCTemp+bChangeTest_TempOffset);
            }
            else if(iATC_Use_Heat_Count==8)
            {
                ATC_InterfaceForm->SetSingleTemp(0,dbSetATCTemp);
                ATC_InterfaceForm->SetSingleTemp(1,dbSetATCTemp);
                ATC_InterfaceForm->SetSingleTemp(2,dbSetATCTemp);
                ATC_InterfaceForm->SetSingleTemp(3,dbSetATCTemp);

                ATC_InterfaceForm->SetSingleTemp(4,dbSetATCTemp+bChangeTest_TempOffset);
                ATC_InterfaceForm->SetSingleTemp(5,dbSetATCTemp+bChangeTest_TempOffset);
                ATC_InterfaceForm->SetSingleTemp(6,dbSetATCTemp+bChangeTest_TempOffset);
                ATC_InterfaceForm->SetSingleTemp(7,dbSetATCTemp+bChangeTest_TempOffset);

            }
        }
    }
    else
    {
        ATC_InterfaceForm->SetAllTemp(dbSetATCTemp);
        bChangeTest_TempAlarm=false;
    }
    fLotInfo->SetATCOffset(true);                                               //Ifor 20241118 : 測試中變溫
}
//---------------------------------------------------------------------------
TQPF_Timer KeyProCheckTime;                                                     //Ifor 20180227 (Steven) add SECS GEM Confirm the Employee ID
bool __fastcall TFormHS::CheckKeyPro(bool bStart)                               //Ifor 20230721 : add Check Keypro
{
    int iLevel=0;
    bool bNeedCheck=false;
    static bool bCheckKeyPro=false;                                             //Ifor 20230721 add:SECS GEM 使用KeyPro

    if(CosFunction.bSECSGEM_UseKeyPro &&
       IniConfig.bEnable_SECS_GEM==true)
    {
        if(SystemStart)
        {
            if(bCheckKeyPro==false)
            {
                KeyProCheckTime.SetSecAndOn(60);
                bCheckKeyPro=true;
            }
            else
            {
                if(KeyProCheckTime.Off())
                {
                  bNeedCheck=true;
                }
            }
        }
        else
        {
            if(bStart==true)
                bNeedCheck=true;
            else
                bNeedCheck=false;
        }

        if(bNeedCheck==true)
        {
            bCheckKeyPro=false;
            iLevel=KEYPRO_GET_LEVEL(3);
            if(iLevel!=1)
            {
                ShowMyMessage("For enabling SECS GEM function, please insert KeyPro.","開啟SECS GEM 功能,請插入KeyPro.");
                return true;
            }
        }
    }
    else
    {
        bCheckKeyPro=false;
    }

    return false;
}
//---------------------------------------------------------------------------
void __fastcall TFormHS::RecordParameter_TFAMDLog()                             //Ifor 20191218 : add KYEC 要求 同SLT輸出表格
{
    //確認資料夾是否存在
    AnsiString sDirPath=AnsiString("D:\\Recipelocal");
    AnsiString aFileName;
    AnsiString sRecord="";
    AnsiString str1;
//    int Data;
    AnsiString SBinData[eTrayCount];

    if(DirectoryExists(sDirPath)==false)
    {
        CreateDir(sDirPath);
    }

    aFileName.sprintf("%s_%s_recipelocal_%s_%s.csv", IniConfig.RMSTesterID, IniConfig.SocketHandlerID, GetDateInfoByString(), GetOnlyTimeInfoByString());
    sDirPath=sDirPath+"\\";

    AnsiString sFullFileName=sDirPath;

    sFullFileName=sFullFileName+aFileName;

    if(FileExists(sFullFileName)==false)
    {
        double dbSetATCTemp=0;

        if(LastSet.iTemperature==Tempture_Hot || LastSet.iTemperature==Tempture_AmbientHot)
            dbSetATCTemp=Temperature.fWorkTemperBase;
        else
            dbSetATCTemp=IniConfig.dATCAmbientTemperature;

//        BinSelect[iTestRunMode].IfError=5;
//        BinSelect[iTestRunMode].bFailure[0];
//        BinSelect[iTestRunMode].iCategData

//        for(int i=0; i<iTestBinCount; i++)
//        {
//            Data=BinSelect[iTestRunMode].iCategData[i];
//            if(Data<=0)
//              continue;
//
//            if(Data==BinSelect[iTestRunMode].IfError+1 && SBinData[Data]=="")
//            {
//                SBinData[Data]="Error,";
//            }
//            SBinData[Data]=SBinData[Data] + AnsiString(i) +",";
//        }

        for(int i=0; i<eTrayCount; i++)
        {
            if(BinSelect[iTestRunMode].iStackDefFailCate[i]==0)
            {
                SBinData[i+1]=SBinData[i+1]+"Pass";
            }
            else
            {
                SBinData[i+1]=SBinData[i+1]+"Reject";
            }
        }

        sRecord = "Tester ID,"                      +IniConfig.RMSTesterID                      +"\r\n";    //"Tester ID"
        sRecord = sRecord+"Handler ID,"             +IniConfig.SocketHandlerID                  +"\r\n";    //"Handler ID"
        sRecord = sRecord+"Setup File,"             +fMain->cbSetupFileName->Text               +"\r\n";    //"Setup File"
        sRecord = sRecord+"Force per pin,"          +AnsiString(DeviceForm_File.ForcePerPinN)    +"gf\r\n";  //"Force per pin"
        sRecord = sRecord+"The no.of pins(balls),"  +AnsiString(DeviceForm_File.iPinCT)         +"\r\n";    //"The no.of pins(balls)"
        sRecord = sRecord+"Die Force,"              +AnsiString(DeviceForm_File.DoubleForce)    +"Kg\r\n";  //"Die Force"
        sRecord = sRecord+"Height Temp,"            +AnsiString(dbSetATCTemp)                   +"\r\n";    //"Height Temp"
        sRecord = sRecord+"Auto 1,"                 +SBinData[1]                      +"\r\n";    //"Auto 1"
        sRecord = sRecord+"Auto 2,"                 +SBinData[2]                      +"\r\n";    //"Auto 2"
        sRecord = sRecord+"Auto 3,"                 +SBinData[3]                      +"\r\n";    //"Auto 3"
        sRecord = sRecord+"Fix 1,"                  +SBinData[4]                      +"\r\n";    //"Fix 1"
        sRecord = sRecord+"Fix 2,"                  +SBinData[5]                      +"\r\n";    //"Fix 2"
        sRecord = sRecord+"Fix 3,"                  +SBinData[6];                                 //"Fix 3"

        WriteDataToFile(sFullFileName.c_str(), sRecord.c_str());
    }
}
//---------------------------------------------------------------------------
AnsiString TFormHS::GetGPIBLogFilePath()
{
    AnsiString asPath="";
    asPath.sprintf("%s\\%04d_%02d", "D:\\GPIBLOG\\Log", SystemYear, SystemMonth);
    return asPath;
}
//---------------------------------------------------------------------------
std::vector<AnsiString> TFormHS::GetGPIBLogFileName()
{
    AnsiString sFileNameFilter=AnsiString().sprintf("%04d-%02d-%02d", SystemYear, SystemMonth ,SystemDate);
    AnsiString folderPath=GetGPIBLogFilePath();
    int iErr=0;
    std::vector<AnsiString> fileNames =FileInfo().GetAllFileNamesInFolder(folderPath,iErr);
    std::vector<AnsiString> retfileNames;
    AnsiString sGetName="";
    for(unsigned int i=0;i<fileNames.size();i++)
    {
        sGetName=fileNames[i];
        if(sGetName.Pos(sFileNameFilter)>0)
        {
            retfileNames.push_back(sGetName);
        }
    }
    return retfileNames;
}
//---------------------------------------------------------------------------
