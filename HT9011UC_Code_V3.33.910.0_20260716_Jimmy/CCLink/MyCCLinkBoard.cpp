#include "MachineDefine.h"
#pragma hdrstop

#include "MyCCLinkBoard.h"

#include "cMyDef.h"
#include "MyCCLinkSensor.h"
#include "common.h"

#pragma package(smart_init)
unsigned int MyCCMask[32]=
    {0x00000001,0x00000002,0x00000004,0x00000008,0x00000010,0x00000020,0x00000040,0x00000080,
     0x00000100,0x00000200,0x00000400,0x00000800,0x00001000,0x00002000,0x00004000,0x00008000,
     0x00010000,0x00020000,0x00040000,0x00080000,0x00100000,0x00200000,0x00400000,0x00800000,
     0x01000000,0x02000000,0x04000000,0x08000000,0x10000000,0x20000000,0x40000000,0x80000000};
//------------------------------------------------------------------------------
__fastcall TMyCCLinkBoard::TMyCCLinkBoard(TComponent* Owner) : TComponent(Owner)
{
    Result      =0;                   //回傳值
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
};
//------------------------------------------------------------------------------
// 解構子
//------------------------------------------------------------------------------
__fastcall TMyCCLinkBoard::~TMyCCLinkBoard()
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
bool TMyCCLinkBoard::ConnectBoard()        //開始連線
{
    int iConnectErrCount=0;
    while(1)
    {
        Result=mdOpen(lPath, -1, &Path);
        if(Result==0 || Result==66)   //連線成功; 66=Already open error
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
            DisConnectBoard();           //先關連線，然後重新連一次看看
        }

        if(iConnectErrCount>5)      //連線失敗超過五次就停止
        {
            bHasErr=true;
            bStatus=false;
            ErrMess="CCLink Board Connect Error!";
            return false;
        }
        MySleepEx(10, false);
    }
};
//---------------------------------------------------------------------------
void TMyCCLinkBoard::DisConnectBoard()     //結束連線
{
    mdClose(lPath);
    bStatus=false;
    MySleepEx(10, false);
};
//---------------------------------------------------------------------------
void TMyCCLinkBoard::ResetBoard()         //重設卡片
{
    mdBdRst(lPath);
    bStatus=false;
    MySleepEx(10, false);
};
//---------------------------------------------------------------------------
void __fastcall TMyCCLinkBoard::CheckBoardStatus()               //檢查連線狀態
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
        if(MemoryNotEnough==false)  //Steven 20140330 : 記憶體不足處理
            bHasErr=true;
        bStatus=false;
        ErrMess.sprintf("Check CCLink Status Error! 0x68=%d, 0x69=%d, 0x6A=%d, 0x90=%d", iCheck[0], iCheck[1], iCheck[2], iCheck[3]);
    }
};
//---------------------------------------------------------------------------
int TMyCCLinkBoard::DoCCLinkSWQuest(int iDevNo)
{
    AnsiString str="";
    int iValue=0;
    int iDevType=0x0E;
    short sSize=2;
    MySleepEx(10, false);
    Result=mdReceive(lPath, StaNo, iDevType, iDevNo, &sSize, Data);
    if(Result!=0)
    {
        bHasErr=true;
        if(Result==77)   //Steven 20140330 : 記憶體不足處理
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
};
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
};
//------------------------------------------------------------------------------
void TMyCCLinkBoard::SetProcessMemory() //Steven 20140330 : 記憶體不足處理
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
bool TMyCCLinkBoard::DoConnectBoard()
{
    int &Task=iCCLinkBoardTask, i;
    static int iErrCount=0;
    static int iResetCount=0;

    switch(Task)
    {
        case 1:
            if(ConnectBoard())     //連線
            {
                iResetCount=0;
                bHasErr=false;
                fCCLink->iAutoRetryCnt=0;    //JerryYang 20160729 連線成功
                Task=100;
            }
            else
            {
                fCCLink->iAutoRetryCnt++;    //JerryYang 20160729 CC-Link連線失敗次數計算
                Task=50;
            }
            break;
        case 50:
            if(iResetCount<=5)      //重設卡片
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
            if(bStatus==true)     //檢查連線狀態
            {
                bHasErr=false;
                iErrCount=0;
                Task=200;
            }
            else
            {
                if(MemoryNotEnough)     //Steven 20140330 : 記憶體不足處理
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
            iConnectModule=DoCCLinkSWQuest(0x72);          //取得目前連接的模組數
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
                        /*if(iSensorNo<=0)  //未完待續
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
};
//------------------------------------------------------------------------------
void __fastcall TMyCCLinkBoard::SetSensorKeyLock(bool bLock)    //按鍵鎖定，未完待續
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
};
//------------------------------------------------------------------------------
int  TMyCCLinkBoard::GetSensorNo(int Value) //Sensor個數，未完待續
{
    //iSensorNo=20;  kevin 20130420 一個模組16個 sensor 3個模組 48/16=3   i < 48+1
    iSensorNo=49;
    if(Value==0)
    {
        if(SHUTTLE_SENSOR_TYPE==eSensorCCLink3)    //Steven 20131008 : for HT9046AH
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
};
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

        Result=mdReceive(lPath, StaNo, DevType, DevNo, &sSize, iData);
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

    short sSize=32;    //一個站控制16個點，每個點佔用兩個空間
    AnsiString str;

    if(iConnectModule<1)
        return false;

    for(i=0; i<iConnectModule; i++)
    {
        DevType=0x25;
        DevNo=MyNCUL[i].wwStart+0x04;
        for(j=0; j<sSize; j++)
            iData[j]='\0';

        Result=mdReceive(lPath, StaNo, DevType, DevNo, &sSize, iData);
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
};
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
        Result=mdReceive(lPath, StaNo, DevType, DevNo, &sSize, iData);

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
    static short sNu=0;             //NU-CL1的號碼
    static short sSen=0;            //Sensor的號碼
    short sWwDevNo=0;               //要寫入的位置
    short sValue=0;                 //要寫入的值
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
            sWwDevNo=0x00;                  //寫入Ww0的位置
            sValue=0x0000;                  //寫入的值
            DoRYOnOff(sNu, 0x10, false); //關掉Y10
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
                sWwDevNo=0x01;                  //寫入Ww1的位置
                sValue=0x0041+sSen*0x100+0x100; //寫入的值 (Sensor ID=1~16*0x100 + 0x0041)
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
                DoRYOnOff(sNu, 0x10, true); //打開Y10
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
                MyNCUL[sNu].iSetValue[sSen]=DoReadWr02(sNu);    //讀取Wr02的資料
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
                DoRYOnOff(sNu, 0x10, false); //關掉Y10
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
};
//------------------------------------------------------------------------------
bool TMyCCLinkBoard::DoSetValue()    //針對單一Sensor設定值
{
    static short sWwDevNo;                //寫入Ww04+SensorID的位置
    static short sRyNo;    //Ry50+SensorID的位置
    int iDelayTime=1;

    int &Task=iDoSetValueTask;
    switch(Task)
    {
        case 1:
            sWwDevNo=0x04+sSetSen;
            sRyNo=0x50+sSetSen;
            DoRYOnOff(sSetNu, sRyNo, false);        //關掉Y50+SensorID
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
                DoRYOnOff(sSetNu, sRyNo, true); //打開Y50+SensorID
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
                DoRYOnOff(sSetNu, sRyNo, false); //關掉Y50+SensorID
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
bool TMyCCLinkBoard::DoSetMultiValue()    //針對單一Sensor設定值
{
    static short sWwDevNo;                //寫入Ww04+SensorID的位置
    static short sRyNo;    //Ry50+SensorID的位置
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
            DoRYOnOff(sSetNu, sRyNo, false);        //關掉Y50+SensorID
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
                DoRYOnOff(sSetNu, sRyNo, true); //打開Y50+SensorID
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
                DoRYOnOff(sSetNu, sRyNo, false); //關掉Y50+SensorID
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
                if(iCount<fCCLink->iAllSetCount)
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
        Result=mdDevSet(lPath, StaNo, DevType, DevNo);    //Sets a bit device.
    else
        Result=mdDevRst(lPath, StaNo, DevType, DevNo);    //Resets a bit device.

    if(Result!=0)
    {
        bHasErr=true;
        ErrMess.sprintf("DoRYOnOff Error! %d", Result);
        return false;
    }
    bHasErr=false;
    ErrMess="";
    return true;
};
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
    Result=mdSend(lPath, StaNo, DevType, DevNo, &sSize, iData);  //Performs batch write of devices.

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
    Result=mdReceive(lPath, StaNo, DevType, DevNo, &sSize, iData);

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
};
//------------------------------------------------------------------------------
/*void TMyCCLinkBoard::GetCClinkErrMes(int ErrCode, AnsiString str[3])
{
    if(     ErrCode==1)
    {
        str[0]="Driver not started";
        str[1]="The driver has not been started. The same interrupt number and I/O address as those of another board are used.";
        str[2]="Correct the error occurred when starting the driver. Check the board settings.";
    }
    else if(ErrCode==2)
    {
        str[0]="Board response error";
        str[1]="A timeout has occurred while waiting for a response to the corrective action.";
        str[2]="Review the operation status and board loading conditions of the access station. Retry with a user program.";
    }
    else if(ErrCode==65)
    {
        str[0]="Channel error";
        str[1]="An unregistered channel number was designated.";
        str[2]="Check the channel number.";
    }
    else if(ErrCode==66)
    {
        str[0]="Already open error";
        str[1]="The designated channel has already been open.";
        str[2]="Open only once.";
    }
    else if(ErrCode==67)
    {
        str[0]="Already closed error";
        str[1]="The designated channel has already been closed.";
        str[2]="Close only once.";
    }
    else if(ErrCode==68)
    {
        str[0]="Path error";
        str[1]="A path other than for an open line was set.";
        str[2]="Set the path to the station number that has an open line.";
    }
    else if(ErrCode==69)
    {
        str[0]="Processing code error";
        str[1]="An unsupported processing code was issued.";
        str[2]="Use supported processing codes.";
    }
    else if(ErrCode==70)
    {
        str[0]="Station number designation error";
        str[1]="The designated station number is incorrect. A process that should have been requested to other station was requested to own station. Or, the station number corresponds to own station (0xFF) but the network number is not 0.";
        str[2]="Correct the designation of the station numbers in a user program.";
    }
    else if(ErrCode==71)
    {
        str[0]="Receiving data error (when RECV is requested)";
        str[1]="Data has not been received.";
        str[2]="Wait until data is received.";
    }
    else if(ErrCode==77)
    {
        str[0]="Memory allocation error";
        str[1]="Sufficient memory could not be allocated.";
        str[2]="End all other application programs that are currently running. Check if the system is operating normally. Restart the system. Increase the minimum working set area of the PC.";
    }
    else if(ErrCode==78)
    {
        str[0]="Timeout error during mode setting";
        str[1]="Mode setting was attempted but failed due to timeout.";
        str[2]="Make sure that the dual-port memory is not used by another board, and restart. Hardware failure.";
    }
    else if(ErrCode==79)
    {
        str[0]="Software setting data error";
        str[1]="Argument parameters were not set correctly during software setting.";
        str[2]="Check the contents of the argument parameters in the software setting data.";
    }
    else if(ErrCode==81)
    {
        str[0]="Startup source channel response error (when SEND is requested)";
        str[1]="A response error when SEND is requested is abnormal.";
        str[2]="Retry. Check if the system is operating normally. Restart the system.";
    }
    else if(ErrCode==85)
    {
        str[0]="Channel number error (when RECV is requested)";
        str[1]="Channel number error.";
        str[2]="Check the channel number used when RECV is requested.";
    }
    else if(ErrCode==100)
    {
        str[0]="Accessing own station board or requesting SEND";
        str[1]="An access request was issued to the own station board while accessing the own station board.";
        str[2]="Retry.";
    }
    else if(ErrCode==101)
    {
        str[0]="Routing parameter error";
        str[1]="The routing parameters are not set.";
        str[2]="Correct the routing parameters.";
    }
    else if(ErrCode==102)
    {
        str[0]="Data sending error";
        str[1]="Data sending has failed.";
        str[2]="";
    }
    else if(ErrCode==103)
    {
        str[0]="Data receiving error";
        str[1]="Data receiving has failed.";
        str[2]="Retry. Check if the system is operating normally. Restart the system.";
    }
    else if(ErrCode==129)
    {
        str[0]="Device type error";
        str[1]="The designated device type is invalid.";
        str[2]="Check the device type.";
    }
    else if(ErrCode==130)
    {
        str[0]="Device number error";
        str[1]="The designated device number is out of range. A device number other than a multiple of 8 was designated for bit device designation.";
        str[2]="Check the device number.";
    }
    else if(ErrCode==131)
    {
        str[0]="Number of device points error";
        str[1]="The designated number of points is out of device range. A device number other than a multiple of 8 was designated for bit device designation.";
        str[2]="Check the size.";
    }
    else if(ErrCode==132)
    {
        str[0]="Number of write bytes error";
        str[1]="The designated number of bytes to be written was out of range.";
        str[2]="Set the number of bytes to be written within the range.";
    }
    else if(ErrCode==133)
    {
        str[0]="Link parameter error";
        str[1]="The link parameters are corrupted. The total number of slave stations is 0 in a link parameter. Link parameter's fixed pattern is corrupted. Link parameter's sum check is corrupted.";
        str[2]="Reset the link parameters.";
    }
    else if(ErrCode==136)
    {
        str[0]="Random write designation error";
        str[1]="A value other than 0 to 2 was designated for random read.";
        str[2]="Correct the setting value to 0 to 2.";
    }
    else if(ErrCode==215)
    {
        str[0]="Receiving data length error / The request data buffer length exceeded the limit";
        str[1]="Receiving data length or byte length exceeds the range. / The request data length exceeded the request data area.";
        str[2]="Retry. Check the cable. / Make the request data smaller.";
    }
    else if(ErrCode==224)
    {
        str[0]="PC number error";
        str[1]="The request destination is not found.";
        str[2]="Correct the station number.";
    }
    else if(ErrCode==225)
    {
        str[0]="Processing mode error";
        str[1]="A processing code that could not be processed by the request destination's ACPU was set. (This is checked by the request destination's ACPU.)";
        str[2]="Review the request destination's ACPU and processing codes.";
    }
    else if(ErrCode==227)
    {
        str[0]="Other data error";
        str[1]="Data such as the address, head step or number of shifts of the request data was incorrect.";
        str[2]="Correct the request data.";
    }
    else if(ErrCode==228)
    {
        str[0]="Link designation error";
        str[1]="A processing code that could not be processed by the request destination station was set. (This is checked by the request destination's link module.)";
        str[2]="Check the request destination's station number and processing codes.";
    }
    else if(ErrCode==1280)
    {
        str[0]="Own board memory access error";
        str[1]="";
        str[2]="Check the switch settings of the own board and move the memory address to an area that is not affected by other board. Change the memory access setting to 16 bits if it is set to 8 bits.";
    }
    else if(ErrCode==1281)
    {
        str[0]="Cannot access the I/O port";
        str[1]="";
        str[2]="Check the I/O port address setting. Perform a self-loopback test of the board and check hardware.";
    }
    else if(ErrCode>=4110 && ErrCode<=8278)
    {
        str[1]="";
        str[2]="Exit the program and restart the PC. Setup the package again. Consult with the dealer regarding the problem.";
        if(     ErrCode==4110)
            str[0]="DLL non-load error";
        else if(ErrCode==8204)
            str[0]="Request cancel";
        else if(ErrCode==8205)
            str[0]="Drive name error";
        else if(ErrCode==8206)
            str[0]="First step error";
        else if(ErrCode==8207)
            str[0]="Parameter type error";
        else if(ErrCode==8208)
            str[0]="File name error";
        else if(ErrCode==8209)
            str[0]="Registering/release/set status error";
        else if(ErrCode==8210)
            str[0]="Detailed condition division error";
        else if(ErrCode==8211)
            str[0]="Step condition error";
        else if(ErrCode==8212)
            str[0]="Bit device condition error";
        else if(ErrCode==8213)
            str[0]="Parameter setting error";
        else if(ErrCode==8215)
            str[0]="Keyword error";
        else if(ErrCode==8216)
            str[0]="Read/write flag error";
        else if(ErrCode==8217)
            str[0]="Refresh method error";
        else if(ErrCode==8218)
            str[0]="Buffer access method error";
        else if(ErrCode==8219)
            str[0]="Start mode/stop mode error";
        else if(ErrCode==8220)
            str[0]="Written clock data error";
        else if(ErrCode==8221)
            str[0]="Online data write error";
        else if(ErrCode==8223)
            str[0]="Trace time error";
        else if(ErrCode==8224)
            str[0]="First I/O number error";
        else if(ErrCode==8225)
            str[0]="First address error";
        else if(ErrCode==8226)
            str[0]="Pattern error";
        else if(ErrCode==8227)
            str[0]="SFC block number error";
        else if(ErrCode==8228)
            str[0]="SFC step number error";
        else if(ErrCode==8229)
            str[0]="Step number error";
        else if(ErrCode==8230)
            str[0]="Data error";
        else if(ErrCode==8231)
            str[0]="System data error";
        else if(ErrCode==8232)
            str[0]="TC set value number error";
        else if(ErrCode==8233)
            str[0]="Clear mode error";
        else if(ErrCode==8234)
            str[0]="Signal flow error";
        else if(ErrCode==8235)
            str[0]="Version administration error";
        else if(ErrCode==8236)
            str[0]="Module has been registered";
        else if(ErrCode==8237)
            str[0]="PI type error";
        else if(ErrCode==8238)
            str[0]="PI No error";
        else if(ErrCode==8239)
            str[0]="PI number error";
        else if(ErrCode==8240)
            str[0]="Shift error";
        else if(ErrCode==8241)
            str[0]="File type error";
        else if(ErrCode==8242)
            str[0]="Specified module error";
        else if(ErrCode==8243)
            str[0]="Error check flag error";
        else if(ErrCode==8244)
            str[0]="Step RUN-operation error";
        else if(ErrCode==8245)
            str[0]="Step RUN data error";
        else if(ErrCode==8246)
            str[0]="Step RUN-time error";
        else if(ErrCode==8247)
            str[0]="Program RUN inside writing error to E2ROM";
        else if(ErrCode==8248)
            str[0]="Clock data read/write error";
        else if(ErrCode==8249)
            str[0]="Trace non-completion";
        else if(ErrCode==8250)
            str[0]="Registration clearness flag error";
        else if(ErrCode==8251)
            str[0]="Operation error";
        else if(ErrCode==8252)
            str[0]="The number of station error";
        else if(ErrCode==8253)
            str[0]="The number of repeat error";
        else if(ErrCode==8254)
            str[0]="The acquisition data selection error";
        else if(ErrCode==8255)
            str[0]="The number of SFC cycle error";
        else if(ErrCode==8258)
            str[0]="The scheduled time setting error";
        else if(ErrCode==8259)
            str[0]="Function count error";
        else if(ErrCode==8260)
            str[0]="System information error";
        else if(ErrCode==8262)
            str[0]="Function number error";
        else if(ErrCode==8263)
            str[0]="RAM operation error";
        else if(ErrCode==8264)
            str[0]="Boot former ROM forwarding failure";
        else if(ErrCode==8265)
            str[0]="Boot former transfer mode specification error";
        else if(ErrCode==8266)
            str[0]="Not enough memory";
        else if(ErrCode==8267)
            str[0]="Backup drive (former boot drive) ROM error";
        else if(ErrCode==8268)
            str[0]="Block size error";
        else if(ErrCode==8269)
            str[0]="RUN-time detaching error";
        else if(ErrCode==8270)
            str[0]="Module has already registered";
        else if(ErrCode==8271)
            str[0]="Password registration data full error";
        else if(ErrCode==8272)
            str[0]="Password unregistration error";
        else if(ErrCode==8273)
            str[0]="Remote password error";
        else if(ErrCode==8274)
            str[0]="IP address error";
        else if(ErrCode==8275)
            str[0]="Error (argument when requesting) outside time-out value range";
        else if(ErrCode==8276)
            str[0]="Instruction cast undetection";
        else if(ErrCode==8277)
            str[0]="Trace execution type error";
        else if(ErrCode==8278)
            str[0]="Version error";
    }
    else if(ErrCode==16386)
    {
        str[0]="A request that could not be processed was received.";
        str[1]="";
        str[2]="Change the request destination.";
    }
    else if(ErrCode==16432)
    {
        str[0]="The designated device type does not exist.";
        str[1]="";
        str[2]="Check the designated device type.";
    }
    else if(ErrCode==16433)
    {
        str[0]="The designated device number is out of range.";
        str[1]="";
        str[2]="Check the designated device number.";
    }
    else if(ErrCode==16448)
    {
        str[0]="A module does not exist.";
        str[1]="";
        str[2]="Do not issue the request that generated the error to the designated special module.";
    }
    else if(ErrCode==16449)
    {
        str[0]="The number of device points is out of range.";
        str[1]="";
        str[2]="Check the head address and number of access points, and access the devices within the existing range.";
    }
    else if(ErrCode==16450)
    {
        str[0]="Corresponding module is abnormal.";
        str[1]="";
        str[2]="Check if the designated module is operating normally.";
    }
    else if(ErrCode==16451)
    {
        str[0]="A module does not exist at the designated location.";
        str[1]="";
        str[2]="Check the start I/O number of the designated module.";
    }
    else if(ErrCode>=16384 && ErrCode<=20479)
    {
        str[0]="Errors detected by the access target CPU.";
        str[1]="";
        str[2]="Refer to the user's manual of the access target CPU module.";
    }
    else if(ErrCode>=28672 && ErrCode<=32767)
    {
        str[0]="Errors detected by intelligent function modules such as the serial communication module.";
        str[1]="";
        str[2]="Refer to the user’s manual of the access target intelligent function module.";
    }
    else if(ErrCode==40577)
    {
        str[0]="Device type error";
        str[1]="The device type designated for the request destination station is invalid. (This is checked by the request destination's link module).";
        str[2]="Check the device type.";
    }
    else if(ErrCode==40578)
    {
        str[0]="Device number error";
        str[1]="The device number designated for the request destination station is out of range. A device number other than a multiple of 8 was designated for bit device designation. (This is checked by the request destination's link module).";
        str[2]="Check the device number.";
    }
    else if(ErrCode==40579)
    {
        str[0]="Error in the number of devices points";
        str[1]="The number of points set for the destination station is out of range. A device number other than a multiple of 8 was designated for bit device designation. (This is checked by the request destination's link module).";
        str[2]="Check the size.";
    }
    else if(ErrCode==-1)
    {
        str[0]="Bus error";
        str[1]="The designated bus is invalid.";
        str[2]="Check the bus that was returned by the mdOpen function.";
    }
    else if(ErrCode==-2)
    {
        str[0]="Device number error";
        str[1]="The designated device number is out of range. When a bit device was designated, the device number was not a multiple of 8.";
        str[2]="Check the head device number for the designated device.";
    }
    else if(ErrCode==-3)
    {
        str[0]="Device type error";
        str[1]="The designated device type is invalid.";
        str[2]="Check if the device type used is found in the device list.";
    }
    else if(ErrCode==-4)
    {
        str[0]="CPU error";
        str[1]="An invalid station was designated.";
        str[2]="Check the status of the communication station. Check the designated station number.";
    }
    else if(ErrCode==-5)
    {
        str[0]="Size error";
        str[1]="The device number and size exceeded the device range. Access was attempted using an odd numbered device. The device number and size exceeded the range for the same block.";
        str[2]="Check the designated device size. Check the device number and size.";
    }
    else if(ErrCode==-6)
    {
        str[0]="Number of blocks error";
        str[1]="The number of blocks designated in dev [0] for device random read/write is out of range.";
        str[2]="Check the number of blocks designated in dev [0].";
    }
    else if(ErrCode==-8)
    {
        str[0]="Channel number error";
        str[1]="The channel number designated in the mdOpen function is invalid.";
        str[2]="Check the designated channel number.";
    }
    else if(ErrCode==-11)
    {
        str[0]="Insufficient buffer area";
        str[1]="The read area size for read data storage array variables is too small.";
        str[2]="Check the read size and read data storage destination size.";
    }
    else if(ErrCode==-12)
    {
        str[0]="Block error";
        str[1]="The block number of the designated extension file register is invalid.";
        str[2]="Check the block number (device type) of the extension file register.";
    }
    else if(ErrCode==-13)
    {
        str[0]="Write protect error";
        str[1]="The block number of the designated extension file register duplicates with the write protect area of the memory cassette.";
        str[2]="Check the block number (device type) of the extension file register. Check the write protection DIP switch of the access destination's memory cassette.";
    }
    else if(ErrCode==-14)
    {
        str[0]="Memory cassette error";
        str[1]="No memory cassette is installed in the accessed CPU, or an incorrect memory cassette is loaded.";
        str[2]="Check the memory cassette of the access destination.";
    }
    else if(ErrCode==-15)
    {
        str[0]="Read area length error";
        str[1]="The read area size for read data storage array variables is too small.";
        str[2]="Check the read size and read data storage destination size.";
    }
    else if(ErrCode==-16)
    {
        str[0]="Station number/network number error";
        str[1]="The station number/network number is out of range.";
        str[2]="Check the designated station number/network number.";
    }
    else if(ErrCode==-17)
    {
        str[0]="All-station/group number designation error";
        str[1]="All-station/group number was designated for a function that does not support all-station/group number designation.";
        str[2]="Check if the function supports all-station/group number designation.";
    }
    else if(ErrCode==-18)
    {
        str[0]="Remote designation error";
        str[1]="An undesignated code was designated.";
        str[2]="An undesignated code was designated.";
    }
    else if(ErrCode==-19)
    {
        str[0]="SEND/RECV channel number error";
        str[1]="The channel number designated with the SEND/RECV function is out of range.";
        str[2]="Check the designated channel number.";
    }
    else if(ErrCode==-21)
    {
        str[0]="Error occurred in gethostbyname ( )";
        str[1]="An error occurred in the gethostbyname ( ) function.";
        str[2]="Check if the designated host name exists in the HOSTS file.";
    }
    else if(ErrCode==-24)
    {
        str[0]="Timeout error occurred in select ( )";
        str[1]="A timeout error occurred in the select ( ) function.";
        str[2]="Check if the MGW server service has been started in the server machine.";
    }
    else if(ErrCode==-25)
    {
        str[0]="Error occurred in sendto ( )";
        str[1]="An error occurred in the sendto ( ) function.";
        str[2]="Check if normal Ethernet communication can be performed with the server machine.";
    }
    else if(ErrCode==-26)
    {
        str[0]="Error ocurred in recvfrom ( )";
        str[1]="An error occurred in the recvfrom ( ) function.";
        str[2]="Check if normal Ethernet communication can be performed with the server machine.";
    }
    else if(ErrCode==-28)
    {
        str[0]="Abnormal response received";
        str[1]="An abnormal response was received.";
        str[2]="Check if normal Ethernet communication can be performed with the server machine.";
    }
    else if(ErrCode==-29)
    {
        str[0]="Receiving data length error";
        str[1]="Too much data was received.";
        str[2]="Check if normal Ethernet communication can be performed with the server machine.";
    }
    else if(ErrCode==-30)
    {
        str[0]="Sequence number error";
        str[1]="The received sequence number is abnormal.";
        str[2]="Check if normal Ethernet communication can be performed with the server machine.";
    }
    else if(ErrCode==-31)
    {
        str[0]="DLL load error";
        str[1]="An attempt to load a DLL required to execute functions has failed.";
        str[2]="Set up the utility software again.";
    }
    else if(ErrCode==-32)
    {
        str[0]="Another task/thread is occupying the resource and the";
        str[1]="resource is not released within 30 seconds.";
        str[2]="Retry. Memory may be insufficient. Close all other applications that are currently running. Check if the system is operating normally. Restart the system. Exit the program and restart the personal computer.";
    }
    else if(ErrCode==-33)
    {
        str[0]="Invalid access destination error";
        str[1]="The setting for the communication destination is invalid.";
        str[2]="Check if the communication destination is correctly set by the utility.";
    }
    else if(ErrCode==-34)
    {
        str[0]="Registry error";
        str[1]="An attempt to open the registry has failed.";
        str[2]="Check if the communication destination is correctly set by the utility.";
    }
    else if(ErrCode==-35)
    {
        str[0]="Registry read error";
        str[1]="An attempt to read from the registry has failed.";
        str[2]="Check if the communication destination is correctly set by the utility.";
    }
    else if(ErrCode==-36)
    {
        str[0]="Registry write error";
        str[1]="An attempt to write to the registry has failed.";
        str[2]="Check if the communication destination is correctly set by the utility.";
    }
    else if(ErrCode==-37)
    {
        str[0]="Communication initialization setting error";
        str[1]="An attempt to perform initial setting for communication has failed.";
        str[2]="Retry. Memory may be insufficient. Close all other applications that are currently running. Check if the system is operating normally. Restart the system.";
    }
    else if(ErrCode==-38)
    {
        str[0]="Ethernet communication error";
        str[1]="An attempt to set an Ethernet communication has failed.";
        str[2]="Retry. Check if the communication destination is correctly set by the utility. Memory may be insufficient. Close all other applications that are currently running. Check if the system is operating normally. Restart the system.";
    }
    else if(ErrCode==-39)
    {
        str[0]="COM communication setting error";
        str[1]="An attempt to set a COM communication has failed.";
        str[2]="Retry. Check if the communication destination is correctly set by the utility. Memory may be insufficient. Close all other applications that are currently running. Check if the system is operating normally. Restart the system.";
    }
    else if(ErrCode==-41)
    {
        str[0]="COM control error";
        str[1]="Control cannot be performed properly in COM communication.";
        str[2]="Retry. Check if the system is operating normally. Restart the system.";
    }
    else if(ErrCode==-42)
    {
        str[0]="Close error";
        str[1]="Communication cannot be closed.";
        str[2]="Retry. Check if the system is operating normally. Restart the system.";
    }
    else if(ErrCode==-43)
    {
        str[0]="ROM operation error";
        str[1]="A TC setting value was written to the CPU during ROM operation.";
        str[2]="Change the TC setting value during RAM operation.";
    }
    else if(ErrCode==-44)
    {
        str[0]="LLT communication setting error";
        str[1]="An attempt to set an LLT communication has failed.";
        str[2]="Retry. Check if the communication destination is correctly set by the utility. Memory may be insufficient. Close all other applications that are currently running.";
    }
    else if(ErrCode==-45)
    {
        str[0]="Ethernet control error";
        str[1]="Control cannot be performed properly in Ethernet communication.";
        str[2]="Retry. Check if the system is operating normally.";
    }
    else if(ErrCode==-46)
    {
        str[0]="USB open error";
        str[1]="Initialization and opening of the USB port failed.";
        str[2]="Memory may be insufficient. Close all other applications that are currently running. Check if the system is operating normally. Restart the system.";
    }
    else if(ErrCode==-47)
    {
        str[0]="Random read condition unsatisfied error";
        str[1]="Random read cannot be performed because the random read condition is not satisfied.";
        str[2]="Conditioned random read is set from a software such as GX Developer. Wait until the conditions are satisfied. Clear the condition settings.";
    }
    else if(ErrCode==-48)
    {
        str[0]="TEL error.";
        str[1]="";
        str[2]="Exit the program and restart the PC. Setup the package again. Consult with the dealer regarding the problem.";
    }
    else if(ErrCode==-50)
    {
        str[0]="Maximum open path value error";
        str[1]="The number of open paths exceeds the maximum value (32).";
        str[2]="Close several paths.";
    }
    else if(ErrCode==-51)
    {
        str[0]="Exclusive control error";
        str[1]="An exclusive control error occurred.";
        str[2]="Retry. Check if the system is operating normally.";
    }
    else if(ErrCode<=-257 && ErrCode>=-4096)
    {
        str[0]="Errors detected in the MELSECNET/H, MELSECNET/10 network system.";
        str[1]="";
        str[2]="MELSECNET/10 network system Refer to the MELSECNET/H, MELSECNET/10 network system reference manual.";
    }
    else if(ErrCode<=-12289 && ErrCode>=-16384)
    {
        str[0]="Errors detected by the Ethernet interface module";
        str[1]="";
        str[2]="Refer to the user’s manual of the Ethernet interface module.";
    }
    else if(ErrCode==-18558)
    {
        str[0]="Request destination station number specification error";
        str[1]="The local station number was specified as the station number of the processing request destination station.";
        str[2]="Review the correct request destination station number.";
    }
    else if(ErrCode==-18560)
    {
        str[0]="A transient request was issued to a remote I/O station.";
        str[1]="";
        str[2]="Check the request destination station.";
    }
    else if(ErrCode==-18575)
    {
        str[0]="Other station's dedicated device was accessed to own station.";
        str[1]="";
        str[2]="Check the device type.";
    }
    else if(ErrCode<=-16385 && ErrCode>=-20480)
    {
        str[0]="Errors detected in the CC-Link system.";
        str[1]="";
        str[2]="Refer to the CC-Link system master/local module user's manual. * Refer to the QJ61BT11N User’s Manual for whether the cyclic data can be transmitted to/from stations compatible with CC-Link Ver.2.";
    }
    else if(ErrCode==-24957)
    {
        str[0]="Number of device points error";
        str[1]="The number of points set for the request destination station is out of device range. When a bit device was designated, the number of points was not a multiple of 8. (This is checked by the request destination's link module.)";
        str[2]="Check the size.";
    }
    else if(ErrCode==-24958)
    {
        str[0]="Device number error";
        str[1]="The device number designated for the request destination station is out of device range. When a bit device was designated, the device number was not a multiple of 8. (This is checked by the request destination's link module.)";
        str[2]="Check the device number.";
    }
    else if(ErrCode==-24959)
    {
        str[0]="Device type error";
        str[1]="The device type designated for the request destination station is invalid. (This is checked by the request destination's link module.)";
        str[2]="Check the device type.";
    }
    else if(ErrCode==-25056)
    {
        str[0]="Processing code error";
        str[1]="A processing code that could not be processed by the request destination station was set. (This is checked by the request destination's link module.)";
        str[2]="Check the request destination station number and processing codes.";
    }
    else if(ErrCode==-26334)
    {
        str[0]="Board reset error";
        str[1]="Another process that used the same channel executed a board reset while accessing other station.";
        str[2]="Retry.";
    }
    else if(ErrCode==-26336)
    {
        str[0]="Request error for another loop";
        str[1]="Routing to another loop was performed.";
        str[2]="Change the routing request destination to an AnUCPU or QnACPU.";
    }
    else if(ErrCode==-28141)
    {
        str[0]="System sleep error";
        str[1]="Shift to the sleep or the hibernate was detected.";
        str[2]="Reboot the system. Change the setting of Power Options to prevent the system from transitioning to sleep or hibernate.";
    }
    else if(ErrCode==-28150)
    {
        str[0]="Device access error of the data link interrupted station";
        str[1]="Attempt was made to access the device ranges of a data link interrupted station of the own station devices RX, RY, RWw and RWr, as well as a reserved station.";
        str[2]="Data can be read and written, but the validity of the data is not guaranteed.";
    }
    else if(ErrCode==-28151)
    {
        str[0]="APS NO error";
        str[1]="Invalid response data was received.";
        str[2]="Change the device that requested the processing.";
    }
    else if(ErrCode==-28156)
    {
        str[0]="Dual-port memory handshake error";
        str[1]="";
        str[2]="Remove other option boards.";
    }
    else if(ErrCode==-28158)
    {
        str[0]="WDT error";
        str[1]="";
        str[2]="Reset the board. Restart the personal computer. Take the measures for WDT error occurrence. (Refer to Section 17.4.)";
    }
    else
    {
        str[0]="CC Link Un-Know Error";
        str[1]="";
        str[2]="";
    }
}
                           */

