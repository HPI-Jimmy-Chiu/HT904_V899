//---------------------------------------------------------------------------
#include "MachineDefine.h"
#include <vcl.h>
#pragma hdrstop

#include "cAutoAlignment.h"
#include "MyMotor.h"
#include "ainarm2.h"

#include "common.h"
#include "cmydef.h"
#include "mymessbox.h"
#include "cpublic.h"
#include "main.h"
#include "aoutarm.h"
#include "MyMotor.h"

#include "database.h"
#include "uMotorTest.h"
#include "ainarm2.h"
#include "LastSet.h"
#include "ainarm9045.h"
#include "aoutarm9045.h"
#include "myQwertyKeyBoard.h"
#include "cBinSel.h"
#include "mycylin.h"
#include "cinitial.h"
#include "note.h"
#include <stdlib.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "HTray"
#pragma link "ALed"
#pragma resource "*.dfm"
TfAutoAlignment *fAutoAlignment;
//In Top CCD Mark 點
enum eIMarkItem
{
    eInShut1_1  =0,         //In Shuttle1 取1點
    eInShut2_1  =1,         //In Shuttle2 取1點
    eHP2_1      =2,         //HP2 取3點
    eHP2_2      =3,
    eHP2_3      =4,
    eHP1_1      =5,         //HP1 取3點
    eHP1_2      =6,
    eHP1_3      =7,
    eLoad_1     =8,        //Load 取2點
    eLoad_2     =9,
    eIMarkTotal//10
};
//TEdit *edInMarkX[eIMarkTotal];
//TEdit *edInMarkY[eIMarkTotal];

//Out Top CCD Mark 點
enum eOMarkItem
{
    eAuto1_1    =0,         //Auto1 取2點
    eAuto1_2    =1,
    eAuto2_1    =2,         //Auto2 取3點
    eAuto2_2    =3,
    eAuto2_3    =4,
    eAuto3_1    =5,         //Auto3 取3點
    eAuto3_2    =6,
    eAuto3_3    =7,
    eFix_1      =8,         //Fix 取兩點
    eFix_2      =9,
    eOutShut1_1 =10,        //Out Shuttle1 取1點
    eOutShut2_1 =11,        //Out Shuttle1 取1點
    eOMarkTotal //12
};

//IN Picker
TEdit *edInPickerZ      [ePickerTotal];
TEdit *edInPickerOfsX   [ePickerTotal];
TEdit *edInPickerOfsY   [ePickerTotal];
TEdit *edInX40Picker    [ePickerTotal];
TEdit *edInX120Picker   [ePickerTotal];
//Out Picker
TEdit *edOutPickerZ     [ePickerTotal];
TEdit *edOutPickerOfsX  [ePickerTotal];
TEdit *edOutPickerOfsY  [ePickerTotal];
TEdit *edOutX40Picker   [ePickerTotal];
TEdit *edOutX120Picker  [ePickerTotal];
TEdit *edCCDIP[4];
TEdit *edCCDPort[4];
TEdit *edCMD[4];
//---------------------------------------------------------------------------
__fastcall TfAutoAlignment::TfAutoAlignment(TComponent* Owner)
    : TForm(Owner)
{
    InAlign=new TCAlignmentB("IN");
    OutAlign=new TCAlignmentB("OUT");
    bEnableVisionSimulate=true;
    InAlign->bEnableVisionSimulate=true;
    OutAlign->bEnableVisionSimulate=true;

    InAlign->vcAlign[0]->clntsckt=clntscktInTop;
    InAlign->vcAlign[1]->clntsckt=clntscktInBottom;
    OutAlign->vcAlign[0]->clntsckt=clntscktOutTop;
    OutAlign->vcAlign[1]->clntsckt=clntscktOutBottom;

    edCCDIP[0]=edCCDIP1;
    edCCDIP[1]=edCCDIP2;
    edCCDIP[2]=edCCDIP3;
    edCCDIP[3]=edCCDIP4;
    edCCDPort[0]=edCCDPort1;
    edCCDPort[1]=edCCDPort2;
    edCCDPort[2]=edCCDPort3;
    edCCDPort[3]=edCCDPort4;
    edCMD[0]=edCCDCMD1;
    edCMD[1]=edCCDCMD2;
    edCMD[2]=edCCDCMD3;
    edCMD[3]=edCCDCMD4;

    //IN Mark
//    edInMarkX[eInShut1_1]=edInSht1_1X;
//    edInMarkY[eInShut1_1]=edInSht1_1Y;
//    edInMarkX[eInShut1_2]=edInSht1_2X;
//    edInMarkY[eInShut1_2]=edInSht1_2Y;
//    edInMarkX[eInShut2_1]=edInSht2_1X;
//    edInMarkY[eInShut2_1]=edInSht2_1Y;
//    edInMarkX[eInShut2_2]=edInSht2_2X;
//    edInMarkY[eInShut2_2]=edInSht2_2Y;
//    edInMarkX[eHP2_1]=edHP2_1X;
//    edInMarkY[eHP2_1]=edHP2_1Y;
//    edInMarkX[eHP2_2]=edHP2_2X;
//    edInMarkY[eHP2_2]=edHP2_2Y;
//    edInMarkX[eHP2_3]=edHP2_3X;
//    edInMarkY[eHP2_3]=edHP2_3Y;
//    edInMarkX[eHP1_1]=edHP1_1X;
//    edInMarkY[eHP1_1]=edHP1_1Y;
//    edInMarkX[eHP1_2]=edHP1_2X;
//    edInMarkY[eHP1_2]=edHP1_2Y;
//    edInMarkX[eHP1_3]=edHP1_3X;
//    edInMarkY[eHP1_3]=edHP1_3Y;
//    edInMarkX[eLoad_1]=edLoad_1X;
//    edInMarkY[eLoad_1]=edLoad_1Y;
//    edInMarkX[eLoad_2]=edLoad_2X;
//    edInMarkY[eLoad_2]=edLoad_2Y;
//    edInMarkX[eLoad_3]=edLoad_3X;
//    edInMarkY[eLoad_3]=edLoad_3Y;
//    edInMarkX[eLoad_4]=edLoad_4X;
//    edInMarkY[eLoad_4]=edLoad_4Y;
    //IN PickerZ
//    edInPickerZ[ePickerA]=edInPickerAZ;
//    edInPickerZ[ePickerB]=edInPickerBZ;
//    edInPickerZ[ePickerC]=edInPickerCZ;
//    edInPickerZ[ePickerD]=edInPickerDZ;
//    edInPickerZ[ePickerE]=edInPickerEZ;
//    edInPickerZ[ePickerF]=edInPickerFZ;
//    edInPickerZ[ePickerG]=edInPickerGZ;
//    edInPickerZ[ePickerH]=edInPickerHZ;
    //IN Picker Ofs
    edInPickerOfsX [ePickerA]=edInPickerAOfsX;
    edInPickerOfsY [ePickerA]=edInPickerAOfsY;
    edInPickerOfsX [ePickerB]=edInPickerBOfsX;
    edInPickerOfsY [ePickerB]=edInPickerBOfsY;
    edInPickerOfsX [ePickerC]=edInPickerCOfsX;
    edInPickerOfsY [ePickerC]=edInPickerCOfsY;
    edInPickerOfsX [ePickerD]=edInPickerDOfsX;
    edInPickerOfsY [ePickerD]=edInPickerDOfsY;
    edInPickerOfsX [ePickerE]=edInPickerEOfsX;
    edInPickerOfsY [ePickerE]=edInPickerEOfsY;
    edInPickerOfsX [ePickerF]=edInPickerFOfsX;
    edInPickerOfsY [ePickerF]=edInPickerFOfsY;
    edInPickerOfsX [ePickerG]=edInPickerGOfsX;
    edInPickerOfsY [ePickerG]=edInPickerGOfsY;
    edInPickerOfsX [ePickerH]=edInPickerHOfsX;
    edInPickerOfsY [ePickerH]=edInPickerHOfsY;
    //IN Picker Pitch
    edInX40Picker  [ePickerA]=edInX40PickerA;
    edInX40Picker  [ePickerB]=edInX40PickerB;
    edInX40Picker  [ePickerC]=edInX40PickerC;
    edInX40Picker  [ePickerD]=edInX40PickerD;
    edInX40Picker  [ePickerE]=edInX40PickerE;
    edInX40Picker  [ePickerF]=edInX40PickerF;
    edInX40Picker  [ePickerG]=edInX40PickerG;
    edInX40Picker  [ePickerH]=edInX40PickerH;
    edInX120Picker [ePickerA]=edInX120PickerA;
    edInX120Picker [ePickerB]=edInX120PickerB;
    edInX120Picker [ePickerC]=edInX120PickerC;
    edInX120Picker [ePickerD]=edInX120PickerD;
    edInX120Picker [ePickerE]=edInX120PickerE;
    edInX120Picker [ePickerF]=edInX120PickerF;
    edInX120Picker [ePickerG]=edInX120PickerG;
    edInX120Picker [ePickerH]=edInX120PickerH;


    //Out Mark

//    edOutMarkX[eAuto1_1]=edAuto1_1X;
//    edOutMarkY[eAuto1_1]=edAuto1_1Y;
//    edOutMarkX[eAuto1_2]=edAuto1_2X;
//    edOutMarkY[eAuto1_2]=edAuto1_2Y;
//    edOutMarkX[eAuto1_3]=edAuto1_3X;
//    edOutMarkY[eAuto1_3]=edAuto1_3Y;
//    edOutMarkX[eAuto1_4]=edAuto1_4X;
//    edOutMarkY[eAuto1_4]=edAuto1_4Y;
//    edOutMarkX[eAuto2_1]=edAuto2_1X;
//    edOutMarkY[eAuto2_1]=edAuto2_1Y;
//    edOutMarkX[eAuto2_2]=edAuto2_2X;
//    edOutMarkY[eAuto2_2]=edAuto2_2Y;
//    edOutMarkX[eAuto3_1]=edAuto3_1X;
//    edOutMarkY[eAuto3_1]=edAuto3_1Y;
//    edOutMarkX[eAuto3_2]=edAuto3_2X;
//    edOutMarkY[eAuto3_2]=edAuto3_2Y;
//    edOutMarkX[eFix_1]=edFix_1X;
//    edOutMarkY[eFix_1]=edFix_1Y;
//    edOutMarkX[eFix_2]=edFix_2X;
//    edOutMarkY[eFix_2]=edFix_2Y;
//    edOutMarkX[eOutShut1_1]=edOutSht1_1X;
//    edOutMarkY[eOutShut1_1]=edOutSht1_1Y;
//    edOutMarkX[eOutShut1_2]=edOutSht1_2X;
//    edOutMarkY[eOutShut1_2]=edOutSht1_2Y;
//    edOutMarkX[eOutShut2_1]=edOutSht2_1X;
//    edOutMarkY[eOutShut2_1]=edOutSht2_1Y;
//    edOutMarkX[eOutShut2_2]=edOutSht2_2X;
//    edOutMarkY[eOutShut2_2]=edOutSht2_2Y;
    //Out PickerZ
//    edOutPickerZ[ePickerA]=edOutPickerAZ;
//    edOutPickerZ[ePickerB]=edOutPickerBZ;
//    edOutPickerZ[ePickerC]=edOutPickerCZ;
//    edOutPickerZ[ePickerD]=edOutPickerDZ;
//    edOutPickerZ[ePickerE]=edOutPickerEZ;
//    edOutPickerZ[ePickerF]=edOutPickerFZ;
//    edOutPickerZ[ePickerG]=edOutPickerGZ;
//    edOutPickerZ[ePickerH]=edOutPickerHZ;
    //IN Picker Ofs
    edOutPickerOfsX [ePickerA]=edOutPickerAOfsX;
    edOutPickerOfsY [ePickerA]=edOutPickerAOfsY;
    edOutPickerOfsX [ePickerB]=edOutPickerBOfsX;
    edOutPickerOfsY [ePickerB]=edOutPickerBOfsY;
    edOutPickerOfsX [ePickerC]=edOutPickerCOfsX;
    edOutPickerOfsY [ePickerC]=edOutPickerCOfsY;
    edOutPickerOfsX [ePickerD]=edOutPickerDOfsX;
    edOutPickerOfsY [ePickerD]=edOutPickerDOfsY;
    edOutPickerOfsX [ePickerE]=edOutPickerEOfsX;
    edOutPickerOfsY [ePickerE]=edOutPickerEOfsY;
    edOutPickerOfsX [ePickerF]=edOutPickerFOfsX;
    edOutPickerOfsY [ePickerF]=edOutPickerFOfsY;
    edOutPickerOfsX [ePickerG]=edOutPickerGOfsX;
    edOutPickerOfsY [ePickerG]=edOutPickerGOfsY;
    edOutPickerOfsX [ePickerH]=edOutPickerHOfsX;
    edOutPickerOfsY [ePickerH]=edOutPickerHOfsY;
    //IN Picker Pitch
    edOutX40Picker  [ePickerA]=edOutX40PickerA;
    edOutX40Picker  [ePickerB]=edOutX40PickerB;
    edOutX40Picker  [ePickerC]=edOutX40PickerC;
    edOutX40Picker  [ePickerD]=edOutX40PickerD;
    edOutX40Picker  [ePickerE]=edOutX40PickerE;
    edOutX40Picker  [ePickerF]=edOutX40PickerF;
    edOutX40Picker  [ePickerG]=edOutX40PickerG;
    edOutX40Picker  [ePickerH]=edOutX40PickerH;
    edOutX120Picker [ePickerA]=edOutX120PickerA;
    edOutX120Picker [ePickerB]=edOutX120PickerB;
    edOutX120Picker [ePickerC]=edOutX120PickerC;
    edOutX120Picker [ePickerD]=edOutX120PickerD;
    edOutX120Picker [ePickerE]=edOutX120PickerE;
    edOutX120Picker [ePickerF]=edOutX120PickerF;
    edOutX120Picker [ePickerG]=edOutX120PickerG;
    edOutX120Picker [ePickerH]=edOutX120PickerH;

    tmrProcessGetData->Enabled=true;
    DoAutoAlign(true);
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::FormShow(TObject *Sender)
{
    fShow=true;
    //這一行請保持在最下面!!-----------------
    myLog.Do_Log(Sender, asUser, asLogPath);    //Steven 20100629
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    fMain->Pause();
    fShow=false;
    //這一行請保持在最下面!!-----------------
    myLog.Do_Log(Sender, asUser, asLogPath);    //Steven 20100629
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::FormDestroy(TObject *Sender)
{
    delete InAlign;
    delete OutAlign;
    tmrCCDInitial->Enabled=false;
    tmrCCDConnect->Enabled=false;
    tmrProcessGetData->Enabled=false;
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::spbSaveClick(TObject *Sender)
{
    WriteFile();
    ReadFile();
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::sbtExitClick(TObject *Sender)
{
    if(bAutoAlignDone)
    {
        ShowMyMessage("Alignment Auto Tune need Finish !!", "需要完成 Alignment Auto Tune!!");
        return;
    }
    sbtExit->Down=false;
    Close();
    fShow=false;
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::clntscktConnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    AnsiString Str;
    TClientSocket *Ptr;
    Ptr=(TClientSocket *)Sender;
    if(InitialOK==false)
    {
        Ptr->Active=false;
        return;
    }

    Str.sprintf("IP: %s, Port:%d [ConnectOK]", Ptr->Address, Ptr->Port);
    SendCommand(Ptr->Tag,Str, "");

    if(Ptr->Tag==0)
    {
        InAlign->vcAlign[0]->ChangeSetupFile();
    }
    else if(InAlign->vcAlign[0]->dVisionVer>=0)
    {
        if(Ptr->Tag==0 &&  InAlign->vcAlign[0]->bEnableVision==true)
        {
            Str.sprintf("E9,3");
            SendCommand(Ptr->Tag,"Clear buffer", Str);      //Reset CCD Shuttle暫存器
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::clntscktAlignDisconnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    AnsiString Str;
    TClientSocket *Ptr;
    Ptr=(TClientSocket *)Sender;
    if(InitialOK==false)
    {
        return;
    }

    Str.sprintf("IP: %s, Port:%d [DisConnect]", Ptr->Address, Ptr->Port);
    SendCommand(Ptr->Tag,Str, "");
}
//---------------------------------------------------------------------------

void __fastcall TfAutoAlignment::clntscktAlignError(TObject *Sender,
      TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
    TClientSocket *Ptr;
    Ptr=(TClientSocket *)Sender;
    if(InitialOK==false)
    {
        return;
    }

    SendCommand(Ptr->Tag,GetErrorMsg(ErrorCode), "");

    ErrorEvent=TErrorEvent(NULL);
    ErrorCode=0;

    if(Ptr->Active)
        Ptr->Close();

    Abort();
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::clntscktAlignRead(TObject *Sender,
      TCustomWinSocket *Socket)
{
    AnsiString EthernetBuffer;
    AnsiString asLog;
    int iBufferLenght=0;

    if(InitialOK==false)
    {
        return;
    }

    TClientSocket *Ptr;
    Ptr=(TClientSocket *)Sender;

    iBufferLenght=Socket->ReceiveLength();
    if(iBufferLenght>0)
    {
        EthernetBuffer=Socket->ReceiveText();
        EthernetBuffer=StringReplace(EthernetBuffer, "\r", ",", TReplaceFlags()<<rfReplaceAll);
        EthernetBuffer=StringReplace(EthernetBuffer, "\n", "", TReplaceFlags()<<rfReplaceAll);
        SocketRead(Ptr->Tag,EthernetBuffer);
    }
}
//---------------------------------------------------------------------------
void TfAutoAlignment::SocketRead(int iCCD,AnsiString EthernetBuffer)
{
    AnsiString asLog;
    asLog.sprintf("Recv, , %s", EthernetBuffer);
    if(iCCD==0)
    {
        InAlign->vcAlign[0]->SocketReceiveList->Add(EthernetBuffer);
        InAlign->vcAlign[0]->AddCommandLog(asLog);
    }
    else if(iCCD==1)
    {
        InAlign->vcAlign[1]->SocketReceiveList->Add(EthernetBuffer);
        InAlign->vcAlign[1]->AddCommandLog(asLog);
    }
    else if(iCCD==2)
    {
        OutAlign->vcAlign[0]->SocketReceiveList->Add(EthernetBuffer);
        OutAlign->vcAlign[0]->AddCommandLog(asLog);
    }
    else if(iCCD==3)
    {
        OutAlign->vcAlign[1]->SocketReceiveList->Add(EthernetBuffer);
        OutAlign->vcAlign[1]->AddCommandLog(asLog);
    }
}
//---------------------------------------------------------------------------
int TfAutoAlignment::WriteFile()
{
    int ret;
    DoUIToData();
    //ret=WriteData("d:\\HT9045\\system\\aligntech.dat", (char *)&AlignTeach.IO[0].dtPixelSizeX, sizeof(ALIGNTEACH));

    AnsiString S="";
    S=GetLastOpenFN();
    AnsiString szDir="";
    szDir.sprintf("%s%s\\HandlerCondition.Data", DataPath, S);
    WriteIniData(szDir, "Configuration", "Auto Alignment Enable",(TestIF_File.bEnableAutoAlign)?1:0);
    WriteIniData(szDir, "Configuration", "Auto Alignment Offset Alarm",TestIF_File.iAlignOffsetAlarm);
    WriteIniData(szDir, "Configuration", "Auto Alignment Accuracy",TestIF_File.iAccuracy);

    WriteIniData(szDir, "Configuration", "Auto Alignment InTopPixelSizeX",TestIF_File.dInTopPixelSizeX);
    WriteIniData(szDir, "Configuration", "Auto Alignment InTopPixelSizeY",TestIF_File.dInTopPixelSizeY);

    WriteIniData(szDir, "Configuration", "Auto Alignment InBottomPixelSizeX",TestIF_File.dInBottomPixelSizeX);
    WriteIniData(szDir, "Configuration", "Auto Alignment InBottomPixelSizeY",TestIF_File.dInBottomPixelSizeY);

    WriteIniData(szDir, "Configuration", "Auto Alignment OutTopPixelSizeX",TestIF_File.dOutTopPixelSizeX);
    WriteIniData(szDir, "Configuration", "Auto Alignment OutTopPixelSizeY",TestIF_File.dOutTopPixelSizeY);

    WriteIniData(szDir, "Configuration", "Auto Alignment OutBottomPixelSizeX",TestIF_File.dOutBottomPixelSizeX);
    WriteIniData(szDir, "Configuration", "Auto Alignment OutBottomPixelSizeY",TestIF_File.dOutBottomPixelSizeY);

    return ret;
}
//---------------------------------------------------------------------------
int TfAutoAlignment::ReadFile()
{
    int ret;
    //ret=ReadData("d:\\HT9045\\system\\aligntech.dat",(char *)&AlignTeach.IO[0].dtPixelSizeX, sizeof(ALIGNTEACH));

    AnsiString S="";
    S=GetLastOpenFN();
    AnsiString szDir="";
    szDir.sprintf("%s%s\\HandlerCondition.Data", DataPath, S);
    if(USE_AUTO_ALIGNMENT)
    {
        TestIF_File.bEnableAutoAlign=ReadIniData(szDir, "Configuration","Auto Alignment Enable",false);
    }
    else
    {
        TestIF_File.bEnableAutoAlign=false;
    }
    TestIF_File.iAlignOffsetAlarm   =ReadIniData(szDir, "Configuration","Auto Alignment Offset Alarm",20);
    TestIF_File.iAccuracy           =ReadIniData(szDir, "Configuration","Auto Alignment Offset Alarm",3);

    TestIF_File.dInTopPixelSizeX    =ReadIniData(szDir, "Configuration","Auto Alignment InTopPixelSizeX",10);
    TestIF_File.dInTopPixelSizeY    =ReadIniData(szDir, "Configuration","Auto Alignment InTopPixelSizeY",10);

    TestIF_File.dInBottomPixelSizeX =ReadIniData(szDir, "Configuration","Auto Alignment InBottomPixelSizeX",10);
    TestIF_File.dInBottomPixelSizeY =ReadIniData(szDir, "Configuration","Auto Alignment InBottomPixelSizeY",10);

    TestIF_File.dOutTopPixelSizeX   =ReadIniData(szDir, "Configuration","Auto Alignment OutTopPixelSizeX",10);
    TestIF_File.dOutTopPixelSizeY   =ReadIniData(szDir, "Configuration","Auto Alignment OutTopPixelSizeY",10);

    TestIF_File.dOutBottomPixelSizeX=ReadIniData(szDir, "Configuration","Auto Alignment OutBottomPixelSizeX",10);
    TestIF_File.dOutBottomPixelSizeY=ReadIniData(szDir, "Configuration","Auto Alignment OutBottomPixelSizeY",10);

    DoDataToUI();

    InAlign->vcAlign[0]->ChangeSetupFile();
    return ret;
}
//---------------------------------------------------------------------------
void TfAutoAlignment::DoDataToUI()
{
    int i=0;
    rgAutoAlignOnOff->ItemIndex=(TestIF_File.bEnableAutoAlign==false)?0:1;
    edAlignOffsetAlarm->Text=IntToStr(TestIF_File.iAlignOffsetAlarm);

    for(i=0; i<4; i++)
    {
        edCCDIP[i]->Text=HSys.asCCDAlignIP[i];
        edCCDPort[i]->Text=HSys.asCCDAlignPort[i];
    }

    InAlign->vcAlign[0]->asIP=HSys.asCCDAlignIP[0];
    InAlign->vcAlign[0]->iPort=atoi(HSys.asCCDAlignPort[0].c_str());
    InAlign->vcAlign[1]->asIP=HSys.asCCDAlignIP[1];
    InAlign->vcAlign[1]->iPort=atoi(HSys.asCCDAlignPort[1].c_str());
    OutAlign->vcAlign[0]->asIP=HSys.asCCDAlignIP[2];
    OutAlign->vcAlign[0]->iPort=atoi(HSys.asCCDAlignPort[2].c_str());
    OutAlign->vcAlign[1]->asIP=HSys.asCCDAlignIP[3];
    OutAlign->vcAlign[1]->iPort=atoi(HSys.asCCDAlignPort[3].c_str());

    //In Top
    edInTopPixelSizeX->Text=FloatToStr(TestIF_File.dInTopPixelSizeX);
    edInTopPixelSizeY->Text=FloatToStr(TestIF_File.dInTopPixelSizeY);
//    edInDummyX->Text=IntToStr(AlignTeach.IO[0].vtDummy.iOriX);
//    edInDummyY->Text=IntToStr(AlignTeach.IO[0].vtDummy.iOriY);
//    edInDummyZ->Text=IntToStr(AlignTeach.IO[0].itDummyZ);
//    edInTopCCDToBasePickerX->Text=IntToStr(AlignTeach.IO[0].vtTop2BasePicker.iOriX);
//    edInTopCCDToBasePickerY->Text=IntToStr(AlignTeach.IO[0].vtTop2BasePicker.iOriY);
    //In Mark
//    for(i=0; i<eIMarkTotal; i++)
//    {
//        edInMarkX[i]->Text=IntToStr(AlignTeach.IO[0].vtMark[i].iOriX);
//        edInMarkY[i]->Text=IntToStr(AlignTeach.IO[0].vtMark[i].iOriY);
//    }
    //In Bottom
    edInBottomPixelSizeX->Text=FloatToStr(TestIF_File.dInBottomPixelSizeX);
    edInBottomPixelSizeY->Text=FloatToStr(TestIF_File.dInBottomPixelSizeY);
//    edInBasePickerOnBottomX->Text=IntToStr(AlignTeach.IO[0].vbBasePickerOnBottom.iOriX);
//    edInBasePickerOnBottomY->Text=IntToStr(AlignTeach.IO[0].vbBasePickerOnBottom.iOriY);
    //In PickerZ
//    for(i=0; i<ePickerTotal; i++)
//    {
//        edInPickerZ[i]->Text=IntToStr(AlignTeach.IO[0].iPickerZ[i]);
//    }
    //Out Top
    edOutTopPixelSizeX->Text=FloatToStr(TestIF_File.dOutTopPixelSizeX);
    edOutTopPixelSizeY->Text=FloatToStr(TestIF_File.dOutTopPixelSizeY);
//    edOutDummyX->Text=IntToStr(AlignTeach.IO[1].vtDummy.iOriX);
//    edOutDummyY->Text=IntToStr(AlignTeach.IO[1].vtDummy.iOriY);
//    edOutDummyZ->Text=IntToStr(AlignTeach.IO[1].itDummyZ);
//    edOutTopCCDToBasePickerX->Text=IntToStr(AlignTeach.IO[1].vtTop2BasePicker.iOriX);
//    edOutTopCCDToBasePickerY->Text=IntToStr(AlignTeach.IO[1].vtTop2BasePicker.iOriY);

    //Out Mark
//    for(i=0; i<eOMarkTotal; i++)
//    {
//        edOutMarkX[i]->Text=IntToStr(AlignTeach.IO[1].vtMark[i].iOriX);
//        edOutMarkY[i]->Text=IntToStr(AlignTeach.IO[1].vtMark[i].iOriY);
//    }

    //Out Bottom
    edOutBottomPixelSizeX->Text=FloatToStr(TestIF_File.dOutBottomPixelSizeX);
    edOutBottomPixelSizeY->Text=FloatToStr(TestIF_File.dOutBottomPixelSizeY);
//    edOutBasePickerOnBottomX->Text=IntToStr(AlignTeach.IO[1].vbBasePickerOnBottom.iOriX);
//    edOutBasePickerOnBottomY->Text=IntToStr(AlignTeach.IO[1].vbBasePickerOnBottom.iOriY);
    //Out PickerZ
//    for(i=0; i<ePickerTotal; i++)
//    {
//        edOutPickerZ[i]->Text=IntToStr(AlignTeach.IO[1].iPickerZ[i]);
//    }
}
//---------------------------------------------------------------------------
void TfAutoAlignment::DoUIToData()
{
    int i=0;
    TestIF_File.bEnableAutoAlign=(rgAutoAlignOnOff->ItemIndex==0)?false:true;
    TestIF_File.iAlignOffsetAlarm=atoi(edAlignOffsetAlarm->Text.c_str());
    TestIF_File.iAccuracy=atoi(edAlignOffsetAlarm->Text.c_str());
    //In Top
    TestIF_File.dInTopPixelSizeX=atof(edInTopPixelSizeX->Text.c_str());
    TestIF_File.dInTopPixelSizeY=atof(edInTopPixelSizeY->Text.c_str());
//    AlignTeach.IO[0].vtDummy.iOriX=atoi(edInDummyX->Text.c_str());
//    AlignTeach.IO[0].vtDummy.iOriY=atoi(edInDummyY->Text.c_str());
//    AlignTeach.IO[0].itDummyZ=atoi(edInDummyZ->Text.c_str());
//    AlignTeach.IO[0].vtTop2BasePicker.iOriX=atoi(edInTopCCDToBasePickerX->Text.c_str());
//    AlignTeach.IO[0].vtTop2BasePicker.iOriY=atoi(edInTopCCDToBasePickerY->Text.c_str());
    //In Mark
//    for(i=0; i<eIMarkTotal; i++)
//    {
//        AlignTeach.IO[0].vtMark[i].iOriX=atoi(edInMarkX[i]->Text.c_str());
//        AlignTeach.IO[0].vtMark[i].iOriY=atoi(edInMarkY[i]->Text.c_str());
//    }
    //In Bottom
    TestIF_File.dInBottomPixelSizeX=atof(edInBottomPixelSizeX->Text.c_str());
    TestIF_File.dInBottomPixelSizeY=atof(edInBottomPixelSizeY->Text.c_str());
//    AlignTeach.IO[0].vbBasePickerOnBottom.iOriX=atoi(edInBasePickerOnBottomX->Text.c_str());
//    AlignTeach.IO[0].vbBasePickerOnBottom.iOriY=atoi(edInBasePickerOnBottomY->Text.c_str());
    //In PickerZ
//    for(i=0; i<ePickerTotal; i++)
//    {
//        AlignTeach.IO[0].iPickerZ[i]=atoi(edInPickerZ[i]->Text.c_str());
//    }

    //Out Top
    TestIF_File.dOutTopPixelSizeX=atof(edOutTopPixelSizeX->Text.c_str());
    TestIF_File.dOutTopPixelSizeY=atof(edOutTopPixelSizeY->Text.c_str());
//    AlignTeach.IO[1].vtDummy.iOriX=atoi(edOutDummyX->Text.c_str());
//    AlignTeach.IO[1].vtDummy.iOriY=atoi(edOutDummyY->Text.c_str());
//    AlignTeach.IO[1].itDummyZ=atoi(edOutDummyZ->Text.c_str());
//    AlignTeach.IO[1].vtTop2BasePicker.iOriX=atoi(edOutTopCCDToBasePickerX->Text.c_str());
//    AlignTeach.IO[1].vtTop2BasePicker.iOriY=atoi(edOutTopCCDToBasePickerY->Text.c_str());
    //Out Mark
//    for(i=0; i<eOMarkTotal; i++)
//    {
//        AlignTeach.IO[1].vtMark[i].iOriX=atoi(edOutMarkX[i]->Text.c_str());
//        AlignTeach.IO[1].vtMark[i].iOriY=atoi(edOutMarkY[i]->Text.c_str());
//    }
    //Out Bottom
    TestIF_File.dOutBottomPixelSizeX==atof(edOutBottomPixelSizeX->Text.c_str());
    TestIF_File.dOutBottomPixelSizeY==atof(edOutBottomPixelSizeY->Text.c_str());
//    AlignTeach.IO[1].vbBasePickerOnBottom.iOriX=atoi(edOutBasePickerOnBottomX->Text.c_str());
//    AlignTeach.IO[1].vbBasePickerOnBottom.iOriY=atoi(edOutBasePickerOnBottomY->Text.c_str());
    //Out PickerZ
//    for(i=0; i<ePickerTotal; i++)
//    {
//        AlignTeach.IO[1].iPickerZ[i]=atoi(edOutPickerZ[i]->Text.c_str());
//    }
}
//---------------------------------------------------------------------------
void TfAutoAlignment::SendCommand(int iCCD,AnsiString Msg, AnsiString Msg2)
{
    if(iCCD==0)
        InAlign->vcAlign[0]->SendCommand(Msg, Msg2);
    else if(iCCD==1)
        InAlign->vcAlign[1]->SendCommand(Msg, Msg2);
    else if(iCCD==2)
        OutAlign->vcAlign[0]->SendCommand(Msg, Msg2);
    else if(iCCD==3)
        OutAlign->vcAlign[1]->SendCommand(Msg, Msg2);
}
//---------------------------------------------------------------------------
int TfAutoAlignment::DoAutoAlign(bool bReset)
{
    int iReturn=0;
    static bool bMoveInAllZ=false,bMoveOutAllZ=false,bMoveTrayX=false,bMoveTrayZ=false,bIndexArmZ=false;
    static int iInAlignOK=0,iOutAlignOK=0;
    int &Task=iAutoAlignTask;
    if(bReset==true)
    {
        bMoveInAllZ=false;
        bMoveOutAllZ=false;
        Task=1;
        return iReturn;
    }
    switch(Task)
    {
        case 1:
            bMoveInAllZ=false;
            bMoveOutAllZ=false;
            bMoveTrayX=false;
            bMoveTrayZ=false;
            bIndexArmZ=false;
            InAlign->iMode=iMode;
            OutAlign->iMode=iMode;
            Task=100;
            break;

        case 100:
            if(bMoveInAllZ==false)          //InArmZ 上升到安全位置
            {
               // bMoveInAllZ=MoveInArmZToPlateSafe();
            }
            if(bMoveOutAllZ==false)         //OutArmZ 上升到安全位置
            {
                bMoveOutAllZ=MoveOutArmZToPlateSafe();
            }
            if(bMoveTrayZ==false)           //TrayArmZ 汽缸上升
            {
               bMoveTrayZ=Cylinder[C_TrayX_UpDown].Pop();
            }

            if(bIndexArmZ==false)           //IndexArmZ 上升到安全位置
            {
                bIndexArmZ=MOT[MTestZ1].Gali_Two_ZAxis_Move(0, 30*5000);
            }

            if(bMoveInAllZ==true && bMoveOutAllZ==true && bMoveTrayZ==true && bIndexArmZ==true)
            {
                bMoveTrayX=false;
                Task=200;
            }
            break;
        case 200:
            if(bMoveTrayX==false)
            {
                bMoveTrayX=MOT[MTrayX].MotorMove(Prod.iXTrayEmpty+13600); //TrayX 移動到安全位置
            }
            if(bMoveTrayX==true)
            {
//                if(iMode==1)
                {
                    InAlign->DoAlign(true);
                    OutAlign->DoAlign(true);
                    iInAlignOK=0;
                    iOutAlignOK=0;
                    Task=400;
                }
//                else
//                {
//                    AutoTeachLoadTrayZ(true, InArm, iInArmZTeachTask);
//                    AutoTeachLoadTrayZ(true, OutArm, iOutArmZTeachTask);
//                    Task=300;
//                }
            }
            break;
        case 300:  //Inarm Z Auto High
            //if(AutoTeachLoadTrayZ(false, InArm, iInArmZTeachTask))
            {
                Task=301;
            }
            break;
        case 301:  //Outarm Z Auto High
            //if(AutoTeachLoadTrayZ(false,OutArm,iOutArmZTeachTask))
            {
                InAlign->DoAlign(true);
                OutAlign->DoAlign(true);
                iInAlignOK=0;
                iOutAlignOK=0;
                Task=400;
            }

        case 400:
            OutAlign->bShuttleIsBusy[0]=InAlign->bShuttleIsBusy[0];  //將  IN 控制 bShuttleIsBusy[0] 狀態放到 OUT bShuttleIsBusy[0]
            InAlign->bShuttleIsBusy[1]=OutAlign->bShuttleIsBusy[1];  //將 OUT 控制 bShuttleIsBusy[1] 狀態放到 IN bShuttleIsBusy[1]
            if(iInAlignOK==0)
            {
                iInAlignOK=InAlign->DoAlign();//IN Auto Alignment;
            }

//            if(iInAlignOK==0 || iInAlignOK==1)
//            {
//
//            }
//            else
//            {
//
//            }

            if(iOutAlignOK==0)
            {
                iOutAlignOK=OutAlign->DoAlign();//OUT Auto Alignment;
                //iOutAlignOK=1;//QQ          //OUT Auto Alignment;
            }

//            if(iOutAlignOK==0 || iOutAlignOK==1)
//            {
//
//            }
//            else
//            {
//
//            }

            if(iInAlignOK==1 && iOutAlignOK==1)
            {
              Task=500;
            }
            break;
        case 500:
            Task=1;
            ShowResult();
            bRunAutoAlign=false;
//            fMain->Pause();
            break;
    }
    return iReturn;
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::tmrCCDInitialTimer(TObject *Sender)
{
    static bool bTimerRun=false;
    if(InitialOK==false || bTimerRun==true || bEnableVisionSimulate==true)
    {
        return;
    }
    bTimerRun=true;

    InAlign->vcAlign[0]->ConnectOkInitial();

    bTimerRun=false;
}
//---------------------------------------------------------------------------

void __fastcall TfAutoAlignment::tmrCCDConnectTimer(TObject *Sender)
{
    static bool bTimerRun=false;
    if(InitialOK==false || bTimerRun==true || bEnableVisionSimulate==true)
    {
        return;
    }
    bTimerRun=true;

    InAlign->vcAlign[0]->Connect();
    InAlign->vcAlign[1]->Connect();
    OutAlign->vcAlign[0]->Connect();
    OutAlign->vcAlign[1]->Connect();

    bTimerRun=false;
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::tmrProcessGetDataTimer(TObject *Sender)
{
    static bool bTimerRun=false;
    if(InitialOK==false || bTimerRun==true)
    {
        return;
    }
    bTimerRun=true;

    InAlign->vcAlign[0]->ProcessGetData();
    InAlign->vcAlign[1]->ProcessGetData();
    OutAlign->vcAlign[0]->ProcessGetData();
    OutAlign->vcAlign[1]->ProcessGetData();

    bTimerRun=false;
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::sbStartClick(TObject *Sender)
{
    if(SystemStart)
        return;
    fMain->Start();
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::sbPasueClick(TObject *Sender)
{
    fMain->Pause();
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::btnCCDConnectClick(TObject *Sender)
{
    TSpeedButton *tempBtn;
    tempBtn = (TSpeedButton *)Sender;
    if(tempBtn->Tag==0)
    {
        InAlign->vcAlign[0]->asIP=edCCDIP[tempBtn->Tag]->Text;
        InAlign->vcAlign[0]->iPort=atoi(edCCDPort[tempBtn->Tag]->Text.c_str());
        InAlign->vcAlign[0]->bEnableVision=true;
    }
    else if(tempBtn->Tag==1)
    {
        InAlign->vcAlign[1]->asIP=edCCDIP[tempBtn->Tag]->Text;
        InAlign->vcAlign[1]->iPort=atoi(edCCDPort[tempBtn->Tag]->Text.c_str());
        InAlign->vcAlign[1]->bEnableVision=true;
    }
    else if(tempBtn->Tag==2)
    {
        OutAlign->vcAlign[0]->asIP=edCCDIP[tempBtn->Tag]->Text;
        OutAlign->vcAlign[0]->iPort=atoi(edCCDPort[tempBtn->Tag]->Text.c_str());
        OutAlign->vcAlign[0]->bEnableVision=true;
    }
    else if(tempBtn->Tag==3)
    {
        OutAlign->vcAlign[1]->asIP=edCCDIP[tempBtn->Tag]->Text;
        OutAlign->vcAlign[1]->iPort=atoi(edCCDPort[tempBtn->Tag]->Text.c_str());
        OutAlign->vcAlign[1]->bEnableVision=true;
    }
}
//---------------------------------------------------------------------------

void __fastcall TfAutoAlignment::btnCCDDisconnectClick(TObject *Sender)
{
    TSpeedButton *tempBtn;
    tempBtn = (TSpeedButton *)Sender;
    if(tempBtn->Tag==0)
    {
        InAlign->vcAlign[0]->bEnableVision=false;
        InAlign->vcAlign[0]->clntsckt->Close();
    }
    else if(tempBtn->Tag==1)
    {
        InAlign->vcAlign[1]->bEnableVision=false;
        InAlign->vcAlign[1]->clntsckt->Close();
    }
    else if(tempBtn->Tag==2)
    {
        OutAlign->vcAlign[0]->bEnableVision=false;
        OutAlign->vcAlign[0]->clntsckt->Close();
    }
    else if(tempBtn->Tag==3)
    {
        OutAlign->vcAlign[1]->bEnableVision=false;
        OutAlign->vcAlign[1]->clntsckt->Close();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::btnCCDSendClick(TObject *Sender)
{
    AnsiString asTarget;
    TSpeedButton *tempBtn;
    tempBtn = (TSpeedButton *)Sender;
    if(tempBtn->Tag==0)
    {
        asTarget.sprintf("IP: %s, Port:%d [Test Command]", InAlign->vcAlign[0]->asIP,InAlign->vcAlign[0]->iPort);
    }
    else if(tempBtn->Tag==1)
    {
        asTarget.sprintf("IP: %s, Port:%d [Test Command]", InAlign->vcAlign[1]->asIP,InAlign->vcAlign[1]->iPort);
    }
    else if(tempBtn->Tag==2)
    {
        asTarget.sprintf("IP: %s, Port:%d [Test Command]", OutAlign->vcAlign[0]->asIP,OutAlign->vcAlign[0]->iPort);
    }
    else if(tempBtn->Tag==3)
    {
        asTarget.sprintf("IP: %s, Port:%d [Test Command]", OutAlign->vcAlign[1]->asIP,OutAlign->vcAlign[1]->iPort);
    }

    SendCommand(tempBtn->Tag,asTarget,edCMD[tempBtn->Tag]->Text);
}
//---------------------------------------------------------------------------
void TfAutoAlignment::ShowResult()
{
    int i;
    //IN Mark Offset
    edLoadOfsX->Text   =IntToStr(InAlign->iMarkOfsX[eLoad_1]);
    edLoadOfsY->Text   =IntToStr(InAlign->iMarkOfsY[eLoad_1]);
    edHPOfsX1->Text    =IntToStr((InAlign->iMarkOfsX[eHP1_1]+InAlign->iMarkOfsX[eHP1_2])/2);
    edHPOfsY1->Text    =IntToStr((InAlign->iMarkOfsY[eHP1_2]+InAlign->iMarkOfsY[eHP1_3])/2);
    edHPOfsX2->Text    =IntToStr((InAlign->iMarkOfsX[eHP2_1]+InAlign->iMarkOfsX[eHP2_2])/2);
    edHPOfsY2->Text    =IntToStr((InAlign->iMarkOfsY[eHP2_2]+InAlign->iMarkOfsY[eHP2_3])/2);
    edInShtOfsX1->Text =IntToStr(InAlign->iMarkOfsX[eInShut1_1]);
    edInShtOfsY1->Text =IntToStr(InAlign->iMarkOfsY[eInShut1_1]);
    edInShtOfsX2->Text =IntToStr(InAlign->iMarkOfsX[eInShut2_1]);
    edInShtOfsY2->Text =IntToStr(InAlign->iMarkOfsY[eInShut2_1]);

    //IN TopCCD To BasePicker Offset
    edInTopCCDToBasePickerOfsX->Text=IntToStr(InAlign->iTop2BasePickerOfsX);
    edInTopCCDToBasePickerOfsY->Text=IntToStr(InAlign->iTop2BasePickerOfsY);

    //IN Picker/Pitch
    for(i=0; i<ePickerTotal; i++)
    {
        //Offset
        edInPickerOfsX[i]->Text=IntToStr(InAlign->iX40PitchTeachOfsX[i]);
        edInPickerOfsY[i]->Text=IntToStr(InAlign->iX40PitchTeachOfsY[i]);
        //Pitch
        edInX40Picker[i]->Text=IntToStr(InAlign->iX40PitchCount[i]);
        edInX120Picker[i]->Text=IntToStr(InAlign->iX120PitchCount[i]);
    }
    edInX40Avg->Text=IntToStr(InAlign->iX40PitchCountAvg);
    edInX120Avg->Text=IntToStr(InAlign->iX120PitchCountAvg);

    //OUT Mark Offset
    edAutoOfsX1->Text   =IntToStr(OutAlign->iMarkOfsX[eAuto1_1]);
    edAutoOfsY1->Text   =IntToStr(OutAlign->iMarkOfsY[eAuto1_1]);
    edAutoOfsX2->Text   =IntToStr(OutAlign->iMarkOfsX[eAuto2_1]);
    edAutoOfsY2->Text   =IntToStr(OutAlign->iMarkOfsY[eAuto2_1]);
    edAutoOfsX3->Text   =IntToStr(OutAlign->iMarkOfsX[eAuto3_1]);
    edAutoOfsY3->Text   =IntToStr(OutAlign->iMarkOfsY[eAuto3_1]);
    edFixOfsX->Text     =IntToStr(OutAlign->iMarkOfsX[eFix_1]);
    edFixOfsY->Text     =IntToStr(OutAlign->iMarkOfsY[eFix_1]);

    edOutShtOfsX1->Text =IntToStr(OutAlign->iMarkOfsX[eOutShut1_1]);
    edOutShtOfsY1->Text =IntToStr(OutAlign->iMarkOfsY[eOutShut1_1]);
    edOutShtOfsX2->Text =IntToStr(OutAlign->iMarkOfsX[eOutShut2_1]);
    edOutShtOfsY2->Text =IntToStr(OutAlign->iMarkOfsY[eOutShut2_1]);
    //OUT TopCCD To BasePicker Offset
    edOutTopCCDToBasePickerOfsX->Text=IntToStr(OutAlign->iTop2BasePickerOfsX);
    edOutTopCCDToBasePickerOfsY->Text=IntToStr(OutAlign->iTop2BasePickerOfsY);

    //OUT Picker/Pitch
    for(i=0; i<ePickerTotal; i++)
    {
        //Offset
        edOutPickerOfsX[i]->Text=IntToStr(OutAlign->iX40PitchTeachOfsX[i]);
        edOutPickerOfsY[i]->Text=IntToStr(OutAlign->iX40PitchTeachOfsY[i]);
        //Pitch
        edOutX40Picker[i]->Text=IntToStr(OutAlign->iX40PitchCount[i]);
        edOutX120Picker[i]->Text=IntToStr(OutAlign->iX120PitchCount[i]);
    }
    edOutX40Avg->Text=IntToStr(OutAlign->iX40PitchCountAvg);
    edOutX120Avg->Text=IntToStr(OutAlign->iX120PitchCountAvg);
}

