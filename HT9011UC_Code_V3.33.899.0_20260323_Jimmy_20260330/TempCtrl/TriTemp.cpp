#include "IncludeAllHeader.H"
#pragma hdrstop

#include "TriTemp.h"

#include "cinitial.h"
#include "cmydef.h"
#include "myTimer.h"
#include "cprod.h"
#include "csystem.h"
#include "adam6024.h"
#include "uLotInfo.h"
#include "uTemp_Set.h"
#include "mysensor.h"
#include "cMyDB.h"
#include "MyMotor.h"
#include "uteach.h"
#include "mymessbox.h"
#include "LastSet.h"
#include "iosetview.h"
#include "myswitch.h"
#include "ATC_Handler_Side.h"
#include "common.h"
#include "MyMotor.h"
#include "mycylin.h"
#include "main.h"
#include "MyKitSuck.h"
#include "cTemperFrom.h"
#include "InterfaceSYS.h"
#include "uhome.h"
#include "ckernel.h"
#include "myTimer.h"
#include "uLotInfo.h"
#include "cDefrostNote.h"
//#include "note.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
TQPF_Timer TimerCheckInterval;
extern bool NoteFormShow();                                                     //Ztex 2023.04.19 Add HT-1032 TriTemp Function
extern void ShowNoteMessage_1032(AnsiString sMsg);
int iTaskCheckUltraHighTemperatureKit=0;                                        //Hmy 20200407 Add Backup SystemData
int iTaskBackUpSystemData=0;                                                    //Hmy 20200407 Add Backup SystemData
extern int ShowErrorMessage(AnsiString Code, int KCode, int Pos, bool bDuplicateErr=false, AnsiString errPart=" ");
//==============================================================================
//                              SCAN TRITEMP STATE
//==============================================================================
void DoTriTempState()
{
    AnsiString aDebugLog="";
    int iWorkTemp=(LastSet.iTemperature==Tempture_Ambient)?25:Temperature.fWorkTemperBase;

    if(DewPoint_Hardware_Install==0)                                            //Ifor 20230515 add:隨時都要偵測露點計
    {

    }
    else
    {
        try                                                                     //Hmy 20180313 add try catch(Auto Close)
        {
            static bool bFirstRun=true;
            static int iFirstRunCount=0;
            if(bFirstRun==true)
            {
                iFirstRunCount++;
                if(iFirstRunCount>=700)
                {
                    bFirstRun=false;
                }
                else
                {
                    return;
                }
            }

            if(fTeach->fShow==false && iWorkTemp<25)
            {
                LowTempIdleCheckSafeDoor();                                     //Isaac 20181214 : 停止狀態偵測chamber的門和Shuttle Top Cover
            }

            if(DewPoint_Hardware_Install>0)
            {
                fCheckDewPointStatus();
            }
        }
        catch(...)
        {
            MyDBIProcess("Exception", "DoTriTempState");
            aDebugLog.printf("%s %s --> %s [ %6d ]",__DATE__,__TIME__,__FUNC__,__LINE__);   //pig 20170105 RecordTryCatchLog
            RecordProcess(aDebugLog);                                                       //pig 20170105 RecordTryCatchLog
        }
    }
}
//---------------------------------------------------------------------------
TQPF_Timer DoCheckDewPoint;                                                     //Ifor 20230515 add:露點計連續異常時間
void fCheckDewPointStatus()                                                     //Hmy 20170510 add check DewPointMeter
{
    static bool bTimerStart=true;
    double dChillerTemp=0.0;                                                    //Ifor 20230515 add:Chiller溫度
    static bool bOldRunStatus=false;                                            //Ifor 20230515 add:Run/Stop flag
    static bool bHasError=false;                                                //Ifor 20230515 add:Error flag
    bool bStartCheck=false;                                                     //Ifor 20230515 add:開始計數
    double dbSetATCTemp=0.0;                                                    //ATC 工作溫度
    TPanel *tPnl[3]={fLotInfo->pnl_DewPoint_InArm,                              //Ztex 2023.04.19 Add HT-1032 TriTemp Function
                     fLotInfo->pnl_DewPoint_Index,
                     fLotInfo->pnl_DewPoint_OutArm};
    static double dAdamDewValue[3]={0, 0, 0};                                   //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    static double dOldDewPValue[3]={0, 0, 0};                                   //Ztex 2023.04.19 Add HT-1032 TriTemp Function
    bool bCheckState[3]={false, false, false};
    AnsiString Str="";

    try                                                                         //Hmy 20170728 add try catch
    {
        if(bTimerStart==true)
        {
            bTimerStart=false;
            TimerCheckInterval.SetSecAndOn(1);                                  //Isaac 20181225 : 一秒記錄一次
        }

        if(TimerCheckInterval.Off())                                            //Steven 20110720 : 等待Hand Down Time
        {
            bTimerStart=true;

            if(LastSet.iTemperature==Tempture_Hot || LastSet.iTemperature==Tempture_AmbientHot)
                dbSetATCTemp=Temperature.fWorkTemperBase;
            else
                dbSetATCTemp=IniConfig.dATCAmbientTemperature;

            if(DewPoint_Hardware_Install==1 || DewPoint_Hardware_Install==2)
            {
                if(Tri_Temp_Machine==1)                                         //Ztex 2023.04.19 Add HT-1032 TriTemp Function
                {
                    for(int i=0; i<3; i++)                                      //檢查 Dew Point Status
                    {
                        ADAM_ReadAIValue(0, 2+i, &dAdamValue_mA, &dAdamValue_Degree);
                        dAdamDewValue[i]=dAdamValue_Degree;
                        if(IniConfig.bL45_SetDewPointOffset==true)              //Ztex 2025.04.01 Add Set Dew Point Offset
                        {
                            dAdamDewValue[i]+=IniConfig.iL45_SetDewPointOffset;
                            if(dAdamDewValue[i]<-60)
                                dAdamDewValue[i]=-60;
                        }
                        tPnl[i]->Caption=FloatToStr(dAdamDewValue[i]);
                        tPnl[i]->Hint   =FloatToStr(dAdamValue_mA)+" mA";

                        if(dOldDewPValue[i]!=dAdamDewValue[i])
                        {
                            dOldDewPValue[i]=dAdamDewValue[i];
                            Str.sprintf("%f, %f", dAdamValue_Degree, dAdamValue_mA);
                            fMain->slDewPointLog[i]->AddTextWithDateTime(Str);  //Steven 20230810 : 三溫露點計log
                        }

                        if(i==2)
                        {
                            dFixAreaDewPoint=dAdamDewValue[i];
                            fTemp_Set->Handler_Send_To_ATC_DewPoint(dAdamDewValue[0], dAdamDewValue[1], dAdamDewValue[2]);
                        }
                    }
                }
                else
                {
                    ADAM_ReadAIValue(0, 3, &dAdamValue_mA, &dAdamValue_Degree); //轉換電流4~20mA為露點值
                    Str.sprintf("%f, %f", dAdamValue_Degree, dAdamValue_mA);
                    fMain->slDewPointLog[0]->AddTextWithDateTime(Str);          //Steven 20230810 : 三溫露點計log
                }

                if(Temperature.bATCActiveCooling==true &&
                   ATC_InterfaceForm->IsConnect()==true)                        //Ifor 20230519 add:ATC連線才判斷Dew
                {
                    if(SystemStart!=bOldRunStatus)                              //狀態改變重新計數
                    {
                        bOldRunStatus=SystemStart;
                        bHasError=false;
                    }

                    if(ATC_InterfaceForm->iATC_MODE_TYPE>=ATC_TYPE_33 &&        //Chiller
                       ATC_InterfaceForm->iATC_MODE_TYPE<=ATC_TYPE_35)
                    {
                        //Ifor 20230515 add:露點計異常報警
                        //==>
                        if(CosFunction.bUseHandlerSetChillerTemp==true)
                        {
                            dChillerTemp=(double)Temperature.iATCChillerTemp;   //Ifor 20230515 add: Handler Chiller 設定值
                        }
                        else
                        {
                            dChillerTemp=ATC_Data.dChillerSetTemp;              //Ifor 20230515 add:目前ATC Chiller 設定溫度
                        }

                        if(dChillerTemp<15.0)                                   //Ifor 20230515 add: 有開ATC 且 水溫設定小於15度才監控露點計
                        {                                                       //ATC 3.3 露點高於-5 報警，ATC 3.5 露點高於-15 報警
                            if((ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_33 && dAdamValue_Degree>-5.0) ||
                               (ATC_InterfaceForm->iATC_MODE_TYPE==ATC_TYPE_35 && dAdamValue_Degree>-15.0)||
                               dAdamValue_Degree>0.0)
                            {
                                bStartCheck=true;
                            }
                            else
                            {
                                bStartCheck=false;
                            }
                        }
                        else
                        {
                            bStartCheck=false;
                        }
                    }
                    else if(ATC_InterfaceForm->iATC_MODE_TYPE>=ATC_TYPE_50 &&   //冷媒機
                            ATC_InterfaceForm->iATC_MODE_TYPE<ATC_TYPE_60)
                    {
                        if(dbSetATCTemp<15)
                        {
                            if(dAdamValue_Degree>-15.0)
                            {
                                bStartCheck=true;
                            }
                            else
                            {
                                bStartCheck=false;
                            }
                        }
                        else
                        {
                            bStartCheck=false;
                        }
                    }
                    else if(ATC_InterfaceForm->iATC_MODE_TYPE>=ATC_TYPE_60 &&   //高低溫循環機
                            ATC_InterfaceForm->iATC_MODE_TYPE<ATC_TYPE_70)
                    {
                        if(Tri_Temp_Machine==1)                                 //Ztex 2023.04.19 Add HT-1032 TriTemp Function
                        {
                            for(int i=0; i<3; i++)
                            {
                                if(dbSetATCTemp<25 &&
                                   dAdamDewValue[i]>dbSetATCTemp)
                                {
                                    tPnl[i]->Font->Color=clWhite;
                                    tPnl[i]->Color=clRed;
                                    bStartCheck=true;
                                    bCheckState[i]=true;
                                }
                                else
                                {
                                    tPnl[i]->Font->Color=clBlack;
                                    tPnl[i]->Color=clAqua;
                                    bCheckState[i]=false;
                                }
                            }

                            if(bCheckState[0]==false && bCheckState[1]==false && bCheckState[2]==false)
                            {
                                bStartCheck=false;
                            }
                        }
                        else if(dbSetATCTemp<15)
                        {
                            if(dAdamValue_Degree>-15.0)
                            {
                                bStartCheck=true;
                            }
                            else
                            {
                                bStartCheck=false;
                            }
                        }
                        else
                        {
                            bStartCheck=false;
                        }
                    }
                    else                                                        //未定義的ATC 系統
                    {
                        ShowMyMessage("Undefined ATC system.", "未定義的 ATC 系統。");
                        bTimerStart=false;
                        TimerCheckInterval.SetSecAndOn(30);                     //30秒報警一次
                    }

                    if(bStartCheck==true)
                    {
                        if(fTeach->fShow==false)                                //Steven 20230712 : 檢查露點計的時候才確認加熱門
                        {
                            LowTempIdleCheckSafeDoor();                         //Isaac 20181214 : 停止狀態偵測chamber的門和Shuttle Top Cover
                        }

                        fLotInfo->pl_DewPoint->Font->Color=clRed;
                        fLotInfo->pl_DewPoint->Color=(TColor)0x001CB3FB;

                        if(bHasError==false)                                    //設定時間
                        {
                            if(SystemStart)
                            {
                                DoCheckDewPoint.SetSecAndOn(10);
                            }
                            else
                            {
                                DoCheckDewPoint.SetSecAndOn(60);
                            }
                            bHasError=true;
                        }

                        if(DoCheckDewPoint.Off())
                        {
                            if(CUSTOMER_CODE!=CC_HONPREC_QC)
                                ShowMyMessage("The dew point is beyond the operating range.", "露點超出操作範圍。");
                            bHasError=false;
                        }
                    }
                    else
                    {
                        fLotInfo->pl_DewPoint->Font->Color=clBlack;
                        fLotInfo->pl_DewPoint->Color=clAqua;
                        bHasError=false;
                    }
                }
                else
                {
                    bHasError=false;
                }
            }
            else if(DewPoint_Hardware_Install>0)                                //未定義的露點計
            {
                ShowMyMessage("Undefined hardware device.", "未定義的硬體裝置。");
                bTimerStart=false;
                TimerCheckInterval.SetSecAndOn(30);                             //30秒報警一次
            }
            fLotInfo->pl_DewPoint->Caption =FloatToStr(dAdamValue_Degree)+" ℃";
            fLotInfo->pl_DewPoint->Hint    =FloatToStr(dAdamValue_mA)+" mA";
        }
    }
    catch(...)
    {
        MyDBIProcess("Exception", "fCheckDewPointStatus");
    }
}
//---------------------------------------------------------------------------
bool fCheckMotorMoveCount_Shuttle(int iSht)                                     //Ztex 2023.04.19 Add HT-1032 TriTemp Function
{
    try
    {
        AnsiString sPath=AuthPath+"config.ini";                                 //檢查Shuttle 使用次數
        static int iTask[2]={1, 1};
        static int iShtMoveCount_His[2]={0, 0};
        static int iJamNowCount[2]={0, 0};
        static bool bNotWriteStatus[2]={false, false};
        bool bResult[2]={false, false};
        int iMot=MInShuttle1+iSht;

        if(!SystemStart)
        {
            if(bNotWriteStatus[iSht])
            {
                bNotWriteStatus[iSht]=false;
                if(iSht==0)
                {
                    WriteIniData(sPath, "F_Shuttle", "F31_3_iShuttleMotorMoveCountNow",     IniConfig.iShtMoveCntNow[iSht]);
                    WriteIniData(sPath, "F_Shuttle", "F31_5_iShuttleMotorMoveCountHistroy", iShtMoveCount_His[iSht]);
                }
                else
                {
                    WriteIniData(sPath, "F_Shuttle", "F31_4_iShuttleMotorMoveCountNow",     IniConfig.iShtMoveCntNow[iSht]);
                    WriteIniData(sPath, "F_Shuttle", "F31_6_iShuttleMotorMoveCountHistroy", iShtMoveCount_His[iSht]);
                }
            }
        }
        switch(iTask[iSht])
        {
            //========================================  //check 檔案存在
            case 1:
                iTask[iSht]=2;
                break;
            //======================================== ///讀取初始值
            case 2:
                iShtMoveCount_His[iSht]=IniConfig.iShtMoveCntHis[iSht];
                iTask[iSht]=10;
                break;
            //========================================//判斷Motor位置
            case 10:
                if(IniConfig.iShtMoveCntNow[iSht]>=IniConfig.iShtMoveCntSet[iSht])
                {
                    if(iJamNowCount[iSht]==0 || iJamNowCount[iSht]>=10000)
                    {
                        ///報警，請人員保養，持續等待人員清料
                        iJamNowCount[iSht]=1;
                        if(iSht==0)
                            ShowErrorMessage("WAR31001", K_RETRY, MMSystem);
                        else
                            ShowErrorMessage("WAR31002", K_RETRY, MMSystem);

                    }
                    else if(iJamNowCount[iSht]<10000)
                    {
                        if(SystemStart)
                            iJamNowCount[iSht]++;
                    }
                    iTask[iSht]=10;
                }
                else
                {
                    iJamNowCount[iSht]=0;
                    iTask[iSht]=20;
                }
                break;
            //========================================//判斷馬達已經回Home完成
            case 20:
                if(MOT[iMot].HomeFlag)
                {
                    iTask[iSht]=100;
                }
                break;
            //========================================//第一次開啟先判斷Motor位置
            case 100:
                if( MOT[iMot].ReadPos()==Prod.InSHT[iSht].iLeft ||
                   (MOT[iMot].ReadPos()<=Prod.InSHT[iSht].iLeft+3 &&
                    MOT[iMot].ReadPos()>=Prod.InSHT[iSht].iLeft-3))
                {
                    iTask[iSht]=110;
                }
                else if( MOT[iMot].ReadPos()==Prod.InSHT[iSht].iRight ||
                        (MOT[iMot].ReadPos()<=Prod.InSHT[iSht].iRight+3 &&
                         MOT[iMot].ReadPos()>=Prod.InSHT[iSht].iRight-3))
                {
                    iTask[iSht]=120;
                }
                break;
            //========================================//Motor位置當下在Left，等待移動至Right
            case 110:
                if( MOT[iMot].ReadPos()==Prod.InSHT[iSht].iRight ||
                   (MOT[iMot].ReadPos()<=Prod.InSHT[iSht].iRight+3 &&
                    MOT[iMot].ReadPos()>=Prod.InSHT[iSht].iRight-3))
                {
                    iTask[iSht]=120;
                }
                break;
            //========================================//Motor位置當下在Left，等待移動至Left
            case 120:
                if( MOT[iMot].ReadPos()==Prod.InSHT[iSht].iLeft ||
                   (MOT[iMot].ReadPos()<=Prod.InSHT[iSht].iLeft+3 &&
                    MOT[iMot].ReadPos()>=Prod.InSHT[iSht].iLeft-3))
                {    ///將Count增加，寫入後再重新判斷位置
                    IniConfig.iShtMoveCntNow[iSht]++;
                    iShtMoveCount_His[iSht]++;
                    IniConfig.iShtMoveCntHis[iSht]=iShtMoveCount_His[iSht];
                    bNotWriteStatus[iSht]=true;
                    if(IniConfig.iShtMoveCntNow[iSht]>=IniConfig.iShtMoveCntSet[iSht])
                    {
                        iJamNowCount[iSht]++;
                        if(iSht==0)
                            ShowErrorMessage("WAR31001", K_RETRY, MMSystem);
                        else
                            ShowErrorMessage("WAR31002", K_RETRY, MMSystem);
                        iTask[iSht]=10;
                        bResult[iSht]=true;
                    }
                    else
                    {
                        iTask[iSht]=110;
                    }
                }
                break;
        }

        return bResult[iSht];
    }
    catch(...)
    {
        MyDBIProcess("Exception", "fCheckMotorMoveCount_Shuttle"+AnsiString(iSht+1));
        return false;
    }
}
//------------------------------------------------------------------------------
bool IsTriSafeDoor6LockCheck()
{
    try
    {
        static bool blogWrite=false;                                            //檢查Safe Door 6
        if(Sen[SnTriTempSafeDoor6Lock].Enable)
        {
            if(Sen[SnTriTempSafeDoor6Lock].IsOn()==false)
            {
                SW[SwTriTempSafeDoor6Lock].On();
                blogWrite=false;
                return false;
            }
            else
            {
                SW[SwTriTempSafeDoor6Lock].Off();
                if(blogWrite==false)
                {
                    ShowErrorMessage("WAR31003", K_RETRY, MMSystem);
                    blogWrite=true;
                }
                return true;
            }
        }
        else
        {
            if(SW[SwTriTempSafeDoor6Lock].Status()==true)                                   
                SW[SwTriTempSafeDoor6Lock].Off();
        }
        return false;
    }
    catch(...)
    {
        MyDBIProcess("Exception", "IsTriSafeDoor6LockCheck");
        return false;
    }
}
//-------------------------------------------------------------------------------//三溫機 除霜相關Function
void fTriMachineSelectDeviceDefrost_Start(int iDevicePos)
{
    try
    {
        if(SystemStart)                                                         //除霜相關流程
            return;
        int iWorkTemp=(LastSet.iTemperature==Tempture_Ambient)?35:Temperature.fWorkTemperBase;
        int iDefrostTemp=IniConfig.iL32_4SetDefrostTemp+3;                      // Add Set Over
        AnsiString sArray[MAX_DEFROST_AREA] = {"IndexArm1、IndexArm2", "Shuttle1、HotPlate1", "Shuttle2、HotPlate2", "", "", "", ""};

        if(iWorkTemp<26)                                                        // Modify 25 ->26///設定溫度大於˙25度不做除霜動作
        {
            fTemp_Set->sgDefrostStatus->Enabled=true;
            for(int i=0; i<MAX_DEFROST_AREA; i++)
            {
                if(fTemp_Set->DefrostFunction.strDefrostDevice[i]=="" && fTemp_Set->DefrostFunction.bDefrostReq[i]==false)
                {
                    fTemp_Set->DefrostFunction.strDefrostDevice[iDevicePos]  = sArray[iDevicePos];;
                    fTemp_Set->DefrostFunction.bDefrostReq[iDevicePos]       = true;
                    fTemp_Set->sgDefrostStatus->Cells[0][iDevicePos+1]       = fTemp_Set->DefrostFunction.strDefrostDevice[iDevicePos];
                    fTemp_Set->sgDefrostStatus->Cells[1][iDevicePos+1]       = "Change Temp";
                    fTemp_Set->btnDefrostStart->Enabled             = false;
                    fTemp_Set->btnDefrostEnd->Enabled               = true;
                    fTemp_Set->bTestDefrost                         = true;
                    bDefrostKeepATCTemp                             = true;

                    SwitchRefrigerator(iDevicePos, 0);
                    fUseDeviceReHeat(iDevicePos, iDefrostTemp);
                    i=MAX_DEFROST_AREA;
                }
            }

            if(fTemp_Set->tmr_ATC_Deforst->Enabled==false)
            {
                fTemp_Set->tmr_ATC_Deforst->Enabled=true;
            }

        }
    }
    catch(...)
    {
        MyDBIProcess("Exception", "fTriMachineSelectDeviceDefrost_Start");
    }
}
//-------------------------------------------------------------------------------//三溫機 除霜相關Function
void fTriMachineSelectDeviceDefrost_End(int iDevicePos)
{
    try
    {
        AnsiString strSelectDefrostDevice;                                      //除霜相關流程
        static bool bDefrostChangeTemp=false;
        AnsiString sArray[MAX_DEFROST_AREA] = {"IndexArm1、IndexArm2", "Shuttle1、HotPlate1", "Shuttle2、HotPlate2", "", "", "", ""};

        for(int i=0; i<MAX_DEFROST_AREA; i++)
        {
            if(fTemp_Set->DefrostFunction.strDefrostDevice[i]==sArray[iDevicePos] &&
               fTemp_Set->DefrostFunction.bDefrostReq[i]==true)
            {
                fTemp_Set->sgDefrostStatus->Cells[1][i+1]       = "Defrost Termination";
                fTemp_Set->DefrostFunction.strDefrostDevice[i]  = "";
                fTemp_Set->DefrostFunction.bDefrostReq[i]       = false;
                fTemp_Set->DefrostFunction.bDefrostHeart_OK[i]  = false;
                fTemp_Set->DefrostFunction.bDefrostStart[i]     = false;
                fTemp_Set->btnDefrostStart->Enabled             = true;
                fTemp_Set->btnDefrostEnd->Enabled               = false;
                bDefrostChangeTemp                              = false;

                break;
            }
            else
            {
                bDefrostChangeTemp=true;
            }
        }

        for(int i=0; i<MAX_DEFROST_AREA; i++)
        {
            if(fTemp_Set->DefrostFunction.strDefrostDevice[i]=="" &&
               fTemp_Set->DefrostFunction.bDefrostReq[i]==false)
            {
                if(i==MAX_DEFROST_AREA-1)
                {
                    fTemp_Set->bTestDefrost =false;
                    bDefrostKeepATCTemp     =false;
                    bManualDefrost_Start    =false;
                    for(int j=0; j<MAX_DEFROST_AREA; j++)
                    {
                        fTemp_Set->sgDefrostStatus->Cells[1][j+1] = "";
                        fTemp_Set->sgDefrostStatus->Cells[0][j]="";
                    }
                    fMain->ChangeATCSiteUse();
                    bSetTempChange=true;
                    fUseDeviceReHeat(iDevicePos, 0);

                    if(bCheckUse_AirStream==true)
                        fLotInfo->btnAirStreamOnOff->Enabled=true;

                    if(bDefrostKeepATCTemp==false)
                        ShowErrorMessage("WAR31008", K_RETRY, MMSystem);
                }
            }
            else
            {
                break;
            }
        }
    }
    catch(...)
    {
        MyDBIProcess("Exception", "fTriMachineSelectDeviceDefrost_End");        //pig 20170105 RecordTryCatchLog
    }
}
//-------------------------------------------------------------------------------
bool fcheckDefrostStatus()
{
    bool bReturnStatus=false;                                                   //除霜相關流程
    static DWORD dwDefrostStartTime[MAX_DEFROST_AREA]  ={0, 0, 0, 0, 0, 0, 0};
    static DWORD dwDefrostCountTime[MAX_DEFROST_AREA]  ={0, 0, 0, 0, 0, 0, 0};
    static DWORD dwDefrostHeatTimeOut[MAX_DEFROST_AREA]={0, 0, 0, 0, 0, 0, 0};
    static bool  bHeatingStart[MAX_DEFROST_AREA]={false, false, false, false, false, false, false};
    double fDefrostStartTime[MAX_DEFROST_AREA]={0, 0, 0, 0, 0, 0, 0};
    double fDefrostCountTime[MAX_DEFROST_AREA]={0, 0, 0, 0, 0, 0, 0};
    AnsiString sArray[MAX_DEFROST_AREA] = {"IndexArm1、IndexArm2", "Shuttle1、HotPlate1", "Shuttle2、HotPlate2", "", "", "", ""};

    try
    {
        for(int i=0; i<MAX_DEFROST_AREA; i++)
        {
            if(fTemp_Set->DefrostFunction.strDefrostDevice[i]!="" && fTemp_Set->DefrostFunction.bDefrostReq[i]==true)
            {
                if(sArray[i]==fTemp_Set->DefrostFunction.strDefrostDevice[i])
                {
                    dwDefrostCountTime[i] =GetTickCount();
                    if(fTemp_Set->DefrostFunction.bDefrostHeart_OK[i]==false)
                    {
                        if(bHeatingStart[i])
                        {
                            bHeatingStart[i]=true;
                            dwDefrostHeatTimeOut[i]=GetTickCount();
                            fTemp_Set->sgDefrostStatus->Cells[1][i+1]="heating";
                        }
                        else if(bHeatingStart[i] && fTemp_Set->sgDefrostStatus->Cells[1][i+1]!="heating")
                        {
                            bHeatingStart[i]=false;
                        }

                        if(fcheckDefrostTempature(i))
                        {
                            fTemp_Set->DefrostFunction.bDefrostHeart_OK[i]=true;
                            fTemp_Set->DefrostFunction.bDefrostStart[i]   =true;
                            dwDefrostStartTime[i]=dwDefrostCountTime[i];
                        }
                        else if(bHeatingStart[i])
                        {
                            if(dwDefrostCountTime[i]-dwDefrostHeatTimeOut[i]>0)
                            {
                                int iHeartingTime_min=(dwDefrostCountTime[i]-dwDefrostHeatTimeOut[i])/1000/60;
                                if(iHeartingTime_min>=20)                           //20min
                                {
                                    ShowErrorMessage("WAR31009", K_RETRY, MMSystem);
                                    bHeatingStart[i]=false;
                                    fTriMachineSelectDeviceDefrost_End(i);
                                }
                            }
                        }
                    }
                    else if(fTemp_Set->DefrostFunction.bDefrostHeart_OK[i] && fTemp_Set->DefrostFunction.bDefrostStart[i] ==true)
                    {
                        fTemp_Set->sgDefrostStatus->Cells[1][i+1]="Defrosting";
                        int iDefrost_s      =IniConfig.iL32_5SetDefrostTime*60;
                        int iDefrost_ms     =IniConfig.iL32_5SetDefrostTime*60*1000;
                        fDefrostStartTime[i]=dwDefrostStartTime[i];
                        fDefrostCountTime[i]=dwDefrostCountTime[i];
                        if((fDefrostCountTime[i]-fDefrostStartTime[i])>=iDefrost_ms)
                        {    ///除霜時間完成
                            fTemp_Set->sgDefrostStatus->Cells[1][i+1]               ="Defrost Finish";
                            fTemp_Set->DefrostFunction.bDefrostReq[i]               =false;
                            fTemp_Set->DefrostFunction.strDefrostDevice[i]          ="";
                            fTemp_Set->DefrostFunction.bDefrostStart[i]             =false;
                            fTemp_Set->DefrostFunction.bDefrostHeart_OK[i]          =false;
                            fTemp_Set->DefrostFunction.bDefrostEnd[i]               =false;
                            fTemp_Set->DefrostFunction.iDefrostCountingDownTime[i]  =0;
                            fTemp_Set->btnDefrostEnd->Enabled                       =false;
                            fTemp_Set->btnDefrostStart->Enabled                     =true;
                            fTemp_Set->btn_DefrostAllUseStart->Enabled              =true;
                            fTemp_Set->btn_DefrostAllUseEnd->Enabled                =false;
                            bHeatingStart[i]                                        =false;
                            bDefrost_abnormalStatus=true;
                            fTriMachineSelectDeviceDefrost_End(i);
                        }
                        else
                        {
                            int iTotalSec=(dwDefrostCountTime[i]-dwDefrostStartTime[i])/1000;
                            int iCountingDownTime=iDefrost_s-iTotalSec;
                            int iMin=iCountingDownTime/60;
                            int iSec=iCountingDownTime%60;
                            fTemp_Set->sgDefrostStatus->Cells[0][i+1]=fTemp_Set->DefrostFunction.strDefrostDevice[i];
                            fTemp_Set->sgDefrostStatus->Cells[1][i+1]="Defrosting :"+IntToStr(iMin)+" Min,"+IntToStr(iSec)+" Sec";
                        }
                    }
                }
            }
            else
            {
                fTemp_Set->sgDefrostStatus->Cells[1][i+1]="";
                fTemp_Set->sgDefrostStatus->Cells[0][i+1]="";
            }
        }
        bReturnStatus=false;
        return bReturnStatus;
    }
    catch(...)
    {
        MyDBIProcess("Exception", "fcheckDefrostStatus");                       //pig 20170105 RecordTryCatchLog
    }

    return false;
}
//------------------------------------------------------------------------------
void SwitchRefrigerator(int iDevice, int iOnOff)                                //除霜相關流程
{
    if(iDevice==0)
        fTemp_Set->SwitchOnOffRefrigeratorNo(1, iOnOff);
    else if(iDevice==1)
        fTemp_Set->SwitchOnOffRefrigeratorNo(2, iOnOff);
    else if(iDevice==2)
        fTemp_Set->SwitchOnOffRefrigeratorNo(3, iOnOff);
}
//------------------------------------------------------------------------------
void fUseDeviceReHeat(int iDevice, int iHeatTemperature)
{   //ATC 5.1系統......
    int iIndex_Arm,iPlate_Shuttle_1;
    double dDefrostTemp =IniConfig.iL32_8SetAirStreamTemp;
    try
    {
        if(Tri_Temp_Machine==1)
        {
            iIndex_Arm       = 32;
            iPlate_Shuttle_1 = 35;
        }
        else
        {
            return;
        }

        if(iDevice==0)//0~31
        {
            for(int i=0; i<iIndex_Arm; i++)
            {
                fTemp_Set->SetSingleWorkTemperature(i, iHeatTemperature,true);
            }
            ATC_InterfaceForm->SendAirMachineStatus(1, dDefrostTemp*10, dDefrostTemp*10);
        }
        else if(iDevice==1)//32~35
        {
            for(int i=iIndex_Arm; i<iPlate_Shuttle_1; i++)
                fTemp_Set->SetSingleWorkTemperature(i, iHeatTemperature,true);
        }
        else if(iDevice==2)//36~39
        {
            for(int i=iPlate_Shuttle_1; i<ATC_MAX_SITE; i++)
                fTemp_Set->SetSingleWorkTemperature(i, iHeatTemperature,true);
        }
    }
    catch(...)
    {
        MyDBIProcess("Exception", "fUseDeviceReHeat");
    }
}
//------------------------------------------------------------------------------
bool __fastcall UseIndexVacuumWhenDefrosting(int iIndex, int iOnOff)            //Add Use IndexArm to blow off when defrosting
{
    if(iIndex==0)                                                               //除霜相關流程
    {
        for(int i=0; i<MAX_Index_Row; i++)
        {
            for(int j=0; j<NEW_MAX_Index_Col; j++)
            {
                if(iOnOff==1)                                                   //Air Blow On
                {
                    if(FTestSuck.Item[i][j]==NULL_IC || FTestSuck.Item[i][j]==HAS_NULL_IC)
                        FTestSuck.Suck[i][j].On();
                }
                else
                {
                    if(FTestSuck.Item[i][j]==NULL_IC || FTestSuck.Item[i][j]==HAS_NULL_IC)
                        FTestSuck.Suck[i][j].Normal();
                }
            }
        }
    }
    else if(iIndex==1)
    {
        for(int i=0; i<MAX_Index_Row; i++)
        {
            for(int j=0; j<NEW_MAX_Index_Col; j++)
            {
                if(iOnOff==1)                                                   //Air Blow On
                {
                    if(BTestSuck.Item[i][j]==NULL_IC || BTestSuck.Item[i][j]==HAS_NULL_IC)
                        BTestSuck.Suck[i][j].On();
                }
                else
                {
                    if(BTestSuck.Item[i][j]==NULL_IC || BTestSuck.Item[i][j]==HAS_NULL_IC)
                        BTestSuck.Suck[i][j].Normal();
                }
            }
        }
    }
    return false;
}
//-------------------------------------------------------------------------------
bool __fastcall fcheckDefrostTempature(int iMachineDevice) //增加手動除霜功能(判斷加熱溫度是否到設定溫度)
{
    //Add ATC系統溫控器數量(預設為1點，目前最多3點)<==
    //HT-1032增加與HT-1028相同功能==>
    TPanel *pnTempArm1[]={fTemperFrom->hlTempAa1, fTemperFrom->hlTempAb1, fTemperFrom->hlTempAc1, fTemperFrom->hlTempAd1, fTemperFrom->hlTempBa1, fTemperFrom->hlTempBb1, fTemperFrom->hlTempBc1, fTemperFrom->hlTempBd1,
                          fTemperFrom->hlTempAe1, fTemperFrom->hlTempAf1, fTemperFrom->hlTempAg1, fTemperFrom->hlTempAh1, fTemperFrom->hlTempBe1, fTemperFrom->hlTempBf1, fTemperFrom->hlTempBg1, fTemperFrom->hlTempBh1};

    TPanel *pnTempArm2[]={fTemperFrom->hlTempAa2, fTemperFrom->hlTempAb2, fTemperFrom->hlTempAc2, fTemperFrom->hlTempAd2, fTemperFrom->hlTempBa2, fTemperFrom->hlTempBb2, fTemperFrom->hlTempBc2, fTemperFrom->hlTempBd2,
                          fTemperFrom->hlTempAe2, fTemperFrom->hlTempAf2, fTemperFrom->hlTempAg2, fTemperFrom->hlTempAh2, fTemperFrom->hlTempBe2, fTemperFrom->hlTempBf2, fTemperFrom->hlTempBg2, fTemperFrom->hlTempBh2};

    if(Tri_Temp_Machine!=1)                                                     //除霜相關流程
        return false;
    //HT-1032增加與HT-1028相同功能<==
    try    //add try catch(Auto Close)
    {
        bool bReturnStatus=false;
        int iDefrostTempature=IniConfig.iL32_4SetDefrostTemp;
        bool bCheck_1=false,bCheck_2=false;

        if(ATC_InterfaceForm->IsConnect()==false)
            return true;

        if(iMachineDevice==0)
        {
            bCheck_1=true;
            bCheck_2=true;
            for(int i=0; i<16; i++)
            {
                bCheck_1&=(pnTempArm1[i]->Caption=="---" || iDefrostTempature<=atoi(pnTempArm1[i]->Caption.c_str()))?true:false;
                bCheck_2&=(pnTempArm2[i]->Caption=="---" || iDefrostTempature<=atoi(pnTempArm2[i]->Caption.c_str()))?true:false;
            }
        }
        else if(iMachineDevice==1)
        {
            if((fTemperFrom->hlTempShuttle1_2->Caption=="---" &&
                fTemperFrom->hlTempShuttle2_2->Caption=="---") ||
               (iDefrostTempature<=atoi(fTemperFrom->hlTempShuttle1_2->Caption.c_str()) &&
                iDefrostTempature<=atoi(fTemperFrom->hlTempShuttle2_2->Caption.c_str())))
            {
                bCheck_1=true;
            }

            if((fTemperFrom->hlTempPlate1_2->Caption=="---" &&
                fTemperFrom->hlTempPlate2_2->Caption=="---") ||
               (iDefrostTempature<=atoi(fTemperFrom->hlTempPlate1_2->Caption.c_str()) &&
                iDefrostTempature<=atoi(fTemperFrom->hlTempPlate2_2->Caption.c_str())))
            {
                bCheck_2=true;
            }
        }
        else if(iMachineDevice==2)
        {
            if((fTemperFrom->hlTempShuttle_3->Caption=="---" &&
                fTemperFrom->hlTempShuttle_4->Caption=="---") ||
               (iDefrostTempature<=atoi(fTemperFrom->hlTempShuttle_3->Caption.c_str()) &&
                iDefrostTempature<=atoi(fTemperFrom->hlTempShuttle_4->Caption.c_str())))
            {
                bCheck_1=true;
            }

            if((fTemperFrom->hlTempPlate_3->Caption=="---" &&
                fTemperFrom->hlTempPlate_4->Caption=="---") ||
               (iDefrostTempature<=atoi(fTemperFrom->hlTempPlate_3->Caption.c_str()) &&
                iDefrostTempature<=atoi(fTemperFrom->hlTempPlate_4->Caption.c_str())))
            {
                bCheck_2=true;
            }
        }

        if(bCheck_1 && bCheck_2)
        {
            bReturnStatus=true;
        }

        return bReturnStatus;
    }
    catch(...)
    {
        MyDBIProcess("Exception", "fUseDeviceReHeat");
    }

    return false;
}
//------------------------------------------------------------------------------
void fAllDevicePosDefrostON_OFF(AnsiString strOnOff)                            //目前只針對有使用之位置做除霜
{
    try
    {
        AnsiString asDevice="";                                                 //除霜相關流程
        double dTemp = IniConfig.iL32_4SetDefrostTemp+3;                        //除霜相關流程

        if(strOnOff=="On")
        {
            for(int i=0; i<MAX_DEFROST_AREA; i++)
            {
               fTemp_Set->DefrostFunction.bDefrostReq[i]        =false;
               fTemp_Set->DefrostFunction.strDefrostDevice[i]   ="";
            }

            for(int j=0; j<fTemp_Set->cbSelectDevice->Items->Count; j++)
            {
                fTriMachineSelectDeviceDefrost_Start(j);
            }

            ATC_InterfaceForm->ChangeRecipe("Defrost_Up",dTemp*10);
            ATC_InterfaceForm->Run();
            ATC_InterfaceForm->Send_AtcDefrostStatus(1);                        //Add DEFROST Command
        }
        else
        {
            for(int j=0; j<fTemp_Set->cbSelectDevice->Items->Count; j++)
            {
                fTriMachineSelectDeviceDefrost_End(j);
            }
        }
    }
    catch(...)
    {
        MyDBIProcess("Exception", "fAllDevicePosDefrostON_OFF");
    }
}
//------------------------------------------------------------------------------
TQPF_Timer HtmrScanDelay;
TQPF_Timer HtmrCheckFanDelay;
bool fHighTemperatureExhaustAir()                                               //High Temperature Must Exhaust Air(Use Fan)
{
    static int iTask=1;                                                         //超高溫使用風扇
    bool bReturnStatus=false;
    static bool bTimerStart=true;

    int iReturn  =0;
    int iWorkTemp=(LastSet.iTemperature==Tempture_Ambient)?35:Temperature.fWorkTemperBase;
    int iCurrStat=0;                                                            //20191112 Mark 變更判斷開啟之方式(先將風扇與門關閉，再判斷是否ATC ON LINE溫度)    
	//Wenqi 20240516 Modify 高溫風扇判斷
    TPanel *tmpPanel[]={fTemperFrom->hlTempShuttle1_2,
                        fTemperFrom->hlTempShuttle2_2,
                        fTemperFrom->hlTempPlate1_2,
                        fTemperFrom->hlTempPlate2_2 };                          //20191112 Mark 變更判斷開啟之方式(先將風扇與門關閉，再判斷是否ATC ON LINE溫度)
    #ifdef SOFT_SIMULTE
        return false;
    #endif

    if(Tri_Temp_Machine!=1)
        return false;

    if(bTimerStart==true)
    {
        bTimerStart=false;
        TimerCheckInterval.SetSecAndOn(1);
    }

    if(TimerCheckInterval.Off()==false)
    {
        return bReturnStatus;
    }

    if(IniConfig.bL35_1OverSetTempOpenFan==false ||
       (iWorkTemp>IniConfig.iL35_2OpenFanTemp && UN150Read[tcATCHotAir1]<10))
    {
        iTask=1;
        return bReturnStatus;
    }

    if(iWorkTemp<=25)                                                           //Ztex 2024.03.26 Add No need to use at low temperatures
    {
        if(SW[SwEnhaustAirFanPowerOn].Status()==true)
            SW[SwEnhaustAirFanPowerOn].Off();
        if(Cylinder[C_EnhaustAirVentOpen].GetOutBit()==true)
            Cylinder[C_EnhaustAirVentOpen].Off();
        if(Cylinder[C_EnhaustAirVentClose].GetOutBit()==false)
            Cylinder[C_EnhaustAirVentClose].On();

        return bReturnStatus;
    }

    if(iWorkTemp>IniConfig.iL35_2OpenFanTemp)
    {
		//Wenqi 20240516 Modify 高溫風扇判斷 ==>
        for(int i=0; i<4; i++)
        {
            if(tmpPanel[i]->Caption =="---" ||
               (atof(tmpPanel[i]->Caption.c_str())>IniConfig.iL35_2OpenFanTemp))
            {
              if(i==3)
                iCurrStat=1;
            }
            else
            {
                iCurrStat=2;
                break;  //Wenqi 20240516 Modify 高溫風扇判斷
            }
        }
		//Wenqi 20240516 Modify 高溫風扇判斷 <==
//        iCurrStat=1;  //Wenqi 20240516 Modify 高溫風扇判斷
    }
    else
    {
        iCurrStat=2;
    }

    switch(iTask)
    {
        case 1:
            if(IniConfig.bL35_1OverSetTempOpenFan==false)                       //no use
                return bReturnStatus;
            else
                iTask=100;
            break;
        case 100:
            if(SW[SwEnhaustAirFanPowerOn].Status()==true)
                SW[SwEnhaustAirFanPowerOn].Off();
            HtmrCheckFanDelay.SetSecAndOn(3);
            iTask=2100;
            break;
        case 2100:
            if(HtmrCheckFanDelay.Off())
            {
                if(Sen[SnEnhaustAirFanAlarmDetect].IsOn()==false)               //check Fan Run
                {
                    if(Cylinder[C_EnhaustAirVentOpen].GetOutBit()==true)
                        Cylinder[C_EnhaustAirVentOpen].Off();
                    if(Cylinder[C_EnhaustAirVentClose].GetOutBit()==false)
                        Cylinder[C_EnhaustAirVentClose].On();
                    HtmrCheckFanDelay.SetSecAndOn(5);
                    iTask=2200;
                }
                else
                {
                    SW[SwEnhaustAirFanPowerOn].Off();
                    StopAllMotor();
                    SystemStart=false;
                    if(NoteFormShow()==false)
                        iReturn=ShowErrorMessage("MES16440", K_RETRY, MMSystem);

                    if(iReturn==K_RETRY)
                    {
                        HtmrCheckFanDelay.SetSecAndOn(5);
                        iTask=2100;
                    }
                }
            }
            break;
        case 2200:
            if(HtmrCheckFanDelay.Off())
            {
                if(Cylinder[C_EnhaustAirVentOpen].OnSensor()==false &&
                   Cylinder[C_EnhaustAirVentClose].OnSensor()==true)
                {
                    if(iCurrStat==1)                                            //20191112 Mark 變更判斷開啟之方式(先將風扇與門關閉，再判斷是否ATC ON LINE溫度) ->
                    {
                        if(Cylinder[C_EnhaustAirVentClose].GetOutBit()==true)
                            Cylinder[C_EnhaustAirVentClose].Off();
                        if(Cylinder[C_EnhaustAirVentOpen].GetOutBit()==false)
                            Cylinder[C_EnhaustAirVentOpen].On();
                        HtmrCheckFanDelay.SetSecAndOn(2);
                        iTask=2300;
                    }
                    else
                    {
                        HtmrCheckFanDelay.SetSecAndOn(3);
                        iTask=2100;
                    }
                }
                else
                {
                    if(Cylinder[C_EnhaustAirVentOpen].OnSensor()==true ||
                       Cylinder[C_LUpEnhaustAirOpen].OnSensor() ==true ||
                       Cylinder[C_RUpEnhaustAirOpen].OnSensor() ==true)     //yunghsin 20220705 Add Up High Temperature Must Exhaust Air(Use Fan)
                    {
                        iReturn=ShowErrorMessage("MES16437", K_RETRY, MMSystem);
                    }

                    if(Cylinder[C_EnhaustAirVentClose].OnSensor()==false ||
                       Cylinder[C_LUpEnhaustAirClose].OnSensor()  ==false ||
                       Cylinder[C_RUpEnhaustAirClose].OnSensor() ==false)   //yunghsin 20220705 Add Up High Temperature Must Exhaust Air(Use Fan)
                    {
                        iReturn=ShowErrorMessage("MES16438", K_RETRY, MMSystem);
                    }

                    if(iReturn==K_RETRY)                                        //yunghsin 20181030 Modify
                    {
                        if(Cylinder[C_EnhaustAirVentOpen].GetOutBit()==true)
                            Cylinder[C_EnhaustAirVentOpen].Off();
                        if(Cylinder[C_EnhaustAirVentClose].GetOutBit()==false)
                            Cylinder[C_EnhaustAirVentClose].On();
                        HtmrCheckFanDelay.SetSecAndOn(5);
                        iTask=2200;
                    }
                }
            }
            break;
        case 2300:
            if(HtmrCheckFanDelay.Off())                                         //check door Open
            {
                if(iCurrStat==1)                                                //20191112 Mark 變更判斷開啟之方式(先將風扇與門關閉，再判斷是否ATC ON LINE溫度)
                {
                    if(Cylinder[C_EnhaustAirVentOpen].OnSensor()==true &&
                       Cylinder[C_EnhaustAirVentClose].OnSensor()==false)
                    {
                        if(SW[SwEnhaustAirFanPowerOn].Status()==false)
                            SW[SwEnhaustAirFanPowerOn].On();
                        HtmrCheckFanDelay.SetSecAndOn(5);
                        iTask=2400;
                    }
                    else
                    {
                        if(Cylinder[C_EnhaustAirVentOpen].OnSensor() ==false)
                        {
                            iReturn=ShowErrorMessage("MES16437", K_RETRY, MMSystem);
                        }
                        if(Cylinder[C_EnhaustAirVentClose].OnSensor() ==true)
                        {
                            iReturn=ShowErrorMessage("MES16438", K_RETRY, MMSystem);
                        }
                        
                        if(iReturn==K_RETRY)                                    //yunghsin 20181030 Modify
                        {
                            if(Cylinder[C_EnhaustAirVentClose].GetOutBit()==true)
                                Cylinder[C_EnhaustAirVentClose].Off();
                            if(Cylinder[C_EnhaustAirVentOpen].GetOutBit()==false)
                                Cylinder[C_EnhaustAirVentOpen].On();
                            HtmrCheckFanDelay.SetSecAndOn(5); 
                            iTask=2300;
                        }
                    }
                }
                else
                {
                    iTask=100;                                                  //再次將風扇與門關閉
                }
            }
            break;
        case 2400:
            if(HtmrCheckFanDelay.Off())
            {
                if(iCurrStat==1)                                                //20191112 Mark 變更判斷開啟之方式(先將風扇與門關閉，再判斷是否ATC ON LINE溫度)
                {
                    if(Sen[SnEnhaustAirFanAlarmDetect].IsOn()==true) 
                    {
                        HtmrCheckFanDelay.SetSecAndOn(2);
                        iTask=2300;
                    }
                    else
                    {
                        SW[SwEnhaustAirFanPowerOn].Off();
                        StopAllMotor();
                        SystemStart=false;
                        if(NoteFormShow()==false)
                            iReturn=ShowErrorMessage("MES16439", K_RETRY, MMSystem);

                        if(iReturn==K_RETRY)
                        {
                            HtmrCheckFanDelay.SetSecAndOn(2);
                            iTask=2500;
                        }
                    }
                }
                else
                {
                    iTask=100;                                                  //再次將風扇與門關閉
                }
            }
            break;
        case 2500:
            if(iCurrStat==1)                                                    //20191112 Mark 變更判斷開啟之方式(先將風扇與門關閉，再判斷是否ATC ON LINE溫度)
            {
                if(HtmrCheckFanDelay.Off())                                     //Retry Fan Power On, Recheck Fan Run
                {
                    SW[SwEnhaustAirFanPowerOn].On();
                    HtmrCheckFanDelay.SetSecAndOn(5);
                    iTask=2400;
                }
            }
            else
            {
                iTask=100;                                                      //再次將風扇與門關閉
            }
        break;
    }
    return bReturnStatus;
}
//------------------------------------------------------------------------------
TQPF_Timer tFixDoorTimer;
int iSecOfFixDoorOpenAlarm=2;
bool __fastcall DoDelayTimeAfterFixDoorOpen()                                   //Add Fix Open Close Must Drying
{
    try
    {
        bool bflag[3]={false, false, false};                                    //開門需等待Drying Air
        static int iTask=1;
        int iWorkTemp=(LastSet.iTemperature==Tempture_Ambient)?35:Temperature.fWorkTemperBase;
        static int  iCurrTemp=-999;                                             //Modify 0 ->-999(工作檔為0度，重開軟體判斷異常)
        static int  iDewPoint=25;
        static bool bDewPoint=false;
        bool bDoorflag[2]={false, false};

        int iTime;
        int iCountCompare;

        if(iCurrTemp!=iWorkTemp)
        {
            iCurrTemp=iWorkTemp;
            if(iCurrTemp>=25)//大於25度
            {
                bDewPoint=true;
                iDewPoint=iCurrTemp -10;
            }
            else
            {
                bDewPoint=false;
                iDewPoint=IniConfig.dL34_3DewPointOfFixDoorOpen;
            }
        }

        bool bFixAreaSmallDoorOpen  =(Sen[SnSafeDoor6Hatchway].IsOff()==true || Sen[SnSafeDoor7Hatchway].IsOff()==true || Sen[SnSafeDoor8Hatchway].IsOff()==true);
        bool bFixAreaBigDoorOpen    =(Sen[SnSafeDoor6].IsOff()==true || Sen[SnSafeDoor7].IsOff()==true || Sen[SnSafeDoor8].IsOff()==true);
        bool bAllDoorClosed         =(bFixAreaBigDoorOpen==false && bFixAreaSmallDoorOpen==false && Sen[SnSafeDoor6PosFixPickPlace].IsOn()==true);
        bool bSafeDoor6Closeded     =(bFixAreaBigDoorOpen==false && bFixAreaSmallDoorOpen==false && Sen[SnTriTempSafeDoor6Lock].IsOn()==false && Sen[SnSafeDoor6].IsOn()==true);

        switch(iTask)
        {
            case 1:
                if(IniConfig.bL34_5FixTrayDoorCynAutoOpen==false)
                {
                    if(bFixAreaBigDoorOpen==true ||
                       Sen[SnSafeDoor6PosFixPickPlace].IsOff()==true)           //small Door open  FixPickPlace Door Close
                    {
                        bDelayTimeAfterFixDoorOpen      =true;
                        iStepOfDelayAfterFixDoorOpen    =2;
                        bAnyOpenDoorForceSwitchDryAir[2]=true;
                        iTask=100;
                    }
                    else if(bFixAreaSmallDoorOpen==true && Sen[SnSafeDoor6PosFixPickPlace].IsOn()==true)
                    {
                        bDelayTimeAfterFixDoorOpen      =true;
                        iStepOfDelayAfterFixDoorOpen    =1;
                        bAnyOpenDoorForceSwitchDryAir[2]=true;
                        iTask=10;
                    }
                    else if(Sen[SnTriTempSafeDoor6Lock].IsOn()==true && Sen[SnFix3Lock].IsOn()==true && Sen[SnSafeLock].IsOff()==true)
                    {
                        bDelayTimeAfterFixDoorOpen      =true;
                        iStepOfDelayAfterFixDoorOpen    =11;
                        bAnyOpenDoorForceSwitchDryAir[2]=true;
                        iTask=1100;
                    }
                    else
                    {
                        bAnyOpenDoorForceSwitchDryAir[2]=false;
                    }
                }
                else
                {
                    if(Sen[SnTriTempSafeDoor6Lock].IsOn()==true)
                    {
                        bDelayTimeAfterFixDoorOpen      =true;
                        iStepOfDelayAfterFixDoorOpen    =11;
                        bAnyOpenDoorForceSwitchDryAir[2]=true;
                        iTask=1100;
                    }
                    else if(bFixAreaBigDoorOpen==true)
                    {
                        bDelayTimeAfterFixDoorOpen      =true;
                        iStepOfDelayAfterFixDoorOpen    =11;
                        bAnyOpenDoorForceSwitchDryAir[2]=true;
                        iTask=1100;
                    }
                    else if(bFixAreaSmallDoorOpen==true)
                    {
                        bDelayTimeAfterFixDoorOpen      =true;
                        iStepOfDelayAfterFixDoorOpen    =11;
                        bAnyOpenDoorForceSwitchDryAir[2]=true;
                        iTask=1100;
                    }
                    else
                    {
                        bAnyOpenDoorForceSwitchDryAir[2]=false;
                    }
                }
                break;
            case 10:
                bDelayTimeAfterFixDoorOpen=true;
                if(CheckPositionIsRange(MInShuttle1, Prod.InSHT[0].iRight) ||
                   CheckPositionIsRange(MInShuttle1, Prod.InSHT[0].iLeft))
                {
                    bflag[0]=true;
                }

                if(CheckPositionIsRange(MInShuttle2, Prod.InSHT[1].iRight) ||
                   CheckPositionIsRange(MInShuttle2, Prod.InSHT[1].iLeft))
                {
                    bflag[1]=true;
                }

                if(bflag[0] && bflag[1])
                {
                    if(Cylinder[C_Shuttle1Floodgate].GetOutBit()==false)        //Modify IO Write change
                    {
                       Cylinder[C_Shuttle1Floodgate].On();
                       MOT[MInShuttle1].fCanMove=false;
                    }
                    if(Cylinder[C_Shuttle2Floodgate].GetOutBit()==false)        //Modify IO Write change
                    {
                        Cylinder[C_Shuttle2Floodgate].On();
                        MOT[MInShuttle2].fCanMove=false;
                    }
                }
                iTask=20;
                break;
            case 20:
                fMain->palMainStatus->Caption="Drying Wait";
                if(bAllDoorClosed==true)
                {
                    tFixDoorTimer.SetSecAndOn(iSecOfFixDoorOpenAlarm);
                    iTask=30;
                }
                else if(Sen[SnSafeDoor6PosFixPickPlace].IsOn()==false)
                {
                    iTask=100;
                }
                else if(bFixAreaBigDoorOpen==true)
                {
                    iTask=100;
                }
                break;
            case 30:
                fMain->palMainStatus->Caption="Drying Wait";
                if(bAllDoorClosed==true)
                {
                    if(tFixDoorTimer.Off())
                    {
                        if(Cylinder[C_Shuttle1Floodgate].GetOutBit()==true)     //Modify IO Write change
                        {
                            if(Cylinder[C_Shuttle1Floodgate].Pop())
                            {
                                MOT[MInShuttle1].fCanMove=true;
                                bDoorflag[0]=true;
                            }
                        }
                        else
                        {
                            MOT[MInShuttle1].fCanMove=true;
                            bDoorflag[0]=true;
                        }

                        if(Cylinder[C_Shuttle2Floodgate].GetOutBit()==true)     //Modify IO Write change
                        {
                            if(Cylinder[C_Shuttle2Floodgate].Pop())
                            {
                                MOT[MInShuttle2].fCanMove=true;
                                bDoorflag[1]=true;
                            }
                        }
                        else
                        {
                            MOT[MInShuttle2].fCanMove=true;
                            bDoorflag[1]=true;
                        }

                        if(bDoorflag[0] && bDoorflag[1])
                        {
                            ShowErrorMessage("WAR31010", K_RETRY, MMSystem);
                            iTask=2100;
                        }
                    }
                    else
                    {
                        fMain->lbl_TriTempState->Visible=true;
                        fMain->lbl_TriTempState->Caption=AnsiString("Fix Area Small Door Open");
                    }
                }
                else
                {
                    if(bFixAreaSmallDoorOpen==true)
                    {
                        iStepOfDelayAfterFixDoorOpen=1;
                        iTask=20;
                    }
                    else
                    {
                        iStepOfDelayAfterFixDoorOpen=2;
                        iTask=100;
                    }
                }
                break;
            case 100:
                if(CheckPositionIsRange(MInShuttle1, Prod.InSHT[0].iRight) ||
                   CheckPositionIsRange(MInShuttle1, Prod.InSHT[0].iLeft))
                {
                    bflag[0]=true;
                }

                if(CheckPositionIsRange(MInShuttle2, Prod.InSHT[1].iRight) ||
                   CheckPositionIsRange(MInShuttle2, Prod.InSHT[1].iLeft))
                {
                    bflag[1]=true;
                }

                if(bflag[0] && bflag[1])
                {
                    if(Cylinder[C_Shuttle1Floodgate].GetOutBit()==false)        //Modify IO Write change
                    {
                       Cylinder[C_Shuttle1Floodgate].On();
                       MOT[MInShuttle1].fCanMove=false;
                    }
                    if(Cylinder[C_Shuttle2Floodgate].GetOutBit()==false)        //Modify IO Write change
                    {
                        Cylinder[C_Shuttle2Floodgate].On();
                        MOT[MInShuttle2].fCanMove=false;
                    }
                }
                iTask=200;
                break;
            case 200:
                fMain->palMainStatus->Caption="Drying Wait";
                if(bAllDoorClosed==true)
                {
                    iStepOfDelayAfterFixDoorOpen=3;
                    Cylinder[C_Auto3TrackFloodgate].On();
                    if(IniConfig.iL34_4OpenAuto3TrackGateSec<IniConfig.iL34_2DelaySecOfFixDoorOpen)
                    {
                        iFixDoorOpenTime=IniConfig.iL34_4OpenAuto3TrackGateSec;
                    }
                    else if(IniConfig.iL34_4OpenAuto3TrackGateSec>IniConfig.iL34_2DelaySecOfFixDoorOpen)
                    {
                        iFixDoorOpenTime=IniConfig.iL34_2DelaySecOfFixDoorOpen;
                    }
                    else
                    {
                        iFixDoorOpenTime=IniConfig.iL34_2DelaySecOfFixDoorOpen;
                    }
                    tFixDoorTimer.SetSecAndOn(iFixDoorOpenTime);
                    iTask=300;
                    fMain->lbl_TriTempState->Visible=true;
                }
                else
                {
                    fMain->lbl_TriTempState->Visible=false;
                }
                break;
            case 300:
                fMain->palMainStatus->Caption="Drying Wait";
                if(bAllDoorClosed==true)
                {
                    iStepOfDelayAfterFixDoorOpen=5;
                    if(tFixDoorTimer.Off())
                    {
                        if(IniConfig.dL34_3DewPointOfFixDoorOpen==0.0)
                        {
                            iCountCompare=IniConfig.iL34_4OpenAuto3TrackGateSec+IniConfig.iL34_2DelaySecOfFixDoorOpen+60;
                            tFixDoorTimer.SetSecAndOn(iCountCompare);
                            iTask=400;
                        }
                        else                                                    //check DewPoint
                        {
                            iTask=500;
                        }
                    }
                    else
                    {
                        iTime=iFixDoorOpenTime-tFixDoorTimer.LatchCycleTimeSec();
                        fMain->lbl_TriTempState->Visible=true;
                        fMain->lbl_TriTempState->Caption=AnsiString("WaitDrying \n CountDown :"+AnsiString(iTime)+" (sec)");
                    }
                }
                else
                {
                    iStepOfDelayAfterFixDoorOpen=2;
                   
                    if(bFixAreaSmallDoorOpen==true)
                    {
                        iStepOfDelayAfterFixDoorOpen=1;
                    }
                    else
                    {
                        iStepOfDelayAfterFixDoorOpen=2;
                    }
                    iTask=200;
                }
                break;
            case 400:
                if(bAllDoorClosed==true)
                {
                    fMain->palMainStatus->Caption="Drying Wait";
                    iStepOfDelayAfterFixDoorOpen=8;

                    if((dFixAreaDewPoint<IniConfig.dL34_3DewPointOfFixDoorOpen) ||
                       (bDewPoint==true && dFixAreaDewPoint<=iDewPoint ||
                        dFixAreaDewPoint==-60.0))
                    {
                        iTask=500;
                    }
                    else if(tFixDoorTimer.Off())
                    {
                        if(CUSTOMER_CODE!=CC_SPIL_SHINCHU) // Sipl Not to Jam
                        {
                            ShowErrorMessage("WAR31011", K_RETRY, MMSystem);
                        }

                        iCountCompare=IniConfig.iL34_4OpenAuto3TrackGateSec+IniConfig.iL34_2DelaySecOfFixDoorOpen+60;
                        tFixDoorTimer.SetSecAndOn(iCountCompare);
                        iTask=400;
                    }
                    else
                    {
                        iTime=iFixDoorOpenTime-tFixDoorTimer.LatchCycleTimeSec();
                        fMain->lbl_TriTempState->Visible=true;
                        fMain->lbl_TriTempState->Caption =AnsiString("WaitDrying \n CountDown :"+AnsiString(iTime)+" (sec)");
                    }
                }
                else
                {
                    if(bFixAreaSmallDoorOpen==true)
                    {
                        iStepOfDelayAfterFixDoorOpen=1;
                    }
                    else
                    {
                        iStepOfDelayAfterFixDoorOpen=2;
                    }
                    iTask=200;
                }
                break;
            case 500:
                if(bAllDoorClosed==true)
                {
                    iStepOfDelayAfterFixDoorOpen=7;

                    if(Cylinder[C_Shuttle1Floodgate].GetOutBit()==false)        //Hmy 20180826 Modify IO Write change
                        Cylinder[C_Shuttle1Floodgate].On();
                    if(Cylinder[C_Shuttle2Floodgate].GetOutBit()==false)        //Hmy 20180826 Modify IO Write change
                        Cylinder[C_Shuttle2Floodgate].On();
                    iTask=2100;
                }
                else
                {
                    if(bFixAreaSmallDoorOpen==true)
                    {
                        iStepOfDelayAfterFixDoorOpen=1;
                    }
                    else
                    {
                        iStepOfDelayAfterFixDoorOpen=2;
                    }
                    iTask=200;
                }
                break;
            case 1100:
                if(CheckPositionIsRange(MInShuttle1, Prod.InSHT[0].iRight) ||
                   CheckPositionIsRange(MInShuttle1, Prod.InSHT[0].iLeft))
                {
                    bflag[0]=true;
                }

                if(CheckPositionIsRange(MInShuttle2, Prod.InSHT[1].iRight) ||
                   CheckPositionIsRange(MInShuttle2, Prod.InSHT[1].iLeft))
                {
                    bflag[1]=true;
                }

                if(bflag[0] && bflag[1])
                {
                    if(Cylinder[C_Shuttle1Floodgate].GetOutBit()==false)        //Modify IO Write change
                    {
                       Cylinder[C_Shuttle1Floodgate].On();
                       MOT[MInShuttle1].fCanMove=false;
                    }
                    if(Cylinder[C_Shuttle2Floodgate].GetOutBit()==false)        //Modify IO Write change
                    {
                        Cylinder[C_Shuttle2Floodgate].On();
                        MOT[MInShuttle2].fCanMove=false;
                    }
                }
                else
                {
                    if(Cylinder[C_Shuttle1Floodgate].GetOutBit()==true)     //Modify IO Write change
                    {
                        if(Cylinder[C_Shuttle1Floodgate].Pop())
                        {
                            MOT[MInShuttle1].fCanMove=true;
                            bDoorflag[0]=true;
                        }
                    }
                    else
                    {
                        MOT[MInShuttle1].fCanMove=true;
                        bDoorflag[0]=true;
                    }

                    if(Cylinder[C_Shuttle2Floodgate].GetOutBit()==true)     //Modify IO Write change
                    {
                        if(Cylinder[C_Shuttle2Floodgate].Pop())
                        {
                            MOT[MInShuttle2].fCanMove=true;
                            bDoorflag[1]=true;
                        }
                    }
                    else
                    {
                        MOT[MInShuttle2].fCanMove=true;
                        bDoorflag[1]=true;
                    }

                    if(bDoorflag[0]==false || bDoorflag[1]==false)
                        break;
                }
                if(Sen[SnTriTempSafeDoor6Lock].IsOn()==true)
                {
                    if(Cylinder[C_SafeDoor6Lock].GetOutBit()==false)            //Modify IO Write change
                        Cylinder[C_SafeDoor6Lock].On();
                }
                iTask=1180;
                break;
            case 1180:
                fMain->palMainStatus->Caption="Drying Wait";
                if(Sen[SnSafeDoor6].IsOn()==false)
                {
                    iTask=1200;
                }
                else if(bFixAreaBigDoorOpen==true)
                {
                    iTask=1200;
                }
                else if(bFixAreaSmallDoorOpen==true)
                {
                    iTask=1180;
                }
                else
                {
                    if(Cylinder[C_Shuttle1Floodgate].GetOutBit()==true)     //Modify IO Write change
                    {
                        if(Cylinder[C_Shuttle1Floodgate].Pop())
                        {
                            MOT[MInShuttle1].fCanMove=true;
                            bDoorflag[0]=true;
                        }
                    }
                    else
                    {
                        MOT[MInShuttle1].fCanMove=true;
                        bDoorflag[0]=true;
                    }

                    if(Cylinder[C_Shuttle2Floodgate].GetOutBit()==true)     //Modify IO Write change
                    {
                        if(Cylinder[C_Shuttle2Floodgate].Pop())
                        {
                            MOT[MInShuttle2].fCanMove=true;
                            bDoorflag[1]=true;
                        }
                    }
                    else
                    {
                        MOT[MInShuttle2].fCanMove=true;
                        bDoorflag[1]=true;
                    }
                    if(bDoorflag[0]==false || bDoorflag[1]==false)
                        break;

                    if(Cylinder[C_SafeDoor6Lock].GetOutBit()==true)             //Hmy 20180826 Modify IO Write change
                        Cylinder[C_SafeDoor6Lock].Off();

                    iTask=2100;
                }
                break;
            case 1200:
                fMain->palMainStatus->Caption="Drying Wait";
                if(bSafeDoor6Closeded)
                {
                    Cylinder[C_Auto3TrackFloodgate].On();
                    if(Cylinder[C_SafeDoor6Lock].GetOutBit()==true)             //Door close must output off
                        Cylinder[C_SafeDoor6Lock].Off();
                    if(IniConfig.iL34_4OpenAuto3TrackGateSec<IniConfig.iL34_2DelaySecOfFixDoorOpen)
                    {
                        iFixDoorOpenTime=IniConfig.iL34_4OpenAuto3TrackGateSec;
                    }
                    else if(IniConfig.iL34_4OpenAuto3TrackGateSec>IniConfig.iL34_2DelaySecOfFixDoorOpen)
                    {
                        iFixDoorOpenTime=IniConfig.iL34_2DelaySecOfFixDoorOpen;
                    }
                    else
                    {
                        iFixDoorOpenTime=IniConfig.iL34_2DelaySecOfFixDoorOpen;
                    }
                    fMain->lbl_TriTempState->Visible=true;
                    tFixDoorTimer.SetSecAndOn(iFixDoorOpenTime);
                    iTask=1300;
                }
                else
                {
                    fMain->lbl_TriTempState->Visible=false;
                }
                break;
            case 1300:
                fMain->palMainStatus->Caption="Drying Wait";
                if(bSafeDoor6Closeded)
                {
                    iStepOfDelayAfterFixDoorOpen=5;
                    if(tFixDoorTimer.Off())
                    {
                        if(IniConfig.dL34_3DewPointOfFixDoorOpen==0.0)
                        {
                            iTask=1500;
                        }
                        else //check DewPoint
                        {
                            iCountCompare=IniConfig.iL34_4OpenAuto3TrackGateSec+IniConfig.iL34_2DelaySecOfFixDoorOpen+60;
                            tFixDoorTimer.SetSecAndOn(iCountCompare);
                            iTask=1400;
                        }
                    }
                    else
                    {
                        iTime=iFixDoorOpenTime-tFixDoorTimer.LatchCycleTimeSec();
                        fMain->lbl_TriTempState->Visible=true;
                        fMain->lbl_TriTempState->Caption=AnsiString("WaitDrying \n CountDown :"+AnsiString(iTime)+" (sec)");
                    }
                }
                else
                {
                    if(Sen[SnTriTempSafeDoor6Lock].IsOn()==true)
                    {
                        if(Cylinder[C_SafeDoor6Lock].GetOutBit()==false)  //Modify IO Write change
                            Cylinder[C_SafeDoor6Lock].On();
                    }
                    iStepOfDelayAfterFixDoorOpen=-1;
                    iTask=1200;
                }
                break;
            case 1400:
                if(bSafeDoor6Closeded)
                {
                    fMain->palMainStatus->Caption="Drying Wait";
                    iStepOfDelayAfterFixDoorOpen = 8;
                    if((dFixAreaDewPoint<IniConfig.dL34_3DewPointOfFixDoorOpen) ||
                       (bDewPoint==true && dFixAreaDewPoint<=iDewPoint) ||
                        dFixAreaDewPoint == -60.0)
                    {
                        iTask=1500;
                    }
                    else if(tFixDoorTimer.Off())
                    {
                        if(CUSTOMER_CODE!=CC_SPIL_SHINCHU) // Sipl Not to Jam
                        {
                            ShowErrorMessage("WAR31011", K_RETRY, MMSystem);
                        }

                        iCountCompare=IniConfig.iL34_4OpenAuto3TrackGateSec+IniConfig.iL34_2DelaySecOfFixDoorOpen+60;
                        tFixDoorTimer.SetSecAndOn(iCountCompare);
                        iTask=1400;
                    }
                    else
                    {
                        iTime=iFixDoorOpenTime-tFixDoorTimer.LatchCycleTimeSec();
                        fMain->lbl_TriTempState->Visible=true;
                        fMain->lbl_TriTempState->Caption =AnsiString("WaitDrying \n CountDown :"+AnsiString(iTime)+" (sec)");
                    }

                }
                else// if(Sen[SnSafeDoor6].IsOn()==false || bFixAreaBigDoorOpen ==true || bFixAreaSmallDoorOpen==true)
                {
                    if(Sen[SnTriTempSafeDoor6Lock].IsOn()==true)
                    {
                        if(Cylinder[C_SafeDoor6Lock].GetOutBit()==false)        //Hmy 20180826 Modify IO Write change
                            Cylinder[C_SafeDoor6Lock].On();
                    }
                    iStepOfDelayAfterFixDoorOpen=-1;
                    iTask=1200;
                }
                break;
            case 1500:
                if(bSafeDoor6Closeded)
                {
                    iStepOfDelayAfterFixDoorOpen=7;
                    if(Cylinder[C_Shuttle1Floodgate].GetOutBit()==true)
                        Cylinder[C_Shuttle1Floodgate].Off();
                    if(Cylinder[C_Shuttle2Floodgate].GetOutBit()==true)
                        Cylinder[C_Shuttle2Floodgate].Off();
                    if(Cylinder[C_SafeDoor6Lock].GetOutBit()==true)             //Modify IO Write change
                        Cylinder[C_SafeDoor6Lock].Off();

                    iTask=2100;
                }
                else
                {
                    if(Sen[SnTriTempSafeDoor6Lock].IsOn()==true)
                    {
                        if(Cylinder[C_SafeDoor6Lock].GetOutBit()==false)  //Modify IO Write change
                            Cylinder[C_SafeDoor6Lock].On();
                    }
                    iStepOfDelayAfterFixDoorOpen=-1;
                    iTask=1200;
                }
                break;
            case 2100:
                tFixDoorTimer.SetSecAndOn(iSecOfFixDoorOpenAlarm);
                Cylinder[C_Auto3TrackFloodgate].Off();
                bAnyOpenDoorForceSwitchDryAir[2]=false;
                iStepOfDelayAfterFixDoorOpen=-1;                
                iTask=2200;
            case 2200:
                if(tFixDoorTimer.Off())
                {
                    bDelayTimeAfterFixDoorOpen=false;
                    iTask=1;
                    fMain->lbl_TriTempState->Visible=false;
                }
                break;
        }
    }
    catch(...)
    {
        MyDBIProcess("Exception", "DoDelayTimeAfterFixDoorOpen");
    }

    return false;
}
//==============================================================================
//                              判斷位置是否在範圍內
//==============================================================================
bool CheckPositionIsRange(int iIndex, int iPos)
{
    int iShift=10;
    int iEncoderPos=MOT[iIndex].ReadEncoderPos();

    if((iEncoderPos>iPos+iShift) ||
       (iEncoderPos<iPos-iShift))
        return false;
    else
        return true;
}
//==============================================================================
int eHotTemp=0;
int eLowTemp=1;
int eMidTemp=2;
int eDefrost=3;
int eOpenTime=4;
int eATCTime=5;
int eRunDefrost=6;
int eDefrosting=7;
int eDefrostDone=8;
int eWaitDewPoint=9;
int eChangeTemp=10;
int eATCOffLine=11;
int eHaltDefrost=12;
//==============================================================================
int DoorOpenAlarmForTriTemp(bool bBigDoor, int iTempMode, int iTaskNow, int iTaskNext, int iCountTime=0)    //0:高溫, 1:低溫, 2:介於之間, 3:除霜溫度, 4:除霜時間, 5:關冷媒機, 6:執行除霜, 7:除霜完成
{
    int iLowTemp =IniConfig.iL33_5DoorOpenTempForLowTemp;
    int iHotTemp =IniConfig.iL33_6DoorOpenTempForHotTemp;
    int iRet;
    AnsiString StrC="", StrE="";
    AnsiString Str1=(bBigDoor==true)?"大":"小";

    if(iTempMode==eHotTemp)
    {
        StrC.sprintf("%s門開啟，請立即關上。\n工作溫度設定大於%d度。", Str1, iHotTemp);
        StrE.sprintf("The door is opened, please close it now. \nThe setting of working temperature is greater than %d degree.", iHotTemp);
    }
    else if(iTempMode==eLowTemp)
    {
        StrC.sprintf("%s門開啟，請立即關上。\n工作溫度設定低於%d度。", Str1, iLowTemp);
        StrE.sprintf("The door is opened, please close it now. \nThe setting of working temperature is lower than %d degree.", iLowTemp);
    }
    else if(iTempMode==eMidTemp)
    {
        StrC.sprintf("%s門開啟，請立即關上。\n工作溫度設定介於%d~%d度。", Str1, iLowTemp, iHotTemp);
        StrE.sprintf("The door is opened, please close it now. \nThe setting of working temperature is between %d ~ %d degree.", iLowTemp, iHotTemp);
    }    
    else if(iTempMode==eDefrost)
    {
        StrC.sprintf("%s門開啟，請立即關上。\n工作溫度設定小於%d度。", Str1, IniConfig.iL32_4SetDefrostTemp);
        StrE.sprintf("The door is opened, please close it now. \nThe setting of working temperature is lower than %d degree.", IniConfig.iL32_4SetDefrostTemp);
    }
    else if(iTempMode==eOpenTime)
    {
        StrC.sprintf("%s門開啟，請立即關上。\n工作溫度設定低於%d度。未關閉將於倒數設定時間(%d秒)後強制升溫除霜。", Str1, iLowTemp, iCountTime);
        StrE.sprintf("The door is opened, please close it now. \nThe setting of working temperature is lower than %d degree.\nIf it is not closed, it will count down to set the time(%d second) after forced heating defrost.", iLowTemp, iCountTime);
    }
    else if(iTempMode==eATCTime)
    {
        StrC.sprintf("%s門開啟超過[L33]設定時間，請立即關上。\n未關閉將於%d秒後強制關閉ATC冷媒機並升溫除霜。", Str1, iCountTime);
        StrE.sprintf("The door has been opened more than setting time of [L33], \nplease close it immediately. \nOtherwise after %d seconds later,\nthe ATC refrigerant machine will be forced to shut down \nand rise temperature.", iCountTime);
    }
    else if(iTempMode==eRunDefrost)
    {
        StrC.sprintf("安全門開啟時間超過[L33]設定時間，強制關閉ATC冷媒機並升溫除霜。");
        StrE.sprintf("The door opening time exceeds the seting time of [L33], \nthe ATC refrigerant machine will be forced to close and rise temperature.");
    }
    else if(iTempMode==eDefrosting)
    {
        StrC.sprintf("門開啟時間超過設定時間，強制關閉ATC冷媒機升溫除霜進行中...");
        StrE.sprintf("The door opening time exceeds the set time, \n and the ATC refrigerant machine will be forced to close. \n The heating and defrosting are in progress");
    }
    else if(iTempMode==eDefrostDone)
    {
        StrC.sprintf("升溫除霜完成，請立即關上安全門!");
        StrE.sprintf("The door is opened after heating and defrosting completed, \n please close it immediately");
    }
    else if(iTempMode==eWaitDewPoint)
    {
        StrC.sprintf("升溫除霜完成門已關上，等待露點溫度低於(25度)!");
        StrE.sprintf("After heating up and defrosting, \n the door is closed, wait for the dew point temperature to be lower than \n (25 degrees)");
    }
    else if(iTempMode==eChangeTemp)
    {
        StrC.sprintf("升溫除霜完成測試區門已關上，切換為生產溫度!");
        StrE.sprintf("After the heating and defrosting were completed, \nthe door of the test area is closed, \n and it is switched to the production temperature");
    }
    else if(iTempMode==eATCOffLine)
    {
        StrC.sprintf("升溫除霜完成測試區門未關上，強制執行ATC Off Line。");
        StrE.sprintf("After the heating and defrosting is completed, \n the door of the test area is not closed, and the ATC Off Line is enforced.");
    }
    else if(iTempMode==eHaltDefrost)
    {
        StrC.sprintf("升溫除霜中斷，門已關上。露點溫度高於25度、強制執行ATC Off Line");
        StrE.sprintf("The warm-up defrost was interrupted and the door was closed. \n When the dew point temperature is higher than 25 degrees, \n the ATC Off Line is enforced");
    }    

    if(LastSet.iLanguageCountry==1)
        ShowNoteMessage_1032(StrC);
    else
        ShowNoteMessage_1032(StrE);

    Str1.sprintf("%d, %d, %s", iTaskNow, iTaskNext, StringReplace(StrE, "\n", "", TReplaceFlags()<<rfReplaceAll));
    fMain->slTriTempDoorlog->AddTextWithDateTime(Str1);        

    if(bBigDoor==true)
        iRet=ShowErrorMessage("WAR31012", K_RETRY, MMSystem);
    else
        iRet=ShowErrorMessage("WAR31013", K_RETRY, MMSystem);
    return iRet;
}
//==============================================================================
TQPF_Timer tBDoorTimer;
TQPF_Timer tSDoorTimer;
TQPF_Timer tOpenDoorTimer;                                                      //紀錄開門的總時間
TQPF_Timer tDefrost2MTimer;                                                     //計時兩分鐘
TQPF_Timer tDefrost1HTimer;                                                     //計時一小時
TQPF_Timer tChkDoorTimer; 
bool __fastcall CheckDoorOpenForTriTemp()
{
    try
    {
        AnsiString asSelectDeviceName;
        #ifdef SOFT_SIMULTE
            int iATC_On=1;
        #else
            int iATC_On=(iATCOnLine==true)?1:0;
        #endif
        int iWorkTemp=(LastSet.iTemperature==Tempture_Ambient)?26:Temperature.fWorkTemperBase;
        int iBDoorAlarmTime             =10;
        int iSDoorAlarmTime             =20;

        static int iTask=1;                                                     //InArmAndIndex開門警報
        static int iBDoorOpenTime       =0;                                     //大門
        static int iSDoorOpenTime       =0;                                     //小門
        static int iHotTemp             =IniConfig.iL33_6DoorOpenTempForHotTemp;
        static int iCurrTemp            =-999;                                  //Modify 0 ->-999(工作檔為0度，重開軟體判斷異常)
        static int iCheckBDoorOpenTime  =90;                                    //Modify 0 ->90(工作檔為0度，重開軟體判斷異常)
        static int iCheckSDoorOpenTime  =120;                                   //Modify 0 ->120(工作檔為0度，重開軟體判斷異常)
        static bool bBDoorOpen          =false;
        static bool bSDoorOpen          =false;                                 //add 小門報警時間設定
        static int  iDoorOpen           =0;
        static bool bSafeDoorHatchway[MAX_HATCH_DOOR_CNT]={true, true, true, true, true, true, true, true, true, true, true, true};

        if((iCurrTemp!=iWorkTemp) || bChangeCheckDoorCheckTime==true)           //增加門檢查時間設定兩組(25度以上、低於25)
        {
            iTask=1;
            iCurrTemp=iWorkTemp;
            iCheckBDoorOpenTime=IniConfig.iL33_3BDoorOpenTimeForLowTemp;
            iCheckSDoorOpenTime=IniConfig.iL33_4SDoorOpenTimeForLowTemp;
            bChangeCheckDoorCheckTime=false;
        }

        for(int i=0; i<4; i++)                                                  //小門及大門
        {
            if(Sen[iSafeDoor[i]].Enable==true &&
               Sen[iSafeDoor[i]].IsOff()==true)
            {
                if(bBDoorOpen==false)
                {
                    tBDoorTimer.SetSecAndOn(iBDoorAlarmTime);
                    bAnyOpenDoorForceSwitchDryAir[0]=true;
                    bBDoorOpen=true;
                    break;
                }
                else
                {
                    break;
                }
            }
            else if(i==4-1)
            {
                if(bSDoorOpen==false)
                {
                    bAnyOpenDoorForceSwitchDryAir[0]=false;
                }
                bBDoorOpen=false;
            }
        }

        for(int i=0; i<MAX_HATCH_DOOR_CNT; i++)
        {
            if(Sen[iSafeDoorHatchway[i]].Enable==true &&
               Sen[iSafeDoorHatchway[i]].IsOff()==true)
            {
                if(iCheckEmptyTrayHasIc!=0)                                     //Add Loader Skip, Only Use And Pass SnSafeDoor1Hatchway
                {
                    if(i==0)
                    {

                    }
                    else
                    {
                        if(bSafeDoorHatchway[i]==true)
                        {
                            bSafeDoorHatchway[i]=false;
                        }
                    }
                }
                else
                {
                    if(bSafeDoorHatchway[i]==true)
                    {
                        bSafeDoorHatchway[i]=false;
                    }
                }
            }
            else
            {
                bSafeDoorHatchway[i]=true;
            }
        }

        for(int i=0; i<MAX_HATCH_DOOR_CNT; i++)
        {
            if(bSafeDoorHatchway[i]==false)
            {
                if(bSDoorOpen==false)
                {
                    tSDoorTimer.SetSecAndOn(iSDoorAlarmTime);
                    bAnyOpenDoorForceSwitchDryAir[0]=true;
                    bSDoorOpen=true;
                    break;
                }
                else
                {
                    break;
                }
            }
            else if(i==MAX_HATCH_DOOR_CNT-1)
            {
                if(bBDoorOpen==false)
                {
                    bAnyOpenDoorForceSwitchDryAir[0]=false;
                }
                bSDoorOpen=false;
            }
        }

        if(bBDoorOpen==true || bSDoorOpen==true)
        {
            if(iDoorOpen==0)
            {
                iDoorOpen=1;
                tOpenDoorTimer.LatchCycleTimeSec(true);                         //紀錄開門的總時間
            }
            bInuptDoorOpend=true;
        }
        else
        {
            iDoorOpen=0;
            bInuptDoorOpend=false;
        }

        if(bBDoorOpen==true && bSDoorOpen==true)                                //yunghsin 20200324 大門跟小門同時開啟以大門為主
        {
            bBDoorOpen=true;
            bSDoorOpen=false;
        }

        switch(iTask)
        {
            case 1:
                if(bBDoorOpen==false && bSDoorOpen==false)
                    return false;

                tBDoorTimer.SetSecAndOn(iBDoorAlarmTime);                       //避免大小門開來開去, 所以同時重置
                tSDoorTimer.SetSecAndOn(iSDoorAlarmTime);

                if(iATC_On==0)
                {
                    DoorOpenAlarmForTriTemp(bBDoorOpen, eHotTemp, 1, 10);
                    iTask=10;
                }
                else if(iWorkTemp>=iHotTemp)                                    //變更判斷溫度 26 ->iLowTemp
                {
                    DoorOpenAlarmForTriTemp(bBDoorOpen, eHotTemp, 1, 50);
                    iTask=50;
                }
                else if(iWorkTemp<=IniConfig.iL32_4SetDefrostTemp &&            //Hmy 20200309 變更判斷溫度 26 ->iLowTemp
                        IniConfig.bL32_1ManuDefrost==true)
                {
                    DoorOpenAlarmForTriTemp(bBDoorOpen, eDefrost, 1, 50);
                    iTask=50;
                }
                else
                {
                    if(IniConfig.bL33_2DoorOpenRunDefrost==false)
                    {
                        DoorOpenAlarmForTriTemp(bBDoorOpen, eLowTemp, 1, 60);
                        iTask=60;
                    }
                    else
                    {
                        if(bBDoorOpen)
                            DoorOpenAlarmForTriTemp(true, eOpenTime, 1, 100, iCheckBDoorOpenTime);
                        else
                            DoorOpenAlarmForTriTemp(false, eOpenTime, 1, 100, iCheckSDoorOpenTime);
                        iTask=100;
                    }
                }
                break;
            case 10:
                if(bBDoorOpen==true)
                {
                    if(tBDoorTimer.Off())
                    {
                        tBDoorTimer.SetSecAndOn(iBDoorAlarmTime);
                        DoorOpenAlarmForTriTemp(true, eHotTemp, 10, 10);
                    }
                }

                if(bSDoorOpen==true)
                {
                    if(tSDoorTimer.Off())
                    {
                        tSDoorTimer.SetSecAndOn(iSDoorAlarmTime);
                        DoorOpenAlarmForTriTemp(false, eHotTemp, 10, 10);
                    }
                }

                if(iATCOnLine==1 ||
                   (bBDoorOpen==false && bSDoorOpen==false))
                {
                    if(iATCOnLine==1)
                        fMain->slTriTempDoorlog->AddTextWithDateTime("10, 1, ATC OnLine");
                    else
                        fMain->slTriTempDoorlog->AddTextWithDateTime("10, 1, Door closed");
                    iTask=1;
                }
                break;
            case 20:
                if(bBDoorOpen==true)
                {
                    if(tBDoorTimer.Off())
                    {
                        tBDoorTimer.SetSecAndOn(iBDoorAlarmTime);
                        DoorOpenAlarmForTriTemp(true, eLowTemp, 20, 20);
                    }
                }
                if(bSDoorOpen==true)
                {
                    if(tSDoorTimer.Off())
                    {
                        tSDoorTimer.SetSecAndOn(iSDoorAlarmTime);
                        DoorOpenAlarmForTriTemp(false, eLowTemp, 20, 20);
                    }
                }

                if(iATCOnLine==1 ||
                   (bBDoorOpen==false && bSDoorOpen==false))
                {
                    if(iATCOnLine==1)
                        fMain->slTriTempDoorlog->AddTextWithDateTime("20, 1, ATC OnLine");
                    else
                        fMain->slTriTempDoorlog->AddTextWithDateTime("20, 1, Door closed");
                    iTask=1;
                }
                break;
            case 50:
                if(bBDoorOpen==true)
                {
                    if(tBDoorTimer.Off())
                    {
                        tBDoorTimer.SetSecAndOn(iBDoorAlarmTime);
                        DoorOpenAlarmForTriTemp(true, eHotTemp, 50, 50);
                    }
                }
                if(bSDoorOpen==true)
                {
                    if(tSDoorTimer.Off())
                    {
                        tSDoorTimer.SetSecAndOn(iSDoorAlarmTime);
                        DoorOpenAlarmForTriTemp(false, eHotTemp, 50, 50);
                    }
                }
                if(iATCOnLine==0 ||
                   (bBDoorOpen==false && bSDoorOpen==false))
                {
                    if(iATCOnLine==0)
                        fMain->slTriTempDoorlog->AddTextWithDateTime("50, 1, ATC OffLine");
                    else
                        fMain->slTriTempDoorlog->AddTextWithDateTime("50, 1, Door closed");

                    ShowMyMessage("Please Manually Defrost");
                    iTask=1;
                }
                break;
            case 60:
                if(bBDoorOpen==true)
                {
                    if(tBDoorTimer.Off())
                    {
                        tBDoorTimer.SetSecAndOn(iBDoorAlarmTime);
                        DoorOpenAlarmForTriTemp(true, eLowTemp, 60, 60);
                    }
                }
                if(bSDoorOpen==true)
                {
                    if(tSDoorTimer.Off())
                    {
                        tSDoorTimer.SetSecAndOn(iSDoorAlarmTime);
                        DoorOpenAlarmForTriTemp(false, eLowTemp, 60, 60);
                    }
                }

                if(iATCOnLine==0 ||
                   (bBDoorOpen==false && bSDoorOpen==false))
                {
                    if(iATCOnLine==0)
                        fMain->slTriTempDoorlog->AddTextWithDateTime("60, 1, ATC OffLine");
                    else
                        fMain->slTriTempDoorlog->AddTextWithDateTime("60, 1, Door closed");                     
                    iTask=1;
                }
                break;
            case 100:
                if(bBDoorOpen==true)
                {
                    iBDoorOpenTime=iCheckBDoorOpenTime-tOpenDoorTimer.LatchCycleTimeSec();
                    if(iBDoorOpenTime<1)
                    {
                        iTask=200;
                        fMain->slTriTempDoorlog->AddTextWithDateTime("100, 200, Door opened run defrost");
                    }
                    else if(tBDoorTimer.Off())
                    {
                        tBDoorTimer.SetSecAndOn(iBDoorAlarmTime);
                        DoorOpenAlarmForTriTemp(true, eATCTime, iBDoorOpenTime, 100, 100);
                    }
                }

                if(bSDoorOpen==true)
                {
                    iSDoorOpenTime=iCheckSDoorOpenTime-tOpenDoorTimer.LatchCycleTimeSec();                
                    if(iSDoorOpenTime<1)
                    {
                        iTask=200;
                        fMain->slTriTempDoorlog->AddTextWithDateTime("100, 200, Door opened run defrost");
                    }
                    else if(tSDoorTimer.Off())
                    {
                        tSDoorTimer.SetSecAndOn(iSDoorAlarmTime);
                        DoorOpenAlarmForTriTemp(false, eATCTime, iSDoorOpenTime, 100, 100);
                    }
                }

                if(iATCOnLine==0 ||
                   (bBDoorOpen==false && bSDoorOpen==false))
                {
                    if(iATCOnLine==0)
                        fMain->slTriTempDoorlog->AddTextWithDateTime("100, 160, ATC OffLine delay 20Sec.");
                    else
                        fMain->slTriTempDoorlog->AddTextWithDateTime("100, 160, Door closed delay 20Sec.");                  
                    tOpenDoorTimer.SetSecAndOn(20);
                    iTask=160;
                }
                break;
            case 160:
                if(bBDoorOpen==false && bSDoorOpen==false)
                {
                    if(tOpenDoorTimer.Off())
                    {
                        fMain->slTriTempDoorlog->AddTextWithDateTime("160, 1, Delay 20Sec done");
                        iTask=1;
                    }
                }
                else
                {
                    fMain->slTriTempDoorlog->AddTextWithDateTime("160, 100, Door Opened");
                    iTask=100;
                }
                break;
            case 200:
                DoorOpenAlarmForTriTemp(bBDoorOpen, eRunDefrost, 200, 300);

                fTemp_Set->gbDefrostFunction->Enabled=false;
                bDefrostKeepATCTemp=true;
                fDefrostNote->sShowMessage="Open the door too long \n and must defrost";
                fDefrostNote->Show();

                if(bCheckUse_AirStream==true)
                {
                    fLotInfo->btnAirStreamOnOff->Enabled=false;
                }
                tDefrost2MTimer.SetSecAndOn(120);                               //計時兩分鐘
                tDefrost1HTimer.SetSecAndOn(3600);                              //計時一小時
                iTask=300;
                break;
            case 300:                                                           //等待機台除霜完成
                if(bDefrostKeepATCTemp==false)
                {
                    tChkDoorTimer.SetSecAndOn(1);
                    iTask=500;
                }
                if(fDefrostNote->bInterruptDefrost==true)
                {
                    iTask = 1;
                }
                break;
            case 500:                                                           //除霜完成，再次檢查們狀態後，自動降溫。
                if(bBDoorOpen==true || bSDoorOpen==true)                        //door open
                {
                    if(tOpenDoorTimer.Off())                                    //Add check Heating over time
                    {
                        if(CUSTOMER_CODE!=CC_SPIL_SHINCHU)                      //Sipl Not to Jam
                            DoorOpenAlarmForTriTemp(bBDoorOpen, eDefrostDone, 500, 1);
                        else
                            fMain->slTriTempDoorlog->AddTextWithDateTime("500, 1, Defrost Done and Door Open");

                        DoorOpenAlarmForTriTemp(bBDoorOpen, eATCOffLine, 500, 1);
                        iTask=1;
                    }
                }
                else
                {
                    fMain->slTriTempDoorlog->AddTextWithDateTime("500, 600, Defrost Done and Door Close Wait DewPoint");
                    tChkDoorTimer.LatchCycleTimeSec(60);
                    iTask=600;                    
                }
                break;
            case 600:                                                           //除霜完成，檢查DEWPOINT
                if(bBDoorOpen==false &&  bSDoorOpen==false)                     //Door Closed
                {
                    if(dFixAreaDewPoint<25.0)                                   //Hmy 20200831 Modify 自動除霜後，修正露點溫度判斷(-40 ->25)
                    {
                        if(bCheckUse_AirStream==true)
                        {
                            fLotInfo->btnAirStreamOnOff->Enabled=true;
                        }

                        fMain->ChangeATCSiteUse();
                        if(bDefrostNeedSendCommandToAtc==true)                  //Hmy 20221016 Modify //Hmy 20220805 Add DEFROST Command
                        {
                            ATC_InterfaceForm->Send_AtcDefrostStatus(0);        //Hmy 20220805 Add DEFROST Command
                        }
                        fTemp_Set->gbDefrostFunction->Enabled=true;             //Hmy 20200204 執行過自動除霜後解除不能執行手動除霜問題
                        DoorOpenAlarmForTriTemp(bBDoorOpen, eChangeTemp, 600, 1);
                        iTask=1;
                    }
                    else
                    {
                        if(tChkDoorTimer.Off())                                 //Hmy 20190617 Add check Heating over time
                        {
                            if(CUSTOMER_CODE!=CC_SPIL_SHINCHU) //Hmy 20181128 Sipl Not to Jam
                                DoorOpenAlarmForTriTemp(bBDoorOpen, eWaitDewPoint, 600, 600);
                            else
                                fMain->slTriTempDoorlog->AddTextWithDateTime("600, 600, Defrost Done and Door Close Wait DewPoint");
                            tChkDoorTimer.LatchCycleTimeSec(60);
                            iTask=600;
                        }
                    }
                }
                else if(bBDoorOpen==true || bSDoorOpen==true)                   //door open
                {
                    if(tChkDoorTimer.Off())                                     //Hmy 20190617 Add check Heating over time
                    {
                        if(CUSTOMER_CODE!=CC_SPIL_SHINCHU)                      //Hmy 20181128 Sipl Not to Jam
                            DoorOpenAlarmForTriTemp(bBDoorOpen, eDefrostDone, 600, 1100);
                        else
                            fMain->slTriTempDoorlog->AddTextWithDateTime("600, 1100, Defrost Done and Door Open");

                        tChkDoorTimer.SetSecAndOn(1);
                        iTask=500;
                    }
                }
                break;
            case 1100:                                                          //除霜完成，再次檢查們狀態後，自動降溫。
                if(bBDoorOpen==true || bSDoorOpen==true)                        //door open
                {
                    if(tChkDoorTimer.Off())                                     //Hmy 20190617 Add check Heating over time
                    {
                        if(CUSTOMER_CODE!=CC_SPIL_SHINCHU)                      //Hmy 20181128 Sipl Not to Jam
                            DoorOpenAlarmForTriTemp(bBDoorOpen, eDefrostDone, 1100, 1200);
                        else
                            fMain->slTriTempDoorlog->AddTextWithDateTime("1100, 1200, Defrost Done and Door Open");
                            
                        iTask=1200;
                    }
                }
                else
                {
                    tChkDoorTimer.LatchCycleTimeSec(60);
                    fMain->slTriTempDoorlog->AddTextWithDateTime("1100, 600, Defrost Done and Door Close Wait DewPoint");
                    iTask=600;
                }
                break;
            case 1200:
                tBDoorTimer.LatchCycleTimeSec(true);
                fMain->slTriTempDoorlog->AddTextWithDateTime("1200, 1300, Defrost Done and Door Open Turn Off Refrigerator");
                tChkDoorTimer.LatchCycleTimeSec(20);
                fTemp_Set->ATC_Online_Or_offline();                
                iTask=1300;
                break;
            case 1300:
                if(tChkDoorTimer.Off())
                {
                    if(bCheckUse_AirStream==true)
                    {
                        fLotInfo->btnAirStreamOnOff->Enabled=true;
                    }
//                    bDockingAreaOpen_FunctionCheckStart     =false;
                    fTemp_Set->gbDefrostFunction->Enabled   =true;              //Hmy 20200204 執行過自動除霜後解除不能執行手動除霜問題

                    fTemp_Set->ATC_Online_Or_offline();
                    DoorOpenAlarmForTriTemp(bBDoorOpen, eATCOffLine, 1300, 1);
                    iTask=1;
                }
                break;
            case 2100:
                if(bBDoorOpen==true || bSDoorOpen==true)                        //door open
                {
                    fTemp_Set->ATC_Online_Or_offline();
                    tChkDoorTimer.LatchCycleTimeSec(20);
                    fMain->slTriTempDoorlog->AddTextWithDateTime("2100, 2200, Halt Defrost and Door Open ATC Off-Line");
                    iTask=2200;
                }
                else
                {
                    if(dFixAreaDewPoint>25)                                     //Hmy 20200831 Modify 自動除霜後，修正露點溫度判斷(-40 ->25)
                    {
                        tBDoorTimer.LatchCycleTimeSec(true);
                        fTemp_Set->ATC_Online_Or_offline();
                        DoorOpenAlarmForTriTemp(bBDoorOpen, eHaltDefrost, 2100, 2200);
                        tChkDoorTimer.LatchCycleTimeSec(20);
                        iTask=2200;
                    }
                    else
                    {
                        fMain->ChangeATCSiteUse();
                        fTemp_Set->gbDefrostFunction->Enabled=true;             //Hmy 20200204 執行過自動除霜後解除不能執行手動除霜問題
                        DoorOpenAlarmForTriTemp(bBDoorOpen, eChangeTemp, 2100, 1);
                        iTask=1;
                    }
                }
            case 2200:
                if(tChkDoorTimer.Off())
                {
                    if(bCheckUse_AirStream==true)
                    {
                        fLotInfo->btnAirStreamOnOff->Enabled=true;
                    }
                    fTemp_Set->ATC_Online_Or_offline();
                    fTemp_Set->gbDefrostFunction->Enabled   =true;              //Hmy 20200204 執行過自動除霜後解除不能執行手動除霜問題
                    DoorOpenAlarmForTriTemp(bBDoorOpen, eHaltDefrost, 2200, 1);
                    iTask=1;
                }
                break;
        }
    }
    catch(...)
    {
        MyDBIProcess("Exception", "CheckDoorOpenForTriTemp");
    }
    return false;
}
//==============================================================================
void DoTriTempState_AlwaysCheck()                                               //Hmy 20190115 Add alway check tri temp satat
{
    try    
    {
        static bool bFirstRun=true;                                             //TriTemp主要流程
        static int iFirstRunCount=0;

        if(bFirstRun==true)
        {
            iFirstRunCount++;
            if(iFirstRunCount>=1500)
            {
                bFirstRun=false;
            }
            else
            {
                return;
            }
        }
        if(Tri_Temp_Machine!=1 || InitialOK==false)
            return;

        if(IniConfig.bC20_4IndexUseForstSensor)
        {
            fCheckIndexArmFrostSensor();      
        }

        if(IniConfig.bL35_1OverSetTempOpenFan==true)
        {
            if(fiosetview->fShow==false)                                        //Hmy 20190322 Maintenance of test personnel
            {
                fHighTemperatureExhaustAir();                                   //Hmy 20180906 High Temperature Must Exhaust Air(Use Fan)
            }
        }

        if(IniConfig.bL39_1AutoRunWhenTempOk==true)                               //Hmy 20191225 Add Automatic operation after waiting for the temperature Reached
        {
            fCheckATCTempOK();
        }

        if(IniConfig.bL32_2AutoDefrostFunction==true)                           //Hmy 20191225 Add LowTemperature Over Set Time Alarm
        {
            fCheckLowTempExecTime(); 
        }

        if(IniConfig.bL33_1CheckDoorOpenForTriTemp==true)                       //20190625 將溫度與濕度分開判斷(濕度分開) add//Hmy 20170202 add 三溫機 艙體門開啟一段時間必須警報->//檢查門開啟大於設定時間必須報警與ATC斷線
        {
            CheckDoorOpenForTriTemp(); 
        }
        fCheckIonBarAirStatus();
        fCheckDryAirStatus();
        Check_ALLTempFirstReady();
        Check_TriTemp_OverRang();
        Check_AirVolume_Lmt();
    }                                  
    catch(...)
    {
        MyDBIProcess("Exception", "DoTriTempState_AlwaysCheck");
    }
}
//==============================================================================
TQPF_Timer tCheckDryAirTimer;
TQPF_Timer tChkTesterDryAirTimer;
//==============================================================================
void fCheckDryAirStatus()
{
    int iWorkTemp=(LastSet.iTemperature==Tempture_Ambient)?25:Temperature.fWorkTemperBase;
    AnsiString sValue[3]={fLotInfo->pnl_DewPoint_InArm->Caption,
                          fLotInfo->pnl_DewPoint_Index->Caption,
                          fLotInfo->pnl_DewPoint_OutArm->Caption};

    static bool bTimerStart=false;
    TQPF_Timer CheckInterval;

    if(ATC_InterfaceForm->IsConnect()==false)
        return;

    if(Sen[SnDryAirIsEnough].Enable)
    {
        if(tCheckDryAirTimer.Off())
        {
            if(Sen[SnDryAirIsEnough].IsOff())                                   //檢查Dew Point Detect Sensor
            {
                if(USE_NOVX3360==true && Tri_Temp_Machine==1)                   //Jeffrey 20180821 add for Ion Bar Auto Calibration
                {
                    if(bIonBarAirOffEcho==false)
                    {
                        SendCommand_ESD(ESD_IONBAR_AirOff);
                        bIonBarAirOnEcho=false;
                    }
                }
                ShowErrorMessage("WAR31014", 0, MMSystem);
                tCheckDryAirTimer.SetSecAndOn(10);                              //10秒叫一次
            }
            else if(Sen[SnDryAirIsEnough].IsOn())
            {
                if(USE_NOVX3360==true && Tri_Temp_Machine==1)                   //Jeffrey 20180821 add for Ion Bar Auto Calibration
                {
                    if(bIonBarAirOnEcho==false)
                    {
                        SendCommand_ESD(ESD_IONBAR_AirOn);
                        bIonBarAirOffEcho=false;
                    }
                }
                tCheckDryAirTimer.SetSecAndOn(1);                               //1秒檢查一次
            }
        }
    }

    if(Temperature.bEnableTesterDryAirControl==true)                            //yunghsin 20190812 Add Tester Dry Air Control
    {
        if(SW[SwTesterDryAirSwitch].Status()==false)
        {
            SW[SwTesterDryAirSwitch].On();
            tChkTesterDryAirTimer.SetSecAndOn(1);
        }

        if(Sen[SnTesterDryAir].Enable)
        {
            if(tChkTesterDryAirTimer.Off())
            {
                if(Sen[SnTesterDryAir].IsOff())                                 //檢查Dew Point Detect Sensor
                {
                    ShowErrorMessage("WAR31015", 0, MMSystem);
                    tChkTesterDryAirTimer.SetSecAndOn(10);                      //10秒叫一次
                }
                else if(Sen[SnTesterDryAir].IsOn())
                {
                    tChkTesterDryAirTimer.SetSecAndOn(1);                       //1秒檢查一次
                }
            }
        }
    }
    else
    {
        if(SW[SwTesterDryAirSwitch].Status()==true)
        {
            SW[SwTesterDryAirSwitch].Off();
        }
    }

    if(bTimerStart==true)
    {
        bTimerStart=false;
        CheckInterval.SetSecAndOn(1);
    }

    if(CheckInterval.Off())
    {
        bTimerStart=true;
        if(IniConfig.bC20_1EnableEnergySavingDryAir==true)
        {
            if(iWorkTemp<IniConfig.iC20_2LowTempOffEnergySaving)                    //Add check Low Temperature Not Use EnergySaving
            {
                if(fiosetview->fShow==false)
                {
                    if(SW[SwDryAirSwitch].Status()==true) //modify  true ->false  Off->On
                        SW[SwDryAirSwitch].Off();
                }
                return;
            }
            else if(iWorkTemp>IniConfig.iC20_3TempOverUseEnergySaving)
            {
                if(fiosetview->fShow==false)
                {
                    if(SW[SwDryAirSwitch].Status()==false)
                        SW[SwDryAirSwitch].On();
                }
                return;
            }
            else
            {
                if(bAnyOpenDoorForceSwitchDryAir[0]==true ||
                   bAnyOpenDoorForceSwitchDryAir[1]==true ||
                   bAnyOpenDoorForceSwitchDryAir[2]==true ||
                   bAnyOpenDoorForceSwitchDryAir[3]==true)
                {
                    if(fiosetview->fShow==false)
                    {
                        if(SW[SwDryAirSwitch].Status()==true)                       //modify  true ->false  Off->On
                            SW[SwDryAirSwitch].Off();
                    }
                    return;
                }
            }
        }
        else
        {
            if(SW[SwDryAirSwitch].Status()==true)                                   //modify  true ->false  Off->On
            {
                if(fiosetview->fShow==false)
                    SW[SwDryAirSwitch].On();
                else
                    SW[SwDryAirSwitch].Off();                
            }
            return;
        }
    }
}
//==============================================================================
TQPF_Timer tCheckIndexDPTimer[2];
void fCheckIndexArmFrostSensor()                                                //Hmy 20170510 add check DewPointMeter
{
    int iIndexDPSensor[2]={SnDewPointDetectIndexArm1, SnDewPointDetectIndexArm2};
    AnsiString AlarmCode[2]={"WAR31016", "WAR31017"};

    for(int i=0; i<2; i++)
    {
        if(Sen[iIndexDPSensor[i]].Enable)
        {
            if(tCheckIndexDPTimer[i].Off())
            {
                if(Sen[iIndexDPSensor[i]].IsOff())                              //檢查Dew Point Detect Sensor
                {
                    ShowErrorMessage(AlarmCode[i], 0, MMSystem);
                    tCheckIndexDPTimer[i].SetSecAndOn(10);                      //10秒叫一次                    
                }
                else if(Sen[iIndexDPSensor[i]].IsOn())
                {
                    tCheckIndexDPTimer[i].SetSecAndOn(1);                       //1秒檢查一次
                }
            }
        }
    }
}
//==============================================================================
TQPF_Timer tCheckIonBarTimer[2];
void fCheckIonBarAirStatus()
{
    int iIonBarAirSensor[2]={SnIonBarInAirIsEnough, SnIonBarOutAirIsEnough};
    AnsiString AlarmCode[2]={"WAR31018", "WAR31019"};

    if(ATC_InterfaceForm->IsConnect()==false)                                   //檢查 IonBar
        return;

    for(int i=0; i<2; i++)
    {
        if(Sen[iIonBarAirSensor[i]].Enable)
        {
            if(tCheckIonBarTimer[i].Off())
            {
                if(Sen[iIonBarAirSensor[i]].IsOff())                            //檢查Dew Point Detect Sensor
                {
                    ShowErrorMessage(AlarmCode[i], 0, MMSystem);
                    tCheckIonBarTimer[i].SetSecAndOn(10);                       //10秒叫一次
                }
                else if(Sen[iIonBarAirSensor[i]].IsOn())
                {
                    tCheckIonBarTimer[i].SetSecAndOn(1);                        //1秒檢查一次
                }
            }
        }
    }
}
//==============================================================================
TQPF_Timer CheckATCTempTimer;
//==============================================================================
bool __fastcall fCheckATCTempOK()                                               //Hmy 20191224  Add Wait Temperature Stable Time
{
    try
    {
        TPanel *pnFirstTemp_Head40[] = { fTemperFrom->hlTempAa1, fTemperFrom->hlTempAb1, fTemperFrom->hlTempAc1, fTemperFrom->hlTempAd1,
                                         fTemperFrom->hlTempBa1, fTemperFrom->hlTempBb1, fTemperFrom->hlTempBc1, fTemperFrom->hlTempBd1,
                                         fTemperFrom->hlTempAe1, fTemperFrom->hlTempAf1, fTemperFrom->hlTempAg1, fTemperFrom->hlTempAh1,
                                         fTemperFrom->hlTempBe1, fTemperFrom->hlTempBf1, fTemperFrom->hlTempBg1, fTemperFrom->hlTempBh1,
                                         fTemperFrom->hlTempAa2, fTemperFrom->hlTempAb2, fTemperFrom->hlTempAc2, fTemperFrom->hlTempAd2,
                                         fTemperFrom->hlTempBa2, fTemperFrom->hlTempBb2, fTemperFrom->hlTempBc2, fTemperFrom->hlTempBd2,
                                         fTemperFrom->hlTempAe2, fTemperFrom->hlTempAf2, fTemperFrom->hlTempAg2, fTemperFrom->hlTempAh2,
                                         fTemperFrom->hlTempBe2, fTemperFrom->hlTempBf2, fTemperFrom->hlTempBg2, fTemperFrom->hlTempBh2,
                                         fTemperFrom->hlTempShuttle1, fTemperFrom->hlTempShuttle1_2, fTemperFrom->hlTempPlate1, fTemperFrom->hlTempPlate1_2,
                                         fTemperFrom->hlTempShuttle2, fTemperFrom->hlTempShuttle2_2, fTemperFrom->hlTempPlate2, fTemperFrom->hlTempPlate2_2};

        int iStableTimeSec=IniConfig.iL39_2WaitTempstabilize;                  //檢查機台溫度
        int  iCurrSec;
        double iWorkTemp=(LastSet.iTemperature==Tempture_Ambient)?26:Temperature.fWorkTemperBase;
        double dTemp;
        bool bAllTempOk=false;
        bool bReturnStatus=false;
        static double dTargetTemp=-9999.0;

        if(Tri_Temp_Machine!=1)
            return false;

        if(CheckATCTempTimer.Off())
        {
            CheckATCTempTimer.SetSecAndOn(0.5);
        }
        else
        {
            return false;
        }

        if(iATCOnLine==0)
        {
            #ifndef SOFT_SIMULTE
                bAllPosTempOkOverTime= true;
                bAllPosTempInRange   = true;
                bReturnStatus=true;   //Tset Finction Use
                return bReturnStatus;
            #else
                bAllPosTempInRange   = false;
                bAllPosTempOkOverTime= false;
                bReturnStatus=true;
                if(bAllPosTempInRange==false && bAllPosTempOkOverTime==false)
                    return bReturnStatus;
            #endif
        }

        if(dTargetTemp!=iWorkTemp || bReChkTempInRange==true)
        {
            bReChkTempInRange       =false;
            dTargetTemp             =iWorkTemp;
            bAllPosTempOkOverTime   =false;
            bAllPosTempInRange      =false;

            for(int i=0; i<ATC_MAX_SITE; i++)
            {
                pnFirstTemp_Head40[i]->Hint="";
            }
        }
        else
        {
            if(IniConfig.iL39_2WaitTempstabilize==0)
                bAllPosTempOkOverTime=true;

            if(IniConfig.bL39_2WaitTempstabilize==false)                      //Add Wait Temperature Stable Time
            {
                if(bAllPosTempInRange==true)
                {
                    bReturnStatus=true;
                    return bReturnStatus;
                }
            }
            else
            {
                if(bAllPosTempOkOverTime==true && bAllPosTempInRange==true)
                {
                    bReturnStatus=true;
                    return bReturnStatus;
                }
            }
        }

        bAllTempOk=true;
        for(int i=0; i<ATC_MAX_SITE; i++)
        {
            if(bATC_EnablesChannel[i]==true)
            {
                if(pnFirstTemp_Head40[i]->Caption=="---")
                {
                    ;
                }
                else
                {
                    dTemp=fabs(dTargetTemp-atof(pnFirstTemp_Head40[i]->Caption.c_str()));
                    if(dTemp<=IniConfig.iL04TemptureRange)                      //在範圍內
                    {

                    }
                    else if(dTemp>IniConfig.iL04TemptureRange)                  //超出範圍
                    {
                        bAllTempOk=false;
                    }
                }
            }
        }

        if(bAllTempOk!=bAllPosTempInRange)                                      //不相等的時候, 要做事
        {
            bAllPosTempInRange=bAllTempOk;
            if(bAllPosTempInRange==true &&                                      //第一次溫度全部到達
               IniConfig.iL39_2WaitTempstabilize!=0)
            {
                CheckATCTempTimer.SetSecAndOn(iStableTimeSec);                  //重置Timer
            }
        }

        if(bAllPosTempInRange==true &&
           IniConfig.bL39_2WaitTempstabilize==true &&
           IniConfig.iL39_2WaitTempstabilize!=0)                                  //Hmy 20191224  Add Wait Temperature Stable Time
        {
            if(CheckATCTempTimer.Off())
            {
                for(int i=0; i<ATC_MAX_SITE; i++)
                {
                    pnFirstTemp_Head40[i]->Hint="";
                    pnFirstTemp_Head40[i]->ShowHint=false;
                }
                bAllPosTempOkOverTime=true;
            }
            else
            {
                iCurrSec=CheckATCTempTimer.LatchCycleTimeSec();
                if(iCurrSec<iStableTimeSec)
                {
                    for(int i=0; i<ATC_MAX_SITE; i++)
                    {
                        if(bATC_EnablesChannel[i]==true)
                        {
                            pnFirstTemp_Head40[i]->ShowHint=true;
                            pnFirstTemp_Head40[i]->Hint    =iCurrSec;
                        }
                    }
                }
            }
        }
        else
        {
            bAllPosTempOkOverTime=true;
        }
        return bReturnStatus;

    }
    catch(...)
    {
        MyDBIProcess("Exception", "fCheckATCTempOK");
    }

    return false;
}
//-------------------------------------------------------------------------------
TQPF_Timer tLowTempExecTimer;
bool __fastcall fCheckLowTempExecTime()                                         //Add Cold Temperature Run Over Set Time
{
    try
    {
        int iAllTimmeSec=IniConfig.iL32_7LowTempRunAlarmHour*3600+
                         IniConfig.iL32_7LowTempRunAlarmMin*60;

        int iWorkTemp=(LastSet.iTemperature==Tempture_Ambient)?26:Temperature.fWorkTemperBase;
        bool bReturnStatus=false;
        static bool bTimerStart=false;
        static int iTask=1;

        if(iATCOnLine==false)
        {
            bReturnStatus=true;                                                 //Tset Finction Use
        }
        else if(bDefrostKeepATCTemp==true && fDefrostNote->bShow==true)
        {
            if(bTimerStart==false)
            {
                bTimerStart=true;
                tLowTempExecTimer.SetSecAndOn(3);
            }

            if(tLowTempExecTimer.Off())
            {
                bTimerStart=false;
                fDefrostNote->BringToFront();
            }
            iTask=1;
        }
        else
        {
            switch(iTask)
            {
                case 1:
                    if(iWorkTemp<=IniConfig.iL32_6LowTempRunAlarmDegree)
                    {
                        tLowTempExecTimer.SetSecAndOn(iAllTimmeSec);
                        iTask=100;
                    }
                    break;
                case 100:
                    if(tLowTempExecTimer.Off())
                    {
                        fDefrostNote->sShowMessage="Defrost Time Has Arrived";
                        fDefrostNote->Show();
                        iTask=1000;
                    }
                    break;
                case 1000:
                    tLowTempExecTimer.SetSecAndOn(3);
                    fDefrostNote->BringToFront();
                    iTask=2000;
                    break;
                case 2000:
                    if(fDefrostNote->bStartDefrost==true)
                    {
                        bDefrostKeepATCTemp=true;
                        iLowTempAutoDefrostTask=1;
                        iTask=1;
                    }

                    if(tLowTempExecTimer.Off())
                    {
                        iTask=1000;
                    }
                    break;
            }
        }
        return bReturnStatus;
    }
    catch(...)
    {
        MyDBIProcess("Exception", "fCheckLowTempExecTime");
    }
    return false;
}
//------------------------------------------------------------------------------
bool __fastcall fUseColdTemperatureAutoDefrost()
{
    static TQPF_Timer tTickTimer;
    static TQPF_Timer tTickTimer_Show;
    AnsiString asSelectDeviceName;
    AnsiString StrC="", StrE="";
    int iDefrostTime = IniConfig.iL32_5SetDefrostTime*60;
    int iCountdown=0;
    static int iDefrost_Time=0;
    static bool bArm1=false;
    static bool bArm2=false;

    if(iATCOnLine==false || bDefrostKeepATCTemp==false)
        return false;

    int &iTask=iLowTempAutoDefrostTask;
    if(fDefrostNote->bInterruptDefrost==true)
    {
        iTask=10000;
    }
    switch(iTask)
    {
        case 1:
            if(HasICUnderMachine()==false && SystemStart==false)                //整機台沒有IC，直接做除霜
            {
                fMain->ChangeATCSiteUse();                                      //1004
                ATC_InterfaceForm->SetAllTemp(IniConfig.iL32_4SetDefrostTemp);  //1002
                ATC_InterfaceForm->SendAirMachineStatus(0, 130*10, 130*10);     //1049
                SwitchRefrigerator(0, 0);                                       //1058
                SwitchRefrigerator(1, 0);                                       //1058
                SwitchRefrigerator(2, 0);                                       //1058
                Temperature.bEnableArm_1_Air=false;
                Temperature.bEnableArm_2_Air=false;
                fMain->Pause("fUseColdTemperatureAutoDefrost");
                fAllDevicePosDefrostON_OFF("On");
                tTickTimer.SetSecAndOn(5);
                iTask=100;
            }
            else
            {
                fDefrostNote->sShowMessage="Must Clean Out!";
            }

            break;

        case 100:
            if(ATC_InterfaceForm->AirMachineInfo.asATC_AirMachineStatus=="Defrost")
            {
                iTask=1000;
            }
            if(tTickTimer.Off()==true)
            {
                fAllDevicePosDefrostON_OFF("On");
                tTickTimer.SetSecAndOn(5);
                iTask=100;
            }

            break;
        case 1000:
            fTemp_Set->gbDefrostFunction->Enabled=false;
            bAutoDefrost_OpenAirStream=false;
            tTickTimer.SetSecAndOn(1200);
            iTask=2000;

            break;
        case 2000:
            iCountdown=ATC_InterfaceForm->AirMachineInfo.iDefrostSec;
            fDefrostNote->sShowMessage="Wait ATC Defrost :" + AnsiString(iCountdown) + "s";
            if(ATC_InterfaceForm->AirMachineInfo.asATC_AirMachineStatus!="Defrost")
            {
                iDefrost_Time=tTickTimer.LatchCycleTimeSec();
                iTask=3000;
            }

            if(Check_DefrostALLTempReady(1)==true && bArm1==false)
            {
                Temperature.bEnableArm_1_Air=true;
                ATC_InterfaceForm->SendAirMachineStatus(1, IniConfig.iL32_8SetAirStreamTemp*10, IniConfig.iL32_8SetAirStreamTemp*10);     //1049
                bArm1=true;
            }

            if(Check_DefrostALLTempReady(2)==true && bArm1==false)
            {
                Temperature.bEnableArm_2_Air=true;
                ATC_InterfaceForm->SendAirMachineStatus(1, IniConfig.iL32_8SetAirStreamTemp*10, IniConfig.iL32_8SetAirStreamTemp*10);     //1049
                bArm2=true;
            }

            if(tTickTimer.Off()==true)
            {
                StrC="ATC 除霜逾時";
                StrE="ATC Defrost timeout";
                if(LastSet.iLanguageCountry==1)
                    ShowNoteMessage_1032(StrC);
                else
                    ShowNoteMessage_1032(StrE);

                iTask=10000;
            }

            break;
        case 3000:
            bAutoDefrost_OpenAirStream=true;
            if(iDefrost_Time<Temperature.iDefrostTimeTooLower)
            {
                tTickTimer.SetSecAndOn(5);
                StrC="除霜時間太短";
                StrE="Defrost Time Too Lower";
                if(LastSet.iLanguageCountry==1)
                    ShowNoteMessage_1032(StrC);
                else
                    ShowNoteMessage_1032(StrE);
            }

            iTask=4000;
            break;
        case 4000:
            if(ATC_InterfaceForm->GetRunState()==false && tTickTimer.Off()==true)//Ztex 2023.04.19 Add HT-1032 TriTemp Function ATC 初始化需要一點時間  //ATC 初始化需要一點時間
            {
                ATC_InterfaceForm->Run();
                tTickTimer.SetSecAndOn(5);
                iTask=4000;
            }
            if(Check_DefrostALLTempReady()==true)
            {
                tTickTimer.SetSecAndOn(iDefrostTime);
                iTask=5000;
            }
            fDefrostNote->sShowMessage="Wait ALL Temp Ready";

            break;
        case 5000:
            iCountdown=iDefrostTime-tTickTimer.LatchCycleTimeSec();
            fDefrostNote->sShowMessage="Wait Handler Defrost :" + AnsiString(iCountdown) + "s";
            if(tTickTimer.Off()==true)
            {
                iTask=10000;
            }

            break;
        case 10000://中斷或完成
            fDefrostNote->bStartDefrost          =false;
            fDefrostNote->sShowMessage="";
            fAllDevicePosDefrostON_OFF("Off");
            bDefrostKeepATCTemp                  =false;
            fMain->ChangeATCSiteUse();
            fTemp_Set->gbDefrostFunction->Enabled=true;
            bSetTempChange                       =true;
            fDefrostNote->bInterruptDefrost      =false;
            bAutoDefrost_OpenAirStream           =false;
            bArm1                                =false;
            bArm2                                =false;
            ATC_InterfaceForm->Send_AtcDefrostStatus(0);
            Temperature.bEnableArm_1_Air         =fTemp_Set->chk_EnableArm_1_Air->Checked;
            Temperature.bEnableArm_2_Air         =fTemp_Set->chk_EnableArm_2_Air->Checked;
            fDefrostNote->Close();
            StrC="除霜完成";
            StrE="Defrost Finish";
            if(LastSet.iLanguageCountry==1)
                ShowNoteMessage_1032(StrC);
            else
                ShowNoteMessage_1032(StrE);
            iTask=1;
            break;
    }
    return false;
}
//==============================================================================
//                              SCAN TRITEMP STATE
//==============================================================================
void DoTriTempState_1032()
{
    try
    {
        static bool bFirstRun=true;                                             //TriTemp主要流程
        static int iFirstRunCount=0;
        if(bFirstRun==true)
        {
            iFirstRunCount++;
            if(iFirstRunCount>=700)
            {
                bFirstRun=false;
            }
            else
            {
                return;
            }
        }
        if(fiosetview->fShow==true)
            return;

        bool bflag[3]={false, false, false};
        static bool bflag_WriteLog[3]={false, false, false};                    //Modify IO Write change
        if(SystemStart==false ||
           (bDelayTimeAfterFixDoorOpen==true && SystemStart))
        {
            if(IniConfig.bL34_1DelayOfFixDoorOpen==1)                           //20190625 將溫度與濕度分開判斷(濕度分開) Add//Hmy 20180827 Add Function,fix area frosting protection
            {
                DoDelayTimeAfterFixDoorOpen();
            }
            else if(IniConfig.bL34_1DelayOfFixDoorOpen==1 &&
                    bSuperHotTempNoChkHumidity==true)
            {
                bDelayTimeAfterFixDoorOpen=false;
                iStepOfDelayAfterFixDoorOpen=-1;
            }
            else
            {
                bDelayTimeAfterFixDoorOpen=false;                               //Add Function,fix area frosting protection
                iStepOfDelayAfterFixDoorOpen=-1;                                //Add Function,fix area frosting protection
                if(Sen[SnTriTempSafeDoor6Lock].IsOn())
                {
                    if(CheckPositionIsRange(MInShuttle1, Prod.InSHT[0].iRight) ||
                       CheckPositionIsRange(MInShuttle1, Prod.InSHT[0].iLeft))
                    {
                        bflag[0]=true;
                    }
                    if(CheckPositionIsRange(MInShuttle2, Prod.InSHT[1].iRight) ||
                       CheckPositionIsRange(MInShuttle2, Prod.InSHT[1].iLeft))
                    {
                        bflag[1]=true;
                    }

                    if(CheckPositionIsRange(MTrayX, Prod.iXTrayEmpty))
                    {
                        bflag[2]=true;
                    }

                    if(bflag[0] && bflag[1] && bflag[2])
                    {
                        if(Cylinder[C_Shuttle1Floodgate].GetOutBit()==false)    //Modify IO Write change
                            Cylinder[C_Shuttle1Floodgate].On();
                        if(Cylinder[C_Shuttle2Floodgate].GetOutBit()==false)    //Modify IO Write change
                            Cylinder[C_Shuttle2Floodgate].On();
                        if(IniConfig.bL34_5FixTrayDoorCynAutoOpen ==true)       //Add Function,fix area frosting protection
                        {
                            if(Cylinder[C_SafeDoor6Lock].GetOutBit()==false)    //Modify IO Write change
                                Cylinder[C_SafeDoor6Lock].On();
                        }
                    }
                    else
                    {
                        if(bflag[0]==false && bflag_WriteLog[0]==false)
                        {
                            bflag_WriteLog[0]=true;
                            fMain->slTriTempDoorlog->AddTextWithDateTime("0, 0, Shuttle1 Pos Not Left(Right)");
                        }
                        if(bflag[1]==false && bflag_WriteLog[1]==false)
                        {
                            bflag_WriteLog[1]=true;
                            fMain->slTriTempDoorlog->AddTextWithDateTime("0, 0, Shuttle2 Pos Not Left(Right)");
                        }
                        if(bflag[2]==false && bflag_WriteLog[2]==false)
                        {
                            bflag_WriteLog[2]=true;
                            fMain->slTriTempDoorlog->AddTextWithDateTime("0, 0, TrayX Pos Not StandSafe");
                        }

                        if(Cylinder[C_Shuttle1Floodgate].GetOutBit()==true)     //Modify IO Write change
                            Cylinder[C_Shuttle1Floodgate].Off();
                        if(Cylinder[C_Shuttle2Floodgate].GetOutBit()==true)     //Modify IO Write change
                            Cylinder[C_Shuttle2Floodgate].Off();
                        if(IniConfig.bL34_5FixTrayDoorCynAutoOpen==true)        //Add Function,fix area frosting protection
                        {
                            if(Cylinder[C_SafeDoor6Lock].GetOutBit()==false)    //Modify IO Write change
                                Cylinder[C_SafeDoor6Lock].On();
                        }

                    }
                }
                else
                {
                    for(int i=0; i<3; i++)
                    {
                        if(bflag_WriteLog[i]==true)
                            bflag_WriteLog[i]=false;
                    }
                    if(IniConfig.bL34_5FixTrayDoorCynAutoOpen==true)            //Add Function,fix area frosting protection
                    {
                        if(Cylinder[C_SafeDoor6Lock].GetOutBit()==true)
                            Cylinder[C_SafeDoor6Lock].Off();
                    }

                    if(Cylinder[C_Shuttle1Floodgate].GetOutBit()==true)
                        Cylinder[C_Shuttle1Floodgate].Off();
                    if(Cylinder[C_Shuttle2Floodgate].GetOutBit()==true)
                        Cylinder[C_Shuttle2Floodgate].Off();
                }
            }
        }
        else
        {
            if(Sen[SnTriTempSafeDoor6Lock].IsOn())
            {
                StopAllMotor();
                ShowErrorMessage("MES1641", K_RETRY, MMSystem);
                SystemStart=false;
            }
        }
        fUseColdTemperatureAutoDefrost();                                       //自動除霜功能，檢查IDLE與RUN狀態
        if(IniConfig.bF31_CheckShtMoveCnt)                                      //add 三溫機 Shuttle Motor移動必須計數
        {
            if(IniConfig.iShtMoveCntSet[0]>0)
                fCheckMotorMoveCount_Shuttle(0);
            if(IniConfig.iShtMoveCntSet[1]>0)
                fCheckMotorMoveCount_Shuttle(1);
        }

        if(SW[SwColdAirSwitch].Enable==true) 
        {
            if(IniConfig.bL44_SetColdAirSwitchTemp==true)
            {
                if(Temperature.fWorkTemperBase>=IniConfig.iL44_SetColdAirSwitchTemp)
                    SW[SwColdAirSwitch].Off();
                else
                    SW[SwColdAirSwitch].On();
            }
            else
            {
                if(Temperature.fWorkTemperBase>25)
                    SW[SwColdAirSwitch].Off();
                else
                    SW[SwColdAirSwitch].On();
            }
        }
        fcheckDefrostStatus();
    }
    catch(...)
    {
        MyDBIProcess("Exception", "DoTriTempState_1032");
    }
}
//------------------------------------------------------------------------------
TQPF_Timer Check_ALLTempFirstReady_Timer;
bool Check_ALLTempFirstReady()
{
    bool bResult=true;
    double dTempWorkBase=Temperature.fWorkTemperBase;
    double dSetAirStream_Index =Temperature.dSetIndexAirstreamTemp;
    double dSetAirStream_Socket=Temperature.fSetTempature2AirMachine;
    double dabsValue,dabsRange,dabsSetTemp,dTemptureRangeOver;
    static bool bArray[tcTotalCount];
    static double dWorkBase=0;

    if(ATC_InterfaceForm->IsConnect()==false)
    {
        bALLTempFirstReady=false;
        return false;
    }

    if(dWorkBase!=dTempWorkBase)
    {
        dWorkBase=dTempWorkBase;
        bALLTempFirstReady=false;
        Check_ALLTempFirstReady_Timer.SetSecAndOn(10);
    }

    if(Check_ALLTempFirstReady_Timer.Off()==false)
    {
        return false;
    }

    for(int i=0; i<tcTotalCount; i++)
    {
        bArray[i]=true;
        if(bUT150Install[i]==true && bUT150HasUse[i]==true)
        {
            if(i>=tcAa1 && i<=tcBd2 || i>=tcAe1 && i<=tcBh2 ||
               i>=tcHotPlate1 && i<=tcShuttle2 || i>=tcHotPlate3 && i<=tcShuttle4)     //設定ATC溫控範圍
            {
                dTemptureRangeOver=IniConfig.iL36_1Tri_Temp_Rang_ATC;
                dabsSetTemp=dTempWorkBase;
            }
            else if(i==tcATCHotAir1 || i==tcATCHotAir2)
            {
                dTemptureRangeOver = Temperature.SetAirstreamTemperatureRang_Index;
                if(i==tcATCHotAir1)
                    dabsSetTemp=dSetAirStream_Index;
                else
                    dabsSetTemp=dSetAirStream_Socket;
            }
            else
            {
                dTemptureRangeOver=IniConfig.iL36_2Tri_Temp_Rang_Heater;               //設定Hrater溫控範圍
                dabsSetTemp=Temperature.fIndividualTemp[i];
            }

            dabsValue  =fabs(UN150Read[i]);
            dabsRange  =dTemptureRangeOver;
            dabsSetTemp=fabs(dabsSetTemp);

            if(dabsValue>(dabsSetTemp-dabsRange) &&
               dabsValue<(dabsSetTemp+dabsRange))
            {
              bArray[i]&=true;
            }
            else
            {
              bArray[i]&=false;
            }
        }

        bResult&=bArray[i];
        if(bResult==true && i==tcTotalCount-1)
            bALLTempFirstReady=true;
    }

    return bResult;
}
//------------------------------------------------------------------------------
bool Check_AirVolume_Lmt()//Air Strream Air Volume Too Small
{
    static bool bTimerStart=false;
    int iIendx_V  =ATC_InterfaceForm->AirMachineInfo_Index.iATC_AirVolume;
    int iSocket_V =ATC_InterfaceForm->AirMachineInfo.iATC_AirVolume;
    bool bIndex   =Temperature.bEnableArm_1_Air==true || Temperature.bEnableArm_2_Air==true;
    bool bSocket  =Temperature.bEnableSocket_Air;
    if(ATC_InterfaceForm->IsConnect()==false || iIendx_V==0 || iSocket_V==0)
        return false;

    if(bTimerStart==true)
    {
        bTimerStart=false;
        TimerCheckInterval.SetSecAndOn(3);
    }

    if(TimerCheckInterval.Off())
    {
        bTimerStart=true;
        if(Temperature.iAirVolumeLmt>iIendx_V && bIndex==true)
        {
            ShowErrorMessage("WAR15402", K_RETRY, MMSystem);
            return true;
        }

        if(Temperature.iAirVolumeLmt>iSocket_V && bSocket==true)
        {
            ShowErrorMessage("WAR15402", K_RETRY, MMSystem);
            return true;
        }
    }
    return false;

}
//------------------------------------------------------------------------------
TQPF_Timer TriTemp_OverRang;
TQPF_Timer CheckInterval;
bool Check_TriTemp_OverRang()
{
    bool bResult = false;
    static double OldTemp[tcTotalCount];
    static int    iCount[tcTotalCount];
    AnsiString sTemp = "";
    static  bool bTimerStart = true;

    AnsiString asTempCtrl[tcTotalCount]={"Plate 1-1", "Plate 1-2", "In Sht 1-1", "In Sht 1-2",
                                        "Head 1/2", "Head 3/4", "Head 5/6", "Head 7/8",
                                        "Dut", "Chamber", "CCD",
                                        "Aa1", "Ab1", "Ac1", "Ad1",
                                        "Ae1", "Af1", "Ag1", "Ah1",
                                        "Aa2", "Ab2", "Ac2", "Ad2",
                                        "Ae2", "Af2", "Ag2", "Ah2",
                                        "Heat Gun1", "Heat Gun2",
                                        "DUT1","DUT2","DUT3","DUT4",
                                        "Ba1", "Bb1", "Bc1", "Bd1",
                                        "Be1", "Bf1", "Bg1", "Bh1",
                                        "Ba2", "Bb2", "Bc2", "Bd2",
                                        "Be2", "Bf2", "Bg2", "Bh2",
                                        "2D", "L/B","Index ESD","CCD1",
                                        "Index", "Socket",
                                        "Out SH1","Out SH2","Base1","Base2",
                                        "Base3","Base4","Base5","Base6",
                                        "Plate 2-1", "Plate 2-1", "In Sht 2-1", "In Sht 2-2"};

    if(ATC_InterfaceForm->IsConnect()==false)
        return false;

    if(bTimerStart==true)
    {
        bTimerStart=false;
        CheckInterval.SetSecAndOn(1);
    }

    for(int i=0; i<tcTotalCount; i++)
    {
        if(bALLTempFirstReady==true)
        {
            OldTemp[i]=UN150Read[i];
        }
        else
        {
            iCount[i] =0;
        }
    }

    if(CheckInterval.Off())
    {
        bTimerStart=true;
        if(bALLTempFirstReady==true && bTimerStart==true)
        {
            for(int i=0; i<tcTotalCount; i++)
            {
                if(bUT150Install[i]==false)
                    continue;

                if(fabs(fabs(UN150Read[i]) - fabs(OldTemp[i])) > IniConfig.iL40ImmediateTempExceedsAlarm)
                {
                    sTemp = asTempCtrl[i];
                    ShowNoteMessage_1032(sTemp+"\n Immediate Temperature Exceed Range");
                }

                if(fabs(fabs(UN150Read[i]) - fabs(OldTemp[i])) > 3)
                {
                    TriTemp_OverRang.SetSecAndOn(IniConfig.iL41TemperatureAlarmSecond);
                    iCount[i]=1;
                }
                else
                {
                    iCount[i]=0;
                }

                if(TriTemp_OverRang.Off()==true)
                {
                    if(iCount[i]==1)
                    {
                        sTemp = asTempCtrl[i];
                        ShowNoteMessage_1032(sTemp+"\n Exceed Temperature Within Set Time ");
                    }
                }
            }
        }
    }

    return bResult;
}
//------------------------------------------------------------------------------
bool Check_DefrostALLTempReady(int iIndex)
{
    bool bResult=true;
    double dabsValue,dabsSetTemp;
    bool bArray[tcTotalCount];
    double dTemp=IniConfig.iL32_4SetDefrostTemp;
    #ifdef SOFT_SIMULTE
        return false;
    #else
    if(ATC_InterfaceForm->IsConnect()==false)
    {
        return false;
    }

    for(int i=0; i<tcTotalCount; i++)
    {
        bArray[i]=true;
        if(bUT150Install[i]==true && bUT150HasUse[i]==true)
        {
            if(iIndex==0)
            {
                if(i>=tcAa1 && i<=tcBd2 || i>=tcAe1 && i<=tcBh2 ||
                   i>=tcHotPlate1 && i<=tcShuttle2 || i>=tcHotPlate3 && i<=tcShuttle4)     //設定ATC溫控範圍
                {
                    dabsSetTemp=dTemp;
                    dabsValue  =fabs(UN150Read[i]);
                    dabsSetTemp=fabs(dabsSetTemp);
                    if(dabsValue>dabsSetTemp)
                    {
                        bArray[i]&=true;
                    }
                    else
                    {
                        bArray[i]&=false;
                    }
                }
                else if(i==tcATCHotAir1 || i==tcATCHotAir2)
                {
                    dabsSetTemp=IniConfig.iL32_8SetAirStreamTemp;
                    dabsValue  =fabs(UN150Read[i]);
                    dabsSetTemp=fabs(dabsSetTemp);
                    if(dabsValue>dabsSetTemp)
                    {
                        bArray[i]&=true;
                    }
                    else
                    {
                        bArray[i]&=false;
                    }
                }
            }
            else if(iIndex==1 && (i>=tcAa1 && i<=tcBd1 || i>=tcAe1 && i<=tcBh1))
            {
                dabsSetTemp=dTemp-20;
                dabsValue  =fabs(UN150Read[i]);
                dabsSetTemp=fabs(dabsSetTemp);

                if(dabsValue>dabsSetTemp)
                {
                    bArray[i]&=true;
                }
                else
                {
                    bArray[i]&=false;
                }
            }
            else if(iIndex==2 && (i>=tcAa2 && i<=tcBd2 || i>=tcAe2 && i<=tcBh2))
            {
                dabsSetTemp=dTemp-20;
                dabsValue  =fabs(UN150Read[i]);
                dabsSetTemp=fabs(dabsSetTemp);

                if(dabsValue>dabsSetTemp)
                {
                    bArray[i]&=true;
                }
                else
                {
                    bArray[i]&=false;
                }
            }

            if(dabsValue>dabsSetTemp)
            {
                bArray[i]&=true;
            }
            else
            {
                bArray[i]&=false;
            }
        }

        bResult&=bArray[i];
    }

    return bResult;
    #endif
}
//------------------------------------------------------------------------------
bool Retrrn_InitialOK()
{
    return InitialOK;
}
//------------------------------------------------------------------------------
int Retrrn_AccessLevel()
{
    return AccessLevel;
}
//------------------------------------------------------------------------------


