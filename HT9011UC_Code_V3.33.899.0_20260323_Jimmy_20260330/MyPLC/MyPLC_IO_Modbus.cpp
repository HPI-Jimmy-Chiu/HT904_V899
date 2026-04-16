//------------------------------------------------------------------------------
#include "MachineDefine.h"
#pragma hdrstop

#include "MyPLC_IO_Modbus.h"

#include "ModbusTCPClient.h"
#include "cmydef.h"
#include "common.h"
#include "mymessbox.h"
#pragma package(smart_init)
//------------------------------------------------------------------------------

TPLCIOThread *MyPLCIOThread=NULL;
ModbusTCPClient PlcComm;
std::vector<TMyPLC_IO_Modbus*> TMyPLC_IO_Modbus::PLC_IO_Members;

int iTransactionID=0;
int iPLCIOTask=-1;
int iPLCStatusTask=-1;                                                          //ben 20230913 add

bool bOSSDData[INPUT_MAX_Slave][2][8];
bool bScanSlave[INPUT_MAX_Slave];
bool bPLCInData[INPUT_MAX_Slave][INPUT_MAX_REGISTER][8];
bool bPLCIO[2048][8];                                                           //for 舊架構
bool bPLCIOEffect=false;
bool bIOPowered=false;
bool bSafePLCThread=false;                                                      //ben 20230913 add

byte InPortData_Byte[INPUT_MAX_Slave][INPUT_MAX_REGISTER];

HTimer hCheckTime;
HTimer hCheckThreadTime;                                                        //ben 20230913 add
//------------------------------------------------------------------------------
void InitPLCIO(AnsiString asIP,int iPort)
{
    int slave;
    iPLCIOTask=1;
    iPLCStatusTask=1;                                                           //ben 20230913 add
    for(int i=0; i<(int)TMyPLC_IO_Modbus::PLC_IO_Members.size(); i++)
    {
        slave=(TMyPLC_IO_Modbus::PLC_IO_Members.at(i))->GetSlave();

        if(slave>=0 && slave<INPUT_MAX_Slave)                                   //僅掃描有使用的
        {
            bScanSlave[slave]=true;
        }
    }
    PlcComm.SetTCPInfo(asIP,iPort);

    PlcComm.SetScan(false);
    PlcComm.SetScan(true);

    if(MyPLCIOThread==NULL)
    {
        MyPLCIOThread=new TPLCIOThread(false);
        MyPLCIOThread->Priority=tpNormal;
        MyPLCIOThread->Resume();                                                //ben 20230913 add
    }
}
//------------------------------------------------------------------------------
__fastcall TPLCIOThread::TPLCIOThread(bool CreateSuspended)
    : TThread(CreateSuspended)
{

}
//------------------------------------------------------------------------------
void __fastcall TPLCIOThread::PLCIOProcess(void)
{
    PLCIOTaskCycle();
    PLCStatusCheck();                                                           //ben 20230913 add
    PlcComm.Cycle();
}
//------------------------------------------------------------------------------
bool bFirstConnect=true;
TQPF_Timer tCycleConnect;
void PLCStatusCheck()                                                           //ben 20230913 add
{
    if(PlcComm.IsConnected()==false)
    {
        if(Enable_PLCSafety_IO)                                                 //Austin 20190531.01 增加PLC_IO模組及安全相關IO. //Jason 20230619 增加安全PLC部分
        {
/*            if(bFirstConnect==false)                                            //JerryYang 20250206 : fix //Ken 20250428 : Mark
            {
                for(int i=0; i<INPUT_MAX_Slave; i++)
                {
                    for(int j=0; j<INPUT_MAX_REGISTER; j++)
                    {
                        if(j>=ePLCSafeDoor1)                                    //KenHsieh 20250208 : Fix PLC 斷線
                        {
                            for(int k=0; k<8; k++)
                            {
                                if(j>=20)                                       //OSSD 安全輸出 30021 30022
                                {
                                    bOSSDData[i][j-20][k]=InPortData_Byte[i][j] & MyBitMask[k];
                                }
                                else
                                {
                                   bPLCInData[i][j][k]=false;
                                }
                            }
                        }
                    }
                }

                ShowMyMessage("PLC disconnect. Initial PLC!");
            }*/

            if(bFirstConnect || tCycleConnect.Off())
            {
                bFirstConnect=false;
                bScanSlave[0]=true;
                InitPLCIO("172.16.8.120", 502);                                 //固定
                tCycleConnect.SetMSAndOn(5000);
            }
        }
        return;
    }
    else
    {
        tCycleConnect.SetMSAndOn(5000);                                         //JerryYang 20250206 : fix
    }

    int &Task=iPLCStatusTask;

    switch(Task)
    {
        case 1:
            hCheckThreadTime.SetSecAndOn(1);
            Task=10;
            break;
        case 10:
            if(hCheckThreadTime.Off())
            {
                bSafePLCThread=!bSafePLCThread;
                Task=1;
            }
            break;
    }
}
//------------------------------------------------------------------------------
void PLCIOTaskCycle()
{
    if(PlcComm.IsConnected()==false)
        return;

    int &Task=iPLCIOTask;
    static int iSlave=0;

    VTBYTEDATA data;
    data.clear();
    AnsiString  asTemp;

    switch(Task)
    {
        case 1:
            if(iSlave<0 || iSlave>=INPUT_MAX_Slave)
            {
                for(int i=0; i<INPUT_MAX_Slave; i++)
                {
                    if(bScanSlave[i])
                    {
                        iSlave=i;
                        i=INPUT_MAX_Slave;
                    }
                }
            }

            if(iTransactionID>0xffff)
                iTransactionID=0;                                               // 0-0xffff (2byte)

            if(bScanSlave[iSlave])
            {
                data.push_back(HIBYTE((INPUT_MAX_REGISTER+1)/2));               //一個Register為1個Byte 但一個位址包含兩個Register
                data.push_back(LOBYTE((INPUT_MAX_REGISTER+1)/2));
                //PLC Reg 30001 == 0x00 回應的資料為2Byte資料 30002 30001 但30001為系統狀態 輸入暫存器從30002開始
                //PLC Reg 30003 == 0x01 回應的資料為2Byte資料 30004 30003
                PlcComm.SendData(iTransactionID, iSlave, 4, 0x00, data);
                hCheckTime.SetSecAndOn(1);
                Task=100;
            }
            else
            {
                iSlave++;
            }
            break;
        case 100:
            if(PlcComm.GetRecevie(iTransactionID,data))                         //讀取成功
            {                                                                   //寫入Byte陣列
                for(int i=0; i<(int)data.size(); i++)
                {
                    if(i<INPUT_MAX_REGISTER)
                    {                                                           //資料順序整理 因PLC暫存器是先放高位資料 所以 0->1  1->0  2->3  3->2
                        InPortData_Byte[iSlave][(i/2)*2+abs(i%2-1)]=data[i];
                    }
                }

                for(int i=0; i<INPUT_MAX_Slave; i++)                            //寫入boolean陣列
                {
                    for(int j=0; j<INPUT_MAX_REGISTER; j++)
                    {
                        for(int k=0; k<8; k++)
                        {
                            if(j>=20)                                           //OSSD 安全輸出 30021 30022
                            {
                                bOSSDData[i][j-20][k]=InPortData_Byte[i][j]&MyBitMask[k];
                            }
                            else
                            {
                                bPLCInData[i][j][k]=InPortData_Byte[i][j]&MyBitMask[k];
                            }
                        }

                    }
                }

                bIOPowered=bOSSDData[0][0][0];

                //if(iPLCSafetyVer==202204)                                     //joe 20220406.01 debug safe plc
                    bIOPowered=true;

                bPLCIOEffect=true;                                              //完成過一次
                iSlave++;
                iTransactionID++;
                Task=1;
            }
            else
            {
                if(hCheckTime.Off())
                {
                    iSlave++;
                    iTransactionID++;
                    Task=1;
                }
            }
            break;
    }
}
//------------------------------------------------------------------------------
void __fastcall TPLCIOThread::Execute()
{
    do
    {
        Synchronize(PLCIOProcess);
        MySleepEx(1,true);
    } while (!Terminated);
}
//------------------------------------------------------------------------------
__fastcall TMyPLC_IO_Modbus::TMyPLC_IO_Modbus()
{
    TMyPLC_IO_Modbus::PLC_IO_Members.push_back(this);
    iSlave=-1;
    iPort=-1;
    iBit=-1;
}
//------------------------------------------------------------------------------
void TMyPLC_IO_Modbus::SetPortInformation(int Card, int Port, int Bit)
{
    iSlave=Card;
    iPort=Port;
    iBit=Bit;
}
//------------------------------------------------------------------------------
bool TMyPLC_IO_Modbus::IsOn()
{
    if(iSlave>=0    && iSlave<INPUT_MAX_Slave       &&
       iPort>=PLCIO_INPUT_START_PORT_NUMBER &&
       iPort<(INPUT_MAX_REGISTER*2+PLCIO_INPUT_START_PORT_NUMBER)   &&
       iBit>=0      && iBit<8)
    {
        if(iPLCSafetyVer==202204)
            return bPLCInData[iSlave][iPort-PLCIO_INPUT_START_PORT_NUMBER+2][iBit];//joe 20220406.01 debug safe plc
        else
            return bPLCInData[iSlave][iPort-PLCIO_INPUT_START_PORT_NUMBER+1][iBit];
    }
    else
    {
        return false;
    }
}
//------------------------------------------------------------------------------
bool TMyPLC_IO_Modbus::IsOff()
{
    return !IsOn();
}
//------------------------------------------------------------------------------
void TMyPLC_IO_Modbus::On()                                                     //Output模組 暫無使用 (預留)
{

}
//------------------------------------------------------------------------------
void TMyPLC_IO_Modbus::Off()                                                    //Output模組 暫無使用 (預留)
{

}
//------------------------------------------------------------------------------
//Byte TMyPLC_IO_Modbus::IOInputByte()
//{
//
//}
//------------------------------------------------------------------------------
void TMyPLC_IO_Modbus::IOByteOut(int port, byte Byte)                           //Output模組 暫無使用 (預留)
{

}
//------------------------------------------------------------------------------
void TMyPLC_IO_Modbus::InitialMyOutIOData()
{

}
//------------------------------------------------------------------------------
void bPLCStatusCheck()                                                          //kevin 20250407 check PLC
{
    PLCIOTaskCycle();
    PLCStatusCheck();                                                           //ben 20230913 add
    PlcComm.Cycle();
}
//------------------------------------------------------------------------------
