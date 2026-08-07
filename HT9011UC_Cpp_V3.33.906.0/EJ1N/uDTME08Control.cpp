// =============================================================================
//  uDTME08Control.cpp  --  Panasonic DTME08 8-channel-per-station Modbus/TCP
//                           temperature controller driver.
//
//  Faithful translation of golden EJ1N/uDTME08Control.cpp (381 lines, BCB6,
//  Big5/cp950 -- carries Chinese error-description strings and one inline
//  comment).
//  Translator: AI(W906-PT-W2) 20260807
//  Translation wave: W906-PT-W2 group "ej1n".  Depends on this same wave's
//  own EJ1N/uSocketServerClient.cpp/.h (uSocketClient) and
//  EJ1N/uModbusCommand.cpp/.h (uModbusCommand) -- both translated in this
//  same wave; see this file's own uModbusCommand.h banner for why that unit
//  was read/understood FIRST even though it is translated last in the
//  assigned file order.
//
//  WAVE SCOPE -- ACTIVE, faithful, verbatim bodies -- ALL 26 golden function
//  definitions (golden line numbers), plus the header's 16 inline one-liners
//  (uDTME08Info's ctor/InitialDatas/SetSettingSV/SetCycleTime = 4;
//  uDTME08Control's GetSettingFilePath[GATE 1, see .h]/GetSettingFileName/
//  GetSettingFileNameWithPath/GetMinReceiveLen/GetMaxStationNumber/
//  GetChannelNumberPerStation/IsConnected/DoCommuncation/DoSocketClose/
//  GetSocketAddress/GetSocketPort/GetSizeDTMEInfo = 12):
//    uDTME08Control::uDTME08Control         golden :12-23
//    uDTME08Control::~uDTME08Control        golden :25-28
//    uDTME08Control::Initialization         golden :30-32 (EMPTY in golden itself)
//    uDTME08Control::GetStationCode         golden :34-38
//    uDTME08Control::ReceiveData            golden :40-44
//    uDTME08Control::ReadSettingFile        golden :46-49
//    uDTME08Control::WriteSettingFile       golden :51-54
//    uDTME08Control::SetSocketAddress       golden :56-59
//    uDTME08Control::SetSocketPort          golden :61-64
//    uDTME08Control::SendCommandSocket      golden :66-75
//    uDTME08Control::DoGetSV                golden :77-110
//    uDTME08Control::DoGetPV                golden :112-162
//    uDTME08Control::DoGetStatus            golden :164-198
//    uDTME08Control::DoSetValue             golden :200-225
//    uDTME08Control::DoSetSV                golden :227-238
//    uDTME08Control::DoSetAT                golden :240-251
//    uDTME08Control::DoSetSensorType        golden :253-264
//    uDTME08Control::DoSetCycleTime         golden :266-277
//    uDTME08Control::DoSetControlActionOut2 golden :279-290
//    uDTME08Control::Chararr2Hexstring      golden :292-298
//    uDTME08Control::AnalysisChannelStatus  golden :300-310
//    uDTME08Control::GetStatus              golden :312-319
//    uDTME08Control::GetFunctionCode        golden :321-343
//    uDTME08Control::ClearClientComm        golden :345-351
//    uDTME08Control::ErrorCodeDescription   golden :353-364
//    uDTME08Control::ReceiveInitial         golden :366-371
//    uDTME08Control::InitialmapDTMInfoList  golden :373-379
//   SATISFIED-BY-SUBSTRATE: uSocketClient (EJ1N/uSocketServerClient.cpp,
//     this same wave) and uModbusCommand (EJ1N/uModbusCommand.cpp, this same
//     wave) supply every socketClient->.../um.... call golden makes.
//
//  MINIMAL NECESSARY ADAPTATION (1, flagged inline with //AI at the line):
//   golden `socketClient->SetReceiveFunc(ReceiveData);` (golden :15) passes
//   a bound-member-function `__closure` implicitly binding `this`; the
//   port's TPointVoidReceive (uSocketServerClient.h) is std::function, so
//   this is wrapped in a lambda capturing `this` -- same translation choice
//   already used by uSocketServerClient.cpp's own OnConnect/OnRead/OnError/
//   OnDisconnect assignments.
//
//  GOLDEN QUIRK preserved verbatim, NOT fixed (rule: don't fix bugs, don't
//  improve arithmetic):
//   `ErrorCodeDescription` (golden :353-364) tests `bdata==0x02` TWICE --
//   once for "感測器斷線" (:356) and again, unreachably, for
//   "輸入資料未穩定" (:361, dead branch, `else if` chain never reaches it
//   because the :356 check already matched). Reproduced exactly, including
//   the unreachable branch and its string.
//
//  VCL/Borland conversions: #pragma hdrstop + #pragma package(smart_init)
//  dropped (golden :2 and :8). `um` (the file-scope `uModbusCommand um;`,
//  golden :10) kept as a genuine file-scope global, matching golden exactly
//  -- every DoGet*/DoSet*/ClearClientComm method reads/writes it directly,
//  same as golden. No __fastcall / __property / __published anywhere in
//  this .cpp (golden has none either). Numeric semantics, magic numbers
//  (the 0x000/0x250/0x268/0x288/0x028/0x0F8/0x0D0 Modbus register offsets,
//  the 0.1/0.0 PV/SV scale factors, `pow(16,3)`) kept EXACT.
//
//  Big5: every Chinese comment/string decoded via cp950 and preserved as
//  UTF-8 (ErrorCodeDescription's 7 diagnostic strings, the
//  "新增可讀寫負數"/"感溫線異常" attribution/label comments). Final gate:
//  ZERO U+FFFD.
// =============================================================================
// BCB6 ORIGINAL include block (mirrored as a comment for provenance):
//   #include "MachineDefine.h" ; #pragma hdrstop
//   #include "uDTME08Control.h"
//   #include "uModbusCommand.h" ; #pragma package(smart_init)
// =============================================================================
#include "MachineDefine.h"          // de-VCL'd include hub (vclcompat umbrella + <math.h>/<cstdlib>)
#include "EJ1N/uDTME08Control.h"    // this unit's own contract
#include "EJ1N/uModbusCommand.h"    // uModbusCommand (this same wave)
//AI(W906-PT-W2) 20260807: golden `__FUNC__` (BCB6 builtin, golden :73) --
//  same shim canary_support.h:45 already established tree-wide
//  (`#define __FUNC__ __func__`); reproduced locally, guarded, rather than
//  pulling in canary_support.h's much heavier LastSet surface for one macro.
#ifndef __FUNC__
#define __FUNC__ __func__
#endif
//---------------------------------------------------------------------------
uModbusCommand um;
//---------------------------------------------------------------------------
uDTME08Control::uDTME08Control()
{
    socketClient=new uSocketClient();
    //AI(W906-PT-W2) 20260807: golden `__closure` bound-method event assign
    //  (golden :15) implicitly binds `this`; wrapped in a lambda -- same
    //  translation choice as uSocketServerClient.cpp's own event wiring.
    socketClient->SetReceiveFunc([this](char* cGet, int iLen){ ReceiveData(cGet, iLen); });
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
