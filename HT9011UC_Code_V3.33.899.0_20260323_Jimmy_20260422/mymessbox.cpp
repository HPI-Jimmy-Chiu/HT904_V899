#include "MachineDefine.h"
#pragma hdrstop

#include "mymessbox.h"

#include "MachineType.h"
#include "note.h"
#include "csystem.h"
//#include "ascanner.h"                                                         //Steven 20140409 : Mark Scanner
#include "mycylin.h"
#include "main.h"
#include "myswitch.h"
#include "mysensor.h"
#include "ckernel.h"
#include "MyMotor.h"
#include "cMyDB.h"
#include "uLotInfo.h"
#include "automation.h"
#include "rs232.h"
#include "CCDInterface.h"
#include "common.h"
#include "cprod.h"
//#include "SECSGEM.h"
#include "Password.h"
#include "myQwertyKeyBoard.h"
#include "INPUT.h"
#include "BarcodeReader.h"

#include "uHGemEquipment.h"
#include "uHGemHT9045.h"
#include "InterfaceSYS.h"
#include "mykitsuck.h"
#include "cmydef.h"
#include "cpublic.h"
#include "asendic_Auto.h"
#include "note.h"
#include "cOffSet.h"
#include "cSpeed.h"
#include "ProductionInfo.h"                                                     //Sam 20170809 (Steven) 移植超豐 OEE 功能 form HT-7045
#include "HS_Function.h"
#include "MyPLC_IO_Modbus.h"
#include "OmronEJ1N.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TMyMessageBox *MyMessageBox;
extern bool bHangTimePause;                                                     //Steven 20090827 : Hang Up dectector
extern bool bSupplyNewICTrayPause;

int iValue=0;
bool bDisableKeypad=false;
bool bChangeForm=false;                                                         //kevin 20150710 改變form
bool bChangeLotEndMsg=false;                                                    //JerryYang 20170424 (wei) for JCET change form
//---------------------------------------------------------------------------
__fastcall TMyMessageBox::TMyMessageBox(TComponent* Owner)
    : TForm(Owner)
{
    fShow=false;
    bDisableAlarmBuzzer=false;
    bNeedPassWord=false;
}
//---------------------------------------------------------------------------
void __fastcall TMyMessageBox::FormShow(TObject *Sender)
{
    AnsiString sMessage;
    bPauseInMotor=true;
    bPauseOutMotor=true;
    bAlarmReset=false;                                                          //Steven 20140905 : 紀錄有被按下Alarm Reset
    ExString="";                                                                //Steven 20151111 : 改成全域變數,避免Alarm與Message按的時候沒紀錄

    pnlPause->Width             =130;                                           //Ifor 20160905 FormShow 時先設定初始值避免Secs Gem Alarm 改變到視窗大小
    pnlPause->Caption           ="OK";
    pnlPause->Color             =(TColor)0x00917B51;
    pnlPause->Font->Color       =clWhite;
    pnlPause->Font->Size        =12;
    pnlPause->Height            =33;
    pnlPause->Left              =155;                                           //JerryYang 20220923 : 調整message UI

    lblChineseMsg->Visible      =true;
    moSecsGem->Font->Size       =12;
    moSecsGem->Color            =(TColor)0x00DFD9CC;
    if(FormHS->bHaltHandler==true)
        moSecsGem->ScrollBars   =ssVertical;
    else
        moSecsGem->ScrollBars   =ssBoth;
    lblSecsMsg->Visible         =false;

    #ifndef SOFT_SIMULTE
    Left=(XResolution-Width)/2;                                                 //Steven 20230912 : 調整畫面為FULL HD
    Top =(YResolution-Height)/2;
    #else
    Left=100;
    Top =100;
    #endif
    AnsiString Str;

    lblMainMsg->Alignment=taCenter;                                             //JerryYang 20220923 : 調整message UI
    lblChineseMsg->Alignment=taCenter;

    if(FormBarcodeReader->bShow && bSECSGEMAlarm==true)                         //Ifor 20161201 Fix MessageBox 視窗 & BarCode 視窗 同時顯示 導致視窗無法操作
        FormBarcodeReader->Close();

    if(IniConfig.bC08_SocketSensor &&
       bIsSocketSensor ||
       bIsContactforce ||                                                       //kevin 20130418 add contact force over error
       bBigMyMessage)                                                           //JerryYang 20220923 : 調整message UI
    {
        Height=381;                                                             //kevin 20130418   250;
        Width=842;                                                              //kevin 20130418    480;
        lblMainMsg->Left    =8;                                                 //kevin 20130421
        lblMainMsg->Top     =42;
        lblMainMsg->Width   =800;
        lblChineseMsg->Left =2;
        lblChineseMsg->Top  =108;
        lblChineseMsg->Width=800;
        pnlMain->Left       =8;
        pnlMain->Top        =8;
        pnlMain->Height     =270;
        pnlMain->Width      =800;
        pnlPause->Left      =210;                                               //JerryYang 20220923 : 調整message UI
        pnlPause->Top       =284;
        pnlYes->Left        =71;
        pnlYes->Top         =284;
        pnlNo->Left         =271;
        pnlNo->Top          =284;
        if(bBigMyMessage)                                                       //JerryYang 20220923 : 調整message UI
        {
            bBigMyMessage=false;
            lblMainMsg->Alignment=taLeftJustify;
            lblChineseMsg->Alignment=taLeftJustify;
            Width=590;
            lblMainMsg->Width      =570;
            lblChineseMsg->Width   =570;
            pnlMain->Width         =570;
        }
    }
    else if(bSECSGEMAlarm==true && CUSTOMER_CODE==CC_KYEC_LEE)
    {
        #ifdef SOFT_SIMULTE
            Height=601;
            Width=600;
            Left=0;
            Top=0;
            palWaitEAP->Left        =10;
            palWaitEAP->Top         =400;
            palWaitEAP->Height      =50;
            palWaitEAP->Width       =600;

            pnlPause->Left          =10;
            pnlPause->Top           =500;
            pnlPause->Caption       ="Close";
            pnlPause->Width         =600;
            pnlPause->Color         =clYellow;
            pnlPause->Font->Color   =clRed;
            pnlPause->Font->Size    =24;
            pnlPause->Height        =42;

            pnlMain->Left           =8;
            pnlMain->Top            =48;
            pnlMain->Height         =438;
            pnlMain->Width          =600;

            moSecsGem->Align        =alClient;
            moSecsGem->Font->Size   =18;
            moSecsGem->Color        =clRed;
            moSecsGem->Font->Color  =clYellow;
            moSecsGem->ScrollBars   =ssNone;

            lblSecsMsg->Top         =8;
            lblSecsMsg->Height      =49;
            lblSecsMsg->Width       =600;
            lblSecsMsg->Caption     ="======= SECS Gem Message =======";
            lblSecsMsg->Visible     =true;

            pnlMain->Enabled=false;
            pnlAlarmReset->Visible=false;                                       //Ifor 20160823 KYEC 要求取消 SECS GEM Alarm Reset 顯示
        #else
            Height=1002;
            Width=1288;
            Left=0;
            Top=0;

            palWaitEAP->Left        =10;
            palWaitEAP->Top         =810;
            palWaitEAP->Height      =50;
            palWaitEAP->Width       =1265;

            pnlPause->Left          =10;
            pnlPause->Top           =923;
            pnlPause->Caption       ="Close";
            pnlPause->Width         =1265;
            pnlPause->Color         =clYellow;
            pnlPause->Font->Color   =clRed;
            pnlPause->Font->Size    =24;
            pnlPause->Height        =42;

            pnlMain->Left           =8;
            pnlMain->Top            =48;
            pnlMain->Height         =876;
            pnlMain->Width          =1265;

            moSecsGem->Align        =alClient;
            moSecsGem->Font->Size   =18;
            moSecsGem->Color        =clRed;
            moSecsGem->Font->Color  =clYellow;
            moSecsGem->ScrollBars   =ssNone;

            lblSecsMsg->Top         =8;
            lblSecsMsg->Height      =49;
            lblSecsMsg->Width       =1265;
            lblSecsMsg->Caption     ="============ SECS Gem Message ============";
            lblSecsMsg->Visible     =true;

            pnlMain->Enabled=false;
            pnlAlarmReset->Visible=false;                                       //Ifor 20160823 KYEC 要求取消 SECS GEM Alarm Reset 顯示
        #endif
    }
    else if(bShowIndexMotorError==true)                                         //Isaac 20201012 : encoder和command/Teaching點比較，show alarm視窗變大
    {
        bShowIndexMotorError=false;
        Height=381;                                                             //kevin 20130418   250;
        Width=842;                                                              //kevin 20130418    480;
        lblMainMsg->Left    =8;                                                 //kevin 20130421
        lblMainMsg->Top     =42;
        lblMainMsg->Width   =800;
        lblChineseMsg->Left =2;
        lblChineseMsg->Top  =108;
        lblChineseMsg->Width=800;
        pnlMain->Left       =8;
        pnlMain->Top        =8;
        pnlMain->Height     =270;
        pnlMain->Width      =800;
        pnlPause->Left      =330;
        pnlPause->Top       =284;
        pnlYes->Left        =71;
        pnlYes->Top         =284;
        pnlNo->Left         =271;
        pnlNo->Top          =284;
    }
    else
    {
        Height=250;                                                             //kevin 20130418   250;
        Width=480;                                                              //kevin 20130418    480;
        lblMainMsg->Left    =8;                                                 //kevin 20130421
        lblMainMsg->Top     =42;
        lblMainMsg->Width   =441;
        lblChineseMsg->Left =2;
        lblChineseMsg->Top  =108;
        lblChineseMsg->Width=441;
        pnlMain->Left       =8;
        pnlMain->Top        =8;
        pnlMain->Height     =153;
        pnlMain->Width      =457;
        pnlPause->Left      =171;
        pnlPause->Top       =172;
        pnlYes->Left        =71;
        pnlYes->Top         =172;
        pnlNo->Left         =271;
        pnlNo->Top          =172;
    }

    if(!iUnLoaderCount)                                                         //Jou 20150721 : 重新啟用功能
    {
        SystemStart=false;
        SoftStart=false;                                                        //ChungHung 20110829 add
        if(SystemInitialOK==true)                                               //Steven 20121102 : 馬達還沒好,不可以下命令
            StopAllMotor();
        bSupplyNewICTrayPause=true;
        bHangTimePause=true;                                                    //Steven 20090827 : Hang Up dectector
    }
//    iUnLoaderCount=0;                                                         //Ifor 20160829 add 避免有些Alarm 不停機造成後面Alarm都不停機    //JerryYang 20160912 Mark掉,close的時候才把iUnLoaderCount設為0
    if(bChangeForm)                                                             //kevin 20150710 改變form
    {
        bChangeForm=false;                                                      //kevin 20150710 改變form
        lblMainMsg->Left=8;                                                     //kevin 20130421
        lblMainMsg->Top =8;
        MyMessageBox->lblChineseMsg->Top=64;                                    //kevin 20150821
        MyMessageBox->lblSubMsg->Top=MyMessageBox->lblChineseMsg->Top+MyMessageBox->lblChineseMsg->Height+15;
    }
    else if(bChangeLotEndMsg)                                                   //JerryYang 20170424 (wei) for JCET change form
    {
        bChangeLotEndMsg=false;
        if(TestIF_File.bRENESAS_EnableFTCT==true)                               //RogerYang 20250923 : 瑞薩FT-CT
        {
            pnlPause->Caption     ="不供給";
        }
        else
        {
            pnlPause->Caption="Skip";
        }
        pnlYes->Left=10;
        pnlNo->Left=172;
        pnlPause->Left=335;
    }

    bAlarmBuzzer=!bDisableAlarmBuzzer;

    for(int i=0; i<7; i++)                                                      //jou 2011-02-21 start : magazine沒動作會alarm
    {
        bLifterPause[i]=true;
        bAuto2Pause[i]=true;                                                    //kevin 20120718 for auto2
    }

    fShow=true;

    if(CUSTOMER_CODE==CC_MTI ||
       CUSTOMER_CODE==CC_PTI)                                                   //Sam 20190429 : Add CC_PTI_NEWWORK
    {
        fAutomation->DoCommandBuffer("MESSAGE_REQUEST", "", lblMainMsg->Caption, 0, "");
    }

    labStopTime->Visible=CosFunction.bShowHandlerStopTime;                      //jou 2014-09-21 Show Handler Stop Time
    lHandlerStopTime.LatchCycleTime(true);                                      //jou 2014-09-21 Show Handler Stop Time
    if(CosFunction.bShowHandlerStopTime)                                        //jou 2014-09-21 Show Handler Stop Time
        Height=Height+50;                                                       //Steven 20220902 : 修正Message畫面高度不足

    if(CosFunction.bOEEFunction && IniConfig.bN14_14_AlarmCtrlMachine)          //Steven 20190724 : [N14-14] Use alarm control machine
    {
        sMessage.sprintf("Alarm,Message,%s", lblMainMsg->Caption);
        fProductionInfo->ACM_WriteMsgAndCallExe(sMessage);
    }

    if(bSECSGEMAlarm || FormHS->bHaltHandler==true)                             //20160624 secs訊息顯示問題
    {
        MyMessageBox->moSecsGem->Visible=true;
        if(CUSTOMER_CODE==CC_KYEC_LEE)
            MyMessageBox->moSecsGem->Top=20;
        else
            MyMessageBox->moSecsGem->Top=0;
    }
    else
    {
        MyMessageBox->moSecsGem->Visible=false;
        MyMessageBox->moSecsGem->Top=150;
    }

    bSendRealCCDSendStart=true;
    if(CUSTOMER_CODE==CC_PANTHER)                                               //Jimmychiu 20250916 : 新增機台運作狀態紀錄
    {
        fMain->machineTime.Pause();
    }
}
//---------------------------------------------------------------------------
extern TQPF_Timer tGalilTwoYMoveDelay;
void __fastcall TMyMessageBox::FormClose(TObject *Sender, TCloseAction &Action)
{
    if(!iUnLoaderCount)                                                         //Jou 20150721 : 重新啟用功能
    {
        if(Sen[SnSafeDoor3].Enable==false)
        {
            bIsTestSitICFallDown=false;
            bContactCTOverCHK=false;
        }
    }
    else                                                                        //Jou 20150721 : 重新啟用功能
    {
        iUnLoaderCount=0;
    }
    AnsiString str;
    str=lblMainMsg->Caption;

    if(CosFunction.bOEEFunction)                                                //Steven 20180417 (Jou) : OEE功能
    {                                                                           //Sam 20170724 (Steven) 移植超豐 OEE 功能 form HT-7045
        fProductionInfo->bFTPError=false;
    }

    fShow=false;

    if(IniConfig.bEnable_SECS_GEM==true)                                        //Steven 20140528 : Secs Gem
    {
        if(bSECSGEMAlarm)                                                       //wei 20150817 S10F3 Alarm Reset按鍵
        {
            EventReport(SECS_EVENT.MymessboxOK);                                // 73     按下 OK
            bSECSGEMAlarm=false;
            MyMessageBox->moSecsGem->Clear();                                   //20160624 secs訊息顯示問題
        }
        else
        {
            EventReport(SECS_EVENT.DoPause);                                    // 2     按下 Pause
        }
    }

    if(CosFunction.bOEEFunction)                                                //Sam 20171213 (Steven) : 超豐良率監控
    {
        MyMessageBox->pnlMain->Color=(TColor)0x00DFD9CC;
    }

    SendCommand_ESD(ESD_SYSTEM_STOP);
    bEnterTestIF=true;
    bAlarmReset=false;                                                          //Steven 20140905 : 紀錄有被按下Alarm Reset

    lHandlerStopTime.LatchCycleTime(true);                                      //jou 2014-09-21 Show Handler Stop Time
    MyMessageBox->moSecsGem->Visible=false;                                     //20160624 secs訊息顯示問題
    MyMessageBox->moSecsGem->Top=150;
    Width=480;                                                                  //JerryYang 20191123 fix Message位置偏移
    Height=250;
    bHasQwertyKeyForm   =false;                                                 //Ifor 20170907 (wei) add
    bHasPasswordForm    =false;
    hAutoCleanHangUp.SetSecAndOn(Prod.iHangupMaxTime);                          //Steven 20220823 : 機台有暫停就要重新計算
    tGalilTwoYMoveDelay.SetSecAndOn(60);
}
//---------------------------------------------------------------------------
void __fastcall TMyMessageBox::pnlPauseClick(TObject *Sender)
{
    if(bWaitSecsGemReply==true)                                                 //Ifor 20180227 (Steven) add SECS GEM Confirm the Employee ID
    {
        return;
    }

    bStartMoveSpeed=false;                                                      //Steven 20231018 : Fixed for G14

    if(bMBoxNeedPassword)                                                       //2013-03-05    Dell 要關掉省電模式訊息要密碼
    {
        Top =0;

        if(DoPassword_MBox()==false)
        {
            return;
        }
        else
        {
            bMBoxNeedPassword=false;
        }
    }

    if(bSECSGEMAlarm)                                                           //wei 20150817 S10F3 按鍵不能按
    {
        if(CosFunction.bUseN07_5==true)                                         //Steven 20200309 : [N07-5]改成by客戶開啟
        {
            if(iSECSMessageCanCloseByOperator!=1)                               //Ifor 20171024 (Steven) : add KYEC SECS GEM Can Close By Operator 0:一般流程 1:直接關閉Alarm 2:啟動工號檢查流程
            {
                if(IniConfig.bN07_EnableEmployeeIdCheak==true)
                {
                    if(iSECSMessageCanCloseByOperator==0)                       //一般帳密
                    {
                        if(DoPassword_MBox()==false)
                        {
                            return;
                        }
                    }
                    else if(iSECSMessageCanCloseByOperator==2)                  //啟動工號檢查流程
                    {
                        bEnableEmployeeIDCheck=true;
                        CheckEmployeeID("工號檢查:請輸入工號密碼");
                        return;
                    }
                }
                else
                {
                    if(DoPassword_MBox()==false)
                    {
                        return;
                    }
                }
            }
            else
            {
                if(fNote->fShow)
                {
                    fNote->Close();
                }
                if(MyMessageBox->Visible==true)
                {
                    MyMessageBox->Close();
                }
                if(fPassword->Visible==true)
                {
                    fPassword->Close();
                }
            }
        }
        else
        {
            if(Barcode_Reader(bcSECSGEM)==0)
            {
                return;
            }
        }
    }

    iValue=0;                                                                   //JerryYang 20170424 (wei) JCET
    if(TestIF_File.bIndexCycleTimeMonitor==true && bResetflag==false)           //Isaac 20180301 (Steven) Index Cycle Time Monitoring function
    {
        bResetflag=true;
    }
    Close();

    if(TestIF_File.bRENESAS_EnableFTCT==true)                                   //RogerYang 20251016 : RogerYang 瑞薩FT-CT 解除alarm，已經手動按掉了就放在後面
    {
        bRenesasFTCTAlarm=false;
    }
}
//---------------------------------------------------------------------------
extern bool CheckTestBinData(class TMyKitSuck &Ptr);                            //kevin 20150202
void __fastcall TMyMessageBox::Timer1Timer(TObject *Sender)
{
    if(fShow==false)
        return;
    fMain->Timer1Timer(fMain);
    fMain->Timer7Timer(fMain);
    fOmron->Main232();                                                          //Steven 20250424 : add protection for Omren EJ1N
    fOmron->Timer1Timer();

    ProcessStatrDigital();

    //DoScan();                                                                 //Steven 20140409 : Mark Scanner

    if(Enable_PLCSafety_IO==true)                                               //kevin 20250407 check PLC
        bPLCStatusCheck();

    if(bSECSGEMAlarm==false)                                                    //wei 20150817 S10F3 按鍵不能按 //Ifor 20170616 (wei) add S10F3 後發生的Note Alarm 要恢復按鍵掃描解除Note後再鎖定按鍵
    {
        if(iControlPanelMode)                                                   //Ken 20210812 : 解決Alarm無法按按鈕
            fPadInterface->Main232();

        ret=ScanPannelKey();
        if(ret==SnFKPause || ret==SnFKRetry || ret==SnFKSkip)
        {
            if(bDisableKeypad==false)
            {
                Close();
            }
        }
        else if(ret==SnFKAlarmReset && bAlarmBuzzer)
        {
            bAlarmReset=true;                                                   //Steven 20140905 : 紀錄有被按下Alarm Reset
            NewRecordProcess("MES2116", "ALARM RESET pressed", "MessageBox_Timer");
            bAlarmBuzzer=false;
            bLampAlarmReset=false;
            SECS_GEM_PPMUSIC_CONTROL_flag=false;
            SECS_GEM_PPSIGNALTOWER_CONTROL_flag=false;
            SW[SwFKAlarmReset].Off();
            SW[SwRKAlarmReset].Off();
            if(IniConfig.bEnable_SECS_GEM==true)                                //Steven 20140528 : Secs Gem
                EventReport(SECS_EVENT.DoAlarmReset);                           //30     按下 Alarm Reset    // 2013.11.07 ); Joye); KYEC SECS/GEM
        }
    }

    if(MyMessageBox->pnlPause->Caption=="Alarm Reset")
        bLampAlarmReset=FlushFlag;
    else
        bLampPause=FlushFlag;

    DoSystemMessage();
    if(bIsTestSitICFallDownResetHT9045)
    {
        CheckSafeDoorForICFallDown();
        if(bIsTestSitICFallDownResetHT9045==false)
            Close();
    }

    if(bAutoCleanCheckOpenDoor)                                                 //Steven 20241019 : 修正auto clean msg關不掉的問題
    {
        CheckSafeDoorForICFallDown();
        if(bAutoCleanCheckOpenDoor==false)
            Close();
    }

    if(bChangeCleanPad)                                                         //Steven 20241019 : 修正auto clean msg關不掉的問題
    {
        CheckSafeDoorForICFallDown();
        if(bChangeCleanPad==false)
            Close();
    }

    if(bContactCTOverCHK)
    {
        CheckSafeDoorForICFallDown();
        if(bContactCTOverCHK==false)
            Close();
    }

    if(bIsContactforce)                                                         //kevin 20130418 add contact force over error
    {
        CheckSafeDoorForICFallDown();
        if(bIsContactforce==false)
            Close();
    }

    if(IniConfig.bC08_SocketSensor && bIsSocketSensor)
    {                                                                           //kevin 20130504 socket sensor detect error
        CheckSafeDoorForICFallDown();
        if(IniConfig.bC08_SocketSensor && bIsSocketSensor==false)
           Close();                                                             //kevin 20130418 socket sensor detect error
    }

    if(IniConfig.bI26TestCloseSiteHaveBin && bTestBinDataError!=0)              //kevin 20150202
    {
        if(bTestBinDataError==1)
        {
            if(CheckTestBinData(FTestSuck))
            {
                bTestBinDataError=0;
                Close();
            }
        }
        else
        {
            if(CheckTestBinData(BTestSuck))
            {
                bTestBinDataError=0;
                Close();
            }
        }
    }

    DoAvoidIndexMotorFallDown();

    if(IniConfig.bEnableCCDUSETCPIP)                                            //Eliot 2010_1209 start
    {
        if(CCDInterfaceForm->iIdentificationStatus==2)                          //2:Fail
            Top=551;
        else if(bSECSGEMAlarm==true && CUSTOMER_CODE==CC_KYEC_LEE)              //Ifor 20160824 add 京元 SECS Message 視窗最大化
            Top=0;
        else
            Top=243;
    }

    if(fNote->fShow==false)
    {
        ScanTrayStatus();
    }

    static unsigned int iCT=0;
    iCT++;
    if(iCT>(ChangeToFloatNonPcnt((double)(1000), (double)(Timer1->Interval))))                                             //jou 2011-08-01 start : 避免訊息被蓋在main form下面
    {
        iCT=0;
        if(fPassword->bShow==true && bSECSGEM_NoteAlarm==false)                 //Ifor 20170804 (wei) add 避免Offset 小鍵盤開啟時Alarm後視窗一直跳到fQwertyKey
        {
            if(bHasQwertyKeyForm==true)
            {
                if(bHasPasswordForm==true)
                {
                    if(fPassword2->bShow==false)
                    {
                        if(FormBarcodeReader->bShow)                            //Steven 20140508
                            FormBarcodeReader->BringToFront();
                        else
                            MyMessageBox->BringToFront();
                    }
                    else
                    {
                        if(fQwertyKey2->bKey2==true)
                            fQwertyKey2->BringToFront();
                        else
                            fPassword2->BringToFront();
                    }
                }
                else
                {
                    if(fQwertyKey2->bKey2==true)
                        fQwertyKey2->BringToFront();
                    else
                        fPassword->BringToFront();
                }
            }
            else
            {
                if(bHasPasswordForm==true)
                {
                    if(fPassword2->bShow==false)
                    {
                        if(FormBarcodeReader->bShow)                            //Steven 20140508
                            FormBarcodeReader->BringToFront();
                        else
                            MyMessageBox->BringToFront();
                    }
                    else
                    {
                        if(fQwertyKey->bShow==true)
                            fQwertyKey->BringToFront();
                        else
                            fPassword2->BringToFront();
                    }
                }
                else
                {
                    if(fQwertyKey->bShow==true)
                        fQwertyKey->BringToFront();
                    else
                        fPassword->BringToFront();
                }
            }
        }
        else if(FormBarcodeReader->bShow==true)                                 //Ifor 20170804 add 避免Offset 小鍵盤開啟時Alarm後視窗一直跳到fQwertyKey
        {
            FormBarcodeReader->BringToFront();
        }
        else if(bSECSGEM_NoteAlarm==true && fNote->fShow==true)                 //Ifor 20170616 (wei) add S10F3 後跳出Note Alarm 需顯示上層避免卡死
        {
            fNote->BringToFront();
        }
        else if(MyMessageBox->fShow==true && bSECSGEMAlarm==true)               //Ifor 20170804 add 避免Offset 小鍵盤開啟時Alarm後視窗一直跳到fQwertyKey
        {
            MyMessageBox->BringToFront();
        }
        else if(fQwertyKey->bShow==true)                                        //ChungHung 20120912 add  ChungHung alter 201201011 ?虛?鷍
        {
            fQwertyKey->BringToFront();
        }
        else
        {
            MyMessageBox->BringToFront();
        }
    }

    if(FormBarcodeReader->bShow)                                                //Steven 20140508
        FormBarcodeReader->BringToFront();

    SocketAirCoolingStart();                                                    //jou 2016-04-28 Socket Air Cooling contact count trun on
}
//---------------------------------------------------------------------------
extern void InitInArmTask();
//---------------------------------------------------------------------------
void ShowMyMessage(AnsiString S1, AnsiString S2, AnsiString S3, bool Ok, bool bServoOff)
{
    AnsiString str;
    bool bAlarmStatus=false;                                                    //Ifor 20170615 (wei) add SECS GEM Alarm狀態備份
    if(InitialOK==false)                                                        //Ifor 20151230 : fixed
    {
        MyDBIProcess("Exception", S1, S3);
        return;
    }
#ifdef SOFT_SIMULTE
    if(S1.Pos(" port error")!=0)
    {
        return;
    }
#endif
    bHandlerPause=true;                                                         //JerryYang 20200407 show alarm以及message也要把flag設true
    iHandlerStartCount=0;

    if(bSECSGEMAlarm || FormHS->bHaltHandler==true)                             //wei 20150817 S10F3 Alarm Reset按鍵
        MyMessageBox->pnlAlarmReset->Visible=true;
    else
        MyMessageBox->pnlAlarmReset->Visible=false;
    MyMessageBox->lblSubMsg->Visible=false;                                     //kevin 20150713

    if(MyMessageBox->fShow==true && iUnLoaderCount==0)                          //JerryYang 20160830 考慮auto滿tray時show message但不停機的情況
    {
        return;
    }
    else if(MyMessageBox->fShow==true && iUnLoaderCount!=0)                     //JerryYang 20160830 當show message但不停機的情況,這時候又跳出message要把Form關掉再show,避免錯誤
    {
        if(CUSTOMER_CODE==CC_KYEC_LEE)                                          //Ifor 20170615 (wei) KYEC add MyMessageBox->Close時備份 SECS GEM Alarm 旗標
        {
            bAlarmStatus=bSECSGEMAlarm;                                         //備份目前SECS GEM Alarm 旗標 避免 Close時被清除
            MyMessageBox->Close();
            bSECSGEMAlarm=bAlarmStatus;                                         //還原保留的SECS GEM Alarm 旗標
        }
        else
        {
            MyMessageBox->Close();
        }
    }

    if(bSECSGEMAlarm==true)                                                     //Ifor 20170815 (wei) add 避免出現兩個QWERTY畫面
    {
        if(fQwertyKey->Visible==true)
            bHasQwertyKeyForm=true;
        if(fPassword->Visible==true)
            bHasPasswordForm=true;
    }

    if(SystemInitialOK==true)                                                   //Steven 20121102 : 馬達還沒好,不可以下命令
        StopAllMotor();
    if(Ok || bSECSGEMAlarm || FormHS->bHaltHandler==true)                       //wei 20150817 S10F3 Alarm Reset按鍵
        MyMessageBox->pnlPause->Caption="OK";
    else
        MyMessageBox->pnlPause->Caption="Pause";

    if(bSECSGEMAlarm || FormHS->bHaltHandler==true)                             //20160624 secs訊息顯示問題
    {
        MyMessageBox->moSecsGem->Clear();
        MyMessageBox->moSecsGem->Visible=true;
        MyMessageBox->moSecsGem->Lines->Add(S1.c_str());
    }
    else
    {
        MyMessageBox->lblMainMsg->Caption=S1;
    }

    MyMessageBox->lblChineseMsg->Caption=S2;
    MyDBIProcess("Message", S1, S3);
    MyMessageBox->fScanPanel=true;
    if(IniConfig.bAlarmNeedServoOff)                                            //Steven 20110802
    {
        if(fNote->bMyServoOffInArm==false)
        {
            if(bServoOff && InArmZSafe(DETECT_ALL_FLAG)==-1)                    //如果Z軸在上才可以推
            {
                fNote->iMyServoOffInArmPosX=MOT[MInArmX].ReadEncoderPos();
                fNote->iMyServoOffInArmPosY=MOT[MInArmY].ReadEncoderPos();      //要在ServoOff之前
                MySleep(200);
                MOT[MInArmX].ServoOnOff(false);
                MOT[MInArmY].ServoOnOff(false);

                str="InArm ServoOff"+AnsiString("RecordX = ") + AnsiString(fNote->iMyServoOffInArmPosX) +"RecordY = " + AnsiString(fNote->iMyServoOffInArmPosY);
                RecordProcess(str);
                fNote->bMyServoOffInArm=true;
            }
        }
    }
    if(bEnableEmployeeIDCheck==true)                                            //等待EAP回覆時不顯示按鍵
        MyMessageBox->pnlPause->Visible=false;

    if(TestIF_File.bRENESAS_EnableFTCT==true)                                   //RogerYang 20251002 : RogerYang 瑞薩FT-CT 解綁manualstart
    {
        if(S1!=sFTCTAlarmStr)                                                   //RogerYang 20251016 : 瑞薩FT-CT 非90Alarm的message一樣要停止
            bAutoRestartAfterFTCTAlarm=false;
        fMain->RENESAS_Server->FTCTManStartUnlock(S1);
    }
    MyMessageBox->ShowModal();

    if(bEnableEmployeeIDCheck==true)                                            //等待EAP回覆時不顯示按鍵
        MyMessageBox->pnlPause->Visible=true;
}
//---------------------------------------------------------------------------
void ShowMyMessageUp(AnsiString S1, AnsiString S2, bool UseButton)              //kevin 20150203
{
    AnsiString str;
#ifdef SOFT_SIMULTE
    if(S1.Pos(" port error")!=0)
    {
        return;
    }
#endif
    MyMessageBox->pnlAlarmReset->Visible=false;                                 //wei 20150817 S10F3 Alarm Reset按鍵
    if(MyMessageBox->fShow==true && iUnLoaderCount==0)                          //JerryYang 20160830 考慮auto滿tray時show message但不停機的情況
    {
        return;
    }
    else if(MyMessageBox->fShow==true && iUnLoaderCount!=0)                     //JerryYang 20160830 當show message但不停機的情況,這時候又跳出message要把Form關掉再show,避免錯誤
    {
        MyMessageBox->Close();
    }

    if(SystemInitialOK==true)                                                   //Steven 20121102 : 馬達還沒好,不可以下命令
        StopAllMotor();

    fMain->Pause("ShowMyMessageUp");
    if(UseButton)
    {
        MyMessageBox->pnlPause->Visible=true;
        MyMessageBox->pnlPause->Caption="Pause";
    }
    else
    {
        MyMessageBox->pnlPause->Visible=false;
    }

    MyMessageBox->lblMainMsg->Caption=S1;
    MyMessageBox->lblChineseMsg->Caption=S2;
    //MyDBIProcess("Message", S1, S3);
    MyMessageBox->fScanPanel=true;
    if(TestIF_File.bRENESAS_EnableFTCT==true)                                   //RogerYang 20251002 : RogerYang 瑞薩FT-CT 解綁manualstart
    {
        bAutoRestartAfterFTCTAlarm=false;
        fMain->RENESAS_Server->FTCTManStartUnlock();
    }
    MyMessageBox->ShowModal();
}
//---------------------------------------------------------------------------
void ShowUnloaderTrayMessage(AnsiString S1, AnsiString S2)
{
    if(MyMessageBox->fShow==true)
        return;
    MyMessageBox->pnlPause->Caption="PAUSE";                                    //JerryYang 20160811 顯示PAUSE
    MyMessageBox->lblMainMsg->Caption=S1;
    MyMessageBox->lblChineseMsg->Caption=S2;
    MyMessageBox->pnlAlarmReset->Visible=false;                                 //Ifor 20160829 add 不顯示 Alarm Reset
    MyDBIProcess("Message", S1);

    if(CUSTOMER_CODE==CC_Greatek)                                               //Sam 20171213 (Steven) : 超豐良率監控
    {
        if(S1=="WAR0701"||S1=="WAR0702"||S1=="WAR0720")
             MyMessageBox->pnlMain->Color=clYellow;
    }

    if(IniConfig.bEnable_SECS_GEM==true &&
       CUSTOMER_CODE==CC_KYEC_LEE &&
       iUnLoaderCount==8)
    {
        EventReport(SECS_EVENT.PreAlarmMessage);                                //Ifor 20171024 : add Pre Alarm Position
    }

    MyMessageBox->fScanPanel=true;
    MyMessageBox->Show();
}
//---------------------------------------------------------------------------
int ShowLotEndMessage(AnsiString S1, AnsiString S2, bool bFullLotEnd)           //JerryYang 20170424 (wei) for JCET, handler要通知測試機lot end或是 full lot end
{
    if(MyMessageBox->fShow==true && iUnLoaderCount==0)                          //JerryYang 20160830 考慮auto滿tray時show message但不停機的情況
    {
        return 3;
    }
    else if(MyMessageBox->fShow==true && iUnLoaderCount!=0)                     //JerryYang 20160830 當show message但不停機的情況,這時候又跳出message要把Form關掉再show,避免錯誤
    {
        MyMessageBox->Close();
    }
    StopAllMotor();

    bDisableKeypad=true;
    bChangeLotEndMsg=true;

    MyMessageBox->pnlPause->Visible     =true;                                  //wei 20150817 S10F3 Alarm Reset按鍵
    MyMessageBox->pnlYes->Visible       =true;
    if(bFullLotEnd)                                                             //JerryYang 20170515 Tray end的時候不用顯示Full lot end
        MyMessageBox->pnlNo->Visible    =true;
    else
        MyMessageBox->pnlNo->Visible    =false;
    MyMessageBox->pnlAlarmReset->Visible=false;

    MyMessageBox->lblSubMsg->Visible    =false;

    if(TestIF_File.bRENESAS_EnableFTCT==true)                                   //RogerYang 20250923 : 瑞薩FT-CT
    {
        MyMessageBox->pnlPause->Caption     ="不供給";
        MyMessageBox->pnlYes->Caption       ="供給";
    }
    else
    {
        MyMessageBox->pnlPause->Caption     ="Skip";
        MyMessageBox->pnlYes->Caption       ="Lot end";
    }
    MyMessageBox->pnlNo->Caption        ="Full lot end";

    MyMessageBox->lblMainMsg->Caption   =S1;
    MyMessageBox->lblChineseMsg->Caption=S2;

    MyMessageBox->fScanPanel            =true;

    if(TestIF_File.bRENESAS_EnableFTCT==true)                                   //RogerYang 20251002 : RogerYang 瑞薩FT-CT 解綁manualstart
    {
        bAutoRestartAfterFTCTAlarm=false;
        fMain->RENESAS_Server->FTCTManStartUnlock();
    }
    MyMessageBox->ShowModal();

    if(iValue==1)
        S1=S1+",  Yes";
    else if(iValue==2)
        S1=S1+",  No";
    else
        S1=S1+",  Skip";

    MyDBIProcess("Message", S1);
    bDisableKeypad                  =false;
    MyMessageBox->pnlPause->Visible =true;
    MyMessageBox->pnlYes->Visible   =false;
    MyMessageBox->pnlNo->Visible    =false;
    MyMessageBox->pnlPause->Caption ="Pause";
    MyMessageBox->pnlYes->Caption   ="Yes";                                     //改回原本的名稱
    MyMessageBox->pnlNo->Caption    ="No";

    return iValue;
}
//---------------------------------------------------------------------------
//input -> str : Message
//output-> iValue : 1->Yes 2->No
//---------------------------------------------------------------------------
int ShowMyMessageBox_YES_NO(AnsiString S1, AnsiString S2, AnsiString S3)
{
    int Lengh=0, Lengh1=0;
    if(MyMessageBox->fShow==true && iUnLoaderCount==0)                          //JerryYang 20160830 考慮auto滿tray時show message但不停機的情況
    {
        return 3;
    }
    else if(MyMessageBox->fShow==true && iUnLoaderCount!=0)                     //JerryYang 20160830 當show message但不停機的情況,這時候又跳出message要把Form關掉再show,避免錯誤
    {
        MyMessageBox->Close();
    }
    StopAllMotor();

    bDisableKeypad=true;

    MyMessageBox->pnlAlarmReset->Visible=false;                                 //wei 20150817 S10F3 Alarm Reset按鍵
    MyMessageBox->pnlPause->Visible     =false;
    MyMessageBox->pnlYes->Visible       =true;
    MyMessageBox->pnlNo->Visible        =true;

    MyMessageBox->fScanPanel            =true;
    MyMessageBox->lblMainMsg->Caption   =S1;

    Lengh= S2.Pos(";");
    Lengh1=S2.Length();
    if(S2.Pos(";"))
    {
        MyMessageBox->lblSubMsg->Visible    =true;                              //kevin 20150713
        bChangeForm                         =true;                              //kevin 20150710 改變form
        MyMessageBox->lblChineseMsg->Top    =64;                                //kevin 20150821
        MyMessageBox->lblChineseMsg->Caption=S2.SubString(1,Lengh-1);
        MyMessageBox->lblSubMsg->Top        =MyMessageBox->lblChineseMsg->Top+MyMessageBox->lblChineseMsg->Height+15;
        MyMessageBox->lblSubMsg->Caption    =S2.SubString(Lengh+1,Lengh1);
    }
    else
    {
        MyMessageBox->lblSubMsg->Visible=false;                                 //kevin 20150713
        MyMessageBox->lblChineseMsg->Caption=S2;
    }

    if(TestIF_File.bRENESAS_EnableFTCT==true)                                   //RogerYang 20251002 : RogerYang 瑞薩FT-CT 解綁manualstart
    {
        bAutoRestartAfterFTCTAlarm=false;
        fMain->RENESAS_Server->FTCTManStartUnlock();
    }
    MyMessageBox->ShowModal();

    if(iValue==1)
        S1=S1+"  Yes";
    else if(iValue==2)
        S1=S1+"  No";

    MyDBIProcess("Message", S1, S3);
    MyMessageBox->pnlPause->Visible=true;
    MyMessageBox->pnlYes->Visible=false;
    MyMessageBox->pnlNo->Visible=false;

    bDisableKeypad=false;

    return iValue;
}
//---------------------------------------------------------------------------
int ShowMyMessageBox_ASEK(AnsiString S1, AnsiString S2, AnsiString S3)          //ChungHung 20130204 add for ASE_KR Socket Tester
{
    if(MyMessageBox->fShow==true && iUnLoaderCount==0)                          //JerryYang 20160830 考慮auto滿tray時show message但不停機的情況
    {
        return 3;
    }
    else if(MyMessageBox->fShow==true && iUnLoaderCount!=0)                     //JerryYang 20160830 當show message但不停機的情況,這時候又跳出message要把Form關掉再show,避免錯誤
    {
        MyMessageBox->Close();
    }
    StopAllMotor();

    bDisableKeypad                      =true;
    MyMessageBox->Caption               ="Socket Communication Error";
    MyMessageBox->pnlPause->OnClick     =MyMessageBox->pnlYesClick;

    MyMessageBox->pnlYes->Left          =63;
    MyMessageBox->pnlYes->Width         =80;
    MyMessageBox->pnlPause->Left        =195;
    MyMessageBox->pnlPause->Width       =80;
    MyMessageBox->pnlNo->Left           =319;
    MyMessageBox->pnlNo->Width          =80;

    MyMessageBox->pnlAlarmReset->Visible=false;                                 //wei 20150817 S10F3 Alarm Reset按鍵
    MyMessageBox->pnlPause->Visible     =true;
    MyMessageBox->pnlPause->Caption     ="RETRY";
    MyMessageBox->pnlYes->Visible       =true;
    MyMessageBox->pnlYes->Caption       ="CLOSE";
    MyMessageBox->pnlNo->Visible        =true;
    MyMessageBox->pnlNo->Caption        ="OFFLINE";

    MyMessageBox->fScanPanel            =true;                                  //ChungHung 20130204  沒有用到
    MyMessageBox->lblMainMsg->Caption   =S2;
    MyMessageBox->lblChineseMsg->Caption=S1;
    MyDBIProcess("Message",S2+":"+S1);
    //MyDBIProcess(S2,S1,S3);

    MyMessageBox->ShowModal();

    MyMessageBox->Caption="Message";

    MyMessageBox->pnlPause->Visible     =true;
    MyMessageBox->pnlPause->Caption     ="Pause";
    MyMessageBox->pnlYes->Visible       =false;
    MyMessageBox->pnlYes->Caption       ="Yes";
    MyMessageBox->pnlNo->Visible        =false;
    MyMessageBox->pnlNo->Caption        ="No";

    MyMessageBox->pnlYes->Left          =63;
    MyMessageBox->pnlYes->Width         =130;
    MyMessageBox->pnlPause->Left        =155;
    MyMessageBox->pnlPause->Width       =130;
    MyMessageBox->pnlNo->Left           =247;
    MyMessageBox->pnlNo->Width          =130;
    MyMessageBox->pnlPause->OnClick     =MyMessageBox->pnlPauseClick;

    bDisableKeypad                      =false;

    return iValue;
}
//---------------------------------------------------------------------------
void __fastcall TMyMessageBox::pnlYesClick(TObject *Sender)
{
    TPanel *P;
    P=(TPanel *)Sender;
    iValue = P->Tag;
    MyMessageBox->pnlPause->Visible=true;
    MyMessageBox->pnlYes->Visible=false;
    MyMessageBox->pnlNo->Visible=false;
    MyMessageBox->pnlAlarmReset->Visible=false;                                 //wei 20150817 S10F3 Alarm Reset按鍵
    Close();
}
//---------------------------------------------------------------------------
int ShowMyMessagePWD(AnsiString S1, AnsiString S2, AnsiString S3, bool Ok, bool bServoOff ,bool Password)
{
#ifdef SOFT_SIMULTE
    if(S1.Pos(" port error")!=0)
    {
        return 0;                                                               //JerryYang 20230925 : add
    }
#endif
    MyMessageBox->pnlAlarmReset->Visible=false;                                 //wei 20150817 S10F3 Alarm Reset按鍵
    if(MyMessageBox->fShow==true && iUnLoaderCount==0)                          //JerryYang 20160830 考慮auto滿tray時show message但不停機的情況
    {
        return 0;                                                               //JerryYang 20230925 : add
    }
    else if(MyMessageBox->fShow==true && iUnLoaderCount!=0)                     //JerryYang 20160830 當show message但不停機的情況,這時候又跳出message要把Form關掉再show,避免錯誤
    {
        MyMessageBox->Close();
    }

    if(SystemInitialOK==true)                                                   //Steven 20121102 : 馬達還沒好,不可以下命令
        StopAllMotor();
    if(Ok)
        MyMessageBox->pnlPause->Caption="OK";
    else
        MyMessageBox->pnlPause->Caption="Pause";
    MyMessageBox->bDisableAlarmBuzzer=true;
    bDisableKeypad=true;
    MyMessageBox->lblMainMsg->Caption=S1;
    MyMessageBox->lblChineseMsg->Caption=S2;
    MyDBIProcess("Message", S1, S3);
    MyMessageBox->fScanPanel=true;
    if(IniConfig.bAlarmNeedServoOff)                                            //Steven 20110802
    {
        if(fNote->bMyServoOffInArm==false)
        {
            if(bServoOff && InArmZSafe(DETECT_ALL_FLAG)==-1)                    //如果Z軸在上才可以推
            {
                fNote->iMyServoOffInArmPosX=MOT[MInArmX].ReadEncoderPos();
                fNote->iMyServoOffInArmPosY=MOT[MInArmY].ReadEncoderPos();      //要在ServoOff之前
                MySleep(200);
                MOT[MInArmX].ServoOnOff(false);
                MOT[MInArmY].ServoOnOff(false);
                fNote->bMyServoOffInArm=true;
            }
        }
    }
    MyMessageBox->bMBoxNeedPassword = Password;
    if(TestIF_File.bRENESAS_EnableFTCT==true)                                   //RogerYang 20251002 : RogerYang 瑞薩FT-CT 解綁manualstart
    {
        bAutoRestartAfterFTCTAlarm=false;
        fMain->RENESAS_Server->FTCTManStartUnlock();
    }
    MyMessageBox->ShowModal();
    bDisableKeypad=false;
    MyMessageBox->bDisableAlarmBuzzer=false;
    if(MyMessageBox->bMBoxNeedPassword==true)                                   //JerryYang 20230925 : add
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
//---------------------------------------------------------------------------
bool TMyMessageBox::DoPassword_MBox()
{
    bool bFlag=true;
    bool bTechComExist=FileExists(pwPath);                                      //2012-01-03    Dell modify
    int iLevel=0;
    iLevel=LevelSet.AccessLevel[35];

    if(fInput->fShow==false)
    {
        if(CUSTOMER_CODE==CC_PTI)                                               //Sam 20230921 : Bin 設定錯誤不能啟動
        {
            #ifndef SOFT_SIMULTE
            fPassword->edPassword->Text="";
            fQwertyKey->ShowQwertyKey(fPassword->edPassword, N_NO_SYMBOL|N_NO_SPACE|N_PASSWORD);
            AnsiString HonPrecPassword=CheckAndReadIniDataGeneral("VENDER", "HONPREC", AnsiString("27025312"));
            if(fPassword->edPassword->Text!=HonPrecPassword)
            {
                bFlag=false;
            }
            #else
            #endif
        }
        else
        {
            if(bTechComExist)
                fMain->cbUserSelectChange(NULL);
            else
                fMain->stOperatorClick(fMain);

            if(AccessLevel<iLevel && iLevel>0)                                  //JerryYang 20230925 : add
            {
                bFlag=false;
                if(bTechComExist)
                {
                    fPassword->Label3->Visible=true;
                    fPassword->Label4->Visible=true;
                }
            }
            else
            {
                if(bTechComExist)
                {
                    fPassword->Label3->Visible=false;
                    fPassword->Label4->Visible=false;
                }
            }

            if(bTechComExist)
            {
                fMain->btLogin->Caption="Login";
                fMain->spbUserName->Caption="Operator";
                fMain->cbUserSelect->ItemIndex=0;
                AccessLevel=0;
                fMain->ChangeLevelAttr();
            }
        }
    }

    return bFlag;
}
//---------------------------------------------------------------------------
void __fastcall TMyMessageBox::pnlAlarmResetClick(TObject *Sender)
{
    if(bAlarmBuzzer)
    {
        bAlarmReset=true;                                                       //Steven 20140905 : 紀錄有被按下Alarm Reset
        NewRecordProcess("MES2116", "ALARM RESET pressed", "MessageBox_PanelClick");
        bAlarmBuzzer=false;
        bLampAlarmReset=false;
        SECS_GEM_PPMUSIC_CONTROL_flag=false;
        SECS_GEM_PPSIGNALTOWER_CONTROL_flag=false;
        SW[SwFKAlarmReset].Off();
        SW[SwRKAlarmReset].Off();
        if(IniConfig.bEnable_SECS_GEM==true)                                    //Steven 20140528 : Secs Gem
            EventReport(SECS_EVENT.DoAlarmReset);                               //30     按下 Alarm Reset    // 2013.11.07 ); Joye); KYEC SECS/GEM
    }
}
//---------------------------------------------------------------------------
int ShowMyMessageBox_YES_SKIP(AnsiString S1, AnsiString S2, AnsiString S3)      //JerryYang 20151028 add for Amkor 新增選YES或SKIP
{
    int Lengh=0, Lengh1=0;
    if(MyMessageBox->fShow==true && iUnLoaderCount==0)                          //JerryYang 20160830 考慮auto滿tray時show message但不停機的情況
    {
        return 3;
    }
    else if(MyMessageBox->fShow==true && iUnLoaderCount!=0)                     //JerryYang 20160830 當show message但不停機的情況,這時候又跳出message要把Form關掉再show,避免錯誤
    {
        MyMessageBox->Close();
    }
    StopAllMotor();

    bDisableKeypad=true;

//--
    MyMessageBox->pnlAlarmReset->Visible=false;                                 //wei 20150817 S10F3 Alarm Reset按鍵
    MyMessageBox->pnlPause->Visible=false;
    MyMessageBox->pnlYes->Visible=true;
    MyMessageBox->pnlNo->Visible=true;
    MyMessageBox->pnlNo->Caption="Skip";
    MyMessageBox->fScanPanel=true;
    MyMessageBox->lblMainMsg->Caption=S1;
    MyMessageBox->pnlMain->Color=clRed;                                         //JerryYang 20151112 底色改成紅色
    Lengh= S2.Pos(";");
    Lengh1=S2.Length();
    if(S2.Pos(";"))
    {
        MyMessageBox->lblSubMsg->Visible=true;                                  //kevin 20150713
        bChangeForm=true;                                                       //kevin 20150710 改變form
        MyMessageBox->lblChineseMsg->Top = 64;                                  //kevin 20150821
        MyMessageBox->lblChineseMsg->Caption=S2.SubString(1, Lengh-1);
        MyMessageBox->lblSubMsg->Top = MyMessageBox->lblChineseMsg->Top+MyMessageBox->lblChineseMsg->Height+15;
        MyMessageBox->lblSubMsg->Caption=S2.SubString(Lengh+1, Lengh1);
    }
    else
    {
        MyMessageBox->lblSubMsg->Visible=false;                                 //kevin 20150713
        MyMessageBox->lblChineseMsg->Caption=S2;
    }

    if(TestIF_File.bRENESAS_EnableFTCT==true)                                   //RogerYang 20251002 : RogerYang 瑞薩FT-CT 解綁manualstart
    {
        bAutoRestartAfterFTCTAlarm=false;
        fMain->RENESAS_Server->FTCTManStartUnlock();
    }
    MyMessageBox->ShowModal();

    if(iValue==1)
        S1=S1+"  Yes";
    else if(iValue==2)
        S1=S1+"  Skip";

    MyDBIProcess("Message", S1, S3);
    MyMessageBox->pnlMain->Color=(TColor)0x00DFD9CC;                            //JerryYang 20151112 關閉時改回預設的顏色
    MyMessageBox->pnlPause->Visible=true;
    MyMessageBox->pnlYes->Visible=false;
    MyMessageBox->pnlNo->Caption="No ";
    MyMessageBox->pnlNo->Visible=false;

    bDisableKeypad=false;

    return iValue;
}
//---------------------------------------------------------------------------
//V3.27W.561 Ifor 20180227 (Steven) add SECS GEM Confirm the Employee ID
//---------------------------------------------------------------------------
void CheckEmployeeID(AnsiString S1)
{
    bWaitSecsGemReply=false;
    fPassword->Label6->Caption=S1;
    fPassword->Label6->Visible=true;
    fPassword->Label5->Caption="權限檢查!AUTHORITY CHECK!    ";
    fPassword->Label5->Visible=true;
    fPassword->ShowModal();
    asSecsGemBarCode=fPassword->edUserName->Text;
    asSecsGemBarCodePasswoed=fPassword->edPassword->Text;
    EventReport(SECS_EVENT.BarcodeReaderEnter);
    bWaitSecsGemReply=true;
    fPassword->Label6->Visible=false;
    fPassword->Label6->Caption="離線模式需要輸入 ID/Password";
    fPassword->Label5->Visible=false;
    fPassword->Label5->Caption="Off line mode need Input ID/Password";
}
//---------------------------------------------------------------------------
void __fastcall TMyMessageBox::FormShortCut(TWMKey &Msg, bool &Handled)         //Sam 20230426 : 通知系統 Handler 已經密碼鎖定
{
    if(Msg.CharCode==VK_F5)                                                     //F5 將視窗恢復初始位置
    {
        this->BringToFront();
        Left=(1024-Width)/2;
        Top =(768-Height)/2;
    }
    else if(Msg.CharCode==VK_F4)                                                //F4 關閉視窗
    {
         Close();
    }
}
//---------------------------------------------------------------------------
