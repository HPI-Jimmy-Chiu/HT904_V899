//------------------------------------------------------------------------------
#ifndef ModbusTCPClientH
#define ModbusTCPClientH
#include "SPComm.hpp"
#include <Classes.hpp>
#include <ScktComp.hpp>
#include <iostream>
#include <vector>
#include <map>

using namespace std;
typedef vector<BYTE> VTBYTEDATA;
typedef map<int, VTBYTEDATA> MAP_VTBYTEDATA;

class ModbusTCPClient
{
    private:
        int iIP;
        int iPort;
        MAP_VTBYTEDATA mapReplyData;                                            //回應資料
        vector<int> vtEarseData;
        TClientSocket* pClinetSocket;
        bool bConnected;
        CRITICAL_SECTION cs;

    public:
        __fastcall ModbusTCPClient();
        ~ModbusTCPClient();
        void SetScan(bool bSet);
        void Cycle();
        void SetTCPInfo(AnsiString asIP, int Port);
        bool Connect();
        bool DisConnect();
        bool IsConnected();
        bool SendData(int iID, int iSlave, int iFunctionCode, int iAddr, VTBYTEDATA data);
        bool GetRecevie(int iID, VTBYTEDATA &data, bool bRemove=true);

        void __fastcall SocketConnect(TObject *Sender, TCustomWinSocket *Socket);
        void __fastcall SocketDisConnect(TObject *Sender, TCustomWinSocket *Socket);
        void __fastcall SocketError(TObject *Sender, TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode);
        void __fastcall SocketRead(TObject *Sender, TCustomWinSocket *Socket);
};
//------------------------------------------------------------------------------
#endif
