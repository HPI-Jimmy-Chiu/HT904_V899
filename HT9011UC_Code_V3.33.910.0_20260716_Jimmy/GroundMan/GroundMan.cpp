#include "MachineDefine.h"
#pragma hdrstop

#include "GroundMan.h"
#include "database.h"
#include "rs232.h"
#include "mymessbox.h"
#include "common.h"
#include "csystem.h"
#include "cmydef.h"
#include "note.h"
#include "MyMotor.h"
#include "myQwertyKeyBoard.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SPComm"
#pragma link "ALed"
#pragma link "MyLedLane"
#pragma resource "*.dfm"
TfGroundMan *fGroundMan;
int Alarm_Continuous_Time;
int Alarm_Occurrences;
//---------------------------------------------------------------------------
__fastcall TfGroundMan::TfGroundMan(TComponent* Owner)
    : TForm(Owner)
{
    ZeroMemory(asRecordValueData, sizeof(asRecordValueData));
    ZeroMemory(bRecordAlarmData, sizeof(bRecordAlarmData));
    ZeroMemory(bReaderOK, sizeof(bReaderOK));
    ZeroMemory(bOpenClose, sizeof(bOpenClose));
    ZeroMemory(asShowName, sizeof(asShowName));

    iGroundMasterTask=1;
    bRs232Ok=false;
    fShow=false;
    bGroundManReset=false;
    bGroundManResetByStart=true;
    bUseOffset=true;

    if(HSys.iGroundManScanPoint==0)  //JerryYang 20210428 : Add 28點接地偵側
    {
        iUseGndBoard=1;
    }
    else if(HSys.iGroundManScanPoint==1)
    {
        iUseGndBoard=3;
    }
    else
    {
        iUseGndBoard=4;
    }

    for(int i=0; i<4; i++)  //JerryYang 20210428 : Add 28點接地偵側
    {
        for(int j=0; j<8; j++)
        {
            bOpenClose[i][j]=true;
        }
    }

    if(HSys.iGroundManScanPoint==0) // 8點偵測
    {
        for(int i=0; i<3; i++)
        {
            for(int j=0; j<8; j++)
            {
                if(i==1 || i==2)
                {
                    bOpenClose[i][j]=false;
                }
            }
        }
    }
    else if(HSys.iGroundManScanPoint==1) // 22點偵測
    {
        bOpenClose[0][0]=false;
        bOpenClose[0][1]=false;
    }
    else if(HSys.iGroundManScanPoint==2) //JerryYang 20210428 : Add 28點接地偵側
    {
        bOpenClose[0][0]=false;
        bOpenClose[0][1]=false;
        bOpenClose[3][6]=false;
        bOpenClose[3][7]=false;
    }
    asShowName[0][0]="InArm";
    asShowName[0][1]="OutArm";
    asShowName[0][2]="Arm1";
    asShowName[0][3]="Arm2";
    asShowName[0][4]="HP1";
    asShowName[0][5]="HP2";
    asShowName[0][6]="SH1";
    asShowName[0][7]="SH2";

    //KaiChen 20200707 ：修改 GroundMan 接點對應位置
    //==>
    asShowName[1][0]="InArm_A";
    asShowName[1][1]="InArm_C";
    asShowName[1][2]="InArm_E";
    asShowName[1][3]="InArm_G";
    asShowName[1][4]="InArm_B";
    asShowName[1][5]="InArm_D";
    asShowName[1][6]="InArm_F";
    asShowName[1][7]="InArm_H";

    asShowName[2][0]="OutArm_A";
    asShowName[2][1]="OutArm_C";
    asShowName[2][2]="OutArm_E";
    asShowName[2][3]="OutArm_G";
    asShowName[2][4]="OutArm_B";
    asShowName[2][5]="OutArm_D";
    asShowName[2][6]="OutArm_F";
    asShowName[2][7]="OutArm_H";
    //<==
    //KaiChen 20200707 ：修改 GroundMan 接點對應位置

    asShowName[3][0]="Loader";  //JerryYang 20210428 : Add 28點接地偵側
    asShowName[3][1]="Empty";
    asShowName[3][2]="Color";
    asShowName[3][3]="Auto1";
    asShowName[3][4]="Auto2";
    asShowName[3][5]="Auto3";
    asShowName[3][6]="Ch6";
    asShowName[3][7]="Ch7";
    asGroundVaule="NA"; //Sam 20211223 : 每顆 IC 測試完畢都要記錄當時的 Ground & ESD 數值。
    sCMD[0]="AlarmSet";     //Sam 20230906 : 新增 GroundMan Msg Log
    sCMD[1]="MachineOhm";
    sCMD[2]="AlarmGet";
    sCMD[3]="BoardOhm";
    sCMD[4]="BoardReset";
    sCMD[5]="BoardVersion";
    bInternalOhm=false;        //KenHsieh 20220803 : GroundMan流程及功能修改
    bVersion=false;            //KenHsieh 20220803 : GroundMan流程及功能修改
    Timer1->Interval=30;            //KenHsieh 20220803 : GroundMan流程及功能修改
    iMachineOhmRetry=0;
    ReadGroundOffset();
}
//---------------------------------------------------------------------------
void __fastcall TfGroundMan::FormShow(TObject *Sender)
{
    Left=100;   //Steven 20220407 : 避免跑到畫面外
    Top=100;
    TLabel *LabelShowPtr[4][8]={
                                {labCH_0_0, labCH_0_1, labCH_0_2, labCH_0_3,
                                labCH_0_4, labCH_0_5, labCH_0_6, labCH_0_7},
                                {labCH_1_0, labCH_1_1, labCH_1_2, labCH_1_3,
                                labCH_1_4, labCH_1_5, labCH_1_6, labCH_1_7},
                                {labCH_2_0, labCH_2_1, labCH_2_2, labCH_2_3,
                                labCH_2_4, labCH_2_5, labCH_2_6, labCH_2_7},  //JerryYang 20210428 : Add 28點接地偵側
                                {labCH_3_0, labCH_3_1, labCH_3_2, labCH_3_3,
                                labCH_3_4, labCH_3_5, labCH_3_6, labCH_3_7}
                                };

    if(HSys.iGroundManScanPoint==0) // 8點偵測
    {
        gbBoard0->Visible=true;
        gbBoard1->Visible=false;
        gbBoard2->Visible=false;
        gbBoard3->Visible=false;
        labBoardOhrm->Visible=true;
        labBoardVersion->Visible=true;
    }
    else if(HSys.iGroundManScanPoint==1) // 22點偵測
    {
        gbBoard0->Visible=true;
        gbBoard1->Visible=true;
        gbBoard2->Visible=true;
        gbBoard3->Visible=false;
        labCH_0_0->Visible=false;
        labCH_0_1->Visible=false;
        led_0_0->Visible=false;
        led_0_1->Visible=false;
        labValue_0_0->Visible=false;
        labValue_0_1->Visible=false;
        labCount_0_0->Visible=false;
        labCount_0_1->Visible=false;

        labBoardOhrm->Visible=true;
        labBoardOhrm2->Visible=true;
        labBoardOhrm3->Visible=true;
        labBoardVersion->Visible=true;
        labBoardVersion2->Visible=true;
        labBoardVersion3->Visible=true;
    }
    else if(HSys.iGroundManScanPoint==2) //JerryYang 20210428 : Add 28點接地偵側
    {
        gbBoard0->Visible=true;
        gbBoard1->Visible=true;
        gbBoard2->Visible=true;
        gbBoard3->Visible=true;
//        labCH_0_0->Visible=false;
//        labCH_0_1->Visible=false;
//        led_0_0->Visible=false;
//        led_0_1->Visible=false;
//        labValue_0_0->Visible=false;
//        labValue_0_1->Visible=false;

        labBoardOhrm->Visible=true;
        labBoardOhrm2->Visible=true;
        labBoardOhrm3->Visible=true;
        labBoardOhrm4->Visible=true;
        labBoardVersion->Visible=true;
        labBoardVersion2->Visible=true;
        labBoardVersion3->Visible=true;
        labBoardVersion4->Visible=true;
    }

    for(int i=0; i<iUseGndBoard; i++)
    {
        for(int j=0; j<8; j++)
        {
            LabelShowPtr[i][j]->Caption=asShowName[i][j];
        }
    }

    if(CUSTOMER_CODE==CC_KYEC_LEE)
    {
        edContinuous_Time->Enabled=false;
        edOccurrences->Enabled=false;
    }

    btnMaintenanceMode->Visible=(CUSTOMER_CODE==CC_SIGURD_PeiXing && AccessLevel==iDefHonPrecLevel);     //Sam 20250122 : 北興廠新增維修模式，不需要報警要一直顯示阻值

    PageControl1->ActivePageIndex=0;
    fShow=true;
}
//---------------------------------------------------------------------------
void __fastcall TfGroundMan::FormClose(TObject *Sender,
      TCloseAction &Action)
{
    bool bReStart=true;

    if(CUSTOMER_CODE==CC_SIGURD_HUKOU || CUSTOMER_CODE==CC_SIGURD_PeiXing)              //Sam 20220107 : 矽格中興俊堯湖口卓邵要求可以關閉 Ground 功能
        bReStart=false;
    else
        bReStart=true;

    if(bReStart)
         ReStart();  //Sam 20220107 : 矽格北興 Initail Start 要重啟 GroundMan //Ifor 20201112 add: 避免 Ground 被關掉後失效
    btnMaintenanceMode->Down=false;                                             //Sam 20250122 : 北興廠新增維修模式，不需要報警要一直顯示阻值

    fShow=false;
    //這一行請保持在最下面!!-----------------
//    myLog.Do_Log(Sender, asUser, asLogPath);    //Steven 20100629
}
//---------------------------------------------------------------------------
void __fastcall TfGroundMan::spbStartComClick(TObject *Sender)
{
    comGM->StopComm();
    Init_GM_RS232();
}
//---------------------------------------------------------------------------
void __fastcall TfGroundMan::spbStopComClick(TObject *Sender)
{
    comGM->StopComm();
    bRs232Ok=false;
}
//---------------------------------------------------------------------------
void __fastcall TfGroundMan::Init_GM_RS232()
{
    bool flag;
    AnsiString str;

    iGroundMasterTask=1;
    bGroundManReset=false;
    ReadGroundOffset();
    for(int i=0; i<2; i++)
    {
        if(bRs232Ok)
        {
            continue;
        }
        flag=GetCOMPortStatus(HSys.asGroundManComPort);
        if(flag==false)
        {
            str.sprintf("GroundMan %d: %s port status error!", i+1, HSys.asGroundManComPort);
            ShowMyMessage(str);
            continue;
        }

        comGM->BaudRate =9600;
        comGM->ByteSize =TByteSize(_8);
        comGM->StopBits =TStopBits(_1);
        comGM->Parity   =None;

        comGM->CommName="\\\\.\\"+HSys.asGroundManComPort;
        try
        {
            comGM->StopComm();
            comGM->StartComm();  //僅能啟動一次
            bRs232Ok=true;
        }
        catch(...)
        {
            MyDBIProcess("Exception", "TfGroundMan::Init_GM_RS232");
            bRs232Ok=false;
            str.sprintf("GroundMan %d: COM%d port start error!", i+1, HSys.asGroundManComPort);
            ShowMyMessage(str);
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfGroundMan::comGMReceiveData(TObject *Sender,
      Pointer Buffer, WORD BufferLength)
{
    unsigned char data[256], cRCStr[256];
    AnsiString S, S1, asStr, asAlarm, asCRC, aCRC_Low, aCRC_High;
    AnsiString asBuf="",sMsg="",asRetVal="";
    int i;
    int iAddress;

    TLabel *LabelBoardOhrmPtr[4]={labBoardOhrm, labBoardOhrm2, labBoardOhrm3, labBoardOhrm4};
    TLabel *LabelBoardVersionPtr[4]={labBoardVersion, labBoardVersion2, labBoardVersion3, labBoardVersion4};
    //JimmyChiu 20210922 Avoid overflow
    //==>
    if(BufferLength>256)
    {
        LogMsg.sprintf("Receive (%d): %s", BufferLength, Buffer);
        ShowMyMessage(LogMsg);
        return;
    }
    //<==
    //JimmyChiu 20210922 Avoid overflow
    memcpy(data, Buffer, BufferLength);
    data[BufferLength] = '\x0';

    iAddress=data[2]-48;

    if(iAddress<0 || iAddress>3)
        return;

    sprintf(cRCStr, ":%c%c%c%c%c%c%c%c%c%c",
            data[1], data[2], data[3], data[4], data[5],
            data[6], data[7], data[8], data[9], data[10]);
    asCRC=calc_crc(cRCStr, strlen(cRCStr));
    asStr=asCRC.SubString(1,asCRC.Pos(",")-1);
    aCRC_Low="0x"+IntToHex(asStr.ToIntDef(0),2);
    asStr=asCRC;
    asStr.Delete(1,asCRC.Pos(","));
    aCRC_High="0x"+IntToHex(asStr.ToIntDef(0),2);
    cRCStr[11]=aCRC_Low.ToIntDef(0);
    cRCStr[12]=aCRC_High.ToIntDef(0);

    if(cRCStr[11]!=data[11] || cRCStr[12]!=data[12])
    {
        return;
    }
    else if(data[1]==48)    // Set    0x30
    {
        sMsg.sprintf("Board[%d] AlarmSet OK",iAddress+1);   //Sam 20230906 : 新增 GroundMan Msg Log
        bReaderOK[iAddress][eAlarmSet]=true;
    }
    else if(data[1]==49) //Read Value   0x31
    {
        asRetVal="";    //Sam 20230906 : 新增 GroundMan Msg Log
        for(i=0; i<8; i++)
        {
            if(data[i+3]<=127)
            {
                asRecordValueData[iAddress][i].sprintf("%2.1f", abs(data[i+3])/10.0);
                dRecordValueData_AddOffset[iAddress][i]=(abs(data[i+3])/10.0)-dOffset[iAddress][i];
            }
            else        //KenHsieh 20220803 : GroundMan流程及功能修改
            {
                asRecordValueData[iAddress][i]="> 10";
                dRecordValueData_AddOffset[iAddress][i]=10.0;
            }
            asRetVal+=asRecordValueData[iAddress][i]+",";   //Sam 20230906 : 新增 GroundMan Msg Log
        }
        sMsg.sprintf("Board[%d] MachineOhm %s",iAddress+1, asRetVal);   //Sam 20230906 : 新增 GroundMan Msg Log
        bReaderOK[iAddress][eMachineOhm]=true;

        //Sam 20211223 : 每顆 IC 測試完畢都要記錄當時的 Ground & ESD 數值。
        //==>
        AnsiString asGround="";
        for(int i=0; i<4; i++)
            for(int j=0; j<8; j++)
                asGround+=asRecordValueData[i][j]+",";                          //Sam 20250609 : Record Ground and ESD at intervals and upload
        asGroundVaule=asGround;
        //<==
        //Sam 20211223 : 每顆 IC 測試完畢都要記錄當時的 Ground & ESD 數值。

        //KenHsieh 20220728 : 新增GroundMan Value Log
        //==>
        if((HSys.iGroundManScanPoint==0 && iAddress==0) ||      //8點讀取最後一站
           (HSys.iGroundManScanPoint==1 && iAddress==2) ||      //22點讀取最後一站
           (HSys.iGroundManScanPoint==2 && iAddress==3))        //28點讀取最後一站
        {
            TStringList *slGroundManValue;
            slGroundManValue=new TStringList();

            for(int i=0; i<iUseGndBoard; i++)
            {
                for(int j=0; j<8; j++)
                {
                    if(bOpenClose[i][j])
                    {
                        if(bUseOffset==true)
                            slGroundManValue->Add(dRecordValueData_AddOffset[i][j]);
                        else
                            slGroundManValue->Add(asRecordValueData[i][j]);
                    }
                    else
                    {
                        slGroundManValue->Add("");
                    }
                }
            }
            slGroundManLog->AddTextWithDateTime(slGroundManValue->CommaText);
            slGroundManLog->MySaveToFile();

            slGroundManValue->Clear();
            delete slGroundManValue;
        }
        //<==
        //KenHsieh 20220728 : 新增GroundMan Value Log
    }
    else if(data[1]==50) //Read Alarm IO    0x32
    {
        for(i=0; i<8; i++)
        {
            if(data[i+3]==48)   //0X30
            {
                bRecordAlarmData[iAddress][i]=false;
            }
            else if(data[i+3]==49)   //0X31
            {
                bRecordAlarmData[iAddress][i]=true;
            }
        }
        bReaderOK[iAddress][eAlarmGet]=true;
    }
    else if(data[1]==51)    //0x33  讀取偵測板-內部組值
    {
        asBuf.sprintf("%c%c%c%c", data[3], data[4], data[5], data[6]);
        LabelBoardOhrmPtr[iAddress]->Caption=asBuf;
        sMsg.sprintf("Board[%d] BoardOhm %s",iAddress+1, asBuf);    //Sam 20230906 : 新增 GroundMan Msg Log
        bReaderOK[iAddress][eBoardOhm]=true;
    }
    else if(data[1]==52)    //0x34  偵測板Reset
    {
        sMsg.sprintf("Board[%d] BoardReset OK",iAddress+1);         //Sam 20230906 : 新增 GroundMan Msg Log
        bReaderOK[iAddress][eBoardReset]=true;
    }
    else if(data[1]==57)    //0x39  讀取偵測板-韌體版號
    {
        asBuf.sprintf("%c%c.%c%c%c%c.%c%c", data[3], data[4], data[5], data[6],
                                            data[7], data[8], data[9], data[10]);
        LabelBoardVersionPtr[iAddress]->Caption=asBuf;
        sMsg.sprintf("Board[%d] BoardVersion %s",iAddress+1, asBuf);    //Sam 20230906 : 新增 GroundMan Msg Log
        bReaderOK[iAddress][eBoardVersion]=true;
    }

    ShowGroundManLog(data, sMsg, false);  //Sam 20230906 : 新增 GroundMan Msg Log
}
//---------------------------------------------------------------------------
void __fastcall TfGroundMan::SetGroundMaster(int iAddress, int iMode)
{
    AnsiString asCRC, aCRC_Low, aCRC_High, asStrValue, asStr, S, S1;
    unsigned char cAddress[4]   ={0x30, 0x31, 0x32, 0x33};
    unsigned char cMode[6]      ={0x30, 0x31, 0x32, 0x33, 0x34, 0x39};
    unsigned char data[15]="";
    int iValue;
    AnsiString str="",sMsg="";

    if(iAddress<0 || iAddress>iUseGndBoard)
    {
        str.sprintf("GroundMan Address Error, Error Address=%d !", iAddress);
        ShowMyMessage(str);
        return;
    }

    if(iMode<0 || iMode>5)
    {
        str.sprintf("GroundMan Mode Error, Error Mode=%d !", iMode);
        ShowMyMessage(str);
        return;
    }

    if(HSys.iGroundManAlarmOhm>0 && HSys.iGroundManAlarmOhm<25)
    {
        iValue=HSys.iGroundManAlarmOhm*10;
    }
    else if(HSys.iGroundManAlarmOhm<=0)
    {
        iValue=10;
    }
    else if(HSys.iGroundManAlarmOhm>=25)
    {
        iValue=250;
    }
    else
    {
        iValue=30;
    }

    asStrValue.sprintf("0x%s", IntToHex(iValue,2));

    sprintf(data, ":%c%c%c%c%c%c%c%c%c%c",
        cMode[iMode],
        cAddress[iAddress],
        asStrValue.ToIntDef(0),
        asStrValue.ToIntDef(0),
        asStrValue.ToIntDef(0),
        asStrValue.ToIntDef(0),
        asStrValue.ToIntDef(0),
        asStrValue.ToIntDef(0),
        asStrValue.ToIntDef(0),
        asStrValue.ToIntDef(0));
    asCRC=calc_crc(data, strlen(data));
    asStr=asCRC.SubString(1, asCRC.Pos(",")-1);
    aCRC_Low="0x"+IntToHex(asStr.ToIntDef(0),2);
    asStr=asCRC;
    asStr.Delete(1, asCRC.Pos(","));
    aCRC_High="0x"+IntToHex(asStr.ToIntDef(0), 2);
    data[11]=aCRC_Low.ToIntDef(0);
    data[12]=aCRC_High.ToIntDef(0);
    data[13]=0x0D;
    data[14]=0x0A;
    comGM->WriteCommData(data, 15);
    sMsg.sprintf("Board[%d] %s",iAddress+1, sCMD[iMode]);   //Sam 20230906 : 新增 GroundMan Msg Log
    ShowGroundManLog(data, sMsg, true);                     //Sam 20230906 : 新增 GroundMan Msg Log
}
//---------------------------------------------------------------------------
AnsiString TfGroundMan::calc_crc(unsigned char *buf, int length)
{
    unsigned short crc = 0xFFFF;
    int i, j;
    unsigned char LSB;
    AnsiString asCrc;

    unsigned char cbuf2[11];

    memcpy(cbuf2, buf, sizeof(cbuf2));

    for(i=0; i<11; i++)
    {
        crc^=cbuf2[i];
        for(j=0; j<8; j++)
        {
            LSB= crc & 1;
            crc=crc >> 1;
            if (LSB)
                crc^=0xA001;
        }
    }
    asCrc.sprintf("%d,%d", ((crc&0xFF00)>>8), ((crc&0x0FF)));
    return asCrc;
}
//---------------------------------------------------------------------------
void __fastcall TfGroundMan::Timer1Timer(TObject *Sender)
{
    static bool bRunTimer1=false;                                           //jou 20240310 : timer加上保護

    if(InitialOK==false)
    {
        bRunTimer1=false;
        return;
    }

    if(bRunTimer1==true)
    {
        return;
    }

    bRunTimer1=true;

    if(USE_GROUND_MAN==1 && bRs232Ok)
    {
        DoGroundMasterMonitor();
    }
    labStatus->Caption=(AnsiString)iGroundMasterTask;

    bRunTimer1=false;
}
//---------------------------------------------------------------------------
void __fastcall TfGroundMan::DoGroundMasterMonitor()
{
    int &Task=iGroundMasterTask;
    static bool bFlag[6]={false, false, false, false, false, false}; //KaiChen 20200916 ：增加 GroundMan 功能 3->6
    static int iTimerOutRetry=0;
    static int iCommand=0;
    AnsiString asAlarmMes="";
    AnsiString asValue="";
    bool bAlarm=false;
    int i=0, j=0;
    int  iNowTime;
    DWORD dwStart;

    TLabel *LabelShowPtr[4][8]={
                                {labValue_0_0, labValue_0_1, labValue_0_2, labValue_0_3,
                                labValue_0_4, labValue_0_5, labValue_0_6, labValue_0_7},
                                {labValue_1_0, labValue_1_1, labValue_1_2, labValue_1_3,
                                labValue_1_4, labValue_1_5, labValue_1_6, labValue_1_7},
                                {labValue_2_0, labValue_2_1, labValue_2_2, labValue_2_3,
                                labValue_2_4, labValue_2_5, labValue_2_6, labValue_2_7},    //JerryYang 20210428 : Add 28點接地偵側
                                {labValue_3_0, labValue_3_1, labValue_3_2, labValue_3_3,
                                labValue_3_4, labValue_3_5, labValue_3_6, labValue_3_7}
                                };

    TMyLedLane *LedShowPtr[4][8]={
                                    {led_0_0, led_0_1, led_0_2, led_0_3, led_0_4, led_0_5, led_0_6, led_0_7},
                                    {led_1_0, led_1_1, led_1_2, led_1_3, led_1_4, led_1_5, led_1_6, led_1_7},
                                    {led_2_0, led_2_1, led_2_2, led_2_3, led_2_4, led_2_5, led_2_6, led_2_7},   //JerryYang 20210428 : Add 28點接地偵側
                                    {led_3_0, led_3_1, led_3_2, led_3_3, led_3_4, led_3_5, led_3_6, led_3_7}
                                 };

    TLabel *LabelShowCount[4][8]={
                                    {labCount_0_0, labCount_0_1, labCount_0_2, labCount_0_3,
                                     labCount_0_4, labCount_0_5, labCount_0_6, labCount_0_7},
                                    {labCount_1_0, labCount_1_1, labCount_1_2, labCount_1_3,
                                     labCount_1_4, labCount_1_5, labCount_1_6, labCount_1_7},
                                    {labCount_2_0, labCount_2_1, labCount_2_2, labCount_2_3,
                                     labCount_2_4, labCount_2_5, labCount_2_6, labCount_2_7},
                                    {labCount_3_0, labCount_3_1, labCount_3_2, labCount_3_3,
                                     labCount_3_4, labCount_3_5, labCount_3_6, labCount_3_7}
                                 };
    switch(Task)
    {
        case 1:     //初始化
            for(i=0; i<4; i++)  //Sam 20220308 : 修正8點偵測異常 iUseGndBoard --> 4
            {
                for(j=0; j<eGroundTotalCMD; j++)  //KaiChen 20200916 ：增加 GroundMan 功能 3->6
                {
                    bReaderOK[i][j]=true;
                }
            }
            for(i=0; i<6; i++)  //Sam 20220308 : 修正8點偵測異常
            {
                bFlag[i]=true;
            }

            for(i=0; i<iUseGndBoard; i++)
            {
                for(int j=0; j<eGroundTotalCMD; j++)  //KaiChen 20200916 ：增加 GroundMan 功能 3->6
                {
                    bReaderOK[i][j]=false;
                }

                for(j=0; j<8; j++)
                {
                    asRecordValueData[i][j]="0";
                    bRecordAlarmData[i][j]=false;
                    bRecordAlarmData_AddOffset[i][j]=false;
                    dRecordValueData_AddOffset[i][j]=0.0;
                }

                bFlag[i]=false; //Sam 20220308 : 修正8點偵測異常
            }

            if(bGroundManResetByStart==true && bGroundManReset==false)
            {
                Task=10;
            }
            else if(bInternalOhm==false)            //KenHsieh 20220803 : GroundMan流程及功能修改
            {
                Task=50;
            }
            else if(bVersion==false)
            {
                Task=70;
            }
            else
            {
                iMachineOhmRetry=0;
                Task=300;
            }
            break;

        case 10:    //0x34  偵測板Reset
            if(bReaderOK[0][eBoardReset]==false || bFlag[0]==false)
            {
                SetGroundMaster(0, eBoardReset);
                tTimerOutTimer.SetSecAndOn(6);      //JerryYang 20250120 : 3->6
                Task=20;
            }
            else if(bReaderOK[1][eBoardReset]==false || bFlag[1]==false)
            {
                SetGroundMaster(1, eBoardReset);
                tTimerOutTimer.SetSecAndOn(6);
                Task=20;
            }
            else if(bReaderOK[2][eBoardReset]==false || bFlag[2]==false)
            {
                SetGroundMaster(2, eBoardReset);
                tTimerOutTimer.SetSecAndOn(6);
                Task=20;
            }
            else if(bReaderOK[3][eBoardReset]==false || bFlag[3]==false)  //JerryYang 20210428 : Add 28點接地偵側
            {
                SetGroundMaster(3, eBoardReset);
                tTimerOutTimer.SetSecAndOn(6);
                Task=20;
            }
            iCommand=eBoardReset;
            break;
        case 20:
            if((iUseGndBoard==1 && bReaderOK[0][eBoardReset]) ||  //Sam 20220308 : 修正8點偵測異常
               (iUseGndBoard==3 && bReaderOK[0][eBoardReset] && bReaderOK[1][eBoardReset] && bReaderOK[2][eBoardReset]) ||
               (iUseGndBoard==4 && bReaderOK[0][eBoardReset] && bReaderOK[1][eBoardReset] && bReaderOK[2][eBoardReset] && bReaderOK[3][eBoardReset]))
            {
                for(i=0; i<iUseGndBoard; i++)
                {
                    bFlag[i]=false;
                }
                bGroundManReset=true;
                tDalayTimer.SetSecAndOn(8);
                Task=22;
            }
            else if(bFlag[0]==false)
            {
                if(bReaderOK[0][eBoardReset])
                {
                    bFlag[0]=true;
                    tDalayTimer.SetSecAndOn(8);
                    Task=22;
                }
            }
            else if(bFlag[1]==false)
            {
                if(bReaderOK[1][eBoardReset])
                {
                    bFlag[1]=true;
                    tDalayTimer.SetSecAndOn(8);
                    Task=22;
                }
            }
            else if(bFlag[2]==false)  //JerryYang 20210428 : Add 28點接地偵側
            {
                if(bReaderOK[2][eBoardReset])
                {
                    bFlag[2]=true;
                    tDalayTimer.SetSecAndOn(8);
                    Task=22;
                }
            }
            else if(bFlag[3]==false)    //KenHsieh 20220803 : GroundMan流程及功能修改
            {
                if(bReaderOK[3][eBoardReset])
                {
                    bFlag[3]=true;
                    tDalayTimer.SetSecAndOn(8);
                    Task=22;
                }
            }

            if(tTimerOutTimer.Off())
            {
                Task=900;
            }
            break;
        case 22:
            if(tDalayTimer.Off())
            {
                if((iUseGndBoard==1 && bReaderOK[0][eBoardReset]) ||                                      //Sam 20220308 : 修正8點偵測異常
                   (iUseGndBoard==3 && bReaderOK[0][eBoardReset] && bReaderOK[1][eBoardReset] && bReaderOK[2][eBoardReset]) ||
                   (iUseGndBoard==4 && bReaderOK[0][eBoardReset] && bReaderOK[1][eBoardReset] && bReaderOK[2][eBoardReset] && bReaderOK[3][eBoardReset]))
                {
                    Task=50;
                }
                else
                {
                    Task=10;
                }
            }
            break;

        case 50:    //0x33  讀取偵測板-內部組值
            if(bReaderOK[0][eBoardOhm]==false || bFlag[0]==false)
            {
                SetGroundMaster(0, eBoardOhm);
                tTimerOutTimer.SetSecAndOn(6);      //JerryYang 20250120 : 3->6
                Task=60;
            }
            else if(bReaderOK[1][eBoardOhm]==false || bFlag[1]==false)
            {
                SetGroundMaster(1, eBoardOhm);
                tTimerOutTimer.SetSecAndOn(6);
                Task=60;
            }
            else if(bReaderOK[2][eBoardOhm]==false || bFlag[2]==false)
            {
                SetGroundMaster(2, eBoardOhm);
                tTimerOutTimer.SetSecAndOn(6);
                Task=60;
            }
            else if(bReaderOK[3][eBoardOhm]==false || bFlag[3]==false)  //JerryYang 20210428 : Add 28點接地偵側
            {
                SetGroundMaster(3, eBoardOhm);
                tTimerOutTimer.SetSecAndOn(6);
                Task=60;
            }
            iCommand=eBoardOhm;
            break;

        case 60:
            if((iUseGndBoard==1 && bReaderOK[0][eBoardOhm]) ||                                      //Sam 20220308 : 修正8點偵測異常
               (iUseGndBoard==3 && bReaderOK[0][eBoardOhm] && bReaderOK[1][eBoardOhm] && bReaderOK[2][eBoardOhm]) ||
               (iUseGndBoard==4 && bReaderOK[0][eBoardOhm] && bReaderOK[1][eBoardOhm] && bReaderOK[2][eBoardOhm] && bReaderOK[3][eBoardOhm]))
            {
                for(i=0; i<iUseGndBoard; i++)
                {
                    bFlag[i]=false;
                }
                bInternalOhm=true;      //KenHsieh 20220803 : GroundMan流程及功能修改
                Task=70;
            }
            else if(bFlag[0]==false)
            {
                if(bReaderOK[0][eBoardOhm])
                {
                    bFlag[0]=true;
                    Task=50;
                }
            }
            else if(bFlag[1]==false)
            {
                if(bReaderOK[1][eBoardOhm])
                {
                    bFlag[1]=true;
                    Task=50;
                }
            }
            else if(bFlag[2]==false)  //JerryYang 20210428 : Add 28點接地偵側
            {
                if(bReaderOK[2][eBoardOhm])
                {
                    bFlag[2]=true;
                    Task=50;
                }
            }
            else if(bFlag[3]==false)  //KenHsieh 20220803 : GroundMan流程及功能修改
            {
                if(bReaderOK[3][eBoardOhm])
                {
                    bFlag[3]=true;
                    Task=50;
                }
            }

            if(tTimerOutTimer.Off())
            {
                Task=900;
            }
            break;

        case 70:    //0x39  讀取偵測板-韌體版號
            if(bReaderOK[0][eBoardVersion]==false || bFlag[0]==false)
            {
                SetGroundMaster(0, eBoardVersion);
                tTimerOutTimer.SetSecAndOn(6);      //JerryYang 20250120 : 3->6
                Task=80;
            }
            else if(bReaderOK[1][eBoardVersion]==false || bFlag[1]==false)
            {
                SetGroundMaster(1, eBoardVersion);
                tTimerOutTimer.SetSecAndOn(6);
                Task=80;
            }
            else if(bReaderOK[2][eBoardVersion]==false || bFlag[2]==false)
            {
                SetGroundMaster(2, eBoardVersion);
                tTimerOutTimer.SetSecAndOn(6);
                Task=80;
            }
            else if(bReaderOK[3][eBoardVersion]==false || bFlag[3]==false)  //JerryYang 20210428 : Add 28點接地偵側
            {
                SetGroundMaster(3, eBoardVersion);
                tTimerOutTimer.SetSecAndOn(6);
                Task=80;
            }
            iCommand=eBoardVersion;
            break;

        case 80:
            if((iUseGndBoard==1 && bReaderOK[0][eBoardVersion]) ||                                      //Sam 20220308 : 修正8點偵測異常
               (iUseGndBoard==3 && bReaderOK[0][eBoardVersion] && bReaderOK[1][eBoardVersion] && bReaderOK[2][eBoardVersion]) ||
               (iUseGndBoard==4 && bReaderOK[0][eBoardVersion] && bReaderOK[1][eBoardVersion] && bReaderOK[2][eBoardVersion] && bReaderOK[3][eBoardVersion]))
            {
                for(i=0; i<iUseGndBoard; i++)   //Sam 20220308 : 修正8點偵測異常
                {
                    bFlag[i]=false;
                }
                bVersion=true;      //KenHsieh 20220803 : GroundMan流程及功能修改
                Task=300;
            }
            else if(bFlag[0]==false)
            {
                if(bReaderOK[0][eBoardVersion])
                {
                    bFlag[0]=true;
                    Task=70;
                }
            }
            else if(bFlag[1]==false)
            {
                if(bReaderOK[1][eBoardVersion])
                {
                    bFlag[1]=true;
                    Task=70;
                }
            }
            else if(bFlag[2]==false)  //JerryYang 20210428 : Add 28點接地偵側
            {
                if(bReaderOK[2][eBoardVersion])
                {
                    bFlag[2]=true;
                    Task=70;
                }
            }
            else if(bFlag[3]==false)  //KenHsieh 20220803 : GroundMan流程及功能修改
            {
                if(bReaderOK[3][eBoardVersion])
                {
                    bFlag[3]=true;
                    Task=70;
                }
            }

            if(tTimerOutTimer.Off())
            {
                Task=900;
            }
            break;

        case 100:   //0x30  硬體Alarm值設定 => 不需要，以HandlerSys設定之Ohm做判斷
            if(bReaderOK[0][eAlarmSet]==false || bFlag[0]==false)
            {
                SetGroundMaster(0, eAlarmSet);
                tTimerOutTimer.SetSecAndOn(6);      //JerryYang 20250120 : 3->6
                Task=200;
            }
            else if(bReaderOK[1][eAlarmSet]==false || bFlag[1]==false)
            {
                SetGroundMaster(1, eAlarmSet);
                tTimerOutTimer.SetSecAndOn(6);
                Task=200;
            }
            else if(bReaderOK[2][eAlarmSet]==false || bFlag[2]==false)
            {
                SetGroundMaster(2, eAlarmSet);
                tTimerOutTimer.SetSecAndOn(6);
                Task=200;
            }
            else if(bReaderOK[3][eAlarmSet]==false || bFlag[3]==false)  //JerryYang 20210428 : Add 28點接地偵側
            {
                SetGroundMaster(3, eAlarmSet);
                tTimerOutTimer.SetSecAndOn(6);
                Task=200;
            }
            iCommand=eAlarmSet;
            break;

        case 200:
            if((iUseGndBoard==1 && bReaderOK[0][eAlarmSet]) ||                                      //Sam 20220308 : 修正8點偵測異常
               (iUseGndBoard==3 && bReaderOK[0][eAlarmSet] && bReaderOK[1][eAlarmSet] && bReaderOK[2][eAlarmSet]) ||
               (iUseGndBoard==4 && bReaderOK[0][eAlarmSet] && bReaderOK[1][eAlarmSet] && bReaderOK[2][eAlarmSet] && bReaderOK[3][eAlarmSet]))
            {
                for(i=0; i<iUseGndBoard; i++)   //Sam 20220308 : 修正8點偵測異常
                {
                    bFlag[i]=false;
                }
                Task=300;
            }
            else if(bFlag[0]==false)
            {
                if(bReaderOK[0][eAlarmSet])
                {
                    bFlag[0]=true;
                    Task=100;
                }
            }
            else if(bFlag[1]==false)
            {
                if(bReaderOK[1][eAlarmSet])
                {
                    bFlag[1]=true;
                    Task=100;
                }
            }
            else if(bFlag[2]==false)  //JerryYang 20210428 : Add 28點接地偵側
            {
                if(bReaderOK[2][eAlarmSet])
                {
                    bFlag[2]=true;
                    Task=100;
                }
            }
            else if(bFlag[3]==false)  //KenHsieh 20220803 : GroundMan流程及功能修改
            {
                if(bReaderOK[3][eAlarmSet])
                {
                    bFlag[3]=true;
                    Task=100;
                }
            }

            if(tTimerOutTimer.Off())
            {
                Task=900;
            }
            break;

        case 300:   //0x31  阻值詢問
            if(bReaderOK[0][eMachineOhm]==false || bFlag[0]==false)
            {
                bReaderOK[0][eMachineOhm]=false;
                SetGroundMaster(0, eMachineOhm);
                tTimerOutTimer.SetSecAndOn(6);      //JerryYang 20250120 : 3->6
                Task=400;
            }
            else if(bReaderOK[1][eMachineOhm]==false || bFlag[1]==false)
            {
                bReaderOK[1][eMachineOhm]=false;
                SetGroundMaster(1, eMachineOhm);
                tTimerOutTimer.SetSecAndOn(6);
                Task=400;
            }
            else if(bReaderOK[2][eMachineOhm]==false || bFlag[2]==false)
            {
                bReaderOK[2][eMachineOhm]=false;
                SetGroundMaster(2, eMachineOhm);
                tTimerOutTimer.SetSecAndOn(6);
                Task=400;
            }
            else if(bReaderOK[3][eMachineOhm]==false || bFlag[3]==false)  //JerryYang 20210428 : Add 28點接地偵側
            {
                bReaderOK[3][eMachineOhm]=false;
                SetGroundMaster(3, eMachineOhm);
                tTimerOutTimer.SetSecAndOn(6);
                Task=400;
            }
            iCommand=eMachineOhm;
            break;

        case 400:
            if((iUseGndBoard==1 && bReaderOK[0][eMachineOhm]) ||                                      //Sam 20220308 : 修正8點偵測異常
               (iUseGndBoard==3 && bReaderOK[0][eMachineOhm] && bReaderOK[1][eMachineOhm] && bReaderOK[2][eMachineOhm]) ||
               (iUseGndBoard==4 && bReaderOK[0][eMachineOhm] && bReaderOK[1][eMachineOhm] && bReaderOK[2][eMachineOhm] && bReaderOK[3][eMachineOhm]))
            {
                for(i=0; i<iUseGndBoard; i++)
                {
                    bFlag[i]=false;
                }
                Task=990;       //KenHsieh 20220803 : GroundMan流程及功能修改
            }
            else if(bFlag[0]==false)
            {
                if(bReaderOK[0][eMachineOhm])
                {
                    iMachineOhmRetry=0;
                    bFlag[0]=true;
                    Task=300;
                }
            }
            else if(bFlag[1]==false)
            {
                if(bReaderOK[1][eMachineOhm])
                {
                    iMachineOhmRetry=0;
                    bFlag[1]=true;
                    Task=300;
                }
            }
            else if(bFlag[2]==false)  //JerryYang 20210428 : Add 28點接地偵側
            {
                if(bReaderOK[2][eMachineOhm])
                {
                    iMachineOhmRetry=0;
                    bFlag[2]=true;
                    Task=300;
                }
            }
            else if(bFlag[3]==false)  //KenHsieh 20220803 : GroundMan流程及功能修改
            {
                if(bReaderOK[3][eMachineOhm])
                {
                    iMachineOhmRetry=0;
                    bFlag[3]=true;
                    Task=300;
                }
            }

            if(tTimerOutTimer.Off())
            {
                Task=900;
            }
            break;

        case 500:   //0x32  警報詢問 => 不需要，以HandlerSys設定之Ohm做判斷
            if(bReaderOK[0][eAlarmGet]==false || bFlag[0]==false)
            {
                SetGroundMaster(0, eAlarmGet);
                tTimerOutTimer.SetSecAndOn(6);      //JerryYang 20250120 : 3->6
                Task=600;
            }
            else if(bReaderOK[1][eAlarmGet]==false || bFlag[1]==false)
            {
                SetGroundMaster(1, eAlarmGet);
                tTimerOutTimer.SetSecAndOn(6);
                Task=600;
            }
            else if(bReaderOK[2][eAlarmGet]==false || bFlag[2]==false)
            {
                SetGroundMaster(2, eAlarmGet);
                tTimerOutTimer.SetSecAndOn(6);
                Task=600;
            }
            else if(bReaderOK[3][eAlarmGet]==false || bFlag[3]==false)  //JerryYang 20210428 : Add 28點接地偵側
            {
                SetGroundMaster(3, eAlarmGet);
                tTimerOutTimer.SetSecAndOn(6);
                Task=600;
            }
            iCommand=eAlarmGet;
            break;

        case 600:
            if((iUseGndBoard==1 && bReaderOK[0][eAlarmGet]) ||                                      //Sam 20220308 : 修正8點偵測異常
               (iUseGndBoard==3 && bReaderOK[0][eAlarmGet] && bReaderOK[1][eAlarmGet] && bReaderOK[2][eAlarmGet]) ||
               (iUseGndBoard==4 && bReaderOK[0][eAlarmGet] && bReaderOK[1][eAlarmGet] && bReaderOK[2][eAlarmGet] && bReaderOK[3][eAlarmGet]))
            {
                for(i=0; i<iUseGndBoard; i++)
                {
                    bFlag[i]=false;
                }
                Task=990;
            }
            else if(bFlag[0]==false)
            {
                if(bReaderOK[0][eAlarmGet])
                {
                    bFlag[0]=true;
                    Task=500;
                }
            }
            else if(bFlag[1]==false)
            {
                if(bReaderOK[1][eAlarmGet])
                {
                    bFlag[1]=true;
                    Task=500;
                }
            }
            else if(bFlag[2]==false)  //JerryYang 20210428 : Add 28點接地偵側
            {
                if(bReaderOK[2][eAlarmGet])
                {
                    bFlag[2]=true;
                    Task=500;
                }
            }
            else if(bFlag[3]==false)
            {
                if(bReaderOK[3][eAlarmGet])
                {
                    bFlag[3]=true;
                    Task=500;
                }
            }

            if(tTimerOutTimer.Off())
            {
                Task=900;
            }
            break;

        case 900:
            if(iCommand==eMachineOhm && iMachineOhmRetry<5)      //Sam 20230908 : 讀取阻抗獨立 Retry 機制
            {
                iMachineOhmRetry++;
                Task=300;
            }
            else if(iCommand!=eMachineOhm && iTimerOutRetry<2)
            {
                iTimerOutRetry++;
                Task=1;
            }
            else
            {
                iTimerOutRetry=0;
                iMachineOhmRetry=0;      //Sam 20230908 : 讀取阻抗獨立 Retry 機制
                //ShowMyMessage("GroundMaster Timer Out Error, Please Check!");
                asAlarmMes.sprintf("%s reply timer out %s %s %s %s",sCMD[iCommand], BoolToStr(bFlag[0]), BoolToStr(bFlag[1]), BoolToStr(bFlag[2]), BoolToStr(bFlag[3]));    //Sam 20230906 : 新增 GroundMan Msg Log
                ShowErrorMessage("WAR1609", K_RETRY, MMSystem, false, asAlarmMes);  //Sam 20230906 : 新增 GroundMan Msg Log
                if(CUSTOMER_CODE==CC_HONPREC_QC)                                //Sam 20241203 : QC 模式不用等待
                {
                    Task=1;
                }
                else
                {
                    tDalayTimer.SetSecAndOn(60);                                //Sam 20231128 :  報警完需要等60秒之後再重新偵測
                    Task=3000;
                }
            }
            break;
        case 990:
            if(bUseOffset==true)
            {
                Task=1800;
            }
            else
            {
                Task=1000;
            }
            break;

        case 1000:  //異常判斷改為程式判斷  //KenHsieh 20220803 : GroundMan流程及功能修改
            for(i=0; i<iUseGndBoard; i++)
            {
                for(j=0; j<8; j++)
                {
                    if(asRecordValueData[i][j]=="> 10")
                    {
                        bRecordAlarmData[i][j]=true;
                    }

                    if(atoi(asRecordValueData[i][j].c_str())>=10.0)
                    {
                        asRecordValueData[i][j]="10.0";
                        bRecordAlarmData[i][j]=true;
                    }
                    else if(atoi(asRecordValueData[i][j].c_str())<=0.1)
                    {
                        asRecordValueData[i][j]="0.1";
                    }
                    else
                    {
                        if(atoi(asRecordValueData[i][j].c_str())>HSys.iGroundManAlarmOhm)
                        {
                            bRecordAlarmData[i][j]=true;
                        }
                    }
                }
            }
            Task=1100;
            break;
        case 1100:
            iTimerOutRetry=0;
            for(i=0; i<iUseGndBoard; i++)
            {
                for(j=0; j<8; j++)
                {
                    LabelShowPtr[i][j]->Caption=asRecordValueData[i][j];
                    LedShowPtr[i][j]->Value=!bRecordAlarmData[i][j];

                    if(bOpenClose[i][j] && bRecordAlarmData[i][j]==true)
                    {
//                        asAlarmMes=asAlarmMes+", "+asShowName[i][j];
//                        bAlarm=true;
                        //Ifor 20201112 add:Ground Alarm 模式改設定時間內連續發生設定次數才Alarm
                        //==>
                        dwStart = GetTickCount();
                        iNowTime = dwStart;
                        iGroundManAlarm_HappenCount[i][j]++;
                        if(bGroundManAlarm_FirstTime[i][j]==true)
                        {
                            bGroundManAlarm_FirstTime[i][j]=false;
                            iGroundManAlarm_StartTime[i][j]=dwStart;
                        }
                        else
                        {
                            if(iNowTime-iGroundManAlarm_StartTime[i][j] >= Alarm_Continuous_Time*1000)
                            {
                                iNowTime = iNowTime-iGroundManAlarm_StartTime[i][j];
                                bGroundManAlarm_FirstTime[i][j]=true;     //時間大於設定時間重置
                                iGroundManAlarm_HappenCount[i][j]=0;
                            }
                            else
                            {
                                if(iGroundManAlarm_HappenCount[i][j]>=Alarm_Occurrences)
                                {
                                    bGroundManAlarm_FirstTime[i][j]=true;
                                    iGroundManAlarm_HappenCount[i][j]=0;
                                    asAlarmMes=asAlarmMes+", "+asShowName[i][j] + "(" + asRecordValueData[i][j] +")";   //Ifor 20201102 add: 新增Ground Man Alarm 時後面帶目前阻值
                                    bAlarm=true;
                                }
                            }
                        }
                        LabelShowCount[i][j]->Caption=iGroundManAlarm_HappenCount[i][j];
                        //<==
                        //Ifor 20201112 add:Ground Alarm 模式改設定時間內連續發生設定次數才Alarm
                    }
                }
            }

            if(CUSTOMER_CODE==CC_SIGURD_PeiXing)                                //Sam 20250122 : 北興廠新增維修模式，不需要報警要一直顯示阻值
            {
                if(btnMaintenanceMode->Down==true)
                    bAlarm=false;
            }

            if(bAlarm)
            {
                StopAllMotor();
                ShowErrorMessage("WAR1609", K_RETRY, MMSystem, false, asAlarmMes);
                SystemStart=false;
                if(CUSTOMER_CODE==CC_HONPREC_QC)                                //Sam 20241203 : QC 模式不用等待
                {
                    Task=1;
                }
                else
                {
                    tDalayTimer.SetSecAndOn(60);                                //Sam 20231128 :  報警完需要等60秒之後再重新偵測
                    Task=3000;
                }
                break;
            }
            Task=1;
            break;

        case 1800:
            for(i=0; i<iUseGndBoard; i++)
            {
                for(j=0; j<8; j++)
                {
                    if(dRecordValueData_AddOffset[i][j]>=10.0)
                    {
                        dRecordValueData_AddOffset[i][j]=10.0;
                        bRecordAlarmData_AddOffset[i][j]=true;
                    }
                    else if(dRecordValueData_AddOffset[i][j]<=0.1)
                    {
                        dRecordValueData_AddOffset[i][j]=0.1;
                    }
                    else
                    {
                        if(dRecordValueData_AddOffset[i][j]>HSys.iGroundManAlarmOhm)
                        {
                            bRecordAlarmData_AddOffset[i][j]=true;
                        }
                    }
                }
            }
            Task=2000;
            break;
        case 2000:
            iTimerOutRetry=0;
            for(i=0; i<iUseGndBoard; i++)
            {
                for(j=0; j<8; j++)
                {
                    asValue.sprintf("%2.1f", dRecordValueData_AddOffset[i][j]);
                    LabelShowPtr[i][j]->Caption=asValue;
                    LedShowPtr[i][j]->Value=!bRecordAlarmData_AddOffset[i][j];

                    if(bOpenClose[i][j] && bRecordAlarmData_AddOffset[i][j]==true)
                    {
//                        asAlarmMes=asAlarmMes+", "+asShowName[i][j] + "(" + asRecordValueData[i][j] +")";   //Ifor 20201102 add: 新增Ground Man Alarm 時後面帶目前阻值
//                        bAlarm=true;
                        //Ifor 20201112 add:Ground Alarm 模式改設定時間內連續發生設定次數才Alarm
                        //==>
                        dwStart = GetTickCount();
                        iNowTime = dwStart;
                        iGroundManAlarm_HappenCount[i][j]++;
                        if(bGroundManAlarm_FirstTime[i][j]==true)
                        {
                            bGroundManAlarm_FirstTime[i][j]=false;
                            iGroundManAlarm_StartTime[i][j]=dwStart;
                        }
                        else
                        {
                            if(iNowTime-iGroundManAlarm_StartTime[i][j] >= Alarm_Continuous_Time*1000)
                            {
                                iNowTime = iNowTime-iGroundManAlarm_StartTime[i][j];
                                bGroundManAlarm_FirstTime[i][j]=true; //時間大於設定時間重置
                                iGroundManAlarm_HappenCount[i][j]=0;
                            }
                            else
                            {
                                if(iGroundManAlarm_HappenCount[i][j]>=Alarm_Occurrences)
                                {
                                    bGroundManAlarm_FirstTime[i][j]=true;
                                    iGroundManAlarm_HappenCount[i][j]=0;
                                    asAlarmMes=asAlarmMes+", "+asShowName[i][j] + "(" + dRecordValueData_AddOffset[i][j] +")";   //Ifor 20201102 add: 新增Ground Man Alarm 時後面帶目前阻值
                                    bAlarm=true;
                                }
                            }
                        }
                        //<==
                        //Ifor 20201112 add:Ground Alarm 模式改設定時間內連續發生設定次數才Alarm
                    }
                    LabelShowCount[i][j]->Caption=iGroundManAlarm_HappenCount[i][j];
                }
            }

            if(CUSTOMER_CODE==CC_SIGURD_PeiXing)                                //Sam 20250122 : 北興廠新增維修模式，不需要報警要一直顯示阻值
            {
                if(btnMaintenanceMode->Down==true)
                    bAlarm=false;
            }

            if(bAlarm)
            {
                ShowErrorMessage("WAR1609", K_RETRY, MMSystem, false, asAlarmMes);
                if(CUSTOMER_CODE==CC_HONPREC_QC)                                //Sam 20241203 : QC 模式不用等待
                {
                    Task=1;
                }
                else
                {
                    tDalayTimer.SetSecAndOn(60);                                //Sam 20231128 :  報警完需要等60秒之後再重新偵測
                    Task=3000;
                }
                break;
            }
            Task=1;
            break;
        case 3000:                                                              //Sam 20231128 :  報警完需要等60秒之後再重新偵測
            if(tDalayTimer.Off())
            {
                Task=1;
            }
            break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfGroundMan::spbSaveClick(TObject *Sender)
{
    if(IniConfig.bA02DisableSaveParsWhenSwitchToOp==true &&                     //RogerYang 20260305 張寧說時間到都要登出，且不可被更改任何設定(A01_2)
        AccessLevel==0)
    {
        ShowMyMessage("[A01_2]目前已切換到Operator權限，\r\n請重新登入再做設定!");
        Close();
    }

    AnsiString sPathName, sFileName;
    AnsiString asOffset="", asTTT="";
    AnsiString aEn="",aCh="";                                                   //Sam 20220105 : GroundMan Alarm Settring 增加保護，避免檢查時間太短無法報警
    sPathName.sprintf("D:\\HT9045\\system");

    sFileName.sprintf("%s\\GroundMan.ini", sPathName);
    MyForceDirectories(sPathName);

    int iAlarm_Continuous_Time=atoi(edContinuous_Time->Text.c_str());
    int iAlarm_Occurrences=atoi(edOccurrences->Text.c_str());

    //Sam 20220105 : GroundMan Alarm Settring 增加保護，避免檢查時間太短無法報警
    //==>
    if(iAlarm_Occurrences<=0)
        iAlarm_Occurrences=1;
    int iT=iAlarm_Continuous_Time/iAlarm_Occurrences;
    if(iT<5)
    {
        aEn.sprintf("ContinuousTime/Occurrences Must be more than 5");
        aCh.sprintf("ContinuousTime/Occurrences 必須大於 5");
        ShowMyMessage(aEn, aCh);
        return;
    }
    //<==
    //Sam 20220105 : GroundMan Alarm Settring 增加保護，避免檢查時間太短無法報警

    /*   //Sam 20220105 : GroundMan Alarm Settring 增加保護，避免檢查時間太短無法報警 Mark
    int iMax_Occurrences=iAlarm_Continuous_Time*3;

    if(iAlarm_Occurrences>iMax_Occurrences)
        iAlarm_Occurrences=iMax_Occurrences;
    */
    if(CUSTOMER_CODE==CC_KYEC_LEE)
    {
        iAlarm_Continuous_Time=15;
        iAlarm_Occurrences=2;
    }

    WriteIniData(sFileName, "System",   "Alarm_Continuous_Time",  iAlarm_Continuous_Time);     //Alarm 允許時間
    WriteIniData(sFileName, "System",   "Alarm_Occurrences",      iAlarm_Occurrences);         //Alarm 允許次數
    ReadGroundOffset();
}
//---------------------------------------------------------------------------

void __fastcall TfGroundMan::sbtExitClick(TObject *Sender)
{
    sbtExit->Down=false;
    Close();
    fShow=false;
}
//---------------------------------------------------------------------------

void TfGroundMan::ShowGroundManLog(unsigned char ucLog[15],AnsiString sMsg, bool bSend) //Sam 20230906 : 新增 GroundMan Msg Log
{
    AnsiString sFileName="", asLog="", sMegTime="", asSend="", asRawLog="", asHexLog="";

    GetTimeInfo();
    sFileName.sprintf("D:\\HT9045_Log\\GroundManLog\\%04d\\%02d\\%02d", SystemYear, SystemMonth, SystemDate);
    MyForceDirectories(sFileName);
    sFileName.sprintf("D:\\HT9045_Log\\GroundManLog\\%04d\\%02d\\%02d\\%04d%02d%02d-%02d.txt", SystemYear, SystemMonth, SystemDate, SystemYear, SystemMonth, SystemDate, SystemHour);
    sMegTime.sprintf("%04d-%02d-%02d %02d:%02d:%02d %03d",
                        SystemYear, SystemMonth, SystemDate, SystemHour, SystemMin, SystemSec, SystemMSec);

    if(bSend)
    {
        mmGroundManLog->Lines->Add("");
        WriteDataToFile(sFileName.c_str() , "");
        asSend="Send ==>";
    }
    else
    {
        asSend="Recv <==";
    }

    for(int i=0; i<15; i++)
    {
        asHexLog.sprintf("%02X", ucLog[i]);        //記錄16進制
        asRawLog=asRawLog+"_"+asHexLog;
    }

    asLog.sprintf("%s,  %s  %s  %s",
        sMegTime,
        asSend,
        asRawLog,           //紀錄原始 RawData
        sMsg);              //記錄人看得懂的命令方便 Debug

        mmGroundManLog->Lines->Add(asLog);
        WriteDataToFile(sFileName.c_str() , asLog.c_str());

        if(mmGroundManLog->Lines->Count>1024)
            mmGroundManLog->Clear();

    //Sam 20211223 : 每顆 IC 測試完畢都要記錄當時的 Ground & ESD 數值。
    //==>
    /*
    AnsiString asGround="";
    for(int i=0;3<i;i++)
        for(int j=0;8<j;j++)
            asGround+=asRecordValueData[i][j]+"-";
    asGroundVaule=asGround;
    */
    //<==
    //Sam 20211223 : 每顆 IC 測試完畢都要記錄當時的 Ground & ESD 數值。

    //KenHsieh 20220728 : 新增GroundMan Value Log
    //==>
    /*
    asMold.sprintf("%02X", ucLog[1]);
    asAdress.sprintf("%02X", ucLog[2]);
    if(bSend==false && asMold=="31" &&
      ((HSys.iGroundManScanPoint==0 && asAdress=="30") ||  //8點讀取最後一站
       (HSys.iGroundManScanPoint==1 && asAdress=="32") ||  //22點讀取最後一站
       (HSys.iGroundManScanPoint==2 && asAdress=="33")))   //28點讀取最後一站
    {
        TStringList *slGroundManValue;
        slGroundManValue=new TStringList();

        for(int i=0; i<iUseGndBoard; i++)
        {
            for(int j=0; j<8; j++)
            {
                if(bOpenClose[i][j])
                {
                    if(bUseOffset==true)
                        slGroundManValue->Add(dRecordValueData_AddOffset[i][j]);
                    else
                        slGroundManValue->Add(asRecordValueData[i][j]);
                }
                else
                {
                    slGroundManValue->Add("");
                }
            }
        }
        slGroundManLog->AddTextWithDateTime(slGroundManValue->CommaText);
        slGroundManLog->MySaveToFile();

        slGroundManValue->Clear();
        delete slGroundManValue;
    }
    */
    //<==
    //KenHsieh 20220728 : 新增GroundMan Value Log
}
//---------------------------------------------------------------------------
void TfGroundMan::ReadGroundOffset()
{
    AnsiString sPathName, sFileName;
    AnsiString asOffset="", asTTT="";

    sPathName.sprintf("D:\\HT9045\\system");
    MyForceDirectories(sPathName);
    sFileName.sprintf("%s\\GroundMan.ini", sPathName);

    bUseOffset=CheckAndReadIniData(sFileName, "System",    "UseOffset",    true);

    if(bUseOffset==true)
        labUseOffset->Caption="Use Offset：ON";
    else
        labUseOffset->Caption="Use Offset：OFF";

    bGroundManResetByStart=CheckAndReadIniData(sFileName, "System",    "UseResetByStart",    true);

    if(bGroundManResetByStart==true)
        labResetByStart->Caption="Use Reset：ON";
    else
        labResetByStart->Caption="Use Reset：OFF";

    //Ifor 20201109 add: Ground Man Alarm 計數
    //==>
    Alarm_Continuous_Time   =CheckAndReadIniData(sFileName, "System",    "Alarm_Continuous_Time",   15);  //Sam 20220104 :  2S > 15S    //Alarm 允許時間
    Alarm_Occurrences       =CheckAndReadIniData(sFileName, "System",    "Alarm_Occurrences",   2);   //Sam 20220104 :  6 > 2           //Alarm 允許時間
    if(CUSTOMER_CODE==CC_KYEC_LEE)
    {
        Alarm_Continuous_Time=15;
        Alarm_Occurrences=2;
    }
    edContinuous_Time->Text=Alarm_Continuous_Time;
    edOccurrences->Text=Alarm_Occurrences;
    //<==
    //Ifor 20201109 add: Ground Man Alarm 計數

    for(int i=0; i<iUseGndBoard; i++)
    {
        for(int j=0; j<8; j++)
        {
            asTTT.sprintf("Board_%d_Offset", i+1);
//            asOffset.sprintf("%d_%d", i+1, j+1);

            dOffset[i][j]=CheckAndReadIniData(sFileName, asTTT,    asShowName[i][j],    0.0);
            bGroundManAlarm_FirstTime[i][j]=true; //初始化
            iGroundManAlarm_HappenCount[i][j]=0;  //初始化
        }
    }
}
//---------------------------------------------------------------------------   //Sam 20220105 : GroundMan Alarm Settring 增加保護，避免檢查時間太短無法報警
void __fastcall TfGroundMan::edOccurrencesMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 1, true, 1, 10);
}
//---------------------------------------------------------------------------   //Sam 20220105 : GroundMan Alarm Settring 增加保護，避免檢查時間太短無法報警
void __fastcall TfGroundMan::edContinuous_TimeMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit *)Sender, N_INTEGER, 5, true, 5, 50);
}
//---------------------------------------------------------------------------
void TfGroundMan::ReStart()                                                     //Sam 20220107 : 矽格北興 Initail Start 要重啟 GroundMan
{
    #ifdef SOFT_SIMULTE
        return;
    #else
    if(USE_GROUND_MAN)
    {
        comGM->StopComm();
        bRs232Ok=false;
        Init_GM_RS232();
    }
    #endif
}
//---------------------------------------------------------------------------