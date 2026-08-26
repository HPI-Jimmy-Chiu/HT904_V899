#include "MachineDefine.h"
#pragma hdrstop

#include "myswitch.h"

#include "MyLaneIo.h"
#include "uPadInterface.h"                                                      //Ken 20210702 AddPadInterface
#include "cmydef.h"                                                             //Ken 20210702 AddPadInterface
//---------------------------------------------------------------------------
#pragma package(smart_init)
class TMySwitch SW[MAX_SWITCH_ITEM];
class TMySwitch SWBackup[MAX_TTL_SWITCH];                                       //Alick 20161011 (Steven) : TTLや穿8Site
_fastcall TMySwitch::TMySwitch()
{
    Name="";
    Ring=0;
    IP=0;
    Port=0;
    Bit=0;
    Type=0;
    Enable=false;
    OutValue=false;
    SetValue=false;
    Using="";
    ISABase=0;
};
//------------------------------------------------------------------------------
void TMySwitch::On()
{
    //if(Type) OutValue=true;
    //else     OutValue=false;
    OutValue=true;

    if(iControlPanelMode==1 && fPadInterface->IsPadButton(Name))                //Ken 20210702 AddPadInterface  //KenHsieh 20211227 : 硄癟狾ぃDatabase
    {
        fPadInterface->SendSwitchStatus(Name,true);
    }
    else
    {
        if(Enable==false)
            return;

        if(Type)
        {
            if(ISABase==eMotionNet||
               ISABase==ePCI1203)                                               //Sam 20230724 : 穝糤 PCI1203 IO 家舱
            {
                MyLaneIO.IOBitOn(Ring, IP, Port, Bit, ISABase, Name);           //Sam 20230724 : 穝糤 PCI1203 IO 家舱
            }
            else if(ISABase==eISABase ||                                        //Nickliu 20230309 add Cylinder ISABase Type
                    ISABase==ePCI1735U ||                                       //Alick 20160809 modify for PCI-1735U
                    ISABase==ePLCbase)
            {
                IOBitOn(Port, Bit);
            }
        }
        else
        {
            if(ISABase==eMotionNet ||
               ISABase==ePCI1203)                                               //Sam 20230724 : 穝糤 PCI1203 IO 家舱
            {
                MyLaneIO.IOBitOff(Ring, IP, Port, Bit, ISABase, Name);          //Sam 20230724 : 穝糤 PCI1203 IO 家舱
            }
            else if(ISABase==eISABase ||                                        //Nickliu 20230309 add Cylinder ISABase Type
                    ISABase==ePCI1735U ||                                       //Alick 20160809 modify for PCI-1735U
                    ISABase==ePLCbase)
            {
                IOBitOff(Port, Bit);
            }
        }
    }
}
//------------------------------------------------------------------------------
void TMySwitch::Off()
{
    OutValue=false;

    if(iControlPanelMode==1 && fPadInterface->IsPadButton(Name))                //Ken 20210702 AddPadInterface  //KenHsieh 20211227 : 硄癟狾ぃDatabase
    {
        fPadInterface->SendSwitchStatus(Name, false);
    }
    else
    {
        if(Enable==false)
            return;

        if(Type)
        {
            if(ISABase==eMotionNet ||
               ISABase==ePCI1203)                                               //Sam 20230724 : 穝糤 PCI1203 IO 家舱
            {
                MyLaneIO.IOBitOff(Ring, IP, Port, Bit, ISABase, Name);          //Sam 20230724 : 穝糤 PCI1203 IO 家舱
            }
            else if(ISABase==eISABase ||                                        //Nickliu 20230309 add Cylinder ISABase Type
                    ISABase==ePCI1735U ||                                       //Alick 20160809 modify for PCI-1735U
                    ISABase==ePLCbase)
            {
                IOBitOff(Port, Bit);
            }
        }
        else
        {
            if(ISABase==eMotionNet ||
               ISABase==ePCI1203)                                               //Sam 20230724 : 穝糤 PCI1203 IO 家舱
            {
                MyLaneIO.IOBitOn(Ring, IP, Port, Bit, ISABase, Name);           //Sam 20230724 : 穝糤 PCI1203 IO 家舱
            }
            else if(ISABase==eISABase ||                                        //Nickliu 20230309 add Cylinder ISABase Type
                    ISABase==ePCI1735U ||                                       //Alick 20160809 modify for PCI-1735U
                    ISABase==ePLCbase)
            {
                IOBitOn(Port, Bit);
            }
        }
    }
}
//------------------------------------------------------------------------------
bool TMySwitch::Status()
{
    if(iControlPanelMode==1 && fPadInterface->IsPadButton(Name))                //Ken 20210702 AddPadInterface  //KenHsieh 20211227 : 硄癟狾ぃDatabase
    {
        OutValue=fPadInterface->ProcessScanKey(Name);
        return OutValue;                                                        //Steven 20211229 : add return value
    }
    else
    {
        if(Enable==false)
            return false;

        if(ISABase==eMotionNet ||
           ISABase==ePCI1203)                                                   //Sam 20230724 : 穝糤 PCI1203 IO 家舱
        {
            OutValue=MyLaneIO.IOOutBitStatus(Ring, IP, Port, Bit, ISABase, Name);                                       //Sam 20230724 : 穝糤 PCI1203 IO 家舱
        }
        else if(ISABase==eISABase ||                                            //Nickliu 20230309 add Cylinder ISABase Type
                ISABase==ePCI1735U ||                                           //Alick 20160809 modify for PCI-1735U
                ISABase==ePLCbase)
        {
            OutValue=IOOutBitStatus(Port, Bit);
        }

        if(Type)
            return OutValue;
        else
            return !OutValue;
    }
}
//------------------------------------------------------------------------------
void TMySwitch::OnOff(bool bOn)
{
    if(bOn)
        On();
    else
        Off();
}
//------------------------------------------------------------------------------
void CopySwitch(TMySwitch *Source ,TMySwitch *Target)
{
    if(Source->ISABase==eISABase ||
       Source->ISABase==ePCI1735U ||                                            //Alick 20160809 modify for PCI-1735U
       Source->ISABase==ePLCbase)
    {
        Target->Port=Source->Port;
        Target->Bit =Source->Bit;
        Target->Type=Source->Type;
        Target->ISABase=Source->ISABase;
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
//------------------------------------------------------------------------------
