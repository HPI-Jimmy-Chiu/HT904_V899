//---------------------------------------------------------------------------
//用CC Link通訊來操作Keyence的Line Sensor
//---------------------------------------------------------------------------
#include "MachineDefine.h"
#pragma hdrstop

#include "MyCCLink.h"
#include "MyCCLinkSensor.h"
#include "common.h"
#pragma package(smart_init)
//------------------------------------------------------------------------------
// 建構子
//------------------------------------------------------------------------------
__fastcall TMyCCLinkCtrl::TMyCCLinkCtrl(TComponent* Owner) : TComponent(Owner)
{
    CCLinkBoard=new TMyCCLinkBoard(fCCLink);
    iCCLinkTask=1;
    bReady=false;
    Timer1=new TTimer(fCCLink);
    Timer1->OnTimer=Timer1Timer;
    Timer1->Interval=30;
    Timer1->Enabled=true;
};
//------------------------------------------------------------------------------
// 解構子
//------------------------------------------------------------------------------
__fastcall TMyCCLinkCtrl::~TMyCCLinkCtrl()
{
    try
    {
        Timer1->Enabled=false;
        MySleep(100);
        if(CCLinkBoard!=NULL)
            delete CCLinkBoard;
    }
    catch(...)
    {
        MyDBIProcess("Exception", "~TMyCCLinkCtrl");
    }
}
//------------------------------------------------------------------------------
extern const unsigned char T_HEX2ASCII[16];
extern const unsigned char T_ASXII2HEX[23];
extern unsigned char T_HEX2ASCII_Mac(unsigned char hex2ascii);
extern unsigned char T_ASXII2HEX_Mac(unsigned char ascii2hex);
//---------------------------------------------------------------------------
void __fastcall TMyCCLinkCtrl::WriteCCLinkStatus(bool Value)
{
    iCCLinkStatus=Value;
    if(Value==true)
    {
        bHasErr=false;
        ErrMess="";
        iCCLinkTask=3;
    }
};
//---------------------------------------------------------------------------
void TMyCCLinkCtrl::DoSetValue(short sNu, short sSen, int Value)                //針對單一Sensor設定值
{
    CCLinkBoard->iDoSetValueTask=1;
    CCLinkBoard->sSetNu=sNu;                                                    //要被設定的NU-CL1
    CCLinkBoard->sSetSen=sSen;                                                  //要被設定的Sensor
    CCLinkBoard->iSetValue=Value;                                               //要被設定的值
    bSetValue=true;
}
//---------------------------------------------------------------------------
void TMyCCLinkCtrl::DoSetMultiValue(short sNu, short sSen, int *Value, int Count)    //針對多個Sensor設定值
{
    if(bSetMultiValue==true)                                                    //jou 20221004 : 避免重複進入DoSetMultiValue
        return;

    memset(CCLinkBoard->iSetMultiValue, 0, sizeof(CCLinkBoard->iSetMultiValue));    //wei 20180622  CCLink Value Save File
    CCLinkBoard->iDoSetMultiValueTask=1;
    CCLinkBoard->sSetNu=sNu;                                                    //要被設定的NU-CL1
    CCLinkBoard->sSetSen=sSen;                                                  //要被設定的Sensor
    for(int i=0; i<Count; i++)
        CCLinkBoard->iSetMultiValue[i]=Value[i];                                //要被設定的值
    bSetMultiValue=true;
}
//------------------------------------------------------------------------------
void __fastcall TMyCCLinkCtrl::Timer1Timer(TObject *Sender)
{
    if(bInitialOK==false)                                                       //jou 2010-05-19 start : 未initital完成,不能執行
        return;

    if(bHasErr==true)
        return;

    int &Task=iCCLinkTask;
    CCLinkBoard->Path=lPath;
    switch(Task)
    {
        case 1:
            CCLinkBoard->iCCLinkBoardTask=1;
            bReady=false;
            Task=2;
        case 2:
            SysMess="Connecting CC-Link Board.";
            if(CCLinkBoard->DoConnectBoard())                                   //初始化連線
            {
                iNucl1No=CCLinkBoard->ConnectModuleNo;
                bHasErr=false;
                ErrMess="";
                Task=50;
            }
            if(CCLinkBoard->HasError)
            {
                bHasErr=true;
                ErrMess=CCLinkBoard->ErrorMessage;
            }
            break;
        case 3:
            SysMess="Resetting CC-Link Board.";
            CCLinkBoard->ResetBoard();
            CCLinkTimer.Set0_1SecAndOn(200);
            Task=4;
            iCCLinkStatus=false;
            break;
        case 4:
            if(CCLinkTimer.Off())
                Task=1;
            break;
        case 50:                                                                //鎖定所有Sensor的按鍵
            CCLinkBoard->SensorKeyLock=true;
            Task=60;
            break;
        case 60:
            SysMess="Doing Sensor Key Lock.";
            if(CCLinkBoard->SensorKeyLock==true)
            {
                bHasErr=false;
                ErrMess="";
                CCLinkBoard->iReadSetValueTask=1;
                Task=70;
            }
            if(CCLinkBoard->HasError)
            {
                bHasErr=true;
                ErrMess=CCLinkBoard->ErrorMessage;
            }
            break;
        case 70:
            SysMess="Reading Set Values.";
            if(CCLinkBoard->DoReadSetValue())                                   //讀取Sensor的門檻值
            {
                bHasErr=false;
                ErrMess="";
                Task=100;
            }
            if(CCLinkBoard->HasError)
            {
                bHasErr=true;
                ErrMess=CCLinkBoard->ErrorMessage;
                Task=1;
            }
            break;
        case 100:                                                               //檢查系統狀態 (Wr0000==0)
            if(CCLinkBoard->DoCheckWr0000())                                    //讀取Sensor資料
            {
                if(CCLinkBoard->HasError)
                {
                    bHasErr=true;
                    ErrMess=CCLinkBoard->ErrorMessage;
                    Task=1;
                }
                else
                {
                    bHasErr=false;
                    ErrMess="";
                    Task=110;
                }
            }
            if(CCLinkBoard->HasError)
            {
                bHasErr=true;
                ErrMess=CCLinkBoard->ErrorMessage;
                Task=1;
            }
            break;
        case 110:
            bReady=true;
            if(bSetValue)
                Task=2000;
            else if(bSetMultiValue)
                Task=3000;
            else if(fCCLink->bNewFile)                                          //wei 20180622 (Steven) : CCLink Value Save File
                Task=4000;
            else
                Task=1000;
            break;
        case 1000:
            SysMess="Reading Current Values.";
            if(CCLinkBoard->DoReadCurrValue())                                  //讀取Sensor資料
            {
                bHasErr=false;
                ErrMess="";
                Task=1500;
            }
            if(CCLinkBoard->HasError)
            {
                bHasErr=true;
                ErrMess=CCLinkBoard->ErrorMessage;
                Task=1;
            }
            break;
        case 1500:
            SysMess="Reading On Off Status.";
            if(CCLinkBoard->DoReadOnOff())                                      //讀取Sensor資料
            {
                bHasErr=false;
                ErrMess="";
                Task=100;
            }

            if(CCLinkBoard->HasError)
            {
                bHasErr=true;
                ErrMess=CCLinkBoard->ErrorMessage;
                Task=1;
            }
            break;
        case 2000:
            SysMess="Setting Value.";
            if(CCLinkBoard->DoSetValue())
            {
                bHasErr=false;
                ErrMess="";
                bSetValue=false;
                Task=100;
            }
            if(CCLinkBoard->HasError)
            {
                bHasErr=true;
                ErrMess=CCLinkBoard->ErrorMessage;
                Task=1;
            }
            break;
        case 3000:
            SysMess="Setting Value.";
            if(CCLinkBoard->DoSetMultiValue())
            {
                bHasErr=false;
                ErrMess="";
                bSetMultiValue=false;
                Task=70;
            }
            if(CCLinkBoard->HasError)
            {
                bHasErr=true;
                ErrMess=CCLinkBoard->ErrorMessage;
                Task=1;
            }
            break;

        case 4000:                                                              //wei 20180622  CCLink Value Save File
            fCCLink->SaveSetupFile();
            fCCLink->bNewFile=false;
            Task=100;
            break;
    }
}
//------------------------------------------------------------------------------
