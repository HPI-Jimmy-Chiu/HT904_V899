#include "MachineDefine.h"
#pragma hdrstop


#include "MyOmronPanel.h"
#include "myQwertyKeyBoard.h"
#include "cmydef.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TMyOmronPanel::TMyOmronPanel(TComponent* Owner) : TComponent(Owner)
{
    GroupBox        =new TGroupBox(this);
    edSV            =new TEdit(this);                                           //設定值
    ImgOmronPanel   =new TImage(this);
    cbEnableAT      =new TCheckBox(this);

    GroupBox->Left      =0;
    GroupBox->Top       =0;
    GroupBox->Width     =206;
    GroupBox->Height    =120;
    GroupBox->DragKind  =dkDock;
    GroupBox->OnMouseUp =GroupBox1MouseUp;
    GroupBox->OnMouseDown=GroupBox1MouseDown;
    GroupBox->OnMouseMove=GroupBox1MouseMove;

    ImgOmronPanel->Parent=GroupBox;
    ImgOmronPanel->Top=20;
    ImgOmronPanel->Left=4;
    ImgOmronPanel->Width=197;
    ImgOmronPanel->Height=95;

    ImgOmronPanel->Canvas->Font->Name ="Arial";
    SetBG        (clGray, clGray);                                              //設定背景
    SetPV        ("##.##",       clYellow);                                     //顯示量測值
    SetDegree    ("℃",          clYellow);                                     //顯示量測單位
    SetRunStop   ("STOP",        clBlack);                                      //顯示目前運作模式
    SetAT        ("AT",          clBlack);                                      //顯示AT是否啟動
    SetInputErr  ("Input Error", clBlack);                                      //感溫線異常
    SetEvent     ("Event",       clBlack);                                      //警報事件
    SetSP        ("SP",          TColor(0x004080FF));                           //目前設定值

    edSV->Parent=GroupBox;
    edSV->AutoSize=false;
    edSV->Left = 72;
    edSV->Top = 85;
    edSV->Width = 128;
    edSV->Height = 28;
    edSV->BorderStyle=bsNone;
    edSV->Text = "0";
    edSV->Font->Charset = DEFAULT_CHARSET;
    edSV->Font->Color = clBlack;
    edSV->Font->Size = 18;
    edSV->Font->Name = "Arial";
    edSV->ParentFont = false;
    edSV->OnClick=setEditValueClick;

    cbEnableAT->Parent  =GroupBox;
    cbEnableAT->Left    =48;
    cbEnableAT->Top     =76;
    cbEnableAT->Width   =17;
    cbEnableAT->Height  =17;
    cbEnableAT->Color   =clGray;
    cbEnableAT->Caption ="";
    dPv=dSettingSV=0.0;
}
//---------------------------------------------------------------------------
__fastcall TMyOmronPanel::~TMyOmronPanel()
{

}
//---------------------------------------------------------------------------
void __fastcall TMyOmronPanel::GroupBox1MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TGroupBox *Ptr;
    Ptr=(TGroupBox *)Sender;
    if(Ptr->Tag!=0)
    {
        Ptr->Tag=0;
    }
}
//---------------------------------------------------------------------------
void __fastcall TMyOmronPanel::GroupBox1MouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
    TGroupBox *Ptr;
    Ptr=(TGroupBox *)Sender;
    if(Ptr->Tag==1)
    {
        Ptr->Left=Ptr->Left+(X-iStartX);
        Ptr->Top =Ptr->Top+(Y-iStartY);
    }
}
//---------------------------------------------------------------------------
void __fastcall TMyOmronPanel::GroupBox1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TGroupBox *Ptr;
    Ptr=(TGroupBox *)Sender;
    if(Ptr->Tag!=1)
        Ptr->Tag=1;
    iStartX=X;
    iStartY=Y;
}
//---------------------------------------------------------------------------
void __fastcall TMyOmronPanel::SetPV(AnsiString PV, TColor Color)               //顯示量測值
{
    int iTextWidth, iTextHeight;
    //溫度--------------
    ImgOmronPanel->Canvas->Font->Color=Color;
    ImgOmronPanel->Canvas->Font->Size=36;
    iTextHeight=ImgOmronPanel->Canvas->TextHeight(PV);

    iTextWidth=120;                                                             //Ifor 20241129 add: 避免文字大小不同殘影問題
    ImgOmronPanel->Canvas->TextOutA(((129-iTextWidth)/2)+72, (44-iTextHeight)/2, "    ");
    iTextWidth=ImgOmronPanel->Canvas->TextWidth(PV);

    ImgOmronPanel->Canvas->TextOutA(((129-iTextWidth)/2)+72, (44-iTextHeight)/2, PV);
    dPv=atof(PV.c_str());                                                       //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
}
//---------------------------------------------------------------------------
void __fastcall TMyOmronPanel::SetDegree(AnsiString Degree, TColor Color)       //顯示量測單位
{
    int iTextWidth, iTextHeight;
    //單位---------------
    ImgOmronPanel->Canvas->Font->Color=Color;
    ImgOmronPanel->Canvas->Font->Size=10;
    iTextWidth=ImgOmronPanel->Canvas->TextWidth(Degree);
    iTextHeight=ImgOmronPanel->Canvas->TextHeight(Degree);
    ImgOmronPanel->Canvas->TextOutA(((69-iTextWidth)/2), (19-iTextHeight)/2, Degree);
}
//---------------------------------------------------------------------------
void __fastcall TMyOmronPanel::SetRunStop(AnsiString RunStop, TColor Color)     //顯示目前運作模式
{
    int iTextWidth, iTextHeight;
    //Run / Stop---------------
    ImgOmronPanel->Canvas->Font->Color=Color;
    ImgOmronPanel->Canvas->Font->Size=10;
    iTextWidth=ImgOmronPanel->Canvas->TextWidth(RunStop);
    iTextHeight=ImgOmronPanel->Canvas->TextHeight(RunStop);
    ImgOmronPanel->Canvas->TextOutA(((69-iTextWidth)/2), (19-iTextHeight)/2+20, RunStop);
}
//---------------------------------------------------------------------------
void __fastcall TMyOmronPanel::SetAT(AnsiString AT, TColor Color)               //顯示AT是否啟動
{
    int iTextWidth, iTextHeight;
    //AT---------------
    ImgOmronPanel->Canvas->Font->Color=Color;
    ImgOmronPanel->Canvas->Font->Size=10;
    iTextWidth=ImgOmronPanel->Canvas->TextWidth(AT);
    iTextHeight=ImgOmronPanel->Canvas->TextHeight(AT);
    ImgOmronPanel->Canvas->TextOutA(((69-iTextWidth)/2), (19-iTextHeight)/2+58, AT);
}
//---------------------------------------------------------------------------
void __fastcall TMyOmronPanel::SetInputErr(AnsiString InputErr, TColor Color)   //感溫線異常
{
    int iTextWidth, iTextHeight;
    //Input Error---------------
    ImgOmronPanel->Canvas->Font->Color=Color;
    ImgOmronPanel->Canvas->Font->Size=10;
    iTextWidth=ImgOmronPanel->Canvas->TextWidth(InputErr);
    iTextHeight=ImgOmronPanel->Canvas->TextHeight(InputErr);
    ImgOmronPanel->Canvas->TextOutA(((69-iTextWidth)/2), (19-iTextHeight)/2+39, InputErr);
}
//---------------------------------------------------------------------------
void __fastcall TMyOmronPanel::SetEvent(AnsiString Event, TColor Color)         //警報事件
{
    int iTextWidth, iTextHeight;
    //事件---------------
    ImgOmronPanel->Canvas->Font->Color=Color;
    ImgOmronPanel->Canvas->Font->Size=10;
    if(Event=="Event")                                                          //Ifor 20241129 add: 避免文字大小不同殘影問題
    {
        iTextWidth=80;
        iTextHeight=16;
        ImgOmronPanel->Canvas->TextOutA(((129-iTextWidth)/2)+72, (21-iTextHeight)/2+44, "                      ");
    }
    iTextWidth=ImgOmronPanel->Canvas->TextWidth(Event);
    iTextHeight=ImgOmronPanel->Canvas->TextHeight(Event);
    ImgOmronPanel->Canvas->TextOutA(((129-iTextWidth)/2)+72, (21-iTextHeight)/2+44, Event);

}
//---------------------------------------------------------------------------
void __fastcall TMyOmronPanel::SetSP(AnsiString SP, TColor Color)               //目前設定值
{
    int iTextWidth, iTextHeight;
    //SP---------------
    ImgOmronPanel->Canvas->Font->Color=Color;
    ImgOmronPanel->Canvas->Font->Size=10;
    iTextWidth=ImgOmronPanel->Canvas->TextWidth(SP);
    iTextHeight=ImgOmronPanel->Canvas->TextHeight(SP);
    ImgOmronPanel->Canvas->TextOutA(((69-iTextWidth)/2), (19-iTextHeight)/2+77, SP);
}
//---------------------------------------------------------------------------
void __fastcall TMyOmronPanel::SetBG(TColor OutLine, TColor BackGround)         //設定背景
{
    ImgOmronPanel->Canvas->Brush->Color=BackGround;
    ImgOmronPanel->Canvas->Pen->Color  =OutLine;
    ImgOmronPanel->Canvas->Rectangle(TRect(0, 0,ImgOmronPanel->Width, ImgOmronPanel->Height));
}
//---------------------------------------------------------------------------
void TMyOmronPanel::SetVisible(bool bSW)                                        //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
{
    GroupBox->Visible=bSW;
}
//---------------------------------------------------------------------------
void TMyOmronPanel::SetSettingSV(double dsv)                                    //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
{
    dSettingSV=dsv;
    edSV->Text=AnsiString(dSettingSV);
}
//---------------------------------------------------------------------------
void TMyOmronPanel::SetSV(double dsv)                                           //JimmyChiu 20210923 : Index使用DTME08版16組加熱器
{
    dSV=dsv;
    SetSP(AnsiString().sprintf("%03.1f", dSV), TColor(0x004080FF));
}
//---------------------------------------------------------------------------
void __fastcall TMyOmronPanel::setEditValueClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 0, 180);      //小鍵盤
}
//---------------------------------------------------------------------------

