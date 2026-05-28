#include "MachineDefine.h"
#pragma hdrstop

#include "AutoTemperature.h"

#include "cpublic.h"
#include "cprod.h"
#include "cMyDef.h"
#include "uTemp_Set.h"
#include "Note.h"
#include "main.h"
#include "common.h"
#include "myQwertyKeyBoard.h"
//#include "RS232.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SPComm"
#pragma resource "*.dfm"
TACTForm *ACTForm;
//extern AnsiString asTempCtrl[tcTotalCount];
int  iChannelCount=40;
//---------------------------------------------------------------------------
__fastcall TMyATPanel::TMyATPanel(TComponent* Owner, int index, AnsiString Name): TComponent(Owner)
{
    AnsiString str;
    _Index          =index;
    _ChannelNo      =101+index;
    _Enable         =false;
    fTemperatureData=0.0;
    _iOffsetWidth   =50;

    gbMeasurePoint      =new TGroupBox  (this);
    cbChannel           =new TComboBox  (this);
    cbEnable            =new TCheckBox  (this);
    pnlTemp             =new TPanel     (this);
    ImgOriginalOffset   =new TImage     (this);

    str.sprintf("gbMeasurePoint%02d", index);
    gbMeasurePoint->Name        =str;
    gbMeasurePoint->Height      =105;
    gbMeasurePoint->Width       =280;
    gbMeasurePoint->Font->Color =clBlack;
    gbMeasurePoint->Font->Name  ="Arial";
    gbMeasurePoint->Font->Size  =14;
    str.sprintf("[%02d] %s", index, Name);
    gbMeasurePoint->Caption     =str;
    gbMeasurePoint->OnMouseUp   =gbMeasurePointMouseUp;
    gbMeasurePoint->OnMouseDown =gbMeasurePointMouseDown;
    gbMeasurePoint->OnMouseMove =gbMeasurePointMouseMove;

    str.sprintf("cbChannel%02d", index);
    cbChannel->Name         =str;
    cbChannel->Parent       =gbMeasurePoint;
    cbChannel->Height       =28;
    cbChannel->Left         =4;
    cbChannel->Top          =48;
    cbChannel->Width        =168;
    cbChannel->Font->Color  =clBlack;
    cbChannel->Font->Name   ="Arial";
    cbChannel->Font->Size   =12;
    cbChannel->Text         =_ChannelNo;
    cbChannel->Tag          =index;
    cbChannel->OnChange     =cbChannelChange;

    str.sprintf("cbEnable%02d", index);
    cbEnable->Name         =str;
    cbEnable->Parent       =gbMeasurePoint;
    cbEnable->Height       =17;
    cbEnable->Left         =4;
    cbEnable->Top          =80;
    cbEnable->Width        =89;
    cbEnable->Font->Color  =clBlack;
    cbEnable->Font->Name   ="Arial";
    cbEnable->Font->Size   =12;
    cbEnable->Caption      ="Use";
    cbEnable->Tag          =index;

    str.sprintf("pnlTemp%02d", index);
    pnlTemp->Name         =str;
    pnlTemp->Parent       =gbMeasurePoint;
    pnlTemp->Color        =TColor(0x009EFCFE);
    pnlTemp->Height       =25;
    pnlTemp->Left         =4;
    pnlTemp->Top          =21;
    pnlTemp->Width        =168;
    pnlTemp->Font->Color  =clBlack;
    pnlTemp->Font->Name   ="Arial";
    pnlTemp->Font->Size   =12;
    pnlTemp->Caption      ="000.00";
    pnlTemp->Tag          =index;

    str.sprintf("ImgOriginalOffset%02d", index);
    ImgOriginalOffset->Name         =str;
    ImgOriginalOffset->Parent       =gbMeasurePoint;
    ImgOriginalOffset->Height       =84;
    ImgOriginalOffset->Left         =174;
    ImgOriginalOffset->Top          =16;
    ImgOriginalOffset->Width        =100;
    ImgOriginalOffset->Tag          =index;
}
//---------------------------------------------------------------------------
__fastcall TMyATPanel::TMyATPanel(TComponent* Owner, int index, AnsiString Name, int iGroupNum, AnsiString GroupName): TComponent(Owner)
{
    AnsiString str;
    _Index          =index;
    _ChannelNo      =101+index;
    _Enable         =false;
    fTemperatureData=0.0;
    _iOffsetWidth   =50;

    gbMeasurePoint      =new TGroupBox  (this);
    cbChannel           =new TComboBox  (this);
    cbEnable            =new TCheckBox  (this);
    pnlTemp             =new TPanel     (this);
    ImgOriginalOffset   =new TImage     (this);
    _GroupName=GroupName;
    _Group=iGroupNum;

    str.sprintf("gbMeasurePoint%02d", index);
    gbMeasurePoint->Name        =str;
    gbMeasurePoint->Height      =105;
    gbMeasurePoint->Width       =280;
    gbMeasurePoint->Font->Color =clBlack;
    gbMeasurePoint->Font->Name  ="Arial";
    gbMeasurePoint->Font->Size  =14;
    str.sprintf("[%02d] %s", index, Name);
    gbMeasurePoint->Caption     =str;
    gbMeasurePoint->OnMouseUp   =gbMeasurePointMouseUp;
    gbMeasurePoint->OnMouseDown =gbMeasurePointMouseDown;
    gbMeasurePoint->OnMouseMove =gbMeasurePointMouseMove;

    str.sprintf("cbChannel%02d", index);
    cbChannel->Name         =str;
    cbChannel->Parent       =gbMeasurePoint;
    cbChannel->Height       =28;
    cbChannel->Left         =4;
    cbChannel->Top          =48;
    cbChannel->Width        =168;
    cbChannel->Font->Color  =clBlack;
    cbChannel->Font->Name   ="Arial";
    cbChannel->Font->Size   =12;
    cbChannel->Text         =_ChannelNo;
    cbChannel->Tag          =index;
    cbChannel->OnChange     =cbChannelChange;

    str.sprintf("cbEnable%02d", index);
    cbEnable->Name         =str;
    cbEnable->Parent       =gbMeasurePoint;
    cbEnable->Height       =17;
    cbEnable->Left         =4;
    cbEnable->Top          =80;
    cbEnable->Width        =89;
    cbEnable->Font->Color  =clBlack;
    cbEnable->Font->Name   ="Arial";
    cbEnable->Font->Size   =12;
    cbEnable->Caption      ="Use";
    cbEnable->Tag          =index;

    str.sprintf("pnlTemp%02d", index);
    pnlTemp->Name         =str;
    pnlTemp->Parent       =gbMeasurePoint;
    pnlTemp->Color        =TColor(0x009EFCFE);
    pnlTemp->Height       =25;
    pnlTemp->Left         =4;
    pnlTemp->Top          =21;
    pnlTemp->Width        =168;
    pnlTemp->Font->Color  =clBlack;
    pnlTemp->Font->Name   ="Arial";
    pnlTemp->Font->Size   =12;
    pnlTemp->Caption      ="000.00";
    pnlTemp->Tag          =index;

    str.sprintf("ImgOriginalOffset%02d", index);
    ImgOriginalOffset->Name         =str;
    ImgOriginalOffset->Parent       =gbMeasurePoint;
    ImgOriginalOffset->Height       =84;
    ImgOriginalOffset->Left         =174;
    ImgOriginalOffset->Top          =16;
    ImgOriginalOffset->Width        =100;
    ImgOriginalOffset->Tag          =index;
}
//---------------------------------------------------------------------------
void __fastcall TMyATPanel::SetParent(TWinControl *Sender)
{
    gbMeasurePoint->Parent=Sender;
    for(int i=0; i<iChannelCount; i++)
    {
        cbChannel->Items->Add(101+i);
    }
}
//---------------------------------------------------------------------------
void __fastcall TMyATPanel::ImageShowOriginalOffset (double low, double middle, double high)
{
    int iTextWidth, iTextHeight;
    AnsiString Temp;
    ImgOriginalOffset->Canvas->Brush->Color =TColor(0x00DFD9CC);
    ImgOriginalOffset->Canvas->Pen->Color   =clBlack;
    ImgOriginalOffset->Canvas->Rectangle(TRect(0, 0,ImgOriginalOffset->Width, ImgOriginalOffset->Height));

    ImgOriginalOffset->Canvas->Font->Color  =clNavy;
    ImgOriginalOffset->Canvas->Font->Name   ="Arial";
    ImgOriginalOffset->Canvas->Font->Size   =12;

    iTextHeight =(20-ImgOriginalOffset->Canvas->TextHeight(low))/2;

    iTextWidth  =ImgOriginalOffset->Canvas->TextWidth("Ori.");
    ImgOriginalOffset->Canvas->TextOutA(((_iOffsetWidth-iTextWidth)/2), iTextHeight, "Ori.");

    iTextWidth  =ImgOriginalOffset->Canvas->TextWidth("Cur.");
    ImgOriginalOffset->Canvas->TextOutA(((_iOffsetWidth-iTextWidth)/2)+_iOffsetWidth, iTextHeight, "Cur.");

    ImgOriginalOffset->Canvas->Font->Color  =clBlack;

    //溫度 Low--------------
    Temp.sprintf("%0.2f", low);
    iTextWidth  =ImgOriginalOffset->Canvas->TextWidth(Temp);
    ImgOriginalOffset->Canvas->TextOutA(((_iOffsetWidth-iTextWidth)/2), iTextHeight+20, Temp);

    //溫度 Middle--------------
    Temp.sprintf("%0.2f", middle);
    iTextWidth  =ImgOriginalOffset->Canvas->TextWidth(Temp);
    ImgOriginalOffset->Canvas->TextOutA(((_iOffsetWidth-iTextWidth)/2), iTextHeight+40, Temp);

    //溫度 High--------------
    Temp.sprintf("%0.2f", high);
    iTextWidth  =ImgOriginalOffset->Canvas->TextWidth(Temp);
    ImgOriginalOffset->Canvas->TextOutA(((_iOffsetWidth-iTextWidth)/2), iTextHeight+60, Temp);
}
//---------------------------------------------------------------------------
void __fastcall TMyATPanel::ImageShowCurrentOffset(int base, double dCurrentOffset)
{
    int iTextWidth, iTextHeight;
    AnsiString Temp;
    ImgOriginalOffset->Canvas->Brush->Color=TColor(0x00DFD9CC);
    ImgOriginalOffset->Canvas->Pen->Color  =TColor(0x00DFD9CC);
    ImgOriginalOffset->Canvas->Rectangle(TRect(_iOffsetWidth+1, 1, _iOffsetWidth-1, 19));

    ImgOriginalOffset->Canvas->Font->Color  =clRed;
    ImgOriginalOffset->Canvas->Font->Name   ="Arial";
    ImgOriginalOffset->Canvas->Font->Size   =12;

    iTextHeight =(20-ImgOriginalOffset->Canvas->TextHeight(dCurrentOffset))/2;
    Temp.sprintf("%0.2f", dCurrentOffset);
    iTextWidth  =ImgOriginalOffset->Canvas->TextWidth(Temp);
    ImgOriginalOffset->Canvas->TextOutA(((_iOffsetWidth-iTextWidth)/2)+_iOffsetWidth, iTextHeight+20*(base+1), Temp);
}
//---------------------------------------------------------------------------
__fastcall TMyATPanel::~TMyATPanel()
{
}
//---------------------------------------------------------------------------
void __fastcall TMyATPanel::gbMeasurePointMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TGroupBox *Ptr;
    Ptr=(TGroupBox *)Sender;
    if(Ptr->Tag!=0)
    {
        Ptr->Tag=0;
    }
}
//---------------------------------------------------------------------------
void __fastcall TMyATPanel::gbMeasurePointMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TGroupBox *Ptr;
    Ptr=(TGroupBox *)Sender;
    if(Ptr->Tag!=1)
        Ptr->Tag=1;
    iStartX=X;
    iStartY=Y;
}
//---------------------------------------------------------------------------
void __fastcall TMyATPanel::gbMeasurePointMouseMove(TObject *Sender, TShiftState Shift, int X,int Y)
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
void __fastcall TMyATPanel::cbChannelChange(TObject *Sender)
{
    _ChannelNo=atoi(cbChannel->Text.c_str());
}
//---------------------------------------------------------------------------
__fastcall TACTForm::TACTForm(TComponent* Owner)
    : TForm(Owner)
{
    bCommConnect=false;
    bAutoStart=false;

    TCheckBox* pBase[3]={cbBase01, cbBase02, cbBase03};
    TPanel*  pBaseTemp[3]={pnlBaseTemp01, pnlBaseTemp02, pnlBaseTemp03};
    LoadACTData(ACTData);
    for(int i=0; i<3; i++)
    {
        _cbBase[i]=pBase[i];
        _pnlBaseTemp[i]=pBaseTemp[i];
        bBaseFinish[i]=false;
    }

    if(ACTData.iThermoCtrlType==Type_Agilent34970A)
    {
        for(int i=0; i<tcTotalCount; i++)   //決定使用幾個制冷片
        {
            myATPal.push_back(new TMyATPanel(this, i, asTempCtrl[i]));
            myATPal[i]->SetParent(ScrollBox1);
        }
    }
    else if(ACTData.iThermoCtrlType==Type_DeltaDTB4824) //Steven 20210510 : 自動K溫使用DTB4824
    {
        myATPal.push_back(new TMyATPanel(this,  0, "HP 1-1",    tcHotPlate1, "HP1"));
        myATPal.push_back(new TMyATPanel(this,  1, "HP 1-2",    tcHotPlate1, "HP1"));
        myATPal.push_back(new TMyATPanel(this,  2, "HP 1-3",    tcHotPlate1, "HP1"));
        myATPal.push_back(new TMyATPanel(this,  3, "HP 1-4",    tcHotPlate1, "HP1"));
        myATPal.push_back(new TMyATPanel(this,  4, "HP 1-5",    tcHotPlate1, "HP1"));
        myATPal.push_back(new TMyATPanel(this,  5, "HP 2-1",    tcHotPlate2, "HP2"));
        myATPal.push_back(new TMyATPanel(this,  6, "HP 2-2",    tcHotPlate2, "HP2"));
        myATPal.push_back(new TMyATPanel(this,  7, "HP 2-3",    tcHotPlate2, "HP2"));
        myATPal.push_back(new TMyATPanel(this,  8, "HP 2-4",    tcHotPlate2, "HP2"));
        myATPal.push_back(new TMyATPanel(this,  9, "HP 2-5",    tcHotPlate2, "HP2"));
        myATPal.push_back(new TMyATPanel(this, 10, "SHT 1-1",   tcShuttle1,  "SHT1"));
        myATPal.push_back(new TMyATPanel(this, 11, "SHT 1-2",   tcShuttle1,  "SHT1"));
        myATPal.push_back(new TMyATPanel(this, 12, "SHT 2-1",   tcShuttle2,  "SHT2"));
        myATPal.push_back(new TMyATPanel(this, 13, "SHT 2-2",   tcShuttle2,  "SHT2"));
        if(iSocketBaseTempCount==eDut2ea)   //Steven 20140116 : Socket數量,改用數字設定
        {
            myATPal.push_back(new TMyATPanel(this, 14, "Base 1-1",    tcDUT1,    "Base"));
            myATPal.push_back(new TMyATPanel(this, 15, "Base 1-2",    tcDUT1,    "Base"));
            myATPal.push_back(new TMyATPanel(this, 16, "Base 2-1",    tcDUT2,    "Base"));
            myATPal.push_back(new TMyATPanel(this, 17, "Base 2-2",    tcDUT2,    "Base"));
            iTotalGroupCount=9;
        }
        else if(iSocketBaseTempCount==eDut4ea)
        {
            myATPal.push_back(new TMyATPanel(this, 14, "Base 1",    tcDUT1,    "Base"));
            myATPal.push_back(new TMyATPanel(this, 15, "Base 2",    tcDUT2,    "Base"));
            myATPal.push_back(new TMyATPanel(this, 16, "Base 3",    tcDUT3,    "Base"));
            myATPal.push_back(new TMyATPanel(this, 17, "Base 4",    tcDUT4,    "Base"));
            iTotalGroupCount=11;
        }
        else
        {
            myATPal.push_back(new TMyATPanel(this, 14, "Base 1",    tcSocket,    "Base"));
            myATPal.push_back(new TMyATPanel(this, 15, "Base 2",    tcSocket,    "Base"));
            myATPal.push_back(new TMyATPanel(this, 16, "Base 3",    tcSocket,    "Base"));
            myATPal.push_back(new TMyATPanel(this, 17, "Base 4",    tcSocket,    "Base"));
            iTotalGroupCount=8;
        }
        myATPal.push_back(new TMyATPanel(this, 18, "Chamber 1", tcChamber,   "Chamber"));
        myATPal.push_back(new TMyATPanel(this, 19, "Chamber 2", tcChamber,   "Chamber"));
        myATPal.push_back(new TMyATPanel(this, 20, "Chamber 3", tcChamber,   "Chamber"));
        myATPal.push_back(new TMyATPanel(this, 21, "HotGun 1",  tcHeatGun1,  "HotGun1"));
        myATPal.push_back(new TMyATPanel(this, 22, "HotGun 2",  tcHeatGun2,  "HotGun2"));

        myATPal.push_back(new TMyATPanel(this, 23, "Reserved 1",  tcCCD_2,   "Reserved"));
        myATPal.push_back(new TMyATPanel(this, 24, "Reserved 2",  tcCCD_2,   "Reserved"));
        myATPal.push_back(new TMyATPanel(this, 25, "Reserved 3",  tcCCD_2,   "Reserved"));
        myATPal.push_back(new TMyATPanel(this, 26, "Reserved 4",  tcCCD_2,   "Reserved"));
        myATPal.push_back(new TMyATPanel(this, 27, "Reserved 5",  tcCCD_2,   "Reserved"));
        myATPal.push_back(new TMyATPanel(this, 28, "Reserved 6",  tcCCD_2,   "Reserved"));
        myATPal.push_back(new TMyATPanel(this, 29, "Reserved 7",  tcCCD_2,   "Reserved"));
        myATPal.push_back(new TMyATPanel(this, 30, "Reserved 8",  tcCCD_2,   "Reserved"));
        myATPal.push_back(new TMyATPanel(this, 31, "Reserved 9",  tcCCD_2,   "Reserved"));
        myATPal.push_back(new TMyATPanel(this, 32, "Reserved 10", tcCCD_2,   "Reserved"));

        for(int i=0; i<int(myATPal.size()); i++)
            myATPal[i]->SetParent(ScrollBox1);

        iGroupSet[0]=tcHotPlate1;
        iGroupSet[1]=tcHotPlate2;
        iGroupSet[2]=tcShuttle1;
        iGroupSet[3]=tcShuttle2;
        iGroupSet[4]=tcChamber;
        iGroupSet[5]=tcHeatGun1;
        iGroupSet[6]=tcHeatGun2;
        if(iSocketBaseTempCount==eDut2ea)
        {
            iGroupSet[7]=tcDUT1;
            iGroupSet[8]=tcDUT2;
        }
        else if(iSocketBaseTempCount==eDut4ea)
        {
            iGroupSet[7]=tcDUT1;
            iGroupSet[8]=tcDUT2;
            iGroupSet[9]=tcDUT3;
            iGroupSet[10]=tcDUT4;
        }
        else
        {
            iGroupSet[7]=tcSocket;
        }
    }

    FilePath="D:\\HT9045\\System\\AutoTemperature.ini";
    TimerACT->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TACTForm::FormDestroy(TObject *Sender)
{
    try
    {
        for(vector<TMyATPanel *>::iterator iter=myATPal.begin(); iter!=myATPal.end(); ++iter)
        {
            delete *iter;
        }
        vec_clr(myATPal);
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TACTForm::FormDestroy");
    }
    LogSoftwareOffTime("TACTForm, FormDestroy");                                //Steven 20210526 : 紀錄軟體執行時間
}
//---------------------------------------------------------------------------
void __fastcall TACTForm::FormShow(TObject *Sender)
{
    Top=4;
    Left=4;
    bAutoStart = false;
    ChangeStartState();
    int iGroup=0;

    // ACT Setting -------------------------------------->>
    LoadACTData(ACTData);
    cbbThermoCtrlType->ItemIndex=ACTData.iThermoCtrlType;
    edtCheckIntervalTime->Text = AnsiString(ACTData.iCheckIntervalTime);
    edtCalibrationRange ->Text = AnsiString(ACTData.iCalibrationRange  );
    edtRange1->Text = AnsiString(ACTData.dCalibrationRange[0]  );   //Steven 20210520 : K溫範圍改成三組
    edtRange2->Text = AnsiString(ACTData.dCalibrationRange[1]  );
    edtRange3->Text = AnsiString(ACTData.dCalibrationRange[2]  );
    edtReadScanTime     ->Text = AnsiString(ACTData.iReadScanTime      );
    edtOffsetLimit      ->Text = AnsiString(ACTData.iOffsetLimit       );
    edSingleOffset      ->Text = AnsiString(ACTData.iSingleOffset      );
    edColumns           ->Text = AnsiString(ACTData.iDisplayColumn     );
    rgOffsetMethod->ItemIndex  = ACTData.iOffsetMethod;
    ShowDefaultPos();
    // ACT Setting --------------------------------------<<

    // Comm Port ---------------------------------------->>
    LoadCommData(ACTCom);

    cbDevice    ->Text = CommName;
    cbBaudRate  ->Text = IntToStr(ACTCom->BaudRate);

    if(ACTCom->ByteSize==_5)
        cbByteSize->ItemIndex=0;
    else if(ACTCom->ByteSize==_6)
        cbByteSize->ItemIndex=1;
    else if(ACTCom->ByteSize==_7)
        cbByteSize->ItemIndex=2;
    else if(ACTCom->ByteSize==_8)
        cbByteSize->ItemIndex=3;

    if(ACTCom->StopBits==_1)
        cbStopBit->ItemIndex=0;
    else if(ACTCom->StopBits==_1_5)
        cbStopBit->ItemIndex=1;
    else if(ACTCom->StopBits==_2)
        cbStopBit->ItemIndex=2;

    if(ACTCom->Parity==None)
        cbParity->ItemIndex=0;
    else if(ACTCom->Parity==Odd)
        cbParity->ItemIndex=1;
    else if(ACTCom->Parity==Even)
        cbParity->ItemIndex=2;
    else if(ACTCom->Parity==Space)
        cbParity->ItemIndex=4;
    else//if(ACTCom->Parity==Mark)
        cbParity->ItemIndex=3;

    OpenCommPort();
    // Comm Port ----------------------------------------<<

    pnlBaseTemp01->Caption=AnsiString(Temperature.fLowBase);
    cbBase01->Checked=true;

    if(Temperature.iTempMode==0)
    {
        pnlBaseTemp02->Caption = "-----";
        cbBase02->Checked = false;
        cbBase02->Enabled = false;
    }
    else
    {
        pnlBaseTemp02->Caption  = AnsiString(Temperature.fMiddBase);
        cbBase02->Checked = true;
        cbBase02->Enabled = true;
    }

    if(ACTData.iThermoCtrlType==Type_DeltaDTB4824)  //Steven 20210510 : 自動K溫使用DTB4824
    {
        for(int i=0; i<(int)myATPal.size(); i++)
            myATPal[i]->cbChannel->Visible=false;
    }

    for(int i=0; i<(int)myATPal.size(); i++)
    {
        if(ACTData.iThermoCtrlType==Type_DeltaDTB4824)  //Steven 20210510 : 自動K溫使用DTB4824
            iGroup=myATPal[i]->_Group;
        else
            iGroup=i;

        myATPal[i]->ImageShowOriginalOffset(Temperature.fTempOffSet[LowBase][iGroup], Temperature.fTempOffSet[MidBase][iGroup], Temperature.fTempOffSet[HigBase][iGroup]);

        myATPal[i]->ImageShowCurrentOffset(LowBase, Temperature.fTempOffSet[LowBase][iGroup]);
        myATPal[i]->ImageShowCurrentOffset(MidBase, Temperature.fTempOffSet[MidBase][iGroup]);
        myATPal[i]->ImageShowCurrentOffset(HigBase, Temperature.fTempOffSet[HigBase][iGroup]);
        myATPal[i]->gbMeasurePoint->Visible=true;
    }

    pnlBaseTemp03->Caption=AnsiString(Temperature.fHighBase);
    cbBase03->Checked=true;

    bBaseFinish[0]=false;
    bBaseFinish[1]=false;
    bBaseFinish[2]=false;

    _pnlBaseTemp[0]->Color=clBtnFace;
    _pnlBaseTemp[1]->Color=clBtnFace;
    _pnlBaseTemp[2]->Color=clBtnFace;

    pcACT->ActivePage=tsTemperature;
    UpdateTemperatureData();
    iAddr=0;
    iReceiveData=0;
    TimerACT->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TACTForm::FormClose(TObject *Sender, TCloseAction &Action)
{
    TimerACT->Enabled=false;
    bAutoStart=false;
    MemoOffset->Clear();
    ChangeStartState();
    CloseCommPort();
}
//---------------------------------------------------------------------------
void __fastcall TACTForm::ACTComReceiveData(TObject *Sender,
      Pointer Buffer, WORD BufferLength)
{
    int i, j;
    double Read;
    char *str;
    AnsiString Com2Buffer, S;
    char rData[1024], sData[1024];
    str=(char *)Buffer;
    if(BufferLength>1000)
        str[1000]=0;
    strncpy(rData, str, sizeof(rData));

    if(ACTData.iThermoCtrlType==Type_Agilent34970A)
    {
        for(i=0; i<iChannelCount; i++)
        {
            for(j=0; j<(int)myATPal.size(); j++)
            {
                if(myATPal[j]->cbEnable->Checked)
                {
                    if(myATPal[j]->_ChannelNo==(i+101))
                    {
                        if(SplitStrByDotSpaceOnly(rData, sData, 32))
                        {
                            double fData=atof(sData);
                            if(fData<0.001)
                                myATPal[j]->fTemperatureData=0.;
                            else
                                myATPal[j]->fTemperatureData=((int)(fData*100))/100.;
                        }
                    }
                }
                else
                {
                    myATPal[j]->fTemperatureData=0.;
                }
            }
        }
    }
    else if(ACTData.iThermoCtrlType==Type_DeltaDTB4824) //Steven 20210510 : 自動K溫使用DTB4824
    {
        Com2Buffer.sprintf("%s", (char*)Buffer);
        iReceiveData=2;
        S=Com2Buffer.SubString(8, 4);    //8~11位元為溫度值;
        Read=(HexStrToInt(S.c_str()))/10.0f;
        if(Read<0.001)
            myATPal[iAddr]->fTemperatureData=0.;
        else
            myATPal[iAddr]->fTemperatureData=Read;
        iReceiveData=2;
    }
    ShowCommData("[Read]", AnsiString(str));
}
//---------------------------------------------------------------------------
bool TACTForm::CloseCommPort()
{
    try
    {
        ACTCom->StopComm();
        bCommConnect = false;

        ShowCommData("[Disconnect]", " OK");
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TACTForm::CloseCommPort");
        ShowCommData("[Disconnect]", " FAIL");
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
bool TACTForm::OpenCommPort()
{
    bool flag;
    if(bCommConnect==true)
    {
        CloseCommPort();
    }

    try
    {
        flag=GetCOMPortStatus(CommName);
        ACTCom->CommName="\\\\.\\"+CommName;
        if(flag==false)
        {
            CloseCommPort();
            ShowCommData("[Connect] "+CommName+" port error", "FAIL");
        }
        else
        {
            CloseCommPort();
            ACTCom->StartComm();
            bCommConnect=true;
            ShowCommData("[Connect]", " OK");
        }
    }
    catch(...)
    {
        CloseCommPort();
        MyDBIProcess("Exception", "TACTForm::OpenCommPort");
        ShowCommData("[Connect]", " FAIL");
        bCommConnect=false;
        return false;
    }
    return true;
}
//---------------------------------------------------------------------------
void TACTForm::SendCommand(AnsiString sData)
{
    if(bCommConnect==false)
    {
        ShowCommData("[Send]", sData);
        ShowCommData("[Connect Error]", " Send Fail");
        return;
    }

    int iSize=sData.Length();
    if(iSize<=0)
    {
        ShowCommData("[Send]", sData);
        ShowCommData("[Send Data Fail]", " Size = 0");
        return;
    }

    char Buff[1024];
    for(int i=0; i<iSize; i++)
    {
        Buff[i]=sData[i+1];
    }

    Buff[iSize]='\0';
    ACTCom->WriteCommData(Buff, strlen(Buff));
    ShowCommData("[Send]", sData);
}
//---------------------------------------------------------------------------
void TACTForm::ShowCommData(AnsiString sType, AnsiString sData)
{
    AnsiString sLog=sType+sData, s2;
    sLog=StringReplace(sLog, "\r\n", "", TReplaceFlags()<<rfReplaceAll);
    s2.sprintf("%02d:%02d:%02d.%03d - %s", SystemHour, SystemMin, SystemSec, SystemMSec, sLog);
    if(MemoLog->Lines->Count>2000)
    {
        MemoLog->Clear();
    }
    MemoLog->Lines->Append(s2);
}
//---------------------------------------------------------------------------
void __fastcall TACTForm::btnUpdateClick(TObject *Sender)
{
    // ACT Setting -------------------------------------->>
    ACTData.iThermoCtrlType     = cbbThermoCtrlType->ItemIndex;
    ACTData.iCheckIntervalTime  = atoi(edtCheckIntervalTime->Text.c_str());
    ACTData.iCalibrationRange   = atoi(edtCalibrationRange->Text.c_str() );
    ACTData.dCalibrationRange[0]= atoi(edtRange1->Text.c_str() );   //Steven 20210520 : K溫範圍改成三組
    ACTData.dCalibrationRange[1]= atoi(edtRange2->Text.c_str() );
    ACTData.dCalibrationRange[2]= atoi(edtRange3->Text.c_str() );
    ACTData.iReadScanTime       = atoi(edtReadScanTime->Text.c_str()     );
    ACTData.iOffsetLimit        = atoi(edtOffsetLimit->Text.c_str()      );
    ACTData.iSingleOffset       = atoi(edSingleOffset->Text.c_str()      );
    ACTData.iDisplayColumn      = atoi(edColumns->Text.c_str()           );
    ACTData.iOffsetMethod       = rgOffsetMethod->ItemIndex;
    SaveACTData( ACTData );
    // ACT Setting --------------------------------------<<

    // Comm Port ---------------------------------------->>
    CommName         = cbDevice->Text;
    ACTCom->CommName="\\\\.\\"+CommName;
    ACTCom->BaudRate = atoi(cbBaudRate->Text.c_str());

    if(cbByteSize->ItemIndex==0)
        ACTCom->ByteSize=_5;
    else if(cbByteSize->ItemIndex==1)
        ACTCom->ByteSize=_6;
    else if(cbByteSize->ItemIndex==2)
        ACTCom->ByteSize=_7;
    else if(cbByteSize->ItemIndex==3)
        ACTCom->ByteSize=_8;

    if(cbStopBit->ItemIndex==0)
        ACTCom->StopBits=_1;
    else if(cbStopBit->ItemIndex==1)
        ACTCom->StopBits=_1_5;
    else if(cbStopBit->ItemIndex==2)
        ACTCom->StopBits=_2;

    if(cbParity->ItemIndex==0)
        ACTCom->Parity=None;
    else if(cbParity->ItemIndex==1)
        ACTCom->Parity=Odd;
    else if(cbParity->ItemIndex==2)
        ACTCom->Parity=Even;
    else if(cbParity->ItemIndex==4)
        ACTCom->Parity=Space;
    else if(cbParity->ItemIndex==3)
        ACTCom->Parity=TParity(3);

    SaveCommData(ACTCom);
    // Comm Port ----------------------------------------<<
}
//---------------------------------------------------------------------------
void __fastcall TACTForm::btnAutoStartClick(TObject *Sender)
{
    bAutoStart=!bAutoStart;
    int iGroup;

    if(bAutoStart)
    {
        TDateTime myDateTime=Now();

        AnsiString StrCap="Time,    \tBase    ";
        AnsiString StrLow=myDateTime.FormatString("hh:nn:ss")+",\tOrig."+pnlBaseTemp01->Caption;
        AnsiString StrMid=myDateTime.FormatString("hh:nn:ss")+",\tOrig."+pnlBaseTemp02->Caption;
        AnsiString StrHig=myDateTime.FormatString("hh:nn:ss")+",\tOrig."+pnlBaseTemp03->Caption;
        AnsiString StrTmp=myDateTime.FormatString("hh:nn:ss")+",\tTemp."+AnsiString(Temperature.fWorkTemperBase);
        for(int i=0; i<(int)myATPal.size(); i++)
        {
            if(ACTData.iThermoCtrlType==Type_DeltaDTB4824)  //Steven 20210510 : 自動K溫使用DTB4824
                iGroup=myATPal[i]->_Group;
            else
                iGroup=i;
            if(myATPal[i]->cbEnable->Checked==true)
            {
                StrCap+=",\t"+myATPal[i]->gbMeasurePoint->Caption;
                StrLow+=",\t"+AnsiString(Temperature.fTempOffSet[LowBase][iGroup]);
                StrMid+=",\t"+AnsiString(Temperature.fTempOffSet[MidBase][iGroup]);
                StrHig+=",\t"+AnsiString(Temperature.fTempOffSet[HigBase][iGroup]);
                StrTmp+=",\t"+myATPal[i]->pnlTemp->Caption;
            }
        }

        MemoOffset->Lines->Add(StrCap);
        MemoOffset->Lines->Add(StrLow);
        MemoOffset->Lines->Add(StrMid);
        MemoOffset->Lines->Add(StrHig);
        MemoOffset->Lines->Add(StrTmp);
        MemoOffset->Lines->Add("<<---------------------------------------------------------->>");
        fMain->ChangeTempMode(1, false, true);
        tCheckIntervalTimer.LatchCycleTimeSec(true);
    }
    else
    {
        fMain->ChangeTempMode(0, false, true);
        bBaseFinish[0]=false;
        bBaseFinish[1]=false;
        bBaseFinish[2]=false;
    }
    ChangeStartState();
}
//---------------------------------------------------------------------------
void TACTForm::ChangeStartState()
{
    if(bAutoStart==false)
    {
        btnAutoStart->Caption="START";
        btnAutoStart->Font->Color=clGreen;
    }
    else
    {
        btnAutoStart->Caption="PAUSE";
        btnAutoStart->Font->Color=clRed;
        tTempScanTimer.SetSecAndOn(1);
    }
}
//---------------------------------------------------------------------------
void TACTForm::UpdateTemperatureData()
{
    for(int i=0; i<(int)myATPal.size(); i++)
    {
        if(myATPal[i]->cbEnable->Checked==true)
        {
            if(myATPal[i]->fTemperatureData<200.0)
            {
                myATPal[i]->pnlTemp->Caption = AnsiString(myATPal[i]->fTemperatureData);
                myATPal[i]->pnlTemp->Color   = (TColor)0x009EFCFE;
            }
            else
            {
                myATPal[i]->pnlTemp->Caption = "ERROR";
                myATPal[i]->pnlTemp->Color   = clYellow;
            }
        }
        else
        {
            myATPal[i]->pnlTemp->Caption = "-----";
            myATPal[i]->pnlTemp->Color   = clWhite;
        }
    }
}
//---------------------------------------------------------------------------
void TACTForm::SaveACTData(ACT_DATA& Data)
{
    TIniFile* IniFile1=new TIniFile(FilePath);
    Data.iThermoCtrlType    = CheckRange(Data.iThermoCtrlType   ,  1, 0);
    Data.iCheckIntervalTime = CheckRange(Data.iCheckIntervalTime, 30, 1);
    Data.iCalibrationRange  = CheckRange(Data.iCalibrationRange , 10, 1);
    ACTData.dCalibrationRange[0]= CheckRange(Data.dCalibrationRange[0] , 10.0, 0.1);   //Steven 20210520 : K溫範圍改成三組
    ACTData.dCalibrationRange[1]= CheckRange(Data.dCalibrationRange[1] , 10.0, 0.1);
    ACTData.dCalibrationRange[2]= CheckRange(Data.dCalibrationRange[2] , 10.0, 0.1);
    Data.iReadScanTime      = CheckRange(Data.iReadScanTime     , 10, 1);
    Data.iOffsetLimit       = CheckRange(Data.iOffsetLimit      , 30, 1);
    Data.iSingleOffset      = CheckRange(Data.iSingleOffset     , 30, 1);
    IniFile1->WriteInteger("ACT", "iThermoCtrlType"   , Data.iThermoCtrlType     );
    IniFile1->WriteInteger("ACT", "CheckIntervalTime" , Data.iCheckIntervalTime  );
    IniFile1->WriteInteger("ACT", "CalibrationRange"  , Data.iCalibrationRange   );
    IniFile1->WriteFloat("ACT", "dCalibrationRange[0]"  , Data.dCalibrationRange[0]   );    //Steven 20210520 : K溫範圍改成三組
    IniFile1->WriteFloat("ACT", "dCalibrationRange[1]"  , Data.dCalibrationRange[1]   );
    IniFile1->WriteFloat("ACT", "dCalibrationRange[2]"  , Data.dCalibrationRange[2]   );
    IniFile1->WriteInteger("ACT", "ReadScanTime"      , Data.iReadScanTime       );
    IniFile1->WriteInteger("ACT", "OffsetLimit"       , Data.iOffsetLimit        );
    IniFile1->WriteInteger("ACT", "SingleOffsetLimit" , Data.iSingleOffset       );
    IniFile1->WriteInteger("Display", "Colums"        , Data.iDisplayColumn      );
    IniFile1->WriteInteger("ACT", "OffsetMethod"      , Data.iOffsetMethod       );
    delete IniFile1;
}
//---------------------------------------------------------------------------
void TACTForm::LoadACTData(ACT_DATA& Data)
{
    TIniFile* IniFile1 = new TIniFile(FilePath);
    Data.iThermoCtrlType    = IniFile1->ReadInteger("ACT", "iThermoCtrlType"  , 1 );
    Data.iCheckIntervalTime = IniFile1->ReadInteger("ACT", "CheckIntervalTime", 3 );
    Data.iCalibrationRange  = IniFile1->ReadInteger("ACT", "CalibrationRange" , 2  );
    Data.dCalibrationRange[0]= IniFile1->ReadFloat("ACT", "dCalibrationRange[0]" , 2.0  );  //Steven 20210520 : K溫範圍改成三組
    Data.dCalibrationRange[1]= IniFile1->ReadFloat("ACT", "dCalibrationRange[1]" , 2.0  );
    Data.dCalibrationRange[2]= IniFile1->ReadFloat("ACT", "dCalibrationRange[2]" , 2.0  );
    Data.iReadScanTime      = IniFile1->ReadInteger("ACT", "ReadScanTime"     , 3  );
    Data.iOffsetLimit       = IniFile1->ReadInteger("ACT", "OffsetLimit"      , 30);
    Data.iSingleOffset      = IniFile1->ReadInteger("ACT", "SingleOffsetLimit", 5  );
    Data.iDisplayColumn     = IniFile1->ReadInteger("Display", "Colums"       , 1  );
    Data.iOffsetMethod      = IniFile1->ReadInteger("ACT", "OffsetMethod"     , 1  );
    Data.iThermoCtrlType    = CheckRange(Data.iThermoCtrlType   ,  1, 0);
    Data.iCheckIntervalTime = CheckRange(Data.iCheckIntervalTime, 30, 1);
    Data.iCalibrationRange  = CheckRange(Data.iCalibrationRange , 10, 1);
    Data.dCalibrationRange[0]  = CheckRange(Data.dCalibrationRange[0] , 10.0, 1.0);    //Steven 20210520 : K溫範圍改成三組
    Data.dCalibrationRange[1]  = CheckRange(Data.dCalibrationRange[1] , 10.0, 1.0);
    Data.dCalibrationRange[2]  = CheckRange(Data.dCalibrationRange[2] , 10.0, 1.0);
    Data.iReadScanTime      = CheckRange(Data.iReadScanTime     , 10, 1);
    Data.iOffsetLimit       = CheckRange(Data.iOffsetLimit      , 30, 1);
    Data.iSingleOffset      = CheckRange(Data.iSingleOffset     , 30, 1);

    delete IniFile1;
}
//---------------------------------------------------------------------------
void __fastcall TACTForm::TimerACTTimer(TObject *Sender)
{
    int i, count=0, iGroup;
    bool bTempFinish[tcTotalCount]={false};
    AnsiString str="";
    static bool bTimerRun=false;
    static int iRetry=0;
    static int iCurrentBase=-1;
    TDateTime myDateTime;
    AnsiString StrOff;
    AnsiString StrTmp;
    bool bFlag=true;

    if(InitialOK==false || bTimerRun==true)
        return;

    bTimerRun=true;

    if(fNote->fShow==true)
    {
        bAutoStart=false;
        ChangeStartState();
    }

    if(ACTData.iThermoCtrlType==Type_Agilent34970A)
    {
        if(tTempScanTimer.Off())                       // Sec.
        {
            str="MEAS:TEMP? TC,K,(@";
            for(i=0; i<(int)myATPal.size(); i++)
            {
                if(myATPal[i]->cbEnable->Checked)
                {
                    count++;
                    str+=AnsiString(myATPal[i]->_ChannelNo)+",";
                }
            }
            if(count!=0)
            {
                str.Delete(str.Length(), 1);
                str+=")\n";
                SendCommand(str);
            }
            tTempScanTimer.SetSecAndOn(ACTData.iReadScanTime);
        }
    }

    if(ACTData.iThermoCtrlType==Type_DeltaDTB4824)  //Steven 20210510 : 自動K溫使用DTB4824
    {
        if(iReceiveData==0)         //第一次進來
        {
            iRetry=0;
            DTB4824_ReadPV(iAddr);
        }
        else if(iReceiveData==2)    //接收完成
        {
            iAddr++;
            if(iAddr>=(int)myATPal.size())
            {
                iAddr=0;
            }
            iRetry=0;
            DTB4824_ReadPV(iAddr);
        }
        else if(iReceiveData==1)  //等待接收完成
        {
            if(tReadTimeOut.Off())
            {
                iRetry++;
                if(iRetry>2)
                {
                    iRetry=0;
                    iAddr++;
                    if(iAddr>=(int)myATPal.size())
                    {
                        iAddr=0;
                    }
                }
                DTB4824_ReadPV(iAddr);
            }
        }
    }

    if(fNote->fShow==false && bAutoStart==false)             //Steven 20210512 : 定時自動K溫
    {
        if(chkAutoStart->Checked==true &&
           SystemHour==atoi(edtAutoStartHr->Text.c_str())  &&
           SystemMin ==atoi(edtAutoStartMin->Text.c_str()) &&
           SystemSec ==0)
        {
            btnAutoStart->Click();
        }
    }

    if(bAutoStart==true)
    {
        int iBaseIndex = -1;
        double fTarget = 30.;
        double fOffset = 0.0;

        fHeaterOK=false;
        for(int i=0; i<3; i++)                                                  //Jimmychiu 20230307 修正for迴圈次數 i<=3->i<3 避免溢位錯誤
        {
            if(_cbBase[i]->Checked==true && bBaseFinish[i]==false)
            {
                iBaseIndex = i;
                fTarget    = atof(_pnlBaseTemp[iBaseIndex]->Caption.c_str());
                if(iCurrentBase!=iBaseIndex)
                {
                    fMain->edWorkTemperBase->Text=fTarget;
                    fMain->SetTemp(false, atof(fMain->edWorkTemperBase->Text.c_str()), atof(fMain->edSoakTime->Text.c_str()));
                    iCurrentBase=iBaseIndex;
                    TDateTime myDateTime=Now();
                    StrOff=myDateTime.FormatString("hh:nn:ss")+",\tOffs."+_pnlBaseTemp[iBaseIndex]->Caption;
                    StrTmp=myDateTime.FormatString("hh:nn:ss")+",\tTemp."+_pnlBaseTemp[iBaseIndex]->Caption;

                    for(int j=0; j<(int)myATPal.size(); j++)
                    {
                        if(ACTData.iThermoCtrlType==Type_DeltaDTB4824)  //Steven 20210510 : 自動K溫使用DTB4824
                            iGroup=myATPal[j]->_Group;
                        else
                            iGroup=j;
                        if(myATPal[j]->cbEnable->Checked==true)
                        {
                            StrOff+=",\t"+AnsiString(Temperature.fTempOffSet[iBaseIndex][iGroup]);
                            StrTmp+=",\t"+myATPal[j]->pnlTemp->Caption;
                        }
                    }
                    MemoOffset->Lines->Add(StrOff);
                    MemoOffset->Lines->Add(StrTmp);
                }
                break;
            }
        }

        if(iBaseIndex>=0 && iBaseIndex<=2)
        {
            if(tBaseTempBlinkTimer.Off())
            {
                if(_pnlBaseTemp[iBaseIndex]->Color==clBtnFace)
                {
                    _pnlBaseTemp[iBaseIndex]->Color=(TColor)0x0000C600;
                }
                else
                {
                    _pnlBaseTemp[iBaseIndex]->Color=clBtnFace;
                }

                tBaseTempBlinkTimer.SetMSAndOn(400);
            }

            str.sprintf("Auto Offset Timer: %02d:%02d", tCheckIntervalTimer.LatchCycleTimeSec()/60, tCheckIntervalTimer.LatchCycleTimeSec()%60);
            labTimer->Caption=str;

            if(tCheckIntervalTimer.LatchCycleTimeSec()>=(ACTData.iCheckIntervalTime*60))     // Min.
            {
                if(ACTData.iThermoCtrlType==Type_Agilent34970A)
                {
                    for(int x=0; x<(int)myATPal.size(); x++)
                    {
                        if(myATPal[x]->cbEnable->Checked==true)
                        {
                            if(fTarget-UN150Read[x]<10)  //超過五度表示還在升溫,所以先不補。
                            {
                                double fRange=(double)ACTData.iCalibrationRange/2;
                                if(myATPal[x]->fTemperatureData>=(fTarget+fRange))        // Over
                                {
                                    fOffset=(myATPal[x]->fTemperatureData-fTarget);
                                    if(fOffset<ACTData.iOffsetLimit)
                                    {
                                        if(fOffset>ACTData.iSingleOffset || fOffset<-ACTData.iSingleOffset)
                                            fOffset>ACTData.iSingleOffset;

                                        Temperature.fTempOffSet[iBaseIndex][x]-=fOffset;
                                        myATPal[x]->ImageShowCurrentOffset(iBaseIndex, Temperature.fTempOffSet[iBaseIndex][x]);
                                    }
                                }
                                else if(myATPal[x]->fTemperatureData<fTarget-fRange)   // Below
                                {
                                    fOffset=fTarget-myATPal[x]->fTemperatureData;
                                    if(fOffset<=ACTData.iOffsetLimit)
                                    {
                                        if(fOffset>ACTData.iSingleOffset || fOffset<-ACTData.iSingleOffset)
                                            fOffset>ACTData.iSingleOffset;

                                        Temperature.fTempOffSet[iBaseIndex][x]+=fOffset;
                                        myATPal[x]->ImageShowCurrentOffset(iBaseIndex, Temperature.fTempOffSet[iBaseIndex][x]);
                                    }
                                }
                                else
                                {
                                    bTempFinish[x]=true;
                                }

                                if(Temperature.fTempOffSet[iBaseIndex][x]>ACTData.iOffsetLimit)
                                {
                                    Temperature.fTempOffSet[iBaseIndex][x]=ACTData.iOffsetLimit;
                                    myATPal[x]->ImageShowCurrentOffset(iBaseIndex, Temperature.fTempOffSet[iBaseIndex][x]);
                                }
                                else if(Temperature.fTempOffSet[iBaseIndex][x]<-ACTData.iOffsetLimit)
                                {
                                    Temperature.fTempOffSet[iBaseIndex][x]=-ACTData.iOffsetLimit;
                                    myATPal[x]->ImageShowCurrentOffset(iBaseIndex, Temperature.fTempOffSet[iBaseIndex][x]);
                                }
                            }
                        }
                        else
                        {
                            bTempFinish[x]=true;
                        }
                    }
                    bFlag=true;
                    for(int x=0; x<(int)myATPal.size(); x++)
                    {
                        if(bTempFinish[x]==false)
                        {
                            bFlag=false;
                        }
                    }
                }
                else    //Steven 20210510 : 自動K溫使用DTB4824
                {
                    //先推算各Group應該要補幾度
                    for(int iG=0; iG<iTotalGroupCount; iG++)
                    {
                        dGroupMax[iG]=0.0;
                        dGroupMin[iG]=0.0;
                        dGroupSum[iG]=0.0;
                        iGroupCount[iG]=0;  //各群組內的數量
                        for(int x=0; x<(int)myATPal.size(); x++)
                        {
                            if(myATPal[x]->cbEnable->Checked==true && myATPal[x]->_Group==iGroupSet[iG])
                            {
                                iGroupCount[iG]++;
                                if(ACTData.iOffsetMethod==0)
                                {
                                    if(dGroupMax[iG]==0.0 || myATPal[x]->fTemperatureData>dGroupMax[iG])
                                        dGroupMax[iG]=myATPal[x]->fTemperatureData;

                                    if(dGroupMin[iG]==0.0 || myATPal[x]->fTemperatureData<dGroupMin[iG])
                                        dGroupMin[iG]=myATPal[x]->fTemperatureData;
                                }
                                else
                                {
                                    dGroupSum[iG]+=myATPal[x]->fTemperatureData;
                                }
                            }
                        }
                        dGroupAvg[iG]=0.0;
                        if(iGroupCount[iG]!=0)
                        {
                            if(ACTData.iOffsetMethod==0)
                            {
                                if(dGroupMax[iG]!=dGroupMin[iG])
                                    dGroupAvg[iG]=(dGroupMax[iG]+dGroupMin[iG])/2.0;
                                else
                                    dGroupAvg[iG]=dGroupMin[iG];
                            }
                            else
                            {
                                dGroupAvg[iG]=dGroupSum[iG]/(double)iGroupCount[iG];
                            }
                        }
                    }

                    for(int iG=0; iG<iTotalGroupCount; iG++)
                    {
                        iGroup=iGroupSet[iG];
                        //計算並寫入每個Group的Offset資料
                        if(dGroupAvg[iG]!=0.0)
                        {
                            if(fTarget-UN150Read[iGroup]<10)  //超過五度表示還在升溫,所以先不補。
                            {
                                double fRange=(double)ACTData.dCalibrationRange[iBaseIndex]/2.0;    //Steven 20210520 : K溫範圍改成三組
                                if(dGroupAvg[iGroup]>=(fTarget+fRange))        // Over
                                {
                                    fOffset=(dGroupAvg[iGroup]-fTarget);
                                    if(fOffset<ACTData.iOffsetLimit)
                                    {
                                        if(fOffset>ACTData.iSingleOffset || fOffset<-ACTData.iSingleOffset)
                                            fOffset>ACTData.iSingleOffset;

                                        Temperature.fTempOffSet[iBaseIndex][iGroup]-=fOffset;
                                    }
                                }
                                else if(dGroupAvg[iGroup]<fTarget-fRange)   // Below
                                {
                                    fOffset=fTarget-dGroupAvg[iGroup];
                                    if(fOffset<=ACTData.iOffsetLimit)
                                    {
                                        if(fOffset>ACTData.iSingleOffset || fOffset<-ACTData.iSingleOffset)
                                            fOffset>ACTData.iSingleOffset;

                                        Temperature.fTempOffSet[iBaseIndex][iGroup]+=fOffset;
                                    }
                                }
                                else
                                {
                                    bTempFinish[iGroup]=true;
                                }

                                if(Temperature.fTempOffSet[iBaseIndex][iGroup]>ACTData.iOffsetLimit)
                                {
                                    Temperature.fTempOffSet[iBaseIndex][iGroup]=ACTData.iOffsetLimit;
                                }
                                else if(Temperature.fTempOffSet[iBaseIndex][iGroup]<-ACTData.iOffsetLimit)
                                {
                                    Temperature.fTempOffSet[iBaseIndex][iGroup]=-ACTData.iOffsetLimit;
                                }
                            }

                            //顯示目前Offset數值
                            for(int x=0; x<(int)myATPal.size(); x++)
                            {
                                if(myATPal[x]->cbEnable->Checked==true && myATPal[x]->_Group==iGroupSet[iGroup])
                                {
                                    myATPal[x]->ImageShowCurrentOffset(iBaseIndex, Temperature.fTempOffSet[iBaseIndex][iGroup]);
                                }
                            }
                        }
                        else
                        {
                            bTempFinish[iG]=true;
                        }
                    }

                    bFlag=true;
                    for(int x=0; x<iTotalGroupCount; x++)
                    {
                        if(bTempFinish[x]==false)
                        {
                            bFlag=false;
                        }
                    }
                }
                bBaseFinish[iBaseIndex]=bFlag;

                fTemp_Set->DoIniDataToForm(true);
                MySleep(100);
                fTemp_Set->spbSave->Click();

                TDateTime myDateTime=Now();
                StrOff=myDateTime.FormatString("hh:nn:ss")+",\tOffs."+_pnlBaseTemp[iBaseIndex]->Caption;
                StrTmp=myDateTime.FormatString("hh:nn:ss")+",\tTemp."+_pnlBaseTemp[iBaseIndex]->Caption;

                for(int i=0; i<(int)myATPal.size(); i++)
                {
                    if(ACTData.iThermoCtrlType==Type_DeltaDTB4824)  //Steven 20210510 : 自動K溫使用DTB4824
                        iGroup=myATPal[i]->_Group;
                    else
                        iGroup=i;
                    if(myATPal[i]->cbEnable->Checked==true)
                    {
                        StrOff+=",\t"+AnsiString(Temperature.fTempOffSet[iBaseIndex][iGroup]);
                        StrTmp+=",\t"+myATPal[i]->pnlTemp->Caption;
                    }
                }
                MemoOffset->Lines->Add(StrOff);
                MemoOffset->Lines->Add(StrTmp);

                tCheckIntervalTimer.LatchCycleTimeSec(true);
            }
        }

        for(int i=0; i<3; i++)           //Jimmychiu 20230307 修正for迴圈次數 i<=3->i<3 避免溢位錯誤
        {
            if(_cbBase[i]->Checked==true && bBaseFinish[i]==true)
            {
                _pnlBaseTemp[i]->Color=(TColor)0x00A6A600;
            }
        }

        if(bBaseFinish[0]==true && bBaseFinish[1]==true && bBaseFinish[2]==true)
        {
            palSaveLogClick(this);
            btnAutoStart->Click();
        }
    }
    else
    {
        iCurrentBase=-1;
    }

    UpdateTemperatureData();
    bTimerRun=false;
}
//---------------------------------------------------------------------------
bool TACTForm::SaveCommData(const TComm* Comm)
{
    TIniFile* IniFile1=new TIniFile(FilePath);
    IniFile1->WriteString ("COMPort", "CommName", CommName       );
    IniFile1->WriteInteger("COMPort", "BaudRate", Comm->BaudRate);
    IniFile1->WriteInteger("COMPort", "ByteSize", Comm->ByteSize);
    IniFile1->WriteInteger("COMPort", "StopBits", Comm->StopBits);
    IniFile1->WriteInteger("COMPort", "Parity"  , Comm->Parity   );
    delete IniFile1;
    return true;
}
//---------------------------------------------------------------------------
bool TACTForm::LoadCommData(TComm* Comm)
{
    TIniFile* IniFile1=new TIniFile(FilePath);
    CommName       =           IniFile1->ReadString ("COMPort", "CommName", "COM3");
    Comm->CommName =           "\\\\.\\"+CommName;
    Comm->BaudRate =           IniFile1->ReadInteger("COMPort", "BaudRate", 9600  );
    Comm->ByteSize = TByteSize(IniFile1->ReadInteger("COMPort", "ByteSize", _8)   );
    Comm->StopBits = TStopBits(IniFile1->ReadInteger("COMPort", "StopBits", _1)   );
    Comm->Parity   = TParity  (IniFile1->ReadInteger("COMPort", "Parity"  , None));
    delete IniFile1;
    return true;
}
//---------------------------------------------------------------------------
void __fastcall TACTForm::FormResize(TObject *Sender)
{
    btnAutoStart->Top   = pnlData->Height-50;
    btnDutOnOff ->Top   = btnAutoStart->Top-50;
    btShowPos ->Top     = btnDutOnOff->Top-50;
}
//---------------------------------------------------------------------------
void __fastcall TACTForm::btnDutOnOffClick(TObject *Sender)
{
    int iGroup;
    for(int i=0; i<(int)myATPal.size(); i++)
    {
        if(ACTData.iThermoCtrlType==Type_Agilent34970A)
            iGroup=i;
        else        //Steven 20210510 : 自動K溫使用DTB4824
            iGroup=myATPal[i]->_Group;
        myATPal[i]->cbEnable->Checked      =bUT150Install[iGroup];
        myATPal[i]->gbMeasurePoint->Visible=bUT150Install[iGroup];
    }
//    myATPal[tcCCD]->cbEnable->Checked=false;
//    if(REAL_TIME_CCD==true && RTC_TemperNumber==2)      //Isaac 20201217 : RTC CCD增加第二組感溫
//    {
//        myATPal[tcCCD_2]->cbEnable->Checked=false;
//    }
    ShowDefaultPos(true);
}
//---------------------------------------------------------------------------
void TACTForm::ShowDefaultPos(bool bHide)
{
    int count=0;
    for(int i=0; i<(int)myATPal.size(); i++)
    {
        myATPal[i]->gbMeasurePoint->Left=4+(myATPal[i]->gbMeasurePoint->Width+10)*(i%ACTData.iDisplayColumn);

        if(ACTData.iDisplayColumn==1 && bHide)
        {
            if(myATPal[i]->cbEnable->Checked)
            {
                myATPal[i]->gbMeasurePoint->Top=4+(myATPal[i]->gbMeasurePoint->Height+10)*count;
                count++;
            }
        }
        else
        {
            myATPal[i]->gbMeasurePoint->Top=4+(myATPal[i]->gbMeasurePoint->Height+10)*(ChangeToFloatNonPcnt((double)(i), (double)(ACTData.iDisplayColumn)));
            myATPal[i]->gbMeasurePoint->Visible=true;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TACTForm::btShowPosClick(TObject *Sender)
{
    ShowDefaultPos();
}
//---------------------------------------------------------------------------
void __fastcall TACTForm::palSaveLogClick(TObject *Sender)
{
    AnsiString Str="D:\\HT9045_Log\\AutoTempCalibration\\";
    MyForceDirectories(OffsetPath);
    Str.sprintf("D:\\HT9045_Log\\AutoTempCalibration\\%04d-%02d-%02d %02d_%02d_%02d.txt", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
    MemoOffset->Lines->SaveToFile(Str);
}
//---------------------------------------------------------------------------
void TACTForm::DTB4824_ReadPV(int Addr)
{
    int iCommand=4700;
    AnsiString TempStr="";
    READBUFF_DTB.sprintf(":%02X03%04d0002", Addr+1, iCommand);
    AnsiString LRC=DTK4848_LRC(READBUFF_DTB);
    READBUFF_DTB.sprintf(":%02X03%04d0002%s\r\n", Addr+1, iCommand, LRC); //Polling 現在溫度
    iReceiveData=1;
    tReadTimeOut.SetSecAndOn(1);
    SendCommand(READBUFF_DTB);
}
//---------------------------------------------------------------------------
void __fastcall TACTForm::edtAutoStartHrMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 2, true, 0, 23);
}
//---------------------------------------------------------------------------
void __fastcall TACTForm::edtAutoStartMinMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 2, true, 0, 59);
}
//---------------------------------------------------------------------------
void __fastcall TACTForm::edtRange1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_DOUBLE, 2, true, 0.1, 10.0);
}
//---------------------------------------------------------------------------
void __fastcall TACTForm::edtCheckIntervalTimeMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TEdit *Ptr;
    Ptr=(TEdit *)Sender;

    if(Ptr->Name=="edtCheckIntervalTime")
    {
        fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 30, 1);
    }
    else if(Ptr->Name=="edtCalibrationRange")
    {
        fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 10, 1);
    }
    else if(Ptr->Name=="edtReadScanTime")
    {
        fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 10, 1);
    }
    else if(Ptr->Name=="edtOffsetLimit")
    {
        fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 30, 1);
    }
    else if(Ptr->Name=="edSingleOffset")
    {
        fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 30, 1);
    }
    else if(Ptr->Name=="edColumns")
    {
        fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 16, 1);
    }
}
//---------------------------------------------------------------------------