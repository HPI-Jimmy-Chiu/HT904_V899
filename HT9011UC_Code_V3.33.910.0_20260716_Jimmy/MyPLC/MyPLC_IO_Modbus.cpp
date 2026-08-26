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

std::vector<TMyPLC_IO_Modbus*> TMyPLC_IO_Modbus::PLC_IO_Members;

ModbusTCPClient g_PlcComm[ePLCGrp_Count];
int g_iTransactionID[ePLCGrp_Count];
int g_iPLCIOTask[ePLCGrp_Count];
int g_iPLCStatusTask[ePLCGrp_Count];                                                          //ben 20230913 add

bool bOSSDData[ePLCGrp_Count][READ_BYTE_COUNT_OSSD][8];                         //所有廠牌共用此參數所以得要宣告最大長度
bool bScanSlave[ePLCGrp_Count];
bool bPLCInData[ePLCGrp_Count][INPUT_MAX_REGISTER][8];                          //所有廠牌共用此參數所以得要宣告最大長度
bool g_bEnSubPage[ePLCGrp_Count] = {
    true,   //ePLCGrp_Safe
};
bool bPLCIO[2048][8];                                                           //for 舊架構
bool bPLCIOEffect=false;
bool bIOPowered=false;
bool bSafePLCThread=false;                                                      //ben 20230913 add

byte InPortData_Byte[ePLCGrp_Count][INPUT_MAX_REGISTER];                        //Raw data經過 Big-endian排序後的結果

HTimer hCheckTime;
HTimer hCheckThreadTime;                                                        //ben 20230913 add

TPLCIOInfo g_tPLCIOInfo;
//------------------------------------------------------------------------------
void InitPLCIO(AnsiString asIP,int iPort, EPLCGrp ePLCGrp)
{
    int slave;
    g_iPLCIOTask[ePLCGrp]=1;
    g_iPLCStatusTask[ePLCGrp]=1;                                                //ben 20230913 add
    for(int i=0; i<(int)TMyPLC_IO_Modbus::PLC_IO_Members.size(); i++)
    {
        slave=(TMyPLC_IO_Modbus::PLC_IO_Members.at(i))->GetSlave();

        if(slave>=0 && slave<ePLCGrp_Count)                                     //僅掃描有使用的
        {
            bScanSlave[slave]=true;
        }
    }
    g_PlcComm[ePLCGrp].SetTCPInfo(asIP,iPort);

    g_PlcComm[ePLCGrp].SetScan(false);
    g_PlcComm[ePLCGrp].SetScan(true);

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
    for(int i = 0; i<ePLCGrp_Count; ++i)
    {
        PLCIOTaskCycle(EPLCGrp(i));
        PLCStatusCheck(EPLCGrp(i));
        g_PlcComm[i].Cycle();
    }
}
//------------------------------------------------------------------------------
void InitPLCIO_All()
{
    AnsiString asIP = "172.16.8.", asIPTmp;//"172.16.8.120"
    int iIPv4_4th = 120, iPort = MODBUS_PORT;
    
    for(int i = 0; i<ePLCGrp_Count; ++i)//each group meas that different slave
    {
        if(g_bEnSubPage[i])
        {
            bScanSlave[i] = true;
            asIPTmp = asIP + IntToStr(iIPv4_4th+i);
            InitPLCIO(asIPTmp, iPort, EPLCGrp(i));
        }
        else
        {
            bScanSlave[i] = false;
        }
    }
}
//------------------------------------------------------------------------------
bool bFirstConnect=true;
TQPF_Timer tCycleConnect;
void PLCStatusCheck(EPLCGrp ePLCGrp)                                            //ben 20230913 add
{
//    int iReadByteCount_InpData = g_tPLCIOInfo.m_iReadByteCount_InpData;
//    int iReadByteCount_OSSD    = g_tPLCIOInfo.m_iReadByteCount_OSSD;
//    int iReadByteCount         = g_tPLCIOInfo.m_iReadByteCount;

    if(g_PlcComm[ePLCGrp].IsConnected()==false)
    {
        if(IsSafePLCIOInstall())                                                //Austin 20190531.01 增加PLC_IO模組及安全相關IO. //Jason 20230619 增加安全PLC部分
        {
/*            if(bFirstConnect==false)                                            //JerryYang 20250206 : fix //Ken 20250428 : Mark
            {
                for(int i=0; i<ePLCGrp_Count; i++)
                {
                    for(int j=0; j<INPUT_MAX_REGISTER; j++)
                    {
                        if(j>=ePLCSafeDoor1)                                    //KenHsieh 20250208 : Fix PLC 斷線
                        {
                            for(int k=0; k<8; k++)
                            {
                                //依照各自廠牌的個數來填入對應的變數，避免不同廠牌的變數長度不一樣造成陣列越界的問題
                                if(j>=iReadByteCount_InpData && j<(iReadByteCount_InpData+iReadByteCount_OSSD))//OSSD 安全輸出 30021 30022
                                {
                                    bOSSDData[i][j-iReadByteCount_InpData][k]=InPortData_Byte[i][j] & MyBitMask[k];
                                }
                                else if(0<=j && j<iReadByteCount_InpData)
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
                InitPLCIO_All();
                tCycleConnect.SetMSAndOn(5000);
            }
        }
        return;
    }
    else
    {
        tCycleConnect.SetMSAndOn(5000);                                         //JerryYang 20250206 : fix
    }

    int &Task=g_iPLCStatusTask[ePLCGrp];

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
FILE *g_fpPLCRecvData = NULL;
AnsiString g_asPLCRecvData_FileName_Pre = "";
AnsiString g_asPLCRecvData_Log_Pre = "";

FILE *g_fpPLCRecvBit = NULL;
AnsiString g_asPLCRecvBit_FileName_Pre = "";

enum{
    ePLCRecvBit_PLCIn = 0,
    ePLCRecvBit_OSSD,
};

typedef enum E_PLCLogFormat
{
    ePLCLogFormat_LogTimeStampAndRecvStr = 0,               //log log timestamp and recved data every time
    ePLCLogFormat_LogTimeStampIfRecvSameStr,                //if received data is same as previous log, just log timestamp, otherwise log timestamp and recved data
    ePLCLogFormat_LogTimeStampAndRecvStrJustForDiffStr,     //just log timestamp and recved data if recved data is different from previous log, not log anything if recved data is same as previous log
}EPLCLogFormat;

AnsiString g_asPLCRecvBit_Log_PLCIn_Pre = "";
AnsiString g_asPLCRecvBit_Log_OSSD_Pre = "";
//------------------------------------------------------------------------------
static bool EnsureFolderExists(const AnsiString &asFolderPath)
{
    AnsiString asPath = ExcludeTrailingPathDelimiter(asFolderPath);

    if(asPath == "")
        return false;

    if(DirectoryExists(asPath))
        return true;

    if(ForceDirectories(asPath))
        return true;

    return DirectoryExists(asPath);
}
//------------------------------------------------------------------------------
void CloseFile_PLCRecvDataLog()
{
    if(g_fpPLCRecvData)
    {
        fclose(g_fpPLCRecvData);
        g_fpPLCRecvData = NULL;
    }

    if(g_fpPLCRecvBit)
    {
        fclose(g_fpPLCRecvBit);
        g_fpPLCRecvBit = NULL;
    }
}
//------------------------------------------------------------------------------
void WriteFile_PLCRecvVal_Base(AnsiString asLog, EPLCLogFormat ePLCLogFormat = ePLCLogFormat_LogTimeStampAndRecvStrJustForDiffStr)
{
    //D:\HT9045_Log\EventLogTxt\YYYY\MM\PLCRecvData_YYYYMMDD.log
    AnsiString asFolderPath = "D:\\HT9045_Log\\EventLogTxt\\" + Now().FormatString("yyyy\\mm\\");
    if(!EnsureFolderExists(asFolderPath))
    {
        RecordProcess("Create folder failed: " + asFolderPath);
        return;
    }
    AnsiString asFileName = asFolderPath + "PLCRecvData_"+ Now().FormatString("yyyymmdd") +".log";
    FILE **ppf = &g_fpPLCRecvData;
    AnsiString &asFileName_Pre = g_asPLCRecvData_FileName_Pre;

    if(NULL==(*ppf))
    {
        *ppf = fopen(asFileName.c_str(), "a");
    }
    else
    {
        if(asFileName_Pre != asFileName)//日期變更
        {
            //先關閉舊檔案
            fclose(*ppf);
            *ppf = NULL;
            //開啟新檔案
            *ppf = fopen(asFileName.c_str(), "a");
        }
    }

    asFileName_Pre = asFileName;

    if(*ppf)
    {
        //2026/04/16_00:00:00.009
        AnsiString asDate = Now().FormatString("yyyy/mm/dd_hh:nn:ss.zzz");
        AnsiString &asLog_Pre = g_asPLCRecvData_Log_Pre;
        bool bSameLog = (asLog_Pre == asLog);

        switch(ePLCLogFormat)
        {
            case ePLCLogFormat_LogTimeStampAndRecvStr:
                fprintf(*ppf, "%s:%s\n", asDate.c_str(), asLog.c_str());
                break;
            case ePLCLogFormat_LogTimeStampIfRecvSameStr:
                if(bSameLog)
                    fprintf(*ppf, "%s\n", asDate.c_str());
                else
                    fprintf(*ppf, "%s:%s\n", asDate.c_str(), asLog.c_str());
                break;
            case ePLCLogFormat_LogTimeStampAndRecvStrJustForDiffStr:
                if(!bSameLog)
                    fprintf(*ppf, "%s:%s\n", asDate.c_str(), asLog.c_str());
                break;
            default:
                break;
        }

        asLog_Pre = asLog;
    }
}
//------------------------------------------------------------------------------
void WriteFile_PLCRecvVal(VTBYTEDATA data, AnsiString asLog_Prefix = "", AnsiString anLog_Postfix = "", EPLCLogFormat ePLCLogFormat = ePLCLogFormat_LogTimeStampAndRecvStrJustForDiffStr)
{
    //write log of variable data which is received from PLC
    AnsiString asLog = asLog_Prefix + "["+IntToStr((int)data.size())+"]";

    for(int i=0; i<(int)data.size(); i++)
    {
        asLog += "_" + IntToStr((int)(data[i]));
    }
    asLog += anLog_Postfix;

    WriteFile_PLCRecvVal_Base(asLog, ePLCLogFormat);
}
//------------------------------------------------------------------------------
void WriteFile_PLCRecvBit_Base(bool *pAry, int iOrd_1, int iOrd_2, int iOrd_3, int iPLCRecvBitType, AnsiString asLog_Prefix = "", EPLCLogFormat ePLCLogFormat = ePLCLogFormat_LogTimeStampAndRecvStrJustForDiffStr)
{
    //D:\HT9045_Log\EventLogTxt\YYYY\MM\PLCRecvData_YYYYMMDD.log
    AnsiString asFolderPath = "D:\\HT9045_Log\\EventLogTxt\\" + Now().FormatString("yyyy\\mm\\");
    if(!EnsureFolderExists(asFolderPath))
    {
        RecordProcess("Create folder failed: " + asFolderPath);
        return;
    }
    AnsiString asFileName = asFolderPath + "PLCRecvData_"+ Now().FormatString("yyyymmdd") +".log";
    FILE **ppf = &g_fpPLCRecvBit;
    AnsiString &asFileName_Pre = g_asPLCRecvBit_FileName_Pre;

    if(NULL==(*ppf))
    {
        *ppf = fopen(asFileName.c_str(), "a");
    }
    else
    {
        if(asFileName_Pre != asFileName)//日期變更
        {
            //先關閉舊檔案
            fclose(*ppf);
            *ppf = NULL;
            //開啟新檔案
            *ppf = fopen(asFileName.c_str(), "a");
        }
    }

    asFileName_Pre = asFileName;

    if(*ppf)
    {
        //2026/04/16_00:00:00.009
        AnsiString asDate = Now().FormatString("yyyy/mm/dd_hh:nn:ss.zzz");
        AnsiString asLog = "";

        if(iPLCRecvBitType == ePLCRecvBit_PLCIn)
            asLog += (asLog_Prefix + "bPLCInData");
        else
            asLog += (asLog_Prefix + "bOSSDData");

        for(int i = 0; i<iOrd_1; i++)
        {
            for(int j = 0; j<iOrd_2; j++)
            {
                for(int k = 0; k<iOrd_3; k++)
                {
                    //前面若有字串了要加入"_"隔開，沒有的話就直接接在後面
                    if(k==0 && asLog!="")
                        asLog += "_";

                    //以iOrd_3為字串長度
                    asLog += (pAry[i*iOrd_2*iOrd_3 + j*iOrd_3 + k] ? "1" : "0");
                }
            }
        }

        bool bSameLog = false;
        if(iPLCRecvBitType == ePLCRecvBit_PLCIn)
        {
            AnsiString &asLog_Pre = g_asPLCRecvBit_Log_PLCIn_Pre;
            bSameLog = (asLog_Pre == asLog);

            asLog_Pre = asLog;
        }
        else
        {
            AnsiString &asLog_Pre = g_asPLCRecvBit_Log_OSSD_Pre;
            bSameLog = (asLog_Pre == asLog);

            asLog_Pre = asLog;
        }

        switch(ePLCLogFormat)
        {
            case ePLCLogFormat_LogTimeStampAndRecvStr:
                fprintf(*ppf, "%s:%s\n", asDate.c_str(), asLog.c_str());
                break;
            case ePLCLogFormat_LogTimeStampIfRecvSameStr:
                if(bSameLog)
                    fprintf(*ppf, "%s\n", asDate.c_str());
                else
                    fprintf(*ppf, "%s:%s\n", asDate.c_str(), asLog.c_str());
                break;
            case ePLCLogFormat_LogTimeStampAndRecvStrJustForDiffStr:
                if(!bSameLog)
                    fprintf(*ppf, "%s:%s\n", asDate.c_str(), asLog.c_str());
                break;
            default:
                break;
        }
    }
}
//------------------------------------------------------------------------------
void WriteFile_PLCRecvBit(AnsiString asLog_Prefix = "", EPLCLogFormat ePLCLogFormat = ePLCLogFormat_LogTimeStampAndRecvStrJustForDiffStr)
{
    WriteFile_PLCRecvBit_Base(&bPLCInData[0][0][0], GET_ARY_SIZE(bPLCInData, 1), GET_ARY_SIZE(bPLCInData, 2), GET_ARY_SIZE(bPLCInData, 3), ePLCRecvBit_PLCIn, asLog_Prefix, ePLCLogFormat);
    WriteFile_PLCRecvBit_Base(&bOSSDData[0][0][0],  GET_ARY_SIZE(bOSSDData, 1),  GET_ARY_SIZE(bOSSDData, 2),  GET_ARY_SIZE(bOSSDData, 3),  ePLCRecvBit_OSSD,  asLog_Prefix, ePLCLogFormat);
}
//------------------------------------------------------------------------------
extern ESafePLCIOType g_eSafePLCIOType;
void PLCIOTaskCycle(EPLCGrp ePLCGrp)
{
    if(g_PlcComm[ePLCGrp].IsConnected()==false)
        return;

    int &Task=g_iPLCIOTask[ePLCGrp];
    static int iSlave=0;

    VTBYTEDATA data;
    data.clear();
    AnsiString  asTemp;

    int iFuncCode = g_tPLCIOInfo.m_iFuncCode;
    int iReadRegOffset = g_tPLCIOInfo.m_iReadRegOffset;

    int iReadByteCount_InpData = g_tPLCIOInfo.m_iReadByteCount_InpData;
    int iReadByteCount_OSSD    = g_tPLCIOInfo.m_iReadByteCount_OSSD;
    int iReadByteCount         = g_tPLCIOInfo.m_iReadByteCount;
    
    switch(Task)
    {
        case 1:
            if(ePLCGrp<0 || ePLCGrp>=ePLCGrp_Count)
            {
                for(int i=0; i<ePLCGrp_Count; i++)
                {
                    if(bScanSlave[i])
                    {
                        ePLCGrp=EPLCGrp(i);
                        break;//需要直接跳出可使用break即可避免誤會
                    }
                }
            }

            if(g_iTransactionID[ePLCGrp]>0xffff)
                g_iTransactionID[ePLCGrp]=0;                                     // 0-0xffff (2byte)

            if(bScanSlave[ePLCGrp])
            {
                //寫入需要讀取的Byte數量
                data.push_back(HIBYTE((iReadByteCount+1)/2));                   //一個Register為1個WORD即2Bytes,包含高低位元
                data.push_back(LOBYTE((iReadByteCount+1)/2));
                //========== ↓Schneider↓ ==========
                //PLC Reg 30001 == 0x00 回應的資料為2Byte資料 30002 30001 但30001為系統狀態 輸入暫存器從30002開始
                //PLC Reg 30003 == 0x01 回應的資料為2Byte資料 30004 30003
                //========== ↑Schneider↑ ==========
                g_PlcComm[ePLCGrp].SendData(g_iTransactionID[ePLCGrp], ePLCGrp, iFuncCode, iReadRegOffset, data);
                hCheckTime.SetSecAndOn(1);
                Task=100;
            }
            else
            {
                iSlave++;
            }
            break;
        case 100:
            if(g_PlcComm[ePLCGrp].GetRecevie(g_iTransactionID[ePLCGrp],data))   //讀取成功
            {                                                                   //寫入Byte陣列
                bool bPLCRecvDataSizeOK = (iReadByteCount==(int)data.size()?true:false);

                if(g_bEnRecPLCRecvData)
                {
                    if(bPLCRecvDataSizeOK)
                        WriteFile_PLCRecvVal(data);
                    else
                        WriteFile_PLCRecvVal(data, "[SIZE_ERROR]_");
                }

                if(!bPLCRecvDataSizeOK)
                {
                    //Discard this packet without updating safety door state
                    g_iTransactionID[ePLCGrp]++;
                    Task=1;
                    AnsiString asLog;
                    asLog = "PLC recv data size error, so re-initialize task number.";
                    asLog += "PLC index:[" + IntToStr(ePLCGrp) + "],";
                    asLog += "Recv size expected:[" + IntToStr(iReadByteCount) + "], actual:[" + IntToStr((int)data.size()) + "],";
                    asLog += "Task number is changed from [" + IntToStr(Task) + "] to [1] ]";
                    RecordProcess(asLog);
                    break;  //Next cycle will re-initialize timer in case 1
                }

                for(int i=0; i<(int)data.size(); i++)
                {
                    if(!bPLCRecvDataSizeOK)                                     //if data size is wrong, then not update
                        break;
                        
                    if(i<iReadByteCount)
                    {                                                           //資料順序整理 因PLC暫存器是先放高位資料 所以 0->1  1->0  2->3  3->2
                        InPortData_Byte[ePLCGrp][(i/2)*2+abs(i%2-1)]=data[i];
                    }
                }

                for(int i=0; i<ePLCGrp_Count; i++)                              //寫入boolean陣列
                {
                    if(!bPLCRecvDataSizeOK)                                     //if data size is wrong, then not update
                        break;
                    if(i!=ePLCGrp)                                              //just handle the current receive data
                        continue;

                    for(int j=0; j<INPUT_MAX_REGISTER; j++)
                    {
                        for(int k=0; k<8; k++)
                        {
                            if(j>=iReadByteCount_InpData && j<(iReadByteCount_InpData+iReadByteCount_OSSD))//OSSD 安全輸出 30021 30022
                            {
                                bOSSDData[i][j-iReadByteCount_InpData][k]=InPortData_Byte[i][j]&MyBitMask[k];//前面為Input data，後面為OSSD
                            }
                            else if(0<=j && j<iReadByteCount_InpData)
                            {
                                bPLCInData[i][j][k]=InPortData_Byte[i][j]&MyBitMask[k];
                            }
                        }
                    }
                }

                if(g_bEnRecPLCRecvData)
                {
                    if(bPLCRecvDataSizeOK)
                        WriteFile_PLCRecvBit();
                    else
                        WriteFile_PLCRecvBit("[SIZE_ERROR]_");
                }

                bIOPowered=bOSSDData[0][0][0];

                //if(iPLCSafetyVer==202204)                                     //joe 20220406.01 debug safe plc
                    bIOPowered=true;

                bPLCIOEffect=true;                                              //完成過一次
                iSlave++;
                g_iTransactionID[ePLCGrp]++;
                Task=1;
            }
            else
            {
                if(hCheckTime.Off())
                {
                    iSlave++;
                    g_iTransactionID[ePLCGrp]++;
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
    WORD wInputStartRegNum = g_tPLCIOInfo.m_wInputStartRegNum;
    WORD wInputEndRegNum = g_tPLCIOInfo.m_wInputEndRegNum;
    
    if(iSlave>=0 && iSlave<ePLCGrp_Count &&
       iPort>=wInputStartRegNum &&
       #if 0
       iPort<(wInputStartRegNum+INPUT_MAX_REGISTER*2)   &&
       #else
       iPort<=wInputEndRegNum &&
       #endif
       iBit>=0      && iBit<8)
    {
        if(iPLCSafetyVer==202204)
            return bPLCInData[iSlave][iPort-wInputStartRegNum+2][iBit];//joe 20220406.01 debug safe plc
        else
            return bPLCInData[iSlave][iPort-wInputStartRegNum+1][iBit];
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
    for(int i = 0; i<ePLCGrp_Count; ++i)
    {
        PLCIOTaskCycle(EPLCGrp(i));
        PLCStatusCheck(EPLCGrp(i));                                             //ben 20230913 add
        g_PlcComm[i].Cycle();
    }
}
//------------------------------------------------------------------------------
void GetPLCIOInfo_Base(ESafePLCIOType eSafePLCIOType, TPLCIOInfo &tPLCIOInfo)
{
    switch(eSafePLCIOType)
    {
        case eSafePLCIOType_InstallSchneider:
        {
            tPLCIOInfo.m_wInputStartRegNum = PLCIO_INPUT_START_PORT_NUMBER_SCHNEIDER;
            tPLCIOInfo.m_wInputEndRegNum = PLCIO_INPUT_START_PORT_NUMBER_SCHNEIDER_UB;
            tPLCIOInfo.m_iFuncCode = MODBUS_FC_READ_INPUT_REGISTERS;

            //========== ↓Schneider↓ ==========
            //PLC Reg 30001 == 0x00 回應的資料為2Byte資料 30002 30001 但30001為系統狀態 輸入暫存器從30002開始
            //PLC Reg 30003 == 0x01 回應的資料為2Byte資料 30004 30003
            //========== ↑Schneider↑ ==========
            tPLCIOInfo.m_iReadRegOffset = 0x00;
            tPLCIOInfo.m_iReadByteCount_InpData = READ_BYTE_COUNT_INP_DATA_SCHNEIDER;
            tPLCIOInfo.m_iReadByteCount_OSSD = READ_BYTE_COUNT_OSSD_SCHNEIDER;
            break;
        }
        case eSafePLCIOType_InstallReeR:
        {
            tPLCIOInfo.m_wInputStartRegNum = PLCIO_INPUT_START_PORT_NUMBER_REER;
            tPLCIOInfo.m_wInputEndRegNum = PLCIO_INPUT_START_PORT_NUMBER_REER_UB;
            tPLCIOInfo.m_iFuncCode = MODBUS_FC_READ_HOLDING_REGISTERS;
            tPLCIOInfo.m_iReadRegOffset = 0x400;//若該設備將基址設在 40001，則 0x40F 對應的就是 41040
            tPLCIOInfo.m_iReadByteCount_InpData = READ_BYTE_COUNT_INP_DATA_REER;
            tPLCIOInfo.m_iReadByteCount_OSSD = READ_BYTE_COUNT_OSSD_REER;

            //OSSD1:OSSD Byte0=>0x40F High byte
            //OSSD2:OSSD Byte1=>0x40F Low byte
            //OSSD3:OSSD Byte2=>0x410 High byte
            //OSSD4:OSSD Byte3=>0x410 Low byte

            // 暫存器地址 (Hex) | 暫存器內容 (16-bit) | 高位位元組 (High Byte) | 低位位元組 (Low Byte)
            //   0x040F (1039) | OSSD 1 & 2         | OSSD 1 (Byte 0)       | OSSD 2 (Byte 1)
            //   0x0410 (1040) | OSSD 3 & 4         | OSSD 3 (Byte 2)       | OSSD 4 (Byte 3)            
            break;
        }
    }
    
    tPLCIOInfo.m_iReadByteCount = tPLCIOInfo.m_iReadByteCount_InpData + tPLCIOInfo.m_iReadByteCount_OSSD;//包含OSSD的Byte數量
}
//------------------------------------------------------------------------------
void GetPLCIOInfo(ESafePLCIOType eSafePLCIOType)
{
    GetPLCIOInfo_Base(eSafePLCIOType, g_tPLCIOInfo);
}
//------------------------------------------------------------------------------
