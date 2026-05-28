//---------------------------------------------------------------------------
#ifndef MonitorTCPIPH
#define MonitorTCPIPH
//---------------------------------------------------------------------------
#include "TCPData.h"
#include "HThreadCtrl.h"
//---------------------------------------------------------------------------

class MonitorTCPIP
{
    private:

    protected:
        TCPIP*          _socket;
        HThreadCtrl     _socket_thread;

        TMemo*          _memo;

        vector< int >   _error_code;

        int    _bCommandReady[10];

    public:
        MonitorTCPIP( TCPIP* pSocket );
        ~MonitorTCPIP();

        void InitialSocket( AnsiString sAddress , int iPort );
        void InitialMemo( TMemo* pMemo );

        void Connect();
        void Disconnect();
        bool IsConnect();

        int SendCommand( DATA SendData );
        int SendCommand( AnsiString SendData );

        int AdapterData( int& iType , DATA& vData );
        void ShowData( AnsiString sType , DATA vData );

        String GetDataString();
        int GetReceiveCommand(int index);

};
void MV_TCPIP_WATCH( void* pParameter );

#endif
