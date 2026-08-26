#include "MachineDefine.h"
#pragma hdrstop

#include "mycylin.h"

//#include "halarm.h"
#include "MyLaneIo.h"
#include "cmydef.h"
#include "mymessbox.h"
#include "SmartDiagnostic.h"
//#include "main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

class TMyCylinder Cylinder[MaxCylinderItem];
_fastcall TMyCylinder::TMyCylinder()
{
    Enable=false;
    Status=false;
    bCylinderOn=false;
    OnTask=1;
    OffTask=1;
    OnOff=2;
    AlarmEnable=false;
    OnAlarmCode=0;
    OffAlarmCode=0;
    OnDelayTime=0;
    OffDelayTime=0;

    OutRingUse="";
    OutRing=0;
    OutIP=0;
    OutPort=0;
    OutBit=0;
    OutType=TYPE_A;                                                             // A or B Type
    OutISABase=eMotionNet;

    OffSenRingUse="";
    OnSenRingUse="";
    OnSenRing=0;
    OnSenIP=0;
    OnSenPort=0;
    OnSenBit=0;
    OnSenType=TYPE_A;
    OnSenISABase=eMotionNet;

    OffSenRing=0;
    OffSenIP=0;
    OffSenPort=0;
    OffSenBit=0;
    OffSenType=TYPE_A;
    OffSenISABase=eMotionNet;

    NeedFinishOnFunction=false;
    NeedFinishOffFunction=false;
    OnTryTask=0;
    OffTryTask=0;
    Change=false;
    CylinderName="";
    OnSensorName="";
    OffSensorName="";
//    iX=0;
//    iY=0;
    PTempWinCtrl=NULL;
    ISABase=eMotionNet;                                                         //Nickliu 20230306 add IO Use ISABase
    bCheckSafeDoor=false;                                                       //Steven 20230703 : Add Cylinder action check SafeDoor

    //Eastsun 20260521 整合//Sam 20230516 : Pre Alrm Cylinder
    //==>
    bfirst=true;
    bOnOffState=false;
    iOnOffCount=0;
    iTimeOutCount=0;
    dOnTime=0.0;
    dOffTime=0.0;
    sResetTime="";
    sListOnTime=new TStringList();
    sListOffTime=new TStringList();
    //<==
    //Eastsun 20260521 整合//Sam 20230516 : Pre Alrm Cylinder
}
//---------------------------------------------------------------------------
extern HAlarm  *Alarm;
void SetAlarm(int AlarmCode)
{
    Alarm->Set(AlarmCode);
}
//---------------------------------------------------------------------------
void ClearAlarm(int AlarmCode)
{
    Alarm->Clear(AlarmCode);
}
//---------------------------------------------------------------------------
bool TMyCylinder::Reset()
{
    OnTask=1;
    OffTask=1;
    OnTryTask=0;
    OffTryTask=0;
    Change=false;
    return true;
}
//---------------------------------------------------------------------------
bool TMyCylinder::GetOutBit()
{
    if(Enable)
    {
        if(OutISABase==eMotionNet ||
           OutISABase==ePCI1203)                                                //Sam 20230724 : 新增 PCI1203 IO 模組
        {
            return MyLaneIO.IOOutBitStatus(OutRing, OutIP, OutPort, OutBit, OutISABase, CylinderName);                  //Sam 20230724 : 新增 PCI1203 IO 模組
        }
        else if(OutISABase==eISABase ||                                         //Nickliu 20230309 add Cylinder ISABase Type
                OutISABase==ePCI1735U ||
                OutISABase==ePLCbase)
        {
            return IOOutBitStatus(OutPort, OutBit);
        }
    }
    return false;
}
//---------------------------------------------------------------------------
bool TMyCylinder::OnStatus()
{
    bool InRet;
    if(OnSenEnable)
    {
        if(OnSenISABase==eMotionNet ||
           OnSenISABase==ePCI1203)                                              //Sam 20230724 : 新增 PCI1203 IO 模組
        {
            InRet=MyLaneIO.IOInputBit(OnSenRing, OnSenIP, OnSenPort, OnSenBit, OnSenISABase,OnSensorName);              //Sam 20230724 : 新增 PCI1203 IO 模組
        }
        else if(OnSenISABase==eISABase ||                                       //Nickliu 20230309 add Cylinder ISABase Type
                OnSenISABase==ePCI1735U ||
                OnSenISABase==ePLCbase)
        {
            InRet=IOInputBit(OnSenPort, OnSenBit);
        }
    }

    if(OnSenEnable==false)
        return false;
    else if(OnSenType==TYPE_A && InRet==1)
        return true;
    else if(OnSenType==TYPE_B && InRet==0)
        return true;
    else
        return false;
}
//---------------------------------------------------------------------------
bool TMyCylinder::OnSensor()
{
    if(Enable==false)
    {
        return true;
    }

    if(OnSenEnable)
    {
        return OnStatus();
    }
    else if(OffSenEnable)
    {
        return !OffStatus();
    }
    return true;
}
//---------------------------------------------------------------------------
bool TMyCylinder::OffSensor()
{
    if(Enable==false)
    {
        return true;
    }

    if(OffSenEnable)
    {
        return OffStatus();
    }
    else if(OnSenEnable)
    {
        return !OnStatus();
    }
    else
    {
        return true;
    }
}
//---------------------------------------------------------------------------
bool TMyCylinder::OffStatus()
{
    bool InRet;

    if(OffSenEnable)
    {
        if(OffSenISABase==eMotionNet ||
           OffSenISABase==ePCI1203)                                             //Sam 20230724 : 新增 PCI1203 IO 模組
        {
            InRet=MyLaneIO.IOInputBit(OffSenRing, OffSenIP, OffSenPort, OffSenBit, OffSenISABase, OffSensorName);       //Sam 20230724 : 新增 PCI1203 IO 模組
        }
        else if(OffSenISABase==eISABase ||                                      //Nickliu 20230309 add Cylinder ISABase Type
                OffSenISABase==ePCI1735U ||
                OffSenISABase==ePLCbase)
        {
            InRet=IOInputBit(OffSenPort, OffSenBit);
        }
    }

    if(OffSenEnable==false)
        return false;
    else if(OffSenType==TYPE_A && InRet==1)
        return true;
    else if(OffSenType==TYPE_B && InRet==0)
        return true;
    else
        return false;
}
//---------------------------------------------------------------------------
void TMyCylinder::OnSwitch()                                                    //Steven 20230721 : 統一開關控制
{
    if(Enable)
    {
        if(OutType==TYPE_A)
        {
            if(OutISABase==eMotionNet ||
               OutISABase==ePCI1203)                                            //Sam 20230724 : 新增 PCI1203 IO 模組
            {
                MyLaneIO.IOBitOn(OutRing, OutIP, OutPort, OutBit, OutISABase, CylinderName);                            //Sam 20230724 : 新增 PCI1203 IO 模組
            }
            else if(OutISABase==eISABase ||                                     //Nickliu 20230309 add Cylinder ISABase Type
                    OutISABase==ePCI1735U ||
                    OutISABase==ePLCbase)
            {
                IOBitOn(OutPort, OutBit);
            }
            fSmartDiagnostic->GetCyliderOnCount(CylinderName);                  //pig 20190420 AddSmartDiagnosticFunction  //JerryYang 20220215 : cylinder作動次數計數
        }
        else
        {
            if(OutISABase==eMotionNet ||
               OutISABase==ePCI1203)                                            //Sam 20230724 : 新增 PCI1203 IO 模組
            {
                MyLaneIO.IOBitOff(OutRing, OutIP, OutPort, OutBit, OutISABase, CylinderName);                           //Sam 20230724 : 新增 PCI1203 IO 模組
            }
            else if(OutISABase==eISABase ||                                     //Nickliu 20230309 add Cylinder ISABase Type
                    OutISABase==ePCI1735U ||
                    OutISABase==ePLCbase)
            {
                IOBitOff(OutPort, OutBit);
            }
            fSmartDiagnostic->GetCyliderOffCount(CylinderName);                 //pig 20190420 AddSmartDiagnosticFunction  //JerryYang 20220215 : cylinder作動次數計數
        }
        AddOnCount();                                                           //Eastsun 20260521 整合//Sam 20230516 : Pre Alrm Cylinder
    }
}
//---------------------------------------------------------------------------
void TMyCylinder::OffSwitch()                                                   //Steven 20230721 : 統一開關控制
{
    if(Enable)
    {
        if(OutType==TYPE_B)
        {
            if(OutISABase==eMotionNet ||
               OutISABase==ePCI1203)                                            //Sam 20230724 : 新增 PCI1203 IO 模組
            {
                MyLaneIO.IOBitOn(OutRing, OutIP, OutPort, OutBit, OutISABase, CylinderName);                            //Sam 20230724 : 新增 PCI1203 IO 模組
            }
            else if(OutISABase==eISABase ||                                     //Nickliu 20230309 add Cylinder ISABase Type
                    OutISABase==ePCI1735U ||
                    OutISABase==ePLCbase)
            {
                IOBitOn(OutPort, OutBit);
            }
            fSmartDiagnostic->GetCyliderOnCount(CylinderName);                  //pig 20190420 AddSmartDiagnosticFunction  //JerryYang 20220215 : cylinder作動次數計數
        }
        else
        {
            if(OutISABase==eMotionNet ||
               OutISABase==ePCI1203)                                            //Sam 20230724 : 新增 PCI1203 IO 模組
            {
                MyLaneIO.IOBitOff(OutRing, OutIP, OutPort, OutBit, OutISABase, CylinderName);                           //Sam 20230724 : 新增 PCI1203 IO 模組
            }
            else if(OutISABase==eISABase ||                                     //Nickliu 20230309 add Cylinder ISABase Type
                    OutISABase==ePCI1735U ||
                    OutISABase==ePLCbase)
            {
                IOBitOff(OutPort, OutBit);
            }
            fSmartDiagnostic->GetCyliderOffCount(CylinderName);                 //pig 20190420 AddSmartDiagnosticFunction  //JerryYang 20220215 : cylinder作動次數計數
        }
        AddOffCount();                                                          //Eastsun 20260521 整合//Sam 20230516 : Pre Alrm Cylinder
    }
}
//---------------------------------------------------------------------------
void TMyCylinder::On()                                                          // no delay,no alarm
{
    Status=true;
    Change=true;
    bCylinderOn=true;
    OnTask=1;
    OnSwitch();
    UpdateSimulateCompomentPosition();
}
//---------------------------------------------------------------------------
void TMyCylinder::Off()                                                         // no delay,no alarm
{
    Status=false;
    Change=false;
    bCylinderOn=false;
    OffTask=1;
    OffSwitch();
    UpdateSimulateCompomentPosition();
}
//---------------------------------------------------------------------------
bool TMyCylinder::Push()
{
    bool InRet;
    Change=true;
    int &Task=OnTask;
    OnSwitch();

    if(Task==1 || Task==2)
    {
        if(OnSenEnable)                                                         //  has install onsensor
        {
            if(OnAlarmTime==0)
            {
                InRet=OnStatus();
                if(InRet)                                                       // ok
                {
                    Task=100;                                                   // do on delay
                }
                else
                {
                    Status=false;
                    Task=1;
                    if(SystemStart)
                    {
                        OnTryTask++;
                        if(OnTryTask>=2)
                        {
                            iTimeOutCount++;                                    //Eastsun 20260521 整合//Sam 20230516 : Pre Alrm Cylinder
                            SetAlarm(OnAlarmCode);
                            OnTryTask=0;
                            Task=1;
                        }
                        else
                        {
                            Task=2;
                        }
                    }
                    return false;
                }
            }
            else
            {
                if(OnStatus())                                                  // already on
                {
                    Task=100;
                }
                else
                {
                    TOn.Set0_1SecAndOn(OnAlarmTime);
                    Task=50;
                }
            }
        }
        else
        {
            Task=100;                                                           //need delay
        }
    }

    if(Task==50)
    {
        InRet=OnStatus();
        if(InRet)
        {
            Task=100;                                                           // do on delay
        }
        else
        {
            if(bHandlerPause)                                                   //Steven 20190123 : 紀錄Handler被暫停, 重置Timer
            {
                TOn.Set0_1SecAndOn(OnAlarmTime);
                tOnOff.LatchCycleTime(true);                                    //Eastsun 20260521 整合//Sam 20230516 : Pre Alrm Cylinder
                return false;
            }

            if(TOn.Off())
            {
                Status=false;
                Task=1;
                if(SystemStart)
                {
                    OnTryTask++;
                    if(OnTryTask>=2)
                    {
                        iTimeOutCount++;                                        //Eastsun 20260521 整合//Sam 20230516 : Pre Alrm Cylinder
                        SetAlarm(OnAlarmCode);
                        OnTryTask=0;
                    }
                    else
                    {
                        Task=2;
                    }
                }
                return false;
            }
            else
            {
                return false;
            }
        }
    }

    if(Task>=100)
    {
        switch(Task)
        {
            case 100:
                LatchOnffTime();                                                //Eastsun 20260521 整合//Sam 20230516 : Pre Alrm Cylinder
                if(OnDelayTime==0)
                {
                    Task=1;
                    Status=true;
                    ClearAlarm(OnAlarmCode);
                    OnTryTask=0;
                    bCylinderOn=true;
                    AddOnCount(true);                                           //Eastsun 20260521 整合//Sam 20230516 : Pre Alrm Cylinder
                    return true;
                }
                else
                {
                    TOnDelay.Set0_1SecAndOn(OnDelayTime);
                    Task=101;
                }
            case 101:
                if(TOnDelay.Off())
                {
                    Status=true;
                    ClearAlarm(OnAlarmCode);
                    Task=1;
                    OnTryTask=0;
                    bCylinderOn=true;
                    AddOnCount(true);                                           //Eastsun 20260521 整合//Sam 20230516 : Pre Alrm Cylinder
                    UpdateSimulateCompomentPosition();
                    return true;
                }
                else
                {
                    return false;
                }
        }
    }
    ClearAlarm(OnAlarmCode);
    Task=1;
    OnTryTask=0;
    bCylinderOn=true;
    UpdateSimulateCompomentPosition();
    return true;
}
//---------------------------------------------------------------------------
bool TMyCylinder::Pop()
{
    bool InRet;
    Change=false;
    int &Task=OffTask;
    OffSwitch();

    if(Task==1 || Task==2)
    {
        if(OffSenEnable)                                                        //  has install onsensor
        {
            if(OffAlarmTime==0)
            {
                InRet=OffStatus();
                if(InRet)
                {
                    Task=100;                                                   // do on delay
                }
                else
                {
                    Status=true;

                    if(SystemStart)
                    {
                        OffTryTask++;
                        if(OffTryTask>=2)
                        {
                           iTimeOutCount++;                                     //Eastsun 20260521 整合//Sam 20230516 : Pre Alrm Cylinder
                           SetAlarm(OffAlarmCode);
                           OffTryTask=0;
                           Task=1;
                        }
                        else
                        {
                           Task=2;
                        }
                    }
                    return false;
                }
            }
            else
            {
                if(OffStatus())                                                 //already off
                {
                    Task=100;
                }
                else
                {
                    TOff.Set0_1SecAndOn(OffAlarmTime);
                    Task=50;
                }
            }
        }
        else
        {
            Task=100;                                                           //need delay
        }
    }

    if(Task==50)
    {
        InRet=OffStatus();
        if(InRet)
        {
            Task=100;                                                           // do on delay
        }
        else
        {
            if(bHandlerPause)                                                   //Steven 20190123 : 紀錄Handler被暫停, 重置Timer
            {
                TOff.Set0_1SecAndOn(OffAlarmTime);
                tOnOff.LatchCycleTime(true);                                    //Eastsun 20260521 整合//Sam 20230516 : Pre Alrm Cylinder
                return false;
            }

            if(TOff.Off())
            {
                Status=true;
                Task=1;
                if(SystemStart)
                {
                    OffTryTask++;
                    if(OffTryTask>=2)
                    {
                       iTimeOutCount++;                                         //Eastsun 20260521 整合//Sam 20230516 : Pre Alrm Cylinder
                       SetAlarm(OffAlarmCode);
                       OffTryTask=0;
                    }
                    else
                    {
                       Task=2;
                    }
                }
                return false;
            }
            else
            {
                return false;
            }
        }
    }

    if(Task>=100)
    {
        switch(Task)
        {
            case 100:
                LatchOnffTime();                                                //Eastsun 20260521 整合//Sam 20230516 : Pre Alrm Cylinder
                if(OffDelayTime==0)
                {
                    Status=false;
                    ClearAlarm(OffAlarmCode);
                    Task=1;
                    OffTryTask=0;
                    bCylinderOn=false;
                    return true;
                }
                else
                {
                    TOffDelay.Set0_1SecAndOn(OffDelayTime);
                    Task=101;
                }
            case 101:
                if(TOffDelay.Off())
                {
                    Task=1;
                    Status=false;
                    ClearAlarm(OffAlarmCode);
                    bCylinderOn=false;
                    UpdateSimulateCompomentPosition();
                    OffTryTask=0;
                    return true;
                }
                else
                {
                    return false;
                }
        }
    }
    Status=false;
    ClearAlarm(OffAlarmCode);
    Task=1;
    OffTryTask=0;
    bCylinderOn=false;
    UpdateSimulateCompomentPosition();
    return true;
}
//---------------------------------------------------------------------------
/*void TMyCylinder::ReCheck()
{
    if(Enable==false)
        return;

    bool bOff=false, bOn=false;                                                 //Nickliu 20230309 add Cylinder ISABase Type
    if(OnSenISABase==eMotionNet)
        bOn =MyLaneIO.IOInputBit(OnSenRing, OnSenIP, OnSenPort, OnSenBit, OnSensorName);
    else if(OnSenISABase==eISABase ||                                           //Nickliu 20230309 add Cylinder ISABase Type
            OnSenISABase==ePCI1735U)
        bOn =IOInputBit(OnSenPort, OnSenBit);

    if(OffSenISABase==eMotionNet)
        bOff=MyLaneIO.IOInputBit(OffSenRing, OffSenIP, OffSenPort, OffSenBit, OffSensorName);
    else if(OnSenISABase==eISABase ||                                           //Nickliu 20230309 add Cylinder ISABase Type
            OnSenISABase==ePCI1735U)
        bOff=IOInputBit(OffSenPort, OffSenBit);

    if(OffSenType==0)
        bOff=!bOff;
    if(OnSenType==0)
        bOn=!bOn;

    if(OffSenEnable && OnSenEnable && Enable)
    {
        if((bOn && bOff) ||( bOn==false && bOff==false))                        // on & off sensor all error
        {
            Status=MyLaneIO.IOOutBitStatus(OutRing,OutIP,OutPort,OutBit,CylinderName);
            if(Status)
            {
                Alarm->Set(OnAlarmCode);
            }
            else
            {
                Alarm->Set(OffAlarmCode);
            }
            return;
        }

        if(bOn)
        {
            Status=true;
            return;
        }
        else
        {
            Status=false;
            return;
        }
    }
    else if(OffSenEnable && Enable)
    {
        if(bOff)
        {
            Status=false;
        }
        else
        {
            Status=true;
        }
    }
    else if(OnSenEnable && Enable)
    {
        if(bOn)
        {
            Status=true;
        }
        else
        {
            Status=false;
        }
    }
    else
    {
        if(OutISABase==eMotionNet)
        {
            Status=MyLaneIO.IOOutBitStatus(OutRing, OutIP, OutPort, OutBit, CylinderName);
        }
        else if(OutISABase==eISABase ||                                         //Nickliu 20230309 add Cylinder ISABase Type
                OutISABase==ePCI1735U)
        {
            Status=IOOutBitStatus(OutPort, OutBit);
        }
    }
    OnTryTask=0;
    OffTryTask=0;
}*/
//---------------------------------------------------------------------------
bool TMyCylinder::FinshFullMotion()
{
    if(OnTask!=1 || OffTask!=1)
        return false;
    return true;
}
//---------------------------------------------------------------------------
bool TMyCylinder::FinshOnMotion()
{
    if(OnTask!=1 || OffTask!=1)
        return false;
    return true;
}
//---------------------------------------------------------------------------
bool TMyCylinder::FinshOffMotion()
{
    if(OnTask!=1 || OffTask!=1)
        return false;
    return true;
}
//---------------------------------------------------------------------------
void TMyCylinder::ResetOnAlarmTime()
{
    OnTask=2;
}
//---------------------------------------------------------------------------
void TMyCylinder::ResetOffAlarmTime()
{
    OffTask=2;
}
//---------------------------------------------------------------------------
void TMyCylinder::ResetOnDelayTime()
{
    OnTask=2;
}
//---------------------------------------------------------------------------
void TMyCylinder::ResetOffDelayTime()
{
    OffTask=2;
}
//---------------------------------------------------------------------------
void TMyCylinder::UpdateSimulateCompomentPosition()                             // Simulate run //
{
    if(PTempWinCtrl!=NULL)
    {
        if(bCylinderOn)                                                         // ben edit 20110726 - s //
        {
            PTempWinCtrl->Left=iOnLeft;
            PTempWinCtrl->Top =iOnTop;
        }
        else
        {
            PTempWinCtrl->Left=iOffLeft;
            PTempWinCtrl->Top =iOffTop;
        }                                                                       // ben edit 20110726 - e //
    }
}
//---------------------------------------------------------------------------
void TMyCylinder::SetSimulateCompoment(TObject *PCtrl, TAnchorKind Alignment, int simuStart, int simuEnd)
{
    PTempWinCtrl=dynamic_cast<TControl *> (PCtrl);
    if(PTempWinCtrl!=NULL)
    {
        iOnLeft =(Alignment==akLeft || Alignment==akRight )?PTempWinCtrl->Left:simuStart;
        iOffLeft=(Alignment==akLeft || Alignment==akRight )?PTempWinCtrl->Left:simuEnd;
        iOnTop  =(Alignment==akTop  || Alignment==akBottom)?PTempWinCtrl->Top:simuStart;
        iOffTop =(Alignment==akTop  || Alignment==akBottom)?PTempWinCtrl->Top:simuEnd;
        UpdateSimulateCompomentPosition();
    }
}
//---------------------------------------------------------------------------
//Sam 20230516 : Pre Alrm Cylinder
//==>
void TMyCylinder::AddOnCount(bool bResetTimer)
{
    if(bOnOffState!=true || bfirst)
    {
        bfirst=false;
        if(bResetTimer && OnSenEnable)
            tOnOff.LatchCycleTime(true);
        bOnOffState=true;
        iOnOffCount++;
    }
}
//---------------------------------------------------------------------------
void TMyCylinder::AddOffCount(bool bResetTimer)
{
    if(bOnOffState!=false || bfirst)
    {
        bfirst=false;
        if(bResetTimer && OffSenEnable)
            tOnOff.LatchCycleTime(true);
        bOnOffState=false;
        iOnOffCount++;
    }
}
//---------------------------------------------------------------------------
void TMyCylinder::LatchOnffTime()
{
    if(bOnOffState)
    {
        if(OnSenEnable)
        {
            dOnTime=(double)tOnOff.LatchCycleTime()/1000.0;
            AddOnTime(dOnTime);
        }
    }
    else
    {
        if(OffSenEnable)
        {
            dOffTime=(double)tOnOff.LatchCycleTime()/1000.0;
            AddOffTime(dOffTime);
        }
    }
}
//---------------------------------------------------------------------------
void TMyCylinder::AddOnTime(double dOnTime)
{
    AnsiString sOnTime="";
    if(sListOnTime->Count>=5)
        sListOnTime->Delete(sListOnTime->Count-1);
    sOnTime.sprintf("%2.3f",dOnTime);
    sListOnTime->Insert(0,sOnTime);
}
//---------------------------------------------------------------------------
void TMyCylinder::AddOffTime(double dOffTime)
{
    AnsiString sOffTime="";
    if(sListOffTime->Count>=5)
        sListOffTime->Delete(sListOffTime->Count-1);
    sOffTime.sprintf("%2.3f",dOffTime);
    sListOffTime->Insert(0,sOffTime);
}
//---------------------------------------------------------------------------
AnsiString TMyCylinder::GetOnTime()
{
    AnsiString sOnTime="NA";
    if(OnSenEnable && bCylPreAlarmByPassT==false)
        sOnTime.sprintf("%2.3f",dOnTime);
    return sOnTime;
}
//---------------------------------------------------------------------------
AnsiString TMyCylinder::GetOffTime()
{
    AnsiString sOffTime="NA";
    if(OffSenEnable && bCylPreAlarmByPassT==false)
        sOffTime.sprintf("%2.3f",dOffTime);
    return  sOffTime;
}
//---------------------------------------------------------------------------
AnsiString TMyCylinder::GetOnTimeAvg()
{
    int iCount=0;
    double dSum=0.0,dAvg=0.0;
    AnsiString sAvg="NA";
    for(int i=0; i<sListOnTime->Count; i++)
    {
        dSum+=atof(sListOnTime->Strings[i].c_str());
        iCount++;
    }

    if(iCount>0)
        dAvg=dSum/(double)iCount;
    if(OnSenEnable && bCylPreAlarmByPassT==false)
        sAvg.sprintf("%2.3f",dAvg);
    return sAvg;
}
//---------------------------------------------------------------------------
AnsiString TMyCylinder::GetOffTimeAvg()
{
    int iCount=0;
    double dSum=0.0,dAvg=0.0;
    AnsiString sAvg="NA";
    for(int i=0; i<sListOffTime->Count; i++)
    {
        dSum+=atof(sListOffTime->Strings[i].c_str());
        iCount++;
    }

    if(iCount>0)
        dAvg=dSum/(double)iCount;
    if(OffSenEnable && bCylPreAlarmByPassT==false)
        sAvg.sprintf("%2.3f",dAvg);
    return sAvg;
}
//---------------------------------------------------------------------------
AnsiString TMyCylinder::GetOnTimeAlarm()
{
    AnsiString sOnTimeAlarm="NA";
    if(OnSenEnable && bCylPreAlarmByPassT==false)
        sOnTimeAlarm.sprintf("%2.3f",dOnTimeAlarm);
    return sOnTimeAlarm;
}
//---------------------------------------------------------------------------
AnsiString TMyCylinder::GetOffTimeAlarm()
{
    AnsiString sOffTimeAlarm="NA";
    if(OffSenEnable && bCylPreAlarmByPassT==false)
        sOffTimeAlarm.sprintf("%2.3f",dOffTimeAlarm);
    return sOffTimeAlarm;
}
//<==
//Sam 20230516 : Pre Alrm Cylinder
//---------------------------------------------------------------------------
