
#include "MachineDefine.h"
#pragma hdrstop

#include "LaserSensor.h"
#include "LaserSensorShuttle.h"
#include "LaserSensorInArm.h"
#include "main.h"
#include "HTEditList.h"
//---------------------------------------------------------------------------

#include "database.h"
//#include "TextProcess.h"
#include "mymessbox.h"
#include "Common.h"
#include "cmydef.h"
#include "cprod.h"
#include "myQwertyKeyBoard.h"
#include "MyMotor.h"

#pragma package(smart_init)
#pragma link "SPComm"
#pragma link "ALed"
#pragma link "HTray"
#pragma resource "*.dfm"
TfLaserSensor *fLaserSensor;

#define STX       2
#define ETX       3
AnsiString asLaserValue[2];                                                     //Eastsun 20260525 laser 整合
//---------------------------------------------------------------------------
__fastcall TfLaserSensor::TfLaserSensor(TComponent* Owner)
    : TForm(Owner)
{
    sResponseCode[0][rcl0000]="0000"; sResponseCode[1][rcl0000]="Response Code OK.";
    sResponseCode[0][rcl1001]="1001"; sResponseCode[1][rcl1001]="命令過長";
    sResponseCode[0][rcl1002]="1002"; sResponseCode[1][rcl1002]="命令過短";
    sResponseCode[0][rcl1003]="1003"; sResponseCode[1][rcl1003]="數量不符, 數據數量與元素數量不符";
    sResponseCode[0][rcl1101]="1101"; sResponseCode[1][rcl1101]="區域類型錯誤, 對應的變數類型並不存在";
    sResponseCode[0][rcl1103]="1103"; sResponseCode[1][rcl1103]="Start address, An error range outside";
    sResponseCode[0][rcl1104]="1104"; sResponseCode[1][rcl1104]="End address, An error range outside";
    sResponseCode[0][rcl2203]="2203"; sResponseCode[1][rcl2203]="操作錯誤, Read-out error";
    sResponseCode[0][rcl2204]="2204"; sResponseCode[1][rcl2204]="操作錯誤, When the mode of a sensor of operation is except RUN mode.";
    sResponseCode[0][rcl2205]="2205"; sResponseCode[1][rcl2205]="操作錯誤, When the command besides regulation is required.";

    sEndCode[0][ec00]="00"; sEndCode[1][ec00]="End Code OK.";
    sEndCode[0][ec0F]="0F"; sEndCode[1][ec0F]="Command error, 無法執行指定的命令";
    sEndCode[0][ec10]="10"; sEndCode[1][ec10]="Parity error, 數據接收中的1位OR與通信校驗中設定不符合";
    sEndCode[0][ec11]="11"; sEndCode[1][ec11]="Framing erro, stop bit is 0.";
    sEndCode[0][ec12]="12"; sEndCode[1][ec12]="Overrun error, 數據暫存已滿時收新數據";
    sEndCode[0][ec13]="13"; sEndCode[1][ec13]="BCC error, BCC檢查碼不符";
    sEndCode[0][ec14]="14"; sEndCode[1][ec14]="Format error, 格式錯誤";
    sEndCode[0][ec16]="16"; sEndCode[1][ec16]="Sub-address error, 子地址不存在";
    sEndCode[0][ec18]="18"; sEndCode[1][ec18]="Frame length error, 接收到的數量超出指定字節數";

    iMaxChannel=2;              //最多幾顆
    iCurrentChannel=0;          //目前做到第幾顆
    iLaserTask=1;
    iLaser1Task=1;
    iLaser2Task=1;
    iLaserInArmTask=1;
    iLaserOutArmTask=1;   //Eastsun 20260525 laser 整合 MissF-5
    iMaxStep=8;                 //要掃幾次
    iCurrentStep=iMaxStep-1;    //目前掃到第幾次

    bSetToZero=false;

    TPanel *Ptr[]={palLaserValue_InShuttle2_2,palLaserValue_InShuttle2_1,palLaserValue_InShuttle1_2,palLaserValue_InShuttle1_1};

    MyPanel[1][1]=Ptr[0];
    MyPanel[0][1]=Ptr[1];
    MyPanel[1][0]=Ptr[2];
    MyPanel[0][0]=Ptr[3];
//    MyPanel[0]=palLaserValue_InShuttle1_2;
//    MyPanel[1]=palLaserValue_InShuttle1_1;

    fShow=false;

    bSh1_2Row=false;
    bSh1_1Row=false;
    bSh2_2Row=false;
    bSh2_1Row=false;
    bInArm=false;

    bSetToZeroSh1   =false;
    bSetToZeroSh2   =false;
    bSetToZeroInArm =false;

    int BCC;
    AnsiString Str;
    //Get Value----------------
    Str.sprintf("000000101E1000%d000001", 1);   //COM1, CH1 = Shuttle 1 B排
    BCC=SetBCC(Str);
    GetValMessASCII[0][0].sprintf("%c%s%c%c", STX, Str, ETX, BCC);
    GetValueMessage[0][0].sprintf("SEND: [STX]%s[ETX](%s)", Str, MyDeCodeASCII(BCC));

    Str.sprintf("000000101E1000%d000001", 2);   //COM1, CH2 = Shuttle 2 B排
    BCC=SetBCC(Str);
    GetValMessASCII[0][1].sprintf("%c%s%c%c", STX, Str, ETX, BCC);
    GetValueMessage[0][1].sprintf("SEND: [STX]%s[ETX](%s)", Str, MyDeCodeASCII(BCC));

    Str.sprintf("000000101E1000%d000001", 1);   //COM2, CH1 = Shuttle 1 A排
    BCC=SetBCC(Str);
    GetValMessASCII[1][0].sprintf("%c%s%c%c", STX, Str, ETX, BCC);
    GetValueMessage[1][0].sprintf("SEND: [STX]%s[ETX](%s)", Str, MyDeCodeASCII(BCC));

    Str.sprintf("000000101E1000%d000001", 2);   //COM2, CH2 = Shuttle 2 A排
    BCC=SetBCC(Str);
    GetValMessASCII[1][1].sprintf("%c%s%c%c", STX, Str, ETX, BCC);
    GetValueMessage[1][1].sprintf("SEND: [STX]%s[ETX](%s)", Str, MyDeCodeASCII(BCC));

    Str.sprintf("000000101E1000%d000001", 1);   //COMInArm
    BCC=SetBCC(Str);
    GetValMessASCIIInArm.sprintf("%c%s%c%c", STX, Str, ETX, BCC);
    GetValueMessageInArm.sprintf("SEND: [STX]%s[ETX](%s)", Str, MyDeCodeASCII(BCC));

    //Set to Zero---------------
    Str.sprintf("000003005380%d0000", 1);   //COM1, CH1 = Shuttle 1 B排
    BCC=SetBCC(Str);
    SetToZeroMessASCII[0][0].sprintf("%c%s%c%c", STX, Str, ETX, BCC);
    SetToZeroMessage[0][0].sprintf("SEND: [STX]%s[ETX](%s)", Str, MyDeCodeASCII(BCC));

    Str.sprintf("000003005380%d0000", 2);   //COM1, CH2 = Shuttle 2 B排
    BCC=SetBCC(Str);
    SetToZeroMessASCII[0][1].sprintf("%c%s%c%c", STX, Str, ETX, BCC);
    SetToZeroMessage[0][1].sprintf("SEND: [STX]%s[ETX](%s)", Str, MyDeCodeASCII(BCC));

    Str.sprintf("000003005380%d0000", 1);   //COM2, CH1 = Shuttle 1 A排
    BCC=SetBCC(Str);
    SetToZeroMessASCII[1][0].sprintf("%c%s%c%c", STX, Str, ETX, BCC);
    SetToZeroMessage[1][0].sprintf("SEND: [STX]%s[ETX](%s)", Str, MyDeCodeASCII(BCC));

    Str.sprintf("000003005380%d0000", 2);   //COM2, CH2 = Shuttle 2 A排
    BCC=SetBCC(Str);
    SetToZeroMessASCII[1][1].sprintf("%c%s%c%c", STX, Str, ETX, BCC);
    SetToZeroMessage[1][1].sprintf("SEND: [STX]%s[ETX](%s)", Str, MyDeCodeASCII(BCC));

    Str.sprintf("000003005380%d0000", 1);   //COMInArm
    BCC=SetBCC(Str);
    SetToZeroMessASCIIInArm.sprintf("%c%s%c%c", STX, Str, ETX, BCC);
    SetToZeroMessageInArm.sprintf("SEND: [STX]%s[ETX](%s)", Str, MyDeCodeASCII(BCC));

    if(USE_LASER_DISTANCE)
        btConnect->Click();

    mtPlate2_Golden->SetColorMap(lsNormal,      clWhite);
    mtPlate2_Golden->SetColorMap(lsChecking,    clYellow);
    mtPlate2_Golden->SetColorMap(lsNeedCheck,   clAqua);
    mtPlate2_Golden->SetColorMap(lsFail,        clRed);
    mtPlate1_Golden->SetColorMap(lsNormal,      clWhite);
    mtPlate1_Golden->SetColorMap(lsChecking,    clYellow);
    mtPlate1_Golden->SetColorMap(lsNeedCheck,   clAqua);
    mtPlate1_Golden->SetColorMap(lsFail,        clRed);
    mtPlate2->SetColorMap(lsNormal,      clWhite);
    mtPlate2->SetColorMap(lsChecking,    clYellow);
    mtPlate2->SetColorMap(lsNeedCheck,   clAqua);
    mtPlate2->SetColorMap(lsFail,        clRed);
    mtPlate1->SetColorMap(lsNormal,      clWhite);
    mtPlate1->SetColorMap(lsChecking,    clYellow);
    mtPlate1->SetColorMap(lsNeedCheck,   clAqua);
    mtPlate1->SetColorMap(lsFail,        clRed);

    mtShuttle2_Golden->SetColorMap(lsNormal,      clWhite);
    mtShuttle2_Golden->SetColorMap(lsChecking,    clYellow);
    mtShuttle2_Golden->SetColorMap(lsNeedCheck,   clAqua);
    mtShuttle2_Golden->SetColorMap(lsFail,        clRed);
    mtShuttle1_Golden->SetColorMap(lsNormal,      clWhite);
    mtShuttle1_Golden->SetColorMap(lsChecking,    clYellow);
    mtShuttle1_Golden->SetColorMap(lsNeedCheck,   clAqua);
    mtShuttle1_Golden->SetColorMap(lsFail,        clRed);
    mtShuttle2->SetColorMap(lsNormal,      clWhite);
    mtShuttle2->SetColorMap(lsChecking,    clYellow);
    mtShuttle2->SetColorMap(lsNeedCheck,   clAqua);
    mtShuttle2->SetColorMap(lsFail,        clRed);
    mtShuttle1->SetColorMap(lsNormal,      clWhite);
    mtShuttle1->SetColorMap(lsChecking,    clYellow);
    mtShuttle1->SetColorMap(lsNeedCheck,   clAqua);
    mtShuttle1->SetColorMap(lsFail,        clRed);

    mtOutShuttle2_Golden->SetColorMap(lsNormal,      clWhite);
    mtOutShuttle2_Golden->SetColorMap(lsChecking,    clYellow);
    mtOutShuttle2_Golden->SetColorMap(lsNeedCheck,   clAqua);
    mtOutShuttle2_Golden->SetColorMap(lsFail,        clRed);
    mtOutShuttle1_Golden->SetColorMap(lsNormal,      clWhite);
    mtOutShuttle1_Golden->SetColorMap(lsChecking,    clYellow);
    mtOutShuttle1_Golden->SetColorMap(lsNeedCheck,   clAqua);
    mtOutShuttle1_Golden->SetColorMap(lsFail,        clRed);
    mtOutShuttle2->SetColorMap(lsNormal,      clWhite);
    mtOutShuttle2->SetColorMap(lsChecking,    clYellow);
    mtOutShuttle2->SetColorMap(lsNeedCheck,   clAqua);
    mtOutShuttle2->SetColorMap(lsFail,        clRed);
    mtOutShuttle1->SetColorMap(lsNormal,      clWhite);
    mtOutShuttle1->SetColorMap(lsChecking,    clYellow);
    mtOutShuttle1->SetColorMap(lsNeedCheck,   clAqua);
    mtOutShuttle1->SetColorMap(lsFail,        clRed);
    InitLaserEdtList();
}
//---------------------------------------------------------------------------
void __fastcall TfLaserSensor::FormShow(TObject *Sender)
{
    iCurrentChannel=0;
    iLaserTask=1;
    iCurrentStep=iMaxStep-1;

    if(USE_LASER_DISTANCE==2)
    {
        PageControl1->Visible=false;
    }

    DoIniDataToForm();
    fShow=true;
}
//---------------------------------------------------------------------------
void __fastcall TfLaserSensor::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    DoIniDataToForm();
    fShow=false;
}
//---------------------------------------------------------------------------
void TfLaserSensor::WriteInfoToMemo(AnsiString asInfo)                               //工作訊息寫入memo
{
//    if(bShow==false)
//        return;
    if(CheckBox1->Checked==false)
        return;

    TDateTime myDateTime=Now();

    if(asInfo.AnsiPos("RECV")!=0 || asInfo.AnsiPos("SEND")!=0)
    {
        if(memoLaserSensor->Lines->Count>4096)
            memoLaserSensor->Clear();

        memoLaserSensor->Lines->Add(myDateTime.TimeString()+" "+asInfo);
    }
    else
    {
        if(MemoStatus->Lines->Count>4096)
            MemoStatus->Clear();

        MemoStatus->Lines->Add(myDateTime.TimeString()+" "+asInfo);
    }
}
//---------------------------------------------------------------------------
void TfLaserSensor::ProcessReceiveData(int iCom, AnsiString Data)
{
//------------------------------------------------
//1     2  3  4  5  6  7    8        9    10
//[STX] 00 00 00 01 01 0000 769C2600 [ETX]|
//
// 2 : Node No. (Always 00)
// 3 : Sub-Address (Always 00)
// 4 : End Code
// 5 : MRC
// 6 : SRC
// 7 : Response code
// 8 : Data
//------------------------------------------------

    bool bHasError=false;
    AnsiString EndCode="";
    AnsiString MRC_SRC="";
    AnsiString Response="";
    AnsiString Information="";
    AnsiString Str;
    int iMRC_SRC=0, iStart;

    EndCode=Data.SubString(5, 2);
    MRC_SRC=Data.SubString(7, 4);
    iMRC_SRC=atoi(MRC_SRC.c_str());
    Response=Data.SubString(11, 4);
    Information=Data.SubString(15, Data.Length());

    if(EndCode.AnsiPos(sEndCode[0][rcl0000])==0)
    {
        bHasError=true;
        for(int i=1; i<rclTotalCount; i++)
        {
            iStart=EndCode.Pos(sEndCode[0][i]);
            if(iStart!=0)
            {
                Str.sprintf("MESS%d: End code status: %s", iCom, sEndCode[1][i]);
                WriteInfoToMemo(Str);
            }
        }
    }
    else
    {
        Str.sprintf("MESS%d: End code status: %s", iCom, sEndCode[1][rcl0000]);
        WriteInfoToMemo(Str);
    }

    if(bHasError==false && Response.AnsiPos(sResponseCode[0][ec00])==0)
    {
        bHasError=true;
        for(int i=1; i<rclTotalCount; i++)
        {
            iStart=Response.Pos(sResponseCode[0][i]);
            if(iStart!=0)
            {
                Str.sprintf("MESS%d: Response status: %s", iCom, sResponseCode[1][i]);
                WriteInfoToMemo(Str);
            }
        }
    }
    else
    {
        Str.sprintf("MESS%d: Response status: %s", iCom, sResponseCode[1][ec00]);
        WriteInfoToMemo(Str);
    }

    if(bHasError==false)
    {
        switch(iMRC_SRC)
        {
            case 101:
                ProcessReceiveData0101(iCom, Information);
                break;
            case 201:
                Str.sprintf("INFO%d: 0201: %s", iCom, Information);
                WriteInfoToMemo(Str);
                break;
            case 202:
                Str.sprintf("INFO%d: 0202: %s", iCom, Information);
                WriteInfoToMemo(Str);
                break;
            case 501:
                Str.sprintf("INFO%d: 0501: %s", iCom, Information);
                WriteInfoToMemo(Str);
                break;
            case 503:
                Str.sprintf("INFO%d: 0503: %s", iCom, Information);
                WriteInfoToMemo(Str);
                break;
            case 601:
                Str.sprintf("INFO%d: 0601: %s", iCom, Information);
                WriteInfoToMemo(Str);
                break;
            case 801:
                Str.sprintf("INFO%d: 0801: %s", iCom, Information);
                WriteInfoToMemo(Str);
                break;
            case 3005:
                Str.sprintf("INFO%d: 3005: %s", iCom, Information);
                WriteInfoToMemo(Str);
                break;
            default:
                Str.sprintf("INFO%d: unknown MRS SRC: %s", iCom, MRC_SRC);
                WriteInfoToMemo(Str);
        }
    }
}
//---------------------------------------------------------------------------
void TfLaserSensor::ProcessReceiveData0101(int iCom, AnsiString Data)
{
    AnsiString Value1, Value2, Str;
    double dValue1;
    int iValue2;

    if(Data.Length()==8)
    {
        Value1=Data.SubString(1, 4);
        Value2=Data.SubString(5, 2);

        if(iCom==2 && cbLD40L->Checked==false)
            dValue1=(double)ConvertByte(Value1.c_str())/100.0;
        else
            dValue1=(double)ConvertByte(Value1.c_str())/1000.0;
        iValue2=atoi(Value2.c_str());

        if(iValue2%2==1)
        {
            dValue1=0.0-dValue1;
        }

        Value2.sprintf("%0.3f", dValue1);

        if(bSh2_2Row || bSh2_1Row || bSh1_2Row || bSh1_1Row)
        {
            if(iCom==2)
            {
                palLaserValue_InArm->Caption=Value2;
            }
            else
            {
                MyPanel[iCom][iCurrentChannelSht]->Caption=Value2;
            }
        }
        else
        {
            if(iCom==2)
            {
                palLaserValue_InArm->Caption=Value2;
            }
            else
            {
                MyPanel[iCom][iCurrentChannel]->Caption=Value2;
            }
        }

        Str.sprintf("INFO%d: 0101: Value : %f", iCom, dValue1);
    }
    else
    {
        Str.sprintf("INFO: 0101: %s", Data);
    }
    WriteInfoToMemo(Str);
}
//---------------------------------------------------------------------------
void __fastcall TfLaserSensor::CommLaser1ReceiveData(TObject *Sender,
      Pointer Buffer, WORD BufferLength)
{
    AnsiString ComOmronBuffer="RECV: ";
    AnsiString ComOmronBuffer2="";

    AnsiString S="";
    byte *data;
    bool bHasETX=false;

    data=(byte *)Buffer;

    for(int i=0; i<BufferLength; i++)
    {
        if(MyDeCodeASCII(data[i])=="ETX")
        {
            ComOmronBuffer+="[ETX]";
            ComOmronBuffer+=MyDeCodeASCII(data[i+1]);
            i++;

            if(i+1<BufferLength)
            {
                if(data[i+1]=='\r')
                {
                    ComOmronBuffer+=MyDeCodeASCII(data[i+1]);
                    i++;
                }

                if(data[i+1]=='\n')
                {
                    ComOmronBuffer+=MyDeCodeASCII(data[i+1]);
                    i++;
                }
            }

            bHasETX=true;
        }
        else if(MyDeCodeASCII(data[i])=="STX")
        {
            ComOmronBuffer+="[STX]";
        }
        else if(MyDeCodeASCII(data[i])=='\0')
        {
            ;
        }
        else
        {
            ComOmronBuffer+=MyDeCodeASCII(data[i]);
            ComOmronBuffer2+=MyDeCodeASCII(data[i]);
        }

        if(bHasETX==true)
        {
            bHasETX=false;
            ProcessReceiveData(0, ComOmronBuffer2);
            ComOmronBuffer2="";
        }
    }
    WriteInfoToMemo("COMLaser1 : "+ComOmronBuffer);
    bRecvData1=true;
}
//---------------------------------------------------------------------------
TQPF_Timer COMLaser1Delay;
TQPF_Timer COMLaser2Delay;
TQPF_Timer COMLaserInDelay;
TQPF_Timer COMLaserOutDelay;
//---------------------------------------------------------------------------
void __fastcall TfLaserSensor::btSetToZeroClick(TObject *Sender)
{
    iCurrentChannel=0;
    iLaserTask=1;
    bSetToZero=true;
}
//---------------------------------------------------------------------------
void __fastcall TfLaserSensor::CommLaser2ReceiveData(TObject *Sender,
      Pointer Buffer, WORD BufferLength)
{
    AnsiString ComOmronBuffer="RECV: ";
    AnsiString ComOmronBuffer2="";

    AnsiString S="";
    byte *data;
    bool bHasETX=false;

    data=(byte *)Buffer;

    for(int i=0; i<BufferLength; i++)
    {
        if(MyDeCodeASCII(data[i])=="ETX")
        {
            ComOmronBuffer+="[ETX]";
            ComOmronBuffer+=MyDeCodeASCII(data[i+1]);
            i++;

            if(i+1<BufferLength)
            {
                if(data[i+1]=='\r')
                {
                    ComOmronBuffer+=MyDeCodeASCII(data[i+1]);
                    i++;
                }

                if(data[i+1]=='\n')
                {
                    ComOmronBuffer+=MyDeCodeASCII(data[i+1]);
                    i++;
                }
            }

            bHasETX=true;
        }
        else if(MyDeCodeASCII(data[i])=="STX")
        {
            ComOmronBuffer+="[STX]";
        }
        else if(MyDeCodeASCII(data[i])=='\0')
        {
            ;
        }
        else
        {
            ComOmronBuffer+=MyDeCodeASCII(data[i]);
            ComOmronBuffer2+=MyDeCodeASCII(data[i]);
        }

        if(bHasETX==true)
        {
            bHasETX=false;
            ProcessReceiveData(1, ComOmronBuffer2);
            ComOmronBuffer2="";
        }
    }
    WriteInfoToMemo("COMLaser2 : "+ComOmronBuffer);
    bRecvData2=true;
}
//---------------------------------------------------------------------------
void __fastcall TfLaserSensor::cbStartPoolingClick(TObject *Sender)
{
    if(cbStartPooling->Checked==true)
    {
        iCurrentChannel=0;
        iLaserTask=1;
        iCurrentStep=iMaxStep-1;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLaserSensor::btGetValueSht1_2RowClick(TObject *Sender)
{
    iLaser1Task=1;
    bSh1_2Row=true;
}
//---------------------------------------------------------------------------
void __fastcall TfLaserSensor::btGetValueSht1_1RowClick(TObject *Sender)
{
    iLaser1Task=1;
    bSh1_1Row=true;
}
//---------------------------------------------------------------------------
void __fastcall TfLaserSensor::btGetValueSht2_2RowClick(TObject *Sender)
{
    iLaser2Task=1;
    bSh2_2Row=true;
}
//---------------------------------------------------------------------------
void __fastcall TfLaserSensor::btGetValueSht2_1RowClick(TObject *Sender)
{
    iLaser2Task=1;
    bSh2_1Row=true;
}
//---------------------------------------------------------------------------
extern bool GetCOMPortStatus(AnsiString Com);    //Steven 20120217 : Com Port改成可定義
void __fastcall TfLaserSensor::btConnectClick(TObject *Sender)
{
    static bool bConnect=false;

    if(bConnect==false)
    {
        btConnect->Caption="To Disconnect";
        bConnect=true;
        if(USE_LASER_DISTANCE!=2)
        {
            if(GetCOMPortStatus(HSys.asLASER_COM[0]))
            {
                CommLaser1->CommName="\\\\.\\"+HSys.asLASER_COM[0];
                try
                {
                    CommLaser1->Parity=Even;
                    CommLaser1->BaudRate=38400;
                    CommLaser1->ByteSize=_8;
                    CommLaser1->ParityCheck=false;
                    CommLaser1->StopBits=_1;
                    CommLaser1->StartComm();  //僅能啟動一次
                    WriteInfoToMemo("Start COM Laser 1 OK");
                }
                catch(...)
                {
                    MyDBIProcess("Exception", "btConnectClick");
                    WriteInfoToMemo("Start COM Laser 1 Fail");
                    ShowMyMessage("Start COM Laser 1 Fail");
                }
            }

            if(GetCOMPortStatus(HSys.asLASER_COM[1]))
            {
                CommLaser2->CommName="\\\\.\\"+HSys.asLASER_COM[1];
                try
                {
                    CommLaser2->Parity=Even;
                    CommLaser2->BaudRate=38400;
                    CommLaser2->ByteSize=_8;
                    CommLaser2->ParityCheck=false;
                    CommLaser2->StopBits=_1;
                    CommLaser2->StartComm();  //僅能啟動一次
                    WriteInfoToMemo("Start COM Laser 2 OK");
                }
                catch(...)
                {
                    MyDBIProcess("Exception", "btConnectClick");
                    WriteInfoToMemo("Start COM Laser 2 Fail");
                    ShowMyMessage("Start COM Laser 2 Fail");
                }
            }
        }
        else
        {
            if(GetCOMPortStatus(HSys.asLASER_COM[3]))
            {
                CommLaserOutArm->CommName="\\\\.\\"+HSys.asLASER_COM[3];
                try
                {
                    CommLaserOutArm->Parity=None;
                    CommLaserOutArm->BaudRate=38400;
                    CommLaserOutArm->ByteSize=_8;
                    CommLaserOutArm->ParityCheck=false;
                    CommLaserOutArm->StopBits=_1;
                    CommLaserOutArm->StartComm();  //僅能啟動一次
                    WriteInfoToMemo("Start COM Laser Out Arm OK");
                }
                catch(...)
                {
                    MyDBIProcess("Exception", "btConnectClick");
                    WriteInfoToMemo("Start COM Laser Out Arm Fail");
                    ShowMyMessage("Start COM Laser Out Arm Fail");
                }
            }
        }

        if(GetCOMPortStatus(HSys.asLASER_COM[2]))
        {
            CommLaserInArm->CommName="\\\\.\\"+HSys.asLASER_COM[2];
            try
            {
                CommLaserInArm->Parity=Even;
                CommLaserInArm->BaudRate=38400;
                CommLaserInArm->ByteSize=_8;
                CommLaserInArm->ParityCheck=false;
                CommLaserInArm->StopBits=_1;
                CommLaserInArm->StartComm();  //僅能啟動一次
                WriteInfoToMemo("Start COM Laser In Arm OK");
            }
            catch(...)
            {
                MyDBIProcess("Exception", "btConnectClick");
                WriteInfoToMemo("Start COM Laser In Arm Fail");
                ShowMyMessage("Start COM Laser In Arm Fail");
            }
        }
    }
    else
    {
        btConnect->Caption="To Connect";
        bConnect=false;
        CommLaser1->StopComm();
        CommLaser2->StopComm();
        CommLaserInArm->StopComm();
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLaserSensor::CommLaserInArmReceiveData(TObject *Sender,
      Pointer Buffer, WORD BufferLength)
{
    AnsiString ComOmronBuffer="RECV: ";
    AnsiString ComOmronBuffer2="";

    AnsiString S="";
    byte *data;
    bool bHasETX=false;

    data=(byte *)Buffer;

    for(int i=0; i<BufferLength; i++)
    {
        if(MyDeCodeASCII(data[i])=="ETX")
        {
            ComOmronBuffer+="[ETX]";
            ComOmronBuffer+=MyDeCodeASCII(data[i+1]);
            i++;

            if(i+1<BufferLength)
            {
                if(data[i+1]=='\r')
                {
                    ComOmronBuffer+=MyDeCodeASCII(data[i+1]);
                    i++;
                }

                if(data[i+1]=='\n')
                {
                    ComOmronBuffer+=MyDeCodeASCII(data[i+1]);
                    i++;
                }
            }

            bHasETX=true;
        }
        else if(MyDeCodeASCII(data[i])=="STX")
        {
            ComOmronBuffer+="[STX]";
        }
        else if(MyDeCodeASCII(data[i])=='\0')
        {
            ;
        }
        else
        {
            if(USE_LASER_DISTANCE==2)
            {
                asLaserValue[0]+=MyDeCodeASCII(data[i]);
            }

            ComOmronBuffer+=MyDeCodeASCII(data[i]);
            ComOmronBuffer2+=MyDeCodeASCII(data[i]);
        }

        if(bHasETX==true)
        {
            bHasETX=false;
            ProcessReceiveData(2, ComOmronBuffer2);
            ComOmronBuffer2="";
        }
    }
    WriteInfoToMemo("COMLaserIn: "+ComOmronBuffer);
    bRecvDataIn=true;
}
//---------------------------------------------------------------------------
void __fastcall TfLaserSensor::btGetValueInArmClick(TObject *Sender)
{
    iLaserInArmTask=1;
    bInArm=true;
}
//---------------------------------------------------------------------------
void __fastcall TfLaserSensor::btSetToZeroSh2Click(TObject *Sender)
{
    bSetToZeroSh2=true;
}
//---------------------------------------------------------------------------
void __fastcall TfLaserSensor::btSetToZeroSh1Click(TObject *Sender)
{
    bSetToZeroSh1=true;
}
//---------------------------------------------------------------------------
void __fastcall TfLaserSensor::btSetToZeroInArmClick(TObject *Sender)
{
    bSetToZeroInArm=true;
}
//---------------------------------------------------------------------------
void __fastcall TfLaserSensor::Timer1Timer(TObject *Sender)
{
    static bool bRun=false;

    if(InitialOK==false || USE_LASER_DISTANCE==0)
    {
        bRun=false;
        return;
    }

    if(TestIF_File.bEnableShuttleLaser==false && TestIF_File.bEnableInArmLaser==false &&
       TestIF_File.bEnableInShuttleFloatingCheck==false && TestIF_File.bEnableOutShuttleFloatingCheck==false) //Eastsun 20260522 整合 : USE_LASER_DISTANCE==2 條件
    {
        bRun=false;
        return;
    }

    if(bRun==false)
    {
        bRun=true;

        if(bSetToZeroSh1)
        {
            switch(iLaser1Task)
            {
                case 1:
                    bRecvData1=false;
                    bRecvData2=false;
                    CommLaser1->WriteCommData(SetToZeroMessASCII[0][0].c_str(), SetToZeroMessASCII[0][0].Length()+1);
                    CommLaser2->WriteCommData(SetToZeroMessASCII[1][0].c_str(), SetToZeroMessASCII[1][0].Length()+1);
                    WriteInfoToMemo(SetToZeroMessage[0][0]);
                    WriteInfoToMemo(SetToZeroMessage[1][0]);
                    COMLaser1Delay.SetSecAndOn(1);
                    iLaser1Task=2;
//                    break;
                case 2:
                    #ifdef SOFT_SIMULTE
                    bRecvData1=true;
                    bRecvData2=true;
                    #endif
                    if(bRecvData1 && bRecvData2)
                    {
                        bRecvData1=false;
                        bRecvData2=false;
                        iLaser1Task=1;
                        bSetToZeroSh1=false;
                        WriteToMemo(MemoSh1, "//--------------");
                        WriteToMemo(MemoSh1, "In Arm Set to Zero.");
                    }
                    else if(COMLaser1Delay.Off())
                    {
                        iLaser1Task=1;
                        bSetToZeroSh1=false;
                    }
                    break;
            }
        }
        else if(bSetToZeroSh2)
        {
            switch(iLaser2Task)
            {
                case 1:
                    bRecvData1=false;
                    bRecvData2=false;
                    CommLaser1->WriteCommData(SetToZeroMessASCII[0][1].c_str(), SetToZeroMessASCII[0][1].Length()+1);
                    CommLaser2->WriteCommData(SetToZeroMessASCII[1][1].c_str(), SetToZeroMessASCII[1][1].Length()+1);
                    WriteInfoToMemo(SetToZeroMessage[0][1]);
                    WriteInfoToMemo(SetToZeroMessage[1][1]);
                    COMLaser2Delay.SetSecAndOn(1);
                    iLaser2Task=2;
//                    break;
                case 2:
                    #ifdef SOFT_SIMULTE
                    bRecvData1=true;
                    bRecvData2=true;
                    #endif

                    if(bRecvData1 && bRecvData2)
                    {
                        bRecvData1=false;
                        bRecvData2=false;
                        iLaser2Task=1;
                        bSetToZeroSh2=false;
                        WriteToMemo(MemoSh2, "//--------------");
                        WriteToMemo(MemoSh2, "In Arm Set to Zero.");
                    }
                    else if(COMLaser2Delay.Off())
                    {
                        iLaser2Task=1;
                    }
                    break;
            }
        }
        else if(bSh1_2Row)
        {
            switch(iLaser1Task)
            {
                case 1:
                    bRecvData1=false;
                    bRecvData2=false;
                    iCurrentChannelSht=0;
                    CommLaser1->WriteCommData(GetValMessASCII[0][iCurrentChannelSht].c_str(), GetValMessASCII[0][iCurrentChannelSht].Length()+1);
                    CommLaser2->WriteCommData(GetValMessASCII[1][iCurrentChannelSht].c_str(), GetValMessASCII[1][iCurrentChannelSht].Length()+1);
                    WriteInfoToMemo(GetValueMessage[0][iCurrentChannelSht]);
                    WriteInfoToMemo(GetValueMessage[1][iCurrentChannelSht]);
                    COMLaser1Delay.SetSecAndOn(1);
                    iLaser1Task=2;
//                    break;
                case 2:
                    #ifdef SOFT_SIMULTE
                    bRecvData1=true;
                    bRecvData2=true;
                    #endif

                    if(bRecvData1 && bRecvData2)
                    {
                        bRecvData1=false;
                        bRecvData2=false;

                        iCurrentChannelSht=0;
                        iCurrentStep=iMaxStep-1;
                        bSh1_2Row=false;
                        iLaser1Task=1;
                    }
                    else if(COMLaser1Delay.Off())
                    {
                        iLaser1Task=1;
                    }
                    break;
            }
        }
        else if(bSh1_1Row)
        {
            switch(iLaser1Task)
            {
                case 1:
                    bRecvData2=false;
                    iCurrentChannelSht=0;
                    CommLaser2->WriteCommData(GetValMessASCII[1][iCurrentChannelSht].c_str(), GetValMessASCII[1][iCurrentChannelSht].Length()+1);
                    WriteInfoToMemo(GetValueMessage[1][iCurrentChannelSht]);
                    iLaser1Task=2;
                    COMLaser1Delay.SetSecAndOn(1);
//                    break;
                case 2:
                    #ifdef SOFT_SIMULTE
                    bRecvData2=true;
                    #endif

                    if(bRecvData2)
                    {
                        bRecvData1=false;
                        bRecvData2=false;

                        iLaser1Task=1;
                        iCurrentChannelSht=0;
                        iCurrentStep=iMaxStep-1;
                        bSh1_1Row=false;
                    }
                    else if(COMLaser1Delay.Off())
                    {
                        iLaser1Task=1;
                    }
                    break;
            }
        }
        else if(bSh2_2Row)
        {
            switch(iLaser2Task)
            {
                case 1:
                    bRecvData1=false;
                    bRecvData2=false;
                    iCurrentChannelSht=1;
                    CommLaser1->WriteCommData(GetValMessASCII[0][iCurrentChannelSht].c_str(), GetValMessASCII[0][iCurrentChannelSht].Length()+1);
                    CommLaser2->WriteCommData(GetValMessASCII[1][iCurrentChannelSht].c_str(), GetValMessASCII[1][iCurrentChannelSht].Length()+1);
                    WriteInfoToMemo(GetValueMessage[0][iCurrentChannelSht]);
                    WriteInfoToMemo(GetValueMessage[1][iCurrentChannelSht]);
                    COMLaser2Delay.SetSecAndOn(1);
                    iLaser2Task=2;
//                    break;
                case 2:
                    #ifdef SOFT_SIMULTE
                    bRecvData1=true;
                    bRecvData2=true;
                    #endif

                    if(bRecvData1 && bRecvData2)
                    {
                        bRecvData1=false;
                        bRecvData2=false;

                        iCurrentChannelSht=1;
                        iCurrentStep=iMaxStep-1;
                        iLaser2Task=1;
                        bSh2_2Row=false;
                    }
                    else if(COMLaser2Delay.Off())
                    {
                        iLaser2Task=1;
                    }
                    break;
            }
        }
        else if(bSh2_1Row)
        {
            switch(iLaser2Task)
            {
                case 1:
                    bRecvData1=false;
                    iCurrentChannelSht=1;
                    CommLaser1->WriteCommData(GetValMessASCII[0][iCurrentChannelSht].c_str(), GetValMessASCII[0][iCurrentChannelSht].Length()+1);
                    WriteInfoToMemo(GetValueMessage[0][iCurrentChannelSht]);
                    COMLaser2Delay.SetSecAndOn(1);
                    iLaser2Task=2;
//                    break;
                case 2:
                    #ifdef SOFT_SIMULTE
                    bRecvData1=true;
                    #endif

                    if(bRecvData1)
                    {
                        bRecvData1=false;
                        bRecvData2=false;

                        iCurrentChannelSht=1;
                        iCurrentStep=iMaxStep-1;
                        iLaser2Task=1;
                        bSh2_1Row=false;
                    }
                    else if(COMLaser2Delay.Off())
                    {
                        iLaser2Task=1;
                    }
                    break;
            }
        }
        else if(fShow && cbStartPooling->Checked && SystemStart==false && cbEnableShuttleLaser->Checked==true)
        {
            switch(iLaserTask)
            {
                case 1:
                    bRecvData1=false;
                    bRecvData2=false;
                    CommLaser1->WriteCommData(GetValMessASCII[0][iCurrentChannel].c_str(), GetValMessASCII[0][iCurrentChannel].Length()+1);
                    CommLaser2->WriteCommData(GetValMessASCII[1][iCurrentChannel].c_str(), GetValMessASCII[1][iCurrentChannel].Length()+1);
                    WriteInfoToMemo(GetValueMessage[0][iCurrentChannel]);
                    WriteInfoToMemo(GetValueMessage[1][iCurrentChannel]);
                    iLaserTask=2;
                    COMLaser1Delay.SetSecAndOn(1);
//                    break;
                case 2:
                    #ifdef SOFT_SIMULTE
                    bRecvData1=true;
                    bRecvData2=true;
                    #endif

                    if(bRecvData1 && bRecvData2)
                    {
                        iCurrentChannel++;
                        if(iCurrentChannel<iMaxChannel)
                        {
                            iLaserTask=1;
                        }
                        else
                        {
                            iCurrentStep--;
                            if(iCurrentStep>=0)
                            {
                                iCurrentChannel=0;
                                iLaserTask=1;
                            }
                            else
                            {
                                bRecvData1=false;
                                bRecvData2=false;

                                iCurrentChannel=0;
                                iLaserTask=1;
                                iCurrentStep=iMaxStep-1;
                            }
                        }
                    }
                    else if(COMLaser1Delay.Off())
                    {
                        iLaserTask=1;
                    }
                    break;
            }
        }
        else
        {
            iCurrentChannel=0;
            iLaserTask=1;
            iCurrentStep=iMaxStep-1;
        }
        bRun=false;
    }
}
//---------------------------------------------------------------------------
void TfLaserSensor::InitLaserEdtList()
{
    bool bShow=true, bEnable=true, bReadFromFile=true, bFixedValue=false, bDisable=false;

    if(USE_LASER_DISTANCE==1)
    {
        elLaser->Add(cbEnableShuttleLaser,     &TestIF_File.bEnableShuttleLaser,       ECBool,     "Laser", "Enable Shuttle Floating Check", bShow, bEnable, bReadFromFile, 0);
        elLaser->Add(cbEnableInArmLaser,       &TestIF_File.bEnableInArmLaser,         ECBool,     "Laser", "Enable In Arm Floating Check",  bShow, bEnable, bReadFromFile, 0);
        elLaser->Add(edICThickness,            &TestIF_File.dIcThickness,              ECDouble,   "Laser", "Device Thickness",            bShow, bEnable, bReadFromFile, 1.0,    false,  0.01,   5.0);
        elLaser->Add(edLaserShtThreshold,      &TestIF_File.dLaserThresholdSht,        ECDouble,   "Laser", "Laser Threshold Shuttle",     bShow, bEnable, bReadFromFile, 0.0,    false,  5.0,   -5.0);
        elLaser->Add(edLaserHPThreshold,       &TestIF_File.dLaserThresholdHP,         ECDouble,   "Laser", "Laser Threshold Hot Plate",   bShow, bEnable, bReadFromFile, 0.0,    false,  5.0,   -5.0);
    }
    else if(USE_LASER_DISTANCE==2)
    {
        elLaser->Add(cbEnableInShuttleFloatingCheck,  &TestIF_File.bEnableInShuttleFloatingCheck,   ECBool,     "Laser", "Enable In Shuttle Floating Check",    bShow, bEnable, bReadFromFile, 0);
        elLaser->Add(cbEnableOutShuttleFloatingCheck, &TestIF_File.bEnableOutShuttleFloatingCheck,  ECBool,     "Laser", "Enable Out Shuttle Floating Check",   bShow, bEnable, bReadFromFile, 0);

        elLaser->Add(edInArmDeviceFloatingCheckRange, &TestIF_File.dInLaserThresholdSht,            ECDouble,   "Laser", "Laser Threshold In Shuttle",          bShow, bEnable, bReadFromFile, 0.0,    false,  5.0,   -5.0);
        elLaser->Add(edOutArmDeviceFloatingCheckRange,&TestIF_File.dOutLaserThresholdSht,           ECDouble,   "Laser", "Laser Threshold Out Shuttle",         bShow, bEnable, bReadFromFile, 0.0,    false,  5.0,   -5.0);

        elLaser->Add(edDeviceXOffset,          &TestIF_File.iDeviceXOffset,            ECInteger,   "Laser", "In Device X Offset",     bShow, bEnable, bReadFromFile, 0,    false,  120,   -120);
        elLaser->Add(edDeviceYOffset,          &TestIF_File.iDeviceYOffset,            ECInteger,   "Laser", "In Device Y Offset",     bShow, bEnable, bReadFromFile, 0,    false,  120,   -120);
        elLaser->Add(edOutDeviceXOffset,       &TestIF_File.iOutDeviceXOffset,         ECInteger,   "Laser", "Out Device X Offset",    bShow, bEnable, bReadFromFile, 0,    false,  120,   -120);
        elLaser->Add(edOutDeviceYOffset,       &TestIF_File.iOutDeviceYOffset,         ECInteger,   "Laser", "Out Device Y Offset",    bShow, bEnable, bReadFromFile, 0,    false,  120,   -120);
    }
    else
    {
        elLaser->Add(cbEnableShuttleLaser,     &TestIF_File.bEnableShuttleLaser,       ECBool,     "Laser", "Enable Shuttle Floating Check", bShow, bDisable, bFixedValue, 0);
        elLaser->Add(cbEnableInArmLaser,       &TestIF_File.bEnableInArmLaser,         ECBool,     "Laser", "Enable In Arm Floating Check",  bShow, bDisable, bFixedValue, 0);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLaserSensor::sbUpdateClick(TObject *Sender)
{
    AnsiString S="";
    AnsiString szDir="";

    szDir.sprintf("%s%s\\", DataPath, GetLastOpenFN());
    MyForceDirectories(szDir);
    elLaser->SaveEditTextToFile(szDir, "HandlerCondition.Data");

    for(int iInout=0; iInout<2; iInout++)                                       //KenHsieh 20260116 : 補上存檔
    {
        for(int iSH=0; iSH<2; iSH++)
            SaveShuttleLaserValue(iSH, iInout);
    }

    ReadFile();
    sbUpdate->Down=false;
    fMain->BackupSetupFile();   //Ifor 20170620 (wei) add Auto BackUp Setup File & Last Data
}
//---------------------------------------------------------------------------
void __fastcall TfLaserSensor::SaveShuttleLaserValue(int iSht, int iInOutSht)  //Eastsun 20260525 laser 整合 : iInOutSht 0=InSht 1=OutSht
{
    AnsiString S="";
    AnsiString szDir="";

    szDir.sprintf("%s%s", DataPath, GetLastOpenFN());
    MyForceDirectories(szDir);
    szDir+="\\HandlerCondition.Data";
    for(int i=0; i<MAX_Index_Row; i++)
    {
        for(int j=0; j<MAX_Index_Col; j++)
        {
            if(iInOutSht==0)
            {
                S.sprintf("Laser Value Shuttle%d %02d%02d", iSht+1, i, j);
                WriteIniData(szDir, "Laser", S, TestIF_File.dLaserValue[iSht][i][j]);   //Steven 20140228 : 雷射測距功能 Golden數值
            }
            else
            {
                S.sprintf("Laser Value Out Shuttle%d %02d%02d", iSht+1, i, j);
                WriteIniData(szDir, "Laser", S, TestIF_File.dLaserValueOutSh[iSht][i][j]);
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLaserSensor::SaveInArmLaserValue()
{
    AnsiString S="";
    AnsiString szDir="";

    szDir.sprintf("%s%s", DataPath, GetLastOpenFN());
    MyForceDirectories(szDir);
    szDir+="\\HandlerCondition.Data";
    for(int k=0; k<2; k++)
    {
        for(int i=0; i<HotPlateForm.XDivision; i++)
        {
            for(int j=0; j<HotPlateForm.YDivision; j++)
            {
                S.sprintf("Laser Value Plate%d %02d%02d", k+1, i, j);
                WriteIniData(szDir, "Laser", S, TestIF_File.dLaserValueIn[k][i][j]);   //Steven 20140228 : 雷射測距功能 Golden數值
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLaserSensor::ReadLaserFile()     //只有換工作檔才讀一次
{
    if(USE_LASER_DISTANCE==0)
        return;

    AnsiString S="";
    AnsiString szDir="";

    szDir.sprintf("%s%s", DataPath, GetLastOpenFN());
    MyForceDirectories(szDir);
    szDir+="\\HandlerCondition.Data";
    for(int k=0; k<2; k++)
    {
        for(int i=0; i<HotPlateForm.XDivision; i++)
        {
            for(int j=0; j<HotPlateForm.YDivision; j++)
            {
                S.sprintf("Laser Value Plate%d %02d%02d", k+1, i, j);
                TestIF_File.dLaserValueIn[k][i][j]=CheckAndReadIniData(szDir, "Laser", S, 0.0);   //Steven 20140228 : 雷射測距功能 Golden數值
            }
        }
    }

    for(int i=0; i<MAX_Index_Row; i++)
    {
        for(int j=0; j<MAX_Index_Col; j++)
        {
            S.sprintf("Laser Value Shuttle%d %02d%02d", 1, i, j);
            WriteIniData(szDir, "Laser", S, TestIF_File.dLaserValue[0][i][j]);   //Steven 20140228 : 雷射測距功能 Golden數值

            S.sprintf("Laser Value Shuttle%d %02d%02d", 2, i, j);
            WriteIniData(szDir, "Laser", S, TestIF_File.dLaserValue[1][i][j]);   //Steven 20140228 : 雷射測距功能 Golden數值

            S.sprintf("Laser Value Out Shuttle%d %02d%02d", 1, i, j);
            WriteIniData(szDir, "Laser", S, TestIF_File.dLaserValueOutSh[0][i][j]);   //Steven 20140228 : 雷射測距功能 Golden數值

            S.sprintf("Laser Value Out Shuttle%d %02d%02d", 2, i, j);
            WriteIniData(szDir, "Laser", S, TestIF_File.dLaserValueOutSh[1][i][j]);   //Steven 20140228 : 雷射測距功能 Golden數值
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLaserSensor::ReadFile()
{
    AnsiString S="";
    AnsiString szDir="";

    szDir.sprintf("%s%s\\", DataPath, GetLastOpenFN());
    MyForceDirectories(szDir);
    elLaser->ReadEditTextFromFile(szDir, "HandlerCondition.Data");
    szDir+="HandlerCondition.Data";

    if(USE_LASER_DISTANCE==0)
        return;

    for(int i=0; i<MAX_Index_Row; i++)
    {
        for(int j=0; j<MAX_Index_Col; j++)
        {
            S.sprintf("Laser Value Shuttle1 %02d%02d", i, j);
            TestIF_File.dLaserValue[0][i][j]=CheckAndReadIniData(szDir, "Laser", S, 0.0);   //Steven 20140228 : 雷射測距功能 Golden數值
            S.sprintf("Laser Value Shuttle2 %02d%02d", i, j);
            TestIF_File.dLaserValue[1][i][j]=CheckAndReadIniData(szDir, "Laser", S, 0.0);   //Steven 20140228 : 雷射測距功能 Golden數值

            S.sprintf("Laser Value Out Shuttle1 %02d%02d", i, j);
            TestIF_File.dLaserValueOutSh[0][i][j]=CheckAndReadIniData(szDir, "Laser", S, 0.0);   //Steven 20140228 : 雷射測距功能 Golden數值
            S.sprintf("Laser Value Out Shuttle2 %02d%02d", i, j);
            TestIF_File.dLaserValueOutSh[1][i][j]=CheckAndReadIniData(szDir, "Laser", S, 0.0);   //Steven 20140228 : 雷射測距功能 Golden數值
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLaserSensor::DoIniDataToForm()
{
    ReadFile();
    elLaser->InitialDataToEdit();

    for(int iX=0; iX<mtPlate1_Golden->XItem; iX++)
    {
        for(int iY=0; iY<mtPlate1_Golden->YItem; iY++)
        {
            mtPlate1_Golden->SetCellNumber(iX, iY, TestIF_File.dLaserValueIn[1][iX][iY]);
            mtPlate2_Golden->SetCellNumber(iX, iY, TestIF_File.dLaserValueIn[0][iX][iY]);
        }
    }

    for(int iX=0; iX<mtShuttle1_Golden->XItem; iX++)
    {
        for(int iY=0; iY<mtShuttle1_Golden->YItem; iY++)
        {
            if(USE_LASER_DISTANCE==2)
            {
                mtShuttle1_Golden->SetCellNumber(iX, iY, TestIF_File.dLaserValue[0][iY][iX]);
                mtShuttle2_Golden->SetCellNumber(iX, iY, TestIF_File.dLaserValue[1][iY][iX]);

                mtOutShuttle1_Golden->SetCellNumber(iX, iY, TestIF_File.dLaserValueOutSh[0][iY][iX]);
                mtOutShuttle2_Golden->SetCellNumber(iX, iY, TestIF_File.dLaserValueOutSh[1][iY][iX]);
            }
            else
            {
                mtShuttle1_Golden->SetCellNumber(iX, iY, TestIF_File.dLaserValue[0][iX][iY]);
                mtShuttle2_Golden->SetCellNumber(iX, iY, TestIF_File.dLaserValue[1][iX][iY]);
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLaserSensor::edICThicknessMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_DOUBLE, 2, true, 5.0, -5.0);
}
//---------------------------------------------------------------------------
void __fastcall TfLaserSensor::sbtExitClick(TObject *Sender)
{
    Close();
    sbtExit->Down=false;
}
//---------------------------------------------------------------------------
void __fastcall TfLaserSensor::btSaveLogClick(TObject *Sender)
{
    AnsiString sFileName, Str1, Str2;
    sFileName.sprintf("%s\\%04d%02d\\", asLaserLogPath, SystemYear, SystemMonth);
    MyForceDirectories(sFileName);

    sFileName.sprintf("%s\\%04d%02d\\%04d%02d%02d%02d%02d%02d_SHT1.logs", asLaserLogPath, SystemYear, SystemMonth, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
    MemoSh1->Lines->SaveToFile(sFileName);
    MemoSh1->Clear();
    sFileName.sprintf("%s\\%04d%02d\\%04d%02d%02d%02d%02d%02d_SHT2.logs", asLaserLogPath, SystemYear, SystemMonth, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
    MemoSh2->Lines->SaveToFile(sFileName);
    MemoSh2->Clear();

    sFileName.sprintf("%s\\%04d%02d\\%04d%02d%02d%02d%02d%02d_HP1.logs", asLaserLogPath, SystemYear, SystemMonth, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
    mtPlate1->SaveCellTextToFile(sFileName);
    sFileName.sprintf("%s\\%04d%02d\\%04d%02d%02d%02d%02d%02d_HP2.logs", asLaserLogPath, SystemYear, SystemMonth, SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec);
    mtPlate2->SaveCellTextToFile(sFileName);
}
//---------------------------------------------------------------------------
void __fastcall TfLaserSensor::WriteToMemo(TMemo *Memo, AnsiString Str)
{
    if(Memo->Lines->Count>4096)
        Memo->Clear();

    Memo->Lines->Add(Str);
}
//---------------------------------------------------------------------------
void __fastcall TfLaserSensor::TimerInArmTimer(TObject *Sender)
{
    static bool bRun=false;
    static int iCount=0;

    if(InitialOK==false || USE_LASER_DISTANCE==0)
    {
        bRun=false;
        return;
    }

    if(TestIF_File.bEnableShuttleLaser==false && TestIF_File.bEnableInArmLaser==false &&
      TestIF_File.bEnableInShuttleFloatingCheck==false && TestIF_File.bEnableOutShuttleFloatingCheck==false)
    {
        bRun=false;
        return;
    }

    if(bRun==false)
    {
        bRun=true;

        if(mtPlate2_Golden->XItem!=HotPlateForm.XDivision)
            mtPlate2_Golden->XItem=HotPlateForm.XDivision;
        if(mtPlate2_Golden->YItem!=HotPlateForm.YDivision)
            mtPlate2_Golden->YItem=HotPlateForm.YDivision;
        if(mtPlate1_Golden->XItem!=HotPlateForm.XDivision)
            mtPlate1_Golden->XItem=HotPlateForm.XDivision;
        if(mtPlate1_Golden->YItem!=HotPlateForm.YDivision)
            mtPlate1_Golden->YItem=HotPlateForm.YDivision;

        if(mtPlate2->XItem!=HotPlateForm.XDivision)
            mtPlate2->XItem=HotPlateForm.XDivision;
        if(mtPlate2->YItem!=HotPlateForm.YDivision)
            mtPlate2->YItem=HotPlateForm.YDivision;
        if(mtPlate1->XItem!=HotPlateForm.XDivision)
            mtPlate1->XItem=HotPlateForm.XDivision;
        if(mtPlate1->YItem!=HotPlateForm.YDivision)
            mtPlate1->YItem=HotPlateForm.YDivision;

        if(USE_LASER_DISTANCE==2)
        {
            if(TestIF_File.bEnableInShuttleFloatingCheck && bGetInArm)
            {
                switch(iLaserInArmTask)
                {
                    case 1:
                        iCount=0;
                        iLaserInArmTask=2;
                        break;
                    case 2:
                        GetInArmValue.sprintf("SR,01,519\r\n");
                        WriteInfoToMemo(GetInArmValue);
                        CommLaserInArm->WriteCommData(GetInArmValue.c_str(), GetInArmValue.Length()+1);
                        COMLaserInDelay.SetSecAndOn(3);
                        iLaserInArmTask=3;
    //                    break;
                    case 3:
                        #ifdef SOFT_SIMULTE
                        bRecvDataIn=true;
                        #endif
                        if(bRecvDataIn)
                        {
                            bRecvDataIn=false;
                            iLaserInArmTask=1;
                            bGetInArm=false;
                        }
                        else if(COMLaserInDelay.Off())
                        {
                            iCount++;
                            if(iCount>=5)
                            {
                                iCount=0;
//                                SW[SwLaserPower].On();
                                COMLaserInDelay.SetSecAndOn(1);
                                iLaserInArmTask=4;
                            }
                            else
                            {
                                iLaserInArmTask=2;
                            }
                        }
                        break;
                    case 4:
                        if(COMLaserInDelay.Off())
                        {
//                            SW[SwLaserPower].Off();
                            iLaserInArmTask=5;
                        }
                        break;
                    case 5:
                        btConnect->Click();
                        COMLaserInDelay.SetSecAndOn(1);
                        iLaserInArmTask=6;
                        break;
                    case 6:
                        if(COMLaserInDelay.Off())
                        {
                            iLaserInArmTask=2;
                        }
                        break;
                }
            }
            else if(TestIF_File.bEnableOutShuttleFloatingCheck && bGetOutArm)
            {
                switch(iLaserOutArmTask)
                {
                    case 1:
                        iCount=0;
                        iLaserOutArmTask=2;
                        break;
                    case 2:
                        GetOutArmValue.sprintf("SR,01,519\r\n");
                        WriteInfoToMemo(GetOutArmValue);
                        CommLaserOutArm->WriteCommData(GetOutArmValue.c_str(), GetOutArmValue.Length()+1);
                        COMLaserOutDelay.SetSecAndOn(3);
                        iLaserOutArmTask=3;
    //                    break;
                    case 3:
                        #ifdef SOFT_SIMULTE
                        bRecvDataOut=true;
                        #endif
                        if(bRecvDataOut)
                        {
                            bRecvDataOut=false;
                            iLaserOutArmTask=1;
                            bGetOutArm=false;
                        }
                        else if(COMLaserOutDelay.Off())
                        {
                            iCount++;
                            if(iCount>=5)
                            {
                                iCount=0;
//                                SW[SwLaserPower].On();
                                COMLaserOutDelay.SetSecAndOn(1);
                                iLaserOutArmTask=4;
                            }
                            else
                            {
                                iLaserOutArmTask=2;
                            }
                        }
                        break;
                    case 4:
                        if(COMLaserOutDelay.Off())
                        {
//                            SW[SwLaserPower].Off();
                            iLaserOutArmTask=5;
                        }
                        break;
                    case 5:
                        btConnect->Click();
                        COMLaserOutDelay.SetSecAndOn(1);
                        iLaserOutArmTask=6;
                        break;
                    case 6:
                        if(COMLaserOutDelay.Off())
                        {
                            iLaserOutArmTask=2;
                        }
                        break;
                }
            }
        }
        else if(bSetToZeroInArm)
        {
            switch(iLaserInArmTask)
            {
                case 1:
                    bRecvDataIn=false;
                    CommLaserInArm->WriteCommData(SetToZeroMessASCIIInArm.c_str(), SetToZeroMessASCIIInArm.Length()+1);
                    WriteInfoToMemo(SetToZeroMessageInArm);
                    COMLaserInDelay.SetSecAndOn(1);
                    iLaserInArmTask=2;
//                    break;
                case 2:
                    #ifdef SOFT_SIMULTE
                    bRecvDataIn=true;
                    #endif
                    if(bRecvDataIn)
                    {
                        bRecvDataIn=false;
                        iLaserInArmTask=1;
                        bSetToZeroInArm=false;
                    }
                    else if(COMLaserInDelay.Off())
                    {
                        iLaserInArmTask=1;
                    }
                    break;
            }
        }
        else if(bInArm)
        {
            switch(iLaserInArmTask)
            {
                case 1:
                    bRecvDataIn=false;
                    CommLaserInArm->WriteCommData(GetValMessASCIIInArm.c_str(), GetValMessASCIIInArm.Length()+1);
                    WriteInfoToMemo(GetValueMessageInArm);
                    COMLaserInDelay.SetSecAndOn(1);
                    iLaserInArmTask=2;
//                    break;
                case 2:
                    #ifdef SOFT_SIMULTE
                    bRecvDataIn=true;
                    #endif

                    if(bRecvDataIn)
                    {
                        bRecvDataIn=false;
                        iLaserInArmTask=1;
                        bInArm=false;
                    }
                    else if(COMLaserInDelay.Off())
                    {
                        iLaserInArmTask=1;
                    }
                    break;
            }
        }
        else if(fShow && cbStartPooling->Checked && SystemStart==false && cbEnableInArmLaser->Checked==true)
        {
            switch(iLaserInArmTask)
            {
                case 1:
                    bRecvDataIn=false;
                    CommLaserInArm->WriteCommData(GetValMessASCIIInArm.c_str(), GetValMessASCIIInArm.Length()+1);
                    WriteInfoToMemo(GetValueMessageInArm);
                    iLaserInArmTask=2;
                    COMLaserInDelay.SetSecAndOn(1);
//                    break;
                case 2:
                    #ifdef SOFT_SIMULTE
                    bRecvDataIn=true;
                    #endif

                    if(bRecvDataIn)
                    {
                        iLaserInArmTask=1;
                    }
                    else if(COMLaserInDelay.Off())
                    {
                        iLaserInArmTask=1;
                    }
                    break;
            }
        }
        else
        {
            iLaserInArmTask=1;
        }

        bRun=false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLaserSensor::FormDestroy(TObject *Sender)
{
    Timer1->Enabled=false;
    TimerInArm->Enabled=false;
    LogSoftwareOffTime("TfLaserSensor, FormDestroy"); //Steven 20210526 : 紀錄軟體執行時間
}
//---------------------------------------------------------------------------
void __fastcall TfLaserSensor::btSetPlate2GoldenToZeroClick(
      TObject *Sender)
{
    for(int iX=0; iX<mtPlate2_Golden->XItem; iX++)
    {
        for(int iY=0; iY<mtPlate1_Golden->YItem; iY++)
        {
            TestIF_File.dLaserValueIn[0][iX][iY]=0.0;
            mtPlate2_Golden->SetCellNumber(iX, iY, 0.0);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLaserSensor::btSetPlate1GoldenToZeroClick(
      TObject *Sender)
{
    for(int iX=0; iX<mtPlate1_Golden->XItem; iX++)
    {
        for(int iY=0; iY<mtPlate1_Golden->YItem; iY++)
        {
            TestIF_File.dLaserValueIn[1][iX][iY]=0.0;
            mtPlate1_Golden->SetCellNumber(iX, iY, 0.0);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLaserSensor::mtPlate2_GoldenMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    mtPlate2_Golden->ConvertIndexCells(X, Y);
    edTemp->Text=mtPlate2_Golden->GetCellText(X, Y);
    fQwertyKey->ShowQwertyKey(edTemp, N_DOUBLE, 2, true, 1.0, -1.0);
    mtPlate2_Golden->SetCellNumber(X, Y, atof(edTemp->Text.c_str()));
    TestIF_File.dLaserValueIn[0][X][Y]=atof(edTemp->Text.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TfLaserSensor::mtPlate1_GoldenMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    mtPlate1_Golden->ConvertIndexCells(X, Y);
    edTemp->Text=mtPlate1_Golden->GetCellText(X, Y);
    fQwertyKey->ShowQwertyKey(edTemp, N_DOUBLE, 2, true, 1.0, -1.0);
    mtPlate1_Golden->SetCellNumber(X, Y, atof(edTemp->Text.c_str()));
    TestIF_File.dLaserValueIn[1][X][Y]=atof(edTemp->Text.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TfLaserSensor::edDeviceXOffsetMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    //==> Eastsun 20260525 laser 整合 : 從 a-side 補實作
    int iX=DeviceForm.XDimension/2;
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 2, true, iX, -iX);
    //<== Eastsun 20260525 laser 整合
}
//---------------------------------------------------------------------------

void __fastcall TfLaserSensor::edDeviceYOffsetMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    //==> Eastsun 20260525 laser 整合 : 從 a-side 補實作
    int iY=DeviceForm.YDimension/2;
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 2, true, iY, -iY);
    //<== Eastsun 20260525 laser 整合
}
//---------------------------------------------------------------------------

void __fastcall TfLaserSensor::edOutDeviceXOffsetMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    //==> Eastsun 20260525 laser 整合 : 從 a-side 補實作
    int iX=DeviceForm.XDimension/2;
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 2, true, iX, -iX);
    //<== Eastsun 20260525 laser 整合
}
//---------------------------------------------------------------------------

void __fastcall TfLaserSensor::edOutDeviceYOffsetMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    //==> Eastsun 20260525 laser 整合 : 從 a-side 補實作
    int iY=DeviceForm.YDimension/2;
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 2, true, iY, -iY);
    //<== Eastsun 20260525 laser 整合
}
//---------------------------------------------------------------------------

void __fastcall TfLaserSensor::btGetInShuttleGoldenClick(TObject *Sender)
{
    //==> Eastsun 20260525 laser 整合 : 從 a-side 補實作
    iGetInShuttleGoldenTask=1;
    iGetOutShuttleGoldenTask=0;
    btGetInShuttleGolden->Enabled=false;
    //<== Eastsun 20260525 laser 整合
}
//---------------------------------------------------------------------------

void __fastcall TfLaserSensor::btGetOutShuttleGoldenClick(TObject *Sender)
{
    //==> Eastsun 20260525 laser 整合 : 從 a-side 補實作
    iGetOutShuttleGoldenTask=1;
    iGetInShuttleGoldenTask=0;
    btGetOutShuttleGolden->Enabled=false;
    //<== Eastsun 20260525 laser 整合
}
//-------------------------------------------------------------
//==> Eastsun 20260525 laser 整合
void TfLaserSensor::DoGetLaserValue()
{
    int &Task=iGetInShuttleGoldenTask;

    static int iSh;

    switch(Task)
    {
        case 1:
             if(TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==0) //Arm1
                iSh=1;
             else if(TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==1) //Arm2
                iSh=2;
             else
                iSh=3;

             Task=100;
             break;
        case 100:
            if(iSh==3)
            {
                Task=500;
            }
            else if(iSh==2)
            {
                Task=1500;
            }
            else if(iSh==1)
            {
                Task=500;
            }
            break;
        case 500:
            if(MOT[MInShuttle1].IsCanMove())
            {
                Task=600;
            }
            break;
        case 600:
            if(MOT[MInShuttle1].MotorMove(Prod.InSHT[0].iLeft))
            {
                Task=700;
                UseInArmCheckShtFloating(0, true, true);
            }
            break;
        case 700:
            if(UseInArmCheckShtFloating(0, false, true))
            {
                if(iSh==3)
                {
                    Task=1500;
                }
                else
                {
                    btGetInShuttleGolden->Enabled=true;
                    Task=3000;
                }
            }
            break;
        case 1500:
            if(MOT[MInShuttle2].IsCanMove())
            {
                Task=1600;
            }
            break;
        case 1600:
            if(MOT[MInShuttle2].MotorMove(Prod.InSHT[1].iLeft))
            {
                Task=1700;
                UseInArmCheckShtFloating(1, true, true);
            }
            break;
        case 1700:
            if(UseInArmCheckShtFloating(1, false, true))
            {
                btGetInShuttleGolden->Enabled=true;
                Task=3000;
            }
            break;
    }
}
//-------------------------------------------------------------
void TfLaserSensor::DoGetOutLaserValue()
{
    int &Task=iGetOutShuttleGoldenTask;

    static int iSh;

    switch(Task)
    {
        case 1:
             if(TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==0) //Arm1
                iSh=1;
             else if(TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==1) //Arm2
                iSh=2;
             else
                iSh=3;

             Task=100;
             break;
        case 100:
            if(iSh==3)
            {
                Task=500;
            }
            else if(iSh==2)
            {
                Task=1500;
            }
            else if(iSh==1)
            {
                Task=500;
            }
            break;
        case 500:
            if(MOT[MInShuttle1].IsCanMove())
            {
                Task=600;
            }
            break;
        case 600:
            if(MOT[MInShuttle1].MotorMove(Prod.InSHT[0].iRight))
            {
                Task=700;
                UseOutArmCheckShtFloating(0, true, true);
            }
            break;
        case 700:
            if(UseOutArmCheckShtFloating(0, false, true))
            {
                if(iSh==3)
                {
                    Task=1500;
                }
                else
                {
                    btGetOutShuttleGolden->Enabled=true;
                    Task=3000;
                }
            }
            break;
        case 1500:
            if(MOT[MInShuttle2].IsCanMove())
            {
                Task=1600;
            }
            break;
        case 1600:
            if(MOT[MInShuttle2].MotorMove(Prod.InSHT[1].iRight))
            {
                Task=1700;
                UseOutArmCheckShtFloating(1, true, true);
            }
            break;
        case 1700:
            if(UseOutArmCheckShtFloating(1, false, true))
            {
                btGetOutShuttleGolden->Enabled=true;
                Task=3000;
            }
            break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfLaserSensor::CommLaserOutArmReceiveData(TObject *Sender,
      Pointer Buffer, WORD BufferLength)
{
    //==> Eastsun 20260525 laser 整合 : 從 a-side 補實作 (OutArm Laser COM 接收)
    AnsiString ComOmronBuffer="RECV: ";
    AnsiString ComOmronBuffer2="";

    AnsiString S="";
    byte *data;
    bool bHasETX=false;

    data=(byte *)Buffer;

    for(int i=0; i<BufferLength; i++)
    {
        if(USE_LASER_DISTANCE==2)
        {
            asLaserValue[1]+=MyDeCodeASCII(data[i]);
        }

        ComOmronBuffer+=MyDeCodeASCII(data[i]);
        ComOmronBuffer2+=MyDeCodeASCII(data[i]);
    }

    if(bHasETX==true)
    {
        bHasETX=false;
        ProcessReceiveData(2, ComOmronBuffer2);
        ComOmronBuffer2="";
    }

    WriteInfoToMemo("COMLaserOut: "+ComOmronBuffer);
    bRecvDataOut=true;
    //<== Eastsun 20260525 laser 整合
}
//---------------------------------------------------------------------------
void __fastcall TfLaserSensor::btGetShValueClick(TObject *Sender)
{
    //==> Eastsun 20260525 laser 整合 : 從 a-side 補實作
    iLaserInArmTask=1;
    bGetInArm=true;
    //<== Eastsun 20260525 laser 整合
}
//---------------------------------------------------------------------------
void __fastcall TfLaserSensor::btGetOutShValueClick(TObject *Sender)
{
    //==> Eastsun 20260525 laser 整合 : 從 a-side 補實作
    iLaserOutArmTask=1;
    bGetOutArm=true;
    //<== Eastsun 20260525 laser 整合
}
//---------------------------------------------------------------------------

