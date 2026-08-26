//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MyTempture_MainPanel.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
TColor myColor[]={(TColor)clYellow, (TColor)0x0025AB12, (TColor)0x008000FF};
__fastcall TMyTempMainPanel::~TMyTempMainPanel()
{
}
//---------------------------------------------------------------------------
__fastcall TMyTempMainPanel::TMyTempMainPanel(TComponent* Owner) : TComponent(Owner)
{
    plTempMain=new TPanel(this);
    plTempName=new TPanel(this);
    plTempValue=new TPanel(this);

    plTempMain->BevelInner=bvNone;
    plTempMain->BevelOuter=bvNone;

    plTempName->Parent=plTempMain;
    plTempName->Color=(TColor)0x00E9AC3A;
    plTempName->BevelInner=bvLowered;
    plTempName->BevelOuter=bvRaised;

    plTempValue->Parent=plTempMain;
    plTempValue->Color=myColor[1];
    plTempValue->BevelInner=bvLowered;
    plTempValue->BevelOuter=bvRaised;

    SetSize(57, 52);
    plTempMain->AutoSize=true;
    iTag=0;
    bEnable=false;
    dUpperBound=0.0;
    dLowerBound=0.0;
    bChangeColor=false;
    bAlarm=false;
}
//---------------------------------------------------------------------------
void TMyTempMainPanel::SetSize(int iWidth, int iHeight)
{
    plTempName->Width=iWidth;
    plTempName->Height=(iHeight-4)/2;
    plTempName->Top=2;
    plTempName->Left=0;

    plTempValue->Width=iWidth;
    plTempValue->Height=(iHeight-4)/2;
    plTempValue->Top=plTempName->Height+2;
    plTempValue->Left=0;

    if(iHeight<60)
    {
        plTempName->BorderStyle=bsNone;
        plTempName->BevelInner=bvNone;
        plTempName->BevelOuter=bvNone;

        plTempValue->BorderStyle=bsNone;
        plTempValue->BevelInner=bvNone;
        plTempValue->BevelOuter=bvNone;
    }
}
//---------------------------------------------------------------------------
void TMyTempMainPanel::SetPosition(int iTop, int iLeft)
{
    plTempMain->Top=iTop;
    plTempMain->Left=iLeft;
}
//---------------------------------------------------------------------------
void TMyTempMainPanel::SetFontSize(int iSize)
{
    plTempName->Font->Size=iSize;
    plTempValue->Font->Size=iSize;
}
//---------------------------------------------------------------------------
void TMyTempMainPanel::SetVisable(bool bVisable)
{
    bEnable=bVisable;
    plTempMain->Visible=bVisable;
}
//---------------------------------------------------------------------------
void TMyTempMainPanel::SetPName(AnsiString Name)                        //設定名稱
{
    plTempName->Caption=Name;
}
//---------------------------------------------------------------------------
void TMyTempMainPanel::SetValue(double Value, bool bMonitor)                           //設定數字
{
    AnsiString str;

    if(bMonitor)
        str.sprintf("%5.1f ℃", Value);
    else
        str.sprintf("---");

    plTempValue->Caption=str;

    if(bChangeColor)
    {
        if(Value>999)
        {
            plTempValue->Caption="ERR";
            plTempValue->Color=myColor[2];
            bAlarm=true;
        }
        else if(Value>dUpperBound)
        {
            plTempValue->Color=myColor[2];
            bAlarm=true;
        }
        else if(Value<dLowerBound)
        {
            plTempValue->Color=myColor[0];
            bAlarm=true;
        }
        else
        {
            plTempValue->Color=myColor[1];
            bAlarm=false;
        }
    }
    else
    {
        plTempValue->Color=myColor[1];
        bAlarm=false;
    }
}
//---------------------------------------------------------------------------
