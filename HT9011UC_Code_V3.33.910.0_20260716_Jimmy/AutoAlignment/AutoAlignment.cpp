//---------------------------------------------------------------------------
#include "MachineDefine.h"
#pragma hdrstop

#include "AutoAlignment.h"
#include "common.h"
#include "csystem.h"
#include "cinitial.h"
#include "InOutArmZteach.h"
#include "ainarm2.h"
#include "ainarm9045.h"
#include "aoutarm.h"
#include "aoutarm9045.h"
#include "mymessbox.h"
#include "mymotor.h"
#include "uteach.h"
#include "main.h"
#include "myQwertyKeyBoard.h"
#include "mycylin.h"
#include "mysensor.h"
#include "iosetview.h"
#include "WinSocketErrorCode.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "HTray"
#pragma resource "*.dfm"
TfAutoAlignment *fAutoAlignment;

AnsiString sSendCMD[TOTAL];
AnsiString sReceiveOKCMD[TOTAL];
AnsiString sReceiveNGCMD[TOTAL];
AnsiString sInArmReceiveResult[TOTAL];
AnsiString sOutArmReceiveResult[TOTAL];

double dInArmOffsetX=0;
double dInArmOffsetY=0;

double dOutArmOffsetX=0;
double dOutArmOffsetY=0;

AnsiString AsReceiveData1;
AnsiString AsReceiveData2;

AnsiString AsLoadFile="";
//int Teach_AutoAlignmentUnit=0;

//---------------------------------------------------------------------------
__fastcall TfAutoAlignment::TfAutoAlignment(TComponent* Owner)
    : TForm(Owner)
{
    bInArmSendTeachCmd=false;
    bOutArmSendTeachCmd=false;

    sSendCMD[LOAD_FILE]       = "@LF%s+";

    sSendCMD[UNIT_CENTER]     = "@GU,C1+";
    sSendCMD[UNIT_LEFT]       = "@GU,L1+";
    sSendCMD[UNIT_RIGHT]      = "@GU,R1+";
    sSendCMD[UNIT_UP]         = "@GU,U1+";
    sSendCMD[UNIT_DOWN]       = "@GU,D1+";

    sSendCMD[TEACH_GOLDEN]    = "@TH,GD+";
    sSendCMD[TEACH_TRAY]      = "@TH,TY+";
    sSendCMD[TEACH_HOTPLATE]  = "@TH,HP+";
    sSendCMD[TEACH_SHUTTLE]   = "@TH,ST+";
    sSendCMD[TEACH_AUTOCLEAN] = "@TH,AC+";          //KenHsieh 20211214 : AOA add AutoClean
    sSendCMD[WAIT_TEACH_GOLDEN] = "@TH,XX+";

    sSendCMD[AUTO_GOLDEN]     = "@AT,GD+";
    sSendCMD[AUTO_TRAY]       = "@AT,TY+";
    sSendCMD[AUTO_HOTPLATE]   = "@AT,HP+";
    sSendCMD[AUTO_SHUTTLE]    = "@AT,ST+";
    sSendCMD[AUTO_AUTOCLEAN]  = "@AT,AC+";          //KenHsieh 20211214 : AOA add AutoClean
  //-----------------------------------------------------------
    sReceiveOKCMD[LOAD_FILE]       = "@LFOK+";

    sReceiveOKCMD[UNIT_CENTER]     = "@GU,C1,";
    sReceiveOKCMD[UNIT_LEFT]       = "@GU,L1,";
    sReceiveOKCMD[UNIT_RIGHT]      = "@GU,R1,";
    sReceiveOKCMD[UNIT_UP]         = "@GU,U1,";
    sReceiveOKCMD[UNIT_DOWN]       = "@GU,D1,";

    sReceiveOKCMD[TEACH_GOLDEN]    = "@TH,GD,";
    sReceiveOKCMD[TEACH_TRAY]      = "@TH,TY,";
    sReceiveOKCMD[TEACH_HOTPLATE]  = "@TH,HP,";
    sReceiveOKCMD[TEACH_SHUTTLE]   = "@TH,ST,";
    sReceiveOKCMD[TEACH_AUTOCLEAN] = "@TH,AC,";     //KenHsieh 20211214 : AOA add AutoClean

    sReceiveOKCMD[AUTO_GOLDEN]     = "@AT,GD,";
    sReceiveOKCMD[AUTO_TRAY]       = "@AT,TY,";
    sReceiveOKCMD[AUTO_HOTPLATE]   = "@AT,HP,";
    sReceiveOKCMD[AUTO_SHUTTLE]    = "@AT,ST,";
    sReceiveOKCMD[AUTO_AUTOCLEAN]  = "@AT,AC,";     //KenHsieh 20211214 : AOA add AutoClean
  //-----------------------------------------------------------
    sReceiveNGCMD[LOAD_FILE]       = "@LFNG+";

    sReceiveNGCMD[UNIT_CENTER]     = "@GU,C1,999";//"@GU,C2+";
    sReceiveNGCMD[UNIT_LEFT]       = "@GU,L1,999";
    sReceiveNGCMD[UNIT_RIGHT]      = "@GU,R1,999";
    sReceiveNGCMD[UNIT_UP]         = "@GU,U1,999";
    sReceiveNGCMD[UNIT_DOWN]       = "@GU,D1,999";

    sReceiveNGCMD[TEACH_GOLDEN]    = "@TH,GD,999";
    sReceiveNGCMD[TEACH_TRAY]      = "@TH,TY,999";
    sReceiveNGCMD[TEACH_HOTPLATE]  = "@TH,HP,999";
    sReceiveNGCMD[TEACH_SHUTTLE]   = "@TH,ST,999";
    sReceiveNGCMD[TEACH_AUTOCLEAN] = "@TH,AC,999";  //KenHsieh 20211214 : AOA add AutoClean

    sReceiveNGCMD[AUTO_GOLDEN]     = "@AT,GD,999";
    sReceiveNGCMD[AUTO_TRAY]       = "@AT,TY,999";
    sReceiveNGCMD[AUTO_HOTPLATE]   = "@AT,HP,999";
    sReceiveNGCMD[AUTO_SHUTTLE]    = "@AT,ST,999";
    sReceiveNGCMD[AUTO_AUTOCLEAN]  = "@AT,AC,999";  //KenHsieh 20211214 : AOA add AutoClean

    //"@TH,TY,X,Y+"
    edtInArmAddress->Text     =CheckAndReadIniDataGeneral("Auto_Alignment",    "AUTO_ALIGNMENT_CCD1_ADRESS"       , AnsiString("172.16.8.131"));
    edtInArmPort->Text        =CheckAndReadIniDataGeneral("Auto_Alignment",    "AUTO_ALIGNMENT_CCD1_PORT"         , AnsiString("5001"));            //KenHsieh 20211204 : 修改預設port "6001->5001"

    edtOutArmAddress->Text    =CheckAndReadIniDataGeneral("Auto_Alignment",    "AUTO_ALIGNMENT_CCD2_ADRESS"       , AnsiString("172.16.8.131"));
    edtOutArmPort->Text       =CheckAndReadIniDataGeneral("Auto_Alignment",    "AUTO_ALIGNMENT_CCD2_PORT"         , AnsiString("5002"));            //KenHsieh 20211204 : 修改預設port "6002->5002

    ClientSocket1->Address  = edtInArmAddress->Text;
    ClientSocket1->Port     = atoi(edtInArmPort->Text.c_str());
    ClientSocket2->Address  = edtOutArmAddress->Text;
    ClientSocket2->Port     = atoi(edtOutArmPort->Text.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::TcpipOpen()
{
    ClientSocket1->Open();
    ClientSocket2->Open();
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::TcpipClose()
{
    ClientSocket1->Close();
    ClientSocket2->Close();
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::ChangeFileName(AnsiString FileName)
{
    AsLoadFile=FileName;
    edtFileName->Text = AsLoadFile;
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::InArmSendCommand(ALIGNMENTCMD cmd)
{
    AnsiString asSendCMD;
    if(cmd==LOAD_FILE)
    {
        asSendCMD.sprintf("@LF%s+",AsLoadFile.c_str());
    }
    else
    {
        asSendCMD = sSendCMD[cmd];
    }

    if(cmd!=WAIT_TEACH_GOLDEN)
        bInArmSendTeachCmd = false;

    ClientSocket1->Socket->SendText(asSendCMD);

    Memo1->Lines->Add(asSendCMD);

    sInArmReceiveResult[cmd]="";
}
//---------------------------------------------------------------------------
int __fastcall TfAutoAlignment::InArmGetResult(ALIGNMENTCMD cmd, double &dOffsetX, double &dOffsetY)
{
    dOffsetX=0;
    dOffsetY=0;
    #ifdef SOFT_SIMULTE
    return 1;
    #else

    if(sInArmReceiveResult[cmd]=="NG")
    {
        dOffsetX = dInArmOffsetX;
        dOffsetY = dInArmOffsetY;
        bInArmSendTeachCmd=false;
        return 2;
    }
    else if(sInArmReceiveResult[cmd]=="OK")
    {
        dOffsetX = (dInArmOffsetX-640);
        dOffsetY = -(dInArmOffsetY-480);
        bInArmSendTeachCmd=false;
        return 1;
    }
    else if(cmd==TEACH_GOLDEN || cmd==TEACH_TRAY || cmd==TEACH_HOTPLATE || cmd==TEACH_SHUTTLE || cmd==TEACH_AUTOCLEAN)
    {
        bInArmSendTeachCmd=true;
    }
    return 0;
    #endif
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::OutArmSendCommand(ALIGNMENTCMD cmd)
{
    AnsiString asSendCMD;
    if(cmd==LOAD_FILE)
    {
        asSendCMD.sprintf("@LF%s+",AsLoadFile.c_str());
    }
    else
    {
        asSendCMD = sSendCMD[cmd];
    }

    if(cmd!=WAIT_TEACH_GOLDEN)
        bOutArmSendTeachCmd=false;
    ClientSocket2->Socket->SendText(asSendCMD);
    Memo2->Lines->Add(asSendCMD);

    sOutArmReceiveResult[cmd]="";
}
//---------------------------------------------------------------------------
int __fastcall TfAutoAlignment::OutArmGetResult(ALIGNMENTCMD cmd, double &dOffsetX, double &dOffsetY)
{
    dOffsetX=0;
    dOffsetY=0;
    #ifdef SOFT_SIMULTE
    return 1;
    #else
    if(sOutArmReceiveResult[cmd]=="NG")
    {
        dOffsetX = dOutArmOffsetX;
        dOffsetY = dOutArmOffsetY;
        bOutArmSendTeachCmd=false;
        return 2;
    }
    else if(sOutArmReceiveResult[cmd]=="OK")
    {
        dOffsetX = (dOutArmOffsetX-640);
        dOffsetY = -(dOutArmOffsetY-480);
        bOutArmSendTeachCmd=false;
        return 1;
    }
    else if(cmd==TEACH_GOLDEN || cmd==TEACH_TRAY || cmd==TEACH_HOTPLATE || cmd==TEACH_SHUTTLE)
    {
        bOutArmSendTeachCmd=true;
    }
    return 0;
    #endif
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::btnConnectClick(TObject *Sender)
{
    TcpipOpen();
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::btnDisconnectClick(TObject *Sender)
{
    TcpipClose();
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::ClientSocket1Read(TObject *Sender,
      TCustomWinSocket *Socket)
{
    AnsiString TempCMD;

    AsReceiveData1 += Socket->ReceiveText();

    for(int i=0; i<TOTAL; i++)
    {
        if(AsReceiveData1.Pos(sReceiveNGCMD[i]))    //20211125 : 若CCD傳回值為999便NG
        {
            sInArmReceiveResult[i]="NG";
            Memo1->Lines->Add(AsReceiveData1);
            AsReceiveData1="";
        }
        else if(AsReceiveData1.Pos(sReceiveOKCMD[i]))
        {
            sInArmReceiveResult[i]="OK";

            if(i!=LOAD_FILE)
            {
                TempCMD = AsReceiveData1;
                TempCMD = TempCMD.SubString(TempCMD.Pos(sReceiveOKCMD[i])+sReceiveOKCMD[i].Length(),TempCMD.Length()-sReceiveOKCMD[i].Length());
                dInArmOffsetX = atof(TempCMD.SubString(1,TempCMD.Pos(",")-1).c_str());
                dInArmOffsetY = atof(TempCMD.SubString(TempCMD.Pos(",")+1,TempCMD.Pos("+")-TempCMD.Pos(",")-1).c_str());
                bInArmSendTeachCmd=false;                                       //KenHsieh 20220218 : 修改"TH,XX"模式，收到資料便停止
            }
            Memo1->Lines->Add(AsReceiveData1);
            AsReceiveData1="";
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::ClientSocket1Connect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    AnsiString asConnectString = "Client1 Connected!";
    Memo1->Lines->Add(asConnectString);
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::ClientSocket1Disconnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    AnsiString asConnectString = "Client1 Disconnect!";
    Memo1->Lines->Add(asConnectString);
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::ClientSocket1Error(TObject *Sender,
      TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
    AsReceiveData1="";
    AnsiString asErrorString = "Client1 ErrorCode :" + AnsiString(ErrorCode);
    Memo1->Lines->Add(asErrorString);
    ErrorCode = 0;
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::ClientSocket2Connect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    AnsiString asConnectString = "Client2 Connected!";
    Memo2->Lines->Add(asConnectString);
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::ClientSocket2Disconnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    AnsiString asConnectString = "Client2 Disconnect!";
    Memo2->Lines->Add(asConnectString);
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::ClientSocket2Error(TObject *Sender,
      TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
    AsReceiveData2="";
    AnsiString asErrorString = "Client2 ErrorCode :" + AnsiString(ErrorCode);
    Memo2->Lines->Add(asErrorString);
    ErrorCode = 0;
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::ClientSocket2Read(TObject *Sender,
      TCustomWinSocket *Socket)
{
    AnsiString TempCMD;

    AsReceiveData2 += Socket->ReceiveText();

    for(int i=0; i<TOTAL; i++)
    {
        if(AsReceiveData2.Pos(sReceiveNGCMD[i]))                                //20211125 : 若CCD傳回值為999便NG
        {
            sOutArmReceiveResult[i]="NG";
            Memo2->Lines->Add(AsReceiveData2);
            AsReceiveData2="";
        }
        else if(AsReceiveData2.Pos(sReceiveOKCMD[i]))
        {
            sOutArmReceiveResult[i]="OK";

            if(i!=LOAD_FILE)
            {
                TempCMD = AsReceiveData2;

                TempCMD = TempCMD.SubString(TempCMD.Pos(sReceiveOKCMD[i])+sReceiveOKCMD[i].Length(),TempCMD.Length()-sReceiveOKCMD[i].Length());
                dOutArmOffsetX = atof(TempCMD.SubString(1,TempCMD.Pos(",")-1).c_str());
                dOutArmOffsetY = atof(TempCMD.SubString(TempCMD.Pos(",")+1,TempCMD.Pos("+")-TempCMD.Pos(",")-1).c_str());
                bOutArmSendTeachCmd=false;      //KenHsieh 20220218 : 修改"TH,XX"模式，收到資料便停止
            }
            Memo2->Lines->Add(AsReceiveData2);
            AsReceiveData2="";
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::Timer1Timer(TObject *Sender)
{
    if(bInArmSendTeachCmd==true)
    {
        InArmSendCommand(WAIT_TEACH_GOLDEN);
    }

    if(bOutArmSendTeachCmd==true)
    {
        OutArmSendCommand(WAIT_TEACH_GOLDEN);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::edtFileNameChange(TObject *Sender)
{
    ChangeFileName(edtFileName->Text);
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::Button1Click(TObject *Sender)
{
    InArmSendCommand((ALIGNMENTCMD)cbInArmCommand->ItemIndex);
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::Button2Click(TObject *Sender)
{
    OutArmSendCommand((ALIGNMENTCMD)cbOutArmCommand->ItemIndex);
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::sbtExitClick(TObject *Sender)
{
    sbtExit->Down=false;
    Close();
    bShow=false;
}
//---------------------------------------------------------------------------
int iInArmXYZHomeTask=1;
int iOutArmXYZHomeTask=1;
int iAutoAlignmentTask=1;
//---------------------------------------------------------------------------
void InitInArmXYZHome()
{
    iInArmXYZHomeTask=1;
}
//---------------------------------------------------------------------------
int DoInArmXYZHome()
{
    int &Task=iInArmXYZHomeTask;
    static bool bFlag[5]={false, false, false, false, false};      //KenHsieh 20210829 : Modify add CCD AUTO ALIGNMENT (XY變距)

    switch(Task)
    {
        case 1: //All Z Homing
            Zteach->InOutArmZHome(0);
            Task = 100;
            break;
        case 100:
            if(Zteach->DoZHome()==false) //Z軸歸零
                return false;

            InitProcessSingleMotorTask(MInArmPitch);
            if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable)            //KenHsieh 20210829 : Modify add CCD AUTO ALIGNMENT (XY變距)
            {
                InitProcessSingleMotorTask(MInArmPitchY);
                InitProcessSingleMotorTask(MInArmPitchX2);
            }
            InitProcessSingleMotorTask(MInArmX);
            InitProcessSingleMotorTask(MInArmY);
            for(int i=0; i<5; i++)                                  //KenHsieh 20210829 : Modify add CCD AUTO ALIGNMENT (XY變距)
                bFlag[i]=false;
            Task = 200;
            break;
        case 200:
            if(bFlag[0]!=true)
                bFlag[0] = ProcessSingleMotorHome(MInArmPitch);
            if(bFlag[1]!=true)
                bFlag[1] = ProcessSingleMotorHome(MInArmX);
            if(bFlag[2]!=true)
                bFlag[2] = ProcessSingleMotorHome(MInArmY);
            if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable)            //KenHsieh 20210829 : Modify add CCD AUTO ALIGNMENT (XY變距)
            {
                if(bFlag[3]!=true)
                    bFlag[3] = ProcessSingleMotorHome(MInArmPitchY);
                if(bFlag[4]!=true)
                    bFlag[4] = ProcessSingleMotorHome(MInArmPitchX2);
            }

            if(bFlag[0]==true && bFlag[1]==true && bFlag[2]==true)
            {
                if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable)        //KenHsieh 20210829 : Modify add CCD AUTO ALIGNMENT (XY變距)
                {
                    if(bFlag[3]==true && bFlag[4]==true)
                    {
                        for(int i=0; i<5; i++)
                            bFlag[i]=false;
                        Task=300;
                    }
                }
                else
                {
                    for(int i=0; i<5; i++)
                        bFlag[i]=false;
                    Task=300;
                }
                break;
            }
            break;
        case 300:
            return 1;
    }
    return 0;
}
//---------------------------------------------------------------------------
void InitOutArmXYZHome()
{
    iOutArmXYZHomeTask=1;
}
//---------------------------------------------------------------------------
int DoOutArmXYZHome()
{
    int &Task=iOutArmXYZHomeTask;
    static bool bFlag[5]={false, false, false, false, false};      //KenHsieh 20210829 : Modify add CCD AUTO ALIGNMENT (XY變距)

    switch(Task)
    {
        case 1: //All Z Homing
            Zteach->InOutArmZHome(1);
            Task = 100;
            break;
        case 100:
            if(Zteach->DoZHome()==false) //Z軸歸零
                return false;

            InitProcessSingleMotorTask(MOutArmPitch);
            if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable)            //KenHsieh 20210829 : Modify add CCD AUTO ALIGNMENT (XY變距)
            {
                InitProcessSingleMotorTask(MOutArmPitchY);
                InitProcessSingleMotorTask(MOutArmPitchX2);
            }
            InitProcessSingleMotorTask(MOutArmX);
            InitProcessSingleMotorTask(MOutArmY);
            for(int i=0; i<5; i++)                                  //KenHsieh 20210829 : Modify add CCD AUTO ALIGNMENT (XY變距)
                bFlag[i]=false;
            Task = 200;
            break;
        case 200:
            if(bFlag[0]!=true)
                bFlag[0] = ProcessSingleMotorHome(MOutArmPitch);
            if(bFlag[1]!=true)
                bFlag[1] = ProcessSingleMotorHome(MOutArmX);
            if(bFlag[2]!=true)
                bFlag[2] = ProcessSingleMotorHome(MOutArmY);
            if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable)            //KenHsieh 20210829 : Modify add CCD AUTO ALIGNMENT (XY變距)
            {
                if(bFlag[3]!=true)
                    bFlag[3] = ProcessSingleMotorHome(MOutArmPitchY);
                if(bFlag[4]!=true)
                    bFlag[4] = ProcessSingleMotorHome(MOutArmPitchX2);
            }

            if(bFlag[0]==true && bFlag[1]==true && bFlag[2]==true)
            {
                if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable)        //KenHsieh 20210829 : Modify add CCD AUTO ALIGNMENT (XY變距)
                {
                    if(bFlag[3]==true && bFlag[4]==true)
                    {
                        for(int i=0; i<5; i++)
                            bFlag[i]=false;
                        Task=300;
                    }
                }
                else
                {
                    for(int i=0; i<5; i++)
                        bFlag[i]=false;
                    Task=300;
                }
                break;
            }
            break;
        case 300:
            return 1;
    }
    return 0;
}
//---------------------------------------------------------------------------
void InitAutoAlignmentTask()
{
    iAutoAlignmentTask=1;
}
//---------------------------------------------------------------------------
void TfAutoAlignment::DoAutoAlignment()
{
    double ResolutionX;
    double ResolutionY;
    double dRadianX;
    double dRadianY;

    int iRet=0;
    int iPicker=0;
    int &Task=iAutoAlignmentTask;

    int iXPos[8], iYPos[8], iZPos[8];

    if(Teach_AutoAlignmentUnit==0)
        return;

    if(IsEMGPressed()==true)
    {
        Teach_AutoAlignmentUnit = 0;
        Task=1;
        return;
    }

    switch(Task)
    {
        case 1:
            if(Teach_AutoAlignmentUnit==1)
                Task=1000;
            else if(Teach_AutoAlignmentUnit==2)
                Task=2000;
            else if(Teach_AutoAlignmentUnit==3)  //InArm Picker X Y Z
                Task=3000;
            else if(Teach_AutoAlignmentUnit==4)  //OutArm Picker X Y Z
                Task=4000;
            else if(Teach_AutoAlignmentUnit==5)  //KenHsieh 20211023 : 新增X Pitch 40mm teach
                Task=6000;
            else if(Teach_AutoAlignmentUnit==6)  //KenHsieh 20211023 : 新增X Pitch 40mm teach
                Task=7000;
            else if(Teach_AutoAlignmentUnit==10)
                Task=5000;
            else if(Teach_AutoAlignmentUnit==11)
                Task=11000;
            else if(Teach_AutoAlignmentUnit==20)
                Task=20000;
            else if(Teach_AutoAlignmentUnit==21)
                Task=21000;
            else
                break;
            SetTechDataToProd();
            break;
        case 1000:
            if(ClientSocket1->Active==false)    //KenHsieh 20210902 : Modify add CCD AUTO ALIGNMENT
            {
                ShowMyMessage("Auto Alignment CCD1 Not Connected");
                break;
            }

            if(DoInArmXYZHome())
            {
                Task=1100;
            }
            break;
        case 1100:
            iRet=DoInArmCCDAlignmentTeachUnitProces(ResolutionX, ResolutionY, dRadianX, dRadianY, 2);   //KenHsieh 20220117 : 避免teach失敗時將值帶入
            if(iRet==1)
            {
                fTeach->edtInArmCCDXResolution->Text = ResolutionX;
                fTeach->edtInArmCCDYResolution->Text = ResolutionY;
                fTeach->edtInArmCCDXRadian->Text = dRadianX;
                fTeach->edtInArmCCDYRadian->Text = dRadianY;

                fTeach->edtInArmXCCDPos->Text = MOT[MInArmX].ReadPos();
                fTeach->edtInArmYCCDPos->Text = MOT[MInArmY].ReadPos();

                Teach_AutoAlignmentUnit=0;

                ShowMyMessage("Auto Alignment In Resolution Teach Finish, Please Save before doing any action.");   //KenHsieh 20211013 : CCD Teach完成後提示新增儲存提醒
                Task=1;
            }
            break;
        case 2000:
            if(ClientSocket2->Active==false)    //KenHsieh 20210902 : Modify add CCD AUTO ALIGNMENT
            {
                ShowMyMessage("Auto Alignment CCD2 Not Connected");
                break;
            }

            if(DoOutArmXYZHome())
            {
                Task=2100;
            }
            break;
        case 2100:
            iRet=DoOutArmCCDAlignmentTeachUnitProces(ResolutionX, ResolutionY, dRadianX, dRadianY, 2);   //KenHsieh 20220117 : 避免teach失敗時將值帶入
            if(iRet==1)
            {
                fTeach->edtOutArmCCDXResolution->Text = ResolutionX;
                fTeach->edtOutArmCCDYResolution->Text = ResolutionY;
                fTeach->edtOutArmCCDXRadian->Text = dRadianX;
                fTeach->edtOutArmCCDYRadian->Text = dRadianY;

                fTeach->edtOutArmXCCDPos->Text = MOT[MOutArmX].ReadPos();
                fTeach->edtOutArmYCCDPos->Text = MOT[MOutArmY].ReadPos();

                Teach_AutoAlignmentUnit=0;

                ShowMyMessage("Auto Alignment Out Resolution Teach Finish, Please Save before doing any action.");  //KenHsieh 20211013 : CCD Teach完成後提示新增儲存提醒
                Task=1;
            }
            break;
        case 3000:
            if(DoInArmXYZHome())
            {
                Task=3100;
            }
            break;
        case 3100:
//            if(USE_IN_OUT_ARM_Y_PITCH==iXPitchManual635 ||
//               USE_IN_OUT_ARM_Y_PITCH==iXPitchManual360)                        //KenHsieh 20210823 : Modify add CCD AUTO ALIGNMENT (XY變距)
//            if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable)
//                iRet=DoInArmPickerXYAndPitchAutoAlignment(5, true, iXPos, iYPos, iZPos);
//            else
//                iRet=DoInArmPickerXYAndPitchAutoAlignment(4, true, iXPos, iYPos, iZPos);
            if(USE_PICKER_COUNT==ep1Picker)
            {
                iPicker=0;
            }
            else if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker ||
                    USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)
            {
                iPicker=iInArmXBase+iInArmYBase*8;
            }
            else
            {
                iPicker=(iInArmXBase*2)+iInArmYBase;                            //KenHsieh 20260116 : Fix AOA 基準吸嘴for Func. 0 2 4 6
            }                                                                   //                                              1 3 5 7

            iRet=DoInArmPickerXYAndPitchAutoAlignment(iPicker, true, iXPos, iYPos, iZPos);

            if(iRet==1)
            {
        /*        fTeach->edtInArmAlignmentPick1_X->Text = Tech.iInArmXCCDAlignmentPos - iXPos[0];
                fTeach->edtInArmAlignmentPick2_X->Text = Tech.iInArmXCCDAlignmentPos - iXPos[2];
                fTeach->edtInArmAlignmentPick3_X->Text = Tech.iInArmXCCDAlignmentPos - iXPos[4];
                fTeach->edtInArmAlignmentPick4_X->Text = Tech.iInArmXCCDAlignmentPos - iXPos[6];
                fTeach->edtInArmAlignmentPick5_X->Text = Tech.iInArmXCCDAlignmentPos - iXPos[1];
                fTeach->edtInArmAlignmentPick6_X->Text = Tech.iInArmXCCDAlignmentPos - iXPos[3];
                fTeach->edtInArmAlignmentPick7_X->Text = Tech.iInArmXCCDAlignmentPos - iXPos[5];
                fTeach->edtInArmAlignmentPick8_X->Text = Tech.iInArmXCCDAlignmentPos - iXPos[7];

                fTeach->edtInArmAlignmentPick1_Y->Text = Tech.iInArmYCCDAlignmentPos - iYPos[0];
                fTeach->edtInArmAlignmentPick2_Y->Text = Tech.iInArmYCCDAlignmentPos - iYPos[2];
                fTeach->edtInArmAlignmentPick3_Y->Text = Tech.iInArmYCCDAlignmentPos - iYPos[4];
                fTeach->edtInArmAlignmentPick4_Y->Text = Tech.iInArmYCCDAlignmentPos - iYPos[6];
                fTeach->edtInArmAlignmentPick5_Y->Text = Tech.iInArmYCCDAlignmentPos - iYPos[1];
                fTeach->edtInArmAlignmentPick6_Y->Text = Tech.iInArmYCCDAlignmentPos - iYPos[3];
                fTeach->edtInArmAlignmentPick7_Y->Text = Tech.iInArmYCCDAlignmentPos - iYPos[5];
                fTeach->edtInArmAlignmentPick8_Y->Text = Tech.iInArmYCCDAlignmentPos - iYPos[7];

                fTeach->edtInArmAlignmentPick1_Z->Text = iZPos[0];
                fTeach->edtInArmAlignmentPick2_Z->Text = iZPos[2];
                fTeach->edtInArmAlignmentPick3_Z->Text = iZPos[4];
                fTeach->edtInArmAlignmentPick4_Z->Text = iZPos[6];
                fTeach->edtInArmAlignmentPick5_Z->Text = iZPos[1];
                fTeach->edtInArmAlignmentPick6_Z->Text = iZPos[3];
                fTeach->edtInArmAlignmentPick7_Z->Text = iZPos[5];
                fTeach->edtInArmAlignmentPick8_Z->Text = iZPos[7];

                fTeach->edtInArmAlignmentPitch1_X->Text = abs(iXPos[0] - iXPos[2]);
                fTeach->edtInArmAlignmentPitch2_X->Text = abs(iXPos[2] - iXPos[4]);
                fTeach->edtInArmAlignmentPitch3_X->Text = abs(iXPos[4] - iXPos[6]);

                //KenHsieh 20211018
                //==>
                fTeach->edtInArmAlignmentPitch4_X->Text = abs(iXPos[1] - iXPos[3]);
                fTeach->edtInArmAlignmentPitch5_X->Text = abs(iXPos[3] - iXPos[5]);
                fTeach->edtInArmAlignmentPitch6_X->Text = abs(iXPos[5] - iXPos[7]);
                fTeach->edtInArmAlignmentPitch_Y->Text  = abs(iYPos[4] - iYPos[5]);
                //<==
                //KenHsieh 20211018                                                    */

                fTeach->edtInArmXAlignAa->Text=Tech.iInArmXCCDAlignmentPos - iXPos[0];  //KenHsieh 20260116 : 修改AOA 未寫入值之問題
                fTeach->edtInArmXAlignAb->Text=Tech.iInArmXCCDAlignmentPos - iXPos[2];
                fTeach->edtInArmXAlignAc->Text=Tech.iInArmXCCDAlignmentPos - iXPos[4];
                fTeach->edtInArmXAlignAd->Text=Tech.iInArmXCCDAlignmentPos - iXPos[6];
                fTeach->edtInArmXAlignBa->Text=Tech.iInArmXCCDAlignmentPos - iXPos[1];
                fTeach->edtInArmXAlignBb->Text=Tech.iInArmXCCDAlignmentPos - iXPos[3];
                fTeach->edtInArmXAlignBc->Text=Tech.iInArmXCCDAlignmentPos - iXPos[5];
                fTeach->edtInArmXAlignBd->Text=Tech.iInArmXCCDAlignmentPos - iXPos[7];

                fTeach->edtInArmYAlignAa->Text=Tech.iInArmYCCDAlignmentPos - iYPos[0];
                fTeach->edtInArmYAlignAb->Text=Tech.iInArmYCCDAlignmentPos - iYPos[2];
                fTeach->edtInArmYAlignAc->Text=Tech.iInArmYCCDAlignmentPos - iYPos[4];
                fTeach->edtInArmYAlignAd->Text=Tech.iInArmYCCDAlignmentPos - iYPos[6];
                fTeach->edtInArmYAlignBa->Text=Tech.iInArmYCCDAlignmentPos - iYPos[1];
                fTeach->edtInArmYAlignBb->Text=Tech.iInArmYCCDAlignmentPos - iYPos[3];
                fTeach->edtInArmYAlignBc->Text=Tech.iInArmYCCDAlignmentPos - iYPos[5];
                fTeach->edtInArmYAlignBd->Text=Tech.iInArmYCCDAlignmentPos - iYPos[7];

                fTeach->edtInArmZAlignAa->Text=iZPos[0];
                fTeach->edtInArmZAlignAb->Text=iZPos[2];
                fTeach->edtInArmZAlignAc->Text=iZPos[4];
                fTeach->edtInArmZAlignAd->Text=iZPos[6];
                fTeach->edtInArmZAlignBa->Text=iZPos[1];
                fTeach->edtInArmZAlignBb->Text=iZPos[3];
                fTeach->edtInArmZAlignBc->Text=iZPos[5];
                fTeach->edtInArmZAlignBd->Text=iZPos[7];

                fTeach->edtInAlignPitchXAa->Text=abs(iXPos[0] - iXPos[2]);
                fTeach->edtInAlignPitchXAb->Text=abs(iXPos[2] - iXPos[4]);
                fTeach->edtInAlignPitchXAc->Text=abs(iXPos[4] - iXPos[6]);

                fTeach->edtInAlignPitchXBa->Text=abs(iXPos[1] - iXPos[3]);
                fTeach->edtInAlignPitchXBb->Text=abs(iXPos[3] - iXPos[5]);
                fTeach->edtInAlignPitchXBc->Text=abs(iXPos[5] - iXPos[7]);
                fTeach->edtInArmAlignPitchY->Text=abs(iYPos[4] - iYPos[5]);

                //KenHsieh 20210829 : Modify add CCD AUTO ALIGNMENT (XY變距)
                //==>
                if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable)
                {
                    fTeach->edtInArmXBasePickerPos->Text = iXPos[5];
                    fTeach->edtInArmYBasePickerPos->Text = iYPos[5];
                }
                else
                {
                    fTeach->edtInArmXBasePickerPos->Text = iXPos[4];
                    fTeach->edtInArmYBasePickerPos->Text = iYPos[4];
                }
                //<==
                //KenHsieh 20210829 : Modify add CCD AUTO ALIGNMENT (XY變距)

                Teach_AutoAlignmentUnit=0;

                ShowMyMessage("Auto Alignment In Pitch Teach Finish");
                Task=1;
            }
            break;
        case 4000:
            if(DoOutArmXYZHome())
            {
                Task=4100;
            }
            break;
        case 4100:
//            if(USE_IN_OUT_ARM_Y_PITCH==iXPitchManual635 ||
//               USE_IN_OUT_ARM_Y_PITCH==iXPitchManual360)                        //KenHsieh 20210823 : Modify add CCD AUTO ALIGNMENT (XY變距)
//                iRet=DoOutArmPickerXYAndPitchAutoAlignment(4, true, iXPos, iYPos, iZPos);
//            else if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable)
//                iRet=DoOutArmPickerXYAndPitchAutoAlignment(3, true, iXPos, iYPos, iZPos);

            if(USE_PICKER_COUNT==ep1Picker)
            {
                iPicker=0;
            }
            else if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker ||
                    USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)
            {
                iPicker=iOutArmXBase+iOutArmYBase*8;
            }
            else
            {
                iPicker=iOutArmXBase*2+iOutArmYBase;                            //KenHsieh 20260116 : Fix AOA 基準吸嘴for Func. 0 2 4 6
            }
                                                                   //                                              1 3 5 7
            iRet=DoOutArmPickerXYAndPitchAutoAlignment(iPicker, true, iXPos, iYPos, iZPos);

            if(iRet==1)
            {
     /*           fTeach->edtOutArmAlignmentPick1_X->Text = Tech.iOutArmXCCDAlignmentPos - iXPos[0];
                fTeach->edtOutArmAlignmentPick2_X->Text = Tech.iOutArmXCCDAlignmentPos - iXPos[2];
                fTeach->edtOutArmAlignmentPick3_X->Text = Tech.iOutArmXCCDAlignmentPos - iXPos[4];
                fTeach->edtOutArmAlignmentPick4_X->Text = Tech.iOutArmXCCDAlignmentPos - iXPos[6];
                fTeach->edtOutArmAlignmentPick5_X->Text = Tech.iOutArmXCCDAlignmentPos - iXPos[1];
                fTeach->edtOutArmAlignmentPick6_X->Text = Tech.iOutArmXCCDAlignmentPos - iXPos[3];
                fTeach->edtOutArmAlignmentPick7_X->Text = Tech.iOutArmXCCDAlignmentPos - iXPos[5];
                fTeach->edtOutArmAlignmentPick8_X->Text = Tech.iOutArmXCCDAlignmentPos - iXPos[7];

                fTeach->edtOutArmAlignmentPick1_Y->Text = Tech.iOutArmYCCDAlignmentPos - iYPos[0];
                fTeach->edtOutArmAlignmentPick2_Y->Text = Tech.iOutArmYCCDAlignmentPos - iYPos[2];
                fTeach->edtOutArmAlignmentPick3_Y->Text = Tech.iOutArmYCCDAlignmentPos - iYPos[4];
                fTeach->edtOutArmAlignmentPick4_Y->Text = Tech.iOutArmYCCDAlignmentPos - iYPos[6];
                fTeach->edtOutArmAlignmentPick5_Y->Text = Tech.iOutArmYCCDAlignmentPos - iYPos[1];
                fTeach->edtOutArmAlignmentPick6_Y->Text = Tech.iOutArmYCCDAlignmentPos - iYPos[3];
                fTeach->edtOutArmAlignmentPick7_Y->Text = Tech.iOutArmYCCDAlignmentPos - iYPos[5];
                fTeach->edtOutArmAlignmentPick8_Y->Text = Tech.iOutArmYCCDAlignmentPos - iYPos[7];

                fTeach->edtOutArmAlignmentPick1_Z->Text = iZPos[0];
                fTeach->edtOutArmAlignmentPick2_Z->Text = iZPos[2];
                fTeach->edtOutArmAlignmentPick3_Z->Text = iZPos[4];
                fTeach->edtOutArmAlignmentPick4_Z->Text = iZPos[6];
                fTeach->edtOutArmAlignmentPick5_Z->Text = iZPos[1];
                fTeach->edtOutArmAlignmentPick6_Z->Text = iZPos[3];
                fTeach->edtOutArmAlignmentPick7_Z->Text = iZPos[5];
                fTeach->edtOutArmAlignmentPick8_Z->Text = iZPos[7];

                fTeach->edtOutArmAlignmentPitch1_X->Text = abs(iXPos[0] - iXPos[2]);
                fTeach->edtOutArmAlignmentPitch2_X->Text = abs(iXPos[2] - iXPos[4]);
                fTeach->edtOutArmAlignmentPitch3_X->Text = abs(iXPos[4] - iXPos[6]);

                //KenHsieh 20211018
                //==>
                fTeach->edtOutArmAlignmentPitch4_X->Text = abs(iXPos[1] - iXPos[3]);
                fTeach->edtOutArmAlignmentPitch5_X->Text = abs(iXPos[3] - iXPos[5]);
                fTeach->edtOutArmAlignmentPitch6_X->Text = abs(iXPos[5] - iXPos[7]);
                //<==
                //KenHsieh 20211018

                //KenHsieh 20210829 : Modify add CCD AUTO ALIGNMENT (XY變距)
                //==>
                if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable)
                {
                    fTeach->edtOutArmXBasePickerPos->Text    = iXPos[3];
                    fTeach->edtOutArmYBasePickerPos->Text    = iYPos[3];
                    fTeach->edtOutArmAlignmentPitch_Y->Text  = abs(iYPos[2] - iYPos[3]);    //KenHsieh 20211111 : 修改Pitch Y的Teach紀錄
                }
                else
                {
                    fTeach->edtOutArmXBasePickerPos->Text    = iXPos[4];
                    fTeach->edtOutArmYBasePickerPos->Text    = iYPos[4];
                    fTeach->edtOutArmAlignmentPitch_Y->Text  = abs(iYPos[4] - iYPos[5]);    //KenHsieh 20211111 : 修改Pitch Y的Teach紀錄
                }
                //<==
                //KenHsieh 20210829 : Modify add CCD AUTO ALIGNMENT (XY變距)
                                                                                   */

                fTeach->edtOutArmXAlignAa->Text=Tech.iOutArmXCCDAlignmentPos - iXPos[0];    //KenHsieh 20260116 : 修改AOA 未寫入值之問題
                fTeach->edtOutArmXAlignAb->Text=Tech.iOutArmXCCDAlignmentPos - iXPos[2];
                fTeach->edtOutArmXAlignAc->Text=Tech.iOutArmXCCDAlignmentPos - iXPos[4];
                fTeach->edtOutArmXAlignAd->Text=Tech.iOutArmXCCDAlignmentPos - iXPos[6];
                fTeach->edtOutArmXAlignBa->Text=Tech.iOutArmXCCDAlignmentPos - iXPos[1];
                fTeach->edtOutArmXAlignBb->Text=Tech.iOutArmXCCDAlignmentPos - iXPos[3];
                fTeach->edtOutArmXAlignBc->Text=Tech.iOutArmXCCDAlignmentPos - iXPos[5];
                fTeach->edtOutArmXAlignBd->Text=Tech.iOutArmXCCDAlignmentPos - iXPos[7];

                fTeach->edtOutArmYAlignAa->Text=Tech.iOutArmYCCDAlignmentPos - iYPos[0];
                fTeach->edtOutArmYAlignAb->Text=Tech.iOutArmYCCDAlignmentPos - iYPos[2];
                fTeach->edtOutArmYAlignAc->Text=Tech.iOutArmYCCDAlignmentPos - iYPos[4];
                fTeach->edtOutArmYAlignAd->Text=Tech.iOutArmYCCDAlignmentPos - iYPos[6];
                fTeach->edtOutArmYAlignBa->Text=Tech.iOutArmYCCDAlignmentPos - iYPos[1];
                fTeach->edtOutArmYAlignBb->Text=Tech.iOutArmYCCDAlignmentPos - iYPos[3];
                fTeach->edtOutArmYAlignBc->Text=Tech.iOutArmYCCDAlignmentPos - iYPos[5];
                fTeach->edtOutArmYAlignBd->Text=Tech.iOutArmYCCDAlignmentPos - iYPos[7];

                fTeach->edtOutArmZAlignAa->Text=iZPos[0];
                fTeach->edtOutArmZAlignAb->Text=iZPos[2];
                fTeach->edtOutArmZAlignAc->Text=iZPos[4];
                fTeach->edtOutArmZAlignAd->Text=iZPos[6];
                fTeach->edtOutArmZAlignBa->Text=iZPos[1];
                fTeach->edtOutArmZAlignBb->Text=iZPos[3];
                fTeach->edtOutArmZAlignBc->Text=iZPos[5];
                fTeach->edtOutArmZAlignBd->Text=iZPos[7];

                fTeach->edtOutAlignPitchXAa->Text=abs(iXPos[0] - iXPos[2]);
                fTeach->edtOutAlignPitchXAb->Text=abs(iXPos[2] - iXPos[4]);
                fTeach->edtOutAlignPitchXAc->Text=abs(iXPos[4] - iXPos[6]);

                fTeach->edtOutAlignPitchXBa->Text=abs(iXPos[1] - iXPos[3]);
                fTeach->edtOutAlignPitchXBb->Text=abs(iXPos[3] - iXPos[5]);
                fTeach->edtOutAlignPitchXBc->Text=abs(iXPos[5] - iXPos[7]);

                if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable)                    //KenHsieh 20210829 : Modify add CCD AUTO ALIGNMENT (XY變距)
                {
                    fTeach->edtOutArmXBasePickerPos->Text=iXPos[3];
                    fTeach->edtOutArmYBasePickerPos->Text=iYPos[3];
                    fTeach->edtOutArmAlignPitchY->Text=abs(iYPos[2] - iYPos[3]);//KenHsieh 20211111 : 修改Pitch Y的Teach紀錄
                }
                else
                {
                    fTeach->edtOutArmXBasePickerPos->Text=iXPos[4];
                    fTeach->edtOutArmYBasePickerPos->Text=iYPos[4];
                    fTeach->edtOutArmAlignPitchY->Text=abs(iYPos[4] - iYPos[5]);//KenHsieh 20211111 : 修改Pitch Y的Teach紀錄
                }

                Teach_AutoAlignmentUnit=0;
                ShowMyMessage("Auto Alignment Out Pitch Teach Finish");
                Task=1;
            }
            break;
        case 5000:
            if(DoAutoRunLoaderTray(0))
            {
                Teach_AutoAlignmentUnit=0;
                Task=1;
            }
            break;
         case 6000:     //KenHsieh 20211023 : 新增X Pitch 40mm teach
            if(DoInArmXYZHome())
            {
                Task=6100;
            }
            break;
        case 6100:
//            if(USE_IN_OUT_ARM_Y_PITCH==iXPitchManual635 || USE_IN_OUT_ARM_Y_PITCH==iXPitchManual360)
//                iRet=DoInArmPickerXYAndPitchAutoAlignment(4, true, iXPos, iYPos, iZPos);
//            else if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable)
//                iRet=DoInArmPickerXYAndPitchAutoAlignment(5, true, iXPos, iYPos, iZPos);

            if(USE_PICKER_COUNT==ep1Picker)
            {
                iPicker=0;
            }
            else if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker ||
                    USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)
            {
                iPicker=iInArmXBase+iInArmYBase*8;
            }
            else
            {
                iPicker=iInArmXBase+iInArmYBase*4;
            }
            iRet=DoInArmPickerXYAndPitchAutoAlignment(iPicker, true, iXPos, iYPos, iZPos);

            if(iRet==1)
            {
//                fTeach->edtInArmAlignmentPitch1_X_40mm->Text = abs(iXPos[0] - iXPos[2]);
//                fTeach->edtInArmAlignmentPitch2_X_40mm->Text = abs(iXPos[2] - iXPos[4]);
//                fTeach->edtInArmAlignmentPitch3_X_40mm->Text = abs(iXPos[4] - iXPos[6]);
//                fTeach->edtInArmAlignmentPitch4_X_40mm->Text = abs(iXPos[1] - iXPos[3]);
//                fTeach->edtInArmAlignmentPitch5_X_40mm->Text = abs(iXPos[3] - iXPos[5]);
//                fTeach->edtInArmAlignmentPitch6_X_40mm->Text = abs(iXPos[5] - iXPos[7]);

                Teach_AutoAlignmentUnit=0;
                bInArmXPitch_40mm=false;    //KenHsieh 20211023 : 新增X Pitch 40mm teach
                ShowMyMessage("Auto Alignment In Pitch Teach Finish");
                Task=1;
            }
            break;
        case 7000:      //KenHsieh 20211023 : 新增X Pitch 40mm teach
            if(DoOutArmXYZHome())
            {
                Task=7100;
            }
            break;
        case 7100:
//            if(USE_IN_OUT_ARM_Y_PITCH==iXPitchManual635 || USE_IN_OUT_ARM_Y_PITCH==iXPitchManual360)
//                iRet=DoOutArmPickerXYAndPitchAutoAlignment(4, true, iXPos, iYPos, iZPos);
//            else if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable)
//                iRet=DoOutArmPickerXYAndPitchAutoAlignment(3, true, iXPos, iYPos, iZPos);

            if(USE_PICKER_COUNT==ep1Picker)
            {
                iPicker=0;
            }
            else if(USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Picker ||
                    USE_IN_OUT_ARM_Y_PITCH==iXYPitch16Bd_Be)
            {
                iPicker=iOutArmXBase+iOutArmYBase*8;
            }
            else
            {
                iPicker=iOutArmXBase+iOutArmYBase*4;
            }
            iRet=DoOutArmPickerXYAndPitchAutoAlignment(iPicker, true, iXPos, iYPos, iZPos);

            if(iRet==1)
            {
//                fTeach->edtOutArmAlignmentPitch1_X_40mm->Text = abs(iXPos[0] - iXPos[2]);
//                fTeach->edtOutArmAlignmentPitch2_X_40mm->Text = abs(iXPos[2] - iXPos[4]);
//                fTeach->edtOutArmAlignmentPitch3_X_40mm->Text = abs(iXPos[4] - iXPos[6]);
//                fTeach->edtOutArmAlignmentPitch4_X_40mm->Text = abs(iXPos[1] - iXPos[3]);
//                fTeach->edtOutArmAlignmentPitch5_X_40mm->Text = abs(iXPos[3] - iXPos[5]);
//                fTeach->edtOutArmAlignmentPitch6_X_40mm->Text = abs(iXPos[5] - iXPos[7]);

                Teach_AutoAlignmentUnit=0;
                bOutArmXPitch_40mm=false;                                       //KenHsieh 20211023 : 新增X Pitch 40mm teach
                ShowMyMessage("Auto Alignment Out Pitch Teach Finish");
                Task=1;
            }
            break;
        case 11000:
            if(DoInArmXYAlignmentTeachProcess(AOA_Loader, false))
            {
                Teach_AutoAlignmentUnit=0;
                Task=1;
            }
            break;
        case 20000:
            if(DoAutoRunUnloaderTray(AOA_InSht1))
            {
               Teach_AutoAlignmentUnit=0;
               Task=1;
            }
            break;
        case 21000:
            if(DoOutArmXYAlignmentTeachProcess(AOA_Fix1, false))
            {
                Teach_AutoAlignmentUnit=0;
                Task=1;
            }
            break;
    }
}
//------------------------------------------------------------------------------
//KenHsieh 20210813 : add CCD AUTO ALIGNMENT
//==>
//ChungHung 20210113 add for Alignment CCD start
//------------------------------------------------------------------------------
extern void SetAOATrayTeachPoint(int &iCol, int &iRow, TTrayMotor *myTray, bool bTraySkipLast=false);   //ChungHung 20210113 add for Alignment CCD
void SetAOATrayTeachPoint(int &iCol, int &iRow, TTrayMotor *myTray, bool bTraySkipLast)     //KenHsieh 20210902 : Modify add CCD AUTO ALIGNMENT
{
    int iXLeft = 0;
    int iXRight = 0;
    int iYTop = 0;
    int iYDown = 0;
    int iDisX = 0;
    int iDisY = 0;
    int iPointX = 0;
    int iPointY = 0;
    int iMaxItemX = 0;
    int iMaxItemY = 0;
    int iPosX = 0;
    int iPosY = 0;

    myTray->fHasTray = true;

    for(int ix=0;ix<myTray->Tray.XItem;ix++)
    {
        for(int iy=0;iy<myTray->Tray.YItem;iy++)
        {
            if(myTray->Tray.Data[ix][iy] == WAIT_ALIGN_IC)
            {
                iXLeft = ix;
                iYTop = iy;
                ix = myTray->Tray.XItem;
                iy = myTray->Tray.YItem;
                break;
            }
        }
    }

    for(int ix=myTray->Tray.XItem;ix>=0;ix--)
    {
        for(int iy=myTray->Tray.YItem;iy>=0;iy--)
        {
            if(myTray->Tray.Data[ix][iy] == WAIT_ALIGN_IC)
            {
                iXRight = ix;
                iYDown = iy;
                ix = -1;
                iy = -1;
                break;
            }
        }
    }

    for(int ix=0;ix<myTray->Tray.XItem;ix++)
    {
        for(int iy=0;iy<myTray->Tray.YItem;iy++)
        {
            myTray->SetTraySingleData(ix, iy, NULL_IC);
        }
    }

    iPointX = iCol;
    iPointY = iRow;
    iMaxItemX = iXRight - iXLeft + 1;
    iMaxItemY = iYDown - iYTop + 1;

    if(iCol<2)
    {
        iCol = 2;
    }

    if(iCol>iMaxItemX)
    {
        iCol = iMaxItemX;
    }

    if(iRow<2)
    {
        iRow = 2;
    }

    if(iRow > iMaxItemY)
    {
        iRow = iMaxItemY;
    }

    iPointX = iCol;
    iPointY = iRow;

    if(iPointX-1<=0)
    {
        iDisX = 0;
    }
    else
    {
        iDisX = (iMaxItemX/(iPointX-1));
    }

    if(iPointY-1<=0)
    {
        iDisY=0;
    }
    else
    {
        iDisY = (iMaxItemY/(iPointY-1));
    }

    for(int ix=0;ix<iPointX;ix++)
    {
        for(int iy=0;iy<iPointY;iy++)
        {
            if(iXLeft==0 && iYTop==0)
            {
                iPosX = ix*iDisX;
                iPosY = iy*iDisY;
                if( ix>=iPointX-1 || iPosX>=iMaxItemX)
                {
                    iPosX = iMaxItemX-1;
                    iPosX = iPosX + iXLeft;
                }

                if( iy>=iPointY-1 || iPosY>=iMaxItemY)
                {
                    iPosY = iMaxItemY-1;
                    iPosY = iPosY + iYTop;
                }
            }
            else        //KenHsieh 20210909 : 修改AOA掃描方式
            {
                iPosX = iXLeft + ix*iDisX;
                iPosY = iYTop + iy*iDisY;
                if( ix>=iPointX-1 || iPosX>=myTray->Tray.XItem)
                {
                    iPosX = iMaxItemX-1;
                    iPosX = iPosX + iXLeft;
                }

                if( iy>=iPointY-1 || iPosY>=myTray->Tray.YItem)
                {
                    iPosY = iMaxItemY-1;
                    iPosY = iPosY + iYTop;
                }
            }
            myTray->SetTraySingleData(iPosX, iPosY, WAIT_ALIGN_IC);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::btnAutoAlignmentTrayPointCalClick(
      TObject *Sender)
{
    int iCol = atoi(edtAlignmentPointX->Text.c_str());
    int iRow = atoi(edtAlignmentPointY->Text.c_str());

    myAlignmentTray->SetColorMap(WAIT_ALIGN_IC, (TColor)0x00000000);            //00V_41 //Mylin 20210504 Modify add for Alignment CCD      //KenHsieh 20210902 : Modify add CCD AUTO ALIGNMENT
    MOT[MMAOASampleTray].fHasTray=true;                                         //Mylin 20210504 Modify add for Alignment CCD               //KenHsieh 20210902 : Modify add CCD AUTO ALIGNMENT
    MOT[MMAOASampleTray].InitNewTray(WAIT_ALIGN_IC, false, __FUNC__);           //Mylin 20210504 Modify add for Alignment CCD               //KenHsieh 20210902 : Modify add CCD AUTO ALIGNMENT
    SetAOATrayTeachPoint(iCol, iRow, &MOT[MMAOASampleTray], true);              //ChungHung 20210113 add for Alignment CCD

    edtAlignmentPointX->Text = iCol;
    edtAlignmentPointY->Text = iRow;
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::btnAutoAlignmentPlatePointCalClick(
      TObject *Sender)
{
    int iCol = atoi(edtAlignmentPlatePointX->Text.c_str());
    int iRow = atoi(edtAlignmentPlatePointY->Text.c_str());

    myAlignmentPlate->SetColorMap(WAIT_ALIGN_IC,(TColor)0x00000000);            //00V_41 //Mylin 20210504 Modify add for Alignment CCD      //KenHsieh 20211013 : Modify Hotplate Scan
    MOT[MMAOASamplePlate].fHasTray=true;                                        //Mylin 20210504 Modify add for Alignment CCD               //KenHsieh 20211013 : Modify Hotplate Scan
    MOT[MMAOASamplePlate].InitNewTray(WAIT_ALIGN_IC, false, __FUNC__);          //Mylin 20210504 Modify add for Alignment CCD               //KenHsieh 20211013 : Modify Hotplate Scan
    SetAOATrayTeachPoint(iCol, iRow, &MOT[MMAOASamplePlate]);                   //ChungHung 20210113 add for Alignment CCD

    edtAlignmentPlatePointX->Text=iCol;
    edtAlignmentPlatePointY->Text=iRow;
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::cbAutoAlignmentTray_AfterHomeClick(
      TObject *Sender)
{
    if(cbAutoAlignmentTray_ChangeSetup->Checked==true)
    {
        cbAutoAlignmentTray_ChangeSetup_Z->Enabled=true;
    }
    else if(cbAutoAlignmentTray_ChangeSetup->Checked==false)
    {
        cbAutoAlignmentTray_ChangeSetup_Z->Enabled=false;
        cbAutoAlignmentTray_ChangeSetup_Z->Checked=false;
    }

    if(cbAutoAlignmentTray_AfterHome->Checked==true)
    {
        cbAutoAlignmentTray_AfterHome_Z->Enabled=true;
    }
    else if(cbAutoAlignmentTray_AfterHome->Checked==false)
    {
        cbAutoAlignmentTray_AfterHome_Z->Enabled=false;
        cbAutoAlignmentTray_AfterHome_Z->Checked=false;
    }

    if(cbAutoAlignmentTray_InitialStart->Checked==true)
    {
        cbAutoAlignmentTray_InitialStart_Z->Enabled=true;
    }
    else if(cbAutoAlignmentTray_InitialStart->Checked==false)
    {
        cbAutoAlignmentTray_InitialStart_Z->Enabled=false;
        cbAutoAlignmentTray_InitialStart_Z->Checked=false;
    }

    if(cbAutoAlignmentTray_EveryTray->Checked==true)
    {
        cbAutoAlignmentTray_EveryTray_Z->Enabled=true;
    }
    else if(cbAutoAlignmentTray_EveryTray->Checked==false)
    {
        cbAutoAlignmentTray_EveryTray_Z->Enabled=false;
        cbAutoAlignmentTray_EveryTray_Z->Checked=false;
    }

    if(cbAutoAlignmentTray_OneCycle->Checked==true)
    {
        cbAutoAlignmentTray_OneCycle_Z->Enabled=true;
    }
    else if(cbAutoAlignmentTray_OneCycle->Checked==false)
    {
        cbAutoAlignmentTray_OneCycle_Z->Enabled=false;
        cbAutoAlignmentTray_OneCycle_Z->Checked=false;
    }

    if(cbAutoAlignmentCK_ChangeSetup->Checked==true)
    {
        cbAutoAlignmentCK_ChangeSetup_Z->Enabled=true;
    }
    else if(cbAutoAlignmentCK_ChangeSetup->Checked==false)
    {
        cbAutoAlignmentCK_ChangeSetup_Z->Enabled=false;
        cbAutoAlignmentCK_ChangeSetup_Z->Checked=false;
    }

    if(cbAutoAlignmentCK_AfterHome->Checked==true)
    {
        cbAutoAlignmentCK_AfterHome_Z->Enabled=true;
    }
    else if(cbAutoAlignmentCK_AfterHome->Checked==false)
    {
        cbAutoAlignmentCK_AfterHome_Z->Enabled=false;
        cbAutoAlignmentCK_AfterHome_Z->Checked=false;
    }

    if(cbAutoAlignmentCK_InitialStart->Checked==true)
    {
        cbAutoAlignmentCK_InitialStart_Z->Enabled=true;
    }
    else if(cbAutoAlignmentCK_InitialStart->Checked==false)
    {
        cbAutoAlignmentCK_InitialStart_Z->Enabled=false;
        cbAutoAlignmentCK_InitialStart_Z->Checked=false;
    }

    if(cbAutoAlignmentCK_OneCycle->Checked==true)
    {
        cbAutoAlignmentCK_OneCycle_Z->Enabled=true;
    }
    else if(cbAutoAlignmentCK_OneCycle->Checked==false)
    {
        cbAutoAlignmentCK_OneCycle_Z->Enabled=false;
        cbAutoAlignmentCK_OneCycle_Z->Checked=false;
    }
}
//---------------------------------------------------------------------------
void CheckAutoAlignmentEvent()
{
    TestIF_File.iAutoAlignmentTrayEvent=0;
    if(fAutoAlignment->cbAutoAlignmentTray_ChangeSetup->Checked==true)
        TestIF_File.iAutoAlignmentTrayEvent = TestIF_File.iAutoAlignmentTrayEvent | AutoAlignmentTray_ChangeSetupFile;
    if(fAutoAlignment->cbAutoAlignmentTray_AfterHome->Checked==true)
        TestIF_File.iAutoAlignmentTrayEvent = TestIF_File.iAutoAlignmentTrayEvent | AutoAlignmentTray_AfterHome;
    if(fAutoAlignment->cbAutoAlignmentTray_InitialStart->Checked==true)
        TestIF_File.iAutoAlignmentTrayEvent = TestIF_File.iAutoAlignmentTrayEvent | AutoAlignmentTray_InitialStart;
    if(fAutoAlignment->cbAutoAlignmentTray_EveryTray->Checked==true)
        TestIF_File.iAutoAlignmentTrayEvent = TestIF_File.iAutoAlignmentTrayEvent | AutoAlignmentTray_EveryTray;
    if(fAutoAlignment->cbAutoAlignmentTray_OneCycle->Checked==true)
        TestIF_File.iAutoAlignmentTrayEvent = TestIF_File.iAutoAlignmentTrayEvent | AutoAlignmentTray_AfterOneCycle;

    TestIF_File.iAutoAlignmentShuttleHotplateEvent=0;
    if(fAutoAlignment->cbAutoAlignmentCK_ChangeSetup->Checked==true)
        TestIF_File.iAutoAlignmentShuttleHotplateEvent = TestIF_File.iAutoAlignmentShuttleHotplateEvent | AutoAlignmentCK_ChangeSetupFile;
    if(fAutoAlignment->cbAutoAlignmentCK_AfterHome->Checked==true)
        TestIF_File.iAutoAlignmentShuttleHotplateEvent = TestIF_File.iAutoAlignmentShuttleHotplateEvent | AutoAlignmentCK_AfterHome;
    if(fAutoAlignment->cbAutoAlignmentCK_InitialStart->Checked==true)
        TestIF_File.iAutoAlignmentShuttleHotplateEvent = TestIF_File.iAutoAlignmentShuttleHotplateEvent | AutoAlignmentCK_InitialStart;
    if(fAutoAlignment->cbAutoAlignmentCK_OneCycle->Checked==true)
        TestIF_File.iAutoAlignmentShuttleHotplateEvent = TestIF_File.iAutoAlignmentShuttleHotplateEvent | AutoAlignmentCK_AfterOneCycle;

    //----------------------------------------------------------------------------------------------------------
    TestIF_File.iAutoAlignmentTrayEvent_Z=0;
    if(fAutoAlignment->cbAutoAlignmentTray_ChangeSetup_Z->Checked==true)
        TestIF_File.iAutoAlignmentTrayEvent_Z = TestIF_File.iAutoAlignmentTrayEvent_Z | AutoAlignmentTray_ChangeSetupFile;
    if(fAutoAlignment->cbAutoAlignmentTray_AfterHome_Z->Checked==true)
        TestIF_File.iAutoAlignmentTrayEvent_Z = TestIF_File.iAutoAlignmentTrayEvent_Z | AutoAlignmentTray_AfterHome;
    if(fAutoAlignment->cbAutoAlignmentTray_InitialStart_Z->Checked==true)
        TestIF_File.iAutoAlignmentTrayEvent_Z = TestIF_File.iAutoAlignmentTrayEvent_Z | AutoAlignmentTray_InitialStart;
    if(fAutoAlignment->cbAutoAlignmentTray_EveryTray_Z->Checked==true)
        TestIF_File.iAutoAlignmentTrayEvent_Z = TestIF_File.iAutoAlignmentTrayEvent_Z | AutoAlignmentTray_EveryTray;
    if(fAutoAlignment->cbAutoAlignmentTray_OneCycle_Z->Checked==true)
        TestIF_File.iAutoAlignmentTrayEvent_Z = TestIF_File.iAutoAlignmentTrayEvent_Z | AutoAlignmentTray_AfterOneCycle;

    TestIF_File.iAutoAlignmentShuttleHotplateEvent_Z=0;
    if(fAutoAlignment->cbAutoAlignmentCK_ChangeSetup_Z->Checked==true)
        TestIF_File.iAutoAlignmentShuttleHotplateEvent_Z  =  TestIF_File.iAutoAlignmentShuttleHotplateEvent_Z | AutoAlignmentCK_ChangeSetupFile;
    if(fAutoAlignment->cbAutoAlignmentCK_AfterHome_Z->Checked==true)
        TestIF_File.iAutoAlignmentShuttleHotplateEvent_Z  =  TestIF_File.iAutoAlignmentShuttleHotplateEvent_Z | AutoAlignmentCK_AfterHome;
    if(fAutoAlignment->cbAutoAlignmentCK_InitialStart_Z->Checked==true)
        TestIF_File.iAutoAlignmentShuttleHotplateEvent_Z =  TestIF_File.iAutoAlignmentShuttleHotplateEvent_Z | AutoAlignmentCK_InitialStart;
    if(fAutoAlignment->cbAutoAlignmentCK_OneCycle_Z->Checked==true)
        TestIF_File.iAutoAlignmentShuttleHotplateEvent_Z =  TestIF_File.iAutoAlignmentShuttleHotplateEvent_Z | AutoAlignmentCK_AfterOneCycle;
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::ReadFile()
{
    AnsiString szDir=GetRecipeFileName("HandlerCondition.Data");
    AnsiString sGroup;

    sGroup="AutoAlignmrnt";

    if(MACHINE_HAS_AUTO_ALIGNMENT_CCD==false)
    {
        TestIF_File.bEnableAutoAlignment=false;
        return;
    }

    TestIF_File.bEnableAutoAlignment                    =ReadIniData(szDir, sGroup, "Enable Auto Alignment",            false);
    TestIF_File.AutoAlignmentFileName                   =ReadIniData(szDir, sGroup, "AutoAlignment FileName",           fMain->cbSetupFileName->Text);     //Kenhsieh 20210922 : save AutoAlignment FileName
    TestIF_File.iAutoAlignmentTrayEvent                 =ReadIniData(szDir, sGroup, "AutoAlignmentTrayEvent",               0);
    TestIF_File.iAutoAlignmentShuttleHotplateEvent      =ReadIniData(szDir, sGroup, "AutoAlignmentShuttleHotplateEvent",    0);
    TestIF_File.iAutoAlignmentTrayEvent_Z               =ReadIniData(szDir, sGroup, "AutoAlignmentTrayEvent_Z",             0);
    TestIF_File.iAutoAlignmentShuttleHotplateEvent_Z    =ReadIniData(szDir, sGroup, "AutoAlignmentShuttleHotplateEvent_Z",  0);

    TestIF_File.iAlignmentPointX                        =ReadIniData(szDir, sGroup, "Alignment Point X",                    1);
    TestIF_File.iAlignmentPointY                        =ReadIniData(szDir, sGroup, "Alignment Point Y",                    1);

    TestIF_File.iAlignmentPlatePointX                   =ReadIniData(szDir, sGroup, "Alignment Plate Point X",              1);
    TestIF_File.iAlignmentPlatePointY                   =ReadIniData(szDir, sGroup, "Alignment Plate Point Y",              1);

    TestIF_File.iAutoAlignment_DeviceThick              =ReadIniData(szDir, sGroup, "Alignment DeviceThick",                0);

    TestIF_File.iAutoAlignmentTray_InArmZRealaseOffset  =ReadIniData(szDir, sGroup, "Alignment InArmZRealaseOffset",        0);
    TestIF_File.iAutoAlignmentTray_InArmZPickUpOffset   =ReadIniData(szDir, sGroup, "Alignment InArmZPickUpOffset",         0);

    TestIF_File.iAutoAlignmentTray_OutArmZRealaseOffset =ReadIniData(szDir, sGroup, "Alignment OutArmZRealaseOffset",       0);
    TestIF_File.iAutoAlignmentTray_OutArmZPickUpOffset  =ReadIniData(szDir, sGroup, "Alignment OutArmZPickUpOffset",        0);

    TestIF_File.iAutoAlignmentCK_InArmZRealaseOffset    =ReadIniData(szDir, sGroup, "Alignment InArmZRealaseOffset",        0);
    TestIF_File.iAutoAlignmentCK_InArmZPickUpOffset     =ReadIniData(szDir, sGroup, "Alignment InArmZPickUpOffset",         0);

    TestIF_File.iAutoAlignmentCK_OutArmZRealaseOffset   =ReadIniData(szDir, sGroup, "Alignment OutArmZRealaseOffset",       0);
    TestIF_File.iAutoAlignmentCK_OutArmZPickUpOffset    =ReadIniData(szDir, sGroup, "Alignment OutArmZPickUpOffset",        0);

    TestIF_File.iAutoAlignment_UseFix[iAutoIndex[eFix1]]=ReadIniData(szDir, sGroup, "Alignment Use Fix1",               false);
    TestIF_File.iAutoAlignment_UseFix[iAutoIndex[eFix2]]=ReadIniData(szDir, sGroup, "Alignment Use Fix2",               false);
    TestIF_File.iAutoAlignment_UseFix[iAutoIndex[eFix3]]=ReadIniData(szDir, sGroup, "Alignment Use Fix3",               false);

    if(AUTO_EMPTY_COLOR>=3)                                                     //Steven 20240428 : Add for HT9011 AOA
    {
        TestIF_File.iAutoAlignment_UseFix[iAutoIndex[eFix4]]=ReadIniData(szDir, sGroup, "Alignment Use Fix4",           false);
        TestIF_File.iAutoAlignment_UseFix[iAutoIndex[eFix5]]=ReadIniData(szDir, sGroup, "Alignment Use Fix5",           false);
        TestIF_File.iAutoAlignment_UseFix[iAutoIndex[eFix6]]=ReadIniData(szDir, sGroup, "Alignment Use Fix6",           false);
    }
    else
    {
        TestIF_File.iAutoAlignment_UseFix[iAutoIndex[eFix4]]=false;
        TestIF_File.iAutoAlignment_UseFix[iAutoIndex[eFix5]]=false;
        TestIF_File.iAutoAlignment_UseFix[iAutoIndex[eFix6]]=false;
    }

    TestIF_File.iAOA_DecodeTimeOut                      =ReadIniData(szDir, sGroup, "Alignment Decode Time Out",         3000);   //Ifor 20211221 add: AOA Decode Time Out
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::DoIniDataToForm()
{
    cbEnabledAutoAlignment->Checked = TestIF_File.bEnableAutoAlignment;

    edtFileName->Text      = TestIF_File.AutoAlignmentFileName;                 //Kenhsieh 20210922 : save AutoAlignment FileName

    TestIF_File.iAutoAlignmentTrayEvent = TestIF_File.iAutoAlignmentTrayEvent | AutoAlignmentTray_AfterHome;

    //cbAutoAlignmentTrayEvent->ItemIndex = TestIF.iAutoAlignmentTrayEvent;             //0:Initial Start 1:After Home 2:Every Tray
    cbAutoAlignmentTray_ChangeSetup->Checked    = TestIF_File.iAutoAlignmentTrayEvent & AutoAlignmentTray_ChangeSetupFile;
    cbAutoAlignmentTray_AfterHome->Checked      = TestIF_File.iAutoAlignmentTrayEvent & AutoAlignmentTray_AfterHome;
    cbAutoAlignmentTray_InitialStart->Checked   = TestIF_File.iAutoAlignmentTrayEvent & AutoAlignmentTray_InitialStart;
    cbAutoAlignmentTray_EveryTray->Checked      = TestIF_File.iAutoAlignmentTrayEvent & AutoAlignmentTray_EveryTray;
    cbAutoAlignmentTray_OneCycle->Checked       = TestIF_File.iAutoAlignmentTrayEvent & AutoAlignmentTray_AfterOneCycle;

    TestIF_File.iAutoAlignmentShuttleHotplateEvent = TestIF_File.iAutoAlignmentShuttleHotplateEvent | AutoAlignmentCK_AfterHome;

    //cbAutoAlignmentShuttleHotplateEvent->ItemIndex = TestIF.iAutoAlignmentShuttleHotplateEvent;  //0:Initial Start 1:After Home 2:Every Tray
    cbAutoAlignmentCK_ChangeSetup->Checked      =  TestIF_File.iAutoAlignmentShuttleHotplateEvent & AutoAlignmentCK_ChangeSetupFile;
    cbAutoAlignmentCK_AfterHome->Checked        =  TestIF_File.iAutoAlignmentShuttleHotplateEvent & AutoAlignmentCK_AfterHome;
    cbAutoAlignmentCK_InitialStart->Checked     =  TestIF_File.iAutoAlignmentShuttleHotplateEvent & AutoAlignmentCK_InitialStart;
    cbAutoAlignmentCK_OneCycle->Checked         =  TestIF_File.iAutoAlignmentShuttleHotplateEvent & AutoAlignmentCK_AfterOneCycle;

    //cbAutoAlignmentTrayEvent->ItemIndex = TestIF.iAutoAlignmentTrayEvent;             //0:Initial Start 1:After Home 2:Every Tray
    cbAutoAlignmentTray_ChangeSetup_Z->Checked    = TestIF_File.iAutoAlignmentTrayEvent_Z & AutoAlignmentTray_ChangeSetupFile;
    cbAutoAlignmentTray_AfterHome_Z->Checked      = TestIF_File.iAutoAlignmentTrayEvent_Z & AutoAlignmentTray_AfterHome;
    cbAutoAlignmentTray_InitialStart_Z->Checked   = TestIF_File.iAutoAlignmentTrayEvent_Z & AutoAlignmentTray_InitialStart;
    cbAutoAlignmentTray_EveryTray_Z->Checked      = TestIF_File.iAutoAlignmentTrayEvent_Z & AutoAlignmentTray_EveryTray;
    cbAutoAlignmentTray_OneCycle_Z->Checked       = TestIF_File.iAutoAlignmentTrayEvent_Z & AutoAlignmentTray_AfterOneCycle;

    //cbAutoAlignmentShuttleHotplateEvent->ItemIndex = TestIF.iAutoAlignmentShuttleHotplateEvent;  //0:Initial Start 1:After Home 2:Every Tray
    cbAutoAlignmentCK_ChangeSetup_Z->Checked      =  TestIF_File.iAutoAlignmentShuttleHotplateEvent_Z & AutoAlignmentCK_ChangeSetupFile;
    cbAutoAlignmentCK_AfterHome_Z->Checked        =  TestIF_File.iAutoAlignmentShuttleHotplateEvent_Z & AutoAlignmentCK_AfterHome;
    cbAutoAlignmentCK_InitialStart_Z->Checked     =  TestIF_File.iAutoAlignmentShuttleHotplateEvent_Z & AutoAlignmentCK_InitialStart;
    cbAutoAlignmentCK_OneCycle_Z->Checked         =  TestIF_File.iAutoAlignmentShuttleHotplateEvent_Z & AutoAlignmentCK_AfterOneCycle;

    edtAlignmentPointX->Text        = TestIF_File.iAlignmentPointX;             //Col
    edtAlignmentPointY->Text        = TestIF_File.iAlignmentPointY;             //Row

    edtAlignmentPlatePointX->Text   = TestIF_File.iAlignmentPlatePointX;        //Col
    edtAlignmentPlatePointY->Text   = TestIF_File.iAlignmentPlatePointY;        //Row

    if(cbAutoAlignmentTray_ChangeSetup->Checked==true)
        cbAutoAlignmentTray_ChangeSetup_Z->Enabled=true;
    else if(cbAutoAlignmentTray_ChangeSetup->Checked==false)
        cbAutoAlignmentTray_ChangeSetup_Z->Enabled=false;

    if(cbAutoAlignmentTray_AfterHome->Checked==true)
        cbAutoAlignmentTray_AfterHome_Z->Enabled=true;
    else if(cbAutoAlignmentTray_AfterHome->Checked==false)
        cbAutoAlignmentTray_AfterHome_Z->Enabled=false;

    if(cbAutoAlignmentTray_InitialStart->Checked==true)
        cbAutoAlignmentTray_InitialStart_Z->Enabled=true;
    else if(cbAutoAlignmentTray_InitialStart->Checked==false)
        cbAutoAlignmentTray_InitialStart_Z->Enabled=false;

    if(cbAutoAlignmentTray_EveryTray->Checked==true)
        cbAutoAlignmentTray_EveryTray_Z->Enabled=true;
    else if(cbAutoAlignmentTray_EveryTray->Checked==false)
        cbAutoAlignmentTray_EveryTray_Z->Enabled=false;

    if(cbAutoAlignmentTray_OneCycle->Checked==true)
        cbAutoAlignmentTray_OneCycle_Z->Enabled=true;
    else if(cbAutoAlignmentTray_OneCycle->Checked==false)
        cbAutoAlignmentTray_OneCycle_Z->Enabled=false;

    if(cbAutoAlignmentCK_ChangeSetup->Checked==true)
        cbAutoAlignmentCK_ChangeSetup_Z->Enabled=true;
    else if(cbAutoAlignmentCK_ChangeSetup->Checked==false)
        cbAutoAlignmentCK_ChangeSetup_Z->Enabled=false;

    if(cbAutoAlignmentCK_AfterHome->Checked==true)
        cbAutoAlignmentCK_AfterHome_Z->Enabled=true;
    else if(cbAutoAlignmentCK_AfterHome->Checked==false)
        cbAutoAlignmentCK_AfterHome_Z->Enabled=false;

    if(cbAutoAlignmentCK_InitialStart->Checked==true)
        cbAutoAlignmentCK_InitialStart_Z->Enabled=true;
    else if(cbAutoAlignmentCK_InitialStart->Checked==false)
        cbAutoAlignmentCK_InitialStart_Z->Enabled=false;

    if(cbAutoAlignmentCK_OneCycle->Checked==true)
        cbAutoAlignmentCK_OneCycle_Z->Enabled=true;
    else if(cbAutoAlignmentCK_OneCycle->Checked==false)
        cbAutoAlignmentCK_OneCycle_Z->Enabled=false;

    edAutoAlignment_DeviceThick->Text             = TestIF_File.iAutoAlignment_DeviceThick;

    edAutoAlignmentTray_InArmZRealaseOffset->Text = TestIF_File.iAutoAlignmentTray_InArmZRealaseOffset;
    edAutoAlignmentTray_InArmZPickUpOffset->Text  = TestIF_File.iAutoAlignmentTray_InArmZPickUpOffset;

    edAutoAlignmentTray_OutArmZRealaseOffset->Text= TestIF_File.iAutoAlignmentTray_OutArmZRealaseOffset;
    edAutoAlignmentTray_OutArmZPickUpOffset->Text = TestIF_File.iAutoAlignmentTray_OutArmZPickUpOffset;

    edAutoAlignmentCK_InArmZRealaseOffset->Text   = TestIF_File.iAutoAlignmentCK_InArmZRealaseOffset;
    edAutoAlignmentCK_InArmZPickUpOffset->Text    = TestIF_File.iAutoAlignmentCK_InArmZPickUpOffset;

    edAutoAlignmentCK_OutArmZRealaseOffset->Text  = TestIF_File.iAutoAlignmentCK_OutArmZRealaseOffset;
    edAutoAlignmentCK_OutArmZPickUpOffset->Text   = TestIF_File.iAutoAlignmentCK_OutArmZPickUpOffset;

    fAutoAlignment->cbAOA_UseFix1->Checked=TestIF_File.iAutoAlignment_UseFix[iAutoIndex[eFix1]];
    fAutoAlignment->cbAOA_UseFix2->Checked=TestIF_File.iAutoAlignment_UseFix[iAutoIndex[eFix2]];
    fAutoAlignment->cbAOA_UseFix3->Checked=TestIF_File.iAutoAlignment_UseFix[iAutoIndex[eFix3]];
    fAutoAlignment->cbAOA_UseFix4->Checked=TestIF_File.iAutoAlignment_UseFix[iAutoIndex[eFix4]];
    fAutoAlignment->cbAOA_UseFix5->Checked=TestIF_File.iAutoAlignment_UseFix[iAutoIndex[eFix5]];
    fAutoAlignment->cbAOA_UseFix6->Checked=TestIF_File.iAutoAlignment_UseFix[iAutoIndex[eFix6]];

    edAOADecodetimeout->Text = TestIF_File.iAOA_DecodeTimeOut;                  //Ifor 20211221 add: AOA Decode Time Out
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::FormShow(TObject *Sender)
{
    bShow=true;

    cbAOA_UseFix4->Visible=(AUTO_EMPTY_COLOR>=3);                               //Steven 20240428 : Add for HT9011 AOA
    cbAOA_UseFix5->Visible=(AUTO_EMPTY_COLOR>=3);
    cbAOA_UseFix6->Visible=(AUTO_EMPTY_COLOR>=3);

    DoIniDataToForm();
    SetTechDataToProd();
    if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable && CUSTOMER_CODE==CC_ASE_KaohSiung)     //KenHsieh 20211214 : AOA add AutoClean //KenHsieh 20220103 : 新增ASEKH判定
        grpInputArea->Caption="Shuttle/Hotplate/AutoClean";
    else
        grpInputArea->Caption="Shuttle/Hotplate";
    bNeedHome=TestIF_File.bEnableAutoAlignment;                                 //KenHsieh 20211202 : 修改重新打開AOA後，先做Onecycle再回Home重做AOA
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    bShow=false;
    if(bNeedHome!=TestIF_File.bEnableAutoAlignment && TestIF_File.bEnableAutoAlignment==true)   //KenHsieh 20211202 : 修改重新打開AOA後，先做Onecycle再回Home重做AOA
    {
        bNeedOneCycleByAutoAlignment=true;
        fMain->BtnOneCycleClick(fMain);
    }
    DoIniDataToForm();                                                          //JerryYang 20250411 : 離開頁面要刷新一次, 避免誤存檔
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::spbSaveClick(TObject *Sender)
{
    AnsiString szDir=GetRecipeFileName("HandlerCondition.Data");
    AnsiString sGroup;

    sGroup="AutoAlignmrnt";
    CheckAutoAlignmentEvent();

    if(MACHINE_HAS_AUTO_ALIGNMENT_CCD==false)
    {
        return;
    }

    WriteIniData(szDir, sGroup, "Enable Auto Alignment",                  (cbEnabledAutoAlignment->Checked)?1:0);

    WriteIniData(szDir, sGroup, "AutoAlignment FileName",                 edtFileName->Text                    );     //Kenhsieh 20210922 : save AutoAlignment FileName

    WriteIniData(szDir, sGroup, "AutoAlignmentTrayEvent",                 TestIF_File.iAutoAlignmentTrayEvent                         );
    WriteIniData(szDir, sGroup, "AutoAlignmentShuttleHotplateEvent",      TestIF_File.iAutoAlignmentShuttleHotplateEvent              );
    WriteIniData(szDir, sGroup, "AutoAlignmentTrayEvent_Z",               TestIF_File.iAutoAlignmentTrayEvent_Z                       );
    WriteIniData(szDir, sGroup, "AutoAlignmentShuttleHotplateEvent_Z",    TestIF_File.iAutoAlignmentShuttleHotplateEvent_Z            );

    WriteIniData(szDir, sGroup, "Alignment Point X",                      atoi(edtAlignmentPointX->Text.c_str())                      );
    WriteIniData(szDir, sGroup, "Alignment Point Y",                      atoi(edtAlignmentPointY->Text.c_str())                      );

    WriteIniData(szDir, sGroup, "Alignment Plate Point X",                atoi(edtAlignmentPlatePointX->Text.c_str())                 );
    WriteIniData(szDir, sGroup, "Alignment Plate Point Y",                atoi(edtAlignmentPlatePointY->Text.c_str())                 );

    WriteIniData(szDir, sGroup, "Alignment DeviceThick",                  atoi(edAutoAlignment_DeviceThick->Text.c_str())             );

    WriteIniData(szDir, sGroup, "Alignment InArmZRealaseOffset",          atoi(edAutoAlignmentTray_InArmZRealaseOffset->Text.c_str()) );
    WriteIniData(szDir, sGroup, "Alignment InArmZPickUpOffset",           atoi(edAutoAlignmentTray_InArmZPickUpOffset->Text.c_str())  );

    WriteIniData(szDir, sGroup, "Alignment OutArmZRealaseOffset",         atoi(edAutoAlignmentTray_OutArmZRealaseOffset->Text.c_str()));
    WriteIniData(szDir, sGroup, "Alignment OutArmZPickUpOffset",          atoi(edAutoAlignmentTray_OutArmZPickUpOffset->Text.c_str()) );

    WriteIniData(szDir, sGroup, "Alignment InArmZRealaseOffset",          atoi(edAutoAlignmentCK_InArmZRealaseOffset->Text.c_str())   );
    WriteIniData(szDir, sGroup, "Alignment InArmZPickUpOffset",           atoi(edAutoAlignmentCK_InArmZPickUpOffset->Text.c_str())    );

    WriteIniData(szDir, sGroup, "Alignment OutArmZRealaseOffset",         atoi(edAutoAlignmentCK_OutArmZRealaseOffset->Text.c_str())  );
    WriteIniData(szDir, sGroup, "Alignment OutArmZPickUpOffset",          atoi(edAutoAlignmentCK_OutArmZPickUpOffset->Text.c_str())   );

    WriteIniData(szDir, sGroup, "Alignment Use Fix1",                    (cbAOA_UseFix1->Checked)?1:0                       );
    WriteIniData(szDir, sGroup, "Alignment Use Fix2",                    (cbAOA_UseFix2->Checked)?1:0                       );
    WriteIniData(szDir, sGroup, "Alignment Use Fix3",                    (cbAOA_UseFix3->Checked)?1:0                       );
    if(AUTO_EMPTY_COLOR>=3)                                                     //Steven 20240428 : Add for HT9011 AOA
    {
        WriteIniData(szDir, sGroup, "Alignment Use Fix4",                (cbAOA_UseFix4->Checked)?1:0                       );
        WriteIniData(szDir, sGroup, "Alignment Use Fix5",                (cbAOA_UseFix5->Checked)?1:0                       );
        WriteIniData(szDir, sGroup, "Alignment Use Fix6",                (cbAOA_UseFix6->Checked)?1:0                       );
    }

    WriteIniData(szDir, sGroup, "Alignment Decode Time Out",              atoi(edAOADecodetimeout->Text.c_str())          );    //KenHsieh 20220103 : AOA Decode Time Out
    ReadFile();
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::edAutoAlignment_DeviceThickMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 10, 0);
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::edtInArmPortMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_PORT);
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::edAutoAlignmentTray_InArmZRealaseOffsetClick(
      TObject *Sender)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, InputLimit.iOffsetZHigh*100, InputLimit.iOffsetZLow*100);
}
//---------------------------------------------------------------------------
void __fastcall TfAutoAlignment::edtAlignmentPointXMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 100, 0);
}
//ChungHung 20210113 add for Alignment CCD end
//------------------------------------------------------------------------------
HTimer HInArmCCDAlignmentMotorDelay;
bool DoMoveInArmCCDAlignmentTeachUnit(int iPosX, int iPosY)
{
    bool flag[2]={true, true};
    if(CheckInArmZ())
    {
        bNeedArmZHome=true;
        InitDoArmZHome();
        return false;
    }

    if(iPosX>=MOT[MInArmX].Motor->PSoftLimitP || iPosX<=MOT[MInArmX].Motor->PSoftLimitN)
    {
        ShowMyMessage("CCD Alignment error. Alignment In Arm motor will out of limit");
        return false;
    }

    if(iPosY>=MOT[MInArmY].Motor->PSoftLimitP || iPosY<=MOT[MInArmY].Motor->PSoftLimitN)
    {
        ShowMyMessage("CCD Alignment error. Alignment In Arm motor will out of limit");
        return false;
    }

    flag[0]=MOT[MInArmX].MotorMove(iPosX);
    flag[1]=MOT[MInArmY].MotorMove(iPosY);

    if(flag[0] && flag[1])
    {
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
//ChungHung 20200207 add for AutoTeach start
//ChungHung 20200207 add for AutoTeach ==>
int iInArmCCDAlignmentTeachUnitProcess=-1;
void InitInArmCCDAlignmentTeachUnitProcess()
{
    iInArmCCDAlignmentTeachUnitProcess=1;
}
//------------------------------------------------------------------------------
int DoInArmCCDAlignmentTeachUnitProces(double &ResolutionX, double &ResolutionY, double &dRadianX, double &dRadianY, int iTeach)   //0: Correction mm/pixel 1: Teach Pattern 2: Teach Pattern and Correction mm/pixel
{
    int &Task=iInArmCCDAlignmentTeachUnitProcess;
    int iPosX=Tech.iInArmXCCDAlignmentPos;
    int iPosY=Tech.iInArmYCCDAlignmentPos;
    static double dTeachPos[4][2];  //0: Left 1: Right 2: Up 3: Down  //0: X 1: Y    //QQQ
    int dLength;
    static double dOffsetX, dOffsetY;
    double dOffsetX1, dOffsetY1;
    int iDis=200;
    int iRet=0;

    switch(Task)
    {
        case 1:
            if(MoveInArmZToPlateSafe(Task))
            {
                if(Cylinder[C_InAreaAlignment].Pop())                           //KenHsieh 20220117 : 新增Golden初始狀態
                {
                    if(iTeach==1 || iTeach==2)
                        Task=5;
                    else
                        Task=100;
                }
            }
            break;
        case 5:                                                                 //Move To Mark Center Postion
            if(DoMoveInArmCCDAlignmentTeachUnit(iPosX, iPosY))
            {
                HInArmCCDAlignmentMotorDelay.SetMSAndOn(iCCDAlignmentMotorDelay);      //Kenhsieh 20210923 : 修改等待時間方式
                Task=10;
            }
            break;
        case 10:
            if(HInArmCCDAlignmentMotorDelay.Off()==false)
                break;
            //TEACH_GOLDEN
            fAutoAlignment->InArmSendCommand(TEACH_GOLDEN);
            Task=25;
            break;
        case 25:
            iRet=fAutoAlignment->InArmGetResult(TEACH_GOLDEN, dOffsetX, dOffsetY);
            if(iRet==0)                                                         //KenHsieh 20220221 : 修改cmd teach類指令取值流程
            {
                ShowMyMessage("Please Check InArm Golden Patten in AutoAlignment PC!");
                return 0;
            }
            else if(iRet==1)
            {
                if(iTeach==1)
                    return 1;
                else
                    Task=100;
            }
            else if(iRet==2)
            {
                ShowMyMessage("Please Check InArm Alignment Teach Golden Mark again!");
                return 2;
            }
            break;
        case 100: //Move To Mark Left
            if(DoMoveInArmCCDAlignmentTeachUnit(iPosX-iDis, iPosY))
            {
                HInArmCCDAlignmentMotorDelay.SetMSAndOn(iCCDAlignmentMotorDelay);
                Task=200;
            }
            break;
        case 200:
            if(HInArmCCDAlignmentMotorDelay.Off()==false)
                break;
            fAutoAlignment->InArmSendCommand(UNIT_LEFT);
            Task=300;
            break;
        case 300:
            iRet=fAutoAlignment->InArmGetResult(UNIT_LEFT, dOffsetX, dOffsetY);
            if(iRet==1)
            {
                dTeachPos[0][0]=dOffsetX;
                dTeachPos[0][1]=dOffsetY;
                Task=400;
            }
            else if(iRet==2)
            {
                ShowMyMessage("Please Check InArm Alignment Left Golden Mark!");
                return 2;
            }
            break;
        case 400: //Move To Mark Right
            if(DoMoveInArmCCDAlignmentTeachUnit(iPosX+iDis, iPosY))
            {
                HInArmCCDAlignmentMotorDelay.SetMSAndOn(iCCDAlignmentMotorDelay);
                Task=500;
            }
            break;
        case 500:
            if(HInArmCCDAlignmentMotorDelay.Off()==false)
                break;
            fAutoAlignment->InArmSendCommand(UNIT_RIGHT);
            Task=600;
            break;
        case 600:
            iRet=fAutoAlignment->InArmGetResult(UNIT_RIGHT, dOffsetX, dOffsetY);
            if(iRet==1)
            {
                dTeachPos[1][0]=dOffsetX;
                dTeachPos[1][1]=dOffsetY;
                Task=700;
            }
            else if(iRet==2)
            {
                ShowMyMessage("Please Check InArm Alignment Right Golden Mark!");
                return 2;
            }
            break;
        case 700: //Move To Mark Up
            if(DoMoveInArmCCDAlignmentTeachUnit(iPosX, iPosY+iDis))
            {
                HInArmCCDAlignmentMotorDelay.SetMSAndOn(iCCDAlignmentMotorDelay);
                Task=800;
            }
            break;
        case 800:
            if(HInArmCCDAlignmentMotorDelay.Off()==false)
                break;
            fAutoAlignment->InArmSendCommand(UNIT_UP);
            Task=900;
            break;
        case 900:
            iRet=fAutoAlignment->InArmGetResult(UNIT_UP, dOffsetX, dOffsetY);
            if(iRet==1)
            {
                dTeachPos[2][0]=dOffsetX;
                dTeachPos[2][1]=dOffsetY;
                Task=1000;
            }
            else if(iRet==2)
            {
                ShowMyMessage("Please Check InArm Alignment Up Golden Mark!");
                return 2;
            }
            break;
        case 1000: //Move To Mark Down
            if(DoMoveInArmCCDAlignmentTeachUnit(iPosX, iPosY-iDis))
            {
                HInArmCCDAlignmentMotorDelay.SetMSAndOn(iCCDAlignmentMotorDelay);
                Task=1100;
            }
            break;
        case 1100:
            if(HInArmCCDAlignmentMotorDelay.Off()==false)
                break;
            fAutoAlignment->InArmSendCommand(UNIT_DOWN);
            Task=1200;
            break;
        case 1200:
            iRet=fAutoAlignment->InArmGetResult(UNIT_DOWN, dOffsetX, dOffsetY);
            if(iRet==1)
            {
                dTeachPos[3][0]=dOffsetX;
                dTeachPos[3][1]=dOffsetY;
                Task=1300;
            }
            else if(iRet==2)
            {
                ShowMyMessage("Please Check InArm Alignment Down Golden Mark!");
                return 2;
            }
            break;
        case 1300: //Move To Mark Center
            if(DoMoveInArmCCDAlignmentTeachUnit(iPosX, iPosY))
            {
                HInArmCCDAlignmentMotorDelay.SetMSAndOn(iCCDAlignmentMotorDelay);
                Task=1400;
            }
            break;
        case 1400:
            if(HInArmCCDAlignmentMotorDelay.Off()==false)
                break;
            fAutoAlignment->InArmSendCommand(UNIT_CENTER);
            Task=1500;
            break;
        case 1500:
            if(fAutoAlignment->InArmGetResult(UNIT_CENTER, dOffsetX, dOffsetY))
            {
                Task=1600;
            }
            else if(iRet==2)
            {
                ShowMyMessage("Please Check InArm Alignment Center Golden Mark!");
                return 2;
            }
            break;
        case 1600:
            //0: Left 1: Right 2: Up 3: Down  //0: X 1: Y
            if(dTeachPos[1][0]>=dTeachPos[0][0])
            {
                ShowMyMessage("Please Check CCD Image Setting!");
                return 2;
            }

            if(dTeachPos[3][1]<=dTeachPos[2][1])
            {
                ShowMyMessage("Please Check CCD Image Setting!");
                return 2;
            }

            dLength     =(dTeachPos[1][1]-dTeachPos[0][1])*(dTeachPos[1][1]-dTeachPos[0][1])+(dTeachPos[1][0]-dTeachPos[0][0])*(dTeachPos[1][0]-dTeachPos[0][0]);
            ResolutionX =ChangeToFloatNonPcnt((double)((2*iDis)), (double)(sqrt(dLength)));
            ResolutionX =Round(ResolutionX, 3);
            dRadianX    =atan(-(dTeachPos[0][1]-dTeachPos[1][1])/(dTeachPos[0][0]-dTeachPos[1][0]));
            dRadianX    =Round(dRadianX, 4);
            dLength     =(dTeachPos[2][1]-dTeachPos[3][1])*(dTeachPos[2][1]-dTeachPos[3][1])+(dTeachPos[2][0]-dTeachPos[3][0])*(dTeachPos[2][0]-dTeachPos[3][0]);
            ResolutionY =ChangeToFloatNonPcnt((double)((2*iDis)), (double)(sqrt(dLength)));
            ResolutionY =Round(ResolutionY, 3);
            dRadianY    =atan(-(dTeachPos[3][0]-dTeachPos[2][0])/-(dTeachPos[3][1]-dTeachPos[2][1]) );
            dRadianY    =Round(dRadianY, 4);
            if(bSpin)
            {
                RotationCoordinates(Round(dOffsetX), Round(dOffsetY), dOffsetX1, dOffsetY1, dRadianX, dRadianY);    //ChungHung 20210113 add for Alignment CCD
                dOffsetX1=dOffsetX1*ResolutionX;
                dOffsetY1=dOffsetY1*ResolutionY;
            }
            else
            {
                dOffsetX1=dOffsetX*ResolutionX;
                dOffsetY1=dOffsetY*ResolutionY;
            }

            if(DoMoveInArmCCDAlignmentTeachUnit(iPosX+(int)dOffsetX1, iPosY+(int)dOffsetY1))
            {
                return 1;
            }
            break;
       case 1700:
            if(HInArmCCDAlignmentMotorDelay.Off()==false)
                break;
            fAutoAlignment->InArmSendCommand(UNIT_CENTER);
            Task=1800;
            break;
        case 1800:
            if(fAutoAlignment->InArmGetResult(UNIT_CENTER, dOffsetX, dOffsetY))
            {
                dLength     =(dTeachPos[1][1]-dTeachPos[0][1])*(dTeachPos[1][1]-dTeachPos[0][1])+(dTeachPos[1][0]-dTeachPos[0][0])*(dTeachPos[1][0]-dTeachPos[0][0]);
                ResolutionX =ChangeToFloatNonPcnt((double)((2*iDis)), (double)(Round(sqrt(dLength), 2)));
                ResolutionX =Round(ResolutionX, 3);
                dRadianX    =atan(-(dTeachPos[0][1]-dTeachPos[1][1])/(dTeachPos[0][0]-dTeachPos[1][0]));
                dRadianX    =Round(dRadianX, 4);
                dLength     =(dTeachPos[2][1]-dTeachPos[3][1])*(dTeachPos[2][1]-dTeachPos[3][1])+(dTeachPos[2][0]-dTeachPos[3][0])*(dTeachPos[2][0]-dTeachPos[3][0]);
                ResolutionY =ChangeToFloatNonPcnt((double)((2*iDis)), (double)(Round(sqrt(dLength),2)));
                ResolutionY =Round(ResolutionY, 3);
                dRadianY    =atan(-(dTeachPos[3][0]-dTeachPos[2][0])/-(dTeachPos[3][1]-dTeachPos[2][1]));
                dRadianY    =Round(dRadianY, 4);
                return 1;
            }
            else if(iRet==2)
            {
                ShowMyMessage("Please Check InArm Alignment Center Golden Mark!!!");
                return 2;
            }
            break;
    }

    ResolutionX=0;
    ResolutionX=0;
    dRadianY   =0;
    dRadianY   =0;
    return false;
}
//------------------------------------------------------------------------------
//ChungHung 20200207 add for AutoTeach start
//ChungHung 20200207 add for AutoTeach ==>
bool DoMoveAlignmentToInShuttleStage(int iSht, int iRow, int iCol, int &iXPos, int &iYPos, bool realMove, bool bCheckXOnly=false, bool bCheckYOnly=false)      //Kenhsieh 20211002 : Modify Shuttle Scan
{
    bool flag[2]={true, true};
    int iShuttleXItem, iShuttleYItem, iShuttleXPitch, iShuttleYPitch;
    int iOffsetX, iOffsetY;

    if(iCol>8 || iCol<0)
        return false;

    if(CheckInArmZ())
    {
        bNeedArmZHome=true;
        InitDoArmZHome();
        return false;
    }

    iShuttleXItem=FLCarryKit.iShtCol;                                               //JerryYang 20250204 : fix AOA
    iShuttleYItem=FLCarryKit.iShtRow;

    iShuttleXPitch=TestIF.dSiteXPitch;
    iShuttleYPitch=TestIF.dSiteYPitch;
//    GetShuttleSize(iShuttleXItem, iShuttleYItem, iShuttleXPitch, iShuttleYPitch);      //ChungHung 20210113 add for Alignment CCD

    if(iShuttleXItem==2)
        iOffsetX=0;
    else
        iOffsetX=(((iShuttleXItem-1)/2.0)*iShuttleXPitch)-iCol*iShuttleXPitch;           // (2-1/2) = 0.5*

    if(iShuttleYItem==1)
        iOffsetY=0;
    else if(iShuttleYItem==2)
        iOffsetY=(iShuttleYPitch/2.0)-(iRow*iShuttleYPitch);

    if(iSht==0)
    {
        //KenHsieh 20210829 : Modify add CCD AUTO ALIGNMENT (XY變距)
        //==>
        if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable)
        {
            iXPos=Prod.XInArm_Shuttle1_Place[iInArmYBase][iInArmXBase]+iInArmShtXCenterPos-iOffsetX+Prod.iInArmCCD_Picker_OffsetX[iInArmYBase][iInArmXBase];   //左上第一點  //JerryYang 20250204 : fix AOA
            iYPos=Prod.YInArm_Shuttle1_Place[iInArmYBase][iInArmXBase]+iInArmShtYCenterPos+iOffsetY+Prod.iInArmCCD_Picker_OffsetY[iInArmYBase][iInArmXBase];
        }
        else
        {
            iXPos=Prod.XInArm_Shuttle1_Place[iInArmYBase][iInArmXBase]+iInArmShtXCenterPos-iOffsetX+Prod.iInArmCCD_Picker_OffsetX[iInArmYBase][iInArmXBase];   //左上第一點
            iYPos=Prod.YInArm_Shuttle1_Place[iInArmYBase][iInArmXBase]+iInArmShtYCenterPos+iOffsetY+Prod.iInArmCCD_Picker_OffsetY[iInArmYBase][iInArmXBase];
        }
        //<==
        //KenHsieh 20210829 : Modify add CCD AUTO ALIGNMENT (XY變距)
    }
    else
    {
        //KenHsieh 20210829 : Modify add CCD AUTO ALIGNMENT (XY變距)
        //==>
        if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable)
        {
            iXPos=Prod.XInArm_Shuttle2_Place[iInArmYBase][iInArmXBase]+iInArmShtXCenterPos-iOffsetX+Prod.iInArmCCD_Picker_OffsetX[iInArmYBase][iInArmXBase];   //左上第一點  //JerryYang 20250204 : fix AOA
            iYPos=Prod.YInArm_Shuttle2_Place[iInArmYBase][iInArmXBase]+iInArmShtYCenterPos+iOffsetY+Prod.iInArmCCD_Picker_OffsetY[iInArmYBase][iInArmXBase];
        }
        else
        {
            iXPos=Prod.XInArm_Shuttle2_Place[iInArmYBase][iInArmXBase]+iInArmShtXCenterPos-iOffsetX+Prod.iInArmCCD_Picker_OffsetX[iInArmYBase][iInArmXBase];   //左上第一點
            iYPos=Prod.YInArm_Shuttle2_Place[iInArmYBase][iInArmXBase]+iInArmShtYCenterPos+iOffsetY+Prod.iInArmCCD_Picker_OffsetY[iInArmYBase][iInArmXBase];
        }
        //<==
        //KenHsieh 20210829 : Modify add CCD AUTO ALIGNMENT (XY變距)
    }

    if(realMove==false)
    {
        if(bCheckYOnly==false)
        {
            if(iXPos>=MOT[MInArmX].Motor->PSoftLimitP || iXPos<=MOT[MInArmX].Motor->PSoftLimitN)
                return false;
        }

        if(bCheckXOnly==false)
        {
            if(iYPos>=MOT[MInArmY].Motor->PSoftLimitP || iYPos<=MOT[MInArmY].Motor->PSoftLimitN)
                return false;
        }
        return true;
    }

    flag[0]=MOT[MInArmX].MotorMove(iXPos);
    flag[1]=MOT[MInArmY].MotorMove(iYPos);

    if(flag[0] && flag[1])
    {
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
bool DoMoveAlignmentToHotPlateStage(int iP, int iRow, int iCol, int &iXPos, int &iYPos, bool realMove, bool bCheckXOnly=false, bool bCheckYOnly=false)     //KenHsieh 20211013 : Modify Hotplate Scan
{
    bool flag[2]={true, true};
    int iOffsetX, iOffsetY;
    if(CheckInArmZ())
    {
        bNeedArmZHome=true;
        InitDoArmZHome();
        return false;
    }

    iOffsetX=iCol*Prod.HotPlateForm[0].iXPitch;
    iOffsetY=iRow*Prod.HotPlateForm[0].iYPitch;

    if(iP==1)//iPickFromPlate=0 先丟2號加熱盤也就上上加熱盤
    {
        //KenHsieh 20210829 : Modify add CCD AUTO ALIGNMENT (XY變距)
        //==>
        if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable)
        {
            iXPos=Prod.XInArm_Plate1_Pick[iInArmYBase][iInArmXBase]+iOffsetX+Prod.iInArmCCD_Picker_OffsetX[iInArmYBase][iInArmXBase];  //JerryYang 20250204 : fix AOA
            iYPos=Prod.YInArm_Plate1_Pick[iInArmYBase][iInArmXBase]-iOffsetY+Prod.iInArmCCD_Picker_OffsetY[iInArmYBase][iInArmXBase];
        }
        else
        {
            iXPos=Prod.XInArm_Plate1_Pick[iInArmYBase][iInArmXBase]+iOffsetX+Prod.iInArmCCD_Picker_OffsetX[iInArmYBase][iInArmXBase];
            iYPos=Prod.YInArm_Plate1_Pick[iInArmYBase][iInArmXBase]-iOffsetY+Prod.iInArmCCD_Picker_OffsetY[iInArmYBase][iInArmXBase];
        }
        //<==
        //KenHsieh 20210829 : Modify add CCD AUTO ALIGNMENT (XY變距)
    }
    else
    {
        //KenHsieh 20210829 : Modify add CCD AUTO ALIGNMENT (XY變距)
        //==>
        if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable)
        {
            iXPos=Prod.XInArm_Plate2_Pick[iInArmYBase][iInArmXBase]+iOffsetX+Prod.iInArmCCD_Picker_OffsetX[iInArmYBase][iInArmXBase];  //JerryYang 20250204 : fix AOA
            iYPos=Prod.YInArm_Plate2_Pick[iInArmYBase][iInArmXBase]-iOffsetY+Prod.iInArmCCD_Picker_OffsetY[iInArmYBase][iInArmXBase];
        }
        else
        {
            iXPos=Prod.XInArm_Plate2_Pick[iInArmYBase][iInArmXBase]+iOffsetX+Prod.iInArmCCD_Picker_OffsetX[iInArmYBase][iInArmXBase];
            iYPos=Prod.YInArm_Plate2_Pick[iInArmYBase][iInArmXBase]-iOffsetY+Prod.iInArmCCD_Picker_OffsetY[iInArmYBase][iInArmXBase];
        }
        //<==
        //KenHsieh 20210829 : Modify add CCD AUTO ALIGNMENT (XY變距)
    }

    if(realMove==false)
    {
        if(bCheckYOnly==false)        //KenHsieh 20211013 : Modify Hotplate Scan
        {
            if(iXPos>=MOT[MInArmX].Motor->PSoftLimitP || iXPos<=MOT[MInArmX].Motor->PSoftLimitN)
                return false;
        }

        if(bCheckXOnly==false)        //KenHsieh 20211013 : Modify Hotplate Scan
        {
            if(iYPos>=MOT[MInArmY].Motor->PSoftLimitP || iYPos<=MOT[MInArmY].Motor->PSoftLimitN)
                return false;
        }
        return true;
    }

    flag[0]=MOT[MInArmX].MotorMove(iXPos);
    flag[1]=MOT[MInArmY].MotorMove(iYPos);

    if(flag[0] && flag[1])
    {
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
bool DoMoveAlignmentToLoaderStage(int iRow, int iCol, int &iXPos, int &iYPos, bool realMove, bool bCheckXOnly=false, bool bCheckYOnly=false)      //KenHsieh 20210902 : Modify add CCD AUTO ALIGNMENT
{
    bool flag1, flag2;
    if(CheckInArmZ())
    {
        bNeedArmZHome=true;
        InitDoArmZHome();
        return false;
    }
    //KenHsieh 20210829 : Modify add CCD AUTO ALIGNMENT (XY變距)
    //==>
    if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable)
    {
        iXPos = Prod.XInArm_Tray_Pick[iInArmYBase][iInArmXBase] + iCol*Prod.LoadForm.iXPitch + Prod.iInArmCCD_Picker_OffsetX[iInArmYBase][iInArmXBase];  //JerryYang 20250204 : fix AOA
        iYPos = Prod.YInArm_Tray_Pick[iInArmYBase][iInArmXBase] - iRow*Prod.LoadForm.iYPitch + Prod.iInArmCCD_Picker_OffsetY[iInArmYBase][iInArmXBase];
    }
    else
    {
        iXPos = Prod.XInArm_Tray_Pick[iInArmYBase][iInArmXBase] + iCol*Prod.LoadForm.iXPitch + Prod.iInArmCCD_Picker_OffsetX[iInArmYBase][iInArmXBase];
        iYPos = Prod.YInArm_Tray_Pick[iInArmYBase][iInArmXBase] - iRow*Prod.LoadForm.iYPitch + Prod.iInArmCCD_Picker_OffsetY[iInArmYBase][iInArmXBase];
    }
    //<==
    //KenHsieh 20210829 : Modify add CCD AUTO ALIGNMENT (XY變距)

    if(realMove==false)
    {
        if(bCheckYOnly==false)
        {
            if(iXPos>=MOT[MInArmX].Motor->PSoftLimitP || iXPos<=MOT[MInArmX].Motor->PSoftLimitN)
                return false;
        }

        if(bCheckXOnly==false)
        {
            if(iYPos>=MOT[MInArmY].Motor->PSoftLimitP || iYPos<=MOT[MInArmY].Motor->PSoftLimitN)
                return false;
        }
        return true;
    }

    flag1=MOT[MInArmX].MotorMove(iXPos);
    flag2=MOT[MInArmY].MotorMove(iYPos);

    if(flag1 && flag2)
    {
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
bool DoMoveAlignmentToAutoCleanStage(int iRow, int iCol, int &iXPos, int &iYPos, bool realMove, bool bCheckXOnly=false, bool bCheckYOnly=false)     //KenHsieh 20211214 : AOA add AutoClean
{
    bool flag[2]={true, true};
    int iOffsetX, iOffsetY;
    if(CheckInArmZ())
    {
        bNeedArmZHome=true;
        InitDoArmZHome();
        return false;
    }

    iOffsetX=iCol*TestIF_File.dAutoClean_XPitch;
    iOffsetY=iRow*TestIF_File.dAutoClean_YPitch;

    //KenHsieh 20210829 : Modify add CCD AUTO ALIGNMENT (XY變距)
    //==>
    if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable)
    {
        iXPos=Prod.XInArm_AutoClean_Pick[iInArmYBase][iInArmXBase]+iOffsetX+Prod.iInArmCCD_Picker_OffsetX[iInArmYBase][iInArmXBase];  //JerryYang 20250204 : fix AOA
        iYPos=Prod.YInArm_AutoClean_Pick[iInArmYBase][iInArmXBase]-iOffsetY+Prod.iInArmCCD_Picker_OffsetY[iInArmYBase][iInArmXBase];
    }
    //<==
    //KenHsieh 20210829 : Modify add CCD AUTO ALIGNMENT (XY變距)

    if(realMove==false)
    {
        if(bCheckYOnly==false)        //KenHsieh 20211013 : Modify Hotplate Scan
        {
            if(iXPos>=MOT[MInArmX].Motor->PSoftLimitP || iXPos<=MOT[MInArmX].Motor->PSoftLimitN)
                return false;
        }

        if(bCheckXOnly==false)        //KenHsieh 20211013 : Modify Hotplate Scan
        {
            if(iYPos>=MOT[MInArmY].Motor->PSoftLimitP || iYPos<=MOT[MInArmY].Motor->PSoftLimitN)
                return false;
        }
        return true;
    }

    flag[0]=MOT[MInArmX].MotorMove(iXPos);
    flag[1]=MOT[MInArmY].MotorMove(iYPos);

    if(flag[0] && flag[1])
    {
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
bool Find_InArmAlgnment_Single(TTrayMotor *myMotor, int &iRow, int &iCol)
{
    for(int iR=0; iR<myMotor->Tray.YItem; iR++)
    {
        for(int iC=0; iC<myMotor->Tray.XItem; iC++)
        {
            if(myMotor->Tray.Data[iC][iR]==WAIT_ALIGN_IC)
            {
                iCol=iC;
                iRow=iR;
                return true;
            }
        }
    }
    return false;
}
//------------------------------------------------------------------------------
int SearchInArmAlignmentUpDown(int iArea, int &iRow, int &iCol, int &iXPos, int &iYPos)  //0: Not Find Device 1:Find Device 2:Move Fail
{
    int _iRow=-1, _iCol=-1;
    TTrayMotor *myMotor=&MOT[MMInArmAOATray];

    Find_InArmAlgnment_Single(myMotor, _iRow, _iCol);

    if(_iRow==-1 && _iCol==-1)
        return 0;

    iRow=_iRow;
    iCol=_iCol;

#ifndef SOFT_SIMULTE
    bool bCheckXOnly=false;                                                     //KenHsieh 20210902 : Modify add CCD AUTO ALIGNMENT
    bool bCheckYOnly=false;                                                     //KenHsieh 20210902 : Modify add CCD AUTO ALIGNMENT
    bool CanMove=false;
    if(iArea==AOA_Loader)
        CanMove=DoMoveAlignmentToLoaderStage(iRow, iCol, iXPos, iYPos, false, bCheckXOnly, bCheckYOnly);        //KenHsieh 20210902 : Modify add CCD AUTO ALIGNMENT
    else if(iArea==AOA_Plate1)
        CanMove=DoMoveAlignmentToHotPlateStage(1, iRow, iCol, iXPos, iYPos, false, bCheckXOnly, bCheckYOnly);   //KenHsieh 20211013 : Modify Hotplate Scan
    else if(iArea==AOA_Plate2)
        CanMove=DoMoveAlignmentToHotPlateStage(0, iRow, iCol, iXPos, iYPos,false,bCheckXOnly,bCheckYOnly);      //KenHsieh 20211013 : Modify Hotplate Scan
    else if(iArea==AOA_InSht1)
        CanMove=DoMoveAlignmentToInShuttleStage(0, iRow, iCol, iXPos, iYPos, false, bCheckXOnly, bCheckYOnly);  //Kenhsieh 20211002 : Modify Shuttle Scan
    else if(iArea==AOA_InSht2)
        CanMove=DoMoveAlignmentToInShuttleStage(1, iRow, iCol, iXPos, iYPos, false, bCheckXOnly, bCheckYOnly);  //Kenhsieh 20211002 : Modify Shuttle Scan
    else if(iArea==AOA_AutoClean)
        CanMove=DoMoveAlignmentToAutoCleanStage(iRow, iCol, iXPos, iYPos, false, bCheckXOnly, bCheckYOnly);

    if(CanMove==false)
    {
        if(iArea==AOA_Loader)
            ShowMyMessage("Tray parameter error.Loader Alignment InArm motor will out of limit");
        if(iArea==AOA_Plate1)
            ShowMyMessage("Tray parameter error.Hotplate1 Alignment InArm motor will out of limit");
        if(iArea==AOA_Plate2)
            ShowMyMessage("Tray parameter error.Hotplate2 Alignment InArm motor will out of limit");
        if(iArea==AOA_InSht1)
            ShowMyMessage("Tray parameter error.Shuttle1 Alignment InArm motor will out of limit");
        if(iArea==AOA_InSht2)
            ShowMyMessage("Tray parameter error.Shuttle2 Alignment InArm motor will out of limit");
        if(iArea==AOA_AutoClean)                                                                                        //KenHsieh 20211214 : AOA add AutoClean
            ShowMyMessage("Tray parameter error.Autoclean Alignment InArm motor will out of limit");
        return 2;
    }
#endif
    return 1;
}
//------------------------------------------------------------------------------
bool MoveInArmAutoAlignmentStage(int iArea, int &iXPos, int &iYPos)             //0:Loader 1:HotPlate1 2:HotPlate2 3:Shttle1 4: Shuttle2
{
    int iRow=0, iCol=0;
    if(CheckInArmZ())
    {
        bNeedArmZHome=true;
        InitDoArmZHome();
        return false;
    }

    if(SearchInArmAlignmentUpDown(iArea, iRow, iCol, iXPos, iYPos)!=1)
        return false;

    bool bCheckXOnly=false;
    bool bCheckYOnly=false;

    if(iArea==AOA_Loader)
    {
        if(DoMoveAlignmentToLoaderStage(iRow, iCol, iXPos, iYPos, true, bCheckXOnly, bCheckYOnly))
        {
            return true;
        }
    }
    else if(iArea==AOA_Plate1)
    {
        if(DoMoveAlignmentToHotPlateStage(1, iRow, iCol, iXPos, iYPos, true, bCheckXOnly, bCheckYOnly))        //KenHsieh 20211013 : Modify Hotplate Scan
        {
            return true;
        }
    }
    else if(iArea==AOA_Plate2)
    {
        if(DoMoveAlignmentToHotPlateStage(0, iRow, iCol, iXPos, iYPos, true, bCheckXOnly, bCheckYOnly))        //KenHsieh 20211013 : Modify Hotplate Scan
        {
            return true;
        }
    }
    else if(iArea==AOA_InSht1)
    {
        if(DoMoveAlignmentToInShuttleStage(0, iRow, iCol, iXPos, iYPos, true, bCheckXOnly, bCheckYOnly))     //Kenhsieh 20211002 : Modify Shuttle Scan
        {
            return true;
        }
    }
    else if(iArea==AOA_InSht2)
    {
        if(DoMoveAlignmentToInShuttleStage(1, iRow, iCol, iXPos, iYPos, true, bCheckXOnly, bCheckYOnly))     //Kenhsieh 20211002 : Modify Shuttle Scan
        {
            return true;
        }
    }
    else if(iArea==AOA_AutoClean)
    {
        if(DoMoveAlignmentToAutoCleanStage(iRow, iCol, iXPos, iYPos, true, bCheckXOnly, bCheckYOnly))
        {
            return true;
        }
    }

    return false;
}
//------------------------------------------------------------------------------
//KenHsieh 20210902 : Modify add CCD AUTO ALIGNMENT
//==>
bool SearchInArmAlignmentMaxArea(int iArea, TTrayMotor *myTray)
{
    bool bCheckXOnly=false;
    bool bCheckYOnly=false;
    bool CanMove    =false;
    int  iColItem   =myTray->Tray.XItem;
    int  iRowItem   =myTray->Tray.YItem;
    int  iXPos      =0;
    int  iYPos      =0;

    for(int iC=0; iC<iColItem; iC++)
    {
        for(int iR=0; iR<iRowItem; iR++)
        {
            myTray->SetTraySingleData(iC, iR, WAIT_ALIGN_IC);
        }
    }

    for(int iR=0; iR<iRowItem; iR++)
    {
        bCheckXOnly=false;
        bCheckYOnly=true;

        if(iArea==AOA_Loader)
        {
            CanMove=DoMoveAlignmentToLoaderStage(iR, iColItem, iXPos, iYPos, false, bCheckXOnly, bCheckYOnly);
        }
        else if(iArea==AOA_Plate1)
        {
            CanMove=DoMoveAlignmentToHotPlateStage(1, iR, iColItem, iXPos, iYPos, false, bCheckXOnly, bCheckYOnly);       //KenHsieh 20211013 : Modify Hotplate Scan
        }
        else if(iArea==AOA_Plate2)
        {
            CanMove=DoMoveAlignmentToHotPlateStage(0, iR, iColItem, iXPos, iYPos, false, bCheckXOnly, bCheckYOnly);        //KenHsieh 20211013 : Modify Hotplate Scan
        }
        else if(iArea==AOA_InSht1)
        {
            CanMove=DoMoveAlignmentToInShuttleStage(0, iR, iColItem, iXPos, iYPos, false, bCheckXOnly, bCheckYOnly);      //Kenhsieh 20211002 : Modify Shuttle Scan
        }
        else if(iArea==AOA_InSht2)
        {
            CanMove=DoMoveAlignmentToInShuttleStage(1, iR, iColItem, iXPos, iYPos, false, bCheckXOnly, bCheckYOnly);      //Kenhsieh 20211002 : Modify Shuttle Scan
        }
        else if(iArea==AOA_AutoClean)
        {
            CanMove=DoMoveAlignmentToAutoCleanStage(iR, iColItem, iXPos, iYPos, false, bCheckXOnly, bCheckYOnly);
        }

        if(CanMove==false)
        {
            for(int iC=0; iC<iColItem; iC++)
            {
                myTray->SetTraySingleData(iC, iR, NULL_IC);
            }
        }
    }

    for(int iC=0; iC<iColItem; iC++)
    {
        bCheckXOnly=true;
        bCheckYOnly=false;

        if(iArea==AOA_Loader)
        {
            CanMove=DoMoveAlignmentToLoaderStage(iRowItem, iC, iXPos, iYPos, false, bCheckXOnly, bCheckYOnly);
        }
        else if(iArea==AOA_Plate1)
        {
            CanMove=DoMoveAlignmentToHotPlateStage(1, iRowItem, iC, iXPos, iYPos, false, bCheckXOnly, bCheckYOnly);       //KenHsieh 20211013 : Modify Hotplate Scan
        }
        else if(iArea==AOA_Plate2)
        {
            CanMove=DoMoveAlignmentToHotPlateStage(0, iRowItem, iC, iXPos, iYPos, false, bCheckXOnly, bCheckYOnly);       //KenHsieh 20211013 : Modify Hotplate Scan
        }
        else if(iArea==AOA_InSht1)
        {
            CanMove=DoMoveAlignmentToInShuttleStage(0, iRowItem, iC, iXPos, iYPos, false, bCheckXOnly, bCheckYOnly);      //Kenhsieh 20211002 : Modify Shuttle Scan
        }
        else if(iArea==AOA_InSht2)
        {
            CanMove=DoMoveAlignmentToInShuttleStage(1, iRowItem, iC, iXPos, iYPos, false, bCheckXOnly, bCheckYOnly);      //Kenhsieh 20211002 : Modify Shuttle Scan
        }
        else if(iArea==AOA_AutoClean)
        {
            CanMove=DoMoveAlignmentToAutoCleanStage(iRowItem, iC, iXPos, iYPos, false, bCheckXOnly, bCheckYOnly);
        }

        if(CanMove==false)
        {
            for(int iR=0; iR<iRowItem; iR++)
            {
                myTray->SetTraySingleData(iC, iR, NULL_IC);
            }
        }
    }

    return true;
}
//<==
//KenHsieh 20210902 : Modify add CCD AUTO ALIGNMENT
//------------------------------------------------------------------------------
int iInArmXYAutoTeachProcess=-1;
//------------------------------------------------------------------------------
void InitInArmXYAutoTeachProcess()
{
    iInArmXYAutoTeachProcess=1;
}
//------------------------------------------------------------------------------
int Find_InArmAlignment_NULL_IC(int iCol)                                       //Kenhsieh 20211007 : AOA Scan時獲取完整Shuttle XItem
{
    TTrayMotor *myMotor=&MOT[MMInArmAOATray];

    for(int iR=0; iR<myMotor->Tray.YItem; iR++)
    {
        for(int iC=0; iC<myMotor->Tray.XItem; iC++)
        {
            if(myMotor->Tray.Data[iC][iR]==NULL_IC)
                return iC;
        }
    }
    return myMotor->Tray.XItem;
}
//------------------------------------------------------------------------------
HTimer HTInArmCylinderDelay;                                                    //Kenhsieh 20210915 : 解決C_LoaderUpPress為打出而Hang up
int DoInArmXYAlignmentTeachProcess(int iArea, int bTeach)                       //iArea 0=loader 1=plate1 2=plate2 3:Shuttle1 4:Shuttle2 5:AutoClean
{
    int &Task=iInArmXYAutoTeachProcess;
    static int iRow=0, iCol=0;
    static bool bflag[2][4]={{false, false, false, false},{false, false, false, false}};
    int iInArmZ[2][4]={{MInArmZA, MInArmZC, MInArmZE, MInArmZG},                //QQQ
                       {MInArmZB, MInArmZD, MInArmZF, MInArmZH}};

    static int iXPos, iYPos;
    static double dOffsetX, dOffsetY;
    static double dOffsetX1, dOffsetY1;
    static int iIndexX, iIndexY;

    AnsiString asAreaFileName;
    AnsiString asAreaFileName_Cal;

    int iRet=0;
    static bool bFailNeedTeach=false;
    static int iInShuttleNoscanItemX=0;                                         //Kenhsieh 20211007 : AOA Scan時獲取完整Shuttle Xitem
    int iInShuttleScanX=0;                                                      //Kenhsieh 20211007 : AOA Scan時獲取完整Shuttle Xitem
    static int iRetryConnect=0;                                                 //Ifor 20211221 add: AOA 斷線重新連線

    switch(Task)
    {
        case 1:                                                                 //Init Area Tray
            if(iArea==AOA_Loader)
            {
                if(Cylinder[C_LoaderEdgePush].Enable)                           //KenHsieh 20211013 : Loader Scan前汽缸動作確認
                {
                    Cylinder[C_LoaderEdgePush].On();
                }
                Cylinder[C_TrayY_Fixer].On();
                HTInArmCylinderDelay.SetSecAndOn(0.5);
                Task=5;
                break;
            }
            else if(iArea==AOA_Plate1)
            {
                MOT[MMInArmAOATray].Tray.XItem = HotPlateForm.XDivision;
                MOT[MMInArmAOATray].Tray.YItem = HotPlateForm.YDivision;
                MOT[MMInArmAOATray].InitNewTray(NULL_IC, false, __FUNC__);
                SearchInArmAlignmentMaxArea(iArea, &MOT[MMInArmAOATray]);       //KenHsieh 20210902 : Modify add CCD AUTO ALIGNMENT
                SetAOATrayTeachPoint(TestIF.iAlignmentPlatePointX, TestIF.iAlignmentPlatePointY, &MOT[MMInArmAOATray]);
            }
            else if(iArea==AOA_Plate2)
            {
                MOT[MMInArmAOATray].Tray.XItem = HotPlateForm.XDivision;
                MOT[MMInArmAOATray].Tray.YItem = HotPlateForm.YDivision;
                MOT[MMInArmAOATray].InitNewTray(NULL_IC, false, __FUNC__);
                SearchInArmAlignmentMaxArea(iArea, &MOT[MMInArmAOATray]);       //KenHsieh 20210902 : Modify add CCD AUTO ALIGNMENT
                SetAOATrayTeachPoint(TestIF.iAlignmentPlatePointX, TestIF.iAlignmentPlatePointY, &MOT[MMInArmAOATray]);
            }
            else if(iArea==AOA_InSht1)
            {
                MOT[MMInArmAOATray].Tray.XItem = FRCarryKit.iShtCol;            //JerryYang 20250204 : fix AOA
                MOT[MMInArmAOATray].Tray.YItem = FRCarryKit.iShtRow;
                MOT[MMInArmAOATray].InitNewTray(NULL_IC, false, __FUNC__);
                SearchInArmAlignmentMaxArea(iArea, &MOT[MMInArmAOATray]);       //KenHsieh 20210902 : Modify add CCD AUTO ALIGNMENT
                SetAOATrayTeachPoint(FRCarryKit.iShtRow, FRCarryKit.iShtCol, &MOT[MMInArmAOATray]);
            }
            else if(iArea==AOA_InSht2)
            {
                MOT[MMInArmAOATray].Tray.XItem = FRCarryKit.iShtCol;            //JerryYang 20250204 : fix AOA
                MOT[MMInArmAOATray].Tray.YItem = FRCarryKit.iShtRow;
                MOT[MMInArmAOATray].InitNewTray(NULL_IC, false, __FUNC__);
                SearchInArmAlignmentMaxArea(iArea, &MOT[MMInArmAOATray]);       //KenHsieh 20210902 : Modify add CCD AUTO ALIGNMENT
                SetAOATrayTeachPoint(FRCarryKit.iShtRow, FRCarryKit.iShtCol, &MOT[MMInArmAOATray]);
            }
            else if(iArea==AOA_AutoClean)
            {
                MOT[MMInArmAOATray].Tray.XItem = TestIF.iAutoClean_XDivision;
                MOT[MMInArmAOATray].Tray.YItem = TestIF.iAutoClean_YDivision;
                MOT[MMInArmAOATray].InitNewTray(NULL_IC, false, __FUNC__);
                SearchInArmAlignmentMaxArea(iArea, &MOT[MMInArmAOATray]);       //KenHsieh 20210902 : Modify add CCD AUTO ALIGNMENT
                SetAOATrayTeachPoint(TestIF.iAlignmentPlatePointX, TestIF.iAlignmentPlatePointY, &MOT[MMInArmAOATray]);
            }
            else
            {
                break;
            }
            iIndexX=0;
            iIndexY=0;
            Task = 20;                                                          //Kenhsieh 20211007 : AOA Scan時獲取完整Shuttle Xitem
            bFailNeedTeach=false;
            break;
        case 5:                                                                 //KenHsieh 20211013 : Loader Scan前汽缸動作確認
            if(Cylinder[C_LoaderEdgePush].Enable)
            {
                if(HTInArmCylinderDelay.Off())
                {
                    if(Cylinder[C_LoaderUpPress].Enable)
                    {
                        Cylinder[C_LoaderUpPress].On();
                        HTInArmCylinderDelay.SetSecAndOn(2);
                    }
                    Task=10;
                }
            }
            else
            {
                if(Cylinder[C_LoaderUpPress].Enable)
                {
                    Cylinder[C_LoaderUpPress].On();
                    HTInArmCylinderDelay.SetSecAndOn(2);
                }
                Task=10;
            }
            break;
        case 10:                                                                //KenHsieh 20211013 : Loader Scan前汽缸動作確認
            if(HTInArmCylinderDelay.Off())
            {
                Task=15;
            }
            break;
        case 15:
            MOT[MMInArmAOATray].Tray.XItem = LoadForm->XDivision;
            MOT[MMInArmAOATray].Tray.YItem = LoadForm->YDivision;
            MOT[MMInArmAOATray].InitNewTray(NULL_IC, false, __FUNC__);
            SearchInArmAlignmentMaxArea(iArea, &MOT[MMInArmAOATray]);           //KenHsieh 20210902 : Modify add CCD AUTO ALIGNMENT
            SetAOATrayTeachPoint(TestIF.iAlignmentPointX, TestIF.iAlignmentPointY, &MOT[MMInArmAOATray], true);

            iIndexX=0;
            iIndexY=0;
            Task=20;                                                            //Kenhsieh 20211007 : AOA Scan時獲取完整Shuttle Xitem
            bFailNeedTeach=false;
            break;
        case 20:                                                                //Kenhsieh 20211007 : AOA Scan時獲取完整Shuttle Xitem
            if(iArea==AOA_InSht1 || iArea==AOA_InSht2)
            {
                iInShuttleNoscanItemX=0;
                iInShuttleNoscanItemX=Find_InArmAlignment_NULL_IC(iInShuttleScanX);
                Task=25;
            }
            Task=25;
            break;
        case 25:                                                                //Clear flag
            for(int i=0; i<InArmSuck.iMotRow; i++)
            {
                for(int j=0; j<InArmSuck.iMotCol; j++)
                {
                    bflag[i][j]=false;
                }
            }
            Task=50;
            //break;
        case 50:                                                                //All Z to safe pos
            for(int i=0; i<InArmSuck.iMotRow; i++)                              //QQQ
            {
                for(int j=0; j<InArmSuck.iMotCol; j++)
                {
                    if(bflag[i][j]==false)
                        bflag[i][j]=MOT[iInArmZ[i][j]].MotorMove(Prod.ZInArmSafe[i][j]);
                }
            }

            if(bflag[0][0] && bflag[0][1] && bflag[0][2] && bflag[0][3] &&
               bflag[1][0] && bflag[1][1] && bflag[1][2] && bflag[1][3])
            {
                iRet=SearchInArmAlignmentUpDown(iArea, iRow, iCol, iXPos, iYPos);
                if(iRet==1)
                    Task=200;
                else if(iRet==0)
                    Task=1000;
                else if(iRet==3)
                    Task=600;
            }
            break;
        case 200:                                                               //Move to iX,iY position
            if(MoveInArmAutoAlignmentStage(iArea, iXPos, iYPos))                //0:Loader 1:HotPlate1 2:HotPlate2 3:Shttle1 4: Shuttle2
            {
                HInArmCCDAlignmentMotorDelay.SetMSAndOn(iCCDAlignmentMotorDelay);
                Task=250;
            }
            break;
        case 250:
            if(HInArmCCDAlignmentMotorDelay.Off()==false)
                break;

            #ifndef SOFT_SIMULTE
            if(fAutoAlignment->ClientSocket1->Active==true)                     //Ifor 20211221 add: AOA 斷線重新連線
            #endif
            {
                iRetryConnect=0;                                                //KenHsieh 20220103 : AOA 斷線重新連線
                //Send Command Get offsetX offsetY
                if(iArea==AOA_Loader)
                {
                    #ifndef SOFT_SIMULTE                                        //KenHsieh 20211013 : Loader Scan前汽缸動作確認
                    if(Sen[SnLoaderSureTray].IsOn())                            //Kenhsieh 20211005 : 檢查是否有Tray
                    {
                        if(bTeach || bFailNeedTeach)
                            fAutoAlignment->InArmSendCommand(TEACH_TRAY);
                        else
                            fAutoAlignment->InArmSendCommand(AUTO_TRAY);
                    }
                    else
                    {
                        ShowMyMessage("Please Check Loader Whether Has Tray!!");
                        return 0;
                    }
                    #else
                    if(bTeach || bFailNeedTeach)
                        fAutoAlignment->InArmSendCommand(TEACH_TRAY);
                    else
                        fAutoAlignment->InArmSendCommand(AUTO_TRAY);
                    #endif
                }
                else if(iArea==AOA_Plate1)
                {
                    if(bTeach || bFailNeedTeach)
                        fAutoAlignment->InArmSendCommand(TEACH_HOTPLATE);
                    else
                        fAutoAlignment->InArmSendCommand(AUTO_HOTPLATE);
                }
                else if(iArea==AOA_Plate2)
                {
                    if(bTeach || bFailNeedTeach)
                        fAutoAlignment->InArmSendCommand(TEACH_HOTPLATE);
                    else
                        fAutoAlignment->InArmSendCommand(AUTO_HOTPLATE);
                }
                else if(iArea==AOA_InSht1)
                {
                    if(bTeach || bFailNeedTeach)
                        fAutoAlignment->InArmSendCommand(TEACH_SHUTTLE);
                    else
                        fAutoAlignment->InArmSendCommand(AUTO_SHUTTLE);
                }
                else if(iArea==AOA_InSht2)
                {
                    if(bTeach || bFailNeedTeach)
                        fAutoAlignment->InArmSendCommand(TEACH_SHUTTLE);
                    else
                        fAutoAlignment->InArmSendCommand(AUTO_SHUTTLE);
                }
                else if(iArea==AOA_AutoClean)
                {
                    if(bTeach || bFailNeedTeach)
                        fAutoAlignment->InArmSendCommand(TEACH_AUTOCLEAN);
                    else
                        fAutoAlignment->InArmSendCommand(AUTO_AUTOCLEAN);
                }
                else
                {
                    break;
                }
                Task=300;
            }
            #ifndef SOFT_SIMULTE
            else
            {
                if(iRetryConnect>=3)
                {
                    ShowMyMessage("AOA System InArm Connect Error!");
                }
                else
                {
                    iRetryConnect++;
                    fAutoAlignment->ClientSocket1->Close();
                    HInArmCCDAlignmentMotorDelay.SetMSAndOn(200);
                    Task=260;
                }
            }
            #endif
            break;
        case 260:                                                               //KenHsieh 20220103 : AOA 斷線重新連線
            if(HInArmCCDAlignmentMotorDelay.Off())
            {
                fAutoAlignment->ClientSocket1->Open();
                HInArmCCDAlignmentMotorDelay.SetMSAndOn(200);
                Task=250;
            }
            break;
        case 300:
            if(iArea==AOA_Loader)
            {
                #ifndef SOFT_SIMULTE
                if(Sen[SnLoaderSureTray].IsOn())                                //Kenhsieh 20211005 : 檢查是否有Tray
                {
                    if(bTeach || bFailNeedTeach)
                    {
                        iRet=fAutoAlignment->InArmGetResult(TEACH_TRAY, dOffsetX, dOffsetY);
                        if(iRet==0)                                             //KenHsieh 20220221 : 修改cmd teach類指令取值流程
                        {
                            ShowMyMessage("Please Check InArm Tray Patten in AutoAlignment PC!");
                            return 0;
                        }
                        else if(iRet==2)
                        {
                            ShowMyMessage("Please Check InArm Alignment Tray Mark!");
                            Task=250;
                            return 2;
                        }
                        bFailNeedTeach=false;
                    }
                    else
                    {
                        iRet=fAutoAlignment->InArmGetResult(AUTO_TRAY, dOffsetX, dOffsetY);
                        if(iRet==0)
                        {
                            return 0;
                        }
                        else if(iRet==2)
                        {
                            bFailNeedTeach=true;
                            Task=250;
                            return 0;
                        }
                    }
                }
                else
                {
                    ShowMyMessage("Please Check Loader Whether Has Tray!!");
                    return 0;
                }
                #else
                if(bTeach || bFailNeedTeach)
                {
                    iRet=fAutoAlignment->InArmGetResult(TEACH_TRAY, dOffsetX, dOffsetY);
                    bFailNeedTeach=false;
                }
                else
                {
                    iRet=fAutoAlignment->InArmGetResult(AUTO_TRAY, dOffsetX, dOffsetY);
                }
                #endif
            }
            else if(iArea==AOA_Plate1)
            {
                if(bTeach || bFailNeedTeach)
                {
                    iRet=fAutoAlignment->InArmGetResult(TEACH_HOTPLATE, dOffsetX, dOffsetY);
                    if(iRet==0)                                                 //KenHsieh 20220221 : 修改cmd teach類指令取值流程
                    {
                        ShowMyMessage("Please Check InArm Hotplate Patten in AutoAlignment PC!");
                        return 0;
                    }
                    else if(iRet==2)
                    {
                        ShowMyMessage("Please Check InArm Alignment Hotplate Mark!");
                        Task=250;
                        return 2;
                    }
                    bFailNeedTeach=false;
                }
                else
                {
                    iRet=fAutoAlignment->InArmGetResult(AUTO_HOTPLATE, dOffsetX, dOffsetY);
                    if(iRet==0)
                    {
                        return 0;
                    }
                    else if(iRet==2)
                    {
                        bFailNeedTeach=true;
                        Task=250;
                        return 0;
                    }
                }
            }
            else if(iArea==AOA_Plate2)
            {
                if(bTeach || bFailNeedTeach)
                {
                    iRet=fAutoAlignment->InArmGetResult(TEACH_HOTPLATE, dOffsetX, dOffsetY);
                    if(iRet==0)                                                 //KenHsieh 20220221 : 修改cmd teach類指令取值流程
                    {
                        ShowMyMessage("Please Check InArm Hotplate Patten in AutoAlignment PC!");
                        return 0;
                    }
                    else if(iRet==2)
                    {
                        ShowMyMessage("Please Check InArm Alignment Hotplate Mark!");
                        Task=250;
                        return 2;
                    }
                    bFailNeedTeach=false;
                }
                else
                {
                    iRet=fAutoAlignment->InArmGetResult(AUTO_HOTPLATE, dOffsetX, dOffsetY);
                    if(iRet==0)
                    {
                        return 0;
                    }
                    else if(iRet==2)
                    {
                        bFailNeedTeach=true;
                        Task=250;
                        return 0;
                    }
                }
            }
            else if(iArea==AOA_InSht1)
            {
//                GetShuttleSize(iShuttleXItem, iShuttleYItem, iShuttleXPitch, iShuttleYPitch);      //ChungHung 20210113 add for Alignment CCD

                if(bTeach || bFailNeedTeach)
                {
                    iRet=fAutoAlignment->InArmGetResult(TEACH_SHUTTLE, dOffsetX, dOffsetY);
                    if(iRet==0)                                                 //KenHsieh 20220221 : 修改cmd teach類指令取值流程
                    {
                        ShowMyMessage("Please Check InArm Shuttle Patten in AutoAlignment PC!");
                        return 0;
                    }
                    else if(iRet==2)
                    {
                        ShowMyMessage("Please Check InArm Alignment Shuttle Mark!");
                        Task=250;
                        return 2;
                    }
                    bFailNeedTeach=false;
                }
                else
                {
                    iRet=fAutoAlignment->InArmGetResult(AUTO_SHUTTLE, dOffsetX, dOffsetY);
                    if(iRet==0)
                    {
                        return 0;
                    }
                    else if(iRet==2)
                    {
                        bFailNeedTeach=true;
                        Task=250;
                        return 0;
                    }
                }
            }
            else if(iArea==AOA_InSht2)
            {
//                GetShuttleSize(iShuttleXItem, iShuttleYItem, iShuttleXPitch, iShuttleYPitch);      //ChungHung 20210113 add for Alignment CCD

                if(bTeach || bFailNeedTeach)
                {
                    iRet=fAutoAlignment->InArmGetResult(TEACH_SHUTTLE, dOffsetX, dOffsetY);
                    if(iRet==0)                                                 //KenHsieh 20220221 : 修改cmd teach類指令取值流程
                    {
                        ShowMyMessage("Please Check InArm Shuttle Patten in AutoAlignment PC!");
                        return 0;
                    }
                    else if(iRet==2)
                    {
                        ShowMyMessage("Please Check InArm Alignment Shuttle Mark!");
                        Task=250;
                        return 2;
                    }
                    bFailNeedTeach=false;
                }
                else
                {
                    iRet=fAutoAlignment->InArmGetResult(AUTO_SHUTTLE, dOffsetX, dOffsetY);
                    if(iRet==0)
                    {
                        return 0;
                    }
                    else if(iRet==2)
                    {
                        bFailNeedTeach=true;
                        Task=250;
                        return 0;
                    }
                }
            }
            else if(iArea==AOA_AutoClean)
            {
                if(bTeach || bFailNeedTeach)
                {
                    iRet=fAutoAlignment->InArmGetResult(TEACH_AUTOCLEAN, dOffsetX, dOffsetY);
                    if(iRet==0)                                                 //KenHsieh 20220221 : 修改cmd teach類指令取值流程
                    {
                        ShowMyMessage("Please Check InArm AutoClean Patten in AutoAlignment PC!");
                        return 0;
                    }
                    else if(iRet==2)
                    {
                        ShowMyMessage("Please Check InArm Alignment AutoClean Mark!");
                        Task=250;
                        return 2;
                    }
                    bFailNeedTeach=false;
                }
                else
                {
                    iRet=fAutoAlignment->InArmGetResult(AUTO_AUTOCLEAN, dOffsetX, dOffsetY);
                    if(iRet==0)
                    {
                        return 0;
                    }
                    else if(iRet==2)
                    {
                        bFailNeedTeach=true;
                        Task=250;
                        return 0;
                    }
                }
            }
            else
            {
                break;
            }

            if(bTeach)
                return 1;
            else
                Task=400;

            break;
        case 400:
            if(bSpin)
            {
                RotationCoordinates(dOffsetX, dOffsetY, dOffsetX1, dOffsetY1, Prod.dInArmCCDXRadian, Prod.dInArmCCDYRadian);    //ChungHung 20210113 add for Alignment CCD
                dOffsetX1=dOffsetX1*Prod.dInArmCCDXResolution;
                dOffsetY1=dOffsetY1*Prod.dInArmCCDYResolution;
            }
            else
            {
                dOffsetX1=dOffsetX*Prod.dInArmCCDXResolution;
                dOffsetY1=dOffsetY*Prod.dInArmCCDYResolution;
            }

            Task=500;
            return 0;
        case 425:
            if(HInArmCCDAlignmentMotorDelay.Off()==false)
                break;
            //Send Command Get offsetX offsetY
            if(iArea==AOA_Loader)
            {
                if(bTeach)
                    fAutoAlignment->InArmSendCommand(TEACH_TRAY);
                else
                    fAutoAlignment->InArmSendCommand(AUTO_TRAY);
            }
            else if(iArea==AOA_Plate1)
            {
                if(bTeach)
                    fAutoAlignment->InArmSendCommand(TEACH_HOTPLATE);
                else
                    fAutoAlignment->InArmSendCommand(AUTO_HOTPLATE);
            }
            else if(iArea==AOA_Plate2)
            {
                if(bTeach)
                    fAutoAlignment->InArmSendCommand(TEACH_HOTPLATE);
                else
                    fAutoAlignment->InArmSendCommand(AUTO_HOTPLATE);
            }
            else if(iArea==AOA_InSht1)
            {
                if(bTeach)
                    fAutoAlignment->InArmSendCommand(TEACH_SHUTTLE);
                else
                    fAutoAlignment->InArmSendCommand(AUTO_SHUTTLE);
            }
            else if(iArea==AOA_InSht2)
            {
                if(bTeach)
                    fAutoAlignment->InArmSendCommand(TEACH_SHUTTLE);
                else
                    fAutoAlignment->InArmSendCommand(AUTO_SHUTTLE);
            }
            else if(iArea==AOA_AutoClean)
            {
                if(bTeach)
                    fAutoAlignment->InArmSendCommand(TEACH_AUTOCLEAN);
                else
                    fAutoAlignment->InArmSendCommand(AUTO_AUTOCLEAN);
            }
            else
            {
                break;
            }
            Task=450;
            break;
        case 450:
            if(iArea==AOA_Loader)
            {
                if(bTeach)
                {
                    iRet=fAutoAlignment->InArmGetResult(TEACH_TRAY, dOffsetX, dOffsetY);
                    if(iRet==0)
                    {
                        return 0;
                    }
                    else if(iRet==2)
                    {
                        ShowMyMessage("Please Check InArm Alignment Tray Mark!");
                        return 2;
                    }
                }
                else
                {
                    iRet=fAutoAlignment->InArmGetResult(AUTO_TRAY, dOffsetX, dOffsetY);
                    if(iRet==0)
                    {
                        return 0;
                    }
                    else if(iRet==2)
                    {
                        ShowMyMessage("Please Check InArm Alignment Tray Mark!!");
                        return 2;
                    }
                }
            }
            else if(iArea==AOA_Plate1)
            {
                if(bTeach)
                {
                    iRet=fAutoAlignment->InArmGetResult(TEACH_HOTPLATE, dOffsetX, dOffsetY);
                    if(iRet==0)
                    {
                        return 0;
                    }
                    else if(iRet==2)
                    {
                        ShowMyMessage("Please Check InArm Alignment HotPlate Mark!");
                        return 2;
                    }
                }
                else
                {
                    iRet=fAutoAlignment->InArmGetResult(AUTO_HOTPLATE, dOffsetX, dOffsetY);
                    if(iRet==0)
                    {
                        return 0;
                    }
                    else if(iRet==2)
                    {
                        ShowMyMessage("Please Check InArm Alignment HotPlate Mark!!");
                        return 2;
                    }
                }
            }
            else if(iArea==AOA_Plate2)
            {
                if(bTeach)
                {
                    iRet=fAutoAlignment->InArmGetResult(TEACH_HOTPLATE, dOffsetX, dOffsetY);
                    if(iRet==0)
                    {
                        return 0;
                    }
                    else if(iRet==2)
                    {
                        ShowMyMessage("Please Check InArm Alignment HotPlate Mark!");
                        return 2;
                    }
                }
                else
                {
                    iRet=fAutoAlignment->InArmGetResult(AUTO_HOTPLATE, dOffsetX, dOffsetY);
                    if(iRet==0)
                    {
                        return 0;
                    }
                    else if(iRet==2)
                    {
                        ShowMyMessage("Please Check InArm Alignment HotPlate Mark!!");
                        return 2;
                    }
                }
            }
            else if(iArea==AOA_InSht1)
            {
//                GetShuttleSize(iShuttleXItem, iShuttleYItem, iShuttleXPitch, iShuttleYPitch);      //ChungHung 20210113 add for Alignment CCD

                if(bTeach)
                {
                    iRet=fAutoAlignment->InArmGetResult(TEACH_SHUTTLE, dOffsetX, dOffsetY);
                    if(iRet==0)
                    {
                        return 0;
                    }
                    else if(iRet==2)
                    {
                        ShowMyMessage("Please Check InArm Alignment Shuttle Mark!");
                        return 2;
                    }
                }
                else
                {
                    iRet=fAutoAlignment->InArmGetResult(AUTO_SHUTTLE, dOffsetX, dOffsetY);
                    if(iRet==0)
                    {
                        return 0;
                    }
                    else if(iRet==2)
                    {
                        ShowMyMessage("Please Check InArm Alignment Shuttle Mark!!");
                        return 2;
                    }
                }
            }
            else if(iArea==AOA_InSht2)
            {
//                GetShuttleSize(iShuttleXItem, iShuttleYItem, iShuttleXPitch, iShuttleYPitch);      //ChungHung 20210113 add for Alignment CCD

                if(bTeach)
                {
                    iRet=fAutoAlignment->InArmGetResult(TEACH_SHUTTLE, dOffsetX, dOffsetY);
                    if(iRet==0)
                    {
                        return 0;
                    }
                    else if(iRet==2)
                    {
                        ShowMyMessage("Please Check InArm Alignment Shuttle Mark!");
                        return 2;
                    }
                }
                else
                {
                    iRet=fAutoAlignment->InArmGetResult(AUTO_SHUTTLE, dOffsetX, dOffsetY);
                    if(iRet==0)
                    {
                        return 0;
                    }
                    else if(iRet==2)
                    {
                        ShowMyMessage("Please Check InArm Alignment Shuttle Mark!!");
                        return 2;
                    }
                }
            }
            else if(iArea==AOA_AutoClean)
            {
                if(bTeach)
                {
                    iRet=fAutoAlignment->InArmGetResult(TEACH_AUTOCLEAN, dOffsetX, dOffsetY);
                    if(iRet==0)
                    {
                        return 0;
                    }
                    else if(iRet==2)
                    {
                        ShowMyMessage("Please Check InArm Alignment AutoClean Mark!");
                        return 2;
                    }
                }
                else
                {
                    iRet=fAutoAlignment->InArmGetResult(AUTO_AUTOCLEAN, dOffsetX, dOffsetY);
                    if(iRet==0)
                    {
                        return 0;
                    }
                    else if(iRet==2)
                    {
                        ShowMyMessage("Please Check InArm Alignment AutoClean Mark!!");
                        return 2;
                    }
                }
            }
            else
            {
                break;
            }
            Task=500;
            break;
        case 500:
            if(iArea==AOA_InSht1 || iArea==AOA_InSht2)
            {
//                GetShuttleSize(iShuttleXItem, iShuttleYItem, iShuttleXPitch, iShuttleYPitch);
                InputAtuoTeachTableCal[iArea].ItemX=iInShuttleNoscanItemX;      //Kenhsieh 20211007 : AOA Scan時獲取完整Shuttle Xitem
                InputAtuoTeachTableCal[iArea].ItemY=FLCarryKit.iShtCol;
                InputAtuoTeachTable[iArea].ItemX=iInShuttleNoscanItemX;         //Kenhsieh 20211007 : AOA Scan時獲取完整Shuttle Xitem
                InputAtuoTeachTable[iArea].ItemY=FLCarryKit.iShtCol;
            }
            else if(iArea==AOA_Plate1 || iArea==AOA_Plate2)
            {
                InputAtuoTeachTableCal[iArea].ItemX=TestIF.iAlignmentPlatePointX;
                InputAtuoTeachTableCal[iArea].ItemY=TestIF.iAlignmentPlatePointY;
                InputAtuoTeachTable[iArea].ItemX=TestIF.iAlignmentPlatePointX;
                InputAtuoTeachTable[iArea].ItemY=TestIF.iAlignmentPlatePointY;
            }
            else
            {
                InputAtuoTeachTableCal[iArea].ItemX=TestIF.iAlignmentPointX;
                InputAtuoTeachTableCal[iArea].ItemY=TestIF.iAlignmentPointY;
                InputAtuoTeachTable[iArea].ItemX=TestIF.iAlignmentPointX;
                InputAtuoTeachTable[iArea].ItemY=TestIF.iAlignmentPointY;
            }

            //KenHsieh 20210829 : Modify add CCD AUTO ALIGNMENT (XY變距) //KenHsieh 20210913 : 新增CCD AUTO ALIGNMENT Log
            //==>
            if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable)
            {
                InputAtuoTeachTableCal[iArea].pointX[iIndexX][iIndexY]=iXPos-Prod.iInArmCCD_Picker_OffsetX[iInArmYBase][iInArmXBase];  //JerryYang 20250204 : fix AOA
                InputAtuoTeachTableCal[iArea].pointY[iIndexX][iIndexY]=iYPos-Prod.iInArmCCD_Picker_OffsetY[iInArmYBase][iInArmXBase];
                InputAtuoTeachTable[iArea].pointX[iIndexX][iIndexY]=iXPos-Prod.iInArmCCD_Picker_OffsetX[iInArmYBase][iInArmXBase]+dOffsetX1;
                InputAtuoTeachTable[iArea].pointY[iIndexX][iIndexY]=iYPos-Prod.iInArmCCD_Picker_OffsetY[iInArmYBase][iInArmXBase]+dOffsetY1;
            }
            else
            {
                InputAtuoTeachTableCal[iArea].pointX[iIndexX][iIndexY]=iXPos-Prod.iInArmCCD_Picker_OffsetX[iInArmYBase][iInArmXBase];
                InputAtuoTeachTableCal[iArea].pointY[iIndexX][iIndexY]=iYPos-Prod.iInArmCCD_Picker_OffsetY[iInArmYBase][iInArmXBase];
                InputAtuoTeachTable[iArea].pointX[iIndexX][iIndexY]=iXPos-Prod.iInArmCCD_Picker_OffsetX[iInArmYBase][iInArmXBase]+dOffsetX1;
                InputAtuoTeachTable[iArea].pointY[iIndexX][iIndexY]=iYPos-Prod.iInArmCCD_Picker_OffsetY[iInArmYBase][iInArmXBase]+dOffsetY1;
            }
            //<==
            //KenHsieh 20210829 : Modify add CCD AUTO ALIGNMENT (XY變距) //KenHsieh 20210913 : 新增CCD AUTO ALIGNMENT Log

            SaveInArmAutoAlignmentLog(iArea, iIndexX, iIndexY);                 //KenHsieh 20220111 : AOA add Save Log

            MOT[MMInArmAOATray].SetTraySingleData(iCol, iRow, NULL_IC);
            iIndexX++;
            if(iIndexX>=InputAtuoTeachTableCal[iArea].ItemX)
            {
                iIndexX=0;
                iIndexY++;
            }

            Task=25;
            break;
        case 600:
            MOT[MMInArmAOATray].SetTraySingleData(iIndexX, iRow, NULL_IC);

            iIndexX++;
            if(iIndexX>=InputAtuoTeachTableCal[iArea].ItemX)
            {
                iIndexX=0;
                iIndexY++;
            }

            Task=25;
            break;
        case 1000:
            if(iArea<TotalInArmAOAType)
            {
                WriteAutoTeachTable(asInArmAOAFileName[iArea].c_str(), &InputAtuoTeachTable[iArea]);
                WriteAutoTeachTable(asInArmAOAFileName_Cal[iArea].c_str(), &InputAtuoTeachTableCal[iArea]);
            }
            else
            {
                break;
            }

            ReadAutoTeachTable_InArm();                                         //KenHsieh 20211208 : In/Out Arm分開讀檔，避免InArm做完直接寫入導致OutArm點位錯誤
            //save data
            return 1;
    }
    return 0;
}
//------------------------------------------------------------------------------
bool DoMoveInArmPickerAndPitchAutoAlignment(int iPicker, int iPitch, int &iXPos, int &iYPos)
{
    bool flag1, flag2, flag3, flag4, flag5;     //KenHsieh 20211013 : Modify 吸嘴Teach X2、Y Pitch位置
    int iR=0, iC=0;
    int iPos[4]={-2,-1,0,1};

    //int iSingleIndex[2]={2,-1};
    if(CheckInArmZ())
    {
        bNeedArmZHome=true;
        InitDoArmZHome();
        return false;
    }

    //KenHsieh 20210829 : Modify add CCD AUTO ALIGNMENT (XY變距)
    //==>
    // 1X4 Picker
    if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable)
    {
        if(iPicker%2==1)
            iR=0;
        else
            iR=1;
        iYPos=Tech.iInArmYBasePickerAlignmentPos-(iR*IN_OUT_ARM_Y_PITCH_MAX);    //KenHsieh 20211013 : Modify 吸嘴Teach X2、Y Pitch位置
    }
    else
    {
        if(iPicker%2==1)
            iR=1;
        else
            iR=0;
        iYPos=Tech.iInArmYBasePickerAlignmentPos+(iR*6000);
    }
    //<==
    //KenHsieh 20210829 : Modify add CCD AUTO ALIGNMENT (XY變距)

    iC=iPicker/2;

    if(bInArmXPitch_40mm)                                                       //KenHsieh 20211111 : Modify X Pitch 40mm teach
        iXPos=Tech.iInArmXBasePickerAlignmentPos-iPos[iC]*iPitch/3;
    else
        iXPos=Tech.iInArmXBasePickerAlignmentPos-iPos[iC]*iPitch;

    if(iXPos>=MOT[MInArmX].Motor->PSoftLimitP || iXPos<=MOT[MInArmX].Motor->PSoftLimitN)
        return false;
    if(iYPos>=MOT[MInArmY].Motor->PSoftLimitP || iYPos<=MOT[MInArmY].Motor->PSoftLimitN)
        return false;

    flag1=MOT[MInArmX].MotorMove(iXPos);
    flag2=MOT[MInArmY].MotorMove(iYPos);

    if(bInArmXPitch_40mm)                                                       //KenHsieh 20211023 : 新增X Pitch 40mm teach
        flag3=MOT[MInArmPitch].MotorMove(GetInArmPitch_9045(iPitch));
    else
        flag3=MOT[MInArmPitch].MotorMove(GetInArmPitch_9045(iPitch*3));

    if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable)                                //KenHsieh 20211013 : Modify 吸嘴Teach X2、Y Pitch位置
    {
        if(flag3==true)                                                         //KenHsieh 20211111 : 解決Hang up
        {
            if(bInArmXPitch_40mm)                                               //KenHsieh 20211111 : Modify X Pitch 40mm teach
                flag4=MOT[MInArmPitchX2].MotorMove(GetInArmPitch2_9045(iPitch));
            else
                flag4=MOT[MInArmPitchX2].MotorMove(GetInArmPitch2_9045(iPitch*3));
            flag5=MOT[MInArmPitchY].MotorMove(GetInArmPitchY_9045(IN_OUT_ARM_Y_PITCH_MAX));
        }
    }
    else
    {
        flag4=true;
        flag5=true;
    }

    if(flag1 && flag2 && flag3 && flag4 && flag5)                               //KenHsieh 20211013 : Modify 吸嘴Teach X2、Y Pitch位置
    {
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
void SaveInArmAutoAlignmentLog(int iArea, int iIndexX, int iIndexY)             //KenHsieh 20220111 : AOA add Save Log
{
    AnsiString AnsBuffer="";                                                    //KenHsieh 20210913 : 新增CCD AUTO ALIGNMENT Log
    AnsiString AnsBuffer01="";                                                  //KenHsieh 20210913 : 新增CCD AUTO ALIGNMENT Log
    AnsiString asPath="";

    asPath.sprintf("%s\\%04d\\%02d", asInArmAutoAlignmentPath, SystemYear, SystemMonth);
    MyForceDirectories(asPath);
    asPath.sprintf("%s\\%04d\\%02d\\InArmAOALog_%04d%02d%02d.txt", asInArmAutoAlignmentPath, SystemYear, SystemMonth, SystemYear, SystemMonth, SystemDate);

    if(fMain->Memo3_AOA_IN->Lines->Count>=1024)
        fMain->Memo3_AOA_IN->Clear();

    AnsBuffer01.sprintf("%02d:%02d:%02d.%03d", SystemHour, SystemMin, SystemSec, SystemMSec);
    AnsBuffer.sprintf("%s:  InputTableCal[%d] PointX[%d][%d]= %f", AnsBuffer01, iArea, iIndexX, iIndexY, InputAtuoTeachTableCal[iArea].pointX[iIndexX][iIndexY]);
    fMain->Memo3_AOA_IN->Lines->Add(AnsBuffer);
    WriteDataToFile(asPath, AnsBuffer);

    AnsBuffer.sprintf("%s:  InputTableCal[%d] PointY[%d][%d]= %f", AnsBuffer01, iArea, iIndexX, iIndexY, InputAtuoTeachTableCal[iArea].pointY[iIndexX][iIndexY]);
    fMain->Memo3_AOA_IN->Lines->Add(AnsBuffer);
    WriteDataToFile(asPath, AnsBuffer);

    AnsBuffer.sprintf("%s:  InputTable   [%d] PointX[%d][%d]= %f", AnsBuffer01, iArea, iIndexX, iIndexY, InputAtuoTeachTable[iArea].pointX[iIndexX][iIndexY]);
    fMain->Memo3_AOA_IN->Lines->Add(AnsBuffer);
    WriteDataToFile(asPath, AnsBuffer);

    AnsBuffer.sprintf("%s:  InputTable   [%d] PointY[%d][%d]= %f", AnsBuffer01, iArea, iIndexX, iIndexY, InputAtuoTeachTable[iArea].pointY[iIndexX][iIndexY]);
    fMain->Memo3_AOA_IN->Lines->Add(AnsBuffer);
    WriteDataToFile(asPath, AnsBuffer);
}
//------------------------------------------------------------------------------
int iInArmPickerXYAndPitchAutoAlignment=1;
//------------------------------------------------------------------------------
void InitInArmPickerXYAndPitchAutoAlignment()
{
    iInArmPickerXYAndPitchAutoAlignment=1;
}
//------------------------------------------------------------------------------
HTimer HInArmPitchZAutoAlignmentMotorDelay;                                     //Kenhsieh 20211006 : Z軸到位Delay
int DoInArmPickerXYAndPitchAutoAlignment(int iBasePicker, bool bAll, int *iXPos, int *iYPos, int *iZPos) //0: 未完成 1: 成功 2: 中斷
{
    int &Task=iInArmPickerXYAndPitchAutoAlignment;
    int iPickerZPos[8]={0, 0, 0, 0, 0, 0, 0, 0};
    int iInArmZ[8]={MInArmZA, MInArmZB, MInArmZC, MInArmZD, MInArmZE, MInArmZF, MInArmZG, MInArmZH};     //QQQ

    static int iStep=0;
    static int iInArmXPos_P[8];
    static int iInArmXPos_N[8];
    static int iInArmYPos_P[8];
    static int iInArmYPos_N[8];
    static int iInArmZPickZ[8]={0, 0, 0, 0, 0, 0, 0, 0};
    static int iXpos;
    static int iYpos;

    static int iIndex;
/*
    for(int i=0; i<8; i++)
        iPickerZPos[i]=Prod.iInArmZBasePickerAlignmentPos[i];
*/

    for (int i=0; i<4; ++i)                                                     //Ifor 20250903 add:改成二維陣列
    {
        // 將 Prod 陣列前排的值，依序放入 iPickerZPos 的偶數索引 (0, 2, 4, 6)
        iPickerZPos[i*2] = Prod.iInArmZBasePickerAlignmentPos[0][i];
        // 將 Prod 陣列後排的值，依序放入 iPickerZPos 的奇數索引 (1, 3, 5, 7)
        iPickerZPos[i*2 +1] = Prod.iInArmZBasePickerAlignmentPos[1][i];
    }

    if(USE_PICKER_COUNT==ep16Picker)                                            //Ifor 20250903 add:16Pick 待更新
    {
    }

    switch(Task)
    {
        case 1:  //All InArmZ Move SafePostion
            Cylinder[C_InAreaAlignment].On();

            if(MoveInArmZToPlateSafe(Task))
            {
                for(int i=0; i<8; i++)
                {
                    iInArmXPos_P[i]=0;
                    iInArmXPos_N[i]=0;
                    iInArmYPos_P[i]=0;
                    iInArmYPos_N[i]=0;

                    iInArmZPickZ[i]=0;
                }
                iIndex=iBasePicker;
                Task=100;
            }

            break;
        case 100:   //Move to Teach Postion
            //檢測Sensor 必須為Off
            if(Sen[SnInAreaAlignmentSenX].IsOn() || (Sen[SnInAreaAlignmentSenY].IsOn() && Sen[SnInAreaAlignmentSenY].Enable))
                Task=3000;
            else if(DoMoveInArmPickerAndPitchAutoAlignment(iIndex, 4000, iXpos, iYpos))
                Task=200;
            break;
        case 200: // InArmZ Move To Touch Sensor
            if(Cylinder[C_InAreaAlignment].Push())
            {
                Task=225;
            }
            break;
        case 225:
            if(iPickerZPos[iIndex]==0)      //KenHsieh 20211015 : Teach吸嘴Z軸位置為0判斷
            {
                Task=2500;
            }
            else if(MOT[iInArmZ[iIndex]].MotorMove(iPickerZPos[iIndex]))  //Move to Z sandby postion
            {
                HInArmPitchZAutoAlignmentMotorDelay.SetSecAndOn(3);        //Kenhsieh 20211006 : Z軸到位Delay
                Task=230;
            }
            break;
        case 230:    //Kenhsieh 20211006 : Z軸到位Delay
            //檢測Sensor 必須為亮
            if(HInArmPitchZAutoAlignmentMotorDelay.Off())
            {
                if(Sen[SnInAreaAlignmentSenX].IsOff())
                {
                    Task=2000;
                }
                else
                {
                    iStep=0;
                    Task=300;
                }
            }
            break;
        case 250:
            iStep++;
            Task=300;
            break;
        case 300:
            if(MOT[MInArmX].MotorMove(iXpos+iXYMovePos*iStep)) // right move
            {
                if(Sen[SnInAreaAlignmentSenX].IsOff())
                {
                    iStep=0;
                    iInArmXPos_P[iIndex]=MOT[MInArmX].ReadPos();
                    Task=400;
                }
                else
                {
                    Task=250;
                }
            }
            break;
        case 350:
            iStep++;
            Task=450;
            break;
        case 400:
            if(MOT[MInArmX].MotorMove(iXpos))
            {
                if(Sen[SnInAreaAlignmentSenX].IsOff())  //KenHsieh 20211109 : 解決吸嘴掃描Golden時，無掃描之問題
                    Task=2000;
                else
                    Task=450;
            }
            break;
        case 450:
            if(MOT[MInArmX].MotorMove(iXpos-iXYMovePos*iStep)) // left move
            {
                if(Sen[SnInAreaAlignmentSenX].IsOff())
                {
                    iInArmXPos_N[iIndex]=MOT[MInArmX].ReadPos();
                    Task=500;
                }
                else
                {
                    Task=350;
                }
            }
            break;
        case 500:
            if(MOT[MInArmX].MotorMove(iXpos))
            {
                iStep=0;
                Task=600;

                Cylinder[C_InAreaAlignment].Off();
            }
            break;
        case 550:
            iStep++;
            Task=630;   //KenHsieh 20211109 : 解決吸嘴掃描Golden時，無掃描之問題
            break;
        case 600:
            if(Cylinder[C_InAreaAlignment].Pop())
            {
                Task=625;
            }
            break;
        case 625:       //KenHsieh 20211109 : 解決吸嘴掃描Golden時，無掃描之問題
            if(Sen[SnInAreaAlignmentSenX].IsOff())
                Task=2000;
            else
                Task=630;
            break;
        case 630:       //KenHsieh 20211109 : 解決吸嘴掃描Golden時，無掃描之問題
            if(MOT[MInArmY].MotorMove(iYpos+iXYMovePos*iStep)) // up move
            {
                if(Sen[SnInAreaAlignmentSenX].IsOff())
                {
                    iStep=0;
                    iInArmYPos_P[iIndex]=MOT[MInArmY].ReadPos();
                    Task=700;
                }
                else
                {
                    Task=550;
                }
            }
            break;
        case 650:
            iStep++;
            Task=710;
            break;
        case 700:
            if(MOT[MInArmY].MotorMove(iYpos)) // down move
            {
                if(Sen[SnInAreaAlignmentSenX].IsOff())      //KenHsieh 20211109 : 解決吸嘴掃描Golden時，無掃描之問題
                    Task=2000;
                else
                    Task=710;
            }
            break;
        case 710:
            if(MOT[MInArmY].MotorMove(iYpos-iXYMovePos*iStep)) // down move
            {
                if(Sen[SnInAreaAlignmentSenX].IsOff())
                {
                    iStep=0;
                    iInArmYPos_N[iIndex]=MOT[MInArmY].ReadPos();
                    Task=725;
                }
                else
                {
                    Task=650;
                }
            }
            break;
        case 725:
            if(MOT[MInArmY].MotorMove(iYpos)) // down move
            {
                iStep=0;
                Task=800;
            }
            break;
        case 750:
            iStep++;
            Task=800;
            break;
        case 800:
            if(MOT[iInArmZ[iIndex]].MotorMove(iPickerZPos[iIndex]+iZMovePos*iStep))  //leave
            {
                if(Sen[SnInAreaAlignmentSenX].IsOff())
                {
                    iStep=0;
                    iInArmZPickZ[iIndex]=MOT[iInArmZ[iIndex]].ReadPos();
                    Task=900;
                }
                else
                {
                    Task=750;
                }
            }
            break;
        case 900:
            if(MoveInArmZToPlateSafe(Task))
            {
                if(iIndex==iBasePicker)
                {
                    //TestIF.iInArmCCD_Picker_UnitX
                    // Get CCD and Base Picker DisX DisY
                }

                if(bAll==false)
                {
                    Task=1000;
                }
                else
                {
                    for(int i=0; i<8; i++)  //1x4
                    {
                        if(iInArmZPickZ[i]==0)
                        {
                            iIndex=i;
                            Task=100;
                            return 0;
                        }
                    }
                    Task=1000;
                }
            }
            else if(bNeedArmZHome==true)
            {
                ShowMyMessage("Please Check InArm Picker Home Sensor");
                return 2;
            }
            break;
        case 1000:
            for(int i=0; i<8; i++)
            {
                iXPos[i]=(iInArmXPos_P[i]+iInArmXPos_N[i])/2;
                iYPos[i]=(iInArmYPos_P[i]+iInArmYPos_N[i])/2;
                iZPos[i]= iInArmZPickZ[i];
            }
            return 1;
        case 2000:
            if(MoveInArmZToPlateSafe(Task))
            {
                ShowMyMessage("Please Check InArm Alignment Picker Teach Position,\r\n and AOA Golden Sensor must On");     //KenHsieh 20211015 : 修改吸嘴未到位提示訊息
                return 2;
            }
            else if(bNeedArmZHome==true)
            {
                ShowMyMessage("Please Check InArm Picker Home Sensor");
                return 2;
            }
            break;
        case 2500:      //KenHsieh 20211015 : Teach吸嘴Z軸位置為0判斷
            if(MoveInArmZToPlateSafe(Task))
            {
                ShowMyMessage("InArm Z Base Pick Pos is 0, please modify the value, and restart picker teach.");
                return 2;
            }
            break;
        case 3000:
            if(MoveInArmZToPlateSafe(Task))
            {
                ShowMyMessage("Please Check InArm Alignment# Picker Teach Position,\r\n and AOA Golden Sensor must Off");   //KenHsieh 20211015 : 修改吸嘴未到位提示訊息
                return 2;
            }
            else if(bNeedArmZHome==true)
            {
                ShowMyMessage("Please Check InArm Picker Home Sensor");
                return 2;
            }
            break;
    }
    return 0;
}
//------------------------------------------------------------------------------
int iInitInArmAlignment=1;
//------------------------------------------------------------------------------
void InitInArmAlignment()
{
    iInitInArmAlignment=1;
}
//------------------------------------------------------------------------------
HTimer HTInArmTeachAlignmentDelay;                                              //KenHsieh 20210830 : add CCD AUTO ALIGNMENT
bool DoInArmTeachAlignment(unsigned long &lAction)
{
    int &Task=iInitInArmAlignment;
    AnsiString sPath="D:\\HT9045\\System\\teach.ini", str="";
    TIniFile *INIFile;
    double ResolutionX, ResolutionY, dRadianX, dRadianY;
    int iRet=0;
    int iXPos[8], iYPos[8], iZPos[8];
    int iPosX, iPosY;

    if(lAction&INARM_ALIGNMENT_TEACH_UNIT)       //0x1
    {
        switch(Task)
        {
            case 1:
                RecordProcess("Alignment Teach Unit Start");
                InitInArmCCDAlignmentTeachUnitProcess();
                Task=100;
                break;
            case 100:
                iRet = DoInArmCCDAlignmentTeachUnitProces(ResolutionX, ResolutionY, dRadianX, dRadianY, 1);
                if(iRet==1)
                {
                    Task=1;
                    lAction=lAction^INARM_ALIGNMENT_TEACH_UNIT;
                    RecordProcess("Alignment Teach Unit Finish");
                    return true;
                }
                else if(iRet==2)
                {
                    Task=1;
                }
                break;
        }
    }
    else if(lAction&INARM_ALIGNMENT_TEACH_PICKER)     //0x2
    {
        switch(Task)
        {
            case 1:
                RecordProcess("Alignment Teach InArm Picker Start");
                InitInArmPickerXYAndPitchAutoAlignment();
                Task=100;
                break;
            case 100:
                iRet=DoInArmPickerXYAndPitchAutoAlignment(4, true, iXPos, iYPos, iZPos);
                if(iRet==1)                                                     //0: 未完成 1: 成功 2: 中斷
                {
                    INIFile=new TIniFile(sPath);
                    INIFile->WriteInteger("InArm", "iInArmPicker1_X", iXPos[0]);         //ChungHung 20210113 add for Alignment CCD
                    INIFile->WriteInteger("InArm", "iInArmPicker1_Y", iYPos[0]);         //ChungHung 20210113 add for Alignment CCD
                    INIFile->WriteInteger("InArm", "iInArmPicker1_Z", iZPos[0]);         //ChungHung 20210113 add for Alignment CCD

                    INIFile->WriteInteger("InArm", "iInArmPicker2_X", iXPos[1]);         //ChungHung 20210113 add for Alignment CCD
                    INIFile->WriteInteger("InArm", "iInArmPicker2_Y", iYPos[1]);         //ChungHung 20210113 add for Alignment CCD
                    INIFile->WriteInteger("InArm", "iInArmPicker2_Z", iZPos[1]);         //ChungHung 20210113 add for Alignment CCD

                    INIFile->WriteInteger("InArm", "iInArmPicker3_X", iXPos[2]);         //ChungHung 20210113 add for Alignment CCD
                    INIFile->WriteInteger("InArm", "iInArmPicker3_Y", iYPos[2]);         //ChungHung 20210113 add for Alignment CCD
                    INIFile->WriteInteger("InArm", "iInArmPicker3_Z", iZPos[2]);         //ChungHung 20210113 add for Alignment CCD

                    INIFile->WriteInteger("InArm", "iInArmPicker4_X", iXPos[3]);         //ChungHung 20210113 add for Alignment CCD
                    INIFile->WriteInteger("InArm", "iInArmPicker4_Y", iYPos[3]);         //ChungHung 20210113 add for Alignment CCD
                    INIFile->WriteInteger("InArm", "iInArmPicker4_Z", iZPos[3]);         //ChungHung 20210113 add for Alignment CCD

                    delete INIFile;
                    Task=1;
                    lAction=lAction^INARM_ALIGNMENT_TEACH_PICKER;
                    RecordProcess("Alignment Teach InArm Picker Finish");
                    return true;
                }
                else if(iRet==2)  //Fail
                {
                    Task=1;
                }
                break;
        }
    }
    else if(lAction&INARM_ALIGNMENT_TEACH_LOADER)     //0x3
    {
        switch(Task)
        {
            case 1:
                //KenHsieh 20210830 : add CCD AUTO ALIGNMENT
                //==>
                Cylinder[C_HotplateVibration].Off();
                Cylinder[C_TrayVibration].Off();
                SW[SwShuttleVibration1].Off();
                SW[SwShuttleVibration2].Off();
                SW[SwLoaderVibration].Off();
                if(MOT[MMTrayY].HasIC()==false             &&                   // load and load buffer no any tray
                   MOT[MMTrayY].fHasTray==false            &&
                   MOT[MMTrayY_Car].Tray.HasIC()==false    &&
                   MOT[MMTrayY_Car].fHasTray==false)
                {
                    Task=1;
                    lAction=lAction^INARM_ALIGNMENT_TEACH_LOADER;
                    RecordProcess("Alignment Teach InArm Loader No Tray Pass");
                    return true;
                }
                else
                {
                    if(Cylinder[C_LoaderUpPress].Enable==true &&
                       Sen[SnLoaderUpPress].IsOff()==true)
                    {
                        HTInArmTeachAlignmentDelay.SetSecAndOn(5);              //Kenhsieh 20210915 : 解決C_LoaderUpPress為打出而Hang up
                        if(HTInArmTeachAlignmentDelay.Off())                    //Kenhsieh 20210915 : 解決C_LoaderUpPress為打出而Hang up
                        {
                            Cylinder[C_LoaderUpPress].On();
                        }
                        break;
                    }
                    RecordProcess("Alignment Teach InArm Loader Start");
                    InitInArmXYAutoTeachProcess();
                    Task=50;
                    HTInArmTeachAlignmentDelay.SetSecAndOn(2);
                }
                break;
            case 50:
                if(HTInArmTeachAlignmentDelay.Off())
                {
                    Task=100;
                }
                break;
                //<==
                //KenHsieh 20210830 : add CCD AUTO ALIGNMENT
            case 100:
                iRet=DoInArmXYAlignmentTeachProcess(AOA_Loader, true);
                if(iRet==1)
                {
                    Task=1;
                    lAction=lAction^INARM_ALIGNMENT_TEACH_LOADER;
                    RecordProcess("Alignment Teach InArm Loader Finish");
                    return true;
                }
                else if(iRet==2)
                {
                    Task=1;
                }
                break;
        }
    }
    else if(lAction&INARM_ALIGNMENT_TEACH_AUTOCLEAN)                            //KenHsieh 20211214 : AOA add AutoClean
    {
        switch(Task)
        {
            case 1:
                RecordProcess("Alignment Teach InArm AutoClean Start");
                InitInArmXYAutoTeachProcess();
                Task=100;
                break;
            case 100:
                iRet=DoInArmXYAlignmentTeachProcess(AOA_AutoClean, true);
                if(iRet==1)
                {
                    Task=1;
                    lAction=lAction^INARM_ALIGNMENT_TEACH_AUTOCLEAN;
                    RecordProcess("Alignment Teach InArm AutoClean Finish");
                    return true;
                }
                else if(iRet==2)
                {
                    Task=1;
                }
                break;
        }
    }
    else if(lAction&INARM_ALIGNMENT_TEACH_HOTPLAYE1)  //0x4
    {
        switch(Task)
        {
            case 1:
                RecordProcess("Alignment Teach InArm Hotplate1 Start");
                InitInArmXYAutoTeachProcess();
                Task=100;
                break;
            case 100:
                iRet=DoInArmXYAlignmentTeachProcess(AOA_Plate1, true);
                if(iRet==1)
                {
                    Task=1;
                    lAction=lAction^INARM_ALIGNMENT_TEACH_HOTPLAYE1;
                    RecordProcess("Alignment Teach InArm Hotplate1 Finish");
                    return true;
                }
                else if(iRet==2)
                {
                    Task=1;
                }
                break;
        }
    }
    else if(lAction&INARM_ALIGNMENT_TEACH_HOTPLAYE2)  //0x5
    {
        switch(Task)
        {
            case 1:
                RecordProcess("Alignment Teach InArm Hotplate2 Start");
                InitInArmXYAutoTeachProcess();
                Task=100;
                break;
            case 100:
                iRet=DoInArmXYAlignmentTeachProcess(AOA_Plate2, true);
                if(iRet==1)  //iArea 0=loader 1=plate1 2=plate2 3:Shuttle1 4:Shuttle2
                {
                    Task=1;
                    lAction=lAction^INARM_ALIGNMENT_TEACH_HOTPLAYE2;
                    RecordProcess("Alignment Teach InArm Hotplate2 Finish");
                    return true;
                }
                else if(iRet==2)
                {
                    Task=1;
                }
                break;
        }
    }
    else if(lAction&INARM_ALIGNMENT_TEACH_SHUTTLE1)   //0x6
    {
        switch(Task)
        {
            case 1:
                RecordProcess("Alignment Teach InArm Shuttle1 Start");
                InitInArmXYAutoTeachProcess();
                Task=100;
                break;
            case 100:
                iRet=DoInArmXYAlignmentTeachProcess(AOA_InSht1, true);
                if(iRet==1)
                {
                    Task=1;
                    lAction=lAction^INARM_ALIGNMENT_TEACH_SHUTTLE1;
                    RecordProcess("Alignment Teach InArm Shuttle1 Finish");
                    return true;
                }
                else if(iRet==2)
                {
                    Task=1;
                }
                break;
        }
    }
    else if(lAction&INARM_ALIGNMENT_TEACH_SHUTTLE2)   //0x7
    {
        switch(Task)
        {
            case 1:
                RecordProcess("Alignment Teach InArm Shuttle2 Start");
                InitInArmXYAutoTeachProcess();
                Task=100;
                break;
            case 100:
                iRet=DoInArmXYAlignmentTeachProcess(AOA_InSht2, true);
                if(iRet==1)
                {
                    Task=1;
                    lAction=lAction^INARM_ALIGNMENT_TEACH_SHUTTLE2;
                    RecordProcess("Alignment Teach InArm Shuttle2 Finish");
                    return true;
                }
                else if(iRet==2)
                {
                    Task=1;
                }
                break;
        }
    }
    else if(lAction&INARM_ALIGNMENT_AUTO_UNIT)        //0x8
    {
        switch(Task)
        {
            case 1:
                RecordProcess("Alignment Auto InArm Unit Start");
                InitInArmCCDAlignmentTeachUnitProcess();
                Task=100;
                break;
            case 100:
                iRet=DoInArmCCDAlignmentTeachUnitProces(ResolutionX, ResolutionY, dRadianX, dRadianY, 0);
                if(iRet==1)
                {
                    INIFile=new TIniFile(sPath);
                    INIFile->WriteFloat("InArm", "dInArmCCDXResolution", ResolutionX);         //ChungHung 20210113 add for Alignment CCD
                    INIFile->WriteFloat("InArm", "dInArmCCDYResolution", ResolutionY);         //ChungHung 20210113 add for Alignment CCD
                    INIFile->WriteFloat("InArm", "dInArmCCDXRadian", dRadianX);         //ChungHung 20210113 add for Alignment CCD
                    INIFile->WriteFloat("InArm", "dInArmCCDYRadian", dRadianY);         //ChungHung 20210113 add for Alignment CCD
                    delete INIFile;
                    Task=1;
                    lAction=lAction^INARM_ALIGNMENT_AUTO_UNIT;
                    RecordProcess("Alignment Auto InArm Unit Finish");
                    return true;
                }
                else if(iRet==2)
                {
                    Task=1;
                }
                break;
        }
    }
//    else if(lAction | INARM_ALIGNMENT_AUTO_PICKER)      //0x9
//    {
//        // 待完成
//    }
    else if(lAction&INARM_ALIGNMENT_AUTO_LOADER)      //0xa
    {
        switch(Task)
        {
            case 1:
                //KenHsieh 20210830 : add CCD AUTO ALIGNMENT
                //==>
                Cylinder[C_HotplateVibration].Off();
                Cylinder[C_TrayVibration].Off();
                SW[SwShuttleVibration1].Off();
                SW[SwShuttleVibration2].Off();
                SW[SwLoaderVibration].Off();

                if(MOT[MMTrayY].HasIC()==false             &&   // load and load buffer no any tray
                   MOT[MMTrayY].fHasTray==false            &&
                   MOT[MMTrayY_Car].Tray.HasIC()==false    &&
                   MOT[MMTrayY_Car].fHasTray==false)
                {
                    Task=1;
                    lAction=lAction^INARM_ALIGNMENT_AUTO_LOADER;
                    RecordProcess("Alignment Auto InArm Loader No Tray Pass");
                    return true;
                }
                else
                {
                    RecordProcess("Alignment Auto InArm Loader Start");
                    InitInArmXYAutoTeachProcess();
                    Task=50;
                    HTInArmTeachAlignmentDelay.SetSecAndOn(2);
                }
                break;
            case 50:
                if(HTInArmTeachAlignmentDelay.Off())
                {
                    Task=100;
                }
                break;
                //<==
                //KenHsieh 20210830 : add CCD AUTO ALIGNMENT
            case 100:
                iRet=DoInArmXYAlignmentTeachProcess(AOA_Loader, false);
                if(iRet==1)
                {
                    Task=1;
                    lAction=lAction^INARM_ALIGNMENT_AUTO_LOADER;
                    RecordProcess("Alignment Auto InArm Loader Finish");
                    return true;
                }
                break;
        }
    }
    else if(lAction&INARM_ALIGNMENT_AUTO_AUTOCLEAN)                             //KenHsieh 20211214 : AOA add AutoClean
    {
        switch(Task)
        {
            case 1:
                RecordProcess("Alignment Auto InArm AutoClean Start");
                InitInArmXYAutoTeachProcess();
                Task=100;
                break;
            case 100:
                iRet=DoInArmXYAlignmentTeachProcess(AOA_AutoClean, false);
                if(iRet==1)
                {
                    Task=1;
                    lAction=lAction^INARM_ALIGNMENT_AUTO_AUTOCLEAN;
                    RecordProcess("Alignment Auto InArmAutoClean Finish");
                    return true;
                }
                break;
        }
    }
    else if(lAction&INARM_ALIGNMENT_AUTO_HOTPLAYE1)   //0xb
    {
        switch(Task)
        {
            case 1:
                RecordProcess("Alignment Auto InArm Hotplate1 Start");
                InitInArmXYAutoTeachProcess();
                Task = 100;
                break;
            case 100:
                iRet=DoInArmXYAlignmentTeachProcess(AOA_Plate1, false);
                if(iRet==1)
                {
                    Task=1;
                    lAction=lAction^INARM_ALIGNMENT_AUTO_HOTPLAYE1;
                    RecordProcess("Alignment Auto InArm Hotplate1 Finish");
                    return true;
                }
                break;
        }
    }
    else if(lAction&INARM_ALIGNMENT_AUTO_HOTPLAYE2)   //0xc
    {
        switch(Task)
        {
            case 1:
                RecordProcess("Alignment Auto InArm Hotplate2 Start");
                InitInArmXYAutoTeachProcess();
                Task=100;
                break;
            case 100:
                iRet=DoInArmXYAlignmentTeachProcess(AOA_Plate2, false);
                if(iRet==1)
                {
                    Task=1;
                    lAction=lAction^INARM_ALIGNMENT_AUTO_HOTPLAYE2;
                    RecordProcess("Alignment Auto InArm Hotplate2 Finish");
                    return true;
                }
                break;
            case 200:       //KenHsieh 20210830 : add CCD AUTO ALIGNMENT
                // 2011.06.07 , Joye , Shuttle X-Axis Detect --------------------->>
                if(MOT[MInShuttle1].MotorMove(Prod.InSHT[0].iLeft))
                {
                    bAutoAlignmentInArmShuttle1Lock=false;
                    Task=1;
                    lAction=lAction^INARM_ALIGNMENT_AUTO_HOTPLAYE2;
                    RecordProcess("Alignment Auto InArm Hotplate2 Finish");
                    return true;
                }
                break;
        }
    }
    else if(lAction&INARM_ALIGNMENT_AUTO_SHUTTLE1)    //0xd
    {
        bool bCheckXOnly=false;       //KenHsieh 20211002 : Modify Shuttle Scan
        bool bCheckYOnly=false;       //KenHsieh 20211002 : Modify Shuttle Scan

        switch(Task)
        {
            case 1:
                if(FIX3_FULL_PLACE==Fix3K_UseCylinder || FIX3_FULL_PLACE==Fix3K_UseCylinder46LA)
                {
                    if(bUseFix3CylinderActive==false)    //Ifor 20210719 add:等待FIX3檢測完成
                    {
                        InitialFix3CanFullTask();
                        Task=10;
                    }
                }
                else
                {
                    if(MOT[MInShuttle1].IsCanMove()==false && MOT[MInShuttle2].IsCanMove()==false)
                    {
                        ShowMyMessage("Alignment IndexZ not in Safe Postion!");
                    }
                    else
                    {
                        Task=25;
                    }
                }
                break;
            case 10:
                if(UseFix3Cylinder(0)==true)
                {
                    if(MOT[MInShuttle1].IsCanMove()==false && MOT[MInShuttle2].IsCanMove()==false)
                    {
                        ShowMyMessage("Alignment IndexZ not in Safe Postion!");
                    }
                    else
                    {
                        Task=25;
                    }
                }
                break;
            case 25:
                if(bAutoAlignmentOutArmShuttle1Lock==true)
                {
                    DoMoveAlignmentToInShuttleStage(0, 0, 0, iPosX, iPosY, true, bCheckXOnly, bCheckYOnly);      //Kenhsieh 20211002 : Modify Shuttle Scan
                    return false;
                }

                // 2011.06.07 , Joye , Shuttle X-Axis Detect --------------------->>
                if(MOT[MInShuttle1].MotorMove(Prod.InSHT[0].iLeft))
                {
                    bAutoAlignmentInArmShuttle1Lock=true;
                    Task=50;
                }
                break;
            case 50:
                RecordProcess("Alignment Auto InArm Shuttle1 Start");
                InitInArmXYAutoTeachProcess();
                Task=100;
                break;
            case 100:
                iRet=DoInArmXYAlignmentTeachProcess(AOA_InSht1, false);
                if(iRet==1)
                {
                    Task=200;
                }
                break;
            case 200:
                // 2011.06.07 , Joye , Shuttle X-Axis Detect --------------------->>
                if(MOT[MInShuttle1].MotorMove(Prod.InSHT[0].iLeft))
                {
                    bAutoAlignmentInArmShuttle1Lock=false;
                    Task=1;
                    lAction=lAction^INARM_ALIGNMENT_AUTO_SHUTTLE1;
                    RecordProcess("Alignment Auto InArm Shuttle1 Finish");
                    return true;
                }
                break;
        }
    }
    else if(lAction&INARM_ALIGNMENT_AUTO_SHUTTLE2)    //0xe
    {
        bool bCheckXOnly=false;       //KenHsieh 20211002 : Modify Shuttle Scan
        bool bCheckYOnly=false;       //KenHsieh 20211002 : Modify Shuttle Scan

        switch(Task)
        {
            case 1:
                if(FIX3_FULL_PLACE==Fix3K_UseCylinder || FIX3_FULL_PLACE==Fix3K_UseCylinder46LA)
                {
                    if(bUseFix3CylinderActive==false)    //Ifor 20210719 add:等待FIX3檢測完成
                    {
                        InitialFix3CanFullTask();
                        Task=10;
                    }
                }
                else
                {
                    if(MOT[MInShuttle1].IsCanMove()==false && MOT[MInShuttle2].IsCanMove()==false)
                    {
                        ShowMyMessage("Alignment IndexZ not in Safe Postion!");
                    }
                    else
                    {
                        Task=25;
                    }
                }
                break;
            case 10:
                if(UseFix3Cylinder(0)==true)
                {
                    if(MOT[MInShuttle1].IsCanMove()==false && MOT[MInShuttle2].IsCanMove()==false)
                    {
                        ShowMyMessage("Alignment IndexZ not in Safe Postion!");
                    }
                    else
                    {
                        Task=25;
                    }
                }
                break;
            case 25:
                if(bAutoAlignmentOutArmShuttle2Lock==true)
                {
                    DoMoveAlignmentToInShuttleStage(1, 0, 0, iPosX, iPosY, true, bCheckXOnly, bCheckYOnly);      //Kenhsieh 20211002 : Modify Shuttle Scan
                    return false;
                }

                if(MOT[MInShuttle2].MotorMove(Prod.InSHT[1].iLeft))             // 2011.06.07 , Joye , Shuttle X-Axis Detect --------------------->>
                {
                    bAutoAlignmentInArmShuttle2Lock=true;
                    Task=50;
                }
                break;
            case 50:
                RecordProcess("Alignment Auto InArm Shuttle2 Start");
                InitInArmXYAutoTeachProcess();
                Task=100;
                break;
            case 100:
                iRet=DoInArmXYAlignmentTeachProcess(AOA_InSht2, false);
                if(iRet==1)
                {
                    Task=200;
                }
                break;
            case 200:                                                           //KenHsieh 20210830 : add CCD AUTO ALIGNMENT
                if(MOT[MInShuttle2].MotorMove(Prod.InSHT[1].iLeft))             // 2011.06.07 , Joye , Shuttle X-Axis Detect --------------------->>
                {
                    bAutoAlignmentInArmShuttle2Lock = false;
                    Task=1;
                    lAction=lAction^INARM_ALIGNMENT_AUTO_SHUTTLE2;
                    RecordProcess("Alignment Auto InArm Shuttle2 Finish");
                    return true;
                }
                break;
        }
    }
    return false;
}
//------------------------------------------------------------------------------
int AutoAlignmentInArmSave_SafePosZ[8]={0, 0, 0, 0, 0, 0, 0, 0};
int AutoAlignmentInArmSave_SafePosX=0;
int AutoAlignmentInArmSave_SafePosY=0;

bool bAutoAlignmentInArmSavePosition=false;
int AutoAlignmentInArmMoveType=1;
//------------------------------------------------------------------------------
void ResetMoveAutoAlignmentInArmMoveSavePositionTask()
{
    for(int i=0; i<8; i++)
        AutoAlignmentInArmSave_SafePosZ[i]=0;
    AutoAlignmentInArmSave_SafePosX=0;
    AutoAlignmentInArmSave_SafePosY=0;

    bAutoAlignmentInArmSavePosition=false;
}
//------------------------------------------------------------------------------
void InitialMoveTrayArmInArmMoveSavePositionTask()
{
    AutoAlignmentInArmMoveType=1;
}
//------------------------------------------------------------------------------
bool WhenMoveAutoAlignmentInArmMoveSavePosition( bool MovePos)                  //2012.05.21 Q_Q TrayArm Drop Tray will Move Two Times
{
    int &Task=AutoAlignmentInArmMoveType;
    bool bflagX   =false;
    bool bflagY   =false;
    bool bflagZ[8]={false, false, false, false, false, false, false, false};

    if(MovePos==true)
    {
        switch(Task)
        {
            case 1:

                if(AutoAlignmentInArmSave_SafePosZ[0]==0 && AutoAlignmentInArmSave_SafePosZ[1]==0 && AutoAlignmentInArmSave_SafePosZ[2]==0 && AutoAlignmentInArmSave_SafePosZ[3]==0 &&
                   AutoAlignmentInArmSave_SafePosZ[4]==0 && AutoAlignmentInArmSave_SafePosZ[5]==0 && AutoAlignmentInArmSave_SafePosZ[6]==0 && AutoAlignmentInArmSave_SafePosZ[7]==0 &&
                   AutoAlignmentInArmSave_SafePosX==0 && AutoAlignmentInArmSave_SafePosY==0)
                {
                    AutoAlignmentInArmSave_SafePosZ[0]=MOT[MInArmZA].ReadPos();
                    AutoAlignmentInArmSave_SafePosZ[1]=MOT[MInArmZB].ReadPos();
                    AutoAlignmentInArmSave_SafePosZ[2]=MOT[MInArmZC].ReadPos();
                    AutoAlignmentInArmSave_SafePosZ[3]=MOT[MInArmZD].ReadPos();
                    AutoAlignmentInArmSave_SafePosZ[4]=MOT[MInArmZE].ReadPos();
                    AutoAlignmentInArmSave_SafePosZ[5]=MOT[MInArmZF].ReadPos();
                    AutoAlignmentInArmSave_SafePosZ[6]=MOT[MInArmZG].ReadPos();
                    AutoAlignmentInArmSave_SafePosZ[7]=MOT[MInArmZH].ReadPos();

                    AutoAlignmentInArmSave_SafePosX=MOT[MInArmX].ReadPos();
                    AutoAlignmentInArmSave_SafePosY=MOT[MInArmY].ReadPos();
                }
                Task=2;
                break;
            case 2:
                if(MoveInArmZToPlateSafe(Task))
                {
                    bAutoAlignmentInArmSavePosition=true;
                    Task=999;
                    return true;
                }
                break;
        }
    }
    else
    {
        if(bAutoAlignmentInArmSavePosition)
        {
            switch(Task)
            {
                case 1:
                    bflagX=MOT[MInArmX].MotorMove(AutoAlignmentInArmSave_SafePosX);
                    bflagY=MOT[MInArmY].MotorMove(AutoAlignmentInArmSave_SafePosY);

                    if(bflagX && bflagY)
                    {
                        Task=15;
                    }
                    break;
                case 15:
                    bflagZ[0]=MOT[MInArmZA].MotorMove(AutoAlignmentInArmSave_SafePosZ[0]);
                    bflagZ[1]=MOT[MInArmZB].MotorMove(AutoAlignmentInArmSave_SafePosZ[1]);
                    bflagZ[2]=MOT[MInArmZC].MotorMove(AutoAlignmentInArmSave_SafePosZ[2]);
                    bflagZ[3]=MOT[MInArmZD].MotorMove(AutoAlignmentInArmSave_SafePosZ[3]);
                    bflagZ[4]=MOT[MInArmZE].MotorMove(AutoAlignmentInArmSave_SafePosZ[4]);
                    bflagZ[5]=MOT[MInArmZF].MotorMove(AutoAlignmentInArmSave_SafePosZ[5]);
                    bflagZ[6]=MOT[MInArmZG].MotorMove(AutoAlignmentInArmSave_SafePosZ[6]);
                    bflagZ[7]=MOT[MInArmZH].MotorMove(AutoAlignmentInArmSave_SafePosZ[7]);

                    if(bflagZ[0] && bflagZ[1] && bflagZ[2] && bflagZ[3] && bflagZ[4] && bflagZ[5] && bflagZ[6] && bflagZ[7])
                    {
                        Task=20;
                    }
                    break;
                case 20:
                    bAutoAlignmentInArmSavePosition=false;
                    Task=999;
                    return true;
            }
        }
    }

    return false;
}
//------------------------------------------------------------------------------
int iDoInArmTeachAlignmentProcessTask=1;
//------------------------------------------------------------------------------
void InitDoInArmTeachAlignmentProcessTask()
{
    bAutoAlignmentInArmShuttle1Lock  =false;
    bAutoAlignmentInArmShuttle2Lock  =false;
    iDoInArmTeachAlignmentProcessTask=1;
    iCCDAlignmentMotorDelay=CheckAndReadIniDataGeneral("MachineDefine", "MACHINE_HAS_AUTO_ALIGNMENT_CCD_DELAY", 200);     //KenHsieh 20210830 : add CCD AUTO ALIGNMENT
}
//------------------------------------------------------------------------------
bool DoInArmTeachAlignmentProcess(unsigned long &lAction)
{
    int &Task=iDoInArmTeachAlignmentProcessTask;
    switch(Task)
    {
        case 1:
            fMain->mtInArmAutoAlignmentTray->Visible=true;                      //KenHsieh 20211215 : 修改AOA掃描之顯示Tray，不掃描便不顯示
            ResetMoveAutoAlignmentInArmMoveSavePositionTask();
            InitialMoveTrayArmInArmMoveSavePositionTask();
            Task=100;
            break;
        case 100:
            if(WhenMoveAutoAlignmentInArmMoveSavePosition(true))
            {
                InitInArmAlignment();
                Task=200;
            }
            break;
        case 200:
            DoInArmTeachAlignment(lAction);
            if(lInArmAutoAlignmentFlag==0x0)
            {
                InitialMoveTrayArmInArmMoveSavePositionTask();
                Task=300;
            }
            break;
        case 300:
            if(WhenMoveAutoAlignmentInArmMoveSavePosition(false))
            {
                Task=400;
            }
            break;
        case 400:
            fMain->mtInArmAutoAlignmentTray->Visible=false;                     //KenHsieh 20211215 : 修改AOA掃描之顯示Tray，不掃描便不顯示
            return true;
    }

    return false;
}
//------------------------------------------------------------------------------
bool DoMoveAlignmentToLoaderStage_CCD(int iWhichUnloader, int iRow, int iCol, bool realMove)
{
    int iXPos=0, iYPos=0;                                                       //pig 2016.09.21 SetInitialValue
    bool flag1, flag2;

    if(CheckInArmZ())
    {
        bNeedArmZHome=true;
        InitDoArmZHome();
        return false;
    }

    //KenHsieh 20210829 : Modify add CCD AUTO ALIGNMENT (XY變距)
    //==>
    if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable)
    {
        iXPos=InputAtuoTeachTable[0].pointX[iInArmYBase][iInArmXBase]+(iCol)*Prod.LoadForm.iXPitch+Prod.iInArmCCD_Picker_OffsetX[iInArmYBase][iInArmXBase];  //JerryYang 20250204 : fix AOA
        iYPos=InputAtuoTeachTable[0].pointY[iInArmYBase][iInArmXBase]-(iRow)*Prod.LoadForm.iYPitch+Prod.iInArmCCD_Picker_OffsetY[iInArmYBase][iInArmXBase];
    }
    else
    {
        iXPos=InputAtuoTeachTable[0].pointX[iInArmYBase][iInArmXBase]+(iCol)*Prod.LoadForm.iXPitch+Prod.iInArmCCD_Picker_OffsetX[iInArmYBase][iInArmXBase];
        iYPos=InputAtuoTeachTable[0].pointY[iInArmYBase][iInArmXBase]-(iRow)*Prod.LoadForm.iYPitch+Prod.iInArmCCD_Picker_OffsetY[iInArmYBase][iInArmXBase];
    }
    //<==
    //KenHsieh 20210829 : Modify add CCD AUTO ALIGNMENT (XY變距)

    if(realMove==false)
    {
        if(iXPos>=MOT[MInArmX].Motor->PSoftLimitP || iXPos<=MOT[MInArmX].Motor->PSoftLimitN)
            return false;
        if(iYPos>=MOT[MInArmY].Motor->PSoftLimitP || iYPos<=MOT[MInArmY].Motor->PSoftLimitN)
            return false;
        return true;
    }

    flag1=MOT[MInArmX].MotorMove(iXPos);
    flag2=MOT[MInArmY].MotorMove(iYPos);

    if(flag1 && flag2)
    {
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
int iDoAutoRunLoaderTrayTask=1;
//------------------------------------------------------------------------------
void InitDoAutoRunLoaderTray()
{
    iDoAutoRunLoaderTrayTask=1;
}
//------------------------------------------------------------------------------
bool DoAutoRunLoaderTray(int iArea)
{
    int &Task=iDoAutoRunLoaderTrayTask;
    static int iRow=0, iCol=0;
    int iXPos, iYPos;
    double dOffsetX, dOffsetY;
    int iRet=0;
    switch(Task)
    {
        case 1:                                                                 //初始化Tray
            ReadAutoTeachTable_InArm();                                         //KenHsieh 20211208 : In/Out Arm分開讀檔，避免InArm做完直接寫入導致OutArm點位錯誤
            MOT[MMInArmAOATray].Tray.XItem=LoadForm->XDivision;
            MOT[MMInArmAOATray].Tray.YItem=LoadForm->YDivision;
            MOT[MMInArmAOATray].InitNewTray(WAIT_ALIGN_IC, false, __FUNC__);
            Task=100;
            break;
        case 100:                                                               //Move To Target
            iRet=SearchInArmAlignmentUpDown(iArea, iRow, iCol, iXPos, iYPos);
            if(iRet==1)
            {
                Task=200;
            }
            else if(iRet==0)
            {
                Task=1000;
            }
            break;
        case 200:
            if(DoMoveAlignmentToLoaderStage_CCD(iArea, iRow, iCol, true))
            {
                HInArmCCDAlignmentMotorDelay.SetMSAndOn(iCCDAlignmentMotorDelay);
                Task=250;
            }
            break;
        case 250:
            if(HInArmCCDAlignmentMotorDelay.Off()==false)
                break;
            MOT[MMInArmAOATray].SetTraySingleData(iCol, iRow, NULL_IC);
            fAutoAlignment->InArmSendCommand(AUTO_TRAY);
            Task=300;
            break;
        case 300:
            if(fAutoAlignment->InArmGetResult(AUTO_TRAY, dOffsetX, dOffsetY))
            {
                Task=100;
            }
            break;
        case 1000:
            return true;
    }
    return false;
}
//------------------------------------------------------------------------------
void CheckInArmXYScaleByAutoTeach(int &iXPos, int &iYPos, int iArea)
{
    if(MACHINE_HAS_AUTO_ALIGNMENT_CCD && TestIF.bEnableAutoAlignment==false)
        return ;

    if(iArea>=TotalInArmAOAType)                                                //JerryYang 20240306 : 增加保護避免超出陣列
    {
        ShowMyMessage("In Arm Auto Alignment data error.");
        return;
    }

    bool bBreak=false;
    double iRealXMatrix[2][2]={{0, 0}, {0, 0}};
    double iRealYMatrix[2][2]={{0, 0}, {0, 0}};
    double iXMatrixCal[2][2]={{0, 0}, {0, 0}};
    double iYMatrixCal[2][2]={{0, 0}, {0, 0}};

    const int iMaxItemX=InputAtuoTeachTableCal[iArea].ItemX;
    const int iMaxItemY=InputAtuoTeachTableCal[iArea].ItemY;

    for(int x=0; x<iMaxItemX-1; x++)
    {
        for(int y=0; y<iMaxItemY-1; y++)
        {
            iXMatrixCal[0][0]=InputAtuoTeachTableCal[iArea].pointX[x][y];       //left top
            iYMatrixCal[0][0]=InputAtuoTeachTableCal[iArea].pointY[x][y];

            iXMatrixCal[1][0]=InputAtuoTeachTableCal[iArea].pointX[x+1][y];     //right top
            iYMatrixCal[1][0]=InputAtuoTeachTableCal[iArea].pointY[x+1][y];

            iXMatrixCal[0][1]=InputAtuoTeachTableCal[iArea].pointX[x][y+1];     //left bottom
            iYMatrixCal[0][1]=InputAtuoTeachTableCal[iArea].pointY[x][y+1];

            iXMatrixCal[1][1]=InputAtuoTeachTableCal[iArea].pointX[x+1][y+1];   //right bottom
            iYMatrixCal[1][1]=InputAtuoTeachTableCal[iArea].pointY[x+1][y+1];

            if(iXPos==iXMatrixCal[0][0] && iYPos==iYMatrixCal[0][0])
            {
                iRealXMatrix[0][0]=InputAtuoTeachTable[iArea].pointX[x][y];     //left top
                iRealYMatrix[0][0]=InputAtuoTeachTable[iArea].pointY[x][y];
                iXPos=iRealXMatrix[0][0];
                iYPos=iRealYMatrix[0][0];
                switch(iArea)                                                   //KenHsieh 20210830 : add CCD AUTO ALIGNMENT //KenHsieh 20210913 : CCD AUTO ALIGNMENT offset修改
                {
                    case AOA_Loader:
                        iXPos=iXPos+atoi(fMain->Ed_LoaderOffset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_LoaderOffset_Y->Text.c_str());
                        break;
                    case AOA_Plate1:
                        iXPos=iXPos+atoi(fMain->Ed_HP1Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_HP1Offset_Y->Text.c_str());
                        break;
                    case AOA_Plate2:
                        iXPos=iXPos+atoi(fMain->Ed_HP2Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_HP2Offset_Y->Text.c_str());
                        break;
                    case AOA_InSht1:
                        iXPos=iXPos+atoi(fMain->Ed_InSH1Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_InSH1Offset_Y->Text.c_str());
                        break;
                    case AOA_InSht2:
                        iXPos=iXPos+atoi(fMain->Ed_InSH2Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_InSH2Offset_Y->Text.c_str());
                        break;
                }
                return;
            }
            else if(iXPos==iXMatrixCal[1][0] && iYPos==iYMatrixCal[1][0])
            {
                iRealXMatrix[1][0]=InputAtuoTeachTable[iArea].pointX[x+1][y];   //right top
                iRealYMatrix[1][0]=InputAtuoTeachTable[iArea].pointY[x+1][y];
                iXPos=iRealXMatrix[1][0];
                iYPos=iRealYMatrix[1][0];
                switch(iArea)                                                   //KenHsieh 20210830 : add CCD AUTO ALIGNMENT //KenHsieh 20210913 : CCD AUTO ALIGNMENT offset修改
                {
                    case AOA_Loader:
                        iXPos=iXPos+atoi(fMain->Ed_LoaderOffset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_LoaderOffset_Y->Text.c_str());
                        break;
                    case AOA_Plate1:
                        iXPos=iXPos+atoi(fMain->Ed_HP1Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_HP1Offset_Y->Text.c_str());
                        break;
                    case AOA_Plate2:
                        iXPos=iXPos+atoi(fMain->Ed_HP2Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_HP2Offset_Y->Text.c_str());
                        break;
                    case AOA_InSht1:
                        iXPos=iXPos+atoi(fMain->Ed_InSH1Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_InSH1Offset_Y->Text.c_str());
                        break;
                    case AOA_InSht2:
                        iXPos=iXPos+atoi(fMain->Ed_InSH2Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_InSH2Offset_Y->Text.c_str());
                        break;
                }
                return;
            }
            else if(iXPos==iXMatrixCal[0][1] && iYPos==iYMatrixCal[0][1])
            {
                iRealXMatrix[0][1]=InputAtuoTeachTable[iArea].pointX[x][y+1];   //left bottom
                iRealYMatrix[0][1]=InputAtuoTeachTable[iArea].pointY[x][y+1];
                iXPos=iRealXMatrix[0][1];
                iYPos=iRealYMatrix[0][1];
                switch(iArea)                                                   //KenHsieh 20210830 : add CCD AUTO ALIGNMENT //KenHsieh 20210913 : CCD AUTO ALIGNMENT offset修改
                {
                    case AOA_Loader:
                        iXPos=iXPos+atoi(fMain->Ed_LoaderOffset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_LoaderOffset_Y->Text.c_str());
                        break;
                    case AOA_Plate1:
                        iXPos=iXPos+atoi(fMain->Ed_HP1Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_HP1Offset_Y->Text.c_str());
                        break;
                    case AOA_Plate2:
                        iXPos=iXPos+atoi(fMain->Ed_HP2Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_HP2Offset_Y->Text.c_str());
                        break;
                    case AOA_InSht1:
                        iXPos=iXPos+atoi(fMain->Ed_InSH1Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_InSH1Offset_Y->Text.c_str());
                        break;
                    case AOA_InSht2:
                        iXPos=iXPos+atoi(fMain->Ed_InSH2Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_InSH2Offset_Y->Text.c_str());
                        break;
                }
                return;
            }
            else if(iXPos==iXMatrixCal[1][1] && iYPos==iYMatrixCal[1][1])
            {
                iRealXMatrix[1][1]=InputAtuoTeachTable[iArea].pointX[x+1][y+1]; //right bottom
                iRealYMatrix[1][1]=InputAtuoTeachTable[iArea].pointY[x+1][y+1];
                iXPos=iRealXMatrix[1][1];
                iYPos=iRealYMatrix[1][1];
                switch(iArea)                                                   //KenHsieh 20210830 : add CCD AUTO ALIGNMENT //KenHsieh 20210913 : CCD AUTO ALIGNMENT offset修改
                {
                    case AOA_Loader:
                        iXPos=iXPos+atoi(fMain->Ed_LoaderOffset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_LoaderOffset_Y->Text.c_str());
                        break;
                    case AOA_Plate1:
                        iXPos=iXPos+atoi(fMain->Ed_HP1Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_HP1Offset_Y->Text.c_str());
                        break;
                    case AOA_Plate2:
                        iXPos=iXPos+atoi(fMain->Ed_HP2Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_HP2Offset_Y->Text.c_str());
                        break;
                    case AOA_InSht1:
                        iXPos=iXPos+atoi(fMain->Ed_InSH1Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_InSH1Offset_Y->Text.c_str());
                        break;
                    case AOA_InSht2:
                        iXPos=iXPos+atoi(fMain->Ed_InSH2Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_InSH2Offset_Y->Text.c_str());
                        break;
                }
                return;
            }
            else if(iXPos>=iXMatrixCal[0][0] && iXPos<iXMatrixCal[1][0] && iXPos>=iXMatrixCal[0][1] && iXPos<iXMatrixCal[1][1] &&
                    iYPos<=iYMatrixCal[0][0] && iYPos<=iYMatrixCal[1][0] && iYPos>iYMatrixCal[0][1] && iYPos>iYMatrixCal[1][1])
            {
                iRealXMatrix[0][0]=InputAtuoTeachTable[iArea].pointX[x][y];                 //left top
                iRealYMatrix[0][0]=InputAtuoTeachTable[iArea].pointY[x][y];

                iRealXMatrix[1][0]=InputAtuoTeachTable[iArea].pointX[x+1][y];               //right top
                iRealYMatrix[1][0]=InputAtuoTeachTable[iArea].pointY[x+1][y];

                iRealXMatrix[0][1]=InputAtuoTeachTable[iArea].pointX[x][y+1];               //left bottom
                iRealYMatrix[0][1]=InputAtuoTeachTable[iArea].pointY[x][y+1];

                iRealXMatrix[1][1]=InputAtuoTeachTable[iArea].pointX[x+1][y+1];             //right bottom
                iRealYMatrix[1][1]=InputAtuoTeachTable[iArea].pointY[x+1][y+1];
                bBreak=true;
                break;
            }
        }

        if(bBreak)
            break;
    }

    if(bBreak==false)
    {
        iXMatrixCal[0][0]=InputAtuoTeachTableCal[iArea].pointX[0][0];                       //left top
        iYMatrixCal[0][0]=InputAtuoTeachTableCal[iArea].pointY[0][0];

        iXMatrixCal[1][0]=InputAtuoTeachTableCal[iArea].pointX[iMaxItemX-1][0];             //right top
        iYMatrixCal[1][0]=InputAtuoTeachTableCal[iArea].pointY[iMaxItemX-1][0];

        iXMatrixCal[0][1]=InputAtuoTeachTableCal[iArea].pointX[0][iMaxItemY-1];             //left bottom
        iYMatrixCal[0][1]=InputAtuoTeachTableCal[iArea].pointY[0][iMaxItemY-1];

        iXMatrixCal[1][1]=InputAtuoTeachTableCal[iArea].pointX[iMaxItemX-1][iMaxItemY-1];   //right bottom
        iYMatrixCal[1][1]=InputAtuoTeachTableCal[iArea].pointY[iMaxItemX-1][iMaxItemY-1];

        iRealXMatrix[0][0]=InputAtuoTeachTable[iArea].pointX[0][0];                         //left top
        iRealYMatrix[0][0]=InputAtuoTeachTable[iArea].pointY[0][0];

        iRealXMatrix[1][0]=InputAtuoTeachTable[iArea].pointX[iMaxItemX-1][0];               //right top
        iRealYMatrix[1][0]=InputAtuoTeachTable[iArea].pointY[iMaxItemX-1][0];

        iRealXMatrix[0][1]=InputAtuoTeachTable[iArea].pointX[0][iMaxItemY-1];               //left bottom
        iRealYMatrix[0][1]=InputAtuoTeachTable[iArea].pointY[0][iMaxItemY-1];

        iRealXMatrix[1][1]=InputAtuoTeachTable[iArea].pointX[iMaxItemX-1][iMaxItemY-1];     //right bottom
        iRealYMatrix[1][1]=InputAtuoTeachTable[iArea].pointY[iMaxItemX-1][iMaxItemY-1];
    }

    double dOffsetX, dOffsetY, dDisX, dDisY;
    double dCalLength, dRealLength, ResolutionX, ResolutionY, dRadianX, dRadianY;

    //原點對齊
    // 0:L 1:R 0:T 1:B
    dCalLength  =(iXMatrixCal[1][0]-iXMatrixCal[0][0])*(iXMatrixCal[1][0]-iXMatrixCal[0][0])+(iYMatrixCal[1][0]-iYMatrixCal[0][0])*(iYMatrixCal[1][0]-iYMatrixCal[0][0]);
    dRealLength =(iRealXMatrix[1][0]-iRealXMatrix[0][0])*(iRealXMatrix[1][0]-iRealXMatrix[0][0])+(iRealYMatrix[1][0]-iRealYMatrix[0][0])*(iRealYMatrix[1][0]-iRealYMatrix[0][0]);
    if(dCalLength==0 || dRealLength==0)
    {
        ResolutionX=1;
        dRadianX=0;
    }
    else
    {
        ResolutionX =ChangeToFloatNonPcnt((double)(sqrt(fabs(dRealLength))), (double)(sqrt(fabs(dCalLength))));
        dRadianX    =atan(((double)iRealYMatrix[1][0]-(double)iRealYMatrix[0][0])/((double)iRealXMatrix[1][0]-(double)iRealXMatrix[0][0]));
    }

    dCalLength  =(iXMatrixCal[0][1]-iXMatrixCal[0][0])*(iXMatrixCal[0][1]-iXMatrixCal[0][0])+(iYMatrixCal[0][1]-iYMatrixCal[0][0])*(iYMatrixCal[0][1]-iYMatrixCal[0][0]);
    dRealLength =(iRealXMatrix[0][1]-iRealXMatrix[0][0])*(iRealXMatrix[0][1]-iRealXMatrix[0][0])+(iRealYMatrix[0][1]-iRealYMatrix[0][0])*(iRealYMatrix[0][1]-iRealYMatrix[0][0]);
    if(dCalLength==0 || dRealLength==0)
    {
        ResolutionY=1;
        dRadianY=0;
    }
    else
    {
        ResolutionY=ChangeToFloatNonPcnt((double)(sqrt(fabs(dRealLength))), (double)(sqrt(fabs(dCalLength))));
        dRadianY   =atan(-((double)iRealXMatrix[0][0]-(double)iRealXMatrix[0][1])/((double)iRealYMatrix[0][0]-(double)iRealYMatrix[0][1]));
    }

    dOffsetX=0;
    dOffsetY=0;

    double px=iXPos-iXMatrixCal[0][0]+dOffsetX;
    double py=iYPos-iYMatrixCal[0][0]+dOffsetY;

    RotationCoordinates(px, py, dDisX, dDisY, dRadianX, dRadianY, false);

    dDisX=dDisX*ResolutionX;
    dDisY=dDisY*ResolutionY;

    double dXpostion=iXPos;
    double dYpostion=iYPos;

    iXPos=iRealXMatrix[0][0]+Round(dDisX);
    iYPos=iRealYMatrix[0][0]+Round(dDisY);

    switch(iArea)                                                               //KenHsieh 20210830 : add CCD AUTO ALIGNMENT for Offset //KenHsieh 20210913 : CCD AUTO ALIGNMENT offset修改
    {
        case AOA_Loader:
            fMain->labLoader_X->Caption=dXpostion-iXPos;
            fMain->labLoader_Y->Caption=dYpostion-iYPos;
            iXPos=iXPos+atoi(fMain->Ed_LoaderOffset_X->Text.c_str());
            iYPos=iYPos+atoi(fMain->Ed_LoaderOffset_Y->Text.c_str());
            break;
        case AOA_Plate1:
            fMain->labHotplate1_X->Caption=dXpostion-iXPos;
            fMain->labHotplate1_Y->Caption=dYpostion-iYPos;
            iXPos=iXPos+atoi(fMain->Ed_HP1Offset_X->Text.c_str());
            iYPos=iYPos+atoi(fMain->Ed_HP1Offset_Y->Text.c_str());
            break;
        case AOA_Plate2:
            fMain->labHotplate2_X->Caption=dXpostion-iXPos;
            fMain->labHotplate2_Y->Caption=dYpostion-iYPos;
            iXPos=iXPos+atoi(fMain->Ed_HP2Offset_X->Text.c_str());
            iYPos=iYPos+atoi(fMain->Ed_HP2Offset_Y->Text.c_str());
            break;
        case AOA_InSht1:
            fMain->labInShuttle1_X->Caption=dXpostion-iXPos;
            fMain->labInShuttle1_Y->Caption=dYpostion-iYPos;
            iXPos=iXPos+atoi(fMain->Ed_InSH1Offset_X->Text.c_str());
            iYPos=iYPos+atoi(fMain->Ed_InSH1Offset_Y->Text.c_str());
            break;
        case AOA_InSht2:
            fMain->labInShuttle2_X->Caption=dXpostion-iXPos;
            fMain->labInShuttle2_Y->Caption=dYpostion-iYPos;
            iXPos=iXPos+atoi(fMain->Ed_InSH2Offset_X->Text.c_str());
            iYPos=iYPos+atoi(fMain->Ed_InSH2Offset_Y->Text.c_str());
            break;
    }
    return;
}
//ChungHung 20200207 add for AutoTeach end
//------------------------------------------------------------------------------
//KenHsieh 20210813 : add CCD AUTO ALIGNMENT
//==>
//ChungHung 20210113 add for Alignment CCD start
void CheckOutArmXYScaleByAutoTeach(int &iXPos, int &iYPos, int iArea)
{
    if(MACHINE_HAS_AUTO_ALIGNMENT_CCD && TestIF.bEnableAutoAlignment==false)
        return ;

    if(iArea>=TotalOutArmAOAType)                                               //JerryYang 20240306 : 增加保護避免超出陣列
    {
        ShowMyMessage("Out Arm Auto Alignment data error.");
        return;
    }

    bool bBreak=false;
    double iRealXMatrix[2][2]={{0, 0}, {0, 0}};
    double iRealYMatrix[2][2]={{0, 0}, {0, 0}};
    double iXMatrixCal[2][2]={{0, 0}, {0, 0}};
    double iYMatrixCal[2][2]={{0, 0}, {0, 0}};

    const int iMaxItemX = OutputAtuoTeachTableCal[iArea].ItemX;
    const int iMaxItemY = OutputAtuoTeachTableCal[iArea].ItemY;

    for(int x=0; x<iMaxItemX-1; x++)
    {
        for(int y=0; y<iMaxItemY-1; y++)
        {
            iXMatrixCal[0][0]=OutputAtuoTeachTableCal[iArea].pointX[x][y];      //left top
            iYMatrixCal[0][0]=OutputAtuoTeachTableCal[iArea].pointY[x][y];

            iXMatrixCal[1][0]=OutputAtuoTeachTableCal[iArea].pointX[x+1][y];    //right top
            iYMatrixCal[1][0]=OutputAtuoTeachTableCal[iArea].pointY[x+1][y];

            iXMatrixCal[0][1]=OutputAtuoTeachTableCal[iArea].pointX[x][y+1];    //left bottom
            iYMatrixCal[0][1]=OutputAtuoTeachTableCal[iArea].pointY[x][y+1];

            iXMatrixCal[1][1]=OutputAtuoTeachTableCal[iArea].pointX[x+1][y+1];  //right bottom
            iYMatrixCal[1][1]=OutputAtuoTeachTableCal[iArea].pointY[x+1][y+1];

            if(iXPos==iXMatrixCal[0][0] && iYPos==iYMatrixCal[0][0])
            {
                iRealXMatrix[0][0]=OutputAtuoTeachTable[iArea].pointX[x][y];    //left top
                iRealYMatrix[0][0]=OutputAtuoTeachTable[iArea].pointY[x][y];
                iXPos=iRealXMatrix[0][0];
                iYPos=iRealYMatrix[0][0];
                switch(iArea)                                                   //KenHsieh 20210915 : CCD AUTO ALIGNMENT offset新增
                {
                    case AOA_Auto1:
                        iXPos=iXPos+atoi(fMain->Ed_Auto1Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Auto1Offset_Y->Text.c_str());
                        break;
                    case AOA_Auto2:
                        iXPos=iXPos+atoi(fMain->Ed_Auto2Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Auto2Offset_Y->Text.c_str());
                        break;
                    case AOA_Auto3:
                        iXPos=iXPos+atoi(fMain->Ed_Auto3Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Auto3Offset_X->Text.c_str());
                        break;
                    case AOA_Auto4:                                             //Steven 20240428 : Add for HT9011 AOA
                        iXPos=iXPos+atoi(fMain->Ed_Auto4Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Auto4Offset_Y->Text.c_str());
                        break;
                    case AOA_Auto5:
                        iXPos=iXPos+atoi(fMain->Ed_Auto5Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Auto5Offset_Y->Text.c_str());
                        break;
                    case AOA_Auto6:
                        iXPos=iXPos+atoi(fMain->Ed_Auto6Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Auto6Offset_X->Text.c_str());
                        break;
                    case AOA_Fix1:
                        iXPos=iXPos+atoi(fMain->Ed_Fix1Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Fix1Offset_Y->Text.c_str());
                        break;
                    case AOA_Fix2:
                        iXPos=iXPos+atoi(fMain->Ed_Fix2Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Fix2Offset_Y->Text.c_str());
                        break;
                    case AOA_Fix3:
                        iXPos=iXPos+atoi(fMain->Ed_Fix3Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Fix3Offset_Y->Text.c_str());
                        break;
                    case AOA_Fix4:                                              //Steven 20240428 : Add for HT9011 AOA
                        iXPos=iXPos+atoi(fMain->Ed_Fix4Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Fix4Offset_Y->Text.c_str());
                        break;
                    case AOA_Fix5:
                        iXPos=iXPos+atoi(fMain->Ed_Fix5Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Fix5Offset_Y->Text.c_str());
                        break;
                    case AOA_Fix6:
                        iXPos=iXPos+atoi(fMain->Ed_Fix6Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Fix6Offset_Y->Text.c_str());
                        break;
                    case AOA_OutSht1:
                        iXPos=iXPos+atoi(fMain->Ed_OutSH1Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_OutSH1Offset_Y->Text.c_str());
                        break;
                    case AOA_OutSht2:
                        iXPos=iXPos+atoi(fMain->Ed_OutSH2Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_OutSH2Offset_Y->Text.c_str());
                        break;
                }
                return;
            }
            else if(iXPos==iXMatrixCal[1][0] && iYPos==iYMatrixCal[1][0])
            {
                iRealXMatrix[1][0]=OutputAtuoTeachTable[iArea].pointX[x+1][y];  //right top
                iRealYMatrix[1][0]=OutputAtuoTeachTable[iArea].pointY[x+1][y];
                iXPos=iRealXMatrix[1][0];
                iYPos=iRealYMatrix[1][0];
                switch(iArea)                                                   //KenHsieh 20210915 : CCD AUTO ALIGNMENT offset新增
                {
                    case AOA_Auto1:
                        iXPos=iXPos+atoi(fMain->Ed_Auto1Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Auto1Offset_Y->Text.c_str());
                        break;
                    case AOA_Auto2:
                        iXPos=iXPos+atoi(fMain->Ed_Auto2Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Auto2Offset_Y->Text.c_str());
                        break;
                    case AOA_Auto3:
                        iXPos=iXPos+atoi(fMain->Ed_Auto3Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Auto3Offset_X->Text.c_str());
                        break;
                    case AOA_Auto4:                                             //Steven 20240428 : Add for HT9011 AOA
                        iXPos=iXPos+atoi(fMain->Ed_Auto4Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Auto4Offset_Y->Text.c_str());
                        break;
                    case AOA_Auto5:
                        iXPos=iXPos+atoi(fMain->Ed_Auto5Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Auto5Offset_Y->Text.c_str());
                        break;
                    case AOA_Auto6:
                        iXPos=iXPos+atoi(fMain->Ed_Auto6Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Auto6Offset_X->Text.c_str());
                        break;
                    case AOA_Fix1:
                        iXPos=iXPos+atoi(fMain->Ed_Fix1Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Fix1Offset_Y->Text.c_str());
                      break;
                    case AOA_Fix2:
                        iXPos=iXPos+atoi(fMain->Ed_Fix2Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Fix2Offset_Y->Text.c_str());
                        break;
                    case AOA_Fix3:
                        iXPos=iXPos+atoi(fMain->Ed_Fix3Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Fix3Offset_Y->Text.c_str());
                        break;
                    case AOA_Fix4:                                              //Steven 20240428 : Add for HT9011 AOA
                        iXPos=iXPos+atoi(fMain->Ed_Fix4Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Fix4Offset_Y->Text.c_str());
                      break;
                    case AOA_Fix5:
                        iXPos=iXPos+atoi(fMain->Ed_Fix5Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Fix5Offset_Y->Text.c_str());
                        break;
                    case AOA_Fix6:
                        iXPos=iXPos+atoi(fMain->Ed_Fix6Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Fix6Offset_Y->Text.c_str());
                        break;
                    case AOA_OutSht1:
                        iXPos=iXPos+atoi(fMain->Ed_OutSH1Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_OutSH1Offset_Y->Text.c_str());
                        break;
                    case AOA_OutSht2:
                        iXPos=iXPos+atoi(fMain->Ed_OutSH2Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_OutSH2Offset_Y->Text.c_str());
                        break;
                }
                return;
            }
            else if(iXPos==iXMatrixCal[0][1] && iYPos==iYMatrixCal[0][1])
            {
                iRealXMatrix[0][1]=OutputAtuoTeachTable[iArea].pointX[x][y+1];  //left bottom
                iRealYMatrix[0][1]=OutputAtuoTeachTable[iArea].pointY[x][y+1];
                iXPos=iRealXMatrix[0][1];
                iYPos=iRealYMatrix[0][1];
                switch(iArea)                                                   //KenHsieh 20210915 : CCD AUTO ALIGNMENT offset新增
                {
                    case AOA_Auto1:
                        iXPos=iXPos+atoi(fMain->Ed_Auto1Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Auto1Offset_Y->Text.c_str());
                        break;
                    case AOA_Auto2:
                        iXPos=iXPos+atoi(fMain->Ed_Auto2Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Auto2Offset_Y->Text.c_str());
                        break;
                    case AOA_Auto3:
                        iXPos=iXPos+atoi(fMain->Ed_Auto3Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Auto3Offset_X->Text.c_str());
                        break;
                    case AOA_Auto4:                                             //Steven 20240428 : Add for HT9011 AOA
                        iXPos=iXPos+atoi(fMain->Ed_Auto4Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Auto4Offset_Y->Text.c_str());
                        break;
                    case AOA_Auto5:
                        iXPos=iXPos+atoi(fMain->Ed_Auto5Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Auto5Offset_Y->Text.c_str());
                        break;
                    case AOA_Auto6:
                        iXPos=iXPos+atoi(fMain->Ed_Auto6Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Auto6Offset_X->Text.c_str());
                        break;
                    case AOA_Fix1:
                        iXPos=iXPos+atoi(fMain->Ed_Fix1Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Fix1Offset_Y->Text.c_str());
                        break;
                    case AOA_Fix2:
                        iXPos=iXPos+atoi(fMain->Ed_Fix2Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Fix2Offset_Y->Text.c_str());
                        break;
                    case AOA_Fix3:
                        iXPos=iXPos+atoi(fMain->Ed_Fix3Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Fix3Offset_Y->Text.c_str());
                        break;
                    case AOA_Fix4:                                              //Steven 20240428 : Add for HT9011 AOA
                        iXPos=iXPos+atoi(fMain->Ed_Fix4Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Fix4Offset_Y->Text.c_str());
                        break;
                    case AOA_Fix5:
                        iXPos=iXPos+atoi(fMain->Ed_Fix5Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Fix5Offset_Y->Text.c_str());
                        break;
                    case AOA_Fix6:
                        iXPos=iXPos+atoi(fMain->Ed_Fix6Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Fix6Offset_Y->Text.c_str());
                        break;
                    case AOA_OutSht1:
                        iXPos=iXPos+atoi(fMain->Ed_OutSH1Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_OutSH1Offset_Y->Text.c_str());
                        break;
                    case AOA_OutSht2:
                        iXPos=iXPos+atoi(fMain->Ed_OutSH2Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_OutSH2Offset_Y->Text.c_str());
                        break;
                }
                return;
            }
            else if(iXPos==iXMatrixCal[1][1] && iYPos==iYMatrixCal[1][1])
            {
                iRealXMatrix[1][1]=OutputAtuoTeachTable[iArea].pointX[x+1][y+1];//right bottom
                iRealYMatrix[1][1]=OutputAtuoTeachTable[iArea].pointY[x+1][y+1];
                iXPos=iRealXMatrix[1][1];
                iYPos=iRealYMatrix[1][1];
                switch(iArea)                                                   //KenHsieh 20210915 : CCD AUTO ALIGNMENT offset新增
                {
                    case AOA_Auto1:
                        iXPos=iXPos+atoi(fMain->Ed_Auto1Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Auto1Offset_Y->Text.c_str());
                        break;
                    case AOA_Auto2:
                        iXPos=iXPos+atoi(fMain->Ed_Auto2Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Auto2Offset_Y->Text.c_str());
                        break;
                    case AOA_Auto3:
                        iXPos=iXPos+atoi(fMain->Ed_Auto3Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Auto3Offset_X->Text.c_str());
                        break;
                    case AOA_Auto4:                                             //Steven 20240428 : Add for HT9011 AOA
                        iXPos=iXPos+atoi(fMain->Ed_Auto4Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Auto4Offset_Y->Text.c_str());
                        break;
                    case AOA_Auto5:
                        iXPos=iXPos+atoi(fMain->Ed_Auto5Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Auto5Offset_Y->Text.c_str());
                        break;
                    case AOA_Auto6:
                        iXPos=iXPos+atoi(fMain->Ed_Auto6Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Auto6Offset_X->Text.c_str());
                        break;
                    case AOA_Fix1:
                        iXPos=iXPos+atoi(fMain->Ed_Fix1Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Fix1Offset_Y->Text.c_str());
                        break;
                    case AOA_Fix2:
                        iXPos=iXPos+atoi(fMain->Ed_Fix2Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Fix2Offset_Y->Text.c_str());
                        break;
                    case AOA_Fix3:
                        iXPos=iXPos+atoi(fMain->Ed_Fix3Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Fix3Offset_Y->Text.c_str());
                        break;
                    case AOA_Fix4:                                              //Steven 20240428 : Add for HT9011 AOA
                        iXPos=iXPos+atoi(fMain->Ed_Fix4Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Fix4Offset_Y->Text.c_str());
                        break;
                    case AOA_Fix5:
                        iXPos=iXPos+atoi(fMain->Ed_Fix5Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Fix5Offset_Y->Text.c_str());
                        break;
                    case AOA_Fix6:
                        iXPos=iXPos+atoi(fMain->Ed_Fix6Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_Fix6Offset_Y->Text.c_str());
                        break;
                    case AOA_OutSht1:
                        iXPos=iXPos+atoi(fMain->Ed_OutSH1Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_OutSH1Offset_Y->Text.c_str());
                        break;
                    case AOA_OutSht2:
                        iXPos=iXPos+atoi(fMain->Ed_OutSH2Offset_X->Text.c_str());
                        iYPos=iYPos+atoi(fMain->Ed_OutSH2Offset_Y->Text.c_str());
                        break;
                }
                return;
            }
            else if(iXPos>=iXMatrixCal[0][0] && iXPos<iXMatrixCal[1][0] && iXPos>=iXMatrixCal[0][1] && iXPos<iXMatrixCal[1][1] &&
                    iYPos<=iYMatrixCal[0][0] && iYPos<=iYMatrixCal[1][0] && iYPos>iYMatrixCal[0][1] && iYPos>iYMatrixCal[1][1]   )
            {
                iRealXMatrix[0][0]=OutputAtuoTeachTable[iArea].pointX[x][y];                    //left top
                iRealYMatrix[0][0]=OutputAtuoTeachTable[iArea].pointY[x][y];

                iRealXMatrix[1][0]=OutputAtuoTeachTable[iArea].pointX[x+1][y];                  //right top
                iRealYMatrix[1][0]=OutputAtuoTeachTable[iArea].pointY[x+1][y];

                iRealXMatrix[0][1]=OutputAtuoTeachTable[iArea].pointX[x][y+1];                  //left bottom
                iRealYMatrix[0][1]=OutputAtuoTeachTable[iArea].pointY[x][y+1];

                iRealXMatrix[1][1]=OutputAtuoTeachTable[iArea].pointX[x+1][y+1];                //right bottom
                iRealYMatrix[1][1]=OutputAtuoTeachTable[iArea].pointY[x+1][y+1];

                bBreak=true;
                break;
            }
        }

        if(bBreak)
            break;
    }

    if(bBreak==false)
    {
        iXMatrixCal[0][0]=OutputAtuoTeachTableCal[iArea].pointX[0][0];                          //left top
        iYMatrixCal[0][0]=OutputAtuoTeachTableCal[iArea].pointY[0][0];

        iXMatrixCal[1][0]=OutputAtuoTeachTableCal[iArea].pointX[iMaxItemX-1][0];                //right top
        iYMatrixCal[1][0]=OutputAtuoTeachTableCal[iArea].pointY[iMaxItemX-1][0];

        iXMatrixCal[0][1]=OutputAtuoTeachTableCal[iArea].pointX[0][iMaxItemY-1];                //left bottom
        iYMatrixCal[0][1]=OutputAtuoTeachTableCal[iArea].pointY[0][iMaxItemY-1];

        iXMatrixCal[1][1]=OutputAtuoTeachTableCal[iArea].pointX[iMaxItemX-1][iMaxItemY-1];      //right bottom
        iYMatrixCal[1][1]=OutputAtuoTeachTableCal[iArea].pointY[iMaxItemX-1][iMaxItemY-1];

        iRealXMatrix[0][0]=OutputAtuoTeachTable[iArea].pointX[0][0];                            //left top
        iRealYMatrix[0][0]=OutputAtuoTeachTable[iArea].pointY[0][0];

        iRealXMatrix[1][0]=OutputAtuoTeachTable[iArea].pointX[iMaxItemX-1][0];                  //right top
        iRealYMatrix[1][0]=OutputAtuoTeachTable[iArea].pointY[iMaxItemX-1][0];

        iRealXMatrix[0][1]=OutputAtuoTeachTable[iArea].pointX[0][iMaxItemY-1];                  //left bottom
        iRealYMatrix[0][1]=OutputAtuoTeachTable[iArea].pointY[0][iMaxItemY-1];

        iRealXMatrix[1][1]=OutputAtuoTeachTable[iArea].pointX[iMaxItemX-1][iMaxItemY-1];        //right bottom
        iRealYMatrix[1][1]=OutputAtuoTeachTable[iArea].pointY[iMaxItemX-1][iMaxItemY-1];
    }

    double dOffsetX, dOffsetY, dDisX, dDisY;
    double dCalLength, dRealLength, ResolutionX, ResolutionY, dRadianX, dRadianY;

    // 0:L 1:R 0:T 1:B
    dCalLength =(iXMatrixCal[1][0]-iXMatrixCal[0][0])*(iXMatrixCal[1][0]-iXMatrixCal[0][0])+(iYMatrixCal[1][0]-iYMatrixCal[0][0])*(iYMatrixCal[1][0]-iYMatrixCal[0][0]);
    dRealLength=(iRealXMatrix[1][0]-iRealXMatrix[0][0])*(iRealXMatrix[1][0]-iRealXMatrix[0][0])+(iRealYMatrix[1][0]-iRealYMatrix[0][0])*(iRealYMatrix[1][0]-iRealYMatrix[0][0]);
    if(dCalLength==0 || dRealLength==0)
        return;

    ResolutionX =ChangeToFloatNonPcnt((double)(sqrt(fabs(dRealLength))), (double)(sqrt(fabs(dCalLength))));
    dRadianX    =atan(((double)iRealYMatrix[1][0]-(double)iRealYMatrix[0][0])/((double)iRealXMatrix[1][0]-(double)iRealXMatrix[0][0]) );
    dCalLength  =(iXMatrixCal[0][1]-iXMatrixCal[0][0])*(iXMatrixCal[0][1]-iXMatrixCal[0][0])+(iYMatrixCal[0][1]-iYMatrixCal[0][0])*(iYMatrixCal[0][1]-iYMatrixCal[0][0]);
    dRealLength =(iRealXMatrix[0][1]-iRealXMatrix[0][0])*(iRealXMatrix[0][1]-iRealXMatrix[0][0])+(iRealYMatrix[0][1]-iRealYMatrix[0][0])*(iRealYMatrix[0][1]-iRealYMatrix[0][0]);

    if(dCalLength==0 || dRealLength==0)
        return;

    ResolutionY =ChangeToFloatNonPcnt((double)(sqrt(fabs(dRealLength))), (double)(sqrt(fabs(dCalLength))));
    dRadianY    =atan(-((double)iRealXMatrix[0][0]-(double)iRealXMatrix[0][1])/((double)iRealYMatrix[0][0]-(double)iRealYMatrix[0][1]));
    dOffsetX    =0;
    dOffsetY    =0;
    double px   =iXPos-iXMatrixCal[0][0]+dOffsetX;
    double py   =iYPos-iYMatrixCal[0][0]+dOffsetY;

    RotationCoordinates(px, py, dDisX, dDisY, dRadianX, dRadianY, false);
    dDisX       =dDisX*ResolutionX;
    dDisY       =dDisY*ResolutionY;
    double dXpostion=iXPos;                                                     //KenHsieh 20210915 : CCD AUTO ALIGNMENT offset新增
    double dYpostion=iYPos;                                                     //KenHsieh 20210915 : CCD AUTO ALIGNMENT offset新增
    iXPos       =iRealXMatrix[0][0]+Round(dDisX);
    iYPos       =iRealYMatrix[0][0]+Round(dDisY);

    switch(iArea)                                                               //KenHsieh 20210915 : CCD AUTO ALIGNMENT offset新增
    {
        case AOA_Auto1:
            fMain->labAuto1_X->Caption=dXpostion-iXPos;
            fMain->labAuto1_Y->Caption=dYpostion-iYPos;
            iXPos=iXPos+atoi(fMain->Ed_Auto1Offset_X->Text.c_str());
            iYPos=iYPos+atoi(fMain->Ed_Auto1Offset_Y->Text.c_str());
            break;
        case AOA_Auto2:
            fMain->labAuto2_X->Caption=dXpostion-iXPos;
            fMain->labAuto2_Y->Caption=dYpostion-iYPos;
            iXPos=iXPos+atoi(fMain->Ed_Auto2Offset_X->Text.c_str());
            iYPos=iYPos+atoi(fMain->Ed_Auto2Offset_Y->Text.c_str());
            break;
        case AOA_Auto3:
            fMain->labAuto3_X->Caption=dXpostion-iXPos;
            fMain->labAuto3_Y->Caption=dYpostion-iYPos;
            iXPos=iXPos+atoi(fMain->Ed_Auto3Offset_X->Text.c_str());
            iYPos=iYPos+atoi(fMain->Ed_Auto3Offset_Y->Text.c_str());
            break;
        case AOA_Auto4:
            fMain->labAuto4_X->Caption=dXpostion-iXPos;                         //Steven 20240428 : Add for HT9011 AOA
            fMain->labAuto4_Y->Caption=dYpostion-iYPos;
            iXPos=iXPos+atoi(fMain->Ed_Auto4Offset_X->Text.c_str());
            iYPos=iYPos+atoi(fMain->Ed_Auto4Offset_Y->Text.c_str());
            break;
        case AOA_Auto5:
            fMain->labAuto5_X->Caption=dXpostion-iXPos;
            fMain->labAuto5_Y->Caption=dYpostion-iYPos;
            iXPos=iXPos+atoi(fMain->Ed_Auto5Offset_X->Text.c_str());
            iYPos=iYPos+atoi(fMain->Ed_Auto5Offset_Y->Text.c_str());
            break;
        case AOA_Auto6:
            fMain->labAuto6_X->Caption=dXpostion-iXPos;
            fMain->labAuto6_Y->Caption=dYpostion-iYPos;
            iXPos=iXPos+atoi(fMain->Ed_Auto6Offset_X->Text.c_str());
            iYPos=iYPos+atoi(fMain->Ed_Auto6Offset_Y->Text.c_str());
            break;
        case AOA_Fix1:
            fMain->labFix1_X->Caption=dXpostion-iXPos;
            fMain->labFix1_Y->Caption=dYpostion-iYPos;
            iXPos=iXPos+atoi(fMain->Ed_Fix1Offset_X->Text.c_str());
            iYPos=iYPos+atoi(fMain->Ed_Fix1Offset_Y->Text.c_str());
            break;
        case AOA_Fix2:
            fMain->labFix2_X->Caption=dXpostion-iXPos;
            fMain->labFix2_Y->Caption=dYpostion-iYPos;
            iXPos=iXPos+atoi(fMain->Ed_Fix2Offset_X->Text.c_str());
            iYPos=iYPos+atoi(fMain->Ed_Fix2Offset_Y->Text.c_str());
            break;
        case AOA_Fix3:
            fMain->labFix3_X->Caption=dXpostion-iXPos;
            fMain->labFix3_Y->Caption=dYpostion-iYPos;
            iXPos=iXPos+atoi(fMain->Ed_Fix3Offset_X->Text.c_str());
            iYPos=iYPos+atoi(fMain->Ed_Fix3Offset_Y->Text.c_str());
            break;
        case AOA_Fix4:                                                          //Steven 20240428 : Add for HT9011 AOA
            fMain->labFix4_X->Caption=dXpostion-iXPos;
            fMain->labFix4_Y->Caption=dYpostion-iYPos;
            iXPos=iXPos+atoi(fMain->Ed_Fix4Offset_X->Text.c_str());
            iYPos=iYPos+atoi(fMain->Ed_Fix4Offset_Y->Text.c_str());
            break;
        case AOA_Fix5:
            fMain->labFix5_X->Caption=dXpostion-iXPos;
            fMain->labFix5_Y->Caption=dYpostion-iYPos;
            iXPos=iXPos+atoi(fMain->Ed_Fix5Offset_X->Text.c_str());
            iYPos=iYPos+atoi(fMain->Ed_Fix5Offset_Y->Text.c_str());
            break;
        case AOA_Fix6:
            fMain->labFix6_X->Caption=dXpostion-iXPos;
            fMain->labFix6_Y->Caption=dYpostion-iYPos;
            iXPos=iXPos+atoi(fMain->Ed_Fix6Offset_X->Text.c_str());
            iYPos=iYPos+atoi(fMain->Ed_Fix6Offset_Y->Text.c_str());
            break;
        case AOA_OutSht1:
            fMain->labOutShuttle1_X->Caption=dXpostion-iXPos;
            fMain->labOutShuttle1_Y->Caption=dYpostion-iYPos;
            iXPos=iXPos+atoi(fMain->Ed_OutSH1Offset_X->Text.c_str());
            iYPos=iYPos+atoi(fMain->Ed_OutSH1Offset_Y->Text.c_str());
            break;
        case AOA_OutSht2:
            fMain->labOutShuttle2_X->Caption=dXpostion-iXPos;
            fMain->labOutShuttle2_Y->Caption=dYpostion-iYPos;
            iXPos=iXPos+atoi(fMain->Ed_OutSH2Offset_X->Text.c_str());
            iYPos=iYPos+atoi(fMain->Ed_OutSH2Offset_Y->Text.c_str());
            break;
    }
    return;
}
//------------------------------------------------------------------------------
HTimer HOutArmCCDAlignDelay;
//------------------------------------------------------------------------------
//KenHsieh 20211110 : AutoAlignment add Auto Z
//==>
int iOutArmPickDetectHeight=1;
//------------------------------------------------------------------------------
void InitOutArmPickDetectHeight()
{
    iOutArmPickDetectHeight=1;
}
//------------------------------------------------------------------------------
HTimer AOAOutArmZCheck;                                                         //Ifor 20220816 add: AOA Z Sensor 需持續亮1秒避免雜訊導致位置異常
HTimer HOutArmDetectZ;
//------------------------------------------------------------------------------
int AOAOutArmSave_SafePosZ[MAX_ARM_Row][MAX_ARM_Col];
int AOAOutArmSave_SafePosX=0;
int AOAOutArmSave_SafePosY=0;
bool AOAOutArmSave_bZFlag[MAX_ARM_Row][MAX_ARM_Col];

int AOA_OutArmMoveToSafeTask=1;
bool bAutoAlignmentOutArmSavePosition;
//------------------------------------------------------------------------------
void ResetAOA_OutArmMoveToSafePos()
{
    ZeroMemory(AOAOutArmSave_SafePosZ, sizeof(AOAOutArmSave_SafePosZ));
    ZeroMemory(AOAOutArmSave_bZFlag, sizeof(AOAOutArmSave_bZFlag));
    AOAOutArmSave_SafePosX=0;
    AOAOutArmSave_SafePosY=0;

    bAutoAlignmentOutArmSavePosition=false;
}
//------------------------------------------------------------------------------
void InitAOA_OutArmMoveToSafePosTask()
{
    AOA_OutArmMoveToSafeTask=1;
}
//------------------------------------------------------------------------------
bool AOA_OutArmMoveToSafePos(bool MovePos)                                      //2012.05.21 Q_Q TrayArm Drop Tray will Move Two Times
{
    int &Task=AOA_OutArmMoveToSafeTask;
    bool bFlag=false;
    int iMot, iFlag;

    if(MovePos==true)
    {
        switch(Task)
        {
            case 1:
                if(InOutArmPickerUseMotor==eptUseMotCyn)
                {
                    if(AOAOutArmSave_SafePosZ[0][0]!=0)
                        bFlag=true;
                }
                else
                {
                    for(int i=0; i<OutArmSuck.iMotRow; i++)
                    {
                        for(int j=0; j<OutArmSuck.iMotCol; j++)
                        {
                            if(AOAOutArmSave_SafePosZ[i][j]!=0)
                                bFlag=true;
                        }
                    }
                }

                if(AOAOutArmSave_SafePosX!=0 || AOAOutArmSave_SafePosY!=0)
                {
                    bFlag=true;
                }

                if(bFlag==false)
                {
                    if(InOutArmPickerUseMotor==eptUseMotCyn)
                    {
                        AOAOutArmSave_SafePosZ[0][0]=MOT[MOutArmZA].ReadPos();
                    }
                    else
                    {
                        for(int i=0; i<OutArmSuck.iMotRow; i++)
                        {
                            for(int j=0; j<OutArmSuck.iMotCol; j++)
                            {
                                iMot=OutArmSuck.Suck[i][j].iMotNo;
                                AOAOutArmSave_SafePosZ[i][j]=MOT[iMot].ReadPos();
                            }
                        }
                    }

                    AOAOutArmSave_SafePosX=MOT[MOutArmX].ReadPos();
                    AOAOutArmSave_SafePosY=MOT[MOutArmY].ReadPos();
                }
                break;
            case 2:
                if(MoveOutArmToAutoSafe())
                {
//                    fiosetview->btnOutCynAllUp->Click();                        //Steven 20230801 : for HT-1032AT
                    bAutoAlignmentOutArmSavePosition=true;
                    Task=1;
                    return true;
                }
                break;
        }
    }
    else
    {
        if(bAutoAlignmentOutArmSavePosition)
        {
            switch(Task)
            {
                case 1:
                    InitPCIL112_OutArmXYMoveTask();
                    Task=10;
                case 10:
                    iFlag=PCIL112_OutArmXYMove(AOAOutArmSave_SafePosX, AOAOutArmSave_SafePosY);

                    if(iFlag>0)
                    {
                        iOutArmZMoveTask=-1;
                        for(int i=0; i<OutArmSuck.iPickRow; i++)                //QQ 這邊是要馬達往下走嗎?
                        {
                            for(int j=0; j<OutArmSuck.iPickCol; j++)
                            {
                                AOAOutArmSave_bZFlag[i][j]=true;
                            }
                        }
                        Task=15;
                    }
                    break;
                case 15:
                    bFlag=OutArmZMoveDown(AOAOutArmSave_bZFlag, AOAOutArmSave_SafePosZ);

                    if(bFlag==true)
                    {
                        Task=20;
                    }
                    break;
                case 20:
                    bAutoAlignmentOutArmSavePosition=false;
                    Task=999;
                    return true;
            }
        }
    }
    return false;
}
//------------------------------------------------------------------------------
HTimer HOutArmCCDAlignmentMotorDelay;
//------------------------------------------------------------------------------
bool DoMoveOutArmCCDAlignmentTeachUnit(int iPosX, int iPosY)
{
    bool flag[2]={true, true};
    if(CheckOutArmZ(false))
    {
        bNeedArmZHome=true;
        InitDoArmZHome();
        return false;
    }

    if(iPosX>=MOT[MOutArmX].Motor->PSoftLimitP || iPosX<=MOT[MOutArmX].Motor->PSoftLimitN)
    {
        ShowMyMessage("CCD Alignment error. Alignment In Arm motor will out of limit");
        return false;
    }

    if(iPosY>=MOT[MOutArmY].Motor->PSoftLimitP || iPosY<=MOT[MOutArmY].Motor->PSoftLimitN)
    {
        ShowMyMessage("CCD Alignment error. Alignment In Arm motor will out of limit");
        return false;
    }

    flag[0]=MOT[MOutArmX].MotorMove(iPosX);
    flag[1]=MOT[MOutArmY].MotorMove(iPosY);

    if(flag[0] && flag[1])
    {
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
int iOutArmCCDAlignmentTeachUnitProcess=-1;
//------------------------------------------------------------------------------
void InitOutArmCCDAlignmentTeachUnitProcess()
{
    iOutArmCCDAlignmentTeachUnitProcess=1;
}
//------------------------------------------------------------------------------
int DoOutArmCCDAlignmentTeachUnitProces(double &ResolutionX, double &ResolutionY, double &dRadianX, double &dRadianY, int iTeach)    //0: Correction mm/pixel 1: Teach Pattern 2: Teach Pattern and Correction mm/pixel
{
    int &Task=iOutArmCCDAlignmentTeachUnitProcess;
    int iPosX=Prod.iOutArmXCCDAlignmentPos;
    int iPosY=Prod.iOutArmYCCDAlignmentPos;
    static double dTeachPos[4][2];                                              //0: Left 1: Right 2: Up 3: Down  //0: X 1: Y
    int dLength;
    static double dOffsetX, dOffsetY;
    double dOffsetX1, dOffsetY1;
    int iDis=200;
    int iRet=0;

    switch(Task)
    {
        case 1:
            if(MoveOutArmToAutoSafe())
            {
                if(Cylinder[C_OutAreaAlignment].Pop())                          //KenHsieh 20220117 : 新增Golden初始狀態
                {
                    if(iTeach==1 || iTeach==2)
                        Task=5;
                    else
                        Task=100;
                }
            }
            break;
        case 5:                                                                 //Move To Mark Center Postion
            if(DoMoveOutArmCCDAlignmentTeachUnit(iPosX, iPosY))
            {
                HOutArmCCDAlignmentMotorDelay.SetMSAndOn(iCCDAlignmentMotorDelay);
                Task=10;
            }
            break;
        case 10:
            if(HOutArmCCDAlignmentMotorDelay.Off()==false)
                break;
            //TEACH_GOLDEN
            fAutoAlignment->OutArmSendCommand(TEACH_GOLDEN);
            Task=25;
            break;
        case 25:
            iRet=fAutoAlignment->OutArmGetResult(TEACH_GOLDEN, dOffsetX, dOffsetY);
            if(iRet==0)                                                         //KenHsieh 20220221 : 修改cmd teach類指令取值流程
            {
                ShowMyMessage("Please Check OutArm Golden Patten in AutoAlignment PC!");
                return 0;
            }
            else if(iRet==1)
            {
                if(iTeach==1)
                    return 1;
                else
                    Task=100;
            }
            else if(iRet==2)
            {
                ShowMyMessage("Please Check OutArm Alignment Teach Golden Mark again!");
                return 2;
            }
            break;
        case 100:                                                               //Move To Mark Left
            if(DoMoveOutArmCCDAlignmentTeachUnit(iPosX-iDis, iPosY))
            {
                HOutArmCCDAlignmentMotorDelay.SetMSAndOn(iCCDAlignmentMotorDelay);
                Task=200;
            }
            break;
        case 200:
            if(HOutArmCCDAlignmentMotorDelay.Off()==false)
                break;
            fAutoAlignment->OutArmSendCommand(UNIT_LEFT);
            Task=300;
            break;
        case 300:
            iRet=fAutoAlignment->OutArmGetResult(UNIT_LEFT, dOffsetX, dOffsetY);
            if(iRet==1)
            {
                dTeachPos[0][0]=dOffsetX;
                dTeachPos[0][1]=dOffsetY;
                Task=400;
            }
            else if(iRet==2)
            {
                ShowMyMessage("Please Check OutArm Alignment Left Golden Mark!");
                return 2;
            }
            break;
        case 400:                                                               //Move To Mark Right
            if(DoMoveOutArmCCDAlignmentTeachUnit(iPosX+iDis, iPosY))
            {
                HOutArmCCDAlignmentMotorDelay.SetMSAndOn(iCCDAlignmentMotorDelay);
                Task=500;
            }
            break;
        case 500:
            if(HOutArmCCDAlignmentMotorDelay.Off()==false)
                break;
            fAutoAlignment->OutArmSendCommand(UNIT_RIGHT);
            Task=600;
            break;
        case 600:
            iRet=fAutoAlignment->OutArmGetResult(UNIT_RIGHT, dOffsetX, dOffsetY);
            if(iRet==1)
            {
                dTeachPos[1][0]=dOffsetX;
                dTeachPos[1][1]=dOffsetY;
                Task=700;
            }
            else if(iRet==2)
            {
                ShowMyMessage("Please Check OutArm Alignment Right Golden Mark!");
                return 2;
            }
            break;
        case 700:                                                               //Move To Mark Up
            if(DoMoveOutArmCCDAlignmentTeachUnit(iPosX, iPosY+iDis))
            {
                HOutArmCCDAlignmentMotorDelay.SetMSAndOn(iCCDAlignmentMotorDelay);
                Task=800;
            }
            break;
        case 800:
            if(HOutArmCCDAlignmentMotorDelay.Off()==false)
                break;
            fAutoAlignment->OutArmSendCommand(UNIT_UP);
            Task=900;
            break;
        case 900:
            iRet=fAutoAlignment->OutArmGetResult(UNIT_UP, dOffsetX, dOffsetY);
            if(iRet==1)
            {
                dTeachPos[2][0]=dOffsetX;
                dTeachPos[2][1]=dOffsetY;
                Task=1000;
            }
            else if(iRet==2)
            {
                ShowMyMessage("Please Check OutArm Alignment Up Golden Mark!");
                return 2;
            }
            break;
        case 1000:                                                              //Move To Mark Down
            if(DoMoveOutArmCCDAlignmentTeachUnit(iPosX, iPosY-iDis))
            {
                HOutArmCCDAlignmentMotorDelay.SetMSAndOn(iCCDAlignmentMotorDelay);
                Task=1100;
            }
            break;
        case 1100:
            if(HOutArmCCDAlignmentMotorDelay.Off()==false)
                break;
            fAutoAlignment->OutArmSendCommand(UNIT_DOWN);
            Task=1200;
            break;
        case 1200:
            iRet=fAutoAlignment->OutArmGetResult(UNIT_DOWN, dOffsetX, dOffsetY);
            if(iRet==1)
            {
                dTeachPos[3][0]=dOffsetX;
                dTeachPos[3][1]=dOffsetY;
                Task=1300;
            }
            else if(iRet==2)
            {
                ShowMyMessage("Please Check OutArm Alignment Down Golden Mark!");
                return 2;
            }
            break;
        case 1300:                                                              //Move To Mark Center
            if(DoMoveOutArmCCDAlignmentTeachUnit(iPosX, iPosY))
            {
                HOutArmCCDAlignmentMotorDelay.SetMSAndOn(iCCDAlignmentMotorDelay);
                Task=1400;
            }
            break;
        case 1400:
            if(HOutArmCCDAlignmentMotorDelay.Off()==false)
                break;
            fAutoAlignment->OutArmSendCommand(UNIT_CENTER);
            Task=1500;
            break;
        case 1500:
            iRet=fAutoAlignment->OutArmGetResult(UNIT_CENTER, dOffsetX, dOffsetY);
            if(iRet==1)
            {
                Task=1600;
            }
            else if(iRet==2)
            {
                ShowMyMessage("Please Check OutArm Alignment Center Golden Mark!");
                return 2;
            }
            break;
        case 1600:                                                              //0: Left 1: Right 2: Up 3: Down  //0: X 1: Y
            if(dTeachPos[1][0]>=dTeachPos[0][0])
            {
                ShowMyMessage("Please Check CCD Image Setting!");
                return 2;
            }

            if(dTeachPos[3][1]<=dTeachPos[2][1])
            {
                ShowMyMessage("Please Check CCD Image Setting!");
                return 2;
            }

            dLength     =(dTeachPos[1][1]-dTeachPos[0][1])*(dTeachPos[1][1]-dTeachPos[0][1])+(dTeachPos[1][0]-dTeachPos[0][0])*(dTeachPos[1][0]-dTeachPos[0][0]);
            ResolutionX =ChangeToFloatNonPcnt((double)((2*iDis)), (double)(sqrt(dLength)));
            ResolutionX =Round(ResolutionX, 3);
            dRadianX    =atan(-(dTeachPos[0][1]-dTeachPos[1][1])/(dTeachPos[0][0]-dTeachPos[1][0]));
            dRadianX    =Round(dRadianX, 4);
            dLength     =(dTeachPos[2][1]-dTeachPos[3][1])*(dTeachPos[2][1]-dTeachPos[3][1])+(dTeachPos[2][0]-dTeachPos[3][0])*(dTeachPos[2][0]-dTeachPos[3][0]);
            ResolutionY =ChangeToFloatNonPcnt((double)((2*iDis)), (double)(sqrt(dLength)));
            ResolutionY =Round(ResolutionY, 3);
            dRadianY    =atan(-(dTeachPos[3][0]-dTeachPos[2][0])/-(dTeachPos[3][1]-dTeachPos[2][1]));
            dRadianY    =Round(dRadianY, 4);
            if(bSpin)
            {
                RotationCoordinates(Round(dOffsetX), Round(dOffsetY), dOffsetX1, dOffsetY1, dRadianX, dRadianY);    //ChungHung 20210113 add for Alignment CCD

                dOffsetX1=dOffsetX1*ResolutionX;
                dOffsetY1=dOffsetY1*ResolutionY;
            }
            else
            {
                dOffsetX1=dOffsetX*ResolutionX;
                dOffsetY1=dOffsetY*ResolutionY;
            }

            if(DoMoveOutArmCCDAlignmentTeachUnit(iPosX+Round(dOffsetX1), iPosY+Round(dOffsetY1)))
            {
                return 1;
            }
            break;
        case 1700:
            if(HOutArmCCDAlignmentMotorDelay.Off()==false)
                break;
            fAutoAlignment->OutArmSendCommand(UNIT_CENTER);
            Task=1800;
            break;
        case 1800:
            iRet=fAutoAlignment->OutArmGetResult(UNIT_CENTER, dOffsetX, dOffsetY);
            if(iRet==1)
            {
                dLength     =(dTeachPos[1][1]-dTeachPos[0][1])*(dTeachPos[1][1]-dTeachPos[0][1])+(dTeachPos[1][0]-dTeachPos[0][0])*(dTeachPos[1][0]-dTeachPos[0][0]);
                ResolutionX =ChangeToFloatNonPcnt((double)((2*iDis)), (double)(Round(sqrt(dLength), 2)));
                ResolutionX =Round(ResolutionX, 3);
                dRadianX    =atan(-(dTeachPos[0][1]-dTeachPos[1][1])/(dTeachPos[0][0]-dTeachPos[1][0]));
                dRadianX    =Round(dRadianX, 4);
                dLength     =(dTeachPos[2][1]-dTeachPos[3][1])*(dTeachPos[2][1]-dTeachPos[3][1])+(dTeachPos[2][0]-dTeachPos[3][0])*(dTeachPos[2][0]-dTeachPos[3][0]);
                ResolutionY =ChangeToFloatNonPcnt((double)((2*iDis)), (double)(Round(sqrt(dLength), 2)));
                ResolutionY =Round(ResolutionY, 3);
                dRadianY    =atan(-(dTeachPos[3][0]-dTeachPos[2][0])/-(dTeachPos[3][1]-dTeachPos[2][1]));
                dRadianY    =Round(dRadianY, 4);
                return 1;
            }
            else if(iRet==2)
            {
                ShowMyMessage("Please Check OutArm Alignment Center Golden Mark!!");
                return 2;
            }
            break;
    }

    ResolutionX=0;
    ResolutionX=0;
    dRadianY=0;
    dRadianY=0;
    return 0;
}
//------------------------------------------------------------------------------
bool Find_OutArmAlgnment_Single(TTrayMotor *myMotor, int &iRow, int &iCol)
{
    for(int iR=0; iR<myMotor->Tray.YItem; iR++)
    {
        for(int iC=0; iC<myMotor->Tray.XItem; iC++)
        {
            if(myMotor->Tray.Data[iC][iR]==WAIT_ALIGN_IC)
            {
                iRow=iR;
                iCol=iC;
                return true;
            }
        }
    }
    return false;
}
//------------------------------------------------------------------------------
bool DoMoveAlignmentToUnloaderStage(int iWhichUnloader, int iRow, int iCol, int &iXPos, int &iYPos, bool realMove, bool bCheckXOnly=false, bool bCheckYOnly=false)     //KenHsieh 20210902 : Modify add CCD AUTO ALIGNMENT
{
    bool flag1, flag2;

    if(CheckOutArmZ(false))
    {
        bNeedArmZHome=true;
        InitDoArmZHome();
        return false;
    }

    //KenHsieh 20210829 : Modify add CCD AUTO ALIGNMENT (XY變距)
    //==>
    if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable)
    {
        iXPos=Prod.XStart[iWhichUnloader][iInArmYBase][iInArmXBase]+(iCol)*AutoForm[iWhichUnloader]->XPitch+Prod.iOutArmCCD_Picker_OffsetX[iInArmYBase][iInArmXBase];  //JerryYang 20250204 : fix AOA
        iYPos=Prod.YStart[iWhichUnloader][iInArmYBase][iInArmXBase]-(iRow)*AutoForm[iWhichUnloader]->YPitch+Prod.iOutArmCCD_Picker_OffsetY[iInArmYBase][iInArmXBase];
    }
    else
    {
        iXPos=Prod.XStart[iWhichUnloader][iInArmYBase][iInArmXBase]+(iCol)*AutoForm[iWhichUnloader]->XPitch+Prod.iOutArmCCD_Picker_OffsetX[iInArmYBase][iInArmXBase];
        iYPos=Prod.YStart[iWhichUnloader][iInArmYBase][iInArmXBase]-(iRow)*AutoForm[iWhichUnloader]->YPitch+Prod.iOutArmCCD_Picker_OffsetY[iInArmYBase][iInArmXBase];
    }
    //<==
    //KenHsieh 20210829 : Modify add CCD AUTO ALIGNMENT (XY變距)

    if(realMove==false)
    {
        if(bCheckYOnly==false)
        {
            if(iXPos>=MOT[MOutArmX].Motor->PSoftLimitP || iXPos<=MOT[MOutArmX].Motor->PSoftLimitN)
                return false;
        }

        if(bCheckXOnly==false)
        {
            if(iYPos>=MOT[MOutArmY].Motor->PSoftLimitP || iYPos<=MOT[MOutArmY].Motor->PSoftLimitN)
                return false;
        }
        return true;
    }

    flag1=MOT[MOutArmX].MotorMove(iXPos);
    flag2=MOT[MOutArmY].MotorMove(iYPos);

    if(flag1 && flag2)
    {
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
bool DoMoveAlignmentToOutShuttleStage(int iShuttle, int iRow, int iCol, int &iXPos, int &iYPos, bool realMove, bool bCheckXOnly=false, bool bCheckYOnly=false)     //KenHsieh 20211002 : Modify Shuttle Scan
{
    bool flag[2]={true, true};
    int iShuttleXItem, iShuttleYItem, iShuttleXPitch, iShuttleYPitch;
    int iOffsetX, iOffsetY;

    if(CheckOutArmZ(false))
    {
        bNeedArmZHome=true;
        InitDoArmZHome();
        return false;
    }

//    GetShuttleSize(iShuttleXItem, iShuttleYItem, iShuttleXPitch, iShuttleYPitch);      //ChungHung 20210113 add for Alignment CCD

    iShuttleXItem=FRCarryKit.iShtCol;                                           //JerryYang 20250204 : fix AOA
    iShuttleYItem=FRCarryKit.iShtRow;

    iShuttleXPitch=TestIF.dSiteXPitch;
    iShuttleYPitch=TestIF.dSiteYPitch;

    if(iShuttleXItem==1)
    {
        iOffsetX=0;
    }
    else
    {
        iOffsetX=(((iShuttleXItem-1)/2.0)*iShuttleXPitch)-(iCol*iShuttleXPitch);
    }

    if(iShuttleYItem==1)
        iOffsetY=0;
    else if(iShuttleYItem==2)
        iOffsetY=(iShuttleYPitch/2.0)-(iRow*iShuttleYPitch);

    if(iShuttle==0)
    {
        //KenHsieh 20210829 : Modify add CCD AUTO ALIGNMENT (XY變距)
        //==>
        if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable)
        {
            iXPos=Prod.XOutArm_Shuttle1_Pick[iOutArmYBase][iOutArmXBase]+iOutArmShtXCenterPos+iOffsetX+Prod.iOutArmCCD_Picker_OffsetX[iInArmYBase][iInArmXBase];   //左上第一點  //JerryYang 20250204 : fix AOA
            iYPos=Prod.YOutArm_Shuttle1_Pick[iOutArmYBase][iOutArmXBase]+iOutArmShtYCenterPos+iOffsetY+Prod.iOutArmCCD_Picker_OffsetY[iInArmYBase][iInArmXBase];
        }
        else
        {
            iXPos=Prod.XOutArm_Shuttle1_Pick[iOutArmYBase][iOutArmXBase]+iOutArmShtXCenterPos+iOffsetX+Prod.iOutArmCCD_Picker_OffsetX[iInArmYBase][iInArmXBase];   //左上第一點
            iYPos=Prod.YOutArm_Shuttle1_Pick[iOutArmYBase][iOutArmXBase]+iOutArmShtYCenterPos+iOffsetY+Prod.iOutArmCCD_Picker_OffsetY[iInArmYBase][iInArmXBase];
        }
    }
    else
    {
        if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable)
        {
            iXPos=Prod.XOutArm_Shuttle2_Pick[iOutArmYBase][iOutArmXBase]+iOutArmShtXCenterPos+iOffsetX+Prod.iOutArmCCD_Picker_OffsetX[iInArmYBase][iInArmXBase];   //左上第一點
            iYPos=Prod.YOutArm_Shuttle2_Pick[iOutArmYBase][iOutArmXBase]+iOutArmShtYCenterPos+iOffsetY+Prod.iOutArmCCD_Picker_OffsetY[iInArmYBase][iInArmXBase];
        }
        else
        {
            iXPos=Prod.XOutArm_Shuttle2_Pick[iOutArmYBase][iOutArmXBase]+iOutArmShtXCenterPos+iOffsetX+Prod.iOutArmCCD_Picker_OffsetX[iInArmYBase][iInArmXBase];   //左上第一點
            iYPos=Prod.YOutArm_Shuttle2_Pick[iOutArmYBase][iOutArmXBase]+iOutArmShtYCenterPos+iOffsetY+Prod.iOutArmCCD_Picker_OffsetY[iInArmYBase][iInArmXBase];
        }
        //<==
        //KenHsieh 20210829 : Modify add CCD AUTO ALIGNMENT (XY變距)
    }

    if(realMove==false)
    {
        if(bCheckYOnly==false)                                                  //Kenhsieh 20211002 : Modify Shuttle Scan
        {
            if(iXPos>=MOT[MOutArmX].Motor->PSoftLimitP || iXPos<=MOT[MOutArmX].Motor->PSoftLimitN)
                return false;
        }

        if(bCheckXOnly==false)                                                  //Kenhsieh 20211002 : Modify Shuttle Scan
        {
            if(iYPos>=MOT[MOutArmY].Motor->PSoftLimitP || iYPos<=MOT[MOutArmY].Motor->PSoftLimitN)
                return false;
        }
        return true;
    }

    flag[0]=MOT[MOutArmX].MotorMove(iXPos);
    flag[1]=MOT[MOutArmY].MotorMove(iYPos);

    if(flag[0] && flag[1])
    {
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
bool SearchOutArmAlignmentMaxArea(int iArea, TTrayMotor *myTray)                //KenHsieh 20210902 : Modify add CCD AUTO ALIGNMENT
{
    bool bCheckXOnly=false;
    bool bCheckYOnly=false;
    bool CanMove    =false;
    int  iColItem   =myTray->Tray.XItem;
    int  iRowItem   =myTray->Tray.YItem;
    int  iXPos      =0;
    int  iYPos      =0;

    for(int iC=0; iC<iColItem; iC++)
    {
        for(int iR=0; iR<iRowItem; iR++)
        {
            myTray->SetTraySingleData(iC, iR, WAIT_ALIGN_IC);
        }
    }

    for(int iR=0; iR<iRowItem; iR++)
    {
        bCheckXOnly=false;
        bCheckYOnly=true;

        if(iArea==AOA_OutSht1)
        {
            CanMove=DoMoveAlignmentToOutShuttleStage(0, iR, iColItem, iXPos, iYPos, false, bCheckXOnly, bCheckYOnly);    //KenHsieh 20211002 : Modify Shuttle Scan
        }
        else if(iArea==AOA_OutSht2)
        {
            CanMove=DoMoveAlignmentToOutShuttleStage(1, iR, iColItem, iXPos, iYPos, false, bCheckXOnly, bCheckYOnly);    //KenHsieh 20211002 : Modify Shuttle Scan
        }
        else
        {
            CanMove=DoMoveAlignmentToUnloaderStage(iArea, iR, iColItem, iXPos, iYPos, false, bCheckXOnly, bCheckYOnly);
        }

        if(CanMove==false)
        {
            for(int iC=0; iC<iColItem; iC++)
            {
                myTray->SetTraySingleData(iC, iR, NULL_IC);
            }
        }
    }

    for(int iC=0; iC<iColItem; iC++)
    {
        bCheckXOnly=true;
        bCheckYOnly=false;

        if(iArea==AOA_OutSht1)
        {
            CanMove=DoMoveAlignmentToOutShuttleStage(0, iRowItem, iC, iXPos, iYPos, false, bCheckXOnly, bCheckYOnly);    //KenHsieh 20211002 : Modify Shuttle Scan
        }
        else if(iArea==AOA_OutSht2)
        {
            CanMove=DoMoveAlignmentToOutShuttleStage(1, iRowItem, iC, iXPos, iYPos, false, bCheckXOnly, bCheckYOnly);    //KenHsieh 20211002 : Modify Shuttle Scan
        }
        else
        {
            CanMove=DoMoveAlignmentToUnloaderStage(iArea, iRowItem, iC, iXPos, iYPos, false, bCheckXOnly, bCheckYOnly);
        }

        if(CanMove==false)
        {
            for(int iR=0; iR<iRowItem; iR++)
            {
                myTray->SetTraySingleData(iC, iR, NULL_IC);
            }
        }
    }

    return true;
}
//------------------------------------------------------------------------------
int SearchOutArmAlignmentUpDown(int iArea, int &iRow, int &iCol, int &iXPos, int &iYPos)  //0: Not Find Device 1:Find Device 2:Move Fail
{
    int _iRow=-1, _iCol=-1;
    TTrayMotor *myMotor=&MOT[MMOutArmAOATray];
    Find_OutArmAlgnment_Single(myMotor, _iRow, _iCol);

    if(_iRow==-1 && _iCol==-1)
        return 0;

    iRow=_iRow;
    iCol=_iCol;

    #ifndef SOFT_SIMULTE
    bool bCheckXOnly=false;                                                     //KenHsieh 20210902 : Modify add CCD AUTO ALIGNMENT
    bool bCheckYOnly=false;                                                     //KenHsieh 20210902 : Modify add CCD AUTO ALIGNMENT
    bool CanMove;

    if(iArea==AOA_OutSht1)
    {
        CanMove=DoMoveAlignmentToOutShuttleStage(0, iRow, iCol, iXPos, iYPos, false, bCheckXOnly, bCheckYOnly);    //KenHsieh 20211002 : Modify Shuttle Scan
    }
    else if(iArea==AOA_OutSht2)
    {
        CanMove=DoMoveAlignmentToOutShuttleStage(1, iRow, iCol, iXPos, iYPos, false, bCheckXOnly, bCheckYOnly);    //KenHsieh 20211002 : Modify Shuttle Scan
    }
    else
    {
        CanMove=DoMoveAlignmentToUnloaderStage(iArea, iRow, iCol, iXPos, iYPos, false, bCheckXOnly, bCheckYOnly);    //KenHsieh 20210902 : Modify add CCD AUTO ALIGNMENT
    }

    if(CanMove==false)
    {
        if(iArea==AOA_Auto1)
            ShowMyMessage("Tray parameter error.Auto1 Alignment Out Arm motor will out of limit");
        if(iArea==AOA_Auto2)
            ShowMyMessage("Tray parameter error.Auto2 Alignment Out Arm motor will out of limit");
        if(iArea==AOA_Auto3)
            ShowMyMessage("Tray parameter error.Auto3 Alignment Out Arm motor will out of limit");
        if(iArea==AOA_Auto4)                                                    //Steven 20240428 : Add for HT9011 AOA
            ShowMyMessage("Tray parameter error.Auto4 Alignment Out Arm motor will out of limit");
        if(iArea==AOA_Auto5)
            ShowMyMessage("Tray parameter error.Auto5 Alignment Out Arm motor will out of limit");
        if(iArea==AOA_Auto6)
            ShowMyMessage("Tray parameter error.Auto6 Alignment Out Arm motor will out of limit");
        if(iArea==AOA_Fix1)
            ShowMyMessage("Tray parameter error.Fix1 Alignment Out Arm motor will out of limit");
        if(iArea==AOA_Fix2)
            ShowMyMessage("Tray parameter error.Fix2 Alignment Out Arm motor will out of limit");
        if(iArea==AOA_Fix3)
            ShowMyMessage("Tray parameter error.Fix3 Alignment Out Arm motor will out of limit");
        if(iArea==AOA_Fix4)                                                     //Steven 20240428 : Add for HT9011 AOA
            ShowMyMessage("Tray parameter error.Fix4 Alignment Out Arm motor will out of limit");
        if(iArea==AOA_Fix5)
            ShowMyMessage("Tray parameter error.Fix5 Alignment Out Arm motor will out of limit");
        if(iArea==AOA_Fix6)
            ShowMyMessage("Tray parameter error.Fix6 Alignment Out Arm motor will out of limit");
        if(iArea==AOA_OutSht1)
            ShowMyMessage("Tray parameter error.Shuttle1 Alignment Out Arm motor will out of limit");
        if(iArea==AOA_OutSht2)
            ShowMyMessage("Tray parameter error.Shuttle2 Alignment Out Arm motor will out of limit");
        return 2;
    }
    #endif
    return 1;
}
//------------------------------------------------------------------------------
bool MoveOutArmAutoAlignmentStage(int iArea, int &iXPos, int &iYPos)
{
    int iRow=0, iCol=0;
    if(CheckOutArmZ(false))
    {
        bNeedArmZHome=true;
        InitDoArmZHome();
        return false;
    }

    if(SearchOutArmAlignmentUpDown(iArea, iRow, iCol, iXPos, iYPos)!=1)
        return false;

    bool bCheckXOnly=false;                                                     //KenHsieh 20210902 : Modify add CCD AUTO ALIGNMENT
    bool bCheckYOnly=false;                                                     //KenHsieh 20210902 : Modify add CCD AUTO ALIGNMENT

    if(iArea==AOA_OutSht1)
    {
        if(DoMoveAlignmentToOutShuttleStage(0, iRow, iCol, iXPos, iYPos, true, bCheckXOnly, bCheckYOnly))    //KenHsieh 20211002 : Modify Shuttle Scan
        {
            return true;
        }
    }
    else if(iArea==AOA_OutSht2)
    {
        if(DoMoveAlignmentToOutShuttleStage(1, iRow, iCol, iXPos, iYPos, true, bCheckXOnly, bCheckYOnly))    //KenHsieh 20211002 : Modify Shuttle Scan
        {
            return true;
        }
    }
    else
    {
        if(DoMoveAlignmentToUnloaderStage(iArea, iRow, iCol, iXPos, iYPos, true, bCheckXOnly, bCheckYOnly))    //KenHsieh 20210902 : Modify add CCD AUTO ALIGNMENT
        {
            return true;
        }
    }
    return false;
}
//------------------------------------------------------------------------------
int iOutArmXYAutoTeachProcess=-1;
//------------------------------------------------------------------------------
void InitOutArmXYAutoTeachProcess()
{
    iOutArmXYAutoTeachProcess=1;
}
//------------------------------------------------------------------------------
int Find_OutArmAlignment_NULL_IC(int iCol)                                      //Kenhsieh 20211007 : AOA Scan時獲取完整Shuttle Xitem
{
    TTrayMotor *myMotor=&MOT[MMOutArmAOATray];

    for(int iR=0; iR<myMotor->Tray.YItem; iR++)
    {
        for(int iC=0; iC<myMotor->Tray.XItem; iC++)
        {
            if(myMotor->Tray.Data[iC][iR]==NULL_IC)
            {
                iCol=iC;
                return iCol;
            }
        }
    }
    return myMotor->Tray.XItem;
}

//---------------------------------------------------------------------------
extern void SetAOATrayTeachPoint(int &iCol, int &iRow, TTrayMotor *myTray, bool bTraySkipLast);   //ChungHung 20210113 add for Alignment CCD
//------------------------------------------------------------------------------
int DoOutArmXYAlignmentTeachProcess(int iArea, int bTeach)
{
    int &Task=iOutArmXYAutoTeachProcess;
    static int iRow=0, iCol=0;
    static bool bflag[MAX_ARM_Row][MAX_ARM_Col];
    int iOutArmZ[MAX_ARM_Row][MAX_ARM_Col]={{MOutArmZA, MOutArmZC, MOutArmZE, MOutArmZG},
                                            {MOutArmZB, MOutArmZD, MOutArmZF, MOutArmZH}};

    static int iXPos, iYPos;
    static double dOffsetX, dOffsetY;
    static double dOffsetX1, dOffsetY1;

    static int iIndexX, iIndexY;

    AnsiString asAreaFileName;
    AnsiString asAreaFileName_Cal;
    int iRet=0;
//    int iShuttleXItem, iShuttleYItem, iShuttleXPitch, iShuttleYPitch;

    static bool bFailNeedTeach=false;
    static int iOutShuttleNoscanItemX=0;                                        //Kenhsieh 20211007 : AOA Scan時獲取完整Shuttle Xitem
    int iOutShuttleScanX=0;                                                     //Kenhsieh 20211007 : AOA Scan時獲取完整Shuttle Xitem
    static int iRetryConnect=0;                                                 //Ifor 20211221 add: AOA 斷線重新連線

    switch(Task)
    {
        case 1:     //Init Area Tray
            if(iArea==AOA_OutSht1)
            {
//                GetShuttleSize(iShuttleXItem, iShuttleYItem, iShuttleXPitch, iShuttleYPitch);      //ChungHung 20210113 add for Alignment CCD
                MOT[MMOutArmAOATray].Tray.XItem=FRCarryKit.iShtCol;             //JerryYang 20250204 : fix AOA
                MOT[MMOutArmAOATray].Tray.YItem=FRCarryKit.iShtRow;
                MOT[MMOutArmAOATray].InitNewTray(NULL_IC, false, __FUNC__);
                SearchOutArmAlignmentMaxArea(iArea, &MOT[MMOutArmAOATray]);     //KenHsieh 20210902 : Modify add CCD AUTO ALIGNMENT
                SetAOATrayTeachPoint(FRCarryKit.iShtRow, FRCarryKit.iShtCol, &MOT[MMOutArmAOATray]);
            }
            else if(iArea==AOA_OutSht2)
            {
//                GetShuttleSize(iShuttleXItem, iShuttleYItem, iShuttleXPitch, iShuttleYPitch);      //ChungHung 20210113 add for Alignment CCD
                MOT[MMOutArmAOATray].Tray.XItem=BRCarryKit.iShtCol;             //JerryYang 20250204 : fix AOA
                MOT[MMOutArmAOATray].Tray.YItem=BRCarryKit.iShtRow;
                MOT[MMOutArmAOATray].InitNewTray(NULL_IC, false, __FUNC__);
                SearchOutArmAlignmentMaxArea(iArea, &MOT[MMOutArmAOATray]);     //KenHsieh 20210902 : Modify add CCD AUTO ALIGNMENT
                SetAOATrayTeachPoint(BRCarryKit.iShtRow, BRCarryKit.iShtCol, &MOT[MMOutArmAOATray]);
            }
            else if(iArea>=AOA_Auto1 && iArea<=AOA_Fix6)
            {
                MOT[MMOutArmAOATray].Tray.XItem=LoadForm->XDivision;
                MOT[MMOutArmAOATray].Tray.YItem=LoadForm->YDivision;
                MOT[MMOutArmAOATray].InitNewTray(NULL_IC, false, __FUNC__);
                SearchOutArmAlignmentMaxArea(iArea, &MOT[MMOutArmAOATray]);     //KenHsieh 20210902 : Modify add CCD AUTO ALIGNMENT
                SetAOATrayTeachPoint(TestIF.iAlignmentPointX, TestIF.iAlignmentPointY, &MOT[MMOutArmAOATray],true);
            }
            else
            {
                break;
            }
            iIndexX=0;
            iIndexY=0;
            Task=5;                                                             //Kenhsieh 20211007 : AOA Scan時獲取完整Shuttle Xitem
            bFailNeedTeach=false;
            break;
        case 5:                                                                 //Kenhsieh 20211007 : AOA Scan時獲取完整Shuttle Xitem
            if(iArea==AOA_OutSht1 || iArea==AOA_OutSht2)
            {
                iOutShuttleNoscanItemX=0;
                iOutShuttleNoscanItemX=Find_OutArmAlignment_NULL_IC(iOutShuttleScanX);
                Task=25;
            }
            Task=25;
            break;
        case 25:                                                                //Clear flag
            for(int i=0; i<2; i++)
            {
                for(int j=0; j<4; j++)
                {
                    bflag[i][j]=false;
                }
            }
            Task=50;
            //break;
        case 50:                                                                //All Z to safe pos
            for(int i=0; i<2; i++)
            {
                for(int j=0; j<4; j++)
                {
                    if(bflag[i][j]==false)
                        bflag[i][j]= MOT[iOutArmZ[i][j]].MotorMove(Prod.ZOutArmSafe[i][j]);
                }
            }

            if(bflag[0][0] && bflag[0][1] && bflag[0][2] && bflag[0][3] &&
               bflag[1][0] && bflag[1][1] && bflag[1][2] && bflag[1][3])
            {
                iRet=SearchOutArmAlignmentUpDown(iArea, iRow, iCol, iXPos, iYPos);
                if(iRet==1)
                    Task=200;
                else if(iRet==0)
                    Task=1000;
                else if(iRet==3)
                    Task=600;
            }
            break;
        case 200:                                                               //Move to iX,iY position
            if(MoveOutArmAutoAlignmentStage(iArea, iXPos, iYPos))
            {
                HOutArmCCDAlignmentMotorDelay.SetMSAndOn(iCCDAlignmentMotorDelay);
                Task=250;
            }
            break;
        case 250:
            if(HOutArmCCDAlignmentMotorDelay.Off()==false)
                break;

            #ifndef SOFT_SIMULTE
            if(fAutoAlignment->ClientSocket2->Active==true)                     //Ifor 20211221 add: AOA 斷線重新連線
            #endif
            {
                iRetryConnect=0;
                //Send Command Get offsetX offsetY
                if(iArea>=AOA_Auto1 && iArea<=AOA_Fix6)
                {
                    if(bAutoAlignmentCheckHasTray(iArea))
                    {
                        if(bTeach || bFailNeedTeach)
                            fAutoAlignment->OutArmSendCommand(TEACH_TRAY);
                        else
                            fAutoAlignment->OutArmSendCommand(AUTO_TRAY);
                    }
                    else
                    {
                        return 0;
                    }
                }
                else if(iArea==AOA_OutSht1)
                {
                    if(bTeach || bFailNeedTeach)
                        fAutoAlignment->OutArmSendCommand(TEACH_SHUTTLE);
                    else
                        fAutoAlignment->OutArmSendCommand(AUTO_SHUTTLE);
                }
                else if(iArea==AOA_OutSht2)
                {
                    if(bTeach || bFailNeedTeach)
                        fAutoAlignment->OutArmSendCommand(TEACH_SHUTTLE);
                    else
                        fAutoAlignment->OutArmSendCommand(AUTO_SHUTTLE);
                }
                else
                {
                    break;
                }
                Task=300;
            }
            #ifndef SOFT_SIMULTE
            else
            {
                if(iRetryConnect>=3)
                {
                    ShowMyMessage("AOA System OutArm Connect Error!");
                }
                else
                {
                    iRetryConnect++;
                    fAutoAlignment->ClientSocket2->Close();
                    HOutArmCCDAlignmentMotorDelay.SetMSAndOn(200);
                    Task=260;
                }
            }
            #endif
            break;
        case 260:                                                               //KenHsieh 20220103 : AOA 斷線重新連線
            if(HOutArmCCDAlignmentMotorDelay.Off())
            {
                fAutoAlignment->ClientSocket2->Open();
                HOutArmCCDAlignmentMotorDelay.SetMSAndOn(200);
                Task=250;
            }
            break;
        case 300:
            if(iArea>=AOA_Auto1 && iArea<=AOA_Fix6)
            {
                if(bAutoAlignmentCheckHasTray(iArea))
                {
                    if(bTeach || bFailNeedTeach)
                    {
                        iRet=fAutoAlignment->OutArmGetResult(TEACH_TRAY, dOffsetX, dOffsetY);
                        if(iRet==0)                                             //KenHsieh 20220221 : 修改cmd teach類指令取值流程
                        {
                            ShowMyMessage("Please Check OutArm Tray Patten in AutoAlignment PC!");
                            return 0;
                        }
                        else if(iRet==2)
                        {
                            ShowMyMessage("Please Check OutArm Alignment Tray Mark or Check Tray");
                            Task=250;
                            return 2;
                        }
                        bFailNeedTeach=false;
                    }
                    else
                    {
                        iRet=fAutoAlignment->OutArmGetResult(AUTO_TRAY, dOffsetX, dOffsetY);
                        if(iRet==0)
                        {
                            return 0;
                        }
                        else if(iRet==2)
                        {
                            bFailNeedTeach=true;
                            Task=250;
                            return 0;
                        }
                    }
                }
                else
                {
                    return 0;
                }
            }
            else if(iArea==AOA_OutSht1)
            {
//                GetShuttleSize(iShuttleXItem, iShuttleYItem, iShuttleXPitch, iShuttleYPitch);      //ChungHung 20210113 add for Alignment CCD

                if(bTeach || bFailNeedTeach)
                {
                    iRet=fAutoAlignment->OutArmGetResult(TEACH_SHUTTLE, dOffsetX, dOffsetY);
                    if(iRet==0)                                                 //KenHsieh 20220221 : 修改cmd teach類指令取值流程
                    {
                        ShowMyMessage("Please Check OutArm Shuttle Patten in AutoAlignment PC!");
                        return 0;
                    }
                    else if(iRet==2)
                    {
                        ShowMyMessage("Please Check OutArm Alignment Shuttle Mark!");
                        Task=250;
                        return 2;
                    }
                    bFailNeedTeach=false;
                }
                else
                {
                    iRet=fAutoAlignment->OutArmGetResult(AUTO_SHUTTLE, dOffsetX, dOffsetY);
                    if(iRet==0)
                    {
                        return 0;
                    }
                    else if(iRet==2)
                    {
                        bFailNeedTeach=true;
                        Task=250;
                        return 0;
                    }
                }
            }
            else if(iArea==AOA_OutSht2)
            {
//                GetShuttleSize(iShuttleXItem, iShuttleYItem, iShuttleXPitch, iShuttleYPitch);      //ChungHung 20210113 add for Alignment CCD

                if(bTeach || bFailNeedTeach)
                {
                    iRet=fAutoAlignment->OutArmGetResult(TEACH_SHUTTLE, dOffsetX, dOffsetY);
                    if(iRet==0)                                                 //KenHsieh 20220221 : 修改cmd teach類指令取值流程
                    {
                        ShowMyMessage("Please Check OutArm Shuttle Patten in AutoAlignment PC!");
                        return 0;
                    }
                    else if(iRet==2)
                    {
                        ShowMyMessage("Please Check OutArm Alignment Shuttle Mark!");
                        Task=250;
                        return 2;
                    }
                    bFailNeedTeach=false;
                }
                else
                {
                    iRet=fAutoAlignment->OutArmGetResult(AUTO_SHUTTLE, dOffsetX, dOffsetY);
                    if(iRet==0)
                    {
                        return 0;
                    }
                    else if(iRet==2)
                    {
                        bFailNeedTeach=true;
                        Task=250;
                        return 0;
                    }
                }
            }
            else
            {
                break;
            }

            if(bTeach)
                return 1;
            else
                Task=400;
            break;
        case 400:
            if(bSpin)
            {
                RotationCoordinates(dOffsetX, dOffsetY, dOffsetX1, dOffsetY1, Prod.dOutArmCCDXRadian, Prod.dOutArmCCDYRadian);    //ChungHung 20210113 add for Alignment CCD
                dOffsetX1=dOffsetX1*Prod.dOutArmCCDXResolution;
                dOffsetY1=dOffsetY1*Prod.dOutArmCCDYResolution;
            }
            else
            {
                dOffsetX1=dOffsetX*Prod.dOutArmCCDXResolution;
                dOffsetY1=dOffsetY*Prod.dOutArmCCDYResolution;
            }
            Task=500;
            return 0;
        case 425:
            if(HOutArmCCDAlignmentMotorDelay.Off()==false)
                break;
            //Send Command Get offsetX offsetY
            if(iArea>=AOA_Auto1 && iArea<=AOA_Fix6)
            {
                if(bTeach)
                    fAutoAlignment->OutArmSendCommand(TEACH_TRAY);
                else
                    fAutoAlignment->OutArmSendCommand(AUTO_TRAY);
            }
            else if(iArea==AOA_OutSht1)
            {
                if(bTeach)
                    fAutoAlignment->OutArmSendCommand(TEACH_SHUTTLE);
                else
                    fAutoAlignment->OutArmSendCommand(AUTO_SHUTTLE);
            }
            else if(iArea==AOA_OutSht2)
            {
                if(bTeach)
                    fAutoAlignment->OutArmSendCommand(TEACH_SHUTTLE);
                else
                    fAutoAlignment->OutArmSendCommand(AUTO_SHUTTLE);
            }
            else
            {
                break;
            }

            Task=450;
            break;
        case 450:
            if(iArea>=AOA_Auto1 && iArea<=AOA_Fix6)
            {
                if(bTeach)
                {
                    iRet=fAutoAlignment->OutArmGetResult(TEACH_TRAY, dOffsetX, dOffsetY);
                    if(iRet==0)
                    {
                        return 0;
                    }
                    else if(iRet==2)
                    {
                        ShowMyMessage("Please Check OutArm Alignment Tray Mark!");
                        return 2;
                    }
                }
                else
                {
                    iRet=fAutoAlignment->OutArmGetResult(AUTO_TRAY, dOffsetX, dOffsetY);
                    if(iRet==0)
                    {
                        return 0;
                    }
                    else if(iRet==2)
                    {
                        ShowMyMessage("Please Check OutArm Alignment Tray Mark!!");
                        return 2;
                    }
                }
            }
            else if(iArea==AOA_OutSht1)
            {
//                GetShuttleSize(iShuttleXItem, iShuttleYItem, iShuttleXPitch, iShuttleYPitch);      //ChungHung 20210113 add for Alignment CCD

                if(bTeach)
                {
                    iRet=fAutoAlignment->OutArmGetResult(TEACH_SHUTTLE, dOffsetX, dOffsetY);
                    if(iRet==0)
                    {
                        return 0;
                    }
                    else if(iRet==2)
                    {
                        ShowMyMessage("Please Check OutArm Alignment Shuttle Mark!");
                        return 2;
                    }
                }
                else
                {
                    iRet=fAutoAlignment->OutArmGetResult(AUTO_SHUTTLE, dOffsetX, dOffsetY);
                    if(iRet==0)
                    {
                        return 0;
                    }
                    else if(iRet==2)
                    {
                        ShowMyMessage("Please Check OutArm Alignment Shuttle Mark!!");
                        return 2;
                    }
                }
            }
            else if(iArea==AOA_OutSht2)
            {
//                GetShuttleSize(iShuttleXItem, iShuttleYItem, iShuttleXPitch, iShuttleYPitch);      //ChungHung 20210113 add for Alignment CCD

                if(bTeach)
                {
                    iRet=fAutoAlignment->OutArmGetResult(TEACH_SHUTTLE, dOffsetX, dOffsetY);
                    if(iRet==0)
                    {
                        return 0;
                    }
                    else if(iRet==2)
                    {
                        ShowMyMessage("Please Check OutArm Alignment Shuttle Mark!");
                        return 2;
                    }
                }
                else
                {
                    iRet=fAutoAlignment->OutArmGetResult(AUTO_SHUTTLE, dOffsetX, dOffsetY);
                    if(iRet==0)
                    {
                        return 0;
                    }
                    else if(iRet==2)
                    {
                        ShowMyMessage("Please Check OutArm Alignment Shuttle Mark!!");
                        return 2;
                    }
                }
            }
            else
            {
                break;
            }
            Task=500;
            break;
        case 500:
            if(iArea==AOA_OutSht1 || iArea==AOA_OutSht2)
            {
//                GetShuttleSize(iShuttleXItem, iShuttleYItem, iShuttleXPitch, iShuttleYPitch);
                OutputAtuoTeachTableCal[iArea].ItemX=iOutShuttleNoscanItemX;    //Kenhsieh 20211007 : AOA Scan時獲取完整Shuttle Xitem
                OutputAtuoTeachTableCal[iArea].ItemY=FRCarryKit.iShtCol;
                OutputAtuoTeachTable[iArea].ItemX=iOutShuttleNoscanItemX;       //Kenhsieh 20211007 : AOA Scan時獲取完整Shuttle Xitem
                OutputAtuoTeachTable[iArea].ItemY=FRCarryKit.iShtCol;
            }
            else
            {
                OutputAtuoTeachTableCal[iArea].ItemX=TestIF.iAlignmentPointX;
                OutputAtuoTeachTableCal[iArea].ItemY=TestIF.iAlignmentPointY;
                OutputAtuoTeachTable[iArea].ItemX=TestIF.iAlignmentPointX;
                OutputAtuoTeachTable[iArea].ItemY=TestIF.iAlignmentPointY;
            }

            //KenHsieh 20210829 : Modify add CCD AUTO ALIGNMENT (XY變距) //KenHsieh 20210913 : 新增CCD AUTO ALIGNMENT Log
            //==>
            if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable)
            {
                OutputAtuoTeachTableCal[iArea].pointX[iIndexX][iIndexY]=iXPos-Prod.iOutArmCCD_Picker_OffsetX[iInArmYBase][iInArmXBase];  //JerryYang 20250204 : fix AOA
                OutputAtuoTeachTableCal[iArea].pointY[iIndexX][iIndexY]=iYPos-Prod.iOutArmCCD_Picker_OffsetY[iInArmYBase][iInArmXBase];
                OutputAtuoTeachTable[iArea].pointX[iIndexX][iIndexY]   =iXPos-Prod.iOutArmCCD_Picker_OffsetX[iInArmYBase][iInArmXBase]+dOffsetX1;
                OutputAtuoTeachTable[iArea].pointY[iIndexX][iIndexY]   =iYPos-Prod.iOutArmCCD_Picker_OffsetY[iInArmYBase][iInArmXBase]+dOffsetY1;
            }
            else
            {
                OutputAtuoTeachTableCal[iArea].pointX[iIndexX][iIndexY]=iXPos-Prod.iOutArmCCD_Picker_OffsetX[iInArmYBase][iInArmXBase];
                OutputAtuoTeachTableCal[iArea].pointY[iIndexX][iIndexY]=iYPos-Prod.iOutArmCCD_Picker_OffsetY[iInArmYBase][iInArmXBase];
                OutputAtuoTeachTable[iArea].pointX[iIndexX][iIndexY]   =iXPos-Prod.iOutArmCCD_Picker_OffsetX[iInArmYBase][iInArmXBase]+dOffsetX1;
                OutputAtuoTeachTable[iArea].pointY[iIndexX][iIndexY]   =iYPos-Prod.iOutArmCCD_Picker_OffsetY[iInArmYBase][iInArmXBase]+dOffsetY1;
            }
            //<==
            //KenHsieh 20210829 : Modify add CCD AUTO ALIGNMENT (XY變距) //KenHsieh 20210913 : 新增CCD AUTO ALIGNMENT Log

            SaveOutArmAutoAlignmentLog(iArea, iIndexX, iIndexY);                //KenHsieh 20220111 : AOA add Save Log

            MOT[MMOutArmAOATray].SetTraySingleData(iCol, iRow, NULL_IC);

            iIndexX++;
            if(iIndexX>=OutputAtuoTeachTableCal[iArea].ItemX)
            {
                iIndexX=0;
                iIndexY++;
            }

            Task=25;
            break;
        case 600:
            MOT[MMOutArmAOATray].SetTraySingleData(iIndexX, iRow, NULL_IC);

            iIndexX++;
            if(iIndexX>=OutputAtuoTeachTableCal[iArea].ItemX)
            {
                iIndexX=0;
                iIndexY++;
            }

            Task=25;
            break;
        case 1000:
            if(iArea<TotalOutArmAOAType)
            {
                WriteAutoTeachTable(asOutArmAOAFileName[iArea].c_str(), &OutputAtuoTeachTable[iArea]);
                WriteAutoTeachTable(asOutArmAOAFileName_Cal[iArea].c_str(), &OutputAtuoTeachTableCal[iArea]);
            }
            else
            {
                break;
            }

            ReadAutoTeachTable_OutArm();                                        //KenHsieh 20211208 : In/Out Arm分開讀檔，避免InArm做完直接寫入導致OutArm點位錯誤
            //save data
            return 1;
    }
    return 0;
}
//------------------------------------------------------------------------------
bool DoMoveAlignmentToUnloaderStage_CCD(int iAuto, int iRow, int iCol, bool realMove)
{
    int iXPos=0, iYPos=0;                                                       //pig 2016.09.21 SetInitialValue
    bool flag1, flag2;

    if(CheckOutArmZ(false))
    {
        bNeedArmZHome=true;
        InitDoArmZHome();
        return false;
    }

    iXPos=Prod.XStart[iAuto][iRow][iCol]+(iCol)*AutoForm[iAuto]->XPitch;
    iYPos=Prod.YStart[iAuto][iRow][iCol]-(iRow)*AutoForm[iAuto]->YPitch;

    if(realMove==false)
    {
        if(iXPos>=MOT[MOutArmX].Motor->PSoftLimitP || iXPos<=MOT[MOutArmX].Motor->PSoftLimitN)
            return false;
        if(iYPos>=MOT[MOutArmY].Motor->PSoftLimitP || iYPos<=MOT[MOutArmY].Motor->PSoftLimitN)
            return false;
        return true;
    }

    flag1=MOT[MOutArmX].MotorMove(iXPos);
    flag2=MOT[MOutArmY].MotorMove(iYPos);
    if(flag1 && flag2)
    {
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
int iDoAutoRunUnloaderTrayTask=1;
//------------------------------------------------------------------------------
void InitDoAutoRunUnloaderTray()
{
    iDoAutoRunUnloaderTrayTask=1;
}
//------------------------------------------------------------------------------
bool DoAutoRunUnloaderTray(int iArea)
{
    int &Task=iDoAutoRunUnloaderTrayTask;

    static int iRow=0, iCol=0;
    int iXPos=0, iYPos=0;
    double dOffsetX=0.0, dOffsetY=0.0;
    int iRet=0;
    switch(Task)
    {
        case 1:                                                                 //初始化Tray
            ReadAutoTeachTable_OutArm();                                        //KenHsieh 20211208 : In/Out Arm分開讀檔，避免InArm做完直接寫入導致OutArm點位錯誤
            MOT[MMOutArmAOATray].Tray.XItem=LoadForm->XDivision;
            MOT[MMOutArmAOATray].Tray.YItem=LoadForm->YDivision;
            MOT[MMOutArmAOATray].InitNewTray(WAIT_ALIGN_IC, false, __FUNC__);
            Task=100;
            break;
        case 100:                                                               //Move To Target
            iRet=SearchOutArmAlignmentUpDown(iArea, iRow, iCol, iXPos, iYPos);
            if(iRet==1)
            {
                Task=200;
            }
            else if(iRet==0)
            {
                Task=1000;
            }
            break;
        case 200:
            if(DoMoveAlignmentToUnloaderStage_CCD(iArea, iRow, iCol, true))
            {
                HOutArmCCDAlignmentMotorDelay.SetMSAndOn(iCCDAlignmentMotorDelay);
                Task=250;
            }
            break;
        case 250:
            if(HOutArmCCDAlignmentMotorDelay.Off()==false)
                break;
            MOT[MMOutArmAOATray].SetTraySingleData(iCol, iRow, NULL_IC);
            fAutoAlignment->OutArmSendCommand(AUTO_TRAY);
            Task=300;
            break;
        case 300:
            if(fAutoAlignment->OutArmGetResult(AUTO_TRAY, dOffsetX, dOffsetY))
            {
                Task=100;
            }
            break;
        case 1000:
            return true;
    }
    return false;
}
//------------------------------------------------------------------------------
bool DoMoveOutArmPickerAndPitchAutoAlignment(int iPicker, int iPitch, int &iXPos, int &iYPos)
{
    bool flag1, flag2, flag3, flag4, flag5;                                     //KenHsieh 20211013 : Modify 吸嘴Teach X2、Y Pitch位置
    int iR=0, iC=0;
    int iPos[4]={-2, -1, 0, 1};
    int iPos2[4]={-1, 0, 1, 2};

    if(CheckOutArmZ(false))
    {
        bNeedArmZHome=true;
        InitDoArmZHome();
        return false;
    }

    //KenHsieh 20210829 : Modify add CCD AUTO ALIGNMENT (XY變距)
    //==>
    if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable)
    {
        if(iPicker%2==1)
            iR=0;
        else
            iR=1;
        iYPos = Tech.iOutArmYBasePickerAlignmentPos-(iR*IN_OUT_ARM_Y_PITCH_MAX);   //KenHsieh 20211013 : Modify 吸嘴Teach X2、Y Pitch位置
    }
    else
    {
        if(iPicker%2==1)
            iR=1;
        else
            iR=0;
        iYPos = Tech.iOutArmYBasePickerAlignmentPos+(iR*6000);
    }

    if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable)
    {
        iC=iPicker/2;
        if(bOutArmXPitch_40mm)                                                  //KenHsieh 20211111 : Modify X Pitch 40mm teach
            iXPos=Tech.iOutArmXBasePickerAlignmentPos-iPos2[iC]*iPitch/3;
        else
            iXPos=Tech.iOutArmXBasePickerAlignmentPos-iPos2[iC]*iPitch;
    }
    else
    {
        iC=iPicker/2;
        if(bOutArmXPitch_40mm)                                                  //KenHsieh 20211111 : Modify X Pitch 40mm teach
            iXPos=Tech.iOutArmXBasePickerAlignmentPos-iPos[iC]*iPitch/3;
        else
            iXPos=Tech.iOutArmXBasePickerAlignmentPos-iPos[iC]*iPitch;
    }
    //KenHsieh 20210829 : Modify add CCD AUTO ALIGNMENT (XY變距)
    //<==

    if(iXPos>=MOT[MOutArmX].Motor->PSoftLimitP || iXPos<=MOT[MOutArmX].Motor->PSoftLimitN)
        return false;
    if(iYPos>=MOT[MOutArmY].Motor->PSoftLimitP || iYPos<=MOT[MOutArmY].Motor->PSoftLimitN)
        return false;

    flag1=MOT[MOutArmX].MotorMove(iXPos);
    flag2=MOT[MOutArmY].MotorMove(iYPos);
    if(bOutArmXPitch_40mm)                                                      //KenHsieh 20211023 : 新增X Pitch 40mm teach
        flag3=MOT[MOutArmPitch].MotorMove(GetOutArmPitch_9045(iPitch));
    else
        flag3=MOT[MOutArmPitch].MotorMove(GetOutArmPitch_9045(iPitch*3));

    if(USE_IN_OUT_ARM_Y_PITCH==iXYPitchVariable)                                //KenHsieh 20211013 : Modify 吸嘴Teach X2、Y Pitch位置
    {
        if(flag3==true)                                                         //KenHsieh 20211111 : 解決Hang up
        {
            if(bOutArmXPitch_40mm)                                              //KenHsieh 20211111 : Modify X Pitch 40mm teach
                flag4=MOT[MOutArmPitchX2].MotorMove(GetOutArmPitch2_9045(iPitch));
            else
                flag4=MOT[MOutArmPitchX2].MotorMove(GetOutArmPitch2_9045(iPitch*3));
            flag5=MOT[MOutArmPitchY].MotorMove(GetOutArmPitchY_9045(IN_OUT_ARM_Y_PITCH_MAX));
        }
    }
    else
    {
        flag4=true;
        flag5=true;
    }

    if(flag1 && flag2 && flag3 && flag4 && flag5)                               //KenHsieh 20211013 : Modify 吸嘴Teach X2、Y Pitch位置
    {
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
void SaveOutArmAutoAlignmentLog(int iArea, int iIndexX, int iIndexY)            //KenHsieh 20220111 : AOA add Save Log
{
    AnsiString AnsBuffer="";                                                    //KenHsieh 20210913 : 新增CCD AUTO ALIGNMENT Log
    AnsiString AnsBuffer01="";                                                  //KenHsieh 20210913 : 新增CCD AUTO ALIGNMENT Log
    AnsiString asPath="";

    asPath.sprintf("%s\\%04d\\%02d", asOutArmAutoAlignmentPath, SystemYear, SystemMonth);
    MyForceDirectories(asPath);
    asPath.sprintf("%s\\%04d\\%02d\\OutArmAOALog_%04d%02d%02d.txt", asOutArmAutoAlignmentPath, SystemYear, SystemMonth, SystemYear, SystemMonth, SystemDate);

    if(fMain->Memo3_AOA_OUT->Lines->Count>=1024)
        fMain->Memo3_AOA_OUT->Clear();

    AnsBuffer01.sprintf("%02d:%02d:%02d.%03d", SystemHour, SystemMin, SystemSec, SystemMSec);
    AnsBuffer.sprintf("%s:  OutputTableCal[%d] PointX[%d][%d]= %f", AnsBuffer01, iArea, iIndexX, iIndexY, OutputAtuoTeachTableCal[iArea].pointX[iIndexX][iIndexY]);
    fMain->Memo3_AOA_OUT->Lines->Add(AnsBuffer);
    WriteDataToFile(asPath, AnsBuffer);

    AnsBuffer.sprintf("%s:  OutputTableCal[%d] PointY[%d][%d]= %f", AnsBuffer01, iArea, iIndexX, iIndexY, OutputAtuoTeachTableCal[iArea].pointY[iIndexX][iIndexY]);
    fMain->Memo3_AOA_OUT->Lines->Add(AnsBuffer);
    WriteDataToFile(asPath, AnsBuffer);

    AnsBuffer.sprintf("%s:  OutputTable   [%d] PointX[%d][%d]= %f", AnsBuffer01, iArea, iIndexX, iIndexY, OutputAtuoTeachTable[iArea].pointX[iIndexX][iIndexY]);
    fMain->Memo3_AOA_OUT->Lines->Add(AnsBuffer);
    WriteDataToFile(asPath, AnsBuffer);

    AnsBuffer.sprintf("%s:  OutputTable   [%d] PointY[%d][%d]= %f", AnsBuffer01, iArea, iIndexX, iIndexY, OutputAtuoTeachTable[iArea].pointY[iIndexX][iIndexY]);
    fMain->Memo3_AOA_OUT->Lines->Add(AnsBuffer);
    WriteDataToFile(asPath, AnsBuffer);
}
//------------------------------------------------------------------------------
int iOutArmPickerXYAndPitchAutoAlignment;
void InitOutArmPickerXYAndPitchAutoAlignment()
{
    iOutArmPickerXYAndPitchAutoAlignment=1;
}
//------------------------------------------------------------------------------
HTimer HOutArmPitchZAutoAlignmentMotorDelay;                                    //Kenhsieh 20211006 : Z軸到位Delay
int DoOutArmPickerXYAndPitchAutoAlignment(int iBasePicker, bool bAll, int *iXPos, int *iYPos, int *iZPos) //0: 未完成 1: 成功 2: 中斷
{
    int &Task=iOutArmPickerXYAndPitchAutoAlignment;
    int iPickerZPos[8]={0, 0, 0, 0, 0, 0, 0, 0};
    int iOutArmZ[8]={MOutArmZA, MOutArmZB, MOutArmZC, MOutArmZD, MOutArmZE, MOutArmZF, MOutArmZG, MOutArmZH};
    static int iStep=0;
    static int iOutArmXPos_P[8];
    static int iOutArmXPos_N[8];
    static int iOutArmYPos_P[8];
    static int iOutArmYPos_N[8];
    static int iOutArmZPickZ[8]={0, 0, 0, 0, 0, 0, 0, 0};
    static int iXpos;
    static int iYpos;
    static int iIndex;

//    for(int i=0; i<8; i++)
//        iPickerZPos[i]=Prod.iOutArmZBasePickerAlignmentPos[i];

    for (int i=0; i<4; ++i)                                                 //Ifor 20250903 add:改成二維陣列
    {
        // 將 Prod 陣列前排的值，依序放入 iPickerZPos 的偶數索引 (0, 2, 4, 6)
        iPickerZPos[i*2] = Prod.iInArmZBasePickerAlignmentPos[0][i];
        // 將 Prod 陣列後排的值，依序放入 iPickerZPos 的奇數索引 (1, 3, 5, 7)
        iPickerZPos[i*2 +1] = Prod.iInArmZBasePickerAlignmentPos[1][i];
    }

    if(USE_PICKER_COUNT==ep16Picker)                                            //Ifor 20250903 add:16Pick 待更新
    {
    }

    switch(Task)
    {
        case 1:  //All OutArmZ Move SafePostion
            Cylinder[C_OutAreaAlignment].On();

            if(MoveOutArmToAutoSafe())
            {
                for(int i=0; i<8; i++)
                {
                    iOutArmXPos_P[i]=0;
                    iOutArmXPos_N[i]=0;
                    iOutArmYPos_P[i]=0;
                    iOutArmYPos_N[i]=0;
                    iOutArmZPickZ[i]=0;
                }
                iIndex=iBasePicker;
                Task=100;
            }
            break;
        case 100:   //Move to Teach Postion
            //檢測Sensor 必須為亮
            if(Sen[SnOutAreaAlignmentSenX].IsOn())
            {
                Task=3000;
            }
            else if(DoMoveOutArmPickerAndPitchAutoAlignment(iIndex, 4000, iXpos, iYpos))
            {
                Task=200;
            }
            break;
        case 200: // InArmZ Move To Touch Sensor
            if(Cylinder[C_OutAreaAlignment].Push())
            {
                Task=225;
            }
            break;
        case 225:
            if(iPickerZPos[iIndex]==0)      //KenHsieh 20211015 : Teach吸嘴Z軸位置為0判斷
            {
                Task=2500;
            }
            else if(MOT[iOutArmZ[iIndex]].MotorMove(iPickerZPos[iIndex]))       //Move to Z sandby postion
            {
                HOutArmPitchZAutoAlignmentMotorDelay.SetSecAndOn(3);            //Kenhsieh 20211006 : Z軸到位Delay
                Task=230;                                                       //Kenhsieh 20211006 : Z軸到位Delay
            }
            break;
        case 230:   //Kenhsieh 20211006 : Z軸到位Delay
            //檢測Sensor 必須為亮
            if(HOutArmPitchZAutoAlignmentMotorDelay.Off())
            {
                if(Sen[SnOutAreaAlignmentSenX].IsOff())
                {
                    Task=2000;
                }
                else
                {
                    iStep=0;
                    Task=300;
                }
            }
            break;
        case 250:
            iStep++;
            Task=300;
            break;
        case 300:
            if(MOT[MOutArmX].MotorMove(iXpos+iXYMovePos*iStep)) // right move
            {
                if(Sen[SnOutAreaAlignmentSenX].IsOff())
                {
                    iStep=0;
                    iOutArmXPos_P[iIndex] = MOT[MOutArmX].ReadPos();
                    Task=400;
                }
                else
                {
                    Task=250;
                }
            }
            break;
        case 350:
            iStep++;
            Task=450;
            break;
        case 400:
            if(MOT[MOutArmX].MotorMove(iXpos))
            {
                if(Sen[SnOutAreaAlignmentSenX].IsOff())     //KenHsieh 20211109 : 解決吸嘴掃描Golden時，無掃描之問題
                    Task=2000;
                else
                    Task=450;
            }
            break;
        case 450:
            if(MOT[MOutArmX].MotorMove(iXpos-iXYMovePos*iStep)) // left move
            {
                if(Sen[SnOutAreaAlignmentSenX].IsOff())
                {
                    iOutArmXPos_N[iIndex]=MOT[MOutArmX].ReadPos();
                    Task=500;
                }
                else
                {
                    Task=350;
                }
            }
            break;
        case 500:
            if(MOT[MOutArmX].MotorMove(iXpos))
            {
                iStep=0;
                Task=600;
                Cylinder[C_OutAreaAlignment].Off();
            }
            break;
        case 550:
            iStep++;
            Task=625;
            break;
        case 600:
            if(Cylinder[C_OutAreaAlignment].Pop())
            {
                Task=610;   //KenHsieh 20211109 : 解決吸嘴掃描Golden時，無掃描之問題
            }
            break;
        case 610:       //KenHsieh 20211109 : 解決吸嘴掃描Golden時，無掃描之問題
            if(Sen[SnOutAreaAlignmentSenX].IsOff())
                Task=2000;
            else
                Task=625;
            break;
        case 625:
            if(MOT[MOutArmY].MotorMove(iYpos+iXYMovePos*iStep)) // up move
            {
                if(Sen[SnOutAreaAlignmentSenX].IsOff())
                {
                    iStep=0;
                    iOutArmYPos_P[iIndex]=MOT[MOutArmY].ReadPos();
                    Task=700;
                }
                else
                {
                    Task=550;
                }
            }
            break;
        case 650:
            iStep++;
            Task=710;
            break;
        case 700:
            if(MOT[MOutArmY].MotorMove(iYpos))
            {
                if(Sen[SnOutAreaAlignmentSenX].IsOff())     //KenHsieh 20211109 : 解決吸嘴掃描Golden時，無掃描之問題
                    Task=2000;
                else
                    Task=710;
            }
            break;
        case 710:
            if(MOT[MOutArmY].MotorMove(iYpos-iXYMovePos*iStep)) // down move
            {
                if(Sen[SnOutAreaAlignmentSenX].IsOff())
                {
                    iStep=0;
                    iOutArmYPos_N[iIndex]=MOT[MOutArmY].ReadPos();
                    Task=725;
                }
                else
                {
                    Task=650;
                }
            }
            break;
        case 725:
            if(MOT[MOutArmY].MotorMove(iYpos)) // down move
            {
                iStep=0;
                Task=800;
            }
            break;
        case 750:
            iStep++;
            Task=800;
            break;
        case 800:
            if(MOT[iOutArmZ[iIndex]].MotorMove(iPickerZPos[iIndex]+iZMovePos*iStep))  //leave
            {
                if(Sen[SnOutAreaAlignmentSenX].IsOff())
                {
                    iStep=0;
                    iOutArmZPickZ[iIndex] = MOT[iOutArmZ[iIndex]].ReadPos();
                    Task=900;
                }
                else
                {
                    Task=750;
                }
            }
            break;
        case 900:
            if(MoveOutArmToAutoSafe())
            {
                if(bAll==false)
                {
                    Task=1000;
                }
                else
                {
                    for(int i=0; i<8; i++)  //1x4
                    {
                        if(iOutArmZPickZ[i]==0)
                        {
                            iIndex = i;
                            Task=100;
                            return 0;
                        }
                    }
                    Task=1000;
                }
            }
            else if(bNeedArmZHome==true)
            {
                ShowMyMessage("Please Check OutArm Picker Home Sensor");
                return 2;
            }
            break;
        case 1000:
            for(int i=0; i<8; i++)
            {
                iXPos[i]=(iOutArmXPos_P[i]+iOutArmXPos_N[i])/2;
                iYPos[i]=(iOutArmYPos_P[i]+iOutArmYPos_N[i])/2;
                iZPos[i]= iOutArmZPickZ[i];
            }
            return 1;
        case 2000:
            if(MoveOutArmToAutoSafe())
            {
                ShowMyMessage("Please Check OutArm Alignment Picker Teach Postion,\r\n and AOA Golden Sensor must On");     //KenHsieh 20211015 : 修改吸嘴未到位提示訊息
                return 2;
            }
            else if(bNeedArmZHome==true)
            {
                ShowMyMessage("Please Check OutArm Picker Home Sensor");
                return 2;
            }
            break;
        case 2500:      //KenHsieh 20211015 : Teach吸嘴Z軸位置為0判斷
            if(MoveOutArmToAutoSafe())
            {
                ShowMyMessage("OutArm Z Base Pick Pos is 0, please modify the value, and restart picker teach.");
                return 2;
            }
            break;
        case 3000:
            if(MoveOutArmToAutoSafe())
            {
                ShowMyMessage("Please Check OutArm Alignment Picker Teach Postion,\r\n and AOA Golden Sensor must Off");   //KenHsieh 20211015 : 修改吸嘴未到位提示訊息
                return 2;
            }
            else if(bNeedArmZHome==true)
            {
                ShowMyMessage("Please Check OutArm Picker Home Sensor");
                return 2;
            }
            break;
    }
    return 0;
}
//------------------------------------------------------------------------------
int iInitOutArmAlignment=1;
//------------------------------------------------------------------------------
void InitOutArmAlignment()
{
    iInitOutArmAlignment=1;
}
//------------------------------------------------------------------------------
bool DoOutArmTeachAlignment(unsigned long &lAction)
{
    int &Task=iInitOutArmAlignment;

    AnsiString sPath="D:\\HT9045\\System\\teach.ini", str="";
    TIniFile *INIFile;

    double ResolutionX, ResolutionY, dRadianX, dRadianY;
    int iRet=0;
    int iXPos[8], iYPos[8], iZPos[8];
    int iPosX, iPosY;
    if(lAction&OUTARM_ALIGNMENT_TEACH_UNIT)       //0x1
    {
        switch(Task)
        {
            case 1:
                RecordProcess("Alignment Teach OutArm Unit Start");
                InitOutArmCCDAlignmentTeachUnitProcess();
                Task=100;
                break;
            case 100:
                iRet=DoOutArmCCDAlignmentTeachUnitProces(ResolutionX, ResolutionY, dRadianX, dRadianY, 1);  //0: Correction mm/pixel 1: Teach Pattern 2: Teach Pattern and Correction mm/pixel
                if(iRet==1)
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_TEACH_UNIT;
                    RecordProcess("Alignment Teach OutArm Unit Finish");
                    return true;
                }
                else if(iRet==2)
                {
                    Task=1;
                }
                break;
        }
    }
    else if(lAction&OUTARM_ALIGNMENT_TEACH_PICKER)     //0x2
    {
        switch(Task)
        {
            case 1:
                RecordProcess("Alignment Teach OutArm Picker Start");
                InitOutArmPickerXYAndPitchAutoAlignment();
                Task=100;
                break;
            case 100:
                iRet=DoOutArmPickerXYAndPitchAutoAlignment(4, true, iXPos, iYPos, iZPos);
                if(iRet==1)
                {
                    INIFile=new TIniFile(sPath);
                    INIFile->WriteInteger("OutArm", "iOutArmPicker1_X", iXPos[0]);         //ChungHung 20210113 add for Alignment CCD
                    INIFile->WriteInteger("OutArm", "iOutArmPicker1_Y", iYPos[0]);         //ChungHung 20210113 add for Alignment CCD
                    INIFile->WriteInteger("OutArm", "iOutArmPicker1_Z", iZPos[0]);         //ChungHung 20210113 add for Alignment CCD

                    INIFile->WriteInteger("OutArm", "iOutArmPicker2_X", iXPos[1]);         //ChungHung 20210113 add for Alignment CCD
                    INIFile->WriteInteger("OutArm", "iOutArmPicker2_Y", iYPos[1]);         //ChungHung 20210113 add for Alignment CCD
                    INIFile->WriteInteger("OutArm", "iOutArmPicker2_Z", iZPos[1]);         //ChungHung 20210113 add for Alignment CCD

                    INIFile->WriteInteger("OutArm", "iOutArmPicker3_X", iXPos[2]);         //ChungHung 20210113 add for Alignment CCD
                    INIFile->WriteInteger("OutArm", "iOutArmPicker3_Y", iYPos[2]);         //ChungHung 20210113 add for Alignment CCD
                    INIFile->WriteInteger("OutArm", "iOutArmPicker3_Z", iZPos[2]);         //ChungHung 20210113 add for Alignment CCD

                    INIFile->WriteInteger("OutArm", "iOutArmPicker4_X", iXPos[3]);         //ChungHung 20210113 add for Alignment CCD
                    INIFile->WriteInteger("OutArm", "iOutArmPicker4_Y", iYPos[3]);         //ChungHung 20210113 add for Alignment CCD
                    INIFile->WriteInteger("OutArm", "iOutArmPicker4_Z", iZPos[3]);         //ChungHung 20210113 add for Alignment CCD
                    delete INIFile;

                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_TEACH_PICKER;
                    RecordProcess("Alignment Teach OutArm Picker Finish");
                    return true;
                }
                else if(iRet==2)
                {
                    Task=1;
                }
                break;
        }
    }
    else if(lAction&OUTARM_ALIGNMENT_TEACH_AUTO1)
    {
        switch(Task)
        {
            case 1:
                if(Sen[SnAutoTrayDetect[eAuto1]].IsOff())                       //Ifor 20210927 add:沒Tray 不掃AOA避免流程卡死
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_TEACH_AUTO1;
                    RecordProcess("Alignment Teach OutArm Auto1 No Tray Pass");
                    return true;
                }
                else
                {
                    RecordProcess("Alignment Teach OutArm Auto1 Start");
                    InitOutArmXYAutoTeachProcess();
                    Task=100;
                }
                break;
            case 100:
                iRet=DoOutArmXYAlignmentTeachProcess(AOA_Auto1, true);
                if(iRet==1)
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_TEACH_AUTO1;
                    RecordProcess("Alignment Teach OutArm Auto1 Finish");
                    return true;
                }
                else if(iRet==2)
                {
                    Task=1;
                }
                break;
        }
    }
    else if(lAction&OUTARM_ALIGNMENT_TEACH_AUTO2)
    {
        switch(Task)
        {
            case 1:
                if(Sen[SnAutoTrayDetect[eAuto2]].IsOff())                       //Ifor 20210927 add:沒Tray 不掃AOA避免流程卡死
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_TEACH_AUTO2;
                    RecordProcess("Alignment Teach OutArm Auto2 No Tray Pass");
                    return true;
                }
                else
                {
                    RecordProcess("Alignment Teach OutArm Auto2 Start");

                    InitOutArmXYAutoTeachProcess();
                    Task=100;
                }
                break;
            case 100:
                iRet=DoOutArmXYAlignmentTeachProcess(AOA_Auto2, true);
                if(iRet==1)
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_TEACH_AUTO2;
                    RecordProcess("Alignment Teach OutArm Auto2 Finish");
                    return true;
                }
                else if(iRet==2)
                {
                    Task=1;
                }
                break;
        }
    }
    else if(lAction&OUTARM_ALIGNMENT_TEACH_AUTO3)
    {
        switch(Task)
        {
            case 1:
                if(Sen[SnAutoTrayDetect[eAuto3]].IsOff())                       //Ifor 20210927 add:沒Tray 不掃AOA避免流程卡死
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_TEACH_AUTO3;
                    RecordProcess("Alignment Teach OutArm Auto3 No Tray Pass");
                    return true;
                }
                else
                {
                    RecordProcess("Alignment Teach OutArm Auto3 Start");

                    InitOutArmXYAutoTeachProcess();
                    Task=100;
                }
                break;
            case 100:
                iRet=DoOutArmXYAlignmentTeachProcess(AOA_Auto3, true);
                if(iRet==1)
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_TEACH_AUTO3;
                    RecordProcess("Alignment Teach OutArm Auto3 Start");
                    return true;
                }
                else if(iRet==2)
                {
                    Task=1;
                }
                break;
        }
    }
    else if(lAction&OUTARM_ALIGNMENT_TEACH_AUTO4)
    {
        switch(Task)
        {
            case 1:
                if(Sen[SnAutoTrayDetect[eAuto4]].IsOff())                       //Ifor 20210927 add:沒Tray 不掃AOA避免流程卡死
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_TEACH_AUTO4;
                    RecordProcess("Alignment Teach OutArm Auto4 No Tray Pass");
                    return true;
                }
                else
                {
                    RecordProcess("Alignment Teach OutArm Auto4 Start");
                    InitOutArmXYAutoTeachProcess();
                    Task=100;
                }
                break;
            case 100:
                iRet=DoOutArmXYAlignmentTeachProcess(AOA_Auto4, true);
                if(iRet==1)
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_TEACH_AUTO4;
                    RecordProcess("Alignment Teach OutArm Auto4 Finish");
                    return true;
                }
                else if(iRet==2)
                {
                    Task=1;
                }
                break;
        }
    }
    else if(lAction&OUTARM_ALIGNMENT_TEACH_AUTO5)
    {
        switch(Task)
        {
            case 1:
                if(Sen[SnAutoTrayDetect[eAuto5]].IsOff())                       //Ifor 20210927 add:沒Tray 不掃AOA避免流程卡死
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_TEACH_AUTO5;
                    RecordProcess("Alignment Teach OutArm Auto5 No Tray Pass");
                    return true;
                }
                else
                {
                    RecordProcess("Alignment Teach OutArm Auto5 Start");
                    InitOutArmXYAutoTeachProcess();
                    Task=100;
                }
                break;
            case 100:
                iRet=DoOutArmXYAlignmentTeachProcess(AOA_Auto5, true);
                if(iRet==1)
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_TEACH_AUTO5;
                    RecordProcess("Alignment Teach OutArm Auto5 Finish");
                    return true;
                }
                else if(iRet==2)
                {
                    Task=1;
                }
                break;
        }
    }
    else if(lAction&OUTARM_ALIGNMENT_TEACH_AUTO6)
    {
        switch(Task)
        {
            case 1:
                if(Sen[SnAutoTrayDetect[eAuto6]].IsOff())                       //Ifor 20210927 add:沒Tray 不掃AOA避免流程卡死
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_TEACH_AUTO6;
                    RecordProcess("Alignment Teach OutArm Auto6 No Tray Pass");
                    return true;
                }
                else
                {
                    RecordProcess("Alignment Teach OutArm Auto6 Start");
                    InitOutArmXYAutoTeachProcess();
                    Task=100;
                }
                break;
            case 100:
                iRet=DoOutArmXYAlignmentTeachProcess(AOA_Auto6, true);
                if(iRet==1)
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_TEACH_AUTO6;
                    RecordProcess("Alignment Teach OutArm Auto6 Finish");
                    return true;
                }
                else if(iRet==2)
                {
                    Task=1;
                }
                break;
        }
    }
    else if(lAction&OUTARM_ALIGNMENT_TEACH_FIX1)
    {
        switch(Task)
        {
            case 1:
                if(TestIF_File.iAutoAlignment_UseFix[iAutoIndex[eFix1]]==true)
                {
                    RecordProcess("Alignment Teach OutArm Fix1 Start");

                    InitOutArmXYAutoTeachProcess();
                    Task=100;
                }
                else
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_TEACH_FIX1;
                    RecordProcess("Alignment Teach OutArm Fix1 Do not use");
                    return true;
                }
                break;
            case 100:
                iRet=DoOutArmXYAlignmentTeachProcess(AOA_Fix1, true);
                if(iRet==1)
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_TEACH_FIX1;
                    RecordProcess("Alignment Teach OutArm Fix1 Finish");
                    return true;
                }
                else if(iRet==2)
                {
                    Task=1;
                }
                break;
        }
    }
    else if(lAction&OUTARM_ALIGNMENT_TEACH_FIX2)
    {
        switch(Task)
        {
            case 1:
                if(TestIF_File.iAutoAlignment_UseFix[iAutoIndex[eFix2]]==true)
                {
                    RecordProcess("Alignment Teach OutArm Fix2 Start");
                    InitOutArmXYAutoTeachProcess();
                    Task=100;
                }
                else
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_TEACH_FIX2;
                    RecordProcess("Alignment Teach OutArm Fix2 Do not use");
                    return true;
                }
                break;
            case 100:
                iRet=DoOutArmXYAlignmentTeachProcess(AOA_Fix2, true);
                if(iRet==1)
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_TEACH_FIX2;
                    RecordProcess("Alignment Teach OutArm Fix2 Finish");
                    return true;
                }
                else if(iRet==2)
                {
                    Task=1;
                }
                break;
        }
    }
    else if(lAction&OUTARM_ALIGNMENT_TEACH_FIX3)
    {
        switch(Task)
        {
            case 1:
                if(TestIF_File.iAutoAlignment_UseFix[iAutoIndex[eFix3]]==true)
                {
                    RecordProcess("Alignment Teach OutArm Fix3 Start");
                    InitOutArmXYAutoTeachProcess();
                    if(EnableFix3UseCylinder())
                    {
                        InitialFix3CanFullTask();
                        Task=10;
                    }
                    else
                    {
                        Task=100;
                    }
                }
                else
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_TEACH_FIX3;
                    RecordProcess("Alignment Teach OutArm Fix3 Do not use");
                    return true;
                }
                break;
            case 10:                                                            //Ifor 20210717 add: Usee Fix3 Full Need Check Cy
                if(UseFix3Cylinder(5)==true)
                {
                    Task=100;
                }
                break;
            case 100:
                iRet=DoOutArmXYAlignmentTeachProcess(AOA_Fix3, true);
                if(iRet==1)
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_TEACH_FIX3;
                    RecordProcess("Alignment Teach OutArm Fix3 Finish");
                    return true;
                }
                else if(iRet==2)
                {
                    Task=1;
                }
                break;
        }
    }
    else if(lAction&OUTARM_ALIGNMENT_TEACH_FIX4)
    {
        switch(Task)
        {
            case 1:
                if(TestIF_File.iAutoAlignment_UseFix[iAutoIndex[eFix4]]==true)
                {
                    RecordProcess("Alignment Teach OutArm Fix4 Start");

                    InitOutArmXYAutoTeachProcess();
                    Task=100;
                }
                else
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_TEACH_FIX4;
                    RecordProcess("Alignment Teach OutArm Fix4 Do not use");
                    return true;
                }
                break;
            case 100:
                iRet=DoOutArmXYAlignmentTeachProcess(AOA_Fix4, true);
                if(iRet==1)
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_TEACH_FIX4;
                    RecordProcess("Alignment Teach OutArm Fix4 Finish");
                    return true;
                }
                else if(iRet==2)
                {
                    Task=1;
                }
                break;
        }
    }
    else if(lAction&OUTARM_ALIGNMENT_TEACH_FIX5)
    {
        switch(Task)
        {
            case 1:
                if(TestIF_File.iAutoAlignment_UseFix[iAutoIndex[eFix5]]==true)
                {
                    RecordProcess("Alignment Teach OutArm Fix5 Start");

                    InitOutArmXYAutoTeachProcess();
                    Task=100;
                }
                else
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_TEACH_FIX5;
                    RecordProcess("Alignment Teach OutArm Fix5 Do not use");
                    return true;
                }
                break;
            case 100:
                iRet=DoOutArmXYAlignmentTeachProcess(AOA_Fix5, true);
                if(iRet==1)
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_TEACH_FIX5;
                    RecordProcess("Alignment Teach OutArm Fix5 Finish");
                    return true;
                }
                else if(iRet==2)
                {
                    Task=1;
                }
                break;
        }
    }
    else if(lAction&OUTARM_ALIGNMENT_TEACH_FIX6)
    {
        switch(Task)
        {
            case 1:
                if(TestIF_File.iAutoAlignment_UseFix[iAutoIndex[eFix6]]==true)
                {
                    RecordProcess("Alignment Teach OutArm Fix6 Start");

                    InitOutArmXYAutoTeachProcess();
                    Task=100;
                }
                else
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_TEACH_FIX6;
                    RecordProcess("Alignment Teach OutArm Fix6 Do not use");
                    return true;
                }
                break;
            case 100:
                iRet=DoOutArmXYAlignmentTeachProcess(AOA_Fix6, true);
                if(iRet==1)
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_TEACH_FIX6;
                    RecordProcess("Alignment Teach OutArm Fix6 Finish");
                    return true;
                }
                else if(iRet==2)
                {
                    Task=1;
                }
                break;
        }
    }
    else if(lAction&OUTARM_ALIGNMENT_TEACH_SHUTTLE1)
    {
        switch(Task)
        {
            case 1:
                if(bRunInArmAutoAlignment==true && (lInArmAutoAlignmentFlag&INARM_ALIGNMENT_TEACH_SHUTTLE1))    //Ifor 20210804 add:需等Inshuttle 做完    //Ifor 20210927 Mark:InShuttle 跑完直接跑OutShuttle
                {
                }
                else
                {
                    RecordProcess("Alignment Teach OutArm Shuttle1 Start");
                    InitOutArmXYAutoTeachProcess();
                    if(EnableFix3UseCylinder())
                    {
                        InitialFix3CanFullTask();
                        Task=10;
                    }
                    else
                    {
                        Task=100;
                    }
                }
                break;
            case 10:    //Ifor 20210717 add: Usee Fix3 Full Need Check Cy
                if(UseFix3Cylinder(0)==true)
                {
                    Task=100;
                }
                break;
            case 100:
                iRet=DoOutArmXYAlignmentTeachProcess(AOA_OutSht1, true);
                if(iRet==1)
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_TEACH_SHUTTLE1;
                    RecordProcess("Alignment Teach OutArm Shuttle1 Finish");
                    return true;
                }
                else if(iRet==2)
                {
                    Task=1;
                }
                break;
        }
    }
    else if(lAction&OUTARM_ALIGNMENT_TEACH_SHUTTLE2)   //0xa
    {
        switch(Task)
        {
            case 1:
                if(bRunInArmAutoAlignment==true && (lInArmAutoAlignmentFlag&INARM_ALIGNMENT_TEACH_SHUTTLE2))    //Ifor 20210804 add:需等Inshuttle 做完    //Ifor 20210927 Mark:InShuttle 跑完直接跑OutShuttle
                {
                }
                else
                {
                    RecordProcess("Alignment Teach OutArm Shuttle2 Start");
                    InitOutArmXYAutoTeachProcess();
                    if(EnableFix3UseCylinder())
                    {
                        InitialFix3CanFullTask();
                        Task=10;
                    }
                    else
                    {
                        Task=100;
                    }
                }
                break;
            case 10:    //Ifor 20210717 add: Usee Fix3 Full Need Check Cy
                if(UseFix3Cylinder(0)==true)
                {
                    Task=100;
                }
                break;
            case 100:
                iRet=DoOutArmXYAlignmentTeachProcess(AOA_OutSht2, true);
                if(iRet==1)
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_TEACH_SHUTTLE2;
                    RecordProcess("Alignment Teach OutArm Shuttle2 Finish");
                    return true;
                }
                else if(iRet==2)
                {
                    Task=2;
                }
                break;
        }
    }
    else if(lAction&OUTARM_ALIGNMENT_AUTO_UNIT)        //0xb
    {
        switch(Task)
        {
            case 1:
                RecordProcess("Alignment Auto OutArm Unit Start");
                InitOutArmCCDAlignmentTeachUnitProcess();
                Task=100;
                break;
            case 100:
                iRet=DoOutArmCCDAlignmentTeachUnitProces(ResolutionX, ResolutionY, dRadianX, dRadianY, 0);
                if(iRet==1)        //0: Correction mm/pixel 1: Teach Pattern 2: Teach Pattern and Correction mm/pixel
                {
                    INIFile=new TIniFile(sPath);
                    INIFile->WriteFloat("InArm", "dInArmCCDXResolution", ResolutionX);         //ChungHung 20210113 add for Alignment CCD
                    INIFile->WriteFloat("InArm", "dInArmCCDYResolution", ResolutionY);         //ChungHung 20210113 add for Alignment CCD
                    INIFile->WriteFloat("InArm", "dInArmCCDXRadian", dRadianX);         //ChungHung 20210113 add for Alignment CCD
                    INIFile->WriteFloat("InArm", "dInArmCCDYRadian", dRadianY);         //ChungHung 20210113 add for Alignment CCD
                    delete INIFile;

                    Task=1;
                    lAction=lAction^INARM_ALIGNMENT_AUTO_UNIT;
                    RecordProcess("Alignment Auto OutArm Unit Finish");
                    return true;
                }
                else if(iRet==2)
                {
                    Task=1;
                }
                break;
        }
    }
    else if(lAction&OUTARM_ALIGNMENT_AUTO_AUTO1)      //0xd
    {
        switch(Task)
        {
            case 1:
                if(Sen[SnAutoTrayDetect[eAuto1]].IsOff())                       //Ifor 20210927 add:沒Tray 不掃AOA避免流程卡死
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_AUTO_AUTO1;
                    bAlignmentChangeUnloadTray[eAuto1]=false;
                    RecordProcess("Alignment Auto OutArm Auto1 No Tray Pass");
                    return true;
                }
                else
                {
                    RecordProcess("Alignment Auto OutArm Auto1 Start");
                    InitOutArmXYAutoTeachProcess();
                    Task=100;
                }
                break;
            case 100:
                iRet=DoOutArmXYAlignmentTeachProcess(AOA_Auto1, false);
                if(iRet==1)
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_AUTO_AUTO1;
                    bAlignmentChangeUnloadTray[eAuto1]=false;
                    RecordProcess("Alignment Auto OutArm Auto1 Finish");
                    return true;
                }
                break;
        }
    }
    else if(lAction&OUTARM_ALIGNMENT_AUTO_AUTO2)   //0xe
    {
        switch(Task)
        {
            case 1:
                if(Sen[SnAutoTrayDetect[eAuto2]].IsOff())                       //Ifor 20210927 add:沒Tray 不掃AOA避免流程卡死
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_AUTO_AUTO2;
                    bAlignmentChangeUnloadTray[eAuto2]=false;
                    RecordProcess("Alignment Auto OutArm Auto2 No Tray Pass");
                    return true;
                }
                else
                {
                    RecordProcess("Alignment Auto OutArm Auto2 Start");
                    InitOutArmXYAutoTeachProcess();
                    Task=100;
                }
                break;
            case 100:
                iRet=DoOutArmXYAlignmentTeachProcess(AOA_Auto2, false);
                if(iRet==1)
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_AUTO_AUTO2;
                    bAlignmentChangeUnloadTray[eAuto2]=false;
                    RecordProcess("Alignment Auto OutArm Auto2 Finish");
                    return true;
                }
                break;
        }
    }
    else if(lAction&OUTARM_ALIGNMENT_AUTO_AUTO3)   //0xf
    {
        switch(Task)
        {
            case 1:
                if(Sen[SnAutoTrayDetect[eAuto3]].IsOff())                       //Ifor 20210927 add:沒Tray 不掃AOA避免流程卡死
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_AUTO_AUTO3;
                    bAlignmentChangeUnloadTray[eAuto3]=false;
                    RecordProcess("Alignment Auto OutArm Auto3 No Tray Pass");
                    return true;
                }
                else
                {
                    RecordProcess("Alignment Auto OutArm Auto3 Start");
                    InitOutArmXYAutoTeachProcess();
                    Task=100;
                }
                break;
            case 100:
                iRet=DoOutArmXYAlignmentTeachProcess(AOA_Auto3, false);
                if(iRet==1)
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_AUTO_AUTO3;
                    bAlignmentChangeUnloadTray[eAuto3]=false;
                    RecordProcess("Alignment Auto OutArm Auto3 Finish");
                    return true;
                }
                break;
        }
    }
    else if(lAction&OUTARM_ALIGNMENT_AUTO_AUTO4)                                //Steven 20240428 : Add for HT9011 AOA
    {
        switch(Task)
        {
            case 1:
                if(Sen[SnAutoTrayDetect[eAuto4]].IsOff())                       //Ifor 20210927 add:沒Tray 不掃AOA避免流程卡死
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_AUTO_AUTO4;
                    bAlignmentChangeUnloadTray[eAuto4]=false;
                    RecordProcess("Alignment Auto OutArm Auto4 No Tray Pass");
                    return true;
                }
                else
                {
                    RecordProcess("Alignment Auto OutArm Auto4 Start");
                    InitOutArmXYAutoTeachProcess();
                    Task=100;
                }
                break;
            case 100:
                iRet=DoOutArmXYAlignmentTeachProcess(AOA_Auto4, false);
                if(iRet==1)
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_AUTO_AUTO4;
                    bAlignmentChangeUnloadTray[eAuto4]=false;
                    RecordProcess("Alignment Auto OutArm Auto4 Finish");
                    return true;
                }
                break;
        }
    }
    else if(lAction&OUTARM_ALIGNMENT_AUTO_AUTO5)
    {
        switch(Task)
        {
            case 1:
                if(Sen[SnAutoTrayDetect[eAuto5]].IsOff())                       //Ifor 20210927 add:沒Tray 不掃AOA避免流程卡死
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_AUTO_AUTO5;
                    bAlignmentChangeUnloadTray[eAuto5]=false;
                    RecordProcess("Alignment Auto OutArm Auto5 No Tray Pass");
                    return true;
                }
                else
                {
                    RecordProcess("Alignment Auto OutArm Auto5 Start");
                    InitOutArmXYAutoTeachProcess();
                    Task=100;
                }
                break;
            case 100:
                iRet=DoOutArmXYAlignmentTeachProcess(AOA_Auto5, false);
                if(iRet==1)
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_AUTO_AUTO5;
                    bAlignmentChangeUnloadTray[eAuto5]=false;
                    RecordProcess("Alignment Auto OutArm Auto5 Finish");
                    return true;
                }
                break;
        }
    }
    else if(lAction&OUTARM_ALIGNMENT_AUTO_AUTO6)
    {
        switch(Task)
        {
            case 1:
                if(Sen[SnAutoTrayDetect[eAuto6]].IsOff())                       //Ifor 20210927 add:沒Tray 不掃AOA避免流程卡死
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_AUTO_AUTO6;
                    bAlignmentChangeUnloadTray[eAuto6]=false;
                    RecordProcess("Alignment Auto OutArm Auto5 No Tray Pass");
                    return true;
                }
                else
                {
                    RecordProcess("Alignment Auto OutArm Auto6 Start");
                    InitOutArmXYAutoTeachProcess();
                    Task=100;
                }
                break;
            case 100:
                iRet=DoOutArmXYAlignmentTeachProcess(AOA_Auto6, false);
                if(iRet==1)
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_AUTO_AUTO6;
                    bAlignmentChangeUnloadTray[eAuto6]=false;
                    RecordProcess("Alignment Auto OutArm Auto6 Finish");
                    return true;
                }
                break;
        }
    }
    else if(lAction&OUTARM_ALIGNMENT_AUTO_FIX1)
    {
        switch(Task)
        {
            case 1:
                if(TestIF_File.iAutoAlignment_UseFix[iAutoIndex[eFix1]]==true)
                {
                    RecordProcess("Alignment Auto OutArm Fix1 Start");
                    InitOutArmXYAutoTeachProcess();
                    Task=100;
                }
                else
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_AUTO_FIX1;
                    bAlignmentChangeUnloadTray[eFix1]=false;
                    RecordProcess("Alignment Auto OutArm Fix1 Do not use");
                    return true;
                }
                break;
            case 100:
                iRet=DoOutArmXYAlignmentTeachProcess(AOA_Fix1, false);
                if(iRet==1)
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_AUTO_FIX1;
                    bAlignmentChangeUnloadTray[eFix1]=false;
                    RecordProcess("Alignment Auto OutArm Fix1 Finish");
                    return true;
                }
                break;
        }
    }
    else if(lAction&OUTARM_ALIGNMENT_AUTO_FIX2)
    {
        switch(Task)
        {
            case 1:
                if(TestIF_File.iAutoAlignment_UseFix[iAutoIndex[eFix2]]==true)
                {
                    RecordProcess("Alignment Auto OutArm Fix2 Start");
                    InitOutArmXYAutoTeachProcess();
                    Task=100;
                }
                else
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_AUTO_FIX2;
                    bAlignmentChangeUnloadTray[eFix2]=false;
                    RecordProcess("Alignment Auto OutArm Fix2 Do not use");
                    return true;
                }
                break;
            case 100:
                iRet=DoOutArmXYAlignmentTeachProcess(AOA_Fix2, false);
                if(iRet==1)
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_AUTO_FIX2;
                    bAlignmentChangeUnloadTray[eFix2]=false;
                    RecordProcess("Alignment Auto OutArm Fix2 Finish");
                    return true;
                }
                break;
        }
    }
    else if(lAction&OUTARM_ALIGNMENT_AUTO_FIX3)
    {
        switch(Task)
        {
            case 1:
                if(TestIF_File.iAutoAlignment_UseFix[iAutoIndex[eFix3]]==true)
                {
                    RecordProcess("Alignment Auto OutArm Fix3 Start");
                    InitOutArmXYAutoTeachProcess();
                    if(EnableFix3UseCylinder())
                    {
                        InitialFix3CanFullTask();
                        Task=10;
                    }
                    else
                    {
                        Task=100;
                    }
                }
                else
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_AUTO_FIX3;
                    bAlignmentChangeUnloadTray[eFix3]=false;
                    RecordProcess("Alignment Auto OutArm Fix3 Do not use");
                    return true;
                }
                break;
            case 10:    //Ifor 20210717 add: Usee Fix3 Full Need Check Cy
                if(UseFix3Cylinder(5)==true)
                {
                    Task=100;
                }
                break;
            case 100:
                iRet=DoOutArmXYAlignmentTeachProcess(AOA_Fix3, false);
                if(iRet==1)
                {
                    if(EnableFix3UseCylinder())    //Ifor 20210719 add:避免不跑Shuttle導致FIX3未歸位
                    {
                        InitialFix3CanFullTask();
                        Task=110;
                    }
                    else
                    {
                        Task=1;
                        lAction=lAction^OUTARM_ALIGNMENT_AUTO_FIX3;
                        bAlignmentChangeUnloadTray[eFix3]=false;
                        RecordProcess("Alignment Auto OutArm Fix3 Finish");
                        return true;
                    }
                }
                break;
            case 110:    //Ifor 20210717 add: Usee Fix3 Full Need Check Cy
                if(UseFix3Cylinder(0)==true)
                {
                    if(MOT[MInShuttle1].IsCanMove()==false && MOT[MInShuttle2].IsCanMove()==false)
                    {
                        ShowMyMessage("Alignment IndexZ not in Safe Postion!" );
                    }
                    else
                    {
                        Task=1;
                        lAction=lAction^OUTARM_ALIGNMENT_AUTO_FIX3;
                        bAlignmentChangeUnloadTray[eFix3]=false;
                        RecordProcess("Alignment Auto OutArm Fix3 Finish");
                        return true;
                    }
                }
                break;
        }
    }
    else if(lAction&OUTARM_ALIGNMENT_AUTO_FIX4)                                 //Steven 20240428 : Add for HT9011 AOA
    {
        switch(Task)
        {
            case 1:
                if(TestIF_File.iAutoAlignment_UseFix[iAutoIndex[eFix4]]==true)
                {
                    RecordProcess("Alignment Auto OutArm Fix4 Start");
                    InitOutArmXYAutoTeachProcess();
                    Task=100;
                }
                else
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_AUTO_FIX4;
                    bAlignmentChangeUnloadTray[eFix4]=false;
                    RecordProcess("Alignment Auto OutArm Fix4 Do not use");
                    return true;
                }
                break;
            case 100:
                iRet=DoOutArmXYAlignmentTeachProcess(AOA_Fix4, false);
                if(iRet==1)
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_AUTO_FIX4;
                    bAlignmentChangeUnloadTray[eFix4]=false;
                    RecordProcess("Alignment Auto OutArm Fix4 Finish");
                    return true;
                }
                break;
        }
    }
    else if(lAction&OUTARM_ALIGNMENT_AUTO_FIX5)
    {
        switch(Task)
        {
            case 1:
                if(TestIF_File.iAutoAlignment_UseFix[iAutoIndex[eFix5]]==true)
                {
                    RecordProcess("Alignment Auto OutArm Fix5 Start");
                    InitOutArmXYAutoTeachProcess();
                    Task=100;
                }
                else
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_AUTO_FIX5;
                    bAlignmentChangeUnloadTray[eFix5]=false;
                    RecordProcess("Alignment Auto OutArm Fix5 Do not use");
                    return true;
                }
                break;
            case 100:
                iRet=DoOutArmXYAlignmentTeachProcess(AOA_Fix5, false);
                if(iRet==1)
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_AUTO_FIX5;
                    bAlignmentChangeUnloadTray[eFix5]=false;
                    RecordProcess("Alignment Auto OutArm Fix5 Finish");
                    return true;
                }
                break;
        }
    }
    else if(lAction&OUTARM_ALIGNMENT_AUTO_FIX6)
    {
        switch(Task)
        {
            case 1:
                if(TestIF_File.iAutoAlignment_UseFix[iAutoIndex[eFix6]]==true)
                {
                    RecordProcess("Alignment Auto OutArm Fix6 Start");
                    InitOutArmXYAutoTeachProcess();
                    Task=100;
                }
                else
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_AUTO_FIX6;
                    bAlignmentChangeUnloadTray[eFix6]=false;
                    RecordProcess("Alignment Auto OutArm Fix6 Do not use");
                    return true;
                }
                break;
            case 100:
                iRet=DoOutArmXYAlignmentTeachProcess(AOA_Fix6, false);
                if(iRet==1)
                {
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_AUTO_FIX6;
                    bAlignmentChangeUnloadTray[eFix6]=false;
                    RecordProcess("Alignment Auto OutArm Fix6 Finish");
                    return true;
                }
                break;
        }
    }
    else if(lAction&OUTARM_ALIGNMENT_AUTO_SHUTTLE1)    //0x13
    {
        bool bCheckXOnly=false;       //KenHsieh 20211002 : Modify Shuttle Scan
        bool bCheckYOnly=false;       //KenHsieh 20211002 : Modify Shuttle Scan

        switch(Task)
        {
            case 1:
                if(bRunInArmAutoAlignment==true && (lInArmAutoAlignmentFlag&INARM_ALIGNMENT_AUTO_SHUTTLE1))    //Ifor 20210804 add:需等Inshuttle 做完 //Ifor 20210927 Mark:InShuttle 跑完直接跑OutShuttle
                {
                }
                else
                {
                    if(EnableFix3UseCylinder())
                    {
                        InitialFix3CanFullTask();
                        Task=10;
                    }
                    else
                    {
                        if(MOT[MInShuttle1].IsCanMove()==false && MOT[MInShuttle2].IsCanMove()==false)
                        {
                            ShowMyMessage("Alignment IndexZ not in Safe Postion!");
                        }
                        else
                        {
                            Task=25;
                        }
                    }
                }
                break;
            case 10:    //Ifor 20210717 add: Usee Fix3 Full Need Check Cy
                if(UseFix3Cylinder(0)==true)
                {
                    if(MOT[MInShuttle1].IsCanMove()==false && MOT[MInShuttle2].IsCanMove()==false)
                    {
                        ShowMyMessage("Alignment IndexZ not in Safe Postion!");
                    }
                    else
                    {
                        Task=25;
                    }
                }
                break;
            case 25:
                if(bAutoAlignmentInArmShuttle1Lock==true)
                {
                    DoMoveAlignmentToOutShuttleStage(0, 0, 0, iPosX, iPosY, true, bCheckXOnly, bCheckYOnly);     //KenHsieh 20211002 : Modify Shuttle Scan
                    return false;
                }
                // 2011.06.07 , Joye , Shuttle X-Axis Detect --------------------->>
                if(MOT[MInShuttle1].MotorMove(Prod.InSHT[0].iRight))
                {
                    bAutoAlignmentOutArmShuttle1Lock = true;
                    Task=50;
                }
                break;
            case 50:
                RecordProcess("Alignment Auto OutArm Shuttle1 Start");
                InitOutArmXYAutoTeachProcess();
                Task=100;
                break;
            case 100:
                iRet=DoOutArmXYAlignmentTeachProcess(AOA_OutSht1, false);
                if(iRet==1)  //iArea 0=auto1 1=auto2 2=auto3 3:fix1 4:fix2 5:fix3 6:shuttle1 7:shuttle2
                {
                    Task=200;
                }
                break;
            case 200:
                // 2011.06.07 , Joye , Shuttle X-Axis Detect --------------------->>
                if(MOT[MInShuttle1].MotorMove(Prod.InSHT[0].iLeft))
                {
                    bAutoAlignmentOutArmShuttle1Lock = false;
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_AUTO_SHUTTLE1;
                    RecordProcess("Alignment Auto OutArm Shuttle1 Finish");
                    return true;
                }
                break;
        }
    }
    else if(lAction&OUTARM_ALIGNMENT_AUTO_SHUTTLE2)    //0x14
    {
        bool bCheckXOnly=false;       //KenHsieh 20211002 : Modify Shuttle Scan
        bool bCheckYOnly=false;       //KenHsieh 20211002 : Modify Shuttle Scan

        switch(Task)
        {
            case 1:
                if(bRunInArmAutoAlignment==true && (lInArmAutoAlignmentFlag&INARM_ALIGNMENT_AUTO_SHUTTLE2))    //Ifor 20210804 add:需等Inshuttle 做完 //Ifor 20210927 Mark:InShuttle 跑完直接跑OutShuttle
                {
                }
                else
                {
                    if(EnableFix3UseCylinder())
                    {
                        InitialFix3CanFullTask();
                        Task=10;
                    }
                    else
                    {
                        if(MOT[MInShuttle1].IsCanMove()==false && MOT[MInShuttle2].IsCanMove()==false)
                        {
                            ShowMyMessage("Alignment IndexZ not in Safe Postion!");
                        }
                        else
                        {
                            Task=25;
                        }
                    }
                }
                break;
            case 10:    //Ifor 20210717 add: Usee Fix3 Full Need Check Cy
                if(UseFix3Cylinder(0)==true)
                {
                    if(MOT[MInShuttle1].IsCanMove()==false && MOT[MInShuttle2].IsCanMove()==false)
                    {
                        ShowMyMessage("Alignment IndexZ not in Safe Postion!");
                    }
                    else
                    {
                        Task=25;
                    }
                }
                break;
            case 25:
                if(bAutoAlignmentInArmShuttle2Lock==true)
                {
                    DoMoveAlignmentToOutShuttleStage(1, 0, 0, iPosX, iPosY, true, bCheckXOnly, bCheckYOnly);     //KenHsieh 20211002 : Modify Shuttle Scan
                    return false;
                }
                // 2011.06.07 , Joye , Shuttle X-Axis Detect --------------------->>
                if(MOT[MInShuttle2].MotorMove(Prod.InSHT[1].iRight))
                {
                    bAutoAlignmentOutArmShuttle2Lock=true;
                    Task=50;
                }
                break;
            case 50:
                RecordProcess("Alignment Auto OutArm Shuttle2 Start");
                InitOutArmXYAutoTeachProcess();
                Task=100;
                break;
            case 100:
                iRet=DoOutArmXYAlignmentTeachProcess(AOA_OutSht2, false);
                if(iRet==1)  //iArea 0=auto1 1=auto2 2=auto3 3:fix1 4:fix2 5:fix3 6:shuttle1 7:shuttle2
                {
                    Task=200;
                }
                break;
            case 200:
                // 2011.06.07 , Joye , Shuttle X-Axis Detect --------------------->>
                if(MOT[MInShuttle2].MotorMove(Prod.InSHT[1].iLeft))
                {
                    bAutoAlignmentOutArmShuttle2Lock=false;
                    Task=1;
                    lAction=lAction^OUTARM_ALIGNMENT_AUTO_SHUTTLE2;
                    RecordProcess("Alignment Auto OutArm Shuttle2 Finish");
                    return true;
                }
                break;
        }
    }
    return false;
}
//------------------------------------------------------------------------------
int AutoAlignmentOutArmSave_SafePosZ[8]={0, 0, 0, 0, 0, 0, 0, 0};
int AutoAlignmentOutArmSave_SafePosX=0;
int AutoAlignmentOutArmSave_SafePosY=0;

int AutoAlignmentOutArmMoveType=1;
//------------------------------------------------------------------------------
void ResetMoveAutoAlignmentOutArmMoveSavePositionTask()
{
    for(int i=0; i<8; i++)
        AutoAlignmentOutArmSave_SafePosZ[i]=0;
    AutoAlignmentOutArmSave_SafePosX=0;
    AutoAlignmentOutArmSave_SafePosY=0;

    bAutoAlignmentOutArmSavePosition=false;
}
//------------------------------------------------------------------------------
void InitialMoveAutoAlignmentOutArmMoveSavePositionTask()
{
    AutoAlignmentOutArmMoveType=1;
}
//------------------------------------------------------------------------------
bool WhenMoveAlignmentOutArmMoveSavePosition(bool MovePos)
{
    //2012.05.21 Q_Q TrayArm Drop Tray will Move Two Times
    int &Task=AutoAlignmentOutArmMoveType;
    bool bflagX   =false;
    bool bflagY   =false;
    bool bflagZ[8]={false, false, false, false, false, false, false, false};

    if(MovePos==true)
    {
        switch(Task)
        {
            case 1:
                if(AutoAlignmentOutArmSave_SafePosZ[0]==0 && AutoAlignmentOutArmSave_SafePosZ[1]==0 && AutoAlignmentOutArmSave_SafePosZ[2]==0 && AutoAlignmentOutArmSave_SafePosZ[3]==0 &&
                   AutoAlignmentOutArmSave_SafePosZ[4]==0 && AutoAlignmentOutArmSave_SafePosZ[5]==0 && AutoAlignmentOutArmSave_SafePosZ[6]==0 && AutoAlignmentOutArmSave_SafePosZ[7]==0 &&
                   AutoAlignmentOutArmSave_SafePosX   ==0 && AutoAlignmentOutArmSave_SafePosY   ==0)
                {
                    AutoAlignmentOutArmSave_SafePosZ[0]=MOT[MOutArmZA].ReadPos();
                    AutoAlignmentOutArmSave_SafePosZ[1]=MOT[MOutArmZB].ReadPos();
                    AutoAlignmentOutArmSave_SafePosZ[2]=MOT[MOutArmZC].ReadPos();
                    AutoAlignmentOutArmSave_SafePosZ[3]=MOT[MOutArmZD].ReadPos();
                    AutoAlignmentOutArmSave_SafePosZ[4]=MOT[MOutArmZE].ReadPos();
                    AutoAlignmentOutArmSave_SafePosZ[5]=MOT[MOutArmZF].ReadPos();
                    AutoAlignmentOutArmSave_SafePosZ[6]=MOT[MOutArmZG].ReadPos();
                    AutoAlignmentOutArmSave_SafePosZ[7]=MOT[MOutArmZH].ReadPos();
                    AutoAlignmentOutArmSave_SafePosX   =MOT[MOutArmX].ReadPos();
                    AutoAlignmentOutArmSave_SafePosY   =MOT[MOutArmY].ReadPos();
                }
                Task=2;
                break;
            case 2:
                if(MoveOutArmToAutoSafe())
                {
                    bAutoAlignmentOutArmSavePosition=true;
                    Task=1;
                    return true;
                }
                break;
        }
    }
    else
    {
        if(bAutoAlignmentOutArmSavePosition)
        {
            switch(Task)
            {
                case 1:
                    bflagX=MOT[MOutArmX].MotorMove(AutoAlignmentOutArmSave_SafePosX);
                    bflagY=MOT[MOutArmY].MotorMove(AutoAlignmentOutArmSave_SafePosY);

                    if(bflagX && bflagY)
                    {
                        Task=15;
                    }
                    break;
                case 15:
                    bflagZ[0]=MOT[MOutArmZA].MotorMove(AutoAlignmentOutArmSave_SafePosZ[0]);
                    bflagZ[1]=MOT[MOutArmZB].MotorMove(AutoAlignmentOutArmSave_SafePosZ[1]);
                    bflagZ[2]=MOT[MOutArmZC].MotorMove(AutoAlignmentOutArmSave_SafePosZ[2]);
                    bflagZ[3]=MOT[MOutArmZD].MotorMove(AutoAlignmentOutArmSave_SafePosZ[3]);
                    bflagZ[4]=MOT[MOutArmZE].MotorMove(AutoAlignmentOutArmSave_SafePosZ[4]);
                    bflagZ[5]=MOT[MOutArmZF].MotorMove(AutoAlignmentOutArmSave_SafePosZ[5]);
                    bflagZ[6]=MOT[MOutArmZG].MotorMove(AutoAlignmentOutArmSave_SafePosZ[6]);
                    bflagZ[7]=MOT[MOutArmZH].MotorMove(AutoAlignmentOutArmSave_SafePosZ[7]);

                    if(bflagZ[0] && bflagZ[1] && bflagZ[2] && bflagZ[3] && bflagZ[4] && bflagZ[5] && bflagZ[6] && bflagZ[7])
                    {
                        Task=20;
                    }
                    break;
                case 20:
                    bAutoAlignmentOutArmSavePosition=false;
                    Task=999;
                    return true;
            }
        }
    }
    return false;
}
//------------------------------------------------------------------------------
int iDoOutArmTeachAlignmentProcessTask=1;
//------------------------------------------------------------------------------
void InitDoOutArmTeachAlignmentProcessTask()
{
    bAutoAlignmentOutArmShuttle1Lock=false;
    bAutoAlignmentOutArmShuttle2Lock=false;
    iDoOutArmTeachAlignmentProcessTask=1;
    iCCDAlignmentMotorDelay=CheckAndReadIniDataGeneral("MachineDefine", "MACHINE_HAS_AUTO_ALIGNMENT_CCD_DELAY", 200);     //KenHsieh 20210830 : add CCD AUTO ALIGNMENT
}
//------------------------------------------------------------------------------
bool DoOutArmTeachAlignmentProcess(unsigned long &lAction)
{
    int &Task=iDoOutArmTeachAlignmentProcessTask;
    switch(Task)
    {
        case 1:
            fMain->mtOutArmAutoAlignmentTray->Visible=true;                     //KenHsieh 20211215 : 修改AOA掃描之顯示Tray，不掃描便不顯示
            ResetMoveAutoAlignmentOutArmMoveSavePositionTask();
            InitialMoveAutoAlignmentOutArmMoveSavePositionTask();
            Task=100;
            break;
        case 100:
            if(WhenMoveAlignmentOutArmMoveSavePosition(true))
            {
                InitOutArmAlignment();
                Task=200;
            }
            break;
        case 200:
            DoOutArmTeachAlignment(lAction);
            if(lOutArmAutoAlignmentFlag==0x0)
            {
                InitialMoveAutoAlignmentOutArmMoveSavePositionTask();
                Task=300;
            }
            break;
        case 300:
            if(WhenMoveAlignmentOutArmMoveSavePosition(false))
            {
                Task=400;
            }
            break;
        case 400:
            fMain->mtOutArmAutoAlignmentTray->Visible=false;                    //KenHsieh 20211215 : 修改AOA掃描之顯示Tray，不掃描便不顯示
            return true;
    }
    return false;
}
//------------------------------------------------------------------------------
//ChungHung 20210113 add for Alignment CCD end
//<==
bool bAutoAlignmentCheckHasTray(int iArea)                                      //KenHsieh 20211005 : 檢查是否有Tray
{
    if(iArea==AOA_Auto1)
    {
        if(Sen[SnAutoTrayDetect[eAuto1]].IsOff())
        {
            ShowMyMessage("Please Check Auto1 Whether Has Tray!!");
            return false;
        }
    }
    else if(iArea==AOA_Auto2)
    {
        if(Sen[SnAutoTrayDetect[eAuto2]].IsOff())
        {
            ShowMyMessage("Please Check Auto2 Whether Has Tray!!");
            return false;
        }
    }
    else if(iArea==AOA_Auto3)
    {
        if(Sen[SnAutoTrayDetect[eAuto3]].IsOff())
        {
            ShowMyMessage("Please Check Auto3 Whether Has Tray!!");
            return false;
        }
    }
    else if(iArea==AOA_Auto4)                                                   //Steven 20240428 : Add for HT9011 AOA
    {
        if(Sen[SnAutoTrayDetect[eAuto4]].IsOff())
        {
            ShowMyMessage("Please Check Auto4 Whether Has Tray!!");
            return false;
        }
    }
    else if(iArea==AOA_Auto5)
    {
        if(Sen[SnAutoTrayDetect[eAuto5]].IsOff())
        {
            ShowMyMessage("Please Check Auto5 Whether Has Tray!!");
            return false;
        }
    }
    else if(iArea==AOA_Auto6)
    {
        if(Sen[SnAutoTrayDetect[eAuto6]].IsOff())
        {
            ShowMyMessage("Please Check Auto6 Whether Has Tray!!");
            return false;
        }
    }
    else if(iArea==AOA_Fix1)
    {
        if(Sen[SnFixedTrayDetect[iAutoIndex[eFix1]]].IsOff())
        {
            ShowMyMessage("Please Check Fix1 Whether Has Tray!!");
            return false;
        }
    }
    else if(iArea==AOA_Fix2)
    {
        if(Sen[SnFixedTrayDetect[iAutoIndex[eFix2]]].IsOff())
        {
            ShowMyMessage("Please Check Fix2 Whether Has Tray!!");
            return false;
        }
    }
    else if(iArea==AOA_Fix3)
    {
        if(Sen[SnFixedTrayDetect[iAutoIndex[eFix3]]].IsOff())
        {
            ShowMyMessage("Please Check Fix3 Whether Has Tray!!");
            return false;
        }
    }
    else if(iArea==AOA_Fix4)                                                    //Steven 20240428 : Add for HT9011 AOA
    {
        if(Sen[SnFixedTrayDetect[iAutoIndex[eFix4]]].IsOff())
        {
            ShowMyMessage("Please Check Fix4 Whether Has Tray!!");
            return false;
        }
    }
    else if(iArea==AOA_Fix5)
    {
        if(Sen[SnFixedTrayDetect[iAutoIndex[eFix5]]].IsOff())
        {
            ShowMyMessage("Please Check Fix5 Whether Has Tray!!");
            return false;
        }
    }
    else if(iArea==AOA_Fix6)
    {
        if(Sen[SnFixedTrayDetect[iAutoIndex[eFix6]]].IsOff())
        {
            ShowMyMessage("Please Check Fix6 Whether Has Tray!!");
            return false;
        }
    }
    return true;
}
//------------------------------------------------------------------------------
bool CheckInArmAutoAlignmentTrayModeBeUse(int iMode, bool bSet)                 //KenHsieh 20210813 : add CCD AUTO ALIGNMENT
{
    if(TestIF.iAutoAlignmentTrayEvent&iMode)
    {
        if(bSet)
        {
            lInArmAutoAlignmentFlag=lInArmAutoAlignmentFlag|INARM_ALIGNMENT_AUTO_LOADER;
            lInArmAutoAlignmentTrayTimingFlag=0;
        }
        else
        {
            lInArmAutoAlignmentTrayTimingFlag=lInArmAutoAlignmentTrayTimingFlag|iMode;
            if(bInArmAutoAlignmentClearFlag==false)                             //Kenhsieh 20211007 : 解決沒有掃描Kit問題
            {
                lInArmAutoAlignmentFlag=0;
            }
        }
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
bool CheckInArmAutoAlignmentCKModeBeUse(int iMode, bool bSet)
{
    if(TestIF.iAutoAlignmentShuttleHotplateEvent&iMode)
    {
        if(bSet)
        {
            if(LastSet.iTemperature==Tempture_Hot)                              //溫控、跑HotPlate
            {
                if(HotPlateForm.iPlateSelect==1)
                {
                    lInArmAutoAlignmentFlag=lInArmAutoAlignmentFlag|INARM_ALIGNMENT_AUTO_HOTPLAYE1;
                    lInArmAutoAlignmentFlag=lInArmAutoAlignmentFlag&~INARM_ALIGNMENT_AUTO_HOTPLAYE2;
                }
                else if(HotPlateForm.iPlateSelect==2)
                {
                    lInArmAutoAlignmentFlag=lInArmAutoAlignmentFlag&~INARM_ALIGNMENT_AUTO_HOTPLAYE1;
                    lInArmAutoAlignmentFlag=lInArmAutoAlignmentFlag|INARM_ALIGNMENT_AUTO_HOTPLAYE2;
                }
                else
                {
                    lInArmAutoAlignmentFlag=lInArmAutoAlignmentFlag|INARM_ALIGNMENT_AUTO_HOTPLAYE1;
                    lInArmAutoAlignmentFlag=lInArmAutoAlignmentFlag|INARM_ALIGNMENT_AUTO_HOTPLAYE2;
                }
            }

            if(TestIF.iShuttleMode==1 && TestIF_File.iShuttle_Sel==0)
            {
                lInArmAutoAlignmentFlag=lInArmAutoAlignmentFlag|INARM_ALIGNMENT_AUTO_SHUTTLE1;
                lInArmAutoAlignmentFlag=lInArmAutoAlignmentFlag&~INARM_ALIGNMENT_AUTO_SHUTTLE2;
            }
            else if(TestIF.iShuttleMode==1 && TestIF_File.iShuttle_Sel==1)
            {
                lInArmAutoAlignmentFlag=lInArmAutoAlignmentFlag&~INARM_ALIGNMENT_AUTO_SHUTTLE1;
                lInArmAutoAlignmentFlag=lInArmAutoAlignmentFlag|INARM_ALIGNMENT_AUTO_SHUTTLE2;
            }
            else
            {
                lInArmAutoAlignmentFlag=lInArmAutoAlignmentFlag|INARM_ALIGNMENT_AUTO_SHUTTLE1;
                lInArmAutoAlignmentFlag=lInArmAutoAlignmentFlag|INARM_ALIGNMENT_AUTO_SHUTTLE2;
            }

            if(TestIF.iAutoClean_Function && USE_IN_Y_IS_AUTO_PITCH==true &&          //JerryYang 20251218 : IN/OUT ARM支援不同模組  //KenHsieh 20211214 : AOA add AutoClean
               CUSTOMER_CODE==CC_ASE_KaohSiung)                                 //KenHsieh 20220103 : 新增ASEKH判定
                lInArmAutoAlignmentFlag=lInArmAutoAlignmentFlag|INARM_ALIGNMENT_AUTO_AUTOCLEAN;

            lInArmAutoAlignmentCKTimingFlag=0;
        }
        else
        {
            lInArmAutoAlignmentCKTimingFlag=lInArmAutoAlignmentCKTimingFlag|iMode;
            lInArmAutoAlignmentFlag=0;
        }
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
bool CheckOutArmAutoAlignmentTrayModeBeUse(int iMode, bool bSet)
{
    if(TestIF.iAutoAlignmentTrayEvent & iMode)
    {
        if(bSet)
        {
            if(MOT[MMAuto1].fHasTray && bAlignmentChangeUnloadTray[eAuto1])
                lOutArmAutoAlignmentFlag=lOutArmAutoAlignmentFlag|OUTARM_ALIGNMENT_AUTO_AUTO1;
            else
                lOutArmAutoAlignmentFlag=lOutArmAutoAlignmentFlag&~OUTARM_ALIGNMENT_AUTO_AUTO1;

            if(MOT[MMAuto2].fHasTray && bAlignmentChangeUnloadTray[eAuto2])
                lOutArmAutoAlignmentFlag=lOutArmAutoAlignmentFlag|OUTARM_ALIGNMENT_AUTO_AUTO2;
            else
                lOutArmAutoAlignmentFlag=lOutArmAutoAlignmentFlag&~OUTARM_ALIGNMENT_AUTO_AUTO2;

            if(MOT[MMAuto3].fHasTray && bAlignmentChangeUnloadTray[eAuto3])
                lOutArmAutoAlignmentFlag=lOutArmAutoAlignmentFlag|OUTARM_ALIGNMENT_AUTO_AUTO3;
            else
                lOutArmAutoAlignmentFlag=lOutArmAutoAlignmentFlag&~OUTARM_ALIGNMENT_AUTO_AUTO3;

            if(MOT[MMAuto4].fHasTray && bAlignmentChangeUnloadTray[eAuto4])
                lOutArmAutoAlignmentFlag=lOutArmAutoAlignmentFlag|OUTARM_ALIGNMENT_AUTO_AUTO4;
            else
                lOutArmAutoAlignmentFlag=lOutArmAutoAlignmentFlag&~OUTARM_ALIGNMENT_AUTO_AUTO4;

            if(MOT[MMAuto5].fHasTray && bAlignmentChangeUnloadTray[eAuto5])
                lOutArmAutoAlignmentFlag=lOutArmAutoAlignmentFlag|OUTARM_ALIGNMENT_AUTO_AUTO5;
            else
                lOutArmAutoAlignmentFlag=lOutArmAutoAlignmentFlag&~OUTARM_ALIGNMENT_AUTO_AUTO5;

            if(MOT[MMAuto6].fHasTray && bAlignmentChangeUnloadTray[eAuto6])
                lOutArmAutoAlignmentFlag=lOutArmAutoAlignmentFlag|OUTARM_ALIGNMENT_AUTO_AUTO6;
            else
                lOutArmAutoAlignmentFlag=lOutArmAutoAlignmentFlag&~OUTARM_ALIGNMENT_AUTO_AUTO6;

            if(MOT[MManualTray1].fHasTray && bAlignmentChangeUnloadTray[eFix1])
                lOutArmAutoAlignmentFlag=lOutArmAutoAlignmentFlag|OUTARM_ALIGNMENT_AUTO_FIX1;
            else
                lOutArmAutoAlignmentFlag=lOutArmAutoAlignmentFlag&~OUTARM_ALIGNMENT_AUTO_FIX1;

            if(MOT[MManualTray2].fHasTray && bAlignmentChangeUnloadTray[eFix2])
                lOutArmAutoAlignmentFlag=lOutArmAutoAlignmentFlag|OUTARM_ALIGNMENT_AUTO_FIX2;
            else
                lOutArmAutoAlignmentFlag=lOutArmAutoAlignmentFlag&~OUTARM_ALIGNMENT_AUTO_FIX2;

            if(MOT[MManualTray3].fHasTray && bAlignmentChangeUnloadTray[eFix3])
                lOutArmAutoAlignmentFlag=lOutArmAutoAlignmentFlag|OUTARM_ALIGNMENT_AUTO_FIX3;
            else
                lOutArmAutoAlignmentFlag=lOutArmAutoAlignmentFlag&~OUTARM_ALIGNMENT_AUTO_FIX3;

            if(MOT[MManualTray4].fHasTray && bAlignmentChangeUnloadTray[eFix4])
                lOutArmAutoAlignmentFlag=lOutArmAutoAlignmentFlag|OUTARM_ALIGNMENT_AUTO_FIX4;
            else
                lOutArmAutoAlignmentFlag=lOutArmAutoAlignmentFlag&~OUTARM_ALIGNMENT_AUTO_FIX4;

            if(MOT[MManualTray5].fHasTray && bAlignmentChangeUnloadTray[eFix5])
                lOutArmAutoAlignmentFlag=lOutArmAutoAlignmentFlag|OUTARM_ALIGNMENT_AUTO_FIX5;
            else
                lOutArmAutoAlignmentFlag=lOutArmAutoAlignmentFlag&~OUTARM_ALIGNMENT_AUTO_FIX5;

            if(MOT[MManualTray6].fHasTray && bAlignmentChangeUnloadTray[eFix6])
                lOutArmAutoAlignmentFlag=lOutArmAutoAlignmentFlag|OUTARM_ALIGNMENT_AUTO_FIX6;
            else
                lOutArmAutoAlignmentFlag=lOutArmAutoAlignmentFlag&~OUTARM_ALIGNMENT_AUTO_FIX6;

            lOutArmAutoAlignmentTrayTimeingFlag=0;
        }
        else
        {
            lOutArmAutoAlignmentTrayTimeingFlag=lOutArmAutoAlignmentTrayTimeingFlag|iMode;
            if(bOutArmAutoAlignmentClearFlag==false)                            //Kenhsieh 20211007 : 解決沒有掃描Kit問題
            {
                lOutArmAutoAlignmentFlag=0;
            }
        }
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
bool CheckOutArmAutoAlignmentCKModeBeUse(int iMode, bool bSet)
{
    if(TestIF.iAutoAlignmentShuttleHotplateEvent & iMode)
    {
        if(bSet)
        {
            if(TestIF.iShuttleMode==1 && TestIF_File.iShuttle_Sel==0)
            {
                lOutArmAutoAlignmentFlag=lOutArmAutoAlignmentFlag|OUTARM_ALIGNMENT_AUTO_SHUTTLE1;
                lOutArmAutoAlignmentFlag=lOutArmAutoAlignmentFlag&~OUTARM_ALIGNMENT_AUTO_SHUTTLE2;
            }
            else if(TestIF.iShuttleMode==1 && TestIF_File.iShuttle_Sel==1)
            {
                lOutArmAutoAlignmentFlag=lOutArmAutoAlignmentFlag&~OUTARM_ALIGNMENT_AUTO_SHUTTLE1;
                lOutArmAutoAlignmentFlag=lOutArmAutoAlignmentFlag|OUTARM_ALIGNMENT_AUTO_SHUTTLE2;
            }
            else
            {
                lOutArmAutoAlignmentFlag=lOutArmAutoAlignmentFlag|OUTARM_ALIGNMENT_AUTO_SHUTTLE1;
                lOutArmAutoAlignmentFlag=lOutArmAutoAlignmentFlag|OUTARM_ALIGNMENT_AUTO_SHUTTLE2;
            }
            lOutArmAutoAlignmentCKTimeingFlag=0;
        }
        else
        {
            lOutArmAutoAlignmentCKTimeingFlag=lOutArmAutoAlignmentCKTimeingFlag|iMode;
            lOutArmAutoAlignmentFlag=0;
        }
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
void VerifyNeedDoAlignment(unsigned long Tray, unsigned long Kit, bool bSet)
{
    bool bDoInAlignment, bDoOutAlignment;
    if(MACHINE_HAS_AUTO_ALIGNMENT_CCD &&
       TestIF.bEnableAutoAlignment==true &&
       (LastSet.iRealDummy==HAS_TRAY ||
        LastSet.iRealDummy==REALLY))
    {
        bDoInAlignment =CheckInArmAutoAlignmentTrayModeBeUse(Tray, bSet);
        bDoInAlignment|=CheckInArmAutoAlignmentCKModeBeUse(Kit, bSet);
        if(bDoInAlignment)
        {
            InitDoInArmTeachAlignmentProcessTask();
            bLoaderNeedTrayMustFinish=true;
            bInArmAutoAlignmentClearFlag=true;                                  //Kenhsieh 20211007 : 解決沒有掃描Kit問題
        }

        bDoOutAlignment =CheckOutArmAutoAlignmentTrayModeBeUse(Tray, bSet);
        bDoOutAlignment|=CheckOutArmAutoAlignmentCKModeBeUse(Kit, bSet);
        if(bDoOutAlignment)
        {
            InitDoOutArmTeachAlignmentProcessTask();
            bAutoNeedTrayMustFinish=true;
            for(int i=eAuto1; i<iFixRight; i++)
            {
                if(Prod.iTrayType[i]!=tNotUse)
                    bAlignmentChangeUnloadTray[i]=true;
                else
                    bAlignmentChangeUnloadTray[i]=false;
            }
            bOutArmAutoAlignmentClearFlag=true;                                 //Kenhsieh 20211007 : 解決沒有掃描Kit問題
        }
    }
}
//------------------------------------------------------------------------------
