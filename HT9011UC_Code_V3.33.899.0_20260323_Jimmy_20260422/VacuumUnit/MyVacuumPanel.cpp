#include "MachineDefine.h"
#pragma hdrstop

#include "MyVacuumPanel.h"
#include "myQwertyKeyBoard.h"
#include "cmydef.h"
#include "VacuumUnit.h"
#include "AdvMotDrv.h"
#include "AdvMotApi.h"
#include "MyKitSuck.h"
#include "MyLaneIo.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
TfVacuumUnit *_fVacuumUnit;
//---------------------------------------------------------------------------
__fastcall TMyVacuumPanel::TMyVacuumPanel(TComponent* Owner, int _Kind ,int _iCol,int _iRow) : TComponent(Owner)
{
    AnsiString str="",str1="",str2="",str3="";

    AnsiString sIndexName_16[2][8]={{"    Aa    ","    Ab    ","    Ac    ","    Ad    ","    Ae    ","    Af    ","    Ag    ","    Ah    "},
                                    {"    Ba    ","    Bb    ","    Bc    ","    Bd    ","    Be    ","    Bf    ","    Bg    ","    Bh    "}};

    AnsiString sInOutName_8[2][4]={{"     A    ","     C    ","     E    ","     G    "},
                                   {"     B    ","     D    ","     F    ","     H    "}};

    //HT9046LS  IndexArm1 2 排序不同，所以分開兩個


    int iIndexVCSort1_16[2][8]={{0,2,4,6,0,2,4,6},
                                {1,3,5,7,1,3,5,7}};

    int iIndexVCSort2_16[2][8]={{3,1,7,5,3,1,7,5},
                                {2,0,6,4,2,0,6,4}};

    int iInOutVCSort[2][4]={{0,1,2,3},
                            {4,5,6,7}};

    iKind=_Kind;      //0:IndexArm1 ; 1:IndexArm2 ; 2:InArm ; 3:OutArm
    iCol=_iCol;
    iRow=_iRow;

    //Ring
    iOnRing=1;
    iOffRing=1;
    iSenRing=1;
    //Type
    iOnType=1;
    iOffType=1;
    iSenType=1;
    //ISABase
    iOnISABase=ePCI1203;
    iOffISABase=ePCI1203;
    iSenISABase=ePCI1203;

    if(iKind==0)         //IndexArm1 HT9046LS
    {
        iVacuOnOffSwap=1;
        str.sprintf("%s", sIndexName_16[iRow][iCol]);
        //IP
        if(iCol<=3)
        {
            iOnIP=0x40;
            iOffIP=0x40;
            iSenIP=0x40;
        }
        else
        {
            iOnIP=0x41;
            iOffIP=0x41;
            iSenIP=0x41;
        }
        //VCNo
        iOnVCNo=iIndexVCSort1_16[iRow][iCol];
        iOffVCNo=iIndexVCSort1_16[iRow][iCol];
        iSenVCNo=iIndexVCSort1_16[iRow][iCol];
    }
    else if(iKind==1)    //IndexArm2 HT9046LS
    {
        iVacuOnOffSwap=0;
        str.sprintf("%s", sIndexName_16[iRow][iCol]);
        //IP
        if(iCol<=3)
        {
            iOnIP=0x50;
            iOffIP=0x50;
            iSenIP=0x50;
        }
        else
        {
            iOnIP=0x51;
            iOffIP=0x51;
            iSenIP=0x51;
        }
        //VCNo
        iOnVCNo=iIndexVCSort2_16[iRow][iCol];
        iOffVCNo=iIndexVCSort2_16[iRow][iCol];
        iSenVCNo=iIndexVCSort2_16[iRow][iCol];
    }
    else if(iKind==2)    //InArm
    {
        iVacuOnOffSwap=1;
        str.sprintf("%s", sInOutName_8[iRow][iCol]);
        //IP
        iOnIP=0x20;
        iOffIP=0x20;
        iSenIP=0x20;
        //VCNo
        iOnVCNo=iInOutVCSort[iRow][iCol];
        iOffVCNo=iInOutVCSort[iRow][iCol];
        iSenVCNo=iInOutVCSort[iRow][iCol];
    }
    else if(iKind==3)    //OutArm
    {
        iVacuOnOffSwap=1;
        str.sprintf("%s", sInOutName_8[iRow][iCol]);
        //IP
        iOnIP=0x30;
        iOffIP=0x30;
        iSenIP=0x30;
        //VCNo
        iOnVCNo=iInOutVCSort[iRow][iCol];
        iOffVCNo=iInOutVCSort[iRow][iCol];
        iSenVCNo=iInOutVCSort[iRow][iCol];
    }
    else if(iKind==4)    //IndexArm1 HT9045
    {
        iVacuOnOffSwap=1;
        str.sprintf("%s", sIndexName_16[iRow][iCol]);
        //IP
        iOnIP=0x40;
        iOffIP=0x40;
        iSenIP=0x40;
        //VCNo
        iOnVCNo=iIndexVCSort2_16[iRow][iCol]; //HT9045 的排序剛好是 HT9046LS Arm2 排序，只接拿來用
        iOffVCNo=iIndexVCSort2_16[iRow][iCol]; //HT9045 的排序剛好是 HT9046LS Arm2 排序，只接拿來用
        iSenVCNo=iIndexVCSort2_16[iRow][iCol]; //HT9045 的排序剛好是 HT9046LS Arm2 排序，只接拿來用
    }
    else if(iKind==5)    //IndexArm2 HT9045
    {
        iVacuOnOffSwap=1;
        str.sprintf("%s", sIndexName_16[iRow][iCol]);
        //IP
        iOnIP=0x50;
        iOffIP=0x50;
        iSenIP=0x50;
        //VCNo
        iOnVCNo=iIndexVCSort2_16[iRow][iCol]; //HT9045 的排序剛好是 HT9046LS Arm2 排序，只接拿來用
        iOffVCNo=iIndexVCSort2_16[iRow][iCol]; //HT9045 的排序剛好是 HT9046LS Arm2 排序，只接拿來用
        iSenVCNo=iIndexVCSort2_16[iRow][iCol]; //HT9045 的排序剛好是 HT9046LS Arm2 排序，只接拿來用
    }
    _fVacuumUnit=(TfVacuumUnit *)Owner;
    GroupBox        =new TGroupBox(this);
    ImgVacuumPanel  =new TImage(this);
    edSV            =new TEdit(this);
    btnSV           =new TSpeedButton(this);
    myld1           =new TMyLed(this);
    bplOn           =new TBtnPanelLane(this);
    bplOff          =new TBtnPanelLane(this);

    GroupBox->Visible   =false;
    GroupBox->Width     =VACUUM_UNIT_WIDTH;
    GroupBox->Height    =VACUUM_UNIT_HEIGHT;
    GroupBox->Color     =clBtnFace;
    GroupBox->Caption=str;

    //背景
    ImgVacuumPanel->Parent=GroupBox;
    ImgVacuumPanel->Top=20;
    ImgVacuumPanel->Left=4;
    ImgVacuumPanel->Width=VACUUM_UNIT_WIDTH-8;
    ImgVacuumPanel->Height=VACUUM_UNIT_HEIGHT-25;
    ImgVacuumPanel->Canvas->Font->Name ="Arial";
    SetBG        (clGray, _fVacuumUnit->pnlCurectVal->Color);                   //設定背景
    ShowCurectVal ("0.0", _fVacuumUnit->pnlCurectVal->Font->Color);             //顯示目前氣壓值
    ShowThreshold ("0.0", _fVacuumUnit->pnlThreshold->Font->Color);             //顯示目前氣壓值下閥值
    SetEvent     ("Event",       clBlack);                                      //警報事件

    //閥值輸入
    edSV->Parent        =GroupBox;
    edSV->Top           =_fVacuumUnit->edSV->Top;
    edSV->Left          =_fVacuumUnit->edSV->Left;
    edSV->Width         =_fVacuumUnit->edSV->Width;
    edSV->Height        =_fVacuumUnit->edSV->Height;
    edSV->Text          =_fVacuumUnit->edSV->Text;
    edSV->Font->Name    =_fVacuumUnit->edSV->Font->Name;
    edSV->Font->Size    =_fVacuumUnit->edSV->Font->Size;
    edSV->Font->Charset =_fVacuumUnit->edSV->Font->Charset;
    edSV->BorderStyle   =_fVacuumUnit->edSV->BorderStyle;
    edSV->OnClick       =edSVClick;

    //閥值設定
    btnSV->Parent   =GroupBox;
    btnSV->Top      =_fVacuumUnit->btnSV->Top;
    btnSV->Left     =_fVacuumUnit->btnSV->Left;
    btnSV->Width    =_fVacuumUnit->btnSV->Width;
    btnSV->Height   =_fVacuumUnit->btnSV->Height;
    btnSV->Caption  =_fVacuumUnit->btnSV->Caption;
    btnSV->OnClick  =btnSVClick;

    //閥值 On/Off
    myld1->Parent   =GroupBox;
    myld1->Top      =_fVacuumUnit->myld1->Top;
    myld1->Left     =_fVacuumUnit->myld1->Left;
    myld1->Width    =_fVacuumUnit->myld1->Width;
    myld1->Height   =_fVacuumUnit->myld1->Height;
    myld1->ShowHint =true;
    iSenPort=128+iSenVCNo;
    str.sprintf("(Lane:%d,IP:%d,Port:%d,Bit:%d) VCNo%d", iSenRing,iSenIP,iSenPort,iSenVCNo,iSenVCNo);
    myld1->Hint     =str;

    int iSwap1=0,iSwap2=1;
    if(iVacuOnOffSwap==1)
    {
        iSwap1=1;
        iSwap2=0;
    }
    //吸真空
    bplOn->Parent           =GroupBox;
    bplOn->Left             =_fVacuumUnit->bplOn->Left;
    bplOn->Top              =_fVacuumUnit->bplOn->Top;
    bplOn->Width            =_fVacuumUnit->bplOn->Width;
    bplOn->Height           =_fVacuumUnit->bplOn->Height;
    bplOn->Caption          =_fVacuumUnit->bplOn->Caption;
    bplOn->Style            =_fVacuumUnit->bplOn->Style;
    bplOn->Color            =_fVacuumUnit->bplOn->Color;
    bplOn->TrueColor        =_fVacuumUnit->bplOn->TrueColor;
    bplOn->TrueFontColor    =_fVacuumUnit->bplOn->TrueFontColor;
    bplOn->FalseColor       =_fVacuumUnit->bplOn->FalseColor;
    bplOn->FalseFontColor   =_fVacuumUnit->bplOn->FalseFontColor;
    bplOn->OnClick          =btnVaccumOnOffOnClick;
    bplOn->ShowHint         =true;
    iOnPort=16+iOnVCNo*2+iSwap1;
    str.sprintf("(Lane:%d,IP:%d,Port:%d,Bit:%d) VCNo%d", iOnRing,iOnIP,iOnPort,iOnVCNo,iOnVCNo);
    bplOn->Hint             =str;

    //吸真空
    bplOff->Parent          =GroupBox;
    bplOff->Left            =_fVacuumUnit->bplOff->Left;
    bplOff->Top             =_fVacuumUnit->bplOff->Top;
    bplOff->Width           =_fVacuumUnit->bplOff->Width;
    bplOff->Height          =_fVacuumUnit->bplOff->Height;
    bplOff->Caption         =_fVacuumUnit->bplOff->Caption;
    bplOff->Style           =_fVacuumUnit->bplOff->Style;
    bplOff->Color           =_fVacuumUnit->bplOff->Color;
    bplOff->TrueColor       =_fVacuumUnit->bplOff->TrueColor;
    bplOff->TrueFontColor   =_fVacuumUnit->bplOff->TrueFontColor;
    bplOff->FalseColor      =_fVacuumUnit->bplOff->FalseColor;
    bplOff->FalseFontColor  =_fVacuumUnit->bplOff->FalseFontColor;
    bplOff->OnClick         =btnVaccumOnOffOnClick;
    bplOff->ShowHint        =true;
    iOffPort                =16+iOffVCNo*2+iSwap2;
    str.sprintf("(Lane:%d,IP:%d,Port:%d,Bit:%d) VCNo%d", iOffRing,iOffIP,iOffPort,iOffVCNo,iOffVCNo);
    bplOff->Hint             =str;

    IO_Index=0x6100;
    IO_SubIndex=0x01;

    CurValue_Index=0x6000;   //有問題
    CurValue_SubIndex=0x01;

    Threshold_Index=0x8000;
    Threshold_SubIndex=0x13;
    ThresholdMode_SubIndex=0x02;

    DataSize=128;
    bInitialThresholdModeOK=false;
    bNeedReadVaccumThreshold=true;
    dOldCurrectVal=0.0;
    dCurrectVal=0.0;
    dThresholdVal=0.0;
    sEvent="";
    sOldEvent="Init";
}
//---------------------------------------------------------------------------
__fastcall TMyVacuumPanel::~TMyVacuumPanel()
{
    //delete GroupBox;
    //delete ImgVacuumPanel;
    //delete edSV;
    //delete btnSV;
    //delete myld1;
    //delete bplOn;
    //delete bplOff;
}
//---------------------------------------------------------------------------
void __fastcall TMyVacuumPanel::ShowCurectVal(AnsiString CurectVal , TColor Color)  //顯示目前氣壓值
{
    if(GroupBox->Visible==false)
        return;
    int iTextWidth=0, iTextHeight=0,iTextOutX=0,iTextOutY=0;
    int iLeft=0,iTop=0,iHeight=0,iWidth=0;
    AnsiString sEmpty="          ";  //顯示前先塞空白字串避免殘影
    ImgVacuumPanel->Canvas->Font->Color=Color;
    ImgVacuumPanel->Canvas->Font->Size=_fVacuumUnit->pnlCurectVal->Font->Size;
    ImgVacuumPanel->Canvas->Font->Style =_fVacuumUnit->pnlCurectVal->Font->Style;

    iLeft       =_fVacuumUnit->pnlCurectVal->Left;
    iTop        =_fVacuumUnit->pnlCurectVal->Top-10;
    iHeight     =_fVacuumUnit->pnlCurectVal->Height;
    iWidth      =_fVacuumUnit->pnlCurectVal->Width;

    iTextWidth  =ImgVacuumPanel->Canvas->TextWidth(sEmpty);
    iTextHeight =ImgVacuumPanel->Canvas->TextHeight(sEmpty);
    iTextOutX   =iLeft+(iWidth-iTextWidth)/2;
    iTextOutY   =iTop-(iHeight-iTextHeight)/2;
    ImgVacuumPanel->Canvas->TextOutA(iTextOutX, iTextOutY, sEmpty);

    iTextWidth  =ImgVacuumPanel->Canvas->TextWidth(CurectVal);
    iTextHeight =ImgVacuumPanel->Canvas->TextHeight(CurectVal);
    iTextOutX   =iLeft+(iWidth-iTextWidth)/2;
    iTextOutY   =iTop-(iHeight-iTextHeight)/2;
    ImgVacuumPanel->Canvas->TextOutA(iTextOutX, iTextOutY, CurectVal);
}
//---------------------------------------------------------------------------
void __fastcall TMyVacuumPanel::ShowThreshold(AnsiString ThresholdVal, TColor Color)  //顯示目前氣壓值閥值
{
    if(GroupBox->Visible==false)
        return;

    int iTextWidth=0, iTextHeight=0,iTextOutX=0,iTextOutY=0;
    int iLeft=0,iTop=0,iHeight=0,iWidth=0;
    AnsiString sEmpty="          ";  //顯示前先塞空白字串避免殘影
    ImgVacuumPanel->Canvas->Font->Color=Color;
    ImgVacuumPanel->Canvas->Font->Size=_fVacuumUnit->pnlThreshold->Font->Size;
    ImgVacuumPanel->Canvas->Font->Charset=_fVacuumUnit->pnlThreshold->Font->Charset;

    iLeft       =_fVacuumUnit->pnlThreshold->Left;
    iTop        =_fVacuumUnit->pnlThreshold->Top-20;
    iHeight     =_fVacuumUnit->pnlThreshold->Height;
    iWidth      =_fVacuumUnit->pnlThreshold->Width;

    iTextWidth  =ImgVacuumPanel->Canvas->TextWidth(sEmpty);
    iTextHeight =ImgVacuumPanel->Canvas->TextHeight(sEmpty);
    iTextOutX   =iLeft+(iWidth-iTextWidth)/2;
    iTextOutY   =iTop-(iHeight-iTextHeight)/2;
    ImgVacuumPanel->Canvas->TextOutA(iTextOutX, iTextOutY, sEmpty);

    iTextWidth  =ImgVacuumPanel->Canvas->TextWidth(ThresholdVal);
    iTextHeight =ImgVacuumPanel->Canvas->TextHeight(ThresholdVal);
    iTextOutX   =iLeft+(iWidth-iTextWidth)/2;
    iTextOutY   =iTop-(iHeight-iTextHeight)/2;
    ImgVacuumPanel->Canvas->TextOutA(iTextOutX, iTextOutY, ThresholdVal);
}
//---------------------------------------------------------------------------
void __fastcall TMyVacuumPanel::SetEvent(AnsiString Event, TColor Color)        //警報事件
{
    if(GroupBox->Visible==false)
        return;
    int iTextWidth=0, iTextHeight=0,iTextOutX=0,iTextOutY=0;
    int iLeft=0,iTop=0,iHeight=0,iWidth=0;
    AnsiString sEmpty="          ";  //顯示前先塞空白字串避免殘影
    ImgVacuumPanel->Canvas->Font->Color=Color;
    ImgVacuumPanel->Canvas->Font->Size=_fVacuumUnit->pnlEvent->Font->Size;
    ImgVacuumPanel->Canvas->Font->Charset=_fVacuumUnit->pnlEvent->Font->Charset;

    iLeft       =_fVacuumUnit->pnlEvent->Left;
    iTop        =_fVacuumUnit->pnlEvent->Top-10;
    iHeight     =_fVacuumUnit->pnlEvent->Height;
    iWidth      =_fVacuumUnit->pnlEvent->Width;

    iTextWidth  =ImgVacuumPanel->Canvas->TextWidth(sEmpty);
    iTextHeight =ImgVacuumPanel->Canvas->TextHeight(sEmpty);
    iTextOutX   =iLeft+(iWidth-iTextWidth)/2;
    iTextOutY   =iTop-(iHeight-iTextHeight)/2;
    ImgVacuumPanel->Canvas->TextOutA(iTextOutX, iTextOutY, sEmpty);

    iTextWidth  =ImgVacuumPanel->Canvas->TextWidth(Event);
    iTextHeight =ImgVacuumPanel->Canvas->TextHeight(Event);
    iTextOutX   =iLeft+(iWidth-iTextWidth)/2;
    iTextOutY   =iTop-(iHeight-iTextHeight)/2;
    ImgVacuumPanel->Canvas->TextOutA(iTextOutX, iTextOutY, Event);
}
//---------------------------------------------------------------------------
void __fastcall TMyVacuumPanel::SetBG(TColor OutLine, TColor BackGround)        //設定背景
{
    ImgVacuumPanel->Canvas->Brush->Color=BackGround;
    ImgVacuumPanel->Canvas->Pen->Color  =OutLine;
    ImgVacuumPanel->Canvas->Rectangle(TRect(0, 0,ImgVacuumPanel->Width, ImgVacuumPanel->Height));
}
//---------------------------------------------------------------------------
void __fastcall TMyVacuumPanel::edSVClick(TObject *Sender)                      //輸入閥值
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_DOUBLE, 0, true, -116.0, 148.0); //小鍵盤
}
//---------------------------------------------------------------------------
void __fastcall TMyVacuumPanel::btnSVClick(TObject *Sender)                     //設定閥值
{
    dWriteThresholdVal=atof(edSV->Text.c_str());
    WriteVaccumThreshold(dWriteThresholdVal);
}
//---------------------------------------------------------------------------
void __fastcall TMyVacuumPanel::btnVaccumOnOffOnClick(TObject *Sender)                     //吸真空開關
{
    TBtnPanelLane *Ptr;
    Ptr=(TBtnPanelLane *) Sender;
    int ret=0;
    Ptr->Down=!Ptr->Down;
    if(Ptr->Down)
    {
        if(Ptr->Caption=="^")   //吸真空開
            ret=Acm_DaqDoSetBitEx(uiDevhand, iOnRing, iOnIP, iOnPort, (iOnType==1)?1:0);
        else                    //破真空開
            ret=Acm_DaqDoSetBitEx(uiDevhand, iOffRing, iOffIP, iOffPort, (iOffType==1)?1:0);
    }
    else
    {
        if(Ptr->Caption=="^")   //吸真空關
            ret=Acm_DaqDoSetBitEx(uiDevhand, iOnRing, iOnIP, iOnPort, (iOnType==1)?0:1);
        else                    //破真空關
            ret=Acm_DaqDoSetBitEx(uiDevhand, iOffRing, iOffIP, iOffPort, (iOffType==1)?0:1);

    }
    ChangeBtnPanelLane(Ptr);

    if(ret!=0)
    {

    }    
}
//---------------------------------------------------------------------------
void __fastcall TMyVacuumPanel::RefreshCurrectVal()
{
    if(sEvent!="")
        return;

    AnsiString s="";
    dCurrectVal=ReadVaccumCurrect();
    if(dOldCurrectVal!=dCurrectVal)
    {
        dOldCurrectVal=dCurrectVal;
        s.sprintf("%0.1f",dCurrectVal);
        ShowCurectVal(s,_fVacuumUnit->pnlCurectVal->Font->Color);
    }
}
//---------------------------------------------------------------------------
void __fastcall TMyVacuumPanel::RefreshThresholdVal()
{
    if(sEvent!="")
        return;

    AnsiString s="";
    if(bNeedReadVaccumThreshold)
    {
        dThresholdVal=ReadVaccumThreshold();
        s.sprintf("%0.1f",dThresholdVal);
        ShowThreshold(s,_fVacuumUnit->pnlThreshold->Font->Color);
    }
}
//---------------------------------------------------------------------------
void __fastcall TMyVacuumPanel::RefreshVaccumIO()
{
    if(sEvent!="")
        return;

    myld1->Value=ReadVaccumIO();
}
//---------------------------------------------------------------------------
void __fastcall TMyVacuumPanel::RefreshEvent()
{
    if(sOldEvent!=sEvent)
    {
        sOldEvent=sEvent;
        if(sEvent=="")
            SetEvent("OK",clGreen);
        else
            SetEvent(sEvent,clRed);
    }
}
//---------------------------------------------------------------------------
void __fastcall TMyVacuumPanel::RefreshDOIO()
{
    int Result1=0,Result2=0;
    U8 Value1, Value2;

    //ECAT-VC8 DO0-DO15 16-35
    Result1=Acm_DaqDoGetBitEx(uiDevhand, iOnRing, iOnIP, iOnPort,  &Value1);
    Result2=Acm_DaqDoGetBitEx(uiDevhand, iOffRing, iOffIP, iOffPort, &Value2);


    if(Result1==SUCCESS)
    {
        if(Value1>0)
            bplOn->Down=(iOnType==1)?true:false;
        else
            bplOn->Down=(iOnType==1)?false:true;
    }
    if(Result2==SUCCESS)
    {
        if(Value2>0)
            bplOff->Down=(iOffType==1)?true:false;
        else
            bplOff->Down=(iOffType==1)?false:true;
    }
    ChangeBtnPanelLane(bplOn);
    ChangeBtnPanelLane(bplOff);
}
//---------------------------------------------------------------------------
void TMyVacuumPanel::ChangeBtnPanelLane(TBtnPanelLane *Ptr)
{
    if(Ptr->Down)
    {
        Ptr->Color          =Ptr->TrueColor;
        Ptr->Font->Color    =Ptr->TrueFontColor;
        Ptr->BevelInner     =bvLowered;
        Ptr->BevelOuter     =bvLowered;
    }
    else
    {
        Ptr->Color          =Ptr->FalseColor;
        Ptr->Font->Color    =Ptr->FalseFontColor;
        if(Ptr->Style==tsFlatButtons)
        {
            Ptr->BevelInner =bvNone;
            Ptr->BevelOuter =bvNone;
        }
        else
        {
            Ptr->BevelInner =bvRaised;
            Ptr->BevelOuter =bvRaised;
        }
    }
}
//---------------------------------------------------------------------------
double __fastcall TMyVacuumPanel::ReadVaccumCurrect()
{
    return MyLaneIO.GetIOValue(iSenRing,iSenIP,iSenVCNo,0,iSenISABase);
}
//---------------------------------------------------------------------------
double __fastcall TMyVacuumPanel::ReadVaccumThreshold()
{
    bNeedReadVaccumThreshold=false;
    return MyLaneIO.GetIOValueThread(iSenRing,iSenIP,iSenVCNo,0,iSenISABase);
}
//---------------------------------------------------------------------------
bool __fastcall TMyVacuumPanel::ReadVaccumIO()
{
    int Result=0;
    U8 Value;
    //ECAT-VC8 DI0-DI7 128-135
    Result=Acm_DaqDiGetBitEx(uiDevhand, iSenRing, iSenIP, iSenPort, &Value);
    if(Result==SUCCESS)
    {
        if(Value>0)
            return true;
        else
            return false;
    }
    else
    {
        sEvent="Error3";
        return false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TMyVacuumPanel::WriteVaccumThreshold(double dKpa)
{
    if(MyLaneIO.SetIOValueThread(dKpa,iSenRing,iSenIP,iSenVCNo,0,iSenISABase))
        bNeedReadVaccumThreshold=true;
}
//---------------------------------------------------------------------------
void TMyVacuumPanel::InitialThresholdMode()
{
    unsigned long Result;
    short iValue=1;   //0:關閉 ; 1:低於設定閥值 ; 2:高於設定閥值 ; 3:等於設定閥值
    if(bInitialThresholdModeOK==false)
    {
        Result=Acm_DevWriteSDOData(uiDevhand, iSenRing, iSenIP, Threshold_Index+iSenVCNo*0x0010, ThresholdMode_SubIndex,  ECAT_TYPE_I16, DataSize, &iValue);
        if(Result==SUCCESS)
            bInitialThresholdModeOK=true;
        else
            sEvent="Error5";
    }
}
//---------------------------------------------------------------------------
void TMyVacuumPanel::Reset()
{
    sEvent="";
    sOldEvent="Reset";
    SetEvent("Reset",clYellow);
    dOldCurrectVal=0.0;
    bInitialThresholdModeOK=false;
    bNeedReadVaccumThreshold=true;
}
//---------------------------------------------------------------------------
