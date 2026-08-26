//---------------------------------------------------------------------------
#include "MachineDefine.h"

#include <vcl.h>
#pragma hdrstop

#include "Delta.h"
#include "DMT.h"
#include <ScktComp.hpp>
#include <stdio.h>

//---------------------------------------------------------------------------

#pragma package(smart_init)
//---------------------------------------------------------------------------
const int MAX_SEGMENT_D=64;
const int MAX_SEGMENT_Y=64;
const int MAX_SEGMENT_X=256;
const int MAX_SEGMENT_M=256;

//---------------------------------------------------------------------------
__fastcall DeltaPLC::DeltaPLC()
{
    conn_num  = 0;
    comm_type = 1;
    ip        = 0;
    asErrMessage = *NullStr;
    bConnection=false;
}
//==============================================================================
DeltaPLC::~DeltaPLC()
{
    CloseModbusSocket();
}
//==============================================================================
void DeltaPLC::SetConnNumeCommTypeIP(int iConnNum, int iCommType,AnsiString asIP)
{
    conn_num  = iConnNum;
    comm_type = iCommType;
    ip = inet_addr((asIP).c_str());
}
//==============================================================================
bool DeltaPLC::OpenModbusSocket()
{
    asErrMessage = *NullStr;
    if(OpenModbusTCPSocket(conn_num, ip) == -1)
    {
        asErrMessage="Connection Failed";
        bConnection=false;
    }
    else
        bConnection=true;
    return bConnection;
}
//==============================================================================
void DeltaPLC::CloseModbusSocket()
{
    bConnection=false;
    CloseSocket(conn_num);
}
//==============================================================================
bool DeltaPLC::GetConnection()
{
    return bConnection;
}
//==============================================================================
AnsiString DeltaPLC::GetErrMessage()
{
    return asErrMessage;
}
//==============================================================================
/*
int ReadCoilsA(int comm_type, int conn_num, int slave_addr, int dev_addr, int qty, unsigned int *data_r, char *req, char *res);
參數
comm_type         通訊種類, 0:序列埠, 1:乙太網路
conn_num            序列埠編號或連線識別碼
slave_addr          站號
dev_addr            MODBUS位址 (可由DevToAddrA函式計算得到)
qty                        讀取筆數,支援範圍1~2000
data_r                  讀取到的數據, 0:Coil Off, 1:Coil On
req                       MODBUS Request字串
res                       MODBUS Response字串
描述
從指定MODBUS位址(Coil)讀取指定筆數資料
回傳值
-1            資料存取失敗
qty          資料存取成功
*/
bool DeltaPLC::ReadCoils(char *cDev_addr,int qty, unsigned int *data_r)
{
    int dev_addr=DevToAddrA("DVP",cDev_addr,1);
    int slave_addr=1;
    int iRes=ReadCoilsA(comm_type,conn_num,slave_addr,dev_addr,qty,data_r, sendbuf, recvbuf);
    if(iRes>=1)
        return true;
    return false;
}
//==============================================================================
/*
int ReadInputsA(int comm_type, int conn_num, int slave_addr, int dev_addr, int qty, unsigned int *data_r, char *req, char *res);
參數
comm_type         通訊種類, 0:序列埠, 1:乙太網路
conn_num            序列埠編號或連線識別碼
slave_addr          站號
dev_addr            MODBUS位址 (可由DevToAddrA函式計算得到)
qty                        讀取筆數,支援範圍1~2000
data_r                  讀取到的數據, 0:Discrete Input Off, 1:Discrete Input On
req                       MODBUS Request字串
res                       MODBUS Response字串
描述
從指定MODBUS位址(Discrete Input)讀取指定筆數資料
回傳值
-1            資料存取失敗
qty          資料存取成功
*/
bool DeltaPLC::ReadInputs(char *cDev_addr,int qty, unsigned int *data_r)
{
    int dev_addr=DevToAddrA("DVP",cDev_addr,1);
    int slave_addr=1;
    int iRes=ReadInputsA(comm_type,conn_num,slave_addr,dev_addr,qty,data_r, sendbuf, recvbuf);
    if(iRes>=1)
        return true;
    return false;
}
//==============================================================================
/*
int WriteSingleCoilA(int comm_type, int conn_num, int slave_addr, int dev_addr, unsigned int data_w, char *req, char *res);
參數
comm_type         通訊種類, 0:序列埠, 1:乙太網路
conn_num            序列埠編號或連線識別碼
slave_addr          站號
dev_addr            MODBUS位址 (可由DevToAddrA函式計算得到)
data_w                欲寫入之數據, 0: Coil Off, 非0: Coil On
req                       MODBUS Request字串
res                       MODBUS Response字串
描述
寫入數據到指定MODBUS位址(Coil)當中
回傳值
-1         資料存取失敗
1          資料存取成功
*/
bool DeltaPLC::WriteSingleCoil(char *cDev_addr,int data_w)
{
    int dev_addr=DevToAddrA("DVP",cDev_addr,1);
    int slave_addr=1;
    int iRes=WriteSingleCoilA(comm_type,conn_num,slave_addr,dev_addr,data_w, sendbuf, recvbuf);
    if(iRes==1)
        return true;
    return false;
}
//==============================================================================
/*
int ReadHoldRegsA(int comm_type, int conn_num, int slave_addr, int dev_addr, int qty, unsigned int *data_r, char *req, char *res);
參數
comm_type         通訊種類, 0:序列埠, 1:乙太網路
conn_num            序列埠編號或連線識別碼
slave_addr          站號
dev_addr            MODBUS位址 (可由DevToAddrA函式計算得到)
qty                        讀取筆數,支援範圍1~125
data_r                  讀取到的數據
req                       MODBUS Request字串
res                       MODBUS Response字串
描述
從指定MODBUS位址(Holding Register)讀取指定筆數資料
回傳值
-1            資料存取失敗
qty          資料存取成功
*/
bool DeltaPLC::ReadHoldRegs(char *cDev_addr,int qty,unsigned int *data_r)
{
    if(qty>125)
        return false;
    int dev_addr=DevToAddrA("DVP",cDev_addr,1);
    int slave_addr=1;
    int iRes=ReadHoldRegsA(comm_type,conn_num,slave_addr,dev_addr,qty,data_r, sendbuf, recvbuf);
    if(iRes>=1)
        return true;
    return false;
}
//==============================================================================
/*
int ReadInputRegsA(int comm_type, int conn_num, int slave_addr, int dev_addr, int qty, unsigned int *data_r, char *req, char *res);
參數
comm_type         通訊種類, 0:序列埠, 1:乙太網路
conn_num            序列埠編號或連線識別碼
slave_addr          站號
dev_addr            MODBUS位址 (可由DevToAddrA函式計算得到)
qty                        讀取筆數,支援範圍1~125
data_r                  讀取到的數據
req                       MODBUS Request字串
res                       MODBUS Response字串
描述
從指定MODBUS位址(Input Register)讀取指定筆數資料
回傳值
-1            資料存取失敗
qty          資料存取成功
*/
bool DeltaPLC::ReadInputRegs(char *cDev_addr)
{
    int dev_addr=DevToAddrA("DVP",cDev_addr,1);
    int slave_addr=1;
    int qty=1;
    unsigned int data_r;
    int iRes=ReadInputRegsA(comm_type,conn_num,slave_addr,dev_addr,qty,&data_r, sendbuf, recvbuf);
    if(iRes>=1)
        return true;
    return false;
}
//==============================================================================
/*
int WriteSingleRegA(int comm_type, int conn_num, int slave_addr, int dev_addr, unsigned int data_w, char *req, char *res);
參數
comm_type         通訊種類, 0:序列埠, 1:乙太網路
conn_num            序列埠編號或連線識別碼
slave_addr          站號
dev_addr            MODBUS位址 (可由DevToAddrA函式計算得到)
data_w                欲寫入之數據, 支援範圍0~65535 (超過則以65535計)
req                       MODBUS Request字串
res                       MODBUS Response字串
描述
寫入數據到指定MODBUS位址(Holding Register)當中
回傳值
-1         資料存取失敗
1          資料存取成功
*/
bool DeltaPLC::WriteSingleReg(char *cDev_addr,unsigned int data_w)
{
    int dev_addr=DevToAddrA("DVP",cDev_addr,1);
    int slave_addr=1;
    int iRes=WriteSingleRegA(comm_type,conn_num,slave_addr,dev_addr,data_w, sendbuf, recvbuf);
    if(iRes==1)
        return true;
    return false;
}
//==============================================================================
/*
int WriteMultiCoilsA(int comm_type, int conn_num, int slave_addr, int dev_addr, int qty, unsigned int *data_w, char *req, char *res);
參數
comm_type         通訊種類, 0:序列埠, 1:乙太網路
conn_num          序列埠編號或連線識別碼
slave_addr        站號
dev_addr          MODBUS位址 (可由DevToAddrA函式計算得到)
qty               寫入 筆數,支援範圍1~1968
data_w            欲寫入的數據,  0:Coil Off, 非0:Coil On
req               MODBUS Request字串
res               MODBUS Response字串
描述
從指定MODBUS位址(Coil)開始寫入指定筆數資料
回傳值
-1            資料存取失敗
qty          資料存取成功
*/
bool DeltaPLC::WriteMultiCoils(char *cDev_addr,unsigned int *data_w,int qty)
{
    int dev_addr=DevToAddrA("DVP",cDev_addr,1);
    int slave_addr=1;
    int iRes=WriteMultiCoilsA(comm_type,conn_num,slave_addr,dev_addr,qty,data_w, sendbuf, recvbuf);
    if(iRes>=1)
        return true;
    return false;
}
//==============================================================================
/*
int ReadInputRegsA(int comm_type, int conn_num, int slave_addr, int dev_addr, int qty, unsigned int *data_r, char *req, char *res);
參數
comm_type         通訊種類, 0:序列埠, 1:乙太網路
conn_num            序列埠編號或連線識別碼
slave_addr          站號
dev_addr            MODBUS位址 (可由DevToAddrA函式計算得到)
qty                        讀取筆數,支援範圍1~125
data_r                  讀取到的數據
req                       MODBUS Request字串
res                       MODBUS Response字串
描述
從指定MODBUS位址(Input Register)讀取指定筆數資料
回傳值
-1            資料存取失敗
qty          資料存取成功
*/
//==============================================================================
/*
int WriteMultiRegsA(int comm_type, int conn_num, int slave_addr, int dev_addr, int qty, unsigned int *data_w, char *req, char *res);
參數
comm_type         通訊種類, 0:序列埠, 1:乙太網路
conn_num            序列埠編號或連線識別碼
slave_addr          站號
dev_addr            MODBUS位址 (可由DevToAddrA函式計算得到)
qty                        寫入 筆數,支援範圍1~123
data_w                欲寫入的數據,  支援範圍0~65535 (超過則以65535計)
req                       MODBUS Request字串
res                       MODBUS Response字串
描述
從指定MODBUS位址(Holding Register)開始寫入指定筆數資料
回傳值
-1            資料存取失敗
qty          資料存取成功
*/
//==============================================================================
bool DeltaPLC::WriteMultiRegs(char *cDev_addr,unsigned int *data_w,int qty)
{
    int dev_addr=DevToAddrA("DVP",cDev_addr,1);
    int slave_addr=1;
    int iRes=WriteMultiRegsA(comm_type,conn_num,slave_addr,dev_addr,qty,data_w, sendbuf, recvbuf);//一次寫入多次//D//筆數1~123//Register_D_Max=5000//目前最大值=D510.
    if(iRes>=1)
        return true;
    return false;
}
//==============================================================================
bool DeltaPLC::CheckRequestComm(AnsiString strReq)
{
    int i;
    AnsiString strValid = "0123456789ABCDEF";
    if(strReq.Length() < 4)                                                     // at least slave address and function code
    {
        asErrMessage="Invalid Modbus Dat";
        return false;
    }

    if(strReq.Length()%2 != 0)                                                  // input data must be even number
    {
        asErrMessage="Modbus Data Must Be Even Number";
        return false;
    }

    for(i = 1 ; i <= strReq.Length() ; ++i)                                     // index of AnsiString starts from 1
    {
        if(strValid.Pos(strReq[i]) == 0)                                        // input data verification
        {
            asErrMessage="Invalid Modbus Data";
            return false;
        }
    }
    return true;
}
//==============================================================================
bool DeltaPLC::SendRequestComm(AnsiString strReq)
{
    asErrMessage = *NullStr;
    AnsiString strRes = *NullStr;
    void (WINAPI *ptrClose)(int); // function pointer for disconnection
    if(OpenModbusTCPSocket(conn_num, ip) == -1)
    {
            asErrMessage="Connection Failed";
            return false;
    }
    ptrClose = CloseSocket;
    int modbus_addr = 0;
    int modbus_func = 0;
    int modbus_addr_ret = 0;
    int modbus_func_ret = 0;
    int sendlen = 0;
    int i = 0;
    if(CheckRequestComm(strReq)==false)
    {
        ptrClose(conn_num);
        return false;
    }
    for(i = 0 ; i <= strReq.Length()-2 ; i+=2) // trans data into bytes and put it into sendbuf
    {
        char* tmp = strReq.c_str();
        AnsiString strTmp = "0x";
        strTmp.Insert(*(tmp+i),strTmp.Length()+1);
        strTmp.Insert(*(tmp+i+1),strTmp.Length()+1);
        if(i == 0)
          modbus_addr = StrToInt(strTmp);
        else if(i == 2)
          modbus_func = StrToInt(strTmp);
        else
          sendbuf[sendlen++] = StrToInt(strTmp);
    }
    int req = RequestData(comm_type, conn_num, modbus_addr, modbus_func, sendbuf, sendlen); // modbus request
    if(req == -1)
    {
        asErrMessage="Request Failed";
        ptrClose(conn_num);
        return false;
    }
    int res = ResponseData(comm_type, conn_num, &modbus_addr_ret, &modbus_func_ret, recvbuf); // modbus response
    if(res > 0)
    {
        AnsiString strTmp;
        strTmp.sprintf("%02X",modbus_addr_ret);
        strRes += strTmp;
        strTmp.sprintf("%02X",modbus_func_ret);
        strRes += strTmp;
        switch (modbus_func_ret)
        {
            case 0x01:  case 0x02:  case 0x03:
            case 0x04:  case 0x11:  case 0x17:
            strTmp.sprintf("%02X",res);
            strRes += strTmp;
            break;
        }
        for(i = 0 ; i < res ; ++i) // recover a string from recvbuf
        {
            strRes.Insert(IntToHex(recvbuf[i],2),strRes.Length()+1);
        }
        asErrMessage="Request Done";
    }
    else
    {
        asErrMessage="No Data Received";
    }
    ptrClose(conn_num);
    return false;
}
//==============================================================================
bool DeltaPLC::SendRequestComm_D(int iSegment)
{
    int modbus_addr = 01;
    int modbus_func = 03;
    int modbus_addr_ret = 0;
    int modbus_func_ret = 0;
    int sendlen = 4;
    int i,iStartSegment;

    iStartSegment=MAX_SEGMENT_D*iSegment;

    sendbuf[0]=0x10+(iStartSegment/0x100);
    sendbuf[1]=0x00+(iStartSegment%0x100);
    sendbuf[2]=0x00;
    sendbuf[3]=0x40;

    int req = RequestData(comm_type, conn_num, modbus_addr, modbus_func, sendbuf, sendlen); // modbus request
    if(req == -1)
    {
        return false;
    }
    int res = ResponseData(comm_type, conn_num, &modbus_addr_ret, &modbus_func_ret, recvbuf); // modbus response
    if(res > 0)
    {
        for(i=0; i<MAX_SEGMENT_D; i++)
        {
            iRegisterD[i+iStartSegment]=recvbuf[i*2]*0x100+recvbuf[i*2+1];
        }
        return true;
    }
    return false;
}
//==============================================================================
bool DeltaPLC::WriteRequestComm_D(int iSegment)
{
    bool bFlag;
    char str[64];
    int iStartSegment;

    iStartSegment=MAX_SEGMENT_D*iSegment;
    sprintf(str,"D%d",iStartSegment);
    bFlag=WriteMultiRegs(str,&iRegisterD[iStartSegment],MAX_SEGMENT_D);
    return bFlag;
}
//==============================================================================
bool DeltaPLC::SendRequestComm_X(int iSegment)
{
    int modbus_addr = 01;
    int modbus_func = 02;
    int modbus_addr_ret = 0;
    int modbus_func_ret = 0;
    int sendlen = 4;
    int i,iStartSegment;

    iStartSegment=MAX_SEGMENT_X*iSegment;

    sendbuf[0]=0x04+(iStartSegment/0x100);
    sendbuf[1]=0x00+(iStartSegment%0x100);
    sendbuf[2]=0x01;
    sendbuf[3]=0x00;
    int req = RequestData(comm_type, conn_num, modbus_addr, modbus_func, sendbuf, sendlen); // modbus request
    if(req == -1)
    {
        return false;
    }
    int res = ResponseData(comm_type, conn_num, &modbus_addr_ret, &modbus_func_ret, recvbuf); // modbus response
    if(res > 0)
    {
        for(i=0; i<MAX_SEGMENT_X; i++)
        {
            if((recvbuf[i/8]&(0x01<<(i%8)))>=1)
                iRegisterX[i+iStartSegment]=1;
            else
                iRegisterX[i+iStartSegment]=0;
        }
        return true;
    }
    return false;
}
//==============================================================================
bool DeltaPLC::SendRequestComm_M(int iSegment)
{
    int modbus_addr = 01;
    int modbus_func = 01;
    int modbus_addr_ret = 0;
    int modbus_func_ret = 0;
    int sendlen = 4;
    int i,iStartSegment;

    iStartSegment=MAX_SEGMENT_X*iSegment;

    sendbuf[0]=0x08+(iStartSegment/0x100);
    sendbuf[1]=0x00+(iStartSegment%0x100);
    sendbuf[2]=0x01;
    sendbuf[3]=0x00;
    int req = RequestData(comm_type, conn_num, modbus_addr, modbus_func, sendbuf, sendlen); // modbus request
    if(req == -1)
    {
        return false;
    }
    int res = ResponseData(comm_type, conn_num, &modbus_addr_ret, &modbus_func_ret, recvbuf); // modbus response
    if(res > 0)
    {
        for(i=0; i<MAX_SEGMENT_M; i++)
        {
            if((recvbuf[i/8]&(0x01<<(i%8)))>=1)
                iRegisterM[i+iStartSegment]=1;
            else
                iRegisterM[i+iStartSegment]=0;
        }
        return true;
    }
    return false;
}
//==============================================================================
bool DeltaPLC::WriteRequestComm_M(int iSegment)
{
    bool bFlag;
    char str[64];
    int iStartSegment;

    iStartSegment=MAX_SEGMENT_M*iSegment;
    sprintf(str,"M%d",iStartSegment);
    //bFlag=WriteMultiCoils(str,&iRegisterM[iStartSegment],MAX_SEGMENT_M);
    bFlag=WriteMultiCoils(str,&iRegisterM[iStartSegment],MAX_SEGMENT_M);
    return bFlag;
}
//==============================================================================
bool DeltaPLC::SendRequestComm_Y(int iSegment)
{
    char str[64];
    int iStartSegment;

    iStartSegment=MAX_SEGMENT_Y*iSegment;
    sprintf(str,"Y%d",MAX_SEGMENT_Y*iSegment);
    bool bFlag=ReadHoldRegs(str,MAX_SEGMENT_Y,&iRegisterY[iStartSegment]);
    return bFlag;
}
//==============================================================================
bool DeltaPLC::WriteRequestComm_Y(int iSegment)
{
    bool bFlag;
    char str[64];
    int iStartSegment;

    iStartSegment=MAX_SEGMENT_M*iSegment;
    sprintf(str,"Y%d",iStartSegment);
    bFlag=WriteMultiRegs(str,&iRegisterY[iStartSegment],MAX_SEGMENT_Y);
    return bFlag;
}
//==============================================================================
