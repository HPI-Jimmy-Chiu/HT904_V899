#include "MachineDefine.h"
#pragma hdrstop

#include "TrayStepMotor.h"

#include "cMyDB.h"
#include "mymessbox.h"
#include "cmydef.h"
#include "uPadInterface.h"                                                      //KenHsieh 20211222 : Pad與步進馬達為同一Comport
#include "cTrayMapping.h"
//#include "cmydef.h"
//#include "database.h"
#include "main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SPComm"
#pragma resource "*.dfm"
TdmTrayMotor *dmTrayMotor;

#define TrackCount 8                                                            //軌道的數量

double dStepMotorDelay=1;                                                       //讀取的Delay間隔
double dVibrateMotorDelay=3;                                                    //讀取的Delay間隔
AnsiString sStepMotorAxis[MAX_TRACK]={"Load", "Auto1", "Auto2", "Auto3", "Empty", "Color", "Auto4", "Auto5", "Auto6"};    //指定軌道的名稱
AnsiString sVibrationMotorAxis[2]={"VibrationMotor1", "VibrationMotor2"};       //指定軌道的名稱
int iSetpMotorVer[MAX_TRACK];                                                   //軌道驅動器的韌體版本

extern int  AUTO_EMPTY_COLOR;
extern int  CUSTOMER_CODE;
extern bool InitialOK;
extern int  LoaderUnload_StepMotor;
extern Word SystemHour, SystemMin, SystemSec, SystemMSec;
//extern Word SystemYear, SystemMonth, SystemDate;
//extern Word SystemYearYesterday, SystemMonthYesterday, SystemDateYesterday;
//---------------------------------------------------------------------------
__fastcall TdmTrayMotor::TdmTrayMotor(TComponent* Owner)
    : TDataModule(Owner)
{
    bNeedSetStepMotorSpeed=true;
    iSetStepMotorTask=1;
    iVibrationMotorTask=1;
    if(iControlPanelMode)                                                       //KenHsieh 20211222 : Pad與步進馬達為同一Comport
        dmTrayMotor->tmrTrayStepMotor->Enabled=false;
    else
        dmTrayMotor->tmrTrayStepMotor->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TdmTrayMotor::DataModuleDestroy(TObject *Sender)
{
    dmTrayMotor->comTrayStepMotor->StopComm();
    dmTrayMotor->tmrTrayStepMotor->Enabled=false;
    mmoTrayStepMotor=NULL;
    LogSoftwareOffTime("TdmTrayMotor, FormDestroy");                            //Steven 20210526 : 紀錄軟體執行時間
}
//---------------------------------------------------------------------------
void TdmTrayMotor::RS232Init(AnsiString ComPort)
{
    #ifndef SOFT_SIMULTE
    AnsiString Str;

    if(iControlPanelMode && LoaderUnload_StepMotor)                             //KenHsieh 20211222 : Pad與步進馬達為同一Comport
        Str="Tray Step Motor & Control Panel : ";
    else if(LoaderUnload_StepMotor)
        Str="Tray Step Motor : ";
    else if(iControlPanelMode)
        Str="Control Panel : ";

    bRunTimer=false;                                                            //KenHsieh 20220107 : 無通訊面板時使用
    comTrayStepMotor->BaudRate=115200;
    comTrayStepMotor->CommName="\\\\.\\"+ComPort;
    comTrayStepMotor->ReadIntervalTimeout=1;                                    //Sam 20220617 : 加快控制面板通訊
    try
    {
        comTrayStepMotor->StartComm();                                          //僅能啟動一次
        if(iControlPanelMode)                                                   //KenHsieh 20211222 : Pad與步進馬達為同一Comport
        {
            fPadInterface->bRs232Ok=true;
            MyPad232Thread->Resume();
        }
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TCOM2::RS232Init");
        ShowMyMessage(Str+ComPort+" port error", "");                           //KenHsieh 20211222 : Pad與步進馬達為同一Comport
    }
    #endif
}
//---------------------------------------------------------------------------
void TdmTrayMotor::StartSetSpeed()
{
    if(LoaderUnload_StepMotor)
    {
        bNeedSetStepMotorSpeed=true;
    }

    if(USE_VibrationCommunication)                                              //JerryYang 20230814 : add震動馬達通訊調速版本
    {
        bNeedSetVibrateMotSpeed=true;
    }
}
//---------------------------------------------------------------------------
void __fastcall TdmTrayMotor::tmrTrayStepMotorTimer(TObject *Sender)
{
    if(bRunTimer)                                                               //KenHsieh 20220107 : 無通訊面板時使用
        return;
    DoTrayStepMotor();                                                          //KenHsieh 20211222 : Pad與步進馬達為同一Comport
}
//---------------------------------------------------------------------------
void TdmTrayMotor::DoTrayStepMotor()                                            //KenHsieh 20211222 : Pad與步進馬達為同一Comport
{
    #ifdef SOFT_SIMULTE
        return;
    #else
    static int iAddr=0;
    int &Task=iSetStepMotorTask;

    static int iErrCount=0;                                                     //JerryYang 20240111 : 新增retry

    if(InitialOK==false ||
       (LoaderUnload_StepMotor==0 && USE_VibrationCommunication==0))            //JerryYang 20230814 : add震動馬達通訊調速版本
        return;

    if(iControlPanelMode==0)                                                    //KenHsieh 20220107 : 無通訊面板時使用
        bRunTimer=true;

    if(bNeedSetStepMotorSpeed && bNeedSetVibrateMotSpeed==false)                //JerryYang 20230814 : add震動馬達通訊調速版本
    {
        iAddr=0;
        Task=1;
        bReceiveData=false;
        bNeedSetStepMotorSpeed=false;
    }

    switch(Task)
    {
        case 1:                                                                 //Ifor 20200620 add: 讀取Tray Step Motor版本
            if(LoaderUnload_StepMotor==0 && USE_VibrationCommunication==1)      //JerryYang 20230814 : add震動馬達通訊調速版本
            {
                Task=6100;
                break;
            }

            if(iAddr>=MAX_TRACK)                                                //Ifor 20220916 : 加上保護避免超出陣列大小      //JerryYang 20230915 : add
                iAddr=0;
            CMD_CheckStepMotorVer(iAddr);
            Task=100;
        case 100:
            if(bReceiveData)
            {
                Task=1000;
            }
            else if(MachineTypeChoice==Type_HT7080)                             //Nickliu 20230315 add HT70XX Loader/Unloader NO Need Get Module Ver
            {
                Task=1000;
            }
            else
            {
                if(SetStepMotorDelay.Off())
                {
                    Task=6000;
                    //例外處理
                }
            }
            break;
        case 1000:
            CMD_SetStepMotorSpeed(iAddr, iStepMotorSpeed[iAddr]);
            Task=1100;
            break;
        case 1100:
            if(bReceiveData)
            {
                Task=2000;
            }
            else
            {
                if(SetStepMotorDelay.Off())
                {
                    Task=2000;
                    //例外處理
                }
            }
            break;
        case 2000:
            CMD_SetStepMotorSecondSpeedAcc(iAddr);
            Task=2100;
            break;
        case 2100:
            if(bReceiveData)
            {
                Task=3000;
            }
            else
            {
                if(SetStepMotorDelay.Off())
                {
                    Task=3000;
                    //例外處理
                }
            }
            break;
        case 3000:
            CMD_SetStepMotorSecondSpeedDec(iAddr);
            Task=3100;
        case 3100:
            if(bReceiveData)
            {
                Task=4000;
            }
            else
            {
                if(SetStepMotorDelay.Off())
                {
                    Task=4000;
                    //例外處理
                }
                break;
            }
        case 4000:
            if(CUSTOMER_CODE==CC_ASE_KaohSiung)
            {
                CMD_SetStepMotorSecondSpeedCW(iAddr, iStepMotorSpeed[9]);       //SECOND SPEED   kevin 20201211
            }
            else
            {
                CMD_SetStepMotorSecondSpeedCW(iAddr, iStepMotorSpeed[iAddr]);
            }
            Task=4100;
        case 4100:
            if(bReceiveData)
            {
                Task=5000;
            }
            else
            {
                if(SetStepMotorDelay.Off())
                {
                    Task=5000;
                    //例外處理
                }
                break;
            }
        case 5000:
            if(CUSTOMER_CODE==CC_ASE_KaohSiung)
            {
                CMD_SetStepMotorSecondSpeedCCW(iAddr, iStepMotorSpeed[9]);      //SECOND SPEED   kevin 20201211
            }
            else
            {
                CMD_SetStepMotorSecondSpeedCCW(iAddr, iStepMotorSpeed[iAddr]);
            }
            Task=5100;
        case 5100:
            if(bReceiveData)
            {
                Task=6000;
            }
            else
            {
                if(SetStepMotorDelay.Off())
                {
                    Task=6000;
                    //例外處理
                }
                break;
            }
        case 6000:
            iAddr++;
            if(AUTO_EMPTY_COLOR!=0)
            {
                if(iAddr<MAX_TRACK)
                    Task=1;
                else
                    Task=6100;
            }
            else
            {
                if(iAddr<4)
                    Task=1;
                else
                    Task=6100;
            }

            if(Task==6100)                                                      //Ifor 20220916 : 加上保護避免超出陣列大小
                iAddr=0;
            break;
        case 6100:                                                              //JerryYang 20230814 : add震動馬達通訊調速版本
            if(bNeedSetVibrateMotSpeed)
            {
                bNeedSetVibrateMotSpeed=false;
                if(USE_VibrationCommunication==false)
                {
                    break;
                }
                iAddr=0;
                iErrCount=0;                                                    //JerryYang 20240111 : 新增retry
                Task=6200;
            }
            else if(bNeedSetStepMotorSpeed)
            {
                iAddr=0;
                Task=1;
                bReceiveData=false;
                bNeedSetStepMotorSpeed=false;
            }
            break;
        case 6200:                                                              //Ifor 20200620 add: 讀取Tray Step Motor版本
            if(iAddr>=VibrationMotorCount)                                      //Ifor 20220916 : 加上保護避免超出陣列大小
            {
                iAddr=0;
            }
            bReceiveVibrateMotData=false;
            CMD_CheckVibrationMotVer(iAddr);
            Task=6300;
            break;
        case 6300:
            if(bReceiveVibrateMotData)
            {
                Task=6400;
                iErrCount=0;
            }
            else
            {
                if(SetVibrateMotorDelay.Off())
                {
                    iErrCount++;
                    if(iErrCount>=3)                                            //JerryYang 20240111 : 新增retry
                    {
                        iErrCount=0;
                        ShowMyMessage("震動馬達通訊timeout");
                        fPadInterface->ResetComm();
                        Task=6000;                                              //例外處理
                    }
                    else
                    {
                        Task=6200;                                              //retry
                    }
                }
            }
            break;
        case 6400:
            CMD_SetVibrationMotorSpeed(iAddr);
            Task=6500;
            break;
        case 6500:
            if(bReceiveVibrateMotData)
            {
                Task=6600;
            }
            else
            {
                if(SetVibrateMotorDelay.Off())
                {
                    iErrCount++;
                    if(iErrCount>=3)                                            //JerryYang 20240111 : 新增retry
                    {
                        iErrCount=0;
                        ShowMyMessage("震動馬達通訊timeout");
                        fPadInterface->ResetComm();
                        Task=6600;                                              //例外處理
                    }
                    else
                    {
                        Task=6400;                                              //retry
                    }
                }
            }
            break;
        case 6600:
            iAddr++;

            if(iAddr<VibrationMotorCount)                                       //JerryYang 20230823 : only 1 board.
                Task=6200;
            else
                Task=6700;

            if(Task==6700)                                                      //Ifor 20220916 : 加上保護避免超出陣列大小
                iAddr=0;
            break;
        case 6700:
            if(bNeedSetStepMotorSpeed)
            {
                iAddr=0;
                Task=1;
                bReceiveData=false;
                bNeedSetStepMotorSpeed=false;
            }
            else if(bNeedSetVibrateMotSpeed)
            {
                iAddr=0;
                Task=6200;
                bReceiveVibrateMotData=false;
                bNeedSetVibrateMotSpeed=false;
            }
            break;
    }
    if(iControlPanelMode==0)                                                    //KenHsieh 20220107 : 無通訊面板時使用
        bRunTimer=false;
    #endif
}
//---------------------------------------------------------------------------
void __fastcall TdmTrayMotor::comTrayStepMotorReceiveData(TObject *Sender,
      Pointer Buffer, WORD BufferLength)
{
    AnsiString sReciveData = (char *)Buffer;
    AnsiString sAxis, sFunc, sSubSunc, Msg;
    AnsiString sUnit;                                                           //KenHsieh 20211222 : Pad與步進馬達為同一Comport
    int iAxis;

    sUnit=sReciveData.SubString(1, 3);                                          //KenHsieh 20211222 : Pad與步進馬達為同一Comport

    if(sUnit=="t07")                                                            //KenHsieh 20211222 : Pad與步進馬達為同一Comport
    {
        sAxis=sReciveData.SubString(4, 1);
        sFunc=sReciveData.SubString(6, 2);
        sSubSunc=sReciveData.SubString(11, 3);
        iAxis=atoi(sAxis.c_str());                                              //Steven 20200706 : 確保數字沒有超出陣列

        if(iAxis>=0 && iAxis<TrackCount)
        {
            if(sFunc=="07")
            {
                Msg.sprintf("RECV, %s, Speed", sStepMotorAxis[atoi(sAxis.c_str())]);
            }
            else if(sFunc=="15")
            {
                if(sSubSunc=="200")
                    Msg.sprintf("RECV, %s, ACC", sStepMotorAxis[atoi(sAxis.c_str())]);
                else if(sSubSunc=="400")
                    Msg.sprintf("RECV, %s, DEC", sStepMotorAxis[atoi(sAxis.c_str())]);
                else
                    Msg.sprintf("RECV, %s, ", sStepMotorAxis[atoi(sAxis.c_str())]);
            }
            else if(sFunc=="17")
            {
                if(sSubSunc=="602")
                    Msg.sprintf("RECV, %s, CW_Speed", sStepMotorAxis[atoi(sAxis.c_str())]);
                else if(sSubSunc=="803")
                    Msg.sprintf("RECV, %s, CCW_Speed", sStepMotorAxis[atoi(sAxis.c_str())]);
                else
                    Msg.sprintf("RECV, %s, ", sStepMotorAxis[atoi(sAxis.c_str())]);
            }
            else if(sFunc=="01")                                                //Ifor 20200620 add: 讀取Tray Step Motor版本
            {
                Msg.sprintf("RECV, %s, ", sStepMotorAxis[atoi(sAxis.c_str())]);
                int iAxisAdd=atoi(sAxis.c_str());
                AnsiString asVersion="";
                AnsiString asData="";
                int iNumber=0;
                int iHight=0;
                int iLow=0;

                for(int i=0; i<7; i++)
                {
                    iNumber=atoi(sReciveData.SubString(8+i*2, 2).c_str());
                    iHight=iNumber/10;
                    iLow=iNumber%10;
                    asData=MyDeCodeASCII((iHight*16)+iLow);
                    asVersion=asVersion+asData;
                }

                switch(iAxisAdd)
                {
                    case 0:
                        fSpeed->Lab_LoaderVer->Caption=asVersion;
                        break;
                    case 1:
                        fSpeed->Lab_Auto1Ver->Caption=asVersion;
                        break;
                    case 2:
                        fSpeed->Lab_Auto2Ver->Caption=asVersion;
                        break;
                    case 3:
                        fSpeed->Lab_Auto3Ver->Caption=asVersion;
                        break;
                    case 4:
                        fSpeed->Lab_EmptyVer->Caption=asVersion;
                        break;
                    case 5:
                        fSpeed->Lab_ColorVer->Caption=asVersion;
                        break;
                    case 6:
                        fSpeed->Lab_Auto4Ver->Caption=asVersion;
                        break;
                    case 7:
                        fSpeed->Lab_Auto5Ver->Caption=asVersion;
                        break;
                    case 8:
                        fSpeed->Lab_Auto6Ver->Caption=asVersion;
                        break;
                }
                iSetpMotorVer[iAxisAdd]=atoi(asVersion.SubString(asVersion.AnsiPos("I")+1, 3).c_str());
            }
            else
            {
                Msg.sprintf("RECV, %s, ", sStepMotorAxis[atoi(sAxis.c_str())]);
            }
        }
        else
        {
            Msg.sprintf("RECV, Data format error!");
        }

        AddTrayStepMotorLog(Msg, sReciveData);
        bReceiveData=true;
    }
    else if(sUnit=="t05")                                                       //KenHsieh 20211222 : Pad與步進馬達為同一Comport
    {
        fPadInterface->CommReceiveLength->Add(BufferLength);
        fPadInterface->CommReceiveList->Add(sReciveData);                       //Sam 20220617 asCommand >> sReciveData

        fPadInterface->RecordCommunication("[Recv]", sReciveData);              //Sam 20220617 asCommand >> sReciveData
    }
    else if(sUnit=="t08")                                                       //JerryYang 20230814 : add震動馬達通訊調速版本
    {
        sAxis=sReciveData.SubString(4, 1);
        sFunc=sReciveData.SubString(6, 2);

        int iSp[4]={0,0,0,0};

        iSp[0]=atoi(sReciveData.SubString(8, 2).c_str());
        iSp[1]=atoi(sReciveData.SubString(10, 2).c_str());
        iSp[2]=atoi(sReciveData.SubString(12, 2).c_str());
        iSp[3]=atoi(sReciveData.SubString(14, 2).c_str());

        iAxis=atoi(sAxis.c_str());                                              //Steven 20200706 : 確保數字沒有超出陣列

        if(iAxis>=0 && iAxis<=VibrationMotorCount)
        {
            Msg.sprintf("RECV, %s, ", sVibrationMotorAxis[atoi(sAxis.c_str())]);
        }
        else
        {
            Msg.sprintf("RECV, Data format error!");
        }

        fMain->RecordCommunication(Msg, sReciveData);
        bReceiveVibrateMotData=true;
    }
}
//---------------------------------------------------------------------------
void TdmTrayMotor::AddTrayStepMotorLog(AnsiString sFunction, AnsiString str)    //Steven 20200529 : Loader入Tray改步進
{
    AnsiString sReciveData;
    sReciveData.sprintf("%02d:%02d:%02d.%03d, %s, %s", SystemHour, SystemMin, SystemSec, SystemMSec, sFunction, str);
    if(mmoTrayStepMotor!=NULL)
    {
        if(mmoTrayStepMotor->Lines->Count>1024)
            mmoTrayStepMotor->Clear();
        mmoTrayStepMotor->Lines->Add(sReciveData);
    }
}
//---------------------------------------------------------------------------
void __fastcall TdmTrayMotor::CMD_CheckStepMotorVer(int Axis)                   //ASK Ver
{
    AnsiString StepMotorCMD, Msg;
    StepMotorCMD.sprintf("t%03x8%02x%02x%02x%02x%02x%02x%02x%02x\r", 0x70+Axis, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
    bReceiveData=false;
    SetStepMotorDelay.SetSecAndOn(dStepMotorDelay);
    comTrayStepMotor->WriteCommData(StepMotorCMD.c_str(), strlen(StepMotorCMD.c_str()));
    Msg.sprintf("SEND, %s, Version", sStepMotorAxis[Axis]);
    AddTrayStepMotorLog(Msg, StepMotorCMD);
}
//---------------------------------------------------------------------------
void __fastcall TdmTrayMotor::CMD_SetStepMotorSpeed(int Axis, int SpeedPerc)
{
    int iSpeedPerc=0;

    AnsiString StepMotorCMD, Msg;
    if(fTrayMapping->IsRunTrayMapLaser() || fTrayMapping->IsRunDeviceRemainLaser())     //20220607 jimmychiu traymap laser run no sencond speed
    {
        iSpeedPerc=fTrayMapping->GetLoaderCWSpeed()*140;
    }
    else
    {
        iSpeedPerc=SpeedPerc*140;                                                   //Steven 20200630 : 奉Jeremy指示, 先讓機台穩穩跑, 所以減慢速度
    }

    StepMotorCMD.sprintf("t%03x8%02x%02x%02x%02x%02x%02x%04x\r", 0x70+Axis, 0x06, 0x01, 0x01, 0x00, 0x00, 0x00, iSpeedPerc);
    bReceiveData=false;
    SetStepMotorDelay.SetSecAndOn(dStepMotorDelay);
    comTrayStepMotor->WriteCommData(StepMotorCMD.c_str(), strlen(StepMotorCMD.c_str()));
    Msg.sprintf("SEND, %s, Speed", sStepMotorAxis[Axis]);
    AddTrayStepMotorLog(Msg, StepMotorCMD);
}
//---------------------------------------------------------------------------
void __fastcall TdmTrayMotor::CMD_SetStepMotorSecondSpeedCW(int Axis, int SpeedPerc)   //使用Sensor 3做CW減速
{
    AnsiString StepMotorCMD_CW, Msg;
    int iSpeedPerc=SpeedPerc*30;
    if(Axis==4 || Axis==5)                                                      //Ifor 20200701 : 修改For 新舊版本韌體使用不同的速度
    {
        iSpeedPerc=SpeedPerc*50;
    }

    if(fTrayMapping->IsRunTrayMapLaser() || fTrayMapping->IsRunDeviceRemainLaser())     //20220607 jimmychiu traymap laser run no sencond speed
    {
        iSpeedPerc=fTrayMapping->GetLoaderCWSpeed()*140;
    } 
    StepMotorCMD_CW.sprintf("t%03x8%02x%02x%02x%02x%02x%02x%04x\r", 0x70+Axis, 0x16, 0x01, 0x05, 0x02, 0x00, 0x00, iSpeedPerc);
    bReceiveData=false;
    SetStepMotorDelay.SetSecAndOn(dStepMotorDelay);
    comTrayStepMotor->WriteCommData(StepMotorCMD_CW.c_str(), strlen(StepMotorCMD_CW.c_str()));
    Msg.sprintf("SEND, %s, CW_Speed", sStepMotorAxis[Axis]);
    AddTrayStepMotorLog(Msg, StepMotorCMD_CW);
}
//---------------------------------------------------------------------------
void __fastcall TdmTrayMotor::CMD_SetStepMotorSecondSpeedCCW(int Axis, int SpeedPerc)  //使用Sensor 4做CCW減速
{
    AnsiString StepMotorCMD_CCW, Msg;
    int iSpeedPerc=SpeedPerc*30;
    if(Axis==4 || Axis==5)                                                      //Ifor 20200701 : 修改For 新舊版本韌體使用不同的速度
    {
        iSpeedPerc=SpeedPerc*50;
    }

    if(fTrayMapping->IsRunTrayMapLaser() || fTrayMapping->IsRunDeviceRemainLaser())     //20220607 jimmychiu traymap laser run no sencond speed
    {
        iSpeedPerc=fTrayMapping->GetLoaderCWSpeed()*140;
    }
    StepMotorCMD_CCW.sprintf("t%03x8%02x%02x%02x%02x%02x%02x%04x\r", 0x70+Axis, 0x16, 0x01, 0x07, 0x03, 0x00, 0x00, iSpeedPerc);
    bReceiveData=false;
    SetStepMotorDelay.SetSecAndOn(dStepMotorDelay);
    comTrayStepMotor->WriteCommData(StepMotorCMD_CCW.c_str(), strlen(StepMotorCMD_CCW.c_str()));
    Msg.sprintf("SEND, %s, CCW_Speed", sStepMotorAxis[Axis]);
    AddTrayStepMotorLog(Msg, StepMotorCMD_CCW);
}
//---------------------------------------------------------------------------
void __fastcall TdmTrayMotor::CMD_SetStepMotorSecondSpeedAcc(int Axis)
{
    AnsiString StepMotorCMD, Msg;
    AnsiString asMotorAcc="0";
    if(MachineTypeChoice==Type_HT7080)                                          //Nickliu 20230315 add HT70XX Loader/Unloader NO Need Get Module Ver
    {
        asMotorAcc=4000;
        StepMotorCMD.sprintf("t%03x8%02x%02x%02x%02x%02x%02x%04x\r", 0x70+Axis, 0x16, 0x01, 0x05, 0x02, 0x00, 0x00, 4000);
    }
    else
    {
        if(iSetpMotorVer[Axis]<230)                                             //Ifor 20200701 : 修改For 新舊版本韌體使用不同的速度
        {
            StepMotorCMD.sprintf("t%03x8%02x%02x%02x%02x%02x%02x%04x\r", 0x70+Axis, 0x14, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00);  //Steven 20200616 : Tray Step馬達加減速改成0, 避免暴衝
        }
        else
        {
            StepMotorCMD.sprintf("t%03x8%02x%02x%02x%02x%02x%02x%04x\r", 0x70+Axis, 0x14, 0x01, 0x01, 0x00, 0x00, 0x00, 0xC8);
            asMotorAcc="200";
        }
    }

    bReceiveData=false;
    SetStepMotorDelay.SetSecAndOn(dStepMotorDelay);
    comTrayStepMotor->WriteCommData(StepMotorCMD.c_str(), strlen(StepMotorCMD.c_str()));
    Msg.sprintf("SEND, %s, ACC", sStepMotorAxis[Axis]);
    AddTrayStepMotorLog(Msg, StepMotorCMD);
}
//---------------------------------------------------------------------------
void __fastcall TdmTrayMotor::CMD_SetStepMotorSecondSpeedDec(int Axis)
{
    AnsiString StepMotorCMD, Msg;
    AnsiString asMotorDec="0";
    if(iSetpMotorVer[Axis]<230)                                                 //Ifor 20200701 : 修改For 新舊版本韌體使用不同的速度
    {
        StepMotorCMD.sprintf("t%03x8%02x%02x%02x%02x%02x%02x%04x\r", 0x70+Axis, 0x14, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00);  //Steven 20200616 : Tray Step馬達加減速改成0, 避免暴衝
    }
    else
    {
        StepMotorCMD.sprintf("t%03x8%02x%02x%02x%02x%02x%02x%04x\r", 0x70+Axis, 0x14, 0x01, 0x03, 0x00, 0x00, 0x00, 0x64);
        asMotorDec="100";
    }

    bReceiveData=false;
    SetStepMotorDelay.SetSecAndOn(dStepMotorDelay);
    comTrayStepMotor->WriteCommData(StepMotorCMD.c_str(), strlen(StepMotorCMD.c_str()));
    Msg.sprintf("SEND, %s, DEC", sStepMotorAxis[Axis]);
    AddTrayStepMotorLog(Msg, StepMotorCMD);
}
//---------------------------------------------------------------------------
void __fastcall TdmTrayMotor::CMD_SetVibrationMotorSpeed(int Axis)              //JerryYang 20230814 : add震動馬達通訊調速版本
{
    AnsiString sVibrationMotorCMD, Msg;
    int iHP1=0, iHP2=0, iSHT1=0, iSHT2=0, iAUTO1=0, iAUTO2=0, iAUTO3=0;

    double dDeviceDev=0.0;
    if(DeviceForm_File.XDimension<DeviceForm_File.YDimension)
    {
        dDeviceDev=DeviceForm_File.XDimension;
    }
    else
    {
        dDeviceDev=DeviceForm_File.YDimension;
    }

    if(dDeviceDev<6.0)
    {
        iHP1    =IniConfig.iC21MotSp_HP1[0];
        iHP2    =IniConfig.iC21MotSp_HP2[0];
        iSHT1   =IniConfig.iC21MotSp_SHT1[0];
        iSHT2   =IniConfig.iC21MotSp_SHT2[0];
        iAUTO1  =IniConfig.iC21MotSp_AUTO1[0];
        iAUTO2  =IniConfig.iC21MotSp_AUTO2[0];
        iAUTO3  =IniConfig.iC21MotSp_AUTO3[0];
    }
    else if(dDeviceDev>=6.0 && dDeviceDev<=20.0)
    {
        iHP1    =IniConfig.iC21MotSp_HP1[1];
        iHP2    =IniConfig.iC21MotSp_HP2[1];
        iSHT1   =IniConfig.iC21MotSp_SHT1[1];
        iSHT2   =IniConfig.iC21MotSp_SHT2[1];
        iAUTO1  =IniConfig.iC21MotSp_AUTO1[1];
        iAUTO2  =IniConfig.iC21MotSp_AUTO2[1];
        iAUTO3  =IniConfig.iC21MotSp_AUTO3[1];
    }
    else
    {
        iHP1    =IniConfig.iC21MotSp_HP1[2];
        iHP2    =IniConfig.iC21MotSp_HP2[2];
        iSHT1   =IniConfig.iC21MotSp_SHT1[2];
        iSHT2   =IniConfig.iC21MotSp_SHT2[2];
        iAUTO1  =IniConfig.iC21MotSp_AUTO1[2];
        iAUTO2  =IniConfig.iC21MotSp_AUTO2[2];
        iAUTO3  =IniConfig.iC21MotSp_AUTO3[2];
    }

    if(Axis==0)
        sVibrationMotorCMD.sprintf("t%03x8%02x%02x%02x%02x%02x%02x%02x%02x\r", 0x80+Axis, 0x04, iHP1, iHP2, iSHT1, iSHT2,0x00,0x00,0x00);
    else
        sVibrationMotorCMD.sprintf("t%03x8%02x%02x%02x%02x%02x%02x%02x%02x\r", 0x80+Axis, 0x04, iAUTO1, iAUTO2, iAUTO3, 0x00, 0x00, 0x00, 0x00);

    bReceiveVibrateMotData=false;
    SetVibrateMotorDelay.SetSecAndOn(dVibrateMotorDelay);
    comTrayStepMotor->WriteCommData(sVibrationMotorCMD.c_str(), strlen(sVibrationMotorCMD.c_str()));
    Msg.sprintf("SEND, %s, DEC", sVibrationMotorAxis[Axis]);
    fMain->RecordCommunication(Msg, sVibrationMotorCMD);
}
//---------------------------------------------------------------------------
void __fastcall TdmTrayMotor::CMD_CheckVibrationMotVer(int Axis)                //ASK Ver
{
    AnsiString StepMotorCMD, Msg;

    StepMotorCMD.sprintf("t%03x80000000000000000\r", 0x80+Axis);

    bReceiveVibrateMotData=false;
    SetVibrateMotorDelay.SetSecAndOn(dVibrateMotorDelay);
    comTrayStepMotor->WriteCommData(StepMotorCMD.c_str(), strlen(StepMotorCMD.c_str()));
    Msg.sprintf("SEND, %s, Version", sVibrationMotorAxis[Axis]);
    fMain->RecordCommunication(Msg,StepMotorCMD);
}
//---------------------------------------------------------------------------

