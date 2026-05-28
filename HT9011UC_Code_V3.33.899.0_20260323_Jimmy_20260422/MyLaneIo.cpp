#include "MachineDefine.h"
#pragma hdrstop

#include "MyLaneIo.h"

#include "MachineType.h"
#include "cmydef.h"

#include "iosetview.h"
#include "mymessbox.h"
#include "cpublic.h"
#include "myMN200motor.h"
#include "AdvMotApi.h"                                                          //Sam 20230724 : 新增 PCI1203 IO 模組

#include "MyPLC_IO_Modbus.h"
#include "ModbusTCPClient.h"
//------------------------------------------------------------------------------
#pragma package(smart_init)
class TLaneIO MyLaneIO;
//------------------------------------------------------------------------------
__fastcall TLaneIO::TLaneIO()                                                   //Jimmychiu 20230313 fixed _fastcall to __fastcall
{
    OutPos=0;
    InPos=0;
    InitialMyOutIOData();
    InitialMyUseIP();                                                           //JerryYang 20241231 : 修改Restore IO模組判斷方式, 有輸入database的才需要恢復
}
//------------------------------------------------------------------------------
void TLaneIO::InitialMyOutIOData()
{
    for(int i=0; i<MAXRing; i++)
    {
        for(int j=0; j<MAXIP; j++)
        {
            for(int k=0; k<MAXPort; k++)
            {
                OutPortIndex[i][j][k]=0;
                OutPortData[i][j][k]=0;
            }
        }
    }
}
//------------------------------------------------------------------------------
void TLaneIO::InitialMyUseIP()
{
    for(int i=0; i<MAXRing; i++)
    {
        for(int j=0; j<MAXIP; j++)
        {
            iUseMNetIP[i][j]=DEV_INF_NO_DEV;                                    //JerryYang 20241231 : 修改Restore IO模組判斷方式, 有輸入database的才需要恢復
        }
    }
}
//------------------------------------------------------------------------------
void TLaneIO::SetUseIP(int Ring, int IP, int iISABase, AnsiString Alias, bool bOutPut)
{
   /* if(Ring<MAXRing && IP<MAXIP)
    {
        if(iISABase==eMotionNet)                                                //JerryYang 20241231 : 修改Restore IO模組判斷方式, 有輸入database的才需要恢復
        {
            if(bOutPut==true)
            {
                if(iUseMNetIP[Ring][IP]==DEV_INF_IO_32IN_DEV)
                {
                    iUseMNetIP[Ring][IP]=DEV_INF_IO_16IN_16OUT_DEV;
                    MNetLog(AnsiString().sprintf("Ring=%d IP=%d OutPut=%d Alias=%s", Ring, IP, (bOutPut)?1:0, Alias));
                }
                else if(iUseMNetIP[Ring][IP]!=DEV_INF_IO_32OUT_DEV &&
                        iUseMNetIP[Ring][IP]!=DEV_INF_IO_16IN_16OUT_DEV)
                {
                    iUseMNetIP[Ring][IP]=DEV_INF_IO_32OUT_DEV;
                    MNetLog(AnsiString().sprintf("Ring=%d IP=%d OutPut=%d Alias=%s", Ring, IP, (bOutPut)?1:0, Alias));
                }
                else
                {
                    iUseMNetIP[Ring][IP]=DEV_INF_IO_32OUT_DEV;
                    MNetLog(AnsiString().sprintf("Ring=%d IP=%d OutPut=%d Alias=%s", Ring, IP, (bOutPut)?1:0, Alias));
                }
            }
            else if(bOutPut==false)
            {
                if(iUseMNetIP[Ring][IP]==DEV_INF_IO_32OUT_DEV)
                {
                    iUseMNetIP[Ring][IP]=DEV_INF_IO_16IN_16OUT_DEV;
                    MNetLog(AnsiString().sprintf("Ring=%d IP=%d OutPut=%d Alias=%s", Ring, IP, (bOutPut)?1:0, Alias));
                }
                else if(iUseMNetIP[Ring][IP]!=DEV_INF_IO_32IN_DEV &&
                        iUseMNetIP[Ring][IP]!=DEV_INF_IO_16IN_16OUT_DEV)
                {
                    iUseMNetIP[Ring][IP]=DEV_INF_IO_32IN_DEV;
                    MNetLog(AnsiString().sprintf("Ring=%d IP=%d OutPut=%d Alias=%s", Ring, IP, (bOutPut)?1:0, Alias));
                }
                else
                {
                    iUseMNetIP[Ring][IP]=DEV_INF_IO_32IN_DEV;
                    MNetLog(AnsiString().sprintf("Ring=%d IP=%d OutPut=%d Alias=%s", Ring, IP, (bOutPut)?1:0, Alias));
                }
            }
        }
    }     */
}
//------------------------------------------------------------------------------
extern bool IdleCheckSafeDoorByCylinder(int Ring, int IP, int Port, int Bit);   //Steven 20230703 : Add for MotionNet IO check SafeDoor
//------------------------------------------------------------------------------
void TLaneIO::IOBitOn(int Ring, int IP, int Port, int Bit, int iISABase, AnsiString Alias)  //Sam 20230724 : 新增 PCI1203 IO 模組
{
    //******************************************************************************
    //  注意!! IdleCheckSafeDoorByCylinder為Handler 安全門相關, 修改時要小心!!
    //******************************************************************************
    if(IdleCheckSafeDoorByCylinder(Ring, IP, Port, Bit)==true)                  //Steven 20230703 : Add for MotionNet IO check SafeDoor
        return;

    int ret;
    AnsiString str;

    if(Ring<=0 && IP<=0 && Port<=0 && Bit<=0)
        return;

    int iHasErr=CheckPortRangeErr(true, iISABase, Ring, IP, Port, Bit);

    if(iHasErr!=0)
    {
        #ifndef SOFT_SIMULTE
        str.sprintf("IOBitOn (%s) Ring %d | IP %d | Port %d | Bit %d Fail. %s", Alias, Ring, IP, Port, Bit, GetIOErrStr(iHasErr));
        MNetLog(str);
        if(MachineTypeChoice==Type_HT9045)                                      //9045
        {
            ShowMyMessage(str, "HT9045 : Maybe input wrong IO position!");
        }
        else if(MachineTypeChoice==Type_HT9045_12Site)                          //ChungHung 20130507 add HT9045 updata for 12site 517
        {
            ShowMyMessage(str, "HT9045_12Site : Maybe input wrong IO position!");
        }
        else
        {
            ShowMyMessage(str, "HT9046 : Maybe input wrong IO position!");
        }
        #endif
        return;
    }

    OutPortData[Ring][IP][Port]|=MyBitMask[Bit];
    if(iISABase==ePCI1203)                                                      //Sam 20230724 : 新增 PCI1203 IO 模組
    {
        ret=Acm_DaqDoSetBitEx(uiDevhand, Ring, IP, Port, 1);
        if(ret==0)
            ret=1;
        else
            ret=-1;
    }
    else if(IO_CARD_TYPE==MotionnetIO_MN200 || IO_CARD_TYPE==NewIO_MN200)       //Steven 20120906 : 新增MN200
    {
        ret=mn_set_port_bit(Ring, IP, Port , Bit, 1);                           //2013-01-15 Dell PISO new version
    }
    else
    {
        ret=_mnet_bit_io_output(Ring, IP, Port, Bit, 1);                        //Steven 20110303
    }

    if(ret<0)
    {
        #ifdef SOFT_SIMULTE
            return;
        #else
            str.sprintf("IOBitOn (%s) Ring %d, IP %d, Port %d, Bit %d Fail, Code: %d", Alias, Ring, IP, Port, Bit, ret);
            MNetLog(str);                                                       //Steven 20110406
            return ;
        #endif
    }
}
//------------------------------------------------------------------------------
void TLaneIO::IOBitOff(int Ring, int IP, int Port, int Bit, int iISABase, AnsiString Alias) //Sam 20230724 : 新增 PCI1203 IO 模組
{
    //******************************************************************************
    //  注意!! IdleCheckSafeDoorByCylinder為Handler 安全門相關, 修改時要小心!!
    //******************************************************************************
    if(IdleCheckSafeDoorByCylinder(Ring, IP, Port, Bit)==true)                  //Steven 20230703 : Add for MotionNet IO check SafeDoor
        return;

    int ret;
    AnsiString str;

    if(Ring<=0 && IP<=0 && Port<=0 && Bit<=0)
        return;

    int iHasErr=CheckPortRangeErr(true, iISABase, Ring, IP, Port, Bit);

    if(iHasErr!=0)
    {
        #ifndef SOFT_SIMULTE
        str.sprintf("IOBitOff (%s) Ring %d | IP %d | Port %d | Bit %d Fail. %s", Alias, Ring, IP, Port, Bit, GetIOErrStr(iHasErr));
        MNetLog(str);
        if(MachineTypeChoice==Type_HT9045)                                      //9045
        {
            ShowMyMessage(str, "HT9045 : Maybe input wrong IO position!");
        }
        else if(MachineTypeChoice==Type_HT9045_12Site)                          //ChungHung 20130507 add HT9045 updata for 12site 517
        {
            ShowMyMessage(str, "HT9045_12Site : Maybe input wrong IO position!");
        }
        else
        {
            ShowMyMessage(str, "HT9046 : Maybe input wrong IO position!");
        }
        #endif
        return;
    }

    OutPortData[Ring][IP][Port]&=~MyBitMask[Bit];
    if(iISABase==ePCI1203)                                                      //Sam 20230724 : 新增 PCI1203 IO 模組
    {
        ret=Acm_DaqDoSetBitEx(uiDevhand, Ring, IP, Port, 0);
        if(ret==0)
            ret=1;
        else
            ret=-1;
    }
    else if(IO_CARD_TYPE==MotionnetIO_MN200 || IO_CARD_TYPE==NewIO_MN200)       //Steven 20120906 : 新增MN200
    {
        ret=mn_set_port_bit(Ring, IP, Port , Bit, 0);                           //2013-01-15 Dell PISO new version
    }
    else
    {
        ret=_mnet_bit_io_output(Ring, IP, Port, Bit, 0);                        //Steven 20110303
    }

    if(ret<0)
    {
        #ifdef SOFT_SIMULTE
            return;
        #else
            str.sprintf("IOBitOff Ring %d, IP %d, Port %d, Bit %d Fail, Code: %d)", Ring, IP, Port, Bit, ret);
            MNetLog(str);                                                       //Steven 20110406
            return;
        #endif
    }
}
//------------------------------------------------------------------------------
bool TLaneIO::IOByteOut(int Ring, int IP, int Port, unsigned int Byte, int iISABase)    //Sam 20230724 : 新增 PCI1203 IO 模組
{
    //******************************************************************************
    //  注意!! IdleCheckSafeDoorByCylinder為Handler 安全門相關, 修改時要小心!!
    //******************************************************************************
    if(IdleCheckSafeDoorByCylinder(Ring, IP, Port, -1)==true)                   //Steven 20230703 : Add for MotionNet IO check SafeDoor
        return false;

    int ret;
    AnsiString str;

    if(Ring<=0 && IP<=0 && Port<=0)
        return false;

    int iHasErr=CheckPortRangeErr(true, iISABase, Ring, IP, Port);

    if(iHasErr!=0)
    {
        #ifndef SOFT_SIMULTE
        str.sprintf("IOByteOut Ring %d | IP %d | Port %d | Byte %d Fail, %s", Ring, IP, Port, Byte, GetIOErrStr(iHasErr));
        MNetLog(str);
        if(MachineTypeChoice==Type_HT9045)                                      //9045
        {
            ShowMyMessage(str, "HT9045 : Maybe input wrong IO position!");
        }
        else if(MachineTypeChoice==Type_HT9045_12Site)                          //ChungHung 20130507 add HT9045 updata for 12site 517
        {
            ShowMyMessage(str, "HT9045_12Site : Maybe input wrong IO position!");
        }
        else
        {
            ShowMyMessage(str, "HT9046 : Maybe input wrong IO position!");
        }
        #endif
        return false;
    }

    OutPortData[Ring][IP][Port]=Byte;

    if(iISABase==ePCI1203)                                                      //Sam 20230724 : 新增 PCI1203 IO 模組
    {
        ret=Acm_DaqDoSetByteEx(uiDevhand, Ring, IP, Port, OutPortData[Ring][IP][Port]);     //感覺怪怪的
        if(ret==0)
            ret=1;
        else
            ret=-1;
    }
    else if(IO_CARD_TYPE==MotionnetIO_MN200 || IO_CARD_TYPE==NewIO_MN200)       //Steven 20120906 : 新增MN200
    {
        ret=mn_set_port_byte(Ring, IP, Port, OutPortData[Ring][IP][Port]);
    }
    else
    {
        ret=_mnet_io_output(Ring, IP, Port, OutPortData[Ring][IP][Port]);
    }

    if(ret<0)
    {
        #ifdef SOFT_SIMULTE
            return true;
        #else
            str.sprintf("IOByteOut Ring %d, IP %d, Port %d, Byte %d Fail, Code: %d", Ring, IP, Port, Byte, ret);
            MNetLog(str);                                                       //Steven 20110406
            return true;
        #endif
    }
    return true;                                                                //Sam 20250325 : IO 恢復狀態時，安全門一直報警問題
}
//------------------------------------------------------------------------------
bool TLaneIO::IOOutBitStatus(int Ring, int IP, int Port, int Bit, int iISABase, AnsiString Alias)   //Sam 20230724 : 新增 PCI1203 IO 模組
{
    AnsiString str;
    if(iISABase==ePCI1203)                                                      //Sam 20230724 : 新增 PCI1203 IO 模組
    {
        if(Ring==0)
            return false;
    }
    else
    {
        if(Ring<=0 && IP<=0 && Port<=0 && Bit<=0)
            return false;
    }

    int iHasErr=CheckPortRangeErr(true, iISABase, Ring, IP, Port, Bit);

    if(iHasErr!=0)
    {
        #ifndef SOFT_SIMULTE
        //AI(ht9045-v899) 20260424: log 維持簡短供解析, UI 改用 BuildIOErrMessage 提供完整修法
        str.sprintf("IOOutBitStatus (%s) Ring %d | IP %d | Port %d | Bit %d Fail. iErr=%d", Alias, Ring, IP, Port, Bit, iHasErr);
        MNetLog(str);
        ShowMyMessage(BuildIOErrMessage(true, "IOOutBitStatus", Alias, Ring, IP, Port, Bit, iHasErr));
        #endif
        return false;
    }

    if(OutPortData[Ring][IP][Port] & MyBitMask[Bit])
        return true;
    else
        return false;
}
//------------------------------------------------------------------------------
//InPut function
//------------------------------------------------------------------------------
bool TLaneIO::IOInputBit(int Ring, int IP, int Port, int Bit, int iISABase, AnsiString Alias)   //Sam 20230724 : 新增 PCI1203 IO 模組
{
    int ret=0;                                                                  //Steven 20110303
    AnsiString str;
    U8 Value;

    if(Ring<=0 && IP<=0 && Port<=0 && Bit<=0)
        return false;

    int iHasErr=CheckPortRangeErr(false, iISABase, Ring, IP, Port, Bit);

    if(iHasErr!=0)
    {
        if(fiosetview->fShow==false)
        {
            #ifndef SOFT_SIMULTE
            //AI(ht9045-v899) 20260424: log 維持簡短供解析, UI 改用 BuildIOErrMessage 提供完整修法
            str.sprintf("IOInputBit (%s) Ring %d, IP %d, Port %d, Bit %d Fail. iErr=%d", Alias, Ring, IP, Port, Bit, iHasErr);
            MNetLog(str);
            ShowMyMessage(BuildIOErrMessage(false, "IOInputBit", Alias, Ring, IP, Port, Bit, iHasErr));
            #endif
        }
        return false;
    }

    if(bPLCIO[Port][Bit] && iISABase==ePLCbase)                                 //Jason 20230619 增加安全PLC部分
    {                                                                           //Austin 20190218.01 增加PLC IO for CE
        if(Port>=PLCIO_INPUT_START_PORT_NUMBER && Port<=0x303)
        {
            if(iPLCSafetyVer==202204)
            {
                return bPLCInData[0][Port-PLCIO_INPUT_START_PORT_NUMBER+2][Bit];//joe 20220406.01 debug safe plc
            }
            else
            {
                return bPLCInData[0][Port-PLCIO_INPUT_START_PORT_NUMBER+1][Bit];//第1個Byte為系統狀態 預留
            }
        }
        else
        {
            return true;
        }
    }
    else
    {
        if(iISABase==ePCI1203)                                                  //Sam 20230724 : 新增 PCI1203 IO 模組
        {
            ret=Acm_DaqDiGetBitEx(uiDevhand, Ring, IP, Port, &Value);
            if(ret==0)
                ret=1;
            else
                ret=-1;
        }
        else if(IO_CARD_TYPE==MotionnetIO_MN200 || IO_CARD_TYPE==NewIO_MN200)   //Steven 20120906 : 新增MN200
        {
            ret=mn_get_port_bit(Ring, IP, Port, Bit, &Value);
        }
        else
        {
            ret=_mnet_bit_io_input(Ring, IP, Port, Bit, &Value);                //Steven 20110303 Start : 改用_mnet_bit_io_input
        }
    }

    if(ret>=0)
    {
        if(Value>0)
            return true;
        else
            return false;
    }
    else
    {
        #ifdef SOFT_SIMULTE
            return true;
        #else
            str.sprintf("IOInputBit (%s) Ring %d, IP %d, Port %d, Bit %d Fail, Code: %d", Alias, Ring, IP, Port, Bit, ret);
            MNetLog(str);                                                       //Steven 20110406
            return false;
        #endif
    }
}
//------------------------------------------------------------------------------
byte TLaneIO::IOInputByte(int Ring, int IP, int Port, int iISABase)             //Sam 20230724 : 新增 PCI1203 IO 模組
{
    byte Value=0;
    AnsiString str;
    int ret=0;

    if(Ring<=0 && IP<=0 && Port<=0)
        return false;

    int iHasErr=CheckPortRangeErr(false, iISABase, Ring, IP, Port);

    if(iHasErr!=0)
    {
        #ifndef SOFT_SIMULTE
        //AI(ht9045-v899) 20260424: log 維持簡短供解析, UI 改用 BuildIOErrMessage 提供完整修法 (Byte 操作無 Bit, 傳 -1)
        str.sprintf("IOInputByte Ring %d | IP %d | Port %d Fail. iErr=%d", Ring, IP, Port, iHasErr);
        MNetLog(str);
        ShowMyMessage(BuildIOErrMessage(false, "IOInputByte", AnsiString("(整 Byte)"), Ring, IP, Port, -1, iHasErr));
        #endif
        return 0;
    }

    if(iISABase==ePCI1203)                                                      //Sam 20230724 : 新增 PCI1203 IO 模組
    {
        ret=Acm_DaqDiGetByteEx(uiDevhand, Ring, IP, Port, &Value);
        if(ret==0)
            ret=1;
        else
            ret=-1;
    }
    else if(IO_CARD_TYPE==1)                                                    //Steven 20120906 : 新增MN200
    {
        ret=mn_get_di_byte(Ring, IP, Port, &Value);
    }
    else
    {
        ret=_mnet_io_input(Ring, IP, Port);                                     //Steven 20110303 Start : 加入Ret檢查
    }

    if(ret<0)
    {
        #ifdef SOFT_SIMULTE
            return 1;
        #else
            str.sprintf("IOInputByte Ring %d, IP %d, Port %d Fail, Code: %d", Ring, IP, Port, ret);
            MNetLog(str);                                                       //Steven 20110406
            return 0;
        #endif
    }
    else
    {
        if(IO_CARD_TYPE==0)                                                     //Steven 20120906 : 新增MN200    //Steven 20130828 : 1 --> 0
            Value=byte(ret);
    }
    return Value;
}
//------------------------------------------------------------------------------
void TLaneIO::BackUpOutputData()
{
    for(int i=0; i<MAXRing; i++)
    {
        for(int j=0; j<MAXIP; j++)
        {
            for(int k=0; k<MAXPort; k++)
            {
                BackOutPortData[i][j][k]=OutPortData[i][j][k];
            }
        }
    }
}
//------------------------------------------------------------------------------
void TLaneIO::RestoreOutputData()
{
    bool bflag=false, bDoorAlarm=false;
    static int iCheck=0;

    for(int i=0; i<2; i++)                                                      //kevin 20150605 i=1->0
    {
        for(int j=0; j<MAXIP; j++)
        {
            for(int k=0; k<MAXPort; k++)
            {
                if(iUseMNetIP[i][j]==DEV_INF_IO_32OUT_DEV ||                    //JerryYang 20241231 : 修改Restore IO模組判斷方式, 有輸入database的才需要恢復
                   iUseMNetIP[i][j]==DEV_INF_IO_16IN_16OUT_DEV)                 //JerryYang 20250108 : 修改Restore IO模組判斷方式, 有輸入database的才需要恢復
                {
                    if(iUseMNetIP[i][j]==DEV_INF_IO_16IN_16OUT_DEV)
                    {
                        if(k==2 || k==3)
                        {
                            if(OutPortData[i][j][k]!=BackOutPortData[i][j][k])  //Sam 20250325 : IO 恢復狀態時，安全門一直報警問題
                            {
                                bflag=IOByteOut(i, j, k, BackOutPortData[i][j][k]);
                                if(bflag)
                                    OutPortData[i][j][k]=BackOutPortData[i][j][k];
                                else
                                    bDoorAlarm=true;
                            }
                        }
                    }
                    else
                    {
                        if(OutPortData[i][j][k]!=BackOutPortData[i][j][k])
                        {
                            bflag=IOByteOut(i, j, k, BackOutPortData[i][j][k]);
                            if(bflag)
                                OutPortData[i][j][k]=BackOutPortData[i][j][k];
                            else
                                bDoorAlarm=true;
                        }
                    }
                }
            }
        }
    }

    if(bDoorAlarm)                                                              //Sam 20250325 : IO 恢復狀態時，安全門一直報警問題
    {
        iCheck++;
        if(iCheck<10)                                                           //避免無限迴圈，安全門提示10次後就不提示了。
            RestoreOutputData();
        else
            iCheck=0;
    }
    else
    {
        iCheck=0;
    }
}
//------------------------------------------------------------------------------
//2013-01-15    Dell 檢查DI/DO範圍
int TLaneIO::CheckPortRangeErr(bool DO_Type, int iISABase, int Ring, int IP, int Port, int Bit)   //沒輸入bit就不檢查
{
    if(InitialOK==false)                                                        //等待卡片讀取
        return 0;

    if(iISABase!=eMotionNet)
        return 0;

    #ifdef SOFT_SIMULTE
    #else
    if(Bit!=999 && (Bit<0 || Bit>7))
    {
        return 1;
    }

    if(Ring<0 || Ring>=MAXRing ||
       IP<0   || IP>=MAXIP ||
       Port<0 || Port>=MAXPort)
    {
        return 2;
    }

    if(iUseMNetIP[Ring][IP]!=DEV_INF_IO_32OUT_DEV &&
       iUseMNetIP[Ring][IP]!=DEV_INF_IO_32IN_DEV &&
       iUseMNetIP[Ring][IP]!=DEV_INF_IO_16IN_16OUT_DEV)                         //JerryYang 20241231 : 修改Restore IO模組判斷方式, 有輸入database的才需要恢復
    {
        return 3;
    }

    if(iUseMNetIP[Ring][IP]==DEV_INF_IO_16IN_16OUT_DEV)
    {
        if(DO_Type==true && !(Port==2 || Port==3))                              //JerryYang 20250108 : 修改Restore IO模組判斷方式, 有輸入database的才需要恢復
        {
            return 4;
        }

        if(DO_Type==false && !(Port==0 || Port==1))
        {
            return 5;
        }
    }
    #endif
    return 0;
}
//------------------------------------------------------------------------------
//void Dectect24VSwitchOn()                                                     //Steven 20111103 : 24V斷電偵測Switch
//{
//    _mnet_bit_io_output(1, 8, 2, 5, 1);                                       //Steven 20111103 : 24V斷電偵測
//}
//---------------------------------------------------------------------------
double TLaneIO::GetIOValue(int Ring, int IP, int Port, int Bit, int iISABase, AnsiString Alias)     //Sam 20230210 : 新增 VacuumUnit 通訊模組
{
    double dKpa=0.0;
    if(iISABase==ePCI1203)
    {
        unsigned long Result1,Result2;
        unsigned char HighValue;
        unsigned char LowValue;
        int iValue;
        AnsiString strLow="",strHigh="",strLowHigh="";

        Result1=Acm_DaqDiGetByteEx(uiDevhand, Ring, IP, Port*2,     &LowValue); //ECAT-VC8 AI0 Channel0 00-01 ; Channel7 15-16
        Result2=Acm_DaqDiGetByteEx(uiDevhand, Ring, IP, Port*2+1,   &HighValue);

        if(Result1==SUCCESS && Result2==SUCCESS)
        {
            strLow=IntToHex(LowValue, 2);
            strHigh=IntToHex(HighValue, 2);
            AnsiString strLowHigh=strHigh+strLow;
            strLowHigh=HexStrToInt(strLowHigh);
            iValue=atoi(strLowHigh.c_str());
            dKpa=VC8ToKpa(iValue);
        }
        else
        {
            dKpa=999.0;
        }
    }
    else
    {
        dKpa=999.0;
    }
    return dKpa;
}
//------------------------------------------------------------------------------
double TLaneIO::GetIOValueThread(int Ring, int IP, int Port, int Bit, int iISABase, AnsiString Alias)
{
    double dKpa=0;
    if(iISABase==ePCI1203)
    {
        unsigned short Threshold_Index=0x8000;
        unsigned short Threshold_SubIndex=0x13;
        unsigned short DataSize=128;
        unsigned long Result;
        short iValue;
        Result=Acm_DevReadSDOData(uiDevhand, Ring, IP, Threshold_Index+Port*0x0010, Threshold_SubIndex,  ECAT_TYPE_I16, DataSize, &iValue);
        if(Result==SUCCESS)
            dKpa=VC8ToKpa(iValue);
        else
            dKpa=999.0;
        return dKpa;
    }
    else
    {
        dKpa=999.0;
    }
    return dKpa;
}
//------------------------------------------------------------------------------
bool TLaneIO::SetIOValueThread(double dKpa, int Ring, int IP, int Port, int Bit, int iISABase, AnsiString Alias)
{
    unsigned long Result;
    short iValue;
    if(iISABase==ePCI1203)
    {
        unsigned short Threshold_Index=0x8000;
        unsigned short Threshold_SubIndex=0x13;
        unsigned short DataSize=128;
        iValue=KpaToVC8(dKpa);
        Result=Acm_DevWriteSDOData(uiDevhand, Ring, IP, Threshold_Index+Port*0x0010, Threshold_SubIndex,  ECAT_TYPE_I16, DataSize, &iValue);
        if(Result==SUCCESS)
            return true;
        else
            return false;
    }
    else
    {
        return false;
    }
}
//---------------------------------------------------------------------------
AnsiString TLaneIO::GetIOErrStr(int iErr)
{
    AnsiString sret="";
    switch(iErr)
    {
        case 0:
            sret="SUCCESS";
            break;
        case 1:
            //AI(ht9045-v899) 20260424: 加上中文說明與修法, 讓售服直接看得懂
            sret="[iErr=1] Bit 範圍錯誤 (應為 0~7)\r\n"
                 "         → 檢查 IO_Table.csv 該列的 Bit 欄位";
            break;
        case 2:
            sret="[iErr=2] Ring/IP/Port 超出系統上限\r\n"
                 "         → 檢查 IO_Table.csv 該列的 Lane/IP/Port 欄位是否打錯";
            break;
        case 3:
            sret="[iErr=3] UseMNetIP Err - 該 Ring/IP 未掃到 IO 模組\r\n"
                 "         可能原因:\r\n"
                 "           1. IP 站沒插 IO 模組或未通電\r\n"
                 "           2. IP 撥碼開關設錯\r\n"
                 "           3. 該 IP 實際是馬達卡, 不能配 IO\r\n"
                 "         → 開啟 D:\\HT9045\\Error\\MNetLog*.txt 確認該 IP 模組型號";
            break;
        case 4:
            sret="[iErr=4] 16IN16OUT 模組的輸出 Port 必須是 2 或 3\r\n"
                 "         → 修改 IO_Table.csv 該列的 Port 欄位";
            break;
        case 5:
            sret="[iErr=5] 16IN16OUT 模組的輸入 Port 必須是 0 或 1\r\n"
                 "         → 修改 IO_Table.csv 該列的 Port 欄位";
            break;
        default:
            sret.sprintf("[iErr=%d] 未知錯誤", iErr);
            break;
    }
    return sret;
}
//---------------------------------------------------------------------------
//AI(ht9045-v899) 20260424: 將 mn200.h 內 DEV_INF_xxx enum 翻成售服看得懂的字串
AnsiString TLaneIO::GetUseMNetIPDevTypeName(int iDevType)
{
    AnsiString sret="";
    switch(iDevType)
    {
        case DEV_INF_NO_DEV:
            sret="未掛模組 (NO_DEV)";
            break;
        case DEV_INF_MOTION_DEV:
            sret="馬達卡 (MOTION_DEV) - 不可放 IO 點位";
            break;
        case DEV_INF_IO_32OUT_DEV:
            sret="32 點純輸出 (IO_32OUT) - Port 限 0~1";
            break;
        case DEV_INF_IO_32IN_DEV:
            sret="32 點純輸入 (IO_32IN) - Port 限 0~1";
            break;
        case DEV_INF_IO_16IN_16OUT_DEV:
            sret="16 入 16 出 (IO_16IN_16OUT) - 輸入 Port 0~1, 輸出 Port 2~3";
            break;
        case DEV_INF_AI_8IN_DEV:
            sret="8 點類比輸入 (AI_8IN) - 不可放數位 IO";
            break;
        default:
            sret.sprintf("未知裝置 (0x%02X)", iDevType);
            break;
    }
    return sret;
}
//---------------------------------------------------------------------------
//AI(ht9045-v899) 20260424: 統一 IO 錯誤訊息格式, 給售服 5 段資訊一次到位
AnsiString TLaneIO::BuildIOErrMessage(bool bDOType, AnsiString sFuncName,
                                      AnsiString sAlias, int Ring, int IP,
                                      int Port, int Bit, int iErr)
{
    AnsiString sMsg="";
    AnsiString sDir = bDOType ? AnsiString("輸出 (DO)") : AnsiString("輸入 (DI)");
    AnsiString sDevType = "Ring/IP 超出範圍, 無法判讀";

    if(Ring>=0 && Ring<MAXRing && IP>=0 && IP<MAXIP)
    {
        sDevType = GetUseMNetIPDevTypeName(iUseMNetIP[Ring][IP]);
    }

    sMsg.sprintf("[IO 設定錯誤] %s\r\n"
                 "點位名稱 : %s (%s)\r\n"
                 "CSV 位置 : Lane=%d, IP=%d, Port=%d, Bit=%d\r\n"
                 "實際模組 : Ring %d / IP %d = %s\r\n"
                 "錯誤說明 :\r\n%s\r\n"
                 "修正建議 :\r\n"
                 "  1. 開啟 D:\\HT9045\\System\\IO_Table.csv 搜尋 \"%s\"\r\n"
                 "  2. 對照上方「實際模組」欄, 確認 Lane/IP/Port/Bit 是否合法\r\n"
                 "  3. 必要時開啟 D:\\HT9045\\Error\\MNetLog*.txt 比對開機掃描結果",
                 sFuncName.c_str(),
                 sAlias.c_str(), sDir.c_str(),
                 Ring, IP, Port, Bit,
                 Ring, IP, sDevType.c_str(),
                 GetIOErrStr(iErr).c_str(),
                 sAlias.c_str());
    return sMsg;
}
//---------------------------------------------------------------------------
