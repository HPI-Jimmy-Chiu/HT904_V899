#include "MachineDefine.h"
#pragma hdrstop

#include "MyCCLinkSensor.h"
//#include "INPUT.h"
#include "common.h"
#include "cmydef.h"
#include "myQwertyKeyBoard.h"
//#include "inifiles.hpp"
#include "cprod.h"
#include "ShuttleMove.h"
#include "csystem.h"
#include "mymessbox.h"
#include "MyKitSuck.h"
#include "cSecurity.h"
#include "note.h"
#include "main.h"
#include "cMyDNM100UD.h"
#include "MyEtherCAT.h"                                                         //Sam 20230707 : EtherCAT Shuttle sensor
#include "mysensor.h"
#include "cinitial.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ALed"
#pragma link "HTray"
#pragma resource "*.dfm"
TfCCLink *fCCLink;
char   cSensorStatus[5][10]={"OK", "Error", "No Use"};                          //kevin 20130420
int iSensorMax =8;                                                              //單一個shuttle
int iErrorCnt=0;                                                                //JerryYang 20160729 連線異常秒數
int iAutoRetryCnt=0;                                                            //JerryYang 20160729 重新連線次數
int iRotateStart=0;
int iColorStart=0;
short sSen=0;
TPanel *AmpAddress[16];
//---------------------------------------------------------------------------
__fastcall TfCCLink::TfCCLink(TComponent* Owner)
    : TForm(Owner)
{
    iCanBusSrachTask=1;                                                         //Sam 20210518 : 新增 CanBus 軟體配置
    grpCanBusSeach->Visible=(SHUTTLE_SENSOR_TYPE==eSensorCanBus ||
                             SHUTTLE_SENSOR_TYPE==eSensorCanBus3);              //Sam 20210518 : 新增 CanBus 軟體配置
    GetStationStart();                                                          //JerryYang 20200327 取得socket/ rotate/ color起始站號
    //Sensor數量---------------------------------------------
    if(MachineTypeChoice==Type_HT9045 && UseSensorMod3())                       //Sam 20230707 : EtherCAT Shuttle sensor //Steven 20131008 : for HT9046AH
    {
        iSensorNo[0]=12;
        iSensorNo[1]=12;
    }
    else
    {
        iSensorNo[0]=16;
        iSensorNo[1]=16;
    }
    iSensorNo[2]=1;       iSensorNo[3]=1;                                       //Input的最後一顆
    iSensorNo[4]=1;       iSensorNo[5]=1;                                       //Output的第一顆
    if(USE_COLOR_TRAY_SENSOR && COLOR_AMP_QTY>0)
    {
        iSensorNo[6]=4;                                                         //kevin 20130420 socket sensor
    }
    else
    {
        iSensorNo[6]=8;                                                         //kevin 20130420 socket sensor
    }
    iSensorNo[7]=4;                                                             //Rotate
    iSensorNo[8]=6;                                                             //wei 20180409 color sensor

    iSensorNo[9]=8;                                                             //Rotate
    iSensorNo[10]=8;                                                            //wei 20180409 color sensor

//    iSensorNo[8]=1;       iSensorNo[9]=1;                                     //kevin 20130420 socket sensor
    //顯示區---------------------------------------------
    TMyTrayPtr[0]=mtIn1;            TMyTrayPtr[1]=mtIn2;
    TMyTrayPtr[2]=mtIn3;            TMyTrayPtr[3]=mtIn4;
    TMyTrayPtr[4]=mtOut1;           TMyTrayPtr[5]=mtOut2;
    TMyTrayPtr[6]=mtInSocketBase;                                               //kevin 20130420 socket sensor
    TMyTrayPtr[7]=mtInOutRotateSen;                                             //Rotate
    TMyTrayPtr[8]=mtColorSen;                                                   //wei 20180409 color sensor
    TMyTrayPtr[9]=mtInSocketBase9_16;                                           //JerryYang 20250501 
    TMyTrayPtr[10]=mtInSocketBase17_24;
    //輸入框---------------------------------------------
    if((MachineTypeChoice==Type_HT9045 ||
        MachineTypeChoice==Type_HT9045_12Site) &&                               //Steven 20131008 : for HT9046AH
       UseSensorMod3())                                                         //Sam 20230707 : EtherCAT Shuttle sensor
    {
        EditPtr[ 0]=edIn2_6;      EditPtr[ 1]=edIn2_5;
        EditPtr[ 2]=edIn2_4;      EditPtr[ 3]=edIn2_3;
        EditPtr[ 4]=edIn2_2;      EditPtr[ 5]=edIn2_1;
        EditPtr[ 6]=edIn2_8;      EditPtr[ 7]=edIn2_7;

        EditPtr[ 8]=edIn1_6;      EditPtr[ 9]=edIn1_5;
        EditPtr[10]=edIn1_4;      EditPtr[11]=edIn1_3;
        EditPtr[12]=edIn1_2;      EditPtr[13]=edIn1_1;
        EditPtr[14]=edIn1_8;      EditPtr[15]=edIn1_7;

        edIn2_8->Top=-50;
        edIn2_7->Top=-50;
        edIn1_8->Top=-50;
        edIn1_7->Top=-50;
    }
    else
    {
        EditPtr[ 0]=edIn2_8;      EditPtr[ 1]=edIn2_7;
        EditPtr[ 2]=edIn2_6;      EditPtr[ 3]=edIn2_5;
        EditPtr[ 4]=edIn2_4;      EditPtr[ 5]=edIn2_3;
        EditPtr[ 6]=edIn2_2;      EditPtr[ 7]=edIn2_1;

        EditPtr[ 8]=edIn1_8;      EditPtr[ 9]=edIn1_7;
        EditPtr[10]=edIn1_6;      EditPtr[11]=edIn1_5;
        EditPtr[12]=edIn1_4;      EditPtr[13]=edIn1_3;
        EditPtr[14]=edIn1_2;      EditPtr[15]=edIn1_1;
    }

    EdInSht1[0]=edIn1_1;    EdInSht2[0]=edIn2_1;
    EdInSht1[1]=edIn1_2;    EdInSht2[1]=edIn2_2;
    EdInSht1[2]=edIn1_3;    EdInSht2[2]=edIn2_3;
    EdInSht1[3]=edIn1_4;    EdInSht2[3]=edIn2_4;
    EdInSht1[4]=edIn1_5;    EdInSht2[4]=edIn2_5;
    EdInSht1[5]=edIn1_6;    EdInSht2[5]=edIn2_6;
    EdInSht1[6]=edIn1_7;    EdInSht2[6]=edIn2_7;
    EdInSht1[7]=edIn1_8;    EdInSht2[7]=edIn2_8;

    EditPtr[16]=edIn1_9;      EditPtr[17]=edIn2_9;
    EditPtr[18]=edOut1;       EditPtr[19]=edOut2;

    //------kevin 20130420-----Socket Base----
    EditPtr[20]=edIn3_1;       EditPtr[21]=edIn3_2;
    EditPtr[22]=edIn3_3;       EditPtr[23]=edIn3_4;
    EditPtr[24]=edIn3_5;       EditPtr[25]=edIn3_6;
    EditPtr[26]=edIn3_7;       EditPtr[27]=edIn3_8;

    EditPtr[28]=edIn3_9;       EditPtr[29]=edIn3_10;                            //rotate in  sensor
    EditPtr[30]=edIn3_11;      EditPtr[31]=edIn3_12;                            //rotate out  sensor

    EditPtr[32]=edIn3_13;      EditPtr[33]=edIn3_14;                            //wei 20180409 color sensor
    EditPtr[34]=edIn3_15;      EditPtr[35]=edIn3_16;                            //wei 20180409 color sensor
    EditPtr[36]=edIn3_17;      EditPtr[37]=edIn3_18;                            //wei 20180409 color sensor

    EditPtr[38]=edIn4_1;       EditPtr[39]=edIn4_2;
    EditPtr[40]=edIn4_3;       EditPtr[41]=edIn4_4;
    EditPtr[42]=edIn4_5;       EditPtr[43]=edIn4_6;
    EditPtr[44]=edIn4_7;       EditPtr[45]=edIn4_8;

    EditPtr[46]=edIn4_9;       EditPtr[47]=edIn4_10;
    EditPtr[48]=edIn4_11;      EditPtr[49]=edIn4_12;
    EditPtr[50]=edIn4_13;      EditPtr[51]=edIn4_14;
    EditPtr[52]=edIn4_15;      EditPtr[53]=edIn4_16;

    //設定鈕---------------------------------------------
    if((MachineTypeChoice==Type_HT9045 ||
        MachineTypeChoice==Type_HT9045_12Site) &&                               //Steven 20131008 : for HT9046AH
       UseSensorMod3())                                                         //Sam 20230707 : EtherCAT Shuttle sensor
    {
        ButtonPtr[ 0]=btIn2_6;    ButtonPtr[ 1]=btIn2_5;
        ButtonPtr[ 2]=btIn2_4;    ButtonPtr[ 3]=btIn2_3;
        ButtonPtr[ 4]=btIn2_2;    ButtonPtr[ 5]=btIn2_1;
        ButtonPtr[ 6]=btIn2_8;    ButtonPtr[ 7]=btIn2_7;

        ButtonPtr[ 8]=btIn1_6;    ButtonPtr[ 9]=btIn1_5;
        ButtonPtr[10]=btIn1_4;    ButtonPtr[11]=btIn1_3;
        ButtonPtr[12]=btIn1_2;    ButtonPtr[13]=btIn1_1;
        ButtonPtr[14]=btIn1_8;    ButtonPtr[15]=btIn1_7;

        btIn2_8->Top=-50;
        btIn2_7->Top=-50;
        btIn1_8->Top=-50;
        btIn1_7->Top=-50;
    }
    else
    {
        ButtonPtr[ 0]=btIn2_8;    ButtonPtr[ 1]=btIn2_7;
        ButtonPtr[ 2]=btIn2_6;    ButtonPtr[ 3]=btIn2_5;
        ButtonPtr[ 4]=btIn2_4;    ButtonPtr[ 5]=btIn2_3;
        ButtonPtr[ 6]=btIn2_2;    ButtonPtr[ 7]=btIn2_1;

        ButtonPtr[ 8]=btIn1_8;    ButtonPtr[ 9]=btIn1_7;
        ButtonPtr[10]=btIn1_6;    ButtonPtr[11]=btIn1_5;
        ButtonPtr[12]=btIn1_4;    ButtonPtr[13]=btIn1_3;
        ButtonPtr[14]=btIn1_2;    ButtonPtr[15]=btIn1_1;
    }

    BtnInSht1[0]=btIn1_1;   BtnInSht2[0]=btIn2_1;
    BtnInSht1[1]=btIn1_2;   BtnInSht2[1]=btIn2_2;
    BtnInSht1[2]=btIn1_3;   BtnInSht2[2]=btIn2_3;
    BtnInSht1[3]=btIn1_4;   BtnInSht2[3]=btIn2_4;
    BtnInSht1[4]=btIn1_5;   BtnInSht2[4]=btIn2_5;
    BtnInSht1[5]=btIn1_6;   BtnInSht2[5]=btIn2_6;
    BtnInSht1[6]=btIn1_7;   BtnInSht2[6]=btIn2_7;
    BtnInSht1[7]=btIn1_8;   BtnInSht2[7]=btIn2_8;

    BtnInSht2[8];

    for(int i=0; i<16; i++)                                                     //Steven 20131023 : For HT9046AH
    {
        ButtonPtr[i]->Tag=i;
    }

    ButtonPtr[16]=btIn1_9;    ButtonPtr[17]=btIn2_9;
    ButtonPtr[18]=btOut1;     ButtonPtr[19]=btOut2;

    ButtonPtr[20]=btIn3_1;    ButtonPtr[21]=btIn3_2;                            //kevin 20130420 socket sensor
    ButtonPtr[22]=btIn3_3;    ButtonPtr[23]=btIn3_4;                            //kevin 20130420 socket sensor
    ButtonPtr[24]=btIn3_5;    ButtonPtr[25]=btIn3_6;                            //kevin 20130420 socket sensor
    ButtonPtr[26]=btIn3_7;    ButtonPtr[27]=btIn3_8;                            //kevin 20130420 socket sensor

    ButtonPtr[28]=btIn3_9;    ButtonPtr[29]=btIn3_10;                           //kevin 20130420 Rotate in sensor
    ButtonPtr[30]=btIn3_11;   ButtonPtr[31]=btIn3_12;                           //kevin 20130420 Rotate out sensor

    ButtonPtr[32]=btIn3_13;   ButtonPtr[33]=btIn3_14;                           //wei 20180409 color sensor
    ButtonPtr[34]=btIn3_15;   ButtonPtr[35]=btIn3_16;                           //wei 20180409 color sensor
    ButtonPtr[36]=btIn3_17;   ButtonPtr[37]=btIn3_18;                           //wei 20180409 color sensor

    ButtonPtr[38]=btIn4_1;    ButtonPtr[39]=btIn4_2;                            //kevin 20130420 socket sensor
    ButtonPtr[40]=btIn4_3;    ButtonPtr[41]=btIn4_4;                            //kevin 20130420 socket sensor
    ButtonPtr[42]=btIn4_5;    ButtonPtr[43]=btIn4_6;                            //kevin 20130420 socket sensor
    ButtonPtr[44]=btIn4_7;    ButtonPtr[45]=btIn4_8;                            //kevin 20130420 socket sensor

    ButtonPtr[46]=btIn4_9;    ButtonPtr[47]=btIn4_10;                            //kevin 20130420 socket sensor
    ButtonPtr[48]=btIn4_11;   ButtonPtr[49]=btIn4_12;                            //kevin 20130420 socket sensor
    ButtonPtr[50]=btIn4_13;   ButtonPtr[51]=btIn4_14;                            //kevin 20130420 socket sensor
    ButtonPtr[52]=btIn4_15;   ButtonPtr[53]=btIn4_16;                            //kevin 20130420 socket sensor

    //指示燈---------------------------------------------
    if((MachineTypeChoice==Type_HT9045 ||
        MachineTypeChoice==Type_HT9045_12Site) &&                               //Steven 20131008 : for HT9046AH
       UseSensorMod3())                                                         //Sam 20230707 : EtherCAT Shuttle sensor
    {
        LamePtr[ 0]=ledInSht2_6;    LamePtr[ 1]=ledInSht2_5;
        LamePtr[ 2]=ledInSht2_4;    LamePtr[ 3]=ledInSht2_3;
        LamePtr[ 4]=ledInSht2_2;    LamePtr[ 5]=ledInSht2_1;
        LamePtr[ 6]=ledInSht2_8;    LamePtr[ 7]=ledInSht2_7;

        LamePtr[ 8]=ledInSht1_6;   LamePtr[ 9]=ledInSht1_5;
        LamePtr[10]=ledInSht1_4;   LamePtr[11]=ledInSht1_3;
        LamePtr[12]=ledInSht1_2;   LamePtr[13]=ledInSht1_1;
        LamePtr[14]=ledInSht1_8;   LamePtr[15]=ledInSht1_7;
    }
    else
    {
        LamePtr[ 0]=ledInSht2_8;    LamePtr[ 1]=ledInSht2_7;
        LamePtr[ 2]=ledInSht2_6;    LamePtr[ 3]=ledInSht2_5;
        LamePtr[ 4]=ledInSht2_4;    LamePtr[ 5]=ledInSht2_3;
        LamePtr[ 6]=ledInSht2_2;    LamePtr[ 7]=ledInSht2_1;

        LamePtr[ 8]=ledInSht1_8;   LamePtr[ 9]=ledInSht1_7;
        LamePtr[10]=ledInSht1_6;   LamePtr[11]=ledInSht1_5;
        LamePtr[12]=ledInSht1_4;   LamePtr[13]=ledInSht1_3;
        LamePtr[14]=ledInSht1_2;   LamePtr[15]=ledInSht1_1;
    }

    LedInSht1[0]=ledInSht1_1;   LedInSht2[0]=ledInSht2_1;
    LedInSht1[1]=ledInSht1_2;   LedInSht2[1]=ledInSht2_2;
    LedInSht1[2]=ledInSht1_3;   LedInSht2[2]=ledInSht2_3;
    LedInSht1[3]=ledInSht1_4;   LedInSht2[3]=ledInSht2_4;
    LedInSht1[4]=ledInSht1_5;   LedInSht2[4]=ledInSht2_5;
    LedInSht1[5]=ledInSht1_6;   LedInSht2[5]=ledInSht2_6;
    LedInSht1[6]=ledInSht1_7;   LedInSht2[6]=ledInSht2_7;
    LedInSht1[7]=ledInSht1_8;   LedInSht2[7]=ledInSht2_8;

    LamePtr[16]=MyLedLane17;   LamePtr[17]=MyLedLane18;
    LamePtr[18]=MyLedLane20;   LamePtr[19]=MyLedLane19;

    LamePtr[20]=SocketLED1;    LamePtr[21]=SocketLED2;                          //kevin 20130420 socket sensor
    LamePtr[22]=SocketLED3;    LamePtr[23]=SocketLED4;                          //kevin 20130420 socket sensor
    LamePtr[24]=SocketLED5;    LamePtr[25]=SocketLED6;                          //kevin 20130420 socket sensor
    LamePtr[26]=SocketLED7;    LamePtr[27]=SocketLED8;                          //kevin 20130420 socket sensor

    LamePtr[28]=RotateLED1;    LamePtr[29]=RotateLED2;                          //kevin 20130420 Rotate in sensor
    LamePtr[30]=RotateLED3;    LamePtr[31]=RotateLED4;                          //kevin 20130420 Rotate out sensor

    LamePtr[32]=ColorLED1;     LamePtr[33]=ColorLED2;                           //wei 20180409 color sensor
    LamePtr[34]=ColorLED3;     LamePtr[35]=ColorLED4;                           //wei 20180409 color sensor
    LamePtr[36]=ColorLED5;     LamePtr[37]=ColorLED6;                           //wei 20180409 color sensor
    LamePtr[38]=ColorLED7;     LamePtr[39]=ColorLED8;                           //wei 20180409 color sensor

    LamePtr[40]=SocketLED9;    LamePtr[41]=SocketLED10;                          //kevin 20130420 socket sensor
    LamePtr[42]=SocketLED11;   LamePtr[43]=SocketLED12;                          //kevin 20130420 socket sensor
    LamePtr[44]=SocketLED13;   LamePtr[45]=SocketLED14;                          //kevin 20130420 socket sensor
    LamePtr[46]=SocketLED15;   LamePtr[47]=SocketLED16;                          //kevin 20130420 socket sensor

    LamePtr[48]=SocketLED17;   LamePtr[49]=SocketLED18;                          //JerryYang 20250501
    LamePtr[50]=SocketLED19;   LamePtr[51]=SocketLED20;
    LamePtr[52]=SocketLED21;   LamePtr[53]=SocketLED22;
    LamePtr[54]=SocketLED23;   LamePtr[55]=SocketLED24;

    if(UseCanBusOrEtherCAT())                                                   //Sam 20230707 : EtherCAT Shuttle sensor
    {
        if(USE_SOCKET_SENSOR || USE_ROTATE_KIT ||
           (USE_COLOR_TRAY_SENSOR==1 && COLOR_AMP_QTY>0))                       //JerryYang 20191003 fix cc-link顯示異常
        {
            iTotalDevices=3;
        }
        else
        {
            iTotalDevices=2;
        }

        if(SOCKET_AMP_QTY_2nd>0)
        {
            iTotalDevices=4;
        }

        if(MachineTypeChoice!=Type_HT9045)
        {
            iAmplifierList[0]=16;
        }
        else if(MachineTypeChoice==Type_HT9045 && UseSensorMod3())              //Sam 20230707 : EtherCAT Shuttle sensor
        {
            iAmplifierList[0]=12;
        }
//        else if(iSpecialSequence==0)
//        {
//            iAmplifierList[0]=16;
//        }
        else
        {
            iAmplifierList[0]=14;
        }

        iAmplifierList[1]=4;
        iAmplifierList[2]=0;                                                    //==> 第三個模組NU-DN1

        if(USE_SOCKET_SENSOR)
        {
            if(SOCKET_AMP_QTY<=8)
                iAmplifierList[2]+=SOCKET_AMP_QTY;
        }

        if(USE_COLOR_TRAY_SENSOR==1 && COLOR_AMP_QTY>0)                         //JerryYang 20191003 fix cc-link顯示異常

        {
            iAmplifierList[2]+=COLOR_AMP_QTY;
        }

        if(USE_ROTATE_KIT)
        {
            iAmplifierList[2]+=ROTATE_AMP_QTY;
        }

        iAmplifierList[3]=SOCKET_AMP_QTY_2nd;

        iDesMACIDList[0]=11;                                                    //InShuttle Address
        iDesMACIDList[1]=12;                                                    //OutShuttle Address
        iDesMACIDList[2]=13;                                                    //Socket/ROTATE/Color Address
        iDesMACIDList[3]=14;                                                            

        if(CANBUS_METHOD==1)                                                    //Sam 20210518 : 新增 CanBus 軟體配置
        {
            iTotalDevices=NUDN1_QTY;
            iAmplifierList[0]=NUDN1_MACID11_AMP_QTY;
            iAmplifierList[1]=NUDN1_MACID12_AMP_QTY;
            iAmplifierList[2]=NUDN1_MACID13_AMP_QTY;
            iAmplifierList[3]=NUDN1_MACID14_AMP_QTY;
        }

        if(SHUTTLE_SENSOR_TYPE==eSensorCanBus ||
           SHUTTLE_SENSOR_TYPE==eSensorCanBus3)                                 //Sam 20230707 : EtherCAT Shuttle sensor
        {
            if(DNM100UD!=NULL)
                delete DNM100UD;
            DNM100UD=new TMyDNM100UD_Card(iTotalDevices, iDesMACIDList, iAmplifierList);
        }
        else                                                                    //Sam 20230707 : EtherCAT Shuttle sensor
        {
            if(MyEtherCAT!=NULL)
                delete MyEtherCAT;
            MyEtherCAT=new TMyEtherCAT(3, iDesMACIDList, iAmplifierList);
        }
    }
    else
    {
        if(MyCCLink!=NULL)
            delete MyCCLink;
        MyCCLink=new TMyCCLinkCtrl(this);
        MyCCLink->Path=81;
    }

    for(int i=0; i<11; i++)                                                      //kevin 20130420 總共有多少 TMyTrayPtr  //wei 20180409 color sensor
    {
        TMyTrayPtr[i]->SetColorMap(0, (TColor)0x00E4DDCF);
        TMyTrayPtr[i]->SetColorMap(1, clRed);
        TMyTrayPtr[i]->SetColorMap(2, clYellow);
        TMyTrayPtr[i]->SetColorMap(3, clGreen);
        TMyTrayPtr[i]->SetColorMap(4, (TColor)0x00D9D9A6);
        TMyTrayPtr[i]->SetColorMap(5, (TColor)0x00808040);
        TMyTrayPtr[i]->SetColorMap(6, (TColor)0x00AEA088);
        TMyTrayPtr[i]->SetColorMap(7, (TColor)0x00C2B8A6);

        for(int j=0; j<iSensorNo[i]; j++)                                       //每一個tray 有多少個sensor
        {
            TMyTrayPtr[i]->SetCellNumber(j, 0, j+1);
            TMyTrayPtr[i]->SetCellColorIndex(j, 0, 0);
            TMyTrayPtr[i]->SetCellColorIndex(j, 1, 0);
            TMyTrayPtr[i]->SetCellColorIndex(j, 2, 0);
            TMyTrayPtr[i]->SetCellColorIndex(j, 3, 0);
        }
    }

    if(USE_COLOR_TRAY_SENSOR==1 && COLOR_AMP_QTY>0)                             //JerryYang 20191003 fix cc-link顯示異常
    {
        if(COLOR_AMP_QTY==5)
        {
            TMyTrayPtr[8]->SetCellNumber(0, 0, "Fix1");
            TMyTrayPtr[8]->SetCellNumber(1, 0, "Fix2");
            TMyTrayPtr[8]->SetCellNumber(2, 0, "Fix3");
            TMyTrayPtr[8]->SetCellNumber(3, 0, "Auto1");
            TMyTrayPtr[8]->SetCellNumber(4, 0, "Color");
            TMyTrayPtr[8]->SetCellNumber(5, 0, "");
        }
        else if(COLOR_AMP_QTY==8)
        {
            TMyTrayPtr[8]->SetCellNumber(0, 0, "Fix1");
            TMyTrayPtr[8]->SetCellNumber(1, 0, "Fix2");
            TMyTrayPtr[8]->SetCellNumber(2, 0, "Fix3");
            TMyTrayPtr[8]->SetCellNumber(3, 0, "Auto1");
            TMyTrayPtr[8]->SetCellNumber(4, 0, "Auto2");
            TMyTrayPtr[8]->SetCellNumber(5, 0, "Auto3");
            TMyTrayPtr[8]->SetCellNumber(6, 0, "Empty");
            TMyTrayPtr[8]->SetCellNumber(7, 0, "Color");
        }
        else
        {
            TMyTrayPtr[8]->SetCellNumber(0, 0, "Fix1");
            TMyTrayPtr[8]->SetCellNumber(1, 0, "Fix2");
            TMyTrayPtr[8]->SetCellNumber(2, 0, "Fix3");
            TMyTrayPtr[8]->SetCellNumber(3, 0, "Auto1");
            TMyTrayPtr[8]->SetCellNumber(4, 0, "Auto2");
            TMyTrayPtr[8]->SetCellNumber(5, 0, "Auto3");
        }
    }

    TMyTrayPtr[2]->SetCellNumber(0, 0, 9);
    TMyTrayPtr[3]->SetCellNumber(0, 0, 9);

    if(iSpecialSequence==0)
        iSensorMax=8;
    else
        iSensorMax=7;

    if(IniConfig.bMaximFunction==true)                                          //Steven 20191202 : Maxim要顯示紅色
    {
        for(int i=0; i<38; i++)
        {
            LamePtr[i]->TrueColor=clRed;
        }
    }
    AmpAddress[0]=palAmplifier0;
    AmpAddress[1]=palAmplifier1;
    AmpAddress[2]=palAmplifier2;
    AmpAddress[3]=palAmplifier3;
    AmpAddress[4]=palAmplifier4;
    AmpAddress[5]=palAmplifier5;
    AmpAddress[6]=palAmplifier6;
    AmpAddress[7]=palAmplifier7;
    AmpAddress[8]=palAmplifier8;
    AmpAddress[9]=palAmplifier9;
    AmpAddress[10]=palAmplifier10;
    AmpAddress[11]=palAmplifier11;
    AmpAddress[12]=palAmplifier12;
    AmpAddress[13]=palAmplifier13;
    AmpAddress[14]=palAmplifier14;
    AmpAddress[15]=palAmplifier15;

    for(int i=0; i<16; i++)
    {
        AmpAddress[i]->Caption="";
    }

    bShow=false;                                                                //Steven 20121015 : Alarm時要到前面
}
//---------------------------------------------------------------------------
void __fastcall TfCCLink::sbExitClick(TObject *Sender)
{
    sbExit->Down=false;
    bShow=false;                                                                //Steven 20121015 : Alarm時要到前面
    if(SHUTTLE_SENSOR_TYPE==eSensorEtherCAT ||
       SHUTTLE_SENSOR_TYPE==eSensorEtherCAT3)                                   //Sam 20230707 : EtherCAT Shuttle sensor
        MyEtherCAT->Pause();

    if(CUSTOMER_CODE==CC_ASE_KaohSiung)                                         //kevin 20220912 record CC-link read data
        ShuttleLog();                                                           //Steven 20110531
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfCCLink::FormShow(TObject *Sender)
{
    PageControl1->ActivePage=TabSheet1;
    if(IniConfig.bC08_SocketSensor || USE_ROTATE_KIT ||                         //kevin 20130510   //JerryYang 20191003 fix cc-link顯示異常
       (USE_COLOR_TRAY_SENSOR==1 && COLOR_AMP_QTY>0))                           //Isaac 20180425 color tray sensor in CClink
        tsSocketAndRotate->TabVisible=true;
    else
        tsSocketAndRotate->TabVisible=false;

    tsSocketBase2->TabVisible=SOCKET_AMP_QTY_2nd>0;

    ReadSetting();
    bShow=true;                                                                 //Steven 20121015 : Alarm時要到前面
    if(SHUTTLE_SENSOR_TYPE==eSensorEtherCAT ||
       SHUTTLE_SENSOR_TYPE==eSensorEtherCAT3)                                   //Sam 20230707 : EtherCAT Shuttle sensor
        MyEtherCAT->Start();

    if(CUSTOMER_CODE==CC_KYEC_LEE || CUSTOMER_CODE==CC_KYEC_XILINX)             //20140320 wei
    {
        if(AccessLevel<LevelSet.AccessLevel[45])
        {
            palInSht1->Enabled=false;
            palInSht2->Enabled=false;
            palOutSht1->Enabled=false;
            palOutSht2->Enabled=false;
            tsCCLinkSetting->Enabled=false;
            tsSocketAndRotate->Enabled=false;
        }
        else
        {
            palInSht1->Enabled=true;
            palInSht2->Enabled=true;
            palOutSht1->Enabled=true;
            palOutSht2->Enabled=true;
            tsCCLinkSetting->Enabled=true;
            tsSocketAndRotate->Enabled=true;
        }
    }
    btSave->Visible=CosFunction.bCCLinkValueSaveFile;                           //wei 20180622 (Steven) : CCLink Value Save File
    btRead->Visible=CosFunction.bCCLinkValueSaveFile;                           //wei 20180622 (Steven) : CCLink Value Save File
    btShuttlePositionMove->Visible=(CUSTOMER_CODE==CC_Greatek);
    Left=75;
    Top=10;

    if(USE_SOCKET_SENSOR || USE_ROTATE_KIT ||
       (USE_COLOR_TRAY_SENSOR==1 && COLOR_AMP_QTY>0))                           //JerryYang 20200407 顯示通訊模組連接順序
    {
        if(SOCKET_AMP_QTY>0 && SOCKET_AMP_QTY<=8)
        {
            for(int i=0; i<SOCKET_AMP_QTY; i++)
            {
                AmpAddress[i]->Caption="Socket"+IntToStr(i+1);
            }
        }

        if(USE_ROTATE_KIT==1 && ROTATE_AMP_QTY>0)
        {
            for(int i=iRotateStart; i<iRotateStart+ROTATE_AMP_QTY; i++)
            {
                AmpAddress[i]->Caption="Rotate"+IntToStr(i-iRotateStart+1);
            }
        }

        if(USE_COLOR_TRAY_SENSOR && COLOR_AMP_QTY>0)
        {
            for(int i=iColorStart; i<iColorStart+COLOR_AMP_QTY; i++)
            {
                AmpAddress[i]->Caption="Color"+IntToStr(i);
            }

            if(COLOR_AMP_QTY==5)
            {
                AmpAddress[iColorStart]->Caption  ="Fix1";
                AmpAddress[iColorStart+1]->Caption="Fix2";
                AmpAddress[iColorStart+2]->Caption="Fix3";
                AmpAddress[iColorStart+3]->Caption="Auto1";
                AmpAddress[iColorStart+4]->Caption="Color";
            }
            else if(COLOR_AMP_QTY==6)
            {
                AmpAddress[iColorStart]->Caption  ="Fix1";
                AmpAddress[iColorStart+1]->Caption="Fix2";
                AmpAddress[iColorStart+2]->Caption="Fix3";
                AmpAddress[iColorStart+3]->Caption="Auto1";
                AmpAddress[iColorStart+4]->Caption="Auto2";
                AmpAddress[iColorStart+5]->Caption="Auto3";
            }
        }
    }
    //這一行請保持在最下面!!-----------------
    myLog.Do_Log(Sender, asUser, asLogPath);                                    //Steven 20100629
    if(UseCanBusOrEtherCAT())                                                   //Sam 20230707 : EtherCAT Shuttle sensor //kevin 20221003 Canbus read shuttle sensor data
        ReadSetupFile();
}
//---------------------------------------------------------------------------
void __fastcall TfCCLink::FormClose(TObject *Sender, TCloseAction &Action)
{
//    fCCLink->SaveSetupFile();                                                 //wei 20180622  CCLink Value Save File
    //這一行請保持在最下面!!-----------------
    if(SHUTTLE_SENSOR_TYPE==eSensorEtherCAT ||
        SHUTTLE_SENSOR_TYPE==eSensorEtherCAT3)                                  //KenHsieh 20260116 : 按關閉後要停止timer
    {
        if(In_Shuttle_Auto_Latch!=eInSHAutoLtc ||
          (In_Shuttle_Auto_Latch==eInSHAutoLtc    &&
          (fShuttleMove->fShow==false &&
           fContact->fShow==false)))
        {
            MyEtherCAT->Pause();
        }
    }
    myLog.Do_Log(Sender, asUser, asLogPath);                                    //Steven 20100629
}
//---------------------------------------------------------------------------
void __fastcall TfCCLink::SaveSetupFile()                                       //wei 20180622 (Steven) : CCLink Value Save File
{
    if(CosFunction.bCCLinkValueSaveFile==false)
        return;

    AnsiString str, str2, str3;
    int iValue=0;
    AnsiString S="";
    AnsiString szDir="";

    S=GetLastOpenFN();
    szDir.sprintf("%s%s", DataPath, S);                                         //kevin 20180829 add
    MyForceDirectories(szDir);
    szDir+="\\CCLink.Data";

    for(int i=0; i<16; i++)                                                     //0~15 寫值
    {
        if(UseCanBusOrEtherCAT())                                               //Sam 20230707 : EtherCAT Shuttle sensor
        {
            iValue=GetIOValue(0, i);                                            //Sam 20230707 : EtherCAT Shuttle sensor
        }
        else
        {
            iValue=MyCCLink->CCLinkBoard->MyNCUL[0].iSetValue[i];
        }
        str2.printf("CH%d", 8-i%8);
        if(i<8)
            WriteIniData(szDir, "Shuttle2", str2, iValue);
        else
            WriteIniData(szDir, "Shuttle1", str2, iValue);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfCCLink::ReadSetupFile()                                       //wei 20180622  CCLink Value Save File
{
    if(CosFunction.bCCLinkValueSaveFile==false)
        return;

    AnsiString str, str2, str3;
    int iValue[16]={0};
    iAllSetCount=16;
    AnsiString S="";
    AnsiString szDir="";
    S=GetLastOpenFN();
    szDir.sprintf("%s%s", DataPath, S);                                         //kevin 20180829 add
    MyForceDirectories(szDir);
    szDir+="\\CCLink.Data";

    if(!FileExists(szDir))
    {
        bNewFile=true;
    }
    else
    {
        for(int i=0; i<16; i++)                                                 //15~0讀值
        {
            if(UseCanBusOrEtherCAT())                                           //Sam 20230707 : EtherCAT Shuttle sensor
            {
                str2.printf("CH%d", 8-i%8);                                     //kevin 20220907
                if(i<8)
                    iValue[i]=ReadIniData(szDir, "Shuttle2", str2, 1480);       //kevin 20220907 shuttle sensor read data error
                else
                    iValue[i]=ReadIniData(szDir, "Shuttle1", str2, 1480);
            }
            else
            {
                str2.printf("CH%d", i%8+1);
                if(i<8)
                    iValue[i]=ReadIniData(szDir, "Shuttle1", str2, 1480);
                else
                    iValue[i]=ReadIniData(szDir, "Shuttle2", str2, 1480);
            }
        }

        if(UseCanBusOrEtherCAT())                                               //Sam 20230707 : EtherCAT Shuttle sensor
        {
            for(int i=0; i<16; i++)
            {
                SetIOValue(0, i, iValue[i]);                                    //Sam 20230707 : EtherCAT Shuttle sensor
            }
        }
        else
        {
            MyCCLink->DoSetMultiValue(0, 15, iValue, iAllSetCount);
        }
    }
}
//------------------------------------------------------------------------------
void __fastcall TfCCLink::Timer1Timer(TObject *Sender)
{
    int k, iTemp, iStatus;
    int iSn;
    bool flag=false;

    if(bShow==false)                                                            //Sam 20230707 : EtherCAT Shuttle sensor
        return;

    if(InitialOK==false || (DNM100UD!=NULL && DNM100UD->bCanBusSeach==true))    //Sam 20210518 : 新增 CanBus 軟體配置 //Steven 20210603 : 避免還沒new就使用
        return;

    if(SHUTTLE_SENSOR_TYPE==eSensorCanBus ||
       SHUTTLE_SENSOR_TYPE==eSensorCanBus3)
    {
        if(DNM100UD!=NULL && DNM100UD->DNM100IsShutDownNeedPowerOffPC==true)
        {
            ShowMyMessage("DNM100 is shutdown. Need power off handler pc!");
            Timer1->Enabled=false;
            return;
        }

        if(DNM100UD!=NULL && DNM100UD->InitialOK==false)
            return;
    }
    else if(SHUTTLE_SENSOR_TYPE==eSensorEtherCAT ||
            SHUTTLE_SENSOR_TYPE==eSensorEtherCAT3)                              //Sam 20230707 : EtherCAT Shuttle sensor
    {
        if(MyEtherCAT!=NULL && MyEtherCAT->InitialOK==false)
            return;
    }
    else
    {
        if(MyCCLink->InitialOK==false)
            return;
    }

    Timer1->Enabled=false;

    if(UseCanBusOrEtherCAT())                                                   //Sam 20230707 : EtherCAT Shuttle sensor
    {
        flag=bHasError();                                                       //Sam 20230707 : EtherCAT Shuttle sensor
        if(flag==false)
        {
            iStatus=0;
            lbErrMess->Font->Color=clBlack;
            lbErrMess->Caption="OK";
        }
        else
        {
            sbReset->Visible=true;
            iStatus=1;
            lbErrMess->Font->Color=clRed;
        }

        if(MachineTypeChoice==Type_HT9045 && UseSensorMod3())                   //Sam 20230707 : EtherCAT Shuttle sensor //eSensorCanBus3 待驗證  //Steven 20131008 : for HT9046AH
        {
            for(int i=0; i<12; i++)
            {
                TMyTrayPtr[i/6]->SetCellNumber(i%6, 1, cSensorStatus[iStatus]);
                TMyTrayPtr[i/6]->SetCellColorIndex(i%6, 1, iStatus);

                iTemp=GetValue(0, i);                                           //Sam 20230707 : EtherCAT Shuttle sensor
                TMyTrayPtr[1-i/6]->SetCellNumber(5-i%6, 2, iTemp);

                iTemp=GetIOValue(0,i);                                          //Sam 20230707 : EtherCAT Shuttle sensor
                TMyTrayPtr[1-i/6]->SetCellNumber(5-i%6, 3, iTemp);

                if(i<6)
                    LamePtr[i]->Value=!GetIO(0, i);                             //Sam 20230707 : EtherCAT Shuttle sensor
                else                                                            //燈號
                    LamePtr[i+2]->Value=!GetIO(0, i);                           //Sam 20230707 : EtherCAT Shuttle sensor
            }
        }
        else if(iSpecialSequence==0)                                            //2013-04-12    Dell :for KYEC 特殊排列方式 serio:29829
        {
            for(int i=0; i<16; i++)
            {
                TMyTrayPtr[i/8]->SetCellNumber(i%8, 1, cSensorStatus[iStatus]);
                TMyTrayPtr[i/8]->SetCellColorIndex(i%8, 1, iStatus);

                iTemp=GetValue(0, i);                                           //Sam 20230707 : EtherCAT Shuttle sensor
                TMyTrayPtr[1-i/8]->SetCellNumber(7-i%8, 2, iTemp);
                if(i>=8)                                                        //kevin 20220912 shuttle 2 data log
                {
                    iSensor[0][i-8]= iTemp;                                     //kevin 20220912 log Shuttle 1 display
                }
                else
                {
                    iSensor[1][i]= iTemp;                                       //kevin 20220912 log Shuttle 2 display
                }
                iTemp=GetIOValue(0, i);                                         //Sam 20230707 : EtherCAT Shuttle sensor
                TMyTrayPtr[1-i/8]->SetCellNumber(7-i%8, 3, iTemp);
                if(i>=8)                                                        //kevin 20220912 shuttle 2 data log
                {
                    iSensor[2][i-8]= iTemp;                                     //kevin 20220912 log Shuttle 1 display
                }
                else
                {
                    iSensor[3][i]= iTemp;                                       //kevin 20220912 log Shuttle 2 display
                }
                LamePtr[i]->Value=!GetIO(0, i);                                 //Sam 20230707 : EtherCAT Shuttle sensor
            }
        }
        else                                                                    //for KYEC 特殊排列方式 serio:29829
        {
            for(int i=0, j=0; i<14; i++, j++)
            {
                if(i<iSensorMax)
                    k=1;
                else
                    k=2;

                TMyTrayPtr[i/7]->SetCellNumber(i%7, 1, cSensorStatus[iStatus]);
                TMyTrayPtr[i/7]->SetCellColorIndex(i%7, 1, iStatus);

                iTemp=GetValue(0, i);                                           //Sam 20230707 : EtherCAT Shuttle sensor
                TMyTrayPtr[1-i/7]->SetCellNumber((7-i%7)-1, 2, iTemp);

                iTemp=GetIOValue(0, i);                                         //Sam 20230707 : EtherCAT Shuttle sensor
                TMyTrayPtr[1-i/7]->SetCellNumber((7-i%7)-1, 3, iTemp);

                LamePtr[i+k]->Value=!GetIO(0, i);                               //Sam 20230707 : EtherCAT Shuttle sensor
            }
        }

        for(int i=0; i<4; i++)
        {
            TMyTrayPtr[i+2]->SetCellNumber(0, 1, cSensorStatus[iStatus]);       //Status
            TMyTrayPtr[i+2]->SetCellColorIndex(0, 1, iStatus);

            iTemp=GetValue(1, i);                                               //Sam 20230707 : EtherCAT Shuttle sensor
            TMyTrayPtr[i+2]->SetCellNumber(0, 2, iTemp);

             iTemp=GetIOValue(1, i);                                            //Sam 20230707 : EtherCAT Shuttle sensor
            TMyTrayPtr[i+2]->SetCellNumber(0, 3, iTemp);

            LamePtr[i+16]->Value=!GetIO(1, i);                                  //Sam 20230707 : EtherCAT Shuttle sensor
        }


        if(USE_SOCKET_SENSOR && SOCKET_AMP_QTY>0 && SOCKET_AMP_QTY<=8)          //JerryYang 20200327 整理第三個通訊模組socket/ rotate/ color
        {
            for(int i=0; i<SOCKET_AMP_QTY; i++)                                 //socket sensor
            {
                TMyTrayPtr[6]->SetCellNumber(i, 1, cSensorStatus[iStatus]);
                TMyTrayPtr[6]->SetCellColorIndex(i, 1, iStatus);

                iTemp=GetValue(2, i);                                           //Sam 20230707 : EtherCAT Shuttle sensor
                TMyTrayPtr[6]->SetCellNumber(i, 2, iTemp);

                iTemp=GetIOValue(2, i);                                         //Sam 20230707 : EtherCAT Shuttle sensor
                TMyTrayPtr[6]->SetCellNumber(i, 3, iTemp);

                LamePtr[i+20]->Value=GetIO(2, i);                               //反向 D-on //Sam 20230707 : EtherCAT Shuttle sensor
            }
        }

        if(USE_ROTATE_KIT && ROTATE_AMP_QTY>0)
        {
            for(int i=0; i<ROTATE_AMP_QTY; i++)                                 //rotate
            {
                TMyTrayPtr[7]->SetCellNumber(i, 1, cSensorStatus[iStatus]);
                TMyTrayPtr[7]->SetCellColorIndex(i, 1, iStatus);

                iTemp=GetValue(2, i+iRotateStart);                              //Sam 20230707 : EtherCAT Shuttle sensor
                TMyTrayPtr[7]->SetCellNumber(i, 2, iTemp);

                iTemp=GetIOValue(2, i+iRotateStart);                            //Sam 20230707 : EtherCAT Shuttle sensor
                TMyTrayPtr[7]->SetCellNumber(i, 3, iTemp);
                if(CUSTOMER_CODE==CC_TERAPOWER)                                 //Sam 20191112 : 英傑要求 Roate Sensor 要反向
                {
                    LamePtr[i+28]->Value=GetIO(2, i+iRotateStart);              //Sam 20230707 : EtherCAT Shuttle sensor
                }
                else
                {
                    LamePtr[i+28]->Value=!GetIO(2, i+iRotateStart);             //Sam 20230707 : EtherCAT Shuttle sensor
                }
            }
        }

        if(USE_COLOR_TRAY_SENSOR==1 && COLOR_AMP_QTY>0)
        {
            for(int i=0; i<COLOR_AMP_QTY; i++)                                  //color
            {
                TMyTrayPtr[8]->SetCellNumber(i, 1, cSensorStatus[iStatus]);
                TMyTrayPtr[8]->SetCellColorIndex(i, 1, iStatus);

                iTemp=GetValue(2, i+iColorStart);                               //Sam 20230707 : EtherCAT Shuttle sensor
                TMyTrayPtr[8]->SetCellNumber(i, 2, iTemp);

                iTemp=GetIOValue(2, i+iColorStart);                             //Sam 20230707 : EtherCAT Shuttle sensor
                TMyTrayPtr[8]->SetCellNumber(i, 3, iTemp);
                if(CUSTOMER_CODE==CC_TERAPOWER)                                 //Sam 20180704 : 晶兆成 color sensor 軟硬體顯示與7系列一樣，亮起是紅色
                {
                    LamePtr[i+32]->Value=GetIO(2, i+iColorStart);               //Sam 20230707 : EtherCAT Shuttle sensor
                }
                else
                {
                    LamePtr[i+32]->Value=!GetIO(2, i+iColorStart);              //Sam 20230707 : EtherCAT Shuttle sensor
                }
            }
        }

        if(USE_SOCKET_SENSOR && SOCKET_AMP_QTY_2nd>0 && SOCKET_AMP_QTY_2nd<=16)
        {
            for(int i=0; i<SOCKET_AMP_QTY_2nd; i++)                             //socket sensor
            {
                if(i>=0 && i<=7)
                {
                    TMyTrayPtr[9]->SetCellNumber(i, 1, cSensorStatus[iStatus]);
                    TMyTrayPtr[9]->SetCellColorIndex(i, 1, iStatus);

                    iTemp=GetValue(3, i);                                           //Sam 20230707 : EtherCAT Shuttle sensor
                    TMyTrayPtr[9]->SetCellNumber(i, 2, iTemp);

                    iTemp=GetIOValue(3, i);                                         //Sam 20230707 : EtherCAT Shuttle sensor
                    TMyTrayPtr[9]->SetCellNumber(i, 3, iTemp);

                    LamePtr[i+40]->Value=GetIO(3, i);                               //反向 D-on //Sam 20230707 : EtherCAT Shuttle sensor
                }
                else
                {
                    TMyTrayPtr[10]->SetCellNumber(i%8, 1, cSensorStatus[iStatus]);
                    TMyTrayPtr[10]->SetCellColorIndex(i%8, 1, iStatus);

                    iTemp=GetValue(3, i);                                           //Sam 20230707 : EtherCAT Shuttle sensor
                    TMyTrayPtr[10]->SetCellNumber(i%8, 2, iTemp);

                    iTemp=GetIOValue(3, i);                                         //Sam 20230707 : EtherCAT Shuttle sensor
                    TMyTrayPtr[10]->SetCellNumber(i%8, 3, iTemp);

                    LamePtr[i+40]->Value=GetIO(3, i);                               //反向 D-on //Sam 20230707 : EtherCAT Shuttle sensor
                }
            }
        }
    }
    else
    {
        flag=MyCCLink->HasError;

        if(flag==false)
        {
            iStatus=0;
            lbErrMess->Font->Color=clBlack;
            lbErrMess->Caption=MyCCLink->SystemMessage;
            iErrorCnt=0;                                                        //JerryYang 20160729 連線異常秒數
        }
        else
        {
            iStatus=1;
            sbReset->Visible=true;
            lbErrMess->Font->Color=clRed;
            lbErrMess->Caption=MyCCLink->ErrorMessage;
            iErrorCnt++;
        }

        if(CosFunction.bShowAlarmIfCCLinkConnectFail)
        {
            if(flag && iErrorCnt>=60)                                           //JerryYang 20160729 CC-link連續60秒連線異常就重新連線
            {
                iErrorCnt=0;
                if(iAutoRetryCnt>=3)
                {
                    #ifndef SOFT_SIMULTE
                    ShowErrorMessage("WAR16109", K_RETRY, MMSystem);            //JerryYang 20160729 CC-link重新Reset 3次失敗後跳出alarm
                    #endif
                    iAutoRetryCnt=0;
                }
                sbReset->Click();
            }
        }
        else
        {
            iErrorCnt=0;
            iAutoRetryCnt=0;
        }

        if((MachineTypeChoice==Type_HT9045 ||
            MachineTypeChoice==Type_HT9045_12Site) &&
           SHUTTLE_SENSOR_TYPE==eSensorCCLink3)                                 //Steven 20131008 : for HT9046AH
        {
            for(int i=0; i<12; i++)
            {
                TMyTrayPtr[i/6]->SetCellNumber(i%6, 1, cSensorStatus[iStatus]);
                TMyTrayPtr[i/6]->SetCellColorIndex(i%6, 1, iStatus);

                iTemp=MyCCLink->CCLinkBoard->MyNCUL[0].iCurrValue[i];
                TMyTrayPtr[1-i/6]->SetCellNumber(5-i%6, 2, iTemp);

                iTemp=MyCCLink->CCLinkBoard->MyNCUL[0].iSetValue[i];
                TMyTrayPtr[1-i/6]->SetCellNumber(5-i%6, 3, iTemp);
                if(i<6)
                    LamePtr[i]->Value=MyCCLink->CCLinkBoard->MyNCUL[0].bOnOff[i];
                else                                                            //kevin 20131115 燈號
                    LamePtr[i+2]->Value=MyCCLink->CCLinkBoard->MyNCUL[0].bOnOff[i];
            }
        }
        else if(iSpecialSequence==0)                                            //2013-04-12    Dell :for KYEC 特殊排列方式 serio:29829
        {
            for(int i=0; i<16; i++)
            {
                TMyTrayPtr[i/8]->SetCellNumber(i%8, 1, cSensorStatus[iStatus]);
                TMyTrayPtr[i/8]->SetCellColorIndex(i%8, 1, iStatus);

                iTemp=MyCCLink->CCLinkBoard->MyNCUL[0].iCurrValue[i];
                TMyTrayPtr[1-i/8]->SetCellNumber(7-i%8, 2, iTemp);

                if(i>=8)                                                        //kevin 20220912 shuttle 2 data log
                {
                    iSensor[0][i-8]= iTemp;                                     //kevin 20220912 log Shuttle 1 display
                }
                else
                {
                    iSensor[1][i]= iTemp;                                       //kevin 20220912 log Shuttle 2 display
                }
                iTemp=MyCCLink->CCLinkBoard->MyNCUL[0].iSetValue[i];
                TMyTrayPtr[1-i/8]->SetCellNumber(7-i%8, 3, iTemp);
                if(i>=8)                                                        //kevin 20220912 shuttle 2 data log
                {
                    iSensor[2][i-8]= iTemp;                                     //kevin 20220912 log Shuttle 1 display
                }
                else
                {
                    iSensor[3][i]= iTemp;                                       //kevin 20220912 log Shuttle 2 display
                }
                LamePtr[i]->Value=MyCCLink->CCLinkBoard->MyNCUL[0].bOnOff[i];
            }
        }
        else                                                                    //for KYEC 特殊排列方式 serio:29829
        {
            for(int i=0, j=0; i<14; i++, j++)
            {
                if(i<iSensorMax)
                    k=1;
                else
                    k=2;

                TMyTrayPtr[i/7]->SetCellNumber(i%7, 1, cSensorStatus[iStatus]);
                TMyTrayPtr[i/7]->SetCellColorIndex(i%7, 1, iStatus);

                iTemp=MyCCLink->CCLinkBoard->MyNCUL[0].iCurrValue[i];
                TMyTrayPtr[1-i/7]->SetCellNumber((7-i%7)-1, 2, iTemp);

                iTemp=MyCCLink->CCLinkBoard->MyNCUL[0].iSetValue[i];
                TMyTrayPtr[1-i/7]->SetCellNumber((7-i%7)-1, 3, iTemp);

                LamePtr[i+k]->Value=MyCCLink->CCLinkBoard->MyNCUL[0].bOnOff[i];
            }
        }

        for(int i=0; i<4; i++)
        {
            TMyTrayPtr[i+2]->SetCellNumber(0, 1, cSensorStatus[iStatus]);       //Status
            TMyTrayPtr[i+2]->SetCellColorIndex(0, 1, iStatus);

            iTemp=MyCCLink->CCLinkBoard->MyNCUL[1].iCurrValue[i];               //Current Value
            TMyTrayPtr[i+2]->SetCellNumber(0, 2, iTemp);

            iTemp=MyCCLink->CCLinkBoard->MyNCUL[1].iSetValue[i];                //Setting Value
            TMyTrayPtr[i+2]->SetCellNumber(0, 3, iTemp);
            LamePtr[i+16]->Value=MyCCLink->CCLinkBoard->MyNCUL[1].bOnOff[i];
        }

        if(USE_COLOR_TRAY_SENSOR==1 && COLOR_AMP_QTY>0)                         //JerryYang 20200327 整理第三個通訊模組socket/ rotate/ color
        {
            if(IniConfig.bC08_SocketSensor &&
               USE_ROTATE_KIT && USE_COLOR_TRAY_SENSOR)                         //kevin 20130420  //Isaac 20180425    //111
            {
                for(int i=0; i<4; i++)                                          //socket sensor
                {
                    TMyTrayPtr[6]->SetCellNumber(i, 1, cSensorStatus[iStatus]);
                    TMyTrayPtr[6]->SetCellColorIndex(i, 1, iStatus);

                    iTemp=MyCCLink->CCLinkBoard->MyNCUL[2].iCurrValue[i];
                    TMyTrayPtr[6]->SetCellNumber(i, 2, iTemp);

                    iTemp=MyCCLink->CCLinkBoard->MyNCUL[2].iSetValue[i];
                    TMyTrayPtr[6]->SetCellNumber(i, 3, iTemp);

                    LamePtr[i+20]->Value=!MyCCLink->CCLinkBoard->MyNCUL[2].bOnOff[i]; //kevin 20150429 cclink 反向 D-on
                }

                for(int i=0; i<4; i++)                                          //rotate
                {
                    TMyTrayPtr[7]->SetCellNumber(i, 1, cSensorStatus[iStatus]);
                    TMyTrayPtr[7]->SetCellColorIndex(i, 1, iStatus);

                    iTemp=MyCCLink->CCLinkBoard->MyNCUL[2].iCurrValue[i+4];
                    TMyTrayPtr[7]->SetCellNumber(i, 2, iTemp);

                    iTemp=MyCCLink->CCLinkBoard->MyNCUL[2].iSetValue[i+4];
                    TMyTrayPtr[7]->SetCellNumber(i, 3, iTemp);
                    if(CUSTOMER_CODE==CC_TERAPOWER)                             //Sam 20191112 : 英傑要求 Roate Sensor 要反向
                    {
                        LamePtr[i+28]->Value=!MyCCLink->CCLinkBoard->MyNCUL[2].bOnOff[i+4];
                    }
                    else
                    {
                        LamePtr[i+28]->Value=MyCCLink->CCLinkBoard->MyNCUL[2].bOnOff[i+4];
                    }
                }

                for(int i=0; i<6; i++)                                          //color
                {
                    TMyTrayPtr[8]->SetCellNumber(i, 1, cSensorStatus[iStatus]);
                    TMyTrayPtr[8]->SetCellColorIndex(i, 1, iStatus);

                    iTemp=MyCCLink->CCLinkBoard->MyNCUL[2].iCurrValue[i+8];
                    TMyTrayPtr[8]->SetCellNumber(i, 2, iTemp);

                    iTemp=MyCCLink->CCLinkBoard->MyNCUL[2].iSetValue[i+8];
                    TMyTrayPtr[8]->SetCellNumber(i, 3, iTemp);

                    if(CUSTOMER_CODE==CC_TERAPOWER)                             //Sam 20180704 : 晶兆成 color sensor 軟硬體顯示與7系列一樣，亮起是紅色
                    {
                        LamePtr[i+32]->Value=!MyCCLink->CCLinkBoard->MyNCUL[2].bOnOff[i+8];
                    }
                    else
                    {
                        LamePtr[i+32]->Value=MyCCLink->CCLinkBoard->MyNCUL[2].bOnOff[i+8];
                    }
                }
            }
            else if(IniConfig.bC08_SocketSensor==false &&
                    USE_ROTATE_KIT && USE_COLOR_TRAY_SENSOR)                    //kevin 20130420  //Isaac 20180425    //011
            {
                for(int i=0; i<4; i++)                                          //rotate
                {
                    TMyTrayPtr[7]->SetCellNumber(i, 1, cSensorStatus[iStatus]);
                    TMyTrayPtr[7]->SetCellColorIndex(i, 1, iStatus);

                    iTemp=MyCCLink->CCLinkBoard->MyNCUL[2].iCurrValue[i+4];
                    TMyTrayPtr[7]->SetCellNumber(i, 2, iTemp);

                    iTemp=MyCCLink->CCLinkBoard->MyNCUL[2].iSetValue[i+4];
                    TMyTrayPtr[7]->SetCellNumber(i, 3, iTemp);
                    if(CUSTOMER_CODE==CC_TERAPOWER)                             //Sam 20191112 : 英傑要求 Roate Sensor 要反向
                    {
                        LamePtr[i+28]->Value=!MyCCLink->CCLinkBoard->MyNCUL[2].bOnOff[i+4];
                    }
                    else
                    {
                        LamePtr[i+28]->Value=MyCCLink->CCLinkBoard->MyNCUL[2].bOnOff[i+4];
                    }
                }

                for(int i=0; i<6; i++)                                          //color
                {
                    TMyTrayPtr[8]->SetCellNumber(i, 1, cSensorStatus[iStatus]);
                    TMyTrayPtr[8]->SetCellColorIndex(i, 1, iStatus);

                    iTemp=MyCCLink->CCLinkBoard->MyNCUL[2].iCurrValue[i+8];
                    TMyTrayPtr[8]->SetCellNumber(i, 2, iTemp);

                    iTemp=MyCCLink->CCLinkBoard->MyNCUL[2].iSetValue[i+8];
                    TMyTrayPtr[8]->SetCellNumber(i, 3, iTemp);

                    if(CUSTOMER_CODE==CC_TERAPOWER)                             //Sam 20180704 : 晶兆成 color sensor 軟硬體顯示與7系列一樣，亮起是紅色
                    {
                        LamePtr[i+32]->Value=!MyCCLink->CCLinkBoard->MyNCUL[2].bOnOff[i];
                    }
                    else
                    {
                        LamePtr[i+32]->Value=MyCCLink->CCLinkBoard->MyNCUL[2].bOnOff[i+8];
                    }
                }
            }
            else if(IniConfig.bC08_SocketSensor==false &&
                    USE_ROTATE_KIT==false && USE_COLOR_TRAY_SENSOR)             //kevin 20130420  //Isaac 20180425    //001
            {
                for(int i=0; i<6; i++)                                          //color
                {
                    TMyTrayPtr[8]->SetCellNumber(i, 1, cSensorStatus[iStatus]);
                    TMyTrayPtr[8]->SetCellColorIndex(i, 1, iStatus);

                    iTemp=MyCCLink->CCLinkBoard->MyNCUL[2].iCurrValue[i];
                    TMyTrayPtr[8]->SetCellNumber(i, 2, iTemp);

                    iTemp=MyCCLink->CCLinkBoard->MyNCUL[2].iSetValue[i];
                    TMyTrayPtr[8]->SetCellNumber(i, 3, iTemp);

                    if(IniConfig.bMaximFunction ||
                       CUSTOMER_CODE==CC_HONPREC_QC ||
                       CUSTOMER_CODE==CC_TERAPOWER)                             //Sam 20180704 : 晶兆成 color sensor 軟硬體顯示與7系列一樣，亮起是紅色
                        LamePtr[i+32]->Value=!MyCCLink->CCLinkBoard->MyNCUL[2].bOnOff[i];
                    else
                        LamePtr[i+32]->Value=MyCCLink->CCLinkBoard->MyNCUL[2].bOnOff[i];
                }
            }
        }
        else
        {
            if(IniConfig.bC08_SocketSensor && USE_ROTATE_KIT)                   //kevin 20130420
            {
                for(int i=0; i<8; i++)                                          // 第3組  //kevin 20130420 socket sensor
                {
                    TMyTrayPtr[6]->SetCellNumber(i, 1, cSensorStatus[iStatus]);
                    TMyTrayPtr[6]->SetCellColorIndex(i, 1, iStatus);

                    iTemp=MyCCLink->CCLinkBoard->MyNCUL[2].iCurrValue[i];
                    TMyTrayPtr[6]->SetCellNumber(i, 2, iTemp);

                    iTemp=MyCCLink->CCLinkBoard->MyNCUL[2].iSetValue[i];
                    TMyTrayPtr[6]->SetCellNumber(i, 3, iTemp);

                    LamePtr[i+20]->Value=!MyCCLink->CCLinkBoard->MyNCUL[2].bOnOff[i]; //kevin 20150429 cclink 反向 D-on
                }

                for(int i=0; i<4; i++)                                          // 第3組後面接第89 input rotate  10 11 output ROTATE SENSOR //kevin 20130420 socket sensor
                {
                    TMyTrayPtr[7]->SetCellNumber(i+8, 1, cSensorStatus[iStatus]);
                    TMyTrayPtr[7]->SetCellColorIndex(i+8, 1, iStatus);

                    iTemp=MyCCLink->CCLinkBoard->MyNCUL[2].iCurrValue[i+8];
                    TMyTrayPtr[7]->SetCellNumber(i+8, 2, iTemp);

                    iTemp=MyCCLink->CCLinkBoard->MyNCUL[2].iSetValue[i+8];
                    TMyTrayPtr[7]->SetCellNumber(i+8, 3, iTemp);
                    if(CUSTOMER_CODE==CC_TERAPOWER)                             //Sam 20191112 : 英傑要求 Roate Sensor 要反向
                    {
                        LamePtr[i+28]->Value=!MyCCLink->CCLinkBoard->MyNCUL[2].bOnOff[i+8];
                    }
                    else
                    {
                        LamePtr[i+28]->Value=MyCCLink->CCLinkBoard->MyNCUL[2].bOnOff[i+8];
                    }
                }
            }
            else if(IniConfig.bC08_SocketSensor==false && USE_ROTATE_KIT)       //kevin 20130420
            {
                for(int i=0; i<4; i++)                                          // 第3組後面接第89 input rotate  10 11 output ROTATE SENSOR //kevin 20130420 socket sensor
                {
                    TMyTrayPtr[7]->SetCellNumber(i, 1, cSensorStatus[iStatus]);
                    TMyTrayPtr[7]->SetCellColorIndex(i, 1, iStatus);

                    iTemp=MyCCLink->CCLinkBoard->MyNCUL[2].iCurrValue[i];
                    TMyTrayPtr[7]->SetCellNumber(i, 2, iTemp);

                    iTemp=MyCCLink->CCLinkBoard->MyNCUL[2].iSetValue[i];
                    TMyTrayPtr[7]->SetCellNumber(i, 3, iTemp);
                    if(CUSTOMER_CODE==CC_TERAPOWER)                             //Sam 20191112 : 英傑要求 Roate Sensor 要反向
                    {
                        LamePtr[i+28]->Value=!MyCCLink->CCLinkBoard->MyNCUL[2].bOnOff[i];
                    }
                    else
                    {
                        LamePtr[i+28]->Value=MyCCLink->CCLinkBoard->MyNCUL[2].bOnOff[i];
                    }
                }
            }
            else if(IniConfig.bC08_SocketSensor && USE_ROTATE_KIT==false)       //kevin 20140829  只偶SOCKET SENSOR
            {
                for(int i=0; i<8; i++)                                          // 第3組  //kevin 20130420 socket sensor
                {
                    TMyTrayPtr[6]->SetCellNumber(i, 1, cSensorStatus[iStatus]);
                    TMyTrayPtr[6]->SetCellColorIndex(i, 1, iStatus);

                    iTemp=MyCCLink->CCLinkBoard->MyNCUL[2].iCurrValue[i];
                    TMyTrayPtr[6]->SetCellNumber(i, 2, iTemp);

                    iTemp=MyCCLink->CCLinkBoard->MyNCUL[2].iSetValue[i];
                    TMyTrayPtr[6]->SetCellNumber(i, 3, iTemp);

                    LamePtr[i+20]->Value=!MyCCLink->CCLinkBoard->MyNCUL[2].bOnOff[i]; //kevin 20150429 cclink 反向 D-on
                }
            }
        }
    }

    if(USE_COLOR_TRAY_SENSOR && COLOR_AMP_QTY>0)                                //JerryYang 20200327 整理第三個通訊模組socket/ rotate/ color
    {
        iSn=8;                                                                  //Sam 20181025 : 晶兆成 8 > 9 Color Sensor 不要自動編號
    }
    else
    {
        iSn=9;
    }

    if(SOCKET_AMP_QTY_2nd>8)
    {
        iSn=11;
    }
    else if(SOCKET_AMP_QTY_2nd>0 && SOCKET_AMP_QTY_2nd<=8)
    {
        iSn=10;
    }

    for(int i=0; i<iSn; i++)                                                    //kevin 20130420 總共有多少 TMyTrayPtr  //wei 20180409 color sensor
    {
        for(int j=0; j<iSensorNo[i]; j++)                                       //每一個tray 有多少個sensor
        {
            TMyTrayPtr[i]->SetCellNumber(j, 0, j+1);
        }
    }

    Timer1->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TfCCLink::sbResetClick(TObject *Sender)
{
    Reset();                                                                    //Sam 20230707 : EtherCAT Shuttle sensor
}
//---------------------------------------------------------------------------
void __fastcall TfCCLink::edIn2_1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 3900, 10);
}
//---------------------------------------------------------------------------
void __fastcall TfCCLink::btIn2_8MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TButton *Ptr;
    Ptr=(TButton *)Sender;
    int iValue=0;
    short sNu;
    sNu=Ptr->Tag/16;
    sSen=Ptr->Tag%16;
    int iIndex =0;

    if(iInputMethod==1)
    {
        if(UseCanBusOrEtherCAT())                                               //Sam 20230707 : EtherCAT Shuttle sensor
        {
            iValue=GetValue(sNu, sSen)*iPercentage/100;                         //Sam 20230707 : EtherCAT Shuttle sensor
        }
        else
        {
            iValue=MyCCLink->CCLinkBoard->MyNCUL[sNu].iCurrValue[sSen]*iPercentage/100;
        }

        if(iValue>4095)
            iValue=iValue*0.9;

        if(iValue<10)                                                           //JerryYang 20160706 超豐賢哥要求100->10
            iValue=10;

        if(CUSTOMER_CODE==CC_JCET)
        {
            if(iValue<iMinValue)
            {
                iValue=iMinValue;
            }
        }
        EditPtr[Ptr->Tag]->Text=AnsiString(iValue);
    }
    else
    {
        iValue=atoi(EditPtr[Ptr->Tag]->Text.c_str());
        if(iValue<10)                                                           //JerryYang 20160706 超豐賢哥要求100->10
            iValue=10;

        if(CUSTOMER_CODE==CC_JCET)
        {
            if(iValue<iMinValue)
            {
                iValue=iMinValue;
            }
        }
    }

    if(UseSensorMod3() && sSen>=8)                                              //Sam 20230707 : EtherCAT Shuttle sensor //kevin 20131115 設定cclink 偵測值     //Steven 20131008 : for HT9046AH
    {
//        if(SHUTTLE_SENSOR_TYPE==eSensorEtherCAT3)                             //Sam 20250821 Mark才對  //Ifor 20241216 Fix Index=2 In Shuttle 1 設定會位移兩格
//            iIndex=0;
//        else
            iIndex=2;
    }
    else if(iSpecialSequence==1)
    {
        iIndex=1;
    }

    if(UseCanBusOrEtherCAT())                                                   //Sam 20230707 : EtherCAT Shuttle sensor
    {
        SetIOValue(sNu, sSen-iIndex, iValue);                                   //Sam 20230707 : EtherCAT Shuttle sensor
    }
    else
    {
        MyCCLink->DoSetValue(sNu, sSen-iIndex, iValue);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfCCLink::ReadSetting()
{
    int iValue=0;
    AnsiString FileName="D:\\HT9045\\config\\config.ini";
    iInputMethod    =CheckAndReadIniData(FileName, "CCLink", "Input Method",    0);
    iPercentage     =CheckAndReadIniData(FileName, "CCLink", "Percentage",      90);
    iDefaultValue   =CheckAndReadIniData(FileName, "CCLink", "Default Value",   1480);
    iMinValue       =CheckAndReadIniData(FileName, "CCLink", "Min Value",       50);
    GetStationStart();                                                          //JerryYang 20200327 取得socket/ rotate/ color起始站號
    rgInputMethod->ItemIndex=iInputMethod;
    edPercentage->Text      =iPercentage;
    edDefaultValue->Text    =iDefaultValue;

    if(iInputMethod==1)
    {
        for(int i=0; i<20; i++)
        {
            EditPtr[i]->Enabled=false;
            if(UseCanBusOrEtherCAT())                                           //Sam 20230707 : EtherCAT Shuttle sensor
            {
                iValue=GetValue(i/16, i%16)*iPercentage/100;                    //Sam 20230707 : EtherCAT Shuttle sensor
            }
            else
            {
                iValue=MyCCLink->CCLinkBoard->MyNCUL[i/16].iCurrValue[i%16]*iPercentage/100;
            }

            if(iValue>3900)
                iValue=3900;
            if(iValue<10)                                                       //JerryYang 20160706 超豐賢哥要求100->10
                iValue=10;

            if(CUSTOMER_CODE==CC_JCET)
            {
                if(iValue<iMinValue)
                {
                    iValue=iMinValue;
                }
            }
            EditPtr[i]->Text=iValue;
        }
    }
    else if(iInputMethod==2)
    {
        for(int i=0; i<20; i++)
        {
            EditPtr[i]->Enabled=false;
            EditPtr[i]->Text=iDefaultValue;
        }
    }
    else
    {
        for(int i=0; i<20; i++)
        {
            EditPtr[i]->Enabled=true;
            EditPtr[i]->Text=0;
        }
    }

    if(iRotate_Type==eInOutArm1Motor)
        palRotate->Visible=false;
    else if(USE_ROTATE_KIT)                                                     //kevin 20130420
        palRotate->Visible=true;
    else
        palRotate->Visible=false;

    if(USE_SOCKET_SENSOR)                                                       //kevin 20130510  //JerryYang 20200327 Socket sensor改用硬體選項判斷
        palSocketSensor->Visible=true;
    else
        palSocketSensor->Visible=false;

    if(USE_COLOR_TRAY_SENSOR && COLOR_AMP_QTY>0)                                //wei 20180409
        palColorSensor->Visible=true;
    else
        palColorSensor->Visible=false;
}
//---------------------------------------------------------------------------
void __fastcall TfCCLink::SaveSetting()
{
    AnsiString FileName="D:\\HT9045\\config\\config.ini";
    WriteIniData(FileName, "CCLink", "Input Method",    rgInputMethod->ItemIndex);
    WriteIniData(FileName, "CCLink", "Percentage",      atoi(edPercentage->Text.c_str()));
    WriteIniData(FileName, "CCLink", "Default Value",   atoi(edDefaultValue->Text.c_str()));
    WriteIniData(FileName, "CCLink", "Min Value",       iMinValue);
    fMain->BackupSetupFile();                                                   //Ifor 20170620 (wei) add Auto BackUp Setup File & Last Data
}
//---------------------------------------------------------------------------
void __fastcall TfCCLink::spbSaveClick(TObject *Sender)
{
    SaveSetting();
    ReadSetting();
}
//---------------------------------------------------------------------------
void __fastcall TfCCLink::edPercentageMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 100, 1);
}
//---------------------------------------------------------------------------
void __fastcall TfCCLink::edDefaultValueMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    fQwertyKey->ShowQwertyKey((TEdit*)Sender, N_INTEGER, 0, true, 3900, 10);
}
//---------------------------------------------------------------------------
void __fastcall TfCCLink::btSetIn2Click(TObject *Sender)
{
    int iValue[8]={0, 0, 0, 0, 0, 0, 0, 0};
    short sNu;
    sNu=btIn2_1->Tag/16;
    sSen=btIn2_1->Tag%16;

    iAllSetCount=iSensorMax;
    for(int i=0; i<iSensorMax; i++)
    {
        if(iInputMethod==1)
        {
            if(UseCanBusOrEtherCAT())                                           //Sam 20230707 : EtherCAT Shuttle sensor
            {
                iValue[i]=GetValue(sNu, sSen-i)*iPercentage/100;                //Sam 20230707 : EtherCAT Shuttle sensor
            }
            else
            {
                iValue[i]=MyCCLink->CCLinkBoard->MyNCUL[sNu].iCurrValue[sSen-i]*iPercentage/100;
            }
            if(iValue[i]>4095)
                iValue[i]=iValue[i]*0.9;
            if(iValue[i]<10)                                                    //JerryYang 20160706 超豐賢哥要求100->10
                iValue[i]=10;
            if(CUSTOMER_CODE==CC_JCET)
            {
                if(iValue[i]<iMinValue)
                {
                    iValue[i]=iMinValue;
                }
            }

            EditPtr[sSen-i]->Text=AnsiString(iValue[i]);
        }
        else
        {
            iValue[i]=atoi(EditPtr[sSen-i]->Text.c_str());
            if(iValue[i]<10)                                                    //JerryYang 20160919 超豐賢哥要求100->10
                iValue[i]=10;
            if(CUSTOMER_CODE==CC_JCET)
            {
                if(iValue[i]<iMinValue)
                {
                    iValue[i]=iMinValue;
                }
            }
        }
    }

    if(UseCanBusOrEtherCAT())                                                   //Sam 20230707 : EtherCAT Shuttle sensor
    {
        for(int i=0; i<iSensorMax; i++)
        {
            SetIOValue(sNu, sSen-i, iValue[i]);                                 //Sam 20230707 : EtherCAT Shuttle sensor
        }
    }
    else
    {
        MyCCLink->DoSetMultiValue(sNu, sSen, iValue, iSensorMax);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfCCLink::btSetIn1Click(TObject *Sender)
{
    int iValue[8]={0, 0, 0, 0, 0, 0, 0, 0};
    short sNu;
    sNu=btIn1_1->Tag/16;
    sSen=btIn1_1->Tag%16;

    iAllSetCount=iSensorMax;
    for(int i=0; i<iSensorMax; i++)
    {
        if(iInputMethod==1)
        {
            if(UseCanBusOrEtherCAT())                                           //Sam 20230707 : EtherCAT Shuttle sensor
            {
                iValue[i]=GetValue(sNu, sSen-i)*iPercentage/100;                //Sam 20230707 : EtherCAT Shuttle sensor
            }
            else
            {
                iValue[i]=MyCCLink->CCLinkBoard->MyNCUL[sNu].iCurrValue[sSen-i]*iPercentage/100;
            }

            if(iValue[i]>4095)
                iValue[i]=iValue[i]*0.9;
            if(iValue[i]<10)                                                    //JerryYang 20160706 超豐賢哥要求100->10
                iValue[i]=10;
            if(CUSTOMER_CODE==CC_JCET)
            {
                if(iValue[i]<iMinValue)
                {
                    iValue[i]=iMinValue;
                }
            }
            EditPtr[sSen-i]->Text=AnsiString(iValue[i]);
        }
        else
        {
            iValue[i]=atoi(EditPtr[sSen-i]->Text.c_str());
            if(iValue[i]<10)                                                    //JerryYang 20160919 超豐賢哥要求100->10
                iValue[i]=10;
            if(CUSTOMER_CODE==CC_JCET)
            {
                if(iValue[i]<iMinValue)
                {
                    iValue[i]=iMinValue;
                }
            }
        }
    }

    if(UseCanBusOrEtherCAT())                                                   //Sam 20230707 : EtherCAT Shuttle sensor
    {
        for(int i=0; i<iSensorMax; i++)
        {
            SetIOValue(sNu, sSen-i, iValue[i]);                                 //Sam 20230707 : EtherCAT Shuttle sensor
        }
    }
    else
    {
        MyCCLink->DoSetMultiValue(sNu, sSen, iValue, iSensorMax);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfCCLink::ShowUseSensor(int iType, int iPitch)
{
    int iLedVisiblePos=8;
    int iLedUnvisiblePos=-30;                                                   //Steven 20131001 : LED元件不能Visible=false, 所以改用換位置的方式

    if(USE_COLOR_TRAY_SENSOR==1 && COLOR_AMP_QTY>0)                             //JerryYang 20191003 fix cc-link顯示異常
    {
        for(int i=24; i<28; i++)
        {
            EditPtr[i]->Visible=false;
            ButtonPtr[i]->Visible=false;
            LamePtr[i]->Top=iLedUnvisiblePos;
        }
    }

    InitShuttleThreadParameter();

    for(int i=0; i<8; i++)                                                      //Steven 20250429 : 統一整理shuttle sensor顯示
    {
        if(SThreadPara.bUseInShtSen[i]==true)
        {
            EdInSht1[i]->Visible=true;
            EdInSht2[i]->Visible=true;
            BtnInSht1[i]->Visible=true;
            BtnInSht2[i]->Visible=true;
            LedInSht1[i]->Top=iLedVisiblePos;
            LedInSht2[i]->Top=iLedVisiblePos;
        }
        else
        {
            EdInSht1[i]->Visible=false;
            EdInSht2[i]->Visible=false;
            BtnInSht1[i]->Visible=false;
            BtnInSht2[i]->Visible=false;
            LedInSht1[i]->Top=iLedUnvisiblePos;
            LedInSht2[i]->Top=iLedUnvisiblePos;
        }
    }

    if(MachineTypeChoice==Type_HT9045 &&
       (SHUTTLE_SENSOR_TYPE==eSensorCCLink3 ||
        SHUTTLE_SENSOR_TYPE==eSensorCanBus3))                                   //Steven 20131008 : for HT9046AH
    {
        mtIn2->XItem=8;
        mtIn1->XItem=8;
        ledInSht2_7->Top=-30;
        ledInSht2_8->Top=-30;
        ledInSht1_7->Top=-30;
        ledInSht1_8->Top=-30;
    }
    else
    {
        mtIn2->XItem=8;
        mtIn1->XItem=8;
//        ledInSht2_7->Top=8;                                                    //Steven 20141022 : Mark
//        ledInSht2_8->Top=8;
//        ledInSht1_7->Top=8;
//        ledInSht1_8->Top=8;
    }

    if(MachineTypeChoice==Type_HT9045 && UseSensorMod3())                       //Sam 20230707 : EtherCAT Shuttle sensor //Steven 20140124 : Fixed for 12 Site CC-Link Sensor
    {
        edIn2_7->Visible=true;
        edIn2_8->Visible=true;
        edIn1_7->Visible=true;
        edIn1_8->Visible=true;
        btIn2_7->Visible=true;
        btIn2_8->Visible=true;
        btIn1_7->Visible=true;
        btIn1_8->Visible=true;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfCCLink::FormDestroy(TObject *Sender)
{
    try
    {
        MySleep(100);
        delete MyCCLink;
        MySleep(100);
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfCCLink::FormDestroy");
    }
    LogSoftwareOffTime("TfCCLink, FormDestroy");                                //Steven 20210526 : 紀錄軟體執行時間
}
//---------------------------------------------------------------------------
void __fastcall TfCCLink::SetSocketClick(TObject *Sender)                       //kevin 20130420 socket sensor
{
    int iValue[8]={0, 0, 0, 0, 0, 0, 0, 0};
    short sNu;

    sNu=2;
    sSen=SOCKET_AMP_QTY-1;                                                      //JerryYang 20200327 重新整理第三個通訊模組

    for(int i=0; i<SOCKET_AMP_QTY; i++)
    {
        if(iInputMethod==1)
        {
            if(UseCanBusOrEtherCAT())                                           //Sam 20230707 : EtherCAT Shuttle sensor
            {
                iValue[i]=GetValue(sNu, i)*iPercentage/100;                     //Sam 20230707 : EtherCAT Shuttle sensor
            }
            else
            {
                iValue[i]=MyCCLink->CCLinkBoard->MyNCUL[sNu].iCurrValue[sSen-i]*iPercentage/100;
            }

            if(iValue[i]>4095)
                iValue[i]=iValue[i]*0.9;
            if(iValue[i]<100)
                iValue[i]=100;

            if(UseCanBusOrEtherCAT())                                           //Sam 20230707 : EtherCAT Shuttle sensor
            {
                EditPtr[20+i]->Text=AnsiString(iValue[i]);
            }
            else
            {
                if(SOCKET_AMP_QTY==4)                                           //JerryYang 20191003 fix cc-link顯示異常
                    EditPtr[23-i]->Text=AnsiString(iValue[i]);
                else
                    EditPtr[27-i]->Text=AnsiString(iValue[i]);
            }
        }
        else
        {
            if(UseCanBusOrEtherCAT())                                           //Sam 20230707 : EtherCAT Shuttle sensor
            {
                iValue[i]=atoi(EditPtr[20+i]->Text.c_str());
            }
            else
            {
                if(SOCKET_AMP_QTY==4)                                           //JerryYang 20191003 fix cc-link顯示異常
                    iValue[i]=atoi(EditPtr[23-i]->Text.c_str());
                else
                    iValue[i]=atoi(EditPtr[27-i]->Text.c_str());
            }
        }
    }

    if(UseCanBusOrEtherCAT())                                                   //Sam 20230707 : EtherCAT Shuttle sensor
    {
        for(int i=0; i<SOCKET_AMP_QTY; i++)
        {
            SetIOValue(sNu, i, iValue[i]);                                      //Sam 20230707 : EtherCAT Shuttle sensor
        }
    }
    else
    {
        MyCCLink->DoSetMultiValue(sNu, sSen, iValue, SOCKET_AMP_QTY);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfCCLink::SetRotateClick(TObject *Sender)                       //kevin 20130420 socket sensor
{
    int iValue[4]={0, 0, 0, 0};
    short sNu;

    sNu=2;
    sSen=0;
    if(USE_SOCKET_SENSOR && SOCKET_AMP_QTY>0 && SOCKET_AMP_QTY<=8)              //JerryYang 20200327 重新整理第三個通訊模組
    {
        sSen+=SOCKET_AMP_QTY;
    }
    if(USE_ROTATE_KIT && ROTATE_AMP_QTY>0)
    {
        sSen+=ROTATE_AMP_QTY;
    }
    sSen=sSen-1;

    for(int i=0; i<ROTATE_AMP_QTY; i++)
    {
        if(iInputMethod==1)
        {
            if(UseCanBusOrEtherCAT())                                           //Sam 20230707 : EtherCAT Shuttle sensor
            {
                iValue[i]=GetValue(sNu, iRotateStart+i)*iPercentage/100;        //Sam 20230707 : EtherCAT Shuttle sensor
            }
            else
            {
                iValue[i]=MyCCLink->CCLinkBoard->MyNCUL[sNu].iCurrValue[sSen-i]*iPercentage/100;
            }
            if(iValue[i]>4095)
                iValue[i]=iValue[i]*0.9;
            if(iValue[i]<100)
                iValue[i]=100;

            if(UseCanBusOrEtherCAT())                                           //Sam 20230707 : EtherCAT Shuttle sensor
            {
                EditPtr[28+i]->Text=AnsiString(iValue[i]);
            }
            else
            {
                if(ROTATE_AMP_QTY==4)
                    EditPtr[31-i]->Text=AnsiString(iValue[i]);
                else
                    EditPtr[29-i]->Text=AnsiString(iValue[i]);
            }
        }
        else
        {
            if(UseCanBusOrEtherCAT())                                           //Sam 20230707 : EtherCAT Shuttle sensor
                iValue[i]=atoi(EditPtr[28+i]->Text.c_str());
            else
            {
                if(ROTATE_AMP_QTY==4)
                    iValue[i]=atoi(EditPtr[31-i]->Text.c_str());
                else
                    iValue[i]=atoi(EditPtr[29-i]->Text.c_str());

            }

        }
    }

    if(UseCanBusOrEtherCAT())                                                   //Sam 20230707 : EtherCAT Shuttle sensor
    {
        for(int i=0; i<ROTATE_AMP_QTY; i++)
        {
            SetIOValue(sNu, i+iRotateStart, iValue[i]);                         //Sam 20230707 : EtherCAT Shuttle sensor
        }
    }
    else
    {
        MyCCLink->DoSetMultiValue(sNu, sSen, iValue, ROTATE_AMP_QTY);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfCCLink::btIn3_1MouseDown(TObject *Sender,                     //kevin 20130420 socket sensor
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TButton *Ptr;
    Ptr=(TButton *)Sender;
    int iValue=0;
    short sNu;
    sNu=2;
    sSen=Ptr->Tag;

    if(iInputMethod==1)
    {
        if(UseCanBusOrEtherCAT())                                               //Sam 20230707 : EtherCAT Shuttle sensor
        {
            iValue=GetValue(sNu, sSen)*iPercentage/100;                         //Sam 20230707 : EtherCAT Shuttle sensor
        }
        else
        {
            iValue=MyCCLink->CCLinkBoard->MyNCUL[sNu].iCurrValue[sSen]*iPercentage/100;
        }
        if(iValue>4095)
            iValue=iValue*0.9;
        if(iValue<100)
            iValue=100;

        EditPtr[20+Ptr->Tag]->Text=AnsiString(iValue);
    }
    else
    {
        iValue=atoi(EditPtr[20+Ptr->Tag]->Text.c_str());
    }

    if(UseCanBusOrEtherCAT())                                                   //Sam 20230707 : EtherCAT Shuttle sensor
    {
        SetIOValue(sNu, sSen, iValue);                                          //Sam 20230707 : EtherCAT Shuttle sensor
    }
    else
    {
        MyCCLink->DoSetValue(sNu, sSen, iValue);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfCCLink::btIn3_9MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TButton *Ptr;
    Ptr=(TButton *)Sender;
    int iValue=0;
    short sNu;
    sNu=2;
    sSen=Ptr->Tag;

    if(iInputMethod==1)
    {
        if(UseCanBusOrEtherCAT())                                               //Sam 20230707 : EtherCAT Shuttle sensor //JerryYang 20200327 重新整理第三個通訊模組
        {
            iValue=GetValue(sNu, sSen+iRotateStart)*iPercentage/100;            //Sam 20230707 : EtherCAT Shuttle sensor
        }
        else
        {
            iValue=MyCCLink->CCLinkBoard->MyNCUL[sNu].iCurrValue[sSen+iRotateStart]*iPercentage/100;
        }
        if(iValue>4095)
            iValue=iValue*0.9;
        if(iValue<=0)
            iValue=1;

         EditPtr[28+Ptr->Tag]->Text=AnsiString(iValue);
    }
    else
    {
        iValue=atoi(EditPtr[28+Ptr->Tag]->Text.c_str());
    }

    if(UseCanBusOrEtherCAT())                                                   //Sam 20230707 : EtherCAT Shuttle sensor
    {
        SetIOValue(sNu, sSen+iRotateStart, iValue);                             //Sam 20230707 : EtherCAT Shuttle sensor
    }
    else
    {
        MyCCLink->DoSetValue(sNu, sSen+iRotateStart, iValue);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfCCLink::btShuttlePositionMoveClick(TObject *Sender)
{
    if(fSecurity->Insufficient(122)==false)
        return;

    if(SystemStart)
        return;

    if(InArmSuck.HasIC()==false && OutArmSuck.HasIC()==false &&
       ShuttleHasIC()==false && IndexHasIC()==false)
    {
        fShuttleMove->ShowModal();
    }
    else
    {
        ShowMyMessage("Please Clean Out or One Cycle", "請完成Clean Out或One Cycle");
    }
}
//---------------------------------------------------------------------------
void __fastcall TfCCLink::btIn3_13MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TButton *Ptr;
    Ptr=(TButton *)Sender;
    int iValue=0;
    short sNu;
    sNu=2;
    sSen=Ptr->Tag;

    if(iInputMethod==1)
    {
        if(UseCanBusOrEtherCAT())                                               //Sam 20230707 : EtherCAT Shuttle sensor //JerryYang 20200327 重新整理第三個通訊模組
        {
            iValue=GetValue(sNu, sSen+iColorStart)*iPercentage/100;             //Sam 20230707 : EtherCAT Shuttle sensor
        }
        else
        {
            iValue=MyCCLink->CCLinkBoard->MyNCUL[sNu].iCurrValue[sSen+iColorStart]*iPercentage/100;
        }
        if(iValue>4095)
            iValue=iValue*0.9;
        if(iValue<100)
            iValue=100;

        EditPtr[32+Ptr->Tag]->Text=AnsiString(iValue);
    }
    else
    {
        iValue=atoi(EditPtr[32+Ptr->Tag]->Text.c_str());
    }

    if(UseCanBusOrEtherCAT())                                                   //Sam 20230707 : EtherCAT Shuttle sensor
    {
        SetIOValue(sNu,sSen+iColorStart,iValue);
    }
    else
    {
        MyCCLink->DoSetValue(sNu, sSen, iValue+iColorStart);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfCCLink::SetColorClick(TObject *Sender)
{
    int iValue[6]={0, 0, 0, 0, 0, 0};
    short sNu;
    sNu=2;
    sSen=0;

    if(USE_SOCKET_SENSOR && SOCKET_AMP_QTY>0 && SOCKET_AMP_QTY<=8)              //JerryYang 20200327 重新整理第三個通訊模組
    {
        sSen+=SOCKET_AMP_QTY;
    }
    if(USE_COLOR_TRAY_SENSOR==1 && COLOR_AMP_QTY>0)                             //JerryYang 20191003 fix cc-link顯示異常
    {
        sSen+=COLOR_AMP_QTY;
    }
    if(USE_ROTATE_KIT && ROTATE_AMP_QTY>0)
    {
        sSen+=ROTATE_AMP_QTY;
    }
    sSen=sSen-1;

    for(int i=0; i<COLOR_AMP_QTY; i++)
    {
        if(iInputMethod==1)
        {
            if(UseCanBusOrEtherCAT())                                           //Sam 20230707 : EtherCAT Shuttle sensor
            {
                iValue[i]=GetValue(sNu, iColorStart+i)*iPercentage/100;         //Sam 20230707 : EtherCAT Shuttle sensor
            }
            else
            {
                iValue[i]=MyCCLink->CCLinkBoard->MyNCUL[sNu].iCurrValue[sSen-i]*iPercentage/100;
            }
            if(iValue[i]>4095)
                iValue[i]=iValue[i]*0.9;
            if(iValue[i]<100)
                iValue[i]=100;
            if(SHUTTLE_SENSOR_TYPE==eSensorCanBus || SHUTTLE_SENSOR_TYPE==eSensorCanBus3)
            {
                EditPtr[32+i]->Text=AnsiString(iValue[i]);
            }
            else
            {
                EditPtr[37-i]->Text=AnsiString(iValue[i]);
            }
        }
        else
        {
            if(UseCanBusOrEtherCAT())                                           //Sam 20230707 : EtherCAT Shuttle sensor
            {
                iValue[i]=atoi(EditPtr[32+i]->Text.c_str());
            }
            else
            {
                iValue[i]=atoi(EditPtr[37-i]->Text.c_str());
            }
        }
    }

    if(UseCanBusOrEtherCAT())                                                   //Sam 20230707 : EtherCAT Shuttle sensor
    {
        for(int i=0; i<COLOR_AMP_QTY; i++)
        {
            SetIOValue(sNu, i+iColorStart, iValue[i]);                          //Sam 20230707 : EtherCAT Shuttle sensor
        }
    }
    else
    {
        MyCCLink->DoSetMultiValue(sNu, sSen, iValue, COLOR_AMP_QTY);
    }
}
//---------------------------------------------------------------------------
void __fastcall TfCCLink::btSaveClick(TObject *Sender)
{
    SaveSetupFile();                                                            //wei 20180622 (Steven) : CCLink Value Save File
}
//---------------------------------------------------------------------------
void __fastcall TfCCLink::btReadClick(TObject *Sender)
{
    ReadSetupFile();                                                            //wei 20180622 (Steven) : CCLink Value Save File
}
//---------------------------------------------------------------------------
void __fastcall TfCCLink::GetStationStart()                                     //JerryYang 20200327 取得socket/ rotate/ color起始站號
{
    iRotateStart=0;
    iColorStart=0;
    if(USE_SOCKET_SENSOR && SOCKET_AMP_QTY>0 && SOCKET_AMP_QTY<=8)
    {
        iRotateStart=SOCKET_AMP_QTY;
    }
    else
    {
        iRotateStart=0;
    }
    iColorStart=iRotateStart;
    if(USE_ROTATE_KIT && ROTATE_AMP_QTY>0)
    {
        iColorStart+=ROTATE_AMP_QTY;
    }
}
//---------------------------------------------------------------------------
//Sam 20210518 : 新增 CanBus 軟體配置
//==>
void __fastcall TfCCLink::btnCanBusSeachClick(TObject *Sender)
{
    if(DNM100UD->bCanBusSeach==false)
    {
        mmoCanBusLog->Clear();
        DNM100UD->bCanBusSeach=true;
        iCanBusSrachTask=1;
    }
    else
    {
        mmoCanBusLog->Lines->Add("Searching");
    }
}
//---------------------------------------------------------------------------
void __fastcall TfCCLink::tmrCanBusSearchTimer(TObject *Sender)
{
    int iRet;
    int &Task=iCanBusSrachTask;
    int iAmplifierHW;
//    int iAmplifierSW;
    AnsiString msg="";
    AnsiString errMsg="";

    if(DNM100UD==NULL || DNM100UD->bCanBusSeach==false)
        return;

    tmrCanBusSearch->Enabled=false;
    switch(Task)
    {
        case 1:
            iSearchCnt=0;
            iIsSearchOKErrCnt=0;
            SearchTotalDevices=0;
            ZeroMemory(SearchDesMACID,          sizeof(SearchDesMACID));
            ZeroMemory(SearchType,              sizeof(SearchType));
            ZeroMemory(SearchDeviceInputLen,    sizeof(SearchDeviceInputLen));
            ZeroMemory(SearchDeviceOutputLen,   sizeof(SearchDeviceOutputLen));
            //break;
        case 100:
            iRet=DNM100_SearchAllDevices(0);
            if(iRet==DNM100_NoError)
            {
                if(iSearchCnt==0)
                {
                    msg.sprintf("search DNM100 start, DLL:%s, Firmware:%s", DNM100UD->asDLLVer, DNM100UD->asFirmwareVer);
                    mmoCanBusLog->Lines->Add(msg);
                }
                Task=200;
            }
            else
            {
                errMsg=DNM100UD->asSearchErrCodeMsg(iRet);
                msg.sprintf("search DNM100 task 100 error %d = %s", iRet, errMsg);
                DNM100UD->bCanBusSeach=false;
                mmoCanBusLog->Lines->Add(msg);
            }
            break;
        case 200:
            iRet=DNM100_IsSearchOK(0);
            if(iRet==DNM100_NoError)
            {
                Task=300;
            }
            else
            {
                iIsSearchOKErrCnt++;
                if(iIsSearchOKErrCnt>100)
                {
                    errMsg=DNM100UD->asSearchErrCodeMsg(iRet);
                    msg.sprintf("search DNM100 task 200 error %d = %s", iRet, errMsg);
                    DNM100UD->bCanBusSeach=false;
                }
                else
                {
                    msg.sprintf("seach count : %d", iIsSearchOKErrCnt);
                }
                mmoCanBusLog->Lines->Add(msg);
            }
            break;
        case 300:
            iRet=DNM100_GetSearchedDevices(0, &SearchTotalDevices, SearchDesMACID, SearchType, SearchDeviceInputLen, SearchDeviceOutputLen);
            if(iRet==DNM100_NoError)
            {
                if(SearchTotalDevices==0)
                {
                     iSearchCnt++;                                              //找不到再重找
                     if(iSearchCnt<=3)
                     {
                         Task=100;
                         break;
                     }
                }
                msg.sprintf("seach NUDN1:%d", SearchTotalDevices);
                mmoCanBusLog->Lines->Add(msg);
                for(int i=0; i<SearchTotalDevices; i++)
                {
                    iAmplifierHW=(SearchDeviceInputLen[i]-8)/2;
                    //MACID                 : NUDN1 站號指撥
                    //iAmplifier            : NUDN1 連結 FSN12 數量
                    //SearchType            : NUDN1 通訊指撥
                    msg.sprintf("seach MACID:%d, FS-N12N:%d, Type:%d", SearchDesMACID[i], iAmplifierHW, SearchType[i]);
                    mmoCanBusLog->Lines->Add(msg);
                }
                DNM100UD->bCanBusSeach=false;
                Task=1;
                msg.sprintf("search DNM100 finish");
                mmoCanBusLog->Lines->Add(msg);
            }
            else
            {
                errMsg=DNM100UD->asSearchErrCodeMsg(iRet);
                msg.sprintf("search DNM100 task 300 error %d = %s", iRet, errMsg);
                mmoCanBusLog->Lines->Add(msg);
                DNM100UD->bCanBusSeach=false;
            }
            break;
    }
    tmrCanBusSearch->Enabled=true;
}
//---------------------------------------------------------------------------
void __fastcall TfCCLink::btnReadCanBusSettingClick(TObject *Sender)
{
    AnsiString msg="";

    msg.sprintf("read NUDN1:%d", iTotalDevices);
    mmoCanBusLog->Lines->Add(msg);
    for(int i=0; i<iTotalDevices; i++)
    {
        //MACID                 : NUDN1 站號指撥
        //iAmplifier            : NUDN1 連結 FS-N12 數量
        //Type                  : NUDN1 通訊指撥
        msg.sprintf("read MACID:%d, FS-N12N:%d, Type:%d", iDesMACIDList[i], iAmplifierList[i], ConType_Poll);
        mmoCanBusLog->Lines->Add(msg);
    }
}
//---------------------------------------------------------------------------
//<==
//Sam 20210518 : 新增 CanBus 軟體配置
void __fastcall TfCCLink::FormShortCut(TWMKey &Msg, bool &Handled)
{
    if(Msg.CharCode==VK_F5)                                                     //Sam 20220725 : 介面新增鍵盤 F5 將視窗恢復初始位置
    {
        Left=75;
        Top=10;
    }
}
//---------------------------------------------------------------------------
////Sam 20230707 : EtherCAT Shuttle sensor
//==>
int TfCCLink::GetValue(int iNU,int iAMPNo)
{
    int iValue=0;
    if(SHUTTLE_SENSOR_TYPE==eSensorCanBus || SHUTTLE_SENSOR_TYPE==eSensorCanBus3)
        iValue=DNM100UD->NUDN1Item[iNU]->iGetValue(iAMPNo);
    else if(SHUTTLE_SENSOR_TYPE==eSensorEtherCAT || SHUTTLE_SENSOR_TYPE==eSensorEtherCAT3)
        iValue=MyEtherCAT->NUEC1Item[iNU]->iGetValue(iAMPNo);
    else
        iValue=MyCCLink->CCLinkBoard->MyNCUL[iNU].iCurrValue[iAMPNo];
    return iValue;
}
//---------------------------------------------------------------------------
int TfCCLink::GetIOValue(int iNU,int iAMPNo)
{
    int iValue=0;
    if(SHUTTLE_SENSOR_TYPE==eSensorCanBus || SHUTTLE_SENSOR_TYPE==eSensorCanBus3)
        iValue=DNM100UD->NUDN1Item[iNU]->iGetIO_Value[iAMPNo];
    else if(SHUTTLE_SENSOR_TYPE==eSensorEtherCAT || SHUTTLE_SENSOR_TYPE==eSensorEtherCAT3)
        iValue=MyEtherCAT->NUEC1Item[iNU]->iGetIO_Value(iAMPNo);
    else
        iValue=MyCCLink->CCLinkBoard->MyNCUL[iNU].iSetValue[iAMPNo];
    return iValue;
}
//---------------------------------------------------------------------------
bool TfCCLink::GetIO(int iNU,int iAMPNo)
{
    if(SHUTTLE_SENSOR_TYPE==eSensorCanBus || SHUTTLE_SENSOR_TYPE==eSensorCanBus3)
        return DNM100UD->NUDN1Item[iNU]->bGetIO(iAMPNo);
    else if(SHUTTLE_SENSOR_TYPE==eSensorEtherCAT || SHUTTLE_SENSOR_TYPE==eSensorEtherCAT3)
        return MyEtherCAT->NUEC1Item[iNU]->bGetIO(iAMPNo);
    else
        return false;
}
//---------------------------------------------------------------------------
void TfCCLink::SetIOValue(int iNU,int iAMPNo,int iValue)
{
    if(SHUTTLE_SENSOR_TYPE==eSensorCanBus || SHUTTLE_SENSOR_TYPE==eSensorCanBus3)
        DNM100UD->NUDN1Item[iNU]->iSetIO_Value[iAMPNo]=iValue;
    else if(SHUTTLE_SENSOR_TYPE==eSensorEtherCAT || SHUTTLE_SENSOR_TYPE==eSensorEtherCAT3)
        MyEtherCAT->NUEC1Item[iNU]->SetSetIOValue(iAMPNo,iValue);
    else
        MyCCLink->DoSetValue(iNU, iAMPNo, iValue);
}
//---------------------------------------------------------------------------
void TfCCLink::Reset()
{
    if(SHUTTLE_SENSOR_TYPE==eSensorCanBus || SHUTTLE_SENSOR_TYPE==eSensorCanBus3)
    {
        DNM100UD->TaskClear();
    }
    else if(SHUTTLE_SENSOR_TYPE==eSensorEtherCAT || SHUTTLE_SENSOR_TYPE==eSensorEtherCAT3)
    {
        MyEtherCAT->TaskClear();
    }
    else
    {
        MyCCLink->CCLinkStatus=true;
        MyCCLink->CCLinkBoard->ClearData();                                     //Setting Value
        MyCCLink->bSetMultiValue=false;
    }
    sbReset->Visible=false;
    sbReset->Down=false;
}
//---------------------------------------------------------------------------
void TfCCLink::SetInitialOK(bool bInitailOK)
{
    if(fCCLink->MyCCLink!=NULL && (SHUTTLE_SENSOR_TYPE==eSensorCCLink || SHUTTLE_SENSOR_TYPE==eSensorCCLink3))          //Steven 20131008 : for HT9046AH
        fCCLink->MyCCLink->InitialOK=InitialOK;
    else if(fCCLink->DNM100UD!=NULL && (SHUTTLE_SENSOR_TYPE==eSensorCanBus || SHUTTLE_SENSOR_TYPE==eSensorCanBus3))     //Sam 20190709 : Add CanBus
        fCCLink->DNM100UD->InitialOK=InitialOK;
    else if(MyEtherCAT!=NULL && (SHUTTLE_SENSOR_TYPE==eSensorEtherCAT || SHUTTLE_SENSOR_TYPE==eSensorEtherCAT3))
        MyEtherCAT->InitialOK=InitialOK;
}
//---------------------------------------------------------------------------
bool TfCCLink::bHasError()
{
    bool bRet=false;
    if(SHUTTLE_SENSOR_TYPE==eSensorCCLink || SHUTTLE_SENSOR_TYPE==eSensorCCLink3)           //Steven 20131008 : for HT9046AH
        bRet=fCCLink->MyCCLink->HasError;
    else if(SHUTTLE_SENSOR_TYPE==eSensorCanBus || SHUTTLE_SENSOR_TYPE==eSensorCanBus3)      //Sam 20190709 : Add CanBus
        bRet=fCCLink->DNM100UD->HasError;
    else if(SHUTTLE_SENSOR_TYPE==eSensorEtherCAT || SHUTTLE_SENSOR_TYPE==eSensorEtherCAT3)
        bRet=MyEtherCAT->HasError;
    return bRet;
}
//---------------------------------------------------------------------------
AnsiString TfCCLink::GetErrorMessage()
{
    AnsiString sErr="";
    if(SHUTTLE_SENSOR_TYPE==eSensorCCLink || SHUTTLE_SENSOR_TYPE==eSensorCCLink3)           //Steven 20131008 : for HT9046AH
        sErr=fCCLink->MyCCLink->ErrorMessage;
    else if(SHUTTLE_SENSOR_TYPE==eSensorCanBus || SHUTTLE_SENSOR_TYPE==eSensorCanBus3)      //Sam 20190709 : Add CanBus
        sErr=fCCLink->DNM100UD->ErrorMessage;
    else if(SHUTTLE_SENSOR_TYPE==eSensorEtherCAT || SHUTTLE_SENSOR_TYPE==eSensorEtherCAT3)
        sErr=MyEtherCAT->ErrorMessage;
    return sErr;
}
//---------------------------------------------------------------------------
bool UseCanBusOrEtherCAT()
{
    if(SHUTTLE_SENSOR_TYPE==eSensorCanBus || SHUTTLE_SENSOR_TYPE==eSensorCanBus3 || SHUTTLE_SENSOR_TYPE==eSensorEtherCAT || SHUTTLE_SENSOR_TYPE==eSensorEtherCAT3)
        return true;
    else
        return false;
}
//---------------------------------------------------------------------------
bool UseSensorMod3()
{
    if(SHUTTLE_SENSOR_TYPE==eSensorCCLink3 || SHUTTLE_SENSOR_TYPE==eSensorCanBus3 || SHUTTLE_SENSOR_TYPE==eSensorEtherCAT3)
        return true;
    else
        return false;
}
//---------------------------------------------------------------------------
//<==
//Sam 20230707 : EtherCAT Shuttle sensor
//---------------------------------------------------------------------------
void SetIOTableByNUEC1()                                                        //Sam 20230725 : EtherCAT NUEC1 IO 取代線路 IO
{
    if(SHUTTLE_SENSOR_TYPE==eSensorEtherCAT || SHUTTLE_SENSOR_TYPE==eSensorEtherCAT3)
    {
        //第一顆 NU-EC1
        if(MachineTypeChoice!=Type_HT9045)
        {
            //iAmplifierList[0]==16
            int iAmp1SHTSort[16]={SnInPutSHT2S8,
                                  SnInPutSHT2S7,
                                  SnInPutSHT2S6,
                                  SnInPutSHT2S5,
                                  SnInPutSHT2S4,
                                  SnInPutSHT2S3,
                                  SnInPutSHT2S2,
                                  SnInPutSHT2S1,
                                  SnInPutSHT1S8,
                                  SnInPutSHT1S7,
                                  SnInPutSHT1S6,
                                  SnInPutSHT1S5,
                                  SnInPutSHT1S4,
                                  SnInPutSHT1S3,
                                  SnInPutSHT1S2,
                                  SnInPutSHT1S1};
            SetIOTableByNUEC1(0, 0, &iAmp1SHTSort[0], sizeof(iAmp1SHTSort)/sizeof(iAmp1SHTSort[0]));
            //第二顆 NU-EC1
            //iAmplifierList[1]==4
            int iAmp2SHTSort[4]={SnInPutSHT1S9,
                                 SnInPutSHT2S9,
                                 SnOutPutSHT1S1,
                                 SnOutPutSHT2S1};
            SetIOTableByNUEC1(1, 0, &iAmp2SHTSort[0], sizeof(iAmp2SHTSort)/sizeof(iAmp2SHTSort[0]));
        }
        else if(MachineTypeChoice==Type_HT9045 && UseSensorMod3())
        {
            //iAmplifierList[0]==12
            int iAmp1SHTSort[12]={SnInPutSHT2S6,
                                  SnInPutSHT2S5,
                                  SnInPutSHT2S4,
                                  SnInPutSHT2S3,
                                  SnInPutSHT2S2,
                                  SnInPutSHT2S1,
                                  SnInPutSHT1S6,
                                  SnInPutSHT1S5,
                                  SnInPutSHT1S4,
                                  SnInPutSHT1S3,
                                  SnInPutSHT1S2,
                                  SnInPutSHT1S1};
            SetIOTableByNUEC1(0, 0, &iAmp1SHTSort[0], sizeof(iAmp1SHTSort)/sizeof(iAmp1SHTSort[0]));
            //第二顆 NU-EC1
            //iAmplifierList[1]==4
            int iAmp2SHTSort[4]={SnInPutSHT1S7,
                                 SnInPutSHT2S7,
                                 SnOutPutSHT1S1,
                                 SnOutPutSHT2S1};
            SetIOTableByNUEC1(1, 0, &iAmp2SHTSort[0], sizeof(iAmp2SHTSort)/sizeof(iAmp2SHTSort[0]));      //Sam 20250603 : 修正 EtherCAT By DataBase 問題
        }
        else
        {
            //IO 資料對起來怪怪的，有問題再修改
            //iAmplifierList[0]==14
            int iAmp1SHTSort[14]={SnInPutSHT2S7,
                                  SnInPutSHT2S6,
                                  SnInPutSHT2S5,
                                  SnInPutSHT2S4,
                                  SnInPutSHT2S3,
                                  SnInPutSHT2S2,
                                  SnInPutSHT2S1,
                                  SnInPutSHT1S7,
                                  SnInPutSHT1S6,
                                  SnInPutSHT1S5,
                                  SnInPutSHT1S4,
                                  SnInPutSHT1S3,
                                  SnInPutSHT1S2,
                                  SnInPutSHT1S1};
            SetIOTableByNUEC1(0, 0, &iAmp1SHTSort[0], sizeof(iAmp1SHTSort)/sizeof(iAmp1SHTSort[0]));
            //第二顆 NU-EC1
            //iAmplifierList[1]==4
            int iAmp2SHTSort[4]={SnInPutSHT1S9,
                                 SnInPutSHT2S9,
                                 SnOutPutSHT1S1,
                                 SnOutPutSHT2S1};
            SetIOTableByNUEC1(1, 0, &iAmp2SHTSort[0], sizeof(iAmp2SHTSort)/sizeof(iAmp2SHTSort[0]));  //Sam 20250603 : 修正 EtherCAT By DataBase 問題
        }

        //第三顆 NU-EC1
        //iAmplifierList[2]=SOCKET_AMP_QTY + SOCKET_AMP_QTY + COLOR_AMP_QTY
        int iSenStar=0;
        if(SOCKET_AMP_QTY==4)
        {
            int iAmp3SocketSort[4]={SnSocket1,
                                    SnSocket2,
                                    SnSocket3,
                                    SnSocket4};
            SetIOTableByNUEC1(2, iSenStar, &iAmp3SocketSort[0], sizeof(iAmp3SocketSort)/sizeof(iAmp3SocketSort[0]));
        }
        else if(SOCKET_AMP_QTY==8)
        {
            int iAmp3SocketSort[8]={SnSocket1,
                                    SnSocket2,
                                    SnSocket3,
                                    SnSocket4,
                                    SnSocket5,
                                    SnSocket6,
                                    SnSocket7,
                                    SnSocket8};
            SetIOTableByNUEC1(2, iSenStar, &iAmp3SocketSort[0], sizeof(iAmp3SocketSort)/sizeof(iAmp3SocketSort[0]));
        }
        else if(SOCKET_AMP_QTY>0)
        {
            ShowMyMessage("Dones not support SOCKET Sensor "+IntToStr(SOCKET_AMP_QTY)+"Qty","");
        }
        iSenStar+=SOCKET_AMP_QTY;

        if(ROTATE_AMP_QTY==4)
        {
            int iAmp3RotateSort[4]={SnRotateRowIn1,
                                    SnRotateRowIn2,
                                    SnRotateRowOut1,
                                    SnRotateRowOut2};
            SetIOTableByNUEC1(2, iSenStar, &iAmp3RotateSort[0], sizeof(iAmp3RotateSort)/sizeof(iAmp3RotateSort[0]));
        }
        else if(ROTATE_AMP_QTY==2)                                              //Sam 20250730 : 修正 EtherCAT 2 sensor Rotate 失效問題。
        {
            int iAmp3RotateSort[2]={SnRotateRowIn1,
                                    SnRotateRowOut1};
            SetIOTableByNUEC1(2, iSenStar, &iAmp3RotateSort[0], sizeof(iAmp3RotateSort)/sizeof(iAmp3RotateSort[0]));
        }
        iSenStar+=ROTATE_AMP_QTY;

        if(COLOR_AMP_QTY==5)
        {
            int iAmp3ColorSort[5]={SnFixColorTrayDetect0,
                                   SnFixColorTrayDetect1,
                                   SnFixColorTrayDetect2,
                                   SnAutoColorTrayDetect0,
                                   SnColorColorTrayDetect};
            SetIOTableByNUEC1(2,iSenStar,&iAmp3ColorSort[0],sizeof(iAmp3ColorSort)/sizeof(iAmp3ColorSort[0]));

        }
        else if(COLOR_AMP_QTY==8)
        {
            int iAmp3ColorSort[8]={SnFixColorTrayDetect0,
                                   SnFixColorTrayDetect1,
                                   SnFixColorTrayDetect2,
                                   SnAutoColorTrayDetect0,
                                   SnAutoColorTrayDetect1,
                                   SnAutoColorTrayDetect2,
                                   SnEmptyColorTrayDetect,
                                   SnColorColorTrayDetect};
            SetIOTableByNUEC1(2,iSenStar,&iAmp3ColorSort[0], sizeof(iAmp3ColorSort)/sizeof(iAmp3ColorSort[0]));
        }
        else if(COLOR_AMP_QTY==6)
        {
            int iAmp3ColorSort[6]={SnFixColorTrayDetect0,
                                   SnFixColorTrayDetect1,
                                   SnFixColorTrayDetect2,
                                   SnAutoColorTrayDetect0,
                                   SnAutoColorTrayDetect1,
                                   SnAutoColorTrayDetect2};
            SetIOTableByNUEC1(2,iSenStar,&iAmp3ColorSort[0], sizeof(iAmp3ColorSort)/sizeof(iAmp3ColorSort[0]));
        }
        else if(COLOR_AMP_QTY>0)
        {
            ShowMyMessage("Dones not support COLOR Sensor "+IntToStr(COLOR_AMP_QTY)+"Qty","");
        }
    }
}
//---------------------------------------------------------------------------
void SetIOTableByNUEC1(int iNu,int iSenStart,int *iAmpSort,int iAmpNum)         //Sam 20230725 : EtherCAT NUEC1 IO 取代線路 IO
{
    if(iSenStart+iAmpNum>16)
    {
        ShowMyMessage("SetIOTable By NUEC1 Error over "+IntToStr(iSenStart+iAmpNum), "");
        return;
    }

    for(int i=0; i<iAmpNum; i++)
    {
        if(Sen[iAmpSort[i]].ISABase==ePCI1203)
        {
            Sen[iAmpSort[i]].Ring=MyEtherCAT->NUEC1Item[iNu]->iGetRing;
            Sen[iAmpSort[i]].IP=MyEtherCAT->NUEC1Item[iNu]->iGetDesMACID;
            Sen[iAmpSort[i]].Port=iSenStart+i;
            Sen[iAmpSort[i]].Bit=0;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfCCLink::btIn4_1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    TButton *Ptr;
    Ptr=(TButton *)Sender;
    int iValue=0;
    short sNu;
    sNu=3;
    sSen=Ptr->Tag;

    if(iInputMethod==1)
    {
        if(UseCanBusOrEtherCAT())                                               //Sam 20230707 : EtherCAT Shuttle sensor
        {
            iValue=GetValue(sNu, sSen)*iPercentage/100;                         //Sam 20230707 : EtherCAT Shuttle sensor
        }
        else
        {
            iValue=MyCCLink->CCLinkBoard->MyNCUL[sNu].iCurrValue[sSen]*iPercentage/100;
        }
        if(iValue>4095)
            iValue=iValue*0.9;
        if(iValue<100)
            iValue=100;

        EditPtr[38+Ptr->Tag]->Text=AnsiString(iValue);
    }
    else
    {
        iValue=atoi(EditPtr[38+Ptr->Tag]->Text.c_str());
    }

    if(UseCanBusOrEtherCAT())                                                   //Sam 20230707 : EtherCAT Shuttle sensor
    {
        SetIOValue(sNu, sSen, iValue);                                          //Sam 20230707 : EtherCAT Shuttle sensor
    }
    else
    {
        MyCCLink->DoSetValue(sNu, sSen, iValue);
    }    
}
//---------------------------------------------------------------------------


