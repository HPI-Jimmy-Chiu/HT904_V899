//---------------------------------------------------------------------------
#include "MachineDefine.h"
#pragma hdrstop

#include "TfAOILaserScan.h"

#include "main.h"
#include "uLotInfo.h"
#include "cprod.h"
#include "MyMotor.h"
#include "asendic.h"
#include "mysensor.h"
#include "acatchtray.h"                                                         //RogerYang 20250830 : Add殘料檢(台車)
//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
TAOISocket::TAOISocket()
{
    iCommandTask=0;
    socketClient=new uSocketClient();
    socketClient->SetReceiveFunc(ReceiveData);
}
//---------------------------------------------------------------------------
TAOISocket::~TAOISocket()
{
    delete socketClient;
}
//---------------------------------------------------------------------------
void TAOISocket::ReceiveData(char* cGet,int iLen)
{
    asReceiveMsg=AnsiString(cGet);
    if(asReceiveMsg.Length()>iLen)
        asReceiveMsg=asReceiveMsg.SubString(0,iLen);
    if(RecordMsg)
        RecordMsg("[Receive]"+asReceiveMsg);
}
//---------------------------------------------------------------------------
AnsiString TAOISocket::DoSendCommand(bool bIsFirst,AnsiString asCommand)
{
    if(bIsFirst)
    {
        iCommandTask=1;
        return "";
    }
    switch(iCommandTask)
    {
        case 1:
            //send command
            SendCommandSocket(asCommand);
            iCommandTask=100;
            break;
        case 100:
            if(asReceiveMsg=="")
            {/*wait receive*/}
            else
            {
                return asReceiveMsg;
            }
            break;
    }
    return "";
}
//---------------------------------------------------------------------------
void TAOISocket::SendCommandSocket(AnsiString asCommand)
{
    asReceiveMsg="";
    bflag=socketClient->SendCommand(asCommand.c_str(),asCommand.WideCharBufSize());
    if(RecordMsg)
    {
        if(bflag)
            RecordMsg("[Send]"+asCommand);
        else
        {
            RecordMsg("[Send Error]"+asCommand+"[IP]"+socketClient->GetSocketAddress()+"[Port]"+socketClient->GetSocketPort());
        }
    }
    Sleep(5);
}
//---------------------------------------------------------------------------
bool TAOISocket::DoCommuncation()
{
    return socketClient->DoOpenCommuncation();
}
//---------------------------------------------------------------------------
bool TAOISocket::IsCommActive()
{
    return socketClient->IsConnected();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
TfAOI::TfAOI(AnsiString asname)
{
    socketClient=new uSocketClient();
    tsAnalysis=new TStringList();
    socketAOI=new TAOISocket();
    socketClient->SetReceiveFunc(ReceiveData);
    bSimulationAOICommand=false;
    iTDCTask=0;
    iCommandTask=0;
    iCommandDelayTime=90;
    asName=asname;
    asTempStr="";
    SetMoveTrack(eMT_Loader);
    bNeedCheckInitialStart=false;
    bEnableRetryCount=false;
    iRemainICLaserRetryCount=0;
    bRemainICLaserAutoSkip=false;
    iActiveDelayTime=90;
}
//---------------------------------------------------------------------------
TfAOI::~TfAOI()
{
    delete tsAnalysis;
    delete socketClient;
    delete socketAOI;
}
//---------------------------------------------------------------------------
AnsiString TfAOI::GetAOI_Command(eAOI_Command eaoi)
{
    switch(eaoi)
    {
        case eAOI_LoadFile:
            return "LF";
        case eAOI_ClearBuffer:
            return "E9";
        case eAOI_GrabIC:
            return "E1";
        case eAOI_GetValueIC:
            return "GV";
        case eAOI_SetLot:
            return "SL";
        case eAOI_CreateFile:
            return "CF";
        case eAOI_SetGoldenImage:
            return "E2";
        case eAOI_GetStatus:
            return "GS";
    }
    return AnsiString("Not Find Mapping Command");
}
//---------------------------------------------------------------------------
AnsiString TfAOI::GetLoadFileName()
{
    return fMain->cbSetupFileName->Text;
}
//---------------------------------------------------------------------------
AnsiString TfAOI::GetLotMO()
{
    return (fLotInfo->ed_PIOEEMO->Text);
}
//---------------------------------------------------------------------------
AnsiString TfAOI::GetErrorMsg(AnsiString asFunName,AnsiString asErrorType,AnsiString asContent)
{
    return AnsiString().sprintf("[Error][%s][%s]%s",asFunName,asErrorType,asContent);
}
//---------------------------------------------------------------------------
AnsiString TfAOI::GetTrayData()
{
    return AnsiString().sprintf("%d_%d_%0.2f_%0.2f_%0.2f_%0.2f",
                                UserDefForm_File[0].XDivision,
                                UserDefForm_File[0].YDivision,
                                UserDefForm_File[0].XStart,
                                UserDefForm_File[0].YStart,
                                UserDefForm_File[0].XPitch,
                                UserDefForm_File[0].YPitch);
}
//---------------------------------------------------------------------------
void TfAOI::ReceiveData(char* cGet,int iLen)
{
    asReceiveMsg=AnsiString(cGet);
    if(asReceiveMsg.Length()>iLen)
        asReceiveMsg=asReceiveMsg.SubString(0,iLen);
    RecordMsgLaser("[Receive]"+asReceiveMsg);
}
//---------------------------------------------------------------------------
AnsiString TfAOI::DoSendCommand(bool bIsFirst,AnsiString asCommand)
{
    if(bIsFirst)
    {
        iCommandTask=1;
        return "";
    }
    switch(iCommandTask)
    {
        case 1:
            //send command
            SendCommandSocket(asCommand);
            iCommandTask=100;
            break;
        case 100:
            if(asReceiveMsg=="")
            {/*wait receive*/}
            else
            {
                return asReceiveMsg;
            }
            break;
    }
    return "";
}
//---------------------------------------------------------------------------
void TfAOI::SendCommandSocket(AnsiString asCommand)
{
    asReceiveMsg="";
    bflag=socketClient->SendCommand(asCommand.c_str(),asCommand.WideCharBufSize());
    if(bflag)
        RecordMsgLaser("[Send]"+asCommand);
    else
    {
        RecordMsgLaser("[Send Error]"+asCommand+"[IP]"+socketClient->GetSocketAddress()+"[Port]"+socketClient->GetSocketPort());
    }
    Sleep(5);
}
//---------------------------------------------------------------------------
bool TfAOI::DoCommuncation()
{
    if(bSimulationAOICommand)
        return true;
    return socketClient->DoOpenCommuncation();
}
//---------------------------------------------------------------------------
bool TfAOI::IsCommActive()
{
    return socketClient->IsConnected();
}
//---------------------------------------------------------------------------
AnsiString TfAOI::GetXC_YC()
{
    iRow=MOT[MMTrayY].Tray.YItem;
    iCol=MOT[MMTrayY].Tray.XItem;
    return IntToStr(iCol)+"_"+IntToStr(iRow); //XC_YC
}
//---------------------------------------------------------------------------
int TfAOI::SetAndBackUpLoaderYSpeed(int iLoaderYCWSpeedPerc)                    //RogerYang 20250906 : Add for MLoaderY
{
    //AI(ht9045-v899) 20260505: 只記錄當下瞬間速度供 log 查看，不再用它做還原 (ReadSpeed 是瞬間速度不是設定值，會把馬達設成 0/2 龜速)
    int iSnapshotSpd=MOT[MLoaderY].GetSpeed();
    MOT[MLoaderY].SetSpeed(iLoaderYCWSpeedPerc);
    return iSnapshotSpd;
}
//---------------------------------------------------------------------------
void TfAOI::RestoreLoaderYSpeedRaw(int iRawSpeed)                               //AI(ht9045-v899) 20260505: 參數忽略 (歷史遺留)，永遠還原為 100% 預設量產速度，避免任何後續 Home/Teach 受影響
{
    //AI(ht9045-v899) 20260505: 修正前版 bug — 原本把 ReadSpeed() 當設定值還原 → 馬達被設成 0 或 2 → Home 反向撞擊、form 關閉龜速漂移
    MOT[MLoaderY].SetSpeed(100);
}
//---------------------------------------------------------------------------
bool TfAOI::DoMoveOut(bool bIsFirst,AnsiString &asError)
{
    #ifdef SOFT_SIMULTE
    return true;
    #else
    if(bIsFirst)
    {
        iTDCTask=1;
        asError="";
        return true;
    }

    if(asError!="")
        return true;
    switch(iTDCTask)
    {
        case 1:
            if(INSTALL_OCR_YMot==eocrYMotInstal)                                //RogerYang 20250830 : Add殘料檢(台車)
            {
                iTDCTask=500;
            }
            else
            {
                iTDCTask=1000;
                TrayMoveOut(false, GetMoveTrack());
            }
//            return false;
        case 500:
            if(MOT[MLoaderY].ReadPos()<=Prod.iMLoaderYCarPos-5 ||               //RogerYang 20251028 : Add
               MOT[MLoaderY].ReadPos()>=Prod.iMLoaderYCarPos+5)
            {
                iTDCTask=600;
            }
            else
            {
                iTDCTask=9999;
            }
            return false;
        case 600:
            //AI(ht9045-v899) 20260505: 與 case 500 判斷的目標位置 (CarPos) 對齊；原本判斷 CarPos 卻 MoveTo OCRPos，導致到位後仍不滿足 ±5 條件 → 死迴圈一直跑到 OCRPos
            if(MOT[MLoaderY].MotorMove(Prod.iMLoaderYCarPos))
            {
                iTDCTask=500;
            }
            return false;
        case 1000:
            TrayMoveOut(true, GetMoveTrack(), -99999);
            iTDCTask=2000;
            return false;
        case 2000:
            TrayMoveOut(true, GetMoveTrack(), -99999);
            if(GetOutSensor())
            {
                TrayArrivalDelay.SetSecAndOn(3);
                iTDCTask=3000;
            }
            return false;
        case 3000:
            TrayMoveOut(true, GetMoveTrack(), -99999);
            if(TrayArrivalDelay.Off() && GetOutSensor())
            {
                TrayMoveOut(false, GetMoveTrack());
            }
            else
                return false;
            iTDCTask=9999;
            return false;
        case 9999: //finish
            return true;
        default:
            asError=AnsiString(__FUNC__)+"，Get Result wrong step="+IntToStr(iTDCTask);
            return true;
    }
    #endif
}
//---------------------------------------------------------------------------
bool TfAOI::DoMoveIn(bool bIsFirst,AnsiString &asError)
{
    #ifdef SOFT_SIMULTE
    return true;
    #else
    if(bIsFirst)
    {
        iTDCTask=1;
        AOIDelay.SetSecAndOn(CommandDelay());
        asError="";
        return true;
    }

    if(AOIDelay.Off())
        asError=AnsiString(__FUNC__)+" TimeOut "+IntToStr(CommandDelay())+" sec step="+IntToStr(iTDCTask);
    if(asError!="")
        return true;
    switch(iTDCTask)
    {
        case 1:
            AOIDelay.SetSecAndOn(CommandDelay());

            if(INSTALL_OCR_YMot==eocrYMotInstal)                                //RogerYang 20250830 : Add殘料檢(台車)
            {
                iTDCTask=500;
            }
            else
            {
                iTDCTask=1000;
                TrayMoveIn(false, GetMoveTrack());
            }
//            return false;
        case 500:
            if(MOT[MLoaderY].ReadPos()<=Prod.iMLoaderYSurePos-5 ||              //RogerYang 20251028 : Add
               MOT[MLoaderY].ReadPos()>=Prod.iMLoaderYSurePos+5)
            {
                iTDCTask=600;
            }
            else
            {
                iTDCTask=9999;
            }
            return false;
        case 600:
            if(MOT[MLoaderY].MotorMove(Prod.iMLoaderYSurePos))                  //RogerYang 20250830 : Add殘料檢(台車)
            {
                iTDCTask=500;
            }
            return false;
        case 1000:
            TrayMoveIn(true, GetMoveTrack(), 99999);
            AOIDelay.SetSecAndOn(CommandDelay());
            iTDCTask=2000;
            return false;
        case 2000:
            TrayMoveIn(true, GetMoveTrack(), 99999);
            if(GetInSensor())                                                   //jimmychiu 20220414 delete SnLoaderPreDete
            {
                htDetectICFloatingDelay.SetSecAndOn(iLD_TrayArrivalDely());     //Sam 20200420 : 到了在滑一下。
                AOIDelay.SetSecAndOn(CommandDelay());
            }
            else
            {
                TrayMoveIn(true, GetMoveTrack(), 99999);
                return false;
            }
            iTDCTask=3000;
            return false;
        case 3000:
            TrayMoveIn(true, GetMoveTrack(), 99999);
            if(htDetectICFloatingDelay.Off() && GetInSensor())
            {
                TrayMoveIn(false, GetMoveTrack(), 99999);                       //Loader 進料停止
                iTDCTask=9999;
            }
            return false;
        case 9999: //finish
            return true;
        default:
            asError=AnsiString(__FUNC__)+"，Get Result wrong step="+IntToStr(iTDCTask);
            return true;
    }
    #endif
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
bool TfAOI::DoSetLot(bool bIsFirst)
{
    if(bIsFirst)
    {
        DoSendCommand(true,"");
        asSendMsg=GetAOI_Command(eAOI_SetLot)+","+GetLotMO(); //SL,Abc1234
        return true;
    }
    else
    {
        if(bSimulationAOICommand)
        {
            asReceiveMsg="SL,1,Abc1234";
            RecordMsgLaser("[Receive]"+asReceiveMsg);
        }
        else
            asReceiveMsg=DoSendCommand(false,asSendMsg);
        if(asReceiveMsg==""){/*wait receive*/}
        else
        {
            tsAnalysis->CommaText=asReceiveMsg;  //SL,1,Abc1234
            if(tsAnalysis->Count==3)
            {
                if(tsAnalysis->Strings[1]=="1")
                    return true;
                else
                    RecordErrorMsgLaser(GetErrorMsg(__FUNC__,GetAOIReturnError(),asReceiveMsg));
            }
            else//Error
                RecordErrorMsgLaser(GetErrorMsg(__FUNC__,GetReceiveWrongCommand(),asReceiveMsg));
            return true;
        }
        return false;
    }
}
//---------------------------------------------------------------------------
bool TfAOI::DoAutoTuning(bool bIsFirst,AnsiString &asError)
{
    if(bIsFirst)
    {
        iTDCTask=1;
        AOIDelay.SetSecAndOn(CommandDelay());
        asError=AnsiString("");
        return true;
    }

    if(AOIDelay.Off())
    {
        asError=__FUNC__;
        asError+=" TimeOut "+IntToStr(CommandDelay())+" sec step="+IntToStr(iTDCTask);
    }

    if(asError!="")
        return true;
    switch(iTDCTask)
    {
        case 1:
            AOIDelay.SetSecAndOn(CommandDelay());
            iTDCTask=1000;
            return false;
        case 1000:
            if(DoCommuncation())
            {
                iTDCTask=2000;
            }
            return false;
        case 2000:
            DoAOI_LoadFile(true);
            AOIDelay.SetSecAndOn(CommandDelay());
            iTDCTask=3000;
            return false;
        case 3000:
            if(DoAOI_LoadFile(false))
            {
                iTDCTask=4000;
            }
            return false;
        case 4000:
            DoCreateFile(true);
            AOIDelay.SetSecAndOn(CommandDelay());
            iTDCTask=5000;
            return false;
        case 5000:
            if(DoCreateFile(false))
            {
                iTDCTask=6000;
            }
            return false;
        case 6000:
            DoGoldenImage(true, 0);                                             //RogerYang 20250830 : 殘料檢變更格式
            AOIDelay.SetSecAndOn(CommandDelay());
            iTDCTask=7000;
            return false;
        case 7000:
            if(DoGoldenImage(false, 0))                                         //RogerYang 20250830 : 殘料檢變更格式
            {
                iTDCTask=8000;
                ttLoaderCarEdgePushDelay.SetSecAndOn(LoaderCarEdgePushDelay());
            }
            return false;
        case 8000:
//            DoGetStatus(true);
//            AOIDelay.SetSecAndOn(CommandDelay());
//            iTDCTask=9000;
//            return false;
//        case 9000:
//            if(DoGetStatus(false))
//            {
//              iTDCTask=10000;
//                ttLoaderCarEdgePushDelay.SetSecAndOn(LoaderCarEdgePushDelay());
//            }
//            return false;
//        case 10000:
            if(ttLoaderCarEdgePushDelay.Off())
            {
                iTDCTask=9999;
            }
            return false;
        case 9999: //finish
            return true;
        default:
            asError=__FUNC__;
            asError+="，Get Result wrong step="+IntToStr(iTDCTask);
            return true;
    }
}
//---------------------------------------------------------------------------
void TfAOI::SetMoveTrack(int iTrack)
{
    iMovetrack=iTrack;
}
//---------------------------------------------------------------------------
int TfAOI::GetMoveTrack()
{
    return iMovetrack;
}
//---------------------------------------------------------------------------
bool TfAOI::GetInSensor()
{
    switch(iMovetrack)
    {
        case eMT_Loader:
            return Sen[SnLoaderSureTray].Enable && Sen[SnLoaderSureTray].IsOn();
        case eMT_Auto1:
            return Sen[SnAuto1TrayDetect].IsOn();
        case eMT_Color:
            return Sen[SenColorHasTray].IsOn();
        default:
            return false;
    }
}
//---------------------------------------------------------------------------
bool TfAOI::GetOutSensor()
{
    switch(iMovetrack)
    {
        case eMT_Loader:
            return Sen[SnLoaderCarHasTray].IsOn();
        case eMT_Auto1:
            return Sen[SnAuto1PreDete].IsOn();
        case eMT_Color:
            return (Sen[SenColorCarHasTray].IsOn() && Sen[SenColorCCWDete].IsOn());
        default:
            return false;
    }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
TfTrayDeviceCheckFromLoader::TfTrayDeviceCheckFromLoader(AnsiString asname) : TfAOI(asname)
{
}
//---------------------------------------------------------------------------
TfTrayDeviceCheckFromLoader::~TfTrayDeviceCheckFromLoader()
{
}
//---------------------------------------------------------------------------
bool TfTrayDeviceCheckFromLoader::DoSettingBeforeMotorMove(bool bIsFirst,AnsiString &asError)
{
    if(bIsFirst)
    {
        iTDCTask=1;
        AOIDelay.SetSecAndOn(CommandDelay());
        asError=AnsiString("");
        return true;
    }

    if(AOIDelay.Off())
        asError=AnsiString(__FUNC__)+" TimeOut "+IntToStr(CommandDelay())+" sec step="+IntToStr(iTDCTask);
    if(asError!="")
        return true;
    switch(iTDCTask)
    {
        case 1:
            AOIDelay.SetSecAndOn(CommandDelay());
            iTDCTask=1000;
            return false;
        case 1000:
            if(DoCommuncation())
            {
                iTDCTask=2000;
            }
            return false;
        case 2000:
            DoAOI_LoadFile(true);
            AOIDelay.SetSecAndOn(CommandDelay());
            iTDCTask=3000;
            return false;
        case 3000:
            if(DoAOI_LoadFile(false))
            {
                iTDCTask=3500;
            }
            return false;
        case 3500:
            DoSetLot(true);
            AOIDelay.SetSecAndOn(CommandDelay());
            iTDCTask=3700;
            return false;
        case 3700:
            if(DoSetLot(false))
            {
                iTDCTask=4000;
            }
            return false;
        case 4000:
            DoAOI_ClearBuffer(true);
            AOIDelay.SetSecAndOn(CommandDelay());
            iTDCTask=5000;
            return false;
        case 5000:
            if(DoAOI_ClearBuffer(false))
            {
                iTDCTask=6000;
            }
            return false;
        case 6000:
            DoAOI_Function(true);
            AOIDelay.SetSecAndOn(CommandDelay());
            iTDCTask=7000;
            return false;
        case 7000:
            if(DoAOI_Function(false))
            {
                iTDCTask=8000;
//                Cylinder[C_LoaderCarEdgePush].On();
                ttLoaderCarEdgePushDelay.SetSecAndOn(LoaderCarEdgePushDelay());
            }
            return false;
        case 8000:
            if(ttLoaderCarEdgePushDelay.Off())
            {
//                Cylinder[C_LoaderCarEdgePush].Off();
                iTDCTask=9999;
            }
            return false;
        case 9999: //finish
            return true;
        default:
            asError=AnsiString(__FUNC__)+"，Get Result wrong step="+IntToStr(iTDCTask);
            return true;
    }
}
//---------------------------------------------------------------------------
bool TfTrayDeviceCheckFromLoader::DoGetValueAfterMotorArrival(bool bIsFirst,AnsiString &asError)
{
    if(bIsFirst)
    {
        iTDCTask=1;
        AOIDelay.SetSecAndOn(CommandDelay());
        asError=AnsiString("");
        return true;
    }

    if(AOIDelay.Off())
        asError=AnsiString(__FUNC__)+" TimeOut "+IntToStr(CommandDelay())+" sec step="+IntToStr(iTDCTask);
    if(asError!="")
        return true;
    switch(iTDCTask)
    {
        case 1:
            AOIDelay.SetSecAndOn(CommandDelay());
            iTDCTask=1000;
            return false;
        case 1000:
            if(DoCommuncation())
                iTDCTask=2000;
            return false;
        case 2000:
            DoGetValue(true);
            AOIDelay.SetSecAndOn(CommandDelay());
            iTDCTask=3000;
            return false;
        case 3000:
            if(DoGetValue(false))
                iTDCTask=9999;
            return false;
        case 9999: //finish
            socketClient->Close();
            return true;
        default:
            asError=AnsiString(__FUNC__)+"，Get Result wrong step="+IntToStr(iTDCTask);
            return true;
    }
}
//---------------------------------------------------------------------------
bool TfTrayDeviceCheckFromLoader::DoAOI_LoadFile(bool bIsFirst)
{
    if(bIsFirst)
    {
        DoSendCommand(true,"");
        asSendMsg=GetAOI_Command(eAOI_LoadFile)+","+GetFunctionCode()+","+GetLoadFileName();
        return true;
    }
    else
    {
        if(bSimulationAOICommand)
        {
            asReceiveMsg="LF,5,1,Filename";
            RecordMsgLaser("[Receive]"+asReceiveMsg);
        }
        else
            asReceiveMsg=DoSendCommand(false,asSendMsg);
        if(asReceiveMsg==""){/*wait receive*/}
        else
        {
            tsAnalysis->CommaText=asReceiveMsg;  //LF,5,1,Filename
            if(tsAnalysis->Count>3)
            {
                if(tsAnalysis->Strings[2]=="1")
                    return true;
                else
                    RecordErrorMsgLaser(GetErrorMsg(__FUNC__,GetAOIReturnError(),asReceiveMsg));
            }
            else//Error
                RecordErrorMsgLaser(GetErrorMsg(__FUNC__,GetReceiveWrongCommand(),asReceiveMsg));
            return true;
        }
        return false;
    }
}
//---------------------------------------------------------------------------
bool TfTrayDeviceCheckFromLoader::DoAOI_ClearBuffer(bool bIsFirst)
{
    if(bIsFirst)
    {
        DoSendCommand(true,"");
        bIsRemainIC=false;
        asSendMsg=GetAOI_Command(eAOI_ClearBuffer)+","+GetFunctionCode()+","+GetXC_YC(); //E9,5,XC_YC
        return true;
    }
    else
    {
        if(bSimulationAOICommand)
        {
            asReceiveMsg="E9,5,1";
            RecordMsgLaser("[Receive]"+asReceiveMsg);
        }
        else
            asReceiveMsg=DoSendCommand(false,asSendMsg);
        if(asReceiveMsg==""){/*wait receive*/}
        else
        {
            tsAnalysis->CommaText=asReceiveMsg;  //E9,5,1
            if(tsAnalysis->Count>2)
            {
                if(tsAnalysis->Strings[2]=="1")
                    return true;
                else
                    RecordErrorMsgLaser(GetErrorMsg(__FUNC__,GetAOIReturnError(),asReceiveMsg));
            }
            else//Error
                RecordErrorMsgLaser(GetErrorMsg(__FUNC__,GetReceiveWrongCommand(),asReceiveMsg));
            return true;
        }
        return false;
    }
}
//---------------------------------------------------------------------------
bool TfTrayDeviceCheckFromLoader::DoAOI_Function(bool bIsFirst, int iIndex)     //RogerYang 20250830 : 殘料檢變更格式
{
    if(bIsFirst)
    {
        DoSendCommand(true,"");
        asSendMsg=GetAOI_Command(eAOI_GrabIC)+","+GetFunctionCode()+","+"0"; //E1,IC_Remain,Index
        return true;
    }
    else
    {
        if(bSimulationAOICommand)
        {
            asReceiveMsg="E1,5,1";
            RecordMsgLaser("[Receive]"+asReceiveMsg);
        }
        else
            asReceiveMsg=DoSendCommand(false,asSendMsg);
        if(asReceiveMsg==""){/*wait receive*/}
        else
        {
            tsAnalysis->CommaText=asReceiveMsg;  //E1,5,Result
            if(tsAnalysis->Count>2)
            {
                if(tsAnalysis->Strings[2]=="1")
                    return true;
                else
                    RecordErrorMsgLaser(GetErrorMsg(__FUNC__,GetAOIReturnError(),asReceiveMsg));
            }
            else//Error
                RecordErrorMsgLaser(GetErrorMsg(__FUNC__,GetReceiveWrongCommand(),asReceiveMsg));
            return true;
        }
        return false;
    }
}
//---------------------------------------------------------------------------
bool TfTrayDeviceCheckFromLoader::DoGetValue(bool bIsFirst)
{
    if(bIsFirst)
    {
        DoSendCommand(true,"");
        asSendMsg=GetAOI_Command(eAOI_GetValueIC)+","+GetFunctionCode(); //GV,5
        return true;
    }
    else
    {
        if(bSimulationAOICommand)
        {
            asReceiveMsg="GV,5,1";
            RecordMsgLaser("[Receive]"+asReceiveMsg);
        }
        else
            asReceiveMsg=DoSendCommand(false,asSendMsg);
        if(asReceiveMsg==""){/*wait receive*/}
        else
        {
            tsAnalysis->CommaText=asReceiveMsg;  //GV,5,1
            if(tsAnalysis->Count>2)
            {
                if(tsAnalysis->Strings[2]=="1") //1:Pass
                {
                    bIsRemainIC=false;
                    return true;
                }
                else if(tsAnalysis->Strings[2]=="2") //2:取像中 or 檢測中
                    return false;
                else if(tsAnalysis->Strings[2]=="3") //3:殘料檢出
                {
                    bIsRemainIC=true;
                    return true;
                }
                else
                    RecordErrorMsgLaser(GetErrorMsg(__FUNC__,GetAOIReturnError(),asReceiveMsg));
            }
            else//Error
                RecordErrorMsgLaser(GetErrorMsg(__FUNC__,GetReceiveWrongCommand(),asReceiveMsg));
            return true;
        }
        return false;
    }
}
//---------------------------------------------------------------------------
bool TfTrayDeviceCheckFromLoader::DoGoldenImage(bool bIsFirst, int iIndex)      //RogerYang 20250830 : 殘料檢變更格式
{
    if(bIsFirst)
    {
        DoSendCommand(true,"");
        asSendMsg=GetAOI_Command(eAOI_SetGoldenImage)+","+GetFunctionCode()+","+IntToStr(iIndex); //RogerYang 20250830 : 殘料檢變更格式 //E2,5,Index
        return true;
    }
    else
    {
        if(bSimulationAOICommand)
        {
            asReceiveMsg="E2,5,1," + IntToStr(iIndex);                          //RogerYang 20250830 : 殘料檢變更格式
            RecordMsgLaser("[Receive]"+asReceiveMsg);
        }
        else
            asReceiveMsg=DoSendCommand(false,asSendMsg);
        if(asReceiveMsg==""){/*wait receive*/}
        else
        {
            tsAnalysis->CommaText=asReceiveMsg;  //E2,5,1
            if(tsAnalysis->Count==3)                                            //RogerYang 20250830 : 殘料檢變更格式
            {
                if(tsAnalysis->Strings[2]=="1")
                    return true;
                else
                    RecordErrorMsgLaser(GetErrorMsg(__FUNC__,GetAOIReturnError(),asReceiveMsg));
            }
            else//Error
                RecordErrorMsgLaser(GetErrorMsg(__FUNC__,GetReceiveWrongCommand(),asReceiveMsg));
            return true;
        }
        return false;
    }
}
//---------------------------------------------------------------------------
bool TfTrayDeviceCheckFromLoader::DoGetStatus(bool bIsFirst)
{
    if(bIsFirst)
    {
        DoSendCommand(true,"");
        asSendMsg=GetAOI_Command(eAOI_GetStatus)+","+GetFunctionCode(); //GS,5
        return true;
    }
    else
    {
        if(bSimulationAOICommand)
        {
            asReceiveMsg="GS,5,1";
            RecordMsgLaser("[Receive]"+asReceiveMsg);
        }
        else
            asReceiveMsg=DoSendCommand(false,asSendMsg);
        if(asReceiveMsg==""){/*wait receive*/}
        else
        {
            tsAnalysis->CommaText=asReceiveMsg;  //GS,5,1
            if(tsAnalysis->Count==3)
            {
                if(tsAnalysis->Strings[2]=="1")
                    return true;
                else
                    RecordErrorMsgLaser(GetErrorMsg(__FUNC__,GetAOIReturnError(),asReceiveMsg));
            }
            else//Error
                RecordErrorMsgLaser(GetErrorMsg(__FUNC__,GetReceiveWrongCommand(),asReceiveMsg));
            return true;
        }
        return false;
    }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
TfTrayMapFromLoader::TfTrayMapFromLoader(AnsiString asname) : TfAOI(asname)
{
}
//---------------------------------------------------------------------------
TfTrayMapFromLoader::~TfTrayMapFromLoader()
{
    delete trayMapIC;
}
//---------------------------------------------------------------------------
bool TfTrayMapFromLoader::DoAOI_LoadFile(bool bIsFirst)
{
    if(bIsFirst)
    {
        DoSendCommand(true,"");
        asSendMsg=GetAOI_Command(eAOI_LoadFile)+","+GetFunctionCode()+","+GetLoadFileName();
        return true;
    }
    else
    {
        if(bSimulationAOICommand)
        {
            asReceiveMsg="LF,4,1,Filename";
            RecordMsgLaser("[Receive]"+asReceiveMsg);
        }
        else
            asReceiveMsg=DoSendCommand(false,asSendMsg);
        if(asReceiveMsg==""){/*wait receive*/}
        else
        {
            tsAnalysis->CommaText=asReceiveMsg;  //LF,4,1,Filename
            if(tsAnalysis->Count>3)
            {
                if(tsAnalysis->Strings[2]=="1")
                    return true;
                else
                    RecordErrorMsgLaser(GetErrorMsg(__FUNC__,GetAOIReturnError(),asReceiveMsg));
            }
            else//Error
                RecordErrorMsgLaser(GetErrorMsg(__FUNC__,GetReceiveWrongCommand(),asReceiveMsg));
            return true;
        }
        return false;
    }
}
//---------------------------------------------------------------------------
bool TfTrayMapFromLoader::DoAOI_ClearBuffer(bool bIsFirst)
{
    if(bIsFirst)
    {
        DoSendCommand(true,"");
        trayMapIC->ClearCell();
        asSendMsg=GetAOI_Command(eAOI_ClearBuffer)+","+GetFunctionCode()+","+GetXC_YC(); //E9,4,3_5
        return true;
    }
    else
    {
        if(bSimulationAOICommand)
        {
            asReceiveMsg="E9,4,1";
            RecordMsgLaser("[Receive]"+asReceiveMsg);
        }
        else
            asReceiveMsg=DoSendCommand(false,asSendMsg);
        if(asReceiveMsg==""){/*wait receive*/}
        else
        {
            tsAnalysis->CommaText=asReceiveMsg;  //E9,4,1
            if(tsAnalysis->Count>2)
            {
                if(tsAnalysis->Strings[2]=="1")
                    return true;
                else
                    RecordErrorMsgLaser(GetErrorMsg(__FUNC__,GetAOIReturnError(),asReceiveMsg));
            }
            else//Error
                RecordErrorMsgLaser(GetErrorMsg(__FUNC__,GetReceiveWrongCommand(),asReceiveMsg));
            return true;
        }
        return false;
    }
}
//---------------------------------------------------------------------------
bool TfTrayMapFromLoader::DoAOI_Function(bool bIsFirst, int iIndex)             //RogerYang 20250830 : 殘料檢變更格式
{
    if(bIsFirst)
    {
        DoSendCommand(true,"");
        asSendMsg=GetAOI_Command(eAOI_GrabIC)+","+GetFunctionCode()+","+"0"; //E1,4,Index
        return true;
    }
    else
    {
        if(bSimulationAOICommand)
        {
            asReceiveMsg="E1,4,1";
            RecordMsgLaser("[Receive]"+asReceiveMsg);
        }
        else
            asReceiveMsg=DoSendCommand(false,asSendMsg);
        if(asReceiveMsg==""){/*wait receive*/}
        else
        {
            tsAnalysis->CommaText=asReceiveMsg;  //E1,4,1
            if(tsAnalysis->Count>2)
            {
                if(tsAnalysis->Strings[2]=="1")
                    return true;
                else
                    RecordErrorMsgLaser(GetErrorMsg(__FUNC__,GetAOIReturnError(),asReceiveMsg));
            }
            else//Error
                RecordErrorMsgLaser(GetErrorMsg(__FUNC__,GetReceiveWrongCommand(),asReceiveMsg));
            return true;
        }
        return false;
    }
}
//---------------------------------------------------------------------------
bool TfTrayMapFromLoader::DoGetValue(bool bIsFirst)
{
    if(bIsFirst)
    {
        DoSendCommand(true,"");
        asSendMsg=GetAOI_Command(eAOI_GetValueIC)+","+GetFunctionCode(); //GV,4
        return true;
    }
    else
    {
        if(bSimulationAOICommand)
        {
            AnsiString asDatas="";
            for(int i=0;i<iRow*iCol;i++)asDatas+=IntToStr((i<24?1:0));
            asReceiveMsg="GV,4,1,"+GetXC_YC()+","+asDatas;
            RecordMsgLaser("[Receive]"+asReceiveMsg);
        }
        else
            asReceiveMsg=DoSendCommand(false,asSendMsg);
        if(asReceiveMsg==""){/*wait receive*/}
        else
        {
            tsAnalysis->CommaText=asReceiveMsg;
            if(tsAnalysis->Count>3)
            {
                if(tsAnalysis->Count>4)  //GV,4,1,3_5,111111111111000
                {
                    if(tsAnalysis->Strings[2]=="1") //1:Pass
                    {
                        int iGetCol,iGetRow;
                        AnalysisRowCol(tsAnalysis->Strings[3],iGetCol,iGetRow);
                        if(iGetCol*iGetRow==0)
                        {
                            RecordErrorMsgLaser("[Error]["+AnsiString(__FUNC__)+"][AOI Col or Row is Zero]"+asReceiveMsg);
                            return true;
                        }

                        if(iRow!=iGetRow || iCol!=iGetCol)
                        {
                            RecordErrorMsgLaser("[Error]["+AnsiString(__FUNC__)+"][AOI Col or Row is Not Mapping Now Setting Size]"+asReceiveMsg);
                            return true;
                        }
                        trayMapIC->XItem=iGetCol;
                        trayMapIC->YItem=iGetRow;
                        AnalysisTrayData(trayMapIC,tsAnalysis->Strings[4]);
                        return true;
                    }
                    else     //0:CCD ERR
                        RecordErrorMsgLaser(GetErrorMsg(__FUNC__,GetAOIReturnError(),asReceiveMsg));
                }
                else                     //GV,4,2
                {
                    if(tsAnalysis->Strings[2]=="2") //2:取像未完成
                    {
                        return false;
                    }
                    else     //0:CCD ERR
                        RecordErrorMsgLaser(GetErrorMsg(__FUNC__,GetAOIReturnError(),asReceiveMsg));
                }
            }
            else//Error
                RecordErrorMsgLaser(GetErrorMsg(__FUNC__,GetReceiveWrongCommand(),asReceiveMsg));
            return true;
        }
        return false;
    }
}
//---------------------------------------------------------------------------
bool TfTrayMapFromLoader::DoCreateFile(bool bIsFirst)
{
    if(bIsFirst)
    {
        DoSendCommand(true,"");
        /*
        CF,4,3_5_30_30_25_25
             XC_YC_XP_YP_W_H
             XC、YC表示IC數量，XP、YP表是間距，W、H表示IC大小(單位:mm)
        */
        asSendMsg=GetAOI_Command(eAOI_CreateFile)+","+GetFunctionCode()+","+GetTrayData(); //CF,4,3_5_30_30_25_25
        return true;
    }
    else
    {
        if(bSimulationAOICommand)
        {
            asReceiveMsg="CF,4,1,0";                                            //RogerYang 20250830 : 殘料檢變更格式
            RecordMsgLaser("[Receive]"+asReceiveMsg);
        }
        else
            asReceiveMsg=DoSendCommand(false,asSendMsg);
        if(asReceiveMsg==""){/*wait receive*/}
        else
        {
            tsAnalysis->CommaText=asReceiveMsg;   //CF,4,1,ScanCount
            if(tsAnalysis->Count==4)                                            //RogerYang 20250830 : 殘料檢變更格式
            {
                if(tsAnalysis->Strings[2]=="1")
                    return true;
                else
                    RecordErrorMsgLaser(GetErrorMsg(__FUNC__,GetAOIReturnError(),asReceiveMsg));
            }
            else//Error
                RecordErrorMsgLaser(GetErrorMsg(__FUNC__,GetReceiveWrongCommand(),asReceiveMsg));
            return true;
        }
        return false;
    }
}
//---------------------------------------------------------------------------
bool TfTrayMapFromLoader::DoGoldenImage(bool bIsFirst, int iIndex)              //RogerYang 20250830 : 殘料檢變更格式
{
    if(bIsFirst)
    {
        DoSendCommand(true,"");
        asSendMsg=GetAOI_Command(eAOI_SetGoldenImage)+","+GetFunctionCode()+","+IntToStr(iIndex); //E2,4,Index  //RogerYang 20250830 : 殘料檢變更格式
        return true;
    }
    else
    {
        if(bSimulationAOICommand)
        {
            asReceiveMsg="E2,4,1,"+IntToStr(iIndex);                            //RogerYang 20250830 : 殘料檢變更格式
            RecordMsgLaser("[Receive]"+asReceiveMsg);
        }
        else
            asReceiveMsg=DoSendCommand(false,asSendMsg);
        if(asReceiveMsg==""){/*wait receive*/}
        else
        {
            tsAnalysis->CommaText=asReceiveMsg;  //E2,4,1
            if(tsAnalysis->Count==3)
            {
                if(tsAnalysis->Strings[2]=="1")
                    return true;
                else
                    RecordErrorMsgLaser(GetErrorMsg(__FUNC__,GetAOIReturnError(),asReceiveMsg));
            }
            else//Error
                RecordErrorMsgLaser(GetErrorMsg(__FUNC__,GetReceiveWrongCommand(),asReceiveMsg));
            return true;
        }
        return false;
    }
}
//---------------------------------------------------------------------------
bool TfTrayMapFromLoader::DoGetStatus(bool bIsFirst)
{
    if(bIsFirst)
    {
        DoSendCommand(true,"");
        asSendMsg=GetAOI_Command(eAOI_GetStatus)+","+GetFunctionCode(); //GS,4
        return true;
    }
    else
    {
        if(bSimulationAOICommand)
        {
            asReceiveMsg="GS,4,1";
            RecordMsgLaser("[Receive]"+asReceiveMsg);
        }
        else
            asReceiveMsg=DoSendCommand(false,asSendMsg);
        if(asReceiveMsg==""){/*wait receive*/}
        else
        {
            tsAnalysis->CommaText=asReceiveMsg;  //GS,4,1
            if(tsAnalysis->Count==3)
            {
                if(tsAnalysis->Strings[2]=="1")
                    return true;
                else
                    RecordErrorMsgLaser(GetErrorMsg(__FUNC__,GetAOIReturnError(),asReceiveMsg));
            }
            else//Error
                RecordErrorMsgLaser(GetErrorMsg(__FUNC__,GetReceiveWrongCommand(),asReceiveMsg));
            return true;
        }
        return false;
    }
}
//---------------------------------------------------------------------------
void TfTrayMapFromLoader::AnalysisRowCol(AnsiString asInput,int &icow,int &irow)
{
    itemp=asInput.Pos("_");
    if(itemp==0)
    {
        icow=0;
        irow=0;
        return;
    }
    icow=atoi(asInput.SubString(0,itemp-1).c_str());
    irow=atoi(asInput.SubString(itemp+1,asInput.Length()).c_str());
}
//---------------------------------------------------------------------------
void TfTrayMapFromLoader::AnalysisTrayData(TTMyTray* mTray,AnsiString asRawData)
{
    int iIC=0;
    int iRow=mTray->YItem;
    int iCol=mTray->XItem;
    if(iCol==0)return;
    for(int i=0;i<asRawData.Length();i++)
    {
        if(iRow<=(i/iCol))
            return;
        //iIC=asRawData.SubString(i+1,1)=="1"?1:0;
        iIC=StrToIntDef(asRawData.SubString(i+1,1), 0);                         //RogerYang 20250907 : 新增翹料
        switch(iIC)
        {
            case 0:
                mTray->SetCellColorIndex(i%iCol, i/iCol, 0);
            break;
            case 1:
                mTray->SetCellColorIndex(i%iCol, i/iCol, 1);
            break;
            case 2:
                mTray->SetCellColorIndex(i%iCol, i/iCol, HAS_OCR_NG);
            break;
        }
    }
}
//---------------------------------------------------------------------------
bool TfTrayMapFromLoader::HasNullIC(TTMyTray* mTray)
{
    for(int i=0;i<mTray->YItem;i++)
    {
        for(int j=0;j<mTray->XItem;j++)
        {
            if(mTray->GetCellData(i,j)==0)
                return true;
        }
    }
    return false;
}
//---------------------------------------------------------------------------
bool TfTrayMapFromLoader::DoSettingBeforeMotorMove(bool bIsFirst,AnsiString &asError)
{
    if(bIsFirst)
    {
        iTDCTask=1;
        AOIDelay.SetSecAndOn(CommandDelay());
        asError="";
        return true;
    }

    if(AOIDelay.Off())
        asError=AnsiString(__FUNC__)+" TimeOut "+IntToStr(CommandDelay())+" sec step="+IntToStr(iTDCTask);
    if(asError!="")
        return true;
    switch(iTDCTask)
    {
        case 1:
            AOIDelay.SetSecAndOn(CommandDelay());
            iTDCTask=1000;
            return false;
        case 1000:
            if(DoCommuncation())
                iTDCTask=2000;
            return false;
        case 2000:
            DoAOI_LoadFile(true);
            AOIDelay.SetSecAndOn(CommandDelay());
            iTDCTask=3000;
            return false;
        case 3000:
            if(DoAOI_LoadFile(false))
                iTDCTask=4000;
            return false;
        case 4000:
            DoAOI_ClearBuffer(true);
            AOIDelay.SetSecAndOn(CommandDelay());
            iTDCTask=5000;
            return false;
        case 5000:
            if(DoAOI_ClearBuffer(false))
                iTDCTask=6000;
            return false;
        case 6000:
            DoAOI_Function(true);
            AOIDelay.SetSecAndOn(CommandDelay());
            iTDCTask=7000;
            return false;
        case 7000:
            if(DoAOI_Function(false))
                iTDCTask=9999;
            return false;
        case 9999: //finish
            return true;
        default:
            asError=AnsiString(__FUNC__)+"，Get Result wrong step="+IntToStr(iTDCTask);
            return true;
    }
}
//---------------------------------------------------------------------------
bool TfTrayMapFromLoader::DoGetValueAfterMotorArrival(bool bIsFirst,AnsiString &asError)
{
    if(bIsFirst)
    {
        iTDCTask=1;
        AOIDelay.SetSecAndOn(CommandDelay());
        return true;
    }

    if(AOIDelay.Off())
    {
        asError=AnsiString(__FUNC__)+" TimeOut "+IntToStr(CommandDelay())+" sec step="+IntToStr(iTDCTask);
    }

    if(asError!="")
        return true;
    switch(iTDCTask)
    {
        case 1:
            AOIDelay.SetSecAndOn(CommandDelay());
            iTDCTask=1000;
            return false;
        case 1000:
            if(DoCommuncation())
                iTDCTask=2000;
            return false;
        case 2000:
            DoGetValue(true);
            AOIDelay.SetSecAndOn(CommandDelay());
            iTDCTask=3000;
            return false;
        case 3000:
            if(DoGetValue(false))
                iTDCTask=9999;
            return false;
        case 9999: //finish
            socketClient->Close();
            return true;
        default:
            asError=AnsiString(__FUNC__)+"，Get Result wrong step="+IntToStr(iTDCTask);
            return true;
    }
}
//---------------------------------------------------------------------------
