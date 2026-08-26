#include "MachineDefine.h"
#pragma hdrstop

#include "adam6024.h"

#include "TextProcess.h"
#include "MachineType.h"
#include "cContact.h"
//#include "adamtcp.h"
#include "MyMotor.h"
#include "cprod.h"
//#include "math.h"
#include "ContactForce.h"
#include "myswitch.h"
#include "cmydef.h"
#include "mymessbox.h"
#include "cMyDB.h"
#include "ADSMOD.h"                                                             //Nickliu 20180827 Add APAX Dll Connection
#include "common.h"
#include "note.h"
#include "main.h"

#include "iosetview.h"
#include "csystem.h"

#define DEFAULT_PORT 502                                                        // Port for Modbus/TCP
#define DEFAULT_ET7226_PORT 10001                                               // Port for Modbus/TCP
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TfAdam6024 *fAdam6024;

unsigned char NetID=0x01;
//-------- default timeout ------
int     iConnectionTimeout=2000;
int     iSendTimeout=2000;
int     iReceiveTimeout=2000;
//---------------------------------------------------------------------------
bool bADAM6420Install=false,bOpen=false;
//AnsiString Address="172.16.8.110";
AnsiString Address[3]={"172.16.8.110", "172.16.8.111", "172.16.8.112"};         //Ifor 20150728 :修改ADAM6024 IP位置為陣列變數
char cAddress[100]={"172.16.8.112"};
double fValue[16];
WORD wGain[10], wHex[16];
bool bCanReadData=true;                                                         //20111217 ChungHung
int iWritePA=0;                                                                 //20111217 ChungHung
int iADAMRange=0;                                                               //20111217 ChungHung
double dADAMRange_Kg=0;                                                         //JerryYang 20171023 (wei) add ADAM Range
void ConnectTcpServerCompletedEventHandler(long lResult, char *i_szIp, void *i_Param);
void DisconnectTcpServerCompletedEventHandler(long lResult, char *i_szIp, void *i_Param);
unsigned long ulClientHandle;                                                   //Nickliu 20180827 Add APAX Dll Connection
bool bConnectStatus[3]={false, false, false};;                                  //Hmy 20170120 add check Adam6024 Connect Status
bool bADAM6420CheckRange[4]={false, false, false, false};                       //ben 20230818 : ADAM check range
static bool bDoubleEPConnectGuideShown=false;                                   //AI(ht9045-v899) 20260505: show EP board setup guidance once per run when Num=2 cannot connect
extern bool MNetLog(AnsiString Message);
typedef enum
{
    Adam6024_AI_mA_4To20    = 7,
    Adam6024_AI_V_Neg10To10 = 8,
    Adam6024_AI_mA_0To20    = 13,
    Adam6024_AI_Unknown     = 255
} Adam6024_AI_Range;

typedef enum
{
    Adam6024_AO_mA_0To20    = 0,
    Adam6024_AO_mA_4To20    = 1,
    Adam6024_AO_V_0To10     = 2,
    Adam6024_AO_Unknown     = 255
} Adam6024_AO_Range;
//---------------------------------------------------------------------------
static void ShowDoubleEPConnectGuide(AnsiString IP, int Num, AnsiString Detail) //AI(ht9045-v899) 20260505: guide field engineer without changing Gerneral.ini automatically
{
    if(Num!=2 || bDoubleEPConnectGuideShown)
        return;

    if(INSTALL_DOUBLE_EP!=DOUBLE_EP_INDIVIAL && INSTALL_DOUBLE_EP!=DOUBLE_EP_MULTI)
        return;

    bDoubleEPConnectGuideShown=true;
    AnsiString Msg;
    if(INSTALL_DOUBLE_EP==DOUBLE_EP_MULTI)
    {
        Msg.sprintf("Double EP board connect failed.\r\nMode=3 is Multi EP (Max Qual Site), not normal left/right independent EP.\r\nThis mode needs the Multi EP APAX board. Expected IP=%s.\r\nIf this machine only needs left/right independent EP, field engineer should set INSTALL_DOUBLE_EP=2.\r\nIf the board exists, check board IP, network segment, cable and power.\r\n%s", IP.c_str(), Detail.c_str());
    }
    else
    {
        Msg.sprintf("Double EP board connect failed.\r\nMode=2 is Individual EP. Expected IP=%s.\r\nIf the board exists, check board IP, network segment, cable and power.\r\nIf the machine has no second EP board, field engineer should select the proper Dual EP Control mode.\r\n%s", IP.c_str(), Detail.c_str());
    }
    NewRecordProcess("", Msg);
    ShowMyMessage(Msg);
}
//---------------------------------------------------------------------------
//AI(ht9045-v899) 20260526: Multi EP pressure output is valid only when the physical SwMultiEp valve is actually ON.
bool IsMultiEPPressureRouteActive()
{
    if(INSTALL_DOUBLE_EP!=DOUBLE_EP_MULTI || TestIF_File.bIndEPSLK!=true)
        return false;

    if(SW[SwMultiEp].Enable==false)
        return false;

    return (SW[SwMultiEp].Status()==true);
}
//---------------------------------------------------------------------------
//AI(ht9045-v899) 20260526: keep mode 2 independent EP behavior, but let mode 3 fall back to normal EP when SwMultiEp is OFF.
bool IsIndependentEPPressureRouteActive()
{
    if(TestIF_File.bIndEPSLK!=true)
        return false;

    if(INSTALL_DOUBLE_EP==DOUBLE_EP_INDIVIAL)
        return true;

    if(INSTALL_DOUBLE_EP==DOUBLE_EP_MULTI)
        return IsMultiEPPressureRouteActive();

    return false;
}
//---------------------------------------------------------------------------
__fastcall TfAdam6024::TfAdam6024(TComponent* Owner) : TForm(Owner)
{                                                                               //JerryYang 20170301 (wei) 修改ADAMErrorMessage陣列位置
    ADAMErrorMessage[ 0]="";
    ADAMErrorMessage[ 1]="ADAM5KTCP_StartupFailure (-1)";
    ADAMErrorMessage[ 2]="ADAM5KTCP_SocketFailure (-2)";
    ADAMErrorMessage[ 3]="ADAM5KTCP_UdpSocketFailure (-3)";
    ADAMErrorMessage[ 4]="ADAM5KTCP_SetTimeoutFailure (-4)";
    ADAMErrorMessage[ 5]="ADAM5KTCP_SendFailure (-5)";
    ADAMErrorMessage[ 6]="ADAM5KTCP_ReceiveFailure (-6)";
    ADAMErrorMessage[ 7]="ADAM5KTCP_ExceedMaxFailure (-7)";
    ADAMErrorMessage[ 8]="ADAM5KTCP_CreateWsaEventFailure (-8)";
    ADAMErrorMessage[ 9]="ADAM5KTCP_ReadStreamDataFailure (-9)";
    ADAMErrorMessage[10]="ADAM5KTCP_InvalidIP (-10)";
    ADAMErrorMessage[11]="ADAM5KTCP_ThisIPNotConnected (-11)";
    ADAMErrorMessage[12]="ADAM5KTCP_AlarmInfoEmpty (-12)";
    ADAMErrorMessage[13]="ADAM5KTCP_NotSupportModule (-13)";
    ADAMErrorMessage[14]="ADAM5KTCP_ExceedDONo (-14)";
    ADAMErrorMessage[15]="ADAM5KTCP_InvalidRange (-15)";
}
//---------------------------------------------------------------------------
bool SetAiInputRange(char *ipAddr, int i_iChannel, byte i_byRange)              //ben 20230818 : ADAM check range
{
    char szSend[128]={0},szRecv[128]={0};
    int iRet;

    sprintf(szSend, "$01A%02X%02X\r", i_iChannel,i_byRange);

    iRet=ADAMTCP_SendReceive6KUDPCmd(ipAddr, szSend, szRecv);
    if(iRet==0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
//---------------------------------------------------------------------------
bool GetAiInputRange(char *ipAddr, int i_iChannel, byte* i_byRange)             //ben 20230818 : ADAM check range
{
    if(CHECK_EP_SETTING==0)                                                     //Steven 20240701 : EP檢查功能加上開關
    {
        i_byRange=0;
        return true;
    }

    char szSend[128]={0},szRecv[128]={0};
    bool rtnValue=false;
    int iRet;

    sprintf(szSend, "$01B%02X\r", i_iChannel);

    iRet=ADAMTCP_SendReceive6KUDPCmd(ipAddr, szSend, szRecv);
    if(iRet==0)
    {
        string recvStr=szRecv;
        if(recvStr.size()>4)
        {
            string szValue=recvStr.substr(3, recvStr.size()-3);
            if(szValue.size()==2)
            {
                unsigned int tmp = 0;
                if(sscanf(szValue.c_str(), "%2x", &tmp)==1)
                {
                    *i_byRange=static_cast<byte>(tmp);
                    rtnValue=true;
                }
            }
        }
    }
    return rtnValue;
}
//---------------------------------------------------------------------------
bool SetAoOutputRange(char *ipAddr, int i_iChannel, byte i_byRange)             //ben 20230818 : ADAM check range
{
    char szSend[128]={0}, szRecv[128]={0};
    int iRet;

    sprintf(szSend, "$01C%02X%02X\r", i_iChannel, i_byRange);

    iRet=ADAMTCP_SendReceive6KUDPCmd(ipAddr, szSend, szRecv);
    if(iRet==0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
//---------------------------------------------------------------------------
bool GetAoOutputRange(char *ipAddr, int i_iChannel, byte* i_byRange)            //ben 20230818 : ADAM check range
{
    if(CHECK_EP_SETTING==0)                                                     //Steven 20240701 : EP檢查功能加上開關
    {
        i_byRange=0;
        return true;
    }

    char szSend[128]={0}, szRecv[128]={0};
    bool rtnValue=false;
    int iRet;

    sprintf(szSend, "$01C%02X\r", i_iChannel);

    iRet=ADAMTCP_SendReceive6KUDPCmd(ipAddr, szSend, szRecv);
    if(iRet==0)
    {
        string recvStr=szRecv;
        if(recvStr.size()>4)
        {
            string szValue=recvStr.substr(3, recvStr.size()-3);
            if(szValue.size()==2)
            {
                unsigned int tmp = 0;
                if(sscanf(szValue.c_str(), "%2x", &tmp)==1)
                {
                    *i_byRange=static_cast<byte>(tmp);
                    rtnValue=true;
                }
            }
        }
    }
    return rtnValue;
}
//---------------------------------------------------------------------------
bool Open_ADAM_6024(AnsiString IP, int Num)                                     //Ifor 20150709 :修改可使用多台ADAM6024
{
#ifndef SOFT_SIMULTE
    AnsiString Str;
    int iRet;
    static int iCount=90;

    int iChannel;                                                               //ben 20230818 : ADAM check range
    int adam6024AiChannelTotal=6;
    int adam6024AoChannelTotal=2;
    byte rangeCode;
    byte rangeReadBack;

    if(EP_Install>0)
    {
        if(!fCheckConnectStatus_ADAM6024(Num))                                  //Hmy 20170120 add check Adam6024 Connect Status ->
            return false;
        bADAM6024FWIsNew[Num]=fCheckModuleFWISNew_ADAM6024(Num);                //Nickliu 20230314 Add Check Adam FW Is New
        Address[Num]=IP;                                                        //Ifor 20150709 ：開啟ADAM 設備時將IP位置寫入暫存器
        if(EP_Install==4)                                                       //Steven 20141202 : PISO DA
        {
            fAdam6024->OpenSocket(IP, DEFAULT_ET7226_PORT);
        }
        else
        {
            if(Num==0)                                                          //Ifor 20160301 避免使用一顆以上ADAM出現異常
            {
                iRet=ADAMTCP_Open();
                if(iRet!=0)                                                     //Steven 20210505 : 針對Adam連線異常加上紀錄
                {
                    iRet=0-iRet;
                    MyDBIProcess("Motion", "ADAMTCP_Open Fail!", fAdam6024->ADAMErrorMessage[iRet]);
                    return false;
                }
            }

            if(Num!=2 &&                                                        //Steven 20231026 : 不檢查APAX
               bADAM6420CheckRange[Num]==false)                                 //ben 20230818 : ADAM check range
            {
                if(ADAMTCP_UDPOpen(iSendTimeout, iReceiveTimeout)==0)
                {
                    for(iChannel=0; iChannel<adam6024AiChannelTotal; iChannel++)
                    {
                        rangeReadBack=0;                                        //Get AI Input Range
                        if(GetAiInputRange(IP.c_str(), iChannel, &rangeReadBack)!=true)
                        {
                            Str.printf("Failed to Get ADAM AI Range! IP=%s, Channel=%d, Readrange=%d", IP, iChannel, rangeReadBack);
                            ShowMyMessage(Str);
                            return false;
                        }

                        if(iChannel==3 ||
                           (MachineTypeChoice==Type_HT1032 &&
                            Tri_Temp_Machine==1 &&
                            (iChannel==2 || iChannel==3 || iChannel==4)))       //Ztex For Type_HT1032
                        {
                            if(rangeReadBack!=Adam6024_AI_mA_4To20)
                            {
                                rangeCode=Adam6024_AI_mA_4To20;                 //Set AI Input Range
                                if(SetAiInputRange(IP.c_str(), iChannel, rangeCode)!=true)
                                {
                                    Str.printf("Failed to Set ADAM AI Range! IP=%s, Channel=%d, Setrange=%d", IP, iChannel, rangeCode);
                                    ShowMyMessage(Str);
                                    return false;
                                }
                            }
                        }
                        else
                        {
                            if(rangeReadBack!=Adam6024_AI_V_Neg10To10)
                            {
                                rangeCode=Adam6024_AI_V_Neg10To10;              //Set AI Input Range
                                if(SetAiInputRange(IP.c_str(), iChannel, rangeCode)!=true)
                                {
                                    Str.printf("Failed to Set ADAM AI Range! IP=%s, Channel=%d, Setrange=%d", IP, iChannel, rangeCode);
                                    ShowMyMessage(Str);
                                    return false;
                                }
                            }
                        }
                    }

                    for(iChannel=0; iChannel<adam6024AoChannelTotal; iChannel++)
                    {
                        rangeReadBack=0;                                        //Get AI Input Range
                        if(GetAoOutputRange(IP.c_str(), iChannel, &rangeReadBack)!=true)
                        {
                            Str.printf("Failed to Get ADAM AO Range! IP=%s, Channel=%d, Readrange=%d", IP, iChannel, rangeReadBack);
                            ShowMyMessage(Str);
                            return false;
                        }

                        if(rangeReadBack!=Adam6024_AI_mA_4To20)
                        {
                            rangeCode=Adam6024_AO_mA_4To20;                     //Set AO Output Range
                            if(SetAoOutputRange(IP.c_str(), iChannel, rangeCode)!=true)
                            {
                                Str.printf("Failed to Set ADAM AO Range! IP=%s, Channel=%d, Setrange=%d", IP, iChannel, rangeCode);
                                ShowMyMessage(Str);
                                return false;
                            }
                        }
                    }
                }
                ADAMTCP_UDPClose();
                bADAM6420CheckRange[Num]=true;
            }

            iRet=ADAMTCP_Connect(IP.c_str(), DEFAULT_PORT, iConnectionTimeout, iSendTimeout, iReceiveTimeout);

            if(iRet!=0)                                                         //Frank 20170206 (Steven) 確認ADAM連線正常
            {
                iCount++;
                iRet=0-iRet;                                                    //JerryYang 20170301 (wei) 修改ADAMErrorMessage陣列位置
                if(iCount>100)                                                  //jou 20170313 (Steven) : Adam EP check alarm 3 -> 100
                {
                    iCount=0;
                    ShowMyMessage("Connect Fail! Please Check ADAM IP!", IP, fAdam6024->ADAMErrorMessage[iRet]);          //wei 20160329 ADAM連線異常Alarm
                    return false;
                }
            }
            else
            {
                iCount=50;                                                      //Steven 20170314 (Jou) : 提早Alarm 0 --> 50
            }
        }

        for(int i=0; i<10; i++)
            wGain[i]=ADAMTCP_BI_10V;                                            // the gain code for channel:3

        if(MachineTypeChoice==Type_HT1032 && Tri_Temp_Machine==1)               //Ztex 2023.04.19 Add HT-1032 TriTemp Function
        {
            wGain[2]=ADAMTCP_UNI_4TO20mA;
            wGain[3]=ADAMTCP_UNI_4TO20mA;
            wGain[4]=ADAMTCP_UNI_4TO20mA;
        }
        bADAM6420Install=true;
        return true;
    }
    else
    {
        return false;
    }
#else
    return true;
#endif
}
//---------------------------------------------------------------------------
bool Open_ADAM_6024()                                                           //Jimmychiu 20230804 : 整合全部連線檢查
{
    bool bflag1=true, bflag2=true, bflag3=true;
    bflag1=Open_ADAM_6024("172.16.8.110", 0);
    if(USE_CKD_FCM_CleanAir)
        bflag2=Open_ADAM_6024("172.16.8.111", 1);
    if(INSTALL_DOUBLE_EP==DOUBLE_EP_INDIVIAL || INSTALL_DOUBLE_EP==DOUBLE_EP_MULTI)                            //AI(ht9045-v899) 20260610: also open the 2nd APAX board (Address[2]) for Multi EP mode 3.
        bflag3=Open_ADAM_6024("172.16.8.112", 2);
    return (bflag1 && bflag2 && bflag3);
}
//---------------------------------------------------------------------------
void Close_ADAM_6024()
{
#ifndef SOFT_SIMULTE
    if(EP_Install)
    {
        if(bADAM6420Install)
        {
            bADAM6420Install=false;
            if(EP_Install==4)                                                   //Steven 20141202 : PISO DA
            {
                fAdam6024->CloseSocket();
            }
            else
            {
                ADAMTCP_Disconnect();
                ADAMTCP_Close();
            }
        }
    }
#endif
}
//---------------------------------------------------------------------------
double ADAM_ReadVoltage(int Num, int iCH)                                       //Ifor 20150709 :修改可使用多台ADAM6024    //Ifor 20190104 : add 讀取ADAM CH 數值
{
    int iRet;
    static bool bEnter=false;
    if(bEnter)
    {
        return 0;
    }
    bEnter=true;

    if(EP_Install)
    {
        if(bADAM6420Install==false)
        {
            bEnter=false;
            return 0;
        }
#ifdef DEBUG_TRY_CATCH
        try
        {
#endif
            if(EP_Install==4)                                                   //Steven 20141202 : PISO DA
            {
                bEnter=false;
            }
            else
            {
                if(!bConnectStatus[Num])                                        //Hmy 20170120 add check Adam6024 Connect Status
                    bConnectStatus[Num] = fCheckConnectStatus_ADAM6024(Num);    //Nickliu 20230330 add check statsu

                iRet=ADAMTCP_Read6KAI(Address[Num].c_str(), 6017, 1, wGain, wHex, fValue);

                if(iRet!=0)                                                     //Ifor 20150709 ：透過ADAM陣列IP讀取資料   //Frank 20170206 (Steven) 確認ADAM連線正常
                {
                    Close_ADAM_6024();
                    Open_ADAM_6024();                                           //Jimmychiu 20230804 : 整合全部連線檢查
                    bEnter=false;
                    return 0;
                }
            }

            bEnter=false;
            if(iCH==3)                                                          //Isaac 20191024 : add for Dew Point
            {
                if(Tri_Temp_Machine==1)                                         //1032 機型露點計使用 2、3、4 CH
                {
                    return fValue[iCH];
                }
                else
                {
                    return (wHex[3]/4095.9375+4);
                }
            }
            else
            {
                return fValue[iCH];                                             //Ifor 20190104 : add 讀取ADAM CH 數值
            }
#ifdef DEBUG_TRY_CATCH
        }
        catch(...)
        {
            bEnter=false;
            MyDBIProcess("Exception", "ADAM_ReadVoltage");
            return 0;
        }
#endif
    }
    else
    {
        bEnter=false;
        return 0;
    }
}
//---------------------------------------------------------------------------
int ADAM_ReadPA(double *dValue, int iCH)                                        //Ifor 20190104 : add   //wei 20220309 Add EP Return Voltage
{
    double v=ADAM_ReadVoltage(0, iCH);                                          //Ifor 20190104 : add 讀取ADAM CH 數值

    *dValue=v;                                                                  //wei 20220309 Add EP Return Voltage
    if(EP_MAXAFB==0.0 || EP_MinAFB==0.0)
        return v;
    if(EP_MAXAFB==EP_MinAFB)                                                    //Steven 20260505 : add zero-guard for (EP_MAXAFB-EP_MinAFB)
        return v;

                                                                                //Steven 20170721 (wei) : 修正EP回傳的內差法公式  //JerryYang 20171023 (wei) 修正電壓轉PA的公式
    int PA=EP_MINMPA*1000.0+(EP_MAXKPA-EP_MINMPA*1000.0)*ChangeToFloatNonPcnt((double)(v-EP_MinAFB), (double)(EP_MAXAFB-EP_MinAFB)); //Steven 20260505 : add zero-guard
    if(iCH==2)                                                                  //kevin 20200325 add dual force data
    {
        if(EPDual_MAXAFB==EPDual_MinAFB)                                        //Steven 20260505 : add zero-guard for (EPDual_MAXAFB-EPDual_MinAFB)
            return v;
        PA=EPDual_MINMPA*1000.0+(EPDual_MAXKPA-EPDual_MINMPA*1000.0)*ChangeToFloatNonPcnt((double)(v-EPDual_MinAFB), (double)(EPDual_MAXAFB-EPDual_MinAFB)); //Steven 20260505 : add zero-guard
    }
    return PA;
}
//---------------------------------------------------------------------------
double AdamOutputToPA(int iAdamOutput)                                          //JerryYang 20171030 (wei) adam output轉成PA
{
    double fMaxUnit=0.0;
    if(EP_Install==1 || EP_Install==3 || EP_Install==5)
    {
        fMaxUnit=4095.0;
    }
    else if(EP_Install==2)
    {
        fMaxUnit=1022.0;
    }
    int iPA=((EP_MAXKPA-EP_MINMPA*1000.0)/(fMaxUnit+1))*iAdamOutput;
    return iPA;
}
//---------------------------------------------------------------------------
void ADAM_Rang(int v)
{
    iADAMRange=v;
}
//---------------------------------------------------------------------------
double iAdamOutValue=0.0;
bool ADAM_Alarm(int iArm)
{
    #ifdef SOFT_SIMULTE
        return false;
    #else
        AnsiString AlarmMsg;
        double dValue=0.0;
        if(!bConnectStatus[0])                                                  //Hmy 20170120 add check Adam6024 Connect Status
            bConnectStatus[0] = fCheckConnectStatus_ADAM6024(0);                //Nickliu 20230330 add check statsu

        int PA;                                                                 //wei 20220309 Add EP Return Voltage

        if(EP_Install==5)
            PA=ADAM_ReadPA(&dValue, iArm);
        else
            PA=ADAM_ReadPA(&dValue);

        iReadAdamEP=PA;                                                         //jou 20170413 (Steven) : Read Adam EP 提升UPH
        iAdamOutValue=AdamOutputToPA(iWritePA);
        if(CUSTOMER_CODE==CC_JCET ||                                            //Richard 20230426 : JECT ADD 固定值改為百分比 //Ifor add KYEC_LEE
           CUSTOMER_CODE==CC_KYEC_LEE)                                          //Eastsun 20260511 F008 整合
        {
            if(IniConfig.iD26_3FixValueOrPercentage==1)                         //Richard 20230428 : EP固定值或百分比
            {
                if(PA>iAdamOutValue*(1+iADAMRange/100.0) ||
                   PA<iAdamOutValue*(1-iADAMRange/100.0))
                {
                    AlarmMsg.sprintf("AdamOutValue=%f, ReadAdamValue=%d, Range=%d", iAdamOutValue, PA, iADAMRange);    //Steven 20240520 : EP alarm log
                    MNetLog(AlarmMsg);
                    return true;
                }
            }
            else if(IniConfig.iD26_3FixValueOrPercentage==0)
            {
                if(PA>iAdamOutValue+iADAMRange ||
                   PA<iAdamOutValue-iADAMRange)
                {
                    AlarmMsg.sprintf("AdamOutValue=%f, ReadAdamValue=%d, Range=%d", iAdamOutValue, PA, iADAMRange);    //Steven 20240520 : EP alarm log
                    MNetLog(AlarmMsg);
                    return true;
                }
            }
        }
        else
        {
            if(PA>iAdamOutValue+iADAMRange ||
               PA<iAdamOutValue-iADAMRange)                                     //JerryYang 20171030 (wei) fix ADAM alarm
            {
                AlarmMsg.sprintf("AdamOutValue=%f, ReadAdamValue=%d, Range=%d", iAdamOutValue, PA, iADAMRange);    //Steven 20240520 : EP alarm log
                MNetLog(AlarmMsg);
                return true;
            }
        }
        AlarmMsg="";
        return false;
    #endif
}
//---------------------------------------------------------------------------
bool ADAM_Alarm_Kg(int iAdd)                                                    //JerryYang 20171024 (wei) add 單顆浮動頭誤差範圍,依照Mars定義給海思的資料
{
    double dValue=0.0;
    int PA =0;                                                                  //wei 20220309 Add EP Return Voltage
    double dKg=0.0;
    double dSetKg=0.0;
    AnsiString AlarmMsg_KG;

    if(iAdd==0)
    {
        PA = ADAM_ReadPA(&dValue);                                              //wei 20220309 Add EP Return Voltage
        dKg=KpaTransferKG(PA);
        dSetKg=fContact->edSetKg->Text.ToDouble();
    iReadAdamEP=PA;                                                             //jou 20170413 (Steven) : Read Adam EP 提升UPH
    }
    else if(iAdd==1)
    {
        PA = ADAM_ReadPA(&dValue,2);                                            //Ifor 20221215 add:讀取Dual EP
        dKg=KpaTransferKG(PA, true);                                            //Ifor 20221215 add:轉換Dual EP 公斤數
        dSetKg=fContact->edDoubleForce->Text.ToDouble();
    }

    if(Temperature.bATCActiveCooling)                                           //ATC layout kit
    {
        if(dSetKg>60)                                                           //單顆浮動頭61~120Kg
        {
            dADAMRange_Kg=2.0;
        }
        else                                                                    //單顆浮動頭8~60Kg
        {
            dADAMRange_Kg=1.0;
        }
    }
    else
    {
        if(dSetKg>60)                                                           //單顆浮動頭61~120Kg
        {
            dADAMRange_Kg=2.0;
        }
        else if(dSetKg>10)                                                      //單顆浮動頭11~60Kg
        {
            dADAMRange_Kg=1.0;
        }
        else if(dSetKg>5)                                                       //單顆浮動頭6~10Kg
        {
            dADAMRange_Kg=0.5;
        }
        else                                                                    //單顆浮動頭1~5Kg
        {
            dADAMRange_Kg=0.25;
        }
    }

    if((dKg>dSetKg+dADAMRange_Kg) ||
       (dKg<dSetKg-dADAMRange_Kg))
    {
        AlarmMsg_KG.sprintf("AdamOutValue=%f, ReadAdamValue=%f, Range=%f", dSetKg, dKg, dADAMRange_Kg);  //Steven 20240520 : EP alarm log
        MNetLog(AlarmMsg_KG);
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
bool ADAM_DualAlarm(int iType)                                                  //Ifor 20221228 add:Dual EP Check
{
    double dValue=0.0;
    double dDualTempKg=0.0, dOutValue;
    int PA=0, iDualTempPA=0;
    AnsiString AlarmMsg_Dual="";

    iDualTempPA=ADAM_ReadPA(&dValue,2);                                         //Ifor 20221215 add:讀取Dual EP
    dDualTempKg=KpaTransferKG(iDualTempPA, true);                               //Ifor 20221215 add:轉換Dual EP 公斤數

    if(iType==0)
    {
        PA=iDualTempPA;
        dOutValue=AdamOutputToPA(fContact->edDoubleForce->Text.ToDouble());
    }
    else if(iType==1)
    {
        PA=dDualTempKg;
        dOutValue=fContact->edDoubleForce->Text.ToDouble();
    }

    if(IniConfig.iD26_3FixValueOrPercentage==1)                                 //Richard 20230428 : EP固定值或百分比
    {
        if(PA>dOutValue*(1+IniConfig.iD26_3DualEPEncoderRange/100.0) ||
           PA<dOutValue*(1-IniConfig.iD26_3DualEPEncoderRange/100.0))
        {
            AlarmMsg_Dual.sprintf("AdamOutValue=%f, ReadAdamValue=%f, Range=%d%%", dOutValue, PA, IniConfig.iD26_3DualEPEncoderRange);//Steven 20240520 : EP alarm log
            MNetLog(AlarmMsg_Dual);
            return true;
        }
    }
    else
    {
        if(PA>dOutValue+IniConfig.iD26_3DualEPEncoderRange ||
           PA<dOutValue-IniConfig.iD26_3DualEPEncoderRange)
        {
            AlarmMsg_Dual.sprintf("AdamOutValue=%f, ReadAdamValue=%f, Range=%d", dOutValue, PA, IniConfig.iD26_3DualEPEncoderRange);//Steven 20240520 : EP alarm log
            MNetLog(AlarmMsg_Dual);
            return true;
        }
    }
    return false;
}
//---------------------------------------------------------------------------
double KpaTransferKG(int fInputMPA, bool bDualForce)                            //Ifor 20150826 :新增 Kpa 轉 公斤 Function  //JerryYang 20171023 (wei) int -> double
{
    double  fTestvalue,
            fTeskKG,
            fLoadRate,
            fDiameter;

    double  dIndex60mmLoadRate ,
            dIndex40mmLoadRate ,
            dIndex30mmLoadRate ,
            dIndex56mmLoadRate ;                                                //wei 20151005 add 56mm

    bool bNSKit=false, bNSKitSwitch=false;
    int iTag=-1;
    if(CosFunction.bEPUseNSSLK==true)                                           //kevin 20170804 (Steven) EP表頭另一種TYPE
    {
        bNSKit=true;                                                            //使用NS KIT
        if(CUSTOMER_CODE==CC_ASE_KaohSiung)
        {
            if(TestIF_File.bNSKitPress)
                bNSKitSwitch=true;                                              //使用NS KIT
        }
        else
        {
            bNSKitSwitch=true;
        }
    }

    if(CosFunction.bUseLoadCellOffsetByHeater &&
       LastSet.iTemperature==Tempture_Hot)                                      //2014-06-26    Dell    for TSMC 高溫Load cell offset
    {
        dIndex60mmLoadRate=LastSet.dIndexLoadRate[0][0]+LastSet.dIndexLoadRate[2][0];
        dIndex56mmLoadRate=LastSet.dIndexLoadRate[0][1]+LastSet.dIndexLoadRate[2][1];   //wei 20151005 add 56mm
        dIndex40mmLoadRate=LastSet.dIndexLoadRate[0][2]+LastSet.dIndexLoadRate[2][2];
        dIndex30mmLoadRate=LastSet.dIndexLoadRate[0][3]+LastSet.dIndexLoadRate[2][3];
    }
    else if(bNSKit && bNSKitSwitch &&                                                                               //kevin 20170804 (Steven) 使用另一種EP 壓力表
            (TestIF_File.bNSKitPress ||
             TestIF_File.bNS7000kit  ||
             TestIF_File.bNS7000CS   ||
             TestIF_File.bNS8000CS))                                            //wei 20150303   京元NS浮動頭
    {
        dIndex60mmLoadRate = LastSet.dIndexLoadRate[1][0];
        dIndex56mmLoadRate = LastSet.dIndexLoadRate[1][1];                      //wei 20151005 add 56mm
        dIndex40mmLoadRate = LastSet.dIndexLoadRate[1][2];
        dIndex30mmLoadRate = LastSet.dIndexLoadRate[1][3];
    }
    else
    {
        dIndex60mmLoadRate = LastSet.dIndexLoadRate[0][0];
        dIndex56mmLoadRate = LastSet.dIndexLoadRate[0][1];                      //wei 20151005 add 56mm
        dIndex40mmLoadRate = LastSet.dIndexLoadRate[0][2];
        dIndex30mmLoadRate = LastSet.dIndexLoadRate[0][3];
    }

    fDiameter=DeviceForm_File.dKitDiameter;

    if(bDualForce==true && INSTALL_DOUBLE_EP==DOUBLE_EP_INDIVIAL)                                //Ifor 20191003 : add Die Force 可以自定義Kit直徑
    {
        if(fDiameter==6.0)
        {
            fLoadRate=dIndex60mmLoadRate;                                       //wei 20150303     IniConfig.dIndex40mmLoadRate-->dIndex60mmLoadRate
        }
        else if(fDiameter==4.0)
        {
            fLoadRate=dIndex40mmLoadRate;                                       //wei 20150303     IniConfig.dIndex40mmLoadRate-->dIndex40mmLoadRate
        }
        else if(fDiameter==5.6)                                                 //wei 20151005 add 56mm
        {
            fLoadRate=dIndex56mmLoadRate;
        }
        else
        {
            if(fDiameter==0)                                                    //Steven 20140627 : 避免分母為0
                fDiameter=3.0;
            fLoadRate=dIndex30mmLoadRate;
        }
    }
    else
    {
        if(CosFunction.bUseDynamicKitDiameter==false)                           //Steven 20170605 (wei) : 可以自定義Kit直徑
        {                                                                       //Wei 20220217 : Add for EP回授數值計算
            if(fDiameter==6.0)
            {
                fLoadRate=dIndex60mmLoadRate;                                   //wei 20150303     IniConfig.dIndex40mmLoadRate-->dIndex60mmLoadRate
            }
            else if(fDiameter==4.0)
            {
                fLoadRate=dIndex40mmLoadRate;                                   //wei 20150303     IniConfig.dIndex40mmLoadRate-->dIndex40mmLoadRate
            }
            else if(fDiameter==5.6)                                             //wei 20151005 add 56mm
            {
                fLoadRate=dIndex56mmLoadRate;
            }
            else
            {
                if(fDiameter==0)                                                //Steven 20140627 : 避免分母為0
                    fDiameter=3.0;
                fLoadRate=dIndex30mmLoadRate;
            }
        }
        else
        {
            if(bDualForce==true)                                                //Ifor 20191003 : add Die Force 可以自定義Kit直徑
            {
                if(INSTALL_DOUBLE_EP==0)                                        //JerryYang 20210119 : 增加dual EP防呆
                {
                   ShowMyMessage("無安裝dual force, 請確認硬體選項");
                }
                fDiameter=DeviceForm_File.dDieForceKitDiameter;
                for(unsigned int i=0; i<fContactForce->DieForceSLKClass.size(); i++)
                {
                    double d=fContactForce->DieForceSLKClass[i]->dDiameter/10.0;
                    if(d==fDiameter)
                    {
                        iTag=i;
                    }
                }

                if(iTag==-1 || iTag>(int)fContactForce->DieForceSLKClass.size())
                    iTag=0;

                fLoadRate=fContactForce->DieForceSLKClass[iTag]->dLoadRate;
            }
            else
            {
                for(unsigned int i=0; i<fContactForce->SLKClass.size(); i++)
                {
                    double d=fContactForce->SLKClass[i]->dDiameter/10.0;

                    if(CUSTOMER_CODE==CC_KYEC_LEE)                              //Ifor 20200407 : Fix KYEC 特殊缸徑造成資料異常
                    {
                        if(d==2.8)
                        {
                            d=3.0;
                        }
                        else if(d==5.8)
                        {
                            d=6.0;
                        }
                    }

                    if(d==fDiameter)
                    {
                        iTag=i;
                    }
                }

                if(iTag==-1 || iTag>(int)fContactForce->SLKClass.size())
                    iTag=0;

                bool bUseNSKit=false;
                if(CUSTOMER_CODE==CC_KYEC_LEE)
                {
                    switch(TestIF.iTestMode)
                    {
                        case SingleSite:
                        case DualSite:
                            if(CosFunction.bCanUseBias==true && TestIF_File.bNS7000kit==true)
                            {
                                bUseNSKit=true;
                            }
                            else
                            {
                                bUseNSKit=false;
                            }
                            break;
                        case QualSite1X4:
                            if((CosFunction.bCanUseBias==true && TestIF_File.bNS7000kit==true) || TestIF_File.bNS7000CS==true)
                            {
                                bUseNSKit=true;
                            }
                            else
                            {
                                bUseNSKit=false;
                            }
                            break;
                        case QualSite2X2:
                            if((CosFunction.bCanUse2x2Bias==true && TestIF_File.bNS7000kit==true) || TestIF_File.bNS7000CS==true)
                            {
                                bUseNSKit=true;
                            }
                            else
                            {
                                bUseNSKit=false;
                            }
                            break;
                        case DualSite2x1:
                            if(TestIF_File.bNS7000CS==true)
                            {
                                bUseNSKit=true;
                            }
                            else
                            {
                                bUseNSKit=false;
                            }
                            break;
                        case TriSite1X3:
                        case _8Site1X4:
                            if(ATC_SYSTEM>eATC30 && TestIF_File.bNS7000CS==true)
                            {
                                bUseNSKit=true;
                            }
                            else
                            {
                                bUseNSKit=false;
                            }
                            break;
                        case _8Site2X4:
                            if((ATC_SYSTEM>eATC30 && TestIF_File.bNS7000CS==true) ||
                               (USE_16_HEATER!=eht4Heater && TestIF_File.bNS8000CS==true))
                            {
                                bUseNSKit=true;
                            }
                            else
                            {
                                bUseNSKit=false;
                            }
                            break;
                        default :
                            bUseNSKit=false;
                            break;
                    }
                }
                else
                {
                    bUseNSKit=false;
                }

                if(bNSKit && bNSKitSwitch &&                                    //kevin 20170804 (Steven) 使用另一種EP 壓力表
                   (TestIF_File.bNSKitPress ||
                    TestIF_File.bNS7000kit ||
                    TestIF_File.bNS7000CS ||
                    TestIF_File.bNS8000CS))                                     //wei 20150303   京元NS浮動頭
                {
                    fLoadRate=fContactForce->SLKClass[iTag]->dLoadRate_NS;
                }
                else if(bUseNSKit==true)
                {
                    fLoadRate=fContactForce->SLKClass[iTag]->dLoadRate_NS;
                }
                else
                {
                    fLoadRate=fContactForce->SLKClass[iTag]->dLoadRate;
                }

                if(CosFunction.bUseLoadCellOffsetByHeater &&
                   LastSet.iTemperature==Tempture_Hot)                          //2014-06-26    Dell    for TSMC 高溫Load cell offset
                {
                    fLoadRate+=fContactForce->SLKClass[iTag]->dHotOffset;
                }
            }
        }
    }
    fTestvalue  =fInputMPA*10.197;
    fTeskKG     =fTestvalue*(fDiameter*fDiameter*M_PI/4.0*fLoadRate);
    fTeskKG     =(fTeskKG/1000.0);
    return  fTeskKG;
}
//---------------------------------------------------------------------------
//AI(ht9045-v899) 20260504: port from V896 (Ifor 20250416). Multi EP per-site Kpa->Kg.
//   Simplified KpaTransferKG: only handles dKitDiameter / dDieForceKitDiameter,
//   includes KYEC special diameter (28->30, 58->60, 40.2->4.0) per V896 logic.
//double MultiTransferKG(int fInputMPA, bool bDualForce)  //Eastsun 20260710 Merge
double MultiTransferKG(double fInputMPA, bool bDualForce)  //Eastsun 20260710 Merge //Eastsun 修正沒有小數點
{
    double fTestvalue, fTeskKG, fLoadRate, fDiameter;
    int iTag=-1;

    if(bDualForce==true)
    {
        if(INSTALL_DOUBLE_EP==DOUBLE_EP_NONE)
        {
            ShowMyMessage("No dual force installed, please check hardware option");
        }
        fDiameter=DeviceForm_File.dDieForceKitDiameter;
        for(unsigned int i=0; i<fContactForce->DieForceSLKClass.size(); i++)
        {
            double d=fContactForce->DieForceSLKClass[i]->dDiameter/10.0;
            if(d==fDiameter)
            {
                iTag=i;
            }
        }

        if(iTag==-1 || iTag>(int)fContactForce->DieForceSLKClass.size())
            iTag=0;

        fLoadRate=fContactForce->DieForceSLKClass[iTag]->dLoadRate;
    }
    else
    {
        fDiameter=DeviceForm_File.dKitDiameter;
        for(unsigned int i=0; i<fContactForce->SLKClass.size(); i++)
        {
            double d=fContactForce->SLKClass[i]->dDiameter/10.0;

            if(CUSTOMER_CODE==CC_KYEC_LEE)
            {
                if(d==2.8)
                {
                    d=3.0;
                }
                else if(d==5.8)
                {
                    d=6.0;
                }
            }

            if(d==fDiameter)
            {
                iTag=i;
                if(fDiameter==40.2)
                    fDiameter=4.0;
            }
        }

        if(iTag==-1 || iTag>(int)fContactForce->SLKClass.size())
            iTag=0;

        fLoadRate=fContactForce->SLKClass[iTag]->dLoadRate;
    }

    fTestvalue = fInputMPA*10.197;
    fTeskKG = fTestvalue*(fDiameter*fDiameter*M_PI/4.0*fLoadRate);
    fTeskKG = (fTeskKG/1000.0);
    return fTeskKG;
}
//---------------------------------------------------------------------------
int TransformFuntion(double fInputKG, bool bDualForce, bool bSoft, int iArm)
{
    double  fMinMPA=0.0,                                                        //最小輸出氣壓
            fMaxMPA=0.0,                                                        //最大輸出氣壓
            fMinUnit=0.0,                                                       //最小輸出單位
            fMaxUnit=0.0,                                                       //最大輸出單位
            f64KgUnit=0.0,                                                      //64KG時的單位數        //Steven 20111214 : 避免誤用
            f16KgUnit=0.0,                                                      //16KG時的單位數        //Steven 20111214 : 避免誤用
            fLoadRate=0.0;                                                      //負荷率 (HT9046<8Site)=0.9;  (HT9046>=8Site)=0.85 (HT9xxx || HT7xxx)=0.95

    double fLoadRateInd, dTempInd, fInputFInd,fInputMPAInd, fInputKGInd, fInputKGIndRel;
    int iResultTemp;

    double  fInputMPA,                                                          //單顆浮動頭的輸出氣壓
            fInputF,                                                            //單顆浮動頭的輸出壓力
            fDiameter;                                                          //浮動頭的直徑 (NS的是3mm, HT的是4mm)
    double  dTemp;

    int iResult;                                                                //最後輸出電流值
    int iTag=-1;

    double dIndex60mmLoadRate, dIndex40mmLoadRate, dIndex30mmLoadRate, dIndex56mmLoadRate;  //2014-06-26    Dell    for TSMC 高溫Load cell offset     //wei 20151005 add 56mm
    bool bNSKit=false, bNSKitSwitch=false;
    if(CosFunction.bEPUseNSSLK==true)                                           //kevin 20170804 (Stven) EP表頭另一種TYPE
    {
        bNSKit=true;                                                            //使用NS KIT
        if(CUSTOMER_CODE==CC_ASE_KaohSiung)
        {
            if(TestIF_File.bNSKitPress)
                bNSKitSwitch=true;                                              //使用NS KIT
        }
        else
        {
            bNSKitSwitch=true;
        }
    }

    if(CosFunction.bUseLoadCellOffsetByHeater &&
       LastSet.iTemperature==Tempture_Hot)                                      //2014-06-26    Dell    for TSMC 高溫Load cell offset
    {
        dIndex60mmLoadRate = LastSet.dIndexLoadRate[0][0] + LastSet.dIndexLoadRate[2][0];
        dIndex56mmLoadRate = LastSet.dIndexLoadRate[0][1] + LastSet.dIndexLoadRate[2][1];   //wei 20151005 add 56mm
        dIndex40mmLoadRate = LastSet.dIndexLoadRate[0][2] + LastSet.dIndexLoadRate[2][2];
        dIndex30mmLoadRate = LastSet.dIndexLoadRate[0][3] + LastSet.dIndexLoadRate[2][3];
    }
    else if(bNSKit && bNSKitSwitch &&                                                                                //kevin 20170804 (Steven) 使用另一種EP 壓力表
            (TestIF_File.bNSKitPress ||
             TestIF_File.bNS7000kit ||
             TestIF_File.bNS7000CS ||
             TestIF_File.bNS8000CS))                                            //wei 20150303   京元NS浮動頭
    {
        dIndex60mmLoadRate = LastSet.dIndexLoadRate[1][0];
        dIndex56mmLoadRate = LastSet.dIndexLoadRate[1][1];                      //wei 20151005 add 56mm
        dIndex40mmLoadRate = LastSet.dIndexLoadRate[1][2];
        dIndex30mmLoadRate = LastSet.dIndexLoadRate[1][3];
    }
    else if(EP_Install==5)
    {
        if(iArm==0)
        {
            dIndex60mmLoadRate = LastSet.dIndexLoadRate[0][0];
            dIndex56mmLoadRate = LastSet.dIndexLoadRate[0][1];                  //wei 20151005 add 56mm
            dIndex40mmLoadRate = LastSet.dIndexLoadRate[0][2];
            dIndex30mmLoadRate = LastSet.dIndexLoadRate[0][3];
        }
        else
        {
            dIndex60mmLoadRate = LastSet.dIndexLoadRate[1][0];
            dIndex56mmLoadRate = LastSet.dIndexLoadRate[1][1];                  //wei 20151005 add 56mm
            dIndex40mmLoadRate = LastSet.dIndexLoadRate[1][2];
            dIndex30mmLoadRate = LastSet.dIndexLoadRate[1][3];
        }
    }
    else
    {
        dIndex60mmLoadRate = LastSet.dIndexLoadRate[0][0];
        dIndex56mmLoadRate = LastSet.dIndexLoadRate[0][1];                      //wei 20151005 add 56mm
        dIndex40mmLoadRate = LastSet.dIndexLoadRate[0][2];
        dIndex30mmLoadRate = LastSet.dIndexLoadRate[0][3];
    }

    fMaxMPA=EP_MAXKPA/1000.00;                                                  //ChungHung 20140513  統一由外面讀取
    fMinMPA=EP_MINMPA;                                                          //Steven 20190304 : 提升小公斤數的精準度

    if(EP_Install==1 || EP_Install==3 || EP_Install==5)                         //20111111 Dell //20111217 ChungHung
    {
        fMaxUnit=4095.0;
    }
    else if(EP_Install==2)
    {
        fMaxUnit=1022.0;
    }
    fMinUnit=0.0;
    fLoadRate=0.9;

    if(WEIGHT_CALIBRATION)                                                      //Steven 20111108
    {
        if(bDualForce==true)
        {
            fDiameter=(DeviceForm_File.dDieForceKitDiameter);                   //Ifor 20191003 : add Die Force 可以自定義Kit直徑
        }
        else
        {
            fDiameter=DeviceForm_File.dKitDiameter;                             //Steven 20240807 : DeviceForm --> DeviceForm_File
        }

        if(fDiameter==6.0)                                                      //Steven 20110722 : 改用fDiameter當作判斷值,並拉到外面來
        {
            f64KgUnit=IniConfig.iContactForceMap[2][1];
            f16KgUnit=IniConfig.iContactForceMap[2][0];
            fLoadRate=dIndex60mmLoadRate;
        }
        else if(fDiameter==4.0)
        {
            f64KgUnit=IniConfig.iContactForceMap[1][1];
            f16KgUnit=IniConfig.iContactForceMap[1][0];
            fLoadRate=dIndex40mmLoadRate;
        }
        else if(fDiameter==5.6)                                                 //wei 20151005 add 56mm
        {
            f64KgUnit=IniConfig.iContactForceMap[3][1];
            f16KgUnit=IniConfig.iContactForceMap[3][0];
            fLoadRate=dIndex56mmLoadRate;
        }
        else
        {
            f64KgUnit=IniConfig.iContactForceMap[0][1];
            f16KgUnit=IniConfig.iContactForceMap[0][0];
            fLoadRate=dIndex30mmLoadRate;
        }

        //    fMaxMPA=64;
        //    fMinMPA=16;
        //    fMaxMPA-fMinMPA=48 直接代入
        iResult=((f64KgUnit-f16KgUnit)/48)*(fInputKG-16)+f16KgUnit;             //Steven 20110312
    }
    else
    {
        //jou 2011-12-14 不能鎖死，因為沒選EP值會錯亂！
//        if(INDEX_PRESS_TYPE==e240KG) //Steven 20110310 : 240KG
//        {
            double fComplianceUnit=1.0;
            if(bDualForce==true)
            {
                fDiameter=(DeviceForm_File.dDieForceKitDiameter);               //Ifor 20191003 : add Die Force 可以自定義Kit直徑
            }
            else
            {
                fDiameter=DeviceForm_File.dKitDiameter;
            }

            switch(DeviceForm_File.iHeadDeviceCT)
            {
                case 2:                                                         // 1 Device with 1 Compliance Unit
                    fComplianceUnit=1.0;
                    dfComplianceUnit=1.0;                                       //kevin 20200313 add 浮動頭對應缸徑
                    break;
                case 3:                                                         // 2 Device with 1 Compliance Unit
                    fComplianceUnit=0.5;
                    dfComplianceUnit=0.5;                                       //kevin 20200313 add 浮動頭對應缸徑
                    break;
                case 4:                                                         // 4 Device with 1 Compliance Unit
                    fComplianceUnit=0.25;
                    dfComplianceUnit=0.25;                                      //kevin 20200313 add 浮動頭對應缸徑
                    break;
                case 5:                                                         // 2 Device with 4 Compliance Unit
                    fComplianceUnit=2.0;
                    dfComplianceUnit=2.0;                                       //kevin 20200313 add 浮動頭對應缸徑
                    break;
                case 6:                                                         // 8 Device with 1 Compliance Unit
                    fComplianceUnit=0.125;
                    dfComplianceUnit = 0.125;                                   //kevin 20200313 add 浮動頭對應缸徑
                    break;
            }

            if(bDualForce==false)
            {
                switch(TestIF.iTestMode)
                {
                    case SingleSite:                                            //JerryYang 20171214 (Steven) fix single site contact force問題
                        if(DeviceForm_File.bUseAddWeight)                       //kevin 20170802 (Steven) add 加重
                        {
                            fInputKG+=1.25;
                        }
                        fInputKG=fInputKG/(1.0*fComplianceUnit);
                        break;
                    case DualSite:                                              //1x2
                    case QualSite2X2N:                                          //Wei 20220216 : fixed for 2x2 nn mode
                        if(DeviceForm_File.bUseAddWeight)                       //kevin 20170802 (Steven) add 加重
                        {
                            if(TestIF.iTestMode==DualSite)
                                fInputKG+=1.25*2;
                            else
                                fInputKG+=1.25;
                        }                                                       //ChungHung 20130910 alter for SCK can close site by Index

                        if(((LastSet.bUseTestSocket[0][0][1]==false && LastSet.bUseTestSocket[1][0][1]==false) ||
                            (LastSet.bUseTestSocket[0][0][0]==false && LastSet.bUseTestSocket[1][0][0]==false)) && //Steven 20110915 : 1x2關Site,單Dut要可以壓到85KG
                            IniConfig.bD27UseSingleSite85kg)                    //2012-01-03    Dell 在1X2模式下,關Site能達85kg
                            fInputKG=fInputKG/(1.0*fComplianceUnit);
                        else
                            fInputKG=fInputKG/(2.0*fComplianceUnit);
                        break;
                    case DualSite2x1:                                           //ChungHung 20130910 alter for SCK can close site by Index
                        if(DeviceForm_File.bUseAddWeight)                       //kevin 20170802 (Steven) add 加重
                        {
                            fInputKG+=1.25*2;
                        }

                        if(((LastSet.bUseTestSocket[0][1][0]==false && LastSet.bUseTestSocket[1][1][0]==false) ||
                            (LastSet.bUseTestSocket[0][0][0]==false && LastSet.bUseTestSocket[1][0][0]==false))  //Steven 20110915 : 1x2關Site,單Dut要可以壓到85KG
                            && IniConfig.bD27UseSingleSite85kg)                 //2012-01-03    Dell 在1X2模式下,關Site能達85kg
                            fInputKG=fInputKG/(1.0*fComplianceUnit);
                        else
                            fInputKG=fInputKG/(2.0*fComplianceUnit);
                        break;
                    case TriSite1X3:                                            //wei 20171102 (jou) adam 1x3模式
                    case _6Site2X3N:                                            //Steven 20220425 : 2X3NN Mode
                        if(DeviceForm_File.bUseAddWeight)                       //kevin 20170802 (Steven) add 加重
                        {
                            fInputKG+=1.25*3;
                        }
                        fInputKG=fInputKG/(3.0*fComplianceUnit);
                        break;
                    case QualSite1X4:                                           //1x4
                    case QualSite2X2:                                           //2x2
                    case _8Site2X4N:                                            //Wei 20231211 : 2X4NN Mode
                        if(DeviceForm_File.bUseAddWeight)                       //kevin 20170802 (Steven) add 加重
                        {
                            fInputKG+=1.25*4;
                        }
                        fInputKG=fInputKG/(4.0*fComplianceUnit);
                        break;
                    case _8Site1X4:                                             //ChungHung 20150528 add for 海思 _8Site1x4
                        fInputKG=fInputKG/(4.0*fComplianceUnit);
                        break;
                    case _6Site2X3:                                             //ChungHung 20140115 add for 2x3_6
                        if(DeviceForm_File.bUseAddWeight)                       //kevin 20170802 (Steven) add 加重
                        {
                            fInputKG+=1.25*6;
                        }
                        fInputKG=fInputKG/(6.0*fComplianceUnit);
                        break;
                    case _16Site4X4:                                            //Sam 20190226 : 16Site4X4
                    case _8Site2X4:                                             //2x4
                        if(TestIF_File.bOctal_12Kit)                            //ChungHung 20140508 add for SCK
                        {
                            if(DeviceForm_File.bUseAddWeight)                   //kevin 20170802 (Steven) add 加重
                            {
                                fInputKG+=1.25*12;
                            }
                            fInputKG=fInputKG/(12.0*fComplianceUnit);
                        }
                        else
                        {
                            if(DeviceForm_File.bUseAddWeight)                   //kevin 20170802 (Steven) add 加重
                            {
                                fInputKG+=1.25*8;
                            }
                            fInputKG=fInputKG/(8.0*fComplianceUnit);
                        }
                        break;
                    case _10Site2X5:                                            //wei 20190614 10 site
                        fInputKG=fInputKG/(10.0*fComplianceUnit);
                        break;
                    case _12Site2X6:
                        if(DeviceForm_File.bUseAddWeight)                       //kevin 20170802 (Steven) add 加重
                        {
                            fInputKG+=1.25*12;
                        }
                        fInputKG=fInputKG/(12.0*fComplianceUnit);
                        break;
                    case _16Site2X8: //2x8
                        if(DeviceForm_File.bUseAddWeight)                       //kevin 20170802 (Steven) add 加重
                        {
                            fInputKG+=1.25*16;
                        }
                        fInputKG=ChangeToFloatNonPcnt((double)(fInputKG), (double)(fContact->dDutCount*fComplianceUnit)); //JerryYang 20221216 : fix 16 site只開中間8site的時候氣量只剩下一半 //Steven 20260505 : add zero-guard for dDutCount
                        break;
                    case _32Site4X8N:                                           //Steven 20140619 : for 32Site
                    case _32Site4X8M:
                        if(DeviceForm_File.bUseAddWeight)                       //kevin 20170802 (Steven) add 加重
                        {
                            fInputKG+=1.25*16;                                  //KenHsieh 20230313 : NN Mode Dutcount 32 -> 16
                        }
                        fInputKG=fInputKG/(16.0*fComplianceUnit);               //KenHsieh 20230313 : NN Mode Dutcount 32 -> 16
                        break;
                }
            }
//        }
//        else
//        {
//            switch(TestIF.iTestMode)
//            {
//                case 0: //1x2
//                case 1: //1x1 BusyShuttle
//                    if(LastSet.bUseTestSocket[0][1]==false || LastSet.bUseTestSocket[0][0]==false)      //Steven 20110915 : 1x2關Site,單Dut要可以壓到85KG
//                        fInputKG=fInputKG/1.0;
//                    else
//                        fInputKG=fInputKG/2.0;
//                    fDiameter=4.0;
//                    break;
//                case 2: //1x4
//                case 3: //2x2
//                case 4: //2x1 BusyShuttle
//                    fInputKG=fInputKG/4.0;
//                    fDiameter=3.0;
//                    break;
//                case 5: //2x4
//                case 6: //2x8
//                    fInputKG=fInputKG/8.0;
//                    fDiameter=3.0;
//                    break;
//            }
//        }

        if(CosFunction.bUseDynamicKitDiameter==false)                           //Steven 20170605 (wei) : 可以自定義Kit直徑
        {
            if(fDiameter==6.0)                                                  //Steven 20110722 : 改用fDiameter當作判斷值,並拉到外面來
            {
                fLoadRate=dIndex60mmLoadRate;                                   //wei 20150303     IniConfig.dIndex40mmLoadRate-->dIndex60mmLoadRate
            }
            else if(fDiameter==4.0)
            {
                fLoadRate=dIndex40mmLoadRate;                                   //wei 20150303     IniConfig.dIndex40mmLoadRate-->dIndex40mmLoadRate
            }
            else if(fDiameter==5.6)
            {
                fLoadRate=dIndex56mmLoadRate;                                   //wei 20151005 add 56mm
            }
            else
            {
                if(fDiameter==0)                                                //Steven 20140627 : 避免分母為0
                    fDiameter=3.0;
                fLoadRate=dIndex30mmLoadRate;
            }
        }
        else
        {
            if(bDualForce==true)                                                //Ifor 20191003 : add Die Force 可以自定義Kit直徑
            {
                if(INSTALL_DOUBLE_EP==0)                                        //JerryYang 20210119 : 增加dual EP防呆
                {
                   ShowMyMessage("無安裝dual force,請確認硬體選項");
                }

                if(INSTALL_DOUBLE_EP==DOUBLE_EP_MULTI && TestIF_File.bIndEPSLK==true)         //==> //Eastsun 20260525 INSTALL_DOUBLE_EP_3 整合 begin
                {
                    int k=0;
                    for(int i=0; i<fContactForce->slDieForceOneByOneSLKType->Count; i++)
                    {
                        for(int j=0; j<8; j++)
                        {
                            if(fContactForce->DieForceOneByOneSLKClass[i*8+j]->dDiameter==fDiameter*10)
                            {
                                fInputKGInd=fInputKG;
                                fLoadRateInd=fContactForce->DieForceOneByOneSLKClass[i*8+j]->dLoadRate;
                                if((fInputKGInd-fContactForce->DieForceOneByOneSLKClass[i*8+j]->dContactOffset)>0)
                                    fInputKGIndRel=fInputKGInd-fContactForce->DieForceOneByOneSLKClass[i*8+j]->dContactOffset;
                                dTempInd=(fDiameter*fDiameter*M_PI/4.0*fLoadRateInd);
                                if(dTempInd!=0) fInputFInd=fInputKGIndRel/dTempInd;
                                else            fInputFInd=fMinMPA;
                                fInputMPAInd=fInputFInd/10.197;
                                iResultTemp=((fMaxUnit-fMinUnit)/(fMaxMPA-fMinMPA))*(fInputMPAInd-fMinMPA)+fMinUnit;
                                iResultTemp=CheckRange(iResultTemp, 0, int(fMaxUnit));
                                iResultTemp=iResultTemp*16;
                                if(DeviceForm_File.bUseDieForce==true)
                                    iAPAXDualEPValue[k]=iResultTemp;
                                else
                                    iAPAXDualEPValue[k]=0;
                                k++;
                            }
                        }
                    }
                }
                else
                {
                    for(unsigned int i=0; i<fContactForce->DieForceSLKClass.size(); i++)
                    {
                        double d=fContactForce->DieForceSLKClass[i]->dDiameter/10.0;
                        if(d==fDiameter)
                        {
                            iTag=i;
                        }
                    }

                    if(iTag==-1 || iTag>(int)fContactForce->DieForceSLKClass.size())
                        iTag=0;

                    if((fInputKG-fContactForce->DieForceSLKClass[iTag]->dContactOffset)>0)
                    {
                        fInputKG=fInputKG-fContactForce->DieForceSLKClass[iTag]->dContactOffset;
                    }

                    fLoadRate=fContactForce->DieForceSLKClass[iTag]->dLoadRate;
                }
            }
            else
            {
                for(unsigned int i=0; i<fContactForce->SLKClass.size(); i++)
                {
                    double d=fContactForce->SLKClass[i]->dDiameter/10.0;

                    if(CUSTOMER_CODE==CC_KYEC_LEE)                              //Ifor 20200407 : Fix KYEC 特殊缸徑造成資料異常
                    {
                        if(d==2.8)
                        {
                            d=3.0;
                        }
                        else if(d==5.8)
                        {
                            d=6.0;
                        }
                    }

                    if(d==fDiameter)
                    {
                        if(EP_Install==5)
                        {
                            if(iArm==0)
                                iTag=i;
                            else
                                iTag=i+1;
                        }
                        else
                        {
                            iTag=i;
                        }
                        break;
                    }
                }

                if(iTag==-1 || iTag>(int)fContactForce->SLKClass.size())
                    iTag=0;

                if(CUSTOMER_CODE==CC_ASE_SG &&
                   fContactForce->SLKClass[iTag]->dDiameter==80 &&
                   DeviceForm_File.dPress>240)
                {
                    iTag=iTag+1;
                }

                bool bUseNSKit=false;
                if(CUSTOMER_CODE==CC_KYEC_LEE)
                {
                    switch(TestIF.iTestMode)
                    {
                        case SingleSite:
                        case DualSite:
                            if(CosFunction.bCanUseBias==true && TestIF_File.bNS7000kit==true)
                            {
                                bUseNSKit=true;
                            }
                            else
                            {
                                bUseNSKit=false;
                            }
                            break;
                        case QualSite1X4:
                            if((CosFunction.bCanUseBias==true && TestIF_File.bNS7000kit==true) || TestIF_File.bNS7000CS==true)
                            {
                                bUseNSKit=true;
                            }
                            else
                            {
                                bUseNSKit=false;
                            }
                            break;
                        case QualSite2X2:
                            if((CosFunction.bCanUse2x2Bias==true && TestIF_File.bNS7000kit==true) || TestIF_File.bNS7000CS==true)
                            {
                                bUseNSKit=true;
                            }
                            else
                            {
                                bUseNSKit=false;
                            }
                            break;
                        case DualSite2x1:
                            if(TestIF_File.bNS7000CS==true)
                            {
                                bUseNSKit=true;
                            }
                            else
                            {
                                bUseNSKit=false;
                            }
                            break;
                        case TriSite1X3:
                        case _8Site1X4:
                            if(ATC_SYSTEM>eATC30 && TestIF_File.bNS7000CS==true)
                            {
                                bUseNSKit=true;
                            }
                            else
                            {
                                bUseNSKit=false;
                            }
                            break;
                        case _8Site2X4:
                            if((ATC_SYSTEM>eATC30 && TestIF_File.bNS7000CS==true) ||
                               (USE_16_HEATER!=eht4Heater && TestIF_File.bNS8000CS==true))
                            {
                                bUseNSKit=true;
                            }
                            else
                            {
                                bUseNSKit=false;
                            }
                            break;
                        default :
                            bUseNSKit=false;
                            break;
                    }
                }
                else
                {
                    bUseNSKit=false;
                }

                if(TestIF_File.bNSKitPress || bUseNSKit==true)
                {
                    if((fInputKG-fContactForce->SLKClass[iTag]->dContactOffset_NS)>0)    //kevin 20170807
                    {
                        fInputKG=fInputKG-fContactForce->SLKClass[iTag]->dContactOffset_NS;
                    }
                }
                else
                {
                    if(INSTALL_DOUBLE_EP==DOUBLE_EP_INDIVIAL  && TestIF_File.bIndEPSLK==true)
                    {
                        //Jimmychiu 20230630 : Individual EP No effect contact force offset of 40mm compliance
                        //<==
                        int iSLKIndClassLen=fContactForce->SLKIndClass.size();
                        int iKitDiameterType=0;
                        if(DeviceForm_File.dKitDiameter==2.0)
                        {
                            iKitDiameterType=0;
                        }
                        else if(DeviceForm_File.dKitDiameter==3.0)
                        {
                            iKitDiameterType=1;
                        }
                        else if(DeviceForm_File.dKitDiameter==4.0)
                        {
                            iKitDiameterType=2;
                        }
                        else
                        {
                            iKitDiameterType=-1;
                        }

                        if(iKitDiameterType>=0)
                        {
                            int itag=iKitDiameterType*16;
                            if(itag<iSLKIndClassLen)
                            {
                                if((fInputKG-fContactForce->SLKIndClass[itag]->dContactOffset)>0)
                                {
                                    fInputKG=fInputKG-fContactForce->SLKIndClass[itag]->dContactOffset;
                                }
                            }
                        }
                        //<==
                        //Jimmychiu 20230630 : Individual EP No effect contact force offset of 40mm compliance
                    }
                    else if(INSTALL_DOUBLE_EP==DOUBLE_EP_MULTI && TestIF_File.bIndEPSLK==true)    //==> //Eastsun 20260525 INSTALL_DOUBLE_EP_3 整合 begin
                    {
                        int iSLKIndClassLen=fContactForce->SLKIndClass.size();
                        int iKitDiameterType=0;
                        if(DeviceForm_File.dKitDiameter==2.0)      iKitDiameterType=0;
                        else if(DeviceForm_File.dKitDiameter==3.0) iKitDiameterType=1;
                        else if(DeviceForm_File.dKitDiameter==4.0) iKitDiameterType=2;
                        else                                       iKitDiameterType=-1;
                        if(iKitDiameterType>=0)
                        {
                            int itag=iKitDiameterType*8;
                            if(itag<iSLKIndClassLen)
                            {
                                if((fInputKG-fContactForce->SLKIndClass[itag]->dContactOffset)>0)
                                    fInputKG=fInputKG-fContactForce->SLKIndClass[itag]->dContactOffset;
                            }
                        }
                    }
                    else
                    {
                        if((fInputKG-fContactForce->SLKClass[iTag]->dContactOffset)>0)
                        {
                            fInputKG=fInputKG-fContactForce->SLKClass[iTag]->dContactOffset;
                        }
                    }
                }

                if(bNSKit && bNSKitSwitch &&                                    //kevin 20170804 (Steven) 使用另一種EP 壓力表
                   (TestIF_File.bNSKitPress ||
                    TestIF_File.bNS7000kit ||
                    TestIF_File.bNS7000CS ||
                    TestIF_File.bNS8000CS))                                     //wei 20150303   京元NS浮動頭
                {
                    fLoadRate=fContactForce->SLKClass[iTag]->dLoadRate_NS;
                }
                else if(bUseNSKit==true)
                {
                    fLoadRate=fContactForce->SLKClass[iTag]->dLoadRate_NS;
                }
                else
                {
                    fLoadRate=fContactForce->SLKClass[iTag]->dLoadRate;
                }

                if(CosFunction.bUseLoadCellOffsetByHeater &&
                   LastSet.iTemperature==Tempture_Hot)                          //2014-06-26    Dell    for TSMC 高溫Load cell offset
                {
                    fLoadRate+=fContactForce->SLKClass[iTag]->dHotOffset;
                }
            }
        }

        dTemp=(fDiameter*fDiameter*M_PI/4.0*fLoadRate);
        //if(INSTALL_DOUBLE_EP!=DOUBLE_EP_MULTI)                            //AI(ht9045-v899) 20260610: keep existing 0/1/2 EP fill; mode 3 (Multi EP) uses the 8-channel block below.
        {                                                                       //Eastsun 20260616 上面註解掉
            int k=0;
            for(int i=0; i<fContactForce->slSLKTypeInd->Count; i++)                 //JerryYang 20210413 : 讀取獨立EP offset
            {
                if(INSTALL_DOUBLE_EP==DOUBLE_EP_MULTI && bDualForce==false)                       //==> //Eastsun 20260525 INSTALL_DOUBLE_EP_3 整合 begin
                {
                    for(int j=0; j<8; j++)
                    {
                        double dDiameterCK=fContactForce->SLKIndClass[i*8+j]->dDiameter;
                        if(CUSTOMER_CODE==CC_KYEC_LEE)
                        {
                            if(dDiameterCK==28)      dDiameterCK=30;
                            else if(dDiameterCK==58) dDiameterCK=60;
                        }

                        if(dDiameterCK==fDiameter*10)
                        {
                            fInputKGInd=fInputKG;
                            fLoadRateInd=fContactForce->SLKIndClass[i*8+j]->dLoadRate;
                            if((fInputKGInd-fContactForce->SLKIndClass[i*8+j]->dContactOffset)>0)
                                fInputKGIndRel=fInputKGInd-fContactForce->SLKIndClass[i*8+j]->dContactOffset;
                            dTempInd=(fDiameter*fDiameter*M_PI/4.0*fLoadRateInd);
                            if(dTempInd!=0) fInputFInd=fInputKGIndRel/dTempInd;
                            else            fInputFInd=fMinMPA;
                            fInputMPAInd=fInputFInd/10.197;
                            iResultTemp=((fMaxUnit-fMinUnit)/(fMaxMPA-fMinMPA))*(fInputMPAInd-fMinMPA)+fMinUnit;
                            iResultTemp=CheckRange(iResultTemp, 0, int(fMaxUnit));
                            iResultTemp=iResultTemp*16;
                            iAPAXEPValue[k]=iResultTemp;
                            k++;
                        }
                    }
                    continue;
                }
                if(INSTALL_DOUBLE_EP==DOUBLE_EP_MULTI)                          //Eastsun 20260616 加個保險
                    continue;

                for(int j=0; j<16; j++)
                {
                    bool bDiaMatch;                                                 //==> Eastsun 20260511 F008 整合: Ifor 20200407 Fix KYEC 特殊烏徑造成資料異常
                    if(CUSTOMER_CODE==CC_KYEC_LEE)                                  //Ifor 20200407 : Fix KYEC 特殊烏徑造成資料異常
                    {
                        double dDiameterCK=fContactForce->SLKIndClass[i*16+j]->dDiameter;
                        if(dDiameterCK==28)      dDiameterCK=30;                    //2.8mm 視為 3.0mm
                        else if(dDiameterCK==58) dDiameterCK=60;                    //5.8mm 視為 6.0mm
                        bDiaMatch=(dDiameterCK==fDiameter*10);
                    }
                    else
                    {
                        bDiaMatch=(fContactForce->SLKIndClass[i*16+j]->dDiameter==fDiameter*10);  // 原 c L1473 原條件，非 KYEC 走原總輯
                    }

                    if(bDiaMatch)
                    {
                        fInputKGInd=fInputKG;
                        fLoadRateInd=fContactForce->SLKIndClass[i*16+j]->dLoadRate;
                        if((fInputKGInd-fContactForce->SLKIndClass[i*16+j]->dContactOffset)>0)
                        {
                            fInputKGIndRel=fInputKGInd-fContactForce->SLKIndClass[i*16+j]->dContactOffset;
                        }
                        dTempInd=(fDiameter*fDiameter*M_PI/4.0*fLoadRateInd);
                        if(dTempInd!=0)
                            fInputFInd=fInputKGIndRel/dTempInd;
                        else
                            fInputFInd=fMinMPA;
                        fInputMPAInd=fInputFInd/10.197;
                        iResultTemp=(int)(ChangeToFloatNonPcnt((double)(fMaxUnit-fMinUnit), (double)(fMaxMPA-fMinMPA)))*(fInputMPAInd-fMinMPA)+fMinUnit; //Steven 20260505 : add zero-guard for (fMaxMPA-fMinMPA)
                        iResultTemp=CheckRange(iResultTemp, 0, int(fMaxUnit));
                        iResultTemp=iResultTemp*16;
                        iAPAXEPValue[k]=iResultTemp;
                        k++;
                    }
                }
            }
        }

        //AI(ht9045-v899) 20260526: Multi EP (mode 3) single-force path - fill iAPAXEPValue from SLKIndClass with 8-channel layout (i*8+j). Uses main zero-guard ChangeToFloatNonPcnt.
//        if(INSTALL_DOUBLE_EP==DOUBLE_EP_MULTI && bDualForce==false)           //Eastsun 20260616 註解掉
//        {
//            int kMep=0;
//            int iSLKIndSizeMep=(int)fContactForce->SLKIndClass.size();
//            for(int iMep=0; iMep<fContactForce->slSLKTypeInd->Count; iMep++)
//            {
//                for(int jMep=0; jMep<8; jMep++)
//                {
//                    int idxMep=iMep*8+jMep;
//                    if(idxMep>=iSLKIndSizeMep)
//                        break;
//
//                    double dDiameterMep=fContactForce->SLKIndClass[idxMep]->dDiameter;
//                    if(CUSTOMER_CODE==CC_KYEC_LEE)
//                    {
//                        if(dDiameterMep==28)
//                            dDiameterMep=30;
//                        else if(dDiameterMep==58)
//                            dDiameterMep=60;
//                    }
//
//                    if(dDiameterMep==fDiameter*10)
//                    {
//                        double fInputKGIndMep = fInputKG;
//                        double fLoadRateIndMep = fContactForce->SLKIndClass[idxMep]->dLoadRate;
//                        double fOffsetMep = fContactForce->SLKIndClass[idxMep]->dContactOffset;
//                        double fInputKGIndRelMep = fInputKGIndMep;
//                        if((fInputKGIndMep - fOffsetMep) > 0)
//                            fInputKGIndRelMep = fInputKGIndMep - fOffsetMep;
//                        double dTempIndMep = (fDiameter*fDiameter*M_PI/4.0*fLoadRateIndMep);
//                        double fInputFIndMep = (dTempIndMep!=0) ? (fInputKGIndRelMep/dTempIndMep) : fMinMPA;
//                        double fInputMPAIndMep = fInputFIndMep/10.197;
//                        int iResultTempMep = (int)(ChangeToFloatNonPcnt((double)(fMaxUnit-fMinUnit), (double)(fMaxMPA-fMinMPA)))*(fInputMPAIndMep-fMinMPA)+fMinUnit; //AI(ht9045-v899) 20260610: main zero-guard
//                        iResultTempMep = CheckRange(iResultTempMep, 0, int(fMaxUnit));
//                        iResultTempMep = iResultTempMep*16;
//                        if(kMep<8) iAPAXEPValue[kMep] = iResultTempMep;
//                        kMep++;
//                    }
//                }
//            }
//        }

        if(dTemp!=0)
            fInputF=fInputKG/dTemp;
        else
            fInputF=fMinMPA;
        fInputMPA=fInputF/10.197;

        double dbTransferKg = KpaTransferKG(fInputMPA*1000);                    //Ifor 20150907 :新增顯示目前下壓公斤數與EP轉換公斤數

        if(bDualForce==false)                                                   //kevin 20220225 Die Force  不改資料
        {
            fContact->edSetKg->Text=FormatFloat("0.0000", fInputKG);
            fContact->edTransfer->Text=FormatFloat("0.0000", dbTransferKg/1000);
            fContact->edAirKPA->Text=FormatFloat("0.0000", fInputMPA);          //Steven 20160630 : 在Contact畫面顯示輸出的壓力值
        }
        iResult=(int)(ChangeToFloatNonPcnt((double)(fMaxUnit-fMinUnit), (double)(fMaxMPA-fMinMPA)))*(fInputMPA-fMinMPA)+fMinUnit; //Steven 20260505 : add zero-guard for (fMaxMPA-fMinMPA)
    }
    int iMax = fMaxUnit;                                                        //20111111  Dell
    iResult=CheckRange(iResult, 0, iMax);
    return iResult;
}
//---------------------------------------------------------------------------
bool ADAM_WriteVoltage(double v)
{
    WORD data;
    if(EP_Install)
    {
        if(!bConnectStatus[0])                                                  //Hmy 20170120 add check Adam6024 Connect Status ->
            bConnectStatus[0]=fCheckConnectStatus_ADAM6024(0);                  //Nickliu 20230330 add check statsu

        if(fContactForce->fShow)
        {
            return true;
        }

        int iInputValue=0, iInValue[2]={0, 0};
        if(v<0)
        {
#ifdef DEBUG_TRY_CATCH
            try
            {
#endif
                if(EP_Install==4)                                               //Steven 20141202 : PISO DA
                {
                    ;
                }
                else
                {
                    ADAM_DirectWriteData(0, 0);                                 //Ifor 20150709 ：加入設備位置
                }
                return true;
#ifdef DEBUG_TRY_CATCH
            }
            catch(...)
            {
                MyDBIProcess("Exception", "ADAM_WriteVoltage");
                return false;
            }
#endif
        }
        else
        {
                if(EP_Install!=5)
                {
                    iInputValue=TransformFuntion(v);
                }
                else
                {
                    iInValue[0]=TransformFuntion(v, false, false, 0);
                    iInValue[1]=TransformFuntion(v, false, false, 1);
                }
    #ifdef DEBUG_TRY_CATCH
                try
                {
    #endif
                    if(IsIndependentEPPressureRouteActive()==true)              //AI(ht9045-v899) 20260526: use actual SwMultiEp route before APAX-only output.
                    {
                        TransformFuntion(v, false);
                        APAX_WriteData(false, 0);
                        if(EP_Install==1 || EP_Install==3 || EP_Install==5)     //20111111 Dell //20111217 ChungHung
                        {
                            data=0;
                            data=CheckRange(data, WORD(0), WORD(4095));
                            iWritePA=data;
                            ADAMTCP_WriteReg(Address[0].c_str(), 1, 12, 1, &data);//Ifor 20190129 : add 選擇CH輸出
                        }
                    }
                    else
                    {
                        if(EP_Install==1 ||
                           EP_Install==3 ||                                     //20111217 ChungHung
                           EP_Install==4)                                       //Steven 20141202 : PISO DA
                        {
                            ADAM_DirectWriteData(iInputValue, 0);               //Ifor 20150709 ：加入設備位置
                        }
                        else if(EP_Install==5)
                        {
                            if(TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==0)
                            {
                                ADAM_DirectWriteData(iInValue[0], 0, 10);
                            }
                            else if(TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==1)
                            {
                                ADAM_DirectWriteData(iInValue[1], 0, 11);
                            }
                            else
                            {
                                ADAM_DirectWriteData(iInValue[0], 0, 10);
                                ADAM_DirectWriteData(iInValue[1], 0, 11);
                            }
                        }
                        else if(EP_Install==2)                                  //20111111 Dell
                        {
                            WriteDigital(iInputValue);
                        }

                        if(IsIndependentEPPressureRouteActive()==true)          //AI(ht9045-v899) 20260526: do not clear/write APAX when Multi valve is OFF.
                        {
                            APAX_WriteData(true, 0);
                        }
                        iWritePA=iInputValue;                                   //20111217 ChungHung
                        iAdamOutValue=AdamOutputToPA(iWritePA);
                    }
                    return true;

    #ifdef DEBUG_TRY_CATCH
                }
                catch(...)
                {
                    MyDBIProcess("Exception", "ADAM_WriteVoltage");
                    return false;
                }
#endif
        }
    }
    else
    {
        return false;
    }
}
//------------------------------------------------------------------------------
void ADAM_WriteMaxData(bool bFullForce)                                         //Steven 20241014 : 整合auto height輸出壓力
{
    if(bFullForce)
    {
        if(DeviceForm_File.dKitDiameter<=2.5)                                   //kevin 20170804 (Steven) 20mm Auto Height
        {
            ADAM_DirectWriteData((EP_MAXKPA<=500)?820:455, 0);                  //充100kpa就好
        }
        else
        {
            ADAM_DirectWriteData((EP_MAXKPA<=500)?4095:2275, 0);                //Ifor 20150709 ：加入設備位置
        }
    }
    else
    {
        ADAM_DirectWriteData(0, 0);
    }
}
//------------------------------------------------------------------------------
void ADAM_DirectWriteData(WORD data, int Num, int iAdd)                         //Ifor 20150709 ：加入設備位置 //Ifor 20190104 : add 選擇 ADAM 輸出 CH
{
    int iRet;
    bCanReadData=false;                                                         //20111217 ChungHung
    if(EP_Install)
    {
        if(!bConnectStatus[Num])                                                //Hmy 20170120 add check Adam6024 Connect Status ->
            bConnectStatus[Num]=fCheckConnectStatus_ADAM6024(Num);              //Nickliu 20230330 add check statsu
    }

    if(EP_Install==4)                                                           //Steven 20141202 : PISO DA
    {
        data=CheckRange(data, WORD(0), WORD(4095));
        fAdam6024->WriteAO(data);
    }
    else
    {
        if(IsIndependentEPPressureRouteActive()==true)                          //AI(ht9045-v899) 20260526: route direct writes to APAX only when independent EP path is active.
        {
            if(Num==0)
            {
                int iMultiEPArm=0;                                             //AI(ht9045-v899) 20260526: keep iosetview and production direct writes on the shared APAX route.
                if(INSTALL_DOUBLE_EP==DOUBLE_EP_MULTI)
                {
                    if(iAdd==10)
                        iMultiEPArm=1;
                    else if(iAdd==11)
                        iMultiEPArm=2;
                }
                APAX_WriteData(true, data, iMultiEPArm);
            }
        }
        else
        {
            if(EP_Install==1 || EP_Install==3)                                  //20111111 Dell //20111217 ChungHung
            {
                data=CheckRange(data, WORD(0), WORD(4095));

                if(data==1250 || data==1251 || data==1252)
                    data=1253;                                                  //Ifor 20241030 : EP不會動, 暫時先跳過這三個數字

                iWritePA=data;                                                  //JerryYang 20171030 (wei) fix EP問題
                iRet=ADAMTCP_WriteReg(Address[Num].c_str(), 1, (11+iAdd), 1, &data);//Ifor 20190129 : add 選擇CH輸出
                if(iRet!=0)                                                     //Frank 20170206 (Steven) 確認ADAM連線正常
                {
                    bConnectStatus[Num]=false;
                }

                if(IsIndependentEPPressureRouteActive()==true)                  //AI(ht9045-v899) 20260526: keep normal ADAM output untouched when SwMultiEp is OFF.
                {
                    APAX_WriteData(true, 0);
                }
            }
            else if(EP_Install==5)
            {
                data=CheckRange(data, WORD(0), WORD(4095));
                iWritePA=data;                                                  //JerryYang 20171030 (wei) fix EP問題

                if(iAdd==10)
                {
                    iRet=ADAMTCP_WriteReg(Address[Num].c_str(), 1, 11, 1, &data);
                }
                else if(iAdd==11)
                {
                    iRet=ADAMTCP_WriteReg(Address[Num].c_str(), 1, 12, 1, &data);
                }
                else
                {
                    iRet=ADAMTCP_WriteReg(Address[Num].c_str(), 1, 11, 1, &data);
                    iRet=ADAMTCP_WriteReg(Address[Num].c_str(), 1, 12, 1, &data);
                }

                if(iRet!=0)                                                     //Frank 20170206 (Steven) 確認ADAM連線正常
                {
                    bConnectStatus[Num]=false;
                }
            }
            else if(EP_Install==2)
            {
                data=CheckRange(data, WORD(0), WORD(1022));
                WriteDigital(data);
            }
        }
    }
    bCanReadData=true;                                                          //20111217 ChungHung
}
//---------------------------------------------------------------------------
//20111111 Dell For E/P Digital
const int Digital_10Bit = 10;
void WriteDigital(double v)
{
    int value=v;

    for(int i=0; i<Digital_10Bit; i++)
    {
        SW[SwEP_D0+i].OnOff(value%2);                                           //10進位 轉 2進位
        value=value>>1;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfAdam6024::ClientSocket1Connect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    for(int i=0; i<8; i++)
    {
        cmdBuf[i].status=0;
        memset(cmdBuf[i].cmdBuf, 0x0, 512);
        cmdBuf[i].cmdSize=0;
    }
    bufIdx=0;
}
//---------------------------------------------------------------------------
void __fastcall TfAdam6024::ClientSocket1Error(TObject *Sender,
      TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
    ErrorCode=0;
    Abort();

    try
    {
        ClientSocket1->Close();
    }
    catch(...)
    {
        LogClientSocketExceptionError(ClientSocket1, "ADAM ClientSocket1 Error");
    }
}
//---------------------------------------------------------------------------
void TfAdam6024::WriteAO(double AO1, double AO2, double AO3, double AO4)
{
    AnsiString strBuf, strbyte;
    unsigned hexValue;
    char Buff[24];

    memset(Buff, 0x0, sizeof(Buff));
    Buff[0]=2;
    Buff[5]=15;
    Buff[6]=NetID;  // Net ID (Station number)
    Buff[7]=16;     // Function code: 16 -> Write multiple registers (4XXXX) for AO
    *(unsigned int *) &Buff[8]=0;  // Reference number
    *(unsigned int *) &Buff[10]=swap16(4);  // Word count
    Buff[12]=8;     // Byte count

    hexValue=float2hex(0x35, AO1, 0);
    *(unsigned int *) &Buff[13]=swap16(hexValue);  // AO0 value

    hexValue=float2hex(0x35, AO2, 0);
    *(unsigned int *) &Buff[15]=swap16(hexValue);  // AO1 value

    hexValue=float2hex(0x35, AO3, 0);
    *(unsigned int *) &Buff[17]=swap16(hexValue);  // AO2 value

    hexValue=float2hex(0x35, AO4, 0);
    *(unsigned int *) &Buff[19]=swap16(hexValue);  // AO3 value

    ClientSocket1->Socket->SendBuf(Buff, 21);
}
//---------------------------------------------------------------------------
void TfAdam6024::OpenSocket(AnsiString IP, int Port)
{
    ClientSocket1->Address=IP;
    ClientSocket1->Port=Port;
    ClientSocket1->Open();
}
//---------------------------------------------------------------------------
void TfAdam6024::CloseSocket()
{
    try
    {
        ClientSocket1->Close();
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfAdam6024::CloseSocket");
    }
}
//---------------------------------------------------------------------------
bool ADAM_ReadAIValue(int iNum, int iChannel, double *dmA, double *dDegree)
{
    static bool bEnter=false;
    bool bReturnStatus=false;
    double dValue=0.0;
    char str[256];
    if(bEnter)
    {
        return bReturnStatus;
    }
    bEnter=true;

    if(EP_Install)
    {
        if(bADAM6420Install==false)
        {
            bEnter=false;                                                       //ChungHung 20160315 modify
        }
        try
        {
            if(EP_Install==4)                                                   //Steven 20141202 : PISO DA
            {
                ;
            }
            else
            {
                #ifndef SOFT_SIMULTE
                    dValue=ADAM_ReadVoltage(iNum, iChannel);
                #else                                                           //Hmy 20170930 Add  SOFT_SIMULTE return value
                    dValue=5.87654;
                #endif

                sprintf(str, "%2.3f", dValue);
                double dCurrent_ma = atof(str);
                double dConvertTemperature =0.0;

                if(DewPoint_Hardware_Install==1)                                //-60~+60
                {
                    if(dCurrent_ma<=12.0)
                      dConvertTemperature=-60+(60/8*(dCurrent_ma-4));
                    else if(dCurrent_ma>12.0)
                      dConvertTemperature=60/8*(dCurrent_ma-12);
                }
                else if(DewPoint_Hardware_Install==2)                           //-80~+20
                {
                    if(dCurrent_ma<=12.0)
                      dConvertTemperature=-80+(50/8 *(dCurrent_ma-4));
                    else if(dCurrent_ma>12.0)
                      dConvertTemperature=-30+50/8*(dCurrent_ma-12);
                }
                else                                                            //無安裝
                {
                    dConvertTemperature=9999;
                }
                sprintf(str, "%2.1f", dConvertTemperature);
                dConvertTemperature=atof(str);
                *dmA=dCurrent_ma;
                *dDegree=dConvertTemperature;
                bReturnStatus=true;
                bEnter=false;                                                   //ChungHung 20160315 modify
            }
        }
        catch(...)
        {
            MyDBIProcess("Exception", "ADAM_ReadAIValue");
            bEnter=false;                                                       //ChungHung 20160315 modify
        }
    }
    else
    {
        bEnter=false;                                                           //ChungHung 20160315 modify
        bReturnStatus=false;
    }
    bEnter=false;                                                               //ChungHung 20160315 modify
    return bReturnStatus;
}
//---------------------------------------------------------------------------
void __fastcall TfAdam6024::FormDestroy(TObject *Sender)
{
    try
    {
    }
    catch(...)
    {
        MyDBIProcess("Exception", "TfAdam6024::FormDestroy");
    }
    LogSoftwareOffTime("TfAdam6024, FormDestroy");                              //Steven 20210526 : 紀錄軟體執行時間
}
//---------------------------------------------------------------------------
//Nickliu 20180827 Add APAX Dll Connection-->
bool Open_APAX(char IP[])
{
    //ChungHung 20180906 modify delete start
//    iRet = MOD_AddTcpClientConnect(Address,           // remote server IP
//                                                    100,          // scan interval (ms)
//                                                    3000,         // connection timeout (ms)
//                                                    100,          // transaction timeout (ms)
//                                                    NULL,
//                                                    NULL,
//                                                    NULL,
//                                                    &ulClientHandle);
    //ChungHung 20180906 modify delete end

    if(EP_Install)
    {
//        if (iEPAPAXMode == 1)
        {
            strcpy(cAddress, IP);

            //ChungHung 20180906 modify for APAX start
            if(MODERR_SUCCESS == MOD_Initialize())
            {
                if(MODERR_SUCCESS == MOD_AddTcpClientConnect(cAddress,          // remote server IP
                                                    100,            // scan interval (ms)
                                                    3000,           // connection timeout (ms)
                                                    100,            // transaction timeout (ms)
                                                    ConnectTcpServerCompletedEventHandler,
                                                    DisconnectTcpServerCompletedEventHandler,
                                                    NULL,
                                                    &ulClientHandle))
                {
                    if(MODERR_SUCCESS == MOD_StartTcpClient())
                    {
                        MOD_SetTcpClientPriority(THREAD_PRIORITY_HIGHEST);
                    }
                    else
                    {
                        return false;
                    }
                }
            }
            return true;
        }
    }
    else
    {
        return false;
    }
}
//---------------------------------------------------------------------------
void ConnectTcpServerCompletedEventHandler(long lResult, char *i_szIp, void *i_Param)
{
    //char str[100];
    AnsiString Str;
    //ChungHung 20180907 modify for APAX Log
    Str.sprintf("APAX Connect to '%s' result = %d\n", i_szIp, lResult);
//    WriteApaxLogs(Str);
    NewRecordProcess("", Str);
    //ChungHung 20180907 modify for APAX Log
    if(lResult!=0)
    {
//        bConnectStatus = false;
        //sprintf(str,"APAX Connect to '%s' result = %d\n", i_szIp, lResult);
        //ShowMyMessage("APAX Connect Fail!");
        bAPAXConnectFileAlarm=true;
    }
    else
    {
        bAPAXConnectFileAlarm=false;
        bApaxWriteFinish=true;
        bApaxReadFinish =true;
    }
}
//---------------------------------------------------------------------------
void DisconnectTcpServerCompletedEventHandler(long lResult, char *i_szIp, void *i_Param)
{
    AnsiString Str;
    Str.sprintf("APAX Disconnect from '%s' result = %d\n", i_szIp, lResult);    //ChungHung 20180907 modify for APAX Log
    NewRecordProcess("", Str);
}
//---------------------------------------------------------------------------
void ClientWriteReg_1_8_Handler(long i_lResult, void *i_Param)
{
    bApaxWriteFinish=true;
}
//---------------------------------------------------------------------------
void APAX_WriteData(bool bDir, WORD wdata, int iArm)
{
    if(INSTALL_DOUBLE_EP<2)
    {
        return;
    }

    if(INSTALL_DOUBLE_EP==DOUBLE_EP_MULTI && IsMultiEPPressureRouteActive()==false) //AI(ht9045-v899) 20260526: SwMultiEp OFF means common EP route, so skip Multi APAX writes.
    {
        return;
    }

    //AI(ht9045-v899) 20260504: full port V874.3 INSTALL_DOUBLE_EP==DOUBLE_EP_MULTI (撤回 20260430 砍半版)。
    //   Hardware: 1x APAX-5070 (Address[2]=172.16.8.112) + 2x APAX-5028 = S0/S1 separated writes.
    //   ch 對應 (參 SPEC-V899-MultiEP-FullPort, ADR-0004):
    //     wData2[0..7]  Arm1: Dual1, Dual2, Site1, Site2, Site3, Site4, Dual3, Dual4
    //     wData2[8..15] Arm2: 同上
    //   ShuttleMode==1 + Sel==0 只更新 Arm1；Sel==1 只更新 Arm2；Mode==0 同時更新。
    //   S0 Arm1: ADAMTCP_WriteReg(Address[2], 1, 1, 8, ...); S1 Arm2: start 33.
    if(INSTALL_DOUBLE_EP==DOUBLE_EP_MULTI)
    {
        bApaxWriteFinish=false;
        int iRetMep, iMep, kMep;
        WORD wData2Mep[16];
        WORD wDataArm1Mep[8];                                                  //AI(ht9045-v899) 20260526: APAX-5028 S0 controls Arm1 AO0-AO7.
        WORD wDataArm2Mep[8];                                                  //AI(ht9045-v899) 20260526: APAX-5028 S1 controls Arm2 AO0-AO7.
        AnsiString StrMep="", str1Mep="", str2Mep="";
        for(iMep=0; iMep<16; iMep++) wData2Mep[iMep]=0;
        for(iMep=0; iMep<8; iMep++)
        {
            wDataArm1Mep[iMep]=0;
            wDataArm2Mep[iMep]=0;
        }

        bool bWriteArm1Mep=((TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==0) || TestIF_File.iShuttleMode==0); //AI(ht9045-v899) 20260526: allow direct writes to select Arm1/Arm2 via iArm while keeping production shuttle selection.
        bool bWriteArm2Mep=((TestIF_File.iShuttleMode==1 && TestIF_File.iShuttle_Sel==1) || TestIF_File.iShuttleMode==0);
        if(iArm==1)
        {
            bWriteArm1Mep=true;
            bWriteArm2Mep=false;
        }
        else if(iArm==2)
        {
            bWriteArm1Mep=false;
            bWriteArm2Mep=true;
        }

        if(bWriteArm1Mep)
        {
            wData2Mep[0]=iAPAXDualEPValue[0];
            wData2Mep[1]=iAPAXDualEPValue[1];
            wData2Mep[2]=iAPAXEPValue[0];
            wData2Mep[3]=iAPAXEPValue[1];
            wData2Mep[4]=iAPAXEPValue[2];
            wData2Mep[5]=iAPAXEPValue[3];
            wData2Mep[6]=iAPAXDualEPValue[2];
            wData2Mep[7]=iAPAXDualEPValue[3];
            if(bDir==true)
            {
                //AI(ht9045-v899) 20260526: direct Multi EP writes follow the installed first four Arm1 AO channels.
                wData2Mep[0]=wdata*16;
                wData2Mep[1]=wdata*16;
                wData2Mep[2]=wdata*16;
                wData2Mep[3]=wdata*16;
                wData2Mep[4]=0;
                wData2Mep[5]=0;
                wData2Mep[6]=0;
                wData2Mep[7]=0;
            }
        }
        else
        {
            for(iMep=0; iMep<8; iMep++) wData2Mep[iMep]=0*16;
        }

        if(bWriteArm2Mep)
        {
            wData2Mep[8]=iAPAXDualEPValue[4];
            wData2Mep[9]=iAPAXDualEPValue[5];
            wData2Mep[10]=iAPAXEPValue[4];
            wData2Mep[11]=iAPAXEPValue[5];
            wData2Mep[12]=iAPAXEPValue[6];
            wData2Mep[13]=iAPAXEPValue[7];
            wData2Mep[14]=iAPAXDualEPValue[6];
            wData2Mep[15]=iAPAXDualEPValue[7];
            if(bDir==true)
            {
                //AI(ht9045-v899) 20260526: direct Multi EP writes follow the installed first four Arm2 AO channels.
                wData2Mep[8]=wdata*16;
                wData2Mep[9]=wdata*16;
                wData2Mep[10]=wdata*16;
                wData2Mep[11]=wdata*16;
                wData2Mep[12]=0;
                wData2Mep[13]=0;
                wData2Mep[14]=0;
                wData2Mep[15]=0;
            }
        }
        else
        {
            for(iMep=8; iMep<16; iMep++) wData2Mep[iMep]=0;
        }

        for(iMep=0; iMep<8; iMep++)                                             //AI(ht9045-v899) 20260526: split logical 16ch buffer into APAX S0/S1 register blocks.
        {
            wDataArm1Mep[iMep]=wData2Mep[iMep];
            wDataArm2Mep[iMep]=wData2Mep[iMep+8];
        }

        for(iMep=0; iMep<10; iMep++)                                            //retry 10 times (same as V874 style)
        {
            iRetMep=ADAMTCP_WriteReg(Address[2].c_str(), 1, 1, 8, wDataArm1Mep);
            if(iRetMep==0 || iRetMep==817)
            {
                break;
            }
            else
            {
                #ifndef SOFT_SIMULTE
                if(iRetMep!=0)
                {
                    str2Mep="";
                    for(kMep=0; kMep<8; kMep++) { str1Mep.sprintf("%d", wDataArm1Mep[kMep]); str2Mep+=str1Mep; }
                    StrMep.sprintf("APAX MEP3 ARM1 S0 SEND DATA FAIL %d, %s \n", iRetMep, str2Mep);
                    NewRecordProcess("", StrMep);
                    Close_ADAM_6024();
                    Open_ADAM_6024();
                }
                #endif
            }
        }

        for(iMep=0; iMep<10; iMep++)                                            //AI(ht9045-v899) 20260526: write Arm2 to APAX-5028 S1 Modbus block.
        {
            iRetMep=ADAMTCP_WriteReg(Address[2].c_str(), 1, 33, 8, wDataArm2Mep);
            if(iRetMep==0 || iRetMep==817)
            {
                break;
            }
            else
            {
                #ifndef SOFT_SIMULTE
                if(iRetMep!=0)
                {
                    str2Mep="";
                    for(kMep=0; kMep<8; kMep++) { str1Mep.sprintf("%d", wDataArm2Mep[kMep]); str2Mep+=str1Mep; }
                    StrMep.sprintf("APAX MEP3 ARM2 S1 SEND DATA FAIL %d, %s \n", iRetMep, str2Mep);
                    NewRecordProcess("", StrMep);
                    Close_ADAM_6024();
                    Open_ADAM_6024();
                }
                #endif
            }
        }
        return;
    }

    bApaxWriteFinish=false;

    int iRet, iEpindex=0, i;
    int data;
    unsigned char byData[16]={0};
    AnsiString Str="", str1="", str2="";
    WORD wData[16];
    iEpindex=0;

    for(i=0; i<16; i++)
    {
        wData[i]=0;
    }

    for(i=0; i<16; i+=2)                                                        //Output all AO ch# = 0x8000
    {
        data=iAPAXEPValue[iEpindex];
        byData[i]  =(data>>8)&0xFF;                                             //High byte
        byData[i+1]=data&0xFF;                                                  //Low byte
        if(i%2==0)
            iEpindex++;
    }

    if(bDir==true)
    {
        if(TestIF_File.iTestMode==DualSite)
        {
            if(iIndEPCnt==4 ||                                                  //JerryYang 20210413 : Add 4組獨立EP版本
                iIndEPCnt==8)                                                   //RogerYang 20260603 : Add 8EP
            {
                wData[0]=wdata*16;
                wData[1]=wdata*16;
                wData[4]=wdata*16;
                wData[5]=wdata*16;
            }
//            else if(iIndEPCnt==8)                                               //RogerYang 20260603 : Add 8EP
//            {
//                for(int j=0; j<8; j++)
//                {
//                    wData[j]=wdata*16;
//                }
//            }
            else
            {
                wData[0]=wdata*16;
                wData[1]=wdata*16;
            }
        }
        else
        {
            if(iIndEPCnt==4)                                                    //JerryYang 20210413 : Add 4組獨立EP版本
            {
                wData[0]=wdata*16;
                wData[1]=wdata*16;
                wData[4]=wdata*16;
                wData[5]=wdata*16;
            }
            else if(iIndEPCnt==8)                                               //RogerYang 20260603 : Add 8EP
            {
                for(int j=0; j<8; j++)
                {
                    wData[j]=wdata*16;
                }
            }
            else
            {
                for(i=0; i<8; i++)
                {
                   wData[i]=wdata*16;
                }
            }
        }
    }
    else
    {
        if(TestIF_File.iTestMode==DualSite)
        {
            if(iIndEPCnt==4 ||                                                  //JerryYang 20210413 : Add 4組獨立EP版本
                iIndEPCnt==8)                                                   //RogerYang 20260603 : Add 8EP
            {
                wData[0]=iAPAXEPValue[0];
                wData[1]=iAPAXEPValue[1];
                wData[4]=iAPAXEPValue[4];
                wData[5]=iAPAXEPValue[5];
            }
//            else if(iIndEPCnt==8)                                             //RogerYang 20260603 : Add 8EP
//            {
//                for(int j=0; j<8; j++)
//                {
//                    wData[j]=iAPAXEPValue[j];
//                }
//            }
            else
            {
                wData[0]=iAPAXEPValue[0];
                wData[1]=iAPAXEPValue[1];
            }
        }
        else
        {
            if(iIndEPCnt==4)                                                    //JerryYang 20210413 : Add 4組獨立EP版本
            {
                for(i=0; i<4; i++)
                {
                   wData[i]=iAPAXEPValue[i];
                }
            }
            else if(iIndEPCnt==8)                                               //RogerYang 20260603 : Add 8EP
            {
                for(int j=0; j<8; j++)
                {
                    wData[j]=iAPAXEPValue[j];
                }
            }
            else
            {
                for(i=0; i<8; i++)
                {
                   wData[i]=iAPAXEPValue[i];
                }
            }
        }
    }

    if(iArm==0 || iArm==2 ||
        iIndEPCnt==4 || iIndEPCnt==8)                                           //RogerYang 20260603 : Add 8EP //JerryYang 20210413 : Add 4組獨立EP版本
    {
        for(i=0; i<10; i++)                                                     //JerryYang 20210622 : 偶發EP error, 改成retry 10次
        {
            iRet=ADAMTCP_WriteReg(Address[2].c_str(), 1, 1, 8, wData);
            if(iRet==0 || iRet==817)
            {
                break;
            }
            else
            {
                #ifndef SOFT_SIMULTE
//                bConnectStatus=false;
                if(iRet!=0)
                {
                    str2="";                                                    //ChungHung 20180907 modify for APAX Log
                    for(i=0; i<16; i++)
                    {
                        str1.sprintf("%d", wData[i]);
                        str2+=str1;
                    }
                    Str.sprintf("APAX SEND DATA FAIL %d, %s \n" , iRet, str2);
                    NewRecordProcess("", Str);
                    Close_ADAM_6024();
                    Open_ADAM_6024();                                           //Jimmychiu 20230804 : 整合全部連線檢查
                }
                #endif
            }
        }
    }

    if(iIndEPCnt==16)                                                           //JerryYang 20210413 : 4組獨立EP版本不用進來
    {
        for(i=0; i<16; i++)
        {
            wData[i]=0;
        }

        if(bDir==true)
        {
            if(TestIF_File.iTestMode==DualSite)
            {
                wData[0]=wdata*16;
                wData[1]=wdata*16;
            }
            else
            {
                for(i=0; i<8; i++)
                {
                   wData[i]=wdata*16;
                }
            }
        }
        else
        {
            if(TestIF_File.iTestMode==DualSite)
            {
                wData[0]=iAPAXEPValue[8];
                wData[1]=iAPAXEPValue[9];
            }
            else
            {
                for(i=0; i<8; i++)
                {
                   wData[i]=iAPAXEPValue[i+8];
                }
            }
        }

        if(iArm==0 || iArm==1)
        {
            for(i=0; i<10; i++)                                                 //JerryYang 20210622 : 偶發EP error, 改成retry 10次
            {
                iRet=ADAMTCP_WriteReg(Address[2].c_str(), 1, 33, 8, wData);
                if(iRet==0 || iRet==817)
                {
                    break;
                }
                else
                {
                    #ifndef SOFT_SIMULTE
//                    bConnectStatus = false;
                    if(iRet!=0)
                    {
                        str2="";                                                //ChungHung 20180907 modify for APAX Log
                        for(i=0; i<16; i++)
                        {
                            str1.sprintf("%d", wData[i]);
                            str2+=str1;
                        }
                        Str.sprintf("APAX SEND DATA FAIL %d, %s \n" , iRet, str2);
                        NewRecordProcess("", Str);
                        Close_ADAM_6024();
                        Open_ADAM_6024();                                       //Jimmychiu 20230804 : 整合全部連線檢查
                    }
                    #endif
                }
            }
        }
    }
}
//---------------------------------------------------------------------------
void ADAM_ReturnValueCheck(bool bHome)                                          //wei 20220309 Add EP Voltage Error Alarm
{
    #ifndef SOFT_SIMULTE
    double dReadVoltage=0.0;
    int iReadPA=0;
    static int iCount=0;
    AnsiString Str;

    if(EP_Install==3 || EP_Install==5)                                          //JerryYang20220701:舊板本EP不支援
    {
        if(IniConfig.bD26EnableEncodeShow ||
           IniConfig.bD24EnableEPCheckFuntion ||
           IniConfig.bD26EnableEPEncoderRange ||
           fContact->fShow ||
           bHome)
        {
            static int iEPVoltageErrorCount[2]={0, 0};                          //yunghsin 20220303 Add EP Voltage Error Alarm  ==>
            for(int i=0; i<2; i++)
            {
                if(EP_Install==5)
                {
                    if(i==0)
                    {
                        iReadPA=ADAM_ReadPA(&dReadVoltage, 0);
                        if(fContact->fShow)
                            fContact->lblReadEP->Caption=AnsiString("Read=")+AnsiString(GetFloatFormatString(iReadPA, 3, 2));  //20111217 ChungHung
                    }
                    else
                    {
                        iReadPA=ADAM_ReadPA(&dReadVoltage, 1);
                        if(fContact->fShow)
                            fContact->lblReadEP2->Caption=AnsiString("Read=")+AnsiString(GetFloatFormatString(iReadPA, 3, 2));  //20111217 ChungHung
                    }
                }
                else
                {
                    if(i==0)
                    {
                        iReadPA=ADAM_ReadPA(&dReadVoltage);
                        if(fContact->fShow)
                            fContact->lblReadEP->Caption=AnsiString("Read=")+AnsiString(GetFloatFormatString(iReadPA, 3, 2));  //20111217 ChungHung
                    }
                    else
                    {
                        if(INSTALL_DOUBLE_EP!=1)                                //Steven 20220324 : ==0 --> !=1
                            continue;

                        iReadPA=ADAM_ReadPA(&dReadVoltage, 2);                  //20111217 ChungHung
                        if(fContact->fShow)
                            fContact->lblDieForceEP->Caption=AnsiString("Read=")+AnsiString(GetFloatFormatString(iReadPA, 3, 2));  //20111217 ChungHung
                    }
                }

                if((dReadVoltage<0.8 || dReadVoltage>5.2) &&                    //EP Controller Normal Voltage DC 1~5V
                   CUSTOMER_CODE!=CC_GIGAS)                                     //Jimmychiu 20241222 : 工程師Ben要求關閉EP數值異常檢測
                {
                    iEPVoltageErrorCount[i]++;
                    if(iEPVoltageErrorCount[i]>100 || bHome)                    //100 sec
                    {
                        Str.sprintf("dReadVoltage=%f", dReadVoltage);
                        if(i==0)
                            ShowErrorMessage("WAR16322", K_RETRY, MMSystem, false, Str);
                        else
                            ShowErrorMessage("WAR16323", K_RETRY, MMSystem, false, Str);

                        if(bHome)
                            fAllMotorHome=false;
                        else
                            iEPVoltageErrorCount[i]=0;
                    }
                }
            }
        }
        else
        {
            iCount++;
            if(iCount>60)                                                       //Sam 20220311 : 60s 讀一次避免 Adam EP 睡著
            {
                iCount=0;
                iReadPA=ADAM_ReadPA(&dReadVoltage);
            }
        }
    }
    #endif
}
//---------------------------------------------------------------------------
//Nickliu 20230314 Add Check Adam FW Is New-->
AnsiString GetModuleName(int Num)
{
    char szSend[128], szRecv[128];
    memset(szRecv, '\0', sizeof(szRecv));
    memset(szSend, '\0', sizeof(szSend));
    int iRet=0;
    strcpy(szSend, "$01M\r");                                                   //Get module Name Command
    AnsiString sTemp="";
    AnsiString recvStr="";
    if(ADAMTCP_UDPOpen(1000, 1000)==0)
    {
        iRet=ADAMTCP_SendReceive6KUDPCmd(Address[Num].c_str(), szSend, szRecv);
        if(iRet==0)
        {
            if(szRecv[0]=='!')
            {
                AnsiString recvStr=szRecv;
                sTemp=recvStr.SubString(3, recvStr.Length());
            }
        }
    }
    ADAMTCP_UDPClose();
    return sTemp;
}
//---------------------------------------------------------------------------
//Nickliu 20230314 Add Check Fireware Name
AnsiString GetFirmwareName(int Num)                                             //Address[Num].c_str()
{
    char szSend[128], szRecv[128];
    memset(szRecv, '\0', sizeof(szRecv));
    memset(szSend, '\0', sizeof(szSend));
    strcpy(szSend, "$01F\r");                                                   //Get FW Version Command
    AnsiString sTemp="";
    AnsiString recvStr="";
    if(ADAMTCP_UDPOpen(1000, 1000)==0)
    {
        int iRet=ADAMTCP_SendReceive6KUDPCmd(Address[Num].c_str(), szSend, szRecv);
        if(iRet==0)
        {
            if(szRecv[0]=='!')
            {
                AnsiString recvStr=szRecv;
                sTemp=recvStr.SubString(5, recvStr.Length()-3);
            }
        }
    }
    ADAMTCP_UDPClose();
    return sTemp;
}
//Nickliu 20230314 Add Check Fireware Name
//---------------------------------------------------------------------------
//Nickliu 20230314 Add Get Module All Connection Count
int GetModuleConnectionCount(int Num)
{
    char szSend[128], szRecv[128];
    memset(szRecv, '\0', sizeof(szRecv));
    memset(szSend, '\0', sizeof(szSend));
    strcpy(szSend, "%01GETMBTCPCN\r");
    //int count=0;
    int count=-1;   //Nickliu 20230323 add .....
    int iRef=-1;
    if(ADAMTCP_UDPOpen(1000, 1000) == 0)
    {
        if(ADAMTCP_SendReceive6KUDPCmd(Address[Num].c_str(), szSend, szRecv)==0)
        {
            if(szRecv[0]=='!')
            {
                count=(int)(szRecv[2]-'0');
                iRef=count;
            }
            else
            {
                iRef=-1;   //Format Error
            }
        }
    }
    ADAMTCP_UDPClose();
    return iRef;
}
//Nickliu 20230314 Add Get Module All Connection Count
//---------------------------------------------------------------------------
//Nickliu 20230314 Add Clear All Connection
bool ClearAllConnection(int Num)
{
    char szSend[128], szRecv[128];
    memset(szRecv, '\0', sizeof(szRecv));
    memset(szSend, '\0', sizeof(szSend));
    strcpy(szSend, "%01GETMBTCPCN\r");
    bool bRef=false;
    if(ADAMTCP_UDPOpen(1000, 1000)==0)
    {
        if(ADAMTCP_SendReceive6KUDPCmd(Address[Num].c_str(), szSend, szRecv)==0)
        {
            if(szRecv[0]=='!')
            {
                bRef=true;
            }
        }
    }
    ADAMTCP_UDPClose();
    return bRef;
}
//Nickliu 20230314 Add Clear All Connection
//---------------------------------------------------------------------------
//Nickliu 20230314 Add Read Host Idle Time
int GetModuleHostIdleTime(int Num)
{
    char szSend[128], szRecv[128];
    memset(szRecv, '\0', sizeof(szRecv));
    memset(szSend, '\0', sizeof(szSend));
    strcpy(szSend, "%01GETMBTCPCN\r");
    int iRef=-1;
    int hostIdleTime=0;
    if(ADAMTCP_UDPOpen(1000, 1000)==0)
    {
        if(ADAMTCP_GetHostIdleTime(Address[Num].c_str(), &hostIdleTime)==ADAMTCP_NoError)
        {
            iRef=hostIdleTime;
        }
    }
    ADAMTCP_UDPClose();
    return iRef;
}
//Nickliu 20230314 Add Read Host Idle Time
//---------------------------------------------------------------------------
//Nickliu 20230314 Add Set Host Idle Time
bool SetModuleHostIdleTime(int ihostIdleTime)
{
    //Set HostIdleTime
//    char szSend[128], szRecv[128];
//    memset ( szRecv , '\0' , sizeof(szRecv));
//    memset ( szSend , '\0' , sizeof(szSend));
//    strcpy(szSend, "%01GETMBTCPCN\r");
    bool bRef=false;
//    if (ADAMTCP_UDPOpen(1000, 1000) == 0)
//    {
//        if (ADAMTCP_SetHostIdleTime(Address[Num].c_str(), ihostIdleTime) == ADAMTCP_NoError)
//        {
//            bRef = true;
//        }
//    }
//    ADAMTCP_UDPClose();
    return bRef;
}
//Nickliu 20230314 Add Set Host Idle Time
//---------------------------------------------------------------------------
//Nickliu 20230314 Add Check Module Name
bool fCheckModuleName_ADAM6024(int Num)   //Nickliu 20230314 Add Check Adam Module Name
{
    bool bRet=true;
    #ifndef SOFT_SIMULTE
    AnsiString sAdamModuleName="";
    sAdamModuleName=GetModuleName(Num);
    if(sAdamModuleName=="16024-D")    //ADAM6024
    {
        bRet=true;
    }
    else
    {
        bRet=false;
    }
    #endif
    return bRet;
}
//Nickliu 20230314 Add Check Module Name
//---------------------------------------------------------------------------
//Nickliu 20230314 Add Check Adam FW Is New-->
bool fCheckModuleFWISNew_ADAM6024(int Num)
{
    bool bRet=false;
    #ifndef SOFT_SIMULTE
    AnsiString sAdamModuleName="";
    AnsiString sFWName="", sFWSubVersion="";
    int iSubVersion=0;
    //Version : 6.01 B21
    //B21 Is Version
    sAdamModuleName =GetFirmwareName(Num);                                      //6.01 B21
    sFWName         =sAdamModuleName.SubString(1, 4);
    sFWSubVersion   =sAdamModuleName.SubString(sAdamModuleName.Length()-1, sAdamModuleName.Length());
    iSubVersion     =atoi(sFWSubVersion.c_str());
    if(sFWName=="6.01" && iSubVersion>0)
    {
        if(iSubVersion>=21)
            bRet=true;
        else
            bRet=false;
    }
    else
    {
       bRet=false;
    }
    #endif
    return bRet;
}
//Nickliu 20230314 Add Check Adam FW Is New--<
//---------------------------------------------------------------------------
bool fCheckConnectStatus_ADAM6024(int Num)                                      //Hmy 20170120 add check Adam6024 Status
{
    // bConnectStatus = false;                                                  //Nickliu 20230330 add check statsu mark
    bool bRef = false;
    #ifndef SOFT_SIMULTE
    int iRetVal =-1;
    AnsiString Msg;

    if(bADAM6024FWIsNew[Num]==true)                                             //Nickliu 20230314 Add Check Adam FW Is New
    {
        if(fCheckModuleName_ADAM6024(Num)==false)                               //Nickliu 20230314 Add Check Adam Module Name
        {
            ShowMyMessage("ADAM Read ModuleName Fail, Please Check Lan Cable");
            bRef=false;                                                         //add return value no need retry connection
            return bRef; //Nickliu 202303                                       //Nickliu 20230330 add check statsu
        }
        else
        {
            if(ClearAllConnection(Num)==false)                                  //Nickliu 20230316 add Adam Clear All Connect
            {
                ShowMyMessage("Clear ADAM Connection Fail!");
                bRef=false;                                                     //Nickliu 20230330 add check statsu
                return bRef;
            }

            if(GetModuleConnectionCount(Num)>=8)                                //Nickliu 20230321 add check connect count
            {                                                                   //connection over 8 and clear fail
                ShowMyMessage("Adam Clear Fail, Please Check Network Cable or IP address");
                bRef=false;                                                     //Nickliu 20230330 add check statsu
                return bRef;
            }
        }
        ADAMTCP_Disconnect();                                                   //Ifor 20230617 : 修正ADAM6024新版韌體才做斷線重連
        iRetVal=ADAMTCP_Connect(Address[Num].c_str(), DEFAULT_PORT,iConnectionTimeout, iSendTimeout, iReceiveTimeout);
        if(iRetVal<0)
        {
            Msg.sprintf("ADAMTCP Connect Fail..., Error Code:%d", iRetVal);
            ShowMyMessage(Msg);
            ADAMTCP_Close();
            MySleepEx(50, false);
            bRef=false;
            return bRef;                                                        //Nickliu 20230330 add check statsu
        }
        else
        {
            bRef=true;                                                          //Nickliu 20230330 add check statsu
            return bRef;                                                        //Nickliu 20230330 add check statsu
        }
    }
    else
    {
        bRef=true;                                                              //Nickliu 20230330 add check statsu
        return bRef;                                                            //Nickliu 20230330 add check statsu
    }
    #else
    bRef=true;                                                                  //Nickliu 20230330 add check statsu
    return bRef;                                                                //Nickliu 20230330 add check statsu
    #endif
}
//------------------------------------------------------------------------------
void EPSwitchOnOff(int iArm)                                                    // 0:全關, 1:Arm1開, 2:Arm2開, 3:全開 //Steven 20250401 : 整合EP開關
{
    if(iArm==eEPSwOff)
    {
        EpSwitch(false, false);
    }
    else if(TestIF_File.bArm1PickPlaceArm2Test==true)
    {
        EpSwitch(true, true);
    }
    else if(IniConfig.bD30EnableSiteModeSelect &&
            TestIF.iShuttleMode==1)                                             //JerryYang 20240111 : add
    {
        if(TestIF.iShuttle_Sel==0)                                              //Front Arm Only
        {
            EpSwitch(true, false);
        }
        else if(TestIF.iShuttle_Sel==1)                                         //Rear Arm Only
        {
            EpSwitch(false, true);
        }
    }
    else
    {
        if(iArm==eEPSwOff)
            EpSwitch(false, false);
        else if(iArm==eEPSwArm1)
            EpSwitch(true, false);
        else if(iArm==eEPSwArm2)
            EpSwitch(false, true);
        else
            EpSwitch(true, true);
    }
}
//------------------------------------------------------------------------------
void EpSwitch(bool Arm1, bool Arm2)                                             //Steven 20250401 : 整合EP開關
{
    if(fiosetview->fShow==true)                                                 //IO畫面
    {
        if(SW[SwEpArm1].Enable==true)                                           //Steven 20110708
        {
            if(FrontTestHeadHasIC())                                            //Index 1 有IC的話,狀態不可以改變
                SW[SwEpArm1].OnOff(Arm1);
        }

        if(SW[SwEpArm2].Enable==true)                                           //Index 2 有IC的話,狀態不可以改變
        {
            if(RearTestHeadHasIC())
                SW[SwEpArm2].OnOff(Arm2);
        }

        if(SW[SwIndEpArm1].Enable==true)                                        //Steven 20110708
        {
            if(FrontTestHeadHasIC())                                            //Index 1 有IC的話,狀態不可以改變
                SW[SwIndEpArm1].OnOff(Arm1);
        }

        if(SW[SwIndEpArm2].Enable==true)                                        //Index 2 有IC的話,狀態不可以改變
        {
            if(RearTestHeadHasIC())
                SW[SwIndEpArm2].OnOff(Arm2);
        }
    }
    else
    {
        if(SW[SwEpArm1].Enable==true)                                           //Steven 20110708
        {
            SW[SwEpArm1].OnOff(Arm1);
        }

        if(SW[SwEpArm2].Enable==true)
        {
            SW[SwEpArm2].OnOff(Arm2);
        }

        if(SW[SwIndEpArm1].Enable==true)                                        //Steven 20110708
        {
            SW[SwIndEpArm1].OnOff(Arm1);
        }

        if(SW[SwIndEpArm2].Enable==true)                                        //Index 2 有IC的話,狀態不可以改變
        {
            SW[SwIndEpArm2].OnOff(Arm2);
        }
    }
}
//---------------------------------------------------------------------------
