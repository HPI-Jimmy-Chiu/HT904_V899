#include "MachineDefine.h"
#pragma hdrstop
#include "myQwertyKeyBoard.h"
#include "WinWaySetting.h"
#include "cmydef.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SPComm"
#pragma resource "*.dfm"
TfWinway *fWinway;
//---------------------------------------------------------------------------
__fastcall TfWinway::TfWinway(TComponent* Owner)
        : TForm(Owner)
{
    edtSetTemp->Text="0";
    Width =330;
    Height=518;
    iWinwayATCIndex=0;

    bShow=false;
    //
    arrATC_Site[0]=new ATC_WinWay(WinWayATCComm1);
    arrATC_Site[1]=new ATC_WinWay(WinWayATCComm2);
    arrATC_Site[2]=new ATC_WinWay(WinWayATCComm3);
    arrATC_Site[3]=new ATC_WinWay(WinWayATCComm4);
    //
    for(int i=0; i<SiteNum; i++)
    {
        LoadCommData(arrATC_Site[i]->WinwayCOM , i);
        SaveCommData(arrATC_Site[i]->WinwayCOM , i);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfWinway::FormShow(TObject *Sender)
{
    Left=(1024-Width)/2;
    Top =(768-Height)/2;
    cbbWinwayATCIndex->ItemIndex=iWinwayATCIndex;
    ShowCommData(iWinwayATCIndex);
    bShow=true;
}
//---------------------------------------------------------------------------
void TfWinway::ShowCommData(int iIndex)
{
    ATC_WinWay* tempATC_WinWay=arrATC_Site[iIndex];
    cbbDevice->Text=tempATC_WinWay->sWinwayCommName;
    cbbBaudRate->Text=AnsiString(tempATC_WinWay->WinwayCOM->BaudRate);

    if(tempATC_WinWay->WinwayCOM->ByteSize==_5)
        cbbByteSize->ItemIndex=0;
    else if(tempATC_WinWay->WinwayCOM->ByteSize==_6)
        cbbByteSize->ItemIndex=1;
    else if(tempATC_WinWay->WinwayCOM->ByteSize==_7)
        cbbByteSize->ItemIndex=2;
    else if(tempATC_WinWay->WinwayCOM->ByteSize==_8)
        cbbByteSize->ItemIndex=3;

    if(tempATC_WinWay->WinwayCOM->StopBits==_1)
        cbbStopBit->ItemIndex=0;
    else if(tempATC_WinWay->WinwayCOM->StopBits==_1_5)
        cbbStopBit->ItemIndex=1;
    else if(tempATC_WinWay->WinwayCOM->StopBits ==_2)
        cbbStopBit->ItemIndex=2;

    if(tempATC_WinWay->WinwayCOM->Parity==None)
        cbbParity->ItemIndex=0;
    else if(tempATC_WinWay->WinwayCOM->Parity==Odd)
        cbbParity->ItemIndex=1;
    else if(tempATC_WinWay->WinwayCOM->Parity==Even)
        cbbParity->ItemIndex=2;
    else if(tempATC_WinWay->WinwayCOM->Parity==Space)
        cbbParity->ItemIndex=4;
    //
    lblShowPT->Caption=FloatToStr(tempATC_WinWay->GetPT());
    edtSetTemp->Text=FloatToStr(tempATC_WinWay->GetST());
}
//---------------------------------------------------------------------------
void __fastcall TfWinway::btnUpdateClick(TObject *Sender)
{
    ATC_WinWay* tempATC_WinWay=arrATC_Site[iWinwayATCIndex];
    tempATC_WinWay->sWinwayCommName=cbbDevice->Text;
    tempATC_WinWay->WinwayCOM->CommName="\\\\.\\"+tempATC_WinWay->sWinwayCommName;
    tempATC_WinWay->WinwayCOM->BaudRate=atoi(cbbBaudRate->Text.c_str());

    if(cbbByteSize->ItemIndex==0)
        tempATC_WinWay->WinwayCOM->ByteSize=_5;
    else if(cbbByteSize->ItemIndex==1)
        tempATC_WinWay->WinwayCOM->ByteSize=_6;
    else if(cbbByteSize->ItemIndex==2)
        tempATC_WinWay->WinwayCOM->ByteSize=_7;
    else if(cbbByteSize->ItemIndex==3)
        tempATC_WinWay->WinwayCOM->ByteSize=_8;

    if(cbbStopBit->ItemIndex==0)
        tempATC_WinWay->WinwayCOM->StopBits=_1;
    else if(cbbStopBit->ItemIndex==1)
        tempATC_WinWay->WinwayCOM->StopBits=_1_5;
    else if(cbbStopBit->ItemIndex==2)
        tempATC_WinWay->WinwayCOM->StopBits=_2;

    if(cbbParity->ItemIndex==0)
        tempATC_WinWay->WinwayCOM->Parity=None;
    else if(cbbParity->ItemIndex==1)
        tempATC_WinWay->WinwayCOM->Parity=Odd;
    else if(cbbParity->ItemIndex==2)
        tempATC_WinWay->WinwayCOM->Parity=Even;
    else if(cbbParity->ItemIndex==4)
        tempATC_WinWay->WinwayCOM->Parity=Space;

    SaveCommData(tempATC_WinWay->WinwayCOM, iWinwayATCIndex);
    tempATC_WinWay->CloseCommPort();
    tempATC_WinWay->OpenCommPort();
    ShowMessage("¡iUpdate¡j Success");
}
//---------------------------------------------------------------------------
bool TfWinway::LoadCommData(TComm* Comm, int iIndex)
{
    ATC_WinWay* tempATC_WinWay=arrATC_Site[iIndex];
    TIniFile* IniFile=new TIniFile(ATCWinWayPath);

    AnsiString sSection="COMPort"+AnsiString(iIndex+1);
    AnsiString sSectionComport="COM"+AnsiString(iIndex+15);
    tempATC_WinWay->sWinwayCommName=IniFile->ReadString(sSection, "CommName", sSectionComport);
    Comm->CommName="\\\\.\\" + tempATC_WinWay->sWinwayCommName;
    Comm->BaudRate=(unsigned) IniFile->ReadInteger(sSection, "BaudRate", 9600);
    Comm->ByteSize=(TByteSize)IniFile->ReadInteger(sSection, "ByteSize", _8);
    Comm->StopBits=(TStopBits)IniFile->ReadInteger(sSection, "StopBits", _1);
    Comm->Parity  =(TParity)  IniFile->ReadInteger(sSection, "Parity"  , None);
    //set temprature
    tempATC_WinWay->SetST(IniFile->ReadFloat(sSection, "SetTemperature", 0.0));
    delete IniFile;
    return true;
}
//---------------------------------------------------------------------------
bool TfWinway::SaveCommData(TComm* Comm, int iIndex)
{
    ATC_WinWay* tempATC_WinWay =arrATC_Site[iIndex];
    TIniFile* IniFile   =new TIniFile(ATCWinWayPath);
    AnsiString sSection ="COMPort"+AnsiString(iIndex+1);
    IniFile->WriteString( sSection, "CommName"    , tempATC_WinWay->sWinwayCommName);
    IniFile->WriteInteger(sSection, "BaudRate"    , Comm->BaudRate);
    IniFile->WriteInteger(sSection, "ByteSize"    , Comm->ByteSize);
    IniFile->WriteInteger(sSection, "StopBits"    , Comm->StopBits);
    IniFile->WriteInteger(sSection, "Parity"      , Comm->Parity);
    IniFile->WriteFloat  (sSection, "SetTemperature" , tempATC_WinWay->GetST());
    delete IniFile;
    return true;
}
//---------------------------------------------------------------------------
void __fastcall TfWinway::cbbWinwayATCIndexChange(TObject *Sender)
{
    iWinwayATCIndex=cbbWinwayATCIndex->ItemIndex;
    ShowCommData(iWinwayATCIndex);
}
//---------------------------------------------------------------------------
bool TfWinway::OpenCommPort(int _idx)
{
    if(arrATC_Site[_idx]->OpenCommPort())
        return true;
    else
        return false;
}
//---------------------------------------------------------------------------
void TfWinway::OpenCommPort()
{
    for(int i=0; i<SiteNum; i++)
        OpenCommPort( i );
}
//---------------------------------------------------------------------------
void TfWinway::SetST()
{
    for(int i=0; i<SiteNum; i++)
        arrATC_Site[i]->SetST(arrATC_Site[i]->GetST());
}
//---------------------------------------------------------------------------
bool TfWinway::CloseCommPort(int _idx)
{
    if(arrATC_Site[_idx]->CloseCommPort())
        return true;
    else
        return false;
}
//---------------------------------------------------------------------------
void __fastcall TfWinway::btnSendTempClick(TObject *Sender)
{
    SetTemprature(StrToFloat(edtSetTemp->Text));
    ShowMessage("¡isend¡jST="+edtSetTemp->Text);
}
//---------------------------------------------------------------------------
void  TfWinway::SetTemprature(double _value)
{
    return arrATC_Site[iWinwayATCIndex]->SetST(_value);
}
//---------------------------------------------------------------------------
void  TfWinway::SetTempratureAll(double _value)
{
    for(int i=0; i<SiteNum; i++)
    {
        arrATC_Site[i]->SetST(_value);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfWinway::btnGetPVClick(TObject *Sender)
{
    lblShowPT->Caption=FloatToStr(arrATC_Site[iWinwayATCIndex]->GetPT());
}
//---------------------------------------------------------------------------
void __fastcall TfWinway::WinWayATCComm1ReceiveData(TObject *Sender,
      Pointer Buffer, WORD BufferLength)
{
    if(BufferLength<7)
        return;
    TComm *tempTComm;
    tempTComm = (TComm *)Sender;
    ATC_WinWay* _atc;
    if(tempTComm->Name=="WinWayATCComm1")
    {
      _atc=arrATC_Site[0];
    }
    else if(tempTComm->Name=="WinWayATCComm2")
    {
      _atc=arrATC_Site[1];
    }
    else if(tempTComm->Name=="WinWayATCComm3")
    {
      _atc=arrATC_Site[2];
    }
    else if(tempTComm->Name=="WinWayATCComm4")
    {
      _atc=arrATC_Site[3];
    }
    _atc->SetPT((char*)Buffer);
}
//---------------------------------------------------------------------------
void __fastcall TfWinway::edtSetTempClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_DOUBLE, 1, true, dTempMax, 0.0);
}
//---------------------------------------------------------------------------