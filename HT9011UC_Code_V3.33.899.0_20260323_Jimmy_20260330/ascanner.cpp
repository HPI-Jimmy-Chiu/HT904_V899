#include "MachineDefine.h"
#pragma hdrstop

#include "ascanner.h"
//#include "cmydef.h"
#include "MyLaneIo.h"
//#include "htimer.h"
#include "mymessbox.h"
#include "cprod.h"
#include "main.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

int iScanResult=-1;
bool bScanLightOn=false;
bool bScannerAlarm=false;
bool bScanInspectOn=false;

#define ICOS

union
{
    int PortData;
    struct {
            bool REQ:1;     // Bit 0 REQ is for Scanner   IRQ is for ICOS
            bool INP:1;     // Bit 1
            bool NA1:1;     // Bit 2
            bool NA2:1;     // Bit 3
            bool CMD1:1;    // Bit 4 CMD is for Scanner
            bool CMD2:1;    // Bit 5 ID  is for ICOS
            bool CMD3:1;    // Bit 6
            bool CMD4:1;    // Bit 7
    }Bit;
}ScanOutA;

union
{
    int PortData;
    struct{
        bool DATA1:1;       // Bit 8
        bool DATA2:1;       // Bit 9
        bool DATA3:1;       // Bit 10
        bool DATA4:1;       // Bit 11
        bool DATA5:1;       // Bit 12
        bool DATA6:1;       // Bit 13
        bool DATA7:1;       // Bit 14
        bool DATA8:1;       // Bit 15
    }Bit;
}ScanOutB;
union
{
    int PortData;
    struct
    {
        bool BUSY:1;        // Bit 0
        bool ACK:1;         // Bit 1
        bool ERR:1;         // Bit 2
        bool ERR1:1;        // Bit 3
        bool ERR2:1;        // Bit 4
        bool ERR3:1;        // Bit 5
        bool MODE:1;        // Bit 6
        bool NA:1;          // Bit 7
    }Bit;
}ScanInA;
union
{
    int PortData;
    struct
    {
        bool RESULT1:1;     // Bit 8     RES 1
        bool RESULT2:1;     // Bit 9     RES 2
        bool NA3:1;         // Bit 10    RES 3
        bool NA4:1;         // Bit 11    RES 4
        bool NA5:1;         // Bit 12
        bool NA6:1;         // Bit 13
        bool NA7:1;         // Bit 14
        bool NA8:1;         // Bit 15
    }Bit;
}ScanInB;

const int BaseAddr=0x2a8;
const int S_ON=0;
const int S_OFF=1;
HTimer HScanDelay;

void ScannerClearPort()
{
   /* ScanOutA.PortData=0xff;
    IOByteOut(BaseAddr,ScanOutA.PortData);
    ScanOutB.PortData=0xff;
    IOByteOut(BaseAddr+1,ScanOutB.PortData);*/
}
void ScannerLightOn()
{
    ScanOutA.Bit.CMD1=S_OFF;                                              // on light command
    ScanOutA.Bit.CMD2=S_ON;
    ScanOutA.Bit.CMD3=S_ON;
    ScanOutA.Bit.CMD4=S_ON;
   // IOByteOut(BaseAddr,ScanOutA.PortData);
}
void ScannerStartInspect()
{
    #ifdef ICOS
    ScanOutA.Bit.CMD1=S_ON;                                              // on light command
    ScanOutA.Bit.CMD2=S_OFF;
    ScanOutA.Bit.CMD3=S_OFF;
    ScanOutA.Bit.CMD4=S_OFF;
    #else
    ScanOutA.Bit.CMD1=S_OFF;                                              // on light command
    ScanOutA.Bit.CMD2=S_ON;
    ScanOutA.Bit.CMD3=S_ON;
    ScanOutA.Bit.CMD4=S_ON;
    #endif
    //IOByteOut(BaseAddr,ScanOutA.PortData);
}
void ScannerStartRequest()
{
    ScanOutA.Bit.REQ=S_ON;
   // IOByteOut(BaseAddr,ScanOutA.PortData);
}
void ScannerEndRequest()
{
    ScanOutA.Bit.REQ=S_OFF;
    //IOByteOut(BaseAddr,ScanOutA.PortData);
}
bool ScannerWaitAckOn()
{
    #ifdef SCAN_SIMULTE
        return true;
    #else
        if(LastSet.iScannerSelect==2)
        {
            //ScanInA.PortData=IOInputByte(BaseAddr);
            if(  ScanInA.Bit.ACK==S_ON )                                         // wait ack
                return true;
            else
                return false;
        }
        else
        {
           // ScanInA.PortData=IOInputByte(BaseAddr);
            if(  ScanInA.Bit.ACK==S_ON )                                         // wait ack
                return true;
            else
                return false;
        }
    #endif
}
bool ScannerWaitAckOff()
{
    #ifdef SCAN_SIMULTE
        return true;
    #else
        if(LastSet.iScannerSelect==2)
        {
           // ScanInA.PortData=IOInputByte(BaseAddr);
            if(  ScanInA.Bit.ACK==S_OFF )                                         // wait ack
                return true;
            else
                return false;
        }
        else
        {
            //ScanInA.PortData=IOInputByte(BaseAddr);
            if(  ScanInA.Bit.ACK==S_OFF )                                         // wait ack
                return true;
            else
                return false;

        }
    #endif
}
bool ScannerBusyOff()
{
    #ifdef SCAN_SIMULTE
        return true;
    #else
        if(LastSet.iScannerSelect==2)
        {
           // ScanInA.PortData=IOInputByte(BaseAddr);
            if(  ScanInA.Bit.BUSY==S_OFF )                                         // wait ack
                return true;
            else
                return false;

        }
        else
        {
            //ScanInA.PortData=IOInputByte(BaseAddr);
            if(  ScanInA.Bit.BUSY==S_OFF )                                         // wait ack
                return true;
            else
                return false;
        }
    #endif
}
int GetScannerResult()
{
    #ifdef SCAN_SIMULTE
        if( random(5))
            return 3;
        return random(4);
    #else
        if(LastSet.iScannerSelect==2)
        {
           // ScanInB.PortData=IOInputByte(BaseAddr+1);
            if(ScanInB.Bit.RESULT1==true && ScanInB.Bit.RESULT2==true)       // Invalid
                return 0;
            else if(ScanInB.Bit.RESULT1==true && ScanInB.Bit.RESULT2==false) // Failure
                return 1;
            else if(ScanInB.Bit.RESULT1==false && ScanInB .Bit.RESULT2==true) // Rework
                return 2;
            else
                return 3;
        }
        else
        {
            //ScanInB.PortData=IOInputByte(BaseAddr+1);
            if(ScanInB.Bit.RESULT1==true && ScanInB.Bit.RESULT2==true)       // Invalid
                return 0;
            else if(ScanInB.Bit.RESULT1==true && ScanInB.Bit.RESULT2==false) // Failure
                return 1;
            else if(ScanInB.Bit.RESULT1==false && ScanInB .Bit.RESULT2==true) // Rework
                return 2;
            else
                return 3;
        }
    #endif
}

#define SCAN_LIGHT_ON 0
#define SCAN_INSPECT  1
int iScanTask=1;
HTimer HScanTimer;
bool DoScanCommand(int Command)
{
    int &Task=iScanTask;
    //char str[4][10]={{"Invalid"},{"Failure"},{"Rework"},{"Pass"}};
    //static int ct=0;
    switch(Task)
    {
        case 1:
            if(Command==SCAN_LIGHT_ON)
                ScannerLightOn();
            else if(Command==SCAN_INSPECT)
            {
               // fMain->palScanner->Caption="";
   /*             if(LastSet.iScannerSelect==3)//Eliot 20070717
                {
                    if(fMain->SendMessageToScan()==false)
                    {
                        bScannerAlarm=true;
                        return true;
                    }
                }
                else
                    ScannerStartInspect();*/
            }
            HScanTimer.SetMS(3000);
            HScanTimer.On();
            if(LastSet.iScannerSelect==3)
                Task=300;
            else
            {
                ScannerStartRequest();
                Task=100;
            }
            break;
        case 100:
            if(ScannerWaitAckOn())                                         // wait ack
            {
                ScannerEndRequest();
                if(Command==SCAN_LIGHT_ON)
                {
                    Task=1;
                    return false;
                }
                HScanTimer.SetMS(3000);
                HScanTimer.On();
                Task=200;
                //ct=0;
                break;
            }
            if( HScanTimer.Off())
            {
                ShowMyMessage("Scanner no response!", "掃瞄器無回應!!");
                ScannerEndRequest();
                if(Command==SCAN_LIGHT_ON)
                {
                    Task=1;
                    bScannerAlarm=true;
                    return false;
                }
                HScanTimer.SetMS(3000);
                HScanTimer.On();
                Task=200;
                //ct=0;
                break;
            }
            break;
        case 200:
            if(ScannerWaitAckOff() && ScannerBusyOff())                                         // wait ack
            {
                if(Command==SCAN_INSPECT)
                {
                    HScanTimer.SetMS(0);
                    HScanTimer.On();
                    Task=210;
                    return true;
                }
                Task=1;
                return false;
            }
            if( HScanTimer.Off())
            {
                ShowMyMessage("Scanner no response!", "掃瞄器無回應!!");
                bScannerAlarm=true;
                if(Command==SCAN_INSPECT)
                    iScanResult=0;
                Task=1;
                return false;
            }
            break;
        case 210:
            if( HScanTimer.Off())
            {
                iScanResult=GetScannerResult();
//                fMain->palScanner->Caption=str[iScanResult];
                Task=1;
                return false;
            }
            break;
        case 300:
            if(fMain->iScannerResult!=99)
            {
                iScanResult=fMain->iScannerResult;
 //               fMain->palScanner->Caption=str[iScanResult];
                Task=1;
                return false;
            }
            if( HScanTimer.Off())
            {
                ShowMyMessage("Scanner no response!", "掃瞄器無回應!!");
                bScannerAlarm=true;
                iScanResult=2;
//                fMain->palScanner->Caption=str[iScanResult];
                Task=1;
                return false;
            }
            break;
    }

    return true;
}
void DoScan()
{
    if(bScanLightOn )
    {
        bScanLightOn=false;
        if(LastSet.iScannerSelect==2)
            return;
        bScanLightOn=DoScanCommand( SCAN_LIGHT_ON );
    }
    else if(bScanInspectOn)
        bScanInspectOn=DoScanCommand( SCAN_INSPECT );
    else
        iScanTask=1;
}
void ScannerTest()
{
    static int Task=1;
    switch (Task)
    {
        case 1:
            ScannerClearPort();
            HScanDelay.SetMS(1000);
            HScanDelay.On();
            Task=50;
            break;
        case 50:
            if(HScanDelay.Off())
                Task=100;
            break;
        case 100:
            ScannerLightOn();
            ScannerStartRequest();
            Task=200;
            break;
        case 200:
            if(ScannerWaitAckOn())                                         // wait ack
            {
                ScannerEndRequest();
                HScanDelay.SetMS(400);
                HScanDelay.On();
                Task=250;
                break;
            }
            break;
        case 250:
            if( HScanDelay.Off())
                Task=300;
            break;
        case 300:
            ScannerStartInspect();
            ScannerStartRequest();
            Task=400;
            break;
        case 400:
            if(ScannerWaitAckOn())                                         // wait ack
            {
                ScannerEndRequest();
                HScanDelay.SetMS(2000);
                HScanDelay.On();
                Task=50;
            }
            break;
    }
}
