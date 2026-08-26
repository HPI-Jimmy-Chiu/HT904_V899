#include "MachineDefine.h"
#pragma hdrstop

#include "login.h"
#include "cmydef.h"
#include "ProductionInfo.h"                                                     //Sam 20170824 (Steven) 移植超豐 ESD Control 功能 form HT-7045
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfLogin *fLogin;
//---------------------------------------------------------------------------
__fastcall TfLogin::TfLogin(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfLogin::FormShow(TObject *Sender)
{
    Left=(1024-Width)/2;
    Top =(768-Height)/2;

    edUserName->Text="";
    edLoginOldPassword->Text="";
    edLoginNewPassword->Text="";

    //jou 2011-11-24 修正login必須分兩種模式
    if(FileExists(pwPath))          //2012-01-03    Dell modify
    {
        labNewPassword->Visible=false;
        edLoginNewPassword->Visible=false;

        if(CUSTOMER_CODE==CC_AMKOR_China || CUSTOMER_CODE==CC_QUALCOMM)   //JerryYang 20170412 (Steven) add QUALCOMM
        {
            if(AccessLevel<=iDefEngineerLevel)  //jou 2014-06-19 Security Have 5 Level 1->iDefEngineerLevel
            {
                rgLoginOption->ItemIndex=2;
                rgLoginOption->Visible=false;
                rgLoginOptionClick(this);
            }
            else
            {
                rgLoginOption->ItemIndex=0;
                rgLoginOption->Visible=true;
            }
        }
        else
        {
            rgLoginOption->ItemIndex=0;
        }
    }
    else
    {
        rgLoginOption->Visible=false;
    }
    //Sam 20170824 (Steven) 移植超豐 ESD Control 功能 form HT-7045
    //==>
    if(CUSTOMER_CODE==CC_Greatek)
    {
        if(IniConfig.bN15UseESDControlMachine==true)
        {
            fProductionInfo->ScreenkeyboardShow();
        }
    }
    //<==
    //Sam 20170824 (Steven) 移植超豐 ESD Control 功能 form HT-7045
    edLoginOldPassword->SetFocus();              //2012-01-03    Dell Fix
}
//---------------------------------------------------------------------------
void __fastcall TfLogin::sbOkClick(TObject *Sender)
{
    if(cbLoginUserName->Visible)
        edUserName->Text=cbLoginUserName->Text;
    sbOk->Down=false;
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfLogin::rgLoginOptionClick(TObject *Sender)
{
    if(rgLoginOption->ItemIndex==1)        //2012-01-03    Dell modify 刪除不需要password
    {
        labOldPassword->Visible=false;
        edLoginOldPassword->Visible=false;
    }
    else
    {
        labOldPassword->Visible=true;
        edLoginOldPassword->Visible=true;
    }

    if(rgLoginOption->ItemIndex==2)
    {
        labNewPassword->Visible=true;
        edLoginNewPassword->Visible=true;
        labOldPassword->Caption ="OldPassword";
    }
    else
    {
        labNewPassword->Visible=false;
        edLoginNewPassword->Visible=false;
        labOldPassword->Caption ="Password";
    }
}
//---------------------------------------------------------------------------
