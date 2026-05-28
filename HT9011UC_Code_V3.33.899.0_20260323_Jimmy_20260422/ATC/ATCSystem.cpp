#include "MachineDefine.h"
#pragma hdrstop

#include "ATCSystem.h"
#include "mymessbox.h"
#include "cmydef.h"                                                             // 2012.02.02 , Joye ,  ATC改
#include "cprod.h"
#include "ATCInterface.h"
#include "ATC_Handler_Side.h"                                                   //Ifor 20151230 :add New ATC Interface
#include "common.h"

//ATC_COMMAND ------------------------------------------------------------------
ATC_COMMAND::ATC_COMMAND(int iType, AnsiString Command, int iDataPosition, int iDataLength, int iCommandLength)
{
    this->Command=Command;
    this->iDataPosition=iDataPosition;
    this->iDataLength=iDataLength;
    this->iCommandLength=iCommandLength;
    this->iType=iType;
};
// HT ATC ----------------------------------------------------------------------
HT_ATC::HT_ATC(TCPIP* pSocket , int iNumber)
{
    InitializeCriticalSection( &csHistory );
    InitializeCriticalSection( &csReadData );

    _socket = pSocket;
    _socket_thread.Initial(HT_ATC_TCPIP_WATCH, this);
    _number = iNumber;
    ATC_ProcessReceiveCMD="";
    AnsiString Str="";

    //ARD_NONE  --> 不做任何動作
    //ARD_VALUE --> 資料交換,根據資料型別存入
    //ARD_ALARM --> 異常需要Alarm的部分, 要顯示的文字寫在 sData 裡面
                                           //{Type, Command, iDataPosition, iDataLength, iCommandLength};
    Str.sprintf("CONNECT %d OK",                    iNumber+1);                                                                             //A->H 10
    ATC_ReceiveCMD[ATC_CONNECT_X_OK                 ]=(new ATC_COMMAND(ARD_VALUE, Str, 0, 0, Str.Length()));
    Str.sprintf("ATC_SETUP_TEMP_OK_%02d",           iNumber+1);                                                                             //A->H 11
    ATC_ReceiveCMD[ATC_SETUP_TEMP_OK_XX             ]=(new ATC_COMMAND(ARD_VALUE, Str, 0, 0, Str.Length()));
    Str.sprintf("ATC_OFFSET_OK_%02d",               iNumber+1);                                                                             //A->H 13
    ATC_ReceiveCMD[ATC_OFFSET_OK_XX                 ]=(new ATC_COMMAND(ARD_VALUE, Str, 0, 0, Str.Length()));
    Str.sprintf("ATC_TEMP_READY_%02d",              iNumber+1);                                                                             //A->H 06
    ATC_ReceiveCMD[ATC_TEMP_READY_XX                ]=(new ATC_COMMAND(ARD_VALUE, Str, 0, 0, Str.Length()));
    Str.sprintf("#SITE%d:",                         iNumber+1);

    ATC_ReceiveCMD[ATC_SITE_XXXX                    ]=(new ATC_COMMAND(ARD_VALUE, Str,  Str.Length()+1, 4, Str.Length()+5));
    ATC_ReceiveCMD[ATC_SITE_STATE_X                 ]=(new ATC_COMMAND(ARD_VALUE, "ATC_SITE_STATE",          16, 1, 16));                   //A->H 21
    ATC_ReceiveCMD[ATC_WORK_TEMP_XXXX               ]=(new ATC_COMMAND(ARD_VALUE, "ATC_WORKTEMP",            14, 4, 18));                   //A->H 22
    ATC_ReceiveCMD[ATC_CHILLER_TEMP_XXXX            ]=(new ATC_COMMAND(ARD_VALUE, "ATC_CHILLERTEMP",         17, 4, 21));                   //A->H 23
//    ATC_ReceiveCMD[ATC_RUN_ATC_FUNCTION_OK          ]=(new ATC_COMMAND(ARD_VALUE, "RUN_ATC_FUNCTION_OK",     0, 0, 18));                    //A->H 15
    ATC_ReceiveCMD[ATC_RUN_ATC_FUNCTION_OK          ]=(new ATC_COMMAND(ARD_VALUE, "RUN_ATC_FUNCTION_OK",     0, 0, 19));                    //A->H 15  //Steven 20140617 : 18 --> 19
    ATC_ReceiveCMD[ATC_STOP_ATC_FUNCTION_OK         ]=(new ATC_COMMAND(ARD_VALUE, "STOP_ATC_FUNCTION_OK",    0, 0, 20));                    //A->H 16  //Steven 20140617 : 18 --> 20
    ATC_ReceiveCMD[ATC_RUN_CHILLER_OK               ]=(new ATC_COMMAND(ARD_VALUE, "RUN_Chiller_OK",          0, 0, 18));                    //A->H 17
    ATC_ReceiveCMD[ATC_STOP_CHILLER_OK              ]=(new ATC_COMMAND(ARD_VALUE, "STOP_Chiller_OK",         0, 0, 15));                    //A->H 18
    ATC_ReceiveCMD[ATC_TEMP_NOT_READY               ]=(new ATC_COMMAND(ARD_VALUE, "ATC_TEMP_NOT_READY",      0, 0, 18));                    //A->H 07
    //ATC_ReceiveCMD[ATC_TEMP_READY                   ]=(new ATC_COMMAND(ARD_VALUE, "ATC_TEMP_READY",          0, 0, 14));                    //A->H 06
    ATC_ReceiveCMD[ATC_CHILLER_HAS_NOT_STARTED_ERROR]=(new ATC_COMMAND(ARD_SYS_ALARM, "CHILLER_Has_Not_Started_Error", 0, 0, 27));          //A->H 20
    ATC_ReceiveCMD[ATC_CHILLER_CONNECT_ERROR        ]=(new ATC_COMMAND(ARD_SYS_ALARM, "Chiller_Connect_Error",   0, 0, 21));                //A->H 12
    ATC_ReceiveCMD[ATC_WATER_FLOW_ERROR             ]=(new ATC_COMMAND(ARD_SYS_ALARM, "Water_Flow_Error",        0, 0, 16));                //A->H 09
    ATC_ReceiveCMD[ATC_POWER_SUPPLY_OFF             ]=(new ATC_COMMAND(ARD_ALARM, "ATC_POWER_SUPPLY_OFF",    0, 0, 20));                    //A->H 04
    ATC_ReceiveCMD[ATC_LEAK_WATER                   ]=(new ATC_COMMAND(ARD_ALARM, "ATC_LEAK_WATER",          0, 0, 14));                    //A->H 05
    ATC_ReceiveCMD[ATC_TEMP_BELOW                   ]=(new ATC_COMMAND(ARD_ALARM, "ATC_TEMP_BELOW",          0, 0, 14));                    //A->H 08
    ATC_ReceiveCMD[ATC_READ_TEN_SAME_TEMP           ]=(new ATC_COMMAND(ARD_ALARM, "Read_Ten_same_Temp_Continuously", 0, 0, 31));            //A->H 19
    ATC_ReceiveCMD[ATC_SETCHILLERTEMP_OK            ]=(new ATC_COMMAND(ARD_NONE,  "ATC_SetChillerTemp_OK",   0, 0, 21));                    //A->H 14
    ATC_ReceiveCMD[ATC_NOW_SETUP_TEMP               ]=(new ATC_COMMAND(ARD_NONE,  "ATC_NOW_SETUP_TEMP",      0, 0, 18));                    //A->H 01
    ATC_ReceiveCMD[ATC_HANDLER_TESTER_POWEROFF      ]=(new ATC_COMMAND(ARD_NONE,  "HANDLER_TESTER_POWEROFF", 0, 0, 23));                    //A->H 02
    ATC_ReceiveCMD[ATC_DISABLE                      ]=(new ATC_COMMAND(ARD_SYS_ALARM,  "ATC_DISABLE",             0, 0, 11));                    //A->H 03
    ATC_ReceiveCMD[ATC_WATER_LEVEL_OVERLOW          ]=(new ATC_COMMAND(ARD_SYS_ALARM, "Water_Level_OverLow", 0, 0, 19));                    //A->H 24
    ATC_ReceiveCMD[ATC_WATER_TEMPERATURE_OVERHIGH   ]=(new ATC_COMMAND(ARD_SYS_ALARM, "WATER_TEMPERATURE_OVERHIGH", 0, 0, 26));             //A->H 25
    ATC_ReceiveCMD[ATC_WATER_FLOW_TEMP_OVERHIGH     ]=(new ATC_COMMAND(ARD_SYS_ALARM, "Water_Flow_Temp_OverHigh", 0, 0, 24));               //A->H 24
    ATC_ReceiveCMD[ATC_SETUP_ARM_OK                 ]=(new ATC_COMMAND(ARD_VALUE, "@SETUP_ARM_OK+",          0, 0, 13));

    //Ifor 20151023 Add ATC 2.0 讀取ATC軟體版本
    ATC_ReceiveCMD[ATC_READ_SW_VER                  ]=(new ATC_COMMAND(ARD_VALUE, "@READ_ATC_SW_VER",  0, 0, 34));        //A->H 27
    //Ifor 20151023 Add ATC 2.0 設定ATC電腦時間同步
    ATC_ReceiveCMD[ATC_SET_TIME_SYNC_OK             ]=(new ATC_COMMAND(ARD_VALUE, "@SET_ATC_TIME_SYNC_OK+",              0, 0, 22));

    //Ifor 20160427 Add ATC 2.0 設定第二點Sensor Function On/Off
    ATC_ReceiveCMD[ATC_SET_DUAL_TEMP_OK             ]=(new ATC_COMMAND(ARD_VALUE, "@SET_DUAL_TEMP_FUN_OK",              0, 0, 21));

    //Ifor 20160718
    ATC_ReceiveCMD[ATC_WATER_TEMPERATURE_OVERLOW    ]=(new ATC_COMMAND(ARD_SYS_ALARM, "WATER_TEMPERATURE_OVERLOW",   0, 0, 25));
    ATC_ReceiveCMD[ATC_WATER_PRESSURE_OVERHIGH      ]=(new ATC_COMMAND(ARD_SYS_ALARM, "WATER_PRESSURE_OVERHIGH",   0, 0, 23));
    ATC_ReceiveCMD[ATC_WATER_PRESSURE_OVERLOW       ]=(new ATC_COMMAND(ARD_SYS_ALARM, "WATER_PRESSURE_OVERLOW",   0, 0, 22));
    ATC_ReceiveCMD[ATC_WRITE_FIVE_SAME_TEMPERATURE  ]=(new ATC_COMMAND(ARD_SYS_ALARM, "WRITE_FIVE_SAME_TEMPERATURE",   0, 0, 27));
    ATC_ReceiveCMD[ATC_Refer_Temp_Alarm             ]=(new ATC_COMMAND(ARD_SYS_ALARM, "Refer_Temp_Alarm",   0, 0, 16));
    ATC_ReceiveCMD[ATC_ALARM_TEMP                   ]=(new ATC_COMMAND(ARD_SYS_ALARM, "ATC_ALARM_TEMP",   0, 0, 14));
    ATC_ReceiveCMD[ATC_TEMP_OVER_HIGH               ]=(new ATC_COMMAND(ARD_SYS_ALARM, "ATC_TEMP_OVER_HIGH",   0, 0, 18));
    //Ifor 20160724 add ATC 2.0 Self Test Receive CMD
    ATC_ReceiveCMD[ATC_SELFTEST_RESULT_NG           ]=(new ATC_COMMAND(ARD_VALUE, "ATC_SELFTEST_RESULT_NG",   0, 0, 22));
    ATC_ReceiveCMD[ATC_SELFTEST_RESULT_OK           ]=(new ATC_COMMAND(ARD_VALUE, "ATC_SELFTEST_RESULT_OK",   0, 0, 22));
    ATC_ReceiveCMD[ATC_READ_STATUS                  ]=(new ATC_COMMAND(ARD_VALUE, "<<ATCSTATUS",  0, 0, 20));        //A->H 27
    ATC_ReceiveCMD[ATC_SELFTEST_STATUS              ]=(new ATC_COMMAND(ARD_VALUE, "ATC_SELFTEST_STATUS",   0, 0, 22));

    ATC_ReceiveCMD[ATC_LotStart_STATUS              ]=(new ATC_COMMAND(ARD_VALUE, "START_LOT[",   0, 0, 20));  //Ifor 20161118 add ATC2.0 Lot Start Sataus  //Ifor 20170103 (Steven) 資料格式修改<<" ">> => [" "]
    ATC_ReceiveCMD[ATC_LotEnd_STATUS                ]=(new ATC_COMMAND(ARD_VALUE, "END_LOT[",   0, 0, 18));    //Ifor 20161118 add ATC2.0 Lot End Sataus    //Ifor 20170103 (Steven) 資料格式修改<<" ">> => [" "]


    for(unsigned int i=0; i<ATC_MAX_CMD; i++)
    {
        ATC_ReceiveCMD[i]->Command=ATC_ReceiveCMD[i]->Command.UpperCase();
    }
}
//------------------------------------------------------------------------------
HT_ATC::~HT_ATC()
{
    try
    {
        for(int i=ATC_MAX_CMD-1; i>=0; i--)                                     //Steven 20120623 : ATC解構
        {
            if(ATC_ReceiveCMD[i]!=NULL)                                         //Steven 20161220 (jou) : 修正delete方式
                delete ATC_ReceiveCMD[i];
        }

        delete _socket;
        _socket_thread.CloseThread(1000);
        DeleteCriticalSection(&csHistory);
        DeleteCriticalSection(&csReadData);
    }
    catch(...)
    {
        MyDBIProcess("Exception", "~HT_ATC()");
    }
}
//------------------------------------------------------------------------------
//Ifor 20150910 ATC PID
void ATCSystem::SetPIDParameter(double fMinP, double fMinI, double fMinD, double fMaxP, double fMaxI, double fMaxD)
{
    ((HT_ATC*)(Channels[0]))->SetPID(fMinP, fMinI, fMinD, fMaxP, fMaxI, fMaxD);
}
//------------------------------------------------------------------------------
//Ifor 20150910 ATC PID
void HT_ATC::SetPID(double fMinP, double fMinI, double fMinD, double fMaxP, double fMaxI, double fMaxD)
{
    string sSendData;

    char buffer[100];
    //@SETUP_ATC_PIDΔ0.00Δ0.00Δ0.00Δ0.00Δ0.00Δ0.00+
    sprintf( buffer ,
             "@SETUP_ATC_PID %.2f %.2f %.2f %.2f %.2f %.2f+" ,
             fMaxP, fMaxI, fMaxD,
             fMinP, fMinI, fMinD);

    sSendData=buffer;
    SendCommand(sSendData);
}
// virtual function ---------------------------------------------------------->>
void HT_ATC::Connect()
{
    _socket_thread.OpenThread();
    _socket->Connect();
}
//------------------------------------------------------------------------------
void HT_ATC::Disconnect()
{
    _socket->DisConnect();
    _socket_thread.CloseThread( 5000);
}
//---------------------------------------------------------------------------
bool HT_ATC::IsConnect()
{
    return _socket->IsConnect();
}
//---------------------------------------------------------------------------
void HT_ATC::ClearHistory()
{
    EnterCriticalSection( &csHistory );
    _history.clear();
    LeaveCriticalSection( &csHistory );
}
//---------------------------------------------------------------------------
void HT_ATC::PushHistoryString(string log)
{
    EnterCriticalSection(&csHistory);
    int len = log.length();
    log[len] = '\0';
    _history.push_back( log);
    LeaveCriticalSection(&csHistory);
}
//---------------------------------------------------------------------------
void HT_ATC::PushHistoryString(string sType, DATA Data)
{
    EnterCriticalSection(&csHistory);
    string sHistory(sType);
    int iSize=(int)Data.size();
    for(int i=0; i<iSize; i++)                                                  //jou 2014-11-13 <= -> < ATC記憶體破壞
    {
        sHistory += Data[i];
//        if(i == iSize)
//            sHistory += '\0';
    }
    sHistory+='\0';
    _history.push_back(sHistory);
    LeaveCriticalSection(&csHistory);
}
//---------------------------------------------------------------------------
bool HT_ATC::PopHistoryString(string& log)
{
    EnterCriticalSection(&csHistory);
    int iSize=(int)_history.size();

    if(iSize>0)
    {
        log=_history[0];
        _history.erase(_history.begin(), _history.begin()+1);
        LeaveCriticalSection(&csHistory);
        return true;
    }
    LeaveCriticalSection(&csHistory);
    return false;
}
//---------------------------------------------------------------------------
int HT_ATC::SendCommand(DATA SendData)
{
    MySleep(10);    // Send Dealy
    return _socket->Send(SendData);
}
//---------------------------------------------------------------------------
int HT_ATC::SendCommand(string SendData)
{
    MySleep(10);    // Send Dealy
    return _socket->Send(SendData);
}
// virtual function ----------------------------------------------------------<<
// normol function ----------------------------------------------------------->>
void HT_ATC::ClearReadData()
{
    EnterCriticalSection( &csReadData );
    _history.clear();
    LeaveCriticalSection( &csReadData );
}
//---------------------------------------------------------------------------
void HT_ATC::PushReadData( ATCData aData )
{
    EnterCriticalSection( &csReadData );
    _read_data.push_back( aData );
    LeaveCriticalSection( &csReadData );
}
//---------------------------------------------------------------------------
bool HT_ATC::PopReadData( ATCData& aData )
{
    bool bReturn(false);
    EnterCriticalSection( &csReadData );
    int iSize = (int)_read_data.size();

    if(iSize > 0)
    {
        aData = _read_data[0];
        _read_data.erase( _read_data.begin(), _read_data.begin()+1);

        bReturn = true;
    }
    LeaveCriticalSection( &csReadData );
    return bReturn;
}
//------------------------------------------------------------------------------
void HT_ATC::InitialSocket(string sAddress, int iPort)
{
    _socket->Initial(sAddress, iPort);
}
//------------------------------------------------------------------------------
int HT_ATC::AdapterData(DATA Data)                                              //接收資料
{
    unsigned int iSize=Data.size();
    static int iATCIndex=0;
    for( unsigned int i=0; i<iSize; i++)
    {
        ATC_ProcessReceiveCMD+=(char)Data[i];
    }
    ATC_ProcessReceiveCMD=ATC_ProcessReceiveCMD.UpperCase();
    AnsiString Log="";
    int iTemp, iTemp2, iTemp3;
    int iStart=0;
    int iStatus=0;
    bool bHasTempIndex=false;                                                   //Ifor 20161006 判斷ATC 版本並是否支援溫度索引值
    int iCheckATCVer=0;
    for(unsigned int i=0; i<ATC_MAX_CMD; i++)
    {
        iStart=ATC_ProcessReceiveCMD.Pos(ATC_ReceiveCMD[i]->Command);
        if(iStart!=0)
        {
            char buffer[100];
            int  iFlag=-1;
            ATCData aData;
            aData.sCommand=ATC_ReceiveCMD[i]->Command.c_str();
            ATC_ReceiveCMD[i]->iCommandLength=ATC_ProcessReceiveCMD.Length();
            switch(i)
            {
                case ATC_SITE_XXXX:
                    iFlag=ATC_SITE_XXXX;
                    ATC_ReceiveCMD[i]->iType=ARD_VALUE;
                    if(Temperature.bUseReferTempSensor==true)                   //Ifor 20151006 ATC 第二組溫度開啟
                    {
                        iTemp=ATC_ProcessReceiveCMD.Pos(":")+1;
                        iTemp2=ATC_ProcessReceiveCMD.Pos(";");
                        iTemp3=ATC_ProcessReceiveCMD.Pos("%");
                        aData.fData=atof(ATC_ProcessReceiveCMD.SubString(iTemp, iTemp2-iTemp).c_str());
                        aData.fReftemp=atof(ATC_ProcessReceiveCMD.SubString(iTemp2+1, iTemp3-iTemp2+1).c_str());
                        iATCIndex=atoi(ATC_ProcessReceiveCMD.SubString(iTemp3+2, 5).c_str());//Ifor 20160720 Add Temp Index
                    }
                    else
                    {
                        iTemp=ATC_ProcessReceiveCMD.Pos(":")+1;
                        iTemp2=ATC_ProcessReceiveCMD.Pos("%");
                        aData.fData=atof(ATC_ProcessReceiveCMD.SubString(iTemp, iTemp2-iTemp).c_str());
                        iATCIndex=atoi(ATC_ProcessReceiveCMD.SubString(iTemp2+2, 5).c_str());//Ifor 20160720 Add Temp Index
                    }

                    if(iATCForHSMode==1)                                        //海思ATC版本 HISI_ATC2X_V02.00.K005            //Ifor 20200914 Fix HISI_ATC2X_V02.00 -> KL_ATC2X_V02.00
                    {                                                           //Ifor 20161006 Add 判斷ATC 2.0版本是否支援 Temp Index Check 機制
                        iCheckATCVer=atoi(sATCVerRead.SubString(11, 2).c_str())*100;            //Ifor 20161108 add 取得ATC2.0大版號    //Ifor 20200914 Fix: HISI -> KL 長度減2 13>11
                        iCheckATCVer=iCheckATCVer+atoi(sATCVerRead.SubString(14, 2).c_str());   //Ifor 20161108 add 取得ATC2.0小版號    //Ifor 20200914 Fix: HISI -> KL 長度減2 16>14
                        if(iCheckATCVer>=200)                                                   //Ifor 20161108 2 -> 200
                            bHasTempIndex=true;
                    }
                    else if(iATCForHSMode==2)                                   //非海思ATC版本 "V3.23A"
                    {
                        iCheckATCVer = atoi(sATCVerRead.SubString(2,1).c_str())*100;            //Ifor 20161006 取ATC2.0 一般本大版號
                        iCheckATCVer = iCheckATCVer + atoi(sATCVerRead.SubString(4,2).c_str()); //Ifor 20161006 取ATC2.0 一般本小版號
                        if(iCheckATCVer>=323)                                                   //Ifor 20161006 ATC2.0 V3.23A 後開始有支援
                            bHasTempIndex=true;
                    }

                    if(bHasTempIndex)
                    {
                        if(iATC_TempIndex[_number][OLD_VALUES]==iATCIndex)      //Ifor 20160719 add ATC first Temp difference Count //Ifor 20160805 add Use Site
                        {
                            iATC_TempIndex[_number][SAME_COUNT]++;
                        }
                        else
                        {
                            iATC_TempIndex[_number][OLD_VALUES]=iATCIndex;
                            iATC_TempIndex[_number][SAME_COUNT]=0;
                        }

                        if(iATC_TempIndex[_number][SAME_COUNT]>=3)              //Ifor 20160719 ATC alarm: ATC temperature Index always same error
                        {
                            sprintf(buffer, "WAR15319, CH%02d", _number+1);     //Ifor 20160721 ATC alarm: ATC temperature Index always same error
                            ATC_ReceiveCMD[i]->iType=ARD_SYS_ALARM;
                            aData.sData=buffer;
                        }
                    }
                    break;
                case ATC_NOW_SETUP_TEMP:                                        //QQ
                    iFlag=ATC_NOW_SETUP_TEMP;
                    break;
                case ATC_HANDLER_TESTER_POWEROFF:                               //QQ
                    iFlag=ATC_HANDLER_TESTER_POWEROFF;
                    break;
                case ATC_POWER_SUPPLY_OFF:
                    iFlag=ATC_POWER_SUPPLY_OFF;
                    sprintf(buffer, "WAR15238, CH%02d", _number+1);             //ATC_POWER_SUPPLY_OFF
                    aData.sData=buffer;
                    break;
                case ATC_LEAK_WATER:
                    iFlag=ATC_LEAK_WATER;
                    aData.sData="WAR15241, ";                                   //ATC_LEAK_WATER
                    break;
                case ATC_TEMP_READY_XX:
                    iFlag=ATC_TEMP_READY_XX;
                    aData.bData = true;
                    break;
                case ATC_TEMP_NOT_READY:
                    iFlag=ATC_TEMP_NOT_READY;
                    aData.bData = false;
                    break;
                //case ATC_TEMP_READY:
                    //iFlag=ATC_TEMP_READY;
                    //aData.bData = true;
                    //break;
                case ATC_TEMP_BELOW:
                    iFlag=ATC_TEMP_BELOW;
                    sprintf(buffer, "WAR15240, CH%02d", _number+1);             //ATC_TEMP_BELOW
                    aData.sData=buffer;
                    break;
                case ATC_WATER_FLOW_ERROR:
                    iFlag=ATC_WATER_FLOW_ERROR;
                    sprintf(buffer, "WAR15236, CH%02d", _number+1);             //ATC2.0 system alarm: Water flow error
                    aData.sData=buffer;
                    break;
                case ATC_CONNECT_X_OK:
                    iFlag=ATC_CONNECT_X_OK;
                    sprintf(buffer, "ATC Channel %02d Connect ok!", _number+1);
                    aData.sData = buffer;
                    aData.bData = true;
                    break;
                case ATC_SETUP_TEMP_OK_XX:
                    iFlag=ATC_SETUP_TEMP_OK_XX;
                    aData.sData = "OK";
                    break;
                case ATC_CHILLER_CONNECT_ERROR:
                    iFlag=ATC_CHILLER_CONNECT_ERROR;
                    aData.sData = "WAR15235, ";                                 //Chiller Connect Error!!
                    break;
                case ATC_OFFSET_OK_XX:
                    iFlag=ATC_OFFSET_OK_XX;
                    aData.bData = true;
                    break;
                case ATC_SETCHILLERTEMP_OK:                                     //QQ
                    iFlag=ATC_SETCHILLERTEMP_OK;
                    break;
                case ATC_RUN_ATC_FUNCTION_OK:
                    iFlag=ATC_RUN_ATC_FUNCTION_OK;
                    aData.bData = true;
                    break;
                case ATC_STOP_ATC_FUNCTION_OK:
                    iFlag=ATC_STOP_ATC_FUNCTION_OK;
                    aData.bData = true;
                    break;
                case ATC_RUN_CHILLER_OK:
                    iFlag=ATC_RUN_CHILLER_OK;
                    //_Chiller_Run=true;
                    aData.bData = true;
                    break;
                case ATC_STOP_CHILLER_OK:
                    iFlag=ATC_STOP_CHILLER_OK;
                    aData.bData = true;
                    break;
                case ATC_READ_TEN_SAME_TEMP:                                    //QQ
                    iFlag=ATC_READ_TEN_SAME_TEMP;
                    sprintf(buffer, "WAR15207, CH%02d", _number+1);             //Read_Ten_same_Temp_Continuously
                    aData.sData = buffer;
                    break;
                case ATC_CHILLER_HAS_NOT_STARTED_ERROR:
                    iFlag=ATC_CHILLER_HAS_NOT_STARTED_ERROR;
                    aData.sData="WAR15234, ";                                   //Chiller has not started error
                    break;
                case ATC_SITE_STATE_X:
                    iFlag=ATC_SITE_STATE_X;
                    aData.fData=atof(ATC_ProcessReceiveCMD.SubString(ATC_ReceiveCMD[i]->iDataPosition, ATC_ReceiveCMD[i]->iDataLength).c_str());
                    break;
                case ATC_WORK_TEMP_XXXX:
                    iFlag=ATC_WORK_TEMP_XXXX;
                    aData.fData=atof(ATC_ProcessReceiveCMD.SubString(ATC_ReceiveCMD[i]->iDataPosition, ATC_ReceiveCMD[i]->iDataLength).c_str())/10.0;
                    break;
                case ATC_CHILLER_TEMP_XXXX:
                    iFlag=ATC_CHILLER_TEMP_XXXX;
                    aData.fData=atof(ATC_ProcessReceiveCMD.SubString(ATC_ReceiveCMD[i]->iDataPosition, ATC_ReceiveCMD[i]->iDataLength).c_str())/10.0;
                    break;
                case ATC_WATER_LEVEL_OVERLOW:
                    iFlag=ATC_WATER_LEVEL_OVERLOW;
                    sprintf(buffer, "WAR15212, CH%02d", _number+1);             //ATC Channel %02d Water Level OverLow Error!
                    aData.sData=buffer;
                    break;
                case ATC_WATER_TEMPERATURE_OVERHIGH:
                    iFlag=ATC_WATER_TEMPERATURE_OVERHIGH;
                    sprintf(buffer, "WAR15213, CH%02d", _number+1);             //ATC Channel %02d Water Temperature OverHigh Error!
                    aData.sData=buffer;
                    break;
                case ATC_WATER_FLOW_TEMP_OVERHIGH:
                    iFlag=ATC_WATER_FLOW_TEMP_OVERHIGH;
                    sprintf(buffer, "WAR15237, CH%02d", _number+1);             //ATC Water Temperature OverHigh Error!
                    aData.sData=buffer;
                    if(bATCWaterTempAlarm==true)                                //Ifor 20160625 防止 ATC 連續送出 Water Temperature Error Alarm 造成無法關閉
                        iFlag=-1;
                    else
                        bATCWaterTempAlarm=true;
                    break;
                case ATC_SETUP_ARM_OK:
                    iFlag=ATC_SETUP_ARM_OK;
                    aData.bData = true;
                    break;
                case ATC_READ_SW_VER:                                           //Ifor 20151023 Add ATC 2.0 讀取ATC軟體版本
                    iFlag=ATC_READ_SW_VER;
                    try                                                         //Ifor 20161006 add 取得ATC2.0 海思版號 & 一般版號
                    {
//                        if(ATC_ProcessReceiveCMD.Pos("_HS")>=1 || ATC_ProcessReceiveCMD.Pos("HISI")>=1 || ATC_ProcessReceiveCMD.Pos("Hisi")>=1)
                        if(ATC_ProcessReceiveCMD.Pos("KL")>=1)                  //Ifor 20200914 Fix: HISI => KL
                        {
                            iATCForHSMode=1;
                            iTemp=ATC_ProcessReceiveCMD.Pos("KL");
                            iTemp2=ATC_ProcessReceiveCMD.Pos("+");
                            aData.sATCSwVer=ATC_ProcessReceiveCMD.SubString(iTemp, iTemp2-iTemp).c_str();
                        }
                        else
                        {
                            iATCForHSMode=2;
                            iTemp2=ATC_ProcessReceiveCMD.Pos("+");
                            aData.sATCSwVer=ATC_ProcessReceiveCMD.SubString(18, iTemp2-18).c_str();
                        }
                    }
                    catch(...)
                    {
                        MyDBIProcess("Exception", "HT_ATC::AdapterData");
                    }
//                    sATCVerRead=aData.sATCSwVer.c_str();                      //Ifor 20160318 ATC 版本
                    break;
                case ATC_SET_TIME_SYNC_OK:                                      //Ifor 20151023 Add ATC 2.0 設定ATC電腦時間同步
                    iFlag=ATC_SET_TIME_SYNC_OK;
                    break;
                case ATC_SET_DUAL_TEMP_OK:                                      //Ifor 20160427 Add ATC 2.0 設定第二點Sensor Function On/Off
                    iFlag=ATC_SET_DUAL_TEMP_OK;
                    break;
                case ATC_WATER_TEMPERATURE_OVERLOW:                             //Ifor 20160718 add ATC2.0 溫度過低
                    iFlag=ATC_WATER_TEMPERATURE_OVERLOW;
                    sprintf(buffer, "WAR15214, CH%02d", _number+1);
                    aData.sData=buffer;
                    break;
                case ATC_WATER_PRESSURE_OVERHIGH:                               //Ifor 20160718 add ATC2.0 壓力過高
                    iFlag=ATC_WATER_PRESSURE_OVERHIGH;
                    sprintf(buffer, "WAR15215, CH%02d", _number+1);             //Ifor 20160721 Water_pressure_OverHigh
                    aData.sData=buffer;
                    break;
                case ATC_WATER_PRESSURE_OVERLOW:                                //Ifor 20160718 add ATC2.0 壓力過低
                    iFlag=ATC_WATER_PRESSURE_OVERLOW;
                    sprintf(buffer, "WAR15216, CH%02d", _number+1);             //Ifor 20160721 Water_pressure_OverLow
                    aData.sData=buffer;
                    break;
                case ATC_WRITE_FIVE_SAME_TEMPERATURE:                           //Ifor 20160718 add ATC2.0 溫度寫入檔案五次相同
                    iFlag=ATC_WRITE_FIVE_SAME_TEMPERATURE;
                    sprintf(buffer, "WAR15233, CH%02d", _number+1);
                    aData.sData=buffer;
                    break;
                case ATC_Refer_Temp_Alarm:                                      //Ifor 20160718 add ATC2.0 第二點溫度異常
                    iFlag=ATC_Refer_Temp_Alarm;
                    sprintf(buffer, "WAR15208, CH%02d", _number+1);
                    aData.sData=buffer;
                    break;
                case ATC_ALARM_TEMP:                                            //Ifor 20160720 add ATC2.0 溫度超過最大值
                    iFlag=ATC_ALARM_TEMP;
                    sprintf(buffer, "WAR15206, CH%02d", _number+1);
                    aData.sData=buffer;
                    break;
                case ATC_TEMP_OVER_HIGH:                                        //Ifor 20160721 add ATC2.0 溫度超過最大值
                    iFlag=ATC_TEMP_OVER_HIGH;
                    sprintf(buffer, "WAR15242, CH%02d", _number+1);
                    aData.sData=buffer;
                    break;
                case ATC_DISABLE:                                               //Ifor 20160721 add ATC2.0 溫度超過最大值
                    iFlag=ATC_DISABLE;
                    sprintf(buffer, "WAR15239, CH%02d", _number+1);
                    aData.sData=buffer;
                    break;
                case ATC_SELFTEST_RESULT_NG:                                    //Ifor 20160720 add ATC2.0 ATC_SELFTEST_RESULT_NG+5 ATC自我測試+5C結果失敗
                    iFlag=ATC_SELFTEST_RESULT_NG;
                    if(ATC_ProcessReceiveCMD.Pos("+5")>=1)
                    {
                        aData.iATCSelfTestResult=1;
                    }
                    else if(ATC_ProcessReceiveCMD.Pos("-5")>=1)
                    {
                        aData.iATCSelfTestResult=2;
                    }
                    else if(ATC_ProcessReceiveCMD.Pos("+0")>=1)
                    {
                        aData.iATCSelfTestResult=3;
                    }
                    else if(ATC_ProcessReceiveCMD.Pos("99")>=1)
                    {
                        aData.iATCSelfTestResult=4;
                    }
                    break;
                case ATC_SELFTEST_RESULT_OK:                                    //Ifor 20160720 add ATC2.0 ATC_SELFTEST_RESULT_OK ATC自我測試結果結果成功
                    iFlag=ATC_SELFTEST_RESULT_OK;
                    aData.iATCSelfTestResult=5;
                    break;
                case ATC_READ_STATUS:                                           //Ifor 20160720 add ATC2.0 溫度超過最大值
                    iFlag=ATC_READ_STATUS;
                    aData.strATCStatus=ATC_ProcessReceiveCMD.SubString(13,5);
                    break;
                case ATC_SELFTEST_STATUS:                                       //Ifor 20160824 add ATC2.0 ATC_SELFTEST_RUN Status
                    iFlag=ATC_SELFTEST_STATUS;
                    if(ATC_ProcessReceiveCMD.Pos("+5")>=1)
                    {
                        aData.iATCSelfTestStatus=1;
                    }
                    else if(ATC_ProcessReceiveCMD.Pos("-5")>=1)
                    {
                        aData.iATCSelfTestStatus=2;
                    }
                    else if(ATC_ProcessReceiveCMD.Pos("+0")>=1)
                    {
                        aData.iATCSelfTestStatus=3;
                    }
                    else if(ATC_ProcessReceiveCMD.Pos("STOP")>=1)
                    {
                        aData.iATCSelfTestStatus=0;
                    }
                    break;
                case ATC_LotStart_STATUS:                                       //Ifor 20161118 add ATC2.0 接收ATC2.0 Lot Start STATUS
                    iFlag=ATC_LotStart_STATUS;
                    if(ATC_ProcessReceiveCMD.Pos("SUCCESS")>=1)
                        aData.bATCLotStart=true;
                    else
                        aData.bATCLotStart=false;
                    break;
                case ATC_LotEnd_STATUS:                                         //Ifor 20161118 add ATC2.0 接收ATC2.0 Lot End STATUS
                    iFlag=ATC_LotEnd_STATUS;
                    if(ATC_ProcessReceiveCMD.Pos("SUCCESS")>=1)
                        aData.bATCLotEnd=true;
                    else
                        aData.bATCLotEnd=false;
                    break;
            }

            if(iFlag>=0)
            {
                aData.Index=iFlag;
                aData.iType=ATC_ReceiveCMD[i]->iType;
                PushReadData(aData);
            }
            iStatus++;
            Log.sprintf("A->H(%d): %s ", iStatus, ATC_ProcessReceiveCMD.SubString(iStart, ATC_ReceiveCMD[i]->iCommandLength));
            PushHistoryString(Log.c_str());
            ATC_ProcessReceiveCMD.Delete(iStart, ATC_ReceiveCMD[i]->iCommandLength);
        }
    }
    ATC_ProcessReceiveCMD="";
    return  iStatus;
}
//---------------------------------------------------------------------------
bool HT_ATC::IsCommandString( string sData , string sCommand , int iPos)
{
    int iDataLen    = sData.length();
    int iCommandLen = sCommand.length();

    if(iDataLen >= iCommandLen )
    {
        string sSubData = sData.substr( iPos , iCommandLen );
        if( sSubData == sCommand)
        {
            return true;
        }
    }
    return false;
}
// normol function -----------------------------------------------------------<<
// ATC Command --------------------------------------------------------------->>
void HT_ATC::SetUse(bool bUse)
{
    AnsiString sSendData;
    if(bUse==true)
    {
        sSendData.sprintf("ATC_USE_SITE", _number);                             //H->A 01
    }
    else
    {
        sSendData.sprintf("ATC_NOT_USE_SITE", _number);                         //H->A 03
    }
    SendCommand(sSendData.c_str());
}
//---------------------------------------------------------------------------
void HT_ATC::SetTargetTemperature(double fTarget)
{
    AnsiString sSendData;
    if(fTarget>=_Temperature_P_Limit)                                           // 2012.02.02 , Joye , ATC改
    {
        fTarget=_Temperature_P_Limit;
    }
    sSendData.sprintf("ATC_TEMP_SET %04d", (int)fTarget*10);                    //H->A 04
    SendCommand(sSendData.c_str());
}
//---------------------------------------------------------------------------
void HT_ATC::SetTargetOffsetTemperature(double fTarget)
{
    AnsiString sSendData;
    sSendData.sprintf("ATC_OFFSET %04d", (int)fTarget*10);                      //H->A 15
    SendCommand(sSendData.c_str());
}
//---------------------------------------------------------------------------
void HT_ATC::LotStart(String sLotID)                                            //Ifor 20161118 add ATC2.0 Lot Start Command for KYEC
{
    AnsiString sSendData;
    sSendData.sprintf("START_LOT[%s]", sLotID);                                 //Ifor 20170103 (Steven) 資料格式修改<<" ">> => [" "]
    SendCommand(sSendData.c_str());
}
//---------------------------------------------------------------------------
void HT_ATC::LotEnd(String sLotID)                                              //Ifor 20161118 add ATC2.0 Lot Start Command for KYEC
{
    AnsiString sSendData;
    sSendData.sprintf("END_LOT[%s]", sLotID);                                   //Ifor 20170103 (Steven) 資料格式修改<<" ">> => [" "]
    SendCommand(sSendData.c_str());
}
//---------------------------------------------------------------------------
void HT_ATC::SetTargetChillerTemperature(int fTarget)
{
    AnsiString sSendData;
    sSendData.sprintf("ATC_SetChillerTemp %04d", (int)fTarget*10);              //H->A 16
    SendCommand(sSendData.c_str());
}
//---------------------------------------------------------------------------
void HT_ATC::CheckConnectState(int iChannel)                                    //12.06.27.01 s
{
    AnsiString sSendData;
    sSendData.sprintf("CONNECT %d", iChannel);                                  //H->A 08
    SendCommand(sSendData.c_str());
}                                                                               //12.06.27.01 e
//---------------------------------------------------------------------------
void HT_ATC::SetHandlerNowArm(int iArm)
{
    AnsiString sSendData;
    sSendData.sprintf("@HANDLER_ARM %d+", iArm);                                //H->A 15
    SendCommand(sSendData.c_str());
}
// ATC Command ---------------------------------------------------------------<<
// Thread function ----------------------------------------------------------->>
void HT_ATC_TCPIP_WATCH( void* pParameter)
{
    HT_ATC* pATC = (HT_ATC*) pParameter;
    TCPIP* pSocket  = pATC->_socket;

    static clock_t StartTime = clock();

    int iState;
//    StartTime=MyTickCount();
    DATA Data;
    pSocket->PopBuffer( iState , Data );

    static int iErrorCount=0;

    if(iState == TD_SEND )
    {
        pATC->PushHistoryString( "H->A: " , Data );
    }
    else if(iState == TD_READ )
    {
        // Read Process -----------------------------------------------------
        pATC->AdapterData( Data );
        // ------------------------------------------------------------------
        StartTime = clock();
    }
    else if(iState == TD_STATE )
    {
        if(Data[0] == 0x00)
        {
            pATC->PushHistoryString( "STATE: Connect OK!" );
        }
        else if(Data[0] == 0xFF )
        {
            pATC->PushHistoryString( "STATE: Disconnect!" );
        }
        else
        {
            pATC->PushHistoryString( "STATE: " , Data );
        }

        pATC->PushHistoryString( "" );
    }
    else if(iState == TD_ERROR )
    {
        clock_t EndTime = clock();    //pig 2011.12.21 ATC改
        string sLog( "ERROR: " );

        int iCode = Data[0]*0xFF + Data[1];
        sLog += IntToStr( iCode).c_str();
        //2011.12.27 Q_Q ATC改改
        if(iCode == 10060)
            iErrorCount++;
        if(iErrorCount > 10)
        {
            iErrorCount = 0;
            pATC->ClearHistory();
        }
        else  //2011.12.27 Q_Q ATC改改
        {
            pATC->PushHistoryString( sLog);
            pATC->PushHistoryString( "" );
        }
        //pig 2011.12.21 ATC改 start
        if( EndTime - StartTime >= 60*1000)         //1分鐘
        {
            StartTime = clock();
        }
        //pig 2011.12.21 ATC改 end
    }
    else
    {
        if( pATC->IsConnect() == true)
        {
            clock_t EndTime = clock();

            if( EndTime - StartTime >= 600*1000)   // 600 Sec
            {
                pATC->ClearHistory();
                pSocket->Clear();
            }
            else
            {
                MySleep(10);
            }
        }
    }
    MySleep(30);
}
// ---------------------------------------------------------------------------<<
// ATC System ------------------------------------------------------------------
ATCSystem::~ATCSystem()
{
    try
    {
        Channels.clear();                                                       //Steven 20120623 : ATC解構
    }
    catch(...)
    {
        MyDBIProcess("Exception", "~ATCSystem()");
    }
}
//---------------------------------------------------------------------------
void ATCSystem::Initial()
{
    Channels.clear();
}
//---------------------------------------------------------------------------
unsigned int ATCSystem::Size()
{
    return Channels.size();
}
//---------------------------------------------------------------------------
void ATCSystem::AddChannel( ATCChannel* pChannel )
{
    Channels.push_back( pChannel );
    _channel_use.push_back( true);
}
//---------------------------------------------------------------------------
bool ATCSystem::DeleteChannel( unsigned int iChannel )
{
    if(iChannel >= Channels.size())
        return false;

    Channels.erase( Channels.begin()+iChannel , Channels.begin()+iChannel+1);
    _channel_use.erase( _channel_use.begin()+iChannel , _channel_use.begin()+iChannel+1);
//    _TemperatureReady.erase( _TemperatureReady.begin()+iChannel , _TemperatureReady.begin()+iChannel+1);
    return true;
}
//---------------------------------------------------------------------------
bool ATCSystem::SetChannelSocket( unsigned int iChannel , string sAddress , int iPort )
{
    if(iChannel >= Channels.size())
        return false;

    ((HT_ATC*)Channels[iChannel])->InitialSocket( sAddress , iPort );
    return true;
}
//---------------------------------------------------------------------------
bool ATCSystem::IsConnect( unsigned int iChannel )
{
    if(iChannel >= Channels.size())
        return false;

    return Channels[iChannel]->IsConnect();
}
//---------------------------------------------------------------------------
bool ATCSystem::Connect( unsigned int iChannel )
{
    if(iChannel >= Channels.size())
        return false;

    if(Channels[iChannel]->IsConnect() == false)
    {
        Channels[iChannel]->Connect();
    }
    return true;
}
//---------------------------------------------------------------------------
bool ATCSystem::Disconnect( unsigned int iChannel )
{
    if(iChannel >= Channels.size())
        return false;

    Channels[iChannel]->Disconnect();
    return true;
}
//---------------------------------------------------------------------------
int ATCSystem::SendCommand( unsigned int iChannel , DATA SendData )
{
    if(iChannel >= Channels.size())
        return false;

    return Channels[iChannel]->SendCommand( SendData );
}
//---------------------------------------------------------------------------
int ATCSystem::SendCommand( unsigned int iChannel , string SendData )
{
    if(iChannel >= Channels.size())
        return -1;

    return Channels[iChannel]->SendCommand( SendData );
}
//---------------------------------------------------------------------------
bool ATCSystem::ClearHistory( unsigned int iChannel )
{
    if(iChannel >= Channels.size())
        return false;

    Channels[iChannel]->ClearHistory();
    return true;
}
//---------------------------------------------------------------------------
bool ATCSystem::PopHistoryString( unsigned int iChannel , string& log)
{
    if(iChannel >= Channels.size())
        return false;

    return Channels[iChannel]->PopHistoryString( log);
}
//---------------------------------------------------------------------------
bool ATCSystem::ClearData( unsigned int iChannel )
{
    if(iChannel >= Channels.size())
        return false;

    ((HT_ATC*)(Channels[iChannel]))->ClearReadData();
    return true;
}
//---------------------------------------------------------------------------
bool ATCSystem::PopData( unsigned int iChannel , ATCData& aData )
{
    if(iChannel >= Channels.size())
        return false;

    return ((HT_ATC*)(Channels[iChannel]))->PopReadData( aData );
}
//---------------------------------------------------------------------------
void ATCSystem::SetOnLine( bool bOnLine)
{
    unsigned int iSize = Size();
    if(bOnLine == true)
    {
        if( _online == false)
        {
            for( unsigned int i = 0 ; i < iSize ; i++)
            {
                Connect(i);
            }

            _ATCRun         = 0;    //Ifor 20160801
            _Chiller_Run    = 0;    //Ifor 20160801
            _online = true;
        }
    }
    else
    {
        for( unsigned int i = 0 ; i < iSize ; i++)
        {
            Disconnect(i);

            ((HT_ATC*)(Channels[i]))->ClearHistory();
            ((HT_ATC*)(Channels[i]))->_socket->Clear();
        }
        _online = false;
    }
}
//pig 2011.12.21 ATC改  end
//---------------------------------------------------------------------------
bool ATCSystem::SetChannelUse( unsigned int iChannel , bool bUse)
{
    if(iChannel >= Channels.size())
        return false;

    ((HT_ATC*)(Channels[iChannel]))->SetUse( bUse);

    _channel_use[iChannel] = bUse;
    return true;
}
//---------------------------------------------------------------------------
bool ATCSystem::SetWorkTemperature( unsigned int iChannel , double fTemp )
{
    if(iChannel >= Channels.size())
        return false;

    ((HT_ATC*)(Channels[iChannel]))->SetTargetTemperature( fTemp );
    return true;
}
//---------------------------------------------------------------------------
bool ATCSystem::CheckTemperatureReady( unsigned int iChannel )
{
    if(iChannel >= Channels.size())
        return false;

    if( _channel_use[iChannel] == false)
        return true;

    ((HT_ATC*)(Channels[iChannel]))->CheckTargetTemperatureReady();
    return true;
}
//---------------------------------------------------------------------------
bool ATCSystem::GetNowTemperature( unsigned int iChannel )
{
    if(iChannel >= Channels.size())
        return false;

    if( _channel_use[iChannel] == false)
        return true;

    if(Temperature.bUseReferTempSensor==true)                                   //Ifor 20151008 Add atc 兩組訊號資料
    {
        ((HT_ATC*)(Channels[iChannel]))->GetNowTemperature_Ref();
    }
    else
    {
        ((HT_ATC*)(Channels[iChannel]))->GetNowTemperature();
    }
    return true;
}
//---------------------------------------------------------------------------
bool ATCSystem::GetAllNowTemperature( unsigned int iChannel )                   //12.06.27.01 s
{
    ((HT_ATC*)(Channels[iChannel]))->GetAllNowTemperature();
    return true;
}                                                                               //12.06.27.01 e
//---------------------------------------------------------------------------
//bool ATCSystem::CheckTemperatureSetOK( unsigned int iChannel )
//{
//    if(iChannel >= Channels.size())
//        return false;
//
//    //if( _channel_use[iChannel] == false)
//    //    return false;
//
//    if(((HT_ATC*)(Channels[iChannel]))->_ha_state.ATC_TEMP_SET == 2 )
//        return true;
//
//    return false;
//}
//---------------------------------------------------------------------------
bool ATCSystem::SendTestStart( unsigned int iChannel )
{
    if(iChannel >= Channels.size())
        return false;

    if( _channel_use[iChannel] == false)
        return false;

    ((HT_ATC*)(Channels[iChannel]))->SendTestStart();
    return true;
}
//---------------------------------------------------------------------------
bool ATCSystem::SendTestEnd( unsigned int iChannel )
{
    if(iChannel >= Channels.size())
        return false;

    if( _channel_use[iChannel] == false)
        return false;

    ((HT_ATC*)(Channels[iChannel]))->SendTestEnd();
    return true;
}
// -----------------------------------------------------------------------------
//pig 2011.12.21 ATC改 start
bool ATCSystem::SetOffsetTemperature( unsigned int iChannel , double fTemp )
{
    if(iChannel>=Channels.size())
        return false;
    ((HT_ATC*)(Channels[iChannel]))->SetTargetOffsetTemperature(fTemp);
    return true;
}
//---------------------------------------------------------------------------
void ATCSystem::SetRunATC()
{
    ((HT_ATC*)(Channels[0]))->RunATC();
}
//---------------------------------------------------------------------------
bool ATCSystem::SetStopATC()
{
    ((HT_ATC*)(Channels[0]))->StopATC();
    return true;
}
//---------------------------------------------------------------------------
void ATCSystem::LotStart(String sLotID)                                         //Ifor 20161118 add ATC2.0 Lot Start Command for KYEC
{
    ((HT_ATC*)(Channels[0]))->LotStart(sLotID);
}
//---------------------------------------------------------------------------
void ATCSystem::LotEnd(String sLotID)                                           //Ifor 20161118 add ATC2.0 Lot End Command for KYEC
{
    ((HT_ATC*)(Channels[0]))->LotEnd(sLotID);
}
//---------------------------------------------------------------------------
bool ATCSystem::SetChillerTemperature(unsigned int fTemp)
{
    ((HT_ATC*)(Channels[0]))->SetTargetChillerTemperature(fTemp);
    return true;
}
//---------------------------------------------------------------------------
bool ATCSystem::SetATCChillerSwitchOpen()
{
    _Chiller_Run=true;                                                          // 2012.05.07 , Joye , Chiller
    _Chiller_Status=false;
    ((HT_ATC*)(Channels[0]))->ATCChillerSwitchOpen();
    return true;
}
//---------------------------------------------------------------------------
bool ATCSystem::SetATCChillerSwitchClose()
{
    _Chiller_Run = false;                                                       // 2012.05.07 , Joye , Chiller
    _Chiller_Status=true;
    ((HT_ATC*)(Channels[0]))->ATCChillerSwitchClose();
    return true;
}
//---------------------------------------------------------------------------
bool ATCSystem::SetATCCloseMessage()
{
    _Chiller_Run = false;                                                       // 2012.05.07 , Joye , Chiller
    _Chiller_Status=true;
    ((HT_ATC*)(Channels[0]))->ATCMessageClose();
    return true;
}
//---------------------------------------------------------------------------
//pig 2011.12.21 ATC改 end
void ATCSystem::SetTemperaturePLimit(double fTemp)                              // 2012.02.02 , Joye , ATC改
{
    unsigned int iSize=Channels.size();

    for(unsigned int i=0; i<iSize; i++)
    {
        ((HT_ATC*)(Channels[i]))->_Temperature_P_Limit=fTemp;
    }
}
//---------------------------------------------------------------------------
void ATCSystem::GetNowWorkTemperature(unsigned int iChannel)                    //Steven 20120528 : 確認工作溫度
{
    if(iChannel>=Channels.size())
        return;

    ((HT_ATC*)(Channels[iChannel]))->GetNowWorkTemperature();
    return;
}
//---------------------------------------------------------------------------
void ATCSystem::GetNowSiteOnOff(unsigned int iChannel)                          //Steven 20120528 : 確認開關Site
{
    if(iChannel==-1)
    {
        unsigned int iSize = Channels.size();

        for(unsigned int i=0; i<iSize; i++)
        {
            ((HT_ATC*)(Channels[i]))->GetNowSiteOnOff();
        }
    }
    else
    {
        ((HT_ATC*)(Channels[iChannel]))->GetNowSiteOnOff();
    }
}
//---------------------------------------------------------------------------
void ATCSystem::GetNowChillerTemperature(unsigned int iChannel)                 //Steven 20120528 : 確認冰水機溫度
{
    if(iChannel>=Channels.size())
        return;

    ((HT_ATC*)(Channels[iChannel]))->GetNowChillerTemperature();
    return;
}
//---------------------------------------------------------------------------
void ATCSystem::GetConnectState(unsigned int iChannel)                          //12.06.27.01 s
{
    if(iChannel>=Channels.size())
        return;

    ((HT_ATC*)(Channels[iChannel]))->CheckConnectState(iChannel);
    return;
}                                                                               //12.06.27.01 e
//---------------------------------------------------------------------------
bool ATCSystem::SetNowArm(int iArm)
{
    ((HT_ATC*)(Channels[0]))->SetHandlerNowArm(iArm);
    return true;
}
//---------------------------------------------------------------------------
//2014-05-30    Dell    for ATC6.0
//---------------------------------------------------------------------------
CRITICAL_SECTION g_WriteCommand;                                                //ChungHung 20141110 add
void __fastcall ATC60System::NewSocket()                                        //ChungHung 20141211 add
{
    if(clientsocket!=NULL)
    {
        _socket_thread.CloseThread(1000);
        delete clientsocket;
    }
    clientsocket                =new TClientSocket(NULL);
    clientsocket->OnConnect     =ClientSocket1Connect;
    clientsocket->OnDisconnect  =ClientSocket1Disconnect;
    clientsocket->OnRead        =clientsocketRead;
    clientsocket->OnError       =ClientSocket1Error;
    clientsocket->Address       ="172.16.8.90";
    clientsocket->Port          =5001;
    clientsocket->Active        =false;
    _socket_thread.Initial(_socket_execute ,this);
}
//---------------------------------------------------------------------------
__fastcall ATC60System::ATC60System()
{
    NewSocket();                                                                //ChungHung 20141211 add

    sList = new TStringList;
    sTjList = new TStringList;

    SetCommand();
    for(int i=0; i<Max_Command; i++)
    {
        bSend[i] = false;
        bReciveOK[i] = false;
    }
    for(int i=0; i<tcTotalCount; i++)                                           //ChungHung 20141105 add for ATC6.0 & 3.0
    {
         ReadTemp[i]=999;                                                       //ChungHung 20141212 add
         SendTemp[i]=999;
    }
    SetTemp     =0;                                                             //ChungHung 20141105 add for ATC6.0 & 3.0
    UseSite     ="";                                                            //ChungHung 20141105 add for ATC6.0 & 3.0
    EnableATC   =false;                                                         //ChungHung 20141105 add for ATC6.0 & 3.0
    HandlerArm  =-1;                                                            //ChungHung 20141105 add for ATC6.0 & 3.0
    TestStart   =-1;                                                            //ChungHung 20141105 add for ATC6.0 & 3.0
    bStartCount =false;

    ErrorMessage="";
    //Connected = false;
    Connecting=false;
    InitializeCriticalSection(&g_WriteCommand);                                 //ChungHung 20141110 add
}
//---------------------------------------------------------------------------
__fastcall ATC60System::~ATC60System()
{
    try
    {
        delete clientsocket;
        _socket_thread.CloseThread(1000);
        sList->Clear();                                                         //Ifor 20170603 (wei) TStringList 刪除前先 Clean
        sTjList->Clear();                                                       //Ifor 20170603 (wei) TStringList 刪除前先 Clean
        delete sList;
        delete sTjList;
    }
    catch(...)
    {
        MyDBIProcess("Exception", "~ATC60System()");
    }
}
//---------------------------------------------------------------------------
void ATC60System::ATCResetParm()
{
    EnterCriticalSection(&g_WriteCommand);                                      //ChungHung 20141110 add
    for(int i=0; i<Max_Command; i++)
    {
        bReciveOK[i]=false;
        bSend[i]=false;
    }

    for(int i=0; i<tcTotalCount; i++)                                           //ChungHung 20141105 add for ATC6.0 & 3.0
    {
         ReadTemp[i]=999;                                                       //ChungHung 20141212 add
         SendTemp[i]=999;
    }

    bStartCount=false;
    ErrorMessage="";
    LeaveCriticalSection(&g_WriteCommand);                                      //ChungHung 20141110 add
}
//---------------------------------------------------------------------------
TQPF_Timer HATC60TimeOut;                                                       //ChungHung 20141105 add for ATC6.0 & 3.0
void ATC60System::SendCommand()
{
    AnsiString command;
    AnsiString CheckCmd;
    int iCommand=-1;

    if(SystemInitialOK==false || IsConnected()==false)
    {
        return;
    }

    for(int i=0; i<Max_Command; i++)
    {
        if(bSend[i]==true)
        {
            return;
        }
    }

    EnterCriticalSection(&g_WriteCommand);                                      //ChungHung 20141110 add
    if(vSendBuf.size()==0)
    {
        LeaveCriticalSection(&g_WriteCommand);                                  //ChungHung 20141110 add
        return;
    }
    command=vSendBuf.front();
    if(command=="")
    {
        vSendBuf.erase(vSendBuf.begin());                                       //ChungHung 20141212 add
        LeaveCriticalSection(&g_WriteCommand);                                  //ChungHung 20141110 add
        return;
    }
    for(int i=0; i<Max_Command; i++)
    {
        if(command.Pos(sCmd[i])!=0)
        {
            iCommand=i;
            break;
        }
    }

    if(iCommand==-1)
    {
        vSendBuf.erase(vSendBuf.begin());                                       //ChungHung 20141212 add
        LeaveCriticalSection(&g_WriteCommand);                                  //ChungHung 20141110 add
        return;
    }
    bSend[iCommand]=true;
    if(clientsocket->Socket->Connected)
    {
        clientsocket->Socket->Lock();
        clientsocket->Socket->SendText(vSendBuf.front());
        clientsocket->Socket->Unlock();
        vSendBuf.erase(vSendBuf.begin());                                       //ChungHung 20141212 add

        HATC60TimeOut.SetSecAndOn(5);                                           //ChungHung 20141105 add for ATC6.0 & 3.0
        bStartCount=true;
    }
    LeaveCriticalSection(&g_WriteCommand);                                      //ChungHung 20141110 add
}
//---------------------------------------------------------------------------
void ATC60System::DoCommmand()
{
    if(SystemInitialOK==false || IsConnected()==false)
    {
        for(int i=0; i<tcTotalCount; i++)
            ReadTemp[i]=999;

        return;
    }
    if(bStartCount==false)
        return;

    if( HATC60TimeOut.Off())
    {
        ATCResetParm();
        CloseSocket();                                                          //ChunHung 20141211 receive error command close socket
        return;
    }

    if(vReciveBuf.size()<=0)
    {
        return ;
    }

    if(vReciveBuf.size()>1)
    {
        for(unsigned int i=0; i<vReciveBuf.size(); i++)
            ShowMessage(vReciveBuf[i]);
    }

    AnsiString ReciveText = vReciveBuf[0];
    AnsiString STX="[STX]";
    AnsiString ETX="[ETX]";
    AnsiString Reply="REPLY_TEMP";

    AnsiString StrTemp="";
    AnsiString sSize="";

    int iStart = ReciveText.Pos("[STX]");
    int iEnd   = ReciveText.Pos("[ETX]");
    int iSize=0;

    if(iStart==0 || iEnd==0)
    {
        return;
    }
    if(iStart>iEnd)                                                             //Data have problem
    {
        ATCResetParm();
        return;
    }

    if(ReciveText.AnsiPos("ATC_ERROR")>0)                                       //ChungHung 20141219 add
    {
        ErrorMessage="ATC_ERROR";
    }
    else if(ReciveText.AnsiPos(rCmd[ATCENABLED])>0)
    {
        bReciveOK[ATCENABLED]=true;
    }
    else if(ReciveText.AnsiPos(rCmd[SETTEMP])>0)
    {
        bReciveOK[SETTEMP]=true;
    }
    else if(ReciveText.AnsiPos(rCmd[USESITE])>0)
    {
        bReciveOK[USESITE] = true;
    }
    else if(ReciveText.AnsiPos(rCmd[HANDLERARM])>0)
    {
        bReciveOK[HANDLERARM]=true;
    }
    else if(ReciveText.AnsiPos(rCmd[SETOFFSET])>0)
    {
        bReciveOK[SETOFFSET]=true;
    }
    else if(ReciveText.AnsiPos(rCmd[TESTSTART])>0)
    {
         bReciveOK[TESTSTART]=true;
    }
    else if(ReciveText.AnsiPos(rCmd[LOADRECIPE])>0)                             //Steven 20150810 : add for ATC
    {
         bReciveOK[LOADRECIPE]=true;
         if(ReciveText.AnsiPos("0")>0)
         {
            ErrorMessage="ATC do no find recipe error!";
         }
    }
    else if(ReciveText.AnsiPos(rCmd[GETTEMP])>0)                                //ChungHung 20141105 add Get Temp
    {
        StrTemp = ReciveText;
        StrTemp.Delete(iStart,STX.Length());
        iEnd=StrTemp.Pos(ETX);
        StrTemp.Delete(iEnd,ETX.Length());
        StrTemp.Delete(1,Reply.Length());
        iStart  = StrTemp.Pos("<");
        iEnd    = StrTemp.Pos(">,");
        sSize   = StrTemp.SubString(iStart+1, iEnd-iStart-1);
        iSize   = atoi(sSize.c_str());
        StrTemp.Delete(iStart, (iEnd-iStart)+2);
        GetATCTemperature(StrTemp,iSize);
        bReciveOK[GETTEMP] = true;
    }
    else
    {
        CloseSocket();                                                          //ChunHung 20141211 receive error command close socket
    }

    for(int i=0; i<Max_Command; i++)
    {
        if(bReciveOK[i]!=bSend[i] && ErrorMessage!="ATC_ERROR")                 //ChungHung 20141219 add
        {
            CloseSocket();                                                      //ChunHung 20141211 receive error command close socket
            return;
        }
    }

    memset(bReciveOK, false, Max_Command);                                      //Jimmychiu 20230307 修正矩陣大小 31->Max_Command 避免溢位寫入資料
    memset(bSend, false, Max_Command);                                          //Jimmychiu 20230307 修正矩陣大小 31->Max_Command 避免溢位寫入資料
    vReciveBuf.clear();
    bStartCount=false;
}
//---------------------------------------------------------------------------
void __fastcall ATC60System::ClientSocket1Connect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    Connected = true;
    Connecting=false;

    ATCInterfaceForm->ATC_60_SYS.ReSendCommand(ATCInterfaceForm->ATC_60_SYS.SETTEMP);
    ATCInterfaceForm->ATC_60_SYS.ReSendCommand(ATCInterfaceForm->ATC_60_SYS.USESITE);
    ATCInterfaceForm->ATC_60_SYS.ReSendCommand(ATCInterfaceForm->ATC_60_SYS.SETOFFSET);
    ATCInterfaceForm->ATC_60_SYS.ReSendCommand(ATCInterfaceForm->ATC_60_SYS.ATCENABLED); //ChungHung 20141124 change
}
//---------------------------------------------------------------------------
void __fastcall ATC60System::ClientSocket1Disconnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    Connected =false;
    Connecting=false;
}
//---------------------------------------------------------------------------
void __fastcall ATC60System::clientsocketRead(TObject *Sender,
      TCustomWinSocket *Socket)
{
    AnsiString rStr, rRec;
    Word thour, tmin, tsec, tmsec;

    rRec = clientsocket->Socket->ReceiveText();
    vReciveBuf.push_back(rRec);

    TDateTime rTime = Time();
    rTime.DecodeTime(&thour, &tmin, &tsec, &tmsec);
    rStr.sprintf("%d:%d:%d:%d  ,%s", thour, tmin, tsec, tmsec, rRec);
}
//------------------------------------------------------------------------------
void __fastcall ATC60System::ClientSocket1Error(TObject *Sender,
      TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
    ErrorMessage=GetErrorMsg(Sender, ErrorCode);
    Connected = false;
    Connecting=false;
    ErrorCode =0;
}
//------------------------------------------------------------------------------
double CheckStrIsNum(char* Num)                                                 //ChungHung 2011127 add 防止傳過來的不是數字
{
    int i=0;
    while(Num[i]!='\0')
    {
        if((Num[i]>='0' && Num[i]<='9' )|| Num[i]=='.')
            i++;
        else
            return 0;
    }
    return atof(Num);
}
//------------------------------------------------------------------------------
void ATC60System::GetATCTemperature(AnsiString aTemp, int iSize)                //ChungHung 20141105 add for ATC6.0 & 3.0
{
    char str[100];
    memset(str, '\0',100);
    strncpy(str, aTemp.c_str(), sizeof(str));
    int iCount=0;

    char *temp = strtok(str, ",");
    while (temp != NULL)
    {
        ReadTemp[iCount]=CheckStrIsNum(temp);
        temp = strtok(NULL, ",");
        iCount++;
        if(iCount>=iSize)
            break;
    }
}
//------------------------------------------------------------------------------
void ATC60System::SetCommand()
{
    sCmd[ATCENABLED]    ="ATC_ENABLED";
    sCmd[SETTEMP]       ="SET_TEMP";
    sCmd[USESITE]       ="USE_SITE";
    sCmd[SETOFFSET]     ="SET_OFFSET";
    sCmd[HANDLERARM]    ="HANDLER_ARM";
    sCmd[TESTSTART]     ="TEST_START";
    sCmd[GETTEMP]       ="GET_TEMP";
    sCmd[LOADRECIPE]    ="LOAD_RECIPE";                                         //Steven 20150810 : add for ATC

    rCmd[ATCENABLED]    ="ATC_ENABLED_OK";
    rCmd[SETTEMP]       ="SET_TEMP_OK";
    rCmd[USESITE]       ="USE_SITE_OK";
    rCmd[SETOFFSET]     ="SET_OFFSET_OK";
    rCmd[HANDLERARM]    ="HANDLER_ARM_OK";
    rCmd[TESTSTART]     ="TEST_START_OK";
    rCmd[GETTEMP]       ="REPLY_TEMP";
    rCmd[LOADRECIPE]    ="LOAD_RECIPE";                                         //Steven 20150810 : add for ATC

    /*
    sCmd[ATCENABLED]    = "ATC_ENABLED";
    sCmd[SETTEMP]       = "SET_TEMP";
    sCmd[USESITE]       = "USE_SITE";  // x,x,x,x ,x,x,x,x ,x,x,x,x ,x,x,x,x
    sCmd[SETOFFSET]     = "SET_OFFSET"; // x,x,x,x ,x,x,x,x ,x,x,x,x ,x,x,x,x
    sCmd[HANDLERARM]    = "HANDLER_ARM";
    sCmd[TESTSTART]     = "TEST_START";
    sCmd[SENDTJ]        = "SEND_TJ";

    rCmd[ATCENABLED]    = "ATC_ENABLED_OK";
    rCmd[SETTEMP]       = "SETTEMP_OK";
    rCmd[USESITE]       = "USE_SITE_OK";
    rCmd[SETOFFSET]     = "SET_OFFSET_OK";
    rCmd[HANDLERARM]    = "HANDLER_ARM_OK";
    rCmd[TESTSTART]     = "TEST_START_OK";
    rCmd[SENDTJ]        = "SEND_TJ_OK";
    */
}
//------------------------------------------------------------------------------
void ATC60System::CheckCommand(int iCommand)
{
    EnterCriticalSection(&g_WriteCommand);                                      //ChungHung 20141110 add
    AnsiString aCommand;

    if(vSendBuf.size()!=0)
    {
        for(unsigned int i=0; i<vSendBuf.size(); i++)
        {
            aCommand=vSendBuf[i];
            if(aCommand.Pos(sCmd[iCommand])!=0)
            {
                vSendBuf.erase(vSendBuf.begin()+i);
            }
        }
    }

    LeaveCriticalSection(&g_WriteCommand);                                      //ChungHung 20141110 add
}
//------------------------------------------------------------------------------
void ATC60System::ReSendCommand(ATCCommandIndex iCommand)
{
    AnsiString aCommand="";
    EnterCriticalSection(&g_WriteCommand);                                      //ChungHung 20141110 add

    switch(iCommand)
    {
        case ATCENABLED:
            SetATCRun(EnableATC);
            break;
        case SETTEMP:
            SetTargetTemperature(SetTemp);
            break;
        case USESITE:
            if(UseSite!="")                                                     //Ifor 20151028 修正ATC3.0 無Site 資料不送CMD 給ATC
                SetChannelUse(UseSite, 16);
            break;
        case SETOFFSET:
            SetTargetOffsetTemperature(SendTemp, 16);
            break;
        case HANDLERARM:
            SetHandlerNowArm(HandlerArm);
            break;
        case TESTSTART:
            SendTestStart(TestStart);
            break;
        case GETTEMP:
            GetRealTemp();
            break;
        case LOADRECIPE:                                                        //Steven 20150810 : add for ATC
            LoadRecipe(FileName);
            break;
    }

    if(sCmdBackUp[iCommand]!="")                                                //ChungHung 20141219 add
    {
        CheckCommand(iCommand);                                                 //ChungHung 20141212 add
        vSendBuf.push_back(sCmdBackUp[iCommand]);
    }
    else
        vSendBuf.erase(vSendBuf.begin());
    LeaveCriticalSection(&g_WriteCommand);                                      //ChungHung 20141110 add
}
//------------------------------------------------------------------------------
void ATC60System::GetRealTemp()
{
    EnterCriticalSection(&g_WriteCommand);                                      //ChungHung 20141110 add
    if(vSendBuf.size()==0)
        vSendBuf.push_back("[STX]GET_TEMP<1>,16[ETX]");
    LeaveCriticalSection(&g_WriteCommand);                                      //ChungHung 20141110 add
}
//------------------------------------------------------------------------------
void ATC60System::LoadRecipe(AnsiString sFileName)                              //Steven 20150810 : add for ATC
{
    bool ret=false;

    if(FileName!=sFileName)
    {
        FileName=sFileName;
        ret=true;
    }

    AnsiString Cmd;
    EnterCriticalSection(&g_WriteCommand);
    Cmd.sprintf("[STX]LOAD_RECIPE<1>,%s[ETX]", FileName);
    sCmdBackUp[LOADRECIPE]=Cmd;
    if(ret)
    {
        CheckCommand(LOADRECIPE);                                               //ChungHung 20141212 add
        vSendBuf.push_back(Cmd);
    }
    LeaveCriticalSection(&g_WriteCommand);
}
//------------------------------------------------------------------------------
void ATC60System::SetTargetTemperature(double fTarget)
{
    bool ret=false;
    if(SetTemp!=fTarget)
    {
        SetTemp=fTarget;
        ret=true;
    }

    AnsiString Cmd;
    EnterCriticalSection(&g_WriteCommand);                                      //ChungHung 20141110 add
    Cmd.sprintf("[STX]SET_TEMP<1>,%0.1f[ETX]", SetTemp);
    sCmdBackUp[SETTEMP]=Cmd;
    if(ret)
    {
        CheckCommand(SETTEMP);                                                  //ChungHung 20141212 add
        vSendBuf.push_back(Cmd);
    }
    LeaveCriticalSection(&g_WriteCommand);                                      //ChungHung 20141110 add
}
//------------------------------------------------------------------------------
void ATC60System::SetChannelUse(AnsiString bUse, int iSize)
{
    bool ret=false;
    if(UseSite!=bUse)
    {
        UseSite=bUse;
        ret=true;
    }

    AnsiString Cmd;
    EnterCriticalSection(&g_WriteCommand);                                      //ChungHung 20141110 add
    Cmd.sprintf("[STX]USE_SITE<%d>%s[ETX]", iSize, bUse);
    sCmdBackUp[USESITE]=Cmd;
    if(ret)
    {
        CheckCommand(USESITE);                                                  //ChungHung 20141212 add
        vSendBuf.push_back(Cmd);
    }
    LeaveCriticalSection(&g_WriteCommand);                                      //ChungHung 20141110 add
}
//------------------------------------------------------------------------------
void ATC60System::SetTargetOffsetTemperature(double *fOffset, int iSize)        //ChungHung 20141105 add for ATC6.0 & 3.0
{
    AnsiString str ,tStr ="";
    bool ret=false;

    for(int i=0; i<iSize; i++)
    {
        if(SendTemp[i]!=fOffset[i])
        {
            SendTemp[i]=fOffset[i];
            ret=true;
        }
    }

    for(int i=0; i<iSize; i++)
    {
        str.sprintf(",%0.1f" ,fOffset[i]);
        tStr += str;
    }

    AnsiString Cmd;
    EnterCriticalSection(&g_WriteCommand);                                      //ChungHung 20141110 add
    Cmd.sprintf("[STX]SET_OFFSET<%d>%s[ETX]",iSize,tStr);
    sCmdBackUp[SETOFFSET]=Cmd;

    if(ret)
    {
        CheckCommand(SETOFFSET);                                                //ChungHung 20141212 add
        vSendBuf.push_back(Cmd);
    }
    LeaveCriticalSection(&g_WriteCommand);                                      //ChungHung 20141110 add
}
//------------------------------------------------------------------------------
void ATC60System::SetATCRun(bool bRunning)
{
    bool ret=false;
    if(EnableATC!=bRunning)
    {
        EnableATC=bRunning;
        ret=true;
    }

    AnsiString Cmd;
    EnterCriticalSection(&g_WriteCommand);                                      //ChungHung 20141110 add

    Cmd.sprintf("[STX]ATC_ENABLED<1>,%d[ETX]", bRunning?1:0);
    sCmdBackUp[ATCENABLED]=Cmd;

    if(ret)
    {
        CheckCommand(ATCENABLED);                                               //ChungHung 20141212 add
        vSendBuf.push_back(Cmd);
    }
    LeaveCriticalSection(&g_WriteCommand);                                      //ChungHung 20141110 add
}
//------------------------------------------------------------------------------
void ATC60System::SetHandlerNowArm(int iArm)
{
    bool ret=false;
    if(HandlerArm!=iArm)
    {
        HandlerArm=iArm;
        ret=true;
    }

    AnsiString Cmd;
    EnterCriticalSection(&g_WriteCommand);                                      //ChungHung 20141110 add
    Cmd.sprintf("[STX]HANDLER_ARM<1>,%d[ETX]", iArm);
    sCmdBackUp[HANDLERARM]=Cmd;
    if(ret)
    {
        CheckCommand(HANDLERARM);                                               //ChungHung 20141212 add
        vSendBuf.insert(vSendBuf.begin(),Cmd);
    }
    LeaveCriticalSection(&g_WriteCommand);                                      //ChungHung 20141110 add
}
//------------------------------------------------------------------------------
void ATC60System::SendTestStart(int iStart)
{
    bool ret=false;
    if(TestStart!=iStart)
    {
        TestStart=iStart;
        ret=true;
    }

    AnsiString Cmd;
    EnterCriticalSection(&g_WriteCommand);                                      //ChungHung 20141110 add
    Cmd.sprintf("[STX]TEST_START<1>,%d[ETX]", iStart);
    sCmdBackUp[TESTSTART]=Cmd;

    if(ret)
    {
        CheckCommand(TESTSTART);                                                //ChungHung 20141212 add
        vSendBuf.insert(vSendBuf.begin(),Cmd);
    }
    LeaveCriticalSection(&g_WriteCommand);                                      //ChungHung 20141110 add
}
//------------------------------------------------------------------------------
void ATC60System::CloseSocket()
{
    try
    {
        if(clientsocket->Active==false)
            return;
        clientsocket->Socket->Lock();
        clientsocket->Close();
        clientsocket->Socket->Unlock();
        vReciveBuf.clear();
        Connecting=false;
        ATCResetParm();                                                         //ChungHung 20141212 add
        MySleep(500);
        _socket_thread.CloseThread(100);

    }
    catch(...)
    {
        MyDBIProcess("Exception", "ATC60System::CloseSocket");
    }
}
//------------------------------------------------------------------------------
void ATC60System::OpenSocket()
{
    if(Connecting && IsConnected())
        return ;

    try
    {
        NewSocket();                                                            //ChungHung 20141211 add
        clientsocket->Open();
        vReciveBuf.clear();
        ATCResetParm();                                                         //ChungHung 20141212 add
        Connecting=true;
        MySleep(500);
        _socket_thread.OpenThread();

    }
    catch(...)
    {
        MyDBIProcess("Exception", "ATC60System::OpenSocket");
    }
}
//------------------------------------------------------------------------------
bool ATC60System::IsConnected()
{
    bool bRet=false;
    if(clientsocket->Socket->Connected)
        bRet=true;

    if(bRet==false)
        ATCResetParm();

    return bRet;
}
//------------------------------------------------------------------------------
bool ATC60System::IsConnecting()
{
    return Connecting;
}
//------------------------------------------------------------------------------
void ATC60System::TransformTjData(AnsiString asStr)
{
    int iHeadSiteMap[8];

    for(int i=0; i<8; i++)
    {
        iHeadSiteMap[i]=TestIF.iSiteMap[i/4][i%4];
    }

    AnsiString str;
    TStringList *IsLine=new TStringList();
    TStringList *BackUp=new TStringList();
    int Num=asStr.Pos("_");
    asStr.Delete(1, Num);

    str=StringReplace(asStr,"_", ",", TReplaceFlags()<<rfReplaceAll);
    IsLine->CommaText=str;

    for(int i=0; i<8; i++)
    {
        if(iHeadSiteMap[i]<=0 || IsLine->Strings[i]=="NULL" || iHeadSiteMap[i]>IsLine->Count)
        {
            IsLine->Strings[i]="0.0";
            continue;
        }
        BackUp->Add(IsLine->Strings[iHeadSiteMap[i]]);
    }

    sTjList->Clear();
    sTjList->Add("SetTJ");
    sTjList->Add(HandlerArm);
    for(int i=0; i<8; i++)
    {
        sTjList->Add(BackUp->Strings[i]);
    }
    IsLine->Clear();                                                            //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    BackUp->Clear();                                                            //Ifor 20170603 (wei) TStringList 刪除前先 Clean
    delete IsLine;                                                              //Steven 20160912 : Add delete for save memory
    delete BackUp;
}
//---------------------------------------------------------------------------
const int Arm1AaSite = 11;
const int Arm2AaSite = 19;
const int iUserOffSet = 3;
void ATC60System::SetTjOffset(AnsiString buffer)
{
    double TjTemp =0;

    ZeroMemory(HandlerOffset, sizeof(HandlerOffset));
    TransformTjData(buffer);

    if(Temperature.bATCActiveCooling && Temperature.bUseTjFunction)
    {
        if(Temperature.iTjMode==0)                                              //by Arm
        {                                                                       //要將offet 補到下一顆Device ,所以當Arm1進測區就Set Arm2
            int iwhichArm=sTjList->Strings[1].ToIntDef(0);

            if(iwhichArm==0)
            {
                for(int i=0; i<8; i++)
                {
                    TjOffset[i]=sTjList->Strings[2+i].ToDouble();
                    TjTemp=TjOffset[8+i];

                    if(TjTemp>0)
                    {
                        TjTemp=Temperature.fWorkTemperBase-TjTemp;

                        if(TjTemp<=10 && TjTemp>=-10)
                        {
                            HandlerOffset[8+i]=TjTemp;
                            TjOffset[8+i]=0;
                        }
                    }
                }
            }
            else
            {
                for(int i=0; i<8; i++)
                {
                    TjOffset[8+i]=sTjList->Strings[10+i].ToDouble();
                    TjTemp=TjOffset[i];

                    if(TjTemp>0)
                    {
                        TjTemp=Temperature.fWorkTemperBase-TjTemp;

                        if(TjTemp<=10 && TjTemp>=-10)
                        {
                            Temperature.fTempOffSet[iUserOffSet][Arm1AaSite+i]+=TjTemp;
                            TjOffset[i] =0;
                        }
                    }
                }
            }
        }
        else if(Temperature.iTjMode==1)                                         //real time
        {
            int iwhichArm=sTjList->Strings[1].ToIntDef(0);

            if(iwhichArm==0)
            {
                for(int i=0; i<8; i++)
                {
                    TjTemp=sTjList->Strings[2+i].ToDouble();
                    if(TjTemp>0)
                    {
                        TjTemp=Temperature.fWorkTemperBase-TjTemp;

                        if(TjTemp<= 10 && TjTemp>=-10)
                        {
                            if(TjTemp>1 || TjTemp<-1)
                            {
                                Temperature.fTempOffSet[iUserOffSet][Arm1AaSite+i]+=TjTemp;
                            }
                        }
                    }
                }
            }
            else
            {
                for(int i=0; i<8; i++)
                {
                    TjTemp=sTjList->Strings[10+i].ToDouble();
                    if(TjTemp>0)
                    {
                        TjTemp=Temperature.fWorkTemperBase-TjTemp;

                        if(TjTemp<=10 && TjTemp>=-10)
                        {
                            if(TjTemp>1 || TjTemp<-1)
                            {
                                Temperature.fTempOffSet[iUserOffSet][Arm2AaSite+i]+=TjTemp;
                            }
                        }
                    }
                }
            }
        }
        else if(Temperature.iTjMode==2)                                         //Avg
        {
            int iwhichArm=sTjList->Strings[1].ToIntDef(0);
            double iAvg;

            if(iwhichArm==0)
            {
                for(int i=0; i<8; i++)
                {
                    TjTemp=sTjList->Strings[2+i].ToDouble();
                    if(TjTemp>0)
                    {
                        vTjTemp[i].push_back(TjTemp);
                    }
                }
            }
            else
            {
                for(int i=0; i<8; i++)
                {
                    TjTemp=sTjList->Strings[10+i].ToDouble();
                    if(TjTemp>0)
                    {
                        vTjTemp[8+i].push_back(TjTemp);
                    }
                }
            }

            int iStart, iEnd;

            if(iwhichArm==0)                                                    //要將offet 補到下一顆Device ,所以當Arm1進測區就Set Arm2
            {
                iStart=8;
                iEnd=16;
            }
            else
            {
                iStart=0;
                iEnd=8;
            }

            for(int i=iStart; i<iEnd; i++)
            {
                if(vTjTemp[i].size()>=unsigned(Temperature.iTjAvgTimes))
                {
                    iAvg=0;
                    for(int j=0; j<Temperature.iTjAvgTimes; j++)
                    {
                        iAvg+=vTjTemp[i].at(j);
                    }
                    iAvg=ChangeToFloatNonPcnt((double)(iAvg), (double)(Temperature.iTjAvgTimes));

                    vTjTemp[i].clear();
                    TjTemp = Temperature.fWorkTemperBase - iAvg;

                    if(TjTemp<=10 && TjTemp>=-10)
                    {
                        Temperature.fTempOffSet[iUserOffSet][Arm1AaSite+i]+=TjTemp;
                    }
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
void ATC60System::SaveTjLog(AnsiString Str)
{
    AnsiString sTemp, sTimestamp;
    Word Hour, Min, Sec, MSec;
    TDateTime logTime=Time();
    DecodeTime(logTime, Hour, Min, Sec, MSec);
    sTimestamp.sprintf("%02d:%02d:%02d.%03d,", Hour, Min, Sec, MSec);
    sTemp=sTimestamp+Str;
    sTjLogList->Add(sTemp);
}
//---------------------------------------------------------------------------
bool ATCSystem::GetNowATCSwVer()                                                //Ifor 20151023 Add ATC 2.0 讀取ATC軟體版本
{
    ((HT_ATC*)(Channels[0]))->GetNowATCSwVer();
    return true;
}
//---------------------------------------------------------------------------
bool ATCSystem::SetATCTimeSync()                                                //Ifor 20151023 Add ATC 2.0 設定ATC電腦時間同步
{
    ((HT_ATC*)(Channels[0]))->SetATCTimeSync();
    return true;
}
//---------------------------------------------------------------------------
void HT_ATC::SetATCTimeSync()
{
    AnsiString str = FormatDateTime("yyyy/mm/dd hh:nn:ss", Now());
    char buffer[100];
    string sSendData;
    sprintf( buffer , "@SET_ATC_TIME_SYNC %s+", str.c_str() );

    sSendData = buffer;

    SendCommand( sSendData );
}
//---------------------------------------------------------------------------
bool ATCSystem::Set2ndTempFunction(bool bEnable)                                //Ifor 20160427 Add ATC 2.0 設定第二點Sensor Function On/Off
{
    ((HT_ATC*)(Channels[0]))->Set2ndTempFunction(bEnable);
    return true;
}
//---------------------------------------------------------------------------
void HT_ATC::Set2ndTempFunction(bool bEnable)
{
    AnsiString sSendData;
    if(bEnable==true)
    {
        sSendData.sprintf("@SET_DUAL_TEMP_FUN 1+");                             //H->A 01
    }
    else
    {
        sSendData.sprintf("@SET_DUAL_TEMP_FUN 0+");                             //H->A 03
    }

    SendCommand(sSendData.c_str());
}
//---------------------------------------------------------------------------
bool ATCSystem::SetATCSelfTest()                                                //Ifor 20160720 Add ATC 2.0 執行自我測試模式
{
    ((HT_ATC*)(Channels[0]))->SetATCSelfTest();
    return true;
}
//---------------------------------------------------------------------------
bool ATCSystem::GetNowATCStatus()                                               //Ifor 20160801 Add ATC 2.0 讀取狀態
{
    ((HT_ATC*)(Channels[0]))->GetNowATCStatus();
    return true;
}
//---------------------------------------------------------------------------
bool ATCSystem::SetATCManualSelfTest()                                          //Ifor 20160823 add Manual Send ATC Self Test Command
{
    ((HT_ATC*)(Channels[0]))->SetATCManualSelfTest();
    return true;
}
//---------------------------------------------------------------------------
bool ATCSystem::GetNowSelfTestStatus()                                          //Ifor 20160824 Add ATC 2.0 讀取Self Test Status
{
    ((HT_ATC*)(Channels[0]))->GetNowSelfTestStatus();
    return true;
}
//---------------------------------------------------------------------------
