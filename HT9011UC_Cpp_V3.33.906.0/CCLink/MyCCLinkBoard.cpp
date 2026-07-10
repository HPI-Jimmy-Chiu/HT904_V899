// =============================================================================
//  CCLink/MyCCLinkBoard.cpp  --  Mitsubishi CC-Link master-board driver wrapper
//
//  Translated from: HT9011UC_Code_V3.33.906.0_20260618/CCLink/MyCCLinkBoard.cpp
//  Translation wave: W5 (CCLink driver pair)
//  Translator: AI(W5-CCLink-Translate) 20260710
//
//  See MyCCLinkBoard.h for the full list of __published-property flattening /
//  fCCLink-form-coupling notes.  Key points repeated briefly here:
//    - `#pragma hdrstop` / `#pragma package(smart_init)` dropped (BCB-specific).
//    - `#include "MyCCLinkSensor.h"` (the golden TfCCLink FORM header) DROPPED --
//      it is only needed there for the SHUTTLE_SENSOR_TYPE/eSensorCCLink3
//      constants (already available via cmydef.h/MachineType.h) and for the
//      fCCLink global (whose 2 touches -- iAutoRetryCnt / iAllSetCount -- are
//      threaded through as parameters/members instead; see .h).
//    - Real vendor Mdfunc.h calls (mdOpen/mdClose/mdBdRst/mdSend/mdReceive/
//      mdDevSet/mdDevRst) are wrapped by 7 tiny static free functions gated
//      `#if HAVE_CCLINK` (default OFF; mirrors IOBackend.cpp's HAVE_MN200/
//      HAVE_PCI1203/HAVE_MNET pattern) so this file compiles+links with ZERO
//      vendor SDK.  All switch(Task) state-machine bodies below are otherwise
//      VERBATIM golden logic -- only the callee name changed (mdXxx ->
//      CCLink_mdXxx) at each vendor call site.
//    - golden `MyCCLinkBoard.cpp:815-1583` contains a large COMMENTED-OUT
//      GetCClinkErrMes(int, AnsiString[3]) vendor-error-code lookup table
//      (mdOpen/mdSend/mdReceive error code -> {title, cause, action} strings).
//      It is DEAD CODE (already fully commented out in golden) and is not
//      reproduced verbatim here to avoid ~770 lines of inert text; see golden
//      citation above for the complete table if ever needed.
// =============================================================================
#include "MyCCLinkBoard.h"

#include "cmydef.h"       // SHUTTLE_SENSOR_TYPE
#include "MachineType.h"  // eSensorCCLink3
#include <cstring>

// AI(W5-CCLink-Translate) 20260710: MySleep/MySleepEx are declared in golden
// common.h (common.h:260-261) but that declaration is currently GATED `#if 0
// // TODO(wave-timing)` in the translated common.h (see KNOWLEDGE.md: real
// bodies live in acarry_shims.cpp, ht9045_sm -- a heavy dependency this small
// W5 comms driver should not require). Declared locally instead, matching the
// same extern-forward-declare + test-local-stub pattern used for MyDBIProcess
// in MyCCLink.cpp. See report for the cross-unit symbol-conflict flag.
extern DWORD MySleepEx(DWORD dwMilliseconds, bool bAlertable); // golden common.h:260
extern void  MySleep(DWORD dwMilliseconds);                    // golden common.h:261

//---------------------------------------------------------------------------
//  Vendor CC-Link driver boundary (Mdfunc.h -- Mitsubishi CC-Link master board
//  DLL API, READ-ONLY vendor/definition header, never modified).  Gated behind
//  #if HAVE_CCLINK (default OFF), mirroring IOBackend.cpp's HAVE_MN200/
//  HAVE_PCI1203/HAVE_MNET pattern: default-OFF stubs return 0 ("success" per
//  the Result==0 check used throughout this file) so the whole driver compiles
//  and its state machines pump over an always-succeeding fake board with NO
//  vendor SDK present.
//---------------------------------------------------------------------------
#if HAVE_CCLINK
#include "Mdfunc.h"   // READ-ONLY vendor header (driver/definition file; do not modify)

static short CCLink_mdOpen(long FirstPath, short AllRelay, long *pPath)
{ return mdOpen((short)FirstPath, AllRelay, pPath); }   // NOTE: golden narrows `long lPath` to
                                                         // mdOpen's SHORT 1st param (faithful quirk)
static short CCLink_mdClose(long lPathVal)
{ return mdClose(lPathVal); }
static short CCLink_mdBdRst(long lPathVal)
{ return mdBdRst(lPathVal); }
static short CCLink_mdSend(long lPathVal, short StaNoVal, short DevType, short DevNo, short *pSize, void *pData)
{ return mdSend(lPathVal, StaNoVal, DevType, DevNo, pSize, pData); }
static short CCLink_mdReceive(long lPathVal, short StaNoVal, short DevType, short DevNo, short *pSize, void *pData)
{ return mdReceive(lPathVal, StaNoVal, DevType, DevNo, pSize, pData); }
static short CCLink_mdDevSet(long lPathVal, short StaNoVal, short DevType, short DevNo)
{ return mdDevSet(lPathVal, StaNoVal, DevType, DevNo); }
static short CCLink_mdDevRst(long lPathVal, short StaNoVal, short DevType, short DevNo)
{ return mdDevRst(lPathVal, StaNoVal, DevType, DevNo); }

#else   // HAVE_CCLINK not defined -- offline stubs, always "success" (Result==0)

static short CCLink_mdOpen(long, short, long*)                                    { return 0; }
static short CCLink_mdClose(long)                                                 { return 0; }
static short CCLink_mdBdRst(long)                                                 { return 0; }
static short CCLink_mdSend(long, short, short, short, short*, void*)              { return 0; }
// AI(W5-CCLink-Translate) 20260710: offline "receive" zeroes the caller's buffer (up to *pSize
// bytes) before reporting success -- mirrors the TSimIOBackend/TSimMotor precedent (offline
// reads return deterministic zero telemetry rather than leaving the buffer's prior/uninitialized
// contents, which would make callers like CheckBoardStatus/DoConnectBoard non-deterministic).
// This is new STUB behavior with no golden equivalent (golden always assumes a real vendor
// board fills the buffer) -- not a "fix" to any translated golden logic.
static short CCLink_mdReceive(long, short, short, short, short *pSize, void *pData)
{
    if(pData!=NULL && pSize!=NULL && *pSize>0)
        memset(pData, 0, static_cast<size_t>(*pSize));
    return 0;
}
static short CCLink_mdDevSet(long, short, short, short)                          { return 0; }
static short CCLink_mdDevRst(long, short, short, short)                          { return 0; }

#endif  // HAVE_CCLINK

//---------------------------------------------------------------------------
unsigned int MyCCMask[32]=
    {0x00000001,0x00000002,0x00000004,0x00000008,0x00000010,0x00000020,0x00000040,0x00000080,
     0x00000100,0x00000200,0x00000400,0x00000800,0x00001000,0x00002000,0x00004000,0x00008000,
     0x00010000,0x00020000,0x00040000,0x00080000,0x00100000,0x00200000,0x00400000,0x00800000,
     0x01000000,0x02000000,0x04000000,0x08000000,0x10000000,0x20000000,0x40000000,0x80000000};
//------------------------------------------------------------------------------
TMyCCLinkBoard::TMyCCLinkBoard(TComponent* Owner) : TComponent(Owner)
{
    Result      =0;                   // return value
    StaNo       =0xFF;
    NetNo       =0x00;

    iCCLinkBoardTask=1;

    SetProcessMemory();
    CmdResponseList[ 0]="\nSuccessfully completed.\n The command has been processed successfully.";
    CmdResponseList[ 1]="\nCommand number error.\n The command number is out of range.";
    CmdResponseList[ 2]="\nData category error.\n The data category value is out of range.";
    CmdResponseList[ 3]="\nID number out of range.\n A nonexisting ID number is specified.";
    CmdResponseList[ 4]="\nData number out of range.\n The data number is out of range.";
    CmdResponseList[ 5]="\nWrite inhibit.\n An attempt was made to write in a read-only data number.";
    CmdResponseList[ 6]="\nWrite data out of range.\n The value of write data is out of range.";
    CmdResponseList[ 7]="\nSensor-to-sensor communication error.\n An error occurred in the communication between sensors. Example: Noise, improper contact,etc.";
    CmdResponseList[ 8]="\nMotion command area access error. \n An attempt was made to read/write to a motion command area.";
    CmdResponseList[ 9]="\nMotion command address error. \n The address is out of range. (The address is not used with this sensor.)";
    CmdResponseList[10]="\nMotion command execution error. \n Failed to execute a motion command.";
    CmdResponseList[11]="\nRead inhibit. \n An attempt was made to read data of a write-only data number.";
}
//------------------------------------------------------------------------------
// destructor
//------------------------------------------------------------------------------
TMyCCLinkBoard::~TMyCCLinkBoard()
{
    MemoryNotEnough=false;
    MySleep(100);
}
//---------------------------------------------------------------------------
void TMyCCLinkBoard::ClearData()
{
    int i, j;
    for(i=0; i<10; i++)
    {
        for(j=0; j<16; j++)
        {
            MyNCUL[i].iCurrValue[j]=0;
            MyNCUL[i].iSetValue[j]=0;
            MyNCUL[i].bOnOff[j]=false;
        }
    }
}
//---------------------------------------------------------------------------
bool TMyCCLinkBoard::ConnectBoard()        // start the connection
{
    int iConnectErrCount=0;
    while(1)
    {
        Result=CCLink_mdOpen(lPath, -1, &lPath);   // golden: mdOpen(lPath,-1,&Path) -- Path is the
                                                    // __property alias of the SAME field lPath (BCB6
                                                    // direct-field properties are addressable);
                                                    // collapsed to the one field here (see .h note).
        if(Result==0 || Result==66)   // connect success; 66=Already open error
        {
            iConnectErrCount=0;
            bStatus=true;
            bHasErr=false;
            ErrMess="";
            MySleepEx(10, false);
            return true;
        }
        else
        {
            iConnectErrCount++;
            DisConnectBoard();           // close first, then try to reconnect
        }

        if(iConnectErrCount>5)      // stop after 5 consecutive connect failures
        {
            bHasErr=true;
            bStatus=false;
            ErrMess="CCLink Board Connect Error!";
            return false;
        }
        MySleepEx(10, false);
    }
}
//---------------------------------------------------------------------------
void TMyCCLinkBoard::DisConnectBoard()     // close the connection
{
    CCLink_mdClose(lPath);
    bStatus=false;
    MySleepEx(10, false);
}
//---------------------------------------------------------------------------
void TMyCCLinkBoard::ResetBoard()         // reset the board
{
    CCLink_mdBdRst(lPath);
    bStatus=false;
    MySleepEx(10, false);
}
//---------------------------------------------------------------------------
void TMyCCLinkBoard::CheckBoardStatus()               // check connection status
{
    int iCheck[4]={0, 0, 0, 0};
    bStatus=true;
    bHasErr=false;
    ErrMess="";
    iCheck[0]=DoCCLinkSWQuest(0x68);
    iCheck[1]=DoCCLinkSWQuest(0x69);
    iCheck[2]=DoCCLinkSWQuest(0x6A);
    iCheck[3]=DoCCLinkSWQuest(0x90);

    if(iCheck[0]!=0 || iCheck[1]!=0 || iCheck[2]!=0 || iCheck[3]!=0)
    {
        if(MemoryNotEnough==false)  // Steven 20140330: low system memory recovery
            bHasErr=true;
        bStatus=false;
        ErrMess.sprintf("Check CCLink Status Error! 0x68=%d, 0x69=%d, 0x6A=%d, 0x90=%d", iCheck[0], iCheck[1], iCheck[2], iCheck[3]);
    }
}
//---------------------------------------------------------------------------
int TMyCCLinkBoard::DoCCLinkSWQuest(int iDevNo)
{
    AnsiString str="";
    int iValue=0;
    int iDevType=0x0E;
    short sSize=2;
    MySleepEx(10, false);
    Result=CCLink_mdReceive(lPath, StaNo, iDevType, iDevNo, &sSize, Data);
    if(Result!=0)
    {
        bHasErr=true;
        if(Result==77)   // Steven 20140330: low system memory recovery
        {
            bHasErr=false;
            MemoryNotEnough=true;
        }
        iValue=-1;

        ErrMess=MyErrMes(iDevNo, Result);
    }
    else
    {
        str.sprintf("000%1hx", Data[0]);
        iValue=CCLinkHexStrToInt(str);
        bHasErr=false;
        ErrMess="";
    }
    return iValue;
}
//------------------------------------------------------------------------------
AnsiString TMyCCLinkBoard::MyErrMes(int iDevNo, int iResult)
{
    AnsiString str="";
    if(iResult!=0)
    {
        switch(iDevNo)
        {
            case 0x68:  str.sprintf("Get Own Station Parameter Status Error!! Code=%d", iResult);    break;
            case 0x69:  str.sprintf("Get the loading status Error!! Code=%d", iResult);              break;
            case 0x6A:  str.sprintf("Get Station number duplicate check Error!! Code=%d", iResult);  break;
            case 0x71:  str.sprintf("Get Number of Modules Error!! Code=%d", iResult);               break;
            case 0x72:  str.sprintf("Get Number of Stations Error!! Code=%d", iResult);              break;
            case 0x90:  str.sprintf("Get Cables Status Error!! Code=%d", iResult);                   break;
        }
    }
    else
    {
        switch(iDevNo)
        {
            case 0x68:  str.sprintf("Own Station Parameter Status Error!!");    break;
            case 0x69:  str.sprintf("Check the loading status!!");              break;
            case 0x6A:  str.sprintf("Station number duplicate Error!!");        break;
            case 0x71:  str.sprintf("Module Connection Error!!");               break;
            case 0x72:  str.sprintf("Stations Number Error!!");                 break;
            case 0x90:  str.sprintf("Cables Status Error!!");                   break;
        }
    }
    return str;
}
//------------------------------------------------------------------------------
void TMyCCLinkBoard::SetProcessMemory() // Steven 20140330: low system memory recovery
{
    DWORD dwProcessId; /*User program ID variable*/
    HANDLE hProcess; /*User program handle variable*/
    DWORD dwMinimumWorkingSetSize; /*Minimum working set variable*/
    DWORD dwMaximumWorkingSetSize; /*Maximum working set variable*/

    /*Obtain the user program ID*/
    dwProcessId=GetCurrentProcessId();

    /*Open the user program handle*/
    hProcess=OpenProcess(PROCESS_SET_QUOTA+PROCESS_QUERY_INFORMATION, false, dwProcessId);
    if(hProcess==NULL)
    {
        /*Error end*/
        return;
    }
    /*Obtain the maximum working set size and minimum working set size of the user program */
    if(GetProcessWorkingSetSize(hProcess, &dwMinimumWorkingSetSize, &dwMaximumWorkingSetSize)==0)
    {
        /*Error end*/
        CloseHandle(hProcess);
        return;
    }

    /*Set the minimum working set size to 1MB*/
    dwMinimumWorkingSetSize = 1 * 1024 * 1024;
    /*Set the maximum working set size to 3MB*/
    dwMaximumWorkingSetSize = 3 * 1024 * 1024;

    /*Change the maximum working set size and minimum working set size of the user program */
    if(SetProcessWorkingSetSize(hProcess, dwMinimumWorkingSetSize, dwMaximumWorkingSetSize)==0)
    {
        /*Error end*/
        CloseHandle(hProcess);
        return;
    }

    /*Close the user program handle*/
    CloseHandle(hProcess);
}
//------------------------------------------------------------------------------
bool TMyCCLinkBoard::DoConnectBoard(int &AutoRetryCnt)
{
    int &Task=iCCLinkBoardTask, i;
    static int iErrCount=0;
    static int iResetCount=0;

    switch(Task)
    {
        case 1:
            if(ConnectBoard())     // connect
            {
                iResetCount=0;
                bHasErr=false;
                AutoRetryCnt=0;    // JerryYang 20160729: connect succeeded (golden: fCCLink->iAutoRetryCnt=0)
                Task=100;
            }
            else
            {
                AutoRetryCnt++;    // JerryYang 20160729: CC-Link connect-failure count (golden: fCCLink->iAutoRetryCnt++)
                Task=50;
            }
            break;
        case 50:
            if(iResetCount<=5)      // reset the board
            {
                ResetBoard();
                iResetCount++;
                Task=1;
            }
            else
            {
                iResetCount=0;
                iErrCount=0;
                bHasErr=true;
                ErrMess="CCLink Card Reset Error!!";
                return false;
            }
            break;
        case 100:
            CheckBoardStatus();
            if(bStatus==true)     // check connection status
            {
                bHasErr=false;
                iErrCount=0;
                Task=200;
            }
            else
            {
                if(MemoryNotEnough)     // Steven 20140330: low system memory recovery
                {
                    SetProcessMemory();
                    MemoryNotEnough=false;
                }
                iErrCount++;
            }

            if(iErrCount>5)
            {
                iErrCount=0;
                Task=1;
            }
            break;
        case 200:
            iConnectModule=DoCCLinkSWQuest(0x72);          // get the currently connected module count
            if(iConnectModule>0)
            {
                for(i=0; i<10; i++)
                {
                    if(i<=iConnectModule)
                    {
                        MyNCUL[i].bHasInstall=true;
                        MyNCUL[i].wwStart=0x18*i;
                        MyNCUL[i].ryStart=0xA0*i;
                        MyNCUL[i].iSensorNo=GetSensorNo(i);
                        /*if(iSensorNo<=0)  //to be continued
                        {
                            bHasErr=true;
                            ErrMess.sprintf("Path %d - Unit%d - Sensor No = %d Error!!", Path, ID, iSensorNo);
                            return false;
                        } */
                    }
                    else
                        MyNCUL[i].bHasInstall=false;   //.InitUnit(false, -1, lPath);
                }
                bHasErr=false;
                ErrMess="";
                Task=300;
                return true;
            }
            else
            {
                bHasErr=true;
                ErrMess.sprintf("The Number of Modules Connection Error!! No.=%d", iConnectModule);
                Task=1;
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
void TMyCCLinkBoard::SetSensorKeyLock(bool bLock)    // key lock, to be continued
{
    /*bool flag=false;
    int i;
    for(int i=0; i<iConnectModule; i++)
    {
        flag=MyNCUL[i].SetKeyLock(bLock);
        if(flag==false)
        {
            bHasErr=MyNCUL[i].HasErr();
            ErrMess=MyNCUL[i].GetErrMess();
            if(bHasErr==true)
                bKeyLock=false;
        }
    }  */
    bKeyLock=bLock;
}
//------------------------------------------------------------------------------
int  TMyCCLinkBoard::GetSensorNo(int Value) // sensor count, to be continued
{
    //iSensorNo=20;  kevin 20130420: one module has 16 sensors, 3 modules 48/16=3   i < 48+1
    iSensorNo=49;   // AI(W5-CCLink-Translate) 20260710: dead write -- see .h note (class member
                     // `iSensorNo` vs. struct member `MyCCLinkNCUL1::iSensorNo` name collision);
                     // preserved verbatim, the meaningful value comes from this function's RETURN.
    if(Value==0)
    {
        if(SHUTTLE_SENSOR_TYPE==eSensorCCLink3)    // Steven 20131008: for HT9046AH
            return 12;
        else
            return 16;
    }
    else if(Value==1)
    {
        return 4;
    }
    else if(Value==2)//kevin 20130420 8sensor
    {
        return 9;
    }
    else return 0;
}
//------------------------------------------------------------------------------
bool TMyCCLinkBoard::DoReadOnOff()
{
    int i, j, DevType, DevNo;
    short sSize=2, sValue;
    if(iConnectModule<1)
        return false;
    AnsiString str;
    for(i=0; i<iConnectModule; i++)
    {
        DevType=0x25;
        DevNo=MyNCUL[i].wwStart+0x03;
        for(j=0; j<sSize; j++)
            iData[j]='\0';

        Result=CCLink_mdReceive(lPath, StaNo, DevType, DevNo, &sSize, iData);
        if(Result!=0)
        {
            bHasErr=true;
            ErrMess.sprintf("DoReadOnOff Error! %d", Result);
            return false;
        }
        else
        {
            bHasErr=false;
            ErrMess="";
            str.sprintf("%02hx%02hx", iData[1], iData[0]);
            sValue=CCLinkHexStrToInt(str);
        }

        for(j=0; j<16; j++)
            MyNCUL[i].bOnOff[j]=!(sValue&MyCCMask[j]);
    }
    return true;
}
//------------------------------------------------------------------------------
bool TMyCCLinkBoard::DoReadCurrValue()
{
    int i, j, DevType, DevNo;

    short sSize=32;    // one station controls 16 points, each point occupies 2 bytes
    AnsiString str;

    if(iConnectModule<1)
        return false;

    for(i=0; i<iConnectModule; i++)
    {
        DevType=0x25;
        DevNo=MyNCUL[i].wwStart+0x04;
        for(j=0; j<sSize; j++)
            iData[j]='\0';

        Result=CCLink_mdReceive(lPath, StaNo, DevType, DevNo, &sSize, iData);
        if(Result!=0)
        {
            bHasErr=true;
            ErrMess.sprintf("DoReadCurrValue Error! %d", Result);
            return false;
        }
        else
        {
            bHasErr=false;
            ErrMess="";
            for(j=0; j<sSize; j+=2)
            {
                str.sprintf("%02hx%02hx", iData[1+j], iData[0+j]);
                MyNCUL[i].iCurrValue[j/2]=CCLinkHexStrToInt(str);
            }
        }
    }
    return true;
}
//---------------------------------------------------------------------------
bool TMyCCLinkBoard::DoCheckWr0000()
{
    int i, DevNo, iTemp=0, DevType=0x25;
    short sSize=2;
    AnsiString str="";

    for(i=0; i<iConnectModule; i++)
    {
        DevNo=MyNCUL[i].wwStart+0x00;

        iData[0]='\0';
        iData[1]='\0';
        Result=CCLink_mdReceive(lPath, StaNo, DevType, DevNo, &sSize, iData);

        if(Result!=0)
        {
            bHasErr=true;
            ErrMess.sprintf("DoCheckWr0000 Error! %d", Result);
            return Result;
        }

        str.sprintf("%02hx%02hx", iData[1], iData[0]);
        iTemp=CCLinkHexStrToInt(str);
        if(iTemp!=0)
        {
            bHasErr=true;
            ErrMess.sprintf("NU-CL1-%d Wr0000 != 0 Error!! Code:%d %s", i, iTemp, CmdResponseList[iTemp]);
            return true;
        }
        bHasErr=false;
        ErrMess="";
    }
    return true;
}
//------------------------------------------------------------------------------
bool TMyCCLinkBoard::DoReadSetValue()
{
    static short sNu=0;             // NU-CL1 module number
    static short sSen=0;            // sensor number
    short sWwDevNo=0;               // position to write
    short sValue=0;                 // value to write
    static int Index=0;
    int iDelayTime=1;

    int &Task=iReadSetValueTask;

    switch(Task)
    {
        case 1:
            Index=0;
            Task=2;
            break;
        case 2:
            sNu=Index/16;
            sSen=Index%16;
            sWwDevNo=0x00;                  // position of Ww0
            sValue=0x0000;                  // value to write
            DoRYOnOff(sNu, 0x10, false); // turn off Y10
            DoWriteWW(sNu, sWwDevNo, sValue);
            if(bHasErr==false)
            {
                CCLinkDelay.Set0_1SecAndOn(iDelayTime);
                Task=3;
            }
            else
                return false;
            break;
        case 3:
            if(CCLinkDelay.Off())
            {
                sWwDevNo=0x01;                  // position of Ww1
                sValue=0x0041+sSen*0x100+0x100; // value to write (Sensor ID=1~16*0x100 + 0x0041)
                DoWriteWW(sNu, sWwDevNo, sValue);
                if(bHasErr==false)
                {
                    CCLinkDelay.Set0_1SecAndOn(iDelayTime);
                    Task=4;
                }
                else
                    return false;
            }
            break;
        case 4:
            if(CCLinkDelay.Off())
            {
                DoRYOnOff(sNu, 0x10, true); // turn on Y10
                if(bHasErr==false)
                {
                    CCLinkDelay.Set0_1SecAndOn(iDelayTime);
                    Task=5;
                }
                else
                {
                    return false;
                }
            }
            break;
        case 5:
            if(CCLinkDelay.Off())
            {
                MyNCUL[sNu].iSetValue[sSen]=DoReadWr02(sNu);    // read the Wr02 data
                if(bHasErr==false)
                {
                    CCLinkDelay.Set0_1SecAndOn(iDelayTime);
                    Task=6;
                }
                else
                {
                    MyNCUL[sNu].iSetValue[sSen]=0;
                    return false;
                }
            }
            break;
        case 6:
            if(CCLinkDelay.Off())
            {
                DoRYOnOff(sNu, 0x10, false); // turn off Y10
                if(bHasErr==false)
                    Task=7;
                else
                    return false;
            }
            break;
        case 7:
            Index++;
            if(Index<iSensorNo)
            {
                Task=2;
                break;
            }
            else
            {
                Task=1;
                return true;
            }
    }
    return false;
}
//------------------------------------------------------------------------------
bool TMyCCLinkBoard::DoSetValue()    // set a single sensor's threshold
{
    static short sWwDevNo;                // position Ww04+SensorID
    static short sRyNo;    // position Ry50+SensorID
    int iDelayTime=1;

    int &Task=iDoSetValueTask;
    switch(Task)
    {
        case 1:
            sWwDevNo=0x04+sSetSen;
            sRyNo=0x50+sSetSen;
            DoRYOnOff(sSetNu, sRyNo, false);        // turn off Y50+SensorID
            DoWriteWW(sSetNu, sWwDevNo, iSetValue);
            if(bHasErr==false)
            {
                CCLinkDelay.Set0_1SecAndOn(iDelayTime);
                Task=3;
            }
            else
            {
                return false;
            }
            break;
        case 3:
            if(CCLinkDelay.Off())
            {
                DoRYOnOff(sSetNu, sRyNo, true); // turn on Y50+SensorID
                if(bHasErr==false)
                {
//                    CCLinkDelay.Clear();
                    CCLinkDelay.Set0_1SecAndOn(iDelayTime);
//                    CCLinkDelay.On();
                    Task=4;
                }
                else
                    return false;
            }
            break;
        case 4:
            if(CCLinkDelay.Off())
            {
                DoRYOnOff(sSetNu, sRyNo, false); // turn off Y50+SensorID
                if(bHasErr==false)
                {
                    Task=5;
                    MyNCUL[sSetNu].iSetValue[sSetSen]=iSetValue;
                    return true;
                }
                else
                    return false;
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
bool TMyCCLinkBoard::DoSetMultiValue()    // set a single sensor's threshold
{
    static short sWwDevNo;                // position Ww04+SensorID
    static short sRyNo;    // position Ry50+SensorID
    static int iCount=0;
    int iDelayTime=1;

    int &Task=iDoSetMultiValueTask;
    switch(Task)
    {
        case 1:
            iCount=0;
            Task=2;
        case 2:
            sWwDevNo=0x04+sSetSen-iCount;
            sRyNo=0x50+sSetSen-iCount;
            DoRYOnOff(sSetNu, sRyNo, false);        // turn off Y50+SensorID
            DoWriteWW(sSetNu, sWwDevNo, iSetMultiValue[iCount]);
            if(bHasErr==false)
            {
                CCLinkDelay.Set0_1SecAndOn(iDelayTime);
                Task=3;
            }
            else
            {
                return false;
            }
            break;
        case 3:
            if(CCLinkDelay.Off())
            {
                DoRYOnOff(sSetNu, sRyNo, true); // turn on Y50+SensorID
                if(bHasErr==false)
                {
//                    CCLinkDelay.Clear();
                    CCLinkDelay.Set0_1SecAndOn(iDelayTime);
//                    CCLinkDelay.On();
                    Task=4;
                }
                else
                    return false;
            }
            break;
        case 4:
            if(CCLinkDelay.Off())
            {
                DoRYOnOff(sSetNu, sRyNo, false); // turn off Y50+SensorID
                if(bHasErr==false)
                {
                    CCLinkDelay.Set0_1SecAndOn(iDelayTime);
                    Task=5;
                    MyNCUL[sSetNu].iSetValue[sSetSen]=iSetValue;
                }
                else
                {
                    return false;
                }
            }
        case 5:
            if(CCLinkDelay.Off())
            {
                iCount++;
                if(iCount<iAllSetCount)   // AI(W5-CCLink-Translate) 20260710: golden reads
                                          // fCCLink->iAllSetCount here (deferred TfCCLink form
                                          // field); see .h class-header note + SetAllSetCount().
                {
                    Task=2;
                }
                else
                {
                    Task=6;
                    return true;
                }
            }
            break;
    }
    return false;
}
//------------------------------------------------------------------------------
bool TMyCCLinkBoard::DoRYOnOff(int Index, int iRyDevNo, bool bOnOff)
{
    int DevNo, DevType=0x02;
    if(Index>=iConnectModule)
        return false;

    DevNo=MyNCUL[Index].ryStart+iRyDevNo;

    if(bOnOff)
        Result=CCLink_mdDevSet(lPath, StaNo, DevType, DevNo);    // Sets a bit device.
    else
        Result=CCLink_mdDevRst(lPath, StaNo, DevType, DevNo);    // Resets a bit device.

    if(Result!=0)
    {
        bHasErr=true;
        ErrMess.sprintf("DoRYOnOff Error! %d", Result);
        return false;
    }
    bHasErr=false;
    ErrMess="";
    return true;
}
//---------------------------------------------------------------------------
bool TMyCCLinkBoard::DoWriteWW(int Index, short sWwDevNo, short Value)
{
    int DevNo, DevType=0x24;
    short sSize=2;
    if(Index>=iConnectModule)
        return false;

    DevNo=MyNCUL[Index].wwStart+sWwDevNo;

    iData[0]=Value%256;
    iData[1]=Value/256;
    Result=CCLink_mdSend(lPath, StaNo, DevType, DevNo, &sSize, iData);  // Performs batch write of devices.

    if(Result!=0)
    {
        bHasErr=true;
        ErrMess.sprintf("DoWriteWW Error! %d", Result);
        return false;
    }
    bHasErr=false;
    ErrMess="";
    return true;
}
//---------------------------------------------------------------------------
int TMyCCLinkBoard::DoReadWr02(int Index)
{
    int DevNo, iTemp=0, DevType=0x25;
    short sSize=2;
    AnsiString str="";
    if(Index>=iConnectModule)
        return -1;

    DevNo=MyNCUL[Index].wwStart+0x02;

    iData[0]='\0';
    iData[1]='\0';
    Result=CCLink_mdReceive(lPath, StaNo, DevType, DevNo, &sSize, iData);

    if(Result!=0)
    {
        bHasErr=true;
        ErrMess.sprintf("DoReadWr02 Error! %d", Result);
        return Result;
    }

    str.sprintf("%02hx%02hx", iData[1], iData[0]);
    iTemp=CCLinkHexStrToInt(str);
    bHasErr=false;
    ErrMess="";
    return iTemp;
}
//---------------------------------------------------------------------------
int TMyCCLinkBoard::CCLinkHexStrToInt(AnsiString str)
{
    int i, sum=0, Scale=1, j;
    char c;
    if(str.Length()==0)  return 0;

    str=str.UpperCase();
    sum=0;
    for(i=3; i>=0; i--)
    {
        c=str.c_str()[i];
        if((c>='0' && c<='9'))
            j=(int)(c-'0');
        else if((c>='A' && c<='F'))
            j=10+(int)(c-'A');
        else
            break;
        sum+=j*Scale;
        Scale*=16;
    }
    return sum;
}
//------------------------------------------------------------------------------
// AI(W5-CCLink-Translate) 20260710: golden .cpp:815-1583 -- a commented-out
// GetCClinkErrMes(int ErrCode, AnsiString str[3]) vendor error-code lookup
// table (mdOpen/mdSend/mdReceive numeric error -> {title, cause, action}
// strings, ~150 branches). DEAD CODE in golden (fully commented out there
// too); not reproduced verbatim here. See golden citation above for the
// complete text if ever needed.
