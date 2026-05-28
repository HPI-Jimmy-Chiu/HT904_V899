//---------------------------------------------------------------------------
#include "MachineDefine.h"
#pragma hdrstop

#include "myQwertyKeyBoard.h"
#include "LoadCCDMap.h"
#include "MyMotor.h"
#include "common.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "MyLed"
#pragma link "ALed"
#pragma link "ALed"
#pragma link "MyLed"
#pragma resource "*.dfm"
TLoadCCD *LoadCCD;

TQPF_Timer CCDDataDelay;
bool bCCDTest=false;
//---------------------------------------------------------------------------
__fastcall TLoadCCD::TLoadCCD(TComponent* Owner)
    : TForm(Owner)
{

}
//---------------------------------------------------------------------------
void __fastcall TLoadCCD::btnConnectClick(TObject *Sender)
{
    ServerSocket1->Active=true;
    ServerSocket1->Port  =StrToInt(edtPort->Text);
}
//---------------------------------------------------------------------------
void __fastcall TLoadCCD::ServerSocket1ClientRead(TObject *Sender,
      TCustomWinSocket *Socket)
{
     memset(pucInBuff, 0, 8191);
    int iReceiveLength, X=0, Y=0;
    int iSPos=0, iEPos=0;
    int iBufferLength=0;
    AnsiString asData="", asTemp="", asBuffer="", asBuffer1="";
    int iData[1000]={0};
    int iData1[1000]={0};

    try
    {
        iReceiveLength=Socket->ReceiveLength();
        Socket->ReceiveBuf(pucInBuff, iReceiveLength);
        asData=pucInBuff;

        if(Memo1->Lines->Count>50)
        {
            MyForceDirectories("D:\\HT9045_Log\\LoadCCD");

            asBuffer1.sprintf("D:\\HT9045_Log\\LoadCCD\\%s.txt", AnsiString(Now().FormatString("yyyymmdd-hh-nn")));
            Memo1->Lines->SaveToFile(asBuffer1);
            //Memo1->Lines->Delete(0);
            Memo1->Clear();
        }
        Memo1->Lines->Add(AnsiString(Now().FormatString("yyyy/m/d hh:nn:ss"))+" [Receive]=>"+asData);

        if(asData.Pos("CCD1,")>=1)
        {
            iSPos=asData.Pos(",");
            iEPos=asData.Pos("_");
            asTemp  =asData.SubString(iSPos+1, iEPos-iSPos-1).Trim();           //CCD1,4_1_2_1_1_2_1     取出 4 個有 IC
            asBuffer=asData.SubString(iEPos+1, iReceiveLength-iEPos).Trim();    //       1_2_1_1_2_1     取出 後面資料
            iBufferLength=asBuffer.Length();                                    //       1_2_1_1_2_1     取出 後面資料 總共多少
            MOT[MMTrayY].fHasTray=true;
            iICCount=StrToInt(asTemp);                                          //CCD IC 有多少顆
            for(int i=0; i<iBufferLength; i++)
            {
                iSPos = asBuffer.Pos("_");
                if(iSPos==0)
                {
                    asBuffer1=asBuffer.SubString(0, 1).Trim();
                    iData[i] =StrToInt(asBuffer1);
                    asBuffer1=asBuffer.SubString(iSPos+1, iBufferLength).Trim();
                    asBuffer =asBuffer1;

                    iTrayMappingDate[X][Y]=StrToInt( asBuffer1);
                    if(X<MOT[MMTrayY].Tray.XItem)
                    {
                        if(iData[i]==1)
                            MOT[MMTrayY].SetTraySingleData(X, Y, HAS_IC);
                        else
                            MOT[MMTrayY].SetTraySingleData(X, Y, NULL_IC);
                        X++;
                    }
                    else if(Y<MOT[MMTrayY].Tray.YItem)
                    {
                        if(iData[i]==1)
                            MOT[MMTrayY].SetTraySingleData(X, Y, HAS_IC);
                        else
                            MOT[MMTrayY].SetTraySingleData(X, Y, NULL_IC);
                        X=0;
                        Y++;
                    }
                    break;
                }
                else
                {
                    asBuffer1=asBuffer.SubString(0, 1).Trim();
                    iData[i] =StrToInt(asBuffer1);
                    if(X<MOT[MMTrayY].Tray.XItem)
                    {
                        if(iData[i]==1)
                            MOT[MMTrayY].SetTraySingleData(X, Y, HAS_IC);
                        else
                            MOT[MMTrayY].SetTraySingleData(X, Y, NULL_IC);
                        X++;
                    }
                    else if(Y<MOT[MMTrayY].Tray.YItem)
                    {
                        X=0;
                        Y++;
                        if(iData[i]==1)
                            MOT[MMTrayY].SetTraySingleData(X, Y, HAS_IC);
                        else
                            MOT[MMTrayY].SetTraySingleData(X, Y, NULL_IC);
                        X++;
                    }
                    asBuffer1=asBuffer.SubString(iSPos+1, iBufferLength).Trim();
                    asBuffer =asBuffer1;
                }
            }
            MOT[MMTrayY].Refresh();
            bReceiv=true;                                                       //收到CCD 資料
        }

        if(asData.Pos("CCD2,")>=1)
        {
            iSPos   =asData.Pos(",");
            iEPos   =asData.Pos("_");
            asTemp  =asData.SubString(iSPos+1, iEPos-iSPos-1).Trim();           //CCD1,4_1_2_1_1_2_1     取出 4 個有 IC
            asBuffer=asData.SubString(iEPos+1, iReceiveLength-iEPos).Trim();    //       1_2_1_1_2_1     取出 後面資料
            iBufferLength=asBuffer.Length();                                    //       1_2_1_1_2_1     取出 後面資料 總共多少
            for(int i=0; i<iBufferLength; i++)
            {
                iSPos=asBuffer.Pos("_");
                if(iSPos==0)
                {
                    asBuffer1=asBuffer.SubString(0, 1).Trim();
                    iData1[i]=StrToInt(asBuffer1);
                    asBuffer1=asBuffer.SubString(iSPos+1, iBufferLength).Trim();
                    asBuffer =asBuffer1;
                    break;
                }
                else
                {
                    asBuffer1=asBuffer.SubString(0, 1).Trim();
                    iData1[i]=StrToInt(asBuffer1);
                    asBuffer1=asBuffer.SubString(iSPos+1, iBufferLength).Trim();
                    asBuffer =asBuffer1;
                }
            }
        }

    }
    catch(...)
    {
        //MyDBIProcess("Exception", "ATC7_ServerSocketClientRead");
    }
}
//---------------------------------------------------------------------------
void __fastcall TLoadCCD::ServerSocket1ClientConnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    LedAConnect->Value=true;
    bLoadServoSocketConnect=true;
}
//---------------------------------------------------------------------------
void __fastcall TLoadCCD::ServerSocket1ClientDisconnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    LedAConnect->Value=false;
}
//---------------------------------------------------------------------------
void __fastcall TLoadCCD::CCD1Click(TObject *Sender)
{
    CCDCommand[0].sprintf("CCD1,1,@CCD1,0");                                    //送給CCD 拍照 命令
    SendData(CCDCommand[0]);
}
//---------------------------------------------------------------------------
void __fastcall TLoadCCD::CCD2Click(TObject *Sender)
{
    CCDCommand[1].sprintf("CCD2,1,@CCD2,0");                                    //送給CCD 拍照 命令
    SendData(CCDCommand[1]);
}
//---------------------------------------------------------------------------
void TLoadCCD::SendData(AnsiString sData)
{
    if(ServerSocket1->Socket->ActiveConnections==1)
    {
        ServerSocket1->Socket->Connections[0]->SendText(sData);
        Memo1->Lines->Add(AnsiString(Now().FormatString("yyyy/m/d hh:nn:ss"))+" [Send]=>"+sData);
    }
    else
    {
        ServerSocket1->Close();
        ServerSocket1->Open();
    }
}
//---------------------------------------------------------------------------
bool TLoadCCD::InitialLoadCCD()
{
    CCDTask=1;
    return false;
}
//---------------------------------------------------------------------------
int TLoadCCD::LoadCCDData()
{
    int &Task=CCDTask;
    switch(Task)
    {
        case 1:
            iICCount=0;
            if(ServerSocket1->Active==true &&bLoadServoSocketConnect)
            {
                Task++;
            }
            else
            {
                try
                {
                    ServerSocket1->Open();
                }
                catch(...)
                {
                }
                Task++;
            }
            break;
        case 2:
            bReceiv=false;                                                      //收到CCD 資料
            CCDCommand[0].sprintf("CCD1,1,@CCD1,0");                            //送給CCD 拍照 命令
            SendData(CCDCommand[0]);
            CCDDataDelay.SetSecAndOn(5);
            Task++;
            break;
        case 3:
            if(bReceiv)                                                         //收到CCD 資料
            {
                Task++;
            }
            else if(CCDDataDelay.Off())                                         //LOAD CCD 斷線
            {
                Task=2;
            }
            break;
        case 4:
            ServerSocket1->Active=false;
            return iICCount;
    }
    return 999;
}
//---------------------------------------------------------------------------
void __fastcall TLoadCCD::Timer1Timer(TObject *Sender)
{
    if(bCCDTest)
    {
        if(LoadCCDData()!=999)
            bCCDTest=false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TLoadCCD::btnCCDTestClick(TObject *Sender)
{
    bCCDTest=true;
    InitialLoadCCD();
}
//---------------------------------------------------------------------------
void __fastcall TLoadCCD::edtPortClick(TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_PORT);
}
//---------------------------------------------------------------------------