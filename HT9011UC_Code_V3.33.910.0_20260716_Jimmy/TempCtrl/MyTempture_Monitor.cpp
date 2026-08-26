//---------------------------------------------------------------------------
//監控溫度
//單獨溫度Range設定
//溫控器監控與否。
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "MyTempture_Monitor.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
__fastcall TMyTempMonitor::~TMyTempMonitor()
{
}
//---------------------------------------------------------------------------
__fastcall TMyTempMonitor::TMyTempMonitor(TComponent* Owner) : TComponent(Owner)
{
    gbTempGroup=new TGroupBox(this);
    lbTempLevel=new TListBox(this);
    pbTemp     =new TProgressBar(this);
    edTempValue=new TEdit(this);
    cbEnable   =new TCheckBox(this);
    edTempRange=new TEdit(this);

    lbTempLevel->Parent=gbTempGroup;
    pbTemp     ->Parent=gbTempGroup;
    edTempValue->Parent=gbTempGroup;
    cbEnable   ->Parent=gbTempGroup;
    edTempRange->Parent=gbTempGroup;

    gbTempGroup->Color =(TColor)0x00C2B8A6;
    gbTempGroup->Height=481;
    gbTempGroup->Width =80;
    gbTempGroup->Font->Size=10;

    lbTempLevel->Height=382;
    lbTempLevel->Width =33;
    lbTempLevel->Left  =18;
    lbTempLevel->Top   =17;
    lbTempLevel->Font->Size=8;

    pbTemp->Height  =382;
    pbTemp->Width   =15;
    pbTemp->Left    =52;
    pbTemp->Top     =17;
    pbTemp->Max     =155;
    pbTemp->Min     =-5;
    pbTemp->Position=-5;

    edTempValue->Height  =24;
    edTempValue->Width   =40;
    edTempValue->Left    =21;
    edTempValue->Top     =401;
    edTempValue->Enabled =false;
    edTempValue->Color   =(TColor)0x00C2B8A6;
    edTempValue->Font->Size=10;
    edTempValue->Text    ="0";
//    edTempValue->Font->Color=clWhite;

    cbEnable->Height  =17;
    cbEnable->Width   =40;
    cbEnable->Left    =23;
    cbEnable->Top     =427;
    cbEnable->Font->Size=8;
    cbEnable->Caption="On";
    cbEnable->Checked =true;
    cbEnable->Visible=false;

    edTempRange->Height  =24;
    edTempRange->Width   =40;
    edTempRange->Left    =21;
    edTempRange->Top     =449;
    edTempRange->Font->Size=10;
    edTempRange->Text    ="0";

    iTag=0;
    bEnable=false;
}
//---------------------------------------------------------------------------
void TMyTempMonitor::SetPosition(int iTop, int iLeft)
{
    gbTempGroup->Top=iTop;
    gbTempGroup->Left=iLeft;
}
//---------------------------------------------------------------------------
void TMyTempMonitor::SetVisable(bool bVisable)
{
    bEnable=bVisable;
    gbTempGroup->Visible=bVisable;
}
//---------------------------------------------------------------------------
void TMyTempMonitor::SetPName(AnsiString Name)                          //設定名稱
{
    gbTempGroup->Caption=Name;
    Name=StringReplace(Name," ","",TReplaceFlags()<<rfReplaceAll);      //砍掉空格
    Name=StringReplace(Name,"/","",TReplaceFlags()<<rfReplaceAll);      //砍掉斜線
    edTempRange->Name=Name+"_SingleRange";
    cbEnable->Name=Name+"_EnableSetting";
}
//---------------------------------------------------------------------------
void TMyTempMonitor::SetValue(double Value)                        //設定數字
{
    pbTemp->Position=Value;
    edTempValue->Text=AnsiString(Value);
}
//---------------------------------------------------------------------------
void TMyTempMonitor::SetLevel(int Max, int Min)                    //設定範圍
{
    int i, iValue;
    AnsiString str;
    pbTemp->Max     =Max;
    pbTemp->Min     =Min;

    iValue=Max-15;
    for(i=0; i<=(Max-Min)/10; i++)
    {
        str.sprintf("%3d -------   ", iValue);
        lbTempLevel->Items->Add(str);
        iValue-=10;

        if(iValue<Min)
            break;
        lbTempLevel->Items->Add("-------------");   //最後一排不要畫線
    }
}
//---------------------------------------------------------------------------
