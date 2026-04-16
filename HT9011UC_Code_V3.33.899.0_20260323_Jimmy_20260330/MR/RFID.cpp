#include "MachineDefine.h"

#include <vcl.h>
#pragma hdrstop

#include "RFID.h"
#include "database.h"
#include "rs232.h"
#include "mymessbox.h"
#include "mymotor.h"
#include "mycylin.h"
#include "mysensor.h"
#include "myswitch.h"
#include "MyKitSuck.h"
#include "common.h"
#include "csystem.h"
//#include "cprod.h"
//#include "cmydef.h"
//#include "cSecurity.h"
//#include "cBuilder.h"
#include "DMT.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SPComm"
#pragma resource "*.dfm"
TfRFID *fRFID;
//---------------------------------------------------------------------------
__fastcall TfRFID::TfRFID(TComponent* Owner)
    : TForm(Owner)
{
    RFIDCOM[0]=RFID_1;
    RFIDCOM[1]=RFID_2;
}
//---------------------------------------------------------------------------
void __fastcall TfRFID::spbResetComClick(TObject *Sender)
{
    RFID_1->StopComm();
    RFID_2->StopComm();
}
//---------------------------------------------------------------------------
void __fastcall TfRFID::spbStartComClick(TObject *Sender)
{
    RFID_1->StopComm();
    RFID_2->StopComm();
    InitRFIDRS232();
}
//---------------------------------------------------------------------------
void __fastcall TfRFID::InitRFIDRS232()
{
    #ifndef SOFT_SIMULTE
    bool flag[RFID_COUNT];
    AnsiString str;
    for(int i=0; i<2; i++)
    {
        flag[i]=COM2->GetCOMPortStatus(HSys.asRFIDComPort[i]);
        if(flag[i]==false)
        {
            str.sprintf("RFID %d: %s port status error!", i+1, HSys.asRFIDComPort[i]);
            ShowMyMessage(str);
            continue;
        }

        RFIDCOM[i]->BaudRate = HSys.RFIDBaudRate;
        if(HSys.RFIDByteSize==5)
            RFIDCOM[i]->ByteSize = TByteSize(_5);
        else if(HSys.RFIDByteSize==6)
            RFIDCOM[i]->ByteSize = TByteSize(_6);
        else if(HSys.RFIDByteSize==7)
            RFIDCOM[i]->ByteSize = TByteSize(_7);
        else if(HSys.RFIDByteSize==8)
            RFIDCOM[i]->ByteSize = TByteSize(_8);

        if(HSys.RFIDStopBit==1)
            RFIDCOM[i]->StopBits = TStopBits(_1);
        else if(HSys.RFIDStopBit==1.5)
            RFIDCOM[i]->StopBits = TStopBits(_1_5);
        else if(HSys.RFIDStopBit==2)
            RFIDCOM[i]->StopBits = TStopBits(_2);

        if(HSys.RFIDParity=="None")
            RFIDCOM[i]->Parity   = None;
        else if(HSys.RFIDParity=="Odd")
            RFIDCOM[i]->Parity   = Odd;
        else if(HSys.RFIDParity=="Even")
            RFIDCOM[i]->Parity   = Even;
        else if(HSys.RFIDParity=="Space")
            RFIDCOM[i]->Parity   = Space;
        else if(HSys.RFIDParity=="Mark")
            RFIDCOM[i]->Parity   = TParity(3);

        RFIDCOM[i]->CommName="\\\\.\\"+HSys.asRFIDComPort[i];
        try
        {
            RFIDCOM[i]->StopComm();
            RFIDCOM[i]->StartComm();  //僅能啟動一次
        }
        catch(...)
        {
            str.sprintf("RFID %d: COM%d port start error!", i+1, HSys.asRFIDComPort[i]);
            ShowMyMessage(str);
        }
    }
    #endif
}
//---------------------------------------------------------------------------
void __fastcall TfRFID::FormClose(TObject *Sender, TCloseAction &Action)
{
    fShow=false;
}
//---------------------------------------------------------------------------
void __fastcall TfRFID::FormShow(TObject *Sender)
{
    DoIniDataToForm();
    fShow=true;
}
//---------------------------------------------------------------------------
void __fastcall TfRFID::btRFID1OnClick(TObject *Sender)
{
    TButton *Ptr;
    Ptr=(TButton *)Sender;

    DoRFIDReader(Ptr->Tag);
}
//---------------------------------------------------------------------------
void __fastcall TfRFID::RFID_1ReceiveData(TObject *Sender, Pointer Buffer,
      WORD BufferLength)
{
    char *data;
    data=(char*)Buffer;
    AnsiString cStr;
    char cStr1[1024]={0};

    strncpy(cStr1, data, BufferLength);
    cStr=cStr1;
    cStr=cStr.SubString(14, 8);
    if(cStr!="")
    {
        bRFIDReaderOK[0]=true;
        MOT[MMLoadPort].Tray.cCassetteID=cStr;
        asLoadPortCassetteID=cStr;
    }
    Memo1->Lines->Add(cStr);
}
//---------------------------------------------------------------------------
AnsiString TfRFID::DoRFIDAnalysis(AnsiString str)
{
    return str.SubString(14, 8);
}
//---------------------------------------------------------------------------
void TfRFID::DoRFIDReader(int Number)
{
    AnsiString CMD, Str;

    if(USE_RFID_SYSTEM)
        CMD="~01 RMID*";        //第一台
    else
        CMD="~00 RMID1*";       //第三台

    Str.sprintf("RFID_%d %s", Number+1, CMD);

    if(Number==0)
        RFID_1->WriteCommData(CMD.c_str(), CMD.Length());
    else if(Number==1)
        RFID_2->WriteCommData(CMD.c_str(), CMD.Length());

    Memo1->Lines->Add(Str);
}
//---------------------------------------------------------------------------
void __fastcall TfRFID::spbSaveClick(TObject *Sender)
{
    AnsiString S="";
    S=GetLastOpenFN();
    AnsiString szDir="";
    szDir.sprintf("%s%s\\HandlerCondition.Data", DataPath, S);

    if(cbEnableRFID->Checked==true && TestIF_File.bEnableRFID!=cbEnableRFID->Checked)
    {
        InitRFIDRS232();
    }
    WriteIniData(szDir, "Configuration", "RFID Enable",                         (cbEnableRFID->Checked)?1:0);
    WriteIniData(szDir, "Configuration", "RFID Delay Time",                     atoi(edRFIDScanDelayTime->Text.c_str()));
    WriteIniData(szDir, "Configuration", "RFID Auto Retry",                     atoi(edRFIDRetryCount->Text.c_str()));

    WriteIniData(szDir, "Configuration", "View Enable",                         (cbEnableView->Checked)?1:0);
    WriteIniData(szDir, "Configuration", "E84 Time Out 1",                      atoi(edTP1->Text.c_str()));
    WriteIniData(szDir, "Configuration", "E84 Time Out 2",                      atoi(edTP2->Text.c_str()));
    WriteIniData(szDir, "Configuration", "E84 Time Out 3",                      atoi(edTP3->Text.c_str()));
    WriteIniData(szDir, "Configuration", "E84 Time Out 4",                      atoi(edTP4->Text.c_str()));
    WriteIniData(szDir, "Configuration", "E84 Time Out 5",                      atoi(edTP5->Text.c_str()));
    WriteIniData(szDir, "Configuration", "E84 Time Out 6",                      atoi(edTP6->Text.c_str()));

    ReadFile();
    spbSave->Down=false;
}
//---------------------------------------------------------------------------
void __fastcall TfRFID::ReadFile()
{
    AnsiString S="";
    S=GetLastOpenFN();
    AnsiString szDir="";
    szDir.sprintf("%s%s\\HandlerCondition.Data", DataPath, S);

    TestIF_File.bEnableRFID          =ReadIniData(szDir, "Configuration", "RFID Enable"           , false);
    TestIF_File.iRFIDDelay           =ReadIniData(szDir, "Configuration", "RFID Delay Time", 5000);
    TestIF_File.iRFIDRetryCount      =ReadIniData(szDir, "Configuration", "RFID Auto Retry", 3);                     //Steven 20151221 : 讀不到Code時,自動Retry的次數
    TestIF_File.bEnableView          =ReadIniData(szDir, "Configuration", "View Enable"           , false);
    TestIF_File.iE84TimeOut[0]       =ReadIniData(szDir, "Configuration", "E84 Time Out 1", 2);
    TestIF_File.iE84TimeOut[1]       =ReadIniData(szDir, "Configuration", "E84 Time Out 2", 2);
    TestIF_File.iE84TimeOut[2]       =ReadIniData(szDir, "Configuration", "E84 Time Out 3", 60);
    TestIF_File.iE84TimeOut[3]       =ReadIniData(szDir, "Configuration", "E84 Time Out 4", 60);
    TestIF_File.iE84TimeOut[4]       =ReadIniData(szDir, "Configuration", "E84 Time Out 5", 2);
    TestIF_File.iE84TimeOut[5]       =ReadIniData(szDir, "Configuration", "E84 Time Out 6", 2);
}
//---------------------------------------------------------------------------
void __fastcall TfRFID::DoIniDataToForm()
{
    cbEnableRFID->Checked        =TestIF_File.bEnableRFID;
    cbEnableView->Checked        =TestIF_File.bEnableView;

    edRFIDScanDelayTime->Text    =AnsiString(TestIF_File.iRFIDDelay);
    edRFIDRetryCount->Text       =AnsiString(TestIF_File.iRFIDRetryCount);//Steven 20151221 : 讀不到Code時,自動Retry的次數

    coCommRFID1->Text=HSys.asRFIDComPort[0];
    coCommRFID2->Text=HSys.asRFIDComPort[1];

    edTP1->Text                 =AnsiString(TestIF_File.iE84TimeOut[0]);
    edTP2->Text                 =AnsiString(TestIF_File.iE84TimeOut[1]);
    edTP3->Text                 =AnsiString(TestIF_File.iE84TimeOut[2]);
    edTP4->Text                 =AnsiString(TestIF_File.iE84TimeOut[3]);
    edTP5->Text                 =AnsiString(TestIF_File.iE84TimeOut[4]);
    edTP6->Text                 =AnsiString(TestIF_File.iE84TimeOut[5]);
    tsE84->TabVisible           =false;
}
//---------------------------------------------------------------------------
void __fastcall TfRFID::sbtExitClick(TObject *Sender)
{
    sbtExit->Down=false;
    Close();
    fShow=false;
}
//---------------------------------------------------------------------------
void __fastcall TfRFID::RFID_2ReceiveData(TObject *Sender, Pointer Buffer,
      WORD BufferLength)
{
    char *data;
    data=(char*)Buffer;
    AnsiString cStr;
    char cStr1[1024]={0};

    strncpy(cStr1, data, BufferLength);
    cStr=cStr1;
    cStr=cStr.SubString(14, 8);
    if(cStr!="")
    {
        bRFIDReaderOK[1]=true;
        MOT[MMCABuffer6].Tray.cCassetteID=cStr;
        asBuffer6CassetteID=cStr;
    }
    Memo1->Lines->Add(cStr);
}
//---------------------------------------------------------------------------
int iE84LoaderTask=1;
void InitialE84LoaderTask()
{
    iE84LoaderTask=1;
}
//------------------------------------------------------------------------------
//extern const int SwE84LREQ                    ;
//extern const int SwE84UREQ                    ;
//extern const int SwE84VA                      ;
//extern const int SwE84READY                   ;
//extern const int SwE84VS0                     ;
//extern const int SwE84VS1                     ;
//extern const int SwE84HOAVBL                  ;
//extern const int SwE84ES                      ;
//extern const int SwE84POWER                   ;

//extern const int SnE84VALID              ;
//extern const int SnE84CS0                ;
//extern const int SnE84CS1                ;
//extern const int SnE84AMAVBL             ;
//extern const int SnE84TRREQ              ;
//extern const int SnE84BUSY               ;
//extern const int SnE84COMPT              ;
//extern const int SnE84CONT               ;
//extern const int SnE84GO                 ;
HTimer E84LoadDelay;
void __fastcall TfRFID::DoE84Load()
{
    int &Task=iE84LoaderTask;

    if(CheckSafeDoor()==false || IsEMGPressed())
        SW[SwE84ES].Off();
    else
        SW[SwE84ES].On();

    switch(Task)
    {
        case 1:
            if(Sen[SnE84GO].IsOn())
            {
                Task=100;
            }
            break;
        case 100:
            if(Sen[SnE84CS0].IsOn())
            {
                E84LoadDelay.SetSecAndOn(0.1);
                Task=200;
            }
            break;
        case 200:
            if(Sen[SnE84VALID].IsOn())
            {
                E84LoadDelay.SetSecAndOn(2.0);
                Task=300;
            }
            break;
        case 300:
            if(Sen[SnLoadPortPresent].IsOn()==false && Sen[SnLoadPortPlacement1].IsOn()==false)
            {
                SW[SwE84LREQ].On();

                E84LoadDelay.SetSecAndOn(TestIF_File.iE84TimeOut[0]);
                Task=400;
            }
            break;
        case 400:
            if(Sen[SnE84TRREQ].IsOn())
            {
                E84LoadDelay.SetSecAndOn(2.0);
                Task=500;
            }
            break;
        case 500:
            if(Sen[SnLoadPortPresent].IsOn()==false && Sen[SnLoadPortPlacement1].IsOn()==false)
            {
                SW[SwE84READY].On();
                E84LoadDelay.SetSecAndOn(TestIF_File.iE84TimeOut[1]);
                Task=600;
            }
            break;
        case 600:
            if(Sen[SnE84BUSY].IsOn())
            {
                E84LoadDelay.SetSecAndOn(TestIF_File.iE84TimeOut[2]);
                Task=700;
            }
            break;
        case 700:
            if(Sen[SnLoadPortPresent].IsOn() && Sen[SnLoadPortPlacement1].IsOn())
            {
                SW[SwE84LREQ].Off();
                E84LoadDelay.SetSecAndOn(TestIF_File.iE84TimeOut[3]);
                Task=800;
            }
            break;
        case 800:
            if(Sen[SnE84BUSY].IsOff() && Sen[SnE84TRREQ].IsOff() && Sen[SnE84COMPT].IsOn())
            {
                E84LoadDelay.SetSecAndOn(2.0);
                Task=1100;
            }
            break;
        case 1100:
            if(Sen[SnLoadPortPresent].IsOn() && Sen[SnLoadPortPlacement1].IsOn())
            {
                SW[SwE84READY].Off();
                E84LoadDelay.SetSecAndOn(TestIF_File.iE84TimeOut[4]);
                Task=1200;
            }
            break;
        case 1200:
            if(Sen[SnE84VALID].IsOff() && Sen[SnE84COMPT].IsOff() && Sen[SnE84CS0].IsOff())
            {
                Task=1500;
            }
            break;
        case 1500:
            if(Sen[SnE84GO].IsOff())
            {
                Task=1;
            }
            break;
    }
}
//---------------------------------------------------------------------------
HTimer E84UnLoadDelay;
void __fastcall TfRFID::DoE84UnLoad()
{
    int &Task=iE84LoaderTask;

    if(CheckSafeDoor()==false || IsEMGPressed())
        SW[SwE84ES].Off();
    else
        SW[SwE84ES].On();

    switch(Task)
    {
        case 1:
            if(Sen[SnE84GO].IsOn())
            {
                Task=100;
            }
            break;
        case 100:
            if(Sen[SnE84CS0].IsOn())
            {
                E84UnLoadDelay.SetSecAndOn(0.1);
                Task=200;
            }
            break;
        case 200:
            if(Sen[SnE84VALID].IsOn())
            {
                E84UnLoadDelay.SetSecAndOn(2.0);
                Task=300;
            }
            break;
        case 300:
            if(Sen[SnLoadPortPresent].IsOn() && Sen[SnLoadPortPlacement1].IsOn())
            {
                SW[SwE84UREQ].On();

                E84UnLoadDelay.SetSecAndOn(TestIF_File.iE84TimeOut[0]);
                Task=400;
            }
            break;
        case 400:
            if(Sen[SnE84TRREQ].IsOn())
            {
                E84UnLoadDelay.SetSecAndOn(2.0);
                Task=500;
            }
            break;
        case 500:
            if(Sen[SnLoadPortPresent].IsOn() && Sen[SnLoadPortPlacement1].IsOn())
            {
                SW[SwE84READY].On();
                E84UnLoadDelay.SetSecAndOn(TestIF_File.iE84TimeOut[1]);
                Task=600;
            }
            break;
        case 600:
            if(Sen[SnE84BUSY].IsOn())
            {
                E84UnLoadDelay.SetSecAndOn(TestIF_File.iE84TimeOut[2]);
                Task=700;
            }
            break;
        case 700:
            if(Sen[SnLoadPortPresent].IsOn()==false && Sen[SnLoadPortPlacement1].IsOn()==false)
            {
                SW[SwE84UREQ].Off();
                E84UnLoadDelay.SetSecAndOn(TestIF_File.iE84TimeOut[3]);
                Task=800;
            }
            break;
        case 800:
            if(Sen[SnE84BUSY].IsOff() && Sen[SnE84TRREQ].IsOff() && Sen[SnE84COMPT].IsOn())
            {
                E84UnLoadDelay.SetSecAndOn(2.0);
                Task=1100;
            }
            break;
        case 1100:
            if(Sen[SnLoadPortPresent].IsOn()==false && Sen[SnLoadPortPlacement1].IsOn()==false)
            {
                SW[SwE84READY].Off();
                E84UnLoadDelay.SetSecAndOn(TestIF_File.iE84TimeOut[4]);
                Task=1200;
            }
            break;
        case 1200:
            if(Sen[SnE84VALID].IsOff() && Sen[SnE84COMPT].IsOff() && Sen[SnE84CS0].IsOff())
            {
                Task=1500;
            }
            break;
        case 1500:
            if(Sen[SnE84GO].IsOff())
            {
                Task=1;
            }
            break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfRFID::DoView(int iFunction, int channel)
{
    AnsiString strReq = *NullStr;
    AnsiString strRes = *NullStr;

    ActStatus->Text = "";

    void (WINAPI *ptrClose)(int); // function pointer for disconnection

    int conn_num = 0;
    int ip = inet_addr((Edit1->Text).c_str());

    int comm_type = 1; // 0:RS-232 , 1:Ethernet
    AnsiString Path, Name, Log;
    ptrClose = CloseSocket;
    int srt=0;

    unsigned char sendbuf[512] = {0};
    unsigned char recvbuf[512] = {0};

    if(bViewCom==false)
    {
        srt= OpenModbusTCPSocket(conn_num, ip);

        if(srt == -1)
        {
            ActStatus->Text = ("Connection Failed");
            bViewCom=false;
            return;
        }
        else
        {
            bViewCom=true;
        }
    }
    AnsiString dataSplit, dataSplit1, dataSplit2;

    TEdit *dataSplitList[30]=
    {Edit3,Edit4,Edit5,
     Edit6,Edit7,Edit8,
     Edit9,Edit10, Edit11,
     Edit12,Edit13,Edit14,
     Edit15,Edit16,Edit17,
     Edit18,Edit19,Edit20,
     Edit21,Edit22,Edit23,
     Edit24,Edit25,Edit26,
     Edit27,Edit28,Edit29,
     Edit30,Edit31,Edit32};

     TEdit *dataSplitList1[3]=
    {Edit34,Edit35,Edit36};


    static int qw=0;
    if(fRFID->fShow)
    {
        for(int l=0; l<30; l++)
        {
            dataSplitList[l]->Text=qw;
        }
        qw++;

        if(qw==9999999999)
            qw=0;
    }
    int l=channel;
    strReq="";

    if(iFunction==0)
    {
        if(l==0)     //1
            strReq="011007D0000A14";
        else if(l==1)
            strReq="011007DB000A14";
        else if(l==2)
            strReq="011007E5000A14";
        else if(l==3)  //2
            strReq="011007EF000A14";
        else if(l==4)
            strReq="011007F9000A14";
        else if(l==5)
            strReq="01100803000A14";
        else if(l==6)   //3
            strReq="0110080D000A14";
        else if(l==7)
            strReq="01100817000A14";
        else if(l==8)
            strReq="01100821000A14";
        else if(l==9)   //4
            strReq="0110082B000A14";
        else if(l==10)
            strReq="01100835000A14";
        else if(l==11)
            strReq="0110083F000A14";
        else if(l==12)  //5
            strReq="01100849000A14";
        else if(l==13)
            strReq="01100853000A14";
        else if(l==14)
            strReq="0110085D000A14";
        else if(l==15) //6
            strReq="01100867000A14";
        else if(l==16)
            strReq="01100871000A14";
        else if(l==17)
            strReq="0110087B000A14";
        else if(l==18)  //7
            strReq="01100885000A14";
        else if(l==19)
            strReq="0110088F000A14";
        else if(l==20)
            strReq="01100899000A14";
        else if(l==21)  //8
            strReq="011008A3000A14";
        else if(l==22)
            strReq="011008AD000A14";
        else if(l==23)
            strReq="011008B7000A14";
        else if(l==24)   //9
            strReq="011008C1000A14";
        else if(l==25)
            strReq="011008CB000A14";
        else if(l==26)
            strReq="011008D5000A14";
        else if(l==27)  //10
            strReq="011008DF000A14";
        else if(l==28)
            strReq="011008E9000A14";
        else if(l==29)
            strReq="011008F3000A14";

        if(fRFID->fShow)
        {
            dataSplit1=dataSplitList[l]->Text.c_str();
        }
        else
        {
            dataSplit1=asViewList[l];
        }

        for(int i=0; i<10; i++)
        {
            strReq=strReq+"00";
            dataSplit=dataSplit1.SubString(0,1);
            if(dataSplit=="")
                strReq=strReq+"00";
            else
                strReq=strReq+IntToHex(MyASCIIToDec(dataSplit),2);
            dataSplit1.Delete(1,1);
        }
    }
    else if(iFunction==1)
    {
        if(l==0)     //1
            strReq="010307D0000A";
        else if(l==1)
            strReq="010307DB000A";
        else if(l==2)
            strReq="010307E5000A";
        else if(l==3)  //2
            strReq="010307EF000A";
        else if(l==4)
            strReq="010307F9000A";
        else if(l==5)
            strReq="01030803000A";
        else if(l==6)   //3
            strReq="0103080D000A";
        else if(l==7)
            strReq="01030817000A";
        else if(l==8)
            strReq="01030821000A";
        else if(l==9)   //4
            strReq="0103082B000A";
        else if(l==10)
            strReq="01030835000A";
        else if(l==11)
            strReq="0103083F000A";
        else if(l==12)  //5
            strReq="01030849000A";
        else if(l==13)
            strReq="01030853000A";
        else if(l==14)
            strReq="0103085D000A";
        else if(l==15) //6
            strReq="01030867000A";
        else if(l==16)
            strReq="01030871000A";
        else if(l==17)
            strReq="0103087B000A";
        else if(l==18)  //7
            strReq="01030885000A";
        else if(l==19)
            strReq="0103088F000A";
        else if(l==20)
            strReq="01030899000A";
        else if(l==21)  //8
            strReq="010308A3000A";
        else if(l==22)
            strReq="010308AD000A";
        else if(l==23)
            strReq="010308B7000A";
        else if(l==24)   //9
            strReq="010308C1000A";
        else if(l==25)
            strReq="010308CB000A";
        else if(l==26)
            strReq="010308D5000A";
        else if(l==27)  //10
            strReq="010308DF000A";
        else if(l==28)
            strReq="010308E9000A";
        else if(l==29)
            strReq="010308F3000A";
    }
    else if(iFunction==2)
    {
        if(l==0)  //10
            strReq="011008FD000A14"; //strReq="011008FD001428";
        else if(l==1)
            strReq="01100907000A14";
        else if(l==2)
            strReq="01100911000F1E";   //strReq="01100911000A14";

        if(fRFID->fShow)
            dataSplit1=dataSplitList1[l]->Text.c_str();
        else
            dataSplit1=asViewMessage[l];

        int len=0;
        if(l==2)
            len=15;
        else
            len=10;
        for(int i=0; i<len; i++)
        {
            dataSplit=dataSplit1.SubString(0,1);
            dataSplit1.Delete(1,1);
            dataSplit2=dataSplit1.SubString(0,1);
            dataSplit1.Delete(1,1);

            if(dataSplit2=="")
                strReq=strReq+"00";
            else
                strReq=strReq+IntToHex(MyASCIIToDec(dataSplit2),2);

            if(dataSplit=="")
                strReq=strReq+"00";
            else
                strReq=strReq+IntToHex(MyASCIIToDec(dataSplit),2);
        }
    }

    int modbus_addr = 0;
    int modbus_func = 0;
    int modbus_addr_ret = 0;
    int modbus_func_ret = 0;
    int sendlen = 0;
    int i = 0;

    AnsiString strValid = "0123456789ABCDEF";

// Example : input verification
//   AnsiStinrg str1 = "0123456789ABCDEF";
//   AnsiString str2 = "AX";
//   int num = str1.Pos(str2[1]); // num = 11
//   num = str1.Pos(str2[2]); // num = 0 : illegal char

// Example : hex string to int
//   AnsiString str = "FF";
//   str.Insert("0x",1); // str = "0xFF"
//   int num = StrToInt(str); // num = 255

    if(strReq.Length() < 4) // at least slave address and function code
    {
        ActStatus->Text = ("Invalid Modbus Data");
        bViewCom=false;
        ptrClose(conn_num);
        return;
    }

    if(strReq.Length()%2 != 0) // input data must be even number
    {
        ActStatus->Text = ("Modbus Data Must Be Even Number");
        bViewCom=false;
        ptrClose(conn_num);
        return;
    }

    for(i = 1 ; i <= strReq.Length() ; ++i) // index of AnsiString starts from 1
    {
        if(strValid.Pos(strReq[i]) == 0) // input data verification
        {
            ActStatus->Text = ("Invalid Modbus Data");
            bViewCom=false;
            ptrClose(conn_num);
            return;
        }
    }

    for(i = 0 ; i <= strReq.Length()-2 ; i+=2) // trans data into bytes and put it into sendbuf
    {
        char* tmp = strReq.c_str();
        AnsiString strTmp = "0x";
        strTmp.Insert(*(tmp+i),strTmp.Length()+1);
        strTmp.Insert(*(tmp+i+1),strTmp.Length()+1);
        if(i == 0)
          modbus_addr = StrToInt(strTmp);
        else if(i == 2)
          modbus_func = StrToInt(strTmp);
        else
          sendbuf[sendlen++] = StrToInt(strTmp);
    }
    Log.sprintf("%04d-%02d-%02d, %02d:%02d:%02d:%03d, %d, %s", SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, SystemMSec, l, strReq.c_str());
    Path.sprintf("%s\\%04d_%02d_%02d", asViewCommLogPath, SystemYear, SystemMonth, SystemDate);
    if(!DirectoryExists(Path))
        ForceDirectories(Path);

    Path.sprintf("%s\\%04d_%02d_%02d\\%04d_%02d_%02d_%02d.txt", asViewCommLogPath, SystemYear, SystemMonth, SystemDate, SystemYear, SystemMonth, SystemDate , SystemHour);

    WriteDataToFile(Path.c_str() , Log.c_str());
    int req = RequestData(comm_type, conn_num, modbus_addr, modbus_func, sendbuf, sendlen); // modbus request
    if(req == -1)
    {
        ActStatus->Text = ("Request Failed");
        bViewCom=false;
        ptrClose(conn_num);
        return;
    }

    int res = ResponseData(comm_type, conn_num, &modbus_addr_ret, &modbus_func_ret, recvbuf); // modbus response

    if(res > 0)
    {
        AnsiString strTmp;
        strTmp.sprintf("%02X",modbus_addr_ret);
        strRes += strTmp;
        strTmp.sprintf("%02X",modbus_func_ret);
        strRes += strTmp;

        // Example : int to hex string
        //   AnsiString str;
        //   int num = 15;
        //   str = IntToHex(num,2); //turns 15 into "0F"

        switch (modbus_func_ret)
        {
            case 0x01:  case 0x02:  case 0x03:
            case 0x04:  case 0x11:  case 0x17:
            strTmp.sprintf("%02X",res);
            strRes += strTmp;
            break;
        }

        if(iFunction==0 || iFunction==2)
        {
            for(i = 0 ; i < res ; ++i) // recover a string from recvbuf
            {
                strRes.Insert(IntToHex(recvbuf[i],2),strRes.Length()+1);
            }
        }
        else if(iFunction==1)
        {
            dataSplit1="";
            for(i = 0 ; i < res ; ++i) // recover a string from recvbuf
            {
                strRes.Insert(IntToHex(recvbuf[i],2),strRes.Length()+1);
                if(i%2!=0)
                {
                    if(MyDeCodeASCII(recvbuf[i])=="NUL")
                        dataSplit1+=" ";
                    else
                        dataSplit1+=MyDeCodeASCII(recvbuf[i]);
                }
            }
            dataSplitList[l]->Text=dataSplit1.c_str();
        }


        ResData->Text = strRes;
        ActStatus->Text = ("Request Done");
        MySleep(10);
    }
    else
    {
        ResData->Text = *NullStr;
        ActStatus->Text = ("No Data Received");
    }
    delete[] sendbuf;
    delete[] recvbuf;
}
//---------------------------------------------------------------------------
void __fastcall TfRFID::Button1Click(TObject *Sender)
{
    AnsiString strReq = *NullStr;
    AnsiString strRes = *NullStr;

    ActStatus->Text = "";

    void (WINAPI *ptrClose)(int); // function pointer for disconnection

    int conn_num = 0;
    int ip = inet_addr((Edit1->Text).c_str());
//    int baud_rate = 9600;
//    int data_len = 8;
//    char parity = 'N';
//    int stop = 1;
//    int modbus_mode = 2; // 1:ASCII , 2:RTU
    int comm_type = 1; // 0:RS-232 , 1:Ethernet

    ptrClose = CloseSocket;

    if(OpenModbusTCPSocket(conn_num, ip) == -1)
    {
        ActStatus->Text = ("Connection Failed");
        return;
    }

    AnsiString dataSplit, dataSplit1;
    AnsiString dataSplitList[30]=
    {Edit3->Text,Edit4->Text,Edit5->Text,
     Edit6->Text,Edit7->Text,Edit8->Text,
     Edit9->Text,Edit10->Text, Edit11->Text,
     Edit12->Text,Edit13->Text,Edit14->Text,
     Edit15->Text,Edit16->Text,Edit17->Text,
     Edit18->Text,Edit19->Text,Edit20->Text,
     Edit21->Text,Edit22->Text,Edit23->Text,
     Edit24->Text,Edit25->Text,Edit26->Text,
     Edit27->Text,Edit28->Text,Edit29->Text,
     Edit30->Text,Edit31->Text,Edit32->Text};

    for(int l=0; l<30; l++)
    {
        if(l==0)     //1
            strReq="011007D0000A14";
        else if(l==1)
            strReq="011007DB000A14";
        else if(l==2)
            strReq="011007E5000A14";
        else if(l==3)  //2
            strReq="011007EF000A14";
        else if(l==4)
            strReq="011007F9000A14";
        else if(l==5)
            strReq="01100803000A14";
        else if(l==6)   //3
            strReq="0110080D000A14";
        else if(l==7)
            strReq="01100817000A14";
        else if(l==8)
            strReq="01100821000A14";
        else if(l==9)   //4
            strReq="0110082B000A14";
        else if(l==10)
            strReq="01100835000A14";
        else if(l==11)
            strReq="0110083F000A14";
        else if(l==12)  //5
            strReq="01100849000A14";
        else if(l==13)
            strReq="01100853000A14";
        else if(l==14)
            strReq="0110085D000A14";
        else if(l==15) //6
            strReq="01100867000A14";
        else if(l==16)
            strReq="01100871000A14";
        else if(l==17)
            strReq="0110087B000A14";
        else if(l==18)  //7
            strReq="01100885000A14";
        else if(l==19)
            strReq="0110088F000A14";
        else if(l==20)
            strReq="01100899000A14";
        else if(l==21)  //8
            strReq="011008A3000A14";
        else if(l==22)
            strReq="011008AD000A14";
        else if(l==23)
            strReq="011008B7000A14";
        else if(l==24)   //9
            strReq="011008C1000A14";
        else if(l==25)
            strReq="011008CB000A14";
        else if(l==26)
            strReq="011008D5000A14";
        else if(l==27)  //10
            strReq="011008DF000A14";
        else if(l==28)
            strReq="011008E9000A14";
        else if(l==29)
            strReq="011008F3000A14";

        if(fRFID->fShow)
            dataSplit1=dataSplitList[l];

        for(int i=0; i<10; i++)
        {
            strReq=strReq+"00";
            dataSplit=dataSplit1.SubString(0,1);
            if(dataSplit=="")
                strReq=strReq+"00";
            else
                strReq=strReq+IntToHex(MyASCIIToDec(dataSplit),2);
            dataSplit1.Delete(1,1);
        }

        unsigned char sendbuf[1024] = {0};
        unsigned char recvbuf[1024] = {0};
        int modbus_addr = 0;
        int modbus_func = 0;
        int modbus_addr_ret = 0;
        int modbus_func_ret = 0;
        int sendlen = 0;
        int i = 0;

        AnsiString strValid = "0123456789ABCDEF";

    // Example : input verification
    //   AnsiStinrg str1 = "0123456789ABCDEF";
    //   AnsiString str2 = "AX";
    //   int num = str1.Pos(str2[1]); // num = 11
    //   num = str1.Pos(str2[2]); // num = 0 : illegal char

    // Example : hex string to int
    //   AnsiString str = "FF";
    //   str.Insert("0x",1); // str = "0xFF"
    //   int num = StrToInt(str); // num = 255

        if(strReq.Length() < 4) // at least slave address and function code
        {
            ActStatus->Text = ("Invalid Modbus Data");
            ptrClose(conn_num);
            return;
        }

        if(strReq.Length()%2 != 0) // input data must be even number
        {
            ActStatus->Text = ("Modbus Data Must Be Even Number");
            ptrClose(conn_num);
            return;
        }

        for(i = 1 ; i <= strReq.Length() ; ++i) // index of AnsiString starts from 1
        {
            if(strValid.Pos(strReq[i]) == 0) // input data verification
            {
                ActStatus->Text = ("Invalid Modbus Data");
                ptrClose(conn_num);
                return;
            }
        }

        for(i = 0 ; i <= strReq.Length()-2 ; i+=2) // trans data into bytes and put it into sendbuf
        {
            char* tmp = strReq.c_str();
            AnsiString strTmp = "0x";
            strTmp.Insert(*(tmp+i),strTmp.Length()+1);
            strTmp.Insert(*(tmp+i+1),strTmp.Length()+1);
            if(i == 0)
              modbus_addr = StrToInt(strTmp);
            else if(i == 2)
              modbus_func = StrToInt(strTmp);
            else
              sendbuf[sendlen++] = StrToInt(strTmp);
        }

        int req = RequestData(comm_type, conn_num, modbus_addr, modbus_func, sendbuf, sendlen); // modbus request
        if(req == -1)
        {
            ActStatus->Text = ("Request Failed");
            ptrClose(conn_num);
            return;
        }

        int res = ResponseData(comm_type, conn_num, &modbus_addr_ret, &modbus_func_ret, recvbuf); // modbus response

        if(res > 0)
        {
            AnsiString strTmp;
            strTmp.sprintf("%02X",modbus_addr_ret);
            strRes += strTmp;
            strTmp.sprintf("%02X",modbus_func_ret);
            strRes += strTmp;

            // Example : int to hex string
            //   AnsiString str;
            //   int num = 15;
            //   str = IntToHex(num,2); //turns 15 into "0F"

            switch (modbus_func_ret)
            {
                case 0x01:  case 0x02:  case 0x03:
                case 0x04:  case 0x11:  case 0x17:
                strTmp.sprintf("%02X",res);
                strRes += strTmp;
                break;
            }

            for(i = 0 ; i < res ; ++i) // recover a string from recvbuf
            {
                strRes.Insert(IntToHex(recvbuf[i],2),strRes.Length()+1);
            }
            ResData->Text = strRes;
            ActStatus->Text = ("Request Done");
        }
        else
        {
            ResData->Text = *NullStr;
            ActStatus->Text = ("No Data Received");
        }
    }

    ptrClose(conn_num);
}
//---------------------------------------------------------------------------
void __fastcall TfRFID::Button2Click(TObject *Sender)
{
    CloseSocket(0);
}
//---------------------------------------------------------------------------
void __fastcall TfRFID::Button3Click(TObject *Sender)
{
    AnsiString strReq = *NullStr;
    AnsiString strRes = *NullStr;
    AnsiString strResNew = *NullStr;

    ActStatus->Text = "";

    void (WINAPI *ptrClose)(int); // function pointer for disconnection

    int conn_num = 0;
    int ip = inet_addr((Edit1->Text).c_str());
    int comm_type = 1; // 0:RS-232 , 1:Ethernet

    ptrClose = CloseSocket;

    if(OpenModbusTCPSocket(conn_num, ip) == -1)
    {
        ActStatus->Text = ("Connection Failed");
        return;
    }

    AnsiString dataSplit, dataSplit1;
    TEdit *dataSplitList[30]=
    {Edit3,Edit4,Edit5,
     Edit6,Edit7,Edit8,
     Edit9,Edit10, Edit11,
     Edit12,Edit13,Edit14,
     Edit15,Edit16,Edit17,
     Edit18,Edit19,Edit20,
     Edit21,Edit22,Edit23,
     Edit24,Edit25,Edit26,
     Edit27,Edit28,Edit29,
     Edit30,Edit31,Edit32};

    for(int l=0; l<30; l++)
    {
        if(l==0)     //1
            strReq="010307D0000A";
        else if(l==1)
            strReq="010307DB000A";
        else if(l==2)
            strReq="010307E5000A";
        else if(l==3)  //2
            strReq="010307EF000A";
        else if(l==4)
            strReq="010307F9000A";
        else if(l==5)
            strReq="01030803000A";
        else if(l==6)   //3
            strReq="0103080D000A";
        else if(l==7)
            strReq="01030817000A";
        else if(l==8)
            strReq="01030821000A";
        else if(l==9)   //4
            strReq="0103082B000A";
        else if(l==10)
            strReq="01030835000A";
        else if(l==11)
            strReq="0103083F000A";
        else if(l==12)  //5
            strReq="01030849000A";
        else if(l==13)
            strReq="01030853000A";
        else if(l==14)
            strReq="0103085D000A";
        else if(l==15) //6
            strReq="01030867000A";
        else if(l==16)
            strReq="01030871000A";
        else if(l==17)
            strReq="0103087B000A";
        else if(l==18)  //7
            strReq="01030885000A";
        else if(l==19)
            strReq="0103088F000A";
        else if(l==20)
            strReq="01030899000A";
        else if(l==21)  //8
            strReq="010308A3000A";
        else if(l==22)
            strReq="010308AD000A";
        else if(l==23)
            strReq="010308B7000A";
        else if(l==24)   //9
            strReq="010308C1000A";
        else if(l==25)
            strReq="010308CB000A";
        else if(l==26)
            strReq="010308D5000A";
        else if(l==27)  //10
            strReq="010308DF000A";
        else if(l==28)
            strReq="010308E9000A";
        else if(l==29)
            strReq="010308F3000A";

        unsigned char sendbuf[1024] = {0};
        unsigned char recvbuf[1024] = {0};
        int modbus_addr = 0;
        int modbus_func = 0;
        int modbus_addr_ret = 0;
        int modbus_func_ret = 0;
        int sendlen = 0;
        int i = 0;

        AnsiString strValid = "0123456789ABCDEF";

    // Example : input verification
    //   AnsiStinrg str1 = "0123456789ABCDEF";
    //   AnsiString str2 = "AX";
    //   int num = str1.Pos(str2[1]); // num = 11
    //   num = str1.Pos(str2[2]); // num = 0 : illegal char

    // Example : hex string to int
    //   AnsiString str = "FF";
    //   str.Insert("0x",1); // str = "0xFF"
    //   int num = StrToInt(str); // num = 255

        if(strReq.Length() < 4) // at least slave address and function code
        {
            ActStatus->Text = ("Invalid Modbus Data");
            ptrClose(conn_num);
            return;
        }

        if(strReq.Length()%2 != 0) // input data must be even number
        {
            ActStatus->Text = ("Modbus Data Must Be Even Number");
            ptrClose(conn_num);
            return;
        }

        for(i = 1 ; i <= strReq.Length() ; ++i) // index of AnsiString starts from 1
        {
            if(strValid.Pos(strReq[i]) == 0) // input data verification
            {
                ActStatus->Text = ("Invalid Modbus Data");
                ptrClose(conn_num);
                return;
            }
        }

        for(i = 0 ; i <= strReq.Length()-2 ; i+=2) // trans data into bytes and put it into sendbuf
        {
            char* tmp = strReq.c_str();
            AnsiString strTmp = "0x";
            strTmp.Insert(*(tmp+i),strTmp.Length()+1);
            strTmp.Insert(*(tmp+i+1),strTmp.Length()+1);
            if(i == 0)
              modbus_addr = StrToInt(strTmp);
            else if(i == 2)
              modbus_func = StrToInt(strTmp);
            else
              sendbuf[sendlen++] = StrToInt(strTmp);
        }

        int req = RequestData(comm_type, conn_num, modbus_addr, modbus_func, sendbuf, sendlen); // modbus request
        if(req == -1)
        {
            ActStatus->Text = ("Request Failed");
            ptrClose(conn_num);
            return;
        }

        int res = ResponseData(comm_type, conn_num, &modbus_addr_ret, &modbus_func_ret, recvbuf); // modbus response

        if(res > 0)
        {
            AnsiString strTmp;
            strTmp.sprintf("%02X",modbus_addr_ret);
            strRes += strTmp;
            strTmp.sprintf("%02X",modbus_func_ret);
            strRes += strTmp;

            // Example : int to hex string
            //   AnsiString str;
            //   int num = 15;
            //   str = IntToHex(num,2); //turns 15 into "0F"

            switch (modbus_func_ret)
            {
                case 0x01:  case 0x02:  case 0x03:
                case 0x04:  case 0x11:  case 0x17:
                strTmp.sprintf("%02X",res);
                strRes += strTmp;
                break;
            }

            dataSplit1="";
            for(i = 0 ; i < res ; ++i) // recover a string from recvbuf
            {
                strRes.Insert(IntToHex(recvbuf[i],2),strRes.Length()+1);
                if(i%2!=0)
                {
                    if(MyDeCodeASCII(recvbuf[i])=="NUL")
                        dataSplit1+=" ";
                    else
                        dataSplit1+=MyDeCodeASCII(recvbuf[i]);
                }
            }
            dataSplitList[l]->Text=dataSplit1.c_str();
            ResData->Text = strRes;
            ActStatus->Text = ("Request Done");
        }
        else
        {
            ResData->Text = *NullStr;
            ActStatus->Text = ("No Data Received");
        }
    }

    ptrClose(conn_num);
}
//---------------------------------------------------------------------------
void __fastcall TfRFID::Button4Click(TObject *Sender)
{
    AnsiString strReq = *NullStr;
    AnsiString strRes = *NullStr;

    ActStatus->Text = "";

    void (WINAPI *ptrClose)(int); // function pointer for disconnection

    int conn_num = 0;
    int ip = inet_addr((Edit1->Text).c_str());
    int comm_type = 1; // 0:RS-232 , 1:Ethernet

    ptrClose = CloseSocket;

    if(OpenModbusTCPSocket(conn_num, ip) == -1)
    {
        ActStatus->Text = ("Connection Failed");
        return;
    }

    AnsiString dataSplit, dataSplit1;

    for(int l=0; l<30; l++)
    {
        if(l==0)     //1
            strReq="011007D0000A14";
        else if(l==1)
            strReq="011007DB000A14";
        else if(l==2)
            strReq="011007E5000A14";
        else if(l==3)  //2
            strReq="011007EF000A14";
        else if(l==4)
            strReq="011007F9000A14";
        else if(l==5)
            strReq="01100803000A14";
        else if(l==6)   //3
            strReq="0110080D000A14";
        else if(l==7)
            strReq="01100817000A14";
        else if(l==8)
            strReq="01100821000A14";
        else if(l==9)   //4
            strReq="0110082B000A14";
        else if(l==10)
            strReq="01100835000A14";
        else if(l==11)
            strReq="0110083F000A14";
        else if(l==12)  //5
            strReq="01100849000A14";
        else if(l==13)
            strReq="01100853000A14";
        else if(l==14)
            strReq="0110085D000A14";
        else if(l==15) //6
            strReq="01100867000A14";
        else if(l==16)
            strReq="01100871000A14";
        else if(l==17)
            strReq="0110087B000A14";
        else if(l==18)  //7
            strReq="01100885000A14";
        else if(l==19)
            strReq="0110088F000A14";
        else if(l==20)
            strReq="01100899000A14";
        else if(l==21)  //8
            strReq="011008A3000A14";
        else if(l==22)
            strReq="011008AD000A14";
        else if(l==23)
            strReq="011008B7000A14";
        else if(l==24)   //9
            strReq="011008C1000A14";
        else if(l==25)
            strReq="011008CB000A14";
        else if(l==26)
            strReq="011008D5000A14";
        else if(l==27)  //10
            strReq="011008DF000A14";
        else if(l==28)
            strReq="011008E9000A14";
        else if(l==29)
            strReq="011008F3000A14";

        dataSplit1=" ";

        for(int i=0; i<10; i++)
        {
            strReq=strReq+"00";
            dataSplit=dataSplit1.SubString(0,1);
            if(dataSplit=="")
                strReq=strReq+"00";
            else
                strReq=strReq+IntToHex(MyASCIIToDec(dataSplit),2);
            dataSplit1.Delete(1,1);
        }

        unsigned char sendbuf[1024] = {0};
        unsigned char recvbuf[1024] = {0};
        int modbus_addr = 0;
        int modbus_func = 0;
        int modbus_addr_ret = 0;
        int modbus_func_ret = 0;
        int sendlen = 0;
        int i = 0;

        AnsiString strValid = "0123456789ABCDEF";

    // Example : input verification
    //   AnsiStinrg str1 = "0123456789ABCDEF";
    //   AnsiString str2 = "AX";
    //   int num = str1.Pos(str2[1]); // num = 11
    //   num = str1.Pos(str2[2]); // num = 0 : illegal char

    // Example : hex string to int
    //   AnsiString str = "FF";
    //   str.Insert("0x",1); // str = "0xFF"
    //   int num = StrToInt(str); // num = 255

        if(strReq.Length() < 4) // at least slave address and function code
        {
            ActStatus->Text = ("Invalid Modbus Data");
            ptrClose(conn_num);
            return;
        }

        if(strReq.Length()%2 != 0) // input data must be even number
        {
            ActStatus->Text = ("Modbus Data Must Be Even Number");
            ptrClose(conn_num);
            return;
        }

        for(i = 1 ; i <= strReq.Length() ; ++i) // index of AnsiString starts from 1
        {
            if(strValid.Pos(strReq[i]) == 0) // input data verification
            {
                ActStatus->Text = ("Invalid Modbus Data");
                ptrClose(conn_num);
                return;
            }
        }

        for(i = 0 ; i <= strReq.Length()-2 ; i+=2) // trans data into bytes and put it into sendbuf
        {
            char* tmp = strReq.c_str();
            AnsiString strTmp = "0x";
            strTmp.Insert(*(tmp+i),strTmp.Length()+1);
            strTmp.Insert(*(tmp+i+1),strTmp.Length()+1);
            if(i == 0)
              modbus_addr = StrToInt(strTmp);
            else if(i == 2)
              modbus_func = StrToInt(strTmp);
            else
              sendbuf[sendlen++] = StrToInt(strTmp);
        }

        int req = RequestData(comm_type, conn_num, modbus_addr, modbus_func, sendbuf, sendlen); // modbus request
        if(req == -1)
        {
            ActStatus->Text = ("Request Failed");
            ptrClose(conn_num);
            return;
        }

        int res = ResponseData(comm_type, conn_num, &modbus_addr_ret, &modbus_func_ret, recvbuf); // modbus response

        if(res > 0)
        {
            AnsiString strTmp;
            strTmp.sprintf("%02X",modbus_addr_ret);
            strRes += strTmp;
            strTmp.sprintf("%02X",modbus_func_ret);
            strRes += strTmp;

            // Example : int to hex string
            //   AnsiString str;
            //   int num = 15;
            //   str = IntToHex(num,2); //turns 15 into "0F"

            switch (modbus_func_ret)
            {
                case 0x01:  case 0x02:  case 0x03:
                case 0x04:  case 0x11:  case 0x17:
                strTmp.sprintf("%02X",res);
                strRes += strTmp;
                break;
            }

            for(i = 0 ; i < res ; ++i) // recover a string from recvbuf
            {
                strRes.Insert(IntToHex(recvbuf[i],2),strRes.Length()+1);
            }
            ResData->Text = strRes;
            ActStatus->Text = ("Request Done");
        }
        else
        {
            ResData->Text = *NullStr;
            ActStatus->Text = ("No Data Received");
        }
    }

    ptrClose(conn_num);
}
//---------------------------------------------------------------------------
void __fastcall TfRFID::Timer1Timer(TObject *Sender)
{
    if(USE_MR_SYSTEM && TestIF_File.bEnableView)
    {
        DoCheckViewChange();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfRFID::Button9Click(TObject *Sender)
{
    Timer1->Enabled=!Timer1->Enabled;
}
//---------------------------------------------------------------------------
void __fastcall TfRFID::DoCheckViewChange()
{
    for(int i=0; i<30; i++)
    {
        if(asViewListOld[i]!=asViewList[i])
        {
            asViewListOld[i]=asViewList[i];
            fRFID->DoView(0, i);
        }
    }

    for(int i=0; i<5; i++)
    {
        if(asViewMessageOld[i]!=asViewMessage[i])
        {
            asViewMessageOld[i]=asViewMessage[i];
            fRFID->DoView(2, i);
        }
    }
}
