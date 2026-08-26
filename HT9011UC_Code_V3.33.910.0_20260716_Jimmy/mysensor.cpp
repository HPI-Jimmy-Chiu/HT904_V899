#include "MachineDefine.h"
#pragma hdrstop

#include "mysensor.h"

#include "MyLaneIo.h"
//#include "myio.h"
#include "cmydef.h"
#include "mymessbox.h"
#include "uPadInterface.h"                                                      //Ken 20210702 AddPadInterface
//---------------------------------------------------------------------------
#pragma package(smart_init)

class TMySensor Sen[MAX_SENSOR_ITEM];
class TMySensor SenBackUp[MAX_TTL_BIT];                                         //Alick 20161011 (Steven) : TTL支援8Site

_fastcall TMySensor::TMySensor()
{
    Name="";
//    OnCode="";
//    OffCode="";
    Ring=0;
    IP=0;
    Port=0;
    Bit=0;
    Type=0;
    Enable=0;
    Using="";
    ISABase=0;
}
//---------------------------------------------------------------------------
_fastcall TMySensor::~TMySensor()
{
    Enable=false;
}
//---------------------------------------------------------------------------
bool TMySensor::Status()
{
    bool ret=false;
    if(iControlPanelMode==1 && fPadInterface->IsPadKey(Name))                   //Ken 20210702 AddPadInterface  //KenHsieh 20211227 : 通訊面板不卡Database
    {
        ret=fPadInterface->ProcessScanKey(Name);
        State=1;
        return ret;
    }
    else
    {
        if(Enable==false)
        {
            State=-1;
            return false;
        }

        if(ISABase==eMotionNet ||
           ISABase==ePCI1203 ||                                                 //Sam 20230724 : 新增 PCI1203 IO 模組
           ISABase==ePLCbase)
        {
            ret=MyLaneIO.IOInputBit(Ring, IP, Port, Bit, ISABase, Name);        //Sam 20230724 : 新增 PCI1203 IO 模組
        }
        else if(ISABase==eISABase ||                                            //Nickliu 20230309 add Cylinder ISABase Type
                ISABase==ePCI1735U)                                             //Alick 20160809 modify for PCI-1735U
        {
            ret=IOInputBit(Port, Bit);
        }

        if(Type)
        {
            State=1;
            return ret;
        }
        else
        {
            State=0;
            return !ret;
        }
    }
}
//---------------------------------------------------------------------------
bool TMySensor::IsOn()
{
    if(iControlPanelMode==1 && fPadInterface->IsPadKey(Name))                   //Ken 20210702 AddPadInterface  //KenHsieh 20211227 : 通訊面板不卡Database
    {
        return fPadInterface->ProcessScanKey(Name);
    }
    else
    {
        if(Enable==false)
        {
            State=-1;
            return false;
        }

        bool ret=false;
        if(ISABase==eMotionNet ||
           ISABase==ePCI1203 ||                                                 //Sam 20230724 : 新增 PCI1203 IO 模組
           ISABase==ePLCbase)
        {
            ret=MyLaneIO.IOInputBit(Ring, IP, Port, Bit, ISABase, Name);
        }
        else if(ISABase==eISABase ||                                            //Nickliu 20230309 add Cylinder ISABase Type
                ISABase==ePCI1735U)                                             //Alick 20160809 modify for PCI-1735U
        {
            ret=IOInputBit(Port, Bit);
        }

        if(Type)
        {
            State=1;
            return ret;
        }
        else
        {
            State=0;
            return !ret;
        }
    }
}
//---------------------------------------------------------------------------
bool TMySensor::IsOff()
{
    bool ret=false;
    if(iControlPanelMode==1 && fPadInterface->IsPadKey(Name))                   //Ken 20210702 AddPadInterface  //KenHsieh 20211227 : 通訊面板不卡Database
    {
        ret=fPadInterface->ProcessScanKey(Name);
        State=0;
        return !ret;
    }
    else
    {
        if(Enable==false)
        {
            State=-1;
            return false;
        }

        if(ISABase==eMotionNet ||
           ISABase==ePCI1203 ||                                                 //Sam 20230724 : 新增 PCI1203 IO 模組
           ISABase==ePLCbase)
        {
            ret=MyLaneIO.IOInputBit(Ring, IP, Port, Bit, ISABase, Name);
        }
        else if(ISABase==eISABase ||                                            //Nickliu 20230309 add Cylinder ISABase Type
                ISABase==ePCI1735U)                                             //Alick 20160809 modify for PCI-1735U
        {
            ret=IOInputBit(Port, Bit);
        }

        if(Type)
        {
            State=0;
            return !ret;
        }
        else
        {
            State=1;
            return  ret;
        }
    }
}
//---------------------------------------------------------------------------
void CopySensor(TMySensor *Source, TMySensor *Target)
{
    if(Source->ISABase==eISABase ||                                             //Nickliu 20230309 add Cylinder ISABase Type
       Source->ISABase==ePCI1735U)                                              //Alick 20160809 modify for PCI-1735U
    {
        Target->Port=Source->Port;
        Target->Bit =Source->Bit;
        Target->Type=Source->Type;
    }
    else
    {
        Target->Ring=Source->Ring;
        Target->IP  =Source->IP;
        Target->Port=Source->Port;
        Target->Bit =Source->Bit;
        Target->Type=Source->Type;
    }
}
//---------------------------------------------------------------------------
/*
bool TMySensor::CheckSenPortRangeErr(int Type)
{
    int iMax =0 ,iMin =0;
    AnsiString sType[]={"Status","IsOn","IsOff"};
    AnsiString sAlm;
    sAlm.sprintf("Sensor %s -- %s Ring %d|IP %d |Port %d, Fail", Name, sType[Type], Ring, IP, Port);

    switch(MachineTypeChoice)
    {
        case Type_HT9045:
            if(USE_46_SENSOR_DB || USE_46_SUCKER_DB)                            //Steven 20131008 : HT9045使用46配電
            {
                iMax=DI_IP_Max_9046;
                iMin=DI_IP_Min_9046;
            }
            else
            {
                iMax=DI_IP_Max_9045;
                iMin=DI_IP_Min_9045;
            }
            break;
        case Type_HT9046:
            iMax = DI_IP_Max_9046;
            iMin = DI_IP_Min_9046;
            break;
        case  Type_HT9046_LS:
            iMax = DI_IP_Max_9046LS;
            iMin = DI_IP_Min_9046LS;
            break;
        case Type_HT9045_12Site:
            if(USE_46_SENSOR_DB || USE_46_SUCKER_DB)                            //Steven 20131008 : HT9045使用46配電
            {
                iMax=DO_IP_Max_9046;
                iMin=DO_IP_Min_9046;
            }
            else
            {
                iMax = DI_IP_Max_9045_12Site;
                iMin = DI_IP_Min_9045_12Site;
            }
            break;
    }

    if(Ring!=0 || IP<iMin || IP>iMax || Port<0 || Port>3 || Bit<0 || Bit>7)
    {
        ShowMyMessage(sAlm);
        return true;
    }

    return false;
}      */
//---------------------------------------------------------------------------
