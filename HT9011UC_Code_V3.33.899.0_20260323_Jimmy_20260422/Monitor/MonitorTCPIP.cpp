#include "MachineDefine.h"
#pragma hdrstop
//---------------------------------------------------------------------------

#include "MonitorTCPIP.h"
//#include "IncludeAllHeader.H"
#include "common.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)
// Thread function ----------------------------------------------------------
void MV_TCPIP_WATCH( void* pParameter )
{
    MonitorTCPIP* pMV = (MonitorTCPIP*) pParameter;

    int iType;
    DATA vData;
    if( pMV->AdapterData( iType , vData ) != 0 )
    {

    }
    else
    {
        MySleep( 500 );
    }

     MySleep( 10 );
}

MonitorTCPIP::MonitorTCPIP( TCPIP* pSocket )
{
    _socket     = pSocket;
    _memo       = NULL;

    for(int i=0; i<10; i++)
        _bCommandReady[i] = 0;

    _socket_thread.Initial( MV_TCPIP_WATCH , this );
    _socket_thread.SetSleepDelayCount(0);
}

MonitorTCPIP::~MonitorTCPIP()
{
    try
    {
        delete _socket;
        _socket_thread.CloseThread(1000);
    }
    catch(...)
    {
        MyDBIProcess("Exception", "~MonitorTCPIP");
    }
}

void MonitorTCPIP::InitialSocket( AnsiString sAddress , int iPort )
{
    _socket->Initial( sAddress.c_str() , iPort );
}

void MonitorTCPIP::InitialMemo( TMemo* pMemo )
{
    _memo       = pMemo;
}

void MonitorTCPIP::Connect()
{
    _socket->Connect();
}

void MonitorTCPIP::Disconnect()
{
    _socket->DisConnect();
}

bool MonitorTCPIP::IsConnect()
{
    return _socket->IsConnect();
}

int MonitorTCPIP::GetReceiveCommand(int index)
{
    return _bCommandReady[index];
}

int MonitorTCPIP::SendCommand( DATA SendData )
{
    //MySleep(10);    // Send Dealy

    return _socket->Send( SendData );
}

int MonitorTCPIP::SendCommand( AnsiString SendData )
{
    //MySleep(10);    // Send Dealy

    return _socket->Send( SendData.c_str() );
}

int MonitorTCPIP::AdapterData( int& iType , DATA& vData )
{
    int iSize;

    iSize = _socket->PopBuffer( iType , vData );

    if( iSize == 0 )
        return 0;

    if( iType == TD_READ )
    {
        ShowData( "[M->H] " , vData );
    }
    else if( iType == TD_SEND  )
    {
        ShowData( "[H->M] " , vData );
    }
    else if( iType == TD_STATE )
    {
        if( vData[0] == 0x00 )
        {
            ShowData( "[STATE] Connect OK! " , vData );
        }
        else if( vData[0] == 0xFF )
        {
            ShowData( "[STATE] Disconnect! " , vData );
        }
        else
        {
            ShowData( "[STATE] Unknow " , vData );
        }

    }
    else if( iType == TD_ERROR )
    {
        //int iCode = vData[0]*0xFF + vData[1];
        //AnsiString sErr = "[ERROR: " + AnsiString( iCode ) + " ]";

        //ShowData( sErr , vData );
    }
    else
    {
        //ShowData( "[UNKNOW]" , vData );
    }

    return iSize;
}

void MonitorTCPIP::ShowData( AnsiString sType , DATA vData )
{
    AnsiString sLog     = sType + "";

    unsigned int iSize = vData.size();

    for( unsigned int i = 0 ; i < iSize ; i++ )
    {
        sLog    += (char)vData[i];
    }

    if( _memo != NULL )
    {
        if( _memo->Lines->Count >= 300 )
        {
            _memo->Clear();
        }
        _memo->Lines->Append( sLog );

    }
}