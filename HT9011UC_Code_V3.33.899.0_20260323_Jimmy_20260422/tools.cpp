#include "MachineDefine.h"
#pragma hdrstop

#include "tools.h"

#include "MyLaneIo.h"
#include "cpublic.h"
#include "common.h"
//#include "INPUT.h"
#include "cmydef.h"
#include "myQwertyKeyBoard.h"
#include "csystem.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#pragma link "ALed"
#pragma resource "*.dfm"
TFTool *FTool;
//---------------------------------------------------------------------------
__fastcall TFTool::TFTool(TComponent* Owner)
    : TForm(Owner)
{
    fShow=false;
    TimeTick=20;
    TimeCT=0;
}
//---------------------------------------------------------------------------
void __fastcall TFTool::FormShow(TObject *Sender)
{
    Left=(1024-Width)/2;
    Top =(768-Height)/2;

    SpeedButton2->Down=false;
    fShow=true;
    TimeCT=0;
}
//---------------------------------------------------------------------------
void __fastcall TFTool::FormClose(TObject *Sender, TCloseAction &Action)
{
    fShow=false;
}
//---------------------------------------------------------------------------
void __fastcall TFTool::Timer1Timer(TObject *Sender)
{
    if(!fShow || SpeedButton2->Down==false)
        return;

//******************************************************************************
//
//  注意!! CheckSafeDoorIsClosed為Handler 安全門相關, 修改時要小心!!
//
//******************************************************************************
    if(CheckSafeDoorIsClosed()==false)
    {
        SpeedButton2->Down=false;
        EnableIOChange(SpeedButton2->Down);
        return;
    }

    TimeCT++;
    static bool Flag=false;

    static int i, j,k,Port1,Port2;
    static int Lane[2],IP[2];
    static char Mask[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
    char Byte1=0,Byte2=0;

    TALed *LPtr1[8]={
        ALed1,ALed2 ,ALed3 ,ALed4 ,ALed5 ,ALed6 ,ALed7 ,ALed8 };
    TALed *LPtr2[8]={
        ALed9,ALed10,ALed11,ALed12,ALed13,ALed14,ALed15,ALed16};

    Lane[0] =atoi(edtLane1->Text.c_str());
    Lane[1] =atoi(edtLane3->Text.c_str());
    IP[0] =atoi(edtIP1->Text.c_str());
    IP[1] =atoi(edtIP3->Text.c_str());

    Port1=atoi(edtPort1->Text.c_str());
    Port2=atoi(edtPort3->Text.c_str());
    if(edtLane1->Text!="" && edtIP1->Text!="" && edtPort1->Text!="")
        Byte1=MyLaneIO.IOInputByte(Lane[0],IP[0],Port1);
    else
        Byte1=0;
    if(edtLane3->Text!="" && edtIP3->Text!="" && edtPort3->Text!="")
        Byte2=MyLaneIO.IOInputByte(Lane[1],IP[1],Port2);
    else
        Byte2=0;

    for(i=0; i<8; i++)
    {
        if(Byte1 & Mask[i])
            LPtr1[i]->Value=true;
        else
            LPtr1[i]->Value=false;

        if(Byte2 & Mask[i])
            LPtr2[i]->Value=true;
        else
            LPtr2[i]->Value=false;
    }

    if(TimeCT<TimeTick)
        return;
    TimeCT=0;
    Flag=!Flag;
    static TCheckBox *Ptr1[16]={
        CheckBox1  ,CheckBox2,
        CheckBox3  ,CheckBox4,
        CheckBox5  ,CheckBox6,
        CheckBox7  ,CheckBox8,
        CheckBox9  ,CheckBox10,
        CheckBox11 ,CheckBox12,
        CheckBox13 ,CheckBox14,
        CheckBox15 ,CheckBox16
    };

    static TCheckBox *Ptr2[16]={
        CheckBox17 ,CheckBox18,
        CheckBox26 ,CheckBox25,
        CheckBox27 ,CheckBox24,
        CheckBox28 ,CheckBox23,
        CheckBox29 ,CheckBox22,
        CheckBox30 ,CheckBox21,
        CheckBox31 ,CheckBox20,
        CheckBox32 ,CheckBox19
    };

    Byte1=0;
    Byte2=0;
    Lane[0] =atoi(edtLane0->Text.c_str());
    Lane[1] =atoi(edtLane2->Text.c_str());
    IP[0] =atoi(edtIP0->Text.c_str());
    IP[1] =atoi(edtIP2->Text.c_str());
    Port1=atoi(edtPort0->Text.c_str());
    Port2=atoi(edtPort2->Text.c_str());
    for(i=0; i<8; i++)
    {
        j=i*2;
        k=j+1;

        if(Ptr1[j]->Checked)
        {
            if(Ptr1[k]->Checked)
            {
                if(Flag)
                    Byte1|=Mask[i];
                else
                    Byte1&=~Mask[i];
            }
            else
            {
                Byte1|=Mask[i];
            }
        }
        else
        {
            Byte1&=~Mask[i];
        }

        if(Ptr2[j]->Checked)
        {
            if(Ptr2[k]->Checked)
            {
                if(Flag)
                    Byte2|=Mask[i];
                else
                    Byte2&=~Mask[i];
            }
            else
            {
                Byte2|=Mask[i];
            }
        }
        else
        {
            Byte2&=~Mask[i];
        }
    }

    bool bOK=true;
    if(edtPort0->Text!="" && edtLane0->Text!="" && edtIP0->Text!="")
        bOK=MyLaneIO.IOByteOut(Lane[0], IP[0], Port1, Byte1);
//    if(bOK==false)  //jou 2013-04-09 修正 IO tools form keyin wrong address then show alarm message can not close.
//    {
//        SpeedButton2->Down=false;
//        EnableIOChange(SpeedButton2->Down);
//        return;
//    }

    bOK=true;
    if(edtLane2->Text!="" && edtIP2->Text!="" && edtPort2->Text!="")
        bOK=MyLaneIO.IOByteOut(Lane[1], IP[1], Port2, Byte2);
    if(bOK==false)  //jou 2013-04-09 修正 IO tools form keyin wrong address then show alarm message can not close.
    {
//        SpeedButton2->Down=false;
//        EnableIOChange(SpeedButton2->Down);
//        return;
    }
}
//---------------------------------------------------------------------------
void __fastcall TFTool::ComboBox1Change(TObject *Sender)
{
/*
    if(ComboBox1->Text =="0.1")
        TimeTick=2;
    else if(ComboBox1->Text =="0.3")
        TimeTick=6;
    else if(ComboBox1->Text =="0.5")
        TimeTick=10;
    else if(ComboBox1->Text =="0.6")
        TimeTick=12;
    else if(ComboBox1->Text =="1")
        TimeTick=20;
    else if(ComboBox1->Text =="2")
        TimeTick=40;
    else if(ComboBox1->Text =="3")
        TimeTick=60;
    else
        TimeTick=20;
*/
    double iWaitTime=atof(ComboBox1->Text.c_str()); //Steven 20120921 : int -> double, atoi -> atof
    iWaitTime=iWaitTime*1000.0;
    TimeTick=ChangeToFloatNonPcnt((double)(iWaitTime), (double)(Timer1->Interval));
    if(TimeTick<0)
        TimeTick=20;
}
//---------------------------------------------------------------------------
void __fastcall TFTool::EnableIOChange(bool bFlag)
{
    bool Flag=!bFlag;
    edtPort0->Enabled=Flag;
    edtPort2->Enabled=Flag;
    edtPort1->Enabled=Flag;
    edtPort3->Enabled=Flag;
    edtLane0->Enabled=Flag;
    edtLane2->Enabled=Flag;
    edtLane1->Enabled=Flag;
    edtLane3->Enabled=Flag;
    edtIP0->Enabled=Flag;
    edtIP2->Enabled=Flag;
    edtIP1->Enabled=Flag;
    edtIP3->Enabled=Flag;
}
//---------------------------------------------------------------------------
void __fastcall TFTool::SpeedButton2Click(TObject *Sender)
{
    EnableIOChange(SpeedButton2->Down);
}
//---------------------------------------------------------------------------
void __fastcall TFTool::edtPort0KeyPress(TObject *Sender, char &Key)
{
    if(OnlyNumberInPut(Key)==false)
        Key=NULL;
}
//---------------------------------------------------------------------------
void __fastcall TFTool::sbExitClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TFTool::edtLane0Click(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 3, 0);
}
//---------------------------------------------------------------------------
void __fastcall TFTool::edtIP0Click(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 63, 0);
}
//---------------------------------------------------------------------------
void __fastcall TFTool::edtPort0Click(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 7, 0);
}
//---------------------------------------------------------------------------
