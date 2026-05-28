#include "MachineDefine.h"
#pragma hdrstop

#include "uDTME08Control.h"

#include "uModbusCommand.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
uModbusCommand um;
//---------------------------------------------------------------------------
uDTME08Control::uDTME08Control()
{
    socketClient=new uSocketClient();
    socketClient->SetReceiveFunc(ReceiveData);
    for(int i=0; i<GetChannelNumberPerStation(); i++)
    {
        mapDTMInfo[mapDTMInfo.size()]=uDTME08Info();
    }
    iCommandLen=0;
    bDebug=false;
    iReceiveLen=0;
}
//---------------------------------------------------------------------------
uDTME08Control::~uDTME08Control()
{
    delete socketClient;
}
//---------------------------------------------------------------------------
void uDTME08Control::Initialization()
{
}
//---------------------------------------------------------------------------
int uDTME08Control::GetStationCode(int istate)
{
    istate=(istate<0 || istate>GetMaxStationNumber())?0:istate;
    return istate*pow(16, 3);
}
//---------------------------------------------------------------------------
void uDTME08Control::ReceiveData(char* cGet, int iLen)
{
    cReceive=cGet;
    iReceiveLen=iLen;
}
//---------------------------------------------------------------------------
void uDTME08Control::ReadSettingFile()
{
    socketClient->ReadSettingFile(GetSettingFileNameWithPath());
}
//---------------------------------------------------------------------------
void uDTME08Control::WriteSettingFile()
{
    socketClient->WriteSettingFile(GetSettingFileNameWithPath());
}
//---------------------------------------------------------------------------
void uDTME08Control::SetSocketAddress(AnsiString asValue)
{
    socketClient->SetSocketAddress(asValue);
}
//---------------------------------------------------------------------------
void uDTME08Control::SetSocketPort(AnsiString asValue)
{
    socketClient->SetSocketPort(asValue);
}
//---------------------------------------------------------------------------
void uDTME08Control::SendCommandSocket(char* cSet, int iLen)
{
    iReceiveLen=0;
    bflag=socketClient->SendCommand(cSet, iLen);
    if(bflag)
        RecordMsg("[Send]"+Chararr2Hexstring(cSet,iLen));
    else
        RecordMsg("[Send Error]"+AnsiString(__FUNC__)+Chararr2Hexstring(cSet, iLen)+"[IP]"+socketClient->GetSocketAddress()+"[Port]"+socketClient->GetSocketPort());
    Sleep(5);
}
//---------------------------------------------------------------------------
bool uDTME08Control::DoGetSV(bool bIsFirst, int iStation)
{
    if(bIsFirst)
    {
        ClearClientComm();
        um.bisWrite=false;
        um.StartAddress=GetStationCode(iStation)+GetFunctionCode(efc_SetSV);
        um.GetClientEncodeSingleTCP(strSendUse, iCommandLen);
        SendCommandSocket(strSendUse, iCommandLen);
        return true;
    }
    else
    {
        if(iReceiveLen==0)
        {
            /*wait receive*/
        }
        else
        {
            if(ReceiveInitial())
                return true;
            //
            um.DecodeToListIntTCP(cReceive, iReceiveLen);
            for(int i=0; i<um.iListValueLen; i++)
            {
                if(i<GetChannelNumberPerStation())
                    mapDTMInfo[i].SV=(double)um.iListValue[i]*0.1;
            }
            iReceiveLen=0;
            return true;
        }
        return false;
    }
}
//---------------------------------------------------------------------------
bool uDTME08Control::DoGetPV(bool bIsFirst, int iStation)
{
    if(bIsFirst)
    {
        ClearClientComm();
        um.bisWrite=false;
        um.StartAddress=GetStationCode(iStation)+GetFunctionCode(efc_GetPV);
        um.GetClientEncodeSingleTCP(strSendUse, iCommandLen);
        SendCommandSocket(strSendUse, iCommandLen);
        return true;
    }
    else
    {
        if(iReceiveLen==0)
        {
            /*wait receive*/
        }
        else
        {
            if(ReceiveInitial())
                return true;
            for(int i=0; i<GetChannelNumberPerStation(); i++)
            {
                if(iReceiveLen<GetMinReceiveLen())
                {
                    tempStr=ErrorCodeDescription((byte)cReceive[iReceiveLen-1]);    //The Channel get Error
                    mapDTMInfo[i].PV=(double)tempInt*0.0;
                    mapDTMInfo[i].Event=tempStr;
                }
                else
                {
                    if((byte)cReceive[9+i*2]==0x80)
                    {
                        tempStr=ErrorCodeDescription((byte)cReceive[9+i*2+1]);    //The Channel get Error
                        mapDTMInfo[i].PV=(double)tempInt*0.0;
                        mapDTMInfo[i].Event=tempStr;
                    }
                    else
                    {
                        tempInt=um.TwoBytes2Short((byte)cReceive[9+i*2],(byte)cReceive[9+i*2+1] );   //Jimmychiu 20230216 新增可讀寫負數
                        mapDTMInfo[i].PV=(double)tempInt*0.1;
                        mapDTMInfo[i].Event="Event";
                    }
                }
            }
            iReceiveLen=0;
            return true;
        }
        return false;
    }
}
//---------------------------------------------------------------------------
bool uDTME08Control::DoGetStatus(bool bIsFirst, int iStation)
{
    if(bIsFirst)
    {
        ClearClientComm();
        um.bisWrite=false;
        um.StartAddress=GetStationCode(iStation)+GetFunctionCode(efc_GetStatus);
        um.GetClientEncodeSingleTCP(strSendUse, iCommandLen);
        SendCommandSocket(strSendUse, iCommandLen);
        return true;
    }
    else
    {
        if(iReceiveLen==0)
        {
            /*wait receive*/
        }
        else
        {
            if(ReceiveInitial())
                return true;
            um.DecodeToListIntTCP(cReceive, iReceiveLen);
            for(int i=0; i<um.iListValueLen; i++)
            {
                if(i<GetChannelNumberPerStation())
                {
                    GetStatus(um.iListValue[i], &mapDTMInfo[i]);
                }
            }
            iReceiveLen=0;
            return true;
        }
        return false;
    }
}
//---------------------------------------------------------------------------
bool uDTME08Control::DoSetValue(eFunctionCode fc, bool bIsFirst, int iStation)
{
    if(bIsFirst)
    {
        um.bisWrite=true;
        um.StartAddress=GetStationCode(iStation)+GetFunctionCode(fc);
        um.GetClientEncodeWriteMultipleTCP(strSendUse, iCommandLen);
        SendCommandSocket(strSendUse, iCommandLen);
        return true;
    }
    else
    {
        if(iReceiveLen==0)
        {
            /*wait receive*/
        }
        else
        {
            if(bDebug)
                RecordMsg(Chararr2Hexstring(cReceive,iReceiveLen));
            iReceiveLen=0;
            return true;
        }
        return false;
    }
}
//---------------------------------------------------------------------------
bool uDTME08Control::DoSetSV(bool bIsFirst, int iStation)
{
    if(bIsFirst)
    {
        um.ClearListValue();
        for(int i=0; i<GetChannelNumberPerStation(); i++)
        {
            um.AddListValue(mapDTMInfo[i].SetSV);
        }
    }
    return DoSetValue(efc_GetSV,bIsFirst, iStation);
}
//---------------------------------------------------------------------------
bool uDTME08Control::DoSetAT(bool bIsFirst, int iStation)
{
    if(bIsFirst)
    {
        um.ClearListValue();
        for(int i=0; i<GetChannelNumberPerStation(); i++)
        {
            um.AddListValue(mapDTMInfo[i].RunAutoTunning?eatTrunRunning:eatTrunStop);
        }
    }
    return DoSetValue(efc_SetAT, bIsFirst, iStation);
}
//---------------------------------------------------------------------------
bool uDTME08Control::DoSetSensorType(bool bIsFirst, int iStation)
{
    if(bIsFirst)
    {
        um.ClearListValue();
        for(int i=0; i<GetChannelNumberPerStation(); i++)
        {
            um.AddListValue(mapDTMInfo[i].SensorType);
        }
    }
    return DoSetValue(efc_SetSensorType, bIsFirst, iStation);
}
//---------------------------------------------------------------------------
bool uDTME08Control::DoSetCycleTime(bool bIsFirst,int iStation)
{
    if(bIsFirst)
    {
        um.ClearListValue();
        for(int i=0;i<GetChannelNumberPerStation();i++)
        {
            um.AddListValue(mapDTMInfo[i].CycleTime);
        }
    }
    return DoSetValue(efc_SetCycleTime,bIsFirst,iStation);
}
//---------------------------------------------------------------------------
bool uDTME08Control::DoSetControlActionOut2(bool bIsFirst,int iStation)
{
    if(bIsFirst)
    {
        um.ClearListValue();
        for(int i=0;i<GetChannelNumberPerStation();i++)
        {
            um.AddListValue(mapDTMInfo[i].ControlActionOut2);
        }
    }
    return DoSetValue(efc_SetControlActionOut2,bIsFirst,iStation);
}
//---------------------------------------------------------------------------
AnsiString uDTME08Control::Chararr2Hexstring(char* cstr, int iNum)
{
    tempStr="";
    for(int i=0; i<iNum; i++)
        tempStr+=" "+AnsiString().sprintf("%02X",(byte)cstr[i]);
    return tempStr;
}
//---------------------------------------------------------------------------
void uDTME08Control::AnalysisChannelStatus(int iInput, bool **arrBool, int iBoolLen)
{
    int inum=0;
    memset(*arrBool, 0x0, iBoolLen);
    while(iInput!=0 || inum<iBoolLen)
    {
        *arrBool[inum]=(iInput%2==1)?true:false;
        iInput/=2;
        inum++;
    }
}
//---------------------------------------------------------------------------
void uDTME08Control::GetStatus(int iInput, uDTME08Info *uinfo)
{
    bool* barray[8]={
                        &uinfo->Alarm3, &uinfo->Alarm2,         &uinfo->IsCelsius,      &uinfo->IsFahrenheit,
                        &uinfo->Alarm1, &uinfo->IsRunOutput2,   &uinfo->IsRunOutput1,   &uinfo->IsRunAutoTuning
                    };
    AnalysisChannelStatus(iInput, barray, 8);
}
//---------------------------------------------------------------------------
int uDTME08Control::GetFunctionCode(eFunctionCode fc)
{
    switch(fc)
    {
        case efc_SetSV:
            return 0x000;
        case efc_SetAT:
            return 0x250;
        case efc_GetSV:
            return 0x000;
        case efc_GetPV:
            return 0x268;
        case efc_GetStatus:
            return 0x288;
        case efc_SetSensorType:
            return 0x028;
        case efc_SetCycleTime:
            return 0x0F8;
        case efc_SetControlActionOut2:
            return 0x0D0;
    }
    return 0;
}
//---------------------------------------------------------------------------
void uDTME08Control::ClearClientComm()
{
    um.Clear();
    um.SlaveID=1;
    um.ClearListValue();
    um.AddListValue(GetChannelNumberPerStation());
}
//---------------------------------------------------------------------------
AnsiString uDTME08Control::ErrorCodeDescription(byte bdata)
{
         if(bdata==0x01) tempStr="EEPROM 無法寫入";
    else if(bdata==0x02) tempStr="感測器斷線";
    else if(bdata==0x03) tempStr="ADC 讀取失敗";
    else if(bdata==0x04) tempStr="內部通訊錯誤";
    else if(bdata==0x05) tempStr="輸入錯誤";
    else if(bdata==0x06) tempStr="通道禁能";
    else if(bdata==0x02) tempStr="輸入資料未穩定";
    else                 tempStr="未知異常";
    return  tempStr;
}
//---------------------------------------------------------------------------
bool uDTME08Control::ReceiveInitial()                     //JimmyChiu 20221208 : 功能待補
{
//    if(bDebug)
        RecordMsg(Chararr2Hexstring(cReceive, iReceiveLen));
    return false;
}
//---------------------------------------------------------------------------
void uDTME08Control::InitialmapDTMInfoList()
{
    for(int i=0; i<GetSizeDTMEInfo(); i++)
    {
        mapDTMInfo[i].InitialDatas();
    }
}
//---------------------------------------------------------------------------
