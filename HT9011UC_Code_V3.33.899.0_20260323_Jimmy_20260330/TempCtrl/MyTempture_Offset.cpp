//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MyTempture_Offset.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TMyTempOffset::~TMyTempOffset()
{

}
//---------------------------------------------------------------------------
__fastcall TMyTempOffset::TMyTempOffset(TComponent* Owner) : TComponent(Owner)
{
    gbTempOffsetGroup=new TGroupBox(this);
    edTempLowBase    =new TEdit(this);
    edTempMiddBase   =new TEdit(this);
    edTempHighBase   =new TEdit(this);
    edTempUserBase   =new TEdit(this);
    pMyPanel         =new TPanel(this);

    gbTempOffsetGroup->Width    =420;
    gbTempOffsetGroup->Height   =45;
    gbTempOffsetGroup->Color    =(TColor)0x00C2B8A6;

    edTempLowBase->Parent       =gbTempOffsetGroup;
    edTempLowBase->Width        =90;
    edTempLowBase->Height       =21;
    edTempLowBase->Left         =10;
    edTempLowBase->Top          =16;
    edTempLowBase->Text         ="0";

    edTempMiddBase->Parent      =gbTempOffsetGroup;
    edTempMiddBase->Width       =90;
    edTempMiddBase->Height      =21;
    edTempMiddBase->Left        =110;
    edTempMiddBase->Top         =16;
    edTempMiddBase->Text        ="0";

    edTempHighBase->Parent      =gbTempOffsetGroup;
    edTempHighBase->Width       =87;
    edTempHighBase->Height      =21;
    edTempHighBase->Left        =210;
    edTempHighBase->Top         =16;
    edTempHighBase->Text        ="0";

    edTempUserBase->Parent      =gbTempOffsetGroup;
    edTempUserBase->Width       =90;
    edTempUserBase->Height      =21;
    edTempUserBase->Left        =320;
    edTempUserBase->Top         =16;
    edTempUserBase->BorderStyle =bsNone;
    edTempUserBase->Color       =clBtnFace;
    edTempUserBase->Text        ="0";

    pMyPanel->Parent            =gbTempOffsetGroup;
    pMyPanel->Width             =1;
    pMyPanel->Height            =32;
    pMyPanel->Left              =310;
    pMyPanel->Top               =10;

    iTag=0;
    bEnable=false;
}
//---------------------------------------------------------------------------
void TMyTempOffset::SetPosition(int iTop, int iLeft)
{
    gbTempOffsetGroup->Top=iTop;
    gbTempOffsetGroup->Left=iLeft;
}
//---------------------------------------------------------------------------
void TMyTempOffset::SetVisable(bool bVisable)
{
    bEnable=bVisable;
    gbTempOffsetGroup->Visible=bVisable;
}
//---------------------------------------------------------------------------
void TMyTempOffset::SetPName(AnsiString Name)                          //設定名稱
{
    gbTempOffsetGroup->Caption=Name;
    Name=StringReplace(Name," ","",TReplaceFlags()<<rfReplaceAll);      //砍掉空格
    Name=StringReplace(Name,"/","",TReplaceFlags()<<rfReplaceAll);      //砍掉斜線
    edTempLowBase->Name=Name+"_Low";
    edTempMiddBase->Name=Name+"_Mid";
    edTempHighBase->Name=Name+"_High";
    edTempUserBase->Name=Name+"_User";
}
//---------------------------------------------------------------------------
