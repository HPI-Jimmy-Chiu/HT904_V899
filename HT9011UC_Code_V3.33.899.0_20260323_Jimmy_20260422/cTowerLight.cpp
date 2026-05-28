#include "MachineDefine.h"
#pragma hdrstop

#include "cTowerLight.h"
#include "cprod.h"
#include "cpublic.h"
#include "myswitch.h"
#include "cmydef.h"
//#include "myio.h"
#include "MyLaneIo.h"
#include "mymessbox.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ALed"
#pragma resource "*.dfm"
TfTowerLight *fTowerLight;

//---------------------------------------------------------------------------
__fastcall TfTowerLight::TfTowerLight(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfTowerLight::UpdateTowerLed(int Attr)
{
    static int ct=0;
    static bool flag=false;
    if(ct++<5 && Attr==0)
        return;
    ct=0;
    flag=!flag;                                                                 //MessageLight最多就8組, 不能再加!!
    static TALed *LedPtr[8][3]=                                                 //Steven 20140409 : Auto Retest
    {
        {RGB00, RGB01, RGB02},
        {RGB10, RGB11, RGB12},
        {RGB20, RGB21, RGB22},
        {RGB30, RGB31, RGB32},
        {RGB40, RGB41, RGB42},
        {RGB50, RGB51, RGB52},
        {RGB60, RGB61, RGB62},
        {RGB70, RGB71, RGB72},
    };

    for(int i=0; i<8; i++)
    {
        for(int j=0; j<3; j++)
        {
            if(     LastSet.MessageLight[i][j]==0)  LedPtr[i][j]->Value=false;
            else if(LastSet.MessageLight[i][j]==1)  LedPtr[i][j]->Value=true;
            else if(LastSet.MessageLight[i][j]==2)  LedPtr[i][j]->Value=flag;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfTowerLight::RGB00Click(TObject *Sender)
{
    if(IniConfig.bA02DisableSaveParsWhenSwitchToOp==true &&                     //RogerYang 20260305 張寧說時間到都要登出，且不可被更改任何設定(A01_2)
        AccessLevel==0)
    {
        ShowMyMessage("[A01_2]目前已切換到Operator權限，\r\n請重新登入再做設定!");
        Close();
    }

    TALed *Ptr;
    Ptr=(TALed *) Sender;
    int i=Ptr->Tag/3;
    int j=Ptr->Tag%3;

    int iOfflineRun=7;                                                          //JerryYang 20230721 : LD/ULD區分不同的異常音樂
    if(IniConfig.bSPILFunction==true)                                           //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction
        iOfflineRun=5;

    if(i>iOfflineRun || i<0 || j>2 || j<0)
        return;
    LastSet.MessageLight[i][j]++;
    if(LastSet.MessageLight[i][j]>2)
        LastSet.MessageLight[i][j]=0;
    WriteLastDataFile();
    UpdateTowerLed(1);
}
//---------------------------------------------------------------------------
void __fastcall TfTowerLight::FormShow(TObject *Sender)
{
    TComboBox *Ptr[8]={cbRunning, cbJam, cbPause, cbMessage, cbHeating,
                       cbHome, cbOffLine, cbART};

    for(int i=0; i<8; i++)                                                      //Steven 20140409 : Auto Retest
    {
        if(LastSet.MusicSelect[i]<0)
            LastSet.MusicSelect[i]=0;
        if(LastSet.MusicSelect[i]>4)
            LastSet.MusicSelect[i]=4;

        Ptr[i]->ItemIndex=LastSet.MusicSelect[i];
    }
    rgMusicTest->ItemIndex=0;
    fShow=true;
    Left=75;                                                                    //Steven 20091103
    Top=10;                                                                     //Steven 20091103
    Timer1->Enabled=true;

    if((CUSTOMER_CODE==CC_KYEC_XILINX &&
        CosFunction.bAutoRetestGPIBmode==true) ||                               //Frank 20160728 add
       CosFunction.bTowerLightUseLD==true)                                      //JerryYang 20230721 : LD/ULD區分不同的異常音樂
    {
        Panel11->Visible=true;
        if(CosFunction.bTowerLightUseLD==true)
        {
            palART->Caption="LD/ULD";
        }
        else
        {
            palART->Caption="Auto Retest";
        }
    }
    else
    {
        Panel11->Visible=false;
    }

    if(IniConfig.bSPILFunction==true)                                           //JerryYang 20170328 (Jou) 矽品客戶碼統一用SPILFunction
        cbOffLine->Enabled=false;                                               //jou 2014-03-28 SPIL Handler  On-line & Offline Switch Flow

    //這一行請保持在最下面!!-----------------
    myLog.Do_Log(Sender, asUser, asLogPath);                                    //Steven 20100629
}
//---------------------------------------------------------------------------
void __fastcall TfTowerLight::FormClose(TObject *Sender, TCloseAction &Action)
{
    TComboBox *Ptr[8]={cbRunning, cbJam, cbPause, cbMessage, cbHeating,
                       cbHome, cbOffLine, cbART};
    for(int i=0; i<8; i++)                                                      //Steven 20140409 : Auto Retest
        LastSet.MusicSelect[i]=Ptr[i]->ItemIndex;
    fShow=false;
    Timer1->Enabled=false;

    //這一行請保持在最下面!!-----------------
    myLog.Do_Log(Sender, asUser, asLogPath);                                    //Steven 20100629
}
//---------------------------------------------------------------------------
void __fastcall TfTowerLight::rgMusicTestClick(TObject *Sender)
{
    for(int i=0; i<4; i++)  SW[SwMusic1+i].Off();
    if(rgMusicTest->ItemIndex>=1 && rgMusicTest->ItemIndex<=4)
        SW[SwMusic1+rgMusicTest->ItemIndex-1].On();
}
//---------------------------------------------------------------------------
void __fastcall TfTowerLight::Timer1Timer(TObject *Sender)
{
    if(fShow==false)
        return;
    UpdateTowerLed(0);
}
//---------------------------------------------------------------------------
void __fastcall TfTowerLight::spbExitClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
